/*
 * -----------------------------------------------------------------------------
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
 * Description: datasheet header file
 *
 * ----------------------------------------------------------------------------- 
 * Synchronicity history: 
 * 
 * $Log: datasheet.h.rca $
 * 
 *  Revision: 1.18 Tue Feb 10 16:29:10 2009 snijders
 *  Updated comment/descriptions.
 *  Renamed 'DBG_IREG'   to 'DSP_DBG_IREG'.
 *  Renamed 'DBG_INOUT*' to 'DSP_DBG_INOUT*'.
 *  Removed 'DSP_PATCH' in 'struct __DSP_CTRL_REG'.
 *  Removed 'BITFLD_PCM_DOMAIN' in 'struct __DSP_MAIN_CNT_REG'.
 *  Updated now completely according the documented DS of 04-feb-2009, v0.42.
 * 
 *  Revision: 1.17 Thu Feb  5 11:24:36 2009 snijders
 *  Added 'DEBUGGER2_REG' and 'CONFIG_REG'.
 * 
 *  Revision: 1.16 Tue Feb  3 14:37:20 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.15 Tue Feb  3 10:32:52 2009 snijders
 *  Changed 'DSP_JTBL_REG' to 'DSP_JTBL_START_REG'.
 * 
 *  Revision: 1.14 Mon Feb  2 11:48:10 2009 snijders
 *  Added DSP_ZCROSS1_OUT_REG & DSP_ZCROSS2_OUT_REG.
 *  Updated now completely according the documented DS of 22-jan-2009, v0.41.
 * 
 *  Revision: 1.13 Fri Jan 30 15:25:53 2009 laarhove
 *  ready debugger test
 * 
 *  Revision: 1.12 Wed Jan 28 17:41:23 2009 laarhove
 *  added several tvc registers
 * 
 *  Revision: 1.11 Wed Jan 28 12:09:23 2009 snijders
 *  Checkin by Simon at request of PvL.
 * 
 *  Revision: 1.9 Thu Jan 15 15:30:24 2009 laarhove
 *  added several ahb and int tests
 * 
 *  Revision: 1.8 Mon Dec 22 14:52:58 2008 snijders
 *  Small update.
 * 
 *  Revision: 1.7 Mon Dec 22 14:47:26 2008 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.6 Thu Dec 18 13:40:05 2008 laarhove
 *  Fixed apb en ahb test.
 * 
 *  Revision: 1.5 Thu Dec 18 11:52:19 2008 snijders
 *  Added the official SiTel header.
 *  Added the IP VERSION REGISTERS. 
 * 
 * ----------------------------------------------------------------------------- 
*/ 

//typedef unsigned short WORD; 

/* 
** vvvvv TEMPORARY ONLY!!! - for compile and basic framework testing!!!
*/
extern unsigned char test_buf[40];
//static unsigned char test_buf2[1200];
//static unsigned char test_buf3[33000];

//#define AHB_BASE_ADR 0xB0000100

#define AHB_BASE_ADR 0xB06F0000
#define APB_BASE_ADR 0xB000A800
#define TST_BASE_ADR &test_buf[0] //0xFFFF0000
/* 
** ^^^^^ TEMPORARY ONLY!!! - for compile and basic framework testing!!!
*/


/* TEST REGISTERS */
#define TEST_REG               (TST_BASE_ADR + 0x0000000)  /* Test number register. */
#define RESULT_REG             (TST_BASE_ADR + 0x0000002)  /* Result register. */
#define DEBUGGER_REG           (TST_BASE_ADR + 0x0000004)  /* Debugger 1 register. */
#define DEBUGGER2_REG          (TST_BASE_ADR + 0x0000006)  /* Debugger 2 register. */
#define CONFIG_REG             (TST_BASE_ADR + 0x0000008)  /* Configuration register. */
#define WTF_COUNTER            (TST_BASE_ADR + 0x0000010)  /* Event counter. */
#define ZC1_COUNTER            (TST_BASE_ADR + 0x0000012)  /* Event counter. */
#define ZC2_COUNTER            (TST_BASE_ADR + 0x0000014)  /* Event counter. */
#define DSP_IRQ_OUT_COUNTER    (TST_BASE_ADR + 0x0000016)  /* Event counter. */
#define DSP_INT_EXT            (TST_BASE_ADR + 0x0000018)  /* */
#define DSP_BREAK_OUT_COUNTER  (TST_BASE_ADR + 0x0000020)  /* Event counter. */
#define DSP_BREAK_IN           (TST_BASE_ADR + 0x0000022)  /* */

/* BMC REGISTERS */
#define BMC_CTRL_REG           (APB_BASE_ADR + 0x0000000)  /* BMC control register. */
#define BMC_CTRL2_REG          (APB_BASE_ADR + 0x0000002)  /* BMC control register 2. */
#define BMC_TX_SFIELDL_REG     (APB_BASE_ADR + 0x0000004)  /* BMC Tx S field register Low. */
#define BMC_TX_SFIELDH_REG     (APB_BASE_ADR + 0x0000006)  /* BMC Tx S field register High. */
#define BMC_RX_SFIELDL_REG     (APB_BASE_ADR + 0x0000008)  /* BMC Rx S field register Low. */
#define BMC_RX_SFIELDH_REG     (APB_BASE_ADR + 0x000000A)  /* BMC Rx S field register High. */

/* CLK GEN */
#define CLK_CODEC_DIV_REG      (APB_BASE_ADR + 0x0000102)  /* Codec divider register. */
#define CLK_PLL_CTRL_REG       (APB_BASE_ADR + 0x0000110)  /* PLL control register. */

/* DIP REGISTERS */
#define DIP_STACK_REG          (APB_BASE_ADR + 0x0000200)  /* DIP Stack pointer register (read only). The DIP stack is 5 deep. */
#define DIP_PC_REG             (APB_BASE_ADR + 0x0000202)  /* DIP program counter register. */
#define DIP_STATUS_REG         (APB_BASE_ADR + 0x0000204)  /* DIP Status register. */
#define DIP_CTRL_REG           (APB_BASE_ADR + 0x0000206)  /* DIP Control register 1. */
#define DIP_STATUS1_REG        (APB_BASE_ADR + 0x0000208)  /* DIP Status register 1. */
#define DIP_CTRL1_REG          (APB_BASE_ADR + 0x000020A)  /* DIP Control register, clears DIP_INT if read. */
#define DIP_SLOT_NUMBER_REG    (APB_BASE_ADR + 0x000020C)  /* DIP slot number register, returns the current slot number. */
#define DIP_CTRL2_REG          (APB_BASE_ADR + 0x000020E)  /* DIP Control register 2 (debug status information). */
#define DIP_MOD_SEL_REG        (APB_BASE_ADR + 0x0000212)  /* DIP Modulo counters selection register. */
#define DIP_MOD_VAL_REG        (APB_BASE_ADR + 0x0000214)  /* DIP Modulo values selection register. */
#define DIP_DC01_REG           (APB_BASE_ADR + 0x0000216)  /* DIP Frame and Multiframe number 0-11 register. */
#define DIP_DC23_REG           (APB_BASE_ADR + 0x0000218)  /* DIP Multiframe number 12-23 register. */
#define DIP_DC34_REG           (APB_BASE_ADR + 0x000021A)  /* DIP DCS IV28-35 register. */

/* IP VERSION REGISTERS */
#define IP_ID1_REG             (APB_BASE_ADR + 0x0000300)  /* IP Identification register 1. */
#define IP_ID2_REG             (APB_BASE_ADR + 0x0000301)  /* IP Identification register 2. */
#define IP_ID3_REG             (APB_BASE_ADR + 0x0000302)  /* IP Identification register 3. */
#define IP_REVISION_REG        (APB_BASE_ADR + 0x0000303)  /* IP Revision version register. */

/* SPU IO REGISTERS */
#define DSP_MAIN_SYNC0_REG     (AHB_BASE_ADR + 0x0007F80)  /* DSP main counter outputs selection register 0. */
#define DSP_MAIN_SYNC1_REG     (AHB_BASE_ADR + 0x0007F82)  /* DSP main counter outputs selection register 1. */
#define DSP_MAIN_CNT_REG       (AHB_BASE_ADR + 0x0007F84)  /* DSP main counter register */

#define DSP_RAM_OUT0_REG       (AHB_BASE_ADR + 0x0007F92)  /* Shared RAM output register 0. */
#define DSP_RAM_OUT1_REG       (AHB_BASE_ADR + 0x0007F94)  /* Shared RAM output register 1. */
#define DSP_RAM_OUT2_REG       (AHB_BASE_ADR + 0x0007F96)  /* Shared RAM output register 2. */
#define DSP_RAM_OUT3_REG       (AHB_BASE_ADR + 0x0007F98)  /* Shared RAM output register 3. */
#define DSP_RAM_IN0_REG        (AHB_BASE_ADR + 0x0007F9A)  /* Shared RAM input register 0. */
#define DSP_RAM_IN1_REG        (AHB_BASE_ADR + 0x0007F9C)  /* Shared RAM input register 1. */
#define DSP_RAM_IN2_REG        (AHB_BASE_ADR + 0x0007F9E)  /* Shared RAM input register 2. */
#define DSP_RAM_IN3_REG        (AHB_BASE_ADR + 0x0007FA0)  /* Shared RAM input register 3. */
#define DSP_ZCROSS1_OUT_REG    (AHB_BASE_ADR + 0x0007FA2)  /* ZERO CROSSING 1 output register. */
#define DSP_ZCROSS2_OUT_REG    (AHB_BASE_ADR + 0x0007FA4)  /* ZERO CROSSING 2 output register. */

#define DSP_MAIN_CTRL_REG      (AHB_BASE_ADR + 0x0007FBC)  /* DSP Main counter control and preset value register. */

/* SPU CONTROL REGISTERS */
#define DSP_CTRL_REG           (AHB_BASE_ADR + 0x0007FD0)  /* DSP control register. */
#define DSP_PC_REG             (AHB_BASE_ADR + 0x0007FD2)  /* DSP Program counter register. */
#define DSP_PC_START_REG       (AHB_BASE_ADR + 0x0007FD4)  /* DSP Program counter start register. */
#define DSP_IRQ_START_REG      (AHB_BASE_ADR + 0x0007FD6)  /* DSP Interrupt vector start register. */
#define DSP_INT_REG            (AHB_BASE_ADR + 0x0007FD8)  /* DSP to System bus interrupt vector register. */
#define DSP_INT_MASK_REG       (AHB_BASE_ADR + 0x0007FDA)  /* DSP to System bus interrupt vector mask register. */
#define DSP_INT_PRIO1_REG      (AHB_BASE_ADR + 0x0007FDC)  /* DSP Interrupt priority mux 1 register. */
#define DSP_INT_PRIO2_REG      (AHB_BASE_ADR + 0x0007FDE)  /* DSP Interrupt priority mux 2 register. */
#define DSP_OVERFLOW_REG       (AHB_BASE_ADR + 0x0007FE0)  /* DSP to System bus Interrupt overflow register. */
#define DSP_JTBL_START_REG     (AHB_BASE_ADR + 0x0007FE2)  /* DSP jump table start address register. */

#define DSP_DBG_IREG           (AHB_BASE_ADR + 0x0007FF0)  /* DSP DEBUG instruction register. */

#define DSP_DBG_INOUT_REG_LSW  (AHB_BASE_ADR + 0x0007FF4)  /* DSP DEBUG data register (32 bits). */
#define DSP_DBG_INOUT_REG_MSW  (AHB_BASE_ADR + 0x0007FF6)  /*  ,,   ,,    ,,    ,,     ,,   ,,   */

/* MEMORIES */
#define SHARED_RAM_START       (AHB_BASE_ADR + 0x000000)   /* Shared RAM (16 kbyte) start address.             */
#define SHARED_RAM_END         (AHB_BASE_ADR + 0x003FFF)   /* Shared RAM (16 kbyte) end   address.             */
#define DSP_MC_RAM_START       (AHB_BASE_ADR + 0x008000)   /* Gen2DSP Micro code RAM (16 kbyte) start address. */
#define DSP_MC_RAM_END         (AHB_BASE_ADR + 0x00BFFF)   /* Gen2DSP Micro code RAM (16 kbyte) end   address. */
#define SEQUENCER_RAM_START    (AHB_BASE_ADR + 0x00FA00)   /* DIP Micro code RAM (1.5 kbyte) start address.    */
#define SEQUENCER_RAM_END      (AHB_BASE_ADR + 0x00FFFF)   /* DIP Micro code RAM (1.5 kbyte) end   address.    */


/*====================================================*/
struct __DSP_CTRL_REG
/*====================================================*/
{
    WORD                                           : 2;
    WORD BITFLD_DSP_EN                             : 1;
    WORD BITFLD_DSP_CLK_EN                         : 1;
    WORD BITFLD_DSP_CR16_INT                       : 1;
    WORD                                           : 3;
    WORD BITFLD_DBG_EN                             : 1;
};

#define DSP_EN                             (0x0004)
#define DSP_CLK_EN                         (0x0008)
#define DSP_CR16_INT                       (0x0010)
#define DBG_EN                             (0x0100)


/*====================================================*/
struct __DSP_INT_PRIO1_REG
/*====================================================*/
{
    WORD BITFLD_DSP_INT2_PRIO                      : 3;
    WORD                                           : 1;
    WORD BITFLD_DSP_INT1_PRIO                      : 3;
    WORD                                           : 1;
    WORD BITFLD_DSP_INT0_PRIO                      : 3;
};

#define DSP_INT2_PRIO                      (0x0007)
#define DSP_INT1_PRIO                      (0x0070)
#define DSP_INT0_PRIO                      (0x0700)


/*====================================================*/
struct __DSP_INT_PRIO2_REG
/*====================================================*/
{
    WORD BITFLD_DSP_INT5_PRIO                      : 3;
    WORD                                           : 1;
    WORD BITFLD_DSP_INT4_PRIO                      : 3;
    WORD                                           : 1;
    WORD BITFLD_DSP_INT3_PRIO                      : 3;
};

#define DSP_INT5_PRIO                      (0x0007)
#define DSP_INT4_PRIO                      (0x0070)
#define DSP_INT3_PRIO                      (0x0700)


/*====================================================*/
struct __DSP_IRQ_START_REG
/*====================================================*/
{
    WORD                                           : 4;
    WORD BITFLD_DSP_IRQ_START                      : 12;
};

#define DSP_IRQ_START                      (0xFFF0)


/*====================================================*/
struct __DSP_MAIN_SYNC0_REG
/*====================================================*/
{
    WORD BITFLD_RAMIN0_SYNC                        : 2;
    WORD BITFLD_RAMIN1_SYNC                        : 2;
    WORD BITFLD_RAMIN2_SYNC                        : 2;
    WORD BITFLD_RAMIN3_SYNC                        : 2;
    WORD BITFLD_RAMOUT0_SYNC                       : 2;
    WORD BITFLD_RAMOUT1_SYNC                       : 2;
    WORD BITFLD_RAMOUT2_SYNC                       : 2;
    WORD BITFLD_RAMOUT3_SYNC                       : 2;
};

#define RAMIN0_SYNC                        (0x0003)
#define RAMIN1_SYNC                        (0x000C)
#define RAMIN2_SYNC                        (0x0030)
#define RAMIN3_SYNC                        (0x00C0)
#define RAMOUT0_SYNC                       (0x0300)
#define RAMOUT1_SYNC                       (0x0C00)
#define RAMOUT2_SYNC                       (0x3000)
#define RAMOUT3_SYNC                       (0xC000)


/*====================================================*/
struct __DSP_MAIN_SYNC1_REG
/*====================================================*/
{
    WORD BITFLD_DSP_SYNC0                          : 2;
    WORD BITFLD_DSP_SYNC1                          : 2;
    WORD BITFLD_DSP_SYNC2                          : 2;
};

#define DSP_SYNC0                          (0x0003)
#define DSP_SYNC1                          (0x000C)
#define DSP_SYNC2                          (0x0030)


/*====================================================*/
struct __DSP_MAIN_CTRL_REG
/*====================================================*/
{
    WORD BITFLD_DSP_MAIN_PRESET                    : 8;
    WORD BITFLD_DSP_MAIN_CTRL                      : 2;
};

#define DSP_MAIN_PRESET                    (0x00FF)
#define DSP_MAIN_CTRL                      (0x0300)


/*====================================================*/
struct __DSP_MAIN_CNT_REG
/*====================================================*/
{
    WORD BITFLD_DSP_MAIN_CNT                       : 8;
    WORD BITFLD_DSP_MAIN_REL                       : 8;
};

#define DSP_MAIN_CNT                       (0x00FF)
#define DSP_MAIN_REL                       (0xFF00)


/*====================================================*/
struct __DSP_OVERFLOW_REG
/*====================================================*/
{
    WORD BITFLD_INT_OVERFLOW                       : 6;
    WORD BITFLD_WTF_OVERFLOW                       : 1;
    WORD BITFLD_IRQ_OVERFLOW                       : 1;
    WORD BITFLD_M_INT_OVERFLOW                     : 6;
    WORD BITFLD_M_WTF_OVERFLOW                     : 1;
    WORD BITFLD_M_IRQ_OVERFLOW                     : 1;
};

#define INT_OVERFLOW                       (0x003F)
#define WTF_OVERFLOW                       (0x0040)
#define IRQ_OVERFLOW                       (0x0080)
#define M_INT_OVERFLOW                     (0x3F00)
#define M_WTF_OVERFLOW                     (0x4000)
#define M_IRQ_OVERFLOW                     (0x8000)

/* SetBits */
#define SHIF(a) ((a)&0x0001?0: (a)&0x0002?1: (a)&0x0004?2: (a)&0x0008?3:\
                 (a)&0x0010?4: (a)&0x0020?5: (a)&0x0040?6: (a)&0x0080?7:\
                 (a)&0x0100?8: (a)&0x0200?9: (a)&0x0400?10:(a)&0x0800?11:\
                 (a)&0x1000?12:(a)&0x2000?13:(a)&0x4000?14: 15)

#define SetBits(a,f,d)  ( SetWord( a, (GetWord(a)&((WORD)~f)) | ((d)<<SHIF(f)) ))
