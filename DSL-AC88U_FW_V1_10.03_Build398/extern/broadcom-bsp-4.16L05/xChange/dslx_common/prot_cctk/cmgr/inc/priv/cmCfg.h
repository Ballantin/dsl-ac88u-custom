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
**      This file contains the Call Manager configuration functionality.
**
***********************************************************************************/

#ifndef __CM_CFG__H__INCLUDED__
#define __CM_CFG__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>
#include <cmgrCfg.h>    /* for CMGRCFG_TLS */
#include <cmSes.h>

/* ---- Constants and Types ----------------------------------------------------- */
#define CM_CFG__KEEP_ALIVE_TIMER__FIXED         24000
#define CM_CFG__KEEP_ALIVE_TIMER__VARMOD        6
#define CM_CFG__KEEP_ALIVE_DEFAULT_PORT         5060

#define CM_CFG__SUB_RETRY_TIMER                 600

/* ---- Public variables -------------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   cmCfgInit
**
** PURPOSE:    Initializes the configuration interface and configuration elements.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if we have successfully initialized our configuration.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgInit ( void );


/***********************************************************************************
** FUNCTION:   cmCfgDenit
**
** PURPOSE:    Deinitializes cmCfg
**
** PARAMETERS: None.
**
** RETURNS:    Nothing
**
***********************************************************************************/
void cmCfgDeinit ( void );


/***********************************************************************************
** FUNCTION:   cmCfgIsCctkInit
**
** PURPOSE:    Checks whether the CCTK (network layer) was configured and
**             initialized at any point in time.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if CCTK had been initialized.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgIsCctkInit ( void );


/***********************************************************************************
** FUNCTION:   cmCfgUser
**
** PURPOSE:    Configure the actual system users.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if we have successfully initialized our users.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgUser ( void );


/***********************************************************************************
** FUNCTION:   cmCfgUserAdd
**
** PURPOSE:    Configure a specific user.
**
** PARAMETERS: pRes - the user resource to configure.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that the user has been allocated its user
**             identification already, which is necessary to configure a user
**             completely.
***********************************************************************************/
void cmCfgUserAdd ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmCfgAudio
**
** PURPOSE:    Configure the audio settings for a given user to be used by the
**             CCTK layer.
**
** PARAMETERS: pRes - the resource to be configured.
**             pSes - session on which to configure audio
**             pCfgAudio - where to store the configuration information.
**
** RETURNS:    BOS_TRUE if we have successfully configured.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgAudio ( void *pRes, CMSES* pSes, CCTKMEDAUD *pCfgAudio );


/***********************************************************************************
** FUNCTION:   cmCfgFax
**
** PURPOSE:    Configure the fax settings for a given user to be used by the
**             CCTK layer.
**
** PARAMETERS: pRes - the resource to be configured.
**             pCfgFax - where to store the configuration information.
**             pbEnable - returned BOS_TRUE if fax configuration is supported,
**                        BOS_FALSE otherwise.
**
** RETURNS:    BOS_TRUE if we have successfully configured.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgFax ( void *pRes, CCTKMEDFAX *pCfgFax, BOS_BOOL *pbEnable );


#if CMGRCFG_VIDEO_MEDIA
/***********************************************************************************
** FUNCTION:   cmCfgVideo
**
** PURPOSE:    Configure the video settings for a given user to be used by the
**             CCTK layer.
**
** PARAMETERS: pRes - the resource to be configured.
**             pCfgVideo - where to store the configuration information.
**
** RETURNS:    BOS_TRUE if we have successfully configured.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgVideo ( void *pRes, CCTKMEDVID *pCfgVideo );
#endif


/***********************************************************************************
** FUNCTION:   cmCfgInitCallMedia
**
** PURPOSE:    Configure the media information to be used by the CCTK during
**             call negotiation.  This is done before any outgoing call as well
**             as before accepting any incoming call.  This gives the most up to
**             date and accurate view of the media capabilities.
**
** PARAMETERS: pRes - the resource to be configured.
**             pSes - the session to be configured.
**
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmCfgInitCallMedia ( void *pRes, CMSES *pSes );


#if CMGRCFG_TLS
/***********************************************************************************
** FUNCTION:   cmCfgTLS
**
** PURPOSE:    Configure the TLS Configuration.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if we have successfully configured TLS.
**             BOS_FALSE otherwise.
**
** NOTES:      - This function only configures the CCTK and need to call cctkConfig()
**             - to propagate the configuration into the SIP stack.
**
***********************************************************************************/
BOS_BOOL cmCfgTLS ( void );
#endif /* CMGRCFG_TLS */


#if CMGRCFG_REMOTE_USER_CONTROL
/***********************************************************************************
** FUNCTION:   cmCfgRemUsrSys
**
** PURPOSE:    Configure the SIP remote user system configuration.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if we have successfully configured.
**             BOS_FALSE otherwise.
**
** NOTES:      - This function only configures the CCTK and need to call cctkSetParam()
**             - to propagate the configuration into the SIP stack.
**
***********************************************************************************/
BOS_BOOL cmCfgRemUsrSys ( void );
#endif /* CMGRCFG_REMOTE_USER_CONTROL */


#if CMGRCFG_MULTILINE
/***********************************************************************************
** FUNCTION:   cmCctkAccntAdd
**
** PURPOSE:    Configure a specific CCTK account
**
** PARAMETERS: pRes - the CCTK account to configure.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that the account has been allocated its user
**             identification already, which is necessary to configure the account
**             completely.
***********************************************************************************/
void cmCctkAccntAdd ( void *pRes );
#endif /* CMGRCFG_MULTILINE */

#if defined(__cplusplus)
}
#endif

#endif /* __CM_CFG__H__INCLUDED__ */

