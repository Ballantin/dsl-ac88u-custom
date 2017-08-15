/**********************************************************************************
** Copyright (c) 2008-2013 Broadcom Corporation
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
**      This module implements the feature processing for the Call Manager.
**
**      This is the only location in the Call Manager code where there is a need
**      to make the support of certain features compile time configurable.  Any
**      feature that is to be attempted will go through the same check to verify
**      that the feature is enabled for the application.  If not, the feature
**      will fail.  In order to determine if a feature is enabled or not, the
**      application configuration MUST define the following macros (for each
**      feature of interest): '#define CMGR__FEATURE__XXX' where XXX is the
**      name of the feature of interest.
**
**      For example 'CMGR__FEATURE__DO_NOT_DISTURB' must be defined in the
**      application configuration file (the cmgrCfgCustom.h module) for the DND
**      (do-not-disturb) feature to be enabled.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>


/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* The following constants are used to format the URI used when invoking
** a feature (service).
*/
#define CMGR_SIP_URI                      "sip:"
#define CMGR_SIPS_URI                     "sips:"
#define CMGR_USER_DIALSTRING_URI          ";user=dialstring"
#define CMGR_AT_URI                       "@"
#define CMGR_SEPARATOR_URI                "."

#define CMGR_PRIVATE_DCS_TRACE_PARTY_ID   "P-DCS-TRACE-PARTY-ID"
#define CMGR_ANONYMOUS_URI                "anonymous@anonymous.invalid"
#define CMGR_USER_PHONE_URI               ";user=phone"
#define CMGR_USER_CALLID_URI              ";callid="
#define CMGR_USER_TIMESTAMP_URI           ";timestamp="

#define CMGR_CALL_INFO_PURPOSE            ";purpose="
#define CMGR_CALL_INFO_PURPOSE_ANSNOBUSY  "answer_if_not_busy"

#define CMGR_STAR_INTERNALCALL            "*"



/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmFeatApplyAction
**
** PURPOSE:    Feature handler to apply an action from the digit manipulation
**             output.
**
** PARAMETERS: pRes - the resource to apply this action on.
**             eAction - the action to execute.
**
** RETURNS:    BOS_TRUE if the action is successfully processed and one can
**             continue with other successive actions following.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatApplyAction ( void *pRes, CMDMACTIONTYPE eAction )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CMTMR tmr;
   BOS_BOOL bRet = BOS_TRUE;

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   switch ( eAction )
   {
      case eCMDM_MAKECALL:
      {
         /* The long interdigit timer may still be running if we came here
         ** because of a short interdigit timer expiry.
         */
         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = pSes->uHdl;
         tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
         cmTmrStop ( &tmr );
         {
            BOS_UINT8 *pActParm;
            BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );

            cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                               &pActParm );

            pSes->uNetUsr = uAccount;


            if ( cmAppNetCallCreate( pSes, pActParm ) != eCMAPPNETSTATUS_SUCCESS )
            {
               cmFsmCallEnd ( pRes );
            }
         }
      }
      break;


      /* Invalid or not handled in this state.  Continue to process
      ** other actions nevertheless if any.
      */
      case eCMDM_UNKNOWNACTION:
      default:
      break;
   }

   return bRet;
}



/***********************************************************************************
** FUNCTION:   cmFeatDialStrManip
**
** PURPOSE:    Apply feature specific digit string manipulation.  This function
**             is meant to be used to see whether a given dial string needs to
**             be morphed to a different one prior to digit map processing.
**
** PARAMETERS: pUsrRes - the user resource to apply this on.
**             pSesRes - the session resource associated to the user (can be
**                       NULL) to apply this on.  Only relevant when the dialing
**                       is associated with a session rather than with the user.
**
** RETURNS:    BOS_TRUE if the dial string was manipulated (i.e. changed in any
**             ways within this function call), BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatDialStrManip ( void *pUsrRes, void *pSesRes )
{
   BOS_BOOL bRet = BOS_FALSE;
   CMUSR *pUsr = (CMUSR *) pUsrRes;
   PROV_CFG_IDX cfgIdx;
   CFGSIPLOCALSPEEDDIALING *pLclSpeedDial = NULL;

   if ( pUsr != NULL )
   {
      BOS_UINT8 *pCurDig =
         (pSesRes != NULL) ?
            ((CMSES *)pSesRes)->ses.med.uDigCol : pUsr->uDigCol;
      BOS_UINT32 *pCurCnt =
         (pSesRes != NULL) ?
            &(((CMSES *)pSesRes)->ses.med.uDigCnt) : &(pUsr->uDigCnt);

      /* The only feature that we currently implement that is able to
      ** swap the current collected digits with a new set is the local
      ** speed dial feature.
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, LocalSpeedDialingConcept ),
                                 &cfgIdx,
                                 (void *) &pLclSpeedDial,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pLclSpeedDial )
         {
            CFGSIPLOCALSPEEDDIALENTRY *pSpDialCur = pLclSpeedDial->pSpeedDialList;
            BOS_UINT8 uDigCol[CMGR_COLLECTED_DIGIT_LEN + 1];
            BOS_UINT8 uTerm[2];
            BOS_UINT8 uPound =
               cmMapGetEvt ( cmMapCmEvt2DigCol, eCMEVT_DTMF_HASH );

            while ( pSpDialCur && !bRet )
            {
               /* Try to match the current string against the local speed
               ** dial code appended with an inter-digit timeout termination.
               */
               memset ( (void *) uDigCol, 0, sizeof ( uDigCol ) );
               sprintf ( (char *) uTerm, "%c",
                         (char) CMDM_INTERDIGIT_TMR_SHORT );
               strMaxCat ( (char *) uDigCol,
                           (const char *) pSpDialCur->pcSpeedDialLocalMapCode,
                           CMGR_COLLECTED_DIGIT_LEN + 1 );
               strMaxCat ( (char *) uDigCol,
                           (const char *) uTerm,
                           CMGR_COLLECTED_DIGIT_LEN + 1 );
               if ( !strcmp ( (const char *) pCurDig,
                              (const char *) uDigCol ) )
               {
                  /* Match found.  Replace the current string and
                  ** exit processing.
                  */
                  CMGRLOG_INFORMATION ((
                     "cmFeatDialStrManip: 0x%X (0x%X) - set \'%s\' to \'%s\'",
                     pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ),
                     pCurDig, pSpDialCur->pcSpeedDialLocalMapDigitString ));

                  memset ( (void *) pCurDig, 0, CMGR_COLLECTED_DIGIT_LEN + 1 );
                  *pCurCnt =
                     (BOS_UINT32) strPrintf ( (char *) pCurDig,
                                              CMGR_COLLECTED_DIGIT_LEN,
                                              "%s",
                                              (const char *) pSpDialCur->
                                                   pcSpeedDialLocalMapDigitString );
                  bRet = BOS_TRUE;
               }

               /* Further try to match the current string against the local speed
               ** dial code appended with a pound termination.
               */
               if ( !bRet )
               {
                  memset ( (void *) uDigCol, 0, sizeof ( uDigCol ) );
                  sprintf ( (char *) uTerm, "%c",
                            (char) uPound );
                  strMaxCat ( (char *) uDigCol,
                              (const char *) pSpDialCur->pcSpeedDialLocalMapCode,
                              CMGR_COLLECTED_DIGIT_LEN + 1 );
                  strMaxCat ( (char *) uDigCol,
                              (const char *) uTerm,
                              CMGR_COLLECTED_DIGIT_LEN + 1 );
                  if ( !strcmp ( (const char *) pCurDig,
                                 (const char *) uDigCol ) )
                  {
                     /* Match found.  Replace the current string and
                     ** exit processing.
                     */
                     CMGRLOG_INFORMATION ((
                        "cmFeatDialStrManip: 0x%X (0x%X) - set \'%s\' to \'%s\'",
                        pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ),
                        pCurDig, pSpDialCur->pcSpeedDialLocalMapDigitString ));

                     memset ( (void *) pCurDig, 0, CMGR_COLLECTED_DIGIT_LEN + 1 );
                     *pCurCnt =
                        (BOS_UINT32) strPrintf ( (char *) pCurDig,
                                                 CMGR_COLLECTED_DIGIT_LEN,
                                                 "%s",
                                                 (const char *) pSpDialCur->
                                                   pcSpeedDialLocalMapDigitString );
                     bRet = BOS_TRUE;
                  }
               }

               pSpDialCur = pSpDialCur->pNext;
            };
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, LocalSpeedDialingConcept ),
                               &cfgIdx,
                               (void *) pLclSpeedDial,
                               CFG_ITEM_DEL );
         pLclSpeedDial = NULL;
      }
   }

   return bRet;
}


