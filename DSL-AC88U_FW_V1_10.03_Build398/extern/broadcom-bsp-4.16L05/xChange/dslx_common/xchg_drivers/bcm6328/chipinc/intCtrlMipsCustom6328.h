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
*    Filename: intCtrlMipsCustom6328.h
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
*     This file provides custom interrupt information for the 6328
*
****************************************************************************/

#ifndef INT6328_CTRL_MIPS_CUSTOM_H
#define INT6328_CTRL_MIPS_CUSTOM_H

/*
** List of 6328 peripheral interrupt id's. These should be used to register user
** interrupt service routines via the xdrvIntCtrlInstallInterrupt() API.
*/
enum
{
   /* Peripheral interrupt id space must be unique. It must not overlap with
   ** the MIPS core interrupt id's. The standard MIPS has 8 interrupts
   ** (id's 0 - 7). So, we'll start at 8. */
   BCM6328_INT_CTRL_MIPS_PERIPH_START = 8,

   /* Do not change the order of interrupts. It must exactly match the bit
   ** ordering in the peripheral interrupt mask register. */
   
   BCM6328_INT_CTRL_MIPS_PERIPH_NAND_IRQ = BCM6328_INT_CTRL_MIPS_PERIPH_START,
   BCM6328_INT_CTRL_MIPS_PERIPH_PCM_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_PCM_DMA_IRQ0,
   BCM6328_INT_CTRL_MIPS_PERIPH_PCM_DMA_IRQ1,
   BCM6328_INT_CTRL_MIPS_PERIPH_USBD_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_USBD_DMA_IRQ0,
   BCM6328_INT_CTRL_MIPS_PERIPH_USBD_DMA_IRQ1,
   BCM6328_INT_CTRL_MIPS_PERIPH_USBD_DMA_IRQ2,
   BCM6328_INT_CTRL_MIPS_PERIPH_USBD_DMA_IRQ3,
   BCM6328_INT_CTRL_MIPS_PERIPH_USBD_DMA_IRQ4,
   BCM6328_INT_CTRL_MIPS_PERIPH_USBD_DMA_IRQ5,
   BCM6328_INT_CTRL_MIPS_PERIPH_DYING_GASP_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EPHY_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EPHY_ENERGY_0_N_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EPHY_ENERGY_1_N_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EPHY_ENERGY_2_N_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EPHY_ENERGY_3_N_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EPHY_ENERGY_0_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EPHY_ENERGY_1_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EPHY_ENERGY_2_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EPHY_ENERGY_3_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_ADSL_PHY_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_PCIE_EP_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_PCIE_RC_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EXTERNAL_0_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EXTERNAL_1_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EXTERNAL_2_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_EXTERNAL_3_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_UART0_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_HS_SPIM_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_WAKE_ON_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_TIMER_IRQ,
   BCM6328_INT_CTRL_MIPS_PERIPH_END = BCM6328_INT_CTRL_MIPS_PERIPH_TIMER_IRQ

   // TODO ADD ALL OTHER INTERRUPTS

};

#endif  /* INT6328_CTRL_MIPS_CUSTOM_H  */
