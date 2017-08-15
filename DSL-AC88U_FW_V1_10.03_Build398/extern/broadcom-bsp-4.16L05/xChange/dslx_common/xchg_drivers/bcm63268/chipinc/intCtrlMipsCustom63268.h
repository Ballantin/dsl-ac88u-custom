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

#ifndef INT_CTRL_MIPS_CUSTOM_63268_H
#define INT_CTRL_MIPS_CUSTOM_63268_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/*
** List of 63268 peripheral interrupt id's. These should be used to register user
** interrupt service routines via the xdrvIntCtrlInstallInterrupt() API.
*/
enum
{
   /* Peripheral interrupt id space must be unique. It must not overlap with
   ** the MIPS core interrupt id's. The standard MIPS has 8 interrupts
   ** (id's 0 - 7). So, we'll start at 8. */
   BCM63268_INT_CTRL_MIPS_PERIPH_START = 8,

   /* Do not change the order of interrupts. It must exactly match the bit
   ** ordering in the peripheral interrupt mask register. */
   
   BCM63268_INT_CTRL_DECT_IRQ0 = BCM63268_INT_CTRL_MIPS_PERIPH_START,
   BCM63268_INT_CTRL_DECT_IRQ1,
   BCM63268_INT_CTRL_UART1,
   BCM63268_INT_CTRL_WLAN_GPIO,
   BCM63268_INT_CTRL_USB_CNTL_TX_DMA_TBD,
   BCM63268_INT_CTRL_USB_BULK_TX_DMA_TBD,
   BCM63268_INT_CTRL_USB_ISO_TX_DMA_TBD,
   BCM63268_INT_CTRL_IPSEC_DMA_1,
   BCM63268_INT_CTRL_PCIE_RC,
   BCM63268_INT_CTRL_PCIE_EP,
   BCM63268_INT_CTRL_MIPS_PERIPH_PCM_DMA_IRQ0,
   BCM63268_INT_CTRL_MIPS_PERIPH_PCM_DMA_IRQ1,
   BCM63268_INT_CTRL_EXTERNAL_IRQ0,
   BCM63268_INT_CTRL_EXTERNAL_IRQ1,
   BCM63268_INT_CTRL_EXTERNAL_IRQ2,
   BCM63268_INT_CTRL_EXTERNAL_IRQ3,
   BCM63268_INT_CTRL_MIPS_PERIPH_END = BCM63268_INT_CTRL_EXTERNAL_IRQ3

   // TODO ADD ALL OTHER INTERRUPTS

};

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* INT_CTRL_MIPS_CUSTOM_63268_H  */
