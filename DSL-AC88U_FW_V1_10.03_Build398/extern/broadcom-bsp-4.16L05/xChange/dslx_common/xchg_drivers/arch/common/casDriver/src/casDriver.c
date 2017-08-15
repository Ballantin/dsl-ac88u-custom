/****************************************************************************
*
* Copyright © 2000-2010 Broadcom Corporation
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
*    Filename: casDriver.c
*
****************************************************************************
*    Description:
*
*     This file implements the CAS driver. The purpose of the CAS driver is:
*
*     1) To interface to the SLIC driver to control the SLIC mode and
*        obtain hookstate information.
*
*     2) To optionally provide a CAS history buffer. The history buffer provides
*        a periodic log of hook-state sampled by an ISR. The samples should be
*        processed by a client state-machine in order to detect CAS events
*        (e.g. onhook, offhook, flash, wink, pulse dial).
*
*     3) To provide low-level debounce logic. Samples may be debounced prior to
*        being logged to the history buffer.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <xchgAssert.h>
#include <xdrvGlobalInterrupt.h>
#include <xdrvCas.h>
#include <casDriver.h>
#include <casDriverCfg.h>

#include <bosTimer.h>

#if CAS_DRIVER_CFG_HISTORY_LOG_ENABLED
   #include <xdrvTimer.h>
#endif

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

#if CAS_DRIVER_CFG_HISTORY_LOG_ENABLED
/* Interface object for periodic timer interrupts */
static XDRV_TIMER_PERIODIC gTimerCfg;
#endif

/* Linked-list of CAS drivers. */
static XDRV_CAS  *gCasDriverListHead = NULL;
static XDRV_CAS  *gCasDriverListTail = NULL;

static XDRV_BOOL  gCasDriverStarted  = XDRV_FALSE;

/* ---- Private Function Prototypes -------------------------------------- */

#if CAS_DRIVER_CFG_HISTORY_LOG_ENABLED
static unsigned int PeriodicTimerIsr( void *context );
#endif
static void hvgUpdateHookStatus(XDRV_APM *pDrv, int endpt, int hookState);
/* ---- Functions -------------------------------------------------------- */
/*
*****************************************************************************
** FUNCTION:   hvgUpdateHookStatus
**
** PURPOSE:    Update HVG parameters based on latest hook state
**
** PARAMETERS: pDrv       - pointer to the APM driver device structure
**             endpt      - endpoint to control
**             hookState  - current hook state
**
** RETURNS:    none
**
*****************************************************************************/
static void hvgUpdateHookStatus(XDRV_APM *pDrv, int endpt, int hookState)
{
   #if defined ( BRCM_6816 ) || defined ( BRCM_6828 ) || defined ( BRCM_6818 ) || defined ( BRCM_6838 )
      xdrvApmHvgUpdateHookStatus( pDrv, endpt, hookState );
   #endif
}
/*
*****************************************************************************
** FUNCTION:   casDriverStartHistoryLog
**
** PURPOSE:    Launch a periodic ISR that logs hookstate samples to a
**             history buffer.
**
** PARAMETERS: historyRateMsec      - Period ISR hookstate sampling rate.
**             debounceIntervalMsec - Hookstate samples are debouced by this amount
**                                    prior to being logged to the history buffer.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int casDriverStartHistoryLog( int historyRateMsec, int debounceIntervalMsec )
{
#if CAS_DRIVER_CFG_HISTORY_LOG_ENABLED

   XDRV_CAS   *pDriver;
   XDRV_BOOL   bEnableCasHistory = XDRV_FALSE;

   if( gCasDriverStarted )
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "casDriver: History log already started!"));
      return ( 1 );
   }

   /* Set the history rate for every channel */
   pDriver = gCasDriverListHead;
   while ( pDriver != NULL )
   {
      if ( pDriver->casHistory.bHistorySupported == XDRV_TRUE )
      {
      pDriver->casHistory.historyRateMsec       = historyRateMsec;
      pDriver->casHistory.historyEnabled        = XDRV_TRUE;
      pDriver->casHistory.debounceIntervalMsec  = debounceIntervalMsec;
         bEnableCasHistory                         = XDRV_TRUE;
      }
      pDriver = pDriver->nextDrv;
   }

   if ( bEnableCasHistory )
   {
   /* Register an IRQ with the Periodic Timer Interrupt Service */
   if ( xdrvTimerPeriodicInstall( PeriodicTimerIsr,
                                  historyRateMsec,
                                  NULL,
                                  &gTimerCfg ))
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "casDriver: xdrvTimerPeriodicInstall failed !!!"));
      return ( 1 );
   }

   /* Start the Periodic Timer Interrupt */
   if ( xdrvTimerPeriodicStart( &gTimerCfg ))
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "casDriver: xdrvTimerPeriodicStart failed !!!"));
      return ( 1 );
   }

   gCasDriverStarted = XDRV_TRUE;
   }
#endif     /* CAS_DRIVER_CFG_HISTORY_LOG_ENABLED */

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   casDriverStopHistoryLog
**
** PURPOSE:    De-initialize the periodic timer IRQ
**
** PARAMETERS: None
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int casDriverStopHistoryLog( void )
{
#if CAS_DRIVER_CFG_HISTORY_LOG_ENABLED

   if( !gCasDriverStarted )
   {
      /* History log already stopped */
      return ( 1 );
   }

   /* Stop an IRQ */
   if ( xdrvTimerPeriodicStop( &gTimerCfg ))
   {
      return ( 1 );
   }


   /* Uninstall the IRQ from the Periodic Timer Interrupt Service */
   if ( xdrvTimerPeriodicUninstall( &gTimerCfg ))
   {
      return ( 1 );
   }

   gCasDriverStarted = XDRV_FALSE;

   return ( 0 );

#else    /* CAS_DRIVER_CFG_HISTORY_LOG_ENABLED */

   return ( 1 );

#endif   /* !CAS_DRIVER_CFG_HISTORY_LOG_ENABLED */
}

#if CAS_DRIVER_CFG_HISTORY_LOG_ENABLED
/*
*****************************************************************************
** FUNCTION:   PeriodicTimerIsr
**
** PURPOSE:    Periodic timer ISR for monitoring hook state
**
** PARAMETERS: context - context ISR runs in
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
static unsigned int PeriodicTimerIsr( void *context )
{
   XDRV_CAS_HOOKSTATE   slicState;
   XDRV_CAS            *pDriver;
   XDRV_CAS_HISTORY    *pHistory;

   /* Restart the Periodic Timer Interrupt */
   if ( xdrvTimerPeriodicStart( &gTimerCfg ))
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "casDriver: xdrvTimerPeriodicStart failed !!!"));
      return ( 1 );
   }

   /* Log every channel */
   pDriver = gCasDriverListHead;
   while ( pDriver != NULL )
   {
      pHistory = &pDriver->casHistory;
      if ( pHistory->bHistoryEnabled )
      {
      if ( xdrvSlicIsOffhook( pDriver->slicDriver ))
      {
         slicState = XDRV_CAS_OFFHOOK;
      }
      else
      {
         slicState = XDRV_CAS_ONHOOK;
      }

      /* Save the current hook state into a history buffer */
      if ( slicState == pHistory->currentState )
      {
         pHistory->currentIntervalMsec = 0;
         *pHistory->writep = pHistory->currentState;
      }
      else
      {
         pHistory->currentIntervalMsec += pHistory->historyRateMsec;

         if ( pHistory->currentIntervalMsec > pHistory->debounceIntervalMsec )
         {
            pHistory->currentState = slicState;
            pHistory->currentIntervalMsec = 0;

            /* Inform APM/HVG of change in hook status */
            if( pHistory->currentState == XDRV_CAS_OFFHOOK )
            {
               hvgUpdateHookStatus( pDriver->apmDriver, pDriver->chan, XDRV_TRUE );
            }
            else
            {
               hvgUpdateHookStatus( pDriver->apmDriver, pDriver->chan, XDRV_FALSE );
            }

            /*
            ** Perform a SLIC mode change if fast SLIC state mode enabled and
            ** hook state change detected. This is needed for some VMWI and
            ** CWCLID devices that perfom a fake offhook switch and expect the
            ** SLIC mode to change instantly.
            */
            if ( pDriver->bFastSlicStateModeEnabled &&
                 pDriver->bFastSlicStateModeSupported )
            {
               if( pHistory->currentState == XDRV_CAS_OFFHOOK )
               {
                  xdrvSlicModeControl( pDriver->slicDriver, XDRV_SLIC_MODE_LCF );
               }
               else
               {
                  xdrvSlicModeControl( pDriver->slicDriver, XDRV_SLIC_MODE_STANDBY );
               }
            }
         }

         *pHistory->writep = pHistory->currentState;
      }

      /* Update the write pointer */
      pHistory->writep++;

      /* Wrap if applicable */
      if( pHistory->writep > &pHistory->hookStateBuf[XDRV_CAS_HIST_BUFFER_SIZE-1] )
      {
         pHistory->writep = &pHistory->hookStateBuf[0];
      }
     }
      pDriver = pDriver->nextDrv;
   }

   return ( 0 );
}
#endif   /* CAS_DRIVER_CFG_HISTORY_LOG_ENABLED */


/*
*****************************************************************************
** FUNCTION:   casDriverInit
**
** PURPOSE:    This function initializes the hook state history log
**             needed for CAS
**
** PARAMETERS: casDriver         - CAS driver to initialize
**             slicDrv           - SLIC driver for this device
**             apmDrv            - APM driver for this device
**             chan              - channel number of this instance
**             bFastSlicSupported - True if fast SLIC mode changing should
**                                 be supported on the platform.
**             bCasHistorySupported - True if history log should be supported.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int casDriverInit( XDRV_CAS *casDriver, XDRV_SLIC *slicDrv, XDRV_APM *apmDrv,  
                   int chan, XDRV_BOOL bFastSlicSupported, 
                   XDRV_BOOL bCasHistorySupported )
{
   int              i;
   XDRV_CAS_HISTORY *pHistory;

   pHistory = &casDriver->casHistory;

   pHistory->debounceIntervalMsec  = 0;

   /* Set history mode */
   pHistory->bHistorySupported     = bCasHistorySupported;
   pHistory->historyEnabled        = XDRV_FALSE;

   pHistory->currentState = XDRV_CAS_UNKNOWN;
   pHistory->currentIntervalMsec = 0;

   pHistory->writep = &pHistory->hookStateBuf[0];
   pHistory->readp  = &pHistory->hookStateBuf[0];
   pHistory->prevp  = &pHistory->hookStateBuf[0];

   for( i = 0; i < XDRV_CAS_HIST_BUFFER_SIZE; i++ )
   {
      pHistory->hookStateBuf[i] = XDRV_CAS_UNKNOWN;
   }

   /* Save channel number */
   casDriver->chan = chan;

   /* Save the SLIC and APM driver pointers */
   casDriver->slicDriver = slicDrv;
   casDriver->apmDriver = apmDrv;

   /* Set the fast SLIC state change mode */
   casDriver->bFastSlicStateModeSupported = bFastSlicSupported;
   casDriver->bFastSlicStateModeEnabled = XDRV_FALSE;

   /*
   ** Create linked-list of drivers.
   */
   if ( gCasDriverListHead == NULL )
   {
      gCasDriverListHead = casDriver;
      gCasDriverListTail = casDriver;
   }
   else
   {
      gCasDriverListTail->nextDrv = casDriver;
      gCasDriverListTail          = casDriver;
   }

   casDriver->nextDrv = NULL;
   casDriver->bDrvEnabled = XDRV_TRUE;

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   casDriverDeinit
**
** PURPOSE:    This function de-initializes a CAS driver
**
** PARAMETERS: casDriver - CAS driver structure for this device
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int casDriverDeinit( XDRV_CAS *casDriver )
{
   XDRV_CAS_HISTORY *pHistory;

   if ( gCasDriverStarted )
   {
      casDriverStopHistoryLog();
   }

   if ( casDriver->bDrvEnabled )
   {
      casDriver->bDrvEnabled = XDRV_FALSE;
      pHistory = &casDriver->casHistory;

      pHistory->writep = &pHistory->hookStateBuf[0];
      pHistory->readp = &pHistory->hookStateBuf[0];
      pHistory->prevp = &pHistory->hookStateBuf[0];

      gCasDriverListHead = NULL;
      gCasDriverListTail = NULL;
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   casNumHookSamples
**
** PURPOSE:    Retrieves the number of samples available in the hook state
**             history buffer since the last request
**
** PARAMETERS: casDriver - CAS driver structure for this device
**
** RETURNS:    Number of samples available
**
*****************************************************************************
*/
int xdrvCasNumHookSamples( XDRV_CAS *casDriver )
{
   int                           length;
#if CAS_DRIVER_CFG_HISTORY_LOG_ENABLED
   volatile XDRV_CAS_HOOKSTATE  *writep;
   volatile XDRV_CAS_HOOKSTATE  *readp;
   XDRV_GLOBAL_INTERRUPT_LOCK    lockKey;
   XDRV_CAS_HISTORY             *casHist;
#endif

   /* If driver not enabled or we are in the middle of the deInit/shutdown
   ** procedure, pretend like there are no hook states available to read
   ** to avoid transient hook state detections */
   if ( !casDriver->bDrvEnabled )
   {
      return ( 0 );
   }

#if CAS_DRIVER_CFG_HISTORY_LOG_ENABLED

   casHist = &casDriver->casHistory;

   if ( casHist->historyEnabled )
   {
   /* Lock critical section */
   xdrvGlobalInterruptDisable( &lockKey );

   /* Get the current write/read pointers on this channel */
   writep = casHist->writep;
   readp  = casHist->readp;

   /* Leave critical section */
   xdrvGlobalInterruptRestore( &lockKey );

   /* Calculate the amount of states available in the history buffer */
   if( readp > writep )
   {
      /* CAS read pointer is ahead of history write pointer */
      length = (int)(&casHist->hookStateBuf[XDRV_CAS_HIST_BUFFER_SIZE] - readp
                     + writep - &casHist->hookStateBuf[0]);
   }
   else
   {
      /* CAS read pointer is behind history write pointer */
      length = (int)(writep - readp);
   }
   }
   else
#endif   /* CAS_DRIVER_CFG_HISTORY_LOG_ENABLED */
   {
   length = 1;
   }

   return ( length );
}

/*
*****************************************************************************
** FUNCTION:   casGetHookState
**
** PURPOSE:    To determine current hook state status
**
** PARAMETERS: casDriver - CAS driver structure for this device
**
** RETURNS:    Current hook state
**
*****************************************************************************
*/
XDRV_CAS_HOOKSTATE xdrvCasGetHookState( XDRV_CAS *casDriver )
{
   XDRV_CAS_HOOKSTATE      state;
   XDRV_CAS_HISTORY             *casHist;

   casHist = &casDriver->casHistory;

#if CAS_DRIVER_CFG_HISTORY_LOG_ENABLED
   if ( casHist->bHistoryEnabled )
   {
      XDRV_GLOBAL_INTERRUPT_LOCK    lockKey;

   /* Lock critical section */
   xdrvGlobalInterruptDisable( &lockKey );

   /* Get hook state from the history buffer */
   state = *casHist->readp;

   /* Perform a SLIC LED action if the hook state changed */
   if (*casHist->readp != *casHist->prevp)
   {
      xdrvSlicLedControl( casDriver->slicDriver, *casHist->readp );
   }

   /* Current hook state becomes the previous one */
   casHist->prevp = casHist->readp;

   /* Update the read pointer */
   casHist->readp++;

   /* Wrap if applicable */
   if (casHist->readp > &casHist->hookStateBuf[XDRV_CAS_HIST_BUFFER_SIZE-1])
   {
      casHist->readp = &casHist->hookStateBuf[0];
   }

   /* Leave critical section */
   xdrvGlobalInterruptRestore( &lockKey );
   }
   else
#endif   /* CAS_DRIVER_CFG_HISTORY_LOG_ENABLED */
   {
   /* Retrieve Slic State */
   state = xdrvSlicIsOffhook( casDriver->slicDriver );

   if( state != casHist->currentState )
   {
      /* Inform APM/HVG of change in hook status */
      if( state == XDRV_CAS_OFFHOOK )
      {
         hvgUpdateHookStatus( casDriver->apmDriver, casDriver->chan, XDRV_TRUE );
      }
      else
      {
         hvgUpdateHookStatus( casDriver->apmDriver, casDriver->chan, XDRV_FALSE );
      }
      /* Update Hookstate LED */
      xdrvSlicLedControl( casDriver->slicDriver, state );
  }

   /* Update state */
   casHist->currentState = state;
   }

   return ( state );
}

/*
*****************************************************************************
** FUNCTION:    xdrvCasEnableFastSlicStateMode
**
** PURPOSE:     Enable/disable the CAS driver's ability to perform
**              fast SLIC state mode changes on hook state changes.
**
** PARAMETERS:  casDriver - CAS driver structure for this device
**              enable - enable/disable flag for fast SLIC state mode changes
**
** RETURNS:     0 on success, else 1
**
*****************************************************************************
*/
int xdrvCasEnableFastSlicStateMode( XDRV_CAS *casDriver, XDRV_BOOL enable )
{
   if( !casDriver->bDrvEnabled )
   {
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "CAS driver disabled - can not set fast SLIC state mode!!!"));
      return ( -2 );
   }

   /* Update the CAS driver's state variable that controls whether
   ** fast SLIC state mode changes are currently enabled */
   casDriver->bFastSlicStateModeEnabled = enable;

   return ( 0 );
}
