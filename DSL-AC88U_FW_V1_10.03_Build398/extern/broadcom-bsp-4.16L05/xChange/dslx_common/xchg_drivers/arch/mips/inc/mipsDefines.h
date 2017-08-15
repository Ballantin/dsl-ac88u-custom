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
*    Filename: mipsDefines.h
*
****************************************************************************
*    Description:
*
*     MIPS registers and associated constants.
*
****************************************************************************/

#ifndef MIPS_DEFINES_H
#define MIPS_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <mipsDefinesCfg.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


#define zero $0
#define AT $1
#define v0 $2
#define v1 $3
#define a0 $4
#define a1 $5
#define a2 $6
#define a3 $7
#define t0 $8
#define t1 $9
#define t2 $10
#define t3 $11
#define t4 $12
#define t5 $13
#define t6 $14
#define t7 $15
#define s0 $16
#define s1 $17
#define s2 $18
#define s3 $19
#define s4 $20
#define s5 $21
#define s6 $22
#define s7 $23
#define t8 $24
#define t9 $25
#define k0 $26
#define k1 $27
#define gp $28
#define sp $29
#define fp $30
#define s8 $30
#define ra $31

/* aliases for general registers */
#define r0     $0
#define  r1    $1    /* assembler temporaries */
#define  r2    $2    /* value holders */
#define  r3    $3
#define  r4    $4    /* arguments */
#define  r5    $5
#define  r6    $6
#define  r7    $7
#define  r8    $8    /* temporaries */
#define  r9    $9
#define  r10   $10
#define  r11   $11
#define  r12   $12
#define  r13   $13
#define  r14   $14
#define  r15   $15
#define  r16   $16      /* saved registers */
#define  r17   $17
#define  r18   $18
#define  r19   $19
#define  r20   $20
#define  r21   $21
#define  r22   $22
#define  r23   $23
#define  r24   $24      /* temporaries */
#define  r25   $25
#define  r26   $26      /* kernel registers */
#define  r27   $27
#define  r28   $28      /* global pointer */
#define  r29   $29      /* stack pointer */
#define  r30   $30      /* saved register */
#define  r31   $31      /* return address */

/* you can't use the usual register names in macros */
#define r_v0    2
#define r_v1    3
#define r_a0    4
#define r_a1    5
#define r_a2    6
#define r_a3    7
#define r_t0    8
#define r_t1    9
#define r_t2    10
#define r_t3    11
#define r_t4    12
#define r_t5    13
#define r_t6    14
#define r_t7    15
#define r_s0    16
#define r_s1    17
#define r_s2    18
#define r_s3    19
#define r_s4    20
#define r_s5    21
#define r_s6    22
#define r_s7    23

/* CP0 Register Defines */

#define C0_INDEX        $0
#define C0_RANDOM       $1
#define C0_ENTRYLO_0    $2
#define C0_ENTRYLO_1    $3
#define C0_CONTEXT      $4
#define C0_PAGEMASK     $5
#define C0_WIRED        $6
#define C0_BADVADDR     $8
#define C0_COUNT        $9
#define C0_ENTRYHI      $10
#define C0_COMPARE      $11
#define C0_STATUS       $12
#define C0_CAUSE        $13
#define C0_EPC          $14
#define C0_PRID         $15
#define C0_CONFIG       $16
#define C0_LLADDR       $17
#define C0_TAGLO        $28
#define C0_TAGHI        $29


#define C0_INDEX_NUM      0
#define C0_RANDOM_NUM     1
#define C0_ENTRYLO_0_NUM  2
#define C0_ENTRYLO_1_NUM  3
#define C0_CONTEXT_NUM    4
#define C0_PAGEMASK_NUM   5
#define C0_WIRED_NUM      6
#define C0_BADVADDR_NUM   8
#define C0_COUNT_NUM      9
#define C0_ENTRYHI_NUM    10
#define C0_COMPARE_NUM    11
#define C0_STATUS_NUM     12
#define C0_CAUSE_NUM      13
#define C0_EPC_NUM        14
#define C0_PRID_NUM       15
#define C0_CONFIG_NUM     16
#define C0_LLADDR_NUM     17
#define C0_TAGLO_NUM      28
#define C0_TAGHI_NUM      29


/* CP0 Status Defines */

#define C0_STATUS_IE    0x1
#define C0_STATUS_EXL   0x2
#define C0_STATUS_ERL   0x4



/* Cache op defines */

#define K_CachePriI             0                       /* Primary Icache */
#define K_CachePriD             1                       /* Primary Dcache */

#define S_CacheFunc             2                       /* Amount to shift function encoding within 5-bit field */
#define K_CacheIndexInv         0                       /* Index invalidate */
#define K_CacheIndexWBInv       0                       /* Index writeback invalidate */
#define K_CacheIndexLdTag       1                       /* Index load tag */
#define K_CacheIndexStTag       2                       /* Index store tag */
#define K_CacheHitInv           4                       /* Hit Invalidate */
#define K_CacheFill             5                       /* Fill (Icache only) */
#define K_CacheHitWBInv         5                       /* Hit writeback invalidate */
#define K_CacheHitWB            6                       /* Hit writeback */
#define K_CacheFetchLock        7                       /* Fetch and lock */

#define ICIndexInv              ((K_CacheIndexInv << S_CacheFunc) | K_CachePriI)
#define DCIndexWBInv            ((K_CacheIndexWBInv << S_CacheFunc) | K_CachePriD)
#define DCIndexInv              DCIndexWBInv
#define ICIndexLdTag            ((K_CacheIndexLdTag << S_CacheFunc) | K_CachePriI)
#define DCIndexLdTag            ((K_CacheIndexLdTag << S_CacheFunc) | K_CachePriD)
#define ICIndexStTag            ((K_CacheIndexStTag << S_CacheFunc) | K_CachePriI)
#define DCIndexStTag            ((K_CacheIndexStTag << S_CacheFunc) | K_CachePriD)
#define ICHitInv                ((K_CacheHitInv << S_CacheFunc) | K_CachePriI)
#define DCHitInv                ((K_CacheHitInv << S_CacheFunc) | K_CachePriD)
#define ICFill                  ((K_CacheFill << S_CacheFunc) | K_CachePriI)
#define DCHitWBInv              ((K_CacheHitWBInv << S_CacheFunc) | K_CachePriD)
#define DCHitWB                 ((K_CacheHitWB << S_CacheFunc) | K_CachePriD)
#define ICFetchLock             ((K_CacheFetchLock << S_CacheFunc) | K_CachePriI)
#define DCFetchLock             ((K_CacheFetchLock << S_CacheFunc) | K_CachePriD)

/* Exception Vectors */
#define GENERAL_EXCP_VECTOR     0x80000180

/* Segement and Address masks defines */

#define K0BASE          0x80000000
#define K1BASE          0xa0000000
#define K2BASE          0xc0000000
#define K3BASE          0xe0000000


/* Status Register */
#define SR_CUMASK 0xf0000000   /* Coprocessor usable bits */
#define SR_CU3    0x80000000   /* Coprocessor 3 usable */
#define SR_CU2    0x40000000   /* coprocessor 2 usable */
#define SR_CU1    0x20000000   /* Coprocessor 1 usable */
#define SR_CU0    0x10000000   /* Coprocessor 0 usable */
#define SR_IST    0x00800000   /* Isolate Tags */
#define SR_BEV    0x00400000   /* Bootstrap Exception Vector */
#define SR_TS     0x00200000   /* TLB shutdown */
#define SR_SR     0x00100000   /* Soft reset indication */
#define SR_NMI    0x00080000   /* NMI interrupt indication */
#define SR_ERL    0x00000004
#define SR_EXL    0x00000002
#define SR_IE     0x00000001

#define SR_IMASK  0x0000ff00  /* Interrupt Mask */
#define SR_IMASK8 0x00000000  /* Interrupt Mask level=8 */
#define SR_IMASK7 0x00008000  /* Interrupt Mask level=7 */
#define SR_IMASK6 0x0000c000  /* Interrupt Mask level=6 */
#define SR_IMASK5 0x0000e000  /* Interrupt Mask level=5 */
#define SR_IMASK4 0x0000f000  /* Interrupt Mask level=4 */
#define SR_IMASK3 0x0000f800  /* Interrupt Mask level=3 */
#define SR_IMASK2 0x0000fc00  /* Interrupt Mask level=2 */
#define SR_IMASK1 0x0000fe00  /* Interrupt Mask level=1 */
#define SR_IMASK0 0x0000ff00  /* Interrupt Mask level=0 */
#define SR_ISHIFT 8

#define SR_IBIT8  0x00008000  /*  (Intr5) */
#define SR_IBIT7  0x00004000  /*  (Intr4) */
#define SR_IBIT6  0x00002000  /*  (Intr3) */
#define SR_IBIT5  0x00001000  /*  (Intr2) */
#define SR_IBIT4  0x00000800  /*  (Intr1) */
#define SR_IBIT3  0x00000400  /*  (Intr0) */
#define SR_IBIT2  0x00000200  /*  (Software Interrupt 1) */
#define SR_IBIT1  0x00000100  /*  (Software Interrupt 0) */
#define SR_INT5   0x00008000  /*  (Intr5) */
#define SR_INT4   0x00004000  /*  (Intr4) */
#define SR_INT3   0x00002000  /*  (Intr3) */
#define SR_INT2   0x00001000  /*  (Intr2) */
#define SR_INT1   0x00000800  /*  (Intr1) */
#define SR_INT0   0x00000400  /*  (Intr0) */


/* Cause Register */
#define CAUSE_BD        0x80000000  /* Branch Delay */
#define CAUSE_CEMASK    0x30000000  /* Coprocessor Error */
#define CAUSE_CESHIFT   28          /* Right justify CE  */
#define CAUSE_IPMASK    0x0000ff00  /* Interrupt Pending */
#define CAUSE_IPSHIFT   8           /* Right justify IP  */
#define CAUSE_IP8       0x00008000  /*  (Intr5) */
#define CAUSE_IP7       0x00004000  /*  (Intr4) */
#define CAUSE_IP6       0x00002000  /*  (Intr3) */
#define CAUSE_IP5       0x00001000  /*  (Intr2) */
#define CAUSE_IP4       0x00000800  /*  (Intr1) */
#define CAUSE_IP3       0x00000400  /*  (Intr0) */
#define CAUSE_INT5      0x00008000  /*  (Intr5) */
#define CAUSE_INT4      0x00004000  /*  (Intr4) */
#define CAUSE_INT3      0x00002000  /*  (Intr3) */
#define CAUSE_INT2      0x00001000  /*  (Intr2) */
#define CAUSE_INT1      0x00000800  /*  (Intr1) */
#define CAUSE_INT0      0x00000400  /*  (Intr0) */
#define CAUSE_SW2       0x00000200  /*  (Software Int 1) */
#define CAUSE_SW1       0x00000100  /*  (Software Int 0) */
#define CAUSE_EXCMASK   0x0000003c  /* Exception Code */
#define CAUSE_EXCSHIFT  2           /* Right justify EXC */

/* Exception Code */
#define EXC_INT      (0 << 2)    /* External interrupt */
#define EXC_MOD      (1 << 2)    /* TLB modification */
#define EXC_TLBL     (2 << 2)    /* TLB miss (Load or Ifetch) */
#define EXC_TLBS     (3 << 2)    /* TLB miss (Save) */
#define EXC_ADEL     (4 << 2)    /* Addr error (Load or Ifetch) */
#define EXC_ADES     (5 << 2)    /* Address error (Save) */
#define EXC_IBE      (6 << 2)    /* Bus error (Ifetch) */
#define EXC_DBE      (7 << 2)    /* Bus error (data load/store) */
#define EXC_SYS      (8 << 2)    /* System call */
#define EXC_BP       (9 << 2)    /* Break point */
#define EXC_RI       (10 << 2)   /* Reserved instruction */
#define EXC_CPU      (11 << 2)   /* Coprocessor unusable */
#define EXC_OVF      (12 << 2)   /* Arithmetic overflow */
#define EXC_TR       (13 << 2)   /* Trap instruction */



/* --------------------------------------------------------------------------
** Macros to perform address conversions between different regions of the
** standard MIPS memory map.
*/

#ifndef K0_TO_K1
#define K0_TO_K1(x)     ((unsigned)(x)|0xA0000000)   /* kseg0 to kseg1 */
#endif

#ifndef K1_TO_K0
#define K1_TO_K0(x)     ((unsigned)(x)&0x9FFFFFFF)   /* kseg1 to kseg0 */
#endif

#ifndef K0_TO_PHYS
#define K0_TO_PHYS(x)   ((unsigned)(x)&0x1FFFFFFF)   /* kseg0 to physical */
#endif

#ifndef K1_TO_PHYS
#define K1_TO_PHYS(x)   ((unsigned)(x)&0x1FFFFFFF)   /* kseg1 to physical */
#endif

#ifndef PHYS_TO_K0
#define PHYS_TO_K0(x)   ((unsigned)(x)|0x80000000)   /* physical to kseg0 */
#endif

#ifndef PHYS_TO_K1
#define PHYS_TO_K1(x)   ((unsigned)(x)|0xA0000000)   /* physical to kseg1 */
#endif

#define  CacheToNonCacheVirtAddress    K0_TO_K1
#define  NonCacheToCacheVirtAddress    K1_TO_K0
#define  CacheVirtToPhyAddress         K0_TO_PHYS
#define  NonCacheVirtToPhyAddress      K1_TO_PHYS
#define  PhyToNonCacheVirtAddress      PHYS_TO_K1
#define  PhyToCacheVirtAddress         PHYS_TO_K0



/* --------------------------------------------------------------------------
** MIPS opcode and operand instruction defines. (This is only a partial list).
*/

/* The "major" opcode field of MIPS instructions - bits 31:26. */
#define  MIPS_OPCODE_MAJOR_MASK           0xfc000000
#define  MIPS_OPCODE_MAJOR_SHIFT          26

#define  MIPS_OPCODE_MAJOR_SPECIAL        0
#define  MIPS_OPCODE_MAJOR_REGIMM         1
#define  MIPS_OPCODE_MAJOR_J              2
#define  MIPS_OPCODE_MAJOR_JAL            3
#define  MIPS_OPCODE_MAJOR_BEQ            4
#define  MIPS_OPCODE_MAJOR_BNE            5
#define  MIPS_OPCODE_MAJOR_BLEZ           6
#define  MIPS_OPCODE_MAJOR_BGTZ           7
#define  MIPS_OPCODE_MAJOR_BEQL           20
#define  MIPS_OPCODE_MAJOR_BNEL           21
#define  MIPS_OPCODE_MAJOR_BLEZL          22
#define  MIPS_OPCODE_MAJOR_BGTZL          23

#define  MIPS_OPCODE_REGIMM_BLTZ          0
#define  MIPS_OPCODE_REGIMM_BGEZ          1
#define  MIPS_OPCODE_REGIMM_BLTZL         2
#define  MIPS_OPCODE_REGIMM_BGEZL         3
#define  MIPS_OPCODE_REGIMM_BLTZAL        16
#define  MIPS_OPCODE_REGIMM_BGEZAL        17
#define  MIPS_OPCODE_REGIMM_BLTZALL       18
#define  MIPS_OPCODE_REGIMM_BGEZALL       19


/* The "minor" opcode field of MIPS instructions - bits 5:0. */
#define  MIPS_OPCODE_MINOR_MASK           0x0000003f
#define  MIPS_OPCODE_MINOR_SHIFT          0

#define  MIPS_OPCODE_MINOR_JR             8
#define  MIPS_OPCODE_MINOR_JALR           9


/* Operand bit fields. */
#define  MIPS_OPERAND_RS_MASK             0x03e00000  /* Bits 25:21. */
#define  MIPS_OPERAND_RS_SHIFT            21

#define  MIPS_OPERAND_RT_MASK             0x001f0000  /* Bits 20:16. */
#define  MIPS_OPERAND_RT_SHIFT            16

#define  MIPS_OPERAND_RD_MASK             0x0000f800  /* Bits 15:11. */
#define  MIPS_OPERAND_RD_SHIFT            11

#define  MIPS_OPERAND_BROFFSET_MASK       0x0000ffff  /* Bits 15:0 - branch offset. */
#define  MIPS_OPERAND_BROFFSET_SHIFT      0

#define  MIPS_OPERAND_INSTR_INDEX_MASK    0x03ffffff  /* Bits 25:0 - instruction index. */
#define  MIPS_OPERAND_INSTR_INDEX_SHIFT   0

/* Miscellaneous defines */
#define  MIPS_CP0_CMT_PRIO_TP0            (0x1 << 4)
#define  MIPS_CP0_CMT_PRIO_TP1            (0x1 << 5)
#define  MIPS_BASE                        0xff400000
#define  MIPS_RAC_CR0                     0x0
#define  MIPS_RAC_CR1                     0x8
#define  MIPS_RAC_D                       0x2
#define  MIPS_RAC_PF_D                    0x8

/* --------------------------------------------------------------------------
**  Macros used to define assembly functions.
*/

#define ASM_START(name)          \
        .set push ;              \
        .globl name;             \
        .ent   name;             \
name:

#define ASM_END(name)            \
name##_end:                      \
        .set pop;                \
        .end name



#ifdef __cplusplus
    }
#endif

#endif  /* MIPS_DEFINES_H  */
