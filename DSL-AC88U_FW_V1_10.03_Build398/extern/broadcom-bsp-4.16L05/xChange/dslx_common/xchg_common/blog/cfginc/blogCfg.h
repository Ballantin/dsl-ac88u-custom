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
*      This file contains the default configuration for the blog module
*
****************************************************************************/

#ifndef _BLOGCFG_H
#define _BLOGCFG_H

/* ---- Include Files ---------------------------------------- */

#include <blogCfgCustom.h>

/* ---- Constants and Types ---------------------------------- */

/*
 * Set to 0 to bypass queue logging and print synchronously
 */
#ifndef  BLOG_CFG_QUEUE_LOGS
#define  BLOG_CFG_QUEUE_LOGS  1
#endif


/*
 * Compile time severity log level
 */
#ifndef BLOG_CFG_SEVERITY_FATAL_ERR_ENABLED
#define BLOG_CFG_SEVERITY_FATAL_ERR_ENABLED        1
#endif

#ifndef BLOG_CFG_SEVERITY_ERR_ENABLED
#define BLOG_CFG_SEVERITY_ERR_ENABLED              1
#endif

#ifndef BLOG_CFG_SEVERITY_WARN_ENABLED
#define BLOG_CFG_SEVERITY_WARN_ENABLED             1
#endif

#ifndef BLOG_CFG_SEVERITY_INITIALIZATION_ENABLED
#define BLOG_CFG_SEVERITY_INITIALIZATION_ENABLED   1
#endif

#ifndef BLOG_CFG_SEVERITY_FNC_ENTRY_EXIT_ENABLED
#define BLOG_CFG_SEVERITY_FNC_ENTRY_EXIT_ENABLED   1
#endif

#ifndef BLOG_CFG_SEVERITY_INFO_ENABLED
#define BLOG_CFG_SEVERITY_INFO_ENABLED             1
#endif

#ifndef BLOG_CFG_SEVERITY_ALWAYS_ENABLED
#define BLOG_CFG_SEVERITY_ALWAYS_ENABLED           1
#endif

#ifndef BLOG_CFG_SEVERITY_USR_ENABLED
#define BLOG_CFG_SEVERITY_USR_ENABLED              1
#endif

/* Raw logs are compiled in by default. */
#ifndef  BLOG_CFG_RAW_LOG_ENABLED
#define  BLOG_CFG_RAW_LOG_ENABLED                  1
#endif

/* Hex logs are compiled in by default. */
#ifndef  BLOG_CFG_HEX_LOG_ENABLED
#define  BLOG_CFG_HEX_LOG_ENABLED                  1
#endif

/* Printf logs are compiled in by default. */
#ifndef  BLOG_CFG_PRINTF_LOG_ENABLED
#define  BLOG_CFG_PRINTF_LOG_ENABLED               1
#endif


/* Set to one to disable all logs at compile time. This overrides all settings
** made above. */
#ifndef  BLOG_CFG_DISABLE_ALL_LOGS
#define  BLOG_CFG_DISABLE_ALL_LOGS                 0
#endif


/*
** Check if the user wants to permanently disable all logging at compile time.
*/
#if BLOG_CFG_DISABLE_ALL_LOGS
   #undef BLOG_CFG_SEVERITY_FATAL_ERR_ENABLED
   #define BLOG_CFG_SEVERITY_FATAL_ERR_ENABLED        0

   #undef BLOG_CFG_SEVERITY_ERR_ENABLED
   #define BLOG_CFG_SEVERITY_ERR_ENABLED              0

   #undef BLOG_CFG_SEVERITY_WARN_ENABLED
   #define BLOG_CFG_SEVERITY_WARN_ENABLED             0

   #undef BLOG_CFG_SEVERITY_INITIALIZATION_ENABLED
   #define BLOG_CFG_SEVERITY_INITIALIZATION_ENABLED   0

   #undef BLOG_CFG_SEVERITY_FNC_ENTRY_EXIT_ENABLED
   #define BLOG_CFG_SEVERITY_FNC_ENTRY_EXIT_ENABLED   0

   #undef BLOG_CFG_SEVERITY_INFO_ENABLED
   #define BLOG_CFG_SEVERITY_INFO_ENABLED             0

   #undef BLOG_CFG_SEVERITY_ALWAYS_ENABLED
   #define BLOG_CFG_SEVERITY_ALWAYS_ENABLED           0

   #undef BLOG_CFG_SEVERITY_USR_ENABLED
   #define BLOG_CFG_SEVERITY_USR_ENABLED              0

   #undef   BLOG_CFG_RAW_LOG_ENABLED
   #define  BLOG_CFG_RAW_LOG_ENABLED                  0

   #undef   BLOG_CFG_HEX_LOG_ENABLED
   #define  BLOG_CFG_HEX_LOG_ENABLED                  0

   #undef   BLOG_CFG_PRINTF_LOG_ENABLED
   #define  BLOG_CFG_PRINTF_LOG_ENABLED               0
#endif


/***************************************************************************/
/* Configuration for log queue and buffer sizes */

#if BLOG_CFG_QUEUE_LOGS

   /*
    * Log queue size
    * NOTE: MUST be a power of 2 for queue implementation used
    */
   #ifndef  BLOG_CFG_QSIZE
   #define  BLOG_CFG_QSIZE   512
   #endif

   /*
    * Number of string buffers for strings copied by the logString function
    */
   #ifndef  BLOG_CFG_NUM_STR_BUFFERS
   #define  BLOG_CFG_NUM_STR_BUFFERS  100
   #endif

#endif /* BLOG_CFG_QUEUE_LOGS */

/*
 * Maximum length of log messages after printf formatting
 * (ie. the length of the message string as it would appear when printed)
 */
#ifndef  BLOG_CFG_MAX_STR_LEN
#define  BLOG_CFG_MAX_STR_LEN  200
#endif

/***************************************************************************/
/*
 * The maximum number of modules that can be registered with the blog utility
 */
#ifndef  BLOG_CFG_MAX_MODULES
#define  BLOG_CFG_MAX_MODULES  20
#endif

/*
 * The maximum length of each module name
 */
#ifndef  BLOG_CFG_MOD_NAME_LEN
#define  BLOG_CFG_MOD_NAME_LEN  5
#endif

/*
 * The maximum number of print callback functions that can be registered
 * including the default one which is set in blogInit
 */
#ifndef  BLOG_CFG_MAX_PRINTCB
#define  BLOG_CFG_MAX_PRINTCB  5
#endif

/***************************************************************************/

/*
 * Set to include thread names in log entries
 */
#ifndef  BLOG_CFG_GET_THREAD
#define  BLOG_CFG_GET_THREAD  0
#endif


/*
 * Set to enable support for function name field. This will display the name
 * of the function that contains the log when it is queued and/or printed.
 */
#ifndef  BLOG_CFG_FUNCTION_FIELD_ENABLED
#define  BLOG_CFG_FUNCTION_FIELD_ENABLED     1
#endif


/***************************************************************************/

#if BLOG_CFG_QUEUE_LOGS

   /* Priority level for the print task */
   #ifndef  BLOG_CFG_TASK_PRTY
   #define  BLOG_CFG_TASK_PRTY BOS_TASK_CLASS_LOW
   #endif

   /* Stack size for the print task */
   #ifndef  BLOG_CFG_TASK_STACK
   #define  BLOG_CFG_TASK_STACK 4096
   #endif

   /* Time in msec for task to sleep between printing the log */
   #ifndef  BLOG_CFG_SLEEP_TIME
   #define  BLOG_CFG_SLEEP_TIME 500
   #endif

#endif /* BLOG_CFG_QUEUE_LOGS */

/***************************************************************************/

#endif  /* _BLOGCFG_H  */
