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
 * Description: test halfword (16 bits) accessibility of APB peripheral registers
 *
 * ------------------------------------------------------------------------------
 * Synchronicity history: 
 * 
 * $Log: dectip_apb.c.rca $
 * 
 *  Revision: 1.9 Fri Feb 13 11:47:13 2009 snijders
 *  Updated according to the documented DS v0.43:
 *    - CLK_PLL_CTRL_REG_ALLOWED (0x0000ffff & 0xffff000f)
 *      to
 *      CLK_PLL_CTRL_REG_ALLOWED (0x0000fff7 & 0xffff000f).
 * 
 *  Revision: 1.8 Thu Feb 12 14:33:44 2009 snijders
 *  Updated now completely according the documented DS of 04-feb-2009, v0.42.
 * 
 *  Revision: 1.7 Thu Feb  5 12:54:07 2009 snijders
 *  Added test register 'DEBUGGER2_REG'.
 *  Changed the usage of 'TEST_REG', 'DEBUGGER_REG' and 'DEBUGGER2_REG'.
 * 
 *  Revision: 1.6 Thu Jan 29 13:55:45 2009 snijders
 *  Added some comment. Added 'check_apb_dc_reg' (incl. "dip.h", SEQ define).
 *  Added the  DIP_DC* registers. Added the read-only IP VERSION REGISTERS.
 *  Updated now completely according the documented DS of 22-jan-2009, v0.41.
 * 
 *  Revision: 1.4 Fri Jan 23 11:25:53 2009 snijders
 *  Updated a lot of comment.
 *  Added some examples of DEBUGGER_REG and TEST_REG usage.
 *  Added (dummy) DSP and DIP ISR's.
 * 
 *  Revision: 1.3 Mon Dec 22 15:12:11 2008 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.2 Mon Dec 22 14:32:59 2008 snijders
 *  Added official SiTel header.
 * 
 * ------------------------------------------------------------------------------
 * Remarks:
 *
 *  1) This file is originally created with 'reg_test_generator.exe'.
 *
 *     To prevent unwanted side effects, the register bit-fields below
 *     are always excluded from the generated testcode
 *
 *         DIP_CTRL_REG [ URST ]
 *         DIP_CTRL_REG [ DIP_BRK_INT ]
 *         DIP_CTRL_REG [ PD1_INT ]
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
#include "dip.h"

/*========================== Local macro definitions ==========================*/

#define SEQ(adr,opc,opr)     SetWord((SEQUENCER_RAM_START + (adr<<1)),(opc)<<8|(opr));

/* 
 * <register>_RESET, _RW_MASK and _ALLOWED are defined as follows:
 *   Example:
 *     #define <register>_RESET   0x00000000                 // Reset value. 
 *     #define <register>_RW_MASK 0x0000009e                 // All R/W bits only.
 *     #define <register>_ALLOWED (0x0000ff7f & 0xffff009f)  // All functional allowable W bits (incl. - and R bits) & all R/W and R bits only.
*/

#define BMC_CTRL_REG_RESET   0x00000000
#define BMC_CTRL_REG_RW_MASK 0x00002100
#define BMC_CTRL_REG_ALLOWED (0x0000ffff & 0xffff2100)

#define BMC_CTRL2_REG_RESET   0x00000000
#define BMC_CTRL2_REG_RW_MASK 0x000003f8
#define BMC_CTRL2_REG_ALLOWED (0x0000ffff & 0xffff03f8)

#define BMC_TX_SFIELDL_REG_RESET   0x0000e98a
#define BMC_TX_SFIELDL_REG_RW_MASK 0x0000ffff
#define BMC_TX_SFIELDL_REG_ALLOWED (0x0000ffff & 0xffffffff)

#define BMC_TX_SFIELDH_REG_RESET   0x0000aaaa
#define BMC_TX_SFIELDH_REG_RW_MASK 0x0000ffff
#define BMC_TX_SFIELDH_REG_ALLOWED (0x0000ffff & 0xffffffff)

#define BMC_RX_SFIELDL_REG_RESET   0x00009755
#define BMC_RX_SFIELDL_REG_RW_MASK 0x0000ffff
#define BMC_RX_SFIELDL_REG_ALLOWED (0x0000ffff & 0xffffffff)

#define BMC_RX_SFIELDH_REG_RESET   0x00000051
#define BMC_RX_SFIELDH_REG_RW_MASK 0x000000ff
#define BMC_RX_SFIELDH_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define CLK_CODEC_DIV_REG_RESET   0x00000009
#define CLK_CODEC_DIV_REG_RW_MASK 0x000000ff
#define CLK_CODEC_DIV_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define CLK_PLL_CTRL_REG_RESET   0x00000000
#define CLK_PLL_CTRL_REG_RW_MASK 0x0000000f
#define CLK_PLL_CTRL_REG_ALLOWED (0x0000fff7 & 0xffff000f)

#define DIP_STACK_REG_RESET   0x00000000
#define DIP_STACK_REG_RW_MASK 0x00000000
#define DIP_STACK_REG_ALLOWED (0x0000ffff & 0xffff03ff)

#define DIP_PC_REG_RESET   0x00000000
#define DIP_PC_REG_RW_MASK 0x00000000
#define DIP_PC_REG_ALLOWED (0x0000ffff & 0xffff03ff)

#define DIP_STATUS_REG_RESET   0x00000080
#define DIP_STATUS_REG_RW_MASK 0x00000000
#define DIP_STATUS_REG_ALLOWED (0x0000ffff & 0xffff00bf)

#define DIP_CTRL_REG_RESET   0x00000080
#define DIP_CTRL_REG_RW_MASK 0x000000b0
#define DIP_CTRL_REG_ALLOWED (0x0000ff4f & 0xffff00bf)

#define DIP_STATUS1_REG_RESET   0x00000000
#define DIP_STATUS1_REG_RW_MASK 0x00000000
#define DIP_STATUS1_REG_ALLOWED (0x0000ffff & 0xffff000f)

#define DIP_CTRL1_REG_RESET   0x00000000
#define DIP_CTRL1_REG_RW_MASK 0x00000000
#define DIP_CTRL1_REG_ALLOWED (0x0000ffff & 0xffff000f)

#define DIP_SLOT_NUMBER_REG_RESET   0x00000000
#define DIP_SLOT_NUMBER_REG_RW_MASK 0x00000000
#define DIP_SLOT_NUMBER_REG_ALLOWED (0x0000ffff & 0xffff001f)

#define DIP_CTRL2_REG_RESET   0x00000000
#define DIP_CTRL2_REG_RW_MASK 0x00000f81
#define DIP_CTRL2_REG_ALLOWED (0x0000ffff & 0xffff0fb7)

#define DIP_MOD_SEL_REG_RESET   0x00000000
#define DIP_MOD_SEL_REG_RW_MASK 0x000000ff
#define DIP_MOD_SEL_REG_ALLOWED (0x0000ffff & 0xffff00ff)

#define DIP_MOD_VAL_REG_RESET   0x00004f27
#define DIP_MOD_VAL_REG_RW_MASK 0x0000ffff
#define DIP_MOD_VAL_REG_ALLOWED (0x0000ffff & 0xffffffff)

#define DIP_DC01_REG_RESET   0x00000000
#define DIP_DC01_REG_RW_MASK 0x0000ffff
#define DIP_DC01_REG_ALLOWED (0x0000ffff & 0xffffffff)

#define DIP_DC23_REG_RESET   0x00000000
#define DIP_DC23_REG_RW_MASK 0x00000fff
#define DIP_DC23_REG_ALLOWED (0x0000ffff & 0xffff0fff)

#define DIP_DC34_REG_RESET   0x00000000
#define DIP_DC34_REG_RW_MASK 0x000000ff
#define DIP_DC34_REG_ALLOWED (0x0000ffff & 0xffff00ff)

/*========================== Global definitions ===============================*/

/*========================== Local function prototypes ========================*/
bool check_apb_reg(uint32 reg, uint32 reset_value, uint32 mask_value, uint32 allowed);
bool check_apb_dc_reg(uint32 reg, uint32 reset_value, uint32 mask_value, uint32 allowed);
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

int dectip_apb(void) 
{
    SetWord(TEST_REG,DECTIP_APB);  /* Write test number register. */
    SetWord(DEBUGGER_REG, 0x0000); /* E.g.: to see where you are in 'dectip_apb' (main program) during simulation. */
    SetWord(DEBUGGER2_REG,0x0000); /* E.g.: to see where you are in 'check_apb_reg' (function) during simulation. */

    gprint("\n#################### dectip_apb test start %d%d%d ####################\n", 0,0,0 );

    /* In the 'check_apb_dc_reg' test the DIP will be turned on, so put some usefull basic code in the Sequencer/TIM RAM. */
    SEQ(0x01,   WT,  0x01);
    SEQ(0x02,   BR,  0x01);

    /* Part A: BMC REGISTERS */
    SetWord(DEBUGGER_REG,0xA000);
    if ( !check_apb_reg(BMC_CTRL_REG, BMC_CTRL_REG_RESET, BMC_CTRL_REG_RW_MASK, BMC_CTRL_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xA002);
    if ( !check_apb_reg(BMC_CTRL2_REG, BMC_CTRL2_REG_RESET, BMC_CTRL2_REG_RW_MASK, BMC_CTRL2_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xA004);
    if ( !check_apb_reg(BMC_TX_SFIELDL_REG, BMC_TX_SFIELDL_REG_RESET, BMC_TX_SFIELDL_REG_RW_MASK, BMC_TX_SFIELDL_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xA006);
    if ( !check_apb_reg(BMC_TX_SFIELDH_REG, BMC_TX_SFIELDH_REG_RESET, BMC_TX_SFIELDH_REG_RW_MASK, BMC_TX_SFIELDH_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xA008);
    if ( !check_apb_reg(BMC_RX_SFIELDL_REG, BMC_RX_SFIELDL_REG_RESET, BMC_RX_SFIELDL_REG_RW_MASK, BMC_RX_SFIELDL_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xA00A);
    if ( !check_apb_reg(BMC_RX_SFIELDH_REG, BMC_RX_SFIELDH_REG_RESET, BMC_RX_SFIELDH_REG_RW_MASK, BMC_RX_SFIELDH_REG_ALLOWED )) return FALSE;
    gprint("\n#################### Part A: BMC Registers passed %d%d%d ####################\n", 0,0,0 );

    /* Part B: CLK GEN */
    SetWord(DEBUGGER_REG,0xB102);
    if ( !check_apb_reg(CLK_CODEC_DIV_REG, CLK_CODEC_DIV_REG_RESET, CLK_CODEC_DIV_REG_RW_MASK, CLK_CODEC_DIV_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xB110);
    if ( !check_apb_reg(CLK_PLL_CTRL_REG, CLK_PLL_CTRL_REG_RESET, CLK_PLL_CTRL_REG_RW_MASK, CLK_PLL_CTRL_REG_ALLOWED )) return FALSE;
    gprint("\n#################### Part B: CLK GEN passed %d%d%d ####################\n", 0,0,0 );

    /* Part C: DIP REGISTERS */
    SetWord(DEBUGGER_REG,0xC200);
    if ( !check_apb_reg(DIP_STACK_REG, DIP_STACK_REG_RESET, DIP_STACK_REG_RW_MASK, DIP_STACK_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC202);
    if ( !check_apb_reg(DIP_PC_REG, DIP_PC_REG_RESET, DIP_PC_REG_RW_MASK, DIP_PC_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC204);
    if ( !check_apb_reg(DIP_STATUS_REG, DIP_STATUS_REG_RESET, DIP_STATUS_REG_RW_MASK, DIP_STATUS_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC206);
    if ( !check_apb_reg(DIP_CTRL_REG, DIP_CTRL_REG_RESET, DIP_CTRL_REG_RW_MASK, DIP_CTRL_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC208);
    if ( !check_apb_reg(DIP_STATUS1_REG, DIP_STATUS1_REG_RESET, DIP_STATUS1_REG_RW_MASK, DIP_STATUS1_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC20A);
    if ( !check_apb_reg(DIP_CTRL1_REG, DIP_CTRL1_REG_RESET, DIP_CTRL1_REG_RW_MASK, DIP_CTRL1_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC20C);
    if ( !check_apb_reg(DIP_SLOT_NUMBER_REG, DIP_SLOT_NUMBER_REG_RESET, DIP_SLOT_NUMBER_REG_RW_MASK, DIP_SLOT_NUMBER_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC20E);
    if ( !check_apb_reg(DIP_CTRL2_REG, DIP_CTRL2_REG_RESET, DIP_CTRL2_REG_RW_MASK, DIP_CTRL2_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC212);
    if ( !check_apb_reg(DIP_MOD_SEL_REG, DIP_MOD_SEL_REG_RESET, DIP_MOD_SEL_REG_RW_MASK, DIP_MOD_SEL_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC214);
    if ( !check_apb_reg(DIP_MOD_VAL_REG, DIP_MOD_VAL_REG_RESET, DIP_MOD_VAL_REG_RW_MASK, DIP_MOD_VAL_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC216);
    if ( !check_apb_dc_reg(DIP_DC01_REG, DIP_DC01_REG_RESET, DIP_DC01_REG_RW_MASK, DIP_DC01_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC218);
    if ( !check_apb_dc_reg(DIP_DC23_REG, DIP_DC23_REG_RESET, DIP_DC23_REG_RW_MASK, DIP_DC23_REG_ALLOWED )) return FALSE;
    SetWord(DEBUGGER_REG,0xC21A);
    if ( !check_apb_reg(DIP_DC34_REG, DIP_DC34_REG_RESET, DIP_DC34_REG_RW_MASK, DIP_DC34_REG_ALLOWED )) return FALSE;
    gprint("\n#################### Part C: DIP Registers passed %d%d%d ####################\n", 0,0,0 );

    /* Part D: IP VERSION REGISTERS (read-only registers) */
    SetWord(DEBUGGER_REG,0xD000);
    //SetByte (IP_ID1_REG, 0x00); /* Just to show as an example that a APB write byte access is not permitted (simulation will stop when activated). */

    SetWord(IP_ID1_REG,0xFFFF); /* Just to show that the IP version registers are read-only registers */
    SetWord(IP_ID3_REG,0xFFFF); /* and that they can not be altered, see the check below.             */

    SetWord(DEBUGGER_REG,0xD300);
    if ( GetWord (IP_ID1_REG)      != 0x3032 ) return FALSE;

    //SetWord(DEBUGGER_REG,0xD301);
    //if ( GetByte (IP_ID2_REG)      != 0x30 ) return FALSE;

    SetWord(DEBUGGER_REG,0xD302);
    if ( GetWord (IP_ID3_REG)      != 0x1030 ) return FALSE;

    //SetWord(DEBUGGER_REG,0xD303);
    //if ( GetByte (IP_REVISION_REG) != 0x10 ) return FALSE;
    gprint("\n#################### Part D: IP Version registers and all tests passed %d%d%d ####################\n", 0,0,0 );


    /* If we get this far, the complete test is OK. */
    return TRUE;
}

bool check_apb_reg(uint32 reg, uint32 reset_value, uint32 mask_value, uint32 allowed)
{
    uint16 j;
    uint32 pattern;
    uint32 pat_lsb;
    /*
     * First test: test reset value.
    */
    SetWord(DEBUGGER2_REG, 0x0f);
    pat_lsb = reset_value & 0xffff;
    if ( GetWord(reg) != pat_lsb ) { return FALSE; }

    /*
     * Second test: write the inverse reset value to all allowed bits,
     *              verify if all writable bits are changed.
    */
    SetWord(DEBUGGER2_REG, 0x0e);
    pattern =   reset_value & ~allowed;  /* Set not allowed bits to reset.     */
    pattern |= ~reset_value &  allowed;  /* Add allowed bits to inverse reset. */
    pat_lsb = pattern & 0xffff;          /* Make byte 2.                       */
    SetWord(reg, pat_lsb);
    /*
     * Are all allowed writable bits changed?
     * Are all read-only bits unchanged?
    */
    if ( ( GetWord(reg) &  mask_value & allowed ) != ( pattern &  mask_value & allowed ) ) { return FALSE; }
    if ( ( GetWord(reg) & ~mask_value )           != ( reset_value & ~mask_value ) ) { return FALSE; }

    /*
     * Third test: write all allowed bits byte wise via a walking one,
     *             verify all allowed bits word wise.
    */
    SetWord(DEBUGGER2_REG, 0x00);
    pattern = 0x0281;
    for(j=0;j<8;j++)
    {
        SetWord(DEBUGGER2_REG, j+1);
        pat_lsb = ( ( (pattern & allowed) | (reset_value & ~allowed) ) ) & 0xffff;
        SetWord(reg, pat_lsb);
        /* 
         * And check. 
        */
	    if ( ( GetWord(reg) & mask_value & allowed ) != ( pattern & mask_value & allowed ) ) { return FALSE; }
	    if ( ( GetWord(reg) & ~mask_value ) != ( reset_value & ~mask_value ) ) { return FALSE; }
        pattern <<= 1;
    }
    return TRUE;
}

bool check_apb_dc_reg(uint32 reg, uint32 reset_value, uint32 mask_value, uint32 allowed)
{
    uint16 j;
    uint32 pattern;
    uint32 pat_lsb;
    uint32 initial_data;
    /*
     * First test: test reset value.
    */
    SetWord(DIP_CTRL_REG, 0x0000); /* Values will be copied and read from mc1, therefore DIP should be turned on. */

    SetWord(DEBUGGER2_REG, 0x100f);
    pat_lsb = (reset_value ) & 0xffff;
    if ( GetWord(reg) != pat_lsb ) { return FALSE; }

    /*
     * Second test: write the inverse reset value to all allowed bits,
     *              verify if all writable bits are changed.
    */
    SetWord(DEBUGGER2_REG, 0x100e);
    pattern =   reset_value & ~allowed;  /* Set not allowed bits to reset.     */
    pattern |= ~reset_value &  allowed;  /* Add allowed bits to inverse reset. */
    SetWord(RESULT_REG, pattern);
    pat_lsb = (pattern ) & 0xffff;       /* Make byte 2.                       */ 
    SetWord(RESULT_REG, pat_lsb);
    initial_data = GetWord(reg);
    SetWord(reg, pat_lsb);
    SetWord(DIP_DC01_REG, pat_lsb);      /* For toggle mechanism apb/mc1 domain, write towards DC01 is needed. */ 
    /* 
     * Wait (with dummy reads) until 'reg' is changed. 
    */
    //initial_data = GetWord(reg);
    while ( GetWord(reg) == initial_data );
    /*
     * Are all allowed writable bits changed?
     * Are all read-only bits unchanged?
    */
    SetWord(RESULT_REG, GetWord(reg));
    SetWord(DEBUGGER2_REG, 0x1001);
    if ( ( GetWord(reg) &  mask_value & allowed ) != ( pattern &  mask_value & allowed ) ) { return FALSE; }
    SetWord(DEBUGGER2_REG, 0x1002);
    if ( ( GetWord(reg) & ~mask_value )           != ( reset_value & ~mask_value ) ) { return FALSE; }

    /*
     * Third test: write all allowed bits byte wise via a walking one,
     *             verify all allowed bits word wise.
    */
    SetWord(DEBUGGER2_REG, 0x1003);
    pattern = 0x0281;
    for(j=0;j<8;j++)
    {
        SetWord(DEBUGGER2_REG, j+1);
        pat_lsb = ( ( (pattern & allowed) | (reset_value & ~allowed) ) ) & 0xffff;
        initial_data = GetWord(reg);
        SetWord(reg, pat_lsb); 
        SetWord(DIP_DC01_REG, pat_lsb);   /* For toggle mechanism apb/mc1 domain, write towards DC01 is needed. */
        /* 
         * Wait (with dummy reads) until 'reg' is changed. 
        */
        //initial_data = GetWord(reg);
        while (GetWord(reg) == initial_data);
        /* 
         * And check. 
        */
	    if ( (GetWord(reg) & mask_value & allowed ) != ( pattern & mask_value & allowed) ) { return FALSE; }
	    if ( (GetWord(reg) & ~mask_value ) != ( reset_value & ~mask_value ) ) { return FALSE; }
        pattern <<= 1;
    }
    SetWord(DIP_CTRL_REG, 0x0080);  /* Turn off DIP. */
    return TRUE;
}
