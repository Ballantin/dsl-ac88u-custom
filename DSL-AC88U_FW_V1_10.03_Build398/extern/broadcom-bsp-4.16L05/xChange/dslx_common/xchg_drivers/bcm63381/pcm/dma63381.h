/****************************************************************************
*
* Copyright © 2008 Broadcom
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
*    Filename: dma63381.h
*
****************************************************************************
*
*  Description: Global definitions for the BCM63381 DMA controllers.
*
****************************************************************************/

#ifndef DMA63381_H
#define DMA63381_H

#ifdef __cplusplus
  extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
#include <xchg_dma.h>
/* ---- Constants and Types ---------------------------------------------- */

#define BCM63381_DMA_UBUS_BYTES               8  /* 63381 Ubus is 64bits/8ytes wide */

#define BCM63381_IUDMA_PCM_RX      BCM_IUDMA_PCM_RX
#define BCM63381_IUDMA_PCM_TX      BCM_IUDMA_PCM_TX

#define BCM63381_IUDMA_DESC                  BCM_IUDMA_DESC
#define BCM63381_IUDMA_DESC_STATUS_OWN       BCM_IUDMA_DESC_STATUS_OWN
#define BCM63381_IUDMA_DESC_STATUS_EOP       BCM_IUDMA_DESC_STATUS_EOP
#define BCM63381_IUDMA_DESC_STATUS_SOP       BCM_IUDMA_DESC_STATUS_SOP
#define BCM63381_IUDMA_DESC_STATUS_WRAP      BCM_IUDMA_DESC_STATUS_WRAP
#define BCM63381_IUDMA_DESC_STATUS_PERIPH    BCM_IUDMA_DESC_STATUS_PERIPH


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* DMA63381_H */
