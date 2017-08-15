/****************************************************************************
*
* <:copyright-BRCM:2014:DUAL/GPL:standard
* 
*    Copyright (c) 2014 Broadcom
*    All Rights Reserved
* 
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed
* to you under the terms of the GNU General Public License version 2
* (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
* with the following added to such license:
* 
*    As a special exception, the copyright holders of this software give
*    you permission to link this software with independent modules, and
*    to copy and distribute the resulting executable under terms of your
*    choice, provided that you also meet, for each linked independent
*    module, the terms and conditions of the license of that module.
*    An independent module is a module which is not derived from this
*    software.  The special exception does not apply to any modifications
*    of the software.
* 
* Not withstanding the above, under no circumstances may you combine
* this software in any way with any other Broadcom software provided
* under a license other than the GPL, without Broadcom's express prior
* written consent.
* 
* :>
*
****************************************************************************
*    Filename: vrgCountryCfg.h
*
****************************************************************************
*    Description:
*
*     This file contains a list of supported countries for voice residential
*     gateway applications.
*
****************************************************************************/

#ifndef VRG_COUNTRY_CFG_H
#define VRG_COUNTRY_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BRCM_VRG_COUNTRY_CFG_NON_STATIC_PROFILE
   /* Explicitly define VRG_COUNTRY_CFG_STATIC_PROFILE = 1*/
   #ifdef VRG_COUNTRY_CFG_STATIC_PROFILE
      #undef VRG_COUNTRY_CFG_STATIC_PROFILE 
   #endif /* VRG_COUNTRY_CFG_STATIC_PROFILE */

   #define VRG_COUNTRY_CFG_STATIC_PROFILE 1
#else
   /* Explicitly define VRG_COUNTRY_CFG_STATIC_PROFILE = 0*/
   #ifdef VRG_COUNTRY_CFG_STATIC_PROFILE
      #undef VRG_COUNTRY_CFG_STATIC_PROFILE 
   #endif /* VRG_COUNTRY_CFG_STATIC_PROFILE */

   #define VRG_COUNTRY_CFG_STATIC_PROFILE 0
#endif /* BRCM_VRG_COUNTRY_CFG_NON_STATIC_PROFILE */


#ifndef BRCM_VRG_COUNTRY_CFG_CUSTOM_PROFILES
   /* Explicitly define VRG_COUNTRY_CFG_ALL_PROFILES = 1*/
   #undef VRG_COUNTRY_CFG_ALL_PROFILES 
   #define VRG_COUNTRY_CFG_ALL_PROFILES 1
#else
   /* Explicitly define VRG_COUNTRY_CFG_ALL_PROFILES = 0*/
   #undef VRG_COUNTRY_CFG_ALL_PROFILES
   #define VRG_COUNTRY_CFG_ALL_PROFILES 0
#endif /* BRCM_VRG_COUNTRY_CFG_CUSTOM_PROFILES */

#if !VRG_COUNTRY_CFG_ALL_PROFILES
/* ------------ Includes -------------------------------------------------- */
#include <vrgCountryCfgCustom.h>
/* ---- Constants and Types ---------------------------------------------- */

#else
/* The following define controls which countries are supported */

#ifndef VRG_COUNTRY_CFG_AUSTRALIA
#define VRG_COUNTRY_CFG_AUSTRALIA      1
#endif

#ifndef VRG_COUNTRY_CFG_AUSTRIA
#define VRG_COUNTRY_CFG_AUSTRIA        0
#endif

#ifndef VRG_COUNTRY_CFG_BELGIUM
#define VRG_COUNTRY_CFG_BELGIUM        1
#endif

#ifndef VRG_COUNTRY_CFG_BRAZIL
#define VRG_COUNTRY_CFG_BRAZIL         1
#endif

#ifndef VRG_COUNTRY_CFG_CHILE
#define VRG_COUNTRY_CFG_CHILE          1
#endif

#ifndef VRG_COUNTRY_CFG_CHINA
#define VRG_COUNTRY_CFG_CHINA          1
#endif

#ifndef VRG_COUNTRY_CFG_CZECH 
#define VRG_COUNTRY_CFG_CZECH          1
#endif

#ifndef VRG_COUNTRY_CFG_DENMARK
#define VRG_COUNTRY_CFG_DENMARK        1
#endif

#ifndef VRG_COUNTRY_CFG_ETSI
#define VRG_COUNTRY_CFG_ETSI           1
#endif

#ifndef VRG_COUNTRY_CFG_FINLAND
#define VRG_COUNTRY_CFG_FINLAND        1
#endif

#ifndef VRG_COUNTRY_CFG_FRANCE
#define VRG_COUNTRY_CFG_FRANCE         1
#endif

#ifndef VRG_COUNTRY_CFG_GERMANY
#define VRG_COUNTRY_CFG_GERMANY        1
#endif

#ifndef VRG_COUNTRY_CFG_HUNGARY
#define VRG_COUNTRY_CFG_HUNGARY        1
#endif

#ifndef VRG_COUNTRY_CFG_INDIA
#define VRG_COUNTRY_CFG_INDIA          1
#endif

#ifndef VRG_COUNTRY_CFG_IRELAND
#define VRG_COUNTRY_CFG_IRELAND        0
#endif

#ifndef VRG_COUNTRY_CFG_ITALY
#define VRG_COUNTRY_CFG_ITALY          1
#endif

#ifndef VRG_COUNTRY_CFG_JAPAN
#define VRG_COUNTRY_CFG_JAPAN          1
#endif

#ifndef VRG_COUNTRY_CFG_NETHERLANDS
#define VRG_COUNTRY_CFG_NETHERLANDS    1
#endif

#ifndef VRG_COUNTRY_CFG_NEW_ZEALAND
#define VRG_COUNTRY_CFG_NEW_ZEALAND    1
#endif

#ifndef VRG_COUNTRY_CFG_NORTH_AMERICA
#define VRG_COUNTRY_CFG_NORTH_AMERICA  1
#endif

#ifndef VRG_COUNTRY_CFG_POLAND
#define VRG_COUNTRY_CFG_POLAND         1
#endif

#ifndef VRG_COUNTRY_CFG_ROMANIA
#define VRG_COUNTRY_CFG_ROMANIA        0
#endif

#ifndef VRG_COUNTRY_CFG_SLOVAKIA
#define VRG_COUNTRY_CFG_SLOVAKIA       0
#endif

#ifndef VRG_COUNTRY_CFG_SLOVENIA
#define VRG_COUNTRY_CFG_SLOVENIA       0
#endif

#ifndef VRG_COUNTRY_CFG_SPAIN
#define VRG_COUNTRY_CFG_SPAIN          1
#endif

#ifndef VRG_COUNTRY_CFG_SWEDEN
#define VRG_COUNTRY_CFG_SWEDEN         1
#endif

#ifndef VRG_COUNTRY_CFG_NORWAY
#define VRG_COUNTRY_CFG_NORWAY         1
#endif

#ifndef VRG_COUNTRY_CFG_SWITZERLAND
#define VRG_COUNTRY_CFG_SWITZERLAND    1
#endif

#ifndef VRG_COUNTRY_CFG_TR57
#define VRG_COUNTRY_CFG_TR57           1
#endif

#ifndef VRG_COUNTRY_CFG_UK
#define VRG_COUNTRY_CFG_UK             1
#endif

#ifndef VRG_COUNTRY_CFG_TAIWAN
#define VRG_COUNTRY_CFG_TAIWAN             1
#endif

#ifndef VRG_COUNTRY_CFG_UNITED_ARAB_EMIRATES
#define VRG_COUNTRY_CFG_UNITED_ARAB_EMIRATES             1
#endif

#ifndef VRG_COUNTRY_CFG_MEXICO
#define VRG_COUNTRY_CFG_MEXICO             1
#endif

#ifndef VRG_COUNTRY_CFG_CYPRUS
#define VRG_COUNTRY_CFG_CYPRUS         1
#endif
#endif  /* if !VRG_COUNTRY_CFG_ALL_PROFILES */

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif  /* VRG_COUNTRY_CFG_H  */
