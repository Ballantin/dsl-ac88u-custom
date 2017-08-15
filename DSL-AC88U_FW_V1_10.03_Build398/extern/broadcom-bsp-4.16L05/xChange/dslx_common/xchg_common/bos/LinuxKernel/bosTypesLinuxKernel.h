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
*  @file    bosTypesLinuxKernel.h
*
*  @brief   LinuxKernel Specific BOS Types.
*
****************************************************************************/

#if !defined( BOSTYPESLINUXKERNEL_H )
#define BOSTYPESLINUXKERNEL_H           /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/**
 * Type which holds a timeout, measured in milliseconds.
 */

typedef unsigned int BOS_TIME_MS;
typedef unsigned int BOS_TIME_NS;

/**
 * Definition of infinite time constant
 */

#define BOS_WAIT_FOREVER  ((BOS_TIME_MS)(-1))

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

#endif /* BOSTYPESLINUXKERNEL_H */

