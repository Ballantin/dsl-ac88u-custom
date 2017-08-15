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
*    Filename: dlb6368.h
*
****************************************************************************
*    Description:
*
*     Interface for 6368-specific dynamic load balancing between MIPS
*     thread-processors. The thread-processors have a number of shared hardware
*     resources (data-cache etc). If the "DSP" thread-processor approaches 100%
*     utilization (or passes 100%), the primary thread processor should be
*     throttled. This should prevent DSP overrun conditions which would
*     otherwise result in poor voice quality. The throttling mechanism
*     should prevent the primary thread processor from accessing shared
*     resources for a specific (and short) amount of time.
*
*
****************************************************************************/

#ifndef DLB_6368_H
#define DLB_6368_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvDlb.h>
#include <xdrvItpc.h>
#include <bosSem.h>
#include <bosTask.h>


/* ---- Constants and Types ---------------------------------------------- */

/*
** 6368 dynamic load balancing (DLB) driver structure. Conceptually, this
** structure is similar to a C++ sub-class that is derived from the 'XDRV_DLB'
** base-class(structure). The base DLB structure specifies a set of function
** pointers that defines the generic hardware-independent API that must be
** provided by all chip specific DLB drivers.
**
** When the 6368 DLB driver initialization function is invoked, the base DLB
** structure function pointers will be intialized to a set of hardware specific
** DLB driver functions. Therefore, given a pointer to the base DLB driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific DLB driver function.
**
** 3368 DLB driver state that is specific to the 3368 should be stored in the
** 'DLB_6368_DRV' structure. A pointer to 'XDRV_DLB_FUNCS' must be the first
** member of the 'DLB_6368_DRV' struct.
*/
typedef struct
{
   /* ---------- Must be first element of this struct. --------------------*/
   const XDRV_DLB_FUNCS   *pDrvFuncs;


   /* ---------- 6368 DLB-specific state info. --------------------------- */

   /* Sema4 used by the ISR to signal task that an load balancing is required. */
   BOS_SEM                 loadBalanceSema4;


   /* Interrupt service task signalled by ISR. */
   BOS_TASK_ID             loadBalanceTaskId;


   /* Inter-thread processor communication driver used to communication with other
   ** thread processor. */
   XDRV_ITPC              *itpcDrv;


   /* Duration to throttle access to shared hardware resources. */
   unsigned int            throttleDurationMsec;


   /* Current state of load balancing. */
   XDRV_BOOL               isEnabled;


   /* Statistics that track the number of times load balancing is performed. */
   unsigned int            currLoadBalanceCount;
   unsigned int            prevLoadBalanceCount;

} DLB_3368_DRV;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   dlb6368Create
**
** PURPOSE:    Instantiate a 6368 dynamic load balancer (DLB) driver.
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
void dlb6368Create
(
   unsigned int   throttleDurationMsec,
   XDRV_ITPC     *itpcDrv,
   DLB_6368_DRV  *drv
);


/*
*****************************************************************************
** FUNCTION:   dlb6368Destroy
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
void dlb3368Destroy( DLB_6368_DRV *drv );



#ifdef __cplusplus
    }
#endif

#endif  /* DLB_6368_H  */
