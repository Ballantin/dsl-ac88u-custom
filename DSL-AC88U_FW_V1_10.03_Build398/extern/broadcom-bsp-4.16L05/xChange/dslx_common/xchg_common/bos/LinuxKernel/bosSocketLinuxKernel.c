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
*****************************************************************************/
/**
*
*  @file    bosSocketLinuxUser.c
*
*  @brief   LinuxUser implementation of the BOS Socket Module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_SOCKET

#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosSocket.h>
#include <bosSocketPrivate.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
BOS_BOOL gBosSocketInitialized = BOS_FALSE;

/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup bosSocket
 * @{
 */


/***************************************************************************/

BOS_STATUS bosSocketInit( void )
{
   BOS_ASSERT( !gBosSocketInitialized );

   gBosSocketInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosSocketInit */

/***************************************************************************/

BOS_STATUS bosSocketTerm( void )
{
   BOS_ASSERT( gBosSocketInitialized );

   gBosSocketInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosSocketTerm */


#if BOS_CFG_TASK
/***************************************************************************/

BOS_STATUS bosSocketShare
(
   BOS_SOCKET    *srcSocket,
   BOS_TASK_ID   *dstTaskId,
   BOS_SOCKET    *dstSocket
)
{
   return BOS_STATUS_ERR;

} /* bosSocketShare */

/***************************************************************************/

BOS_STATUS bosSocketCloseShared
(
   BOS_SOCKET    *s
)
{
   return BOS_STATUS_ERR;

} /* bosSocketCloseShared */
#endif   /* BOS_CFG_TASK */


/***************************************************************************/

BOS_STATUS bosSocketCheckStatus( BOS_SOCKET_ERROR_TYPE err, BOS_SOCKET_FUNC_ID funcId )
{
   return bosErrorCheckStatusLinuxKernel( err,
                                      BOS_MAKE_ERROR_SOURCE( BOS_SOCKET_MODULE, funcId ));

} /* bosSocketCheckStatus */

/***************************************************************************/

BOS_STATUS bosSocketSetIpTos
(
   BOS_SOCKET *s,
   BOS_UINT8   tos
)
{
   return BOS_STATUS_ERR;

} /* bosSocketSetIpTos */


#if BOS_CFG_SOCKET_INTERFACE_SUPPORT
/***************************************************************************/

BOS_STATUS bosSocketSetInterface
(
   BOS_SOCKET *s,
   int         ifno
)
{
   return BOS_STATUS_ERR;

} /* bosSocketSetInterface */

/** @} */

#endif      /* BOS_CFG_SOCKET_INTERFACE_SUPPORT */

#endif      /* BOS_CFG_SOCKET */
