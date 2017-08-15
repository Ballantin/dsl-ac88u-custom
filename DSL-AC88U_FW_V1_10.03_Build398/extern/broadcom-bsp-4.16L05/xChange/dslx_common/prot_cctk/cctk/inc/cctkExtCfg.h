/**********************************************************************************
** Copyright (c) 2007-2013 Broadcom
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
**      This file contains the CCTK extended configuration.
**
***********************************************************************************/

#ifndef __CCTK_EXT_CFG_API__H__INCLUDED__
#define __CCTK_EXT_CFG_API__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <cctkCfg.h>
#include <cctkMainCfg.h>
#include <bosTypes.h>
#include <bosIpAddr.h>

#if CCTKCFG_ICE_LITE_SUPPORT
#include <stun.h>
#endif /* CCTKCFG_ICE_LITE_SUPPORT */

/* ---- Constants and Types ----------------------------------------------------- */
/* Signaling tranport - fallback to TCP MTU size. */
#define CCTK_TRANSPORT_TCP_FALLBACK_MTU   1300

/* Transport protocol used for SIP (bitmap) */
#define CCTK_TRANSPORT_UDP          ( 0x00000001 )
#define CCTK_TRANSPORT_TCP          ( 0x00000002 )
#define CCTK_TRANSPORT_TLS          ( 0x00000004 ) /* Not used if CCTKCFG_TLS
                                                   ** disabled.
                                                   */
#define CCTK_TRANSPORT_ALL          ( CCTK_TRANSPORT_UDP          |                \
                                      CCTK_TRANSPORT_TCP          |                \
                                      CCTK_TRANSPORT_TLS )

/* Trace level control (bitmap) */
#define CCTK_TRACELVL_NONE          ( 0x0000 )
#define CCTK_TRACELVL_INFO          ( 0x0001 )
#define CCTK_TRACELVL_WARNING       ( 0x0002 )
#define CCTK_TRACELVL_DEBUG         ( 0x0004 )     /* Not used if CCTKCFG_DEBUG
                                                   ** disabled.
                                                   */
#define CCTK_TRACELVL_DEFAULT       ( CCTK_TRACELVL_INFO          |                \
                                      CCTK_TRACELVL_WARNING )
#define CCTK_TRACELVL_ALL           ( CCTK_TRACELVL_INFO          |                \
                                      CCTK_TRACELVL_WARNING       |                \
                                      CCTK_TRACELVL_DEBUG )

/* Tracing group control (bitmap)
** NOTE: If CCTKCFG_TRACE_CCTK is enabled, any trace groups other than
**       CCTK_TRACEGRP_CCTK will not be used.
*/
#define CCTK_TRACEGRP_NONE          ( 0x0000 )
#define CCTK_TRACEGRP_CCTK          ( 0x0001 )
#define CCTK_TRACEGRP_SCE           ( 0x0002 )
#define CCTK_TRACEGRP_TRANSPORT     ( 0x0004 )
#define CCTK_TRACEGRP_SDP           ( 0x0008 )
#define CCTK_TRACEGRP_SIP           ( 0x0010 )
#define CCTK_TRACEGRP_MISC          ( 0x0020 )
#define CCTK_TRACEGRP_STUN          ( 0x0040 )
#define CCTK_TRACEGRP_DEFAULT       ( CCTK_TRACEGRP_CCTK )
#define CCTK_TRACEGRP_ALL           ( CCTK_TRACEGRP_CCTK          |                \
                                      CCTK_TRACEGRP_SCE           |                \
                                      CCTK_TRACEGRP_TRANSPORT     |                \
                                      CCTK_TRACEGRP_SDP           |                \
                                      CCTK_TRACEGRP_SIP           |                \
                                      CCTK_TRACEGRP_MISC )

/* Event subscription control (bitmap) */
#define CCTK_SUBEVT_NONE            ( 0x00000000 )
#define CCTK_SUBEVT_DIALOG          ( 0x00000001 ) /* "dialog" - RFC4235 */
#define CCTK_SUBEVT_PRESENCE        ( 0x00000002 ) /* "presence" - RFC3856 */
#define CCTK_SUBEVT_REG             ( 0x00000004 ) /* "reg" - RFC3680 */
#define CCTK_SUBEVT_WINFO           ( 0x00000008 ) /* "winfo" - RFC3857 */
#define CCTK_SUBEVT_ALL             ( CCTK_SUBEVT_DIALOG          |                \
                                      CCTK_SUBEVT_PRESENCE        |                \
                                      CCTK_SUBEVT_REG             |                \
                                      CCTK_SUBEVT_WINFO )

/* SIP method control (bitmap) */
#define CCTK_METHOD_NONE            ( 0x00000000 )
#define CCTK_METHOD_ACK             ( 0x00000001 )
#define CCTK_METHOD_BYE             ( 0x00000002 )
#define CCTK_METHOD_INFO            ( 0x00000004 )
#define CCTK_METHOD_INVITE          ( 0x00000008 )
#define CCTK_METHOD_MESSAGE         ( 0x00000010 )
#define CCTK_METHOD_NOTIFY          ( 0x00000020 )
#define CCTK_METHOD_OPTIONS         ( 0x00000040 )
#define CCTK_METHOD_PRACK           ( 0x00000080 )
#define CCTK_METHOD_REFER           ( 0x00000100 )
#define CCTK_METHOD_REGISTER        ( 0x00000200 )
#define CCTK_METHOD_SUBSCRIBE       ( 0x00000400 )
#define CCTK_METHOD_UPDATE          ( 0x00000800 )
#define CCTK_METHOD_GRP_CALL        ( CCTK_METHOD_ACK             |                \
                                      CCTK_METHOD_BYE             |                \
                                      CCTK_METHOD_INVITE          |                \
                                      CCTK_METHOD_PRACK           |                \
                                      CCTK_METHOD_REFER           |                \
                                      CCTK_METHOD_UPDATE )
#define CCTK_METHOD_GRP_EVT         ( CCTK_METHOD_NOTIFY          |                \
                                      CCTK_METHOD_SUBSCRIBE )
#define CCTK_METHOD_GRP_REG         ( CCTK_METHOD_REGISTER )
#define CCTK_METHOD_ALL             ( CCTK_METHOD_ACK             |                \
                                      CCTK_METHOD_BYE             |                \
                                      CCTK_METHOD_INFO            |                \
                                      CCTK_METHOD_INVITE          |                \
                                      CCTK_METHOD_MESSAGE         |                \
                                      CCTK_METHOD_NOTIFY          |                \
                                      CCTK_METHOD_OPTIONS         |                \
                                      CCTK_METHOD_PRACK           |                \
                                      CCTK_METHOD_REFER           |                \
                                      CCTK_METHOD_REGISTER        |                \
                                      CCTK_METHOD_SUBSCRIBE       |                \
                                      CCTK_METHOD_UPDATE )

/* SIP outgoing signaling behavioral control (bitmap) */
#define CCTK_SIGBEHVTX_NONE         ( 0x00000000 )
#define CCTK_SIGBEHVTX_FORCEQUOTES  ( 0x00000001 ) /* Enabled to embrace all display
                                                   ** names by double quotes.
                                                   */
#define CCTK_SIGBEHVTX_CALLIDNOHOST ( 0x00000002 ) /* Enabled to exclude @host in
                                                   ** the Call-Id header when
                                                   ** sending new SIP request.
                                                   */
#define CCTK_SIGBEHVTX_ESCAPESTAR   ( 0x00000004 ) /* Enabled to escape any star
                                                   ** character '*' in "userinfo"
                                                   ** of SIP-URI with the %2A
                                                   ** sequence.
                                                   */
#define CCTK_SIGBEHVTX_NOSVCRTEREG  ( 0x00000008 ) /* Enabled to prevent the use of
                                                   ** a discovered Service Route
                                                   ** information within REGISTER
                                                   ** requests.
                                                   */
#define CCTK_SIGBEHVTX_NOFQDNINRTE  ( 0x00000010 ) /* Enabled to prevent the use of
                                                   ** a FQDN within the top most
                                                   ** Route header.
                                                   */
#define CCTK_SIGBEHVTX_INSCACHEAUTH ( 0x00000020 ) /* Enabled to insert cached
                                                   ** Authorization header if the
                                                   ** request would otherwise not
                                                   ** contain any.  The cached value
                                                   ** is the last one generated.
                                                   */
#define CCTK_SIGBEHVTX_NOFQDNINVIA  ( 0x00000040 ) /* Enabled to prevent the use of
                                                   * a FQDN within the Via header.
                                                   */
#define CCTK_SIGBEHVTX_RESETCSEQ    ( 0x00000080 ) /* Enabled to reset the CSeq data
                                                   * to 1 on each new transaction or
                                                   * dialog.
                                                   */
#define CCTK_SIGBEHVTX_REDUCESIZE   ( 0x00000100 ) /* Enabled to enable some packet
                                                   ** size reduction features such
                                                   ** as header short format, group
                                                   ** headers, etc...
                                                   */
#define CCTK_SIGBEHVTX_FAILONREREG  ( 0x00000200 ) /* Enabled to apply a failover
                                                   ** action processing before trying to
                                                   ** re-register in cases that would
                                                   ** normally not require such transition.
                                                   */
#define CCTK_SIGBEHVTX_ESCAPEPOUND  ( 0x00000400 ) /* Enabled to escape any pound
                                                   ** character '#' in "userinfo"
                                                   ** of SIP-URI with the %23
                                                   ** sequence.
                                                   */
#define CCTK_SIGBEHVTX_BKOFFSUCCESSREGCYLCE ( 0x00000800) /* Enable to apply backoff failover  
                                                   ** delay before starting a new reg cycle
                                                   ** even if the current reg cycle didn't fail.
                                                   */

/* SIP incoming signaling behavioral control (bitmap) */
#define CCTK_SIGBEHVRX_NONE         ( 0x00000000 )
#define CCTK_SIGBEHVRX_TWOSTEPBYE   ( 0x00000001 ) /* Enabled to send a second BYE
                                                   ** without any optional SIP
                                                   ** headers.
                                                   */
#define CCTK_SIGBEHVRX_REGONINV4XX  ( 0x00000002 ) /* Enabled to initiate a new
                                                   ** registration when receiving
                                                   ** certain 4xx response to an
                                                   ** INVITE. Currently 403, 408 480
                                                   ** responses fall under this.
                                                   */
#define CCTK_SIGBEHVRX_REGONINVTIMEOUT  ( 0x00000004 ) /* Enabled to initiate a new
                                                   ** registration when an INVITE times
                                                   ** out (i.e. a locally generated 408 
                                                   ** response seen).
                                                   */
#define CCTK_SIGBEHVRX_REGONSUBTIMEOUT  ( 0x00000008 ) /* Enabled to initiate a new
                                                   ** registration when a SUBSCRIBE times
                                                   ** out (i.e.  a locally generated 408 
                                                   ** response seen).
                                                   */
#define CCTK_SIGBEHVRX_REREGONBYE4XX  ( 0x00000010 ) /* Enabled to initiate a new
                                                   ** registration when receiving 4xx
                                                   ** responses to a BYE.
                                                   */
#define CCTK_SIGBEHVRX_REGONHLDREINVTIMEOUT ( 0x00000020 ) /* Enabled to initiate a new
                                                   ** registration when a REINVITE to
                                                   ** place a call on hold or unhold times
                                                   ** out (i.e. a locally generated 408
                                                   ** response seen ).
                                                   */
#define CCTK_SIGBEHVRX_REGONINV5XX  ( 0x00000040 ) /* Enabled to initiate a new
                                                   ** registration for certain 5xx
                                                   ** response to an INVITE. Currently
                                                   ** 500, 503 & 504 responses fall
                                                   ** under this.
                                                   */
#define CCTK_SIGBEHVRX_REGFVRONSTDERR ( 0x00000080 )    /* Enabled to apply registration
                                                   ** failover on the standard set of
                                                   ** failover responses:
                                                   ** ( 408, 500, 504, 600 )
                                                   */
#define CCTK_SIGBEHVRX_REGFVRRESTARTON305 ( 0x00000100 )
                                                   /* Enabled to restart registration
                                                   ** procedures from the top of the
                                                   ** target list on a 305 response.
                                                   */

/* SIP inter-operability control (bitmap) */
#define CCTK_INTEROP_NONE           ( 0x00000000 )
#define CCTK_INTEROP_ACKBRANCH      ( 0x00000001 ) /* Enabled to bypass branch-id
                                                   ** check in ACK. ACK with the
                                                   ** same branch-id as its
                                                   ** corresponding INVITE is
                                                   ** allowed. Enabling this breaks
                                                   ** section 8.1.1.7 of RFC3261.
                                                   */
#define CCTK_INTEROP_PRECONDNOADVT  ( 0x00000002 ) /* Enabled to not advertize
                                                   ** support for preconditions in
                                                   ** Supported header even if we
                                                   ** do support the feature.
                                                   ** Enabling this breaks section
                                                   ** 11 of RFC3312.
                                                   */
#define CCTK_INTEROP_NO_TOTAGMATCH  ( 0x00000004 ) /* Enabled to disable to-tag
                                                   ** matching. Enabling this breaks
                                                   ** section 12 of RFC3261.
                                                   */
#define CCTK_INTEROP_REGCONTACTHDR  ( 0x00000008 ) /* Enabled to disable
                                                   ** registration contact header
                                                   ** matching. Enabling this breaks
                                                   ** section 10.2 of RFC3261.
                                                   */
#define CCTK_INTEROP_SUBJECTALTNAME ( 0x00000010 ) /* Enabled to bypass
                                                   ** SubjectAltName / Request-URI
                                                   ** host check (for TLS). Enabling
                                                   ** this breaks section 4.2.1.7 of
                                                   ** RFC3280.
                                                   */
#define CCTK_INTEROP_DATEGMTFORMAT  ( 0x00000020 ) /* Enabled to disable Date GMT
                                                   ** format check to allow single
                                                   ** digit date and time. Enabling
                                                   ** this breaks the ABNF specified
                                                   ** in RFC3261.
                                                   */
#define CCTK_INTEROP_NOTIFYSUBSTATE ( 0x00000040 ) /* Enabled to disable
                                                   ** Subscription-State header
                                                   ** check in received NOTIFY.
                                                   ** Enabling this breaks section
                                                   ** 3.2.4 of RFC3265.
                                                   */
#define CCTK_INTEROP_STATUSLINECRLF ( 0x00000080 ) /* Enabled to disable checking on
                                                   ** whether Status-Line in
                                                   ** received NOTIFY payload is
                                                   ** ended with a CRLF. Enabling
                                                   ** this breaks section 2.4.5 of
                                                   ** RFC3515.
                                                   */
#define CCTK_INTEROP_MSGSUMCRLF     ( 0x00000100 ) /* Enabled to disable checking on
                                                   ** whether Message-Waiting-Summary
                                                   ** in received NOTIFY payload is
                                                   ** ended with a CRLF. Enabling
                                                   ** this breaks section 5.2 of
                                                   ** RFC3842.
                                                   */
#define CCTK_INTEROP_REGFVRALLRSP   ( 0x00000200 ) /* Enabled to apply registration
                                                   ** failover on all responses.
                                                   ** (except for 401, 407, and 423)
                                                   ** Enabling this breaks the
                                                   ** registration behaviours
                                                   ** specified in RFC3261.
                                                   */
#define CCTK_INTEROP_REVALRTNFO     ( 0x00000400 ) /* Enabled to reverse the order
                                                   ** of the Alert-Info received
                                                   ** data such that the last value
                                                   ** in the received header is used
                                                   ** first.
                                                   */
#define CCTK_INTEROP_REGENDON403    ( 0x00000800 ) /* Enabled to stop registration
                                                   ** attempt following a 403.  This
                                                   ** is applicable only when
                                                   ** CCTK_INTEROP_REGFVRALLRSP is
                                                   ** also used.
                                                   ** Note that if the Retry-After 
                                                   ** header is included in the 403
                                                   ** response or configured, then
                                                   ** the Retry-After value will 
                                                   ** take precedence and registration
                                                   ** will be re-attempted.
                                                   */
#define CCTK_INTEROP_REREGNOSUB     ( 0x00001000 ) /* Enabled to prevent subscription
                                                   ** refresh from happening following
                                                   ** a user initiated re-registration.
                                                   */
#define CCTK_INTEROP_EMGNOACCEPT    ( 0x00002000 ) /* Enabled to omit the Accept header
                                                   ** from the INVITE for emergency
                                                   ** calls only
                                                   */
#define CCTK_INTEROP_KASKIPOUTBOUND ( 0x00004000 ) /* Enable to skip the check for
                                                   ** Outbound option-tag in the Required/
                                                   ** Supported header of 200-REG 
                                                   ** responses to establish keep
                                                   ** alive sessions */
#define CCTK_INTEROP_MAINTCALLREGFAIL ( 0x00008000 ) /* Enable to maintain active calls
                                                   ** even when a registration failure/
                                                   ** failover occurs */

#define CCTK_INTEROP_KASKIPOOSREPORT  ( 0x00010000 ) /* Enable to not use the keep-alive
                                                   ** status to determine the in-service/
                                                   ** out-of-service status */

#define CCTK_INTEROP_RELAX_ALRTNFO  ( 0x00020000 ) /* Enable to relax the Alert-Info
                                                   ** syntax check */

#define CCTK_INTEROP_INVNOACCEPT    ( 0x00040000 ) /* Enabled to omit the Accept header
                                                   ** from the INVITE for all calls
                                                   */
                                                   
#define CCTK_INTEROP_KEEPSUBREFRESHFAIL ( 0x00080000 ) /* Enable to keep the subscription
                                                   ** on refresh failures instead of treating
                                                   ** it the same as an initial request
                                                   ** failure. Subscription expiry without
                                                   ** succesful refresh leads to a new
                                                   ** request without a delay.
                                                   */

#define CCTK_INTEROP_DEFAULT_PORT_IN_CONTACT_HDR ( 0x00100000 ) /* include port number in contact header
                                                                   ** if it is default values 5060 and 5061.
                                                                */
#define CCTK_INTEROP_ADVSUPEXT      ( 0x00200000 ) /* Enabled to advertize advanced
                                                   ** supported extensions in the
                                                   ** Supported header.
                                                   */

#define CCTK_INTEROP_ACCESS_NETWORK_HDR ( 0x00400000 ) /* Enable to include 
                                                       ** P-Access-Network-Info header to 
                                                       ** all SIP messages.
                                                       */

#define CCTK_INTEROP_EARLY_MEDIA_HDR ( 0x00800000 ) /* Enable to include 
                                                    ** P-Early-Media header to
                                                    ** INVITE, PRACK, and UPDATE.
                                                    */

#define CCTK_INTEROP_RANDOM_REREG ( 0x01000000 ) /* Enable to Randomize 
                                                 ** Re-Registration Timer
                                                 */
#define CCTK_INTEROP_USER_AGENT_IN_ALL ( 0x02000000 ) /* Enable to include User-Agent
                                                      ** header in all requests and
                                                      ** responses.
                                                      */
#define CCTK_INTEROP_XRTPSTAT_IN_CALLEND ( 0x04000000 ) /* Enable to include X-RTP-Stat 
                                                           ** header in call termination (in BYE
                                                           ** or 200 OK to BYE messages)
                                                           */

#define CCTK_INTEROP_NO_ROUTE_HEADER_IN_REQUESTS ( 0x08000000 ) /* Remove route header from
                                                    ** all requests.
                                                    */

/* CCTK parameter macros */
#define CCTK_IS_MC( eParm )         ( eParm >= eCCTKPARM_MC_START &&               \
                                      eParm <= eCCTKPARM_MC_END )
#define CCTK_IS_MC_SYS( eParm )     ( eParm >= eCCTKPARM_MC_SYS_START &&           \
                                      eParm <= eCCTKPARM_MC_SYS_END )
#define CCTK_IS_MC_USR( eParm )     ( eParm >= eCCTKPARM_MC_USR_START &&           \
                                      eParm <= eCCTKPARM_MC_USR_END )
#define CCTK_IS_MC_USR_OVR( eParm ) ( eParm == eCCTKPARM_MC_OBPROXYLST )
#define CCTK_IS_MC_CALL( eParm )    (( eParm >= eCCTKPARM_MC_CALL_START &&         \
                                       eParm <= eCCTKPARM_MC_CALL_END ) ||         \
                                     ( CCTK_IS_MC_USR_OVR( eParm ) ))
#define CCTK_IS_MC_EVT( eParm )     (( eParm >= eCCTKPARM_MC_EVT_START &&          \
                                       eParm <= eCCTKPARM_MC_EVT_END ) ||          \
                                     ( CCTK_IS_MC_USR_OVR( eParm ) ))
#define CCTK_IS_EC( eParm )         ( eParm >= eCCTKPARM_EC_START &&               \
                                      eParm <= eCCTKPARM_EC_END )
#define CCTK_IS_EC_SYS( eParm )     ( eParm >= eCCTKPARM_EC_SYS_START &&           \
                                      eParm <= eCCTKPARM_EC_SYS_END )
#define CCTK_IS_EC_USR( eParm )     ( eParm >= eCCTKPARM_EC_USR_START &&           \
                                      eParm <= eCCTKPARM_EC_USR_END )
#define CCTK_IS_EC_USR_OVR( eParm ) ( eParm == eCCTKPARM_EC_GENHDRCFG )
#define CCTK_IS_EC_CALL( eParm )    (( eParm >= eCCTKPARM_EC_CALL_START &&         \
                                       eParm <= eCCTKPARM_EC_CALL_END ) ||         \
                                     ( CCTK_IS_EC_USR_OVR( eParm ) ))
#define CCTK_IS_EC_EVT( eParm )     (( eParm >= eCCTKPARM_EC_EVT_START &&          \
                                       eParm <= eCCTKPARM_EC_EVT_END ) ||          \
                                     ( CCTK_IS_EC_USR_OVR( eParm ) ))
#define CCTK_IS_CFGPARM( eParm )    ( CCTK_IS_MC( eParm ) || CCTK_IS_EC( eParm ) )
#define CCTK_IS_DRPARM( eParm )     ( eParm >= eCCTKPARM_DR_START &&               \
                                      eParm <= eCCTKPARM_DR_END )
#define CCTK_IS_SYSPARM( eParm )    ( CCTK_IS_EC_SYS( eParm ) ||                   \
                                      CCTK_IS_MC_SYS( eParm ) )
#define CCTK_IS_USRPARM( eParm )    ( CCTK_IS_MC_USR( eParm ) ||                   \
                                      CCTK_IS_EC_USR( eParm ) )
#define CCTK_IS_CALLPARM( eParm )   ( CCTK_IS_MC_CALL( eParm ) ||                  \
                                      CCTK_IS_EC_CALL( eParm ) )
#define CCTK_IS_EVTPARM( eParm )    ( CCTK_IS_MC_EVT( eParm ) ||                   \
                                      CCTK_IS_EC_EVT( eParm ) )

/* CCTK parameter types */
typedef enum CCTKPARM
{
   /* --------------------- MAIN CONFIG ------------------------- */
   eCCTKPARM_MC_START,
   /* -------------- SYS CONFIG -------------- */

   eCCTKPARM_MC_SYS_START = eCCTKPARM_MC_START,
   /* -- SystemXtl.Network.General -- */
   eCCTKPARM_MC_HOSTINFO = eCCTKPARM_MC_SYS_START,
   
   eCCTKPARM_MC_SYS_END = eCCTKPARM_MC_HOSTINFO,
   
   /* -------------- USER CONFIG -------------- */
   eCCTKPARM_MC_USR_START,
   /* -- User.Network -- */
   eCCTKPARM_MC_SIPNAME = eCCTKPARM_MC_USR_START,
                                       /* uid: SIP name info (CCTKSIPNAME) */
   eCCTKPARM_MC_OBPROXYLST,            /* eid/cid/uid: Outbound proxy list update
                                       ** (CCTKROUTELST)
                                       */

   /* -- MWI.General -- */
   eCCTKPARM_MC_MWISVRADDR,            /* uid: MWI server address
                                       ** (CCTKSIPUSRHOST)
                                       */

   /* -------------- CALL CONFIG -------------- */
   eCCTKPARM_MC_CALL_START,

   /* -- User.Default.Call.Media -- */
   eCCTKPARM_MC_MEDIACFG = eCCTKPARM_MC_CALL_START,
                                       /* uid/cid: Media config (CCTKMEDCFG) */
   eCCTKPARM_MC_MEDIALIST,
                                       /* uid/cid: Offer media list (CCTKMEDLST) */

   eCCTKPARM_MC_CALL_END = eCCTKPARM_MC_MEDIALIST,
   /* -------------- CALL CONFIG (End) -------------- */

   /* -------------- EVENT CONFIG -------------- */
   eCCTKPARM_MC_EVT_START = eCCTKPARM_MC_CALL_END,

   eCCTKPARM_MC_EVT_END = eCCTKPARM_MC_EVT_START,
   /* -------------- EVENT CONFIG (End) -------------- */

   eCCTKPARM_MC_USR_END = eCCTKPARM_MC_EVT_END,
   /* -------------- USER CONFIG (End) -------- */

   eCCTKPARM_MC_END = eCCTKPARM_MC_USR_END,
   /* --------------------- MAIN CONFIG (End) ------------------- */

   /* --------------------- EXTENDED CONFIG --------------------- */
   eCCTKPARM_EC_START,

   /* -------------- USER CONFIG -------------- */
   eCCTKPARM_EC_USR_START = eCCTKPARM_EC_START,

   /* -- User.Local -- */
   eCCTKPARM_EC_SUBEVT = eCCTKPARM_EC_USR_START,
                                       /* uid: Subscription events supported by CCTK
                                       ** (BOS_UINT32 - CCTK_SUBEVT_XXX)
                                       */
   eCCTKPARM_EC_GENHDRCFG,             /* eid/cid/uid: Generic header config
                                       ** (CCTKGENHDRCFG)
                                       */

   /* -- User.Network -- */
   eCCTKPARM_EC_USRPORTCFG,            /* uid: User local port configuration
                                       ** (CCTKUSRPORTCFG)
                                       */
   eCCTKPARM_EC_DIAGEVTCFG,            /* uid: Dialog event configuration
                                       ** (CCTKDIAGEVTCFG)
                                       */
   eCCTKPARM_EC_UAPEVTCFG,             /* uid: User-Agent Profile event
                                       **      configuration
                                       ** (CCTKUAPEVTCFG)
                                       */
   eCCTKPARM_EC_USREMGCFG,             /* uid: User emergency call configuration
                                       ** (CCTKUSREMGCFG)
                                       */
   eCCTKPARM_EC_USRBRCFG,              /* uid: User backup and recovery
                                       **      configuration
                                       ** (CCTKUSRBRCFG)
                                       */
   eCCTKPARM_EC_SVCROUTE,              /* uid: Service-Route header config override
                                       ** (CCTKSVCROUTE)
                                       */
   eCCTKPARM_EC_USRCONTACT,            /* uid: User contact config (CCTKCONTACT) */

   /* -- User.Security.Authentication -- */
   eCCTKPARM_EC_AUTHCFG,               /* uid: User authentication config 
                                       ** (CCTKAUTHCFG) 
                                       */

   /* -- User.Security.DoS -- */
   eCCTKPARM_EC_WHTLST,                /* uid: White list (CCTKWHTLSTCFG) */
   eCCTKPARM_EC_BLKLST,                /* uid: Black list (CCTKIPADDRLST) */

   /* -- User.Security.NAT -- */

   /* -- Registration.General -- */
   eCCTKPARM_EC_REGTMR,                /* uid: Registration timer configuration
                                       ** (CCTKREGTMR)
                                       */
   eCCTKPARM_EC_REGEVTCFG,             /* uid: Registration event configuration
                                       ** (CCTKREGEVTCFG)
                                       */

   /* -- Registration.Advanced -- */

   /* -- MWI.General -- */
   eCCTKPARM_EC_MWICFG,                /* uid: MWI configuration
                                       ** (CCTKMWICFG)
                                       */
   eCCTKPARM_EC_MWITMR,                /* uid: MWI timer configuration
                                       ** (CCTKMWITMR)
                                       */
#if CCTKCFG_REMOTE_USER_CONTROL      
   /* -- remUsr.cfg -- */
   eCCTKPARM_EC_REMUSRCFG,             /* Remote user configuration 
                                       ** (CCTKREMUSRCFG)
                                       */
#endif
   /* -------------- CALL CONFIG -------------- */
   eCCTKPARM_EC_CALL_START,

   /* -- Basic.General -- */
   eCCTKPARM_EC_EMGCALLCFG = eCCTKPARM_EC_CALL_START,
                                       /* cid/uid: Emergency call configuration
                                       ** (CCTKEMGCALLCFG)
                                       */
#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
   eCCTKPARM_EC_TELCALL,               /* cid/uid: Telephone call configuration
                                       ** (CCTKTELCALL)
                                       */
#endif
   eCCTKPARM_EC_CALLSTATS,             /* cid: Call statistics for a given call
                                       ** (CCTKCALLMETRICS)
                                       */
   /* -- Basic.Advanced -- */
   eCCTKPARM_EC_PRACK,                 /* cid/uid: PRACK support configuration
                                       ** (CCTKPRACK)
                                       */
   eCCTKPARM_EC_SESTMR,                /* cid/uid: Session timer configuration
                                       ** (CCTKSESTMR)
                                       */
   eCCTKPARM_EC_CALLPRIV,              /* cid/uid: Privacy config (CCTKPRIVCFG) */
   eCCTKPARM_EC_CALLNAI,               /* cid/uid: Network asserted identity config
                                       ** (CCTKNAICFG)
                                       */
   eCCTKPARM_EC_CONNECTEDID,           /* cid/uid: Connected ID config RFC 4916
                                       ** (CCTKCONNECTEDID)
                                       */

   /* -- Supplementary -- */
   eCCTKPARM_EC_CALLHOLD,              /* cid/uid: Call hold config (CCTKHOLDCFG) */
   eCCTKPARM_EC_CALLXFER,              /* cid/uid: Call transfer configuration
                                       ** (CCTKXFERCFG)
                                       */

   /* -- Media -- */
   eCCTKPARM_EC_MEDNEGCFG,             /* cid/uid: Media negotiation configuration
                                       ** (CCTKMEDNEGCFG)
                                       */

   eCCTKPARM_EC_CALL_END = eCCTKPARM_EC_MEDNEGCFG,
   /* -------------- CALL CONFIG (End) -------------- */

   /* -------------- EVENT CONFIG -------------- */
   eCCTKPARM_EC_EVT_START,

   /* -- General -- */
   eCCTKPARM_EC_EVTTMR = eCCTKPARM_EC_EVT_START,
                                       /* eid/uid: Timer config (CCTKEVTTMR) */

   /* -- Advanced -- */
   eCCTKPARM_EC_EVTPRIV,               /* eid/uid: Privacy config (CCTKPRIVCFG) */
   eCCTKPARM_EC_EVTNAI,                /* eid/uid: Network asserted identity config
                                       ** (CCTKNAICFG)
                                       */

   eCCTKPARM_EC_EVT_END = eCCTKPARM_EC_EVTNAI,
   /* -------------- EVENT CONFIG (End) -------------- */

   eCCTKPARM_EC_USR_END = eCCTKPARM_EC_EVT_END,
   /* -------------- USER CONFIG (End) -------------- */

   /* -------------- SYSTEM CONFIG -------------- */
   /* NOTE: All system extended config can be changed only when CCTK has never been
   ** configured yet (i.e. after cctkStartup() but before cctkConfig()) or when CCTK
   ** is in the "Reset" state (i.e. no ongoing calls and no CCTK resources are
   ** allocated).
   */
   eCCTKPARM_EC_SYS_START,

   /* -- Network.General -- */
   eCCTKPARM_EC_LOCPORT = eCCTKPARM_EC_SYS_START,
                                       /* Local port configuration (CCTKLOCPORT) */
   eCCTKPARM_EC_TRANSPORT,             /* Transport configuration
                                       ** (BOS_UINT32 - CCTK_TRANSPORT_XXX)
                                       */
#if CCTKCFG_TLS
   eCCTKPARM_EC_TLSCFG,                /* TLS configuration (CCTKTLSCFG)
                                       ** (not used if CCTKCFG_TLS disabled)
                                       ** (can be modified after CCTK start-up)
                                       */
#endif
   eCCTKPARM_EC_CNXACTIVITY,           /* Connection activity config (CCTKCNXCFG)
                                       ** (can be modified after CCTK start-up)
                                       */

   /* -- Network.Protocol.General -- */
   eCCTKPARM_EC_UAIDHDR,               /* UA identity header config (CCTKUAIDHDR)
                                       ** (can be modified after CCTK start-up)
                                       */
   eCCTKPARM_EC_ROUTINGHDR,            /* Routing-related header configuration
                                       ** (CCTKROUTINGHDR)
                                       ** (can be modified after CCTK start-up)
                                       */
   eCCTKPARM_EC_DNSMODE,               /* DNS query policy (CCTKDNSMODE)
                                       ** (can be modified after CCTK start-up)
                                       */
   eCCTKPARM_EC_SIGBEHAVE,             /* SIP signaling behavioral config.
                                       ** Enabled to alter SIP signaling behaviors.
                                       ** (CCTKSIGBEHAVE)
                                       */
   eCCTKPARM_EC_INTEROP,               /* SIP inter-operability config. Enabled to
                                       ** resolve inter-op issues. Enabling this
                                       ** config can break SIP compliances (see
                                       ** CCTK_INTEROP_XXX for detail).
                                       ** (BOS_UINT32 - CCTK_INTEROP_XXX)
                                       */
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
   eCCTKPARM_EC_IMSCFG,                /* IMS-specific config (CCTKIMSCFG) */
#endif
   eCCTKPARM_EC_GRUUCFG,               /* GRUU config (CCTKGRUUCFG) */
   eCCTKPARM_EC_PAUCFG,                /* P-Associated-URI config (CCTKPAUCFG) */
   eCCTKPARM_EC_PANICFG,               /* P-Access-Network_Info config (CCTKPANICFG) */
#if CCTKCFG_ICE_LITE_SUPPORT
   eCCTKPARM_EC_ICECTLCFG,             /* ICE control config (CCTKICECTLCFG) */
   eCCTKPARM_EC_ICEIPLSTCFG,           /* ICE IP Address List (CCTKIPADDRLST) */
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
   /* -- Network.Protocol.Timer -- */
   eCCTKPARM_EC_TRANSTMR,              /* Transaction timer configuration
                                       ** (CCTKTRANSTMRCFG)
                                       ** (can be modified after CCTK start-up)
                                       */

   /* -- Local.General -- */

   /* -- Local.Debug -- */
   eCCTKPARM_EC_PKTLOG,                /* Packet logger via UDP messages
                                       ** (CCTKPKTLOG)
                                       */
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   eCCTKPARM_EC_TRACE,                 /* Tracing configuration (CCTKTRACE)
                                       ** (not used if CCTKCFG_TRACE_DISABLED set)
                                       ** (can be modified after CCTK start-up)
                                       */
#endif

   /* -- Security.General -- */

   /* -- Security.NAT.General -- */
   eCCTKPARM_EC_NATGENCFG,             /* General NAT configuration
                                       ** (CCTKNATGENCFG)
                                       */
   /* -- Security.NAT.RG -- */
   eCCTKPARM_EC_NATRGCTL,              /* RG NAT control configuration
                                       ** (CCTKRGNATCTL) 
                                       */
   /* -- Security.NAT.STUN -- */

   /* -- Security.DoS -- */
#if CCTKCFG_MEMORY_MONITORING
   eCCTKPARM_EC_MEMPROTECT,            /* Memory allocation protection
                                       ** (CCTKMEMPROTCFG)
                                       */
#endif

   /* -- RemoteUsr.General -- */
#if CCTKCFG_REMOTE_USER_CONTROL
   eCCTKPARM_EC_REMUSRSYS,             /* SIP remote user System configuration 
                                       ** (CCTKREM
                                       */
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

#if CCTKCFG_REMOTE_USER_CONTROL   
   eCCTKPARM_EC_SYS_END = eCCTKPARM_EC_REMUSRSYS,
#elif CCTKCFG_MEMORY_MONITORING
   eCCTKPARM_EC_SYS_END = eCCTKPARM_EC_MEMPROTECT,
#else
   eCCTKPARM_EC_SYS_END = eCCTKPARM_EC_NATRGCTL,
#endif
   /* -------------- SYSTEM CONFIG (End) -------------- */

   eCCTKPARM_EC_END = eCCTKPARM_EC_SYS_END,
   /* --------------------- EXTENDED CONFIG (End) --------------------- */

   /* --------------------- DATA RETRIEVAL ---------------------------- */

   eCCTKPARM_DR_START,

   eCCTKPARM_MEDINFO = eCCTKPARM_DR_START,
                                       /* Media info (CCTKMEDINFO) */
   eCCTKPARM_LASTCALLIN,               /* Last incoming call for a user
                                       ** (CCTKCALLINFO)
                                       */
   eCCTKPARM_LASTCALLOUT,              /* Last outgoing call for a user
                                       ** (CCTKCALLINFO)
                                       */
   eCCTKPARM_FEATINFO,                 /* Dynamic feature information for
                                       ** a user
                                       ** (CCTKFEATINFO)
                                       */
   eCCTKPARM_EXPINFO,                  /* Register/Expiration info (CCTKEXPIREINFO) */

   eCCTKPARM_DR_END = eCCTKPARM_EXPINFO,

   /* --------------------- DATA RETRIEVAL (End) ---------------------- */

   eCCTKPARM_MAX                       /* Parameter count (not used) */
} CCTKPARM;

/* Map for default extended configuration (used by cctkExtCfg.cpp) */
typedef struct CCTKEXTCFGMAP
{
   CCTKPARM       eParm;               /* Parameter type */
   const void     *pCfg;               /* Configuration block */
} CCTKEXTCFGMAP;

/* IP address with port */
typedef struct CCTKIPPORT
{
   BOS_IP_ADDRESS addr;                /* IP address */
   BOS_UINT16     uPort;               /* Port number (host byte ordering) */
} CCTKIPPORT;

/* IP address/port list */
typedef struct CCTKIPPORTLST
{
   BOS_UINT32 uNum;                    /* Number of ipport items in list */
   CCTKIPPORT ipport[CCTKCFG_MAX_ADDRLST];   /* Ipport list */
} CCTKIPPORTLST;

/* Packet logging policy */
typedef enum CCTKPKTLOGMODE
{
   eCCTKPKTLOGMODE_SIP,                /* Log parsed incoming and outgoing SIP
                                       ** packets with additional address info
                                       ** pre-pended to the logged packet.
                                       */
   eCCTKPKTLOGMODE_SIPONLY,            /* Log parsed incoming and outgoing SIP
                                       ** packets without any additional address
                                       ** info pre-pended to the logged packet.
                                       */
   eCCTKPKTLOGMODE_RAW,                /* Log raw data coming from and going onto
                                       ** the network with additional address info
                                       ** pre-pended to the logged packet.
                                       */
   eCCTKPKTLOGMODE_RAWONLY             /* Log raw data coming from and going onto
                                       ** the network without any additional address
                                       ** info pre-pended to the logged packet.
                                       */
} CCTKPKTLOGMODE;

/* Packet logger configuration */
typedef struct CCTKPKTLOG
{
   CCTKIPPORT     ipPort;              /* Logger address.
                                       ** Set to 0 to disable logging.
                                       */
   CCTKPKTLOGMODE eMode;               /* Packet logging policy */
} CCTKPKTLOG;

#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
/***********************************************************************************
**  FUNCTION:   CCTKTRACEHDLR
**
**  PURPOSE:    Trace handler prototype.
**
**  PARAMETERS:
**              uLevel - Trace level.
**              uGroup - Trace group.
**              pcTraceStr - Pointer to a NULL terminated string of characters.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      If the application install Trace hander is blocking or
**              inefficient, it can affect the CCTK operation.
***********************************************************************************/
typedef void ( *CCTKTRACEHDLR )( BOS_UINT16 uLevel,
                                 BOS_UINT16 uGroup,
                                 const BOS_UINT8 *pcTraceStr );

/* Tracing configuration */
typedef struct CCTKTRACE
{
   CCTKTRACEHDLR  pHandler;            /* Trace output handler.
                                       ** NULL to disable tracing.
                                       */
   BOS_UINT16     uLevel;              /* Trace level (bitmap)
                                       ** - CCTK_TRACELVL_XXX
                                       */
   BOS_UINT16     uGroup;              /* Trace group (bitmap)
                                       ** - CCTK_TRACEGRP_XXX
                                       */
} CCTKTRACE;
#endif

/* Local port configuration */
typedef struct CCTKLOCPORT
{
   BOS_UINT16     uSipPort;            /* Port used for SIP communication
                                       ** (UDP and TCP).
                                       ** Set to 0 for default, i.e. 5060.
                                       */
   BOS_UINT16     uSipsPort;           /* Port used for SIP communication (TLS).
                                       ** Set to 0 for default, i.e. 5061.
                                       */
   BOS_BOOL       bNoSymUdp;           /* True to disable symmetric UDP, i.e.
                                       ** send UDP packets using a different port
                                       ** from the listening port.
                                       */
} CCTKLOCPORT;

/* Connection activity configuration */
typedef struct CCTKCNXCFG
{
   BOS_UINT32     uMinCnx;             /* Min number of connections to keep alive.
                                       ** This must be smaller than or equal to
                                       ** uMaxCnx. Set to CCTKCFG_DEFAULT_CNX_CFG for default, i.e. 5.
                                       */
   BOS_UINT32     uMaxCnx;             /* Max number of simulteously connections
                                       ** allowed. Set to CCTKCFG_DEFAULT_CNX_CFG for default, i.e. 10.
                                       */
   BOS_UINT32     uMaxIdleTime;        /* Max time (in sec) a connection can remain
                                       ** without activity before be closed (unless
                                       ** this means going under uMinCnx).
                                       ** Set to CCTKCFG_DEFAULT_CNX_CFG for default, i.e. 64.
                                       */
} CCTKCNXCFG;


/* Transaction timer configuration */
typedef struct CCTKTRANSTMRCFG
{
   BOS_UINT32     uT1;                 /* T1 retransmission timer (in ms).
                                       ** Set to 0 for default, i.e. 500.
                                       */
   BOS_UINT32     uT2;                 /* T2 retransmission timer (in ms).
                                       ** Set to 0 for default, i.e. 4000.
                                       */
   BOS_UINT32     uTimeout;            /* Transaction timeout timer (in ms).
                                       ** Set to 0 for default, i.e. 64 * uT1 = 32s.
                                       ** This affects the D or J timers.
                                       */
   BOS_UINT32     uT4;                 /* T4 duration a message will remain on
                                       ** network timer (in ms).
                                       ** Set to 0 for default, i.e. 5000.
                                       */
   BOS_UINT32     uB;                  /* B transaction timeout timer (in ms).
                                       ** Set to 0 for default, i.e. 64 * uT1 = 32s.
                                       */
   BOS_UINT32     uD;                  /* D wait timer for response retransmits
                                       ** (in ms). Set to 0 for default,
                                       ** i.e. 64 * uT1 = 32s for UDP, 0s for TCP.
                                       */
   BOS_UINT32     uF;                  /* F non-INVITE transaction timer (in ms).
                                       ** Set to 0 for default, i.e. 64 * uT1 = 32s.
                                       */
   BOS_UINT32     uH;                  /* H wait time for ACK receipt timer (in ms).
                                       ** Set to 0 for default, i.e. 64 * uT1 = 32s.
                                       */
   BOS_UINT32     uJ;                  /* J wait time for non-INVITE request
                                       ** retransmit timer (in ms). Set to 0 for
                                       ** default, i.e. 64 * uT1 = 32s for UDP,
                                       ** 0s for TCP.
                                       */
} CCTKTRANSTMRCFG;

#if CCTKCFG_MEMORY_MONITORING
/* Memory allocation protection policy */
typedef enum CCTKMEMPROT
{
   eCCTKMEMPROT_NOTIFY,                /* Report event to app with
                                       ** eCCTKEVT_MEM_OVERTHRESHOLD
                                       ** on allocation above threshold.
                                       ** eCCTKEVT_MEM_UNDERTHRESHOLD
                                       ** on allocation below threshold.
                                       */
   eCCTKMEMPROT_DENY_REQ,              /* Deny all incoming requests silently
                                       ** (until allocation below threshold).
                                       */
   eCCTKMEMPROT_DENY_REQ_NOTIFY        /* Deny all incoming requests and
                                       ** report event to app with
                                       ** eCCTKEVT_MEM_OVERTHRESHOLD
                                       ** on allocation above threshold.
                                       ** eCCTKEVT_MEM_UNDERTHRESHOLD
                                       ** on allocation below threshold.
                                       */
} CCTKMEMPROT;

/* Memory allocation protection */
typedef struct CCTKMEMPROTCFG
{
   BOS_UINT32     uThreshold;          /* Memory threshold (in bytes).
                                       ** Set to 0 to disable.
                                       */
   CCTKMEMPROT    ePolicy;             /* Protection policy */
} CCTKMEMPROTCFG;
#endif /* CCTKCFG_MEMORY_MONITORING */

/* IP address list in numerical format */
typedef struct CCTKIPADDRLST
{
   BOS_UINT32     uNum;                /* Number of IP addresses */
   BOS_IP_ADDRESS addr[CCTKCFG_MAX_ADDRLST];
                                       /* IP address list */
} CCTKIPADDRLST;

/* White list configuration */
typedef struct CCTKWHTLSTCFG
{
   CCTKIPADDRLST  lst;                 /* IP address(es) */
   BOS_BOOL       bOnlyFromLst;        /* True to accept incoming packets only from
                                       ** the list
                                       */
} CCTKWHTLSTCFG;

#if CCTKCFG_TLS
/* Diffie-Hellman prime */
typedef struct CCTKDHPRIME
{
   BOS_UINT8      *puPrime;            /* Diffie-Hellman prime with length "uLen" */
   BOS_UINT32     uLen;                /* Prime length (in bits) for generating
                                       ** Diffie-Hellman keys. Set to 0 for default
                                       ** prime length, i.e. 256.
                                       */
} CCTKDHPRIME;

/* TLS configuration */
typedef struct CCTKTLSCFG
{
   BOS_UINT8      **ppcCertChain;      /* Local certificate chain in PEM format
                                       ** (NULL terminated in order CA, int1, int2,
                                       ** ..., local).
                                       */
   BOS_UINT8      **ppcTrustedCerts;   /* Trusted certificates in PEM format */
   BOS_UINT8      *pcPrivKey;          /* Private key associated with the local
                                       ** certificate in PEM format.
                                       */
   BOS_UINT8      *pcCipher;           /* Rule set that indicates which ciphers are
                                       ** allowed or not. E.g. "aRSA+SHA1" for
                                       ** cipher with RSA algorithm with SHA1
                                       ** digest. If set to NULL, all supported
                                       ** cipher suites would be used.
                                       */
   BOS_BOOL       bClientAuth;         /* True to enable TLS client peer
                                       ** authentication.
                                       */
   BOS_BOOL       bServerAuth;         /* True to enable TLS server peer
                                       ** authentication.
                                       */
   CCTKDHPRIME    preGenDhPrime;       /* Pre-generated Diffie-Hellman prime.
                                       ** Set "puPrime" to NULL to generate the
                                       ** prime at real time. Prime generation can
                                       ** be a CPU intensive task.
                                       */
} CCTKTLSCFG;
#endif /* #if CCTKCFG_TLS */

/* MWI event reporting policy */
typedef enum CCTKMWIEVT
{
   eCCTKMWIEVT_NOTIFY_ALWAYS,          /* Report MWI event to app on every MWI
                                       ** notification.
                                       */
   eCCTKMWIEVT_NOTIFY_UPDATE           /* Report MWI event to app only if message
                                       ** summary in incoming MWI notification is
                                       ** different from previous.
                                       */
} CCTKMWIEVT;

/* MWI operational policies */
typedef struct CCTKMWIMODE
{
   CCTKMWIEVT     eEvtPolicy;          /* MWI event reporting policy */
} CCTKMWIMODE;

/* MWI configuration */
typedef struct CCTKMWICFG
{
   CCTKMWIMODE    mode;                /* MWI operational policies */
} CCTKMWICFG;

/* MWI timer configuration */
typedef struct CCTKMWITMR
{
   BOS_UINT32     uExpTmr;             /* MWI expiration time (in sec).
                                       ** Used only when MWI server is configured.
                                       ** Set to 0 to disable. This is the
                                       ** expiration specified in the SUBSCRIBE.
                                       */
   BOS_UINT32     uRetryTmr;           /* MWI subscription failure retry time
                                       ** (in sec). Used only when MWI is enabled.
                                       ** This is the time to wait for re-attempting
                                       ** the subscription following a failure of
                                       ** the current attempted one. Set to 0 to
                                       ** retry immediately.
                                       */
} CCTKMWITMR;

/* Registration timer configuration */
typedef struct CCTKREGTMR
{
   BOS_UINT32     uRegExpTmr;          /* Registration expiration time (in sec).
                                       ** Used only when registrar is used. Set to 0
                                       ** to disable. This is the expiration
                                       ** specified in the REGISTER.
                                       ** Note: the Registrar could override the
                                       ** actual expiration time in the 200 OK.
                                       */
   BOS_UINT32     uSubExpTmr;          /* Registration state subscription expiration
                                       ** time (in sec). Used only when registrar is
                                       ** used. Set to 0 to use the default value,
                                       ** i.e. 3761 (RFC3680).
                                       */
   BOS_UINT32     uThreshold;          /* Time (in sec) prior to the contacts
                                       ** expiration that CCTK would attempt to
                                       ** re-register.
                                       ** Note: This value is not used when half of
                                       ** the expiration time returned by the
                                       ** registrar is less than this value. CCTK
                                       ** will refresh the registration half of the
                                       ** returned expiration time before the
                                       ** registration expires.
                                       */
   BOS_UINT32     uRegRetryTmr;        /* Registration failure retry time (in sec).
                                       ** Used only when registrar is used.  This is
                                       ** the time to wait for attempting the next
                                       ** registration following a failure of the
                                       ** current attempted one.  Set to 0 to retry
                                       ** immediately.  Some process such as the
                                       ** backup and recovery may alter this value
                                       ** automatically as required by the feature
                                       ** internal behavior.
                                       */
   BOS_UINT32     uSubRetryTmr;        /* Registration state subscription failure
                                       ** retry time (in sec). This is the time to
                                       ** wait for re-attempting a subscription
                                       ** following a failure of the current
                                       ** attempted one. Set to 0 to retry
                                       ** immediately.
                                       */
   BOS_UINT32     uFlowMaxTmr;         /* Keep alive flow recovery max timer
                                       ** as defined in RFC 5626.  Set to 0 to
                                       ** bypass flow recovery procedure and try
                                       ** to recover immediately.
                                       */
   BOS_UINT32     uFlowBaseNokTmr;     /* Keep alive flow recovery base timer
                                       ** as defined in RFC 5626 applied when all
                                       ** possible connections have failed.  Set
                                       ** to 0 to bypass exponential flow recovery
                                       ** procedure.
                                       */ 
   BOS_UINT32     uFlowBaseOkTmr;      /* Keep alive flow recovery base timer
                                       ** as defined in RFC 5626 applied when at
                                       ** least one connection was successful
                                       ** previously.  Set to 0 to bypass 
                                       ** exponential flow recovery procedure.
                                       */
   BOS_UINT32     uRegRetryTmrOn403;   /* Registration failure retry timer (in sec)
                                       ** used only for 403 response.  This is
                                       ** the time to wait for attempting the next
                                       ** registration following a failure of the
                                       ** current attempted one.  When the timer
                                       ** is set, it will override the bitmask
                                       ** CCTK_INTEROP_REGENDON403.
                                       */
} CCTKREGTMR;

/* "User-Agent" header field format */
typedef enum CCTKUAHDRFMT
{
   eCCTKUAHDRFMT_FULL,                 /* Include "pcHdr" (if not NULL) along with
                                       ** CCTK and SIP stack versions. (default)
                                       */
   eCCTKUAHDRFMT_APPONLY               /* Include "pcHdr" only. CCTK and SIP stack
                                       ** versions are not included. If "pcHdr" is
                                       ** NULL, "User-Agent" header would not be
                                       ** included.
                                       */
} CCTKUAHDRFMT;

/* User agent identity header configuration */
typedef struct CCTKUAIDHDR
{
   BOS_UINT8      *pcUaHdr;            /* Name of the product, filled in to part of
                                       ** the "User-Agent" header field. Set to NULL
                                       ** if not used. See also "eUaHdrFmt".
                                       ** Note this field is not used if privacy
                                       ** service is enabled.
                                       */
   CCTKUAHDRFMT   eUaHdrFmt;           /* "User-Agent" header format for outgoing
                                       ** packets.
                                       */
   BOS_UINT8      *pcSvrHdr;           /* Value of the "Server" header field for all
                                       ** SIP responses. Set to NULL if not used.
                                       */
   BOS_UINT8      *pcSipInstId;        /* Value of the "sip.instance" field
                                       ** parameter in the "Contact" header field
                                       ** for outgoing REGISTER requests.
                                       ** draft-ietf-sip-outbound-13 requires the
                                       ** instance ID must be a URN (i.e. begins
                                       ** with "urn:") and its value must persist
                                       ** across reboot cycles.
                                       ** Set to NULL if not used.
                                       */
} CCTKUAIDHDR;

/* Routing-related header configuration */
typedef struct CCTKROUTINGHDR
{
   BOS_UINT32     uMaxFwds;            /* Value to fill into the "Max-Forwards"
                                       ** header as in RFC 3261.
                                       ** Set to 0 for default, i.e. 70.
                                       */
} CCTKROUTINGHDR;

/* Address header information */
typedef struct CCTKADDRHDR
{
   BOS_UINT8      *pcAddr;             /* Header's URI value. Note that the URI-
                                       ** scheme must be specified (e.g.
                                       ** "sip:usr@host:port;parm"). Set to NULL if
                                       ** this address header info is not used.
                                       */
   BOS_UINT8      *pcHdrParm;          /* Header parameters with semi-colons
                                       ** separated (e.g. "parm1=ab;parm2;parm3=1").
                                       ** This is not used if "pcAddr" is not
                                       ** configured. Set to NULL if no parameters
                                       ** are to be included.
                                       */
} CCTKADDRHDR;

/* User contact configuration */
typedef struct CCTKCONTACT
{
   CCTKADDRHDR    hdrOverride;         /* Override the "Contact" header value. When
                                       ** this is not configured, the Contact header
                                       ** URI value is derived from the system's
                                       ** default host address "addr" in
                                       ** CCTKHOSTINFO for all outgoing SIP packets.
                                       ** This feature is commonly used if the
                                       ** contact address (i.e. WAN address) is
                                       ** different from the configured host address
                                       ** (e.g. CCTK operates behind NAT). Note that
                                       ** enabling this feature can affect the SCE
                                       ** contact building, e.g. GRUU contact will
                                       ** not be updated internally. Set "pcAddr" to
                                       ** NULL to let CCTK generate the contact
                                       ** address based on the default "addr" info.
                                       */
   BOS_UINT8      *pcUsrParm;          /* User parameter value "user=" to be
                                       ** inserted to the Contact header URI. If
                                       ** user type needs to be specified in the
                                       ** Contact SIP-URI, e.g.
                                       ** "sip:usr@host:port;user=phone", one should
                                       ** configure this field to "phone".
                                       */
} CCTKCONTACT;

#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
/* Telephone call configuration */
typedef struct CCTKTELCALL
{
   BOS_BOOL       bNoEnum;             /* True to make telephone call without
                                       ** electronic numbering (ENUM) lookup.
                                       ** Outbound proxy must be configured when
                                       ** "bNoEnum" is enabled.
                                       ** (not used if CCTKCFG_TEL_ALL is not set)
                                       */
   BOS_BOOL       bDenyInCall;         /* True to deny incoming calls with tel-URI
                                       ** as Request-URI.
                                       */
} CCTKTELCALL;
#endif

/* PRACK policy */
typedef enum CCTKPRACK
{
   eCCTKPRACK_SUPPORTED,               /* PRACK supported */
   eCCTKPRACK_REQUIRED,                /* PRACK required */
   eCCTKPRACK_DISABLED                 /* PRACK disabled */
} CCTKPRACK;

/* Connected ID policy */
typedef enum CCTKCONNECTEDID
{
   eCCTKCONNECTEDID_SUPPORTED,          /* Connected ID supported */
   eCCTKCONNECTEDID_DISABLED             /* Connected ID disabled */
} CCTKCONNECTEDID;

/* Session timer method policy */
typedef enum CCTKSESTMRMODE
{
   eCCTKSESTMRMODE_UPDATE,             /* UPDATE used for session refresh
                                       ** (recommended by RFC4028), only if the
                                       ** remote party supports UPDATE, i.e. UPDATE
                                       ** in Allow header. CCTK will automatically
                                       ** fall back to use INVITE if UPDATE is not
                                       ** supported.
                                       */
   eCCTKSESTMRMODE_INVITE              /* INVITE used for session refresh */
} CCTKSESTMRMODE;

/* Session timer configuration */
typedef struct CCTKSESTMR
{
   BOS_UINT32     uTmr;                /* Session timer (in sec). 0 to disable. */
   BOS_UINT32     uMinSe;              /* "Min-SE" header for session timer
                                       ** (in sec). Used only if session timer is
                                       ** enabled, i.e. "uTmr" > 0. "Min-SE" header
                                       ** not sent when value is 0.
                                       */
   CCTKSESTMRMODE eRefresh;            /* Method used for session refresh */
} CCTKSESTMR;

/* Privacy level control
** NOTE: If level is not set to eCCTKPRIVLVL_NONE, user-provided privacy is applied
** except for REGISTER where "user" is inserted in Privacy header.
*/
typedef enum CCTKPRIVLVL
{
   eCCTKPRIVLVL_NONE,                  /* None */
   eCCTKPRIVLVL_LOCAL,                 /* Local */
   eCCTKPRIVLVL_NETID,                 /* Network - Id */
   eCCTKPRIVLVL_NETIDNC,               /* Network - Id (Non Critical) */
   eCCTKPRIVLVL_NETHDR,                /* Network - Header */
   eCCTKPRIVLVL_NETSES,                /* Network - Session */
   eCCTKPRIVLVL_ALL                    /* All */
} CCTKPRIVLVL;

/* Privacy configuration */
typedef struct CCTKPRIVCFG
{
   CCTKPRIVLVL    eLevel;              /* Privacy level */
   BOS_BOOL       bCritical;           /* True to indicate privacy service is
                                       ** critical. If any of the privacy services
                                       ** specified in "eLevel" cannot be provided
                                       ** by the network, the outgoing SIP packet
                                       ** will be rejected (see RFC3323). Not used
                                       ** if "eLevel" is set to eCCTKPRIVLVL_NONE.
                                       */
   BOS_BOOL       bRetry;              /* True to internally retry (without
                                       ** notifying the app) sending the request
                                       ** without privacy support when 433
                                       ** (Anonymity Disallowed) is received.
                                       ** False to notify the app with 433 response.
                                       */
   BOS_BOOL       bRejectAnon;         /* True to reject anonymous incoming requests
                                       ** with the 433 response.
                                       */
} CCTKPRIVCFG;

/* Event subscription timer configuration */
typedef struct CCTKEVTTMR
{
   BOS_UINT32     uRetryTmr;           /* Event subscription failure retry time
                                       ** (in sec). This is the time to wait for
                                       ** re-attempting an event subscription
                                       ** following a failure of the current
                                       ** attempted one. Set to 0 to retry
                                       ** immediately.
                                       */
   BOS_BOOL       bMap500To408;        /* Whether to map a failure code of 500 to
                                       ** be equivalent in behavior as a 408 code.
                                       ** If TRUE, it means essentially to reattempt
                                       ** the event subscription on such failure.
                                       */
} CCTKEVTTMR;

/* NAI client operational mode
** NOTE: Section 6 of RFC3325 mandates that pre-loaded route must be configured when
** preferred identity support is enabled.
** (i.e. other than eCCTKNAICLTMODE_NET is configured)
*/
typedef enum CCTKNAICLTMODE
{
   eCCTKNAICLTMODE_NET,                /* Network identity generated by network
                                       ** (i.e. no P-Preferred-Identity header is
                                       ** sent).
                                       */
   eCCTKNAICLTMODE_AOR,                /* Address-of-record is used as preferred
                                       ** network identity (i.e. AOR used in
                                       ** P-Preferred-Identity header).
                                       */
   eCCTKNAICLTMODE_CONTACT             /* Contact address is used as preferred
                                       ** network identity (i.e. Contact used in
                                       ** P-Preferred-Identity header).
                                       */
} CCTKNAICLTMODE;

/* NAI client operational policy */
typedef struct CCTKNAICLT
{
   CCTKNAICLTMODE eMode;               /* Client operational mode */
   BOS_BOOL       bSec;                /* True to indicate network identity needs to
                                       ** be sent securely. "id" is added to
                                       ** "Privacy" header.
                                       */
   CCTKSIPUSRHOST *pPreferredId;       /* Preferred network identity to be used in
                                       ** the P-Preferred-Identity header. Enabling
                                       ** this will ignore the value in "eMode".
                                       ** Set to NULL to use the value in "eMode"
                                       ** for generating the P-Preferred-Identity
                                       ** header value).
                                       */
} CCTKNAICLT;

/* NAI server operational mode
** NOTE: Section 6 of RFC3325 mandates that pre-loaded route must be configured when
** network-asserted identity support is enabled.
** (i.e. other than eCCTKNAISVRMODE_IGNORED is configured)
*/
typedef enum CCTKNAISVRMODE
{
   eCCTKNAISVRMODE_WHTLST,             /* Use network identity received from hosts
                                       ** in white-list only, i.e. CCTKWHTLSTCFG. If
                                       ** white-list is not defined, network
                                       ** identity is ignored.
                                       */
   eCCTKNAISVRMODE_IGNORED,            /* Incoming network identity is ignored, i.e.
                                       ** "P-Asserted-Identity" header is not used
                                       ** by CCTK.
                                       */
   eCCTKNAISVRMODE_ANY                 /* Use network identity received from any
                                       ** hosts.
                                       */
} CCTKNAISVRMODE;

/* NAI server operational policy */
typedef struct CCTKNAISVR
{
   CCTKNAISVRMODE eMode;               /* Server operational mode */
} CCTKNAISVR;

/* Network asserted identity configuration */
typedef struct CCTKNAICFG
{
   CCTKNAICLT     cli;                 /* Client operational policy */
   CCTKNAISVR     svr;                 /* Server operational policy */
} CCTKNAICFG;

/* Call hold configuration */
typedef struct CCTKHOLDCFG
{
   BOS_BOOL       bNoQuadZeros;        /* True to use the RFC3264 hold method only.
                                       ** False to use the old 0.0.0.0 hold method
                                       ** for IPv4 along with the a=sendrecv/
                                       ** sendonly/recvonly/inactive attribute set,
                                       ** and switch to use the RFC 3264 hold method
                                       ** only if an offer SDP with a= set to
                                       ** sendonly/inactive is received. Set this to
                                       ** false to ensure backwards compatibility.
                                       */
   BOS_BOOL       bUseInactive;        /* True to use the a=inactive media attribute
                                       ** instead of a=sendonly when putting a call
                                       ** on hold.
                                       */
   BOS_BOOL       bUseUpdate;          /* True to use UPDATE for call hold/unhold,
                                       ** only if the remote party supports UPDATE,
                                       ** i.e. in Allow header. Fall back to use
                                       ** INVITE if UPDATE is not supported.
                                       ** False to use INVITE only (default).
                                       */
   BOS_BOOL       bLocalHold;          /* Special hold mode by not sending re-INVITE
                                       ** nor UPDATE when the local hold state
                                       ** changes (i.e. the remote party would not
                                       ** be notified with the local hold event).
                                       ** Note that bUseInactive must be set to
                                       ** BOS_TRUE when the local hold mode is used.
                                       */
   BOS_BOOL       bBypassHoldRspChk;   /* True to bypass hold response processing
                                       ** instead of ensuring that the hold response
                                       ** has all the expected attributes set (such
                                       ** as media.
                                       */
} CCTKHOLDCFG;

/* Transferer configuration */
typedef struct CCTKXFERERCFG
{
   BOS_UINT32     uNotifyWaitTime;     /* Wait time for the first notification from
                                       ** transferee. Transferer will terminate the
                                       ** existing calls from transferer to 
                                       ** transferee and transfer target when the
                                       ** wait time expires. Set to 0 to disable the
                                       ** time-out, i.e. wait for the first NOTIFY
                                       ** indefinitely.
                                       */
   BOS_BOOL       bEndOnNotify;        /* True to terminate the existing calls from
                                       ** from transferer to transferee and transfer
                                       ** target, upon receiving the first NOTIFY
                                       ** from the transferee. Otherwise, the calls
                                       ** are terminated either the time waiting for
                                       ** the first notification expires, i.e.
                                       ** uNotifyWaitTime, or the application ends
                                       ** the calls explicitly.
                                       */
   BOS_BOOL       bUseOodRefer;        /* True to use out-of-dialog REFER in order
                                       ** to originate a call transfer.  This also
                                       ** requires to support RFC 4538 (the target
                                       ** dialog header).  If set to true and the
                                       ** remote end is not advertizing support for
                                       ** RFC 4538, then the behavior is dictated
                                       ** by the bOodRefFallback flag.
                                       */
   BOS_BOOL       bOodRefFallBack;     /* True to fallback using in-dialog REFER
                                       ** in order to originate a call transfer in
                                       ** the case where we may have determined
                                       ** that the transfer target is not supporting
                                       ** out of dialog REFER.  If false and the
                                       ** transfer target is not supporting out of
                                       ** dialog REFER then attempt to use out of
                                       ** dialog REFER for transfer will result in
                                       ** a failure to transfer.
                                       */
   BOS_BOOL       bCleanOnFail;        /* True to terminate all the calls associated
                                       ** with a transfer when that latter has failed
                                       ** for any reason.  If false, the calls will be
                                       ** returned in the latest state they were in
                                       ** prior to the failed transfer attempt.
                                       */
} CCTKXFERERCFG;

/* Transferee (transferred party) operational policy */
typedef enum CCTKXFEREE
{
   CCTKXFEREE_ACCEPT,                  /* Accept all incoming call transfers,
                                       ** i.e. REFER.
                                       */
   CCTKXFEREE_DENY                     /* Deny all incoming call transfers,
                                       ** i.e. REFER.
                                       */
} CCTKXFEREE;

/* Transfer target operational policy */
typedef enum CCTKXFERTGET
{
   CCTKXFERTGET_ACCEPT,                /* Accept all incoming replacement calls */
   CCTKXFERTGET_DENY                   /* Deny all incoming replacement calls */
} CCTKXFERTGET;

/* Call transfer configuration */
typedef struct CCTKXFERCFG
{
   CCTKXFERERCFG  tfererCfg;           /* Transferer configuration */
   CCTKXFEREE     eTferee;             /* Transferee (transferred party) operational
                                       ** policy.
                                       */
   CCTKXFERTGET   eTarget;             /* Transfer target operational policy */
} CCTKXFERCFG;

/* Media negotiation configuration */
typedef struct CCTKMEDNEGCFG
{
   BOS_BOOL       bUseUpdate;          /* True to use UPDATE for media update, only
                                       ** if the remote party supports UPDATE, i.e.
                                       ** in Allow header. Fall back to use INVITE
                                       ** if UPDATE is not supported.
                                       ** False to use INVITE only (default).
                                       */
   BOS_BOOL       bUseMptime;          /* Use mptime instead of ptime. */
   BOS_BOOL       bSymmetricPtime;     /* Respond with the offer ptime if exists. */
   BOS_BOOL       bUseBandwidth;       /* Include b=TIAS, b=AS and a=maxprate */
   BOS_BOOL       bDisableRTCP;        /* Include b=RS:0 and b=RR:0 (RFC 3556) to
                                       ** disable receiving RTCP from remote.
                                       */
   BOS_BOOL       bSdpAnswerIn180;     /* Include SDP answer in 180 */
   BOS_BOOL       bFirstSdpIsFinal;    /* Treat the first sdp received as the final answer */
   BOS_BOOL       bLbSupport;          /* Loopback support.  Enabled to accept
                                       ** incoming Loopback offers.
                                       */
   BOS_UINT8      uRtpCode[eCCTKCODEC_MAX];
                                       /* List of RTP payload code mapping
                                       ** (static and dynamic), CCTKCODEC indexed.
                                       */
   BOS_UINT8      uTelEvtRtpCode;      /* RTP payload code for telephone-event. */
   BOS_UINT8      uTelEvtWbRtpCode;    /* RTP payload code for wideband telephone-event. */
   BOS_UINT8      uEncapRtpCode;       /* RTP payload code for loopback mode
                                       ** (encapsulated RTP).
                                       */
   BOS_UINT8      uRtpLbRtpCode;       /* RTP payload code for loopback mode
                                       ** (RTP loopback).
                                       */
   BOS_UINT8      uCnRtpCode;          /* RTP payload code for comfort noise. */
   BOS_UINT8      uCnWbRtpCode;        /* RTP payload code for wideband comfort noise. */
   BOS_BOOL       bSdpNoStaticRtpmap;  /* Control inclusion of 'rtpmap' attribute
                                       ** for static payload type. */
   /* TODO: m-line grouping */
} CCTKMEDNEGCFG;

/* General NAT configuration */
typedef struct CCTKNATGENCFG
{
   BOS_BOOL       bNoRport;            /* True to disable "rport" support in all
                                       ** outgoing requests.
                                       */
} CCTKNATGENCFG;

/* DNS query policy */
typedef enum CCTKDNSMODE
{
   eCCTKDNSMODE_ALL,                   /* All DNS query supports are enabled */
   eCCTKDNSMODE_NO_NAPTR,              /* NAPTR DNS query support is disabled while
                                       ** SRV and A/AAAA are enabled.
                                       */
   eCCTKDNSMODE_NO_NAPTR_SRV           /* NAPTR and SRV DNS query supports are
                                       ** disabled. Only A/AAAA query is supported.
                                       */
} CCTKDNSMODE;

/* Header type */
typedef enum CCTKHDRTYPE
{
   eCCTKHDRTYPE_PRIVATE,               /* Private "P-" header.
                                       ** Private header name (i.e. pcName) must be
                                       ** filled in if this is enabled.
                                       */
   eCCTKHDRTYPE_CALL_INFO,             /* Call-Info header */
   eCCTKHDRTYPE_WARNING,               /* Warning header */
   eCCTKHDRTYPE_AUTHORIZATION,         /* Authorization header */
} CCTKHDRTYPE;

/* Header structure */
typedef struct CCTKHDR
{
   CCTKHDRTYPE    eType;               /* Header type */
   BOS_UINT8      *pcName;             /* Private header name. Used only if eType
                                       ** is set to eCCTKHDRTYPE_PRIVATE.
                                       ** The header name must begin with "P-" for
                                       ** indicating a private SIP header.
                                       */
   BOS_UINT8      *pcValue;            /* Header value
                                       ** (including header parameters).
                                       */
} CCTKHDR;

/* Header insertion mapping */
typedef enum CCTKHDRMAP
{
   eCCTKHDRMAP_ALL,                    /* Insert header in all outgoing SIP packet
                                       ** (i.e. requests and responses).
                                       */
   eCCTKHDRMAP_REQ,                    /* Insert header in outgoing requests */
   eCCTKHDRMAP_RSP                     /* Insert header in outgoing responses */
} CCTKHDRMAP;

/* Header insertion policy */
typedef enum CCTKHDRMODE
{
   eCCTKHDRMODE_APPEND,                /* Append header (even if there exists a
                                       ** header with the same header name).
                                       */
   eCCTKHDRMODE_OVRWRITE,              /* Overwrite any existing header with the
                                       ** same header name with the new header.
                                       ** Insert the header if there does not exist
                                       ** any header with the same header name.
                                       */
   eCCTKHDRMODE_ONCE                   /* Insert header only if there does not exist
                                       ** any header with the same header name.
                                       */
} CCTKHDRMODE;

/* Generic header */
typedef struct CCTKGENHDR
{
   CCTKHDR        hdr;                 /* Header structure */
   BOS_UINT32     uMethod;             /* Methods where the generic header is
                                       ** inserted (CCTK_METHOD_XXX) (bitmap)
                                       */
   CCTKHDRMAP     eMap;                /* Header insertion mapping */
   CCTKHDRMODE    eMode;               /* Header insertion policy */
} CCTKGENHDR;

/* Generic header configuration */
typedef struct CCTKGENHDRCFG
{
   BOS_UINT32     uNum;                /* Number of generic headers to be inserted
                                       ** into the SIP packet.
                                       */
   CCTKGENHDR     hdr[CCTKCFG_MAX_GENHDRLST];
                                       /* Generic header list */
} CCTKGENHDRCFG;

/* IMS-specific config */
typedef struct CCTKIMSCFG
{
   BOS_BOOL       bNoAuthHdrReg;       /* False to include Authorization header
                                       ** in all outgoing REGISTER requests.
                                       ** True to send REGISTER requests without
                                       ** Authorization header unless CCTK is
                                       ** challenged by the registrar.
                                       */
   BOS_BOOL       bClidInfoFrom;       /* True to use the From header information
                                       ** to derive CLID data.
                                       ** False to only use P-Asserted-Identity
                                       ** header information to derive CLID data.
                                       */
   BOS_BOOL       bClidInfoPaiSip;     /* True to use the SIP URI present in the
                                       ** P-Asserted-Identity to derive CLID
                                       ** information if no other means have been
                                       ** able to provide one.
                                       */
   BOS_BOOL       bCnamePaiSip;        /* True to use the TEL URI present in the
                                       ** P-Asserted-Identity to derive CNAME
                                       ** information if no other means have been
                                       ** able to provide one.
                                       */
   BOS_BOOL       bIgnoreInvalidPai;   /* True to ignore invalid URI present in the 
                                       ** P-Asserted-Identity instead of rejecting
                                       ** the request.
                                       */
   BOS_BOOL       bNoClidInfoPai;      /* True will universally disable taking
                                       ** CLID information from P-Asserted-Idenity
                                       */                                 
} CCTKIMSCFG;

/* GRUU generation policy */
typedef enum CCTKGRUUMODE
{
   eCCTKGRUUMODE_REG,                  /* GRUU support is enabled with GRUU
                                       ** obtained through registration.
                                       ** Enabling this must enable instance ID
                                       ** (i.e. fill in "pcSipInstId").
                                       */
   eCCTKGRUUMODE_DISABLED,             /* GRUU support is disabled */
   eCCTKGRUUMODE_SELFMADE              /* GRUU support is enabled with a self-made
                                       ** GRUU. The self-made GRUU is constructed
                                       ** using AOR if "aor" is set, or hostname if
                                       ** "aor" is not set, as URI domain.
                                       ** "pcUsrName" is used as user part of URI.
                                       ** "pcSipInstId" is used as "gr" URI
                                       ** parameter. (pcUsrName@aor;gr=pcSipInstId)
                                       ** Enabling this must enable instance ID
                                       ** (i.e. fill in "pcSipInstId"). This feature
                                       ** is useful when GRUU cannot be obtained
                                       ** through registration, either registration
                                       ** is not possible (i.e. no registrar is
                                       ** configured) or registrar does not support
                                       ** GRUU. If a registrar with GRUU support is
                                       ** configured, the GRUU obtained from
                                       ** registrar is used instead.
                                       */
} CCTKGRUUMODE;

/* GRUU config */
typedef struct CCTKGRUUCFG
{
   CCTKGRUUMODE   eMode;               /* GRUU generation policy */
} CCTKGRUUCFG;

/* Dialog event config */
typedef struct CCTKDIAGEVTCFG
{
   BOS_UINT8      *pcSubscriber;       /* URI address of dialog event subscriber,
                                       ** which is allowed to receive notifications
                                       ** on the detailled dialog information of
                                       ** a user.
                                       ** This feature is commonly used if a service
                                       ** administrator needs the knowledge of the
                                       ** user's dialog usage state. Set to NULL to
                                       ** disallow any third party to get a clear
                                       ** view of the dialog(s) present on the user.
                                       ** Note that setting to NULL will not disable
                                       ** services such as auto-recall and auto-
                                       ** callback where any third party user can
                                       ** subscribe to dialog event notification but
                                       ** sees only an overview of the dialog info
                                       ** as opposed to the full content.
                                       */
   BOS_UINT32     uSuberMax;           /* Maximum number allowed of concurent dialog
                                       ** event subscription originated by the local
                                       ** user at any given time.  Subscription in
                                       ** excess will fail to be sent out.
                                       */
   BOS_UINT32     uSubeeMax;           /* Maximum number allowed of concurent dialog
                                       ** event subscription received by the local
                                       ** user at any given time.  Subscription in
                                       ** excess will be rejected.
                                       */
} CCTKDIAGEVTCFG;

/* User-Agent Profile event config */
typedef struct CCTKUAPEVTCFG
{
   BOS_UINT32     uSubExpTmr;          /* User-agent profile subscription expiration
                                       ** time (in sec). Set to 0 to use the default
                                       ** value, i.e. 86400 as defined in
                                       ** draft-ietf-sipping-config-framework.
                                       */
   BOS_BOOL       bUnsolNtfy;          /* True to allow incoming unsolicited UA profile
                                       ** notification. False to treat unsolicited UA
                                       ** profile notification as not valid.
                                       */
   BOS_UINT32     uNum;                /* Number of profile configured.
                                       */
   BOS_UINT8      *pcProfile[CCTKCFG_MAX_PROFCFGLST];
                                       /* Profile configuration to be used for the
                                       ** subscription to the user-agent profile of
                                       ** interest.
                                       */

} CCTKUAPEVTCFG;

/* Emergency call config */
typedef struct CCTKEMGCALLCFG
{
   BOS_BOOL       bEmergency;          /* True to indicate this is an emergency
                                       ** call. False to indicate a regular call.
                                       */
   BOS_BOOL       bAddLocInfo;         /* True to indicate need to add location
                                       ** information as part of the call setup.
                                       ** False otherwise.
                                       */
} CCTKEMGCALLCFG;

/* IPv4 Type of Serviece (ToS) config */
typedef struct CCTKTOSCFG
{
   BOS_UINT8      uDefault;            /* Default ToS octet value to be used in the
                                       ** IP header for all outgoing packets.
                                       */
   BOS_UINT8      uEmgCall;            /* Type of service (ToS) octet value for
                                       ** emergency calls. This value is used in the
                                       ** IP header of outgoing emergency call
                                       ** packets, so it overrides the "uDefault"
                                       ** value above. Set to 0 to use the default
                                       ** ToS value specified in "uDefault".
                                       */
} CCTKTOSCFG;

/* User local port config */
typedef struct CCTKUSRPORTCFG
{
   CCTKTOSCFG     tosCfg;              /* IPv4 Type of Service (ToS) configuration.
                                       ** This configuration is not used in IPv6.
                                       */
} CCTKUSRPORTCFG;

/* User support for emergency call config */
typedef struct CCTKUSREMGCFG
{
   BOS_BOOL       bEndCallOnInc;       /* True to explicitly terminate any call
                                       ** associated with the user upon receiving
                                       ** a new emergency call from the network.
                                       ** The emergency call is determined by 
                                       ** proper signaling.  False otherwise.
                                       */

} CCTKUSREMGCFG;

typedef enum CCTKBRSVCMODE
{
   eCCTKBRSVCMODE_DISABLED,            /* Disable the backup and recovery feature
                                       ** for the user.
                                       */
   eCCTKBRSVCMODE_DLM,                 /* Manage destination selection via the DNS
                                       ** lookup manipulation.
                                       */
   eCCTKBRSVCMODE_EXTLIST,             /* Target addresses are found by external
                                       ** means and provided to the backup and recovery
                                       ** module by configuring the extList. This method
                                       ** controls the destination target via outbound
                                       ** proxy configuration - to be used in conjunction
                                       ** with persistent connection.
                                       */
} CCTKBRSVCMODE;

/* Back to primary operational mode
*/
typedef enum CCTKBRBACKTOPRIMMODE
{
   eCCTKBRBACKTOPRIMMODE_DISABLED,     /* Failback to primary is disabled.
                                       */
   eCCTKBRBACKTOPRIMMODE_SILENT,       /* The failback registration is attempted
                                       ** in parallel to the current re-registration.
                                       ** If the failback re-registration succeeds
                                       ** then the user silently switches over to
                                       ** primary target, otherwise registration
                                       ** is maintaiend on the current target.
                                       */
   eCCTKBRBACKTOPRIMMODE_DEREG,        /* On the third succesful re-registration
                                       ** attempt, the user will de-register from
                                       ** the current target and attempt registration
                                       ** with the primary target.
                                       */
} CCTKBRBACKTOPRIMMODE;

/* Backup and recovery operational mode
*/
typedef enum CCTKBRBKOFFMODE
{
   eCCTKBRBKOFFMODE_ALLFAILED,         /* Apply exponential back off when all
                                       ** known targets have been tried and
                                       ** and have failed.
                                       */
   eCCTKBRBKOFFMODE_ANYFAIL,           /* Apply backoff algorithm on every
                                       ** target failure, even if not all
                                       ** targets have been tried yet.
                                       */
} CCTKBRBKOFFMODE;

/* User support for backup and recovery config */
typedef struct CCTKUSRBRCFG
{
   CCTKBRSVCMODE  eMode;               /* The service mode of the feature.
                                       */
   BOS_BOOL       bFailover;           /* True to act on the target failure by
                                       ** attempting to recover registration
                                       ** through failover process. False otherwise,
                                       ** to not act on the failure.
                                       */

   BOS_BOOL       bEnableInbdReg;      /* True to attempt registration to a non-registered
                                       ** server when an incoming call is received
                                       ** from it. If registration to the 'inbound
                                       ** server is successful, the old one will
                                       ** be terminated and the new one honored, and
                                       ** all calls will be maintained on the server
                                       ** it was established on.
                                       ** False otherwise
                                       */
   CCTKBRBACKTOPRIMMODE eBackToPrimMode;  /* Back to primary mode to recover
                                       **  registration on the 'primary' server.
                                       ** The primary server discovered is the
                                       ** first server from the server list we
                                       ** are working with that we connected 
                                       ** to successfully.
                                       */
   CCTKBRBKOFFMODE eBackoffMode;       /* Backoff algorithm to use when target failure
                                       ** is detected.
                                       */
   BOS_BOOL       bOutboundBackoff;    /* True to use RFC5626 backoff algorithm,
                                       ** where the wait time would be a random
                                       ** value within 50-100% of the upper bound
                                       ** wait-time:
                                       ** W = min(max-time,
                                       **     base-time*(2^consecutive-failures)))
                                       ** If set to false simple exponential backoff
                                       ** will be applied, where the wait-time is:
                                       ** W = min(max_time,
                                              min_time*(2^(consec-allfailed - 1)))
                                       */
   BOS_UINT32     uBackoffMinTmr;      /* The min value (in seconds) for the
                                       ** exponential backoff algorithm implemented
                                       ** as part of the backup process.  Set to 0
                                       ** to disable exponential backoff and use
                                       ** pre-defined fixed value (5 minutes). If
                                       ** outbound backoff mode is specified
                                       ** this value will be used as the base-time
                                       ** parameter for the wait time formula in
                                       ** RFC 5626.
                                       */
   BOS_UINT32     uBackoffMaxTmr;      /* The max value (in seconds) for the
                                       ** exponential backoff algorithm implemented
                                       ** as part of the backup process.  Set to 0
                                       ** to disable exponential backoff and use
                                       ** uBackoffMinTmr as fixed timer value if set
                                       ** (see above).
                                       */
   BOS_UINT32     uBackToPrimMinTmr;   /* The min value (in seconds) for to attempt 
                                       ** to re-register back to the 'primary' server
                                       ** Note, this is used in conjunction with 
                                       ** uBackToPrimMaxTmr where the retry timer is
                                       ** calculated as rand (MinTmr, Max Tmr).
                                       ** Set to 0 to disable using the random timer
                                       */
   BOS_UINT32     uBackToPrimMaxTmr;   /* The max value (in seconds) for to attempt 
                                       ** to re-register back to the 'primary' server
                                       ** Note, this is used in conjunction with 
                                       ** uBackToPrimMaxTmr where the retry timer is
                                       ** calculated as rand (MinTmr, Max Tmr).
                                       ** Set to 0 to disable using the random timer
                                       */
   CCTKIPPORTLST   extList;            /* Target list to use when the eMode is set to
                                       ** eCCTKBRSVCMODE_EXTLIST. Ignore otherwise.
                                       */
} CCTKUSRBRCFG;

/* Service-Route header configuration */
typedef struct CCTKSVCROUTE
{
   CCTKROUTE      route[CCTKCFG_MAX_OBPROXY];
                                       /* Route set to be appended to the pre-loaded
                                       ** route set, if it is configured (refer to
                                       ** "obProxyLst"). The SIP stack will use the
                                       ** sum of the pre-loaded route and the
                                       ** configured Service-Route when building
                                       ** the Route header for an outgoing packet.
                                       */
} CCTKSVCROUTE;

/* User registration event package config */
typedef struct CCTKREGEVTCFG
{
   BOS_BOOL       bRegEvtSupport;      /* Control run time support for registration
                                       ** state event package subscription and use.
                                       */
   BOS_BOOL       bMapRejToDeact;      /* Control whether to re-map the 'rejected'
                                       ** event to 'deactivated' in order to apply
                                       ** similar processing in either cases.
                                       */
} CCTKREGEVTCFG;

/* SIP signaling behavioral config */
typedef struct CCTKSIGBEHAVE
{
   BOS_UINT32     uTx;                 /* Control outgoing SIP signaling behaviour
                                       ** (CCTK_SIGBEHVTX_XXX).
                                       */
   BOS_UINT32     uRx;                 /* Control incoming SIP signaling behaviour
                                       ** (CCTK_SIGBEHVRX_XXX).
                                       */
} CCTKSIGBEHAVE;

/* Signaling transport protocol. */
typedef struct CCTKSIGTRANS
{
   BOS_UINT32    uSigTrans;            /* Acceptable signaling transport type
                                       ** bitmap (CCTK_TRANSPORT_XXX)
                                       */
   BOS_UINT32    uTcpFallbackThresh;   /* Threshold of the signaling transport MTU
                                       ** to attempt TCP fallback first.  Setting
                                       ** a value of 0 will disable this feature.
                                       ** Note that TCP as a signaling transport
                                       ** must be enabled for this to work.
                                       */
   BOS_BOOL      bKeepAlive;           /* BOS_TRUE to enable keep alive on the
                                       ** signaling transport link.  BOS_FALSE
                                       ** otherwise.  Note that in order to
                                       ** enable keep alive, one need to be setup
                                       ** in using IMS configuration profile (i.e.
                                       ** 'outbound keep alive').
                                       */

} CCTKSIGTRANS; 


/* P-Asscociated-URI config */
typedef struct CCTKPAUCFG
{
   BOS_BOOL bNoPauUnreg;               /* BOS_TRUE to invalidate a
                                       ** successful registration if no
                                       ** P-Associated-URI information
                                       ** was present. BOS_FALSE otherwise.
                                       */
   BOS_BOOL bPauNoValid;               /* BOS_TRUE to prevent the full
                                       ** validation as per IMS rules
                                       ** of the P-Associated-URI. BOS_FALSE
                                       ** otherwise.
                                       */
   BOS_BOOL bPauUpdCtc;                /* BOS_TRUE to update the Contact header
                                       ** information with the P-Associated-URI
                                       ** content, this can lead to failure to
                                       ** place any subsequent request when the
                                       ** proxy authenticates requests on the
                                       ** registered Contact.  BOS_FALSE otherwise.
                                       */
   BOS_BOOL bPauUsrSwap;               /* BOS_TRUE to swap the current user with
                                       ** the first user as discovered from the
                                       ** P-Associated-URI content if this later
                                       ** is not the same default user.  BOS_FALSE
                                       ** otherwise.
                                       */

} CCTKPAUCFG;

/* P-Access-Network-Info config */
typedef struct CCTKPANICFG
{
   BOS_BOOL       bIncludeNetworkInfo; /* BOS_TRUE to include the 
                                       ** P-Access-Network-Info header.
                                       */
   BOS_UINT8      *pcNetworkInfo;      /* String to append to the
                                       ** P-Access-Network-Info header
                                       */

} CCTKPANICFG;

/* Authentication config */
typedef struct CCTKAUTHCFG
{
   BOS_UINT32     uloopMax;         /* Number of consecutive authentication challenges
                                    ** that will be handled before a loop is detected 
                                    ** and authentication is canceled.
                                    */
}CCTKAUTHCFG;

/***********************************************************************************
**  FUNCTION:   CCTK_RGNAT_SES_ADD_FUNC
**
**  PURPOSE:    Callback function prototype for adding a RG NAT session.
**
**  PARAMETERS:
**              internalIpAdd  - the LAN client address for the new RG NAT session
**              uInternalPort  - the starting LAN client port
**              uPortCnt       - number of needed continuous port   
**              remoteIpAdd    - the Remote party address (Optional) 
**              uRemotePort    - the Remote party starting port (Optional)
**              iProt          - protocol for the NAT session: 0=UDP, 1=TCP
**              uNatTimeout    - timeout value for the expected NAT entry
**                               The NAT session will be expired and closed if no
**                               data flows thru after this timeout duration.
**              pExternalIpAdd - the mapped external address (WAN address) 
**              puExternalPort - the mapped starting external port (WAN port) 
**              puSesHandle    - the returned session handle
**
**  RETURNS:    BOS_TRUE if successful, and BOS_FALSE otherwise
**
**  NOTES:      
***********************************************************************************/
typedef BOS_BOOL ( *CCTK_RGNAT_SES_ADD_FUNC )( 
   BOS_IP_ADDRESS *internalIpAdd, // (IN) LAN client address 
   BOS_UINT16     uInternalPort,  // (IN) LAN client starting port 
   BOS_UINT16     uPortCnt,       // (IN) number of continuous port needed    
   BOS_IP_ADDRESS *remoteIpAdd,   // (IN) Remote party address (Optional) 
   BOS_UINT16     uRemotePort,    // (IN) Remote party starting port (Optional)
   int            iProt,          // (IN) protocol : 0=UDP, 1=TCP
   BOS_UINT32     uNatTimeout,    // (IN) value for timeout expected NAT entry 
   BOS_IP_ADDRESS *pExternalIpAdd, // (OUT) WAN mapped address 
   BOS_UINT16     *puExternalPort, // (OUT) WAN mapped starting port 
   BOS_UINT32     *puSesHandle     // (OUT) Handle assigned for this new mapping
);

/***********************************************************************************
**  FUNCTION:   CCTK_RGNAT_SES_DELETE_FUNC
**
**  PURPOSE:    Callback function prototype for removing an opened RG NAT session.
**
**  PARAMETERS:
**              uSesHandle - the returned puSesHandle from the 
**                           CCTK_RGNAT_SES_ADD_FUNC function
**
**  RETURNS:    None
**
**  NOTES:      
***********************************************************************************/
typedef void ( *CCTK_RGNAT_SES_DELETE_FUNC )( BOS_UINT32 uSesHandle );


/***********************************************************************************
**  FUNCTION:   CCTK_RGNAT_IS_LAN_ADDR_FUNC
**
**  PURPOSE:    Callback function prototype for determining if an address is 
**              a LAN address
**
**  PARAMETERS:
**              addr - the address to be checked
**
**  RETURNS:    BOS_TRUE if it is a LAN address, BOS_FALSE otherwise.
**
**  NOTES:      
***********************************************************************************/
typedef BOS_BOOL ( *CCTK_RGNAT_IS_LAN_ADDR_FUNC )( const BOS_IP_ADDRESS *addr );

/* RG NAT Session Control configuration */
typedef struct CCTKRGNATCTL
{
   CCTK_RGNAT_IS_LAN_ADDR_FUNC pNatIsLanAddrFunc;
   CCTK_RGNAT_SES_ADD_FUNC     pNatSesAddFunc;
   CCTK_RGNAT_SES_DELETE_FUNC  pNatSesDelFunc;
} CCTKRGNATCTL;


#if CCTKCFG_REMOTE_USER_CONTROL
/* General SIP remote user system configuration */

typedef enum CCTKREMUSRADMMODE
{
   eCCTKREMUSRADMMODE_NONE = 0,
   eCCTKREMUSRADMMODE_PRECONFIG = 1,
   eCCTKREMUSRADMMODE_LANUSR = 2,
   eCCTKREMUSRADMMODE_ALL = 3,
} CCTKREMUSRADMMODE;

typedef struct CCTKREMUSRSYS
{
   BOS_BOOL             bRemUsrSupport;
   CCTKREMUSRADMMODE    eAdmMode;
   BOS_UINT32           uDftAssIMPUIndexRef;
   BOS_UINT8            *pcDftAssIMPUUserId;
   BOS_UINT32           uDftIMPIIndexRef;
   BOS_UINT8            *pcDftIMPIUserId;
   BOS_UINT8            *pcDftIMPIUserPassword;
} CCTKREMUSRSYS;

/* SIP remote user configuration */
typedef struct CCTKREMUSRCFG
{
   BOS_UINT32     uIMPIIndexRef;
   BOS_UINT8      *pcIMPIUserId;
   BOS_UINT8      *pcIMPIPassword;
} CCTKREMUSRCFG;

#endif /* CCTKCFG_REMOTE_USER_CONTROL */  

#if CCTKCFG_ICE_LITE_SUPPORT

typedef enum CCTKICESTUNHMACTYPE
{
   eCCTKICESTUNHMACTYPE_HMAC_SHA1 = 0,
} CCTKICESTUNHMACTYPE;

/***********************************************************************************
**  FUNCTION:   CCTK_ICE_OPEN_CNX_FUNC
**
**  PURPOSE:    Callback function prototype for opening a socket of an ICE
**              connnection.
**
**  PARAMETERS:
**              lclAddr  - [IN] the local address
**              lclPort  - [IN] the local port
**              pCnxHdl  - [OUT] returned handle of the connection
**
**  RETURNS:    BOS_STATUS_OK if successful, and otherwise if failure
**
**  NOTES:      
***********************************************************************************/
typedef BOS_STATUS ( *CCTK_ICE_OPEN_CNX_FUNC )( 
   const BOS_IP_ADDRESS* lclAddr,
   const BOS_UINT16 lclPort,
   void* *pCnxHdl );
   
/***********************************************************************************
**  FUNCTION:   CCTK_ICE_CLOSE_CNX_FUNC
**
**  PURPOSE:    Callback function prototype for closing a socket of an ICE
**              connnection.
**
**  PARAMETERS:
**              cnxHdl    - [IN] handle of the connection
**
**  RETURNS:    BOS_STATUS_OK if successful, and otherwise if failure
**
**  NOTES:      
***********************************************************************************/
typedef BOS_STATUS ( *CCTK_ICE_CLOSE_CNX_FUNC )( void* cnxHdl );

typedef STUN_SENDPKTCB CCTK_ICE_STUN_SEND_FUNC;
typedef STUN_LOGCB CCTK_ICE_LOG_FUNC;
typedef STUN_CRC32CB CCTK_ICE_CRC32_FUNC;
typedef STUN_HMACCB CCTK_ICE_HMAC_FUNC;
   
/* ICE Control configuration */
typedef struct CCTKICECTLCFG
{
   CCTK_ICE_OPEN_CNX_FUNC    pIceOpenCnxFunc;
   CCTK_ICE_CLOSE_CNX_FUNC   pIceCloseCnxFunc;
   CCTK_ICE_STUN_SEND_FUNC      pIceStunSendFunc;
   CCTK_ICE_LOG_FUNC            pIceLogFunc;
   CCTK_ICE_CRC32_FUNC          pIceCrc32Func;
   CCTK_ICE_HMAC_FUNC           pIceHmacFunc;
   
   BOS_BOOL               bIPv6PrefMedia;   /* IPv6 is the Pref IP for Media */
} CCTKICECTLCFG;
   
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
/* ---- Variable Externs -------------------------------------------------------- */
/* ---- Function Prototypes ----------------------------------------------------- */

#endif /* __CCTK_EXT_CFG_API__H__INCLUDED__ */
