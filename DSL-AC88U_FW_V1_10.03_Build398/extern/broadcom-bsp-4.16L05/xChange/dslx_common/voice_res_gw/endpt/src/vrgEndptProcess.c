/****************************************************************************
*
*  Copyright (c) 2000-2014 Broadcom Corporation
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
*    Filename: vrgEndptProcess.c
*
****************************************************************************
*    Description:
*
*      This file contains all the endpoint command and event processing
*      logic.
*
****************************************************************************/


/*
*****************************************************************************
** INCLUDE FILES
*****************************************************************************
*/
volatile int gCSCnxId = -1;

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

#include <boardHalInit.h>
#include <boardHalApm.h>
#include <boardHalCas.h>
#include <boardHalDsp.h>
#include <boardHalDaa.h>
#include <boardHalSlic.h>
#include <boardHalPower.h>
#include <boardHalDect.h>
#include <boardHalProv.h>
#if defined( NTR_SUPPORT )
#include <boardHalNtr.h>
#endif
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

#ifndef BRCM_NONDIST_BUILD
#include <xdrvItpc.h>
#include <itpcSharedCmds.h>
#endif /* BRCM_NONDIST_BUILD */

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

#include <boardparms_voice.h>

/* These are needed for the randomization functions  */
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>

#include <asm/uaccess.h>

#include "vrgEndptPrivate.h"


/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/

/* Define the VAD shift if this constant is not defined in the DSP header files */
#ifndef VPC_VADSHIFT
   #define  VPC_VADSHIFT    8
#endif

/* Macro for identifying SAS + CAS signals */
#define EPSIG_IS_SAS_CAS( signal ) ( ( signal == EPSIG_SAS_CAS ) || \
                                     ( signal == EPSIG_SAS_CAS1 ) || \
                                     ( signal == EPSIG_SAS_CAS2 ) || \
                                     ( signal == EPSIG_SAS_CAS3 ) || \
                                     ( signal == EPSIG_SAS_CAS4 ) )

/* MIB value for CNG detection */
#define MIB_CNG_DET_ENABLED      1
#define MIB_CNG_DET_DISABLED     2

/* MIB value for Tone Detection */
#define MIB_TONE_DET_ALL_ENABLED            1
#define MIB_TONE_DET_ALL_DISABLED           2
#define MIB_TONE_DET_V18ALARMPOS_DISABLED   3
#define MIB_TONE_DET_ALARMPOS_DISABLED      4

/* Mask to identify codecs that do not run at 10msec */
#define IS_NON_10MS_CODEC(codec)  (( codec == CODEC_ILBC_20 ) || \
                                   ( codec == CODEC_ILBC_30 ) || \
                                   ( codec == CODEC_G7231_53 ) || \
                                   ( codec == CODEC_G7231_63 ) || \
                                   ( codec == CODEC_G7231A_53 ) || \
                                   ( codec == CODEC_G7231A_63 ) || \
                                   ( codec == CODEC_G7231_53_VAR ) || \
                                   ( codec == CODEC_G7231_63_VAR ) || \
                                   ( codec == CODEC_G7231_VAR ) )

#define DISABLE_ECAN_ON_TDD_EVENT         1


/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/

/*
** CONST Mapping of algorithm code to algorithm name
*/
typedef struct EP_ALGS_MAP
{
   HAPI_PKT_VOICECODE   algsCode;
   const char*          algsName;

} EP_ALGS_MAP;

/* Constant string, value of the string cannot be changed */
typedef const char *cstring;

/* Constant pointer to a constant string, neither the pointer nor the string value
 * can be changed. */
typedef cstring const* cpcstring;

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

static const EPZSIG2PTEDIGITMAP epSig2PteDigitMap[] =
{
   { EPSIG_DTMF0, HAPI_PTE_DTMFDDGT0   },
   { EPSIG_DTMF1, HAPI_PTE_DTMFDDGT1   },
   { EPSIG_DTMF2, HAPI_PTE_DTMFDDGT2   },
   { EPSIG_DTMF3, HAPI_PTE_DTMFDDGT3   },
   { EPSIG_DTMF4, HAPI_PTE_DTMFDDGT4   },
   { EPSIG_DTMF5, HAPI_PTE_DTMFDDGT5   },
   { EPSIG_DTMF6, HAPI_PTE_DTMFDDGT6   },
   { EPSIG_DTMF7, HAPI_PTE_DTMFDDGT7   },
   { EPSIG_DTMF8, HAPI_PTE_DTMFDDGT8   },
   { EPSIG_DTMF9, HAPI_PTE_DTMFDDGT9   },
   { EPSIG_DTMFS, HAPI_PTE_DTMFDDGTSTR },
   { EPSIG_DTMFH, HAPI_PTE_DTMFDDGTPND },
   { EPSIG_DTMFA, HAPI_PTE_DTMFDDGTA   },
   { EPSIG_DTMFB, HAPI_PTE_DTMFDDGTB   },
   { EPSIG_DTMFC, HAPI_PTE_DTMFDDGTC   },
   { EPSIG_DTMFD, HAPI_PTE_DTMFDDGTD   },
   { EPSIG_NULL,  HAPI_PTE_DTMFDDGTERROR }
};

/*
** Net VHD voice algorithms
*/
static const EP_ALGS_MAP algsMap[] =
{
   { HAPI_PKT_VC_G711_ULAW,      "G.711-u"},
   { HAPI_PKT_VC_G711_ALAW,      "G.711-a"},
   { HAPI_PKT_VC_G722_64_MODE_1, "G.722 Mode 1 64 kbps"},
   { HAPI_PKT_VC_G726_16,        "G.726 16 kbps"},
   { HAPI_PKT_VC_G726_24,        "G.726 24 kbps"},
   { HAPI_PKT_VC_G726_32,        "G.726 32 kbps"},
   { HAPI_PKT_VC_G726_40,        "G.726 40 kbps"},

   { HAPI_PKT_VC_G729A_8,        "G.729 (Annex A)"},
   { HAPI_PKT_VC_G729_8,         "G.729 8"},
   { HAPI_PKT_VC_G729_118,       "G.729 (Annex E)"},
   { HAPI_PKT_VC_G7231_53,       "G.723.1 5.3 kbps"},
   { HAPI_PKT_VC_G7231_63,       "G.723.1 6.3 kbps"},
   { HAPI_PKT_VC_G728_16,        "G.728"},

   { HAPI_PKT_VC_BV16,           "BV 16"},
   { HAPI_PKT_VC_ILBC20,         "iLBC 15.2 kbps"},
   { HAPI_PKT_VC_ILBC30,         "iLBC 13.3 kbps"},
   { HAPI_PKT_VC_G711WB_ULAW,    "G.711 wb-u"},
   { HAPI_PKT_VC_G711WB_ALAW,    "G.711 wb-a"},
   { HAPI_PKT_VC_BV32,           "BV 32"},
   { HAPI_PKT_VC_LINPCM64,       "Linear PCM 64 kbps"},
   { HAPI_PKT_VC_LINPCM128,      "Linear PCM 128 kbps"},
   { HAPI_PKT_VC_LINPCM256,      "Linear PCM 256 kbps"},

   { HAPI_PKT_VC_GSM_AMR_122,    "GSM AMR 12.2 kbps"},
   { HAPI_PKT_VC_GSM_AMR_102,    "GSM AMR 10.2 kbps"},
   { HAPI_PKT_VC_GSM_AMR_795,    "GSM AMR 7.95 kbps"},
   { HAPI_PKT_VC_GSM_AMR_740,    "GSM AMR 7.40 kbps"},
   { HAPI_PKT_VC_GSM_AMR_670,    "GSM AMR 6.70 kbps"},
   { HAPI_PKT_VC_GSM_AMR_590,    "GSM AMR 5.90 kbps"},
   { HAPI_PKT_VC_GSM_AMR_515,    "GSM AMR 5.15 kbps"},
   { HAPI_PKT_VC_GSM_AMR_475,    "GSM AMR 4.75 kbps"},
   { HAPI_PKT_VC_GSM_AMR_OA_122, "GSM AMR OA 12.2 kbps"},
   { HAPI_PKT_VC_GSM_AMR_OA_102, "GSM AMR OA 10.2 kbps"},
   { HAPI_PKT_VC_GSM_AMR_OA_795, "GSM AMR OA 7.95 kbps"},
   { HAPI_PKT_VC_GSM_AMR_OA_740, "GSM AMR OA 7.40 kbps"},
   { HAPI_PKT_VC_GSM_AMR_OA_670, "GSM AMR OA 6.70 kbps"},
   { HAPI_PKT_VC_GSM_AMR_OA_590, "GSM AMR OA 5.90 kbps"},
   { HAPI_PKT_VC_GSM_AMR_OA_515, "GSM AMR OA 5.15 kbps"},
   { HAPI_PKT_VC_GSM_AMR_OA_475, "GSM AMR OA 4.75 kbps"},

   { HAPI_PKT_VC_GSM_EFR_122,    "GSM EFR 12.2 kbps"},

   { HAPI_PKT_VC_WB_AMR_660,     "AMR WB 6.60 kbps"},
   { HAPI_PKT_VC_WB_AMR_885,     "AMR WB 8.85 kbps"},
   { HAPI_PKT_VC_WB_AMR_1265,    "AMR WB 12.65 kbps"},
   { HAPI_PKT_VC_WB_AMR_1425,    "AMR WB 14.25 kbps"},
   { HAPI_PKT_VC_WB_AMR_1585,    "AMR WB 15.85 kbps"},
   { HAPI_PKT_VC_WB_AMR_1825,    "AMR WB 18.25 kbps"},
   { HAPI_PKT_VC_WB_AMR_1985,    "AMR WB 19.85 kbps"},
   { HAPI_PKT_VC_WB_AMR_2305,    "AMR WB 23.05 kbps"},
   { HAPI_PKT_VC_WB_AMR_2385,    "AMR WB 23.85 kbps"},
   { HAPI_PKT_VC_WB_AMR_OA_660,  "AMR WB OA 6.60 kbps"},
   { HAPI_PKT_VC_WB_AMR_OA_885,  "AMR WB OA 8.85 kbps"},
   { HAPI_PKT_VC_WB_AMR_OA_1265, "AMR WB OA 12.65 kbps"},
   { HAPI_PKT_VC_WB_AMR_OA_1425, "AMR WB OA 14.25 kbps"},
   { HAPI_PKT_VC_WB_AMR_OA_1585, "AMR WB OA 15.85 kbps"},
   { HAPI_PKT_VC_WB_AMR_OA_1825, "AMR WB OA 18.25 kbps"},
   { HAPI_PKT_VC_WB_AMR_OA_1985, "AMR WB OA 19.85 kbps"},
   { HAPI_PKT_VC_WB_AMR_OA_2305, "AMR WB OA 23.05 kbps"},
   { HAPI_PKT_VC_WB_AMR_OA_2385, "AMR WB OA 23.85 kbps"},

   { HAPI_PKT_VC_VOICE_NOCODE,   "Unknown" }
};

/*
** Vad types
*/

static const cstring vadtypes[HAPI_NUMVADTYPES] =
{
   "",
   "gVAD",
   "built-in VAD"
};

static VRG_CLASS_FSK       fskParmsPlaceHolder[VRG_ENDPT_CFG_NUM_ENDPT];
static VRG_CLASS_DTMF      dtmfParmsPlaceHolder[VRG_ENDPT_CFG_NUM_ENDPT];

#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
static VRG_UINT8 DectModeCheck = VRG_FALSE;
#endif /* VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT */

/*
*****************************************************************************
** LOCAL FUNCTION PROTOTYPES
*****************************************************************************
*/

#define CASE_PROCESS_HAPI_EVT( hapiEvt )                       \
   case ( (hapiEvt) ):                                         \
   {                                                           \
      evtStr = #hapiEvt;                                       \
   }                                                           \
   break;

#define VRG_DATA_MODE_T38(mode) \
   (((mode) == EPDATAMODE_T38 || (mode) == EPDATAMODE_T38_MUTE) ? 1 : 0 )

#if defined( NTR_SUPPORT )
static void ProcessVrgEndptNtrCmd( ENDPT_STATE *endptState, VRG_UINT8 ntrAction, VRG_UINT8 isEnbl, VRG_SINT32 pcmStepsAdjust,
                              VRG_UINT32 *localCount, VRG_UINT32 *ntrCount, VRG_UINT32 *ndivInt, VRG_UINT32 *ndivFrac );
#endif /* NTR_SUPPORT */
static EPSTATUS ProcessVrgEndptCapabilities( ENDPT_STATE *endptState, EPZCAP* capabilities );
static EPSTATUS ProcessVrgEndptSignal( ENDPT_STATE   *endptState, int cnxId, EPSIG signal,
                                  unsigned int value, int duration, int period, int repetition );
static EPSTATUS ProcessVrgEndptCreate( int physId, int lineId, VRG_ENDPT_STATE *endptState );
static EPSTATUS ProcessVrgEndptDestroy( VRG_ENDPT_STATE *vrgEndptState );
static EPSTATUS ProcessVrgEndptCreateConnection( ENDPT_STATE *endptState, int cnxId, EPZCNXPARAM *cnxParam );
static EPSTATUS ProcessVrgEndptModifyConnection( ENDPT_STATE *endptState, int cnxId, EPZCNXPARAM *cnxParam );
static EPSTATUS ProcessVrgEndptDeleteConnection( ENDPT_STATE *endptState, int cnxId );
static EPSTATUS ProcessVrgEndptConsoleCmd( ENDPT_STATE *endptState, EPCONSOLECMD cmd, EPCMD_PARMS *consoleCmdParams );
static EPSTATUS ProcessVrgEndptMuteConnection( ENDPT_STATE *endptState, int cnxId, VRG_BOOL mute );
static EPSTATUS ProcessVrgEndptLinkInternal( ENDPT_STATE *endptState, ENDPT_STATE *otherEndptState, VRG_BOOL link );
static EPSTATUS ProcessVrgEndptGetRtpStats( ENDPT_STATE *endptState, int cnxId, EPZCNXSTATS *epRtpStats, VRG_BOOL bReset );
static void     ProcessVrgEndptGetT38Stats( ENDPT_STATE *endptState, int cnxId, EPZT38CNXSTATS *epT38Stats );

#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
static void DectEventProcess( VRG_DECT_EVT *dectEvt );
#endif /* VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT */

#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
static void TpdEventProcess( VRG_TPD_EVT *tpdEvt );
#else
static void MltEventProcess( VRG_MLT_EVT *mltEvt );
#endif
#endif

static void CasEventProcess( VRG_CAS_EVT *casEvt );
static void HapiEndptEventProcess( VRG_HAPI_EVT *hapiEvt );
static void HapiCnxEventProcess( VRG_HAPI_EVT *hapiEvt );
static void HapiEndptEventPostProcess( VRG_HAPI_EVT *hapiEvt );
static void HapiCnxEventPostProcess( VRG_HAPI_EVT *hapiEvt );
static void HapiRmEventPostProcess( VRG_HAPI_EVT *hapiEvt );
static void NteEventProcess(VRG_NTE_EVT *nteEvent);
static void PstnEventProcess( VRG_PSTN_EVT *pstnEvt );

extern void SetAdditionalGain( CNXSTATE *cnx, VRG_BOOL bAddGainVBD );
extern void SetSilenceSuppression( CNXSTATE *cnx, VRG_UINT16 vad, VRG_UINT16 plc, VRG_UINT16 comfortnoise );

/*
*****************************************************************************
** EXTERNS
*****************************************************************************
*/
extern void GetRtpRegisters(CNXSTATE *cnx);
extern void ProgramRtpRegisters(CNXSTATE *cnx);
extern void SetRtcpInterval( CNXSTATE *cnx, VRG_UINT16 intMs );


#ifndef BRCM_NONDIST_BUILD
extern XDRV_ITPC gHalItpcDrv;
#endif

/*****************************************************************************
*  FUNCTION:   ProcessVrgEndptCapabilities
*
*  PURPOSE:    Retrieve the capabilities for an endpoint
*
*  PARAMETERS: endptState   - a pointer to endpoint state structure
*              capabilities - a pointer to the capabilities struct to populate
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS ProcessVrgEndptCapabilities
(
   ENDPT_STATE   *endptState,
   EPZCAP        *capabilities
)
{
   VRG_ENDPT     *state;
   VRG_BOOL       bLiteWeightCnx;
   int            i;
   int            lineId;
   int            dspNum;
   int            j;

   lineId = ((VRG_ENDPT_STATE *) endptState)->lineId;
   dspNum = lineId / (VRG_ENDPT_CFG_NUM_ENDPT/VRG_GLOBAL_CFG_MAX_NUM_DSP);

   /* Initialize Endpoint Capabilities */
   *capabilities = vrgEndptCap[dspNum];

   /*
   ** Get the endpoint state
   */
   state = GetEndptState( lineId );
   if( ! state )
   {
      resetCodecCap(capabilities->codecCap);
      return ( EPSTATUS_ENDPT_UNKNOWN );
   }

   /* Get the CLID signaling information for this endpoint. */
   Provision( endptState, EPPROV_CIDSigProtocol, &capabilities->clidType );
   if( capabilities->clidType == EPCLIDTYPE_DTMF )
   {
      VRG_CLASS_DTMF dtmfParms;
      Provision( endptState, EPPROV_CIDDtmfParms, &dtmfParms );
      capabilities->clidData[0] = GetDigitEvent( (HAPI_PTE_DTMFDDGT)dtmfParms.startCode );
      capabilities->clidData[1] = GetDigitEvent( (HAPI_PTE_DTMFDDGT)dtmfParms.endCode );
   }

   bLiteWeightCnx = VRG_FALSE;

#if FLEXIBLE_CONFERENCING_SUPPORTED
   /* Check for any reservation */
   for ( i = PRIMARY_CNX_INDEX; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
      {
         if ( state->cnx[i] && (state->cnx[i]->cnxId == CNX_IDLE) )
         {
            bLiteWeightCnx = state->cnx[i]->bLiteWeight;
            break;
         }
      }

      if ( i == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT )
#else
   /* Check if the primary vhd is in use */
   if ( state->cnx[PRIMARY_CNX_INDEX]->cnxId == CNX_UNUSED )
   {
      bLiteWeightCnx = state->cnx[PRIMARY_CNX_INDEX]->bLiteWeight;
   }
   /* Else check for the next unused secondary VHD */
   else
#endif
      {
#if FLEXIBLE_CONFERENCING_SUPPORTED
      for ( i = 0; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
#else
      /* Check that there is an available endpoint connection */
      for ( i = SECONDARY_CNX_START_INDEX; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
#endif
      {
         if ( state->cnx[i] == NULL )
         {
            break;
         }
      }
      /* If there is no availabe endpoint connection return no capabilities */
      if ( state->cnx[i] != NULL )
      {
         resetCodecCap(capabilities->codecCap);
         return ( EPSTATUS_SUCCESS );
      }

      /* Check that there is a free connection in the pool */
#if FLEXIBLE_CONFERENCING_SUPPORTED
      j = i;

      for ( i = 0; i < VRG_ENDPT_CFG_NUM_CNX; i++ )
#else
      for ( i = vrgEndptGetNumEndpoints(); i < VRG_ENDPT_CFG_NUM_CNX; i++ )
#endif
      {
         if ( (vrgCnxState[i].dspNum == dspNum) &&
              (vrgCnxState[i].cnxId == CNX_UNUSED) )
         {
#if FLEXIBLE_CONFERENCING_SUPPORTED
            /* reserve this resource for following connection */
            state->cnx[j] = &vrgCnxState[i];
            state->cnx[j]->cnxId = CNX_IDLE;
#endif
            bLiteWeightCnx = vrgCnxState[i].bLiteWeight;
        break;
         }
      }
      /* If there is no free connection in the pool return no capabilities */
      if ( i == VRG_ENDPT_CFG_NUM_CNX )
      {
         resetCodecCap(capabilities->codecCap);
         return ( EPSTATUS_SUCCESS );
      }
   }

   /* If the primary connection associated with the endpoint is a "lite-weight"
   ** connection, then remove all advanced vocoders from the endpoint capabilities. */
   if ( bLiteWeightCnx )
   {
      for (i = 0; i < CODEC_MAX_TYPES; i++)
      {
         if ( i != CODEC_PCMU && i != CODEC_PCMA && i != CODEC_NTE && i != CODEC_RED )
         {
            capabilities->codecCap[i] = CODEC_UNSUPPORTED;
         }
      }
   }


   /*
   ** Since RTP only recognizes G.729, not G.729A. we will report G.729 in our
   ** capabilities if the DSP supports either codec to the outside world.
   ** Internally, we need to keep track of which codec is actually supported
   ** by the DSP so that we can correctly convert RTP packets into xChange packets.
   */
   else if ( capabilities->codecCap[CODEC_G729A] == CODEC_SUPPORTED )
   {
      capabilities->codecCap[CODEC_G729A] = CODEC_UNSUPPORTED;
      capabilities->codecCap[CODEC_G729] = CODEC_SUPPORTED;
   }

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   ProcessVrgEndptConsoleCmd
**
** PURPOSE:    Display endpoint info to console
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
EPSTATUS ProcessVrgEndptConsoleCmd
(
   ENDPT_STATE   *endptState,
   EPCONSOLECMD   cmd,
   EPCMD_PARMS   *consoleCmdParams
)
{
   VRG_ENDPT     *state;
   XDRV_CAS      *casDriver;
   int            lineId = ((VRG_ENDPT_STATE *) endptState)->lineId;
   XDRV_DEBUG    *pDebugDriver;
   EPSTATUS      status = EPSTATUS_SUCCESS;

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "CONSOLECMD: ept:%d cmd:%d args: %d %d %d",
                                                  lineId,
                                                  cmd,
                                                  consoleCmdParams->arg[0],
                                                  consoleCmdParams->arg[1],
                                                  consoleCmdParams->arg[2] ));

   /* Get the endpoint state */
   state = GetEndptState( lineId );
   if ( state == NULL )
   {
      return ( EPSTATUS_ERROR );
   }

   /* Get CAS pointer */
   casDriver = boardHalCasGetDriver( state->endptNum );

   switch ( cmd )
   {
      case EPCMD_DSP_MIPS:
      {
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "Current DSP MIPS usage: %ld", hdspRmGetTotalMips() ));
      }
      break;

      case EPCMD_PRINT_VHDS:
      {
         PrintVhdInfo();
      }
      break;

      case EPCMD_ENDPTVHD_STATS:
      {
         if ( state )
         {
            hdspVhdDisplayEndptStats( state->lineVhdHdl );
         }
      }
      break;

      case EPCMD_VHD_STATS_FROM_CNX:
      {
         HSZNETVOICESTATS   netvoicestats;
         VRG_UINT16 vhdHdl = (VRG_UINT16)consoleCmdParams->arg[0];

         if( HAPISUCCESS == hdspVhdGetVoiceStats( vhdHdl, &netvoicestats ) )
         {
            hdspVhdDisplayVoiceStatsRaw( vhdHdl, &netvoicestats );
         }
         else
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Cannot display Voice Stats for vhdhdl = 0x%x", vhdHdl ));
         }
      }
      break;

      case EPCMD_VHD_STATS:
      {
         VRG_UINT16 vhdhdl = (VRG_UINT16)consoleCmdParams->arg[0];

         if (IsCnxVhd(vhdhdl))
         {
            HSZNETVOICESTATS netvoicestats;

            if( HAPISUCCESS == hdspVhdGetVoiceStats( vhdhdl, &netvoicestats ) )
            {
               hdspVhdDisplayVoiceStatsRaw( vhdhdl, &netvoicestats );
            }
            else
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Cannot display Voice Stats for vhdhdl = 0x%x", vhdhdl ));
            }
            hdspVhdDisplayPveState( vhdhdl );
         }
         else
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "VHD handle 0x%x is not a connection VHD !!!", vhdhdl ));
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Type 'vhd_info' to display VHD handle assignments"));
         }
      }
      break;

      case EPCMD_RTP_STATS:
      {
         EPZCNXSTATS epRtpStats;
         int cnxId = consoleCmdParams->arg[0];
         VRG_BOOL bReset = consoleCmdParams->arg[3];

         if(ProcessVrgEndptGetRtpStats( endptState, cnxId, &epRtpStats, bReset) == EPSTATUS_SUCCESS)
         {
            vrgEndptPrintRtpStats(&epRtpStats);
         }
      }
      break;

      case EPCMD_VHD_JBREGS:
      {
         VRG_UINT16 vhdHdl = (VRG_UINT16)consoleCmdParams->arg[0];

         if (IsCnxVhd(vhdHdl))
         {
            VRG_BOOL          voiceJBFixed;
            VRG_UINT16        voiceJBMin;
            VRG_UINT16        voiceJBMax;
            VRG_UINT16        voiceJBTarget;
            VRG_UINT16        dataJBTarget;
            VRG_UINT16        ajcCustomReg;
            VRG_SINT16        result;

            result = hdspVhdGetJitterBuffer( vhdHdl,
                                             &voiceJBFixed,
                                             &voiceJBMin,
                                             &voiceJBMax,
                                             &voiceJBTarget,
                                             &dataJBTarget,
                                             &ajcCustomReg );

            if( result != HAPISUCCESS )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR getting jitter buffer registers for VHD handle 0x%x", vhdHdl ));
            }
            else
            {
               /* Display registers. */
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "VHD Jitter Buffer settings for vhdhdl = %x", vhdHdl ));
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "------------------------------------------"));
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "voiceJBFixed = %s", (voiceJBFixed) ? ("FIXED") : ("ADAPTIVE") ));
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "voiceJBMin = %d (ms)", voiceJBMin ));
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "voiceJBMax = %d (ms)", voiceJBMax ));
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "voiceJBTarget = %d (ms)", voiceJBTarget ));
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "dataJBTarget = %d (ms)", dataJBTarget ));
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "ajcCustomReg = 0x%x", ajcCustomReg ));
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "------------------------------------------"));
            }
         }
         else
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "VHD handle 0x%x is not a connection VHD !!!", vhdHdl ));
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Type 'vhd_info' to display VHD handle assignments"));
         }
      }
      break;

      case EPCMD_FAXR_STATS:
      {
         VRG_UINT16 vhdHdl = (VRG_UINT16)consoleCmdParams->arg[0];

         if (IsCnxVhd(vhdHdl))
         {
            hdspVhdDisplayFaxrStats( vhdHdl );
         }
         else
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "VHD handle 0x%x is not a connection VHD !!!", vhdHdl ));
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Type 'vhd_info' to display VHD handle assignments"));
         }
      }
      break;

      case EPCMD_IPC_STATS:
      {
#if BRCM_NONDIST_BUILD
         VRG_LOG_INFO((VRG_LOG_MOD_EPT,  "IPC not supported"));
#else
         BOARD_HAL_IPC_COM_HANDLE ipcDumpComHandle = (BOARD_HAL_IPC_COM_HANDLE)consoleCmdParams->arg[0];
         int ipcDumpMode = consoleCmdParams->arg[1];
         boardHalIpcDump( ipcDumpComHandle, ipcDumpMode );
#endif /* BRCM_NONDIST_BUILD */
      }
      break;

      case EPCMD_DSP_BUFFERS:
      {
         int dspNum = consoleCmdParams->arg[0];
         hdspRmQueryDspBuffers( dspNum );
      }
      break;

      case EPCMD_PWRMGMT_VPM:
      {
#if BRCM_NONDIST_BUILD
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "Power saving mode not supported!"));
#else
         int enable = consoleCmdParams->arg[0];

         if (enable)
         {
            boardHalPowerSavingsEnable();
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Power save mode enabled."));
         }
         else
         {
            boardHalPowerSavingsDisable();
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Power save mode disabled."));
         }
#endif /* BRCM_NONDIST_BUILD */
      }
      break;

      case EPCMD_PWRMGMT_AFE:
      {
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "AFE power management not supported!" ));
      }
      break;

      case EPCMD_DSP_GETCNT:
      {
#if BRCM_NONDIST_BUILD
         unsigned int i, intCount;

         for ( i = 0; i < 3; i++ )
         {
            intCount = boardHalIntCountGet();
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Interrupt  count (%d)  0x%x", i, intCount ));
         }
#else
         pDebugDriver = vrgEndptGetDebugDriver();
         if ( pDebugDriver )
         {
            if ( xdrvDebugDspReadCnt( pDebugDriver ) != 0 )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "No response from DSP or DSP counters not moving !!!" ));
               status = EPSTATUS_RESET_PENDING;
            }
         }
#endif /* BRCM_NONDIST_BUILD */
      }
      break;

      case EPCMD_DSP_FORCEASSERT:
      {
#if BRCM_NONDIST_BUILD
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "Force assert not supported!"));
#else
         pDebugDriver = vrgEndptGetDebugDriver();
         if ( pDebugDriver )
         {
            xdrvDebugDspForceAssert( pDebugDriver );
         }
#endif /* BRCM_NONDIST_BUILD */
      }
      break;

      case EPCMD_DSP_CHECKASSERT:
      {
#if BRCM_NONDIST_BUILD
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "Check assert not supported"));
#else
         pDebugDriver = vrgEndptGetDebugDriver();
         if ( pDebugDriver )
         {
            xdrvDebugDspGetAssertStackPtr( pDebugDriver );
         }
#endif /* BRCM_NONDIST_BUILD */
      }
      break;

      case EPCMD_DSP_GETREGS:
      {
#if BRCM_NONDIST_BUILD
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "DSP get regs not supported"));
#else
         pDebugDriver = vrgEndptGetDebugDriver();
         if ( pDebugDriver )
         {
            xdrvDebugDspGetRegs( pDebugDriver );
         }
#endif /* BRCM_NONDIST_BUILD */
      }
      break;

      case EPCMD_DSP_READDMEM:
      {
         pDebugDriver = vrgEndptGetDebugDriver();
         if ( pDebugDriver )
         {
            xdrvDebugDspDumpDataMem( pDebugDriver, consoleCmdParams->arg[0], consoleCmdParams->arg[1]);
         }
      }
      break;


      case EPCMD_TR57_ENABLE:
      {
         int enableTr57;
         int cnxId;
         CNXSTATE      *cnx;

         cnxId = consoleCmdParams->arg[0];

         /* Get connection state */
         cnx = GetCnxState( cnxId );


         if (state)
         {
            /* cnxId carries the enable/disable info */
            enableTr57 = consoleCmdParams->arg[1];

            if (enableTr57)
            {
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Enabling TR57 test mode on line %d ...", state->lineId));

               if (state->offHook)
               {
                  /* Set the line into VBD mode */
                  cnx->vbData.bEnabled = VRG_TRUE;
                  SetVoiceMode( cnx, VM_ACTIVE );

                  /* Turn the ecan off - bypass the Ecan state machine */
                  EcanStateSet(state, ECAN_OFF);
               }

               state->testMode = TESTMODE_TR57;

               VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: TR57 test mode enabled.", state->lineId));
            }
            else
            {
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Disabling TR57 test mode on line %d ...", state->lineId));
               state->testMode = TESTMODE_NONE;

               if (state->offHook)
               {
                  /* Set the line into voice mode */
                  cnx->vbData.bEnabled = VRG_FALSE;
                  SetVoiceMode( cnx, VM_ACTIVE );

                  /* Turn the ecan on - bypass the Ecan state machine */
                  EcanStateSet(state, ECAN_ON);
               }

               VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: TR57 test mode disabled.", state->lineId));
            }
         }
      }
      break;

      case EPCMD_VHD_LOOPBACK_ENABLE:
      {
      /*
      ** FIXME: This code results in compilation errors. cnxId used below is not
      **        defined.
      */
      #if 0
         int enableVHDloopback;

         if (state)
         {
            /* cnxId carries the enable/disable info */
            enableVHDloopback = cnxId;

            if (enableVHDloopback)
            {
               state->testMode = TESTMODE_PACKETLOOPBACK;
               UpdateSBMasks(state->cnx[PRIMARY_CNX_INDEX], EPCNXMODE_SNDRX);
               /*
               ** Put gateway VHD into packet-voice mode.
               */
               SetVoiceMode( state->cnx[PRIMARY_CNX_INDEX], VM_ACTIVE );
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: VHD loopback enabled.", state->lineId));
            }
            else
            {
               /*
               ** Put gateway VHD into idle mode.
               */
               SetVoiceMode( state->cnx[PRIMARY_CNX_INDEX], VM_IDLE );
               UpdateSBMasks(state->cnx[PRIMARY_CNX_INDEX], state->cnx[PRIMARY_CNX_INDEX]->mode);
               state->testMode = TESTMODE_NONE;

               VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: VHD loopback disabled.", state->lineId));
            }
         }
      #endif
      }
      break;

      case EPCMD_READ_HOOK_STATE:
      {
         if (state)
         {
            XDRV_SLIC *slicDriver = boardHalSlicGetDriver( state->endptNum );
            XDRV_BOOL isOffhook = xdrvSlicIsOffhook(slicDriver);
            if (isOffhook)
            {
               state->offHook = VRG_TRUE;
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: OFF-HOOK", state->lineId ));
            }
            else
            {
               state->offHook = VRG_FALSE;
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: ON-HOOK", state->lineId ));
               status = EPSTATUS_ERROR;
            }

            if ( state->bGR303Support )
            {
               /* Determine Connection State from specified Identifier */
               int cnxId = consoleCmdParams->arg[0];
               int cnxIndex = 0;
               for( ; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
               {
                  if ((state->cnx[cnxIndex]) && (state->cnx[cnxIndex]->cnxId == cnxId))
                  {
                     /* cnx index found */
                     break;
                  }
               }

               if (cnxIndex == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT)
               {
                  /* specified cnx is not within the endpoint's cnx list */
                  break;
               }

               GenerateNteEvent( state->cnx[cnxIndex], (state->offHook ? RTP_NTE_OFFHOOK : RTP_NTE_ONHOOK) );
            }
         }
      }
      break;

      case EPCMD_SET_SLIC_STATE:
      {
         if ( state )
         {
            int slicState = consoleCmdParams->arg[0];
            switch (slicState)
            {
               case EPSLIC_MODE_LCF:
               {
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: SLIC in active mode", lineId));
                  xdrvCasSetSlicMode( casDriver, XDRV_SLIC_MODE_LCF );
               }
               break;

               case EPSLIC_MODE_STANDBY:
               {
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: SLIC in Standby mode", lineId));
                  xdrvCasSetSlicMode( casDriver, XDRV_SLIC_MODE_STANDBY);
               }
               break;

               case EPSLIC_MODE_LCFO:
               {
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: SLIC in disconnect mode", lineId));
                  xdrvCasSetSlicMode( casDriver, XDRV_SLIC_MODE_LCFO );
               }
               break;

               default:
               {
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: SLIC mode unchanged", lineId));
               }
               break;
            }
         }
      }
      break;

      case EPCMD_PRINT_DSP_CAPABILITIES:
      {
         boardHalDspDisplayCfg();
      }
      break;

      case EPCMD_DSP_STACK_SHOW:
      {
#if BRCM_NONDIST_BUILD
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "DSP stack show not supported"));
#else
         /* cnxId is actually a pointer to input arguments */
         pDebugDriver = vrgEndptGetDebugDriver();
         if ( pDebugDriver )
         {
            xdrvDebugDspStackShow( pDebugDriver );
         }
#endif /* BRCM_NONDIST_BUILD */
      }
      break;

      case EPCMD_DSP_ICP_CALIBRATE:
      {
#if BRCM_NONDIST_BUILD
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "Icp not supported"));
#else
         hdspIcpCalibrate();
#endif /* BRCM_NONDIST_BUILD */
      }
      break;

      case EPCMD_DSP_ICP_START:
      {
#if BRCM_NONDIST_BUILD
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "Icp not supported"));
#else
         hdspIcpStart( consoleCmdParams->arg[0], consoleCmdParams->arg[1] );
#endif /* BRCM_NONDIST_BUILD */
      }
      break;

      case EPCMD_DSP_ICP_STOP:
      {
#if BRCM_NONDIST_BUILD
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "Icp not supported"));
#else
         hdspIcpStop( VRG_TRUE );
#endif /* BRCM_NONDIST_BUILD */
      }
      break;

#if DPR_ENABLED
      case EPCMD_DSP_DPR_START:
      {
         VRG_UINT16 vhdhdl = (VRG_UINT16)consoleCmdParams->arg[0];
         if ( IsCnxVhd(vhdhdl) || (vhdhdl == state->lineVhdHdl))
         {
            /* Start dpr on dsp #1 */
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "" ));
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "Vhd 0x%x   Measure DPR ingress", vhdhdl ));
            startDprIngress( vhdhdl );
            bosSleep( 10*1000 );
            stopDpr();

            VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "" ));
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "Vhd 0x%x   Measure DPR egress", vhdhdl ));
            startDprEgress( vhdhdl );
            bosSleep( 10*1000 );
            stopDpr();
         }
         else
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "VHD handle (0x%x) is not a connection or line VHD !!!", vhdhdl ));
         }
      }
      break;
#endif

      case EPCMD_HAPINET_CONFIG_SETREG:
      {
         VRG_UINT16 vhdHdl = (VRG_UINT16)consoleCmdParams->arg[0];
         VRG_UINT16 offset = (VRG_UINT16)consoleCmdParams->arg[1];
         VRG_UINT16 newVal = (VRG_UINT16)consoleCmdParams->arg[2];

         if ( IsCnxVhd(vhdHdl) )
         {
            if( offset < sizeof( HSZNETREGS ) )
            {
               HSZNETREGS netRegs;

               if( HAPISUCCESS == hdspSendCmdData( vhdHdl,
                                HAPINET_CONFIG_GETREGS,
                                HSCMDEXTDATA,
                                sizeof(netRegs),
                                &netRegs))
               {
                  /* Set the register value */
                  *(VRG_UINT16 *)(((unsigned int)&netRegs) + offset) = newVal;

                  if( HAPISUCCESS != hdspSendCmdData( vhdHdl,
                                                      HAPINET_CONFIG_SETREGS,
                                                      HSCMDEXTDATA,
                                                      sizeof(netRegs),
                                                      &netRegs ))
                  {
                     VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Unable to set register." ));
                  }
               }
               else
               {
                     VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Unable to get registers." ));
               }
            }
            else
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Specified offset is bigger than structure size (%d)!!!",
                        sizeof(HSZNETREGS) ));
            }
         }
         else
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "VHD handle 0x%x is not a connection VHD !!!", vhdHdl ));
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Type 'vhd_info' to display VHD handle assignments"));
         }
      }
      break;

      case EPCMD_CONFIGURE_NETWTEST:
      {
         /*
         ** Remove the switchboard connection from the endpoint VHD to the
         ** connection VHD.
         */
         if ( HAPISUCCESS != hdspRmSwitchBoardConnect( state->lineVhdHdl,
                                                       HAPI_SWB_TOS,
                                                       state->cnx[PRIMARY_CNX_INDEX]->vhdhdl,
                                                       HAPI_SWB_BOS,
                                                       HAPI_SWB_SIMPLEX,
                                                       VRG_FALSE ) )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR removing line->gateway VHD switchboard "
                      "connection for endpt %d", state->lineId ));
         }

         if ( HAPISUCCESS != hdspRmSwitchBoardConnect( state->cnx[PRIMARY_CNX_INDEX]->vhdhdl,
                                                       HAPI_SWB_BOS,
                                                       state->lineVhdHdl,
                                                       HAPI_SWB_TOS,
                                                       HAPI_SWB_SIMPLEX,
                                                       VRG_TRUE ) )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR adding gateway->line VHD switchboard "
                      "connection for endpt %d", state->lineId ));
         }
      }
      break;

      case EPCMD_HEC_BURST_ENABLE:
      {
         VRG_SINT16    result;

         result = hdspSendCmdAsync( state->lineVhdHdl,
                                    HAPI_HEC_SETREG1_CMD,
                                    HSCMDDATA,
                                    VRG_OFFSETOF(HEC_REGS, options),
                                    HAPI_HEC_EGRESSBURST_ON );
         if ( result != HAPISUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting HEC Burst on %d", state->lineVhdHdl ));
         }

         result = hdspSendCmdAsync( state->lineVhdHdl,
                                    HAPI_HEC_SETREG1_CMD,
                                    HSCMDDATA,
                                    VRG_OFFSETOF(HEC_REGS, volumedB),
                                    10 * ( 1 << 8 ) );
         if ( result != HAPISUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting HEC Burst volume %d", state->lineVhdHdl ));
         }
      }
      break;

      case EPCMD_HEC_NOISE_ENABLE:
      {
         VRG_SINT16    result;

         result = hdspSendCmdAsync( state->lineVhdHdl,
                                    HAPI_HEC_SETREG1_CMD,
                                    HSCMDDATA,
                                    VRG_OFFSETOF(HEC_REGS, volumedB),
                                    10 * ( 1 << 8 ) );
         if ( result != HAPISUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting HEC Burst volume %d", state->lineVhdHdl ));
         }

         result = hdspSendCmdAsync( state->lineVhdHdl,
                                    HAPI_HEC_SETREG1_CMD,
                                    HSCMDDATA,
                                    VRG_OFFSETOF(HEC_REGS, options),
                                    HAPI_HEC_EGRESSNOISE_ON );
         if ( result != HAPISUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting HEC Burst on %d", state->lineVhdHdl ));
         }
      }
      break;

      case EPCMD_HEC_DEFAULT:
      {
         VRG_SINT16    result;

         result = hdspSendCmdAsync( state->lineVhdHdl,
                                    HAPI_HEC_SETREG1_CMD,
                                    HSCMDDATA,
                                    VRG_OFFSETOF(HEC_REGS, options),
                                    0 );
         if ( result != HAPISUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting HEC Burst on %d", state->lineVhdHdl ));
         }
      }
      break;

      case EPCMD_HEC_GET_STATE:
      {
         VRG_SINT16    result;

         result = hdspSendCmdAsync( state->lineVhdHdl,
                                    HAPI_HEC_GETSTATE_CMD,
                                    HSCMDDATA,
                                    0,
                                    0);
         if ( result != HAPISUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR triggering HEC get state %d", state->lineVhdHdl ));
         }
      }
      break;

      case EPCMD_HEC_DBG_SET:
      {
         VRG_SINT16 result = hdspSendCmd( state->lineVhdHdl,
                                          HAPI_HEC_SETREG1_CMD,
                                          HSCMDDATA,
                                          VRG_OFFSETOF(HEC_REGS, eventMask),
                                          consoleCmdParams->arg[0] );
         if(result != HAPISUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Error setting HEC Debug mask." ));
         }
      }
      break;

      case EPCMD_HEC_RESET:
      case EPCMD_HEC_RESET_COEF:
      {
         if(HAPISUCCESS != hdspSendCmd( state->lineVhdHdl,
                                        (cmd == EPCMD_HEC_RESET) ? HAPI_HEC_RESET_CMD : HAPI_HEC_RESETCOEFF_CMD,
                                        HSCMDDATA,
                                        consoleCmdParams->arg[0],
                                        0 ) )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Error sending HEC Reset." ));
         }
      }
      break;

      case EPCMD_CASEVT_GENERATE:
      {
         CasEventCallback( endptState->lineId, CAS_CTL_DETECT_EVENT, consoleCmdParams->arg[0] );
      }
      break;

      case EPCMD_TONEEVT_GENERATE:
      {
         HSZEVT evt;

         /* Currently only two event types: 0 - tone off event, 1 - tone on */
         int eventType = consoleCmdParams->arg[0];

         evt.hsxop2.val = consoleCmdParams->arg[1];
         evt.hsxdevhdl = 0x54 + endptState->lineId;

         switch ( eventType )
         {
            case 0:
            {
               evt.hsxevt = HAPI_PTE_TONE_OFF_EVT;
               EndptVhdEventCallback( &evt, 0 );
            }
            break;

            case 1:
            {
               evt.hsxevt = HAPI_PTE_TONE_ON_EVT;
               EndptVhdEventCallback( &evt, 0 );
            }
            break;

            default:
               break;
         }
      }
      break;

      case EPCMD_INDTMF_GENERATE:
      {
         VRG_UINT32 dbLevel, pulseTime, skew;
         int i;
         const EPZPTEDIGIT2EPEVTMAP * digitMap = digit2epevtMap;

         if ( consoleCmdParams->arg[0] < HAPI_PTE_DTMFDMAXDGT )
         {
            /* Let the heartbeat module know this endpt now has a cnx signal being applied */
            hbUpdatePatientState( state->cnxSignalHBId, HBSTATE_ACTIVE );

            /* Get the country specific DTMF settings */
            Provision( state->endptArchive, EPPROV_DtmfDbLevel, &dbLevel );
            Provision( state->endptArchive, EPPROV_DtmfPulseTime, &pulseTime );
            Provision( state->endptArchive, EPPROV_DTMFSkew, &skew );

            VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "Generate ingressDTMF: lineVhdHdl:%d, digit:%d, dbLevel:%lu, pulseTime:%lu",state->lineVhdHdl, consoleCmdParams->arg[0], dbLevel, pulseTime));

            /* Generate ingress signal */
            hdspDigitOn( state->lineVhdHdl, state->lineId, consoleCmdParams->arg[0], dbLevel, skew, pulseTime, INGRESS_TONE );

            /* Notify call manager */
            for ( i = 0; i < consoleCmdParams->arg[0]; i++ )
            {
               digitMap++;
            }

            (*endptConfig.notify)( state->endptArchive, -1, digitMap->epEvent, NULL, 0, EPDTMFACT_TONEON);
            (*endptConfig.notify)( state->endptArchive, -1, digitMap->epEvent, NULL, 0, EPDTMFACT_TONEOFF);
         }
      }
      break;

      case EPCMD_EGRESSGAIN_SET:
      {
         state->rxGain = consoleCmdParams->arg[0];

         hdspVhdSetEgressGain( state->lineVhdHdl, state->rxGain );
      }
      break;

      case EPCMD_INGRESSGAIN_SET:
      {
         state->txGain = consoleCmdParams->arg[0];

         hdspVhdSetIngressGain( state->lineVhdHdl, state->txGain );
      }
      break;

      case EPCMD_CNX_EGRESSGAIN_SET:
      {
         int egGaindB = consoleCmdParams->arg[0];
         int cnxId    = consoleCmdParams->arg[1];
         CNXSTATE      *cnx;

         /* Get connection state */
         cnx = GetCnxState( cnxId );
         cnx->rxVolume = egGaindB;
         if ( cnx == NULL )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT - cnx is not valid "));
            break;
         }

         hdspVhdSetPVEEgressVolume(cnx->vhdhdl, egGaindB );
      }
      break;

      case EPCMD_CNX_INGRESSGAIN_SET:
      {
         int inGaindB = consoleCmdParams->arg[0];
         int cnxId    = consoleCmdParams->arg[1];
         CNXSTATE      *cnx;

         /* Get connection state */
         cnx = GetCnxState( cnxId );
         cnx->txVolume = inGaindB;

         if ( cnx == NULL )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT - cnx is not valid "));
            break;
         }

         hdspVhdSetPVEIngressVolume(cnx->vhdhdl, inGaindB );
      }
      break;

#ifdef VRG_ENDPT_CFG_G168_TEST
      case EPCMD_G168SETUP:
      {
         /* We are assuming both lines are on a same DSP */
         VRG_ENDPT* state1 = GetEndptState( endptState->lineId );
         VRG_ENDPT* state2 = GetEndptState( consoleCmdParams->arg[0] );
         VRG_BOOL   setup  = consoleCmdParams->arg[1];

         /* Create duplex SB connection btw the two Line VHDs on bottom of stack */
         VRG_SINT16 result = hdspRmSwitchBoardConnect( state1->lineVhdHdl,
                                              HAPI_SWB_BOS,
                                              state2->lineVhdHdl,
                                              HAPI_SWB_BOS,
                                              HAPI_SWB_DUPLEX,
                                              setup );

         if ( result != EPSTATUS_SUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "SB %s failed for lines %d and %d ", setup? "connect" : "disconnect", endptState->lineId, consoleCmdParams->arg[0]));
         }

         /* Disconnect a simplex SB connection from the HAL devices to the Line VHDs */
         result = hdspRmSwitchBoardConnect( state1->ldxHalHdl,
                                            HAPI_SWB_TOS,
                                            state1->lineVhdHdl,
                                            HAPI_SWB_BOS,
                                            HAPI_SWB_SIMPLEX,
                                            !setup );

         if ( result != EPSTATUS_SUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "SB %s failed for lineVhdHdl 0x%x and ldxHalHdl 0x%x ", setup? "disconnect" : "connect", state1->lineVhdHdl, state1->ldxHalHdl ));
         }

         result = hdspRmSwitchBoardConnect( state2->ldxHalHdl,
                                            HAPI_SWB_TOS,
                                            state2->lineVhdHdl,
                                            HAPI_SWB_BOS,
                                            HAPI_SWB_SIMPLEX,
                                            !setup );

         if ( result != EPSTATUS_SUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "SB %s failed for lineVhdHdl 0x%x and ldxHalHdl 0x%x ", setup? "disconnect" : "connect", state2->lineVhdHdl, state2->ldxHalHdl ));
         }
      }
#else
      VRG_LOG(( "G168 Testing not supported\n"));
#endif
      break;

      case EPCMD_CONNECTSB:
      {
         /* We are assuming both lines are on a same DSP */
         VRG_ENDPT* state1 = GetEndptState( endptState->lineId );
         VRG_ENDPT* state2 = GetEndptState( consoleCmdParams->arg[0] );
         VRG_SINT16 result = hdspRmSwitchBoardConnect( state1->lineVhdHdl,
                                              HAPI_SWB_BOS,
                                              state2->lineVhdHdl,
                                              HAPI_SWB_BOS,
                                              HAPI_SWB_DUPLEX,
                                              VRG_TRUE );

         if ( result == EPSTATUS_SUCCESS )
         {
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Switchboard connection established between lines %d and %d ", endptState->lineId, consoleCmdParams->arg[0]));
         }
         else
         {
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ConnectSB failed for lines %d and %d ", endptState->lineId, consoleCmdParams->arg[0]));
         }
      }
      break;

      case EPCMD_SET_ECAN_STATE:         /* Force ECAN to a certain state (on, off, NLP off) */
      {
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Set ECAN state to %d ", consoleCmdParams->arg[0]));
         EcanStateSet(state, consoleCmdParams->arg[0]);
      }
      break;

      case EPCMD_DECT_START_BUFF_PROC:
      {
#ifndef BRCM_NONDIST_BUILD
         xdrvItpcSendSyncCommand( &gHalItpcDrv, ITPC_SET_START_DECT_BUFFER_PROCESSING_CMD_ID,(XDRV_ITPC_DATA)VRG_TRUE, 0 );
#else
         XDRV_UINT32 ddrAddrTxV, ddrAddrRxV;

         ddrAddrTxV = consoleCmdParams->arg[0];
         ddrAddrRxV = consoleCmdParams->arg[1];
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "DECT start buff proc: ddrAddrTxV = 0x%08x, ddrAddrRxV = 0x%08x", (unsigned int)ddrAddrTxV, (unsigned int)ddrAddrRxV));
         boardHalDectIStartDectBuffProcessing(ddrAddrTxV, ddrAddrRxV);
#endif
      }
      break;

      case EPCMD_DECT_STOP_BUFF_PROC:
      {
#ifndef BRCM_NONDIST_BUILD
         xdrvItpcSendSyncCommand( &gHalItpcDrv, ITPC_SET_START_DECT_BUFFER_PROCESSING_CMD_ID, VRG_FALSE, 0 );
#else
         boardHalDectIStopDectBuffProcessing();
#endif
      }
      break;

#if VRG_ENDPT_CFG_DECTTEST_SUPPORT
      case EPCMD_DECTTEST:
      {
         VRG_LOG_NOTICE((VRG_LOG_MOD_EPT, "Executing DECT TEST %d:", consoleCmdParams->arg[0]));
         dectTest(consoleCmdParams->arg[0]);
      }
      break;
#endif

      case EPCMD_PBDT_START:
      {
         /* Request related service */
         VRG_SINT16 result = hdspVhdEnableCmtd(state->lineVhdHdl, state->country);
         if (result != HAPISUCCESS)
         {
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "CMTD Service not available"));
            break;
         }

         /* Enter Diagnostic Mode */
         requestLoopback(state, VRG_TRUE);
      }
      break;

      case EPCMD_PBDT_STOP:
      {
         VRG_SINT16 result;
         requestLoopback(state, VRG_FALSE);

         /* Disable CMTD */
         result = hdspVhdDisableCmtd(state->lineVhdHdl);
         if (result != HAPISUCCESS)
         {
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "CMTD Service could not be disabled"));
         }
      }
      break;

      case EPCMD_PBDT_POWER:
      {
         VRG_UINT32 power;
         VRG_SINT16 result = hdspVhdGetCmtdPowerLevel(state->lineVhdHdl, &power);
         if (result != HAPISUCCESS)
         {
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Request for tone power failed"));
         }
         else
         {
            (*endptConfig.notify)( state->endptArchive, -1, EPEVT_DTMF_LEVEL, &power, sizeof(power), -1 );
         }
      }
      break;

      case EPCMD_TPD_START:
      {
#if VRG_ENDPT_CFG_USE_TPD
         requestTpd(state, EPSIG_DIAGTESTS_NOPROV_START, consoleCmdParams->arg[0]);
#else
         /* Ignore */
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Ignoring TPD START request" ));
#endif
      }
      break;

      case EPCMD_RTCP_NTP_TS_UPDATE:
      {
         VRG_BOOL    cnxIdFound = VRG_TRUE;
         HAPI_RTP_NTP_TIMESTAMP ntpTimestamp;
         int cnxId = consoleCmdParams->arg[0];

         if ( cnxIdFound == VRG_FALSE )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Connection id %d NOT found for line %d. Timestamp change not applied !!!", cnxId, lineId ));
            status = EPSTATUS_CNX_UNKNOWN;
            break;
         }

         /* Create 16-bit high/low values before passing the data to the DSP */
         ntpTimestamp.ntpTimeStampSecHigh    = (consoleCmdParams->arg[1] >> 16) & 0xFFFF;
         ntpTimestamp.ntpTimeStampSecLow     = (consoleCmdParams->arg[1]) & 0xFFFF;
         ntpTimestamp.ntpTimeStampUsecHigh   = (consoleCmdParams->arg[2] >> 16) & 0xFFFF;
         ntpTimestamp.ntpTimeStampUsecLow    = (consoleCmdParams->arg[2]) & 0xFFFF;

         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Setting NTP RTCP timestamp to %u sec %u usec ",
                                          (unsigned int)((ntpTimestamp.ntpTimeStampSecHigh << 16)  | ntpTimestamp.ntpTimeStampSecLow),
                                          (unsigned int)((ntpTimestamp.ntpTimeStampUsecHigh << 16) | ntpTimestamp.ntpTimeStampUsecLow)
                     ));

         /* RTCP timestamps are global variables on the DSP side so we only set it for VHD 0x50 (the first VHD) */
         if ( HAPISUCCESS != hdspSendCmdData( 0x50, HAPI_RTP_SET_NTP_TIMESTAMP_CMD, HSCMDEXTDATA, sizeof(ntpTimestamp), &ntpTimestamp ) )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Couldn't set NTP RTCP timestamp !!!" ));
         }
         else
         {
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "NTP RTCP timestamp SET " ));
         }
      }
      break;

      case EPCMD_PROVTABLE_SHOW:
      {
         endptProvShow();
      }
      break;

      case EPCMD_RESET_PROVTABLE:
      {
         endptProvResetDefaults(state->country);
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Resetting provision table to defaults." ));
      }
      break;

      case EPCMD_HOSTASSERT:
      {
         /* Intentionally cause host tp assert */
         XCHG_ASSERT( 0 );
      }

#if FEAT_HAL_CAPTURE_TO_FILE
      case EPCMD_HALCAP_SETUP:
      {
         lhapiHalCaptureInit( lineId );
      }
      break;

      case EPCMD_HALCAP_START:
      {
         lhapiHalCaptureStart();
      }
      break;

      case EPCMD_HALCAP_STOP:
      {
         lhapiHalCaptureStop();
      }
      break;

      case EPCMD_HALCAP_DEINIT:
      {
         lhapiHalCaptureDeInit();
      }
      break;
#endif  /* FEAT_HAL_CAPTURE_TO_FILE */

      case EPCMD_SET_AMR_CONFIG:
      {
         int cnxId = consoleCmdParams->arg[0];

         if( (cnxId >= VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT) || (state->cnx[cnxId] == NULL) )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Invalid cnxId for line %d", lineId ));
            return EPSTATUS_CNX_UNKNOWN;
         }

         status = hdspVhdSetAmrSettings( state->cnx[cnxId]->vhdhdl,   /* vhd handle */
                                          consoleCmdParams->arg[2],   /* mode set */
                                          consoleCmdParams->arg[3],   /* mode change neighbor */
                                          consoleCmdParams->arg[4],   /* mode change period */
                                          consoleCmdParams->arg[0],   /* ptime */
                                          consoleCmdParams->arg[1] ); /* redundancy */
         if( status != (EPSTATUS) HAPISUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting AMR registers for line %d", lineId ));
         }
      }
      break;

      default:
      {
      }
      break;
   }

   return ( status );
}

/*
*****************************************************************************
** FUNCTION:   ProcessVrgEndptCreate
**
** PURPOSE:    This function is used to create an VRG endpoint object.
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
EPSTATUS ProcessVrgEndptCreate( int physId, int lineId, VRG_ENDPT_STATE *endptState )
{
   VRG_ENDPT          *state;
   XDRV_CAS           *casDriver;
   CAS_CTL_FXO_PARMS  fxoParms;
   VRG_UINT32         maxMakeMsec;
   VRG_UINT32         interDigitMsec;
   VRG_UINT32         provItem;
   int                enableAnalogLoopback;
   EPSTATUS           status;
   VRG_BOOL           bPulse2dtmf;
   XDRV_SLIC          *slicDriver;
   CAS_TYPE           channelType = CAS_TYPE_FXO;
   VRG_BOOL           bGR303Support = VRG_FALSE;

   /* Validate the physical ID */
   XCHG_ASSERT( physId < vrgEndptGetNumEndpoints() );

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "ENDPT CREATE: Starting initialization for endpt %d", physId ));

   /*
   ** Get the endpoint state
   */
   state = &vrgEndpt[physId];
   if( state->isLineUsed == VRG_TRUE )
   {
      return ( EPSTATUS_ENDPT_UNKNOWN );
   }

   /* Set endpoint type */
   /* Is this endpoint a SLIC, DAA, or NoSignalling? */
   if (boardHalIsDaa(lineId))
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "EPTYPE_PSTN"));
      state->endptType = EPTYPE_PSTN;
   }
   else
   /* Is the endpoint a DECT */
   if (boardHalIsDect(lineId))
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "EPTYPE_DECT"));
      state->endptType = EPTYPE_DECT;
   }
   else
   if (boardHalIsSlic(lineId))
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "EPTYPE_FXS"));
      state->endptType = EPTYPE_FXS;
   }
   else
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "EPTYPE_NOSIG"));
      state->endptType = EPTYPE_NOSIG;
   }

   /* Init endpt archive object. */
   endptState->endptFuncs  = &gVrgEndptFuncs;
   endptState->lineId      = lineId;
   endptState->endptType   = state->endptType;
   state->endptArchive     = (ENDPT_STATE *) endptState;

   /* Register to heartbeat module */
   hbRegisterPatient( HBSTATE_INACTIVE, HBPATIENT_TYPE_HOOKSTATE,
                      lineId, &state->hookStateHBId );
   hbRegisterPatient( HBSTATE_INACTIVE, HBPATIENT_TYPE_OOSIGNAL,
                      lineId, &state->signalOnOffHBId );
   hbRegisterPatient( HBSTATE_INACTIVE, HBPATIENT_TYPE_CALLERID,
                      lineId, &state->signalCallerIdHBId );
   hbRegisterPatient( HBSTATE_INACTIVE, HBPATIENT_TYPE_SIGNAL,
                      lineId, &state->signalHBId );
   hbRegisterPatient( HBSTATE_INACTIVE, HBPATIENT_TYPE_CNXSIGNAL,
                      lineId, &state->cnxSignalHBId );


   /*
   ** Initialize endpoint instance memory
   */
   state->isLineUsed             = VRG_TRUE;
   state->lineId                 = lineId;
   state->intLinkVhdHdl          = 0 ;

   state->endptNum               = physId;
#if FLEXIBLE_CONFERENCING_SUPPORTED
   state->cnx[PRIMARY_CNX_INDEX] = 0; /* allocate when required PR13085*/
#else
   state->cnx[PRIMARY_CNX_INDEX] = &vrgCnxState[ physId ];
   vrgCnxState[ physId ].lineId  = lineId;
#endif

   /* No active brief signals */
   state->activeBriefSignal = EPSIG_NULL;

   /*
   ** Open VHDs for the endpoint
   */
   if ( InitVhds( state ) != EPSTATUS_SUCCESS )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR InitVhds() failed for endpt %d", state->endptNum ));
      return ( EPSTATUS_RESOURCES );
   }


   /*
   ** Load FXO parameters
   */
   Provision( state->endptArchive, EPPROV_MinHookFlash,        &fxoParms.minHookFlash );
   Provision( state->endptArchive, EPPROV_MaxHookFlash,        &fxoParms.maxHookFlash );
   Provision( state->endptArchive, EPPROV_PlsDlMinBreakTime,   &fxoParms.pulseMinBreak );
   Provision( state->endptArchive, EPPROV_PlsDlMaxBreakTime,   &fxoParms.pulseMaxBreak );
   Provision( state->endptArchive, EPPROV_PlsDlMinMakeTime,    &fxoParms.pulseMinMake );
   Provision( state->endptArchive, EPPROV_PlsDlMaxMakeTime,    &maxMakeMsec );
   Provision( state->endptArchive, EPPROV_PlsDlInterdigitTime, &interDigitMsec );

   /* Set fxo max make point to halfway between the provisioned values
   ** for the max make and interdigit interval */
   fxoParms.pulseMaxMake = ( maxMakeMsec + interDigitMsec ) / 2;

   /* Use country specific default value for connect/disconnect time. */
   Provision( state->endptArchive, EPPROV_MinConnect, &fxoParms.minConnect );
   Provision( state->endptArchive, EPPROV_MinDisconnect, &fxoParms.minDisconnect );

   /* Use default CAS values for wink detection */
   fxoParms.minWink        = CAS_CTL_DEFAULT_FXO_MINMAKE_MSEC;
   fxoParms.minPostWink    = CAS_CTL_DEFAULT_FXO_POSTWINK_MSEC;

   fxoParms.earlyActiveHookInterval = (fxoParms.pulseMinMake < fxoParms.pulseMinBreak) ?
                                       fxoParms.pulseMinMake : fxoParms.pulseMinBreak;
   fxoParms.delayActiveHookInterval = 100;

   /*
   ** Get the provisioned value for GR303 support (aka RFC 2833 for trunking aka Nuera support )
   */
   status = Provision( state->endptArchive, EPPROV_GR303Support, &bGR303Support );
   if ( status == EPSTATUS_SUCCESS )
   {
      state->bGR303Support = bGR303Support;
   }

   /*
   ** Get the provisioned pulse dial to DTMF digit conversion flag
   */
   status = Provision( state->endptArchive, EPPROV_Pulse2DtmfConvert, &bPulse2dtmf );
   if ( status == EPSTATUS_SUCCESS )
   {
      state->bPulse2dtmf = bPulse2dtmf;
   }

   /*
   ** If pulse2dtmf conversion is enabled we want to filter out
   ** pulse dial events and not send early onhook/offhook RTP events.
   ** This is because in this configuration we chose to inform the remote
   ** side of pulse dial events via inband DTMF and not onhook/offhook
   ** RTP events (sending both would be redundant).
   */
   if (state->bGR303Support && state->bPulse2dtmf)
   {
      fxoParms.earlyActiveHookInterval = fxoParms.minHookFlash;

#if VRG_ENDPT_CFG_USE_HFE
      /* Create Timer used in Hookflash Emulation */
      state->hookflashTimer.data = state->lineId;
      state->hookflashTimer.function = hookflashTimerCb;
      bosTimerCreate( &state->hookflashTimer );
#endif
   }

   if ( state->endptType == EPTYPE_FXS )
   {
      /* FXS/FXO convention is opposite in casCtl module */
      channelType = CAS_TYPE_FXO;
      casDriver = (XDRV_CAS *)boardHalCasGetDriver( state->endptNum );
   }
   else if ( state->endptType == EPTYPE_PSTN )
   {
      /* FXS/FXO convention is opposite in casCtl module */
      channelType = CAS_TYPE_FXS;
      casDriver = (XDRV_CAS *)boardHalDaaGetDriver( state->endptNum );
   }
   else  /* No signalling */
   {
      casDriver = NULL;
      channelType = CAS_TYPE_INACTIVE;
      state->offHook = VRG_FALSE;
   }

   /*
   ** Enable CAS
   */
   if ( casCtlEndptInit( channelType, casDriver, &state->casCtlHandle, &fxoParms, NULL ) != CAS_CTL_SUCCESS )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR casCtlEndptInit for endpt %d", state->endptNum ));
      return ( EPSTATUS_RESOURCES );
   }

      if ( state->endptType == EPTYPE_FXS )
      {
         status = SetRingParams(state);
         if (status != EPSTATUS_SUCCESS)
         {
            return ( status );
         }

         slicDriver = boardHalSlicGetDriver( state->endptNum );
         if( !slicDriver )
         {
            return ( EPSTATUS_RESOURCES );
         }

         /* Set the provisioned boosted loop current option if applicable */
         status = Provision( state->endptArchive, EPPROV_BoostedLoopCurrent, &provItem );
         if ( status == EPSTATUS_SUCCESS )
         {
            /* Translate the provisioned value into a boolean. */
            if(provItem == 1)
            {
                 casCtlSendCommand(CAS_REQ_SLIC_BOOTED_LOOP_CURRENT, CAS_CMD_MODE_ASYNC, (VRG_UINT32)slicDriver, VRG_TRUE);
            }
            else if(provItem  == 2)
            {
                 casCtlSendCommand(CAS_REQ_SLIC_BOOTED_LOOP_CURRENT, CAS_CMD_MODE_ASYNC, (VRG_UINT32)slicDriver, VRG_FALSE);
            }
         }
      }

   /* Get the provisioned analog loopback value */
   status = Provision( state->endptArchive, EPPROV_AnalogLoopback, &enableAnalogLoopback);
   if ( status == EPSTATUS_SUCCESS )
   {
      /* Translate the provisioned value into a boolean. */
      if(enableAnalogLoopback == 1)
      {
         /* set echo cancellation to off in analog loopback mode */
         EcanStateSet(state, ECAN_OFF);
         state->testMode = TESTMODE_ANALOGLOOPBACK;
         boardHalSetAnalogLoopback( state->lineId, VRG_TRUE );
      }
      else if(enableAnalogLoopback == 2)
      {
         state->testMode = TESTMODE_NONE;
         boardHalSetAnalogLoopback( state->lineId, VRG_FALSE );
      }
   }

   /*
   ** Initialize the class transmitter state machine variables
   */
   if ( state->endptType == EPTYPE_FXS || state->endptType == EPTYPE_DECT )
   {
      classStmInit( &state->classState,
                    state->casCtlHandle,
                    state->lineVhdHdl,
                    endptConfig.country );
   }

   if( state->endptType == EPTYPE_PSTN )
   {
      if( state->pstnVhdHdl != 0xFF )
      {
         /* Initialize PSTN dialing for FXS endpoint */

         if ( pstnCtlEndptInit( state->pstnVhdHdl ) != PSTN_CTL_SUCCESS )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR pstnCtlEndptInit for endpt %d",
                      state->endptNum ));
            return ( EPSTATUS_RESOURCES );
         }
         else
         {
            /* Check if dtmf clidrx is required */
            EPCLIDTYPE clidType;
            Provision( state->endptArchive, EPPROV_CIDSigProtocol, &clidType );

            /* Turn on DTMFCLIDRX module based on clidrx setting */
            if ( clidType == EPCLIDTYPE_DTMF )
            {
               PSTN_CTL_DTMF_CLID_PARMS dtmfParmsp;
               Provision(state->endptArchive, EPPROV_CIDDtmfParms, &dtmfParmsp);

               if ( pstnCtlDTMFClidRxInit( state->country, state->pstnVhdHdl,
                                           &dtmfParmsp ) != PSTN_CTL_SUCCESS )
               {
                  VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR pstnCtlDTMFClidRxInit for endpt %d",
                            state->endptNum ));
                  return ( EPSTATUS_RESOURCES );
               }
               /* Disable FSK clid rx */
               enableFskClidRx(VRG_FALSE, state->pstnVhdHdl);
            }
            else
            {
               /* Enable FSK clid rx */
               enableFskClidRx(VRG_TRUE, state->pstnVhdHdl);
            }
         }
      }
   }

#ifdef HDSP_CFG_CLIDRX
   if ( state->endptType == EPTYPE_PSTN )
   {
      /* On startup, configure the caller id RX service for on-hook (type 1) caller id detection */
      if ( HAPISUCCESS != classRxConfigure( state->pstnVhdHdl, CLASS_MSG_ONHOOK_CLID, endptConfig.country ) )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "classRxConfigure TYPE1 FAIL vhd=0x%x!!!", state->pstnVhdHdl ));
      }
   }
#endif

   /* If this is the PSTN endpoint and if we are supporting a type of CLID Rx
   ** that comes before the first ring (DTMF CLID Rx, DTAS, etc.), then we need
   ** to preset the DAA in on-hook data receive mode to allow CLID Rx to work
   ** from the creation of the endpoint. For example, since DTMF CLID Rx
   ** usually comes before the first ring, the CAS state machine would not catch it.
   ** This is based on the current assumption (valid now) that once the DAA is
   ** set in on-hook data mode once it does not need to be reset between each call.
   ** If the reset is needed, then we would need to do this after each call. */
   if ( state->endptType == EPTYPE_PSTN )
   {
        casCtlSendCommand(CAS_REQ_DAA_MODE_CONTROL, CAS_CMD_MODE_ASYNC, (VRG_UINT32)boardHalDaaGetDriver( state->endptNum ),
                          XDRV_DAA_MODE_ONHOOK_DATA);
   }

   /*
   ** Startup complete
   */
   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED successful initialization for endpt %d", state->endptNum ));

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   ProcessVrgEndptDestroy
**
** PURPOSE:    This function is used to de-allocate a VRG endpoint object.
**
** PARAMETERS: endptState  (mod) Created endpt object.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS ProcessVrgEndptDestroy( VRG_ENDPT_STATE *vrgEndptState )
{
   VRG_ENDPT    *state;
   int i;

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "ENDPT DESTROY: endpt %d ", vrgEndptState->lineId ));

   state = GetEndptState( vrgEndptState->lineId );
   if (state != NULL)
   {
      hbUnregisterPatient( state->cnxSignalHBId );
      hbUnregisterPatient( state->signalHBId );
      hbUnregisterPatient( state->signalCallerIdHBId );
      hbUnregisterPatient( state->signalOnOffHBId );
      hbUnregisterPatient( state->hookStateHBId );

      /* Clear the connection queue associated with the endpoint */
      for(i = 0; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++)
      {
         if ( state->cnx[i] != NULL )
         {
            state->cnx[i]->cnxId = CNX_UNUSED;
            state->cnx[i] = NULL;
         }
      }
      /* clear Switchboard connetion mask in each cnx vhd */
      for ( i = 0; i < VRG_ENDPT_CFG_NUM_CNX; i++ )
      {
         if ( vrgCnxState[i].vhdhdl )
         {
            /* clear inMask and outMask */
            vrgCnxState[i].inMask &= ~(1 << state->endptNum);
            vrgCnxState[i].outMask &= ~(1 << state->endptNum);
         }
      }
   }
   else
   {
      return ( EPSTATUS_ENDPT_UNKNOWN );
   }

   /* Shutdown the CAS module */
   if ( endptConfig.initState & EPEINIT_CAS )
   {
      if ( casCtlEndptDeinit(state->casCtlHandle) != CAS_CTL_SUCCESS )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "casCtlDeinit FAILED" ));
         return ( EPSTATUS_SHUTDOWN_ERR );
      }
   }

#if VRG_ENDPT_CFG_USE_HFE
   /* Remove Timer from List */
   bosTimerDestroy( &state->hookflashTimer );
#endif

   state->isLineUsed = VRG_FALSE;
   if ( state->lineVhdHdl != 0xffff )
   {

      /* check if the endpoint we're destroying is pstn and has DTMF Clid */
      if( state->endptType == EPTYPE_PSTN && state->pstnVhdHdl != 0xFF )
      {
         /* Check if dtmf clidrx is required */
         EPCLIDTYPE clidType;
         Provision( state->endptArchive, EPPROV_CIDSigProtocol, &clidType );

         if ( clidType == EPCLIDTYPE_DTMF )
         {
            /* Deinit pstnCtlDTMFClid */
            pstnCtlDTMFClidRxDestroy( state->pstnVhdHdl );
         }
      }

      /* Close ECAN line VHD */
      if ( hdspVhdClose( state->lineVhdHdl, state->dspNum ) != HAPISUCCESS )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "hdspVhdClose FAILED for dsp %d, endpt %d, line-vhd 0x%x",
                   state->dspNum, state->endptNum, state->lineVhdHdl ));
      }
   }

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED ENDPT DESTROY: endpt %d ", vrgEndptState->lineId ));

   return ( EPSTATUS_SUCCESS );
}

#if defined( NTR_SUPPORT )
/*****************************************************************************
*  FUNCTION:   ProcessVrgEndptNtrCmd
*
*  PURPOSE:    Update the PCM feedback divider registers based upon NTR preferences
*
*  PARAMETERS: ntrParm structure filled in with the new Ndiv_frac value for the
*              PCM feedback control register
*
*  RETURNS:    ntrParm->status = EPSTATUS_SUCCESS if PCM register write is success
*              EPSTATUS_NOTREADY otherwise
*
*****************************************************************************/
static void ProcessVrgEndptNtrCmd
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
#if ( NTR_DEBUG_LOG )
   VRG_LOG_DBG((VRG_LOD_MOD_EPT, ("%s: pcmStepsAdjust=[%08li]", __FUNCTION__, pcmStepsAdjust)));
   VRG_LOG_DBG((VRG_LOD_MOD_EPT, "%s::isEnbl==[%hu]", __FUNCTION__, isEnbl));
#endif /* NTR_DEBUG_LOG */

   VRG_SINT32  newRegVal = pcmStepsAdjust;

   if (ntrAction == 0)
      boardHalNtrSetFeedback( newRegVal, isEnbl );
   else if (ntrAction == 1)
      boardHalNtrGetCounts( localCount, ntrCount );
   else if (ntrAction == 2)
      boardHalNtrGetNdiv( ndivInt, ndivFrac );
   else /* (ntrAction == 3) */
      boardHalNtrPrint( isEnbl, newRegVal );

#if ( NTR_DEBUG_LOG )
   VRG_LOG_DBG((VRG_LOD_MOD_EPT, "%s::isEnbl==[%hu]", __FUNCTION__, isEnbl));
   VRG_LOG_DBG((VRG_LOD_MOD_EPT, "%s::(*localCount)==[%lu],(*ntrCount)==[%lu]", __FUNCTION__, (*localCount), (*ntrCount)));
   VRG_LOG_DBG((VRG_LOD_MOD_EPT, "%s::(*ndivInt)==[%lu],(*ndivFrac)==[%lu]", __FUNCTION__, (*ndivInt), (*ndivFrac)));
#endif /* NTR_DEBUG_LOG */
   return ;
}
#endif /* NTR_SUPPORT */

/*****************************************************************************
*  FUNCTION:   ProcessVrgEndptSignal
*
*  PURPOSE:    Generate a signal on the endpoint (or connection)
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
EPSTATUS ProcessVrgEndptSignal
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
   VRG_ENDPT              *state;
   CNXSTATE               *cnx;
   int                     i;
   VRG_UINT32              callWtgDelay = 0;
   VRG_TONE_SPEC           vrgTone;
   int                     lineId = ((VRG_ENDPT_STATE *) endptState)->lineId;
   EPSIG                   briefSignal = EPSIG_NULL;
   XDRV_SLIC              *slicDriver;
   
   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "SIGNAL: ept:%d cnx:%d s:%d v:%u d:%d p:%d r:%d ", lineId, cnxId, signal, value, duration, period, repetition ));

   /*
   ** Get the endpoint state
   */
   state = GetEndptState( lineId );
   if( ! state )
   {
      return(EPSTATUS_ENDPT_UNKNOWN);
   }


   /*
   ** Get the cnx state
   */
   if( cnxId != -1 )
   {
      cnx = GetCnxState( cnxId );
      if( ! cnx )
      {
         return(EPSTATUS_CNX_UNKNOWN);
      }
      else if (signal == EPSIG_SB_DISCONNECT )
      {
         for( i=0; i<VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
         {
            if ( state->cnx[i] && (state->cnx[i]->cnxId == cnxId) )
            {
               if (value)
               {
                  cnx->bSBDisconnect = VRG_TRUE;
                  UpdateSBMasks( cnx, cnx->mode );
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Disconnect switchboard for ept %d cnx %d", lineId, cnxId ));
               }
               else
               {
                  cnx->bSBDisconnect = VRG_FALSE;
                  UpdateSBMasks( cnx, cnx->mode );
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Revert switchboard connection for ept %d cnx %d", lineId, cnxId ));
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED SIGNAL %d", signal ));
               return (EPSTATUS_SUCCESS);
            }
         }
         return(EPSTATUS_CNX_UNKNOWN);
      }
      else if (signal == EPSIG_LINE_MODE_CONTROL )
      {
         for( i=0; i<VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
         {
            if ( state->cnx[i] && (state->cnx[i]->cnxId == cnxId) )
            {
               if (value)
               {
                  cnx->cnxModeControlsLineVhd = VRG_TRUE;
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Subsequent connection mode will apply to a line VHD" ));
               }
               else
               {
                  cnx->cnxModeControlsLineVhd = VRG_FALSE;
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Subsequent connection mode will NOT apply to a line VHD" ));
               }
               VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED SIGNAL %d", signal ));
               return (EPSTATUS_SUCCESS);
            }
         }
         return(EPSTATUS_CNX_UNKNOWN);
      }
      else if (signal == EPSIG_INGRESS_DTMF)
      {
         for( i=0; i<VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
         {
            if ( state->cnx[i] && (state->cnx[i]->cnxId == cnxId) &&
                (state->cnx[i]->mode != EPCNXMODE_INACT) && (state->cnx[i]->mode != EPCNXMODE_RCVO) )
            {
               VRG_UINT32 dbLevel, pulseTime, skew;

               if ( value < HAPI_PTE_DTMFDMAXDGT )
               {
                  /* Let the heartbeat module know this endpt now has a cnx signal being applied */
                  hbUpdatePatientState( state->cnxSignalHBId, HBSTATE_ACTIVE );

                  /* Get the country specific DTMF settings */
                  Provision( state->endptArchive, EPPROV_DtmfDbLevel, &dbLevel );
                  Provision( state->endptArchive, EPPROV_DtmfPulseTime, &pulseTime );
                  Provision( state->endptArchive, EPPROV_DTMFSkew, &skew );

                  /* Generate ingress signal */
                  hdspDigitOn( state->lineVhdHdl, state->lineId, value, dbLevel, skew, pulseTime, INGRESS_TONE );

                  /* Since DTMF ingress tones are brief signals, we will wait for
                  ** HAPI_PTE_TONE_GEN_EVT to determine when it ends.  Since these signals
                  ** are only generated when there is a connection, we will rely on the
                  ** cnx state hb patient to determine the overall device activity state. */
               }
               VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED SIGNAL %d", signal ));
               return (EPSTATUS_SUCCESS);
            }
         }
         return(EPSTATUS_CNX_UNKNOWN);
      }

      /* iterate through the list of cnx for the endpoint to determine the vhdhdl */
      for( i=0; i<VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
      {
         if ( state->cnx[i] && (state->cnx[i]->cnxId == cnxId) )
         {
            const EPZTONEMAP *toneMap = getEpzToneMapEntry(signal);
            cnx->activeSignal = value;

            if ( toneMap->event == EPSIG_NULL )
            {
               /* Could not find the mapping for this tone! */
               return(EPSTATUS_SIGNAL_UNKNOWN);
            }

            switch( value )
            {
               case EPSIG_DIRECTION_EGRESS_ON:
               {
                  applyEgressTone(state, endptConfig.country, toneMap, duration);
                  break;
               }
               case EPSIG_DIRECTION_INGRESS_ON:
               {
                  applyIngressTone(state, i, endptConfig.country, toneMap);
                  break;
               }
               case EPSIG_DIRECTION_BOTH_ON:
               {
                  applyEgressTone(state, endptConfig.country, toneMap, duration);
                  applyIngressTone(state, i, endptConfig.country, toneMap);
                  break;
               }
               case EPSIG_DIRECTION_EGRESS_OFF:
               {
                  removeEgressTone(state, signal);
                  break;
               }
               case EPSIG_DIRECTION_INGRESS_OFF:
               {
                  removeIngressTone(state, i);
                  break;
               }
               case EPSIG_DIRECTION_BOTH_OFF:
               {
                  removeEgressTone(state, signal);
                  removeIngressTone(state, i);
                  break;
               }
               default:
               {
                  /* Revert to legacy behavior */
                  if (value % 2)
                  {
                     applyIngressTone(state, i, endptConfig.country, toneMap);
                  }
                  else
                  {
                     removeIngressTone(state, i);
                  }
               }
            }

            VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED SIGNAL %d", signal ));
            return(EPSTATUS_SUCCESS);
         }
      }

      /* couldn't find a connection ID match */
      return(EPSTATUS_CNX_UNKNOWN);
   }

   /*
   ** Process event
   */
   switch ( signal )
   {

      case EPSIG_SAS_CAS:
      case EPSIG_SAS_CAS1:
      case EPSIG_SAS_CAS2:
      case EPSIG_SAS_CAS3:
      case EPSIG_SAS_CAS4:
      case EPSIG_CALLWT:
      case EPSIG_CALLW1:
      case EPSIG_CALLW2:
      case EPSIG_CALLW3:
      case EPSIG_CALLW4:
      {
         if ( value != 0 )
         {
            const EPZTONEMAP *toneMap = getEpzToneMapEntry(signal);
            if ( toneMap->event == EPSIG_NULL )
            {
               break;
            }

            Provision( state->endptArchive,EPPROV_CallWtgDelay,&callWtgDelay );

            /* Let the heartbeat module know this endpt now has a signal being applied */
            hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

            /* Reset Off-hook state machine */
            classStmReset( &state->classState, CLASSSTM_TYPE_OFFHOOK_CLID );

            /* Get the provisioned SAS tone. Fill in the defaults if nothing to provision. */
            if (( toneMap->provItem == - 1) ||
                ( EPSTATUS_SUCCESS != Provision( state->endptArchive,
                                                 toneMap->provItem,
                                                 &vrgTone )))
            {
               /* there was nothing to provision */
               VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "The SAS tone cannot be found in provTable !!!" ));
            }

            state->classState.callWtgInfo.sasCustomProfile = (HDSP_TONE_PROV_PATTERN)vrgTone;
            state->classState.callWtgInfo.delay   = (VRG_UINT16) callWtgDelay;
            state->classState.callWtgInfo.profile = CT_CALLW_SAS_CAS;
            Provision(state->endptArchive, EPPROV_CIDFskParms, &fskParmsPlaceHolder[lineId]);
            state->classState.fskParmp = &fskParmsPlaceHolder[lineId];
            Provision(state->endptArchive, EPPROV_CIDDtmfParms, &dtmfParmsPlaceHolder[lineId]);
            state->classState.dtmfParmp = &dtmfParmsPlaceHolder[lineId];

            if ( EPSIG_IS_SAS_CAS( signal ) )
            {
               /* Call-waiting with CLID - trigger CAS tone */
               classStmEventProcess( &state->classState, CLASSSTM_CLID_SAS_CAS, state->lineId );
            }
            else
            {
               /* Normal call-waiting without CLID doesn't use CAS tone */
               classStmEventProcess( &state->classState, CLASSSTM_CLID_SAS_ONLY, state->lineId );
            }
         }
         else
         {
            /* In case we were inactive and we get a request to turn a signal off,
            ** we better let the heartbeat module know we need to do some processing. */
            hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

            classStmEventProcess( &state->classState, CLASSSTM_CLID_SAS_CAS_OFF, state->lineId );

            if ( EPSIG_IS_SAS_CAS( signal ) )
            {
               if ( state->activeBriefSignal == EPSIG_CALLID )
               {
                  /* Let the heartbeat module know this endpt no longer has an active
                  ** caller ID signal */
                  hbUpdatePatientState( state->signalCallerIdHBId, HBSTATE_INACTIVE );
               }
            }

            /* Let the heartbeat module know this endpt no longer has an active
            ** endpt signal */
            hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
         }
      }
      break;

      /* Call progress tones */
      case EPSIG_BUSY:
      case EPSIG_CONF:
      case EPSIG_MSGW:
      case EPSIG_OHWARN:
      case EPSIG_REORDER:
      case EPSIG_DIAL:
      case EPSIG_RINGBACK:
      case EPSIG_STUTTER:
      case EPSIG_INTRUSION:
      case EPSIG_CALLCARD:
      case EPSIG_NETBUSY:
      case EPSIG_NETBUSY1:
      case EPSIG_RINGBACK_CUST1:
      case EPSIG_SPECIAL_DIAL:
      case EPSIG_DISCONNECT:
      case EPSIG_PAYPHONE_REC:
      case EPSIG_WARNING:
      {
         if( value )
         {
            const EPZTONEMAP *toneMap = getEpzToneMapEntry(signal);
            if ( toneMap->event == EPSIG_NULL )
            {
               /* Could not find the mapping for this tone! */
               return(EPSTATUS_SIGNAL_UNKNOWN);
            }

            applyEgressTone(state, endptConfig.country, toneMap, duration);

            if ( signal == EPSIG_CONF )
            {
               /* Save active brief signal */
               briefSignal = signal;
            }

            if (signal == EPSIG_DIAL || signal == EPSIG_SPECIAL_DIAL)
            {
#if VRG_ENDPT_CFG_USE_TPD
               /* Signal the TPD task of the dialtone event */
               tpdCtlSignalDialtone( lineId );
#endif
               /* Process Ecan state machine */
               EcanProcessStm( state, ECANEVT_DIALTONE_ON );
            }
         }
         else
         {
            removeEgressTone(state, signal);
         }
      }
      break;

      case EPSIG_LOOP_CLOSE:
      case EPSIG_LOOP_OPEN:
      case EPSIG_NO_BATTERY:
      case EPSIG_NORMAL_POLARITY:
      case EPSIG_REDUCED_BATTERY:
      case EPSIG_REVERSE_POLARITY:
      case EPSIG_INIT_RING:
      case EPSIG_ENABLE_METERING:
      {
         if ( value )
         {
            state->signalRep = repetition;

            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "vrgEndptSignal: Pulsed signal with pulse duration of %i msec,"
                      "period = %i, repetition = %i", duration, period, repetition));

            (*endptConfig.notify)( state->endptArchive, -1, signal, NULL, 0, -1 );
         }
      }
      break;

      case EPSIG_METERING_BURST:
      {
         if( value )
         {
#if VRG_ENDPT_CFG_USE_APM_PULSE_METERING
            CAS_CTL_EXT_CMD_PARM parms;
#else
            HAPI_RM_PULSE_SIGNAL_PARMS meterPulse;
#endif
            EPPULSESIGFREQUENCY frequency = PULSE_SIGNAL_FREQ_16KHZ;

            Provision( state->endptArchive, EPPROV_MeteringFrequency, &frequency );
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT,
                           "Pulsed signal: duration %d msec, period %d, repetition %d, frequency %d",
                           duration, period, repetition, frequency ));

#if VRG_ENDPT_CFG_USE_APM_PULSE_METERING
            /* Retrieve Pointer to APM Driver Structure */
            parms.drvHdl       = (void*)boardHalApmGetDriver(state->dspNum);
            parms.arguments[0] = state->endptNum;
            parms.arguments[1] = duration;
            parms.arguments[2] = period;
            parms.arguments[3] = repetition;
            parms.arguments[4] = 1;
            parms.arguments[5] = frequency;

            /* Generate pulse metering signal via the APM block */
            casCtlSendExtendedCommand( CAS_REQ_APM_PULSE_SIGNAL, CAS_CMD_MODE_ASYNC, &parms );

#else
            /* Save pulse signal parameters for DSP configuration */
            meterPulse.pulseLineHdl = state->endptNum;
            meterPulse.pulseDuration = duration;
            meterPulse.pulsePeriod = period;

            /* Pulse count = number of times that the signal is played (1 + repetition) */
            meterPulse.pulseCount = repetition;
            meterPulse.pulseAmplitude = 1;
            meterPulse.pulseFrequency = frequency;

            /* Configure pulse signal parameters at the DSP */
            hdspSendCmdData( ((state->dspNum << 8) | HAPIRESMAN),
                             HAPI_RM_CFG_PULSE_SIGNAL_CMD,
                             HSCMDEXTDATA_SYNC,
                             sizeof( meterPulse ),
                             &meterPulse );

#endif
         }
      }
      break;

      case EPSIG_STEADY_FULL_BATTERY:
      case EPSIG_STEADY_LOOP_CLOSE:
      case EPSIG_STEADY_LOOP_OPEN:
      case EPSIG_STEADY_NO_BATTERY:
      case EPSIG_STEADY_NORMAL_POLARITY:
      case EPSIG_STEADY_REDUCED_BATTERY:
      case EPSIG_STEADY_REVERSED_POLARITY:
      {
         /* TODO: need to implement steady signals */
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, " vrgEndptSignal: Steady signal" ));
      }
      break;

       /* DTMF tones */
      case EPSIG_DTMF0:
      case EPSIG_DTMF1:
      case EPSIG_DTMF2:
      case EPSIG_DTMF3:
      case EPSIG_DTMF4:
      case EPSIG_DTMF5:
      case EPSIG_DTMF6:
      case EPSIG_DTMF7:
      case EPSIG_DTMF8:
      case EPSIG_DTMF9:
      case EPSIG_DTMFS:
      case EPSIG_DTMFH:
      case EPSIG_DTMFA:
      case EPSIG_DTMFB:
      case EPSIG_DTMFC:
      case EPSIG_DTMFD:
      {
         VRG_UINT32 dbLevel, pulseTime, skew;

         if( value )
         {
            const EPZSIG2PTEDIGITMAP *dtmfMap = epSig2PteDigitMap;

            /* Map from EPEVT to CUSTOM_TONE */
            while(( dtmfMap->signal != EPSIG_NULL ) &&
                  ( dtmfMap->signal != signal ))
            {
               dtmfMap++;
            }

            if( dtmfMap->signal != EPSIG_NULL )
            {
               /* Let the heartbeat module know this endpt now has a signal being applied */
               hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

               /* Get the country specific DTMF settings */
               Provision( state->endptArchive, EPPROV_DtmfDbLevel, &dbLevel );
               Provision( state->endptArchive, EPPROV_DtmfPulseTime, &pulseTime );
               Provision( state->endptArchive, EPPROV_DTMFSkew, &skew );

               /* Generate egress signal */
               hdspDigitOn( state->lineVhdHdl, state->lineId, dtmfMap->digit, dbLevel, skew, pulseTime, EGRESS_TONE );

               /* Save active brief signal */
               briefSignal = signal;
            }
         }
         else
         {
            /* In case we were inactive and we get a request to turn a signal off,
            ** we better let the heartbeat module know we need to do some processing. */
            hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

            hdspDigitOff( state->lineVhdHdl );

            /* Let the heartbeat module know this endpt no longer has an active
            ** endpt signal */
            hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
         }
      }
      break;

      /* Ringer control */
      case EPSIG_CALLID_RING0:
      case EPSIG_CALLID_RING1:
      case EPSIG_CALLID_RING2:
      case EPSIG_CALLID_RING3:
      case EPSIG_CALLID_RING4:
      case EPSIG_CALLID_RING5:
      case EPSIG_CALLID_RING6:
      case EPSIG_CALLID_RING7:
      case EPSIG_CALLID_RINGING:
      /*If it is DECT endpt, we pass through to the EPSIG_RINGING to start ringing.
      There is no need to start a CLASS STM to send CLID to DECT module because
      DECT can receive CLID at anytime. CLID will be sent when EPSIG_CLID signal
      is received.*/
      if (state->endptType != EPTYPE_DECT)
      {
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, " vrgEndptSignal: EPSIG_CALLID_RING0 - RINGING, actual signal = %d, value = %d", signal, value ));
         if ( value )
         {
            int classStmType;
            CLASSSTM_RING_STATE pattern;

            /* Let the heartbeat module know this endpt now has a signal being applied */
            hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );
            Provision( state->endptArchive, EPPROV_CIDMode, &classStmType );

            GetRingPattern( signal, endptConfig.country, &pattern );

            classStmReset( &state->classState, classStmType );

            /* Set Ring Pattern */
            state->classState.ringInfo = pattern;

            /* Set Caller Id Parameters */
            Provision( state->endptArchive, EPPROV_CIDFskAfterRing, &state->classState.fskAfterRing );
            Provision( state->endptArchive, EPPROV_CIDRingAfterFSK, &state->classState.ringAfterFsk );
            Provision( state->endptArchive, EPPROV_CIDFskAfterDTAS, &state->classState.fskAfterDtas );
            Provision( state->endptArchive, EPPROV_CIDFskAfterRPAS, &state->classState.fskAfterRp );
            Provision( state->endptArchive, EPPROV_CIDDTASAfterLR, &state->classState.dtasAfterLr );
            Provision( state->endptArchive, EPPROV_CIDSigProtocol, &state->classState.clidProtocol );
            Provision( state->endptArchive, EPPROV_CIDFskParms, &fskParmsPlaceHolder[lineId]);
            state->classState.fskParmp = &fskParmsPlaceHolder[lineId];
            Provision( state->endptArchive, EPPROV_CIDDtmfParms, &dtmfParmsPlaceHolder[lineId]);
            state->classState.dtmfParmp = &dtmfParmsPlaceHolder[lineId];


         }
         else
         {
            /* In case we were inactive and we get a request to turn a signal off,
            ** we better let the heartbeat module know we need to do some processing. */
            hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

            if ( !state->offHook )
            {
               /* queue up the stop ringing command for the state machine */
               classStmEventProcess( &state->classState, CLASSSTM_RING_OFF, state->lineId );
            }

            /* Let the heartbeat module know this endpt no longer has an
            ** endpt signal */
            hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );

            /* Let the heartbeat module know this endpt no longer has an active
            ** caller ID signal */
            if ( state->activeBriefSignal == EPSIG_CALLID )
            {
               hbUpdatePatientState( state->signalCallerIdHBId, HBSTATE_INACTIVE );
            }
         }
      break;
      }

      case EPSIG_RING0:
      case EPSIG_RING1:
      case EPSIG_RING2:
      case EPSIG_RING3:
      case EPSIG_RING4:
      case EPSIG_RING5:
      case EPSIG_RING6:
      case EPSIG_RING7:
      case EPSIG_RINGING:
      case EPSIG_SPLASH:
      {
         if( value )
         {
            CLASSSTM_RING_STATE  pattern;
            /* Always try to apply ring interleave if we support it. */
            VRG_BOOL ringInterleave = VRG_TRUE;

            /* Let the heartbeat module know this endpt now has a signal being applied */
            hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

            GetRingPattern( signal, endptConfig.country, &pattern );

            /* Disable ring interleave for test purposes only. */
            if( (signal == EPSIG_RINGING) && (duration == VRG_ENDPT_NO_RING_INTERLEAVE))
            {
               ringInterleave = VRG_FALSE;
            }

            casCtlRingProfileOn( state->casCtlHandle, &pattern, ringInterleave );

            if ( signal == EPSIG_SPLASH )
            {
               /* save active brief signal */
               briefSignal = signal;
            }
         }
         else
         {
            /* In case we were inactive and we get a request to turn a signal off,
            ** we better let the heartbeat module know we need to do some processing. */
            hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

            if( ! state->offHook )
            {
               casCtlRingOff( state->casCtlHandle, VRG_FALSE );
            }

            /* Let the heartbeat module know this endpt no longer has an active
            ** endpt signal */
            hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
         }
      }
      break;

      /* Caller ID message */
      case EPSIG_CALLID:
      {
         int            event;

         /* Let the heartbeat module know this endpt now has a signal being applied */
         hbUpdatePatientState( state->signalCallerIdHBId, HBSTATE_ACTIVE );

         if ( !state->offHook )
         {
            event = CLASSSTM_CLID_RING;
         }
         else
         {
            event = CLASSSTM_CLID_INFO;
         }

         hdspClassParseStr( (char *) value,
                            endptConfig.country,
                            &state->classState.clidInfo );

         if (state->endptType == EPTYPE_DECT)
         {
            /* For call waiting case, we need to change the state of CLASS STM
            for generating SAS CAS tones. No caller id will be sent through DSP, because
            the handset will not ACK the SAS tone.*/
            if (event == CLASSSTM_CLID_INFO)
            {
               classStmEventProcess( &state->classState, event, state->lineId );
            }

            /* CLID is sent by DECT module. */
         }
         else
         {
            Provision( state->endptArchive, EPPROV_CIDFskParms, &fskParmsPlaceHolder[lineId]);
            state->classState.fskParmp = &fskParmsPlaceHolder[lineId];
            Provision( state->endptArchive, EPPROV_CIDDtmfParms, &dtmfParmsPlaceHolder[lineId]);
            state->classState.dtmfParmp = &dtmfParmsPlaceHolder[lineId];
            classStmEventProcess( &state->classState, event, state->lineId );
         }
         /* save active brief signal */
         briefSignal = signal;


      }
      break;

      /* visual message waiting indicator */
      case EPSIG_VMWI:
      {
         int vmwiType;

         /* Let the heartbeat module know this endpt has an active on/off
         ** signal */
         hbUpdatePatientState( state->signalOnOffHBId, HBSTATE_ACTIVE );

         Provision( state->endptArchive, EPPROV_VMWIMode, &vmwiType );

         classStmReset( &state->classState, vmwiType );


         /* Get provisioned VMWI parameters. */
         Provision( state->endptArchive, EPPROV_VMWIFskAfterDTAS,   &state->classState.fskAfterDtas );
         Provision( state->endptArchive, EPPROV_VMWIFskAfterRPAS,   &state->classState.fskAfterRp );
         Provision( state->endptArchive, EPPROV_VMWIDTASAfterLR,    &state->classState.dtasAfterLr );
         Provision( state->endptArchive, EPPROV_CIDFskParms, &fskParmsPlaceHolder[lineId]);
         state->classState.fskParmp = &fskParmsPlaceHolder[lineId];
         Provision( state->endptArchive, EPPROV_CIDDtmfParms, &dtmfParmsPlaceHolder[lineId] );
         state->classState.dtmfParmp = &dtmfParmsPlaceHolder[lineId];
         Provision( state->endptArchive, EPPROV_VMWIMsgFormat, &state->classState.vmwiMsgFormat);
         Provision( state->endptArchive, EPPROV_VMWISigProtocol, &state->classState.vmwiProtocol );


         if ( value == 0 )
         {
            state->classState.vmwiType = CLASS_VMWI_DEACTIVATE;
            classStmEventProcess( &state->classState, CLASSSTM_VMWI_OFF, state->lineId );

            /* Let the heartbeat module know this endpt no longer has an active
            ** on/off signal */
            hbUpdatePatientState( state->signalOnOffHBId, HBSTATE_INACTIVE );
         }
         else
         {
            state->classState.vmwiType = CLASS_VMWI_ACTIVATE;
            classStmEventProcess( &state->classState, CLASSSTM_VMWI_ON, state->lineId );
         }
      }
      break;

      /* phase reversal */
      case EPSIG_REV:
      {
         /* Get the SLIC driver for this endpoint */
         slicDriver = boardHalSlicGetDriver( state->endptNum );

         if( slicDriver != NULL )
         {
         /* Signal the SLIC of the phase reversal signal request */
         casCtlIgnoreHookState( state->casCtlHandle, 200 );
         casCtlSendCommand(CAS_REQ_SLIC_PHASE_REVERSAL_CONTROL, CAS_CMD_MODE_ASYNC, (VRG_UINT32)slicDriver, value);
         }
      }
      break;

      /* Open switch interval */
      case EPSIG_OSI:
      {
         /* Let the heartbeat module know this endpt now has a signal being applied */
         hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

         /*
         ** Send supervisory disconnect / Open Switch Interval
         ** Bellcore SR-TSV-002476 pg 9 No DC between tip and ring
         */
         if ( value )
         {
            /* The OSI duration will be provided in the 'value' parameter */
            casCtlSupervisoryDisconnect( state->casCtlHandle, (VRG_UINT16) value, CAS_OSI_DEFAULT );
         }
         else
         {
            /* Let the heartbeat module know this endpt no longer has an active
            ** endpt signal */
            hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
         }
      }
      break;

      /* Analog loopback */
      case EPSIG_ANALOG_LOOPBACK:
      {
         requestLoopback(state, value);

         /* preserve previous active brief signal, we want to detect signal ending event */
         briefSignal=state->activeBriefSignal;
      }
      break;

      /* Line reset.
      ** Note: heartbeat is being processed inside TPD. There must be no line/DSP
      ** processing in here (outside tpdCtl module). */
      case EPSIG_LINE_RESET:
      {
#if VRG_ENDPT_CFG_USE_TPD
         /* Signal the TPD task to start processing. Ignore active state, and use provisioning */
         tpdCtlProcessSignal( TPD_CTL_SIGNAL_LINE_RESET, lineId, VRG_FALSE, -1 );
#endif
      }
      break;

      /* Diag tests start.
      ** Note: heartbeat is being processed inside TPD. There must be no line/DSP
      ** processing in here (outside tpdCtl module). */
      case EPSIG_DIAGTESTS_PROV_START:
      case EPSIG_DIAGTESTS_NOPROV_START:
      case EPSIG_DIAGTESTS_PC15_START:
      case EPSIG_DIAGTESTS_CALIBRATE_START:
      {
#if VRG_ENDPT_CFG_USE_TPD
         requestTpd(state, signal, value);
#else
         /* Ignore */
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Ignoring DIAGTESTS START request" ));
#endif
      }
      break;

      /* Update DTMF playout value  */
      case EPSIG_UPDATE_DTMFPLAYOUT:
      {
         /* Provisioned DTMF playout value */
         int dtmfPlayout;
         int result;
         int status;

         /* Let the heartbeat module know this endpt now has a signal being applied */
         hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

         status = Provision( state->endptArchive, EPPROV_DTMFPlayout, &dtmfPlayout );
         if ( status == EPSTATUS_SUCCESS )
         {
            result = hdspSendCmd( state->cnx[PRIMARY_CNX_INDEX]->vhdhdl,
                                  HAPINET_PTE_SETREG1,
                                  HSCMDDATA,
                                  VRG_OFFSETOF( HAPI_PTE_REGS, minPulseOn ),
                                  ( dtmfPlayout << 3 ));

            if ( result != HAPISUCCESS )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting PTE registers for vhd 0x%x", state->lineVhdHdl ));
            }
         }

         /* Let the heartbeat module know this endpt no longer has an
         ** endpt signal */
         hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
      }
      break;

      case EPSIG_PSTNCALLSETUP:
      {
         VRG_ENDPT* pstnState;

         if (value)
         {
            HSZEVT evt;

            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "vrgEndptSignal (EPSIG_PSTNCALLSETUP) "));
            pstnState = GetPstnVrgEndptState();

            if ( pstnState != NULL )
            {
               /* Setup the routed FXS line identifer.
               **
               **   [1] For port to port call, indicates which FXS line is routed
               **       over the FXO port.
               **
               **   [2] For VoIP call, the value is equal to the FXO line itself.
               */
               pstnState->routedFxsLineId = state->lineId;

               evt.hsxdevhdl = pstnState->pstnVhdHdl;
               evt.hsxevt = PSTN_CTL_EVT_CALL_SETUP;
               evt.hsxop1 = pstnState->lineId;
               evt.hsxop2.ref = (char*)value;
               if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &evt ) )
               {
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_CALL_SETUP FAIL"));
               }
            }
         }
      }
      break;

      case EPSIG_PSTNCALLSETUP_CONNECTONLY:
      {
         VRG_ENDPT* pstnState;

         if (value)
         {
            HSZEVT evt;

            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "vrgEndptSignal (EPSIG_PSTNCALLSETUP) "));
            pstnState = GetPstnVrgEndptState();

            if (pstnState != NULL)
            {

               /* Setup the routed FXS line identifer.
               **
               **   [1] For port to port call, indicates which FXS line is routed
               **       over the FXO port.
               **
               **   [2] For VoIP call, the value is equal to the FXO line itself.
               */
               pstnState->routedFxsLineId = state->lineId;

               evt.hsxdevhdl = pstnState->pstnVhdHdl;
               evt.hsxevt = PSTN_CTL_EVT_CALL_SETUP;
               evt.hsxop1 = pstnState->lineId;
               evt.hsxop2.ref = PSTN_CONNECTONLY_DLSTRNG;
               if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &evt ) )
               {
                  VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_CALL_SETUP FAIL"));
               }
            }
         }
      }
      break;

      case EPSIG_OFFHOOK:
      case EPSIG_ANSWER:
      {
         VRG_ENDPT* pstnState;
         HSZEVT pstnEvt;   /* PSTN Ctl uses HSZEVT to recieve events */

         if (value)
         {
            pstnState = GetPstnVrgEndptState();
            if ( pstnState != NULL )
            {
#if !VRG_PSTN_TEST
               if( value != PSTN_CTL_DO_NOT_SIGNAL_LINE )
               {
                  casCtlSetStateFXS( pstnState->casCtlHandle, CAS_CTL_FXS_SEIZED );
                  VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Seized PSTN line %d", pstnState->lineId ));
               }
#endif
               pstnEvt.hsxdevhdl = pstnState->pstnVhdHdl;
               pstnEvt.hsxevt = PSTN_CTL_EVT_OFFHOOK;
               pstnEvt.hsxop1 = 0;
               pstnEvt.hsxop2.ref = NULL;
               if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &pstnEvt ) )
               {
                  VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_OFFHOOK FAIL"));
               }

               /* Disable FSK clid-rx service so that we dont get any false events while offhook */
               enableFskClidRx( VRG_FALSE, pstnState->pstnVhdHdl );

               if( signal == EPSIG_ANSWER )
               {
                  if ( pstnState->isFxsFxoCnxEstablished == VRG_FALSE )
                  {
                     pstnState->routedFxsLineId = state->lineId;
                     if ( ConnectSB( pstnState->lineId, pstnState->routedFxsLineId ) == EPSTATUS_SUCCESS )
                     {
                        pstnState->isFxsFxoCnxEstablished = VRG_TRUE;

                        /*
                        ** Send answer event to pstnCtl module so it can enable tone detection. We only
                        ** only need to change hsxevt in pstnEvt since the rest of the values remain the
                        ** same and are never modified by pstnCtlSendEvent.
                        */
                        pstnEvt.hsxevt = PSTN_CTL_EVT_ANSWER;
                        if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &pstnEvt ) )
                        {
                           VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_ANSWER FAIL"));
                        }

                        /* Once FXO call is established, configure the caller id RX service for off-hook (type 2) caller id detection */
                        if ( HAPISUCCESS != classRxConfigure( pstnState->pstnVhdHdl, CLASS_MSG_OFFHOOK_CLID, endptConfig.country ) )
                        {
                           VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "classRxConfigure TYPE2 FAIL vhd=0x%x!!!", pstnState->pstnVhdHdl ));
                        }

                        /* Illuminate corresponding LED */
                        xdrvLedCtrl(pstnState->lineId, 1);
                     }
                  }

                  if( value == PSTN_CTL_3_WAY_CONFERENCE )
                  {
                     CNXSTATE *cnx = GetCnxState( duration );
                     if( cnx != NULL )
                     {
                        hdspRmSwitchBoardConnect( cnx->vhdhdl,
                                                  HAPI_SWB_BOS,
                                                  pstnState->lineVhdHdl,
                                                  HAPI_SWB_TOS,
                                                  HAPI_SWB_DUPLEX,
                                                  VRG_TRUE );
                        pstnState->confCnxVhdHdl = cnx->vhdhdl;
                        VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Connecting PSTN 0x%x <-> VHD 0x%x: 3-way conference start",
                                 pstnState->lineVhdHdl, cnx->vhdhdl));
                     }
                  }
               }
                  pstnState->offHook = VRG_TRUE;
            }
            else
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Cannot find PSTN CAS control handle. Signal not applied"));
            }
         }
      }
      break;

      case EPSIG_ONHOOK:
      {
         VRG_ENDPT* pstnState;
         HSZEVT pstnEvt;   /* PSTN Ctl uses HSZEVT to recieve events */

         if (value)
         {
            pstnState = GetPstnVrgEndptState();
            if ( pstnState != NULL)
            {
               switch( value )
               {
                  case PSTN_CTL_DO_NOT_DISCONNECT_PRIMARY:
                  case PSTN_CTL_3_WAY_CONFERENCE:
                  {
                     if( pstnState->confCnxVhdHdl != 0x00 )
                     {
                        hdspRmSwitchBoardConnect( pstnState->confCnxVhdHdl,
                                                  HAPI_SWB_BOS,
                                                  pstnState->lineVhdHdl,
                                                  HAPI_SWB_TOS,
                                                  HAPI_SWB_DUPLEX,
                                                  VRG_FALSE );
                        VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Disconecting PSTN 0x%x <-> VHD 0x%x: 3-way conference end",
                                 pstnState->lineVhdHdl, pstnState->confCnxVhdHdl));
                        pstnState->confCnxVhdHdl = 0x00;
                     }
                  }
                  /* Fall through */
                  case PSTN_CTL_DO_NOT_SIGNAL_LINE:
                  break;

                  default:
                  {
                     EPCLIDTYPE clidType;
#if !VRG_PSTN_TEST
                     casCtlSetStateFXS( pstnState->casCtlHandle, CAS_CTL_FXS_RELEASED );
                     VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Released PSTN line %d", pstnState->lineId));
#endif
                     pstnEvt.hsxdevhdl = pstnState->pstnVhdHdl;
                     pstnEvt.hsxevt = PSTN_CTL_EVT_ONHOOK;
                     pstnEvt.hsxop1 = 0;
                     pstnEvt.hsxop2.ref = NULL;
                     if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &pstnEvt ) )
                     {
                        VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_ONHOOK FAIL" ));
                     }

                     /* Check if fsk clidrx is required */
                     Provision( state->endptArchive, EPPROV_CIDSigProtocol, &clidType );

                     /* Turn on FSKCLIDRX module based on clidrx setting */
                     if ( clidType == EPCLIDTYPE_FSK )
                     {
                        /* Re-enable clid rx service */
                        enableFskClidRx(VRG_TRUE, pstnState->pstnVhdHdl);
                     }
                  }
                  break;
               }

               if( pstnState->isFxsFxoCnxEstablished == VRG_TRUE  &&
                   value != PSTN_CTL_DO_NOT_DISCONNECT_PRIMARY )
               {
                  if ( DisconnectSB( state->lineId, pstnState->lineId ) == EPSTATUS_SUCCESS )
                  {
                     /* No FXS line is routed anymore over the FXO port */
                     pstnState->routedFxsLineId = -1;
                     pstnState->isFxsFxoCnxEstablished = VRG_FALSE;

                     /* Once FXO call is torn down, configure the caller id RX service for on-hook (type 1) caller id detection */
                     if ( HAPISUCCESS != classRxConfigure( pstnState->pstnVhdHdl, CLASS_MSG_ONHOOK_CLID, endptConfig.country ) )
                     {
                        VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "classRxConfigure TYPE1 FAIL vhd=0x%x!!!", pstnState->pstnVhdHdl ));
                     }

                     /* Disable corresponding LED */
                     xdrvLedCtrl(pstnState->lineId, 0);


                  }
                  else
                  {
                     VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR disconnecting lines %d and %d", state->lineId, pstnState->lineId ));
                  }
               }

               pstnState->offHook = VRG_FALSE;
            }
            else
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Cannot find PSTN CAS control handle. Signal not applied" ));
            }
         }
      }
      break;

      /* Update Tx Gain value */
      case EPSIG_TXGAIN:
      {
         int gain = (int)value;
         state->txGain = gain;

         hdspVhdSetIngressGain(state->lineVhdHdl, gain);
      }
      break;

      /* Update Rx Gain value */
      case EPSIG_RXGAIN:
      {
         int gain = (int)value;
         state->rxGain = gain;

         hdspVhdSetEgressGain(state->lineVhdHdl, gain);
      }
      break;

      /* Boosted Loop Current */
      case EPSIG_BOOSTED_LOOP_CURRENT:
      {
         /* Let the heartbeat module know this endpt now has a signal being applied */
         hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

         /* Get the SLIC driver for this endpoint */
         slicDriver = boardHalSlicGetDriver( state->endptNum );

         if( slicDriver == NULL )
         {
            break;
         }

         if (value)
         {
              casCtlSendCommand(CAS_REQ_SLIC_BOOTED_LOOP_CURRENT, CAS_CMD_MODE_ASYNC, (VRG_UINT32)slicDriver, VRG_TRUE);
         }
         else
         {
              casCtlSendCommand(CAS_REQ_SLIC_BOOTED_LOOP_CURRENT, CAS_CMD_MODE_ASYNC, (VRG_UINT32)slicDriver, VRG_FALSE);
         }

         if (!value)
         {
            /* Let the heartbeat module know this endpt now has a signal being applied */
            hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
         }
      }
      break;

      /* Update ringing waveform parameters */
      case EPSIG_RING_WAVEFORM:
      {
         EPSTATUS status = SetRingParams(state);
         if (status != EPSTATUS_SUCCESS)
         {
            return ( status );
         }
      }
      break;

      case EPSIG_ADMIN_STATUS:
      {
         CAS_CTL_FXO_INTERFACE_STATE casState;
         VRG_BOOL                    bEnable;

         /*
         ** This is an administrative signal request indicating the Endpoint
         ** status has changed (enabled/disabled)
         */
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: EPSIG_ADMIN_STATUS signal request, val=%d ", state->lineId, value ));

         if (value)
         {
             /* Enable - set the CAS state to IDLE and re-enable the Endpoint */
             casState =  CAS_CTL_FXO_IDLE;
             bEnable  = VRG_TRUE;
         }
         else
         {
             /* Disable - set the CAS state to service-denied and disnable the Endpoint */
             casState =  CAS_CTL_FXO_SERVICEDENIED;
             bEnable  = VRG_FALSE;
         }


         /*
         ** If the Line diags are currently in progress, don't shutdown the line
         ** immediately. Instead, just signal the TPD of this status change, so when
         ** it completes it can restore the line into the right mode.
         */
         if (state->testMode != TESTMODE_LINEDIAG )
         {
            /* Send CAS control command to go into right state */
            casCtlSetState( state->casCtlHandle, casState );

            /* Pause for CAS state to properly take place */
            bosSleep( 100 );

            /* Control the actual physical interface on the Endpoint */
            boardHalSlicEnable( state->endptNum, bEnable );
         }

#if VRG_ENDPT_CFG_USE_TPD
         /*
         ** Signal the TPD task that the line admin status changed
         ** Do this regardless if the tests are currently in progress so the
         ** next time the diagnostics are executed, the line can be restored
         ** into the correct state.
         */
         tpdCtlSignalAdminStatus( state->endptNum, bEnable );
#endif
      }
      break;

      case EPSIG_DAA_DISABLE:
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "EPSIG_DAA_DISABLE" ));
         boardHalDaaDeinit();
      break;

      case EPSIG_DAA_ENABLE:
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "EPSIG_DAA_ENABLE" ));
         boardHalDaaInit( endptConfig.country );
      break;

      /* This signal is used for external endpoint who does not suppport in-band DTMF
         to inject back DTMF for in-band DTMF relay.  The cnxId is not known for the
         external endpoint. Hence it should be set to -1.
         If cnxId is known, the signal will be processed in early stage of
         this function. */
      case EPSIG_INGRESS_DTMF:
      {
         VRG_UINT32 dbLevel, pulseTime, skew;
         const EPZPTEDIGIT2EPEVTMAP *digitMap = digit2epevtMap;
         VRG_ENDPT* pstnState;

         if ((cnxId != -1 ) || ((value >= EPEVT_DTMFD) && (value <= EPEVT_DTMF0)))
         {
            /* the case cnxId != -1 is processed previously */
            break;
         }

         /* Find the correspondig event for the tone */
         while( digitMap->epEvent != value )
         {
            digitMap++;
         }

         /* Generate ingress signal on cnx */ 
       Provision( state->endptArchive,EPPROV_DtmfDbLevel, &dbLevel);
       Provision( state->endptArchive,EPPROV_DtmfPulseTime, &pulseTime);       
         Provision( state->endptArchive, EPPROV_DTMFSkew, &skew );

         hdspDigitOn( state->lineVhdHdl, state->lineId, digitMap->digit, dbLevel, skew, pulseTime, INGRESS_TONE );

         pstnState = GetPstnVrgEndptState();
         if (pstnState != NULL)
         {
            if ((pstnState->routedFxsLineId == state->lineId) && ( pstnState->isFxsFxoCnxEstablished == VRG_TRUE ))
            {
               /* We are connected to PSTN line, So, send the DTMF tone to PSTN line. */
               Provision( pstnState->endptArchive,EPPROV_DtmfDbLevel, &dbLevel);
               Provision( pstnState->endptArchive,EPPROV_DtmfPulseTime, &pulseTime);
               Provision( pstnState->endptArchive, EPPROV_DTMFSkew, &skew );
               hdspDigitOn( pstnState->lineVhdHdl, pstnState->lineId, digitMap->digit, dbLevel, skew, pulseTime, EGRESS_TONE );
            }
         }

      }
      break;


      default:
      {
         /* Check if this is E-line package cadence ringing.  Since there are
         ** 128 possible cadence ringing in the E-line package, we don't
         ** want to add 128 'cases'.  So we will just check whether the
         ** specified signal is within the cadence ringing range */
         if ( ( signal >= EPSIG_CADENCE_RING_BASE ) &&
              ( signal <= EPSIG_CADENCE_RING_MAX ) )
         {
            if ( value )
            {
               CLASSSTM_RING_STATE  pattern;


               /* Let the heartbeat module know this endpt now has a signal being applied */
               hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

               GetRingPattern( signal, endptConfig.country, &pattern );

               casCtlRingProfileOn( state->casCtlHandle, &pattern, VRG_TRUE );
            }
            else
            {
               /* Let the heartbeat module know this endpt now has a signal being applied */
               hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

               if( ! state->offHook )
               {
                  casCtlRingOff( state->casCtlHandle, VRG_FALSE );
               }

               /* Let the heartbeat module know this endpt no longer has an active
               ** endpt signal */
               hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
            }
         }
         else
         {
            return( EPSTATUS_SIGNAL_UNKNOWN );
         }
      }
      break;

   }

   /* Save active brief signal to detect signal ending event */
   state->activeBriefSignal = briefSignal;

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED SIGNAL %d", signal ));

   return(EPSTATUS_SUCCESS);
}

/*****************************************************************************
*  FUNCTION:   ProcessVrgEndptCreateConnection
*
*  PURPOSE:    Function to create a connection
*
*  PARAMETERS: endptState - pointer to endpoint state structure
*              cnxId      - connection identifier
*              cnxParam   - ptr to connection parameter block
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS ProcessVrgEndptCreateConnection
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPZCNXPARAM   *cnxParam
)
{
   VRG_ENDPT        *state;
   CNXSTATE         *cnx;
   EPSTATUS          status;
   int               cnxIndex;
   int               lineId = ((VRG_ENDPT_STATE *) endptState)->lineId;
#ifndef VRG_ENDPT_CFG_G168_TEST
   int               newVoiceJBFixed;
   int               newVoiceJBMin;
   int               newVoiceJBMax;
   int               newVoiceJBTarget;
   int               newDataJBTarget;
#else
   VRG_BOOL          voiceJBFixed;
   VRG_UINT16        voiceJBMin;
   VRG_UINT16        voiceJBMax;
   VRG_UINT16        voiceJBTarget;
   VRG_UINT16        dataJBTarget;
   VRG_UINT16        ajcCustomReg;
#endif
   HSZNETREGS        netregs;
   int               provCngDetEnabled;
   VRG_SINT16        result;
   int               dspNum;
   VRG_BOOL          digitalMode = BOS_FALSE;

   dspNum = lineId / (VRG_ENDPT_CFG_NUM_ENDPT/VRG_GLOBAL_CFG_MAX_NUM_DSP);

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "CREATE CNX: dsp %d, endpt %d, cnx %d", dspNum, lineId, cnxId));
   printConnectionParams( cnxParam );

   /*
   ** Get the endpoint state
   */
   state = GetEndptState( lineId );
   if( ! state )
   {
      return ( EPSTATUS_ENDPT_UNKNOWN );
   }

   /*
   ** Verify the requested connection mode against the endpoint's capabilities
   */
   if (!IsCnxModeSupported(cnxParam->mode, dspNum))
   {
      return ( EPSTATUS_UNSUPPORTED_MODE );
   }

   /*
   ** Check if we need to enable digital mode
   */
   if( cnxParam->cnxParmList.send.codecs[0].type == CODEC_CLEARCHANNEL )
   {
      digitalMode = VRG_TRUE;
   }

   /*
   ** Get the cnx state
   */

   /* If connection is being made to pstn endpoint, check if primary cnx vhd is full one *
    * if not then try to grab a full vhd. If not available then lite vhd is okay          */
   if ( state->endptType == EPTYPE_PSTN  )
   {
      if( state->cnx[PRIMARY_CNX_INDEX] )
      {
         cnx = state->cnx[PRIMARY_CNX_INDEX];
         while ( cnx->bLiteWeight )
         {
            cnx = GetNewSecondaryCnxState( dspNum );
            if( !cnx )
            {
               /* global cnx queue is full */
                VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "No full VHD found for connection to pstn endpoint. Using lite VHD" ));
                break;
            }
         }

         if ( cnx )
         {
            state->cnx[PRIMARY_CNX_INDEX] = cnx;
            cnxIndex = PRIMARY_CNX_INDEX;
         }
      }
   }

#if FLEXIBLE_CONFERENCING_SUPPORTED
   /* Check for any reservation */
   for( cnxIndex = PRIMARY_CNX_INDEX; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
   {
      if (state->cnx[cnxIndex] &&
      (state->cnx[cnxIndex]->cnxId == CNX_IDLE))
      {
         /* found index into endpoint's cnx queue */
         /* save the cnx state in the endpoint model */
         cnx = state->cnx[cnxIndex];
         break;
      }
   }

   if (cnxIndex == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT)
#else
   /* check for the availability of the primary vhd */
   if ( state->cnx[PRIMARY_CNX_INDEX]->cnxId == CNX_UNUSED )
   {
      /* point to the first slot in the endpt's cnx queue */
      cnx = state->cnx[PRIMARY_CNX_INDEX];
      cnxIndex = PRIMARY_CNX_INDEX;
   }
   /* secondary vhd */
   else
#endif
      {
      /* find the next empty slot in the global cnx queue */
      cnx = GetNewSecondaryCnxState( dspNum );
      if( !cnx )
      {
         /* global cnx queue is full */
         return ( EPSTATUS_RESOURCES );
      }

      /* determine the next empty slot in the endpoint's cnx queue, starting from the 2nd position */
#if FLEXIBLE_CONFERENCING_SUPPORTED
      for( cnxIndex = PRIMARY_CNX_INDEX; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
#else
      for( cnxIndex = SECONDARY_CNX_START_INDEX; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
#endif
      {
         if (state->cnx[cnxIndex] == 0)
         {
            /* found index into endpoint's cnx queue */
            /* save the cnx state in the endpoint model */
            state->cnx[cnxIndex] = cnx;
            break;
         }
      }

      if (cnxIndex == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT)
      {
         /* endpoint's cnx queue is full */
         return ( EPSTATUS_RESOURCES );
      }
   }

   /*
   ** Update cnx state
   */
   cnx->cnxId = cnxId;
   cnx->lineId = lineId;
   cnx->firstValidRTPPkt = 1;
   cnx->bSBDisconnect = VRG_FALSE;

   /*
   ** CNG detection is on by default
   */
   cnx->bCngDetEnabled = (digitalMode ? VRG_FALSE : VRG_TRUE);
   cnx->bNotifyFaxCall = VRG_FALSE;

   cnx->localSsrc = cnxParam->localSsrc;

   /* Inform heartbeat module of the endpt number assiocated with this cnx */
   hbUpdatePatientEndptId( cnx->cnxStateHBId, lineId );

   /* Let the heartbeat module know this cnx is now active */
   hbUpdatePatientState( cnx->cnxStateHBId, HBSTATE_ACTIVE );

   if ( state->endptType == EPTYPE_PSTN  )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "CREATE CNX: Calling ConnectPstnGwSB"));
      ConnectPstnGwSB(state->cnx[cnxIndex]->vhdhdl, state->lineVhdHdl);
   }
   else
   {
      /* update masks for Switchboard cnx setups */
      UpdateSBMasks( state->cnx[cnxIndex], cnxParam->mode);

      /*
      ** Needed to change from Addressing to Active state to enable
      ** flash detection instead of pulse dialing.
      */
      if( state->offHook )
      {
         casCtlSetState( state->casCtlHandle, CAS_CTL_FXO_ACTIVE );
      }
      /* If we're on-hook then we still need to be able to pass signals through. */
      else
      {
#if VRG_ENDPT_CFG_TWO_WAY_PATH_IN_ONHOOK
         hdspVhdSetIngressGain( state->lineVhdHdl, state->txGain );
         casCtlSetState( state->casCtlHandle, CAS_CTL_FXO_ALERTING_OHT );

#else /* !VRG_ENDPT_CFG_TWO_WAY_PATH_IN_ONHOOK */
         if ( state->bGR303Support )
         {
            casCtlSetState( state->casCtlHandle, CAS_CTL_FXO_ALERTING_OHT );
         }
#endif /* !VRG_ENDPT_CFG_TWO_WAY_PATH_IN_ONHOOK */
      }
   }

   /*
   ** Process echo cancellation parameter.
   ** Process Ecan state machine accordingly.
   */
   if (cnxParam->echocancel && !digitalMode)
   {
      EcanProcessStm(state, ECANEVT_CRCX_ON);
   }
   else
   {
      EcanProcessStm(state, ECANEVT_CRCX_OFF);
   }

#ifndef VRG_ENDPT_CFG_G168_TEST
   /*
   ** Load latest jitter buffer parameters
   */
   if (cnxParam->csCall)
   {
      status = EPSTATUS_SUCCESS;
      newVoiceJBFixed   = 0;
      newVoiceJBMin     = 0;
      newVoiceJBMax     = 40;
      newVoiceJBTarget  = 40;
      newDataJBTarget   = 40;
   }
   else if(digitalMode)
   {
      status = EPSTATUS_SUCCESS;
      /* For digital mode, fix the jitter buffer at 100ms */
      newVoiceJBFixed   = 1;
      newVoiceJBMin     = 100;
      newVoiceJBMax     = 100;
      newVoiceJBTarget  = 100;
      newDataJBTarget   = 100;
   }
   else
   {
      /* Setup the JB according (in ms) to the provisioning if it has changed. */
      status = ~EPSTATUS_SUCCESS;
      if( ( Provision( state->endptArchive,
                       EPPROV_VoiceJitterBuffFixed,
                       &newVoiceJBFixed ) == EPSTATUS_SUCCESS ) &&
          ( Provision( state->endptArchive,
                       EPPROV_VoiceJitterBuffMin,
                       &newVoiceJBMin ) == EPSTATUS_SUCCESS ) &&
          ( Provision( state->endptArchive,
                       EPPROV_VoiceJitterBuffMax,
                       &newVoiceJBMax ) == EPSTATUS_SUCCESS ) &&
          ( Provision( state->endptArchive,
                       EPPROV_VoiceJitterBuffTarget,
                       &newVoiceJBTarget ) == EPSTATUS_SUCCESS ) &&
          ( Provision( state->endptArchive,
                       EPPROV_DataJitterBuffTarget,
                       &newDataJBTarget ) == EPSTATUS_SUCCESS ) )
      {
         status = EPSTATUS_SUCCESS;
      }
   }

   /*
   ** Set the JB registers if any of the provisioning was successful and at
   ** least one of the values has changed.
   */
   if(
       ( status == EPSTATUS_SUCCESS ) &&
       (
         ( newVoiceJBFixed != cnx->vrgVoiceJBFixed ) ||
         ( newVoiceJBMin != cnx->vrgVoiceJBMin ) ||
         ( newVoiceJBMax != cnx->vrgVoiceJBMax ) ||
         ( newVoiceJBTarget != cnx->vrgVoiceJBTarget ) ||
         ( newDataJBTarget != cnx->vrgDataJBTarget )
       )
     )
   {
      VRG_SINT16       result;

      result = hdspVhdSetJitterBuffer( cnx->vhdhdl,
                                       newVoiceJBFixed,
                                       newVoiceJBMin,
                                       newVoiceJBMax,
                                       newVoiceJBTarget,
                                       newDataJBTarget,
                                       cnx->ajcCustomReg );

      if( result != HAPISUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting jitter buffer registers for line %d", cnx->lineId ));
      }
      else
      {
         /* Update the connection settings. */
         cnx->vrgVoiceJBFixed = newVoiceJBFixed;
         cnx->vrgVoiceJBMin = newVoiceJBMin;
         cnx->vrgVoiceJBMax = newVoiceJBMax;
         cnx->vrgVoiceJBTarget = newVoiceJBTarget;
         cnx->vrgDataJBTarget = newDataJBTarget;
      }
   }
#else
   result = hdspVhdGetJitterBuffer( cnx->vhdhdl,
                                    &voiceJBFixed,
                                    &voiceJBMin,
                                    &voiceJBMax,
                                    &voiceJBTarget,
                                    &dataJBTarget,
                                    &ajcCustomReg );

   if( result != HAPISUCCESS )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR getting jitter buffer registers for VHD handle 0x%x", cnx->vhdhdl ));
   }
   else
   {
      /* Display registers. */
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "VHD Jitter Buffer settings for vhdhdl = %x", cnx->vhdhdl ));
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "------------------------------------------"));
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "voiceJBFixed = %s", (voiceJBFixed) ? ("FIXED") : ("ADAPTIVE") ));
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "voiceJBMin = %d (ms)", voiceJBMin ));
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "voiceJBMax = %d (ms)", voiceJBMax ));
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "voiceJBTarget = %d (ms)", voiceJBTarget ));
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "dataJBTarget = %d (ms)", dataJBTarget ));
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "ajcCustomReg = 0x%x", ajcCustomReg ));
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "------------------------------------------"));
   }

#endif /* VRG_ENDPT_CFG_G168_TEST */

   /*
   ** Configure AMR settings for the connection.
   */
   if( cnxParam->cnxParmList.send.codecs[0].type >= CODEC_GSMAMR_12K &&
       cnxParam->cnxParmList.send.codecs[0].type <= CODEC_AMRWB_OA_2385 )
   {
      result = hdspVhdSetAmrSettings( cnx->vhdhdl,
                                      cnxParam->amrConfig.modeSet,
                                      cnxParam->amrConfig.modeChangeNeighbor,
                                      cnxParam->amrConfig.modeChangePeriod,
                                      cnxParam->amrConfig.ptime,
                                      cnxParam->amrConfig.red );
      if( result != HAPISUCCESS )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting AMR registers for line %d", cnx->lineId ));
      }
   }

   netregs = cnx->netRegs;

   /*
   ** Query the MIB to determine if CNG detection should be enabled.
   */
   status = Provision( state->endptArchive, EPPROV_FaxDetection,
                       &provCngDetEnabled );

   if ( status == EPSTATUS_SUCCESS && !digitalMode )
   {
      /*
      ** Set net register based on MIB value
      */
      if ( provCngDetEnabled == MIB_CNG_DET_ENABLED )
      {
         netregs.hsxNetOptions &= ~HSNET_NET_OPTION_IGNORE_CNG_ING_EVT;
         cnx->bCngDetEnabled = VRG_TRUE;
      }
      else
      {
         netregs.hsxNetOptions |= HSNET_NET_OPTION_IGNORE_CNG_ING_EVT;
         cnx->bCngDetEnabled = VRG_FALSE;
      }
   }

   /*
   ** Query the MIB to determine which tone detection should be enabled.
   */
   cnx->bBell103Enabled = VRG_FALSE;
   netregs.hsxNetOptions |= HSNET_NET_OPTION_CDIS;
   netregs.hsxNetOptions &= ~HSNET_NET_OPTION_IGNORE_CNG_EGR_EVT; /* Allow switching on egress CNG */
   netregs.hsxCallDOptions |= ( HSNET_CDIS_OPTION_V18ACTIVE | HSNET_CDIS_OPTION_BELL103ACTIVE | HSNET_CDIS_OPTION_CUSTOMTONEACTIVE );


   if ( cnxParam->V18Support == VRG_TRUE )
   {
      cnx->bBell103Enabled = VRG_TRUE;
      netregs.hsxCallDOptions |= ( HSNET_CDIS_OPTION_V18ACTIVE | HSNET_CDIS_OPTION_BELL103ACTIVE | HSNET_CDIS_OPTION_CUSTOMTONEACTIVE );
   }
   else 
   {
      cnx->bBell103Enabled = VRG_FALSE;
      netregs.hsxCallDOptions &= ~( HSNET_CDIS_OPTION_V18ACTIVE | HSNET_CDIS_OPTION_BELL103ACTIVE | HSNET_CDIS_OPTION_CUSTOMTONEACTIVE );
   }
            

   
   /*
    ** Set Auto encoder switch configuration
    */
   if (cnxParam->autoEncoder == EPAUTOENC_ON)
   {
      netregs.hsxNetOptions |= HSNET_NET_OPTION_AUTO_ENC_SWITCH;
   }
   else
   {
      netregs.hsxNetOptions &= ~HSNET_NET_OPTION_AUTO_ENC_SWITCH;
   }


   /* turn on egress DTMF detection */
   {
      VRG_BOOL  egressDtmfEnabled = VRG_FALSE;
      Provision( state->endptArchive, EPPROV_EgressDtmfDetection, &egressDtmfEnabled );
      if( egressDtmfEnabled )
      {
         netregs.hsxPtePacketVoiceOptions |= HSNET_PTE_OPTION_DETECTOR_EGRESSMASK|HSNET_PTE_OPTION_DIG_EVT ;
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "turn on egress DTMF detection %d", netregs.hsxPtePacketVoiceOptions));
      }
      else
      {
         netregs.hsxPtePacketVoiceOptions &= (~HSNET_PTE_OPTION_DETECTOR_EGRESSMASK);
      }

      VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "hsxPtePacketVoiceOption = 0x%x\n", netregs.hsxPtePacketVoiceOptions));
   }

   /*
   ** If we are in digital mode, turn off all detectors, etc., reset gains, and
   ** enable no compress mode on the HAL.
   */
   if (digitalMode)
   {
      SetDigitalMode(state, cnx, VRG_TRUE);
   }

   /*
   ** Send new net configuration to the DSP (for CNG detection and tone detection) **************************
   */
   if ( (netregs.hsxCallDOptions != cnx->netRegs.hsxCallDOptions) || \
        (netregs.hsxNetOptions != cnx->netRegs.hsxNetOptions)     || \
        (netregs.hsxPtePacketVoiceOptions != cnx->netRegs.hsxPtePacketVoiceOptions))
   {
      VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "HAPINET_CONFIG_SETREGS"));
      result = hdspSendCmdDataAsync( cnx->vhdhdl,
                                     HAPINET_CONFIG_SETREGS,
                                     HSCMDEXTDATA_ASYNC,
                                     sizeof( netregs ),
                                     &netregs );

      if( result )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting net registers for line %d", cnx->lineId ));
      }

      cnx->netRegs.hsxCallDOptions = netregs.hsxCallDOptions;
      cnx->netRegs.hsxNetOptions = netregs.hsxNetOptions;
      cnx->netRegs.hsxPtePacketVoiceOptions = netregs.hsxPtePacketVoiceOptions;
   }


   /* Set connection digit relay type before initializing RTP service */
   cnx->digitRelayType = (digitalMode ? EPDTMFRFC2833_DISABLED : cnxParam->digitRelayType);

   /* Init RTP service config registers BEFORE setting voice mode */
   GetRtpRegisters(state->cnx[cnxIndex]);

   InitRtpServiceRegisters( state->cnx[cnxIndex] );
   SetRtpServiceRegisters( state->cnx[cnxIndex], cnxParam->rtcpXRMode, &cnxParam->vbdparam, cnxParam->secHdrSize );

   /* Initialize RTP stats */
   RtpInit( &cnx->rtpstate );

   /*
   ** First set the connection options, e.g. voice encoder, silence suppression etc.
   ** so that when we set the voice mode we receive properly formatted packets
   */
   status = SetConnectionOptions( cnx, cnxParam, VRG_TRUE );
   if ( status != EPSTATUS_SUCCESS )
   {
      /* Invalidate the endpt number in the heartbeat module since
      ** create connection failed */
      hbUpdatePatientEndptId( cnx->cnxStateHBId, HBPATIENT_UNKNOWN_ENDPT );

      /* Let the heartbeat module know this cnx is now inactive */
      hbUpdatePatientState( cnx->cnxStateHBId, HBSTATE_INACTIVE );

      /* mark the current cnx as IDLE, so we won't run out the connections next time */
      cnx->cnxId = CNX_IDLE;
      cnx->lineId = BAD_ENDPTID;

      return ( status );
   }

   /* process mode parameter */
   switch( cnxParam->mode )
   {
      case EPCNXMODE_SNDO:      /* Send only */
      case EPCNXMODE_RCVO:      /* Receive only */
      case EPCNXMODE_SNDRX:     /* Send-Receive mode */
      case EPCNXMODE_LOOP:      /* Network Loopback test */
      case EPCNXMODE_TEST:      /* Network Continuity test */
      case EPCNXMODE_REPLCATE:  /* Replicate */
      case EPCNXMODE_CONF:      /* Send-Receive Conference mode */
      case EPCNXMODE_TRANSCODE: /* Transcode mode */
      {
         /* Now set the voice mode, everything else is in place */
         SetVoiceMode( state->cnx[cnxIndex], VM_ACTIVE );
      }
      break;

      case EPCNXMODE_INACT:     /* Inactive */
      {
         /* Save connection state parameters */
         SaveConnectionOptions( cnx, cnxParam );

         /* Switch to active voice mode */
         SetVoiceMode( state->cnx[cnxIndex], VM_ACTIVE );

         return ( EPSTATUS_SUCCESS );
      }
      break;

      default:
      {
         /* Invalidate the endpt number in the heartbeat module since
         ** create connection failed */
         hbUpdatePatientEndptId( cnx->cnxStateHBId, HBPATIENT_UNKNOWN_ENDPT );

         /* Let the heartbeat module know this cnx is now inactive */
         hbUpdatePatientState( cnx->cnxStateHBId, HBSTATE_INACTIVE );

         /* mark the current cnx as IDLE, so we won't run out the connections next time */
         cnx->cnxId = CNX_IDLE;
         cnx->lineId = BAD_ENDPTID;

         return ( EPSTATUS_UNSUPPORTED_MODE );
      }
      break;
   }

   if (state->testMode == TESTMODE_TR57)
   {
      if ( cnxParam->mode == EPCNXMODE_SNDRX)
      {
         /* Save the voice mode, and change the mode of the VHD to VBD */
         cnx->vbData.bEnabled = VRG_TRUE;
         SetVoiceMode( cnx, VM_ACTIVE );
      }
   }

   /*
   ** Set RTCP interval
   */
   if( cnxParam->rtcpIntMs )
   {
      SetRtcpInterval( state->cnx[cnxIndex], cnxParam->rtcpIntMs );
   }
   else
   {
      SetRtcpInterval( state->cnx[cnxIndex], HAPI_RTP_MIN_RTCP_PACKET_INTERVAL );
   }

   /*
   ** Set PVE ingress/egress gain
   */
   cnx->rxVolume = cnxParam->rxVolume;
   hdspVhdSetPVEEgressVolume(cnx->vhdhdl, cnxParam->rxVolume );

   cnx->txVolume = cnxParam->txVolume;
   hdspVhdSetPVEIngressVolume(cnx->vhdhdl, cnxParam->txVolume );


   /*
   ** Set RTP mode & payload types
   */
   SetRtpPayloadTypes( cnx, &cnxParam->cnxParmList );

   SetRtpMode( state->cnx[cnxIndex], cnxParam->mode );

   ProgramRtpRegisters(state->cnx[cnxIndex]);
   /*
   ** Save connection state parameters
   */
   SaveConnectionOptions( cnx, cnxParam );

   /* now check GR303 flag, and sends out current hook state */
   if ( state->bGR303Support )
   {
      /* Update hookstate by sending an offhook / onhook event to the LDX RTP service */
      GenerateNteEvent( cnx, (state->earlyOffHook? RTP_NTE_OFFHOOK : RTP_NTE_ONHOOK) );
   }

   /* Save the connection id if this is CS call */
   if (cnxParam->csCall == VRG_TRUE) {
     gCSCnxId = cnxId;
     SetRtpCSCallEnable ( cnx );
     cnx->csCall = VRG_TRUE;
     VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "2G/3G call is now connected\n"));
   }


   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED CREATE CNX: dsp %d, endpt %d, cnx %d ", dspNum, lineId, cnxId ));
   return ( EPSTATUS_SUCCESS );
}

/*****************************************************************************
*  FUNCTION:   ProcessVrgEndptModifyConnection
*
*  PURPOSE:    Function to modify a connection
*
*  PARAMETERS: endptState - pointer to endpoint state structure
*              cnxId      - connection identifier
*              cnxParam   - ptr to connection parameter block
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS ProcessVrgEndptModifyConnection
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPZCNXPARAM   *cnxParam
)
{
   VRG_ENDPT        *state;
   CNXSTATE         *cnx;
   int               cnxIndex;
   VRG_UINT16        status;
   int               lineId = ((VRG_ENDPT_STATE *) endptState)->lineId;
   int               dspNum;
   VRG_BOOL          digitalMode = BOS_FALSE;

   dspNum = lineId / (VRG_ENDPT_CFG_NUM_ENDPT/VRG_GLOBAL_CFG_MAX_NUM_DSP);

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "MODIFY CNX: dsp %d, endpt %d, cnx %d", dspNum, lineId, cnxId ));
   printConnectionParams( cnxParam );

   /*
   ** Get the endpoint state
   */
   state = GetEndptState( lineId );
   if( ! state )
   {
      return ( EPSTATUS_ENDPT_UNKNOWN );
   }

   /*
   ** Verify the requested connection mode against the endpoint's capabilities
   */
   if (!IsCnxModeSupported(cnxParam->mode, dspNum))
   {
      return ( EPSTATUS_UNSUPPORTED_MODE );
   }

   /*
   ** Check if we need to enable digital mode
   */
   if( cnxParam->cnxParmList.send.codecs[0].type == CODEC_CLEARCHANNEL )
   {
      digitalMode = VRG_TRUE;
   }

   /* Determine the index for the current cnx in the endpoint's cnx queue */
   for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
   {
      if ((state->cnx[cnxIndex]) && (state->cnx[cnxIndex]->cnxId == cnxId))
      {
         /* cnx index found */
         break;
      }
   }
   if (cnxIndex == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT)
   {
      /* specified cnx is not within the endpoint's cnx list */
      return ( EPSTATUS_CNX_UNKNOWN );
   }

   /* fetch the cnx state */
   cnx = state->cnx[cnxIndex];

   /* If our connection is already in FAX mode and we are told to keep it
   ** like so, do not process this command further.
   */
   if( (cnx->vhdMode == NETMODE_FAXRELAY) && (cnxParam->preserveFaxMode) )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "ENDPT %d: Preserving fax mode on connection %d",
                cnx->lineId, cnxIndex ));
      return ( EPSTATUS_SUCCESS );
   }

   /* If the lineVhd associated with this connection is internally
    * connected to another line VHD, and we are going into conf
    * mode, setup a swboard connection to the other line VHD as well */
   if ( state->intLinkVhdHdl )
   {
      VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "Conferencing lineVhdl 0x%0x gwVhdl 0x%x with intlinked lineVhdl 0x%x",
                       state->lineVhdHdl,
                       state->cnx[cnxIndex]->vhdhdl,
                       state->intLinkVhdHdl ));
      hdspRmSwitchBoardConnect( state->cnx[cnxIndex]->vhdhdl,
                                HAPI_SWB_BOS,
                                state->intLinkVhdHdl,
                                HAPI_SWB_TOS,
                                HAPI_SWB_DUPLEX,
                                ((cnxParam->mode == EPCNXMODE_CONF)? VRG_TRUE:VRG_FALSE) );
   }

   /* update masks for Switchboard cnx setups */
   UpdateSBMasks( state->cnx[cnxIndex], cnxParam->mode);

   /*
   ** Process echo cancellation parameter.
   ** Process Ecan state machine accordingly.
   */
   if (cnxParam->echocancel && !digitalMode)
   {
      EcanProcessStm(state, ECANEVT_MDCX_ON);
   }
   else
   {
      EcanProcessStm(state, ECANEVT_MDCX_OFF);
   }

   /*
   ** Check if we need to reset the build out.  Build out reset is
   ** only required if we are switching from a non 10msec vocoder
   ** to a 10msec vocoder
   */
   if ( IS_NON_10MS_CODEC(cnx->codecType) &&
        (!( IS_NON_10MS_CODEC(cnxParam->cnxParmList.send.codecs[0].type))))
   {
      vrgEndptResetBuildOut( lineId, state->lineVhdHdl );
   }

   GetRtpRegisters(state->cnx[cnxIndex]);

   /*
   ** Process the request
   */
   switch( cnxParam->mode )
   {
      case EPCNXMODE_SNDO:      /* Send only */
      case EPCNXMODE_RCVO:      /* Receive only */
      case EPCNXMODE_SNDRX:     /* Send-Receive mode */
      case EPCNXMODE_LOOP:      /* Network Loopback test */
      case EPCNXMODE_TEST:      /* Network Continuity test */
      case EPCNXMODE_REPLCATE:  /* Replicate */
      case EPCNXMODE_CONF:      /* Send-Receive Conference mode */
      case EPCNXMODE_TRANSCODE: /* Transcode mode */
      {
         /* Only change to voice mode when we are actually switching to
         ** a voice codec */
         if ( !( cnxParam->cnxParmList.send.codecs[0].type == CODEC_T38 || cnxParam->cnxParmList.send.codecs[0].type == CODEC_T38_MUTE ) )
         {
#if 0    /* To support Redundancy in both voice mode and voice band data mode, 
            we need to comment out the following lines, or it will cause 
            voice mode to change vbd unexpectedly and even to crash when
            using G.729a as primary codec. */
            
            /* If we are enabling redundancy (which is only used for VBD V.152
            ** calls in fact) make sure the VHD mode will be set to voice-band-data. */
            if( (cnxParam->cnxParmList.send.codecs[0].type == CODEC_RED) &&
                (state->cnx[cnxIndex]->vbData.bEnabled == VRG_FALSE) )
            {
               state->cnx[cnxIndex]->vbData.bEnabled = VRG_TRUE;
            }
#endif

            /* Configure AMR settings for the connection, if applicable */
            if( cnxParam->cnxParmList.send.codecs[0].type >= CODEC_GSMAMR_12K &&
                cnxParam->cnxParmList.send.codecs[0].type <= CODEC_AMRWB_OA_2385 )
            {
               status = hdspVhdSetAmrSettings( cnx->vhdhdl,
                                               cnxParam->amrConfig.modeSet,
                                               cnxParam->amrConfig.modeChangeNeighbor,
                                               cnxParam->amrConfig.modeChangePeriod,
                                               cnxParam->amrConfig.ptime,
                                               cnxParam->amrConfig.red );

               if( status != HAPISUCCESS )
               {
                  VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting AMR registers for line %d", cnx->lineId ));
               }
            }

            /* If we are changing data mode but are in fax relay mode now, make sure we
            ** mark the VBD flag to go into the correct mode.
            */
            if( (cnx->dataMode != cnxParam->dataMode) &&
                (cnx->vhdMode == NETMODE_FAXRELAY) )
            {
               state->cnx[cnxIndex]->vbData.bEnabled = VRG_TRUE;
            }

            SetVoiceMode( state->cnx[cnxIndex], VM_ACTIVE );
         }
      }
      break;

      case EPCNXMODE_INACT:     /* Inactive */
      {
         /* No need to set RTP mode if voicemode is not active */
         if ( cnx->voiceMode != VM_ACTIVE || cnx->vhdMode == NETMODE_FAXRELAY )
         {
            /* Save connection state parameters */
            SaveConnectionOptions( cnx, cnxParam );
            return ( EPSTATUS_SUCCESS );
         }
         else
         {
            if( (state->cnx[cnxIndex]->mode != cnxParam->mode) &&
                (cnxParam->mode != EPCNXMODE_LOOP) && cnx->activeSignal )
            {
               VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: cnx %d - preserve RTP mode to %d",
                        lineId, cnxId, state->cnx[cnxIndex]->mode));
               /* Save connection state parameters, preserve the mode for the connection.
               */
               cnxParam->mode = state->cnx[cnxIndex]->mode;
               SaveConnectionOptions( cnx, cnxParam );
            }
            else
            {
               SetRtpMode( state->cnx[cnxIndex], cnxParam->mode );
               ProgramRtpRegisters( state->cnx[cnxIndex] );

               /* Save connection state parameters */
               SaveConnectionOptions( cnx, cnxParam );
            }

            return ( EPSTATUS_SUCCESS );
         }
      }
      break;

      default:
      {
         return ( EPSTATUS_UNSUPPORTED_MODE );
      }
      break;
   }


   if ( ConnectionOptionsChanged( cnx, cnxParam ))
   {
      /* Check if we are transitioning from T.38 to voice.  Generate an event
      ** to indicate that T.38 session has ended */
      if ( ( cnx->codecType == CODEC_T38 || cnx->codecType == CODEC_T38_MUTE ) &&
           (!( cnxParam->cnxParmList.send.codecs[0].type == CODEC_T38 || cnxParam->cnxParmList.send.codecs[0].type == CODEC_T38_MUTE )) )
      {
         /* Since the spec does not mandate what events to be reported
         ** in this scenario, we will just transmit the stop event to indicate
         ** the end of the T.38 fax relay procedure */
         (*endptConfig.notify)( state->endptArchive, cnx->cnxId, EPEVT_T38_STOP, NULL, 0, -1 );
      }

      /*
      ** Set the connection options, e.g. voice encoder, silence suppresion etc.
      */
      status = SetConnectionOptions( cnx, cnxParam, VRG_FALSE );
      if ( status != EPSTATUS_SUCCESS )
      {
         return ( status );
      }
   }


   if (state->testMode == TESTMODE_TR57)
   {
      if ( cnxParam->mode == EPCNXMODE_SNDRX)
      {
         /* Save the voice mode, and change the mode of the VHD to VBD */
         cnx->vbData.bEnabled = VRG_TRUE;
         SetVoiceMode( cnx, VM_ACTIVE );
      }
   }

   /*
   ** Set RTP mode & payload types
   */
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: cnx %d - set RTP registers and payload types", lineId, cnxId ));
   SetRtpServiceRegisters( state->cnx[cnxIndex], cnxParam->rtcpXRMode, &cnxParam->vbdparam, cnxParam->secHdrSize );
   SetRtpPayloadTypes( cnx, &cnxParam->cnxParmList );

   /*
   ** Check if we have some active connection signals ongoing on this
   ** endpoint, if so and if the new mode is not network loopback, do
   ** not apply any connection RTP mode change to preserve the signals.
   */
   if( (state->cnx[cnxIndex]->mode != cnxParam->mode) &&
       (cnxParam->mode != EPCNXMODE_LOOP) && cnx->activeSignal )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ENDPT %d: cnx %d - preserve RTP mode to %d",
               lineId, cnxId, state->cnx[cnxIndex]->mode));
   }
   else
   {
      SetRtpMode( state->cnx[cnxIndex], cnxParam->mode );
   }

   /*
   ** Set PVE ingress/egress gain
   */   
   cnx->rxVolume = cnxParam->rxVolume;  
   hdspVhdSetPVEEgressVolume(cnx->vhdhdl, cnxParam->rxVolume );      
   
   cnx->txVolume = cnxParam->txVolume;     
   hdspVhdSetPVEIngressVolume(cnx->vhdhdl, cnxParam->txVolume );
         
   /*
   ** Set RTCP interval
   */
   if( cnxParam->rtcpIntMs )
   {
      SetRtcpInterval( state->cnx[cnxIndex], cnxParam->rtcpIntMs );
   }
   else
   {
      SetRtcpInterval( state->cnx[cnxIndex], HAPI_RTP_MIN_RTCP_PACKET_INTERVAL );
   }

   /* Set digitRelay mode */
   if( !digitalMode && cnx->digitRelayType != cnxParam->digitRelayType )
   {   
      cnx->digitRelayType = cnxParam->digitRelayType;
      
      if ( cnx->digitRelayType == EPDTMFRFC2833_SUBTRACT )
      {
         cnx->rtpRegs2.configFlags2 |= HAPI_RTP_SUBTRACT_DTMF_ENABLE_MASK;
      }
      else
      {                       
         cnx->rtpRegs2.configFlags2 &= ~HAPI_RTP_SUBTRACT_DTMF_ENABLE_MASK;
      }
   }   

   ProgramRtpRegisters(state->cnx[cnxIndex]);

   /*
   ** If we are switching to or from a digital mode call, disable or enable
   ** required services.
   */
   if( (cnx->codecType != cnxParam->cnxParmList.send.codecs[0].type) )
   {
      if( (cnx->codecType == CODEC_CLEARCHANNEL) )
      {
         /* Disable digital mode */
         SetDigitalMode( state, cnx, VRG_FALSE );
      }
      else if( cnxParam->cnxParmList.send.codecs[0].type == CODEC_CLEARCHANNEL )
      {
         /* For digital mode, fix the jitter buffer at 100ms */
         hdspVhdSetJitterBuffer( cnx->vhdhdl, 1, 100, 100, 100, 100, cnx->ajcCustomReg );

         /* Enable digital mode */
         SetDigitalMode( state, cnx, VRG_TRUE );
      }
   }

   /* Save connection state parameters */
   SaveConnectionOptions( cnx, cnxParam );

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED MODIFY CNX: dsp %d, endpt %d, cnx %d", dspNum, lineId, cnxId ));

   return ( EPSTATUS_SUCCESS );
}

/*****************************************************************************
*  FUNCTION:   ProcessVrgEndptDeleteConnection
*
*  PURPOSE:    Function to delete a connection
*
*  PARAMETERS: endptState - a pointer to endpoint state structure
*              cnxId      - connection identifier
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS ProcessVrgEndptDeleteConnection( ENDPT_STATE *endptState, int cnxId )
{
   VRG_ENDPT*  state;
   CNXSTATE*   cnx;
   int         cnxIndex;
   int         lineId = ((VRG_ENDPT_STATE *) endptState)->lineId;
   VRG_BOOL    digitalMode = VRG_FALSE;

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "DELETE CNX: endpt %d, cnx %d", lineId, cnxId ));

   /*
   ** Get the endpoint state
   */
   state = GetEndptState( lineId );
   if( ! state )
   {
      return(EPSTATUS_ENDPT_UNKNOWN);
   }

   if (cnxId == CNX_UNUSED)
   {
#if FLEXIBLE_CONFERENCING_SUPPORTED
      /* cancel the reservation if there is one */
      for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
      {
         if ((state->cnx[cnxIndex]) && (state->cnx[cnxIndex]->cnxId == CNX_IDLE))
         {
            /* release the idle connection */
            state->cnx[cnxIndex]->cnxId = CNX_UNUSED;
            state->cnx[cnxIndex]->lineId = BAD_ENDPTID;
            state->cnx[cnxIndex] = 0;
         }
      }
#endif
      return(EPSTATUS_SUCCESS);
   }

   /*
   ** Determine the index for the current cnx in the endpoint's cnx queue
   */
   for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
   {
      if ((state->cnx[cnxIndex]) && (state->cnx[cnxIndex]->cnxId == cnxId))
      {
         /* cnx index found */
         break;
      }
   }

   if (cnxIndex == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT)
   {
      /* specified cnx is not within the endpoint's cnx list */
      return ( EPSTATUS_CNX_UNKNOWN );
   }

   /* fetch the cnx state */
   cnx = state->cnx[cnxIndex];

   /* Remove CS call */
   if ( cnx->csCall == VRG_TRUE ) {
     SetRtpCSCallDisable ( cnx );
     cnx->csCall = VRG_FALSE;
     gCSCnxId = -1;
   }

   if (cnx->codecType == CODEC_CLEARCHANNEL)
   {
      digitalMode = VRG_TRUE;
   }

   /* Re-init rtp */
   RtpInit( &cnx->rtpstate );

   /* Disable Digit relay if it was in use */
   if ( EnableRelay( cnx, VRG_FALSE, 0, EPDATAMODE_VBD, EPDTMFRFC2833_DISABLED ))
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT %d: Error returned by enableRelay(), enable: 0",
                  cnx->lineId ));
   }

   /* Disable voice packet encoding/decoding */
   SetVoiceMode( state->cnx[cnxIndex], VM_IDLE );

   /* If we were in digital mode, re-enable DSP services */
   if(digitalMode)
   {
      SetDigitalMode( state, cnx, VRG_FALSE );
   }

   bosSleep(20);

   /* Remove switchboard connections */
   if ( state->endptType == EPTYPE_PSTN  )
   {
      DisConnectPstnGwSB(state->cnx[cnxIndex]->vhdhdl, state->lineVhdHdl);
   }
   else
   {
      UpdateSBMasks( state->cnx[cnxIndex], EPCNXMODE_INACT);
   }

   /* If the lineVhd associated with this connection is internally
    * connected to another line VHD, ensure that there is no
    * switchboard connection between other line VHD and this
    * gateway vhd */
   if ( state->intLinkVhdHdl )
   {
      VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "Unconferencing lineVhdl 0x%0x gwVhdl 0x%x with intlinked lineVhdl 0x%x",
                       state->lineVhdHdl,
                       state->cnx[cnxIndex]->vhdhdl,
                       state->intLinkVhdHdl ));
      hdspRmSwitchBoardConnect( state->cnx[cnxIndex]->vhdhdl,
                                HAPI_SWB_BOS,
                                state->intLinkVhdHdl,
                                HAPI_SWB_TOS,
                                HAPI_SWB_DUPLEX,
                                VRG_FALSE );
   }

   /* Reset build out */
   vrgEndptResetBuildOut( lineId, state->lineVhdHdl );

#if !FLEXIBLE_CONFERENCING_SUPPORTED
   if (cnxIndex != PRIMARY_CNX_INDEX)
#endif
   {
      /* Secondary vhd - remove the cnx state from the endpoint model */
      state->cnx[cnxIndex] = 0;
      cnx->lineId = BAD_ENDPTID;
   }

   /*
   ** Process echo cancellation state machine
   */
   EcanProcessStm(state, ECANEVT_DLCX);


   /* clear cnx parameters */
   cnx->cnxId = CNX_UNUSED;
   ClearConnectionOptions( cnx );

   /* Turning off any ingress signals on this cnx  */
   hdspToneOff( cnx->vhdhdl, INGRESS_TONE );
   cnx->activeSignal = VRG_FALSE;

   if ( state->bGR303Support )
   {
      /* End OSI */
      if ( state->casCtlBits == EP_CAS_CTL_LCFO )
      {
         /* End OSI */
         casCtlSupervisoryDisconnectOff( state->casCtlHandle, VRG_FALSE, state->offHook );

         state->casCtlBits = EP_CAS_CTL_LCF;
      }
      /* Stop ring */
      else if( state->casCtlBits == EP_CAS_CTL_RINGING )
      {
         /* stops ringing */
         casCtlRingOff( state->casCtlHandle, VRG_TRUE );

         state->casCtlBits = EP_CAS_CTL_LCF;
      }
   }

   /* Invalidate the endpt number in the heartbeat module since
   ** the connection has been deleted */
   hbUpdatePatientEndptId( cnx->cnxStateHBId, HBPATIENT_UNKNOWN_ENDPT );

   /* Let the heartbeat module know this cnx is now inactive */
   hbUpdatePatientState( cnx->cnxStateHBId, HBSTATE_INACTIVE );

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "COMPLETED DELETE CNX: endpt %d, cnx %d", lineId, cnxId ));

   return(EPSTATUS_SUCCESS);
}

/*****************************************************************************
*  FUNCTION:   ProcessVrgEndptGetRtpStats
*
*  PURPOSE:    To retrieve RTP stats from RTP service
*
*  PARAMETERS: endptstate         - endpt state
*              cxid               - connection id
*              epRtpStats         - statistics to be populated
*              bReset             - Reset stats after querying
*
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
static EPSTATUS ProcessVrgEndptGetRtpStats( ENDPT_STATE *endptState, int cnxId, EPZCNXSTATS *epRtpStats, VRG_BOOL bReset )
{
   EPSTATUS  epStatus;
   VRG_ENDPT *state;
   CNXSTATE  *cnx;
   int       cnxIndex;
   int       lineId;

   lineId = ((VRG_ENDPT_STATE *) endptState)->lineId;
   epStatus = EPSTATUS_SUCCESS;

   /*
   ** Get the endpoint state
   */
   state = GetEndptState( lineId );
   if( ! state )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: Could not find endpoint for line %d", __FUNCTION__, lineId ));
      return( EPSTATUS_ENDPT_UNKNOWN );
   }

   /* Determine the index for the current cnx in the endpoint's cnx queue */
   for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
   {
      if ((state->cnx[cnxIndex]) && (state->cnx[cnxIndex]->cnxId == cnxId))
      {
         /* cnx index found */
         break;
      }
   }
   if (cnxIndex == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT)
   {
      /* specified cnx is not within the endpoint's cnx list */
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "%s: Could not find connection %d for line %d", __FUNCTION__, cnxId, lineId ));
      return( EPSTATUS_CNX_UNKNOWN );
   }

   /* fetch the cnx state */
   cnx = state->cnx[cnxIndex];

   epStatus = GetConnectionRtpStats(cnx, epRtpStats, bReset);

   return( epStatus );
}

/*****************************************************************************
*  FUNCTION:   ProcessVrgEndptGetT38Stats
*
*  PURPOSE:    To retrieve T38 stats from T38 service
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
static void ProcessVrgEndptGetT38Stats( ENDPT_STATE *endptState, int cnxId,
                                 EPZT38CNXSTATS *epT38Stats )
{
   VRG_SINT16       status;
   VRG_ENDPT        *state;
   int              cnxIndex;
   int              lineId = ((VRG_ENDPT_STATE *) endptState)->lineId;
   HSZFAXRSTATS     netfaxrstats;

   /*
   ** Get the endpoint state
   */
   state = GetEndptState( lineId );
   if( ! state )
   {
      return;
   }

   /* Determine the index for the current cnx in the endpoint's cnx queue */
   for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
   {
      if ((state->cnx[cnxIndex]) && (state->cnx[cnxIndex]->cnxId == cnxId))
      {
         /* cnx index found */
         break;
      }
   }
   if (cnxIndex == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT)
   {
      /* specified cnx is not within the endpoint's cnx list */
      return;
   }

   /* Get the faxr stats from the DSP */
   status = hdspSendCmdData( state->cnx[cnxIndex]->vhdhdl,
                             HAPINET_GETFAXSTATS,
                             HSCMDRESP,
                             sizeof( netfaxrstats ),
                             &netfaxrstats );

   if ( status == HAPISUCCESS )
   {
      /* Keep track of the number of pages sent/received for the life of the cnx.
      ** Note that the DSP only reports the fax stats for the current fax cnx */
      state->cnx[cnxIndex]->t38param.numFaxPagesRx += netfaxrstats.hsxfaxrEgressStats.hsxNumRxPages;
      state->cnx[cnxIndex]->t38param.numFaxPagesTx += netfaxrstats.hsxfaxrIngressStats.hsxNumTxPages;

      /* Report the number of cumulative number of pages sent/received */
      epT38Stats->rxpg = state->cnx[cnxIndex]->t38param.numFaxPagesRx;
      epT38Stats->txpg = state->cnx[cnxIndex]->t38param.numFaxPagesTx;
   }
   else
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "failed to retrieve the faxr stats from the DSP" ));
      epT38Stats->rxpg = 0;
      epT38Stats->txpg = 0;
   }
}

/*****************************************************************************
*  FUNCTION:   ProcessVrgEndptMuteConnection
*
*  PURPOSE:    Function to toggle the mute status of a connection
*
*  PARAMETERS: endptState - pointer to endpoint state structure
*              cnxId      - connection identifier
*              mute       - mute status of the connection
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS ProcessVrgEndptMuteConnection
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   VRG_BOOL       mute
)
{
   VRG_ENDPT        *state;
   CNXSTATE         *cnx;
   int               cnxIndex, lineId;

   lineId = ((VRG_ENDPT_STATE *) endptState)->lineId;

   /*
   ** Get the endpoint state
   */
   state = GetEndptState( lineId );
   if( ! state )
   {
      return ( EPSTATUS_ENDPT_UNKNOWN );
   }

   /* Determine the index for the current cnx in the endpoint's cnx queue */
   for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
   {
      if ((state->cnx[cnxIndex]) && (state->cnx[cnxIndex]->cnxId == cnxId))
      {
         /* cnx index found */
         break;
      }
   }
   if (cnxIndex == VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT)
   {
      /* specified cnx is not within the endpoint's cnx list */
      return ( EPSTATUS_CNX_UNKNOWN );
   }

   /* Fetch the cnx state */
   cnx = state->cnx[cnxIndex];

   /* Set the mute status for the connection. */
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Setting mute status on cnxId %d, endpt %d, value %s",
             cnxId, lineId, ((mute == VRG_TRUE) ? "True" : "False") ));
   //cnx->bMuteAudio = mute;
   /* Set to receive only mode */
   if( mute )
   {
      SetRtpMode( cnx, EPCNXMODE_RCVO );
      ProgramRtpRegisters( cnx );
   }

   return ( EPSTATUS_SUCCESS );
}


/*****************************************************************************
*  FUNCTION:   ProcessVrgEndptLinkInternal
*
*  PURPOSE:    Function to internall link or unlink two endpoints
*
*  PARAMETERS: endptState      - pointer to endpoint state structure
*              otherEndptState - pointer to the other endpt state struct
*              link            - whether or not to link or unlink the endpts
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS ProcessVrgEndptLinkInternal
(
   ENDPT_STATE   *endptState,
   ENDPT_STATE   *otherEndptState,
   VRG_BOOL       link
)
{
   VRG_ENDPT        *state1;
   VRG_ENDPT        *state2;
   int               lineId1, lineId2,i;

   lineId1 = ((VRG_ENDPT_STATE *) endptState)->lineId;
   lineId2 = ((VRG_ENDPT_STATE *) otherEndptState)->lineId;

   /*
   ** Get the endpoint state
   */
   state1 = GetEndptState( lineId1 );
   state2 = GetEndptState( lineId2 );
   if( !state1 || !state2 )
   {
      return ( EPSTATUS_ENDPT_UNKNOWN );
   }

   if( HAPISUCCESS != hdspRmSwitchBoardConnect( state1->lineVhdHdl,
                                                HAPI_SWB_TOS,
                                                state2->lineVhdHdl,
                                                HAPI_SWB_TOS,
                                                HAPI_SWB_DUPLEX,
                                                (link) ) )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR %s line->line VHD switchboard "
                                      "connection from endpt %d to endpt %d",
                     (link) ? "connecting" : "disconnecting",
                     state1->lineId,
                     state2->lineId ));
   }

   /* Update internalLink tracking */
   if( link )
   {
      state1->intLinkVhdHdl = state2->lineVhdHdl;
      state2->intLinkVhdHdl = state1->lineVhdHdl;
   }
   else
   {
      state1->intLinkVhdHdl = 0;
      state2->intLinkVhdHdl = 0;
   }

   /* Internal Link conferencing support */

   /* iterate through all cnxs in the endpoint in conference mode and link to cnx vhd */
   for ( i = 0; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
   {
      /* only connect to other vhd's when the existing cnx is in conference mode */
      if (( state1->cnx[i] )          &&
          ( state1->cnx[i]->mode == EPCNXMODE_CONF ))
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "%s lineVhdl 0x%0x gwVhdl 0x%x with intlinked lineVhdl 0x%x",
                                                                                                   ((link)?"Conferencing":"Unconferencing"),
                                                                                                   state1->lineVhdHdl,
                                                                                                   state1->cnx[i]->vhdhdl,
                                                                                                   state2->lineVhdHdl   ));
         if( HAPISUCCESS != hdspRmSwitchBoardConnect( state1->cnx[i]->vhdhdl,
                                                      HAPI_SWB_BOS,
                                                      state2->lineVhdHdl,
                                                      HAPI_SWB_TOS,
                                                      HAPI_SWB_DUPLEX,
                                                      (link) ) )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR %s line->Cnx VHD conf-internal switchboard "
                                            "connection from endpt %d Cnx 0x%x to endpt %d",
                           (link) ? "connecting" : "disconnecting",
                           state1->lineId,
                           state1->cnx[i]->vhdhdl,
                           state2->lineId ));
         }
      }
   }

   /* iterate through all cnxs in the endpoint */
   for ( i = 0; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
   {
      /* only connect to other vhd's when the existing cnx is in conference mode */
      if (( state2->cnx[i] )          &&
          ( state2->cnx[i]->mode == EPCNXMODE_CONF ))
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "%s lineVhdl 0x%0x gwVhdl 0x%x with intlinked lineVhdl 0x%x",
                                                                                                   ((link)?"Conferencing":"Unconferencing"),
                                                                                                   state2->lineVhdHdl,
                                                                                                   state2->cnx[i]->vhdhdl,
                                                                                                   state1->lineVhdHdl   ));
         if( HAPISUCCESS != hdspRmSwitchBoardConnect( state2->cnx[i]->vhdhdl,
                                                      HAPI_SWB_BOS,
                                                      state1->lineVhdHdl,
                                                      HAPI_SWB_TOS,
                                                      HAPI_SWB_DUPLEX,
                                                      (link) ) )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR %s line->Cnx VHD conf-internal switchboard "
                                            "connection from endpt %d Cnx 0x%x to endpt %d",
                           (link) ? "connecting" : "disconnecting",
                           state2->lineId,
                           state2->cnx[i]->vhdhdl,
                           state1->lineId ));
         }
      }
   }

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   HapiRmEventPostProcess
**
** PURPOSE:    To perform "post-processing" of HAPI events. Currently,
**             this simply consists of logging the events of interest. This
**             should be performed in the context of the endpt event processing
**             task and not the hausware task, so that the hausware task doesn't
**             block on serialized logging requests to the operating system,
**             which would add jitter to outbound packets.
**
** PARAMETERS: hapiEvt  (in)  Event received from hausware.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void HapiRmEventPostProcess( VRG_HAPI_EVT *hapiEvt )
{
   const char *evtStr = NULL;
   BOS_TIME_MS timeStamp = 0;

   switch( hapiEvt->eventId )
   {
      CASE_PROCESS_HAPI_EVT( HAPI_HEC_STATE_EVT                )
      CASE_PROCESS_HAPI_EVT( HAPI_RM_ERROR_EVT                 )
      CASE_PROCESS_HAPI_EVT( HAPI_GAIN_SET_GAIN_CMD            )
      CASE_PROCESS_HAPI_EVT( HAPI_SWB_CONNECT_CMD              )
      CASE_PROCESS_HAPI_EVT( HAPI_SWB_DISCONNECT_CMD           )
      CASE_PROCESS_HAPI_EVT( HAPI_RM_DSP_INGRESS_OVERRUN_EVT   )
      CASE_PROCESS_HAPI_EVT( HAPI_RM_DSP_EGRESS_UNDERRUN_EVT   )

      default:
      {
         evtStr = "Unknown evt";
      }
      break;
   }


   XCHG_ASSERT( evtStr != NULL );

   bosTimeGetMs( &timeStamp );

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "'%s' (0x%x), hdl:0x%x, op1:0x%x, op2:0x%x ts: %u ",
             evtStr,
             hapiEvt->eventId,
             hapiEvt->handle,
             hapiEvt->op1,
             hapiEvt->op2.val,
             timeStamp ));

   /* Free dynamically allocated buffer if extended event data */
   if ( hapiEvt->reqId == HSEVTEXTDATA &&  /* Extended event data */
        hapiEvt->op2.ref != NULL       &&  /* Valid valid data */
        hapiEvt->op1 != 0 )                /* Valid event data size */
   {
      free(hapiEvt->op2.ref);
   }
}

/*
*****************************************************************************
** FUNCTION:   HapiEndptEventProcess
**
** PURPOSE:    Process line VHD events.
**
** PARAMETERS: hapiEvt  (in)  Hausware event.
**
** RETURNS:    none.
**
** NOTE:
*****************************************************************************
*/
static void HapiEndptEventProcess( VRG_HAPI_EVT *hapiEvt )
{
   VRG_ENDPT  *state;
   VRG_BOOL    logEvent;

   /* By default, don't log the event. */
   logEvent = VRG_FALSE;

   state = GetEndptStateFromVHD( hapiEvt->handle );
   if( state == NULL )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT - endptVhdEventCallback: Unknown VHD handle 0x%x, event: 0x%x",
                        hapiEvt->handle, hapiEvt->eventId ));
      return;
   }

   /*
   ** Switch on the events of interest.
   */
   switch( hapiEvt->eventId )
   {
      case HAPI_PTE_TONE_ON_EVT:
      {
         /* Need to extract the lower 8 bits for the tone index since the
          * upper 8 bits represent the power level */
         VRG_UINT8    tone = hapiEvt->op2.val & 0xFF;
         HSZEVT pstnEvt;   /* PSTN Ctl uses HSZEVT to recieve events */

         /* Check if this is a long duration tone.  For a long duration tone,
          * the tone index equals 16 + index of the corresponding short tone */
         if ( tone >= HAPI_PTE_DTMFDMAXDGT )
         {
            /* Notify EPEVT of event */
            (*endptConfig.notify)( state->endptArchive, -1, EPEVT_DTMFL, NULL, 0, -1 );
         }
         else
         {
            const EPZPTEDIGIT2EPEVTMAP *digitMap = digit2epevtMap;

            /* Find the correspondig event for the tone */
            while(( digitMap->digit != tone ) &&
                  ( digitMap->digit != HAPI_PTE_DTMFDDGTERROR ))
            {
               digitMap++;
            }

            if(( digitMap->digit != HAPI_PTE_DTMFDDGTERROR ) && (digitMap->epEvent != EPEVT_NULL))
            {
               /* Notify EPEVT of event */
               if( state->endptType == EPTYPE_PSTN && state->pstnVhdHdl == hapiEvt->handle )
                  ; /* Filter out events from the PSTN VHD since those are already reported
                       by the associated Line VHD.  This architecture may be changed in the
                       future to only have one VHD, then this check should be removed. */
               else
               {
                  if( state->endptType == EPTYPE_PSTN )
                  {
                     pstnEvt.hsxdevhdl = state->pstnVhdHdl;
                     pstnEvt.hsxevt = PSTN_CTL_EVT_DTMF_DIGIT;
                     pstnEvt.hsxop1 = digitMap->digit;
                     pstnEvt.hsxop2.ref = NULL;
                     if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &pstnEvt ) )
                     {
                        VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_DTMF_DIGIT FAIL"));
                     }
                  }
                  (*endptConfig.notify)( state->endptArchive, -1, digitMap->epEvent, NULL, 0, EPDTMFACT_TONEON);
               }
               if( state->endptType != EPTYPE_PSTN )
                  /* process event in DSP proc for Class */
                  ClassEvent( state, hapiEvt );
            }
         }
      }
      break;


      case HAPI_PTE_TONE_OFF_EVT:
      {
         /* Need to extract the lower 8 bits for the tone index since the
          * upper 8 bits represent the power level */
         VRG_UINT8    tone = hapiEvt->op2.val & 0xFF;

         const EPZPTEDIGIT2EPEVTMAP *digitMap = digit2epevtMap;

         /* Find the correspondig event for the tone */
         while(( digitMap->digit != tone ) &&
               ( digitMap->digit != HAPI_PTE_DTMFDDGTERROR ))
         {
            digitMap++;
         }

         if(( digitMap->digit != HAPI_PTE_DTMFDDGTERROR ) && (digitMap->epEvent != EPEVT_NULL))
         {
            /* Notify EPEVT of event */
            (*endptConfig.notify)( state->endptArchive, -1, digitMap->epEvent, NULL, 0, EPDTMFACT_TONEOFF );
         }

         if( state->endptType != EPTYPE_PSTN )
            /* process event in DSP proc for Class */
            ClassEvent( state, hapiEvt );
      }
      break;


      case HAPI_PTE_TONE_GEN_EVT:
      {
         HSZEVT evt;
         logEvent = VRG_TRUE;

         ClassEvent( state, hapiEvt );

         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "ENDPT %d: HAPI_PTE_TONE_GEN_EVT", state->lineId ));
         if( vrgEndptGetNumFxoEndpoints() > 0 )
         {
            evt.hsxdevhdl = state->pstnVhdHdl;
            evt.hsxevt = PSTN_CTL_EVT_DIAL_COMPLETE;
            evt.hsxop1 = state->lineId;
            evt.hsxop2.ref = NULL;
            if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &evt ) )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_DIAL_COMPLETE FAIL"));
            }
         }
      }
      break;


      case HAPI_CLIDTX_MSG_ERROR_EVT:
      case HAPI_CLIDTX_DONE:
      {
         logEvent = VRG_TRUE;

         ClassEvent( state, hapiEvt );
      }
      break;

#ifdef HDSP_CFG_CLIDRX
      case HAPI_CLIDRX_PKT_EVT:
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "ENDPT %d: CLIDRX_PKT_EVT detection event", state->lineId ));
         ClassEvent( state, hapiEvt );
      }
      break;
#endif

      case HAPI_DTAS_DETECT_EVT:
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT,  "EndptVhdEventCallback: DTAS event at the endpoint!"));
      }
      break;

      case HAPI_CMTD_TONE_DET_EVT:
      {
         HSZEVT evt;

         /* Typecasting op1 into a char, and creating a char array */
         unsigned char hsxop1CharArray[] = { (unsigned char)( hapiEvt->op1 & 0x00FF ), '\0' };

         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "ENDPT %d: CMTD tone detection event op1:0x%x",
                  state->lineId, hapiEvt->op1 ));

         if ( vrgEndptGetNumFxoEndpoints() > 0 )
         {
            evt.hsxdevhdl = state->pstnVhdHdl;
            evt.hsxevt = PSTN_CTL_EVT_CPTONE_DETECT;
         }
         else
         {
            evt.hsxdevhdl = state->lineVhdHdl;
            evt.hsxevt = HAPI_CMTD_TONE_DET_EVT;
         }

         evt.hsxop1 = state->lineId;
         evt.hsxop2.ref = hsxop1CharArray;   /* pstnCtlSendEvent expects op2 to be char array */

         if ( vrgEndptGetNumFxoEndpoints() > 0 )
         {
            if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &evt ) )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_CPTONE_DETECT FAIL"));
            }
         }
         else
         {
            CallProgressToneDetect(state, &evt);
            logEvent = VRG_TRUE;
         }
      }
      break;

      case HAPI_CMTD_TONE_END_EVT:
      {
         (*endptConfig.notify)( state->endptArchive, -1, EPEVT_DIALTONE_END, NULL, 0, -1 );
      }
      break;

      case HAPINET_EXCEPTION:
      case HAPI_HEC_STATE_EVT:
      {
         logEvent = VRG_TRUE;
      }
      break;

      default:
      {
      }
      break;
   }

   if ( logEvent )
   {
      /* Perform post processing if required */
      HapiEndptEventPostProcess( hapiEvt );
   }

   /* Free dynamically allocated buffer if extended event data */
   if ( hapiEvt->reqId == HSEVTEXTDATA &&  /* Extended event data */
        hapiEvt->op2.ref != NULL       &&  /* Valid valid data */
        hapiEvt->op1 != 0 )                /* Valid event data size */
   {
      free(hapiEvt->op2.ref);
   }
}

/*
*****************************************************************************
** FUNCTION:   HapiEndptEventPostProcess
**
** PURPOSE:    To perform "post-processing" of HAPI events. Currently,
**             this simply consists of logging the events of interest. This
**             should be performed in the context of the endpt event processing
**             task and not the hausware task, so that the hausware task doesn't
**             block on serialized logging requests to the operating system,
**             which would add jitter to outbound packets.
**
** PARAMETERS: hapiEvt  (in)  Hausware event.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void HapiEndptEventPostProcess( VRG_HAPI_EVT *hapiEvt )
{
   VRG_ENDPT  *state;
   const char *evtStr = NULL;
   BOS_TIME_MS timeStamp = 0;

   state = GetEndptStateFromVHD( hapiEvt->handle );
   if( state == NULL )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT - HapiEndptEventPostProcess: Unknown VHD handle 0x%x, event: 0x%x",
                hapiEvt->handle, hapiEvt->eventId ));
      return;
   }


   switch( hapiEvt->eventId )
   {

      /*
      ** Log these events.
      */
      CASE_PROCESS_HAPI_EVT( HAPI_CMTD_TONE_DET_EVT   )
      CASE_PROCESS_HAPI_EVT( HAPI_CMTD_TONE_END_EVT   )
      CASE_PROCESS_HAPI_EVT( HAPINET_EXCEPTION        )
      CASE_PROCESS_HAPI_EVT( HAPI_CLIDTX_DONE         )
      CASE_PROCESS_HAPI_EVT( HAPI_HEC_STATE_EVT       )
      CASE_PROCESS_HAPI_EVT( HAPI_PTE_TONE_GEN_EVT    )
      CASE_PROCESS_HAPI_EVT( HAPI_HEC_GETSTATE_EVT    )


      default:
      {
         evtStr = "Unknown evt";
      }
      break;
   }

   XCHG_ASSERT( evtStr != NULL );

   bosTimeGetMs( &timeStamp );

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "'%s' (0x%x), line:%d, hdl:0x%x, op1:0x%x, op2:0x%x, ts: %u ",
             evtStr,
             hapiEvt->eventId,
             state->lineId,
             hapiEvt->handle,
             hapiEvt->op1,
             hapiEvt->op2.val,
             timeStamp ));

   if ( hapiEvt->eventId ==  HAPI_PTE_TONE_GEN_EVT )
   {
      /* If the current signal is not a caller ID signal, then update
      ** the heartbeat module of signal completion.  For caller ID
      ** signals, the CLID TX done event will notify heartbeat module
      ** of signal completion. */
      if ( ( state->activeBriefSignal != EPSIG_NULL ) &&
           ( state->activeBriefSignal != EPSIG_CALLID ) )
      {
         hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
         state->activeBriefSignal = EPSIG_NULL;
      }
   }


   if ( hapiEvt->eventId == HAPI_CLIDTX_DONE )
   {
      /* If caller ID is completed, update the heartbeat module of
      ** signal completion */
      if ( state->activeBriefSignal == EPSIG_CALLID )
      {
         hbUpdatePatientState( state->signalCallerIdHBId, HBSTATE_INACTIVE );
         state->activeBriefSignal = EPSIG_NULL;
      }
   }

}

static void NteEventProcess(VRG_NTE_EVT *nteEvent)
{
   /* Get the endpoint state */
   VRG_ENDPT *state = GetEndptState( nteEvent->lineNum );
   if( !state )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Unknown CAS line id %u", nteEvent->lineNum ));
      return;
   }

   /* Begin Hook-Flash Emulation */
   if (state->bGR303Support && state->bPulse2dtmf)
   {
      int cnxIndex = 0;

      /* Determine the index of the current cnx in the endpoint's cnx queue */
      for( ; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
      {
         if ( state->cnx[cnxIndex] )
         {
            /* Update the current hookstate and generate onhook event as required */
            GenerateNteEvent( state->cnx[cnxIndex], nteEvent->id );
         }
      }
   }

   /* Send response */
   bosSemGive( &vrgNteSem );
}

/*
*****************************************************************************
** FUNCTION:   HapiCnxEventProcess
**
** PURPOSE:    Process connection VHD events
**
** PARAMETERS: hapiEvt  (in)  Hausware event.
**
** RETURNS:    none.
**
** NOTE:
*****************************************************************************
*/
static void HapiCnxEventProcess( VRG_HAPI_EVT *hapiEvt )
{
   VRG_ENDPT  *endptState;
   CNXSTATE   *cnx;
   VRG_BOOL    logEvent;
   VRG_UINT16  plcType;

   int data = -1;

   /* By default, don't log the event. */
   logEvent = VRG_FALSE;

   /*
   ** Get the connection state
   */
   cnx = GetCnxStateFromVHD( hapiEvt->handle );
   if ( cnx == NULL )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT - HapiCnxEventProcess: Unknown VHD handle "
                "0x%x, evt: 0x%x, op1: 0x%x, op2: 0x%x",
                hapiEvt->handle, hapiEvt->eventId, hapiEvt->op1, hapiEvt->op2.val ));

      return;
   }
   else if ( cnx->cnxId == CNX_UNUSED )
   {
      VRG_LOG_DBG(( VRG_LOG_MOD_EPT, "ENDPT - HapiCnxEventProcess: Cnx is inactive "
                "0x%x, evt: 0x%x, op1: 0x%x, op2: 0x%x",
                hapiEvt->handle, hapiEvt->eventId, hapiEvt->op1, hapiEvt->op2.val ));

      return;
   }

   endptState = GetEndptState( cnx->lineId );
   if( !endptState )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "cnxVhdEventCallback: ERROR getting state for line: %d", cnx->lineId ));
      return;
   }

   /*
   ** Switch on the events of interest
   */
   switch( hapiEvt->eventId )
   {
      case HAPI_CDIS_HDLCDETECT:
      {
         logEvent = VRG_TRUE;

         /* Process Ecan state machine */
         EcanProcessStm( endptState, ECANEVT_HDLC );

         /* Check if we need to switch to T.38 at a later time.  If so, we
         ** will need to save the current detected event for proper T.38 transition */
         if ( cnx->dataMode == EPDATAMODE_T38_MUTE )
         {
            cnx->lastFaxEvent = EPFAX_EVT_HDLC;
         }

         /* Notify the callclient of detected event */
         if ( ( cnx->dataMode == EPDATAMODE_T38_MUTE ) ||
              ( cnx->dataMode == EPDATAMODE_T38 ) )
         {
            /* For T.38, if we've previously notified for a fax event, we do not
            ** want to notify the fax event again */
            if ( cnx->bNotifyFaxCall == VRG_FALSE )
            {
               (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId, EPEVT_FAX, NULL, 0, -1 );
               cnx->bNotifyFaxCall = VRG_TRUE;
            }
         }
         else
         {
            (*endptConfig.notify)( endptState->endptArchive, -1, EPEVT_FAX, NULL, 0, -1 );
         }
      }
      break;

      case HAPI_CDIS_DETECT:
      {
         EPEVT evt = EPEVT_NULL;
         EPEVTREASON reason = -1;
         int intData = -1;

         logEvent = VRG_TRUE;

         if( hapiEvt->op1 & CDIS_CNG )
         {
            evt = EPEVT_NULL; /* Never send fax event on ingress CNG */
            reason = EPEVTREASON_CNG;

            /* Process Ecan state machine */
            EcanProcessStm(endptState, ECANEVT_CNG);

            /* Check if we need to switch to T.38 at a later time.  If so, we
            ** will need to save the current detected event
            ** for proper T.38 transition */
            if ( cnx->dataMode == EPDATAMODE_T38_MUTE )
            {
               cnx->lastFaxEvent = EPFAX_EVT_CNG_INGRESS;
            }
         }
         else if ( hapiEvt->op1 & CDIS_PHASE_REVERSAL )
         {
            /* Phase reversal answer tone, signal Modem detected */
            evt = EPEVT_MODEM;
            reason = EPEVTREASON_prANS;

            /* Process Ecan state machine */
            EcanProcessStm(endptState, ECANEVT_PHREV);
         }
         else if ( hapiEvt->op1 & CDIS_V18_DETECT )
         {
            if ( hapiEvt->op2.val == CDIS_V18ASCII )
            {
               /* V.18 Ascii (Bell 103) detected, but if it is disabled by MIB, we'll ignore this event */
               if ( cnx->bBell103Enabled == VRG_FALSE )
               {
                  logEvent = VRG_FALSE;
               }
               else
               {
                  /* Turn off adapt in jitter buffer */
                  hdspSendCmdAsync( endptState->lineVhdHdl,
                                    HAPI_CODEC_STOP_ADAPT_CMD,
                                    HSCMDDATA,
                                    1,
                                    0 );
                  /* Process Ecan state machine */
                  EcanProcessStm(endptState, ECANEVT_BELL103);
               }
            }
            else /* if ( evtp->hsxop2.val == CDIS_V18BAUDOT || evtp->hsxop2.val == CDIS_V18PREAMBLE ) */
            {
               /* V.18 tones detected */
               evt = EPEVT_TDD;

#if DISABLE_ECAN_ON_TDD_EVENT
               /* Process Ecan state machine */
               EcanProcessStm(endptState, ECANEVT_V18);
#endif /* DISABLE_ECAN_ON_TDD_EVENT */
            }
            /* Map reason based on tone detected. */
            if( hapiEvt->op2.val == CDIS_V18ASCII )
            {
               reason = EPEVTREASON_BELLTONE;
            }
            else if( hapiEvt->op2.val == CDIS_V18BAUDOT )
            {
               reason = EPEVTREASON_BAUDOT;
            }
            else if( hapiEvt->op2.val == CDIS_V18PREAMBLE )
            {
               reason = EPEVTREASON_V21tone;
            }
            else if( hapiEvt->op2.val == CDIS_V18BAUDOT_SHORT )
            {
               reason = EPEVTREASON_BAUDOT_SHORT;
            }
         }
         else if ( hapiEvt->op1 & CDIS_CUSTOMTONE_DETECT )
         {
            evt = EPEVT_CUSTOMTONE;
            reason = EPEVTREASON_CUSTTONE_INGRESS;
         }
         else if ( hapiEvt->op1 & CDIS_MACHINE )
         {
            /* Answer tone detected, signal Modem detected as per NCS spec */
            evt = EPEVT_MODEM;
            reason = EPEVTREASON_ANS;

            /* Process Ecan state machine */
            EcanProcessStm(endptState, ECANEVT_MACHINE);
         }
         else if ( hapiEvt->op1 & (CDIS_LOW_ENERGY | CDIS_EXT_LOW_ENERGY) )
         {
            /*
            ** Low energy detected on both directions.
            ** Process the Ecan state machine accordingly.
            */
            EcanProcessStm(endptState, ECANEVT_LOWEN);
            if ( hapiEvt->op1 & CDIS_EXT_LOW_ENERGY )
            {
               evt = EPEVT_VBD_STOP;
               intData = (int)EVEVTVDBSTOP_EXTLOWENERGY;
            }
            else
            {
               evt = EPEVT_LOWENERGY;
            }
         }
         else if( hapiEvt->op1 & CDIS_BELL103_DETECT )
         {
            /* V.18 Ascii (Bell 103) detected, but if it is disabled by MIB, we'll ignore this event */
            if ( cnx->bBell103Enabled == VRG_FALSE )
            {
               logEvent = VRG_FALSE;
            }
            else
            {
               /* Turn off adapt in jitter buffer */
               hdspSendCmdAsync( endptState->lineVhdHdl,
                                 HAPI_CODEC_STOP_ADAPT_CMD,
                                 HSCMDDATA,
                                 1,
                                 0 );

               /* Process Ecan state machine */
               EcanProcessStm(endptState, ECANEVT_BELL103);
            }
         }

         if ( evt != EPEVT_NULL )
         {
            /* Notify the callclient of detected event */
            data = reason;
            (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId, evt, (void *)&data, sizeof(data), intData );

            if( (evt == EPEVT_FAX) && VRG_DATA_MODE_T38( cnx->dataMode ) )
            {
               /* Notify the client of an end of VBD call because we detected FAX call.
               */
               (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId, EPEVT_VBD_STOP, NULL, 0, -1 );
            }
         }
      }
      break;


      case HAPI_CDIS_EGRESS_DETECT:
      {
         EPEVT evt = EPEVT_NULL;
         EPEVTREASON reason = -1;
         int intData = -1;
         logEvent = VRG_TRUE;

         if( hapiEvt->op1 & CDIS_CNG )
         {
            reason = EPEVTREASON_EGRESS_CNG;
            evt = (cnx->bCngDetEnabled == VRG_FALSE) ? EPEVT_NULL : EPEVT_FAX;

            /* Calling fax tone detected, process Ecan state machine */
            EcanProcessStm(endptState, ECANEVT_CNG);
         }
         else if ( hapiEvt->op1 & CDIS_PHASE_REVERSAL )
         {
            /* Phase reversal answer tone, signal Modem detected */
            evt = EPEVT_MODEM;
            reason = EPEVTREASON_EGRESS_prANS;

        /* Phase reversal answer tone detected, process Ecan state machine */
            EcanProcessStm(endptState, ECANEVT_PHREV);
         }
         else if ( hapiEvt->op1 & CDIS_MACHINE )
         {
            /* Answer tone detected, signal Modem detected as per NCS spec */
            evt = EPEVT_MODEM;
            reason = EPEVTREASON_EGRESS_ANS;

        /* Answer tone detected, process Ecan state machine */
            EcanProcessStm(endptState, ECANEVT_MACHINE);
         }
         else if ( hapiEvt->op1 & CDIS_V18_DETECT )
         {
            if ( hapiEvt->op2.val == CDIS_V18ASCII )
            {
               /* V.18 Ascii (Bell 103) detected, but if it is disabled by MIB, we'll ignore this event */
               if ( cnx->bBell103Enabled == VRG_FALSE )
               {
                  logEvent = VRG_FALSE;
               }
               else
               {
                  /* Turn off adapt in jitter buffer */
                  hdspSendCmdAsync( endptState->lineVhdHdl,
                                    HAPI_CODEC_STOP_ADAPT_CMD,
                                    HSCMDDATA,
                                    1,
                                    0 );

                  /* Process Ecan state machine */
                  EcanProcessStm(endptState, ECANEVT_BELL103);
               }
            }
            else /* if ( evtp->hsxop2.val == CDIS_V18BAUDOT || evtp->hsxop2.val == CDIS_V18PREAMBLE ) */
            {
               /* V.18 tones detected */
               evt = EPEVT_TDD;
               if( hapiEvt->op2.val == CDIS_V18BAUDOT_SHORT )
               {
                  reason = EPEVTREASON_EGRESS_BAUDOT_SHORT;
               }
            }
         }
         else if ( hapiEvt->op1 & CDIS_CUSTOMTONE_DETECT )
         {
            evt = EPEVT_CUSTOMTONE;
            reason = EPEVTREASON_CUSTTONE_EGRESS;
         }
         else if ( hapiEvt->op1 & (CDIS_LOW_ENERGY | CDIS_EXT_LOW_ENERGY) )
         {
            /*
            ** Low energy detected on both directions.
            ** Process the Ecan state machine accordingly.
            */

            EcanProcessStm(endptState, ECANEVT_LOWEN);
         }
         else if( hapiEvt->op1 & CDIS_BELL103_DETECT )
         {
            /* V.18 Ascii (Bell 103) detected, but if it is disabled by MIB, we'll ignore this event */
            if ( cnx->bBell103Enabled == VRG_FALSE )
            {
               logEvent = VRG_FALSE;
            }
            else
            {
               /* Turn off adapt in jitter buffer */
               hdspSendCmdAsync( endptState->lineVhdHdl,
                                    HAPI_CODEC_STOP_ADAPT_CMD,
                                    HSCMDDATA,
                                    1,
                                    0 );

               /* Process Ecan state machine */
               EcanProcessStm(endptState, ECANEVT_BELL103);
            }
         }

         if ( evt != EPEVT_NULL )
         {
            /* Notify the callclient of detected event */
            data = reason;
            (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId, evt, (void *)&data, sizeof(data), intData );
         }
      }
      break;


      case HAPI_PTE_TONE_ON_EVT:
      case HAPI_PTE_TONE_OFF_EVT:
      {
         /*
         ** Tone events are received on the CNX only when digit relay enabled.
         ** Since the actual tone information is carried in-band in tone packets
         ** there is really nothing to do here. We need to have these listed
         ** here only so that an "unknown event" doesn't get printed out.
         */
      }
      break;
      case HAPI_PTE_TONE_ON_EGRESS_EVT:
      case HAPI_PTE_TONE_OFF_EGRESS_EVT:
      {
         VRG_UINT8    tone;
         VRG_LOG_DBG(( VRG_LOG_MOD_EPT, "ENDPT - cnxVhdEventCallback: endpt 0x%x egress dtmf digit %d eventId %d",
                        hapiEvt->handle, hapiEvt->op2.val & 0xFF, hapiEvt->eventId ));

         /* Need to extract the lower 8 bits for the tone index since the
          * upper 8 bits represent the power level */
         tone = hapiEvt->op2.val & 0xFF;

         /* Check if this is a long duration tone.  For a long duration tone,
          * the tone index equals 16 + index of the corresponding short tone */
         if ( tone >= HAPI_PTE_DTMFDMAXDGT )
         {
            /* Notify EPEVT of event */
            (*endptConfig.notify)( endptState->endptArchive, -1, EPEVT_DTMFL, NULL, 0, -1 );
         }
         else
         {
            const EPZPTEDIGIT2EPEVTMAP *digitMap = digit2epevtMap;

            /* Find the correspondig event for the tone */
            while(( digitMap->digit != tone ) &&
                  ( digitMap->digit != HAPI_PTE_DTMFDDGTERROR ))
            {
               digitMap++;
            }

            if(( digitMap->digit != HAPI_PTE_DTMFDDGTERROR ) && (digitMap->epEvent != EPEVT_NULL))
            {
               /* Notify EPEVT of event */
               if( endptState->endptType == EPTYPE_PSTN && endptState->pstnVhdHdl == hapiEvt->handle )
                  ; /* Filter out events from the PSTN VHD since those are already reported
                       by the associated Line VHD.  This architecture may be changed in the
                       future to only have one VHD, then this check should be removed. */
               else
               {
                  (*endptConfig.notify)( endptState->endptArchive, -1, digitMap->epEvent, NULL, 0,
                                        (hapiEvt->eventId == HAPI_PTE_TONE_ON_EGRESS_EVT)?EPDTMFACT_EGRESS_TONEON:EPDTMFACT_EGRESS_TONEOFF);
               }
            }
         }
      }
      break;

      case HAPI_PTE_TONE_GEN_EVT:
      {
         logEvent = VRG_TRUE;

         /*
         ** Currently, the only activeSignal on connection is RINGBACK.
         ** TONE_GEN_EVT otherwise would indicate that it is an ingress DTMF on connection.
         */
         if ( (cnx->bSBDisconnect == VRG_TRUE) && (cnx->activeSignal == VRG_FALSE) )
         {
            HAPI_PTE_TONEGENEVT *toneGenEvtp;
            toneGenEvtp = (HAPI_PTE_TONEGENEVT *)hapiEvt->op2.ref;

            if (toneGenEvtp->slotNum == 0xFFFF)
            {
               (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId, EPEVT_INGRESS_DTMF, NULL, 0, -1 );
            }
         }
         else
         {
            cnx->activeSignal = VRG_FALSE;
            SetRtpMode( cnx, cnx->mode );
            ProgramRtpRegisters( cnx );
         }
      }
      break;

      case HAPINET_MODE:
      {
         switch (hapiEvt->op1)
         {
            case NETMODE_IDLE:
            case NETMODE_CALLSETUP:
            case NETMODE_DATARELAY:
            {
               cnx->vbData.bEnabled = VRG_FALSE;

               if (cnx->vhdMode == NETMODE_VOICEBANDDATA)
               {
                  /* If we are switching out of VBD mode,
                  ** Revert Tx/Rx gains back to original settings */
                  SetAdditionalGain( cnx, VRG_FALSE );
               }
            }
            break;

            case NETMODE_PACKETVOICE:
            {
               /* If we were in voice band data mode and we are exiting from it,
               ** inform the endpoint about this.
               **
               ** Note that in the current Hausware, exiting from VBD mode puts
               ** us in PacketVoice mode all the time, so there are no other cases
               ** to worry about at this point.
               */
               if( cnx->vbData.bEnabled == VRG_TRUE )
               {
                  data = EPEVTREASON_PTSW;
                  (*endptConfig.notify)( endptState->endptArchive,
                                         cnx->cnxId, EPEVT_VBD_STOP,
                                         (void *)&data, sizeof(data), -1 );
               }

               cnx->vbData.bEnabled = VRG_FALSE;

               /* Revert changes if switching out of VBD mode */
               if (cnx->vhdMode == NETMODE_VOICEBANDDATA)
               {
                  /* Revert Tx/Rx gains back to original settings */
                  SetAdditionalGain( cnx, VRG_FALSE );

                  /* Restore silence suppression to original settings */
                  plcType = vrgEndptRetrievePlcSettings( cnx->codecType );
                  SetSilenceSuppression( cnx, HAPI_GVAD_MODE_TRANSPARENT, plcType, cnx->comfortNoise );
               }
            }
            break;

            case NETMODE_FAXRELAY:
            {
               cnx->vbData.bEnabled = VRG_FALSE;

               if (cnx->vhdMode == NETMODE_VOICEBANDDATA)
               {
                  /* If we are switching out of VBD mode,
                  ** Revert Tx/Rx gains back to original settings */
                  SetAdditionalGain( cnx, VRG_FALSE );
               }
            }
            break;

            case NETMODE_VOICEBANDDATA:
            {
               /* If we were not in voice band data mode and we are entering it,
               ** inform the endpoint about this.
               */
               if( cnx->vbData.bEnabled == VRG_FALSE )
               {
                  data = EPEVTREASON_PTSW;
                  (*endptConfig.notify)( endptState->endptArchive,
                                         cnx->cnxId, EPEVT_VBD_START,
                                         (void *)&data, sizeof(data), -1 );
               }
               cnx->vbData.bEnabled = VRG_TRUE;

               /* Set Tx/Rx gains for VBD mode */
               SetAdditionalGain( cnx, VRG_TRUE );

               /* Disable silence suppression in VBD mode */
               plcType = vrgEndptRetrievePlcSettings( cnx->codecType );
               SetSilenceSuppression( cnx, HAPI_GVAD_MODE_OFF, plcType, cnx->comfortNoise );
            }
            break;

            default:
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR - cnxVhdEventCallback: Unknown mode change event - op1 (0x%x); op2 (0x%x)", hapiEvt->op1, hapiEvt->op2.val ));
               return;
            }
            break;
         }

         /* Log the mode change event */
         logEvent = VRG_TRUE;
      }
      break;


      case HAPI_NET_VOICESTATS_EVT:
      {
         hdspVhdDisplayVoiceStatsRaw( hapiEvt->handle, hapiEvt->op2.ref );
      }
      break;

      case HAPI_FAXR_CALLEND_EVT:
      {
         logEvent = VRG_TRUE;

         /* Generate notification to the callclient based on the received operand */
         if ( hapiEvt->op2.val & HAPI_FAXR_CALL_SUCCESS )
         {
            /* T.38 completed successfully */
            (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId, EPEVT_T38_STOP, NULL, 0, -1 );
         }
         else
         {
            /* T.38 failed */
            (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId, EPEVT_T38_FAILURE, NULL, 0, -1 );
         }
      }
      break;

      /* V.21 fax premable detected during fax relay */
      case HAPI_FAXR_V21_PREAMBLE_EVT:
      {
         logEvent = VRG_TRUE;

         /* For T.38, we only need to report V.21 fax
         ** preamble if CNG was not reported previously */
         if ( cnx->bNotifyFaxCall == VRG_FALSE )
         {
            data = EPEVTREASON_V21flag;
            /* Notify the callclient of detected fax event */
            (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId,
                                EPEVT_FAX, (void *)&data, sizeof(data), -1 );

            if( VRG_DATA_MODE_T38( cnx->dataMode ) )
            {
               /* Notify the client of an end of VBD mode.
               */
               data = EPEVTREASON_V21flag;
               (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId,
                                   EPEVT_VBD_STOP, (void *)&data, sizeof(data), -1 );
            }
         }
      }
      break;

      /* CNG or CED detected during fax relay */
      case HAPI_FAXR_DEBUGRXTONE_EVT:
      {
         logEvent = VRG_TRUE;

         /* Notify CNG */
         if ( hapiEvt->op1 == HSFAXRCNG )
         {
            data = EPEVTREASON_CNG;
            /* Notify the callclient of detected fax event */
            (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId,
                                   EPEVT_FAX, (void *)&data, sizeof(data), -1 );
         }

         /* Notify CED */
         if ( hapiEvt->op1 == HSFAXRCED )
         {
            data = EPEVTREASON_ANS;
            /* Notify the callclient of detected fax event */
            (*endptConfig.notify)( endptState->endptArchive, cnx->cnxId,
                                   EPEVT_MODEM, (void *)&data, sizeof(data), -1 );
         }
      }
      break;

      case HAPI_CODEC_VOICERATE_EVT:
      case HAPINET_VOICERATE:
      case HAPINET_EXCEPTION:
      case HAPINET_HEARTBEAT:
      case HAPI_NET_FAXRATE_EVT:
      case HAPI_NET_FAXSTATS_EVT:
      case HAPI_FAXR_BM_DEBUG_EVT:
      case HAPI_VFAX_NOCARRIER_EVT:
      {
         logEvent = VRG_TRUE;
      }
      break;

      case HAPI_RTP_EGRESS_ERROR_EVT:
      {
         logEvent = VRG_TRUE;
      }
      break;
      case HAPI_RTP_EGRESS_SSRC_COLLISION_EVT:
      {
         logEvent = VRG_TRUE;
      }
      break;
      case HAPI_RTP_EGRESS_DTMF_EVT:
      case HAPI_RTP_INGRESS_RTCP_REPORT_EVT:
      case HAPI_RTP_EGRESS_RTCP_REPORT_EVT:
      case HAPI_RTP_BYE_EVT:
      case HAPI_RTP_SID_INGRESS_EVT:
      {
         logEvent = VRG_FALSE;
      }
      break;

      case HAPI_RTP_THRESHOLD_MONITOR_EVT:
      {
         /* sample code */
         HAPI_RTP_MONITOR_STATE* thresholdEvt = (HAPI_RTP_MONITOR_STATE*) hapiEvt->op2.ref;
         if (thresholdEvt->monitorID == HAPI_RTP_MONITOR_SL || thresholdEvt->monitorID == HAPI_RTP_MONITOR_NL)
         {
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "CNX %d -- ID: %u, zone %u, current %d, threshold %d", cnx->lineId, thresholdEvt->monitorID, thresholdEvt->currentZone, thresholdEvt->currentValue.sValue, thresholdEvt->thresholdValue.sValue));
         } else {
            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "CNX %d -- ID: %u, zone %u, current %u, threshold %u", cnx->lineId, thresholdEvt->monitorID, thresholdEvt->currentZone, thresholdEvt->currentValue.uValue, thresholdEvt->thresholdValue.uValue));
         }

         logEvent = VRG_FALSE;
      }
      break;

      case HAPI_CODEC_EGRESS_VOICE_TO_VBD_DETECT_EVT:
      {
         logEvent = VRG_TRUE;
         data = EPEVTREASON_PTSW;

         /* Notify the client of a start of VBD call.
         */
         (*endptConfig.notify)( endptState->endptArchive,
                                cnx->cnxId, EPEVT_VBD_START,
                                (void *)&data, sizeof(data), -1 );
      }
      break;

      case HAPI_CODEC_EGRESS_VBD_TO_VOICE_DETECT_EVT:
      {
         logEvent = VRG_TRUE;
         data = EPEVTREASON_PTSW;

         /* Notify the client of an end of VBD call.
         */
         (*endptConfig.notify)( endptState->endptArchive,
                                cnx->cnxId, EPEVT_VBD_STOP,
                                (void *)&data, sizeof(data), -1 );
      }
      break;

      default:
      {
      }
      break;
   }

   if ( logEvent )
   {
      /* Perform post processing if required */
      HapiCnxEventPostProcess( hapiEvt );
   }

   /* Free dynamically allocated buffer if extended event data */
   if ( hapiEvt->reqId == HSEVTEXTDATA &&  /* Extended event data */
        hapiEvt->op2.ref != NULL       &&  /* Valid valid data */
        hapiEvt->op1 != 0 )                /* Valid event data size */
   {
      free(hapiEvt->op2.ref);
   }
}

/*
*****************************************************************************
** FUNCTION:   HapiCnxEventPostProcess
**
** PURPOSE:    To perform "post-processing" of HAPI events. Currently,
**             this simply consists of logging the events of interest. This
**             should be performed in the context of the endpt event processing
**             task and not the hausware task, so that the hausware task doesn't
**             block on serialized logging requests to the operating system,
**             which would add jitter to outbound packets.
**
** PARAMETERS: hapiEvt  (in)  Hausware event.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void HapiCnxEventPostProcess( VRG_HAPI_EVT *hapiEvt )
{
   CNXSTATE      *cnx;
   const char    *evtStr = NULL;
   char           evtDescriptionStr[128];
   BOS_TIME_MS    timeStamp = 0;

   evtDescriptionStr[0] = '\0';


   /*
   ** Get the connection state
   */
   cnx = GetCnxStateFromVHD( hapiEvt->handle );
   if ( cnx == NULL )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT - HapiCnxEventPostProcess: Unknown VHD handle "
                "0x%x, evt: 0x%x, op1: 0x%x, op2: 0x%x",
                hapiEvt->handle, hapiEvt->eventId, hapiEvt->op1, hapiEvt->op2.val ));

      return;
   }

   /*
   ** Switch on the events of interest
   */
   switch( hapiEvt->eventId )
   {

      /*
      ** Log these events.
      */
      CASE_PROCESS_HAPI_EVT( HAPINET_MODE                      )
      CASE_PROCESS_HAPI_EVT( HAPINET_EXCEPTION                 )
      CASE_PROCESS_HAPI_EVT( HAPI_CDIS_HDLCDETECT              )
      CASE_PROCESS_HAPI_EVT( HAPI_CDIS_DETECT                  )
      CASE_PROCESS_HAPI_EVT( HAPI_CDIS_EGRESS_DETECT           )
      CASE_PROCESS_HAPI_EVT( HAPINET_HEARTBEAT                 )
      CASE_PROCESS_HAPI_EVT( HAPI_CODEC_VOICERATE_EVT          )
      CASE_PROCESS_HAPI_EVT( HAPINET_VOICERATE                 )
      CASE_PROCESS_HAPI_EVT( HAPI_FAXR_CALLEND_EVT             )
      CASE_PROCESS_HAPI_EVT( HAPI_FAXR_V21_PREAMBLE_EVT        )
      CASE_PROCESS_HAPI_EVT( HAPI_FAXR_DEBUGRXTONE_EVT         )
      CASE_PROCESS_HAPI_EVT( HAPI_NET_FAXRATE_EVT              )
      CASE_PROCESS_HAPI_EVT( HAPI_FAXR_BM_DEBUG_EVT            )
      CASE_PROCESS_HAPI_EVT( HAPI_VFAX_NOCARRIER_EVT           )
      CASE_PROCESS_HAPI_EVT( HAPI_NET_FAXSTATS_EVT             )
      CASE_PROCESS_HAPI_EVT( HAPI_RTP_EGRESS_ERROR_EVT         )
      CASE_PROCESS_HAPI_EVT( HAPI_RTP_EGRESS_SSRC_COLLISION_EVT )
      CASE_PROCESS_HAPI_EVT( HAPI_CODEC_EGRESS_VOICE_TO_VBD_DETECT_EVT )
      CASE_PROCESS_HAPI_EVT( HAPI_CODEC_EGRESS_VBD_TO_VOICE_DETECT_EVT )

      default:
      {
         evtStr = "Unknown evt";
      }
      break;
   }


   if (  ( hapiEvt->eventId == HAPI_CODEC_VOICERATE_EVT  )
      || ( hapiEvt->eventId == HAPINET_VOICERATE         ) )
   {
      VRG_UINT8          codecIndex;
      VRG_UINT8          vadIndex;
      const EP_ALGS_MAP *pAlgsMap = algsMap;

      /* Map the codec name */
      codecIndex = HAPI_PKT_GETVC( hapiEvt->op1 );
      while ( ( pAlgsMap->algsCode != codecIndex ) &&
              ( pAlgsMap->algsCode != HAPI_PKT_VC_VOICE_NOCODE ) )
      {
         pAlgsMap++;
      }

      /* Validate VAD index */
      vadIndex = hapiEvt->op1 >> VPC_VADSHIFT;
      XCHG_ASSERT( vadIndex < HAPI_NUMVADTYPES );

      strPrintf( evtDescriptionStr,
                 sizeof( evtDescriptionStr ),
                 "(%s: %s %s)",
                 hapiEvt->op2.val ? "dec" : "enc",
                 pAlgsMap->algsName,
                 vadtypes[vadIndex] );
   }

   if ( ( hapiEvt->eventId == HAPI_CDIS_HDLCDETECT ) ||
        (( hapiEvt->eventId == HAPI_CDIS_DETECT ) && ( hapiEvt->op1 & CDIS_CNG )
           && ( cnx->bCngDetEnabled == VRG_TRUE )) )
   {
      /* Check if we need to switch to T.38 at a later time.  If so, we
      ** will need to mute the audio immediately. */
      if ( cnx->dataMode == EPDATAMODE_T38_MUTE )
      {
         cnx->bMuteT38 = VRG_TRUE;
      }
   }

   if ( hapiEvt->eventId == HAPINET_MODE )
   {
      switch( hapiEvt->op1 )
      {
         case NETMODE_VOICEBANDDATA:
         {
#ifndef VRG_ENDPT_CFG_G168_TEST
            if ( cnx->silence )
            {
               VRG_UINT16  vad;
               VRG_UINT16  cng;
               VRG_UINT16  plcType;

               plcType = vrgEndptRetrievePlcSettings( cnx->codecType );
               vad = HAPI_GVAD_MODE_OFF;
               cng = cnx->comfortNoise;

               /* Modify the VAD/CNG/PLC settings only if something changed */
               if ((cnx->VadMode != vad) ||
                   (cnx->CngMode != cng) ||
                   (cnx->PlcMode != plcType))
               {
                  hdspVhdSetVadCngPlc( cnx->vhdhdl, vad, cng, plcType );

                  /* Save the new settings for future reference */
                  cnx->VadMode = vad;
                  cnx->CngMode = cng;
                  cnx->PlcMode = plcType;
               }
            }
#endif
         }
         break;

         case NETMODE_PACKETVOICE:
         {
            /* Check if we are switching from VBD mode back to voice mode */
            if ( cnx->vhdMode == NETMODE_VOICEBANDDATA )
            {
#ifndef VRG_ENDPT_CFG_G168_TEST
               /*
               ** Since VBD mode mandates that silence suppression be disabled, we need to
               ** restore the original silence suppression configuration upon exiting
               ** VBD mode.
               */
               if ( cnx->silence )
               {
                  VRG_UINT16  plcType;
                  VRG_UINT16  vad;
                  VRG_UINT16  cng;

                  plcType = vrgEndptRetrievePlcSettings( cnx->codecType );
                  vad = HAPI_GVAD_MODE_TRANSPARENT;
                  cng = cnx->comfortNoise;

                  /* Modify the VAD/CNG/PLC settings only if something changed */
                  if ((cnx->VadMode != vad) ||
                      (cnx->CngMode != cng) ||
                      (cnx->PlcMode != plcType))
                  {
                     hdspVhdSetVadCngPlc( cnx->vhdhdl, vad, cng, plcType );

                     /* Save the new settings for future reference */
                     cnx->VadMode = vad;
                     cnx->CngMode = cng;
                     cnx->PlcMode = plcType;
                  }
               }
#endif
            }
         }
         break;

         default:
         {
            /* do nothing special */
         }
         break;
      }

      /* Save current vhdMode for future processing */
      cnx->vhdMode = hapiEvt->op1;
   }

   if ( hapiEvt->eventId == HAPI_RTP_EGRESS_ERROR_EVT )
   {
      if ( hapiEvt->op1 )
      {
         strPrintf( evtDescriptionStr,
                 sizeof( evtDescriptionStr ),
                 "ERROR" );
      }
   }

   if ( hapiEvt->eventId == HAPI_RTP_EGRESS_SSRC_COLLISION_EVT )
   {
      strPrintf( evtDescriptionStr,
                 sizeof( evtDescriptionStr ),
                 "SSRC (%d)",
                 (hapiEvt->op1 << 16)+(hapiEvt->op2.val) );
   }

   XCHG_ASSERT( evtStr != NULL );

   bosTimeGetMs( &timeStamp );

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "'%s' (0x%x), cnx:%d, line:%d, hdl:0x%x, op1:0x%x, op2:0x%x %s ts: %u ",
             evtStr,
             hapiEvt->eventId,
             cnx->cnxId,
             cnx->lineId,
             hapiEvt->handle,
             hapiEvt->op1,
             hapiEvt->op2.val,
             evtDescriptionStr,
             timeStamp ));

}

#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
/*
*****************************************************************************
** FUNCTION:   TpdEventProcess
**
** PURPOSE:    Event handler for TPD events.
**
** PARAMETERS: tpdEvt - event message
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void TpdEventProcess( VRG_TPD_EVT *tpdEvt )
{
   int length;

   /* Set default length for set operations */
   length = sizeof( VRG_UINT32 );

   switch( tpdEvt->eventId )
   {
      case TPD_CTL_EVENT_TEST_START:
      {
         if ( endptConfig.getProvision != NULL )
         {
            if ( mibSet )
            {
               endptConfig.setProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsStart,
                                         tpdEvt->provValue,
                                         length );
            }
            else
            {
               endptConfig.getProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsStart,
                                         tpdEvt->provValue );
            }
         }
      }
      break;

      case TPD_CTL_EVENT_TEST_STOP:
      {
         if ( endptConfig.setProvision != NULL )
         {
            if ( tpdEvt->mibSet )
            {
               endptConfig.setProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsStop,
                                         tpdEvt->provValue,
                                         length );
            }
            else
            {
               endptConfig.getProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsStop,
                                         tpdEvt->provValue );
            }
         }
      }
      break;

      case TPD_CTL_EVENT_TEST_VALID:
      {
         if ( endptConfig.setProvision != NULL )
         {
            if ( tpdEvt->mibSet )
            {
               endptConfig.setProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsValid,
                                         tpdEvt->provValue,
                                         length );
            }
            else
            {
               endptConfig.getProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsValid,
                                         tpdEvt->provValue );
            }
         }
      }
      break;

      case TPD_CTL_EVENT_TEST_RESULT:
      {
         if ( endptConfig.setProvision != NULL )
         {
            if ( tpdEvt->mibSet )
            {
               endptConfig.setProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsResult,
                                         tpdEvt->provValue,
                                         length );
            }
            else
            {
               endptConfig.getProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsResult,
                                         tpdEvt->provValue );
            }
         }
      }
      break;

      case TPD_CTL_EVENT_TEST_FAILED:
      {
         if ( endptConfig.setProvision != NULL )
         {
            if ( tpdEvt->mibSet )
            {
               length = sizeof( VRG_UINT8 );
               endptConfig.setProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsFault,
                                         tpdEvt->provValue,
                                         length );
            }
         }
      }
      break;

      case TPD_CTL_EVENT_TEST_VALUE:
      {
         if ( endptConfig.setProvision != NULL )
         {
            if ( tpdEvt->mibSet )
            {
               length = strlen( provValue );
               endptConfig.setProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsValue,
                                         tpdEvt->provValue,
                                         length );
            }
            else
            {
               endptConfig.getProvision( tpdEvt->chan,
                                         EPPROV_DiagTestsValue,
                                         tpdEvt->provValue );
            }
         }
      }
      break;

      case TPD_CTL_EVENT_PC_TEST_START:
      {
         if ( endptConfig.getProvision != NULL )
         {
            if ( tpdEvt->mibSet )
            {
               length = sizeof( VRG_UINT8 );
               endptConfig.setProvision( tpdEvt->chan,
                                         EPPROV_PcDiagTestsStart,
                                         tpdEvt->provValue,
                                         length );
            }
            else
            {
               endptConfig.getProvision( tpdEvt->chan,
                                         EPPROV_PcDiagTestsStart,
                                         tpdEvt->provValue );
            }
         }
      }
      break;

      case TPD_CTL_EVENT_PC_TEST_CLEAR:
      {
         if ( endptConfig.setProvision != NULL )
         {
            if ( tpdEvt->mibSet )
            {
               length = sizeof( VRG_UINT8 );
               endptConfig.setProvision( tpdEvt->chan,
                                         EPPROV_PcDiagTestsClear,
                                         tpdEvt->provValue,
                                         length );
            }
            else
            {
               endptConfig.getProvision( tpdEvt->chan,
                                         EPPROV_PcDiagTestsClear,
                                         tpdEvt->provValue );
            }
         }
      }
      break;

      case TPD_CTL_EVENT_PC_TEST_VALID:
      {
         if ( endptConfig.setProvision != NULL )
         {
            if ( tpdEvt->mibSet )
            {
               length = sizeof( VRG_UINT8 );
               endptConfig.setProvision( tpdEvt->chan,
                                         EPPROV_PcDiagTestsValid,
                                         tpdEvt->provValue,
                                         length );
            }
            else
            {
               endptConfig.getProvision( tpdEvt->chan,
                                         EPPROV_PcDiagTestsValid,
                                         tpdEvt->provValue );
            }
         }
      }
      break;

      case TPD_CTL_EVENT_PC_TEST_RESULT:
      {
         if ( endptConfig.setProvision != NULL )
         {
            if ( tpdEvt->mibSet )
            {
               length = sizeof( VRG_UINT8 );
               endptConfig.setProvision( tpdEvt->chan,
                                         EPPROV_PcDiagTestsResult,
                                         tpdEvt->provValue,
                                         length );
            }
            else
            {
               endptConfig.getProvision( tpdEvt->chan,
                                         EPPROV_PcDiagTestsResult,
                                         tpdEvt->provValue );
            }
         }
      }
      break;

      case TPD_CTL_EVENT_TEST_IN_PROGRESS:
      {
         if (endptConfig.setProvision != NULL)
         {
            endptConfig.setProvision( tpdEvt->chan,
                                      EPPROV_LineTestInProgress,
                                      tpdEvt->provValue,
                                      length );
         }
      }
      break;

      case TPD_CTL_EVENT_TEST_DONE:
      {
         if (endptConfig.setProvision != NULL)
         {
            endptConfig.setProvision( tpdEvt->chan,
                                      EPPROV_LineTestDone,
                                      tpdEvt->provValue,
                                      length );
         }
      }
      break;

      default:
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR: Unrecognized TPD event. " ));
      }
      break;
   }

   /* Send response */
   bosSemGive( &vrgTpdSem );
}

#else
/*
*****************************************************************************
** FUNCTION:   MltEventProcess
**
** PURPOSE:    Event handler for MLT events.
**
** PARAMETERS: mltEvt - event message
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void MltEventProcess( VRG_MLT_EVT *mltEvt )
{
   VRG_ENDPT *pState = GetEndptState(mltEvt->portId);
   if (pState == NULL)
   {
      return;
   }

   /* Unconditional clear of Test Mode.
    * Overlapping requests remain unhandled: currently treated as pathalogical (mis)use
    */
   pState->testMode = TESTMODE_NONE;

   (*endptConfig.notify)( pState->endptArchive, -1, EPEVT_MLT_MEASUREMENT, mltEvt->pResult, sizeof(MLT_REPORT), -1 );

   /* Send response */
   bosSemGive( &vrgMltSem );
}

#endif
#endif

/*
*****************************************************************************
** FUNCTION:   CasEventProcess
**
** PURPOSE:    Event handler for CAS events.
**
** PARAMETERS: casEvt - event message
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void CasEventProcess( VRG_CAS_EVT *casEvt )
{
   VRG_ENDPT  *state;
   CLASSSTM_CASEVT_BLOCK BlockEvent;
   VRG_UINT16  endptId = 0xffff;

   XCHG_ASSERT( GetEndptIndexFromCasHandle( casEvt->handle, &endptId ) );

   /*
   ** Get the endpoint state
   */
   state = GetEndptStateFromPhysId( endptId );
   if( ! state )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT ERROR: Unknown line %d !!!", endptId ));
      return;
   }


   BlockEvent = state->classState.evBlock;

   /*
   ** Switch on the events of interest.
   */
   switch( casEvt->eventType )
   {
      case CAS_CTL_DETECT_EVENT:
      {
         VRG_UINT16 event;

         /* Some sort of CAS Detection event has occured */
         event = casEvt->eventId;

         switch(event)
         {
            case CAS_CTL_EVENT_ONHOOK:
            {
               /* Disable corresponding LED */
               xdrvLedCtrl(state->lineId, 0);

               /* Update internal state machine */
               EndptUpdateCasState( state->lineId, EPCAS_ONHOOK );

               if ( BlockEvent != CLASSSTM_CASEVT_BLOCK_ALL)
               {
                  /* Notify upper layer of on-hook event */
                  (*endptConfig.notify)( state->endptArchive, -1, EPEVT_ONHOOK, NULL, 0, -1 );
               }

               if ( state->bGR303Support )
               {
                  /* If there are still connections up then keep SLIC in on-hook
                   * transmit state. */
                  if ( state->cnxCnt > 0 )
                  {
                     casCtlSetState( state->casCtlHandle, CAS_CTL_FXO_ALERTING_OHT );
                  }
               }
            }
            break;

            case CAS_CTL_EVENT_OFFHOOK:
            {
               /* Illuminate corresponding LED */
               xdrvLedCtrl(state->lineId, 1);

               /* Update internal state machine */
               EndptUpdateCasState( state->lineId, EPCAS_OFFHOOK );

               if ( BlockEvent != CLASSSTM_CASEVT_BLOCK_ALL)
               {
                  /* Notify upper layer of off-hook event */
                  (*endptConfig.notify)( state->endptArchive, -1, EPEVT_OFFHOOK, NULL, 0, -1 );
               }
            }
            break;

            case CAS_CTL_EVENT_FLASH:
            {
               /* Update internal state machine */
               EndptUpdateCasState( state->lineId, EPCAS_FLASH );

            if ( BlockEvent != CLASSSTM_CASEVT_BLOCK_ALL)
               {
                  /* Notify upper layer of flash event */
               (*endptConfig.notify)( state->endptArchive, -1, EPEVT_FLASH, NULL, 0, -1 );
               }
            }
            break;

            case CAS_CTL_EVENT_EARLY_OFFHOOK:
            {
               /* Update internal state machine */
               EndptUpdateCasState( state->lineId, EPCAS_EARLY_OFFHOOK );

               if ( BlockEvent != CLASSSTM_CASEVT_BLOCK_ALL)
               {
                  /* Notify upper layer of off-hook event */
                  (*endptConfig.notify)( state->endptArchive, -1, EPEVT_EARLY_OFFHOOK, NULL, 0, -1 );
               }
            }
            break;

            case CAS_CTL_EVENT_EARLY_ONHOOK:
            {
               /* Update internal state machine */
               EndptUpdateCasState( state->lineId, EPCAS_EARLY_ONHOOK );

               if ( BlockEvent != CLASSSTM_CASEVT_BLOCK_ALL)
               {
                  /* Notify upper layer of off-hook event */
                  (*endptConfig.notify)( state->endptArchive, -1, EPEVT_EARLY_ONHOOK, NULL, 0, -1 );
               }
            }
            break;

            case CAS_CTL_EVENT_RINGON:
            {
               EndptUpdateCasState( state->lineId, EPCAS_RINGON );

               if ( BlockEvent != CLASSSTM_CASEVT_BLOCK_ALL )
               {
                  /* Notify upper layer of ring-on event */
                  (*endptConfig.notify)( state->endptArchive, -1, EPEVT_RINGDET, NULL, 0, -1 );
               }
            }
            break;

            case CAS_CTL_EVENT_RINGOFF:
            {
               EndptUpdateCasState( state->lineId, EPCAS_RINGOFF );

               if ( BlockEvent != CLASSSTM_CASEVT_BLOCK_ALL )
               {
                  /* Notify upper layer of ring-off event */
                  (*endptConfig.notify)( state->endptArchive, -1, EPEVT_RINGOFF, NULL, 0, -1 );
               }
            }
            break;

            case CAS_CTL_EVENT_RINGEND:
            {
               EndptUpdateCasState( state->lineId, EPCAS_RINGEND );

               if ( BlockEvent != CLASSSTM_CASEVT_BLOCK_ALL )
               {
                  /* Notify upper layer of ring-off event */
                  (*endptConfig.notify)( state->endptArchive, -1, EPEVT_RINGEND, NULL, 0, -1 );
               }
            }

            case CAS_CTL_EVENT_LCFO:
            {
               /* Remote side has hung up, we will go onhook as well */
               casCtlSetStateFXS(state->casCtlHandle, CAS_CTL_FXS_RELEASED);

               /*
               ** An onhook event should result from this, which will
               ** cause a notify, so don't notify now
               */
            }
            break;
         }
      }
      break;

      case CAS_CTL_STATECTL_EVENT:
      {
         VRG_UINT16 event;

         /* Some sort of CAS Detection event has occured */
         event = casEvt->eventId;

         /* A state event has occured */
         /* These events can be turned off by changing the profiles */
         switch ( event )
         {
            case CAS_CTL_EVENT_FIRST_RINGOFF:
            {
               /* Update internal state machine */
               EndptUpdateCasState( state->lineId, EPCAS_FIRST_RINGOFF );
            }
            break;

            case CAS_CTL_EVENT_OSI_END :
            {
               /* Update internal state machine */
               EndptUpdateCasState( state->lineId, EPCAS_VMWI_OSI_SENT );
            }
            break;

            case CAS_CTL_EVENT_TIMER_EXPIRE :
            {
               /* Queue the event for the caller ID state machine */
               classStmEventProcess( &state->classState, CLASSSTM_TIMER_EXPIRE, state->lineId );
            }
            break;

            case CAS_CTL_EVENT_OFFHOOK:
            {
               /* Update internal state machine */
               EndptUpdateCasState( state->lineId, EPCAS_OFFHOOK );

               /* Notify upper layer of off-hook event */
               (*endptConfig.notify)( state->endptArchive, -1, EPEVT_OFFHOOK, NULL, 0, -1 );
            }
            break;

            case CAS_CTL_EVENT_ONHOOK:
            {
               /* Update internal state machine */
               EndptUpdateCasState( state->lineId, EPCAS_ONHOOK );

               /* Notify upper layer of off-hook event */
               (*endptConfig.notify)( state->endptArchive, -1, EPEVT_ONHOOK, NULL, 0, -1 );
            }
            break;

            default:
            {
            }
            break;
         }
      }
      break;

      case CAS_CTL_PULSEDIAL_EVENT:
      {
         VRG_UINT32 dbLevel, pulseTime, skew;
         const EPZPTEDIGIT2EPEVTMAP *digitMap = digit2epevtMap;
         int i;

         if ( BlockEvent == CLASSSTM_CASEVT_BLOCK_PULSE)
         {
            break;
         }

         /* Pulse digit was completed */
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "Pulse Dial Event, #Pulses: %u, 0x%x",
               casEvt->eventId, state->endptNum ));

         /* Find the correspondig endpoint event for the pulse digit */
         while(( digitMap->casEvent != casEvt->eventId ) &&
               ( digitMap->epEvent != EPEVT_NULL ))
         {
            digitMap++;
         }

         if (( digitMap->casEvent != CAS_CTL_EVENT_NONE ) && (digitMap->epEvent != EPEVT_NULL))
         {
             /* Notify upper layer of pulse dial event */
            (*endptConfig.notify)( state->endptArchive, -1, digitMap->epEvent, NULL, 0, -1 );

            /* Convert the pulse digit into an egress dtmf tone if the conversion is enabled */
            if (state->bPulse2dtmf)
            {
               /* Get the country specific DTMF settings */
               Provision( state->endptArchive, EPPROV_DtmfDbLevel, &dbLevel );
               Provision( state->endptArchive, EPPROV_DtmfPulseTime, &pulseTime );
               Provision( state->endptArchive, EPPROV_DTMFSkew, &skew );

               /* Iterate through the list of cnx for the endpoint to determine the vhdhdl.
               ** Then send the inband DTMF tone to each cnx in use */
               for( i=0; i<VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
               {
                  if ((state->cnx[i]) && (state->cnx[i]->voiceMode != VM_IDLE))
                  {
                     /* Generate ingress signal on cnx */
                     hdspDigitOn( state->lineVhdHdl, state->lineId, digitMap->digit, dbLevel, skew, pulseTime, INGRESS_TONE );
                  }
               }
            }
         }
         else
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT ERROR: No EPEVT for pulsedial=%d !!!", casEvt->eventId));
         }
      }
      break;

      case CAS_CTL_BRIEF_RING_COMPLETE_EVENT:
      {
         /* Let the heartbeat module know this endpt no longer has a signal being applied */
         hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
      }
      break;

      default:
      {
      }
      break;
   }
}

/*
*****************************************************************************
** FUNCTION:   PstnEventProcess
**
** PURPOSE:    Event handler for PSTN events.
**
** PARAMETERS: pstnEvt - event message
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void PstnEventProcess( VRG_PSTN_EVT *pstnEvt )
{
   VRG_ENDPT  *state;

   /*
   ** Get the endpoint state
   */
   state = GetEndptStateFromVHD( pstnEvt->vhdHdl );
   if( !state )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "PstnEventCallback: Unknown vhdHdl %d", pstnEvt->vhdHdl ));
      return;
   }

   switch ( pstnEvt->pEventInfo->event )
   {
      case PSTN_CTL_EVT_CPTONE_TIMEOUT :
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "PstnEventCallback: PSTN_CTL_EVT_CPTONE_TIMEOUT"));
         (*endptConfig.notify)( state->endptArchive, -1, EPEVT_CPTONETIMEOUT, NULL, 0, -1 );
      }
      break;

      case PSTN_CTL_EVT_CALL_SETUP_COMPLETE :
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "PstnEventCallback: PSTN_CTL_EVT_CALL_SETUP_COMPLETE"));
         /*
         * before connect two endpoints from switchboard, check the endpoints offhook state
         * if offhook is not true, than the FXO line have been released before dialing was finished
         * ignore SETUP_COMPLETE event
         */
         if( state->offHook == VRG_TRUE)
         {
            if( (state->isFxsFxoCnxEstablished == VRG_FALSE)
                && (state->routedFxsLineId != state->lineId) )
            {


               if ( ConnectSB( state->lineId, state->routedFxsLineId ) == EPSTATUS_SUCCESS )
               {
                  state->isFxsFxoCnxEstablished = VRG_TRUE;
                  /* Once FXO call is established, configure the caller id RX service for off-hook (type 2) caller id detection */
                  if ( HAPISUCCESS != classRxConfigure( state->pstnVhdHdl, CLASS_MSG_OFFHOOK_CLID, endptConfig.country ) )
                  {
                     VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "classRxConfigure TYPE2 FAIL vhd=0x%x!!!", state->pstnVhdHdl ));
                  }

                  /* Illuminate corresponding LED */
                  xdrvLedCtrl(state->lineId, 1);
               }
               else
               {
                  VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting connection between lines %d and %d", state->lineId, state->routedFxsLineId ));
               }

            }
            else if( state->routedFxsLineId != state->lineId )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR Missing FXS line id for routing over FXO"));
            }
            (*endptConfig.notify)( state->endptArchive, -1, EPEVT_OPCOMPLETE, NULL, 0 , -1);
         }
      }
      break;

      case PSTN_CTL_EVT_VALID_DTMFCLID:
      case PSTN_CTL_EVT_PARTIAL_DTMFCLID:
      {
         VRG_CLIDRX_DATA vrgClidRxData;
         PSTN_CTL_CLID_EVENT_INFO *dtmfClidInfo;

         memset( &vrgClidRxData, 0, sizeof(VRG_CLIDRX_DATA) );
         dtmfClidInfo = (PSTN_CTL_CLID_EVENT_INFO *)pstnEvt->pEventInfo->pInfo;

         /* DTMF CLIDRX ready to be sent */

         /* Copy over the number */
         strncpy( vrgClidRxData.number, dtmfClidInfo->clidData.callingNumber,
                                           sizeof(vrgClidRxData.number) );

         /* If info is available, check it, and block number if necessary. Set the reason
            in the number field also for logging purposes, it will not be used as it will
            be characters not numbers. The reasonNoNumber field itself will be used */

         if( dtmfClidInfo->clidData.infoRecieved == DTMFCLIDRX_INFO_PRIVATE )
         {
            vrgClidRxData.reasonNoNumber = 'P';
            strncpy( vrgClidRxData.number, "Private", sizeof(vrgClidRxData.number) );
         }
         else if( dtmfClidInfo->clidData.infoRecieved == DTMFCLIDRX_INFO_UNAVAILABLE )
         {
            vrgClidRxData.reasonNoNumber = 'O';
            strncpy( vrgClidRxData.number, "Unavailable", sizeof(vrgClidRxData.number) );
         }
         else if( dtmfClidInfo->clidData.infoRecieved == DTMFCLIDRX_INFO_ERROR )
         {
            vrgClidRxData.reasonNoNumber = 'E'; /* Not used by most countries.
                                                   Used by Denmark. */
            strncpy( vrgClidRxData.number, "Error", sizeof(vrgClidRxData.number) );
         }
         else if( dtmfClidInfo->clidData.infoRecieved == DTMFCLIDRX_INFO_NONE )
         {
            if( strlen(dtmfClidInfo->clidData.callingNumber) == 0 )
            {
               /* We have no calling number and no reason, just mark as unavailable */
               vrgClidRxData.reasonNoNumber = 'O';
               strncpy( vrgClidRxData.number, "Unavailable", sizeof(vrgClidRxData.number) );
            }
         }

         if( pstnEvt->pEventInfo->event == PSTN_CTL_EVT_VALID_DTMFCLID )
         {
            VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "PstnEventCallback: PSTN_CTL_EVT_VALID_DTMFCLID"));
         }
         else
         {
            VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "PstnEventCallback: PSTN_CTL_EVT_PARTIAL_DTMFCLID"));
         }
         (*endptConfig.notify)( state->endptArchive, -1, EPEVT_DTMF_CLIDRX, (void *)&vrgClidRxData,
                                sizeof(vrgClidRxData), 0 );
      }
      break;

      case PSTN_CTL_EVT_INVALID_DTMFCLID:
      {
         /* Dont send up any event, as we have nothing useful */
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "PstnEventCallback: PSTN_CTL_EVT_INVALID_DTMFCLID"));
      }
      break;

      default:
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "PstnEventCallback: Unexpected Event" ));
         break;
   }

   /* Send response */
   bosSemGive( &vrgPstnSem );
}

/*****************************************************************************
*  FUNCTION:   VrgEventProcess
*
*  PURPOSE:    Event handler for various VRG events
*
*  PARAMETERS: evtp   - event message
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the VRGDISP task
*
*****************************************************************************/
void VrgEventProcess( VRG_EVT *vrgEvt )
{
   switch ( vrgEvt->type )
   {
      case ( VRG_EVT_TYPE_CAS        ) : CasEventProcess( &vrgEvt->evt.cas         ); break;
      case ( VRG_EVT_TYPE_HAPI_RM    ) : HapiRmEventPostProcess( &vrgEvt->evt.hapi ); break;
      case ( VRG_EVT_TYPE_HAPI_ENDPT ) : HapiEndptEventProcess( &vrgEvt->evt.hapi  ); break;
      case ( VRG_EVT_TYPE_HAPI_CNX   ) : HapiCnxEventProcess(&vrgEvt->evt.hapi     ); break;
      case ( VRG_EVT_TYPE_NTE        ) : NteEventProcess( &vrgEvt->evt.nte         ); break;
      case ( VRG_EVT_TYPE_PSTN       ) : PstnEventProcess( &vrgEvt->evt.pstn       ); break;
#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
      case ( VRG_EVT_TYPE_DECT       ) : DectEventProcess( &vrgEvt->evt.dect       ); break;
#endif

#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
      case ( VRG_EVT_TYPE_TPD        ) : TpdEventProcess( &vrgEvt->evt.tpd         ); break;
#else
      case ( VRG_EVT_TYPE_MLT        ) : MltEventProcess( &vrgEvt->evt.mlt         ); break;
#endif
#endif
      default                          : XCHG_ASSERT( 0                            ); break;
   }
}

/*****************************************************************************
*  FUNCTION:   VrgEptCmdProcess
*
*  PURPOSE:    Handler for VRG endpoint API and console commands
*
*  PARAMETERS: evtp   - event message
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the VRGDISP task
*
*****************************************************************************/
void VrgEptCmdProcess( VRG_EPT_CMD *vrgEptCmd )
{
   switch ( vrgEptCmd->command )
   {
      case ( VRG_EPT_CREATE ):
      {
         vrgEptCmd->status = ProcessVrgEndptCreate((int)vrgEptCmd->arguments[0], (int)vrgEptCmd->arguments[1],
                                              (VRG_ENDPT_STATE*)vrgEptCmd->arguments[2]);
      }
      break;

      case ( VRG_EPT_DESTROY ):
      {
         vrgEptCmd->status = ProcessVrgEndptDestroy((VRG_ENDPT_STATE*)vrgEptCmd->arguments[0]);
      }
      break;

      case ( VRG_EPT_CREATE_CNX ):
      {
         vrgEptCmd->status = ProcessVrgEndptCreateConnection((ENDPT_STATE*)vrgEptCmd->arguments[0], (int)vrgEptCmd->arguments[1],
                                                        (EPZCNXPARAM*)vrgEptCmd->arguments[2]);
      }
      break;

      case ( VRG_EPT_DELETE_CNX ):
      {
         vrgEptCmd->status = ProcessVrgEndptDeleteConnection((ENDPT_STATE*)vrgEptCmd->arguments[0], (int)vrgEptCmd->arguments[1]);
      }
      break;

      case ( VRG_EPT_MODIFY_CNX ):
      {
         vrgEptCmd->status = ProcessVrgEndptModifyConnection((ENDPT_STATE*)vrgEptCmd->arguments[0], (int)vrgEptCmd->arguments[1],
                                                        (EPZCNXPARAM*)vrgEptCmd->arguments[2]);
      }
      break;

      case ( VRG_EPT_MUTE_CNX ):
      {
         vrgEptCmd->status = ProcessVrgEndptMuteConnection((ENDPT_STATE*)vrgEptCmd->arguments[0], (int)vrgEptCmd->arguments[1],
                                                      (VRG_BOOL)vrgEptCmd->arguments[2]);
      }
      break;

      case ( VRG_EPT_LINK_INT ):
      {
         vrgEptCmd->status = ProcessVrgEndptLinkInternal( (ENDPT_STATE*)vrgEptCmd->arguments[0],
                                                          (ENDPT_STATE*)vrgEptCmd->arguments[1],
                                                          (VRG_BOOL)vrgEptCmd->arguments[2] );
      }
      break;

      case ( VRG_EPT_CAPABILITIES ):
      {
         vrgEptCmd->status = ProcessVrgEndptCapabilities((ENDPT_STATE*)vrgEptCmd->arguments[0], (EPZCAP*)vrgEptCmd->arguments[1]);
      }
      break;

      case ( VRG_EPT_SIGNAL ):
      {
         vrgEptCmd->status = ProcessVrgEndptSignal((ENDPT_STATE*)vrgEptCmd->arguments[0], (int)vrgEptCmd->arguments[1],
                                              *((EPSIG*)vrgEptCmd->arguments[2]), (unsigned int)vrgEptCmd->arguments[3],
                                              (int)vrgEptCmd->arguments[4], (int)vrgEptCmd->arguments[5], (int)vrgEptCmd->arguments[6]);
      }
      break;

      case ( VRG_EPT_GET_RTP_STATS ):
      {
         vrgEptCmd->status = ProcessVrgEndptGetRtpStats((ENDPT_STATE*)vrgEptCmd->arguments[0], (int)vrgEptCmd->arguments[1],
                                                   (EPZCNXSTATS*)vrgEptCmd->arguments[2],(VRG_BOOL)vrgEptCmd->arguments[3]);
      }
      break;

      case ( VRG_EPT_GET_T38_STATS ):
      {
         ProcessVrgEndptGetT38Stats((ENDPT_STATE*)vrgEptCmd->arguments[0], (int)vrgEptCmd->arguments[1],
                               (EPZT38CNXSTATS*)vrgEptCmd->arguments[2]);
      }
      break;

#if defined (NTR_SUPPORT)
      case ( VRG_EPT_NTR_CMD ):
      {
         ProcessVrgEndptNtrCmd((ENDPT_STATE*)vrgEptCmd->arguments[0], (VRG_UINT8)vrgEptCmd->arguments[1],
                          (VRG_UINT8)vrgEptCmd->arguments[2], (VRG_SINT32)vrgEptCmd->arguments[3],
                          (VRG_UINT32*)vrgEptCmd->arguments[4], (VRG_UINT32*)vrgEptCmd->arguments[5],
                          (VRG_UINT32*)vrgEptCmd->arguments[6], (VRG_UINT32*)vrgEptCmd->arguments[7]);
      }
      break;
#endif

      case ( VRG_EPT_CONSOLE_CMD ):
      {
         vrgEptCmd->status = ProcessVrgEndptConsoleCmd((ENDPT_STATE*)vrgEptCmd->arguments[0], *((EPCONSOLECMD*)vrgEptCmd->arguments[1]),
                                                  (EPCMD_PARMS*)vrgEptCmd->arguments[2]);
      }
      break;

      default:
      {
         XCHG_ASSERT(0);
      }
      break;
   }

   /* Send response */
   bosSemGive( &vrgEptApiSem );
}
