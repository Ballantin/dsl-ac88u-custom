/**********************************************************************************
** Copyright (c) 2008-2012 Broadcom Corporation
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
**      This file contains the Call Manager physical interface implementation.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
static BOS_BOOL gbCmPhyStartup = BOS_FALSE;
static BOS_UINT32 guCmPhyIfMap[ CMGRCFG_PHY_MAX ];

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmPhyStartup
**
** PURPOSE:    Launches the physical (endpoint) interface.
**
** PARAMETERS: epEvtCb - the callback used by the external physical layer to
**                       notify CMGR of events.
**
** RETURNS:    eCMGRSTATUS_STARTUP_SUCCESS on success
**             eCMGRSTATUS_STARTUP_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyStartup ( CMGREPEVT epEvtCb )
{
   CMGREPCMD epCmd;
   BOS_UINT32 uIx;

   BOS_ASSERT ( epEvtCb != NULL );

   CMGRLOG_INFORMATION (( "cmPhyStartup: starting endpoint support." ));

   epCmd.eAction = eCMGRPHYCMD_INIT;
   epCmd.uPhy = CMGR_INVALID_HANDLE;

   epCmd.cmd.init.evt = epEvtCb;

   gCmCfgCb.cmgrEptCtlCb ( &epCmd );

   for ( uIx = 0 ; uIx < CMGRCFG_PHY_MAX ; uIx++ )
   {
      guCmPhyIfMap [ uIx ] = CMGR_INVALID_HANDLE;
   }

   gbCmPhyStartup = BOS_TRUE;

   return eCMGRSTATUS_STARTUP_SUCCESS;
}


/***********************************************************************************
** FUNCTION:   cmPhyIsStarted
**
** PURPOSE:    Checks whether the physical (endpoint) interface is started.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if the physical interface had been started at some point.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmPhyIsStarted ()
{
   return gbCmPhyStartup;
}


/***********************************************************************************
** FUNCTION:   cmPhyShutdown
**
** PURPOSE:    Shutsdown the physical (endpoint) interface.
**
** PARAMETERS: None.
**
** RETURNS:    eCMGRSTATUS_SHUTDOWN_COMPLETE on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyShutdown ( void )
{
   CMGREPCMD epCmd;

   CMGRLOG_WARNING (( "cmPhyShutdown: terminating endpoint support." ));

   epCmd.eAction = eCMGRPHYCMD_DEINIT;
   epCmd.uPhy = CMGR_INVALID_HANDLE;

   gCmCfgCb.cmgrEptCtlCb ( &epCmd );

   gbCmPhyStartup = BOS_FALSE;

   return eCMGRSTATUS_STARTUP_SUCCESS;
}


/***********************************************************************************
** FUNCTION:   cmPhyCreate
**
** PURPOSE:    Creates a specific physical (endpoint) interface.
**
** PARAMETERS: uPhy - the interface identifier to be created.
**
** RETURNS:    eCMGRSTATUS_COMMAND_SUCCESS on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyCreate ( BOS_UINT32 uPhy )
{
   CMGREPCMD epCmd;
   CMGRSTATUS eStatus = eCMGRSTATUS_COMMAND_FAILED;
   BOS_UINT32 uIx;

   CMGRLOG_INFORMATION (( "cmPhyCreate: interface %d.", uPhy ));

   if ( uPhy != CMGR_INVALID_HANDLE )
   {
      epCmd.eAction = eCMGRPHYCMD_CREATE;
      epCmd.uPhy = uPhy;

      gCmCfgCb.cmgrEptCtlCb ( &epCmd );

      /* Save the information in the physical map, this is to be
      ** used when attempting to 'cmPhyTerminate'.
      */
      for ( uIx = 0 ; uIx < CMGRCFG_PHY_MAX ; uIx++ )
      {
         if ( guCmPhyIfMap [ uIx ] == CMGR_INVALID_HANDLE )
         {
            guCmPhyIfMap [ uIx ] = uPhy;
            break;
         }
      }

      eStatus = eCMGRSTATUS_COMMAND_SUCCESS;
   }


   return eStatus;
}


/***********************************************************************************
** FUNCTION:   cmPhyDestroy
**
** PURPOSE:    Destroys a specific physical (endpoint) interface.
**
** PARAMETERS: uPhy - the interface identifier to be destroyed.
**
** RETURNS:    eCMGRSTATUS_COMMAND_SUCCESS on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyDestroy ( BOS_UINT32 uPhy )
{
   CMGREPCMD epCmd;
   CMGRSTATUS eStatus = eCMGRSTATUS_COMMAND_FAILED;

   CMGRLOG_INFORMATION (( "cmPhyDestroy: interface %d.", uPhy ));

   if ( uPhy != CMGR_INVALID_HANDLE )
   {
      epCmd.eAction = eCMGRPHYCMD_DESTROY;
      epCmd.uPhy = uPhy;

      gCmCfgCb.cmgrEptCtlCb ( &epCmd );

      eStatus = eCMGRSTATUS_COMMAND_SUCCESS;
   }


   return eStatus;
}


/***********************************************************************************
** FUNCTION:   cmPhySignal
**
** PURPOSE:    Signals something to a physical (endpoint) interface.
**
** PARAMETERS: uPhy - physical interface to signal to.
**             pSig - what to signal.
**
** RETURNS:    eCMGRSTATUS_COMMAND_SUCCESS on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhySignal ( BOS_UINT32 uPhy, CMGREPSIG *pSig )
{
   CMGREPCMD epCmd;
   CMGRSTATUS eStatus = eCMGRSTATUS_COMMAND_FAILED;

   BOS_ASSERT ( pSig != NULL );

   CMGRLOG_INFORMATION (( "cmPhySignal: if %d, ses 0x%X, sig %d,"
                          " act %d, data \'%s\', cid %X",
                          uPhy, pSig->uSes, pSig->uSig,
                          pSig->eAct,
                          (pSig->pData != NULL && pSig->uSig != eEPTSIG_INGRESS_DTMF) ? pSig->pData : "",
                          pSig->uCid ));


   if ( uPhy != CMGR_INVALID_HANDLE )
   {
      epCmd.eAction = eCMGRPHYCMD_SIGNAL;
      epCmd.uPhy = uPhy;

      memcpy ( &epCmd.cmd.sig, pSig, sizeof( CMGREPSIG ) );;

      gCmCfgCb.cmgrEptCtlCb ( &epCmd );

      eStatus = eCMGRSTATUS_COMMAND_SUCCESS;
   }

   return eStatus;
}


/***********************************************************************************
** FUNCTION:   cmPhyAllocate
**
** PURPOSE:    Allocates a physical interface with a given user if none is
**             associated with it at this time.
**
** PARAMETERS: pUsr - the user to associate an interface with.
**
** RETURNS:    eCMGRSTATUS_COMMAND_SUCCESS on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyAllocate ( CMUSR *pUsr )
{
   CMGRSTATUS eStatus = eCMGRSTATUS_COMMAND_FAILED;
   CFGSIPUSERCFG *pUsrCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT32 uPhy;

   if ( ( uPhy = pUsr->uPhys ) == CMGR_INVALID_HANDLE )
   {
      switch ( pUsr->eType )
      {
         case eCMUSRTYPE_LINE:
         case eCMUSRTYPE_PSTN:
         {
            /* This should come directly from the configuration information.
            ** Locate the information from there.
            */
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               CFGSIPUSERENDPNTMAP *pEpt = pUsrCfg->pEndPntMap;
               while ( pEpt )
               {
                  if ( ( uPhy == CMGR_INVALID_HANDLE ) && (pEpt->bIn || pEpt->bOut) )
                  {
                     /* Make sure this physical resource is not already used
                     ** by someone else.
                     */
                     if ( cmUsrFindPhyHdl ( pEpt->sEndpntNum ) == NULL )
                     {
                        uPhy = pEpt->sEndpntNum;
                        break;
                     }
                  }
                  /* Increment the interface.
                  */
                  pEpt = pEpt->pNext;
               };

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                     &cfgIdx,
                                     (void *) pUsrCfg,
                                     CFG_ITEM_DEL );
               pUsrCfg = NULL;
            }

            /* If allocated successfully...
            */
            if ( uPhy != CMGR_INVALID_HANDLE )
            {
               pUsr->uPhys = uPhy;

               eStatus = eCMGRSTATUS_COMMAND_SUCCESS;
            }
         }
         break;

         case eCMUSRTYPE_SHAREDLINE:
         {
            /* Shared line is a tricky case.  One should ensure that the
            ** resource can be used at this particular time.
            */

            /* Unfinished at this time... */
         }
         break;

         case eCMUSRTYPE_REMOTE:
         {
            /* A remote user is a user which is managed by the local application
            ** but for which physical resources are in fact remote.  In such case
            ** we do not need a local physical resource allocation and we can
            ** assume always success in such resource allocation.  If a physical
            ** resource ends up being not available on the remote target, the
            ** call or service would terminate by other means (call clearing for
            ** example).
            */
            eStatus = eCMGRSTATUS_COMMAND_SUCCESS;
         }
         break;

         default:
         break;
      }
   }
   else
   {
      eStatus = eCMGRSTATUS_COMMAND_SUCCESS;
   }

   CMGRLOG_INFORMATION (( "cmPhyAllocate: usr 0x%X -> phy 0x%X",
                          pUsr->uHdl, uPhy ));

   return eStatus;
}


/***********************************************************************************
** FUNCTION:   cmPhyToneFromString
**
** PURPOSE:    Attemps to lookup a tone information to be applied on the physical
**             interface from its string representation value.
**
** PARAMETERS: puStr - the tone string information.
**
** RETURNS:    The tone identification if found, CMGR_INVALID_HANDLE otherwise.
**
**             WARNING: This is a temporary fix until the API into the endpoint
**                      allows for proper tone and announcement support.  After
**                      which such processing will not be required any longer.
***********************************************************************************/
BOS_UINT32 cmPhyToneFromString ( BOS_UINT8 *puStr )
{
   BOS_UINT32 uRet = CMGR_INVALID_HANDLE;
   BOS_UINT8 *puTone = NULL;
   BOS_UINT8 *puDelim = NULL;
   BOS_UINT8 *puFile = NULL;
   BOS_UINT8 *puHttp = NULL;
   BOS_UINT8 *puBellcore = NULL;
   BOS_UINT32 uPreambleSize = 0;

   if ( puStr != NULL )
   {
      /* Check for preamble, should be mutually exclusive.  If one is found, assign the
      ** size to skip over it.
      */
      if ( ( ( puFile = (BOS_UINT8 *) strstr ( (const char *) puStr,
                                 CMGR_TONE_PREAMBLE_FILE_IDENTIFIER )) != NULL) ||
           ( ( puHttp = (BOS_UINT8 *) strstr ( (const char *) puStr,
                                 CMGR_TONE_PREAMBLE_HTTP_IDENTIFIER )) != NULL) )
      {
         if ( puFile )
         {
            uPreambleSize = strlen ( CMGR_TONE_PREAMBLE_FILE_IDENTIFIER );
         }

         if ( puHttp )
         {
            uPreambleSize = strlen ( CMGR_TONE_PREAMBLE_HTTP_IDENTIFIER );
         }
      }
      /*
      ** Check for angled brace delimiter.
      */
      if ( ( puDelim = 
               (BOS_UINT8 *) strchr ( (const char *) puStr, '<' ) ) != NULL )
      {
         puDelim = (BOS_UINT8 *) strchr ( (const char *) puStr, '>' );
         if ( puDelim != NULL )
         {
            *puDelim = '\0';
         }
      }
      /*
      ** Skip over the preamble.
      */
      puTone = puStr + uPreambleSize + ( ( puDelim != NULL ) ? 1 : 0 );
      /*
      ** Additionally, we may have a 'Bellcore-d' syntax prefix identifier for the
      ** tone, if so we remove it as well prior to matching the final tone.
      */
      if ( ( puBellcore = (BOS_UINT8 *) strstr( (const char *) puTone,
                                CMGR_TONE_PREAMBLE_BELLCORE_IDENTIFIER ) ) != NULL )
      {
         puTone = puTone + strlen ( CMGR_TONE_PREAMBLE_BELLCORE_IDENTIFIER );
      }
      /*
      ** Finally match the tone.
      */
      uRet = cmMapGetIdFromStr ( cmMapCmTone2Name, puTone );
      /*
      ** Restore afterwards if needed.
      */
      if ( puDelim != NULL )
      {
         *puDelim = '>';
      }
   }

   return uRet;
}


/***********************************************************************************
** FUNCTION:   cmPhyToneWithClid
**
** PURPOSE:    Maps a tone type to its equivalent tone when CLID is enabled with
**             this tone.
**
** PARAMETERS: uSig - the tone to map.
**             bClid - whether CLID is enabled for this tone.
**
** RETURNS:    uSig if CLID is not enabled, the mapped equivalent of uSig if CLID
**             is enabled.
***********************************************************************************/
BOS_UINT32 cmPhyToneWithClid ( BOS_UINT32 uSig, BOS_BOOL bClid )
{
   BOS_UINT32 uRet = uSig;

   if ( bClid )
   {
      uRet = cmMapGetId ( cmMapTone2Clid, uSig );

      /* Could not map, must be a mistake, return the original tone.
      */
      if ( uRet ==  CMGR_INVALID_HANDLE )
      {
         uRet = uSig;
      }
   }

   return uRet;
}


/***********************************************************************************
** FUNCTION:   cmPhyTerminate
**
** PURPOSE:    Terminates (destroy) all the created physical (endpoint)
**             interface(s).
**
** PARAMETERS: None.
**
** RETURNS:    eCMGRSTATUS_COMMAND_SUCCESS on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyTerminate ( void )
{
   CMGRSTATUS eStatus = eCMGRSTATUS_COMMAND_SUCCESS;
   BOS_UINT32 uIx;

   CMGRLOG_INFORMATION (( "cmPhyTerminate: destroying all interfaces" ));

   for ( uIx = 0 ; uIx < CMGRCFG_PHY_MAX ; uIx++ )
   {
      if ( guCmPhyIfMap [ uIx ] != CMGR_INVALID_HANDLE )
      {
         if ( cmPhyDestroy ( guCmPhyIfMap [ uIx ] ) ==
                                    eCMGRSTATUS_COMMAND_SUCCESS )
         {
            guCmPhyIfMap [ uIx ] = CMGR_INVALID_HANDLE;
            cmUsrPhyRelease ( uIx );
         }
         else
         {
            eStatus = eCMGRSTATUS_COMMAND_FAILED;
         }
      }
   }

   return eStatus;
}


/***********************************************************************************
** FUNCTION:   cmPhyToneValidateRing
**
** PURPOSE:    Validates that the tone passed as input corresponds to the a ring
**             tone (alerting tone).
**
** PARAMETERS: uSig - the tone to check.
**
** RETURNS:    Identity if valid, CMGR_INVALID_HANDLE otherwise.
**
** NOTE:       The tone MUST be checked prior to applying CLID tone mapping.
***********************************************************************************/
BOS_UINT32 cmPhyToneValidateRing ( BOS_UINT32 uSig )
{
   BOS_UINT32 uRet = CMGR_INVALID_HANDLE;   

   switch ( uSig )
   {
      case eEPTSIG_RINGING:
      case eEPTSIG_RING0:
      case eEPTSIG_RING1:
      case eEPTSIG_RING2:
      case eEPTSIG_RING3:
      case eEPTSIG_RING4:
      case eEPTSIG_RING5:
      case eEPTSIG_RING6:
      case eEPTSIG_RING7:
      {
         uRet = uSig;   
      }
      break;
      
      default:
      break;   
   }

   return uRet;
}


/***********************************************************************************
** FUNCTION:   cmPhyToneValidateCallWait
**
** PURPOSE:    Validates that the tone passed as input corresponds to the a call
**             waiting tone.
**
** PARAMETERS: uSig - the tone to check.
**
** RETURNS:    Identity if valid, CMGR_INVALID_HANDLE otherwise.
**
** NOTE:       The tone MUST be checked prior to applying CLID tone mapping.
***********************************************************************************/
BOS_UINT32 cmPhyToneValidateCallWait ( BOS_UINT32 uSig )
{
   BOS_UINT32 uRet = CMGR_INVALID_HANDLE;   

   switch ( uSig )
   {
      case eEPTSIG_CALLW1:
      case eEPTSIG_CALLW2:
      case eEPTSIG_CALLW3:
      case eEPTSIG_CALLW4:
      {
         uRet = uSig;   
      }
      break;
      
      default:
      break;   
   }

   return uRet;
}


/***********************************************************************************
** FUNCTION:   cmPhyToneRing2CallWait
**
** PURPOSE:    Checks whether the tone passed is a valid ring tone and if so change
**             it to a corresponding valid call waiting tone.
**
** PARAMETERS: uSig - the tone to check.
**
** RETURNS:    Tone mapped if valid, CMGR_INVALID_HANDLE otherwise.
***********************************************************************************/
BOS_UINT32 cmPhyToneRing2CallWait ( BOS_UINT32 uSig )
{
   return cmMapGetId ( cmMapToneRing2CallWait, uSig );
}

