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
*     FlexiCalc output constants for the following inputs:
*
*       BCM6816                          
*       SLIC = Silicon Labs Si3239       
*       R1 = 200 ohms
*       R2 = 680 ohms
*       C1 = 100 nano Farads 
*       DLP =-4dB
*       ELP =-2dB
*       HWDACgain =0dB
*       HW_impedance =680 ohms
*       Protection resistor =10 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_CHINA_3239_H
#define FLEXICALC_CHINA_3239_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_CHINA
#define flexicalcCHINAArchive3239 ((const APM6816_FLEXICALC_CFG*) NULL)
#else
const APM6816_FLEXICALC_CFG flexicalcCHINAArchive3239[] =
{
{
   0x3239,               /* Slic type: Si3239 */
   45,               /* Ring Voltage (RMS) */
   -4,                   /* DLP - Decode Level Point (receive loss) in dB */
   -2,                   /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0007,               /* eq_rx_shft */
   0x000B,           /* eq_tx_shft */
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
      0x2CE4C,   /* YFIR1_VAL */
      0x93808,   /* YFIR2_VAL */
      0x36851,   /* YFIR3_VAL */
      0x23A46,   /* YFIR4_VAL */
      0xF061E,   /* YFIR5_VAL */
      0xE590C,   /* YFIR6_VAL */
      0xFDFDD,   /* YFIR7_VAL */
      0x11772,   /* YFIR8_VAL */
      0x0BE15,   /* YFIR9_VAL */
      0xFB363,   /* YFIR10_VAL */
      0xF591A,   /* YFIR11_VAL */
      0xFDE81,   /* YFIR12_VAL */
      0x06087,   /* YFIR13_VAL */
      0x0490F,   /* YFIR14_VAL */
      0x00206,   /* YFIR15_VAL */
      0xFF2CF,   /* YFIR16_VAL */
      0xF16F9,   /* YFIR17_VAL */
      0x0A1CE    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x7FAC7},          /* y_iir1_filter[1] */
   0x96,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0x6010, 0x8321, 0x5532, 0x8321, 0x638C},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x4FB3, 0xE3A6, 0x0754, 0xFB0E, 0x0147, 0xFE0F, 0x001D, 0xFEC6,
      0xFFD9, 0xFF1A, 0xFFC8, 0xFF5B, 0xFFCC, 0xFF82, 0xFFD8, 0xFFA7,
      0xFFE6, 0xFFBD, 0xFFF2, 0xFFCC, 0xFFF4, 0xFFDC, 0xFFFC, 0xFFDC,
      0xFFF7, 0xFFE4, 0xFFF7, 0xFFE4, 0xFFF9, 0xFFE9, 0xFFF4, 0xFFEC,
      0xFFF8, 0xFFEB, 0xFFFC, 0xFFF0, 0xFFFC, 0xFFF5, 0x0002, 0xFFF7,
      0x0003, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x4243, 0xFB19, 0x015B, 0xFE71, 0xFFE5, 0xFF5E, 0xFFBE, 0xFF89,
      0xFFBE, 0xFFA3, 0xFFC5, 0xFFB9, 0xFFD2, 0xFFCA, 0xFFDF, 0xFFD7,
      0xFFEB, 0xFFE3, 0xFFF2, 0xFFEA, 0xFFF4, 0xFFF0, 0xFFF6, 0xFFF0,
      0xFFF7, 0xFFF3, 0xFFF6, 0xFFF5, 0xFFF7, 0xFFF5, 0xFFF9, 0xFFF6,
      0xFFFA, 0xFFF7, 0xFFFC, 0xFFF9, 0xFFFD, 0xFFFC, 0xFFFF, 0xFFFE,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x6CCE,           /* asrc_int_scale */
   0x09DE,           /* asrc_dec_scale */
   2,               /* vtx_pg */
   0,               /* vrx_pg */
   0,                /* hpf_en */
   5,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_CHINA_3239_H */
