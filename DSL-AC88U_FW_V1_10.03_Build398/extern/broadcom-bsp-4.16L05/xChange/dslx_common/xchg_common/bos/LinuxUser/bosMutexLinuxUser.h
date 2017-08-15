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
*  @file    bosMutexLinuxUser.h
*
*  @brief   LinuxUser specific definitions for the BOS Mutex module.
*
****************************************************************************/

#if !defined( BOSMUTEXLINUXUSER_H )
#define BOSMUTEXLINUXUSER_H       /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXUSER_H )
#  include "bosLinuxUser.h"
#endif

#include <pthread.h>

/**
 * @addtogroup bosMutex
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * LinuxUser version of a BOS_MUTEX.
 */

typedef pthread_mutex_t*    BOS_MUTEX;    /* LinuxUser mutex type */

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSMUTEXLINUXUSER_H */

