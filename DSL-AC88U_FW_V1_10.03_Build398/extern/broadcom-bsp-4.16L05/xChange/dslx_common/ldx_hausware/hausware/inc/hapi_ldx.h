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
*  hapi_ldx.h
*
*  PURPOSE:
*
*     This file contains global definitions for the HAPI LDX psuedo service.
*     Basically, these messages are used internally by the LDX-Stack
*     infrastructure and not intended as user messages. This header
*     file is not used any more
*
*  NOTES:
*
****************************************************************************/

#if !defined( HAPI_LDX_H )
#define HAPI_LDX_H

/* ---- Include Files ---------------------------------------------------- */

#if !defined( HAPI_TYPES_H )
#  include <hapi_types.h>
#endif

#if !defined( HAPIIDS_H )
#  include <hapiids.h>
#endif

#if !defined( HAPI_OPCODE_H )
#  include <hapi_opcode.h>
#endif

/* ---- Constants and Types ---------------------------------------------- */

typedef struct
{
   HAPI_UINT16   sender;
   HAPI_UINT16   target;
   HAPI_UINT16   header;
   HAPI_UINT16   op1;
   HAPI_UINT16   op2;

} HAPI_LDX_STD_MSG;

typedef struct
{
   HAPI_UINT16   sender;
   HAPI_UINT16   target;
   HAPI_UINT16   header;

} HAPI_LDX_EXT_MSG;

typedef struct
{
   HAPI_UINT16   maskHi;
   HAPI_UINT16   maskLo;
   HAPI_UINT16   header;
   HAPI_UINT16   op1;
   HAPI_UINT16   op2;

} HAPI_LDX_STD_DATA_MSG;

typedef struct
{
   HAPI_UINT16   maskHi;
   HAPI_UINT16   maskLo;
   HAPI_UINT16   header;

} HAPI_LDX_EXT_DATA_MSG;

/***************************************************************************
*                         HAPI LDX Commands
****************************************************************************/

#define  HAPI_LDX_STD_MSG_CMD       HSF_MKCMD_EXT( HAPI_LDX_SRV, 0 )
#define  HAPI_LDX_EXT_MSG_CMD       HSF_MKCMD_EXT( HAPI_LDX_SRV, 1 )
#define  HAPI_LDX_STD_DATA_MSG_CMD  HSF_MKCMD_EXT( HAPI_LDX_SRV, 2 )
#define  HAPI_LDX_EXT_DATA_MSG_CMD  HSF_MKCMD_EXT( HAPI_LDX_SRV, 3 )
#define  HAPI_LDX_QUIT_CMD          HSF_MKCMD_STD( HAPI_LDX_SRV, 4 )

/***************************************************************************
*                         HAPI LDX Events
****************************************************************************/

#define  HAPI_LDX_STD_MSG_EVT       HSF_MKEVT_EXT( HAPI_LDX_SRV, 0 )
#define  HAPI_LDX_EXT_MSG_EVT       HSF_MKEVT_EXT( HAPI_LDX_SRV, 1 )
#define  HAPI_LDX_STD_DATA_MSG_EVT  HSF_MKEVT_EXT( HAPI_LDX_SRV, 2 )
#define  HAPI_LDX_EXT_DATA_MSG_EVT  HSF_MKEVT_EXT( HAPI_LDX_SRV, 3 )

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/****************************************************************************
*
**********************************************************************/

#endif /* HAPI_LDX_H */

