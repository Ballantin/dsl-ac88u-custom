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
*  @file    gpiol.h
*
*  @brief   Lower GPIO device driver header file
*
*  This file contains the lower device driver header information for the
*  GPIO module.
*/
/****************************************************************************/

#ifndef GPIOL_H
#define GPIOL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

/**
*  @brief   GPIO Direction
*
*  The keypad enumeration is used to describe the GPIO direction
*/
typedef enum
{
   GPIOL_DIR_INPUT = 0,    /* GPIO configured as input */
   GPIOL_DIR_OUTPUT        /* GPIO configured as output */
} GPIOL_DIR;

typedef struct
{
   XDRV_SINT32 (*gpiolNumFp)( void );
   XDRV_SINT32 (*gpiolSetDirFp)( XDRV_SINT32 gpioNumber, GPIOL_DIR direction );
   GPIOL_DIR   (*gpiolGetDirFp)( XDRV_SINT32 gpioNumber );
   XDRV_SINT32 (*gpiolSetValFp)( XDRV_SINT32 gpioNumber );
   XDRV_SINT32 (*gpiolClearValFp)( XDRV_SINT32 gpioNumber );
   XDRV_SINT32 (*gpiolToggleValFp)( XDRV_SINT32 gpioNumber );
   XDRV_SINT32 (*gpiolGetValFp)( XDRV_SINT32 gpioNumber );
   XDRV_UINT32 (*gpiolSetDirRegFp)( XDRV_UINT32 mask, GPIOL_DIR direction );
   XDRV_UINT32 (*gpiolGetDirRegFp)( void );
   XDRV_UINT32 (*gpiolSetValRegFp)( XDRV_UINT32 mask, XDRV_UINT32 val );
   XDRV_UINT32 (*gpiolToggleValRegFp)( XDRV_UINT32 mask );
   XDRV_UINT32 (*gpiolGetValRegFp)( void );
} GPIOL_FUNCS;


#define gpiolNum( pDrv )                   ( pDrv->pDrvFuncs->gpiolNumFp( ) )
#define gpiolSetDir( pDrv, gpioNumber, direction ) \
                                           ( pDrv->pDrvFuncs->gpiolSetDirFp( gpioNumber, direction ) )
#define gpiolGetDir( pDrv, gpioNumber )    ( pDrv->pDrvFuncs->gpiolGetDirFp( gpioNumber ) )
#define gpiolSetVal( pDrv, gpioNumber )    ( pDrv->pDrvFuncs->gpiolSetValFp( gpioNumber ) )
#define gpiolClearVal( pDrv, gpioNumber )  ( pDrv->pDrvFuncs->gpiolClearValFp( gpioNumber ) )
#define gpiolToggleVal( pDrv, gpioNumber ) ( pDrv->pDrvFuncs->gpiolToggleValFp( gpioNumber ) )
#define gpiolGetVal( pDrv, gpioNumber )    ( pDrv->pDrvFuncs->gpiolGetValFp( gpioNumber ) )
#define gpiolSetDirReg( pDrv, mask, direction ) \
                                           ( pDrv->pDrvFuncs->gpiolSetDirRegFp( mask, direction ) )
#define gpiolGetDirReg( pDrv )             ( pDrv->pDrvFuncs->gpiolGetDirRegFp( ) )
#define gpiolSetValReg( pDrv, mask, val )  ( pDrv->pDrvFuncs->gpiolSetValRegFp( mask, val ) )
#define gpiolToggleValReg( pDrv, mask )    ( pDrv->pDrvFuncs->gpiolToggleValRegFp( mask ) )
#define gpiolGetValReg( pDrv )             ( pDrv->pDrvFuncs->gpiolGetValRegFp( ) )

typedef struct
{
   /* This must be the first structure member. */
   const GPIOL_FUNCS *pDrvFuncs;

} GPIOL_DRV;

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
