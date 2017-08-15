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
*    Filename: hdspRm.h
*
****************************************************************************
*    Description:
*
*     This file contains an interface for the resource manager API.
*
****************************************************************************/

#ifndef HDSP_RM_H
#define HDSP_RM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <hapi_rm.h>
#include <hapi_swb.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

VRG_SINT32  hdspRmGetTotalMips( void );
void        hdspRmQueryDspBuffers( int dsp );

VRG_SINT16  hdspRmSwitchBoardConnect
(
   VRG_UINT16           srcHandle,
   HAPI_SWB_INTERFACE   srcInterface,
   VRG_UINT16           dstHandle,
   HAPI_SWB_INTERFACE   dstInterface,
   HAPI_SWB_TYPE        type,
   VRG_BOOL             bConnect
);

VRG_SINT16 hdspRmSetFramesizeAndMode
(
   VRG_UINT16           vhdHandle,
   VRG_UINT16           frameSize,
   HAPI_RM_VHD_MODE     mode
);

VRG_SINT16 hdspRmSetVhdProcessing
(
   VRG_UINT16           vhdHandle,
   HAPI_RM_DIRECTION    direction,
   VRG_UINT16           state
);

#ifdef __cplusplus
    }
#endif

#endif  /* HDSP_RM_H  */
