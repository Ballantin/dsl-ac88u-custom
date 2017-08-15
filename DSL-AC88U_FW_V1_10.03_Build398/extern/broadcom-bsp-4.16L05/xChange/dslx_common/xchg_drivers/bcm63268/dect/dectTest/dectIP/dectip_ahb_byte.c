/*
 * ------------------------------------------------------------------------------
 * (c) Copyright 2008, SiTel Semiconductor BV
 * All Rights Reserved
 * ------------------------------------------------------------------------------
 * SITEL SEMICONDUCTOR CONFIDENTIAL
 * ------------------------------------------------------------------------------
 * This code includes Confidential, Proprietary Information and is a Trade 
 * Secret of Sitel Semiconductor BV. All use, disclosure, and/or reproduction 
 * is prohibited unless authorized in writing.
 * ------------------------------------------------------------------------------
 *
 * Description: test byte (8 bits) accessibility of AHB peripheral registers
 *
 * ------------------------------------------------------------------------------
 * Synchronicity history: 
 * 
 * $Log: dectip_ahb_byte.c.rca $
 * 
 *  Revision: 1.15 Tue Feb 10 17:20:24 2009 snijders
 *  Changed 'DBG_INOUT_REG' to 'DSP_DBG_INOUT_REG' according to the DS 0.42
 *  of 4-feb-2009.
 * 
 *  Revision: 1.14 Mon Feb  9 17:11:18 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.13 Mon Feb  9 17:04:25 2009 snijders
 *  Added test register 'DEBUGGER2_REG'.
 *  Changed the usage of 'TEST_REG', 'DEBUGGER_REG' and 'DEBUGGER2_REG'.
 *  Added code to SEQUENCER_RAM of part 2.2 to improve code coverage.
 * 
 *  Revision: 1.12 Tue Feb  3 10:32:18 2009 snijders
 *  Changed 'DSP_JTBL_REG' to 'DSP_JTBL_START_REG'.
 * 
 *  Revision: 1.11 Mon Feb  2 15:57:30 2009 snijders
 *  Now really removed "DSP_RAM_IN3_REG+2" & "DSP_RAM_IN3_REG+4".
 * 
 *  Revision: 1.10 Mon Feb  2 15:50:22 2009 snijders
 *  Added "DSP_ZCROSS1_OUT_REG" & "DSP_ZCROSS2_OUT_REG" register test.
 *  Removed "DSP_RAM_IN3_REG+2" & "DSP_RAM_IN3_REG+4" register test.
 *  Updated now completely according the documented DS of 22-jan-2009, v0.41.
 * 
 *  Revision: 1.9 Fri Jan 23 11:49:32 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.8 Fri Jan 23 11:11:34 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.7 Fri Jan 23 10:33:36 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.6 Thu Jan 22 15:26:45 2009 snijders
 *  Updated a lot of comment.
 *  Added some examples of DEBUGGER_REG and TEST_REG usage.
 *  Removed the hexadecimal increments at DSP_RAM_IN3_REG (kept +26).
 *  Updated part "2.2: Memories (incl. boundary test)."
 *  Changed variable "i" to "p" in the Word part (data_size == 1) of the third 
 *  test in 'check_dsp_reg'.
 * 
 *  Revision: 1.5 Tue Jan 20 11:21:04 2009 laarhove
 *  added isr dip dsp
 * 
 *  Revision: 1.4 Mon Jan 19 16:26:59 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.3 Mon Jan 19 16:23:03 2009 snijders
 *  Updated the 'byte' part of the 'Second test' in 'check_dsp_reg' (check
 *  the comment string "//HS/19-jan-2009)"):
 *    - changed the remaining 'i' to 'p',
 *    - added additional '(' and ')' in the second compare.
 *  Changed 'spu_reg_test.h' to 'dectip_ahb_reg_test.h'.
 *  Added the second remark.
 * 
 *  Revision: 1.2 Sat Jan 17 21:32:59 2009 snijders
 *  Added some temporary basic byte, word and dword access tests.
 * 
 *  Revision: 1.1 Thu Jan 15 15:27:33 2009 laarhove
 *  added several ahb and int tests
 * 
 * ------------------------------------------------------------------------------
 * Remarks:
 *  
 *  1) This file is originally created with 'reg_test_generator.exe'.
 *
 *     To prevent unwanted side effects, the register bit-fields below
 *     are always excluded from the generated testcode
 *
 *        DSP_CTRL_REG [ DSP_EN ]
 *
 *  2) Used terminology.
 *
 *     # bits access | AMBA terminology | This C-code terminology
 *     ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *           8       |     byte         |       byte
 *          16       |     halfword     |       word
 *          32       |     word         |       doubleword (DWord)
 *  
 * ------------------------------------------------------------------------------
 */ 


/*========================== Include files ====================================*/
#include "sitel_io.h"
#include "sitel_functions.h"
#include "dectip_ahb_reg_test.h"

/*========================== Local macro definitions ==========================*/

/*========================== Global definitions ===============================*/

/*========================== Local function prototypes ========================*/
bool check_dsp_reg(uint32 reg, uint32 reset_value, uint32 mask_value, uint32 allowed, WORD data_size);
extern void gprint(const char *fmt, unsigned long arg0, unsigned long arg1, unsigned long arg2);

/* The following DSP and DIP ISR's are only added in this test for software compatibility 
   reasons, they are not funtional used in this test.
*/
// ISR for DSP interrupt
//void isr_dsp_int (int i, int *o) { }
    
// ISR for DIP interrupt
//void isr_dip_int (int i, int *o) { }

/*========================== Local data definitions ===========================*/

/*========================== Function definitions =============================*/

int dectip_ahb_byte(void) 
{
    /* Part 1: Execute the most basic types of AHB access tests. */
    /* ========================================================= */
   unsigned char val;

        /* Part 1.1: AHB Byte access. */
            SetByte(SHARED_RAM_START+0,0xAA);
	    val = GetByte(SHARED_RAM_START+0);
            gprint("\n#################### read back val=%d %d%d ####################\n", val,val,val );
            if ( val != 0xAA ) {return FALSE;} 

            SetByte(SHARED_RAM_START+1,0xBB);
            if ( GetByte(SHARED_RAM_START+1) != 0xBB ) return FALSE;

            SetByte(SHARED_RAM_START+2,0xCC);
            if ( GetByte(SHARED_RAM_START+2) != 0xCC ) return FALSE;

            SetByte(SHARED_RAM_START+3,0xDD);
            if ( GetByte(SHARED_RAM_START+3) != 0xDD ) return FALSE;

        /* Part 1.2: AHB Word access. */
            SetWord(SHARED_RAM_START+4,0x0123);
            if ( GetWord(SHARED_RAM_START+4) != 0x0123 ) return FALSE;

            SetWord(SHARED_RAM_START+6,0x4567);
            if ( GetWord(SHARED_RAM_START+6) != 0x4567 ) return FALSE;

        /* Part 1.3: AHB Double Word access. */
            SetDWord(SHARED_RAM_START+8,0x89ABCDEF);
            if ( GetDWord(SHARED_RAM_START+8) != 0x89ABCDEF ) return FALSE;

	    gprint("\n#################### part 1 passed %d%d%d ####################\n", 0,0,0 );

    /* Part 2: Execute the more extensive access tests. */
    /* ================================================ */

      WORD data_size = dByte; //change for different data widths

      /* 2.1: Registers. */
      /* --------------- */
          SetWord(TEST_REG,DECTIP_AHB_BYTE); /* Write test number register. */
          SetWord(DEBUGGER_REG, 0x0000);     /* E.g.: to see where you are in 'dectip_ahb_byte' (main program) during simulation. */
          SetWord(DEBUGGER2_REG,0x0000);     /* E.g.: to see where you are in 'check_dsp_reg' (function) during simulation. */

          SetWord(DEBUGGER_REG,0x0000);
          if ( !check_dsp_reg(DSP_MAIN_SYNC0_REG, DSP_MAIN_SYNC0_REG_RESET, DSP_MAIN_SYNC0_REG_RW_MASK, DSP_MAIN_SYNC0_REG_ALLOWED, data_size )) return FALSE;
          SetWord(DEBUGGER_REG,0x0001);
          if ( !check_dsp_reg(DSP_MAIN_SYNC1_REG, DSP_MAIN_SYNC1_REG_RESET, DSP_MAIN_SYNC1_REG_RW_MASK, DSP_MAIN_SYNC1_REG_ALLOWED, data_size )) return FALSE;
          SetWord(DEBUGGER_REG,0x0002);
          if ( !check_dsp_reg(DSP_MAIN_CNT_REG, DSP_MAIN_CNT_REG_RESET, DSP_MAIN_CNT_REG_RW_MASK, DSP_MAIN_CNT_REG_ALLOWED, data_size )) return FALSE;
          SetWord(DEBUGGER_REG,0x0003);
          if ( !check_dsp_reg(DSP_MAIN_CNT_REG+2, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          SetWord(DEBUGGER_REG,0x0004);
          if ( !check_dsp_reg(DSP_MAIN_CNT_REG+4, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_MAIN_CNT_REG+6, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_MAIN_CNT_REG+8, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_MAIN_CNT_REG+10, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_MAIN_CNT_REG+12, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_OUT0_REG, DSP_RAM_OUT0_REG_RESET, DSP_RAM_OUT0_REG_RW_MASK, DSP_RAM_OUT0_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_OUT1_REG, DSP_RAM_OUT1_REG_RESET, DSP_RAM_OUT1_REG_RW_MASK, DSP_RAM_OUT1_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_OUT2_REG, DSP_RAM_OUT2_REG_RESET, DSP_RAM_OUT2_REG_RW_MASK, DSP_RAM_OUT2_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_OUT3_REG, DSP_RAM_OUT3_REG_RESET, DSP_RAM_OUT3_REG_RW_MASK, DSP_RAM_OUT3_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN0_REG, DSP_RAM_IN0_REG_RESET, DSP_RAM_IN0_REG_RW_MASK, DSP_RAM_IN0_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN1_REG, DSP_RAM_IN1_REG_RESET, DSP_RAM_IN1_REG_RW_MASK, DSP_RAM_IN1_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN2_REG, DSP_RAM_IN2_REG_RESET, DSP_RAM_IN2_REG_RW_MASK, DSP_RAM_IN2_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG, DSP_RAM_IN3_REG_RESET, DSP_RAM_IN3_REG_RW_MASK, DSP_RAM_IN3_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_ZCROSS1_OUT_REG, DSP_ZCROSS1_OUT_REG_RESET, DSP_ZCROSS1_OUT_REG_RW_MASK, DSP_ZCROSS1_OUT_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_ZCROSS2_OUT_REG, DSP_ZCROSS2_OUT_REG_RESET, DSP_ZCROSS2_OUT_REG_RW_MASK, DSP_ZCROSS2_OUT_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+6, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+8, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+10, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+12, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+14, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+16, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+18, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+20, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+22, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+24, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_RAM_IN3_REG+26, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_MAIN_CTRL_REG, DSP_MAIN_CTRL_REG_RESET, DSP_MAIN_CTRL_REG_RW_MASK, DSP_MAIN_CTRL_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_CTRL_REG, DSP_CTRL_REG_RESET, DSP_CTRL_REG_RW_MASK, DSP_CTRL_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_PC_REG, DSP_PC_REG_RESET, DSP_PC_REG_RW_MASK, DSP_PC_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_PC_START_REG, DSP_PC_START_REG_RESET, DSP_PC_START_REG_RW_MASK, DSP_PC_START_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_IRQ_START_REG, DSP_IRQ_START_REG_RESET, DSP_IRQ_START_REG_RW_MASK, DSP_IRQ_START_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_INT_REG, DSP_INT_REG_RESET, DSP_INT_REG_RW_MASK, DSP_INT_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_INT_MASK_REG, DSP_INT_MASK_REG_RESET, DSP_INT_MASK_REG_RW_MASK, DSP_INT_MASK_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_INT_PRIO1_REG, DSP_INT_PRIO1_REG_RESET, DSP_INT_PRIO1_REG_RW_MASK, DSP_INT_PRIO1_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_INT_PRIO2_REG, DSP_INT_PRIO2_REG_RESET, DSP_INT_PRIO2_REG_RW_MASK, DSP_INT_PRIO2_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_OVERFLOW_REG, DSP_OVERFLOW_REG_RESET, DSP_OVERFLOW_REG_RW_MASK, DSP_OVERFLOW_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_JTBL_START_REG, DSP_JTBL_START_REG_RESET, DSP_JTBL_START_REG_RW_MASK, DSP_JTBL_START_REG_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_JTBL_START_REG+2, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_JTBL_START_REG+4, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_JTBL_START_REG+6, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_JTBL_START_REG+8, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_JTBL_START_REG+10, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_JTBL_START_REG+12, Reserved_RESET, Reserved_RW_MASK,Reserved_ALLOWED,data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_DBG_INOUT_REG_LSW, DSP_DBG_INOUT_REG_LSW_RESET, DSP_DBG_INOUT_REG_LSW_RW_MASK, DSP_DBG_INOUT_REG_LSW_ALLOWED, data_size )) return FALSE;
          if ( !check_dsp_reg(DSP_DBG_INOUT_REG_MSW, DSP_DBG_INOUT_REG_MSW_RESET, DSP_DBG_INOUT_REG_MSW_RW_MASK, DSP_DBG_INOUT_REG_MSW_ALLOWED, data_size )) return FALSE;
      gprint("\n#################### part 2.1 passed %d%d%d ####################\n", 0,0,0 );

      /* 2.2: Memories (incl. boundary test). */
      /* ------------------------------------ */
      
          /* SHARED_RAM. */
          /* ----------- */
          /* Clear SHARED_RAM content first (see Part 1). */
          SetDWord(SHARED_RAM_START+0,0x00000000);
          SetDWord(SHARED_RAM_START+4,0x00000000);
          SetDWord(SHARED_RAM_START+8,0x00000000);
          SetWord(DEBUGGER_REG,0x00A0);
          SetWord(DEBUGGER2_REG,0x0000);     /* E.g.: to see where you are in 'check_dsp_reg' (function) during simulation. */
          SetByte(SHARED_RAM_START,0x00);
	  if ( !check_dsp_reg(SHARED_RAM_START, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00A1);
          SetByte(SHARED_RAM_START+1,0x00);
	  if ( !check_dsp_reg(SHARED_RAM_START+1, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00A2);
          SetByte(SHARED_RAM_START+2,0x00);
          if ( !check_dsp_reg(SHARED_RAM_START+2, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00A3);	
          SetByte(SHARED_RAM_START+3,0x00);
          if ( !check_dsp_reg(SHARED_RAM_START+3, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

/* BRCM ADD: Clear RAM content first.  Without this, test will fail because the check_dsp_reg routine checks for 2 bytes every time,
   but, only 1 byte is being set to 0x00 in the next 3 tests, which means the other byte will contain random values.
 */
          SetDWord(SHARED_RAM_END-3, 0x00000000);     /* Clear 4 bytes at SHARED_RAM_END-3 */
 
          SetWord(DEBUGGER_REG,0x00A4);
          SetByte(SHARED_RAM_END-3,0x0000);
          if ( !check_dsp_reg(SHARED_RAM_END-3, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00A5);
          SetByte(SHARED_RAM_END-2,0x0000);
          if ( !check_dsp_reg(SHARED_RAM_END-2, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00A6);
          SetByte(SHARED_RAM_END-1,0x0000);
          if ( !check_dsp_reg(SHARED_RAM_END-1, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;
      
	  gprint("\n#################### part 2.2 Shared RAM passed %d%d%d ####################\n", 0,0,0 );

          /* DSP_MC_RAM. */
          /* ----------- */
/* BRCM ADD: Clear RAM content first.  Without this, test will fail because the check_dsp_reg routine checks for 2 bytes every time,
   but, only 1 byte is being set to 0x00 in the next 3 tests, which means the other byte will contain random values.
 */
          SetDWord(DSP_MC_RAM_START, 0x00000000);     /* Clear 4 bytes at DSP_MC_RAM_START */
          SetDWord(DSP_MC_RAM_START+4, 0x00000000);   /* Clear 4 bytes at DSP_MC_RAM_START+4 */
                    
          SetWord(DEBUGGER_REG,0x00B0);
          SetByte(DSP_MC_RAM_START,0x00);
          if ( !check_dsp_reg(DSP_MC_RAM_START, DSP_MC_RAM_START_RESET, DSP_MC_RAM_START_RW_MASK,DSP_MC_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00B1);
          SetByte(DSP_MC_RAM_START+1,0x00);
          if ( !check_dsp_reg(DSP_MC_RAM_START+1, DSP_MC_RAM_START_RESET, DSP_MC_RAM_START_RW_MASK,DSP_MC_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00B2);
          SetByte(DSP_MC_RAM_START+2,0x00);
          if ( !check_dsp_reg(DSP_MC_RAM_START+2, DSP_MC_RAM_START_RESET, DSP_MC_RAM_START_RW_MASK,DSP_MC_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00B3);
          SetByte(DSP_MC_RAM_START+3,0x00);
          if ( !check_dsp_reg(DSP_MC_RAM_START+3, DSP_MC_RAM_START_RESET, DSP_MC_RAM_START_RW_MASK,DSP_MC_RAM_START_ALLOWED,data_size )) return FALSE;

/* BRCM ADD: Clear RAM content first.  Without this, test will fail because the check_dsp_reg routine checks for 2 bytes every time,
   but, only 1 byte is being set to 0x00 in the next 3 tests, which means the other byte will contain random values.
 */
          SetDWord(DSP_MC_RAM_END-3, 0x00000000);      /* Clear 4 bytes at DSP_MC_RAM_END-3 */

          SetWord(DEBUGGER_REG,0x00B4);
          SetByte(DSP_MC_RAM_END-3,0x00);
          if ( !check_dsp_reg(DSP_MC_RAM_END-3, DSP_MC_RAM_END_RESET, DSP_MC_RAM_END_RW_MASK,DSP_MC_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00B5);
          SetByte(DSP_MC_RAM_END-2,0x00);
          if ( !check_dsp_reg(DSP_MC_RAM_END-2, DSP_MC_RAM_END_RESET, DSP_MC_RAM_END_RW_MASK,DSP_MC_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00B6);
          SetByte(DSP_MC_RAM_END-1,0x00);
          if ( !check_dsp_reg(DSP_MC_RAM_END-1, DSP_MC_RAM_END_RESET, DSP_MC_RAM_END_RW_MASK,DSP_MC_RAM_END_ALLOWED,data_size )) return FALSE;
      
	  gprint("\n#################### part 2.2 DSP_MC_RAM passed %d%d%d ####################\n", 0,0,0 );

          /* SEQUENCER_RAM. */
          /* -------------- */
/* BRCM ADD: Clear RAM content first.  Without this, test will fail because the check_dsp_reg routine checks for 2 bytes every time,
   but, only 1 byte is being set to 0x00 in the next 3 tests, which means the other byte will contain random values.
 */
          SetDWord(SEQUENCER_RAM_START, 0x00000000);        /* Clear 4 bytes at SEQUENCER_RAM_START */
          SetDWord(SEQUENCER_RAM_START+4, 0x00000000);      /* Clear 4 bytes at SEQUENCER_RAM_START+4 */
                    
          SetWord(DEBUGGER_REG,0x00C0);
          SetByte(SEQUENCER_RAM_START,0x00);
          if ( !check_dsp_reg(SEQUENCER_RAM_START, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00C1);
          SetByte(SEQUENCER_RAM_START+1,0x00);
          if ( !check_dsp_reg(SEQUENCER_RAM_START+1, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00C2);
          SetByte(SEQUENCER_RAM_START+2,0x00);
          if ( !check_dsp_reg(SEQUENCER_RAM_START+2, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00C3);
          SetByte(SEQUENCER_RAM_START+3,0x00);
          if ( !check_dsp_reg(SEQUENCER_RAM_START+3, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

/* BRCM ADD: Clear RAM content first.  Without this, test will fail because the check_dsp_reg routine checks for 2 bytes every time,
   but, only 1 byte is being set to 0x00 in the next test, which means the other byte will contain random values.
 */
          SetDWord(SEQUENCER_RAM_START+512, 0x00000000);    /* Clear 4 bytes at +512 */

          /* Added to improve code coverage. */
          SetWord(DEBUGGER_REG,0x00C4);
          SetByte(SEQUENCER_RAM_START+512,0x00); /* Toggle SEQUENCER_RAM address bit 8. */
          if ( !check_dsp_reg(SEQUENCER_RAM_START+512, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

/* BRCM ADD: Clear RAM content first.  Without this, test will fail because the check_dsp_reg routine checks for 2 bytes every time,
   but, only 1 byte is being set to 0x00 in the next 3 tests, which means the other byte will contain random values.
 */
          SetDWord(SEQUENCER_RAM_END-3, 0x00000000);  /* Clear 4 bytes at SEQUENCER_RAM_END-3 */

          SetWord(DEBUGGER_REG,0x00C5);
          SetByte(SEQUENCER_RAM_END-3,0x00);
          if ( !check_dsp_reg(SEQUENCER_RAM_END-3, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00C6);
          SetByte(SEQUENCER_RAM_END-2,0x00);
          if ( !check_dsp_reg(SEQUENCER_RAM_END-2, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00C7);
          SetByte(SEQUENCER_RAM_END-1,0x00);
          if ( !check_dsp_reg(SEQUENCER_RAM_END-1, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;

      gprint("\n#################### All test are passed %d%d%d ####################\n", 0,0,0 );

    /* If we get this far, the complete test is OK. */
    return TRUE;
}

bool check_dsp_reg(uint32 reg, uint32 reset_value, uint32 mask_value, uint32 allowed, WORD data_size)
{
    BYTE data_byte;
    uint32 p, j;
    uint32 pattern;
    uint32 pat_lsb;

    /* First test: test reset value. */
    SetWord(DEBUGGER2_REG, (GetWord(DEBUGGER2_REG) & 0xFF00) + 0x0100);
    
      if (data_size == 0)
      {
        for (p=0;p<2;p++)
        {
          pat_lsb = (reset_value >> p*8) & 0xff;
          data_byte = GetByte(reg + p);
gprint("pat lsb: 0x%02x %d %d\n", pat_lsb, 0, 0); 
gprint("data byte: 0x%02x %d %d\n", data_byte, 0, 0);          
          if ( data_byte != pat_lsb ) { return FALSE; }    
        }
      }
      else if (data_size == 1)
      {
        pat_lsb = (reset_value >> 0) & 0xffff;
gprint("pat lsb: 0x%04x %d %d\n", pat_lsb, 0, 0); 
gprint("data byte: 0x%04x %d %d\n", GetWord(reg), 0, 0);          
        if ( GetWord(reg) != pat_lsb ) { return FALSE; }    
      }
      else if (data_size == 2)
      {
        pat_lsb = (reset_value >> 0) & 0xffffffff;
        if ( GetDWord(reg) != pat_lsb ) { return FALSE; }
      }
    
    /* Second test: write the inverse reset value to all allowed bits,
                    verify if all writable bits are changed.
    */
    SetWord(DEBUGGER2_REG, GetWord(DEBUGGER2_REG) + 0x0001);
    pattern =   reset_value & ~allowed;  /* Set not allowed bits to reset.     */
    pattern |= ~reset_value &  allowed;  /* Add allowed bits to inverse reset. */
    
      if (data_size == 0)
      {
        for (p=0;p<2;p++)
        {
          pat_lsb = (pattern >> p*8) & 0xff;     /* Make Byte. */
          SetByte ( (reg+p), pat_lsb );
          /*
           * Are all allowed writeable bits changed?
           * Are all read-only bits unchanged?
          */
          data_byte = GetByte(reg + p);
          if ( ( data_byte & ((mask_value >> p*8) & 0xff) & ((allowed >> p*8) & 0xff) ) != 
               ( ((pattern >> p*8) & 0xff) & ((mask_value >> p*8) & 0xff) & ((allowed >> p*8) & 0xff) ) ) { return FALSE; }
          if ( (data_byte & ((~mask_value >> p*8) & 0xff)) != 
             ( ((reset_value >> p*8) & 0xff) & ((~mask_value >> p*8) & 0xff) ) ) { return FALSE; }
        }  
      }
      else if (data_size == 1)
      {
        pat_lsb = (pattern >> 0 ) & 0xffff;      /* Make Word. */
        SetWord(reg, pat_lsb);        
        /*
         * Are all allowed writeable bits changed?
         * Are all read-only bits unchanged?
        */
        if ( ( GetWord(reg) &  mask_value & allowed ) != ( pattern & mask_value & allowed ) ) { return FALSE; }
        if ( ( GetWord(reg) & ~mask_value )           != ( reset_value & ~mask_value ) ) { return FALSE; }
      }
      
      else if (data_size == 2)
      {
        pat_lsb = (pattern >> 0 ) & 0xffffffff;  /* Make DWord. */
        SetDWord(reg, pat_lsb);        
        /*
         * Are all allowed writeable bits changed?
         * Are all read-only bits unchanged?
        */
        if ( ( GetDWord(reg) &  mask_value & allowed ) != ( pattern & mask_value & allowed ) ) { return FALSE; }
        if ( ( GetDWord(reg) & ~mask_value )           != ( reset_value & ~mask_value ) ) { return FALSE; }
      }

    /*
     * Third test: write all allowed bits byte wise via a walking one,
     *             verify all allowed bits word wise.
    */
    SetWord(DEBUGGER2_REG, GetWord(DEBUGGER2_REG) + 0x0001);
    pattern = 0x1; 
    
      if (data_size == 0)
      {
        for (j=0;j<2;j++)
        {
          for (p=0;p<8;p++)
          {
            SetWord(DEBUGGER2_REG, GetWord(DEBUGGER2_REG) + 0x0001);
            pat_lsb = ( ( (pattern & 0xff) & ((allowed >> j*8) & 0xff) ) | ( ((reset_value >> j*8) & 0xff) & ((~allowed>>j*8) & 0xff)) ) & 0xff;
            SetByte( (reg+j), pat_lsb );
            data_byte = GetByte(reg + j);
	          if ( ( data_byte & ((mask_value >> j*8) & 0xff) & ((allowed >> j*8) & 0xff) ) != ( (pattern & 0xff) & ((mask_value >> j*8) & 0xff) & ((allowed >> j*8) & 0xff) ) )
	          {
	  	          return FALSE;
	          }
	          if ( ( data_byte & ((~mask_value >> j*8) & 0xff) ) != ( ((reset_value >> j*8) & 0xff) & ((~mask_value >> j*8) & 0xff) ) )
	          {
	  	          return FALSE;
	          }
            pattern <<= 1;
          }
          pattern >>= 8;
        }
      }
      else if (data_size == 1)
      {
        for (p=0;p<16;p++)
        {
          SetWord(DEBUGGER2_REG, GetWord(DEBUGGER2_REG) + 0x0001);
          pat_lsb = ( ( (pattern & allowed) | (reset_value & ~allowed) ) >>  0 ) & 0xffff;
          SetWord(reg+0, pat_lsb);
	        if ( (GetWord(reg) & mask_value & allowed) != (pattern & mask_value & allowed) )
	        {
	  	        return FALSE;
	        }
	        if ( (GetWord(reg) & ~mask_value) != (reset_value & ~mask_value ) )
	        {
	  	        return FALSE;
	        }
          pattern <<= 1;
        }
      }
      else if (data_size == 2)
      {
        {
        SetWord(DEBUGGER2_REG, GetWord(DEBUGGER2_REG) + 0x0001);
        pat_lsb = ( ( (pattern & allowed) | (reset_value & ~allowed) ) >>  0 ) & 0xffffffff;
          SetDWord(reg+0, pat_lsb);
	        if ( (GetDWord(reg) & mask_value & allowed) != (pattern & mask_value & allowed) )
	        {
	  	        return FALSE;
	        }
	        if ( (GetDWord(reg) & ~mask_value) != (reset_value & ~mask_value) )
	        {
	  	        return FALSE;
	        }
          pattern <<= 1;
        }
      }  
    return TRUE;
}
