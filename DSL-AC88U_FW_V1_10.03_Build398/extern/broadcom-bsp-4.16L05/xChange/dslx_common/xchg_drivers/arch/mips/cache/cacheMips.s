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
*    Filename: cacheMips.s
*
****************************************************************************
*    Description:
*
*     MIPS-specific implementation of cache operations.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <mipsDefines.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

    
   .text

/*
*****************************************************************************
** FUNCTION:   xdrvDCacheClearByAddress
**
** PURPOSE:    Clears data stored in the cache of the given address range.
**             Data may be flushed and/or invalidated depending on the given mask.
**
** PARAMETERS: clearMask   (in)  Flush, invalidate, or both.
**             startAddr   (in)  Start address to clear.
**             lengthBytes (in)  Length in bytes of the address range to clear.
**
** RETURNS:    Nothing.
**
** NOTE:
**             void xdrvDCacheClearByAddress
**             (
**                unsigned int   clearMask,
**                void          *startAddr,
**                unsigned int   lengthBytes
**             );
** 
*****************************************************************************
*/
    .globl  xdrvDCacheClearByAddress
    .ent xdrvDCacheClearByAddress
xdrvDCacheClearByAddress:
    .set noreorder

    # DPullen - July 18, 2003; completely rewrote this function to
    # optimize for size and speed.  I was able to get more than 10%
    # improvement, and shrank the code by more than 50%.

#if 0 //!(CACHE_WRITEBACK_MODE)
    # NOTE:  If the dcache is in writethrough mode, then there is
    # no need to flush cache (and in fact it may not work correctly).
    # If we're being told to flush cache, then just bail.  If we're
    # being told to flush and invalidate, then just do the invalidate
    # part.
    li      t2, 1
    beq     a0, t2, 2f
    nop
#endif

    # Skip all this if the size is 0 or negative; nothing to do in
    # this case.  The addiu following the branch will be executed,
    # but it just modifies a temp register.
    blez    a2, 2f

    # Push the start address back to the beginning of the cache line by masking
    # off the lower bits.
    addiu   t2, zero, -DCACHE_LINE_SIZE_BYTES
    and     t0, a1, t2

    # Calculate the address that we need to stop at and push the end address
    # back to the beginning of that cache line.  Note that this cache line
    # will not be modified; we will stop after doing the line before this one.
    addu    t1, a1, a2
    addiu   t1, t1, DCACHE_LINE_SIZE_BYTES - 1
    and     t1, t1, t2

#if 1 //(CACHE_WRITEBACK_MODE)
    # NOTE:  If the dcache is in writethrough mode, then there really is
    # only one thing to do - invalidate.  The code in this block will be
    # compiled out and only the invalidate loop will be left, which will
    # be executed.

    # Branch to the appropriate loop based on the first parameter that was
    # passed in.  We need separate loops because the cache op value must be
    # specified as a constant.
    li      t2, 1
    beq     a0, t2, 20f
    li      t2, 2          # Branch delay; this will be ignored if we take the branch.
    beq     a0, t2, 31f

    # The addiu below will be executed; this is fine because we are skipping
    # the addiu at branch label 30, going directly to the bne at label 31.

    # Handle flush/invalidate here.
10: addiu   t0, t0, DCACHE_LINE_SIZE_BYTES
    bne     t0, t1, 10b
    cache   0x15, -DCACHE_LINE_SIZE_BYTES(t0)

    jr      ra

    # The addiu below will be executed, but the result will be ignored.

    # Handle flush here.
20: addiu   t0, t0, DCACHE_LINE_SIZE_BYTES
    bne     t0, t1, 20b
    cache   0x19, -DCACHE_LINE_SIZE_BYTES(t0)

    jr      ra

    # The addiu below will be executed, but the result will be ignored.

#endif

    # Handle invalidate here.
30: addiu   t0, t0, DCACHE_LINE_SIZE_BYTES
31: bne     t0, t1, 30b
    cache   0x11, -DCACHE_LINE_SIZE_BYTES(t0)

2:  jr      ra
    nop

    .set reorder
    .end xdrvDCacheClearByAddress

