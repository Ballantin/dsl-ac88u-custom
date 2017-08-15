/****************************************************************************
*
*  Copyright (c) 2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*    Filename: ntr6328.c
*
****************************************************************************
*
*    Description:  NTR device driver for BCM6328
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

/* 
** Always include this file before any other include files
** to avoid conflicts with remapped printf command 
*/
#include <kernelMainWrapper.h>
#include <linux/dma-mapping.h>

#include <xdrvTypes.h>        /* Primitive types definitions */
#include <xdrvLog.h>          /* logging */

#include <bosSleep.h>         /* BOS sleep */
#include <bcmChip.h>         /* chip-specific PCM memory maps */
#include <ntr6328.h>

/* This debug flag is set in ntrSync.h userspace */
#if ( NTR_DEBUG_LOG )
#  define NTR_LOGDEBUG(x) XDRV_LOG_ERROR(x)
#else
#  define NTR_LOGDEBUG(x)
#endif

#define  PCM_PLL_REG1_RESERVED_0_MASK       (1<<28)/* must write 0 to this bit */
#define  PCM_PCM_PLL_REG1_NDIV_INT_MASK_OFF (16) /* Ndiv Int feedback register offset */
#define  PCM_PCM_PLL_REG1_NDIV_INT_MASK     (0x1FF)  /* mask for all of Ndiv Int bits */

#define  PCM_PLL_REG2_RESERVED_0_MASK       (1<<27)   // 2F /* must write '0' to this bit */
#define  PCM_PCM_REG2_NDIV_FRAC_MASK        (0xFFFFFF) /* bit length for Ndiv fine grain feedback */
#define  PCM_PLL_NDIV_MODE_INTEGER_N_MODE   (0x0) /* 3 bits; disabled feedback divider */
#define  PCM_PLL_NDIV_MODE_MFB_MODE         (0x2) /* 3 bits; enables feedback divider */
#define  PCM_PLL_NDIV_MODE_OFF              (24)  /* offset for feedback divider bits */
#define  PCM_PLL_NDIV_MODE_ALL_MASK_1       (0x7) /* mask used to blank out feedback bits */
#define  PCM_PLL_NDIV_PWRDOWN_OFF           (31)  /* power down ctrl2 */
#define  PCM_PLL_BYPASS_MODULATOR_OFF       (30)  /*  */

#define  GPIO_MODE_NTR_ENABLE               (1<<15)

/* Platform specific: GPIO 31 is for NTR Compatibility mode */
static XDRV_UINT8 gpio31InitFlag = 0;
static void ntr6328_init(void)
{
   if ( !gpio31InitFlag )
   {
      gpio31InitFlag = 1;

      PCM->pcm_ctrl  |= PCM_NTR_ENABLE;
      GPIO->GPIODir  &= ~(1<<31);
   
      bosSleep(10);
   }
   return;
}

void ntr6328_getCounts( XDRV_UINT32 * localCount, XDRV_UINT32 * ntrCount )
{
   /* Macro that accesses the mips counter (200MHz on 6328, 150MHz on 6358) */
   #define rdtscl(dest) __asm__ __volatile__("mfc0 %0,$9; nop" : "=r" (dest))

   ntr6328_init();

   rdtscl(*localCount);                   /* obtain local count */
   (*ntrCount) = PCM->pcm_ntr_counter;    /* obtain NTR register count */
}

void ntr6328_getNdiv( XDRV_UINT32 * ndivInt, XDRV_UINT32 * ndivFrac )
{
   XDRV_UINT32 ctrl1, ctrl2;
   
   ctrl1 = (PCM->pcm_pll_ctrl1); /* read PCM_PLL_REG1 */
   ctrl2 = (PCM->pcm_pll_ctrl2); /* read PCM_PLL_REG2 */
   
   *ndivInt  = (ctrl1 & ( PCM_PCM_PLL_REG1_NDIV_INT_MASK << PCM_PCM_PLL_REG1_NDIV_INT_MASK_OFF )) >> PCM_PCM_PLL_REG1_NDIV_INT_MASK_OFF; 
   *ndivFrac = (ctrl2 & ( PCM_PCM_REG2_NDIV_FRAC_MASK ) );   
   
}

void ntr6328_setFeedback
( 
   XDRV_SINT32        pcmStepsAdjust, 
   XDRV_BOOL          isEnbl
)
{
   XDRV_UINT32 ctrl1, ctrl2;
   XDRV_SINT32 newNdivFrac, newNdivFracTemp;
   XDRV_UINT32 newNdivInt, newNdivMode;

   ntr6328_init();

   NTR_LOGDEBUG(("%s: ", __FUNCTION__ ));
   NTR_LOGDEBUG(("(0) pcmStepsAdjust=[%li]", pcmStepsAdjust ));

   NTR_LOGDEBUG(("\n\nBeginning of ntr6328_setFeedback.  Dispaying unmodified registers."));
   NTR_LOGDEBUG(("[PCM_PLL_CTRL1 == 0x[%08x]", (volatile unsigned int)PCM->pcm_pll_ctrl1));
   NTR_LOGDEBUG(("[PCM_PLL_CTRL2 == 0x[%08x]", (volatile unsigned int)PCM->pcm_pll_ctrl2));
   
   if ( isEnbl )
   {
      /* Enable feedback divider */
      newNdivMode = PCM_PLL_NDIV_MODE_MFB_MODE;
   }
   else
   {
      /* Disable feedback divider */
      newNdivMode = PCM_PLL_NDIV_MODE_INTEGER_N_MODE;
   }

   ctrl1 = (PCM->pcm_pll_ctrl1); /* read PCM_PLL_REG1 */
   ctrl2 = (PCM->pcm_pll_ctrl2); /* read PCM_PLL_REG2 */

   /* mask PLL_NDIV_MODE bits to all zeros */
   ctrl2 &= ~( PCM_PLL_NDIV_MODE_ALL_MASK_1 << PCM_PLL_NDIV_MODE_OFF );
   /* apply new NDIV_MODE mask */
   ctrl2 |= ( newNdivMode << PCM_PLL_NDIV_MODE_OFF );


   /* This bit is unknown when read, but must be written 0 */
   ctrl2 &= ~( PCM_PLL_REG2_RESERVED_0_MASK );
   /* This bit is unknown when read, but must be written 0 */
   ctrl1 &= ~PCM_PLL_REG1_RESERVED_0_MASK;

   newNdivFrac = pcmStepsAdjust;
   if ( newNdivFrac >= 0 )
   {
      newNdivInt = 128 + (newNdivFrac >> 24);
   }
   else
   {
      newNdivFracTemp = ~(newNdivFrac >> 24) + 1;
      newNdivInt = 128 - newNdivFracTemp;
   }
 
   if ( newNdivInt >= 512 )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "%s: newNdivInt overran past the limit. ", __FUNCTION__ ));
       
      /* Should not get this big ... this value is obviously way off */
   } 
   else if ( newNdivInt <= 9 )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "%s: newNdivInt underran past the limit.", __FUNCTION__ ));
     
      /* Should not get this small ... this value is obviously way off */
   }

   if ( newNdivFrac >= 0 )
   {
      newNdivFrac &= ((1<<24) -1);
   }
   else
   {
      newNdivFrac &= ~(-1 << 24);
   }

   NTR_LOGDEBUG(("\n\nFinal register values"));
   NTR_LOGDEBUG(("(signed int)   pcmStepsAdjust   = [%li]", pcmStepsAdjust));
   NTR_LOGDEBUG(("NEW:(unsigned) NdivFrac      = [0x%08lx]", newNdivFrac));
   NTR_LOGDEBUG(("NEW:(unsigned) NdivInt       = [0x%08lx]", newNdivInt));

   newNdivFrac &= PCM_PCM_REG2_NDIV_FRAC_MASK;	  /* ensure the new Frac value is within bounds */
   newNdivInt &= PCM_PCM_PLL_REG1_NDIV_INT_MASK;     /* ensure the new Int value is within bounds */

   /* mask NdivFrac bits to all zeros */
   ctrl2 &= ~( PCM_PCM_REG2_NDIV_FRAC_MASK );
   /* set new NdivFrac value */
   ctrl2 |= ( newNdivFrac & PCM_PCM_REG2_NDIV_FRAC_MASK );

   /* mask PCM_NDIV_INT bits to all zeros */
   ctrl1 &= ~( PCM_PCM_PLL_REG1_NDIV_INT_MASK << PCM_PCM_PLL_REG1_NDIV_INT_MASK_OFF );
   /* set new NdivInt value */
   ctrl1 |= (( newNdivInt & PCM_PCM_PLL_REG1_NDIV_INT_MASK ) << PCM_PCM_PLL_REG1_NDIV_INT_MASK_OFF );

   /* Write new feedback values */
   PCM->pcm_pll_ctrl1 = ctrl1;
   PCM->pcm_pll_ctrl2 = ctrl2;

   if ( (PCM->pcm_pll_stat & 0x1 ) == 1 ) 
   { 
      NTR_LOGDEBUG(("%s: new PCM register values locked.", __FUNCTION__ ));
   } else {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "%s: new PCM register values NOT locked.", __FUNCTION__ ));
   }
   
   NTR_LOGDEBUG(("%s: Changes made. Displaying registers after changes", __FUNCTION__ ));
   NTR_LOGDEBUG(("[PCM_PLL_CTRL1 == 0x[%08x]", (volatile unsigned int)PCM->pcm_pll_ctrl1));
   NTR_LOGDEBUG(("[PCM_PLL_CTRL2 == 0x[%08x]", (volatile unsigned int)PCM->pcm_pll_ctrl2));
   
   return;
}

void ntr6328_print( XDRV_BOOL numSamples, XDRV_SINT32 sampleIntervalMs)
{
   if ( numSamples && sampleIntervalMs ) /* check for NULL pointers */
   {
      int zz;
      unsigned long ntrClock[numSamples], mipsClock[numSamples];

      NTR_LOGDEBUG(("[PCM_PLL_CTRL1 == 0x[%08x]", (volatile unsigned int)PCM->pcm_pll_ctrl1));
      NTR_LOGDEBUG(("[PCM_PLL_CTRL2 == 0x[%08x]", (volatile unsigned int)PCM->pcm_pll_ctrl2));

      NTR_LOGDEBUG(("Taking %u samples, with an interval of %lims between successive samples.", \
      (unsigned)numSamples, sampleIntervalMs));

      for ( zz = 0; zz < numSamples; zz++ )
      {
         /* read the latest count of the PCM NTR counter and store in ntrClock */
         ntrClock[zz] = PCM->pcm_ntr_counter;

         /* read the latest count of the PCM MIPS counter and store in mipsClock */
         rdtscl(mipsClock[zz]);   

         bosSleep(sampleIntervalMs);
      }

      NTR_LOGDEBUG((" PCM->pcm_ntr_counter [hex]"));
      NTR_LOGDEBUG((" =========================="));
      /* Display numSamples successive clocks */
      for ( zz = 0; zz < numSamples; zz++ )
      {                          
         NTR_LOGDEBUG(("%08lx", ntrClock[zz]));
      }
   
      NTR_LOGDEBUG((" PCM MIPS Counter [hex]"));
      NTR_LOGDEBUG((" =================="));
      /* Display numSamples successive clocks */
      for ( zz = 0; zz < numSamples; zz++ )
      {                          
         NTR_LOGDEBUG(("%08lx", mipsClock[zz]));
      }
   }
   else
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_XDRV, "*** Error: NULL pointer parameter passed to %s", __FUNCTION__ ));
   }
   
   return;
}
