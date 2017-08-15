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
*  @file    bosCritSectLinuxUser.h
*
*  @brief   LinuxUser specific definitions for the BOS Critical Section module.
*
****************************************************************************/

#if !defined( BOSCRITSECTLINUXUSER_H )
#define BOSCRITSECTLINUXUSER_H       /**< Include Guard                         */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXUSER_H )
#  include "bosLinuxUser.h"
#endif

#include <bosMutex.h>

#if !BOS_CFG_MUTEX
#error bos Critical section module requires Mutex module support (not present)
#endif

/**
 * @addtogroup bosCritSect
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * LinuxUser version of a BOS_CRITSECT.
 */

typedef BOS_MUTEX    BOS_CRITSECT;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSCRITSECTLINUXUSER_H */

