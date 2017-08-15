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
*      Call Monitoring Tone Detection Service Hausware API (HAPI).
*
****************************************************************************/

#ifndef HAPI_CMTD_H                        /* support nested includes */
#define HAPI_CMTD_H

/* ---- Include Files ---------------------------------------- */
#ifndef HAPI_TYPES_H
#include <hapi_types.h>                /* Hausware type definitions */
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
#define HAPI_CMTD_SRV_NAME "CMTD"

/* ---- Constants and Types ---------------------------------- */

/***************************************************************************
**        SERVICE COMMANDS
****************************************************************************/

/* Generic commands and events have the STD/EXT bit set in the HAPI_OPC_xxx define */
/* Also they don't require descriptions of OP1 and OP2 - see hapi_opcode.h for these */
#define HAPI_CMTD_ENABLE_CMD     (HSF_MKHDR(HAPI_CMTD_SRV, HAPI_OPC_ENABLE_CMD))

#define HAPI_CMTD_ENABLE_OP1_ENA_SRV_BIT      0x0001  /* use this in OP1 to indicate the service is enabled */
#define HAPI_CMTD_ENABLE_OP1_ENA_A_FILTER_BIT 0x0010  /* use this in OP1 to enable a single filter */
#define HAPI_CMTD_ENABLE_OP1_ENA_A_STM_BIT    0x0020  /* use this in OP1 to enable a single STM */
#define HAPI_CMTD_ENABLE_OP1_ENA_FILTERS_BIT  0x0040  /* use this in OP1 to enable a set of filters */
#define HAPI_CMTD_ENABLE_OP1_ENA_STMS_BIT     0x0080  /* use this in OP1 to enable a set of STMS */
#define HAPI_CMTD_ENABLE_OP1_DIS_A_FILTER_BIT 0x0100  /* use this in OP1 to disable a single filter */
#define HAPI_CMTD_ENABLE_OP1_DIS_A_STM_BIT    0x0200  /* use this in OP1 to disable a single STM */
#define HAPI_CMTD_ENABLE_OP1_DIS_FILTERS_BIT  0x0400  /* use this in OP1 to disable a set of filters */
#define HAPI_CMTD_ENABLE_OP1_DIS_STMS_BIT     0x0800  /* use this in OP1 to disable a set of STMS */
#define HAPI_CMTD_ENABLE_OP1_DIS_SRV_BIT      0x1000  /* use this in OP1 to indicate the service is disabled */

#define HAPI_CMTD_ENABLE_OP2_FILTER_BITMASK   0x00ff
#define HAPI_CMTD_ENABLE_OP2_STM_BITMASK      0xff00
#define HAPI_CMTD_ENABLE_OP2_STM_BITSHIFT     8

/*
** Configure a specific filter in the service.
*/
#define HAPI_CMTD_SETFILTER_CMD  (HSF_MKCMD_EXT(HAPI_CMTD_SRV, 0))
      /* OP1: length */
      /* OP2: (HAPI_CMTD_FILTER *) */

/*
** Configure a specific state machine in the service.
*/
#define HAPI_CMTD_SETSTM_CMD     (HSF_MKCMD_EXT(HAPI_CMTD_SRV, 1))
      /* OP1: length */
      /* OP2: (HAPI_CMTD_STM *) */

/*
** Request estimate of power occupying filter frequencies
*/
#define HAPI_CMTD_GETPOWER_CMD   (HSF_MKCMD_EXT(HAPI_CMTD_SRV, 2))
      /* OP1: length */
      /* OP2: (HAPI_CMTD_FILTER_POWER *) */

/***************************************************************************
**        SERVICE COMMAND RESPONSES
****************************************************************************/
#define HAPI_CMTD_GETPOWER_RSP     (HSF_MKRSP_EXT(HAPI_CMTD_GETPOWER_CMD))


/***************************************************************************
**        SERVICE ASYNCHRONOUS EVENTS
****************************************************************************/

/* Non-generic events require specifying HSF_MKHDR_STD or HSF_MKHDR_EXT */

/* Call Monitoring Tone Detection Service event detected - fixed format */
#define HAPI_CMTD_TONE_DET_EVT   (HSF_MKEVT_STD(HAPI_CMTD_SRV, 0))
#define HAPI_CMTD_TONE_END_EVT   (HSF_MKEVT_STD(HAPI_CMTD_SRV, 1))

/***************************************************************************
**        SERVICE API STRUCTURES AND ASSOCIATED DEFINES
****************************************************************************/

/* Used to specify a state can last forever by setting the maxTimeMs field
 * to HAPI_CMTD_MAX_TIME_FOREVER
 */
#define HAPI_CMTD_MAX_TIME_FOREVER	0xFFFF

typedef struct
{
   HAPI_UINT16 minTimeMs;
   HAPI_UINT16 maxTimeMs;
   HAPI_UINT16 activeFilter;
   HAPI_UINT16 reserved;

} HAPI_CMTD_SUBSTATEPARAMS;

#define HAPI_CMTD_NUM_SUBSTATES 6

typedef struct
{
   HAPI_CMTD_SUBSTATEPARAMS substate[HAPI_CMTD_NUM_SUBSTATES];

} HAPI_CMTD_STMPARAMS;

typedef struct
{
   HAPI_UINT16 index;
   HAPI_UINT16 reserved;
   HAPI_CMTD_STMPARAMS stmParams;

} HAPI_CMTD_STM;

#define HAPI_CMTD_FILTER_NUM_TAPS 79
typedef struct
{
   HAPI_UINT16 type;
   HAPI_UINT16 minAttenuation;
   HAPI_SINT16 filterTaps[HAPI_CMTD_FILTER_NUM_TAPS];

} HAPI_CMTD_FILTERCOEFS;

typedef struct
{
   HAPI_UINT16 index;
   HAPI_UINT16 reserved;
   HAPI_CMTD_FILTERCOEFS coefs;
} HAPI_CMTD_FILTER;

typedef struct
{
   HAPI_UINT32 power;
} HAPI_CMTD_FILTER_POWER;

#endif      /* HAPI_CMTD_H */
