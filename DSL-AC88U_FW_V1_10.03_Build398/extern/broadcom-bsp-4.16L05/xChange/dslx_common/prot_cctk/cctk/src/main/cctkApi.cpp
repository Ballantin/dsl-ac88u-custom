/**********************************************************************************
** Copyright (c) 2007-2012 Broadcom Corporation
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This file implements the CCTK API.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkFramework.h>
#include <cctkInc.h>

/* ---- Public Variables -------------------------------------------------------- */
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
CCTKTRACEHDLR gpCctkTraceCb = NULL;
#endif
#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
STraceNode gCctkTraceNode;
#endif

CCTKEVTCB gCctkEvtCb;

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
/* Jerry note: To be moved to the Framework object. */
MX_NS CctkFramework *gpCctkFramework;

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */
#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
extern void tkmMxTraceHandler( EMxTraceLevel eLevel,
                               uint32_t uTraceUniqueId,
                               const char *pszTrace,
                               int sMsgSize );
#endif

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   cctkStartup
**
**  PURPOSE:    Initializes and starts up CCTK.
**
**  PARAMETERS:
**              evtCb - Event "callback" function pointer.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
**              - This function also initializes internal queue, thread, framework
**                and the SIP stack.
***********************************************************************************/
CCTKSTATUS cctkStartup( CCTKEVTCB evtcb )
{
   /* No trace is added here because M5T trace utility has not been enabled yet.
   ** Trace can be added once CCTK custom tracing support is implemented.
   */

   /* --- Internal validity checks --- */
   /* Check if the base function event ID is big enough */
   BOS_SINT32 sFuncBase = CCTKEVT_FN_BASE;
   CCTK_ASSERT( sFuncBase > eCCTKEVT_MAX );

   /* Check if status code has mis-aligned */
   BOS_SINT32 sCodeSize =
      ( eCCTKSTATUS_SUCCESS_END - eCCTKSTATUS_SUCCESS_START + 1 ) +
      ( eCCTKSTATUS_FAIL_END - eCCTKSTATUS_FAIL_START + 1);
   CCTK_ASSERT( sCodeSize == eCCTKSTATUS_MAX );
   /* --- Internal validity checks (END) --- */

   if ( gpCctkFramework != NULL )
   {
      return eCCTKSTATUS_BADSTATE;
   }

   if ( evtcb == NULL )
   {
      return eCCTKSTATUS_BADPARM;
   }

   /* Initialize UASSP and its dependencies.
   ** NOTE: Initialize here to activate the memory allocator in the M5T Framework.
   */
   mxt_result res = MX_NS CSceUaSspInitializer::Initialize();
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( 
         "cctkStartup: Error initializing UASSP and its dependencies (%d)", res );
      return eCCTKSTATUS_NORSC;
   }

   /* Initialize CCTK framework */
   gpCctkFramework = BRCM_NEW( MX_NS CctkFramework )();
   CCTK_ASSERT( gpCctkFramework != NULL );
   if ( gpCctkFramework == NULL )
   {
      return eCCTKSTATUS_NORSC;
   }

   /* Check CCTK framework initialization status */
   CCTKSTATUS eRes = gpCctkFramework->getInitStatus();
   if ( CCTKSTATUS_IS_FAIL( eRes ) )
   {
      CCTK_TRACEI1( "cctkStartup: Error initializing CCTK Framework (%d)", eRes );

      gpCctkFramework->Release();
      gpCctkFramework = NULL;

      return eRes;
   }

   /* Assign the event callback for CCTK to update the application */
   gCctkEvtCb = evtcb;

   /* Startup CCTK framework */
   eRes = gpCctkFramework->startUp();
   if ( CCTKSTATUS_IS_FAIL( eRes ) )
   {
      CCTK_TRACEI1( "cctkStartup: Error starting up CCTK Framework (%d)", eRes );

      BRCM_DELETE( gpCctkFramework );
      gpCctkFramework = NULL;

      return eRes;
   }

   CCTK_TRACEI0( "cctkStartup: Startup complete." );

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cctkShutdown
**
**  PURPOSE:    Shuts down CCTK.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
**              - This function also shuts down internal queue, thread, framework
**                and the SIP stack.
**              - Termination is immediate. All resources are destroyed forcefully.
**                Application is responsible for taking care of call signalling
**                clean-ups such as by calling cctkReset() before calling this API.
***********************************************************************************/
CCTKSTATUS cctkShutdown( void )
{
   CCTK_TRACED0( "cctkShutdown()" );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkShutdown: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   /* Shut down CCTK */
   CCTKSTATUS eStatus = gpCctkFramework->shutDown();
   if ( CCTKSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "cctkShutdown: Error shutting down CCTK (%d).", eStatus );
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) || eStatus == eCCTKSTATUS_RSCLEAK )
   {
      /* SCE and SIP stack have been successfully shut down. 
      ** Release Main Framework.
      */
      MX_NS CBinarySemaphore semRelease;
      gpCctkFramework->release( &semRelease );
      semRelease.Wait();
      gpCctkFramework = NULL;

      /* Finalize all M5T modules only if M5T SIP-UA has been shut down
      ** successfully.
      */
      if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
      {
         /* Finalize UASSP and its dependencies */
         MX_NS CSceUaSspInitializer::Finalize();
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   cctkReset
**
**  PURPOSE:    Cleans up CCTK resources.
**
**  PARAMETERS:
**              eRstMode - Reset mode to be applied for this particular action.
***
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkReset( CCTKRSTMODE eRstMode )
{
   CCTK_TRACED1( "cctkReset(%d)", eRstMode );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkReset: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   /* Reset all CCTK resources */
   CCTKSTATUS eStatus = gpCctkFramework->reset( eRstMode );
   if ( CCTKSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "cctkReset: Error resetting CCTK resources (%d).", eStatus );
      return eStatus;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cctkConfig
**
**  PURPOSE:    Configure CCTK after startup.
**
**  PARAMETERS:
**              pCfg - Configuration block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - API can be called multiple times to reconfigure CCTK.
**              - Application is responsible for ensuring CCTK does not have
**                calls in progress or user registered. App must take down all
**                calls and registrations such as by calling cctkReset() before
**                calling this API. Error will be returned otherwise.
***********************************************************************************/
CCTKSTATUS cctkConfig( const CCTKCFG *pCfg )
{
   CCTK_TRACED1( "cctkConfig(%p)", pCfg );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkConfig: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( pCfg == NULL )
   {
      CCTK_TRACEI0( "cctkConfig: NULL config pointer." );
      return eCCTKSTATUS_BADPARM;
   }

   CCTKSTATUS eStatus = gpCctkFramework->config( pCfg );
   if ( CCTKSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "cctkConfig: Error configuring CCTK (%d)", eStatus );
      return eStatus;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cctkSetParm
**
**  PURPOSE:    Set parameter for id.
**
**  PARAMETERS:
**              uId     - Object id to set parameter.
**              eParm   - Parameter id.
**              pVal    - Parameter value.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - Can be used when CCTK is enabled.
***********************************************************************************/
CCTKSTATUS cctkSetParm( BOS_UINT32 uId, CCTKPARM eParm, const void *pVal )
{
   CCTK_TRACED3( "cctkSetParm(%lu, %d, %p)", uId, eParm, pVal );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkSetParm: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( !CCTK_IS_CFGPARM( eParm ) || pVal == NULL )
   {
      CCTK_TRACEI0( "cctkSetParm: Bad parameters." );
      return eCCTKSTATUS_BADPARM;
   }

   return gpCctkFramework->setParm( uId, eParm, pVal );
}

/***********************************************************************************
**  FUNCTION:   cctkGetParm
**
**  PURPOSE:    Get parameter for id.
**
**  PARAMETERS:
**              uId     - Object id to set parameter.
**              eParm   - Parameter id.
**              pVal    - Parameter value.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
**              - Can be used when CCTK is enabled.
***********************************************************************************/
CCTKSTATUS cctkGetParm( BOS_UINT32 uId, CCTKPARM eParm, void *pVal )
{
   CCTK_TRACED3( "cctkGetParm(%lu, %d, %p)", uId, eParm, pVal );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkGetParm: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( pVal == NULL || eParm < 0 || eParm >= eCCTKPARM_MAX )
   {
      CCTK_TRACEI0( "cctkGetParm: Bad parameters." );
      return eCCTKSTATUS_BADPARM;
   }

   return gpCctkFramework->getParm( uId, eParm, pVal );
}

/***********************************************************************************
**  FUNCTION:   cctkUserAdd
**
**  PURPOSE:    Add a user to the system.
**
**  PARAMETERS:
**              pUid - User id (id created on return).
**              pCfg - User config block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkUserAdd( BOS_UINT32 *pUid, const CCTKUSRCFG *pCfg )
{
   CCTK_TRACED2( "cctkUserAdd(%p, %p)", pUid, pCfg );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkUserAdd: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( pUid == NULL || pCfg == NULL )
   {
      CCTK_TRACEI0( "cctkUserAdd: Bad parameters." );
      return eCCTKSTATUS_BADPARM;
   }

   /* Create a unique user id */
   *pUid = tkmGenerateId();

   return gpCctkFramework->userAdd( *pUid, pCfg );
}

/***********************************************************************************
**  FUNCTION:   cctkUserRegister
**
**  PURPOSE:    Sends a REGISTER to register the configured user.
**
**  PARAMETERS:
**              uUid - User id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkUserRegister( BOS_UINT32 uUid )
{
   CCTK_TRACED1( "cctkUserRegister(0x%lX)", uUid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkUserRegister: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->userRegister( uUid );
}

/***********************************************************************************
**  FUNCTION:   cctkUserUnregister
**
**  PURPOSE:    Send a REGISTER with expires=0 to unregister the contact.
**
**  PARAMETERS:
**              uUid    - User id.
**              bClear  - Set clear to TRUE to unregister all registrar-cached
**                        contacts associated with the Address of Record (AoR).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkUserUnregister( BOS_UINT32 uUid, BOS_BOOL bClear )
{
   CCTK_TRACED2( "cctkUserUnregister(0x%lX, %d)", uUid, bClear );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkUserUnregister: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->userUnregister( uUid, bClear );
}

/***********************************************************************************
**  FUNCTION:   cctkUserRemove
**
**  PURPOSE:    Remove the user from the system.
**
**  PARAMETERS:
**              uUid - User id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The uUid must not be used on successful return.
***********************************************************************************/
CCTKSTATUS cctkUserRemove( BOS_UINT32 uUid )
{
   CCTK_TRACED1( "cctkUserRemove(0x%lX)", uUid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkUserRemove: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->userRemove( uUid );
}

/***********************************************************************************
**  FUNCTION:   cctkEventCreate
**
**  PURPOSE:    Create an event object for subscription.
**
**  PARAMETERS:
**              pEid       - Subscription event id (id created on return).
**              uUid       - User id for the subscription.
**              eSubtype   - Subscription event type (e.g. reg-event).
**              uExpTime   - Subscription expiration time (must be non-zero).
**              pcTo       - Subscription destination address (URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkEventCreate( BOS_UINT32 *pEid,
                            BOS_UINT32 uUid,
                            CCTKSUBEVT eSubtype,
                            BOS_UINT32 uExpTime,
                            const BOS_UINT8 *pcTo )
{
   CCTK_TRACED4( 
      "cctkEventCreate(0x%lX, %d, %lu, %p)", uUid, eSubtype, uExpTime, pcTo );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkEventCreate: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( pEid == NULL || uExpTime == 0 || pcTo == NULL )
   {
      CCTK_TRACEI0( "cctkEventCreate: Bad parameters." );
      return eCCTKSTATUS_BADPARM;
   }

   /* Create a unique event id */
   *pEid = tkmGenerateId();

   return gpCctkFramework->eventCreate( *pEid, uUid, eSubtype, uExpTime, pcTo );
}

/***********************************************************************************
**  FUNCTION:   cctkEventSubscribe
**
**  PURPOSE:    Send a SUBSCRIBE to the event service.
**
**  PARAMETERS:
**              uEid - Event id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkEventSubscribe( BOS_UINT32 uEid )
{
   CCTK_TRACED1( "cctkEventSubscribe(0x%lX)", uEid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkEventSubscribe: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->eventSubscribe( uEid );
}

/***********************************************************************************
**  FUNCTION:   cctkEventUnsubscribe
**
**  PURPOSE:    Send a SUBSCRIBE with Expires equal to 0 to unscribe from
**              the event service.
**
**  PARAMETERS:
**              uEid - Event id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkEventUnsubscribe( BOS_UINT32 uEid )
{
   CCTK_TRACED1( "cctkEventUnsubscribe(0x%lX)", uEid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkEventUnsubscribe: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->eventUnsubscribe( uEid );
}

/***********************************************************************************
**  FUNCTION:   cctkEventDelete
**
**  PURPOSE:    Unsubscribe and delete the (subscribed) event.
**
**  PARAMETERS:
**              uEid - Event id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The uEid must not be used on return.
***********************************************************************************/
CCTKSTATUS cctkEventDelete( BOS_UINT32 uEid )
{
   CCTK_TRACED1( "cctkEventDelete(0x%lX)", uEid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkEventDelete: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->eventDelete( uEid );
}

/***********************************************************************************
**  FUNCTION:   cctkCallCreate
**
**  PURPOSE:    Creates a handle for call operation.
**
**  PARAMETERS:
**              pCid - Call id (created on return).
**              uUid - User id for the call.
**              pcTo - Call destination address (dial-string or URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallCreate( BOS_UINT32 *pCid,
                           BOS_UINT32 uUid,
                           const BOS_UINT8 *pcTo )
{
   CCTK_TRACED3( "cctkCallCreate(%p, 0x%lX, %p)", pCid, uUid, pcTo );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallCreate: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( pCid == NULL || pcTo == NULL )
   {
      CCTK_TRACEI0( "cctkCallCreate: Bad parameters." );
      return eCCTKSTATUS_BADPARM;
   }

   /* Create a unique call id */
   *pCid = tkmGenerateId();

   return gpCctkFramework->callCreate( *pCid, uUid, pcTo );
}

/***********************************************************************************
**  FUNCTION:   cctkCallMake
**
**  PURPOSE:    Initiate a call (Send an INVITE).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallMake( BOS_UINT32 uCid )
{
   CCTK_TRACED1( "cctkCallMake(0x%lX)", uCid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallMake: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->callMake( uCid );
}

/***********************************************************************************
**  FUNCTION:   cctkCallAnswer
**
**  PURPOSE:    Answer the call (Send a 200 OK for the INVITE).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallAnswer( BOS_UINT32 uCid )
{
   CCTK_TRACED1( "cctkCallAnswer(0x%lX)", uCid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallAnswer: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->callAnswer( uCid );
}

/***********************************************************************************
**  FUNCTION:   cctkCallForward
**
**  PURPOSE:    Forward the call (Send a 3xx response for the INVITE).
**
**  PARAMETERS:
**              uCid    - Call id.
**              pcFwdTo - Forward-to address (dial-string or URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallForward( BOS_UINT32 uCid, const BOS_UINT8 *pcFwdTo )
{
   CCTK_TRACED2( "cctkCallForward(0x%lX, %p)", uCid, pcFwdTo );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallForward: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( pcFwdTo == NULL )
   {
      CCTK_TRACEI0( "cctkCallForward: NULL destination pointer." );
      return eCCTKSTATUS_BADPARM;
   }

   return gpCctkFramework->callForward( uCid, pcFwdTo );
}

/***********************************************************************************
**  FUNCTION:   cctkCallHold
**
**  PURPOSE:    Hold the current call (Send a re-INVITE with SDP/a=sendonly).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallHold( BOS_UINT32 uCid )
{
   CCTK_TRACED1( "cctkCallHold(0x%lX)", uCid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallHold: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->callHold( uCid );
}

/***********************************************************************************
**  FUNCTION:   cctkCallUnhold
**
**  PURPOSE:    Unhold the current call (Send a re-INVITE with SDP/a=sendonly).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallUnhold( BOS_UINT32 uCid )
{
   CCTK_TRACED1( "cctkCallUnhold(0x%lX)", uCid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallUnhold: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->callUnhold( uCid );
}

/***********************************************************************************
**  FUNCTION:   cctkCallTransfer
**
**  PURPOSE:    Attended call transfer (REFER with replaces).
**
**  PARAMETERS:
**              uCid    - Call id for the call to the transferer.
**              uXcid   - Call id for the call to transfer to.
**              bNoCallTermination - Boolean to indicate whether to terminate the calls
**                                   upon successful transfer. When set to BOS_FALSE, results in 
**                                   standard call transfer behaviour with call termination
**                                   done by CCTK upon successful transfer. When set to BOS_TRUE,
**                                   results in special tranfer behaviour that does not terminate 
**                                   any calls as needed for server-based 
**                                   transfer scenario.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallTransfer( BOS_UINT32 uCid, BOS_UINT32 uXcid, BOS_BOOL bNoCallTermination )
{
   CCTK_TRACED2( "cctkCallTransfer(0x%lX, 0x%lX)", uCid, uXcid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallTransfer: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->callTransfer( uCid, uXcid, bNoCallTermination );
}

/***********************************************************************************
**  FUNCTION:   cctkCallTransferBlind
**
**  PURPOSE:    Unattended call transfer (REFER no replaces).
**
**  PARAMETERS:
**              uCid   - Call id for the call to the transferer.
**              pcAddr - Transfer-to address (dial-string or URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallTransferBlind( BOS_UINT32 uCid, const BOS_UINT8 *pcAddr )
{
   CCTK_TRACED2( "cctkCallTransferBlind(0x%lX, %p)", uCid, pcAddr );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallTransferBlind: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( pcAddr == NULL )
   {
      CCTK_TRACEI0( "cctkCallTransferBlind: NULL address pointer." );
      return eCCTKSTATUS_BADPARM;
   }

   return gpCctkFramework->callTransferBlind( uCid, pcAddr );
}

/***********************************************************************************
**  FUNCTION:   cctkCallMediaNotify
**
**  PURPOSE:    Update remote party that a change in media has taken place.
**
**  PARAMETERS:
**              uCid - Call id.
**              eReason - Reason for the update.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - This API triggers a SDP (offer) to be sent.
***********************************************************************************/
CCTKSTATUS cctkCallMediaNotify( BOS_UINT32 uCid, CCTKMUPDRSN eReason )
{
   CCTK_TRACED1( "cctkCallMediaNotify(0x%lX)", uCid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallMediaNotify: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( eReason >= eCCTKMUPDRSN_MAX )
   {
      CCTK_TRACEI1( "cctkCallMediaNotify: Bad reason %d.", eReason );
      return eCCTKSTATUS_BADPARM;
   }

   return gpCctkFramework->callMediaNotify( uCid, eReason );
}

/***********************************************************************************
**  FUNCTION:   cctkCallDelete
**
**  PURPOSE:    Delete allocated call resources.
**
**  PARAMETERS:
**              uCid - Call id.
**              eReason - Reason for the call deletion.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - Send BYE / terminate the current call.
**              - The uCid must not be used on return.
***********************************************************************************/
CCTKSTATUS cctkCallDelete( BOS_UINT32 uCid, CCTKCALLDELRSN eReason )
{
   CCTK_TRACED1( "cctkCallDelete(0x%lX)", uCid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallDelete: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( eReason >= eCCTKCALLDELRSN_MAX )
   {
      CCTK_TRACEI1( "cctkCallDelete: Bad reason %d.", eReason );
      return eCCTKSTATUS_BADPARM;
   }

   return gpCctkFramework->callDelete( uCid, eReason );
}

/***********************************************************************************
**  FUNCTION:   cctkCallDtmfInfo
**
**  PURPOSE:    Issues an information message about DTMF detected on the call
**              resource.
**
**  PARAMETERS:
**              uCid - Call id.
**              uDtmf - DTMF identifier.
**              uDur - Duration of the DTMF.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallDtmfInfo( BOS_UINT32 uCid, BOS_UINT32 uDtmf, BOS_UINT32 uDur )
{
   CCTK_TRACED1( "cctkCallDtmfInfo(0x%lX)", uCid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallDtmfInfo: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->callDtmfInfo( uCid, uDtmf, uDur );
}

/***********************************************************************************
**  FUNCTION:   cctkCallFlashInfo
**
**  PURPOSE:    Issues an information message about hook flash detected on the call
**              resource.
**
**  PARAMETERS:
**              uCid - Call id.
**              pcHfMimeType - Content-Type of the INFO message
**              pcHfContent  - Content of the INFO message
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallFlashInfo( BOS_UINT32 uCid,
                              const BOS_UINT8 *pcHfMimeType,
                              const BOS_UINT8 *pcHfContent )
{
   CCTK_TRACED3( "cctkCallFlashInfo(0x%lX), %p, %p", uCid, pcHfMimeType, pcHfContent );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallFlashInfo: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->callFlashInfo( uCid, pcHfMimeType, pcHfContent );
}

/***********************************************************************************
**  FUNCTION:   cctkCallPrivInfo
**
**  PURPOSE:    Updates the privacy information for the current call.
**
**  PARAMETERS:
**              uCid - Call id.
**              pPrivCfg - Privacy config.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
CCTKSTATUS cctkCallPrivInfo( BOS_UINT32 uCid, const CCTKPRIVCFG *pPrivCfg )
{
   CCTK_TRACED2( "cctkCallPrivInfo(0x%lX, %p)", uCid, pPrivCfg );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallPrivInfo: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }
   if ( pPrivCfg == NULL )
   {
      CCTK_TRACEI0( "cctkCallPrivInfo: NULL privacy config pointer." );
      return eCCTKSTATUS_BADPARM;
   }

   return gpCctkFramework->callPrivInfo( uCid, pPrivCfg );
}

/***********************************************************************************
**  FUNCTION:   cctkMemStatusShow
**
**  PURPOSE:    External API to show the memory allocation status.
**
**  PARAMETERS:
**              bCctkOnly     - True to retrieve the amount of memory allocated
**                              by CCTK.
**              bTraceTable   - True to trace the entire memory allocation table.
**                              If bCctkOnly is enabled, only the memory blocks
**                              allocated by CCTK are traced.
**                              Not used when CCTK_TRACELVL_INFO is disabled.
**
**  RETURNS:    Nothing.
**
**  NOTES:      - This function is asynchronous.
**              - This is a CCTK utility function which is not exposed in the
**                CCTK API (i.e. cctkApi.h). The app has to link in this function
**                (by using extern) for making use of it.
***********************************************************************************/
void cctkMemStatusShow( BOS_BOOL bCctkOnly, BOS_BOOL bTraceTable )
{
#if CCTKCFG_MEMORY_MONITORING && CCTKCFG_DEBUG
   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkMemStatusShow: CCTK has not started up yet." );
   }
   else
   {
      /* NOTE: we ignore the return status here as it is not important */
      gpCctkFramework->showMemStatus( bCctkOnly, bTraceTable );
   }
#else
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( bCctkOnly );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( bTraceTable );

   CCTK_TRACEI0( "cctkMemStatusShow: Feature not enabled." );
#endif /* #if CCTKCFG_MEMORY_MONITORING && CCTKCFG_DEBUG */
}

/***********************************************************************************
**  FUNCTION:   cctkStatusShow
**
**  PURPOSE:    Show the data status information for those data under CCTK
**              control.
**
**  PARAMETERS: None
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
void cctkStatusShow( void )
{
#if CCTKCFG_APPLICATION_STATUS
   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkStatusShow: CCTK has not started up yet." );
   }
   else
   {
      /* NOTE: we ignore the return status here as it is not important */
      gpCctkFramework->showStatus();
   }
#else
   CCTK_TRACEI0( "cctkStatusShow: Feature not enabled." );
#endif /* #if CCTKCFG_APPLICATION_STATUS */
}


/***********************************************************************************
**  FUNCTION:   cctkPublish
**
**  PURPOSE:    Publishes information from the application to the remote end.
**
**  PARAMETERS:
**              uUsrGuid - The user GUID associated with this action.
**              uResGuid - The resource GUID associated with this action.
**              eType    - The type of publication.
**              pcTo     - Where to publish.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The type (eType) passed as parameter is used to determine how
**                to interpret the resource GUID.  For example, if the type is
**                'call-metrics', then the GUID must be the call identifier of
**                interest.
**              - This API is to be used for publication of out-of-dialog
**                information which will not affect the resource state it applies
**                to in any ways.  It simply uses the resource as source of
**                necessary information in order to know what, how and to whom the
**                publication is to be made.  The resource target reserves the
**                right to refuse/reject a publication at any time.
***********************************************************************************/
CCTKSTATUS cctkPublish( BOS_UINT32 uUsrGuid,
                        BOS_UINT32 uResGuid,
                        CCTKPUBLISH eType, 
                        const BOS_UINT8 *pcTo )
{
   CCTK_TRACED2( "cctkPublish(0x%lX, 0x%lX)", uUsrGuid, uResGuid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkPublish: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( eType >= eCCTKPUBLISH_MAX )
   {
      CCTK_TRACEI1( "cctkPublish: Bad event %d.", eType );
      return eCCTKSTATUS_BADPARM;
   }

   if ( pcTo == NULL )
   {
      CCTK_TRACEI0( "cctkPublish: Bad target." );
      return eCCTKSTATUS_BADPARM;
   }

   return gpCctkFramework->infoPublish( uUsrGuid, uResGuid, eType, pcTo );
}


/***********************************************************************************
**  FUNCTION:   cctkServiceControl
**
**  PURPOSE:    Controls a service that is owned by the CCTK.
**
**  PARAMETERS:
**              uUsrGuid - The user GUID associated with this action.
**              eType    - The type of service to control.
**              bEnable  - Whether to enable or disable the service.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The type (eType) passed as parameter is used to determine what
**                specific action is to be taken for enabling or disabling the
**                service.
**              - This API is to be used for control over a service that is owned
**                by CCTK and on which the outside application has no visibility
**                or direct control, instead it relies on the CCTK user module to
**                apply the needed action for the specific service.
***********************************************************************************/
CCTKSTATUS cctkServiceControl( BOS_UINT32 uUsrGuid,
                               CCTKSUBEVT eType,
                               BOS_BOOL bEnable )
{
   CCTK_TRACED1( "cctkServiceControl(0x%lX)", uUsrGuid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkServiceControl: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( eType >= eCCTKSUBEVT_MAX )
   {
      CCTK_TRACEI1( "cctkServiceControl: Bad event %d.", eType );
      return eCCTKSTATUS_BADPARM;
   }

   return gpCctkFramework->serviceControl( uUsrGuid, eType, bEnable );
}


#if CCTKCFG_REMOTE_USER_CONTROL
/***********************************************************************************
**  FUNCTION:   cctkCallProxy
**
**  PURPOSE:    Proxy a call resource.
**
**  PARAMETERS:
**              uCid - Call id.
**              uPid - Call id of the proxy resource.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - Forward the current call on to the network and proxy the
**                result to the user.
**              - The uCid must not be used on return.
***********************************************************************************/
CCTKSTATUS cctkCallProxy( BOS_UINT32 uCid, BOS_UINT32 uPid )
{
   CCTK_TRACED2( "cctkCallProxy(0x%lX, 0x%lX)", uCid, uPid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkCallProxy: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->callProxy( uCid, uPid );
}
#endif /* CCTKCFG_REMOTE_USER_CONTROL */


#if CCTKCFG_MESSAGE_PASS_THROUGH
/***********************************************************************************
**  FUNCTION:   cctkPassThruMsg
**
**  PURPOSE:    Issue a pass through message from the application via a MESSAGE
**              method.
**
**  PARAMETERS:
**              puResGuid - The resource GUID created to manage this action.
**              uUsrGuid  - The user GUID associated with this action.
**              pcTo      - Where to send the message to.
**              pcBody    - The body of the message to be happended (transparent).
**              uSize     - The size of the body information to be happended.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The application must keep reference of the returned GUID
**                (puResGuid) to be used later on when final status for this
**                message is received.
**              - Once final response is received and passed to the application
**                the resource is terminated and the application may release the
**                GUID.  Further action on the GUID will have no effect.
***********************************************************************************/
CCTKSTATUS cctkPassThruMsg( BOS_UINT32 *puResGuid,
                            BOS_UINT32 uUsrGuid,
                            const BOS_UINT8 *pcTo,
                            const BOS_UINT8 *pcBody,
                            const BOS_UINT32 uSize )
{
   CCTK_TRACED1( "cctkPassThruMsg(0x%lX)", uUsrGuid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkPassThruMsg: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   if ( puResGuid == NULL )
   {
      CCTK_TRACEI0( "cctkPassThruMsg: Bad parameter." );
      return eCCTKSTATUS_BADPARM;
   }

   if ( pcTo == NULL )
   {
      CCTK_TRACEI0( "cctkPassThruMsg: Bad target." );
      return eCCTKSTATUS_BADPARM;
   }

   if ( (pcBody == NULL) || !uSize )
   {
      CCTK_TRACEI0( "cctkPassThruMsg: Bad user data." );
      return eCCTKSTATUS_BADPARM;
   }

   /* Create a unique request identifier.
   */
   *puResGuid = tkmGenerateId();

   return gpCctkFramework->passThruMsg( *puResGuid,
                                        uUsrGuid,
                                        pcTo,
                                        pcBody,
                                        uSize );
}


/***********************************************************************************
**  FUNCTION:   cctkPassThruRsp
**
**  PURPOSE:    Issue a final response to a pass through request received prior.
**
**  PARAMETERS:
**              uResGuid  - The resource GUID to associate this response with.
**              uFinalRsp - The final response indication.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is asynchronous.
**              - The application MUST pass the response in the form of a
**                2xx/4xx/5xx/6xx final SIP response.
**              - Provisional (1xx/3xx) responses are unacceptable and will
**                be remapped to a default 200 response if invoked.
**              - Following this API call, the application may release the GUID
**                associated with the pass through request as it will be removed
**                from internal knowledge and will be invalid.
***********************************************************************************/
CCTKSTATUS cctkPassThruRsp( BOS_UINT32 uResGuid,
                            BOS_UINT32 uFinalRsp )
{
   CCTK_TRACED2( "cctkPassThruRsp(0x%lX, 0x%lX)", uResGuid, uFinalRsp );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "cctkPassThruRsp: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->passThruRsp( uResGuid, uFinalRsp );
}
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */


/***********************************************************************************
**  FUNCTION:   cctkGenerateId
**
**  PURPOSE:    Generate a unique object id for identification.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Returns a global unique identifier or an invalid value if
**              such identifier cannot be selected.
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
BOS_UINT32 cctkGenerateId( void )
{
   CCTK_TRACED0( "cctkGenerateId()" );

   return ( tkmGenerateId() );
}


/***********************************************************************************
**  FUNCTION:   tkmKaFailure
**
**  PURPOSE:    Keep alive event failure from the Keep Alive module (NsNatKa).
**
**  PARAMETERS:
**              uUid    - User id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
***********************************************************************************/
CCTKSTATUS tkmKaFailure( BOS_UINT32 uUid )
{
   CCTK_TRACED1( "tkmKaFailure(0x%lX)", uUid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "tkmKaFailure: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->kaFailure( uUid );
}

/***********************************************************************************
**  FUNCTION:   tkmKaSuccess
**
**  PURPOSE:    Keep alive event success from the Keep Alive module (NsNatKa).
**
**  PARAMETERS:
**              uUid    - User id.
**              eType   - The type of transport used for the keep alive.
**              puAddr  - The address of the keep alive remote.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
***********************************************************************************/
CCTKSTATUS tkmKaSuccess( BOS_UINT32 uUid, TKMTRANSPORT eType, BOS_UINT8 *puAddr )
{
   CCTK_TRACED1( "tkmKaSuccess(0x%lX)", uUid );

   if ( gpCctkFramework == NULL )
   {
      CCTK_TRACEI0( "tkmKaSuccess: CCTK has not started up yet." );
      return eCCTKSTATUS_NOTINIT;
   }

   return gpCctkFramework->kaSuccess( uUid, eType, puAddr );
}

/***********************************************************************************
**  FUNCTION:   tkmEvent
**
**  PURPOSE:    Events to be passed to the application.
**
**  PARAMETERS:
**              uUid    - User id.
**              uCid    - Call id.
**              eEvt    - Event id.
**              pData   - Event data.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
***********************************************************************************/
CCTKSTATUS tkmEvent( BOS_UINT32 uUid, BOS_UINT32 uCid, CCTKEVT eEvt, void *pData )
{
   CCTK_TRACED4( "tkmEvent(0x%lX, 0x%lX, %d, %p)", uUid, uCid, eEvt, pData );

   /* Inform the Call Engine module of the event that is being passed out
   ** to the application.  Call Engine may trigger some processing on those
   ** events as well since those may be coming from different source within the
   ** CCTK that Call Engine is not aware of.
   */
   ceOutEvtInfo ( uUid, uCid, eEvt );

   CCTK_EVTCB(( eEvt, uUid, uCid, pData ));

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   tkmGenerateId
**
**  PURPOSE:    Generate a unique object id for identification.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Returns a global unique identifier or an invalid value if
**              such identifier cannot be selected.
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
BOS_UINT32 tkmGenerateId( void )
{
   CCTK_TRACED0( "tkmGenerateId()" );

   return ( ceGuid() );
}

/***********************************************************************************
**  FUNCTION:   tkmMsgPost
**
**  PURPOSE:    Post a message to the CCTK message queue. pMsgcb will be called
**              when the core thread is ready to process this message.
**
**  PARAMETERS:
**              bWait      - True to post a message synchronously,
**                           i.e. this function is blocked until the message has
**                           been processed.
**              uId        - Message ID.
**              pData      - Optional data
**              msgCb      - Callback function to call when message is ready.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
***********************************************************************************/
CCTKSTATUS tkmMsgPost( BOS_BOOL bWait, BOS_UINT32 uId, void *pData, TKMMSGCB msgCb )
{
   if ( msgCb == NULL )
   {
      CCTK_TRACEI0( "tkmMsgPost: NULL callback function pointer." );
      return eCCTKSTATUS_BADPARM;
   }

   return gpCctkFramework->postMsg( bWait, uId, pData, msgCb );
}

/***********************************************************************************
**  FUNCTION:   tkmSetParm
**
**  PURPOSE:    Set parameter for id synchronously.
**
**  PARAMETERS:
**              uId        - Object id to set parameter.
**              eParm      - Parameter id.
**              pVal       - Parameter value.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
**              - This function MUST be called within the stack's core thread
**                context; thus, this function MUST only be used by CCTK modules.
***********************************************************************************/
CCTKSTATUS tkmSetParm( BOS_UINT32 uId, CCTKPARM eParm, const void *pVal )
{
   CCTK_TRACED3( "tkmSetParm(0x%lX, %d, %p)", uId, eParm, pVal );

   if ( !CCTK_IS_CFGPARM( eParm ) || pVal == NULL )
   {
      CCTK_TRACEI0( "tkmSetParm: Bad parameters." );
      return eCCTKSTATUS_BADPARM;
   }

   CCTK_ASSERT( gpCctkFramework != NULL );

   return gpCctkFramework->setParmHelper( uId, eParm, pVal );
}

/***********************************************************************************
**  FUNCTION:   tkmTimerStart
**
**  PURPOSE:    Start timer, tmrcb will be called at msec.
**
**  PARAMETERS:
**              pId        - Object id (created on return).
**              uMsec      - Timer in msec.
**              bRecurring - Indicates whether the timer is recurring.
**              pData      - Optional data
**              pTmrcb     - Callback function to call when timer is up.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
***********************************************************************************/
CCTKSTATUS tkmTimerStart( BOS_UINT32 *pId,
                          BOS_UINT32 uMsec,
                          BOS_BOOL bRecurring,
                          void *pData,
                          TKMMSGCB tmrCb )
{
   CCTK_TRACED4( "tkmTimerStart(%p, %lu, %d, %p)", pId, uMsec, bRecurring, pData );

   /* Generate unique timer id */
   *pId = tkmGenerateId();

   return gpCctkFramework->startTimer( *pId, uMsec, bRecurring, pData, tmrCb );
}

/***********************************************************************************
**  FUNCTION:   tkmTimerStop
**
**  PURPOSE:    Stop timer.
**
**  PARAMETERS:
**              uId - Object id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
***********************************************************************************/
CCTKSTATUS tkmTimerStop( BOS_UINT32 uId )
{
   CCTK_TRACED1( "tkmTimerStop(0x%lX)", uId );

   return gpCctkFramework->stopTimer( uId );
}

/***********************************************************************************
**  FUNCTION:   tkmResetCont
**
**  PURPOSE:    Continue reset of the application.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      - This function is synchronous.
***********************************************************************************/
void tkmResetCont( void )
{
   CCTK_TRACED0( "tkmResetCont");

   return gpCctkFramework->internalResetCont();
}

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )

/***********************************************************************************
**  FUNCTION:   tkmMxTraceHandler
**
**  PURPOSE:    Handler function for outputing traces from M5T tracing utility.
**
**  PARAMETERS:
**              eLevel           - Trace level.
**              uTraceUniqueId   - Trace ID.
**              pszTrace         - A pointer to a null-terminated trace message.
**              sMsgSize         - Trace message length, excluding null-terminator.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void tkmMxTraceHandler( EMxTraceLevel eLevel,
                        uint32_t uTraceUniqueId,
                        const char *pszTrace,
                        int sMsgSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( sMsgSize );

   if ( gpCctkTraceCb != NULL && pszTrace != NULL && strlen( pszTrace ) > 0 )
   {
      BOS_UINT16 uTraceLevel;
      BOS_UINT16 uTraceGroup;

      uTraceLevel = cfgCvtTraceLvlFromMx( eLevel );
      uTraceGroup = cfgCvtTraceGrpFromMx( uTraceUniqueId );
      ( *gpCctkTraceCb )( uTraceLevel,
                          uTraceGroup,
                          reinterpret_cast< const BOS_UINT8* >( pszTrace ) );
   }
}

#elif ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_CCTK )

/***********************************************************************************
**  FUNCTION:   tkmTrace
**
**  PURPOSE:    Trace formating routine.
**
**  PARAMETERS:
**              uLevel  - Trace level.
**              pFormat - Format string.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This routine should only be used when stack tracing is disabled.
***********************************************************************************/
#define TKM_TRACE_MSG_LEN 127
void tkmTrace( BOS_UINT16 uLevel, const char *pFormat, ... )
{
   if ( gpCctkTraceCb != NULL &&
        pFormat != NULL &&
        strlen( pFormat ) > 0 &&
        UTIL_BMP_IS_ENABLED( uLevel, CFG_SYS->xtl.loc.debug.trace.uLevel ) )
   {
      va_list argList;
      BOS_UINT8 cMsg[TKM_TRACE_MSG_LEN + 1];

      va_start( argList, pFormat );
      vStrPrintf( reinterpret_cast< char* >( cMsg ),
                  TKM_TRACE_MSG_LEN,
                  pFormat,
                  argList );
      cMsg[TKM_TRACE_MSG_LEN] = '\0';

      ( *gpCctkTraceCb )( uLevel, CCTK_TRACEGRP_CCTK, cMsg );
   }
}

#endif /* #if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL ) */
