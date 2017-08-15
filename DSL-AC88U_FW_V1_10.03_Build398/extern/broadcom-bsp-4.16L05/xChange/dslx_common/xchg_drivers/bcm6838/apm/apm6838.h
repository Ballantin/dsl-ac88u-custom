/****************************************************************************
*    Copyright © 2008 Broadcom
*
*    This program is the proprietary software of Broadcom and/or
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
*****************************************************************************
*
*  apm6838.h - BCM6838 APM Definitions
*
*  PURPOSE:
*
*  NOTES:
*
****************************************************************************/

#ifndef APM6838_H
#define APM6838_H

#if defined(__cplusplus)
  extern "C" {
#endif

/* ---- Include Files ----------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvApm.h>
#include <bcm6838.h>    /*TODO: This I need only for PerfControl, remove in the future */
#include <mipsDefines.h>
#include "dma6838.h"
#include <apm.h>

/* ---- Public Constants and Types ---------------------------------------- */

#define APM_CH_MIN          0
#define APM_CH_MAX          1
#define APM_CH_NUM          (APM_CH_MAX - APM_CH_MIN + 1)

/* ---- Constants and Types ---------------------------------------------- */

/*
** This likely doesn't belong here. It should go somewhere more generic, but
** I'm not sure where at the moment...
*/
#define ALIGN_DATA(addr, boundary) ((addr + boundary - 1) & ~(boundary - 1))


#define APM6838_NUM_CHANNELS     2  /* Number of channels supported by this device */
#define CHANNEL_0_ONLY           0  /* APM Ch0 Rx */
#define CHANNEL_1_ONLY           0  /* APM Ch0 Tx */

/*
** APM coefficient filter type
**
** The enumeration is used to describe which APM filter to program
*/
typedef enum
{
   APM6838_FILTER_TX_EQ = 0,    /* Transmit Equalizer coefficients */
   APM6838_FILTER_RX_EQ,        /* Receive Equalizer coefficients */
   APM6838_FILTER_HISPD_HYBRID_BAL,   /* High Speed Hybrid Balance coefficients */
   APM6838_FILTER_LOSPD_HYBRID_BAL,   /* Low Speed Hybrid Balance coefficients */
   APM6838_FILTER_Y_FILTER,     /* Y-filter coefficients */
   APM6838_FILTER_ASRC_INT,     /* ASRC Interpolator coefficients */
   APM6838_FILTER_ASRC_DEC      /* ASRC Decimator coefficients */
} APM6838_FILTER_TYPE;


/* Typedef to local shared types */
typedef ApmControlRegisters    BCM6838_APM;
typedef ApmPcmIudma            BCM6838_IUDMA;
typedef ApmPcmIudmaRegisters   BCM6838_IUDMA_REGS; 
typedef ApmPcmIudmaChannelCtrl BCM6838_IUDMA_CTRL; 
typedef ApmPcmIudmaStateRam    BCM6838_IUDMA_STRAM;

/* Register block pointers */
#define BCM6838_APM_PTR               ((volatile BCM6838_APM *)APM_BASE)
#define BCM6838_APM_IUDMA_PTR         ((volatile BCM6838_IUDMA *)APM_PCM_DMA_BASE)


#define DMA_RX_NUM_BUFFERS    2     /* Only need two buffers for double buffering */
#define DMA_TX_NUM_BUFFERS    2     /* Only need two buffers for double buffering */

#define APM6838_TX_DMA_BUF_DEPTH_WORDS BCM_APM_TX_DMA_BUF_DEPTH_WORDS /* This is in 32-bit words. This is the depth of the TX pipeline on the APM IUDMA side (16 X 64bit words)   */   

#define APM6838_TX_DMA_BUF_DEPTH_BYTES    (APM6838_TX_DMA_BUF_DEPTH_WORDS *sizeof(XDRV_UINT32))      

#define APM6838_DMA_MAX_BUFFER_SIZE ( 48 + APM6838_TX_DMA_BUF_DEPTH_WORDS )  /* This is in 32-bit words.
                                                                       ** Allocate enaough for (5ms + 1ms prime) worth of 16kHz data:
                                                                       ** 1ms=16*16-bit=8 32-bit words
                                                                       ** 6ms=6*8*32-bit=48 32bit words
                                                                       ** then we add to this the amount required to fill the TX pipeline */


#define APM6838_DMA_MAX_BUFFER_SIZE_BYTES   (APM6838_DMA_MAX_BUFFER_SIZE*sizeof(XDRV_UINT32))

/* Data buffers for DMA descriptor rings. The start address of the rx buffer
 * must be 32-bit aligned */
typedef struct APM6838_DMA_BUFFER
{
   /* Add padding so that we can align the DMA buffer on a cache-line boundary. */
   #define DMA_PADDING_WORD32    ( DCACHE_LINE_SIZE_BYTES / 4 )
   volatile XDRV_UINT32 dmaBuff[ APM6838_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32 ];
} APM6838_DMA_BUFFER;


/*
** APM Flexicalc configuration structure as generated by FlexiCalc tool
*/
typedef struct APM6838_FLEXICALC_CFG
{
   unsigned int slic_type;
   unsigned int ring_voltage;
   signed int dBdlp;
   signed int dBelp;
   unsigned int eq_rx_shft;
   unsigned int eq_tx_shft;
   unsigned int eq_imp_resp;
   unsigned int yfltr_en;
   unsigned int yfltr_iir2[APM_YFLTR_IIR2_NUM_TAPS];
   unsigned int yfltr_iir2_shift;
   unsigned int yfltr_fir[APM_YFLTR_FIR_NUM_TAPS];
   unsigned int yfltr_fir_shift;
   unsigned int yfltr_gain;
   unsigned int yfltr_iir1[APM_YFLTR_IIR1_NUM_TAPS];
   unsigned int yfltr_iir1_shift;
   unsigned int hybal_shft;
   unsigned int hybal_audio_fir[APM_HYBAL_NUM_TAPS];
   unsigned int hybal_pm_fir[APM_HYBAL_NUM_TAPS];
   unsigned int hybal_en;
   unsigned int eq_rx_filter[APM_RX_EQ_NUM_TAPS];
   unsigned int eq_tx_filter[APM_TX_EQ_NUM_TAPS];
   unsigned int cic_inc_shft;
   unsigned int cic_dec_shft;
   unsigned int asrc_int_scale;
   unsigned int asrc_dec_scale;
   unsigned int vtx_pg;
   unsigned int vrx_pg;
   unsigned int hpf_en;
   unsigned int hybal_smpl_offset;

} APM6838_FLEXICALC_CFG;


/* APM PM configuration structure */
typedef struct APM6838_PM_SETTINGS
{
   XDRV_UINT16 pmBurstCont;    /* PM burst type */

} APM6838_PM_SETTINGS;

/*
** 6838 APM configuration structure
*/
typedef struct APM6838_CFG
{
   APM6838_PM_SETTINGS          pmCfg;        /* Pulse metering configuration */
   const APM6838_FLEXICALC_CFG *pFlexiCalc;   /* FlexiCalc configuration */
   int                          sampleRate;   /* Sampling rate in kHz (8 or 16) */
   int                          packetRate;   /* The rate (in ms) at which APM generates packets/interrupts */
   int                          useRingGen;   /* Controls APM ring generator usage */
   int                          useJapDocsisClock; /* Sets US 10.24MHz Docsis clock or Japan 9.216MHz clock */
   int                          hvgType;      /* HVG design topology */
   int                          sharedReferencePin; /* Shared HVG refernce pin */
   int                          inputSupplyVoltage; /* Input power supply voltage */

} APM6838_CFG;

/*
** 6838 APM driver structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'XDRV_APM' base-class(structure).
** The base APM structure specifies a set of function pointers that defines
** the generic hardware-independent API that must be provided by all chip
** specific APM drivers.
**
** When the 6838 APM driver initialization function is invoked, the base API
** structure function pointers will be intialized to a set of hardware specific
** APM driver functions. Therefore, given a pointer to the base APM driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific APM driver function.
**
** APM driver state that is specific to the 6838 should be stored in the
** 'APM_6838_DRV' structure. A pointer to 'XDRV_APM_FUNCS' must be the first
** member of the 'APM_6838_DRV' struct.
*/
typedef struct
{
   /* Must be first element of this struct. */
   const XDRV_APM_FUNCS          *pDrvFuncs;

   /* APM 6838-specific state info. */
   volatile BCM6838_APM          *pApm;    /* pointer to 6838's APM module */
   volatile BCM6838_IUDMA        *pIuDma;  /* pointer to 6838's APM IUDMA module */
   volatile PerfControl          *pIntCtl; /* pointer to 6838's Chip Control module */
   APM6838_CFG                   Cfg;      /* APM configuration structure */
   int                           dcOffset[APM6838_NUM_CHANNELS];
   int                           battery[APM6838_NUM_CHANNELS];   
   int                           loopCurrent[APM6838_NUM_CHANNELS];                 
   int                           hookState[APM6838_NUM_CHANNELS]; 
   int                           slicState[APM6838_NUM_CHANNELS];                 
   int                           highVoltage[APM6838_NUM_CHANNELS];                 
   int                           overVoltage[APM6838_NUM_CHANNELS];                 
   int                           hookStateUpdatesEnabled[APM6838_NUM_CHANNELS];                 
   int                           ringStop[APM6838_NUM_CHANNELS]; 
   int                           ringVoltage;           /* Current Ring Voltage */
   int                           ringOffset;            /* Current Ring DC Offset */
   int                           ringOffsetCal;         /* Current Ring DC Offset Calibration */
   int                           ringFrequency;         /* Current Ring Frequency */
   int                           ringWaveshape;         /* Current Ring Waveshape */
   int                           vDac[APM6838_NUM_CHANNELS];
   int                           channelSwap;           /* for those APM and SLIC channel need swap board */                 
   int                           hybalShiftAdj;

} APM_6838_DRV;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

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
);

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
int apm6838_deinit( APM_6838_DRV *pDrv );


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
int apm6838_start( APM_6838_DRV  *pDrv );

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
int apm6838_stop( APM_6838_DRV *pDrv );

/*
*****************************************************************************
** FUNCTION:   apm6838SetClkSource
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
void apm6838SetClkSource( XDRV_APM *pDrv, int clkSource );

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
int apm6838_flexicalcConfig(volatile BCM6838_APM *pApm,const APM6838_FLEXICALC_CFG *flexicalcp,APM_6838_DRV *pDrv);

XDRV_SINT64 apmDiv(XDRV_SINT64 dividend, XDRV_SINT64 divisor);
XDRV_SINT64 apmRem(XDRV_SINT64 dividend, XDRV_SINT64 divisor);
XDRV_SINT64 apmAbs(XDRV_SINT64 value);

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
void apm6838CalculateKadjValues
(
   XDRV_SINT64 *numerator,
   XDRV_SINT64 *divisor,
   XDRV_SINT64 *dacnum,
   XDRV_SINT64 *dacdiv
);
#if defined(__cplusplus)
}
#endif

#endif /* APM6838_H */
