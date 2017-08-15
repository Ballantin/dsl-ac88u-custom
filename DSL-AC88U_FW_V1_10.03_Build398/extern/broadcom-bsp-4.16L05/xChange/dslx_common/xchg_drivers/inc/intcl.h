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
*  @file    intcl.h
*
*  @brief   Lower interrupt controller device driver header file
*
*  This file contains the lower device driver header information for the
*  interrupt controller module.
*/
/****************************************************************************/

#ifndef INTCL_H
#define INTCL_H


/* ---- Include Files ----------------------------------------------------- */


/* ---- Public Constants and Types ---------------------------------------- */

typedef struct
{
   int (*intclIrqp)( void );
   int (*intclIrqClearp)( int irq );
   int (*intclIrqDisablep)( int irq );
   int (*intclIrqEnablep)( int irq );
   int (*intclIrqIsEnablep)( int irq );
   int (*intclTriggerGetp)( int irq );
   int (*intclTriggerSetp)( int irq, int trig );
   int (*intclIrqLowerToUpperp)( int irq );
   int (*intclIrqUpperToLowerp)( int irq );
   int (*intclTrigUpperToLowerp)( int trig );
} INTCL_FUNCS;

#define intclIrq( drvp )                     (drvp)->funcs->intclIrqp()
#define intclIrqClear( drvp, irq )           (drvp)->funcs->intclIrqClearp( irq )
#define intclIrqDisable( drvp, irq )         (drvp)->funcs->intclIrqDisablep( irq )
#define intclIrqEnable( drvp, irq )          (drvp)->funcs->intclIrqEnablep( irq )
#define intclIrqIsEnable( drvp, irq )        (drvp)->funcs->intclIrqIsEnablep( irq )
#define intclTriggerGet( drvp, trig )         (drvp)->funcs->intclTriggerGetp( trig )
#define intclTriggerSet( drvp, irq, trig )   (drvp)->funcs->intclTriggerSetp( irq, trig )
#define intclIrqLowerToUpper( drvp, irq )    (drvp)->funcs->intclIrqLowerToUpperp( irq )
#define intclIrqUpperToLower( drvp, irq )    (drvp)->funcs->intclIrqUpperToLowerp( irq )
#define intclTrigUpperToLower( drvp, trig )  (drvp)->funcs->intclTrigUpperToLowerp( trig )

typedef struct
{
   /* This must be the first structure member. */
   const INTCL_FUNCS *funcs;

} INTCL_DRV;


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
