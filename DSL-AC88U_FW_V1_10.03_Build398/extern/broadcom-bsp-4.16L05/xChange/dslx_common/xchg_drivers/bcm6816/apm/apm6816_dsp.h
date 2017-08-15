/* THIS FILE NEEDS TO BE REMOVED */
#error THIS FILE NEEDS TO BE REMOVED
// /****************************************************************************
// *    Copyright © 2008 Broadcom
// *
// *    This program is the proprietary software of Broadcom and/or
// *    its licensors, and may only be used, duplicated, modified or
// *    distributed pursuant to the terms and conditions of a separate, written
// *    license agreement executed between you and Broadcom (an "Authorized
// *    License").  Except as set forth in an Authorized License, Broadcom
// *    grants no license (express or implied), right to use, or waiver of any
// *    kind with respect to the Software, and Broadcom expressly reserves all
// *    rights in and to the Software and all intellectual property rights
// *    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO
// *    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM
// *    AND DISCONTINUE ALL USE OF THE SOFTWARE.
// *
// *
// *    Except as expressly set forth in the Authorized License,
// *
// *    1.     This program, including its structure, sequence and
// *    organization, constitutes the valuable trade secrets of Broadcom, and
// *    you shall use all reasonable efforts to protect the confidentiality
// *    thereof, and to use this information only in connection with your use
// *    of Broadcom integrated circuit products.
// *
// *    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
// *    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
// *    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR
// *    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
// *    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
// *    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
// *    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
// *    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT
// *    OF USE OR PERFORMANCE OF THE SOFTWARE.
// *
// *    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
// *    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL,
// *    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
// *    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
// *    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
// *    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE
// *    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
// *    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
// *
// *****************************************************************************
// *
// *  apm6816_dsp.h - BCM6816 APM Definitions for the DSP only
// *
// *  PURPOSE:
// *
// *  NOTES:
// *
// ****************************************************************************/

// #ifndef APM6816_DSP_H
// #define APM6816_DSP_H

// #if defined(__cplusplus)
//   extern "C" {
// #endif

// /* ---- Include Files ----------------------------------------------------- */

// #include <xdrvTypes.h>
// #include <xdrvApm.h>
// #include <mipsDefines.h>


// /* ---- Public Constants and Types ---------------------------------------- */

// #define BCM6816_APM_CH_MIN          0
// #define BCM6816_APM_CH_MAX          1
// #define BCM6816_APM_CH_NUM          (BCM6816_APM_CH_MAX - BCM6816_APM_CH_MIN + 1)

// /* ---- Constants and Types ---------------------------------------------- */

// /*
// ** This likely doesn't belong here. It should go somewhere more generic, but
// ** I'm not sure where at the moment...
// */
// #define ALIGN_DATA(addr, boundary) ((addr + boundary - 1) & ~(boundary - 1))


// #define BCM6816_NUM_APM_CHANNELS         2  /* Number of channels supported by this device */
// #define BCM6816_CHANNEL_0_ONLY           0  /* APM Ch0 Rx */
// #define BCM6816_CHANNEL_1_ONLY           0  /* APM Ch0 Tx */

// #define BCM6816_DMA_RX_NUM_BUFFERS    2     /* Only need two buffers for double buffering */
// #define BCM6816_DMA_TX_NUM_BUFFERS    2     /* Only need two buffers for double buffering */

// #define APM6816_DMA_MAX_BUFFER_SIZE 48    /* This is in 32-bit words.
//                                       **  Allocate enaough for (5ms + 1ms prime) worth of 16kHz data:
//                                       **   1ms=16*16-bit=8 32-bit words
//                                       **  6ms=6*8*32-bit=48 32bit words */

// #define APM6816_DMA_MAX_BUFFER_SIZE_BYTES   (APM6816_DMA_MAX_BUFFER_SIZE*sizeof(XDRV_UINT32))



// #if defined(__cplusplus)
// }
// #endif

// #endif /* APM6816_DSP_H */
