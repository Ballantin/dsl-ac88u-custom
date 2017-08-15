/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16251 Laguna Canyon Road
*           Irvine, California  92618
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosSemLinuxKernel.h
*
*  @brief   LinuxKernel specific definitions for the BOS timer module.
*
****************************************************************************/

#if !defined( BOSTIMERLINUXKERNEL_H )
#define BOSTIMERLINUXKERNEL_H         /**< Include Guard */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXKERNEL_H )
#  include "bosLinuxKernel.h"
#endif

#include <linux/timer.h>

/**
 * @addtogroup bosSem
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * LinuxKernel version of a BOS_TIMER.
 */

typedef struct timer_list BOS_TIMER;    /* LINUXKERNEL timer type */

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSTIMERLINUXKERNEL_H */

