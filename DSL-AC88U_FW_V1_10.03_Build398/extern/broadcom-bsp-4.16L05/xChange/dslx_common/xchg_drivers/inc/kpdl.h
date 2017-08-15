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
*  @file    kpdl.h
*
*  @brief   Lower keypad device driver header file
*
*  This file contains the lower device driver header information for the
*  keypad module.
*/
/****************************************************************************/

#ifndef KPDL_H
#define KPDL_H


/* ---- Include Files ----------------------------------------------------- */


/* ---- Public Constants and Types ---------------------------------------- */

/**
*  @brief   Keypad mode
*
*  The keypad enumeration is used to describe the current operating mode of 
*  the driver.
*/
typedef enum
{
   KPDL_MODE_DELTA = 0,    /**< Interrupt generated on key matrix change
                           *    (edge triggered)
                           */
   KPDL_MODE_PERSIST       /**< Interrupt generated every debounce interval
                           *    while a key is in the down position (level
                           *    triggered)
                           */
} KPDL_MODE;

/**
*  @brief   Keyout mode
*
*  The keypad enumeration is used to describe the current operating mode of 
*  the keyout pins.
*/
typedef enum
{
  KPDL_KEYOUT_CMOS = 0,     /**< Keyout pins are driver as standard
                            *    CMOS output
                            */
  KPDL_KEYOUT_OPENDRAIN     /**< Keyout pins are driven as open drain
                            */
} KPDL_KEYOUT;


/**
*  @brief   Keypad matrix key state
*
*  This macro is used to determine the state of a key in the keypad matrix
*  handle returned by kpdlMatrix().
*
*  @param   m - [IN] Keypad matrix handle
*  @param   r - [IN] Key row
*  @param   c - [IN] Key column
*
*  @return
*     KPD_KEYSTATE_DOWN if key is in down position, or
*     KPD_KEYSTATE_UP if key is in up position,
*/
#define kpdlKeyIsDown(m,r,c)     !(((int *)(m))[r] & (1 << (c)))


typedef struct
{
   int (*kpdlColsp)( void );
   int (*kpdlDebounceGetp)( void );
   int (*kpdlDebounceSetp)( int count );
   int (*kpdlDisablep)( void );
   int (*kpdlEnablep)( void );
   int (*kpdlIrqDisablep)( void );
   int (*kpdlIrqEnablep)( void );
   int (*kpdlIrqIsEnabledp)( void );
   int (*kpdlIsEnabledp)( void );
   int (*kpdlMatrixp)( void );
   KPDL_MODE (*kpdlModeGetp)( void );
   KPDL_MODE (*kpdlModeSetp)( KPDL_MODE mode );
   int (*kpdlSampleGetp)( void );
   int (*kpdlSampleSetp)( int freq );
   KPDL_KEYOUT (*kpdlKeyoutModeGetp)( void );
   KPDL_KEYOUT (*kpdlKeyoutModeSetp)( KPDL_KEYOUT keyout_mode );
    int (*kpdlRowsp)( void );
} KPDL_FUNCS;

#define kpdlCols( drvp )                  (drvp)->funcp->kpdlColsp()
#define kpdlDebounceGet( drvp )           (drvp)->funcp->kpdlDebounceGetp()
#define kpdlDebounceSet( drvp, count )    (drvp)->funcp->kpdlDebounceSetp( count )
#define kpdlDisable( drvp )               (drvp)->funcp->kpdlDisablep()
#define kpdlEnable( drvp )                (drvp)->funcp->kpdlEnablep()
#define kpdlIrqDisable( drvp )            (drvp)->funcp->kpdlIrqDisablep()
#define kpdlIrqEnable( drvp )             (drvp)->funcp->kpdlIrqEnablep()
#define kpdlIrqIsEnabled( drvp )          (drvp)->funcp->kpdlIrqIsEnabledp()
#define kpdlIsEnabled( drvp )             (drvp)->funcp->kpdlIsEnabledp()
#define kpdlMatrix( drvp )                (drvp)->funcp->kpdlMatrixp()
#define kpdlModeGet( drvp )               (drvp)->funcp->kpdlModeGetp()
#define kpdlModeSet( drvp, mode )         (drvp)->funcp->kpdlModeSetp( mode )
#define kpdlSampleGet( drvp )             (drvp)->funcp->kpdlSampleGetp()
#define kpdlSampleSet( drvp, freq )       (drvp)->funcp->kpdlSampleSetp( freq )
#define kpdlKeyoutModeGet( drvp )         (drvp)->funcp->kpdlKeyoutModeGetp()
#define kpdlKeyoutModeSet( drvp, mode )   (drvp)->funcp->kpdlKeyoutModeSetp( mode )
#define kpdlRows( drvp )                  (drvp)->funcp->kpdlRowsp()

typedef struct
{
   /* This must be the first structure member. */
   const KPDL_FUNCS *funcp;

} KPDL_DRV;


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
