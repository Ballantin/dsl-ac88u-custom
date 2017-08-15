/****************************************************************************
*
*     Copyright (c) 2003 Broadcom
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
*  @file    knl_framesync.h
*
*  @brief   Global definitions for kernel that application need
*
****************************************************************************/
#if !defined( KNL_FRAMESYNC_H )
#define KNL_FRAMESYNC_H

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/****************************************************************************
* Call this to run all the hausware threads that have become ready as a
* result of the periodic tick to hausware.
*/
void KNL_RunReadyThreads( void );


/****************************************************************************
* Call this to run all the hausware threads asynchronously.  Do not use
* when using sync calls.
*/
void KNL_RunReadyThreadsAsync( void );

/** @} */

#endif /* KNL_FRAMESYNC_H */

