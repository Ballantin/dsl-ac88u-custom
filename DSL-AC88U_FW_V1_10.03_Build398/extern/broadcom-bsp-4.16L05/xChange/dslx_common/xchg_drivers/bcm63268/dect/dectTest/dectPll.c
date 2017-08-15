/* Broadcom Corp. */

/* 
** Always include this file before any other include files
** to avoid conflicts with remapped printf command 
*/
#include <kernelMainWrapper.h>
#include <linux/dma-mapping.h>

#include "bcm63268Dect.h"

void dectPllConfig()
{
   int i;
   
      
   INT_CTRL->int_softresetb |= SOFT_RST_DECT;
  
   
/* Enable master clock to dect block */
   INT_CTRL->int_clkcontrol |= DECT_CLKEN;
   
   /* Pwr on the PLLs */
   DECT_CTRL->dect_shm_pll_reg_1 &= ~(PLL_PWRDWN | PLL_REFCOMP_PWRDOWN | PLL_NDIV_PWRDOWN | PLL_CH1_PWRDOWN | PLL_CH2_PWRDOWN | PLL_CH3_PWRDOWN);
   
   //DECT_CTRL->dect_shm_pll_reg_1 &= ~(1 << 6);   // Set VCO Range = 800-1599Mhz
  
   //DECT_CTRL->dect_shm_pll_reg_2 = 0xC0000000;   // Set Ndiv_int=192 (default setting)
   //DECT_CTRL->dect_shm_pll_reg_3 = 0x11101010;     // Set P2div=1, P1div=1, Mdiv_ch3=16, Mdiv_ch2=16, Mdiv_ch1=16
   
   //DECT_CTRL->dect_shm_pll_reg_0 = 0x382c2860;   // Default setting
   DECT_CTRL->dect_shm_pll_reg_0 = 0x382c6860;     // Set Cz=0b11 when using 10.368Mhz XTAL
   
   /* Bring Analog portion out of reset */
   DECT_CTRL->dect_shm_pll_reg_1 &= ~PLL_ARESET;
   
   /* Check for Phase lock */
   for ( i=0; i<100; i++ )
   {
      if ( DECT_CTRL->dect_shm_irq_status & ( 1 << DECT_SHM_IRQ_PLL_PHASE_LOCK ) )
      {
         /* PLL is locked */
         gprint("####################SUCCESS: DECT PLL IS LOCKED#################\n", 0, 0, 0);
         gprint("dect shm irq status: 0x%08x, i: %d\n", DECT_CTRL->dect_shm_irq_status, i, 0);
         //break;
      }
      else
      {
         gprint("###FAILURE#FAILURE: DECT PLL IS NOT LOCKED##FAILURE#FAILURE###\n", 0, 0, 0);         
         gprint("dect shm irq status: 0x%08x, i: %d\n", DECT_CTRL->dect_shm_irq_status, i, 0);
      }
   }
   
   if (( i == 100 ) && ((DECT_CTRL->dect_shm_irq_status & ( 1 << DECT_SHM_IRQ_PLL_PHASE_LOCK ))!=0x80))
   {
      /* Error PLL not locked */
      gprint("####################ERROR: DECT PLL IS NOT LOCKED#################\n", 0, 0, 0);
   }
   else
   {
      /* PLL locked, bring Digital Portion out of reset */
      DECT_CTRL->dect_shm_pll_reg_1 &= ~PLL_DRESET;
   }

   gprint("DECT PLL init completed. DECT PLL registers set to:\n", 0, 0, 0);
   gprint("DECT_CTRL->dect_shm_pll_reg_0       = 0x%08X \n", (unsigned int)DECT_CTRL->dect_shm_pll_reg_0, 0, 0);
   gprint("DECT_CTRL->dect_shm_pll_reg_1       = 0x%08X \n", (unsigned int)DECT_CTRL->dect_shm_pll_reg_1, 0, 0);
   gprint("DECT_CTRL->dect_shm_pll_reg_2       = 0x%08X \n", (unsigned int)DECT_CTRL->dect_shm_pll_reg_2, 0, 0);
   gprint("DECT_CTRL->dect_shm_pll_reg_3       = 0x%08X \n", (unsigned int)DECT_CTRL->dect_shm_pll_reg_3, 0, 0);

#if 1 /* Configure switches when using DECT PLL as main clock source for DECT */
   DECT_CTRL->dect_shm_ctrl |= DECT_PLL_REF_DECT | DECT_CLK_CORE_DECT | DECT_CLK_OUT_XTAL | 
                               PHCNT_CLK_SRC_PLL |
                               PCM_PULSE_COUNT_ENABLE | DECT_PULSE_COUNT_ENABLE;
                                                    
#endif

}


void dectPCMPLLConfig( void )
{
   int i;
   
   /* Enable master clock to dect block */
   INT_CTRL->int_clkcontrol |= DECT_CLKEN;
      
   /* Power up PLL */
   PCM_CTRL->pcm_pll_ctrl1 &= ~(PCM_PLL_PWRDN | PCM_PLL_PWRDN_CH1 | PCM_PLL_REFCMP_PWRDN ); 
   //PCM_CTRL->pcm_pll_ctrl1 &= ~(PCM_PLL_PWRDN | PCM_PLL_REFCMP_PWRDN );  // Power up PLL, power up RefComp, Keep Ch1 off
   PCM_CTRL->pcm_pll_ctrl4 &= ~(0x00100000);  // Turn on CH2

#if 0  // 10.368 Mhz XTAL, PCM CH1=8.192Mhz, PCM CH2=8.192Mhz
   PCM_CTRL->pcm_pll_ctrl1   = 0x1CC011F3;  // Set P1div=1, P2div=1, Ndiv_int=192, M1div=243, using 10.368 for PCM PLL => 8.192
   PCM_CTRL->pcm_pll_ctrl2   = 0x12000000;  // Modulator Active, Bypass Enabled, Dithering Off, VCO>=1600Mhz, NDIV_Mode=MFB, NDIV_FRAC=0 
   PCM_CTRL->pcm_pll_ctrl3   = 0x382c6860;  // Set Cz=0b11 when using 10.368Mhz XTAL
   PCM_CTRL->pcm_pll_ctrl4   = 0x010F3015;  // Set reference clock to 10.368Mhz from DECT, Turn on PCM PLL CH2, set M2DIV=243 => 8.192
#endif
#if 0  // 10.368 Mhz XTAL, PCM CH1=16.384Mhz, PCM CH2=16.384Mhz
   PCM_CTRL->pcm_pll_ctrl1   = 0x1CC0117A;  // Set P1div=1, P2div=1, Ndiv_int=192, M1div=122, using 10.368 for PCM PLL => 16.384 (w/ndiv_frac)
   PCM_CTRL->pcm_pll_ctrl2   = 0x12CA4588;  // Modulator Active, Bypass Enabled, Dithering Off, VCO>=1600Mhz, NDIV_Mode=MFB, NDIV_FRAC=0xCA4588 => 16.384
   PCM_CTRL->pcm_pll_ctrl3   = 0x382c6860;  // Set Cz=0b11 when using 10.368Mhz XTAL
   PCM_CTRL->pcm_pll_ctrl4   = 0x0107A015;  // Set reference clock to 10.368Mhz from DECT, Turn on PCM PLL CH2, set M2DIV=122 => 16.384 (w/ndiv_frac)
#endif
#if 1  // 64 Mhz XTAL, PCM CH1=16.384Mhz, PCM CH2=16.384Mhz
   PCM_CTRL->pcm_pll_ctrl1   = 0x1C40127D;  // Set P1div=2, P2div=1, Ndiv_int=64, M1div=125, using 64MHz for PCM PLL => 16.384
   PCM_CTRL->pcm_pll_ctrl2   = 0x12000000;  // Modulator Active, Bypass Enabled, Dithering Off, VCO>=1600Mhz, NDIV_Mode=MFB, NDIV_FRAC=0 
   PCM_CTRL->pcm_pll_ctrl3   = 0x382c2860;  // When using 64Mhz XTAL
   PCM_CTRL->pcm_pll_ctrl4   = 0x0007D015;  // Set reference clock to 64MHz XTAL, Turn on PCM PLL CH2, set M2DIV=125  => 16.384
#endif   
#if 0  // 64 Mhz XTAL, PCM PLL source for both DECT and PCM, PCM CH1=8.192Mhz, PCM CH2=124.416Mhz
   PCM_CTRL->pcm_pll_ctrl1   = 0x1C5D13F3;  // Set P1div=3, P2div=1, Ndiv_int=93, M1div=243, using 64Mhz for PCM PLL => 8.192 (w/ndiv_frac)   
   PCM_CTRL->pcm_pll_ctrl2   = 0x124FDF3B;  // Modulator Active, Bypass Enabled, Dithering Off, VCO>=1600Mhz, NDIV_Mode=MFB, NDIV_FRAC=0x4FDF3B => 8.192/124.416  
   PCM_CTRL->pcm_pll_ctrl3   = 0x382c2860;  // When using 64Mhz XTAL
   PCM_CTRL->pcm_pll_ctrl4   = 0x00010015;  // Set reference clock to 64MHz XTAL, Turn on PCM PLL CH2, set M2DIV=16   => 124.416 (w/ndiv_frac) 
#endif

   /* Remove Analog Reset */
   PCM_CTRL->pcm_pll_ctrl1 &= ~PCM_PLL_ARESET;

   /* Remove Digital Reset */
   //PCM_CTRL->pcm_pll_ctrl1 &= ~PCM_PLL_DRESET;

   /* Clear clk 16 reset */
   PCM_CTRL->pcm_pll_ctrl1 &= ~PCM_CLK16_RESET;

   bosSleep(10);

#if 0
   if ( (PCM_CTRL->pcm_pll_stat & PCM_PLL_LOCK) != PCM_PLL_LOCK )
   {
      gprint("ERROR: PCM PLL didn't lock to programmed output frequency 0x%08x\n", (unsigned int)PCM_CTRL->pcm_pll_stat, 0, 0);
   }
   else
   {
      gprint("*************PCM PLL locked!*************\n", 0, 0, 0);
   }
#endif

   /* Check for Phase lock */
   for ( i=0; i<100; i++ )
   {
      if ( PCM_CTRL->pcm_pll_stat & PCM_PLL_LOCK )
      {
         /* PLL is locked */
         gprint("####################SUCCESS: PCM PLL IS LOCKED#################\n", 0, 0, 0);
         gprint(" PCM PLL Status: 0x%08x, i: %d\n", PCM_CTRL->pcm_pll_stat, i, 0);
         //break;
      }
      else
      {
         gprint("###FAILURE#FAILURE: PCM PLL IS NOT LOCKED##FAILURE#FAILURE###\n", 0, 0, 0);            
         gprint("PCM PLL Status: 0x%08x, i: %d\n", PCM_CTRL->pcm_pll_stat, i, 0);
      }
   }
   
   
   if (( i == 100 ) && ((PCM_CTRL->pcm_pll_stat & PCM_PLL_LOCK) != 0x1))
   {
      /* Error PLL not locked */
      gprint("####################ERROR: PCM PLL IS NOT LOCKED#################\n", 0, 0, 0);
   }
   else
   {
      /* PLL locked, bring Digital Portion out of reset */
      PCM_CTRL->pcm_pll_ctrl1 &= ~PCM_PLL_DRESET;
   }


   gprint("PLL init completed. PLL registers set to:\n", 0, 0, 0);
   gprint("PCM->pcm_pll_ctrl1       = 0x%08X \n", (unsigned int)PCM_CTRL->pcm_pll_ctrl1, 0, 0);
   gprint("PCM->pcm_pll_ctrl2       = 0x%08X \n", (unsigned int)PCM_CTRL->pcm_pll_ctrl2, 0, 0);
   gprint("PCM->pcm_pll_ctrl3       = 0x%08X \n", (unsigned int)PCM_CTRL->pcm_pll_ctrl3, 0, 0);
   gprint("PCM->pcm_pll_ctrl4       = 0x%08X \n", (unsigned int)PCM_CTRL->pcm_pll_ctrl4, 0, 0);

#if 0 /* Configure switches when using PCM PLL as main clock source for DECT */
   DECT_CTRL->dect_shm_ctrl |= DECT_PLL_REF_DECT | DECT_CLK_CORE_PCM | DECT_CLK_OUT_XTAL | 
                               PHCNT_CLK_SRC_XTAL |
                               PCM_PULSE_COUNT_ENABLE | DECT_PULSE_COUNT_ENABLE;
#endif
}

