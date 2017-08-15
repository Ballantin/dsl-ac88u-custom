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

#ifndef INT_CTRL_MIPS_CUSTOM_6848_H
#define INT_CTRL_MIPS_CUSTOM_6848_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/*=====================================================================*/
/* Peripheral ISR Table Offset                                              */
/*=====================================================================*/
#define BCM6848_INTERNAL_ISR_TABLE_OFFSET             8
#define BCM6848_INTERNAL_HIGH_ISR_TABLE_OFFSET        (BCM6848_INTERNAL_ISR_TABLE_OFFSET + 32)
#define BCM6848_INTERNAL_EXT_ISR_TABLE_OFFSET         (BCM6848_INTERNAL_HIGH_ISR_TABLE_OFFSET + 32)
#define BCM6848_INTERNAL_HIGH_EXT_ISR_TABLE_OFFSET    (BCM6848_INTERNAL_EXT_ISR_TABLE_OFFSET + 32)

#if 1
enum
{
   /* Peripheral interrupt id space must be unique. It must not overlap with
   ** the MIPS core interrupt id's. The standard MIPS has 8 interrupts
   ** (id's 0 - 7). So, we'll start at 8. */
   BCM6848_INT_CTRL_MIPS_PERIPH_START=(BCM6848_INTERNAL_ISR_TABLE_OFFSET),
   BCM6848_INT_CTRL_PCM_DMA_0 = BCM6848_INT_CTRL_MIPS_PERIPH_START + 14,
   BCM6848_INT_CTRL_PCM_DMA_1 = BCM6848_INT_CTRL_MIPS_PERIPH_START + 15,
   BCM6848_INT_CTRL_MIPS_PERIPH_END = BCM6848_INT_CTRL_MIPS_PERIPH_START + 16, 
};
#else
/*
** List of 6848 peripheral interrupt id's. These should be used to register user
** interrupt service routines via the xdrvIntCtrlInstallInterrupt() API.
*/
enum
{
   /* Peripheral interrupt id space must be unique. It must not overlap with
   ** the MIPS core interrupt id's. The standard MIPS has 8 interrupts
   ** (id's 0 - 7). So, we'll start at 8. */
   BCM6848_INT_CTRL_MIPS_PERIPH_START=(BCM6848_INTERNAL_EXT_ISR_TABLE_OFFSET),
   BCM6848_INT_CTRL_ATM_DMA_0 = BCM6848_INT_CTRL_MIPS_PERIPH_START,
   BCM6848_INT_CTRL_ATM_DMA_1,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 1)  */
   BCM6848_INT_CTRL_ATM_DMA_2,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 2)  */
   BCM6848_INT_CTRL_ATM_DMA_3,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 3)  */
   BCM6848_INT_CTRL_ATM_DMA_4,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 4)  */
   BCM6848_INT_CTRL_ATM_DMA_5,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 5)  */
   BCM6848_INT_CTRL_ATM_DMA_6,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 6)  */
   BCM6848_INT_CTRL_ATM_DMA_7,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 7)  */
   BCM6848_INT_CTRL_ATM_DMA_8,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 8)  */
   BCM6848_INT_CTRL_ATM_DMA_9,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 9)  */
   BCM6848_INT_CTRL_ATM_DMA_10,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 10) */
   BCM6848_INT_CTRL_ATM_DMA_11,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 11) */
   BCM6848_INT_CTRL_ATM_DMA_12,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 12) */
   BCM6848_INT_CTRL_ATM_DMA_13,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 13) */
   BCM6848_INT_CTRL_ATM_DMA_14,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 14) */
   BCM6848_INT_CTRL_ATM_DMA_15,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 15) */
   BCM6848_INT_CTRL_ATM_DMA_16,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 16) */
   BCM6848_INT_CTRL_ATM_DMA_17,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 17) */
   BCM6848_INT_CTRL_ATM_DMA_18,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 18) */
   BCM6848_INT_CTRL_ATM_DMA_19,          /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 19) */
   BCM6848_INT_CTRL_PCM_DMA_0,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 20) */
   BCM6848_INT_CTRL_PCM_DMA_1,           /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 21) */
   BCM6848_INT_CTRL_USB_CNTL_RX_DMA,     /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 22) */
   BCM6848_INT_CTRL_USB_CNTL_TX_DMA,     /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 23) */
   BCM6848_INT_CTRL_USB_BULK_RX_DMA,     /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 24) */
   BCM6848_INT_CTRL_USB_BULK_TX_DMA,     /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 25) */
   BCM6848_INT_CTRL_USB_ISO_RX_DMA,      /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 26) */
   BCM6848_INT_CTRL_USB_ISO_TX_DMA,      /* (INTERNAL_EXT_ISR_TABLE_OFFSET + 27) */
   BCM6848_INT_CTRL_MIPS_PERIPH_END = BCM6848_INT_CTRL_USB_ISO_TX_DMA,

   // TODO ADD ALL OTHER INTERRUPTS

};
#endif
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* INT_CTRL_MIPS_CUSTOM_6848_H  */
