/***************************************************************************
*    Copyright 2004  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: xdrvDaa.h
*
****************************************************************************
*    Description:
*
*     Public API for hardware-independent interface to the DAAs.
*
****************************************************************************/

#ifndef XDRV_DAA_H
#define XDRV_DAA_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

/* Public DAA modes */
typedef enum
{
   XDRV_DAA_MODE_ONHOOK,
   XDRV_DAA_MODE_ONHOOK_DATA,
   XDRV_DAA_MODE_OFFHOOK,
   XDRV_DAA_MODE_RING_VALID_ON,
   XDRV_DAA_MODE_RING_VALID_OFF

} XDRV_DAA_MODE;

/* Public DAA loop current readings */
typedef enum
{
   XDRV_DAA_LOOP_OPEN,
   XDRV_DAA_LOOP_CLOSED,
   XDRV_DAA_LOOP_CLOSED_REVERSE
} XDRV_DAA_LOOP;


/* --------------------------------------------------------------------------
** 'XDRV_DAA' is the base, generic DAA structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific DAA drivers.
**
** 'XDRV_DAA_FUNCS' must be the only structure member of 'XDRV_DAA'.
**
** Chip specific DAA drivers should provide a "derived" version of XDRV_DAA:
**
**    typedef struct DAA_CHIP_DRV
**    {
**       const XDRV_DAA_FUNCS  *drvFuncs;
**
**       int   chipSpecificData1;
**       int   chipSpecificData2;
**       ...
**
**    } DAA_CHIP_DRV;
**
** Clients, which have access to XDRV_DAA structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_DAA struct as the first parameter. This struct can be cast to the chip
** specific version of the DAA structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_DAA_FUNCS *pDrvFuncs;

} XDRV_DAA;


/* --------------------------------------------------------------------------
** DAA API function pointers.
*/

/*
*****************************************************************************
** FUNCTION:   xdrvDaaModeControl
**
** PURPOSE:    Sets the DAA into one of the defined modes
**
** PARAMETERS: pDrv  - pointer to the DAA driver device structure
**             mode  - the mode to set the DAA into.
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvDaaModeControl)( XDRV_DAA *pDrv, XDRV_DAA_MODE mode );

/*
*****************************************************************************
** FUNCTION:   xdrvDaaLedControl
**
** PURPOSE:    This function controls the on Hook State LED, if any
**
** PARAMETERS: pDrv  - pointer to the DAA driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvDaaLedControl) ( XDRV_DAA *pDrv, int value );

/*
*****************************************************************************
** FUNCTION:   xdrvDaaIsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
typedef XDRV_BOOL (*xdrvDaaIsOffhook)  ( XDRV_DAA *pDrv );

/*
*****************************************************************************
** FUNCTION:   xdrvDaaIsRingActive
**
** PURPOSE:    Determine if the ring signal is asserted on the channel
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if ring active, XDRV_FALSE if not active
**
** NOTE:       This function tests the instantaneous state of the ring
**             signal. No history or debounce is provided. Output may or
**             may not be true for the entire duration of one 'ring', so
**             it is necessary to consider multiple samples to detect
**             the presence of ringing on the channel.
*****************************************************************************
*/
typedef XDRV_BOOL (*xdrvDaaIsRingActive)  ( XDRV_DAA *pDrv );

/*
*****************************************************************************
** FUNCTION:   xdrvDaaGetLoopCurrent
**
** PURPOSE:    Determine state of loop current for channel
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_DAA_LOOP
**
*****************************************************************************
*/
typedef XDRV_DAA_LOOP (*xdrvDaaGetLoopCurrent)  ( XDRV_DAA *pDrv );

/*
*****************************************************************************
** FUNCTION:   xdrvDaaProcessEvents
**
** PURPOSE:    To process events.
**
** PARAMETERS: pDrv  - pointer to the DAA driver device structure
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvDaaProcessEvents)( XDRV_DAA *pDrv );

/*
*****************************************************************************
** FUNCTION:   xdrvDaaProbe
**
** PURPOSE:    To probe registers. (DEBUG ONLY)
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef XDRV_UINT32 (*xdrvDaaProbe)( XDRV_DAA *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                                     XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                                     XDRV_UINT8 set );

/*
** Structure that contains all function pointers that define the API.
*/
typedef struct XDRV_DAA_FUNCS
{
   xdrvDaaModeControl       daaModeControl;
   xdrvDaaLedControl        daaLedControl;
   xdrvDaaIsOffhook         daaIsOffhook;
   xdrvDaaIsRingActive      daaIsRingActive;
   xdrvDaaGetLoopCurrent    daaGetLoopCurrent;
   xdrvDaaProcessEvents     daaProcessEvents;
   xdrvDaaProbe             daaProbe;
} XDRV_DAA_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the DAA.
*/

#define xdrvDaaModeControl( pDrv, mode ) \
         ( (pDrv)->pDrvFuncs->daaModeControl( (pDrv), (mode) ) )

#define xdrvDaaLedControl( pDrv, value ) \
         ( (pDrv)->pDrvFuncs->daaLedControl( (pDrv), (mode) ) )

#define xdrvDaaIsOffhook( pDrv ) \
         ( (pDrv)->pDrvFuncs->daaIsOffhook( (pDrv) ) )

#define xdrvDaaIsRingActive( pDrv ) \
         ( (pDrv)->pDrvFuncs->daaIsRingActive( (pDrv) ) )

#define xdrvDaaGetLoopCurrent( pDrv ) \
         ( (pDrv)->pDrvFuncs->daaGetLoopCurrent( (pDrv) ) )

#define xdrvDaaProcessEvents( pDrv )\
         ( (pDrv)->pDrvFuncs->daaProcessEvents( (pDrv) ) )

#define xdrvDaaProbe( pDrv, deviceId, chan, reg, regSize, value, valueSize, indirect, set )\
         ( pDrv->pDrvFuncs->daaProbe( pDrv, deviceId, chan, reg, regSize, value, valueSize, indirect, set ) )

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_DAA_H  */
