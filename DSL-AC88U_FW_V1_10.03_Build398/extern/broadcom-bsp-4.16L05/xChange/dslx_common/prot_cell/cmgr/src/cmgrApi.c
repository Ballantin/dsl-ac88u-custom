/**********************************************************************************
** Copyright (c) 2010-2013 Broadcom Corporation
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
**      This file implements the Call Manager API.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>


/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
BOS_BOOL gbShutDown = BOS_TRUE;


/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmgrInit
**
** PURPOSE:    Init the Call Manager stack.
**
** PARAMETERS: statusCb - the status callback function to be registered, this
**                        callback cannot be NULL or Call Manager will
**                        not proceed initialization.  This callback will be
**                        invoked with the resulting information..
**
** RETURNS:    None.
**
** NOTE:       This starts up the Call Manager processing task and event queue.
**             Result of this action will be invoked on the status callback
**             registered interface.
**
***********************************************************************************/
void cmgrInit( CMGRSTATUSCB statusCb )
{
   /* Validate that all the mandatory callbacks are properly set.
   */
   BOS_ASSERT ( statusCb != NULL );
   gCmStatusCb = statusCb;

   gbShutDown = BOS_FALSE;

   /* Launch the Call Manager.
   */
   cmMainInit();
}

/***********************************************************************************
** FUNCTION:   cmgrDeinit
**
** PURPOSE:    Shuts down the Call Manager stack.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       This closes the Call Manager processing task and event queue.
**             Result of this action will be invoked on the status callback
**             registered interface.
**
***********************************************************************************/
void cmgrDeinit( void )
{

   /* if no shutdown has been queued, queue one now. At this point we don't
    * know if the cmgr init and startup was completed so we need to clean up
    * and shutdown completely.
    * */
   if ( !gbShutDown )
   {
      CMEVT       evt;
      BOS_TIME_MS timeout = 0;      

      gbShutDown = BOS_TRUE;

      /* Post the shutdown event.
      */
      memset ( &evt, 0, sizeof(CMEVT) );
      evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCMCMD_SHUTDOWN);

      if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
      {
         cmSignalEvt ();
      }

      /* wait for the CM state to change, then deinit */
      /* CMGR MUST notify me with statusCB */
      while ( ! ( ( cmGetState( CMGR_INVALID_HANDLE ) == eCMGRSTATUS_SHUTDOWN_COMPLETE  ) &&
                  ( cmGetState( CMGR_INVALID_HANDLE ) == eCMGRSTATUS_SHUTDOWN_FAILURE ) ) )
      {
         bosSleep(20);  
         timeout += 20;
         if ( timeout > 500 ) 
         {
            CMGRLOG_CRITICAL (( "cmgrDeinit() - shutdown did not complete after 500 ms, deiniting anyways." ));
            cmMainDeinit();
            break;
         }
         else
         {
            if ( ( cmGetState( CMGR_INVALID_HANDLE ) == eCMGRSTATUS_SHUTDOWN_COMPLETE ) ||
                 ( cmGetState( CMGR_INVALID_HANDLE ) == eCMGRSTATUS_SHUTDOWN_FAILURE ) )
            {
               cmMainDeinit();
               break;
            }
         }
      }
   }
   else
   {
      cmMainDeinit();
   }
}

/***********************************************************************************
** FUNCTION:   cmgrStartup
**
** PURPOSE:    Startups the Call Manager processing.
**
** PARAMETERS: statusCb - the status callback function to be registered, this
**                        callback cannot be NULL or Call Manager will
**                        not proceed to startup.  This callback will be
**                        invoked with the resulting information.
**             pCfg     - pointer to the configuration block containing in
**                        particular the necessary external callbacks that
**                        the Call Manager will use during its process.
**
** RETURNS:    None.
**
** NOTE:       Invoking this function will do the following:
**
**                 [I]  - Register all the necessary callbacks.
**                [II]  - Setup the Call Manager with all third
**                        party modules and setup those modules as
**                        necessary.
**               [III]  - Create and launch the Call Manager
**                        execution thread.
**                [IV]  - Signal to the application the readiness
**                        of the Call Manager to accept further
**                        commands and actions.
**
***********************************************************************************/
void cmgrStartup ( CMGRSTATUSCB statusCb, CMGRCFGBLK *pCfg )
{
   CMEVT evt;
   
   /* Validate that all the mandatory callbacks are properly set.
   */
   BOS_ASSERT ( statusCb != NULL );
   BOS_ASSERT ( pCfg != NULL );
   BOS_ASSERT ( pCfg->cmgrEptCtlCb != NULL );
   BOS_ASSERT ( pCfg->cmgrProvCb != NULL );
   BOS_ASSERT ( pCfg->cmgrSesCtlCb != NULL );

   gCmStatusCb = statusCb;
   memcpy ( &gCmCfgCb, pCfg, sizeof(CMGRCFGBLK) );

   /* Post the initial startup event.
   */
   memset ( &evt, 0, sizeof(CMEVT) );
   evt.uEvt = (BOS_UINT32) ( CMGR_EVT_CLASS__USR_CMD | eCMCMD_STARTUP );

   if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
   {
      cmSignalEvt ();
   }
}


/***********************************************************************************
** FUNCTION:   cmgrShutdown
**
** PURPOSE:    Shuts down the Call Manager processing.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       Result of this action will be invoked on the status callback
**             registered interface.
**
***********************************************************************************/
void cmgrShutdown ( void )
{
   if ( !gbShutDown )
   {
      CMEVT evt;

      gbShutDown = BOS_TRUE;

      /* Post the shutdown event.
      */
      memset ( &evt, 0, sizeof(CMEVT) );
      evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCMCMD_SHUTDOWN);

      if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
      {
         cmSignalEvt ();
      }
   }
   else
   {
      /* If we already ended the main loop, shutdown took place already, this
      ** could be some rogue application call or could be for a valid reason,
      ** either ways we should let the application know that the shutdown is a
      ** success as it may be waiting on this to proceed further.
      */
      if ( cmPhyIsStarted () )
      {
         cmPhyShutdown ();
      }

      if ( gCmStatusCb )
      {
         cmUpdateState ( CMGR_INVALID_HANDLE,
                         eCMGRSTATUS_SHUTDOWN_COMPLETE );
      }
   }
}


/*****************************************************************************
** FUNCTION:    cmgrCmd
**
** PURPOSE:     Send a command to the call manager for execution of some
**              associated functionality.
**
** PARAMETERS:  eCmd     - the command to be executed.
**              pCmdInfo - the command related parameters (when necessary).
**
** RETURNS:     None.
**
** NOTE:        Unless explicitely stated, commands are to be executed in
**              an asynchronous fashion as they are queued up to the
**              Call Manager thread for execution.  The command response
**              (when such is required) is passed back through the status
**              callback registered during initialization.
******************************************************************************/
void cmgrCmd ( CMGRCMD eCmd, void *pCmdInfo )
{
   CMEVT evt;

   memset ( &evt, 0, sizeof(CMEVT) );

   CMGRLOG_DEBUG (( "cmgrCmd: 0x%lX (0x%X)", eCmd, pCmdInfo ));

   /* Return immediately if SIP stack is already shuts down or never initialized
   */
   if ( gbShutDown )
   {
      CMGRLOG_INFORMATION (( "cmgrCmd: 0x%lX (0x%X) returning immediately, CMGR is "
         "shutdown or not yet initialized.", eCmd, pCmdInfo ));
      return;  
   }
   
   switch ( eCmd )
   {
      case eCMCMD_MEMSTATS:
      {
         CMGRMEMSTATS *pMemStat = (CMGRMEMSTATS *) pCmdInfo;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) pMemStat->bCctkOnly;
         evt.uDat2 = (BOS_UINT32) pMemStat->bTraceTable;
      }
      break;

      case eCMCMD_NEWCONFIG:
      {
         CMGRRECONFIG *pReConfig = (CMGRRECONFIG *) pCmdInfo;
         BOS_UINT8 *pUid = (BOS_UINT8 *) malloc ( sizeof ( pReConfig->uUid ) );

         if ( pUid )
         {
            memcpy ( (void *) pUid,
                     (void *) pReConfig->uUid,
                     sizeof ( pReConfig->uUid ) ); 
         }

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) pReConfig->eCfgAction;
         evt.pDat3 = (void *) pUid;
      }
      break;

      case eCMCMD_STATUS:
      {
         CMGRSHOWSTATUS *pStatus = (CMGRSHOWSTATUS *) pCmdInfo;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) pStatus->bShowCmgr;
         evt.uDat2 = (BOS_UINT32) pStatus->bShowCctk;
      }
      break;

      case eCMCMD_SHUTDOWN:
      {
         gbShutDown = BOS_TRUE;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
      }
      break;

      case eCMCMD_RESTART:
      {
         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);

         if( pCmdInfo != NULL )
         {
            /* Pass user specified restart mode */
            evt.uDat1 = *( (BOS_UINT32 *) pCmdInfo );
         }
      }
      break;

      /* Any other cases, we do not need further information but the
      ** command itself.
      */
      default:
      {
         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
      }
      break;
   }

   if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
   {
      cmSignalEvt ();
   }
   else 
   {
      CMGRLOG_CRITICAL (( "cmgrCmd: failed to post 0x%lX!", eCmd ));

      if ( evt.pDat3 != NULL )
      {
         free ( evt.pDat3 );
         evt.pDat3 = NULL;
      }
   }
}

