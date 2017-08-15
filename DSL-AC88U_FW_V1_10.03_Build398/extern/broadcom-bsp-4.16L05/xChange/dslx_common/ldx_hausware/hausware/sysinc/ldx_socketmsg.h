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
#if !defined( LDX_SOCKETMSG_H )
#define LDX_SOCKETMSG_H

/* ---- Include Files ---------------------------------------------------- */

#include <inline_ldx.h>

#if !defined( HAPI_TYPES_H )
#  include <hapi_types.h>
#endif

#include <ldx_criticalsection.h>
#if !defined( HSF_MSG_H )
#include <hsf_msg.h>
#endif

#if !defined( HSF_MASKS_H )
#  include <hsf_masks.h>
#endif

#include <ldx_hapi.h>


/* Local defines  */

#define LDX_SRC_HDR_DEBUG        0
/* ---- Constants and Types ---------------------------------------------- */
/****************************************************************************/
/**
* The LDX_MsgFlags contains bitwise flags to indicate information about
* a message.
*/

typedef enum
{
   LDX_MsgFlagBufNeedsFree    = 0x0001,   /* Data needs to be freed        */

   LDX_MsgFlagResponse        = 0x0002,   /* Message response flag     */
   LDX_MsgFlagGenResponse     = 0x0004,   /* Msg should generate a response */

   /*
    * The following flag is set on messages that are transferred from the
    * Host to the DSP or vice-versa. A given message should only ever
    * cross the boundary once. This prevents messages from getting sent
    * back and forth in a circle.
    */
   LDX_MsgFlagOutsideOrigin   = 0x0008,   /* Msg originated from other CPU */

   /*
    * Following defines the different messaging types: IBD and OBD.
    * IBD messages are directional and can be either ingress or
    * egress.
    */

   LDX_MsgFlagObd          = 0x0010,         /* OBD message */
   LDX_MsgFlagIbdIngress   = 0x0020,         /* ingress IBD message */
   LDX_MsgFlagIbdEgress    = 0x0040,         /* egress IBD message */
   LDX_MsgFlagHostMsg      = 0x0080,         /* Host command */
   LDX_MsgFlagExtDataWithSrcHeader = 0x0100, /* Ext message that has source
                                              handle */
   /*
    * Note that the upper 5 bits are reserved for storing the HAPIRESULT
    * return code for HAPI commands. There are 20 different error codes
    * which can be represented in 5 bits. See HAPI layer implementation.
    */
   LDX_MsgFlag_Reserved_0 = 0x0200,
   LDX_MsgFlag_Reserved_1 = 0x0400,
   LDX_MsgFlag_Reserved_2 = 0x0800,
   LDX_MsgFlag_Reserved_3 = 0x1000,
   LDX_MsgFlag_Reserved_4 = 0x2000,
   LDX_MsgFlag_Reserved_5 = 0x4000,
   LDX_MsgFlag_Reserved_6 = 0x8000


} LDX_MsgFlags;

#define HSFRETC_MASK  ( LDX_MsgFlag_Reserved_0 | \
                        LDX_MsgFlag_Reserved_1 | \
                        LDX_MsgFlag_Reserved_2 | \
                        LDX_MsgFlag_Reserved_3 | \
                        LDX_MsgFlag_Reserved_4 | \
                        LDX_MsgFlag_Reserved_5 | \
                        LDX_MsgFlag_Reserved_6 )

/*
** Macros for working on LDX_MsgFlags
*/
#define HSFRETCODEID(retCode)       (((retCode) & HSFRETC_MASK) >> 8)
#define HSFMKRETCODEID(retCode)     (((retCode) << 8) & HSFRETC_MASK)
#define HSFCLRRETCODEID(retCode)    (retCode & (~HSFRETC_MASK))


/**
* Defines the service id type
*/
typedef HAPI_UINT16 HSFSRVID;

/****************************************************************************/
/**
* Describes the unionized message ID containing the IBD mask and OBD
* sender and target service IDs.
*/

typedef union
{
   HSF_IBD_MASK mask;
   struct
   {
      HSFSRVID sender;
      HSFSRVID target;  /* Used interchangable with LDX_Handle */
   } srv;

} LDX_MsgId;

/****************************************************************************/
/**
* The LDX_SocketMsg describes a message entry that is stored in the
* message queue of a socket.
*
* Note that we need the "struct LDX_SocketMsg" so that we can create a
* reference in LDX_Hapi.h
*/

typedef struct LDX_SocketMsg
{
   HSFMSG         msg;        /* The message being sent */
   LDX_MsgId      id;         /* Message ID */
   LDX_MsgFlags   msgFlags;   /* Private message flags */
   HAPI_UINT16    userTag;    /* Used to uniquely ID synchronous commands */
   HAPI_UINT16    handle;     /* Used to store the VHD handle */

} LDX_SocketMsg;


/****************************************************************************/
/**
* The LDX_SocketMsgExtSrcHeader describes a partial message header that is
* stored before the actual payload data. This header is used to keep track of
* the source VHD handle of this particular message. This is typically used
* for IBD messages to route them via the IBD Switchboard. This header is
* used by extended payload messages allocated off OS buffers or allocated
* in IPC buffers.
*
*/

typedef struct
{
   HAPI_UINT16 srcHandle;          /* Source Handle                    */
#if LDX_SRC_HDR_DEBUG
   HAPI_UINT16 rateMask;           /* Rate mask                        */
   HAPI_UINT16 seqNum;             /* Sequence Number                  */
#endif

} LDX_SocketMsgExtSrcHeader;

/****************************************************************************/
/**
* The LDX_SocketMsgExtHeader describes a message header that is stored
* before the actual payload data. This header is used by the extended
* payload data allocated off OS buffers
*
*/

typedef struct
{
   HAPI_UINT16 allocCount;                /* Number of payload virtual copies */
   LDX_SocketMsgExtSrcHeader srcHdr; /* Source header     */

} LDX_SocketMsgExtHeader;

/* Accessing payload data from payload header and vice versa */
#define LDX_DATA_TO_HDR(ptr) ((void *)((char*)ptr - sizeof(LDX_SocketMsgExtHeader)))
#define LDX_DATA_TO_SRC_HDR(ptr) (void *)((char*)ptr - sizeof(LDX_SocketMsgExtSrcHeader))
#define LDX_HDR_TO_DATA(ptr) ((char*)ptr + sizeof(LDX_SocketMsgExtHeader))
#define LDX_SRC_HDR_TO_DATA(ptr) ((char*)ptr + sizeof(LDX_SocketMsgExtSrcHeader))

/* ---- Function Prototypes ---------------------------------------------- */

/****************************************************************************
   LDX_MsgFlags   msgFlags,
   LDX_Handle     srchandle

* Queues a standard or extended IBD message onto
* an appropriate socket's IBD queue. The IBD message queue
* is indicated by the message flags parameter.
*
*     LDX_MsgFlagIbdIngress   - ingress IBD messages
*     LDX_MsgFlagIbdEgress    - egress IBD messages
*
* Note that the following inline functions are provided here for the
* convenience of the code to abstract the LDX_MsgId structure.
*/
void LDX_QueueStdIbdMsg
(
   LDX_Handle     handle,
   HAPI_UINT16    header,
   HAPI_UINT16    op1,
   HAPI_UINT16    op2,
   HSF_IBD_MASK   mask,
   LDX_MsgFlags   msgFlags
);

void LDX_QueueExtIbdMsg
(
   LDX_Handle     handle,
   HAPI_UINT16    header,
   void           *bufp,
   HAPI_UINT16    len,
   HSF_IBD_MASK   mask,
   LDX_MsgFlags   msgFlags,
   LDX_Handle     srcHandle
);

/****************************************************************************
* Creates and queues a standard or extended OBD message onto
* a socket's OBD queue.
*/
void LDX_QueueStdObdMsg
(
   LDX_Handle     handle,
   HAPI_UINT16    header,
   HAPI_UINT16    op1,
   HAPI_UINT16    op2,
   HSFSRVID       sender,
   HSFSRVID       target,
   HAPI_UINT16    userTag,
   LDX_MsgFlags   msgFlags
);

void LDX_QueueExtObdMsg
(
   LDX_Handle     handle,
   HAPI_UINT16    header,
   void           *bufp,
   HAPI_UINT16    len,
   HSFSRVID       sender,
   HSFSRVID       target,
   HAPI_UINT16    userTag,
   LDX_MsgFlags   msgFlags,
   LDX_Handle     srcHandle
);

/****************************************************************************
* Peeks the number of free buffers in the pool and returns true is buffers
* exceed the min threshold
*/
int LDX_PeekMsgPool( void );

/****************************************************************************
* Peeks the number of free buffers in the pool and returns true is buffers
* exceed the min threshold
*/
int LDX_PeekExtMsgPool( HAPI_UINT16 lengthinbytes );


/****************************************************************************
* Releases a buffer previously allocated using LDX_AllocSocketMsg back
* into the appropriate buffer pool.
*
* This function was moved from ldx_msg.h to this file since ldx_msg.h
* is an internal header file and the apps have no access to that file.
* Since this function is called from app callback, it got moved
* to this header file in sysinc folder
*/

void LDX_FreeSocketMsg_Unprotected( LDX_SocketMsg *msg );

INLINE void LDX_FreeSocketMsg( LDX_SocketMsg *msg )
{
   HSZBIOSSTATE   svState;

   ldxProtectGenericStart(svState);
   {
      LDX_FreeSocketMsg_Unprotected( msg );
   }
   ldxProtectGenericEnd(svState);
}


#endif /* LDX_SOCKETMSG_H */
