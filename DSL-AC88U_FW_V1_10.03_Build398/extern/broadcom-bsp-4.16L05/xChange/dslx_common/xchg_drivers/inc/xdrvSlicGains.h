/***************************************************************************
*    Copyright © 2000-2008 Broadcom                                        
*                                                                           
*    This program is the proprietary software of Broadcom and/or
*    its licensors, and may only be used, duplicated, modified or           
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized   
*    License").  Except as set forth in an Authorized License, Broadcom    
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
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,          
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
*    Filename: xdrvSlicGains.h
*
****************************************************************************
*    Description:
*
*     Public defines for Tx/Rx gain values for supported countries. 
*
****************************************************************************/

#ifndef XDRV_SLIC_GAINS_H
#define XDRV_SLIC_GAINS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

/* Country specific Rx gains */
#define FXS_AUSTRALIA_Profile_Dlp            -9
#define FXS_AUSTRIA_Profile_Dlp              -6
#define FXS_BELGIUM_Profile_Dlp              -6
#define FXS_BRAZIL_Profile_Dlp               -11
#define FXS_CHILE_Profile_Dlp                -11
#define FXS_CHINA_Profile_Dlp                -4
#define FXS_CZECH_Profile_Dlp                -6
#define FXS_DENMARK_Profile_Dlp              -11
#define FXS_ETSI_Profile_Dlp                 -6
#define FXS_FINLAND_Profile_Dlp              -10
#define FXS_FRANCE_Profile_Dlp               -10
#define FXS_GERMANY_Profile_Dlp              -10
#define FXS_HUNGARY_Profile_Dlp              -11
#define FXS_INDIA_Profile_Dlp                -4
#define FXS_IRELAND_Profile_Dlp              -6
#define FXS_ITALY_Profile_Dlp                -3
#define FXS_JAPAN_Profile_Dlp                -3
#define FXS_MEXICO_Profile_Dlp               -7
#define FXS_NETHERLANDS_Profile_Dlp          -10
#define FXS_NEW_ZEALAND_Profile_Dlp          -4
#define FXS_NORTH_AMERICA_Profile_Dlp        -4
#define FXS_NORWAY_Profile_Dlp               -4
#define FXS_POLAND_Profile_Dlp               -6
#define FXS_ROMANIA_Profile_Dlp              -6
#define FXS_SLOVAKIA_Profile_Dlp             -6
#define FXS_SLOVENIA_Profile_Dlp             -6
#define FXS_SPAIN_Profile_Dlp                -4
#define FXS_SWEDEN_Profile_Dlp               -10
#define FXS_SWITZERLAND_Profile_Dlp          -11
#define FXS_TAIWAN_Profile_Dlp               -4
#define FXS_TR57_Profile_Dlp                 -3
#define FXS_UK_Profile_Dlp                   -9
#define FXS_UNITED_ARAB_EMIRATES_Profile_Dlp -6
#define FXS_CYPRUS_Profile_Dlp               -7

/* Country specific Tx gains */
#define FXS_AUSTRALIA_Profile_Tx_Gain            -3
#define FXS_AUSTRIA_Profile_Tx_Gain              -4
#define FXS_BELGIUM_Profile_Tx_Gain              -3
#define FXS_BRAZIL_Profile_Tx_Gain               -4
#define FXS_CHILE_Profile_Tx_Gain                -4
#define FXS_CHINA_Profile_Tx_Gain                -2
#define FXS_CZECH_Profile_Tx_Gain                -4
#define FXS_DENMARK_Profile_Tx_Gain              -4
#define FXS_ETSI_Profile_Tx_Gain                 -4
#define FXS_FINLAND_Profile_Tx_Gain              -4
#define FXS_FRANCE_Profile_Tx_Gain               -4
#define FXS_GERMANY_Profile_Tx_Gain              -4
#define FXS_HUNGARY_Profile_Tx_Gain              -4
#define FXS_INDIA_Profile_Tx_Gain                -2
#define FXS_IRELAND_Profile_Tx_Gain              -4
#define FXS_ITALY_Profile_Tx_Gain                -3
#define FXS_JAPAN_Profile_Tx_Gain                -3
#define FXS_MEXICO_Profile_Tx_Gain                0
#define FXS_NETHERLANDS_Profile_Tx_Gain          -4
#define FXS_NEW_ZEALAND_Profile_Tx_Gain          -2
#define FXS_NORTH_AMERICA_Profile_Tx_Gain        -2
#define FXS_NORWAY_Profile_Tx_Gain               -2
#define FXS_POLAND_Profile_Tx_Gain               -4
#define FXS_ROMANIA_Profile_Tx_Gain              -4
#define FXS_SLOVAKIA_Profile_Tx_Gain             -4
#define FXS_SLOVENIA_Profile_Tx_Gain             -4
#define FXS_SPAIN_Profile_Tx_Gain                -2
#define FXS_SWEDEN_Profile_Tx_Gain               -4
#define FXS_SWITZERLAND_Profile_Tx_Gain          -4
#define FXS_TAIWAN_Profile_Tx_Gain               -2
#define FXS_TR57_Profile_Tx_Gain                 -2
#define FXS_UK_Profile_Tx_Gain                   -4
#define FXS_UNITED_ARAB_EMIRATES_Profile_Tx_Gain -4
#define FXS_CYPRUS_Profile_Tx_Gain               -0

#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_SLIC_GAINS_H  */
