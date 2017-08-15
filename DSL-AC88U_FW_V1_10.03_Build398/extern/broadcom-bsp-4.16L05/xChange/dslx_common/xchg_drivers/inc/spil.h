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
*  @file    spil.h
*
*  @brief   Lower SPI device driver header file
*
*  This file contains the lower device driver header information for the
*  SPI module.
*/
/****************************************************************************/

#ifndef SPIL_H
#define SPIL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
   int (*spilInitp)( void );
   int (*spilChValidp)( int ch );
   int (*spilDisablep)( int ch );
   int (*spilEnablep)( int ch );
   int (*spilIsEnabledp)( int ch );
   int (*spilResetp)( int ch );

   int (*spilSetDefaultsp)( int ch );
   int (*spilIrqModeGetp)( int ch );
   int (*spilIrqModeSetp)( int ch, int mode );
   int (*spilIsrp)( void );
   int (*spilLpbkModeGetp)( int ch );
   int (*spilLpbkModeSetp)( int ch, int mode );
   int (*spilReadp)( int ch, XDRV_UINT16 *bufp, int len );
   int (*spilSckFreqGetp)( int ch );
   int (*spilSckFreqSetp)( int ch, int sckFreq );
   int (*spilWrapModeGetp)( int ch );
   int (*spilWrapModeSetp)( int ch, int mode );
   int (*spilWritep)( int ch, XDRV_UINT16 *bufp, int len );
} SPIL_FUNCS;

#define spilInit( drvp )                     (drvp)->funcp->spilInitp()
#define spilChValid( drvp , ch )             (drvp)->funcp->spilChValidp( ch )
#define spilDisable( drvp , ch )             (drvp)->funcp->spilDisablep( ch )
#define spilEnable( drvp , ch )              (drvp)->funcp->spilEnablep( ch )
#define spilIsEnabled( drvp , ch )           (drvp)->funcp->spilIsEnabledp( ch )
#define spilReset( drvp , ch )               (drvp)->funcp->spilResetp( ch )

#define spilSetDefaults( drvp , ch )         (drvp)->funcp->spilSetDefaultsp( ch )
#define spilIrqModeGet( drvp , ch )          (drvp)->funcp->spilIrqModeGetp( ch )
#define spilIrqModeSet( drvp , ch, mode )    (drvp)->funcp->spilIrqModeSetp( ch, mode )
#define spilIsr( drvp )                      (drvp)->funcp->spilIsrp( )
#define spilLpbkModeGet( drvp , ch )         (drvp)->funcp->spilLpbkModeGetp( ch )
#define spilLpbkModeSet( drvp , ch, mode )   (drvp)->funcp->spilLpbkModeSetp( ch, mode )
#define spilRead( drvp , ch, bufp, len )     (drvp)->funcp->spilReadp( ch, bufp, len )
#define spilSckFreqGet( drvp , ch )          (drvp)->funcp->spilSckFreqGetp( ch )
#define spilSckFreqSet( drvp , ch, sckFreq ) (drvp)->funcp->spilSckFreqSetp( ch, sckFreq )
#define spilWrapModeGet( drvp , ch )         (drvp)->funcp->spilWrapModeGetp( ch )
#define spilWrapModeSet( drvp , ch, mode )   (drvp)->funcp->spilWrapModeSetp( ch, mode )
#define spilWrite( drvp , ch, bufp, len )    (drvp)->funcp->spilWritep( ch, bufp, len )

typedef struct
{
   /* This must be the first structure member. */
   const SPIL_FUNCS *funcp;

} SPIL_DRV;


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
