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
*    Filename: dlbStub.h
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      dynamic load balancing (DLB) API.
*
****************************************************************************/

#ifndef DLB_STUB_H
#define DLB_STUB_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <xdrvDlb.h>


/*
** Stub dynamic load balancing (DLB) driver structure. Conceptually, this
** structure is similar to a C++ sub-class that is derived from the 'XDRV_DLB'
** base-class(structure). The base DLB structure specifies a set of function
** pointers that defines the generic hardware-independent API that must be
** provided by all chip specific DLB drivers.
**
** When the stub DLB driver initialization function is invoked, the base DLB
** structure function pointers will be intialized to a set of hardware specific
** DLB driver functions. Therefore, given a pointer to the base DLB driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific DLB driver function.
**
** Stub DLB driver state that is specific to the stub should be stored in the
** 'DLB_stub_DRV' structure. A pointer to 'XDRV_DLB_FUNCS' must be the first
** member of the 'DLB_STUB_DRV' struct.
*/
typedef struct
{
   /* Must be first element of this struct. */
   const XDRV_DLB_FUNCS          *pDrvFuncs;

   /* DLB stub-specific state info. */

} DLB_STUB_DRV;


/*
*****************************************************************************
** FUNCTION:   dlbStubCreate
**
** PURPOSE:    Initialize the DLB
**
** PARAMETERS: pDev       - pointer to the DLB info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void dlbStubCreate( DLB_STUB_DRV *pDev );


/*
*****************************************************************************
** FUNCTION:   dlbStubDestroy
**
** PURPOSE:    Shutdown the DLB
**
** PARAMETERS: pDev  - pointer to the DLB info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void dlbStubDestroy( DLB_STUB_DRV *pDev );


#if defined(__cplusplus)
}
#endif



#endif   /* DLB_STUB_H */
