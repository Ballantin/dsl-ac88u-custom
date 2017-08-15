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
*    Filename: casDriverSim.h
*
****************************************************************************
*    Description:
*
*     Interface for ...
*
****************************************************************************/

#ifndef CAS_DRIVER_SIM_H
#define CAS_DRIVER_SIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvSlic.h>
#include <xdrvCas.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

int casDriverSimInit( XDRV_SLIC *slicDrv, XDRV_CAS *casDriver );
int casDriverSimDeinit( XDRV_CAS *casDriver );



#ifdef __cplusplus
    }
#endif

#endif  /* CAS_DRIVER_SIM_H  */
