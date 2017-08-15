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
*    Filename: vrgCountryCfgCustom.h
*
****************************************************************************
*    Description:
*
*     Overrides default build options in vrgCountryCfg.h
*
****************************************************************************/


#ifndef VRG_COUNTRY_CFG_CUSTOM_H
#define VRG_COUNTRY_CFG_CUSTOM_H

/*
 * The version of this file which exists in the \cfginc\
 * directory should be EMPTY.
 *
 * ALL customizations should go into a copy of this file located in the
 * appropriate custom build configuration directory:
 *
 * The prescence of an xxxCfgCustom.h file in the custom configuration directory
 * precludes the inclusion of the same file from the default custom directory.
 *
 * The empty versions of the files are provided so that if no defaults need
 * to be overridden, then no copy of the file has to exist in the custom
 * build configuration directory.
 *
 * This comment block may be removed from copies of this file located in
 * a custom build configuration directory.
 */

#ifdef BRCM_VRG_COUNTRY_CFG_AUSTRALIA
   #define VRG_COUNTRY_CFG_AUSTRALIA      1
#else
   #define VRG_COUNTRY_CFG_AUSTRALIA      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_AUSTRIA
   #define VRG_COUNTRY_CFG_AUSTRIA      1
#else
   #define VRG_COUNTRY_CFG_AUSTRIA      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_BELGIUM
   #define VRG_COUNTRY_CFG_BELGIUM      1
#else
   #define VRG_COUNTRY_CFG_BELGIUM      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_BRAZIL
   #define VRG_COUNTRY_CFG_BRAZIL      1
#else
   #define VRG_COUNTRY_CFG_BRAZIL      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_CHILE
   #define VRG_COUNTRY_CFG_CHILE      1
#else
   #define VRG_COUNTRY_CFG_CHILE      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_CHINA
   #define VRG_COUNTRY_CFG_CHINA      1
#else
   #define VRG_COUNTRY_CFG_CHINA      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_CZECH
   #define VRG_COUNTRY_CFG_CZECH      1
#else
   #define VRG_COUNTRY_CFG_CZECH      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_DENMARK
   #define VRG_COUNTRY_CFG_DENMARK      1
#else
   #define VRG_COUNTRY_CFG_DENMARK      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_ETSI
   #define VRG_COUNTRY_CFG_ETSI      1
#else
   #define VRG_COUNTRY_CFG_ETSI      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_FINLAND
   #define VRG_COUNTRY_CFG_FINLAND      1
#else
   #define VRG_COUNTRY_CFG_FINLAND      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_FRANCE
   #define VRG_COUNTRY_CFG_FRANCE      1
#else
   #define VRG_COUNTRY_CFG_FRANCE      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_GERMANY
   #define VRG_COUNTRY_CFG_GERMANY      1
#else
   #define VRG_COUNTRY_CFG_GERMANY      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_HUNGARY
   #define VRG_COUNTRY_CFG_HUNGARY      1
#else
   #define VRG_COUNTRY_CFG_HUNGARY      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_INDIA
   #define VRG_COUNTRY_CFG_INDIA      1
#else
   #define VRG_COUNTRY_CFG_INDIA      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_IRELAND
   #define VRG_COUNTRY_CFG_IRELAND      1
#else
   #define VRG_COUNTRY_CFG_IRELAND      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_ITALY
   #define VRG_COUNTRY_CFG_ITALY      1
#else
   #define VRG_COUNTRY_CFG_ITALY      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_JAPAN
   #define VRG_COUNTRY_CFG_JAPAN      1
#else
   #define VRG_COUNTRY_CFG_JAPAN      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_NETHERLANDS
   #define VRG_COUNTRY_CFG_NETHERLANDS      1
#else
   #define VRG_COUNTRY_CFG_NETHERLANDS      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_NEW_ZEALAND
   #define VRG_COUNTRY_CFG_NEW_ZEALAND      1
#else
   #define VRG_COUNTRY_CFG_NEW_ZEALAND      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_NORTH_AMERICA
   #define VRG_COUNTRY_CFG_NORTH_AMERICA      1
#else
   #define VRG_COUNTRY_CFG_NORTH_AMERICA      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_POLAND
   #define VRG_COUNTRY_CFG_POLAND      1
#else
   #define VRG_COUNTRY_CFG_POLAND      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_ROMANIA
   #define VRG_COUNTRY_CFG_ROMANIA      1
#else
   #define VRG_COUNTRY_CFG_ROMANIA      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_SLOVAKIA
   #define VRG_COUNTRY_CFG_SLOVAKIA      1
#else
   #define VRG_COUNTRY_CFG_SLOVAKIA      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_SLOVENIA
   #define VRG_COUNTRY_CFG_SLOVENIA      1
#else
   #define VRG_COUNTRY_CFG_SLOVENIA      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_SPAIN
   #define VRG_COUNTRY_CFG_SPAIN      1
#else
   #define VRG_COUNTRY_CFG_SPAIN      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_SWEDEN
   #define VRG_COUNTRY_CFG_SWEDEN      1
#else
   #define VRG_COUNTRY_CFG_SWEDEN      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_NORWAY
   #define VRG_COUNTRY_CFG_NORWAY      1
#else
   #define VRG_COUNTRY_CFG_NORWAY      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_TAIWAN
   #define VRG_COUNTRY_CFG_TAIWAN      1
#else
   #define VRG_COUNTRY_CFG_TAIWAN      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_SWITZERLAND
   #define VRG_COUNTRY_CFG_SWITZERLAND      1
#else
   #define VRG_COUNTRY_CFG_SWITZERLAND      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_TR57
   #define VRG_COUNTRY_CFG_TR57      1
#else
   #define VRG_COUNTRY_CFG_TR57      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_UK
   #define VRG_COUNTRY_CFG_UK      1
#else
   #define VRG_COUNTRY_CFG_UK      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_UNITED_ARAB_EMIRATES
   #define VRG_COUNTRY_CFG_UNITED_ARAB_EMIRATES      1
#else
   #define VRG_COUNTRY_CFG_UNITED_ARAB_EMIRATES      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_MEXICO
   #define VRG_COUNTRY_CFG_MEXICO      1
#else
   #define VRG_COUNTRY_CFG_MEXICO      0
#endif

#ifdef BRCM_VRG_COUNTRY_CFG_CYPRUS
   #define VRG_COUNTRY_CFG_CYPRUS      1
#else
   #define VRG_COUNTRY_CFG_CYPRUS      0
#endif
#endif   /* VRG_COUNTRY_CFG_CUSTOM_H */
