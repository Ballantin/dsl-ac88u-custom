/**********************************************************************************
** Copyright (c) 2010-2012 Broadcom
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
**      This file contains the CCTK compile-time configuration headers.
**
***********************************************************************************/

#ifndef __CCTK_CFG__H__INCLUDED__
#define __CCTK_CFG__H__INCLUDED__

/* ---- Constants and Types ----------------------------------------------------- */
/* --- FEATURES --- */
/* 
** -- CCTK operation --
** This feature allows control over CCTK's operational behaviour.
** CCTKCFG_CCTK_MODE must be defined with one of the following:
** - CCTKCFG_CCTK_SIP
**    SIP call flow support is enabled.
** - CCTKCFG_CCTK_IMS
**    IMS (and SIP) call flow support is enabled.
**    Enabling this facility MUST enable:
**      - CCTKCFG_NETWORK_ASSERTED_ID
**      - CCTKCFG_PRIVACY
**      - CCTKCFG_TEL_MODE
*/
#define CCTKCFG_CCTK_SIP                                 0
#define CCTKCFG_CCTK_IMS                                 1
#define CCTKCFG_CCTK_MODE                                CCTKCFG_CCTK_SIP

/*
** -- IPv6 facility --
** This feature allows control over IPv6 support.
**
** If ENABLED,
**   SIP stack:   Run in IPv4/IPv6 hybrid mode.
**   Sending:     Allow sending to remote party with IPv4 or
**                IPv6 IP addresses.
**   Receiving:   Allow listening to IPv4 and IPv6
**                IP addresses, depending on system config.
**
** If DISABLED,
**   SIP stack:   Run in IPv4 mode.
**   Sending:     Allow sending to remote party with IPv4
**                IP addresses only.
**   Receiving:   Allow listening to IPv4 IP addresses only.
*/
#define CCTKCFG_IPV6                                     0

/*
** -- TLS facility --
** This feature allows control over TLS transport support.
*/
#define CCTKCFG_TLS                                      0

/*
** -- Proxy Failover facility --
** This feature allows control on failover mechanism when a proxy cannot
** be reached for a given SIP request.
*/
#define CCTKCFG_PROXY_FAILOVER                           0

/*
** -- Memory Monitoring facility --
** This feature allows control over memory usage monitoring.
*/
#define CCTKCFG_MEMORY_MONITORING                        0

/*
** -- Telephone call facility --
** This feature allows control over telephone call facility,
** including both Tel-URI and electronic numbering (ENUM) service.
** CCTKCFG_TEL_URI_MODE must be defined with one of the following:
** - CCTKCFG_TEL_DISABLED
**    Both Tel-URI and ENUM supports are disabled.
** - CCTKCFG_TEL_ALL
**    Tel-URI is enabled with ENUM service.
** - CCTKCFG_TEL_WITHOUT_ENUM
**    Tel-URI is enabled without ENUM service.
**    Enabling CCTKCFG_TEL_WITHOUT_ENUM must ensure outbound
**    proxy is configured when setting up a telephone call.
*/
#define CCTKCFG_TEL_DISABLED                             0
#define CCTKCFG_TEL_ALL                                  1
#define CCTKCFG_TEL_WITHOUT_ENUM                         2
#define CCTKCFG_TEL_MODE                                 CCTKCFG_TEL_DISABLED

/*
** -- Session Timer facility --
** This feature allows control over the session timer facility
** defined in RFC4028.
*/
#define CCTKCFG_SESSION_TIMER                            0

/*
** -- Privacy facility --
** This feature allows control over the SIP privacy facility
** defined in RFC3323 and RFC3324.
*/
#define CCTKCFG_PRIVACY                                  0

/*
** -- Network Asserted Identity facility --
** This feature allows control over the network-asserted identity facility
** defined in RFC3325.
*/
#define CCTKCFG_NETWORK_ASSERTED_ID                      0

/*
** -- Video SDP Negotiation facility --
** This feature allows control over the support of video
** SDP offer/answer negotiation
*/
#define CCTKCFG_VIDEO                                    0

/*
** -- SRTP facility --
** This feature allows control over using a=crypto in SDP
** for SRTP key negotiation.
*/
#define CCTKCFG_SRTP                                     0

/*
** -- Debug facility --
** This feature allows control over the CCTK's debug facility.
** Enabling this facility MUST enable CCTKCFG_ASSERT_MODE.
*/
#define CCTKCFG_DEBUG                                    0

/*
** -- Application Status facility --
** This feature allows control over the ability for the publishing
** dynamic application information via the log system.
*/
#define CCTKCFG_APPLICATION_STATUS                       0

/*
** -- Keep Alive facility --
** This feature allows control over signaling keep alive.
*/
#define CCTKCFG_KEEP_ALIVE                               0


/*
** -- Assert facility --
** This feature allows control over the assert facility.
** CCTKCFG_ASSERT_MODE must be defined with one of the following:
** - CCTKCFG_ASSERT_DISABLED
**    Asserts are disabled.
** - CCTKCFG_ASSERT_ALL
**    Asserts are enabled in both CCTK and the SIP stack.
** - CCTKCFG_ASSERT_CCTK
**    Asserts are enabled in CCTK only. Asserts in the SIP stack are disabled.
*/
#define CCTKCFG_ASSERT_DISABLED                          0
#define CCTKCFG_ASSERT_ALL                               1
#define CCTKCFG_ASSERT_CCTK                              2
#define CCTKCFG_ASSERT_MODE                              CCTKCFG_ASSERT_DISABLED

/*
** -- Tracing facility --
** This feature allows control over the tracing facility.
** CCTKCFG_TRACE_MODE must be defined with one of the following:
** - CCTKCFG_TRACE_DISABLED
**    Tracing is disabled.
** - CCTKCFG_TRACE_ALL
**    Tracing is enabled in both CCTK and the SIP stack.
** - CCTKCFG_TRACE_CCTK
**    Tracing is enabled in CCTK only. Tracing in the SIP stack is disabled.
*/
#define CCTKCFG_TRACE_DISABLED                           0
#define CCTKCFG_TRACE_ALL                                1
#define CCTKCFG_TRACE_CCTK                               2
#define CCTKCFG_TRACE_MODE                               1


/*
** -- Remote User Control Facility --
** This feature allows remote user to REGISTER and be controlled via the local
** CCTK application.  The CCTK plays a proxy role in this case for those users.
*/
#define CCTKCFG_REMOTE_USER_CONTROL                      0

/*
** -- GRUU Support --
** This feature is not fully supported currently and therefore disabled by default.
*/
#define CCTKCFG_GRUU_SUPPORT                             0

/*
** -- Incoming Dialing by IP Facility --
** This feature allows remote user to call the CCTK application via IP dialing
** the IP address recognized is the first address on which the CCTK application
** runs.  When a call by IP is received, it is automatically directed to the
** first user on the system.
*/
#define CCTKCFG_IP_ADDRESS_DIALING                       0


/*
** -- Message Pass Through Facility --
** This feature allows the CCTK to send and receive MESSAGE data in a pass
** through mode, meaning no attempt is made to understand the message payload
** and the application is fully responsible to manage the payload as well as
** the final response to be given to the request.
*/
#define CCTKCFG_MESSAGE_PASS_THROUGH                     0


/* --- CONSTANTS --- */
/*
** -- Maximum number of host addresses
** This value limits the maximum number of configured host addresses.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_HOSTADDR                             0

/*
** -- Maximum number of access networks
** This value limits the maximum number of accessible network for a given
** network interface address. MUST be at least 1.
*/
#define CCTKCFG_MAX_ACCESSNET                            0

/*
** -- Maximum number of outbound proxies
** This value limits the maximum number of configured outbound proxies.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_OBPROXY                              0

/*
** -- Maximum number of realms for authentication per user
** This value limits the maximum number of realms for authentication.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_REALM                                0

/*
** -- Maximum size of an address list
** This value limits the maximum size of an address list.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_ADDRLST                              0

/*
** -- Maximum size of a generic header list
** This value limits the maximum size of a generic header list.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_GENHDRLST                            0

/*
** -- Maximum size of a profile configuration list
** This value limits the maximum size of a profile configuration list.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_PROFCFGLST                           0

/*
** -- Maximum number of CCTK users
** This value limits the maximum number of CCTK users.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_USR                                  0

/*
** -- Maximum number of *virtual* CCTK users
** This value limits the maximum number of *virtual* CCTK users.
**
** Virtual users are user resources that may be needed to provide some
** specific services to an application but which would be associated with
** no other resources (as example no calls can be associated with virtual
** users).  CCTK will adjust the total number of system users supported
** based on CCTKCFG_MAX_VIRTUAL_USR and CCTKCFG_MAX_USR.
** CAN be NULL.
*/
#define CCTKCFG_MAX_VIRTUAL_USR                          0

/*
** -- Maximum number of calls
** This value limits the maximum number of simultaneous calls.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_CALL                                 0

/*
** -- Maximum number of event subscriptions
** This value limits the maximum number of simultaneous event subscriptions.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_EVT                                  0

/*
** -- Maximum number of notifications
** This value limits the maximum number of simultaneous notifications.
** Notifications are for sending or receiving information not associated
** with a call or a subscription.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_NOT                                  0

/*
** -- Maximum number of CCTK call data records
** This value limits the maximum number of CCTK call data records shared
** amongst all users.
** MUST be at least CCTKCFG_MAX_USR, in practical terms, this value should
** be a factor of CCTKCFG_MAX_USR such that each user is treated equally and
** there is no wasted resource.
*/
#define CCTKCFG_MAX_CDR                                  0

/*
** -- Maximum number of transport address
** This value limits the maximum number of transport address.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_TRANSPORT_ADDRESS                    1

/*
** -- Maximum number of audio codecs
** This value limits the maximum number of audio codecs.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_AUDIO_CODEC                          0

/*
** -- Maximum number of video codecs
** This value limits the maximum number of video codecs.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_VIDEO_CODEC                          0

/*
** -- Maximum number of supported crypto suite
** This value limits the maximum number of crypto suites.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_CRYPTO                               0

/*
** -- Maximum number of crypto session parameters
** This value limits the maximum number of crypto session parameters.
** MUST be at least 0.
*/
#define CCTKCFG_MAX_CRYPTO_SESPARM                       0

/*
** -- Maximum number of assumed identity(ies)
** This value limits the maximum number of assumed identity a user can
** manage.  Assumed identity is an identity that is different from the 
** primary (configured) user identity but that can be used to contact the
** same user.  Assumed identity information is typically received through
** network signaling following a successful user registration.
** MUST be at least 0.
*/
#define CCTKCFG_MAX_ASSUMED_ID                           0

/*
** -- Maximum number of backup and recovery item(s)
** This value limits the number of items that can be considered as part of
** the backup and recovery feature.  Setting this maximum value means that the
** feature can only manage up to that amount of raw information data in order
** to apply its processing.  Each item would correspond to one single destination
** tuple (IP address, port, protocol) for the backup and recovery feature.
*/
#define CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS                0

/*
** -- Maximum number of alert information
** This value limits the maximum number of alert information the call can
** deal with at once.  Alert information is provided by the SIP Alert-Info
** header and is used to direct the local signaling to the alerted user.
** MUST be at least 1.
*/
#define CCTKCFG_MAX_ALERT_INFO                           0

/*
** -- The size (depth) of the resource state history.
** This value defines the number of state (including current and
** passed) that a resource keeps track of.
** MUST be at least 2.
*/
#define CCTKCFG_RES_STATE_DEPTH                          0

/*
** -- The size (depth) of the resource event history.
** This value defines the number of event (including current and
** passed) that a resource keeps track of.
** MUST be at least 2.
*/
#define CCTKCFG_RES_EVENT_DEPTH                          0

/*
** -- The size (depth) of the resource finite state machine history.
** This value defines the number of finite state machine (including current and
** passed) that a resource keeps track of.
** MUST be at least 2.
*/
#define CCTKCFG_RES_FSM_DEPTH                            0

/* Specify the value to include in CCTKCNXCFG when one wants to use the 
 * default values in CCTK. The default values are CFGENGINE_MIN_CNX (5), 
 * CFGENGINE_MAX_CNX (10) and CFGENGINE_MAX_CNX_IDLE_TIME (64).
 * If the values in CCTKCNXCFG are not CCTKCFG_DEFAULT_CNX_CFG, then those
 * values will be used. For example if uMinCnx is set to 4, the value of 4
 * will be assigned instead of the default value of 5.
*/
#define CCTKCFG_DEFAULT_CNX_CFG                          0

/*
** -- Enable CACHING of DNS responses in the stack
*/
#define CCTKCFG_RESOLVER_CACHE_ENABLE                    0

/*
** -- Enable allocation of Random RTP port numbers
*/
#define CCTKCFG_RANDOM_PORTS                             0

/*
** -- ICE-LITE Support --
*/
#define CCTKCFG_ICE_LITE_SUPPORT                         0

/*
** -- Enable different hold method
*/
#define CCTKCFG_MULTI_HOLD_METHOD                        0

/* ---- Variable Externs -------------------------------------------------------- */
/* ---- Function Prototypes ----------------------------------------------------- */

/* ---- Include Files ----------------------------------------------------------- */
/*
** CUSTOM CONFIGURATION - This must be included after all default
** build configuration options. Custom include files should undef default
** config options and then re-define them.
*/
#include <cctkCfgCustom.h>

/* Configuration Verification */
#include <cctkCfgChk.h>

#endif /* __CCTK_CFG__H__INCLUDED__ */
