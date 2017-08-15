/***************************************************************************
*
*  Copyright 2000  Broadcom
*  All Rights Reserved
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*  All information contained in this document is Broadcom
*  company private, proprietary, and trade secret.
*
****************************************************************************
*
*  Description:
*     HAPI Interface Definitions
*
****************************************************************************/

#ifndef HAPI_H
#define HAPI_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>
#endif
#ifndef HAUS_CMD_H
#include <haus_cmd.h>
#endif

/****************************************************************************
*  DEFINITIONS
****************************************************************************/
/*
** Resource Manager identifier
*/
#define HAPIRESMAN            0x30 

/* Resource manager handle used for distributed host portion */
#define HAPI_HOST_RESMAN      0x2F 

/*
** Custom cmd/evt identifier
*/
#define HAPICUSTOM            0x2f

/* Type of stacks */

enum
{
   HAPI_GATEWAY_VHD = 0x00,
   HAPI_PHONE_VHD,
   HAPI_TONE_VHD,
   HAPI_CONFERENCE_VHD,       /* voice only, G.711 + similar rate capable */
   HAPI_HALF_INGRESS_VHD,
   HAPI_HALF_EGRESS_VHD,
   HAPI_PSTN_VHD,
   HAPI_LINE_ECAN_VHD,
   HAPI_LINE_AEC_VHD,
   HAPI_DATA_ACCESS_POINT

};

/* VHD levels */

enum 
{
   HAPI_VHD_LEVEL_0 = 0x00,
   HAPI_VHD_LEVEL_1,
   HAPI_VHD_LEVEL_2,
   HAPI_VHD_LEVEL_3,
   HAPI_VHD_LEVEL_4,
   HAPI_VHD_LEVEL_MAX = 0x7D  /* Chosen to be 7D since we add 1 to level while
                                 calculating the priority. And we reserve 
                                 level 0x7E for the Resource Manager.
                                 We could also start the levels from 1 
                                 instead of zero and then HAPI_VHD_LEVEL_MAX 
                                 can be set to 0x7E */
};


/* Object call-back task ptr for host processor callbacks */
typedef void   ( *HAPICALLBKP )( HSZEVT *, HAPI_UINT16 );

#if defined(__cplusplus)
extern "C"
{
#endif

/*
** Initalizes Hausware
*/
HAPIRESULT hapiInit( HAPICALLBKP callbackp, HAPI_UINT16 dspNum );

/* Open a channel
** All out of band events from the resource manager and PXDs are forwarded to the client via the callbackp ptr
*/
HAPI_UINT16 hapiOpen( HAPI_UINT16 vhdType, HAPICALLBKP callbackp, HAPI_UINT16 dspNum );

/*
**  Close a channel
*/
HAPIRESULT hapiClose( HAPI_UINT16 vhdHdl, HAPI_UINT16 dspNum );

/* Send a blocking HAPI command
** Blocking command only applies to out of band control commands
*/
HAPIRESULT hapiIOctl( HSZCMD *cmdp, HAPI_UINT16 dspNum );

/*
** Send a nonblocking HAPI command
** Note that all data messages (network packets) should ALWAYS be send using
** hapiAsyncIOctl since it shouldn't be blocking. Blocking commands
** ONLY refer to out of band control commands
*/
HAPIRESULT hapiAsyncIOctl( HSZCMD *cmdp, HAPI_UINT16 dspNum );

/*
** Send a blocking HAPI command with multiple commands in the payload
**
** The payload is to be an array of HSZCMD.  Extended payloads
** follow it's respective HSZCMD.
*/
HAPIRESULT hapiMultiIOctl( HSZCMD *cmdp, HAPI_UINT16 dspNum );


#if defined(__cplusplus)
}
#endif
#endif
