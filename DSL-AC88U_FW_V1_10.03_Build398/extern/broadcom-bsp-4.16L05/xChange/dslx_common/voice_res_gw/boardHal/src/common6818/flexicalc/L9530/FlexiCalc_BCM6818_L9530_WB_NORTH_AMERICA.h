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

*       DLP = -4dB

*       ELP = -2dB

*       HWDACgain = 0dB

*       HW_impedance = 600 ohms

*       Protection resistor = 50 ohms

*       Ringing frequency = 20 hertz

*       Ringing amplitude = 57Vrms

*

*    Flexicalc Version: 3.7

*

****************************************************************************/



#ifndef FLEXICALC_WB_NORTH_AMERICA_9530_H

#define FLEXICALC_WB_NORTH_AMERICA_9530_H



#ifdef __cplusplus

extern "C" {

#endif



/****************************************************************************

** Flexicalc Values Structure                                              **

****************************************************************************/



#if !VRG_COUNTRY_CFG_NORTH_AMERICA

#define flexicalcWBNORTH_AMERICAArchive9530 ((const APM6818_FLEXICALC_CFG*) NULL)

#else

const APM6818_FLEXICALC_CFG flexicalcWBNORTH_AMERICAArchive9530[] =

{

{

   0x9530,           /* Slic type: L9530 */

   57,               /* Ring Voltage (RMS) */

   -4,               /* DLP - Decode Level Point (receive loss) in dB */

   -2,               /* ELP - Encode Level Point (transmitt loss) in dB */

   0x0007,           /* eq_rx_shft */

   0x000B,           /* eq_tx_shft */

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

      0xDA089,   /* YFIR1_VAL */

      0x38A74,   /* YFIR2_VAL */

      0xEC24D,   /* YFIR3_VAL */

      0x0B85A,   /* YFIR4_VAL */

      0xFE15D,   /* YFIR5_VAL */

      0xF0C35,   /* YFIR6_VAL */

      0x04208,   /* YFIR7_VAL */

      0xFFA13,   /* YFIR8_VAL */

      0xFD473,   /* YFIR9_VAL */

      0x087AF,   /* YFIR10_VAL */

      0x010EB,   /* YFIR11_VAL */

      0xFC9F2,   /* YFIR12_VAL */

      0x01D18,   /* YFIR13_VAL */

      0xFC835,   /* YFIR14_VAL */

      0xFDB4D,   /* YFIR15_VAL */

      0x03937,   /* YFIR16_VAL */

      0x015F5,   /* YFIR17_VAL */

      0xFE829    /* YFIR18_VAL */

   },

   0x06,               /* y_fir_filter_shift */

   0x7FFFF,            /* yfltr_gain */

   {0x7EF9B},          /* y_iir1_filter[1] */

   0x66,               /* y_iir1_filter_shift */



   /* Hybrid Balance Coefficients */

   6,                  /* hybal_shft */

   {0x6730, 0xB0F2, 0x0954, 0x423F, 0xD612},    /* hybal_audio_fir[5] */

   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},    /* hybal_pm_fir[5] */

   1,                  /* hybal_en */



   {  /* Rx Equalization Coefficents */

      0x40F1, 0xFEBF, 0x006F, 0xFFBF, 0x0036, 0xFFE2, 0x0017, 0xFFF1,

      0x000E, 0xFFF8, 0x0009, 0xFFFC, 0x0005, 0xFFFE, 0x0004, 0x0000,

      0x0004, 0x0000, 0x0003, 0x0000, 0x0003, 0x0000, 0x0003, 0x0000,

      0x0003, 0x0000, 0x0002, 0x0000, 0x0002, 0x0000, 0x0002, 0x0000,

      0x0001, 0x0001, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,

      0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000

   },



   {  /* Tx Equalization Coefficents */

      0x40BB, 0xFF06, 0x0060, 0xFFC9, 0x0033, 0xFFE6, 0x0016, 0xFFF5,

      0x000D, 0xFFFC, 0x0007, 0xFFFE, 0x0005, 0x0000, 0x0004, 0x0002,

      0x0004, 0x0001, 0x0004, 0x0000, 0x0004, 0x0000, 0x0003, 0x0000,

      0x0003, 0x0001, 0x0002, 0x0001, 0x0002, 0x0001, 0x0002, 0x0000,

      0x0002, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,

      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

      0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000

   },



   0x0004,           /* cic_int_shft */

   0x0004,           /* cic_dec_shft */

   0x7066,           /* asrc_int_scale */

   0x1440,           /* asrc_dec_scale */

   2,                /* vtx_pg */

   0,                /* vrx_pg */

   0,                /* hpf_en */

   4,                /* hybal_smpl_offset */



}

};

#endif



#ifdef __cplusplus

}

#endif



#endif  /* FLEXICALC_WB_NORTH_AMERICA_9530_H */

