/***************************************************************************
*    Copyright (c) 2000-2013 Broadcom                             
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
*       BCM6838                          
*       SLIC = Silicon Labs Si32392      
*       R1 = 900 ohms
*       R2 = 1E+09 ohms
*       C1 = 2160 nano Farads 
*       DLP = -3dB
*       ELP = -3dB
*       HWDACgain = 0dB
*       HW_impedance = 680 ohms
*       Protection resistor = 10 ohms
*       Ringing frequency = 20 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.8
*
****************************************************************************/

#ifndef FLEXICALC_TR57_32392_H
#define FLEXICALC_TR57_32392_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_TR57
#define flexicalcTR57Archive32392 ((const APM6838_FLEXICALC_CFG*) NULL)
#else
const APM6838_FLEXICALC_CFG flexicalcTR57Archive32392[] =
{
{
   0x32392,          /* Slic type: Si32392 */
   45,               /* Ring Voltage (RMS) */
   -3,               /* DLP - Decode Level Point (receive loss) in dB */
   -3,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0007,           /* eq_rx_shft */
   0x000A,           /* eq_tx_shft */
   0,                /* eq_imp_resp */

   /*
   ** Y-filter Coefficients
   */
   1,                /* yfltr_en */
   {  /* IIR 2 Filter Coefficients */
      0x04966,   /* Y IIR2 filter b0 */
      0x061FA,   /* Y IIR2 filter b1 */
      0x04966,   /* Y IIR2 filter b2 */
      0x4E1DC,   /* Y IIR2 filter a1 */
      0xE28AB    /* Y IIR2 filter a2 */
   },
   0x77,             /* y_iir2_filter_shift */
   {  /* Fir Filter Coefficients */
      0x5E40F,   /* YFIR1_VAL */
      0x8A746,   /* YFIR2_VAL */
      0xEA316,   /* YFIR3_VAL */
      0x11CA6,   /* YFIR4_VAL */
      0x13527,   /* YFIR5_VAL */
      0x07F9A,   /* YFIR6_VAL */
      0x002C7,   /* YFIR7_VAL */
      0x01612,   /* YFIR8_VAL */
      0x06079,   /* YFIR9_VAL */
      0x06D13,   /* YFIR10_VAL */
      0x019E0,   /* YFIR11_VAL */
      0xFA0AD,   /* YFIR12_VAL */
      0xF532C,   /* YFIR13_VAL */
      0xF5752,   /* YFIR14_VAL */
      0xFAEB6,   /* YFIR15_VAL */
      0x062A1,   /* YFIR16_VAL */
      0x1A45F,   /* YFIR17_VAL */
      0xF03BB    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x7F4C7},          /* y_iir1_filter[1] */
   0x86,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0x9DE5, 0x7CA5, 0x8B4A, 0x7CD7, 0xC5F1},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7AAD, 0xFCFE, 0xF83E, 0xFB8C, 0xF9E3, 0xFBDD, 0xFB57, 0xFC7B,
      0xFC71, 0xFD37, 0xFD4A, 0xFDD7, 0xFE04, 0xFE61, 0xFE94, 0xFEDE,
      0xFEFC, 0xFF31, 0xFF4C, 0xFF71, 0xFF70, 0xFF92, 0xFF91, 0xFFA5,
      0xFF9E, 0xFFB0, 0xFFA4, 0xFFAE, 0xFFAE, 0xFFB4, 0xFFB1, 0xFFBE,
      0xFFC7, 0xFFD1, 0xFFD6, 0xFFE7, 0xFFE8, 0xFFF6, 0xFFFB, 0x0005,
      0x0003, 0x0012, 0x0010, 0x000E, 0x000C, 0x000A, 0x0009, 0x0008,
      0x0007, 0x0006, 0x0005, 0x0004, 0x0004, 0x0003, 0x0003, 0x0002,
      0x0002, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x7C81, 0x0102, 0xFBFB, 0xFEDA, 0xFCE7, 0xFE77, 0xFDAF, 0xFE92,
      0xFE31, 0xFEC7, 0xFE9C, 0xFF01, 0xFEF5, 0xFF39, 0xFF3C, 0xFF69,
      0xFF73, 0xFF8F, 0xFF96, 0xFFAE, 0xFFAC, 0xFFBE, 0xFFB7, 0xFFCA,
      0xFFBE, 0xFFCE, 0xFFC6, 0xFFD2, 0xFFCC, 0xFFD6, 0xFFD4, 0xFFDB,
      0xFFDC, 0xFFE3, 0xFFE5, 0xFFEC, 0xFFED, 0xFFF6, 0xFFF5, 0xFFFF,
      0xFFFB, 0x0003, 0x0003, 0x0003, 0x0002, 0x0002, 0x0002, 0x0002,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0003,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x43DE,           /* asrc_int_scale */
   0x0EF7,           /* asrc_dec_scale */
   0,                /* vtx_pg */
   0,                /* vrx_pg */
   1,                /* hpf_en */
   6,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_TR57_32392_H */
