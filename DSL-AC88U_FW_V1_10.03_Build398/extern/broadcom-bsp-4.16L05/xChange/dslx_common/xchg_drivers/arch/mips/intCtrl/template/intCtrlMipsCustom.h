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

#ifndef INT_CTRL_MIPS_CUSTOM_H
#define INT_CTRL_MIPS_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/*
** List of 3368 peripheral interrupt id's. These should be used to register user
** interrupt service routines via the xdrvIntCtrlInstallInterrupt() API.
*/
enum
{
   /* Peripheral interrupt id space must be unique. It must not overlap with
   ** the MIPS core interrupt id's. The standard MIPS has 8 interrupts
   ** (id's 0 - 7). So, we'll start at 8. */
   INT_CTRL_MIPS_PERIPH_START = 8,

   /* Do not change the order of interrupts. It must exactly match the bit
   ** ordering in the peripheral interrupt mask register. */
   INT_CTRL_MIPS_PERIPH_TIMER = INT_CTRL_MIPS_PERIPH_START,
   INT_CTRL_MIPS_PERIPH_SPI,
   INT_CTRL_MIPS_PERIPH_UART0,
   INT_CTRL_MIPS_PERIPH_UART1,
   INT_CTRL_MIPS_PERIPH_RESERVED,
   INT_CTRL_MIPS_PERIPH_US,
   INT_CTRL_MIPS_PERIPH_EMAC2,
   INT_CTRL_MIPS_PERIPH_USB,
   INT_CTRL_MIPS_PERIPH_ETH,
   INT_CTRL_MIPS_PERIPH_EPHY,
   INT_CTRL_MIPS_PERIPH_TC,
   INT_CTRL_MIPS_PERIPH_USB_CNTL_RX_DMA,
   INT_CTRL_MIPS_PERIPH_USB_CNTL_TX_DMA,
   INT_CTRL_MIPS_PERIPH_USB_BULK_RX_DMA,
   INT_CTRL_MIPS_PERIPH_USB_BULK_TX_DMA,
   INT_CTRL_MIPS_PERIPH_EMAC_RX_DMA,
   INT_CTRL_MIPS_PERIPH_EMAC_TX_DMA,
   INT_CTRL_MIPS_PERIPH_EMAC2_RX_DMA,
   INT_CTRL_MIPS_PERIPH_EMAC2_TX_DMA,
   INT_CTRL_MIPS_PERIPH_DS_AI,
   INT_CTRL_MIPS_PERIPH_DS1,
   INT_CTRL_MIPS_PERIPH_DS2,
   INT_CTRL_MIPS_PERIPH_APM,
   INT_CTRL_MIPS_PERIPH_APM_DMA_RX_A,
   INT_CTRL_MIPS_PERIPH_APM_DMA_TX_A,
   INT_CTRL_MIPS_PERIPH_APM_DMA_RX_B,
   INT_CTRL_MIPS_PERIPH_APM_DMA_TX_B,
   INT_CTRL_MIPS_PERIPH_APM_DMA_RX_PCM,
   INT_CTRL_MIPS_PERIPH_APM_DMA_TX_PCM,
   INT_CTRL_MIPS_PERIPH_HMAC_DATA,
   INT_CTRL_MIPS_PERIPH_HMAC_CNTL,
   INT_CTRL_MIPS_PERIPH_PCI,

   INT_CTRL_MIPS_PERIPH_END = INT_CTRL_MIPS_PERIPH_PCI
};


/*
** Indicates whether a specified interrupt id is a peripheral interrupt.
*/
#define INT_CTRL_MIPS_IS_PERIPH_ID( id )  \
            ( ((id) >= INT_CTRL_MIPS_PERIPH_START) && ((id) <= INT_CTRL_MIPS_PERIPH_END) )


/*
** Maps the peripheral interrupt id to a 0-indexed number.
*/
#define INT_CTRL_MIPS_PERIPH_ID_TO_INDEX( id )  ((id) - INT_CTRL_MIPS_PERIPH_START)


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* INT_CTRL_MIPS_CUSTOM_H  */
