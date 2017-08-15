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
 * Description: TDO-RDI test
 *
 * -----------------------------------------------------------------------------




/*========================== Include files ==================================*/
#include "sitel_io.h"
#include "sitel_functions.h"
#include "simul.h"
#define IO_ADDRESS(x)           (((x) & 0x00ffffff) + 0xfc000000)
#include "63148_map_part.h"
#include "dip.h"

//NOT AVAILABLE IN SC14450.H 
#define DSP_MAIN_REL (0xFF00)
#define DSP_MAIN_CNT (0x00FF)

/*========================== Global definitions =============================*/
#define SEQ(adr,opc,opr)        SetWord((SEQUENCER_RAM_START + (adr<<1)),(opc)<<8|(opr));    

#define ADPCM_MAIN_BANK_ADDR    (0x1000)
#define ADPCM0_BANK             (0x100)
#define ADPCM2_BANK             (0x200)

#define BMC_BANK_ADDR           (0x1000)
#define BMC_CTRL_TR_ADDR        (0x0000)   //BMC CONTROL DATA WHILE TRANSMITTING -> SHARED_RAM + 0x1000
#define BMC_CTRL_RD_ADDR        (0x0010)   //BMC_CONTROL DATA WHILE RECEIVING    -> SHARED_RAM + 0x1010

#define AFIELD_TR_ADDR          (0x0020)   //AFIELD TRANSMIT AT POSITION SHARED_RAM + BMC_BANK_ADDR + AFIELD_TR_ADDR -> SHARED_RAM+0x1020
#define AFIELD_RD_ADDR          (0x0028)   //AFIELD RECEIVE AT POSITION SHARED_RAM +  BMC_BANK_ADDR + AFIELD_RD_ADDR -> SHARED_RAM+0x1028

#define TRANSMIT_BUF_1          (0x0000)   //BUFFER1 TRANSMIT AT POSITION SHARED_RAM + ADPCM0_ADDR + TRANSMIT_BUF_1 -> SHARED_RAM+0x1100-0x1128
#define RECEIVE_BUF_1           (0x0060)   //BUFFER1 RECEIVE AT POSITION SHARED_RAM + ADPCM0_ADDR + RECEIVE_BUF_1 -> SHARED_RAM+0x1160-0x1188
#define TRANSMIT_BUF_2          (0x0010)   //BUFFER2 TRANSMIT  AT POSITION SHARED_RAM + ADPCM2_ADDR + TRANSMIT_BUF_2 -> SHARED_RAM+0x1210-0x1260
#define RECEIVE_BUF_2           (0x0070)   //BUFFER2 RECEIVE  AT POSITION SHARED_RAM + ADPCM2_ADDR + RECEIVE_BUF_2 -> SHARED_RAM+0x1270-0x12C0

#define DSP_TR_BUF_1          (0x1300)   //BUFFER1 TRANSMIT AT POSITION SHARED_RAM + DSP_TR_BUF_1 -> SHARED_RAM+0x1300
#define DSP_RD_BUF_1          (0x1360)   //BUFFER1 RECEIVE AT POSITION SHARED_RAM + DSP_RD_BUF_1 -> SHARED_RAM+0x1360
#define DSP_TR_BUF_2          (0x1400)   //BUFFER2 TRANSMIT  AT POSITION SHARED_RAM + DSP_TR_BUF_2 -> SHARED_RAM+0x1400
#define DSP_RD_BUF_2          (0x1460)   //BUFFER2 RECEIVE  AT POSITION SHARED_RAM + DSP_RD_BUF_2 -> SHARED_RAM+0x1460

#define DEBUG_AREA            (0x1500)  
/*===========================================================================*/
/*  Local data definitions                                                   */
/*===========================================================================*/  

BOOL check_data(int rx_addr, int data)
{
  gprint("At address %x expected data = %x received data %x\n", rx_addr, data, GetByte(rx_addr));
  if (GetByte(rx_addr) != data ) return FALSE;
  else return TRUE;
}

// Connection1: based on 8khz and ADPCM0.  Slot0 and slot12
// Connection2: based on wideband audio (16 khz codec) and ADPCM2. Slot4,4 and Slot16,17
void  program_seq_tdo_rdi(void)
{
    BYTE  dip_cnt;
    BYTE  br_dip_cnt;

    br_dip_cnt = 1;
    dip_cnt = 1;

/* INIT ADPCM*/  

    SEQ(dip_cnt++,    RFEN,  0x01); 
    SEQ(dip_cnt++,    BK_MC,  BMC_BANK_ADDR>>8); 
    SEQ(dip_cnt++,    BK_MA,  ADPCM_MAIN_BANK_ADDR>>8);   
    SEQ(dip_cnt++,    BK_A,   ADPCM0_BANK>>3);
    SEQ(dip_cnt++,    BK_A2,  ADPCM2_BANK>>3);
    SEQ(dip_cnt++,    A_LDW,  TRANSMIT_BUF_1);            
    SEQ(dip_cnt++,    A_LDR,  RECEIVE_BUF_1);
    SEQ(dip_cnt++,    A_LDW2, TRANSMIT_BUF_2);            
    SEQ(dip_cnt++,    A_LDR2, RECEIVE_BUF_2); 
    SEQ(dip_cnt++,    A_NORM, 0x00); 
    SEQ(dip_cnt++,    A_TX,   0);  
    SEQ(dip_cnt++,    A_RX,   0);  

    /* TRANSMIT */

/* TRANSMIT SLOT, SLOT0 */
/*
    GUARD SPACE,        56  BITS
    PRELONGED SFIELD,   0   BITS
    SFIELD,             32  BITS
    AFIELD              48  BITS
    AFIELD_CRC          16  BITS
    BFIELD              320 BITS
    BFIELD_CRC          0   BITS
*/                                                                     
/* GUARD SPACE*/
    br_dip_cnt = dip_cnt;

    SEQ(dip_cnt++,   WNT, 1); //SLOT0

    SEQ(dip_cnt++,   A_TX,   0); 
    SEQ(dip_cnt++,   B_RC,  BMC_CTRL_TR_ADDR); //READ BMC CONTROL INFORMATION FOR TRANSMIT
    SEQ(dip_cnt++,   WT,    9);    
    SEQ(dip_cnt++,   B_RST, 0); 
    SEQ(dip_cnt++,   WT,    20-13);

/* S-FIELD */
    SEQ(dip_cnt++,   B_ST2, 0);
    SEQ(dip_cnt++,   WT,    31);

/* A-FIELD + AFIELD_CRC */
    SEQ(dip_cnt++,   B_AT2, AFIELD_TR_ADDR);
    SEQ(dip_cnt++,   WT,    63);

/* B-FIELD */
    SEQ(dip_cnt++,   B_BT2,  0xFC); // START TRANSMIT FROM ADPCM BUFFER 
    SEQ(dip_cnt++,   WT,     200);              // 200 CYCLES
    SEQ(dip_cnt++,   WT,     119 + 7);          // 119 CYCLES + 7 CYCLES TO ENSURE ALL DATA IS TRANSMITTED
    SEQ(dip_cnt++,   B_RST,  0);
    SEQ(dip_cnt++,   WNT, 1); 



// DUMMY SLOTS
    SEQ(dip_cnt++,   WNT, 1); //SLOT2
    SEQ(dip_cnt++,   WNT, 1); //SLOT3
    SEQ(dip_cnt++,   WNT, 1); //SLOT4    


/* TRANSMIT SLOT, SLOT5 and 6 ADPCM2*/
/*
    GUARD SPACE,        216  BITS
    PRELONGED SFIELD,   0   BITS
    SFIELD,             32  BITS
    AFIELD              48  BITS
    AFIELD_CRC          16  BITS
    BFIELD              640 BITS
    BFIELD_CRC          8   BITS
*/                                                                     
/* GUARD SPACE*/
    SEQ(dip_cnt++,   A_TX,   2);  
    SEQ(dip_cnt++,   B_RC,  BMC_CTRL_TR_ADDR); //READ BMC CONTROL INFORMATION FOR TRANSMIT
    SEQ(dip_cnt++,   WT,    9);    
    SEQ(dip_cnt++,   B_RST, 0); 
    SEQ(dip_cnt++,   WT,    20-13);

/* S-FIELD */
    SEQ(dip_cnt++,   B_ST2, 0);
    SEQ(dip_cnt++,   WT,    31);

/* A-FIELD + AFIELD_CRC */
    SEQ(dip_cnt++,   B_AT2, AFIELD_TR_ADDR);
    SEQ(dip_cnt++,   WT,    63);

/* B-FIELD */
    SEQ(dip_cnt++,   B_BTLU, 0xFC); // START TRANSMIT FROM ADPCM BUFFER 
    SEQ(dip_cnt++,   WT,     145);           // 145 CYCLES
    SEQ(dip_cnt++,   WT,     0xFF);          // 255 CYCLES
    SEQ(dip_cnt++,   WT,     0xFF);          // 255 CYCLES,  TOTAL 145+255+255=655 CYCLES --> B+X+Z=640+4+4+(+7 DELAY)=655
    SEQ(dip_cnt++,   B_RST,  0);
//    SEQ(dip_cnt++,   U_VINT, 1); 
    SEQ(dip_cnt++,   WNT, 1);    

// DUMMY SLOTS
    SEQ(dip_cnt++,   WNT, 1); //SLOT7
    SEQ(dip_cnt++,   WNT, 1); //SLOT8
    SEQ(dip_cnt++,   WNT, 1); //SLOT9
    SEQ(dip_cnt++,   WNT, 1); //SLOT10
    SEQ(dip_cnt++,   WNT, 1); //SLOT11


    
    /* RECEIVE */
    SEQ(dip_cnt++,   WNT, 1); //SLOT12


     /* RECEIVE SLOT, SLOT13 */
    /*
    GUARD SPACE,        56  BITS
    PRELONGED SFIELD,   0   BITS
    SFIELD,             32  BITS
    AFIELD              48  BITS
    AFIELD_CRC          16  BITS
    BFIELD              320 BITS
    BFIELD_CRC          0   BITS */
    /* GUARD SPACE*/
    SEQ(dip_cnt++,   A_RX,   0);  
    SEQ(dip_cnt++,   B_RC,  BMC_CTRL_RD_ADDR); //READ BMC CONTROL INFORMATION FOR RECEIVE
    SEQ(dip_cnt++,   WT,    19);    

/* S-FIELD */
    SEQ(dip_cnt++,   B_SR, 0);
    SEQ(dip_cnt++,   WT,    31+ 16);   //+16 NECESSARY FOR ALLOWING PHASE OFFSET

/* A-FIELD + AFIELD_CRC */
    SEQ(dip_cnt++,   B_AR2, AFIELD_RD_ADDR);
    SEQ(dip_cnt++,   WT,    63);

/* B-FIELD */
    SEQ(dip_cnt++,   B_BR2,  0xFC); // START RECEIVE B-FIELD AND STORE IN ADPCM BUFFER 
    SEQ(dip_cnt++,   WT,     200);              // 200 CYCLES
    SEQ(dip_cnt++,   WT,     119 + 5);          // 119 CYCLES + 5 CYCLES TO ENSURE ALL DATA IS TRANSMITTED
    SEQ(dip_cnt++,   B_RST,  0);
    SEQ(dip_cnt++,   WNT, 1); 
    
    // DUMMY SLOTS
    SEQ(dip_cnt++,   WNT, 1); //SLOT14
    SEQ(dip_cnt++,   WNT, 1); //SLOT15
    SEQ(dip_cnt++,   WNT, 1); //SLOT16   

    /*RECEIVE SLOT, SLOT17 and 18 */
    /*
    GUARD SPACE,        216  BITS
    PRELONGED SFIELD,   0   BITS
    SFIELD,             32  BITS
    AFIELD              48  BITS
    AFIELD_CRC          16  BITS
    BFIELD              640 BITS
    BFIELD_CRC          8   BITS
    */
                                                                     
/* GUARD SPACE*/
    SEQ(dip_cnt++,   A_RX,   2);  
    SEQ(dip_cnt++,   B_RC,  BMC_CTRL_RD_ADDR); //READ BMC CONTROL INFORMATION FOR RECEIVE
    SEQ(dip_cnt++,   WT,    19);    

/* S-FIELD */
    SEQ(dip_cnt++,   B_SR, 0);
    SEQ(dip_cnt++,   WT,    31+ 16);   //+16 NECESSARY FOR ALLOWING PHASE OFFSET

/* A-FIELD + AFIELD_CRC */
    SEQ(dip_cnt++,   B_AR2, AFIELD_RD_ADDR);
    SEQ(dip_cnt++,   WT,    63);

/* B-FIELD */
    SEQ(dip_cnt++,   B_BRLU, 0xFC); // START RECEIVE B-FIELD AND STORE IN ADPCM BUFFER 
    SEQ(dip_cnt++,   WT,     138);           // 138 CYCLES
    SEQ(dip_cnt++,   WT,     0xFF);          // 255 CYCLES
    SEQ(dip_cnt++,   WT,     0xFF);          // 255 CYCLES,  TOTAL 648 CYCLES, B+X+Z=640+4+4=648
    SEQ(dip_cnt++,   B_RST,  0);
//    SEQ(dip_cnt++,   U_VINT, 2); 
    SEQ(dip_cnt++,   WNT, 1); 

// DUMMY SLOTS 
    SEQ(dip_cnt++,   WNT, 1); //SLOT19
    SEQ(dip_cnt++,   WNT, 1); //SLOT20
    SEQ(dip_cnt++,   WNT, 1); //SLOT21

    SEQ(dip_cnt++,   WNT, 1); //SLOT22
    SEQ(dip_cnt++,   WT,     200);           
//    SEQ(dip_cnt++,   U_VINT, 3); 
    SEQ(dip_cnt++,   WNT, 1); //SLOT23

    SEQ(dip_cnt++,   BR , br_dip_cnt); //BRANCH TO SLOT0
}
   
void dectip_tdo_rdi(void)
{ 
    WORD main_cnt_value= 0x0000;
    WORD j=1;
    WORD m=1;
    int sample_count=0;
    BYTE temp=0x00;
   
    BOOL active_pulse = TRUE;
    BOOL passed = FALSE; 
//    SetWord(TEST_REG, DECTIP_TDO_RDI);

   dectShimSetAHBSwapMode( AHB_SWAP_ACCESS );
   SetWord(CLK_PLL_CTRL_REG,     0x000C);

   gprint("### DECT DEBUGGER 0x0001\n", 0,0,0 );

// Initialize ADPCM BUFFERS, tr_ and receive buffer
    for(j = 0; j<40; j++)
    {
        SetByte(( SHARED_RAM_START + ADPCM_MAIN_BANK_ADDR + ADPCM0_BANK + TRANSMIT_BUF_1 + j), 0x00);
        SetByte(( SHARED_RAM_START + ADPCM_MAIN_BANK_ADDR + ADPCM0_BANK+ RECEIVE_BUF_1  + j),  0x11);
        SetByte(( SHARED_RAM_START + DSP_TR_BUF_1 + j), j);
        SetByte(( SHARED_RAM_START + DSP_RD_BUF_1 + j), 0xFF);
    }

    for(j = 0; j<80; j++)
    {
        SetByte( (SHARED_RAM_START + ADPCM_MAIN_BANK_ADDR + ADPCM2_BANK+ TRANSMIT_BUF_2 + j),  0x22);
        SetByte( (SHARED_RAM_START + ADPCM_MAIN_BANK_ADDR + ADPCM2_BANK+ RECEIVE_BUF_2  + j),  0x33); 
        SetByte( (SHARED_RAM_START + DSP_TR_BUF_2 + j),  j+100);
        SetByte( (SHARED_RAM_START + DSP_RD_BUF_2 + j),  0xFF); 
    }

// INIT A-FIELD
    for(j = 0; j<8; j++)
    {
      SetByte(SHARED_RAM_START + BMC_BANK_ADDR + AFIELD_TR_ADDR + j,   j+0x10);
      SetByte(SHARED_RAM_START + BMC_BANK_ADDR + AFIELD_RD_ADDR + j,   0x55);
    } 

    
// INIT BMC_CONTROLLER
/*
--------------------------------------------------------------------------------------------
RC[0]: S_err[3] | S_err[2] | S_err[1] | S_err[0] | inv_RDI  | inv_TDO  | SenseA  | PP/FPn  |
RC[1]: Slide[3] | Slide[2] | Slide[1] | Slide[0] | Mask[3]  | Mask[2]  | Mask[1] | Mask[0] |
RC[2]: CM5      | CM6      | Vol[5]   | Vol[4]   | Vol[3]   | Vol[2]   | Vol[1]  | Vol[0]  |
RC[3]: DON      | TM_ANA   | DAC[5]   | DAC[4]   | DAC[3]   | DAC[2]   | DAC[1]  | DAC[0]  |
RC[4]: PLL[2]   | DPLL[1]  | DPLL[0]  | ADP      | WIN[3]   | WIN[2]   | WIN[1]  | WIN[0]  |
RC[5]: ENC_OFF  | SenseS   | Ron      | Mutelevel| SCoff    | DOFr     | M[1]    | M[0]    |
RC[6]: MFR[3]   | MFR[2]   | MFR[1]   | MFR[0]   | FR_nr[3] | FR_nr[2] | FRnr[1] | FRnr[0] |
-------------------------------------------------------------------------------------------- */

    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_TR_ADDR + 0, 0x20);    //FP MODE, 2 errors accepted in Sfield
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_TR_ADDR + 1, 0xFF);
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_TR_ADDR + 2, 0x3F);
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_TR_ADDR + 3, 0xA0);
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_TR_ADDR + 4, 0x0F);    //WINDOW SFIELD
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_TR_ADDR + 5, 0x89);    //NO SCRAMBLING, NO ENCRYPTION, TDO Gaussian on
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_TR_ADDR + 6, 0x00);          
   
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_RD_ADDR + 0, 0x21);    //PP MODE, 2 errors accepted in Sfield
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_RD_ADDR + 1, 0xFF);
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_RD_ADDR + 2, 0x3F);
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_RD_ADDR + 3, 0xA0);
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_RD_ADDR + 4, 0x0F);    //WINDOW SFIELD
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_RD_ADDR + 5, 0x88);    //NO SCRAMBLING, NO ENCRYPTION
    SetByte(SHARED_RAM_START + BMC_BANK_ADDR + BMC_CTRL_RD_ADDR + 6, 0x00);  


//DIP SETTINGS
    SetWord(DIP_CTRL_REG,     0x0080);   
    SetWord(DIP_MOD_SEL_REG,  0x0044); // ADPCM0 MODULO 40, ADPCM2 MODULO 80 
    SetWord(BMC_CTRL_REG,     0x0000);   
    
   gprint("### DECT DEBUGGER 0x0002\n", 0,0,0 );
// PROGRAM SEQUENCER PROGRAM
    program_seq_tdo_rdi();
  
// START 8KHZ CHECKER
//    SetBits(DSP_MAIN_CNT_REG, DSP_MAIN_REL, 0x8F); //  0-0x8F TICKS = 125 uSEC --> 8 KHZ
    SetWord(DSP_MAIN_CTRL_REG,     0x028F); 
    SetWord(CLK_CODEC_DIV_REG, 0x006C);
//    SetBits(DSP_MAIN_SYNC0_REG, RAMIN0_SYNC,  0); //4 KHz
//    SetBits(DSP_MAIN_SYNC0_REG, RAMOUT0_SYNC, 0); //4 KHz
//    SetBits(DSP_MAIN_SYNC0_REG, RAMIN2_SYNC,  1); //8 KHz
//    SetBits(DSP_MAIN_SYNC0_REG, RAMOUT2_SYNC, 1); //8 KHz
    SetWord(DSP_MAIN_SYNC0_REG,     0xDCDC); 

// INIT RAM IO REGISTERS
    SetByte(DSP_RAM_OUT0_REG, 0x00);  //Use 0x00 as reset value as first byte will be sent immediately
    SetByte(DSP_RAM_IN0_REG,  0x55);
    SetByte(DSP_RAM_OUT2_REG, 0x64);  //Use 0x64 as reset value as first byte will be sent immediately
    SetByte(DSP_RAM_IN2_REG,  0x55); 

// START SEQUENCER
    SetWord(DIP_CTRL_REG, 0x00);

// WAIT TILL FINISHED
    
    while(sample_count != 240)
    {
        main_cnt_value = GetWord(DSP_MAIN_CNT_REG) &0xFF;
        //WAIT FOR 8KHZ TICK
        while((( main_cnt_value == 0x1A)  ||
               ( main_cnt_value == 0x1B)  ||
               ( main_cnt_value == 0x1C)  ||
               ( main_cnt_value == 0x1D)  ||
               ( main_cnt_value == 0x1E)) &&
                 passed == TRUE)
        {
             main_cnt_value = GetWord(DSP_MAIN_CNT_REG) &0xFF;
        }
        passed = FALSE;

        while(( main_cnt_value <0x1A || main_cnt_value >0x1E) && passed == FALSE)
        {
             main_cnt_value = GetWord(DSP_MAIN_CNT_REG) &0xFF;
        }
        passed = TRUE;


//COPY ADPCM2 RAMIO REGISTERS ON 4 KHZ BASE
      if(active_pulse)
        {
            temp = GetByte(SHARED_RAM_START+DSP_TR_BUF_1+m);
            SetByte(DSP_RAM_OUT0_REG, temp);
//            SetWord((SHARED_RAM_START+DEBUG_AREA+m), m);
//            SetWord((SHARED_RAM_START+DEBUG_AREA+m+0x40), temp);
//            SetWord((SHARED_RAM_START+DEBUG_AREA+m+0x80), sample_count);
            SetByte((SHARED_RAM_START+DSP_RD_BUF_1+m), (GetWord(DSP_RAM_IN0_REG)));
            active_pulse = FALSE;
            m++;
        }
      else
        {
            active_pulse = TRUE;
        }

        if (m>=40)
        {
            m=0;
        }


//COPY ADPCM2 RAMIO REGISTERS ON 8 KHZ BASE
        temp = GetByte(SHARED_RAM_START+DSP_TR_BUF_2+j);
        SetByte(DSP_RAM_OUT2_REG, temp);
        SetByte((SHARED_RAM_START+DSP_RD_BUF_2+j), (GetWord(DSP_RAM_IN2_REG)));
        j++;
        if (j>=80)
        {
            j=0;
        }
        sample_count++;
    }

// STOP DIP
//    SetWord(DIP_CTRL_REG,     0x0080); 
// STOP DSP
//    SetWord(DSP_MAIN_CTRL_REG,     0x0000); 
    SetWord(DSP_MAIN_SYNC0_REG,    0xFFFF); 
   gprint("### DECT DEBUGGER 0x0003\n", 0,0,0 );

/* test CONNECTION1 DATA, ADPCM0   */

    for(j = 0; j<22; j++)
    {
      if (check_data((SHARED_RAM_START + DSP_RD_BUF_1 + j), j+18) != TRUE) return FALSE;
    }
          gprint("######## ADPCM1 first part passed \n");
    for(j = 22; j<40; j++)
    {
      if (check_data((SHARED_RAM_START + DSP_RD_BUF_1 + j), j-22) != TRUE) return FALSE;
    }
          gprint("######## ADPCM1 second part passed \n");

/* test CONNECTION2 DATA, ADPCM2   */

    for(j = 0; j<42; j++)
    {
      if (check_data((SHARED_RAM_START + DSP_RD_BUF_2 + j), j+138) != TRUE) return FALSE;
    }
          gprint("######## ADPCM2 first part passed \n");

   for(j = 42; j<80; j++)
    {
      if (check_data((SHARED_RAM_START + DSP_RD_BUF_2 + j), j+58) != TRUE) return FALSE;
    }
          gprint("######## ADPCM2 second part passed \n");

//    return TRUE;
}

