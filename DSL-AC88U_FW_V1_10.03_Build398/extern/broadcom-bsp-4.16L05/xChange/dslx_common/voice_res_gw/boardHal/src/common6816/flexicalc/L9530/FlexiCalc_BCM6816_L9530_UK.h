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
*       R1 = 300 ohms
*       R2 = 1000 ohms
*       C1 = 220 nano Farads 
*       DLP = -9dB
*       ELP = -4dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 16.66 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_UK_9530_H
#define FLEXICALC_UK_9530_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_UK
#define flexicalcUKArchive9530 ((const APM6816_FLEXICALC_CFG*) NULL)
#else
const APM6816_FLEXICALC_CFG flexicalcUKArchive9530[] =
{
{
   0x9530,           /* Slic type: L9530 */
   45,               /* Ring Voltage (RMS) */
   -9,               /* DLP - Decode Level Point (receive loss) in dB */
   -4,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0007,               /* eq_rx_shft */
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
      0xD44D1,   /* YFIR1_VAL */
      0x55C2F,   /* YFIR2_VAL */
      0xE37DC,   /* YFIR3_VAL */
      0xE3888,   /* YFIR4_VAL */
      0x05177,   /* YFIR5_VAL */
      0x120BC,   /* YFIR6_VAL */
      0x05A9F,   /* YFIR7_VAL */
      0xF6651,   /* YFIR8_VAL */
      0xF5C6B,   /* YFIR9_VAL */
      0x001B8,   /* YFIR10_VAL */
      0x0717E,   /* YFIR11_VAL */
      0x041EE,   /* YFIR12_VAL */
      0xFD485,   /* YFIR13_VAL */
      0xFB062,   /* YFIR14_VAL */
      0xFDC78,   /* YFIR15_VAL */
      0x01B1D,   /* YFIR16_VAL */
      0x0B081,   /* YFIR17_VAL */
      0xF88CD    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x7B2D6},          /* y_iir1_filter[1] */
   0xA6,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   5,                  /* hybal_shft */
   {0x4AF5, 0xD895, 0x0D9A, 0x2974, 0xB357},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7BDB, 0x0443, 0xF967, 0x0110, 0xFC77, 0xFF63, 0xFD79, 0xFEC5,
      0xFDF6, 0xFEBE, 0xFE5A, 0xFEDA, 0xFEBA, 0xFF0E, 0xFF08, 0xFF4C,
      0xFF43, 0xFF79, 0xFF76, 0xFFA4, 0xFF8E, 0xFFB8, 0xFFA4, 0xFFC3,
      0xFFB0, 0xFFC9, 0xFFB6, 0xFFCA, 0xFFC2, 0xFFCD, 0xFFC7, 0xFFD1,
      0xFFD2, 0xFFDD, 0xFFD7, 0xFFE9, 0xFFE2, 0xFFF2, 0xFFEF, 0xFFFD,
      0xFFF7, 0x0004, 0x0005, 0x0004, 0x0004, 0x0003, 0x0003, 0x0002,
      0x0002, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x5C33, 0x34C7, 0x17DE, 0x0FAC, 0x069E, 0x0424, 0x00F8, 0x0062,
      0xFF33, 0xFF43, 0xFECA, 0xFEF7, 0xFECE, 0xFF07, 0xFEFD, 0xFF27,
      0xFF34, 0xFF6A, 0xFF6F, 0xFF99, 0xFF94, 0xFFB1, 0xFF93, 0xFF93,
      0xFF89, 0xFFA8, 0xFFAA, 0xFFBC, 0xFFB3, 0xFFBA, 0xFFB8, 0xFFC1,
      0xFFC3, 0xFFCC, 0xFFCC, 0xFFD8, 0xFFD8, 0xFFE6, 0xFFE3, 0xFFF4,
      0xFFF1, 0xFFFC, 0x0000, 0x0001, 0x0002, 0x0002, 0x0002, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x4A06,           /* asrc_int_scale */
   0x0962,           /* asrc_dec_scale */
   0,               /* vtx_pg */
   1,               /* vrx_pg */
   0,               /* hpf_en */
   6,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_UK_9530_H */
