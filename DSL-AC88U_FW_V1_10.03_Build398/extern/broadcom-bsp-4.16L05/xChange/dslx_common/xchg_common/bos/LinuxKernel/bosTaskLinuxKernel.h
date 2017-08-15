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
*  @file    bosTaskLinuxKernel.h
*
*  @brief   LinuxKernel specific definitions for the BOS Task module.
*
****************************************************************************/

#if !defined( BOSTASKLINUXKERNEL_H )
#define BOSTASKLINUXKERNEL_H        /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXKERNEL_H )
#  include "bosLinuxKernel.h"
#endif

/**
 * @addtogroup bosTask
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * This is the task ID which is used to uniquely identify a task within
 * the Task module. For LinuxKernel, the task ID is an index into the
 * @a gBosTaskEntry table.
 */

typedef int    BOS_TASK_ID;   /**< Task identifier                         */

/**
 * A value which is not considered to be an valid Task ID.
 */

#define  BOS_TASK_INVALID_ID  (-1)

/**
 * This is the native OS task priority type used for the mapping of
 * BOS_TASK_CLASS to native priorities in the @a gTaskPriorites table.
 */

typedef unsigned long      BOS_TASK_PRIORITY;   /** Native task priority */

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSTASKLINUXKERNEL_H */

