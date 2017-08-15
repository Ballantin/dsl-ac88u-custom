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
*       R1 = 200 ohms
*       R2 = 680 ohms
*       C1 = 100 nano Farads 
*       DLP = -7dB
*       ELP = 0dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.9
*
****************************************************************************/

#ifndef FLEXICALC_CHINA_9540_H
#define FLEXICALC_CHINA_9540_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_CHINA
#define flexicalcCHINAArchive9540 ((const APM6838_FLEXICALC_CFG*) NULL)
#else
const APM6838_FLEXICALC_CFG flexicalcCHINAArchive9540[] =
{
{
   0x9540,           /* Slic type: L9540 */
   45,               /* Ring Voltage (RMS) */
   -7,               /* DLP - Decode Level Point (receive loss) in dB */
   0,               /* ELP - Encode Level Point (transmitt loss) in dB */
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
      0x3633C,   /* YFIR1_VAL */
      0x99084,   /* YFIR2_VAL */
      0x21E58,   /* YFIR3_VAL */
      0x23B97,   /* YFIR4_VAL */
      0xF8452,   /* YFIR5_VAL */
      0xE6CA3,   /* YFIR6_VAL */
      0xF8E08,   /* YFIR7_VAL */
      0x0E856,   /* YFIR8_VAL */
      0x0D42B,   /* YFIR9_VAL */
      0xFBD24,   /* YFIR10_VAL */
      0xF2EAA,   /* YFIR11_VAL */
      0xFCE1F,   /* YFIR12_VAL */
      0x0AB41,   /* YFIR13_VAL */
      0x088EC,   /* YFIR14_VAL */
      0xF8EF5,   /* YFIR15_VAL */
      0xF4C97,   /* YFIR16_VAL */
      0x0E866,   /* YFIR17_VAL */
      0xF9C53    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x5D47C},          /* y_iir1_filter[1] */
   0xA6,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   5,                  /* hybal_shft */
   {0xBDBF, 0xF4C0, 0x3470, 0xBE4A, 0x3FB0},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7AD1, 0x0444, 0xFA49, 0x008A, 0xFBE7, 0xFF09, 0xFD40, 0xFEAD,
      0xFDE6, 0xFEBB, 0xFE5C, 0xFEDF, 0xFEC1, 0xFF12, 0xFF0E, 0xFF50,
      0xFF4B, 0xFF7D, 0xFF79, 0xFFA4, 0xFF91, 0xFFBB, 0xFFA7, 0xFFC6,
      0xFFB3, 0xFFCD, 0xFFBA, 0xFFCC, 0xFFC3, 0xFFCD, 0xFFC8, 0xFFD1,
      0xFFD3, 0xFFDE, 0xFFD9, 0xFFEB, 0xFFE4, 0xFFF3, 0xFFF0, 0xFFFE,
      0xFFF8, 0x0004, 0x0005, 0x0004, 0x0004, 0x0003, 0x0003, 0x0002,
      0x0002, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x6610, 0x2084, 0xFF55, 0x0191, 0xFD5F, 0xFF73, 0xFE29, 0xFF3A,
      0xFE91, 0xFF40, 0xFEEA, 0xFF57, 0xFF27, 0xFF79, 0xFF5E, 0xFF98,
      0xFF86, 0xFFB5, 0xFF9E, 0xFFC7, 0xFFB2, 0xFFD5, 0xFFC1, 0xFFDA,
      0xFFC9, 0xFFDC, 0xFFCD, 0xFFDC, 0xFFD1, 0xFFDF, 0xFFD9, 0xFFE4,
      0xFFDF, 0xFFED, 0xFFE5, 0xFFF2, 0xFFED, 0xFFFA, 0xFFF3, 0x0001,
      0xFFFA, 0x0000, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0003,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x67EC,           /* asrc_int_scale */
   0x0C81,           /* asrc_dec_scale */
   0,                /* vtx_pg */
   1,                /* vrx_pg */
   0,                /* hpf_en */
   4,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_CHINA_9540_H */
