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
*  hapi_icp.h - Idle CPU Profiler (ICP) header file
*
*  PURPOSE:
*     The Idle CPU Profiler (ICP) is used to profile the amount of time
*     the DSP spends idling.  Such a measurement is useful to infer the
*     percent CPU usage of the DSP.
*
*     The basis of the ICP profiler is a counter that increments while
*     the DSP is idling.  The counter effectively consumes the idling
*     cycles of the DSP.  In order for the counter to consume only the
*     idling cycles, the counter runs in a background task that is
*     pre-empted by HausWare tasks.
*
*     In order for the profile results to be meaningful, the results need
*     to be compared to a meaningful reference counter value.  Since ICP
*     measures the amount of idle time on the DSP, it makes sense to obtain,
*     as the reference value, the DSP idling 100% of the time.  ICP
*     measurements on a busy DSP are compared to the reference value to
*     obtain percent CPU usage.
*
*     ICP consists of two phases.  The first phase, known as the calibration
*     phase, is used to obtain the reference counter value.  In order to
*     obtain this value, the DSP must not be servicing interrupts nor
*     executing any code.  As a result, this measurement is obtained during
*     system initialization where interrupts has yet to be enabled.  The
*     length of time profiled in the calibration phase is 1 second.
*
*     The second phase is the profile phase.  When enabled, ICP measures
*     the idle time of the DSP.  The user specifies the length of time to
*     be profiled for each data point (default value is 1 second).
*
*     The user can specify ICP to return the statistics periodically to the
*     client application as events.  Also, the user can specify ICP to
*     return statistic events whenever a substantial change occurs in the
*     statistics.  In addition, the user can query the statistics at any
*     point during run-time.
*
*  NOTES:
*
****************************************************************************/

#ifndef HAPI_ICP_H
#define HAPI_ICP_H

/* ---- Include Files ---------------------------------------------------- */

#ifndef HAPI_TYPES_H
#include <hapi_types.h>                   /* Hausware environment defines */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* Opcode information           */
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Service IDs                  */
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* Global Definitions           */
#endif

/* ---- Constants and Types ---------------------------------------------- */

/*
 * Idle CPU Profiler generic commands
 */
#define HAPI_ICP_ENABLE_CMD      (HSF_MKHDR(HAPI_ICP_SRV, HAPI_OPC_ENABLE_CMD))
#define HAPI_ICP_SETREGS_CMD     (HSF_MKHDR(HAPI_ICP_SRV, HAPI_OPC_SETREGS_CMD))
#define HAPI_ICP_SETREG1_CMD     (HSF_MKHDR(HAPI_ICP_SRV, HAPI_OPC_SETREG1_CMD))
#define HAPI_ICP_RESET_STATS_CMD (HSF_MKHDR(HAPI_ICP_SRV, HAPI_OPC_RESET_STATS_CMD))
#define HAPI_ICP_GETREGS_CMD     (HSF_MKCMD_EXT(HAPI_ICP_SRV, 0))
#define HAPI_ICP_GETSTATS_CMD    (HSF_MKCMD_EXT(HAPI_ICP_SRV, 1))

/*
 * Idle CPU Profiler responses
 */
#define HAPI_ICP_ENABLE_RSP      (HSF_MKRSP_STD(HAPI_ICP_ENABLE_CMD))
#define HAPI_ICP_SETREGS_RSP     (HSF_MKRSP_STD(HAPI_ICP_SETREGS_CMD))
#define HAPI_ICP_SETREG1_RSP     (HSF_MKRSP_STD(HAPI_ICP_SETREG1_CMD))
#define HAPI_ICP_RESET_STATS_RSP (HSF_MKRSP_STD(HAPI_ICP_RESET_STATS_CMD))
#define HAPI_ICP_GETREGS_RSP     (HSF_MKRSP_EXT(HAPI_ICP_GETREGS_CMD))
#define HAPI_ICP_GETSTATS_RSP    (HSF_MKRSP_EXT(HAPI_ICP_GETSTATS_CMD))

/*
 * Idle CPU Profiler asynchronous events
 */
#define HAPI_ICP_STATS_EVT       (HSF_MKEVT_EXT(HAPI_ICP_SRV, 0))
   /* op1: Length of ICPSTATS structure
      op2: Pointer to ICPSTATS structure */

/*
 * Default Configuration Register
 */

#define HAPI_ICP_DEFAULT_UPDATEINTERVAL   50
#define HAPI_ICP_DEFAULT_EVENTINTERVAL    0
#define HAPI_ICP_DEFAULT_CHANGENOTIFY     0
#define HAPI_ICP_DEFAULT_CHANGETHRESHOLD  5

#define HAPI_ICP_DEF_REGS \
{ \
   HAPI_ICP_DEFAULT_UPDATEINTERVAL,      /* Update interval (units of 10ms) */ \
   HAPI_ICP_DEFAULT_EVENTINTERVAL,       /* Event interval (units of 10ms) This should be a multiple \
                                            of update interval */ \
   HAPI_ICP_DEFAULT_CHANGENOTIFY,        /* Notify on substantial change 1 to enable 0 to disable */ \
   HAPI_ICP_DEFAULT_CHANGETHRESHOLD      /* Substantial change threshold, in percent   */ \
}

/*
 * Idle CPU Profiler Configuration Registers
 */
typedef struct
{
   HAPI_UINT16 updateInterval;      /* Update interval (units of 10ms)  */
   HAPI_UINT16 eventInterval;       /* Event interval (units of 10ms)   */
   HAPI_UINT16 changeNotify;        /* Notify on substantial change     */
   HAPI_UINT16 changeThreshold;     /* Substantial change threshold     */
} ICPREGS;


/*
 * Idle CPU Profiler Statistic (16 bit version)
 */
typedef struct
{
   HAPI_UINT16 calibrationCountLow;  /* Calibration Count                */
   HAPI_UINT16 calibrationCountHigh; /* Calibration Count                */
   HAPI_UINT16 iterationsLow;        /* Number of data points taken      */
   HAPI_UINT16 iterationsHigh;       /* Number of data points taken      */
   HAPI_UINT16 currentCountLow;      /* Current count value              */
   HAPI_UINT16 currentCountHigh;     /* Current count value              */
   HAPI_UINT16 totalCountLowLow;     /* Total Count - low 32 bits        */
   HAPI_UINT16 totalCountLowHigh;    /* Total Count - low 32 bits        */
   HAPI_UINT16 totalCountHighLow;    /* Total Count - high 32 bits       */
   HAPI_UINT16 totalCountHighHigh;   /* Total Count - high 32 bits       */
   HAPI_UINT16 avgCountLow;          /* Running average count            */
   HAPI_UINT16 avgCountHigh;         /* Running average count            */
   HAPI_UINT16 maxCountLow;          /* Maximum count                    */
   HAPI_UINT16 maxCountHigh;         /* Maximum count                    */
   HAPI_UINT16 minCountLow;          /* Minimum count                    */
   HAPI_UINT16 minCountHigh;         /* Minimum count                    */
} ICPSTATS;


/* ---- Function Prototypes ---------------------------------------------- */

#endif /* HAPI_ICP_H */
