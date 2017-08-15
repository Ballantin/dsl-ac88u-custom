/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom Corporation
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
*    Filename: hdspVhd.c
*
****************************************************************************
*    Description:
*
*     This file implements helper functions for managing VHDs
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stdio.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <hdspCfg.h>
#include <hapi.h>
#include <hapi_net.h>
#include <hapi_pte.h>
#include <hapi_clidrx.h>
#include <hapi_dtas.h>
#include <hapi_pve.h>
#include <haus_hapi.h>
#include <hapi_faxr.h>
#include <vrgCountry.h>
#include <hapi_cmtd.h>
#include <hapi_gain.h>                 /* Gain Function */
#include <hdspTone.h>
#if HDSP_CFG_DEBUG_SUPPORT
#include <debug.h>
#endif
#include <xdrvApm.h>
#include <boardHalApm.h>
#include "hdspCmd.h"
#include "hdspVhd.h"
#include <hdspVhdStateControl.h>

#include <hapi_line.h>
#include <boardHalDsp.h>
#include <hdspCfg.h>

#include <hapi_rtp.h>
#include <bosSleep.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* Power level to Q.12 pair map */
typedef struct DBM_Q12_MAP
{
   int dBm;             /* power level in dBm */
   VRG_UINT16 q12;      /* power level as Q.12 value */
} DBM_Q12_MAP;
/* ---- Private Variables ------------------------------------------------ */
/* Mapping from dBm power levels to Q.12 values */
static const DBM_Q12_MAP dBm2Q12Map[] =
{
   {16, 25844},
   {15, 23033},
   {14, 20528},
   {13, 18296},
   {12, 16306},
   {11, 14533},
   {10, 12952},
   {9, 11544},
   {8, 10288},
   {7, 9169},
   {6, 8172},
   {5, 7283},
   {4, 6491},
   {3, 5785},
   {2, 5156},
   {1, 4595},
   {0, 4096},
   {-1, 3650},
   {-2, 3253},
   {-3, 2899},
   {-4, 2584},
   {-5, 2303},
   {-6, 2052},
   {-7, 1829},
   {-8, 1630},
   {-9, 1453},
   {-10, 1295},
   {-11, 1154},
   {-12, 1028},
   {-13, 916},
   {-14, 817},
   {-15, 728},
   {-16, 649}
};

/* ---- Private Function Prototypes -------------------------------------- */
static VRG_UINT16 dBmToQ12( int dBmVal );
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   hdspVhdSetMode
**
** PURPOSE:    Set the VHD mode
**
** PARAMETERS: vhdHdl      - VHD to set
**             mode        - new mode to set
**             initMode    - start mode (only applicable to faxr mode)
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE: This call might report (log) transient failure if there are some process 
**       pending in DSP
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetMode( VRG_UINT16 vhdHdl, VRG_UINT16 mode, VRG_UINT16 initMode )
{
   VRG_SINT16 status;

   status = hdspSendCmd( vhdHdl,
                         HAPINET_SETMODE,
                         HSCMDDATA,
                         mode,
                         initMode );

   /* Try once more since DSP might be busy processing other requests */
   if ( status != HAPISUCCESS )
   {
      /* Add some delay */
      bosSleep (10);
      /* Retry the command */
      status = hdspSendCmd( vhdHdl,
                            HAPINET_SETMODE,
                            HSCMDDATA,
                            mode,
                            initMode );      
   }

   return status;
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdSetVoiceCodec
**
** PURPOSE:    Set the voice encoder rate on a channel
**
** PARAMETERS: vhdHdl            - the handle of the channel
**             codec             - the codec type and rate
**             vad               - vad types ( HAPI_VAD_TYPES )
**             superPktIntervals - superpacket interval in units of samples
**                                 sampled at 8kHz
**             bInitialChange    - flag to inidicate whether this is the
**                                 initial voice rate change for this cnx.
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetVoiceCodec
(
   VRG_UINT16  vhdHdl,
   VRG_UINT16  codec,
   VRG_UINT16  vad,
   VRG_UINT16  superPktIntervals,
   VRG_BOOL    bInitialChange
)
{
   /*
   ** HACK - due to a bug in hausware, setting the voice rate via HAPI_SETVOICERATE
   **        when switching from idle->voice will cause a NET assert.  As a
   **        temporary work-around, we will set the initial voice rate via the
   **        voice registers.  Subsequence voice  rate changes can be set
   **        via HAPI_SETVOICERATE w/out any problems
   */
   if ( bInitialChange == VRG_FALSE )
   {
      /* Subsequent voice rate change
      **
      ** NOTE: the setvoicerate command must be sent sync due to potential race
      **       conditions between voice mode and voice rate changes
      */
      if (hdspSendCmd( vhdHdl,
                       HAPINET_SETVOICERATE,
                       HSCMDDATA,
                       (VRG_UINT16)( ( vad << 8 ) + codec ),
                       superPktIntervals ))
      {
          VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR setting voice rate for vhd 0x%x !!!", vhdHdl ));
          return ( HAPIFAILURE );
      }

      /* Update the VBD packet interval such that we maintain the
      ** same packetization rate when we swith into VBD mode.
      ** Use SETREG1 to simply overwrite the previous setting because
      ** we don't care what it was before and it's faster than
      ** standard GETREGS/SETREGS procedure */
      if (  hdspSendCmdAsync(  vhdHdl,
                          HAPINET_CONFIG_SETREG1,
                          HSCMDDATA,
                          VRG_OFFSETOF(HSZNETREGS, hsxVBDSuperPktInterval),
                          superPktIntervals ))
      {
          VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR setting hsxVBDSuperPktInterval NET register for vhd 0x%x !!!", vhdHdl ));
          return ( HAPIFAILURE );
      }
   }
   else
   {
      /* First voice rate change */

      HSZNETREGS netregs;

      if( hdspSendCmdData( vhdHdl,
                           HAPINET_CONFIG_GETREGS,
                           HSCMDEXTDATA_SYNC,
                           sizeof(HSZNETREGS),
                           &netregs ))
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR getting NET config registers for vhd 0x%x !!!", vhdHdl ));
         return ( HAPIFAILURE );
      }

      netregs.hsxPacketVoiceEncoder = (VRG_UINT16)( ( vad << 8 ) + codec );

      netregs.hsxVoiceSuperPktInterval = superPktIntervals;
      netregs.hsxVBDSuperPktInterval = superPktIntervals;

      if ( hdspSendCmdDataAsync( vhdHdl,
                            HAPINET_CONFIG_SETREGS,
                            HSCMDEXTDATA_ASYNC,
                            sizeof(HSZNETREGS),
                            &netregs ))
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR setting NET config registers for vhd 0x%x !!!", vhdHdl ));
         return ( HAPIFAILURE );
      }
   }

   return ( HAPISUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdSetVBDCodec
**
** PURPOSE:    Set the vbd encoder rate on a channel
**
** PARAMETERS: vhdHdl            - the handle of the channel
**             codec             - the codec type and rate
**             vad               - vad types ( HAPI_VAD_TYPES )
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetVBDCodec
(
   VRG_UINT16  vhdHdl,
   VRG_UINT16  codec,
   VRG_UINT16  vad
)
{

   /* Update the VBD codec such that we switch into the correct codec
   ** as we change into VBD mode from FAXR mode.
   ** Use SETREG1 to simply overwrite the previous setting because
   ** we don't care what it was before and it's faster than
   ** standard GETREGS/SETREGS procedure */
   if ( hdspSendCmdAsync( vhdHdl,
                          HAPINET_CONFIG_SETREG1,
                          HSCMDDATA,
                          VRG_OFFSETOF(HSZNETREGS, hsxVoiceBandDefault),
                          (VRG_UINT16)( ( vad << 8 ) + codec ) ))
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "Failed to set hsxVoiceBandDefault NET register for vhd 0x%x !!!", vhdHdl ));
      return ( HAPIFAILURE );
   }

   return ( HAPISUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   hdspVhdSetVadCngPlc
**
** PURPOSE:    Enable/Disable the VAD on a channel
**
** PARAMETERS: vhdHdl - the handle of the channel
**             vad - vad setting
**             cng - cng setting
**             plc - plc setting
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetVadCngPlc( VRG_UINT16 vhdHdl, VRG_UINT16 vad, VRG_UINT16 cng,
                                VRG_UINT16 plc )
{
   HSZVOICEREGS voiceregs;
   VRG_SINT16 status;

   /* Specify VAD and CNG options */
   voiceregs.voxregs.hsxVadCngPlcOptions = (VRG_UINT16) (vad + (VRG_UINT16)(cng << CNG_SHIFT));

   /* For now, we will always use Frame Repeat for PLC. */
   voiceregs.voxregs.hsxVadCngPlcOptions |= ( plc << PLC_SHIFT );

   status = hdspSendCmdAsync( vhdHdl,
                             HAPINET_VOICE_SETREG1,
                             HSCMDDATA,
                             OFFSETOF(HSZVOICEREGS, voxregs.hsxVadCngPlcOptions),
                             voiceregs.voxregs.hsxVadCngPlcOptions);

   return status;
}


/*****************************************************************************
*  FUNCTION:   hdspVhdGetVoiceStats
*
*  PURPOSE:    Fills in the HSZNETVOICESTATS structure from the DSP.
*
*  PARAMETERS: vhdhdl - the NETVHD handle
*              netvoicestats - structure to fill in
*
*  RETURNS:    status - HAPISUCCESS == 0
*
*  NOTE:
*****************************************************************************/
VRG_SINT16 hdspVhdGetVoiceStats( VRG_UINT16 vhdhdl,
                                 HSZNETVOICESTATS *netvoicestats )
{
   /* Get the voice stats from the DSP */
   return( hdspSendCmdData( vhdhdl,
                            HAPINET_GETVOICESTATS,
                            HSCMDRESP,
                            sizeof( HSZNETVOICESTATS ),
                            netvoicestats ));
}

/*****************************************************************************
*  FUNCTION:   hdspVhdDisplayVoiceStatsRaw
*
*  PURPOSE:    To display Voice Statistics out to console (raw display)
*
*  PARAMETERS: vhdhdl - the NETVHD handle
*              netvoicestats - structure to display
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
void hdspVhdDisplayVoiceStatsRaw( VRG_UINT16 vhdhdl,
                                  HSZNETVOICESTATS *netvoicestats )
{
   HAPI_PVE_ENCODERSTATS* pveENCstats;
   HAPI_PVE_DECODERSTATS* pveDECstats;

   pveENCstats = &netvoicestats->hsxPVEstats.encoder;
   pveDECstats = &netvoicestats->hsxPVEstats.decoder;

   VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "VOICE STATS: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
      vhdhdl,

      pveDECstats->peakHoldingTime,
      pveDECstats->packetCount,
      pveDECstats->addTailCount,
      pveDECstats->reorderCount,
      pveDECstats->overrunCount,
      pveDECstats->duplicateCount,
      pveDECstats->outOfRangeCount,
      pveDECstats->tooLateCount,
      pveDECstats->cantDecodeCount,
      pveDECstats->ajcUnderrunCount,
      pveDECstats->ajcDeleteCount,
      pveDECstats->ajcRepeatCount,
      pveDECstats->ajcResyncCount,
      pveDECstats->ajcPhaseJitterCount,

      pveENCstats->inactiveFrameCount,
      pveENCstats->activeFrameCount,
      pveENCstats->sidFrameCount,
      pveENCstats->toneRelayCount ));
}

/*****************************************************************************
*  FUNCTION:   hdspVhdFormatVoiceStats
*
*  PURPOSE:    To display Voice Statistics out to console
*
*  PARAMETERS: netvoicestats - structure to format
*              strBuffer     - buffer into which formatted string will be
*                              printed
*              bufSize       - size of strBuffer
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
void hdspVhdFormatVoiceStats( HSZNETVOICESTATS *netvoicestats, char *strBuffer,
                              unsigned int bufSize )
{
#if 0
   HAPI_PVE_ENCODERSTATS* pveENCstats;
   HAPI_PVE_DECODERSTATS* pveDECstats;

   /* Get the encoder and decoder stats. */
   pveENCstats = &netvoicestats->hsxPVEstats.encoder;
   pveDECstats = &netvoicestats->hsxPVEstats.decoder;
#endif

   /* Start with an empty string. */
   strBuffer[0] = '\0';

   /* Always leave room for '\0'. */
   bufSize -= 1;

   /* Write each line of output into a temp buffer then concatenate to
    * strBuffer to make sure we never run off the end of strBuffer. */

#if 0
   char tempBuf[80]; /* Temporary buffer to store each line of output. */
   sprintf(tempBuf, "Encoder statistics:\n");
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Suppressed packets (silence)", pveENCstats->inactiveFrameCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Packets sent", pveENCstats->activeFrameCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "SID packets", pveENCstats->sidFrameCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "RFC 2833 packets", pveENCstats->toneRelayCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "\nDecoder statistics:\n");
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Peak holding time (ms)", (pveDECstats->peakHoldingTime / 8));
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Packet Count", pveDECstats->packetCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Packets appended to JB", pveDECstats->addTailCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Reordered JB packets", pveDECstats->reorderCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Deleted due to JB overrun", pveDECstats->overrunCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Duplicates deleted by JB", pveDECstats->duplicateCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Out of range timestamp", pveDECstats->outOfRangeCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Arrived too late", pveDECstats->tooLateCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Bad packets", pveDECstats->cantDecodeCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "JB underrun states", pveDECstats->ajcUnderrunCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Deleted to reduce hold time", pveDECstats->ajcDeleteCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "Repeats to increase hold time", pveDECstats->ajcRepeatCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "JB resyncs", pveDECstats->ajcResyncCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );

   sprintf(tempBuf, "%30s: %d\n", "JB phase discontinuities", pveDECstats->ajcPhaseJitterCount);
   strncat(strBuffer, tempBuf, (bufSize - strlen(strBuffer)) );
 #endif
}

/*****************************************************************************
*  FUNCTION:   hdspVhdDisplayPveState
*
*  PURPOSE:    To display the PVE state out to console
*
*  PARAMETERS: vhdhdl - the NETVHD handle
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
void hdspVhdDisplayPveState( VRG_UINT16 vhdhdl )
{
   HAPI_PVE_STATE pveState;

   /* Get PVE state from the DSP */
   if( HAPISUCCESS == hdspSendCmdData( vhdhdl,
                                       HAPINET_GETVOICESTATE,
                                       HSCMDEXTDATA,
                                       sizeof( pveState ),
                                       &pveState ))
   {
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "NETVHD PVE State for vhdhdl 0x%x", vhdhdl ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "----------------"));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "timer                (Real time clock) = %d",
            pveState.timer ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "expectedTimestamp    (Next expected packet timestamp)         = %d",
            pveState.expectedTimestamp ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "lastArrivalTime      (Time since last packet arrived)         = %d",
            pveState.lastArrivalTime ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "lastArrTimeStamp     (Timestamp of last arrived packet)       = %d",
            pveState.lastArrTimeStamp ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "lastReleaseTimestamp (Timestamp of last released packet)      = %d",
            pveState.lastReleaseTimestamp ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "actualHoldTime       (Time from packet arrival to release)    = %d",
            pveState.actualHoldTime ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "currentHoldTime      (AJC algorithms adjusted hold time)      = %d",
            pveState.currentHoldTime ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "minHoldTimeTracker   (Estimate of min hold time (in samples)) = %d",
            pveState.minHoldTimeTracker ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "maxHoldTimeTracker   (Estimate of max hold time (in samples)) = %d",
            pveState.maxHoldTimeTracker ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "releaseBump          (How early packets can be released)      = %d",
            pveState.releaseBump ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "underrunDuration     (For hold time calc. and packet replays) = %d",
            pveState.underrunDuration ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "actionPendingTimer   (Replay/del on low energy or timer <= 0) = %d",
            pveState.actionPendingTimer ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "currentDec           (Current decoder index)                  = %d",
            pveState.currentDec ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "timerLastArrival     (last packet arrival time)               = %d",
            pveState.timerLastArrival ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "currentEnc           (Current encoder: vad type + voice code) = %d",
            pveState.currentEnc ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "currentEncPktTime    (Current encoder packet time in samples) = %d",
            pveState.currentEncPktTime ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "currentEncPktSize    (Superpacket size - header and preamble) = %d",
            pveState.currentEncPktSize ));
   }
   else
   {
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "ENDPT: Cannot display PVE State for vhdhdl = 0x%x", vhdhdl ));
   }
}


/*****************************************************************************
*  FUNCTION:   hdspVhdDisplayEndptStats
*
*  PURPOSE:    To display EndptVHD Statistics out to console
*
*  PARAMETERS: vhdhdl - the EndptVHD handle
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
void hdspVhdDisplayEndptStats( VRG_UINT16 vhdhdl )
{
   /* For LDX 19x, the concept of Endpoint VHD does not exist.  Thus,
   ** we will send a message to warn the user */

   VRG_LOG_WARN((VRG_LOG_MOD_HDSP,  "Warning: LDX19X does not incorporate the VHD model. Only use this function with LDX18X."));
}

/*****************************************************************************
*  FUNCTION:   hdspVhdDisplayFaxrStats
*
*  PURPOSE:    To display FAX relay Statistics out to console
*
*  PARAMETERS: vhdhdl - the NETVHD handle
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
void hdspVhdDisplayFaxrStats( VRG_UINT16 vhdhdl )
{
   HSZFAXRSTATS netfaxrstats;

   /* Get the jitter value from the DSP */
   if( HAPISUCCESS == hdspSendCmdData( vhdhdl,
                                       HAPINET_GETFAXSTATS,
                                       HSCMDRESP,
                                       sizeof( netfaxrstats ),
                                       &netfaxrstats ))
   {
      HSZFAXREGRESS_STATS   *hsxfaxrEgressStats;
      HSZFAXRINGRESS_STATS  *hsxfaxrIngressStats;



      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "Fax Relay Statistics for vhdhdl = %x", vhdhdl ));
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "------------------------------------------"));

      hsxfaxrEgressStats = &netfaxrstats.hsxfaxrEgressStats;
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "Egress statistics:" ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of frames received from network         = %d",
            hsxfaxrEgressStats->hsxFrameCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Jitter buffer overflow counter                 = %d",
            hsxfaxrEgressStats->hsxOverrunCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " High-speed data spoofing counter               = %d",
            hsxfaxrEgressStats->hsxSpoofCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of reordered packets                    = %d",
            hsxfaxrEgressStats->hsxReorderCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of packets that could not be reordered  = %d",
            hsxfaxrEgressStats->hsxfailtoReorderCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of retrain requests rcv from network    = %d",
            hsxfaxrEgressStats->hsxretrainCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of partial page req rcv from network    = %d",
            hsxfaxrEgressStats->hsxPPRCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of bad HDLC frames rcv from network     = %d",
            hsxfaxrEgressStats->hsxbadframeCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Spoofing timeout counter                       = %d",
            hsxfaxrEgressStats->hsxSpoofTimeoutCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " V.21 Tx. underflow counter                     = %d",
            hsxfaxrEgressStats->hsxv21TxUflowCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of fax pages received                   = %d",
            hsxfaxrEgressStats->hsxNumRxPages ));

      hsxfaxrIngressStats = &netfaxrstats.hsxfaxrIngressStats;
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "Ingress statistics:" ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of frames sent over the network         = %d",
            hsxfaxrIngressStats->hsxFrameCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Jitter buffer overflow counter                 = %d",
            hsxfaxrIngressStats->hsxOverrunCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of retrain requests sent over network   = %d",
            hsxfaxrIngressStats->hsxretrainCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of partial page req sent over network   = %d",
            hsxfaxrIngressStats->hsxPPRCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of bad HDLC frames sent over network    = %d",
            hsxfaxrIngressStats->hsxbadframeCount ));

      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  " Number of fax pages sent                       = %d",
            hsxfaxrIngressStats->hsxNumTxPages ));
   }
   else
   {
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "ENDPT: Cannot display NETVHD Stats for vhdhdl = %x", vhdhdl ));
   }
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdOpen
**
** PURPOSE:
**
** PARAMETERS: vhdType  - type of VHD to create
**             callback - callback to invoke for events generated by this VHD
**             dspNum   - DSP number
**             detectAdmin - the parameters to be used in the PTE DTMF detector
**                           for particular administrations.
**             detectPower - the minimum power level at which detect DTMF digits
**             egressVolume - PVE volume control on the decoded signal
**
** RETURNS:    VRG_SINT16
**
** NOTE:
*****************************************************************************
*/
VRG_SINT16 hdspVhdOpen
(
   VRG_UINT16     vhdType,
   HAPICALLBKP    callbackp,
   VRG_UINT16     dspNum,
   HAPI_PTE_ADM   pteDetectAdmin,
   VRG_UINT16     pteMinDetectPower,
   VRG_UINT16     pveEgressVolume,
   VRG_UINT16    *vhdhdl
)
{
   HAPI_PTE_REGS  pteregs;
   HSZVOICEREGS   voiceregs;

   /* Adjust pteMinDetectPower for ELP */
   pteMinDetectPower = hdspVhdAdjustIngressPowerLevel( pteMinDetectPower*10 ) / 10;

   if ( ( *vhdhdl = hapiOpen( vhdType, callbackp, dspNum )) == HAPIVHD_INVALID )
   {
      return HAPIFAILURE;
   }

   /* If VHD successfully opened, register it with the state control block */
   if ( HAPISUCCESS != hdspVhdStateControlRegisterVhd( *vhdhdl ) )
   {
      return HAPIFAILURE;
   }

   if( vhdType == HAPI_PSTN_VHD )
   {
      /*
      ** Enable CLIDRX Service
      */
      if ( HAPISUCCESS != hdspSendCmd( *vhdhdl,
                                       HAPI_CLIDRX_ENABLE_CMD,
                                       HSCMDDATA,
                                       VRG_TRUE,
                                       0 ) )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR enabling CLIDRX for ENDPT vhd 0x%x", *vhdhdl ));
      }

#if VRG_ENDPT_CFG_DECT_SUPPORT
      /*
      ** Enable DTAS Service
      */
      if ( HAPISUCCESS != hdspSendCmd( *vhdhdl,
                                       HAPI_DTAS_ENABLE,
                                       HSCMDDATA,
                                       VRG_TRUE,
                                       0) )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR enabling DTAS for ENDPT vhd 0x%x", *vhdhdl ));
      }
#endif

#if HDSP_CFG_CMTD_ENABLED
      /*
      ** Configure the Call Monitor Tone Detection Service
      */
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "ENDPT: Starting Call Monitor Tone Detection on ENDPT vhd 0x%x", *vhdhdl ));

      if ( ( hdspCmtdDBRInit( *vhdhdl , country ) )  != HAPISUCCESS )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR starting Call Monitor Tone Detection on ENDPT vhd 0x%x", *vhdhdl ));
      }
#endif
   }
   else if( vhdType == HAPI_LINE_ECAN_VHD )
   {
      HAPI_PTE_FILTER filterRegs;

      /*
      ** Enable PTE Service and DTMF digit event reporting.
      */
      if ( HAPISUCCESS != hdspSendCmd( *vhdhdl,
                                       HAPI_PTE_ENABLE_CMD,
                                       HSCMDDATA,
                                       VRG_TRUE,
                                       HAPI_PTE_MODE_DIG_EVT | HAPI_PTE_INGRESSMASK ))
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR enabling PTE for ENDPT vhd 0x%x", *vhdhdl ));
      }

      /*
      ** Enable long duration DTMF digit event reporting.
      */

      /* Configure DMTF filter control:
       *       - DTMF detection on permanently
       *       - report digit OFF events to host
       *       - detect for all 16 short and long DTMF digits
       *       - detection time for long DTMF digits is 2 sec
       *         ( longDetectTime in units of 100 ms )
       */
      filterRegs.filterControl = ( HAPI_PTE_FILTER_TYPE_PERMANENT |
                                   HAPI_PTE_FILTER_OFF_TO_HOST );
      filterRegs.filterDigit = 0xFFFF;
      filterRegs.filterLongDigit = 0xFFFF;
      filterRegs.longDetectTime = 20;

      if ( HAPISUCCESS != hdspSendCmdData( *vhdhdl,
                                           HAPI_PTE_FILTER_CMD,
                                           HSCMDEXTDATA_SYNC,
                                           sizeof(HAPI_PTE_FILTER),
                                           &filterRegs ))
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR enabling long duration DTMF reporting for ENDPT vhd 0x%x", *vhdhdl ));
      }

      /*
      ** Enable International DTMF Detection
      */

      /*
      ** For the tone stack, NET service doesn't exist so we cant use
      ** HAPINET_PTE_GET/SETREGS. We have to go through PTE API.
      ** PTE doesn't have an extended GETREGS command. PTE_GETREGS
      ** is a standard command which returns the registers in an
      ** extended command response. Therefore, we have to use
      ** SETREG1 which sets only one of the registers in the PTE
      ** register structure to set the registers we care about.
      */
      if ( HAPISUCCESS != hdspSendCmd( *vhdhdl,
                                       HAPI_PTE_SETREG1_CMD,
                                       HSCMDDATA,
                                       VRG_OFFSETOF(HAPI_PTE_REGS, detectAdmin),
                                       pteDetectAdmin ))
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR setting admin field of PTE registers for vhd 0x%x", *vhdhdl ));
      }

      if ( HAPISUCCESS != hdspSendCmd( *vhdhdl,
                                       HAPI_PTE_SETREG1_CMD,
                                       HSCMDDATA,
                                       VRG_OFFSETOF(HAPI_PTE_REGS, minDetectPower),
                                       pteMinDetectPower ))
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR setting power field of PTE registers for vhd 0x%x", *vhdhdl ));
      }

#if HDSP_CFG_CMTD_ENABLED
      /*
      ** Configure the Call Monitor Tone Detection Service
      */
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "ENDPT: Starting Call Monitor Tone Detection on ENDPT vhd 0x%x", *vhdhdl ));

      if ( ( hdspCmtdDBRInit( *vhdhdl , country ) )  != HAPISUCCESS )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR starting Call Monitor Tone Detection on ENDPT vhd 0x%x", *vhdhdl ));
      }
#endif

      /*
      ** Enable the line highpass filter.
      */
      if( HAPISUCCESS != hdspSendCmd( *vhdhdl, HAPI_LINE_HIGHPASS_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 ) )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ENDPT: ERROR enabling line HPF for ENDPT vhd 0x%x", *vhdhdl ));
      }
   }
   else if( vhdType == HAPI_GATEWAY_VHD || vhdType == HAPI_CONFERENCE_VHD )
   {
      /*
      ** Enable International DTMF Detection
      */

      /*
      ** We have to go to through NET service API here so that
      ** NET service and PTE have the same copy of the registers
      */
      if ( HAPISUCCESS != hdspSendCmdData( *vhdhdl,
                                           HAPINET_PTE_GETREGS,
                                           HSCMDRESP,
                                           sizeof( pteregs ),
                                           &pteregs ))
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR getting PTE registers for vhd 0x%x", *vhdhdl ));
      }

      pteregs.detectAdmin = pteDetectAdmin;
      pteregs.minDetectPower = pteMinDetectPower;

      if ( HAPISUCCESS != hdspSendCmdData( *vhdhdl,
                                           HAPINET_PTE_SETREGS,
                                           HSCMDEXTDATA_SYNC,
                                           sizeof( pteregs ),
                                           &pteregs ))
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR setting PTE registers for vhd 0x%x", *vhdhdl ));
      }

      /*
      ** Set PVE Egress Volume
      */
      if( pveEgressVolume > 0 )
      {
         if( HAPISUCCESS != hdspSendCmdData( *vhdhdl,
                                             HAPINET_VOICE_GETREGS,
                                             HSCMDRESP,
                                             sizeof(HSZVOICEREGS),
                                             &voiceregs ))
         {
            VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR setting Voice registers for vhd 0x%x", *vhdhdl ));
         }

         voiceregs.voxregs.pveEgressVolume = pveEgressVolume;

         if( HAPISUCCESS != hdspSendCmdData( *vhdhdl,
                                             HAPINET_VOICE_SETREGS,
                                             HSCMDEXTDATA_SYNC,
                                             sizeof(HSZVOICEREGS),
                                             &voiceregs ))
         {
            VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR setting Voice registers for vhd 0x%x", *vhdhdl ));
         }
      }
   }
   else
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR unknown vhd type %d for vhd 0x%x", vhdType, *vhdhdl ));
      *vhdhdl = HAPIVHD_INVALID;
   }

   return ( (*vhdhdl != HAPIVHD_INVALID) ? HAPISUCCESS : HAPIFAILURE );
}


/*
*****************************************************************************
** FUNCTION:   hdspVhdClose
**
** PURPOSE:
**
** PARAMETERS: vhdHdl  - VHD handle
**             dspNum  - DSP number
**
** RETURNS:    VRG_SINT16
**
** NOTE:
*****************************************************************************
*/
VRG_SINT16 hdspVhdClose( VRG_UINT16 vhdHdl, VRG_UINT16 dspNum )
{
    /* Unregister VHD from the state control block */
    if ( HAPISUCCESS != hdspVhdStateControlUnregisterVhd( vhdHdl ) )
    {
       return HAPIFAILURE;
    }

    return hapiClose( vhdHdl, dspNum );
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdSetJitterBuffer
**
** PURPOSE:    Set various jitter buffer registers.
**
** PARAMETERS: vhdHdl      - VHD to set
**             fixed       - True if the JB should be fixed. False for adaptive.
**             voiceMin    - Minimum holding time for voice packets.
**             voiceMax    - Maximum holding time for voice packets.
**             voiceTarget - Target holding time for voice packets.
**             dataTarget  - Target holding time for VBD packets.
**             ajcCustomReg - Custom features register.
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       This function is synchronous, it blocks waiting for responses
**             from the DSP before returning.
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetJitterBuffer
(
   VRG_UINT16     vhdhdl,
   VRG_BOOL       fixed,
   VRG_UINT16     voiceMin,
   VRG_UINT16     voiceMax,
   VRG_UINT16     voiceTarget,
   VRG_UINT16     dataTarget,
   VRG_UINT16     ajcCustomReg
)
{
   HSZVOICEREGS     pveregs;
   VRG_SINT16       status;

   /* Retrieve the PVE current register set on the channel */
   status = hdspSendCmdData( vhdhdl,
                             HAPINET_VOICE_GETREGS,
                             HSCMDRESP,
                             sizeof( pveregs ),
                             &pveregs );

   if( status != HAPISUCCESS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR getting jitter buffer registers for VHD 0x%x",
                vhdhdl ));
   }
   else
   {
      /* Setup the JB (in samples). */
      pveregs.ajbregs.options = ( (fixed) ? (AJC_OPTIONS_NOADAPT) : (0) );
      pveregs.ajbregs.jitterMin = voiceMin * 8;
      pveregs.ajbregs.jitterMax = voiceMax * 8;
      pveregs.ajbregs.jitterTarget = voiceTarget * 8;
      pveregs.ajbregs.jitterVBDTarget = dataTarget * 8;
      pveregs.ajbregs.customFeatures = ajcCustomReg;

      status = hdspSendCmdData( vhdhdl,
                                HAPINET_VOICE_SETREGS,
                                HSCMDRESP,
                                sizeof( pveregs ),
                                &pveregs );

      if( status != HAPISUCCESS )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR setting jitter buffer registers for VHD 0x%x",
                   vhdhdl ));
      }
   }

   return status;
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdGetJitterBuffer
**
** PURPOSE:    Get various jitter buffer registers.
**
** PARAMETERS: vhdHdl       - VHD to set
**             fixed        - True if the JB should be fixed. False for adaptive.
**             voiceMin     - Minimum holding time for voice packets.
**             voiceMax     - Maximum holding time for voice packets.
**             voiceTarget  - Target holding time for voice packets.
**             dataTarget   - Target holding time for VBD packets.
**             ajcCustomReg - Custom features register.
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       This function is synchronous, it blocks waiting for responses
**             from the DSP before returning.
*****************************************************************************
*/
VRG_SINT16 hdspVhdGetJitterBuffer
(
   VRG_UINT16     vhdhdl,
   VRG_BOOL      *fixed,
   VRG_UINT16    *voiceMin,
   VRG_UINT16    *voiceMax,
   VRG_UINT16    *voiceTarget,
   VRG_UINT16    *dataTarget,
   VRG_UINT16    *ajcCustomReg
)
{
   HSZVOICEREGS     pveregs;
   VRG_SINT16       status;

   /* Retrieve the PVE current register set on the channel */
   status = hdspSendCmdData( vhdhdl,
                             HAPINET_VOICE_GETREGS,
                             HSCMDRESP,
                             sizeof( pveregs ),
                             &pveregs );

   if( status != HAPISUCCESS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR getting jitter buffer registers for VHD 0x%x",
                vhdhdl ));
   }
   else
   {
      *fixed = ( (pveregs.ajbregs.options == AJC_OPTIONS_NOADAPT) ?
                 (VRG_TRUE) : (VRG_FALSE) );
      *voiceMin = pveregs.ajbregs.jitterMin / 8;
      *voiceMax = pveregs.ajbregs.jitterMax / 8;
      *voiceTarget = pveregs.ajbregs.jitterTarget / 8;
      *dataTarget = pveregs.ajbregs.jitterVBDTarget / 8;
      *ajcCustomReg = pveregs.ajbregs.customFeatures;
   }

   return status;
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdGetAvgJBDelay
**
** PURPOSE:    Get various jitter buffer registers.
**
** PARAMETERS: vhdHdl - VHD identifier
**             delay  - Average delay
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       This function is synchronous, it blocks waiting for responses
**             from the DSP before returning.
*****************************************************************************
*/
VRG_SINT16 hdspVhdGetAvgJBDelay
(
   VRG_UINT16     vhdhdl,
   VRG_UINT16    *delay
)
{
   HAPI_PVE_STATE pveState;

   /* Get PVE state from the DSP */
   VRG_SINT16 status = hdspSendCmdData(vhdhdl,
                                       HAPINET_GETVOICESTATE,
                                       HSCMDEXTDATA,
                                       sizeof( pveState ),
                                       &pveState );
   if( status != HAPISUCCESS )
   {
      return status;
   }

   *delay = pveState.actualHoldTime >> 3;

   return status;
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdSetAmrSettings
**
** PURPOSE:    Set various AMR registers.
**
** PARAMETERS: vhdHdl      - VHD to set
**             modeSet     - Mode set (bitmap from 0-8)
**             modeChangeNeighbor - Only change rates to neighboring mode
**             modeChangePeriod - Change every 2nd frame (false changes on
**                                any frame )
**             maxPtime    - Max packet time, including redundant frames
**             maxRed      - Max redundant time per packet.
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       This function is synchronous, it blocks waiting for responses
**             from the DSP before returning.
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetAmrSettings
(
   VRG_UINT16     vhdhdl,
   VRG_UINT16     modeSet,
   VRG_BOOL       modeChangeNeighbor,
   VRG_BOOL       modeChangePeriod,
   VRG_UINT16     ptime,
   VRG_UINT16     red
)
{
   HSZVOICEREGS     pveregs;
   VRG_SINT16       status;

   /* Retrieve the PVE current register set on the channel */
   status = hdspSendCmdData( vhdhdl,
                             HAPINET_VOICE_GETREGS,
                             HSCMDRESP,
                             sizeof( pveregs ),
                             &pveregs );

   if( status != HAPISUCCESS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR getting amr registers for VHD 0x%x", vhdhdl ));
   }
   else
   {
      /* Setup the AMR settings */
      pveregs.voxregs.amrOptions.modeSet             = (modeSet ? modeSet : 0x1FF);
      pveregs.voxregs.amrOptions.modeChangeNeighbor  = modeChangeNeighbor;
      pveregs.voxregs.amrOptions.modeChangePeriod    = modeChangePeriod;
      pveregs.voxregs.amrOptions.ptime               = ptime;
      pveregs.voxregs.amrOptions.red                 = red;

      status = hdspSendCmdData( vhdhdl,
                                HAPINET_VOICE_SETREGS,
                                HSCMDRESP,
                                sizeof( pveregs ),
                                &pveregs );

      if( status != HAPISUCCESS )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "ENDPT: ERROR setting amr registers for VHD 0x%x",
                   vhdhdl ));
      }
   }

   return status;
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdLineEquEnable
**
** PURPOSE:    Enable / Disable Equalizer
**
** PARAMETERS: dspNum - dsp number
**             vhdHdl - line VHD handle
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
*****************************************************************************
*/
VRG_SINT16 hdspVhdLineEquEnable( VRG_UINT16 vhdhdl, VRG_BOOL enable )
{
   VRG_SINT16              status;
   VRG_UINT16              equMask;
   BOARD_HAL_DSP_CFG       dspCfg;

   boardHalDspGetCfg( &dspCfg );

   if ( !dspCfg.isEquEnabled )
   {
      /* No need to configure if EQU not supported in DSP */
      return ( HAPISUCCESS );
   }

#if VRG_COUNTRY_CFG_WIDEBAND_SUPPORT
   equMask = HAPI_LINE_EQU_CONFIG_WIDEBAND_MASK;
#else
   equMask = HAPI_LINE_EQU_CONFIG_NARROWBAND_MASK;
#endif

   /*
   ** Enable/Disable TX FIR filter block
   */
   status = hdspSendCmdAsync( vhdhdl,
                              HAPI_LINE_EQU_CONFIG_FILTER_CMD,
                              HSCMDDATA,
                              HAPI_LINE_EQU_TX_FIR,
                              ( equMask | enable ) );

   if( status != HAPISUCCESS )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_HDSP, "Failed to set TX FIR filter enable for VHD 0x%x to %d",
                vhdhdl, enable ));

      return ( status );
   }

   return ( HAPISUCCESS );

}


/*
*****************************************************************************
** FUNCTION:   hdspVhdAdjustIngressPowerLevel
**
** PURPOSE:    Adjust the tone detect profile power level to account for
**             APM ELP setting. Tone detect profiles have the dB level set
**             to the actual minimum detection power level. However, because
**             the analog interace is configured to meet certain nominal
**             input/output power levels, the actual level at which the tone
**             hits the DTMF detector would be offset by the input level
**             analog setting (ELP). Therefore before setting the minimum
**             tone detection level we must adjust the profile to account
**             for ELP so we detect tones at the correct level.
**
** PARAMETERS: powerLevel - power level to adjust (pos. value in tens of dBm)
**
** RETURNS:    Adjusted power level (pos. value in tens of dBm).
**
** NOTE:
**
*****************************************************************************
*/
VRG_UINT16 hdspVhdAdjustIngressPowerLevel( VRG_UINT16 powerLevel )
{
   XDRV_APM  *apmDriver;
   VRG_SINT16 elp;
   VRG_SINT16 newPowerLevel;

   /*
    * DSL: DSP number is zero
    */
   apmDriver = boardHalApmGetDriver(0);
   elp = xdrvApmGetElp( apmDriver );

   /*
   ** The power level specified in is a positive number that represents ten times
   ** the negative dB level at which the tone should come out on
   ** the phone line (ie. 135 means generate a tone at -13.5dB level).
   ** The APM ELP setting is a signed number that represents a gain that should
   ** be applied to all signals coming in on the phone line (ie. elp=-3dB
   ** means attenuate all signals by 3dB).
   **
   ** After adjusting for the ELP, we ensure that the minimum tone detect level
   ** to be passed to the DSP is between -25 and -45 dB inclusive to adhere to
   ** spec and LDX requirements.
   */

   newPowerLevel = powerLevel - (elp*10);

   if (newPowerLevel < 250)
   {
      newPowerLevel = 250;
   }
   else if (newPowerLevel > 450)
   {
      newPowerLevel = 450;
   }

   return ( newPowerLevel );
}


/*****************************************************************************
** FUNCTION:   hdspVhdEnableCmtd
**
** PURPOSE:    Helper function configures CMTD Service
**
** PARAMETERS: vhdHdl - line VHD handle
**             country - Country code
**
** RETURNS:    status - HAPISUCCESS if request was successful otherwise
**                      HAPIFAILURE
**
** NOTE:
**
*****************************************************************************/
VRG_SINT16 hdspVhdEnableCmtd( VRG_UINT16 lineVhdHdl, VRG_COUNTRY country )
{
   VRG_SINT16 status = cmtdCtlConfigFilter( lineVhdHdl,
                                            country,
                                            CMTD_TONE_TYPE_DIAL,
                                            CMTD_FILTER_1 );
   if ( status != HAPISUCCESS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "%s failed", __FUNCTION__ ));
   }
   return( status );
}


/*****************************************************************************
** FUNCTION:   hdspVhdDisableCmtd
**
** PURPOSE:    Helper function disables CMTD Service
**
** PARAMETERS: vhdHdl - line VHD handle
**
** RETURNS:    status - HAPISUCCESS if request was successful otherwise
**                      HAPIFAILURE
**
** NOTE:
**
*****************************************************************************/
VRG_SINT16 hdspVhdDisableCmtd( VRG_UINT16 lineVhdHdl )
{
   VRG_SINT16 status = cmtdCtlDisable( lineVhdHdl );
   if ( status != HAPISUCCESS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "%s failed", __FUNCTION__ ));
   }
   return( status );
}


/*
*****************************************************************************
** FUNCTION:   hdspVhdGetCmtdPowerLevel
**
** PURPOSE:    Request estimate of filtered signal strength
**
** PARAMETERS: vhdHdl - line VHD handle
**             pPower - Power estimate (in dBm)
**
** RETURNS:    status - HAPISUCCESS if request was successful otherwise
**                      HAPIFAILURE
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspVhdGetCmtdPowerLevel( VRG_UINT16 vhdHdl, VRG_UINT32 *pPower )
{
   VRG_UINT32 power;

   VRG_SINT16 status = hdspSendCmdData( vhdHdl,
                                        HAPI_CMTD_GETPOWER_CMD,
                                        HSCMDRESP,
                                        sizeof(HAPI_CMTD_FILTER_POWER),
                                        &power );
   if(status == HAPISUCCESS)
   {
      *pPower = power;
   }
   return( status );
}


/*
*****************************************************************************
** FUNCTION:   hdspVhdSetSSRC
**
** PURPOSE:    Set the local SSRC for the RTP connection
**
** PARAMETERS: vhdHdl      - VHD to set
**             ssrc        - new local SSRC
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       The new SSRC will take effect after the
**             voice mode switches from idle to active
**
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetSSRC( VRG_UINT16 vhdHdl, VRG_UINT32 localSsrc )
{
   VRG_SINT16 status;

   VRG_UINT16 localSsrcHigh   = (VRG_UINT16)(localSsrc >> 16);
   VRG_UINT16 localSsrcLow    = (VRG_UINT16)(localSsrc & 0xFFFF);

   status = hdspSendCmd( vhdHdl,
                         HAPI_RTP_SET_SSRC_CMD,
                         HSCMDDATA,
                         localSsrcHigh,
                         localSsrcLow );

   return status;
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdSetEgressGain
**
** PURPOSE:    Configure egress gain in the line VHD
**
** PARAMETERS: vhdhdl - line VHD handle
**             gaindB - gain in dB from HAPI_HEC_VOLUME_MIN/Q_DB to 
**                      HAPI_HEC_VOLUME_MAX/Q_DB
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       The egress gain is applied to the HEC service in the line VHD
**             and therefore applies to voice and tones on the line.
**
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetEgressGain( VRG_UINT16 vhdhdl, int gaindB )
{
   VRG_SINT16 status = HAPIFAILURE;
   gaindB *= Q_DB;

   if( (gaindB >= HAPI_HEC_VOLUME_MIN) && (gaindB <= HAPI_HEC_VOLUME_MAX) )
   {
      status = hdspSendCmdAsync( vhdhdl,
                                 HAPI_HEC_SETREG1_CMD,
                                 HSCMDDATA,
                                 VRG_OFFSETOF( HEC_REGS, volumedB ),
                                 gaindB );

      if ( status != HAPISUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR configuring RX (egress) gain for vhd %d", vhdhdl ));
      }
   }
   else
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Request to set RX gain to %d dB is outside limits", gaindB ));
   }

   return status;
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdSetIngressGain
**
** PURPOSE:    Configure ingress gain in the line VHD
**
** PARAMETERS: vhdhdl - line VHD handle
**             gaindB - gain in dB from HAPI_HEC_GAIN_MIN/Q_DB to
**                      HAPI_HEC_GAIN_MAX/Q_DB
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       The ingress gain is applied to the HEC service in the line VHD
**             and therefore applies to voice and tones on the line.
**
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetIngressGain( VRG_UINT16 vhdhdl, int gaindB )
{
   VRG_SINT16 status = HAPIFAILURE;
   gaindB *= Q_DB;

   if( (gaindB >= HAPI_HEC_GAIN_MIN) && (gaindB <= HAPI_HEC_GAIN_MAX) )
   {
      status = hdspSendCmdAsync( vhdhdl,
                                 HAPI_HEC_SETREG1_CMD,
                                 HSCMDDATA,
                                 VRG_OFFSETOF( HEC_REGS, gaindB ),
                                 gaindB );

      if ( status != HAPISUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR configuring TX (ingress) gain for vhd %d", vhdhdl ));
      }
   }
   else
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Request to set TX gain to %d dB is outside limits", gaindB ));
   }

   return status;
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdSetPVEEgressVolume
**
** PURPOSE:    Configure egress volume at the PVE
**
** PARAMETERS: vhdhdl - cnx VHD handle
**             volumedB - volume in dB from HDSPVHD_MIN_GAIN to HDSPVHD_MAX_GAIN
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       The egress volume is applied to connection VHDs and therefore
**             only applies voice and not tones.
**
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetPVEEgressVolume( VRG_UINT16 vhdhdl, int volumedB )
{
   HSZVOICEREGS   voiceregs;
   VRG_SINT16     status;

   if( (volumedB >= HDSPVHD_MIN_GAIN) && (volumedB <= HDSPVHD_MAX_GAIN) )
   {
      voiceregs.voxregs.pveEgressVolume = dBmToQ12(volumedB);

      status = hdspSendCmdAsync( vhdhdl,
                                 HAPINET_VOICE_SETREG1,
                                 HSCMDDATA,
                                 OFFSETOF(HSZVOICEREGS, voxregs.pveEgressVolume),
                                 voiceregs.voxregs.pveEgressVolume);

      if ( status != HAPISUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "Failed to set Voice register pveEgressVolume for vhd 0x%x", vhdhdl ));
      }
   }
   else
   {
      status = HAPIFAILURE;

      VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "Ignoring request to set Rx gain to %d dB", volumedB ));
   }

   return status;
}

/*
*****************************************************************************
** FUNCTION:   hdspVhdSetPVEIngressVolume
**
** PURPOSE:    Configure ingress volume at the PVE
**
** PARAMETERS: vhdhdl - cnx VHD handle
**             volumedB - volume in dB from HDSPVHD_MIN_GAIN to HDSPVHD_MAX_GAIN
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       The ingress volume is applied to connection VHDs and therefore
**             only applies voice and not tones.
**
*****************************************************************************
*/
VRG_SINT16 hdspVhdSetPVEIngressVolume( VRG_UINT16 vhdhdl, int volumedB )
{
   HSZVOICEREGS   voiceregs;
   VRG_SINT16     status;

   if( (volumedB >= HDSPVHD_MIN_GAIN) && (volumedB <= HDSPVHD_MAX_GAIN) )
   {
      voiceregs.voxregs.pveIngressVolume = dBmToQ12(volumedB);

      status = hdspSendCmdAsync( vhdhdl,
                                 HAPINET_VOICE_SETREG1,
                                 HSCMDDATA,
                                 OFFSETOF(HSZVOICEREGS, voxregs.pveIngressVolume),
                                 voiceregs.voxregs.pveIngressVolume);

      if ( status != HAPISUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "Failed to set Voice register pveIngressVolume for vhd 0x%x", vhdhdl ));
      }
   }
   else
   {
      status = HAPIFAILURE;

      VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "Ignoring request to set Tx gain to %d dB", volumedB ));
   }

   return status;
}

/*
*****************************************************************************
** FUNCTION:   dBmToQ12
**
** PURPOSE:    Converts a dBm value to a Q.12 number via table lookup.
**
** PARAMETERS: dBmVal - power level in dBm
**
** RETURNS:    power level represented as a Q.12 number
**
** NOTE:
**
*****************************************************************************
*/
static VRG_UINT16 dBmToQ12( int dBmVal )
{
   unsigned int i;

   for(i = 0; dBmVal < dBm2Q12Map[i].dBm; i++);

   return ( dBm2Q12Map[i].q12 );
}

