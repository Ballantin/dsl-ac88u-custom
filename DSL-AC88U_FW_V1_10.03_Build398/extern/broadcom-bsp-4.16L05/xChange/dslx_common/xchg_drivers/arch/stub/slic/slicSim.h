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
*    Filename: slicSim.h
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      Subscriber Line Interface Circuits (SLIC) API.
*
****************************************************************************/

#ifndef SLIC_SIM_H
#define SLIC_SIM_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <xdrvSlic.h>


/*
** Simlutaion SLIC driver structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'XDRV_SLIC' base-class(structure).
** The base SLIC structure specifies a set of function pointers that defines
** the generic hardware-independent API that must be provided by all chip
** specific SLIC drivers.
**
** When the SLIC driver initialization function is invoked, the base slic
** structure function pointers will be intialized to a set of hardware specific
** SLIC driver functions. Therefore, given a pointer to the base SLIC driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific SLIC driver function.
**
** SLIC driver state that is specific to the simulation should be stored in the
** 'SLIC_SIM_DRV' structure. A pointer to 'XDRV_SLIC_FUNCS' must be the first
** member of the 'SLIC_SIM_DRV' struct.
*/
typedef struct
{
   /* Must be first element of this struct. */
   const XDRV_SLIC_FUNCS  *pDrvFuncs;

   /* Simluation-specific state info. */
   int                     chan;      /* SLIC channel ID              */

   /* Current hook state */
   XDRV_BOOL               isOffHook;

} SLIC_SIM_DRV;


/*
*****************************************************************************
** FUNCTION:   slicSimInit
**
** PURPOSE:    Initialize and open the SLIC
**
** PARAMETERS: chan       - the line number ( 0 referenced )
**             pDev       - pointer to the SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSimInit
(
   int                     chan,
   SLIC_SIM_DRV           *pDev
);



/*
*****************************************************************************
** FUNCTION:   slicSimDeinit
**
** PURPOSE:    Shutdown the SLIC
**
** PARAMETERS: pDev  - pointer to the SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSimDeinit( SLIC_SIM_DRV *pDev );


/*
*****************************************************************************
** FUNCTION:   slicSimSetHookState
**
** PURPOSE:    To set the current hook state.
**
** PARAMETERS: pDrv        (mod) SLIC driver object.
**             isOffHook   (in)  Hook state.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void slicSimSetHookState( XDRV_SLIC *pDrv, XDRV_BOOL isOffHook );


#if defined(__cplusplus)
}
#endif



#endif   /* SLIC_SIM_H */
