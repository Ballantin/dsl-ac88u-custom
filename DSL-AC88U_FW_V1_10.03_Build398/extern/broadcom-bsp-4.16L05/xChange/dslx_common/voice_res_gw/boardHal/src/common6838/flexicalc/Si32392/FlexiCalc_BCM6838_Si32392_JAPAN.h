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
*       SLIC = Silicon Labs Si32392      
*       R1 = 600 ohms
*       R2 = 1E+09 ohms
*       C1 = 1000 nano Farads 
*       DLP = -3dB
*       ELP = -3dB
*       HWDACgain = 0dB
*       HW_impedance = 680 ohms
*       Protection resistor = 10 ohms
*       Ringing frequency = 20 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.8
*
****************************************************************************/

#ifndef FLEXICALC_JAPAN_32392_H
#define FLEXICALC_JAPAN_32392_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_JAPAN
#define flexicalcJAPANArchive32392 ((const APM6838_FLEXICALC_CFG*) NULL)
#else
const APM6838_FLEXICALC_CFG flexicalcJAPANArchive32392[] =
{
{
   0x32392,          /* Slic type: Si32392 */
   45,               /* Ring Voltage (RMS) */
   -3,               /* DLP - Decode Level Point (receive loss) in dB */
   -3,               /* ELP - Encode Level Point (transmitt loss) in dB */
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
      0xD5BC7,   /* YFIR1_VAL */
      0x26C15,   /* YFIR2_VAL */
      0x14356,   /* YFIR3_VAL */
      0x0A058,   /* YFIR4_VAL */
      0xFFA66,   /* YFIR5_VAL */
      0xF46CE,   /* YFIR6_VAL */
      0xEF765,   /* YFIR7_VAL */
      0xF2DCD,   /* YFIR8_VAL */
      0xFCBAD,   /* YFIR9_VAL */
      0x06F70,   /* YFIR10_VAL */
      0x0BDB2,   /* YFIR11_VAL */
      0x09606,   /* YFIR12_VAL */
      0x02491,   /* YFIR13_VAL */
      0xFBACD,   /* YFIR14_VAL */
      0xF9868,   /* YFIR15_VAL */
      0xFCAB3,   /* YFIR16_VAL */
      0x03721,   /* YFIR17_VAL */
      0x0035B    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x7E38E},          /* y_iir1_filter[1] */
   0x66,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0xF595, 0xC9F3, 0x0BB5, 0x7CF3, 0xA61F},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7406, 0xE19A, 0xE856, 0xED3C, 0xF169, 0xF472, 0xF724, 0xF92B,
      0xFB02, 0xFC56, 0xFD72, 0xFE54, 0xFF0A, 0xFF8F, 0x0001, 0x004C,
      0x007E, 0x00A1, 0x00C2, 0x00C6, 0x00C0, 0x00C1, 0x00C1, 0x00B5,
      0x00AE, 0x00A2, 0x0091, 0x0084, 0x0084, 0x007B, 0x0078, 0x0084,
      0x008E, 0x008F, 0x0095, 0x009A, 0x0097, 0x0095, 0x0094, 0x0088,
      0x007D, 0x007B, 0x0069, 0x005A, 0x004D, 0x0042, 0x0038, 0x0030,
      0x0029, 0x0024, 0x001F, 0x001A, 0x0017, 0x0013, 0x0011, 0x000E,
      0x000C, 0x000B, 0x0009, 0x0008, 0x0007, 0x0006, 0x0005, 0x0004,
      0x0003, 0x0003, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x7EF5, 0xFC1D, 0xFCC6, 0xFD00, 0xFD56, 0xFD54, 0xFD9B, 0xFDBC,
      0xFDF8, 0xFE24, 0xFE5A, 0xFE88, 0xFEBA, 0xFEDF, 0xFF0E, 0xFF2C,
      0xFF52, 0xFF64, 0xFF81, 0xFF91, 0xFF9B, 0xFFA4, 0xFFAB, 0xFFB2,
      0xFFB3, 0xFFB9, 0xFFBC, 0xFFC1, 0xFFC3, 0xFFC6, 0xFFCC, 0xFFCD,
      0xFFD5, 0xFFD7, 0xFFE1, 0xFFE3, 0xFFEB, 0xFFF1, 0xFFF7, 0xFFFD,
      0xFFFE, 0x0006, 0x0006, 0x0005, 0x0004, 0x0004, 0x0003, 0x0003,
      0x0002, 0x0002, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0003,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x50A2,           /* asrc_int_scale */
   0x11C8,           /* asrc_dec_scale */
   0,                /* vtx_pg */
   0,                /* vrx_pg */
   0,                /* hpf_en */
   6,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_JAPAN_32392_H */
