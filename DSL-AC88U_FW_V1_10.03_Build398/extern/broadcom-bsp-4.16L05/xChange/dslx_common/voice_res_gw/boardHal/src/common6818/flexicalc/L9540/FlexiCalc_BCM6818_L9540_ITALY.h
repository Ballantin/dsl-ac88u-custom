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
*       R1 = 400 ohms
*       R2 = 700 ohms
*       C1 = 200 nano Farads 
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

#ifndef FLEXICALC_BCM6818_L9540_ITALY_H
#define FLEXICALC_BCM6818_L9540_ITALY_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_ITALY
#define flexicalcITALYArchive9540 ((const APM6818_FLEXICALC_CFG*) NULL)
#else
const APM6818_FLEXICALC_CFG flexicalcITALYArchive9540[] =
{
{
   0x9540,           /* Slic type:  */
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
      0xBB543,   /* YFIR1_VAL */
      0x64886,   /* YFIR2_VAL */
      0x038DC,   /* YFIR3_VAL */
      0xE8388,   /* YFIR4_VAL */
      0xF2EE1,   /* YFIR5_VAL */
      0x03DF1,   /* YFIR6_VAL */
      0x08DA7,   /* YFIR7_VAL */
      0xFFFA0,   /* YFIR8_VAL */
      0xF5FD1,   /* YFIR9_VAL */
      0xF5C28,   /* YFIR10_VAL */
      0xFED83,   /* YFIR11_VAL */
      0x075FB,   /* YFIR12_VAL */
      0x071E7,   /* YFIR13_VAL */
      0xFFC20,   /* YFIR14_VAL */
      0xFAF39,   /* YFIR15_VAL */
      0x009B5,   /* YFIR16_VAL */
      0x0EA0A,   /* YFIR17_VAL */
      0xF368E    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x7AF08},          /* y_iir1_filter[1] */
   0x76,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   6,                  /* hybal_shft */
   {0x4460, 0xD911, 0xFC7A, 0x49C6, 0xAF63},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7E75, 0xFF42, 0xFD4F, 0xFE5E, 0xFDD9, 0xFEE3, 0xFDF9, 0xFE92,
      0xFE40, 0xFEA6, 0xFE87, 0xFECE, 0xFEDA, 0xFF08, 0xFF24, 0xFF48,
      0xFF57, 0xFF74, 0xFF8A, 0xFFA1, 0xFF9E, 0xFFB3, 0xFFB2, 0xFFC0,
      0xFFBA, 0xFFC8, 0xFFBF, 0xFFC6, 0xFFCA, 0xFFCD, 0xFFCA, 0xFFD2,
      0xFFD7, 0xFFDA, 0xFFDC, 0xFFE6, 0xFFE6, 0xFFEF, 0xFFF2, 0xFFFA,
      0xFFF9, 0x0004, 0x0004, 0x0003, 0x0003, 0x0003, 0x0002, 0x0002,
      0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x648C, 0x2522, 0x0C1E, 0x0519, 0x00CB, 0x0064, 0xFF11, 0xFF48,
      0xFECF, 0xFF1C, 0xFEEC, 0xFF29, 0xFF19, 0xFF4F, 0xFF50, 0xFF6F,
      0xFF7D, 0xFF9D, 0xFF9F, 0xFFB6, 0xFFB3, 0xFFCB, 0xFFBC, 0xFFBF,
      0xFFBE, 0xFFCF, 0xFFC8, 0xFFD3, 0xFFCC, 0xFFD2, 0xFFD2, 0xFFD8,
      0xFFDA, 0xFFE0, 0xFFE1, 0xFFE8, 0xFFEA, 0xFFF2, 0xFFF1, 0xFFFC,
      0xFFF9, 0x0000, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x6017,           /* asrc_int_scale */
   0x0E8B,           /* asrc_dec_scale */
   1,                /* vtx_pg */
   0,                /* vrx_pg */
   0,                /* hpf_en */
   5,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_BCM6818_L9540_ITALY_H */
