/****************************************************************************
*
* Copyright (c) 2002 Broadcom, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom
* company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosMutexPrivate.h
*
*  @brief   Private BOS Mutex definitions which are OS neutral.
*
*  This file contains constants, types, etc, which are private to BOS Mutex
*  but which are OS neutral.
*
****************************************************************************/

#if !defined( BOSMUTEXPRIVATE_H )
#define BOSMUTEXPRIVATE_H     /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/***************************************************************************/
/**
*  Initializes the gBosMutexFunctionName table. This function is called
*  from bosMutexInit().
*/

void bosMutexInitFunctionNames( void );

#endif /* BOSMUTEXPRIVATE_H */

