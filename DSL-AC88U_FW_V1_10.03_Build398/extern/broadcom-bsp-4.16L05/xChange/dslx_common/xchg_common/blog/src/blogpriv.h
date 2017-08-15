/***************************************************************************
*
*     Copyright (c) 2002 Broadcom, All Rights Reserved.
*     Contains proprietary and confidential information.
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*      Private header file for the blog module.
*
****************************************************************************/

#ifndef _BLOGPRIV_H
#define _BLOGPRIV_H

/* ---- Include Files ---------------------------------------- */

#include "blog.h"
#include "blogCfg.h"
#include "cbf.h"
#include "bosMutex.h"
#include "bosTask.h"

/* ---- Constants and Types ---------------------------------- */

#define TIMESTAMP_LENGTH 16  /* hh:mm:ss.sss */
#define MSEC_PER_HOUR    3600000
#define MSEC_PER_MIN     60000

#ifndef MSEC_PER_SEC
#define MSEC_PER_SEC     1000
#endif

typedef BOS_SINT32 MOD_SEV_TYPE; /* module severity bitmask type */
typedef BOS_SINT8  MOD_DIS_TYPE; /* module display fields bitmask type */

/* Structure of log queue entries
 * Note: this structure is used even if BLOG_CFG_QUEUE_LOGS is not set
 */
typedef struct
{
   BLOG_TIME timestamp;
   BLOG_MOD_HNDL modHandle;
   BLOG_SEVERITY severity;
#if BLOG_CFG_GET_THREAD
   BOS_TASK_ID taskId;  /* Use BOS to get task name when printing */
#endif
   const char* msgStrp;
   BLOG_PARAM param1;
   BLOG_PARAM param2;
   BLOG_PARAM param3;
   BOS_BOOL isConstStr;
   BOS_BOOL bDisplayFields;
   const char *functionName;
} Q_ENTRY;

/* The log queue and related variables */
#if BLOG_CFG_QUEUE_LOGS
typedef struct
{
   BOS_MUTEX mutex;

   /* Queue offsets to next free log and next log to print */
   volatile CBF_Q_OFFSET writeOffset;
   volatile CBF_Q_OFFSET readOffset;

   Q_ENTRY logQueue[BLOG_CFG_QSIZE];
   BOS_BOOL queueFull;

} Q_VARS;
#endif

/* Buffers for storing string messages and related variable */
#if BLOG_CFG_QUEUE_LOGS
typedef struct
{

   BOS_MUTEX mutex;
   int       numStrLogs;
   BOS_BOOL  strBufFull;
   char      strBufpp[BLOG_CFG_NUM_STR_BUFFERS] [BLOG_CFG_MAX_STR_LEN+1];
   int       strBufIndex;

} STR_BUF_VARS;
#endif /* BLOG_CFG_QUEUE_LOGS */

/* Stores filter settings for all modules */
typedef struct
{
   char modName[BLOG_CFG_MOD_NAME_LEN + 1]; /* plus 1 for NULL */
   MOD_SEV_TYPE logSeverity;     /* Filter for logs stored in queue */
   MOD_SEV_TYPE printSeverity;   /* Filter for logs printed */
   MOD_DIS_TYPE displayFields;
   BOS_BOOL     bAddEndOfLine;
} MODULE_CTRL;

/* Table for registered modules
 * The first entry is reserved as the default module handle so
 * size is one greater than BLOG_CFG_MAX_MODULES
 */
typedef struct
{
   BOS_MUTEX mutex;
   MODULE_CTRL table[BLOG_CFG_MAX_MODULES + 1];
   int numModules;

   MOD_SEV_TYPE defaultLogSeverity;     /* Filter for logs stored in queue */
   MOD_SEV_TYPE defaultPrintSeverity;   /* Filter for logs printed */
   MOD_DIS_TYPE defaultDisplayFields;

} MODULE_VARS;

/* Structure for registered print callbacks */
typedef struct
{
   BLOG_PRINTCB printCB;
   BOS_BOOL     isEnabled;
} PRINT_CB_CTRL;

/* Table of registered callback functions for printing
 * Functions for directing the log output must be registered,
 * and can then be enabled/disabled using the provided handle
 */
typedef struct
{
   BOS_MUTEX mutex;
   PRINT_CB_CTRL table[BLOG_CFG_MAX_PRINTCB];
   int  numCB;
} PRINTCB_VARS;


/* ---- Private Function Prototypes ---------------------------- */

void printSingleLog(Q_ENTRY* log, BOS_BOOL purgeLog);
int formatTime( BLOG_TIME* timestamp, char* buffer );

#if BLOG_CFG_QUEUE_LOGS
void addToQueue(Q_ENTRY* log);
void printQTask(void* context);
void clearLog(void);
#endif




#if 0
#define BLOG_VAR_ARGS( severity, bDisplayFields )                    \
         do                                                          \
         {                                                           \
            va_list argList;                                         \
            va_start( argList, format );                             \
            blogString( module, BLOG_DEBUG_PARMS (severity), format, \
                       (bDisplayFields), argList );                  \
            va_end( argList );                                       \
         }                                                           \
         while ( 0 );
#else
#define BLOG_VAR_ARGS( severity, bDisplayFields )                    \
         (void)function;                                             \
         (void)module;                                               \
         do                                                          \
         {                                                           \
            va_list argList;                                         \
            va_start( argList, format );                             \
            memset( gBlogBuffer, 0, BLOG_CFG_MAX_STR_LEN + 1 );      \
            vStrPrintf( gBlogBuffer, BLOG_CFG_MAX_STR_LEN,           \
                        format, argList );                           \
            va_end( argList );                                       \
            printf( "%s", gBlogBuffer );                             \
         }                                                           \
         while ( 0 );
#endif

#endif /* BLOGPRIV_H */
