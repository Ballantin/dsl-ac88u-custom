/****************************************************************************
 *
 *  HAUSWARE Definitions for Call Discriminator
 *
 *  PURPOSE:
 *     This file contains the API definitions for the Call Discriminator
 *
 *           (c) Copyright 1995-1999, HotHaus Technologies Inc.
 *                          All rights reserved
 *
 ****************************************************************************/

#ifndef HAPI_DTAS_H                        /* support nested includes */
#define HAPI_DTAS_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware service ids */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* Hausware generic cmds/events */
#endif

#define HAPIDTASSRV                    HAPI_DTAS_SRV

/* status register mask */
#define DTAS_ENABLED         0x1     /* DTAS Enabled */

/***************************************************************************
**        DTAS SERVICE COMMANDS
****************************************************************************/

#define HAPI_DTAS_ENABLE_CMD            (HSF_MKHDR(HAPIDTASSRV, HAPI_OPC_ENABLE_CMD))  /* enable */
      /* OP1: 1=ENABLE, 0=DISABLE */
      /* OP2: reserved = 0        */

/***************************************************************************
**        DTAS SERVICE ASYNCHRONOUS EVENTS
****************************************************************************/

#define HAPI_DTAS_DETECT_EVT            (HSF_MKEVT_STD(HAPIDTASSRV, 0))  /* DTAS detected */
      /* OP1: DTAS Detector Mask */
      /* OP2: 1 = ON, 0 = OFF */

/* Mapping from new to old defines */
/* This is to minimize the code changes */

#define HAPI_DTAS_ENABLE              HAPI_DTAS_ENABLE_CMD

#define HAPI_DTAS_DETECT              HAPI_DTAS_DETECT_EVT


/* status register mask */
#define DTAS_ENABLED         0x1     /* CDIS Enabled */

#endif      /* HAPI_CDIS_H */


