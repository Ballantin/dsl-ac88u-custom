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
*  @file    wdl.h
*
*  @brief   Lower watchdog device driver header file
*
*  This file contains the lower device driver header information for the
*  watchdog module.
*/
/****************************************************************************/

#ifndef WDL_H
#define WDL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */


typedef struct
{
   XDRV_SINT32 (*wdlEnableFp)( void );
   XDRV_SINT32 (*wdlDisableFp)( void );
   XDRV_UINT32 (*wdlIntervalSetFp)( XDRV_UINT32 msec );
   XDRV_UINT32 (*wdlIntervalGetFp)( void );
   XDRV_UINT32 (*wdlResetLengthSetFp)( XDRV_UINT32 msec );
   XDRV_UINT32 (*wdlResetLengthGetFp)( void );
   void (*wdlPreResetFp)( void );
} WDL_FUNCS;



#define wdlEnable( pDrv )                  ( pDrv->pDrvFuncs->wdlEnableFp( ) )
#define wdlDisable( pDrv )                 ( pDrv->pDrvFuncs->wdlDisableFp( ) )
#define wdlIntervalSet( pDrv, msec )       ( pDrv->pDrvFuncs->wdlIntervalSetFp( msec ) )
#define wdlIntervalGet( pDrv )             ( pDrv->pDrvFuncs->wdlIntervalGetFp( ) )
#define wdlResetLengthSet( pDrv, msec )    ( pDrv->pDrvFuncs->wdlResetLengthSetFp( msec ) )
#define wdlResetLengthGet( pDrv )          ( pDrv->pDrvFuncs->wdlResetLengthGetFp( ) )
#define wdlPreReset( pDrv )                ( pDrv->pDrvFuncs->wdlPreResetFp( ) )

typedef struct
{
   /* This must be the first structure member. */
   const WDL_FUNCS *pDrvFuncs;

} WDL_DRV;

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
