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
*    Filename: xdrvDlb.h
*
****************************************************************************
*    Description:
*
*     Public API for hardware-independent interface for dynamic load balancing.
*
****************************************************************************/

#ifndef XDRV_DLB_H
#define XDRV_DLB_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

/* --------------------------------------------------------------------------
** 'XDRV_DLB' is the base, generic DLB structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific DLB drivers.
**
** 'XDRV_DLB_FUNCS' must be the only structure member of 'XDRV_DLB'.
**
** Chip specific DLB drivers should provide a "derived" version of XDRV_DLB:
**
**    typedef struct DLB_CHIP_DRV
**    {
**       const XDRV_DLB_FUNCS  *drvFuncs;
**
**       int   chipSpecificData1;
**       int   chipSpecificData2;
**       ...
**
**    } DLB_CHIP_DRV;
**
** Clients, which have access to XDRV_DLB structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_DLB struct as the first parameter. This struct can be cast to the chip
** specific version of the DLB structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_DLB_FUNCS *pDrvFuncs;

} XDRV_DLB;


/* --------------------------------------------------------------------------
** DLB API function pointers.
*/

/*
*****************************************************************************
** FUNCTION:   xdrvDlbEnable
**
** PURPOSE:    Enable/disable dynamic load balancing.
**
** PARAMETERS: pDrv     (in)  Pointer to the DLB driver device structure.
**             enable   (in)
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvDlbEnable)( XDRV_DLB *pDrv, XDRV_BOOL enable );


/*
*****************************************************************************
** FUNCTION:   xdrvDlbIsEnabled
**
** PURPOSE:    Indicates current state of DLB (enabled or disable).
**
** PARAMETERS: pDrv     (in)  Pointer to the DLB driver device structure.
**
** RETURNS:    XDRV_TRUE or XDRV_FALSE.
**
*****************************************************************************
*/
typedef XDRV_BOOL (*xdrvDlbIsEnabled)( XDRV_DLB *pDrv );


/*
*****************************************************************************
** FUNCTION:   xdrvDlbDisplayStats
**
** PURPOSE:    Display load balancing statistics (for debug).
**
** PARAMETERS: pDrv     (in)  Pointer to the DLB driver device structure.
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
typedef void (*xdrvDlbDisplayStats)( XDRV_DLB *pDrv );


/*
*****************************************************************************
** FUNCTION:   xdrvDlbEnableStaticMode
**
** PURPOSE:    Enable/disable static load balancing, instead of dynamic.
**
** PARAMETERS: pDrv           (in)  Pointer to the DLB driver device structure.
**             enable         (in)
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
typedef void (*xdrvDlbEnableStaticMode)( XDRV_DLB *pDrv, XDRV_BOOL enable );


/*
*****************************************************************************
** FUNCTION:   xdrvDlbSetThrottleDuration
**
** PURPOSE:    Set the throttleing duration used by the DLB algorithm. This
**             is really an implementation specific parameter, and probably
**             doesn't belong in the generic API.
**
** PARAMETERS: pDrv           (in)  Pointer to the DLB driver device structure.
**             durationMsec   (in)  Throttling duration used by DLB algorithm.
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
typedef void (*xdrvDlbSetThrottleDuration)( XDRV_DLB *pDrv, unsigned int durationMsec );



/*
** Structure that contains all function pointers that define the API.
*/
typedef struct XDRV_DLB_FUNCS
{
   xdrvDlbEnable                 dlbEnable;
   xdrvDlbIsEnabled              dlbIsEnabled;
   xdrvDlbDisplayStats           dlbDisplayStats;
   xdrvDlbEnableStaticMode       dlbEnableStaticMode;
   xdrvDlbSetThrottleDuration    dlbSetThrottleDuration;

} XDRV_DLB_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the DLB.
*/

#define xdrvDlbEnable( pDrv, enable ) \
         ( pDrv->pDrvFuncs->dlbEnable( (pDrv), (enable) ) )

#define xdrvDlbIsEnabled( pDrv ) \
         ( pDrv->pDrvFuncs->dlbIsEnabled( (pDrv) ) )

#define xdrvDlbDisplayStats( pDrv ) \
         ( pDrv->pDrvFuncs->dlbDisplayStats( (pDrv) ) )

#define xdrvDlbEnableStaticMode( pDrv, enable ) \
         ( pDrv->pDrvFuncs->dlbEnableStaticMode( (pDrv), (enable) ) )

#define xdrvDlbSetThrottleDuration( pDrv, value ) \
         ( pDrv->pDrvFuncs->dlbSetThrottleDuration( (pDrv), (value) ) )


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_DLB_H  */
