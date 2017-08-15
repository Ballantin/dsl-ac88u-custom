/**********************************************************************************
** Copyright (c) 2009-2012 Broadcom
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
**      This file defines the API (application programming interface) to the
**      Call Control Toolkit Backup and Recovery module.
**
***********************************************************************************/
#ifndef __NS_BR_API__H__INCLUDED__
#define __NS_BR_API__H__INCLUDED__

#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Constants and Types ----------------------------------------------------- */

/* Defines the status of the backup and recovery information block associated
** with a resource.
*/
typedef enum
{
   eNSBRSTA_INVALID,             /* Backup and recovery is either not enabled
                                 ** or resource is not associated with the
                                 ** service.
                                 */
   eNSBRSTA_IDLE,                /* Initial state, or we have a valid target
                                 ** being used, all is well.
                                 */
   eNSBRSTA_BACKUP_AVAILABLE,    /* Failure has been detected a backup target
                                 ** is available for use.
                                 */
   eNSBRSTA_FAILED,              /* Failure detected, all possible targets are
                                 ** failed, need 'hard' restart.
                                 */

} NSBRSTATUS;

/* ---- Include Files ----------------------------------------------------------- */
#include <nsBr.h>  /* Needs NSBRSTATUS defined. */

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------------- */

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
void nsBrInit( void );


/***********************************************************************************
**  FUNCTION:   nsBrDeInit
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
void nsBrDeInit( void );


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
                     void *pContext );


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
void nsBrDetach( BOS_UINT32 uResGuid );


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
BOS_BOOL nsBrRegister( BOS_UINT32 uResGuid );


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
void nsBrDeRegister( BOS_UINT32 uResGuid );


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
NSBRSTATUS nsBrStatus( BOS_UINT32 uResGuid );


/***********************************************************************************
**  FUNCTION:   nsBrBackupOnFail
**
**  PURPOSE:    Asks the backup and recovery module to provide a backup solution in
**              case of a failure detected.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**              bAsgnBkoffTmr - whether to assign the back off timer if there is no
**                              backup solution.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:
***********************************************************************************/
NSBRSTATUS nsBrBackupOnFail( BOS_UINT32 uResGuid, BOS_BOOL bAsgnBkoffTmr);


/***********************************************************************************
**  FUNCTION:   nsBrRestart
**
**  PURPOSE:    Asks the backup and recovery module to start up fresh again in case
**              there has been a failure previously.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:
***********************************************************************************/
NSBRSTATUS nsBrRestart( BOS_UINT32 uResGuid );


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
BOS_UINT32 nsBrBackOff( BOS_UINT32 uResGuid );

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
BOS_BOOL nsBrResetBackOff( BOS_UINT32 uResGuid );


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
BOS_BOOL nsBrFailedPrimary( BOS_UINT32 uResGuid );

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
BOS_BOOL nsBrPrimaryStillPreferred( BOS_UINT32 uResGuid );


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
NSBRSTATUS nsBrTargetCopy( BOS_UINT32 uResGuidFrom, BOS_UINT32 uResGuidTo );


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
BOS_BOOL nsBrRestrict2Primary( BOS_UINT32 uResGuid, BOS_BOOL bRestrict );


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
                                 MXD_GNS::CSocketAddr *pAddr);

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
BOS_BOOL nsBrRemoveRestrictions( BOS_UINT32 uResGuid );

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
BOS_BOOL nsBrGetActiveTgt( BOS_UINT32 uResGuid, MXD_GNS::CSocketAddr& rSockAddr );

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
NSBRSTATUS nsBrSave( BOS_UINT32 uGuid, NSBRBLK *pSaveTo );


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
NSBRSTATUS nsBrPrime( BOS_UINT32 uGuid, NSBRBLK *pPrimeFrom );

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
                          CCTKHOSTPORT& rHostPort );

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
                                 MXD_GNS::CSocketAddr& rActSockAddr );

#ifdef __cplusplus
}
#endif

#endif /* __NS_BR_API__H__INCLUDED__ */
