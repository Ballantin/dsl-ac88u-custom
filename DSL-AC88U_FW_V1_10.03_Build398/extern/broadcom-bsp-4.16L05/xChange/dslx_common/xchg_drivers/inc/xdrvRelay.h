/***************************************************************************
*    Copyright 2000  Broadcom
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
*    Filename: xdrvSlic.h
*
****************************************************************************
*    Description:
*
*     Public API for hardware-independent interface to the voice channel RELAYs.
*
****************************************************************************/

#ifndef XDRV_RELAY_H
#define XDRV_RELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

/* Public SLIC modes */
typedef enum
{
   XDRV_RELAY_MODE_DEACTIVE = 0,
   XDRV_RELAY_MODE_ACTIVE
} XDRV_RELAY_MODE;


/* --------------------------------------------------------------------------
** 'XDRV_RELAY' is the base, generic RELAY structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific voice channel RELAY drivers.
**
** 'XDRV_RELAY_FUNCS' must be the only structure member of 'XDRV_RELAY'.
**
** Clients, which have access to XDRV_RELAY structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_RELAY struct as the first parameter. This struct can be cast to the chip
** specific version of the SLIC structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_RELAY_FUNCS *pDrvFuncs;

} XDRV_RELAY;


/* --------------------------------------------------------------------------
** RELAY API function pointers.
*/

/*
*****************************************************************************
** FUNCTION:   xdrvRelayIsActive
**
** PURPOSE:    Get the active status of relay
**
** PARAMETERS: pDrv  - pointer to the RELAY driver device structure
**
** RETURNS:    TRUE or FALSE
**
*****************************************************************************
*/
typedef XDRV_BOOL (*xdrvRelayIsActive)( XDRV_RELAY *pDrv );

/*
*****************************************************************************
** FUNCTION:   xdrvRelaySetActive
**
** PURPOSE:    This function controls the on HW relay
**
** PARAMETERS: pDrv  - pointer to the relay driver device structure
**             mode  1 (relay on) or 0 (off)
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvRelaySetActive) ( XDRV_RELAY *pDrv, XDRV_RELAY_MODE mode );

/*
** Structure that contains all function pointers that define the API.
*/
typedef struct XDRV_RELAY_FUNCS
{
   xdrvRelayIsActive         relayIsActive;
   xdrvRelaySetActive        relaySetActive;
} XDRV_RELAY_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the RELAY.
*/

#define xdrvRelayIsActive( pDrv ) \
         ( pDrv->pDrvFuncs->relayIsActive( pDrv ) )

#define xdrvRelaySetActive( pDrv, mode ) \
         ( pDrv->pDrvFuncs->relaySetActive( pDrv, mode ) )


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_RELAY_H  */
