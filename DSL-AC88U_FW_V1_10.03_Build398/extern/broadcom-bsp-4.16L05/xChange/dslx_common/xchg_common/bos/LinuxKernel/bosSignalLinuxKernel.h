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

#if !defined( BOSSIGNALLINUXKERNEL_H )
#define BOSSIGNALLINUXKERNEL_H         /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXKERNEL_H )
#  include "bosLinuxKernel.h"
#endif

/**
 * @addtogroup bosSignal
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */
BOS_STATUS bosSignalMapSigToStatus( void );

/** @} */

#endif /* BOSSIGNALLINUXKERNEL_H */

