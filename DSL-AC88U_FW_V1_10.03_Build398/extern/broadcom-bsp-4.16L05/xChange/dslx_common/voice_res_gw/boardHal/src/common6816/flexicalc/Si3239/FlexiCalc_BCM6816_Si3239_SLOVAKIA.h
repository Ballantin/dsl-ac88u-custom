/***************************************************************************
*    Copyright (c) 2000-2011 Broadcom                             
*                                                                           
*    This program is the proprietary software of Broadcom and/or
*    its licensors, and may only be used, duplicated, modified or           
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an Authorized     
*    License).  Except as set forth in an Authorized License, Broadcom      
*    grants no license (express or implied), right to use, or waiver of any 
*    kind with respect to the Software, and Broadcom expressly reserves all 
*    rights in and to the Software and all intellectual property rights     
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO 
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM   
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.                               
*                                                                           
*                                                                           
*    Except as expressly set forth in the Authorized License,               
*                                                                           
*    1.     This program, including its structure, sequence and             
*    organization, constitutes the valuable trade secrets of Broadcom, and  
*    you shall use all reasonable efforts to protect the confidentiality    
*    thereof, and to use this information only in connection with your use  
*    of Broadcom integrated circuit products.                               
*                                                                           
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    AS IS AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,              
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR  
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY        
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,    
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,    
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR         
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT  
*    OF USE OR PERFORMANCE OF THE SOFTWARE.                                 
*                                                                           
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL       
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, 
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN  
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR    
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE 
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY 
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: FlexiCalc.h
*
****************************************************************************
*    Description:
*
*    FlexiCalc output constants for the following inputs:
*
*       BCM6816                          
*       SLIC = Silicon Labs Si3239       
*       R1 = 220 ohms
*       R2 = 820 ohms
*       C1 = 115 nano Farads 
*       DLP = -6dB
*       ELP = -4dB
*       HWDACgain = 0dB
*       HW_impedance = 680 ohms
*       Protection resistor = 10 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 57Vrms
*
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_SLOVAKIA_3239_H
#define FLEXICALC_SLOVAKIA_3239_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_SLOVAKIA
#define flexicalcSLOVAKIAArchive3239 ((const APM6816_FLEXICALC_CFG*) NULL)
#else
const APM6816_FLEXICALC_CFG flexicalcSLOVAKIAArchive3239[] =
{
{
   0x3239,           /* Slic type: Si3239 */
   57,               /* Ring Voltage (RMS) */
   -6,               /* DLP - Decode Level Point (receive loss) in dB */
   -4,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0008,               /* eq_rx_shft */
   0x000B,               /* eq_tx_shft */
   0,               /* eq_imp_resp */

   /*
   ** Y-filter Coefficients
   */
   1,               /* yfltr_en */
   {  /* IIR 2 Filter Coefficients */
      0x04966,   /* Y IIR2 filter b0 */
      0x061FA,   /* Y IIR2 filter b1 */
      0x04966,   /* Y IIR2 filter b2 */
      0x4E1DC,   /* Y IIR2 filter a1 */
      0xE28AB    /* Y IIR2 filter a2 */
   },
   0x77,               /* y_iir2_filter_shift */
   {  /* Fir Filter Coefficients */
      0x3886F,   /* YFIR1_VAL */
      0x8EBBC,   /* YFIR2_VAL */
      0x1F98C,   /* YFIR3_VAL */
      0x29661,   /* YFIR4_VAL */
      0xFC763,   /* YFIR5_VAL */
      0xE3436,   /* YFIR6_VAL */
      0xF2F83,   /* YFIR7_VAL */
      0x0D367,   /* YFIR8_VAL */
      0x1168E,   /* YFIR9_VAL */
      0x00656,   /* YFIR10_VAL */
      0xF3757,   /* YFIR11_VAL */
      0xFA084,   /* YFIR12_VAL */
      0x08D25,   /* YFIR13_VAL */
      0x0AAB7,   /* YFIR14_VAL */
      0xFD79C,   /* YFIR15_VAL */
      0xF663E,   /* YFIR16_VAL */
      0x06D6E,   /* YFIR17_VAL */
      0xFBE50    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x46020},          /* y_iir1_filter[1] */
   0xA6,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,               /* hybal_shft */
   {0x3F44, 0x9A41, 0x64BE, 0x832B, 0x4BDF},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x552C, 0xD83E, 0x0984, 0xF994, 0x021F, 0xFDDB, 0x007E, 0xFEC1,
      0x001A, 0xFF1F, 0xFFF7, 0xFF62, 0xFFF0, 0xFF8B, 0xFFF4, 0xFFAC,
      0xFFF9, 0xFFC1, 0x0003, 0xFFCF, 0x0000, 0xFFDE, 0x0006, 0xFFDD,
      0x0000, 0xFFE7, 0xFFFE, 0xFFE6, 0x0000, 0xFFEB, 0xFFFA, 0xFFEF,
      0xFFFD, 0xFFEC, 0x0001, 0xFFF0, 0xFFFF, 0xFFF6, 0x0004, 0xFFF6,
      0x0005, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x4221, 0xFA94, 0x0118, 0xFDF4, 0xFFE2, 0xFF6B, 0xFFD7, 0xFF9E,
      0xFFD4, 0xFFB5, 0xFFD7, 0xFFC8, 0xFFE0, 0xFFD6, 0xFFEB, 0xFFE0,
      0xFFF4, 0xFFE9, 0xFFF8, 0xFFEE, 0xFFF9, 0xFFF4, 0xFFFB, 0xFFF5,
      0xFFFC, 0xFFF7, 0xFFF8, 0xFFF8, 0xFFFB, 0xFFF8, 0xFFFC, 0xFFF9,
      0xFFFC, 0xFFF9, 0xFFFE, 0xFFFB, 0xFFFE, 0xFFFC, 0x0000, 0xFFFE,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x51E9,           /* asrc_int_scale */
   0x09E8,           /* asrc_dec_scale */
   1,               /* vtx_pg */
   1,               /* vrx_pg */
   0,               /* hpf_en */
   6,               /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_SLOVAKIA_3239_H */
