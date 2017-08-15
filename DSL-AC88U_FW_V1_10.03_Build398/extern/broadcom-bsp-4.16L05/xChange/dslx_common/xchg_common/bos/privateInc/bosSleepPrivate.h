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
*  @file    bosSleepPrivate.h
*
*  @brief   Private BOS Sleep definitions which are OS neutral.
*
*  This file contains constants, types, etc, which are private to BOS Sleep
*  but which are OS neutral.
*
****************************************************************************/

#if !defined( BOSSLEEPPRIVATE_H )
#define BOSSLEEPPRIVATE_H     /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/***************************************************************************/
/**
*  Initializes the gBosSleepFunctionName table. This function is called
*  from bosSleepInit().
*/

void bosSleepInitFunctionNames( void );

#endif /* BOSSLEEPPRIVATE_H */

