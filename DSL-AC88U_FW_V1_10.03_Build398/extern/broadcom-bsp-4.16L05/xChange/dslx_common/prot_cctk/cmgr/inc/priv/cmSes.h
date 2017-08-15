/**********************************************************************************
** Copyright (c) 2008-2012 Broadcom
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
**      This file contains the Call Manager session resource.
**
***********************************************************************************/

#ifndef __CM_SES__H__INCLUDED__
#define __CM_SES__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>

/* ---- Constants and Types ----------------------------------------------------- */

#define CMSES_MEDIA_OFFSET                 0x10000000
#define CMSES_SERVICE_OFFSET               0x20000000

#define CMSES_MEDIA_ACT_MON_THRESH_MSEC    500

/* Defines the session resource type.
*/
typedef enum
{
   eCMSESTYPE_NONE,
   eCMSESTYPE_MEDIA,
   eCMSESTYPE_SRV,

} CMSESTYPE;

/* Define some sub-state information related to auto-recovery.
*/
typedef enum
{
   eCMSESAUTOREC_NONE,
   eCMSESAUTOREC_LCL,   /* Local feedback only. */
   eCMSESAUTOREC_TPF,   /* Third party feedback. */

} CMSESAUTOREC;

/* Defines some information relative to a fax relay session.
*/
typedef struct
{
   BOS_BOOL bFailed;
   BOS_BOOL bAttempt;
   BOS_BOOL bOngoing;
   BOS_BOOL bDeferEnd;

} CMFAXRELAY;

/* Defines the media session control block.
*/
typedef struct
{
   BOS_UINT32 uPhy;           /* The physical handle identifying the resource.    */
   BOS_BOOL bFocus;           /* The focus session flag.  When BOS_TRUE, the      */
                              /* session is the primary session used by the       */
                              /* local user and is the focus of all actions.      */
   BOS_BOOL bEmergency;       /* Whether this is an emergency session or not.     */
   BOS_BOOL bEmgExpired;      /* Whether this is an expired emergency session     */
                              /* or not.                                          */
   BOS_BOOL bMediaRx;         /* Whether media was received for the session       */
   BOS_BOOL bCfv;             /* Whether this is a call forward activation        */
                              /* session or not.                                  */
   BOS_BOOL bHotline;         /* Whether this is a hotline (or warmline) session  */
                              /* or not.                                          */
   BOS_BOOL bHeld;            /* Whether this session is held remotely            */
   BOS_BOOL bRemInit;         /* Whether this session was initiated by peer       */
   BOS_BOOL bMediaKAM;        /* Whether media keep alive monitoring expected     */
   BOS_BOOL bNeedFinal;       /* Whether final media needs to be signaled or not  */
   BOS_BOOL bFastTrac;        /* Whether media fast track was used             */
   BOS_UINT8 uDigCol[CMGR_COLLECTED_DIGIT_LEN + 1]; /* Collected digits.       */
   BOS_UINT32 uDigCnt;        /* Number of collected digits.                   */
   BOS_UINT32 uConf;          /* The handle identifying the conference linked. */
   BOS_UINT32 uBargeIn;       /* The handle identifying the session this session  */
                              /* is barging into                                  */
   CCTKCALLINFO callInfo;     /* Call information for the session.             */
   CMFAXRELAY faxRelay;       /* Fax specific sub-state information.           */
   CMDMACTIONTYPE eAction;    /* Pending/current digit map action associated.  */
   void *pAutoCall;           /* Auto-call resource reference.                 */
   CMSESAUTOREC eAutoRec;     /* Auto-recovery sub-state information.          */
   BOS_BOOL bBgFocus;         /* The background focus session flag. When BOS_TRUE */
                              /* the session is a background session associated   */
                              /* with the user, primarily for remote association. */
   void *pLinkedSes;          /* A session linked to this one.                    */

} CMSESMED;

/* Defines the service session control block.
*/
typedef struct
{
   BOS_UINT8 uSrv;
   void *pAutoCall;

} CMSESSRV;

#if CMGRCFG_FIREWALL_CONTROL
/* Defines the media port firewall info.
*/
typedef struct
{
   BOS_UINT32 localRtpPort;      /* Local RTP port */
   BOS_UINT32 localRtcpPort;     /* Local RTCP port */
   BOS_UINT32 localT38Port;      /* Local T38 port */
   BOS_IP_ADDRESS remoteAddr;    /* IP address of remote source */
} CMSESMEDFWINFO;
#endif /* CMGRCFG_FIREWALL_CONTROL */

/* Defines the session resource control block.
*/
typedef struct
{
   BOS_UINT32 uHdl;           /* The local handle identifying this resource. */
   BOS_UINT32 uUsr;           /* The user handle associated to this resource. */

   BOS_UINT32 uNet;           /* The network handle identifying this resource. */
   BOS_UINT32 uApp;           /* The application handle identifying
                              ** this resource. */
   BOS_UINT32 uNetUsr;        /* The network user handle associated with this
                              ** resource. */

   CMSESTYPE  eType;

   BOS_BOOL   bEstab;         /* Flag to track session establishement. */
   BOS_BOOL   bForwarded;     /* Flag to track if this was a session forwaded 
                              ** from another user */
   BOS_BOOL   bMaintdOnOos;   /* Flag to track established session maintained on out-of-service
                              ** event. */

   union
   {
      CMSESMED med;
      CMSESSRV srv;

   } ses;

#if CMGRCFG_FIREWALL_CONTROL
   CMSESMEDFWINFO sesMedFwInfo;
#endif /* CMGRCFG_FIREWALL_CONTROL */
   
} CMSES;

/* Defines a list of session handles
*/
typedef struct
{
   BOS_UINT32 uLength;
   BOS_UINT32 uSes[ CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX ];

} CMSESLIST;

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   cmSes
**
** PURPOSE:    Manages the session resources.
**
** PARAMETERS: bCreate - Whether resources should be created (BOS_TRUE) or ended
**                       (BOS_FALSE) when this call is invoked.
**
** RETURNS:    None.
***********************************************************************************/
void cmSes ( BOS_BOOL bCreate );


/***********************************************************************************
** FUNCTION:   cmSesAlloc
**
** PURPOSE:    Allocates a session resources to a specific user for usage by the
**             application
**
** PARAMETERS: eType - the type of session to be used.
**             uHdl - the user handle for which this resource is allocated.
**
** RETURNS:    A valid pointer to the resource allocated, or NULL if no resource
**             could be allocated.
**
** NOTE:       This function does *not* allocate physical memory, it fetches a
**             free resource from a pool.
***********************************************************************************/
CMSES * cmSesAlloc ( CMSESTYPE eType, BOS_UINT32 uHdl );


/***********************************************************************************
** FUNCTION:   cmSesFree
**
** PURPOSE:    Frees a session resources from a specific user.
**
** PARAMETERS: pSes - the session resource to be freed.
**             uHdl - the user handle freeing this resource.
**
** RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
**
** NOTE:       This function does *not* free physical memory, it returns a
**             resource to a pool.
***********************************************************************************/
BOS_BOOL cmSesFree ( CMSES *pSes, BOS_UINT32 uHdl );


/***********************************************************************************
** FUNCTION:   cmSesFindNetHdl
**
** PURPOSE:    Finds a session based on its network handle.
**
** PARAMETERS: uNet - the network handle to check for.
**             uUsr - the user handle for which to check.
**
** RETURNS:    Pointer to the session block if found, NULL otherwise.
**
** NOTE:       The network handle is the only way we can identify events for a
**             given session when reported by the CCTK layer.
***********************************************************************************/
CMSES * cmSesFindNetHdl ( BOS_UINT32 uNet, BOS_UINT32 uUsr );


/***********************************************************************************
** FUNCTION:   cmSesFindPhyHdl
**
** PURPOSE:    Finds a session based on its physical (endpoint layer) handle.
**
** PARAMETERS: uPhy - the physical handle to check for.
**
** RETURNS:    Pointer to the session block if found, NULL otherwise.
**
** NOTE:       The physical handle is the only way we can identify events for a
**             given session when reported by the endpoint (physical) layer.
***********************************************************************************/
CMSES * cmSesFindPhyHdl ( BOS_UINT32 uPhy );


/***********************************************************************************
** FUNCTION:   cmSesFindAppHdl
**
** PURPOSE:    Finds a session based on its application handle.
**
** PARAMETERS: uApp - the application handle to check for.
**
** RETURNS:    Pointer to the session block if found, NULL otherwise.
**
** NOTE:       The application handle is the only way we can identify events for a
**             given session when reported by the application (external) layer.
***********************************************************************************/
CMSES * cmSesFindAppHdl ( BOS_UINT32 uApp );


/***********************************************************************************
** FUNCTION:   cmSesValid
**
** PURPOSE:    Checks whether a session handle is for a valid session.
**
** PARAMETERS: uHdl - the session handle to check for.
**
** RETURNS:    Pointer to valid session or NULL if not found.
***********************************************************************************/
CMSES * cmSesValid ( BOS_UINT32 uHdl );


/***********************************************************************************
** FUNCTION:   cmSesMedValid
**
** PURPOSE:    Checks whether a session handle is for a valid media session.
**
** PARAMETERS: uHdl - the session handle to check for.
**
** RETURNS:    Pointer to valid session or NULL if not found.
***********************************************************************************/
CMSES * cmSesMedValid ( BOS_UINT32 uHdl );


/***********************************************************************************
** FUNCTION:   cmSesSrvValid
**
** PURPOSE:    Checks whether a session handle is for a valid service session.
**
** PARAMETERS: uHdl - the session handle to check for.
**
** RETURNS:    Pointer to valid session or NULL if not found.
***********************************************************************************/
CMSES * cmSesSrvValid ( BOS_UINT32 uHdl );


/***********************************************************************************
** FUNCTION:   cmSesClid
**
** PURPOSE:    Formats and creates the caller identification information for a
**             session.
**
** PARAMETERS: pSes - the session of interest.
**             bName - whether the name information should be passed with the CLID
**             bNumber - whether the number information should be passing with
**                       the CLID
**
** RETURNS:    None.
***********************************************************************************/
void cmSesClid ( CMSES *pSes, BOS_BOOL bName, BOS_BOOL bNumber );


/***********************************************************************************
** FUNCTION:   cmSesPhyCreate
**
** PURPOSE:    Creates the physical layer to support this session (i.e. the data
**             stream).
**
** PARAMETERS: pSes - the session of interest.
**
** RETURNS:    None.
***********************************************************************************/
void cmSesPhyCreate ( CMSES *pSes );


/***********************************************************************************
** FUNCTION:   cmSesPhyUpdate
**
** PURPOSE:    Updates the physical layer to support this session (i.e. the data
**             stream).
**
** PARAMETERS: pSes - the session of interest.
**             bFax - BOS_TRUE if this update is due to a fax event, BOS_FALSE
**                    otherwise.
**
** RETURNS:    None.
***********************************************************************************/
void cmSesPhyUpdate ( CMSES *pSes, BOS_BOOL bFax );


/***********************************************************************************
** FUNCTION:   cmSesPhyDelete
**
** PURPOSE:    Deletes the physical layer to support this session (i.e. the data
**             stream).
**
** PARAMETERS: pSes - the session of interest.
**
** RETURNS:    None.
***********************************************************************************/
void cmSesPhyDelete ( CMSES *pSes );


/***********************************************************************************
** FUNCTION:   cmSesPhyMute
**
** PURPOSE:    Mutes the physical layer taht support this session (i.e. the data
**             stream).
**
** PARAMETERS: pSes - the session of interest.
**
** RETURNS:    None.
***********************************************************************************/
void cmSesPhyMute ( CMSES *pSes );


/***********************************************************************************
** FUNCTION:   cmSesConferenceLocal
**
** PURPOSE:    Creates a locally-mixed conference between the two sessions identified.
**
** PARAMETERS: pSes_1 - the first session of interest.
**             pSes_2 - the second session of interest.
**
** RETURNS:    BOS_TRUE - if the conference could be setup between those two
**                        sessions.
**             BOS_FALSE - otherwise.
***********************************************************************************/
BOS_BOOL cmSesConferenceLocal ( CMSES *pSes_1, CMSES *pSes_2 );


/***********************************************************************************
** FUNCTION:   cmSesTerminate
**
** PURPOSE:    Terminates a session being used.
**
** PARAMETERS: pSes - the session resource to be freed.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmSesTerminate ( CMSES *pSes );


/***********************************************************************************
** FUNCTION:   cmSesGetDscp
**
** PURPOSE:    Gets the DSCP configuration information for this session.
**
** PARAMETERS: pSes - the session resource to be queried for.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
BOS_UINT32 cmSesGetDscp ( CMSES *pSes );


/***********************************************************************************
** FUNCTION:   cmSesGetStats
**
** PURPOSE:    Gets the statistics collected for this session.
**
** PARAMETERS: pSes - the session resource to be queried for.
**             pStats - the statistics information to return.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmSesGetStats ( CMSES *pSes, CCTKCALLMETRICS *pStats );


/***********************************************************************************
** FUNCTION:   cmSesPhyActMonThresh
**
** PURPOSE:    Setup the activity monitor threshold on the session, when set, the
**             session would report an event when media is no longer seen passed
**             the configured threshold.
**
** PARAMETERS: pSes - the session of interest.
**             uMsec - the threshold in milliseconds, 0 to disable monitoring.
**
** RETURNS:    None.
***********************************************************************************/
void cmSesPhyActMonThresh ( CMSES *pSes, BOS_UINT32 uMsec );


/***********************************************************************************
** FUNCTION:   cmSesMedXfer
**
** PURPOSE:    Updates the physical layer of one session with the media 
**             info of another session.
**
** PARAMETERS: pSes_1 - the existing phyiscal session of interest to be updated.
**             uNet_2 - the network handle identify the resource to retrieve the
**                      media information from.
**
** RETURNS:    None.
**
** NOTE:       This functionality is typically required during early media
**             configuration in call transfer only.
***********************************************************************************/
void cmSesMedXfer ( CMSES *pSes_1, BOS_UINT32 uNet_2 );


/***********************************************************************************
** FUNCTION:   cmNumSesWithCid
**
** PURPOSE:    Finds the number of sessions with this call-id
**
** PARAMETERS: uCid  - the network call id of interest.
**
** RETURNS:    Number of sessions having this network call id.
***********************************************************************************/
BOS_UINT32 cmNumSesWithCid( BOS_UINT32 uCid );


/***********************************************************************************
** FUNCTION:   cmNumSesWithNet
**
** PURPOSE:    Finds the number of sessions with this network handle
**
** PARAMETERS: uNet  - the network id of interest.
**
** RETURNS:    Number of sessions having this network id.
***********************************************************************************/
BOS_UINT32 cmNumSesWithNet( BOS_UINT32 uNet );

/***********************************************************************************
** FUNCTION:   cmNumSesBargedIn
**
** PURPOSE:    Finds the number of sessions that have barged in to this session
**
** PARAMETERS: uBargeHdl  - the session to find.
**
** RETURNS:    Number of sessions barged in to this session.
***********************************************************************************/
BOS_UINT32 cmNumSesBargedIn( BOS_UINT32 uBargeHdl );


/***********************************************************************************
** FUNCTION:   cmSesListWithCid
**
** PURPOSE:    Finds the list of sessions with the associated call id.
**
** PARAMETERS: uCid  (IN)  - the call id to use
**             pList (OUT) - the pointer to the CMSESLIST that we need to populate
**
** RETURNS:    None.
***********************************************************************************/
void cmSesListWithCid( BOS_UINT32 uCid, CMSESLIST* pList );


/***********************************************************************************
** FUNCTION:   cmSesListWithNet
**
** PURPOSE:    Finds the list of sessions with the associated network id.
**
** PARAMETERS: uNet  (IN)  - the network id to use
**             pList (OUT) - the pointer to the CMSESLIST that we need to populate
**
** RETURNS:    None.
***********************************************************************************/
void cmSesListWithNet( BOS_UINT32 uNet, CMSESLIST* pList );

/***********************************************************************************
** FUNCTION:   cmSesListWithBargeIn
**
** PURPOSE:    Finds the list of sessions that have barged-in to the session.
**
** PARAMETERS: uBargeHdl (IN)  - the barge-in session to find
**             pList (OUT) - the pointer to the CMSESLIST that we need to populate
**
** RETURNS:    None.
***********************************************************************************/
void cmSesListWithBargeIn( BOS_UINT32 uBargeHdl, CMSESLIST* pList );

/***********************************************************************************
** FUNCTION:   cmSesListRelease
**
** PURPOSE:    Posts a release event for each session in the list.
**
** PARAMETERS: pList(IN) - the pointer to the CMSESLIST with sessions to release
**             pSesExempt (IN) - If not NULL, session to exempt from release
**
** RETURNS:    None.
***********************************************************************************/
void cmSesListRelease( CMSESLIST* pList, CMSES *pSesExempt );




#if defined(__cplusplus)
}
#endif

#endif /* __CM_SES__H__INCLUDED__ */

