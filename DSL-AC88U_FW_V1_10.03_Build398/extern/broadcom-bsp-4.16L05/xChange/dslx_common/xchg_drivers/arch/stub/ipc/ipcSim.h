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
*    Filename: ipcSim.h
*
****************************************************************************
*    Description:
*
*
****************************************************************************/

#ifndef IPC_SIM_H
#define IPC_SIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvIpc.h>


/* ---- Constants and Types ---------------------------------------------- */

/*
** IPC driver structure. Conceptually, this structure is similar to a
** C++ sub-class that is derived from the 'XDRV_IPC' base-class(structure).
** The base IPC structure specifies a set of function pointers that defines
** the generic hardware-independent API that must be provided by all chip
** specific IPC drivers.
**
** When the IPC driver initialization function is invoked, the base IPC
** structure function pointers will be intialized to a set of hardware specific
** IPC driver functions. Therefore, given a pointer to the base IPC driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific IPC driver function.
**
** IPC driver state that is specific to the simulation should be stored in the
** 'IPC_SIM_DRV' structure. A pointer to 'XDRV_IPC_FUNCS' must be the first
** member of the 'IPC_SIM_DRV' struct.
*/
typedef struct IPC_SIM_DRV
{
   /*
   ** Must be first element of this struct.
   */
   const XDRV_IPC_FUNCS      *pDrvFuncs;


} IPC_SIM_DRV;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   ipcSimInit
**
** PURPOSE:    To initialize the IPC module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void ipcSimInit( void );

/*
*****************************************************************************
** FUNCTION:   ipcSimDeinit
**
** PURPOSE:    To de-initialize the IPC module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void ipcSimDeinit( void );

/*
*****************************************************************************
** FUNCTION:   ipcSimOpen
**
** PURPOSE:    To create IPC driver.
**
** PARAMETERS: ipcDrv            (out) Created IPC driver object.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void ipcSimOpen( IPC_SIM_DRV *ipcDrv );

/*
*****************************************************************************
** FUNCTION:   ipcSimClose
**
** PURPOSE:    To destroy a previously openned IPC driver.
**
** PARAMETERS: ipcDrv   (mod) IPC driver.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void ipcSimClose( IPC_SIM_DRV *ipcDrv );



#ifdef __cplusplus
    }
#endif

#endif  /* IPC_MTA_SIM_H  */
