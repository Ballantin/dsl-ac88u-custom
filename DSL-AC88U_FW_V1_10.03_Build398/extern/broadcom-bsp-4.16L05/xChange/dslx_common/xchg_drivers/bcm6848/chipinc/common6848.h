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
/*   MODULE:  common6848.h                                             */
/*   DATE:    02/01/08                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6848                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __COMMON_6848_H
#define __COMMON_6848_H


#ifdef __cplusplus
extern "C" {
#endif

#define BCM6848_REG_BASE             0xb0000000
#define BCM6848_PCM_BASE             (BCM6848_REG_BASE + 0x00010c00)  /* PCM registers */
#define BCM6848_PCM_DMA_BASE         (BCM6848_REG_BASE + 0x00011000)  /* PCM DMA registers */
#define BCM6848_PERF_BASE            (BCM6848_REG_BASE + 0x00800000)  /* chip control */
#define BCM6848_TIMR_BASE            (BCM6848_REG_BASE + 0x00800400)  /* timer registers */
#define BCM6848_GPIO_BASE            (BCM6848_REG_BASE + 0x00800500)  /* gpio registers */
#define BCM6848_MISC_BASE            (BCM6848_REG_BASE + 0x00802600)  /* Miscellaneous Registers */
#define BCM6848_UART_BASE            (BCM6848_REG_BASE + 0x00800640)  /* uart registers */



#ifdef __cplusplus
}
#endif

#endif /* __BCM6848_MAP_COMMON_H */
