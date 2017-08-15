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
*  @file    tscl.h
*
*  @brief   Lower touch screen controller device driver header file
*
*  This file contains the lower device driver header information for the
*  touch screen controller module.
*/
/****************************************************************************/

#ifndef TSCL_H
#define TSCL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

/**
*  @brief   Touchscreen mode
*
*  The touch screen enumeration is used to describe the current operating mode of 
*  the driver.
*/
typedef enum
{
   TSCL_MODE_4WIRE = 0,    /**< 4-Wire mode */
   TSCL_MODE_5WIRE         /**< 5-Wire mode */
} TSCL_MODE;


typedef struct
{
   XDRV_UINT32 (*tsclDisableFp)( void );
   XDRV_UINT32 (*tsclEnableFp)( void );
   XDRV_UINT32 (*tsclIsEnableFp)( void );
   XDRV_UINT32 (*tsclSampleRateGetFp)( void );
   XDRV_UINT32 (*tsclSampleRateSetFp)( XDRV_UINT32 sampleRate );
   XDRV_UINT32 (*tsclDebounceGetFp)( void );
   XDRV_UINT32 (*tsclDebounceSetFp)( XDRV_UINT32 timeout );
   XDRV_UINT32 (*tsclSettlingGetFp)( void );
   XDRV_UINT32 (*tsclSettlingSetFp)( XDRV_UINT32 timeout );
   XDRV_UINT32 (*tsclDataPointAverageGetFp)( void );
   XDRV_UINT32 (*tsclDataPointAverageSetFp)( XDRV_UINT32 average );
   XDRV_UINT32 (*tsclDataThresholdGetFp)( void );
   XDRV_UINT32 (*tsclDataThresholdSetFp)( XDRV_UINT32 threshold );
   TSCL_MODE   (*tsclModeGetFp)( void );
   TSCL_MODE   (*tsclModeSetFp)( TSCL_MODE mode );
   XDRV_UINT32 (*tsclReadFp)( char *bufp, XDRV_SINT32 len );
   XDRV_UINT32 (*tsclWriteFp)( char *bufp, XDRV_SINT32 len );
   XDRV_UINT32 (*tsclIsrDataFp)( void );
   XDRV_UINT32 (*tsclIrqDataEnableFp)( void );
   XDRV_UINT32 (*tsclIrqDataDisableFp)( void );
   XDRV_UINT32 (*tsclIsrPenFp)( void );
   XDRV_UINT32 (*tsclIrqPenEnableFp)( void );
   XDRV_UINT32 (*tsclIrqPenDisableFp)( void );
   XDRV_UINT32 (*tsclIsPenDownFp)( void );
   XDRV_UINT32 (*tsclIsDataAvailableFp)( void );
} TSCL_FUNCS;



#define tsclDisable( pDrv )                       ( pDrv->pDrvFuncs->tsclDisableFp( ) )
#define tsclEnable( pDrv )                        ( pDrv->pDrvFuncs->tsclEnableFp( ) )
#define tsclIsEnable( pDrv )                      ( pDrv->pDrvFuncs->tsclIsEnableFp( ) )
#define tsclSampleRateGet( pDrv )                 ( pDrv->pDrvFuncs->tsclSampleRateGetFp( ) )
#define tsclSampleRateSet( pDrv , sampleRate )    ( pDrv->pDrvFuncs->tsclSampleRateSetFp( sampleRate ) )
#define tsclDebounceGet( pDrv )                   ( pDrv->pDrvFuncs->tsclDebounceGetFp( ) )
#define tsclDebounceSet( pDrv, timeout)           ( pDrv->pDrvFuncs->tsclDebounceSetFp( timeout ) )
#define tsclSettlingGet( pDrv )                   ( pDrv->pDrvFuncs->tsclSettlingGetFp( ) )
#define tsclSettlingSet( pDrv, timeout )          ( pDrv->pDrvFuncs->tsclSettlingSetFp( timeout ) )
#define tsclDataPointAverageGet( pDrv )           ( pDrv->pDrvFuncs->tsclDataPointAverageGetFp( ) )
#define tsclDataPointAverageSet( pDrv, average )  ( pDrv->pDrvFuncs->tsclDataPointAverageSetFp( average ) )
#define tsclDataThresholdGet( pDrv )              ( pDrv->pDrvFuncs->tsclDataThresholdGetFp( ) )
#define tsclDataThresholdSet( pDrv, threshold )   ( pDrv->pDrvFuncs->tsclDataThresholdSetFp( threshold ) )
#define tsclModeGet( pDrv )                       ( pDrv->pDrvFuncs->tsclModeGetFp( ) )
#define tsclModeSet( pDrv, mode )                 ( pDrv->pDrvFuncs->tsclModeSetFp( mode ) )
#define tsclRead( pDrv, bufp, len )               ( pDrv->pDrvFuncs->tsclReadFp( bufp, len ) )
#define tsclWrite( pDrv, bufp, len )              ( pDrv->pDrvFuncs->tsclWriteFp( bufp, len ) )
#define tsclIsrData( pDrv )                       ( pDrv->pDrvFuncs->tsclIsrDataFp( ) )
#define tsclIrqDataEnable( pDrv )                 ( pDrv->pDrvFuncs->tsclIrqDataEnableFp( ) )
#define tsclIrqDataDisable( pDrv )                ( pDrv->pDrvFuncs->tsclIrqDataDisableFp( ) )
#define tsclIsrPen( pDrv )                        ( pDrv->pDrvFuncs->tsclIsrPenFp( ) )
#define tsclIrqPenEnable( pDrv )                  ( pDrv->pDrvFuncs->tsclIrqPenEnableFp( ) )
#define tsclIrqPenDisable( pDrv )                 ( pDrv->pDrvFuncs->tsclIrqPenDisableFp( ) )
#define tsclIsPenDown( pDrv )                     ( pDrv->pDrvFuncs->tsclIsPenDownFp( ) )
#define tsclIsDataAvailable( pDrv )               ( pDrv->pDrvFuncs->tsclIsDataAvailableFp( ) )

typedef struct
{
   /* This must be the first structure member. */
   const TSCL_FUNCS *pDrvFuncs;

} TSCL_DRV;


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
