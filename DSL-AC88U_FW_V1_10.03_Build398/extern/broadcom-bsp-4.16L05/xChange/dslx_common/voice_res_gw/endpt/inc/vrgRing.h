/****************************************************************************
 *
 * Copyright 2008 Broadcom
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


#ifndef VRGRING_H_
#define VRGRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "vrgTypes.h"

/** Bit-field representation of Ring Cadence.
 *
 * The ring cadence representation starts with the
 * first 1 in the pattern (the leading 0s in the MSB are
 * padding and are to be ignored).  Each bit
 * represents 100ms of tone; 1 is tone, 0 is no tone. 64
 * bits MUST be used for cadence representation, LSB 4 bits
 * are used for representing repeatable characteristics.
 * 0000 means repeatable, and 1000 means non repeatable.
 * As an example, the
 * hex representation of a ring cadence of 0.5 secs on; 4
 * secs off; repeatable would be:0x0001F00000000000.
 */
typedef struct VRG_RING_SHORT_PATTERN
{
   VRG_UINT32 pattern1;
   VRG_UINT32 pattern2;

} VRG_RING_SHORT_PATTERN;

/** Ring Cadence extended to include repeatability characteristics.
 *
 * The first three higher order octets are reserved. The
 * octets that follow are used to encode a 'bit-string', with
 * each bit corresponding to 50 milliseconds. A bit value of
 * '1' indicates the presence of a ring-tone and a bit value
 * of '0' indicates the absence of a ring-tone, for that
 * duration (50 ms) (Note: A minimum number of octets
 * required to encode the bit-string MUST be used).
 *
 * The first two of the reserved octets MUST indicate the
 * length of the encoded cadence (in bits) and MUST range
 * between 1 and 296. (Note: The length in bits MUST also be
 * consistent with the number of octets that encode the
 * cadence).
 *
 * The third of the reserved octets indicates 'repeatability'.
 * The MSB = 1: indicating 'non-repeatability' and the MSB = 0:
 * indicating 'repeatability'. The least 7 bits is ring state value
 *  where the repeat pattern starts when MSB = 0.
 */
typedef struct VRG_RING_LONG_RING_PATTERN
{
   VRG_UINT32  pattern1;
   VRG_UINT32  pattern2;
   VRG_UINT32  pattern3;
   VRG_UINT32  pattern4;
   VRG_UINT32  pattern5;
   VRG_UINT32  pattern6;
   VRG_UINT32  pattern7;
   VRG_UINT32  pattern8;
   VRG_UINT32  pattern9;
   VRG_UINT32  pattern10;

} VRG_RING_LONG_PATTERN;

/** Ring Cadence. */
typedef union VRG_RING_PATTERN
{
   /* 6 sec ring pattern - each bit represents a 100 ms duration of the ring cadence */
   VRG_RING_SHORT_PATTERN     shortRingPattern;

   /* 13.2 sec ring pattern - each bit represents a 50 ms duration of the ring cadence */
   VRG_RING_LONG_PATTERN      longRingPattern;

} VRG_RING_PATTERN;

/** Ring Cadence selection. */
typedef enum VRG_RING_TYPE
{
   VRG_RING_SHORT = 1,
   VRG_RING_LONG
} VRG_RING_TYPE;

/** Provision-able Ring Cadence profile. */
typedef struct VRG_RING_PROFILE
{
   VRG_RING_TYPE    type;
   VRG_RING_PATTERN pattern;
} VRG_RING_PROFILE;

#ifdef __cplusplus
}
#endif

#endif /* VRGRING_H_ */

