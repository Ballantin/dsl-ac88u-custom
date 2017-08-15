/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    memDebug.h
*
*  @brief   Encapsulates the Microsoft Visual C++ heap debugging.
*
****************************************************************************/
/**
*  @defgroup   memDebug   Memory Debugging Facilities
*  @ingroup    mem
*
*  @brief   Adds some additional memory debugging facilities. Currently
*           only available for Win32 debug builds.
*
****************************************************************************/

#if !defined( MEMDEBUG_H )
#define MEMDEBUG_H             /**< Include Guard                          */

/* ---- Include Files ---------------------------------------------------- */

/**
 * @addtogroup memDebug
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * @def MEM_DEBUG_ON Determines if memory debugging support is enabled
 */

#if ( BOS_OS_WIN32 == 1 ) && defined( _DEBUG )

#  define  MEM_DEBUG_ON   1
#  if !defined( _INC_CRTDBG )
#     include <CRTDBG.H>
#  endif

#else
#  define  MEM_DEBUG_ON   0
#endif

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

#if MEM_DEBUG_ON

BOOL memDebugCheckHeap( void );
void memDebugInit( void );

#else

/**
 * Used to stub out the memDebugCheckHeap() function when debugging is not
 * being used.
 */
#define  memDebugCheckHeap() (TRUE)

/**
 * Used to stub out the memDebugInit() function when debugging is not
 * being used.
 */
#define  memDebugInit()

#endif   /* MEM_DEBUG_ON */

/** @} */

#endif /* MEMDEBUG_H */

