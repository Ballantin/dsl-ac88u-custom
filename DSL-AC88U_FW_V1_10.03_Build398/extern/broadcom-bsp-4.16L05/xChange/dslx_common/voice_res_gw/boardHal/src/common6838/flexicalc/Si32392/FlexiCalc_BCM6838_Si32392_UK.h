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
*       R1 = 300 ohms
*       R2 = 1000 ohms
*       C1 = 220 nano Farads 
*       DLP = -9dB
*       ELP = -4dB
*       HWDACgain = 0dB
*       HW_impedance = 680 ohms
*       Protection resistor = 10 ohms
*       Ringing frequency = 16.66 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.8
*
****************************************************************************/

#ifndef FLEXICALC_UK_32392_H
#define FLEXICALC_UK_32392_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_UK
#define flexicalcUKArchive32392 ((const APM6838_FLEXICALC_CFG*) NULL)
#else
const APM6838_FLEXICALC_CFG flexicalcUKArchive32392[] =
{
{
   0x32392,          /* Slic type: Si32392 */
   45,               /* Ring Voltage (RMS) */
   -9,               /* DLP - Decode Level Point (receive loss) in dB */
   -4,               /* ELP - Encode Level Point (transmitt loss) in dB */
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
      0xD0B7A,   /* YFIR1_VAL */
      0x5931F,   /* YFIR2_VAL */
      0xE57C8,   /* YFIR3_VAL */
      0xE08C5,   /* YFIR4_VAL */
      0x048B0,   /* YFIR5_VAL */
      0x15B15,   /* YFIR6_VAL */
      0x080CC,   /* YFIR7_VAL */
      0xF4D07,   /* YFIR8_VAL */
      0xF3CDE,   /* YFIR9_VAL */
      0x01D42,   /* YFIR10_VAL */
      0x0AA03,   /* YFIR11_VAL */
      0x03C8A,   /* YFIR12_VAL */
      0xF8125,   /* YFIR13_VAL */
      0xF83A9,   /* YFIR14_VAL */
      0x041D1,   /* YFIR15_VAL */
      0x08B6A,   /* YFIR16_VAL */
      0xF6E1F,   /* YFIR17_VAL */
      0x03FAC    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x54989},          /* y_iir1_filter[1] */
   0xA6,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0xBE96, 0x49E5, 0xCC4C, 0x7CE4, 0xA0B7},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x58A3, 0xC801, 0x0420, 0xFB1E, 0x01AF, 0xFE86, 0x0073, 0xFF31,
      0x002B, 0xFF6F, 0x000B, 0xFF98, 0x0008, 0xFFBA, 0x0007, 0xFFC8,
      0x0003, 0xFFD8, 0x000D, 0xFFE2, 0x0003, 0xFFE7, 0x0008, 0xFFEF,
      0x0008, 0xFFF1, 0x0004, 0xFFF5, 0x0005, 0xFFF5, 0x0002, 0xFFF9,
      0x0003, 0xFFF3, 0x0002, 0xFFF6, 0x0001, 0xFFFA, 0x0004, 0xFFF9,
      0x0004, 0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x41EF, 0xF9CC, 0xFFFD, 0xFDB1, 0xFF92, 0xFF3B, 0xFFC9, 0xFF9E,
      0xFFE1, 0xFFC5, 0xFFEA, 0xFFDA, 0xFFF2, 0xFFE6, 0xFFF8, 0xFFED,
      0xFFFC, 0xFFF2, 0xFFFE, 0xFFF4, 0xFFFF, 0xFFFA, 0x0002, 0xFFFC,
      0x0002, 0xFFFC, 0xFFFD, 0xFFFB, 0xFFFE, 0xFFFC, 0xFFFF, 0xFFFD,
      0xFFFF, 0xFFFD, 0x0000, 0xFFFD, 0x0000, 0xFFFE, 0x0001, 0xFFFF,
      0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0003,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x41A8,           /* asrc_int_scale */
   0x0CE0,           /* asrc_dec_scale */
   0,                /* vtx_pg */
   1,                /* vrx_pg */
   0,                /* hpf_en */
   6,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_UK_32392_H */
