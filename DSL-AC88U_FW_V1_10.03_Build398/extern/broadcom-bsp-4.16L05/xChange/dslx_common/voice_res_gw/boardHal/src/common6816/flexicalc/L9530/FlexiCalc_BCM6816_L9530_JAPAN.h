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
*       BCM6816
*       SLIC = Legerity L9530
*       R1 = 600 ohms
*       R2 = 1E+09 ohms
*       C1 = 1000 nano Farads 
*       DLP = -3dB
*       ELP = -3dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 20 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_JAPAN_9530_H
#define FLEXICALC_JAPAN_9530_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_JAPAN
#define flexicalcJAPANArchive9530 ((const APM6816_FLEXICALC_CFG*) NULL)
#else
const APM6816_FLEXICALC_CFG flexicalcJAPANArchive9530[] =
{
{
   0x9530,           /* Slic type: L9530 */
   45,               /* Ring Voltage (RMS) */
   -3,               /* DLP - Decode Level Point (receive loss) in dB */
   -3,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0008,           /* eq_rx_shft */
   0x000B,               /* eq_tx_shft */
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
      0xDFB25,   /* YFIR1_VAL */
      0x428E5,   /* YFIR2_VAL */
      0xE3310,   /* YFIR3_VAL */
      0xEBB57,   /* YFIR4_VAL */
      0x0A9AE,   /* YFIR5_VAL */
      0x0F161,   /* YFIR6_VAL */
      0xFF08A,   /* YFIR7_VAL */
      0xF443E,   /* YFIR8_VAL */
      0xFA9A4,   /* YFIR9_VAL */
      0x06963,   /* YFIR10_VAL */
      0x08318,   /* YFIR11_VAL */
      0xFE9EF,   /* YFIR12_VAL */
      0xF7716,   /* YFIR13_VAL */
      0xFCBD8,   /* YFIR14_VAL */
      0x07A1D,   /* YFIR15_VAL */
      0x07092,   /* YFIR16_VAL */
      0xF2901,   /* YFIR17_VAL */
      0x058B3    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x7FB96},          /* y_iir1_filter[1] */
   0x66,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,               /* hybal_shft */
   {0x559E, 0x8301, 0x30D1, 0x7D0B, 0x8ADC},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x4023, 0xFF0E, 0xFF70, 0xFF6D, 0xFF7C, 0xFF4E, 0xFF56, 0xFF44,
      0xFF55, 0xFF55, 0xFF61, 0xFF6C, 0xFF7C, 0xFF87, 0xFF99, 0xFFA7,
      0xFFB2, 0xFFBC, 0xFFCA, 0xFFD0, 0xFFD4, 0xFFDA, 0xFFDF, 0xFFDF,
      0xFFE2, 0xFFE3, 0xFFE3, 0xFFE4, 0xFFE8, 0xFFE7, 0xFFE6, 0xFFEA,
      0xFFEC, 0xFFED, 0xFFEF, 0xFFF2, 0xFFF3, 0xFFF6, 0xFFFA, 0xFFFB,
      0xFFFD, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x4321, 0x08C0, 0x086A, 0x07D9, 0x0746, 0x0689, 0x0604, 0x055F,
      0x04EC, 0x0460, 0x03FA, 0x0383, 0x0327, 0x02CD, 0x0279, 0x0235,
      0x01EE, 0x01BF, 0x0186, 0x015A, 0x0137, 0x0113, 0x00FA, 0x00DF,
      0x00D5, 0x00C6, 0x00C1, 0x00B6, 0x00B4, 0x00AC, 0x00A2, 0x009D,
      0x0090, 0x0088, 0x0077, 0x006E, 0x005F, 0x004F, 0x0043, 0x0034,
      0x002E, 0x001D, 0x0019, 0x0016, 0x0012, 0x0010, 0x000D, 0x000C,
      0x000A, 0x0008, 0x0007, 0x0006, 0x0005, 0x0005, 0x0004, 0x0003,
      0x0003, 0x0003, 0x0002, 0x0002, 0x0002, 0x0002, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x404C,           /* asrc_int_scale */
   0x092B,           /* asrc_dec_scale */
   2,               /* vtx_pg */
   0,               /* vrx_pg */
   0,               /* hpf_en */
   6,               /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_JAPAN_9530_H */
