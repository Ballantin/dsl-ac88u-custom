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
*       SLIC = Silicon Labs Si3239       
*       R1 = 600 ohms
*       R2 = 0 ohms
*       C1 = 0 nano Farads 
*       DLP = -4dB
*       ELP = -2dB
*       HWDACgain = 0dB
*       HW_impedance = 680 ohms
*       Protection resistor = 10 ohms
*       Ringing frequency = 20 hertz
*       Ringing amplitude = 57Vrms
*
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_WB_NORTH_AMERICA_3239_H
#define FLEXICALC_WB_NORTH_AMERICA_3239_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_NORTH_AMERICA
#define flexicalcWBNORTH_AMERICAArchive3239 ((const APM6818_FLEXICALC_CFG*) NULL)
#else
const APM6818_FLEXICALC_CFG flexicalcWBNORTH_AMERICAArchive3239[] =
{
{
   0x3239,           /* Slic type: Si3239 */
   57,               /* Ring Voltage (RMS) */
   -4,               /* DLP - Decode Level Point (receive loss) in dB */
   -2,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0008,           /* eq_rx_shft */
   0x000B,           /* eq_tx_shft */
   0,               /* eq_imp_resp */

   /*
   ** Y-filter Coefficients
   */
   1,               /* yfltr_en */
   {  /* IIR 2 Filter Coefficients */
      0x16860,   /* Y IIR2 filter b0 */
      0x28E0B,   /* Y IIR2 filter b1 */
      0x16860,   /* Y IIR2 filter b2 */
      0x4A22B,   /* Y IIR2 filter a1 */
      0xDFB13    /* Y IIR2 filter a2 */
   },
   0x66,             /* y_iir2_filter_shift */
   {  /* Fir Filter Coefficients */
      0x51B1E,   /* YFIR1_VAL */
      0xB0C55,   /* YFIR2_VAL */
      0x2D8DD,   /* YFIR3_VAL */
      0xCD44E,   /* YFIR4_VAL */
      0xF67AA,   /* YFIR5_VAL */
      0x210B3,   /* YFIR6_VAL */
      0xEA6AA,   /* YFIR7_VAL */
      0x150DA,   /* YFIR8_VAL */
      0x103E2,   /* YFIR9_VAL */
      0xEA9E8,   /* YFIR10_VAL */
      0x0A638,   /* YFIR11_VAL */
      0xFB81F,   /* YFIR12_VAL */
      0xEEAC3,   /* YFIR13_VAL */
      0x0CC77,   /* YFIR14_VAL */
      0xFD8EF,   /* YFIR15_VAL */
      0xFDDFB,   /* YFIR16_VAL */
      0x0C9E6,   /* YFIR17_VAL */
      0xF87F9    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0xD2B66},          /* y_iir1_filter[1] */
   0x76,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   5,               /* hybal_shft */
   {0x7A59, 0xA787, 0x1B42, 0x488F, 0xCBD8},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x4096, 0xFEFE, 0x0088, 0xFFB8, 0x0033, 0xFFD9, 0x000C, 0xFFF0,
      0x0002, 0xFFF6, 0x0001, 0xFFF8, 0xFFFF, 0xFFF8, 0xFFFE, 0xFFFA,
      0xFFFF, 0xFFFA, 0xFFFD, 0xFFFB, 0xFFFE, 0xFFFB, 0xFFFE, 0xFFFC,
      0xFFFE, 0xFFFD, 0xFFFE, 0xFFFC, 0xFFFF, 0xFFFD, 0xFFFF, 0xFFFD,
      0xFFFE, 0xFFFE, 0xFFFF, 0xFFFE, 0xFFFF, 0xFFFE, 0xFFFF, 0xFFFF,
      0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x40C5, 0xFEBE, 0x009C, 0xFFB1, 0x0038, 0xFFD5, 0x000F, 0xFFEF,
      0x0004, 0xFFF5, 0x0001, 0xFFF6, 0xFFFF, 0xFFF8, 0xFFFF, 0xFFFA,
      0xFFFF, 0xFFFA, 0xFFFE, 0xFFFB, 0xFFFF, 0xFFFC, 0xFFFF, 0xFFFC,
      0xFFFF, 0xFFFD, 0xFFFF, 0xFFFE, 0xFFFF, 0xFFFE, 0xFFFF, 0xFFFE,
      0x0000, 0xFFFE, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x44C7,           /* asrc_int_scale */
   0x18F3,           /* asrc_dec_scale */
   2,               /* vtx_pg */
   0,               /* vrx_pg */
   0,                /* hpf_en */
   4,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_WB_NORTH_AMERICA_3239_H */
