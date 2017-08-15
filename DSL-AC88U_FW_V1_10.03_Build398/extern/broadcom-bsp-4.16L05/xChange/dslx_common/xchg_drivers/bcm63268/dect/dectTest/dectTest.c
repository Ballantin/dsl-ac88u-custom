/*                                                         
BROADCOM CORP.
*/

/* 
** Always include this file before any other include files
** to avoid conflicts with remapped printf command 
*/
#include <kernelMainWrapper.h>
#include <linux/dma-mapping.h>

#include <xdrvTypes.h>
#include "dectTest.h"
#include "xdrvIntCtrl.h"  /* Linux Interrupt Example Test */
#include "63268_intr.h"    
#include "bcm63268Dect.h"  

#define MY_DIP_RAM_START (DECT_AHB_SHARED_RAM_BASE + 0xfa00)

/* External test functions */
extern int dectip_apb(void);
extern int dectip_ahb_byte(void);
extern int dectip_ahb_halfword(void);
extern int dectip_ahb_word(void);
extern inline void dectIPReset( void );
extern inline void dectShimSetAHBSwapMode(XDRV_UINT32 mode);

extern void dectPllConfig(void);
extern void dectPCMPllConfig(void);
extern void dectIfTest(XDRV_UINT32 test);
extern void dectDMATestStart(void);

extern void isr_dsp_int(void);
extern void isr_dip_int(void);
extern void dectip_int(void);
extern void dectip_aux(void);

void irqTest( void );
static void irqEnable( XDRV_INT_CTRL_INTERRUPT irqID );
static void irqDisable( XDRV_INT_CTRL_INTERRUPT irqID );
static unsigned int testIsr( void );
extern int dectip_gauss_010101(void);
extern int dectip_gauss_000111(void);
extern int dectip_gauss_data063(void);
extern int dectip_gauss_mid900mv(void); 

extern void MemoryLatencyTest(void);
extern void SetGpioTestMode(void);
extern int dectip_pin(void);
extern void dectip_tdo_rdi(void);

unsigned char test_buf[40];

/* Re-define all instances of gprint in SITEL IP test code */
void gprint(const char *fmt, unsigned long arg0, unsigned long arg1, unsigned long arg2)
{
   printk( fmt, arg0, arg1, arg2 );
}

/* Main DECT Test function */
void dectTest( XDRV_UINT32 testNum )
{
   unsigned short *s;

   switch( testNum )
   {
      case 0:
         printk("DECT TEST %d: DECT PLL Config\n", testNum);
         dectPllConfig();
         dectShimSetAHBSwapMode( AHB_SWAP_ACCESS );
         break;
      case 1:
         printk("DECT TEST %d: PCM PLL Config\n", testNum);
         dectPCMPLLConfig();
         break;
      case 2:
         printk("DECT TEST %d: APB\n", testNum);
         dectIfTest(4);
         break;
      case 3:
         printk("DECT TEST %d: AHB - Byte\n", testNum);
         dectIfTest(1);
         break;
      case 4:
         printk("DECT TEST %d: AHB - Half Word\n", testNum);
         dectIfTest(2);
         break;
      case 5:
         printk("DECT TEST %d: AHB - Word\n", testNum);
         dectIfTest(3);
         break;
      case 6:
         printk("DECT TEST %d: DECT - INT\n", testNum);
         dectip_int(); 
         break;
      case 7:
         printk("DECT TEST %d: DECT - AUX\n", testNum);
         dectip_aux();
         break;
      case 8:
         printk("DECT TEST %d: BRCM DECT DMA\n", testNum);
         dectDMATestStart();
         break;
      case 9:
         printk("DECT TEST %d: IRQ EXAMPLE TEST\n", testNum);
         irqTest();
         break;
      case 10:
         printk("DECT TEST %d: GAUSSIAN pattern 010101\n", testNum);
         dectShimSetAHBSwapMode( AHB_SWAP_16_BIT );
         if (dectip_gauss_010101())
         {
            printk ("GAUSSIAN pattern 010101 test passed!");
         }
         else
         {
            printk ("GAUSSIAN pattern 010101 test failed!");
         }
         break;
      case 11:
         printk("DECT TEST %d: GAUSSIAN pattern 000111\n", testNum);
         dectShimSetAHBSwapMode( AHB_SWAP_16_BIT );
         if (dectip_gauss_000111())
         {
            printk ("GAUSSIAN pattern 000111 test passed!");
         }
         else
         {
            printk ("GAUSSIAN pattern 000111 test failed!");
         }
         break;
      case 12:
         printk("DECT TEST %d: GAUSSIAN pattern data063\n", testNum);
         dectShimSetAHBSwapMode( AHB_SWAP_16_BIT );
         if ( dectip_gauss_data063())
         {
            printk ("GAUSSIAN pattern data063 test passed!");
         }
         else
         {
            printk ("GAUSSIAN pattern data063 test failed!");
         }         
         break;
      case 13:
         printk("DECT TEST %d: GAUSSIAN pattern mid900mv\n", testNum);
         dectShimSetAHBSwapMode( AHB_SWAP_16_BIT );
         if ( dectip_gauss_mid900mv())
         {
            printk ("GAUSSIAN pattern mid900mv test passed!");
         }
         else
         {
            printk ("GAUSSIAN pattern mid900mv test failed!");
         }
         break;
 
      case 14:
         printk("Soft Reset DECT IP block\n");
         dectIPReset();
         break;  
      case 15:


         s = (unsigned short*)(MY_DIP_RAM_START);
      
         dectShimSetAHBSwapMode( AHB_SWAP_ACCESS  );
         s[0x10] = 0x6061;
         s[0x11] = 0x6263;
         s[0x12] = 0x6465;
         s[0x13] = 0x6667;

         dectShimSetAHBSwapMode( AHB_SWAP_16_BIT );
         s[0x20] = 0x6061;
         s[0x21] = 0x6263;
         s[0x22] = 0x6465;
         s[0x23] = 0x6667;
 
         dectShimSetAHBSwapMode( AHB_SWAP_ACCESS  );
         printk("DIP %4.4x %4.4x %4.4x %4.4x   %8.8x %8.8x\n",
             s[0x10], s[0x11], s[0x12], s[0x13],
             *(unsigned int*)&s[0x10], *(unsigned int*)&s[0x12]);
         printk("DIP %4.4x %4.4x %4.4x %4.4x   %8.8x %8.8x\n",
             s[0x20], s[0x21], s[0x22], s[0x23],
             *(unsigned int*)&s[0x20], *(unsigned int*)&s[0x22]);
      
         break;
      case 16:
         printk("****** Memory Latency tests\n");
         MemoryLatencyTest();
         break;  

      case 17:
         printk("****** GPIO Diagnostic mode ******");
         SetGpioTestMode();
         break;

      case 18:
         if (dectip_pin())
         {
            printk("Pin test passed\n");
         }
         else 
         {  
            printk("Pin test failed\n");
         }
         break;  
         
      case 19:
         printk("tdo_rdi test start ....\n"); 
         dectip_tdo_rdi();
         printk("tdo_rdi test end ....\n"); 
         break;
                 

      /* Add cases above as required */                     
      default:
         printk("Incorrect DECT TEST specified!\n");
         break;
    }
}



void irqTest( void )
{
   XDRV_INT_CTRL_INTERRUPT irqID = INTERRUPT_ID_SOFTWARE_1;    /* Fake S/W interrupt for testing */

   irqEnable(irqID);
   
   printk("Waiting for interrupt...\n");
   xdrvIntCtrlGenerateSoftInterrupt( INTERRUPT_ID_SOFTWARE_1 ); /* Fake interrupt generation for testing */
  
   irqDisable(irqID);
   printk("Interrupt test completed.\n");
}

static void irqEnable( XDRV_INT_CTRL_INTERRUPT irqID )
{

   xdrvIntCtrlDisableInterrupt( irqID );
   xdrvIntCtrlInstallInterrupt( irqID,
                                testIsr,
                                0 /* isr parm */ );

   /* Disable */
   xdrvIntCtrlDisableInterrupt( irqID );
   /* Perform actions */
   
   /* Enable */
   xdrvIntCtrlEnableInterrupt( irqID );
   
}

static void irqDisable( XDRV_INT_CTRL_INTERRUPT irqID )
{
   
   xdrvIntCtrlDisableInterrupt( irqID );
   xdrvIntCtrlUninstallInterrupt( irqID );
    
}

static unsigned int testIsr( void )
{
   printk("INTERRUPT SERVICE ROUTINE triggered!...\n");
   
   /* Depends on edge or level interrupts */
   /* Clear interrupt status bits, if req'd */
   /* Enable interrupt again, if req'd */
   
   return 0;
}



unsigned int GetDWord(unsigned long a)
{
//   return *((unsigned int*)a);
   __asm("lw $2,0($4)"); 
}

unsigned short GetWord(unsigned long a)
{
//   return *((unsigned short*)a);
   __asm("lhu $2,0($4)"); 
}

unsigned char GetByte(unsigned long a)
{
//   return *((unsigned char*)a);
   __asm("lbu $2,0($4)"); 
}

void SetDWord(unsigned long a, unsigned int d)
{
//   *((unsigned int*)a) = d
   __asm("sw $5,0($4)"); 
}

void SetWord(unsigned long a, unsigned short d)
{
//   *((unsigned short*)a) = d;
   __asm("sh $5,0($4)"); 
}

void SetByte(unsigned long a, unsigned char d)
{
//   *((unsigned char*)a) = d
   __asm("sb $5,0($4)");
}


