/***************************************************************************
*    Copyright © 2008 Broadcom Corporation
*
*    This program is the proprietary software of Broadcom Corporation and/or
*    its licensors, and may only be used, duplicated, modified or
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized
*    License").  Except as set forth in an Authorized License, Broadcom
*    grants no license (express or implied), right to use, or waiver of any
*    kind with respect to the Software, and Broadcom expressly reserves all
*    rights in and to the Software and all intellectual property rights
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.
*
*
*    Except as expressly set forth in the Authorized License,
*
*    1.     This program, including its structure, sequence and
*    organization, constitutes the valuable trade secrets of Broadcom, and
*    you shall use all reasonable efforts to protect the confidentiality
*    thereof, and to use this information only in connection with your use
*    of Broadcom integrated circuit products.
*
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT
*    OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL,
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: pcm63381.c
*
****************************************************************************
*
*    Description:  PCM device driver for BCM63381
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
#include <pmc_pcm.h>
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
#include "pcm63381.h"
#include "dma63381.h"

/* -- Constants & defines -- */

#define NTR_OUT_DEBUG   0     /* Routes NTR signal to output GPIO. 
				 When enabled, please be sure the GPIO used is not affecting other operation in the board */
#define NTR_REF_CLK_KHZ 256   /* 256Khz NTR Ref clock present   */

#define PCM_NTR_IN_CTRL_SHIFT             29 /* shift for PCM_NTR_IN clock routing field */
#define PCM_NTR_IN_CTRL_SEL_DECT_NTR      3  /* Selection for routing DECT refclk as PCM_NTR_IN */
#define PCM_NTR_IN_CTRL_SEL_VDSL_NTR_0    2  /* Selection for routing VDSL_NTR[0] as PCM_NTR_IN */
#define PCM_NTR_IN_CTRL_SEL_VDSL_NTR_1    1  /* Selection for routing VDSL_NTR[1] as PCM_NTR_IN */
#define PCM_NTR_IN_CTRL_SEL_VDSL_GPIO     0  /* Selection for routing NTR_PULES_IN as PCM_NTR_IN */


/* DMA descriptor or DMA data handle */
typedef dma_addr_t      DMA_HANDLE;

#define P_NCO_FCW_MISC     0xa7c5ac47  /* 0xa7c5ac47 for MISC */
#define P_PCM_NCO_SHIFT    0x0         /* 0x0 for DPLL, 0x1 for MISC */
#define P_PCM_NCO_MUX_CNTL_TRC  0x0    /* 0x0 for TRC: Used for cable */
#define P_PCM_NCO_MUX_CNTL_DPLL 0x2    /* 0x2 for DPLL */
#define P_PCM_NCO_MUX_CNTL_MISC 0x3    /* 0x3 for MISC */

/* DPLL lock settings */
#define DPLL_LOCK_WAIT                 1           /* Whether to wait for DPLL lock or not */
#define DPLL_LOCK_MAX_WAIT             10000       /* Maximum 1ms iterations to wait for DPLL lock */
#define DPLL_RETRY_CNT                 2           /* Number of times to retry in case of PLL lock fails*/

extern int Print(char *f, ...);

/* DMA buffers for APM Rx/Tx data */
static PCM63381_DMA_BUFFER dmaRxData[PCM63381_NUM_CHANNELS*DMA_RX_NUM_BUFFERS];
static PCM63381_DMA_BUFFER dmaTxData[PCM63381_NUM_CHANNELS*DMA_TX_NUM_BUFFERS];

/* DMA Channel info (simplex - one way) for easy access to DMA channels */
typedef struct PCM63381_DMA_SIMPLEX_CHANNEL_INFO
{
   int                  iuChanNum;     /* IUDMA channel number assosiated with this APM channel */
   BCM63381_IUDMA_DESC   *dmaDescp;     /* Where are the descriptors for this channel? */
   DMA_HANDLE           dmaDescHandle;
   PCM63381_DMA_BUFFER   *dmaDatap;     /* Where are the DMA buffers to store/fetch the data to/from for each channel */
   DMA_HANDLE           dmaBufHandle;
} PCM63381_DMA_SIMPLEX_CHANNEL_INFO;

/* DMA Channel info (duplex - both ways) for easy access to DMA channels */
typedef struct PCM63381_DMA_DUPLEX_CHANNEL_INFO
{
   PCM63381_DMA_SIMPLEX_CHANNEL_INFO rxChan;  /* RX channel */
   PCM63381_DMA_SIMPLEX_CHANNEL_INFO txChan;  /* TX channel */
} PCM63381_DMA_DUPLEX_CHANNEL_INFO;  

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
extern struct dma_pool *pcm_dma_pool;
#endif

/* Initialized DMA channel info */
static PCM63381_DMA_DUPLEX_CHANNEL_INFO gPcmDmaInfo[PCM63381_NUM_CHANNELS] =
{
   {
      /* RX Channel 0 */
      {
         BCM63381_IUDMA_PCM_RX,
         NULL,    /* Will be filled with dynamically allocated value for dmaRxDesc_c */
         (DMA_HANDLE)&dmaRxData[0],
       },

      /* TX Channel 0 */
      {
         BCM63381_IUDMA_PCM_TX,
         NULL,    /* Will be filled with dynamically allocated value for dmaTxDesc_c */
         (DMA_HANDLE)&dmaTxData[0],
      }
   }
};

static XDRV_UINT8 gPcmSpecialMode;

static int pcm63381_clkInit( void );
static int pcm63381_regInit( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap );
static int pcm63381_regDeinit ( void );
static void PcmTsAllocate( int chnum, int ts);
static int pcm63381_iudmaInit( void );
static int pcm63381_iudmaDeinit( void );
static int pcm63381_dmaDescrInit( void );
static int  pcm63381_dmaDescrDeinit(void);
static int pcm63381_enable( int chan_mask );
static int pcm63381_disable( void );

void printRegs( void )
{
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM: Interrupt Masks"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "---------------"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->regs.gbl_int_mask = 0x%08X ", (unsigned int)PCM_IUDMA->regs.gbl_int_mask));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->ctrl[0].intMask = 0x%08X ", (unsigned int)PCM_IUDMA->ctrl[0].intMask));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->ctrl[1].intMask = 0x%08X ", (unsigned int)PCM_IUDMA->ctrl[1].intMask));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM: Interrupt Status"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "-----------------"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->ctrl[0].intStat = 0x%08X ", (unsigned int)PCM_IUDMA->ctrl[0].intStat));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->ctrl[1].intStat = 0x%08X ", (unsigned int)PCM_IUDMA->ctrl[1].intStat));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->reg_pcm_clk_cntl_0         = 0x%08X ", (unsigned int)PCM->reg_pcm_clk_cntl_0));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->reg_pcm_clk_cntl_1         = 0x%08X ", (unsigned int)PCM->reg_pcm_clk_cntl_1));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->reg_pcm_clk_cntl_2         = 0x%08X ", (unsigned int)PCM->reg_pcm_clk_cntl_2));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_int_pending       = 0x%08X ", (unsigned int)PCM->pcm_int_pending));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_int_mask          = 0x%08X ", (unsigned int)PCM->pcm_int_mask   ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_ctrl              = 0x%08X ", (unsigned int)PCM->pcm_ctrl       ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_chan_ctrl         = 0x%08X ", (unsigned int)PCM->pcm_chan_ctrl  ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->regs.ctrlConfig = 0x%08X ", (unsigned int)PCM_IUDMA->regs.ctrlConfig));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->ctrl[0].maxBurst                  = 0x%08X ", (unsigned int)PCM_IUDMA->ctrl[0].maxBurst                 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->ctrl[0].config                    = 0x%08X ", (unsigned int)PCM_IUDMA->ctrl[0].config                   ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->stram[0].baseDescPointer          = 0x%08X ", (unsigned int)PCM_IUDMA->stram[0].baseDescPointer         ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->stram[0].stateBytesDoneRingOffset = 0x%08X ", (unsigned int)PCM_IUDMA->stram[0].stateBytesDoneRingOffset));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->stram[0].flagsLengthStatus        = 0x%08X ", (unsigned int)PCM_IUDMA->stram[0].flagsLengthStatus       ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->stram[0].currentBufferPointer     = 0x%08X ", (unsigned int)PCM_IUDMA->stram[0].currentBufferPointer    ));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->ctrl[1].maxBurst                  = 0x%08X ", (unsigned int)PCM_IUDMA->ctrl[1].maxBurst                 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->ctrl[1].config                    = 0x%08X ", (unsigned int)PCM_IUDMA->ctrl[1].config                   ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->stram[1].baseDescPointer          = 0x%08X ", (unsigned int)PCM_IUDMA->stram[1].baseDescPointer         ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->stram[1].stateBytesDoneRingOffset = 0x%08X ", (unsigned int)PCM_IUDMA->stram[1].stateBytesDoneRingOffset));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->stram[1].flagsLengthStatus        = 0x%08X ", (unsigned int)PCM_IUDMA->stram[1].flagsLengthStatus       ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM_IUDMA->stram[1].currentBufferPointer     = 0x%08X ", (unsigned int)PCM_IUDMA->stram[1].currentBufferPointer    ));
}

/*****************************************************************************
*
*  FUNCTION: pcm63381_init
*
*  PURPOSE:
*      Intialize 63381 PCM Module
*
*  PARAMETERS: channelMask - Mask to enable specific TX/RX pcm timeslots
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int pcm63381_init( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap, XDRV_UINT32 channelMask, XDRV_UINT8 supportMode  )
{
   gPcmSpecialMode = supportMode;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "------------------ PCM CONFIGURATION START ------------------ "));

   /* Init the PCM block */
   pmc_pcm_power_up();

#if NTR_OUT_DEBUG
   /* Set pinmux for NTR out */
   bcm_set_pinmux( 65, 3 ); /* NTR_OUT0 -> GPIO22 */
#endif /* NTR_OUT_DEBUG */

   /* Add PinMux for PCM pins (TODO:Remove Magic Numbers) */
   bcm_set_pinmux( 70, PINMUX_PCM ); /* GPIO27-PinmuxSel70 - PCM_CLK   */
   bcm_set_pinmux( 71, PINMUX_PCM ); /* GPIO28-PinmuxSel71 - PCM_SDIN  */
   bcm_set_pinmux( 72, PINMUX_PCM ); /* GPIO29-PinmuxSel72 - PCM_SDOUT */
   bcm_set_pinmux( 73, PINMUX_PCM ); /* GPIO30-PinmuxSel73 - PCM_FSYN  */

   /* Initialize PCM clock */
   pcm63381_clkInit();

   /* Initialize PCM registers */
   pcm63381_regInit( chanTsMap );

   pcm63381_enable( channelMask );

   pcm63381_dmaDescrInit();

   pcm63381_iudmaInit();


   printRegs();

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "------------------ PCM CONFIGURATION END ------------------ "));

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm63381_deinit
*
*  PURPOSE:
*      Deintialize 63381 PCM Module
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int pcm63381_deinit( void )
{ 
   pcm63381_disable();

   pcm63381_iudmaDeinit();

   pcm63381_dmaDescrDeinit();

   pcm63381_regDeinit();

   printRegs();

   /* Power off the PCM block */
   pmc_pcm_power_down();

   return ( 0 );
}

#if HW_NTR_SUPPORT
/*****************************************************************************
*
*  FUNCTION: pcm_dpllStart
*
*  PURPOSE:  Initalize PCM DPLL
*
*  PARAMETERS:
*
*  RETURNS:  0 : On Success
*           -1 : On Failure
*  NOTES:
*
*****************************************************************************/
static int pcm_dpllStart(void)
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
   PCM->pcm_dpll_control |= DPLL_SOFT_INIT;
   /* Sleep for 10ms */
   bosSleep(10);

   /* Start the PLL */   
   PCM->pcm_dpll_control &= ~(DPLL_SOFT_INIT | DPLL_FORCE_ACQ | DPLL_OPEN_LOOP | DPLL_CONST_K);
   PCM->pcm_dpll_control &= ~0xF;
   PCM->pcm_dpll_control |= ( (5 << DPLL_PHS_THSHLD_SHIFT) | 12 );
   PCM->pcm_dpll_nom_freq = 0xa7c5ac47;

#if HW_NTR_SUPPORT

#if (NTR_REF_CLK_KHZ == 8)
   /* Using ref_div of   1    => 8Khz VDSL_NTR  refclk / 1    = 8Khz clock to comparator     *
    * Using local_div of 8192 => 65.536Mhz local clock / 8192 = 8Khz clock to comparator     */
   PCM->pcm_dpll_div = (0 << DPLL_REF_DIV_SHIFT) | 8191;   // 8KHz/65.536MHz = 1/8192
#elif (NTR_REF_CLK_KHZ == 16)
   /* Using ref_div of   2    => 16Khz VDSL_NTR  refclk / 2   = 8Khz clock to comparator     *
    * Using local_div of 8192 => 65.536Mhz local clock / 8192 = 8Khz clock to comparator     */
   PCM->pcm_dpll_div = (1 << DPLL_REF_DIV_SHIFT) | 8191;   // 16KHz/65.536MHz = 2/8192
#elif (NTR_REF_CLK_KHZ == 256)
   /* Using ref_div of   32   => 256Khz VDSL_NTR refclk / 32  = 8Khz clock to comparator     *
    * Using local_div of 8192 => 65.536Mhz local clock / 8192 = 8Khz clock to comparator     */    
   PCM->pcm_dpll_div = (31 << DPLL_REF_DIV_SHIFT) | 8191;   // 256KHz/65.536MHz = 32/8192
#else
   XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: Unsupported NTR_REF_CLK frequency", __FUNCTION__));      
#endif /* NTR_REF_CLK_KHZ */

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%s: Synching PLL to VDSL_NTR[0] output", __FUNCTION__));

#endif /* HW_NTR_SUPPORT */

   /* Force loop aquisition */
   PCM->pcm_dpll_control |= DPLL_FORCE_ACQ;
   PCM->pcm_dpll_control &= ~DPLL_FORCE_ACQ;

   /* Read DPLL status */
   temp = PCM->pcm_dpll_status;

#if DPLL_LOCK_WAIT
   /* Waiting for the DPLL_IN_SYNC and DPLL_ACQ_FREQ_VALID bits to be set */
   while( (!(temp & DPLL_IN_SYNC) || !(temp & DPLL_ACQ_FREQ_VALID))  )
   {
      temp = PCM->pcm_dpll_status;
      i++;

      if ( i == DPLL_LOCK_MAX_WAIT )
      {
         XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: DPLL failed to Lock, dpll_status 0x%08X = 0x%08X", __FUNCTION__, (unsigned int) &(PCM->pcm_dpll_status), (unsigned int) temp));
	 goto retry_lock;
      }
      /* Sleep for 1ms */
      bosSleep(1);
   }

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%s: DPLL Locked, Iteration = %u, dpll_status = 0x%08x, dpll_acq_freq = 0x%08x",
                  __FUNCTION__, (unsigned int)i, (unsigned int) temp, (unsigned int) PCM->pcm_dpll_acq_freq));
#else
   XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: DPLL Lock wait skipped , dpll_status = 0x%08x", __FUNCTION__, (unsigned int) temp));
#endif /* DPLL_LOCK_WAIT */

}

#endif /* HW_NTR_SUPPORT */
/*****************************************************************************
*
*  FUNCTION: pcm63381_clkInit
*
*  PURPOSE:
*      Set up APM registers to generate PCM clock.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static int pcm63381_clkInit( void )
{
   XDRV_UINT32 pcmNCOMuxCtl;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "Set up PCM registers to generate PCM clock"));
   
#if HW_NTR_SUPPORT
   /* DPLL will supply the FCW */

   /* Set PCM NCO FCW source as DPLL */
   pcmNCOMuxCtl = P_PCM_NCO_MUX_CNTL_DPLL;

   /* Choose the DPLL refclk source to VDSL_NTR[0] clock */
   MISC->miscAdslClockSample  &= ~(0x70000000);
   MISC->miscAdslClockSample  |= ( PCM_NTR_IN_CTRL_SEL_VDSL_NTR_0 << PCM_NTR_IN_CTRL_SHIFT);  /* set PCM_NTR_IN_CTRL to VDSL_NTR[0] output. */

   /* Do not use MISC FCW */
   PCM->reg_pcm_clk_cntl_0 = 0;
   /* Start DPLL */
   if ( pcm_dpllStart () == -1 ) {
        XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: Using Fixed FCW instead of DPLL", __FUNCTION__));
        /* Set PCM NCO FCW source as MISC */
        pcmNCOMuxCtl = P_PCM_NCO_MUX_CNTL_MISC;
        /* Load MISC register with target FCW  */
        PCM->reg_pcm_clk_cntl_0 = P_NCO_FCW_MISC;  
   }
#else
   /* Set PCM NCO FCW source as MISC */
   pcmNCOMuxCtl = P_PCM_NCO_MUX_CNTL_MISC;
   /* Load MISC register with target FCW  */
   PCM->reg_pcm_clk_cntl_0 = P_NCO_FCW_MISC;
#endif /* HW_NTR_SUPPORT */

   if ( gPcmSpecialMode & MODE_ISI )
   {  /* Settings with ISI */
      PCM->reg_pcm_clk_cntl_1 = 0x60000000;        // PCM_NCO clk_out = 24.576Mhz. NOTE: This value is based on o/p DPLL fcw of 0xA7C5AC47.
      PCM->pcm_msif_intf      = 0x00000001;        // R_MSIF_ENABLE = 1
   }
   else if ( gPcmSpecialMode & MODE_ZSI )
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
*  FUNCTION: pcm63381_regInit
*
*  PURPOSE:
*      Intialize 63381 PCM Module Registers
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static int pcm63381_regInit( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap )
{
   XDRV_UINT32 i;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "Set up PCM registers"));

  // PCM Control
   PCM->pcm_ctrl = ((PCM_AP_SEL)  |
                           (0 << PCM_SLAVE_SEL_SHIFT) |
                           (1 << PCM_CLOCK_INV_SHIFT) |
                           (0 << PCM_FS_INVERT_SHIFT)  |
                           (0 << PCM_FS_FREQ_16_8_SHIFT)  |
                           (0 << PCM_FS_LONG_SHIFT)  |
                           (1 << PCM_FS_TRIG_SHIFT)  |
                           (1 << PCM_DATA_OFF_SHIFT)  |
#ifndef PCM_ALAW
#  ifndef PCM_ULAW
                           (1 << PCM_DATA_16_8_SHIFT)  |
#  endif
#endif
                           (0 << PCM_LSB_FIRST_SHIFT)  |
                           (0 << PCM_LOOPBACK_SHIFT)  |
                           (0 << PCM_EXTCLK_SEL_SHIFT)  |
                           (PCM_BITS_PER_FRAME_256) );

   /* Set pcm clk to 2.048Mhz */
   if ( gPcmSpecialMode & MODE_ISI && ( PCM->pcm_msif_intf & 0x00000001 ) )
   {
      /* Enable the special div/12 clock divider for ISI mode input clock of 24.576Mhz */
      PCM->pcm_ctrl |= ((7 << PCM_CLOCK_SEL_SHIFT) & PCM_CLOCK_SEL);
   }
   else
   {
      /* Enable the div/8 clock divider for PCM/ZSI mode input clock of 16.384Mhz */
      PCM->pcm_ctrl |= ((2 << PCM_CLOCK_SEL_SHIFT) & PCM_CLOCK_SEL);
   }

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "reg_pcm_control = 0x%lx", PCM->pcm_ctrl));

   PCM->pcm_chan_ctrl = 0;


   // clear time slot allocation table
   for (i=0; i < PCM_MAX_TIMESLOT_REGS; i++)
   {
      PCM->pcm_slot_alloc_tbl[i] = 0;
   }

   for ( i = 0; i < PCM_MAX_CHANNELS; i++ )
   {
      if ( chanTsMap[i].chan == PCM_CHAN_INVALID )
      {
         break;
      }
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM,  "PCM: Channel/Line %lu Timeslot %d ", i, (int)chanTsMap[i].txTimeslot ));
      PcmTsAllocate( i, chanTsMap[i].txTimeslot );
   }

   /* Disable all direct interrupts from the PCM block */
   PCM->pcm_int_mask = 0;

   return( 0 );
}

static int pcm63381_regDeinit( void )
{
   XDRV_UINT32 i;

   // clear time slot allocation table
   for (i=0; i < PCM_MAX_TIMESLOT_REGS; i++)
   {
      PCM->pcm_slot_alloc_tbl[i] = 0;
   }

   return( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm63381_enable
*
*  PURPOSE:
*      Enable 63381 PCM Module
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int pcm63381_enable( int chan_mask )
{
   // Enable PCM operation
   PCM->pcm_ctrl |= PCM_ENABLE ;

   // Enable receive/transmit channels
   PCM->pcm_chan_ctrl |= chan_mask & ( PCM_TX0_EN | PCM_TX1_EN | PCM_TX2_EN | PCM_TX3_EN |
                                       PCM_TX4_EN | PCM_TX5_EN | PCM_TX6_EN | PCM_TX7_EN  );

   PCM->pcm_chan_ctrl |= chan_mask & ( PCM_RX0_EN | PCM_RX1_EN | PCM_RX2_EN | PCM_RX3_EN |
                                       PCM_RX4_EN | PCM_RX5_EN | PCM_RX6_EN | PCM_RX7_EN  );

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm63381_disable
*
*  PURPOSE:
*      Disable 63381 PCM Module
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int pcm63381_disable( void )
{
   // Disable receive/transmit channels
   PCM->pcm_chan_ctrl = 0;

   // Disable PCM operation
   PCM->pcm_ctrl &= ~PCM_ENABLE ;

   PCM->pcm_ctrl = 0;

   PCM->pcm_int_pending |=  PCM->pcm_int_pending;

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm63381_iudmaInit
*
*  PURPOSE:
*      Intialize 63381 IUDMA registers.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES: PCM is channels 0 and 1.
*
*****************************************************************************/
static int pcm63381_iudmaInit( void )
{
   int chnum;
   volatile PcmIudma *iuDmap = PCM_IUDMA;

   /* Initialize the Rx/Tx IUDMA pair for PCM channel */
   for (chnum=0; chnum < PCM63381_NUM_CHANNELS; chnum++)
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "initIudma: chan %d, descBase 0x%x, descBaseIudma 0x%x",
                        gPcmDmaInfo[chnum].rxChan.iuChanNum,
                        (unsigned int)gPcmDmaInfo[chnum].rxChan.dmaDescp,
                        CacheVirtToPhyAddress(gPcmDmaInfo[chnum].rxChan.dmaDescp)
                        ));

      /* Explicitly Reset DMA channel */
      iuDmap->regs.channelReset |= ( (1 << gPcmDmaInfo[chnum].rxChan.iuChanNum) | (1 << gPcmDmaInfo[chnum].txChan.iuChanNum) );

       /* Init RX side */
      iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].maxBurst = 4; /* in 32-bit words */
      iuDmap->stram[gPcmDmaInfo[chnum].rxChan.iuChanNum].baseDescPointer = CacheVirtToPhyAddress(gPcmDmaInfo[chnum].rxChan.dmaDescp);
      iuDmap->stram[gPcmDmaInfo[chnum].rxChan.iuChanNum].stateBytesDoneRingOffset = 0;
      iuDmap->stram[gPcmDmaInfo[chnum].rxChan.iuChanNum].flagsLengthStatus = 0;
      iuDmap->stram[gPcmDmaInfo[chnum].rxChan.iuChanNum].currentBufferPointer = 0;
      iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].intStat |= iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].intStat;
      iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].intMask = BCM63381_IUDMA_INTMASK_BDONE | BCM63381_IUDMA_INTMASK_NOTVLD;

      /* Enable PCMDMA interrupts for RX only */
      iuDmap->regs.gbl_int_mask |= ( 1 << gPcmDmaInfo[chnum].rxChan.iuChanNum );  /* Global Int Mask */

      XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "initIudma: chan %d, descBase 0x%x, descBaseIudma 0x%x",
                        gPcmDmaInfo[chnum].txChan.iuChanNum,
                        (unsigned int)gPcmDmaInfo[chnum].txChan.dmaDescp,
                        CacheVirtToPhyAddress(gPcmDmaInfo[chnum].txChan.dmaDescp)
                        ));
      /* Init TX side */
      iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].maxBurst = 4; /* in 32-bit words */
      iuDmap->stram[gPcmDmaInfo[chnum].txChan.iuChanNum].baseDescPointer = CacheVirtToPhyAddress(gPcmDmaInfo[chnum].txChan.dmaDescp);
      iuDmap->stram[gPcmDmaInfo[chnum].txChan.iuChanNum].stateBytesDoneRingOffset = 0;
      iuDmap->stram[gPcmDmaInfo[chnum].txChan.iuChanNum].flagsLengthStatus = 0;
      iuDmap->stram[gPcmDmaInfo[chnum].txChan.iuChanNum].currentBufferPointer = 0;
      iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].intStat |= iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].intStat;
      iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].intMask = BCM63381_IUDMA_INTMASK_BDONE | BCM63381_IUDMA_INTMASK_NOTVLD;

      /* Enable PCMDMA interrupts for RX only */
      iuDmap->regs.gbl_int_mask &= ~( 1 << gPcmDmaInfo[chnum].txChan.iuChanNum );  /* Global Int Mask */
   }

   /* Enable the IUDMA master */
   iuDmap->regs.ctrlConfig = IUDMA_REGS_CTRLCONFIG_MASTER_EN;

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm63381_iudmaDeinit
*
*  PURPOSE:
*      De-intialize 63381 IUDMA registers.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES: PCM is channels 0 and 1.
*
*****************************************************************************/
static int pcm63381_iudmaDeinit( void)
{
   int chnum;
   volatile PcmIudma *iuDmap = PCM_IUDMA;
   int i=0;

   /* Halt individual channels first */
   for (chnum=0; chnum < PCM63381_NUM_CHANNELS; chnum++)
   {

      /* Deinit RX side */
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "RX DMA STATUS (1=busy 0=IDLE) : 0x%08x", (unsigned int)iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].config));
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "HALTING RX DMA...."));

      /* Set the burstHalt bit while clearing endma bit */
      iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].config = ( BCM63381_IUDMA_CONFIG_BURSTHALT );

      while( (iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].config & BCM63381_IUDMA_CONFIG_ENDMA) != 0x00000000 )
      {
         /* Set the burstHalt bit while clearing endma bit */
         iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].config = ( BCM63381_IUDMA_CONFIG_BURSTHALT );
         XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "RX DMA HALT Waiting, iteration:%d",i));
         i++;
      }

      i=0;

      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "RX DMA IDLE"));
      iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].intMask = 0;
      iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].intStat |= iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].intStat;

      /* Deinit TX side */

      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "TX DMA STATUS (1=busy 0=IDLE) : 0x%08x", (unsigned int)iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].config));
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "HALTING TX DMA...."));

      /* Set the burstHalt bit while clearing endma bit */
      iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].config = ( BCM63381_IUDMA_CONFIG_BURSTHALT );

      while( (iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].config & BCM63381_IUDMA_CONFIG_ENDMA) != 0x00000000 )
      {
         /* Set the burstHalt bit while clearing endma bit */
         iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].config = ( BCM63381_IUDMA_CONFIG_BURSTHALT );
         XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "TX DMA HALT Waiting, iteration:%d",i));
         i++;
      }
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "TX DMA IDLE"));

      iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].intMask = 0;
      iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].intStat |= iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].intStat;

      /* Explicitly Reset DMA channel */
      iuDmap->regs.channelReset |= ( (1 << gPcmDmaInfo[chnum].rxChan.iuChanNum) | (1 << gPcmDmaInfo[chnum].txChan.iuChanNum) );

   }

   /* Disable pcm iudma controller */
   iuDmap->regs.ctrlConfig &= ~IUDMA_REGS_CTRLCONFIG_MASTER_EN;


   return( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm63381_dmaDescrInit
*
*  PURPOSE:
*      Intialize 63381 DMA descriptors.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static int pcm63381_dmaDescrInit( void )
{
   XDRV_UINT8                       i,chnum;
   volatile BCM63381_IUDMA_DESC*     dmaTxDesc;
   volatile BCM63381_IUDMA_DESC*     dmaRxDesc;
   volatile BCM63381_IUDMA_DESC*     dmaTxDesc_c;
   volatile BCM63381_IUDMA_DESC*     dmaRxDesc_c;
   volatile PCM63381_DMA_BUFFER*     dmaRxData;
   volatile PCM63381_DMA_BUFFER*     dmaTxData;
   PCM63381_DMA_SIMPLEX_CHANNEL_INFO *dmaChanInfop;
   unsigned                         bufferSizeBytes = 0;
   XDRV_UINT32                      rxDescriptorArea;
   XDRV_UINT32                      txDescriptorArea;
   XDRV_UINT32                      rxBufferArea;
   XDRV_UINT32                      txBufferArea;
   volatile XDRV_UINT32             *dmaDatap;

   rxDescriptorArea = sizeof(BCM63381_IUDMA_DESC) * PCM63381_NUM_CHANNELS * DMA_RX_NUM_BUFFERS + sizeof(XDRV_UINT8) * BCM63381_DMA_UBUS_BYTES;
   txDescriptorArea = sizeof(BCM63381_IUDMA_DESC) * PCM63381_NUM_CHANNELS * DMA_TX_NUM_BUFFERS + sizeof(XDRV_UINT8) * BCM63381_DMA_UBUS_BYTES;

   /* Allocate receive and transmit descriptors */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
   dmaRxDesc_c = dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, &gPcmDmaInfo[0].rxChan.dmaDescHandle );
   dmaTxDesc_c = dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, &gPcmDmaInfo[0].txChan.dmaDescHandle );
#else
   dmaRxDesc_c = dma_alloc_coherent( NULL, rxDescriptorArea, &gPcmDmaInfo[0].rxChan.dmaDescHandle, GFP_KERNEL );
   dmaTxDesc_c = dma_alloc_coherent( NULL, txDescriptorArea, &gPcmDmaInfo[0].txChan.dmaDescHandle, GFP_KERNEL );
#endif
   if ( (dmaRxDesc_c == NULL) || (dmaTxDesc_c == NULL) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_PCM, "Could not allocate memory for DMA descriptors !!! "));
      return( -1 );
   }

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "rxDescriptorArea       = %d ", (int)rxDescriptorArea));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "txDescriptorArea       = %d ", (int)txDescriptorArea));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "Unaligned dmaRxDesc_c  = 0x%08X ", (unsigned int)dmaRxDesc_c));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "Unaligned dmaTxDesc_c  = 0x%08X ", (unsigned int)dmaTxDesc_c));

   /* Fill in the global PCM DMA structure with the allocated pointers to descriptors */
   for (i=0; i < PCM63381_NUM_CHANNELS; i++)
   {
      /* Align DMA descriptior to 64Bit Ubus */
      dmaRxDesc_c = (volatile BCM63381_IUDMA_DESC *) ALIGN_DATA( (unsigned int) dmaRxDesc_c, BCM63381_DMA_UBUS_BYTES );
      dmaTxDesc_c = (volatile BCM63381_IUDMA_DESC *) ALIGN_DATA( (unsigned int) dmaTxDesc_c, BCM63381_DMA_UBUS_BYTES );

      gPcmDmaInfo[i].rxChan.dmaDescp = (BCM63381_IUDMA_DESC*)(dmaRxDesc_c + i * DMA_RX_NUM_BUFFERS);
      gPcmDmaInfo[i].txChan.dmaDescp = (BCM63381_IUDMA_DESC*)(dmaTxDesc_c + i * DMA_TX_NUM_BUFFERS);
   }

   /* Clear the descriptors */
   memset( (void *) CacheToNonCacheVirtAddress( dmaRxDesc_c ), 0, rxDescriptorArea );
   memset( (void *) CacheToNonCacheVirtAddress( dmaTxDesc_c ), 0, txDescriptorArea );
   xdrvDCacheClearByAddress( XDRV_CACHE_INVALIDATE, (void*)dmaTxDesc_c, sizeof( dmaTxDesc_c ) );
   xdrvDCacheClearByAddress( XDRV_CACHE_INVALIDATE, (void*)dmaRxDesc_c, sizeof( dmaRxDesc_c ) );

   /* Allocate data buffers */
   rxBufferArea = sizeof(XDRV_UINT32) * (PCM63381_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM63381_NUM_CHANNELS * DMA_RX_NUM_BUFFERS;
   txBufferArea = sizeof(XDRV_UINT32) * (PCM63381_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM63381_NUM_CHANNELS * DMA_TX_NUM_BUFFERS;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
   dmaRxData = dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, &gPcmDmaInfo[0].rxChan.dmaBufHandle );
   dmaTxData = dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, &gPcmDmaInfo[0].txChan.dmaBufHandle );
#else
   dmaRxData = dma_alloc_coherent( NULL, rxBufferArea, &gPcmDmaInfo[0].rxChan.dmaBufHandle, GFP_KERNEL );
   dmaTxData = dma_alloc_coherent( NULL, txBufferArea, &gPcmDmaInfo[0].txChan.dmaBufHandle, GFP_KERNEL );
#endif

   if ( (dmaRxData == NULL) || (dmaTxData == NULL ) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_PCM, "Could not allocate memory for DMA data buffers !!! "));
      return( -1 );
   }

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "rxBufferArea          = %d ", (int)rxBufferArea));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "txBufferArea          = %d ", (int)txBufferArea));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "Unaligned dmaRxData   = 0x%08X ", (unsigned int)dmaRxData));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "Unaligned dmaTxData   = 0x%08X ", (unsigned int)dmaTxData));

   /* Fill in the global PCM DMA structure with the allocated pointers to descriptors */
   for (i=0; i < PCM63381_NUM_CHANNELS; i++)
   {
      gPcmDmaInfo[i].rxChan.dmaDatap = (PCM63381_DMA_BUFFER*)(dmaRxData + i * DMA_RX_NUM_BUFFERS);
      gPcmDmaInfo[i].txChan.dmaDatap = (PCM63381_DMA_BUFFER*)(dmaTxData + i * DMA_TX_NUM_BUFFERS);
   }

   /* Reset the data buffers */
   memset( (void*) dmaRxData, 0, rxBufferArea);
   memset( (void*) dmaTxData, 0, txBufferArea);

   for (chnum=0; chnum < PCM63381_NUM_CHANNELS; chnum++)
   {
      /* Get the pointer to TX DMA channel info */
      dmaChanInfop = &gPcmDmaInfo[chnum].txChan;

      /* Get the pointer to Tx descriptors for this channel. Convert to non-cached addresses */
      dmaTxDesc = (volatile BCM63381_IUDMA_DESC *)CacheToNonCacheVirtAddress((XDRV_UINT32)dmaChanInfop->dmaDescp);

      /* Init Tx DMA descriptor table */
      for( i = 0; i < DMA_TX_NUM_BUFFERS; i++ )
      {
         /* TX descriptor buffers should be enough to hold packetRate ms worth of sampleRate data */
         bufferSizeBytes = PCM63381_DMA_MAX_BUFFER_SIZE_BYTES;

         /* Each buffer is a complete packet */
         dmaTxDesc[i].flagsLengthStatus.status = BCM63381_IUDMA_DESC_STATUS_SOP |
                                                 BCM63381_IUDMA_DESC_STATUS_EOP;

         if( i == 0 )
         {
            /* First buffer - prime the TX fifo */
            dmaTxDesc[i].flagsLengthStatus.status |= BCM63381_IUDMA_DESC_STATUS_OWN;

            /* Prime with packetRate + 1ms for processing head room from the start of the ISR
            to allow us time to provide another packetRate worth of data to the TX APM DMA. */
            bufferSizeBytes = PCM63381_DMA_MAX_BUFFER_SIZE_BYTES;
         }

         if( i == ( DMA_TX_NUM_BUFFERS - 1 ) )
         {
            /* Last buffer - set the WRAP bit */
            dmaTxDesc[i].flagsLengthStatus.status |= BCM63381_IUDMA_DESC_STATUS_WRAP;
         }

         /* Check if we can accomodate the desired buffer size */
         if (bufferSizeBytes <= PCM63381_DMA_MAX_BUFFER_SIZE_BYTES)
         {
            dmaTxDesc[i].flagsLengthStatus.length = bufferSizeBytes;
         }
         else
         {
            /* Sorry, the allocated buffer size is smaller than what
            ** you want the buffer size configured to, so I can not do this.
            ** You can either increase the compile time buffer size (PCM63381_DMA_MAX_BUFFER_SIZE),
            ** or decrease the buffer size via packetRate/sampleRate */
            XDRV_LOG_ERROR((XDRV_LOG_MOD_PCM, "XDRV_PCM: ERROR - Can not accomodate configured buffer size (%d) ", bufferSizeBytes));
            return ( -1 );
         }


         /* Cache operations (invalidate and write-back) are performed on these
         ** buffers before and after DMA operations. Since cache operations
         ** operate on the entire cache-line, it is necessary to make the
         ** buffers aligned on the cache-line size (16-bytes for the 3368).
         ** Failure to do so may result in corruption of data that shares the
         ** same cache-line as the DMA buffer. */
         dmaDatap = dmaChanInfop->dmaDatap[i].dmaBuff;
         dmaDatap = (volatile XDRV_UINT32 *) ALIGN_DATA( (unsigned int) dmaDatap, DCACHE_LINE_SIZE_BYTES );

         /*  Use physical address */
         dmaTxDesc[i].addr.addr32 = CacheVirtToPhyAddress( (XDRV_UINT32)dmaDatap );

         XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "Tx Desc: chan %d, buf %d, sts 0x%x, len %d, bufp 0x%x ",
                         chnum, i, dmaTxDesc[i].flagsLengthStatus.status,  dmaTxDesc[i].flagsLengthStatus.length,  (unsigned int)dmaTxDesc[i].addr.addr32 ));
      }

      /* Get the pointer to RX DMA channel info */
      dmaChanInfop=&gPcmDmaInfo[chnum].rxChan;

      /* Get the pointer to Rx descriptors for this channel. Convert to non-cached addresses */
      dmaRxDesc = (volatile BCM63381_IUDMA_DESC *)CacheToNonCacheVirtAddress((XDRV_UINT32)dmaChanInfop->dmaDescp);

      /* Init Rx DMA descriptor table */
      for( i = 0; i < DMA_RX_NUM_BUFFERS; i++ )
      {
         /* RX descriptor buffers should be enough to hold packetRate ms worth of sampleRate data */
         bufferSizeBytes = PCM63381_DMA_MAX_BUFFER_SIZE_BYTES;

         /* Give ownership of all descriptors to RX DMA */
         dmaRxDesc[i].flagsLengthStatus.status = BCM63381_IUDMA_DESC_STATUS_OWN;

         if( i == ( DMA_RX_NUM_BUFFERS - 1 ) )
         {
            /* Last descriptor - set the wrap bit */
            dmaRxDesc[i].flagsLengthStatus.status |= BCM63381_IUDMA_DESC_STATUS_WRAP;
         }

         /* Check if we can accomodate the desired buffer size */
         if (bufferSizeBytes <= PCM63381_DMA_MAX_BUFFER_SIZE_BYTES)
         {
            /* Buffer size in bytes */
            dmaRxDesc[i].flagsLengthStatus.length = bufferSizeBytes;
         }
         else
         {
            /* Sorry, the allocated buffer size is smaller than what
            ** you want the buffer size configured to, so I can not do this.
            ** You can either increase the compile time buffer size (PCM63381_DMA_MAX_BUFFER_SIZE),
            ** or decrease the buffer size via packetRate/sampleRate */
            XDRV_LOG_ERROR((XDRV_LOG_MOD_PCM, "XDRV_PCM: ERROR - Can not accomodate configured buffer size (%d) ", bufferSizeBytes));
            return ( -1 );
         }

         /* Cache operations (invalidate and write-back) are performed on these
         ** buffers before and after DMA operations. Since cache operations
         ** operate on the entire cache-line, it is necessary to make the
         ** buffers aligned on the cache-line size (16-bytes for the 63381).
         ** Failure to do so may result in corruption of data that shares the
         ** same cache-line as the DMA buffer. */
         dmaDatap = dmaChanInfop->dmaDatap[i].dmaBuff;
         dmaDatap = (volatile XDRV_UINT32 *) ALIGN_DATA( (unsigned int) dmaDatap, DCACHE_LINE_SIZE_BYTES );

         /* Use physical address */
         dmaRxDesc[i].addr.addr32 = CacheVirtToPhyAddress( (XDRV_UINT32)dmaDatap );

         XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "Rx Desc: chan %d, buf %d, sts 0x%x, len %d, bufp 0x%x ",
                     chnum, i, dmaRxDesc[i].flagsLengthStatus.status, dmaRxDesc[i].flagsLengthStatus.length, (unsigned int)dmaRxDesc[i].addr.addr32 ));
      } /* for */

   } /* for */

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm63381_dmaDescrDeinit
*
*  PURPOSE:
*      Deintialize 63381 DMA descriptors.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static int pcm63381_dmaDescrDeinit( void )
{
   XDRV_UINT8                       i;
   XDRV_UINT32                      rxDescriptorArea;
   XDRV_UINT32                      txDescriptorArea;
   XDRV_UINT32                      rxBufferArea;
   XDRV_UINT32                      txBufferArea;

   rxDescriptorArea = sizeof(BCM63381_IUDMA_DESC) * PCM63381_NUM_CHANNELS * DMA_RX_NUM_BUFFERS;
   txDescriptorArea = sizeof(BCM63381_IUDMA_DESC) * PCM63381_NUM_CHANNELS * DMA_TX_NUM_BUFFERS;

   /* Clear the descriptors */
   memset( (void *) CacheToNonCacheVirtAddress( gPcmDmaInfo[0].rxChan.dmaDescp ), 0, rxDescriptorArea);
   memset( (void *) CacheToNonCacheVirtAddress( gPcmDmaInfo[0].txChan.dmaDescp ), 0, txDescriptorArea);

   /* Free receive and transmit descriptors */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
   dma_pool_free( pcm_dma_pool, gPcmDmaInfo[0].rxChan.dmaDescp, gPcmDmaInfo[0].rxChan.dmaDescHandle );
   dma_pool_free( pcm_dma_pool, gPcmDmaInfo[0].txChan.dmaDescp, gPcmDmaInfo[0].txChan.dmaDescHandle );
#else
   dma_free_coherent(NULL, rxDescriptorArea, gPcmDmaInfo[0].rxChan.dmaDescp, gPcmDmaInfo[0].rxChan.dmaDescHandle);
   dma_free_coherent(NULL, txDescriptorArea, gPcmDmaInfo[0].txChan.dmaDescp, gPcmDmaInfo[0].txChan.dmaDescHandle);
#endif

   for (i=0; i < PCM63381_NUM_CHANNELS; i++)
   {
      gPcmDmaInfo[i].rxChan.dmaDescp = NULL;
      gPcmDmaInfo[i].txChan.dmaDescp = NULL;
   }


   rxBufferArea = sizeof(XDRV_UINT32) * (PCM63381_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM63381_NUM_CHANNELS * DMA_RX_NUM_BUFFERS;
   txBufferArea = sizeof(XDRV_UINT32) * (PCM63381_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM63381_NUM_CHANNELS * DMA_TX_NUM_BUFFERS;

   /* Clear the data buffers */
   memset( (void*) gPcmDmaInfo[0].rxChan.dmaDatap, 0, rxBufferArea);
   memset( (void*) gPcmDmaInfo[0].txChan.dmaDatap, 0, txBufferArea);

   /* Free data buffers */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
   dma_pool_free( pcm_dma_pool, gPcmDmaInfo[0].rxChan.dmaDatap, gPcmDmaInfo[0].rxChan.dmaBufHandle );
   dma_pool_free( pcm_dma_pool, gPcmDmaInfo[0].txChan.dmaDatap, gPcmDmaInfo[0].txChan.dmaBufHandle );
#else
   dma_free_coherent(NULL, rxBufferArea, gPcmDmaInfo[0].rxChan.dmaDatap, gPcmDmaInfo[0].rxChan.dmaBufHandle);
   dma_free_coherent(NULL, txBufferArea, gPcmDmaInfo[0].txChan.dmaDatap, gPcmDmaInfo[0].txChan.dmaBufHandle);
#endif

   /* Fill in the global PCM DMA structure with the allocated pointers to descriptors */
   for (i=0; i < PCM63381_NUM_CHANNELS; i++)
   {
      gPcmDmaInfo[i].rxChan.dmaDatap = NULL;
      gPcmDmaInfo[i].txChan.dmaDatap = NULL;
   }

   return ( 0 );
}


/*****************************************************************************
*
*  FUNCTION: PcmTsAllocate
*
*  PURPOSE:
*      Allocate PCM timeslot
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
static void PcmTsAllocate( int chnum, int ts)
{
   int         WORD_OFFSET;
   int         bit_offset = 0;
   XDRV_UINT32 entry, mask;

   WORD_OFFSET = ts >> 3;
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

   entry = PCM->pcm_slot_alloc_tbl[WORD_OFFSET];
   mask = ( (0xf << bit_offset) * (-1) ) - 1;
   entry &= mask;
   entry |= ( (PCM_TS_VALID | chnum) << bit_offset) ;

   PCM->pcm_slot_alloc_tbl[WORD_OFFSET] = entry;

  XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "   Channel %d assigned to timeslot %d",chnum,ts));
}


