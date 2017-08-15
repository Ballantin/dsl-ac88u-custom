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
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_TR57_3239_H
#define FLEXICALC_TR57_3239_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_TR57
#define flexicalcTR57Archive3239 ((const APM6818_FLEXICALC_CFG*) NULL)
#else
const APM6818_FLEXICALC_CFG flexicalcTR57Archive3239[] =
{
{
   0x3239,           /* Slic type: Si3239 */
   45,               /* Ring Voltage (RMS) */
   -3,               /* DLP - Decode Level Point (receive loss) in dB */
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
      0xB4903,   /* YFIR1_VAL */
      0x5F25C,   /* YFIR2_VAL */
      0x11418,   /* YFIR3_VAL */
      0xF15D4,   /* YFIR4_VAL */
      0xF018A,   /* YFIR5_VAL */
      0xF9541,   /* YFIR6_VAL */
      0xFFAC0,   /* YFIR7_VAL */
      0xFECE4,   /* YFIR8_VAL */
      0xFB335,   /* YFIR9_VAL */
      0xFAACD,   /* YFIR10_VAL */
      0xFEE13,   /* YFIR11_VAL */
      0x04F8C,   /* YFIR12_VAL */
      0x08F41,   /* YFIR13_VAL */
      0x08E73,   /* YFIR14_VAL */
      0x0471D,   /* YFIR15_VAL */
      0xFA9EA,   /* YFIR16_VAL */
      0xE889E,   /* YFIR17_VAL */
      0x0E62F    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x7F731},          /* y_iir1_filter[1] */
   0x86,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,               /* hybal_shft */
   {0x6630, 0x8E45, 0x6BE8, 0x8326, 0x3B77},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7B1A, 0xFD82, 0xF883, 0xFC2C, 0xFA4B, 0xFC4B, 0xFBA6, 0xFCC7,
      0xFCA9, 0xFD6A, 0xFD70, 0xFDF8, 0xFE1C, 0xFE76, 0xFEA3, 0xFEEB,
      0xFF05, 0xFF38, 0xFF50, 0xFF76, 0xFF73, 0xFF95, 0xFF93, 0xFFA7,
      0xFFA0, 0xFFB2, 0xFFA6, 0xFFB0, 0xFFB0, 0xFFB6, 0xFFB3, 0xFFC0,
      0xFFC8, 0xFFD1, 0xFFD5, 0xFFE6, 0xFFE6, 0xFFF4, 0xFFF9, 0x0003,
      0x0001, 0x000F, 0x000E, 0x000C, 0x000A, 0x0009, 0x0008, 0x0007,
      0x0006, 0x0005, 0x0004, 0x0004, 0x0003, 0x0003, 0x0003, 0x0002,
      0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x7CFB, 0x015D, 0xFC40, 0xFF74, 0xFD51, 0xFEE9, 0xFE05, 0xFEE6,
      0xFE74, 0xFF06, 0xFECF, 0xFF2E, 0xFF19, 0xFF5A, 0xFF56, 0xFF81,
      0xFF85, 0xFFA0, 0xFFA3, 0xFFBA, 0xFFB6, 0xFFC8, 0xFFBF, 0xFFD2,
      0xFFC5, 0xFFD5, 0xFFCC, 0xFFD8, 0xFFD2, 0xFFDC, 0xFFD9, 0xFFE0,
      0xFFE0, 0xFFE8, 0xFFE7, 0xFFEF, 0xFFEF, 0xFFF7, 0xFFF6, 0xFFFF,
      0xFFFB, 0x0002, 0x0002, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x405F,           /* asrc_int_scale */
   0x0946,           /* asrc_dec_scale */
   2,               /* vtx_pg */
   0,               /* vrx_pg */
   1,               /* hpf_en */
   6,               /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_TR57_3239_H */
