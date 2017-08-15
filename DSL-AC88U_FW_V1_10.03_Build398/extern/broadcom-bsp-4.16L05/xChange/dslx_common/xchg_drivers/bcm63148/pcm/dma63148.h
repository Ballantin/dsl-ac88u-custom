/****************************************************************************
*
*     Copyright (c) 2013 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
****************************************************************************
*
*  PURPOSE:
*
*     This file contains global definitions for the DMA controllers.
*
*  NOTES:
*
****************************************************************************/

#ifndef DMA63148_H
#define DMA63148_H

#ifdef __cplusplus
  extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/*
** PCM / IUDMA Channel associations
*/
#define BCM63148_DMA_UBUS_BYTES           0


/* Wrap common types to chip-specific types using defines */
#define BCM63148_IUDMA_DESC                BCM_IUDMA_DESC
#define BCM63148_IUDMA_DESC_STATUS_OWN     BCM_IUDMA_DESC_STATUS_OWN
#define BCM63148_IUDMA_DESC_STATUS_EOP     BCM_IUDMA_DESC_STATUS_EOP
#define BCM63148_IUDMA_DESC_STATUS_SOP     BCM_IUDMA_DESC_STATUS_SOP
#define BCM63148_IUDMA_DESC_STATUS_WRAP    BCM_IUDMA_DESC_STATUS_WRAP
#define BCM63148_IUDMA_DESC_STATUS_PERIPH  BCM_IUDMA_DESC_STATUS_PERIPH


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* DMA63148_H */
