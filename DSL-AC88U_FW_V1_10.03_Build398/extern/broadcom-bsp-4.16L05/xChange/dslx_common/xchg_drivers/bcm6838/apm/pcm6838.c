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
*    Filename: pcm6838.c
*
****************************************************************************
*
*    Description:  PCM device driver for BCM6838
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */
/*
** Always include this file before any other include files
** to avoid conflicts with remapped printf command
*/
#include <kernelMainWrapper.h>
#include <linux/dma-mapping.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
#include <linux/dmapool.h>
#endif
#include <pmc_drv.h>
#include <BPCM.h>
#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <xchg_pcm.h>
#include <xchg_dma.h>
#include "pcm6838.h"
#include "apm6838.h"
#include "dma6838.h"
#include <bcm6838.h>
#include <xdrvCache.h>
#include <xchgAssert.h>
#include <shared_utils.h>
#include <bosSleep.h>
/* DMA descriptor or DMA data handle */
typedef dma_addr_t      DMA_HANDLE;

#define P_NCO_FCW_MISC     0xa7c5ac47  /* 0xa7c5ac47 for DPLL */
#define P_PCM_NCO_SHIFT    0x0         /* 0x0 for DPLL, 0x1 for MISC */
#define P_PCM_NCO_MUX_CNTL_DPLL 0x2    /* 0x2 for DPLL */
#define P_PCM_NCO_MUX_CNTL_MISC 0x3    /* 0x3 for MISC */




extern int Print(char *f, ...);

/* DMA buffers for APM Rx/Tx data */
static PCM6838_DMA_BUFFER dmaRxData[PCM6838_NUM_CHANNELS*DMA_RX_NUM_BUFFERS];
static PCM6838_DMA_BUFFER dmaTxData[PCM6838_NUM_CHANNELS*DMA_TX_NUM_BUFFERS];

/* DMA Channel info (simplex - one way) for easy access to DMA channels */
typedef struct PCM6838_DMA_SIMPLEX_CHANNEL_INFO
{
   int                  iuChanNum;     /* IUDMA channel number assosiated with this APM channel */
   BCM6838_IUDMA_DESC   *dmaDescp;     /* Where are the descriptors for this channel? */
   DMA_HANDLE           dmaDescHandle;
   PCM6838_DMA_BUFFER   *dmaDatap;     /* Where are the DMA buffers to store/fetch the data to/from for each channel */
   DMA_HANDLE           dmaBufHandle;
} PCM6838_DMA_SIMPLEX_CHANNEL_INFO;

/* DMA Channel info (duplex - both ways) for easy access to DMA channels */
typedef struct PCM6838_DMA_DUPLEX_CHANNEL_INFO
{
   PCM6838_DMA_SIMPLEX_CHANNEL_INFO rxChan;  /* RX channel */
   PCM6838_DMA_SIMPLEX_CHANNEL_INFO txChan;  /* TX channel */
} PCM6838_DMA_DUPLEX_CHANNEL_INFO;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
extern struct dma_pool *pcm_dma_pool;
#endif

/* ---- Public Variables ------------------------------------ */
extern int gPllClockFailed;

/* Initialized DMA channel info */
static PCM6838_DMA_DUPLEX_CHANNEL_INFO gPcmDmaInfo[PCM6838_NUM_CHANNELS] =
{
   {
      /* RX Channel 0 */
      {
         BCM6838_APM_IUDMA_PCM_RX,
         NULL,    /* Will be filled with dynamically allocated value for dmaRxDesc_c */
         (DMA_HANDLE)&dmaRxData[0],
       },

      /* TX Channel 0 */
      {
         BCM6838_APM_IUDMA_PCM_TX,
         NULL,    /* Will be filled with dynamically allocated value for dmaTxDesc_c */
         (DMA_HANDLE)&dmaTxData[0],
      }
   }
};

static XDRV_UINT8 gPcmSpecialMode;

static int pcm6838_clkInit( void );
static int pcm6838_regInit( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap );
static int pcm6838_regDeinit ( void );
static void PcmTsAllocate( int chnum, int ts);
static int pcm6838_iudmaInit( void );
static int pcm6838_iudmaDeinit( void );
static int pcm6838_dmaDescrInit( void );
static int  pcm6838_dmaDescrDeinit(void);
static int pcm6838_enable( int chan_mask );
static int pcm6838_disable( void );

void printRegs( void )
{
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM: Interrupt Masks"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "---------------"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->regs.gbl_int_mask = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->regs.gbl_int_mask));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->ctrl[4].intMask = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->ctrl[4].intMask));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->ctrl[5].intMask = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->ctrl[5].intMask));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM: Interrupt Status"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "-----------------"));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->ctrl[4].intStat = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->ctrl[4].intStat));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->ctrl[5].intStat = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->ctrl[5].intStat));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->reg_pcm_clk_cntl_0         = 0x%08X ", (unsigned int)PCM->reg_pcm_clk_cntl_0));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->reg_pcm_clk_cntl_1         = 0x%08X ", (unsigned int)PCM->reg_pcm_clk_cntl_1));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->reg_pcm_clk_cntl_2         = 0x%08X ", (unsigned int)PCM->reg_pcm_clk_cntl_2));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_int_pending       = 0x%08X ", (unsigned int)PCM->pcm_int_pending));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_int_mask          = 0x%08X ", (unsigned int)PCM->pcm_int_mask   ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_ctrl              = 0x%08X ", (unsigned int)PCM->pcm_ctrl       ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "PCM->pcm_chan_ctrl         = 0x%08X ", (unsigned int)PCM->pcm_chan_ctrl  ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->regs.ctrlConfig = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->regs.ctrlConfig));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->ctrl[4].maxBurst                  = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->ctrl[4].maxBurst                 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->ctrl[4].config                    = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->ctrl[4].config                   ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->stram[4].baseDescPointer          = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->stram[4].baseDescPointer         ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->stram[4].stateBytesDoneRingOffset = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->stram[4].stateBytesDoneRingOffset));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->stram[4].flagsLengthStatus        = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->stram[4].flagsLengthStatus       ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->stram[4].currentBufferPointer     = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->stram[4].currentBufferPointer    ));

   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->ctrl[5].maxBurst                  = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->ctrl[5].maxBurst                 ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->ctrl[5].config                    = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->ctrl[5].config                   ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->stram[5].baseDescPointer          = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->stram[5].baseDescPointer         ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->stram[5].stateBytesDoneRingOffset = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->stram[5].stateBytesDoneRingOffset));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->stram[5].flagsLengthStatus        = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->stram[5].flagsLengthStatus       ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "BCM6838_APM_IUDMA_PTR->stram[5].currentBufferPointer     = 0x%08X ", (unsigned int)BCM6838_APM_IUDMA_PTR->stram[5].currentBufferPointer    ));
}

/*****************************************************************************
*
*  FUNCTION: pcm6838_init
*
*  PURPOSE:
*      Intialize 6838 PCM Module
*
*  PARAMETERS: channelMask - Mask to enable specific TX/RX pcm timeslots
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int pcm6838_init( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap, XDRV_UINT32 channelMask, XDRV_UINT8 supportMode  )
{
   BPCM_SR_CONTROL sr_control;

   gPcmSpecialMode = supportMode;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "------------------ PCM CONFIGURATION START ------------------ "));

   /* RESET PCM via BPCM ( Block Power Control Module ) */
   ResetZone(PMB_ADDR_APM, APM_Zone_PCM  );

   /* Assert PCM SoftResets via BPCM */
   ReadBPCMRegister(PMB_ADDR_APM, BPCMRegOffset(sr_control), &sr_control.Reg32);
   sr_control.Bits.sr |= ( BPCM_APM_SRESET_PCM_N );
   WriteBPCMRegister(PMB_ADDR_APM, BPCMRegOffset(sr_control), sr_control.Reg32);

   /* Sleep to ensure full soft reset */
   bosSleep(1000);

   /* De-assert PCM SoftResets via BPCM */
   ReadBPCMRegister(PMB_ADDR_APM, BPCMRegOffset(sr_control), &sr_control.Reg32);
   sr_control.Bits.sr &= ~( BPCM_APM_SRESET_PCM_N );
   WriteBPCMRegister(PMB_ADDR_APM, BPCMRegOffset(sr_control), sr_control.Reg32);

    /* Sleep to ensure full soft reset */
   bosSleep(1000);

   /* Add PinMux for PCM pins (TODO:Remove Magic Numbers) */
   set_pinmux( 0, PINMUX_PCM ); /* GPIO0 - PCM_SDIN */
   set_pinmux( 1, PINMUX_PCM ); /* GPIO1 - PCM_SDOUT */
   set_pinmux( 2, PINMUX_PCM ); /* GPIO2 - PCM_CLK */
   set_pinmux( 3, PINMUX_PCM ); /* GPIO3 - PCM_FS */

   /* Initialize PCM clock */
   pcm6838_clkInit();

   /* Initialize PCM registers */
   pcm6838_regInit( chanTsMap );

   pcm6838_enable( channelMask );

   pcm6838_dmaDescrInit();

   pcm6838_iudmaInit();


   printRegs();

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "------------------ PCM CONFIGURATION END ------------------ "));

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm6838_deinit
*
*  PURPOSE:
*      Deintialize 6838 PCM Module
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int pcm6838_deinit( void )
{
   BPCM_SR_CONTROL sr_control;

   pcm6838_disable();

   pcm6838_iudmaDeinit();

   pcm6838_dmaDescrDeinit();

   pcm6838_regDeinit();

   printRegs();

    /* Assert PCM SoftResets via BPCM */
   ReadBPCMRegister(PMB_ADDR_APM, BPCMRegOffset(sr_control), &sr_control.Reg32);
   sr_control.Bits.sr |= ( BPCM_APM_SRESET_PCM_N );
   WriteBPCMRegister(PMB_ADDR_APM, BPCMRegOffset(sr_control), sr_control.Reg32);

   /* Sleep to ensure full soft reset */
   bosSleep(1000);

   PowerOffZone(PMB_ADDR_APM, APM_Zone_PCM  );

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm6838_clkInit
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
static int pcm6838_clkInit( void )
{

   /* This code assumes that the DPLL was initialized already in the apm_pllStart() in apm6838.c */

   XDRV_LOG_INFO(( XDRV_LOG_MOD_PCM, "Set up PCM registers to generate PCM clock"));

   /* Load MISC register with target FCW from DPLL */
   PCM->reg_pcm_clk_cntl_0 = P_NCO_FCW_MISC;

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

   /* Use PLL */
   if ( (HW_NTR_SUPPORT)  &&  (! gPllClockFailed)  ) {

     PCM->reg_pcm_clk_cntl_2 = ((P_PCM_NCO_SHIFT & PCM_NCO_SHIFT)
                                | ((P_PCM_NCO_MUX_CNTL_DPLL << 4) & PCM_NCO_MUX_CNTL)
                                |  (PCM_NCO_SOFT_INIT));
     
     PCM->reg_pcm_clk_cntl_2 = ((P_PCM_NCO_SHIFT & PCM_NCO_SHIFT)
                                | ((P_PCM_NCO_MUX_CNTL_DPLL << 4) & PCM_NCO_MUX_CNTL));
   }
   /* Use fixed FCW */
   else 
   {
     /* Soft reset NCO */
     PCM->reg_pcm_clk_cntl_2 = ((P_PCM_NCO_SHIFT & PCM_NCO_SHIFT)
                                | ((P_PCM_NCO_MUX_CNTL_MISC << 4) & PCM_NCO_MUX_CNTL)
                                |  (PCM_NCO_SOFT_INIT));
     
     /* Toggle LOAD_MISC */
     PCM->reg_pcm_clk_cntl_2 = ((P_PCM_NCO_SHIFT & PCM_NCO_SHIFT)
                                | ((P_PCM_NCO_MUX_CNTL_MISC << 4) & PCM_NCO_MUX_CNTL)
                                |  (PCM_NCO_LOAD_MISC));

     PCM->reg_pcm_clk_cntl_2 = ((P_PCM_NCO_SHIFT & PCM_NCO_SHIFT)
                                | ((P_PCM_NCO_MUX_CNTL_MISC << 4) & PCM_NCO_MUX_CNTL));

     if ( gPllClockFailed ) {
       XDRV_LOG_ERROR(( XDRV_LOG_MOD_PCM, "Loaded MISC FCW to PCM NCO: 0x%08x", (unsigned int)PCM->reg_pcm_clk_cntl_0 ));
     }

   }

  return(0);
}



/*****************************************************************************
*
*  FUNCTION: pcm6838_regInit
*
*  PURPOSE:
*      Intialize 6838 PCM Module Registers
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static int pcm6838_regInit( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap )
{
   volatile BCM6838_APM *pApm = BCM6838_APM_PTR;

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

static int pcm6838_regDeinit( void )
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
*  FUNCTION: pcm6838_enable
*
*  PURPOSE:
*      Enable 6838 PCM Module
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int pcm6838_enable( int chan_mask )
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
*  FUNCTION: pcm6838_disable
*
*  PURPOSE:
*      Disable 6838 PCM Module
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int pcm6838_disable( void )
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
*  FUNCTION: pcm6838_iudmaInit
*
*  PURPOSE:
*      Intialize 6838 IUDMA registers.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES: IUDMA is shared with APM. PCM is channels 5 and 6.
*
*****************************************************************************/
static int pcm6838_iudmaInit( void )
{
   int chnum;
   volatile BCM6838_IUDMA *iuDmap = BCM6838_APM_IUDMA_PTR;

   /* Initialize the Rx/Tx IUDMA pair for PCM channel */
   for (chnum=0; chnum < PCM6838_NUM_CHANNELS; chnum++)
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
      iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].intMask = BCM6838_IUDMA_INTMASK_BDONE | BCM6838_IUDMA_INTMASK_NOTVLD;

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
      iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].intMask = BCM6838_IUDMA_INTMASK_BDONE | BCM6838_IUDMA_INTMASK_NOTVLD;

      /* Enable PCMDMA interrupts for RX only */
      iuDmap->regs.gbl_int_mask &= ~( 1 << gPcmDmaInfo[chnum].txChan.iuChanNum );  /* Global Int Mask */
   }

   /* Enable the IUDMA master */
   iuDmap->regs.ctrlConfig = IUDMA_REGS_CTRLCONFIG_MASTER_EN;

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: pcm6838_iudmaDeinit
*
*  PURPOSE:
*      De-intialize 6838 IUDMA registers.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES: IUDMA is shared with APM. PCM is channels 5 and 6.
*
*****************************************************************************/
static int pcm6838_iudmaDeinit( void)
{
   int chnum;
   volatile BCM6838_IUDMA *iuDmap = BCM6838_APM_IUDMA_PTR;
   int i=0;

   /* Halt individual channels first */
   for (chnum=0; chnum < PCM6838_NUM_CHANNELS; chnum++)
   {

      /* Deinit RX side */
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "RX DMA STATUS (1=busy 0=IDLE) : 0x%08x", (unsigned int)iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].config));
      XDRV_LOG_DBG((XDRV_LOG_MOD_PCM, "HALTING RX DMA...."));

      /* Set the burstHalt bit while clearing endma bit */
      iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].config = ( BCM6838_IUDMA_CONFIG_BURSTHALT );

      while( (iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].config & BCM6838_IUDMA_CONFIG_ENDMA) != 0x00000000 )
      {
         /* Set the burstHalt bit while clearing endma bit */
         iuDmap->ctrl[gPcmDmaInfo[chnum].rxChan.iuChanNum].config = ( BCM6838_IUDMA_CONFIG_BURSTHALT );
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
      iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].config = ( BCM6838_IUDMA_CONFIG_BURSTHALT );

      while( (iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].config & BCM6838_IUDMA_CONFIG_ENDMA) != 0x00000000 )
      {
         /* Set the burstHalt bit while clearing endma bit */
         iuDmap->ctrl[gPcmDmaInfo[chnum].txChan.iuChanNum].config = ( BCM6838_IUDMA_CONFIG_BURSTHALT );
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
*  FUNCTION: pcm6838_dmaDescrInit
*
*  PURPOSE:
*      Intialize 6838 DMA descriptors.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static int pcm6838_dmaDescrInit( void )
{
   XDRV_UINT8                       i,chnum;
   volatile BCM6838_IUDMA_DESC*     dmaTxDesc;
   volatile BCM6838_IUDMA_DESC*     dmaRxDesc;
   volatile BCM6838_IUDMA_DESC*     dmaTxDesc_c;
   volatile BCM6838_IUDMA_DESC*     dmaRxDesc_c;
   volatile PCM6838_DMA_BUFFER*     dmaRxData;
   volatile PCM6838_DMA_BUFFER*     dmaTxData;
   PCM6838_DMA_SIMPLEX_CHANNEL_INFO *dmaChanInfop;
   unsigned                         bufferSizeBytes = 0;
   XDRV_UINT32                      rxDescriptorArea;
   XDRV_UINT32                      txDescriptorArea;
   XDRV_UINT32                      rxBufferArea;
   XDRV_UINT32                      txBufferArea;
   volatile XDRV_UINT32             *dmaDatap;

   rxDescriptorArea = sizeof(BCM6838_IUDMA_DESC) * PCM6838_NUM_CHANNELS * DMA_RX_NUM_BUFFERS + sizeof(XDRV_UINT8) * BCM6838_DMA_UBUS_BYTES;
   txDescriptorArea = sizeof(BCM6838_IUDMA_DESC) * PCM6838_NUM_CHANNELS * DMA_TX_NUM_BUFFERS + sizeof(XDRV_UINT8) * BCM6838_DMA_UBUS_BYTES;

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
   for (i=0; i < PCM6838_NUM_CHANNELS; i++)
   {
      /* Align DMA descriptior to 64Bit Ubus */
      dmaRxDesc_c = (volatile BCM6838_IUDMA_DESC *) ALIGN_DATA( (unsigned int) dmaRxDesc_c, BCM6838_DMA_UBUS_BYTES );
      dmaTxDesc_c = (volatile BCM6838_IUDMA_DESC *) ALIGN_DATA( (unsigned int) dmaTxDesc_c, BCM6838_DMA_UBUS_BYTES );

      gPcmDmaInfo[i].rxChan.dmaDescp = (BCM6838_IUDMA_DESC*)(dmaRxDesc_c + i * DMA_RX_NUM_BUFFERS);
      gPcmDmaInfo[i].txChan.dmaDescp = (BCM6838_IUDMA_DESC*)(dmaTxDesc_c + i * DMA_TX_NUM_BUFFERS);
   }

   /* Clear the descriptors */
   memset( (void *) CacheToNonCacheVirtAddress( dmaRxDesc_c ), 0, rxDescriptorArea );
   memset( (void *) CacheToNonCacheVirtAddress( dmaTxDesc_c ), 0, txDescriptorArea );
   xdrvDCacheClearByAddress( XDRV_CACHE_INVALIDATE, (void*)dmaTxDesc_c, sizeof( dmaTxDesc_c ) );
   xdrvDCacheClearByAddress( XDRV_CACHE_INVALIDATE, (void*)dmaRxDesc_c, sizeof( dmaRxDesc_c ) );

   /* Allocate data buffers */
   rxBufferArea = sizeof(XDRV_UINT32) * (PCM6838_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM6838_NUM_CHANNELS * DMA_RX_NUM_BUFFERS;
   txBufferArea = sizeof(XDRV_UINT32) * (PCM6838_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM6838_NUM_CHANNELS * DMA_TX_NUM_BUFFERS;
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
   for (i=0; i < PCM6838_NUM_CHANNELS; i++)
   {
      gPcmDmaInfo[i].rxChan.dmaDatap = (PCM6838_DMA_BUFFER*)(dmaRxData + i * DMA_RX_NUM_BUFFERS);
      gPcmDmaInfo[i].txChan.dmaDatap = (PCM6838_DMA_BUFFER*)(dmaTxData + i * DMA_TX_NUM_BUFFERS);
   }

   /* Reset the data buffers */
   memset( (void*) dmaRxData, 0, rxBufferArea);
   memset( (void*) dmaTxData, 0, txBufferArea);

   for (chnum=0; chnum < PCM6838_NUM_CHANNELS; chnum++)
   {
      /* Get the pointer to TX DMA channel info */
      dmaChanInfop = &gPcmDmaInfo[chnum].txChan;

      /* Get the pointer to Tx descriptors for this channel. Convert to non-cached addresses */
      dmaTxDesc = (volatile BCM6838_IUDMA_DESC *)CacheToNonCacheVirtAddress((XDRV_UINT32)dmaChanInfop->dmaDescp);

      /* Init Tx DMA descriptor table */
      for( i = 0; i < DMA_TX_NUM_BUFFERS; i++ )
      {
         /* TX descriptor buffers should be enough to hold packetRate ms worth of sampleRate data */
         bufferSizeBytes = PCM6838_DMA_MAX_BUFFER_SIZE_BYTES;

         /* Each buffer is a complete packet */
         dmaTxDesc[i].flagsLengthStatus.status = BCM6838_IUDMA_DESC_STATUS_SOP |
                                                 BCM6838_IUDMA_DESC_STATUS_EOP;

         if( i == 0 )
         {
            /* First buffer - prime the TX fifo */
            dmaTxDesc[i].flagsLengthStatus.status |= BCM6838_IUDMA_DESC_STATUS_OWN;

            /* Prime with packetRate + 1ms for processing head room from the start of the ISR
            to allow us time to provide another packetRate worth of data to the TX APM DMA. */
            bufferSizeBytes = PCM6838_DMA_MAX_BUFFER_SIZE_BYTES;
         }

         if( i == ( DMA_TX_NUM_BUFFERS - 1 ) )
         {
            /* Last buffer - set the WRAP bit */
            dmaTxDesc[i].flagsLengthStatus.status |= BCM6838_IUDMA_DESC_STATUS_WRAP;
         }

         /* Check if we can accomodate the desired buffer size */
         if (bufferSizeBytes <= PCM6838_DMA_MAX_BUFFER_SIZE_BYTES)
         {
            dmaTxDesc[i].flagsLengthStatus.length = bufferSizeBytes;
         }
         else
         {
            /* Sorry, the allocated buffer size is smaller than what
            ** you want the buffer size configured to, so I can not do this.
            ** You can either increase the compile time buffer size (PCM6838_DMA_MAX_BUFFER_SIZE),
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
      dmaRxDesc = (volatile BCM6838_IUDMA_DESC *)CacheToNonCacheVirtAddress((XDRV_UINT32)dmaChanInfop->dmaDescp);

      /* Init Rx DMA descriptor table */
      for( i = 0; i < DMA_RX_NUM_BUFFERS; i++ )
      {
         /* RX descriptor buffers should be enough to hold packetRate ms worth of sampleRate data */
         bufferSizeBytes = PCM6838_DMA_MAX_BUFFER_SIZE_BYTES;

         /* Give ownership of all descriptors to RX DMA */
         dmaRxDesc[i].flagsLengthStatus.status = BCM6838_IUDMA_DESC_STATUS_OWN;

         if( i == ( DMA_RX_NUM_BUFFERS - 1 ) )
         {
            /* Last descriptor - set the wrap bit */
            dmaRxDesc[i].flagsLengthStatus.status |= BCM6838_IUDMA_DESC_STATUS_WRAP;
         }

         /* Check if we can accomodate the desired buffer size */
         if (bufferSizeBytes <= PCM6838_DMA_MAX_BUFFER_SIZE_BYTES)
         {
            /* Buffer size in bytes */
            dmaRxDesc[i].flagsLengthStatus.length = bufferSizeBytes;
         }
         else
         {
            /* Sorry, the allocated buffer size is smaller than what
            ** you want the buffer size configured to, so I can not do this.
            ** You can either increase the compile time buffer size (PCM6838_DMA_MAX_BUFFER_SIZE),
            ** or decrease the buffer size via packetRate/sampleRate */
            XDRV_LOG_ERROR((XDRV_LOG_MOD_PCM, "XDRV_PCM: ERROR - Can not accomodate configured buffer size (%d) ", bufferSizeBytes));
            return ( -1 );
         }

         /* Cache operations (invalidate and write-back) are performed on these
         ** buffers before and after DMA operations. Since cache operations
         ** operate on the entire cache-line, it is necessary to make the
         ** buffers aligned on the cache-line size (16-bytes for the 6838).
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
*  FUNCTION: pcm6838_dmaDescrDeinit
*
*  PURPOSE:
*      Deintialize 6838 DMA descriptors.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static int pcm6838_dmaDescrDeinit( void )
{
   XDRV_UINT8                       i;
   XDRV_UINT32                      rxDescriptorArea;
   XDRV_UINT32                      txDescriptorArea;
   XDRV_UINT32                      rxBufferArea;
   XDRV_UINT32                      txBufferArea;

   rxDescriptorArea = sizeof(BCM6838_IUDMA_DESC) * PCM6838_NUM_CHANNELS * DMA_RX_NUM_BUFFERS;
   txDescriptorArea = sizeof(BCM6838_IUDMA_DESC) * PCM6838_NUM_CHANNELS * DMA_TX_NUM_BUFFERS;

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

   for (i=0; i < PCM6838_NUM_CHANNELS; i++)
   {
      gPcmDmaInfo[i].rxChan.dmaDescp = NULL;
      gPcmDmaInfo[i].txChan.dmaDescp = NULL;
   }


   rxBufferArea = sizeof(XDRV_UINT32) * (PCM6838_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM6838_NUM_CHANNELS * DMA_RX_NUM_BUFFERS;
   txBufferArea = sizeof(XDRV_UINT32) * (PCM6838_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * PCM6838_NUM_CHANNELS * DMA_TX_NUM_BUFFERS;

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
   for (i=0; i < PCM6838_NUM_CHANNELS; i++)
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


