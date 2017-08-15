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

#ifndef HAPI_CDIS_H                        /* support nested includes */
#define HAPI_CDIS_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware service ids */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* Hausware generic cmds/events */
#endif

#define HAPICDISSRV                    HAPI_CDIS_SRV

/***************************************************************************
**        CDIS SERVICE COMMANDS
****************************************************************************/

#define HAPI_CDIS_ENABLE_CMD            (HSF_MKHDR(HAPICDISSRV, HAPI_OPC_ENABLE_CMD))  /* enable */
      /* OP1: 1=ENABLE, 0=DISABLE */
      /* OP2: reserved = 0        */

#define HAPI_CDIS_V21ENABLE_CMD         (HSF_MKCMD_STD(HAPICDISSRV, 0))  /* V.21 HDLC detect enable */
      /* OP1: 1=ENABLE, 0=DISABLE */
      /* OP2: reserved = 0        */

#define HAPI_CDIS_EXTLOWENERGYDET_CMD  (HSF_MKCMD_STD(HAPICDISSRV, 1))  /* Extended Low Energy detect enable */
      /* OP1: 1=ENABLE, 0=DISABLE */
      /* OP2: reserved = 0        */

#define HAPI_CDIS_CONFIG_CMD           (HSF_MKCMD_STD(HAPICDISSRV, 2))
      /* OP1 : register setting */
      /* OP2 : reserved = 0     */

/*
** Configure a custom tone  filter in the service.
*/
#define HAPI_CDIS_SETCUSTOMFILTER_CMD  (HSF_MKCMD_EXT(HAPICDISSRV, 3))
      /* OP1: length */
      /* OP2: (HAPI_CDIS_CUSTOMTONE_FILTER *) */


/***************************************************************************
**        CDIS SERVICE ASYNCHRONOUS EVENTS
****************************************************************************/

#define HAPI_CDIS_DETECT_EVT            (HSF_MKEVT_STD(HAPICDISSRV, 0))  /* fax/modem detected */
      /* OP1: CDIS Detector Mask */
      /* OP2: 1 = ON, 0 = OFF */

#define HAPI_CDIS_HDLCDETECT_EVT        (HSF_MKEVT_STD(HAPICDISSRV, 1))   /* HDLCs detected in ingress
                                               direction */
      /* OP1: Reserved = 0 */
      /* OP2: Reserved = 0 */

#define HAPI_CDIS_V21TIMEOUT_EVT        (HSF_MKEVT_STD(HAPICDISSRV, 2))   /* V.21 HDLC detector has timed
                                               out */
      /* OP1: Reserved = 0 */
      /* OP2: Reserved = 0 */

#define HAPI_CDIS_EGRESS_DETECT_EVT     (HSF_MKEVT_STD(HAPICDISSRV, 3))  /* fax/modem detected */
      /* OP1: CDIS Detector Mask */
      /* OP2: 1 = ON, 0 = OFF */
 
#define HAPI_CDIS_EGRESS_HDLCDETECT_EVT (HSF_MKEVT_STD(HAPICDISSRV, 4))  /* HDLCs detected in egress direction */ /* Currently not supported */
      /* OP1: Reserved = 0 */
      /* OP2: Reserved = 0 */
 

/* Mapping from new to old defines */
/* This is to minimize the code changes */

#define HAPI_CDIS_ENABLE              HAPI_CDIS_ENABLE_CMD
#define HAPI_CDIS_V21ENABLE           HAPI_CDIS_V21ENABLE_CMD
#define HAPI_CDIS_EXTLOWENERGYDET     HAPI_CDIS_EXTLOWENERGYDET_CMD

#define HAPI_CDIS_DETECT              HAPI_CDIS_DETECT_EVT
#define HAPI_CDIS_HDLCDETECT          HAPI_CDIS_HDLCDETECT_EVT
#define HAPI_CDIS_V21TIMEOUT          HAPI_CDIS_V21TIMEOUT_EVT
#define HAPI_CDIS_EGRESS_DETECT       HAPI_CDIS_EGRESS_DETECT_EVT
#define HAPI_CDIS_EGRESS_HDLCDETECT   HAPI_CDIS_EGRESS_HDLCDETECT_EVT



/* V18 Preambles supported */
typedef enum {
   CDIS_V18ASCII=0,  /* Bell 103 indicator tone. 1280Hz or 2225 Hz */
   CDIS_V18BAUDOT,   /* Baudot 45.5, 1400Hz Preamble */
   CDIS_V18PREAMBLE, /* V18 Preamble As defined by V18 Spec */
   CDIS_V18BAUDOT_SHORT   /* Baudot 45.5, 1400Hz Preamble, shorter count */
} CDIS_V18_PREAMBLE_TYPES;

/* CDIS V.21 HDLC detector timeout type */
typedef enum
{
   V21TIMEOUT_ON = 0,
   V21TIMEOUT_OFF

} HSECDISV21TIMER;


#define HAPI_CDIS_CUSTOMTONEFILTER_NUM_TAPS     65

/* Custom tone filter */
typedef struct
{
   HAPI_UINT16 duration;   /* detection duration */
   HAPI_SINT16 offset;     /* detection duration offset - can be used to fine-tune detection duration */
   HAPI_SINT16 filterTaps[HAPI_CDIS_CUSTOMTONEFILTER_NUM_TAPS/2 + 1];
} HAPI_CDIS_CUSTOMTONE_FILTER;


/* status register mask */
#define CDIS_ENABLED         0x1     /* CDIS Enabled */
#define CDIS_CNG_ENABLE      0x4     /* CDIS CNG detection Enabled */
#define CDIS_V18_ENABLE      0x8     /* enable V.18 detection, it should match the define
                                        HSNET_OPTION_V18ACTIVE in hapi_net.h */
#define CDIS_BELL103_ENABLE  0x10    /* CDIS BELL 103 detection Enabled */
#define CDIS_CUSTOMTONE_ENABLE  0x20    /* Custom tone detection Enabled */

/* CDIS Detector Mask */
#define CDIS_MACHINE         0x2     /* Machine detect (CED/CNG) */
#define CDIS_CNG             0x8     /* CNG Detected */
#define CDIS_EARLY_ANS       0x10    /* Early detection of ANS tone */
#define CDIS_FALSE_ANS       0x20    /* False Early detection of ANS tone */
#define CDIS_V18_DETECT      0x40    /* V18 Detect */
#define CDIS_PHASE_REVERSAL  0x80    /* Phase reversal detected in ANS tone */
#define CDIS_LOW_ENERGY      0x100   /* low energy signal detected
                                      * AFTER phase reversal detection */
#define CDIS_EXT_LOW_ENERGY  0x200   /* extended low energy signal detected */
#define CDIS_SILENCE         0x400   /* silence detected (1.5 second no signal in egress and 20ms no signal in ingress) */
#define CDIS_BELL103_DETECT  0x800   /* Bell 103 Detection */

#define CDIS_CUSTOMTONE_DETECT  0x2000   /* Custom tone Detection */

#endif      /* HAPI_CDIS_H */


