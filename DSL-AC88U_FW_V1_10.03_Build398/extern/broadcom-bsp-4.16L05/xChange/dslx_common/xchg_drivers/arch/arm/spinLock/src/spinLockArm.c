/***************************************************************************
*    Copyright 2013  Broadcom Corporation
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
*    Filename: spinLockArm.c
*
****************************************************************************
*    Description:
*
*     Implements ARM-specific spin-locks. Implements to the generic spin-lock
*     API defined by xdrvSpinLock.h. A spin-lock is a synchronization object
*     used to provide protection against concurrent access of data structures
*     shared by more than one processor. It relies upon architecture-specific
*     instructions to perform atomic read-modify-writes.
*
*     Other methods of providing concurrency protection typically used on
*     uniprocessor systems (interrupt disable, mutex etc) are not sufficient
*     on multiprocessor systems. These traditional primitives can only provide
*     protection on a single processor.
*
*     The spin-lock is basically a flag, that is initially clear. When the
*     spin-lock is acquired, the flag is set. And when it is released, the flag
*     is cleared. If the flag is already set when an acquire is performed,
*     the caller will block (spin) until is is released by the owning processor.
*     For the spin-lock to work correctly, the test-modify-update of the flag
*     in the acquire function must be performed atomically.
*
*     It is important that the code protected by the spin-lock is extremely
*     short, since other processors attempting to acquire the lock will spin
*     until the lock is released.
*
*     It is also important to prevent the owner of a spin-lock from being
*     preempted (either by an interrupt, or context switch). This could result
*     in other processors spinning for long periods of time, if one processor
*     owns the spin-lock, but isn't executing the code protected by the lock.
*     To prevent this situation, interrupts are disabled prior to acquiring
*     the spin-lock, and enabled after releasing the spin-lock.
*
*     Spin-locks are not nestable. Nested calls will likely result in deadlock.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvSpinLock.h>
#include <xdrvGlobalInterrupt.h>
#include "spinLockAtomicAccessArm.h"
#include <kernelMainWrapper.h>
#include <linux/spinlock.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
unsigned long slirqflags;
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   xdrvSpinLockCreate
**
** PURPOSE:    To create a spin-lock synchronization primitive.
**
** PARAMETERS: spinLock (out)    Spin-lock to create. Memory allocated by user.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvSpinLockCreate( XDRV_SPIN_LOCK *spinLock )
{
   spin_lock_init( (spinlock_t *) spinLock );
}


/*
*****************************************************************************
** FUNCTION:   xdrvSpinLockDestroy
**
** PURPOSE:    Destroys a previously created spin-lock object.
**
** PARAMETERS: spinLock (mod)    Spin-lock to destroy.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvSpinLockDestroy( XDRV_SPIN_LOCK *spinLock )
{
}


/*
*****************************************************************************
** FUNCTION:   xdrvSpinLockAcquire
**
** PURPOSE:    To acquire a spin-lock object. If the spin-lock has already been
**             acquired by another processor, this function will block (spin)
**             until the spin-lock is released.
**
** PARAMETERS: spinLock (mod)    Spin-lock to acquire.
**             intLock  (out)    Lock used to disable interrupts prior to
**                               acquiring the spin-lock. This prevents the owner
**                               of the spin-lock from being preempted.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvSpinLockAcquire
(
   XDRV_SPIN_LOCK               *spinLock,
   XDRV_GLOBAL_INTERRUPT_LOCK   *intLock
)
{
   spin_lock_irqsave( (spinlock_t*) spinLock, slirqflags );

   return XDRV_STATUS_OK;
}


/*
*****************************************************************************
** FUNCTION:   xdrvSpinLockRelease
**
** PURPOSE:    To release a previously acquired spin-lock. This will make the
**             spin-lock available to other processors.
**
** PARAMETERS: spinLock (mod)    Spin-lock to release.
**             intLock  (mod)    Lock used to re-able interrupts after releasing
**                               the spin-lock.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvSpinLockRelease
(
   XDRV_SPIN_LOCK               *spinLock,
   XDRV_GLOBAL_INTERRUPT_LOCK   *intLock
)
{
   (void)intLock;
   spin_unlock_irqrestore( (spinlock_t*) spinLock, slirqflags );
}


/*
*****************************************************************************
** FUNCTION:   xdrvSpinLockAcquireNoInt
**
** PURPOSE:    To acquire a spin-lock object. If the spin-lock has already been
**             acquired by another processor, this function will block (spin)
**             until the spin-lock is released.
**
**             This function is an optimized version of xdrvSpinLockAcquire().
**             It does not disabled interrupts. It can be used in situations
**             where it is known that interrupts are already disabled,
**             e.g. in ISRs.
**
**
** PARAMETERS: spinLock (mod)    Spin-lock to acquire.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvSpinLockAcquireNoInt( XDRV_SPIN_LOCK *spinLock )
{
   spin_lock( (spinlock_t*) spinLock );
   return XDRV_STATUS_OK;
}


/*
*****************************************************************************
** FUNCTION:   xdrvSpinLockReleaseNoInt
**
** PURPOSE:    To release a previously acquired spin-lock. This will make the
**             spin-lock available to other processors.
**
**             This function is an optimized version of xdrvSpinLockRelease().
**             It does not re-enable interrupts. It can be used in situations
**             where it is known that interrupts were already disabled prior
**             to calling xdrvSpinLockAcquireNoInt(), e.g. in ISRs.
**
** PARAMETERS: spinLock (mod)    Spin-lock to release.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvSpinLockReleaseNoInt( XDRV_SPIN_LOCK *spinLock )
{
   spin_unlock( (spinlock_t*) spinLock );
}
