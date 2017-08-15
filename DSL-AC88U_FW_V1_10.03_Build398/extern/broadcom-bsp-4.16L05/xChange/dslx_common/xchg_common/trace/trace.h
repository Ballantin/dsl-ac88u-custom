/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: trace.h
*
****************************************************************************
*    Description:
*
*     Interface for trace debug module. This allows users to capture data
*     to a circular trace buffer that can later be printed/post-processed.
*     Users can specify whether the circular buffer should wrap, and whether
*     timestamps and task ids should be logged. The trace can be enabled
*     and disabled at run-time. Also, users can provide an application
*     specific handler to print/post-process the data.
*
*     An instance of the trace object is created by calling traceCreate().
*     Data is logged to the trace buffer by using one of the traceLogx() macros,
*     and can later be printed by calling tracePrintContents().
*
****************************************************************************/

#ifndef TRACE_H
#define TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <bosTypes.h>
#include <bosTime.h>
#include <bosTask.h>


/* ---- Constants and Types ---------------------------------------------- */

/* Set to 0 to disable all trace logging. */
#define TRACE_ENABLED    1

/* Maximum number of data arguments that can be captured by calling traceLogx(). */
#define TRACE_MAX_RECORD_ARGS    4


/* Macros used to capture data to the trace buffer. */
#define traceLog1( trace, a )          traceLog( (trace), (a), 0, 0, 0 )
#define traceLog2( trace, a, b )       traceLog( (trace), (a), (b), 0, 0 )
#define traceLog3( trace, a, b, c )    traceLog( (trace), (a), (b), (c), 0 )
#define traceLog4( trace, a, b, c, d ) traceLog( (trace), (a), (b), (c), (d) )


/* Captured trace data type. */
typedef int TRACE_DATA;

/* Data captured by calling traceLogx() is stored in a TRACE_RECORD.
** The circular trace buffer is an array of TRACE_RECORDS. */
typedef struct TRACE_RECORD
{
   BOS_BOOL       isUsed;
   BOS_TIME_MS    timeMsec;
   BOS_TASK_ID    taskId;
   TRACE_DATA     arg[ TRACE_MAX_RECORD_ARGS ];

} TRACE_RECORD;


typedef struct TRACE_PRINT_RECORD_INFO
{
   const struct TRACE_STATE   *state;
   const struct TRACE_RECORD  *record;
   unsigned int index;

} TRACE_PRINT_RECORD_INFO;


/* Function prototype of application-specific print trace record callback. */
typedef void (*traceRecordPrintCallback)
(
   TRACE_PRINT_RECORD_INFO *info
);


/* Trace object instance memory. */
typedef struct TRACE_STATE
{
   unsigned int               magic;
   const char                *name;
   unsigned int               maxTraceRecords;
   unsigned int               numTraceRecordArgs;
   unsigned int               traceRecordIndex;
   TRACE_RECORD              *traceRecords;
   BOS_BOOL                   bWrapRecords;
   BOS_BOOL                   bLogTimestamps;
   BOS_BOOL                   bLogTaskId;
   BOS_BOOL                   bEnabled;
   traceRecordPrintCallback   printRecordCallback;

} TRACE_STATE;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   traceInit
**
** PURPOSE:    Initialize the trace module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
#if TRACE_ENABLED
   void traceInit( void );
#else
   #define traceInit()
#endif


/*
*****************************************************************************
** FUNCTION:   traceDeinit
**
** PURPOSE:    De-initializes the trace module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
#if TRACE_ENABLED
   void traceDeinit( void );
#else
   #define traceDeinit()
#endif

/*
*****************************************************************************
** FUNCTION:   traceCreate
**
** PURPOSE:    Instantiate an trace capture object.
**
** PARAMETERS: name                 (in)  Name of trace capture (may be NULL).
**             maxTraceRecords      (in)  Max number of records that can be captured.
**             numTraceRecordArgs   (in)  Number of data elements captured per record.
**             bWrapRecords         (in)  Enable/disable wrap of capture buffer.
**             bLogTimestamps       (in)  Enable/disable timestamp logging.
**             bLogTaskId           (in)  Enable/disable task id logging.
**             bEnable              (in)  Sets initial state of trace capture.
**             printRecordCallback  (in)  App-specific print trace record callback.
**                                        (Set to NULL to use default print function).
**             trace                (out) Created trace object.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/

#if TRACE_ENABLED
   void traceCreate
   (
      const char                *name,
      unsigned int               maxTraceRecords,
      unsigned int               numTraceRecordArgs,
      BOS_BOOL                   bWrapRecords,
      BOS_BOOL                   bLogTimestamps,
      BOS_BOOL                   bLogTaskId,
      BOS_BOOL                   bEnable,
      traceRecordPrintCallback   printRecordCallback,
      TRACE_STATE               *trace
   );
#else
   #define traceCreate( a, b, c, d, e, f, g, h, t )
#endif

/*
*****************************************************************************
** FUNCTION:   traceDestroy
**
** PURPOSE:    To de-allocate a trace capture object.
**
** PARAMETERS: trace (mod) Trace object.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
#if TRACE_ENABLED
   void traceDestroy( TRACE_STATE *trace );
#else
   #define traceDestroy( t )
#endif

/*
*****************************************************************************
** FUNCTION:   traceEnable
**
** PURPOSE:    Enable trace capture.
**
** PARAMETERS: trace (mod) Trace object.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
#if TRACE_ENABLED
   void traceEnable( TRACE_STATE *trace );
#else
   #define traceEnable( t )
#endif

/*
*****************************************************************************
** FUNCTION:   traceDisable
**
** PURPOSE:    Disable trace capture.
**
** PARAMETERS: trace (mod) Trace object.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
#if TRACE_ENABLED
   void traceDisable( TRACE_STATE *trace );
#else
   #define traceDisable( t )
#endif

/*
*****************************************************************************
** FUNCTION:   traceLog
**
** PURPOSE:    Main trace capture function. This should be called indirectly
**             through one of the traceLogx() macros.
**
** PARAMETERS: trace (mod) Trace object.
**             arg1  (in)  Data arg1 to capture.
**             arg2  (in)  Data arg2 to capture (optional).
**             arg3  (in)  Data arg3 to capture (optional).
**             arg4  (in)  Data arg4 to capture (optional).
**
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
#if TRACE_ENABLED
   void traceLog
   (
      TRACE_STATE   *trace,
      TRACE_DATA     arg1,
      TRACE_DATA     arg2,
      TRACE_DATA     arg3,
      TRACE_DATA     arg4
   );
#else
   #define traceLog( t, a, b, c, d )
#endif

/*
*****************************************************************************
** FUNCTION:   tracePrintContexts
**
** PURPOSE:    Dump the contents of the specified trace buffer.
**
** PARAMETERS: trace (in)  Trace capture object.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
#if TRACE_ENABLED
   void tracePrintContents( const TRACE_STATE *trace );
#else
   #define tracePrintContents( t )
#endif

/*
*****************************************************************************
** FUNCTION:   tracePrintList
**
** PURPOSE:    To display a list of all the trace captures that have been created.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
#if TRACE_ENABLED
   void tracePrintList( void );
#else
   #define tracePrintList()
#endif


#ifdef __cplusplus
    }
#endif

#endif  /* TRACE_H  */
