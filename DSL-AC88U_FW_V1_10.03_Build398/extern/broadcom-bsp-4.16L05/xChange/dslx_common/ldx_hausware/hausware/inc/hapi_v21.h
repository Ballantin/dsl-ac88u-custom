/****************************************************************************
*
*  HAPI_V21.H - HAUSWARE Definitions for a V21 Modem Service
*
*  PURPOSE:
*     This file contains the V21 Modem API definitions.
*
*  DESCRIPTIONS:
*
*   COMMANDS:
*
*     HAPIV21ENABLE     Enable/Disable modem
*                          OP1 -> 1/0
*
*     HAPIV21TXDATA     Send transmit data buffer to modem
*                          OP1 -> buffer size
*                          OP2 -> points to the data buffer
*
*     HAPIV21TXCFG      Configure V.21 Transmitter
*                          OP1 -> mode
*
*     HAPIV21RXCFG      Configure V.21 Receiver
*                          OP1 -> 1/0
*   EVENTS:
*
*     HAPIV21RXDATA    One word (16-bits) or less of data is received
*                          ID  -> -1
*                          OP1 -> Number of valid bits starting from the MSB
*                          OP2 -> The received data bits
*
*     HAPIV21TXDATASENT   Finished sending one buffer of data
*                          ID  -> -1
*                          OP1 -> size
*                          OP2 -> points to the data buffer *
*  NOTES:
*
*                 (c) Copyright HotHaus Technologies 1996
*                                  All rights reserved
*
****************************************************************************/
#ifndef HAPI_V21_H                           /* support nested includes */
#define HAPI_V21_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>                   /* Hausware environment definitions */
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* Hausware API Global definitions */
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware service ids */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* Hausware generic cmds/events */
#endif
#ifndef HAPI_VMDM
#include <hapi_vmdm.h>                 /* Modem generic definitions */
#endif

                                          /* V.21 MODEM SERVICE COMMANDS */
/*
 * Generic commands:
 *    - have the STD/EXT bit set in the HAPI_OPC_xxx define
 *    - don't require descriptions of op1 and op2 - see hapi_opcode.h for these
 */
#define  HAPI_V21_ENABLE_CMD        (HSF_MKHDR(HAPIV21SRV, HAPI_OPC_ENABLE_CMD))
#define  HAPI_V21_SETREGS_CMD       (HSF_MKHDR(HAPIV21SRV, HAPI_OPC_SETREGS_CMD))
#define  HAPI_V21_RESETREGS_CMD     (HSF_MKHDR(HAPIV21SRV, HAPI_OPC_RESETREGS_CMD))
#define  HAPI_V21_SETREG1_CMD       (HSF_MKHDR(HAPIV21SRV, HAPI_OPC_SETREG1_CMD))
#define  HAPI_V21_GETREGS_CMD       (HSF_MKHDR(HAPIV21SRV, HAPI_OPC_GETREGS_CMD))
#define  HAPI_V21_GETSTATE_CMD      (HSF_MKHDR(HAPIV21SRV, HAPI_OPC_GETSTATE_CMD))
/*
 * V.21 service specific commands
 */
#define  HAPI_V21_GETSTATUS_CMD     (HSF_MKCMD_STD(HAPIV21SRV, HAPIVMDMGETSTATUS))
   /* op1: reserved = 0
      op2: reserved = 0 */
#define  HAPI_V21_DISCONNECT_CMD    (HSF_MKCMD_STD(HAPIV21SRV, HAPIVMDMDISCONNECT))
   /* op1: reserved = 0
      op2: reserved = 0 */

#define  HAPI_V21_RXCFG_CMD         (HSF_MKCMD_STD(HAPIV21SRV, HAPIVMDMRXCFG))
   /* op1: 1 for start, 0 for stop
      op2: reserved = 0 */

#define  HAPI_V21_SETIDLEPTN_CMD    (HSF_MKCMD_EXT(HAPIV21SRV, HAPIVMDMSETIDLEPTN))
   /* op1: no. of 16 bit words of idle ptn
      op2: ptr to idle ptn */

#define  HAPI_V21_START_CMD         (HSF_MKCMD_STD(HAPIV21SRV, HAPIVMDMSTART))
   /* op1: reserved = 0
      op2: reserved = 0 */

#define  HAPI_V21_TXCFG_CMD         (HSF_MKCMD_STD(HAPIV21SRV, HAPIVMDMTXCFG))
   /* op1: see HAPI_V21_TXCFG below
      op2: reserved = 0 */

#define  HAPI_V21_TXDATA_CMD        (HSF_MKCMD_EXT(HAPIV21SRV, HAPIVMDMTXDATA))
   /* op1: no. of 16 bit words in buffer
      op2: ptr to buffer */

/***************************************************************************
**        SERVICE COMMAND RESPONSES
****************************************************************************/

#define HAPI_V21_GETREGS_RSP        (HSF_MKRSP_EXT(HAPI_V21_GETREGS_CMD))
#define HAPI_V21_GETSTATE_RSP       (HSF_MKRSP_EXT(HAPI_V21_GETSTATE_CMD))
#define HAPI_V21_GETSTATUS_RSP       (HSF_MKRSP_EXT(HAPI_V21_GETSTATUS_CMD))
#define  HAPIV21SETREG1    (( HAPIV21SRV << 8 ) | HAPIVMDMSETREG1 )

                                          /* V.21 MODEM SERVICE EVENTS */

/*
 * V.21 service specific events
 */
#define  HAPI_V21_DATASENT_EVT      (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMDATASENT))
   /* op1: no. of bits sent
      op2: reserved = 0 */

#define  HAPI_V21_TXDATASENT_EVT      (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMTXDATASENT))
   /* op1: no. of bits sent
      op2: reserved = 0 */


#define  HAPI_V21_DISCONNECT_EVT    (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMDISCONNECT))
   /* op1: TRUE
      op2: reserved = 0 */

#define  HAPI_V21_NOCARRIER_EVT     (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMNOCARRIER))
   /* op1: Rx power level
      op2: Rx power level threshold */

#define  HAPI_V21_RXCED_EVT         (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMRXCED))
   /* op1: 1
      op2: Rx power level */

#define  HAPI_V21_RXCNG_EVT         (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMRXCNG))
   /* op1: 1
      op2: Rx power level */

#define  HAPI_V21_RXDATA_EVT        (HSF_MKEVT_EXT(HAPIV21SRV, HAPIVMDMRXDATA))
   /* op1: no. of 16 bit words in buffer
      op2: ptr to buffer */

#define  HAPI_V21_TXDATAQHIGH_EVT   (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMTXCMDQHIGH))
   /* op1: reserved = no. of buffers in data queue
      op2: reserved = 0 */

#define  HAPI_V21_TXDATAQLOW_EVT    (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMTXCMDQLOW))
   /* op1: reserved = no. of buffers in data queue
      op2: reserved = 0 */

#define  HAPI_V21_TXDATAQUFLOW_EVT  (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMTXCMDQUFLOW))
   /* op1: reserved = 0
      op2: reserved = 0 */

#define  HAPI_V21_TXREADY_EVT       (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMTXREADY))
#define  HAPI_V21_BMDEBUG_EVT       (HSF_MKEVT_STD(HAPIV21SRV, HAPIVMDMBMDEBUG))
   /* op1: 1
      op2: connect speed */

                                          /* default threshold and transmit signal gains */
#define  HSV21TXAMPLITUDE        16384    /* -12 dB */
#define  HSV21CEDAMPLITUDE       16384    /* -12 dB */
#define  HSV21CNGAMPLITUDE       16384    /* -12 dB */
#define  HSV21CNGONDURATION      500      /* 500 ms (as per T.30) */
#define  HSV21CNGOFFDURATION     3000
#define  HSV21RXTHRESHOLD        100
#define  HSV21CEDTHRESHOLD       512
#define  HSV21CNGTHRESHOLD       512

                                          /* status word bit use */
#define  HSV21INITQUEUE          0x0001   /* bit 0 */
#define  HSV21ENABLE             0x0002   /* bit 1 */
#define  HSV21CEDON              0x0004   /* bit 2 CED on check */
#define  HSV21CNGON              0x0008   /* bit 3 CNG on check */
#define  HSV21TXENABLED          0x0010   /* bit 4 TX on check */


#define  HSV21RXBUFLEN           0x04     /* 1 + 16 words */

/* control code constants */

#define HSV21CMDQLEN         32
#define HSV21CMDQLOWTHRESH   8
#define HSV21CMDQHIGHTHRESH  (HSV21CMDQLEN - HSV21CMDQLOWTHRESH)


typedef enum                              /* transmitter states */
{
   HSV21TXIDLE = 0,
   HSV21TXCED,
   HSV21TXCNG,
   HSV21TXDATA

} HSZV21TXSTATE;

typedef enum
{
  HSV21ORIGINATE = 0,
  HSV21ANSWER,
  HSV21SIMPLEX

} HSZV21DIRECTION;

typedef enum
{
   HSV21SYNC = 0,
   HSV21ASYNC

} HSZV21MODE;

/*
 *  State structure (returned by HAPI_V21_GETSTATE_RSP)
 */
typedef struct
{
   HAPI_UINT16 status;
   HAPI_UINT16 reserved;

} HSZV21STATE;



typedef struct
{
   HAPI_UINT16       hsxtxamplitude;           /* modulation amplitude */
   HAPI_UINT16       hsxCEDamplitude;          /* CED tone amplitude */
   HAPI_UINT16       hsxCNGamplitude;          /* CNG tone amplitude */
   HAPI_UINT16       hsxCNGonduration;         /* CNG on duration */
   HAPI_UINT16       hsxCNGoffduration;        /* CNG off duration */
   HAPI_UINT16       hsxrxthreshold;           /* demod carrier detect threshold */
   HAPI_UINT16       hsxdettones;              /* TRUE to detect CNG and CED */
   HAPI_UINT16       hsxCEDthreshold;          /* CED detect threshold */
   HAPI_UINT16       hsxCNGthreshold;          /* CNG detect threshold */
   HAPI_UINT16       hsxCEDdetduration;        /* CED detect duration ms */
   HAPI_UINT16       hsxCNGdetduration;        /* CNG detect duration ms */
   HAPI_UINT16       hsxnumbits;               /* number of bits in rx packet */
   HAPI_UINT16       hsxdirection;        /* simplex, answer or call */
   HAPI_UINT16       hsxmode;                  /* sync or async */
   HAPI_UINT16       hsxFDansdur;              /* Full duplex answer tone duration */

   HAPI_UINT16       hsxcmdqhighthreshold;     /* command queue high threshold */
   HAPI_UINT16       hsxcmdqlowthreshold;      /* command queue low threshold */

}  HSZV21REGS;

/* default registers */

#define  HAPI_V21_DEF_REGS  \
{     \
   HSV21TXAMPLITUDE,                      /* DSPK generation */     \
   HSV21CEDAMPLITUDE,                     /* CED generation */     \
   HSV21CNGAMPLITUDE,     \
   HSV21CNGONDURATION,     \
   HSV21CNGOFFDURATION,                   /* CNG generation */     \
   HSV21RXTHRESHOLD,                      /* DPSK receiver threshold */     \
   1,                                     /* receiver detect tones */     \
   HSV21CEDTHRESHOLD,                     /* CED detection */     \
   HSV21CNGTHRESHOLD,                     /* CNG detection */     \
   500,                                   /* CED duration ms */     \
   400,                                   /* CNG duration ms (500ms - 15% - margin) */     \
   160,                                   /* 160 ms per Rx data pkt (48 bits) */     \
   HSV21SIMPLEX,                          /* simplex mode */   \
   HSV21SYNC,                             /* synchronous mode */   \
   3300,                                  /* 3.3 seconds answer tone duration */   \
   HSV21CMDQHIGHTHRESH,                   /* command queue high threshold */   \
   HSV21CMDQLOWTHRESH                     /* command queue low threshold */   \
}

/*
 *  Set this bit in hsxrxpktsize register if the receive packet size is
 *  specified in bits rather than ms
 */
#define HAPI_V21_RXPKTSIZE_IN_BITS HAPI_VMDM_RXPKTSIZE_IN_BITS


#endif   /* HAPI_V21_H */



