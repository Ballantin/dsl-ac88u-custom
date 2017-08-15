/****************************************************************************
*
* Copyright (c) 2006-2011 Broadcom
*
* This program is the proprietary software of Broadcom and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*  Filename: ntcfg.h
*
****************************************************************************
*    Description:
*      Configuration header for NAT Traversal modules.
*
****************************************************************************/
#ifndef __NTCFG_H_
#define __NTCFG_H_



/* ------------------------------------------------------------------------ */
/* ------------ Features -------------------------------------------------- */
/* ------------------------------------------------------------------------ */

/* -- STUN
**
**  This flag enables the support of synchronous events in the STUN module
*/
#define NTCFG_STUN_SYNCD_RESPONSE_SUPPORT          0

/* -- STUN
**
**  These information are defining the thread characteristics for the STUN Tx
**  module.
*/
#define NTCFG_STUN_TX_THREAD_PRIORITY              BOS_TASK_CLASS_MED
#define NTCFG_STUN_TX_THREAD_STACK                 (6 * 1024)

/* -- STUN
**
**  This flag enables support for the latest retransmission scheme for STUN
**  Tx module defined in RFC 5389.  If not enabled, the retransmission
**  scheme used is the one backward compatible with the earlier RFC 3489.
*/
#define NTCFG_STUN_RFC5389_RETRANS_SUPPORT      0


/* -- TURN
**
**  This flag enables the support of TURN in the STUN module
*/
#define NTCFG_TURN_SUPPORT                         0
#define NTCFG_STUN_MAGIC_COOKIE_SUPPORT            0

/* -- ICE
**
**  This flag enables the support of ICE in the STUN and TURN module
*/
#define NTCFG_ICE_SUPPORT                         0

/*
*****************************************************************************
** CUSTOM CONFIGURATION - This must be included after all default
** build configuration options. Custom include files should undef default
** config options and then re-define them.
*****************************************************************************
*/
#include <ntcfgCustom.h>

#endif

