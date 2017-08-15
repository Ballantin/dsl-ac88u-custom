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
 * Description: GAUSS test vector
 *
 * --------------------------------------------------------------------------------
 * Synchronicity history: 
 * 
 * $Log: $
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

static void    program_sequencer(void);
#if 0
volatile enum
{
        busy,
        ready
}dip_test;
#endif

// start of test
int dectip_gauss_010101(void)
{ 
 BYTE j;
// start of test
SetWord (DEBUGGER_REG, 0xff);
SetWord (CLK_PLL_CTRL_REG, 0x000C);

/* BMC RC data */
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0x80, 0xFF21); 
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0x82, 0x0015);
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0x84, 0x0120);

/* BMC S-field */
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0x70, 0x0055);
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0x72, 0x5555);
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0x74, 0x5555);
SetWord (SHARED_RAM_START + DIP_BANK_ADDR + 0x76, 0x5555);

   program_sequencer();

   //dip_test = busy;

/* start sequencer */
   SetWord(DIP_CTRL_REG, 0x00);

   while(GetWord(DIP_SLOT_NUMBER_REG) != 0x01)
     {}

return TRUE;
}


static void  program_sequencer(void)
{
    BYTE  dip_cnt;
    BYTE  br_dip_cnt;

    dip_cnt = 1;

//---------------------------------------------------------------
//-- DIP main
//---------------------------------------------------------------
  SEQ(dip_cnt++,  RFEN,  0x01); 
  SEQ(dip_cnt++,  BK_MC,  DIP_BANK_ADDR>>8);
  SEQ(dip_cnt++,  B_RC,  0x80);
  SEQ(dip_cnt++,  WT,    0x0A);
  SEQ(dip_cnt++,  B_ST,  0x70);
  SEQ(dip_cnt++,  WT,    0x07);
   br_dip_cnt = dip_cnt;
  SEQ(dip_cnt++,  B_ST,  0x70);
  SEQ(dip_cnt++,  WT,    0x04);
  SEQ(dip_cnt++,  MEN1,  0x01);
  SEQ(dip_cnt++,  MEN1N, 0x01);
  SEQ(dip_cnt++,  BR, br_dip_cnt);

}

/* ********************* */
/* End sequencer program */
/* ********************* */

// ISR for DIP interrupt
/*void isr_dip_int (int i, int *o) {
}*/

// ISR for DSP interrupt
/*void isr_dsp_int (int i, int *o) {
}*/

