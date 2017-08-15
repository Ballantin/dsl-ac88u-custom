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
*  @brief   LinuxKernel specific definitions for the BOS Semaphore module.
*
****************************************************************************/

#if !defined( BOSSEMLINUXKERNEL_H )
#define BOSSEMLINUXKERNEL_H         /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXKERNEL_H )
#  include "bosLinuxKernel.h"
#endif

#include <linux/version.h>
#include <linux/semaphore.h>

/**
 * @addtogroup bosSem
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * LinuxKernel version of a BOS_SEM.
 */

typedef struct semaphore* BOS_SEM;    /* LINUXKERNEL semaphore type */

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSSEMLINUXKERNEL_H */

