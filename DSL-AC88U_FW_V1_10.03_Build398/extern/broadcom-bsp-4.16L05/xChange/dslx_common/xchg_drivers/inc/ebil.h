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
*  @file    ebil.h
*
*  @brief   Lower ebi device driver header file
*
*  This file contains the lower device driver header information for the
*  ebi module.
*/
/****************************************************************************/

#ifndef EBIL_H
#define EBIL_H


/* ---- Include Files ----------------------------------------------------- */

/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
   unsigned int (*ebilBaseAddrSet)(  int ch, unsigned int addr );
   unsigned int (*ebilBaseAddrGet)(  int ch );
   unsigned int (*ebilSizeSet)(  int ch, unsigned int size );
   unsigned int (*ebilSizeGet)(  int ch );
   unsigned int (*ebilWaitStatesSet)(  int ch, unsigned int waitStates );
   unsigned int (*ebilWaitStatesGet)(  int ch );
   unsigned int (*ebilBusWidthSet)(  int ch, unsigned int width );
   unsigned int (*ebilBusWidthGet)(  int ch );
   int (*ebilEnable)(  int ch );
   int (*ebilDisable)(  int ch );
   int (*ebilIsEnabled)( int ch );
   int (*ebilIsChValid)( int ch );  
   int (*ebilTransmitModeSet)( int ch, unsigned int mode ); 
   int (*ebilTransmitModeGet)( int ch );
} EBIL_FUNCS;

#define ebilBaseAddrSet( drvp, ch, addr )          (drvp)->funcp->ebilBaseAddrSet( ch, addr )
#define ebilBaseAddrGet( drvp, ch )                (drvp)->funcp->ebilBaseAddrGet( ch ) 
#define ebilSizeSet( drvp, ch, size )              (drvp)->funcp->ebilSizeSet( ch, size ) 
#define ebilSizeGet( drvp, ch )                    (drvp)->funcp->ebilSizeGet( ch )
#define ebilWaitStatesSet( drvp, ch, waitStates )  (drvp)->funcp->ebilWaitStatesSet( ch, waitStates )
#define ebilWaitStatesGet( drvp, ch )              (drvp)->funcp->ebilWaitStatesGet( ch )
#define ebilBusWidthSet( drvp, ch, width )         (drvp)->funcp->ebilBusWidthSet( ch, width ) 
#define ebilBusWidthGet( drvp, ch )                (drvp)->funcp->ebilBusWidthGet( ch ) 
#define ebilEnable( drvp, ch )                     (drvp)->funcp->ebilEnable( ch )
#define ebilDisable( drvp, ch )                    (drvp)->funcp->ebilDisable( ch ) 
#define ebilIsEnabled( drvp,  ch )                 (drvp)->funcp->ebilIsEnabled( ch )    
#define ebilIsChValid( drvp,  ch )                 (drvp)->funcp->ebilIsChValid( ch )    
#define ebilTransmitModeSet( drvp, ch, mode)       (drvp)->funcp->ebilTransmitModeSet( ch , mode ) 
#define ebilTransmitModeGet( drvp, ch )            (drvp)->funcp->ebilTransmitModeGet( ch ) 

typedef struct
{
   /* This must be the first structure member. */
   const EBIL_FUNCS *funcp;

} EBIL_DRV;


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
