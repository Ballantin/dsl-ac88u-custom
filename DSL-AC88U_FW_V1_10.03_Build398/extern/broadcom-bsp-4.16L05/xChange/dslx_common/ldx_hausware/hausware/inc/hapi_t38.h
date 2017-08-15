/***************************************************************************
*
* Copyright (c) 2002 Broadcom, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom
* company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
*     T.38 service public API.
*
****************************************************************************/

#ifndef HAPI_T38_H                           /* support nested includes */
#define HAPI_T38_H

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

/* Service name, version */
#define HAPI_T38_SRV_NAME "T38"

/***************************************************************************
**        SERVICE COMMANDS
****************************************************************************/

/*
 * Generic commands:
 *    - have the STD/EXT bit set in the HAPI_OPC_xxx define
 *    - don't require descriptions of op1 and op2 - see hapi_opcode.h for these
 */
#define  HAPI_T38_ENABLE_CMD        (HSF_MKHDR(HAPI_T38_SRV, HAPI_OPC_ENABLE_CMD))
#define  HAPI_T38_SETREGS_CMD       (HSF_MKHDR(HAPI_T38_SRV, HAPI_OPC_SETREGS_CMD))
#define  HAPI_T38_SETREG1_CMD       (HSF_MKHDR(HAPI_T38_SRV, HAPI_OPC_SETREG1_CMD))
#define  HAPI_T38_GETREGS_CMD       (HSF_MKHDR(HAPI_T38_SRV, HAPI_OPC_GETREGS_CMD))
#define  HAPI_T38_GETSTATE_CMD      (HSF_MKHDR(HAPI_T38_SRV, HAPI_OPC_GETSTATE_CMD))
#define  HAPI_T38_GETSTATS_CMD      (HSF_MKHDR(HAPI_T38_SRV, HAPI_OPC_GETSTATS_CMD))
#define  HAPI_T38_REG_UPDATE_CMD    (HSF_MKHDR(HAPI_T38_SRV, HAPI_OPC_REG_UPDATE_CMD))
#define  HAPI_T38_RESET_STATS_CMD   (HSF_MKHDR(HAPI_T38_SRV, HAPI_OPC_RESET_STATS_CMD))

#define  HAPI_T38_SHUTDOWN_CMD      (HSF_MKCMD_STD(HAPI_T38_SRV, 0))
#define  HAPI_T38_EGRESSPACKET_CMD  (HSF_MKCMD_EXT(HAPI_T38_SRV, 1)) /* incoming network packet */

#define HAPIT38_EGRESSPACKET  HAPI_T38_EGRESSPACKET_CMD
/***************************************************************************
**        SERVICE COMMAND RESPONSES
****************************************************************************/

#define HAPI_T38_GETREGS_RSP        (HSF_MKRSP_EXT(HAPI_T38_GETREGS_CMD))
#define HAPI_T38_GETSTATE_RSP       (HSF_MKRSP_EXT(HAPI_T38_GETSTATE_CMD))
#define HAPI_T38_GETSTATS_RSP       (HSF_MKRSP_EXT(HAPI_T38_GETSTATS_CMD))

/***************************************************************************
**        SERVICE ASYNCHRONOUS EVENTS
****************************************************************************/

/* Generic events:
 *    - have the STD/EXT bit set in the HAPI_OPC_xxx define
 *    - don't require descriptions of op1 and op2 - see hapi_opcode.h for these
 */

#define HAPI_T38_SHUTDOWN_EVT       (HSF_MKEVT_STD(HAPI_T38_SRV, 0))
#define HAPI_T38_INGRESSPACKET_EVT  (HSF_MKEVT_EXT(HAPI_T38_SRV, 1))  /* outgoing network packet */
#define HAPI_T38_BMDEBUG_EVT        (HSF_MKEVT_STD(HAPI_T38_SRV, 2))  /* outgoing network packet */


#define HAPIT38_INGRESSPACKET  HAPI_T38_INGRESSPACKET_EVT

/*
 * T.38 service events
 */

/*
 *  State structure (returned by HAPI_T38_GETSTATE_RSP)
 */
typedef struct
{
   HAPI_UINT16 status;
   HAPI_UINT16 reserved;

} HAPI_T38_STATE;

/*
 * T.38 status bit definitions
 */
typedef enum
{
   HAPI_T38_ENABLE = 0x0001

} HAPI_T38_STATUS;

/*
 * T.38 registers
 */
typedef  struct
{
   HAPI_UINT16 numRedundantIFP;
   HAPI_UINT16 reorderDelay;

} HAPI_T38_REGS;

/*
 * Default T.38 Register settings.  TODO - put realistic values
 * Host: declare HAPI_T38_REGS t38Regs = HAPI_T38_DEF_REGS;
 * Usage: hapiSetReg1(handle, &t38Regs, HAPI_T38_SETREG1_CMD,
 *                    OFFSETOF(HAPI_T38_REGS, numRedundantIFP), 0);
 * hapiSetReg1 modifies local copy to keep register copy consistent.
 */
#define HAPI_T38_DEF_REGS                                              \
{                                                                      \
   0x0303,                      /* number of redundant packets */      \
   200,                         /* Maximum reodering delay (ms) */     \
}

/*
 * Statistics structures
 */

/* Ingress stats */
typedef struct
{
   HAPI_UINT16    frameCountHigh;        /* 16 MSBs of number of ingress packets sent */
   HAPI_UINT16    frameCountLow;         /* 16 LSBs of number of ingress packets sent */
   HAPI_UINT16    unknownPkt;            /* number of HAPI_PKT_FAXRELAY pkts not recognized by T.38 */
   HAPI_UINT16    bufferPoolDry;         /* number of failed attempts at bmalloc() - includes all buffer pools */

} HAPI_T38_INGRESS_STATS;


/* Egress stats */
typedef struct
{
   HAPI_UINT16    frameCountHigh;        /* 16 MSBs of number of egress packets received */
   HAPI_UINT16    frameCountLow;         /* 16 LSBs of number of egress packets received */
   HAPI_UINT16    reorderCount;          /* number of IFP pkts reordered or recovered */
   HAPI_UINT16    failtoReorderCount;    /* number of IFP pkts not reordered or recovered */
   HAPI_UINT16    invalidIFPCount;       /* number of IFP pkts received that did not pass sanity check */
   HAPI_UINT16    bufferPoolDry;         /* number of failed attempts at bmalloc() - includes all buffer pools  */

} HAPI_T38_EGRESS_STATS;

/* Stats */
typedef struct
{
   HAPI_T38_INGRESS_STATS ingressStats;
   HAPI_T38_EGRESS_STATS  egressStats;

} HAPI_T38_STATS;

/*
** Define 3 structures, which are simply each of the REGS, STATE, and STATS
** structures, prefixed with a srvid field. This is done to support the
** net service holding the slave services' data for mode switching and
** maintaining a consistent API across all modes.
*/
HAPI_SRVID_STRUCT(T38_REGS, regs)
HAPI_SRVID_STRUCT(T38_STATE, state)
HAPI_SRVID_STRUCT(T38_STATS, stats)

/*
** Define a single amalgamated structure of T38 DATA that is
** stored in the supervisor. A single structure simplifies the
** mechanism used to set and retrieve pointers to here.
*/
typedef struct
{
   HAPI_T38_REGS_SRVID t38Regs;
   HAPI_T38_STATE_SRVID t38State;
   HAPI_T38_STATS_SRVID t38Stats;
}
HAPI_T38_DATA;

#endif /* HAPI_T38_H */

