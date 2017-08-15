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
*  @file    bosSpinlockLinuxKernel.h
*
*  @brief   LinuxKernel specific definitions for the BOS Spinlock module.
*
****************************************************************************/

#if !defined( BOSSPINLOCKLINUXKERNEL_H )
#define BOSSPINLOCKLINUXKERNEL_H         /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXKERNEL_H )
#  include "bosLinuxKernel.h"
#endif

#include <linux/version.h>
#include <linux/spinlock.h>

/**
 * @addtogroup bosSpinlock
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * LinuxKernel version of a BOS_SPINLOCK.
 */
typedef struct 
{
   unsigned int owner;                /* Spinlock owner */
   unsigned int nesting;              /* Nested calls */
   spinlock_t lock;                   /* LINUXKERNEL spinlock type */
} BOS_SPINLOCK; 

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSSPINLOCKLINUXKERNEL_H */

