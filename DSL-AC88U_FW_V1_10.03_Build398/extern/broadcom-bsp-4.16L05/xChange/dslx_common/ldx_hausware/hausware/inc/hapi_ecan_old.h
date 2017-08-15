/****************************************************************************
*
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*           Broadcom
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  hapi_ecan_old.h
*
*  PURPOSE:
*
*     This file implements the ECAN API definitions.
*
*  NOTES:
*
****************************************************************************/
#ifndef HAPI_ECAN_OLD_H                          /* support nested includes */
#define HAPI_ECAN_OLD_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>                /* Hausware environment definitions */
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

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
*  DEPENDENCIES
****************************************************************************/
/*
 * Generic commands:
 *    - have the STD/EXT bit set in the HAPI_OPC_xxx define
 *    - don't require descriptions of op1 and op2 - see hapi_opcode.h for these
 */
#define  HAPI_ECAN_ENABLE_CMD        (HSF_MKHDR(HAPI_ECAN_SRV, HAPI_OPC_ENABLE_CMD))

/*
 * Ecan service specific commands
 */

#define  HAPI_ECAN_DISABLE_CMD        (HSF_MKCMD_STD(HAPI_ECAN_SRV, 0))
#define  HAPI_ECAN_ADAPT_CMD          (HSF_MKCMD_STD(HAPI_ECAN_SRV, 2))
#define  HAPI_ECAN_NLP_CMD            (HSF_MKCMD_STD(HAPI_ECAN_SRV, 3))
#define  HAPI_ECAN_RESETCOEFF_CMD     (HSF_MKCMD_STD(HAPI_ECAN_SRV, 4))
#define  HAPI_ECAN_SETMINERL_CMD      (HSF_MKCMD_STD(HAPI_ECAN_SRV, 8))
#define  HAPI_ECAN_NOISEGEN_CMD       (HSF_MKCMD_STD(HAPI_ECAN_SRV, 9))
#define  HAPI_ECAN_SETMAXERL_CMD      (HSF_MKCMD_STD(HAPI_ECAN_SRV, 10))

/*
 * Ecan service specific events
 */
#define  HAPI_ECAN_STATE_EVT      (HSF_MKEVT_STD(HAPI_ECAN_SRV, 0))
   /* op1: no. of bits sent
      op2: reserved = 0 */


#define HAPI_ECAN_GETSTATS_EVT         (HSF_MKEVT_EXT(HAPI_ECAN_SRV, 5))
   /* op1: size of structure in bytes */
   /* op2:  hszecanstats structure */


typedef struct HSZECANSTATS
{
   HAPI_UINT16 ltRxPowerErlLo;                 /* rx power lo */
   HAPI_UINT16 ltRxPowerErlHi;                 /* rx power hi */
   HAPI_UINT16 ltTxPowerErlLo;                 /* tx power lo*/
   HAPI_UINT16 ltTxPowerErlHi;                 /* tx power hi */
   HAPI_UINT16 errorPowerpMinLo;               /* error power lo */
   HAPI_UINT16 errorPowerpMinHi;               /* error power hi */
   HAPI_UINT16 ecanClientOptions;              /* NLPEnable, AdaptEnable, Bypass, bit values */
   HAPI_UINT16 erl;                            /* Echo Return Loss */
   HAPI_UINT16 erle;                           /* Echo Return Loss Enhanced */
   HAPI_UINT16 minimumErl;                     /* Minimum ERL value */
   HAPI_UINT16 hangCountOn;                    /* the hang count before NLP is turned on */
   HAPI_UINT16 hangCountOff;                   /* the hang count before NLP is turned off */
   HAPI_UINT16 adaptStatus;                    /* Indicates whether ECAN is currently adapting */
   HAPI_UINT16 adaptCount;                     /* adaptation count */
   HAPI_UINT16 hangCountNear;                  /* hang counter when near end is detected */
   HAPI_UINT16 combinedLossNLP;                /* the combined loss before NLP is applied */
   HAPI_SINT16 nlpBurstCount;                  /* counter for NLP (increments when on, decrements when off) */
   HAPI_SINT16 noiseAdjust;                    /* adjustment applied to CNG power */
   HAPI_UINT16 maximumErl;                     /* Maximum ERL value */

} HSZECANSTATS;


/* GENERIC ECAN COMMANDS */
#define  HAPIECAN_ENABLE     HAPI_ECAN_ENABLE_CMD
#define  HAPIECAN_DISABLE    HAPI_ECAN_DISABLE_CMD


/* HOTHAUS ECAN SPECIFIC COMMANDS */
#define  HAPIECAN_ADAPT      HAPI_ECAN_ADAPT_CMD
#define  HAPIECAN_NLP        HAPI_ECAN_NLP_CMD
#define  HAPIECAN_RESETCOEFF HAPI_ECAN_RESETCOEFF_CMD
#define  HAPIECAN_SETMINERL  HAPI_ECAN_SETMINERL_CMD
/* ECAN EVENTS SENT TO RESOURCE MANAGER */
#define HAPIECAN_STATE       HAPI_ECAN_STATE_EVT   /* ECAN state of pxd */

#ifdef __cplusplus
}
#endif

#endif /* HAPI_ECAN_OLD_H */
