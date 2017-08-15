/***************************************************************************
*
* Copyright (c) 2003 Broadcom, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom
* company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
*      Switchboard API
*
****************************************************************************/

#ifndef HAPI_SWB_H
#define HAPI_SWB_H

/* ---- Include Files ---------------------------------------- */

#ifndef HAPI_TYPES_H
#include <hapi_types.h>    /* HAPI environment definitions     */
#endif

#ifndef HAPIIDS_H
#include <hapiids.h>       /* Hausware API object ids          */
#endif

#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>   /* Hausware generic cmds/events     */
#endif

/* ---- Constants and Types ---------------------------------- */

/*
** HAPI_SWB_INTERFACE defines the interfaces of a device
*/
typedef enum 
{
   HAPI_SWB_TOS = 0,  /* Top-Of-Stack    */
   HAPI_SWB_BOS,      /* Bottom-Of_Stack */

} HAPI_SWB_INTERFACE;

/*
** HAPI_SWB_TYPE defines the connection type
*/
typedef enum 
{
   HAPI_SWB_SIMPLEX = 0,  /* Simplex connection   */
   HAPI_SWB_DUPLEX,       /* Duplex connection    */

} HAPI_SWB_TYPE;

/*
** HAPI_SWB_PARAMS defines 
*  This structure is passed as OP2 for connect/disconnect commands.
*/
typedef struct
{
  HAPI_UINT16 srcHandle;    /* Source handle      */
  HAPI_UINT16 srcInterface; /* HAPI_SWB_INTERFACE */
  HAPI_UINT16 dstHandle;    /* Destination handle */
  HAPI_UINT16 dstInterface; /* HAPI_SWB_INTERFACE */
  HAPI_UINT16 type;         /* HAPI_SWB_TYPE      */

} HAPI_SWB_PARAMS;

/***************************************************************************
** Switchboard Commands
****************************************************************************/

/* Make switchboard connection from source to destination */
#define HAPI_SWB_CONNECT_CMD          (HSF_MKCMD_EXT(HAPI_SWB_SRV, 0))
#define HAPI_SWB_CONNECT_RSP          (HSF_MKRSP_STD(HAPI_SWB_CONNECT_CMD))
      /* OP1: length of HAPI_SWB_PARAMS */
      /* OP2: ptr to HAPI_SWB_PARAMS*/

/* Disconnect switchboard connection between source and destination */
#define HAPI_SWB_DISCONNECT_CMD          (HSF_MKCMD_EXT(HAPI_SWB_SRV, 1))
#define HAPI_SWB_DISCONNECT_RSP          (HSF_MKRSP_STD(HAPI_SWB_DISCONNECT_CMD))
      /* OP1: length of HAPI_SWB_PARAMS */
      /* OP2: ptr to HAPI_SWB_PARAMS*/

/* End of call command */
#define HAPI_SWB_CALL_END_CMD            (HSF_MKCMD_STD(HAPI_SWB_SRV, 2))
#define HAPI_SWB_CALL_END_RSP            (HSF_MKRSP_STD(HAPI_SWB_CALL_END_CMD))
      /* OP1: line VHD handle */
      /* OP2: Reserved*/
#endif /* HAPI_SWB_H */
