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
*       SLIC = Zarlink L9530            
*       R1 = 472 ohms
*       R2 = 637 ohms
*       C1 = 219 nano Farads 
*       DLP = -10dB
*       ELP = -4dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.7
*
****************************************************************************/

#ifndef FLEXICALC_NETHERLANDS_9530_H
#define FLEXICALC_NETHERLANDS_9530_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_NETHERLANDS
#define flexicalcNETHERLANDSArchive9530 ((const APM6818_FLEXICALC_CFG*) NULL)
#else
const APM6818_FLEXICALC_CFG flexicalcNETHERLANDSArchive9530[] =
{
{
   0x9530,           /* Slic type: L9530 */
   45,               /* Ring Voltage (RMS) */
   -10,               /* DLP - Decode Level Point (receive loss) in dB */
   -4,               /* ELP - Encode Level Point (transmitt loss) in dB */
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
      0xA7E7D,   /* YFIR1_VAL */
      0x57876,   /* YFIR2_VAL */
      0x2B7EB,   /* YFIR3_VAL */
      0x1ADA4,   /* YFIR4_VAL */
      0xFFB78,   /* YFIR5_VAL */
      0xE14A2,   /* YFIR6_VAL */
      0xD4903,   /* YFIR7_VAL */
      0xDF3F3,   /* YFIR8_VAL */
      0xFAD5B,   /* YFIR9_VAL */
      0x15354,   /* YFIR10_VAL */
      0x1EE53,   /* YFIR11_VAL */
      0x1449C,   /* YFIR12_VAL */
      0xFF989,   /* YFIR13_VAL */
      0xF02AB,   /* YFIR14_VAL */
      0xF02D0,   /* YFIR15_VAL */
      0xFF87B,   /* YFIR16_VAL */
      0x17791,   /* YFIR17_VAL */
      0xF12B6    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x7A7D7},          /* y_iir1_filter[1] */
   0x76,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   5,                  /* hybal_shft */
   {0x4D4B, 0xD317, 0xF32B, 0x68E4, 0x9BCE},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7F75, 0xFDCE, 0xFE16, 0xFDE0, 0xFE68, 0xFE6D, 0xFE38, 0xFE59,
      0xFE62, 0xFE8A, 0xFE9B, 0xFEC4, 0xFEE5, 0xFF04, 0xFF2B, 0xFF46,
      0xFF5E, 0xFF74, 0xFF90, 0xFF9E, 0xFFA5, 0xFFB1, 0xFFB9, 0xFFBE,
      0xFFBF, 0xFFC6, 0xFFC3, 0xFFC5, 0xFFCD, 0xFFCC, 0xFFCB, 0xFFD2,
      0xFFD8, 0xFFD9, 0xFFDE, 0xFFE5, 0xFFE8, 0xFFEE, 0xFFF5, 0xFFF8,
      0xFFFA, 0x0004, 0x0004, 0x0003, 0x0003, 0x0003, 0x0002, 0x0002,
      0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x68B8, 0x2076, 0x0B05, 0x042D, 0x00E6, 0x000A, 0xFF22, 0xFF20,
      0xFEE0, 0xFF0A, 0xFEF9, 0xFF23, 0xFF25, 0xFF4D, 0xFF59, 0xFF71,
      0xFF84, 0xFF9E, 0xFFA5, 0xFFB7, 0xFFB8, 0xFFCA, 0xFFC0, 0xFFC0,
      0xFFC3, 0xFFCE, 0xFFCC, 0xFFD3, 0xFFCF, 0xFFD3, 0xFFD4, 0xFFD9,
      0xFFDC, 0xFFE0, 0xFFE4, 0xFFE8, 0xFFEC, 0xFFF2, 0xFFF3, 0xFFFB,
      0xFFFA, 0x0000, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x44AF,           /* asrc_int_scale */
   0x0D13,           /* asrc_dec_scale */
   1,                /* vtx_pg */
   1,                /* vrx_pg */
   0,                /* hpf_en */
   5,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_NETHERLANDS_9530_H */
