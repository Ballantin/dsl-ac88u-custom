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
/*   MODULE:  common63381.h                                             */
/*   DATE:    02/01/08                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM63381                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __COMMON_63381_H
#define __COMMON_63381_H

#ifdef __cplusplus
extern "C" {
#endif

#define BCM63381_REG_BASE                       0xb0000000
#define BCM63381_PERF_BASE            (BCM63381_REG_BASE + 0x00000000)  /* chip control */
#define BCM63381_TIMR_BASE            (BCM63381_REG_BASE + 0x000000c0)  /* timer registers */
#define BCM63381_GPIO_BASE            (BCM63381_REG_BASE + 0x00000180)  /* gpio registers */
#define BCM63381_MISC_BASE            (BCM63381_REG_BASE + 0x00000200)  /* Miscellaneous Registers */
#define BCM63381_UART_BASE            (BCM63381_REG_BASE + 0x00000280)  /* uart registers */
#define BCM63381_PCM_BASE             (BCM63381_REG_BASE + 0x00010c00)  /* PCM registers */
#define BCM63381_PCM_DMA_BASE         (BCM63381_REG_BASE + 0x00011000)  /* PCM DMA registers */




#ifdef __cplusplus
}
#endif

#endif /* __BCM63381_MAP_COMMON_H */
