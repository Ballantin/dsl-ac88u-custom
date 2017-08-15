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
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_ITALY_9530_H
#define FLEXICALC_ITALY_9530_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_ITALY
#define flexicalcITALYArchive9530 ((const APM6816_FLEXICALC_CFG*) NULL)
#else
const APM6816_FLEXICALC_CFG flexicalcITALYArchive9530[] =
{
{
   0x9530,           /* Slic type: L9530 */
   45,               /* Ring Voltage (RMS) */
   -3,               /* DLP - Decode Level Point (receive loss) in dB */
   -3,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0007,               /* eq_rx_shft */
   0x000A,           /* eq_tx_shft */
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
      0xC98A6,   /* YFIR1_VAL */
      0x516D7,   /* YFIR2_VAL */
      0x01F3D,   /* YFIR3_VAL */
      0xEC60D,   /* YFIR4_VAL */
      0xF560A,   /* YFIR5_VAL */
      0x03518,   /* YFIR6_VAL */
      0x072C7,   /* YFIR7_VAL */
      0xFFBD7,   /* YFIR8_VAL */
      0xF7A04,   /* YFIR9_VAL */
      0xF7AC5,   /* YFIR10_VAL */
      0xFF481,   /* YFIR11_VAL */
      0x063D1,   /* YFIR12_VAL */
      0x05DDD,   /* YFIR13_VAL */
      0xFFB7F,   /* YFIR14_VAL */
      0xFBCEB,   /* YFIR15_VAL */
      0x008F1,   /* YFIR16_VAL */
      0x0C25C,   /* YFIR17_VAL */
      0xF5765    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x7B492},          /* y_iir1_filter[1] */
   0x76,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0x478B, 0xAD5D, 0x43FA, 0xE35F, 0xEFBE},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7E73, 0xFF41, 0xFD35, 0xFE76, 0xFDBE, 0xFF07, 0xFDF6, 0xFEA5,
      0xFE40, 0xFEB0, 0xFE88, 0xFED3, 0xFEDB, 0xFF0A, 0xFF24, 0xFF49,
      0xFF57, 0xFF75, 0xFF8A, 0xFFA2, 0xFF9D, 0xFFB4, 0xFFB1, 0xFFC1,
      0xFFB9, 0xFFC9, 0xFFBF, 0xFFC7, 0xFFCA, 0xFFCD, 0xFFCA, 0xFFD2,
      0xFFD7, 0xFFDA, 0xFFDC, 0xFFE6, 0xFFE5, 0xFFF0, 0xFFF2, 0xFFFA,
      0xFFF8, 0x0004, 0x0004, 0x0003, 0x0003, 0x0003, 0x0002, 0x0002,
      0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x649C, 0x250E, 0x0C09, 0x0521, 0x00BB, 0x0079, 0xFF18, 0xFF59,
      0xFED6, 0xFF27, 0xFEF0, 0xFF2F, 0xFF1B, 0xFF52, 0xFF51, 0xFF71,
      0xFF7E, 0xFF9E, 0xFF9F, 0xFFB7, 0xFFB3, 0xFFCB, 0xFFBC, 0xFFC0,
      0xFFBE, 0xFFCF, 0xFFC8, 0xFFD3, 0xFFCC, 0xFFD3, 0xFFD2, 0xFFD9,
      0xFFDA, 0xFFE1, 0xFFE1, 0xFFE9, 0xFFEA, 0xFFF2, 0xFFF1, 0xFFFC,
      0xFFF9, 0x0000, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x4C79,           /* asrc_int_scale */
   0x0E8A,           /* asrc_dec_scale */
   1,                /* vtx_pg */
   0,               /* vrx_pg */
   0,               /* hpf_en */
   6,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_ITALY_9530_H */
