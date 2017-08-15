/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: intCtrlMipsCustom.h
*
****************************************************************************
*    Description:
*
*     Chip specific interrupt control module definitions. This provides an
*     extension to the generic MIPS interrupt control module, which allows
*     users to register interrupt service routines for different types of
*     interrupts (external, peripheral, software, and timer).
*
*     Since each BRCM MIPS-based processor provides a different set of peripheral
*     interrupt sources, it is necessary to provide this custom list of
*     peripheral interrupts. This allows user to register chip specific interrupts
*     while sharing the same interrupt control implementation.
*
*     This file provides custom interrupt information for the 3368.
*
****************************************************************************/

#ifndef INT_CTRL_MIPS_CUSTOM_63381_H
#define INT_CTRL_MIPS_CUSTOM_63381_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/*=====================================================================*/
/* Peripheral ISR Table Offset                                              */
/*=====================================================================*/
#define BCM63381_INTERNAL_ISR_TABLE_OFFSET             8
#define BCM63381_INTERNAL_HIGH_ISR_TABLE_OFFSET        (BCM63381_INTERNAL_ISR_TABLE_OFFSET + 32)
#define BCM63381_INTERNAL_EXT_ISR_TABLE_OFFSET         (BCM63381_INTERNAL_HIGH_ISR_TABLE_OFFSET + 32)
#define BCM63381_INTERNAL_HIGH_EXT_ISR_TABLE_OFFSET    (BCM63381_INTERNAL_EXT_ISR_TABLE_OFFSET + 32)

/*
** List of 63381 peripheral interrupt id's. These should be used to register user
** interrupt service routines via the xdrvIntCtrlInstallInterrupt() API.
*/
enum
{
   /* Peripheral interrupt id space must be unique. It must not overlap with
   ** the MIPS core interrupt id's. The standard MIPS has 8 interrupts
   ** (id's 0 - 7). So, we'll start at 8. */
   BCM63381_INT_CTRL_MIPS_PERIPH_START=(BCM63381_INTERNAL_EXT_ISR_TABLE_OFFSET),
   BCM63381_INT_CTRL_ATM_DMA_0 = BCM63381_INT_CTRL_MIPS_PERIPH_START,
   BCM63381_INT_CTRL_ATM_DMA_1,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 1)  */
   BCM63381_INT_CTRL_ATM_DMA_2,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 2)  */
   BCM63381_INT_CTRL_ATM_DMA_3,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 3)  */
   BCM63381_INT_CTRL_ATM_DMA_4,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 4)  */
   BCM63381_INT_CTRL_ATM_DMA_5,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 5)  */
   BCM63381_INT_CTRL_ATM_DMA_6,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 6)  */
   BCM63381_INT_CTRL_ATM_DMA_7,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 7)  */
   BCM63381_INT_CTRL_ATM_DMA_8,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 8)  */
   BCM63381_INT_CTRL_ATM_DMA_9,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 9)  */
   BCM63381_INT_CTRL_ATM_DMA_10,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 10) */
   BCM63381_INT_CTRL_ATM_DMA_11,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 11) */
   BCM63381_INT_CTRL_ATM_DMA_12,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 12) */
   BCM63381_INT_CTRL_ATM_DMA_13,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 13) */
   BCM63381_INT_CTRL_ATM_DMA_14,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 14) */
   BCM63381_INT_CTRL_ATM_DMA_15,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 15) */
   BCM63381_INT_CTRL_ATM_DMA_16,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 16) */
   BCM63381_INT_CTRL_ATM_DMA_17,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 17) */
   BCM63381_INT_CTRL_ATM_DMA_18,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 18) */
   BCM63381_INT_CTRL_ATM_DMA_19,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 19) */
   BCM63381_INT_CTRL_PCM_DMA_0,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 20) */
   BCM63381_INT_CTRL_PCM_DMA_1,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 21) */
   BCM63381_INT_CTRL_USB_CNTL_RX_DMA,     /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 22) */
   BCM63381_INT_CTRL_USB_CNTL_TX_DMA,     /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 23) */
   BCM63381_INT_CTRL_USB_BULK_RX_DMA,     /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 24) */
   BCM63381_INT_CTRL_USB_BULK_TX_DMA,     /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 25) */
   BCM63381_INT_CTRL_USB_ISO_RX_DMA,      /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 26) */
   BCM63381_INT_CTRL_USB_ISO_TX_DMA,      /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 27) */
   BCM63381_INT_CTRL_MIPS_PERIPH_END = BCM63381_INT_CTRL_USB_ISO_TX_DMA,

   // TODO ADD ALL OTHER INTERRUPTS

};

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* INT_CTRL_MIPS_CUSTOM_63381_H  */
