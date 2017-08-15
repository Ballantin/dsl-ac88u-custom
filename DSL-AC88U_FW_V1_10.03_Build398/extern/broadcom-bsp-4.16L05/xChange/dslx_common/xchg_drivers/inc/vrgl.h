/*****************************************************************************
*
*  Copyright (c) 2004 Broadcom, All Rights Reserved.
*  Contains proprietary and confidential information.
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom
*        16215 Alton Parkway
*        P.O. Box 57013
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom
*  company private, proprietary, and trade secret.
*
*****************************************************************************/

/****************************************************************************/
/**
*  @file    vrgl.h
*
*  @brief   Lower voltage regulator device driver header file
*
*  This file contains the lower device driver header information for the
*  voltage regulator module.
*/
/****************************************************************************/

#ifndef VRGL_H
#define VRGL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

/**
*  @brief   Voltage regulator controller
*
*  The voltage regulator controller is used to specify the module supplying
*  a particular voltage.
*/
typedef enum
{
   VRGL_ID_2_5 = 0,      /**< 2.5V regulator */
   VRGL_ID_1_2           /**< 1.2V regulator */
} VRGL_ID;


typedef struct
{
   XDRV_SINT32 (*vrglAdjGetFp)( VRGL_ID regulatorId );
   XDRV_SINT32 (*vrglAdjSetFp)( VRGL_ID regulatorId, XDRV_SINT32 percent );
} VRGL_FUNCS;



#define vrglAdjGet( pDrv, regulatorId )            ( pDrv->pDrvFuncs->vrglAdjGetFp( regulatorId ) )
#define vrglAdjSet( pDrv, regulatorId, percent )   ( pDrv->pDrvFuncs->vrglAdjSetFp( regulatorId, percent ) )

typedef struct
{
   /* This must be the first structure member. */
   const VRGL_FUNCS *pDrvFuncs;

} VRGL_DRV;

#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Public Variables -------------------------------------------------- */


/* ---- Public Function Prototypes ---------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif
