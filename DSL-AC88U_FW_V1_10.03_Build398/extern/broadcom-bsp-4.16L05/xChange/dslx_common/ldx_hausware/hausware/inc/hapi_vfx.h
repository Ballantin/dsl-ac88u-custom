/****************************************************************************
*
*  HAPI_VFAX.H - HAUSWARE Definitions for a FAX Modem Service
*
*  PURPOSE:
*     This file contains the FAX Modem API definitions.
*
*  (c) Copyright HotHaus Technologies 1996, 1997
*	    All rights reserved
*
****************************************************************************/

#ifndef HAPI_VFAX_H                             /* support nested includes */
#define HAPI_VFAX_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>                   /* Hausware environment definitions */
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* Hausware API Global definitions */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* Hausware generic cmds/events */
#endif
#ifndef HAPI_GFAX
#include <hapi_gfax.h>                          /* generic fax definitions */
#endif

#if !defined( HAPIIDS_H )
#  include <hapiids.h>
#endif

#define  HAPIVFAXSRV                HAPI_VFAX_SRV

/***************************************************************************
**        SERVICE COMMANDS
****************************************************************************/

/*
 * Generic commands:
 *    - have the STD/EXT bit set in the HAPI_OPC_xxx define
 *    - don't require descriptions of op1 and op2 - see hapi_opcode.h for these
 */
#define  HAPI_VFAX_ENABLE_CMD        (HSF_MKHDR(HAPIVFAXSRV, HAPI_OPC_ENABLE_CMD))
#define  HAPI_VFAX_SETREGS_CMD       (HSF_MKHDR(HAPIVFAXSRV, HAPI_OPC_SETREGS_CMD))
#define  HAPI_VFAX_SETREG1_CMD       (HSF_MKHDR(HAPIVFAXSRV, HAPI_OPC_SETREG1_CMD))
#define  HAPI_VFAX_RESETREGS_CMD     (HSF_MKHDR(HAPIVFAXSRV, HAPI_OPC_RESETREGS_CMD))
#define  HAPI_VFAX_GETREGS_CMD       (HSF_MKHDR(HAPIVFAXSRV, HAPI_OPC_GETREGS_CMD))
#define  HAPI_VFAX_GETSTATE_CMD      (HSF_MKHDR(HAPIVFAXSRV, HAPI_OPC_GETSTATE_CMD))

/*
 * V.FAX service specific commands
 */
#define  HAPI_VFAX_GETSTATUS_CMD     (HSF_MKCMD_STD(HAPIVFAXSRV, HAPIGFAXGETSTATUS))
   /* op1: reserved = 0
      op2: reserved = 0 */
#define  HAPI_VFAX_RXCFG_CMD         (HSF_MKCMD_STD(HAPIVFAXSRV, HAPIGFAXRXCFG))
   /* op1: see HAPI_VFAX_RXENABLE below
      op2: see HAPI_VFAX_RXCFG below */

#define  HAPI_VFAX_SETIDLEPTN_CMD    (HSF_MKCMD_EXT(HAPIVFAXSRV, HAPIGFAXSETIDLEPTN))
   /* op1: no. of 16 bit words of idle ptn
      op2: ptr to idle ptn */

#define  HAPI_VFAX_TXCFG_CMD         (HSF_MKCMD_STD(HAPIVFAXSRV, HAPIGFAXTXCFG))
   /* op1: see HAPI_VFAX_TXCFG below
      op2: reserved = 0 */

#define  HAPI_VFAX_TXDATA_CMD        (HSF_MKCMD_EXT(HAPIVFAXSRV, HAPIGFAXTXDATA))
   /* op1: no. of 16 bit words in buffer
      op2: ptr to buffer */


/***************************************************************************
**        SERVICE COMMAND RESPONSES
****************************************************************************/

#define HAPI_VFAX_GETREGS_RSP      (HSF_MKRSP_EXT(HAPI_VFAX_GETREGS_CMD))
#define HAPI_VFAX_GETSTATE_RSP     (HSF_MKRSP_EXT(HAPI_VFAX_GETSTATE_CMD))
#define HAPI_VFAX_GETSTATUS_RSP    (HSF_MKRSP_EXT(HAPI_VFAX_GETSTATUS_CMD))

/***************************************************************************
**        SERVICE ASYNCHRONOUS EVENTS
****************************************************************************/

/*
 * V.FAX service events
 */
#define  HAPI_VFAX_TXDATASENT_EVT      (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXTXDATASENT))
   /* op1: no. of bits sent
      op2: reserved = 0 */

#define  HAPI_VFAX_MODEMRDY_EVT      (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXMODEMRDY))
   /* op1: reserved = 0
      op2: reserved = 0 */

#define  HAPI_VFAX_NOCARRIER_EVT     (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXNOCARRIER))
   /* op1: reserved = 0
      op2: reserved = 0 */

#define  HAPI_VFAX_RXDATA_EVT        (HSF_MKEVT_EXT(HAPIVFAXSRV, HAPIGFAXRXDATA))
   /* op1: no. of 16 bit words in buffer
      op2: ptr to buffer */

#define  HAPI_VFAX_RXDATAEND_EVT     (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXRXDATAEND))
   /* op1: peak average error
      op2: 1 if RTC detected, else 0 */

#define  HAPI_VFAX_RXTCFERR_EVT      (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXRXTCFERR))
   /* op1: no. of bit errors
      op2: peak average error */

#define  HAPI_VFAX_TRAINRX_EVT       (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXTRAINRX))
   /* op1: 1
      op2: reserved = 0 */

#define  HAPI_VFAX_TXCMDQHIGH_EVT    (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXTXCMDQHIGH))
   /* op1: reserved = no. of buffers in data queue
      op2: reserved = 0 */

#define  HAPI_VFAX_TXCMDQLOW_EVT     (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXTXCMDQLOW))
   /* op1: reserved = no. of buffers in data queue
      op2: reserved = 0 */

#define  HAPI_VFAX_TXDATAEND_EVT     (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXTXDATAEND))
   /* op1: reserved = 0
      op2: reserved = 0 */

#define  HAPI_VFAX_DATASENT_EVT     (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXDATASENT))
   /* op1: reserved = 0
      op2: reserved = 0 */

#define  HAPI_VFAX_RXAVEERROR_EVT     (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXRXAVEERROR))
   /* op1: reserved = 0
      op2: reserved = 0 */

#define  HAPI_VFAX_NOCARRIER_EVT     (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXNOCARRIER))
#define  HAPI_VFAX_BMDEBUG_EVT     (HSF_MKEVT_STD(HAPIVFAXSRV, HAPIGFAXBMDEBUG))
   /* op1: reserved = 0
      op2: reserved = 0 */

/*
 *  State structure (returned by HAPI_VFAX_GETSTATE_RSP)
 */
typedef struct
{
   HAPI_UINT16 status;
   HAPI_UINT16 reserved;

} HAPI_VFAX_STATE;


/* VFAX status bit definition */

#define  HSVFAXENABLE             0x0001    /* fax enabled */

#define  HSVFAXTXMODE             0x0002    /* 0 = TCF training mode */
                                            /* 1 = data mode */
#define  HSVFAXRXMODE             0x0004    /* 0 = TCF training mode */
                                            /* 1 = data mode */
#define  HSVFAXV33MODE            0x0008    /* 0 = V.33 not in use */
                                            /* 1 = V.33 in use */
#define  HSVFAXLONGSTART          0x0010    /* 0 = short startup sequence */
                                            /* 1 = long startup sequence */
#define  HSVFAXRTCDETD            0x0020    /* synchronization lost */

/*
 *  Set this bit in hsxrxpktsize register if the receive packet size is
 *  specified in bits rather than ms
 */

#define HAPI_VFX_RXPKTSIZE_IN_BITS HAPI_GFAX_RXPKTSIZE_IN_BITS
typedef enum
{
   HSVFAXTXCFGIDLE      = HSGFAXTXCFGIDLE,     /* put transmitter in idle mode */
   HSVFAXTXCFGTURNON    = HSGFAXTXCFGTURNON, /* transmit turn on and data */
   HSVFAXTXCFGTRAIN     = HSGFAXTXCFGTRAIN,    /* transmit training check */
   HSVFAXTXCFGEXTTURNON = HSGFAXTXCFGEXTTURNON /* transmit extended turn on and  data */

} HSEVFAXTXCFG;

typedef enum                              /* RX ENABLE DEFINITIONS */
{
   HSVFAXRXCFGOFF = HSGFAXRXCFGOFF,       /* put receiver in idle mode */
   HSVFAXRXCFGON  = HSGFAXRXCFGON          /* put receiver in active mode */

} HSEVFAXRXENABLE;

typedef enum                              /* RX CONFIGURATION DEFINITIONS */
{
   HSVFAXRXCFGTRAIN      = HSGFAXRXCFGTRAIN,   /* receive training check */
   HSVFAXRXCFGTURNON     = HSGFAXRXCFGTURNON, /* receive turnon and data */
   HSVFAXRXCFGEXTTURNON  = HSGFAXRXCFGEXTTURNON, /* treated same as
                                                   HSVFAXRXCFGTURNON */
   HSVFAXRXCFGTCFDATA    = HSGFAXRXCFGTCFDATA,      /* receive TCF as data */
   HSVFAXRXCFGECMDATA    = HSGFAXRXCFGECMDATA,
   HSVFAXRXCFGEXTECMDATA = HSGFAXRXCFGEXTECMDATA

} HSEVFAXRXCFG;


/* data rate */

typedef enum
{
   /* the V.17 code is dependent on the order of these definitions */
   HSVFAX_0      = 0,
   HSV27TER_2400 = HSGFAX_2400,
   HSV27TER_4800 = HSGFAX_4800,
   HSV29_7200    = HSGFAX_7200,
   HSV29_9600    = HSGFAX_9600,
   /* V17 rate define */
   HSV33_12000   = HSGFAX_12000_V33,       /* V.33 12 kbps trellis coded */
   HSV33_14400   = HSGFAX_14400_V33,       /* V.33 14.4 kbps trellis coded */
   HSV17_7200    = HSGFAX_7200_V17,        /* V.17 7.2 kbps trellis coded */
   HSV17_9600    = HSGFAX_9600_V17,        /* V.17 9.6 kbps trellis coded */
   HSV17_12000   = HSGFAX_12000_V17,       /* V.17 12 kbps trellis coded */
   HSV17_14400   = HSGFAX_14400_V17        /* V.17 14.4 kbps trellis coded */

} HSEVFAXRATE;


typedef struct
{
   HAPI_UINT16         hsxrate;
   HAPI_UINT16         hsxeptgenflag;
   HAPI_UINT16         hsxeptgenduration;
   HAPI_UINT16         hsxsilenceduration;
   HAPI_UINT16         hsxeptamplitude;
   HAPI_UINT16         hsxtxamplitude;
   HAPI_UINT16         hsxnumbits;
   HAPI_UINT16         hsxcmdqhighthreshold;/* command queue high threshold */
   HAPI_UINT16         hsxcmdqlowthreshold; /* command queue low threshold */
   HAPI_UINT16         alignmentPad;           /* for 32-bit alignment */

} HSZVFAXREGS;


#define HAPI_VFAX_DEF_REGS                                           \
{                                                                    \
   HSV17_14400,         /* data rate */                              \
   0,                   /* echo protection tone (EPT) */             \
   200,                 /* ept duration*/                            \
   20,                  /* silence duration */                       \
   16384,               /* ept amplitude */                          \
   16384,               /* fax transmitted signal amplitude */       \
   20,                  /* number of ms per receive packet */        \
   24,                  /* command queue high threshold */           \
   8,                   /* command queue low threshold */            \
   0                                                                 \
};

#endif                                    /* HAPI_VFAX_H */
