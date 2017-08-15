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
/*   MODULE:  common63268.h                                             */
/*   DATE:    02/01/08                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM63268                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __COMMON_63268_H
#define __COMMON_63268_H

#ifdef __cplusplus
extern "C" {
#endif

#define BCM63268_PERF_BASE            0xb0000000  /* chip control registers */
#define BCM63268_TIMR_BASE            0xb0000080  /* timer registers */
#define BCM63268_GPIO_BASE            0xb00000c0  /* gpio registers */
#define BCM63268_UART_BASE            0xb0000180  /* uart registers */
#define BCM63268_MISC_BASE            0xb0001800  /* Miscellaneous Registers */
#define BCM63268_PCM_BASE             0xb000b000
#define BCM63268_PCM_DMA_BASE         0xb000b800  /* PCM IUDMA register base */
#define BCM63268_DECT_SHIM_CTRL_BASE         0xb000a000
#define BCM63268_DECT_SHIM_DMA_CTRL_BASE     0xb000a050
#define BCM63268_DECT_SHIM_TEST_BASE         0xb000a0f0
#define BCM63268_DECT_APB_REG_BASE           0xb000a800
#define BCM63268_DECT_AHB_SHARED_RAM_BASE    0xb06f0000
#define BCM63268_DECT_AHB_REG_BASE           0xb06f7f80


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
#define BCM63268_GPIO_SWREG_CONFIG       0x1c
#define BCM63268_GPIO_LIN_VREG_ADJ_SHIFT 0x0
#define BCM63268_GPIO_LIN_VREG_ADJ_MASK  (0xf<<BCM63268_GPIO_LIN_VREG_ADJ_SHIFT)
#define BCM63268_GPIO_SW_VREG_SEL_SHIFT	0x8
#define BCM63268_GPIO_SW_VREG_SEL_MASK	(0xf<<BCM63268_GPIO_SW_VREG_SEL_SHIFT)

/*
#####################################################################
# Miscellaneous Registers
#####################################################################
*/
#define BCM63268_MISC_MEMC_CONTROL		0x10
#define BCM63268_MISC_STRAP_BUS			0x14

/*
#####################################################################
# UART Control Registers
#####################################################################
*/
#define BCM63268_UART0CONTROL     0x01
#define BCM63268_UART0CONFIG      0x02
#define BCM63268_UART0RXTIMEOUT   0x03
#define BCM63268_UART0BAUD        0x04
#define BCM63268_UART0FIFOCFG     0x0a
#define BCM63268_UART0INTMASK     0x10
#define BCM63268_UART0INTSTAT     0x12
#define BCM63268_UART0DATA        0x17

#define BCM63268_BRGEN            0x80   /* Control register bit defs */
#define BCM63268_TXEN             0x40
#define BCM63268_RXEN             0x20
#define BCM63268_LOOPBK           0x10
#define BCM63268_TXPARITYEN       0x08
#define BCM63268_TXPARITYEVEN     0x04
#define BCM63268_RXPARITYEN       0x02
#define BCM63268_RXPARITYEVEN     0x01

#define BCM63268_XMITBREAK        0x40   /* Config register */
#define BCM63268_BITS5SYM         0x00
#define BCM63268_BITS6SYM         0x10
#define BCM63268_BITS7SYM         0x20
#define BCM63268_BITS8SYM         0x30
#define BCM63268_ONESTOP          0x07
#define BCM63268_TWOSTOP          0x0f

#define BCM63268_DELTAIP         0x0001
#define BCM63268_TXUNDERR        0x0002
#define BCM63268_TXOVFERR        0x0004
#define BCM63268_TXFIFOTHOLD     0x0008
#define BCM63268_TXREADLATCH     0x0010
#define BCM63268_TXFIFOEMT       0x0020
#define BCM63268_RXUNDERR        0x0040
#define BCM63268_RXOVFERR        0x0080
#define BCM63268_RXTIMEOUT       0x0100
#define BCM63268_RXFIFOFULL      0x0200
#define BCM63268_RXFIFOTHOLD     0x0400
#define BCM63268_RXFIFONE        0x0800
#define BCM63268_RXFRAMERR       0x1000
#define BCM63268_RXPARERR        0x2000
#define BCM63268_RXBRK           0x4000


/*
#####################################################################
# DECT IP Control Registers
#####################################################################
*/
#define BCM63268_DECT_STARTCTL      0xb06f0818
#define BCM63268_PCM_BUFF_CTL3      0xb06f082c
#define BCM63268_PCM_BUFF_CTL7      0xb06f083c
#define BCM63268_DECT_AHB_CHAN0_RX  0xb06f0a20
#define BCM63268_DECT_AHB_CHAN1_RX  0xb06f0de0
#define BCM63268_DECT_AHB_CHAN2_RX  0xb06f11a0
#define BCM63268_DECT_AHB_CHAN3_RX  0xb06f1560
#define BCM63268_DECT_AHB_CHAN0_TX  0xb06f0840
#define BCM63268_DECT_AHB_CHAN1_TX  0xb06f0c00
#define BCM63268_DECT_AHB_CHAN2_TX  0xb06f0fc0
#define BCM63268_DECT_AHB_CHAN3_TX  0xb06f1380
#define BCM63268_DECT_CLKEN         0x00000040   

#ifdef __cplusplus
}
#endif

#endif /* __BCM63268_MAP_COMMON_H */
