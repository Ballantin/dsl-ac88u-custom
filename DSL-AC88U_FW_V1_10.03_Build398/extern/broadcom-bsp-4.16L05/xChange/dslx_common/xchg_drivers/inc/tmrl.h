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
*  @file    tmrl.h
*
*  @brief   Lower timer device driver header file
*
*  This file contains the lower device driver header information for the
*  timer module.
*/
/****************************************************************************/

#ifndef TMRL_H
#define TMRL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

/**
*  @enum     TMRL_MODE_PERIODIC
*  The counter is cleared, and then starts again.  Clearing the interrupt
*  does not actually reset the timer value.  Therefore, if there is a delay
*  in interrupt processing, the interrupt itself is asserted with fixed period.
*/
/**
*  @enum     TMRL_MODE_APERIODIC
*  Clearing the interrupt results in the counter also being cleared.  This
*  results in a fixed delay between interrupt clear steps
*/

/**
*  @brief   Timer mode
*
*  The timer enumeration is used to describe the current operating mode of 
*  the driver.
*/
typedef enum
{
   TMRL_MODE_PERIODIC = 0,   /**< Periodic Mode */
   TMRL_MODE_APERIODIC       /**< Aperiodic Mode */
} TMRL_MODE;

typedef struct
{
   XDRV_SINT32 (*tmrlEnableFp)( XDRV_SINT32 timerNum );
   XDRV_SINT32 (*tmrlDisableFp)( XDRV_SINT32 timerNum );
   XDRV_SINT32 (*tmrlIsEnableFp)( XDRV_SINT32 timerNum );
   XDRV_SINT32 (*tmrlIrqEnableFp)( XDRV_SINT32 timerNum );
   XDRV_SINT32 (*tmrlIrqDisableFp)( XDRV_SINT32 timerNum );
   XDRV_SINT32 (*tmrlIsIrqEnableFp)( XDRV_SINT32 timerNum );
   XDRV_SINT32 (*tmrlIsInterruptActiveFp)( XDRV_SINT32 timerNum );
   XDRV_SINT32 (*tmrlIsWdInterruptActiveFp)( void );
   XDRV_SINT32 (*tmrlInterruptClearFp)( XDRV_SINT32 timerNum );
   TMRL_MODE   (*tmrlModeSetFp)( XDRV_SINT32 timerNum, TMRL_MODE mode );
   TMRL_MODE   (*tmrlModeGetFp)( XDRV_SINT32 timerNum );
   XDRV_UINT32 (*tmrlIntervalSetFp)( XDRV_SINT32 timerNum, XDRV_UINT32 usec );
   XDRV_UINT32 (*tmrlIntervalGetFp)( XDRV_SINT32 timerNum );
   XDRV_UINT32 (*tmrlCurrentCountSetFp)( XDRV_SINT32 timerNum, XDRV_UINT32 usec );
   XDRV_UINT32 (*tmrlCurrentCountGetFp)( XDRV_SINT32 timerNum );
} TMRL_FUNCS;

#define tmrlEnable( pDrv, timerNum )             ( pDrv->pDrvFuncs->tmrlEnableFp( timerNum ) )
#define tmrlDisable( pDrv, timerNum )            ( pDrv->pDrvFuncs->tmrlDisableFp( timerNum ) )
#define tmrlIsEnable( pDrv, timerNum )           ( pDrv->pDrvFuncs->tmrlIsEnableFp( timerNum ) )
#define tmrlIrqEnable( pDrv, timerNum )          ( pDrv->pDrvFuncs->tmrlIrqEnableFp( timerNum ) )
#define tmrlIrqDisable( pDrv, timerNum )         ( pDrv->pDrvFuncs->tmrlIrqDisableFp( timerNum ) )
#define tmrlIsIrqEnable( pDrv, timerNum )        ( pDrv->pDrvFuncs->tmrlIsIrqEnableFp( timerNum ) )
#define tmrlIsInterruptActive( pDrv, timerNum )  ( pDrv->pDrvFuncs->tmrlIsInterruptActiveFp( timerNum ) )
#define tmrlIsWdInterruptActive( pDrv )          ( pDrv->pDrvFuncs->tmrlIsWdInterruptActiveFp( ) )
#define tmrlInterruptClear( pDrv, timerNum )     ( pDrv->pDrvFuncs->tmrlInterruptClearFp( timerNum ) )
#define tmrlModeSet( pDrv, timerNum, mode )      ( pDrv->pDrvFuncs->tmrlModeSetFp( timerNum, mode ) )
#define tmrlModeGet( pDrv, timerNum )            ( pDrv->pDrvFuncs->tmrlModeGetFp( timerNum ) )
#define tmrlIntervalSet( pDrv, timerNum, usec )  ( pDrv->pDrvFuncs->tmrlIntervalSetFp( timerNum, usec ) )
#define tmrlIntervalGet( pDrv, timerNum )        ( pDrv->pDrvFuncs->tmrlIntervalGetFp( timerNum ) )
#define tmrlCurrentCountSet( pDrv, timerNum, usec ) \
                                                 ( pDrv->pDrvFuncs->tmrlCurrentCountSetFp( timerNum, usec ) )
#define tmrlCurrentCountGet( pDrv, timerNum )    ( pDrv->pDrvFuncs->tmrlCurrentCountGetFp( timerNum ) )

typedef struct
{
   /* This must be the first structure member. */
   const TMRL_FUNCS *pDrvFuncs;

} TMRL_DRV;

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
