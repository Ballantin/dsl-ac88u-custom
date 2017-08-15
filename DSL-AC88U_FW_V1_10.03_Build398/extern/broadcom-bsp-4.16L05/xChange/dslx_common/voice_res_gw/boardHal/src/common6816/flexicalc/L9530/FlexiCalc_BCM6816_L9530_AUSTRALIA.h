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
*       SLIC = Legerity L9530            
*       R1 = 220 ohms
*       R2 = 820 ohms
*       C1 = 115 nano Farads 
*       DLP = -9dB
*       ELP = -3dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_AUSTRALIA_9530_H
#define FLEXICALC_AUSTRALIA_9530_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_AUSTRALIA
#define flexicalcAUSTRALIAArchive9530 ((const APM6816_FLEXICALC_CFG*) NULL)
#else
const APM6816_FLEXICALC_CFG flexicalcAUSTRALIAArchive9530[] =
{
{
   0x9530,           /* Slic type: L9530 */
   45,               /* Ring Voltage (RMS) */
   -9,               /* DLP - Decode Level Point (receive loss) in dB */
   -3,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0007,               /* eq_rx_shft */
   0x000A,               /* eq_tx_shft */
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
      0xCD1BA,   /* YFIR1_VAL */
      0x67A5D,   /* YFIR2_VAL */
      0xD9343,   /* YFIR3_VAL */
      0xDCA42,   /* YFIR4_VAL */
      0x09E1E,   /* YFIR5_VAL */
      0x1934E,   /* YFIR6_VAL */
      0x055FA,   /* YFIR7_VAL */
      0xF07B7,   /* YFIR8_VAL */
      0xF35EA,   /* YFIR9_VAL */
      0x050E1,   /* YFIR10_VAL */
      0x0CE9B,   /* YFIR11_VAL */
      0x0269C,   /* YFIR12_VAL */
      0xF55AB,   /* YFIR13_VAL */
      0xF8325,   /* YFIR14_VAL */
      0x07132,   /* YFIR15_VAL */
      0x0A637,   /* YFIR16_VAL */
      0xF3559,   /* YFIR17_VAL */
      0x05104    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x695B7},          /* y_iir1_filter[1] */
   0xA6,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   6,                  /* hybal_shft */
   {0x43AB, 0xBDEF, 0x33A9, 0xE1B7, 0xF50A},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7A78, 0x045E, 0xFA08, 0x00C8, 0xFB5C, 0xFF29, 0xFD1E, 0xFEB7,
      0xFDD5, 0xFEBF, 0xFE52, 0xFEE0, 0xFEBB, 0xFF11, 0xFF09, 0xFF50,
      0xFF47, 0xFF7E, 0xFF77, 0xFFA5, 0xFF8D, 0xFFBC, 0xFFA4, 0xFFC6,
      0xFFB1, 0xFFCE, 0xFFB8, 0xFFCD, 0xFFC1, 0xFFCE, 0xFFC8, 0xFFD1,
      0xFFD3, 0xFFDF, 0xFFD8, 0xFFEC, 0xFFE3, 0xFFF4, 0xFFEF, 0xFFFF,
      0xFFF8, 0x0004, 0x0005, 0x0004, 0x0004, 0x0003, 0x0003, 0x0002,
      0x0002, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x5F24, 0x2827, 0x04E8, 0x0315, 0xFDBF, 0xFF5C, 0xFE21, 0xFF17,
      0xFE83, 0xFF26, 0xFEDC, 0xFF43, 0xFF1B, 0xFF6C, 0xFF55, 0xFF8C,
      0xFF7F, 0xFFB0, 0xFF99, 0xFFBF, 0xFFAE, 0xFFD5, 0xFFC0, 0xFFD4,
      0xFFC7, 0xFFDA, 0xFFCC, 0xFFD8, 0xFFCE, 0xFFDA, 0xFFD5, 0xFFE1,
      0xFFDD, 0xFFEA, 0xFFE3, 0xFFF0, 0xFFEC, 0xFFF8, 0xFFF2, 0x0000,
      0xFFFA, 0x0000, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x4FEB,           /* asrc_int_scale */
   0x0F28,           /* asrc_dec_scale */
   1,               /* vtx_pg */
   1,               /* vrx_pg */
   0,               /* hpf_en */
   5,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_AUSTRALIA_9530_H */
