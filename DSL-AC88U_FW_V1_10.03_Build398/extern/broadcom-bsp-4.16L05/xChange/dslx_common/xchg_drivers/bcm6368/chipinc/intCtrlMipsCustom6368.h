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
*    Filename: intCtrlMipsCustom6368.h
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

#ifndef INT6368_CTRL_MIPS_CUSTOM_H
#define INT6368_CTRL_MIPS_CUSTOM_H

/*
** List of 6368 peripheral interrupt id's. These should be used to register user
** interrupt service routines via the xdrvIntCtrlInstallInterrupt() API.
*/
enum
{
   /* Peripheral interrupt id space must be unique. It must not overlap with
   ** the MIPS core interrupt id's. The standard MIPS has 8 interrupts
   ** (id's 0 - 7). So, we'll start at 8. */
   BCM6368_INT_CTRL_MIPS_PERIPH_START = 8,

   /* Do not change the order of interrupts. It must exactly match the bit
   ** ordering in the peripheral interrupt mask register. */
   
   BCM6368_INT_CTRL_MIPS_PERIPH_SWITCH_RX_DMA_IRQ0 = BCM6368_INT_CTRL_MIPS_PERIPH_START,
   BCM6368_INT_CTRL_MIPS_PERIPH_SWITCH_RX_DMA_IRQ1,
   BCM6368_INT_CTRL_MIPS_PERIPH_SWITCH_RX_DMA_IRQ2,
   BCM6368_INT_CTRL_MIPS_PERIPH_SWITCH_RX_DMA_IRQ3,
   BCM6368_INT_CTRL_MIPS_PERIPH_SWITCH_TX_DMA_IRQ0,
   BCM6368_INT_CTRL_MIPS_PERIPH_SWITCH_TX_DMA_IRQ1,
   BCM6368_INT_CTRL_MIPS_PERIPH_SWITCH_TX_DMA_IRQ2,
   BCM6368_INT_CTRL_MIPS_PERIPH_SWITCH_TX_DMA_IRQ3,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ0,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ1,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ2,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ3,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ4,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ5,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ6,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ7,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ8,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ9,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ10,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ11,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ12,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ13,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ14,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ15,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ16,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ17,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ18,
   BCM6368_INT_CTRL_MIPS_PERIPH_SAR_DMA_IRQ19,
   BCM6368_INT_CTRL_MIPS_PERIPH_IPSEC_DMA_IRQ0,
   BCM6368_INT_CTRL_MIPS_PERIPH_IPSEC_DMA_IRQ1,
   BCM6368_INT_CTRL_MIPS_PERIPH_PCM_DMA_IRQ0,
   BCM6368_INT_CTRL_MIPS_PERIPH_PCM_DMA_IRQ1,
   BCM6368_INT_CTRL_MIPS_PERIPH_END = BCM6368_INT_CTRL_MIPS_PERIPH_PCM_DMA_IRQ1

   // TODO ADD ALL OTHER INTERRUPTS

};

#endif  /* INT6368_CTRL_MIPS_CUSTOM_H  */
