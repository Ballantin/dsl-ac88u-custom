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
 * Description: test halfword (16 bits) accessibility of AHB peripheral registers
 *
 * ------------------------------------------------------------------------------
 * Synchronicity history: 
 * 
 * $Log: dectip_ahb_halfword.c.rca $
 * 
 *  Revision: 1.12 Tue Feb 10 16:53:50 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.11 Tue Feb 10 16:52:45 2009 snijders
 *  Changed 'DBG_INOUT_REG' to 'DSP_DBG_INOUT_REG' according to the DS 0.42
 *  of 4-feb-2009.
 *  Added part 0 for testing the 'SetBits' macro and put in comment again..
 * 
 *  Revision: 1.10 Mon Feb  9 17:23:19 2009 snijders
 *  Added test register 'DEBUGGER2_REG'.
 *  Changed the usage of 'TEST_REG', 'DEBUGGER_REG' and 'DEBUGGER2_REG'.
 *  Added code to SEQUENCER_RAM of part 2.2 to improve code coverage.
 * 
 *  Revision: 1.9 Tue Feb  3 10:32:18 2009 snijders
 *  Changed 'DSP_JTBL_REG' to 'DSP_JTBL_START_REG'.
 * 
 *  Revision: 1.8 Mon Feb  2 15:32:43 2009 snijders
 *  Added "DSP_ZCROSS1_OUT_REG" & "DSP_ZCROSS2_OUT_REG" register test.
 *  Removed "DSP_RAM_IN3_REG+2" & "DSP_RAM_IN3_REG+4" register test.
 *  Updated now completely according the documented DS of 22-jan-2009, v0.41.
 * 
 *  Revision: 1.7 Fri Jan 23 11:45:13 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.6 Fri Jan 23 11:11:48 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.5 Thu Jan 22 15:35:45 MET 2009 snijders
 *  Updated a lot of comment.
 *  Added some examples of DEBUGGER_REG and TEST_REG usage.
 *  Removed the hexadecimal increments at DSP_RAM_IN3_REG (kept +26).
 *  Updated part "2.2: Memories (incl. boundary test)."
 *  Changed variable "i" to "p" in the Word part (data_size == 1) of the third
 *  test in 'check_dsp_reg'.
 *  Added (dummy) DSP and DIP ISR's.
 *  Updated the 'byte' part of the 'Second test' in 'check_dsp_reg':
 *    - changed the remaining 'i' to 'p',
 *    - added additional '(' and ')' in the second compare.
 *  Added "Part 1: Execute the most basic types of AHB access tests."
 * 
 *  Revision: 1.4 Mon Jan 19 16:37:49 MET 2009 snijders
 *  Updated the description.
 * 
 *  Revision: 1.3 Mon Jan 19 16:32:06 2009 snijders
 *  Changed 'spu_reg_test.h' to 'dectip_ahb_reg_test.h'.
 * 
 *  Revision: 1.2 Fri Jan 16 12:15:41 2009 snijders
 *  Updated "bool dectip_ahb_word" to "bool dectip_ahb_halfword" according
 *  to the new used filename.
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

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/*========================== Local macro definitions ==========================*/

/*========================== Global definitions ===============================*/

/*========================== Local function prototypes ========================*/
bool check_dsp_reg1(uint32 reg, uint32 reset_value, uint32 mask_value, uint32 allowed, WORD data_size);
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

int dectip_ahb_halfword(void) 
{
    /* Part 0: Test the 'SetBits' macro. */
    /* ================================= */
      /*
        SetWord(DEBUGGER_REG, 0xDB01);       
        SetBits(DSP_MAIN_CTRL_REG, DSP_MAIN_CTRL,2);
        SetWord(DEBUGGER_REG, 0xDB02);       
        SetBits(DSP_MAIN_CTRL_REG, DSP_MAIN_CTRL,1);   //  Just testing 'SetBits'.
        SetWord(DEBUGGER_REG, 0xDB03);       
        SetBits(DSP_CTRL_REG, DSP_CR16_INT, 1);
        SetWord(DEBUGGER_REG, 0xDB04);       
        SetBits(DSP_MAIN_CNT_REG, DSP_MAIN_REL, 0x8F); //  0-0x8F TICKS = 125 uSEC --> 8 KHZ
        SetWord(DEBUGGER_REG, 0xDB05);       
        SetBits(DSP_MAIN_CNT_REG, DSP_MAIN_REL, 0xAA); //  Just testing 'SetBits'.

        SetWord(DEBUGGER_REG, 0xDB06);       
        SetBits(DSP_MAIN_SYNC0_REG, RAMIN0_SYNC,  0); //4 KHz
        SetWord(DEBUGGER_REG, 0xDB07);       pat_lsb = (reset_value >> 0) & 0xffff;
        SetBits(DSP_MAIN_SYNC0_REG, RAMOUT0_SYNC, 0); //4 KHz
        SetWord(DEBUGGER_REG, 0xDB08);       
        SetBits(DSP_MAIN_SYNC0_REG, RAMIN2_SYNC,  1); //8 KHz
        SetWord(DEBUGGER_REG, 0xDB09);       
        SetBits(DSP_MAIN_SYNC0_REG, RAMOUT2_SYNC, 1); //8 KHz
      */


    /* Part 1: Execute the most basic types of AHB access tests. */
    /* ========================================================= */
#ifdef AHB_AUTO_SWAP
        /* Part 1.1: AHB Byte access. */
            SetByte(SHARED_RAM_START+0,0xAA);
            if ( GetByte(SHARED_RAM_START+0) != 0xAA ) return FALSE;

            SetByte(SHARED_RAM_START+1,0xBB);
            if ( GetByte(SHARED_RAM_START+1) != 0xBB ) return FALSE;

            SetByte(SHARED_RAM_START+2,0xCC);
            if ( GetByte(SHARED_RAM_START+2) != 0xCC ) return FALSE;

            SetByte(SHARED_RAM_START+3,0xDD);
            if ( GetByte(SHARED_RAM_START+3) != 0xDD ) return FALSE;
	        gprint("\n#################### part 1.1 passed %d%d%d ####################\n", 0,0,0 );

#endif
        /* Part 1.2: AHB Word access. */
            SetWord(SHARED_RAM_START+4,0x0123);
            if ( GetWord(SHARED_RAM_START+4) != 0x0123 ) return FALSE;
	    
            SetWord(SHARED_RAM_START+6,0x4567);
            if ( GetWord(SHARED_RAM_START+6) != 0x4567 ) return FALSE;
	        gprint("\n#################### part 1.2 passed %d%d%d ####################\n", 0,0,0 );

        /* Part 1.3: AHB Double Word access. */
            SetDWord(SHARED_RAM_START+8,0x89ABCDEF);
            if ( GetDWord(SHARED_RAM_START+8) != 0x89ABCDEF ) return FALSE;

	         gprint("\n#################### part 1 passed %d%d%d ####################\n", 0,0,0 );

    /* Part 2: Execute the more extensive access tests. */
    /* ================================================ */

      WORD data_size = dWord; //change for different data widths

      /* 2.1: Registers. */
      /* --------------- */
          SetWord(TEST_REG,DECTIP_AHB_HALFWORD); /* Write test number register. */
          SetWord(DEBUGGER_REG, 0x0000);         /* E.g.: to see where you are in 'dectip_ahb_halfword' (main program) during simulation. */
          SetWord(DEBUGGER2_REG,0x0000);         /* E.g.: to see where you are in 'check_dsp_reg' (function) during simulation. */
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
          SetWord(DEBUGGER_REG,0x00A0);
          SetWord(SHARED_RAM_START,0x0000);
          if ( !check_dsp_reg(SHARED_RAM_START, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00A1);
          SetWord(SHARED_RAM_START+2,0x0000);
          if ( !check_dsp_reg(SHARED_RAM_START+2, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00A2);
          SetWord(SHARED_RAM_START+4,0x0000);
          if ( !check_dsp_reg(SHARED_RAM_START+4, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00A3);
          SetWord(SHARED_RAM_END-1,0x0000);
          if ( !check_dsp_reg(SHARED_RAM_END-1, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00A4);
          SetWord(SHARED_RAM_END-3,0x0000);
          if ( !check_dsp_reg(SHARED_RAM_END-3, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00A5);
          SetWord(SHARED_RAM_END-5,0x0000);
          if ( !check_dsp_reg(SHARED_RAM_END-5, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;
	  gprint("\n#################### part 2.2 Shared RAM passed %d%d%d ####################\n", 0,0,0 );

          /* DSP_MC_RAM. */
          /* ----------- */
          SetWord(DEBUGGER_REG,0x00B0);   
          SetWord(DSP_MC_RAM_START,0x0000);
          if ( !check_dsp_reg(DSP_MC_RAM_START, DSP_MC_RAM_START_RESET, DSP_MC_RAM_START_RW_MASK,DSP_MC_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00B1);
          SetWord(DSP_MC_RAM_START+2,0x0000);
          if ( !check_dsp_reg(DSP_MC_RAM_START+2, DSP_MC_RAM_START_RESET, DSP_MC_RAM_START_RW_MASK,DSP_MC_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00B2);
          SetWord(DSP_MC_RAM_START+4,0x0000);
          if ( !check_dsp_reg(DSP_MC_RAM_START+4, DSP_MC_RAM_START_RESET, DSP_MC_RAM_START_RW_MASK,DSP_MC_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00B3);
          SetWord(DSP_MC_RAM_END-1,0x0000);
          if ( !check_dsp_reg(DSP_MC_RAM_END-1, DSP_MC_RAM_END_RESET, DSP_MC_RAM_END_RW_MASK,DSP_MC_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00B4);
          SetWord(DSP_MC_RAM_END-3,0x0000);
          if ( !check_dsp_reg(DSP_MC_RAM_END-3, DSP_MC_RAM_END_RESET, DSP_MC_RAM_END_RW_MASK,DSP_MC_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00B5);
          SetWord(DSP_MC_RAM_END-5,0x0000);
          if ( !check_dsp_reg(DSP_MC_RAM_END-5, DSP_MC_RAM_END_RESET, DSP_MC_RAM_END_RW_MASK,DSP_MC_RAM_END_ALLOWED,data_size )) return FALSE;
	  gprint("\n#################### part 2.2 DSP_MC_RAM passed %d%d%d ####################\n", 0,0,0 );

          /* SEQUENCER_RAM. */
          /* -------------- */
          SetWord(DEBUGGER_REG,0x00C0);
          SetWord(SEQUENCER_RAM_START,0x0000);
          if ( !check_dsp_reg(SEQUENCER_RAM_START, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00C1);
          SetWord(SEQUENCER_RAM_START+2,0x0000);
          if ( !check_dsp_reg(SEQUENCER_RAM_START+2, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00C2);
          SetWord(SEQUENCER_RAM_START+4,0x0000);
          if ( !check_dsp_reg(SEQUENCER_RAM_START+4, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          /* Added to improve code coverage. */
          SetWord(DEBUGGER_REG,0x00C3);
          SetWord(SEQUENCER_RAM_START+512,0x0000); /* Toggle SEQUENCER_RAM address bit 8. */
          if ( !check_dsp_reg(SEQUENCER_RAM_START+512, SHARED_RAM_START_RESET, SHARED_RAM_START_RW_MASK,SHARED_RAM_START_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00C4);
          SetWord(SEQUENCER_RAM_END-1,0x0000);
          if ( !check_dsp_reg(SEQUENCER_RAM_END-1, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00C5);
          SetWord(SEQUENCER_RAM_END-3,0x0000);
          if ( !check_dsp_reg(SEQUENCER_RAM_END-3, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;

          SetWord(DEBUGGER_REG,0x00C6);
          SetWord(SEQUENCER_RAM_END-5,0x0000);
          if ( !check_dsp_reg(SEQUENCER_RAM_END-5, SHARED_RAM_END_RESET, SHARED_RAM_END_RW_MASK,SHARED_RAM_END_ALLOWED,data_size )) return FALSE;

      gprint("\n#################### All test are passed %d%d%d ####################\n", 0,0,0 );

    /* If we get this far, the complete test is OK. */
    return TRUE;
}

#if 0    
bool check_dsp_reg1(uint32 reg, uint32 reset_value, uint32 mask_value, uint32 allowed, WORD data_size)
{
    BYTE data_byte;
    uint32 p, j;
    uint32 pattern;
    uint32 pat_lsb;
    unsigned short pat_16;
    
#if 1    
    /* First test: test reset value. */
    SetWord(DEBUGGER2_REG, GetWord(DEBUGGER2_REG) + 0x0100);
    
      if (data_size == 0)
      {
        for (p=0;p<2;p++)
        {
          pat_lsb = (reset_value >> p*8) & 0xff;
          data_byte = GetByte(reg + p);
          if ( data_byte != pat_lsb ) { return FALSE; }
        }
      }
      else if (data_size == 1)
      {
        pat_lsb = (reset_value >> 0) & 0xffff;
        if ( GetWord(reg) != pat_lsb ) { return FALSE; }    
      }
      else if (data_size == 2)
      {
        pat_lsb = (reset_value >> 0) & 0xffffffff;
        if ( GetDWord(reg) != pat_lsb ) { return FALSE; }
      }
#endif 

    /* Second test: write the inverse reset value to all allowed bits,
                    verify if all writable bits are changed.
    */
    SetWord(DEBUGGER2_REG, GetWord(DEBUGGER2_REG) + 0x0001);
    pattern =   reset_value & ~allowed;  /* Set not allowed bits to reset.     */
    pattern |= ~reset_value &  allowed;  /* Add allowed bits to inverse reset. */
#if 0    
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
      else
#endif 
      if (data_size == 1)
      {
        pat_16 = (pattern >> 0 ) & 0xffff;      /* Make Word. */
        SetWord(reg, pat_16);        
        /*
         * Are all allowed writeable bits changed?
         * Are all read-only bits unchanged?
        */
        if ( ( GetWord(reg) &  mask_value & allowed ) != ( pattern & mask_value & allowed ) ) { 

	    gprint("\n############ GetWord 0x%04X patten 0x%08X allowed 0x%08X \n", GetWord(reg), pattern, allowed );
	    gprint("\n############ pat_16 0x%03X mask_value 0x%08X reg 0x%08X \n", pat_16, mask_value, reg );
		return FALSE; }
        if ( ( GetWord(reg) & ~mask_value )           != ( reset_value & ~mask_value ) ) { 
		
	    gprint("\n############ SYNC0_REG 0x%04X 0x%08X 0x08X \n", GetWord(reg), pat_lsb, mask_value );
		return FALSE; }
      }
#if 0      
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
            SetByte( (reg+j), pat_lsb );    BYTE data_byte;
            data_byte = GetByte(reg + j);
	          if ( ( data_byte & ((mask_value >> j*8) & 0xff) & ((allowed >> j*8) & 0xff) ) != ( (pattern & 0xff) & ((mask_value >> j*8) & 0xff) & ((allowed >> j*8) & 0xff) ) )    BYTE data_byte;
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
#endif
    return TRUE;
}
#endif

