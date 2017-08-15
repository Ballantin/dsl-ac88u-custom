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
*       BCM6818                          
*       SLIC = Silicon Labs Si3239       
*       R1 = 600 ohms
*       R2 = 0 ohms
*       C1 = 0 nano Farads 
*       DLP = -7dB
*       ELP = 0dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.7
*
****************************************************************************/

#ifndef FLEXICALC_CYPRUS_3239_H
#define FLEXICALC_CYPRUS_3239_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_CYPRUS
#define flexicalcCYPRUSArchive3239 ((const APM6818_FLEXICALC_CFG*) NULL)
#else
const APM6818_FLEXICALC_CFG flexicalcCYPRUSArchive3239[] =
{
{
   0x3239,           /* Slic type: Si3239 */
   45,               /* Ring Voltage (RMS) */
   -7,               /* DLP - Decode Level Point (receive loss) in dB */
   0,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0008,           /* eq_rx_shft */
   0x000B,           /* eq_tx_shft */
   0,                /* eq_imp_resp */

   /*
   ** Y-filter Coefficients
   */
   1,                /* yfltr_en */
   {  /* IIR 2 Filter Coefficients */
      0x16860,   /* Y IIR2 filter b0 */
      0x28E0B,   /* Y IIR2 filter b1 */
      0x16860,   /* Y IIR2 filter b2 */
      0x4A22B,   /* Y IIR2 filter a1 */
      0xDFB13    /* Y IIR2 filter a2 */
   },
   0x66,             /* y_iir2_filter_shift */
   {  /* Fir Filter Coefficients */
      0x4B25D,   /* YFIR1_VAL */
      0xDC241,   /* YFIR2_VAL */
      0x9D050,   /* YFIR3_VAL */
      0xE4C5C,   /* YFIR4_VAL */
      0x45F27,   /* YFIR5_VAL */
      0x48AEF,   /* YFIR6_VAL */
      0x08B58,   /* YFIR7_VAL */
      0xD7D2E,   /* YFIR8_VAL */
      0xCBF47,   /* YFIR9_VAL */
      0xE37C7,   /* YFIR10_VAL */
      0x12ABB,   /* YFIR11_VAL */
      0x39625,   /* YFIR12_VAL */
      0x27F5D,   /* YFIR13_VAL */
      0xDFC2F,   /* YFIR14_VAL */
      0xBFB2F,   /* YFIR15_VAL */
      0xFCAE2,   /* YFIR16_VAL */
      0x458F7,   /* YFIR17_VAL */
      0xE2CEC    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x7F866},          /* y_iir1_filter[1] */
   0x76,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   4,                  /* hybal_shft */
   {0x1AF6, 0x0FE1, 0xDFFB, 0x43BD, 0xDB82},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7ED5, 0xFDF0, 0xFDE7, 0xFDF8, 0xFE06, 0xFE26, 0xFE38, 0xFE55,
      0xFE79, 0xFE9B, 0xFEB6, 0xFEDB, 0xFEFA, 0xFF18, 0xFF39, 0xFF56,
      0xFF6B, 0xFF80, 0xFF97, 0xFFA4, 0xFFAB, 0xFFB7, 0xFFC0, 0xFFC3,
      0xFFC6, 0xFFCB, 0xFFCA, 0xFFCB, 0xFFD0, 0xFFCF, 0xFFCF, 0xFFD5,
      0xFFDA, 0xFFDC, 0xFFE1, 0xFFE8, 0xFFEA, 0xFFF0, 0xFFF6, 0xFFF9,
      0xFFFB, 0x0004, 0x0003, 0x0003, 0x0003, 0x0002, 0x0002, 0x0002,
      0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x7F65, 0xFEEB, 0xFEDF, 0xFEEF, 0xFEF2, 0xFF03, 0xFF0E, 0xFF23,
      0xFF30, 0xFF46, 0xFF56, 0xFF6B, 0xFF7B, 0xFF8C, 0xFF9D, 0xFFAA,
      0xFFB8, 0xFFC0, 0xFFCB, 0xFFD2, 0xFFD6, 0xFFDA, 0xFFDC, 0xFFDF,
      0xFFDF, 0xFFE2, 0xFFE2, 0xFFE5, 0xFFE5, 0xFFE7, 0xFFE9, 0xFFEA,
      0xFFED, 0xFFEE, 0xFFF2, 0xFFF3, 0xFFF6, 0xFFF8, 0xFFFA, 0xFFFD,
      0xFFFD, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x6A37,           /* asrc_int_scale */
   0x0943,           /* asrc_dec_scale */
   2,                /* vtx_pg */
   1,                /* vrx_pg */
   0,                /* hpf_en */
   3,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_CYPRUS_3239_H */
