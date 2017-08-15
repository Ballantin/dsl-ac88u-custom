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
*    Filename: spinLockAtomicAccessMips.h
*
****************************************************************************
*    Description:
*
*     Interface for MIPS-specific functions that perform atomic
*     test-modify-updates of spin-lock flags. This is accomplished using
*     the MIPS ll (load-linked) and sc (store-conditional) instructions. sc
*     will only write the addressed location if there has been no competing
*     access since the last ll and will leave a 1/0 value in a register to
*     indicate success or failure.
*
****************************************************************************/

#ifndef SPIN_LOCK_ATOMIC_ACCESS_MIPS_H
#define SPIN_LOCK_ATOMIC_ACCESS_MIPS_H

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
** FUNCTION:   spinLockAcquireMips
**
** PURPOSE:    To acquire a spin-lock object. If the spin-lock has already been
**             acquired by another processor, this function will block (spin)
**             until the spin-lock is released.
**
**             This function uses the MIPS-specific ll/sc instructions to
**             perform atomic test-modify-updates of the spin-lock object.
**
** PARAMETERS: spinLock (mod)    Spin-lock to acquire.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS spinLockAcquireMips( XDRV_SPIN_LOCK *spinLock );


/*
*****************************************************************************
** FUNCTION:   spinLockReleaseMips
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
void spinLockReleaseMips( XDRV_SPIN_LOCK *spinLock );


#ifdef __cplusplus
    }
#endif

#endif  /* SPIN_LOCK_ATOMIC_ACCESS_MIPS_H  */
