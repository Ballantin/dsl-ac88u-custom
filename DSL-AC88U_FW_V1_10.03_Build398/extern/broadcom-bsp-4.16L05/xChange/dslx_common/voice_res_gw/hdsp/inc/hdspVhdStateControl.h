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
*    Filename: hdspVhdStateControl.h
*
****************************************************************************
*    Description:
*
*     This file contains an interface for the VHD state control API.
*
****************************************************************************/

#ifndef HDSP_VHD_STATE_CONTROL_H
#define HDSP_VHD_STATE_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

void hdspVhdStateControlInit( void );
void hdspVhdStateControlSetPstnInfoBlock( VRG_UINT16 pstnVhdHandle, VRG_UINT16 lineVhdHandle );
VRG_SINT16 hdspVhdStateControlRegisterVhd( VRG_UINT16 vhdHandle );
VRG_SINT16 hdspVhdStateControlUnregisterVhd( VRG_UINT16 vhdHandle );
VRG_SINT16 hdspVhdStateControlEnableVhd( VRG_UINT16 vhdHandle );
VRG_SINT16 hdspVhdStateControlDisableVhd( VRG_UINT16 vhdHandle );
VRG_BOOL hdspVhdStateControlIsVhdEnabled( VRG_UINT16 vhdHandle );
VRG_SINT16 hdspVhdStateControlAdjustTimeIncrement( VRG_UINT16 vhdHandle, VRG_UINT16 value, VRG_UINT16 *updatedTimer );
VRG_BOOL hdspVhdStateControlIsVhdRegistered( VRG_UINT16 vhdHandle );

#ifdef __cplusplus
    }
#endif

#endif  /* HDSP_VHD_STATE_CONTROL_H  */
