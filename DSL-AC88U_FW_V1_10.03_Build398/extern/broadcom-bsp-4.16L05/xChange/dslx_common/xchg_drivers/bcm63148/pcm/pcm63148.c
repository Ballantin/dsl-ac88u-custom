/****************************************************************************
*
*  Copyright (c) 2013 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*    Filename: pcm63148.c
*
****************************************************************************
*
*    Description:  PCM device driver for BCM63148
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

/*
** Always include this file before any other include files
** to avoid conflicts with remapped printf command
*/
#include <kernelMainWrapper.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/spinlock.h>

#include <pmc_drv.h>
#include <pmc_apm.h>
#include <BPCM.h>
#include <bcm_pinmux.h>

#include <bcmChip.h>
#include <xchgAssert.h>
#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <xdrvCache.h>
#include <bosSleep.h>
#include <xchg_dma.h>
#include <xchg_pcm.h>
#include <apm.h>
#include "pcm63148.h"
#include "dma63148.h"

/* -- Constants & defines -- */

#define NTR_OUT_DEBUG   0     /* Routes NTR signal to output GPIO */
#define NTR_REF_CLK_KHZ 256   /* 256Khz NTR Ref clock present   */

#define PCM_NTR_IN_CTRL_SHIFT             29 /* shift for PCM_NTR_IN clock routing field */
#define PCM_NTR_IN_CTRL_SEL_DECT_NTR      3  /* Selection for routing DECT refclk as PCM_NTR_IN */
#define PCM_NTR_IN_CTRL_SEL_VDSL_NTR_0    2  /* Selection for routing VDSL_NTR[0] as PCM_NTR_IN */
#define PCM_NTR_IN_CTRL_SEL_VDSL_NTR_1    1  /* Selection for routing VDSL_NTR[1] as PCM_NTR_IN */
#define PCM_NTR_IN_CTRL_SEL_VDSL_GPIO     0  /* Selection for routing NTR_PULES_IN as PCM_NTR_IN */

/* PCM NCO Settings */
#define P_NCO_FCW_MISC     0xa7c5ac47  /* 0xa7c5ac47 for DPLL */
#define P_PCM_NCO_SHIFT    0x0         /* 0x0 for DPLL, 0x1 for MISC */
#define P_PCM_NCO_MUX_CNTL_DPLL 0x2    /* 0x2 for DPLL */
#define P_PCM_NCO_MUX_CNTL_MISC 0x3    /* 0x3 for MISC */

/* DPLL lock settings */
#define DPLL_LOCK_WAIT                 1           /* Whether to wait for DPLL lock or not */
#define DPLL_RETRY_CNT                 2           /* Number of times to retry in case of PLL lock fails*/
#define DPLL_LOCK_MAX_WAIT             5000        /* Maximum 1ms iterations to wait for DPLL lock */

/* DMA descriptor or DMA data handle */
typedef dma_addr_t      DMA_HANDLE;

/* -- Private variables -- */
static XDRV_UINT8 gPcmSpecialMode;
static BCM_IUDMA_ADDR_CTL* gDmaCtl = NULL;

/* -- Public & externed variables -- */
extern struct dma_pool *pcm_dma_pool;

/* -- Private functions -- */
static void printRegs( void );
static void printCtl( void );
static void pcm63148_enable( int chan_mask );
static void pcm63148_disable( void );
static int  pcm63148_clkInit( void );
static int  pcm63148_iudmaInit( BCM_IUDMA_ADDR_CTL* dmaCtl );
static int  pcm63148_iudmaDeinit( void );
static int  pcm63148_dmaDescrInit( BCM_IUDMA_ADDR_CTL* dmaCtl );
static int  pcm63148_dmaDescrDeinit( void );
static int  pcm63148_regInit( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap, BCM_IUDMA_ADDR_CTL* dmaCtl );
static int  pcm63148_regDeinit( void );
static void pcm63148_timeslotAlloc( int chnum, int ts);


/* -- Functions -- */

/*****************************************************************************
*
*  FUNCTION: pcm63148_init
*
*  PURPOSE:
*      Intialize 63148 PCM Module
*
*  PARAMETERS: channelMask - Mask to enable specific TX/RX pcm timeslots
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/

int pcm63148_init( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap, XDRV_UINT32 channelMask, XDRV_UINT8 supportMode, BCM_IUDMA_ADDR_CTL* dmaCtl, XDRV_UINT32* dmaCount )
{
   gPcmSpecialMode = supportMode;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "------------------ PCM CONFIGURATION START ------------------ "));

   /* Init the APM module */
   pmc_apm_power_up();

   /* Init the PCM block */
   pmc_pcm_power_up();

#if NTR_OUT_DEBUG
   /* Set pinmux for NTR out */
   bcm_set_pinmux( 4, 2 );  /* NTR_OUT0 -> GPIO4  */
   bcm_set_pinmux( 13, 1 ); /* NTR_OUT0 -> GPIO13 */
   bcm_set_pinmux( 26, 2 ); /* NTR_OUT0 -> GPIO26 */   
   
   bcm_set_pinmux( 7, 2 );  /* NTR_OUT1 -> GPIO7  */
   bcm_set_pinmux( 24, 1 ); /* NTR_OUT1 -> GPIO24 */   
   bcm_set_pinmux( 27, 1 ); /* NTR_OUT1 -> GPIO27 */   
#endif /* NTR_OUT_DEBUG */
      
   /* Initialize PCM clock */
   pcm63148_clkInit();

   /* Save the start of the PCM DMA control block */
   gDmaCtl = &dmaCtl[*dmaCount];

   /* Initialize PCM registers */
   pcm63148_regInit( chanTsMap, gDmaCtl );
   pcm63148_enable( channelMask );
   pcm63148_dmaDescrInit( gDmaCtl );
   pcm63148_iudmaInit( gDmaCtl );

   /* Increase our DMA count by the number of DMAs allocated in this function */
   *dmaCount += PCM63148_NUM_DMA_CHANNELS;

   printRegs();
   printCtl();

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "------------------ PCM CONFIGURATION END ------------------ "));
   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm63148_deinit
*
*  PURPOSE:
*      Deintialize 63148 PCM Module
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int pcm63148_deinit( void )
{
   pcm63148_disable();
   pcm63148_iudmaDeinit();
   pcm63148_dmaDescrDeinit();
   pcm63148_regDeinit();

   printRegs();

   /* Wipe the PCM DMA control block pointer */
   gDmaCtl = NULL;

   /* Power off the PCM block */
   pmc_pcm_power_down();

   return ( 0 );
}


/*****************************************************************************
*
*  FUNCTION: pcm_pllStart
*
*  PURPOSE:  Initalize PCM DPLL
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
static int pcm_pllStart(void)
{
   volatile XDRV_UINT32 temp = 0;

#if DPLL_LOCK_WAIT
   XDRV_UINT32 i;
   int retry_count = -1;

 retry_lock:

   /* Keep track of retrying DPLL phase lock */
   retry_count ++;
   if ( retry_count > DPLL_RETRY_CNT ) {
       XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: DPLL failed to Lock %d times", __FUNCTION__, DPLL_RETRY_CNT));
       return -1;
   }

   i = 0;
#endif /* DPLL_LOCK_WAIT */

   /* suspend the operation of the PLL first */
   APM->dpll_control |= DPLL_SOFT_INIT;
   /* Sleep for 10ms */
   bosSleep(10);

   /* Start the PLL */
   APM->dpll_control &= ~(DPLL_SOFT_INIT | DPLL_FORCE_ACQ | DPLL_OPEN_LOOP | DPLL_CONST_K);
   APM->dpll_control &= ~0xF;
   APM->dpll_control |= ( (5 << DPLL_PHS_THSHLD_SHIFT) | 12 );
   APM->dpll_nom_freq = 0xa7c5ac47;

#if HW_NTR_SUPPORT

#if (NTR_REF_CLK_KHZ == 8)
   /* Using ref_div of   1    => 8Khz VDSL_NTR  refclk / 1    = 8Khz clock to comparator     *
    * Using local_div of 8192 => 65.536Mhz local clock / 8192 = 8Khz clock to comparator     */
   APM->dpll_div = (0 << DPLL_REF_DIV_SHIFT) | 8191;   // 8KHz/65.536MHz = 1/8192
#elif (NTR_REF_CLK_KHZ == 16)
   /* Using ref_div of   2    => 16Khz VDSL_NTR  refclk / 2   = 8Khz clock to comparator     *
    * Using local_div of 8192 => 65.536Mhz local clock / 8192 = 8Khz clock to comparator     */
   APM->dpll_div = (1 << DPLL_REF_DIV_SHIFT) | 8191;   // 16KHz/65.536MHz = 2/8192
#elif (NTR_REF_CLK_KHZ == 256)
   /* Using ref_div of   32   => 256Khz VDSL_NTR refclk / 32  = 8Khz clock to comparator     *
    * Using local_div of 8192 => 65.536Mhz local clock / 8192 = 8Khz clock to comparator     */    
   APM->dpll_div = (31 << DPLL_REF_DIV_SHIFT) | 8191;   // 256KHz/65.536MHz = 32/8192
#else
   XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: Unsupported NTR_REF_CLK frequency", __FUNCTION__));      
#endif /* NTR_REF_CLK_KHZ */

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%s: Synching PLL to VDSL_NTR[0] output", __FUNCTION__));
#else
   if ( gPcmSpecialMode & MODE_DECT )   
   {
      /* Using ref_div of   81  => 10.368Mhz DECT refclk / 81  = 128Khz clock to comparator     *
       * Using local_div of 512 => 65.536Mhz local clock / 512 = 128Khz clock to comparator     */
      APM->dpll_div = (80 << DPLL_REF_DIV_SHIFT) | 511;   // 10.368MHz/65.536MHz = 81/512
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%s: Synching PLL to DECT_NTR output", __FUNCTION__));
   }
   else
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: Unsupported PLL operating mode", __FUNCTION__));
   }
#endif /* HW_NTR_SUPPORT */

   /* Force loop aquisition */
   APM->dpll_control |= DPLL_FORCE_ACQ;
   APM->dpll_control &= ~DPLL_FORCE_ACQ;

   /* Read DPLL status */
   temp = APM->dpll_status;

#if DPLL_LOCK_WAIT
   /* Waiting for the DPLL_IN_SYNC and DPLL_ACQ_FREQ_VALID bits to be set */
   while( (!(temp & DPLL_IN_SYNC) || !(temp & DPLL_ACQ_FREQ_VALID))  )
   {
      temp = APM->dpll_status;
      i++;

      if ( i == DPLL_LOCK_MAX_WAIT )
      {
         if (!(temp & DPLL_ACQ_FREQ_VALID)) 
         {
           XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: DPLL failed to aquire the frequency, dpll_status = 0x%08x", __FUNCTION__, (unsigned int) temp));
         }
         else
         {
           XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: DPLL failed to Lock, dpll_status = 0x%08x", __FUNCTION__, (unsigned int) temp));
         }
	 goto retry_lock;
      }
      /* Sleep for 10ms */
      bosSleep(10);
   }

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%s: DPLL Locked, Iteration = %u, dpll_status = 0x%08x, dpll_acq_freq = 0x%08x",
                  __FUNCTION__, (unsigned int)i, (unsigned int) temp, (unsigned int) APM->dpll_acq_freq));
#else
   XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: DPLL Lock wait skipped , dpll_status = 0x%08x", __FUNCTION__, (unsigned int) temp));
#endif /* DPLL_LOCK_WAIT */
   return 0;

}

/*****************************************************************************
*
*  FUNCTION: pcm63148_clkInit
*
*  PURPOSE:
*      Set up APM registers needed to generate PCM clock.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static int pcm63148_clkInit( void )
{
   XDRV_UINT32 pcmNCOMuxCtl;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "Set up PCM registers to generate PCM clock"));

#if HW_NTR_SUPPORT
   /* set the DPLL refclk source to VDSL_NTR[0] clock */
   MISC->miscAdsl_clock_sample  &= ~(0xF0000000);
   MISC->miscAdsl_clock_sample  |= ( PCM_NTR_IN_CTRL_SEL_VDSL_NTR_0 << PCM_NTR_IN_CTRL_SHIFT);  /* set PCM_NTR_IN_CTRL to VDSL_NTR[0] output. */

   /* Set PCM NCO FCW source as DPLL */
   pcmNCOMuxCtl = P_PCM_NCO_MUX_CNTL_DPLL;
#else   
   if ( gPcmSpecialMode & MODE_DECT )
   {
      /* set the DPLL refclk source to DECT clock */
      MISC->miscAdsl_clock_sample  &= ~(0xF0000000);
      MISC->miscAdsl_clock_sample  |= ( PCM_NTR_IN_CTRL_SEL_DECT_NTR << PCM_NTR_IN_CTRL_SHIFT);  /* set PCM_NTR_IN_CTRL to DECT NTR output. */

      /* Set PCM NCO FCW source as DPLL */
      pcmNCOMuxCtl = P_PCM_NCO_MUX_CNTL_DPLL;
   }
   else
   {
      /* Set PCM NCO FCW source as MISC */
      pcmNCOMuxCtl = P_PCM_NCO_MUX_CNTL_MISC;
   }
#endif /* HW_NTR_SUPPORT */

   /* initialize the PCM DPLL */
   if( HW_NTR_SUPPORT || ( gPcmSpecialMode & MODE_DECT ) )
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "miscAdsl_clock_sample 0x%08x @ 0x%08x", (unsigned int)MISC->miscAdsl_clock_sample 
                                                                               , (unsigned int)&(MISC->miscAdsl_clock_sample) ));
      if ( pcm_pllStart() == -1 ) {
           XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: Using Fixed FCW instead of DPLL", __FUNCTION__));
           /* Set MUX to use fixed FCW */
           pcmNCOMuxCtl = P_PCM_NCO_MUX_CNTL_MISC;
      }
   }
   
   /* Load MISC register with target FCW from DPLL */
   PCM->reg_pcm_clk_cntl_0 = P_NCO_FCW_MISC;

   if( gPcmSpecialMode & MODE_ISI )
   {  /* Settings with ISI */
      PCM->reg_pcm_clk_cntl_1 = 0x60000000;        // PCM_NCO clk_out = 24.576Mhz. NOTE: This value is based on o/p DPLL fcw of 0xA7C5AC47.
      PCM->pcm_msif_intf      = 0x00000001;        // R_MSIF_ENABLE = 1
   }
   else if( gPcmSpecialMode & MODE_ZSI )
   {  /* Settings with ZSI */
      PCM->reg_pcm_clk_cntl_1 = 0xc0000000;        // PCM_NCO clk_out = 49.152Mhz. NOTE: This value is based on o/p DPLL fcw of 0xA7C5AC47.
      PCM->pcm_zds_intf       = 0x00000001;        // R_ZDS_ENABLE = 1
   }
   else
   {  /* Settings with normal PCM operation */
      PCM->reg_pcm_clk_cntl_1 = 0x40000000;        // PCM_NCO clk_out = 16.384Mhz. NOTE: This value is based on o/p DPLL fcw of 0xA7C5AC47.
   }

   /* Soft-init PCM NCO */
   PCM->reg_pcm_clk_cntl_2 = ((P_PCM_NCO_SHIFT & PCM_NCO_SHIFT)
                              | ((pcmNCOMuxCtl << 4) & PCM_NCO_MUX_CNTL)
                              |  (PCM_NCO_SOFT_INIT));
   PCM->reg_pcm_clk_cntl_2 = ((P_PCM_NCO_SHIFT & PCM_NCO_SHIFT)
                              | ((pcmNCOMuxCtl << 4) & PCM_NCO_MUX_CNTL));

   if( pcmNCOMuxCtl == P_PCM_NCO_MUX_CNTL_MISC )
   {
      /* Load the MISC FCW */
      PCM->reg_pcm_clk_cntl_2 = ((P_PCM_NCO_SHIFT & PCM_NCO_SHIFT)
                                 | ((pcmNCOMuxCtl << 4) & PCM_NCO_MUX_CNTL)
                                 |  (PCM_NCO_LOAD_MISC));
      PCM->reg_pcm_clk_cntl_2 = ((P_PCM_NCO_SHIFT & PCM_NCO_SHIFT)
                                 | ((pcmNCOMuxCtl << 4) & PCM_NCO_MUX_CNTL));

      XDRV_LOG_DBG(( XDRV_LOG_MOD_PCM, "Loading MISC FCW to PCM NCO: 0x%08x", (unsigned int)PCM->reg_pcm_clk_cntl_0 ));
   }
  return(0);
}

/*****************************************************************************
*
*  FUNCTION: pcm63148_regInit
*
*  PURPOSE:
*      Intialize 63148 PCM Module Registers
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static int pcm63148_regInit( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap, BCM_IUDMA_ADDR_CTL* dmaCtl )
{
   volatile ApmControlRegisters *pApm = APM;
   XDRV_UINT32 i;

   XDRV_LOG_INFO((XDRV_LOG_MOD_PCM, "Set up PCM registers"));

   /* PCM Control */
   PCM->pcm_ctrl = ((PCM_AP_SEL)  |
                    (0 << PCM_SLAVE_SEL_SHIFT) |
                    (1 << PCM_CLOCK_INV_SHIFT) |
                    (0 << PCM_FS_INVERT_SHIFT)  |
                    (0 << PCM_FS_FREQ_16_8_SHIFT)  |
                    (0 << PCM_FS_LONG_SHIFT)  |
                    (1 << PCM_FS_TRIG_SHIFT)  |
                    (1 << PCM_DATA_OFF_SHIFT)  |
#if !defined(PCM_ALAW) && !defined(PCM_ULAW)
                    (1 << PCM_DATA_16_8_SHIFT)  |
#endif
                    (0 << PCM_LSB_FIRST_SHIFT)  |
                    (0 << PCM_LOOPBACK_SHIFT)  |
                    (0 << PCM_EXTCLK_SEL_SHIFT)  |
                    (PCM_BITS_PER_FRAME_256) );

   /* Set pcm clk to 2.048Mhz */
   if( gPcmSpecialMode & MODE_ISI && ( PCM->pcm_msif_intf & 0x00000001 ) )
   {
      /* Enable the special div/12 clock divider for ISI mode input clock of 24.576Mhz */
      PCM->pcm_ctrl |= ((7 << PCM_CLOCK_SEL_SHIFT) & PCM_CLOCK_SEL);
   }
   else
   {
      /* Enable the div/8 clock divider for PCM/ZSI mode input clock of 16.384Mhz */
      PCM->pcm_ctrl |= ((2 << PCM_CLOCK_SEL_SHIFT) & PCM_CLOCK_SEL);
   }

   XDRV_LOG_DBG(( XDRV_LOG_MOD_PCM, "reg_pcm_control = 0x%lx", PCM->pcm_ctrl));

   PCM->pcm_chan_ctrl = 0;

   /* clear time slot allocation table */
   for(i = 0; i < PCM_MAX_TIMESLOT_REGS; i++)
   {
      PCM->pcm_slot_alloc_tbl[i] = 0;
   }

   for(i = 0; i < PCM_MAX_CHANNELS; i++)
   {
      if( chanTsMap[i].chan == PCM_CHAN_INVALID )
      {
         break;
      }
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM: Channel/Line %d Timeslot %d ", (int)i, (int)chanTsMap[i].txTimeslot ));
      pcm63148_timeslotAlloc( i, chanTsMap[i].txTimeslot );
      dmaCtl[0].chanCount++;
   }

   /* Clear PCM interrupts at the APM device */
   pApm->apm_dev_irq_pend |= DMA_PCM_RX | DMA_PCM_TX;

   /* Disable pcm interrupts - There is a dedicated interrupt line
   ** for DMA interrupts which originates directly from the iudma core
   ** we will therefore use that interrupt line and disable the
   ** interrupts originating from the APM block. The dedicated iudma
   ** interrupt line is controlled via the regs.gbl_int_mask */
   pApm->apm_dev_irq_mask &= ~( DMA_PCM_RX | DMA_PCM_TX );
  
   /* Disable all direct interrupts from the PCM block */
   PCM->pcm_int_mask = 0;

  return( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm63148_enable
*
*  PURPOSE:
*      Enable 63148 PCM Module
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static void pcm63148_enable( int chan_mask )
{
   /* Enable PCM operation */
   PCM->pcm_ctrl |= PCM_ENABLE ;

   /* Enable receive/transmit channels */
   PCM->pcm_chan_ctrl |= chan_mask & ( PCM_TX0_EN | PCM_TX1_EN | PCM_TX2_EN | PCM_TX3_EN |
                                       PCM_TX4_EN | PCM_TX5_EN | PCM_TX6_EN | PCM_TX7_EN  );

   PCM->pcm_chan_ctrl |= chan_mask & ( PCM_RX0_EN | PCM_RX1_EN | PCM_RX2_EN | PCM_RX3_EN |
                                       PCM_RX4_EN | PCM_RX5_EN | PCM_RX6_EN | PCM_RX7_EN  );
}

/*****************************************************************************
*
*  FUNCTION: pcm63148_disable
*
*  PURPOSE:
*      Disable 63148 PCM Module
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static void pcm63148_disable( void )
{
   /* Disable receive/transmit channels */
   PCM->pcm_chan_ctrl = 0;

   /* Disable PCM operation */
   PCM->pcm_ctrl &= ~PCM_ENABLE;

   PCM->pcm_ctrl = 0;

   PCM->pcm_int_pending |= PCM->pcm_int_pending;
}

static int pcm63148_regDeinit( void )
{
   XDRV_UINT32 i;

   /* clear time slot allocation table */
   for(i = 0; i < PCM_MAX_TIMESLOT_REGS; i++)
   {
      PCM->pcm_slot_alloc_tbl[i] = 0;
   }

   return( 0 );
}


/*
*****************************************************************************
** FUNCTION:   pcm63148_initDmaDescriptors
**
** PURPOSE:    Initialize the DMA descriptor rings.
**
** PARAMETERS: sampleRate - sampling rate in kHz (8 or 16k)
**             packetRate - the rate (in ms) at which APM generates packets/interrupts
**
** RETURNS:    none
**
*****************************************************************************
*/
static int pcm63148_dmaDescrInit( BCM_IUDMA_ADDR_CTL* dmaCtl )
{
   XDRV_UINT32              i, chnum;
   BCM63148_IUDMA_DESC      *dmaTxDesc;
   BCM63148_IUDMA_DESC      *dmaRxDesc;
   IUDMA_ADDR               dmaRxData;
   IUDMA_ADDR               dmaTxData;
   unsigned                 bufferSizeBytes = 0;
   XDRV_UINT32              rxDescriptorArea;
   XDRV_UINT32              txDescriptorArea;
   XDRV_UINT32              rxBufferArea;
   XDRV_UINT32              txBufferArea;

   rxDescriptorArea = sizeof(BCM63148_IUDMA_DESC) * PCM63148_NUM_DMA_CHANNELS * DMA_RX_NUM_BUFFERS + sizeof(XDRV_UINT8) * BCM63148_DMA_UBUS_BYTES;
   txDescriptorArea = sizeof(BCM63148_IUDMA_DESC) * PCM63148_NUM_DMA_CHANNELS * DMA_TX_NUM_BUFFERS + sizeof(XDRV_UINT8) * BCM63148_DMA_UBUS_BYTES;

   /* Allocate receive and transmit descriptors */

   dmaRxDesc = dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, (dma_addr_t*) &dmaCtl[0].rx[0].descPhy );
   dmaTxDesc = dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, (dma_addr_t*) &dmaCtl[0].tx[0].descPhy );

   /* Save our addresses for when we free memory */
   dmaCtl[0].rx[0].descVirt = dmaRxDesc;
   dmaCtl[0].tx[0].descVirt = dmaTxDesc;

   if( (dmaRxDesc == NULL) || (dmaTxDesc == NULL) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_PCM, "Could not allocate memory for DMA descriptors !!! "));
      goto init_err_desc;
   }

   XDRV_LOG_INFO((XDRV_LOG_MOD_PCM, "rxDescriptorArea       = %d ", (int)rxDescriptorArea));
   XDRV_LOG_INFO((XDRV_LOG_MOD_PCM, "txDescriptorArea       = %d ", (int)txDescriptorArea));
   XDRV_LOG_INFO((XDRV_LOG_MOD_PCM, "dmaRxDesc  = 0x%08X ", (unsigned int)dmaRxDesc));
   XDRV_LOG_INFO((XDRV_LOG_MOD_PCM, "dmaTxDesc  = 0x%08X ", (unsigned int)dmaTxDesc));
   XDRV_LOG_INFO((XDRV_LOG_MOD_PCM, "Sizeof(BCM63148_IUDMA_DESC) %d\n", sizeof(BCM63148_IUDMA_DESC)));

   /* Fill in the global PCM DMA structure with the allocated pointers to descriptors */
   for(chnum = 0; chnum < PCM63148_NUM_DMA_CHANNELS; chnum++)
   {
      dmaCtl[chnum].type = IUDMA_TYPE_PCM;
      dmaCtl[chnum].rx[0].descVirt = (dmaRxDesc + chnum * DMA_RX_NUM_BUFFERS);
      dmaCtl[chnum].rx[1].descVirt = (dmaRxDesc + chnum * DMA_RX_NUM_BUFFERS + 1);
      dmaCtl[chnum].rx[0].descPhy = (void*)(dmaCtl[chnum].rx[0].descPhy + chnum * DMA_RX_NUM_BUFFERS);
      dmaCtl[chnum].rx[1].descPhy = (void*)(dmaCtl[chnum].rx[0].descPhy + chnum * DMA_RX_NUM_BUFFERS + 1);

      dmaCtl[chnum].tx[0].descVirt = (dmaTxDesc + chnum * DMA_TX_NUM_BUFFERS);
      dmaCtl[chnum].tx[1].descVirt = (dmaTxDesc + chnum * DMA_TX_NUM_BUFFERS + 1);
      dmaCtl[chnum].tx[0].descPhy = (void*)(dmaCtl[chnum].tx[0].descPhy + chnum * DMA_RX_NUM_BUFFERS);
      dmaCtl[chnum].tx[1].descPhy = (void*)(dmaCtl[chnum].tx[0].descPhy + chnum * DMA_RX_NUM_BUFFERS + 1);
   }

   /* Clear the descriptors */
   memset( dmaRxDesc, 0, rxDescriptorArea);
   memset( dmaTxDesc, 0, txDescriptorArea);

   /* Allocate data buffers */
   rxBufferArea = sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM63148_NUM_DMA_CHANNELS * DMA_RX_NUM_BUFFERS;
   txBufferArea = sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM63148_NUM_DMA_CHANNELS * DMA_TX_NUM_BUFFERS;

   dmaRxData.addr32 = (XDRV_UINT32) dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, (dma_addr_t*) &dmaCtl[0].rx[0].bufPhy );
   dmaTxData.addr32 = (XDRV_UINT32) dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, (dma_addr_t*) &dmaCtl[0].tx[0].bufPhy );

   /* Save our addresses for when we free memory */
   dmaCtl[0].rx[0].bufVirt = dmaRxData;
   dmaCtl[0].tx[0].bufVirt = dmaTxData;

   if( ((void*)dmaRxData.addr32 == NULL) || ((void*)dmaTxData.addr32 == NULL) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_PCM, "Could not allocate memory for DMA data buffers !!! "));
      goto init_err_buf;
   }

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "rxBufferArea        = %d ", (int)rxBufferArea));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "txBufferArea        = %d ", (int)txBufferArea));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "dmaRxData = 0x%08X ", (unsigned int)dmaRxData.addr32));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "dmaTxData = 0x%08X ", (unsigned int)dmaTxData.addr32));

   /* Fill in the global PCM DMA structure with the allocated pointers to descriptors */
   for(chnum = 0; chnum < PCM63148_NUM_DMA_CHANNELS; chnum++)
   {
      dmaCtl[chnum].rx[0].bufVirt.addr32 = (dmaRxData.addr32 + chnum * DMA_RX_NUM_BUFFERS * sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32));
      dmaCtl[chnum].rx[1].bufVirt.addr32 = (dmaRxData.addr32 + chnum * DMA_RX_NUM_BUFFERS * sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) + sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32));
      dmaCtl[chnum].rx[0].bufPhy.addr32 = dmaCtl[chnum].rx[0].bufPhy.addr32 + chnum * DMA_RX_NUM_BUFFERS * sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32);
      dmaCtl[chnum].rx[1].bufPhy.addr32 = dmaCtl[chnum].rx[0].bufPhy.addr32 + chnum * DMA_RX_NUM_BUFFERS * sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) + sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32);

      dmaCtl[chnum].tx[0].bufVirt.addr32 = (dmaTxData.addr32 + chnum * DMA_TX_NUM_BUFFERS * sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32));
      dmaCtl[chnum].tx[1].bufVirt.addr32 = (dmaTxData.addr32 + chnum * DMA_TX_NUM_BUFFERS * sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) + sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32));
      dmaCtl[chnum].tx[0].bufPhy.addr32 = dmaCtl[chnum].tx[0].bufPhy.addr32 + chnum * DMA_TX_NUM_BUFFERS * sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32);
      dmaCtl[chnum].tx[1].bufPhy.addr32 = dmaCtl[chnum].tx[0].bufPhy.addr32 + chnum * DMA_TX_NUM_BUFFERS * sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) + sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32);
   }

   /* Reset the data buffers */
   memset( (void*) dmaRxData.addr32, 0, rxBufferArea);
   memset( (void*) dmaTxData.addr32, 0, txBufferArea);

   for(chnum = 0; chnum < PCM63148_NUM_DMA_CHANNELS; chnum++)
   {
      bufferSizeBytes = PCM63148_DMA_MAX_BUFFER_SIZE_BYTES;

      /* Initialize our TX channels */
      for(i = 0; i < DMA_TX_NUM_BUFFERS; i++)
      {
         /* Set buffer status, size, and physical address */
         dmaCtl[chnum].tx[i].descVirt->flagsLengthStatus.status = BCM63148_IUDMA_DESC_STATUS_SOP | BCM63148_IUDMA_DESC_STATUS_EOP;
         dmaCtl[chnum].tx[i].descVirt->flagsLengthStatus.length = bufferSizeBytes;
         dmaCtl[chnum].tx[i].descVirt->addr.addr32 = dmaCtl[chnum].tx[i].bufPhy.addr32;

         if( i == 0 )
         {
            /* First buffer - prime the TX FIFO */
            dmaCtl[chnum].tx[i].descVirt->flagsLengthStatus.status |= BCM63148_IUDMA_DESC_STATUS_OWN;
         }
         else if( i == (DMA_TX_NUM_BUFFERS - 1) )
         {
            /* Last buffer - set the WRAP bit */
            dmaCtl[chnum].tx[i].descVirt->flagsLengthStatus.status |= BCM63148_IUDMA_DESC_STATUS_WRAP;
         }
      }

      /* Initialize our RX channels */
      for(i = 0; i < DMA_RX_NUM_BUFFERS; i++)
      {
         /* Give ownership to all RX descriptors, set buffer size, and physical DMA addresses */
         dmaCtl[chnum].rx[i].descVirt->flagsLengthStatus.status = BCM63148_IUDMA_DESC_STATUS_OWN;
         dmaCtl[chnum].rx[i].descVirt->flagsLengthStatus.length = bufferSizeBytes;
         dmaCtl[chnum].rx[i].descVirt->addr.addr32 = dmaCtl[chnum].rx[i].bufPhy.addr32;

         /* Last descriptor - set the wrap bit */
         if( i == ( DMA_RX_NUM_BUFFERS - 1 ) )
         {
            dmaCtl[chnum].rx[i].descVirt->flagsLengthStatus.status |= BCM63148_IUDMA_DESC_STATUS_WRAP;
         }
      }
   }

   return ( 0 );


init_err_buf:
   if(dmaCtl[0].rx[0].bufVirt.addr32)
   {
      dma_pool_free( pcm_dma_pool, (void*)dmaCtl[0].rx[0].bufVirt.addr32, dmaCtl[0].rx[0].bufPhy.addr32 );
      dmaCtl[0].rx[0].bufVirt.addr32 = 0;
   }
   if(dmaCtl[0].tx[0].bufVirt.addr32)
   {
      dma_pool_free( pcm_dma_pool, (void*)dmaCtl[0].tx[0].bufVirt.addr32, dmaCtl[0].tx[0].bufPhy.addr32 );
      dmaCtl[0].tx[0].bufVirt.addr32 = 0;
   }

init_err_desc:
   if(dmaCtl[0].rx[0].descVirt)
   {
      dma_pool_free( pcm_dma_pool, dmaCtl[0].rx[0].descVirt, (unsigned int)dmaCtl[0].rx[0].descPhy );
      dmaCtl[0].rx[0].descVirt = NULL;
   }
   if(dmaCtl[0].tx[0].descVirt)
   {
      dma_pool_free( pcm_dma_pool, dmaCtl[0].tx[0].descVirt, (unsigned int)dmaCtl[0].tx[0].descPhy );
      dmaCtl[0].tx[0].descVirt = NULL;
   }

   return ( -1 );
}


static int pcm63148_dmaDescrDeinit( void )
{
   XDRV_UINT8                       i;
   XDRV_UINT32                      rxDescriptorArea;
   XDRV_UINT32                      txDescriptorArea;
   XDRV_UINT32                      rxBufferArea;
   XDRV_UINT32                      txBufferArea;

   if(gDmaCtl == NULL)
   {
      return ( 0 );
   }

   rxDescriptorArea = sizeof(BCM63148_IUDMA_DESC) * PCM63148_NUM_DMA_CHANNELS * DMA_RX_NUM_BUFFERS;
   txDescriptorArea = sizeof(BCM63148_IUDMA_DESC) * PCM63148_NUM_DMA_CHANNELS * DMA_TX_NUM_BUFFERS;

   /* Clear the descriptors */
   memset( gDmaCtl[0].rx[0].descVirt, 0, rxDescriptorArea );
   memset( gDmaCtl[0].tx[0].descVirt, 0, txDescriptorArea );

   /* Free receive and transmit descriptors */
   dma_pool_free( pcm_dma_pool, gDmaCtl[0].rx[0].descVirt, (unsigned int)gDmaCtl[0].rx[0].descPhy );
   dma_pool_free( pcm_dma_pool, gDmaCtl[0].tx[0].descVirt, (unsigned int)gDmaCtl[0].tx[0].descPhy );

   for(i = 0; i < PCM63148_NUM_DMA_CHANNELS; i++)
   {
      gDmaCtl[i].rx[0].descVirt = NULL;
      gDmaCtl[i].tx[0].descVirt = NULL;
   }

   rxBufferArea = sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM63148_NUM_DMA_CHANNELS * DMA_RX_NUM_BUFFERS;
   txBufferArea = sizeof(XDRV_UINT32) * (PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM63148_NUM_DMA_CHANNELS * DMA_TX_NUM_BUFFERS;

   /* Clear the data buffers */
   memset( (void*) gDmaCtl[0].rx[0].bufVirt.addr32, 0, rxBufferArea);
   memset( (void*) gDmaCtl[0].tx[0].bufVirt.addr32, 0, txBufferArea);

   /* Free data buffers */
   dma_pool_free( pcm_dma_pool, (void*) gDmaCtl[0].rx[0].bufVirt.addr32, gDmaCtl[0].rx[0].bufPhy.addr32 );
   dma_pool_free( pcm_dma_pool, (void*) gDmaCtl[0].tx[0].bufVirt.addr32, gDmaCtl[0].tx[0].bufPhy.addr32 );

   /* Fill in the global PCM DMA structure with the allocated pointers to descriptors */
   for(i = 0; i < PCM63148_NUM_DMA_CHANNELS; i++)
   {
      gDmaCtl[i].rx[0].bufVirt.addr32 = 0;
      gDmaCtl[i].tx[0].bufVirt.addr32 = 0;
   }

   /* We no longer need a pointer to the configuration struct */
   gDmaCtl = NULL;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   pcm63148_initIudma
**
** PURPOSE:    Initialize the PCM IUDMA
**
** PARAMETERS: iuDmap       - pointer to IUDMA module
**             dmaSettingsp - pointer to DMA configuration structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static int pcm63148_iudmaInit( BCM_IUDMA_ADDR_CTL* dmaCtl )
{
   int chnum;
   volatile ApmPcmIudma *iuDmap = APM_PCM_IUDMA;

   dmaCtl[0].dmaChanRx = BCM_APM_IUDMA_PCM_RX;
   dmaCtl[0].dmaChanTx = BCM_APM_IUDMA_PCM_TX;

   /* Initialize the Rx/Tx IUDMA pair for each PCM channel */
   for(chnum = 0; chnum < PCM63148_NUM_DMA_CHANNELS; chnum++)
   {
      /* Explicitly Reset DMA channel */
      iuDmap->ctrl[dmaCtl[chnum].dmaChanRx].config = 0;
      iuDmap->ctrl[dmaCtl[chnum].dmaChanTx].config = 0;
      iuDmap->regs.channelReset |= ( (1 << dmaCtl[chnum].dmaChanRx) | (1 << dmaCtl[chnum].dmaChanTx) );

      /* Init RX side */
      XDRV_LOG_DBG(( XDRV_LOG_MOD_PCM, "%s: RX - chan %u, descBase 0x%x, descBaseIudma 0x%x",
                      __FUNCTION__,
                      (unsigned int)dmaCtl[chnum].dmaChanRx,
                      (unsigned int)dmaCtl[chnum].rx[0].descVirt,
                      (unsigned int)dmaCtl[chnum].rx[0].descPhy ));
      iuDmap->ctrl[dmaCtl[chnum].dmaChanRx].maxBurst = 4; /* in 64-bit words */
      iuDmap->stram[dmaCtl[chnum].dmaChanRx].baseDescPointer = (unsigned int)dmaCtl[chnum].rx[0].descPhy;
      iuDmap->stram[dmaCtl[chnum].dmaChanRx].stateBytesDoneRingOffset = 0;
      iuDmap->stram[dmaCtl[chnum].dmaChanRx].flagsLengthStatus = 0;
      iuDmap->stram[dmaCtl[chnum].dmaChanRx].currentBufferPointer = 0;
      iuDmap->ctrl[dmaCtl[chnum].dmaChanRx].intStat |= iuDmap->ctrl[dmaCtl[chnum].dmaChanRx].intStat;
      iuDmap->ctrl[dmaCtl[chnum].dmaChanRx].intMask = BCM63148_IUDMA_INTMASK_BDONE | BCM63148_IUDMA_INTMASK_NOTVLD;

      /* Enable PCMDMA interrupts for RX only */
      iuDmap->regs.gbl_int_mask |= (1 << dmaCtl[chnum].dmaChanRx);

      /* Init TX side */
      XDRV_LOG_DBG(( XDRV_LOG_MOD_PCM, "%s: TX - chan %u, descBase 0x%x, descBaseIudma 0x%x",
                      __FUNCTION__,
                      (unsigned int)dmaCtl[chnum].dmaChanTx,
                      (unsigned int)dmaCtl[chnum].tx[0].descVirt,
                      (unsigned int)dmaCtl[chnum].tx[0].descPhy ));
      iuDmap->ctrl[dmaCtl[chnum].dmaChanTx].maxBurst = 4; /* in 64-bit words */
      iuDmap->stram[dmaCtl[chnum].dmaChanTx].baseDescPointer = (unsigned int)dmaCtl[chnum].tx[0].descPhy;
      iuDmap->stram[dmaCtl[chnum].dmaChanTx].stateBytesDoneRingOffset = 0;
      iuDmap->stram[dmaCtl[chnum].dmaChanTx].flagsLengthStatus = 0;
      iuDmap->stram[dmaCtl[chnum].dmaChanTx].currentBufferPointer = 0;
      iuDmap->ctrl[dmaCtl[chnum].dmaChanTx].intStat |= iuDmap->ctrl[dmaCtl[chnum].dmaChanTx].intStat;
      iuDmap->ctrl[dmaCtl[chnum].dmaChanTx].intMask = BCM63148_IUDMA_INTMASK_BDONE | BCM63148_IUDMA_INTMASK_NOTVLD;

      /* Disable PCMDMA interrupts for TX */
      iuDmap->regs.gbl_int_mask &= ~( 1 << dmaCtl[chnum].dmaChanTx );
   }

   /* Enable the IUDMA master */
   iuDmap->regs.ctrlConfig = IUDMA_REGS_CTRLCONFIG_MASTER_EN;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   pcm63148_iudmaDeinit
**
** PURPOSE:    Deinitialize the PCM IUDMA
**
** PARAMETERS:
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
static int pcm63148_iudmaDeinit( void )
{
   int chnum;
   volatile ApmPcmIudma *iuDmap = APM_PCM_IUDMA;
   int i = 0;

   /* Halt individual channels first */
   for(chnum = 0; chnum < PCM63148_NUM_DMA_CHANNELS; chnum++)
   {
      /* Deinit RX side */
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "RX DMA STATUS (1=busy 0=IDLE) : 0x%08x", (unsigned int)iuDmap->ctrl[gDmaCtl[chnum].dmaChanRx].config));
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "HALTING RX DMA...."));

      /* Set the burstHalt bit while clearing endma bit */
      iuDmap->ctrl[gDmaCtl[chnum].dmaChanRx].config = ( BCM63148_IUDMA_CONFIG_BURSTHALT );

      while( (iuDmap->ctrl[gDmaCtl[chnum].dmaChanRx].config & BCM63148_IUDMA_CONFIG_ENDMA) != 0x00000000 )
      {
         /* Set the burstHalt bit while clearing endma bit */
         iuDmap->ctrl[gDmaCtl[chnum].dmaChanRx].config = ( BCM63148_IUDMA_CONFIG_BURSTHALT );
         XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "RX DMA HALT Waiting, iteration: %d", i));
         i++;
      }

      i = 0;
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "RX DMA IDLE"));
      iuDmap->ctrl[gDmaCtl[chnum].dmaChanRx].intMask = 0;
      iuDmap->ctrl[gDmaCtl[chnum].dmaChanRx].intStat |= iuDmap->ctrl[gDmaCtl[chnum].dmaChanRx].intStat;

      /* Deinit TX side */
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "TX DMA STATUS (1=busy 0=IDLE) : 0x%08x", (unsigned int)iuDmap->ctrl[gDmaCtl[chnum].dmaChanTx].config));
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "HALTING TX DMA...."));

      /* Set the burstHalt bit while clearing endma bit */
      iuDmap->ctrl[gDmaCtl[chnum].dmaChanTx].config = ( BCM63148_IUDMA_CONFIG_BURSTHALT );

      while( (iuDmap->ctrl[gDmaCtl[chnum].dmaChanTx].config & BCM63148_IUDMA_CONFIG_ENDMA) != 0x00000000 )
      {
         /* Set the burstHalt bit while clearing endma bit */
         iuDmap->ctrl[gDmaCtl[chnum].dmaChanTx].config = ( BCM63148_IUDMA_CONFIG_BURSTHALT );
         XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "TX DMA HALT Waiting, iteration:%d",i));
         i++;
      }

      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "TX DMA IDLE"));
      iuDmap->ctrl[gDmaCtl[chnum].dmaChanTx].intMask = 0;
      iuDmap->ctrl[gDmaCtl[chnum].dmaChanTx].intStat |= iuDmap->ctrl[gDmaCtl[chnum].dmaChanTx].intStat;

      /* Explicitly Reset DMA channel */
      iuDmap->regs.channelReset |= ( APM_IUDMA_IRQ_RX_PCM | APM_IUDMA_IRQ_TX_PCM );
   }

   /* Disable pcm iudma controller */
   iuDmap->regs.ctrlConfig &= ~IUDMA_REGS_CTRLCONFIG_MASTER_EN;

   return( 0 );
}


/*****************************************************************************
*
*  FUNCTION: pcm63148_timeslotAlloc
*
*  PURPOSE:
*      Allocate PCM timeslot in the PCM table
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
static void pcm63148_timeslotAlloc( int chnum, int ts )
{
   int         ts_register_offset;
   int         bit_offset = 0;
   XDRV_UINT32 entry, mask;

   /*
   ** Each PCM timeslot register contains configuration for 8 timeslots.
   ** Timeslots 0 to 7 are in the first PCM timeslot register,
   ** timeslots 8 to 15 in the second, etc.
   */
   ts_register_offset = ts >> 3;

   switch (ts & 7)
   {
     case 0:  bit_offset = 28; break;
     case 1:  bit_offset = 24; break;
     case 2:  bit_offset = 20; break;
     case 3:  bit_offset = 16; break;
     case 4:  bit_offset = 12; break;
     case 5:  bit_offset = 8; break;
     case 6:  bit_offset = 4; break;
     case 7:  bit_offset = 0;
   }

   /* Read, modify, write for the PCM timeslot register */
   entry = PCM->pcm_slot_alloc_tbl[ts_register_offset];
   mask  = ~(0xF << bit_offset);
   entry &= mask;
   entry |= ( (PCM_TS_VALID | chnum) << bit_offset);

   PCM->pcm_slot_alloc_tbl[ts_register_offset] = entry;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "%s: 0x%08X: 0x%08X", __FUNCTION__, (unsigned int)&PCM->pcm_slot_alloc_tbl[ts_register_offset], (unsigned int)entry ));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "   Channel %d assigned to timeslot %d", chnum, ts));
}


static void printRegs( void )
{
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM: Interrupt Masks"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "---------------"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->regs.gbl_int_mask = 0x%08X ", (unsigned int)APM_PCM_IUDMA->regs.gbl_int_mask));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->ctrl[4].intMask = 0x%08X ", (unsigned int)APM_PCM_IUDMA->ctrl[4].intMask));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->ctrl[5].intMask = 0x%08X ", (unsigned int)APM_PCM_IUDMA->ctrl[5].intMask));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM: Interrupt Status"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "-----------------"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->ctrl[4].intStat = 0x%08X ", (unsigned int)APM_PCM_IUDMA->ctrl[4].intStat));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->ctrl[5].intStat = 0x%08X ", (unsigned int)APM_PCM_IUDMA->ctrl[5].intStat));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->reg_pcm_clk_cntl_0         = 0x%08X ", (unsigned int)PCM->reg_pcm_clk_cntl_0));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->reg_pcm_clk_cntl_1         = 0x%08X ", (unsigned int)PCM->reg_pcm_clk_cntl_1));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->reg_pcm_clk_cntl_2         = 0x%08X ", (unsigned int)PCM->reg_pcm_clk_cntl_2));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_int_pending       = 0x%08X ", (unsigned int)PCM->pcm_int_pending));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_int_mask          = 0x%08X ", (unsigned int)PCM->pcm_int_mask   ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_ctrl              = 0x%08X ", (unsigned int)PCM->pcm_ctrl       ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_chan_ctrl         = 0x%08X ", (unsigned int)PCM->pcm_chan_ctrl  ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->regs.ctrlConfig = 0x%08X ", (unsigned int)APM_PCM_IUDMA->regs.ctrlConfig));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->ctrl[4].maxBurst                  = 0x%08X ", (unsigned int)APM_PCM_IUDMA->ctrl[4].maxBurst                 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->ctrl[4].config                    = 0x%08X ", (unsigned int)APM_PCM_IUDMA->ctrl[4].config                   ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->stram[4].baseDescPointer          = 0x%08X ", (unsigned int)APM_PCM_IUDMA->stram[4].baseDescPointer         ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->stram[4].stateBytesDoneRingOffset = 0x%08X ", (unsigned int)APM_PCM_IUDMA->stram[4].stateBytesDoneRingOffset));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->stram[4].flagsLengthStatus        = 0x%08X ", (unsigned int)APM_PCM_IUDMA->stram[4].flagsLengthStatus       ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->stram[4].currentBufferPointer     = 0x%08X ", (unsigned int)APM_PCM_IUDMA->stram[4].currentBufferPointer    ));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->ctrl[5].maxBurst                  = 0x%08X ", (unsigned int)APM_PCM_IUDMA->ctrl[5].maxBurst                 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->ctrl[5].config                    = 0x%08X ", (unsigned int)APM_PCM_IUDMA->ctrl[5].config                   ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->stram[5].baseDescPointer          = 0x%08X ", (unsigned int)APM_PCM_IUDMA->stram[5].baseDescPointer         ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->stram[5].stateBytesDoneRingOffset = 0x%08X ", (unsigned int)APM_PCM_IUDMA->stram[5].stateBytesDoneRingOffset));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->stram[5].flagsLengthStatus        = 0x%08X ", (unsigned int)APM_PCM_IUDMA->stram[5].flagsLengthStatus       ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "APM_PCM_IUDMA->stram[5].currentBufferPointer     = 0x%08X ", (unsigned int)APM_PCM_IUDMA->stram[5].currentBufferPointer    ));
}

static void printCtl( void )
{
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "  DMA RX"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "----------"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "DescVirt[0]: %p", gDmaCtl[0].rx[0].descVirt ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "*DescVirt[0]: %08X %08X", ((unsigned int*)gDmaCtl[0].rx[0].descVirt)[0], ((unsigned int*)gDmaCtl[0].rx[0].descVirt)[1] ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "DescVirt[1]: %p", gDmaCtl[0].rx[1].descVirt ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "*DescVirt[1]: %08X %08X", ((unsigned int*)gDmaCtl[0].rx[1].descVirt)[0], ((unsigned int*)gDmaCtl[0].rx[1].descVirt)[1] ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "DescPhy[0]: %p", gDmaCtl[0].rx[0].descPhy ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "DescPhy[1]: %p", gDmaCtl[0].rx[1].descPhy ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM IUDMA CTRL: %p",  &APM_PCM_IUDMA->ctrl[gDmaCtl[0].dmaChanRx] ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM IUDMA STRAM: %p", &APM_PCM_IUDMA->stram[gDmaCtl[0].dmaChanRx] ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BufVirt[0]: %X", (unsigned int)gDmaCtl[0].rx[0].bufVirt.addr32 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BufVirt[1]: %X", (unsigned int)gDmaCtl[0].rx[1].bufVirt.addr32 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BufPhy[0]: %X",  (unsigned int)gDmaCtl[0].rx[0].bufPhy.addr32 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BufPhy[1]: %X",  (unsigned int)gDmaCtl[0].rx[1].bufPhy.addr32 ));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "  DMA TX"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "----------"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "DescVirt[0]: %p", gDmaCtl[0].tx[0].descVirt ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "*DescVirt[0]: %08X %08X", ((unsigned int*)gDmaCtl[0].tx[0].descVirt)[0], ((unsigned int*)gDmaCtl[0].tx[0].descVirt)[1] ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "DescVirt[1]: %p", gDmaCtl[0].tx[1].descVirt ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "*DescVirt[1]: %08X %08X", ((unsigned int*)gDmaCtl[0].tx[1].descVirt)[0], ((unsigned int*)gDmaCtl[0].tx[1].descVirt)[1] ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "DescPhy[0]: %p", gDmaCtl[0].tx[0].descPhy ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "DescPhy[1]: %p", gDmaCtl[0].tx[1].descPhy ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM IUDMA CTRL: %p",  &APM_PCM_IUDMA->ctrl[gDmaCtl[0].dmaChanTx]));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM IUDMA STRAM: %p", &APM_PCM_IUDMA->stram[gDmaCtl[0].dmaChanTx]));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BufVirt[0]: %X", (unsigned int)gDmaCtl[0].tx[0].bufVirt.addr32 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BufVirt[1]: %X", (unsigned int)gDmaCtl[0].tx[1].bufVirt.addr32 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BufPhy[0]: %X",  (unsigned int)gDmaCtl[0].tx[0].bufPhy.addr32 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BufPhy[1]: %X",  (unsigned int)gDmaCtl[0].tx[1].bufPhy.addr32 ));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM IUDMA CTRL: %p",  &APM_PCM_IUDMA->ctrl[0]));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM IUDMA STRAM: %p", &APM_PCM_IUDMA->stram[0]));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM IUDMA CTRL: %p",  APM_PCM_IUDMA->ctrl));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM IUDMA STRAM: %p", APM_PCM_IUDMA->stram));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "DMA Chan Rx: %d", (int)gDmaCtl[0].dmaChanRx));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "DMA Chan Tx: %d", (int)gDmaCtl[0].dmaChanTx));
}
