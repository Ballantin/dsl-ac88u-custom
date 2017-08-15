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
*  @file    uartl.h
*
*  @brief   Lower UART device driver header file
*
*  This file contains the lower device driver header information for the
*  UART module.
*/
/****************************************************************************/

#ifndef UARTL_H
#define UARTL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>


/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
   XDRV_SINT32 (*uartlBaudGetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBaudSetFp)( XDRV_SINT32 ch, XDRV_SINT32 baud );
   XDRV_SINT32 (*uartlBitsGetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBitsSetFp)( XDRV_SINT32 ch, XDRV_SINT32 bits );
   XDRV_SINT32 (*uartlChValidFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlCtsFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlDtrDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlDtrEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlDtrIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlFlowIrqDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlFlowIrqEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlFlowIrqIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlIsrTxFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlIsrRxFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlParityDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlParityEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlParityEvenDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlParityEvenEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlParityEvenIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlParityIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlReadFp)( XDRV_SINT32 ch, char *datap );
   XDRV_SINT32 (*uartlRtsDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlRtsEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlRtsIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlRxDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlRxEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlRxIrqDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlRxIrqEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlRxIrqIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlRxIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlRxResetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlStopBitsGetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlStopBitsSetFp)( XDRV_SINT32 ch, XDRV_SINT32 halfBits );
   XDRV_SINT32 (*uartlTxDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlTxEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlTxIrqDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlTxIrqEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlTxIrqIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlTxIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlTxResetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlWriteFp)( XDRV_SINT32 ch, char data );

   XDRV_SINT32 (*uartlIrdaDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlIrdaEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlIrdaIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBaudDetDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBaudDetEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBaudDetIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBaudDetGetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBaudRtModDisableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBaudRtModEnableFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBaudRtModIsEnabledFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBaudRtModValGetFp)( XDRV_SINT32 ch );
   XDRV_SINT32 (*uartlBaudRtModValSetFp)( XDRV_SINT32 ch, int adj0, int adj1 );
} UARTL_FUNCS;


#define uartlBaudGet( pDrv, ch )               ( pDrv->pDrvFuncs->uartlBaudGetFp( ch ) )
#define uartlBaudSet( pDrv, ch, baud )         ( pDrv->pDrvFuncs->uartlBaudSetFp( ch, baud ) )
#define uartlBitsGet( pDrv, ch )               ( pDrv->pDrvFuncs->uartlBitsGetFp( ch ) )
#define uartlBitsSet( pDrv, ch, bits )         ( pDrv->pDrvFuncs->uartlBitsSetFp( ch, bits ) )
#define uartlChValid( pDrv, ch )               ( pDrv->pDrvFuncs->uartlChValidFp( ch ) )
#define uartlCts( pDrv, ch )                   ( pDrv->pDrvFuncs->uartlCtsFp( ch ) )
#define uartlDtrDisable( pDrv, ch )            ( pDrv->pDrvFuncs->uartlDtrDisableFp( ch ) )
#define uartlDtrEnable( pDrv, ch )             ( pDrv->pDrvFuncs->uartlDtrEnableFp( ch ) )
#define uartlDtrIsEnabled( pDrv, ch )          ( pDrv->pDrvFuncs->uartlDtrIsEnabledFp( ch ) )
#define uartlFlowIrqDisable( pDrv, ch )        ( pDrv->pDrvFuncs->uartlFlowIrqDisableFp( ch ) )
#define uartlFlowIrqEnable( pDrv, ch )         ( pDrv->pDrvFuncs->uartlFlowIrqEnableFp( ch ) )
#define uartlFlowIrqIsEnabled( pDrv, ch )      ( pDrv->pDrvFuncs->uartlFlowIrqIsEnabledFp( ch ) )
#define uartlIsrTx( pDrv, ch )                 ( pDrv->pDrvFuncs->uartlIsrTxFp( ch ) )
#define uartlIsrRx( pDrv, ch )                 ( pDrv->pDrvFuncs->uartlIsrRxFp( ch ) )
#define uartlParityDisable( pDrv, ch )         ( pDrv->pDrvFuncs->uartlParityDisableFp( ch ) )
#define uartlParityEnable( pDrv, ch )          ( pDrv->pDrvFuncs->uartlParityEnableFp( ch ) )
#define uartlParityEvenDisable( pDrv, ch )     ( pDrv->pDrvFuncs->uartlParityEvenDisableFp( ch ) )
#define uartlParityEvenEnable( pDrv, ch )      ( pDrv->pDrvFuncs->uartlParityEvenEnableFp( ch ) )
#define uartlParityEvenIsEnabled( pDrv, ch )   ( pDrv->pDrvFuncs->uartlParityEvenIsEnabledFp( ch ) )
#define uartlParityIsEnabled( pDrv, ch )       ( pDrv->pDrvFuncs->uartlParityIsEnabledFp( ch ) )
#define uartlRead( pDrv, ch, datap )           ( pDrv->pDrvFuncs->uartlReadFp( ch, datap ) )
#define uartlRtsDisable( pDrv, ch )            ( pDrv->pDrvFuncs->uartlRtsDisableFp( ch ) )
#define uartlRtsEnable( pDrv, ch )             ( pDrv->pDrvFuncs->uartlRtsEnableFp( ch ) )
#define uartlRtsIsEnabled( pDrv, ch )          ( pDrv->pDrvFuncs->uartlRtsIsEnabledFp( ch ) )
#define uartlRxDisable( pDrv, ch )             ( pDrv->pDrvFuncs->uartlRxDisableFp( ch ) )
#define uartlRxEnable( pDrv, ch )              ( pDrv->pDrvFuncs->uartlRxEnableFp( ch ) )
#define uartlRxIrqDisable( pDrv, ch )          ( pDrv->pDrvFuncs->uartlRxIrqDisableFp( ch ) )
#define uartlRxIrqEnable( pDrv, ch )           ( pDrv->pDrvFuncs->uartlRxIrqEnableFp( ch ) )
#define uartlRxIrqIsEnabled( pDrv, ch )        ( pDrv->pDrvFuncs->uartlRxIrqIsEnabledFp( ch ) )
#define uartlRxIsEnabled( pDrv, ch )           ( pDrv->pDrvFuncs->uartlRxIsEnabledFp( ch ) )
#define uartlRxReset( pDrv, ch )               ( pDrv->pDrvFuncs->uartlRxResetFp( ch ) )
#define uartlStopBitsGet( pDrv, ch )           ( pDrv->pDrvFuncs->uartlStopBitsGetFp( ch ) )
#define uartlStopBitsSet( pDrv, ch, halfBits ) ( pDrv->pDrvFuncs->uartlStopBitsSetFp( ch, halfBits ) )
#define uartlTxDisable( pDrv, ch )             ( pDrv->pDrvFuncs->uartlTxDisableFp( ch ) )
#define uartlTxEnable( pDrv, ch )              ( pDrv->pDrvFuncs->uartlTxEnableFp( ch ) )
#define uartlTxIrqDisable( pDrv, ch )          ( pDrv->pDrvFuncs->uartlTxIrqDisableFp( ch ) )
#define uartlTxIrqEnable( pDrv, ch )           ( pDrv->pDrvFuncs->uartlTxIrqEnableFp( ch ) )
#define uartlTxIrqIsEnabled( pDrv, ch )        ( pDrv->pDrvFuncs->uartlTxIrqIsEnabledFp( ch ) )
#define uartlTxIsEnabled( pDrv, ch )           ( pDrv->pDrvFuncs->uartlTxIsEnabledFp( ch ) )
#define uartlTxReset( pDrv, ch )               ( pDrv->pDrvFuncs->uartlTxResetFp( ch ) )
#define uartlWrite( pDrv, ch, data )           ( pDrv->pDrvFuncs->uartlWriteFp( ch, data ) )

#define uartlIrdaDisable( pDrv, ch )           ( pDrv->pDrvFuncs->uartlIrdaDisableFp( ch ) )
#define uartlIrdaEnable( pDrv, ch )            ( pDrv->pDrvFuncs->uartlIrdaEnableFp( ch ) )
#define uartlIrdaIsEnabled( pDrv, ch )         ( pDrv->pDrvFuncs->uartlIrdaIsEnabledFp( ch ) )
#define uartlBaudDetDisable( pDrv, ch )        ( pDrv->pDrvFuncs->uartlBaudDetDisableFp( ch ) )
#define uartlBaudDetEnable( pDrv, ch )         ( pDrv->pDrvFuncs->uartlBaudDetEnableFp( ch ) )
#define uartlBaudDetIsEnabled( pDrv, ch )      ( pDrv->pDrvFuncs->uartlBaudDetIsEnabledFp( ch ) )
#define uartlBaudDetGet( pDrv, ch )            ( pDrv->pDrvFuncs->uartlBaudDetGetFp( ch ) )
#define uartlBaudRtmodDisable( pDrv, ch )      ( pDrv->pDrvFuncs->uartlBaudRtModDisableFp( ch ) )
#define uartlBaudRtmodEnable( pDrv, ch )       ( pDrv->pDrvFuncs->uartlBaudRtModEnableFp( ch ) )
#define uartlBaudRtmodIsEnabled( pDrv, ch )    ( pDrv->pDrvFuncs->uartlBaudRtModIsEnabledFp( ch ) )
#define uartlBaudRtmodValGet( pDrv, ch )       ( pDrv->pDrvFuncs->uartlBaudRtModValGetFp( ch ) )
#define uartlBaudRtmodValSet( pDrv, ch, adj0, adj1 ) \
                                               ( pDrv->pDrvFuncs->uartlBaudRtModValSetFp( ch, adj0, adj1 ) )


typedef struct
{
   /* This must be the first structure member. */
   const UARTL_FUNCS *pDrvFuncs;

} UARTL_DRV;

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
