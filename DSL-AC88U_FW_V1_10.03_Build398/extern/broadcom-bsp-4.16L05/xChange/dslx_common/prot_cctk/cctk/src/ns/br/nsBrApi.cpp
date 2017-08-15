/**********************************************************************************
** Copyright (c) 2009-2012 Broadcom Corporation
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
**      This file implements the API functionality of the Backup and Recovery
**      module.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <nsDlm.h>
#include <nsBr.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
NsDlm *gpNsDlm = NULL;
NsBr *gpNsBr = NULL;

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   nsBrInit
**
**  PURPOSE:    Module initializer.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void nsBrInit( void )
{
#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr == NULL )
   {
      gpNsBr = BRCM_NEW( NsBr );
      CCTK_TRACEI1( "nsBrInit - Br: %p", gpNsBr );
   }

   if ( gpNsDlm == NULL )
   {
      gpNsDlm = BRCM_NEW( NsDlm ) ( gpNsBr );
      CCTK_TRACEI1( "nsBrInit - Dlm: %p", gpNsDlm );
   }
#endif /* CCTKCFG_PROXY_FAILOVER */ 
}


/***********************************************************************************
**  FUNCTION:   nsBrDeInit
**
**  PURPOSE:    Module de-initializer.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void nsBrDeInit( void )
{
#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr != NULL )
   {
      CCTK_TRACEI1( "nsBrDeInit - Br: %p", gpNsBr );

      BRCM_DELETE( gpNsBr );
      gpNsBr = NULL;
   }

   if ( gpNsDlm != NULL )
   {
      CCTK_TRACEI1( "nsBrDeInit - Dlm: %p", gpNsDlm );

      BRCM_DELETE( gpNsDlm );
      gpNsDlm = NULL;
   }
#endif /* CCTKCFG_PROXY_FAILOVER */ 
}


/***********************************************************************************
**  FUNCTION:   nsBrAttach
**
**  PURPOSE:    Attach the backup and recovery service for this resource.
**
**  PARAMETERS:
**              uParentGuid - the parent (user) GUID for this resource.
**              uResGuid - the resource GUID (could be the same as the parent
**                         if the resource is a user).
**              pContext - pointer to the SCE context to attach this service
**                         onto.
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL nsBrAttach( BOS_UINT32 uParentGuid,
                     BOS_UINT32 uResGuid,
                     void *pContext )
{
   BOS_BOOL bRet = BOS_FALSE;
   
#if CCTKCFG_PROXY_FAILOVER
   BOS_BOOL bRemRes = BOS_FALSE;
#   if CCTKCFG_REMOTE_USER_CONTROL
   if ( ceRemMgmtRes( uParentGuid ) )
   {
      /* It does not make sense to associate any remote user
      ** resource with the backup and recovery feature framework
      ** since we do not run such feature for a remote target.
      */
      bRemRes = BOS_TRUE;
   }
#   else
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uParentGuid );
#   endif /* CCTKCFG_REMOTE_USER_CONTROL */

   /* Make sure we can allocate resources to take care of this
   ** service.
   **
   ** Make sure there is no duplication of existing resource usage.
   */
   if ( gpNsBr &&
        !bRemRes &&
        !gpNsBr->nsBrExists( uResGuid ) &&
        gpNsBr->nsBrFreeResBlk() )
   {
      bRet = BOS_TRUE;
   }

   if ( bRet && gpNsDlm )
   {
      /* Attempt to allocate the service now that we are good to go.
      */
      bRet = gpNsDlm->nsDlmAttach( uResGuid,
                                   pContext );
   }
   else
   {
      bRet = BOS_FALSE;
   }
#else
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uParentGuid );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pContext );
#endif /* CCTKCFG_PROXY_FAILOVER */

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   nsBrDetach
**
**  PURPOSE:    Detach the backup and recovery service for this resource.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void nsBrDetach( BOS_UINT32 uResGuid )
{
#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      gpNsBr->nsBrClean ( uResGuid );
   }
#else
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */
}


/***********************************************************************************
**  FUNCTION:   nsBrRegister
**
**  PURPOSE:    Registers a resource to be using the backup and recovery module.
**
**  PARAMETERS:
**              uResGuid - the resource GUID (only user resource can register
**                         for this service).
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL nsBrRegister( BOS_UINT32 uResGuid )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrRegister ( uResGuid );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return BOS_FALSE;
   }
}


/***********************************************************************************
**  FUNCTION:   nsBrDeRegister
**
**  PURPOSE:    De-registers a resource to be using the backup and recovery module.
**
**  PARAMETERS:
**              uResGuid - the resource GUID (only user resource can de-register
**                         for this service).
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void nsBrDeRegister( BOS_UINT32 uResGuid )
{
#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      gpNsBr->nsBrDeRegister ( uResGuid );
   }
#else
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */
}


/***********************************************************************************
**  FUNCTION:   nsBrStatus
**
**  PURPOSE:    Queries the status of the backup and recovery service for the
**              given resource of interest.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:
***********************************************************************************/
NSBRSTATUS nsBrStatus( BOS_UINT32 uResGuid )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrStatus ( uResGuid );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return eNSBRSTA_INVALID;
   }
}


/***********************************************************************************
**  FUNCTION:   nsBrBackupOnFail
**
**  PURPOSE:    Asks the backup and recovery module to provide a backup solution in
**              case of a failure detected.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**              bAsgnBkoffTmr - whether to assign the back off timer if there isn't a
**                              backup solution.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:
***********************************************************************************/
NSBRSTATUS nsBrBackupOnFail( BOS_UINT32 uResGuid, BOS_BOOL bAsgnBkoffTmr )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrBackupOnFail ( uResGuid, bAsgnBkoffTmr );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return eNSBRSTA_INVALID;
   }
}


/***********************************************************************************
**  FUNCTION:   nsBrRestart
**
**  PURPOSE:    Asks the backup and recovery module start up fresh again in case
**              there has been a failure previously.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:
***********************************************************************************/
NSBRSTATUS nsBrRestart( BOS_UINT32 uResGuid )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrRestart ( uResGuid );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return eNSBRSTA_INVALID;
   }
}


/***********************************************************************************
**  FUNCTION:   nsBrBackOff
**
**  PURPOSE:    Gets the value of the back-off timer to be applied as part of the
**              backup process.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    The value of the backoff timer, or 0 if invalid.
**
**  NOTES:
***********************************************************************************/
BOS_UINT32 nsBrBackOff( BOS_UINT32 uResGuid )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrBackOff ( uResGuid );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      /* Return 0 by default, should be interpreted as 'do not apply'.
      */
      return 0;
   }
}

/***********************************************************************************
**  FUNCTION:   nsBrResetBackOff
**
**  PURPOSE:    Restart the value of the back-off timer to be applied as part of the
**              backup process.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    BOS_TRUE if the timer was succesfully restarted. BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL nsBrResetBackOff( BOS_UINT32 uResGuid )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrResetBackOff ( uResGuid );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return BOS_FALSE;
   }
}



/***********************************************************************************
**  FUNCTION:   nsBrFailedPrimary
**
**  PURPOSE:    Asks the backup and recovery module whether the resource has failed
**              with its primary target or not.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    BOS_TRUE if the primary target is in failure, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL nsBrFailedPrimary( BOS_UINT32 uResGuid )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrFailedPrimary ( uResGuid );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return BOS_FALSE;
   }
}

/***********************************************************************************
**  FUNCTION:   nsBrPrimaryStillPreferred
**
**  PURPOSE:    Asks the backup and recovery module whether the current primary
**              is still the most preferred target.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    BOS_TRUE if the marked primary target equals the saved preferred 
**              target, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL nsBrPrimaryStillPreferred( BOS_UINT32 uResGuid )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrPrimaryStillPreferred ( uResGuid );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return BOS_FALSE;
   }
}


/***********************************************************************************
**  FUNCTION:   nsBrRestrict2Primary
**
**  PURPOSE:    Asks the backup and recovery module to restrict or not its knowledge
**              set to the primary target alone, this can be used in some failover
**              scenario when we want to ensure only the primary target is used.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**              bRestrict - whether to restrict (BOS_TRUE) or lift the restriction
**                          (BOS_FALSE)
**
**  RETURNS:    BOS_TRUE if the restriction was applied, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL nsBrRestrict2Primary( BOS_UINT32 uResGuid, BOS_BOOL bRestrict )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( bRestrict );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrApplyRestriction ( uResGuid, bRestrict, eNSBRRESTRICTIONTYPE_PRIMONLY );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return BOS_FALSE;
   }
}

/***********************************************************************************
**  FUNCTION:   nsBrRestrict2Address
**
**  PURPOSE:    Asks the backup and recovery module to restrict or not its knowledge
**              set to the specified address alone, this can be used if we want
**              to restrict all signalling to a specific target only.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**              bRestrict - whether to restrict (BOS_TRUE) or lift the restriction
**                          (BOS_FALSE)
**              pAddr - the address which the resource should be restricted to.                
**
**  RETURNS:    BOS_TRUE if the restriction was applied, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL nsBrRestrict2Address( BOS_UINT32 uResGuid, BOS_BOOL bRestrict, 
                                 MXD_GNS::CSocketAddr *pAddr)
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( bRestrict );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pAddr );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrApplyRestriction ( uResGuid, bRestrict, 
                                            eNSBRRESTRICTIONTYPE_ADDRSPECIFIED, pAddr );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return BOS_FALSE;
   }
}

/***********************************************************************************
**  FUNCTION:   nsBrRemoveRestrictions
**
**  PURPOSE:    Asks the backup and recovery module to remove any known restrictions
**              placed on target selection.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    BOS_TRUE if the restriction was removed, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL nsBrRemoveRestrictions( BOS_UINT32 uResGuid )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrApplyRestriction ( uResGuid, BOS_FALSE, 
                                            eNSBRRESTRICTIONTYPE_NONE );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return BOS_FALSE;
   }
}


/***********************************************************************************
**  FUNCTION:   nsBrGetActiveTgt
**
**  PURPOSE:    Retrieves the active target information associated with the GUID
**              parameter.
**
**  PARAMETERS:
**              uResGuid - the GUID of interest.
**              rSockAddr - reference to the active target address to retrieve.
**
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL nsBrGetActiveTgt( BOS_UINT32 uResGuid, MXD_GNS::CSocketAddr& rSockAddr )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rSockAddr );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrGetActiveTgt ( uResGuid, rSockAddr );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return BOS_FALSE;
   }
}


/***********************************************************************************
**  FUNCTION:   nsBrTargetCopy
**
**  PURPOSE:    Asks the backup and recovery module to copy a list of targets from
**              one resource to the next.  This is to be used in cases where one
**              want an exact same set to be applied to different resources.
**
**  PARAMETERS:
**              uResGuidFrom - the resource GUID to copy information from.
**              uResGuidTo - the resource GUID to copy information to.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:
***********************************************************************************/
NSBRSTATUS nsBrTargetCopy( BOS_UINT32 uResGuidFrom, BOS_UINT32 uResGuidTo )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuidFrom );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuidTo );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrCopyTgt ( uResGuidFrom, uResGuidTo );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return eNSBRSTA_INVALID;
   }
}


/***********************************************************************************
**  FUNCTION:   nsBrSave
**
**  PURPOSE:    Asks the backup and recovery module to save the current block
**              configuration associated with the GUID.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**              pSaveTo - where to save the information to.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:      nsBrSave and nsBrPrime are paired function, they MUST be invoked
**              together and within the same execution context to prevent memory
**              leakage.  They are used for temporary hold of some information
**              only.
***********************************************************************************/
NSBRSTATUS nsBrSave( BOS_UINT32 uGuid, NSBRBLK *pSaveTo )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uGuid );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pSaveTo );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrSave ( uGuid, pSaveTo );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return eNSBRSTA_INVALID;
   }
}


/***********************************************************************************
**  FUNCTION:   nsBrPrime
**
**  PURPOSE:    Asks the backup and recovery module to prime the current block
**              configuration associated with the GUID with the data provided.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**              pPrimeFrom - where to prime the information from.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:      nsBrSave and nsBrPrime are paired function, they MUST be invoked
**              together and within the same execution context to prevent memory
**              leakage.  They are used for temporary hold of some information
**              only.
***********************************************************************************/
NSBRSTATUS nsBrPrime( BOS_UINT32 uGuid, NSBRBLK *pPrimeFrom )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uGuid );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pPrimeFrom );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrPrime ( uGuid, pPrimeFrom );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return eNSBRSTA_INVALID;
   }
}
/***********************************************************************************
**  FUNCTION:   nsBrGetAvailTgt
**
**  PURPOSE:    Retrieves the first target address which is not in the failed state.
**
**  PARAMETERS:
**              uResGuid - the GUID of interest.
**              rHostPort - hostport reference to set to target address.
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL nsBrGetAvailTgt( BOS_UINT32 uResGuid,
                          CCTKHOSTPORT& rHostPort )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uResGuid );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rHostPort );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrGetAvailTgt ( uResGuid, rHostPort );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return BOS_FALSE;
   }
}

/***********************************************************************************
**  FUNCTION:   nsBrXtlLstSetFailedTgt
**
**  PURPOSE:    Sets the specified target address as failed in the backup
**              and recovery block of the user
**
**  PARAMETERS: uGuid - the resource GUID to update the target for.
**              rActSockAddr - socket address which failed.
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:      This to only be used when the backup and recovery feature
**              in external service mode for the user.
***********************************************************************************/
BOS_BOOL nsBrXtlLstSetFailedTgt( BOS_UINT32 uGuid,
                                 MXD_GNS::CSocketAddr& rActSockAddr )
{
#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uGuid );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rActSockAddr );
#endif /* CCTKCFG_PROXY_FAILOVER */

#if CCTKCFG_PROXY_FAILOVER
   if ( gpNsBr )
   {
      return gpNsBr->nsBrXtlLstSetFailedTgt ( uGuid, rActSockAddr );
   }
   else
#endif /* CCTKCFG_PROXY_FAILOVER */
   {
      return BOS_FALSE;
   }
}
