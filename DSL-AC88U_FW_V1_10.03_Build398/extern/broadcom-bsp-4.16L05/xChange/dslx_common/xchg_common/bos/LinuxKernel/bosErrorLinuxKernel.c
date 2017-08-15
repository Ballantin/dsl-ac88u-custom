/****************************************************************************
*
*     Copyright (c) 2001 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16251 Laguna Canyon Road
*           Irvine, California  92618
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  @file    bosErrorLinuxKernel.c
*
*  @brief   LinuxKernel implementation of the BOS Error module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <linux/string.h>
#include <bosLog.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup bosErrorLinuxKernel
 * @{
 */

/***************************************************************************/
/**
*  Translates native LinuxKernel error codes into an appropriate BOS status code.
*
*  @param   err         (in) The LinuxKernel error code
*  @param   errSrc      (in) Module/Function that detected the error.
*
*  @return  One of the BOS_STATUS enumerated values.
*/

BOS_STATUS bosErrorCheckStatusLinuxKernel( int err, BOS_ERROR_SOURCE errSrc )
{
   if ( err != 0 )
   {
      char  errSrcStr[ 40 ];

      bosErrorFunctionStr( errSrc, errSrcStr, sizeof( errSrcStr ));

      blogStrErr(( gBosBlog, "returned by %s: %u", errSrcStr, err ));
      return ( BOS_STATUS_ERR );
   }

   return ( BOS_STATUS_OK );

} /* bosErrorCheckStatusLinuxKernel */

/** @} */

