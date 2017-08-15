/****************************************************************************
*
*  HAPI_CLIDTX.H - HAUSWARE Definitions for the Class Relay Modem service
*
*  PURPOSE:
*
*     This file contains the Caller ID (Class) TX Modem API definitions.
*
**********************************************************************/

#ifndef  HAPI_CLIDTX_H
#define  HAPI_CLIDTX_H

#ifndef HAPI_TYPES_H
#include  <hapi_types.h>            /* Hausware environment */
#endif
#ifndef HAPI_NET_H
#include <hapi_net.h>
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware service ids */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* Hausware generic cmds/events */
#endif

#define CLIDTXDEBUG           0  /* Debugging mode for CLIDTX */


#define HAPICLIDTXSRV         HAPI_CLIDTX_SRV

/***************************************************************************
**        SERVICE COMMANDS
****************************************************************************/

/*
 * Generic commands:
 *    - have the STD/EXT bit set in the HAPI_OPC_xxx define
 *    - don't require descriptions of op1 and op2 - see hapi_opcode.h for these
 */
#define  HAPI_CLIDTX_ENABLE_CMD        (HSF_MKHDR(HAPICLIDTXSRV, HAPI_OPC_ENABLE_CMD))
#define  HAPI_CLIDTX_SETREGS_CMD       (HSF_MKHDR(HAPICLIDTXSRV, HAPI_OPC_SETREGS_CMD))
#define  HAPI_CLIDTX_SETREG1_CMD       (HSF_MKHDR(HAPICLIDTXSRV, HAPI_OPC_SETREG1_CMD))


/*
 * clid tx service specific commands
 */
#define  HAPI_CLIDTX_START_CMD      (HSF_MKCMD_STD(HAPICLIDTXSRV, 0))  /* start tx'ing class data,           */
                                                                    /* after counting down delay reg      */

/***************************************************************************
**        SERVICE ASYNCHRONOUS EVENTS
****************************************************************************/

#define HAPI_CLIDTX_DONE_EVT       (HSF_MKEVT_STD(HAPICLIDTXSRV, 0))/* Class message has been sent    */
#define HAPI_CLIDTX_MSG_ERROR_EVT  (HSF_MKEVT_STD(HAPICLIDTXSRV, 1))/* CLID not ready for new message */


#define HAPI_CLIDTX_ENABLE         HAPI_CLIDTX_ENABLE_CMD     /* enable disable modem               */
#define HAPI_CLIDTX_SETREGS        HAPI_CLIDTX_SETREGS_CMD    /* set the tx modem registers         */
#define HAPI_CLIDTX_SETREGS1       HAPI_CLIDTX_SETREG1_CMD   /* set the tx modem registers         */

#define HAPI_CLIDTX_START          HAPI_CLIDTX_START_CMD     /* start tx'ing class data,           */
                                                        /* after counting down delay reg      */

#define HAPI_CLIDTX_DONE           HAPI_CLIDTX_DONE_EVT


/* TX register set */
typedef struct
{
   HAPI_UINT16   delay;               /* Delay (ms) before sending Class message  */
   HAPI_UINT16   Amplitude;           /* Amplitude of Mark signal (0dBm = 22656)  */
   HAPI_UINT16   MarkFrequency;       /* Mark Frequency (Hz)                      */
   HAPI_UINT16   SpaceFrequency;      /* Space Frequency (Hz)                     */
#if CLIDTXDEBUG || (defined MAKEDEFS)
/* Debug TX Register Set */
   HAPI_UINT16   SpaceAmplitude;      /* Amplitude of Class signal (0dBm = 22656) */
#endif

}  HSZCLIDTXREGS;


/* Default register settings */
#define  CLIDTX_DELAY       500  /* 500 ms                                     */
#define  CLIDTX_AMP         4788  /* Mark Amplitude (-13.5 dBm)  (22656 = 0dBm) */
#define  CLIDTX_MARK_FREQ   1200  /* Mark Frequency 1200  Hz                    */
#define  CLIDTX_SPACE_FREQ  2200  /* Space Frequency 2200 Hz                    */
#define  CLIDTX_SPACE_AMP   4788  /* Space Amplitude (-13.5 dBm) (22656 = 0dBm) */

/* Max. message packet size */
#define CLIDTX_MESSAGESIZE    21  /* 21 words can be contained in CLASS message */

/* NET Class Relay Packet Structure */
typedef struct
{
   HAPI_PKT_EGRESS_HDR   hsxPacketHdr;     /* Standard NetVHD Header */
   HAPI_SINT16            hsxWord0;         /* siezecnt/msglen */
   HAPI_UINT16            hsxWord1;         /* markcnt */
   HAPI_UINT16            hsxClassMsg[CLIDTX_MESSAGESIZE];

} HSZCLASSRELAYPACKET;

#define HAPI_CLIDTX_DEF_REGS   \
{                              \
   CLIDTX_DELAY,               \
   CLIDTX_AMP,                 \
   CLIDTX_MARK_FREQ,           \
   CLIDTX_SPACE_FREQ           \
}

#endif

