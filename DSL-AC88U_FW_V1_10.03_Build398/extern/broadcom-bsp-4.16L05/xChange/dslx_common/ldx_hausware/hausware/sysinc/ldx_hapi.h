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
*  ldx_hapi.h
*
*  PURPOSE:
*     This file defines the interface which is analagous to the "HAPI"
*     interface. It implements the required functionality, but only allows
*     for the newer style asynchronous only API.
*
*     It is expected that both the older style API (synchronous hapiIOctl),
*     and the newer style APIs could be built on top of this.
****************************************************************************/

#if !defined( LDX_HAPI_H )
#define  LDX_HAPI_H

/* ---- Include Files ---------------------------------------------------- */

#if !defined( HAPI_TYPES_H )
#  include <hapi_types.h>
#endif

#include <inline_ldx.h>  /* Needed for INLINE */

#if defined( __cplusplus )
extern "C" {
#endif

/* ---- Constants and Types ---------------------------------------------- */

#define  LDX_INVALID_HANDLE   0xFFFFu

typedef  HAPI_UINT16   LDX_ChannelType;
typedef  HAPI_UINT16   LDX_StackType;
typedef  HAPI_UINT16   LDX_Handle;
typedef  HAPI_UINT16   LDX_ChannelLevel;

struct LDX_SocketMsg;

/*---------------------------------------------------------------------------
*
* LDX_MsgCallback
*
* This callback is called to deliver host bound messages from a socket.
* hndl will be the handle that was returned from LDX_HapiOpen (or possibly
* the constant HAPIRESMAN) and msg will be a buffer containing an
* LDX_SocketMsg. It is the callback's responsibility to release 'msg' back
* into the message pool, and to also release any extended data.
*/

typedef  void (*LDX_MsgCallback)
(
   LDX_Handle              handle,
   struct LDX_SocketMsg   *msg
);


/*
** Internal use only, provided here because we have inlined the LDX_Handle functions
*/
#define LDX_HANDLE_DSPNUM_MASK  0xff00u
#define LDX_MAX_INTERNAL_HANDLE 0x0030u

/*---------------------------------------------------------------------------
* These are very simple "functions".  For now implement them as macros.
*
* LDX_HandleCreate      - Creates and LDX_Handle
* LDX_HandleGetInternal - Gets the "internal" handle from the LDX_Handle
* LDX_HandleGetDspnum   - Gets the dsp number from the LDX_Handle
* LDX_IsInternalHandle  - Determines if a given handle is "internal".
*/
#define LDX_HandleCreate( index, dspNum )    ((LDX_Handle)(((dspNum) << 8) | (index)))
#define LDX_HandleGetInternal( handle )      ( (handle) & ~LDX_HANDLE_DSPNUM_MASK )
#define LDX_HandleGetDspnum( handle )        ( (handle) >> 8 )
#define LDX_HandleIsInternal( handle )       ( LDX_HandleGetInternal( (handle) ) <= LDX_MAX_INTERNAL_HANDLE )

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*---------------------------------------------------------------------------
* LDX_HapiInit
*
* Initializes the LDX_Hapi interface. Events will be directed to the
* callbacks provided.
*/
void LDX_HapiInit
(
   LDX_MsgCallback      hostMsgCallback,
   LDX_MsgCallback      dspMsgCallback
);


#if defined( __cplusplus )
}
#endif

#endif   /* LDX_HAPI_H  */

