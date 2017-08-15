/***************************************************************************
*
*     Copyright (c) 2002 Broadcom Corporation, All Rights Reserved.
*     Contains proprietary and confidential information.
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*    Description:
*     Public header file for the Broadcom Logging (BLOG) module.
*
*     BLOG provides a set of macros/functions that allow users to selectively
*     log information, using syntax similar to the standard printf function.
*
*     Each log statement has an associated "severity" (error, warning,
*     informational, etc). Only statements with enabled severities are logged.
*
*     Debug information (timestamp, module name, thread id etc) is also logged
*     for each statement. These are known as fields, and can also be selectively
*     enabled.
*
*     Messages can be logged to a queue (to be displayed after the fact), or
*     printed to the console (or other output stream), or both.
*
*     Messages can be printed to the console by a background task, or
*     printed synchronously.
*
*     Also, log statements can be "compiled-out", to save code space.
*
*     It is necessary to create a logging instance (typically, one per module)
*     prior to using the logging functions. The logging instance stores
*     information about which severities, fields, etc are enabled. Each instance
*     may have different settings. e.g.
*
*        gEndptBlogSettingsHdl = blogAddDefaultModule( "endpt" );
*
*     This handle should be used to modify logging instance attributes, e.g.
*     enabled severities, fields, etc.
*
*     A separate logging handle should be declared by the user that contains
*     implicit debug information, and is used in actual log statements, e.g.
*
*        #define gEndptBlogHdl  BLOG_HDL( gEndptBlogSettingsHdl )
*
*        blogStrErr(( gEndptBlogHdl, "Failed to free buffer %p\n", buffer ));
*
*     Note the required double parenthesis.
*
*     A set of const macros are also provided, e.g.
*
*        blogConstStrErr1( gEndptBlogHdl, "Failed to free buffer %p\n", buffer );
*
*     The number of parameters for const strings is specified explicitly in
*     the macro name.
*
*     Const macros do not memcpy the format string or parameters into the
*     logging queue. They simply store a pointer to the format string which
*     must have static storage.
*
*     "Raw" logs can be performed without blog handles. There are no format
*     fields or severities associated with raw logs, and they are always
*     displayed.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

#include "blog.h"
#include "bosInit.h"
#include "bosSleep.h"
#include "bosError.h"
#include "bosMutex.h"
#include "bosTime.h"
#include "blogpriv.h"
#include "str.h"
#include <string.h>
#include <xchgAssert.h>

#if BLOG_CFG_QUEUE_LOGS
#include "cbf.h"
#include "bosTask.h"
#endif

/* ---- Private Variables ------------------------------------ */

static BOS_BOOL          logInitialized = BOS_FALSE;

#if BLOG_CFG_QUEUE_LOGS
static Q_VARS            qVars;        /* Variables for the log queue */
static CBF_Q_OFFSET      maxQSize;     /* Stores the max queue size */
static BOS_TASK_ID       printQTaskID; /* Stores the ID of the print task */
static STR_BUF_VARS      strVars;      /* Variables for the string buffers */
#endif

static MODULE_VARS       modVars;      /* Variables for the module table */
static PRINTCB_VARS      printCBVars;  /* Variables for the print CB table */
static BLOG_TIMESTAMPCB  getTimeCB;    /* Function called to get timestamps */

static BLOG_MOD_HNDL     gRawHandle;

#if 1
/*
   Used when the BLOG_VAR_ARGS macro requires it (see in blogpriv.h)
*/
char gBlogBuffer[BLOG_CFG_MAX_STR_LEN + 1];
#endif

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------- */

/*****************************************************************************
*
*  FUNCTION:   blogInit
*
*  PURPOSE:    Initializes the blog module.
*
*  PARAMETERS:
*          defaultCB   - Callback used as the default for printing.
*                        NULL to use default internal print function.
*          timestampCB - Callback used for getting timestamp values.
**                       NULL to use default internal timestamp function.
*
*  RETURNS:    BOS_TRUE on success or BOS_FALSE on error
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL blogInit(BLOG_PRINTCB defaultCB, BLOG_TIMESTAMPCB timestampCB)
{

   BOS_STATUS  status;
   int i;

   if ( logInitialized )
   {
      return BOS_TRUE;
   }

   /* Check for default callbacks. */
   if ( defaultCB == NULL )
   {
      defaultCB = blogDefaultPrintCallback;
   }

   if ( timestampCB == NULL )
   {
      timestampCB = blogDefaultTimeStampCallback;
   }


#if BLOG_CFG_QUEUE_LOGS

   /* Check the log queue size */
   XCHG_ASSERT( cbf_isPowerOf2(BLOG_CFG_QSIZE) );
   maxQSize = BLOG_CFG_QSIZE;

   /* Create the mutexes */
   status = bosMutexCreate( "blogStrVars", &strVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );
   status = bosMutexCreate( "blogQVars", &qVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   clearLog();

#endif

   status = bosMutexCreate( "blogModVars", &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );
   status = bosMutexCreate( "blogPrintCBVars", &printCBVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );


   /* Initialize the first module entry as the default
    * Log entries with invalid module handle will be stored using this default
    */
   modVars.numModules = 0;
   for ( i = 0; i < BLOG_CFG_MOD_NAME_LEN; i++ )
   {
      modVars.table[0].modName[i] = ' ';
   }
   modVars.table[0].modName[BLOG_CFG_MOD_NAME_LEN] = '\0';
   modVars.table[0].logSeverity = BLOG_SEVERITY_ALL;
   modVars.table[0].printSeverity = BLOG_SEVERITY_ALL;
   modVars.table[0].displayFields = BLOG_FIELDS_ALL;

   modVars.defaultDisplayFields  = BLOG_FIELDS_ALL;

   modVars.defaultLogSeverity    = BLOG_SEVERITY_FATAL_ERR |
                                   BLOG_SEVERITY_ERR       |
                                   BLOG_SEVERITY_WARN;

   modVars.defaultPrintSeverity  = BLOG_SEVERITY_FATAL_ERR |
                                   BLOG_SEVERITY_ERR       |
                                   BLOG_SEVERITY_WARN;


   /* Add default callback to the table of registered print functions */
   printCBVars.numCB = 1;
   printCBVars.table[0].printCB = defaultCB;
   printCBVars.table[0].isEnabled = BOS_TRUE;

   getTimeCB = timestampCB;

   logInitialized = BOS_TRUE;


   /* Add a "Raw" module. This allows user to log without a module handle. */
   gRawHandle = blogAddModule( "RAW",
                               BLOG_SEVERITY_ALWAYS,
                               BLOG_SEVERITY_ALWAYS,
                               BLOG_FIELDS_NONE );


#if BLOG_CFG_QUEUE_LOGS
   /* Create a task to print the queue */
   if ( bosTaskCreate( "blog", BLOG_CFG_TASK_STACK, BLOG_CFG_TASK_PRTY,
                         printQTask, NULL, &printQTaskID) != BOS_STATUS_OK )
   {
      return BOS_FALSE;
   }
#endif

   return BOS_TRUE;
}

/*****************************************************************************
*
*  FUNCTION:   blogTerm
*
*  PURPOSE:    Terminates the blog module.
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:      After calling blogTerm, blogInit must be called again to use
*              the blog module again
*
*****************************************************************************/
void blogTerm(void)
{
   XCHG_ASSERT( logInitialized );

   logInitialized = BOS_FALSE;

#if BLOG_CFG_QUEUE_LOGS
   bosTaskDestroy( &printQTaskID);
   clearLog();
   bosMutexDestroy( &qVars.mutex );
   bosMutexDestroy( &strVars.mutex );
#endif

   blogClearModules();
   blogClearPrintCB();
   bosMutexDestroy( &modVars.mutex );
   bosMutexDestroy( &printCBVars.mutex );

}

/*****************************************************************************
*
*  FUNCTION:   blogSetDefaultFields
*
*  PURPOSE:    Set the default display fields for all blog modules created with
*              the blogAddDefaultModule API.
*
*  PARAMETERS: displayFields  (in)  Default mask of fields to display when logging.
*
*  RETURNS:    Nothing.
*
*  NOTES:
*
*****************************************************************************/
void blogSetDefaultFields( BLOG_FIELDS displayFields )
{
   XCHG_ASSERT( logInitialized );

   bosMutexAcquire( &modVars.mutex );
   {
      modVars.defaultDisplayFields = displayFields;
   }
   bosMutexRelease( &modVars.mutex );
}


/*****************************************************************************
*
*  FUNCTION:   blogSetDefaultLogSeverity
*
*  PURPOSE:    Set the default logging severity for all blog modules created with
*              the blogAddDefaultModule API.
*
*  PARAMETERS: logSeverity (in)  Default severity logging level.
*
*  RETURNS:    Nothing.
*
*  NOTES:
*
*****************************************************************************/
void blogSetDefaultLogSeverity( BLOG_SEVERITY logSeverity )
{
   XCHG_ASSERT( logInitialized );

   bosMutexAcquire( &modVars.mutex );
   {
      modVars.defaultLogSeverity = logSeverity;
   }
   bosMutexRelease( &modVars.mutex );
}

/*****************************************************************************
*
*  FUNCTION:   blogSetDefaultLogSeverity
*
*  PURPOSE:    Set the default print severity for all blog modules created with
*              the blogAddDefaultModule API.
*
*  PARAMETERS: printSeverity (in)  Default severity print level.
*
*  RETURNS:    Nothing.
*
*  NOTES:
*
*****************************************************************************/
void blogSetDefaultPrintSeverity( BLOG_SEVERITY printSeverity )
{
   XCHG_ASSERT( logInitialized );

   bosMutexAcquire( &modVars.mutex );
   {
      modVars.defaultPrintSeverity = printSeverity;
   }
   bosMutexRelease( &modVars.mutex );
}

/*****************************************************************************
*
*  FUNCTION:   blogAddDefaultModule
*
*  PURPOSE:    Registers a module and assigns a handle to the module. The
*              default severities and field display settings are used.
*
*  PARAMETERS:
*          modNameStrp - module name with length BLOG_CFG_MOD_NAME_LEN
*
*  RETURNS:    Module handle greater than 0 if successful
*              0 if not successful
*
*  NOTES:      The maximum number of modules that can be added is defined by
*              BLOG_CFG_MAX_MODULES
*
*****************************************************************************/
BLOG_MOD_HNDL blogAddDefaultModule( const char *modNameStrp )
{
   return ( blogAddModule( modNameStrp,
                           modVars.defaultLogSeverity,
                           modVars.defaultPrintSeverity,
                           modVars.defaultDisplayFields ) );
}

/*****************************************************************************
*
*  FUNCTION:   blogAddModule
*
*  PURPOSE:    Registers a module and assigns a handle to the module
*
*  PARAMETERS:
*          modNameStrp - module name with length BLOG_CFG_MOD_NAME_LEN
*          logSeverity - the severity mask for logging msgs
*          printSeverity - the severity mask for printing msgs
*          displayFields - mask for selecting which fields to display
*
*  RETURNS:    Module handle greater than 0 if successful
*              0 if not successful
*
*  NOTES:      The maximum number of modules that can be added is defined by
*              BLOG_CFG_MAX_MODULES
*
*****************************************************************************/
BLOG_MOD_HNDL blogAddModule
(
   const char    *modNameStrp,
   BLOG_SEVERITY  logSeverity,
   BLOG_SEVERITY  printSeverity,
   BLOG_FIELDS    displayFields
)
{
   BOS_STATUS status;
   BLOG_MOD_HNDL newHndl;

   XCHG_ASSERT( logInitialized );

   status = bosMutexAcquire( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   if ( modVars.numModules == BLOG_CFG_MAX_MODULES )
   {
      /* max number of modules registered already */
      newHndl = 0;
   }
   else
   {
      /* copy the module entry */
      modVars.numModules++;
      modVars.table[modVars.numModules].logSeverity = logSeverity;
      modVars.table[modVars.numModules].printSeverity = printSeverity;
      modVars.table[modVars.numModules].displayFields = displayFields;
      modVars.table[modVars.numModules].bAddEndOfLine = BOS_FALSE;
      strMaxCpy( &( modVars.table[modVars.numModules].modName[0] ),
                     modNameStrp, (BLOG_CFG_MOD_NAME_LEN + 1) );
      newHndl = modVars.numModules;
   }

   status = bosMutexRelease( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   return newHndl;
}


/*****************************************************************************
*
*  FUNCTION:   blogRemoveModule
*
*  PURPOSE:    De-registers a module.
*
*  PARAMETERS: handle - Handle of module to remove.
*
*  RETURNS: void
*
*  NOTES:
*
*****************************************************************************/
void blogRemoveModule( BLOG_MOD_HNDL handle )
{
   (void) handle;  /* Make the compiler happy */

#if 0
   /* Implement me. */
   XCHG_ASSERT( 0 );
#endif
}


/*****************************************************************************
*
*  FUNCTION:   blogSetModule
*
*  PURPOSE:    Changes the settings for a registered module.
*
*  PARAMETERS:
*          moduleHandle - handle for module to set
*          logSeverity - the severity mask for logging msgs
*          printSeverity - the severity mask for printing msgs
*          displayFields - mask for selecting which fields to display
*
*  RETURNS:    BOS_TRUE if successful
*              BOS_FALSE if not successful
*
*  NOTES:      Use BLOG_MODULES_ALL as the handle to set all modules
*
*****************************************************************************/
BOS_BOOL blogSetModule
(
   BLOG_MOD_HNDL  moduleHandle,
   BLOG_SEVERITY  logSeverity,
   BLOG_SEVERITY  printSeverity,
   BLOG_FIELDS    displayFields
)
{

   BOS_STATUS status;
   BOS_BOOL   result = BOS_TRUE;

   XCHG_ASSERT( logInitialized );

   status = bosMutexAcquire( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   if ( moduleHandle == BLOG_MODULES_ALL )
   {
      /* Loop through the module table and update each one */
      int index;
      for ( index = 1; index <= modVars.numModules; index++ )
      {
         if ( logSeverity != BLOG_SEVERITY_INVALID )
         {
            modVars.table[index].logSeverity = logSeverity;
         }

         if ( printSeverity != BLOG_SEVERITY_INVALID )
         {
            modVars.table[index].printSeverity = printSeverity;
         }

         if ( displayFields != BLOG_FIELDS_INVALID )
         {
            modVars.table[index].displayFields = displayFields;
         }
      }

   }
   else if ( (moduleHandle < 1) || (moduleHandle > modVars.numModules) )
   {
      /* Unregistered module handle */
      result = BOS_FALSE;
   }
   else
   {
      if ( logSeverity != BLOG_SEVERITY_INVALID )
      {
         modVars.table[moduleHandle].logSeverity = logSeverity;
      }

      if ( printSeverity != BLOG_SEVERITY_INVALID )
      {
         modVars.table[moduleHandle].printSeverity = printSeverity;
      }

      if ( displayFields != BLOG_FIELDS_INVALID )
      {
         modVars.table[moduleHandle].displayFields = displayFields;
      }
   }

   status = bosMutexRelease( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   return result;

}

/*****************************************************************************
*
*  FUNCTION:   blogGetModule
*
*  PURPOSE:    Returns the paramters for the provided module handle
*
*  PARAMETERS:
*          moduleHandle - handle for module to get
*          modNameBufp  - buffer for module name.  It is assumed that
*                         the buffer of size BLOG_CFG_MOD_NAME_LEN is already
*                         allocated.
*          logSeverity   - the severity mask for logging msgs
*          printSeverity - the severity mask for printing msgs
*          displayFields - mask for selecting which fields to display
*
*  RETURNS:    BOS_TRUE if successful
*              BOS_FALSE if not successful
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL blogGetModule
(
   BLOG_MOD_HNDL  moduleHandle,
   char          *modNameBufp,
   BLOG_SEVERITY *logSeverity,
   BLOG_SEVERITY *printSeverity,
   BLOG_FIELDS   *displayFields
)
{
   BOS_STATUS status;
   BOS_BOOL   result = BOS_TRUE;

   XCHG_ASSERT( logInitialized );

   status = bosMutexAcquire( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   if ( (moduleHandle < 1) || (moduleHandle > modVars.numModules) )
   {
      /* Unregistered module handle */
      result = BOS_FALSE;
   }
   else
   {
      if ( modNameBufp != NULL )
      {
         strMaxCpy( modNameBufp,
                    &( modVars.table[modVars.numModules].modName[0] ),
                     (BLOG_CFG_MOD_NAME_LEN + 1) );
      }

      if ( logSeverity != NULL )
      {
         *logSeverity = modVars.table[moduleHandle].logSeverity;
      }

      if ( printSeverity != NULL )
      {
         *printSeverity = modVars.table[moduleHandle].printSeverity;
      }

      if ( displayFields != NULL )
      {
         *displayFields = modVars.table[moduleHandle].displayFields;
      }
   }

   status = bosMutexRelease( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   return result;

}

/*****************************************************************************
*
*  FUNCTION:   blogEnableAutoEndOfLineSupport
*
*  PURPOSE:    End-of-line characters may be added automatically to all logs
*              by enabling this feature. Use of this feature is not encouraged
*              and it is disabled by default. This provides users with the
*              the flexibility to decide whether a newline is required or not
*              for each log statement (as well as providing a consistent interface
*              with printf). There are numerous cases where a newline
*              is not desirable. This function has only been provided to allow
*              backwards compatibility support for existing blog macros that
*              assume a newline will be automatically added.
*
*  PARAMETERS: enable   - BOS_TRUE or BOS_FALSE
*
*  RETURNS:    Nothing.
*
*  NOTES:
*
*****************************************************************************/
void blogEnableAutoEndOfLine( BLOG_MOD_HNDL moduleHandle, BOS_BOOL enable )
{
   if (moduleHandle == 0)    /* reserved for default handle  gRawHandle */
   {
      moduleHandle = gRawHandle;
   }
   if ( (moduleHandle < 1) || (moduleHandle > modVars.numModules) )


   XCHG_ASSERT( logInitialized );
   XCHG_ASSERT_COND( moduleHandle, >= ,1);
   XCHG_ASSERT_COND( moduleHandle, <= ,modVars.numModules );

   bosMutexAcquire( &modVars.mutex );
   {
      modVars.table[ moduleHandle ].bAddEndOfLine = enable;
   }
   bosMutexRelease( &modVars.mutex );
}

/*****************************************************************************
*
*  FUNCTION:   blogPrintModules
*
*  PURPOSE:    Logs all registered entries in the module table to the queue
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void blogPrintModules(void)
{
   int i;

   blogStrRaw(( "BLOG: Registered Modules:"));
   for (i = 0; i < (modVars.numModules + 1); i++)
   {
      blogStrRaw(( "BLOG: (%d) %s: logSev=0x%x, printSev=0x%x, disp=0x%x",
                   i,
                   modVars.table[i].modName,
                   modVars.table[i].logSeverity,
                   modVars.table[i].printSeverity,
                   modVars.table[i].displayFields ));
   }
}

/*****************************************************************************
*
*  FUNCTION:   blogClearModules
*
*  PURPOSE:    Removes all registered modules
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void blogClearModules(void)
{

   BOS_STATUS status;
   status = bosMutexAcquire( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );
   modVars.numModules = 0;
   status = bosMutexRelease( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );
}

/*****************************************************************************
*
*  FUNCTION:   blogAddPrintCB
*
*  PURPOSE:    Registers a callback function for printing
*
*  PARAMETERS:
*          printCB - callback function
*          enable  - initial enable/disable print setting for this cb
*
*  RETURNS:    Callback handle, or 0 if could not add the callback.
*
*  NOTES:      A maximum of BLOG_CFG_MAX_PRINTCB callbacks can be registered
*
*****************************************************************************/
BLOG_CB_HNDL blogAddPrintCB(BLOG_PRINTCB printCB, BOS_BOOL enable)
{
   BLOG_CB_HNDL newHndl;
   BOS_STATUS status;

   XCHG_ASSERT( logInitialized );

   status = bosMutexAcquire( &printCBVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   if ( printCBVars.numCB < BLOG_CFG_MAX_PRINTCB )
   {
      printCBVars.table[printCBVars.numCB].printCB = printCB;
      printCBVars.table[printCBVars.numCB].isEnabled = enable;
      newHndl = printCBVars.numCB;
      printCBVars.numCB++;
   }
   else
   {
      newHndl = 0;
   }

   status = bosMutexRelease( &printCBVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   return newHndl;
}

/*****************************************************************************
*
*  FUNCTION:   blogEnablePrintCB
*
*  PURPOSE:    enable/disable registered print callbacks
*
*  PARAMETERS:
*          cbHandle - handle for callback function
*          enable   -  enable/disable print setting for this cb
*
*  RETURNS:    BOS_TRUE if successful
*              BOS_FALSE if not successful
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL blogEnablePrintCB(BLOG_CB_HNDL cbHandle, BOS_BOOL enable)
{

   BOS_STATUS status;
   BOS_BOOL   result = BOS_TRUE;

   XCHG_ASSERT( logInitialized );

   status = bosMutexAcquire( &printCBVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   if ( cbHandle == BLOG_PRINTCB_ALL )
   {
      /* Loop through the table and enable/disable each one */
      int index;
      for ( index = 0; index < printCBVars.numCB; index++ )
      {
         printCBVars.table[index].isEnabled = enable;
      }
   }
   else if ( (cbHandle >= 0) && (cbHandle < printCBVars.numCB) )
   {
      printCBVars.table[cbHandle].isEnabled = enable;
   }
   else
   {
      result = BOS_FALSE;
   }

   status = bosMutexRelease( &printCBVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   return result;
}

/*****************************************************************************
*
*  FUNCTION:   blogPrintCBStats
*
*  PURPOSE:    Logs all registered entries in the print callback table to
*              the queue
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void blogPrintCBStats(void)
{
   int i;

   blogStrRaw(( 0, "BLOG: Registered Print Callbacks:" ));
   for (i = 0; i < (printCBVars.numCB); i++)
   {
      blogStrRaw(( 0, "BLOG: CB #%d, isEnabled=%d",
                        i, printCBVars.table[i].isEnabled ));
   }
}

/*****************************************************************************
*
*  FUNCTION:   blogClearPrintCB
*
*  PURPOSE:    Removes all registered print callbacks INCLUDING the default.
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void blogClearPrintCB(void)
{

   BOS_STATUS status;
   status = bosMutexAcquire( &printCBVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );
   printCBVars.numCB = 0;
   status = bosMutexRelease( &printCBVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );
}

#if BLOG_CFG_QUEUE_LOGS
/*****************************************************************************
*
*  FUNCTION:   blogPurge
*
*  PURPOSE:    To dump and clear the entire log queue.
*              Severities and field masks are ignored.
*              If no print callbacks are enabled, the default is used.
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void blogPurge( void )
{
   BOS_STATUS status;
   Q_ENTRY currentLog;
   int numLogs;
   int index;

   XCHG_ASSERT( logInitialized );

   status = bosMutexAcquire( &(qVars.mutex) );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   numLogs = cbf_qSize( qVars.writeOffset, qVars.readOffset );

   if ( numLogs > 0 )
   {
      /* Print each queue entry */
      for ( index = 0; index < numLogs; index++ )
      {

         currentLog = *cbf_qPointer( maxQSize,
                                      qVars.logQueue, qVars.readOffset );
         qVars.readOffset++;
         printSingleLog( &currentLog, BOS_TRUE );
      }
   }

   clearLog();

   status = bosMutexRelease( &(qVars.mutex) );
   XCHG_ASSERT( status == BOS_STATUS_OK );

}
#endif

/*****************************************************************************
*
*  FUNCTION:   blogConstString
*
*  PURPOSE:    Creates a log entry with a constant message string and adds it
*              to the queue.
*
*  PARAMETERS:
*          module - handle for calling module
*          severity  - severity assigned to the log message
*          string - Message string.  The string is assumed to be
*                   constant in memory and is not copied locally.
*          param1, param2, param3 - The printf style parameters
*
*  RETURNS:    Nothing
*
*  NOTES:      Used by macro definitions for const strings.
*              Should not be called directly.
*
*****************************************************************************/
void blogConstString
(
   BLOG_MOD_HNDL  module,
   BLOG_DEBUG_PARMS_DECL
   BLOG_SEVERITY  severity,
   const char    *string,
   BLOG_PARAM     param1,
   BLOG_PARAM     param2,
   BLOG_PARAM     param3
)
{
   Q_ENTRY newEntry;
   BOS_STATUS   status;

   XCHG_ASSERT( logInitialized );

   /* Check the module handle - if invalid set log handle to 0 */
   status = bosMutexAcquire( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   if ( (module < 1) || (module > modVars.numModules) )
   {
      newEntry.modHandle = 0;
   }
   else
   {
      newEntry.modHandle = module;
   }


#if BLOG_CFG_QUEUE_LOGS
   /* Check the logSeverity for the module */
   if ( ( severity & ( modVars.table[ newEntry.modHandle ].logSeverity )) != severity )
   {
      /* Do not add the log to the queue */
      status = bosMutexRelease( &modVars.mutex );
      XCHG_ASSERT( status == BOS_STATUS_OK );
      return;
   }
#endif

   status = bosMutexRelease( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   /* Create the log entry */
   getTimeCB( &(newEntry.timestamp) );
   newEntry.severity = severity;
#if BLOG_CFG_GET_THREAD
   bosTaskGetMyTaskId( &(newEntry.taskId));
#endif
   newEntry.msgStrp = string;
   newEntry.param1 = param1;
   newEntry.param2 = param2;
   newEntry.param3 = param3;
   newEntry.isConstStr = BOS_TRUE;
   BLOG_DEBUG_SET_INFO( &newEntry );

#if BLOG_CFG_QUEUE_LOGS
   addToQueue( &newEntry );

#else

   /* Print the log synchronously */
   printSingleLog( &newEntry, BOS_FALSE );

#endif

}

/*****************************************************************************
*
*  FUNCTION:   blogString
*
*  PURPOSE:    Creates a log entry with a non-const message string and adds it
*              to the queue.
*
*  PARAMETERS:
*          module - handle for calling module
*          severity  - severity assigned to the log message
*          format - message string
*          bDisplayFields  - display format fields; true or false.
*          argList - associated arguments
*
*  RETURNS:    Nothing
*
*  NOTES:      Used by macro definitions for non-const strings.
*              Should not be called directly.
*
*****************************************************************************/
void blogString
(
   BLOG_MOD_HNDL  module,
   BLOG_DEBUG_PARMS_DECL
   BLOG_SEVERITY  severity,
   const char    *format,
   BOS_BOOL       bDisplayFields,
   va_list        argList
)
{

   Q_ENTRY newEntry;
   int msgLen;
   BOS_STATUS status;

#if BLOG_CFG_QUEUE_LOGS
   BOS_BOOL logFullMsg = BOS_FALSE;
#else
   char strBufp[ BLOG_CFG_MAX_STR_LEN+1 ];
#endif

   XCHG_ASSERT( logInitialized );

   status = bosMutexAcquire( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   /* Check the module handle - if invalid set log handle to 0 */
   if ( (module < 1) || (module > modVars.numModules) )
   {
      newEntry.modHandle = 0;
   }
   else
   {
      newEntry.modHandle = module;
   }

#if BLOG_CFG_QUEUE_LOGS
   /* Check the logSeverity for the module */
   if ( ( severity & ( modVars.table[ newEntry.modHandle ].logSeverity )) != severity )
   {
      /* Do not add the log to the queue */
      status = bosMutexRelease( &modVars.mutex );
      XCHG_ASSERT( status == BOS_STATUS_OK );
      return;
   }
#endif


   status = bosMutexRelease( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   /* Create the log entry */
   getTimeCB( &(newEntry.timestamp) );
   newEntry.severity = severity;
#if BLOG_CFG_GET_THREAD
   bosTaskGetMyTaskId( &(newEntry.taskId));
#endif
   newEntry.param1 = 0;
   newEntry.param2 = 0;
   newEntry.param3 = 0;
   newEntry.isConstStr = BOS_FALSE;
   newEntry.bDisplayFields = bDisplayFields;
   BLOG_DEBUG_SET_INFO( &newEntry );

#if BLOG_CFG_QUEUE_LOGS

   /* Lock the string buffer mutex */
   status = bosMutexAcquire( &(strVars.mutex) );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   if ( strVars.numStrLogs < BLOG_CFG_NUM_STR_BUFFERS )
   {
      /* There is a free string buffer */
      (strVars.numStrLogs)++;
   }

   else if ( strVars.strBufFull == BOS_FALSE )
   {
      /* No free string buffers - need to log a warning message */
      logFullMsg = BOS_TRUE;
      strVars.strBufFull = BOS_TRUE;
   }

   /* Print the message to the next local buffer */
   msgLen = vStrPrintf( strVars.strBufpp[ strVars.strBufIndex ],
                         BLOG_CFG_MAX_STR_LEN+1,
                          format, argList);
   /*XCHG_ASSERT(msgLen > 0);*/
   newEntry.msgStrp = strVars.strBufpp[ strVars.strBufIndex ];

   /* Increment the string buffer index and check for wrap condition */
   strVars.strBufIndex++;
   if ( strVars.strBufIndex >= BLOG_CFG_NUM_STR_BUFFERS )
   {
      strVars.strBufIndex = 0;
   }

   /* Unlock the string buffer mutex */
   status = bosMutexRelease( &(strVars.mutex) );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   if ( logFullMsg == BOS_TRUE )
   {
      /* The string buffer is full - log an error using module handle 0
       * which has all severities turned on so logs always print
       * Only one error is logged until the entire queue has been printed
       * New logs will overwrite old ones, even if they haven't been read
       * yet so the new logs may appear twice in the log queue.
       */
      Q_ENTRY errorLog;
      errorLog.timestamp = newEntry.timestamp;
      errorLog.modHandle = 0;
      errorLog.severity = BLOG_SEVERITY_ERR;
#if BLOG_CFG_GET_THREAD
      errorLog.taskId = newEntry.taskId;
#endif
      errorLog.msgStrp = "BLOG: NO FREE STRING BUFFERS - OVERWRITING OLD LOGS";
      errorLog.param1 = 0;
      errorLog.param2 = 0;
      errorLog.param3 = 0;
      errorLog.isConstStr = BOS_TRUE;
      addToQueue(&errorLog);
   }

   /* Log the new message */
   addToQueue(&newEntry);

#else

   /* Print the log synchronously */
   msgLen = vStrPrintf(strBufp, BLOG_CFG_MAX_STR_LEN+1, format, argList);
   XCHG_ASSERT(msgLen > 0);
   newEntry.msgStrp = strBufp;
   printSingleLog(&newEntry, BOS_FALSE);

#endif /* BLOG_CFG_QUEUE_LOGS */

}

/*****************************************************************************
*
*  FUNCTION:   blogStrErr/Warn/Info/Always/Fnc/UsrFunc
*
*  PURPOSE:    Functions used in macro definitions.
*              Should not be called directly.
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/

void blogStrFatalErrFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_FATAL_ERR, BOS_TRUE );
}

void blogStrErrFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_ERR, BOS_TRUE );
}

void blogStrWarnFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_WARN, BOS_TRUE );
}

void blogStrInitFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_INITIALIZATION, BOS_TRUE );
}

void blogStrInfoFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_INFO, BOS_TRUE );
}

void blogStrAlwaysFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_ALWAYS, BOS_TRUE );
}

void blogStrFncFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_FNC_ENTRY_EXIT, BOS_TRUE );
}

void blogStrUsrFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL BLOG_SEVERITY usrSeverity,
                      const char *format, ...)
{
   BLOG_VAR_ARGS( usrSeverity, BOS_TRUE );
}

void blogStrFatalErrNoFieldsFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_FATAL_ERR, BOS_FALSE );
}

void blogStrErrNoFieldsFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_ERR, BOS_FALSE );
}

void blogStrWarnNoFieldsFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_WARN, BOS_FALSE );
}

void blogStrInitNoFieldsFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_INITIALIZATION, BOS_FALSE );
}

void blogStrInfoNoFieldsFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_INFO, BOS_FALSE );
}

void blogStrAlwaysNoFieldsFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_ALWAYS, BOS_FALSE );
}

void blogStrFncNoFieldsFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   BLOG_VAR_ARGS( BLOG_SEVERITY_FNC_ENTRY_EXIT, BOS_FALSE );
}

void blogStrUsrNoFieldsFunc(BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL BLOG_SEVERITY usrSeverity,
                      const char *format, ...)
{
   BLOG_VAR_ARGS( usrSeverity, BOS_FALSE );
}

void blogStrRawFunc( const char *format, ... )
{
   const char    *function = NULL;
   BLOG_MOD_HNDL  module   = gRawHandle;

   BLOG_VAR_ARGS( BLOG_SEVERITY_ALWAYS, BOS_FALSE );
}

/*blog functions with variable list parameter*/
void blogVStrFatalErrFunc( BLOG_MOD_HNDL module, const char *format, va_list arg)
{
   const char    *function = NULL;
   blogString( module, BLOG_DEBUG_PARMS (BLOG_SEVERITY_FATAL_ERR), format, (BOS_TRUE), arg );
}

void blogVStrErrFunc( BLOG_MOD_HNDL module, const char *format, va_list arg)
{
   const char    *function = NULL;
   blogString( module, BLOG_DEBUG_PARMS (BLOG_SEVERITY_ERR), format, (BOS_TRUE), arg );
}

void blogVStrWarnFunc( BLOG_MOD_HNDL module, const char *format, va_list arg)
{
   const char    *function = NULL;
   blogString( module, BLOG_DEBUG_PARMS (BLOG_SEVERITY_WARN), format, (BOS_TRUE), arg );
}

void blogVStrInitFunc( BLOG_MOD_HNDL module, const char *format, va_list arg)
{
   const char    *function = NULL;
   blogString( module, BLOG_DEBUG_PARMS (BLOG_SEVERITY_INITIALIZATION), format, (BOS_TRUE), arg );
}

void blogVStrInfoFunc( BLOG_MOD_HNDL module, const char *format, va_list arg)
{
   const char    *function = NULL;
   blogString( module, BLOG_DEBUG_PARMS (BLOG_SEVERITY_INFO), format, (BOS_TRUE), arg );
}

void blogVStrAlwaysFunc( BLOG_MOD_HNDL module, const char *format, va_list arg)
{
   const char    *function = NULL;
   blogString( module, BLOG_DEBUG_PARMS (BLOG_SEVERITY_ALWAYS), format, (BOS_TRUE), arg );
}

void blogVStrFncFunc( BLOG_MOD_HNDL module, const char *format, va_list arg)
{
   const char    *function = NULL;
   blogString( module, BLOG_DEBUG_PARMS (BLOG_SEVERITY_FNC_ENTRY_EXIT), format, (BOS_TRUE), arg );
}

void blogVStrUsrFunc( BLOG_MOD_HNDL module, BLOG_SEVERITY usrSeverity, const char *format, va_list arg)
{
   const char    *function = NULL;
   blogString( module, BLOG_DEBUG_PARMS (usrSeverity), format, (BOS_TRUE), arg );
}

void nullFunc (BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...)
{
   va_list argList;

   (void) module;
   va_start (argList, format);
   va_end (argList);
}

/*
*****************************************************************************
** FUNCTION:   blogHexBufferFunc
**
** PURPOSE:    Log an array of bytes in ASCII hex format.
**
** PARAMETERS: bytes          - input - array of bytes to log to the console
**             numberOfBytes  - input - size of the array in bytes
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
void blogHexBufferFunc( const BOS_UINT8 *bytes, unsigned int numberOfBytes )
{
   /* This probably shouldn't be declared on the stack. If the user sets this
   ** to a large value, it's likely that we will blow the thread stack. */
   char           outputBuf[ BLOG_CFG_MAX_STR_LEN ];

   unsigned int   offset = 0;

#define OUTPUT_WRITE_PTR   &outputBuf[ strlen( outputBuf ) ]
#define OUTPUT_MAX_LEN     ( BLOG_CFG_MAX_STR_LEN - strlen( outputBuf ) )

   outputBuf[0] = '\0';


   if ( bytes != NULL )
   {
      unsigned int i;
      char asciiRep[17];
      unsigned int lineOffset = 0;

      /* Fill in the end of the ASCII representation for printing. */
      asciiRep[16] = '\0';

      for (i = 0; i < numberOfBytes; i++)
      {
         /* Every 4 bytes, print an extra couple of spaces, to make the
         ** dwords stand out. */
         if( ((i & 0x03) == 0) && !((i == 0) && (numberOfBytes <= 8)) )
         {
            /* every 4 bytes (except for special case of first byte when
            ** total bytes to print is <= 8), print an extra couple of
            ** spaces, to make the dwords stand out. */
            strPrintf( OUTPUT_WRITE_PTR, OUTPUT_MAX_LEN, "  " );
         }

         strPrintf( OUTPUT_WRITE_PTR, OUTPUT_MAX_LEN, "%02x ", bytes[offset + i] );


         /* Build the ASCII rep for this line. */
         asciiRep[lineOffset] = (char) bytes[offset + i];
         /* Do not use isprint since then we need to include ctype.h  */
         if ((asciiRep[lineOffset] < ' ') || (asciiRep[lineOffset] > '~'))
         {
            asciiRep[lineOffset] = '.';
         }

         lineOffset++;

         /* Print the ASCII rep and a newline after 16 bytes have been
         ** printed. */
         if ((i & 0x0f) == 0x0f)
         {
            strPrintf( OUTPUT_WRITE_PTR, OUTPUT_MAX_LEN, " | %s\n", asciiRep );

            lineOffset = 0;
         }
      }

      /* Print the ASCII rep and a newline if we didn't just do one. */
      if ((i & 0x0f) != 0x00)
      {
         /* If we printed more than one line, then we need to pad out to
         ** the end of the line so that the ASCII rep lines up correctly. */
         if (numberOfBytes > 16)
         {
            i &= 0x0f;

            while (i < 16)
            {
               if ((i & 0x03) == 0)
               {
                  strPrintf( OUTPUT_WRITE_PTR, OUTPUT_MAX_LEN, "  " );
               }

               strPrintf( OUTPUT_WRITE_PTR, OUTPUT_MAX_LEN, "   " );

               i++;
            }
         }

         /* Truncate the ASCII rep string. */
         asciiRep[lineOffset] = '\0';

         /* Print it. */
         strPrintf( OUTPUT_WRITE_PTR, OUTPUT_MAX_LEN, " | %s\n", asciiRep );
      }


      /* If the output buffer has been truncated, then over-write the last
      ** character with a newline. */
      if ( strlen( outputBuf ) == ( BLOG_CFG_MAX_STR_LEN - 1 ) )
      {
         blogStrRaw(( "WARNING: Hex buffer truncated!\n" ));
         outputBuf[ BLOG_CFG_MAX_STR_LEN - 2 ] = '\n';
      }


      /* Display formatted output. */
      blogStrRaw(( "%s", outputBuf ));
   }
}



#if BLOG_CFG_QUEUE_LOGS
/*****************************************************************************
*
*  FUNCTION:   addToQueue
*
*  PURPOSE:    Adds the initialized log entry to the queue if the queue is
*              not full.
*
*  PARAMETERS:
*          logEntry - the initialized log to add to the queue
*
*  RETURNS:
*
*  NOTES:      If the queue is full, the log will still be queued but will
*              overwrite the oldest entry in the queue.
*
*****************************************************************************/
void addToQueue(Q_ENTRY* logEntry)
{
   BOS_STATUS status;

   XCHG_ASSERT(logInitialized);

   /* Lock the queue variables mutex */
   status = bosMutexAcquire( &(qVars.mutex) );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   if ( cbf_qError( maxQSize, (qVars.writeOffset)+1, qVars.readOffset ) )
   {

      /* The queue is full.
       * The current message will still be logged, but it will overwrite the
       * oldest entry in the queue.  An error will also be logged.
       */

      if ( !(qVars.queueFull) )
      {
         Q_ENTRY errorLog;

         errorLog.timestamp = (*logEntry).timestamp;
         errorLog.modHandle = 0;
         errorLog.severity = BLOG_SEVERITY_ERR;
#if BLOG_CFG_GET_THREAD
         errorLog.taskId = (*logEntry).taskId;
#endif
         errorLog.msgStrp = "BLOG:  LOG QUEUE IS FULL - OVERWRITING OLD LOGS";
         errorLog.param1 = 0;
         errorLog.param2 = 0;
         errorLog.param3 = 0;
         errorLog.isConstStr = BOS_TRUE;
         *cbf_qPointer( maxQSize, qVars.logQueue,
                         (qVars.writeOffset)++ ) = errorLog;
         qVars.readOffset++;
         qVars.queueFull = BOS_TRUE;
      }

      /* Add the log entry to the queue,
         increment both queue pointers to overwrite the oldest entry */
      *cbf_qPointer(maxQSize, qVars.logQueue,
                         (qVars.writeOffset)++) = *logEntry;
      qVars.readOffset++;

   }
   else
   {
      /* The queue is not full, add the log message */
      *cbf_qPointer(maxQSize, qVars.logQueue,
                         (qVars.writeOffset)++) = *logEntry;
   }

   /* Unlock the queue variables mutex */
   status = bosMutexRelease( &(qVars.mutex) );
   XCHG_ASSERT( status == BOS_STATUS_OK );

}
#endif /* BLOG_CFG_QUEUE_LOGS */


#if BLOG_CFG_QUEUE_LOGS
/*****************************************************************************
*
*  FUNCTION:   printQTask
*
*  PURPOSE:    Continuously prints the entire log queue.
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void printQTask(void* context)
{

   BOS_STATUS status;
   Q_ENTRY currentLog;
   int i;
   int numLogs;
   int numStrLogsPrinted = 0;

   XCHG_ASSERT(logInitialized);

   (void) context;

   while (1)
   {

      /* Get the queue size */
      status = bosMutexAcquire( &(qVars.mutex) );
      XCHG_ASSERT( status == BOS_STATUS_OK );
      numLogs = cbf_qSize(qVars.writeOffset, qVars.readOffset);
      status = bosMutexRelease( &(qVars.mutex) );
      XCHG_ASSERT( status == BOS_STATUS_OK );

      if ( numLogs > 0 )
      {

         /* Go through the queue only if at least 1 print CB
          * function is enabled */
         BOS_BOOL foundEnCB = BOS_FALSE;
         int cbIndex;
         status = bosMutexAcquire( &(printCBVars.mutex) );
         XCHG_ASSERT( status == BOS_STATUS_OK );
         for ( cbIndex = 0; cbIndex < printCBVars.numCB; cbIndex++ )
         {
            if ( printCBVars.table[cbIndex].isEnabled == BOS_TRUE )
            {
               foundEnCB = BOS_TRUE;
               break;
            }
         }
         status = bosMutexRelease( &(printCBVars.mutex) );
         XCHG_ASSERT( status == BOS_STATUS_OK );

         if ( foundEnCB == BOS_TRUE )
         {

            /* Print each queue entry */
            for ( i = 0; i < numLogs; i++ )
            {

               status = bosMutexAcquire( &(qVars.mutex) );
               XCHG_ASSERT( status == BOS_STATUS_OK );

               currentLog = *cbf_qPointer(maxQSize,
                                           qVars.logQueue, qVars.readOffset);
               qVars.readOffset++;

               status = bosMutexRelease( &(qVars.mutex) );
               XCHG_ASSERT( status == BOS_STATUS_OK );

               printSingleLog(&currentLog, BOS_FALSE);

               /* Count the number of string logs printed to track the number
                  of string buffers free */
               if ( (currentLog.isConstStr) == BOS_FALSE )
               {
                  numStrLogsPrinted++;
               }

            }

            /* Update the queue and string buffer flags */

            status = bosMutexAcquire( &(qVars.mutex) );
            XCHG_ASSERT( status == BOS_STATUS_OK );
            qVars.queueFull = BOS_FALSE;
            status = bosMutexRelease( &(qVars.mutex) );
            XCHG_ASSERT( status == BOS_STATUS_OK );

            status = bosMutexAcquire( &(strVars.mutex) );
            XCHG_ASSERT( status == BOS_STATUS_OK );
            strVars.strBufFull = BOS_FALSE;
            strVars.numStrLogs -= numStrLogsPrinted;
            if ( strVars.numStrLogs < 0 )
            {
               /* The buffers must have been reused because all string buffers
                * were full */
               strVars.numStrLogs = 0;
            }
            status = bosMutexRelease( &(strVars.mutex) );
            XCHG_ASSERT( status == BOS_STATUS_OK );

         }

      } /* end if numCB > 0 */

      bosSleep(BLOG_CFG_SLEEP_TIME);
   }

}
#endif

/*****************************************************************************
*
*  FUNCTION:   printSingleLog
*
*  PURPOSE:    Checks the severity of the log and prints if necessary.
*              Formats the log entry before calling the printCB functions.
*
*  PARAMETERS:
*           log - the log entry to print
*           purgeLog - flag to print without filtering
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void printSingleLog(Q_ENTRY* log, BOS_BOOL purgeLog)
{

   char logBufp[BLOG_CFG_MAX_STR_LEN+1];
   BOS_UINT16  logLen = 0;
   int  index;
   int  startIndex = 0;
   BOS_STATUS status;
   MOD_SEV_TYPE printSev;
   MOD_DIS_TYPE displayFields;
   BOS_BOOL bAddEndOfLine;

   XCHG_ASSERT( logInitialized );

   logBufp[0] = '\0';

   /* Get the module print and display fields */
   status = bosMutexAcquire( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );
   printSev = modVars.table[log->modHandle].printSeverity;
   displayFields = modVars.table[log->modHandle].displayFields;
   bAddEndOfLine = modVars.table[log->modHandle].bAddEndOfLine;
   status = bosMutexRelease( &modVars.mutex );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   if ( purgeLog == BOS_TRUE )
   {
      /* Print all logs regardless of severity and all fields */
      printSev = BLOG_SEVERITY_ALL;
      displayFields = BLOG_FIELDS_ALL;
   }

   if ( log->bDisplayFields == BOS_FALSE )
   {
      displayFields = BLOG_FIELDS_NONE;
   }

   /* Compare log severity to the print severity for the module
      and if enabled, selectively print each field */
   if ( ( (log->severity) & printSev ) == (log->severity) )
   {

      if ( BLOG_FIELDS_TIME & displayFields )
      {
         logLen += formatTime( &(log->timestamp), logBufp );
      }

#if BLOG_CFG_GET_THREAD
      if ( BLOG_FIELDS_THREAD & displayFields )
      {
         logLen += strPrintf( &(logBufp[logLen]),
                              BLOG_CFG_MAX_STR_LEN - logLen - 1, " [" );
         status = bosTaskGetNameFromId( &(log->taskId), &(logBufp[logLen]),
                              BLOG_CFG_MAX_STR_LEN - logLen - 1 );

         logLen = strlen( logBufp );
         logLen += strPrintf( &(logBufp[logLen]),
                              BLOG_CFG_MAX_STR_LEN - logLen - 1, "] " );
      }
#endif


#if BLOG_CFG_FUNCTION_FIELD_ENABLED
      if ( BLOG_FIELDS_FUNCTION & displayFields )
      {
         logLen += strPrintf(&(logBufp[logLen]),
                             BLOG_CFG_MAX_STR_LEN - logLen - 1,
                             "%s: ", log->functionName );
      }
#endif


      if ( BLOG_FIELDS_MODULE & displayFields )
      {
         logLen += strPrintf(&(logBufp[logLen]),
                             BLOG_CFG_MAX_STR_LEN - logLen - 1,
                              "(%s) ", modVars.table[log->modHandle].modName);
      }

      if ( BLOG_FIELDS_SEVERITY & displayFields )
      {
         switch (log->severity)
         {
            case BLOG_SEVERITY_FATAL_ERR:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "FATAL ERROR: ");
               break;
            case BLOG_SEVERITY_ERR:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "ERROR: ");
               break;
            case BLOG_SEVERITY_WARN:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "WARNING: ");
               break;
            case BLOG_SEVERITY_INITIALIZATION:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "INIT: ");
               break;
            case BLOG_SEVERITY_FNC_ENTRY_EXIT:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "FUNC: ");
               break;
            case BLOG_SEVERITY_INFO:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "INFO: ");
               break;
            case BLOG_SEVERITY_ALWAYS:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "");
               break;
            case BLOG_SEVERITY_USR1:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "USR1: ");
               break;
            case BLOG_SEVERITY_USR2:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "USR2: ");
               break;
            case BLOG_SEVERITY_USR3:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "USR3: ");
               break;
            case BLOG_SEVERITY_USR4:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "USR4: ");
               break;
            case BLOG_SEVERITY_USR5:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "USR5: ");
               break;
            case BLOG_SEVERITY_USR6:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "USR6: ");
               break;
            case BLOG_SEVERITY_USR7:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "USR7: ");
               break;
            case BLOG_SEVERITY_USR8:
               logLen += strPrintf( &(logBufp[logLen]),
                                     BLOG_CFG_MAX_STR_LEN - logLen - 1,
                                      "USR8: ");
               break;
            default:
               /* Should never get a unknown severity */
               XCHG_ASSERT( BOS_FALSE );
               break;
         }

      }

      /* Add the log message to the end of the buffer */
      if ( log->isConstStr == BOS_TRUE )
      {
         /* The message string is in printf format */
         logLen += strPrintf( &(logBufp[logLen]),
                               ( BLOG_CFG_MAX_STR_LEN - logLen ),
                                log->msgStrp, log->param1,
                                 log->param2, log->param3);
      }
      else
      {
         /* The message has already been printed to a buffer */
         strMaxCat( &(logBufp[logLen]), log->msgStrp,
                      BLOG_CFG_MAX_STR_LEN - logLen );
         logLen += strlen( &(logBufp[logLen]) );
      }


      if ( bAddEndOfLine )
      {
         /* Add the newline character
          * The 1 is for the terminating NULL */
         logLen += strPrintf( &(logBufp[logLen]),
                              BLOG_CFG_MAX_STR_LEN - logLen + 1, "\n");
      }


      /* Print the log buffer
       * If purgeLog is true print the log to the default and any other
       * enabled callbacks
       */
      status = bosMutexAcquire( &printCBVars.mutex );
      XCHG_ASSERT( status == BOS_STATUS_OK );

      if ( purgeLog == BOS_TRUE )
      {
         /* Print using the default callback even if it is disabled */
         printCBVars.table[0].printCB( logBufp, logLen );
         startIndex = 1;
      }
      /* loop through the table of callbacks and print if enabled */
      for ( index = startIndex; index < printCBVars.numCB; index++ )
      {
         if ( printCBVars.table[index].isEnabled == BOS_TRUE )
         {
            printCBVars.table[index].printCB( logBufp, logLen );
         }
      }

      status = bosMutexRelease( &printCBVars.mutex );
      XCHG_ASSERT( status == BOS_STATUS_OK );

   }
}
/*****************************************************************************
*
*  FUNCTION:   formatTime
*
*  PURPOSE:    Converts the timestamp to a string in the form of hh:mm:ss.sss
*              The buffer should already be allocated for the correct length.
*
*  PARAMETERS:
*           timestamp - Upper 32 bit field specifies the time in
*                       milliseconds, lower 16 bit precision field specifies
*                       microseconds and must wrap at 1000
*           buffer - time in string format "hh:mm:ss.sss"
*
*  RETURNS:    Number of characters written to buffer.
*
*  NOTES:
*
*****************************************************************************/
int formatTime( BLOG_TIME* timestamp, char* buffer )
{

   int hours;
   int mins;
   int secs;
   int msecs;
   int size;
   int temp1, temp2;

   temp1 = (timestamp->msecs) % MSEC_PER_HOUR;
   temp2 = temp1 % MSEC_PER_MIN;

   hours = (timestamp->msecs) / MSEC_PER_HOUR;
   mins = temp1 / MSEC_PER_MIN;
   secs = temp2 / MSEC_PER_SEC;
   msecs = temp2 % MSEC_PER_SEC;

   size = strPrintf( buffer, TIMESTAMP_LENGTH + 1,
                       "%02d:%02d:%02d.%03d%03d ",
                         hours, mins, secs, msecs, timestamp->usecs );
   return size;
}

#if BLOG_CFG_QUEUE_LOGS
/*****************************************************************************
*
*  FUNCTION:   clearLog
*
*  PURPOSE:    Clears all log entries.
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void clearLog(void)
{
   BOS_STATUS status;

   status = bosMutexAcquire( &(qVars.mutex) );
   XCHG_ASSERT( status == BOS_STATUS_OK );
   qVars.writeOffset = qVars.readOffset = 0;
   qVars.queueFull = BOS_FALSE;
   status = bosMutexRelease( &(qVars.mutex) );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   status = bosMutexAcquire( &(strVars.mutex) );
   XCHG_ASSERT( status == BOS_STATUS_OK );
   strVars.numStrLogs = 0;
   strVars.strBufFull = BOS_FALSE;
   strVars.strBufIndex = 0;
   status = bosMutexRelease( &(strVars.mutex) );
   XCHG_ASSERT( status == BOS_STATUS_OK );

}
#endif /* BLOG_CFG_QUEUE_LOGS */


/*****************************************************************************
*
*  FUNCTION:
*     blogDefaultPrintCallback
*
*  PURPOSE:
*     Default print callback for blog module
*
*  PARAMETERS:
*     string - the string that will be printed
*     length - the length of the string
*
*  RETURNS:
*     None
*
*  NOTES:
*     This function has been made public so that it can be added and removed
*     as a registered print callback.
*
*****************************************************************************/
int blogDefaultPrintCallback( const char* string, BOS_UINT16 length )
{
   printf( "%s", string );
   return ( length );
}


/*****************************************************************************
*
*  FUNCTION:
*     blogDefaultTimeStampCallback
*
*  PURPOSE:
*     Deafult callback to obtain the current time for the Blog module
*
*  PARAMETERS:
*     timestamp - the timestamp is returned to the calling function in this
*                 variable
*
*  RETURNS:
*     None
*
*  NOTES:
*     This function has been made public so that it can be added and removed
*     as a registered print callback.
*
*****************************************************************************/
int blogDefaultTimeStampCallback( BLOG_TIME* timestamp )
{
   BOS_TIME_MS timeMsec;

   bosTimeGetMs( &timeMsec );

   timestamp->msecs = timeMsec;
   timestamp->usecs = 0;

   return ( 1 );
}


/*****************************************************************************
*
*  FUNCTION:
*     blogSetTimeStampCallback
*
*  PURPOSE:
*     To set the callback to obtain the current time for the Blog module
*
*  PARAMETERS:
*     timestampCallback - new timestamp callback.
*
*  RETURNS:
*     Old timestamp callback.
*
*  NOTES:
*
*****************************************************************************/
BLOG_TIMESTAMPCB blogSetTimeStampCallback( BLOG_TIMESTAMPCB timestampCallback )
{
   BLOG_TIMESTAMPCB  oldTimestampCallback;

   /* Save the old callback to return the caller. */
   oldTimestampCallback = getTimeCB;

   /* Set the new callback. */
   getTimeCB = timestampCallback;

   return ( oldTimestampCallback );
}
