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
*  @file    bosErrorLinuxKernel.h
*
*  @brief   LinuxKernel specific Error definitions.
*
****************************************************************************/

#if !defined( BOSERRORLINUXKERNEL_H )
#define BOSERRORLINUXKERNEL_H       /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXKERNEL_H )
#  include <bosLinuxKernel.h>
#endif

/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

BOS_STATUS bosErrorCheckStatusLinuxKernel( int err, BOS_ERROR_SOURCE errSrc );

#endif /* BOSERRORLINUXKERNEL_H */

