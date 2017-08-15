/***************************************************************************
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
*
*    Filename: countryArchive.h
*
****************************************************************************
*    Description:
*
*     This file provides a central list or archive of supported countries.
*     It is intended to be used to generate an enumerated type of countries.
*     It MUST be included by another wrapper header file which provides
*     the enum skeleton and macro to generate enumerated type names. The
*     enum macro that MUST be defined in the containing header file is
*     'COUNTRY_ARCHIVE_MAKE_NAME'. For example, the containing header file
*     should declare something similar to the following:
*
*        #define COUNTRY_ARCHIVE_MAKE_NAME( country )    MY_COUNTRY_##country
*
*        typedef enum
*        {
*           #include <countryArchive.h>
*
*        } MY_COUNTRY;
*
*
*     This example would generate a list of countries as follows:
*
*        enum
*        {
*           MY_COUNTRY_JAPAN,
*           MY_COUNTRY_GERMANY,
*           MY_COUNTRY_SWEDEN,
*           ...
*        } MY_COUNTRY;
*
*     Using this method allows the list of countries to be maintained in a
*     central location. However, modules are free to create their own country
*     name-space by providing a wrapper for this file.
*
****************************************************************************/

/*
** Include guards have been purposefully excluded. This header MUST be included
** by a wrapper header file which provides the inclusion protection.
*/

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

#ifndef COUNTRY_ARCHIVE_MAKE_NAME
   #error "COUNTRY_ARCHIVE_MAKE_NAME must be defined!"
#endif

#if VRG_COUNTRY_CFG_AUSTRALIA
COUNTRY_ARCHIVE_MAKE_NAME( AUSTRALIA     )
#endif
#if VRG_COUNTRY_CFG_BELGIUM
COUNTRY_ARCHIVE_MAKE_NAME( BELGIUM       )
#endif
#if VRG_COUNTRY_CFG_BRAZIL
COUNTRY_ARCHIVE_MAKE_NAME( BRAZIL        )
#endif
#if VRG_COUNTRY_CFG_CHILE
COUNTRY_ARCHIVE_MAKE_NAME( CHILE         )
#endif
#if VRG_COUNTRY_CFG_CHINA
COUNTRY_ARCHIVE_MAKE_NAME( CHINA         )
#endif
#if VRG_COUNTRY_CFG_CZECH
COUNTRY_ARCHIVE_MAKE_NAME( CZECH         )
#endif
#if VRG_COUNTRY_CFG_DENMARK
COUNTRY_ARCHIVE_MAKE_NAME( DENMARK       )
#endif
#if VRG_COUNTRY_CFG_ETSI
COUNTRY_ARCHIVE_MAKE_NAME( ETSI          )
#endif
#if VRG_COUNTRY_CFG_FINLAND
COUNTRY_ARCHIVE_MAKE_NAME( FINLAND       )
#endif
#if VRG_COUNTRY_CFG_FRANCE
COUNTRY_ARCHIVE_MAKE_NAME( FRANCE        )
#endif
#if VRG_COUNTRY_CFG_GERMANY
COUNTRY_ARCHIVE_MAKE_NAME( GERMANY       )
#endif
#if VRG_COUNTRY_CFG_HUNGARY
COUNTRY_ARCHIVE_MAKE_NAME( HUNGARY       )
#endif
#if VRG_COUNTRY_CFG_INDIA
COUNTRY_ARCHIVE_MAKE_NAME( INDIA         )
#endif
#if VRG_COUNTRY_CFG_ITALY
COUNTRY_ARCHIVE_MAKE_NAME( ITALY         )
#endif
#if VRG_COUNTRY_CFG_JAPAN
COUNTRY_ARCHIVE_MAKE_NAME( JAPAN         )
#endif
#if VRG_COUNTRY_CFG_NETHERLANDS
COUNTRY_ARCHIVE_MAKE_NAME( NETHERLANDS   )
#endif
#if VRG_COUNTRY_CFG_NEW_ZEALAND
COUNTRY_ARCHIVE_MAKE_NAME( NEW_ZEALAND   )
#endif
#if VRG_COUNTRY_CFG_NORTH_AMERICA
COUNTRY_ARCHIVE_MAKE_NAME( NORTH_AMERICA )
#endif
#if VRG_COUNTRY_CFG_SPAIN
COUNTRY_ARCHIVE_MAKE_NAME( SPAIN         )
#endif
#if VRG_COUNTRY_CFG_SWEDEN
COUNTRY_ARCHIVE_MAKE_NAME( SWEDEN        )
#endif
#if VRG_COUNTRY_CFG_NORWAY
COUNTRY_ARCHIVE_MAKE_NAME( NORWAY        )
#endif
#if VRG_COUNTRY_CFG_SWITZERLAND
COUNTRY_ARCHIVE_MAKE_NAME( SWITZERLAND   )
#endif
#if VRG_COUNTRY_CFG_TR57
COUNTRY_ARCHIVE_MAKE_NAME( TR57          )
#endif
#if VRG_COUNTRY_CFG_UK
COUNTRY_ARCHIVE_MAKE_NAME( UK            )
#endif
#if VRG_COUNTRY_CFG_TAIWAN
COUNTRY_ARCHIVE_MAKE_NAME( TAIWAN        )
#endif
#if VRG_COUNTRY_CFG_POLAND
COUNTRY_ARCHIVE_MAKE_NAME( POLAND        )
#endif
#if VRG_COUNTRY_CFG_AUSTRIA
COUNTRY_ARCHIVE_MAKE_NAME( AUSTRIA       )
#endif
#if VRG_COUNTRY_CFG_IRELAND
COUNTRY_ARCHIVE_MAKE_NAME( IRELAND       )
#endif
#if VRG_COUNTRY_CFG_SLOVAKIA
COUNTRY_ARCHIVE_MAKE_NAME( SLOVAKIA      )
#endif
#if VRG_COUNTRY_CFG_ROMANIA
COUNTRY_ARCHIVE_MAKE_NAME( ROMANIA       )
#endif
#if VRG_COUNTRY_CFG_SLOVENIA
COUNTRY_ARCHIVE_MAKE_NAME( SLOVENIA      )
#endif
#if VRG_COUNTRY_CFG_UNITED_ARAB_EMIRATES
COUNTRY_ARCHIVE_MAKE_NAME( UNITED_ARAB_EMIRATES      )
#endif
#if VRG_COUNTRY_CFG_MEXICO
COUNTRY_ARCHIVE_MAKE_NAME( MEXICO      )
#endif
#if VRG_COUNTRY_CFG_CYPRUS
COUNTRY_ARCHIVE_MAKE_NAME( CYPRUS        )
#endif
#undef COUNTRY_ARCHIVE_MAKE_NAME


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */
