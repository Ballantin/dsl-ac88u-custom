/****************************************************************************
*
* Copyright © 2000-2007 Broadcom
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
*    Filename: vrgEndptCfg.h
*
****************************************************************************
*    Description:
*
*     VRG endpt default build options.  To override defaults in
*     vrgCfg.h make a file called vrgCfgCustom.h in your build
*     directory.
*
****************************************************************************/

#ifndef VRG_ENDPT_CFG_H
#define VRG_ENDPT_CFG_H

#ifdef __cplusplus
extern "C" {
#endif


#include <vrgEndptCfgCustom.h>


/*
*****************************************************************************
** CONSTANT DECLARATIONS
*****************************************************************************
*/


/* Task priority for VRG Event Processing */
#ifndef VRG_ENDPT_CFG_DISP_TASK_PRIORITY
#define VRG_ENDPT_CFG_DISP_TASK_PRIORITY           BOS_TASK_CLASS_MED_HIGH
#endif


/* Maximum number of VRG endpoints */
#ifndef VRG_ENDPT_CFG_NUM_ENDPT
#define VRG_ENDPT_CFG_NUM_ENDPT                    4
#endif


/* Maximum number of VRG connections */
#ifndef VRG_ENDPT_CFG_NUM_CNX
#define VRG_ENDPT_CFG_NUM_CNX                      6
#endif

/* Disable TPD functionality */
#ifndef VRG_ENDPT_CFG_USE_TPD
#define VRG_ENDPT_CFG_USE_TPD                      0
#endif

/* Disable MLT API */
#ifndef VRG_ENDPT_CFG_USE_MLT
#define VRG_ENDPT_CFG_USE_MLT                      0
#else
#if (VRG_ENDPT_CFG_USE_TPD == 0)
#error 'MLT API requires TPD support'
#endif
#endif

/* Disable Hook-flash Emulation */
#ifndef VRG_ENDPT_CFG_USE_HFE
#define VRG_ENDPT_CFG_USE_HFE                      0
#else
#ifndef VRG_ENDPT_CFG_HFE_INTERVAL
#define VRG_ENDPT_CFG_HFE_INTERVAL                 600
#endif
#endif

/* Disable APM Pulse Metering functionality */
#ifndef VRG_ENDPT_CFG_USE_APM_PULSE_METERING
#define VRG_ENDPT_CFG_USE_APM_PULSE_METERING       0
#endif

/* Disable Endpoint Muting while Onhook */
#ifndef VRG_ENDPT_CFG_ONHOOK_MUTING_SUPPORT
#define VRG_ENDPT_CFG_ONHOOK_MUTING_SUPPORT        0
#endif

/* Maximum number of connections per endpoint. */
#ifndef VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT
#define VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT            3
#endif

/* Support for G726 ITU.366 Appendix E */
#ifndef VRG_ENDPT_CFG_G726_ITU_I_366_2_ANNEX_E
#define VRG_ENDPT_CFG_G726_ITU_I_366_2_ANNEX_E     0
#endif

/* Pre-load RTP payload map table with static payload types.  If enabled,
** this feature allows decoding of any pre-configured static payload types
** even if not negotiated during call establishment by higher layers.
*/
#ifndef VRG_ENDPT_CFG_PRELOAD_RTP_PAYLOAD_MAP
#define VRG_ENDPT_CFG_PRELOAD_RTP_PAYLOAD_MAP      0
#endif

/* Audio packet to stop tone playout.  If enabled, this feature allows
** an egress audio packet to stop the tone playout.
*/
#ifndef VRG_ENDPT_CFG_AUDIO_STOP_TONE
#define VRG_ENDPT_CFG_AUDIO_STOP_TONE              0
#endif

/* Egress packets coming from the network are looped back in the DSP
** and sent back to the network as ingress packets */
#ifndef VRG_ENDPT_CFG_EGRESSPACKET_LOOPBACK
#define VRG_ENDPT_CFG_EGRESSPACKET_LOOPBACK        0
#endif

/* Support for faxr debug info logging */
#ifndef VRG_ENDPT_CFG_FAXR_DEBUG_INFO
#define VRG_ENDPT_CFG_FAXR_DEBUG_INFO              1
#endif

/* Enable if two-way speech path is desired when connection
** is created, but the line is still on-hook. */
#ifndef VRG_ENDPT_CFG_TWO_WAY_PATH_IN_ONHOOK
#define VRG_ENDPT_CFG_TWO_WAY_PATH_IN_ONHOOK       0
#endif

/* Convert 0xFF to 0x7F in payload of PCMU ingress packet */
#ifndef VRG_ENDPT_CFG_INGRESS_RTP_G711U_SPECIAL_ENCODING
#define VRG_ENDPT_CFG_INGRESS_RTP_G711U_SPECIAL_ENCODING    0
#endif

/* Force provisioning integrity check and return error during initialization 
** if ANY provisioning parameter is not set */
#ifndef VRG_ENDPT_CFG_FORCE_PROV_INTEGRITY_CHECK
#define VRG_ENDPT_CFG_FORCE_PROV_INTEGRITY_CHECK            0
#endif

/* APM Channel Swapping */
#ifndef VRG_ENDPT_CFG_APM_CHAN_SWAP
#define VRG_ENDPT_CFG_APM_CHAN_SWAP             0
#endif


#ifdef __cplusplus
}
#endif

#endif
