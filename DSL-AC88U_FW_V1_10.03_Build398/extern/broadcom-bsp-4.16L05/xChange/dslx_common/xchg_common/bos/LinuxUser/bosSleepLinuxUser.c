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
*  @file    bosSleep.c
*
*  @brief   LinuxUser implementation of the BOS Sleep Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_SLEEP

#include <bosError.h>
#include <bosSleep.h>
#include <bosSleepPrivate.h>
#include <bosErrorLinuxUser.h>
#include <bosLinuxUser.h>

#include <unistd.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

static   BOS_BOOL gBosSleepInitialized = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/

BOS_STATUS bosSleepInit( void )
{
   BOS_ASSERT( !gBosSleepInitialized );

   bosSleepInitFunctionNames();

   gBosSleepInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosSleepInit */

/***************************************************************************/

BOS_STATUS bosSleepTerm( void )
{
   BOS_ASSERT( gBosSleepInitialized );

   gBosSleepInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosSleepTerm */

/***************************************************************************/

BOS_STATUS bosSleep( BOS_TIME_MS mSec )
{
   usleep( mSec * 1000 );
   return ( BOS_STATUS_OK );

} /* bosSleep */

/** @} */

#endif   /* BOS_CFG_SLEEP */

