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
 * Description: test the PIN
 *
 * --------------------------------------------------------------------------------
 * Synchronicity history: 
 * 
 * $Log: dectip_pin.c.rca $
 * 
 *  Revision: 1.7 Mon Mar  9 14:38:34 2009 laarhove
 *  fixed typo in osr dsp
 * 
 *  Revision: 1.6 Mon Mar  9 14:27:48 2009 laarhove
 *  fixed isr dsp
 * 
 *  Revision: 1.5 Mon Mar  9 13:52:55 2009 barels
 *  changed dip_isr
 * 
 *  Revision: 1.4 Fri Feb 13 23:18:59 2009 snijders
 *  Added official SiTel header.
 * 
 * --------------------------------------------------------------------------------
*/ 

/*========================== Include files ==================================*/

#include "sitel_io.h"
#include "sitel_functions.h"
#include "simul.h"
#include "dip.h"

#define DIP_BANK_ADDR           (0x1000)

/*========================== Global definitions =============================*/

#define SEQ(adr,opc,opr)        SetWord((SEQUENCER_RAM_START + (adr<<1)),(opc)<<8|(opr));

void    program_sequencer_pin(void);


// start of test
int dectip_pin(void)
{ 
 BYTE j;
// start of test
SetWord (DEBUGGER_REG, 0xff);

/* ****************** */
/* *       MWR      * */
/* ****************** */
/* MWR data used for 10.368 MHz */
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xB0, 0x75F5); // data[7] = RWn, data[6:0] = address
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xB2, 0x5581); // Tx data
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xB4, 0x0000); // Rx data will be placed here.
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xB8, 0x0000); // Rx data will be placed here.

/* MWR data used for 4.6 MHz */
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xC0, 0x76F6);
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xC2, 0x5682);
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xC4, 0x0000); // Rx data will be placed here.
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xC8, 0x0000); // Rx data will be placed here.

/* MWR data used for 2.3 MHz */
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xD0, 0x77F7);
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xD2, 0x5783);
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xD4, 0x0000); // Rx data will be placed here.
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xD8, 0x0000); // Rx data will be placed here.

/* MWR data used for 1.152 MHz */
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xE0, 0x78F8);
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xE2, 0x5884);
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xE4, 0x0000); // Rx data will be placed here.
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0xE8, 0x0000); // Rx data will be placed here.

#if 0
/* test values of dip registers */

   if (GetWord(DIP_STACK_REG)       != 0x00) return FALSE;
   if (GetWord(DIP_PC_REG)          != 0x00) return FALSE;
   if (GetWord(DIP_STATUS_REG)      != 0x80) return FALSE;
   if (GetWord(DIP_CTRL_REG)        != 0x80) return FALSE;
   if (GetWord(DIP_STATUS1_REG)     != 0x00) return FALSE;
   if (GetWord(DIP_CTRL1_REG)       != 0x00) return FALSE;
   if (GetWord(DIP_SLOT_NUMBER_REG) != 0x00) return FALSE;
   if (GetWord(DIP_CTRL2_REG)       != 0x00) return FALSE;
#endif 

   SetWord (CLK_PLL_CTRL_REG, 0x000C);

   program_sequencer_pin();


/* start sequencer */
   SetWord(DIP_CTRL_REG, 0x00);

return TRUE;
}


void  program_sequencer_pin(void)
{
    BYTE  dip_cnt;
    BYTE  br_dip_cnt;

    dip_cnt = 1;

//---------------------------------------------------------------
//-- DIP main
//---------------------------------------------------------------
  SEQ(dip_cnt++,  BK_MC,  DIP_BANK_ADDR>>8);
  SEQ(dip_cnt++,  P_EN,  0x00);

  br_dip_cnt = dip_cnt;
      
  SEQ(dip_cnt++,  P_LDL, 0x80);
  SEQ(dip_cnt++,  P_LDL, 0x40);
  SEQ(dip_cnt++,  P_LDL, 0x20);
  SEQ(dip_cnt++,  P_LDL, 0x10);
  SEQ(dip_cnt++,  P_LDL, 0x08);
  SEQ(dip_cnt++,  P_LDL, 0x04);
  SEQ(dip_cnt++,  P_LDL, 0x02);
  SEQ(dip_cnt++,  P_LDL, 0x01);
  SEQ(dip_cnt++,  P_LDL, 0x00);
  SEQ(dip_cnt++,  P_LDH, 0x01);
  SEQ(dip_cnt++,  P_LDH, 0x02);
  SEQ(dip_cnt++,  P_LDH, 0x04);
  SEQ(dip_cnt++,  P_LDH, 0x08);
  SEQ(dip_cnt++,  P_LDH, 0x10);
  SEQ(dip_cnt++,  P_LDH, 0x20);
  SEQ(dip_cnt++,  P_LDH, 0x40);
  SEQ(dip_cnt++,  P_LDH, 0x80);
  SEQ(dip_cnt++,  P_LDL, 0xFF);
  SEQ(dip_cnt++,  RFEN,  0x00);
  SEQ(dip_cnt++,  WT,    0x02);
  SEQ(dip_cnt++,  RFDIS, 0x00);
  SEQ(dip_cnt++,  P_LDH, 0xFF);


  SEQ(dip_cnt++, WT    , 0x01);

  SEQ(dip_cnt++, BR    , br_dip_cnt);

}

/* ********************* */
/* End sequencer program */
/* ********************* */

