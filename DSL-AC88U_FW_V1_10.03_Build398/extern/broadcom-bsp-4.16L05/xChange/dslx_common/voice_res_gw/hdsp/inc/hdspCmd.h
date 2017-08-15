/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: hdspCmd.h
*
****************************************************************************
*    Description:
*
*      This file contains functions prototypes for the DSP interface
*
****************************************************************************/

#ifndef HDSP_CMD_H
#define HDSP_CMD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <vrgTypes.h>
#include <hapi.h>
#include <payload.h>

typedef enum HDSPSTATUS
{
   HDSPSTATUS_SUCCESS,
   HDSPSTATUS_FAILED,
   HDSPSTATUS_PENDING_RESET,

} HDSPSTATUS;

#define HDSP_SYNCCMD_ERR_IGN      0xCAFE0000    /* Ignore any error handling if command fails */
#define HDSP_SYNCCMD_ERR_IGN_MASK 0xFFFF0000

/*
*****************************************************************************
** EXTERNAL FUNCTION PROTOTYPES
*****************************************************************************
*/


VRG_SINT16 hdspSendCmd( VRG_UINT16 hdl, VRG_UINT32 cmd, VRG_UINT16 id, VRG_UINT16 op1, VRG_UINT16 op2 );
VRG_SINT16 hdspSendCmdAsync( VRG_UINT16 hdl, VRG_UINT16 cmd, VRG_UINT16 id, VRG_UINT16 op1, VRG_UINT16 op2 );
VRG_SINT16 hdspSendCmdData( VRG_UINT16 hdl, VRG_UINT32 cmd, VRG_UINT16 id, VRG_UINT16 op1, void* op2 );
VRG_SINT16 hdspSendCmdDataAsync( VRG_UINT16 hdl, VRG_UINT16 cmd, VRG_UINT16 id, VRG_UINT16 op1, void* op2 );
VRG_SINT16 hdspSendDataPacket( VRG_UINT16 vhdHdl, void *packet );
VRG_SINT16 hdspSendVoicePacket( VRG_UINT16 vhdHdl, void *packet );
void hdspSetVbdPayloadType( HAPI_PKT_HDR *netPacket, VRG_BOOL bVbdEnabled );
VRG_SINT16 hdspSendT38Packet( VRG_UINT16 vhdHdl, VRG_UINT16 length, void *packetp );

#if HDSP_ERR_RECOVERY_MECHANISM
void hdspErrorInit( HAPICALLBKP    eventCallbackp );
void hdspErrorDeinit( void );
#endif

HDSPSTATUS hdspGetOperationalStatus( VRG_UINT16 vhdHdl );

#ifdef __cplusplus
}
#endif

#endif /* HDSP_CMD_H */
