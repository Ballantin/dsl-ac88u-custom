/****************************************************************************
*
*  Copyright (c) 2000-2009 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*    Filename: vrgEndptCallbacks.c
*
****************************************************************************
*    Description:
*
*      This file contains the implementation of all the endpoint callback
*      functions.
*
****************************************************************************/


/*
*****************************************************************************
** INCLUDE FILES
*****************************************************************************
*/

#include <string.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <vrgEndptCfg.h>
#include <hapi_pkt.h>
#include <hapi_net.h>                  /* Net VHD API */
#include <hapi_pte.h>                  /* PTE Service */
#include <hapi_t38.h>                  /* FXR Service */
#include <hapi_faxr.h>                 /* FXR Service */
#include <hapi_cdis.h>                 /* CDIS Service */
#include <hapi_clidtx.h>               /* CLIDTX Service */
#include <hapi_rtp.h>                  /* RTP Service */
#include <endpt.h>                     /* Endpoint include file */
#include <codec.h>                     /* codec map */
#include <ctype.h>
#include <bosSem.h>
#include <bosMisc.h>
#include <bosTask.h>
#include <bosSleep.h>
#include <bosSocket.h>
#if VRG_ENDPT_CFG_USE_HFE
#include <bosTimer.h>
#endif

#if BRCM_DUAL_3341
#include <hapi.h>
#endif

#include <hapi_rm.h>
#include <hapi_tone.h>
#include <hapi_gvad_defs.h>
#include <hapi_wrap_defs.h>
#include <hapi_gain.h>
#include <haus_hapi.h>
#include <hapi_swb.h>
#include <hapi_hec.h>

#include <hdspCmd.h>                   /* DSP interface */
#include <hdspTone.h>                  /* DSP tone interface */
#include <vrgTone.h>
#include <vrgClass.h>
#include <hapi_cmtd.h>                 /* CMTD Service */
#include <hapi_dtas.h>                 /* DTAS Service */
#include <hdspInit.h>
#include <hdspVhd.h>
#include <hdspRm.h>
#include <hdspHal.h>
#include <hdspIcp.h>

#include <boardHalInit.h>
#include <boardHalApm.h>
#include <boardHalCas.h>
#include <boardHalDsp.h>
#include <boardHalDaa.h>
#include <boardHalSlic.h>
#include <boardHalPower.h>
#include <boardHalDect.h>
#include <boardHalProv.h>
#include <vrgEndpt.h>                  /* VRG local header file */
#include <vrgProv.h>                   /* Endpoint provisioning */
#include "vrgEndptGlobe.h"             /* Endpoint around the "Globe" interface */
#include <classStm.h>                  /* CLASS state machine */
#include <casCtl.h>                    /* CAS signalling support */
#include <vrgRing.h>                   /* Provisioned Ring Cadences */
#include <cmtdCtl.h>
#include <pstnCtl.h>                   /* PSTN call setup support */
#include <xdrvDaa.h>                   /* DAA driver interface */
#if VRG_ENDPT_CFG_USE_TPD
#include <tpdCtl.h>                    /* TPD testing support */
#endif
#if VRG_ENDPT_CFG_USE_MLT
#include <mltApi.h>
#endif
#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
#include <dectCtl.h>                /* DECT control interface */
#include <xdrvDect.h>
#include <dectLineIf.h>
#include <dectCallMgr.h>
#endif /* VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT */

#if VRG_ENDPT_CFG_DECTTEST_SUPPORT
#include "dectTest.h"
#endif

#include <xdrvApm.h>                   /* APM driver interface */
#include <xdrvSlic.h>                  /* SLIC driver interface */
#include <debug.h>                     /* Debug module interface */
#include <heartbeat.h>                 /* Heartbeat monitoring module */

#include <boardHalIpc.h>
#include <lhapiExtension.h>

#include <str.h>
#include <memCircBuf.h>
#include <bosTime.h>

#include <leds.h>                      /* LED control wappers */

/* These are needed for the randomization functions  */
//#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
//#include <linux/mm.h>
#include <linux/string.h>
#include <linux/types.h>

#include <asm/uaccess.h>

#include "vrgEndptPrivate.h"


/*
*****************************************************************************
** GLOBAL SHARED VARIABLES
*****************************************************************************
*/

/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/

/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/

/*
*****************************************************************************
** LOCAL FUNCTION PROTOTYPES
*****************************************************************************
*/

static void PostHapiEvent(VRG_EVT_TYPE eventType, HSZEVT *evt, VRG_UINT16 dspNum);

/*
*****************************************************************************
** EXTERNS
*****************************************************************************
*/
extern void HapiProcessEcanEvt( HSZEVT *hapiEvt );
extern void process_cs_ingress_audio (void* buffer, unsigned short length);
/*****************************************************************************
*  FUNCTION:   CasEventCallback
*
*  PURPOSE:    Event handler for CAS events
*
*  PARAMETERS: handle    - CAS handle
*              eventType - event types that are sent to the client app
*              event     - event sent to the client app
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the CAS task
*
*****************************************************************************/
void CasEventCallback
(
   CAS_CTL_HANDLE       handle,
   CAS_CTL_EVENT_TYPE   eventType,
   CAS_CTL_EVENT        event
)
{
   VRG_EVT vrgEvt;

   memset( &vrgEvt, 0, sizeof( vrgEvt ));

   vrgEvt.type                = VRG_EVT_TYPE_CAS;
   vrgEvt.evt.cas.handle      = handle;
   vrgEvt.evt.cas.eventType   = eventType;
   vrgEvt.evt.cas.eventId     = event;

   /* Post this event for the VRG_EVT task to process */
   if( vrgEventQ != -1 )
   {
      if ( VrgQueuePost(vrgEventQ, (void*)&vrgEvt, sizeof(vrgEvt) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
      }
   }
}

/*****************************************************************************
*  FUNCTION:   CnxVhdEventCallback
*
*  PURPOSE:    Event handler for connection VHDs
*
*  PARAMETERS: evtp   - event message
*              dspNum - dsp number (not used)
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the HAPI task.
*              Only important processing should be performed in this thread
*              context. Logging and other less important processing should be
*              done as part of the "post-processing", which is done in the
*              endpt event processing task context (VRGEVPR). This is important
*              to minimize the amount of jitter added to packets sent towards the
*              network. Since logging requests are often serialized by the
*              operating system, it's possible that logging from the HAPI
*              task context would add 10's or 100's of milliseconds of jitter
*              to outbound packets since the task could potentially block waiting
*              for other tasks to finishing logging.
*
*****************************************************************************/
void CnxVhdEventCallback( HSZEVT *evtp, VRG_UINT16 dspNum )
{
   VRG_ENDPT  *endptState;
   CNXSTATE   *cnx;
   EPPACKET    epPacket;

   /*
   ** Get the connection state
   */
   cnx = GetCnxStateFromVHD( evtp->hsxdevhdl );
   if ( cnx == NULL )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT - CnxVhdEventCallback: Unknown VHD handle "
                "0x%x, evt: 0x%x, op1: 0x%x, op2: 0x%x",
                evtp->hsxdevhdl, evtp->hsxevt, evtp->hsxop1, evtp->hsxop2.val ));

      return;
   }

   /*
   ** Switch on the events of interest
   */
   switch( evtp->hsxevt )
   {
      case HAPINET_INGRESSRTPPKT:
      {
         if ( cnx->csCall == VRG_TRUE ) {
#ifdef VRG_ENDPT_2G3G_SUPPORT
           process_cs_ingress_audio (evtp->hsxop2.ref, evtp->hsxop1);
#endif
           return;
         }

         endptState = GetEndptState( cnx->lineId );
         if( !endptState )
         {
            VRG_LOG_DBG(( VRG_LOG_MOD_EPT, "cnxVhdEventCallback: ERROR getting state for line: %d", cnx->lineId ));
            return;
         }

         /* Check if we are transmitting CLASS II message, mute the far endpt if we are */
         if ( endptState->classState.classType == CLASS_MSG_OFFHOOK_CLID )
         {
            if ( classStmIsIdle( &endptState->classState ) == VRG_FALSE )
            {
               return;
            }
         }

         /* Only forward the packet if we are in non-mute mode */
         if ( (cnx->bMuteAudio == VRG_FALSE) && (cnx->bMuteT38 == VRG_FALSE) )
         {
            if (endptState->testMode == TESTMODE_PACKETLOOPBACK)
            {
               if ( cnx->vhdhdl == 0x52 )
               {
                  hdspSendCmdDataAsync( 0x54, HAPINET_EGRESSRTPPKT, HSCMDEXTDATA_ASYNC,
                                       evtp->hsxop1, (RTPPACKET*) evtp->hsxop2.ref );
               }
               else if ( cnx->vhdhdl == 0x54 )
               {
                  hdspSendCmdDataAsync( 0x52, HAPINET_EGRESSRTPPKT, HSCMDEXTDATA_ASYNC,
                                        evtp->hsxop1, (RTPPACKET*) evtp->hsxop2.ref );
               }
            }
            else
            {
               if ( endptState )
               {
                  /*  No need to do ModeCheck() or activeSignal check here.
                   *  Send/Recv mode is already set in SetRtpMode().
                   *  If in sending mode, rtp packets are generated and counted in DSP;
                   *  if in non-sending mode, no rtp packets will reach this point.
                   */
                  epPacket.packetp = (RTPPACKET*) evtp->hsxop2.ref;
                  epPacket.mediaType = EPMEDIATYPE_RTP;

#if VRG_ENDPT_CFG_INGRESS_RTP_G711U_SPECIAL_ENCODING
                  if( ((RTPPACKET *)epPacket.packetp)->pt == HAPI_RTP_IETF_PT_CODEC_G711U )
                  {
                     ingressG711USpecialEncoding( (VRG_UINT8 *)(epPacket.packetp+sizeof(RTPPACKET)), evtp->hsxop1-sizeof(RTPPACKET) );
                  }
#endif /* VRG_ENDPT_CFG_INGRESS_RTP_G711U_SPECIAL_ENCODING */

                  (*endptConfig.packet)( endptState->endptArchive, cnx->cnxId, &epPacket, evtp->hsxop1 );
               }
            }
         }
         return;
      }
      break;
      case HAPINET_INGRESSRTCPPKT:
      {
         endptState = GetEndptState( cnx->lineId );
         if( !endptState )
         {
            VRG_LOG_DBG(( VRG_LOG_MOD_EPT, "cnxVhdEventCallback: ERROR getting state for line: %d", cnx->lineId ));
            return;
         }

         if (endptState->testMode == TESTMODE_PACKETLOOPBACK)
         {
            if ( cnx->vhdhdl == 0x52 )
            {
               hdspSendCmdDataAsync( 0x54, HAPINET_EGRESSRTCPPKT, HSCMDEXTDATA_ASYNC,
                                       evtp->hsxop1, (RTPPACKET*) evtp->hsxop2.ref );
            }
            else if ( cnx->vhdhdl == 0x54 )
            {
               hdspSendCmdDataAsync( 0x52, HAPINET_EGRESSRTCPPKT, HSCMDEXTDATA_ASYNC,
                                        evtp->hsxop1, (RTPPACKET*) evtp->hsxop2.ref );
            }
         }
         else
         {
            if ( endptState )
            {
               epPacket.packetp = (RTPPACKET*) evtp->hsxop2.ref;
               epPacket.mediaType = EPMEDIATYPE_RTCP;
               (*endptConfig.packet)( endptState->endptArchive, cnx->cnxId, &epPacket, evtp->hsxop1 );
            }
         }
         return;
      }
      break;
      case HAPIT38_INGRESSPACKET:
      {
         endptState = GetEndptState( cnx->lineId );
         if( !endptState )
         {
            VRG_LOG_DBG(( VRG_LOG_MOD_EPT, "cnxVhdEventCallback: ERROR getting state for line: %d", cnx->lineId ));
            return;
         }

         /* Check if we are transmitting CLASS II message, mute the far endpt if we are */
         if ( endptState->classState.classType == CLASS_MSG_OFFHOOK_CLID )
         {
            if ( classStmIsIdle( &endptState->classState ) == VRG_FALSE )
            {
               return;
            }
         }

         if ( (cnx->bMuteAudio == VRG_FALSE) && (cnx->bMuteT38 == VRG_FALSE) )
         {
            if (endptState->testMode == TESTMODE_PACKETLOOPBACK)
            {
               if ( cnx->vhdhdl == 0x52 )
               {
                  hdspSendVoicePacket( 0x54,
                                       (HAPI_PKT_HDR*) evtp->hsxop2.ref );
               }
               else if ( cnx->vhdhdl == 0x54 )
               {
                  hdspSendVoicePacket( 0x52,
                                       (HAPI_PKT_HDR*) evtp->hsxop2.ref );
               }
            }
            else
            {
               if ( endptState )
               {
                  if (( ModeCheck( cnx->mode, SENDMODE )) || ( cnx->activeSignal ))
                  {
                     epPacket.packetp = (RTPPACKET*) evtp->hsxop2.ref;
                     epPacket.mediaType = EPMEDIATYPE_T38;
                     (*endptConfig.packet)( endptState->endptArchive, cnx->cnxId, &epPacket, evtp->hsxop1 );
                  }
               }
            }
         }
         return;
      }
      break;

      case HAPINET_INGRESSPACKET:
      {
         /* INGRESSPACKET should now go thru LDX RTP service and appear as HAPINET_INGRESSRTPPKT */
         return;
      }
      break;
   }
   
   /* All other control events will be handled by the VRG dispatcher */
   PostHapiEvent( VRG_EVT_TYPE_HAPI_CNX, evtp, dspNum );
}

/*****************************************************************************
*  FUNCTION:   DectEventCallback
*
*  PURPOSE:    Event handler for Dect events
*
*  PARAMETERS: chanNum   - DECT channel number, for the control channel use
*                          the value 'DECT_CONTROL_CHANNEL'
*              event     - Event reported
*              pData     - Extra information reported with the event, can
*                          be NULL (no other information).
*              usize     - Size of the information reported with the event,
*                          can be zero (no other information).
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the DECT task
*
*****************************************************************************/
#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
void DectEventCallback
(
   VRG_UINT16     chanNum,
   DECT_EVENT     event,
   void           *pData,
   unsigned int   uSize
)
{
   VRG_EVT vrgEvt;

   memset( &vrgEvt, 0, sizeof( vrgEvt ));

   vrgEvt.type             = VRG_EVT_TYPE_DECT;
   vrgEvt.evt.dect.chanNum = chanNum;
   vrgEvt.evt.dect.eventId = event;
   vrgEvt.evt.dect.pData   = pData;
   vrgEvt.evt.dect.uSize   = uSize;   

//   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "offset:%d length:%d uSize:%d", ((DECT_NVS_UPDATE *)pData)->offset, ((DECT_NVS_UPDATE *)pData)->nvsDataLength, uSize ));

   /* Post this event for the VRG_EVT task to process */
   if( vrgEventQ != -1 )
   {
      if ( VrgQueuePost(vrgEventQ, (void*)&vrgEvt, sizeof(vrgEvt) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
      }
   }
   
   /* Wait for response from VRG dispatcher */
   if ( bosSemTake( &vrgDectSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTake failed!", __FUNCTION__ ));
   }
}
#endif /* VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT */

/*****************************************************************************
*  FUNCTION:   EndptVhdEventCallback
*
*  PURPOSE:    Event handler for endpoint VHDs
*
*  PARAMETERS: evtp   - event message
*              dspNum - dsp number (not used)
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the HAPI task.
*              Only important processing should be performed in this thread
*              context. Logging and other less important processing should be
*              done as part of the "post-processing", which is done in the
*              endpt event processing task context (VRGEVPR). This is important
*              to minimize the amount of jitter added to packets sent towards the
*              network. Since logging requests are often serialized by the
*              operating system, it's possible that logging from the HAPI
*              task context would add 10's or 100's of milliseconds of jitter
*              to outbound packets since the task could potentially block waiting
*              for other tasks to finishing logging.
*
*****************************************************************************/
void EndptVhdEventCallback( HSZEVT *evtp, VRG_UINT16 dspNum )
{
   /*
   ** Process ECAN debug events, if applicable.
   */
   HapiProcessEcanEvt( evtp );

   /*
   ** Post this to the event queue for the dispatcher to handle.
   */
   PostHapiEvent( VRG_EVT_TYPE_HAPI_ENDPT, evtp, dspNum );
}

#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
/*****************************************************************************
*  FUNCTION:   TpdEventCallback
*
*  PURPOSE:    Event handler for TPD events
*
*  PARAMETERS: event     - event sent to the client app
*              provValue - value associated with event
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the TPD task
*
*****************************************************************************/
void TpdEventCallback
(
   TPD_CTL_EVENT    event,
   VRG_BOOL         mibSet,
   void            *provValue,
   int              chan
)
{
   VRG_EVT vrgEvt;

   memset( &vrgEvt, 0, sizeof( vrgEvt ));

   vrgEvt.type               = VRG_EVT_TYPE_TPD;
   vrgEvt.evt.tpd.eventId    = event;
   vrgEvt.evt.tpd.midSet     = mibSet;
   vrgEvt.evt.tpd.provValue  = provValue;
   vrgEvt.evt.tpd.chan       = chan;


   /* Post this event for the VRG_EVT task to process */
   if( vrgEventQ != -1 )
   {
      if ( VrgQueuePost( vrgEventQ, (void*)&vrgEvt, sizeof(vrgEvt) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
      }
   }
   
   /* Wait for response from VRG dispatcher */
   if ( bosSemTake( &vrgTpdSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTake failed!", __FUNCTION__ ));
   }   
}
#else
void MltCallback( MLT_PORT portId, MLT_REPORT *pResult )
{
   VRG_EVT vrgEvt;

   memset( &vrgEvt, 0, sizeof( vrgEvt ));

   vrgEvt.type            = VRG_EVT_TYPE_MLT;
   vrgEvt.evt.mlt.portId  = portId;
   vrgEvt.evt.mlt.pResult = pResult;


   /* Post this event for the VRG_EVT task to process */
   if( vrgEventQ != -1 )
   {
      if ( VrgQueuePost( vrgEventQ, (void*)&vrgEvt, sizeof(vrgEvt) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTake( &vrgMltSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTake failed!", __FUNCTION__ ));
   }
}
#endif
#endif

/*****************************************************************************
*  FUNCTION:   PstnEventCallback
*
*  PURPOSE:    Event handler for PSTN events
*
*  PARAMETERS: vhdHdl     - VHD handle
*              pEventInfo - PSTN event information
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the PSTN task
*
*****************************************************************************/
void PstnEventCallback
(
   VRG_UINT16     vhdHdl,
   PSTN_CTL_EVENT_INFO *pEventInfo
)
{
   VRG_EVT vrgEvt;

   memset( &vrgEvt, 0, sizeof( vrgEvt ));

   vrgEvt.type                = VRG_EVT_TYPE_PSTN;
   vrgEvt.evt.pstn.vhdHdl     = vhdHdl;
   vrgEvt.evt.pstn.pEventInfo = pEventInfo;

   /* Post this event for the VRG_EVT task to process */
   if( vrgEventQ != -1 )
   {
      if ( VrgQueuePost(vrgEventQ, (void*)&vrgEvt, sizeof(vrgEvt) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
      }
   }
   
   /* Wait for response from VRG dispatcher */
   if ( bosSemTake( &vrgPstnSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTake failed!", __FUNCTION__ ));
   }   
}

/*****************************************************************************
*  FUNCTION:   RmEventCallback
*
*  PURPOSE:    Event handler for Resource Manager.
*
*  PARAMETERS: evtp   - event message
*              dspNum - dsp number (not used)
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the HAPI task.
*              Only important processing should be performed in this thread
*              context. Logging and other less important processing should be
*              done as part of the "post-processing", which is done in the
*              endpt event processing task context (VRGEVPR). This is important
*              to minimize the amount of jitter added to packets sent towards the
*              network. Since logging requests are often serialized by the
*              operating system, it's possible that logging from the HAPI
*              task context would add 10's or 100's of milliseconds of jitter
*              to outbound packets since the task could potentially block waiting
*              for other tasks to finishing logging.
*
*****************************************************************************/
void RmEventCallback( HSZEVT *evtp, VRG_UINT16 dspNum )
{

   if ( evtp->hsxevt == HAPI_ICP_STATS_EVT )
   {
      hdspIcpStop( VRG_FALSE );

      hdspIcpDisplayStats( (ICPSTATS *) evtp->hsxop2.ref );

      hdspIcpRestart();
   }
   else
   {
      /*
      ** No immediate action required. Post this to the endpt event processing
      ** task which will the log the event.
      */

      PostHapiEvent( VRG_EVT_TYPE_HAPI_RM, evtp, dspNum );
   }
}

/*****************************************************************************
*  FUNCTION:   PostHapiEvent
*
*  PURPOSE:    Send a hausware event to the endpt processing task.
*
*  PARAMETERS: eventType   (in)  Type of event.
**             evt         (in)  Hausware event.
**             dspNum      (in)  DSP number that the event originated from.
*
*  RETURNS:    Nothing.
*
*  NOTE:
*
*****************************************************************************/
static void PostHapiEvent
(
   VRG_EVT_TYPE         eventType,
   HSZEVT              *evt,
   VRG_UINT16           dspNum
)
{
   VRG_EVT vrgEvt;

   memset( &vrgEvt, 0, sizeof( vrgEvt ));

   vrgEvt.type                = eventType;
   vrgEvt.evt.hapi.dspNum     = dspNum;
   vrgEvt.evt.hapi.reqId      = evt->hsxreqid;
   vrgEvt.evt.hapi.eventId    = evt->hsxevt;
   vrgEvt.evt.hapi.handle     = evt->hsxdevhdl;
   vrgEvt.evt.hapi.op1        = evt->hsxop1;
   vrgEvt.evt.hapi.op2        = evt->hsxop2;

   if ( evt->hsxreqid == HSEVTEXTDATA &&  /* Extended evant data */
        evt->hsxop2.ref != NULL       &&  /* Valid valid data */
        evt->hsxop1 != 0 )                /* Valid event data size */
   {
#if defined(BRCM_NONDIST_BUILD)
      vrgEvt.evt.hapi.op2.ref = mallocAtomic(evt->hsxop1);
#else
      vrgEvt.evt.hapi.op2.ref = malloc(evt->hsxop1);
#endif /* defined(BRCM_NONDIST_BUILD) */      
      if ( vrgEvt.evt.hapi.op2.ref == NULL )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR Could not allocate memory for event data!" ));
         return;
      }
      
      /* Copy event data to dynamically allocated buffer */
      memcpy( vrgEvt.evt.hapi.op2.ref, evt->hsxop2.ref, evt->hsxop1);
   }

   /* Post this event for the VRG_EVT task to process */
   if( vrgEventQ != -1 )
   {
      if ( VrgQueuePost( vrgEventQ, (void*)&vrgEvt, sizeof(vrgEvt) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         
         if ( vrgEvt.evt.hapi.reqId == HSEVTEXTDATA &&  /* Extended evant data */
              vrgEvt.evt.hapi.op2.ref != NULL       &&  /* Valid valid data */
              vrgEvt.evt.hapi.op1 != 0 )                /* Valid event data size */
         {
            free(vrgEvt.evt.hapi.op2.ref);
         }
      }
   }
}
