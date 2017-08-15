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
*  @file    ledml.h
*
*  @brief   Lower LED Matrix device driver header file
*
*  This file contains the lower device driver header information for the
*  LED Matrix module.
*/
/****************************************************************************/

#ifndef LEDML_H
#define LEDML_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
   int (*ledmlInitp)( int ch, int serialMode, int numRows, int numCols, 
      int rowOnVal, int colOnVal );
   int (*ledmlChValidp)( int ch );
   int (*ledmlDisablep)( int ch );
   int (*ledmlEnablep)( int ch );
   int (*ledmlIsEnabledp)( int ch );
   int (*ledmlResetp)( int ch );

   int (*ledmlGuardTimeGetp)( int ch );
   int (*ledmlGuardTimeSetp)( int ch, XDRV_UINT32 guardTime );
   int (*ledmlOnTimeGetp)( int ch );
   int (*ledmlOnTimeSetp)( int ch, XDRV_UINT32 onTime );
   int (*ledmlReadp)( int ch, XDRV_UINT16 *bufp, int len );
   int (*ledmlResolutionSetp)( int ch, int res );
   int (*ledmlResolutionGetp)( int ch );
   int (*ledmlSerialClkGetp)( int ch );
   int (*ledmlSerialClkSetp)( int ch, int freq );
   int (*ledmlSetDefaultsp)( int ch );
   int (*ledmlWritep)( int ch, void *bufp, int len );
   int (*ledmlWriteCoordsp)( int ch, int row, int col, int onOff );
} LEDML_FUNCS;

#define ledmlInit( drvp , ch, serialMode, \
      numRows, numCols, rowOnVal, colOnVal )  (drvp)->funcp->ledmlInitp( ch, serialMode, numRows, numCols, rowOnVal, colOnVal )
#define ledmlChValid( drvp , ch )             (drvp)->funcp->ledmlChValidp( ch )
#define ledmlDisable( drvp , ch )             (drvp)->funcp->ledmlDisablep( ch )
#define ledmlEnable( drvp , ch )              (drvp)->funcp->ledmlEnablep( ch )
#define ledmlIsEnabled( drvp , ch )           (drvp)->funcp->ledmlIsEnabledp( ch )
#define ledmlReset( drvp , ch )               (drvp)->funcp->ledmlResetp( ch )

#define ledmlGuardTimeGet( drvp , ch )             (drvp)->funcp->ledmlGuardTimeGetp( ch )
#define ledmlGuardTimeSet( drvp , ch, guardTime )  (drvp)->funcp->ledmlGuardTimeSetp( ch, guardTime )
#define ledmlOnTimeGet( drvp , ch )                (drvp)->funcp->ledmlOnTimeGetp( ch )
#define ledmlOnTimeSet( drvp , ch, onTime )        (drvp)->funcp->ledmlOnTimeSetp( ch, onTime )
#define ledmlRead( drvp , ch, bufp, len )          (drvp)->funcp->ledmlReadp( ch, bufp, len )
#define ledmlResolutionSet( drvp , ch, res )       (drvp)->funcp->ledmlResolutionSetp( ch, res )
#define ledmlResolutionGet( drvp , ch )            (drvp)->funcp->ledmlResolutionGetp( ch )
#define ledmlSerialClkGet( drvp , ch )             (drvp)->funcp->ledmlSerialClkGetp( ch )
#define ledmlSerialClkSet( drvp , ch, freq )       (drvp)->funcp->ledmlSerialClkSetp( ch, freq )
#define ledmlSetDefaults( drvp , ch )              (drvp)->funcp->ledmlSetDefaultsp( ch )
#define ledmlWrite( drvp , ch, bufp, len )         (drvp)->funcp->ledmlWritep( ch, bufp, len )
#define ledmlWriteCoords( drvp , ch, row, col, onOff) (drvp)->funcp->ledmlWriteCoordsp( ch, row, col, onOff )

typedef struct
{
   /* This must be the first structure member. */
   const LEDML_FUNCS *funcp;

} LEDML_DRV;


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
