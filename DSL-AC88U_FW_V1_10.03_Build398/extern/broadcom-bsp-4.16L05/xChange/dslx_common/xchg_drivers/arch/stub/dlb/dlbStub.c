/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: dlbStub.c
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      dynamic load balancing (DLB) API.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <dlbStub.h>
#include <string.h>
#include <xdrvLog.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */

static void DisplayStats( XDRV_DLB *drv );
static void SetThrottleDuration( XDRV_DLB *drv, unsigned int durationMsec );
static void EnableLoadBalancing( XDRV_DLB *drv, XDRV_BOOL enable );
static XDRV_BOOL IsLoadBalancingEnabled( XDRV_DLB *drv );
static void EnableStaticLoadBalancing( XDRV_DLB *drv, XDRV_BOOL enable );



/* DLB "meta-driver" interface */
static const XDRV_DLB_FUNCS  gDlbStubDrvFuncs =
{
   EnableLoadBalancing,
   IsLoadBalancingEnabled,
   DisplayStats,
   EnableStaticLoadBalancing,
   SetThrottleDuration
};


/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   dlbStubCreate
**
** PURPOSE:    Instantiate a stub dynamic load balancer (DLB) driver.
**
** PARAMETERS: throttleDurationMsec (in)  Duration to throttle access to shared
**                                        hardware resources.
**             itpcDrv              (in)  Inter-thread processor communication
**                                        driver used to communication with other
**                                        thread processor.
**             drv                  (out) Load balancer driver object.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void dlbStubCreate( DLB_STUB_DRV  *drv )
{
   memset( drv, 0, sizeof( DLB_STUB_DRV ) );

   /* Set the "meta-driver" interface */
   drv->pDrvFuncs = &gDlbStubDrvFuncs;

}


/*
*****************************************************************************
** FUNCTION:   dlbStubDestroy
**
** PURPOSE:    To destroy a previously created load balancer driver.
**
** PARAMETERS: drv   (mod) Load balancer driver object.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void dlbStubDestroy( DLB_STUB_DRV *drv )
{
   (void) drv;
}


/*
*****************************************************************************
** FUNCTION:   EnableLoadBalancing
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
static void EnableLoadBalancing( XDRV_DLB *dlbDrv, XDRV_BOOL enable )
{
   (void) dlbDrv;
   (void) enable;
}


/*
*****************************************************************************
** FUNCTION:   IsLoadBalancingEnabled
**
** PURPOSE:    Indicates current state of DLB (enabled or disable).
**
** PARAMETERS: pDrv     (in)  Pointer to the DLB driver device structure.
**
** RETURNS:    XDRV_TRUE or XDRV_FALSE.
**
*****************************************************************************
*/
static XDRV_BOOL IsLoadBalancingEnabled( XDRV_DLB *dlbDrv )
{
   (void) dlbDrv;

   return ( XDRV_FALSE );
}



/*
*****************************************************************************
** FUNCTION:   EnableStaticLoadBalancing
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
static void EnableStaticLoadBalancing( XDRV_DLB *dlbDrv, XDRV_BOOL enable )
{
   (void) dlbDrv;
   (void) enable;
}


/*
*****************************************************************************
** FUNCTION:   SetThrottleDuration
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
static void SetThrottleDuration( XDRV_DLB *dlbDrv, unsigned int durationMsec )
{
   (void) dlbDrv;
   (void) durationMsec;
}


/*
*****************************************************************************
** FUNCTION:   DisplayStats
**
** PURPOSE:    Display load balancing statistics (for debug).
**
** PARAMETERS: pDrv     (in)  Pointer to the DLB driver device structure.
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
static void DisplayStats( XDRV_DLB *dlbDrv )
{
   (void) dlbDrv;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "No stats available, using stub load balancer!" ));
}
