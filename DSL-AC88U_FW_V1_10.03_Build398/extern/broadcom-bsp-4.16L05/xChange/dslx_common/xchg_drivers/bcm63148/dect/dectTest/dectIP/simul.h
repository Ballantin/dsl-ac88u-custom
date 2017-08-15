#ifndef SIMUL_H_INCLUDED
#define SIMUL_H_INCLUDED

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
 * Description: test number definitions (used for TEST_REG)
 *
 * ----------------------------------------------------------------------------- 
 * Synchronicity history: 
 * 
 * $Log: simul.h.rca $
 * 
 *  Revision: 1.23 Fri Feb 13 21:07:31 2009 snijders
 *  Added "DECTIP_AHB_ERROR" test number.
 * 
 *  Revision: 1.22 Fri Feb 13 14:58:10 2009 laarhove
 *  update added ahb word2..16 write cycles
 * 
 *  Revision: 1.21 Wed Feb 11 13:57:24 2009 barels
 *  added SetloopBack4slots
 * 
 *  Revision: 1.20 Wed Feb 11 13:51:55 2009 barels
 *  added mem_loop
 * 
 *  Revision: 1.19 Mon Feb  9 17:21:38 2009 barels
 *  seq00 redefines changed
 * 
 *  Revision: 1.18 Mon Feb  9 17:17:19 2009 barels
 *  added tim seq tests
 * 
 *  Revision: 1.17 Fri Feb  6 19:42:48 2009 snijders
 *  Added 'DECTIP_ZCROSS'.
 * 
 *  Revision: 1.16 Thu Feb  5 15:47:38 2009 elfrink
 *  added dectip_rdi_dsc
 * 
 *  Revision: 1.15 Thu Feb  5 13:42:56 2009 elfrink
 *  changed testnumbers and added config constants
 * 
 *  Revision: 1.14 Wed Feb  4 15:57:37 2009 barels
 *  added tdo_rdi_short
 * 
 *  Revision: 1.13 Wed Feb  4 14:15:28 2009 elfrink
 *  change numbers and added tests
 * 
 *  Revision: 1.12 Tue Feb  3 15:40:12 2009 snijders
 *  Updated some comment.
 * 
 *  Revision: 1.11 Tue Feb  3 15:38:58 2009 snijders
 *  Major cleanup for the sc24200.
 * 
 *  Revision: 1.10 Mon Jan 19 12:25:32 2009 barels
 *  added tdo_rdi
 * 
 *  Revision: 1.9 Mon Dec 22 14:53:31 2008 snijders
 *  Small update.
 * 
 *  Revision: 1.8 Mon Dec 22 14:50:59 2008 snijders
 *  Small update.
 * 
 *  Revision: 1.7 Mon Dec 22 14:49:26 2008 snijders
 *  Updated some comment. File needs to be updated further.
 * 
 *  Revision: 1.6 Wed Dec 17 15:45:22 2008 laarhove
 *  Updated spu_reg_test.
 * 
 *  Revision: 1.5 Tue Dec 16 16:37:30 2008 snijders
 *  Added official SiTel header. 
 *  Changed (added GetDWord and SetDWord):
 *    //HS/16-dec-2008: #define GetWord(a) read_data_dword(i, o, a)
 *    #define GetDWord(a) read_data_dword(i, o, a)
 *  
 *    //HS/16-dec-2008: #define SetWord(a, d) rtl_write_dword(i, o, a, d)
 *    #define SetDWord(a, d) rtl_write_dword(i, o, a, d) 
 * 
 *  Revision: 1.4 Thu Nov 27 15:33:41 2008 witlox
 *  Added word and dword support
 * 
 *  Revision: 1.3 Wed Nov 26 08:43:32 2008 witlox
 *  Removed check_apb_reg definition
 * 
 *  Revision: 1.2 Tue Jun 24 11:53:00 2008 witlox
 *  All addresses relative to DECT_IP_XXX_BASE
 * 
 *  Revision: 1.1 Tue Jun  3 13:08:50 2008 witlox
 *  dectip header files, for SW based environment
 * 
 * ----------------------------------------------------------------------------- 
*/ 


/*========================== Include files ==================================*/


/*========================== Local macro definitions ========================*/


/* Other definitions for GetWord and SetWord */
#if 0
#define GetDWord(a) *((unsigned int*)a)
#define GetWord(a) *((unsigned short*)a)
#define GetByte(a) *((unsigned char*)a)


#define SetWord32(a, d) *((unsigned int*)a)   = d
#define SetWord16(a, d) *((unsigned short*)a) = d
#define SetWord8(a, d)  *((unsigned char*)a)  = d
#define SetWord4(a, d)  
#define SetWord2(a, d)
#define SetDWord(a, d)  *((unsigned int*)a)   = d
#define SetWord(a, d)   *((unsigned short*)a) = d
#define SetByte(a, d)   *((unsigned char*)a)  = d
#endif
#if 1
extern unsigned int GetDWord(unsigned long a);

extern unsigned short GetWord(unsigned long a);

extern unsigned char GetByte(unsigned long a);

extern void SetDWord(unsigned long a, unsigned int d);

extern void SetWord(unsigned long a, unsigned short d);

extern void SetByte(unsigned long a, unsigned char d);

#endif

/* Test number definitions (used for TEST_REG) */

#define DECTIP_APB              1
#define DECTIP_AHB_BYTE         2
#define DECTIP_AHB_HALFWORD     3
#define DECTIP_AHB_WORD         4
#define DECTIP_INT              5
#define DECTIP_DEBUGGER         6
#define DECTIP_DSP_TEST         7
#define DECTIP_MWR              8
#define DECTIP_PIN              9
#define DECTIP_GAUSS           10
#define DECTIP_DECT_LOOP       11
#define DECTIP_CATIQ_LOOP      12
#define DECTIP_AUX             13
#define DECTIP_TDO_RDI         14
#define DECTIP_TDO_RDI_SHORT   15
#define DECTIP_DSC             16
#define DECTIP_TDO_DSC         17
#define DECTIP_RDI_DSC         18
#define DECTIP_ZCROSS          19
#define DECTIP_TIM_SEQ_00      20 
#define DECTIP_TIM_SEQ_01      21
#define DECTIP_TIM_SEQ_02      22 
#define DECTIP_MEM_LOOP        23
#define DECTIP_AHB_ERROR       24


/*========================== Global variabeles ==============================*/
/* Variables used for Config register */
#define SetLoopBack        0xFF00
#define SetLoopBackShort   0xFF01
#define SetLoopBack4slots  0xFF02
#define SetCheckMode       0xFE00  /* bits [7:0] can be used for additional control */
#define SetTdoMode         0xFD00  /* bits [7:0] can be used for additional control */
#define SetRdiMode         0xFC00  /* bits [7:0] can be used for additional control */

/*========================== Global function prototypes =====================*/

#endif /* SIMUL_H_INCLUDED */
