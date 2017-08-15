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
*       BCM6828                          
*       SLIC = Silicon Labs Si3239       
*       R1 = 472 ohms
*       R2 = 637 ohms
*       C1 = 219 nano Farads 
*       DLP = -10dB
*       ELP = -4dB
*       HWDACgain = 0dB
*       HW_impedance = 680 ohms
*       Protection resistor = 10 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_NETHERLANDS_3239_H
#define FLEXICALC_NETHERLANDS_3239_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_NETHERLANDS
#define flexicalcNETHERLANDSArchive3239 ((const APM6828_FLEXICALC_CFG*) NULL)
#else
const APM6828_FLEXICALC_CFG flexicalcNETHERLANDSArchive3239[] =
{
{
   0x3239,           /* Slic type: Si3239 */
   45,               /* Ring Voltage (RMS) */
   -10,               /* DLP - Decode Level Point (receive loss) in dB */
   -4,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0007,           /* eq_rx_shft */
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
      0x5286D,   /* YFIR1_VAL */
      0x8B03B,   /* YFIR2_VAL */
      0xF44E8,   /* YFIR3_VAL */
      0x156C7,   /* YFIR4_VAL */
      0x0E93B,   /* YFIR5_VAL */
      0xFAB64,   /* YFIR6_VAL */
      0xF34AF,   /* YFIR7_VAL */
      0xFE147,   /* YFIR8_VAL */
      0x0C9A6,   /* YFIR9_VAL */
      0x0F88B,   /* YFIR10_VAL */
      0x05250,   /* YFIR11_VAL */
      0xF9162,   /* YFIR12_VAL */
      0xF6FE4,   /* YFIR13_VAL */
      0xFE858,   /* YFIR14_VAL */
      0x04687,   /* YFIR15_VAL */
      0xFEC10,   /* YFIR16_VAL */
      0xF0B3A,   /* YFIR17_VAL */
      0x0B88F    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x57B89},          /* y_iir1_filter[1] */
   0x86,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,               /* hybal_shft */
   {0x3AE5, 0xA323, 0x64DE, 0x832A, 0x4661},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x4EE5, 0xE379, 0xFF04, 0xFDBD, 0x002F, 0xFED1, 0xFFC5, 0xFF2C,
      0xFFB3, 0xFF5E, 0xFFB0, 0xFF84, 0xFFBE, 0xFFA6, 0xFFCF, 0xFFBC,
      0xFFD9, 0xFFCD, 0xFFEA, 0xFFDC, 0xFFEC, 0xFFE1, 0xFFF3, 0xFFE7,
      0xFFF4, 0xFFE9, 0xFFF3, 0xFFED, 0xFFF6, 0xFFEE, 0xFFF5, 0xFFF2,
      0xFFF7, 0xFFF1, 0xFFF9, 0xFFF5, 0xFFFB, 0xFFF9, 0x0000, 0xFFFB,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x40E2, 0xFDA9, 0xFFBB, 0xFF0F, 0xFFDC, 0xFF6F, 0xFFC2, 0xFF92,
      0xFFC2, 0xFFA9, 0xFFC8, 0xFFBE, 0xFFD3, 0xFFCE, 0xFFDE, 0xFFDC,
      0xFFE8, 0xFFE6, 0xFFEF, 0xFFED, 0xFFF2, 0xFFF0, 0xFFF5, 0xFFF2,
      0xFFF6, 0xFFF3, 0xFFF6, 0xFFF5, 0xFFF7, 0xFFF6, 0xFFF8, 0xFFF7,
      0xFFF9, 0xFFF8, 0xFFFB, 0xFFFA, 0xFFFD, 0xFFFC, 0xFFFF, 0xFFFE,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x6F8B,           /* asrc_int_scale */
   0x0AB0,           /* asrc_dec_scale */
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

#endif  /* FLEXICALC_NETHERLANDS_3239_H */
