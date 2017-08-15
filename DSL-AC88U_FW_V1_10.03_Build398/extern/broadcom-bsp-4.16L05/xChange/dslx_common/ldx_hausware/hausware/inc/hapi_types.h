/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  hapi_types.h - HausWare type definitions
*
*  PURPOSE:
*     This file contains type and macro definitions for HAPI.
*
*
*           (c) Copyright 2003 Broadcom Canada Limited
*                       All Rights Reserved
****************************************************************************/

#ifndef HAPI_TYPES_H
#define HAPI_TYPES_H

/*-------------------------------------------------------------------------
*  Standard variable types
*/
#if !defined( HAPI_UINT16_DEFINED )
#define HAPI_UINT16_DEFINED
typedef unsigned short  HAPI_UINT16;     /* UINT16 is platform independant */
#endif

#if !defined( HAPI_SINT16_DEFINED )
#define HAPI_SINT16_DEFINED
typedef signed short    HAPI_SINT16;     /* SINT16 is platform independant */
#endif

#if !defined( HAPI_UINT32_DEFINED )
#define HAPI_UINT32_DEFINED
typedef unsigned long   HAPI_UINT32;
#endif

#if !defined( HAPI_SINT32_DEFINED )
#define HAPI_SINT32_DEFINED
typedef signed long     HAPI_SINT32;
#endif


#define SIZE_IN_BYTES( size )     ((size) * ( 3 - sizeof( HAPI_UINT16 )))
#define BYTES_TO_WORDS( size )    (((size)+1) >> 1)

/*-------------------------------------------------------------------------
* SIZEOF_W is a macro that returns the size of a data object in UINT16's
* (which some people mistakenly refer to as words).
*/
#define SIZEOF_W( data_obj ) ( sizeof( data_obj ) / sizeof( HAPI_UINT16 ))

/*-------------------------------------------------------------------------
* SIZEOF_B is a macro which returns the size of a data object in bytes
* (read 8-bit octets).
*
* The expression 3 - sizeof( UINT16) will return 2 on platforms whose native
* unit is a 16-bit word, and will return 1 on platforms whose native
* unit is an 8-bit word.
*/
#define SIZEOF_B( data_obj ) SIZE_IN_BYTES( sizeof( data_obj ))

/*-------------------------------------------------------------------------
* OFFSETOF returns the offset (in units of sizeof) of a structure member
* from the beginning of a structure
*/

#define OFFSETOF( structName, memberName )   ( (size_t)&((structName *)0)->memberName )
#define OFFSETOF_IN_BYTES( structName, memberName )   SIZE_IN_BYTES( OFFSETOF( structName, memberName ))


#endif   /* HAPI_TYPES_H */
