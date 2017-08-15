/****************************************************************************
*
*  Copyright (c) 2013 Broadcom
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
*  pcm63148.h - PCM highway Definitions
*
*  PURPOSE:
*
*  NOTES:
*
****************************************************************************/

#ifndef PCM63148_H
#define PCM63148_H

#if defined(__cplusplus)
  extern "C" {
#endif


#include <xdrvTypes.h>
#include <xchg_dma.h>
#include <armDefines.h>

#define ALIGN_DATA(addr, boundary) ((addr + boundary - 1) & ~(boundary - 1))

#define HAUSWARE_PERIOD_MS                   5

#ifndef PCM_ALAW
#   ifndef PCM_ULAW
/* PCM linear */
/* For 8Khz Fsync, we have 8 samples/ms/channel
 * 8 channels  available, then we have 8 * 8 = 64 samples/ms
 * If each sample is 16bits, then we have 64 * 16 = 1024 bits
 * We need buffer of 1024 bits = 128 bytes/ms
 * If buffer is UINT32, we need 128/4 = 32 element deep buffer/ms
*/
#   define PCM63148_DMA_MAX_BUFFER_SIZE          ( 32 * HAUSWARE_PERIOD_MS )
#   else
/* PCM ULAW */
/* For 8Khz Fsync, we have 8 samples/ms/channel
 * 8 channels  available, then we have 8 * 8 = 64 samples/ms
 * If each sample is 8bits, then we have 64 * 8 = 512 bits
 * We need buffer of 512 bits = 64  bytes/ms
 * If buffer is UINT32, we need 64/4 = 16  element deep buffer/ms
*/
#   define PCM63148_DMA_MAX_BUFFER_SIZE          ( 16 * HAUSWARE_PERIOD_MS )
#   endif
#else
/* PCM ALAW */
#define PCM63148_DMA_MAX_BUFFER_SIZE          ( 16 * HAUSWARE_PERIOD_MS )
#endif

#define PCM63148_DMA_MAX_BUFFER_SIZE_BYTES   (PCM63148_DMA_MAX_BUFFER_SIZE*sizeof(XDRV_UINT32))

typedef struct BCM63148_DMA_BUFFER
{
   /* Add padding so that we can align the DMA buffer on a cache-line boundary. */
   #define DMA_PADDING_WORD32    ( DCACHE_LINE_SIZE_BYTES / 4 )
   volatile XDRV_UINT32 dmaBuff[ PCM63148_DMA_MAX_BUFFER_SIZE + DMA_PADDING_WORD32 ];
} BCM63148_DMA_BUFFER;

/* Maximum number of 16-bit PCM channels that can be processed by the PCM module */
#define PCM_MAX_CHANNELS      8

#define PCM_CHAN_INVALID      0xFF
#define PCM_TIMESLOT_INVALID  0xFF

typedef struct PCM_CHANNEL_TIMESLOT_MAP
{
   int chan;
   int txTimeslot;
   int rxTimeslot;
} PCM_CHANNEL_TIMESLOT_MAP;

#define MODE_REG  0x0
#define MODE_ISI  0x1
#define MODE_ZSI  0x2
#define MODE_DECT 0x4

int pcm63148_init( PCM_CHANNEL_TIMESLOT_MAP* chanTsMap, XDRV_UINT32 channelMask, XDRV_UINT8 supportMode, BCM_IUDMA_ADDR_CTL* dmaCtl, XDRV_UINT32* dmaCount );
int pcm63148_deinit( void );

#define PCM63148_NUM_DMA_CHANNELS    1     /* Only one duplex RX/TX DMA channel */

#define DMA_RX_NUM_BUFFERS    2 //4     /* Only need two buffers for double buffering */
#define DMA_TX_NUM_BUFFERS    2 //8      /* Only need two buffers for double buffering */

#if defined(__cplusplus)
}
#endif

#endif /* PCM63148_H */
