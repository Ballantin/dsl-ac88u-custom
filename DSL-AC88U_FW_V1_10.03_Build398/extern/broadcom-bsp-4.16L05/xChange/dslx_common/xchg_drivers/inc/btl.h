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
*  @file    btl.h
*
*  @brief   Lower BlueTooth UART device driver header file
*
*  This file contains the lower device driver header information for the
*  BlueTooth UART module.
*/
/****************************************************************************/

#ifndef BTL_H
#define BTL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
   XDRV_SINT32 (*btlBaudGetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBaudSetFp)( XDRV_SINT32 ch, XDRV_SINT32 baud );
   XDRV_SINT32 (*btlBitsGetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBitsSetFp)( XDRV_SINT32 ch, XDRV_SINT32 bits );
   XDRV_SINT32 (*btlChValidFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlCtsFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlDtrDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlDtrEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlDtrIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlFlowIrqDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlFlowIrqEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlFlowIrqIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlIsrTxFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlIsrRxFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlParityDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlParityEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlParityEvenDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlParityEvenEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlParityEvenIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlParityIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlReadFp)( XDRV_SINT32 ch, char *datap );
   XDRV_SINT32 (*btlRtsDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlRtsEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlRtsIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlRxDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlRxEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlRxIrqDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlRxIrqEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlRxIrqIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlRxIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlRxResetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlStopBitsGetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlStopBitsSetFp)( XDRV_SINT32 ch, XDRV_SINT32 halfBits );
   XDRV_SINT32 (*btlTxDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlTxEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlTxIrqDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlTxIrqEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlTxIrqIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlTxIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlTxResetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlWriteFp)( XDRV_SINT32 ch, char data );

   XDRV_SINT32 (*btlIrdaDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlIrdaEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlIrdaIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBaudDetDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBaudDetEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBaudDetIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBaudDetGetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBaudRtModDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBaudRtModEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBaudRtModIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBaudRtModGetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*btlBaudRtModSetFp)( XDRV_SINT32 ch, int adj0, int adj1 );
} BTL_FUNCS;


#ifdef UARTL_H
/* uartl.h contains the same macro definitions for accessing the lower layer
*  functions.  Since the function mapping structure BTL_FUNS matches 
*  UARTL_FUNCS, it is possible to share the same macros between the UART and
*  Bluetooth drivers.
*/
#else
#define uartlBaudGet( pDrv, ch )               ( pDrv->pDrvFuncs->btlBaudGetFp( ch ) )
#define uartlBaudSet( pDrv, ch, baud )         ( pDrv->pDrvFuncs->btlBaudSetFp( ch, baud ) )
#define uartlBitsGet( pDrv, ch )               ( pDrv->pDrvFuncs->btlBitsGetFp( ch ) )
#define uartlBitsSet( pDrv, ch, bits )         ( pDrv->pDrvFuncs->btlBitsSetFp( ch, bits ) )
#define uartlChValid( pDrv, ch )               ( pDrv->pDrvFuncs->btlChValidFp( ch ) )
#define uartlCts( pDrv, ch )                   ( pDrv->pDrvFuncs->btlCtsFp( ch ) )
#define uartlDtrDisable( pDrv, ch )            ( pDrv->pDrvFuncs->btlDtrDisableFp( ch ) )
#define uartlDtrEnable( pDrv, ch )             ( pDrv->pDrvFuncs->btlDtrEnableFp( ch ) )
#define uartlDtrIsEnabled( pDrv, ch )          ( pDrv->pDrvFuncs->btlDtrIsEnabledFp( ch ) )
#define uartlFlowIrqDisable( pDrv, ch )        ( pDrv->pDrvFuncs->btlFlowIrqDisableFp( ch ) )
#define uartlFlowIrqEnable( pDrv, ch )         ( pDrv->pDrvFuncs->btlFlowIrqEnableFp( ch ) )
#define uartlFlowIrqIsEnabled( pDrv, ch )      ( pDrv->pDrvFuncs->btlFlowIrqIsEnabledFp( ch ) )
#define uartlIsrTx( pDrv, ch )                 ( pDrv->pDrvFuncs->btlIsrTxFp( ch ) )
#define uartlIsrRx( pDrv, ch )                 ( pDrv->pDrvFuncs->btlIsrRxFp( ch ) )
#define uartlParityDisable( pDrv, ch )         ( pDrv->pDrvFuncs->btlParityDisableFp( ch ) )
#define uartlParityEnable( pDrv, ch )          ( pDrv->pDrvFuncs->btlParityEnableFp( ch ) )
#define uartlParityEvenDisable( pDrv, ch )     ( pDrv->pDrvFuncs->btlParityEvenDisableFp( ch ) )
#define uartlParityEvenEnable( pDrv, ch )      ( pDrv->pDrvFuncs->btlParityEvenEnableFp( ch ) )
#define uartlParityEvenIsEnabled( pDrv, ch )   ( pDrv->pDrvFuncs->btlParityEvenIsEnabledFp( ch ) )
#define uartlParityIsEnabled( pDrv, ch )       ( pDrv->pDrvFuncs->btlParityIsEnabledFp( ch ) )
#define uartlRead( pDrv, ch, datap )           ( pDrv->pDrvFuncs->btlReadFp( ch, datap ) )
#define uartlRtsDisable( pDrv, ch )            ( pDrv->pDrvFuncs->btlRtsDisableFp( ch ) )
#define uartlRtsEnable( pDrv, ch )             ( pDrv->pDrvFuncs->btlRtsEnableFp( ch ) )
#define uartlRtsIsEnabled( pDrv, ch )          ( pDrv->pDrvFuncs->btlRtsIsEnabledFp( ch ) )
#define uartlRxDisable( pDrv, ch )             ( pDrv->pDrvFuncs->btlRxDisableFp( ch ) )
#define uartlRxEnable( pDrv, ch )              ( pDrv->pDrvFuncs->btlRxEnableFp( ch ) )
#define uartlRxIrqDisable( pDrv, ch )          ( pDrv->pDrvFuncs->btlRxIrqDisableFp( ch ) )
#define uartlRxIrqEnable( pDrv, ch )           ( pDrv->pDrvFuncs->btlRxIrqEnableFp( ch ) )
#define uartlRxIrqIsEnabled( pDrv, ch )        ( pDrv->pDrvFuncs->btlRxIrqIsEnabledFp( ch ) )
#define uartlRxIsEnabled( pDrv, ch )           ( pDrv->pDrvFuncs->btlRxIsEnabledFp( ch ) )
#define uartlRxReset( pDrv, ch )               ( pDrv->pDrvFuncs->btlRxResetFp( ch ) )
#define uartlStopBitsGet( pDrv, ch )           ( pDrv->pDrvFuncs->btlStopBitsGetFp( ch ) )
#define uartlStopBitsSet( pDrv, ch, halfBits ) ( pDrv->pDrvFuncs->btlStopBitsSetFp( ch, halfBits ) )
#define uartlTxDisable( pDrv, ch )             ( pDrv->pDrvFuncs->btlTxDisableFp( ch ) )
#define uartlTxEnable( pDrv, ch )              ( pDrv->pDrvFuncs->btlTxEnableFp( ch ) )
#define uartlTxIrqDisable( pDrv, ch )          ( pDrv->pDrvFuncs->btlTxIrqDisableFp( ch ) )
#define uartlTxIrqEnable( pDrv, ch )           ( pDrv->pDrvFuncs->btlTxIrqEnableFp( ch ) )
#define uartlTxIrqIsEnabled( pDrv, ch )        ( pDrv->pDrvFuncs->btlTxIrqIsEnabledFp( ch ) )
#define uartlTxIsEnabled( pDrv, ch )           ( pDrv->pDrvFuncs->btlTxIsEnabledFp( ch ) )
#define uartlTxReset( pDrv, ch )               ( pDrv->pDrvFuncs->btlTxResetFp( ch ) )
#define uartlWrite( pDrv, ch, data )           ( pDrv->pDrvFuncs->btlWriteFp( ch, data ) )

#define uartlIrdaDisable( pDrv, ch )           ( pDrv->pDrvFuncs->btlIrdaDisableFp( ch ) )
#define uartlIrdaEnable( pDrv, ch )            ( pDrv->pDrvFuncs->btlIrdaEnableFp( ch ) )
#define uartlIrdaIsEnabled( pDrv, ch )         ( pDrv->pDrvFuncs->btlIrdaIsEnabledFp( ch ) )
#define uartlBaudDetDisable( pDrv, ch )        ( pDrv->pDrvFuncs->btlBaudDetDisableFp( ch ) )
#define uartlBaudDetEnable( pDrv, ch )         ( pDrv->pDrvFuncs->btlBaudDetEnableFp( ch ) )
#define uartlBaudDetIsEnabled( pDrv, ch )      ( pDrv->pDrvFuncs->btlBaudDetIsEnabledFp( ch ) )
#define uartlBaudDetGet( pDrv, ch )            ( pDrv->pDrvFuncs->btlBaudDetGetFp( ch ) )
#define uartlBaudRtmodDisable( pDrv, ch )      ( pDrv->pDrvFuncs->btlBaudRtModDisableFp( ch ) )
#define uartlBaudRtmodEnable( pDrv, ch )       ( pDrv->pDrvFuncs->btlBaudRtModEnableFp( ch ) )
#define uartlBaudRtmodIsEnabled( pDrv, ch )    ( pDrv->pDrvFuncs->btlBaudRtModIsEnabledFp( ch ) )
#define uartlBaudRtmodValGet( pDrv, ch )       ( pDrv->pDrvFuncs->btlBaudRtModValGetFp( ch ) )
#define uartlBaudRtmodValSet( pDrv, ch, adj0, adj1 ) \
                                               ( pDrv->pDrvFuncs->btlBaudRtModValSetFp( ch, adj0, adj1 ) )
#endif

typedef struct
{
   /* This must be the first structure member. */
   const BTL_FUNCS *pDrvFuncs;

} BTL_DRV;


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
