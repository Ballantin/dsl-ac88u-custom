/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
*      Generic opcodes.
*
****************************************************************************/

#ifndef HAPI_OPCODE_H
#define HAPI_OPCODE_H

/* ---- Include Files ---------------------------------------- */
#ifndef HAPI_TYPES_H
#include <hapi_types.h>                   /* Hausware environment definitions */
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* Hausware API Global definitions */
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>
#endif

/* ---- Constants and Types ---------------------------------- */
/*
** each opcode contains the following information:
**
**   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
** +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
** |Reserved |    |    |    |    |    |    |Std | Evt|    |    |    |    |    |    |
** +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
** |         |     Service ID              |  1 | 1/0| Message opcode(srv specific)|
** +---------+-----------------------------+---------------------------------------+
**
**  Std:  1 = Standard Fixed size message (op1 and op2 and values)
**        0 = Extended data message - see (op1 = length, op2 = *bufp )
**  Evt:  1 = Asynchronous Event
**        0 = Command/Response to a command
*/

/*
** 0x00-0x2f      Service-specific commands (overlapped with other services)
** 0x30-0x3f      Generic commands (non-overlapped)
** 0x40-0x6f      Service-specific events (overlapped with other services)
** 0x70-0x7f      Generic events (non-overlapped)
**
*/
/* Opcode Range Conventions */
#define HAPI_CMD_BASE            0x0000u
#define HAPI_GENERIC_CMD_BASE    0x0030u
#define HAPI_EVT_BASE            0x0040u        /* EVT or RSP types */
#define HAPI_GENERIC_EVT_BASE    0x0070u        /* EVT or RSP types */

#define HAPI_OPCODE_SRVID_MASK   0xff00u
#define HAPI_OPCODE_EVT_MASK     0x0040u
#define HAPI_OPCODE_STD_MASK     0x0080u
#define HAPI_OPCODE_EXT_MASK     0x0000u
#define HAPI_OPCODE_OPC_MASK     0x003fu

#define HAPI_OPCODE_IS_STD(header)     (header & HAPI_OPCODE_STD_MASK)
#define HAPI_OPCODE_IS_EXT(header)     (!(HAPI_OPCODE_IS_STD(header)))

#define HAPI_OPCODE_GET_OPC(header)    (header & HAPI_OPCODE_OPC_MASK)

/* Macros to put service id in upper byte with std/ext or unknown format */

/* Use this for generic cmds/evts - hap_codes.h specifies the STD/EXT bit. */
#define HSF_MKHDR(srvid, opcode)     ((srvid << 8) + opcode)

/* Use this for non-generic cmds/evts */
#define HSF_MKCMD_STD(srvid, opcode) (HAPI_OPCODE_STD_MASK + HSF_MKHDR(srvid, HAPI_CMD_BASE + opcode))
#define HSF_MKCMD_EXT(srvid, opcode) (HAPI_OPCODE_EXT_MASK + HSF_MKHDR(srvid, HAPI_CMD_BASE + opcode))
#define HSF_MKRSP_STD(command)       (command | HAPI_OPCODE_STD_MASK)
#define HSF_MKRSP_EXT(command)       (command &~ HAPI_OPCODE_STD_MASK)
#define HSF_MKEVT_STD(srvid, opcode) (HAPI_OPCODE_STD_MASK + HSF_MKHDR(srvid, HAPI_EVT_BASE + opcode))
#define HSF_MKEVT_EXT(srvid, opcode) (HAPI_OPCODE_EXT_MASK + HSF_MKHDR(srvid, HAPI_EVT_BASE + opcode))



/*
** Define a structure suitable for extracting srvid.
*/
typedef struct
{
   HAPI_UINT16 srvid;
   HAPI_UINT16 reserved; /* remove when compiler support 16-bit aligned structures */
   HAPI_UINT16 buf[1];
}
HAPI_SRVID_HDR;

/*
** Define a structure suitable for declaring REGS, STATS, or STATE prepended by srvid.
** Note that the reserved field is required because the compiler starts structures
** on 32-bit boundaries, and without the reserved field, a host which doesn't do
** the same will get misaligned.
** Example: HAPI_SRVID_STRUCT(PTE_REGS, regs)
*/
#define HAPI_SRVID_STRUCT(name, member) \
                                        typedef struct \
                                        { \
                                        HAPI_UINT16 srvid; \
                                        HAPI_UINT16 reserved; /* remove when compiler support 16-bit aligned structures */ \
                                        HAPI_##name member; \
                                        } \
                                        HAPI_##name##_SRVID;

/***************************************************************************
**        SERVICE COMMANDS
****************************************************************************/

#define HAPI_OPC_ENABLE_CMD          (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_CMD_BASE + 0)  /* enable/disable ecan */
      /* OP1: enable=1, disable=0 */
      /* OP2: reserved = 0 */

#define HAPI_OPC_GETREGS_CMD         (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_CMD_BASE + 1)
      /* OP1: reserved = 0 */
      /* OP2: reserved = 0 */

#define HAPI_OPC_SETREGS_CMD         (HAPI_OPCODE_EXT_MASK + HAPI_GENERIC_CMD_BASE + 2)
      /* OP1: length */
      /* OP2: regp */

#define HAPI_OPC_RESETREGS_CMD       (HAPI_OPCODE_EXT_MASK + HAPI_GENERIC_CMD_BASE + 3)
      /* OP1: length */
      /* OP2: regp */

#define HAPI_OPC_SETREG1_CMD         (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_CMD_BASE + 4)
      /* OP1: offset  i.e. use offsetof(STRUCT,member) macro */
      /* OP2: value (HAPI_UINT16 only) */

#define HAPI_OPC_GETSTATS_CMD        (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_CMD_BASE + 5)
      /* OP1: 0 = accumulate ( don't clear next frame time) */
      /*      1 = clear stats next frame time */
      /* OP2: reserved = 0 */

#define HAPI_OPC_GETSTATE_CMD        (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_CMD_BASE + 6)
      /* OP1: reserved = 0 */
      /* OP2: reserved = 0 */
/*
 * NET is telling service that a register has changed and must act accordingly.
 * e.g. error checking, reconfiguration.
 */
#define HAPI_OPC_REG_UPDATE_CMD      (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_CMD_BASE + 7)
      /* OP1: enable=1, disable=0 */
      /* OP2: reserved = 0 */

/*
 * NET is telling service to reset its statistics.
 */
#define HAPI_OPC_RESET_STATS_CMD     (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_CMD_BASE + 8)
      /* OP1: reserved = 0 */
      /* OP2: reserved = 0 */

#define HAPI_OPC_SYNC_UP_CMD         (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_CMD_BASE + 9)
      /* OP1: vhdhandle */
      /* OP2: reserved = 0 */

#define HAPI_OPC_STACK_SWITCH_CMD    (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_CMD_BASE + 10)
      /* OP1: vhdhandle */
      /* OP2: reserved = 0 */

/***************************************************************************
**        SERVICE COMMAND RESPONSES
****************************************************************************/

#define HAPI_OPC_GETREGS_RSP     (HSF_MKRSP_EXT(HAPI_OPC_GETREGS_CMD))
      /* OP1: length */
      /* OP2: regp */

#define HAPI_OPC_GETSTATS_RSP    (HSF_MKRSP_EXT(HAPI_OPC_GETSTATS_CMD))
      /* OP1: length */
      /* OP2: statsp */

#define HAPI_OPC_GETSTATE_RSP    (HSF_MKRSP_EXT(HAPI_OPC_GETSTATE_CMD))
      /* OP1: length */
      /* OP2: statep */

#define HAPI_OPC_SYNC_UP_EVT      (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_EVT_BASE + 0)
      /* OP1: vhdhandle */
      /* OP2: reserved = 0 */
#define HAPI_OPC_STACK_SWITCH_EVT (HAPI_OPCODE_STD_MASK + HAPI_GENERIC_EVT_BASE + 1)
      /* OP1: vhdhandle */
      /* OP2: reserved = 0 */
/* ---- Variable Externs ------------------------------------- */
/* ---- Function Prototypes ---------------------------------- */

#endif  /* HAPI_OPCODE_H */
