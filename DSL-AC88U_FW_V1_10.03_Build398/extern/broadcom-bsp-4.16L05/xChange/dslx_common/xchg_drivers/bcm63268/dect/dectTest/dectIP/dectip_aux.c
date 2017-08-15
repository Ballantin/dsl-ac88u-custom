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
 * Description: AUX test
 *
 * --------------------------------------------------------------------------------
 * Synchronicity history: 
 * 
 * $Log: dectip_aux.c.rca $
 * 
 *  Revision: 1.2 Fri Feb 13 23:27:09 2009 snijders
 *  Added official SiTel header.
 * 
 * --------------------------------------------------------------------------------
*/ 

/*========================== Include files ==================================*/
#include "sitel_io.h"
#include "sitel_functions.h"
#include "../bcm63268Dect.h"
#include "gen2dsp_aux_0x0000_0x0500.PM"

extern void gprint(const char *fmt, unsigned long arg0, unsigned long arg1, unsigned long arg2);
extern void dumpTestRegs( void );

// ISR for DSP interrupt
void isr_dsp_int (void) {}

// ISR for DIP interrupt
void isr_dip_int (void) {}

bool dectip_aux(void)
{

    BOOL Result = TRUE;
    int p, pp , pp_old;

    p = 0;

   /* Use SWAP based access to keep things simple */
   DECT_CTRL->dect_shm_ctrl &= ~AHB_SWAP_MASK;  
   DECT_CTRL->dect_shm_ctrl |= AHB_SWAP_ACCESS;
   
   /* Enable DSP interrupt in DECT_SHM_IRQ_ENABLE register */
   DECT_CTRL->dect_shm_irq_enable |= 0x7303;

	SetWord(DEBUGGER_REG, 0x0000);

    for(p=0;p<PM_size_aux;p++) {
     SetWord((SHARED_RAM_START+((PM_addr_aux[p])<<1)),PM_val_aux[p]);
     if ((PM_addr_aux[p]+1)<PM_addr_aux[p+1] || p==PM_size_aux-1) {
      SetWord((SHARED_RAM_START+((PM_addr_aux[p])<<1)+2),0x0000);
      SetWord((SHARED_RAM_START+((PM_addr_aux[p])<<1)+4),0x0000);
      SetWord((SHARED_RAM_START+((PM_addr_aux[p])<<1)+6),0x0000);
     }
    }

	SetWord(DSP_PC_START_REG, DSP_PC_START_ADDRESS_aux);
    SetWord(DSP_IRQ_START_REG,DSP_IRQ_START_ADDRESS_aux);
    
gprint("Starting DECTIP AUX testing!\n", 0, 0, 0);

    // Set DSP in stanby by with inetrupt enabled
	SetWord(DEBUGGER_REG, 0x0001);
    SetWord(DSP_INT_PRIO1_REG, 0x7577);            // prio settings
    SetWord(DSP_INT_PRIO2_REG, 0x7777);            // prio settings
	SetWord(DSP_CTRL_REG, 0x0014);                 // dsp enable
	SetWord(DSP_INT_MASK_REG, 0x0000);             // mask irq interupts
    while (GetWord(DSP_INT_REG)==0x0000){};        // wait for interrupt
    if ((GetWord(DSP_INT_REG))!=0x0001) {
     gprint("######## DECTIP_AUX1: False val = 0x%x\n",GetWord(DSP_INT_REG),0,0);
     Result = FALSE;
     if (Result == FALSE)
        dumpTestRegs();
    }

//    if ((GetWord(DSP_IRQ_OUT_COUNTER))!=0x0002) {
//     Result = FALSE;
//    }
	SetWord(DSP_INT_REG, 0xFFFF);                 // clear all interrupts
    SetWord(DSP_INT_PRIO1_REG, 0x7777);            // prio settings
    SetWord(DSP_INT_PRIO2_REG, 0x7777);            // prio settings

    // Test DSP_EXT_INT -> INT0
	SetWord(DEBUGGER_REG, 0x0002);
    SetWord(DSP_INT_PRIO1_REG, 0x7377);            // prio settings
	SetWord(DSP_INT_EXT, 0x0000);                  // generate INT
    DECT_CTRL->dect_shm_irq_trig |= 0x1;           // trigger DSP interrupt
    while (GetWord(DSP_INT_REG)==0x0000){};        // wait for interrupt
    if ((GetWord(DSP_INT_REG))!=0x0001) {
     gprint("######## DECTIP_AUX2: False val = 0x%x\n",GetWord(DSP_INT_REG),0,0);
     Result = FALSE;
     if (Result == FALSE)
        dumpTestRegs();
    }
//    if ((GetWord(DSP_IRQ_OUT_COUNTER))!=4) {
//     Result = FALSE;
//    }
    DECT_CTRL->dect_shm_irq_trig &= ~0x1;         // trigger DSP interrupt
	SetWord(DSP_INT_REG, 0xFFFF);                 // clear all interrupts
    SetWord(DSP_INT_PRIO1_REG, 0x7777);            // prio settings

    // Test DSP_EXT_INT -> INT1
	SetWord(DEBUGGER_REG, 0x0003);
    SetWord(DSP_INT_PRIO1_REG, 0x7737);            // prio settings
	SetWord(DSP_INT_EXT, 0x0000);                  // generate INT
    DECT_CTRL->dect_shm_irq_trig |= 0x1;           // trigger DSP interrupt
    while (GetWord(DSP_INT_REG)==0x0000){};        // wait for interrupt
    if ((GetWord(DSP_INT_REG))!=0x0002) {
     gprint("######## DECTIP_AUX3: False val = 0x%x\n",GetWord(DSP_INT_REG),0,0);
     Result = FALSE;
     if (Result == FALSE)
        dumpTestRegs();
    }
//    if ((GetWord(DSP_IRQ_OUT_COUNTER))!=6) {
//     Result = FALSE;
//    }
    DECT_CTRL->dect_shm_irq_trig &= ~0x1;         // trigger DSP interrupt
	SetWord(DSP_INT_REG, 0xFFFF);                 // clear all interrupts
    SetWord(DSP_INT_PRIO1_REG, 0x7777);            // prio settings
	
    // Test DSP_EXT_INT -> INT2
	SetWord(DEBUGGER_REG, 0x0004);
    SetWord(DSP_INT_PRIO1_REG, 0x7773);            // prio settings
	SetWord(DSP_INT_EXT, 0x0000);                  // generate INT
    DECT_CTRL->dect_shm_irq_trig |= 0x1;           // trigger DSP interrupt
    while (GetWord(DSP_INT_REG)==0x0000){};        // wait for interrupt
    if ((GetWord(DSP_INT_REG))!=0x0003) {
     gprint("######## DECTIP_AUX4: False val = 0x%x\n",GetWord(DSP_INT_REG),0,0);
     Result = FALSE;
     if (Result == FALSE)
        dumpTestRegs();     
    }
//    if ((GetWord(DSP_IRQ_OUT_COUNTER))!=8) {
//     Result = FALSE;
//    }
    DECT_CTRL->dect_shm_irq_trig &= ~0x1;         // trigger DSP interrupt
	SetWord(DSP_INT_REG, 0xFFFF);                 // clear all interrupts
    SetWord(DSP_INT_PRIO1_REG, 0x7777);            // prio settings

    // Test DSP_EXT_INT -> INT3
	SetWord(DEBUGGER_REG, 0x0005);
    SetWord(DSP_INT_PRIO2_REG, 0x7377);            // prio settings
	SetWord(DSP_INT_EXT, 0x0000);                  // generate INT
    DECT_CTRL->dect_shm_irq_trig |= 0x1;           // trigger DSP interrupt
    while (GetWord(DSP_INT_REG)==0x0000){};        // wait for interrupt
    if ((GetWord(DSP_INT_REG))!=0x0004) {
     gprint("######## DECTIP_AUX5: False val = 0x%x\n",GetWord(DSP_INT_REG),0,0);
     Result = FALSE;
     if (Result == FALSE)
        dumpTestRegs();     
    }
//    if ((GetWord(DSP_IRQ_OUT_COUNTER))!=10) {
//     Result = FALSE;
//    }
    DECT_CTRL->dect_shm_irq_trig &= ~0x1;         // trigger DSP interrupt
	SetWord(DSP_INT_REG, 0xFFFF);                 // clear all interrupts
    SetWord(DSP_INT_PRIO2_REG, 0x7777);            // prio settings

    // Test DSP_EXT_INT -> INT4
	SetWord(DEBUGGER_REG, 0x0006);
    SetWord(DSP_INT_PRIO2_REG, 0x7737);            // prio settings
	SetWord(DSP_INT_EXT, 0x0000);                  // generate INT
    DECT_CTRL->dect_shm_irq_trig |= 0x1;           // trigger DSP interrupt
    while (GetWord(DSP_INT_REG)==0x0000){};        // wait for interrupt
    if ((GetWord(DSP_INT_REG))!=0x0005) {
     gprint("######## DECTIP_AUX6: False val = 0x%x\n",GetWord(DSP_INT_REG),0,0);
     Result = FALSE;
     if (Result == FALSE)
        dumpTestRegs();     
    }
//    if ((GetWord(DSP_IRQ_OUT_COUNTER))!=12) {
//     Result = FALSE;
//    }
    DECT_CTRL->dect_shm_irq_trig &= ~0x1;         // trigger DSP interrupt
	SetWord(DSP_INT_REG, 0xFFFF);                 // clear all interrupts
    SetWord(DSP_INT_PRIO2_REG, 0x7777);            // prio settings

    // Test DSP_EXT_INT -> INT5
	SetWord(DEBUGGER_REG, 0x0006);
    SetWord(DSP_INT_PRIO2_REG, 0x7773);            // prio settings
	SetWord(DSP_INT_EXT, 0x0000);                  // generate INT
    DECT_CTRL->dect_shm_irq_trig |= 0x1;           // trigger DSP interrupt
    while (GetWord(DSP_INT_REG)==0x0000){};        // wait for interrupt
    if ((GetWord(DSP_INT_REG))!=0x0006) {
     gprint("######## DECTIP_AUX7: False val = 0x%x\n",GetWord(DSP_INT_REG),0,0);
     Result = FALSE;
     if (Result == FALSE)
        dumpTestRegs();     
    }
//    if ((GetWord(DSP_IRQ_OUT_COUNTER))!=14) {
//     Result = FALSE;
//    }
    DECT_CTRL->dect_shm_irq_trig &= ~0x1;         // trigger DSP interrupt
	SetWord(DSP_INT_REG, 0xFFFF);                 // clear all interrupts
    SetWord(DSP_INT_PRIO2_REG, 0x7777);            // prio settings

gprint("Completed DECTIP AUX testing!\n", 0, 0, 0);

   return Result;
}

    
