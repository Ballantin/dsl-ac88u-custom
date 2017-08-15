/****************************************************************************
*
* Copyright © 2008-2010 Broadcom
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
*****************************************************************************
*
*    Description:
*
*      Si32392 mode definitions
*
****************************************************************************/

#ifndef SLIC_SI32392_DEFS_H
#define SLIC_SI32392_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************
** SI32392 Specific Defines
************************************************************************/
#define SI32392_READ_CMD        0x00
#define SI32392_WRITE_CMD       0x10

/************************************************************************
** Register addresses - In Numerical Order
************************************************************************/
#define SI32392_REG_DEVICE_STATUS                    (0)
#define SI32392_REG_LINEFEED_CONFIG_1                (1)
#define SI32392_REG_LINEFEED_CONFIG_2                (2)
#define SI32392_REG_LINEFEED_CONFIG_3                (3)

/************************************************************************
** Register bit field defines & masks
************************************************************************/

/* Device status - Reg 0 */
#define SI32392_CHAN0_HOOKSWITCH_STAT_ON      0x0000   /* Bit 11: 0-On-hook */
#define SI32392_CHAN0_HOOKSWITCH_STAT_OFF     0x0800   /* Bit 11: 1-Off-hook */
#define SI32392_CHAN0_HOOKSWITCH_STAT_MASK    0x0800   /* Bit 11 Reg0 */

#define SI32392_CHAN1_HOOKSWITCH_STAT_ON      0x0000   /* Bit 5: 0-On-hook */
#define SI32392_CHAN1_HOOKSWITCH_STAT_OFF     0x0020   /* Bit 5: 1-Off-hook */
#define SI32392_CHAN1_HOOKSWITCH_STAT_MASK    0x0020   /* Bit 5 Reg0 */

/* Linefeed config 1 */
#define SI32392_CHAN0_MODE_DISCONNECT         0x0000   /* Disconnect */
#define SI32392_CHAN0_MODE_STANDBY            0x0200   /* Standby */
#define SI32392_CHAN0_MODE_ACTIVE             0x0400   /* Active */
#define SI32392_CHAN0_MODE_POLARITY_REV       0x0600   /* Polarity Reversal */
#define SI32392_CHAN0_MODE_RING_OPEN          0x0800   /* Ring open */
#define SI32392_CHAN0_MODE_TIP_OPEN           0x0A00   /* Tip open */
#define SI32392_CHAN0_MODE_AC_RING_TRIP       0x0C00   /* Ringing with ac ring trip */
#define SI32392_CHAN0_MODE_DC_RING_TRIP       0x0E00   /* Ringing with dc ring trip */
#define SI32392_CHAN0_MODE_MASK               0x0E00   /* Bits 9-11 of Reg1 are Chan 0 mode select */
#define SI32392_CHAN0_MODE_SHIFT                   9   /* Bits 9-11 of Reg1 are Chan 0 mode select */

#define SI32392_CHAN1_MODE_DISCONNECT         0x0000   /* Disconnect */
#define SI32392_CHAN1_MODE_STANDBY            0x0010   /* Standby */
#define SI32392_CHAN1_MODE_ACTIVE             0x0020   /* Active */
#define SI32392_CHAN1_MODE_POLARITY_REV       0x0030   /* Polarity Reversal */
#define SI32392_CHAN1_MODE_RING_OPEN          0x0040   /* Ring open */
#define SI32392_CHAN1_MODE_TIP_OPEN           0x0050   /* Tip open */
#define SI32392_CHAN1_MODE_AC_RING_TRIP       0x0060   /* Ringing with ac ring trip */
#define SI32392_CHAN1_MODE_DC_RING_TRIP       0x0070   /* Ringing with dc ring trip */
#define SI32392_CHAN1_MODE_MASK               0x0070   /* Bit 4-6 of Reg1 are Chan 1 mode select */
#define SI32392_CHAN1_MODE_SHIFT                   4   /* Bit 4-6 of Reg1 are Chan 1 mode select */

/* Linefeed config 2 */
#define SI32392_CHAN0_LOOP_CUR_THRESH_25      0x0000   /* Bit 11&1: 00-25mA */
#define SI32392_CHAN0_LOOP_CUR_THRESH_40      0x0800   /* Bit 11&1: 10-40mA */
#define SI32392_CHAN0_LOOP_CUR_THRESH_32      0x0002   /* Bit 11&1: x1-32mA */
#define SI32392_CHAN0_LOOP_CUR_THRESH_MASK    0x0802   /* Bit 11&1 Reg2 */

#define SI32392_CHAN1_LOOP_CUR_THRESH_25      0x0000   /* Bit 8&0: 00-25mA */
#define SI32392_CHAN1_LOOP_CUR_THRESH_40      0x0100   /* Bit 8&0: 10-40mA */
#define SI32392_CHAN1_LOOP_CUR_THRESH_32      0x0001   /* Bit 8&0: x1-32mA */
#define SI32392_CHAN1_LOOP_CUR_THRESH_MASK    0x0101   /* Bit 8&0 Reg2 */

#define SI32392_CHAN0_ENHANCED_CONTROL_OFF    0x0000   /* Bit 10: 0FF */
#define SI32392_CHAN0_ENHANCED_CONTROL_ON     0x0400   /* Bit 10: ON */
#define SI32392_CHAN0_ENHANCED_CONTROL_MASK   0x0400   /* Bit 10 Reg2 */

#define SI32392_CHAN0_TEST_LOAD_OFF           0x0000   /* Bit 9: Off */
#define SI32392_CHAN0_TEST_LOAD_ON            0x0200   /* Bit 9: On */
#define SI32392_CHAN0_TEST_LOAD_MASK          0x0200   /* Bit 9 Reg2 */

#define SI32392_CHAN1_ENHANCED_CONTROL_OFF    0x0000   /* Bit 7: 0FF */
#define SI32392_CHAN1_ENHANCED_CONTROL_ON     0x0080   /* Bit 7: ON */
#define SI32392_CHAN1_ENHANCED_CONTROL_MASK   0x0080   /* Bit 7 Reg2 */

#define SI32392_CHAN1_TEST_LOAD_OFF           0x0000   /* Bit 6: Off */
#define SI32392_CHAN1_TEST_LOAD_ON            0x0040   /* Bit 6: On */
#define SI32392_CHAN1_TEST_LOAD_MASK          0x0040   /* Bit 6 Reg2 */

#define SI32392_RINGTRIP_DETECT_THRESH_60     0x0000   /* Bit 5: 0-60mA */
#define SI32392_RINGTRIP_DETECT_THRESH_70     0x0020   /* Bit 5: 1-70mA */
#define SI32392_RINGTRIP_DETECT_THRESH_MASK   0x0020   /* Bit 5 Reg2 */

#define SI32392_LOOP_DETECT_THRESH_10         0x0000   /* Bit 4: 0-10mA */
#define SI32392_LOOP_DETECT_THRESH_14         0x0010   /* Bit 4: 1-14mA */
#define SI32392_LOOP_DETECT_THRESH_MASK       0x0010   /* Bit 4 Reg2 */

#define SI32392_INTERRUPT_ENABLE              0x0000   /* Bit 3: 0-Disable */
#define SI32392_INTERRUPT_DISABLE             0x0008   /* Bit 3: 1-Enabale */
#define SI32392_INTERRUPT_DISABLE_MASK        0x0008   /* Bit 3 Reg2 */

#define SI32392_RSPOL_NORMAL                  0x0000   /* Bit 2: 0-Normal */
#define SI32392_RSPOL_REVERSED                0x0004   /* Bit 2: 1-Reverse */
#define SI32392_RSPOL_MASK                    0x0004   /* Bit 2 Reg2 */

/* Linefeed config 3 */
#define SI32392_TEST_MASK                     0x0F80
#define SI32392_TEST_NORMAL_OPERATION         0x0000
#define SI32392_TEST_TIP_VOLTAGE_SENSE        0x0900
#define SI32392_TEST_TIP_FV_SENSE             0x0980
#define SI32392_TEST_RING_VOLTAGE_SENSE       0x0A00
#define SI32392_TEST_RING_FV_SENSE            0x0A80
#define SI32392_TEST_LOOP_CURRENT_SENSE       0x0800
#define SI32392_TEST_REF_TIP_FV_SENSE         0x0180
#define SI32392_TEST_REF_RING_FV_SENSE        0x0280
#define SI32392_TEST_TRES                     0x0880
#define SI32392_TEST_REF                      0x0000
#define SI32392_TEST_IGN1                     0x0040
#define SI32392_TEST_IGN2                     0x0020
#define SI32392_TEST_TSDT                     0x0002

#define SI32392_TEST_CHAN_SHIFT               10

#ifdef __cplusplus
}
#endif

#endif /* SLIC_SI32392_DEFS_H */
