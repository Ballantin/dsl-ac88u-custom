/****************************************************************************
*
*  Copyright (c) 2005-2008 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*  Filename: bcmChip.h
*
****************************************************************************
*  Description: Wrapper for inclusion of chip-specific defines
*
*
****************************************************************************/

#ifndef BCMCHIP_H
#define BCMCHIP_H

#ifdef BRCM_6328
#include <bcm6328.h>
#endif /* BRCM_6328 */

#ifdef BRCM_6362
#include <bcm6362.h>
#endif /* BRCM_6362 */

#ifdef BRCM_6368
#include <bcm6368.h>
#endif /* BRCM_6368 */

#ifdef BRCM_68500
#include <bcm68500.h>
#endif /* BRCM_68500 */

#ifdef BRCM_6816
#include <bcm6816.h>
#endif /* BRCM_6816 */

#ifdef BRCM_63268
#include <bcm63268.h>
#endif /* BRCM_63268 */

#ifdef BRCM_6818
#include <bcm6818.h>
#endif /* BRCM_6818 */

#ifdef BRCM_6838
#include <bcm6838.h>
#endif /* BRCM_6838 */

#ifdef BRCM_6828
#include <bcm6828.h>
#endif /* BRCM_6828 */

#ifdef BRCM_63138
#include <bcm63138.h>
#endif /* BRCM_63138 */

#ifdef BRCM_63148
#include <bcm63148.h>
#endif /* BRCM_63148 */

#ifdef BRCM_63381
#include <bcm63381.h>
#endif /* BRCM_63381 */

#ifdef BRCM_6848
#include <bcm6848.h>
#endif /* BRCM_6848 */
/*
** Chip ids and submasks for the currently available chips.
** (GPIO->DieRevID & CHIP_SUB_ID_MASK_<base_chip>) should result in CHIP_ID_<base_chip>
** where chip can be either 6328, 6362, 6368 or 6816
*/
#define CHIP_ID_6328                0x63280000
#define CHIP_SUB_ID_MASK_6328       0xFFFF0000

#define CHIP_ID_6362                0x63620000
#define CHIP_SUB_ID_MASK_6362       0xFFFF0000

#define CHIP_ID_6368                0x63680000
/* Chip id for 6368-based chips can be either 6368 or 6369. */
#define CHIP_SUB_ID_MASK_6368       0xFFFE0000

#define CHIP_ID_6816                0x68100000
/* Multiple chip id combinations are possible for 6816-based chips all starting with 681X */
#define CHIP_SUB_ID_MASK_6816       0xFFF00000

#define CHIP_ID_63268               0x63268000
#define CHIP_SUB_ID_MASK_63268      0xFFFFF000

#define CHIP_ID_6818                0x68180000
#define CHIP_SUB_ID_MASK_6818       0xFFFFF000

#define CHIP_ID_6838                0x68380000
#define CHIP_SUB_ID_MASK_6838       0xFFFFF000

#define CHIP_ID_6828                0x68280000
#define CHIP_SUB_ID_MASK_6828       0xFFFFF000

#define CHIP_ID_63138               0x63138000
#define CHIP_SUB_ID_MASK_63138      0xFFFFF000

#define CHIP_ID_63148               0x63148000
#define CHIP_SUB_ID_MASK_63148      0xFFFFF000

#define CHIP_ID_63381               0x63381000
#define CHIP_SUB_ID_MASK_63381      0xFFFFF000

#define CHIP_ID_6848                0x68480000
#define CHIP_SUB_ID_MASK_6848       0xFFFF0000

#endif /* BCMCHIP_H */


