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
*  @file    bosMsgQLinuxKernel.h
*
*  @brief   LinuxKernel specific definitions for the BOS Message Queue module.
*
****************************************************************************/

#if !defined( BOSMSGQLINUXKERNEL_H )
#define BOSMSGQLINUXKERNEL_H       /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXKERNEL_H )
#  include "bosLinuxKernel.h"
#endif

/**
 * @addtogroup bosMSGQ
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * LinuxKernel version of a BOS_MSGQ.
 */

typedef unsigned long   BOS_MSGQ;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSMSGQLINUXKERNEL_H */

