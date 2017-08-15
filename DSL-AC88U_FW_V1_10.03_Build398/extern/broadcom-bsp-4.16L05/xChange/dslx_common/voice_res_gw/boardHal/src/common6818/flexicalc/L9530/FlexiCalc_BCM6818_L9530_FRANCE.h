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
*       SLIC = Zarlink L9530            
*       R1 = 600 ohms
*       R2 = 0 ohms
*       C1 = 0 nano Farads 
*       DLP = -10dB
*       ELP = -4dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 50 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.7
*
****************************************************************************/

#ifndef FLEXICALC_FRANCE_9530_H
#define FLEXICALC_FRANCE_9530_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_FRANCE
#define flexicalcFRANCEArchive9530 ((const APM6818_FLEXICALC_CFG*) NULL)
#else
const APM6818_FLEXICALC_CFG flexicalcFRANCEArchive9530[] =
{
{
   0x9530,           /* Slic type: L9530 */
   45,               /* Ring Voltage (RMS) */
   -10,               /* DLP - Decode Level Point (receive loss) in dB */
   -4,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0006,           /* eq_rx_shft */
   0x000A,           /* eq_tx_shft */
   0,                /* eq_imp_resp */

   /*
   ** Y-filter Coefficients
   */
   1,                /* yfltr_en */
   {  /* IIR 2 Filter Coefficients */
      0x16860,   /* Y IIR2 filter b0 */
      0x28E0B,   /* Y IIR2 filter b1 */
      0x16860,   /* Y IIR2 filter b2 */
      0x4A22B,   /* Y IIR2 filter a1 */
      0xDFB13    /* Y IIR2 filter a2 */
   },
   0x66,             /* y_iir2_filter_shift */
   {  /* Fir Filter Coefficients */
      0xCD60C,   /* YFIR1_VAL */
      0x4B89B,   /* YFIR2_VAL */
      0xE5867,   /* YFIR3_VAL */
      0x0F5CD,   /* YFIR4_VAL */
      0xFD726,   /* YFIR5_VAL */
      0xEBAF1,   /* YFIR6_VAL */
      0x0580B,   /* YFIR7_VAL */
      0xFF81A,   /* YFIR8_VAL */
      0xFC5EF,   /* YFIR9_VAL */
      0x0B4EA,   /* YFIR10_VAL */
      0x0168F,   /* YFIR11_VAL */
      0xFB7EE,   /* YFIR12_VAL */
      0x026CB,   /* YFIR13_VAL */
      0xFB59B,   /* YFIR14_VAL */
      0xFCF11,   /* YFIR15_VAL */
      0x04C49,   /* YFIR16_VAL */
      0x01D47,   /* YFIR17_VAL */
      0xFE037    /* YFIR18_VAL */
   },
   0x06,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x7EF9B},          /* y_iir1_filter[1] */
   0x76,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   5,                  /* hybal_shft */
   {0x4E03, 0xC25C, 0x0A7E, 0x2F45, 0xE135},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7F28, 0xFD35, 0xFE06, 0xFDC6, 0xFDF9, 0xFE0A, 0xFE2B, 0xFE42,
      0xFE6F, 0xFE8D, 0xFEAF, 0xFED2, 0xFEF5, 0xFF12, 0xFF36, 0xFF52,
      0xFF69, 0xFF7E, 0xFF96, 0xFFA2, 0xFFAA, 0xFFB6, 0xFFBF, 0xFFC1,
      0xFFC6, 0xFFCA, 0xFFC9, 0xFFC9, 0xFFCF, 0xFFCE, 0xFFCE, 0xFFD4,
      0xFFDA, 0xFFDC, 0xFFE1, 0xFFE7, 0xFFEA, 0xFFEF, 0xFFF6, 0xFFF9,
      0xFFFB, 0x0004, 0x0004, 0x0003, 0x0003, 0x0002, 0x0002, 0x0002,
      0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x7FA0, 0xFE50, 0xFEF2, 0xFEBE, 0xFEE0, 0xFEE5, 0xFEFD, 0xFF0E,
      0xFF23, 0xFF37, 0xFF4C, 0xFF60, 0xFF74, 0xFF85, 0xFF98, 0xFFA4,
      0xFFB5, 0xFFBD, 0xFFC9, 0xFFCF, 0xFFD4, 0xFFD8, 0xFFDA, 0xFFDD,
      0xFFDD, 0xFFE0, 0xFFE1, 0xFFE3, 0xFFE4, 0xFFE5, 0xFFE8, 0xFFE9,
      0xFFEC, 0xFFED, 0xFFF1, 0xFFF2, 0xFFF5, 0xFFF8, 0xFFFA, 0xFFFD,
      0xFFFE, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x70AA,           /* asrc_int_scale */
   0x0AB9,           /* asrc_dec_scale */
   1,                /* vtx_pg */
   1,                /* vrx_pg */
   0,                /* hpf_en */
   4,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_FRANCE_9530_H */
