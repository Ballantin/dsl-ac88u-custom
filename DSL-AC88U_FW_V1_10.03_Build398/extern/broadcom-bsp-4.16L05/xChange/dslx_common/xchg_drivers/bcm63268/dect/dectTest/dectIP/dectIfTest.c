/* includes */
//#include "datasheet.h"
#include "sitel_io.h"
#include "simul.h"
#include "bcm63268Dect.h"
#include <linux/compiler.h>
 

/* Function prototypes */
void dectIfTest(  XDRV_UINT32 test );
void ahbTest( void );
void apbTest( void );
void shimTest( void );
void dumpTestRegs( void );
inline void dectIPReset( void );
inline void dectShimSetAHBSwapMode(XDRV_UINT32 mode);
void SetGpioTestMode(void);

extern void gprint(const char *fmt, unsigned long arg0, unsigned long arg1, unsigned long arg2);

extern int dectip_ahb_byte(void);
extern int dectip_ahb_halfword(void);
extern int dectip_ahb_word(void);
extern int dectip_apb(void);
extern int dectip_dsp_test(void);
extern int dectip_debugger(void);


#define rdtscl(dest) __asm__ __volatile__("mfc0 %0,$9; nop" : "=r" (dest)) 


void dectIfTest( XDRV_UINT32 test )
{
   /* Start fresh */
   dectIPReset();
   
#ifdef AHB_AUTO_SWAP   
   dectShimSetAHBSwapMode( AHB_SWAP_ACCESS );
   gprint("\n#################### DECT - Using AHB_SWAP_ACCESS mode  ####################\n", 0,0,0 );   
#endif

   switch( test )
   {
      case 0:

#ifndef AHB_AUTO_SWAP
   dectShimSetAHBSwapMode( AHB_SWAP_16_BIT );
#endif   

      //apbTest();
      ahbTest();
      //shmTest();
      dectIPReset();
      break;
      
      
      case 1:
   
#ifndef AHB_AUTO_SWAP
      dectShimSetAHBSwapMode( AHB_SWAP_8_BIT );
#endif
      gprint("\n#################### DECT ahb byte test start  ####################\n", 0,0,0 );

      if (dectip_ahb_byte())
      {
         gprint("\n#################### DECT ahb byte test pass  ####################\n\n", 0,0,0 );
      }	   
      else
      {
         gprint("\n#################### DECT ahb byte test fail  ####################\n\n", 0,0,0 );
         dumpTestRegs();
      }

      dectIPReset();
      break;     


      case 2:

#ifndef AHB_AUTO_SWAP
      dectShimSetAHBSwapMode( AHB_SWAP_16_BIT );
#endif
      gprint("\n#################### DECT ahb halfword test start ####################\n", 0,0,0 );

      if (dectip_ahb_halfword())
      {
         gprint("\n#################### DECT ahb halfword test pass ####################\n\n", 0,0,0 );
      }	   
      else
      {
         gprint("\n#################### DECT ahb halfword test fail ####################\n\n", 0,0,0 );
         dumpTestRegs();
      }

      dectIPReset();
      break;


      case 3:

#ifndef AHB_AUTO_SWAP
      dectShimSetAHBSwapMode( AHB_SWAP_NONE );
#endif
      gprint("\n#################### DECT ahb word test start ####################\n", 0,0,0 );

      if (dectip_ahb_word())
      {
         gprint("\n#################### DECT ahb word test pass ####################\n\n", 0,0,0 );
      }	   
      else
      {
         gprint("\n#################### DECT ahb fword test fail ####################\n\n", 0,0,0 );
         dumpTestRegs();
      }	   

      dectIPReset();
      break;


      case 4:
         
      gprint("\n#################### DECT apb test start ####################\n", 0,0,0 );

      if (dectip_apb())
      {
         gprint("\n#################### DECT APB test pass ####################\n", 0,0,0 );
      }	   
      else
      {
         gprint("\n#################### DECT APB test fail ####################\n", 0,0,0 );
         dumpTestRegs();
      }

      dectIPReset();
      break;

      default:         
      break;
   }      
#if 0   
   dectShimSetAHBSwapMode( AHB_SWAP_16_BIT );

   gprint("\n#################### DECT dsp test start ####################\n", 0,0,0 );

   if (dectip_dsp_test())
   {
      gprint("\n#################### DECT dsp test pass ####################\n", 0,0,0 );
   }	   
   else
   {
      gprint("\n#################### DECT dsp test fail ####################\n", 0,0,0 );
      dumpTestRegs();
   }
#endif 

#if 0   
   dectShimSetAHBSwapMode( AHB_SWAP_16_BIT );

   gprint("\n#################### DECT debugger test start ####################\n", 0,0,0 );

   if (dectip_debugger())
   {
      gprint("\n#################### DECT debugger test pass ####################\n", 0,0,0 );
   }	   
   else
   {
      gprint("\n#################### DECT debugger test fail ####################\n", 0,0,0 );
      dumpTestRegs();
   }
#endif 

}


void ahbTest(void)
{
   unsigned long pAhbMemW = SHARED_RAM_START;
   unsigned long pAhbMemHW = SHARED_RAM_START + 0x20;
   unsigned long pAhbMemB = SHARED_RAM_START + 0x40; 

#if 0
   SetWord(0xB0E57FBC,0x03FF);
   SetWord(0xB0E57F82,0x003F);
   gprint("## get 7fbc 0x%04x  get 7f82 0x%04x \n", GetWord(0xB0E57FBC), GetWord(0xB0E57F82), 0);

   gprint("## AHB WORD READ 0x%08x 0x%08x 0x%08x\n", *pAhbMemW, *(pAhbMemW+4), *(pAhbMemW+8));
   *pAhbMemW =0xca1fbeef;
   *(pAhbMemW+4) = 0xbeefca1f;
   *(pAhbMemW+8) = 0xdeadbeef;
   gprint("## AHB after WORD WRITE 0x%08x 0x%08x 0x%08x\n", *pAhbMemW, *(pAhbMemW+4), *(pAhbMemW+8));
   gprint("\n#########",0,0,0); 

   gprint("## AHB HalfWord READ 0x%04x 0x%04x 0x%04x\n", *pAhbMemHW, *(pAhbMemHW+2), *(pAhbMemHW+4));
   *pAhbMemHW =0xca1f;
   *(pAhbMemHW+2) = 0xbeef;
   *(pAhbMemHW+4) = 0xdead;
   gprint("## AHB after Halfword WRITE 0x%04x 0x%04x 0x%04x\n", *pAhbMemHW, *(pAhbMemHW+2), *(pAhbMemHW+4));
   gprint("\n#########",0,0,0); 
   
   gprint("## AHB Byte READ 0x%02x 0x%02x 0x%02x\n", *pAhbMemB, *(pAhbMemB+1), *(pAhbMemB+2));
   *pAhbMemB =0xca;
   *(pAhbMemB+1) = 0xbe;
   *(pAhbMemB+2) = 0xde;
   gprint("## AHB after Byte WRITE 0x%02x 0x%02x 0x%02x\n", *pAhbMemB, *(pAhbMemB+1), *(pAhbMemB+2));
   gprint("\n#########",0,0,0); 
#endif

   gprint("## AHB 32bit READ 0x%08x 0x%08x 0x%08x\n", GetDWord(pAhbMemW), GetDWord(pAhbMemW+4), GetDWord(pAhbMemW+8));
   SetDWord(pAhbMemW, 0xaabbccdd);
   SetDWord(pAhbMemW+4, 0xeeff0011);
   SetDWord(pAhbMemW+8, 0x22334455);
   gprint("## AHB after 32bit byte read 0x%02x 0x%02x 0x%02x\n", GetByte(pAhbMemW), GetByte(pAhbMemW+1), GetByte(pAhbMemW+2));
   gprint("## AHB after 32bit byte read 0x%02x 0x%02x 0x%02x\n", GetByte(pAhbMemW+3), GetByte(pAhbMemW+4), GetByte(pAhbMemW+5));
   gprint("## AHB after 32bit byte read 0x%02x 0x%02x 0x%02x\n", GetByte(pAhbMemW+6), GetByte(pAhbMemW+7), GetByte(pAhbMemW+8));
   gprint("## AHB after 32bit WRITE 0x%08x 0x%08x 0x%08x\n", GetDWord(pAhbMemW), GetDWord(pAhbMemW+4), GetDWord(pAhbMemW+8));
   gprint("\n#########",0,0,0); 

#if 1   
   gprint("## AHB 16bit READ 0x%04x 0x%04x 0x%04x\n", GetWord(pAhbMemHW), GetWord(pAhbMemHW+2), GetWord(pAhbMemHW+4));
   SetWord(pAhbMemHW, 0x6677);
   SetWord(pAhbMemHW+2, 0x8899);
   SetWord(pAhbMemHW+4,  0x0011);
   gprint("## AHB after 16bit WRITE 0x%04x 0x%04x 0x%04x\n", GetWord(pAhbMemHW), GetWord(pAhbMemHW+2), GetWord(pAhbMemHW+4));
   gprint("## AHB after 16bit byte read 0x%02x 0x%02x 0x%02x\n", GetByte(pAhbMemHW), GetByte(pAhbMemHW+1), GetByte(pAhbMemHW+2));
   gprint("## AHB after 16bit byte read 0x%02x 0x%02x 0x%02x\n", GetByte(pAhbMemHW+3), GetByte(pAhbMemHW+4), GetByte(pAhbMemHW+5));
   gprint("\n#########",0,0,0); 
#endif

   gprint("## AHB 8bit READ 0x%02x 0x%02x 0x%02x\n", GetByte(pAhbMemB), GetByte(pAhbMemB+1), GetByte(pAhbMemB+2));
   SetByte(pAhbMemB, 0x01);
   SetByte(pAhbMemB+1, 0x23);
   SetByte(pAhbMemB+2, 0x34);
   gprint("## AHB after 8bit WRITE 0x%02x 0x%02x 0x%02x\n", GetByte(pAhbMemB), GetByte(pAhbMemB+1), GetByte(pAhbMemB+2));
   gprint("\n#########",0,0,0); 


}	
 
void apbTest(void)
{
   XDRV_UINT32 * pApbMem = DECT_APB_REG_BASE; // + 0x202;
   gprint("## APB READ 0x%08x @ 0x%08x    %d\n", *pApbMem, pApbMem, 0);
   *pApbMem =0xca1fbeef;
   gprint("## APB WRITE 0x%08x @ 0x%08x     %d\n", *pApbMem, pApbMem, 0);
}

void shmTest(void)
{
   XDRV_UINT32 * pShmMem = DECT_SHIM_TEST_BASE;
   gprint("## SHIM READ 0x%08x @ 0x%08x    %d\n", *pShmMem, pShmMem, 0);
   *pShmMem =0xAAAA5555;
   gprint("## SHIM WRITE 0x%08x @ 0x%08x     %d\n", *pShmMem, pShmMem, 0);
}



void dumpTestRegs( void )
{
   gprint("########## REG TEST_REG : 0x%04x\n #########",*(unsigned short *)TEST_REG, 0 ,0 );   
   gprint("########## REG RESULT_REG : 0x%04x\n #########",*(unsigned short *)RESULT_REG, 0 ,0 );   
   gprint("########## REG DEBUGGER_REG : 0x%04x\n #########",*(unsigned short *)DEBUGGER_REG, 0 ,0 );   
   gprint("########## REG DEBUGGER2_REG : 0x%04x\n #########",*(unsigned short *)DEBUGGER2_REG, 0 ,0 );   
   gprint("########## REG CONFIG_REG : 0x%04x\n #########",*(unsigned short *)CONFIG_REG, 0 ,0 );   
   gprint("########## REG WTF_COUNTER : 0x%04x\n #########",*(unsigned short *)WTF_COUNTER, 0 ,0 );   
   gprint("########## REG ZC1_COUNTER : 0x%04x\n #########",*(unsigned short *)ZC1_COUNTER, 0 ,0 );   
   gprint("########## REG ZC2_COUNTER : 0x%04x\n #########",*(unsigned short *)ZC2_COUNTER, 0 ,0 );   
   gprint("########## REG DSP_IRQ_OUT_COUNTER : 0x%04x\n #########",*(unsigned short *)DSP_IRQ_OUT_COUNTER, 0 ,0 );   
   gprint("########## REG DSP_INT_EXT : 0x%04x\n #########",*(unsigned short *)DSP_INT_EXT, 0 ,0 );   
   gprint("########## REG DSP_BREAK_OUT_COUNTER : 0x%04x\n #########",*(unsigned short *)DSP_BREAK_OUT_COUNTER, 0 ,0 );   
   gprint("########## REG DSP_BREAK_IN : 0x%04x\n #########",*(unsigned short *)DSP_BREAK_IN, 0 ,0 );   
}

inline void dectIPReset( void )
{
   DECT_CTRL->dect_shm_ctrl |= DECT_SOFT_RESET;
   DECT_CTRL->dect_shm_ctrl &= ~DECT_SOFT_RESET;
}

inline void dectShimSetAHBSwapMode(XDRV_UINT32 mode)
{
   DECT_CTRL->dect_shm_ctrl &= ~AHB_SWAP_MASK;
   DECT_CTRL->dect_shm_ctrl |= mode;
}

void MemoryLatencyTest1(void)
{
   //go_cacheable();

   int i;
   unsigned int tpNum = 0;
   unsigned int val1 = 0;
   unsigned int val2 = 0;
   unsigned int val3 = 0;

   asm(".set mips32; mfc0 %0, $22, 3" : "=d" (tpNum) : );

   for (i=0; i<1000; i++)
   {
      val3++;

      if ( val3 == 1 )
      {
         val2 = ((unsigned int)(&val1) | 0x20000000) ;
      }
      else if ( val3 == 2 )
      {
         val2 = ((unsigned int)(&val1) & 0x9FFFFFFF) ;
      }
      else
      {
         val2 = DECT_SHIM_CTRL_BASE; //0xb0e50000; //0xb0000100;
         val3 = 0;
      }


      printk("\n##### DDR ACCESS FROM: 0x%08x #####\n", val2 );
#if 0

      asm(".set noreorder");

      /* Load address of variable on stack */
      asm(".set mips32; move $10, %0" : : "d" (val2) );

      /* Read first timestamp */
      asm(".set mips32; mfc0 $8, $9, 0");

      /* Read from memory */
      asm(".set mips32; lw  $11, 0($10)");

      /* Flushing the pipeline */
//      asm("nop");
//      asm("nop");
//      asm("nop");
//      asm("nop");
//      asm("nop");

      asm(".set mips32; or $11, $11, $9");

      /* Read 2nd time stamp */
      asm(".set mips32; mfc0 $9, $9, 0");

      /* Subtract two timestamps */
      asm(".set mips32; subu %0, $9, $8" : "=d" (val1) : );

      asm(".set reorder");
#endif
      

      printk("\n##### diff: %d #######\n", val1 );

   }
}

void MemoryLatencyTest(void)
{
   unsigned long lastCount;
   unsigned long currentCount;
   unsigned long add1=DSP_MAIN_CNT_REG; //SHARED_RAM_START;
   unsigned long add2=DSP_MAIN_CNT_REG+16; //SHARED_RAM_START+4;
   unsigned long add3=DSP_MAIN_CNT_REG+32; //SHARED_RAM_START+6;
   XDRV_UINT16 val,val1,val2;
   int i,j;
 
   rdtscl(lastCount);  
   for ( i=0; i<100; i++)
   {

      //barrier();
      //SetWord(add1,i);
      val = GetWord(add1) & 0xFF;
      if ((val < 0x1E) && (i == 0))
      {val = 0;}
      
      //SetWord(add2,i);
      //val1 = GetWord(add2);
      //SetWord(add3,i);
      //val2 = GetWord(add3);      
      //barrier();

   }
   barrier();
   rdtscl(currentCount);
   printk(" diff = %d, val = %d \n", (currentCount - lastCount), val);

   
}

void SetGpioTestMode(void)
{
#if 0
  //INT_CTRL->int_extrachipirqmask = 0xfffff0ff;
  GPIO_BLOCK_CTL->gpio_diagselcontrol = 0x0801;
  GPIO_BLOCK_CTL->gpio_test_control = 0x28A5;
  GPIO_BLOCK_CTL->gpio_dis_tpout = 0xfff87fff;
  GPIO_BLOCK_CTL->gpio_base_mode = 0x3;
#else
  GPIO_BLOCK_CTL->gpio_base_mode |= 0x20; //bit 5 -- DECTPD_OVERRIDE  

#endif
}
