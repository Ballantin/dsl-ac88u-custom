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
/*   MODULE:  common6328.h                                             */
/*   DATE:    01/05/10                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            6328 Chip for the DSP only.                              */
/*                                                                     */
/***********************************************************************/\
#ifndef __COMMON_6328_H
#define __COMMON_6328_H

#define BCM6328_PERF_BASE            0xb0000000  /* chip control registers */
#define BCM6328_GPIO_BASE            0xb0000080  /* gpio registers */
#define BCM6328_UART_BASE            0xb0000100  /* uart registers */
#define BCM6328_DDR_BASE             0xb0003000  /* DDR IO Buf Control registers */
#define BCM6328_MISC_BASE            0xb0001800  /* Miscellaneous Registers */
#define BCM6328_PCM_BASE             0xb000a000  /* PCM control registers */
#define BCM6328_PCM_DMA_BASE         0xb000a800  /* PCM UIDMA register base */
/*
#####################################################################
# UART Control Registers
#####################################################################
*/
#define BCM6328_UART0CONTROL     0x01
#define BCM6328_UART0CONFIG      0x02
#define BCM6328_UART0RXTIMEOUT   0x03
#define BCM6328_UART0BAUD        0x04
#define BCM6328_UART0FIFOCFG     0x0a
#define BCM6328_UART0INTMASK     0x10
#define BCM6328_UART0INTSTAT     0x12
#define BCM6328_UART0DATA        0x17

#define BCM6328_BRGEN            0x80   /* Control register bit defs */
#define BCM6328_TXEN             0x40
#define BCM6328_RXEN             0x20
#define BCM6328_LOOPBK           0x10
#define BCM6328_TXPARITYEN       0x08
#define BCM6328_TXPARITYEVEN     0x04
#define BCM6328_RXPARITYEN       0x02
#define BCM6328_RXPARITYEVEN     0x01

#define BCM6328_XMITBREAK        0x40   /* Config register */
#define BCM6328_BITS5SYM         0x00
#define BCM6328_BITS6SYM         0x10
#define BCM6328_BITS7SYM         0x20
#define BCM6328_BITS8SYM         0x30
#define BCM6328_ONESTOP          0x07
#define BCM6328_TWOSTOP          0x0f

#define BCM6328_DELTAIP         0x0001
#define BCM6328_TXUNDERR        0x0002
#define BCM6328_TXOVFERR        0x0004
#define BCM6328_TXFIFOTHOLD     0x0008
#define BCM6328_TXREADLATCH     0x0010
#define BCM6328_TXFIFOEMT       0x0020
#define BCM6328_RXUNDERR        0x0040
#define BCM6328_RXOVFERR        0x0080
#define BCM6328_RXTIMEOUT       0x0100
#define BCM6328_RXFIFOFULL      0x0200
#define BCM6328_RXFIFOTHOLD     0x0400
#define BCM6328_RXFIFONE        0x0800
#define BCM6328_RXFRAMERR       0x1000
#define BCM6328_RXPARERR        0x2000
#define BCM6328_RXBRK           0x4000 


#endif /* __BCM6328_MAP_COMMON_DSP_H */
