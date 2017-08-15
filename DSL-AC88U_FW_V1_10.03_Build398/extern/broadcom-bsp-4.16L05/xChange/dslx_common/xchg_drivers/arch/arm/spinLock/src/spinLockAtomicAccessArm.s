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
*    Filename: spinLockAtomicAccessArm.s
*
****************************************************************************
*    Description:
*
*     Implementation for ARM-specific functions that perform atomic
*     test-modify-updates of spin-lock flags.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   spinLockAcquireArm
**
** PURPOSE:    To acquire a spin-lock object. If the spin-lock has already been
**             acquired by another processor, this function will block (spin)
**             until the spin-lock is released.
**
** PARAMETERS: spinLock (mod)    Spin-lock to acquire.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
**             XDRV_STATUS spinLockAcquireArm( XDRV_SPIN_LOCK *spinLock );
*****************************************************************************
*/
.global spinLockAcquireArm
.global spinLockReleaseArm

spinLockAcquireArm:

   mov      r1, #1         /* Set 'lock taken' value */

spa_loop:
   ldrex    r2, [r0]       /* Exclusively load lock value */
   cmp      r2, #0         /* Check if lock is free */
   strexeq  r2, r1, [r0]   /* Try to claim lock */
   cmpeq    r2, #0         /* If exclusive lock failed */
   bne      spa_loop
   dmb                     /* Memory barrier */

   /* Return success. */
   mov      r0, #XDRV_STATUS_OK
   mov      pc, lr         /* Return */



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
**             void spinLockReleaseArm( XDRV_SPIN_LOCK *spinLock );
*****************************************************************************
*/
spinLockReleaseArm:

   mov   r1, #0      /* Set lock to 0 */
   dmb               /* Memory barrier */
   str   r1, [r0]

   mov   pc, lr   /* Return */

