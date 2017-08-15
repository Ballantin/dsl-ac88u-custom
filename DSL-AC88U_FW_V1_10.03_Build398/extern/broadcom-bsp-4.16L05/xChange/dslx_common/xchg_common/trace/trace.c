/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: trace.c
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

/* ---- Include Files ---------------------------------------------------- */

#include "trace.h"

#if TRACE_ENABLED

#include <string.h>
#include <stdlib.h>
#include <xchgAssert.h>
#include <bosTask.h>
#include <bosTime.h>
#include <bosMutex.h>
#include <bosCritSect.h>
#include <str.h>
#include <linkedList.h>
#include <blog.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define TRACE_CRITICAL_SECTION_ENTER   bosCritSectEnter( &gTraceCriticalSection ); \
                                       do

#define TRACE_CRITICAL_SECTION_LEAVE   while ( 0 ); \
                                       bosCritSectLeave( &gTraceCriticalSection );



/* Used to check the integrity of trace capture objects. */
#define MAGIC_NUMBER    0xC0FFEE


/* ---- Private Variables ------------------------------------------------ */

static LINKED_LIST   gTraceList;
static BOS_MUTEX     gTraceListMutex;
static BOS_CRITSECT  gTraceCriticalSection;
static BOS_BOOL      gInitialized = BOS_FALSE;


/* ---- Private Function Prototypes -------------------------------------- */

static void DefaultPrintRecord( TRACE_PRINT_RECORD_INFO *info );


/* ---- Functions -------------------------------------------------------- */

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
void traceInit( void )
{
   BOS_STATUS  status;

   XCHG_ASSERT( !gInitialized );

   status = bosMutexCreate( "traceMutex", &gTraceListMutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   status = bosCritSectCreate( &gTraceCriticalSection );
   XCHG_ASSERT( status == BOS_STATUS_OK );


   linkedListCreate( &gTraceList );

   gInitialized = BOS_TRUE;
}


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
void traceDeinit( void )
{
   XCHG_ASSERT( gInitialized );

   gInitialized = BOS_FALSE;

   linkedListDestroy( &gTraceList );

   bosCritSectDestroy( &gTraceCriticalSection );
   bosMutexDestroy( &gTraceListMutex );
}


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
)
{
   TRACE_RECORD        *traceRecord;
   LINKED_LIST_ELEMENT *traceListElement;

   XCHG_ASSERT( gInitialized );
   XCHG_ASSERT_COND( numTraceRecordArgs, <= , TRACE_MAX_RECORD_ARGS );

   memset( trace, 0, sizeof( *trace ) );

   traceRecord = (TRACE_RECORD *) calloc( maxTraceRecords, sizeof( TRACE_RECORD ) );

   /*
   ** Init trace capture object...
   */
   trace->magic               = MAGIC_NUMBER;
   trace->bLogTaskId          = bLogTaskId;
   trace->bLogTimestamps      = bLogTimestamps;
   trace->bWrapRecords        = bWrapRecords;
   trace->maxTraceRecords     = maxTraceRecords;
   trace->numTraceRecordArgs  = numTraceRecordArgs;
   trace->traceRecords        = traceRecord;

   if ( printRecordCallback == NULL )
   {
      trace->printRecordCallback = DefaultPrintRecord;
   }
   else
   {
      trace->printRecordCallback = printRecordCallback;
   }


   if ( name == NULL )
   {
      trace->name = "No name";
   }
   else
   {
      trace->name = name;
   }

   trace->bEnabled = bEnable;


   /*
   ** Add trace object to the global list.
   */
   traceListElement        = (LINKED_LIST_ELEMENT *) calloc( 1, sizeof( LINKED_LIST_ELEMENT ) );
   traceListElement->data  = trace;

   bosMutexAcquire( &gTraceListMutex );
   {
      linkedListAddToBack( &gTraceList, traceListElement );
   }
   bosMutexRelease( &gTraceListMutex );
}


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
void traceDestroy( TRACE_STATE *trace )
{
   LINKED_LIST_ELEMENT *traceListElement;

   XCHG_ASSERT( gInitialized );

   bosMutexAcquire( &gTraceListMutex );
   {
      traceListElement = linkedListRemoveElement( &gTraceList, trace );
   }
   bosMutexRelease( &gTraceListMutex );

   free( traceListElement );

   free( trace->traceRecords );

   memset( trace, 0, sizeof( *trace ) );
}


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
void traceEnable( TRACE_STATE *trace )
{
   XCHG_ASSERT( gInitialized );

   trace->bEnabled            = BOS_TRUE;
}


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
void traceDisable( TRACE_STATE *trace )
{
   XCHG_ASSERT( gInitialized );

   trace->bEnabled            = BOS_FALSE;
}


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
void traceLog
(
   TRACE_STATE   *trace,
   TRACE_DATA     arg1,
   TRACE_DATA     arg2,
   TRACE_DATA     arg3,
   TRACE_DATA     arg4
)
{
   TRACE_RECORD  *record;

   TRACE_CRITICAL_SECTION_ENTER
   {
      if ( !trace->bEnabled )
      {
         /*
         ** Trace is disabled...bail...
         */
         break;
      }



      if (  ( !trace->bWrapRecords )
         && ( trace->traceRecordIndex >= trace->maxTraceRecords ) )
      {
         /*
         ** Wrapping is disabled, and the record buffer is full...bail...
         */
         break;
      }


      /* Wrap check. */
      if ( trace->traceRecordIndex == trace->maxTraceRecords )
      {
         trace->traceRecordIndex = 0;
      }


      record = &trace->traceRecords[trace->traceRecordIndex];

      /* Log trace arguments. */
      record->arg[0] = arg1;
      record->arg[1] = arg2;
      record->arg[2] = arg3;
      record->arg[3] = arg4;

      /* Log timestamp. */
      if ( trace->bLogTimestamps )
      {
         bosTimeGetMs( &record->timeMsec );
      }

      /* Log task id. */
      if ( trace->bLogTaskId )
      {
         bosTaskGetMyTaskId( &record->taskId );
      }

      /* Mark the record as used. */
      record->isUsed = BOS_TRUE;

      trace->traceRecordIndex++;
   }
   TRACE_CRITICAL_SECTION_LEAVE
}


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
void tracePrintContents( const TRACE_STATE *trace )
{
   unsigned int   index = 0;
   TRACE_RECORD  *record, *oldestRecord;

   if ( !gInitialized )
   {
      blogStrRaw(( "ERROR: Sorry, trace module hasn't been initialized.\n" ));
      return;
   }

   /* Error check. */
   if ( ( trace == NULL ) || ( trace->magic != MAGIC_NUMBER ) )
   {
      blogStrRaw(( "ERROR: Invalid trace object '%08x'\n", (unsigned int) trace ));
      return;
   }


   blogStrRaw(( "Trace contents for object (%08x), oldest to newest:\n", (unsigned int) trace ));


   /* Wrap check. */
   oldestRecord   = &trace->traceRecords[trace->traceRecordIndex];
   if ( trace->traceRecordIndex == trace->maxTraceRecords )
   {
      oldestRecord = &trace->traceRecords[0];
   }

   record         = oldestRecord;
   XCHG_ASSERT( trace->printRecordCallback != NULL );

   do
   {
      if ( record->isUsed )
      {
         TRACE_PRINT_RECORD_INFO info = { trace, record, ++index };
         trace->printRecordCallback( &info );
      }


      /* Wrap check. */
      if ( record == &trace->traceRecords[trace->maxTraceRecords - 1] )
      {
         record = &trace->traceRecords[0];
      }
      else
      {
         record++;
      }

   }
   while ( record != oldestRecord );

   blogStrRaw(( "\n" ));
}


/*
*****************************************************************************
** FUNCTION:   DefaultPrintRecord
**
** PURPOSE:    Default handler for displaying captured trace buffers.
**
** PARAMETERS: info  (in)
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void DefaultPrintRecord( TRACE_PRINT_RECORD_INFO *info )
{
   char                 displayStr[ 256 ];
   const TRACE_RECORD  *record   = info->record;
   const TRACE_STATE   *trace    = info->state;
   unsigned int         index    = info->index;


   XCHG_ASSERT( gInitialized );

   displayStr[0] = '\0';

   strPrintf( displayStr, sizeof( displayStr ), "%4d) ", index );


   if ( trace->bLogTimestamps )
   {
      strPrintf( &displayStr[ strlen( displayStr ) ],
                 sizeof( displayStr ) - strlen( displayStr ),
                 "[%d] ",
                 record->timeMsec );
   }

   if ( trace->bLogTaskId )
   {
      strPrintf( &displayStr[ strlen( displayStr ) ],
                 sizeof( displayStr ) - strlen( displayStr ),
                 "(%08x) ",
                 record->taskId );
   }

   strPrintf( &displayStr[ strlen( displayStr ) ],
              sizeof( displayStr ) - strlen( displayStr ),
              "%08x %08x %08x %08x\n",
              record->arg[0],
              record->arg[1],
              record->arg[2],
              record->arg[3] );

   blogStrRaw(( "%s", displayStr ));
}


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
void tracePrintList( void )
{
   TRACE_STATE            *trace;
   LINKED_LIST_ELEMENT    *listIterator;
   unsigned int            index = 0;

   if ( !gInitialized )
   {
      blogStrRaw(( "Sorry, trace module hasn't been initialized.\n" ));
      return;
   }

   bosMutexAcquire( &gTraceListMutex );
   {
      listIterator = linkedListPeekFront( &gTraceList );

      while( listIterator != NULL )
      {
         trace = (TRACE_STATE *) listIterator->data;

         blogStrRaw(( "%2d) Trace '%s' (%08x)\n", ++index, trace->name, (unsigned int) trace ));

         listIterator = listIterator->next;
      }
   }
   bosMutexRelease( &gTraceListMutex );

   blogStrRaw(( "\n\n" ));
}

#endif   /* TRACE_ENABLED */
