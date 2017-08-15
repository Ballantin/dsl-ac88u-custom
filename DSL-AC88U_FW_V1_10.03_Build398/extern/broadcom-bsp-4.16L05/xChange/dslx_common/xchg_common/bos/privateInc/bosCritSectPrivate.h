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
*  @file    bosCritSectPrivate.h
*
*  @brief   Private BOS CritSect definitions which are OS neutral.
*
*  This file contains constants, types, etc, which are private to BOS CritSect
*  but which are OS neutral.
*
****************************************************************************/

#if !defined( BOSCRITSECTPRIVATE_H )
#define BOSCRITSECTPRIVATE_H     /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/***************************************************************************/
/**
*  Initializes the gBosCritSectFunctionName table. This function is called
*  from bosCritSectInit().
*/

void bosCritSectInitFunctionNames( void );

#endif /* BOSCRITSECTPRIVATE_H */

