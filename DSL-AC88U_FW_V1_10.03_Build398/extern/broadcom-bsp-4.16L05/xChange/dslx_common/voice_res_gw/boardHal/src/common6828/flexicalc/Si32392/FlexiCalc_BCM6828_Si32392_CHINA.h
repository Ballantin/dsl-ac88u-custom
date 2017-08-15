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
*       R1 = 200 ohms
*       R2 = 680 ohms
*       C1 = 100 nano Farads 
*       DLP = -7dB
*       ELP = 0dB
*       HWDACgain = 0dB
*       HW_impedance = 680 ohms
*       Protection resistor = 10 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.7
*
****************************************************************************/

#ifndef FLEXICALC_CHINA_32392_H
#define FLEXICALC_CHINA_32392_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_CHINA
#define flexicalcCHINAArchive32392 ((const APM6828_FLEXICALC_CFG*) NULL)
#else
const APM6828_FLEXICALC_CFG flexicalcCHINAArchive32392[] =
{
{
   0x32392,          /* Slic type: Si32392 */
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
      0x04966,   /* Y IIR2 filter b0 */
      0x061FA,   /* Y IIR2 filter b1 */
      0x04966,   /* Y IIR2 filter b2 */
      0x4E1DC,   /* Y IIR2 filter a1 */
      0xE28AB    /* Y IIR2 filter a2 */
   },
   0x77,             /* y_iir2_filter_shift */
   {  /* Fir Filter Coefficients */
      0x2F6F5,   /* YFIR1_VAL */
      0x9081B,   /* YFIR2_VAL */
      0x358B3,   /* YFIR3_VAL */
      0x24DBC,   /* YFIR4_VAL */
      0xF1337,   /* YFIR5_VAL */
      0xE5286,   /* YFIR6_VAL */
      0xFD2EA,   /* YFIR7_VAL */
      0x11575,   /* YFIR8_VAL */
      0x0C606,   /* YFIR9_VAL */
      0xFB846,   /* YFIR10_VAL */
      0xF550C,   /* YFIR11_VAL */
      0xFD9A1,   /* YFIR12_VAL */
      0x0639C,   /* YFIR13_VAL */
      0x04F8B,   /* YFIR14_VAL */
      0x00027,   /* YFIR15_VAL */
      0xFEBDA,   /* YFIR16_VAL */
      0xF272F,   /* YFIR17_VAL */
      0x09710    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x7DEC5},          /* y_iir1_filter[1] */
   0xA6,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0x4ADF, 0x8332, 0x74FD, 0x832D, 0x49BB},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x4F71, 0xE40C, 0x0718, 0xFB25, 0x012E, 0xFE08, 0x0012, 0xFEC0,
      0xFFD3, 0xFF17, 0xFFC4, 0xFF59, 0xFFC9, 0xFF81, 0xFFD6, 0xFFA6,
      0xFFE4, 0xFFBD, 0xFFF1, 0xFFCC, 0xFFF3, 0xFFDB, 0xFFFC, 0xFFDB,
      0xFFF6, 0xFFE4, 0xFFF6, 0xFFE3, 0xFFF8, 0xFFE8, 0xFFF3, 0xFFEC,
      0xFFF7, 0xFFEB, 0xFFFC, 0xFFF0, 0xFFFC, 0xFFF5, 0x0002, 0xFFF7,
      0x0003, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000,
      0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x4202, 0xFB6F, 0x013A, 0xFE7B, 0xFFD5, 0xFF53, 0xFFB4, 0xFF81,
      0xFFB7, 0xFF9E, 0xFFC0, 0xFFB6, 0xFFCF, 0xFFC8, 0xFFDD, 0xFFD6,
      0xFFE9, 0xFFE2, 0xFFF0, 0xFFE9, 0xFFF3, 0xFFEF, 0xFFF5, 0xFFF0,
      0xFFF6, 0xFFF3, 0xFFF5, 0xFFF4, 0xFFF7, 0xFFF4, 0xFFF8, 0xFFF6,
      0xFFF9, 0xFFF7, 0xFFFC, 0xFFF9, 0xFFFD, 0xFFFC, 0xFFFF, 0xFFFE,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x4E43,           /* asrc_int_scale */
   0x0C56,           /* asrc_dec_scale */
   2,                /* vtx_pg */
   1,                /* vrx_pg */
   0,                /* hpf_en */
   5,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_CHINA_32392_H */
