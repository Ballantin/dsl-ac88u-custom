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

#ifndef BLOG_H
#define BLOG_H

/* ---- Include Files ---------------------------------------- */

#include <bosCfg.h>
#include <blogCfg.h>
#include <bosTypes.h>
#include <stdarg.h>

/* ---- Constants and Types ---------------------------------- */


/*
** BLOG_FUNCTION_NAME_MACRO is the compiler macro used to determine the name
** of the function that contains the log statement. The function name can be
** optionally displayed when messages are printed.
**
** The latest C standard, known as ISO/IEC 9899:1999 (aka C99), supports the
** __func__ identifier. This identifier is implicitly declared by the compiler
** as if, immediately following the opening brace of each function definition,
** the declaration:
**
**          static const char __func__[] = "function-name";
**
** appeared. Where "function-name" is the name of the enclosing function.
**
** __func__ is supported by the GNU C compiler, and used to set the function-name
** macro.
**
**
** The Microsoft compiler doesn't appear to support __func__; however, version 7
** of the Visual C++ compiler supports __FUNCTION__.
**
**
** Other compilers may support __func__, __FUNCTION__, or other similar extensions.
** They should be added to the list below as they are discovered.
*/
#if BLOG_CFG_FUNCTION_FIELD_ENABLED
   #if defined( __GNUC__ )
      #define BLOG_FUNCTION_NAME_MACRO       __func__
   #elif (( BOS_OS_WIN32 == 1 ) && ( _MSC_VER >= 1300 ))
      #define BLOG_FUNCTION_NAME_MACRO       __FUNCTION__
   #else
      #define BLOG_FUNCTION_NAME_MACRO       "<unknown>"
   #endif
#endif


/*
** These are the set of compiler macros used to implicitly generate debug
** information for logging statements, using predefined compiler macros.
** Currently, they support logging of the function name that contains the
** log statement. However, they could easily be extended to support the
** file and line number of the log statement using the predefined macros
** __FILE__ and __LINE__.
*/
#if BLOG_CFG_FUNCTION_FIELD_ENABLED

   #define BLOG_DEBUG_PARMS_DECL       const char *function,
   #define BLOG_DEBUG_PARMS            function,
   #define BLOG_HDL( module )          (module), BLOG_FUNCTION_NAME_MACRO

   #define BLOG_DEBUG_SET_INFO(x)            \
      do                                     \
      {                                      \
         (x)->functionName = function;       \
      } while ( 0 )

#else

   #define BLOG_DEBUG_PARMS_DECL
   #define BLOG_DEBUG_PARMS
   #define BLOG_HDL( module )          (module)
   #define BLOG_DEBUG_SET_INFO(x)

#endif




/* Handles to pass in when setting all modules in blogSetModule
 * or enabling all print callback functions in blogEnableCB
 */
#define BLOG_MODULES_ALL          ( BLOG_CFG_MAX_MODULES + 1 )
#define BLOG_PRINTCB_ALL          ( BLOG_CFG_MAX_PRINTCB )


typedef int          BLOG_MOD_HNDL;
typedef int          BLOG_CB_HNDL;
typedef BOS_UINT32   BLOG_PARAM;

/* Bit mask values for log severity levels */
typedef enum BLOG_SEVERITY
{
   BLOG_SEVERITY_OFF             = 0x00000000,

   /* Will always log/print msgs */
   BLOG_SEVERITY_ALWAYS          = 0x00000000,

   /* Generic severities used for most purposes. */
   BLOG_SEVERITY_FATAL_ERR       = 0x00000001,
   BLOG_SEVERITY_ERR             = 0x00000002,
   BLOG_SEVERITY_WARN            = 0x00000004,
   BLOG_SEVERITY_INITIALIZATION  = 0x00000008,
   BLOG_SEVERITY_FNC_ENTRY_EXIT  = 0x00000010,
   BLOG_SEVERITY_INFO            = 0x00000020,

   /* User defined severity levels */
   BLOG_SEVERITY_USR1            = 0x00000040,
   BLOG_SEVERITY_USR2            = 0x00000080,
   BLOG_SEVERITY_USR3            = 0x00000100,
   BLOG_SEVERITY_USR4            = 0x00000200,
   BLOG_SEVERITY_USR5            = 0x00000400,
   BLOG_SEVERITY_USR6            = 0x00000800,
   BLOG_SEVERITY_USR7            = 0x00001000,
   BLOG_SEVERITY_USR8            = 0x00002000,

   /* Masks for composite severity levels */
   BLOG_SEVERITY_ALL_USR         = 0x00003fc0,
   BLOG_SEVERITY_ALL_BUT_USR     = 0x0000003f,
   BLOG_SEVERITY_ALL             = 0x00003fff,

   BLOG_SEVERITY_INVALID         = 0xffffffff

} BLOG_SEVERITY;

/*
** Bit mask values for enabling fields printed.
**
** Reserved bit-fields are maintained for compatibility with other severities
** logging systems.
*/
typedef enum BLOG_FIELDS
{
   BLOG_FIELDS_SEVERITY    = 0x0001,
   BLOG_RESERVED1          = 0x0002,
   BLOG_FIELDS_FUNCTION    = 0x0004,
   BLOG_FIELDS_MODULE      = 0x0008,
   BLOG_FIELDS_TIME        = 0x0010,
   BLOG_FIELDS_THREAD      = 0x0020,

   BLOG_FIELDS_NONE        = 0x0000,
   BLOG_FIELDS_ALL         = 0x003F,

   BLOG_FIELDS_INVALID     = 0xffff

} BLOG_FIELDS;




/* The timestamp supports microsecond resolution.
 * A 32 bit number stores the time in milliseconds, and a 16 bit precision
 * field can be used to achieve microsecond resolution.  The precision field
 * must wrap at 1000 (every millisecond)
 */
typedef struct
{
   BOS_UINT32 msecs;  /* time in milliseconds */
   BOS_UINT16 usecs;  /* usec precision value that wraps at 1000 */
} BLOG_TIME;


/* Callback function for getting timestamps
 * The format of the timestamp is expected to be in microseconds
 */
typedef BOS_BOOL ( *BLOG_TIMESTAMPCB )( BLOG_TIME* timestamp );

/* Callback function for directing log output */
typedef int ( *BLOG_PRINTCB )( const char* data, BOS_UINT16 length );

/* ---- Macro Definitions ------------------------------------ */

/* Use the following macros to log a string to BLOG.  Do not use direct
 * calls to blogConstString() and blogString() functions.
 *
 */

/* Raw log without blog handle. No severities are specified, and no fields
** will be displayed. */
#if BLOG_CFG_RAW_LOG_ENABLED
   #define blogStrRaw( a )          blogStrRawFunc a
#else
   #define blogStrRaw( a )
#endif


/* Raw octet buffer logging function, in ASCII hex format. */
#if BLOG_CFG_HEX_LOG_ENABLED
   #define blogHexBuffer( a )       blogHexBufferFunc a
#else
   #define blogHexBuffer( a )
#endif


/* blogPrintf is simply an alias for printf, nothing more and nothing less.
** It should ONLY be used in special circumstances, typically only prior to the
** initialization of the BLOG module. e.g. To perform a "Hello, world!" type
** indication at the entry point of an application, prior to initializing BLOG.
** Or alternatively, it may be required to use this macro in initialization
** sequences of modules that must be initialized prior to BLOG. e.g. BOS must
** be initialized prior to BLOG, since BLOG uses BOS primitives. Using
** blogPrintf if prefered to simply using printf because it can be compiled out,
** resulting in memory savings. */
#if BLOG_CFG_PRINTF_LOG_ENABLED
   #define blogPrintf( a )          printf a
#else
   #define blogPrintf( a )
#endif



/* Fatal error Log Messages */
#if BLOG_CFG_SEVERITY_FATAL_ERR_ENABLED

#define blogConstStrFatalErr0(module, string) \
         blogConstString(module, BLOG_SEVERITY_FATAL_ERR, string, 0, 0, 0)

#define blogConstStrFatalErr1(module, string, p1) \
         blogConstString(module, BLOG_SEVERITY_FATAL_ERR, string, p1, 0, 0)

#define blogConstStrFatalErr2(module, string, p1, p2) \
         blogConstString(module, BLOG_SEVERITY_FATAL_ERR, string, p1, p2, 0)

#define blogConstStrFatalErr3(module, string, p1, p2, p3) \
         blogConstString(module, BLOG_SEVERITY_FATAL_ERR, string, p1, p2, p3)

#define blogStrFatalErr( a )           blogStrFatalErrFunc a
#define blogStrFatalErrNoFields( a )   blogStrFatalErrNoFieldsFunc a
#define blogVStrFatalErr( a )          blogVStrFatalErrFunc a

#else

#define blogConstStrFatalErr0(module, string)
#define blogConstStrFatalErr1(module, string, p1)
#define blogConstStrFatalErr2(module, string, p1, p2)
#define blogConstStrFatalErr3(module, string, p1, p2, p3)
#define blogStrFatalErr( a )
#define blogStrFatalErrNoFields( a )
#define blogVStrFatalErr( a )

#endif



/* Error Log Messages */
#if BLOG_CFG_SEVERITY_ERR_ENABLED

#define blogConstStrErr0(module, string) \
         blogConstString(module, BLOG_SEVERITY_ERR, string, 0, 0, 0)

#define blogConstStrErr1(module, string, p1) \
         blogConstString(module, BLOG_SEVERITY_ERR, string, p1, 0, 0)

#define blogConstStrErr2(module, string, p1, p2) \
         blogConstString(module, BLOG_SEVERITY_ERR, string, p1, p2, 0)

#define blogConstStrErr3(module, string, p1, p2, p3) \
         blogConstString(module, BLOG_SEVERITY_ERR, string, p1, p2, p3)

#ifndef blogStrErr
#define blogStrErr( a )          blogStrErrFunc a
#endif

#define blogStrErrNoFields( a )  blogStrErrNoFieldsFunc a
#define blogVStrErr( a )         blogVStrErrFunc a

#else

#define blogConstStrErr0(module, string)
#define blogConstStrErr1(module, string, p1)
#define blogConstStrErr2(module, string, p1, p2)
#define blogConstStrErr3(module, string, p1, p2, p3)
#define blogStrErr( a )
#define blogStrErrNoFields( a )
#define blogVStrErr( a )

#endif

/* Warning Log Messages */
#if BLOG_CFG_SEVERITY_WARN_ENABLED

#define blogConstStrWarn0(module, string) \
         blogConstString(module, BLOG_SEVERITY_WARN, string, 0, 0, 0)

#define blogConstStrWarn1(module, string, p1) \
         blogConstString(module, BLOG_SEVERITY_WARN, string, p1, 0, 0)

#define blogConstStrWarn2(module, string, p1, p2) \
         blogConstString(module, BLOG_SEVERITY_WARN, string, p1, p2, 0)

#define blogConstStrWarn3(module, string, p1, p2, p3) \
         blogConstString(module, BLOG_SEVERITY_WARN, string, p1, p2, p3)

#ifndef blogStrWarn
#define blogStrWarn( a )         blogStrWarnFunc a
#endif

#define blogStrWarnNoFields( a ) blogStrWarnNoFieldsFunc a
#define blogVStrWarn( a )        blogVStrWarnFunc a

#else

#define blogConstStrWarn0(module, string)
#define blogConstStrWarn1(module, string, p1)
#define blogConstStrWarn2(module, string, p1, p2)
#define blogConstStrWarn3(module, string, p1, p2, p3)
#define blogStrWarn( a )
#define blogStrWarnNoFields( a )
#define blogVStrWarn( a )

#endif

/* Initialization Log Messages */
#if BLOG_CFG_SEVERITY_INITIALIZATION_ENABLED

#define blogConstStrInit0(module, string) \
         blogConstString(module, BLOG_SEVERITY_INITIALIZATION, string, 0, 0, 0)

#define blogConstStrInit1(module, string, p1) \
         blogConstString(module, BLOG_SEVERITY_INITIALIZATION, string, p1, 0, 0)

#define blogConstStrInit2(module, string, p1, p2) \
         blogConstString(module, BLOG_SEVERITY_INITIALIZATION, string, p1, p2, 0)

#define blogConstStrInit3(module, string, p1, p2, p3) \
         blogConstString(module, BLOG_SEVERITY_INITIALIZATION, string, p1, p2, p3)

#define blogStrInit( a )         blogStrInitFunc a
#define blogStrInitNoFields( a ) blogStrInitNoFieldsFunc a
#define blogVStrInit( a )        blogVStrInitFunc a

#else

#define blogConstStrInit0(module, string)
#define blogConstStrInit1(module, string, p1)
#define blogConstStrInit2(module, string, p1, p2)
#define blogConstStrInit3(module, string, p1, p2, p3)
#define blogStrInit( a )
#define blogStrInitNoFields( a )
#define blogVStrInit( a )

#endif


/* Function Entry Exit Log Messages */
#if BLOG_CFG_SEVERITY_FNC_ENTRY_EXIT_ENABLED

#define blogConstStrFnc0(module, string) \
         blogConstString(module, \
                          BLOG_SEVERITY_FNC_ENTRY_EXIT, string, 0, 0, 0)

#define blogConstStrFnc1(module, string, p1) \
         blogConstString(module, \
                          BLOG_SEVERITY_FNC_ENTRY_EXIT, string, p1, 0, 0)

#define blogConstStrFnc2(module, string, p1, p2) \
         blogConstString(module, \
                          BLOG_SEVERITY_FNC_ENTRY_EXIT, string, p1, p2, 0)

#define blogConstStrFnc3(module, string, p1, p2, p3) \
         blogConstString(module, \
                          BLOG_SEVERITY_FNC_ENTRY_EXIT, string, p1, p2, p3)

#ifndef blogStrFnc
#define blogStrFnc( a )          blogStrFncFunc a
#endif

#define blogStrFncNoFields( a )  blogStrFncNoFieldsFunc a
#define blogVStrFnc( a )         blogVStrFncFunc a

#else

#define blogConstStrFnc0(module, string)
#define blogConstStrFnc1(module, string, p1)
#define blogConstStrFnc2(module, string, p1, p2)
#define blogConstStrFnc3(module, string, p1, p2, p3)
#define blogStrFnc( a )
#define blogStrFncNoFields( a )
#define blogVStrFnc( a )

#endif

/* Info Log Messages */
#if BLOG_CFG_SEVERITY_INFO_ENABLED

#define blogConstStrInfo0(module, string) \
         blogConstString(module, BLOG_SEVERITY_INFO, string, 0, 0, 0)

#define blogConstStrInfo1(module, string, p1) \
         blogConstString(module, BLOG_SEVERITY_INFO, string, p1, 0, 0)

#define blogConstStrInfo2(module, string, p1, p2) \
         blogConstString(module, BLOG_SEVERITY_INFO, string, p1, p2, 0)

#define blogConstStrInfo3(module, string, p1, p2, p3) \
         blogConstString(module, BLOG_SEVERITY_INFO, string, p1, p2, p3)

#ifndef blogStrInfo
#define blogStrInfo( a )         blogStrInfoFunc a
#endif

#define blogStrInfoNoFields( a ) blogStrInfoNoFieldsFunc a
#define blogVStrInfo( a )        blogVStrInfoFunc a

#else

#define blogConstStrInfo0(module, string)
#define blogConstStrInfo1(module, string, p1)  { (void)(p1); }
#define blogConstStrInfo2(module, string, p1, p2) { (void)(p1); (void)(p2); }
#define blogConstStrInfo3(module, string, p1, p2, p3) { (void)(p1); (void)(p2); (void)(p3); }
#define blogStrInfo( a )  nullFunc a
#define blogStrInfoNoFields( a )  nullFunc a
#define blogVStrInfo( a )  nullFunc a

#endif

/* User Log Messages
 * Note that individual user severities can not be compiled out individually.
 * Also the severity is passed in for these functions and must be one of the
 * defined USR severities.
 */
#if BLOG_CFG_SEVERITY_USR_ENABLED

#define blogConstStrUsr0(module, severity, string) \
         blogConstString(module, severity, string, 0, 0, 0)

#define blogConstStrUsr1(module, severity, string, p1) \
         blogConstString(module, severity, string, p1, 0, 0)

#define blogConstStrUsr2(module, severity, string, p1, p2) \
         blogConstString(module, severity, string, p1, p2, 0)

#define blogConstStrUsr3(module, severity, string, p1, p2, p3) \
         blogConstString(module, severity, string, p1, p2, p3)

#define blogStrUsr( a )          blogStrUsrFunc a
#define blogStrUsrNoFields( a )  blogStrUsrNoFieldsFunc a
#define blogVStrUsr( a )         blogVStrUsrFunc a

#else

#define blogConstStrUsr0(module, severity, string)
#define blogConstStrUsr1(module, severity, string, p1)
#define blogConstStrUsr2(module, severity, string, p1, p2)
#define blogConstStrUsr3(module, severity, string, p1, p2, p3)
#define blogStrUsr( a )
#define blogStrUsrNoFields( a )
#define blogVStrUsr( a )

#endif

/* Always Log Messages
 * These macros will always log and print the messages
 */
#if BLOG_CFG_SEVERITY_ALWAYS_ENABLED

#define blogConstStrAlways0(module, string) \
         blogConstString(module, BLOG_SEVERITY_ALWAYS, string, 0, 0, 0)

#define blogConstStrAlways1(module, string, p1) \
         blogConstString(module, BLOG_SEVERITY_ALWAYS, string, p1, 0, 0)

#define blogConstStrAlways2(module, string, p1, p2) \
         blogConstString(module, BLOG_SEVERITY_ALWAYS, string, p1, p2, 0)

#define blogConstStrAlways3(module, string, p1, p2, p3) \
         blogConstString(module, BLOG_SEVERITY_ALWAYS, string, p1, p2, p3)

#ifndef blogStrAlways
#define blogStrAlways( a )          blogStrAlwaysFunc a
#endif

#ifndef blogStrAlwaysNoFields
#define blogStrAlwaysNoFields( a )  blogStrAlwaysNoFieldsFunc a
#endif

#define blogVStrAlways( a )         blogVStrAlwaysFunc a

#else

#define blogConstStrAlways0(module, string)
#define blogConstStrAlways1(module, string, p1)
#define blogConstStrAlways2(module, string, p1, p2)
#define blogConstStrAlways3(module, string, p1, p2, p3)
#define blogStrAlways( a )
#define blogStrAlwaysNoFields( a )
#define blogVStrAlways( a )

#endif

/* ---- Function Prototypes ---------------------------------- */

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
BOS_BOOL blogInit(BLOG_PRINTCB defaultCB, BLOG_TIMESTAMPCB timestampCB);

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
void blogTerm(void);

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
void blogSetDefaultFields( BLOG_FIELDS displayFields );


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
void blogSetDefaultLogSeverity( BLOG_SEVERITY logSeverity );

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
void blogSetDefaultPrintSeverity( BLOG_SEVERITY printSeverity );

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
BLOG_MOD_HNDL blogAddDefaultModule( const char *modNameStrp );

/*****************************************************************************
*
*  FUNCTION:   blogAddModule
*
*  PURPOSE:    Registers a module and assigns a handle to the module. The
*              severities and display fields are explictly specified. Use
*              blogAddDefaultModule to use the default settings.
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
);

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
void blogRemoveModule( BLOG_MOD_HNDL handle );


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

#define blogSetPrintSeverity( m, s )   \
         blogSetModule( (m), BLOG_SEVERITY_INVALID, (s), BLOG_FIELDS_INVALID )

#define blogSetLogSeverity( m, s )     \
         blogSetModule( (m), (s), BLOG_SEVERITY_INVALID, BLOG_FIELDS_INVALID )

#define blogSetFields( m, f )          \
         blogSetModule( (m), BLOG_SEVERITY_INVALID, BLOG_SEVERITY_INVALID, (f) )

BOS_BOOL blogSetModule
(
   BLOG_MOD_HNDL  moduleHandle,
   BLOG_SEVERITY  logSeverity,
   BLOG_SEVERITY  printSeverity,
   BLOG_FIELDS    displayFields
);

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

#define blogGetName( m, n )            blogGetModule( (m), (n),   NULL, NULL, NULL )
#define blogGetPrintSeverity( m, s  )  blogGetModule( (m), NULL,  NULL, (s),  NULL )
#define blogGetLogSeverity( m, s  )    blogGetModule( (m), NULL,  (s),  NULL, NULL )
#define blogGetFields( m, f )          blogGetModule( (m), NULL,  NULL, NULL, (f) )

BOS_BOOL blogGetModule
(
   BLOG_MOD_HNDL  moduleHandle,
   char          *modNameBufp,
   BLOG_SEVERITY *logSeverity,
   BLOG_SEVERITY *printSeverity,
   BLOG_FIELDS   *displayFields
);


/*****************************************************************************
*
*  FUNCTION:   blogEnableAutoEndOfLineSupport
*
*  PURPOSE:    End-of-line characters may be added automatically to all logs
*              by enabling this feature. Use of this feature is not encouraged
*              and it is disabled by default. The default setting provides users
*              with the the flexibility to decide whether a newline is required
*              or not for each log statement (as well as providing a consistent
*              interface with printf). There are numerous cases where a newline
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
void blogEnableAutoEndOfLine( BLOG_MOD_HNDL moduleHandle, BOS_BOOL enable );

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
void blogPrintModules(void);

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
void blogClearModules(void);


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
BLOG_CB_HNDL blogAddPrintCB(BLOG_PRINTCB printCB, BOS_BOOL enable);

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
BOS_BOOL blogEnablePrintCB(BLOG_CB_HNDL cbHandle, BOS_BOOL enable);

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
void blogPrintCBStats(void);

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
void blogClearPrintCB(void);


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
void blogPurge(void);
#endif


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
int blogDefaultPrintCallback( const char* string, BOS_UINT16 length );


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
int blogDefaultTimeStampCallback( BLOG_TIME* timestamp );


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
BLOG_TIMESTAMPCB blogSetTimeStampCallback( BLOG_TIMESTAMPCB timestampCallback );


/*****************************************************************************
*
*  FUNCTION:   blogConstString/blogString functions
*
*  PURPOSE:    Used by macro definitions for const and non-const strings
*              Should not be called directly.
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTES:
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
);


void blogString
(
   BLOG_MOD_HNDL  module,
   BLOG_DEBUG_PARMS_DECL
   BLOG_SEVERITY  severity,
   const char    *format,
   BOS_BOOL       bDisplayFields,
   va_list        argList
);


#define BLOG_STR_FUNC_PARAM_DECLS   \
            BLOG_MOD_HNDL module, BLOG_DEBUG_PARMS_DECL const char *format, ...

void blogStrFatalErrFunc( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrErrFunc     ( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrWarnFunc    ( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrInitFunc    ( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrFncFunc     ( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrInfoFunc    ( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrAlwaysFunc  ( BLOG_STR_FUNC_PARAM_DECLS );

void blogStrUsrFunc
(
   BLOG_MOD_HNDL  module,
   BLOG_DEBUG_PARMS_DECL
   BLOG_SEVERITY  usrSeverity,
   char const    *format,
   ...
);

void blogVStrInfoFunc
(
   BLOG_MOD_HNDL  module,
   const char   *format,
   va_list arg
);


void nullFunc 
(
   BLOG_MOD_HNDL module, 
   BLOG_DEBUG_PARMS_DECL const char *format, 
   ...
);

void blogStrFatalErrNoFieldsFunc( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrErrNoFieldsFunc     ( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrWarnNoFieldsFunc    ( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrInitNoFieldsFunc    ( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrFncNoFieldsFunc     ( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrInfoNoFieldsFunc    ( BLOG_STR_FUNC_PARAM_DECLS );
void blogStrAlwaysNoFieldsFunc  ( BLOG_STR_FUNC_PARAM_DECLS );

void blogStrUsrNoFieldsFunc
(
   BLOG_MOD_HNDL  module,
   BLOG_DEBUG_PARMS_DECL
   BLOG_SEVERITY  usrSeverity,
   const char    *format,
   ...
);


void blogStrRawFunc( const char *format, ... );

void blogHexBufferFunc( const BOS_UINT8 *bytes, unsigned int numberOfBytes );


#endif  /* BLOG_H  */
