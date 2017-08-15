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
*       SLIC = Zarlink             
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
*    Flexicalc Version: 3.7
*
****************************************************************************/

#ifndef FLEXICALC_BCM6818_L9540_JAPAN_H
#define FLEXICALC_BCM6818_L9540_JAPAN_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_JAPAN
#define flexicalcJAPANArchive9540 ((const APM6818_FLEXICALC_CFG*) NULL)
#else
const APM6818_FLEXICALC_CFG flexicalcJAPANArchive9540[] =
{
{
   0x9540,           /* Slic type:  */
   45,               /* Ring Voltage (RMS) */
   -3,               /* DLP - Decode Level Point (receive loss) in dB */
   -3,               /* ELP - Encode Level Point (transmitt loss) in dB */
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
      0xD6A46,   /* YFIR1_VAL */
      0x2D2F2,   /* YFIR2_VAL */
      0x10956,   /* YFIR3_VAL */
      0x00230,   /* YFIR4_VAL */
      0xF9754,   /* YFIR5_VAL */
      0xF6944,   /* YFIR6_VAL */
      0xF77D3,   /* YFIR7_VAL */
      0xFA794,   /* YFIR8_VAL */
      0xFE79B,   /* YFIR9_VAL */
      0x025BB,   /* YFIR10_VAL */
      0x0550F,   /* YFIR11_VAL */
      0x06B8E,   /* YFIR12_VAL */
      0x062A8,   /* YFIR13_VAL */
      0x0374A,   /* YFIR14_VAL */
      0xFE842,   /* YFIR15_VAL */
      0xF7730,   /* YFIR16_VAL */
      0xEE5F8,   /* YFIR17_VAL */
      0x10001    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x7FA7D},          /* y_iir1_filter[1] */
   0x66,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0x5A57, 0x8B9E, 0x0B4A, 0x7CF1, 0x9F88},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x4021, 0xFF1F, 0xFF9F, 0xFF5E, 0xFF8F, 0xFF50, 0xFF59, 0xFF42,
      0xFF54, 0xFF51, 0xFF5E, 0xFF68, 0xFF79, 0xFF83, 0xFF97, 0xFFA4,
      0xFFB0, 0xFFBA, 0xFFC8, 0xFFCE, 0xFFD3, 0xFFD8, 0xFFDE, 0xFFDE,
      0xFFE1, 0xFFE2, 0xFFE3, 0xFFE3, 0xFFE7, 0xFFE6, 0xFFE6, 0xFFE9,
      0xFFEC, 0xFFEC, 0xFFEF, 0xFFF1, 0xFFF3, 0xFFF5, 0xFFFA, 0xFFFB,
      0xFFFC, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x4315, 0x08E1, 0x089B, 0x07D6, 0x0762, 0x0697, 0x0612, 0x0569,
      0x04F4, 0x0466, 0x03FE, 0x0387, 0x0329, 0x02CF, 0x027A, 0x0235,
      0x01EF, 0x01BF, 0x0187, 0x015A, 0x0137, 0x0113, 0x00FA, 0x00DE,
      0x00D5, 0x00C5, 0x00C1, 0x00B6, 0x00B3, 0x00AB, 0x00A2, 0x009D,
      0x0090, 0x0088, 0x0077, 0x006D, 0x005F, 0x004F, 0x0043, 0x0034,
      0x002E, 0x001D, 0x0019, 0x0015, 0x0012, 0x0010, 0x000D, 0x000B,
      0x000A, 0x0008, 0x0007, 0x0006, 0x0005, 0x0005, 0x0004, 0x0003,
      0x0003, 0x0003, 0x0002, 0x0002, 0x0002, 0x0002, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x50A4,           /* asrc_int_scale */
   0x0927,           /* asrc_dec_scale */
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

#endif  /* FLEXICALC_BCM6818_L9540_JAPAN_H */
