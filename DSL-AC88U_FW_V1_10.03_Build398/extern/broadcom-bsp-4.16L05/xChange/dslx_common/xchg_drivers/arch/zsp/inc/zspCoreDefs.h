/****************************************************************************
*
*  zspregs.h - ZSP164xx Register Defintions
*
*  PURPOSE:
*     This file provides definitions for each register found on the ZSP164xx
*     platform of DSP's.
*
**********************************************************************/

#ifndef ZSPCORE_DEFS_H
#define ZSPCORE_DEFS_H

/****************************************************************************
**
** CONTROL REGISTERS
**
****************************************************************************/

/***************************************************************************
*  C0: Functional Mode (fmode) Definitions
***************************************************************************/
#define  FMODE_MRE_BMASK   0x0001   /* MAC round enable */
#define  FMODE_SRE_BMASK   0x0002   /* Shift round enable */
#define  FMODE_Q15_BMASK   0x0004   /* Fixed-point format = q15 */
#define  FMODE_Q14_BMASK   0x0008   /* Fixed-point format = q14 */
#define  FMODE_SAT_BMASK   0x0010   /* Enable saturation on arithmetic ops */
#define  FMODE_REZ_BMASK   0x0020   /* Round Zero Enable */

#define  FMODE_MRE_BPOS         0   /* MRE bit position in reg  */
#define  FMODE_SRE_BPOS         1   /* SRE bit position in reg  */
#define  FMODE_Q15_BPOS         2   /* Q15 bit position in reg  */
#define  FMODE_Q14_BPOS         3   /* Q14 bit position in reg  */
#define  FMODE_SAT_BPOS         4   /* SAT bit position in reg  */
#define  FMODE_REZ_BPOS         5   /* REZ bit position in reg  */

/***************************************************************************
*  C1: Timer Control (tc) Definitions
***************************************************************************/
#define  TC_TMRDIV0_BMASK  0x003F   /* 6-bit Prescale value for timer 0 */
#define  TC_CM0_BMASK      0x0040   /* Control Mode for timer 0 */
#define  TC_ET0_BMASK      0x0080   /* Enable Timer 0 */
#define  TC_TMRDIV1_BMASK  0x003F   /* 6-bit Prescale value for timer 1 */
#define  TC_CM1_BMASK      0x0040   /* Control Mode for timer 1 */
#define  TC_ET1_BMASK      0x0080   /* Enable Timer 1 */

#define  TC_CM0_BPOS            6   /* CM0 bit position */
#define  TC_ET0_BPOS            7   /* ET0 bit position */
#define  TC_CM1_BPOS           14   /* CM1 bit position */
#define  TC_ET1_BPOS           15   /* ET1 bit position */

/***************************************************************************
*  C2: Interrupt Mask (imask) Definitions
***************************************************************************/
#define  IMASK_ME0_BMASK   0x0001   /* Mask external interrupt 0 (INT0) */
#define  IMASK_ME1_BMASK   0x0002   /* Mask external interrupt 1 (INT1) */
#define  IMASK_ME2_BMASK   0x0004   /* Mask external interrupt 2 (INT2) */
#define  IMASK_ME3_BMASK   0x0008   /* Mask external interrupt 3 (INT3) */
#define  IMASK_ME4_BMASK   0x0010   /* Mask external interrupt 4 (INT4) */
#define  IMASK_MT0_BMASK   0x0020   /* Mask timer0 interrupt (t0) */
#define  IMASK_MT1_BMASK   0x0040   /* Mask timer1 interrupt (t1) */
#define  IMASK_MD_BMASK    0x0080   /* Mask DMA interrupt */
#define  IMASK_M0T_BMASK   0x0100   /* Mask serial port0 transmit int (s0t) */
#define  IMASK_M0R_BMASK   0x0200   /* Mask serial port0 receive int (s0r) */
#define  IMASK_M1T_BMASK   0x0400   /* Mask serial port1 transmit int (s1t) */
#define  IMASK_M1R_BMASK   0x0800   /* Mask serial port1 receive int (s1r) */
#define  IMASK_MHT_BMASK   0x1000   /* Mask HPI transmit interrupt (hit) */
#define  IMASK_MHR_BMASK   0x2000   /* Mask HPI receive interrupt (hir) */
#define  IMASK_PGIE_BMASK  0x4000   /* Previous Global Interrupt Enable */
#define  IMASK_GIE_BMASK   0x8000   /* Global Interrupt Enable */

#define  IMASK_ME0_BPOS   0         /* INT0 bit position in imask reg */
#define  IMASK_ME1_BPOS   1         /* INT1 bit position in imask reg */
#define  IMASK_ME2_BPOS   2         /* INT2 bit position in imask reg */
#define  IMASK_ME3_BPOS   3         /* INT3 bit position in imask reg */
#define  IMASK_ME4_BPOS   4         /* INT4 bit position in imask reg */
#define  IMASK_MT0_BPOS   5         /* t0 bit position in imask reg */
#define  IMASK_MT1_BPOS   6         /* t1 bit position in imask reg */
#define  IMASK_MD_BPOS    7         /* DMA bit position in imask reg */
#define  IMASK_M0T_BPOS   8         /* s0t bit position in imask reg */
#define  IMASK_M0R_BPOS   9         /* s0r bit position in imask reg */
#define  IMASK_M1T_BPOS   10        /* s1t bit position in imask reg */
#define  IMASK_M1R_BPOS   11        /* s1r bit position in imask reg */
#define  IMASK_MHT_BPOS   12        /* hit bit position in imask reg */
#define  IMASK_MHR_BPOS   13        /* hir bit position in imask reg */
#define  IMASK_PGIE_BPOS  14        /* PGIE bit position in imask reg */
#define  IMASK_GIE_BPOS   15        /* GIE bit position in imask reg */


/***************************************************************************
*  C3: Interrupt Priority Register 0 (ip0) Definitions
***************************************************************************/
#define  IP0_S0T_BMASK     0x0003   /* Serial Port0 transmit ip level */
#define  IP0_S0R_BMASK     0x000C   /* Serial Port0 receive ip level */
#define  IP0_S1T_BMASK     0x0030   /* Serial Port1 transmit ip level */
#define  IP0_S1R_BMASK     0x00C0   /* Serial Port1 receive ip level */
#define  IP0_HIT_BMASK     0x0300   /* HPI transmit ip level */
#define  IP0_HIR_BMASK     0x0C00   /* HPI receive ip level */
#define  IP0_PEPL_BMASK    0x3000   /* Previous execution priority level */
#define  IP0_EPL_BMASK     0xC000   /* Current execution priority level */

/***************************************************************************
*  C4: Interrupt Priority Register 1 (ip1) Definitions
***************************************************************************/
#define  IP1_EX0_BMASK     0x0003   /* External Int0 ip level */
#define  IP1_EX1_BMASK     0x000C   /* External Int1 ip level */
#define  IP1_EX2_BMASK     0x0030   /* External Int2 ip level */
#define  IP1_EX3_BMASK     0x00C0   /* External Int3 ip level */
#define  IP1_EX4_BMASK     0x0300   /* External Int4 ip level */
#define  IP1_T0_BMASK      0x0C00   /* Timer0 (t0) ip level */
#define  IP1_T1_BMASK      0x3000   /* Timer1 (t1) ip level */
#define  IP1_DMA_BMASK     0xC000   /* DMA ip level */

/***************************************************************************
*  C5: Loop Counter Register 0 (loop0) Definitions
***************************************************************************/

/***************************************************************************
*  C6: Loop Counter Register 1 (loop1) Definitions
***************************************************************************/

/***************************************************************************
*  C7: Guard Register (guard) Definitions
***************************************************************************/

/***************************************************************************
*  C8: Conditional Codes (hwflag) Definitions
***************************************************************************/
#define  HWFLAG_ER_BMASK   0x0001   /* Emulation recieve bit */
#define  HWFLAG_EX_BMASK   0x0002   /* Emulation transmit bit */
#define  HWFLAG_IR_BMASK   0x0004   /* Interrupt pending flag */
#define  HWFLAG_Z_BMASK    0x0008   /* Result is zero flag */
#define  HWFLAG_GT_BMASK   0x0010   /* Result is greater-than flag */
#define  HWFLAG_GE_BMASK   0x0020   /* Result is greater-or-equal flag */
#define  HWFLAG_C_BMASK    0x0040   /* Carry flag */
#define  HWFLAG_GSV_BMASK  0x0080   /* Guard register sticky overflow flag */
#define  HWFLAG_SV_BMASK   0x0100   /* Sticky overflow flag */
#define  HWFLAG_GV_BMASK   0x0200   /* Guard register overflow flag */
#define  HWFLAG_V_BMASK    0x0400   /* Overflow flag */

#define  HWFLAG_ER_BPOS         0   /* ER bit position in reg */
#define  HWFLAG_EX_BPOS         1   /* EX bit position in reg */
#define  HWFLAG_IR_BPOS         2   /* IR bit position in reg */
#define  HWFLAG_Z_BPOS          3   /* Z bit position in reg */
#define  HWFLAG_GT_BPOS         4   /* GT bit position in reg */
#define  HWFLAG_GE_BPOS         5   /* GE bit position in reg */
#define  HWFLAG_C_BPOS          6   /* C bit position in reg */
#define  HWFLAG_GSV_BPOS        7   /* GSV bit position in reg */
#define  HWFLAG_SV_BPOS         8   /* SV bit position in reg */
#define  HWFLAG_GV_BPOS         9   /* GV bit position in reg */
#define  HWFLAG_V_BPOS         10   /* V bit position in reg */

/***************************************************************************
*  C9: Interrupt Request (ireq) Definitions
***************************************************************************/
#define  IREQ_EX0_BMASK    0x0001   /* External interrupt 0 (INT0) */
#define  IREQ_EX1_BMASK    0x0002   /* External interrupt 1 (INT1) */
#define  IREQ_EX2_BMASK    0x0004   /* External interrupt 2 (INT2) */
#define  IREQ_EX3_BMASK    0x0008   /* External interrupt 3 (INT3) */
#define  IREQ_EX4_BMASK    0x0010   /* External interrupt 4 (INT4) */
#define  IREQ_T0_BMASK     0x0020   /* Timer0 interrupt (t0) */
#define  IREQ_T1_BMASK     0x0040   /* Timer1 interrupt (t1) */
#define  IREQ_DMA_BMASK    0x0080   /* DMA interrupt */
#define  IREQ_S0T_BMASK    0x0100   /* Serial port0 transmit int (s0t) */
#define  IREQ_S0R_BMASK    0x0200   /* Serial port0 receive int (s0r) */
#define  IREQ_S1T_BMASK    0x0400   /* Serial port1 transmit int (s1t) */
#define  IREQ_S1R_BMASK    0x0800   /* Serial port1 receive int (s1r) */
#define  IREQ_HIT_BMASK    0x1000   /* HPI transmit interrupt (hit) */
#define  IREQ_HIR_BMASK    0x2000   /* HPI receive interrupt (hir) */
#define  IREQ_DEI_BMASK    0x4000   /* Device emulation interrupt */
#define  IREQ_NMI_BMASK    0x8000   /* Non-maskable interrupt */

#define  IREQ_EX0_BPOS   0         /* INT0 bit position in ireq reg */
#define  IREQ_EX1_BPOS   1         /* INT1 bit position in ireq reg */
#define  IREQ_EX2_BPOS   2         /* INT2 bit position in ireq reg */
#define  IREQ_EX3_BPOS   3         /* INT3 bit position in ireq reg */
#define  IREQ_EX4_BPOS   4         /* INT4 bit position in ireq reg */
#define  IREQ_T0_BPOS    5         /* t0 bit position in ireq reg */
#define  IREQ_T1_BPOS    6         /* t1 bit position in ireq reg */
#define  IREQ_DMA_BPOS   7         /* DMA bit position in ireq reg */
#define  IREQ_S0T_BPOS   8         /* s0t bit position in ireq reg */
#define  IREQ_S0R_BPOS   9         /* s0r bit position in ireq reg */
#define  IREQ_S1T_BPOS   10        /* s1t bit position in ireq reg */
#define  IREQ_S1R_BPOS   11        /* s1r bit position in ireq reg */
#define  IREQ_HIT_BPOS   12        /* hit bit position in ireq reg */
#define  IREQ_HIR_BPOS   13        /* hir bit position in ireq reg */
#define  IREQ_DEI_BPOS   14        /* dei bit position in ireq reg */
#define  IREQ_NMI_BPOS   15        /* nmi bit position in ireq reg */

/***************************************************************************
*  C10: Reserved
***************************************************************************/

/***************************************************************************
*  C11: Reserved
***************************************************************************/

/***************************************************************************
*  C12: Viterbi Traceback (vitr) Definitions
***************************************************************************/

/***************************************************************************
*  C13: Reserved
***************************************************************************/

/***************************************************************************
*  C14: Reserved
***************************************************************************/

/***************************************************************************
*  C15: System Mode (smode) Definitions
***************************************************************************/
#define  SMODE_DDR_BMASK   0x0001   /* Disable internal data RAM */
#define  SMODE_DIR_BMASK   0x0002   /* Disable internal instruction RAM */
#define  SMODE_CB1_BMASK   0x0004   /* Circular buffer1 enable */
#define  SMODE_CB0_BMASK   0x0008   /* Circular buffer0 enable */
#define  SMODE_SIS_BMASK   0x0010   /* Store instruction space enable */
#define  SMODE_LIS_BMASK   0x0020   /* Load instruction space enable */
#define  SMODE_US_BMASK    0x0040   /* Uni-scalar mode enable */
#define  SMODE_UVT_BMASK   0x0080   /* User vector table address enable */
#define  SMODE_DSB_BMASK   0x0100   /* MIU store buffer enable */
#define  SMODE_ICT_BMASK   0x0200   /* Invalidate contents of the IU cache */
#define  SMODE_FIE_BMASK   0x0400   /* Force internal execution */
#define  SMODE_DCT_BMASK   0x0800   /* Invalidate contents of the data cache */
#define  SMODE_SHD_BMASK   0x1000   /* Select the shadow registers */
#define  SMODE_LVL_BMASK   0xE000   /* Power Level bits */

#define  SMODE_DDR_BPOS         0   /* DDR bit position in reg */
#define  SMODE_DIR_BPOS         1   /* DIR bit position in reg */
#define  SMODE_CB1_BPOS         2   /* CB1 bit position in reg */
#define  SMODE_CB0_BPOS         3   /* CB0 bit position in reg */
#define  SMODE_SIS_BPOS         4   /* SIS bit position in reg */
#define  SMODE_LIS_BPOS         5   /* LIS bit position in reg */
#define  SMODE_US_BPOS          6   /* US bit position in reg */
#define  SMODE_UVT_BPOS         7   /* USV bit position in reg */
#define  SMODE_DSB_BPOS         8   /* DSB bit position in reg */
#define  SMODE_ICT_BPOS         9   /* ICT bit position in reg */
#define  SMODE_FIE_BPOS        10   /* FIE bit position in reg */
#define  SMODE_DCT_BPOS        11   /* DCT bit position in reg */
#define  SMODE_SHD_BPOS        12   /* SHD bit position in reg */

/***************************************************************************
*  C16: Program Counter (pc) Definitions
***************************************************************************/

/***************************************************************************
*  C17: Return Program Counter (rpc) Definitions
***************************************************************************/

/***************************************************************************
*  C18: Trap interrupt return program counter (tpc) Definitions
***************************************************************************/

/***************************************************************************
*  C19: Circular Buffer 0 Start Address (cb0_beg)
***************************************************************************/

/***************************************************************************
*  C20: Circular Buffer 1 Start Address (cb1_beg)
***************************************************************************/

/***************************************************************************
*  C21: Circular Buffer 0 End Address (cb0_end)
***************************************************************************/

/***************************************************************************
*  C22: Circular Buffer 1 End Address (cb1_end)
***************************************************************************/

/***************************************************************************
*  C23: Timer 0 Counter (timer0)
***************************************************************************/

/***************************************************************************
*  C24: Timer 1 Counter (timer1)
***************************************************************************/

/***************************************************************************
*  C25: Reserved
***************************************************************************/

/***************************************************************************
*  C26: Reserved
***************************************************************************/

/***************************************************************************
*  C27: Reserved
***************************************************************************/

/***************************************************************************
*  C28: Reserved
***************************************************************************/

/***************************************************************************
*  C29: Reserved
***************************************************************************/

/***************************************************************************
*  C30: Device Emulation Instruction (dei) Register
***************************************************************************/

/***************************************************************************
*  C31: Device Emulation Data (ded) Register
***************************************************************************/

#endif   /* ZSPCORE_DEFS_H */
