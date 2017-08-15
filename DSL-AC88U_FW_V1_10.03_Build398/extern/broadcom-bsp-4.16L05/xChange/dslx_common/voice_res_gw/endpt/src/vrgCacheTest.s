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
*    Filename: vrgCacheTest.s
*
****************************************************************************
*
*    Description: Code for flushing D-Cache (Copied from CFE)
*
****************************************************************************/

##-----------------------------------------------------------------------------
## 

#include <mipsDefines.h>   

   
#ifdef at
#undef at
#endif

#define Index_Load_Tag_D     0x05
#define Index_Store_Tag_D    0x09
#define Index_Writeback_Inv_D   0x01

#define CP0_CFG_ISMSK      (0x7 << 22)
#define CP0_CFG_ISSHF      22
#define CP0_CFG_ILMSK      (0x7 << 19)
#define CP0_CFG_ILSHF      19
#define CP0_CFG_IAMSK      (0x7 << 16)
#define CP0_CFG_IASHF      16
#define CP0_CFG_DSMSK      (0x7 << 13)
#define CP0_CFG_DSSHF      13
#define CP0_CFG_DLMSK      (0x7 << 10)
#define CP0_CFG_DLSHF      10
#define CP0_CFG_DAMSK      (0x7 << 7)
#define CP0_CFG_DASHF      7

#define cacheop(kva, size, linesize, op) \
        .set    noreorder;       \
        addu    t1, kva, size;   \
        subu    t2, linesize, 1; \
        not     t2;              \
        and     t0, kva, t2;     \
        addu    t1, -1;          \
        and     t1, t2;          \
10:     cache   op, 0(t0);       \
        bne     t0, t1, 10b;     \
        addu    t0, linesize;    \
11:                              \
        .set    reorder



#define size_dcache(size, linesize) \
        mfc0    t7, C0_CONFIG, 1;       \
        and     t0, t7, CP0_CFG_DLMSK;  \
        srl     t0, t0, CP0_CFG_DLSHF;  \
        move    linesize, zero;         \
        beq     t0, zero,1f;            \
        add     t0, 1;                  \
        li      linesize, 1;            \
        sll     linesize, t0;           \
1:      and     t0, t7, CP0_CFG_DSMSK;  \
        srl     t0, t0, CP0_CFG_DSSHF;  \
        li      size, 64;               \
        sll     size, t0;               \
        and     t0, t7, CP0_CFG_DAMSK;  \
        srl     t0, t0, CP0_CFG_DASHF;  \
        add     t0, 1;                  \
        mult    size, t0;               \
        mflo    size;                   \
        mult    size, linesize;         \
        mflo    size


##-----------------------------------------------------------------------------
## Startup code
   
   .set   noreorder
   .text


ASM_START(vrgDCacheInvalidate)
   .set noreorder

        li      a0, K0BASE
        size_dcache(a1, a2)

# before flushing cache clear tags pointing to flash memory to avoid writes into flash
        addu    t1, a0, a1
        subu    t2, a2, 1
        not     t2
        and     t0, a0, t2
        addu    t1, -1
        and     t1, t2
1:
        cache   Index_Load_Tag_D, 0(t0)
        nop
        nop
        nop
        nop
        nop
        nop
        mfc0    t2, C0_TAGLO         # Read TAGLO register
        and     t2, 0x1f000000       # check address
        li      t3, 0x1f000000
        bne     t2, t3, 2f
        mtc0    zero, C0_TAGLO
        cache   Index_Store_Tag_D, 0(t0)    # Reset tag for flash memory locations
2:
        .set noreorder;
        bne     t0, t1, 1b
        addu    t0, a2
        .set reorder

        cacheop(a0, a1, a2, Index_Writeback_Inv_D)

		j	ra

ASM_END(vrgDCacheInvalidate)


