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
*    Filename: apmSim.h
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      Analog Processsor Module (APM) API.
*
****************************************************************************/

#ifndef APM_SIM_H
#define APM_SIM_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <xdrvApm.h>


/*
** 3341 APM driver structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'XDRV_APM' base-class(structure).
** The base APM structure specifies a set of function pointers that defines
** the generic hardware-independent API that must be provided by all chip
** specific APM drivers.
**
** When the 3341 APM driver initialization function is invoked, the base API
** structure function pointers will be intialized to a set of hardware specific
** APM driver functions. Therefore, given a pointer to the base APM driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific APM driver function.
**
** APM driver state that is specific to the 3341 should be stored in the
** 'APM_3341_DRV' structure. A pointer to 'XDRV_APM_FUNCS' must be the first
** member of the 'APM_3341_DRV' struct.
*/
typedef struct
{
   /* Must be first element of this struct. */
   const XDRV_APM_FUNCS          *pDrvFuncs;

   /* APM sim-specific state info. */

} APM_SIM_DRV;


/*
*****************************************************************************
** FUNCTION:   apmSimInit
**
** PURPOSE:    Initialize the APM
**
** PARAMETERS: pDev       - pointer to the APM info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void apmSimInit( APM_SIM_DRV *pDev );


/*
*****************************************************************************
** FUNCTION:   apmSimDeinit
**
** PURPOSE:    Shutdown the APM
**
** PARAMETERS: pDev  - pointer to the APM info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void apmSimDeinit( APM_SIM_DRV *pDev );


#if defined(__cplusplus)
}
#endif



#endif   /* APM_SIM_H */
