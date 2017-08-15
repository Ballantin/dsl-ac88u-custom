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
*    Filename: vrgEndpt.c
*
****************************************************************************
*    Description:
*
*      Voice Residential Gateway (VRG) Endpoint abstraction layer.
*
****************************************************************************/


/*
*****************************************************************************
** INCLUDE FILES
*****************************************************************************
*/

#include <kernelMainWrapper.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>

#include <string.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <vrgEndptCfg.h>
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
#include <hdspVhdStateControl.h>

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

#include "vrgEndptPrivate.h"
#include <vrgTest.h>

#include "vrgEndptModem.h"
/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/
#define RTP_PACKET_HDR_SIZE     12

/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/

/*
 ***************************************************************************
 * EXPORTED VARIABLES
 ***************************************************************************
*/


/*
*****************************************************************************
** LOCAL VARIABLES
*****************************************************************************
*/
void vrgEndptDoDeinit( struct work_struct* data );
DECLARE_WORK( deinitWorker, &vrgEndptDoDeinit );

/* VRG command and event dispatcher task id variable */
BOS_TASK_ID vrgDispProcessTaskId;

/* VRG endpoint command Queue */
MEMCIRCBUFDRV vrgEptCmdQ;

/* VRG Event Queue */
MEMCIRCBUFDRV vrgEventQ;

/* Binary semaphore for queues services by dispatcher */
BOS_SEM       vrgDispSem;

/* Binary semaphore for synchronizing VRG endpoint API commands */
BOS_SEM       vrgEptApiSem;

/* Binary semaphore for synchronizing PSTN callback commands */
BOS_SEM       vrgPstnSem;

#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
/* Binary semaphore for synchronizing DECT callback commands */
BOS_SEM       vrgDectSem;
#endif

#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
/* Binary semaphore for synchronizing TPD callback commands */
BOS_SEM       vrgTpdSem;
#else
/* Binary semaphore for synchronizing MLT callback commands */
BOS_SEM       vrgMltSem;
#endif
#endif
/* Binary semaphore for synchronizing NTE callback commands */
BOS_SEM       vrgNteSem;

ENDPTZCONFIG endptConfig;

/*
** Mapping from PTE and CAS events to Endpoint events
*/
const EPZPTEDIGIT2EPEVTMAP digit2epevtMap[] =
{
   /* PTE event              EP event     CAS pulse evt */
   { HAPI_PTE_DTMFDDGT0,     EPEVT_DTMF0, 10                 },
   { HAPI_PTE_DTMFDDGT1,     EPEVT_DTMF1, 1                  },
   { HAPI_PTE_DTMFDDGT2,     EPEVT_DTMF2, 2                  },
   { HAPI_PTE_DTMFDDGT3,     EPEVT_DTMF3, 3                  },
   { HAPI_PTE_DTMFDDGT4,     EPEVT_DTMF4, 4                  },
   { HAPI_PTE_DTMFDDGT5,     EPEVT_DTMF5, 5                  },
   { HAPI_PTE_DTMFDDGT6,     EPEVT_DTMF6, 6                  },
   { HAPI_PTE_DTMFDDGT7,     EPEVT_DTMF7, 7                  },
   { HAPI_PTE_DTMFDDGT8,     EPEVT_DTMF8, 8                  },
   { HAPI_PTE_DTMFDDGT9,     EPEVT_DTMF9, 9                  },
   { HAPI_PTE_DTMFDDGTSTR,   EPEVT_DTMFS, CAS_CTL_EVENT_NONE },
   { HAPI_PTE_DTMFDDGTPND,   EPEVT_DTMFH, CAS_CTL_EVENT_NONE },
   { HAPI_PTE_DTMFDDGTA,     EPEVT_DTMFA, CAS_CTL_EVENT_NONE },
   { HAPI_PTE_DTMFDDGTB,     EPEVT_DTMFB, CAS_CTL_EVENT_NONE },
   { HAPI_PTE_DTMFDDGTC,     EPEVT_DTMFC, CAS_CTL_EVENT_NONE },
   { HAPI_PTE_DTMFDDGTD,     EPEVT_DTMFD, CAS_CTL_EVENT_NONE },
   { HAPI_PTE_DTMFDDGTERROR, EPEVT_NULL , CAS_CTL_EVENT_NONE }
};

/*
** Default Endpoint State
*/
VRG_ENDPT vrgEndpt[VRG_ENDPT_CFG_NUM_ENDPT];

/*
**  Note: If PSTN is enabled then we have an additional Endpoint which is of type
**  PSTN. However, this leads to the unusual situation that the number of endpoints
**  is actually greater than the number of possible connections. Thus, the size of
**  this array must be increased by 1 to compensate.
*/
CNXSTATE vrgCnxState[VRG_ENDPT_CFG_NUM_CNX+1];


/*
** Default Endpoint Capabilities
*/
EPZCAP vrgEndptCap[VRG_GLOBAL_CFG_MAX_NUM_DSP];
static const EPZCAP defaultVrgEndptCap =
{
   {10 | (1<<15), 30},     /* Packetazation period: 10-30 msec */

   { 0 },

   CODEC_NTE_DTMF |        /* Bit-encoded value representing named telephone events */
   CODEC_NTE_LCS,

   VRG_TRUE,               /* Echo cancelation */
   VRG_TRUE,               /* Silence Suppression */

   /* Connection mode capabilities */
   {
      EPCNXMODE_SNDO,            /* Send only */
      EPCNXMODE_RCVO,            /* Receive only */
      EPCNXMODE_SNDRX,           /* Send-Receive */
      EPCNXMODE_INACT,           /* Inactive */
      EPCNXMODE_CONF,            /* Conference */
      EPCNXMODE_REPLCATE,        /* Replicate */
      EPCNXMODE_TEST,            /* Network Continuity test */
      EPCNXMODE_LOOP,            /* Network Loopback test */
      EPCNXMODE_TRANSCODE,       /* Transcode */
      EPCNXMODE_MAX
   },

   EPVBDMODE_LEGACY,       /* Voice band data default mode */
   0                       /* Voice band data default max redundancy level (ie disabled) */
};

/*
** Codec Table
*/
#if 0
VRG_CODEC_TABLE_ROW   vrgCodecTable[VRG_CODEC_TABLE_MAX_ROWS];
#endif
/*
** DSP capabilities - this list needs to be expanded when we have multi-DSP support
*/
BOARD_HAL_DSP_CFG   gDspCfg;

VRG_CLASS_FSK       fskParmsPlaceHolder[VRG_ENDPT_CFG_NUM_ENDPT];

/*
*****************************************************************************
** LOCAL FUNCTION PROTOTYPES
*****************************************************************************
*/

#if defined( NTR_SUPPORT )
static void vrgEndptNtrCmd( ENDPT_STATE *endptState, VRG_UINT8 ntrAction, VRG_UINT8 isEnbl, VRG_SINT32 pcmStepsAdjust,
  VRG_UINT32 *localCount, VRG_UINT32 *ntrCount, VRG_UINT32 *ndivInt, VRG_UINT32 *ndivFrac );
#endif /* NTR_SUPPORT */
static EPSTATUS vrgEndptCapabilities( ENDPT_STATE *endptState, EPZCAP* capabilities );
static EPSTATUS vrgEndptCreateConnection( ENDPT_STATE *endptState, int cnxId, EPZCNXPARAM *cnxParam );
static EPSTATUS vrgEndptModifyConnection( ENDPT_STATE *endptState, int cnxId, EPZCNXPARAM *cnxParam );
static EPSTATUS vrgEndptDeleteConnection( ENDPT_STATE *endptState, int cnxId );
static EPSTATUS vrgEndptConsoleCmd( ENDPT_STATE *endptState, EPCONSOLECMD cmd, EPCMD_PARMS *consoleCmdParams );
static EPSTATUS vrgEndptMuteConnection( ENDPT_STATE *endptState, int cnxId, VRG_BOOL mute );
static EPSTATUS vrgEndptLinkInternal( ENDPT_STATE *endptState, ENDPT_STATE *otherEndptState, VRG_BOOL link );
static EPSTATUS vrgEndptSendCasEvt( int line, CAS_CTL_EVENT_TYPE casEvtType, CAS_CTL_EVENT casCtlEvt);
static EPSTATUS vrgEndptPacket( ENDPT_STATE   *endptState, int cnxId, EPPACKET *packet, int length, unsigned int bufDesc );
static void vrgEndptPowerSourceChanged( int newPowerSource );
static void vrgEndptGetT38Stats( ENDPT_STATE *endptState, int cnxId, EPZT38CNXSTATS *epT38Stats );
static void vrgEndptIsEnabled( int line, int *enabled);

static EPSTATUS vrgCreateEndptSemaphores(void);
static void vrgDestroyEndptSemaphores(void);

#if 0
static VRG_UINT16 GetEndptIndexFromCasChannel( VRG_UINT16 casChannel, VRG_UINT16 *endptIndex );
#endif

#if FLEXIBLE_CONFERENCING_SUPPORTED
static EPSTATUS InitCnxVhd( CNXSTATE*, VRG_BOOL, VRG_COUNTRY country );
#else
static EPSTATUS InitCnxVhd( CNXSTATE*, VRG_BOOL, VRG_BOOL, VRG_COUNTRY country );
#endif

#if 0
static VRG_CODEC_TABLE_ROW* vrgCodecTableAddNewRow( CODEC_TYPE codec, int maxSimultaneousCodec );
static void vrgCodecTableAddExistingRow( VRG_CODEC_TABLE_ROW *pCodecTableRow,
                                         CODEC_TYPE codec, int maxSimultaneousCodec );
#endif

/* VRG endpoint interface */
const ENDPT_FUNCS gVrgEndptFuncs =
{
   vrgEndptCapabilities,
   vrgEndptSignal,
   vrgEndptCreateConnection,
   vrgEndptModifyConnection,
   vrgEndptDeleteConnection,
   vrgEndptLinkInternal,
   vrgEndptPacket,
   vrgEndptConsoleCmd,
   vrgEndptPowerSourceChanged,
   vrgEndptGetRtpStats,
   vrgEndptGetT38Stats,
   vrgEndptIsEnabled,
   vrgEndptMuteConnection,
   vrgEndptSendCasEvt,
#if defined( NTR_SUPPORT )
   vrgEndptNtrCmd
#endif /* NTR_SUPPORT */
};

#if 0
static void GetT38Stats( CNXSTATE *cnx );
#endif
static RTPPKT_TYPE RtpGetPacketType( RTPSTATE *state, RTPPACKET* rtpPacket, int length, VRG_BOOL bGR303Support );
static VRG_UINT8 RtpGetNtePayloadType(RTPSTATE *state, VRG_UINT8 sendFlag);
static void RtpParsePacket( RTPSTATE* state, RTPPACKET* rtpPacket, int length);

#if 0
static void SetRtpThresholdMonitor( CNXSTATE *cnx );
static void GetRtpThresholdMonitor( CNXSTATE *cnx );
#endif

#define VRG_DATA_MODE_T38(mode) \
   (((mode) == EPDATAMODE_T38 || (mode) == EPDATAMODE_T38_MUTE) ? 1 : 0 )

/*
*****************************************************************************
** EXTERNS
*****************************************************************************
*/
extern void finishVrgEndptDeinit(void);

/*
*****************************************************************************
** FUNCTION:   vrgEndptInit
**
** PURPOSE:    Module initialization for the VRG endpoint. The endpoint
**             module is responsible for controlling a set of endpoints.
**             Individual endpoints are initialized using the vrgEndptCreate() API.
**
** PARAMETERS: endptInitCfg      - pointer to endpt cfg block
**             notifyCallback    - Callback to use for event notification
**             packetCallback    - Callback to use for endpoint packets
**             provisionCallback - Callback to get provisioned values. May be
**                                 set to NULL.
**             packetReleaseCallback - Callback to release ownership of
**                                        endpoint packet back to caller
**             taskShutdownCallback - Callback invoked to indicate endpt task
**                                    shutdown
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS vrgEndptInit
(
   VRG_ENDPT_INIT_CFG        *endptInitCfg,
   endptEventCallback         notifyCallback,
   endptPacketCallback        packetCallback,
   endptGetProvCallback       getProvisionCallback,
   endptSetProvCallback       setProvisionCallback,
   endptPacketReleaseCallback packetReleaseCallback,
   endptTaskShutdownCallback  taskShutdownCallback
)
{
   signed int        i, j;
   EPSTATUS          status;

   VRG_COUNTRY       country = endptInitCfg->country;
   HBSTATE           initPowerState;
   VRG_UINT32        provItem;
#ifndef BRCM_NONDIST_BUILD
   XDRV_DLB         *dlbDrv = boardHalDspGetDlbDriver();
#endif /* BRCM_NONDIST_BUILD */
   int               lastLocaleId = 0;

   memset( &endptConfig, 0, sizeof( ENDPTZCONFIG ));

#ifdef VRG_ENDPT_2G3G_SUPPORT
   memset( (void*)gCSMedia, 0, sizeof (MODEM_STREAM_INFO) * MODEM_MAX_AUDIO_STREAM);
#endif

#if VRG_ENDPT_CFG_FORCE_PROV_INTEGRITY_CHECK
   if( endptProvIntegrityCheck() != EPSTATUS_SUCCESS )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR endpoint provision database is not set properly" ));
   }
#endif /* VRG_ENDPT_CFG_FORCE_PROV_INTEGRITY_CHECK */

   /* conditional copy of default values to provis if they are undefined in provis */
   endptProvInitDefaults(country);

   /* Optionally set defaults for new locale in case of a change */
   Provision( NULL, EPPROV_LastLocale, &lastLocaleId );
   if( lastLocaleId != country )
   {
      endptProvResetDefaults(country);
      endptProvSetValue(0, EPPROV_LastLocale, &country, sizeof(int));
   }

   /*
   ** Retrieve DSP capabilities
   */
   boardHalDspGetCfg( &gDspCfg );

   /* Initialize heartbeat monitor module */
   hbInit();
   endptConfig.initState |= EPEINIT_HEARTBEAT;

   /* Shutdown callback no-longer supported. */
   (void) taskShutdownCallback;

   endptConfig.country           = country;
   endptConfig.notify            = notifyCallback;
   endptConfig.packet            = packetCallback;
   endptConfig.getProvision      = getProvisionCallback;
   endptConfig.setProvision      = setProvisionCallback;
   endptConfig.packetRelease     = packetReleaseCallback;
   endptConfig.acPowerHBId       = -1;

   for( i = 0; i < vrgEndptGetNumEndpoints(); i++)
   {
      memset( &vrgEndpt[i], 0, sizeof( vrgEndpt[i] ));
      vrgEndpt[i].lineVhdHdl      = 0xffff;
      vrgEndpt[i].casCtlHandle    = 0xffff;
      vrgEndpt[i].pstnVhdHdl      = 0xffff;
      vrgEndpt[i].confCnxVhdHdl   = 0x00;
      vrgEndpt[i].country         = country;
      vrgEndpt[i].casCtlBits      = EP_CAS_CTL_LCF;
      vrgEndpt[i].testMode        = TESTMODE_NONE;
      vrgEndpt[i].hookStateHBId   = -1;
      vrgEndpt[i].routedFxsLineId = -1;
      vrgEndpt[i].dspNum          = i / (VRG_ENDPT_CFG_NUM_ENDPT/VRG_GLOBAL_CFG_MAX_NUM_DSP);
      vrgEndpt[i].bGR303Support   = VRG_FALSE;
   }

   for( i = 0; i < VRG_ENDPT_CFG_NUM_CNX; i++)
   {
      memset( &vrgCnxState[i], 0, sizeof( vrgCnxState[i] ));
      vrgCnxState[i].vrgVoiceJBMin    = HAPI_PVE_MIN_JITTER_REG_DEFAULT;
      vrgCnxState[i].vrgVoiceJBMax    = HAPI_PVE_MAX_JITTER_REG_DEFAULT;
#ifdef VRG_ENDPT_CFG_G168_TEST
      /* 150ms fixed jitter buffer is used to accommodate for observed large 
      ** jitter at beginning of G.168 calls */
      vrgCnxState[i].vrgVoiceJBFixed  = VRG_TRUE;
      vrgCnxState[i].vrgVoiceJBTarget = 150; 
#else
      vrgCnxState[i].vrgVoiceJBFixed  = VRG_FALSE;
      vrgCnxState[i].vrgVoiceJBTarget = HAPI_PVE_TARGET_JITTER_REG_DEFAULT;
#endif /* VRG_ENDPT_CFG_G168_TEST */
      vrgCnxState[i].vrgDataJBTarget  = HAPI_PVE_VBD_TARGET_JITTER_REG_DEFAULT;
      vrgCnxState[i].cnxId            = CNX_UNUSED;
      vrgCnxState[i].lineId           = BAD_ENDPTID;
      vrgCnxState[i].mode             = EPCNXMODE_INACT;
      vrgCnxState[i].voiceMode        = VM_IDLE;
      vrgCnxState[i].vhdMode          = NETMODE_IDLE;
      vrgCnxState[i].dataMode         = EPDATAMODE_VBD;
      vrgCnxState[i].cnxStateHBId     = -1;
      vrgCnxState[i].dspNum           = i / (VRG_ENDPT_CFG_NUM_CNX/VRG_GLOBAL_CFG_MAX_NUM_DSP);
      vrgCnxState[i].bMuteAudio       = VRG_FALSE;
      vrgCnxState[i].bMuteT38         = VRG_FALSE;
      vrgCnxState[i].t38param.ecMode  = EPT38EC_UDPREDUNDANCY;
      vrgCnxState[i].VadMode          = -1;
      vrgCnxState[i].CngMode          = -1;
      vrgCnxState[i].PlcMode          = -1;
      vrgCnxState[i].localSsrc        = 0;

      /*
      ** Set custom registers for AJC and FAXR
      */
      vrgCnxState[i].ajcCustomReg     = HAPI_PVE_AJC_CUSTOM_DEFAULT;
      vrgCnxState[i].faxrCustomReg    = HAPI_FAXR_CUSTOM_DEFAULT;

      /*
      ** Initialize RTP state
      */
      RtpInit( &vrgCnxState[i].rtpstate );
   }

   endptConfig.initState |= EPEINIT_CNXSTATE;


   /* Set the default endpoint capabilities */
   for( i = 0 ; i < VRG_GLOBAL_CFG_MAX_NUM_DSP ; i++ )
   {
      vrgEndptCap[i] = defaultVrgEndptCap;
   }


   /* Determine the initial state of the AC power heartbeat patient.  If we
   ** are initially running on batteries, we want to enable power savings
   ** immediately.  If, however, we are running on AC, we'll still register
   ** with the heartbeat module so that when we later switch to batteries,
   ** power savings can be initiated */
   initPowerState = ( ( endptInitCfg->currentPowerSource ) ? HBSTATE_INACTIVE : HBSTATE_ACTIVE );
   hbRegisterPatient( initPowerState, HBPATIENT_TYPE_POWER,
                      HBPATIENT_ALL_ENDPT, &endptConfig.acPowerHBId );


   /*
   ** Copy vocoder and VHD capabilities from the DSP
   **
   ** Note: it is assumed that in the case of multiple DSPs, the same DSP image is used
   **       on all of them, therefore the same capability apply.
   */
   for( i = 0 ; i < VRG_GLOBAL_CFG_MAX_NUM_DSP ; i++ )
   {
      for (j=0; j<CODEC_MAX_TYPES; j++)
      {
         vrgEndptCap[i].codecCap[j] = gDspCfg.codecCapabilities[j];
      }

      /*
      ** Copy VBD capabilities from the DSP
      */
      vrgEndptCap[i].vbdCap = gDspCfg.vbdCapability;
      /*
      ** Copy RFC 2198 capabilities from the DSP
      */
      vrgEndptCap[i].redLvl = gDspCfg.maxRfc2198Level;
   }

   /* Initialize circular buffer driver */
   if ( memCircBufInit() != BUF_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error initializing memCircBuf driver"));
      return ( EPSTATUS_ERROR );
   }

   /*
   ** Low-level HAPI module init.
   */
   lhapiInit( gDspCfg.isDistributed );
   endptConfig.initState |= EPEINIT_LHAPI;

   /*
   ** Initialize the VRG dispatcher task to process
   ** endpoint commands and events
   */
   if( VrgDispatcherProcessInit() )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR starting VRGDISP service" ));
      return ( EPSTATUS_RESOURCES );
   }
   endptConfig.initState |= EPEINIT_VRGDISP;

   vrgCreateEndptSemaphores();

   /*
   ** Refresh line voltage state if needed.
   */
   boardHalSlicHvgRefresh();

   /*
   ** Initialize CAS control for phones
   */
   if( casCtlInit( CasEventCallback ) != CAS_CTL_SUCCESS )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR starting CAS service" ));
      return ( EPSTATUS_RESOURCES );
   }
   endptConfig.initState |= EPEINIT_CAS;

#if VRG_ENDPT_CFG_USE_TPD
   /*
   ** Initialize TPD testing control for endpoints
   */
#ifndef VRG_ENDPT_CFG_USE_MLT
   if( tpdCtlInit( TpdEventCallback ) != TPD_CTL_SUCCESS )
#else
   if( mltInit( MltCallback ) != MLT_SUCCESS)
#endif
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR starting TPD service" ));
      return ( EPSTATUS_RESOURCES );
   }
#endif
   endptConfig.initState |= EPEINIT_TPD;

   /*
   ** Init HHAPI
   */
   if ( HAPISUCCESS != hdspInit( RmEventCallback, VRG_GLOBAL_CFG_MAX_NUM_DSP ))
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR starting HHAPI service on DSP %d", i ));
      return ( EPSTATUS_RESOURCES );
   }
   endptConfig.initState |= EPEINIT_HDSP;

   /* Initialize the VHD state control block */
   hdspVhdStateControlInit();

#if BRCM_TRACK_IDLE_STATUS
   idleStatusWatchDogInit();
#endif /* BRCM_TRACK_IDLE_STATUS */

   /*
   ** Determine if DSP load balancing is required. Default to disabled.
   */
   Provision( NULL, EPPROV_LoadBalancing, &provItem );

#ifdef CONFIG_SMP
   /*
   ** Register callback with board HAL that should be invoked periodically
   ** to invoke the hausware kernel.
   */
   boardHalDspRegisterCallback( lhapiKNL_RunReadyThreads );
#else
#ifndef BRCM_NONDIST_BUILD
   xdrvDlbEnable( dlbDrv, provItem );

   /*
   ** Indicate to the DSP that IPC init data is available
   */
   for( i = 0 ; i < VRG_GLOBAL_CFG_MAX_NUM_DSP ; i++ )
   {
      boardHalIpcReady( i );
   }

   /*
   ** Register callback with board HAL that should be invoked periodically
   ** to invoke the hausware kernel.
   */
   boardHalDspRegisterCallback( lhapi_ProcessEntrySync );

   /* Start the DSP task timer */
   boardHalDspTaskStartTimer();
#endif /* BRCM_NONDIST_BUILD */
#endif /* CONFIG_SMP */

#ifdef BRCM_NONDIST_BUILD
   /* Enable PCM or APM interrupts */
   boardHalIsrEnable();
#endif /* BRCM_NONDIST_BUILD */

#ifdef G729_DSPTEST_APP
   dspTestInit();
   while ( 1 )
   {
      bosSleep(1000);
   }
#endif

   /*
   ** Verify VHD endpoint capabilities
   */
   if (( gDspCfg.maxEndPts < VRG_ENDPT_CFG_NUM_ENDPT/VRG_GLOBAL_CFG_MAX_NUM_DSP ) ||
       ( (VRG_SINT16)gDspCfg.maxFullCnxs < (VRG_ENDPT_CFG_NUM_CNX - VRG_ENDPT_CFG_NUM_LITE_CNX)/VRG_GLOBAL_CFG_MAX_NUM_DSP) ||
       (( gDspCfg.maxFullCnxs + gDspCfg.maxLiteCnxs ) < VRG_ENDPT_CFG_NUM_CNX/VRG_GLOBAL_CFG_MAX_NUM_DSP ))
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "gDspCfg.maxEndPts: %d", gDspCfg.maxEndPts ));
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "gDspCfg.maxFullCnxs: %d", gDspCfg.maxFullCnxs));
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "gDspCfg.maxFullCnxs + gDspCfg.maxLiteCnxs: %d", gDspCfg.maxFullCnxs + gDspCfg.maxLiteCnxs ));
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR - Insufficient number of endpoints or connections on the DSP" ));
      return ( EPSTATUS_RESOURCES );
   }

   /*
   ** Initialize the pool of secondary connection VHDs - these VHDs are not bound
   ** to a specific endpt, they are free to be used by any endpoint - first come,
   ** first serve.
   */
   for ( j = 0 ; j < VRG_GLOBAL_CFG_MAX_NUM_DSP ; j++ )
   {
#if FLEXIBLE_CONFERENCING_SUPPORTED
      for ( i = 0; i < VRG_ENDPT_CFG_NUM_CNX/VRG_GLOBAL_CFG_MAX_NUM_DSP; i++ )
#else
      for ( i = VRG_ENDPT_CFG_NUM_ENDPT/VRG_GLOBAL_CFG_MAX_NUM_DSP; i < VRG_ENDPT_CFG_NUM_CNX/VRG_GLOBAL_CFG_MAX_NUM_DSP ; i++ )
#endif
      {
         VRG_BOOL  bLiteWeightCnx = VRG_FALSE;

         if( i >= ((VRG_ENDPT_CFG_NUM_CNX - VRG_ENDPT_CFG_NUM_LITE_CNX)/VRG_GLOBAL_CFG_MAX_NUM_DSP) )
         {
            bLiteWeightCnx = VRG_TRUE;
         }

         /* Parms: cnxState, primaryCnx, lite-weightCnx */
#if FLEXIBLE_CONFERENCING_SUPPORTED
         status = InitCnxVhd( &vrgCnxState[j*(VRG_ENDPT_CFG_NUM_CNX/VRG_GLOBAL_CFG_MAX_NUM_DSP)+i], bLiteWeightCnx,
                              country );
#else
         status = InitCnxVhd( &vrgCnxState[j*(VRG_ENDPT_CFG_NUM_CNX/VRG_GLOBAL_CFG_MAX_NUM_DSP)+i], VRG_FALSE, bLiteWeightCnx,
                              country );
#endif
         if ( status != EPSTATUS_SUCCESS )
         {
            return ( status );
         }
      }
   }
   endptConfig.initState |= EPEINIT_CNXVHD;

   if ( vrgEndptGetNumFxoEndpoints() > 0 )
   {
      if ( PSTN_CTL_SUCCESS == pstnCtlInit( endptConfig.country, PstnEventCallback ) )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "pstnCtlInit successful" ));
      }
      else
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "pstnCtlInit FAIL" ));
      }
   }

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   vrgCreateEndptSemaphores
**
** PURPOSE:    Creates all the binary semaphores used by the dispatcher to
**             communicate with various endpoint tasks.
**
** PARAMETERS:
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
static EPSTATUS vrgCreateEndptSemaphores(void)
{
   /* Create semaphore for VRG endpoint API commands */
   if ( bosSemCreate("vrgEptApiSem", 0, 1, &vrgEptApiSem) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgEptApiSem"));
      return ( EPSTATUS_ERROR );
   }

   /* Create semaphore for PSTN callback commands */
   if ( bosSemCreate("vrgPstnSem", 0, 1, &vrgPstnSem) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgPstnSem"));
      return ( EPSTATUS_ERROR );
   }

#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
   /* Create semaphore for DECT callback commands */
   if ( bosSemCreate("vrgDectSem", 0, 1, &vrgDectSem) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgDectSem"));
      return ( EPSTATUS_ERROR );
   }
#endif

#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
   /* Create semaphore for TPD callback commands */
   if ( bosSemCreate("vrgTpdSem", 0, 1, &vrgTpdSem) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgTpdSem"));
      return ( EPSTATUS_ERROR );
   }
#else
   /* Create semaphore for MLT callback commands */
   if ( bosSemCreate("vrgMltSem", 0, 1, &vrgMltSem) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgMltSem"));
      return ( EPSTATUS_ERROR );
   }
#endif
#endif
   /* Create semaphore for NTE callback commands */
   if ( bosSemCreate("vrgNTESem", 0, 1, &vrgNteSem) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgNteSem"));
      return ( EPSTATUS_ERROR );
   }

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   vrgDestroyEndptSemaphores
**
** PURPOSE:    Destroys all the binary semaphores used by the dispatcher to
**             communicate with various endpoint tasks.
**
** PARAMETERS:
**
** RETURNS:
**
** NOTE:
*****************************************************************************
*/
static void vrgDestroyEndptSemaphores(void)
{
   /* Destroy all endpoint semahpores */
   bosSemDestroy( &vrgEptApiSem );
   bosSemDestroy( &vrgPstnSem );

#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
   bosSemDestroy( &vrgDectSem );
#endif

#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
   bosSemDestroy( &vrgTpdSem );
#else
   bosSemDestroy( &vrgMltSem );
#endif
#endif
   bosSemDestroy( &vrgNteSem );
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptCreate
**
** PURPOSE:    Send command to dispatcher to create an endpt object.
**
** PARAMETERS: physId      (in)  Physical interface.
**             lineId      (in)  Endpoint line identifier.
**             endptState  (out) Created endpt object.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptCreate( int physId, int lineId, VRG_ENDPT_STATE *endptState )
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_CREATE;
   vrgCmd.arguments[0] = (unsigned int)physId;
   vrgCmd.arguments[1] = (unsigned int)lineId;
   vrgCmd.arguments[2] = (unsigned int)endptState;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }

   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptDestroy
**
** PURPOSE:    Send command to dispatcher to destroy an endpt object.
**
** PARAMETERS: endptState  (mod) Created endpt object.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptDestroy( VRG_ENDPT_STATE *vrgEndptState )
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_DESTROY;
   vrgCmd.arguments[0] = (unsigned int)vrgEndptState;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptDeinit
**
** PURPOSE:    VRG endpoint module shutdown - call once during system shutdown.
**             This will shutdown all endpoints and free all resources used by
**             the VRG endpt.
**             This queues an event for the endpoint to process later.
**
** PARAMETERS: none
**
** RETURNS:    EPSTATUS
**
** NOTE:
**
*****************************************************************************
*/
EPSTATUS vrgEndptDeinit( void )
{
   /* Start the deinit worker and return */
   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "Endpoint deinitialization posted." ));
   schedule_work(&deinitWorker);
   return EPSTATUS_SUCCESS;
}

/*****************************************************************************
*  FUNCTION:   InitCnxVhd
*
*  PURPOSE:
*
*  PARAMETERS: cnx         - connection state structure
*              bPrimaryCnx - TRUE if the connection to be initialized is the
*                            primary connection. Each endpoint has a dedicated
*                            connection VHD associated with it, known as the
*                            primary VHD.
*              bLiteWeightCnx - TRUE if the connection to be initialized is a
*                               lite-weight connection. Lite-weight connections
*                               do not support advanced vocoders.
*              country     - country code
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/

#if FLEXIBLE_CONFERENCING_SUPPORTED
static EPSTATUS InitCnxVhd( CNXSTATE *cnx, VRG_BOOL bLiteWeightCnx,
                            VRG_COUNTRY country )
#else
static EPSTATUS InitCnxVhd( CNXSTATE *cnx, VRG_BOOL bPrimaryCnx,
                            VRG_BOOL bLiteWeightCnx, VRG_COUNTRY country )
#endif
{
   HSZNETREGS     netregs;
   VRG_UINT16     vhdType;
   VRG_SINT16     result;
   const char    *cnxType;
   VRG_UINT16     status;
   HAPI_PTE_ADM   pteAdmin;
   VRG_UINT32     minDetectPower;
   VRG_UINT32     pveEgressVolume;

#if !FLEXIBLE_CONFERENCING_SUPPORTED
   if ( bPrimaryCnx )
   {
      cnxType = "Primary";
   }
   else
#endif
   {
      cnxType = "Secondary";
   }

   if ( bLiteWeightCnx )
   {
      vhdType = HAPI_CONFERENCE_VHD;
   }
   else
   {
      vhdType = HAPI_GATEWAY_VHD;
   }

   /*
   ** Open a Connection VHD
   */
   /* Retrieve the country specific PVE/PTE settings */
   Provision(NULL, EPPROV_PteDetectAdmin, &pteAdmin);
   Provision(NULL, EPPROV_PteMinDetectPower, &minDetectPower);
   Provision(NULL, EPPROV_PveEgressVolume, &pveEgressVolume);

   status = hdspVhdOpen( vhdType, CnxVhdEventCallback, cnx->dspNum,
                         pteAdmin, minDetectPower, pveEgressVolume,
                         &(cnx->vhdhdl) );

   if( status != HAPISUCCESS )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR hdspVhdOpen Connection VHD failed. DSP %d", cnx->dspNum ));
      return ( EPSTATUS_RESOURCES );
   }
   else
   {
      VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "hdspVhdOpen %s Connection VHD success. DSP %d, VHD (0x%x) of type: 0x%x",
                  cnxType, cnx->dspNum, cnx->vhdhdl, vhdType ));
   }

   /*
   ** Retrieve the VHD registers for configuration
   */
   result = hdspSendCmdData( cnx->vhdhdl,
                             HAPINET_CONFIG_GETREGS,
                             HSCMDRESP,
                             sizeof( netregs ),
                             &netregs);

   if( result )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR getting registers for vhd %d", cnx->vhdhdl ));
   }

   /*
   ** Disable DTMF relay, automatic encoder switch during mode switch,
   ** event reporting (DTMF events are reported by the endpoint VHD),
   ** ECAN control on the NET, as well as FAXR.
   ** DTMF and FAX reley will get reenabled if negotiated via MGCP.
   */

   netregs.hsxPtePacketVoiceOptions &= (~HSNET_PTE_OPTION_DIG_RELAY);
   netregs.hsxPtePacketVoiceOptions &= (~HSNET_PTE_OPTION_DIG_EVT);
   netregs.hsxNetOptions            &= ~(HSNET_NET_OPTION_AUTO_ENC_SWITCH |
                                         HSNET_NET_OPTION_ECAN_CONTROL |
                                         HSNET_NET_OPTION_FAXRELAY);
   netregs.hsxPteCallSetupOptions   &= (~HSNET_PTE_OPTION_DIG_EVT);
   netregs.hsxCallDOptions          |= ( HSNET_CDIS_OPTION_V18ACTIVE | HSNET_CDIS_OPTION_BELL103ACTIVE | HSNET_CDIS_OPTION_CUSTOMTONEACTIVE );

   cnx->netRegs = netregs;

   /*
   ** Send new configuration to the DSP
   */
   result = hdspSendCmdData( cnx->vhdhdl,
                             HAPINET_CONFIG_SETREGS,
                             HSCMDEXTDATA_SYNC,
                             sizeof( netregs ),
                             &netregs);

   if( result )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting registers for line %d", cnx->lineId ));
   }

   /*
   ** Configure the redundancy level if the DSP image supports T.38
   */
   if ( vrgEndptCap[cnx->dspNum].codecCap[CODEC_T38] == CODEC_SUPPORTED )
   {
      result = SetT38EcMode( cnx, cnx->t38param.ecMode );

      if( result )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting T.38 registers for vhd %d", cnx->vhdhdl ));
      }

      /*
      ** Configure faxr service to send faxr events
      */
      result = hdspSendCmd( cnx->vhdhdl,
                            HAPINET_FAXR_SETREG1,
                            HSCMDDATA,
                            OFFSETOF( HSZFAXRREGS, hsxfaxrsendevents ),
                            1 );

      if( result )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting faxr registers for vhd %d", cnx->vhdhdl ));
      }

      /*
      ** Configure faxr service custom register
      */
      result = hdspSendCmd( cnx->vhdhdl,
                            HAPINET_FAXR_SETREG1,
                            HSCMDDATA,
                            OFFSETOF( HSZFAXRREGS, hsxcustomfeatures ),
                            cnx->faxrCustomReg );

      if( result )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting faxr registers for vhd %d", cnx->vhdhdl ));
      }

   }

   /* Configure Auto Switch to VBD for V18 Baudot and Preamble, but not for V18 Ascii (Bell 103) */
   result = hdspSendCmdAsync( cnx->vhdhdl,
                              HAPI_NET_VBD_AUTOSWITCH_MASK,
                              HSCMDDATA,
                              (HAPI_NET_AUTOSWITCH_MASK_V18_BAUDOT | HAPI_NET_AUTOSWITCH_MASK_V18_PREAMBLE),
                              0 );

   if( result )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting VBD auto switch for line %d", cnx->lineId ));
   }

   /*
   ** Update connection state
   */
   cnx->bLiteWeight = bLiteWeightCnx;
#if !FLEXIBLE_CONFERENCING_SUPPORTED
   cnx->bPrimary = bPrimaryCnx;
#endif


   /*
   ** Set the channel into idle mode
   */
   SetVoiceMode( cnx, VM_IDLE );


   /*
   ** Register this connection with the heartbeat monitor.  The 'endpt'
   ** value will be updated in the heartbeat module when the cnx
   ** is actually engaged.
   */
   hbRegisterPatient( HBSTATE_INACTIVE, HBPATIENT_TYPE_CNX,
                      HBPATIENT_UNKNOWN_ENDPT, &cnx->cnxStateHBId );

   /* Set Jitter buffer registers */
   result = hdspVhdSetJitterBuffer( cnx->vhdhdl,
                                    cnx->vrgVoiceJBFixed,
                                    cnx->vrgVoiceJBMin,
                                    cnx->vrgVoiceJBMax,
                                    cnx->vrgVoiceJBTarget,
                                    cnx->vrgDataJBTarget,
                                    cnx->ajcCustomReg );
   if( result )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting jitter buffer registers for Gateway VHD %d", cnx->vhdhdl ));
   }

   return ( EPSTATUS_SUCCESS );
}


/*****************************************************************************
*  FUNCTION:   vrgEndptGetNumEndpoints
*
*  PURPOSE:    Retrieve the number of endpoints in a collection
*
*  PARAMETERS: None
*
*  RETURNS:    Number of endpoints
*
*****************************************************************************/
int vrgEndptGetNumEndpoints( void )
{
   return( boardHalGetNumEndpoints() );
}


/*****************************************************************************
*  FUNCTION:   vrgEndptGetNumFxsEndpoints
*
*  PURPOSE:    Retrieve the number of FXO endpoints in a collection
*
*  PARAMETERS: None
*
*  RETURNS:    Number of FXO endpoints
*
*****************************************************************************/
int vrgEndptGetNumFxsEndpoints( void )
{
   return( boardHalGetNumFxsEndpoints() );
}



/*****************************************************************************
*  FUNCTION:   vrgEndptGetNumFxoEndpoints
*
*  PURPOSE:    Retrieve the number of FXO endpoints in a collection
*
*  PARAMETERS: None
*
*  RETURNS:    Number of FXO endpoints
*
*****************************************************************************/
int vrgEndptGetNumFxoEndpoints( void )
{
   return( boardHalGetNumFxoEndpoints() );
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptGetNumDectEndpoints
**
** PURPOSE:    Retrieve the number of DECT endpoints in a collection
**
** PARAMETERS: None
**
** RETURNS:    Number of DECT endpoints
**
*****************************************************************************
*/
int vrgEndptGetNumDectEndpoints( void )
{
   return ( boardHalGetNumDectEndpoints() );
}

#if PSTN_LIFE_LINE_SUPPORT
/*
*****************************************************************************
** FUNCTION:   vrgEndptIsPstnLifeLineSupported
**
** PURPOSE:    Determines whether pstn life line is supported
**
** PARAMETERS: None
**
** RETURNS:    true if supported; false otherwise
**
*****************************************************************************
*/
VRG_BOOL vrgEndptIsPstnLifeLineSupported( void )
{
   return ( boardHalIsPstnLifeLineSupported() );
}
#endif

#if defined( NTR_SUPPORT )
/*****************************************************************************
*  FUNCTION:   vrgEndptNtrCmd
*
*  PURPOSE:    Send command to the dispatcher to execute an NTR command.
*
*  PARAMETERS: ntrParm structure filled in with the new Ndiv_frac value for the
*              PCM feedback control register
*
*  RETURNS:    ntrParm->status = EPSTATUS_SUCCESS if PCM register write is success
*              EPSTATUS_NOTREADY otherwise
*
*****************************************************************************/
static void vrgEndptNtrCmd
(
   ENDPT_STATE       *endptState,
   VRG_UINT8         ntrAction,
   VRG_UINT8         isEnbl,
   VRG_SINT32        pcmStepsAdjust,
   VRG_UINT32        *localCount,
   VRG_UINT32        *ntrCount,
   VRG_UINT32        *ndivInt,
   VRG_UINT32        *ndivFrac
)
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_NTR_CMD;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)ntrAction;
   vrgCmd.arguments[2] = (unsigned int)isEnbl;
   vrgCmd.arguments[3] = (unsigned int)pcmStepsAdjust;
   vrgCmd.arguments[4] = (unsigned int)localCount;
   vrgCmd.arguments[5] = (unsigned int)ntrCount;
   vrgCmd.arguments[6] = (unsigned int)ndivInt;
   vrgCmd.arguments[7] = (unsigned int)ndivFrac;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
      }

   }

   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
   }
}
#endif /* NTR_SUPPORT */

/*****************************************************************************
*  FUNCTION:   vrgEndptCapabilities
*
*  PURPOSE:    Send command to dispatcher to retrieve endpt capabilities.
*
*  PARAMETERS: endptState   - a pointer to endpoint state structure
*              capabilities - a pointer to the capabilities struct to populate
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptCapabilities
(
   ENDPT_STATE   *endptState,
   EPZCAP        *capabilities
)
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_CAPABILITIES;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)capabilities;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

/*****************************************************************************
*  FUNCTION:   vrgEndptSignal
*
*  PURPOSE:    Send command to dispatcher to generate a signal on the endpt.
*
*  PARAMETERS: endptState - a pointer to endpoint state structure
*              cnxId  - connection identifier (-1 if not applicable)
*              signal - signal type code (see EPSIG)
*              value  - signal value
*                       BR signal types - 1
*                       OO signal types - 0 == off, 1 == on
*                       TO signal types - 0 = stop/off, 1= start/on
*                       String types - (char *) cast to NULL-term string value
**             duration   - pulse duration (-1 if not used)
**             period     - pulse period (-1 if not used)
**             repetition - depending on the pulsed signal, this parameter
**                          has different meaning:
**                            - 'em': defines the report pulse count (rpc)
**                            - other pulsed signals: defines the repetition (rep)
**                          (-1 if not used)
*
*  RETURNS:    EPSTATUS
*
*****************************************************************************/
EPSTATUS vrgEndptSignal
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPSIG          signal,
   unsigned int   value,
   int            duration,
   int            period,
   int            repetition
)
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_SIGNAL;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)cnxId;
   vrgCmd.arguments[2] = (unsigned int)&signal;
   vrgCmd.arguments[3] = (unsigned int)value;
   vrgCmd.arguments[4] = (unsigned int)duration;
   vrgCmd.arguments[5] = (unsigned int)period;
   vrgCmd.arguments[6] = (unsigned int)repetition;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

/*****************************************************************************
*  FUNCTION:   vrgEndptCreateConnection
*
*  PURPOSE:    Send command to dispatcher to create a connection.
*
*  PARAMETERS: endptState - pointer to endpoint state structure
*              cnxId      - connection identifier
*              cnxParam   - ptr to connection parameter block
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptCreateConnection
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPZCNXPARAM   *cnxParam
)
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_CREATE_CNX;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)cnxId;
   vrgCmd.arguments[2] = (unsigned int)cnxParam;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

/*****************************************************************************
*  FUNCTION:   vrgEndptModifyConnection
*
*  PURPOSE:    Send command to the dispatcher to modify a connection.
*
*  PARAMETERS: endptState - pointer to endpoint state structure
*              cnxId      - connection identifier
*              mute       - mute status of the connection
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptModifyConnection
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPZCNXPARAM   *cnxParam
)
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_MODIFY_CNX;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)cnxId;
   vrgCmd.arguments[2] = (unsigned int)cnxParam;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

/*****************************************************************************
*  FUNCTION:   vrgEndptDeleteConnection
*
*  PURPOSE:    Send command to dispatcher to delete a connection.
*
*  PARAMETERS: endptState - a pointer to endpoint state structure
*              cnxId      - connection identifier
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptDeleteConnection( ENDPT_STATE *endptState, int cnxId )
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_DELETE_CNX;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)cnxId;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

/*****************************************************************************
*  FUNCTION:   vrgEndptGetRtpStats
*
*  PURPOSE:    Send command to dispatcher to get RTP statistics.
*
*  PARAMETERS: endptstate         - endpt state
*              cxid               - connection id
*              epRtpStats         - statistics to be populated
*
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptGetRtpStats( ENDPT_STATE *endptState, int cnxId, EPZCNXSTATS *epRtpStats, VRG_BOOL bReset )
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_GET_RTP_STATS;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)cnxId;
   vrgCmd.arguments[2] = (unsigned int)epRtpStats;
   vrgCmd.arguments[3] = (unsigned int)bReset;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

/*****************************************************************************
*  FUNCTION:   vrgEndptGetT38Stats
*
*  PURPOSE:    Send command to dispatcher to get T38 statistics.
*
*  PARAMETERS: endptstate         - endpt state
*              cxid               - connection id
*              epT38Stats         - statistics to be populated
*
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
static void vrgEndptGetT38Stats( ENDPT_STATE *endptState, int cnxId,
                                 EPZT38CNXSTATS *epT38Stats )
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_GET_T38_STATS;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)cnxId;
   vrgCmd.arguments[2] = (unsigned int)epT38Stats;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
   }
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptPacket
**
** PURPOSE:    Transmit a endpt packet to the endpoint. This function is asynchronous,
**             it does not wait for the data to be transmitted  before returning.
**             In addition, the buffer release mechanism is asynchronous.
**             The endpoint owns the packet passed to this function until a callback
**             is invoked releasing the packet back to the caller (endptPacketReleaseCallback).
**             The value of 'bufDesc' will be passed to the callback function so that
**             the caller can identify which packet is being released by the endpoint.
**
** PARAMETERS: endptState - a pointer to endpoint state structure
**             cnxId      - the connection index
**             epPacket   - pointer to the EPPACKET packet (RTP or T.38 UDP)
**             length     - the length of the RTP packet including RTP header
**             bufDesc    - buffer descriptor value that uniquely identifies
**                          the endpt packet. This value is returned in the
**                          callback invoked once the buffer has been completely
**                          transmitted. This value is only meaningful to the
**                          caller and is opaque to the endpoint.
**
** RETURNS:    EPSTATUS
**
** NOTE:       If this function does not return EPSTATUS_SUCCESS, it is the caller's
**             responsibility to free the packet.
**
*****************************************************************************
*/
EPSTATUS vrgEndptPacket
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPPACKET      *epPacket,
   int            length,
   unsigned int   bufDesc
)
{
   VRG_ENDPT*        state;
   CNXSTATE*         cnx;
   int               cnxIndex;
   VRG_BOOL          packetSent;
   RTPPKT_TYPE       rtpType;
   EPSTATUS          status = EPSTATUS_SUCCESS;

   /*
   ** Get the connection state
   */
   cnx = GetCnxState( cnxId );

   if( ! cnx )
   {
      status = EPSTATUS_CNX_UNKNOWN;
      VRG_LOG_DBG(( VRG_LOG_MOD_EPT, "%s status %d", __FUNCTION__, status))
      return( status );
   }

   /* Just return if this is CS call */
   if ( cnx->csCall ) {
      return ( EPSTATUS_SUCCESS );
   }

   state = GetEndptState( cnx->lineId );
   if( ! state )
   {
      status = EPSTATUS_ENDPT_UNKNOWN;
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "%s status %d", __FUNCTION__, status))
      return( status );
   }

   /*
   ** Determine the cnx index for the endpoint
   */
   for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
   {
      if (state->cnx[cnxIndex] == cnx)
      {
         /* cnx index found */
         break;
      }
   }
   if (cnxIndex == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT)
   {
      /* specified cnx is not within the endpoint's cnx list */
      status = EPSTATUS_CNX_UNKNOWN;
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "%s status %d", __FUNCTION__, status))
      return( status );
   }

   /*
   ** first valid RTP packet in receive mode, generate the Media Start event.
   */
   if( cnx->firstValidRTPPkt && ( EPMEDIATYPE_RTP == epPacket->mediaType ) )
   {
      if ( ModeCheck( cnx->mode, RECVMODE) )
      {
         cnx->firstValidRTPPkt = 0;
         (*endptConfig.notify)( state->endptArchive, cnxId, EPEVT_MEDIA, NULL, 0, -1 );
      }
   }

   /*
   ** If in network loopback mode, reflect all packets
   */
   if( cnx->mode == EPCNXMODE_LOOP )
   {
      (*endptConfig.packet)( state->endptArchive, cnx->cnxId, epPacket, length );
   }

   /*
   ** Verify connection is in a mode that is supposed to receive packets
   */
   packetSent = VRG_FALSE;

   if ( state->bGR303Support )
   {
      /* RTP packet */
      if ( epPacket->mediaType == EPMEDIATYPE_RTP )
      {
         /* Determine the packet type */
         rtpType = RtpGetPacketType( &cnx->rtpstate, (RTPPACKET *)epPacket->packetp, length, state->bGR303Support );

         switch ( rtpType )
         {
            /* NTE packet requesting for ringing tones */
            case RTPPKT_RING_EVENT:
            {
               if ( !state->offHook )
               {
                  if( state->casCtlBits != EP_CAS_CTL_RINGING )
                  {
                     /* starts ringing */
                     casCtlRingOn( state->casCtlHandle );

                     state->casCtlBits = EP_CAS_CTL_RINGING;

                     (*endptConfig.notify)( state->endptArchive, -1, EPEVT_RINGING_ON, NULL, 0, -1 );
                  }

                  /* Packet successfully sent */
                  packetSent = VRG_TRUE;

               }
               else
               {
                  status = EPSTATUS_BUSY;
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "%s status %d", __FUNCTION__, status))
                  return( status );
               }
            }
            break;

            /* NTE packet to disable ringing and end OSI */
            case RTPPKT_RINGOFF_EVENT:
            {
               /* Always remove OSI */
               if ( state->casCtlBits == EP_CAS_CTL_LCFO )
               {
                  /*
                   * If On-hook then return to ALERTING
                   * (not IDLE as RINGOFF should always be preceded by RING).
                   * If Off-hook then return to ACTIVE.
                   */
                  casCtlSupervisoryDisconnectOff( state->casCtlHandle, VRG_TRUE, state->offHook );

                  state->casCtlBits = EP_CAS_CTL_LCF;

                  /* Packet successfully sent */
                  packetSent = VRG_TRUE;
               }
               /* Only remove Ring if On-hook */
               else if ( (state->casCtlBits == EP_CAS_CTL_RINGING) && !(state->offHook) )
               {
                  casCtlRingOff( state->casCtlHandle, VRG_TRUE );

                  state->casCtlBits = EP_CAS_CTL_LCF;

                  (*endptConfig.notify)( state->endptArchive, -1, EPEVT_RINGING_OFF, NULL, 0, -1 );

                  /* Packet successfully sent */
                  packetSent = VRG_TRUE;
               }
               else
               {
                  status = EPSTATUS_BUSY;
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "%s status %d", __FUNCTION__, status))
                  return( status );
               }
            }
            break;

            /* NTE packet requesting for OSI events */
            case RTPPKT_OSI_EVENT:
            {
               if ( state->casCtlBits != EP_CAS_CTL_LCFO )
               {
                  /* Start OSI */
                  casCtlSupervisoryDisconnectOn( state->casCtlHandle );

                  state->casCtlBits = EP_CAS_CTL_LCFO;
               }
               packetSent = VRG_TRUE;
            }
            break;

            case RTPPKT_DTMF_EVENT:
            case RTPPKT_MEDIA:
            {
               /* If the packet is smaller than the size of just an RTP header,
                * drop it since the DSP can't even start decoding a NULL
                * payload. */
               if(length <= RTP_PACKET_HDR_SIZE)
               {
                  break;
               }

               /*  No need to do ModeCheck() here.
                *  Send/Recv mode is set in SetRtpMode(); we should pass received pkts to RTP service.
                *  If in receiving mode, rtp packets are counted and processed by DSP;
                *  if in non-receiving mode, rtp packets are counted only.
                  */
               if ( HAPISUCCESS == hdspSendCmdDataAsync( cnx->vhdhdl,
                                       HAPINET_EGRESSRTPPKT,
                                       HSCMDEXTDATA_ASYNC,
                                       length,
                                       (RTPPACKET *) epPacket->packetp ))
               {
                  packetSent = VRG_TRUE;
               }
            }
            break;

            default:
            {
            }
         }
      }
      /* T.38 packet */
      if ( epPacket->mediaType == EPMEDIATYPE_T38 )
      {
          if ( IsT38Negotiated( cnx ) )
          {
              if ( HAPISUCCESS == hdspSendT38Packet( cnx->vhdhdl,
                                                     length,
                                                     epPacket->packetp ) )
              {
                 packetSent = VRG_TRUE;
              }
              else
              {
                 VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT cnx %d: ERROR - T38 packet received in non FAXR mode; dataMode=%d",
                             cnx->cnxId, cnx->dataMode));
              }
           }
      }
      /* RTCP packet */
      if ( epPacket->mediaType == EPMEDIATYPE_RTCP )
      {
         if (HAPISUCCESS == hdspSendCmdDataAsync( cnx->vhdhdl,
                            HAPINET_EGRESSRTCPPKT,
                            HSCMDEXTDATA_ASYNC,
                            length,
                            (RTPPACKET *) epPacket->packetp ))
         {
            packetSent = VRG_TRUE;
         }
         return ( EPSTATUS_SUCCESS );
      }


      if ( packetSent == VRG_FALSE )
      {
         status = EPSTATUS_BAD_MODE;
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "%s status %d", __FUNCTION__, status))
         return( status );
      }

   }
   else
   {
      /* GR303 Support is false */

      /* Only process packet if we are in the non-mute mode */
      if ( cnx->bMuteT38 == VRG_FALSE )
      {
         /* RTP packet */
         if ( epPacket->mediaType == EPMEDIATYPE_RTP )
         {
            /* Determine the packet type */
            rtpType = RtpGetPacketType( &cnx->rtpstate, (RTPPACKET *)epPacket->packetp, length, state->bGR303Support );

            switch (rtpType)
            {
               /* NTE packet requesting for ringing tones */
               case RTPPKT_RING_EVENT:
               {
                  if ( ModeCheck( cnx->mode, RECVMODE ))
                  {
                     if ( !state->offHook )
                     {
                        if( state->casCtlBits != EP_CAS_CTL_RINGING )
                        {
                           /* starts ringing */
                           casCtlRingOn( state->casCtlHandle );

                           state->casCtlBits = EP_CAS_CTL_RINGING;
                        }
                        packetSent = VRG_TRUE;

                     }
                     else
                     {
                        status = EPSTATUS_BUSY;
                        VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "%s status %d", __FUNCTION__, status))
                        return( status );
                     }
                  }
               }
               break;

               /* NTE packet requesting for OSI events */
               case RTPPKT_OSI_EVENT:
               {
                  if ( ModeCheck( cnx->mode, RECVMODE ) )
                  {
                     if ( !state->offHook )
                     {
                        if ( state->casCtlBits != EP_CAS_CTL_LCFO )
                        {
                           /* activate VMWI */
                           casCtlSupervisoryDisconnectOn( state->casCtlHandle );

                           state->casCtlBits = EP_CAS_CTL_LCFO;
                        }
                        packetSent = VRG_TRUE;

                     }
                     else
                     {
                        status = EPSTATUS_BUSY;
                        VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "%s status %d", __FUNCTION__, status))
                        return( status );
                     }
                  }
               }
               break;

               case RTPPKT_DTMF_EVENT:
               case RTPPKT_MEDIA:
               {
                  if ( ModeCheck( cnx->mode, RECVMODE ))
                  {
                     if ( state->casCtlBits == EP_CAS_CTL_RINGING )
                     {
                        /* stops ringing */
                        casCtlRingOff( state->casCtlHandle, VRG_TRUE );

                        state->casCtlBits = EP_CAS_CTL_LCF;
                     }

                     if ( state->casCtlBits == EP_CAS_CTL_LCFO )
                     {
                        /* End OSI */
                        casCtlSupervisoryDisconnectOff( state->casCtlHandle, VRG_FALSE, state->offHook );

                        state->casCtlBits = EP_CAS_CTL_LCF;
                     }
                  }
                  /*
                  ** If LCS is in use packets have to be sent down to the DSP
                  ** even if it is on hook. This is because Caller ID and VMWI
                  ** for LCS are comming in in-band, as voice packets, and we
                  ** need to pass them on to the DSP even when on hook.
                  */
                  if (!(cnx->namedPhoneEvts & CODEC_NTE_LCS))
                  {
                     /* Do not send packets down to DSP until Tone Generation or CLIDTX is over */
                     if ( classStmIsIdle( &state->classState ) == VRG_FALSE )
                     {
                        status = EPSTATUS_BUSY;
                        VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "%s status %d", __FUNCTION__, status))
                        return( status );
                     }
                  }

                  /* If the packet is smaller than the size of just an RTP header,
                   * drop it since the DSP can't even start decoding a NULL
                   * payload. */
                  if(length <= RTP_PACKET_HDR_SIZE)
                  {
                     break;
                  }

                  /*  No need to do ModeCheck() here.
                   *  Send/Recv mode is set in SetRtpMode(); we should pass received pkts to RTP service.
                   *  If in receiving mode, rtp packets are counted and processed by DSP;
                   *  if in non-receiving mode, rtp packets are counted only.
                   */
                  if ( HAPISUCCESS == hdspSendCmdDataAsync( cnx->vhdhdl,
                            HAPINET_EGRESSRTPPKT,
                            HSCMDEXTDATA_ASYNC,
                            length,
                            (RTPPACKET *) epPacket->packetp ))
                  {
                     packetSent = VRG_TRUE;
                  }
               }
               break;

               default:
               {
               }

            }
         }

         /* T.38 packet */
         if ( epPacket->mediaType == EPMEDIATYPE_T38 )
         {
            if ( ModeCheck( cnx->mode, RECVMODE ))
            {
               /* Ignore T.38 packets when we are supposed to mute audio and discard
               ** all rogue egress T.38 packets */
               if ( IsT38Negotiated( cnx ) )
               {
                  if ( HAPISUCCESS == hdspSendT38Packet( cnx->vhdhdl,
                                                         length,
                                                         epPacket->packetp ))
                  {
                     packetSent = VRG_TRUE;
                  }
               }
               else
               {
                  VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT cnx %d: ERROR - T38 packet received in non FAXR mode; dataMode=%d",
                         cnx->cnxId, cnx->dataMode));
               }
            }
         }
      }

      /* RTCP packet */
      if ( epPacket->mediaType == EPMEDIATYPE_RTCP )
      {
         if (HAPISUCCESS == hdspSendCmdDataAsync( cnx->vhdhdl,
                            HAPINET_EGRESSRTCPPKT,
                            HSCMDEXTDATA_ASYNC,
                            length,
                            (RTPPACKET *) epPacket->packetp ))
         {
            packetSent = VRG_TRUE;
         }
         return ( EPSTATUS_SUCCESS );
      }

      if ( packetSent == VRG_FALSE )
      {
         status = EPSTATUS_BAD_MODE;
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "%s status %d", __FUNCTION__, status))
         return( status );
      }

   }


   /*
   ** Free the buffer back to the caller
   */
   if ( endptConfig.packetRelease != NULL  )
   {
      (*endptConfig.packetRelease)( state->endptArchive, cnxId, bufDesc );
   }

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptConsoleCmd
**
** PURPOSE:    Send command to dispatcher to process endpt console command.
**
** PARAMETERS: endptState        - a pointer to endpoint state structure
**                                 (lineId needs to be filled in)
**
**             cmd               - endpoint console command type
**             consoleCmdParams  - additional parameters
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptConsoleCmd
(
   ENDPT_STATE   *endptState,
   EPCONSOLECMD   cmd,
   EPCMD_PARMS   *consoleCmdParams
)
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_CONSOLE_CMD;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)&cmd;
   vrgCmd.arguments[2] = (unsigned int)consoleCmdParams;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

#if 0
/*
*****************************************************************************
** FUNCTION:   vrgCodecTableInit
**
** PURPOSE:    Initialize the codec table to reflect simultaneous support
**             of different codecs.
**
** PARAMETERS: none
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgCodecTableInit( void )
{
   int                        i, j;
   VRG_UINT16                 maxCnx;
   VRG_UINT16                 maxFullCnx;
   VRG_UINT16                 maxLiteCnx;
   VRG_UINT32                 codecCap;
   VRG_UINT32                 mask;
   VRG_CODEC_TABLE_ROW       *pRowEntry;

   maxCnx = gDspCfg.maxFullCnxs + gDspCfg.maxLiteCnxs;
   codecCap = gDspCfg.codecCapability;
   maxFullCnx = gDspCfg.maxFullCnxs;
   maxLiteCnx = gDspCfg.maxLiteCnxs;

   /* Sanity check - ensure the DSP supports at least some type of codec cnx */
   if ( maxCnx == 0 )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "DSP does not have any cnx support" ));
      return ( EPSTATUS_RESOURCES );
   }

   /* Initialize the codec table */
   memset( &vrgCodecTable, 0, sizeof( VRG_CODEC_TABLE_ROW ) * VRG_CODEC_TABLE_MAX_ROWS );

   /*
   ** 1. add default codec capabilities - by default we will always have
   **    support of PCMU/PCMA
   */

   /* Loop through all possible PCMU/PCMA combo */
   for ( i = 1; i <= maxCnx; i++ )
   {
      /* Create new table row */
      pRowEntry = vrgCodecTableAddNewRow( CODEC_PCMU, i );

      /* Add new entries into current row if there are remaining cnxs */
      if ( maxCnx - i )
      {
         vrgCodecTableAddExistingRow( pRowEntry, CODEC_PCMA, maxCnx - i );
      }
   }

   /* The above loop does not account for the case when only PCMA
   ** is the only codec in the row.  We will manually add it here */
   vrgCodecTableAddNewRow( CODEC_PCMA, maxCnx );


   /*
   ** 2. Add enhanced vocoder capabilities if heavy vhds are supported.
   */
   if ( maxFullCnx > 0 )
   {
      /* Remove PCMU and PCMA from the capability list since they are not
      ** enhanced vocoders.  In addition, remove any non-voice codecs and
      ** variable bit rate placeholder */
      codecCap &= ~( CODEC_PCMU | CODEC_PCMA | CODEC_NON_VOICE | CODEC_G723x_VAR );

      /* In the codec list, iLBC 20ms and 30ms are treated as two
      ** separate codecs.  We'll only include 1 flavour of the iLBC codec (20ms)
      ** in the codec list since the mib does not distinguish between the
      ** 2 flavours */
      codecCap &= ( ~CODEC_ILBC_30 );

      /* Loop through all enhanced vocoder */
      mask = codecCap;
      while ( mask )
      {
         /* Isolate the least significant bit in the codec capabilities */
         mask = (VRG_UINT32)( -codecCap );  /* Two's complement of codecs */
         mask &= (VRG_UINT32)( codecCap );  /* AND with codecs            */

         if ( mask )
         {
            /* Loop through all PCMU/PCMA combo with the enhanced vocoder */
            for ( i = 1; i <= maxFullCnx; i++ )
            {
               maxLiteCnx = maxCnx - i;
               if ( maxLiteCnx )
               {
                  /* Add PCMA + enhanced vocoder combo first */
                  pRowEntry = vrgCodecTableAddNewRow( mask, i );
                  vrgCodecTableAddExistingRow( pRowEntry, CODEC_PCMA, maxLiteCnx );

                  /* Add remaining PCMU/PCMA/enhanced vocoder combo */
                  for ( j = 1; j <= maxLiteCnx; j++)
                  {
                     pRowEntry = vrgCodecTableAddNewRow( mask, i );
                     vrgCodecTableAddExistingRow( pRowEntry, CODEC_PCMU, j );

                     if ( maxLiteCnx - j )
                     {
                        vrgCodecTableAddExistingRow( pRowEntry, CODEC_PCMA, maxLiteCnx - j );
                     }
                  }
               }
            }

            codecCap &= ( ~mask );
         }
      }
   }

   return ( EPSTATUS_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   vrgCodecTableAddNewRow
**
** PURPOSE:    Add a new row into the codec table
**
** PARAMETERS: codec                - new codec to add
**             maxSimultaneousCodec - maximum number of simultaneous operation
**                                    supported by the specified codec
**
** RETURNS:    pointer to the created row
**
** NOTE:
*****************************************************************************
*/
static VRG_CODEC_TABLE_ROW* vrgCodecTableAddNewRow( CODEC_TYPE codec,
                                                    int maxSimultaneousCodec )
{
   VRG_CODEC_TABLE_ROW   *pCodecTableRow;

   /* Points to the global codec table */
   pCodecTableRow = vrgCodecTable;

   /* Find a free entry in the codec table */
   while ( pCodecTableRow && ( pCodecTableRow->codecTableNumEntry != 0 ) )
   {
      pCodecTableRow++;
   }

   if ( !( pCodecTableRow ) )
   {
      /* No more available entries */
      XCHG_ASSERT( 0 );
   }

   /* Add entry into the new row */
   pCodecTableRow->codecTableEntry[0].codec = codec;
   pCodecTableRow->codecTableEntry[0].maxCodec = maxSimultaneousCodec;
   pCodecTableRow->codecTableNumEntry = 1;

   /* Return pointer to the new row of the codec table */
   return ( pCodecTableRow );
}


/*
*****************************************************************************
** FUNCTION:   vrgCodecTableAddExistingRow
**
** PURPOSE:    Add a new entry into the existing row of the codec table
**
** PARAMETERS: pCodecTableRow       - pointer to the row of the codec table
**                                    to udpate
**             codec                - new codec to add
**             maxSimultaneousCodec - maximum number of simultaneous operation
**                                    supported by the specified codec
**
** RETURNS:    none
**
** NOTE:
*****************************************************************************
*/
static void vrgCodecTableAddExistingRow( VRG_CODEC_TABLE_ROW *pCodecTableRow,
                                         CODEC_TYPE codec, int maxSimultaneousCodec )
{
   int      index;

   /* Add to exisiting row */
   XCHG_ASSERT( pCodecTableRow != NULL );
   XCHG_ASSERT( pCodecTableRow->codecTableNumEntry != 0 );

   /* New index to use */
   index = pCodecTableRow->codecTableNumEntry;

   /* Ensure there is room */
   XCHG_ASSERT( index + 1 <= VRG_CODEC_TABLE_MAX_ROW_ENTRY );

   /* Add new entry */
   pCodecTableRow->codecTableEntry[index].codec = codec;
   pCodecTableRow->codecTableEntry[index].maxCodec = maxSimultaneousCodec;
   pCodecTableRow->codecTableNumEntry++;
}


/*
*****************************************************************************
** FUNCTION:   vrgCodecTableRetrieveRow
**
** PURPOSE:    Return a single row from the codec table
**
** PARAMETERS: index        - row index to return (0-indexed)
**             pNumOfCodecs - pointer to the location for storing the
**                            retrieved row
**
** RETURNS:    none
**
** NOTE:
*****************************************************************************
*/
void vrgCodecTableRetrieveRow( int index, VRG_CODEC_TABLE_ROW *pNumOfCodecs )
{
   /* Ensure this is a valid row */
   XCHG_ASSERT( index < VRG_CODEC_TABLE_MAX_ROWS );

   /* Copy the entire row */
   memcpy( pNumOfCodecs, &(vrgCodecTable[index]), sizeof( VRG_CODEC_TABLE_ROW ) );
}

#endif

/*
*****************************************************************************
** FUNCTION:   vrgEndptPowerSourceChanged
**
** PURPOSE:    Event handler for modified power source
**
** PARAMETERS: newPowerSource - 0: operating on AC
**                              1: operating on battery power
**
** RETURNS:    nothing
**
** NOTE:
*****************************************************************************
*/
static void vrgEndptPowerSourceChanged( int newPowerSource )
{
   XDRV_SLIC         *slicDriver;
   int                i;
   HBSTATE           hbState;
   VRG_BOOL          bOnBattery;

   if ( newPowerSource == 1 )
   {
       hbState = HBSTATE_INACTIVE;
       bOnBattery = VRG_TRUE;
   }
   else
   {
       hbState = HBSTATE_ACTIVE;
       bOnBattery = VRG_FALSE;
   }

   /* Update the Heartbeat patient state */
   hbUpdatePatientState( endptConfig.acPowerHBId, hbState );

   /* Tell the SLIC driver about the operating power source */
   for (i=0; i<vrgEndptGetNumEndpoints(); i++)
   {
      /* Get the SLIC driver for this line */
      slicDriver = boardHalSlicGetDriver(i);

      if (slicDriver != NULL)
      {
         /* Set the power source for this line */
         casCtlSendCommand(CAS_REQ_SLIC_BOOTED_LOOP_CURRENT, CAS_CMD_MODE_ASYNC, (VRG_UINT32)slicDriver, bOnBattery);
      }
   }
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptIsEnabled
**
** PURPOSE:    To Determine if endpoint is enabled/disabled
**
** PARAMETERS: line           - endpoint number    (starting from 0)
**
**
** RETURNS:    1 if enabled
**             0 if disabled
**
** NOTE:  This function is used to pass information on the endpt state
**        based on information that is not available at a higher level (ie. SNMP).
**        For example, a SLIC for a particular endpoint is disabled so, that
**        endpoint cannot be used to make voice calls.
*****************************************************************************
*/
static void vrgEndptIsEnabled( int line, int *enabled)
{
   int rc;

   /*For now, we just check if a SLIC has been disabled, due to a switch to
     battery power.  In the future, other such conditions could be communicated
     through this function.*/

   if (boardHalSlicIsDisabled( line ))
   {
      /*SLIC is disabled so line is essentially disabled.*/
      rc = 0;
   }
   else
   {
      /*SLIC is enabled*/
      rc = 1;
   }

   *enabled = rc;
}

static EPSTATUS vrgEndptSendCasEvt( int line, CAS_CTL_EVENT_TYPE casEvtType, CAS_CTL_EVENT casCtlEvt)
{

   VRG_EVT vrgEvt;

   memset( &vrgEvt, 0, sizeof( vrgEvt ));

   vrgEvt.type                = VRG_EVT_TYPE_CAS;
   vrgEvt.evt.cas.handle      = vrgEndpt[ line ].casCtlHandle;
   vrgEvt.evt.cas.eventType   = casEvtType;
   vrgEvt.evt.cas.eventId     = casCtlEvt;

   /* Post this event for the VRG_EVT task to process */
   if( vrgEventQ != -1 )
   {
      if( VrgQueuePost( vrgEventQ, (void*)&vrgEvt, sizeof( vrgEvt )) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!\n", __FUNCTION__ ));
      }
   }

   return ( EPSTATUS_SUCCESS );
}

/*****************************************************************************
*  FUNCTION:   vrgEndptMuteConnection
*
*  PURPOSE:    Send command to the dispatcher to mute a connection.
*
*  PARAMETERS: endptState - pointer to endpoint state structure
*              cnxId      - connection identifier
*              mute       - mute status of the connection
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptMuteConnection
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   VRG_BOOL       mute
)
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_MUTE_CNX;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)cnxId;
   vrgCmd.arguments[2] = (unsigned int)mute;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

/*****************************************************************************
*  FUNCTION:   vrgEndptLinkInternal
*
*  PURPOSE:    Send command to the dispatcher to link or unlink two endpts.
*
*  PARAMETERS: endptState      - pointer to endpoint state structure
*              otherEndptState - pointer to other endpoint state struct
*              link            - whether or not to link or unlink
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptLinkInternal
(
   ENDPT_STATE   *endptState,
   ENDPT_STATE   *otherEndptState,
   VRG_BOOL       link
)
{
   VRG_EPT_CMD vrgCmd;
   VRG_EPT_CMD *vrgCmdp = &vrgCmd;

   /* Construct the command */
   vrgCmd.command      = VRG_EPT_LINK_INT;
   vrgCmd.arguments[0] = (unsigned int)endptState;
   vrgCmd.arguments[1] = (unsigned int)otherEndptState;
   vrgCmd.arguments[2] = (unsigned int)link;

   /* Queue the command */
   if( vrgEptCmdQ != -1 )
   {
      if ( VrgQueuePost( vrgEptCmdQ, &vrgCmdp, sizeof(VRG_EPT_CMD*) ) != BUF_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: VrgQueuePost failed!\n", __FUNCTION__ ));
         return EPSTATUS_ERROR;
      }
   }

   /* Wait for response from VRG dispatcher */
   if ( bosSemTakeBlock( &vrgEptApiSem ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: bosSemTakeBlock failed!\n", __FUNCTION__ ));
      return EPSTATUS_ERROR;
   }

   return vrgCmd.status;
}

/*****************************************************************************
*  FUNCTION:   vrgEndptGetDebugDriver
*
*  PURPOSE:    Get the boardhal-specific debug driver
*
*  PARAMETERS: parms - pointer to endpoint command parameters
*
*  RETURNS:    pointer to debug driver
*
*  NOTE:
*****************************************************************************/
XDRV_DEBUG* vrgEndptGetDebugDriver( void )
{
   XDRV_DEBUG *pDebugDriver = NULL;

   pDebugDriver = boardHalDspGetDebugDriver();

   return ( pDebugDriver );
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptGetEndptOperStatus
**
** PURPOSE:    Get endpt operation status.  An endpt becomes un-operational
**             if any of the vhds associated with the endpt is pending a reset
**
** PARAMETERS: lineId - line Id
**
** RETURNS:    EPSTATUS_RESET_PENDING if the endpoint is non-operational.
**             In this case the application should initiate restart of the
**             endpoint through vrgEndptDeinit.
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptGetEndptOperStatus( int lineId )
{
   XDRV_DEBUG    *pDebugDriver;
   VRG_ENDPT     *state;
   int            i;

   /* Check if DSP counters are moving at all */
   pDebugDriver = vrgEndptGetDebugDriver();
   if ( pDebugDriver )
   {
      if ( xdrvDebugDspReadCnt( pDebugDriver ) != 0 )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "No response from DSP or DSP counters not moving !!!" ));
         return ( EPSTATUS_RESET_PENDING );
      }
   }

   /* Loop through all the active vhds on this endpt */
   state = GetEndptState( lineId );
   if( !state )
   {
      return( EPSTATUS_ENDPT_UNKNOWN );
   }

   /* First, check if the line vhd is operational */
   if ( hdspGetOperationalStatus( state->lineVhdHdl ) == HDSPSTATUS_PENDING_RESET )
   {
      return ( EPSTATUS_RESET_PENDING );
   }

   /* Next, check the gateway vhd */
   if ( state->cnx[PRIMARY_CNX_INDEX] )
   {
      if ( hdspGetOperationalStatus( state->cnx[PRIMARY_CNX_INDEX]->vhdhdl ) == HDSPSTATUS_PENDING_RESET )
      {
         return ( EPSTATUS_RESET_PENDING );
      }

      /* Finally, check the conference channels */
      for ( i = SECONDARY_CNX_START_INDEX; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
      {
         if ( state->cnx[i] != NULL )
         {
            if ( hdspGetOperationalStatus( state->cnx[i]->vhdhdl ) == HDSPSTATUS_PENDING_RESET )
            {
               return ( EPSTATUS_RESET_PENDING );
            }
         }
      }
   }

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptResetBuildOut
**
** PURPOSE:    Reset build out if there is only 1 cnx assiocated with
**             the current endpt.
**
** PARAMETERS: lineId - current endpt
**             vhdhdl - line vhd handle
**
** RETURNS:    nothing
**
** NOTE:
*****************************************************************************
*/
void vrgEndptResetBuildOut( int lineId, int vhdhdl )
{
   VRG_ENDPT  *state;
   int         i;
   VRG_SINT16  status;

   state = GetEndptState( lineId );

   /*
   ** Check if there are more than 1 active cnx assiocated with the
   ** current endpt.
   */
   for( i = 0; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
   {
      if ( ( state->cnx[i] ) && ( state->cnx[i]->cnxId != CNX_UNUSED ) &&
           ( state->cnx[i]->vhdhdl != vhdhdl ) )
      {
         break;
      }
   }

   /* Only 1 cnx assiocated with the endpt - reset build out */
   if ( i == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT )
   {
      status = hdspSendCmdData( HAPIRESMAN,
                                HAPI_SWB_CALL_END_CMD,
                                HSCMDDATA,
                                vhdhdl,
                                0 );

      if ( status != HAPISUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR reset build out failed for vhdhdl 0x%x", vhdhdl ));
      }
   }
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptRetrievePlcSettings
**
** PURPOSE:    Determine PLC settings for the codec in question
**
** PARAMETERS: codec - codec type to look up PLC settings for
**
** RETURNS:    PLC type to use
**
** NOTE:
*****************************************************************************
*/
VRG_UINT16 vrgEndptRetrievePlcSettings( CODEC_TYPE codec )
{
   switch ( codec )
   {
      case ( CODEC_PCMU ):
      case ( CODEC_PCMA ):
      {
         /* Since BVC offers better loss concealment than GPLC, we'll use
         ** BVC if the DSP image supports it */
         if ( gDspCfg.bvcSupport )
         {
            return ( PLC_BVC );
         }
      }
      break;

      default:
      {
      }
      break;
   }

   if ( gDspCfg.gplcSupport )
   {
      return ( PLC_FRAMEREPEAT );
   }

   return ( PLC_OFF );
}

/*
*****************************************************************************
** FUNCTION:   RtpGetPacketType
**
** PURPOSE:    Categorize the RTP packet (media, dtmf, ringing, lcs...etc)
**
** PARAMETERS: state     - pointer to endpoint's RTP state struct
**             rtpPacket - ptr to the RTP packet received
**             length    - length of RTP packet
**             bGR303Support - Indicates whether GR303 (RFC 2833 for trunking) is enabled
**
** RETURNS:    RTP packet type
**
*****************************************************************************
*/
static RTPPKT_TYPE RtpGetPacketType(RTPSTATE* state, RTPPACKET* rtpPacket, int length, VRG_BOOL bGR303Support)
{
   RTP_PAYLOAD_TYPE    payloadType;
   RTP_PAYLOAD_TYPE    NTEpayloadType;
   NTE_PAYLOAD*        ntep;

   /* parse packet for info needed */
   RtpParsePacket( state, rtpPacket, length );

   payloadType = rtpPacket->pt;

   if (bGR303Support)
   {
      /* Act based upon the received RTP payload type */
      NTEpayloadType = state->recvNtePayloadType;

      if ( NTEpayloadType == payloadType )
      {
         ntep = state->pPayload;

         if ( ntep->event == RTP_NTE_RING )
         {
            return ( RTPPKT_RING_EVENT );
         }
         else if ( ntep->event == RTP_NTE_RINGOFF )
         {
            return ( RTPPKT_RINGOFF_EVENT );
         }
         else if (ntep->event == RTP_NTE_OSI)
         {
            return ( RTPPKT_OSI_EVENT );
         }
         else
         {
            return ( RTPPKT_UNSUPPORTED );
         }
      }
      else
      {
         return ( RTPPKT_MEDIA );
      }
   }
   else
   {
      /* Act based upon the received RTP payload type */
      NTEpayloadType = RtpGetNtePayloadType( state, 0 );

      if ( NTEpayloadType == payloadType )
      {
         ntep = state->pPayload;

         if (ntep->event == RTP_NTE_RING)
         {
            return ( RTPPKT_RING_EVENT );
         }
         else if (ntep->event == RTP_NTE_OSI)
         {
            return ( RTPPKT_OSI_EVENT );
         }
         else if ( ntep->event <= RTP_NTE_DTMFD )
         {
            return ( RTPPKT_DTMF_EVENT );
         }
         else
         {
            return ( RTPPKT_UNSUPPORTED );
         }
      }
      else
      {
         return ( RTPPKT_MEDIA );
      }
   }
}

/*
*****************************************************************************
** FUNCTION:   RtpGetNtePayloadType
**
** PURPOSE:    Retrieve a RTP payload type for NTE codec
**
** PARAMETERS: state       - pointer to endpoint's RTP state struct
**             sendFlag    - indicates whether this is an ingress or egress
**                           codec
**
** RETURNS:    codec rate
*****************************************************************************
*/
static VRG_UINT8 RtpGetNtePayloadType( RTPSTATE* state, VRG_UINT8 sendFlag )
{
   if ( sendFlag )
   {
      return ( state->sendNtePayloadType );
   }
   else
   {
      return ( state->recvNtePayloadType );
   }
}

/*
*****************************************************************************
** FUNCTION:   RtpParsePacket
**
** PURPOSE:    parse RTP data packet and store packet information to ease processing
**
** PARAMETERS: state     - pointer to endpoint's RTP state struct
**             rtpPacket - pointer to the RTP packet
**             length    - length of RTP packet (ie. header + payload) size in bytes
**
** RETURNS:    nothing
**
** NOTE:       The control structure indexed by the handle is updated
*****************************************************************************
*/
static void RtpParsePacket( RTPSTATE* state, RTPPACKET* rtpPacket, int length)
{
   char* buf = (char*) rtpPacket;

   /* No padding, CSRC and extension byte initially */
   memset(&state->stats, 0, sizeof(RTPSTATS));

   /* find padding byte */
   if ( rtpPacket->p != 0) /* p == 1 */
   {
      /* last octet of the payload contains the padding size */
      state->stats.bPaddingByte = buf[length-1];
   }

   /* find CSRC source */
   if (rtpPacket->cc != 0) /* cc == 1 to 16 */
   {
      state->stats.wCSRCByte = (VRG_UINT16)rtpPacket->cc;
      state->stats.wCSRCByte <<= 2;   /* cc * 4; */
   }
   else
   {
      state->stats.wCSRCByte = 0;
   }

   /* find extension byte */
   if (rtpPacket->x != 0) /* x == 1 */
   {
      VRG_UINT16 extlen;

      /* Extension length */
      extlen = *(VRG_UINT16*)&buf[sizeof(RTPPACKET) + state->stats.wCSRCByte + 2];

      /*
      ** number of 16 bit byte = (number of 32 bit word + 1) x 2
      ** (+1 comes from the extension descriptor.  Please see section 5.3.1 RFC1889)
      */
      state->stats.wExtensionByte = (VRG_UINT16) ((bosSocketNtoHS(extlen) + 1) << 1);
   }

   state->pPayload = (char*)rtpPacket + sizeof(RTPPACKET) + state->stats.wCSRCByte + state->stats.wExtensionByte;   /* neglect CSRC  */

}

/*
*****************************************************************************
** FUNCTION:   vrgEndptDoDeinit
**
** PURPOSE:    VRG endpoint module shutdown - call once during system shutdown.
**             This will shutdown all endpoints and free all resources used by
**             the VRG endpt. (i.e. this function should free all resources
**             allocated in vrgEndptInit() and vrgEndptCreate()).
**
** PARAMETERS: data (in) - unused
**
** RETURNS:    None.
**
** NOTE:       It is assumed that this function is only called after all endpoint
**             tasks have been notified of a pending application reset, and each
**             one has acknowledged the notification. This implies that each endpoint
**             task is currently blocked, waiting to be resumed so that they may
**             complete the shutdown procedure.
**
**             It is also assumed that no task is currently blocked on any OS
**             resource that was created in the module initialization functions.
**
*****************************************************************************
*/
void vrgEndptDoDeinit( struct work_struct* data )
{
   int               i;
   VRG_ENDPT    *state;

   (void) data;

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "Deinitializing endpoint ..." ));
   for (i = 0; i < vrgEndptGetNumEndpoints(); i++)
   {
      state = &vrgEndpt[i];

      state->isLineUsed = VRG_FALSE;

      /* Shutdown CLASS state machines */
      classStmDeinit( &state->classState );
   }

   if ( endptConfig.initState & EPEINIT_CNXSTATE )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Deinitializing line VHDs..." ));
      /* Close each endpoint vhd */
      for ( i = 0; i < vrgEndptGetNumEndpoints(); i++ )
      {
         /* Get the endpoint state */
         state = &vrgEndpt[i];

         if ( state->lineVhdHdl != 0xffff )
         {
            /* Close ECAN line VHD */
            if ( hdspVhdClose( state->lineVhdHdl, state->dspNum ) != HAPISUCCESS )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "hdspVhdClose failed for dsp %d, endpt %d, line-vhd 0x%x",
                         state->dspNum, state->endptNum, state->lineVhdHdl ));
            }
         }
         if ( state->endptType == EPTYPE_PSTN && state->pstnVhdHdl )
         {
            /* Close PSTN line VHD */
            if ( hdspVhdClose( state->pstnVhdHdl, state->dspNum ) != HAPISUCCESS )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "hdspVhdClose failed for dsp %d, endpt %d, pstn-vhd 0x%x",
                         state->dspNum, state->endptNum, state->pstnVhdHdl ));
            }
         }
#if VRG_ENDPT_CFG_USE_HFE
         /* Remove Timer from List */
         bosTimerDestroy( &state->hookflashTimer );
#endif
      }
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "COMPLETED deinitializing line VHDs." ));
   }

   /* At least one line VHD was created (since EPEINIT_CNXVHD was set ) */
   if ( endptConfig.initState & EPEINIT_CNXVHD )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Deinitializing connection VHDs..." ));
      /* Close each cnx vhd */
      for ( i = 0; i < VRG_ENDPT_CFG_NUM_CNX; i++ )
      {
         if ( vrgCnxState[i].vhdhdl )
         {
            /* close cnx VHD */
            if ( hdspVhdClose( vrgCnxState[i].vhdhdl, vrgCnxState[i].dspNum ) != HAPISUCCESS )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "hdspVhdClose failed for dsp %d, cnx vhd 0x%x",
                         vrgCnxState[i].dspNum, vrgCnxState[i].vhdhdl ));
            }
         }
      }
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "COMPLETED deinitializing connection VHDs." ));
   }
   endptConfig.initState &= ~EPEINIT_CNXVHD;

#ifdef BRCM_NONDIST_BUILD
   /* Disable PCM or APM interrupts */
   boardHalIsrDisable();
#endif /* BRCM_NONDIST_BUILD */

   /* Reset all tasks created by BOS */
   bosAppReset();

   /* Shutdown the HHAPI module */
   if ( endptConfig.initState & EPEINIT_HDSP )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Deinitializing HDSP..." ));
      if ( hdspDeinit() != HAPISUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "hdspShutDown failed" ));
      }
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "COMPLETED deinitializing HDSP." ));
   }
   endptConfig.initState &= ~EPEINIT_HDSP;

#if VRG_ENDPT_CFG_USE_TPD
   /* Shutdown the TPD module */
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Deinitializing TPD..." ));
   if ( endptConfig.initState & EPEINIT_TPD )
   {
#ifndef VRG_ENDPT_CFG_USE_MLT
      if ( tpdCtlDeinit() != TPD_CTL_SUCCESS )
#else
      if ( mltDeinit() != MLT_SUCCESS )
#endif
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "tpdCtlDeinit FAILED" ));
      }
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "COMPLETED deinitializing TPD." ));
#endif
   endptConfig.initState &= ~EPEINIT_TPD;


   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Deinitializing CAS..." ));
   /* Shutdown the CAS module */
   if ( endptConfig.initState & EPEINIT_CAS )
   {
      if ( casCtlDeinit() != CAS_CTL_SUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "casCtlDeinit FAILED" ));
      }
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "COMPLETED deinitializing CAS." ));
   endptConfig.initState &= ~EPEINIT_CAS;

   if ( vrgEndptGetNumFxoEndpoints() > 0 )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Deinitializing PSTN..." ));
      /* Shutdown the PSTN control module */
      if ( pstnCtlDeinit() != PSTN_CTL_SUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "FATAL ERROR: pstnCtlDeinit FAILED" ));
      }
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "COMPLETED deinitializing PSTN." ));
   }


#if BRCM_TRACK_IDLE_STATUS
   /* Gracefully shutdown idle watchdog before shuting down dispatcher. 
         Because dispatcher will destroy some semaphores, on which idle watchdog might sleep.
      */
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Deinitializing ISTW..." ));
   idleStatusWatchDogDeinit();
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "COMPLETED deinitializing ISTW." ));
#endif /* BRCM_TRACK_IDLE_STATUS */


   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Deinitializing VRG dispatcher..." ));
   /* Shutdown the VRG Event Processing module */
   if ( endptConfig.initState & EPEINIT_VRGDISP )
   {
      if ( VrgDispatcherProcessDeinit() != HAPISUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "VrgDispatcherProcessDeinit FAILED" ));
      }
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "COMPLETED Deinitializing event processing." ));
   endptConfig.initState &= ~EPEINIT_VRGDISP;

   vrgDestroyEndptSemaphores();

   /*
   ** Low-level HAPI module de-init.
   */

   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Deinitializing LHAPI..." ));
   if ( endptConfig.initState & EPEINIT_LHAPI )
   {
      lhapiDeinit();
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "COMPLETED deinitializing LHAPI." ));
   endptConfig.initState &= ~EPEINIT_LHAPI;

   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Deinitializing HEARTBEAT..." ));
   if ( endptConfig.initState & EPEINIT_HEARTBEAT )
   {
      /* Deinitialize heartbeat monitor module */
      hbDeInit();
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "COMPLETED deinitializing HEARTBEAT..." ));
   endptConfig.initState &= ~EPEINIT_HEARTBEAT;

   /* Deinitialize circular buffer driver */
   if ( memCircBufDeinit() != BUF_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error deinitializing memCircBuf driver"));
   }

   /* We reached the end of vrgEndptDeinit - reset the init state */
   endptConfig.initState = 0;

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED deinitializing endpoint !" ));

   /* Call the endpointdrv deinit function to finish cleaning up */
   finishVrgEndptDeinit();
}
