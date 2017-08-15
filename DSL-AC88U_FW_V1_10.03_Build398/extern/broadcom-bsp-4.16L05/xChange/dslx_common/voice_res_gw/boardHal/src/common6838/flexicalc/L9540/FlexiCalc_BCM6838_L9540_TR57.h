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
*       SLIC = Zarlink L9540            
*       R1 = 900 ohms
*       R2 = 1E+09 ohms
*       C1 = 2160 nano Farads 
*       DLP = -3dB
*       ELP = -3dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 20 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.8
*
****************************************************************************/

#ifndef FLEXICALC_TR57_9540_H
#define FLEXICALC_TR57_9540_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_TR57
#define flexicalcTR57Archive9540 ((const APM6838_FLEXICALC_CFG*) NULL)
#else
const APM6838_FLEXICALC_CFG flexicalcTR57Archive9540[] =
{
{
   0x9540,           /* Slic type: L9540 */
   45,               /* Ring Voltage (RMS) */
   -3,               /* DLP - Decode Level Point (receive loss) in dB */
   -3,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0007,           /* eq_rx_shft */
   0x000B,           /* eq_tx_shft */
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
      0xC9DD8,   /* YFIR1_VAL */
      0x4B146,   /* YFIR2_VAL */
      0xF918F,   /* YFIR3_VAL */
      0xE712E,   /* YFIR4_VAL */
      0xFAF55,   /* YFIR5_VAL */
      0x0DF5C,   /* YFIR6_VAL */
      0x0C220,   /* YFIR7_VAL */
      0xFD6AA,   /* YFIR8_VAL */
      0xF4BC2,   /* YFIR9_VAL */
      0xF9ED9,   /* YFIR10_VAL */
      0x0452F,   /* YFIR11_VAL */
      0x072CE,   /* YFIR12_VAL */
      0x002AF,   /* YFIR13_VAL */
      0xF8F8F,   /* YFIR14_VAL */
      0xFB261,   /* YFIR15_VAL */
      0x045AE,   /* YFIR16_VAL */
      0x06651,   /* YFIR17_VAL */
      0xFBF16    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x4B7C3},          /* y_iir1_filter[1] */
   0x86,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   6,                  /* hybal_shft */
   {0xBE84, 0x3CA5, 0x0B27, 0xB109, 0x2E12},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7F14, 0xF8C7, 0xFF9F, 0xFA6B, 0xFCF9, 0xFC1C, 0xFD26, 0xFCEA,
      0xFDA0, 0xFD99, 0xFE13, 0xFE29, 0xFE87, 0xFE9A, 0xFEEB, 0xFF03,
      0xFF39, 0xFF47, 0xFF78, 0xFF7B, 0xFF93, 0xFF98, 0xFFB0, 0xFFA7,
      0xFFB6, 0xFFB5, 0xFFBA, 0xFFB1, 0xFFC1, 0xFFBA, 0xFFBD, 0xFFC3,
      0xFFD0, 0xFFCF, 0xFFDD, 0xFFE1, 0xFFE9, 0xFFED, 0xFFFB, 0xFFF9,
      0x0000, 0x000A, 0x0008, 0x0007, 0x0007, 0x0006, 0x0005, 0x0004,
      0x0004, 0x0003, 0x0003, 0x0002, 0x0002, 0x0002, 0x0002, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x4062, 0xFE71, 0x019B, 0xFEED, 0x0001, 0xFF6F, 0xFFD0, 0xFF94,
      0xFFCB, 0xFFAC, 0xFFCF, 0xFFC0, 0xFFD8, 0xFFD0, 0xFFE3, 0xFFDC,
      0xFFED, 0xFFE7, 0xFFF3, 0xFFED, 0xFFF6, 0xFFF2, 0xFFF7, 0xFFF1,
      0xFFF8, 0xFFF4, 0xFFF7, 0xFFF5, 0xFFF8, 0xFFF6, 0xFFF9, 0xFFF7,
      0xFFFA, 0xFFF8, 0xFFFC, 0xFFFA, 0xFFFD, 0xFFFC, 0xFFFF, 0xFFFE,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0003,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x5396,           /* asrc_int_scale */
   0x11F9,           /* asrc_dec_scale */
   0,                /* vtx_pg */
   0,                /* vrx_pg */
   1,                /* hpf_en */
   4,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_TR57_9540_H */
