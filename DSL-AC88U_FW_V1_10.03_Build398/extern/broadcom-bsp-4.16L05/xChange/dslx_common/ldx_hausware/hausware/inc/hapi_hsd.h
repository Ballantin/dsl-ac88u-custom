/****************************************************************************
*
*  HAPI_HSD.H - HAUSWARE Definitions for the HSD
*
*  PURPOSE:
*
*     This file contains the Human Speech detector API definitions.
*
**********************************************************************/

#ifndef  HAPI_HSD_H
#define  HAPI_HSD_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware service ids */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* Hausware generic cmds/events */
#endif

#define HAPIHSDSRV         HAPI_HSD_SRV

/***************************************************************************
**        HSD SERVICE COMMANDS
****************************************************************************/

#define HAPI_HSD_ENABLE_CMD        (HSF_MKHDR(HAPIHSDSRV, HAPI_OPC_ENABLE_CMD))  /* enable */


/***************************************************************************
**        CDIS SERVICE ASYNCHRONOUS EVENTS
****************************************************************************/
#define HAPI_HSD_VOICE_EVT          (HSF_MKEVT_STD(HAPIHSDSRV, 0))  /* Voice event detected */

/* HSD status word */
#define HSD_ENABLE             0x0001      /* bit zero */

 
#define HAPI_HSD_ENABLE   HAPI_HSD_ENABLE_CMD
#define HAPI_HSD_VOICE    HAPI_HSD_VOICE_EVT

#endif

