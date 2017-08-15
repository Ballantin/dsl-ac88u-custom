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
*       R1 = 270 ohms
*       R2 = 750 ohms
*       C1 = 150 nano Farads 
*       DLP = -11dB
*       ELP = -4dB
*       HWDACgain = 0dB
*       HW_impedance = 680 ohms
*       Protection resistor = 10 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.7
*
****************************************************************************/

#ifndef FLEXICALC_DENMARK_32392_H
#define FLEXICALC_DENMARK_32392_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_DENMARK
#define flexicalcDENMARKArchive32392 ((const APM6828_FLEXICALC_CFG*) NULL)
#else
const APM6828_FLEXICALC_CFG flexicalcDENMARKArchive32392[] =
{
{
   0x32392,          /* Slic type: Si32392 */
   45,               /* Ring Voltage (RMS) */
   -11,               /* DLP - Decode Level Point (receive loss) in dB */
   -4,               /* ELP - Encode Level Point (transmitt loss) in dB */
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
      0x48FFB,   /* YFIR1_VAL */
      0x85767,   /* YFIR2_VAL */
      0x12E46,   /* YFIR3_VAL */
      0x2C908,   /* YFIR4_VAL */
      0x030E1,   /* YFIR5_VAL */
      0xE30E7,   /* YFIR6_VAL */
      0xED3EB,   /* YFIR7_VAL */
      0x096E1,   /* YFIR8_VAL */
      0x12CE6,   /* YFIR9_VAL */
      0x03A4E,   /* YFIR10_VAL */
      0xF4252,   /* YFIR11_VAL */
      0xF8035,   /* YFIR12_VAL */
      0x07563,   /* YFIR13_VAL */
      0x0BA8A,   /* YFIR14_VAL */
      0xFF87D,   /* YFIR15_VAL */
      0xF7705,   /* YFIR16_VAL */
      0x065F0,   /* YFIR17_VAL */
      0xFAB48    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x35FD4},          /* y_iir1_filter[1] */
   0xA6,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0x412C, 0x993D, 0x6414, 0x832A, 0x4AE3},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x55F8, 0xD693, 0x07BD, 0xFAAC, 0x01B3, 0xFE12, 0x0059, 0xFEDA,
      0x0006, 0xFF2E, 0xFFEB, 0xFF6B, 0xFFE8, 0xFF92, 0xFFEE, 0xFFB1,
      0xFFF5, 0xFFC4, 0xFFFE, 0xFFD2, 0xFFFD, 0xFFDF, 0x0003, 0xFFE0,
      0xFFFD, 0xFFE7, 0xFFFC, 0xFFE9, 0xFFFF, 0xFFED, 0xFFFA, 0xFFF0,
      0xFFFC, 0xFFED, 0xFFFF, 0xFFF1, 0xFFFF, 0xFFF6, 0x0003, 0xFFF7,
      0x0004, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x4209, 0xFB11, 0x00D0, 0xFE43, 0xFFE3, 0xFF62, 0xFFD1, 0xFF96,
      0xFFCF, 0xFFB0, 0xFFD3, 0xFFC4, 0xFFDC, 0xFFD3, 0xFFE7, 0xFFDF,
      0xFFF0, 0xFFE8, 0xFFF6, 0xFFED, 0xFFF7, 0xFFF3, 0xFFFA, 0xFFF4,
      0xFFFA, 0xFFF6, 0xFFF8, 0xFFF7, 0xFFFA, 0xFFF7, 0xFFFB, 0xFFF8,
      0xFFFB, 0xFFF9, 0xFFFD, 0xFFFA, 0xFFFE, 0xFFFC, 0x0000, 0xFFFE,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x6120,           /* asrc_int_scale */
   0x0A35,           /* asrc_dec_scale */
   1,                /* vtx_pg */
   1,                /* vrx_pg */
   0,                /* hpf_en */
   6,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_DENMARK_32392_H */
