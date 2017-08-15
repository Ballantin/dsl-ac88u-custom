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
*       SLIC = Zarlink L9530            
*       R1 = 370 ohms
*       R2 = 620 ohms
*       C1 = 310 nano Farads 
*       DLP = -4dB
*       ELP = -2dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 20 hertz
*       Ringing amplitude = 57Vrms
*
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_NEW_ZEALAND_9530_H
#define FLEXICALC_NEW_ZEALAND_9530_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_NEW_ZEALAND
#define flexicalcNEW_ZEALANDArchive9530 ((const APM6816_FLEXICALC_CFG*) NULL)
#else
const APM6816_FLEXICALC_CFG flexicalcNEW_ZEALANDArchive9530[] =
{
{
   0x9530,           /* Slic type: L9530 */
   57,               /* Ring Voltage (RMS) */
   -4,               /* DLP - Decode Level Point (receive loss) in dB */
   -2,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0006,               /* eq_rx_shft */
   0x000A,               /* eq_tx_shft */
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
      0xD08C6,   /* YFIR1_VAL */
      0x48AED,   /* YFIR2_VAL */
      0xFCAE4,   /* YFIR3_VAL */
      0xEAECC,   /* YFIR4_VAL */
      0xF8982,   /* YFIR5_VAL */
      0x07780,   /* YFIR6_VAL */
      0x08ACA,   /* YFIR7_VAL */
      0xFECCD,   /* YFIR8_VAL */
      0xF6C40,   /* YFIR9_VAL */
      0xF8B2A,   /* YFIR10_VAL */
      0x01560,   /* YFIR11_VAL */
      0x07007,   /* YFIR12_VAL */
      0x042A7,   /* YFIR13_VAL */
      0xFCE3C,   /* YFIR14_VAL */
      0xFA501,   /* YFIR15_VAL */
      0x0143F,   /* YFIR16_VAL */
      0x0CD50,   /* YFIR17_VAL */
      0xF6D1C    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x7B801},          /* y_iir1_filter[1] */
   0x86,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0x3625, 0xDED7, 0x0098, 0x390D, 0xBF90},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7CC0, 0x017C, 0xFB97, 0xFF94, 0xFC96, 0xFF8B, 0xFDA9, 0xFEEA,
      0xFE1B, 0xFED5, 0xFE74, 0xFEE5, 0xFED0, 0xFF14, 0xFF1A, 0xFF50,
      0xFF4F, 0xFF7B, 0xFF82, 0xFFA7, 0xFF94, 0xFFB9, 0xFFA9, 0xFFC5,
      0xFFB4, 0xFFCE, 0xFFBB, 0xFFCA, 0xFFC6, 0xFFCF, 0xFFCA, 0xFFD2,
      0xFFD6, 0xFFDD, 0xFFDA, 0xFFE8, 0xFFE3, 0xFFF2, 0xFFEF, 0xFFFC,
      0xFFF7, 0x0004, 0x0004, 0x0003, 0x0003, 0x0003, 0x0002, 0x0002,
      0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x65C9, 0x27D4, 0x1069, 0x09C4, 0x02CD, 0x0233, 0xFFC8, 0xFFEB,
      0xFEF7, 0xFF4C, 0xFEDF, 0xFF26, 0xFEFD, 0xFF3D, 0xFF30, 0xFF58,
      0xFF63, 0xFF8D, 0xFF8F, 0xFFAC, 0xFFA7, 0xFFC2, 0xFFA8, 0xFFAF,
      0xFFA6, 0xFFC2, 0xFFBB, 0xFFCB, 0xFFC2, 0xFFCA, 0xFFC8, 0xFFD1,
      0xFFD1, 0xFFD9, 0xFFD9, 0xFFE2, 0xFFE2, 0xFFEE, 0xFFEB, 0xFFF9,
      0xFFF5, 0xFFFF, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x7AF4,           /* asrc_int_scale */
   0x0EB6,           /* asrc_dec_scale */
   1,                /* vtx_pg */
   0,               /* vrx_pg */
   0,                /* hpf_en */
   6,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_NEW_ZEALAND_9530_H */
