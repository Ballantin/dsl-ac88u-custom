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
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    LDX_Hpi.h
*
*  @brief   Global definitions for the HPI message layer implementation.
*
****************************************************************************/
/**
*  @defgroup HPI_Message   HPI Message Layer
*
*  @brief   HPI message layer translates message between the Stack and
*           the HPI.
*
*  The HPI message layer translates LDX Stack messages into HPI
*  messages and vice versa. This provides the high-level API functions
*  to transport LDX messages across the HPI.
****************************************************************************/

#if !defined( LDX_HPI_H )
#define LDX_HPI_H

/* ---- Include Files ---------------------------------------------------- */

#if !defined( HAPI_TYPES_H )
#  include <hapi_types.h>
#endif

#if !defined( LDX_SOCKETMSG_H )
#  include "ldx_socketmsg.h"
#endif

#if !defined( LDX_HAPI_H )
#  include "ldx_hapi.h"      /* Needed for LDX_Handle */
#endif

/**
 * @addtogroup LDX
 * @{
 */

/**
*  Following describes the LDX message that is written to the HPI.
*
*  It is important that its members are defined with 16-bit
*  words to ensure correct packing of data to be transported
*  between the DSP and host processors.
*
*  It is also important that the size of this structure is multiples
*  of 32-bits to avoid load exceptions on 32-bit machines.
*
*/
typedef struct
{
   HAPI_UINT16 handle;                /**< handle */
   HAPI_UINT16 header;                /**< data header */
   HAPI_UINT16 op1;                   /**< traditional op1 */
   HAPI_UINT16 op2;                   /**< traditional op2 */
   HAPI_UINT16 id_h;                  /**< message id high */
   HAPI_UINT16 id_l;                  /**< message id low */
   HAPI_UINT16 flags;                 /**< message flags */
   HAPI_UINT16 usertag;               /**< message user tag */

} LDX_ExternalMsg;

/**
 *  This structure describes a reduced form of the LDX_ExternalMsg.
 *
 *  This structure is currently used with hapiMultiIOctl function which
 *  only needs a sub-set of LDX_ExernalMsg.  The members have to be
 *  16-bit values because the message may be written to the HPI.
 */
typedef struct
{
   HAPI_UINT16 handle;                /**< handle */
   HAPI_UINT16 header;                /**< data header */
   HAPI_UINT16 op1;                   /**< traditional op1 */
   HAPI_UINT16 op2;                   /**< traditional op2 */

} LDX_ExternalSubMsg;

/* ---- Constants and Types ---------------------------------------------- */

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

void LDX_ExternalMsgReadAll( void *ipcId );
void LDX_ExternalMsgFreeAll( void *ipcId );

int LDX_ExternalMsgWrite
(
   LDX_Handle handle,
   LDX_SocketMsg *msgp
);


/** @} */

#endif /* LDX_HPI_H */
