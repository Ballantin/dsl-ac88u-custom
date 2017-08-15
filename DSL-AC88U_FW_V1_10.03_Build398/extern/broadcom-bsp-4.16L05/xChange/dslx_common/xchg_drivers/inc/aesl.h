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
*  @file    aesl.h
*
*  @brief   Lower AES (Advanced Encryption Standard) device driver header file
*
*  This file contains the lower device driver header information for the
*  AES module.
*/
/****************************************************************************/

#ifndef AESL_H
#define AESL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
   int (*aeslInitFp)( void );
   int (*aeslChValidFp)( int ch );
   int (*aeslDisableFp)( int ch );
   int (*aeslEnableFp)( int ch );
   int (*aeslIsEnabledFp)( int ch );
   int (*aeslResetFp)( int ch );
   int (*aeslCreateCmdFp)( int ch, int cmdOp, int cmdMode, int keySize, XDRV_UINT32 *cmdWord );
} AESL_FUNCS;

#define aeslInit( pDrv )            ( pDrv->pDrvFuncs->aeslInitFp( ) )
#define aeslChValid( pDrv , ch )    ( pDrv->pDrvFuncs->aeslChValidFp( ch ) )
#define aeslDisable( pDrv , ch )    ( pDrv->pDrvFuncs->aeslDisableFp( ch ) )
#define aeslEnable( pDrv , ch )     ( pDrv->pDrvFuncs->aeslEnableFp( ch ) )
#define aeslIsEnabled( pDrv , ch )  ( pDrv->pDrvFuncs->aeslIsEnabledFp( ch ) )
#define aeslReset( pDrv , ch )      ( pDrv->pDrvFuncs->aeslResetFp( ch ) )
#define aeslCreateCmd( pDrv , ch, cmdOp, cmdMode, keySize, cmdWordp ) \
                                    ( pDrv->pDrvFuncs->aeslCreateCmdFp( ch, cmdOp, cmdMode, keySize, cmdWordp ) )

typedef struct
{
   /* This must be the first structure member. */
   const AESL_FUNCS *pDrvFuncs;

} AESL_DRV;


/**
*  @brief   DMA descriptor options
*
*  The DMA descriptor options used to specify any special options
*  used to configure the DMA descriptor.
*/
#define AESL_DMA_OPT_CMD    0x1      /**< AES command descriptor */
#define AESL_DMA_OPT_KEY    0x8      /**< AES key     descriptor */
#define AESL_DMA_OPT_IV     0x4      /**< AES iv      descriptor */
#define AESL_DMA_OPT_DATA   0x0      /**< AES data    descriptor */
#define AESL_DMA_OPT_STS    0x2      /**< AES status  descriptor */



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
