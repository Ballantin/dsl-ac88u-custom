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
*       SLIC = Silicon Labs Si32392      
*       R1 = 600 ohms
*       R2 = 0 ohms
*       C1 = 0 nano Farads 
*       DLP = -7dB
*       ELP = -7dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 57Vrms
*
*    Flexicalc Version: 3.7
*
****************************************************************************/

#ifndef FLEXICALC_UNITED_ARAB_EMIRATES_32392_H
#define FLEXICALC_UNITED_ARAB_EMIRATES_32392_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_UNITED_ARAB_EMIRATES
#define flexicalcUNITED_ARAB_EMIRATESArchive32392 ((const APM6828_FLEXICALC_CFG*) NULL)
#else
const APM6828_FLEXICALC_CFG flexicalcUNITED_ARAB_EMIRATESArchive32392[] =
{
{
   0x32392,          /* Slic type: Si32392 */
   57,               /* Ring Voltage (RMS) */
   -7,               /* DLP - Decode Level Point (receive loss) in dB */
   -7,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0008,           /* eq_rx_shft */
   0x000A,           /* eq_tx_shft */
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
      0x4D4A8,   /* YFIR1_VAL */
      0xF8765,   /* YFIR2_VAL */
      0x833E8,   /* YFIR3_VAL */
      0xBEE50,   /* YFIR4_VAL */
      0x4A46E,   /* YFIR5_VAL */
      0x6E948,   /* YFIR6_VAL */
      0x1BB24,   /* YFIR7_VAL */
      0xCDA97,   /* YFIR8_VAL */
      0xB9AD7,   /* YFIR9_VAL */
      0xD22A2,   /* YFIR10_VAL */
      0x0DF80,   /* YFIR11_VAL */
      0x4E542,   /* YFIR12_VAL */
      0x3FC86,   /* YFIR13_VAL */
      0xD88D1,   /* YFIR14_VAL */
      0xA634C,   /* YFIR15_VAL */
      0xF725D,   /* YFIR16_VAL */
      0x5EDBA,   /* YFIR17_VAL */
      0xD9746    /* YFIR18_VAL */
   },
   0x04,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x7F93B},          /* y_iir1_filter[1] */
   0x86,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   4,                  /* hybal_shft */
   {0x4009, 0xCCAC, 0x0968, 0x1E2C, 0xEB4D},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7ED0, 0xFDF0, 0xFDE2, 0xFDF5, 0xFE03, 0xFE23, 0xFE36, 0xFE53,
      0xFE78, 0xFE99, 0xFEB5, 0xFEDA, 0xFEF9, 0xFF17, 0xFF38, 0xFF55,
      0xFF6B, 0xFF80, 0xFF97, 0xFFA4, 0xFFAB, 0xFFB7, 0xFFBF, 0xFFC2,
      0xFFC6, 0xFFCB, 0xFFCA, 0xFFCA, 0xFFD0, 0xFFCF, 0xFFCF, 0xFFD5,
      0xFFDA, 0xFFDC, 0xFFE1, 0xFFE8, 0xFFEA, 0xFFF0, 0xFFF6, 0xFFF9,
      0xFFFB, 0x0004, 0x0003, 0x0003, 0x0003, 0x0002, 0x0002, 0x0002,
      0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x7F5F, 0xFEEB, 0xFEDB, 0xFEEB, 0xFEEF, 0xFF00, 0xFF0B, 0xFF20,
      0xFF2F, 0xFF44, 0xFF54, 0xFF69, 0xFF7A, 0xFF8B, 0xFF9C, 0xFFA9,
      0xFFB8, 0xFFC0, 0xFFCB, 0xFFD2, 0xFFD5, 0xFFDA, 0xFFDB, 0xFFDF,
      0xFFDF, 0xFFE1, 0xFFE2, 0xFFE5, 0xFFE5, 0xFFE7, 0xFFE9, 0xFFEA,
      0xFFED, 0xFFEE, 0xFFF1, 0xFFF3, 0xFFF6, 0xFFF8, 0xFFFA, 0xFFFD,
      0xFFFD, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x6B7E,           /* asrc_int_scale */
   0x0ADF,           /* asrc_dec_scale */
   1,                /* vtx_pg */
   1,                /* vrx_pg */
   0,                /* hpf_en */
   4,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_UNITED_ARAB_EMIRATES_32392_H */
