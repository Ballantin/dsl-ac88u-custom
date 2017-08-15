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
*    Filename: vrgEndptHelpers.c
*
****************************************************************************
*    Description:
*
*      This file contains all the endpoint helper functions.
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
#include <hapi_net.h>                  /* Net VHD API */
#include <hapi_pte.h>                  /* PTE Service */
#include <hapi_t38.h>                  /* FXR Service */
#include <hapi_faxr.h>                 /* FXR Service */
#include <hapi_cdis.h>                 /* CDIS Service */
#include <hapi_clidtx.h>               /* CLIDTX Service */
#include <hapi_rtp.h>                  /* RTP Service */
#include <hapi_v21.h>                  /* V21 Service */
#include <hapi_line.h>                 /* Line Service */
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
#include <hapi_aec.h>
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
** CONSTANTS
*****************************************************************************
*/

/* Define the default packet time constant for all codecs */
#if VRG_ENDPT_PACKET_TIME_CHECK
#   define VRG_ENDPT_PACKET_TIME_ALL_DEFAULT     20
#   define VRG_ENDPT_PACKET_TIME_G723_DEFAULT    30
#endif // VRG_ENDPT_PACKET_TIME_CHECK

/* Default T.38 redundancy value:
**    - redundancy level of 4 for T.30 control messages
**    - redundancy level of 1 for T.4 phase C data
***/
#define  T38_DEFAULT_REDUNDANCY     0x0104

/* Minimum and maximum gain settings (dB) */
#define EP_MIN_GAIN -96
#define EP_MAX_GAIN  32

#define VRG_ENDPT_PRINT_RTP_PAYLOADMAPS   0

#define RED_DEFAULT_PAYLOAD 129
#define NTE_DEFAULT_PAYLOAD 128

/* Default frame size for Line VHDs associated with Narrowband HAL devices */
#ifdef BRCM_NONDIST_BUILD
#define NARROWBAND_FRAME_SIZE 80
#else
#define NARROWBAND_FRAME_SIZE 40
#endif


/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/

typedef struct EPFAX_EVT_MAP
{
   VRG_UINT16     faxLastEvt;
   VRG_UINT16     faxInitMode;

} EPFAX_EVT_MAP;

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

static const EPZRATEMAP epRateMap[] =
{
   { CODEC_PCMU,            HAPI_PKT_VC_G711_ULAW        },
   { CODEC_PCMA,            HAPI_PKT_VC_G711_ALAW        },
   { CODEC_G722_MODE_1,     HAPI_PKT_VC_G722_64_MODE_1   },
   { CODEC_G726_16,         HAPI_PKT_VC_G726_16          },
   { CODEC_G726_24,         HAPI_PKT_VC_G726_24          },
   { CODEC_G726_32,         HAPI_PKT_VC_G726_32          },
   { CODEC_G726_40,         HAPI_PKT_VC_G726_40          },
   { CODEC_G7231_53,        HAPI_PKT_VC_G7231_53         },
   { CODEC_G7231_63,        HAPI_PKT_VC_G7231_63         },
   { CODEC_G7231A_53,       HAPI_PKT_VC_G7231_53         },
   { CODEC_G7231A_63,       HAPI_PKT_VC_G7231_63         },
   { CODEC_G7231_VAR,       HAPI_PKT_VC_G7231_VARIABLE   },
   { CODEC_G7231_53_VAR,    HAPI_PKT_VC_G7231_VARIABLE   },
   { CODEC_G7231_63_VAR,    HAPI_PKT_VC_G7231_VARIABLE   },
   { CODEC_G729A,           HAPI_PKT_VC_G729A_8          },
   { CODEC_G729B,           HAPI_PKT_VC_G729A_8          },
   { CODEC_G728,            HAPI_PKT_VC_G728_16          },
   { CODEC_G729,            HAPI_PKT_VC_G729_8           },
   { CODEC_G729E,           HAPI_PKT_VC_G729_118         },
   { CODEC_BV16,            HAPI_PKT_VC_BV16             },
   { CODEC_BV32,            HAPI_PKT_VC_BV32             },
   { CODEC_LINPCM64,        HAPI_PKT_VC_LINPCM64         },
   { CODEC_LINPCM128,       HAPI_PKT_VC_LINPCM128        },
   { CODEC_LINPCM256,       HAPI_PKT_VC_LINPCM256        },
   { CODEC_CLEARCHANNEL,    HAPI_PKT_VC_LINPCM64         },
   { CODEC_ILBC_20,         HAPI_PKT_VC_ILBC20           },
   { CODEC_ILBC_30,         HAPI_PKT_VC_ILBC30           },
   { CODEC_GSMAMR_475,      HAPI_PKT_VC_GSM_AMR_475      },
   { CODEC_GSMAMR_515,      HAPI_PKT_VC_GSM_AMR_515      },
   { CODEC_GSMAMR_590,      HAPI_PKT_VC_GSM_AMR_590      },
   { CODEC_GSMAMR_670,      HAPI_PKT_VC_GSM_AMR_670      },
   { CODEC_GSMAMR_740,      HAPI_PKT_VC_GSM_AMR_740      },
   { CODEC_GSMAMR_795,      HAPI_PKT_VC_GSM_AMR_795      },
   { CODEC_GSMAMR_10K,      HAPI_PKT_VC_GSM_AMR_102      },
   { CODEC_GSMAMR_12K,      HAPI_PKT_VC_GSM_AMR_122      },
   { CODEC_GSMAMR_OA_475,   HAPI_PKT_VC_GSM_AMR_OA_475   },
   { CODEC_GSMAMR_OA_515,   HAPI_PKT_VC_GSM_AMR_OA_515   },
   { CODEC_GSMAMR_OA_590,   HAPI_PKT_VC_GSM_AMR_OA_590   },
   { CODEC_GSMAMR_OA_670,   HAPI_PKT_VC_GSM_AMR_OA_670   },
   { CODEC_GSMAMR_OA_740,   HAPI_PKT_VC_GSM_AMR_OA_740   },
   { CODEC_GSMAMR_OA_795,   HAPI_PKT_VC_GSM_AMR_OA_795   },
   { CODEC_GSMAMR_OA_10K,   HAPI_PKT_VC_GSM_AMR_OA_102   },
   { CODEC_GSMAMR_OA_12K,   HAPI_PKT_VC_GSM_AMR_OA_122   },
   { CODEC_GSMEFR,          HAPI_PKT_VC_GSM_EFR_122      },
   { CODEC_GSMFR,           HAPI_PKT_VC_GSM_FR_13        },
   { CODEC_GSMHR,           HAPI_PKT_VC_GSM_HR_56        },
   { CODEC_AMRWB_66,        HAPI_PKT_VC_WB_AMR_660       },
   { CODEC_AMRWB_885,       HAPI_PKT_VC_WB_AMR_885       },
   { CODEC_AMRWB_1265,      HAPI_PKT_VC_WB_AMR_1265      },
   { CODEC_AMRWB_1425,      HAPI_PKT_VC_WB_AMR_1425      },
   { CODEC_AMRWB_1585,      HAPI_PKT_VC_WB_AMR_1585      },
   { CODEC_AMRWB_1825,      HAPI_PKT_VC_WB_AMR_1825      },
   { CODEC_AMRWB_1985,      HAPI_PKT_VC_WB_AMR_1985      },
   { CODEC_AMRWB_2305,      HAPI_PKT_VC_WB_AMR_2305      },
   { CODEC_AMRWB_2385,      HAPI_PKT_VC_WB_AMR_2385      },
   { CODEC_AMRWB_OA_66,     HAPI_PKT_VC_WB_AMR_OA_660    },
   { CODEC_AMRWB_OA_885,    HAPI_PKT_VC_WB_AMR_OA_885    },
   { CODEC_AMRWB_OA_1265,   HAPI_PKT_VC_WB_AMR_OA_1265   },
   { CODEC_AMRWB_OA_1425,   HAPI_PKT_VC_WB_AMR_OA_1425   },
   { CODEC_AMRWB_OA_1585,   HAPI_PKT_VC_WB_AMR_OA_1585   },
   { CODEC_AMRWB_OA_1825,   HAPI_PKT_VC_WB_AMR_OA_1825   },
   { CODEC_AMRWB_OA_1985,   HAPI_PKT_VC_WB_AMR_OA_1985   },
   { CODEC_AMRWB_OA_2305,   HAPI_PKT_VC_WB_AMR_OA_2305   },
   { CODEC_AMRWB_OA_2385,   HAPI_PKT_VC_WB_AMR_OA_2385   },
   { CODEC_NTE,             0                            },
   { CODEC_RED,             0                            },
   { CODEC_NULL,            0                            }
};

/*
** Identify the initial faxr mode based on the last detected event
*/
static const EPFAX_EVT_MAP epFaxEvtMap[] =
{
   { EPFAX_EVT_CNG_INGRESS,   HSFAXRCNGDET },
   { EPFAX_EVT_HDLC,          HSFAXRRXHDLC },
   { EPFAX_EVT_CDIS_EGRESS,   HSFAXRNORMAL },
   { EPFAX_EVT_NONE,          0 }
};

static VRG_UINT32 net_rand_seed = 152L;

/*
*****************************************************************************
** LOCAL FUNCTION PROTOTYPES
*****************************************************************************
*/

static void EcanStmInit(VRG_ENDPT *endptState);
static VRG_UINT32 net_random(void);
static void ResetVBDataState(EPZVBDATASTATE *vbData);
static void checkG7231var(CODECCNXLIST *codecList);
#if VRG_ENDPT_PACKET_TIME_CHECK
static VRG_UINT16 checkPacketTime( EPZCNXPARAM *pConParm );
#endif // VRG_ENDPT_PACKET_TIME_CHECK

void GetRtpRegisters(CNXSTATE *cnx);
void ProgramRtpRegisters(CNXSTATE *cnx);
void SetRtcpInterval( CNXSTATE *cnx, VRG_UINT16 intMs );
void SetSilenceSuppression( CNXSTATE *cnx, VRG_UINT16 vad, VRG_UINT16 plc, VRG_UINT16 comfortnoise );


/*
*****************************************************************************
** EXTERNS
*****************************************************************************
*/
extern void lhapiHalPcmNoCompress( XDRV_UINT16 chan, XDRV_BOOL value );


/*****************************************************************************
*  FUNCTION:   GetRtpRegisters
*
*  PURPOSE:    Helper function to get RTP registers for a given connection and
*                 store them in the connection structure
*
*  PARAMETERS: cnx - pointer to the appropriate connection structure
*
*  RETURNS:    nothing
*
*  NOTE:
*
*****************************************************************************/
void GetRtpRegisters(CNXSTATE *cnx)
{
   HAPI_RTP_REGS     rtpRegs;
   HAPI_RTP_REGS2    rtpRegs2;
   VRG_SINT16        status;

   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Getting RTP service registers for line %d", cnx->lineId ));

   status = hdspSendCmdData( cnx->vhdhdl,
                             HAPINET_RTP_GETREGS,
                             HSCMDRESP,
                             sizeof(rtpRegs),
                             &rtpRegs );
   if( status )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "failed setting RTP registers for line %d", cnx->lineId ));
   }

   status = hdspSendCmdData( cnx->vhdhdl,
                             HAPINET_RTP_GETREGS2,
                             HSCMDRESP,
                             sizeof(rtpRegs2),
                             &rtpRegs2 );
   if( status )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "failed setting RTP registers for line %d", cnx->lineId ));
   }

   cnx->rtpRegs = rtpRegs;
   cnx->rtpRegs2 = rtpRegs2;
}

/*****************************************************************************
*  FUNCTION:   ProgramRtpRegisters
*
*  PURPOSE:    Helper function to program RTP registers stored in a
*              given connection structure
*
*  PARAMETERS: cnx - pointer to the appropriate connection structure
*
*  RETURNS:    nothing
*
*  NOTE:
*
*****************************************************************************/
void ProgramRtpRegisters(CNXSTATE *cnx)
{
   HAPI_RTP_REGS     rtpRegs;
   HAPI_RTP_REGS2    rtpRegs2;
   VRG_SINT16        status;

   rtpRegs = cnx->rtpRegs;
   rtpRegs2 = cnx->rtpRegs2;

   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Programming RTP service registers for line %d", cnx->lineId ));

   /* Set RTP registers */
   status = hdspSendCmdDataAsync( cnx->vhdhdl,
                             HAPINET_RTP_SETREGS,
                             HSCMDEXTDATA_ASYNC,
                             sizeof( rtpRegs ),
                             &rtpRegs);
   if( status )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "failed setting RTP registers for line %d", cnx->lineId ));
   }

   status = hdspSendCmdDataAsync( cnx->vhdhdl,
                         HAPINET_RTP_SETREGS2,
                         HSCMDEXTDATA_ASYNC,
                         sizeof( rtpRegs2 ),
                         &rtpRegs2);

   if( status )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "failed setting RTP registers2 for line %d", cnx->lineId ));
   }
}

/*
*****************************************************************************
** EXTERNS
*****************************************************************************
*/

/*****************************************************************************
*  FUNCTION:   CallProgressToneDetect
*
*  PURPOSE:    Helper function processes CMTD Event
*
*  PARAMETERS: pState - endpoint state pointer
*              evtp   - HAPI event interface pointer
*
*  RETURNS:    nothing
*
*  NOTE:
*
*****************************************************************************/
void CallProgressToneDetect(VRG_ENDPT *pState, HSZEVT *pEvt)
{
   CMTD_FILTER_INDEX filterIndex = CMTD_FILTER_MAX;
   CMTD_TONE_TYPE detectedTone;

   if ( pEvt->hsxop2.ref != NULL )
   {
      /* Convert op2 from char to CMTD_FILTER_INDEX */
      filterIndex = ( CMTD_FILTER_INDEX )( *( unsigned char* )pEvt->hsxop2.ref );
   }

   /* Determine which tone triggered the detection event */
   detectedTone = cmtdCtlGetConfigTone( filterIndex );
   if (detectedTone == CMTD_TONE_TYPE_DIAL)
   {
      (*endptConfig.notify)( pState->endptArchive, -1, EPEVT_DIALTONE, NULL, 0, -1 );
   }
}

/*****************************************************************************
*  FUNCTION:   ClassEvent
*
*  PURPOSE:    This function processes Class1 and Class2 service related events
*              from the DSP.
*
*  PARAMETERS: state - pointer to endpoint state
*              evtp  - pointer to event info
*
*  RETURNS:    Nothing
*
*  NOTE:
*****************************************************************************/
void ClassEvent( VRG_ENDPT* state, VRG_HAPI_EVT* evtp )
{
   switch( evtp->eventId )
   {
      /* Tone detected */
      case HAPI_PTE_TONE_ON_EVT:
      {
         /* need to extract the lower 8 bits for the tone index since the
          * upper 8 bits represent the power level */
         VRG_UINT8    tone = evtp->op2.val & 0xFF;

         /* check if this is a caller ID ACK from the Caller ID Box */
         if (( tone == DTMFDDGTD ) || ( tone == DTMFDDGTA ))
         {
            /* Update class2 state machine */
            classStmEventProcess( &state->classState, CLASSSTM_CLID_ACK_RCVD, state->lineId );
         }
      }
      break;

      /* Tone off detected */
      case HAPI_PTE_TONE_OFF_EVT:
      {
      }
      break;


      case HAPI_PTE_TONE_GEN_EVT:
      {
         HAPI_PTE_TONEGENEVT *toneGenEvtp;
         toneGenEvtp = (HAPI_PTE_TONEGENEVT *)evtp->op2.ref;

         /*
         ** HAPI_PTE_TONE_GEN_EVT is generated for the following reasons:
         **    - A dialstring generation has completed.
         **    - A custom tone profile generation has completed.
         **    - A custom tone profile overall timeout has expired.
         **    - A custom tone profile state containing HAPI_PTE_CADBIT_EVENT completed.
         */
         if (( toneGenEvtp->slotNum == HAPI_PKT_TC_TONE_NOCODE ) ||
             ( toneGenEvtp->slotNum == 0 ))
         {
            /*
            ** We are currently only interested in events generated in response
            ** to custom tone profile states that contain HAPI_PTE_CADBIT_EVENT
            ** (indicated by slotNum = 0) or events that are in response to the
            ** completion of a dialstring (slotNum = HAPI_PKT_TC_TONE_NOCODE).
            **
            ** Notify the CLASS state machine of tone profile events.
            */
            classStmEventProcess( &state->classState, CLASSSTM_TONE_GEN_EVT, state->lineId );
         }
      }
      break;

      case HAPI_CLIDTX_DONE:
      {
         /* Post the clidtx done event to the clid state machine */
         classStmEventProcess( &state->classState, CLASSSTM_CLID_TX_END, state->lineId );
      }
      break;

#ifdef HDSP_CFG_CLIDRX
      case HAPI_CLIDRX_PKT_EVT:
      {
         VRG_CLIDRX_DATA vrgClidRxData;
         memset( &vrgClidRxData, 0, sizeof(VRG_CLIDRX_DATA) );

         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "ENDPT %d: ClassEvent HAPI_CLIDRX_PKT_EVT", state->endptNum ));

         if ( hdspClidRxParse( endptConfig.country, ( HSZCLIDRXPACKET * )evtp->op2.ref, &state->clidRxState ) )
         {
            /*CLIDRX packet disassembly complete */
            strncpy( vrgClidRxData.name, state->clidRxState.clidRxData.name, sizeof(vrgClidRxData.name) );
            strncpy( vrgClidRxData.number, state->clidRxState.clidRxData.number, sizeof(vrgClidRxData.number) );
            strncpy( vrgClidRxData.date, state->clidRxState.clidRxData.date, sizeof(vrgClidRxData.date) );

            vrgClidRxData.reasonNoName = state->clidRxState.clidRxData.reasonNoName;
            vrgClidRxData.reasonNoNumber = state->clidRxState.clidRxData.reasonNoNumber;

            (*endptConfig.notify)( state->endptArchive, -1, EPEVT_FSK_CLIDRX, (void *)&vrgClidRxData, sizeof(vrgClidRxData), 0 );
         }
      }
      break;
#endif

      case HAPI_CLIDTX_MSG_ERROR_EVT:
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT %d: ERROR - CLIDTX not ready for new message", state->endptNum ));
      }
      break;

      default:
      {
      }
      break;
   }
}

/*****************************************************************************
*  FUNCTION:   ClearConnectionOptions
*
*  PURPOSE:    Clear local codec parameters (e.g. codec type, period etc.)
*
*  PARAMETERS: cnx   - Connection state to clear
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
void ClearConnectionOptions( CNXSTATE *cnx )
{
   cnx->mode                      = EPCNXMODE_MAX;
   cnx->silence                   = 0;
   cnx->comfortNoise              = HAPI_GCNG_HOTHNOISE;
   cnx->period                    = 0;
   cnx->codecType                 = CODEC_NULL;
   cnx->namedPhoneEvts            = 0;
   cnx->lastFaxEvent              = EPFAX_EVT_NONE;
   cnx->bMuteAudio                = VRG_FALSE;
   cnx->t38param.ecMode           = EPT38EC_UDPREDUNDANCY;
   cnx->t38param.numFaxPagesRx    = 0;
   cnx->t38param.numFaxPagesTx    = 0;
   cnx->vbData.param.vbdRedMode   = EPVBDRED_OFF;
   cnx->vbData.param.vbdMode      = EPVBDMODE_LEGACY;
   cnx->localSsrc                 = 0;
   cnx->toneRelayRefreshInterval  = 0;
}

/*
*****************************************************************************
** FUNCTION:   ConnectPstnGwSB
**
** PURPOSE:    Establish a switchboard connection pstnline vhd and gw vhd
**
** PARAMETERS: int gwVhdhdl      , vhdhdl of gw vhd
**             int pstnLineVhdhdl, vhdhdl of pstn line vhd
**
**
** RETURNS:    EPSTATUS
**
** NOTE:       vhd's MUST be on the same DSP.  This is a limitation
**             of the Hausware support.
*****************************************************************************
*/
EPSTATUS ConnectPstnGwSB(int gwVhdhdl, int pstnLineVhdhdl)
{
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ConnectPstnGwSB: connecting gw VHDHDL 0x%04x to pstn endpoint line VHDHDL 0x%04x", gwVhdhdl, pstnLineVhdhdl  ));

   return hdspRmSwitchBoardConnect( gwVhdhdl,
                                     HAPI_SWB_BOS,
                                     pstnLineVhdhdl,
                                     HAPI_SWB_TOS,
                                     HAPI_SWB_DUPLEX,
                                     VRG_TRUE );
}

/*
*****************************************************************************
** FUNCTION:   ConnectSB
**
** PURPOSE:    Establish a switchboard connection between line1 and line2
**
** PARAMETERS: line1   - index of line1
**             line2   - index of line2
**
** RETURNS:    EPSTATUS
**
** NOTE:       line1 and line2 MUST be on the same DSP.  This is a limitation
**             of the Hausware support.
*****************************************************************************
*/
EPSTATUS ConnectSB( int line1, int line2 )
{
   VRG_ENDPT* state1;
   VRG_ENDPT* state2;

   state1 = GetEndptState( line1 );
   state2 = GetEndptState( line2 );

   if( state1->dspNum != state2->dspNum )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ConnectSB: failed for line%d (%d) and line%d (%d) - Must be on same DSP!",
              line1, state1->dspNum, line2, state2->dspNum ));

      return EPSTATUS_RESOURCES;
   }
   else
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ConnectSB: connecting line%d and line%d ", line1, line2));

      return hdspRmSwitchBoardConnect( state1->lineVhdHdl,
                                      HAPI_SWB_TOS,
                                      state2->lineVhdHdl,
                                      HAPI_SWB_TOS,
                                      HAPI_SWB_DUPLEX,
                                      VRG_TRUE );

   }
}

/*****************************************************************************
*  FUNCTION:   ConnectionOptionsChanged
*
*  PURPOSE:    Compare local codec parameters. Determine if any of the
*              parameters have changed.
*
*  PARAMETERS: cnx      - current connection state instance memory
*              newParam - new connection parameters (codec, pktperiod, etc)
*
*  RETURNS:    TRUE if connection parameters have changed, else FALSE
*
*  NOTE:
*****************************************************************************/
VRG_BOOL ConnectionOptionsChanged( CNXSTATE *cnx, EPZCNXPARAM *newParm )
{
   VRG_UINT32  oldDtmfNte, newDtmfNte;
   VRG_BOOL    changed = VRG_FALSE;

   oldDtmfNte = cnx->namedPhoneEvts & CODEC_NTE_DTMF;
   newDtmfNte = newParm->namedPhoneEvts & CODEC_NTE_DTMF;

   if (( cnx->mode      != newParm->mode )      ||
       ( cnx->silence   != newParm->silence )   ||
#if VRG_ENDPT_PACKET_TIME_CHECK
       ( cnx->period    != checkPacketTime( newParm ))    ||
#else
       ( cnx->period    != newParm->cnxParmList.send.period[0] )    ||
#endif // VRG_ENDPT_PACKET_TIME_CHECK
       ( cnx->codecType != newParm->cnxParmList.send.codecs[0].type ) ||
       ( oldDtmfNte     != newDtmfNte ) ||
       ( cnx->dataMode  != newParm->dataMode ) ||
       ( cnx->t38param.ecMode != newParm->t38param.ecMode ) ||
       ( cnx->comfortNoise != newParm->comfortNoise )||
 	   ( cnx->digitRelayType != newParm->digitRelayType ))
   {
      changed = VRG_TRUE;
   }

   return ( changed );
}

/*
*****************************************************************************
** FUNCTION:   DisConnectPstnGwSB
**
** PURPOSE:    Disconnect a switchboard connection pstnline vhd and gw vhd
**
** PARAMETERS: int gwVhdhdl      , vhdhdl of gw vhd
**             int pstnLineVhdhdl, vhdhdl of pstn line vhd
**
**
** RETURNS:    EPSTATUS
**
** NOTE:       vhd's MUST be on the same DSP.  This is a limitation
**             of the Hausware support.
*****************************************************************************
*/
EPSTATUS DisConnectPstnGwSB(int gwVhdhdl, int pstnLineVhdhdl)
{
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "DisConnectPstnGwSB: disconnecting gw VHDHDL 0x%04x from pstn endpoint line VHDHDL 0x%04x", gwVhdhdl, pstnLineVhdhdl  ));

   return hdspRmSwitchBoardConnect( gwVhdhdl,
                                     HAPI_SWB_BOS,
                                     pstnLineVhdhdl,
                                     HAPI_SWB_TOS,
                                     HAPI_SWB_DUPLEX,
                                     VRG_FALSE );
}

/*
*****************************************************************************
** FUNCTION:   DisconnectSB
**
** PURPOSE:    Removes a switchboard connection between line1 and line2
**
** PARAMETERS: line1   - index of line1
**             line2   - index of line2
**
** RETURNS:    EPSTATUS
**
** NOTE:       line1 and line2 MUST be on the same DSP.  This is a limitation
**             of the Hausware support.
*****************************************************************************
*/
EPSTATUS DisconnectSB( int line1, int line2 )
{
   VRG_ENDPT* state1;
   VRG_ENDPT* state2;

   state1 = GetEndptState( line1 );
   state2 = GetEndptState( line2 );

   if( state1->dspNum != state2->dspNum )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "DisconnectSB: failed for line%d (%d) and line%d (%d) - Must be on same DSP!",
              line1, state1->dspNum, line2, state2->dspNum ));

      return EPSTATUS_RESOURCES;
   }
   else
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "DisconnectSB: disconnecting line%d and line%d ", line1, line2));

      return hdspRmSwitchBoardConnect( state1->lineVhdHdl,
                                      HAPI_SWB_TOS,
                                      state2->lineVhdHdl,
                                      HAPI_SWB_TOS,
                                      HAPI_SWB_DUPLEX,
                                      VRG_FALSE );
   }
}

/*
****************************************************************************
** FUNCTION: EcanProcessStm
**
** PURPOSE:  Processes the Ecan state machine based on the received event
**
** PARAMETERS: state - enpoint state
**             evt   - event based on which to process the Ecan state machine
**
** RETURNS:
**
** NOTE:
**
*****************************************************************************
*/
void EcanProcessStm(VRG_ENDPT *endptState, ECANEVT evt)
{
#ifdef VRG_ENDPT_CFG_G168_TEST
   const char * ECANEVT_STR[] = {
      "ECANEVT_CRCX_ON",
      "ECANEVT_CRCX_OFF",
      "ECANEVT_MDCX_ON",
      "ECANEVT_MDCX_OFF",
      "ECANEVT_DLCX",
      "ECANEVT_CNG",
      "ECANEVT_HDLC",
      "ECANEVT_MACHINE",
      "ECANEVT_PHREV",
      "ECANEVT_LOWEN",
      "ECANEVT_V18",
      "ECANEVT_BELL103",
      "ECANEVT_ONHOOK",
      "ECANEVT_OFFHOOK",
      "ECANEVT_DIALTONE_ON",
      "ECANEVT_DIALTONE_OFF"
   };

   VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ENDPT %d: Ignoring EVT %s \n", endptState->endptNum, ECANEVT_STR[evt]));
#else
   EPZECANSTATE *ecanState = &endptState->ecanState;

   /*
   ** Do not update the state if in TR57 test mode or analog loopback test mode or ECAN debug mode.
   ** Ecan should have been set upon entering any of the above test modes,
   ** leave it in the same state until this mode terminates.
   */
   if (endptState->testMode == TESTMODE_TR57 || endptState->testMode == TESTMODE_ANALOGLOOPBACK || endptState->testMode == TESTMODE_ECANDEBUGCMD)
   {
      return;
   }

   switch (ecanState->currState)
   {
      case ECAN_OFF:
      {
         switch (evt)
         {
            case ECANEVT_OFFHOOK:
            {
               if (endptState->cnxCnt == 0)
               {
                  /*
                  ** Turn the Ecan ON if no connections on this endpoint.
                  */
                  ecanState->currState = ECAN_ON;
                  EcanStateSet(endptState, ECAN_ON);
               }
               else
               {
                  /*
                  ** Turn Ecan to the state directed by CRCX/MDCX parameters
                  ** in the existing connection.
                  */
                  if (ecanState->currState != ecanState->origState)
                  {
                     ecanState->currState = ecanState->origState;
                     EcanStateSet(endptState, ecanState->currState);
                  }
               }
            }
            break;

            case ECANEVT_CRCX_OFF:
            {
               /* We are already off, only keep track of # of cnx on this endpt */
               endptState->cnxCnt++;
            };
            /* Intentional fall through !!! */
            case ECANEVT_MDCX_OFF:
            {
               /* Save this state so that we can restore it when we exit VBD */
               ecanState->origState = ECAN_OFF;
            }
            break;

            case ECANEVT_CRCX_ON:
            {
               /* Only change the Ecan state if we are the first cnx on this endpt.
               ** Otherwise, existing cnx may need it off, so better leave it as is */
               endptState->cnxCnt++;
               if (endptState->cnxCnt == 1)
               {
                  /* Save this state so that we can restore into right state */
                  ecanState->origState = ECAN_ON;

                  /* Only turn the Ecan ON if we are offhook. If we are onhook
                  ** we will go to the right state on offhook transition */
                  if (endptState->offHook)
                  {
                     /* We are offhook. Turn the Ecan ON */
                     ecanState->currState = ECAN_ON;
                     EcanStateSet(endptState, ECAN_ON);
                  }
               }
            }
            break;

            case ECANEVT_MDCX_ON:
            {
               /* If Ecan was originally OFF than we can go to ON.
               ** Otherwise, we have been switched to OFF and we won't interfere */
               if (ecanState->origState == ECAN_OFF)
               {
                  /* Only turn the Ecan ON if we are offhook. If we are onhook
                  ** we will go to the right state on offhook transition */
                  if (endptState->offHook)
                  {
                     ecanState->currState = ECAN_ON;
                     EcanStateSet(endptState, ECAN_ON);
                  }
               }

               /* Either way, save this state so that we can restore into right state */
               ecanState->origState = ECAN_ON;
            }
            break;


            case ECANEVT_DLCX:
            {
               /* We are already off, only update the # of cnx on this endpt */
               endptState->cnxCnt--;
            }
            break;

            case ECANEVT_LOWEN:
            {
               /* Low Energy detected, turn the Ecan back to its original state */
               if (ecanState->currState != ecanState->origState)
               {
                  ecanState->currState = ecanState->origState;
                  EcanStateSet(endptState, ecanState->currState);
               }
            }
            break;

            case ECANEVT_HDLC:
            {
              ecanState->currState = ECAN_NLP_OFF;
              EcanStateSet(endptState, ECAN_NLP_OFF);
            }
            break;

            default:
            {
            }
            break;
         }
      }
      break;

      case ECAN_ON:
      {
         switch (evt)
         {
            case ECANEVT_ONHOOK:
            {
               /*
               ** Turn the Ecan OFF regardless of potential connections.
               ** This is because we don't want to leave it on and have it re-converge
               ** on the Echo path change due to onhook state.
               */
               ecanState->currState = ECAN_OFF;
               EcanStateSet(endptState, ECAN_OFF);
        }
            break;

            case ECANEVT_CRCX_ON:
            {
               /* Ecan already ON, only keep track of # of cnx on this endpt */
               endptState->cnxCnt++;
            };
            /* Intentional fall through !!! */
            case ECANEVT_MDCX_ON:
            {
               /* Save this state so that we can restore it when we exit VBD */
               ecanState->origState = ECAN_ON;
            };
            break;

            case ECANEVT_CRCX_OFF:
            {
               /* Update the # of cnx on this endpt */
               endptState->cnxCnt++;
            };
            /* Intentional fall through !!! */
            case ECANEVT_MDCX_OFF:
            {
               /* Save this state so that we can restore it when we exit VBD */
               ecanState->origState = ECAN_OFF;

               /* Must turn the Ecan OFF if requested by CRCX/MDCX */
               ecanState->currState = ECAN_OFF;
               EcanStateSet(endptState, ECAN_OFF);
            }
            break;

            case ECANEVT_DLCX:
            {
               /* Only turn the Ecan OFF if we are the last cnx on this endpt.
               ** Otherwise, existing cnx may need it ON, so better leave it as is */
               endptState->cnxCnt--;
               if (endptState->cnxCnt == 0)
               {
                  ecanState->currState = ECAN_OFF;
                  EcanStateSet(endptState, ECAN_OFF);
               }
            }
            break;

            case ECANEVT_CNG:
            case ECANEVT_HDLC:
            case ECANEVT_MACHINE:
            case ECANEVT_V18:
            case ECANEVT_BELL103:
            case ECANEVT_PHREV:
            {
               /* CDIS events detected, turn the Ecan's Nonlinear processing OFF */
               ecanState->currState = ECAN_NLP_OFF;
               EcanStateSet(endptState, ECAN_NLP_OFF);
            }
            break;

            case ECANEVT_LOWEN:
            {
               /* Shouldn't happen. If we get low energy means we had CDIS events before
               ** therefore, Ecan shouldn't be on, so we shouldn't get here */
            }
            break;

            case ECANEVT_DIALTONE_ON:
            {
               /* Save this state so that we can restore it when dialtone stops */
               ecanState->origState = ECAN_ON;

               /* Turn NLP off during dialtone to allow Ecan to converge */
               ecanState->currState = ECAN_NLP_OFF;
               EcanStateSet(endptState, ECAN_NLP_OFF);
            }
            break;

            default:
            {
            }
            break;
         }
      }
      break;

      case ECAN_NLP_OFF:
      {
         switch (evt)
         {
            case ECANEVT_ONHOOK:
            {
               /*
               ** Turn the Ecan OFF regardless of potential connections.
               ** This is because we don't want to leave it on and have it re-converge
               ** on the Echo path change due to onhook state.
               */
               ecanState->currState = ECAN_OFF;
               EcanStateSet(endptState, ECAN_OFF);
            }
            break;

            case ECANEVT_OFFHOOK:
            {
               /* We can only be in this state if we got CDIS events before. Since
               ** those can't happen onhook, there is no way we could be here and
               ** get an offhook event */
            }
            break;

            case ECANEVT_CRCX_ON:
            {
               /* Only keep track of # of cnx on this endpt.
               ** Don't turn the NLP back ON as existing cnx may need it as is */
               endptState->cnxCnt++;
            };
            /* Intentional fall through !!! */
            case ECANEVT_MDCX_ON:
            {
               /* Save this state so that we can restore it when we exit VBD */
               ecanState->origState = ECAN_ON;
            };
            break;

            case ECANEVT_CRCX_OFF:
            {
               /* Must turn the Ecan OFF if requested by CRCX */
               endptState->cnxCnt++;
            };
            /* Intentional fall through !!! */
            case ECANEVT_MDCX_OFF:
            {
               /* Save this state so that we can restore it when we exit VBD */
               ecanState->origState = ECAN_OFF;

               /* Must turn the Ecan OFF if requested by MDCX */
               ecanState->currState = ECAN_OFF;
               EcanStateSet(endptState, ECAN_OFF);
            }
            break;

            case ECANEVT_DLCX:
            {
               /* Only turn the Ecan OFF if we are the last cnx on this endpt.
               ** Otherwise, leave it as is */
               endptState->cnxCnt--;
               if (endptState->cnxCnt == 0)
               {
                  ecanState->currState = ECAN_OFF;
                  EcanStateSet(endptState, ECAN_OFF);
               }
            }
            break;

            case ECANEVT_PHREV:
            {
               /* Modem/TDD detected.
               ** Must turn the Ecan OFF (until Low Energy detected) */
               ecanState->currState = ECAN_OFF;
               EcanStateSet(endptState, ECAN_OFF);
            }
            break;

            case ECANEVT_DIALTONE_OFF:
            case ECANEVT_LOWEN:
            {
               /* Low Energy or dialtone off detected, turn the Ecan back to its original state */
               if (ecanState->currState != ecanState->origState)
               {
                  ecanState->currState = ecanState->origState;
                  EcanStateSet(endptState, ecanState->currState);
               }
            }
            break;

            default:
            {
            }
            break;
         }
      }
      break;

      default:
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT %d: Unknown Ecan state change event %d !!!", endptState->endptNum, ecanState->currState));
      }
   }
#endif /* VRG_ENDPT_CFG_G168_TEST */
}



/*****************************************************************************
*  FUNCTION:   EcanStateSet
*
*  PURPOSE:    Sets the Echo Cancellation module into a given state
*
*  PARAMETERS: endptState    - endpoint state structure
*              newEcanState  - ecan state to set the Ecan to
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
void EcanStateSet( VRG_ENDPT *endptState, EPEECANSTATE newEcanState )
{
   VRG_UINT32 cngLevel;

   switch (newEcanState)
   {
      case ECAN_ON:
      {
         /* HAPI_HEC_ENABLE_CMD is command used to do a hot reset of the ECAN.
         ** A hot reset only resets the ECAN history but does not reset the ECAN
         ** registers. The command also sets the HAPI_HEC_BYPASS_ON to 0. */
         hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );

         /* HAPI_HEC_OPTIONS_OFF_HOOK will set the options register to be all 0.
         ** Setting the option register to all 0 will enable and disable all ECAN options
         ** that are required on an offhook state. This includes enabling NLP. */
         hdspSendCmdAsync( endptState->lineVhdHdl,
                           HAPI_HEC_SETREG1_CMD,
                           HSCMDDATA,
                           VRG_OFFSETOF(HEC_REGS, options),
                           HAPI_HEC_OPTIONS_OFF_HOOK );

         Provision( endptState->endptArchive, EPPROV_CngLevel, &cngLevel );
         if (cngLevel != 0)
         {
            hdspSendCmdAsync( endptState->lineVhdHdl,
                              HAPI_HEC_SETREG1_CMD,
                              HSCMDDATA,
                              VRG_OFFSETOF(HEC_REGS, options),
                              HAPI_HEC_CNG_CAP_ON );
         }

#ifdef VRG_ENDPT_CFG_G168_TEST
         /* Update endpoint's ECAN option register */
         endptState->ecanOptions |= HAPI_HEC_OPTIONS_OFF_HOOK;
#endif
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "ECAN Enabled for endpt %d", endptState->endptNum ));
      }
      break;

      case ECAN_OFF:
      {
         /* Disable Echo canceller. HAPI_HEC_ENABLE_CMD simply sets the bypass flag
         ** to be true. */
         hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_FALSE, 0 );

#ifdef VRG_ENDPT_CFG_G168_TEST
         /* Update endpoint's ECAN option register */
         endptState->ecanOptions &= ~(HAPI_HEC_BYPASS_ON);
#endif

         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "ECAN disabled for endpt %d", endptState->endptNum ));
      }
      break;


      case ECAN_NLP_OFF:
      {
         /* Disable Non-linear processing (voiceband data options) */
#ifndef VRG_ENDPT_CFG_G168_TEST
         hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );
#endif

         /* HAPI_HEC_OPTIONS_VBD will set the options register such that HAPI_HEC_NLP_OFF bit and
         ** HAPI_HEC_LEVELCTRL_OFF bit will be set to 1. HAPI_HEC_NLP_OFF turns off NLP.
         ** HAPI_HEC_LEVELCTRL_OFF disables the volume compressor and which makes the NLP more aggressive
         ** for high level signals if NLP were enabled. */
         hdspSendCmdAsync( endptState->lineVhdHdl,
                           HAPI_HEC_SETREG1_CMD,
                           HSCMDDATA,
                           VRG_OFFSETOF(HEC_REGS, options),
                           HAPI_HEC_OPTIONS_VBD );

#ifdef VRG_ENDPT_CFG_G168_TEST
         /* Update endpoint's ECAN option register */
         endptState->ecanOptions |= HAPI_HEC_OPTIONS_VBD;
#endif

         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "ECAN NLP disabled for endpt %d", endptState->endptNum ));
      }
      break;

#ifdef VRG_ENDPT_CFG_G168_TEST
      case ECAN_NLP_ON:
      {
         /* Enable NLP */
         if (endptState->ecanOptions & HAPI_HEC_ADAPT_OFF)
         {
            /* If HAPI_HEC_ADAPT_OFF is set such that Adaptation is disabled, we only want to enable
            ** NLP by setting HAPI_HEC_NLP_OFF to 0. Since we are not even adapting, we don't have to
            ** worry about HAPI_HEC_LEVELCTRL_OFF. */
            endptState->ecanOptions &= ~(HAPI_HEC_NLP_OFF);
         }
         else
         {
            /* If HAPI_HEC_ADAPT_OFF is not set such that Adaptation is enabled, we want to set both
            HAPI_HEC_NLP_OFF and HAPI_HEC_LEVELCTRL_OFF to be 0. */
            endptState->ecanOptions &= ~(HAPI_HEC_OPTIONS_VBD);
         }

         // hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );
         hdspSendCmdAsync( endptState->lineVhdHdl,
                           HAPI_HEC_SETREG1_CMD,
                           HSCMDDATA,
                           VRG_OFFSETOF(HEC_REGS, options),
                           endptState->ecanOptions );

         VRG_LOG(( "ENDPT %d: ECAN NLP enabled\n", endptState->endptNum ));
      }
      break;

      case ECAN_ADAPT_OFF:
      {
         /* HAPI_HEC_OPTIONS_CALLSETUP will set both HAPI_HEC_ADAPT_OFF and HAPI_HEC_LEVELCTRL_OFF bits
         ** as 1. Setting HAPI_HEC_ADAPT_OFF will turn adaption off. Setting HAPI_HEC_LEVELCTRL_OFF will
         ** disable volumn compressor. Since we are not adapting, we don't worry about level control */
         endptState->ecanOptions |= HAPI_HEC_OPTIONS_CALLSETUP;

         // hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );
         hdspSendCmdAsync( endptState->lineVhdHdl,
                           HAPI_HEC_SETREG1_CMD,
                           HSCMDDATA,
                           VRG_OFFSETOF(HEC_REGS, options),
                           endptState->ecanOptions );

         VRG_LOG(( "ENDPT %d: ECAN Adaptation disabled\n", endptState->endptNum ));
      }
      break;

      case ECAN_ADAPT_ON:
      {
         /* Enable Adaptation */
         if (endptState->ecanOptions & HAPI_HEC_NLP_OFF)
         {
            /* If NLP is off, then we just set HAPI_HEC_ADAPT_OFF bit to be 0 to turn adaption on. */
            endptState->ecanOptions &= ~(HAPI_HEC_ADAPT_OFF);
         }
         else
         {
            /* If NLP is on, then we want to also set HAPI_HEC_LEVELCTRL_OFF to be 0 to enable
            ** volumn compressor for the NLP */
            endptState->ecanOptions &= ~(HAPI_HEC_OPTIONS_CALLSETUP);
         }

         //hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );
         hdspSendCmdAsync( endptState->lineVhdHdl,
                           HAPI_HEC_SETREG1_CMD,
                           HSCMDDATA,
                           VRG_OFFSETOF(HEC_REGS, options),
                           endptState->ecanOptions );

         VRG_LOG(( "ENDPT %d: ECAN Adaptation enabled\n", endptState->endptNum ));
      }
      break;

      case ECAN_BYPASS_OFF:
      {
         /* Setting HAPI_HEC_BYPASS_ON bit to be 0 will disable bypass, which will enable
         ** echo cancelling */
         endptState->ecanOptions &= ~(HAPI_HEC_BYPASS_ON);

         //hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );
         hdspSendCmdAsync( endptState->lineVhdHdl,
                           HAPI_HEC_SETREG1_CMD,
                           HSCMDDATA,
                           VRG_OFFSETOF(HEC_REGS, options),
                           endptState->ecanOptions );

         VRG_LOG(( "ENDPT %d: ECAN By-pass disabled\n", endptState->endptNum ));
      }
      break;

      case ECAN_BYPASS_ON:
      {
         /* Setting HAPI_HEC_BYPASS_ON bit to be 1 will enable bypass, which will disable
         ** echo cancelling */
         endptState->ecanOptions |= HAPI_HEC_BYPASS_ON;

         //hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );
         hdspSendCmdAsync( endptState->lineVhdHdl,
                           HAPI_HEC_SETREG1_CMD,
                           HSCMDDATA,
                           VRG_OFFSETOF(HEC_REGS, options),
                           endptState->ecanOptions );

         VRG_LOG(( "ENDPT %d: ECAN By-pass enabled\n", endptState->endptNum ));
      }
      break;

      case ECAN_CNG_ON:
      {
         /* When HAPI_HEC_CNG_OFF bit is 0 and HAPI_HEC_CNG_SUPPRESSOR bit is 1, then CNG is
         ** a comfort noise generator */
         endptState->ecanOptions &= ~HAPI_HEC_CNG_OFF;
         endptState->ecanOptions |= HAPI_HEC_CNG_SUPPRESSOR;

         //hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );
         hdspSendCmdAsync( endptState->lineVhdHdl,
                           HAPI_HEC_SETREG1_CMD,
                           HSCMDDATA,
                           VRG_OFFSETOF(HEC_REGS, options),
                           endptState->ecanOptions );

         VRG_LOG(( "ENDPT %d: ECAN CNG enabled\n", endptState->endptNum ));
      }
      break;

      case ECAN_CNG_OFF:
      {
         /* When HAPI_HEC_CNG_OFF and HAPI_HEC_CNG_SUPPRESSOR bits are 1, then CNG is off
         ** and absolute silence is generated if NLP is enabled */
         endptState->ecanOptions |= (HAPI_HEC_CNG_OFF | HAPI_HEC_CNG_SUPPRESSOR);

         //hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );
         hdspSendCmdAsync( endptState->lineVhdHdl,
                           HAPI_HEC_SETREG1_CMD,
                           HSCMDDATA,
                           VRG_OFFSETOF(HEC_REGS, options),
                           endptState->ecanOptions );

         VRG_LOG(( "ENDPT %d: ECAN CNG disabled\n", endptState->endptNum ));
      }
      break;

      case ECAN_H_RESET:
      {
         /* This command resets the coefficients used to model the echo path
         ** response. This is equivalent to resetting the H register in G.168.
         ** HAPI_HEC_RESETCOEFF_ALL resets the foreground and background
         ** cancellers */
         hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_RESETCOEFF_CMD, HSCMDDATA, HAPI_HEC_RESETCOEFF_ALL, 0 );

         VRG_LOG(( "ENDPT %d: ECAN H Reset\n", endptState->endptNum ));
      }
      break;

      case ECAN_RESET:
      {
         /* Reset Ecan - performs a cold reset which resets the registers back
         ** to their default values. A hot reset can be done by setting bypass
         ** on and off. A hot reset, however, only resets the ECAN history */
         hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_RESET_CMD, HSCMDDATA, HAPI_HEC_RESET_COLD, 0 );

         VRG_LOG(( "ENDPT %d: ECAN Reset\n", endptState->endptNum ));
      }
      break;
#endif
      default:
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT %d: Unknown Ecan state set requested, ecan state %d !!!", endptState->endptNum, newEcanState));
      }
   }
}

/*
****************************************************************************
** FUNCTION: EcanStmInit
**
** PURPOSE:  Initializes the Ecan State machine and the Echo canceller itself
**
** PARAMETERS: endptState - endpoint state
**
** RETURNS:
**
** NOTE:
**
*****************************************************************************
*/
static void EcanStmInit(VRG_ENDPT *endptState)
{
   endptState->ecanState.currState = ECAN_OFF;
   endptState->ecanState.origState = ECAN_OFF;

   /*
   ** Make sure H/W and S/W Ecan (if present) are initialy OFF
   */

   /* DSP Ecan */
   hdspSendCmdAsync( endptState->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, 0, 0 );
}

/*
*****************************************************************************
** FUNCTION:   EnableRelay
*
*  PURPOSE:
*     Enables/Disables Digit relay and/or fax relay/vbd on a channel
*
* PARAMETERS: cnx        - pointer to connection state
*             digitRelay - TRUE to enable, FALSE to disable
*             rate       - superpacket interval in units of samples sampled at 8kHz
*             dataMode   - T38, vbd, ...
*  RETURNS:
*     EPSTATUS_SUCCESS or error code
*
*  NOTE:
*
*****************************************************************************
*/
int EnableRelay( CNXSTATE *cnx, VRG_BOOL digitRelay, VRG_UINT16 rate, EPDATAMODE dataMode, EPDTMFRFC2833CONFIG digitRelayType )
{
   HSZNETREGS netregs;
   VRG_SINT16 result;

   /* If this is an enable command, set the refresh interval to the correct rate */
   if ( digitRelay )
   {
      /* Skip if the refresh rate is already correct */
      if (cnx->toneRelayRefreshInterval != (rate>>3))
      {
         /* Set Refresh Interval in Number of Samples */
          result = hdspSendCmdAsync( cnx->vhdhdl,
                               HAPINET_PTE_SETREG1,
                               HSCMDDATA,
                               VRG_OFFSETOF(HAPI_PTE_REGS, refreshInterval),
                               rate );

         if ( result != HAPISUCCESS )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting PTE refresh interval for endpt %d", cnx->lineId ));
            return ( EPSTATUS_RESOURCES );
         }

         VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "DTMF Relay period set to %d ms ", rate>>3));
         cnx->toneRelayRefreshInterval = (rate>>3);
      }
   }

   /* Check if already in the right mode */
   if ( (cnx->digitRelay == digitRelay) && (cnx->dataMode == dataMode)&& (cnx->digitRelayType == digitRelayType) )
   {
      return ( EPSTATUS_SUCCESS );
   }

   netregs = cnx->netRegs;

   if ( digitRelay )
   {
      /* Enable dtmf relay option */
      netregs.hsxPtePacketVoiceOptions |= HSNET_PTE_OPTION_DIG_RELAY|HSNET_PTE_OPTION_DETECTOR_INGRESSMASK;
	  if ( digitRelayType == EPDTMFRFC2833_SUBTRACT )
      {
          cnx->rtpRegs2.configFlags2 |= HAPI_RTP_SUBTRACT_DTMF_ENABLE_MASK;
      }
      else
      {
          cnx->rtpRegs2.configFlags2 &= ~HAPI_RTP_SUBTRACT_DTMF_ENABLE_MASK;          
      }
   }
   else
   {
      /* Disable dtmf relay option */
      netregs.hsxPtePacketVoiceOptions &= (~HSNET_PTE_OPTION_DIG_RELAY);
   }

   switch ( dataMode )
   {
      case EPDATAMODE_T38_MUTE:
      case EPDATAMODE_T38:
      {
         netregs.hsxNetOptions |= HSNET_NET_OPTION_FAXRELAY;
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "EnableRelay: enable fax relay for endpt %d", cnx->lineId ));
      }
      break;

      case EPDATAMODE_VBD:
      default:
      {
         netregs.hsxNetOptions &= ~HSNET_NET_OPTION_FAXRELAY;
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "EnableRelay: VBD mode for endpt %d", cnx->lineId ));
      }
      break;
   }

   /* Send the new configuration */
   result = hdspSendCmdDataAsync( cnx->vhdhdl,
                                  HAPINET_CONFIG_SETREGS,
                                  HSCMDEXTDATA_ASYNC,
                                  sizeof( netregs ),
                                  &netregs );

   if ( result != HAPISUCCESS )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting registers for endpt %d", cnx->lineId ));
      return ( EPSTATUS_RESOURCES );
   }

   /* Save digit relay mode */
   cnx->digitRelay = digitRelay;
   cnx->dataMode = dataMode;
   cnx->netRegs.hsxNetOptions = netregs.hsxNetOptions;
   cnx->netRegs.hsxPtePacketVoiceOptions = netregs.hsxPtePacketVoiceOptions;
   cnx->digitRelayType = digitRelayType;

   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "DTMF Relay %d, Fax %d ", digitRelay, dataMode ));

   return ( EPSTATUS_SUCCESS );
}

/*****************************************************************************
*  FUNCTION:   EndptUpdateCasState
*
*  PURPOSE:    To update the endpoint state machine based upon CAS state.
*              This function is invoked indirectly through endptUpdateCasState()
*              if the CAS is external to the endpoint model. If the CAS is internal
*              this function is invoked via the CAS callback.
*
*  PARAMETERS: lineId      - the line id
*              casState    - the current cas state (e.g. on-hook, off-hook...)
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS EndptUpdateCasState( int lineId, EPCASSTATE casState )
{
   VRG_ENDPT     *state;
   int            cnxIndex;
   HSZEVT pstnEvt;   /* PSTN Ctl uses HSZEVT to recieve events */

   /*
   ** Get the endpoint state
   */
   state = GetEndptState( lineId );
   if( ! state )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "Unknown CAS line id %d", lineId ));
      return(EPSTATUS_ENDPT_UNKNOWN);
   }

   switch( casState )
   {
#if VRG_ENDPT_CFG_ONHOOK_MUTING_SUPPORT
      case EPCAS_DELAY_OFFHOOK:
      {
         /* Change endpoint state used for endpoint muting */
         state->delayOffHook = VRG_TRUE;

         /* Disable endpoint muting and restore original gain setting */
         hdspVhdSetIngressGain( state->lineVhdHdl, state->txGain );
      }
      break;

      case EPCAS_DELAY_ONHOOK:
      {
         /* Change endpoint state used for endpoint muting */
         state->delayOffHook = VRG_FALSE;

         /* Enable endpoint muting */
         hdspVhdSetIngressGain( state->lineVhdHdl, EP_MIN_GAIN );
      }
      break;
#endif

      case EPCAS_EARLY_OFFHOOK:
      {
         /* Change endpoint state used for RFC2833 */
         state->earlyOffHook = VRG_TRUE;

         /* Disable endpoint muting and restore original gain setting
         ** from when the line went onhook */
         hdspVhdSetIngressGain( state->lineVhdHdl, state->txGain );

         /* Process Ecan state machine:
         ** Treat early offhook same as real offhook to get the Ecan going
         ** as soon as possible */
         EcanProcessStm( state, ECANEVT_OFFHOOK );

         if ( state->bGR303Support && !(state->bPulse2dtmf))
         {
            /* Determine the index of the current cnx in the endpoint's cnx queue */
            for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
            {
               if ( state->cnx[cnxIndex] )
               {
                  /* Update the current hookstate */
                  GenerateNteEvent( state->cnx[cnxIndex], RTP_NTE_OFFHOOK );
               }
            }
         }
      }
      break;

      case EPCAS_EARLY_ONHOOK:
      {
         /* Change endpoint state used for RFC2833 */
         state->earlyOffHook = VRG_FALSE;

         /* Process Ecan state machine:
         ** Treat early onhook same as real onhook to stop the Ecan as soon
         ** as possible and prevent it from re-converging on Echo path change due
         ** to onhook transition */
         EcanProcessStm( state, ECANEVT_ONHOOK );

         /* Enable endpoint muting in the ingress direction to prevent reflection
         ** towards the remote connection (if still active) since Ecan will be
         ** disabled in the onhook state. This should also prevent reflected ringing and
         ** onhook caller-id from being encoded by the DSP and sent to the network */
         hdspVhdSetIngressGain( state->lineVhdHdl, EP_MIN_GAIN );

         if ( state->bGR303Support && !(state->bPulse2dtmf))
         {
            /* Determine the index of the current cnx in the endpoint's cnx queue */
            for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
            {
               if ( state->cnx[cnxIndex] )
               {
                  /* Update the current hookstate and generate onhook event as required */
                  GenerateNteEvent( state->cnx[cnxIndex], RTP_NTE_ONHOOK );
               }
            }
         }
      }
      break;

      case EPCAS_ONHOOK:
      {
         state->offHook = VRG_FALSE;
         state->earlyOffHook = VRG_FALSE;
         state->delayOffHook = VRG_FALSE;
         if ( state->bGR303Support )
         {
#if VRG_ENDPT_CFG_USE_HFE
            /* Qualified On-hook must cancel Hook-flash Emulation */
            if ( state->bPulse2dtmf )
            {
               /* Remove Timer from List: Semantic equivalent is bosTimerStop() */
               bosTimerDestroy(&state->hookflashTimer);
            }
#endif
            /* Determine the index of the current cnx in the endpoint's cnx queue */
            for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
            {
               if ( state->cnx[cnxIndex] )
               {
                  /* Update the current hookstate and generate onhook event as required */
                  GenerateNteEvent( state->cnx[cnxIndex], RTP_NTE_ONHOOK );
               }
            }
         }

         /* Process Ecan state machine */
         EcanProcessStm( state, ECANEVT_ONHOOK );

         /* Post the event for the caller ID state machine */
         classStmEventProcess( &state->classState, CLASSSTM_ONHOOK, state->lineId );

         hbUpdatePatientState( state->hookStateHBId, HBSTATE_INACTIVE );
      }
      break;

      case EPCAS_OFFHOOK:
      {
         hbUpdatePatientState( state->hookStateHBId, HBSTATE_ACTIVE );

         state->offHook = VRG_TRUE;
         state->earlyOffHook = VRG_TRUE;
         state->delayOffHook = VRG_TRUE;

         /* Process Ecan state machine */
         EcanProcessStm( state, ECANEVT_OFFHOOK );

#if FLEXIBLE_CONFERENCING_SUPPORTED
         /*
            We do not need to associate a connection with PSTN endpoints because the connection
            for such endpoint is currently reserved by default.
         */
         if (state->endptType != EPTYPE_PSTN)
         {
            if ( !state->cnx[PRIMARY_CNX_INDEX] )
            {
               state->cnx[PRIMARY_CNX_INDEX] = GetNewSecondaryCnxState( state->dspNum );
               if ( state->cnx[PRIMARY_CNX_INDEX] )
               {
                  state->cnx[PRIMARY_CNX_INDEX]->lineId = state->lineId;
               }
            }
            else
            {
               state->cnx[PRIMARY_CNX_INDEX]->lineId = state->lineId;
            }
         }
#endif

         /* Determine the index of the current cnx in the endpoint's cnx queue */
         for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
         {
            if (state->cnx[cnxIndex])
            {
                if ((state->endptType == EPTYPE_FXS)||( state->endptType == EPTYPE_DECT ))
                {
                  /* Update the current hookstate */
                  if ( state->bGR303Support )
                  {
                     /* Update the current hookstate */
                     GenerateNteEvent( state->cnx[cnxIndex], RTP_NTE_OFFHOOK );
                  }
                }
            }
         }

         /* Do not invoke CLASS state machine for PSTN (FX0) endpoint. Class STM is only for
           for Caller ID Tx, which PSTN VHD does not support */
         if ((state->endptType == EPTYPE_FXS)||( state->endptType == EPTYPE_DECT ))
         {
            /* Post the event for the caller ID state machine */
            classStmEventProcess( &state->classState, CLASSSTM_OFFHOOK, state->lineId );
         }
      }
      break;

      case EPCAS_RINGON:
      {
         if( state->endptType == EPTYPE_PSTN )
         {
            pstnEvt.hsxdevhdl = state->pstnVhdHdl;
            pstnEvt.hsxevt = PSTN_CTL_EVT_RINGON;
            pstnEvt.hsxop1 = 0;
            pstnEvt.hsxop2.ref = NULL;
            if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &pstnEvt ) )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_RINGON FAIL" ));
            }
         }
      }
      break;

      case EPCAS_RINGOFF:
      {
         if( state->endptType == EPTYPE_PSTN )
         {
            pstnEvt.hsxdevhdl = state->pstnVhdHdl;
            pstnEvt.hsxevt = PSTN_CTL_EVT_RINGOFF;
            pstnEvt.hsxop1 = 0;
            pstnEvt.hsxop2.ref = NULL;
            if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &pstnEvt ) )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_RINGOFF FAIL" ));
            }
         }
      }
      break;

      case EPCAS_RINGEND:
      {
         if( state->endptType == EPTYPE_PSTN )
         {
            pstnEvt.hsxdevhdl = state->pstnVhdHdl;
            pstnEvt.hsxevt = PSTN_CTL_EVT_RINGEND;
            pstnEvt.hsxop1 = 0;
            pstnEvt.hsxop2.ref = NULL;
            if ( PSTN_CTL_SUCCESS != pstnCtlSendEvent( &pstnEvt ) )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "pstnCtlSendEvent: PSTN_CTL_EVT_RINGEND FAIL" ));
            }
         }
      }
      break;

      case EPCAS_FIRST_RINGOFF:
      {
         /*
         ** First ring-off - it's time to start the CallerID Transmitter
         ** Issue a command to the DSP to indicate this is the time
         ** to start transmitting the CallerID message (if one had
         ** previously been sent to the DSP it will be transmitted now).
         */

         /* Post the event for the caller ID state machine */
         classStmEventProcess( &state->classState, CLASSSTM_CLID_FIRST_RINGOFF, state->lineId );
      }
      break;

      case EPCAS_VMWI_OSI_SENT:
      {
         /* Post the event for the caller ID state machine */
         classStmEventProcess( &state->classState, CLASSSTM_VMWI_OSI_END, state->lineId );
      }
      break;

      case EPCAS_FLASH:
      {
         /* Queue the event for the caller ID state machine */
         classStmEventProcess( &state->classState, CLASSSTM_FLASH, state->lineId );

#if VRG_ENDPT_CFG_USE_HFE
         /* Begin Hook-Flash Emulation */
         if ( state->bGR303Support && state->bPulse2dtmf)
         {
            /* Determine the index of the current cnx in the endpoint's cnx queue */
            for( cnxIndex = 0; cnxIndex < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; cnxIndex++ )
            {
               if ( state->cnx[cnxIndex] )
               {
                  /* Update the current hookstate and generate onhook event as required */
                  GenerateNteEvent( state->cnx[cnxIndex], RTP_NTE_ONHOOK );
               }
            }

            bosTimerStart( &state->hookflashTimer, VRG_ENDPT_CFG_HFE_INTERVAL, BOS_TIMER_MODE_ONCE);
         }
#endif
      }
      break;


      default:
      {
      }
      break;
   }

   return(EPSTATUS_SUCCESS);
}

/*
*****************************************************************************
** FUNCTION:   GenerateNteEvent
**
** PURPOSE:    Generate NTE event
**             (for GR303 Nuera onhook / offhook events, hookstate will be
**              sent to LDX RTP service, which will generate the event if
**              there is a hookstate change or in 5 sec intervals)
**
** PARAMETERS: cnx     - pointer to cnx
**             event   - event to generate
**
** RETURNS:    RTP packet type
**
*****************************************************************************
*/
void GenerateNteEvent( CNXSTATE *cnx, int event )
{
   VRG_SINT16 status;
   int vhdHdl = cnx->vhdhdl;

   if ( cnx->voiceMode != VM_ACTIVE || cnx->vhdMode == NETMODE_FAXRELAY )
   {
      return;
   }

   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Generate NTE event %d for line %d", event, cnx->lineId ));

   status = hdspSendCmd( vhdHdl,
                         HAPI_RTP_GEN_NTE_CMD,
                         HSCMDDATA,
                         event,
                         0);

   if( status )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR generating NTE event for line %d", cnx->lineId ));
   }

}

/*****************************************************************************
*  FUNCTION:   GetCnxState
*
*  PURPOSE:    This function is used to retreive a pointer to the connection's
*              instance memory.
*
*  PARAMETERS: cnxId - the connection index
*
*  RETURNS:    connection state pointer, NULL on invalid index.
*
*  NOTES:
*
*****************************************************************************/
CNXSTATE* GetCnxState( int cnxId )
{
   int i;

#if FLEXIBLE_CONFERENCING_SUPPORTED
   if ( cnxId > CNX_UNUSED )
#else
   if ( cnxId != CNX_UNUSED )
#endif
   {
      for (i = 0; i < VRG_ENDPT_CFG_NUM_CNX; i++)
      {
         if ( vrgCnxState[i].cnxId == cnxId )
         {
            return &vrgCnxState[i];
         }
      }
   }
   return(NULL);
}

/*****************************************************************************
*  FUNCTION:   GetCnxStateFromVHD
*
*  PURPOSE:    This function is used to retreive a pointer to the connection's
*              instance memory given the connection VHD handle.
*
*  PARAMETERS: vhdhdl - the connection VHD handle
*
*  RETURNS:    connection state pointer, NULL on invalid index.
*
*  NOTES:
*
*****************************************************************************/
CNXSTATE* GetCnxStateFromVHD( int vhdhdl )
{
   int i;

   for (i = 0; i < VRG_ENDPT_CFG_NUM_CNX; i++)
   {
      if ( vrgCnxState[i].vhdhdl == vhdhdl )
      {
         return ( &vrgCnxState[i] );
      }
   }

   return ( NULL );
}

/*
*****************************************************************************
** FUNCTION:   GetDigitEvent
**
** PURPOSE:    Converts a PTE DTMF digit to an endpoint DTMF event
**
** PARAMETERS: pteDigit - The PTE digit received
**
** RETURNS:    The DTMF event
**
** NOTE:       None.
*****************************************************************************
*/
EPEVT GetDigitEvent( HAPI_PTE_DTMFDDGT pteDigit )
{
   const EPZPTEDIGIT2EPEVTMAP *digitMap = digit2epevtMap;

   while(( digitMap->digit != pteDigit ) &&
         ( digitMap->digit != HAPI_PTE_DTMFDDGTERROR ))
   {
      digitMap++;
   }

   return digitMap->epEvent;
}

/*****************************************************************************
*  FUNCTION:   GetEndptIndexFromCasHandle
*
*  PURPOSE:    This function is used to retreive a pointer to the endpoint's
*              instance memory, based upon the CAS handle.
*
*  PARAMETERS: casHandle   (in)  The endpoint CAS handle.
*              endptIndex  (out) Channel number index.
*
*  RETURNS:    TRUE or FALSE.
*
*****************************************************************************/
VRG_UINT16 GetEndptIndexFromCasHandle( CAS_CTL_HANDLE casHandle, VRG_UINT16 *endptIndex )
{
   int idx;

   for ( idx = 0; idx < vrgEndptGetNumEndpoints(); idx++)
   {
      if ( vrgEndpt[ idx ].casCtlHandle == casHandle )
      {
         *endptIndex = idx;
         return ( VRG_TRUE );
      }
   }

   return ( VRG_FALSE );
}

/*****************************************************************************
*  FUNCTION:   GetEndptState
*
*  PURPOSE:    This function is used to retreive a pointer to the endpoint's
*              instance memory.
*
*  PARAMETERS: lineId - the endpoint line identifier
*
*  RETURNS:    endpoint state pointer, NULL on invalid index.
*
*****************************************************************************/
VRG_ENDPT* GetEndptState( int lineId )
{
   int i;

   for (i = 0; i < vrgEndptGetNumEndpoints(); i++)
   {
      if (( vrgEndpt[i].lineId == lineId ) && ( vrgEndpt[i].isLineUsed == VRG_TRUE ))
      {
         return &vrgEndpt[i];
      }
   }

   return(NULL);
}

/*****************************************************************************
*  FUNCTION:   GetEndptStateFromPhysId
*
*  PURPOSE:    This function is used to retreive a pointer to the endpoint's
*              instance memory.
*
*  PARAMETERS: physId - physical endpoint number ( 0 referenced )
*
*  RETURNS:    endpoint state pointer, NULL if invalid.
*
*****************************************************************************/
VRG_ENDPT* GetEndptStateFromPhysId( VRG_UINT16 physId )
{

   if ( physId < vrgEndptGetNumEndpoints() )
   {
      if ( vrgEndpt[ physId ].isLineUsed == VRG_TRUE )
      {
         return &vrgEndpt[ physId ];
      }
   }
   return(NULL);
}

/*****************************************************************************
*  FUNCTION:   GetEndptStateFromVHD
*
*  PURPOSE:    This function is used to retreive a pointer to the endpoint's
*              instance memory, based upon the endpt VHD handle.
*
*  PARAMETERS: vhdhdl - the endpoint VHD handle
*
*  RETURNS:    endpoint state pointer, NULL if invalid.
*
*****************************************************************************/
VRG_ENDPT* GetEndptStateFromVHD( VRG_UINT16 vhdHdl )
{
   int i;

   for (i = 0; i < vrgEndptGetNumEndpoints(); i++)
   {
      if ( vrgEndpt[i].lineVhdHdl == vhdHdl )
      {
         return &vrgEndpt[i];
      }

      if ( vrgEndpt[i].pstnVhdHdl == vhdHdl )
      {
        return &vrgEndpt[i];
      }
   }

   return(NULL);
}

/*****************************************************************************
*  FUNCTION:   GetNewSecondaryCnxState
*
*  PURPOSE:    This function is used to retreive a pointer to the instance
 *             memory of an unused secondary connection.
*
*  PARAMETERS: dspNum - the DSP number to get the connection associated with
*
*  RETURNS:    Connection state pointer, NULL if none available
*
*  NOTES:      Secondary connections are not bound to a specific endpoint.
*              They are available to be used by any endpoint.
*
*****************************************************************************/
CNXSTATE* GetNewSecondaryCnxState( int dspNum )
{
   int i;

#if FLEXIBLE_CONFERENCING_SUPPORTED
   for ( i = 0; i < VRG_ENDPT_CFG_NUM_CNX; i++ )
#else
   for ( i = VRG_ENDPT_CFG_NUM_ENDPT; i < VRG_ENDPT_CFG_NUM_CNX; i++ )
#endif
   {
      if ( vrgCnxState[i].cnxId == CNX_UNUSED &&
           vrgCnxState[i].dspNum == dspNum )
      {
#if FLEXIBLE_CONFERENCING_SUPPORTED
         vrgCnxState[i].cnxId = CNX_IDLE;
#endif
         return ( &vrgCnxState[i] );
      }
   }

   return ( NULL );
}

VRG_ENDPT* GetPstnVrgEndptState( void )
{
   int lineId;

   for (lineId = 0; lineId < vrgEndptGetNumEndpoints(); lineId++)
   {
      if (boardHalIsDaa(lineId))
      {
         return( GetEndptState( lineId ) );
      }
   }

   return( NULL );
}

/*
*********************************************************************************
** FUNCTION:   GetRingPattern
**
** PURPOSE:    Perform a table lookup to determine the prov id 
**             (or CAS profile for special countries) given the endpoint ringing signal type.
**
** PARAMETERS: ringSignal   (in) Endpoint ring signal type
**             country      (in) Country type
**             pattern      (out) pointer to ring pattern structure to fill in.
**
** RETURNS:    void
**
** NOTE:
*********************************************************************************
*/
void GetRingPattern
(
   EPSIG                ringSignal,
   VRG_COUNTRY          country,
   CLASSSTM_RING_STATE *pattern
)
{
   int provItem;
   
   XCHG_ASSERT( country < VRG_COUNTRY_MAX );

   /*
   ** fetch the associated provId. If the country does not support dynamic provisioning, 
   ** fetch the CAS ring pattern index instead   
   */
   globeEndptGetRingPattern(country, ringSignal, pattern, &provItem);

   if ( provItem >= 0 )
   {
      /* If this pattern is provisionable, overwrite the static profile
      ** with the provisionable bit pattern */
      VRG_RING_PROFILE provProfile;

      /* Retrieve provisionable ring cadence */
      EPSTATUS status = Provision( vrgEndpt[0].endptArchive,
                                   provItem,
                                   &provProfile);
      if (status == EPSTATUS_SUCCESS)
      {
         translateRingProfile(&provProfile, pattern);
      }
   }
   else
   {
      /* Check if this is E-line package cadence ringing.  Since there are
      ** 128 possible cadence ringing in the E-line package, we don't
      ** want to add 128 'cases'.  So we will just check whether the
      ** specified signal is within the cadence ringing range */
      if ( ( ringSignal >= EPSIG_CADENCE_RING_BASE ) &&
           ( ringSignal <= EPSIG_CADENCE_RING_MAX ) )
      {
         int      index;
         int      cadenceIndex;

         /* Determine the signal offset from the base */
         index = ringSignal - EPSIG_CADENCE_RING_BASE;
         cadenceIndex = EPPROV_CadenceRingBase + index;

         Provision( vrgEndpt[0].endptArchive, cadenceIndex, pattern );
      }
   }

   /* Retrieve CLIDTX delay from the MIB */
   Provision( vrgEndpt[0].endptArchive,
              EPPROV_CIDFskAfterRing,
              &pattern->clidTxDelay );


   switch (pattern->ringType)
   {
      case CAS_CTRL_LONG_BRIEF_RING:
      case CAS_CTRL_LONG_NORMAL_RING:
      {
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Long ring info: patternId=%d; type=%d; delay=%u",
                   pattern->pattern, pattern->ringType, (unsigned int)pattern->clidTxDelay));

         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "   P1-P4: 0x%08x 0x%08x 0x%08x 0x%08x",
                                          (unsigned int)pattern->provBits.longRingPattern.pattern1,
                                          (unsigned int)pattern->provBits.longRingPattern.pattern2,
                                          (unsigned int)pattern->provBits.longRingPattern.pattern3,
                                          (unsigned int)pattern->provBits.longRingPattern.pattern4 ));
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "   P5-P7: 0x%08x 0x%08x 0x%08x",
                                          (unsigned int)pattern->provBits.longRingPattern.pattern5,
                                          (unsigned int)pattern->provBits.longRingPattern.pattern6,
                                          (unsigned int)pattern->provBits.longRingPattern.pattern7 ));
      }
      break;

      case CAS_CTRL_SHORT_BRIEF_RING:
      case CAS_CTRL_SHORT_NORMAL_RING:
      {
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Short ring info: patternId=%d; type=%d; delay=%u",
                   pattern->pattern, pattern->ringType, (unsigned int)pattern->clidTxDelay));

         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "   P1-P2: 0x%08x 0x%08x",
                                          (unsigned int)pattern->provBits.shortRingPattern.pattern1,
                                          (unsigned int)pattern->provBits.shortRingPattern.pattern2 ));
      }
      break;

      case CAS_CTRL_STATIC_RING:
      {
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Static ring info: patternId=%d; type=%d; delay=%lu",
                   pattern->pattern, pattern->ringType, pattern->clidTxDelay));
      }
      break;

      default:
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT ERROR: GetRingPattern - unknown ring type %d", pattern->ringType));
      }
      break;
   }
}

/*
*****************************************************************************
** FUNCTION:   InitRtpServiceRegisters
**
** PURPOSE:    - set up RTP service registers to initial state
**
** PARAMETERS: cnx         - pointer to cnx block
**
** RETURNS:
**
*****************************************************************************
*/
void InitRtpServiceRegisters( CNXSTATE *cnx )
{
   VRG_ENDPT*        state;
   HAPI_RTP_REGS     rtpRegs;
   HAPI_RTP_REGS2    rtpRegs2;

   char* rtpRegsCname = (char*)rtpRegs.cname;
   VRG_UINT32 nteToneLevel;

   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Setting RTP service registers for line %d", cnx->lineId ));

   rtpRegs = cnx->rtpRegs;
   rtpRegs2 = cnx->rtpRegs2;

   rtpRegs.configFlags |= HAPI_RTP_EGRESS_REPORT_ERROR_TO_HOST_ENABLE_MASK;
   rtpRegs.configFlags |= HAPI_RTP_INGRESS_SEND_RTCP_PACKETS_ENABLE_MASK;
   rtpRegs.configFlags &= ~HAPI_RTP_INGRESS_SEND_RTCP_PACKETS_TO_HOST_ENABLE_MASK;
   rtpRegs.configFlags &= ~HAPI_RTP_EGRESS_SEND_RTCP_PACKETS_TO_HOST_ENABLE_MASK;
   rtpRegs.configFlags &= ~HAPI_RTP_EGRESS_CHECK_FOR_SSRC_COLLISION_MASK;
   rtpRegs.configFlags &= ~HAPI_RTP_EGRESS_DROP_REDUNDANT_PAYLOAD_MASK;
   rtpRegs.configFlags &= ~HAPI_RTP_INGRESS_RFC2198_ENABLE_MASK;
   rtpRegs.configFlags |= HAPI_RTP_INGRESS_GENERATE_RTCP_BYE_PACKET_ENABLE_MASK;
   rtpRegs.configFlags &= ~HAPI_RTP_EGRESS_ACCEPT_RTCP_BYE_PACKET_ENABLE_MASK;
   rtpRegs.configFlags &= ~HAPI_RTP_QOS_EVENT_ENABLE_MASK;

   /* Disable RTP send mode before activitating voice */
   rtpRegs2.configFlags2 |= HAPI_RTP_INGRESS_SEND_RTP_PACKETS_DISABLE_MASK;
   rtpRegs2.configFlags2 |= HAPI_RTP_EGRESS_PROCESS_RTP_PACKETS_DISABLE_MASK;
   rtpRegs.configFlags &= ~HAPI_RTP_INGRESS_SEND_RTCP_XR_PACKETS_ENABLE_MASK;
#if VRG_ENDPT_CFG_G726_ITU_I_366_2_ANNEX_E
   rtpRegs2.configFlags2 |= HAPI_RTP_G726_ANNEX_E_ENABLE_MASK;
#endif

   /* Disable V152 VBD mode */
   rtpRegs.configFlags &= ~HAPI_RTP_VBD_V152_ENABLE_MASK;
   rtpRegs.configFlags &= ~HAPI_RTP_VBD_V152_INGRESS_RFC2198_ENABLE_MASK;

   if ( cnx->localSsrc != 0 )
   {
      rtpRegs.configFlags |= HAPI_RTP_USE_USER_DEFINED_SSRC_MASK;
   }

   state = GetEndptState( cnx->lineId );
   if( ! state )
   {
      return;
   }

   if (Provision( state->endptArchive,
                  EPPROV_Rfc2833ToneLevel,
                  &nteToneLevel ) == EPSTATUS_SUCCESS)
   {
      rtpRegs.nteToneLevel = nteToneLevel;
   }

   rtpRegs2.configFlags2 |= HAPI_RTP_TONE_TRIPLICATE_REDUNDANCY_MASK;

   if ( state->bGR303Support )
   {
      /* Triple redundancy is disabled for GR303 Nuera Support */
      rtpRegs2.configFlags2 |= HAPI_RTP_GR303_NUERA_ENABLE_MASK;
      rtpRegs2.configFlags2 |= HAPI_RTP_RFC2833_KEEP_ALIVE_ENABLE_MASK;
   }
   else
   {
      rtpRegs2.configFlags2 &= ~HAPI_RTP_GR303_NUERA_ENABLE_MASK;
      rtpRegs2.configFlags2 &= ~HAPI_RTP_RFC2833_KEEP_ALIVE_ENABLE_MASK;
   }

   /* Enable / Disable DTMF subtraction */
   if ( cnx->digitRelayType == EPDTMFRFC2833_SUBTRACT )
   {
      rtpRegs2.configFlags2 |= HAPI_RTP_SUBTRACT_DTMF_ENABLE_MASK;
   }
   else
   {
      rtpRegs2.configFlags2 &= ~HAPI_RTP_SUBTRACT_DTMF_ENABLE_MASK;
   }

   /* Initialize RTCP SDES CNAME data to random value -- this should NOT change within a call session */
   sprintf( rtpRegsCname, "%d", (int)net_random() );
   rtpRegs.cnameLength = strlen(rtpRegsCname);

   /* Cache into a local copy for future use */
   cnx->rtpRegs = rtpRegs;
   cnx->rtpRegs2 = rtpRegs2;
}

/*****************************************************************************
*  FUNCTION:   InitVhds
*
*  PURPOSE:    To open and initialize the endpoint and connection VHDs for
*              a specific endpoint.
*
*  PARAMETERS: state - endpoint state structure
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS InitVhds( VRG_ENDPT* state )
{
#if !FLEXIBLE_CONFERENCING_SUPPORTED
   EPSTATUS       epStatus;
#endif
   HAPICALLBKP    endptVhdCallback;
#if !FLEXIBLE_CONFERENCING_SUPPORTED
   VRG_BOOL       bLiteWeightCnx = VRG_FALSE;
#endif
   HAPI_PTE_ADM   pteAdmin;
   VRG_UINT32     minDetectPower;
   VRG_UINT32     pveEgressVolume;
   VRG_SINT16     status;
   VRG_SINT32     gain;
   VOICE_BOARD_PARMS* voiceParams;

#if !FLEXIBLE_CONFERENCING_SUPPORTED
   /*
   ** Open the primary connection VHD
   */
   if ( (VRG_SINT16)state->endptNum >= ( VRG_ENDPT_CFG_NUM_CNX - gDspCfg.maxLiteCnxs ))
   {
      bLiteWeightCnx = VRG_TRUE;
   }
#endif

   /* Retrieve the country specific PVE/PTE settings */
   Provision(state->endptArchive, EPPROV_PteDetectAdmin, &pteAdmin);
   Provision(state->endptArchive, EPPROV_PteMinDetectPower, &minDetectPower);
   Provision(state->endptArchive, EPPROV_PveEgressVolume, &pveEgressVolume);
   /*
   ** Open the line VHD
   */
   endptVhdCallback = EndptVhdEventCallback;

   /* Parms: cnxState, primaryCnx, lite-weightCnx */
   if( state->endptType == EPTYPE_FXS || state->endptType == EPTYPE_DECT)
   {
#if !FLEXIBLE_CONFERENCING_SUPPORTED
      epStatus = InitCnxVhd( state->cnx[PRIMARY_CNX_INDEX], VRG_TRUE, bLiteWeightCnx, state->country );
      if ( epStatus != EPSTATUS_SUCCESS )
      {
         return ( EPSTATUS_RESOURCES );
      }
#endif
   }
   else if ( state->endptType == EPTYPE_PSTN )
   {
      status = hdspVhdOpen( HAPI_PSTN_VHD, endptVhdCallback, state->dspNum,
                            pteAdmin, minDetectPower, pveEgressVolume,
                            &(state->pstnVhdHdl) );
      if( status != HAPISUCCESS )
      {
         VRG_LOG_CRIT(( VRG_LOG_MOD_EPT, "ERROR hdspVhdOpen PSTN VHD failed. DSP %d", state->dspNum ));
         return ( EPSTATUS_RESOURCES );
      }
      else
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "hdspVhdOpen PSTN VHD success. DSP %d, VHD (0x%x) of type: 0x%x",
                   state->dspNum, state->pstnVhdHdl, HAPI_PSTN_VHD ));
      }
   }

   if ( ( state->endptType == EPTYPE_FXS )||( state->endptType == EPTYPE_PSTN ) ||( state->endptType == EPTYPE_NOSIG )||
        ( state->endptType == EPTYPE_DECT ))
   {
      status = hdspVhdOpen( HAPI_LINE_ECAN_VHD, endptVhdCallback, state->dspNum,
                            pteAdmin, minDetectPower, pveEgressVolume,
                            &(state->lineVhdHdl) );
      if( status != HAPISUCCESS )
      {
         VRG_LOG_CRIT(( VRG_LOG_MOD_EPT, "ERROR hdspVhdOpen Line VHD failed. DSP %d",
                   state->dspNum ));
         return ( EPSTATUS_RESOURCES );
      }
      else
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "hdspVhdOpen Line VHD success. DSP %d, VHD (0x%x) of type: 0x%x",
                   state->dspNum, state->lineVhdHdl, HAPI_LINE_ECAN_VHD ));
      }

      /* If PSTN endpoint, register the PSTN VHD and FXO Line VHD handles with the VHD state control block */
      if ( state->endptType == EPTYPE_PSTN )
      {
         hdspVhdStateControlSetPstnInfoBlock( state->pstnVhdHdl, state->lineVhdHdl );
      }
   }
   else
   {
      state->lineVhdHdl = 0xFF;
   }

   /* Retrieve the hausware HAL handle to connect to */
   voiceParams = boardHalProvGetVoiceParms();
   if( ( boardHalApmGetChanSwapStatus() == XDRV_TRUE ) && ( voiceParams->flags & BP_FLAG_DSP_APMHAL_ENABLE ) )
   {
      /* SWAP LDX HAL assignments if APM channels are swapped */
      state->ldxHalHdl = hdspHalGetHandle( ((vrgEndptGetNumFxsEndpoints()-1) - state->endptNum), state->dspNum );
   }
   else
   {
      state->ldxHalHdl = hdspHalGetHandle( state->endptNum, state->dspNum );
   }

   /*
   ** A line VHD connected to a narrowband HAL device can run in narrowband mode at a 5ms frame
   ** rate. This step MUST be performed before establishing the switchboard connection between the
   ** line VHD and the HAL device.
   */
   if ( boardHalDspIsNarrowbandHal(state->ldxHalHdl - HAPIHAL0) == XDRV_TRUE )
   {
      hdspRmSetFramesizeAndMode(state->lineVhdHdl, NARROWBAND_FRAME_SIZE, HAPI_RM_VHD_NARROWBAND_MODE);
   }

#if !VRG_ENDPT_CFG_EGRESSPACKET_LOOPBACK
   /*
   ** Make a switchboard connection between the ECAN line vhd and the HAL device
   */
   if ( HAPISUCCESS != hdspRmSwitchBoardConnect( state->lineVhdHdl,
                                                 HAPI_SWB_BOS,
                                                 state->ldxHalHdl,
                                                 HAPI_SWB_TOS,
                                                 HAPI_SWB_DUPLEX,
                                                 VRG_TRUE ) )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting switchboard connection for endpt %d", state->endptNum ));
      return ( EPSTATUS_RESOURCES );
   }
#endif

   /* Set up duplex connection between PSTN VHD and Line VHD */
   if ( ( state->endptType == EPTYPE_PSTN ) )
   {

       if ( HAPISUCCESS != hdspRmSwitchBoardConnect( state->pstnVhdHdl,
                                                 HAPI_SWB_BOS,
                                                 state->lineVhdHdl,
                                                 HAPI_SWB_TOS,
                                                 HAPI_SWB_DUPLEX,
                                                 VRG_TRUE ) )
       {
          VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting switchboard connection for endpt %d", state->endptNum ));
          return ( EPSTATUS_RESOURCES );
       }

   }

   /*
   ** Initialize the Ecan State Machine
   */
   EcanStmInit(state);


   /*
   ** Configure ingress gain - default is 0 dB
   */
   if( !Provision( state->endptArchive,
                  EPPROV_TxGain,
                  &gain ) == EPSTATUS_SUCCESS )
   {
      gain = state->txGain;
   }
   else
   {
      /* If provisioning was successful, update */
      state->txGain = gain;
   }
   hdspVhdSetIngressGain(state->lineVhdHdl, gain);

   /*
   ** Configure egress gain - default is 0 dB
   */
   if( !Provision( state->endptArchive,
                  EPPROV_RxGain,
                  &gain ) == EPSTATUS_SUCCESS )
   {
      gain = state->rxGain;
   }
   else
   {
      /* If provisioning was successful, update */
      state->rxGain = gain;
   }
   hdspVhdSetEgressGain(state->lineVhdHdl, gain);

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   IsCnxModeSupported
**
** PURPOSE:    Verifies whether endpoint supports the current connection mode
**
** PARAMETERS: mode - current mode to check against
**             dspNum - The DSP number to check the configuration from
**
** RETURNS:    TRUE - endpoint supports the current connection mode
**             FALSE - current mode not supported by the endpoint
**
** NOTE:
*****************************************************************************
*/
VRG_BOOL IsCnxModeSupported(EPCNXMODE mode, int dspNum)
{
   unsigned int i = 0;

   /*
   ** Iterate through the endpoint's connection mode capabilities list
   ** to determine whether the requested mode is supported.
   */
   while ( ( vrgEndptCap[dspNum].mCap[i] != (int) EPCNXMODE_MAX )  &&
           ( vrgEndptCap[dspNum].mCap[i] != (int) mode )           &&
           ( i < EPCNXMODE_MAX ) )
   {
      i++;
   }

   if ( vrgEndptCap[dspNum].mCap[i] == (int) mode )
   {
      return ( VRG_TRUE );
   }

   return ( VRG_FALSE );
}

/*
*****************************************************************************
** FUNCTION:   IsCnxVhd
**
** PURPOSE:    Check if given vhdHdl is a connection VHD handle
**
** PARAMETERS: vhdHdl - VHD handle
**
** RETURNS:    True if handle represents a connection VHD
**
** NOTE:
*****************************************************************************
*/
VRG_BOOL IsCnxVhd( VRG_UINT16 vhdHdl )
{
   int               i;
   CNXSTATE         *cnxState;

   for ( i = 0; i < VRG_ENDPT_CFG_NUM_CNX; i++ )
   {
      if ( ( cnxState = &vrgCnxState[i] ) && (cnxState->vhdhdl == vhdHdl) )
      {
         return ( VRG_TRUE );
      }
   }

   return ( VRG_FALSE );
}

/*
*****************************************************************************
** FUNCTION:   IsT38Negotiated
**
** PURPOSE:    Returns true if T.38 fax relay has been negotiated for the
**             specified endpt and the DSP has switched to fax relay mode
**             to accept T.38 packets
**
** PARAMETERS: cnx   (in) Connection state.
**
** RETURNS:    TRUE or FALSE.
**
** NOTE:
*****************************************************************************
*/
VRG_BOOL IsT38Negotiated( CNXSTATE  *cnx )
{
   int dspNum = cnx->lineId / (VRG_ENDPT_CFG_NUM_ENDPT/VRG_GLOBAL_CFG_MAX_NUM_DSP);

   /* Verify that fax relay is supported by the DSP. */
   if ( vrgEndptCap[dspNum].codecCap[CODEC_T38] == CODEC_UNSUPPORTED )
   {
      return ( VRG_FALSE );
   }


   /* Verify that fax relay has been selected as the data handling mode. */
   if ( ( cnx->dataMode != EPDATAMODE_T38 )           &&
        ( cnx->dataMode != EPDATAMODE_T38_MUTE ) )
   {
      return ( VRG_FALSE );
   }

   /* Verify that the DSP has switched to fax relay mode.  If the DSP has not
   ** switched to T.38, the endpt should not attempt to send it any egress
   ** T.38 packets.  Note that this check is omitted for the case when
   ** the data mode is autoenc.  By using the autoenc mode, it lets
   ** the DSP handle any egress T.38 processing. */
   if ( ( cnx->dataMode == EPDATAMODE_T38 ) ||
        ( cnx->dataMode == EPDATAMODE_T38_MUTE ) )
   {
      if ( cnx->vhdMode != NETMODE_FAXRELAY )
      {
         return ( VRG_FALSE );
      }
   }


   return ( VRG_TRUE );
}

/*****************************************************************************
*  FUNCTION:   MapRtpPayloadType
*
*  PURPOSE:    Map the RTP payload type that corresponds to an Endpoint codec
*              type
*
*  PARAMETERS: codec   - xChange endpoint codec type
*              mediaPayloadType - mediaPayloadType
*
*  RETURNS:    1 - if the table was updated.
*              0 - otherwise (skipped the entry).
*
*  NOTE:
*****************************************************************************/

int MapRtpPayloadType ( VRG_UINT16 codec, VRG_UINT8 payloadType, HAPI_UINT16* mediaPayloadTypeMap )
{
   HAPI_UINT16 mediaPayloadType = 0;
   int i;

   /*
    * This assumes that capabilities are the same between multiple DSP images (DSL)
    * if we have more than one. So we just pick the first element of the array
    */
   if (( codec == HAPI_PKT_VC_G729_8 ) && ( vrgEndptCap[0].codecCap[CODEC_G729A] == CODEC_SUPPORTED ))
   {
      codec = HAPI_PKT_VC_G729A_8;
   }

   mediaPayloadType = HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY( payloadType, codec );

   /* Start at index 1 so the first entry will always be mapping for PCMU codec. */
   for( i = 1 ; i < HAPI_RTP_MAX_N_CODECS ; i++ )
   {
      if( mediaPayloadType == mediaPayloadTypeMap[i] )
      {
         return 0;
      }

      /* Do not clear the RTP payload type map if we are updating
      ** G723.x payload type.  This ensures that both G723.1-5.3
      ** and G723.1-6.3 will have valid RTP payload types. */
      if ( payloadType == HAPI_RTP_GET_PAYLOAD_TYPE_FROM_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(mediaPayloadTypeMap[i]) &&
           (!(HAPI_PKT_IS_7231( HAPI_RTP_GET_CODEC_FROM_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(mediaPayloadTypeMap[i]))) ||
            !(HAPI_PKT_IS_7231( codec )) ) )
      {
         mediaPayloadTypeMap[i] = 0;
      }

      if ( mediaPayloadTypeMap[i] == 0 )
      {
         mediaPayloadTypeMap[i] = mediaPayloadType;
         return 1;
      }
   }

   return 0;
}

/*
*****************************************************************************
** FUNCTION:   ModeCheck
**
** PURPOSE:    Verifies whether the current mode can send/recv voice streams
**
** PARAMETERS: mode - current mode to check against
**             direction - send or receive voice streams
**
** RETURNS:    TRUE - current mode supports voice stream in the specified direction
**             FALSE - current mode does not support the specified voice stream direction
**
** NOTE:
*****************************************************************************
*/
VRG_BOOL ModeCheck(EPCNXMODE mode, CNX_DIRECTION direction)
{
   switch( direction )
   {
      case SENDMODE:
      {
         /* only send, send/recv, conf, replicate, netwcont can be in send mode */
         if(( mode == EPCNXMODE_SNDO ) || ( mode == EPCNXMODE_SNDRX ) ||
            ( mode == EPCNXMODE_CONF ) || ( mode == EPCNXMODE_REPLCATE ) ||
            ( mode == EPCNXMODE_TEST ) || ( mode == EPCNXMODE_TRANSCODE ) )
         {
            return VRG_TRUE;
         }
      }
      break;

      case RECVMODE:
      {
         /* only recvonly, send/recv, conf, netwtest can be in recv mode */
         if(( mode == EPCNXMODE_RCVO ) || ( mode == EPCNXMODE_SNDRX ) ||
            ( mode == EPCNXMODE_CONF )  || ( mode == EPCNXMODE_TEST) ||
            ( mode == EPCNXMODE_TRANSCODE ) )
         {
            return VRG_TRUE;
         }
      }
      break;

      default:
      {
      }
   }
   return VRG_FALSE;
}

/*
*****************************************************************************
** FUNCTION:   PrintVhdInfo
**
** PURPOSE:
**
** PARAMETERS:
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void PrintVhdInfo( void )
{
   int               i;
   VRG_ENDPT        *endptState;
   CNXSTATE         *cnxState;
   const char       *cnxTypeStr;



   for ( i = 0; i < vrgEndptGetNumEndpoints(); i++ )
   {
      if ( ( endptState = &vrgEndpt[i] ) && ( endptState->isLineUsed ) )
      {
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "VHD handle 0x%x - resource manager",
                   (endptState->dspNum << 8) | HAPIRESMAN ));
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "VHD handle 0x%x - endpt %d VHD", endptState->lineVhdHdl, i ));

#if !FLEXIBLE_CONFERENCING_SUPPORTED
         cnxState = endptState->cnx[ PRIMARY_CNX_INDEX ];
         if ( cnxState != NULL )
         {
            if ( cnxState->bLiteWeight )
            {
               cnxTypeStr = "lite";
            }
            else
            {
               cnxTypeStr = "heavy";
            }

            VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "VHD handle 0x%x - primary (%s) cnx %d, endpt %d",
                       cnxState->vhdhdl, cnxTypeStr, (int)cnxState->cnxId, i ));
         }
#endif
      }
   }

#if FLEXIBLE_CONFERENCING_SUPPORTED
   for ( i = 0; i < VRG_ENDPT_CFG_NUM_CNX; i++ )
#else
   for ( i = VRG_ENDPT_CFG_NUM_ENDPT; i < VRG_ENDPT_CFG_NUM_CNX; i++ )
#endif
   {
      cnxState = &vrgCnxState[i];
      if ( cnxState )
      {
         if ( cnxState->bLiteWeight )
         {
            cnxTypeStr = "lite";
         }
         else
         {
            cnxTypeStr = "heavy";
         }

         VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "VHD handle 0x%x - secondary (%s) cnx %d, endpt %d",
                    cnxState->vhdhdl, cnxTypeStr, (int)cnxState->cnxId, cnxState->lineId ));
      }
   }
}

/*****************************************************************************
*  FUNCTION:   Provision
*
*  PURPOSE:    Look up the provisioned value of an item. If that item is not
*              provisioned, then a country specific default value will be
*              looked up via "Endpoint Around the Globe" API .
*              If both of these methods fail, an error will be returned.
*
*  PARAMETERS: Same as endptGetProvCallback
*
*  RETURNS:    EPSTATUS
*
*****************************************************************************/

EPSTATUS Provision( ENDPT_STATE *endptState, EPPROV provItemId, void *provItemValue )
{
   EPSTATUS rc = EPSTATUS_UNKNOWN_TYPE;
   int lineId = (endptState != NULL) ? endptState->lineId : 0;

   rc = endptProvGetValue(lineId, provItemId, provItemValue);
   
   if ( rc != EPSTATUS_SUCCESS )
   {
      /* Item has not been provisioned.
      ** An error message will be printed */
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Couldn't get the prov item !!! provItemID %d", provItemId));
  
   }
   return ( rc );
}

EPSTATUS ProvisionRingParams( VRG_ENDPT *state, EPRINGPARAM *ringParam )
{
   EPSTATUS status;
   VRG_BOOL ringHighVoltage;

   #define FREQUENCY_DEFAULT    20
   #define WAVEFORM_DEFAULT     XDRV_RINGWAVEFORM_SINE
   #define WAVEFORM_DEFAULT_STR "sine"
   #define VOLTAGE_DEFAULT      50
   #define VOLTAGE_LOW          40
   #define VOLTAGE_HIGH         57
   #define OFFSET_DEFAULT       25
   #define OFFSET_LOW           0
   #define OFFSET_HIGH          25
   #define OFFSET_CAL_DEFAULT   0

   /* Retrieve the provisioned power ring frequency */
   status = Provision( state->endptArchive, EPPROV_PowerRingFrequency, &(ringParam->frequency) );
   if ( status != EPSTATUS_SUCCESS )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Using ring frequency of %d Hz for endpt %d",
                                      FREQUENCY_DEFAULT, state->endptNum ));
      ringParam->frequency = FREQUENCY_DEFAULT;
   }

   /* Retrieve the provisioned ringing waveform shape */
   status = Provision( state->endptArchive, EPPROV_RingWaveform, &(ringParam->waveshape) );
   if ( status != EPSTATUS_SUCCESS )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Using %s ring waveform for endpt %d",
                                      WAVEFORM_DEFAULT_STR, state->endptNum ));
      ringParam->waveshape = WAVEFORM_DEFAULT;
   }

   /* Retrieve the provisioned ringing voltage */
   status = Provision( state->endptArchive, EPPROV_RingVoltage, &(ringParam->voltage) );
   if ( status != EPSTATUS_SUCCESS )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Using ring voltage of %d VRMS for endpt %d",
                                      VOLTAGE_DEFAULT, state->endptNum ));
      ringParam->voltage = VOLTAGE_DEFAULT;
   }
   else
   {
      if ((ringParam->voltage < VOLTAGE_LOW) || (ringParam->voltage > VOLTAGE_HIGH))
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT,
            "Ring voltage of %d not supported. Using default of %d VRMS for endpt %d",
            ringParam->voltage, VOLTAGE_DEFAULT, state->endptNum ));
         ringParam->voltage = VOLTAGE_DEFAULT;
      }
   }

   /* Retrieve the provisioned high voltage ring option */
   status = Provision( state->endptArchive, EPPROV_HighVring, &ringHighVoltage );
   if ( status != EPSTATUS_SUCCESS )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Using ring offset of %d V for endpt %d",
                                      OFFSET_DEFAULT, state->endptNum ));
   }

   if( ringHighVoltage != VRG_FALSE )
   {
      /* Retrieve the provisioned power ring offset */
      status = Provision( state->endptArchive, EPPROV_RingDCOffset, &(ringParam->offset) );
      if ( status != EPSTATUS_SUCCESS )
      {
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Using ring offset of %d V for endpt %d",
                                         OFFSET_DEFAULT, state->endptNum ));
         ringParam->offset = OFFSET_DEFAULT;
      }
      else
      {
         if (ringParam->offset >= OFFSET_HIGH)
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT,
               "Ring offset of %d not supported. Using default of %d V for endpt %d",
               ringParam->offset, OFFSET_DEFAULT, state->endptNum ));
            ringParam->offset = OFFSET_DEFAULT;
         }
      }
   }

   /* Retrieve the stored power ring offset calibration factor */
   status = Provision( state->endptArchive, EPPROV_RingOffsetCal, &(ringParam->offsetCal) );
   if ( status != EPSTATUS_SUCCESS )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Default ring offset calibration to %d V for endpt %d",
                                      OFFSET_CAL_DEFAULT, state->endptNum ));
      ringParam->offsetCal = OFFSET_CAL_DEFAULT;
   }

   return ( status );
}

/*****************************************************************************
*  FUNCTION:   ResetVBDataState
*
*  PURPOSE:    Clear the state of the voice-band data structure
*
*  PARAMETERS: vbData - pointer to voiceband data structure
*
*  RETURNS:    nothing
*
*  NOTE:
*
*****************************************************************************/
static void ResetVBDataState( EPZVBDATASTATE *pVbData )
{
   pVbData->bEnabled = VRG_FALSE;
}

/*
*****************************************************************************
** FUNCTION:   RtpInit
**
** PURPOSE:    Initialize endpoint's RTP state
**
** PARAMETERS: state    - pointer to endpoint's RTP state struct
**
** RETURNS:    RSSTATUS
*****************************************************************************
*/
void RtpInit( RTPSTATE* state )
{
   /* clear all state variables */
   memset( state, 0, sizeof( RTPSTATE ) );

   state->sendNtePayloadType = RTP_PAYLOAD_INVALID;
   state->recvNtePayloadType = RTP_PAYLOAD_INVALID;

   state->sendRedPayloadType = RTP_PAYLOAD_INVALID;
   state->recvRedPayloadType = RTP_PAYLOAD_INVALID;
}

/*****************************************************************************
*  FUNCTION:   SaveConnectionOptions
*
*  PURPOSE:    Save local codec parameters (e.g. codec type, period etc.)
*
*  PARAMETERS: cnx         - connection state instance memory
*              cnxParam    - connection parameters (codec, pktperiod, etc)
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
void SaveConnectionOptions( CNXSTATE *cnx, EPZCNXPARAM *cnxParam )
{
   if (!cnx->cnxModeControlsLineVhd)
   {
      cnx->mode = cnxParam->mode;
   }
   cnx->silence                 = cnxParam->silence;
   cnx->comfortNoise            = cnxParam->comfortNoise;
#if VRG_ENDPT_PACKET_TIME_CHECK
   cnx->period                  = checkPacketTime( cnxParam );
#else
   cnx->period                  = cnxParam->cnxParmList.send.period[0];
#endif // VRG_ENDPT_PACKET_TIME_CHECK
   cnx->codecType               = cnxParam->cnxParmList.send.codecs[0].type;
   cnx->namedPhoneEvts          = cnxParam->namedPhoneEvts;
   cnx->t38param.ecMode         = cnxParam->t38param.ecMode;
   cnx->vbData.param.vbdMode    = cnxParam->vbdparam.vbdMode;
   cnx->vbData.param.vbdRedMode = cnxParam->vbdparam.vbdRedMode;
}

/*****************************************************************************
*  FUNCTION:   SetConnectionOptions
*
*  PURPOSE:    Set the voice encoder, and silence suppression for a connection
*
*  PARAMETERS: cnx            - connection state instance memory
*              cnxParam       - connection parameters (codec, pktperiod, etc)
*              bInitialChange - flag to inidicate whether this is the
*                               initial voice rate change for this cnx.
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS SetConnectionOptions( CNXSTATE *cnx, EPZCNXPARAM *cnxParam,
                                      VRG_BOOL bInitialChange )
{
   VRG_ENDPT*        state;
   const EPZRATEMAP  *rateMap;
   VRG_UINT16        vadMode;
   int               i;
   VRG_BOOL          bEnableDigitRelay = VRG_FALSE;
   CODEC_TYPE        selectedCodec;
   unsigned int      baseFrameRate;
   VRG_UINT16        superPktInterval;
   VRG_UINT16        vadType;
   VRG_UINT16        xChgCodec;
   int               dspNum = 0;
   VRG_UINT16        plcType;
   VRG_UINT16        cng;

   VRG_BOOL          bMuteT38 = VRG_FALSE;
#ifdef BRCM_DUAL_3341
   dspNum = cnx->dspNum;
#endif
   VRG_BOOL          digitalMode = VRG_FALSE;

   state = GetEndptState( cnx->lineId );
   if( ! state )
   {
      return(EPSTATUS_ENDPT_UNKNOWN);
   }

   /*
   ** Check if we need to enable digital mode
   */
   if( cnxParam->cnxParmList.send.codecs[0].type == CODEC_CLEARCHANNEL )
   {
      digitalMode = VRG_TRUE;
   }

   /*
   ** Enable / disable digit relay
   */
   if ( digitalMode )
   {
      bEnableDigitRelay = VRG_FALSE;
   }
   else if ( cnxParam->digitRelayType == EPDTMFRFC2833_SUBTRACT )
   {
      bEnableDigitRelay = VRG_TRUE;
   }
   else if ( cnxParam->digitRelayType == EPDTMFRFC2833_ENABLED )
   {
      for ( i = 0; i < cnxParam->cnxParmList.send.numCodecs; i++ )
      {
         if ( cnxParam->cnxParmList.send.codecs[i].type == CODEC_NTE )
         {
            if ( cnxParam->namedPhoneEvts & CODEC_NTE_DTMF )
            {
               bEnableDigitRelay = VRG_TRUE;
            }
         }
      }
   }

   if ( cnxParam->localSsrc != 0 )
   {
      EPSTATUS epstatus = hdspVhdSetSSRC( cnx->vhdhdl, cnxParam->localSsrc );
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "Setting user-defined SSRC = 0x%08X", (unsigned int)cnxParam->localSsrc ));
      if ( epstatus != EPSTATUS_SUCCESS )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Couldn't set custom SSRC !!!" ));
         return ( epstatus );
      }
   }

   /* Quick return if the connection mode is inactive and there are no codec's specified yet */
   if ( (cnxParam->mode == EPCNXMODE_INACT || cnxParam->mode == EPCNXMODE_RCVO) &&
        cnxParam->cnxParmList.send.numCodecs == 0 )
   {
      return ( EPSTATUS_SUCCESS );
   }

   selectedCodec = cnxParam->cnxParmList.send.codecs[0].type;

   switch ( selectedCodec )
   {
      case ( CODEC_T38_MUTE ):
      {
         EPSTATUS    rc;

         /* Configure the T.38 packet size.  This must be done prior to
         ** the enabling of T.38 service */
         rc = SetT38PktSize( cnx, cnxParam->cnxParmList.send.period[0] );
         if ( rc != EPSTATUS_SUCCESS )
         {
            return ( rc );
         }

         /* Configure the T.38 Error Correction.  This must be done prior to
         ** the enabling of T.38 service */
         rc = SetT38EcMode( cnx, cnxParam->t38param.ecMode );
         if ( rc != EPSTATUS_SUCCESS )
         {
            return ( rc );
         }

         /* Set bit to mute audio path until an RCD w/ T.38 is received */
         bMuteT38 = VRG_TRUE;

         /* Force to go into faxr immediately.  We will discard packets
         ** on the host to simulate muting audio.  */
         SetVoiceMode( cnx, VM_FAX_RELAY );
      }
      break;

      case ( CODEC_T38 ):
      {
         EPSTATUS    rc;

         /* Configure the T.38 packet size.  This must be done prior to
         ** the enabling of T.38 service */
         rc = SetT38PktSize( cnx, cnxParam->cnxParmList.send.period[0] );
         if ( rc != EPSTATUS_SUCCESS )
         {
            return ( rc );
         }

         /* Configure the T.38 Error Correction.  This must be done prior to
         ** the enabling of T.38 service */
         rc = SetT38EcMode( cnx, cnxParam->t38param.ecMode );
         if ( rc != EPSTATUS_SUCCESS )
         {
            return ( rc );
         }

         /* Force to go into faxr immediately  */
         SetVoiceMode( cnx, VM_FAX_RELAY );
      }
      break;

      /* This is used when switching to VBD V.152 call. */
      case ( CODEC_RED ):
      {
         /* Retrieve correct codec for this connection. */
         selectedCodec = cnxParam->cnxParmList.send.codecs[0].options.rfc2198Type;
         
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT,  "ENDPT %d: Selected CODEC_RED as its first codec, redundant codec is %d", cnx->lineId, selectedCodec ));
         /* enable rfc2198 packing when CODEC_RED is set as the first codec */
         cnx->rtpRegs.configFlags |= HAPI_RTP_INGRESS_RFC2198_ENABLE_MASK;
         cnx->rtpRegs.configFlags &= ~HAPI_RTP_TONE_TRIPLICATE_REDUNDANCY_MASK;
      }

      default:
      {
      }
      break;
   }

   /*
   ** Determine the number of superpacket interval in units of samples
   ** sampled at 8kHz:
   */
#if VRG_ENDPT_PACKET_TIME_CHECK
   superPktInterval = checkPacketTime( cnxParam ) * 8;
#else
   superPktInterval = cnxParam->cnxParmList.send.period[0] * 8;
#endif // VRG_ENDPT_PACKET_TIME_CHECK

   if ( EnableRelay( cnx, bEnableDigitRelay, superPktInterval, cnxParam->dataMode, cnxParam->digitRelayType  ) != EPSTATUS_SUCCESS )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT %d: Error returned by enableRelay(), digitRelay: %d, dataMode: %d",
                  cnx->lineId, bEnableDigitRelay, cnxParam->dataMode ));

      return ( EPSTATUS_RESOURCES );
   }

   /* If we are simply switching to T.38, no additional cnx configuration is required */
   if ( selectedCodec == CODEC_T38 || selectedCodec == CODEC_T38_MUTE )
   {
      cnx->bMuteT38 = bMuteT38;
      return ( EPSTATUS_SUCCESS );
   }



   /*
   ** Process list of negotiated codecs between the local and remote endpoint.
   ** Note that the list is ordered by preference. The first codec in the list
   ** should be selected as the encoder.
   */

   /*
   ** G.729A and G.729 are completely interoperable; however, RTP only recognizes
   ** G.729. Therefore, we reported G.729 in our capabilities if either codec
   ** is supported to the outside world. However, we need to set the DSP vocoder
   ** based upon the actual capabilities of the DSP. i.e. we need to switch the DSP
   ** encoder to G.729A if it supports it, and G.729 was requested by the client app.
   */
   if (( selectedCodec == CODEC_G729 ) && ( vrgEndptCap[dspNum].codecCap[CODEC_G729A] == CODEC_SUPPORTED ) &&
       ( vrgEndptCap[dspNum].codecCap[CODEC_G729B] == CODEC_SUPPORTED ))
   {
      selectedCodec = CODEC_G729B;
   }
   else if (( selectedCodec == CODEC_G729 ) && ( vrgEndptCap[dspNum].codecCap[CODEC_G729A] == CODEC_SUPPORTED ))
   {
      selectedCodec = CODEC_G729A;
   }

   rateMap = epRateMap;
   while (( rateMap->codec != CODEC_NULL ) && ( rateMap->codec != selectedCodec ))
   {
      rateMap++;
   }

   if ( rateMap->codec == CODEC_NULL )
   {
      return ( EPSTATUS_UNSUPPORTED_MODE );
   }

   if ( ( baseFrameRate = codecGetBaseFrameRate( selectedCodec ) ) == 0 )
   {
      return ( EPSTATUS_UNSUPPORTED_MODE );
   }


   /* Lite connections should accept only G.711/NTE codecs */
   if ( cnx->bLiteWeight )
   {
      if (( selectedCodec != CODEC_PCMU ) &&
          ( selectedCodec != CODEC_PCMA ) &&
          ( selectedCodec != CODEC_NTE ))
      {
         return ( EPSTATUS_UNSUPPORTED_MODE );
      }
   }

   /*
   ** If we are in VoiceBandData mode, we need to disable silence suppression
   */
#ifdef VRG_ENDPT_CFG_G168_TEST
   /*
   ** VAD is not required.  Turn off the VAD detector
   */
   vadMode = HAPI_GVAD_MODE_OFF;
   vadType = HAPI_VAD_TYPE_NONE;
#else
   if ( cnx->vbData.bEnabled || digitalMode )
   {
      vadMode = HAPI_GVAD_MODE_OFF;
      vadType = HAPI_VAD_TYPE_NONE;
   }
   else
   {
      /*
      ** Check if we need to enable VAD
      */
      if ( cnxParam->silence )
      {
         /*
         ** VAD is required.  Enable the VAD detector.
         */
         vadMode = HAPI_GVAD_MODE_TRANSPARENT;

         /*
         ** Determine the VAD type
         **
         ** Currently, we have no way to determine whether G723.1A and G729B
         ** use the built-in VAD or generic VAD.  Thus, we will assume that
         ** we will always use built-in VAD for these two codecs.
         */

         if ( (rateMap->codec == CODEC_G729B) ||
            ( rateMap->codec == CODEC_G7231A_53 ) || ( rateMap->codec == CODEC_G7231A_63 ) ||
            ( rateMap->codec >= CODEC_GSMAMR_12K && rateMap->codec <= CODEC_GSMAMR_OA_475) ||
              rateMap->codec == CODEC_GSMEFR ||
              rateMap->codec == CODEC_GSMFR ||
              rateMap->codec == CODEC_GSMHR ||
            ( rateMap->codec >= CODEC_AMRWB_66 && rateMap->codec <= CODEC_AMRWB_OA_2385))
         {
            vadType = HAPI_VAD_TYPE_BUILTIN;
         }
         else
         {
            vadType = HAPI_VAD_TYPE_GENERIC;
         }
      }
      else
      {
         /*
         ** VAD is not required.  Turn off the VAD detector
         */
         vadMode = HAPI_GVAD_MODE_OFF;
         vadType = HAPI_VAD_TYPE_NONE;
      }
   }
#endif
   /*
   ** Retrieve PLC settings
   */
   plcType = vrgEndptRetrievePlcSettings( selectedCodec );

   /*
   ** Configure the VAD, CNG, and PLC settings.  Note that we only
   ** set the VAD detector mode here.  The VAD type (used when VAD
   ** is enabled) will be configured later.
   */
   cng = ( digitalMode ? HAPI_GCNG_NOISEOFF : cnxParam->comfortNoise );

   /* Modify the VAD/CNG/PLC settings only if something changed */
   if ((cnx->VadMode != vadMode) ||
       (cnx->CngMode != cng) ||
       (cnx->PlcMode != plcType))
   {
      hdspVhdSetVadCngPlc( cnx->vhdhdl, vadMode, cng, plcType );

      /* Save the new settings for future reference */
      cnx->VadMode = vadMode;
      cnx->CngMode = cng;
      cnx->PlcMode = plcType;
   }


   /*
   ** Set the voice encoder, VAD type and packetization period
   */

   /*
   ** Check if a "preferred" variable G723x codec type is specified
   ** in the FIRST codec of the list.  If it is, we need to save
   ** this "preferred" value so that the DSP can be informed on the
   ** starting codec rate
   */
   switch ( cnxParam->cnxParmList.send.codecs[0].type )
   {
      case ( CODEC_G7231_53_VAR ):
      {
         xChgCodec = HAPI_PKT_VC_G7231_53;
      }
      break;

      case ( CODEC_G7231_63_VAR ):
      {
         xChgCodec = HAPI_PKT_VC_G7231_63;
      }
      break;

      case ( CODEC_G7231_VAR ):
      {
         /* If no explicit rates were specified, we will assume that
         ** 6.3 is the "preferred" starting rate */
         xChgCodec = HAPI_PKT_VC_G7231_63;
      }
      break;

      default:
      {
         xChgCodec = rateMap->rate;
      }
      break;
   }


   /* Check if there are ANY "preferred" variable G7231 codecs in the
   ** codec lists.  If there are, we need to replace these "preferred" variable
   ** codecs with the generic variable G7231 codec since there is no notion of
   ** "preferred" G7231 variable codecs in the RTP shim. */
   checkG7231var( &cnxParam->cnxParmList.recv );
   checkG7231var( &cnxParam->cnxParmList.send );

   /* Modify the voice codec settings only if something changed */
   if (( cnx->silence   != cnxParam->silence )   ||
       ( cnx->period    != cnxParam->cnxParmList.send.period[0] )    ||
       ( cnx->codecType != cnxParam->cnxParmList.send.codecs[0].type ))
   {
      VRG_UINT16 vbdCodec = HAPI_PKT_VC_G711_ALAW;
#ifndef VRG_ENDPT_CFG_G168_TEST
      /* VBD codec is set to G.711ALaw instead due to convenience in analyzing data.
      ** All Tests are conducted using G.711ALaw so using G.711ALaw as VBD codec will not
      ** require separate decoding when importing to an audio editor software */
      int preferredVBDCodec;
#endif
      /* Inform the DSP of the selected vocoder */
      cnx->netRegs.hsxPacketVoiceEncoder = (VRG_UINT16)( ( vadType << 8 ) + xChgCodec );
      cnx->netRegs.hsxVoiceSuperPktInterval = superPktInterval;
      cnx->netRegs.hsxVBDSuperPktInterval = superPktInterval;

      if (bInitialChange)
      {
         if ( hdspSendCmdDataAsync( cnx->vhdhdl,
                               HAPINET_CONFIG_SETREGS,
                               HSCMDEXTDATA_ASYNC,
                               sizeof(HSZNETREGS),
                               & cnx->netRegs ))
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "Failed to set NET config registers for vhd 0x%x !!!", cnx->vhdhdl ));
            return ( HAPIFAILURE );
         }
      }
      else
      {
         hdspVhdSetVoiceCodec( cnx->vhdhdl,
                               xChgCodec,
                               vadType,
                               superPktInterval, bInitialChange );
      }

#ifdef VRG_ENDPT_CFG_G168_TEST
      vbdCodec = HAPI_PKT_VC_G711_ALAW;
#else
      vbdCodec = HAPI_PKT_VC_G711_ULAW;

      /* Set preferred VBD codec, such that the preferred codec is used upon changing into VBD with auto-switch option */
      if ( Provision( state->endptArchive, EPPROV_PreferredVBDCodec, &preferredVBDCodec ) == EPSTATUS_SUCCESS )
      {
         switch ( preferredVBDCodec )
         {
            case EPVBDCODEC_PCMU:
            {
               vbdCodec = HAPI_PKT_VC_G711_ULAW;
            }
            break;
            case EPVBDCODEC_PCMA:
            {
               vbdCodec = HAPI_PKT_VC_G711_ALAW;
            }
            break;
            case EPVBDCODEC_NONE:
            default:
            {
               if( (xChgCodec == HAPI_PKT_VC_G711_ULAW) || (xChgCodec == HAPI_PKT_VC_G711_ALAW) )
               {
                  vbdCodec = xChgCodec;
               }
            }
            break;
         }
      }
#endif

      /* Set VBD codec */
      cnx->netRegs.hsxVoiceBandDefault = (VRG_UINT16)( ( vadType << 8 ) + vbdCodec );
      hdspVhdSetVBDCodec( cnx->vhdhdl, vbdCodec, vadType );
   }

   cnx->bMuteT38 = bMuteT38;

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "Set codec: "
             "codec=0x%x, pc=0x%x, s=%d, p=%d",
             (unsigned int)selectedCodec, xChgCodec, cnxParam->silence, cnxParam->cnxParmList.send.period[0] ));

   return ( EPSTATUS_SUCCESS );
}

EPSTATUS SetRingParams( VRG_ENDPT *state )
{
   XDRV_APM    *apmDriver;
   XDRV_SLIC   *slicDriver;
   EPRINGPARAM *ringParam = &(state->ringParam);
   CAS_CTL_EXT_CMD_PARM parms;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();
   
   slicDriver = boardHalSlicGetDriver( state->endptNum );
   if( !slicDriver )
   {
      return ( EPSTATUS_RESOURCES );
   }
   apmDriver = boardHalApmGetDriver( state->dspNum );

   ProvisionRingParams(state, ringParam);

   parms.drvHdl = (void*)apmDriver;
   parms.arguments[0] = ringParam->frequency;
   parms.arguments[1] = ringParam->waveshape;
   parms.arguments[2] = ringParam->voltage;
   parms.arguments[3] = ringParam->offset;
   parms.arguments[4] = ringParam->offsetCal;

   /* Set the provisioned ring parameters */   
   if( voiceParams->flags & BP_FLAG_DSP_APMHAL_ENABLE )
   { 
      casCtlSendExtendedCommand( CAS_REQ_APM_SET_RING_PARMS, CAS_CMD_MODE_ASYNC, &parms );
   }

   parms.drvHdl = (void*)slicDriver;
   casCtlSendExtendedCommand( CAS_REQ_SLIC_SET_RING_PARMS, CAS_CMD_MODE_ASYNC, &parms );

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   SetRtcpInterval
**
** PURPOSE:    set up RTCP packet interval.
**
** PARAMETERS: cnx      - pointer to cnx block
**             intMs    - desired rtcp packet interval in ms
**
** RETURNS:
**
*****************************************************************************
*/
void SetRtcpInterval( CNXSTATE *cnx, VRG_UINT16 intMs )
{
   HAPI_RTP_REGS rtpRegs = cnx->rtpRegs;

   /* Set the rtcp interval */
   rtpRegs.rtcpPacketInterval = intMs;

   /* Save Rtp registers */
   cnx->rtpRegs = rtpRegs;
}

/*
*****************************************************************************
** FUNCTION:   SetRtpCSCallEnable
**
** PURPOSE:    Allow RTP service to handle CS voice packets
**
** PARAMETERS: cnx      - pointer to cnx block
**
** RETURNS:
**
*****************************************************************************
*/
void SetRtpCSCallEnable ( CNXSTATE *cnx )
{
     VRG_SINT16    result;
     result = hdspSendCmdAsync( cnx->vhdhdl, HAPI_RTP_SET_CSVOICE_CMD, 
                                HSCMDDATA, 1, 0 );


     if ( result != HAPISUCCESS ) {
          VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting (enable) CSVOICE "));
     }
}

/*
*****************************************************************************
** FUNCTION:   SetRtpCSCallDisable
**
** PURPOSE:    Allow RTP service to handle CS voice packets
**
** PARAMETERS: cnx      - pointer to cnx block
**
** RETURNS:
**
*****************************************************************************
*/
void SetRtpCSCallDisable ( CNXSTATE *cnx )
{
     VRG_SINT16    result;

     result = hdspSendCmdAsync( cnx->vhdhdl, HAPI_RTP_SET_CSVOICE_CMD, 
                                HSCMDDATA, 0, 0 );

     if ( result != HAPISUCCESS ) {
          VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting (disable) CSVOICE "));
     }
}

/*
*****************************************************************************
** FUNCTION:   SetRtpMode
**
** PURPOSE:    set up RTP service registers for sending / receiving RTP packets.
**
** PARAMETERS: cnx      - pointer to cnx block
**             mode     - endpoint cnx mode
**
** RETURNS:
**
*****************************************************************************
*/
void SetRtpMode( CNXSTATE *cnx, EPCNXMODE mode )
{
   HAPI_RTP_REGS2 rtpRegs2 = cnx->rtpRegs2;

   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Setting RTP mode for vhd 0x%x to %d", cnx->vhdhdl, mode ));

   /*
   ** Check if we have some active connection signals ongoing on this
   ** endpoint, if so and if the new mode is not network loopback,
   ** we need to enable the sending of RTP packets.
   */
   if ( ModeCheck( mode, SENDMODE) || ( cnx->activeSignal && (mode != EPCNXMODE_LOOP) ) )
   {
      rtpRegs2.configFlags2 &= ~HAPI_RTP_INGRESS_SEND_RTP_PACKETS_DISABLE_MASK;
   }
   else
   {
      rtpRegs2.configFlags2 |= HAPI_RTP_INGRESS_SEND_RTP_PACKETS_DISABLE_MASK;
   }
   if ( ModeCheck( mode, RECVMODE) )
   {
      rtpRegs2.configFlags2 &= ~HAPI_RTP_EGRESS_PROCESS_RTP_PACKETS_DISABLE_MASK;
   }
   else
   {
      rtpRegs2.configFlags2 |= HAPI_RTP_EGRESS_PROCESS_RTP_PACKETS_DISABLE_MASK;
   }

   if ( rtpRegs2.configFlags2 != cnx->rtpRegs2.configFlags2 )
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Setting RTP mode for vhd 0x%x to %d", cnx->vhdhdl, mode ));
      if ( cnx->activeSignal && (mode != EPCNXMODE_LOOP) )
      {
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Overriding RTP mode to send signal on vhd 0x%x", cnx->vhdhdl ));
      }

      /* Save Rtp registers */
      cnx->rtpRegs2 = rtpRegs2;
   }

}

/*****************************************************************************
*  FUNCTION:   SetRtpPayloadTypes
*
*  PURPOSE:    To inform the RTP shim of the RTP payload types for local
*              and remote codecs contained in the codec list
*
*  PARAMETERS: cnx         - connection state instance memory
*              codecList   - list of codecs for which to set RTP payload types
*
*
*  RETURNS:    none
*
*  NOTE:
*****************************************************************************/
void SetRtpPayloadTypes( CNXSTATE *cnx, CODECCNXPARMLIST *codecList )
{
   const EPZRATEMAP  *rateMap;
   const EPZRATEMAP  *rateRedMap;
   int i;
   HAPI_RTP_REGS rtpRegs = cnx->rtpRegs;

   int codecMapSize;
   int codecPcmuSet;

   /* Default static payload map: packets of these types will be decoded even if not negotiated
   ** when the feature is enabled.
   **
   ** Note that we always default the first table entry to PCMU to avoid colliding with the algorithm
   ** which setups the full table in MapRtpPayloadType function.  But that entry would be overwritten
   ** in the end if not valid.
   */
   HAPI_UINT16 mediaPayloadTypeMap[HAPI_RTP_MAX_N_CODECS] = {
#ifdef VRG_ENDPT_2G3G_SUPPORT
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(109,              HAPI_PKT_VC_GSM_HR_56), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(110,              HAPI_PKT_VC_GSM_FR_13), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(111,              HAPI_PKT_VC_GSM_EFR_122), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(112,              HAPI_PKT_VC_GSM_AMR_OA_122), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(113,              HAPI_PKT_VC_WB_AMR_OA_2385)
#else
#if VRG_ENDPT_CFG_PRELOAD_RTP_PAYLOAD_MAP
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G711U,               HAPI_PKT_VC_G711_ULAW), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G711A,               HAPI_PKT_VC_G711_ALAW), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G728,                HAPI_PKT_VC_G728_16), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G729,                HAPI_PKT_VC_G729_8), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G723,                HAPI_PKT_VC_G7231_VARIABLE),  \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_GSM,                 HAPI_PKT_VC_GSM_AMR_VARIABLE),  \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_G722),              HAPI_PKT_VC_G722_64_MODE_1)
#else
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G711U,               HAPI_PKT_VC_G711_ULAW)
#endif /* VRG_ENDPT_CFG_PRELOAD_RTP_PAYLOAD_MAP */
#endif /* VRG_ENDPT_2G3G_SUPPORT */
   };
   HAPI_UINT16 mediaEgressPayloadTypeMap[HAPI_RTP_MAX_N_CODECS] = {
#ifdef VRG_ENDPT_2G3G_SUPPORT
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(109,              HAPI_PKT_VC_GSM_HR_56), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(110,              HAPI_PKT_VC_GSM_FR_13), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(111,              HAPI_PKT_VC_GSM_EFR_122), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(112,              HAPI_PKT_VC_GSM_AMR_OA_122), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(113,              HAPI_PKT_VC_WB_AMR_OA_2385)
#else
#if VRG_ENDPT_CFG_PRELOAD_RTP_PAYLOAD_MAP
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G711U,               HAPI_PKT_VC_G711_ULAW), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G711A,               HAPI_PKT_VC_G711_ALAW), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G728,                HAPI_PKT_VC_G728_16), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G729,                HAPI_PKT_VC_G729_8), \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G723,                HAPI_PKT_VC_G7231_VARIABLE),  \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_GSM,                 HAPI_PKT_VC_GSM_AMR_VARIABLE),  \
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY((HAPI_RTP_IETF_PT_CODEC_G722),              HAPI_PKT_VC_G722_64_MODE_1)
#else
   HAPI_RTP_MAKE_MEDIA_PAYLOAD_TYPE_MAP_ENTRY(HAPI_RTP_IETF_PT_CODEC_G711U,               HAPI_PKT_VC_G711_ULAW)
#endif /* VRG_ENDPT_CFG_PRELOAD_RTP_PAYLOAD_MAP */
#endif /* VRG_ENDPT_2G3G_SUPPORT */
   };


#ifdef VRG_ENDPT_2G3G_SUPPORT
   /* Quick setup and return if this is for 2G/3G call */
   VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "Setting payload map for 2G/3g calls\n"));
   memcpy( rtpRegs.mediaPayloadTypeMap,
           mediaPayloadTypeMap,
           sizeof(mediaPayloadTypeMap) );
   memcpy( rtpRegs.mediaEgressPayloadTypeMap,
           mediaEgressPayloadTypeMap,
           sizeof(mediaEgressPayloadTypeMap));
   cnx->rtpRegs = rtpRegs;
   return;
#endif


   (void) codecPcmuSet;
   /*
   ** Set RTP payload types in RTP service config registers for each negotiated local codec
   */
   codecMapSize = 1; /* Always default PCMU set. */
   codecPcmuSet = 0;
   for ( i = 0; i < codecList->send.numCodecs; i++ )
   {
      rateMap  = epRateMap;
      while (( rateMap->codec != CODEC_NULL ) &&
            (( rateMap->codec != codecList->send.codecs[i].type )))
      {
         rateMap++;
      }

      if( rateMap->codec != CODEC_NULL )
      {
         if( rateMap->codec == CODEC_NTE )
         {
            rtpRegs.ntePayloadType = codecList->send.codecs[i].rtpPayloadType;
            cnx->rtpstate.sendNtePayloadType = codecList->send.codecs[i].rtpPayloadType;
         }
         else if( rateMap->codec == CODEC_RED )
         {
            /* Set RED payload type. */
            rtpRegs.rfc2198PayloadType       = codecList->send.codecs[i].rtpPayloadType;
            cnx->rtpstate.sendRedPayloadType = codecList->send.codecs[i].rtpPayloadType;

            /* Set RED level. */
            rtpRegs.rfc2198RedundancyLevels  = codecList->send.codecs[i].options.opt1;
            cnx->rtpstate.redLevel           = codecList->send.codecs[i].options.opt1;

            /* Set the corresponding VBD codec to be associated with this redundant codec. */
            rateRedMap = epRateMap;
            while (( rateRedMap->codec != CODEC_NULL ) &&
                  (( rateRedMap->codec != codecList->send.codecs[i].options.rfc2198Type )))
            {
               rateRedMap++;
            }
            if( rateRedMap->codec != CODEC_NULL )
            {
               VRG_UINT16 vbdCodec;
#ifdef VRG_ENDPT_CFG_G168_TEST
               vbdCodec = HAPI_PKT_VC_G711_ALAW;
#else
               VRG_ENDPT* state;
               int preferredVBDCodec;

               vbdCodec = HAPI_PKT_VC_G711_ULAW;

               state = GetEndptState( cnx->lineId );
               /* add vbd preferred codec type */
               if ( Provision( state->endptArchive, EPPROV_PreferredVBDCodec, &preferredVBDCodec ) == EPSTATUS_SUCCESS ) 
               {
                  switch (preferredVBDCodec)
                  {
                     case EPVBDCODEC_PCMU:
                        vbdCodec = HAPI_PKT_VC_G711_ULAW;
                        break;
                     case EPVBDCODEC_PCMA:
                        vbdCodec = HAPI_PKT_VC_G711_ALAW;
                        break;
                     case EPVBDCODEC_NONE:
                     default:
                        if( (rateRedMap->rate == HAPI_PKT_VC_G711_ULAW) || (rateRedMap->rate == HAPI_PKT_VC_G711_ALAW) )
                        {
                           vbdCodec = rateRedMap->rate;
                        }
                        break;
                  }
               }
#endif

               hdspSendCmd( cnx->vhdhdl, 
                            HAPI_RTP_VBD_PAYLOADMAP_SET_CMD,
                            HSCMDDATA,
                            (vbdCodec == HAPI_PKT_VC_G711_ULAW) ? HAPI_RTP_IETF_PT_CODEC_G711U : HAPI_RTP_IETF_PT_CODEC_G711A,
                            vbdCodec );
            }
            else
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "vhdhdl 0x%x - cannot map redundant codec type %d",
                         cnx->vhdhdl, codecList->send.codecs[i].options.rfc2198Type ));
            }

            VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "vhdhdl 0x%x - set RED LVL %d and PT %d for codec %d (PT %d)",
                      cnx->vhdhdl,
                      codecList->send.codecs[i].options.opt1,
                      codecList->send.codecs[i].rtpPayloadType,
                      codecList->send.codecs[i].options.rfc2198Type,
                      codecList->send.codecs[i].options.opt2 ));
         }
         else
         {
            /* Filter out VBD (V.152) codec payload map, since already set via redundancy
            ** codec setting.
            */
            if( !codecList->send.codecs[i].options.opt1 )
            {
               if( rateMap->codec == CODEC_PCMU )
               {
                  codecPcmuSet = 1;
               }
               codecMapSize += MapRtpPayloadType( rateMap->rate,
                                               codecList->send.codecs[i].rtpPayloadType,
                                               mediaPayloadTypeMap );
            }
         }
      }
   }

#if !VRG_ENDPT_CFG_PRELOAD_RTP_PAYLOAD_MAP
   /* If we do not preload the RTP/codec mapping table with default value, we need to
   ** override the non-set entries with the last well known entry from the table to avoid
   ** default mapping of unwanted codec (for example PCMU with a payload type of 0 could
   ** slip into this list due to initialization).
   */
   for( i = codecMapSize ; i < HAPI_RTP_MAX_N_CODECS ; i++ )
   {
      mediaPayloadTypeMap[i] = mediaPayloadTypeMap[codecMapSize - 1];
   }

   /* If codec PCMU was not set in this case, we need to override the first entry
   ** in the table as well.  The reason why the first entry is kept to PCMU by default
   ** is because the algorithm used to fill in the table.
   */
   if( !codecPcmuSet )
   {
      mediaPayloadTypeMap[0] = mediaPayloadTypeMap[codecMapSize - 1];
   }
#endif


   /* Set safe default payload types for NTE and RED. These values will be overwritten below
   ** if NTE and/or RED payload types are present in the remote SDP (and if we support the codecs).
   */
   rtpRegs.nteEgressPayloadType = cnx->rtpstate.recvNtePayloadType = NTE_DEFAULT_PAYLOAD;
   rtpRegs.rfc2198EgressPayloadType = cnx->rtpstate.recvRedPayloadType = RED_DEFAULT_PAYLOAD;

   /*
   ** Set RTP payload types in RTP service config registers for each negotiated remote codec
    */
   codecMapSize = 1; /* Always default PCMU set. */
   codecPcmuSet = 0;
   for ( i = 0; i < codecList->recv.numCodecs; i++ )
   {
      rateMap  = epRateMap;
      while (( rateMap->codec != CODEC_NULL ) &&
            (( rateMap->codec != codecList->recv.codecs[i].type )))
      {
         rateMap++;
      }

      if( rateMap->codec != CODEC_NULL )
      {
         if( rateMap->codec == CODEC_NTE )
         {
            rtpRegs.nteEgressPayloadType = codecList->recv.codecs[i].rtpPayloadType;
            cnx->rtpstate.recvNtePayloadType = codecList->recv.codecs[i].rtpPayloadType;
         }
         else if( rateMap->codec == CODEC_RED )
         {
            /* Set RED payload type. */
            rtpRegs.rfc2198EgressPayloadType = codecList->recv.codecs[i].rtpPayloadType;
            cnx->rtpstate.recvRedPayloadType = codecList->recv.codecs[i].rtpPayloadType;
         }
         else
         {
            /* Filter out VBD (V.152) codec payload map, since already set via redundancy
            ** codec setting.
            */
            if( !codecList->recv.codecs[i].options.opt1 )
            {
               if( rateMap->codec == CODEC_PCMU )
               {
                  codecPcmuSet = 1;
               }
               codecMapSize += MapRtpPayloadType( rateMap->rate,
                                                  codecList->recv.codecs[i].rtpPayloadType,
                                                  mediaEgressPayloadTypeMap );
            }
         }
      }
   }

#if !VRG_ENDPT_CFG_PRELOAD_RTP_PAYLOAD_MAP
   /* If we do not preload the RTP/codec mapping table with default value, we need to
   ** override the non-set entries with the last well known entry from the table to avoid
   ** default mapping of unwanted codec (for example PCMU with a payload type of 0 could
   ** slip into this list due to initialization).
   */
   for( i = codecMapSize ; i < HAPI_RTP_MAX_N_CODECS ; i++ )
   {
      mediaEgressPayloadTypeMap[i] = mediaEgressPayloadTypeMap[codecMapSize - 1];
   }

   /* If codec PCMU was not set in this case, we need to override the first entry
   ** in the table as well.  The reason why the first entry is kept to PCMU by default
   ** is because the algorithm used to fill in the table.
   */
   if( !codecPcmuSet )
   {
      mediaEgressPayloadTypeMap[0] = mediaEgressPayloadTypeMap[codecMapSize - 1];
   }
#endif

   memcpy( rtpRegs.mediaPayloadTypeMap,
           mediaPayloadTypeMap,
           sizeof(mediaPayloadTypeMap) );
   memcpy( rtpRegs.mediaEgressPayloadTypeMap,
           mediaEgressPayloadTypeMap,
           sizeof(mediaEgressPayloadTypeMap));

   cnx->rtpRegs = rtpRegs;

#if VRG_ENDPT_PRINT_RTP_PAYLOADMAPS
   for ( i = 0 ; i < codecMapSize ; i++ )
   {
      VRG_LOG_DBG(( VRG_LOG_MOD_EPT, "inPtMap[PType %03d Codec %02d] egPtMap[PType %03d Codec %02d]", (mediaPayloadTypeMap[i]>>8), (mediaPayloadTypeMap[i] & 0x00FF), (mediaEgressPayloadTypeMap[i]>>8), (mediaEgressPayloadTypeMap[i] & 0x00FF) ));
   }
#endif /* VRG_ENDPT_PRINT_RTP_PAYLOADMAPS */
}


/*
*****************************************************************************
** FUNCTION:   SetRtpServiceRegisters
**
** PURPOSE:    - set up RTP XR service registers to current cnx state
**             - set up V.152 RTP service registers to current cnx state
**
** PARAMETERS: cnx      - pointer to cnx block
**             XRmode   - XR mode
**             vbdParam - pointer to VBD parameter
**
** RETURNS:
**
*****************************************************************************
*/
void SetRtpServiceRegisters( CNXSTATE *cnx, EPRTCPXRMODE XRmode, EPZVBDPARAM *vbdParam, VRG_UINT16 secHdrSize)
{
   HAPI_RTP_REGS rtpRegs = cnx->rtpRegs;
   HAPI_RTP_REGS2 rtpRegs2 = cnx->rtpRegs2;

   /* XR register config */
   switch (XRmode)
   {
      case EPRTCPXRMODE_ON:
      {
         rtpRegs.configFlags |= HAPI_RTP_INGRESS_SEND_RTCP_XR_PACKETS_ENABLE_MASK;
         rtpRegs.rtcpXrConfigFlags |= HAPI_RTP_RTCP_XR_GENERATE_VOIP_REPORT;
      }
      break;
      case EPRTCPXRMODE_NEGOTIATE:
      {
         rtpRegs.configFlags |= HAPI_RTP_INGRESS_SEND_RTCP_XR_PACKETS_ENABLE_MASK;
         rtpRegs.rtcpXrConfigFlags &= ~HAPI_RTP_RTCP_XR_GENERATE_VOIP_REPORT;
      }
      break;
      case EPRTCPXRMODE_OFF:
      default:
      {
         rtpRegs.configFlags &= ~HAPI_RTP_INGRESS_SEND_RTCP_XR_PACKETS_ENABLE_MASK;
         rtpRegs.rtcpXrConfigFlags &= ~HAPI_RTP_RTCP_XR_GENERATE_VOIP_REPORT;
      }
      break;
   }

   /* Initialize RTCP XR reporting: turning OFF the XR config flags for all other reports */
   rtpRegs.rtcpXrConfigFlags &= ~HAPI_RTP_RTCP_XR_GENERATE_LOSS_RLE_REPORT;
   rtpRegs.rtcpXrConfigFlags &= ~HAPI_RTP_RTCP_XR_GENERATE_DUPLICATE_RLE_REPORT;
   rtpRegs.rtcpXrConfigFlags &= ~HAPI_RTP_RTCP_XR_GENERATE_PACKET_RECEIPT_TIMES_REPORT;
   rtpRegs.rtcpXrConfigFlags &= ~HAPI_RTP_RTCP_XR_GENERATE_RECEIVER_REFERENCE_TIME_REPORT;
   rtpRegs.rtcpXrConfigFlags &= ~HAPI_RTP_RTCP_XR_GENERATE_DLRR_REPORT;
   rtpRegs.rtcpXrConfigFlags &= ~HAPI_RTP_RTCP_XR_GENERATE_STATISTICS_REPORT;


   /* Voice band data mode. */
   if( (vbdParam->vbdMode & EPVBDMODE_V152) == EPVBDMODE_V152 )
   {
      rtpRegs.configFlags |= HAPI_RTP_VBD_V152_ENABLE_MASK;
      /* Voice band data redundancy mode. */
      switch( vbdParam->vbdRedMode )
      {
         case EPVBDRED_ON:
            rtpRegs.configFlags |= HAPI_RTP_VBD_V152_INGRESS_RFC2198_ENABLE_MASK;
         break;

         case EPVBDRED_OFF:
         default:
            rtpRegs.configFlags &= ~HAPI_RTP_VBD_V152_INGRESS_RFC2198_ENABLE_MASK;
         break;
      }
   }
   else
   {
      rtpRegs.configFlags &= ~HAPI_RTP_VBD_V152_ENABLE_MASK;
      rtpRegs.configFlags &= ~HAPI_RTP_VBD_V152_INGRESS_RFC2198_ENABLE_MASK;
   }

   /* Save the changes into a local copy for future use */
   cnx->rtpRegs = rtpRegs;
   cnx->rtpRegs2 = rtpRegs2;
}

/*
*****************************************************************************
** FUNCTION:   GetConnectionRtpStats
**
** PURPOSE:    Retreive RTP Statistics associated with specified Connection
**
** PARAMETERS: cnx        - pointer to connection state
**             epRtpStats - statistics to be populated
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS GetConnectionRtpStats(CNXSTATE *cnx, EPZCNXSTATS *epRtpStats, VRG_BOOL bReset)
{
   VRG_SINT16 status;
   HAPI_RTP_STATS RTPStats;
   HSZNETVOICESTATS pveStats;

   memset( &RTPStats, 0, sizeof(RTPStats) );
   memset( &pveStats, 0, sizeof(pveStats) );
   status = HAPISUCCESS;

   /* Do not access RTP service if voicemode is not active */
   if ( cnx->voiceMode != VM_ACTIVE || cnx->vhdMode == NETMODE_FAXRELAY )
   {
      return( EPSTATUS_BAD_MODE );
   }

   /* Since this is a non-critical command, ignore any errors */
   status = hdspSendCmdData( cnx->vhdhdl,
                         (HAPI_RTP_GETSTATS_EXT_CMD | HDSP_SYNCCMD_ERR_IGN),
                         HSCMDRESP,
                         sizeof(HAPI_RTP_STATS),
                         &RTPStats);

   if ( HAPISUCCESS != status )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR getting RTP stats for vhd %d, status %d ", cnx->vhdhdl, status ));
      return( EPSTATUS_BAD_MODE );
   }

   status = hdspVhdGetVoiceStats( cnx->vhdhdl, &pveStats );
   if( HAPISUCCESS != status )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ERROR getting PVE stats for vhd %d, status %d ", cnx->vhdhdl, status ));
      return( EPSTATUS_BAD_MODE );
   }

   status = hdspVhdGetAvgJBDelay( cnx->vhdhdl, &(epRtpStats->jbAvg));
   if( HAPISUCCESS != status )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ENDPT: ERROR getting JB state for vhd %d, status %d \n", cnx->vhdhdl, status ));
      return( EPSTATUS_BAD_MODE );
   }

   if( bReset == VRG_TRUE )
   {
      status = hdspSendCmd( cnx->vhdhdl,
                            HAPI_RTP_RESET_STATS_CMD,
                            HSCMDDATA,
                            0,
                            0 );
      if( HAPISUCCESS != status )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "ENDPT: ERROR resetting RTP stats for vhd %d, status %d \n", cnx->vhdhdl, status ));
         return( EPSTATUS_BAD_MODE );
      }
   }


   epRtpStats->txpkts    = ((RTPStats.cumulative.ingressRtpPacketsHigh)<<16) + RTPStats.cumulative.ingressRtpPacketsLow;
   epRtpStats->txbytes   = ((RTPStats.cumulative.ingressRtpBytesHigh)<<16) + RTPStats.cumulative.ingressRtpBytesLow;
   epRtpStats->rxpkts    = ((RTPStats.cumulative.egressRtpPacketsHigh)<<16) + RTPStats.cumulative.egressRtpPacketsLow;
   epRtpStats->rxbytes   = ((RTPStats.cumulative.egressRtpBytesHigh)<<16) + RTPStats.cumulative.egressRtpBytesLow;
   epRtpStats->lost      = (((RTPStats.cumulative.egressRtpCumulativeLostPacketsHigh)<<24)>>8)
                              + RTPStats.cumulative.egressRtpCumulativeLostPacketsLow;
   epRtpStats->discarded = ((RTPStats.cumulative.egressRtpDiscardedPacketsHigh)<<16) + RTPStats.cumulative.egressRtpDiscardedPacketsLow;

   epRtpStats->jitter = ((RTPStats.session.egressRtpJitterHigh)<<16) + RTPStats.session.egressRtpJitterLow;
   epRtpStats->jitter = epRtpStats->jitter >> 3;  // convert jitter from 8kHz samples to msec

   epRtpStats->latency = ((RTPStats.session.ingressRtpLatencyHigh)<<16) + RTPStats.session.ingressRtpLatencyLow;
   epRtpStats->latency = (epRtpStats->latency >> 3) / 2;  // convert latency from 8kHz samples to msec, and divide roundtrip time by 2

   epRtpStats->peakLatency = ((RTPStats.session.ingressPeakRtpLatencyHigh)<<16) + RTPStats.session.ingressPeakRtpLatencyLow;
   epRtpStats->peakLatency = (epRtpStats->peakLatency >> 3) / 2;  // convert latency from 8kHz samples to msec, and divide roundtrip time by 2

   epRtpStats->txRtcpPkts = RTPStats.cumulative.ingressRtcpPackets;
   epRtpStats->rxRtcpPkts = RTPStats.cumulative.egressRtcpPackets;

   epRtpStats->txRtcpXrPkts = RTPStats.cumulative.ingressRtcpXrPackets;
   epRtpStats->rxRtcpXrPkts = RTPStats.cumulative.egressRtcpXrPackets;

   epRtpStats->MOSLQ = RTPStats.session.MOSlq;
   epRtpStats->MOSCQ = RTPStats.session.MOScq;

   epRtpStats->jbMin = RTPStats.session.jitterMin >> 3;
   epRtpStats->jbMax = RTPStats.session.jitterMax >> 3;

   epRtpStats->overRuns   = pveStats.hsxPVEstats.decoder.overrunCount;
   epRtpStats->underRuns  = pveStats.hsxPVEstats.decoder.ajcUnderrunCount;
   epRtpStats->peakJitter = pveStats.hsxPVEstats.decoder.peakHoldingTime >> 3; // convert peak jitter from 8kHz samples to msec

#if VRG_ENDPT_PRINT_RTP_STATS
   vrgEndptPrintRtpStats ( epRtpStats );
#endif /* VRG_ENDPT_PRINT_RTP_STATS */

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   SetT38EcMode
**
** PURPOSE:    Configure T.38 Error Correction mode
**
** PARAMETERS: cnx    - pointer to connection state
**             ecMode - error correction mode to be set
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS SetT38EcMode( CNXSTATE *cnx, EPT38ECMODE ecMode )
{
   EPSTATUS    result = EPSTATUS_SUCCESS;
   VRG_UINT16  value = 0x0000;

   switch( ecMode )
   {
      case EPT38EC_UDPREDUNDANCY:
         value = T38_DEFAULT_REDUNDANCY;
         /* Fall to next case */
      case EPT38EC_NONE:
         result = hdspSendCmd( cnx->vhdhdl,
                               HAPINET_T38_SETREG1,
                               HSCMDDATA,
                               OFFSETOF( HAPI_T38_REGS, numRedundantIFP ),
                               value );
      break;

      default:
         /* Do not do anything to be backward compatible */
      break;
   }

   if( result )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting T.38 error correction mode for vhd %d", cnx->vhdhdl ));
   }
   else
   {
      VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "Setting T.38 error correction mode for vhd %d to 0x%x",
                cnx->vhdhdl, value ));
   }

   return ( result );
}

/*
*****************************************************************************
** FUNCTION:   SetT38PktSize
**
** PURPOSE:    Configure T.38 packetization period
**
** PARAMETERS: cnx   - pointer to connection state
**             ptime - packetization time for T.38 packets
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS SetT38PktSize( CNXSTATE *cnx, CODEC_PKT_PERIOD ptime )
{
   EPSTATUS    result;

   /*
   ** Change T.38 ptime
   */
   result = hdspSendCmd( cnx->vhdhdl,
                         HAPINET_FAXR_SETREG1,
                         HSCMDDATA,
                         OFFSETOF_IN_BYTES( HSZFAXRREGS, hsxrxpacketsize ),
                         ptime );

   if( result )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting T.38 registers for vhd %d", cnx->vhdhdl ));
   }

   return ( result );
}

/*
*****************************************************************************
** FUNCTION:   SetVoiceMode
**
** PURPOSE:    To change the current state of the voice mode associated with
**             a connection.
**
** PARAMETERS: cnx   - pointer to connection state
**             mode  - voice mode to set
**
** RETURNS:    None
**
** NOTE:       See declaration of VOICEMODE for description of voicemode
**             state machine.
**
*****************************************************************************
*/
void SetVoiceMode( CNXSTATE *cnx, VOICEMODE mode )
{
   VRG_UINT16 vhdMode = NETMODE_IDLE;
   VRG_UINT16 initMode  = 0;

   XCHG_ASSERT( cnx );

   if ((cnx->voiceMode == mode) && (mode != VM_FAX_RELAY) && (!cnx->vbData.bEnabled))
   {
      /* We are already in the right mode - no need to do anything */
      return;
   }

   /* If the current voice mode is VM_IDLE, or a transition is being made from
    * VM_IDLE, then clear the voiceband data state. This ensures that a transition
    * from VM_IDLE to VM_ACTIVE will always cause the DSP to transition to PACKETVOICE
    * mode. */
   if (( mode == VM_IDLE ) || ( cnx->voiceMode == VM_IDLE ))
   {
      ResetVBDataState( &cnx->vbData );
   }

   switch ( mode )
   {
      case ( VM_IDLE ):
      {
         /* VM_IDLE maps to xChange mode NETMODE_IDLE */
         vhdMode = NETMODE_IDLE;
      }
      break;

      case ( VM_ACTIVE ):
      {
         /* Map VM_ACTIVE depending on the state of the voiceband data flag */
         if( cnx->vbData.bEnabled )
         {
            vhdMode = NETMODE_VOICEBANDDATA;
         }
         else
         {
            vhdMode = NETMODE_PACKETVOICE;
         }
      }
      break;

      case ( VM_FAX_RELAY ):
      {
         const EPFAX_EVT_MAP *pFaxMap = epFaxEvtMap;

         while ( ( pFaxMap->faxLastEvt != EPFAX_EVT_NONE ) &&
                 ( pFaxMap->faxLastEvt != cnx->lastFaxEvent ) )
         {
            pFaxMap++;
         }

         if ( pFaxMap->faxLastEvt != EPFAX_EVT_NONE )
         {
            initMode = pFaxMap->faxInitMode;
         }
         else
         {
            /* Default fax mode is normal */
            initMode = HSFAXRNORMAL;
         }

         vhdMode = NETMODE_FAXRELAY;
      }
      break;

      default:
      {
         XCHG_ASSERT( 0 );
      }
      break;
   }


   /* Change VHD mode */
   hdspVhdSetMode( cnx->vhdhdl, vhdMode, initMode );

   /* Set cached mode */
   cnx->voiceMode = mode;

}

/*
*****************************************************************************
** FUNCTION:   SetupSB
**
** PURPOSE:    Update SwitchBoard according to the SB Masks
**
**             Each bit of the SB mask denotes whether the current cnx contains a DSP
**             SB cnx to/from a set of linside/netside VHDs.  The setting/clearing
**             a bit indicates that a SB cnx does/doesn't exist with the current cnx.
**
**             The inMask and outMask that are passed in are the desired SB configurations.  Using
**             these masks in parallel with inMaskDiff/outMaskDiff, one can determine which cnx
**             needs to be created/deleted.
**
** PARAMETERS: vhdhdl - source vhdhandle
**             inMask - bit Mask indicating the desired input cnxs
**             inMaskDiff - bit Mask indicating changes in the incoming direction
**                          (with respect to the last SB setup)
**             outMask - bit Mask indicating the deisred output cnxs
**             outMaskDiff - bit Mask indicating changes in the outgoing direction
**                          (with respect to the last SB setup)
**
** RETURNS:    Nothing
**
** Note: All bit field takes on the form: Vm-1...V0 Pn-1...P0
**       where V = VHDhandle index, P = Physical endpt index, m = VRG_ENDPT_CFG_NUM_CNX, and n = VRG_ENDPT_CFG_NUM_ENDPT
**
*****************************************************************************
*/
void SetupSB(VRG_UINT16 vhdhdl, int inMask, int inMaskDiff, int outMask, int outMaskDiff)
{
   int bitmask;
   int n;
   int i;
   VRG_UINT16         srcHandle;
   HAPI_SWB_INTERFACE srcInterface;
   VRG_UINT16         dstHandle;
   HAPI_SWB_INTERFACE dstInterface;
   VRG_BOOL           bConnect;

   /* initialization */
   n = VRG_ENDPT_CFG_NUM_CNX + vrgEndptGetNumEndpoints();

   /* iterate through all possible SB cnxs */
   for (i = 0; i < n; i++)
   {
      bitmask = 1 << i;

      /* update the receiving direction  */
      if (bitmask & outMaskDiff)
      {
         /* determine whether to connect or disconnect a SB cnx */
         bConnect = ( bitmask & outMask ) ? VRG_TRUE : VRG_FALSE;

         /* determine the DSP parms */
         if ( i < vrgEndptGetNumEndpoints() )
         {
            dstHandle = vrgEndpt[i].lineVhdHdl;
            dstInterface = HAPI_SWB_TOS;
         }
         else
         {
            dstHandle = vrgCnxState[ i - vrgEndptGetNumEndpoints() ].vhdhdl;
            dstInterface = HAPI_SWB_BOS;
         }

         if ( HAPISUCCESS != hdspRmSwitchBoardConnect( vhdhdl,
                                                       HAPI_SWB_BOS,
                                                       dstHandle,
                                                       dstInterface,
                                                       HAPI_SWB_SIMPLEX,
                                                       bConnect ) )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting switchboard connection for vhdhdl %x", vhdhdl ));
         }
      }

      /* update the sending direction  */
      if (bitmask & inMaskDiff)
      {
         /* determine whether to connect or disconnect a SB cnx */
         bConnect = ( bitmask & inMask ) ? VRG_TRUE : VRG_FALSE;

         /* determine the DSP parms */
         if ( i < vrgEndptGetNumEndpoints() )
         {
            srcHandle = vrgEndpt[i].lineVhdHdl;
            srcInterface = HAPI_SWB_TOS;
         }
         else
         {
            srcHandle = vrgCnxState[ i - vrgEndptGetNumEndpoints() ].vhdhdl;
            srcInterface = HAPI_SWB_BOS;
         }

         if ( HAPISUCCESS != hdspRmSwitchBoardConnect( srcHandle,
                                                       srcInterface,
                                                       vhdhdl,
                                                       HAPI_SWB_BOS,
                                                       HAPI_SWB_SIMPLEX,
                                                       bConnect ) )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR setting switchboard connection for vhdhdl %x", vhdhdl ));
         }
      }
   }
}

/*
*****************************************************************************
** FUNCTION:   UpdateSBMasks
**
** PURPOSE:    Update SwitchBoard masks according to the provided cnx mode
**
** PARAMETERS: cnx      - connection state structure
**             mode     - connection mode
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
void UpdateSBMasks( CNXSTATE *cnx, EPCNXMODE mode)
{
   int   oldinMask;
   int   oldoutMask;
   int   inMaskDiff;
   int   outMaskDiff;
   int   i;
   int   j;
   int   endptNum;
   VRG_ENDPT *endptState;

   endptState = GetEndptState( cnx->lineId );
   if ( endptState == NULL )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "UpdateSBMasks ERROR: unable to get endpt state for lineId %d",
               cnx->lineId ));
      return;
   }

   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "UpdateSBMasks: mode = %d, lineControl = %d", mode, cnx->cnxModeControlsLineVhd));

   if (endptState->testMode == TESTMODE_PACKETLOOPBACK)
   {
      mode = EPCNXMODE_SNDRX;
   }

   /* initializations */
   endptNum = endptState->endptNum;
   oldinMask = cnx->inMask;
   oldoutMask = cnx->outMask;

   switch( mode )
   {
      case EPCNXMODE_SNDO:      /* Send only */
      {
         if (cnx->cnxModeControlsLineVhd)
         {
            cnx->outMask &= ~( 1 << endptNum );
         }
         else
         {
            cnx->outMask = 0;
         }

         cnx->inMask  = ( 1 << endptNum );
         /* iterate through all cnxs in the endpoint */
         for ( i = 0; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
         {
            if (( endptState->cnx[i] )          &&
                ( endptState->cnx[i] != cnx ))
            {
               /* search for the index in the global cnx queue */
               for ( j = 0; j < VRG_ENDPT_CFG_NUM_CNX; j++ )
               {
                  if ( vrgCnxState[j].vhdhdl == endptState->cnx[i]->vhdhdl )
                  {
                     /* enable receiving from other VHDs */
                     cnx->inMask  |= ( 1 << j << vrgEndptGetNumEndpoints() );
                     break;
                  }
               }
            }
         }
      }
      break;

      case EPCNXMODE_RCVO:      /* Receive only */
      {
         if (cnx->cnxModeControlsLineVhd)
         {
            cnx->inMask &= ~( 1 << endptNum );
         }
         else
         {
            cnx->inMask = 0;
         }

         cnx->outMask = ( 1 << endptNum );
         /* iterate through all cnxs in the endpoint */
         for ( i = 0; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
         {
            if (( endptState->cnx[i] )          &&
                ( endptState->cnx[i] != cnx ))
            {
               /* search for the index in the global cnx queue */
               for ( j = 0; j < VRG_ENDPT_CFG_NUM_CNX; j++ )
               {
                  if ( vrgCnxState[j].vhdhdl == endptState->cnx[i]->vhdhdl )
                  {
                     /* enable sending to other VHDs */
                     cnx->outMask |= ( 1 << j << vrgEndptGetNumEndpoints() );
                     break;
                  }
               }
            }
         }
      }
      break;

      case EPCNXMODE_SNDRX:     /* Send-Receive mode */
      {
         if (cnx->cnxModeControlsLineVhd)
         {
            cnx->outMask |= ( 1 << endptNum );
            cnx->inMask  |= ( 1 << endptNum );
         }
         else
         {
            cnx->outMask = ( 1 << endptNum );
            cnx->inMask  = ( 1 << endptNum );
         }
      }
      break;

      case EPCNXMODE_TEST:      /* Network Continuity test */
      {
         /* search for the index for the current cnx in the global cnx queue */
         for (j = 0; j < VRG_ENDPT_CFG_NUM_CNX; j++)
         {
            if ( vrgCnxState[j].vhdhdl == cnx->vhdhdl )
            {
               break;
            }
         }

         /* connect vhd back to itself */
         cnx->inMask = ( 1 << j << vrgEndptGetNumEndpoints() );

         /* disconnect all outgoing cnx from the vhd */
         cnx->outMask = 0;
      }
      break;

      case EPCNXMODE_REPLCATE:  /* Replicate */
      {
         /* create a simple cnx to the vhd handle */
         cnx->inMask = ( 1 << endptNum );

         /* disable all outgoing cnxs */
         cnx->outMask = 0;

         /* iterate through all cnxs associated with the endpoint */
         for (i = 0; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++)
         {
            /* do not reconnect the current vhd */
            if (( endptState->cnx[i] ) && ( endptState->cnx[i] != cnx ))
            {
               /* search for the index in the global cnx queue */
               for (j = 0; j < VRG_ENDPT_CFG_NUM_CNX; j++)
               {
                  if ( vrgCnxState[j].vhdhdl == endptState->cnx[i]->vhdhdl )
                  {
                     /* enable receiving from other vhds */
                     cnx->inMask |= ( 1 << j << vrgEndptGetNumEndpoints() );
                     break;
                  }
               }
            }
         }
      }
      break;

      case EPCNXMODE_CONF:      /* Send-Receive Conference mode */
      {
         /* create a duplex cnx to the lineside handle */
         cnx->inMask  = ( 1 << endptNum );
         cnx->outMask = ( 1 << endptNum );

         /* iterate through all cnxs in the endpoint */
         for ( i = 0; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
         {
            /* only connect to other vhd's when the existing cnx is in conference mode */
            if (( endptState->cnx[i] )          &&
                ( endptState->cnx[i] != cnx )   &&
                ( endptState->cnx[i]->mode == EPCNXMODE_CONF ))
            {

               /* search for the index in the global cnx queue */
               for ( j = 0; j < VRG_ENDPT_CFG_NUM_CNX; j++ )
               {
                  if ( vrgCnxState[j].vhdhdl == endptState->cnx[i]->vhdhdl )
                  {
                     /* connect to the vhds that are associated with the endpoint */
                     cnx->inMask  |= ( 1 << j << vrgEndptGetNumEndpoints() );
                     cnx->outMask |= ( 1 << j << vrgEndptGetNumEndpoints() );
                     break;
                  }
               }

               if (j == VRG_ENDPT_CFG_NUM_CNX)
               {
                  VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR cannot find index in the global cnx queue for vhdhdl %x",
                              endptState->cnx[i]->vhdhdl ));
               }
            }
         }
      }
      break;

      case EPCNXMODE_LOOP:       /* Network loopback */
      case EPCNXMODE_INACT:      /* Inactive mode */
      {
         if (cnx->cnxModeControlsLineVhd)
         {
            /* Remove switchboard connections to line VHD */
            cnx->inMask &= ~( 1 << endptNum );
            cnx->outMask &= ~( 1 << endptNum );
         }
         else
         {
            /* Remove all switchboard connections */
            cnx->inMask  = 0;
            cnx->outMask = 0;
         }
      }
      break;

      case EPCNXMODE_TRANSCODE:      /* Transcoding mode */
      {
         /* iterate through all cnxs in the endpoint */
         for ( i = 0; i < VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT; i++ )
         {
            /* only connect to other vhd's when the existing cnx is in transcode mode */
            if (( endptState->cnx[i] )          &&
               ( endptState->cnx[i] != cnx )   &&
               ( endptState->cnx[i]->mode == EPCNXMODE_TRANSCODE ))
            {
               /* search for the index in the global cnx queue */
               for ( j = 0; j < VRG_ENDPT_CFG_NUM_CNX; j++ )
               {
                  if ( vrgCnxState[j].vhdhdl == endptState->cnx[i]->vhdhdl )
                  {
                    /* connect to the vhds that are associated with the endpoint */
                    cnx->inMask  |= ( 1 << j << vrgEndptGetNumEndpoints() );
                    cnx->outMask |= ( 1 << j << vrgEndptGetNumEndpoints() );
                    break;
                  }
               }

               if (j == VRG_ENDPT_CFG_NUM_CNX)
               {
                  VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "ENDPT: ERROR cannot find index in the global cnx queue for vhdhdl %x",
                             endptState->cnx[i]->vhdhdl ));
               }
            }
         }
      }
      break;

      default:
      {
         /* no actions required */
      }
      break;
   }

   /*
   ** Regardless of hookstate, always connect the ingress switchboard connection
   ** between the PxD and the connection VHD, even if the connection is not
   ** supposed to be in a send mode. This allows us to detect fax and modem
   ** tones in the connection VHD, in all endpt modes.
   **
   ** An exception to the above rule is when the connection is put into network
   ** test mode or during transcoding.  During network test mode, we do not want local events at the
   ** endpt (e.g. DTMF digits) to be forwarded to the cnx.  Thus, we will
   ** leave the switchboard connection between the lineside and the network
   ** un-attached.  A drawback of this is that we will not be able to
   ** detect fax and modem tones in network test mode.
   ** For transcoding we are just converting from one codec to another
   ** using two connection(gateway) VHDs and the line VHD is not involved.
   */
   if ( (mode != EPCNXMODE_TEST) && (mode != EPCNXMODE_TRANSCODE) )
   {
     cnx->inMask  |= ( 1 << endptNum );
   }

   /* Update the mask changes */
   inMaskDiff  = oldinMask  ^ cnx->inMask;
   outMaskDiff = oldoutMask ^ cnx->outMask;

   /* Setup Switchboard cnxs according to the 2 Switchboard bit masks */
   SetupSB( cnx->vhdhdl,
            cnx->inMask,
            inMaskDiff,
            cnx->outMask,
            outMaskDiff);
}



/*****************************************************************************
*  FUNCTION:   checkG7231var
*
*  PURPOSE:    Check for "preferred" G7231 variable rates codec.  If a "preferred"
*              G7231 variable rate codec is present, we need to replace this
*              with the G7231 variable rate codec since the RTP shim does not
*              distinguish between "preferred" rate and supported rate.
*
*  PARAMETERS: codecList  [IN/OUT]  - codec list to check for variable G7231 rates
*
*  RETURNS:    none
*
*  NOTE:       Only 1 G7231 variable rate is allowed per codec list
*
*****************************************************************************/
static void checkG7231var( CODECCNXLIST *codecList )
{
   int   i;

   for ( i = 0; i < codecList->numCodecs; i++ )
   {
      if ( ( codecList->codecs[i].type == CODEC_G7231_53_VAR ) ||
           ( codecList->codecs[i].type == CODEC_G7231_63_VAR ) )
      {
         codecList->codecs[i].type = CODEC_G7231_VAR;
         return;
      }
   }

}

#if VRG_ENDPT_PACKET_TIME_CHECK
/*
*****************************************************************************
** FUNCTION:   checkPacketTime
**
** PURPOSE:    Checks that the packetization time passed as parameter is
**             valid according to provisioned (supported) value.  If not,
**             return the default assumed value for the given codec used.
**
** PARAMETERS: pConParam   (in) Connection parameters.
**
** RETURNS:    Packetization time to be used.
**
** NOTE:
*****************************************************************************
*/
static VRG_UINT16 checkPacketTime( EPZCNXPARAM *pConParm )
{
    VRG_UINT16 retVal = pConParm->cnxParmList.send.period[0];
    VRG_BOOL invalid = VRG_FALSE;

    /* Check against the range if necessary.  Note here the usage of the
       15th bit to carry the 'range' information. */
    if( vrgEndptCap[0].pCap[0] & (1<<15) )
    {
        if( (retVal < (vrgEndptCap[0].pCap[0] & 0x7FFF)) ||
            (retVal > vrgEndptCap[0].pCap[1]))
        {
           invalid = VRG_TRUE;
        }
    }
    /* Check against single value */
    else
    {
       if( retVal != vrgEndptCap[0].pCap[0] )
       {
          invalid = VRG_TRUE;
       }
    }

    /* If the packetization time passed is deemed invalid, replace it with one
       that we do support. */
    if( invalid == VRG_TRUE )
    {
       switch( pConParm->cnxParmList.send.codecs[0].type )
       {
          case CODEC_G7231_53:
          case CODEC_G7231_63:
          case CODEC_G7231A_53:
          case CODEC_G7231A_63:
          case CODEC_G7231_VAR:
          case CODEC_G7231_53_VAR:
          case CODEC_G7231_63_VAR:
           retVal = VRG_ENDPT_PACKET_TIME_G723_DEFAULT;
          break;

          default:
           retVal = VRG_ENDPT_PACKET_TIME_ALL_DEFAULT;
          break;
       }
       /* Log it for feedback */
       VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "checkPacketTime: Unsupported ptime %d ms for codec %d - using %d ms instead",
          pConParm->cnxParmList.send.period[0], pConParm->cnxParmList.send.codecs[0].type, retVal));
    }

    return retVal;
}
#endif // VRG_ENDPT_PACKET_TIME_CHECK

/*
*****************************************************************************
** FUNCTION:   vrgEndptPrintRtpStats
**
** PURPOSE:    Debugging aide.
**
** PARAMETERS: epRtpStats   (in) Statistics reported to API.
**
** RETURNS:    none
**
** NOTE:
*****************************************************************************
*/
void vrgEndptPrintRtpStats ( EPZCNXSTATS *epRtpStats )
{
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP ingressRtpPackets                   %u", (unsigned int)epRtpStats->txpkts));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP ingressRtpBytes                     %u", (unsigned int)epRtpStats->txbytes));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP egressRtpPackets                    %u", (unsigned int)epRtpStats->rxpkts));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP egressRtpBytes                      %u", (unsigned int)epRtpStats->rxbytes));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP egressRtpCumulativeLostPackets      %u", (unsigned int)epRtpStats->lost));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP egressRtpDiscardedPackets           %u", (unsigned int)epRtpStats->discarded));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP ingressRtcpPackets                  %u", (unsigned int)epRtpStats->txRtcpPkts));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP egressRtcpPackets                   %u", (unsigned int)epRtpStats->rxRtcpPkts));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP ingressRtcpXrPackets                %u", epRtpStats->txRtcpXrPkts));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP egressRtcpXrPackets                 %u", epRtpStats->rxRtcpXrPkts));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP jitter (ms)                         %u",  (unsigned int)epRtpStats->jitter));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP latency (ms)                        %u",  (unsigned int)epRtpStats->latency));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "RTP peak latency (ms)                   %u", epRtpStats->peakLatency));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "JB overruns                             %u", epRtpStats->overRuns));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "JB underruns                            %u", epRtpStats->underRuns));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "JB Min (ms)                             %u", epRtpStats->jbMin));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "JB Avg (ms)                             %u", epRtpStats->jbAvg));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "JB Peak(ms)                             %u", epRtpStats->peakJitter));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "JB Max (ms)                             %u", epRtpStats->jbMax));
}

EPSTATUS enableFskClidRx(VRG_BOOL enable, int pstnVhdl)
{
   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "FskClidRx Service is %s",enable?"Enabled":"Disabled"));

   if ( HAPISUCCESS != hdspSendCmd( pstnVhdl,
                                    HAPI_CLIDRX_ENABLE_CMD,
                                    HSCMDDATA,
                                    enable,
                                    0 ) )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ERROR enabling CLIDRX for ENDPT vhd 0x%x", pstnVhdl ));
      return EPSTATUS_BAD_MODE;
   }
   return EPSTATUS_SUCCESS;
}

static VRG_UINT32 net_random(void)
{
    net_rand_seed=net_rand_seed*69069L+1;
        return net_rand_seed^jiffies;
}

/*
*****************************************************************************
** FUNCTION:   printConnectionParams
**
** PURPOSE:    Print all connection parameters
**
** PARAMETERS: cnxParam    (in)  - connection parameters.
**
** RETURNS:
**
** NOTE:
*****************************************************************************
*/
void printConnectionParams( EPZCNXPARAM* cnxParam )
{
   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "m:%d np:%lu e:%d s:%d cn:%d pf:%d dm:%d ae:%d xr:%d dr:%d ssrc:0x%08X",
                                    cnxParam->mode,
                                    cnxParam->namedPhoneEvts,
                                    cnxParam->echocancel,
                                    cnxParam->silence,
                                    cnxParam->comfortNoise,
                                    cnxParam->preserveFaxMode,
                                    cnxParam->dataMode,
                                    cnxParam->autoEncoder,
                                    cnxParam->rtcpXRMode,
                                    cnxParam->digitRelayType,
                                    (unsigned int)cnxParam->localSsrc
               ));

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "send: c: %d %d %d %d %d %d %d %d pt: %d %d %d %d %d %d %d %d num: %d per:%d",
                                    cnxParam->cnxParmList.send.codecs[0].type,
                                    cnxParam->cnxParmList.send.codecs[1].type,
                                    cnxParam->cnxParmList.send.codecs[2].type,
                                    cnxParam->cnxParmList.send.codecs[3].type,
                                    cnxParam->cnxParmList.send.codecs[4].type,
                                    cnxParam->cnxParmList.send.codecs[5].type,
                                    cnxParam->cnxParmList.send.codecs[6].type,
                                    cnxParam->cnxParmList.send.codecs[7].type,
                                    cnxParam->cnxParmList.send.codecs[0].rtpPayloadType,
                                    cnxParam->cnxParmList.send.codecs[1].rtpPayloadType,
                                    cnxParam->cnxParmList.send.codecs[2].rtpPayloadType,
                                    cnxParam->cnxParmList.send.codecs[3].rtpPayloadType,
                                    cnxParam->cnxParmList.send.codecs[4].rtpPayloadType,
                                    cnxParam->cnxParmList.send.codecs[5].rtpPayloadType,
                                    cnxParam->cnxParmList.send.codecs[6].rtpPayloadType,
                                    cnxParam->cnxParmList.send.codecs[7].rtpPayloadType,
                                    cnxParam->cnxParmList.send.numCodecs,
                                    cnxParam->cnxParmList.send.period[0]
                                     ));

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "recv: c: %d %d %d %d %d %d %d %d pt: %d %d %d %d %d %d %d %d num: %d ",
                                    cnxParam->cnxParmList.recv.codecs[0].type,
                                    cnxParam->cnxParmList.recv.codecs[1].type,
                                    cnxParam->cnxParmList.recv.codecs[2].type,
                                    cnxParam->cnxParmList.recv.codecs[3].type,
                                    cnxParam->cnxParmList.recv.codecs[4].type,
                                    cnxParam->cnxParmList.recv.codecs[5].type,
                                    cnxParam->cnxParmList.recv.codecs[6].type,
                                    cnxParam->cnxParmList.recv.codecs[7].type,
                                    cnxParam->cnxParmList.recv.codecs[0].rtpPayloadType,
                                    cnxParam->cnxParmList.recv.codecs[1].rtpPayloadType,
                                    cnxParam->cnxParmList.recv.codecs[2].rtpPayloadType,
                                    cnxParam->cnxParmList.recv.codecs[3].rtpPayloadType,
                                    cnxParam->cnxParmList.recv.codecs[4].rtpPayloadType,
                                    cnxParam->cnxParmList.recv.codecs[5].rtpPayloadType,
                                    cnxParam->cnxParmList.recv.codecs[6].rtpPayloadType,
                                    cnxParam->cnxParmList.recv.codecs[7].rtpPayloadType,
                                    cnxParam->cnxParmList.recv.numCodecs
                                    ));
}

/*****************************************************************************
*  FUNCTION:   resetCodecCap
*
*  PURPOSE:    Resets the codec capabilities list by setting all codecs to
*              unsupported.
*  PARAMETERS: codecCap - The codec list ot reset
*
*  RETURNS:    Nothing
*
*****************************************************************************/

void resetCodecCap(VRG_UINT8 codecCap[])
{
   int i;

   for (i = 0; i < CODEC_MAX_TYPES; i++)
   {
      codecCap[i] = CODEC_UNSUPPORTED;
   }
}

#if DPR_ENABLED
static void startDprEgress( HAPI_UINT16 handle )
{
   startDpr( handle, DPR_DIRECTION_EGRESS );
}

static void startDprIngress( HAPI_UINT16 handle )
{
   startDpr( handle, DPR_DIRECTION_INGRESS );
}

HAPI_UINT16 gHandle;

static void startDpr( HAPI_UINT16 handle, HAPI_UINT16 direction )
{
   DPRREGS dprRegs;

   dprRegs.handle    = handle;
   dprRegs.direction = direction;
   dprRegs.srvId     = DPR_BLOCK_VHD_THREAD;

   gHandle = handle;


   hdspSendCmdData( hdsp_LDX_HandleCreate( HAPIRESMAN, handle >> 8 ),
                    HAPI_DPR_SETREGS_CMD,
                    HSCMDEXTDATA,
                    sizeof(dprRegs),
                    &dprRegs );

   hdspSendCmd( hdsp_LDX_HandleCreate( HAPIRESMAN, handle >> 8 ),
                       HAPI_DPR_RESET_STATS_CMD,
                       HSCMDDATA,
                       0,
                       0 );

   hdspSendCmd( hdsp_LDX_HandleCreate( HAPIRESMAN, handle >> 8 ),
                       HAPI_DPR_ENABLE_CMD,
                       HSCMDDATA,
                       1,
                       0 );
}


static void stopDpr( void )
{
   DPRSTATS  dprStats;

   hdspSendCmd( hdsp_LDX_HandleCreate( HAPIRESMAN, gHandle >> 8 ),
                       HAPI_DPR_ENABLE_CMD,
                       HSCMDDATA,
                       0,
                       0 );

   hdspSendCmdData( hdsp_LDX_HandleCreate( HAPIRESMAN, gHandle >> 8 ),
                           HAPI_DPR_GETSTATS_CMD,
                           HSCMDEXTDATA,
                           sizeof( dprStats ),
                           &dprStats);

   VRG_LOG_DBG(VRG_LOG_MOD_EPT,  "" );
   VRG_LOG_DBG(VRG_LOG_MOD_EPT,  "DPR Stats:  " );
   VRG_LOG_DBG(VRG_LOG_MOD_EPT, "------------" );

   {
      unsigned long iterations;
      unsigned long current;
      unsigned long totalHigh;
      unsigned long totalLow;
      unsigned long avg;
      unsigned long max;
      unsigned long min;


      iterations = (dprStats.iterationsHigh << 16) + dprStats.iterationsLow;
      current    = (dprStats.currentHigh    << 16) + dprStats.currentLow;
      totalHigh  = (dprStats.totalHighHigh  << 16) + dprStats.totalHighLow;
      totalLow   = (dprStats.totalLowHigh   << 16) + dprStats.totalLowLow;
      avg        = (dprStats.avgHigh        << 16) + dprStats.avgLow;
      max        = (dprStats.maxHigh        << 16) + dprStats.maxLow;
      min        = (dprStats.minHigh        << 16) + dprStats.minLow;

      VRG_LOG_DBG(VRG_LOG_MOD_EPT, "max         %12lu (0x%08lx)",    max,        max );
      VRG_LOG_DBG(VRG_LOG_MOD_EPT, "average     %12lu remain %12lu", totalLow/iterations, totalLow%iterations );
   }
   VRG_LOG_DBG(VRG_LOG_MOD_EPT, "" );
}

#endif /* DPR_ENABLED */

void translateRingProfile(VRG_RING_PROFILE *pVrgProfile, CLASSSTM_RING_STATE *pCasProfile)
{
   switch (pVrgProfile->type)
   {
      case VRG_RING_SHORT:
      {
         memcpy(&(pCasProfile->provBits.shortRingPattern),
                &(pVrgProfile->pattern.shortRingPattern),
                sizeof(VRG_RING_SHORT_PATTERN));
         pCasProfile->ringType = CAS_CTRL_SHORT_NORMAL_RING;
      }
      break;

      case VRG_RING_LONG:
      {
         memcpy(&(pCasProfile->provBits.longRingPattern),
                &(pVrgProfile->pattern.longRingPattern),
                sizeof(VRG_RING_LONG_PATTERN));
         pCasProfile->ringType = CAS_CTRL_LONG_NORMAL_RING;
      }
      break;
      default:
         VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT ERROR: translateRingProfile - unknown ring type %d", pVrgProfile->type));
         break;
   }
}



/*****************************************************************************
*  FUNCTION:   requestTpd
*
*  PURPOSE:    Helper method to invoke TPD Processing
*
*  PARAMETERS: state  - endpoint state structure
*              signal - endpoint test start signal
*              value  - requested test case
*  RETURNS:
*
*  NOTE:
*
*****************************************************************************/
void requestTpd(VRG_ENDPT* state, EPSIG signal, unsigned int value)
{
#if VRG_ENDPT_CFG_USE_TPD
   VRG_BOOL bActive;
   int lineId = ((VRG_ENDPT_STATE *) state)->lineId;

   /* Set the test mode to indicate the linediags are in progress */
   if (state->testMode == TESTMODE_LINEDIAG)
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT,
                     "Overlapping TPD Request on line %d", lineId));
   }
   state->testMode = TESTMODE_LINEDIAG;

   /* Line is active if offhook or has a cnx on it */
   bActive = (state->offHook) || (state->cnxCnt > 0);

   if ( bActive )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "ENDPT %d WARNING: Request to run TPD in active state (offhook %d, cnxCnt %d)!",
                        lineId, state->offHook, state->cnxCnt));
   }
#ifndef VRG_ENDPT_CFG_USE_MLT
   if( signal == EPSIG_DIAGTESTS_PROV_START )
   {
      /* Signal the TPD task to start processing */
      tpdCtlProcessSignal( TPD_CTL_SIGNAL_PROV_TESTS, state->endptNum, bActive, -1 );
   }
   else if( signal == EPSIG_DIAGTESTS_NOPROV_START )
   {
      /* Signal the TPD task to start processing */
      tpdCtlProcessSignal( TPD_CTL_SIGNAL_NOPROV_TESTS, state->endptNum, bActive, value );
   }
   else if( signal == EPSIG_DIAGTESTS_PC15_START )
   {
      /* Signal the TPD task to start processing */
      tpdCtlProcessSignal( TPD_CTL_SIGNAL_PC15_TESTS, state->endptNum, bActive, -1 );
   }
   else if( signal == EPSIG_DIAGTESTS_CALIBRATE_START )
   {
      /* Signal the TPD task to start processing */
      tpdCtlProcessSignal( TPD_CTL_SIGNAL_CALIBRATION_TESTS, state->endptNum, bActive, -1 );
   }
#else
   mltRequest( state->endptNum, value, bActive );
#endif /* VRG_ENDPT_CFG_USE_MLT */
#endif /* VRG_ENDPT_CFG_USE_TPD */
}

/*****************************************************************************
*  FUNCTION:   requestLoopback
*
*  PURPOSE:    Helper method sets Loopback mode
*
*  PARAMETERS: state  - endpoint state structure
*              value  - dis/enable loopback
*
*  RETURNS:
*
*  NOTE:
*
*****************************************************************************/
void requestLoopback(VRG_ENDPT* state, unsigned int value)
{
   /* Let the heartbeat module know this endpt now has a signal being applied */
   hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

   if (value)
   {
      /* set echo cancellation to off in analog loopback mode */
      EcanStateSet(state, ECAN_OFF);
      state->testMode = TESTMODE_ANALOGLOOPBACK;
   }
   else
   {
      state->testMode = TESTMODE_NONE;
   }

   boardHalSetAnalogLoopback( state->lineId, value );

   if (!value)
   {
      /* Let the heartbeat module know this endpt now has a signal being applied */
      hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
   }
}

void applyIngressTone(VRG_ENDPT* state, int cnxIdx, VRG_COUNTRY country, const EPZTONEMAP* toneMapEntry)
{
   VRG_TONE_SPEC vrgTone;
   HDSP_TONE_PROV_PATTERN *pHdspTonePattern;

   /* Let the heartbeat module know this endpt now has a cnx signal being applied */
   hbUpdatePatientState( state->cnxSignalHBId, HBSTATE_ACTIVE );

   /* Get provisioned ringback pattern */
   if ( EPSTATUS_SUCCESS == Provision( state->endptArchive,
                                       toneMapEntry->provItem,
                                       &vrgTone ))
   {
      /* Successfully retrieved a provisioned ringback pattern, use it */
      pHdspTonePattern = (HDSP_TONE_PROV_PATTERN*)&vrgTone;
   }
   else
   { 
   VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Function applyIngressTone: cannot obtain the item from provTable!!!" ));   
   return;
   }

   /* Generate ingress tone */
   hdspToneOn( state->cnx[cnxIdx]->vhdhdl,
               state->lineId,
               CT_NOTONE,
               0,
               0,
               pHdspTonePattern,
               INGRESS_TONE,
               VRG_FALSE,
               country );
}

void applyEgressTone(VRG_ENDPT* state, VRG_COUNTRY country, const EPZTONEMAP* toneMapEntry, int duration)
{
   VRG_TONE_SPEC vrgTone;
   HDSP_TONE_PROV_PATTERN *pHdspTonePattern;
   VRG_UINT32 toneDelay = 0;

   /* Let the heartbeat module know this endpt now has a signal being applied */
   hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

   if (( toneMapEntry->provItem != - 1 ) &&
       ( EPSTATUS_SUCCESS == Provision( state->endptArchive,
                                        toneMapEntry->provItem,
                                        &vrgTone )))
   {
      /* Successfully retrieved a provisioned ringback pattern, use it */
      pHdspTonePattern = (HDSP_TONE_PROV_PATTERN*)&vrgTone;
   }
   else
   {
   VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Function applyIngressTone: cannot obtain the item from provTable!!!" ));   
   return;
   }

   /* Generate egress tone */
   hdspToneOn( state->lineVhdHdl,
               state->lineId,
               CT_NOTONE,
               (VRG_UINT16)toneDelay,
               0,
               pHdspTonePattern,
               EGRESS_TONE,
               VRG_FALSE,
               country );
}

void removeIngressTone(VRG_ENDPT* state, int cnxIdx)
{
   /* In case we were inactive and we get a request to turn a signal off,
   ** we better let the heartbeat module know we need to do some processing. */
   hbUpdatePatientState( state->cnxSignalHBId, HBSTATE_ACTIVE );

   /* Turning off ingress signals on a cnx  */
   hdspToneOff( state->cnx[cnxIdx]->vhdhdl, INGRESS_TONE );

   /* Return RTP mode to original state */
   SetRtpMode( state->cnx[cnxIdx], state->cnx[cnxIdx]->mode );
   ProgramRtpRegisters( state->cnx[cnxIdx] );

   /* Let the heartbeat module know this endpt no longer has an active
   ** cnx signal */
   hbUpdatePatientState( state->cnxSignalHBId, HBSTATE_INACTIVE );
}

void removeEgressTone(VRG_ENDPT* state, EPSIG signal)
{
   /* In case we were inactive and we get a request to turn a signal off,
   ** we better let the heartbeat module know we need to do some processing. */
   hbUpdatePatientState( state->signalHBId, HBSTATE_ACTIVE );

   hdspToneOff( state->lineVhdHdl, EGRESS_TONE );

   if (signal == EPSIG_DIAL || signal == EPSIG_SPECIAL_DIAL)
   {
      /* Process Ecan state machine */
      EcanProcessStm( state, ECANEVT_DIALTONE_OFF );
   }

   /* Let the heartbeat module know this endpt no longer has an active
   ** endpt signal */
   hbUpdatePatientState( state->signalHBId, HBSTATE_INACTIVE );
}

/*
*****************************************************************************
** FUNCTION:   SetDigitalMode
**
** PURPOSE:    Enable or disable 'digital mode' for a given connection
**
** PARAMETERS: state - endpoint state for the connection
**             cnx   - connection
**             value - VRG_TRUE to enable, VRG_FALSE to disable
**
** RETURNS:    void
*****************************************************************************
*/
void SetDigitalMode( VRG_ENDPT* state, CNXSTATE* cnx, VRG_BOOL value )
{
   if( value )
   {
      BOS_UINT32 result = 0;
      cnx->bBell103Enabled = VRG_FALSE;
      cnx->netRegs.hsxNetOptions = HSNET_NET_OPTION_IGNORE_CNG_ING_EVT |
                              HSNET_NET_OPTION_IGNORE_CNG_EGR_EVT |
                              HSNET_NET_OPTION_VBD_FAXR_AUTO_SWITCH_DISABLE;
      cnx->netRegs.hsxCallDOptions = 0;
      cnx->netRegs.hsxPtePacketVoiceOptions = 0;
      hdspVhdSetEgressGain(state->lineVhdHdl, 0);
      hdspVhdSetIngressGain(state->lineVhdHdl, 0);

      /* Disable services on the gateway */
      result = hdspSendCmd( cnx->vhdhdl, HAPI_PTE_ENABLE_CMD, HSCMDDATA, VRG_FALSE, 0 );
      if ( HAPISUCCESS != result)
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ENDPT: ERROR %X disabling PTE for GW vhd 0x%x", (unsigned int)result, cnx->vhdhdl ));
      }
      result = hdspSendCmd( cnx->vhdhdl, HAPI_CDIS_ENABLE, HSCMDDATA, VRG_FALSE, 0 );
      if ( HAPISUCCESS != result)
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ENDPT: ERROR %X disabling CDIS for GW vhd 0x%x", (unsigned int)result, cnx->vhdhdl ));
      }
      result = hdspSendCmd( cnx->vhdhdl, HAPI_V21_ENABLE_CMD, HSCMDDATA, VRG_FALSE, 0 );
      if ( HAPISUCCESS != result)
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ENDPT: ERROR %X disabling V21 for GW vhd 0x%x", (unsigned int)result, cnx->vhdhdl ));
      }

      /* Disable services on the line */
      result = hdspSendCmd( state->lineVhdHdl, HAPI_PTE_ENABLE_CMD, HSCMDDATA, VRG_FALSE, 0 );
      if ( HAPISUCCESS != result)
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ENDPT: ERROR %X disabling PTE for ENDPT vhd 0x%x", (unsigned int)result, state->lineVhdHdl ));
      }
      result = hdspSendCmd( state->lineVhdHdl, HAPI_LINE_HIGHPASS_ENABLE_CMD, HSCMDDATA, VRG_FALSE, 0 );
      if ( HAPISUCCESS != result )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ENDPT: ERROR disabling line HPF for ENDPT vhd 0x%x", state->lineVhdHdl ));
      }
      result = hdspSendCmd( state->lineVhdHdl, HAPI_LINE_BANDSPLIT_ENABLE_CMD, HSCMDDATA, VRG_FALSE, 0 );
      if ( HAPISUCCESS != result )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ENDPT: ERROR disabling band split for ENDPT vhd 0x%x", state->lineVhdHdl ));
      }
      result = hdspSendCmd( state->lineVhdHdl, HAPI_LINE_BANDCOMBINE_ENABLE_CMD, HSCMDDATA, VRG_FALSE, 0 );
      if ( HAPISUCCESS != result )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ENDPT: ERROR disabling band combine for ENDPT vhd 0x%x", state->lineVhdHdl ));
      }
      result = hdspSendCmd( state->lineVhdHdl, HAPI_LINE_EQU_CONFIG_FILTER_CMD, HSCMDDATA, 0, 0 );
      if ( HAPISUCCESS != result )
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ENDPT: ERROR disabling equalizer filter for ENDPT vhd 0x%x", state->lineVhdHdl ));
      }

      /* Enable no-compress mode on the HAL */
      lhapiHalPcmNoCompress( state->lineId, VRG_TRUE );

      /* Update the line's state to offhook. This ensures gains on the line are 0 */
      EndptUpdateCasState( state->lineId, EPCAS_OFFHOOK );

      /* disable ECAN */
      hdspSendCmdAsync( state->lineVhdHdl, HAPI_HEC_ENABLE_CMD, HSCMDDATA, VRG_FALSE, 0 );
   }
   else
   {
      /* Re-enable services on the gateway */
      hdspSendCmd( cnx->vhdhdl, HAPI_PTE_ENABLE_CMD, HSCMDDATA, VRG_TRUE, HAPI_PTE_MODE_DIG_EVT );
      hdspSendCmd( cnx->vhdhdl, HAPI_CDIS_ENABLE, HSCMDDATA, VRG_TRUE, 0);
      hdspSendCmd( cnx->vhdhdl, HAPI_V21_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );

      /* Re-enable services on the line */
      hdspSendCmd( state->lineVhdHdl, HAPI_PTE_ENABLE_CMD, HSCMDDATA, VRG_TRUE, HAPI_PTE_MODE_DIG_EVT | HAPI_PTE_INGRESSMASK );
      hdspSendCmd( state->lineVhdHdl, HAPI_LINE_HIGHPASS_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );
      hdspSendCmd( state->lineVhdHdl, HAPI_LINE_BANDSPLIT_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );
      hdspSendCmd( state->lineVhdHdl, HAPI_LINE_BANDCOMBINE_ENABLE_CMD, HSCMDDATA, VRG_TRUE, 0 );

      /* Disable no-compress mode on the HAL */
      lhapiHalPcmNoCompress( state->lineId, VRG_FALSE );
   }
}

const EPZTONEMAP* getEpzToneMapEntry(EPSIG signal)
{
   static const EPZTONEMAP epToneMap[] =
   {
      { EPSIG_BUSY,             EPPROV_ToneBusy           }, /* Busy tone */
      { EPSIG_CONF,             EPPROV_ToneConfirmation   }, /* Confirmation tone */
      { EPSIG_DIAL,             EPPROV_ToneDial           }, /* Dial tone */
      { EPSIG_MSGW,             EPPROV_ToneMessageWaiting }, /* Message waiting */
      { EPSIG_CALLWT,           EPPROV_ToneCallWaiting    }, /* Call waiting */
      { EPSIG_CALLW1,           EPPROV_ToneCallWaiting1   }, /* Call waiting */
      { EPSIG_CALLW2,           EPPROV_ToneCallWaiting2   }, /* Call waiting */
      { EPSIG_CALLW3,           EPPROV_ToneCallWaiting3   }, /* Call waiting */
      { EPSIG_CALLW4,           EPPROV_ToneCallWaiting4   }, /* Call waiting */
      { EPSIG_SAS_CAS,          EPPROV_ToneCallWaiting    }, /* SAS & CAS tones for CLASS2 */
      { EPSIG_SAS_CAS1,         EPPROV_ToneCallWaiting1   }, /* SAS & CAS tones #1 for CLASS2 */
      { EPSIG_SAS_CAS2,         EPPROV_ToneCallWaiting2   }, /* SAS & CAS tones #2 for CLASS2 */
      { EPSIG_SAS_CAS3,         EPPROV_ToneCallWaiting3   }, /* SAS & CAS tones #3 for CLASS2 */
      { EPSIG_SAS_CAS4,         EPPROV_ToneCallWaiting4   }, /* SAS & CAS tones #4 for CLASS2 */
      { EPSIG_OHWARN,           EPPROV_ToneOffHookWarning }, /* Receiver off hook */
      { EPSIG_REORDER,          EPPROV_ToneReOrder        }, /* Reorder */
      { EPSIG_RINGBACK,         EPPROV_ToneRingBack       }, /* Ringback */
      { EPSIG_STUTTER,          EPPROV_ToneStutterdial    }, /* Stutter dial */
      { EPSIG_CALLCARD,         EPPROV_CallingCard                        }, /* Calling card */
      { EPSIG_NETBUSY,          EPPROV_ToneCongestion     }, /* Network busy */
      { EPSIG_NETBUSY1,         EPPROV_ToneCongestion1    }, /* Network busy */
      { EPSIG_DISCONNECT,       EPPROV_ToneUserDefined5   }, /* Line Disconnect */
      { EPSIG_PAYPHONE_REC,     EPPROV_ToneUserDefined6   }, /* Special Information */
      { EPSIG_INTRUSION,        EPPROV_ToneUserDefined2   }, /* Instrusion tone (Norway) */
      { EPSIG_WARNING,          EPPROV_ToneUserDefined3   }, /* Warning tone (Norway) */
      { EPSIG_DIAL2,            EPPROV_ToneUserDefined4   }, /* Centrex dial tone (Norway) */
      { EPSIG_METERING_BURST,   -1                        }, /* Pulse Burst */
      { EPSIG_ENABLE_METERING,  -1                        }, /* Enable Metering */
      { EPSIG_INIT_RING,        -1                        }, /* Initial ring */
      { EPSIG_RINGBACK_CUST1,   EPPROV_ToneUserDefined1   }, /* Custom Ringback 1 */
      { EPSIG_SPECIAL_DIAL,     EPPROV_ToneSpecialDial    }, /* Special dial tone */
      { EPSIG_FILL_INFO,        -1                        }, /* Filler Information (UAE) */
      { EPSIG_NULL,             -1                        }
   };

   const EPZTONEMAP *toneMap = epToneMap;

   /* Map from EPEVT to CUSTOM_TONE */
   while(( toneMap->event != EPSIG_NULL ) &&
         ( toneMap->event != signal ))
   {
      toneMap++;
   }

   return(toneMap);
}


/*
*****************************************************************************
** FUNCTION:   SetAdditionalGain
**
** PURPOSE:    To set the RX/TX gain in VBD mode for optimized V.90 performance.
**
** PARAMETERS: cnx   - pointer to connection state
**             mode  - NET mode
**
** RETURNS:    NONE
**
**
*****************************************************************************
*/
void SetAdditionalGain( CNXSTATE *cnx, VRG_BOOL bAddGainVBD )
{
   VRG_ENDPT     *state;

   state = GetEndptState( cnx->lineId );

   if( !state )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_EPT, "SetAdditionalGain: unknown endpt for line %d", cnx->lineId ));
      return;
   }

   if ( bAddGainVBD )
   {
      /* provTable does not have items EPPROV_VBDTxGain and EPPROV_VBDRxGain. 
       * These items are currently not supported.
       * To enable VGD Gain in the future, add its provItem to provTable and provision below*/

      /* Set the ingress and egress gains to 0 */
      hdspVhdSetIngressGain( state->lineVhdHdl, 0 );
      hdspVhdSetEgressGain( state->lineVhdHdl, 0 );
   }
   else
   {
      /* Revert ingress & egress gain to original */
      if (state->offHook)
      {
         /* Only revert ingress gain in offhook state.
         ** Endpt is muted to prevent reflection in onhook state. */
         hdspVhdSetIngressGain( state->lineVhdHdl, state->txGain );
      }

      hdspVhdSetEgressGain( state->lineVhdHdl, state->rxGain );
   }
}

/*
*****************************************************************************
** FUNCTION:   SetSilenceSuppression
**
** PURPOSE:    To set silence suppression.
**
** PARAMETERS: cnx            - pointer to connection state
**             vad            - VAD mode
**             plc            - plc mode
**             comfortnoise   - comfort noise mode
**
** RETURNS:    NONE
**
**
*****************************************************************************
*/
void SetSilenceSuppression( CNXSTATE *cnx, VRG_UINT16 vad, VRG_UINT16 plc, VRG_UINT16 comfortnoise )
{
   VRG_ENDPT     *state;

   state = GetEndptState( cnx->lineId );
   if (state)
   {
      if ( state->testMode & TESTMODE_VADSET )
      {
         if (state->diagVadMode == -1)
         {
            return;
         }
         else
         {
            /* Restore the vad mode which was previously set. */
            vad = (VRG_UINT16)(state->diagVadMode);
         }
      }

      /* Modify the VAD/CNG/PLC settings only if something changed */
      if ((cnx->VadMode != vad) ||
          (cnx->PlcMode != plc) ||
          (cnx->comfortNoise != comfortnoise))
      {
         hdspVhdSetVadCngPlc( cnx->vhdhdl, vad,
                              cnx->comfortNoise, plc );

         /* Save the new settings for future reference */
         cnx->VadMode = vad;
         cnx->PlcMode = plc;
      }
   }
}

/*
*****************************************************************************
** FUNCTION:   HapiProcessEcanEvent
**
** PURPOSE:    Handle printing of ECAN debug events.
**
** PARAMETERS: hapiEvt  (in)  Hausware event.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void HapiProcessEcanEvt( HSZEVT *hapiEvt )
{
   VRG_UINT16 op1 = hapiEvt->hsxop1;
   VRG_UINT16 op2 = hapiEvt->hsxop2.val;

   switch( hapiEvt->hsxevt )
   {
      case HAPI_HEC_STATE_EVT:
      case HAPI_AEC_STATE_EVT:
         switch(op1)
         {
            case 0: VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN mode quiescent"));  break;
            case 2: VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN mode receive"));    break;
            case 4: VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN mode send"));       break;
            case 6: VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN mode doubletalk")); break;
            case 252: VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN autotune aborted, too noisy (Lsin=%d)", HEC_DBQ82DB((VRG_SINT16)op2) )); break;
            case 253: VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN autotune silence measure (Lsin=%d)", HEC_DBQ82DB((VRG_SINT16)op2) )); break;
            case 254:
               if (op2)
               {
                  VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN autotune mode (0x%x)",op2))
               }
               else
               {
                  VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN autotune complete"));
               }
               break;
            case 255: VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN autotune complete")); break;
            default:  VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN STATE_EVT unknown")); break;
         }
         break;
      case HAPI_HEC_CONVERGED_EVT:
      case HAPI_AEC_CONVERGED_EVT:
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN %sVERGED (%u)",op1?"CON":"DI",op1));
         break;
      case HAPI_AEC_RESET_EVT:
      case HAPI_HEC_RESET_EVT:
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN (%s) RESET=%u",op2==0xaec?"AEC":"HEC",op1));
         break;
      case HAPI_AEC_PUSH_EVT:
      case HAPI_HEC_PUSH_EVT:
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN PUSH=%u %d",op1,((VRG_SINT16)op2)/256));
         break;
      case HAPI_AEC_NEAREND_EVT:
      case HAPI_HEC_NEAREND_EVT:
         if (op1 || op2)
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN near end ON (unwht=%d wht=%d high=%d sb=%d sbindex=%d)", (op1>>8)&0xff, op1&0xff, (op2>>8)&0xff, (op2&0xff)>>3, op2&7));
         }
         else
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN near end OFF"));
         }
         break;
      case HAPI_AEC_NLP_EVT:
      case HAPI_HEC_NLP_EVT:
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN NLP %s (mask=0x%x)",op1?"on":"off",op2));
         break;
      case HAPI_AEC_HIGHLEVEL_EVT:
      case HAPI_HEC_HIGHLEVEL_EVT:
         if (op1 >= 0x7f00)
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "obstruction change, zone=%d reflector=%d",op1&0xff,op2));
         }
         else if (op1 && op2)
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "High level clipped"));
         }
         else if (op1)
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "Clipped (hc=%d)",op1));
         }
         else if (op2)
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "High level (hc=%d)",op2));
         }
         else
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "High level off"));
         }
         break;
      case HAPI_AEC_ERL_EVT:
      case HAPI_HEC_ERL_EVT:
         if (op1 <= 8000)
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN ERL=%d dB  tone=%u Hz", ((VRG_SINT16)op2+128)/256, op1));
         }
         else
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN ERL=%d dB", ((VRG_SINT16)op2+128)/256));
         }
         break;
      case HAPI_AEC_ERLE_EVT:
      case HAPI_HEC_ERLE_EVT:
         if (op1 <= 8000)
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN ERLE=%d dB tone=%u Hz", ((VRG_SINT16)op2+128)/256, op1));
         }
         else
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN ERLE=%d dB", ((VRG_SINT16)op2+128)/256));
         }
         break;
      case HAPI_AEC_ACOM_EVT:
      case HAPI_HEC_ACOM_EVT:
         if (op1 <= 8000)
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN ERL=%d dB ERLE=%d dB Acom=%d dB tone=%u Hz",
                     ((VRG_SINT16)op2&0xff00)/256, (char)(op2 & 0xFF),
                     ((VRG_SINT16)op2&0xff00)/256 + (char)(op2 & 0xFF), op1));
         }
         else
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN ERL=%d dB ERLE=%d dB Acom=%d dB",
                     ((VRG_SINT16)op2&0xff00)/256, (char)(op2 & 0xFF),
                     ((VRG_SINT16)op2&0xff00)/256 + (char)(op2 & 0xFF)));
         }
         break;
      case HAPI_AEC_TUNEACOM_EVT:
      case HAPI_HEC_TUNEACOM_EVT:
         if (op2==0)
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN Acom (-12 dBm noise)=%d", ((VRG_SINT16)op1+128)/256));
         }
         else if (op2==99)
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN tone Acom=%d", ((VRG_SINT16)op1+128)/256));
         }
         else
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN Acom=%d (noise level = %d dBm)", ((VRG_SINT16)op1+128)/256,-12+op2));
         }
         break;
      case HAPI_AEC_AGC_EVT:
      case HAPI_HEC_AGC_EVT:
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN AGC=[%d,%d]", ((VRG_SINT16)op1+128)/256,((VRG_SINT16)op2+128)/256));
         break;
      case HAPI_AEC_TUNEERL_EVT:
      case HAPI_HEC_TUNEERL_EVT:
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN minmaxERLdB=[%d,%d]", ((VRG_SINT16)op1+128)/256,((VRG_SINT16)op2+128)/256));
         break;
      case HAPI_AEC_TUNEERLE_EVT:
      case HAPI_HEC_TUNEERLE_EVT:
         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "ECAN minmaxERLEdB=[%d,%d]", ((VRG_SINT16)op1+128)/256,((VRG_SINT16)op2+128)/256));
         break;
      case HAPI_HEC_GETSTATE_EVT:
      {
         HEC_STATE *statep = (HEC_STATE *)hapiEvt->hsxop2.ref;
         HAPI_SINT16 *ptr = (HAPI_SINT16 *)statep;

         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "ECAN state converged=%d,send=%d,receive=%d,stationary=%d,high mips=%d,zone=%d)",
                        (ptr[0]&HAPI_HEC_STATE_CONVERGED)?1:0,
                        (ptr[0]&HAPI_HEC_MODE_SEND)?1:0,
                        (ptr[0]&HAPI_HEC_MODE_RECEIVE)?1:0,
                        (ptr[0]&HAPI_HEC_STATE_RXSTATIONARY)?1:0,
                        (ptr[0]&HAPI_HEC_STATE_HIGH_COMPLEXITY)?1:0,
                        (ptr[0]&HAPI_HEC_STATE_ZONEMASK)>>HAPI_HEC_STATE_ZONESHIFT));
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "  state 0x%x, Lrin %d, Lrout %d, Lsin %d, Lsout %d, LsoutMin %d",
                        statep->state,
                        HEC_DBQ82DB(statep->Lrin),
                        HEC_DBQ82DB(statep->Lrout),
                        HEC_DBQ82DB(statep->Lsin),
                        HEC_DBQ82DB(statep->Lsout),
                        HEC_DBQ82DB(statep->LsoutMin) ));
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "  ERL %d, Erlc %d, ERLE %d, Vol (auto) %d, Gain (auto) %d",
                        HEC_DBQ82DB(statep->erldB),
                        HEC_DBQ82DB(statep->erlcdB),
                        HEC_DBQ82DB(statep->erledB),
                        HEC_DBQ82DB(statep->volumeAutodB),
                        HEC_DBQ82DB(statep->gainAutodB) ));
         VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "  NLP dec 0x%x, Voice dec 0x%x, Adapt cnt %d, Peak refl %d, Peak refl val %d",
                        statep->nlp,
                        statep->voiceDecisions,
                        statep->adaptCount,
                        statep->peakReflector,
                        statep->peakReflectorValue ));
      }
      break;

      default:
         break;
   }
}
