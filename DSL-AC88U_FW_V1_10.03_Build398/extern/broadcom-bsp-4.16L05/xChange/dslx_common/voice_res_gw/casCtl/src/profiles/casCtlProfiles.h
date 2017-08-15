/***************************************************************************
*    Copyright © 2000-2007 Broadcom                                        
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
*    Filename: casCtlProfiles.h
*
****************************************************************************
*    Description:
*
*     This file contains the public declarations for CAS profiles
*
****************************************************************************/

#ifndef CAS_PROFILES_H
#define CAS_PROFILES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <casCtl.h>
#include <casCtlStateMachine.h>
#include <vrgCountry.h>

/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */


#if VRG_COUNTRY_CFG_AUSTRALIA
   extern const CASSTM gCasCtlRingPatterns_AUSTRALIA[];
#else
   #define gCasCtlRingPatterns_AUSTRALIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_AUSTRIA
   extern const CASSTM gCasCtlRingPatterns_AUSTRIA[];
#else
   #define gCasCtlRingPatterns_AUSTRIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_BELGIUM
   extern const CASSTM gCasCtlRingPatterns_BELGIUM[];
#else
   #define gCasCtlRingPatterns_BELGIUM ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_BRAZIL
   extern const CASSTM gCasCtlRingPatterns_BRAZIL[];
#else
   #define gCasCtlRingPatterns_BRAZIL ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_CHILE
   extern const CASSTM gCasCtlRingPatterns_CHILE[];
#else
   #define gCasCtlRingPatterns_CHILE ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_CHINA
   extern const CASSTM gCasCtlRingPatterns_CHINA[];
#else
   #define gCasCtlRingPatterns_CHINA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_CZECH
   extern const CASSTM gCasCtlRingPatterns_CZECH[];
#else
   #define gCasCtlRingPatterns_CZECH ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_DENMARK
   extern const CASSTM gCasCtlRingPatterns_DENMARK[];
#else
   #define gCasCtlRingPatterns_DENMARK ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_ETSI
   extern const CASSTM gCasCtlRingPatterns_ETSI[];
#else
   #define gCasCtlRingPatterns_ETSI ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_FINLAND
   extern const CASSTM gCasCtlRingPatterns_FINLAND[];
#else
   #define gCasCtlRingPatterns_FINLAND ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_FRANCE
   extern const CASSTM gCasCtlRingPatterns_FRANCE[];
#else
   #define gCasCtlRingPatterns_FRANCE ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_GERMANY
   extern const CASSTM gCasCtlRingPatterns_GERMANY[];
#else
   #define gCasCtlRingPatterns_GERMANY ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_HUNGARY
   extern const CASSTM gCasCtlRingPatterns_HUNGARY[];
#else
   #define gCasCtlRingPatterns_HUNGARY ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_INDIA
   extern const CASSTM gCasCtlRingPatterns_INDIA[];
#else
   #define gCasCtlRingPatterns_INDIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_IRELAND
   extern const CASSTM gCasCtlRingPatterns_IRELAND[];
#else
   #define gCasCtlRingPatterns_IRELAND ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_ITALY
   extern const CASSTM gCasCtlRingPatterns_ITALY[];
#else
   #define gCasCtlRingPatterns_ITALY ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_NETHERLANDS
   extern const CASSTM gCasCtlRingPatterns_NETHERLANDS[];
#else
   #define gCasCtlRingPatterns_NETHERLANDS ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_JAPAN
   extern const CASSTM gCasCtlRingPatterns_JAPAN[];
#else
   #define gCasCtlRingPatterns_JAPAN ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_NORTH_AMERICA
   extern const CASSTM gCasCtlRingPatterns_NORTH_AMERICA[];
#else
   #define gCasCtlRingPatterns_NORTH_AMERICA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_NEW_ZEALAND
   extern const CASSTM gCasCtlRingPatterns_NEW_ZEALAND[];
#else
   #define gCasCtlRingPatterns_NEW_ZEALAND ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_POLAND
   extern const CASSTM gCasCtlRingPatterns_POLAND[];
#else
   #define gCasCtlRingPatterns_POLAND ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_ROMANIA
   extern const CASSTM gCasCtlRingPatterns_ROMANIA[];
#else
   #define gCasCtlRingPatterns_ROMANIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_SLOVAKIA
   extern const CASSTM gCasCtlRingPatterns_SLOVAKIA[];
#else
   #define gCasCtlRingPatterns_SLOVAKIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_SLOVENIA
   extern const CASSTM gCasCtlRingPatterns_SLOVENIA[];
#else
   #define gCasCtlRingPatterns_SLOVENIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_SPAIN
   extern const CASSTM gCasCtlRingPatterns_SPAIN[];
#else
   #define gCasCtlRingPatterns_SPAIN ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_SWEDEN
   extern const CASSTM gCasCtlRingPatterns_SWEDEN[];
#else
   #define gCasCtlRingPatterns_SWEDEN ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_NORWAY
   extern const CASSTM gCasCtlRingPatterns_NORWAY[];
#else
   #define gCasCtlRingPatterns_NORWAY ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_SWITZERLAND
   extern const CASSTM gCasCtlRingPatterns_SWITZERLAND[];
#else
   #define gCasCtlRingPatterns_SWITZERLAND ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_TR57
   extern const CASSTM gCasCtlRingPatterns_TR57[];
#else
   #define gCasCtlRingPatterns_TR57 ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_UK
   extern const CASSTM gCasCtlRingPatterns_UK[];
#else
   #define gCasCtlRingPatterns_UK ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_TAIWAN
   extern const CASSTM gCasCtlRingPatterns_TAIWAN[];
#else
   #define gCasCtlRingPatterns_TAIWAN ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_UNITED_ARAB_EMIRATES
   extern const CASSTM gCasCtlRingPatterns_UNITED_ARAB_EMIRATES[];
#else
   #define gCasCtlRingPatterns_UNITED_ARAB_EMIRATES ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_MEXICO
   extern const CASSTM gCasCtlRingPatterns_MEXICO[];
#else
   #define gCasCtlRingPatterns_MEXICO ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_CYPRUS
   extern const CASSTM gCasCtlRingPatterns_CYPRUS[];
#else
   #define gCasCtlRingPatterns_CYPRUS ((const CASSTM*) NULL )
#endif



/* ---- Function Prototypes ---------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif  /* CAS_PROFILES_H  */

