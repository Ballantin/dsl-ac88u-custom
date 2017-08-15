/****************************************************************************
*
*     Copyright (c) 2005 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  hapi_agc.h
*
*  PURPOSE:
*     This file contains the AGC Service API definitions.
*
***************************************************************************/
#ifndef HAPI_AGC_H
#define HAPI_AGC_H

/* ---- Include Files ---------------------------------------------------- */
#ifndef HAPI_TYPES_H
#include <hapi_types.h>                /* HausWare environment definitions */
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* HausWare API Global definitions  */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* HausWare generic cmds/events     */
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* HausWare API IDs                 */
#endif
/* ---- Constants and Types ---------------------------------------------- */

/*
* AGC service register structure 
* Note that the reserved components are there for future expansion
*/
typedef struct HAPI_AGC_REGS
{
   HAPI_SINT16   options;
   HAPI_SINT16   eventMask;
   HAPI_UINT16   eventInterval;
   HAPI_SINT16   gaindB;
   HAPI_SINT16   targetLeveldB;
   HAPI_SINT16   speedUpdB;
   HAPI_SINT16   speedDowndB;
   HAPI_SINT16   gainMaxdB;
   HAPI_SINT16   gainMindB;
   HAPI_SINT16   peakLeveldB;
   HAPI_SINT16   peakSpeeddB;
   HAPI_SINT16   peakStatLeveldB;
   HAPI_SINT16   reserved1;
} HAPI_AGC_REGS;


#define AGC_OPTION_ENABLE     0x0001  /* enable AGC */
#define AGC_OPTION_FREEZE     0x0002  /* FREEZE adaptation */
#define AGC_OPTION_RESERVED   0x0004  /* do not use, reserved for internal use */
#define AGC_OPTION_TONEBYPASS 0x0008  /* disable AGC gain when tones are present */

#define AGC_EVENT_STATIONARY     0x0001
#define AGC_EVENT_FINEGRAINED    0x0002
#define AGC_EVENT_PERIODIC_STATE 0x8000
#define AGC_EVENT_PERIODIC_STATS 0x4000

/****************************************************************************
* COMMANDS                                                              *
****************************************************************************/
#define HAPI_AGC_ENABLE_CMD (HSF_MKHDR(HAPI_AGC_SRV, HAPI_OPC_ENABLE_CMD))
   /* op1 : true to enable the AGC service, false to disable */
   /* op2 : reserved */

#define HAPI_AGC_SETREGS_CMD (HSF_MKHDR(HAPI_AGC_SRV, HAPI_OPC_SETREGS_CMD))
   /* OP1: length of the HAPI_AGC_REGS structure   */
   /* OP2: pointer to the HAPI_AGC_REGS structure  */

#define HAPI_AGC_SETREG1_CMD (HSF_MKHDR(HAPI_AGC_SRV, HAPI_OPC_SETREG1_CMD))
   /* OP1: index to the HAPI_AGC_REGS structure */
   /* OP2: new value of the register  component */

#define HAPI_AGC_GETREGS_CMD (HSF_MKHDR(HAPI_AGC_SRV, HAPI_OPC_GETREGS_CMD))
   /* OP1: reserved */
   /* OP2: reserved */
 
#define HAPI_AGC_GETSTATE_CMD (HSF_MKHDR(HAPI_AGC_SRV, HAPI_OPC_GETSTATE_CMD))
   /* OP1: reserved */
   /* OP2: reserved */

#define HAPI_AGC_GETSTATS_CMD (HSF_MKHDR(HAPI_AGC_SRV, HAPI_OPC_GETSTATS_CMD))
   /* OP1: reserved */
   /* OP2: reserved */

#define HAPI_AGC_FREEZE_CMD  (HSF_MKCMD_STD(HAPI_AGC_SRV, 0))
   /* op1 : true to freeze the AGC service, false to unfreeze */
   /* op2 : reserved */

#define HAPI_AGC_SET_EVENT_CMD (HSF_MKCMD_STD(HAPI_AGC_SRV, 1))
   /* op1 : event mask */
   /* op2 : reserved */


/****************************************************************************
* EVENTS                                                                    *
****************************************************************************/
#define HAPI_AGC_STATE_EVT   (HSF_MKEVT_STD(HAPI_AGC_SRV, 0))
   /* op1 : volAuto */
   /* op2 : p.56 estimate average */

#define HAPI_AGC_STATS_EVT (HSF_MKEVT_EXT(HAPI_AGC_SRV, 1))
   /* OP1: length of state structure in bytes */
   /* OP2: pointer to the state structure */

#define HAPI_AGC_GETREGS_EVT (HSF_MKEVT_EXT(HAPI_AGC_SRV, 2))
   /* OP1: length of register structure in bytes */
   /* OP2: pointer to the register structure */

#define HAPI_AGC_STATIONARY_EVT (HSF_MKEVT_STD(HAPI_AGC_SRV, 3))
   /* OP1 */
   /* OP2 */

/* dB conversion (Q8) */
#ifndef Q_DB
#define Q_DB                    256
#endif

#define HAPI_AGC_DEFAULT_OPTION     0
#define HAPI_AGC_EVENT_DEFAULT      0
#define HAPI_AGC_DEFAULT_GAIN       0
#define HAPI_AGC_TARGET_DB          (-22*Q_DB)
#define HAPI_AGC_SPEEDUP            (8*Q_DB)
#define HAPI_AGC_SPEEDDOWN          (32*Q_DB)
#define HAPI_AGC_GAINMAX            (18*Q_DB)
#define HAPI_AGC_GAINMIN            (-18*Q_DB)
#define HAPI_AGC_PEAKLVL            (6*Q_DB)
#define HAPI_AGC_PEAKSPEED          (8*Q_DB)
#define HAPI_AGC_PEAKSTATLVL        (6*Q_DB)
#define HAPI_AGC_RESERVED           0

#define AGC_DEF_REGS \
{                             \
   HAPI_AGC_DEFAULT_OPTION,   \
   HAPI_AGC_EVENT_DEFAULT,    \
   0,                         \
   HAPI_AGC_DEFAULT_GAIN,     \
   HAPI_AGC_TARGET_DB,        \
   HAPI_AGC_SPEEDUP,          \
   HAPI_AGC_SPEEDDOWN,        \
   HAPI_AGC_GAINMAX,          \
   HAPI_AGC_GAINMIN,          \
   HAPI_AGC_PEAKLVL,          \
   HAPI_AGC_PEAKSPEED,        \
   HAPI_AGC_PEAKSTATLVL,      \
   HAPI_AGC_RESERVED          \
}

#endif
