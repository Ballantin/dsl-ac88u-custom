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
*  @file    shal.h
*
*  @brief   Lower SHA (Secure Hash Algorithm) device driver header file
*
*  This file contains the lower device driver header information for the
*  SHA module.
*/
/****************************************************************************/

#ifndef SHAL_H
#define SHAL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
   int (*shalInitFp)( void );
   int (*shalChValidFp)( int ch );
   int (*shalDisableFp)( int ch );
   int (*shalEnableFp)( int ch );
   int (*shalIsEnabledFp)( int ch );
   int (*shalResetFp)( int ch );
   int (*shalSetCmdOptionFp)( int ch, int cmdOption, int cmdFlag );
   int (*shalCreateCmdFp)( int ch, int cmdMode, int keySize, 
                              XDRV_UINT32 *cmdWord );
} SHAL_FUNCS;


#define shalInit( pDrv )           ( pDrv->pDrvFuncs->shalInitFp( ) )
#define shalChValid( pDrv , ch )   ( pDrv->pDrvFuncs->shalChValidFp( ch ) )
#define shalDisable( pDrv , ch )   ( pDrv->pDrvFuncs->shalDisableFp( ch ) )
#define shalEnable( pDrv , ch )    ( pDrv->pDrvFuncs->shalEnableFp( ch ) )
#define shalIsEnabled( pDrv , ch ) ( pDrv->pDrvFuncs->shalIsEnabledFp( ch ) )
#define shalReset( pDrv , ch )     ( pDrv->pDrvFuncs->shalResetFp( ch ) )
#define shalSetCmdOption( pDrv , ch, cmdOption, cmdFlag ) \
                                   ( pDrv->pDrvFuncs->shalSetCmdOptionFp( ch , cmdOption , cmdFlag ) )
#define shalCreateCmd( pDrv , ch , cmdOp , cmdMode , keySize , cmdWordp ) \
                                   ( pDrv->pDrvFuncs->shalCreateCmdFp( ch , cmdMode , keySize , cmdWordp ) )

typedef struct
{
   /* This must be the first structure member. */
   const SHAL_FUNCS *pDrvFuncs;

} SHAL_DRV;


/**
*  @brief   DMA descriptor options
*
*  The DMA descriptor options used to specify any special options
*  used to configure the DMA descriptor.
*/
#define SHAL_DMA_OPT_CMD    0x2      /**< SHA command descriptor */
#define SHAL_DMA_OPT_DATA   0x0      /**< SHA data    descriptor */
#define SHAL_DMA_OPT_STS    0x1      /**< SHA status  descriptor */


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
