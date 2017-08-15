/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom Corporation
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
*    Filename: boardHalInitCommon.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent initialization interface.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <boardparms_voice.h>
#include <vrgLog.h>
#include <vrgTypes.h>
#include <boardHalInit.h>
#include <vrgCountry.h>
#include <boardHalSlic.h>
#include <boardHalRelay.h>
#include <boardHalApm.h>
#include <boardHalCas.h>
#include <boardHalIpc.h>
#include <boardHalTpd.h>
#include <boardHalMspi.h>
#include <boardHalDspTask.h>
#include <boardHalDaa.h>
#include <boardHalPower.h>
#include <boardHalProv.h>
#include <boardHalDect.h>
#include <boardHal6xxx.h>
#include <bcmChip.h>

#ifdef BRCM_6816
#include <hvg6816.h>
#endif

#ifdef BRCM_6828
#include <hvg6828.h>
#endif

#ifdef BRCM_6818
#include <hvg6818.h>
#include <boardHal6818.h>
#endif

#ifdef BRCM_6838
#include <hvg6838.h>
#include <boardHal6838.h>
#endif

#include <debugCmtInterface.h>
#include <xdrvItpc.h>
#include <intCtrlBcm63xx.h>
#include <exceptionHdlrCmtMips.h>
#include <xdrvSpinLock.h>
#include <bosSleep.h>
#include <bosMutex.h>
#include <pcmChip.h>
#include <xchgAssert.h>
#include <boardHalSpiClk.h>
#include <boardHalGpio.h>
#include <xdrvCache.h>    /* For xdrvSetTP1DCache */

#if USE_DLB_STUB
#include <dlbStub.h>
#else 
#include <dlbChip.h>
#endif /* USE_DLB_STUB */



/* ---- Defines ---------------------------------------------------------- */
#define BCM_PCM_CHAN_MASK (     PCM_TX0_EN | PCM_RX0_EN | \
                                PCM_TX1_EN | PCM_RX1_EN | \
                                PCM_TX2_EN | PCM_RX2_EN | \
                                PCM_TX3_EN | PCM_RX3_EN | \
                                PCM_TX4_EN | PCM_RX4_EN | \
                                PCM_TX5_EN | PCM_RX5_EN | \
                                PCM_TX6_EN | PCM_RX6_EN | \
                                PCM_TX7_EN | PCM_RX7_EN )


/* ---- Public Variables ------------------------------------------------- */

extern XDRV_DEBUG gDebugCmtDrv;

/*
** Shared memory locations used to pass commands between thread processors.
*/
static XDRV_ITPC_CMD          *gHalItpcSyncRecvCmd;
static XDRV_ITPC_CMD          *gHalItpcSyncSendCmd;
static XDRV_ITPC_STATUS_REG   *gHalItpcRecvStatusReg;
static XDRV_ITPC_STATUS_REG   *gHalItpcSendStatusReg;
XDRV_ITPC_SHARED_MEM_DATA     *gHalItpcSharedMemData;

LHAPI_HALSHIM_CONFIG          *gHalshimConfig;

XDRV_UINT32                   gHalshimEntryCount;                   

/* Mutex used to serialize synchronous ITPC command requests. */
static BOS_MUTEX              gHalItpcBosMutex;
static XDRV_ITPC_MUTEX        gHalItpcMutex;

/* ITPC instance memory. */
XDRV_ITPC                     gHalItpcDrv;

/* Spin lock synchronization primitive used to prevent concurrent access
** to shared memory data structures by both MIPS cores. */
XDRV_SPIN_LOCK                *gHalSpinLock;

/* Dynamic load balancing driver. */
#if USE_DLB_STUB
DLB_STUB_DRV                  gHalDlbDrv;
#else 
DLB_DRV                       gHalDlbDrv;
#endif /* !USE_DLB_STUB */

XDRV_ITPC_DSP_STACK_DUMP     *gHalDspStackDump;
static XDRV_ITPC_DSP_STACK_PTR      *gHalDspStackPtr;

/* ---- Private Constants and Types -------------------------------------- */

static int mspiInUse = 0;
static LHAPI_HALSHIM_CONFIG halShimConfig[BP_MAX_VOICE_CHAN];

/* ---- Private Function Prototypes -------------------------------------- */

static int CmtExceptionCallback( EXCEPTION_HDLR_CMT_CALLBACK_PARM );
static unsigned int ItpcMutexAcquire( XDRV_ITPC_MUTEX_PARM parm );
static unsigned int ItpcMutexRelease( XDRV_ITPC_MUTEX_PARM parm );
static int bhiPopulateHalshimConfig( LHAPI_HALSHIM_CONFIG* halShimConfig );
static void bhiPrepareChannelTimeslotMap( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap );


/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalInitUni
**
** PURPOSE:    Initialize the HAL.
**
** PARAMETERS: boardHalInitCfg   - board hal config structure
**             initModules       - modules to be initialized
**
** RETURNS:    0 on success, otherwise error code.
**
*****************************************************************************
*/
int boardHalInitUni( VRG_BOARDHAL_INIT_CFG  *boardHalInitCfg, int initModules )
{
   BOARD_HAL_DSP_CFG dspCfg;
   VRG_BOOL           bUseHvg;
   VOICE_BOARD_PARMS* voiceParams;
   PCM_CHANNEL_TIMESLOT_MAP chanTsMap[PCM_MAX_CHANNELS];
   XDRV_UINT8 supportedPCMPllMode = MODE_REG;
   VRG_BOOL           bFastSlicSupported;
   VRG_BOOL           bCasHistorySupported = VRG_FALSE;

   static VRG_BOOL firstTime = VRG_TRUE;

   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing board HAL..."));

   /* Retrieve Voice Params */
   voiceParams = boardHalProvGetVoiceParms();

   /* 
   ** Initialize the GPIO module
   */
   if ( initModules & BOARDCFG_MODULE_GPIO )
   {
      if ( boardHalGpioInit() )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_BOARDHAL,  "ERROR: GPIO init FAILED" ));
         return ( -1 );
      }
      else
      {
         VRG_LOG_INFO((VRG_LOG_MOD_BOARDHAL,  "GPIO INITIALIZED" ));
      }
   }
   /* 
   ** HACK: Only allocate memory once! 
   **       (see boardHalDeinit: it should not be freed for reset purposes)
   */
   if (firstTime)
   {
      /* Allocate memory for the shared IPC in KSeg0 */
      gHalItpcSyncRecvCmd = malloc( sizeof(XDRV_ITPC_CMD) );
      if ( gHalItpcSyncRecvCmd == NULL ) return (-1 );

      gHalItpcSyncSendCmd = malloc( sizeof(XDRV_ITPC_CMD) );
      if ( gHalItpcSyncSendCmd == NULL ) return (-1 );

      gHalItpcRecvStatusReg = malloc( sizeof(XDRV_ITPC_STATUS_REG) );
      if ( gHalItpcRecvStatusReg == NULL ) return (-1 );

      gHalItpcSendStatusReg = malloc( sizeof(XDRV_ITPC_STATUS_REG) );
      if ( gHalItpcSendStatusReg == NULL ) return (-1 );

      gHalItpcSharedMemData = malloc( sizeof(XDRV_ITPC_SHARED_MEM_DATA) );
      if ( gHalItpcSharedMemData == NULL ) return (-1 );

      gHalSpinLock = malloc( sizeof(XDRV_SPIN_LOCK) );
      if ( gHalSpinLock == NULL ) return (-1 );

      gHalDspStackPtr = malloc( XDRV_ITPC_DSP_STACK_PTR_MAX * sizeof(XDRV_ITPC_DSP_STACK_PTR) );
      if ( gHalDspStackPtr == NULL ) return (-1 );

      gHalDspStackDump = malloc( sizeof(XDRV_ITPC_DSP_STACK_DUMP) );
      if ( gHalDspStackDump == NULL ) return (-1 );

      gHalshimConfig = (LHAPI_HALSHIM_CONFIG*)malloc( sizeof(halShimConfig) );
      if ( gHalshimConfig == NULL ) return (-1 );

      memset(gHalshimConfig, 0, sizeof(gHalshimConfig));

      gHalshimEntryCount = bhiPopulateHalshimConfig( gHalshimConfig );      
      
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "gHalshimEntryCount = %u ", (unsigned int)gHalshimEntryCount ));


      firstTime = VRG_FALSE;
   }

   /* Get the DSP configuration */
   boardHalDspGetCfg(&dspCfg);

   /* Determine if the HVG is required */
   if ( initModules & BOARDCFG_MODULE_HVGRING )
   {
      bUseHvg = VRG_TRUE;
   }
   else
   {
      bUseHvg = VRG_FALSE;
   }

   /*
   ** Initialize the Audio Processing Module (APM)
   */
   /* Assume that APM will be running system-wide in either wideband or narrowband mode, not
   ** on a per-channel or per-slic basisc. We choose whether to run in wideband or narrowband
   ** mode by checking the first halShimConfig entry. */
   if ( boardHalApmInit( boardHalInitCfg, ((gHalshimConfig->halShimMode == HALSHIM_WIDEBAND_MODE) ? XDRV_TRUE : XDRV_FALSE ), dspCfg.frameSyncSamples, bUseHvg ))
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "APM init FAILED" ));
      return ( -1 );
   }

   /* 
   ** This function should be invoked before pcmInit. The function ensures that all SLICs and
   ** relays are set to their correct default state before PCM initializes.  
   */
   (void)boardHalResetGpios();

   /*
    * Initialize PCM module and DMA. Must be done before DSP initialization, where we will 
    *  kick off the PCM interrupt processing.
    */
   bhiPrepareChannelTimeslotMap( chanTsMap );
   
   /* Check if ISI configuration or ZSI configuration. */
   if ( (voiceParams->flags & BP_FLAG_ISI_SUPPORT) == 0x2 )
   {
      supportedPCMPllMode |= MODE_ISI;
   }
   else if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) == 0x4 )
   {
      supportedPCMPllMode |= MODE_ZSI;
   }

   if ( voiceParams->numDectLines > 0 )
   {   
      supportedPCMPllMode |= MODE_DECT;
   }
   pcmInit( chanTsMap, BCM_PCM_CHAN_MASK, supportedPCMPllMode );

   /*
   ** Create a spin-lock object. This is used to prevent concurrent access
   ** to shared memory data between the two thread processors.
   */
   xdrvSpinLockCreate( gHalSpinLock );

   /*
   ** Initialize the the thread-processor communcation layer.
   */
   gHalItpcSharedMemData->recvStatusReg = gHalItpcRecvStatusReg;
   gHalItpcSharedMemData->sendStatusReg = gHalItpcSendStatusReg;
   gHalItpcSharedMemData->syncRecvCmd   = gHalItpcSyncRecvCmd;
   gHalItpcSharedMemData->syncSendCmd   = gHalItpcSyncSendCmd;

   memset( &gHalItpcMutex, 0, sizeof( gHalItpcMutex ) );
   gHalItpcMutex.acquireFunc  = ItpcMutexAcquire;
   gHalItpcMutex.releaseFunc  = ItpcMutexRelease;
   gHalItpcMutex.data         = &gHalItpcBosMutex;
   bosMutexCreate( "ITPC-mutex", &gHalItpcBosMutex );

   gHalDspStackDump->stackTraceCount = 0;
   gHalDspStackDump->stackPtr = gHalDspStackPtr;

   xdrvItpcInit( INTERRUPT_ID_SOFTWARE_0,
                 gHalItpcSharedMemData,
                 gHalSpinLock,
                 &gHalItpcMutex,
                 &gHalItpcDrv );


   /*
   ** Init the exception module that will handle remote exceptions that
   ** occur on the "DSP" thread processor.
   */
   exceptionHdlrCmtInit( CmtExceptionCallback, NULL, &gHalItpcDrv );


   /*
   ** Init the thread-processor dynamic load balancing (DLB) module.
   */
#if USE_DLB_STUB
   dlbStubCreate( &gHalDlbDrv );
#else
   dlbCreate( /* Throttle duration mSec - half the min DSP kernel thread-rate. */
                  (dspCfg.frameSyncSamples >> (3 + 1)),
                  &gHalItpcDrv,
                  &gHalDlbDrv );
#endif /* !USE_DLB_STUB */

   /*
   ** Init the "DSP" thread processor, load the "DSP" image, and kick-start it.
   */
   if ( boardHalDspInit() )
   {
      return ( -4 );
   }
#ifdef BRCM_6328
#ifdef BRCM_PCM_SOFT_IRQ_BUILD
   else
   {
      VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL,  "PcmEnabled from host ********* " ));
      boardHalPcmEnable();
   }
#endif
#endif

   /*
   ** Initialize the host-DSP communications layer
   */
   if ( boardHalIpcInit( 0 /* not used */, NULL /* not used */, 0 /* not used */ ) )
   {
      return ( -5 );
   }

   /*
   ** Init the timer task used to perform DSP data processing.
   */
   if ( boardHalDspTaskInit() )
   {
      return ( -6 );
   }


   /*
   ** Init the debug module.
   */
   //debugCmtInterfaceInit( &gHalItpcDrv, &gDebugCmtDrv );

#ifdef BRCM_6328
   /*
   ** Invert the polarity of the SPI bus clock and configure
   ** associated launch and latch registers
   */
   //boardHalSpiClkInit(); - Configured through BcmSpiReserveSlave2 now using Linux SPI Framework.
#endif

   /*
   ** Initialize the MSPI module
   */
   if ( initModules & BOARDCFG_MODULE_MSPI )
   {
      if ( boardHalMspiInit() )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_BOARDHAL,  "ERROR: MSPI init FAILED" ));
         return ( -1 );
      }
      else
      {
         VRG_LOG_INFO((VRG_LOG_MOD_BOARDHAL,  "MSPI INITIALIZED" ));
         mspiInUse = 1;
      }
   }

#ifndef G729_DSPTEST_APP
   /*
   ** Initialize the SLIC interface
   */
   if ( boardHalSlicInit(boardHalInitCfg, VRG_FALSE) )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_BOARDHAL,  "ERROR: SLIC init FAILED" ));
      return ( -2 );
   }
#endif


   if ( voiceParams->numDectLines > 0 )
   {
      /*
      ** Initialize the DECT interface
      */
      if ( boardHalDectInit(VRG_FALSE) )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_BOARDHAL,  "ERROR: DECT init FAILED" ));
         return ( -2 );
      }
   }


  switch( boardHalInitCfg->slicType )
   {
      case XDRV_SLIC_L9530:
      {
         bCasHistorySupported = VRG_TRUE;
      }
      break;               
      case XDRV_SLIC_SI3239:
      case XDRV_SLIC_SI32392:
      {
         bCasHistorySupported = VRG_FALSE;
      }
      break;
      default:
      break;
   }
   /*
   ** Initialize the hook state history log for CAS
   */
   if ( initModules & BOARDCFG_MODULE_FASTSLIC )
   {
      bFastSlicSupported = VRG_TRUE;
   }
   else
   {
      bFastSlicSupported = VRG_FALSE;
   }
   
   /*
   ** Initialize the hook state history log for CAS
   */
   if ( boardHalCasInit(bFastSlicSupported,bCasHistorySupported) )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_BOARDHAL,  "ERROR: CAS History init FAILED" ));
      return ( -3 );
   }

#if PSTN_LIFE_LINE_SUPPORT   
   if ( boardHalIsPstnLifeLineSupported() )
   {
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Failover relay(s) PRESENT " )); 
      if ( boardHalRelayInit(VRG_FALSE) )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_BOARDHAL,  "ERROR: RELAY init FAILED" ));
         return ( -2 );
      }
   }
   else
   {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Failover relay NOT PRESENT !!! " )); 
   }
#endif 

   /*
   ** Initialize the DAA
   */
   if ( boardHalDaaInit( boardHalInitCfg->country ) )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_BOARDHAL,  "ERROR: DAA init FAILED" ));
      return ( -4 );
   }

#if defined ( BRCM_6816 )/* || defined ( BRCM_6828 ) */
   if ( initModules & BOARDCFG_MODULE_TPD )
   {
      /*
      ** Initialize the TPD (linetest) service
      */
      if ( boardHalTpdInit() )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "TPD init FAILED" ));
         return ( -4 );
      }
   }
#endif

   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "COMPLETED Initializing board HAL"));

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalDeinitUni
**
** PURPOSE:    Uninitialize the HAL. Reset any local storage and
**             free all resources.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, otherwise error code.
**             This function should only return an error under catastrophic
**             circumstances. i.e. Something that cannot be fixed by re-invoking
**             the module initialization function.
**
*****************************************************************************
*/
int boardHalDeinitUni( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg, int deinitModules )
{
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   /*
   ** Shutdown the CAS driver.
   */
   boardHalCasDeinit();

#ifndef G729_DSPTEST_APP
   /*
   ** Shutdown the SLICs
   */
   boardHalSlicDeinit(boardHalInitCfg);
   boardHalDaaDeinit();
#endif

#ifdef BRCM_6328
   /*
   ** Revert SPI clock parameters to their reset values
   */
   boardHalSpiClkDeinit();
#endif

   /*
   ** Shutdown the MSPI module
   */
   if( mspiInUse )
   {
      boardHalMspiDeinit();
      mspiInUse = 0;
   }
   
   /*
   ** Shutdown the debug module.
   */
   debugCmtInterfaceDeinit();

   /*
   ** Shutdown the DSP timer task.
   */
   boardHalDspTaskDeinit();

   /*
   ** Shutdown the host-DSP communications layer
   */
   boardHalIpcDeinit(0);


   /*
   ** Shutdown the DSP thread processor.
   */
   boardHalDspDeinit();

#ifdef BRCM_6328
#ifdef BRCM_PCM_SOFT_IRQ_BUILD
   VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL,  "PcmEnabled from host ********* " ));
   boardHalPcmDisable();
#endif
#endif

   /*
   ** Shutdown the DECT DMA. This needs to happen after the DSP has
   ** been deinitialized.
   */
   if ( voiceParams->numDectLines > 0 )
   {
      /*
      ** De-Initialize the DECT interface
      */
      if ( boardHalDectDeinit() )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_BOARDHAL,  "ERROR: DECT de init FAILED" ));
         return ( -2 );
      }
   }

   /*
   ** Shutdown the thread-processor dynamic load balancing module.
   */
#if USE_DLB_STUB
   dlbStubDestroy( &gHalDlbDrv );
#else 
   dlbDestroy( &gHalDlbDrv );
#endif /* !USE_DLB_STUB */


   /*
   ** Shutdown the exception handler for the "DSP" thread processor.
   */
   exceptionHdlrCmtDeinit();


   /*
   ** Shutdown the thread-processor communication layer.
   */
   xdrvItpcDeinit( &gHalItpcDrv );
   bosMutexDestroy( &gHalItpcBosMutex );


   /*
   ** Destroy the spin-lock.
   */
   xdrvSpinLockDestroy( gHalSpinLock );

   /*
   ** Shutdown the PCM module
   */
   pcmDeinit();

   /*
   ** Shutdown the APM module.
   */
   boardHalApmDeinit();

#if 0
   /* 
   ** HACK: Do not free shared memory so that reset will work!
   **       DSP already set up pointers to these memory locations during bootup,
   **         and it does not remapped the pointers when host tries to reset it.
   **       To keep the ITPC communication between the host and DSP, we better 
   **         not change the memory block location after it was first initalized.
   */
   /* 
   ** Deallocate shared memory 
   */
   free( gHalItpcSyncRecvCmd );
   free( gHalItpcSyncSendCmd );
   free( gHalItpcRecvStatusReg );
   free( gHalItpcSendStatusReg );
   free( gHalItpcSharedMemData );
   free( gHalSpinLock );
#endif

   VRG_LOG_NOTICE((VRG_LOG_MOD_BOARDHAL, "%s: De-initialization complete.", __FUNCTION__));

   return ( 0 );
}

#ifdef DYING_GASP_API
/*
*****************************************************************************
** FUNCTION:   boardHalShutdownUni
**
** PURPOSE:    reset slic, slac/daa and DSP
**
** PARAMETERS: context pointer
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalShutdown( void *context)
{
   boardHalSlicShutdown( context );
   boardHalDaaShutdown( context );
#ifndef BRCM_NONDIST_BUILD 
   boardHalDspShutdown( context );
#endif
}
#endif

#ifdef BRCM_NONDIST_BUILD
/*
*****************************************************************************
** FUNCTION:   boardHalIsrEnable
**
** PURPOSE:    Calls either boardHalPcmEnable or boardHalApmEnable depending
**             on the mode
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalIsrEnable( void )
{
   halIsrEnable();
}

/*
*****************************************************************************
** FUNCTION:   boardHalIsrDisable
**
** PURPOSE:    Calls either boardHalPcmDisable or boardHalApmDisable depending
**             on the mode
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalIsrDisable( void )
{
   halIsrDisable();
}

/*
*****************************************************************************
** FUNCTION:   boardHalIsrDisable
**
** PURPOSE:    Retrieves the ISR counter
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
int boardHalIntCountGet( void )
{
   return( halIntCountGet() );
}

#endif

/*
*****************************************************************************
** FUNCTION:   CmtExceptionCallback
**
** PURPOSE:    Concurrent multi-threaded (CMT) exception callback. Invoked
**             when an exception occurs on the "DSP" thread processor.
**
** PARAMETERS: param (in)  User registered data.
**
** RETURNS:    0 on success, else error code
**
*****************************************************************************
*/
static int CmtExceptionCallback( EXCEPTION_HDLR_CMT_CALLBACK_PARM param )
{
   XDRV_DEBUG    *pDebugDriver = &gDebugCmtDrv;
   int i = 0;

   /* Not used. */
   (void) param;

   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "CmtExceptionCallback: DSP Call Dump" ));
   for (i = 0; i < gHalDspStackDump->stackTraceCount; i++)
   {
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "[<%x>]", gHalDspStackDump->stackPtr[i] ));
   }


   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "******* DSP Thread Processor Exception ********" ));

   /*
   ** Utilize the Debug module to retrive some debug info from the "DSP"
   ** thread processor.
   */
   xdrvDebugDspLogAssertContext( pDebugDriver );


   /*
   ** Let things settle while "DSP" thread processor debug info is logged.
   ** Don't want it to be mangled with the debug info that will result from
   ** the purposeful crash below (which will write directly to the UART driver
   ** from exception context).
   */
   bosSleep( 250 );


   /* Notify that the DSP asserted */
   XCHG_ASSERT(0);

   return( 0 );
}



/*
*****************************************************************************
** FUNCTION:   ItpcMutexAcquire
**
** PURPOSE:    Callback invoked to acquire ITPC mutex.
**
** PARAMETERS: parm  (in) User registered data (pointer to mutex).
**
** RETURNS:    0 on success.
**
** NOTE:
*****************************************************************************
*/
static unsigned int ItpcMutexAcquire( XDRV_ITPC_MUTEX_PARM parm )
{
   BOS_MUTEX *mutex = (BOS_MUTEX *) parm;

   if ( bosMutexAcquire( mutex ) != BOS_STATUS_OK )
   {
      return ( -1 );
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   ItpcMutexRelease
**
** PURPOSE:    Callback invoked to release ITPC mutex.
**
** PARAMETERS: parm  (in) User registered data (pointer to mutex).
**
** RETURNS:    0 on success.
**
** NOTE:
*****************************************************************************
*/
static unsigned int ItpcMutexRelease( XDRV_ITPC_MUTEX_PARM parm )
{
   bosMutexRelease( (BOS_MUTEX *) parm );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   bhiPopulateHalshimConfig
**
** PURPOSE:    Populate the halshim config table in order to initialize the
**             DSP with the right settings for the channels
**
** PARAMETERS: halShimConfig  -  [IN]  Pointer to the LHAPI_HALSHIM_CONFIG structure
**
** RETURNS:
**
** NOTE:       Memory for halShimConfig should be allocated by the caller
**
*****************************************************************************
*/
static int bhiPopulateHalshimConfig( LHAPI_HALSHIM_CONFIG* halShimConfig )
{
   int deviceId = 0;
   int lineIdTotal = 0;

   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   while ( voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE )
   {
      if ( (voiceParams->numDectLines == 0 ) &&
           ((voiceParams->voiceDevice[deviceId].voiceDeviceType == BP_VD_IDECT1)|| 
            (voiceParams->voiceDevice[deviceId].voiceDeviceType == BP_VD_EDECT1))  )
      {
         /* number of DECT line is zero, so we should not configure any DECT device. */
         deviceId++;
         continue;
      }

      int lineIdWithinDevice = 0;
      while ( lineIdWithinDevice < BP_MAX_CHANNELS_PER_DEVICE )
      {
         if ( voiceParams->voiceDevice[deviceId].channel[lineIdWithinDevice].status == BP_VOICE_CHANNEL_ACTIVE )
         {
            if ( voiceParams->voiceDevice[deviceId].channel[lineIdWithinDevice].freqRange == BP_VOICE_CHANNEL_NARROWBAND )
            {
               halShimConfig->halShimMode          = HALSHIM_NARROWBAND_MODE;
            }
            else
            {
               halShimConfig->halShimMode          = HALSHIM_WIDEBAND_MODE;
            }

            switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
            {
               case ( BP_VD_IDECT1 ):
               {
                  halShimConfig->halShimPcmChannels   = HALSHIM_PCM_CHANNELS_NONE;
                  halShimConfig->halShimType          = HAL_SHIM_CONFIG_AUDIO_IDECT;
               }
               break;

               case ( BP_VD_ZARLINK_9530 ):
               case ( BP_VD_ZARLINK_9540 ):
               case ( BP_VD_ZARLINK_9541 ):
               case ( BP_VD_SILABS_3239 ):
               case ( BP_VD_SILABS_32392 ):
               {
                  halShimConfig->halShimPcmChannels   = HALSHIM_PCM_CHANNELS_NONE;
                  halShimConfig->halShimType          = HAL_SHIM_CONFIG_AUDIO_SPECIAL;
               }
               break;

               default:
               {
                  if ( halShimConfig->halShimMode == HALSHIM_NARROWBAND_MODE )
                  {
                     halShimConfig->halShimPcmChannels   = HALSHIM_PCM_CHANNELS_ONE;
                     halShimConfig->halShimType          = HAL_SHIM_CONFIG_AUDIO;
                  }
                  else
                  {
                     if( (voiceParams->voiceDevice[deviceId].voiceDeviceType == BP_VD_EDECT1) && 
                         (voiceParams->voiceDevice[deviceId].channel[lineIdWithinDevice].sampleSize == BP_VOICE_CHANNEL_SAMPLE_SIZE_8BITS) )
                     {
                        /* Special configuration for External Dect Compressed Wideband */
                        halShimConfig->halShimPcmChannels   = HALSHIM_PCM_CHANNELS_ONE;
                        halShimConfig->halShimType          = HAL_SHIM_CONFIG_AUDIO;
                     }
                     else
                     {
                        /* We currrently support as default, two pcm channels when wideband voice channels are configured */
                        halShimConfig->halShimPcmChannels   = HALSHIM_PCM_CHANNELS_TWO;
                        halShimConfig->halShimType          = HAL_SHIM_CONFIG_AUDIO;
                     }
                  }
               }
            }

            halShimConfig->halShimSampleSize       = ( voiceParams->voiceDevice[deviceId].channel[lineIdWithinDevice].sampleSize == BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS ) ? 
                                                         HALSHIM_SAMPLE_SIZE_16BITS : HALSHIM_SAMPLE_SIZE_8BITS;

            switch ( voiceParams->voiceDevice[deviceId].channel[lineIdWithinDevice].pcmCompMode )
            {
               case BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE:
               {
                  halShimConfig->halShimPcmCompMode = HALSHIM_PCM_COMP_NONE;
               }
               break;

               case BP_VOICE_CHANNEL_PCMCOMP_MODE_ALAW:
               {
                  halShimConfig->halShimPcmCompMode = HALSHIM_PCM_COMP_ALAW;
               }
               break;

               case BP_VOICE_CHANNEL_PCMCOMP_MODE_ULAW:
               {
                  halShimConfig->halShimPcmCompMode = HALSHIM_PCM_COMP_ULAW;
               }
               break;

               default:
               {
                  halShimConfig->halShimPcmCompMode = HALSHIM_PCM_COMP_NONE;
               }
               break;
            }

            halShimConfig->halShimPcmChannelMute   = HALSHIM_PCM_CHANNEL_MUTE_OFF;
            halShimConfig->halShimByteOrder        = ( voiceParams->voiceDevice[deviceId].channel[lineIdWithinDevice].endianMode == BP_VOICE_CHANNEL_ENDIAN_BIG ) ? 
                                                         HALSHIM_BYTE_ORDER_BIG_ENDIAN : HALSHIM_BYTE_ORDER_LITTLE_ENDIAN;

            halShimConfig++;
         }
         lineIdWithinDevice++;
         lineIdTotal++;
      }

      deviceId++;
   }

   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Total number of lines        =  %d", (voiceParams->numFxsLines + voiceParams->numFxoLines + voiceParams->numDectLines) )); 

   return ( voiceParams->numFxsLines + voiceParams->numFxoLines + voiceParams->numDectLines );
}


/*
*****************************************************************************
** FUNCTION:   bhiPrepareChannelTimeslotMap
**
** PURPOSE:    Populate the timeslot maps for the channels
**
** PARAMETERS: 
**
** RETURNS:
**
** NOTE:       Memory for halShimConfig should be allocated by the caller
**
*****************************************************************************
*/
static void bhiPrepareChannelTimeslotMap( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap )
{
   BOARDHALPROV_DEVICE_CHANNEL_MAP* pDeviceChannelMap;
   int i;
   int pcmChannel = 0;
  
   for ( i = 0; i < boardHalGetNumEndpoints(); i++ )
   {
      pDeviceChannelMap = boardHalProvGetChannelMap( i );
      if ( pDeviceChannelMap->txTimeslot == BP_TIMESLOT_INVALID )
      {
         VRG_LOG_NOTICE((VRG_LOG_MOD_BOARDHAL, "Unused or Invalid PCM timeslot %d for endpoint %d ", BP_TIMESLOT_INVALID, i ));
      }
      else
      {
         chanTsMap[pcmChannel].chan = i;
         chanTsMap[pcmChannel].txTimeslot = pDeviceChannelMap->txTimeslot;
         chanTsMap[pcmChannel].rxTimeslot = pDeviceChannelMap->rxTimeslot;
         VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "     ***** %s %d chanTsMap[%d].txTimeslot = %d ", __FUNCTION__, __LINE__, pcmChannel, chanTsMap[pcmChannel].txTimeslot));  
         VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "     ***** %s %d pDeviceChannelMap->txTimeslot = %d ", __FUNCTION__, __LINE__, pDeviceChannelMap->txTimeslot));
         pcmChannel++;
         
         if( pDeviceChannelMap->deviceChannelFreqRange == BP_VOICE_CHANNEL_WIDEBAND )
         {
            /* For wideband devices add a second adjacent PCM channel */
            chanTsMap[pcmChannel].chan = i;
            
            /* By default we support a wideband pcm configuration in which 2 samples per    *
             * voice channel are transmitted in an 8Khz pcm frame. The timeslot for         *
             * the second sample is at a fixed offset from the first sample ( offset is     *
             * is 8 for 16bit timeslots). This pcm scheme mirrors the scheme in our slacs   */
            chanTsMap[pcmChannel].txTimeslot = pDeviceChannelMap->txTimeslot + 8;
            chanTsMap[pcmChannel].rxTimeslot = pDeviceChannelMap->rxTimeslot + 8;

            
            VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "     ***** %s %d chanTsMap[%d].txTimeslot = %d ", __FUNCTION__, __LINE__, pcmChannel, chanTsMap[pcmChannel].txTimeslot));
            VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "     ***** %s %d pDeviceChannelMap->txTimeslot = %d ", __FUNCTION__, __LINE__, chanTsMap[pcmChannel].rxTimeslot));
            pcmChannel++;                        
         }
      }
   }

   /* Fill PCM_CHAN_INVALID and PCM_TIMESLOT_INVALID at the end of the channel-timeslot map */
   if ( pcmChannel < PCM_MAX_CHANNELS ) 
   {
      chanTsMap[pcmChannel].chan       = PCM_CHAN_INVALID;
      chanTsMap[pcmChannel].txTimeslot = PCM_TIMESLOT_INVALID;
      chanTsMap[pcmChannel].rxTimeslot = PCM_TIMESLOT_INVALID;
   }
}
#if 0 /* chip ID check is obsolete */
/*
*****************************************************************************
** FUNCTION:   bhiVerifyChipId
**
** PURPOSE:    Verify the chip id and return error if the image
**             is running on non-expected chip
**
** PARAMETERS: 
**
** RETURNS:    0 for success, -1 otherwise
**
** NOTE:
**
*****************************************************************************
*/
static int bhiVerifyChipId( void )
{
#ifdef BRCM_6328
   if ( (GPIO->DieRevID & CHIP_SUB_ID_MASK_6328) != CHIP_ID_6328 )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Incorrect chip ID 0x%08X, this is not a BCM6328-based chip !!!", (unsigned int)GPIO->DieRevID));
      return ( -1 );
   }
#endif

#ifdef BRCM_6362
   if ( (GPIO->DieRevID & CHIP_SUB_ID_MASK_6362) != CHIP_ID_6362 )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Incorrect chip ID 0x%08X, this is not a BCM6362-based chip !!!", (unsigned int)GPIO->DieRevID));
      return ( -1 );
   }
#endif

#ifdef BRCM_63268
   if ( (GPIO->DieRevID & CHIP_SUB_ID_MASK_63268) != CHIP_ID_63268 )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Incorrect chip ID 0x%08X, this is not a BCM63268-based chip !!!", (unsigned int)GPIO->DieRevID));
      return ( -1 );
   }
#endif

#ifdef BRCM_6368
   if ( (GPIO->DieRevID & CHIP_SUB_ID_MASK_6368) != CHIP_ID_6368 )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Incorrect chip ID 0x%08X, this is not a BCM6368-based chip !!!", (unsigned int)GPIO->DieRevID));
      return ( -1 );
   }
#endif

#ifdef BRCM_6816
   if ( (GPIO->DieRevID & CHIP_SUB_ID_MASK_6816) != CHIP_ID_6816 )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Incorrect chip ID 0x%08X, this is not a BCM6816-based chip !!!", (unsigned int)GPIO->DieRevID));
      return ( -1 );
   }
#endif

#ifdef BRCM_63381
   if ( (GPIO->DieRevID & CHIP_SUB_ID_MASK_63381) != CHIP_ID_63381)
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Incorrect chip ID 0x%08X, this is not a BCM63381-based chip !!!", (unsigned int)GPIO->DieRevID));
      return ( -1 );
   }
#endif

#ifdef BRCM_6848
   if ( (GPIO->DieRevID & CHIP_SUB_ID_MASK_6848) != CHIP_ID_6848)
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Incorrect chip ID 0x%08X, this is not a BCM6848-based chip !!!", (unsigned int)GPIO->DieRevID));
      return ( -1 );
   }
#endif
   VRG_LOG_DBG(( VRG_LOG_MOD_EPT, "GPIO->DieRevID = 0x%08X PERF->RevID = 0X%08X", (unsigned int)GPIO->DieRevID, (unsigned int)PERF->RevID ));

   return ( 0 );
}
#endif /* chip ID check is obsolete */

#if PSTN_LIFE_LINE_SUPPORT
/*
*****************************************************************************
** FUNCTION:   boardHalIsPstnLifeLineSupported
**
** PURPOSE:    Determines whether pstn life line is supported for this platform
**
** PARAMETERS: nothing
**
** RETURNS:    VRG_TRUE if pstn life line is supported; VRG_FALSE otherwise
**
** NOTE:      
**
*****************************************************************************
*/
VRG_BOOL boardHalIsPstnLifeLineSupported( void )
{
   VOICE_BOARD_PARMS* voiceParams;

   /* Retrieve Voice Params */
   voiceParams = boardHalProvGetVoiceParms();

   if ( voiceParams->numFailoverRelayPins > 0 )
   {
      return VRG_TRUE;
   }
   else
   {  
      return VRG_FALSE;
   }
}        
#endif /* PSTN_LIFE_LINE_SUPPORT */
