/**********************************************************************************
** Copyright © 2008 - 2009 Broadcom
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
**      Call Control Toolkit Paylod Modifier module.
**
***********************************************************************************/
#ifndef __PM_API__H__INCLUDED__
#define __PM_API__H__INCLUDED__

#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Include Files ----------------------------------------------------------- */

/* ---- Constants and Types ----------------------------------------------------- */

/* Those defines are shared amongst different CCTK module, therefore they are
** showing up in the public API module for the observer, other defined that are
** not to be shared outside of the observer are defined directly in the observer
** module implementation.
*/
#define PM__SIGNALING_P_EARLY_MEDIA                "P-Early-Media"

#define PM__SIGNALING_PROF_TYPE_EVENT              "ua-profile"

#define PM__SIGNALING_ACCEPT_BLIND_NOT_REASON_INT  "Blind Notify Accepted"

/* ---- Variable Externs -------------------------------------------------------- */


/* ---- Function Prototypes ----------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   pmInit
**
**  PURPOSE:    External API to initialize (and launch) the Payload Modifier module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This API must be invoked prior to any further Payload Modifier work
**              is possible.
***********************************************************************************/
void pmInit( void );


/***********************************************************************************
**  FUNCTION:   pmDeInit
**
**  PURPOSE:    External API to de-initialize the Payload Modifier module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      After this API has been invoked, there is no further processing
**              possible from the Payload Modifier.
***********************************************************************************/
void pmDeInit( void );


/***********************************************************************************
**  FUNCTION:   pmGetPktObs
**
**  PURPOSE:    External API to retrieve the payload modifier observer.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    The pointer to the payload modifier observer object.
**
**  NOTES:      .
***********************************************************************************/
void *pmGetPktObs( void );


/***********************************************************************************
**  FUNCTION:   pmLastChance
**
**  PURPOSE:    External API to pass the payload to be modified to the observer
**              at the last minute prior to actually sending out the packet, hence
**              the 'last chance' denomination.
**
**  PARAMETERS:
**              ppNewData - the new data information to use as replacement of the
**                          current data.  This pointer must be assigned to some
**                          valid value or the replacement data will be ignored.
**              destInfo - the destination information associated with this
**                         data, this is the resolved address of the target.
**              pData    - the data currently being sent, if ppNewData does not
**                         reallocate some new data, the pData data is the one
**                         used.
**              uSize    - the size of the current data (pData).
**
**  RETURNS:    The size of the new data (ppNewData) to be consider for sending
**              or zero if no data is reallocated.
**
**  NOTES:      The newly reallocated data will live untouched until the next
**              call to pmLastChance is placed during which the data may be
**              replaced.  Therefore the information returned by this call is
**              not meant to be cached by the observer.  If caching must take
**              place, it should take place at the layer calling this API.
***********************************************************************************/
BOS_UINT32 pmLastChance( BOS_UINT8 **ppNewData,
                         CCTKIPPORT *destInfo,
                         const BOS_UINT8 *pData,
                         BOS_UINT32 uSize );

#ifdef __cplusplus
}
#endif

#endif /* __PM_API__H__INCLUDED__ */
