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
*       R1 = 400 ohms
*       R2 = 700 ohms
*       C1 = 200 nano Farads 
*       DLP = -3dB
*       ELP = -3dB
*       HWDACgain = 0dB
*       HW_impedance = 680 ohms
*       Protection resistor = 10 ohms
*       Ringing frequency = 20 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.7
*
****************************************************************************/

#ifndef FLEXICALC_ITALY_32392_H
#define FLEXICALC_ITALY_32392_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_ITALY
#define flexicalcITALYArchive32392 ((const APM6828_FLEXICALC_CFG*) NULL)
#else
const APM6828_FLEXICALC_CFG flexicalcITALYArchive32392[] =
{
{
   0x32392,          /* Slic type: Si32392 */
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
      0x3FEE3,   /* YFIR1_VAL */
      0x84C94,   /* YFIR2_VAL */
      0x1E619,   /* YFIR3_VAL */
      0x2ADB8,   /* YFIR4_VAL */
      0x026DD,   /* YFIR5_VAL */
      0xE7FBB,   /* YFIR6_VAL */
      0xF0961,   /* YFIR7_VAL */
      0x0853D,   /* YFIR8_VAL */
      0x1274F,   /* YFIR9_VAL */
      0x07DBD,   /* YFIR10_VAL */
      0xF7E46,   /* YFIR11_VAL */
      0xF4213,   /* YFIR12_VAL */
      0xFE846,   /* YFIR13_VAL */
      0x0A345,   /* YFIR14_VAL */
      0x0A424,   /* YFIR15_VAL */
      0xFC722,   /* YFIR16_VAL */
      0xE6D42,   /* YFIR17_VAL */
      0x1001E    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x7190E},          /* y_iir1_filter[1] */
   0x86,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0x7B9D, 0x8310, 0x2F4D, 0x8310, 0x6F7E},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x520B, 0xDD45, 0x0155, 0xFCFC, 0x009E, 0xFEA7, 0xFFF1, 0xFF1D,
      0xFFCE, 0xFF56, 0xFFC4, 0xFF81, 0xFFCE, 0xFFA4, 0xFFDB, 0xFFBB,
      0xFFE3, 0xFFCC, 0xFFF1, 0xFFDB, 0xFFF2, 0xFFE2, 0xFFF8, 0xFFE6,
      0xFFF8, 0xFFE9, 0xFFF6, 0xFFED, 0xFFF9, 0xFFEF, 0xFFF7, 0xFFF2,
      0xFFFA, 0xFFF0, 0xFFFB, 0xFFF4, 0xFFFC, 0xFFF8, 0x0001, 0xFFFA,
      0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x413D, 0xFCCF, 0x0019, 0xFEC4, 0xFFE0, 0xFF6A, 0xFFC8, 0xFF95,
      0xFFC8, 0xFFAD, 0xFFCD, 0xFFC1, 0xFFD7, 0xFFD0, 0xFFE2, 0xFFDD,
      0xFFEB, 0xFFE7, 0xFFF2, 0xFFEE, 0xFFF4, 0xFFF1, 0xFFF7, 0xFFF3,
      0xFFF8, 0xFFF4, 0xFFF7, 0xFFF6, 0xFFF8, 0xFFF6, 0xFFF9, 0xFFF7,
      0xFFFA, 0xFFF9, 0xFFFC, 0xFFFA, 0xFFFD, 0xFFFC, 0xFFFF, 0xFFFE,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x7D83,           /* asrc_int_scale */
   0x08D6,           /* asrc_dec_scale */
   2,                /* vtx_pg */
   0,                /* vrx_pg */
   0,                /* hpf_en */
   6,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_ITALY_32392_H */
