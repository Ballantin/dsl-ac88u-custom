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
*  @file    ledcl.h
*
*  @brief   Lower LED Cadence device driver header file
*
*  This file contains the lower device driver header information for the
*  LED Cadence module.
*/
/****************************************************************************/

#ifndef LEDCL_H
#define LEDCL_H


/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
   int (*ledclInitp)( int ch );
   int (*ledclChValidp)( int ch );
   int (*ledclDisablep)( int ch );
   int (*ledclEnablep)( int ch );
   int (*ledclIsEnabledp)( int ch );
   int (*ledclResetp)( int ch );

   int (*ledclUseOnOffCntGetp)( int ch );
   int (*ledclUseOnOffCntSetp)( int ch, int arg );
   int (*ledclOffCntGetp)( int ch );
   int (*ledclOffCntSetp)( int ch, int arg );
   int (*ledclOnCntGetp)( int ch );
   int (*ledclOnCntSetp)( int ch, int arg );
   int (*ledclOffTmrGetp)( int ch );
   int (*ledclOffTmrSetp)( int ch, int arg );
   int (*ledclOnTmrGetp)( int ch );
   int (*ledclOnTmrSetp)( int ch, int arg );
   int (*ledclResolutionGetp)( int ch );
   int (*ledclResolutionSetp)( int ch, int arg );
   int (*ledclActiveHighSetp)( int ch, int arg );
} LEDCL_FUNCS;

#define ledclInit( drvp , ch )                (drvp)->funcp->ledclInitp( ch )
#define ledclChValid( drvp , ch )             (drvp)->funcp->ledclChValidp( ch )
#define ledclDisable( drvp , ch )             (drvp)->funcp->ledclDisablep( ch )
#define ledclEnable( drvp , ch )              (drvp)->funcp->ledclEnablep( ch )
#define ledclIsEnabled( drvp , ch )           (drvp)->funcp->ledclIsEnabledp( ch )
#define ledclReset( drvp , ch )               (drvp)->funcp->ledclResetp( ch )

#define ledclUseOnOffCntGet( drvp, ch )       (drvp)->funcp->ledclUseOnOffCntGetp( ch )
#define ledclUseOnOffCntSet( drvp, ch, arg )  (drvp)->funcp->ledclUseOnOffCntSetp( ch, arg )
#define ledclOffCntGet( drvp, ch )            (drvp)->funcp->ledclOffCntGetp( ch )
#define ledclOffCntSet( drvp, ch, arg )       (drvp)->funcp->ledclOffCntSetp( ch, arg )
#define ledclOnCntGet( drvp, ch )             (drvp)->funcp->ledclOnCntGetp( ch )
#define ledclOnCntSet( drvp, ch, arg )       (drvp)->funcp->ledclOnCntSetp( ch, arg )
#define ledclOffTmrGet( drvp, ch )            (drvp)->funcp->ledclOffTmrGetp( ch )
#define ledclOffTmrSet( drvp, ch, arg )       (drvp)->funcp->ledclOffTmrSetp( ch, arg )
#define ledclOnTmrGet( drvp, ch )             (drvp)->funcp->ledclOnTmrGetp( ch )
#define ledclOnTmrSet( drvp, ch, arg )        (drvp)->funcp->ledclOnTmrSetp( ch, arg )
#define ledclResolutionGet( drvp, ch )        (drvp)->funcp->ledclResolutionGetp( ch )
#define ledclResolutionSet( drvp, ch, arg )   (drvp)->funcp->ledclResolutionSetp( ch, arg )
#define ledclActiveHighSet( drvp, ch, arg )   (drvp)->funcp->ledclActiveHighSetp( ch, arg )

typedef struct
{
   /* This must be the first structure member. */
   const LEDCL_FUNCS *funcp;

} LEDCL_DRV;


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
