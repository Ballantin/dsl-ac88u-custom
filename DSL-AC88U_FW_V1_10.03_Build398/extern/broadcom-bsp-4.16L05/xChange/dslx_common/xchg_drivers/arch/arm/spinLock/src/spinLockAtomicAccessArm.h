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
*    Filename: spinLockAtomicAccessArm.h
*
****************************************************************************
*    Description:
*
*     Interface for ARM-specific functions that perform atomic
*     test-modify-updates of spin-lock flags.
*
****************************************************************************/

#ifndef SPIN_LOCK_ATOMIC_ACCESS_ARM_H
#define SPIN_LOCK_ATOMIC_ACCESS_ARM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvSpinLock.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   spinLockAcquireArm
**
** PURPOSE:    To acquire a spin-lock object. If the spin-lock has already been
**             acquired by another processor, this function will block (spin)
**             until the spin-lock is released.
**
**             This function uses the ARM-specific instructions to perform
**             atomic test-modify-updates of the spin-lock object.
**
** PARAMETERS: spinLock (mod)    Spin-lock to acquire.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS spinLockAcquireArm( XDRV_SPIN_LOCK *spinLock );


/*
*****************************************************************************
** FUNCTION:   spinLockReleaseArm
**
** PURPOSE:    To release a previously acquired spin-lock. This will make the
**             spin-lock available to other processors.
**
** PARAMETERS: spinLock (mod)    Spin-lock to release.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void spinLockReleaseArm( XDRV_SPIN_LOCK *spinLock );


#ifdef __cplusplus
    }
#endif

#endif  /* SPIN_LOCK_ATOMIC_ACCESS_ARM_H  */
