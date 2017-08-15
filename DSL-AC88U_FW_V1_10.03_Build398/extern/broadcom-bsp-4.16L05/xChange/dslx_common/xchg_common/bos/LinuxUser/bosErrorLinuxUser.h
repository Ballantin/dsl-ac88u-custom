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
*  @file    bosErrorLinuxUser.h
*
*  @brief   LinuxUser specific Error definitions.
*
****************************************************************************/

#if !defined( BOSERRORLINUXUSER_H )
#define BOSERRORLINUXUSER_H       /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXUSER_H )
#  include <bosLinuxUser.h>
#endif

/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

BOS_STATUS bosErrorCheckStatusLinuxUser( int err, BOS_ERROR_SOURCE errSrc );

#endif /* BOSERRORLINUXUSER_H */

