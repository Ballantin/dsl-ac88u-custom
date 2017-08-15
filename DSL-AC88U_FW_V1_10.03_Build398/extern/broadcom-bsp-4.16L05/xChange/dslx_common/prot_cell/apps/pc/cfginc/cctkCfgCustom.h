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

/*****************************************************************************
*
*    Description:
*
*      This file contains the custom configuration headers for cctkCfg.h.
*
*****************************************************************************/

#ifndef __CCTK_CFG_CUSTOM__H__INCLUDED__
#define __CCTK_CFG_CUSTOM__H__INCLUDED__

/* Override Default Configuration */

#undef CCTKCFG_CCTK_MODE
#define CCTKCFG_CCTK_MODE                                CCTKCFG_CCTK_IMS

#undef CCTKCFG_IPV6
#define CCTKCFG_IPV6                                     0

#undef CCTKCFG_TLS
#define CCTKCFG_TLS                                      0

#undef CCTKCFG_PROXY_FAILOVER
#define CCTKCFG_PROXY_FAILOVER                           1

#undef CCTKCFG_KEEP_ALIVE
#define CCTKCFG_KEEP_ALIVE                               0

#undef CCTKCFG_MEMORY_MONITORING
#define CCTKCFG_MEMORY_MONITORING                        1

#undef CCTKCFG_TEL_MODE
#define CCTKCFG_TEL_MODE                                 CCTKCFG_TEL_ALL

#undef CCTKCFG_SESSION_TIMER
#define CCTKCFG_SESSION_TIMER                            1

#undef CCTKCFG_PRIVACY
#define CCTKCFG_PRIVACY                                  1

#undef CCTKCFG_NETWORK_ASSERTED_ID
#define CCTKCFG_NETWORK_ASSERTED_ID                      1

#undef CCTKCFG_VIDEO
#define CCTKCFG_VIDEO                                    1

#undef CCTKCFG_SRTP
#define CCTKCFG_SRTP                                     1

#undef CCTKCFG_DEBUG
#define CCTKCFG_DEBUG                                    1

#undef CCTKCFG_APPLICATION_STATUS
#define CCTKCFG_APPLICATION_STATUS                       1

#undef CCTKCFG_ASSERT_MODE
#define CCTKCFG_ASSERT_MODE                              CCTKCFG_ASSERT_ALL

#undef CCTKCFG_TRACE_MODE
#define CCTKCFG_TRACE_MODE                               CCTKCFG_TRACE_ALL

#undef CCTKCFG_MAX_HOSTADDR
#define CCTKCFG_MAX_HOSTADDR                             2

#undef CCTKCFG_MAX_ACCESSNET
#define CCTKCFG_MAX_ACCESSNET                            2

#undef CCTKCFG_MAX_OBPROXY
#define CCTKCFG_MAX_OBPROXY                              2

#undef CCTKCFG_MAX_REALM
#define CCTKCFG_MAX_REALM                                2

#undef CCTKCFG_MAX_ADDRLST
#define CCTKCFG_MAX_ADDRLST                              2

#undef CCTKCFG_MAX_GENHDRLST
#define CCTKCFG_MAX_GENHDRLST                            2

#undef CCTKCFG_MAX_PROFCFGLST
#define CCTKCFG_MAX_PROFCFGLST                           3

#undef CCTKCFG_MAX_USR
#define CCTKCFG_MAX_USR                                  5

#undef CCTKCFG_MAX_CALL
#define CCTKCFG_MAX_CALL                                 10

#undef CCTKCFG_MAX_EVT
#define CCTKCFG_MAX_EVT                                  20

#undef CCTKCFG_MAX_NOT
#define CCTKCFG_MAX_NOT                                  5

#undef CCTKCFG_MAX_CDR
#define CCTKCFG_MAX_CDR                                  3 * CCTKCFG_MAX_USR

#undef CCTKCFG_MAX_AUDIO_CODEC
#define CCTKCFG_MAX_AUDIO_CODEC                          6

#undef CCTKCFG_MAX_VIDEO_CODEC
#define CCTKCFG_MAX_VIDEO_CODEC                          3

#undef CCTKCFG_MAX_CRYPTO
#define CCTKCFG_MAX_CRYPTO                               2

#undef CCTKCFG_MAX_CRYPTO_SESPARM
#define CCTKCFG_MAX_CRYPTO_SESPARM                       2

#undef CCTKCFG_MAX_ASSUMED_ID
#define CCTKCFG_MAX_ASSUMED_ID                           4

#undef CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS
#define CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS                5

#undef CCTKCFG_MAX_ALERT_INFO
#define CCTKCFG_MAX_ALERT_INFO                           4

#undef CCTKCFG_RES_STATE_DEPTH
#define CCTKCFG_RES_STATE_DEPTH                          4

#undef CCTKCFG_RES_EVENT_DEPTH
#define CCTKCFG_RES_EVENT_DEPTH                          8

#undef CCTKCFG_RES_FSM_DEPTH
#define CCTKCFG_RES_FSM_DEPTH                            4

/* In order to use the Win32 application with the Cable PCSim2 IMS
** simulator, one need to define this element such that the application
** is setup properly.
**
** Defining this element will enable the correct CCTK features as well
** as setup the callclient.cpp configuration to create one single user
** to be used with the simulator.
**
** Other than this #define, the CMGR application MUST also be configured to
** use the IMS profile (which is the default configuration).
*/
//#define CCTKCFG_USE_CABLE_SIMULATOR

#endif /* __CCTK_CFG_CUSTOM__H__INCLUDED__ */
