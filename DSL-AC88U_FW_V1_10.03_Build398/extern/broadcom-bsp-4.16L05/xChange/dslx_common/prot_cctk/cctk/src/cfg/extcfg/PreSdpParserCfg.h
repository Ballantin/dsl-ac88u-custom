/**********************************************************************************
** Copyright (c) 2007-2012 Broadcom
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
**      This file contains the pre SDP Parser configuration definitions.
**
***********************************************************************************/

#ifndef MXG_PRESDPPARSERCFG_H
#define MXG_PRESDPPARSERCFG_H

#include <cctkCfg.h>

/* T.38 support */
#define MXD_SDP_ENABLE_T38_SUPPORT

/* Telephone-Event support */
#define MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE

/* RED(RFC2198) support */
#define MXD_SDP_ENABLE_REDUNDANCY_FMTP_ATTRIBUTE

/* G.722.1 support */
#define MXD_SDP_ENABLE_G7221_FMTP_ATTRIBUTE

/* Preconditions attributes support */
#define MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT

/* SRTP support */
#undef MXD_SDP_ENABLE_SRTP_SUPPORT
#if CCTKCFG_SRTP
   #define MXD_SDP_ENABLE_SRTP_SUPPORT
#endif

/* Video support */
#undef MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT
#if CCTKCFG_VIDEO
   #define MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT
#endif

/* Custom RTP codecs */
#define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM                             \
   eGSMEFR,                         /* RFC4856 */                                  \
   eGSMAMR,                         /* RFC4867 */                                  \
   eAMRWB,                                                                         \
   eLBENCAPRTP,                     /* draft-ietf-mmusic-media-loopback-08 */      \
   eLBLBRTPLOOPBACK,                /* draft-ietf-mmusic-media-loopback-08 */      \
   eG72248,                         /* G722-48 */                                  \
   eG72256,                         /* G722-56 */                                  \
   eG72264,                         /* G722-64 */
#define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME                             \
   "GSM-EFR",                       /* eGSMEFR */                                  \
   "AMR",                           /* eGSMAMR */                                  \
   "AMR-WB",                        /* eAMRWB  */                                  \
   "encaprtp",                      /* draft-ietf-mmusic-media-loopback-08 */      \
   "rtploopback",                   /* draft-ietf-mmusic-media-loopback-08 */      \
   "G722-48",                       /* G722-48 */                                  \
   "G722-56",                       /* G722-56 */                                  \
   "G722-64",                       /* G722-64 */
#define MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY                                    \
   {116, 8000,  1},                 /* eGSMEFR */                                  \
   {117, 8000,  1},                 /* eGSMAMR */                                  \
   {118, 16000, 1},                 /* eAMRWB  */                                  \
   {119, 8000,  1},                 /* draft-ietf-mmusic-media-loopback-08 */      \
   {120, 8000,  1},                 /* draft-ietf-mmusic-media-loopback-08 */      \
   {121, 8000,  1},                 /* G722-48 */                                  \
   {122, 8000,  1},                 /* G722-56 */                                  \
   {123, 8000,  1},                 /* G722-64 */

#define MXD_SDP_ICE_ENABLE_SUPPORT
/* #define MXD_BRCM_SDP_GENERATOR */

#endif /* #ifndef MXG_PRESDPPARSERCFG_H */
