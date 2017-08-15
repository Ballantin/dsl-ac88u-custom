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
*       SLIC = Zarlink L9540            
*       R1 = 270 ohms
*       R2 = 910 ohms
*       C1 = 120 nano Farads 
*       DLP = -10dB
*       ELP = -4dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 25 hertz
*       Ringing amplitude = 45Vrms
*
*    Flexicalc Version: 3.8
*
****************************************************************************/

#ifndef FLEXICALC_FINLAND_9540_H
#define FLEXICALC_FINLAND_9540_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_FINLAND
#define flexicalcFINLANDArchive9540 ((const APM6838_FLEXICALC_CFG*) NULL)
#else
const APM6838_FLEXICALC_CFG flexicalcFINLANDArchive9540[] =
{
{
   0x9540,           /* Slic type: L9540 */
   45,               /* Ring Voltage (RMS) */
   -10,               /* DLP - Decode Level Point (receive loss) in dB */
   -4,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0007,           /* eq_rx_shft */
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
      0x3D764,   /* YFIR1_VAL */
      0x855E8,   /* YFIR2_VAL */
      0x25095,   /* YFIR3_VAL */
      0x29EFD,   /* YFIR4_VAL */
      0xFD091,   /* YFIR5_VAL */
      0xE7244,   /* YFIR6_VAL */
      0xF4B29,   /* YFIR7_VAL */
      0x0B317,   /* YFIR8_VAL */
      0x10254,   /* YFIR9_VAL */
      0x036C4,   /* YFIR10_VAL */
      0xF6DF4,   /* YFIR11_VAL */
      0xF751E,   /* YFIR12_VAL */
      0x0112F,   /* YFIR13_VAL */
      0x0862C,   /* YFIR14_VAL */
      0x072BE,   /* YFIR15_VAL */
      0xFDF37,   /* YFIR16_VAL */
      0xE943C,   /* YFIR17_VAL */
      0x0EC92    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,            /* yfltr_gain */
   {0x78C7F},          /* y_iir1_filter[1] */
   0xA6,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   5,                  /* hybal_shft */
   {0xF135, 0xB8A4, 0x61C4, 0x9A12, 0x4BEF},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,                  /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7CF6, 0x01EB, 0xFBA4, 0xFFB4, 0xFD16, 0xFED1, 0xFD8D, 0xFE84,
      0xFE02, 0xFE9F, 0xFE66, 0xFECC, 0xFEC5, 0xFF06, 0xFF12, 0xFF47,
      0xFF4D, 0xFF75, 0xFF7E, 0xFF9F, 0xFF96, 0xFFB5, 0xFFAB, 0xFFC1,
      0xFFB5, 0xFFC7, 0xFFBB, 0xFFC8, 0xFFC5, 0xFFCC, 0xFFC8, 0xFFD0,
      0xFFD4, 0xFFDB, 0xFFDA, 0xFFE8, 0xFFE5, 0xFFF1, 0xFFF1, 0xFFFC,
      0xFFF9, 0x0004, 0x0004, 0x0004, 0x0003, 0x0003, 0x0003, 0x0002,
      0x0002, 0x0002, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x5F17, 0x2989, 0x0887, 0x03DB, 0xFF58, 0xFFB4, 0xFE7B, 0xFF0C,
      0xFE94, 0xFF0C, 0xFED8, 0xFF2C, 0xFF13, 0xFF57, 0xFF4E, 0xFF7A,
      0xFF7A, 0xFFA3, 0xFF99, 0xFFBA, 0xFFAE, 0xFFCD, 0xFFBD, 0xFFC8,
      0xFFC3, 0xFFD3, 0xFFC9, 0xFFD5, 0xFFCC, 0xFFD5, 0xFFD2, 0xFFDB,
      0xFFDA, 0xFFE4, 0xFFE2, 0xFFEC, 0xFFEB, 0xFFF5, 0xFFF2, 0xFFFE,
      0xFFFA, 0x0000, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0003,           /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x4F7B,           /* asrc_int_scale */
   0x0C0E,           /* asrc_dec_scale */
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

#endif  /* FLEXICALC_FINLAND_9540_H */
