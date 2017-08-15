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
/*   MODULE:  common6368.h                                             */
/*   DATE:    01/05/10                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            6368 Chip for the DSP only.                              */
/*                                                                     */
/***********************************************************************/\
#ifndef __COMMON_6368_H
#define __COMMON_6368_H

#define BCM6368_PERF_BASE            0xb0000000  /* chip control registers */
#define BCM6368_GPIO_BASE            0xb0000080  /* gpio registers */
#define BCM6368_UART_BASE            0xb0000100  /* uart registers */
#define BCM6368_DDR_BASE             0xb0001280  /* DDR IO Buf Control registers */
#define BCM6368_PCM_BASE             0xb0004000  /* PCM control registers */
#define BCM6368_PCM_DMA_BASE         0xb0005800  /* PCM UIDMA register base */
/*
#####################################################################
# UART Control Registers
#####################################################################
*/
#define BCM6368_UART0CONTROL     0x01
#define BCM6368_UART0CONFIG      0x02
#define BCM6368_UART0RXTIMEOUT   0x03
#define BCM6368_UART0BAUD        0x04
#define BCM6368_UART0FIFOCFG     0x0a
#define BCM6368_UART0INTMASK     0x10
#define BCM6368_UART0INTSTAT     0x12
#define BCM6368_UART0DATA        0x17

#define BCM6368_BRGEN            0x80   /* Control register bit defs */
#define BCM6368_TXEN             0x40
#define BCM6368_RXEN             0x20
#define BCM6368_LOOPBK           0x10
#define BCM6368_TXPARITYEN       0x08
#define BCM6368_TXPARITYEVEN     0x04
#define BCM6368_RXPARITYEN       0x02
#define BCM6368_RXPARITYEVEN     0x01

#define BCM6368_XMITBREAK        0x40   /* Config register */
#define BCM6368_BITS5SYM         0x00
#define BCM6368_BITS6SYM         0x10
#define BCM6368_BITS7SYM         0x20
#define BCM6368_BITS8SYM         0x30
#define BCM6368_ONESTOP          0x07
#define BCM6368_TWOSTOP          0x0f

#define BCM6368_DELTAIP         0x0001
#define BCM6368_TXUNDERR        0x0002
#define BCM6368_TXOVFERR        0x0004
#define BCM6368_TXFIFOTHOLD     0x0008
#define BCM6368_TXREADLATCH     0x0010
#define BCM6368_TXFIFOEMT       0x0020
#define BCM6368_RXUNDERR        0x0040
#define BCM6368_RXOVFERR        0x0080
#define BCM6368_RXTIMEOUT       0x0100
#define BCM6368_RXFIFOFULL      0x0200
#define BCM6368_RXFIFOTHOLD     0x0400
#define BCM6368_RXFIFONE        0x0800
#define BCM6368_RXFRAMERR       0x1000
#define BCM6368_RXPARERR        0x2000
#define BCM6368_RXBRK           0x4000


#endif /* __BCM6368_MAP_COMMON_DSP_H */
