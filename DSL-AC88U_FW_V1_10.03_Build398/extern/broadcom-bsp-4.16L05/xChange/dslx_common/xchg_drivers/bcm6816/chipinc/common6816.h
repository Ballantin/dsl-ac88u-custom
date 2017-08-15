/*
<:copyright-broadcom 
 
 Copyright (c) 2007 Broadcom 
 All Rights Reserved 
 No portions of this material may be reproduced in any form without the 
 written permission of: 
          Broadcom 
          16215 Alton Parkway 
          Irvine, California 92619 
 All information contained in this document is Broadcom 
 company private, proprietary, and trade secret. 
 
:>
*/
/***********************************************************************/
/*                                                                     */
/*   MODULE:  common6816.h                                             */
/*   DATE:    02/01/08                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6816                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __COMMON_6816_H
#define __COMMON_6816_H

#ifdef __cplusplus
extern "C" {
#endif

#define BCM6816_PERF_BASE            0xb0000000  /* chip control registers */
#define BCM6816_GPIO_BASE            0xb0000080  /* gpio registers */
#define BCM6816_UART_BASE            0xb0000100  /* uart registers */
#define BCM6816_MISC_BASE            0xb0001800
#define BCM6816_APM_BASE             0xb0008000
#define BCM6816_PCM_BASE             0xb0008200
#define BCM6816_APM_HVG_BASE         0xb0008300
#define BCM6816_APM_IUDMA_BASE       0xb0008800
#define BCM6816_APM_MEM_BASE         0xb0010000

/*
#####################################################################
# GPIO Control Registers
#####################################################################
*/
#define BCM6816_GPIO_SWREG_CONFIG       0x1c
#define BCM6816_GPIO_LIN_VREG_ADJ_SHIFT 0x0
#define BCM6816_GPIO_LIN_VREG_ADJ_MASK  (0xf<<BCM6816_GPIO_LIN_VREG_ADJ_SHIFT)
#define BCM6816_GPIO_SW_VREG_SEL_SHIFT	0x8
#define BCM6816_GPIO_SW_VREG_SEL_MASK	(0xf<<BCM6816_GPIO_SW_VREG_SEL_SHIFT)

/*
#####################################################################
# Miscellaneous Registers
#####################################################################
*/
#define BCM6816_MISC_MEMC_CONTROL		0x10
#define BCM6816_MISC_STRAP_BUS			0x14

/*
#####################################################################
# UART Control Registers
#####################################################################
*/
/* TBD. Verify these definitions on BCM6816 */
#define BCM6816_UART0CONTROL     0x01
#define BCM6816_UART0CONFIG      0x02
#define BCM6816_UART0RXTIMEOUT   0x03
#define BCM6816_UART0BAUD        0x04
#define BCM6816_UART0FIFOCFG     0x0a
#define BCM6816_UART0INTMASK     0x10
#define BCM6816_UART0INTSTAT     0x12
#define BCM6816_UART0DATA        0x17

#define BCM6816_BRGEN            0x80   /* Control register bit defs */
#define BCM6816_TXEN             0x40
#define BCM6816_RXEN             0x20
#define BCM6816_LOOPBK           0x10
#define BCM6816_TXPARITYEN       0x08
#define BCM6816_TXPARITYEVEN     0x04
#define BCM6816_RXPARITYEN       0x02
#define BCM6816_RXPARITYEVEN     0x01

#define BCM6816_XMITBREAK        0x40   /* Config register */
#define BCM6816_BITS5SYM         0x00
#define BCM6816_BITS6SYM         0x10
#define BCM6816_BITS7SYM         0x20
#define BCM6816_BITS8SYM         0x30
#define BCM6816_ONESTOP          0x07
#define BCM6816_TWOSTOP          0x0f

#define BCM6816_DELTAIP         0x0001
#define BCM6816_TXUNDERR        0x0002
#define BCM6816_TXOVFERR        0x0004
#define BCM6816_TXFIFOTHOLD     0x0008
#define BCM6816_TXREADLATCH     0x0010
#define BCM6816_TXFIFOEMT       0x0020
#define BCM6816_RXUNDERR        0x0040
#define BCM6816_RXOVFERR        0x0080
#define BCM6816_RXTIMEOUT       0x0100
#define BCM6816_RXFIFOFULL      0x0200
#define BCM6816_RXFIFOTHOLD     0x0400
#define BCM6816_RXFIFONE        0x0800
#define BCM6816_RXFRAMERR       0x1000
#define BCM6816_RXPARERR        0x2000
#define BCM6816_RXBRK           0x4000

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_6816_H */
