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
**      This file contains the Call Manager (CMGR) API.
**
***********************************************************************************/

#ifndef __CMGR_API__H__INCLUDED__
#define __CMGR_API__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>
#include <cmgrCfg.h>

#include <cctkApi.h>

#include <cmgrMainCfg.h>

#include <ept.h>
#include <cfgAccess.h>


/* ---- Constants and Types ----------------------------------------------------- */

/* Associate the Call Manager invalid handle to be the CCTK invalid handle.
** This is extremely useful for the CMGR <-> CCTK API functionality.
**
** WARNING: Changing this association may render the system useless and will
**          be root cause for unexplained errors.
*/
#define CMGR_INVALID_HANDLE                        CCTK_INVALID_HANDLE

/* Ensure the CLID size will support all the variants.  CLID is constructed
** as follows:
**                - Fixed size date/time (11 bytes) and a comma plus space -> 13.
**                - CLID number from CCTK, with quotes and a comma plus space
**                  -> 1 + CCTK_CLID_LEN + 1 + 2.
**                - CLID name from CCTK, with quotes -> 1 + CCTK_CLID_LEN + 1.
*/
#define CMGR_CLID_LEN           13 + (CCTK_CLID_LEN + 4) + (CCTK_CLID_LEN + 2)


/* Define the different class of events which can be handled by the Call Manager.
** This is used to differentiate the event source as well as distribute the event
** to the most appropriate handler.
*/
#define CMGR_EVT_CLASS__USR_CMD                    0x10000000
#define CMGR_EVT_CLASS__CCTK                       0x20000000
#define CMGR_EVT_CLASS__PHY_IF                     0x30000000

#define CMGR_EVT_CLASS__CLASS_MASK                 0xF0000000
#define CMGR_EVT_CLASS__EVT_MASK                   0x0FFFFFFF


/***********************************************************************************
**  FUNCTION:   CMGRSTATUSCB
**
**  PURPOSE:    Callback from Call Manager to deliver status information to the
**              outer application.
**
**  PARAMETERS: uRid - resource identifier, may be used in some specific status
**                     information when necessary.
**              eStatus - the status reported.
**
**  RETURNS:    Nothing.
**
***********************************************************************************/
typedef void ( *CMGRSTATUSCB )( BOS_UINT32 uRid,
                                CMGRSTATUS eStatus );


#if CMGRCFG_ANNOUNCEMENT
/***********************************************************************************
** FUNCTION:   CMGRANNCB
**
** PURPOSE:    Callback functionality for announcement control outside of
**             the Call Manager.
**
** PARAMETERS: eCmd - the announcement related command to be applied.
**             uUsr - the user for which this announcement applies.
**             eRes - the reason for the announcement action.
**
** RETURNS:    None.
**
** NOTE:
***********************************************************************************/
typedef void ( *CMGRANNCB )( CMGRANNCMD eCmd,
                             BOS_UINT32 uUsr,
                             CMGRANNRES eRes );
#endif


/***********************************************************************************
** FUNCTION:   CMGREPTCTLCB
**
** PURPOSE:    Callback functionality for endpoint action control outside of
**             the Call Manager.
**
** PARAMETERS: pCmd - the command to be passed to the endpoint.  This is a
**                    generic command interface which can be used for many
**                    purposes.
**
** RETURNS:    None.
**
** NOTE:
***********************************************************************************/
typedef void ( *CMGREPTCTLCB )( CMGREPCMD *pCmd );


/***********************************************************************************
** FUNCTION:   CMGREVTCB
**
** PURPOSE:    Callback functionality for event publication outside of
**             the Call Manager.
**
** PARAMETERS: eEvt  - the event of interest published.
**             uHdl  - the physical, user or application handle for which this 
**                     event is being published.
**             uUsr  - the user handle on which this event occurs (may be INVALID)
**             uLine - the line handle for which the event occurs (may be INVALID)
**             pData - additional data (may be NULL) associated to the
**                     published event.
**
** RETURNS:    None.
**
** NOTE:
***********************************************************************************/
typedef void ( *CMGREVTCB )( CMGREVT eEvt,
                             BOS_UINT32 uHdl,
#if CMGRCFG_MULTILINE
                             BOS_UINT32 uUsr,
                             BOS_UINT32 uLine,
#endif /* CMGRCFG_MULTILINE */
                             void *pData );


#if CMGRCFG_FIREWALL_CONTROL
/**********************************************************************************
** FUNCTION:   CMGRFWCB
**
** PURPOSE:    Callback functionality for firewall control outside of
**             the Call Manager.
**
** PARAMETERS: eCmd  - the firewall command to apply.
**             uUsr  - the user for which this command applies.
**             pData - additional data (may be NULL) necessary to control
**                     the firewall action of interest.
**                     When set, this is a CMGRFWINFO structure.
**
** RETURNS:    None.
**
** NOTE:
***********************************************************************************/
typedef void ( *CMGRFWCB )( CMGRFWCMD eCmd,
                            BOS_UINT32 uUsr,
                            void *pData );
#endif


/****************************************************************************
** FUNCTION:   CMGRPROVCB
**
** PURPOSE:    Callback functionality for provisioning control outside of
**             the Call Manager.
**
** PARAMETERS: cfgItem  - the provisioning item of interest.
**             pCfgIx   - the index associated with the provisioning item.
**             pData    - pointer to the placeholder for the provisioning
**                        information.
**             eCfgAct  - the provisioning action to take.
**
** RETURNS:    BOS_STATUS_OK on success, BOS_STATUS_ERROR otherwise.
**
** NOTE:
*****************************************************************************/
typedef BOS_STATUS ( *CMGRPROVCB )( PROV_CFG_ITEM cfgItem,
                                    PROV_CFG_IDX *pCfgIx,
                                    void *pData,
                                    CFG_ACCESS_ACT eCfgAct );


/****************************************************************************
** FUNCTION:   CMGRSESCTLCB
**
** PURPOSE:    Callback functionality for session control outside of
**             the Call Manager.
**
** PARAMETERS: eCmd  - the session command to apply.
**             uPhy  - the physical interface associated with the session.
**             uHdl  - the handle for which this command applies.
**             uCid  - the call id for this session
**             uLine - the line id for this session
**             pData - additional data (may be NULL) necessary to complete
**                     the action.
**
** RETURNS:    The reference index for the session of interest as allocated
**             by the outside world.
**
** NOTE:
*****************************************************************************/
typedef BOS_UINT32 ( *CMGRSESCTLCB )( CMGRSESCMD eCmd,
                                      BOS_UINT32 uPhy,
                                      BOS_UINT32 uHdl,
                                      BOS_UINT32 uCid,
#if CMGRCFG_MULTILINE
                                      BOS_UINT32 uLine,
#endif /* CMGRCFG_MULTILINE */
                                      void *pData );


/* CMGR configuration block information */
typedef struct
{
   CMGREPTCTLCB   cmgrEptCtlCb;
   CMGREVTCB      cmgrEvtCb;
   CMGRPROVCB     cmgrProvCb;
   CMGRSESCTLCB   cmgrSesCtlCb;
#if CMGRCFG_ANNOUNCEMENT
   CMGRANNCB      cmgrAnnCb;
#endif
#if CMGRCFG_FIREWALL_CONTROL
   CMGRFWCB       cmgrFwCb;
#endif

} CMGRCFGBLK;


/* ---- Function Prototypes ----------------------------------------------------- */

#if defined(__cplusplus)
extern "C"
{
#endif

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
void cmgrInit( CMGRSTATUSCB statusCb );

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
void cmgrDeinit( void );

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
void cmgrStartup( CMGRSTATUSCB statusCb, CMGRCFGBLK *pCfg );


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
void cmgrShutdown( void );


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
void cmgrCmd( CMGRCMD eCmd, void *pCmdInfo );


#if defined(__cplusplus)
}
#endif

#endif /* __CMGR_API__H__INCLUDED__ */

