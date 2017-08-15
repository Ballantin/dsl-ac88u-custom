/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: hdspRm.c
*
****************************************************************************
*    Description:
*
*     This file implements resource manager helper functions; as well as
*     user-defined commands.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgLog.h>
#include <hdspCfg.h>
#include <hapi.h>
#include "hdspRm.h"
#include "hdspCmd.h"
#include <hapi_rm.h>
#include <hapi_swb.h>
#include "hdspHal.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   hdspRmGetTotalMips
**
** PURPOSE:    Gets the current DSP mips usage
**
** PARAMETERS: none
**
** RETURNS:    MIPS usage
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT32 hdspRmGetTotalMips( void )
{
   VRG_UINT32 mipsCount = 0;

   return (VRG_SINT32) mipsCount;
}


/*****************************************************************************
*  FUNCTION:   hdspRmQueryDspBuffers
*
*  PURPOSE:    To display DSP buffer pool stats
*
*  PARAMETERS: dsp     - the DSP number.
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
void hdspRmQueryDspBuffers( int dsp )
{
   VRG_SINT16 result;
   HAPI_RM_DSPBUFSTATS bufStats;

   /*
   ** The dspNumber field must be filled in as an input.
   ** This should come from the endpt level as an input parameter,
   ** but we'll hack it to 0 for now.
   */
   bufStats.dspNumber = dsp;

   /*
   ** Query DSP stats.
   */
   result = hdspSendCmdData( ((dsp << 8) | HAPIRESMAN),
                             HAPI_RM_DSP_BUFFER_QUERY_CMD,
                             HSCMDRESP,
                             sizeof( HAPI_RM_DSPBUFSTATS ),
                             &bufStats );

   if ( result != HAPISUCCESS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "Error : Query buffer failed."));
   }
   else
   {

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "DSP - Buffer Manager Information:"));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "%u %u:\t Free/Low Buffers in Message Buffer Pool",
                  bufStats.hapiMsgBufPoolnumFree,
                  bufStats.hapiMsgBufPoollowWater));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "%u %u:\t Free/Low Buffers in Extended Data Buffer Pool",
                  bufStats.hapiExtDataBufPoolnumFree,
                  bufStats.hapiExtDataBufPoollowWater));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "%u %u:\t\t Free/Low Buffers in Big Extended Data Buffer Pool",
                  bufStats.hapiBigExtDataBufPoolnumFree,
                  bufStats.hapiBigExtDataBufPoollowWater));
   }


   /*
   ** Query host stats.
   */
   result = hdspSendCmdData( ((dsp << 8) | HAPIRESMAN),
                             HAPI_RM_HOST_BUFFER_QUERY_CMD,
                             HSCMDRESP,
                             sizeof( HAPI_RM_DSPBUFSTATS ),
                             &bufStats );


   if ( result != HAPISUCCESS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "Error : Query buffer failed."));
   }
   else
   {

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "Host - Buffer Manager Information:"));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "%u %u:\t Free/Low Buffers in Message Buffer Pool",
                  bufStats.hapiMsgBufPoolnumFree,
                  bufStats.hapiMsgBufPoollowWater));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "%u %u:\t Free/Low Buffers in Extended Data Buffer Pool",
                  bufStats.hapiExtDataBufPoolnumFree,
                  bufStats.hapiExtDataBufPoollowWater));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "%u %u:\t\t Free/Low Buffers in Big Extended Data Buffer Pool",
                  bufStats.hapiBigExtDataBufPoolnumFree,
                  bufStats.hapiBigExtDataBufPoollowWater));
   }
}


/*****************************************************************************
*  FUNCTION:   hdspRmSwitchBoardConnect
*
*  PURPOSE:    Connect and/or disconnect switchboard connections.
*
*  PARAMETERS: srcHandle      (in)  Source VHD handle for SB connection.
*              srcInterface   (in)  Source interface (top-of-stack or bottom-of-stack).
*              dstHandle      (in)  Destination handle for SB connection.
*              dstInterface   (in)  Destination interface (top-of-stack or bottom-of-stack).
*              type           (in)  Connection type: simple/duplex.
*              bConnect       (in)  Connect/disconnect flag.
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
VRG_SINT16 hdspRmSwitchBoardConnect
(
   VRG_UINT16           srcHandle,
   HAPI_SWB_INTERFACE   srcInterface,
   VRG_UINT16           dstHandle,
   HAPI_SWB_INTERFACE   dstInterface,
   HAPI_SWB_TYPE        type,
   VRG_BOOL             bConnect
)
{
   VRG_SINT16        status;
   HAPI_SWB_PARAMS   swbParms;
   VRG_UINT16        cmd;

   /*
   ** Setup parameters for switchboard interaction
   */
   swbParms.srcHandle    = srcHandle;
   swbParms.srcInterface = srcInterface;
   swbParms.dstHandle    = dstHandle;
   swbParms.dstInterface = dstInterface;
   swbParms.type         = type;

   cmd = ( bConnect == VRG_TRUE ) ? HAPI_SWB_CONNECT_CMD : HAPI_SWB_DISCONNECT_CMD;

   status = hdspSendCmdData( ((hdspHalGetDspIndex(srcHandle) << 8) | HAPIRESMAN),
                             cmd,
                             HSCMDEXTDATA,
                             sizeof(swbParms),
                             &swbParms );

   if( status != HAPISUCCESS )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_HDSP, "ERROR setting switchboard connection for src 0x%x and dst 0x%x",
                srcHandle, dstHandle ));

   }
   else
   {
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "Switchboard %s (type %s): VHD 0x%x/%d with VHD 0x%x/%d", 
                (bConnect ? "ON" : "OFF"), 
                ((type == HAPI_SWB_SIMPLEX) ? "Simplex" : "Duplex"),
                srcHandle, srcInterface, 
                dstHandle, dstInterface));
      
   }

   return ( status );
}

/*****************************************************************************
*  FUNCTION:   hdspRmSetFramesizeAndMode
*
*  PURPOSE:    Set the frame size and mode of a VHD.
*
*  PARAMETERS: vhdHandle      (in)  VHD handle.
*              frameSize       (in)  Frame size to set for the VHD.
*              mode      (in)  Mode to set for the VHD (narrow band wideband).
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
VRG_SINT16 hdspRmSetFramesizeAndMode
(
   VRG_UINT16           vhdHandle,
   VRG_UINT16           frameSize,
   HAPI_RM_VHD_MODE     mode
)
{
   VRG_SINT16        status;

   /* Mode validity check */
   if ( mode >= HAPI_RM_VHD_MAX_MODES )
   {
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "Invalid mode %d", mode ));
      return HAPIFAILURE;
   }

   status = hdspSendCmd( ((hdspHalGetDspIndex(vhdHandle) << 8) | HAPIRESMAN),
                         HAPI_RM_SET_FRAME_SIZE_CMD,
                         HSCMDDATA,
                         vhdHandle,
                         (mode << HAPI_RM_MEDQ_CMD_MODE_SHIFT) | frameSize );

   if( status != HAPISUCCESS )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_HDSP, "ERROR setting frame size and mode for vhd 0x%x", vhdHandle ));

   }
   else
   {
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "VHD 0x%x: Frame size = %d, Mode = %s", 
                 vhdHandle, frameSize, (mode == HAPI_RM_VHD_NARROWBAND_MODE) ? "Narrowband" : "Wideband"));
   }

   return ( status );
}

/*****************************************************************************
*  FUNCTION:   hdspRmSetVhdProcessing
*
*  PURPOSE:    Enable or disable ingress and/or egress VHD processing.
*
*  PARAMETERS: vhdHandle (in)  VHD handle.
*              direction (in)  Processing direction to control.
*              state     (in)  Enable or disable processing.
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
VRG_SINT16 hdspRmSetVhdProcessing
(
   VRG_UINT16           vhdHandle,
   HAPI_RM_DIRECTION    direction,
   VRG_UINT16           state
)
{
   VRG_SINT16        status;

   /* Mode validity check */
   if ( state != 0 && state != 1 )
   {
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "Invalid state %d", state ));
      return HAPIFAILURE;
   }

   /* Direction validity check */   
   if ( direction != HAPI_RM_DIRECTION_UNSPECIFIED                          && 
        direction != HAPI_RM_DIRECTION_INGRESS                              &&
        direction != HAPI_RM_DIRECTION_EGRESS                               &&
        direction != (HAPI_RM_DIRECTION_INGRESS | HAPI_RM_DIRECTION_EGRESS)
      )
   {
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "Invalid direction %d", direction ));
      return HAPIFAILURE;
   }

   status = hdspSendCmd( ((hdspHalGetDspIndex(vhdHandle) << 8) | HAPIRESMAN),
                         (HAPI_RM_VHD_ENABLE_CMD | HDSP_SYNCCMD_ERR_IGN),
                         HSCMDDATA,
                         vhdHandle,
                         (direction << HAPI_RM_DIRECTION_SHIFT) | state );

   if( status != HAPISUCCESS )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_HDSP, "ERROR setting processing state for vhd 0x%x", vhdHandle ));
   }
   else
   {
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "VHD 0x%x: Direction = %d, State = %d", vhdHandle, direction, state));     
   }
   
   /* Needed to flush switchboard connections */
   hdspSendCmdAsync( ((hdspHalGetDspIndex(vhdHandle) << 8) | HAPIRESMAN),
                      HAPI_SWB_CALL_END_CMD,
                      HSCMDDATA,
                      vhdHandle,
                      0 );

   return ( status );
}
