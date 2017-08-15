/*
 * --------------------------------------------------------------------------------
 * (c) Copyright 2008, SiTel Semiconductor BV
 * All Rights Reserved
 * --------------------------------------------------------------------------------
 * SITEL SEMICONDUCTOR CONFIDENTIAL
 * --------------------------------------------------------------------------------
 * This code includes Confidential, Proprietary Information and is a Trade 
 * Secret of Sitel Semiconductor BV. All use, disclosure, and/or reproduction 
 * is prohibited unless authorized in writing.
 * --------------------------------------------------------------------------------
 *
 * Description: header file for: 'dectip_ahb_byte.c', 'dectip_ahb_halfword.c' and
 *                               'dectip_ahb_word.c'.
 *
 * --------------------------------------------------------------------------------
 * Synchronicity history: 
 * 
 * $Log: dectip_ahb_reg_test.h.rca $
 * 
 *  Revision: 1.7 Thu Feb 12 16:34:15 2009 snijders
 *  Updated now completely according the documented DS of 04-feb-2009, v0.42.
 * 
 *  Revision: 1.6 Tue Feb 10 15:13:51 2009 snijders
 *  Changed 'DBG_INOUT_REG*' to 'DSP_DBG_INOUT_REG*'.
 * 
 *  Revision: 1.5 Fri Feb  6 19:22:04 2009 snijders
 *  Changed 'DSP_JTBL_START_REG_RESET' from "0x00008000" to "0x00000000".
 * 
 *  Revision: 1.4 Tue Feb  3 10:32:17 2009 snijders
 *  Changed 'DSP_JTBL_REG' to 'DSP_JTBL_START_REG'.
 * 
 *  Revision: 1.3 Mon Feb  2 15:52:39 2009 snijders
 *  Added "DSP_ZCROSS1_OUT_REG" & "DSP_ZCROSS2_OUT_REG" definitions.
 *  Updated now completely according the documented DS of 22-jan-2009, v0.41.
 * 
 *  Revision: 1.2 Mon Jan 19 16:08:23 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.1 Mon Jan 19 16:06:12 2009 snijders
 *  Copied from 'spu_reg_test.h'. Removed some old comment.
 *  No further changes w.r.t. masking.
 * 
 * --------------------------------------------------------------------------------
 * Remarks:
 *  This file is originally created with 'reg_test_generator.exe'.
 *
 *  To prevent unwanted side effects, the register bit-fields below
 *  are always excluded from the generated testcode
 *
 *     DSP_CTRL_REG [ DSP_EN ]
 *
 * --------------------------------------------------------------------------------
*/ 

/*========================== Include files ======================================*/
#include "sitel_io.h"
#include "sitel_functions.h"

/*========================== Local macro definitions ============================*/

/* 
 * <register>_RESET, _RW_MASK and _ALLOWED are defined as follows:
 *   Example:
 *     #define <register>_RESET   0x00000000                 // Reset value. 
 *     #define <register>_RW_MASK 0x0000009e                 // All R/W bits only.
 *     #define <register>_ALLOWED (0x0000ff7f & 0xffff009f)  // All functional allowable W bits (incl. - and R bits) & all R/W and R bits only.
*/

#define DSP_MAIN_SYNC0_REG_RESET   0x0000ffff
#define DSP_MAIN_SYNC0_REG_RW_MASK 0x0000ffff
#define DSP_MAIN_SYNC0_REG_ALLOWED (0x0000ffff & 0xffffffff)

#define DSP_MAIN_SYNC1_REG_RESET   0x00000000
#define DSP_MAIN_SYNC1_REG_RW_MASK 0x0000003f
#define DSP_MAIN_SYNC1_REG_ALLOWED (0x0000ffff & 0xffff003f)

#define DSP_MAIN_CNT_REG_RESET   0x00008f00
#define DSP_MAIN_CNT_REG_RW_MASK 0x0000ff00
#define DSP_MAIN_CNT_REG_ALLOWED (0x0000ffff & 0xffffffff)

#define DSP_RAM_OUT0_REG_RESET   0x00000000
#define DSP_RAM_OUT0_REG_RW_MASK 0x000000ff
#define DSP_RAM_OUT0_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define DSP_RAM_OUT1_REG_RESET   0x00000000
#define DSP_RAM_OUT1_REG_RW_MASK 0x000000ff
#define DSP_RAM_OUT1_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define DSP_RAM_OUT2_REG_RESET   0x00000000
#define DSP_RAM_OUT2_REG_RW_MASK 0x000000ff
#define DSP_RAM_OUT2_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define DSP_RAM_OUT3_REG_RESET   0x00000000
#define DSP_RAM_OUT3_REG_RW_MASK 0x000000ff
#define DSP_RAM_OUT3_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define DSP_RAM_IN0_REG_RESET   0x00000000
#define DSP_RAM_IN0_REG_RW_MASK 0x00000000
#define DSP_RAM_IN0_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define DSP_RAM_IN1_REG_RESET   0x00000000
#define DSP_RAM_IN1_REG_RW_MASK 0x00000000
#define DSP_RAM_IN1_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define DSP_RAM_IN2_REG_RESET   0x00000000
#define DSP_RAM_IN2_REG_RW_MASK 0x00000000
#define DSP_RAM_IN2_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define DSP_RAM_IN3_REG_RESET   0x00000000
#define DSP_RAM_IN3_REG_RW_MASK 0x00000000
#define DSP_RAM_IN3_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define DSP_ZCROSS1_OUT_REG_RESET   0x00000000
#define DSP_ZCROSS1_OUT_REG_RW_MASK 0x00008000
#define DSP_ZCROSS1_OUT_REG_ALLOWED (0x00008000 & 0xffffffff)

#define DSP_ZCROSS2_OUT_REG_RESET   0x00000000
#define DSP_ZCROSS2_OUT_REG_RW_MASK 0x00008000
#define DSP_ZCROSS2_OUT_REG_ALLOWED (0x00008000 & 0xffffffff)

#define DSP_MAIN_CTRL_REG_RESET   0x000002AF
#define DSP_MAIN_CTRL_REG_RW_MASK 0x000003ff
#define DSP_MAIN_CTRL_REG_ALLOWED (0x0000ffff & 0xffff03ff)

#define DSP_CTRL_REG_RESET   0x00000000
#define DSP_CTRL_REG_RW_MASK 0x0000010c // removed bit 4
#define DSP_CTRL_REG_ALLOWED (0x0000fffb & 0xffff010c) //removed bit 4

#define DSP_PC_REG_RESET   0x00000000
#define DSP_PC_REG_RW_MASK 0x00000000
#define DSP_PC_REG_ALLOWED (0x0000ffff & 0xffffffff)

#define DSP_PC_START_REG_RESET   0x00000000
#define DSP_PC_START_REG_RW_MASK 0x0000ffff
#define DSP_PC_START_REG_ALLOWED (0x0000ffff & 0xffffffff)

#define DSP_IRQ_START_REG_RESET   0x00000000
#define DSP_IRQ_START_REG_RW_MASK 0x0000fff0
#define DSP_IRQ_START_REG_ALLOWED (0x0000ffff & 0xfffffff0)

#define DSP_INT_REG_RESET   0x00000000
#define DSP_INT_REG_RW_MASK 0x00000000 // special reg '1' clear int bit
#define DSP_INT_REG_ALLOWED (0x0000ffff & 0xffff0000) // special reg '1' clear int bit

#define DSP_INT_MASK_REG_RESET   0x00000000
#define DSP_INT_MASK_REG_RW_MASK 0x0000ffff
#define DSP_INT_MASK_REG_ALLOWED (0x0000ffff & 0xffffffff)

#define DSP_INT_PRIO1_REG_RESET   0x00000777
#define DSP_INT_PRIO1_REG_RW_MASK 0x00000777
#define DSP_INT_PRIO1_REG_ALLOWED (0x0000ffff & 0xffff0777)

#define DSP_INT_PRIO2_REG_RESET   0x00000777
#define DSP_INT_PRIO2_REG_RW_MASK 0x00000777
#define DSP_INT_PRIO2_REG_ALLOWED (0x0000ffff & 0xffff0777)

#define DSP_OVERFLOW_REG_RESET   0x00000000
#define DSP_OVERFLOW_REG_RW_MASK 0x0000ff00 // removed bit 7 -> 0
#define DSP_OVERFLOW_REG_ALLOWED (0x0000ffff & 0xffffff00) // removed bit 7 -> 0

#define DSP_JTBL_START_REG_RESET   0x00000000
#define DSP_JTBL_START_REG_RW_MASK 0x0000fe00 // removed bit 8 -> 0
#define DSP_JTBL_START_REG_ALLOWED (0x0000ffff & 0xfffffe00) // removed bit 7 -> 0

#define Reserved_RESET   0x00000000
#define Reserved_RW_MASK 0x00000000
#define Reserved_ALLOWED (0x00000000 & 0xffffffff)

#define DSP_DBG_INOUT_REG_LSW_RESET   0x00000000
#define DSP_DBG_INOUT_REG_LSW_RW_MASK 0x00000000
#define DSP_DBG_INOUT_REG_LSW_ALLOWED (0x00000000 & 0xffffffff)

#define DSP_DBG_INOUT_REG_MSW_RESET   0x00000000
#define DSP_DBG_INOUT_REG_MSW_RW_MASK 0x00000000
#define DSP_DBG_INOUT_REG_MSW_ALLOWED (0x00000000 & 0xffffffff)

#define SHARED_RAM_START_RESET   0x00000000
#define SHARED_RAM_START_RW_MASK 0x0000ffff
#define SHARED_RAM_START_ALLOWED (0x0000ffff & 0xffffffff)

#define SHARED_RAM_END_RESET   0x00000000
#define SHARED_RAM_END_RW_MASK 0x0000ffff
#define SHARED_RAM_END_ALLOWED (0x0000ffff & 0xffffffff)

#define DSP_MC_RAM_START_RESET   0x00000000
#define DSP_MC_RAM_START_RW_MASK 0x0000ffff
#define DSP_MC_RAM_START_ALLOWED (0x0000ffff & 0xffffffff)

#define DSP_MC_RAM_END_RESET   0x00000000
#define DSP_MC_RAM_END_RW_MASK 0x0000ffff
#define DSP_MC_RAM_END_ALLOWED (0x0000ffff & 0xffffffff)


#define dByte  0x0000
#define dWord  0x0001
#define dDWord 0x0002


