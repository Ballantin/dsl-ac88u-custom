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
/*   MODULE:  common6362.h                                             */
/*   DATE:    02/01/08                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6362                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __COMMON_6362_H
#define __COMMON_6362_H

#ifdef __cplusplus
extern "C" {
#endif

#define BCM6362_PERF_BASE            0xb0000000  /* chip control registers */
#define BCM6362_TIMR_BASE            0xb0000040  /* timer registers */
#define BCM6362_GPIO_BASE            0xb0000080  /* gpio registers */
#define BCM6362_UART_BASE            0xb0000100  /* uart registers */
#define BCM6362_MISC_BASE            0xb0001800  /* Miscellaneous Registers */
#define BCM6362_PCM_BASE             0xb000a000
#define BCM6362_PCM_DMA_BASE         0xb000a800  /* PCM IUDMA register base */
#define BCM6362_DECT_SHIM_CTRL_BASE         0xb000b000
#define BCM6362_DECT_SHIM_DMA_CTRL_BASE     0xb000b050
#define BCM6362_DECT_SHIM_TEST_BASE         0xb000b0f0
#define BCM6362_DECT_APB_REG_BASE           0xb000e000
#define BCM6362_DECT_AHB_SHARED_RAM_BASE    0xb0e50000
#define BCM6362_DECT_AHB_REG_BASE           0xb0e57f80


/*
#####################################################################
# System PLL Control Register
#####################################################################
*/

/*
#####################################################################
# GPIO Control Registers
#####################################################################
*/
#define BCM6362_GPIO_SWREG_CONFIG       0x1c
#define BCM6362_GPIO_LIN_VREG_ADJ_SHIFT 0x0
#define BCM6362_GPIO_LIN_VREG_ADJ_MASK  (0xf<<BCM6362_GPIO_LIN_VREG_ADJ_SHIFT)
#define BCM6362_GPIO_SW_VREG_SEL_SHIFT	0x8
#define BCM6362_GPIO_SW_VREG_SEL_MASK	(0xf<<BCM6362_GPIO_SW_VREG_SEL_SHIFT)

/*
#####################################################################
# Miscellaneous Registers
#####################################################################
*/
#define BCM6362_MISC_MEMC_CONTROL		0x10
#define BCM6362_MISC_STRAP_BUS			0x14

/*
#####################################################################
# UART Control Registers
#####################################################################
*/
#define BCM6362_UART0CONTROL     0x01
#define BCM6362_UART0CONFIG      0x02
#define BCM6362_UART0RXTIMEOUT   0x03
#define BCM6362_UART0BAUD        0x04
#define BCM6362_UART0FIFOCFG     0x0a
#define BCM6362_UART0INTMASK     0x10
#define BCM6362_UART0INTSTAT     0x12
#define BCM6362_UART0DATA        0x17

#define BCM6362_BRGEN            0x80   /* Control register bit defs */
#define BCM6362_TXEN             0x40
#define BCM6362_RXEN             0x20
#define BCM6362_LOOPBK           0x10
#define BCM6362_TXPARITYEN       0x08
#define BCM6362_TXPARITYEVEN     0x04
#define BCM6362_RXPARITYEN       0x02
#define BCM6362_RXPARITYEVEN     0x01

#define BCM6362_XMITBREAK        0x40   /* Config register */
#define BCM6362_BITS5SYM         0x00
#define BCM6362_BITS6SYM         0x10
#define BCM6362_BITS7SYM         0x20
#define BCM6362_BITS8SYM         0x30
#define BCM6362_ONESTOP          0x07
#define BCM6362_TWOSTOP          0x0f

#define BCM6362_DELTAIP         0x0001
#define BCM6362_TXUNDERR        0x0002
#define BCM6362_TXOVFERR        0x0004
#define BCM6362_TXFIFOTHOLD     0x0008
#define BCM6362_TXREADLATCH     0x0010
#define BCM6362_TXFIFOEMT       0x0020
#define BCM6362_RXUNDERR        0x0040
#define BCM6362_RXOVFERR        0x0080
#define BCM6362_RXTIMEOUT       0x0100
#define BCM6362_RXFIFOFULL      0x0200
#define BCM6362_RXFIFOTHOLD     0x0400
#define BCM6362_RXFIFONE        0x0800
#define BCM6362_RXFRAMERR       0x1000
#define BCM6362_RXPARERR        0x2000
#define BCM6362_RXBRK           0x4000


/*
#####################################################################
# DECT IP Control Registers
#####################################################################
*/
#define BCM6362_DECT_STARTCTL      0xb0e50818
#define BCM6362_PCM_BUFF_CTL3      0xb0e5082c
#define BCM6362_PCM_BUFF_CTL7      0xb0e5083c
#define BCM6362_DECT_AHB_CHAN0_RX  0xb0e50a20
#define BCM6362_DECT_AHB_CHAN1_RX  0xb0e50de0
#define BCM6362_DECT_AHB_CHAN2_RX  0xb0e511a0
#define BCM6362_DECT_AHB_CHAN3_RX  0xb0e51560
#define BCM6362_DECT_AHB_CHAN0_TX  0xb0e50840
#define BCM6362_DECT_AHB_CHAN1_TX  0xb0e50c00
#define BCM6362_DECT_AHB_CHAN2_TX  0xb0e50fc0
#define BCM6362_DECT_AHB_CHAN3_TX  0xb0e51380
#define BCM6362_DECT_CLKEN         0x00000040   

#ifdef __cplusplus
}
#endif

#endif /* __BCM6362_MAP_COMMON_H */
