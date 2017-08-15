/****************************************************************************
 *
 * Copyright 2009 Broadcom
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
 *****************************************************************************/

#ifndef VRGCLASS_H_
#define VRGCLASS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "vrgTypes.h"

#define VRG_CLASS_MAX_PRIVATECODE_COMPONENTS 16
#define VRG_CLASS_MAX_UNAVAILCODE_COMPONENTS 16
#define VRG_CLASS_MAX_ERRORCODE_COMPONENTS 16
/** International Class FSK parameters */
typedef struct
{
   VRG_UINT16 amplitude;         /**< Amplitude (pos. number in tenths of dB) */
   VRG_UINT16 markFrequency;     /**< Mark Frequency */
   VRG_UINT16 spaceFrequency;    /**< Space Frequency */
   VRG_SINT16 startNumSeizes;    /**< number of seizes to send for start msg */
   VRG_UINT16 startNumMarks;     /**< number of marks to follow seize for start msg */

} VRG_CLASS_FSK;

/** International Class DTMF parameters */
typedef struct
{
   VRG_UINT16 dbLevel;                /**< DTMF tone level Amplitude */
   VRG_UINT16 pulseTime;              /**< Duration of DTMF on and off periods */
   char startCode;                    /**< DTMF start code */
   char endCode;                      /**< DTMF end code */
   char privateCode[16];              /**< DTMF code for "private" (must end with error code) */
   char unavailCode[16];              /**< DTMF code for "unavailable"/"blocked" (must end with error code) */
   char errorCode[16];                /**< DTMF code for "error" (ie cannot be delivered due to technical reasons */
   char redirectingCode;              /**< DTMF digit signalling redirecting number */
   char informationCode;              /**< DTMF digit signalling information string */
   VRG_UINT16 DTMFpauseTimeout;       /**< Timeout for recieving next DTMF digit after previous */
   VRG_UINT16 DTMFsessionTimeout;     /**< Timeout for recieving whole CLID in a session */
   VRG_UINT16 ringAfterClidTimeout;   /**< Timeout for recieving a ring after pre-ring CLID */

} VRG_CLASS_DTMF;

#ifdef __cplusplus
}
#endif

#endif /* VRGCLASS_H_ */
