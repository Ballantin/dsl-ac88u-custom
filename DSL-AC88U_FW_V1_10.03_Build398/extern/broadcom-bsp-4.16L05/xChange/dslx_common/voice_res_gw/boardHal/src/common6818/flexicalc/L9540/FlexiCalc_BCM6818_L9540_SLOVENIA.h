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
*       R1 = 270 ohms
*       R2 = 750 ohms
*       C1 = 150 nano Farads 
*       DLP = -6dB
*       ELP = -4dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 20 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.7
*
****************************************************************************/

#ifndef FLEXICALC_BCM6818_L9540_SLOVENIA_H
#define FLEXICALC_BCM6818_L9540_SLOVENIA_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_SLOVENIA
#define flexicalcSLOVENIAArchive9540 ((const APM6818_FLEXICALC_CFG*) NULL)
#else
const APM6818_FLEXICALC_CFG flexicalcSLOVENIAArchive9540[] =
{
{
   0x9540,           /* Slic type:  */
   45,               /* Ring Voltage (RMS) */
   -6,               /* DLP - Decode Level Point (receive loss) in dB */
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
      0xCF99E,   /* YFIR1_VAL */
      0x5DCC9,   /* YFIR2_VAL */
      0xE1E8D,   /* YFIR3_VAL */
      0xE0686,   /* YFIR4_VAL */
      0x0509A,   /* YFIR5_VAL */
      0x14308,   /* YFIR6_VAL */
      0x06D08,   /* YFIR7_VAL */
      0xF559D,   /* YFIR8_VAL */
      0xF4662,   /* YFIR9_VAL */
      0x0074F,   /* YFIR10_VAL */
      0x08A82,   /* YFIR11_VAL */
      0x0485C,   /* YFIR12_VAL */
      0xFBA0C,   /* YFIR13_VAL */
      0xF9AA3,   /* YFIR14_VAL */
      0xFF829,   /* YFIR15_VAL */
      0x0462F,   /* YFIR16_VAL */
      0x0531F,   /* YFIR17_VAL */
      0xFBA4F    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x74C21},          /* y_iir1_filter[1] */
   0xA6,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   5,                  /* hybal_shft */
   {0x4DDB, 0xAF82, 0x3E85, 0xE6AD, 0xEA62},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7B5D, 0x03BB, 0xFA5D, 0x00A2, 0xFC0C, 0xFEE8, 0xFD4A, 0xFE9C,
      0xFDE9, 0xFEB1, 0xFE5C, 0xFEDA, 0xFEC0, 0xFF0F, 0xFF0C, 0xFF4F,
      0xFF4A, 0xFF7C, 0xFF79, 0xFFA3, 0xFF91, 0xFFBA, 0xFFA8, 0xFFC5,
      0xFFB3, 0xFFCB, 0xFFBA, 0xFFCC, 0xFFC3, 0xFFCD, 0xFFC8, 0xFFD1,
      0xFFD3, 0xFFDE, 0xFFD9, 0xFFEA, 0xFFE4, 0xFFF3, 0xFFF0, 0xFFFD,
      0xFFF8, 0x0004, 0x0005, 0x0004, 0x0004, 0x0003, 0x0003, 0x0002,
      0x0002, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x5ECC, 0x299E, 0x083A, 0x0474, 0xFEE7, 0xFF89, 0xFE45, 0xFF02,
      0xFE83, 0xFF10, 0xFED5, 0xFF31, 0xFF13, 0xFF5E, 0xFF4F, 0xFF80,
      0xFF7A, 0xFFA7, 0xFF97, 0xFFBA, 0xFFAE, 0xFFD1, 0xFFBE, 0xFFCB,
      0xFFC2, 0xFFD6, 0xFFCB, 0xFFD5, 0xFFCB, 0xFFD5, 0xFFD2, 0xFFDD,
      0xFFDA, 0xFFE6, 0xFFE2, 0xFFED, 0xFFEB, 0xFFF6, 0xFFF2, 0xFFFF,
      0xFFFA, 0x0000, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x4447,           /* asrc_int_scale */
   0x09C2,           /* asrc_dec_scale */
   0,                /* vtx_pg */
   1,                /* vrx_pg */
   0,                /* hpf_en */
   5,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_BCM6818_L9540_SLOVENIA_H */
