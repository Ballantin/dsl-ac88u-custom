/***************************************************************************
*    Copyright (c) 2009-2012 Broadcom Corporation
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
*    Filename: apm6838.c
*
****************************************************************************
*
*    Description:  APM (Audio Processor Module) device driver for BCM6838
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

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
#include <xdrvTimer.h>
#include <xchgAssert.h>
#include <xdrvApm.h>
#include <xdrvSlic.h>
#include <string.h>
#include <xdrvCache.h>
#include <mipsDefines.h>

#include <bcm6838.h>
#include "apm6838.h"
#include "dma6838.h"
#include <xchg_dma.h>
#include "hvg6838.h"
#include <apm6838Cfg.h>
#include <shared_utils.h>
#ifdef USE_KERNEL_LIB
#include <linux/math64.h>
#endif

#include <bcmChip.h>
#include <board.h>

/* ---- Public Variables ------------------------------------- */
int gPllClockFailed = 0;
/* ---- Private Constants and Types -------------------------- */

#define APM_POWER_WORKAROUND        0
#if APM_POWER_WORKAROUND
#include <BPCM.h>
#endif /* APM_POWER_WORKAROUND */

/* DMA descriptor or DMA data handle */
typedef dma_addr_t      DMA_HANDLE;
#define RING_SCALE_FACTOR           256
#define RING_PHASE_FACTOR           1
#define ASRC_DEC_SCALE_FACTOR       32 / 25
#define DEFAULT_ASRC_DEC_SCALE      0xD11
#define DEFAULT_ASRC_DEC_SHFT       3
#define DEFAULT_ASRC_INT_FIR_SHFT   3

/* Ring Frequency Ranges and Scaling Factors */
#define APM6838_RING_FREQ_MAX         75
#define APM6838_RING_FREQ_MIN         15
#define APM6838_RING_FREQ_SCALE       32768 / 3125   /* 2^20/100kHz */

/* DPLL lock settings */
#define DPLL_LOCK_WAIT                 1           /* Whether to wait for DPLL lock or not */
#define DPLL_LOCK_MAX_WAIT             20000000    /* Maximum iterations to wait for DPLL lock */
#define DPLL_RETRY_CNT                 2           /* Number of times to retry in case of PLL lock fails*/

/* Specific defs/includes for RDPA */
#if HW_NTR_SUPPORT
typedef XDRV_SINT8 s8;
#undef printf
#define printf printf
#include <linux/kconfig.h>
#include <rdpa_api.h>
#undef printf
#define printf printk
#endif /* HW_NTR_SUPPORT */
                                          
/* Program all channels */
#define APM6838_ALL_CHANNELS          0xFF

typedef struct
{
   int clkSource;                      /* APM clock source */
   int clkRev;                         /* APM clock rev */
   int sampleRate;                     /* Sampling frequency (in kHz) */
   int fcw_scale;                      /* Frequency Control Word Scale */
   int fcw_shift;                      /* Frequency Control Word Shift */
   int fcw_input;                      /* Frequency Control Word Input */
} FREQ_TABLE;

/* DMA Channel info (simplex - one way) for easy access to DMA channels */
typedef struct APM6838_DMA_SIMPLEX_CHANNEL_INFO
{
   int                  iuChanNum;     /* IUDMA channel number assosiated with this APM channel */
   BCM6838_IUDMA_DESC   *dmaDescp;     /* Where are the descriptors for this channel? */
   DMA_HANDLE           dmaDescHandle;
   APM6838_DMA_BUFFER   *dmaDatap;     /* Where are the DMA buffers to store/fetch the data to/from for each channel */
   DMA_HANDLE           dmaBufHandle;
} APM6838_DMA_SIMPLEX_CHANNEL_INFO;

/* DMA Channel info (duplex - both ways) for easy access to DMA channels */
typedef struct APM6838_DMA_DUPLEX_CHANNEL_INFO
{
   APM6838_DMA_SIMPLEX_CHANNEL_INFO rxChan;  /* RX channel */
   APM6838_DMA_SIMPLEX_CHANNEL_INFO txChan;  /* TX channel */
} APM6838_DMA_DUPLEX_CHANNEL_INFO;

/* Ring Gain Settings */
typedef struct APM6838_RING_GAIN_INFO
{
   XDRV_UINT32   slicType;
   XDRV_UINT32   slicResistance;
   XDRV_UINT32   protectionResistance;
   XDRV_UINT32   ringGain;

} APM6838_RING_GAIN_INFO;

/* asin table struct */
typedef struct APM6838_ASIN
{
   XDRV_SINT32   value;
   XDRV_SINT32   angle;
} APM6838_ASIN;

/* ---- Private Variables ------------------------------------ */


/* DMA buffers for APM Rx/Tx data */
static APM6838_DMA_BUFFER dmaRxData[APM6838_NUM_CHANNELS*DMA_RX_NUM_BUFFERS];
static APM6838_DMA_BUFFER dmaTxData[APM6838_NUM_CHANNELS*DMA_TX_NUM_BUFFERS];

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
extern struct dma_pool *pcm_dma_pool;
#endif

/* Initialized DMA channel info */
static APM6838_DMA_DUPLEX_CHANNEL_INFO gApmDmaInfo[APM6838_NUM_CHANNELS] =
{
   {
      /* RX Channel 0 */
      {
         BCM6838_APM_IUDMA_RX_A,
         NULL,    /* Will be filled with dynamically allocated value for dmaRxDesc_c */
         (DMA_HANDLE)&dmaRxData[0],

       },

      /* TX Channel 0 */
      {
         BCM6838_APM_IUDMA_TX_A,
         NULL,    /* Will be filled with dynamically allocated value for dmaTxDesc_c */
         (DMA_HANDLE)&dmaTxData[0],
      }
   },
#if (APM6838_NUM_CHANNELS>1)
   {
      /* RX Channel 1 */
      {
         BCM6838_APM_IUDMA_RX_B,
         NULL,    /* Will be filled with dynamically allocated value for dmaRxDesc_c */
         (DMA_HANDLE)&dmaRxData[DMA_RX_NUM_BUFFERS],
      },

      /* TX Channel 1 */
      {
         BCM6838_APM_IUDMA_TX_B,
         NULL,    /* Will be filled with dynamically allocated value for dmaTxDesc_c */
         (DMA_HANDLE)&dmaTxData[DMA_TX_NUM_BUFFERS],
      }
   }
#endif
};

/* Table for mapping sampling frequency (in Hz) to a register divider
** for DPLL sync configuration. All values were derived from formulas listed
** in chan_nco.doc  */
static const FREQ_TABLE freqTableDPLL[] =
{
#if HW_NTR_SUPPORT
   {  0, 0,  8,  0x10000000,    0,  0x0A7C5AC4 }, /* 08Khz fcw_audio of 0x0A7c5AC4 based on DPLL o/p fcw 0xa7c5ac47 - DPLL FCW */
   {  0, 0, 16,  0x20000000,    0,  0x14F8B589 }, /* 16KHz fcw_audio of 0x14F8B589 based on DPLL o/p fcw 0xa7c5ac47 - DPLL FCW */
#else
   {  1, 0,  8,  0x10000000,    0,  0x0A7C5AC4 }, /* 08Khz fcw_audio of 0x0A7c5AC4 based on DPLL o/p fcw 0xa7c5ac47 - Misc FCW */
   {  1, 0, 16,  0x20000000,    0,  0x14F8B589 }, /* 16KHz fcw_audio of 0x14F8B589 based on DPLL o/p fcw 0xa7c5ac47 - DPLL FCW */
#endif /* HW_NTR_SUPPORT */
   {  -1, -1,  -1, -1,            -1, -1         }
};

/* Table for mapping ring gains factors to slic types */
static const APM6838_RING_GAIN_INFO ringGainTable[] =
{
   { 0x3239,  58,  10,  114 },
   { 0x32392, 63,  10,  143 },
   { 0x9530,  18,  50,  150 },
   { 0x9540,  18,  50,  130 },
   { 0x0000,   0,  50,    0 }
};

/*Table of asin from -90 to +90, step=2*/
static const APM6838_ASIN apmAsinTable[]=
{
   { -1000 ,-1571}, { -999 , -1536 },{ -998 , -1501 },{ -995 , -1466 },{ -990 , -1431 },{ -985 , -1396 },{ -978 , -1361 },{ -970 , -1326 },{ -961 , -1292 },
   { -951 , -1257}, { -940 , -1222 },{ -927 , -1187 },{ -914 , -1152 },{ -899 , -1117 },{ -883 , -1082 },{ -866 , -1047 },{ -848 , -1012 },{ -829 , -977 },
   { -809 , -942 }, { -788 , -908 }, { -766 , -873 }, { -743 , -838 }, { -719 , -803 }, { -695 , -768 }, { -669 , -733 }, { -643 , -698 }, { -616 , -663 },
   { -588 , -628 }, { -559 , -593 }, { -530 , -559 }, { -500 , -524 }, { -469 , -489 }, { -438 , -454 }, { -407 , -419 }, { -375 , -384 }, { -342 , -349 },
   { -309 , -314 }, { -276 , -279 }, { -242 , -244 }, { -208 , -209 }, { -174 , -175 }, { -139 , -140 }, { -105 , -105 }, {  -70 ,  -70 }, {  -35 ,  -35 },
   {    0 ,    0 }, {   35 ,   35 }, {   70 ,   70 }, {  105 ,  105 }, {  139 ,  140 }, {  174 ,  175 }, {  208 ,  209 }, {  242 ,  244 }, {  276 ,  279 },
   {  309 ,  314 }, {  342 ,  349 }, {  375 ,  384 }, {  407 ,  419 }, {  438 ,  454 }, {  469 ,  489 }, {  500 ,  524 }, {  530 ,  559 }, {  559 ,  593 },
   {  588 ,  628 }, {  616 ,  663 }, {  643 ,  698 }, {  669 ,  733 }, {  695 ,  768 }, {  719 ,  803 }, {  743 ,  838 }, {  766 ,  873 }, {  788 ,  908 },
   {  809 ,  942 }, {  829 ,  977 }, {  848 , 1012 }, {  866 , 1047 }, {  883 , 1082 }, {  899 , 1117 }, {  914 , 1152 }, {  927 , 1187 }, {  940 , 1222 },
   {  951 , 1257 }, {  961 , 1292 }, {  970 , 1326 }, {  978 , 1361 }, {  985 , 1396 }, {  990 , 1431 }, {  995 , 1466 }, {  998 , 1501 }, {  999 , 1536 },
   { 1000 , 1571 }
};
/* ---- Private Function Prototypes --------------------------------------- */
static int apm6838_dmaInit( volatile BCM6838_APM *apmp, volatile BCM6838_IUDMA *iuDmap, int sampleRate, int packetRate);
static int apm6838_dmaDeinit( volatile BCM6838_APM *apmp, volatile BCM6838_IUDMA *iuDmap);
static int apm6838_regInit(volatile BCM6838_APM *pApm, const APM6838_CFG *pCfg );
static int apm6838_iudmaChanDeinit( int iuChanNum, volatile BCM6838_IUDMA *iuDmap );
static int apm6838_iudmaDeinit( volatile BCM6838_IUDMA *iuDmap );
static int apm_pllStart(volatile BCM6838_APM *pApm);
static int initIudma( volatile BCM6838_IUDMA *iuDmap);
static int deinitIudma( volatile BCM6838_IUDMA *iuDmap );
static int initDmaDescriptors( int sampleRate, int packetRate) ;
static int deinitDmaDescriptors( void );
#if 0
static int FlexiCalcRegWrite( volatile XDRV_UINT32 *regp, unsigned int value, unsigned int mask, unsigned int shift );
#endif
static int WriteApmFilterCoeffs( volatile BCM6838_APM *pApm, APM_6838_DRV *pDrv, int chan, APM6838_FILTER_TYPE filter, int *data, int length );
static XDRV_SINT64 divRound( XDRV_SINT64 dividend, XDRV_SINT64 divisor );

/* APM "meta-driver" interface functions */
static void apm6838SetRingParms( XDRV_APM *pDrv, int ringFrequency, int ringWaveshape, int ringVoltage, int ringOffset, int ringOffsetCal );
static void apm6838GetRingParms( XDRV_APM *pDrv, int *ringFrequency, int *ringWaveshape, int *ringVoltage, int *ringOffset, int *ringOffsetCal );
static XDRV_SINT32 apm6838DebugStatusDump( XDRV_APM *pDrv );
static XDRV_SINT16 apm6838GetDlp( XDRV_APM *pDrv);
static XDRV_SINT16 apm6838GetElp( XDRV_APM *pDrv);
static void apm6838PulseSignal( XDRV_APM *pDrv, int endpt, int duration, int period,
                                int repetition, int amplitude, int frequency );
static void apm6838SetHPF( XDRV_APM *pDrv, int endpt, XDRV_BOOL enable );
static int apm6838SetTxRxGains( XDRV_APM *pDrv, int chan, int txGain, int rxGain, int *apmTxGain, int *apmRxGain );
static XDRV_SINT64 apmAsin(XDRV_SINT32 value);

#if APM_POWER_WORKAROUND
/* Redefine the PMC API to the internal definition */
#define PowerOnDevice    ApmPowerOnDevice
#define PowerOffDevice   ApmPowerOffDevice
#define PowerOnZone      ApmPowerOnZone
#define PowerOffZone     ApmPowerOffZone
#define ResetDevice      ApmResetDevice
#define ResetZone        ApmResetZone
static int ApmPowerOnDevice(int devAddr);
static int ApmPowerOffDevice(int devAddr, int repower);
static int ApmPowerOnZone(int devAddr, int zone);
static int ApmPowerOffZone(int devAddr, int zone);
static int ApmResetDevice(int devAddr);
static int ApmResetZone(int devAddr, int zone);
#endif /* APM_POWER_WORKAROUND */

/* APM interface */
static const XDRV_APM_FUNCS bcm6838ApmDrvFuncs =
{
   apm6838SetRingParms,
   apm6838GetRingParms,
   apm6838GetDlp,
   apm6838DebugStatusDump,
   apm6838GetElp,
   hvg6838GetOverCurrentStatus,
   hvg6838GetOverVoltageStatus,
   apm6838PulseSignal,
   hvg6838SetVoltage,
   apm6838SetHPF,
   hvg6838UpdateSlicStatus,
   hvg6838UpdateHookStatus,
   hvg6838UpdateHookStatusEnable,
   apm6838SetTxRxGains
};

/* ---- Public Functions -------------------------------------------------- */
/* ---- Private Functions ------------------------------------------------- */
#define APM6838_USLEEP(cycles) ( {volatile int i; for (i=0; i < (cycles); i++) {}} )

/*****************************************************************************
*
*  FUNCTION: apm6838_init
*
*  PURPOSE:
*      Intialize 6838 APM Driver interface
*
*  PARAMETERS:
*      pDrv      - driver interface
*      pIntCtl   - pointer to 6838's Interrupt Control module
*      cfgp      - APM configuration structure (PLL/DMA/Flexicalc)
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int apm6838_init(
   APM_6838_DRV    *pDrv,
   volatile PerfControl *pIntCtl,
   const APM6838_CFG *cfgp
)
{
   int result;

   /* Set the "meta-driver" interface */
   pDrv->pDrvFuncs = &bcm6838ApmDrvFuncs;

   /* Initialize the driver state variables */
   pDrv->pApm = BCM6838_APM_PTR;
   pDrv->pIuDma = BCM6838_APM_IUDMA_PTR;
   pDrv->pIntCtl = pIntCtl;
   pDrv->Cfg = *cfgp;

   /* Now start the APM module */
   result = apm6838_start(pDrv);

   return ( result );
}

/*****************************************************************************
*
*  FUNCTION: apm6838_deinit
*
*  PURPOSE:
*      DeIntialize 6838 APM Driver interface
*
*  PARAMETERS:
*      pDrv       - 6838 driver interface
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int apm6838_deinit( APM_6838_DRV *pDrv )
{
   unsigned int * miscPmcUbusActivePortControlReg = (unsigned int*)0xb3e80040;

   /* First stop the APM */
   apm6838_stop(pDrv);

   /* Clear the driver state */
   memset(pDrv,0, sizeof(APM_6838_DRV));

   /* de-register APM register space/ubus port */
   *miscPmcUbusActivePortControlReg &= ~(1<<13);

   /* Power off zones */
   PowerOffZone(PMB_ADDR_APM, APM_Zone_Audio);
   PowerOffZone(PMB_ADDR_APM, APM_Zone_Main );

   /* Set for HVG_DRIVER_A/B pins GPIO and drive a logic 0 */
   set_pinmux( 49, PINMUX_GPIO ); /* GPIO49 - HVG_DRIVER_A */
   set_pinmux( 50, PINMUX_GPIO ); /* GPIO50 - HVG_DRIVER_B */
   kerSysSetGpio( 49, kGpioInactive );
   kerSysSetGpio( 50, kGpioInactive );

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: apm6838_start
*
*  PURPOSE:
*      Initialize and start the actual 6838 Audio Processing Module
*
*  PARAMETERS:
*      pDrv       - 6838 driver interface
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
int apm6838_start( APM_6838_DRV  *pDrv )
{
   volatile int i;
   int clkrst_zone0 = 0;
   BPCM_SR_CONTROL sr_control;
   unsigned int * miscPmcUbusActivePortControlReg = (unsigned int*)0xb3e80040;

   /* Enable 200Mhz Clock via BPCM. All FCW calculations are based on this 200Mhz Clock */
   WriteZoneRegister(PMB_ADDR_CHIP_CLKRST, clkrst_zone0, BPCMZoneRegOffset(config2), 0);

   APM6838_USLEEP(1000);

   /* RESET APM via BPCM ( Block Power Control Module ) */
   ResetDevice( PMB_ADDR_APM);
   ResetZone(PMB_ADDR_APM, APM_Zone_Main );
   ResetZone(PMB_ADDR_APM, APM_Zone_Audio);

   /* Register APM address space/Ubus port */
   *miscPmcUbusActivePortControlReg |= (1<<13);

   /* Assert APM SoftResets via BPCM */
   ReadBPCMRegister(PMB_ADDR_APM, BPCMRegOffset(sr_control), &sr_control.Reg32);
   sr_control.Bits.sr |= ( BPCM_APM_SRESET_AUDIO_N | BPCM_APM_SRESET_200_N  |
                    BPCM_APM_SRESET_HVGA_N  | BPCM_APM_SRESET_HVGB_N |
                    BPCM_APM_SRESET_BMU_N  ) ;
   WriteBPCMRegister(PMB_ADDR_APM, BPCMRegOffset(sr_control), sr_control.Reg32);

   /* Sleep to ensure full soft reset */
   APM6838_USLEEP(1000);

   /* De-Assert APM SoftResets via BPCM */
   ReadBPCMRegister(PMB_ADDR_APM, BPCMRegOffset(sr_control), &sr_control.Reg32);
   sr_control.Bits.sr &= ~( BPCM_APM_SRESET_AUDIO_N | BPCM_APM_SRESET_200_N  |
                     BPCM_APM_SRESET_HVGA_N  | BPCM_APM_SRESET_HVGB_N |
                     BPCM_APM_SRESET_BMU_N  ) ;
   WriteBPCMRegister(PMB_ADDR_APM, BPCMRegOffset(sr_control), sr_control.Reg32);

   /* Sleep to ensure full soft reset */
   APM6838_USLEEP(1000);

   /* Add PinMux for HVG_DRIVER_A/B pins (TODO:Remove Magic Numbers) */
   set_pinmux( 49, PINMUX_APM ); /* GPIO49 - HVG_DRIVER_A */
   set_pinmux( 50, PINMUX_APM ); /* GPIO50 - HVG_DRIVER_B */

   APM6838_USLEEP(1000);

   /*
   ** Configure the PLL.
   */
   if ( apm_pllStart( pDrv->pApm ) == -1 ) {
     return ( -1 );
   }

   /*
   ** Configure the APM registers.
   */
   if ( apm6838_regInit( pDrv->pApm, &pDrv->Cfg ))
   {
      return ( -1 );
   }

   /*
   ** Load the FlexiCalc settings. Note that this overwrites the default settings.
   */
   if ( apm6838_flexicalcConfig( pDrv->pApm, pDrv->Cfg.pFlexiCalc, pDrv ))
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - FlexiCalc config failed !!!"));
      return ( -2 );
   }

   /*
   ** Launch the APM DMAs
   */
   if ( apm6838_dmaInit( pDrv->pApm, pDrv->pIuDma, pDrv->Cfg.sampleRate, pDrv->Cfg.packetRate ))
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - DMA setup failed !!!"));
      return ( -2 );
   }

   /* Save initial vDac settings */
   for( i = 0; i < APM6838_NUM_CHANNELS; i++ )
   {
      pDrv->vDac[i] = pDrv->Cfg.pFlexiCalc->vrx_pg;
   }

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: apm6838_stop
*
*  PURPOSE:
*      Stop and deinitialize the 6838 Audio Processing Module
*
*  PARAMETERS:
*      pDrv       - 6838 driver interface
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
int apm6838_stop( APM_6838_DRV *pDrv )
{
   /*
   ** Stop the APM DMA
   */
   if (apm6838_dmaDeinit( pDrv->pApm, pDrv->pIuDma))
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - DMA deinit failed !!!"));
      return ( -1 );
   }

   if (deinitDmaDescriptors())
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - DMA descriptor deinit failed !!!"));
      return ( -1 );
   }

   /* Place the channels under soft reset */
   pDrv->pApm->reg_cha_eq_config |= SOFT_INIT;
   pDrv->pApm->reg_chb_eq_config |= SOFT_INIT;

   return ( 0 );
}


/*****************************************************************************
*
*  FUNCTION: apm6838_dmaInit
*
*  PURPOSE:
*
*  PARAMETERS:
*      apmp       - pointer to 6838's APM module
*      iuDmap     - pointer to 6838's APM IUDMA module
*      sampleRate - sampling rate in kHz (8 or 16k)
*      packetRate - the rate (in ms) at which APM generates packets/interrupts
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
static int apm6838_dmaInit
(
   volatile BCM6838_APM    *apmp,
   volatile BCM6838_IUDMA  *iuDmap,
   int                     sampleRate,
   int                     packetRate
)
{
   /* Initialize the DMA descriptor table */
   if (initDmaDescriptors(sampleRate, packetRate))
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - initDmaDescriptors failed !!!"));
      return ( -1 );
   }

   /* Initialize the IUDMA */
   if (initIudma(iuDmap))
   {
      return ( -2 );
   }



   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   initDmaDescriptors
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
static int initDmaDescriptors( int sampleRate, int packetRate )
{
   XDRV_UINT8                       i,chnum;
   volatile BCM6838_IUDMA_DESC*     dmaTxDesc;
   volatile BCM6838_IUDMA_DESC*     dmaRxDesc;
   volatile BCM6838_IUDMA_DESC*     dmaTxDesc_c;
   volatile BCM6838_IUDMA_DESC*     dmaRxDesc_c;
   volatile APM6838_DMA_BUFFER*     dmaRxData;
   volatile APM6838_DMA_BUFFER*     dmaTxData;
   APM6838_DMA_SIMPLEX_CHANNEL_INFO *dmaChanInfop;
   unsigned                          bufferSizeBytes = 0;
   XDRV_UINT32                      rxDescriptorArea;
   XDRV_UINT32                      txDescriptorArea;
   XDRV_UINT32                      rxBufferArea;
   XDRV_UINT32                      txBufferArea;
   volatile XDRV_UINT32 *dmaDatap;

   rxDescriptorArea = sizeof(BCM6838_IUDMA_DESC) * APM6838_NUM_CHANNELS * DMA_RX_NUM_BUFFERS + sizeof(XDRV_UINT8) * BCM6838_DMA_UBUS_BYTES;
   txDescriptorArea = sizeof(BCM6838_IUDMA_DESC) * APM6838_NUM_CHANNELS * DMA_TX_NUM_BUFFERS + sizeof(XDRV_UINT8) * BCM6838_DMA_UBUS_BYTES;

   /* Allocate receive and transmit descriptors */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
   dmaRxDesc_c = dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, &gApmDmaInfo[0].rxChan.dmaDescHandle );
   dmaTxDesc_c = dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, &gApmDmaInfo[0].txChan.dmaDescHandle );
#else
   dmaRxDesc_c = (volatile BCM6838_IUDMA_DESC *)dma_alloc_coherent( NULL, rxDescriptorArea, &gApmDmaInfo[0].rxChan.dmaDescHandle, GFP_KERNEL );
   dmaTxDesc_c = (volatile BCM6838_IUDMA_DESC *)dma_alloc_coherent( NULL, txDescriptorArea, &gApmDmaInfo[0].txChan.dmaDescHandle, GFP_KERNEL );
#endif
   if ( (dmaRxDesc_c == NULL) || (dmaTxDesc_c == NULL) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "Could not allocate memory for DMA descriptors !!! "));
      return( -1 );
   }

   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "rxDescriptorArea       = %d ", (int)rxDescriptorArea));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "txDescriptorArea       = %d ", (int)txDescriptorArea));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "Unaligned dmaRxDesc_c  = 0x%08X ", (unsigned int)dmaRxDesc_c));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "Unaligned dmaTxDesc_c  = 0x%08X ", (unsigned int)dmaTxDesc_c));

   /* Fill in the global APM DMA structure with the allocated pointers to descriptors */
   for (i=0; i < APM6838_NUM_CHANNELS; i++)
   {
      /* Align DMA descriptior to 64Bit Ubus */
      dmaRxDesc_c = (volatile BCM6838_IUDMA_DESC *) ALIGN_DATA( (unsigned int) dmaRxDesc_c, BCM6838_DMA_UBUS_BYTES );
      dmaTxDesc_c = (volatile BCM6838_IUDMA_DESC *) ALIGN_DATA( (unsigned int) dmaTxDesc_c, BCM6838_DMA_UBUS_BYTES );

      gApmDmaInfo[i].rxChan.dmaDescp = (BCM6838_IUDMA_DESC*)(dmaRxDesc_c + i * DMA_RX_NUM_BUFFERS);
      gApmDmaInfo[i].txChan.dmaDescp = (BCM6838_IUDMA_DESC*)(dmaTxDesc_c + i * DMA_TX_NUM_BUFFERS);
   }

   /* Clear the descriptors */
   memset( (void *) CacheToNonCacheVirtAddress( dmaTxDesc_c ), 0, txDescriptorArea );
   memset( (void *) CacheToNonCacheVirtAddress( dmaRxDesc_c ), 0, rxDescriptorArea );

   xdrvDCacheClearByAddress( XDRV_CACHE_INVALIDATE, (void*)dmaTxDesc_c, sizeof( dmaTxDesc_c ) );
   xdrvDCacheClearByAddress( XDRV_CACHE_INVALIDATE, (void*)dmaRxDesc_c, sizeof( dmaRxDesc_c ) );

   /* Allocate data buffers */
   rxBufferArea = sizeof(XDRV_UINT32) * (APM6838_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * APM6838_NUM_CHANNELS * DMA_RX_NUM_BUFFERS;
   txBufferArea = sizeof(XDRV_UINT32) * (APM6838_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * APM6838_NUM_CHANNELS * DMA_TX_NUM_BUFFERS;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
   dmaRxData = dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, &gApmDmaInfo[0].rxChan.dmaBufHandle );
   dmaTxData = dma_pool_alloc( pcm_dma_pool, GFP_KERNEL, &gApmDmaInfo[0].txChan.dmaBufHandle );
#else
   dmaRxData = dma_alloc_coherent( NULL, rxBufferArea, &gApmDmaInfo[0].rxChan.dmaBufHandle, GFP_KERNEL );
   dmaTxData = dma_alloc_coherent( NULL, txBufferArea, &gApmDmaInfo[0].txChan.dmaBufHandle, GFP_KERNEL );
#endif

   if ( (dmaRxData == NULL) || (dmaTxData == NULL ) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "Could not allocate memory for DMA data buffers !!! "));
      return( -1 );
     }

   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "rxBufferArea          = %d ", (int)rxBufferArea));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "txBufferArea          = %d ", (int)txBufferArea));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "Unaligned dmaRxData   = 0x%08X ", (unsigned int)dmaRxData));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "Unaligned dmaTxData   = 0x%08X ", (unsigned int)dmaTxData));

   /* Fill in the global PCM DMA structure with the allocated pointers to descriptors */
   for (i=0; i < APM6838_NUM_CHANNELS; i++)
     {
      gApmDmaInfo[i].rxChan.dmaDatap = (APM6838_DMA_BUFFER*)(dmaRxData + i * DMA_RX_NUM_BUFFERS);
      gApmDmaInfo[i].txChan.dmaDatap = (APM6838_DMA_BUFFER*)(dmaTxData + i * DMA_TX_NUM_BUFFERS);
   }

   /* Reset the data buffers */
   memset( (void*) dmaRxData, 0, rxBufferArea);
   memset( (void*) dmaTxData, 0, txBufferArea);

   for (chnum=0; chnum<APM6838_NUM_CHANNELS; chnum++)
   {
      /* Get the pointer to TX DMA channel info */
      dmaChanInfop=&gApmDmaInfo[chnum].txChan;

      /* Get the pointer to Tx descriptors for this channel. Convert to non-cached addresses */
      dmaTxDesc = (volatile BCM6838_IUDMA_DESC *)CacheToNonCacheVirtAddress((XDRV_UINT32)dmaChanInfop->dmaDescp);

      /* Init Tx DMA descriptor table */
      for( i = 0; i < DMA_TX_NUM_BUFFERS; i++ )
      {
            /* TX descriptor buffers should be enough to hold packetRate ms worth of sampleRate data */
         bufferSizeBytes = (packetRate*sampleRate)<<1;

         /* Each buffer is a complete packet */
         dmaTxDesc[i].flagsLengthStatus.status = BCM6838_IUDMA_DESC_STATUS_SOP |
                                                 BCM6838_IUDMA_DESC_STATUS_EOP;

         if( i == 0 )
         {
            /* First buffer - prime the TX fifo */
            dmaTxDesc[i].flagsLengthStatus.status |= BCM6838_IUDMA_DESC_STATUS_OWN;

            /* Prime with packetRate 5ms + 1ms + TX buffer depth for processing head room from the start of the ISR
            to allow us time to provide another packetRate worth of data to the TX APM DMA. */
            bufferSizeBytes = (((packetRate+1)*sampleRate)<<1) + APM6838_TX_DMA_BUF_DEPTH_BYTES;
         }

         if( i == ( DMA_TX_NUM_BUFFERS - 1 ) )
         {
            /* Last buffer - set the WRAP bit */
            dmaTxDesc[i].flagsLengthStatus.status |= BCM6838_IUDMA_DESC_STATUS_WRAP;
         }

         /* Check if we can accomodate the desired buffer size */
         if (bufferSizeBytes <= APM6838_DMA_MAX_BUFFER_SIZE_BYTES)
         {
            dmaTxDesc[i].flagsLengthStatus.length = bufferSizeBytes;
            XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_APM: Chan %d TX Buff %d Size (%d) ",chnum, i, bufferSizeBytes));
         }
         else
         {
            /* Sorry, the allocated buffer size is smaller than what
            ** you want the buffer size configured to, so I can not do this.
            ** You can either increase the compile time buffer size (APM6838_DMA_MAX_BUFFER_SIZE),
            ** or decrease the buffer size via packetRate/sampleRate */
            XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - Can not accomodate configured buffer size (%d) ", bufferSizeBytes));
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

         /*  Use physical address */
         dmaTxDesc[i].addr.addr32 = CacheVirtToPhyAddress( (XDRV_UINT32)dmaDatap );

         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Tx Desc: chan %d, buf %d, sts 0x%x, len %d, bufp 0x%x ",
                         chnum, i, dmaTxDesc[i].flagsLengthStatus.status,  dmaTxDesc[i].flagsLengthStatus.length,  (unsigned int)dmaTxDesc[i].addr.addr32 ));
      }

      /* Get the pointer to RX DMA channel info */
      dmaChanInfop=&gApmDmaInfo[chnum].rxChan;

      /* Get the pointer to Rx descriptors for this channel. Convert to non-cached addresses */
      dmaRxDesc = (volatile BCM6838_IUDMA_DESC *)CacheToNonCacheVirtAddress((XDRV_UINT32)dmaChanInfop->dmaDescp);

      /* Init Rx DMA descriptor table */
      for( i = 0; i < DMA_RX_NUM_BUFFERS; i++ )
      {
         /* RX descriptor buffers should be enough to hold packetRate ms worth of sampleRate data */
         bufferSizeBytes = (packetRate*sampleRate)<<1;

         /* Give ownership of all descriptors to RX DMA */
         dmaRxDesc[i].flagsLengthStatus.status = BCM6838_IUDMA_DESC_STATUS_OWN;

         if( i == ( DMA_RX_NUM_BUFFERS - 1 ) )
         {
            /* Last descriptor - set the wrap bit */
            dmaRxDesc[i].flagsLengthStatus.status |= BCM6838_IUDMA_DESC_STATUS_WRAP;
         }

         /* Check if we can accomodate the desired buffer size */
         if (bufferSizeBytes <= APM6838_DMA_MAX_BUFFER_SIZE_BYTES)
         {
            /* Buffer size in bytes */
            dmaRxDesc[i].flagsLengthStatus.length = bufferSizeBytes;
            XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_APM: Chan %d RX Buff %d Size (%d) ", chnum, i, bufferSizeBytes));
         }
         else
         {
            /* Sorry, the allocated buffer size is smaller than what
            ** you want the buffer size configured to, so I can not do this.
            ** You can either increase the compile time buffer size (APM6838_DMA_MAX_BUFFER_SIZE),
            ** or decrease the buffer size via packetRate/sampleRate */
            XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - Can not accomodate configured buffer size (%d) ", bufferSizeBytes));
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

         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Rx Desc: chan %d, buf %d, sts 0x%x, len %d, bufp 0x%x ",
                     chnum, i, dmaRxDesc[i].flagsLengthStatus.status, dmaRxDesc[i].flagsLengthStatus.length, (unsigned int)dmaRxDesc[i].addr.addr32 ));
      }
   }

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: deinitDmaDescriptors
*
*  PURPOSE:
*      Deintialize the DMA descriptor rings.
*
*  PARAMETERS:
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
static int deinitDmaDescriptors( void )
{
   XDRV_UINT8                       i;
   XDRV_UINT32                      rxDescriptorArea;
   XDRV_UINT32                      txDescriptorArea;
   XDRV_UINT32                      rxBufferArea;
   XDRV_UINT32                      txBufferArea;

   rxDescriptorArea = sizeof(BCM6838_IUDMA_DESC) * APM6838_NUM_CHANNELS * DMA_RX_NUM_BUFFERS + sizeof(XDRV_UINT8) * BCM6838_DMA_UBUS_BYTES;
   txDescriptorArea = sizeof(BCM6838_IUDMA_DESC) * APM6838_NUM_CHANNELS * DMA_TX_NUM_BUFFERS + sizeof(XDRV_UINT8) * BCM6838_DMA_UBUS_BYTES;

   /* Clear the descriptors */
   memset( (void *) CacheToNonCacheVirtAddress( gApmDmaInfo[0].rxChan.dmaDescp ), 0, rxDescriptorArea);
   memset( (void *) CacheToNonCacheVirtAddress( gApmDmaInfo[0].txChan.dmaDescp ), 0, txDescriptorArea);

   /* Free receive and transmit descriptors */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
   dma_pool_free( pcm_dma_pool, gApmDmaInfo[0].rxChan.dmaDescp, gApmDmaInfo[0].rxChan.dmaDescHandle );
   dma_pool_free( pcm_dma_pool, gApmDmaInfo[0].txChan.dmaDescp, gApmDmaInfo[0].txChan.dmaDescHandle );
#else
   dma_free_coherent(NULL, rxDescriptorArea, gApmDmaInfo[0].rxChan.dmaDescp, gApmDmaInfo[0].rxChan.dmaDescHandle);
   dma_free_coherent(NULL, txDescriptorArea, gApmDmaInfo[0].txChan.dmaDescp, gApmDmaInfo[0].txChan.dmaDescHandle);
#endif

   for (i=0; i < APM6838_NUM_CHANNELS; i++)
   {
      gApmDmaInfo[i].rxChan.dmaDescp = NULL;
      gApmDmaInfo[i].txChan.dmaDescp = NULL;
   }

   rxBufferArea = sizeof(XDRV_UINT32) * (APM6838_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * APM6838_NUM_CHANNELS * DMA_RX_NUM_BUFFERS;
   txBufferArea = sizeof(XDRV_UINT32) * (APM6838_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32) * APM6838_NUM_CHANNELS * DMA_TX_NUM_BUFFERS;

   /* Clear the data buffers */
   memset( (void*) gApmDmaInfo[0].rxChan.dmaDatap, 0, rxBufferArea);
   memset( (void*) gApmDmaInfo[0].txChan.dmaDatap, 0, txBufferArea);

   /* Free data buffers */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
   dma_pool_free( pcm_dma_pool, gApmDmaInfo[0].rxChan.dmaDatap, gApmDmaInfo[0].rxChan.dmaBufHandle );
   dma_pool_free( pcm_dma_pool, gApmDmaInfo[0].txChan.dmaDatap, gApmDmaInfo[0].txChan.dmaBufHandle );
#else
   dma_free_coherent(NULL, rxBufferArea, gApmDmaInfo[0].rxChan.dmaDatap, gApmDmaInfo[0].rxChan.dmaBufHandle);
   dma_free_coherent(NULL, txBufferArea, gApmDmaInfo[0].txChan.dmaDatap, gApmDmaInfo[0].txChan.dmaBufHandle);
#endif

   /* Fill in the global APM DMA structure with the allocated pointers to descriptors */
   for (i=0; i < APM6838_NUM_CHANNELS; i++)
   {
      gApmDmaInfo[i].rxChan.dmaDatap = NULL;
      gApmDmaInfo[i].txChan.dmaDatap = NULL;
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   initIudma
**
** PURPOSE:    Initialize the APM IUDMA
**
** PARAMETERS: iuDmap       - pointer to IUDMA module
**             dmaSettingsp - pointer to DMA configuration structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static int initIudma( volatile BCM6838_IUDMA *iuDmap)
{
   int chnum;
   volatile BCM6838_IUDMA_STRAM *stRamp = iuDmap->stram;

   /* Reset entire IUDMA state RAM */
   memset( (void*)stRamp, 0, BCM6838_MAX_APM_PCM_DMA_CHANNELS * sizeof(BCM6838_IUDMA_STRAM) );

   /* Initialize the Rx/Tx IUDMA pair for each APM channel */
   for (chnum=0; chnum<APM6838_NUM_CHANNELS; chnum++)
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "initIudma: chan %d, descBase 0x%x, descBaseIudma 0x%x",
                        gApmDmaInfo[chnum].rxChan.iuChanNum,
                        (unsigned int)gApmDmaInfo[chnum].rxChan.dmaDescp,
                        CacheVirtToPhyAddress(gApmDmaInfo[chnum].rxChan.dmaDescp)
                        ));

      /* Explicitly Reset DMA channel */
      iuDmap->regs.channelReset |= ( (1 << gApmDmaInfo[chnum].rxChan.iuChanNum) | (1 << gApmDmaInfo[chnum].txChan.iuChanNum) );

      /* Init RX side */
      iuDmap->ctrl[gApmDmaInfo[chnum].rxChan.iuChanNum].maxBurst = 5; /* in 64-bit words */
      iuDmap->stram[gApmDmaInfo[chnum].rxChan.iuChanNum].baseDescPointer = CacheVirtToPhyAddress(gApmDmaInfo[chnum].rxChan.dmaDescp);
      iuDmap->stram[gApmDmaInfo[chnum].rxChan.iuChanNum].stateBytesDoneRingOffset = 0;
      iuDmap->stram[gApmDmaInfo[chnum].rxChan.iuChanNum].flagsLengthStatus = 0;
      iuDmap->stram[gApmDmaInfo[chnum].rxChan.iuChanNum].currentBufferPointer = 0;
      iuDmap->ctrl[gApmDmaInfo[chnum].rxChan.iuChanNum].intStat |= iuDmap->ctrl[gApmDmaInfo[chnum].rxChan.iuChanNum].intStat;
      iuDmap->ctrl[gApmDmaInfo[chnum].rxChan.iuChanNum].intMask = BCM6838_IUDMA_INTMASK_BDONE | BCM6838_IUDMA_INTMASK_NOTVLD;

      /* Enable APMDMA interrupts for APM chan B RX only */
      if( chnum == APM6838_NUM_CHANNELS-1 )
      {
         /* Enable APMDMA interrupt */
         iuDmap->regs.gbl_int_mask |= ( 1 << gApmDmaInfo[chnum].rxChan.iuChanNum );
      }
      else
      {
         /* Disable APMDMA interrupt */
         iuDmap->regs.gbl_int_mask &= ~( 1 << gApmDmaInfo[chnum].rxChan.iuChanNum );
      }

      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "initIudma: chan %d, descBase 0x%x, descBaseIudma 0x%x",
                        gApmDmaInfo[chnum].txChan.iuChanNum,
                        (unsigned int)gApmDmaInfo[chnum].txChan.dmaDescp,
                        CacheVirtToPhyAddress(gApmDmaInfo[chnum].txChan.dmaDescp)
                        ));
      /* Init TX side */
      iuDmap->ctrl[gApmDmaInfo[chnum].txChan.iuChanNum].maxBurst = 5; /* in 64-bit words */
      iuDmap->stram[gApmDmaInfo[chnum].txChan.iuChanNum].baseDescPointer = CacheVirtToPhyAddress(gApmDmaInfo[chnum].txChan.dmaDescp);
      iuDmap->stram[gApmDmaInfo[chnum].txChan.iuChanNum].stateBytesDoneRingOffset = 0;
      iuDmap->stram[gApmDmaInfo[chnum].txChan.iuChanNum].flagsLengthStatus = 0;
      iuDmap->stram[gApmDmaInfo[chnum].txChan.iuChanNum].currentBufferPointer = 0;
      iuDmap->ctrl[gApmDmaInfo[chnum].txChan.iuChanNum].intStat |= iuDmap->ctrl[gApmDmaInfo[chnum].txChan.iuChanNum].intStat;
      iuDmap->ctrl[gApmDmaInfo[chnum].txChan.iuChanNum].intMask = BCM6838_IUDMA_INTMASK_BDONE | BCM6838_IUDMA_INTMASK_NOTVLD;

      /* Enable APMDMA interrupts on the RX channel only */
      iuDmap->regs.gbl_int_mask &= ~( 1 << gApmDmaInfo[chnum].txChan.iuChanNum );

   }

   /* Enable the IUDMA master */
   iuDmap->regs.ctrlConfig = IUDMA_REGS_CTRLCONFIG_MASTER_EN;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   deinitIudma
**
** PURPOSE:    De-initialize the APM IUDMA
**
** PARAMETERS: iuDmap       - pointer to IUDMA module
**
** RETURNS:    none
**
*****************************************************************************
*/
static int deinitIudma( volatile BCM6838_IUDMA *iuDmap )
{
   /* Disable the IUDMA master */
   iuDmap->regs.ctrlConfig &= ~IUDMA_REGS_CTRLCONFIG_MASTER_EN;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   apm6838_iudmaChanDeinit
**
** PURPOSE:    Deinitialize the IUDMA channel
**
** PARAMETERS: None
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
static int apm6838_iudmaChanDeinit( int iuChanNum, volatile BCM6838_IUDMA *iuDmap )
{
   int i = 0;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DMA chan %d STATUS (1=busy 0=IDLE) : 0x%08x\n", iuChanNum, (unsigned int)iuDmap->ctrl[iuChanNum].config));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "HALTING DMA chan %d....", iuChanNum ));

   /* Set the burstHalt bit while clearing endma bit */
   iuDmap->ctrl[iuChanNum].config = ( BCM6838_IUDMA_CONFIG_PKTHALT );

   while( ( iuDmap->ctrl[iuChanNum].config & BCM6838_IUDMA_CONFIG_ENDMA ) != 0x00000000 )
   {
      /* Set the burstHalt bit while clearing endma bit */
      iuDmap->ctrl[iuChanNum].config = ( BCM6838_IUDMA_CONFIG_PKTHALT );
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DMA chan %d HALT Waiting, iteration:%d\n", iuChanNum, i ));
      i++;
   }
   i = 0;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DMA chan%d IDLE\n", iuChanNum ));
   iuDmap->ctrl[iuChanNum].intMask = 0;
   iuDmap->ctrl[iuChanNum].intStat |= iuDmap->ctrl[iuChanNum].intStat;

   /* Explicitly Reset DMA channel */
   iuDmap->regs.channelReset |= 1 << iuChanNum;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   apm6838_iudmaDeinit
**
** PURPOSE:    De-initialize the APM IUDMA individual channels
**
** PARAMETERS: iuDmap       - pointer to IUDMA module
**
** RETURNS:    none
**
*****************************************************************************
*/
static int apm6838_iudmaDeinit( volatile BCM6838_IUDMA *iuDmap )
{
   int chnum;

   /* De-initialize the Rx/Tx IUDMA pair for each APM channel */
   for ( chnum = 0; chnum < APM6838_NUM_CHANNELS; chnum++ )
   {
      apm6838_iudmaChanDeinit(gApmDmaInfo[chnum].rxChan.iuChanNum, iuDmap);
      apm6838_iudmaChanDeinit(gApmDmaInfo[chnum].txChan.iuChanNum, iuDmap);
   }

   return ( 0 );
}


/*****************************************************************************
*
*  FUNCTION: apm6838_dmaDeinit
*
*  PURPOSE:
*
*  PARAMETERS:
*      apmp     - pointer to 6838's APM module
*      dmap     - pointer to IUDMA module
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
static int apm6838_dmaDeinit
(
   volatile BCM6838_APM *apmp,
   volatile BCM6838_IUDMA *dmap
)
{
   /* Stop the IUDMA */
   apm6838_iudmaDeinit( dmap );
   deinitIudma( dmap );

   /* Disable all APM DMAs */
   apmp->apm_dev_control &= ~( (RX_DMA_ENABLE_A | TX_DMA_ENABLE_A)  |
                               (RX_DMA_ENABLE_B | TX_DMA_ENABLE_B) );

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: apm_pllStart
*
*  PURPOSE:  Initalize APM PLL
*
*  PARAMETERS:
*      pApm     - pointer to 6838's APM module
*
*  RETURNS:  Success or Failure
*
*  NOTES:
*
*****************************************************************************/
static int apm_pllStart(volatile BCM6838_APM *pApm)
{
   volatile XDRV_UINT32 temp = 0;
   XDRV_BOOL bStandAlone = XDRV_TRUE;

#if DPLL_LOCK_WAIT
   XDRV_UINT32 i;
   int retry_count = -1;
   
   gPllClockFailed = 0;
 retry_lock:
   /* Keep track of retrying DPLL phase lock */
   retry_count ++;
   if ( retry_count > DPLL_RETRY_CNT ) {
       XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: DPLL failed to Lock %d times", __FUNCTION__, DPLL_RETRY_CNT));
       /* Let PCM block know that PLL failed to initialize */
       gPllClockFailed = 1;
       return -1;
   }

   i = 0;
#endif /* DPLL_LOCK_WAIT */

   pApm->dpll_control &= ~(DPLL_SOFT_INIT | DPLL_FORCE_ACQ | DPLL_OPEN_LOOP | DPLL_CONST_K);
   pApm->dpll_control &= ~0xF;
   pApm->dpll_control |= ( (5 << DPLL_PHS_THSHLD_SHIFT) | 12 );
   pApm->dpll_nom_freq = 0xa7c5ac47;

#if HW_NTR_SUPPORT
   /* Setup DPLL registers based on WAN type */   
   switch (rdpa_wan_type_get())
   {
      case rdpa_wan_gpon:
      {
         /* Local clk is generated via DPLL NCO @ 65.536Mhz *
         ** Ref clk from GPON Serdes is @ 256Khz            *
         ** Phase comparator compares both clocks @ 8Khz    *
         ** 8Khz = 65.536Mhz / (8191+1) = 256Khz / (31+1)   *
         ** local_div = 8191, ref_div = 31                  *
         */
         pApm->dpll_div = (31 << DPLL_REF_DIV_SHIFT) | 8191;
         bStandAlone = XDRV_FALSE;
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%s: DPLL programmed for GPON refclk @ 256Khz", __FUNCTION__));
      }
      break;

      case rdpa_wan_epon:
      {
         /* Local clk is generated via DPLL NCO @ 65.536Mhz *
         ** Ref clk from EPON Serdes is @ 40Khz             *
         ** Phase comparator compares both clocks @ 8Khz    *
         ** 8Khz = 65.536Mhz / (8191+1) = 40Khz / (4+1)     *
         ** local_div = 8191, ref_div = 4                   *
         */
         pApm->dpll_div = (4 << DPLL_REF_DIV_SHIFT) | 8191;
         bStandAlone = XDRV_FALSE;
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%s: DPLL programmed for EPON refclk @ 40Khz", __FUNCTION__));
      }
      break;

      default:
      {
         XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: Unknown WAN mode, switching to stand_alone mode!!", __FUNCTION__ ));
         bStandAlone = XDRV_TRUE;
      }
      break;
   }
#endif /* HW_NTR_SUPPORT    */

   /* Force loop aquisition */
   pApm->dpll_control |= DPLL_FORCE_ACQ;
   pApm->dpll_control &= ~DPLL_FORCE_ACQ;

   /* Read DPLL status */
   temp = pApm->dpll_status;

#if DPLL_LOCK_WAIT
   /* Waiting for the DPLL_IN_SYNC and DPLL_ACQ_FREQ_VALID bits to be set */
   while(  !bStandAlone && (!(temp & DPLL_IN_SYNC) || !(temp & DPLL_ACQ_FREQ_VALID))  )
   {
      temp = pApm->dpll_status;
      i++;

      if ( i == DPLL_LOCK_MAX_WAIT*5 )
      {
         XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: DPLL failed to Lock, dpll_status = 0x%08x", __FUNCTION__, (unsigned int) temp));
	 goto retry_lock;
      }
   }

   if( !bStandAlone )
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%s: DPLL Locked, Iteration = %u, dpll_status = 0x%08x, dpll_acq_freq = 0x%08x",
                     __FUNCTION__, (unsigned int)i, (unsigned int) temp, (unsigned int) pApm->dpll_acq_freq));
   }
   else
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%s: DPLL Lock wait skipped , stand_alone mode", __FUNCTION__));
   }
#else
   XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "%s: DPLL Lock wait skipped , dpll_status = 0x%08x", __FUNCTION__, (unsigned int) temp));
#endif /* DPLL_LOCK_WAIT */

   return ( 0 );

}


/*****************************************************************************
*
*  FUNCTION: apm6838_regInit
*
*  PURPOSE:  Initalize APM block
*
*  PARAMETERS:
*      pApm     - pointer to 6838's APM module
*      pCfg     - pointer to configuration info
*
*  RETURNS:  Success or Failure
*
*  NOTES:
*
*****************************************************************************/
static int apm6838_regInit(volatile BCM6838_APM *pApm, const APM6838_CFG *pCfg )
{
   const FREQ_TABLE *tablep;


      tablep = &freqTableDPLL[0];

   /* Find the matching sampleRate in the freq config table */
   while(( pCfg->sampleRate != tablep->sampleRate ) &&
         ( tablep->sampleRate != -1 ))
   {
      tablep++;
   }

   if( tablep->sampleRate == -1 )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "APM6838 regInit ERROR: Unsupported sample rate (%d) !!! ", pCfg->sampleRate));
      return ( -1 );
   }

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "APM6838 regInit: Using %d kHz sample rate.", tablep->sampleRate));

   /*
   ** Initialize clock control registers
   */

   /* Load frequency control word from NTP block */
   pApm->reg_fcw_config_1 = ( tablep->fcw_input & FCW_REG );

   /* Toggle bits for loading FCW into channels A and B */
   pApm->reg_fcw_config_2 = (( 0xF << FCW_LIMIT_SHIFT ) & FCW_LIMIT );
   pApm->reg_fcw_config_2 = (( 1 << LOAD_MISC_B_SHIFT ) |
                             ( 1 << LOAD_MISC_A_SHIFT ) |
                             ( 1 << LOAD_FCW_NOM_B_SHIFT ) |
                             ( 1 << LOAD_FCW_NOM_A_SHIFT ) |
                             ( 1 << LOAD_NTP_SHIFT ) |
                             (( 0xF << FCW_LIMIT_SHIFT ) & FCW_LIMIT ));
   pApm->reg_fcw_config_2 = (( 0xF << FCW_LIMIT_SHIFT ) & FCW_LIMIT );

   /* Load NTP control registers */
   pApm->reg_ntp_config_1 = ( 0x176 & NTP_TIME_DVD );
   pApm->reg_ntp_config_2 = ( 0 & NTP_READ_TIME );

   /*
   ** Initialize Channel A Audio Channel Processor (ACP) registers
   */

   /* Clear the bits at RX_IN_MUX_CTRL to set input to RX data */
   pApm->reg_cha_test_config = (((0 << RX_IN_MUX_CTRL_SHIFT) & RX_IN_MUX_CTRL) |
                                (0 << NETWORK_LPBK_SHIFT)                  |
                                ((0 << RX_98K_MUX_CTRL_SHIFT) & RX_98K_MUX_CTRL) |
                                ((1 << ADC_MUX_CTRL_SHIFT) & ADC_MUX_CTRL) |
                                (0 << ADC_DAC_LPBK_SHIFT)                  |
                                (0 << RX_TX_98K_LPBK_SHIFT)                |
                                (0 << SLEW_INSTANT_SHIFT)                  |
                                (1 << ASRC_EN_SHIFT));

   /* Initializes state machines and registers */
   pApm->reg_cha_eq_config = ((( 0x4F << EQ_RX_NUM_TAPS_SHIFT ) & EQ_RX_NUM_TAPS) |
                              (( 0x4F << EQ_TX_NUM_TAPS_SHIFT) & EQ_TX_NUM_TAPS) |
                              (( 0x6 << EQ_RX_SHFT_SHIFT) & EQ_RX_SHFT) |
                              (( 0xA << EQ_TX_SHFT_SHIFT) & EQ_TX_SHFT) |
                              ( 0 << EQ_RX_IMP_RESP_SHIFT) |
                              ( 0 << EQ_TX_IMP_RESP_SHIFT) |
                              ( 1 << EQ_TX_ENABLE_SHIFT) |
                              ( 1 << EQ_RX_ENABLE_SHIFT) |
                              ( 1 << SOFT_INIT_SHIFT));

   /* Disable Y-filter to turn off sidetone */
   pApm->reg_cha_hybal_config = ((( 0x05 << HISPD_HYBAL_SHFT_SHIFT ) & HISPD_HYBAL_SHFT) |
                                 (( 0x10 << LOSPD_HYBAL_SHFT_SHIFT ) & LOSPD_HYBAL_SHFT) |
                                 ( 1 << HISPD_HYBAL_EN_SHIFT ) |
                                 ( 0 << LOSPD_HYBAL_EN_SHIFT ) |
                                 (( 4 << HISPD_HYBAL_SMPL_OFFSET_SHIFT ) & HISPD_HYBAL_SMPL_OFFSET) |
                                 ( 1 << YFLTR_EN_SHIFT ) |
                                 ( 1 << HPF_EN_SHIFT ) |
                                 (( 0 << LOSPD_HYBAL_SMPL_OFFSET_SHIFT ) & LOSPD_HYBAL_SMPL_OFFSET));

   /* Set ASRC_INT_COEFF_SEL for low frequency (8/16 kHz sampling ) */
   pApm->reg_cha_int_config_1 = (((0x13 << ASRC_INT_NUM_SECT_SHIFT) & ASRC_INT_NUM_SECT) |
                                 ((2 << ASRC_INT_HRNR_SHFT_SHIFT) & ASRC_INT_HRNR_SHFT)|
                                 (0 << ASRC_INT_SYM_ASYM_SHIFT) |
                                 ((0 << ASRC_INT_COEFF_SEL_SHIFT) & ASRC_INT_COEFF_SEL ));

   pApm->reg_cha_int_config_2 = ((0x720A & ASRC_INT_SCALE) |
                                 ((3 << ASRC_INT_FIR_SHFT_SHIFT) & ASRC_INT_FIR_SHFT) |
                                 ((0 << ASRC_SLEW_SPEED_SHIFT) & ASRC_SLEW_SPEED));

   pApm->reg_cha_pm_config_1 = (((0 << PM_ON_TIME_SHIFT) & PM_ON_TIME) |
                                ((0 << PM_OFF_TIME_SHIFT) & PM_OFF_TIME) |
                                (0 << PM_FREQ_16_12_SHIFT ));

   pApm->reg_cha_pm_config_2 = (((0 << PM_SCALE_SHIFT) & PM_SCALE) |
                                ((0 << PM_RISE_INC_SHIFT) & PM_RISE_INC) |
                                ((0 << PM_FALL_INC_SHIFT) & PM_FALL_INC));

   pApm->reg_cha_pm_config_3 = ((0 << PM_ENABLE_SHIFT ) |
                                ((0 << PM_BURST_LENGTH_SHIFT) & PM_BURST_LENGTH) |
                                (0 << PM_BURST_CONT_SHIFT) |
                                (0 << PM_BURST_START_SHIFT));

   /* Configure CIC register values */
   pApm->reg_cha_cic_config =  (((4 << CIC_DEC_SHFT_SHIFT) & CIC_DEC_SHFT) |
                                ((4 << CIC_INC_SHFT_SHIFT) & CIC_INC_SHFT) |
                                (1 << CIC_DEC_EQ_EN_SHIFT) |
                                (1 << CIC_INC_EQ_EN_SHIFT));

   /* Set ASRC_DEC_COEFF_SEL for low frequency (8/16 kHz sampling ) */
   pApm->reg_cha_asrc_dec_config = (((0xD11 << ASRC_DEC_SCALE_SHIFT) & ASRC_DEC_SCALE) |
                                    ((0x13 << ASRC_DEC_NUM_SECT_SHIFT) & ASRC_DEC_NUM_SECT) |
                                    ((3 << ASRC_DEC_SHFT_SHIFT) & ASRC_DEC_SHFT) |
                                    (0 << ASRC_DEC_SYM_ASYM_SHIFT ) |
                                    ((0 << ASRC_DEC_COEFF_SEL_SHIFT) & ASRC_DEC_COEFF_SEL));

   pApm->reg_cha_fcw_config_1 = (tablep->fcw_scale & FCW_SCALE);

   pApm->reg_cha_fcw_config_2 = (((tablep->fcw_shift << FCW_SHFT_SHIFT) & FCW_SHFT) |
                                 ((tablep->clkSource << CLOCK_SOURCE_SHIFT) & CLOCK_SOURCE));

   pApm->reg_cha_digmod_config = (((0x80 << DIGMOD_DITH_SCALE_SHIFT) & DIGMOD_DITH_SCALE ) |
                                  (0 <<  DIGMOD_DEM_DIS_SHIFT) |
                                  (0 << DIGMOD_DEM_TONE_SHIFT));

   /*
   ** Initialize Channel B Audio Channel Processor (ACP) registers
   */

   /* Clear the bits at RX_IN_MUX_CTRL to set input to RX data */
   pApm->reg_chb_test_config = (((0 << RX_IN_MUX_CTRL_SHIFT) & RX_IN_MUX_CTRL) |
                                (0 << NETWORK_LPBK_SHIFT)                  |
                                ((0 << RX_98K_MUX_CTRL_SHIFT) & RX_98K_MUX_CTRL) |
                                ((1 << ADC_MUX_CTRL_SHIFT) & ADC_MUX_CTRL) |
                                (0 << ADC_DAC_LPBK_SHIFT)                  |
                                (0 << RX_TX_98K_LPBK_SHIFT)                |
                                (0 << SLEW_INSTANT_SHIFT)                  |
                                (1 << ASRC_EN_SHIFT));

   /* Initializes state machines and registers */
   pApm->reg_chb_eq_config = ((( 0x4F << EQ_RX_NUM_TAPS_SHIFT ) & EQ_RX_NUM_TAPS) |
                              (( 0x4F << EQ_TX_NUM_TAPS_SHIFT) & EQ_TX_NUM_TAPS) |
                              (( 0x6 << EQ_RX_SHFT_SHIFT) & EQ_RX_SHFT) |
                              (( 0xA << EQ_TX_SHFT_SHIFT) & EQ_TX_SHFT) |
                              ( 0 << EQ_RX_IMP_RESP_SHIFT) |
                              ( 0 << EQ_TX_IMP_RESP_SHIFT) |
                              ( 1 << EQ_TX_ENABLE_SHIFT) |
                              ( 1 << EQ_RX_ENABLE_SHIFT) |
                              ( 1 << SOFT_INIT_SHIFT));

   /* Disable Y-filter to turn off sidetone */
   pApm->reg_chb_hybal_config = ((( 0x05 << HISPD_HYBAL_SHFT_SHIFT ) & HISPD_HYBAL_SHFT) |
                                 (( 0x10 << LOSPD_HYBAL_SHFT_SHIFT ) & LOSPD_HYBAL_SHFT) |
                                 ( 1 << HISPD_HYBAL_EN_SHIFT ) |
                                 ( 0 << LOSPD_HYBAL_EN_SHIFT ) |
                                 (( 4 << HISPD_HYBAL_SMPL_OFFSET_SHIFT ) & HISPD_HYBAL_SMPL_OFFSET) |
                                 ( 1 << YFLTR_EN_SHIFT ) |
                                 ( 1 << HPF_EN_SHIFT ) |
                                 (( 0 << LOSPD_HYBAL_SMPL_OFFSET_SHIFT ) & LOSPD_HYBAL_SMPL_OFFSET));

   /* Set ASRC_INT_COEFF_SEL for low frequency (8/16 kHz sampling ) */
   pApm->reg_chb_int_config_1 = (((0x13 << ASRC_INT_NUM_SECT_SHIFT) & ASRC_INT_NUM_SECT) |
                                 ((2 << ASRC_INT_HRNR_SHFT_SHIFT) & ASRC_INT_HRNR_SHFT)|
                                 (0 << ASRC_INT_SYM_ASYM_SHIFT) |
                                 ((0 << ASRC_INT_COEFF_SEL_SHIFT) & ASRC_INT_COEFF_SEL ));

   pApm->reg_chb_int_config_2 = ((0x720A & ASRC_INT_SCALE) |
                                 ((3 << ASRC_INT_FIR_SHFT_SHIFT) & ASRC_INT_FIR_SHFT) |
                                 ((0 << ASRC_SLEW_SPEED_SHIFT) & ASRC_SLEW_SPEED));

   pApm->reg_chb_pm_config_1 = (((0 << PM_ON_TIME_SHIFT) & PM_ON_TIME) |
                                ((0 << PM_OFF_TIME_SHIFT) & PM_OFF_TIME) |
                                (0 << PM_FREQ_16_12_SHIFT ));

   pApm->reg_chb_pm_config_2 = (((0 << PM_SCALE_SHIFT) & PM_SCALE) |
                                ((0 << PM_RISE_INC_SHIFT) & PM_RISE_INC) |
                                ((0 << PM_FALL_INC_SHIFT) & PM_FALL_INC));

   pApm->reg_chb_pm_config_3 = ((0 << PM_ENABLE_SHIFT ) |
                                ((0 << PM_BURST_LENGTH_SHIFT) & PM_BURST_LENGTH) |
                                (0 << PM_BURST_CONT_SHIFT) |
                                (0 << PM_BURST_START_SHIFT));

   /* Configure CIC register values */
   pApm->reg_chb_cic_config =  (((4 << CIC_DEC_SHFT_SHIFT) & CIC_DEC_SHFT) |
                                ((4 << CIC_INC_SHFT_SHIFT) & CIC_INC_SHFT) |
                                (1 << CIC_DEC_EQ_EN_SHIFT) |
                                (1 << CIC_INC_EQ_EN_SHIFT));

   /* Set ASRC_DEC_COEFF_SEL for low frequency (8/16 kHz sampling ) */
   pApm->reg_chb_asrc_dec_config = (((0xD11 << ASRC_DEC_SCALE_SHIFT) & ASRC_DEC_SCALE) |
                                    ((0x13 << ASRC_DEC_NUM_SECT_SHIFT) & ASRC_DEC_NUM_SECT) |
                                    ((3 << ASRC_DEC_SHFT_SHIFT) & ASRC_DEC_SHFT) |
                                    (0 << ASRC_DEC_SYM_ASYM_SHIFT ) |
                                    ((0 << ASRC_DEC_COEFF_SEL_SHIFT) & ASRC_DEC_COEFF_SEL));

   pApm->reg_chb_fcw_config_1 = (tablep->fcw_scale & FCW_SCALE);

   pApm->reg_chb_fcw_config_2 = (((tablep->fcw_shift << FCW_SHFT_SHIFT) & FCW_SHFT) |
                                 ((tablep->clkSource << CLOCK_SOURCE_SHIFT) & CLOCK_SOURCE));

   pApm->reg_chb_digmod_config = (((0x80 << DIGMOD_DITH_SCALE_SHIFT) & DIGMOD_DITH_SCALE ) |
                                  (0 <<  DIGMOD_DEM_DIS_SHIFT) |
                                  (0 << DIGMOD_DEM_TONE_SHIFT));

   /*
   ** Initialize Common ACP registers
   */

   /* Load ring control registers */
   if( pCfg->hvgType == XDRV_APM_HVG_DESIGN_FLYBACK )
   {
      pApm->reg_ring_config_2 = ((0 & RING_OFFSET) |
                                ((0x1A54 << RING_SCALE_SHIFT) & RING_SCALE) );

      pApm->reg_ring_config_3 = ((0 & RING_OFFSET) |
                                ((0x1A54 << RING_SCALE_SHIFT) & RING_SCALE) );
   }
   else
   {
      pApm->reg_ring_config_2 = ((0 & RING_OFFSET) |
                                ((0x4600 << RING_SCALE_SHIFT) & RING_SCALE) );

      pApm->reg_ring_config_3 = ((0 & RING_OFFSET) |
                                ((0x4600 << RING_SCALE_SHIFT) & RING_SCALE) );
   }

   pApm->reg_ring_config_1 = ((( 0x33B << RING_OMEGA_NORM_SHIFT ) & RING_OMEGA_NORM ) |
                              (( 0x00 << RING_DELAY_SHIFT ) & RING_DELAY ) |
                              (( 0 << RING_GEN_SOFT_INIT_SHIFT ) & RING_GEN_SOFT_INIT ) |
                              (( 0 << RING_START_IMMED_A_SHIFT ) & RING_START_IMMED_A ) |
                              (( 0 << RING_START_IMMED_B_SHIFT ) & RING_START_IMMED_B ) |
                              (( 0 << RING_START_NEXT_PHASE_A_SHIFT ) & RING_START_NEXT_PHASE_A ) |
                              (( 0 << RING_START_NEXT_PHASE_B_SHIFT ) & RING_START_NEXT_PHASE_B ) |
                              (( 0 << RING_STOP_IMMED_A_SHIFT ) & RING_STOP_IMMED_A ) |
                              (( 0 << RING_STOP_IMMED_B_SHIFT ) & RING_STOP_IMMED_B ) |
                              (( 0 << RING_STOP_NEXT_PHASE_A_SHIFT ) & RING_STOP_NEXT_PHASE_A ) |
                              (( 0 << RING_STOP_NEXT_PHASE_B_SHIFT ) & RING_STOP_NEXT_PHASE_B ));

   pApm->reg_ring_config_4 = ((( 0 << RING_GEN_PHASE_A_SHIFT ) & RING_GEN_PHASE_A ) |
                              (( 0 << RING_GEN_PHASE_B_SHIFT ) & RING_GEN_PHASE_B ) |
                             ((1 << RING_GEN_ENABLE_A_SHIFT) & RING_GEN_ENABLE_A) |
                              (( 1 << RING_GEN_ENABLE_B_SHIFT ) & RING_GEN_ENABLE_B ) |
                              (( 0 << RING_CREST_FACTOR_A_SHIFT ) & RING_CREST_FACTOR_A ) |
                              (( 0 << RING_CREST_FACTOR_B_SHIFT ) & RING_CREST_FACTOR_B ));

   pApm->reg_stats_config = ( STATS_MUX_CTL_VADCA |
                              STATS_MUX_CTL_VADCB );

   /* Set soft init bit to start using new register values */
   pApm->reg_ser_config = ((( 1 << SOFT_INIT_AP_SHIFT ) & SOFT_INIT_AP ) |
                           (( 0 << SER_TST_CLKS_SHIFT ) & SER_TST_CLKS ) |
                           (( 0 << AUD_SER_TST_FCW_MUX_CTRL_SHIFT ) & AUD_SER_TST_FCW_MUX_CTRL ) |
                           (( 0 << SER_MUX_SEL_SHIFT ) & SER_MUX_SEL ));

   /* Clear soft init bits */
   pApm->reg_cha_eq_config &= ~SOFT_INIT;
   pApm->reg_chb_eq_config &= ~SOFT_INIT;
   pApm->reg_ser_config &= ~SOFT_INIT_AP;

   /* Clear all device interrupts */
   pApm->apm_dev_irq_pend = ( DMA_A_RX            |
                              DMA_A_TX            |
                              DMA_B_RX            |
                              DMA_B_TX            |
                              DMA_TX_UNDERFLOW_A  |
                              DMA_TX_UNDERFLOW_B  |
                              DMA_RX_OVERFLOW_A   |
                              DMA_RX_OVERFLOW_B );

   /* Disable all interrupts - There is a dedicated interrupt line
   ** for DMA interrupts which originates directly from the iudma core
   ** we will therefore use that interrupt line and disable the
   ** interrupts originating from the APM block. The dedicated iudma
   ** interrupt line is controlled via the regs.gbl_int_mask */
   pApm->apm_dev_irq_mask &= ~( DMA_A_RX            |
                                DMA_A_TX            |
                                DMA_B_RX            |
                                DMA_B_TX            |
                                DMA_TX_UNDERFLOW_A  |
                                DMA_TX_UNDERFLOW_B  |
                                DMA_RX_OVERFLOW_A   |
                                DMA_RX_OVERFLOW_B );

   /* Clear all channel A interrupts */
   pApm->reg_apm_irq_pend_1 = 0xffffffff;

   /* Clear all channel B interrupts */
   pApm->reg_apm_irq_pend_2 = 0xffffffff;

   /* Clear all IRQ register 3 for stats collection */
   pApm->reg_apm_irq_pend_3 = 0xffffffff;

   /* Initialize codec configuration registers */
   pApm->reg_codec_config_0 = 0x00000000;
   pApm->reg_codec_config_1 = 0x00000000;

   /* Determine shuffler settings */

      pApm->reg_codec_config_2 = 0x00100000;
      pApm->reg_codec_config_3 = 0x00010000;

   pApm->reg_codec_config_4 = 0x00000000;

   /* Disable DC offset injection */
   pApm->reg_codec_config_5 = 0x00000012;

   pApm->reg_codec_config_6 = 0x00000000;
   pApm->reg_codec_config_7 = 0x00000000;
   pApm->reg_codec_config_8 = 0x00000000;

   return ( 0 );
}

/*****************************************************************************
*
*  FUNCTION: apm6838_flexicalcConfig
*
*  PURPOSE:
*      Initialize FlexiCalc(tm) controlled APM parameters
*
*  PARAMETERS:
*      pDrv       - 6838 driver interface
*      flexicalcp - pointer to FlexiCalc configuration structure
*
*  RETURNS: 0 on success
*
*  NOTES:
*
*****************************************************************************/
int apm6838_flexicalcConfig
(
   volatile BCM6838_APM *pApm,
   const APM6838_FLEXICALC_CFG *flexicalcp,
   APM_6838_DRV *pDrv
)
{
   volatile XDRV_UINT32* pRegApmAudioAdcA;
   volatile XDRV_UINT32* pRegApmAudioAdcB;
   volatile XDRV_UINT32* pRegApmAudioDac;
   XDRV_UINT32 asrcDecScaleTemp;
   XDRV_UINT32 asrcIntScaleTemp;
   XDRV_SINT64 kFactorAdjNum;
   XDRV_SINT64 kFactorAdjDiv;
   XDRV_SINT64 kDacAdjNum;
   XDRV_SINT64 kDacAdjDiv;
   pRegApmAudioAdcA = &pApm->reg_codec_config_2;
   pRegApmAudioAdcB = &pApm->reg_codec_config_3;
   pRegApmAudioDac  = &pApm->reg_codec_config_4;
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "---------------- FLEXICALC CONFIGURATION BEGIN --------------"));

   /* Retrieve adjustment values */
   apm6838CalculateKadjValues( &kFactorAdjNum, &kFactorAdjDiv, &kDacAdjNum, &kDacAdjDiv );
   asrcIntScaleTemp = divRound( flexicalcp->asrc_int_scale * kDacAdjNum, kDacAdjDiv );
   asrcDecScaleTemp = divRound( flexicalcp->asrc_dec_scale * kFactorAdjNum, kFactorAdjDiv );

   /* Load High-speed Hybrid Balance coefficients for both channels */
   if ( WriteApmFilterCoeffs( pApm, pDrv, APM6838_ALL_CHANNELS, APM6838_FILTER_HISPD_HYBRID_BAL, (int *)&flexicalcp->hybal_audio_fir[0], APM_HYBAL_NUM_TAPS ))
   {
      return ( 1 );
   }

   /* Load High-speed Hybrid Balance coefficients for both channels */
   if ( WriteApmFilterCoeffs( pApm, pDrv, APM6838_ALL_CHANNELS, APM6838_FILTER_LOSPD_HYBRID_BAL, (int *)&flexicalcp->hybal_audio_fir[0], APM_HYBAL_NUM_TAPS ))
   {
      return ( 1 );
   }

   /* Load Y-filter coefficients for both channels */
   if ( WriteApmFilterCoeffs( pApm, pDrv, APM6838_ALL_CHANNELS, APM6838_FILTER_Y_FILTER, (int *)&flexicalcp->yfltr_iir2[0], APM_YFLTR_BLOCK_SIZE ))
   {
      return ( 1 );
   }

   /* Load RX equalizer coefficients for both channels */
   if ( WriteApmFilterCoeffs( pApm, pDrv, APM6838_ALL_CHANNELS, APM6838_FILTER_RX_EQ, (int *)&flexicalcp->eq_rx_filter[0], APM_RX_EQ_NUM_TAPS ))
   {
      return ( 1 );
   }

   /* Load TX equalizer coefficients for both channels */
   if ( WriteApmFilterCoeffs( pApm, pDrv, APM6838_ALL_CHANNELS, APM6838_FILTER_TX_EQ, (int *)&flexicalcp->eq_tx_filter[0], APM_TX_EQ_NUM_TAPS ))
   {
      return ( 1 );
   }

   /* Reconfigure channel A registers with custom values */
   pApm->reg_cha_hybal_config &= ~( HISPD_HYBAL_SHFT | HISPD_HYBAL_EN | HPF_EN | HISPD_HYBAL_SMPL_OFFSET | YFLTR_EN );
   pApm->reg_cha_hybal_config |= (((( flexicalcp->hybal_shft + pDrv->hybalShiftAdj ) << HISPD_HYBAL_SHFT_SHIFT ) & HISPD_HYBAL_SHFT ) |
                                  ( flexicalcp->hybal_en << HISPD_HYBAL_EN_SHIFT ) |
                                  (( flexicalcp->hybal_smpl_offset << HISPD_HYBAL_SMPL_OFFSET_SHIFT ) & HISPD_HYBAL_SMPL_OFFSET ) |
                                  ( flexicalcp->yfltr_en << YFLTR_EN_SHIFT ) |
                                  ( flexicalcp->hpf_en << HPF_EN_SHIFT ));

   pApm->reg_cha_eq_config &= ~( EQ_RX_SHFT | EQ_TX_SHFT | EQ_RX_IMP_RESP | EQ_TX_IMP_RESP );
   pApm->reg_cha_eq_config |= ((( flexicalcp->eq_rx_shft << EQ_RX_SHFT_SHIFT ) & EQ_RX_SHFT ) |
                               (( flexicalcp->eq_tx_shft << EQ_TX_SHFT_SHIFT ) & EQ_TX_SHFT ) |
                               ( flexicalcp->eq_imp_resp << EQ_RX_IMP_RESP_SHIFT ) |
                               ( flexicalcp->eq_imp_resp << EQ_TX_IMP_RESP_SHIFT ));

   pApm->reg_cha_cic_config &= ~( CIC_DEC_SHFT | CIC_INC_SHFT );
   pApm->reg_cha_cic_config |= ((( flexicalcp->cic_dec_shft << CIC_DEC_SHFT_SHIFT ) & CIC_DEC_SHFT ) |
                               (( flexicalcp->cic_inc_shft << CIC_INC_SHFT_SHIFT ) & CIC_INC_SHFT ));

   pApm->reg_cha_int_config_2 &= ~( ASRC_INT_SCALE );
   pApm->reg_cha_int_config_2 |= ( asrcIntScaleTemp & ASRC_INT_SCALE );

   pApm->reg_cha_asrc_dec_config &= ~( ASRC_DEC_SCALE );
   pApm->reg_cha_asrc_dec_config |= (( asrcDecScaleTemp << ASRC_DEC_SCALE_SHIFT ) & ASRC_DEC_SCALE );

   /* Reconfigure channel B registers with custom values */
   pApm->reg_chb_hybal_config &= ~( HISPD_HYBAL_SHFT | HISPD_HYBAL_EN | HPF_EN | HISPD_HYBAL_SMPL_OFFSET | YFLTR_EN );
   pApm->reg_chb_hybal_config |= (((( flexicalcp->hybal_shft + pDrv->hybalShiftAdj ) << HISPD_HYBAL_SHFT_SHIFT ) & HISPD_HYBAL_SHFT ) |
                                  ( flexicalcp->hybal_en << HISPD_HYBAL_EN_SHIFT ) |
                                  (( flexicalcp->hybal_smpl_offset << HISPD_HYBAL_SMPL_OFFSET_SHIFT ) & HISPD_HYBAL_SMPL_OFFSET ) |
                                  ( flexicalcp->yfltr_en << YFLTR_EN_SHIFT ) |
                                  ( flexicalcp->hpf_en << HPF_EN_SHIFT ));

   pApm->reg_chb_eq_config &= ~( EQ_RX_SHFT | EQ_TX_SHFT | EQ_RX_IMP_RESP | EQ_TX_IMP_RESP );
   pApm->reg_chb_eq_config |= ((( flexicalcp->eq_rx_shft << EQ_RX_SHFT_SHIFT ) & EQ_RX_SHFT ) |
                               (( flexicalcp->eq_tx_shft << EQ_TX_SHFT_SHIFT ) & EQ_TX_SHFT ) |
                               ( flexicalcp->eq_imp_resp << EQ_RX_IMP_RESP_SHIFT ) |
                               ( flexicalcp->eq_imp_resp << EQ_TX_IMP_RESP_SHIFT ));

   pApm->reg_chb_cic_config &= ~( CIC_DEC_SHFT | CIC_INC_SHFT );
   pApm->reg_chb_cic_config |= ((( flexicalcp->cic_dec_shft << CIC_DEC_SHFT_SHIFT ) & CIC_DEC_SHFT ) |
                               (( flexicalcp->cic_inc_shft << CIC_INC_SHFT_SHIFT ) & CIC_INC_SHFT ));

   pApm->reg_chb_int_config_2 &= ~( ASRC_INT_SCALE );
   pApm->reg_chb_int_config_2 |= ( asrcIntScaleTemp & ASRC_INT_SCALE );

   pApm->reg_chb_asrc_dec_config &= ~( ASRC_DEC_SCALE );
   pApm->reg_chb_asrc_dec_config |= (( asrcDecScaleTemp << ASRC_DEC_SCALE_SHIFT ) & ASRC_DEC_SCALE );

   /* Reconfigure codec registers with custom values */
   *pRegApmAudioAdcA &= ~( PGA_CTL_VADC );
   *pRegApmAudioAdcA |= (( flexicalcp->vtx_pg << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );

   *pRegApmAudioAdcB &= ~( PGA_CTL_VADC );
   *pRegApmAudioAdcB |= (( flexicalcp->vtx_pg << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );

   *pRegApmAudioDac &= ~( PGA_GAIN_CTL_VDAC_A | PGA_GAIN_CTL_VDAC_B );
   *pRegApmAudioDac |= ((( flexicalcp->vrx_pg << PGA_GAIN_CTL_VDAC_A_SHIFT ) & PGA_GAIN_CTL_VDAC_A ) |
                        (( flexicalcp->vrx_pg << PGA_GAIN_CTL_VDAC_B_SHIFT ) & PGA_GAIN_CTL_VDAC_B ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "----------------- FLEXICALC CONFIGURATION END ---------------"));

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   apm6838DebugStatusDump
**
** PURPOSE:    Dump APM registers for debug purpose
**
** PARAMETERS: pDrv  - pointer to the APM driver interface
**
** RETURNS:    0 on success
**
** NOTES:      none
**
*****************************************************************************
*/
static XDRV_SINT32 apm6838DebugStatusDump( XDRV_APM *pDrv )
{
   APM_6838_DRV            *pDev  = (APM_6838_DRV *)pDrv;
   volatile BCM6838_APM    *pApm;
   volatile BCM6838_IUDMA  *iuDmap;
   int                      chnum;

   /* Note: late assignment of variable to allow compilation of SLIM build
            (build with no XDRV_LOG defined). */
   pApm = pDev->pApm;
   iuDmap = pDev->pIuDma;

   for ( chnum=0; chnum<APM6838_NUM_CHANNELS; chnum++ )
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "APM IUDMA RX chan %d status info ...", chnum));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  baseDescp  0x%08lx;",
            iuDmap->stram[gApmDmaInfo[chnum].rxChan.iuChanNum].baseDescPointer));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  cfg        0x%04x;   burst      0x%04x;",
            (unsigned int)iuDmap->ctrl[gApmDmaInfo[chnum].rxChan.iuChanNum].config,
            (unsigned int)iuDmap->ctrl[gApmDmaInfo[chnum].rxChan.iuChanNum].maxBurst));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  intStatus  0x%04x;   intMask    0x%04x; ",
            iuDmap->ctrl[gApmDmaInfo[chnum].rxChan.iuChanNum].intStat,
            iuDmap->ctrl[gApmDmaInfo[chnum].rxChan.iuChanNum].intMask));

      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "APM IUDMA TX chan %d status info ...", chnum));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  baseDescp  0x%08lx;",
            iuDmap->stram[gApmDmaInfo[chnum].txChan.iuChanNum].baseDescPointer));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  cfg        0x%04x;   burst      0x%04x;",
            (unsigned int)iuDmap->ctrl[gApmDmaInfo[chnum].txChan.iuChanNum].config,
            (unsigned int)iuDmap->ctrl[gApmDmaInfo[chnum].txChan.iuChanNum].maxBurst));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  intStatus  0x%04x;   intMask    0x%04x; ",
            iuDmap->ctrl[gApmDmaInfo[chnum].txChan.iuChanNum].intStat,
            iuDmap->ctrl[gApmDmaInfo[chnum].txChan.iuChanNum].intMask));
   }

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, ""));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "APM IUDMA Global Interrupt Info ..."));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  gbl_int_status  0x%08x;   gbl_int_mask    0x%08x; ",
            (unsigned int)iuDmap->regs.gbl_int_stat,
            (unsigned int)iuDmap->regs.gbl_int_mask));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, ""));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "APM reg status info ..."));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  devIrq    0x%08x;   devIrqPend  0x%08lx; ",
   (unsigned int)pApm->apm_dev_irq_status, pApm->apm_dev_irq_pend));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  irqPend1  0x%08lx;   irqPend2    0x%08lx; ",
   pApm->reg_apm_irq_pend_1, pApm->reg_apm_irq_pend_2));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  irqPend3  0x%08lx;   ",
   pApm->reg_apm_irq_pend_3));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  status1   0x%08lx;   status2     0x%08lx; ",
   pApm->reg_apm_status_1, pApm->reg_apm_status_2));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  status3   0x%08lx;   status4     0x%08lx; ",
   pApm->reg_apm_status_3, pApm->reg_apm_status_4));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  status5   0x%08lx;   status6     0x%08lx; ",
   pApm->reg_apm_status_5, pApm->reg_apm_status_6));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  status7   0x%08lx;   status8     0x%08lx; ",
   pApm->reg_apm_status_7, pApm->reg_apm_status_8));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  status9   0x%08lx;   status10    0x%08lx; ",
   pApm->reg_apm_status_9, pApm->reg_apm_status_10));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  status11  0x%08lx;   status12    0x%08lx; ",
   pApm->reg_apm_status_11, pApm->reg_apm_status_12));

   if(( pApm->apm_dev_irq_pend & DMA_TX_UNDERFLOW_A ) ||
      ( pApm->apm_dev_irq_pend & DMA_TX_UNDERFLOW_B ) ||
      ( pApm->apm_dev_irq_pend & DMA_RX_OVERFLOW_A ) ||
      ( pApm->apm_dev_irq_pend & DMA_RX_OVERFLOW_B ))
   {
      return ( -1 );
   }
   else
   {
      return ( 0 );
   }
}

/*
*****************************************************************************
** FUNCTION:   apm6838GetDlp
**
** PURPOSE:    Retreive the APM Decode Level Point (receive loss) in dB
**
** PARAMETERS: pDrv  - pointer to the APM driver interface
**
** RETURNS:    DLP in dB
**
*****************************************************************************
*/
static XDRV_SINT16 apm6838GetDlp( XDRV_APM *pDrv )
{
   APM_6838_DRV  *pDev = (APM_6838_DRV *)pDrv;
   XDRV_SINT16   dlp;

   dlp = pDev->Cfg.pFlexiCalc->dBdlp;

   /* The 6838 currently only supports DLP in the [-12dB,0dB] range */
   XCHG_ASSERT(( dlp <= 0 ) && ( dlp >= -12 ));

   return ( dlp );
}

/*
*****************************************************************************
** FUNCTION:   apm6838GetElp
**
** PURPOSE:    Retreive the APM Encode Level Point (ingress loss) in dB
**
** PARAMETERS: pDrv  - pointer to the APM driver interface
**
** RETURNS:    ELP in dB
**
*****************************************************************************
*/
static XDRV_SINT16 apm6838GetElp( XDRV_APM *pDrv )
{
   APM_6838_DRV  *pDev = (APM_6838_DRV *)pDrv;
   XDRV_SINT16   elp;

   elp = pDev->Cfg.pFlexiCalc->dBelp;

   /* The 6838 currently only supports ELP in the [-12dB,0dB] range */
   XCHG_ASSERT(( elp <= 0 ) && ( elp >= -12 ));

   return ( elp );
}

/*
*****************************************************************************
** FUNCTION:   apm6838SetTxRxGains
**
** PURPOSE:    Change the ingress and egress gains in the APM block
**
** PARAMETERS: pDrv       - pointer to the APM driver device structure
**             chan       - channel to modify
**             txGain     - desired ingress gain
**             rxGain     - desired egress gain
**             apmTxGain  - realized ingress gain
**             apmRxGain  - realized egress gain
**
** RETURNS:    none
**
*****************************************************************************
*/
static int apm6838SetTxRxGains( XDRV_APM *pDrv, int chan, int txGain, int rxGain, int *apmTxGain, int *apmRxGain )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pDrv;
   XDRV_SINT16 elp = pDev->Cfg.pFlexiCalc->dBelp;
   XDRV_SINT16 dlp = pDev->Cfg.pFlexiCalc->dBdlp;
   volatile XDRV_UINT32* pRegApmAudioAdcA;
   volatile XDRV_UINT32* pRegApmAudioAdcB;
   volatile XDRV_UINT32* pRegApmAudioDac;
   int hiSpdHyBalShift = pDev->Cfg.pFlexiCalc->hybal_shft + pDev->hybalShiftAdj;
   int pgaCtlVadc = pDev->Cfg.pFlexiCalc->vtx_pg;
   int pgaGainCtlVdac = pDev->Cfg.pFlexiCalc->vrx_pg;
   int yiir2Shift = pDev->Cfg.pFlexiCalc->yfltr_iir1_shift;
   int coeffRamType;

   chan = pDev->channelSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;
   pRegApmAudioAdcA = &pDev->pApm->reg_codec_config_2;
   pRegApmAudioAdcB = &pDev->pApm->reg_codec_config_3;
   pRegApmAudioDac  = &pDev->pApm->reg_codec_config_4;

   /* Adjust RX gain */
   if(( rxGain < abs(dlp) ) && ( pgaGainCtlVdac = 1 ) && ( abs(dlp) >= 6 ))
   {
      hiSpdHyBalShift += 1;
      pgaGainCtlVdac -= 1;
      yiir2Shift -= 0x10;
      *apmRxGain = abs(dlp) - 6;
   }
   else
   {
      *apmRxGain = abs(dlp);
   }

   /* Currently no adjustments are made to TX gain */
   *apmTxGain = abs(elp);

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "APM6838 reprogramming APM gains."));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "TxGain %d, RxGain %d, ApmTxGain %d, ApmTxGain %d",
                   txGain, rxGain, *apmTxGain, *apmRxGain ));

   if( chan == 0 )
   {
      coeffRamType = YFLTR_ACCESS_COEFF_RAM_A;

      /* Update APM register values */
      pDev->pApm->reg_cha_hybal_config &= ~HISPD_HYBAL_SHFT;
      pDev->pApm->reg_cha_hybal_config |= (( hiSpdHyBalShift << HISPD_HYBAL_SHFT_SHIFT ) & HISPD_HYBAL_SHFT );
      *pRegApmAudioAdcA &= ~( PGA_CTL_VADC );
      *pRegApmAudioAdcA |= (( pgaCtlVadc << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );
      *pRegApmAudioDac &= ~PGA_GAIN_CTL_VDAC_A;
      *pRegApmAudioDac |= (( pgaGainCtlVdac << PGA_GAIN_CTL_VDAC_A_SHIFT ) & PGA_GAIN_CTL_VDAC_A );
      pDev->vDac[chan] = pgaGainCtlVdac;
   }
   else
   {
      coeffRamType = YFLTR_ACCESS_COEFF_RAM_B;

      /* Update APM register values */
      pDev->pApm->reg_chb_hybal_config &= ~HISPD_HYBAL_SHFT;
      pDev->pApm->reg_chb_hybal_config |= (( hiSpdHyBalShift << HISPD_HYBAL_SHFT_SHIFT ) & HISPD_HYBAL_SHFT );
      *pRegApmAudioAdcB &= ~( PGA_CTL_VADC );
      *pRegApmAudioAdcB |= (( pgaCtlVadc << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );
      *pRegApmAudioDac &= ~PGA_GAIN_CTL_VDAC_B;
      *pRegApmAudioDac |= (( pgaGainCtlVdac << PGA_GAIN_CTL_VDAC_B_SHIFT ) & PGA_GAIN_CTL_VDAC_B );
      pDev->vDac[chan] = pgaGainCtlVdac;
   }

   /* Update RAM coefficient */
   pDev->pApm->reg_apm_coeff_wr_data = yiir2Shift;
   pDev->pApm->reg_apm_coeff_config = 0;
   pDev->pApm->reg_apm_coeff_config = ( 0x1B & COEFF_PROG_ADDR ) | coeffRamType;
   pDev->pApm->reg_apm_coeff_config = ( 0x1B & COEFF_PROG_ADDR ) | coeffRamType | COEFF_PROG_WE;
   pDev->pApm->reg_apm_coeff_config = ( 0x1B & COEFF_PROG_ADDR ) | coeffRamType;
   pDev->pApm->reg_apm_coeff_config = 0;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   apm6838SetRingParms
**
** PURPOSE:    Sets the APM ringing waveform parameters
**
** PARAMETERS: pDrv           - pointer to the APM driver device structure
**             ringFrequency  - ringing frequency
**             ringWaveshape  - ringing waveshape
**             ringVoltage    - ringing voltage
**             ringOffset     - ringing DC offset
**             ringOffsetCal  - ringing DC offset calibration
**
** RETURNS:    none
**
** NOTES:      Several coefficient values are artificially mutliplied by
**             factors of 10 to correct for rounding problems with fixed
**             point math. These include ring_gain, crest_factor, and
**             ring coefficent for non-linear gain calculations. In addition,
**             the voltages are calculated for 5 REN loads.
**
*****************************************************************************
*/
static void apm6838SetRingParms( XDRV_APM *pDrv, int ringFrequency, int ringWaveshape,
                                 int ringVoltage, int ringOffset, int ringOffsetCal )
{
   APM_6838_DRV *pDev  = (APM_6838_DRV *)pDrv;
   volatile BCM6838_APM *pApm = pDev->pApm;
   volatile BCM6838_HVG *pHvg = BCM6838_HVG_PTR;
   const APM6838_RING_GAIN_INFO *ringGainTablep;
   XDRV_SINT64 ring_gain, crest_factor, peak_ac, peak_ring;
   XDRV_SINT64 ring_amph, ring_offh, ring_offhcal, ring_offhtot;
   XDRV_SINT64 ring_freqh, ring_crest;
   XDRV_SINT64 voh_min, vbat_min, vbat_min1, vres, fsamp, ringFS;
   XDRV_SINT64 hvg_com_ring, hvg_ring_gain, hvg_ring_ovhd;
   XDRV_SINT64 max_hvg_slic;
   XDRV_SINT64 ring_stop = 48000;
   XDRV_SINT64 ring_gen_res = 256;
   XDRV_SINT64 ring_stoph, ring_gen_asin, ring_gen_phase, ring_amp;
   XDRV_UINT32 ring_delay = 0;
   /* XDRV_SINT64 ring_sin_amp */
   XDRV_SINT64 kFactorAdjNum;
   XDRV_SINT64 kFactorAdjDiv;
   XDRV_SINT64 kDacAdjNum;
   XDRV_SINT64 kDacAdjDiv;

   /* Retrieve adjustment values */
   apm6838CalculateKadjValues( &kFactorAdjNum, &kFactorAdjDiv, &kDacAdjNum, &kDacAdjDiv );

#if XDRV_CFG_BALANCED_RINGING
   XDRV_SINT64 vmax_hvg_slic, vreg_ref_gain
#endif /* XDRV_CFG_BALANCED_RINGING */


   /* Use ring voltage stored in flexicalc header file */
   if( ringVoltage == -1 )
   {
      ringVoltage = pDev->Cfg.pFlexiCalc->ring_voltage;
   }

   /* Retrieve correct ring gains for this slic type */
   ringGainTablep = ringGainTable;

   /* Find the matching slic type in the ring gain table */
   while(( ringGainTablep->slicType != pDev->Cfg.pFlexiCalc->slic_type ) &&
         ( ringGainTablep->slicType != 0 ))
   {
      ringGainTablep++;
   }

   /* Verify SLIC supported on this platform */
   if( ringGainTablep->slicType == 0 )
   {
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "Unknown SLIC type in APM ring configuration."));
      return;
   }

   /* Save ring settings */
   pDev->ringVoltage = ringVoltage;
   pDev->ringOffset = ringOffset;
   pDev->ringOffsetCal = ringOffsetCal;
   pDev->ringFrequency = ringFrequency;
   pDev->ringWaveshape = ringWaveshape;

   switch ( ringWaveshape )
   {
      case XDRV_RINGWAVEFORM_TRAPEZOID:
      {
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "APM6838 reprogramming ring registers to trapezoidal waveform."));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Frequency %d, Voltage %d, Offset %d, OffsetCal %d, Type %d",
                         ringFrequency, ringVoltage, ringOffset, ringOffsetCal, ringWaveshape ));

         /* Coefficients based on waveform type */
         /* ring_sin_amp = 20851; */
         ring_crest = 3;
         crest_factor = 12500;   /* Increased by factor of 10000 */
      }
      break;

      case XDRV_RINGWAVEFORM_SINE:
      {
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "APM6838 reprogramming ring registers to sinusoidal waveform."));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Frequency %d, Voltage %d, Offset %d, OffsetCal %d, Type %d",
                         ringFrequency, ringVoltage, ringOffset, ringOffsetCal, ringWaveshape ));

         /* Coefficients based on waveform type */
         /* ring_sin_amp = 32767; */
         ring_crest = 0;
         crest_factor = 14142;   /* Increased by factor of 10000 */
      }
      break;

      default:
      {
         XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "Unknown ringing waveform in APM ring configuration."));
         return;
      }
      break;
   }

   /* Calculate ring waveform voltages ( in mV ) */
   ringVoltage *= 1000;
   ringOffset *= 1000;
   peak_ac = crest_factor * ringVoltage * ( 1400 + 2 * ( ringGainTablep->slicResistance + ringGainTablep->protectionResistance ) );
   peak_ac = apmDiv(peak_ac, 1400);
   peak_ac = apmDiv(peak_ac, 10000);
   peak_ring = peak_ac + apmAbs( ringOffset );

   /* Load ring gain for this SLIC */
   ring_gain = ringGainTablep->ringGain;

   /* Calculate SLIC specifc voltage settings */
   switch( ringGainTablep->slicType )
   {
      case 0x32392:
      {
         peak_ring = apmDiv( (peak_ac * 106) , 100) + apmAbs( ringOffset ); /* Increase for ring gain tolerance */
         hvg_com_ring = 0;
         fsamp = 390625;                          /* Increased by factor of 4 */
         ringFS = divRound( 2400 * kDacAdjDiv, kDacAdjNum );                           /* Increased by factor of 1000 */
         vres = 75;                               /* Increased by factor of 2000 */
        /* ringFS = 2400;*/                           /* Increased by factor of 1000 */
         voh_min = 10000;                         /* Increased by factor of 1000 */
#if XDRV_CFG_BALANCED_RINGING
         vreg_ref_gain = 93;                      /* Increased by factor of 2 */
         vbat_min1 = apmDiv(peak_ring , 2) + apmDiv( (voh_min * 4) , 3);           /* Increased by factor of 1000 */
         vmax_hvg_slic = apmDiv( (( vbat_min1 - voh_min ) * 2 ) , vreg_ref_gain ); /* Does NOT include ringOffsetCal */
         max_hvg_slic = divRound(( vmax_hvg_slic * 100 * 4096 ), ( 330000 ));
#else
         vbat_min1 = 15000;
         /* vmax_hvg_slic = 0; */
         max_hvg_slic = 0;
#endif
         vbat_min = vbat_min1 + apmAbs( ringOffsetCal );   /* Increased to account for ringOffsetCal */      }
      break;

      case 0x9540:
      {
         peak_ring = peak_ac + apmAbs( ringOffset );
         hvg_com_ring = 0;
         fsamp = 390625;                      /* Increased by factor of 4 */
         ringFS = divRound( 2400 * kDacAdjDiv, kDacAdjNum );                           /* Increased by factor of 1000 */
         vres = 75;                           /* Increased by factor of 2000 */
         /*ringFS = 2400;*/                       /* Increased by factor of 1000 */
         voh_min = 4000;                      /* Increased by factor of 1000 */
         vbat_min = 8000;                     /* Increased by factor of 1000 */
         vbat_min1 = 8000;                    /* Increased by factor of 1000 */
         /* vreg_ref_gain = 0; */
         /* vmax_hvg_slic = 0; */
         max_hvg_slic = 0;
      }
      break;

      default:
      {
         XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "Unknown SLIC type in APM ring configuration."));
         return;
      }
      break;
   }

   /* Calculate ring coefficients */
   ring_freqh = divRound(( 1048576 * 16 * ringFrequency ), ( fsamp ));
   ring_amph = divRound(( 32768 * peak_ac ), ( ring_gain * apmAbs( ringFS )));
   ring_offh = divRound(( 32768 * ringOffset ), ( ring_gain * ringFS ));
   ring_offhcal = divRound(( 32768 * ringOffsetCal ), ( ring_gain * ringFS ));
   ring_offhtot = ring_offh + ring_offhcal;

   /* Determine HVG Gain */
   hvg_ring_gain = divRound((( peak_ring + voh_min - vbat_min1 ) * 65536 * 2 ), ( vres * ( ring_amph + abs( ring_offh ))));
   hvg_ring_ovhd = divRound( vbat_min * 2, vres );

   /* Determine ring start/stop phase */
   ring_amp = divRound(( 32768 * peak_ac ), ( ring_gain * apmAbs( ringFS )));
   ring_stoph = divRound(( 32768 * ( ring_stop - ringOffset )),
                         ( ring_gain * ringFS ));
   ring_gen_asin = apmAsin(apmDiv(1000*ring_stoph,ring_amp));
   ring_gen_phase = divRound( ( 2 * ring_gen_res * ring_gen_asin ), 3142 * RING_PHASE_FACTOR );

   /* Adjust ring phase to the 2nd or 3rd quadarant */
   ring_gen_phase = 2 * ring_gen_res - ring_gen_phase;

   /* Update appropriate registers */
   pApm->reg_ring_config_1 = ((( ring_freqh << RING_OMEGA_NORM_SHIFT ) & RING_OMEGA_NORM ) |
                              (( ring_delay << RING_DELAY_SHIFT ) & RING_DELAY ) |
                              (( 0 << RING_GEN_SOFT_INIT_SHIFT ) & RING_GEN_SOFT_INIT ) |
                              (( 0 << RING_START_IMMED_A_SHIFT ) & RING_START_IMMED_A ) |
                              (( 0 << RING_START_IMMED_B_SHIFT ) & RING_START_IMMED_B ) |
                              (( 0 << RING_START_NEXT_PHASE_A_SHIFT ) & RING_START_NEXT_PHASE_A ) |
                              (( 0 << RING_START_NEXT_PHASE_B_SHIFT ) & RING_START_NEXT_PHASE_B ) |
                              (( 0 << RING_STOP_IMMED_A_SHIFT ) & RING_STOP_IMMED_A ) |
                              (( 0 << RING_STOP_IMMED_B_SHIFT ) & RING_STOP_IMMED_B ) |
                              (( 0 << RING_STOP_NEXT_PHASE_A_SHIFT ) & RING_STOP_NEXT_PHASE_A ) |
                              (( 0 << RING_STOP_NEXT_PHASE_B_SHIFT ) & RING_STOP_NEXT_PHASE_B ));

   pApm->reg_ring_config_2 = ((( ring_offhtot << RING_OFFSET_SHIFT ) & RING_OFFSET ) |
                              (( ring_amph << RING_SCALE_SHIFT ) & RING_SCALE ));

   pApm->reg_ring_config_3 = ((( ring_offhtot << RING_OFFSET_SHIFT ) & RING_OFFSET ) |
                              (( ring_amph << RING_SCALE_SHIFT ) & RING_SCALE ));

   pApm->reg_ring_config_4 = ((( ring_gen_phase << RING_GEN_PHASE_A_SHIFT ) & RING_GEN_PHASE_A ) |
                              (( ring_gen_phase << RING_GEN_PHASE_B_SHIFT ) & RING_GEN_PHASE_B ) |
                              (( 1 << RING_GEN_ENABLE_A_SHIFT ) & RING_GEN_ENABLE_A ) |
                              (( 1 << RING_GEN_ENABLE_B_SHIFT ) & RING_GEN_ENABLE_B ) |
                              (( ring_crest << RING_CREST_FACTOR_A_SHIFT ) & RING_CREST_FACTOR_A ) |
                              (( ring_crest << RING_CREST_FACTOR_B_SHIFT ) & RING_CREST_FACTOR_B ));

   pHvg->reg_hvg_ring_setup = ((( hvg_ring_gain << RING_GAIN_SHIFT ) & RING_GAIN ) |
                         (( hvg_ring_ovhd << RING_OVHD_SHIFT ) & RING_OVHD ) |
                         (( hvg_com_ring << HVG_COMMON_RING_REF_SHIFT ) & HVG_COMMON_RING_REF ));

   pHvg->reg_hvg_cha_max_hvg_slic = max_hvg_slic;
   pHvg->reg_hvg_chb_max_hvg_slic = max_hvg_slic;
}


/*
*****************************************************************************
** FUNCTION:   apm6838GetRingParms
**
** PURPOSE:    Gets the APM ringing waveform parameters
**
** PARAMETERS: pDrv           - pointer to the APM driver device structure
**             ringFrequency  - ringing frequency
**             ringWaveshape  - ringing waveshape
**             ringVoltage    - ringing voltage
**             ringOffset     - ringing DC offset
**             ringOffsetCal  - ringing DC offset calibration
**
** RETURNS:    none
**
*****************************************************************************
*/
static void apm6838GetRingParms( XDRV_APM *pDrv, int *ringFrequency, int *ringWaveshape,
                                 int *ringVoltage, int *ringOffset, int *ringOffsetCal )
{
   APM_6838_DRV *pDev  = (APM_6838_DRV *)pDrv;

   /* Retrieve ring settings */
   *ringVoltage = pDev->ringVoltage;
   *ringOffset = pDev->ringOffset;
   *ringOffsetCal = pDev->ringOffsetCal;
   *ringFrequency = pDev->ringFrequency;
   *ringWaveshape = pDev->ringWaveshape;
}


/*
*****************************************************************************
** FUNCTION:   apm6838PulseSignal
**
** PURPOSE:    Generate Pulse Metering Signal
**
** PARAMETERS: pDrv       - pointer to the APM driver interface
**             endpt      - endpoint to control
**             duration   - duration of signal
**             period     - period of signal
**             repetition - number of repetitions of signal
**             amplitude  - amplitude of signal
**             frequency  - frequency of signal
**
** RETURNS:    none
**
*****************************************************************************
*/
static void apm6838PulseSignal( XDRV_APM *pDrv, int endpt, int duration, int period,
                                int repetition, int amplitude, int frequency )
{
   APM_6838_DRV *pDev  = (APM_6838_DRV *)pDrv;
   volatile BCM6838_APM *pApm = pDev->pApm;

   endpt = pDev->channelSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - endpt : endpt;

   if( endpt == 0 )
   {
      /* Enable PM block */
      pApm->reg_cha_pm_config_3 = PM_ENABLE;

      /* Set off time, on time, and frequency */
      pApm->reg_cha_pm_config_1 = ((((( period - duration ) * PM_TIME_MS ) << PM_OFF_TIME_SHIFT ) & PM_OFF_TIME ) |
                                   (( duration * PM_TIME_MS ) & PM_ON_TIME ));

      if( frequency == 16000 )
      {
         pApm->reg_cha_pm_config_1 |= PM_FREQ_16_12;
      }

      /* Set amplitude */
      pApm->reg_cha_pm_config_2 = PM_FALL_INC | PM_RISE_INC | ( 0x01FF & PM_SCALE );

      /* Start signal and specify number of repetitions */
      pApm->reg_cha_pm_config_3 = PM_ENABLE | PM_BURST_START | ( repetition & PM_BURST_LENGTH );
   }
   else
   {
      /* Enable PM block */
      pApm->reg_chb_pm_config_3 = PM_ENABLE;

      /* Set off time, on time, and frequency */
      pApm->reg_chb_pm_config_1 = ((((( period - duration ) * PM_TIME_MS ) << PM_OFF_TIME_SHIFT ) & PM_OFF_TIME ) |
                                   (( duration * PM_TIME_MS ) & PM_ON_TIME ));

      if( frequency == 16000 )
      {
         pApm->reg_chb_pm_config_1 |= PM_FREQ_16_12;
      }

      /* Set amplitude */
      pApm->reg_chb_pm_config_2 = PM_FALL_INC | PM_RISE_INC | ( 0x01FF & PM_SCALE );

      /* Start signal and specify number of repetitions */
      pApm->reg_chb_pm_config_3 = PM_ENABLE | PM_BURST_START | ( repetition & PM_BURST_LENGTH );
   }

   return;
}

/*
*****************************************************************************
** FUNCTION:   apm6838SetHPF
**
** PURPOSE:    This function set HPF in APM module
**
** PARAMETERS: pDrv  - pointer to the APM driver interface
**             endpt  - endpoint to control
**             enable - enable or disable HPF
**
** RETURNS:    none
**
**  NOTES:
**
*****************************************************************************
*/
static void apm6838SetHPF( XDRV_APM *pDrv, int endpt, XDRV_BOOL enable )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pDrv;
   volatile BCM6838_APM *pApm = pDev->pApm;

   endpt = pDev->channelSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - endpt : endpt;

   if( endpt == 0 )
   {
      if ( enable )
      {
         pApm->reg_cha_hybal_config |= HPF_EN;
      }
      else
      {
         pApm->reg_cha_hybal_config &= ~HPF_EN;
      }
   }
   else
   {
      if ( enable )
      {
         pApm->reg_chb_hybal_config |= HPF_EN;
      }
      else
      {
         pApm->reg_chb_hybal_config &= ~HPF_EN;
      }
   }
}

/*
*****************************************************************************
** FUNCTION:   apm3380SetClkSource
**
** PURPOSE:    This function set frequency source for APM module
**
** PARAMETERS: pDrv  - pointer to the APM driver interface
**             clkSource - APM clock source
**
** RETURNS:    none
**
**  NOTES:
**
*****************************************************************************
*/
void apm6838SetClkSource( XDRV_APM *pDrv, int clkSource )
{
}

#if 0
/*
*****************************************************************************
** FUNCTION:   FlexiCalcRegWrite
**
** PURPOSE:    Write FlexiCalc parameters into APM registers.
**
** PARAMETERS: regp  - pointer to APM register to write
**             value - value to be written
**             mask  - bitmask for the bitfields to be written
**             shift - shift for the bitfields to be written
**
** RETURNS:    0 on success
**
** NOTES:      This overwrites the default APM register settings, so care
**             must be taken to leave all register bitfields that
**             are not FlexiCalc businesses intact.
*****************************************************************************
*/
static int FlexiCalcRegWrite( volatile XDRV_UINT32 *regp, unsigned int value, unsigned int mask, unsigned int shift )
{
   /* Sanity check - value must not exceed valid bitmask */
   if ((value<<shift) & (~mask))
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_APM: addr 0x%lx, val 0x%.8x, mask 0x%.8x, shift %d !!!", (XDRV_UINT32)regp, value, mask, shift));
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - FlexiCalc parameter out of range !!!"));
      return ( 1 );
   }

   /* Clear existing value */
   *regp &= ~mask;

   /* Update with new value */
   *regp |= (value<<shift);

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "&=0x%lx, val=0x%.8x, mask 0x%.8x, shift %d, *&=0x%.8x", (XDRV_UINT32)regp, value, mask, shift, (unsigned int)*regp));

   return ( 0 );
}
#endif


/*
*****************************************************************************
** FUNCTION:   WriteApmFilterCoeffs
**
** PURPOSE:    This function will write coefficients to the specified filter block
**             for the specified channel. Additionally the filter block will be
**             configured to use the programmed values instead of the default ROM values.
**
** PARAMETERS: pApm   - pointer to 6838's APM module
**             chan   - channel numbers (bitwise)
**             filter - APM filter to program coefficients for
**             data   - coefficient data
**             length - number of coefficients
**
** RETURNS:    0 on success
**
** NOTES:
*****************************************************************************
*/
static int WriteApmFilterCoeffs( volatile BCM6838_APM *pApm, APM_6838_DRV *pDrv, int chan, APM6838_FILTER_TYPE filter, int *data, int length )
{
   int coeffRamType;
   int i;
   XDRV_SINT16 dataTemp16;
   XDRV_UINT32 dataTemp32;
   XDRV_SINT64 dataTemp64;
   XDRV_SINT64 kFactorAdjNum;
   XDRV_SINT64 kFactorAdjDiv;
   XDRV_SINT64 kDacAdjNum;
   XDRV_SINT64 kDacAdjDiv;

   if (( chan < 0 || chan > APM6838_NUM_CHANNELS ) && ( chan != APM6838_ALL_CHANNELS ))
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - invalid ch id (%d) to program coefficients !!!", chan));
      return -1;
   }

   if ( data == NULL )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - no datap to program filter (%d) coefficients !!!", filter));
      return -1;
   }

   /* Retrieve adjustment values */
   apm6838CalculateKadjValues( &kFactorAdjNum, &kFactorAdjDiv, &kDacAdjNum, &kDacAdjDiv );

   if( chan != APM6838_ALL_CHANNELS )
   {
      chan = pDrv->channelSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;
   }

   switch (filter)
   {
      case APM6838_FILTER_TX_EQ:
      {
         if( chan == APM6838_ALL_CHANNELS )
         {
            coeffRamType = EQ_TX_ACCESS_COEFF_RAM_A | EQ_TX_ACCESS_COEFF_RAM_B;
         }
         else if( chan == 0 )
         {
            coeffRamType = EQ_TX_ACCESS_COEFF_RAM_A;
         }
         else
         {
            coeffRamType = EQ_TX_ACCESS_COEFF_RAM_B;
         }
      }
      break;

      case APM6838_FILTER_RX_EQ:
      {
         if( chan == APM6838_ALL_CHANNELS )
         {
            coeffRamType = EQ_RX_ACCESS_COEFF_RAM_A | EQ_RX_ACCESS_COEFF_RAM_B;
         }
         else if( chan == 0 )
         {
            coeffRamType = EQ_RX_ACCESS_COEFF_RAM_A;
         }
         else
         {
            coeffRamType = EQ_RX_ACCESS_COEFF_RAM_B;
         }
      }
      break;

      case APM6838_FILTER_HISPD_HYBRID_BAL:
      {
         if( chan == APM6838_ALL_CHANNELS )
         {
            coeffRamType = HISPD_HYBAL_ACCESS_COEFF_RAM_A | HISPD_HYBAL_ACCESS_COEFF_RAM_B;
         }
         else if( chan == 0 )
         {
            coeffRamType = HISPD_HYBAL_ACCESS_COEFF_RAM_A;
         }
         else
         {
            coeffRamType = HISPD_HYBAL_ACCESS_COEFF_RAM_B;
         }
      }
      break;

      case APM6838_FILTER_LOSPD_HYBRID_BAL:
      {
         if( chan == APM6838_ALL_CHANNELS )
         {
            coeffRamType = LOSPD_HYBAL_ACCESS_COEFF_RAM_A | LOSPD_HYBAL_ACCESS_COEFF_RAM_B;
         }
         else if( chan == 0 )
         {
            coeffRamType = LOSPD_HYBAL_ACCESS_COEFF_RAM_A;
         }
         else
         {
            coeffRamType = LOSPD_HYBAL_ACCESS_COEFF_RAM_B;
         }
      }
      break;

      case APM6838_FILTER_Y_FILTER:
      {
         if( chan == APM6838_ALL_CHANNELS )
         {
            coeffRamType = YFLTR_ACCESS_COEFF_RAM_A | YFLTR_ACCESS_COEFF_RAM_B;
         }
         else if( chan == 0 )
         {
            coeffRamType = YFLTR_ACCESS_COEFF_RAM_A;
         }
         else
         {
            coeffRamType = YFLTR_ACCESS_COEFF_RAM_B;
         }
      }
      break;

      case APM6838_FILTER_ASRC_INT:
      {
         if( chan == APM6838_ALL_CHANNELS )
         {
            coeffRamType = ASRC_INT_ACCESS_COEFF_RAM_A | ASRC_INT_ACCESS_COEFF_RAM_B;
         }
         else if( chan == 0 )
         {
            coeffRamType = ASRC_INT_ACCESS_COEFF_RAM_A;
         }
         else
         {
            coeffRamType = ASRC_INT_ACCESS_COEFF_RAM_B;
         }
      }
      break;

      case APM6838_FILTER_ASRC_DEC:
      {
         if( chan == APM6838_ALL_CHANNELS )
         {
            coeffRamType = ASRC_DEC_ACCESS_COEFF_RAM_A | ASRC_DEC_ACCESS_COEFF_RAM_B;
         }
         else if( chan == 0 )
         {
            coeffRamType = ASRC_DEC_ACCESS_COEFF_RAM_A;
         }
         else
         {
            coeffRamType = ASRC_DEC_ACCESS_COEFF_RAM_B;
         }
      }
      break;

      default:
      {
         XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - invalid filter type (%d) to program coefficients !!!", filter));
         return -1;
      }
   }

   /* Initialize hybal shift adjustment values */
   if(( filter == APM6838_FILTER_HISPD_HYBRID_BAL ) || ( filter == APM6838_FILTER_LOSPD_HYBRID_BAL ))
   {
      pDrv->hybalShiftAdj = 0;
   }

   /* Initialize RAM programming control register */
   pApm->reg_apm_coeff_config = 0;

   for ( i = 0; i < length; i++ )
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Loading coeff type 0x%x: %d = 0x%x ...", coeffRamType, i, *( data + i )));

      switch (filter)
      {
         case APM6838_FILTER_Y_FILTER:
         {
            if( i == APM_YFLTR_GAIN_INDEX )
            {
               dataTemp32 = *( data + i );
               dataTemp32 = divRound( dataTemp32 * kFactorAdjNum * kDacAdjNum, kFactorAdjDiv * kDacAdjDiv );
               pApm->reg_apm_coeff_wr_data = dataTemp32;
            }
            else
            {
      pApm->reg_apm_coeff_wr_data = *( data + i );
            }
         }
         break;

         case APM6838_FILTER_HISPD_HYBRID_BAL:
         case APM6838_FILTER_LOSPD_HYBRID_BAL:
         {
            dataTemp16 = *( data + i );
            dataTemp64 = divRound( dataTemp16 * kFactorAdjDiv * kDacAdjDiv, kFactorAdjNum * kDacAdjNum );
            if(( dataTemp64 > 32767 ) || ( dataTemp64 < -32768 ))
            {
               dataTemp64 = divRound( dataTemp16 * kFactorAdjDiv * kDacAdjDiv, kFactorAdjNum * kDacAdjNum * 2 );
               pDrv->hybalShiftAdj++;
            }
            pApm->reg_apm_coeff_wr_data = dataTemp64 & 0xFFFF;
         }
         break;

         default:
         {
            pApm->reg_apm_coeff_wr_data = *( data + i );
         }
      }

      pApm->reg_apm_coeff_config = ( i & COEFF_PROG_ADDR ) | coeffRamType;
      pApm->reg_apm_coeff_config = ( i & COEFF_PROG_ADDR ) | coeffRamType | COEFF_PROG_WE;
      pApm->reg_apm_coeff_config = ( i & COEFF_PROG_ADDR ) | coeffRamType;
   }

   pApm->reg_apm_coeff_config = 0;

   return 0;
}


/*
*****************************************************************************
** FUNCTION:   divRound
**
** PURPOSE:    This function will divide two numbers and round the result
**             to the nearest whole number
**
** PARAMETERS: dividend - dividend of the operation
**             divisor - divisor of the operation
**
** RETURNS:    rounded result of the divide operation
**
** NOTES:
*****************************************************************************
*/
static XDRV_SINT64 divRound( XDRV_SINT64 dividend, XDRV_SINT64 divisor )
{
   XDRV_SINT64 result;

   if( divisor == 0 )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "ERROR: APM6838 - Divisor of zero entered!"));
      return( 1 );
   }

   /* Calculate result */
   result = apmDiv( dividend , divisor ) + apmDiv((apmRem( dividend , divisor ) * 2 ) , divisor );

   return( result );
}

/*
*****************************************************************************
** FUNCTION:   apm6838CalculateKadjValues
**
** PURPOSE:    This function will return the K adjustment factors
**
** PARAMETERS: numerator - numerator of the operation
**             divisor - divisor of the operation
**             dacnum - adjusted DAC numerator
**             dacdiv - adjusted DAC divisor
**
** RETURNS:    none
**
** NOTES:
*****************************************************************************
*/
void apm6838CalculateKadjValues( XDRV_SINT64 *numerator, XDRV_SINT64 *divisor, XDRV_SINT64 *dacnum, XDRV_SINT64 *dacdiv )
{
     if ( OTP_GET_MFG_ID() == MFG_ID_UMC )
     {
         /* Adjust the gain based on JIRA SWBCACPEVOIP-8706 */
         *numerator = 115;
         *divisor = 117;
         *dacnum = 81;
         *dacdiv = 82;
     }
     else
     {
         *numerator = 1;
         *divisor = 1;
         *dacnum = 1;
         *dacdiv = 1;
     }
}

XDRV_SINT64 apmDiv(XDRV_SINT64 dividend, XDRV_SINT64 divisor)
{
#ifdef USE_KERNEL_LIB
   XDRV_UINT64 __dividend, __divisor;
   int sign = 0;
   if ((dividend) < 0)
   {
      sign = ~sign;
      __dividend = -(dividend);
   }
   else
   {
      __dividend = (dividend);
   }

   if ((divisor) < 0)
   {
      sign = ~sign;
      __divisor = -(divisor);
   }
   else
   {
      __divisor = (divisor);
   }

   XDRV_SINT64 quotient = div64_u64(__dividend, __divisor);
   if (sign)
   {
      quotient = -quotient;
   }

   return (quotient);
#else
   return ((dividend) / (divisor));
#endif
}

XDRV_SINT64 apmRem(XDRV_SINT64 dividend, XDRV_SINT64 divisor)
{
#ifdef USE_KERNEL_LIB
   XDRV_SINT64 quotient = apmDiv((dividend), (divisor));
   XDRV_SINT64 remainder = (dividend) - ((divisor) * quotient);

   return (remainder);
#else
   return ((dividend) % (divisor));
#endif
}

XDRV_SINT64 apmAbs(XDRV_SINT64 value)
{
#ifdef USE_KERNEL_LIB
   return ( (value < 0) ? -(value) : value );
#else
   return (apmAbs(value));
#endif
}

static XDRV_SINT64 apmAsin(XDRV_SINT32 value)
{
   int cnt=0;
   /* need confirm with cable for this exception*/
   if( value<apmAsinTable[0].value || value> apmAsinTable[sizeof(apmAsinTable)/sizeof(APM6838_ASIN)-1].value)
   {
     return 0;
   }
   while(value>apmAsinTable[cnt].value)
   {
     cnt++;
   }
   return (XDRV_SINT64)apmAsinTable[cnt].angle;
}


#if APM_POWER_WORKAROUND
/* APM-specific power operations */
static int PowerOnDevice(int devAddr)
{
   int ix, status;
   BPCM_CAPABILITES_REG capabilities;

   status = ReadBPCMRegister(devAddr, BPCMOffset(capabilities), &capabilities.Reg32);
   for(ix = 0; (ix < capabilities.Bits.num_zones) && (status != kPMC_NO_ERROR); ix++)
   {
      BPCM_PWR_ZONE_N_CONTROL ctl;
      status = ReadBPCMRegister(devAddr, BPCMOffset(zones[ix].control), &ctl.Reg32);
      if(status == kPMC_NO_ERROR && ctl.Bits.pwr_on_state == 0)
      {
         ctl.Bits.mem_pwr_ctl_en = 1;
         ctl.Bits.blk_reset_assert = 1;
         ctl.Bits.dpg_ctl_en = 1;
         ctl.Bits.pwr_up_req = 1;
         WriteBPCMRegister(devAddr, BPCMOffset(zones[ix].control), ctl.Reg32);
      }
   }

   return status;
}

static int PowerOffDevice(int devAddr, int repower)
{
   // we can power off the entire device by powering off the 0th zone.
   BPCM_PWR_ZONE_N_CONTROL reg;
   int status;

   status = ReadBPCMRegister(devAddr, BPCMOffset(zones[0].control), &reg.Reg32);

   if(status == kPMC_NO_ERROR && reg.Bits.pwr_off_state == 0)
   {
      reg.Bits.pwr_dn_req = 1;
      WriteBPCMRegister(devAddr, BPCMOffset(zones[0].control), reg.Reg32);
   }

   return status;
}

static int PowerOnZone(int devAddr, int zone)
{
   BPCM_PWR_ZONE_N_CONTROL reg;
   int status;

   status = ReadBPCMRegister(devAddr, BPCMOffset(zones[zone].control), &reg.Reg32);

   if(status == kPMC_NO_ERROR)
   {
      reg.Bits.pwr_up_req = 1;
      status = WriteBPCMRegister(devAddr, BPCMOffset(zones[zone].control), reg.Reg32);
   }

   return status;
}

static int PowerOffZone(int devAddr, int zone)
{
   BPCM_PWR_ZONE_N_CONTROL reg;
   int status;

   status = ReadBPCMRegister(devAddr, BPCMOffset(zones[zone].control), &reg.Reg32);

   if(status == kPMC_NO_ERROR)
   {
      reg.Bits.pwr_dn_req = 1;
      status = WriteBPCMRegister(devAddr, BPCMOffset(zones[zone].control), reg.Reg32);
   }

   return status;
}

static int ResetDevice(int devAddr)
{
   // all zones had their blk_reset_assert bits set at initial config time
   BPCM_PWR_ZONE_N_CONTROL reg;
   int status;

   status = PowerOffDevice(devAddr, 0);

   do
   {
      status = ReadBPCMRegister(devAddr, BPCMOffset(zones[0].control), &reg.Reg32);
   }
   while((reg.Bits.pwr_off_state != 1) && (status != kPMC_NO_ERROR));

   if(status == kPMC_NO_ERROR)
      status = ApmPowerOnDevice(devAddr);

   return status;
}

static int ResetZone(int devAddr, int zone)
{
   BPCM_PWR_ZONE_N_CONTROL reg;
   int status;

   status = PowerOffZone(devAddr, zone);

   do
   {
      status = ReadBPCMRegister(devAddr, BPCMOffset(zones[zone].control), &reg.Reg32);
   }
   while((reg.Bits.pwr_off_state != 1) && (status != kPMC_NO_ERROR));

   if(status == kPMC_NO_ERROR)
      status = PowerOnZone(devAddr, zone);

   return status;
}
#endif /* APM_POWER_WORKAROUND */