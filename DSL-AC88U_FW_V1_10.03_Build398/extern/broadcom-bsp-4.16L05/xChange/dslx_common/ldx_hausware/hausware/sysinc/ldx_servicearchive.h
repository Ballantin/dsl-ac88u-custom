/****************************************************************************
*
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    LDX_Service.h
*
****************************************************************************/
#if !defined( LDX_SERVICEARCHIVE_H )
#define LDX_SERVICEARCHIVE_H

/* ---- Include Files ---------------------------------------------------- */

#if !defined( HAPI_TYPES_H )
#  include <hapi_types.h>
#endif

#if !defined( SRVARCS_H )
#  include <srvarcs.h>
#endif

/* ---- Constants and Types ---------------------------------------------- */

/**
* Set to 1 if you want to store the name of the service in the service
* archive. This configuration really should be defined in a configuration
* header file.
*/
#define LDX_SERVICE_ARCHIVE_NAME_SUPPORT   0

/* macro for defining an LDX_ServiceArchive */
#if LDX_SERVICE_ARCHIVE_NAME_SUPPORT
#  define LDX_SERVICE_ARCHIVE(archp, isSwapped, parentSvcId, namep) { archp, isSwapped, parentSvcId, namep }
#else
#  define LDX_SERVICE_ARCHIVE(archp,isSwapped, parentSvcId, namep) { archp, isSwapped, parentSvcId }
#endif


typedef struct LDX_ServiceArchive
{
   const HSFSRVARC *archive;
   const int       isSwappable;
   const HAPI_UINT16    parentSvcId;
#if LDX_SERVICE_ARCHIVE_NAME_SUPPORT
   const char      *name;
#endif
} LDX_ServiceArchive;

#endif /* LDX_SERVICEARCHIVE_H */
