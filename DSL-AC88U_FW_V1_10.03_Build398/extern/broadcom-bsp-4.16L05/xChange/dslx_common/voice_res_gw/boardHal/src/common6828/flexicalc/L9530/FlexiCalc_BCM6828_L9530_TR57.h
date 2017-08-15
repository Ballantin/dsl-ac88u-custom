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
*       SLIC = Zarlink L9530            
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
*    Flexicalc Version: 3.7
*
****************************************************************************/

#ifndef FLEXICALC_TR57_9530_H
#define FLEXICALC_TR57_9530_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_TR57
#define flexicalcTR57Archive9530 ((const APM6828_FLEXICALC_CFG*) NULL)
#else
const APM6828_FLEXICALC_CFG flexicalcTR57Archive9530[] =
{
{
   0x9530,           /* Slic type: L9530 */
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
      0x4EB8A,   /* YFIR1_VAL */
      0x8BC40,   /* YFIR2_VAL */
      0x0FBC5,   /* YFIR3_VAL */
      0x26930,   /* YFIR4_VAL */
      0x0571D,   /* YFIR5_VAL */
      0xEA0DF,   /* YFIR6_VAL */
      0xEF407,   /* YFIR7_VAL */
      0x05CCF,   /* YFIR8_VAL */
      0x11278,   /* YFIR9_VAL */
      0x07D05,   /* YFIR10_VAL */
      0xF86AC,   /* YFIR11_VAL */
      0xF56FE,   /* YFIR12_VAL */
      0x008E1,   /* YFIR13_VAL */
      0x0A952,   /* YFIR14_VAL */
      0x065AE,   /* YFIR15_VAL */
      0xF8D44,   /* YFIR16_VAL */
      0xF6D0D,   /* YFIR17_VAL */
      0x066D8    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x57778},          /* y_iir1_filter[1] */
   0x86,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   6,                  /* hybal_shft */
   {0x6096, 0x9D3F, 0x01CD, 0x6255, 0xC309},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7F14, 0xF8CA, 0xFFA5, 0xFA6F, 0xFCF8, 0xFC1A, 0xFD25, 0xFCE8,
      0xFD9F, 0xFD98, 0xFE12, 0xFE28, 0xFE86, 0xFE9A, 0xFEEB, 0xFF03,
      0xFF38, 0xFF47, 0xFF78, 0xFF7B, 0xFF93, 0xFF98, 0xFFB0, 0xFFA7,
      0xFFB6, 0xFFB5, 0xFFBA, 0xFFB1, 0xFFC1, 0xFFBA, 0xFFBD, 0xFFC3,
      0xFFD0, 0xFFCF, 0xFFDD, 0xFFE1, 0xFFE9, 0xFFED, 0xFFFB, 0xFFF9,
      0x0000, 0x000A, 0x0008, 0x0008, 0x0007, 0x0006, 0x0005, 0x0004,
      0x0004, 0x0003, 0x0003, 0x0002, 0x0002, 0x0002, 0x0002, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x4062, 0xFE72, 0x019E, 0xFEEF, 0x0001, 0xFF6E, 0xFFD0, 0xFF93,
      0xFFCB, 0xFFAB, 0xFFCE, 0xFFC0, 0xFFD8, 0xFFD0, 0xFFE3, 0xFFDC,
      0xFFED, 0xFFE7, 0xFFF3, 0xFFED, 0xFFF5, 0xFFF2, 0xFFF7, 0xFFF1,
      0xFFF8, 0xFFF4, 0xFFF7, 0xFFF5, 0xFFF8, 0xFFF6, 0xFFF9, 0xFFF7,
      0xFFFA, 0xFFF8, 0xFFFC, 0xFFFA, 0xFFFD, 0xFFFC, 0xFFFF, 0xFFFE,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x4CBA,           /* asrc_int_scale */
   0x0AFB,           /* asrc_dec_scale */
   2,                /* vtx_pg */
   0,                /* vrx_pg */
   1,                /* hpf_en */
   4,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_TR57_9530_H */
