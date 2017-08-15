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
*     FlexiCalc output constants for the following inputs:
*
*       BCM6816                          
*       SLIC = Silicon Labs Si3239       
*       R1 = 370 ohms
*       R2 = 620 ohms
*       C1 = 310 nano Farads 
*       DLP = -4dB
*       ELP = -2dB
*       HWDACgain =0dB
*       HW_impedance =680 ohms
*       Protection resistor =10 ohms
*       Ringing frequency = 20 hertz
*       Ringing amplitude = 57Vrms
*
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_NEW_ZEALAND_3239_H
#define FLEXICALC_NEW_ZEALAND_3239_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_NEW_ZEALAND
#define flexicalcNEW_ZEALANDArchive3239 ((const APM6816_FLEXICALC_CFG*) NULL)
#else
const APM6816_FLEXICALC_CFG flexicalcNEW_ZEALANDArchive3239[] =
{
{
   0x3239,               /* Slic type: Si3239 */
   57,               /* Ring Voltage (RMS) */
   -4,               /* DLP - Decode Level Point (receive loss) in dB */
   -2,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0007,               /* eq_rx_shft */
   0x000B,           /* eq_tx_shft */
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
      0x409C9,   /* YFIR1_VAL */
      0x8A4D7,   /* YFIR2_VAL */
      0x225A3,   /* YFIR3_VAL */
      0x29185,   /* YFIR4_VAL */
      0xF9DA8,   /* YFIR5_VAL */
      0xE3BE1,   /* YFIR6_VAL */
      0xF5C56,   /* YFIR7_VAL */
      0x0EC23,   /* YFIR8_VAL */
      0x0FCA6,   /* YFIR9_VAL */
      0xFD5C8,   /* YFIR10_VAL */
      0xF208D,   /* YFIR11_VAL */
      0xFB369,   /* YFIR12_VAL */
      0x0A8C2,   /* YFIR13_VAL */
      0x0A0FE,   /* YFIR14_VAL */
      0xFA304,   /* YFIR15_VAL */
      0xF455F,   /* YFIR16_VAL */
      0x0CEC6,   /* YFIR17_VAL */
      0xFA2BC    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x575C7},          /* y_iir1_filter[1] */
   0x96,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   7,                  /* hybal_shft */
   {0x7AC3, 0x8310, 0x2593, 0x830D, 0x760C},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x50C8, 0xDB4A, 0xFED1, 0xFC40, 0x0095, 0xFEC9, 0xFFEF, 0xFF3B,
      0xFFD7, 0xFF6E, 0xFFCB, 0xFF90, 0xFFD7, 0xFFB2, 0xFFE4, 0xFFC2,
      0xFFE7, 0xFFD3, 0xFFF8, 0xFFE1, 0xFFF3, 0xFFE3, 0xFFF9, 0xFFEC,
      0xFFFC, 0xFFEE, 0xFFF8, 0xFFF0, 0xFFFC, 0xFFF1, 0xFFF9, 0xFFF6,
      0xFFFD, 0xFFF3, 0xFFFC, 0xFFF6, 0xFFFD, 0xFFFA, 0x0001, 0xFFFB,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x41ED, 0xFB09, 0x0058, 0xFE29, 0xFFDB, 0xFF4D, 0xFFC6, 0xFF8C,
      0xFFCA, 0xFFAB, 0xFFD0, 0xFFC2, 0xFFDB, 0xFFD1, 0xFFE6, 0xFFDE,
      0xFFEF, 0xFFE7, 0xFFF5, 0xFFED, 0xFFF6, 0xFFF2, 0xFFF9, 0xFFF4,
      0xFFFA, 0xFFF5, 0xFFF8, 0xFFF7, 0xFFF9, 0xFFF7, 0xFFFA, 0xFFF8,
      0xFFFB, 0xFFF9, 0xFFFD, 0xFFFA, 0xFFFE, 0xFFFC, 0x0000, 0xFFFE,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,               /* cic_dec_shft */
   0x79AD,           /* asrc_int_scale */
   0x0B09,           /* asrc_dec_scale */
   2,               /* vtx_pg */
   0,               /* vrx_pg */
   0,                /* hpf_en */
   6,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_NEW_ZEALAND_3239_H */
