/****************************************************************************
*
*  HPPVE.H - HAUSWARE Definitions for a Voice Algorithm Service
*
*  PURPOSE:
*     This file contains the PVE API definitions.
*
**********************************************************************/


#ifndef HP_CODEC_H
#define HP_CODEC_H

#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* Hausware API Global definitions */
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware service ids */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>              /* Hausware generic cmds/events */
#endif


#define HAPICODECSRV                HAPI_CODEC_SRV

/*************************************************************************
**          CODEC SERVICE COMMANDS
*************************************************************************/

#define  HAPI_CODEC_ENABLE_CMD      (HSF_MKHDR(HAPICODECSRV, HAPI_OPC_ENABLE_CMD))
      /* OP1: 1=ENABLE, 0=DISABLE */
      /* OP2: reserved = 0        */

#define HAPI_CODEC_GETREGS_CMD      (HSF_MKHDR(HAPICODECSRV, HAPI_OPC_GETREGS_CMD)) /* get codec registers */
      /* OP1: reserved = 0          */
      /* OP2: pointer to register (return) */

#define HAPI_CODEC_SETREGS_CMD      (HSF_MKHDR(HAPICODECSRV, HAPI_OPC_SETREGS_CMD))  /* set vocoder options */
      /* OP1: reserved = 0          */
      /* OP2: new register values   */

#define HAPI_CODEC_SETREG1_CMD      (HSF_MKHDR(HAPICODECSRV, HAPI_OPC_SETREG1_CMD)) /* set vocoder options */

#define HAPI_CODEC_GETSTATS_CMD     (HSF_MKHDR(HAPICODECSRV, HAPI_OPC_GETSTATS_CMD))/* Get PVE statistics */

#define HAPI_CODEC_GETSTATE_CMD     (HSF_MKHDR(HAPICODECSRV, HAPI_OPC_GETSTATE_CMD))/* Get PVE state */

#define HAPI_CODEC_ENCSTART_CMD     (HSF_MKCMD_STD(HAPICODECSRV, 0)) /* start / stop the encoder */
      /* OP1: 1 - ENABLE, 0=DISABLE */
      /* OP2: reserved = 0          */

#define HAPI_CODEC_SETVOICERATE_CMD (HSF_MKCMD_STD(HAPICODECSRV, 1)) /* set/change the vocoder rate */
      /* OP1: vocoder rate (refer to payload.h) */
      /* OP2: superpacketization interval in number of 8kHz samples(40, 80,160,240)  */
#define HAPI_CODEC_FREEZE_CMD       (HSF_MKCMD_STD(HAPICODECSRV, 2)) /* freeze the jitter buffer */
      /* OP1: TRUE = FREEZE, FALSE = UNFREEZE */
      /* OP2: TRUE = VBD mode, FALSE = non-VBD mode */

#define HAPI_CODEC_DECFLUSH_CMD     (HSF_MKCMD_STD(HAPICODECSRV, 3)) /* flush the decoder playout Q */

#define HAPI_CODEC_CONFIG_CMD       (HSF_MKCMD_STD(HAPICODECSRV, 4))

#define HAPI_CODEC_TONERELAY_CMD    (HSF_MKCMD_STD(HAPICODECSRV, 5))
      /* OP1: 1 = tone relay starts, 0 = tone relay ends */
      /* OP2: reserved */

#define HAPI_CODEC_PAYLOADFLUSH     (HSF_MKCMD_STD(HAPICODECSRV, 6))

#define HAPI_CODEC_BOP_DEVICE_ENABLE_CMD    (HSF_MKCMD_STD(HAPICODECSRV, 7))
      /* OP1: 1 = Disables bandsplit/combine, 0 = Enables bandsplit/combine */
      /* OP2: reserved */
#define HAPI_CODEC_GETJB_CMD        (HSF_MKCMD_STD(HAPICODECSRV, 8))
      /* op1: Get JB option */
      /* op2: enable / disable */
#define HAPI_CODEC_PTEOPTION_CMD    (HSF_MKCMD_STD(HAPICODECSRV, 9))
      /* op1: pte option */
      /* op2: 0 */

#define HAPI_CODEC_GETSTATUS_CMD    (HSF_MKCMD_STD(HAPICODECSRV, 10))
      /* op1: reserved */
      /* op2: reserved */

#define HAPI_CODEC_AUDIOSTOPTONE_CMD (HSF_MKCMD_STD(HAPICODECSRV, 11))
      /* OP1: 1 = enable audio to stop tone relay, 0 = disable  */
      /* OP2: reserved  */

#define HAPI_CODEC_STOP_ADAPT_CMD    (HSF_MKCMD_STD(HAPICODECSRV, 12)) /* freeze the jitter buffer at the current hold time*/
      /* OP1: TRUE = FREEZE, FALSE = UNFREEZE */
      /* OP2: not used */

#define HAPI_CODEC_INTERMSG_CMD      (HSF_MKCMD_STD(HAPICODECSRV, 13)) /* special coder <-> decoder communication */
      /* OP1: special command type (see HAPI_CODEC_INTERMSG_ list) */
      /* OP2: pointer to parameters, or new parameter value */


/* List of coder <-> decoder communication messages */
#define HAPI_CODEC_INTERMSG_CMR      0 /* Codec-mode-change request from decoder to encoder (used in AMR) */


typedef enum
{
   PVE_GETJB_PRIME_JB = 0,
   PVE_GETJB_ENABLED

} CODEC_GETJB_OPTION;

/* responses */
#define HAPI_CODEC_ENABLE_RSP       (HSF_MKRSP_EXT(HAPI_CODEC_ENABLE_CMD))
#define HAPI_CODEC_GETREGS_RSP      (HSF_MKRSP_EXT(HAPI_CODEC_GETREGS_CMD))
#define HAPI_CODEC_SETREGS_RSP      (HSF_MKRSP_EXT(HAPI_CODEC_SETREGS_CMD))
#define HAPI_CODEC_SETREG1_RSP      (HSF_MKRSP_EXT(HAPI_CODEC_SETREG1_CMD))
#define HAPI_CODEC_GETSTATS_RSP     (HSF_MKRSP_EXT(HAPI_CODEC_GETSTATS_CMD))
#define HAPI_CODEC_GETSTATE_RSP     (HSF_MKRSP_EXT(HAPI_CODEC_GETSTATE_CMD))

#define HAPI_CODEC_ENCSTART_RSP     (HSF_MKRSP_STD(HAPI_CODEC_ENCSTART_CMD))
#define HAPI_CODEC_SETVOICERATE_RSP (HSF_MKRSP_STD(HAPI_CODEC_SETVOICERATE_CMD))
#define HAPI_CODEC_FREEZE_RSP       (HSF_MKRSP_STD(HAPI_CODEC_FREEZE_CMD))
#define HAPI_CODEC_DECFLUSH_RSP     (HSF_MKRSP_STD(HAPI_CODEC_DECFLUSH_CMD))
#define HAPI_CODEC_CONFIG_RSP       (HSF_MKRSP_STD(HAPI_CODEC_CONFIG_CMD))
#define HAPI_CODEC_TONERELAY_RSP    (HSF_MKRSP_STD(HAPI_CODEC_TONERELAY_CMD))

#define HAPI_CODEC_BOP_DEVICE_ENABLE_RSP (HSF_MKRSP_STD(HAPI_CODEC_BOP_DEVICE_ENABLE_CMD))
#define HAPI_CODEC_GETJB_RSP (HSF_MKRSP_STD(HAPI_CODEC_GETJB_CMD))

/*************************************************************************
**          CODEC SERVICE EVENTS
*************************************************************************/
#define HAPI_CODEC_INGRESSPACKET_EVT (HSF_MKEVT_EXT(HAPICODECSRV, 0))   /* encoded data from encoder */
#define HAPI_CODEC_INGRESSPACKET     HAPI_CODEC_INGRESSPACKET_EVT
      /* OP1: length of packet       */
      /* OP2: pointer to packet data */

#define HAPI_CODEC_VOICERATE_EVT     (HSF_MKEVT_STD(HAPICODECSRV, 1))   /* change of encoder rate */
#define HAPI_CODEC_VOICERATE         HAPI_CODEC_VOICERATE_EVT
      /* OP1: current voice rate (payloadcode) */
      /* OP2: 0 = encoder rate change          */
      /* OP2: 1 = decoder rate change          */

#define HAPI_CODEC_GETREGS_EVT       (HSF_MKEVT_EXT(HAPICODECSRV, 2))   /* response to getreg command */
#define HAPI_CODEC_GETREGS           HAPI_CODEC_GETREGS_EVT
      /* OP1: length of register (in bytes) */
      /* OP2: pointer to buffer             */

#define HAPI_CODEC_GETSTATS_EVT      (HSF_MKEVT_EXT(HAPICODECSRV, 3))   /* response to getreg command */
#define HAPI_CODEC_GETSTATS          HAPI_CODEC_GETSTATS_EVT
      /* OP1: length of register (in bytes) */
      /* OP2: pointer to buffer             */

#define HAPI_CODEC_EXCEPTION_EVT     (HSF_MKEVT_STD(HAPICODECSRV, 4))   /* EXCEPTION event from the CODEC service */
#define HAPI_CODEC_EXCEPTION         HAPI_CODEC_EXCEPTION_EVT
      /* OP1: type of problem (refer to HSNETEXCEPTION) */
      /* OP2: reserved = 0          */

#define HAPI_CODEC_GETSTATE_EVT      (HSF_MKEVT_EXT(HAPICODECSRV, 5))   /* response to getreg command */
#define HAPI_CODEC_GETSTATE          HAPI_CODEC_GETSTATE_EVT
      /* OP1: length of register (in bytes) */
      /* OP2: pointer to buffer             */

/* used for debug clock drift compensation algorithm */
#define HAPI_CODEC_CLKDRIFT_EVT       (HSF_MKEVT_STD(HAPICODECSRV, 6))
#define HAPI_CODEC_CLKDRIFT           HAPI_CODEC_CLKDRIFT_EVT
#define HAPI_CODEC_CLKHOLDTIME_EVT   (HSF_MKEVT_STD(HAPICODECSRV, 7))
#define HAPI_CODEC_CLKHOLDTIME       HAPI_CODEC_CLKHOLDTIME_EVT

#define HAPI_CODEC_CNG_EVT           (HSF_MKEVT_STD(HAPICODECSRV, 8))   /* CNG generation start/stop event */
      /* OP1: 0 = stop, 1 = start */
      /* OP2: reserved */

#define HAPI_CODEC_GETSTATUS_EVT     (HSF_MKEVT_EXT(HAPICODECSRV, 9))   /* response to get status command */
#define HAPI_CODEC_GETSTATUS          HAPI_CODEC_GETSTATUS_EVT
      /* OP1: length of register (in bytes) */
      /* OP2: pointer to buffer             */

#define HAPI_CODEC_AJC_REPEAT_EVT    (HSF_MKEVT_STD(HAPICODECSRV, 10))  /*  */
      /* OP1:  currentHoldTime */
      /* OP2:  0 */

#define HAPI_CODEC_AJC_DELETE_EVT    (HSF_MKEVT_STD(HAPICODECSRV, 11))  /*  */
      /* OP1:  currentHoldTime */
      /* OP2:  0: packet discarded/ 1: "virtual" delete (jitter buffer empty) */

#define HAPI_CODEC_AJC_STATE_EVT     (HSF_MKEVT_STD(HAPICODECSRV, 12))  /*  */
      /* OP1:  HAPI_CODEC_AJC_BUILDOUT/ HAPI_CODEC_AJC_ACTIVE/ HAPI_CODEC_AJC_SILENCE/ HAPI_CODEC_AJC_UNDERRUN */
      /* OP2:  1: buildout entered due to long silence, 0: otherwise */

#define HAPI_CODEC_AJC_ADAPT_EVT     (HSF_MKEVT_STD(HAPICODECSRV, 13))  /*  */
      /* OP1:  HAPI_CODEC_AJC_LATEREORDER/ HAPI_CODEC_AJC_METRICS */
      /* OP2:  currentHoldtime */

#define HAPI_CODEC_AJC_UNDERRUN_EVT  (HSF_MKEVT_STD(HAPICODECSRV, 14))  /*  */
      /* OP1:  currentHoldtime (0 if it wasn't adapted) */
      /* OP2:  underrun duration in packets */

#define HAPI_CODEC_AJC_PENDING_EVT   (HSF_MKEVT_STD(HAPICODECSRV, 15))  /*  */
      /* OP1:  HAPI_CODEC_AJC_PENDINGREPEAT_ABORTED/ HAPI_CODEC_AJC_PENDINGDELETE_ABORTED/ HAPI_CODEC_AJC_LAUNCHPENDING_REPEAT/ HAPI_CODEC_AJC_LAUNCHPENDING_DELETE */
      /* OP2:  minHoldTimeTracker*/

#define HAPI_CODEC_AJC_LOSTPACKETS_EVT (HSF_MKEVT_STD(HAPICODECSRV, 16))  /*  */
      /* OP1:  number of lost packets */
      /* OP2:  0- underran/ 1- no underrun */

#define HAPI_CODEC_AJC_LATE_EVT  (HSF_MKEVT_STD(HAPICODECSRV, 17))  /*  */
      /* OP1:  HAPI_CODEC_AJC_ACTIVE/ HAPI_CODEC_AJC_SID */
      /* OP2:  0 */

#define HAPI_CODEC_AJC_RESET_EVT     (HSF_MKEVT_STD(HAPICODECSRV, 18))  /*  */
      /* OP1:  HAPI_CODEC_AJC_RESET_BADTIMESTAMP/  HAPI_CODEC_AJC_RESET_OVERFLOW */
      /* OP2:  0 */

#define HAPI_CODEC_AJC_DISCARD_EVT   (HSF_MKEVT_STD(HAPICODECSRV, 19))  /*  */
      /* OP1:  HAPI_CODEC_AJC_DISCARD_LATE */
      /* OP2:  */

#define HAPI_CODEC_EGRESS_VBD_TO_VOICE_DETECT_EVT (HSF_MKEVT_STD(HAPICODECSRV, 20))  /*  */
      /* OP1:  0 */
      /* OP2:  0 */

#define HAPI_CODEC_EGRESS_VOICE_TO_VBD_DETECT_EVT (HSF_MKEVT_STD(HAPICODECSRV, 21))  /*  */
      /* OP1:  0 */
      /* OP2:  0 */

#define HAPI_CODEC_AJC_BUILDOUT  0
#define HAPI_CODEC_AJC_ACTIVE    1
#define HAPI_CODEC_AJC_SILENCE   2
#define HAPI_CODEC_AJC_UNDERRUN  3

#define HAPI_CODEC_AJC_LATEREORDER 0
#define HAPI_CODEC_AJC_METRICS     1

#define HAPI_CODEC_AJC_PENDINGREPEAT_ABORTED 0
#define HAPI_CODEC_AJC_PENDINGDELETE_ABORTED 1
#define HAPI_CODEC_AJC_LAUNCHPENDING_REPEAT  2
#define HAPI_CODEC_AJC_LAUNCHPENDING_DELETE  3

#define HAPI_CODEC_AJC_LATEACTIVE 0
#define HAPI_CODEC_AJC_LATESID    1

#define HAPI_CODEC_AJC_RESET_BADTIMESTAMP 0
#define HAPI_CODEC_AJC_RESET_OVERFLOW         1

#define HAPI_CODEC_AJC_DISCARD_LATE       0

/* Status word defines */
#define HSCODECENABLE           0x1      /* status word bit 0 */

#endif      /* HAPI CODEC H */



