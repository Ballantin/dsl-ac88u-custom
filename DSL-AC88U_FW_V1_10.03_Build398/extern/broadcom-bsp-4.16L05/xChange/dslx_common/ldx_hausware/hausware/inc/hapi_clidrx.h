/****************************************************************************
*
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  hapi_clidrx.h - HAUSWARE Definitions for CLIDRX
*
*  PURPOSE:
*
*     This file contains the CLIDRX API definitions.
*
*  NOTES:
*
****************************************************************************/

#ifndef  HAPI_CLIDRX_H
#define  HAPI_CLIDRX_H

/* ---- Include Files ---------------------------------------------------- */

#include <hapi_types.h>          /* Hausware environment.                  */
#include <hapi_net.h>            /* Net definitions                        */
#include <hapi_opcode.h>         /* Hausware generic cmds/events           */

/* ---- Constants and Types ---------------------------------------------- */

/*
 * CLID RX Commands
 */
#define HAPI_CLIDRX_ENABLE_CMD     (HSF_MKHDR(HAPI_CLIDRX_SRV, HAPI_OPC_ENABLE_CMD))
#define HAPI_CLIDRX_SETREGS_CMD    (HSF_MKHDR(HAPI_CLIDRX_SRV, HAPI_OPC_SETREGS_CMD))
#define HAPI_CLIDRX_SETREG1_CMD    (HSF_MKHDR(HAPI_CLIDRX_SRV, HAPI_OPC_SETREG1_CMD))

/*
 * CLID RX Responses
 */
#define HAPI_CLIDRX_ENABLE_RSP     (HSF_MKRSP_STD(HAPI_CLIDRX_ENABLE_CMD))
#define HAPI_CLIDRX_SETREGS_RSP    (HSF_MKRSP_STD(HAPI_CLIDRX_SETREGS_CMD))
#define HAPI_CLIDRX_SETREG1_RSP    (HSF_MKRSP_STD(HAPI_CLIDRX_SETREG1_CMD))

/*
 * CLID RX Events
 */
#define HAPI_CLIDRX_PKT_EVT        (HSF_MKEVT_EXT(HAPI_CLIDRX_SRV,0))

/*
 * CLID Message buffer size
 */
#define CLIDRXNETMSGBUFSIZE     8

/*
 * CLID RX Registers
 */
typedef struct
{
   HAPI_UINT16 hsxminseizurelen;  /* Min seizure bits for valid Class signal     */
   HAPI_UINT16 hsxminmarklen;     /* Min mark signal bits for valid Class signal */
   HAPI_UINT16 hsxmaxmsglen;      /* Maximum message length (bytes)              */
   HAPI_SINT16 hsxminrxthreshold; /* Minimum power threshold (dB)                */

}  HSZCLIDRXREGS;

/*
 * Default CLID RX register values
 *
 * North American standards specified in GR-30-CORE, Issue 1, Dec. 1994:
 *    Seizure length - 300
 *    Mark length    - 180
 *    Max msg length - 1-255 bytes
 */
#define CLIDRXSEIZURELEN        250  /* Min seizure length                */
#define CLIDRXMARKLEN           150  /* Min mark length                   */
#define CLIDRXMAXMSGLEN         255  /* Maximum message length (bytes)    */
#define CLIDRXMINTHRESHOLD      -35  /* Minimum power threshold (dB)      */

/*
 * Default CLID RX register
 */
#define HAPI_CLIDRX_DEF_REGS \
{ \
   CLIDRXSEIZURELEN, \
   CLIDRXMARKLEN, \
   CLIDRXMAXMSGLEN, \
   CLIDRXMINTHRESHOLD \
}

/*
 * CLIDRX Packet Codes
 */
typedef enum {
   CLIDRX_PKT_START = 0,        /* Start of CLIDRX message   */
   CLIDRX_PKT_BODY,             /* CLIDRX message            */
   CLIDRX_PKT_END               /* CLIDRX message terminator */
} CLIDRX_PKT_CODE;

/*
 * CLID RX Packet
 */
typedef struct
{
   HAPI_UINT16  hsxClassCode;                      /* CLIDRX Code      */
   HAPI_UINT16  hsxClassMsgWord0;                  /* Word 0           */
   HAPI_UINT16  hsxClassMsgWord1;                  /* Word 1           */
   HAPI_UINT16  hsxClassMsg[CLIDRXNETMSGBUFSIZE];  /* CLID data        */
} HSZCLIDRXPACKET;

/*
 * Binary FSK symbols
 */
typedef enum
{
   HSCLIDRXSMBLSPACE = 0,      /* Space         */
   HSCLIDRXSMBLMARK,           /* Mark          */
   HSCLIDRXSMBLSILENCE,        /* Silence       */
   HSCLIDRXSMBLNULL            /* Unknown       */

} HSECLIDRXSYMBOL;

#endif /* HAPI_CLIDRX_H */
