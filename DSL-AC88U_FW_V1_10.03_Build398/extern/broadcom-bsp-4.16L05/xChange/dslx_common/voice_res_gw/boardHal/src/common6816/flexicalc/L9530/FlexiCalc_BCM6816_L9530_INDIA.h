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
*       SLIC = Zarlink L9530            
*       R1 = 600 ohms
*       R2 = 0 ohms
*       C1 = 0 nano Farads
*       DLP = -4dB
*       ELP = -4dB
*       HWDACgain = 0dB
*       HW_impedance = 600 ohms
*       Protection resistor = 50 ohms
*       Ringing frequency = 20 hertz
*       Ringing amplitude = 57Vrms
*
*    Flexicalc Version: 3.3
*
****************************************************************************/

#ifndef FLEXICALC_INDIA_9530_H
#define FLEXICALC_INDIA_9530_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
** Flexicalc Values Structure                                              **
****************************************************************************/

#if !VRG_COUNTRY_CFG_INDIA
#define flexicalcINDIAArchive9530 ((const APM6816_FLEXICALC_CFG*) NULL)
#else
const APM6816_FLEXICALC_CFG flexicalcINDIAArchive9530[] =
{
{
   0x9530,               /* Slic type: L9530 */
   57,               /* Ring Voltage (RMS) */
   -4,                   /* DLP - Decode Level Point (receive loss) in dB */
   -4,               /* ELP - Encode Level Point (transmitt loss) in dB */
   0x0006,               /* eq_rx_shft */
   0x000A,               /* eq_tx_shft */
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
      0xBAFEF,   /* YFIR1_VAL */
      0x73C98,   /* YFIR2_VAL */
      0xCC4DB,   /* YFIR3_VAL */
      0x1921E,   /* YFIR4_VAL */
      0xF6BDF,   /* YFIR5_VAL */
      0xE29B7,   /* YFIR6_VAL */
      0x0DF50,   /* YFIR7_VAL */
      0x0022F,   /* YFIR8_VAL */
      0xFB7EE,   /* YFIR9_VAL */
      0x11578,   /* YFIR10_VAL */
      0xFE8ED,   /* YFIR11_VAL */
      0xF7427,   /* YFIR12_VAL */
      0x04B3F,   /* YFIR13_VAL */
      0xF96B9,   /* YFIR14_VAL */
      0xFC4AA,   /* YFIR15_VAL */
      0x0AC53,   /* YFIR16_VAL */
      0xFD5F1,   /* YFIR17_VAL */
      0xFED3C    /* YFIR18_VAL */
   },
   0x05,               /* y_fir_filter_shift */
   0x7FFFF,               /* yfltr_gain */
   {0x7DCE7},          /* y_iir1_filter[1] */
   0x76,               /* y_iir1_filter_shift */

   /* Hybrid Balance Coefficients */
   6,               /* hybal_shft */
   {0x3565, 0x884F, 0x57C7, 0xE425, 0x3228},    /* hybal_audio_fir[5] */
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */
   1,               /* hybal_en */

   {  /* Rx Equalization Coefficents */
      0x7F45, 0xFCED, 0xFE16, 0xFDB2, 0xFDF5, 0xFE02, 0xFE2A, 0xFE40,
      0xFE71, 0xFE8D, 0xFEB1, 0xFED3, 0xFEF7, 0xFF13, 0xFF37, 0xFF53,
      0xFF6B, 0xFF7E, 0xFF97, 0xFFA3, 0xFFAB, 0xFFB6, 0xFFC0, 0xFFC2,
      0xFFC6, 0xFFCA, 0xFFCA, 0xFFCA, 0xFFD0, 0xFFCF, 0xFFCE, 0xFFD4,
      0xFFDA, 0xFFDC, 0xFFE2, 0xFFE7, 0xFFEA, 0xFFF0, 0xFFF6, 0xFFF9,
      0xFFFB, 0x0004, 0x0003, 0x0003, 0x0003, 0x0002, 0x0002, 0x0002,
      0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
      0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   {  /* Tx Equalization Coefficents */
      0x7FD2, 0xFDEA, 0xFF0C, 0xFEA7, 0xFEDF, 0xFEDD, 0xFEFE, 0xFF0B,
      0xFF26, 0xFF37, 0xFF4F, 0xFF61, 0xFF77, 0xFF86, 0xFF9A, 0xFFA6,
      0xFFB7, 0xFFBE, 0xFFCA, 0xFFCF, 0xFFD5, 0xFFD8, 0xFFDB, 0xFFDD,
      0xFFDF, 0xFFE1, 0xFFE2, 0xFFE4, 0xFFE5, 0xFFE6, 0xFFE9, 0xFFE9,
      0xFFEC, 0xFFED, 0xFFF1, 0xFFF2, 0xFFF6, 0xFFF8, 0xFFFB, 0xFFFD,
      0xFFFE, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
   },

   0x0004,               /* cic_int_shft */
   0x0004,           /* cic_dec_shft */
   0x70D2,           /* asrc_int_scale */
   0x0AB9,           /* asrc_dec_scale */
   1,                /* vtx_pg */
   0,               /* vrx_pg */
   1,               /* hpf_en */
   0,                /* hybal_smpl_offset */

}
};
#endif

#ifdef __cplusplus
}
#endif

#endif  /* FLEXICALC_INDIA_9530_H */
