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
*  @file    bosSemLinuxUser.h
*
*  @brief   LinuxUser specific definitions for the BOS Semaphore module.
*
****************************************************************************/

#if !defined( BOSSEMLINUXUSER_H )
#define BOSSEMLINUXUSER_H         /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXUSER_H )
#  include "bosLinuxUser.h"
#endif

#include <semaphore.h>

/**
 * @addtogroup bosSem
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * LinuxUser version of a BOS_SEM.
 */

typedef sem_t BOS_SEM;       /* LinuxUser semaphone ID type */


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSSEMLINUXUSER_H */

