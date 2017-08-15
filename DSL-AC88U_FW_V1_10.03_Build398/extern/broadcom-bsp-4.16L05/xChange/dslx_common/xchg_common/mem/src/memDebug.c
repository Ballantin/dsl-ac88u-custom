/****************************************************************************
*
*     Copyright (c) 2001 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    memDebug.c 
*
*  @brief   Encapsulates the Microsoft Visual C++ heap debugging.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosTypes.h>
#include <memDebug.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

#if MEM_DEBUG_ON

/**
 * @addtogroup memDebug
 * @{
 */

/***************************************************************************/
/**
*  Checks the heap for validity.
*
*  @return  BOS_TRUE if the heap appears to be valid, BOS_FALSE otherwise.
*/

BOOL memDebugCheckHeap( void )
{
   return _CrtCheckMemory();

} /* MEM_DebugCheckHeap */

/***************************************************************************/
/**
*  This function initializes the MEM_Debug module.
*/

void memDebugInit( void )
{
   int newFlags	= _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

   newFlags |= _CRTDBG_CHECK_ALWAYS_DF;

   _CrtSetDbgFlag( newFlags );

} /* MEM_DebugInit */

/** @} */

#endif   /* MEM_DEBUG_ON */

