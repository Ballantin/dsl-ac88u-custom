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
**      This file contains the CMGR compile-time configuration headers.
**
***********************************************************************************/

#ifndef __CMGR_CFG__H__INCLUDED__
#define __CMGR_CFG__H__INCLUDED__

/* ---- Features ---------------------------------------------------------------- */

/*
** -- Annoucement support --
**
** This feature allows control over external annoucement.  External annoucements
** are typically not driven by the network signaling and are of locale nature.
*/
#define CMGRCFG_ANNOUNCEMENT                                                  0


/*
** -- Redundancy support (RFC2198) --
**
** This feature allows control over redundancy as described in RFC2198.
*/
#define CMGRCFG_RED                                                           1

/*
** -- Firewall control --
**
** This feature allows control over an external to the Call Manager firewall.
** Typically a system wide firewall which the Call Manager needs to be able to
** setup accordingly in order for the calls to succeed.
*/
#define CMGRCFG_FIREWALL_CONTROL                                              0 

/*
** -- TLS Support  --
**
** This feature is used to control TLS support.
*/
#define CMGRCFG_TLS                                                           0

/*
** -- IPv6 Support  --
**
** This feature is used to control IPv6 support.
*/
#define CMGRCFG_IPv6                                                          0


/*
** -- Thread related information --
**
** The following configuration elements can be used to customize the Call Manager
** thread.
**
** Note that it is *NOT* recommended to change those values unless you are well
** aware of the consequences since mocking up with this information may render
** the system useless and non functional.
*/
#define CMGRCFG_THREAD_NAME                                   "CMGR"
#define CMGRCFG_THREAD_STACK                                  (32 * 1024)
#define CMGRCFG_THREAD_PRI                                    0
#define CMGRCFG_THREAD_SLEEP                                  50
#define CMGRCFG_THREAD_QDEPTH                                 20 /* Matches BOS. */
#define CMGRCFG_THREAD_QNAME                                  "CM-Q"
#define CMGRCFG_THREAD_SEMNAME                                "CM-S"
#define CMGRCFG_THREAD_SHUTDOWN_SEMNAME                       "CM-SDS"


/*
** -- Configuration profile  --
**
** This feature allows enabling of a specific configuration profile, which is an
** association between the configuration data and how it is used to produce an
** application which may behave differently based on this setting.
**
** Values are:
**
**      - CMGRCFG_CFG_PROFILE_RFC3261, this is the default RFC 3261 based
**        application configuration profile.
**
**      - CMGRCFG_CFG_PROFILE_IMS, this is the default IMS (3GPP TS 24.229) based
**        application configuration profile.
**
**      - CMGRCFG_CFG_PROFILE_STANDALONE, this is the application profile to use
**        when the application is to be run in standalone mode (neither of the
**        above).
**
**      - CMGRCFG_CFG_PROFILE_CUSTOM, this is room for expansion and specific
**        customization.
*/
#define CMGRCFG_CFG_PROFILE_RFC3261                                    1
#define CMGRCFG_CFG_PROFILE_IMS                                        2
#define CMGRCFG_CFG_PROFILE_STANDALONE                                 3
#define CMGRCFG_CFG_PROFILE_CUSTOM                                     4
#define CMGRCFG_CFG_PROFILE                      CMGRCFG_CFG_PROFILE_IMS


/*
** -- Outboud Signaling Keep Alive --
**
** The following configuration element determines whether to use keep alive for
** signaling or not.  Keep alive for signaling is implemented through the
** mechanisms of 'sip-outbound' (IETF draft).  It also setups the application to
** use the 'outbound' destination as a fixed destination until failure is
** encountered.
**
** For IMS configuration profile, it is MANDATORY to have it enabled in order to
** be compliant with the requirements of the IMS core, which are that once a target
** destination (P-CSCF) is found, it MUST continue to be used for any outgoing
** signaling until that target is determined unusable (failed).  Enabling this does
** not necessarely implies that a keep alive of some sort will be transmitted over
** the signaling link, but it will setup the link to the target as intended.
**
** Default configuration is 'enabled' since default profile configuration is IMS.
*/
#define CMGRCFG_OUTBOUND_KEEP_ALIVE                                           1


/*
** -- Remote User Control Facility --
** This feature allows the CMGR to manage remote user and remote user resources,
** in particular it means the CMGR will consider a registered remote user as its
** own resource and can use it to offer calls and services in conjunction with
** other users (local) on the system.
*/
#define CMGRCFG_REMOTE_USER_CONTROL                                           0

/*
** -- Video Media Facility --
** This feature allows the CMGR to enable support for video media.  By default
** video media is not supported as its support adds some quite significant chunk
** of resources that may not be needed in most cases.
*/
#define CMGRCFG_VIDEO_MEDIA                                                   0


/*
** -- Pass Thru Message Facility --
** This feature allows the CMGR to enable support for pass thru messages which 
** are messages handled by an external application.
*/
#define CMGRCFG_PASS_THRU_MSG                                                 0

/*
** -- Warmline activation via digitmap --
** This feature allows the warmline feature to be activated/deactivated 
** via VSCs in the digitmap.
*/
#define CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT                                 0
/* ---- Constants --------------------------------------------------------------- */

/*
** -- Maximum users --
**
** This defines the maximum number of users managed by the Call Manager at any
** one time.
*/
#define CMGRCFG_USER_MAX                                                      1


/*
** -- Maximum states and events --
**
** This defines the maximum number of states and events *PER USER*.
**
** Note that in order for the state machine to work properly, we require a
** minimum depth of two (2) for those values.
*/
#define CMGRCFG_STATE_MAX                                                     2
#define CMGRCFG_EVENT_MAX                                                     2


/*
** -- Maximum media session and service session --
**
** This defines the maximum number of media (i.e. call related) session and
** service (i.e. non call related) session managed *PER USER*.
*/
#define CMGRCFG_MEDSES_MAX                                                    3
#define CMGRCFG_SRVSES_MAX                                                    3


/*
** -- Maximum physical interface --
**
** This defines the maximum number of physical interface that can be managed
** at once.
*/
#define CMGRCFG_PHY_MAX                                                       2


/*
** -- Maximum auto-call tracking information --
**
** This defines the maximum number of automatic call session (auto-callback,
** auto-recall) that can be managed on a *PER USER* basis.
** 
** Auto-call resources are split evenly between auto-recall and auto-callback,
** so making this constant an even number is recommended to avoid wasting
** resources.
**
** Also to be noted that an auto-call resource requires a corresponding service
** session to work with (since auto-call involves establishing a service session
** to a remote peer to keep track of that peer activity), so in other words,
** CMGRCFG_SRVSES_MAX and CMGRCFG_AUTOCALL_MAX should be properly synch'ed.
*/
#define CMGRCFG_AUTOCALL_MAX                                                  2



/*
** -- Report debug information through CMGREVTCB --
**
** This enables the sending some internal debugging information to the 
** application via the CMGREVTCB. This is for debugging purposes use only.
** 
*/
#define CMGRCFG_DEBUG_INFO                                                    0


/*
** -- Show CMGR status information --
**
** This enables CMGR internal status information to be displayed to the
** user through the CMGR logging callback.
** 
*/
#define CMGRCFG_STATUS_SHOW                                                   0


/*
** -- Maximum targets --
**
** This defines the maximum number of targets that can be managed
** at once on a *PER USER* basis
*/
#define CMGRCFG_TARGETS_MAX                                                   2


/*
** -- Multiline Support  --
**
** This feature is used to control multiline support.
*/
#define CMGRCFG_MULTILINE                 0


/*
** -- DECT Support  --
**
** This feature is used to control DECT support.
*/
#define CMGRCFG_DECT                      0

/*
** -- Maximum cctk users (Multiline Support) --
**
** This defines the maximum number of cctk users or sip accounts that can be 
** managed.
*/
#define CMGRCFG_MAX_CCTK                  CMGRCFG_USER_MAX

/*
** -- Maximum line associations (Multiline Support)--
**
** This defines the maximum number of account to terminal associations that can
** be managed.
*/
#define CMGRCFG_MAX_ACCNT_ASSOCIATIONS    ( CMGRCFG_PHY_MAX * CMGRCFG_MAX_CCTK )

/*
** -- Maximum forced lines (Multiline Support)--
**
** This defines the maximum number of forced lines that will be allowed.
*/
#define CMGRCFG_MAX_ACCNT_FORCED           CMGRCFG_USER_MAX

/*
** -- Multiple users per active account (Multiline support) --
**
** This configures the ability to have multiple users using the same active account.  
** An active account is one where a call is engaged.  For outgoing calls, when this 
** flag is set, it allows another user to select the same (active account) for calls.
** For incoming calls, when this flag is set, it allow all users associated with the 
** account to be alerted.
*/
#define CMGRCFG_MULTILINE_MODE_ACTIVE_ACCNT_MULTIPLE_USR          0

/* SRTP support */
#define CMGRCFG_SRTP                                              0

/* ---- Variable Externs -------------------------------------------------------- */
/* ---- Function Prototypes ----------------------------------------------------- */

/* ---- Include Files ----------------------------------------------------------- */
/*
** CUSTOM CONFIGURATION - This must be included after all default build
** configuration options. Custom include files should undef default config options
** and then re-define them.
*/
#include <cmgrCfgCustom.h>

/* Configuration Verification */
#include <cmgrCfgChk.h>

#endif /* __CMGR_CFG__H__INCLUDED__ */
