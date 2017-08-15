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
*    Filename: spinLockAtomicAccessMips.s
*
****************************************************************************
*    Description:
*
*     Implementation for MIPS-specific functions that perform atomic
*     test-modify-updates of spin-lock flags. This is accomplished using
*     the MIPS ll (load-linked) and sc (store-conditional) instructions. sc
*     will only write the addressed location if there has been no competing
*     access since the last ll and will leave a 1/0 value in a register to
*     indicate success or failure.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <mipsDefines.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

    
   .text
   
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
**             XDRV_STATUS spinLockAcquireMips( XDRV_SPIN_LOCK *spinLock );
*****************************************************************************
*/
ASM_START( spinLockAcquireMips )   
   .set noreorder

   /* Prime return-value register with failure code (for timeouts). */
   li    v0, XDRV_STATUS_ERR
   
   /* We don't want to get stuck here spinning forever, so we'll have a re-try
   ** count. If this decrements to 0, then we'll give-up trying to acquire
   ** the spin-lock. In theory, this should not happen. Spin-locks should be
   ** held for very, very short periods of time (with interrupts disabled). */
   li    t0, 100000
1:
   addiu t0, -1
   bltz  t0, 2f
   
   /* Spin until the spin-lock is available (has a value of 0). Once, it's 
   ** available, attempt to acquire it (set it's value to 1). This is performed
   ** using ll (load-linked) and sc (store-conditional) instructions. sc
   ** will only write the addressed location if there has been no competing
   ** access since the last ll and will leave a 1/0 value in the specified 
   ** register to indicate success or failure. This provides an atomic
   ** read-modify-update of the spin-lock flag. */
   
   ll    t1, 0(a0)      /* Branch delay slot. No harm done if branch is taken. */
   bne   t1, zero, 1b
   nop
   
   li    t1, 1   
   sc    t1, 0(a0)
   beq   t1, zero, 1b
   nop
   
   /* Return success. */
   li    v0, XDRV_STATUS_OK
   
2:   
   jr    ra
   nop

ASM_END( spinLockAcquireMips )   



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
**             void spinLockReleaseMips( XDRV_SPIN_LOCK *spinLock );
*****************************************************************************
*/
ASM_START( spinLockReleaseMips )   
   .set noreorder
   
   /* Not much to do here, just clear the spin-lock flag. */   
   sw    zero, 0(a0)
   
   jr    ra
   nop

ASM_END( spinLockReleaseMips )   
