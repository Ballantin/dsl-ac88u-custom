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
*  @file    bosCritSectLinuxKernel.h
*
*  @brief   LinuxKernel specific definitions for the BOS Critical Section module.
*
****************************************************************************/

#if !defined( BOSCRITSECTLINUXKERNEL_H )
#define BOSCRITSECTLINUXKERNEL_H       /**< Include Guard                         */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXKERNEL_H )
#  include "bosLinuxKernel.h"
#endif

#include <bosMutex.h>

/**
 * @addtogroup bosCritSect
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * LinuxKernel version of a BOS_CRITSECT.
 */

typedef BOS_MUTEX* BOS_CRITSECT;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSCRITSECTLINUXKERNEL_H */

