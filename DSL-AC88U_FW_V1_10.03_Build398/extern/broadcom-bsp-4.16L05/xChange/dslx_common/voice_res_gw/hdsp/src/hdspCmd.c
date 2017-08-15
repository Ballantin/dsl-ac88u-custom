/****************************************************************************
*
* Copyright © 2000-2009 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************
*
*    Filename: hdspCmd.c
*
****************************************************************************
*    Description:
*
*      This file contains functions for the DSP interface
*
****************************************************************************/

#include <vrgTypes.h>
#include <vrgLog.h>
#include <hapi.h>
#include <hapi_net.h>
#include <hapi_pkt.h>
#include <hapi_t38.h>
#include <hdspCfg.h>
#if HDSP_CFG_DEBUG_SUPPORT
#include <debug.h>
#endif
#include "hdspCmd.h"
#include "hdspHal.h"
#include <hdspVhdStateControl.h>
#if HDSP_ERR_RECOVERY_MECHANISM
#include <vrgEndptCfg.h>
#include <haus_hapi.h>
#include <bosSleep.h>
#include <xdrvDebug.h>
#include <boardHalDsp.h>
#include <bosError.h>
#endif

/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/

#if HDSP_ERR_RECOVERY_MECHANISM

static HAPICALLBKP    gHdspErrorCallback;

/*
** Maximum number of VHD channels to track errors for:
**    = max connections + max endpts
*/
#define HDSP_ERROR_MAX_VHD_CHANNELS    ( VRG_ENDPT_CFG_NUM_ENDPT + VRG_ENDPT_CFG_NUM_CNX )

#define HDSP_ERROR_ALL_CHANNELS        HDSP_ERROR_MAX_VHD_CHANNELS


/*
** Tracks the number of errors detected on each handle
*/
typedef struct HDSP_ERROR_COUNT
{
   /* 
   ** Current running error count.  This error count is incremented upon errors
   ** and decremented upon subsequent successful commands.
   */
   VRG_UINT16  currErrCount;

   /*
   ** Flag to indicate that this handle is pending a reset.  This flag is set when
   ** a certain number of errors have been detected
   */
   VRG_BOOL    bPendingReset;
   
} HDSP_ERROR_COUNT;


/*
** Tracks the number of errors detected on the overall device
*/
typedef struct HDSP_DEVICE_ERROR_COUNT
{
   /* VHD error count */
   HDSP_ERROR_COUNT  vhdErrCount [ HDSP_ERROR_MAX_VHD_CHANNELS ];

   /* Resource manager error count */
   HDSP_ERROR_COUNT  rmErrCount;

   /* 
   ** Overall device running error count.  This error count is incremented 
   ** upon errors and decremented upon subsequent successful commands. 
   */
   VRG_UINT16  currDevErrCount;

} HDSP_DEVICE_ERROR_COUNT;

#endif


/*
*****************************************************************************
** LOCAL FUNCTION PROTOTYPES
*****************************************************************************
*/

#if HDSP_ERR_RECOVERY_MECHANISM
static HDSPSTATUS hdspErrorProcess( VRG_UINT16 hdl, HSZCMD *pCmd );
static void hdspErrorClear( VRG_UINT16 hdl );
static void hdspErrorRecovery( int index );
#endif


/*
*****************************************************************************
** LOCAL VARIABLES
*****************************************************************************
*/

#define HDSP_CMD_DEBUG              0
#if HDSP_ERR_RECOVERY_MECHANISM
static HDSP_DEVICE_ERROR_COUNT  gHdspErrorCount;
static VRG_BOOL   bHdspErrorInitialized = VRG_FALSE;
#endif


/*
*****************************************************************************
** FUNCTION:   hdspSendCmd
**
** PURPOSE:    Send a HAPI command (IOCTL) to a hausware object
**
** PARAMETERS: hdl - the vhd handle
**             cmd - command type
**             id  -
**             op1 - 1st single word parameter
**             op2 - 2nd single word parameter
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       This function is synchronous, it blocks waiting for a response
**             from the DSP before returning.
**
*****************************************************************************
*/
VRG_SINT16 hdspSendCmd( VRG_UINT16 hdl, VRG_UINT32 cmd, VRG_UINT16 id, VRG_UINT16 op1, VRG_UINT16 op2 )
{
   HSZCMD      command;
   VRG_SINT16  status;

   command.hsxdevhdl = hdl;
   command.hsxcmd = (cmd & ~HDSP_SYNCCMD_ERR_IGN_MASK);
   command.hsxreqid = id;
   command.hsxop1 = op1;
   command.hsxop2.val = op2;

#if HDSP_CMD_DEBUG
   VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "ENDPT: hdspSendCmd DSP %d, VHD 0x%x, cmd 0x%x, op1 0x%x, op2 0x%x",
             hdspHalGetDspIndex(hdl), hdl, (cmd & ~HDSP_SYNCCMD_ERR_IGN_MASK), op1, op2));
#endif

   /* Enable VHD before sending the command */
   status = hdspVhdStateControlEnableVhd( hdl & 0xff );
   if ( status != HAPISUCCESS )
   {
      return status;
   }

   status = hapiIOctl( &command, hdspHalGetDspIndex(hdl) );   

#if HDSP_CFG_DEBUG_SUPPORT
   if (status)
   {
#ifndef BRCM_NONDIST_BUILD
      debugIpcQueryChannel(hdl);
#endif /* BRCM_NONDIST_BUILD */
   }
#endif

#if HDSP_ERR_RECOVERY_MECHANISM
      if ( status && !( cmd & HDSP_SYNCCMD_ERR_IGN ) )
      {
         /* Log the error.  The error module will attempt to re-try the command.
         ** If, during the re-try, the command is successful, we will ignore
         ** the original error and move on. */
         if ( HDSPSTATUS_SUCCESS == hdspErrorProcess( hdl, &command ) )
         {
            /* Cleare HDSP error statistics */
            hdspErrorClear( hdl );
            return ( HAPISUCCESS );
         }
      }
      else
      {
         /* Update the running error count to indicate success */
         hdspErrorClear( hdl );
      }
#endif
   
   
   return ( status );
}


/*
*****************************************************************************
** FUNCTION:   hdspSendCmdAsync
**
** PURPOSE:    Send a HAPI command (IOCTL) to a hausware object
**
** PARAMETERS: hdl - the vhd handle
**             cmd - command type
**             id  -
**             op1 - 1st single word parameter
**             op2 - 2nd single word parameter
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       This function is asynchronous, it does not block waiting for a
**             response from the DSP. It returns immediately after sending
**             the command.
**
*****************************************************************************
*/
VRG_SINT16 hdspSendCmdAsync( VRG_UINT16 hdl, VRG_UINT16 cmd, VRG_UINT16 id, VRG_UINT16 op1, VRG_UINT16 op2 )
{
   HSZCMD      command;
   VRG_SINT16  status;

   command.hsxdevhdl = hdl;
   command.hsxcmd = cmd;
   command.hsxreqid = id;
   command.hsxop1 = op1;
   command.hsxop2.val = op2;

#if HDSP_CMD_DEBUG
   VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "ENDPT: hdspSendCmdAsync DSP %d, VHD 0x%x, cmd 0x%x, op1 0x%x, op2 0x%x",
             hdspHalGetDspIndex(hdl), hdl, cmd, op1, op2));
#endif

   /* Enable VHD before sending the command */
   status = hdspVhdStateControlEnableVhd( hdl & 0xff );
   if ( status != HAPISUCCESS )
   {
      return status;
   }
   
   status = hapiAsyncIOctl( &command, hdspHalGetDspIndex(hdl) );

   return status;
}


/*
*****************************************************************************
** FUNCTION:   hdspSendCmdData
**
** PURPOSE:    Send a HAPI command (IOCTL) with associated data to a hausware object
**
** PARAMETERS: hdl - the vhd handle
**             cmd - command type
**             id  -
**             op1 - size in words (VRG_UINT16) of the data buffer to send/receive
**             op2 - pointer to the data buffer (must be VRG_UINT16 aligned)
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       This function is synchronous, it blocks waiting for a response
**             from the DSP before returning.
**
*****************************************************************************
*/
VRG_SINT16 hdspSendCmdData( VRG_UINT16 hdl, VRG_UINT32 cmd, VRG_UINT16 id, VRG_UINT16 op1, void* op2 )
{
   HSZCMD      command;
   VRG_SINT16  status;

   command.hsxdevhdl = hdl;
   command.hsxcmd = (cmd & ~HDSP_SYNCCMD_ERR_IGN_MASK) ;
   command.hsxreqid = id;
   command.hsxop1 = op1;
   command.hsxop2.ref = op2;

#if HDSP_CMD_DEBUG
   VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "ENDPT: hdspSendCmdData DSP %d, VHD 0x%x, cmd 0x%x, op1 0x%x, op2 0x%x",
             hdspHalGetDspIndex(hdl), hdl, (cmd & ~HDSP_SYNCCMD_ERR_IGN_MASK), op1, op2));
#endif

   /* Enable VHD before sending the command */
   status = hdspVhdStateControlEnableVhd( hdl & 0xff );
   if ( status != HAPISUCCESS )
   {
      return status;
   }
   
   status = hapiIOctl( &command, hdspHalGetDspIndex(hdl) );

#if HDSP_CFG_DEBUG_SUPPORT
   if (status)
   {
#ifndef BRCM_NONDIST_BUILD
      debugIpcQueryChannel(hdl);
#endif /* BRCM_NONDIST_BUILD */
   }
#endif

#if HDSP_ERR_RECOVERY_MECHANISM
      if ( status && !( cmd & HDSP_SYNCCMD_ERR_IGN ) )
      {
         /* Log the error.  The error module will attempt to re-try the command.
         ** If, during the re-try, the command is successful, we will ignore
         ** the original error and move on. */
         if ( HDSPSTATUS_SUCCESS == hdspErrorProcess( hdl, &command ) )
         {
            /* Cleare HDSP error statistics */
            hdspErrorClear( hdl );
            return ( HAPISUCCESS );
         }
      }
      else
      {
         /* Update the running error count to indicate success */
         hdspErrorClear( hdl );
      }
#endif
   

   return ( status );
}


/*
*****************************************************************************
** FUNCTION:   hdspSendCmdDataAsync
**
** PURPOSE:    Send a HAPI command (IOCTL) with associated data to a hausware object
**
** PARAMETERS: hdl - the vhd handle
**             cmd - command type
**             id  -
**             op1 - size in words (VRG_UINT16) of the data buffer to send/receive
**             op2 - pointer to the data buffer (must be VRG_UINT16 aligned)
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       This function is asynchronous, it does not block waiting for a
**             response from the DSP. It returns immediately after sending
**             the command. Currently only supported for HAPINET_EGRESSPACKET.
**
*****************************************************************************
*/
VRG_SINT16 hdspSendCmdDataAsync( VRG_UINT16 hdl, VRG_UINT16 cmd, VRG_UINT16 id, VRG_UINT16 op1, void* op2 )
{
   HSZCMD            command;
   VRG_SINT16        status;

   command.hsxdevhdl = hdl;
   command.hsxcmd = cmd;
   command.hsxreqid = id;
   command.hsxop1 = op1;
   command.hsxop2.ref = op2;

#if HDSP_CMD_DEBUG
   VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "ENDPT: hdspSendCmdDataAsync DSP %d, VHD 0x%x, cmd 0x%x, op1 0x%x, op2 0x%x",
             hdspHalGetDspIndex(hdl), hdl, cmd, op1, op2));
#endif

   /* Enable VHD before sending the command */
   status = hdspVhdStateControlEnableVhd( hdl & 0xff );
   if ( status != HAPISUCCESS )
   {
      return status;
   }
   
   status = hapiAsyncIOctl( &command, hdspHalGetDspIndex(hdl) );

   return status;
}


/*
*****************************************************************************
** FUNCTION:   hdspSendDataPacket
**
** PURPOSE:    Send a packet to a VHD. This function does not block waiting
**             for a response from the DSP. It returns immediately after
**             sending the command. The packet release mechanism is
**             synchronous; therefore, the packet is free for re-use
**             upon return from this function.
**
** PARAMETERS: vhdHdl - the vhd handle
**             packet - a pointer to the voice packet
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       Currently only supported for HAPINET_EGRESSPACKET.
**
*****************************************************************************
*/
VRG_SINT16 hdspSendDataPacket( VRG_UINT16 vhdHdl, void *packet )
{
   HAPI_PKT_HDR* netPacket = (HAPI_PKT_HDR*) packet;

   return( hdspSendCmdDataAsync( vhdHdl,
                                 HAPINET_EGRESSPACKET,
                                 HSCMDEXTDATA_ASYNC,
                                 (VRG_UINT16) ( sizeof (HAPI_PKT_HDR ) + netPacket->payloadLength ),
                                 packet ));
}


/*
*****************************************************************************
** FUNCTION:   hdspSendVoicePacket
**
** PURPOSE:    Send a packet to a VHD. This function does not block waiting
**             for a response from the DSP. It returns immediately after
**             sending the command. The packet release mechanism is
**             synchronous; therefore, the packet is free for re-use
**             upon return from this function.
**
** PARAMETERS: vhdHdl      - the vhd handle
**             packet      - a pointer to the voice packet
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       Currently only supported for HAPINET_EGRESSPACKET.
**
*****************************************************************************
*/
VRG_SINT16 hdspSendVoicePacket( VRG_UINT16 vhdHdl, void *packet )
{
   HSZCMD         command;
   HAPI_PKT_HDR*  netPacket = (HAPI_PKT_HDR*) packet;

   command.hsxdevhdl = vhdHdl;
   command.hsxcmd = HAPINET_EGRESSPACKET;
   command.hsxreqid = HSCMDEXTDATA_ASYNC;
   command.hsxop1 = (VRG_UINT16)( sizeof ( HAPI_PKT_HDR ) + netPacket->payloadLength );
   command.hsxop2.ref = packet;

   return ( hapiAsyncIOctl( &command, hdspHalGetDspIndex(vhdHdl) ));
}


/*
*****************************************************************************
** FUNCTION:   hdspSendT38Packet
**
** PURPOSE:    Send a T.38 packet to a vhd
**
** PARAMETERS: vhdHdl - the vhd handle
**             length - packet length
**             packet - a pointer to the T.38 packet
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspSendT38Packet( VRG_UINT16 vhdHdl, VRG_UINT16 length, void *packetp )
{
   HSZCMD         command;

   command.hsxdevhdl = vhdHdl;
   command.hsxcmd = HAPIT38_EGRESSPACKET;
   command.hsxreqid = HSCMDEXTDATA_ASYNC;
   command.hsxop1 = length;
   command.hsxop2.ref = packetp;

   return ( hapiAsyncIOctl( &command, hdspHalGetDspIndex(vhdHdl) ));
}


/*
*****************************************************************************
** FUNCTION:   hdspSetVbdPayloadType
**
** PURPOSE:    If VBD mode enabled, convert the payload type of voice
**             packets into voiceband data.
**
** PARAMETERS: netPacket   - a pointer to the voice packet
**             bVbdEnabled - flag indicating voiceband data mode
**
** RETURNS:
**
** NOTE:
**
*****************************************************************************
*/
void hdspSetVbdPayloadType( HAPI_PKT_HDR *netPacket, VRG_BOOL bVbdEnabled )
{
   /*
   ** IN LDX Stack model, voiceband data payload type is not supported
   ** so there is nothing to do here.
   */

   (void) netPacket;
   (void) bVbdEnabled;
}


#if HDSP_ERR_RECOVERY_MECHANISM

/*
*****************************************************************************
** FUNCTION:   hdspErrorInit
**
** PURPOSE:    Initialize the DSP error handling module
**
** PARAMETERS: eventCallbackp
**
** RETURNS:    nothing
**
** NOTE:       none
**
*****************************************************************************
*/
void hdspErrorInit( HAPICALLBKP    eventCallbackp )
{
   /* Initialize error handling state variables */
   memset( &gHdspErrorCount, 0, sizeof( gHdspErrorCount ) );

   gHdspErrorCallback = eventCallbackp;

   bHdspErrorInitialized = VRG_TRUE;
}


/*
*****************************************************************************
** FUNCTION:   hdspErrorDeinit
**
** PURPOSE:    De-initialize the DSP error handling module
**
** PARAMETERS: none
**
** RETURNS:    nothing
**
** NOTE:       none
**
*****************************************************************************
*/
void hdspErrorDeinit( void )
{
   if ( bHdspErrorInitialized == VRG_FALSE )
   {
      /* Clear error handling state variables */
      memset( &gHdspErrorCount, 0, sizeof( gHdspErrorCount ) );

      gHdspErrorCallback = NULL;

      bHdspErrorInitialized = VRG_FALSE;
   }
}

/*
*****************************************************************************
** FUNCTION:   hdspErrorProcess
**
** PURPOSE:    Process a DSP error.  Confirms the error by re-transmitting
**             the command and also checking the DSP counters.  If subsequent
**             attempt still fails, log the error appropriately or initiate
**             a reset.
**
** PARAMETERS: hdl   - handle on which the error occurred
**             pCmd  - command that the error was reported on
**
** RETURNS:    HDSPSTATUS_SUCCESS       - re-transmission succeeded
**             HDSPSTATUS_FAILED        - re-transmission failed, but the DSP
**                                        counters are still moving or maximum
**                                        error count has not been reached.
**             HDSPSTATUS_PENDING_RESET - re-transmission failed and DSP
**                                        counters are not moving.  Trigger
**                                        a reset.
**
** NOTE:       none
**
*****************************************************************************
*/
static HDSPSTATUS hdspErrorProcess( VRG_UINT16 hdl, HSZCMD *pCmd )
{
   /* Only process error requests if the error handling module has 
   ** been initialized */
   if ( bHdspErrorInitialized == VRG_TRUE )
   {
      HDSP_ERROR_COUNT *pErrCount = NULL;
      int               index;
      XDRV_DEBUG       *pDebugDriver = NULL;

     /* cnxId is actually a pointer to input arguments */
     pDebugDriver = boardHalDspGetDebugDriver();
     if ( pDebugDriver )
     {
        xdrvDebugDspReadCnt( pDebugDriver );
     }

      /* Check the status of the DSP counters */
      if ( xdrvDebugDspReadCnt( pDebugDriver ) )
      {
         /*
         ** DSP counters are not moving.  This implies that the DSP is either
         ** frozen or interrupts are not being serviced properly.  Read the DSP
         ** counters a couple more times to confirm that we in fact have a problem.
         */
         if ( xdrvDebugDspReadCnt( pDebugDriver ) )
         {
            /* Integrate a short sleep to ensure that the DSP countesr have enough time 
            ** for one tick */
            bosSleep( 1 );

            if ( xdrvDebugDspReadCnt( pDebugDriver ) )
            {
               /* DSP counters are not moving - proceed with the recovery 
               ** mechanism immediately to minimize down time */
               VRG_LOG_CRIT(( VRG_LOG_MOD_HDSP, "DSP counters are static: all channels are un-operational" ));
               hdspErrorRecovery( HDSP_ERROR_ALL_CHANNELS );
               return ( HDSPSTATUS_PENDING_RESET );
            }
         }
      }

      /* DSP counters are moving.  This implies that there may be a problem 
      ** with the hausware task or the DSP is having problems processing
      ** certain commands.  Re-issue the command again to confirm that
      ** an error does in fact exists. */

      if ( hapiIOctl( pCmd, 0 ) == HAPISUCCESS )
      {
         /* False alarm.  We will ignore this error for now */
         return ( HDSPSTATUS_SUCCESS );
      }

      /* Command still fails after a second attempt.  Track this error */
      if ( hdl == HAPIRESMAN )
      {
         /* Error in the RESMAN channel */
         pErrCount = &gHdspErrorCount.rmErrCount;

         /* Mark all channels as unavailable */
         index = HDSP_ERROR_ALL_CHANNELS;
      }
      else
      {
         /* Error in the VHD channels */

         /* Only update the error count for valid vhd handles.
         */
         index = hdl - HAPIVHD0;
         if ( ( index >= 0 ) && ( index < HDSP_ERROR_MAX_VHD_CHANNELS ) )
         {
            pErrCount = &gHdspErrorCount.vhdErrCount[index];
         }
         else
         {
            /* Unrecognized VHD handle */
            return( HDSPSTATUS_FAILED );
         }
      }

      /* Ignore all subsequent attempts if we've already marked the current
      ** handle as un-operational */
      if ( pErrCount->bPendingReset == VRG_TRUE )
      {
         return ( HDSPSTATUS_FAILED );
      }

      /* 
      ** Update the per-handle error count
      */
      pErrCount->currErrCount++;

      /*
      ** Update the overall device error counters
      */
      gHdspErrorCount.currDevErrCount++;

      /*
      ** Check against pre-defined error thresholds.  We currently have
      ** 4 thresholds set up to account for various error scenarios.  If
      ** any of the thresholds have been reached, the recovery mechanism
      ** is triggered.
      */
      if ( ( gHdspErrorCount.currDevErrCount >= HDSP_ERR_RECOVERY_MAX_DEVICE_ERR_COUNT ) ||
           ( pErrCount->currErrCount >= HDSP_ERR_RECOVERY_MAX_VHD_ERR_COUNT ) )
      {
         VRG_LOG_CRIT(( VRG_LOG_MOD_HDSP, "Maximum DSP error thresholds reached:" ));
         VRG_LOG_CRIT(( VRG_LOG_MOD_HDSP, "vhd (0x%x) - running: %i", hdl, pErrCount->currErrCount));
         VRG_LOG_CRIT(( VRG_LOG_MOD_HDSP, "device     - running: %i", gHdspErrorCount.currDevErrCount));

          hdspErrorRecovery( index );
         return ( HDSPSTATUS_PENDING_RESET );
      }

      return ( HDSPSTATUS_FAILED );
   }

   return ( HDSPSTATUS_FAILED );
}

/*
*****************************************************************************
** FUNCTION:   hdspErrorClear
**
** PURPOSE:    Clear a DSP error.  Decrements the error count for the
**             respective vhd
**
** PARAMETERS: hdl - the vhd handle
**
** RETURNS:    nothing
**
** NOTE:       none
**
*****************************************************************************
*/
static void hdspErrorClear( VRG_UINT16 hdl )
{
   /* Only process error clearing requests if the error handling module has 
   ** been initialized */
   if ( bHdspErrorInitialized == VRG_TRUE )
   {
      HDSP_ERROR_COUNT *pErrCount;
      int               index;

      if ( hdl == HAPIRESMAN )
      {
         /* RESMAN channel */
         pErrCount = &gHdspErrorCount.rmErrCount;
      }
      else
      {
         /* Only update the error count for valid vhd handles.
         */
         index = hdl - HAPIVHD0;
         if ( ( index >= 0 ) && ( index < HDSP_ERROR_MAX_VHD_CHANNELS ) )
         {
            pErrCount = &gHdspErrorCount.vhdErrCount[index];
         }
         else
         {
            /* Unrecognized VHD handle */
            return;
         }
      }

      if ( pErrCount->bPendingReset == VRG_TRUE )
      {
         /* Ignore all subsequent attempts if we've already marked the current
         ** handle as un-operational */
         return;
      }

      /* Update running error count for the current handle */
      if ( pErrCount->currErrCount > 0 )
      {
         pErrCount->currErrCount = 0;
      }

      /* Update current error count for the overall device */
      if ( gHdspErrorCount.currDevErrCount > 0 )
      {
         gHdspErrorCount.currDevErrCount = 0;
      }
   }
}


/*
*****************************************************************************
** FUNCTION:   hdspErrorRecovery
**
** PURPOSE:    Trigger the dsp error recovery mechanism.
**
** PARAMETERS: index - line index for which the error occurred on
**
** RETURNS:    nothing
**
** NOTE:       none
**
*****************************************************************************
*/
static void hdspErrorRecovery( int index )
{
   /* Only initiate error recovery request if the error handling module has 
   ** been initialized */
   if ( bHdspErrorInitialized == VRG_TRUE )
   {
      /* Check if all channels require error recovery */
      if ( index == HDSP_ERROR_ALL_CHANNELS )
      {
         int   i;

         /* Update mask to indicate all vhds are inactive */
         for ( i = 0; i < HDSP_ERROR_MAX_VHD_CHANNELS; i++ )
         {
            gHdspErrorCount.vhdErrCount[i].bPendingReset = VRG_TRUE;
         }
      }
      else
      {
         /* Set flag to indicate a reset has been initiated */
         gHdspErrorCount.vhdErrCount[index].bPendingReset = VRG_TRUE;
      }

      /* Hack: crash here upon any hdsp pending reset */
      BOS_ASSERT(0);
   }
}

#endif


/*
*****************************************************************************
** FUNCTION:   hdspGetOperationalStatus
**
** PURPOSE:    Retrieve the operational status of a particular vhd
**
** PARAMETERS: vhdHdl - vhd handle
**
** RETURNS:    operational status of the VHD:
**                HDSPSTATUS_PENDING_RESET - VHD is un-operational 
**                                           (pending a reset)
**                HDSPSTATUS_SUCCESS       - VHD is operational
**
** NOTE:       none
**
*****************************************************************************
*/
HDSPSTATUS hdspGetOperationalStatus( VRG_UINT16 vhdHdl )
{
#if HDSP_ERR_RECOVERY_MECHANISM
   if ( bHdspErrorInitialized == VRG_TRUE )
   {
      int   index;

      index = vhdHdl - HAPIVHD0;
      if ( ( index >= 0 ) && ( index < HDSP_ERROR_MAX_VHD_CHANNELS ) )
      {
         if ( gHdspErrorCount.vhdErrCount[index].bPendingReset == VRG_TRUE )
         {
            return ( HDSPSTATUS_PENDING_RESET );
         }
      }
   }
#endif

   return ( HDSPSTATUS_SUCCESS );
}
