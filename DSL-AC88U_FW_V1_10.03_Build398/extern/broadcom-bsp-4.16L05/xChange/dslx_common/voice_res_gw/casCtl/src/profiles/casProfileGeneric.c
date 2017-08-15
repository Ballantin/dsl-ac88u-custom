/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: casProfileGeneric.c
*
****************************************************************************
*    Description:
*
*     Currently, all CAS profiles are assigned to NULL except countries which do not 
*     support dynamic provisioning (e.g. Japan). 
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */
#include "casCtlProfiles.h"

/* ---- Private Constants and Types -------------------------- */

#if VRG_COUNTRY_CFG_AUSTRALIA
    const CASSTM gCasCtlRingPatterns_AUSTRALIA[] = {};
#else
   #define gCasCtlRingPatterns_AUSTRALIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_AUSTRIA
    const CASSTM gCasCtlRingPatterns_AUSTRIA[] = {};
#else
   #define gCasCtlRingPatterns_AUSTRIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_BELGIUM
    const CASSTM gCasCtlRingPatterns_BELGIUM[] = {};
#else
   #define gCasCtlRingPatterns_BELGIUM ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_BRAZIL
    const CASSTM gCasCtlRingPatterns_BRAZIL[] = {};
#else
   #define gCasCtlRingPatterns_BRAZIL ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_CHILE
    const CASSTM gCasCtlRingPatterns_CHILE[] = {};
#else
   #define gCasCtlRingPatterns_CHILE ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_CHINA
    const CASSTM gCasCtlRingPatterns_CHINA[] = {};
#else
   #define gCasCtlRingPatterns_CHINA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_CZECH
    const CASSTM gCasCtlRingPatterns_CZECH[] = {};
#else
   #define gCasCtlRingPatterns_CZECH ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_DENMARK
    const CASSTM gCasCtlRingPatterns_DENMARK[] = {};
#else
   #define gCasCtlRingPatterns_DENMARK ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_ETSI
    const CASSTM gCasCtlRingPatterns_ETSI[] = {};
#else
   #define gCasCtlRingPatterns_ETSI ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_FINLAND
    const CASSTM gCasCtlRingPatterns_FINLAND[] = {};
#else
   #define gCasCtlRingPatterns_FINLAND ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_FRANCE
    const CASSTM gCasCtlRingPatterns_FRANCE[] = {};
#else
   #define gCasCtlRingPatterns_FRANCE ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_GERMANY
    const CASSTM gCasCtlRingPatterns_GERMANY[] = {};
#else
   #define gCasCtlRingPatterns_GERMANY ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_HUNGARY
    const CASSTM gCasCtlRingPatterns_HUNGARY[] = {};
#else
   #define gCasCtlRingPatterns_HUNGARY ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_INDIA
    const CASSTM gCasCtlRingPatterns_INDIA[] = {};
#else
   #define gCasCtlRingPatterns_INDIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_IRELAND
    const CASSTM gCasCtlRingPatterns_IRELAND[] = {};
#else
   #define gCasCtlRingPatterns_IRELAND ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_ITALY
    const CASSTM gCasCtlRingPatterns_ITALY[] = {};
#else
   #define gCasCtlRingPatterns_ITALY ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_NETHERLANDS
    const CASSTM gCasCtlRingPatterns_NETHERLANDS[] = {};
#else
   #define gCasCtlRingPatterns_NETHERLANDS ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_NORTH_AMERICA
    const CASSTM gCasCtlRingPatterns_NORTH_AMERICA[] = {};
#else
   #define gCasCtlRingPatterns_NORTH_AMERICA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_NEW_ZEALAND
    const CASSTM gCasCtlRingPatterns_NEW_ZEALAND[] = {};
#else
   #define gCasCtlRingPatterns_NEW_ZEALAND ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_POLAND
    const CASSTM gCasCtlRingPatterns_POLAND[] = {};
#else
   #define gCasCtlRingPatterns_POLAND ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_ROMANIA
    const CASSTM gCasCtlRingPatterns_ROMANIA[] = {};
#else
   #define gCasCtlRingPatterns_ROMANIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_SLOVAKIA
    const CASSTM gCasCtlRingPatterns_SLOVAKIA[] = {};
#else
   #define gCasCtlRingPatterns_SLOVAKIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_SLOVENIA
    const CASSTM gCasCtlRingPatterns_SLOVENIA[] = {};
#else
   #define gCasCtlRingPatterns_SLOVENIA ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_SPAIN
    const CASSTM gCasCtlRingPatterns_SPAIN[] = {};
#else
   #define gCasCtlRingPatterns_SPAIN ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_SWEDEN
    const CASSTM gCasCtlRingPatterns_SWEDEN[] = {};
#else
   #define gCasCtlRingPatterns_SWEDEN ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_NORWAY
    const CASSTM gCasCtlRingPatterns_NORWAY[] = {};
#else
   #define gCasCtlRingPatterns_NORWAY ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_SWITZERLAND
    const CASSTM gCasCtlRingPatterns_SWITZERLAND[] = {};
#else
   #define gCasCtlRingPatterns_SWITZERLAND ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_TR57
    const CASSTM gCasCtlRingPatterns_TR57[] = {};
#else
   #define gCasCtlRingPatterns_TR57 ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_UK
    const CASSTM gCasCtlRingPatterns_UK[] = {};
#else
   #define gCasCtlRingPatterns_UK ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_TAIWAN
    const CASSTM gCasCtlRingPatterns_TAIWAN[] = {};
#else
   #define gCasCtlRingPatterns_TAIWAN ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_UNITED_ARAB_EMIRATES
    const CASSTM gCasCtlRingPatterns_UNITED_ARAB_EMIRATES[] = {};
#else
   #define gCasCtlRingPatterns_UNITED_ARAB_EMIRATES ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_MEXICO
    const CASSTM gCasCtlRingPatterns_MEXICO[] = {};
#else
   #define gCasCtlRingPatterns_MEXICO ((const CASSTM*) NULL )
#endif

#if VRG_COUNTRY_CFG_CYPRUS
    const CASSTM gCasCtlRingPatterns_CYPRUS[] = {};
#else
   #define gCasCtlRingPatterns_CYPRUS ((const CASSTM*) NULL )
#endif
