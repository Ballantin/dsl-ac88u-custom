//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2002-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_MXASSERT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXASSERT_H
//M5T_INTERNAL_USE_END


// Verify that this header file is included by MxConfig.h only. This
// verification is performed because we state that the assertion mechanism
// configuration is done through the PreMxConfig.h, which is only included by
// MxConfig.h. If we were to allow the direct inclusion of MxAssert.h, we would
// loose the configuration done in PreMxConfig.h.
//-----------------------------------------------------------------------------
#if !defined(MXG_MXCONFIG_H)
    #error This file must be included through MxConfig.h only!
#endif


//==============================================================================
//====  Include Files  =========================================================
//==============================================================================
#if !defined (MXD_OS_WINDOWS_CE)
#include <errno.h>
#endif

#if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
    #include <sys/types.h>
    #include <unistd.h>
    #include <signal.h>
#elif defined(MXD_OS_VXWORKS)
    #include <taskLib.h>
// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    #include <bosTask.h>
// BRCM_CUSTOM - [end] - Add ECOS support.
#endif


//==============================================================================
//====  GLOBAL FUNCTION  =======================================================
//==============================================================================


//==SDOC========================================================================
//==
//==  mxt_PFNAssertFailHandler
//==
//==============================================================================
//<GROUP BASIC_TYPES>
//
//  Summary:
//      Defines the prototype for an assertion failed handler.
//
//  Description:
//      This data type defines a function prototype for the assertion failed
//      handler, which is responsible for handling failed assertions
//      and notifications most of the time, by using other handlers like
//      mxt_PFNAssertTraceHandler.
//
//  Parameters:
//             opq: The opaque value provided at registration.
//    pszAssertion: String describing the assertion.
//      nErrNumber: When available, the error number associated with the
//                  assertion failure.
// pszExtraComment: Additional information that can be passed to the user.
//         pszFile: File in which the assertion failed.
//           uLine: Line number where the assertion failed.
//
//==EDOC========================================================================
typedef void (*mxt_PFNAssertFailHandler)(
    IN mxt_opaque opq,
    IN const char* pszAssertion,
    IN int nErrNumber,
    IN const char* pszExtraComment,
    IN const char* pszFile,
    IN unsigned int uLine);

//==SDOC========================================================================
//==
//== SAssertFailHandler
//==
//==============================================================================
//<GROUP BASIC_STRUCTS>
//
//  Summary:
//      Defines the structure to be given back to the assertion handler.
//
//  Description:
//      Defines the structure that associates a prototype for an assertion
//      failed handler and an opaque value that is given back to the handler
//      when it is called.
//
//==EDOC========================================================================
typedef struct
{
    // Pointer to an assertion failed handler function.
    mxt_PFNAssertFailHandler pfnHandler;
    // Opaque value to be passed to the assertion failed handler function.
    mxt_opaque opq;
} SAssertFailHandler;

//==SDOC========================================================================
//==
//==  mxt_PFNAssertTraceHandler
//==
//==============================================================================
//<GROUP BASIC_TYPES>
//
//  Summary:
//      Defines the prototype for a trace handler.
//
//  Description:
//      This data type defines a function prototype for tracing assertion failed
//      information. It is called by the default mxt_PFNAssertFailHandler when
//      an assertion fails.
//
//  Parameters:
//             opq: The opaque value provided at registration.
//    pszAssertion: String describing the assertion.
//      nErrNumber: When available, the error number associated with the
//                  assertion failure.
// pszExtraComment: Additional information that can be passed to the user.
//         pszFile: File in which the assertion failed.
//           uLine: Line number where the assertion failed.
//
//==EDOC========================================================================
typedef void (*mxt_PFNAssertTraceHandler)(
    IN mxt_opaque opq,
    IN const char* pszAssertion,
    IN int nErrNumber,
    IN const char* pszExtraComment,
    IN const char* pszFile,
    IN unsigned int uLine);

//==SDOC========================================================================
//==
//== SAssertTraceHandler
//==
//==============================================================================
//<GROUP BASIC_STRUCTS>
//
//  Summary:
//      Defines the structure to be given back to the trace handler.
//
//  Description:
//      Defines the structure that associates a prototype for an assertion trace
//      handler and an opaque value that is given back to the handler when it is
//      called.
//
//==EDOC========================================================================
typedef struct
{
    // Pointer to an assertion trace handler function.
    mxt_PFNAssertTraceHandler pfnHandler;
    // Opaque value to be passed to the assertion trace handler function.
    mxt_opaque opq;
} SAssertTraceHandler;

//==SDOC========================================================================
//==
//==  mxt_PFNAssertCallStackTraceHandler
//==
//==============================================================================
//<GROUP BASIC_TYPES>
//
//  Summary:
//      Defines the prototype for a call stack trace handler.
//
//  Parameters:
//      opq: The opaque value provided at registration.
//
//  Description:
//      This data type defines a function prototype for a call stack trace
//      handler that is called by the default mxt_PFNAssertFailHandler when an
//      assertion fails.
//
//==EDOC========================================================================
typedef void (*mxt_PFNAssertCallStackTraceHandler)(IN mxt_opaque opq);

//==SDOC========================================================================
//==
//== SAssertCallStackTraceHandler
//==
//==============================================================================
//<GROUP BASIC_STRUCTS>
//
//  Summary:
//      Defines the structure to be given back to the callstack trace handler.
//
//  Description:
//      Defines the structure that associates a prototype for an assertion call
//      stack trace handler and an opaque value that is given back to the
//      handler when it is called.
//
//==EDOC========================================================================
typedef struct
{
    // Pointer to an assertion call stack trace handler function.
    mxt_PFNAssertCallStackTraceHandler pfnHandler;
    // Opaque value to be passed to the assertion call stack trace handler
    // function.
    mxt_opaque opq;
} SAssertCallStackTraceHandler;

//==SDOC========================================================================
//==
//==  mxt_PFNAssertFinalBehaviorHandler
//==
//==============================================================================
//<GROUP BASIC_TYPES>
//
//  Summary:
//      Defines the prototype for a final behaviour handler.
//
//  Parameters:
//      opq: The opaque value provided at registration.
//
//  Description:
//      This data type defines a function prototype for a final behaviour
//      handler called after assertion failure.
//
//      When an assertion fails and the final behaviour handler is enabled and
//      MXD_ASSERT_FINAL_BEHAVIOR_IS_FATAL is defined, the default behaviour is
//      to call <C>exit()</C>.
//
//  See Also:
//      MXD_ASSERT_FINAL_BEHAVIOR_IS_FATAL, MXD_ASSERT_FINAL_BEHAVIOR_DISABLE,
//      MXD_ASSERT_FINAL_BEHAVIOR_OVERRIDE
//
//==EDOC========================================================================
typedef void (*mxt_PFNAssertFinalBehaviorHandler)(IN mxt_opaque opq);

//==SDOC========================================================================
//==
//== SAssertFinalBehaviorHandler
//==
//==============================================================================
//<GROUP BASIC_STRUCTS>
//
//  Summary:
//      Defines the structure to be given back to the final behaviour handler.
//
//  Description:
//      Defines the structure that associates a prototype for an assertion final
//      behaviour handler and an opaque value that is given back to the handler
//      when it is called.
//
//==EDOC========================================================================
typedef struct
{
    //  Pointer to an assertion final behavior handler function.
    mxt_PFNAssertFinalBehaviorHandler pfnHandler;
    //  Opaque value to be passed to the assertion final behavior handler
    //  function.
    mxt_opaque opq;
} SAssertFinalBehaviorHandler;

//M5T_INTERNAL_USE_BEGIN

#ifdef __cplusplus
extern "C" {
#endif
extern SAssertFailHandler* g_pstAssertFailHandler;
extern SAssertTraceHandler* g_pstAssertTraceHandler;
extern SAssertCallStackTraceHandler* g_pstAssertCallStackTraceHandler;
extern SAssertFinalBehaviorHandler* g_pstAssertFinalBehaviorHandler;
#ifdef __cplusplus
}
#endif
//M5T_INTERNAL_USE_END

// Summary:
// Installs a new failure handler.
SAssertFailHandler MxAssertSetNewFailHandler(
    IN SAssertFailHandler* pstNewHandler);

// Summary:
// Installs a new assert trace handler.
SAssertTraceHandler MxAssertSetNewTraceHandler(
    IN SAssertTraceHandler* pstNewHandler);

// Summary:
// Installs a new call stack trace handler.
SAssertCallStackTraceHandler MxAssertSetNewCallStackTraceHandler(
    IN SAssertCallStackTraceHandler* pstNewHandler);

// Summary:
// Installs a new final behavior handler.
SAssertFinalBehaviorHandler MxAssertSetNewFinalBehaviorHandler(
    IN SAssertFinalBehaviorHandler* pstNewHandler);

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_ASSERT_ENABLE_ALL)
    #undef  MXD_ASSERT_ENABLE_STD
    #undef  MXD_ASSERT_ENABLE_RT
    #define MXD_ASSERT_ENABLE_STD
    #define MXD_ASSERT_ENABLE_RT
#endif

#undef MXD_ASSERT_ENABLE_SUPPORT
#if defined(MXD_ASSERT_ENABLE_STD) || \
    defined(MXD_ASSERT_ENABLE_RT)
    #define MXD_ASSERT_ENABLE_SUPPORT
#endif

#if defined(MXD_ASSERT_DISABLE_OUTPUT_FILENAME)
    #define MX_ASSERT_FILENAME_COMPILER_MACRO NULL
#elif !defined(MX_ASSERT_FILENAME_COMPILER_MACRO)
    #define MX_ASSERT_FILENAME_COMPILER_MACRO __FILE__
#endif

#if defined(MXD_ASSERT_DISABLE_OUTPUT_LINENUMBER)
    #define  MX_ASSERT_LINENUMBER_COMPILER_MACRO 0
#elif !defined(MX_ASSERT_LINENUMBER_COMPILER_MACRO)
    #define MX_ASSERT_LINENUMBER_COMPILER_MACRO __LINE__
#endif

#if !defined(MXD_ASSERT_DISABLE_DEBUG_BREAK)      &&  \
    (!defined(MXD_ASSERT_FINAL_BEHAVIOR_IS_FATAL) ||  \
     defined(MXD_ASSERT_FINAL_BEHAVIOR_OVERRIDE)  ||  \
     defined(MXD_ASSERT_FINAL_BEHAVIOR_DISABLE))  &&  \
    !defined(MX_ASSERT_DEBUG_BREAK)
        #if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
            #define MX_ASSERT_DEBUG_BREAK  kill(getpid(), SIGABRT)
        #elif defined(MXD_OS_NUCLEUS)
            #define MX_ASSERT_DEBUG_BREAK  NU_Suspend_Task(NU_Current_Task_Pointer())
        #elif defined(MXD_OS_VXWORKS)
            #define MX_ASSERT_DEBUG_BREAK  taskSuspend(taskIdSelf())
        #elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
            #if defined(MXD_ARCH_IX86)
            #define MX_ASSERT_DEBUG_BREAK  _asm { int 03 }
// BRCM_CUSTOM - [begin] - Add ECOS support.
        #elif defined(MXD_OS_BOS_ECOS)
            #define MX_ASSERT_DEBUG_BREAK   BOS_TASK_ID taskId; bosTaskGetMyTaskId(&taskId); bosTaskSuspend(&taskId)
// BRCM_CUSTOM - [end] - Add ECOS support.
        #else
            #define MX_ASSERT_DEBUG_BREAK
        #endif
#else
    #define MX_ASSERT_DEBUG_BREAK
#endif
#else
    #define MX_ASSERT_DEBUG_BREAK
#endif

#if !defined(MX_ASSERT_STANDARD_MACRO)
    #if defined(MXD_ASSERT_ENABLE_STD)    || \
        defined(MXD_ASSERT_ENABLE_RT)
        #define MX_ASSERT_STANDARD_MACRO(assertion, szExtraComment) \
            { \
                if (!(assertion)) \
                { \
                    g_pstAssertFailHandler->pfnHandler( \
                        g_pstAssertFailHandler->opq, \
                        #assertion, \
                        0, \
                        szExtraComment, \
                        MX_ASSERT_FILENAME_COMPILER_MACRO, \
                        MX_ASSERT_LINENUMBER_COMPILER_MACRO); \
                    MX_ASSERT_DEBUG_BREAK; \
                } \
            }
    #else
        #define MX_ASSERT_STANDARD_MACRO(assertion, szExtraComment)
    #endif
#endif

#if !defined(MX_ASSERT_PERROR_STANDARD_MACRO)
    #if defined(MXD_ASSERT_ENABLE_STD)    || \
        defined(MXD_ASSERT_ENABLE_RT)
        #define MX_ASSERT_PERROR_STANDARD_MACRO(szExtraComment) \
            { \
                if (MX_ERRNO_GET() != 0) \
                { \
                    g_pstAssertFailHandler->pfnHandler( \
                        g_pstAssertFailHandler->opq, \
                        NULL, \
                        MX_ERRNO_GET(), \
                        szExtraComment, \
                        MX_ASSERT_FILENAME_COMPILER_MACRO, \
                        MX_ASSERT_LINENUMBER_COMPILER_MACRO); \
                    MX_ASSERT_DEBUG_BREAK; \
                } \
            }
    #else
        #define MX_ASSERT_PERROR_STANDARD_MACRO(szExtraComment)
    #endif
#endif

#if !defined(MX_ASSERT)
    #if defined(MXD_ASSERT_ENABLE_STD)
        #define MX_ASSERT(assertion) \
            MX_ASSERT_STANDARD_MACRO(assertion, NULL)
    #else
        #define MX_ASSERT(assertion)
    #endif
#endif

#if !defined(MX_ASSERT_EX)
    #if defined(MXD_ASSERT_ENABLE_STD)
        #define MX_ASSERT_EX(assertion, szExtraComment) \
            MX_ASSERT_STANDARD_MACRO(assertion, szExtraComment)
    #else
        #define MX_ASSERT_EX(assertion, szExtraComment)
    #endif
#endif

#if !defined(MX_ASSERT_RT)
    #if defined(MXD_ASSERT_ENABLE_RT)
        #define MX_ASSERT_RT(assertion) \
            MX_ASSERT_STANDARD_MACRO(assertion, NULL)
    #else
        #define MX_ASSERT_RT(assertion)
    #endif
#endif

#if !defined(MX_ASSERT_RT_EX)
    #if defined(MXD_ASSERT_ENABLE_RT)
        #define MX_ASSERT_RT_EX(assertion, szExtraComment) \
            MX_ASSERT_STANDARD_MACRO(assertion, szExtraComment)
    #else
        #define MX_ASSERT_RT_EX(assertion, szExtraComment)
    #endif
#endif

#if !defined(MX_ASSERT_PERROR)
    #if defined(MXD_ASSERT_ENABLE_STD)
        #define MX_ASSERT_PERROR() \
            MX_ASSERT_PERROR_STANDARD_MACRO(NULL)
    #else
        #define MX_ASSERT_PERROR()
    #endif
#endif

#if !defined(MX_ASSERT_PERROR_EX)
    #if defined(MXD_ASSERT_ENABLE_STD)
        #define MX_ASSERT_PERROR_EX(szExtraComment) \
            MX_ASSERT_PERROR_STANDARD_MACRO(szExtraComment)
    #else
        #define MX_ASSERT_PERROR_EX(szExtraComment)
    #endif
#endif

#if !defined(MX_ASSERT_PERROR_RT)
    #if defined(MXD_ASSERT_ENABLE_RT)
        #define MX_ASSERT_PERROR_RT() \
            MX_ASSERT_PERROR_STANDARD_MACRO(NULL)
    #else
        #define MX_ASSERT_PERROR_RT()
    #endif
#endif

#if !defined(MX_ASSERT_PERROR_RT_EX)
    #if defined(MXD_ASSERT_ENABLE_RT)
        #define MX_ASSERT_PERROR_RT_EX(szExtraComment) \
            MX_ASSERT_PERROR_STANDARD_MACRO(szExtraComment)
    #else
        #define MX_ASSERT_PERROR_RT_EX(szExtraComment)
    #endif
#endif

#if !defined(MX_ASSERT_ONLY)
    #if defined(MXD_ASSERT_ENABLE_SUPPORT) && defined(MXD_ASSERT_ENABLE_STD)
        #define MX_ASSERT_ONLY(x) x
    #else
        #define MX_ASSERT_ONLY(x)
    #endif
#endif

#if !defined(MX_ASSERT_ONLY_RT)
    #if defined(MXD_ASSERT_ENABLE_SUPPORT) && defined(MXD_ASSERT_ENABLE_RT)
        #define MX_ASSERT_ONLY_RT(x) x
    #else
        #define MX_ASSERT_ONLY_RT(x)
    #endif
#endif


#if !defined(MXD_DISABLE_EXTERNAL_ASSERT_OVERRIDE)
    #undef  assert
    #define assert(x) MX_ASSERT(x)

    #undef  assert_perror
    #define assert_perror(x) MX_ASSERT_PERROR()

    #undef  ASSERT
#endif

//M5T_INTERNAL_USE_END

#endif // #ifndef MXG_MXASSERT_H

// Doc-O-Matic documentation section
#if 0

//@@ASSERTION_MECHANISM_OVERVIEW
//<GROUP FRAMEWORK>
//<TOPICORDER 1>
//<TITLE Assertion Mechanism Overview>
//==SDOC========================================================================
//
// Description:
//
//  The framework offers a complete assertion mechanism solution with different
//  levels and extensions.  Assertions are used in debug mode to track down
//  problems related to limit conditions, unhandled conditions, or so-called
//  impossible conditions.
//
//  To enable the assertions in an application, one of the
//  MXD_ASSERT_ENABLE_STD macros must be defined in the application's build.
//
//  The assertion mechanism is highly configurable and scalable. The behaviour
//  on assertion failure can be changed by the user at compile time or at run
//  time and different assertions macros can be enabled or disabled at compile
//  time to add or remove validations.
//
//  The assertion mechanism makes use of various handlers when an assertion
//  fails:
//
//      - The "Assertion Failed Handler" is the first handler called when an
//        assertion fails. When assertions are enabled, the default handler
//        calls the trace handler, the call-stack trace handler, and the final
//        behaviour handler.
//      - The "Trace Handler", called by the previous handler, prints or sends
//        a trace about the failed assertion. The information outputted contains
//        the assertion condition along with the file name and line number where
//        the assertion occurred as in the following example:
//        "Assertion Failed (true == false)  [152]SomeFile.cpp"
//      - The "Call-Stack Trace Handler", also called by the first handler,
//        outputs the current state of the call-stack.
//      - Finally, the "Final Behaviour Handler" is called last by the first
//        handler, after all tracing handlers have been called to terminate or
//        restart the application.
//
//  All of these handlers can be overridden or disabled by defining the proper
//  compilation #define.
//
//  By default, the assertion mechanism uses the tracing mechanism. To do
//  otherwise, the tracing mechanism or the assertion behaviour must be
//  overridden using the MXD_ASSERT_TRACE_OVERRIDE macro or by setting a new
//  handler by calling MxAssertSetNewTraceHandler at run time. If traces are not
//  enabled and the tracing handler is not overridden, the application may look
//  like it just hung without any indication.
//
//  The following assertion macros are available within the framework. Note that
//  if an application wants to use any of these macros, it must include
//  "Config/MxConfig.h" first.
//  #MX_ASSERT#
//    Basic assertion macro.  Takes only one parameter: the assertion itself.
//    <CODE>
//      MX_ASSERT(i < m_vecItems.Size());
//    </CODE>
//  #MX_ASSERT_EX#
//    Basic assertion macro. Other than the assertion, takes an extra comment to
//    clarify the assertion. If the assertion fails, the comment is included in
//    the assertion trace.
//    <CODE>
//      MX_ASSERT_EX(i < m_vecItems.Size(), "Illegal access on m_vecItems.");
//    </CODE>
//  #MX_ASSERT_RT#
//    Same as MX_ASSERT but with a real-time configuration level. This assertion
//    is used in time critical code so it can be added or removed easily to
//    avoid adding latency to the execution.
//    <CODE>
//      MX_ASSERT_RT(m_rtpPacket.IsValid());
//    </CODE>
//  #MX_ASSERT_RT_EX#
//    Same as MX_ASSERT_RT but with an extra comment to clarify the assertion.
//    <CODE>
//      MX_ASSERT_RT_EX(m_rtpPacket.IsValid(), "Invalid RTP packet!");
//    </CODE>
//  #MX_ASSERT_PERROR#
//    Asserts that <C>errno</C> is zero (no error). Prints <C>errno</C> if the
//    assertion fails.
//    <CODE>
//      MX_ASSERT_PERROR();
//    </CODE>
//  #MX_ASSERT_PERROR_EX#
//    Same as MX_ASSERT_PERROR but with an extra comment to clarify the
//    assertion.
//    <CODE>
//      MX_ASSERT_PERROR_EX("Call to bind() failed.");
//    </CODE>
//  #MX_ASSERT_PERROR_RT#
//    Same as MX_ASSERT_RT but asserts on <C>errno</C>.
//    <CODE>
//      MX_ASSERT_PERROR_RT();
//    </CODE>
//  #MX_ASSERT_PERROR_RT_EX#
//    Same as MX_ASSERT_RT_EX but asserts on <C>errno</C>.
//    <CODE>
//      MX_ASSERT_PERROR_RT_EX("send() failed!");
//    </CODE>
//  #MX_ASSERT_ONLY#
//    Used to declare variables used in assertion macros only. This is to avoid
//    annoying compiler warnings for unused variables.
//    <CODE>
//      MX_ASSERT_ONLY(bool bValid = false);
//      MX_ASSERT(bValid);
//    </CODE>
//
// Note:
//  All the assertion macros involving <C>errno</C> described above should
//  be used with care since not all functions set it.  Moreover, some
//  functions set <C>errno</C> only on a specific operating system.
//
//==EDOC========================================================================


//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Basic assertion macro.
//
//  Parameters:
//      assertion:
//          Expression to assert.
//
// Description:
//  Basic assertion macro. The macro outputs data only if the expression passed
//  is false.
//
//    <CODE>
//      MX_ASSERT(i < m_vecItems.Size());
//    </CODE>
//
//  NOTES:
//      If an application wants to use any of these macros, it must include
//      "Config/MxConfig.h" first.
//
// Location:
//   Basic/MxAssert.h
//
//==EDOC========================================================================
#define MX_ASSERT(assertion)

//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Extended basic assertion macro.
//
//  Parameters:
//      assertion:
//          Expression to assert.
//
//      string:
//          String to output with the assertion.
//
// Description:
//  Extended basic assertion macro. Other than the assertion, takes an extra
//  comment to clarify the assertion. If the assertion fails, the comment is
//  included in the assertion trace. The macro outputs data only if the
//  expression passed is false.
//
//    <CODE>
//      MX_ASSERT_EX(i < m_vecItems.Size(), "Illegal access on m_vecItems.");
//    </CODE>
//
//  NOTES:
//      If an application wants to use any of these macros, it must include
//      "Config/MxConfig.h" first.
//
// Location:
//   Basic/MxAssert.h
//
//==EDOC========================================================================
#define MX_ASSERT_EX(assertion, string)

//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Real-time assertion macro.
//
//  Parameters:
//      assertion:
//          Expression to assert.
//
// Description:
//  Real-time assertion macro. The macro outputs data only if the expression
//  passed is false. This assertion is used in time critical code so it can be
//  added or removed easily to avoid adding latency to the execution.
//
//    <CODE>
//      MX_ASSERT_RT(m_rtpPacket.IsValid());
//    </CODE>
//
//  NOTES:
//      If an application wants to use any of these macros, it must include
//      "Config/MxConfig.h" first.
//
// Location:
//   Basic/MxAssert.h
//
//==EDOC========================================================================
#define MX_ASSERT_RT(assertion)

//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Extended real-time assertion macro.
//
//  Parameters:
//      assertion:
//          Expression to assert.
//
//      string:
//          String to output with the assertion.
//
// Description:
//  Extended real-time assertion macro. Other than the assertion, takes an extra
//  comment to clarify the assertion. This assertion is used in time critical
//  code so it can be added or removed easily to avoid adding latency to the
//  execution. If the assertion fails, the comment is included in the assertion
//  trace. The macro outputs data only if the expression passed is false.
//
//    <CODE>
//      MX_ASSERT_EX(i < m_vecItems.Size(), "Illegal access on m_vecItems.");
//    </CODE>
//
//  NOTES:
//   If an application wants to use any of these macros, it must include
//   "Config/MxConfig.h" first.
//
// Location:
//   Basic/MxAssert.h
//
//==EDOC========================================================================
#define MX_ASSERT_RT_EX(assertion, string)

//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Macro asserting on errno.
//
// Description:
//  Macro asserting on errno.
//
//  NOTES:
//   All the assertion macros involving <C>errno</C> described above should be
//   used with care since not all functions set it. Moreover, some functions set
//   <C>errno</C> only on a specific operating system. If an application wants
//   to use any of these macros, it must include "Config/MxConfig.h" first.
//
// Location:
//   Basic/MxAssert.h
//
//==EDOC========================================================================
#define MX_ASSERT_PERROR

//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Extended macro asserting on errno.
//
// Parameters:
//      string:
//          String to print out.
//
// Description:
//  Extended macro asserting on errno.
//
//    <CODE>
//      MX_ASSERT_PERROR_EX("send() failed!");
//    </CODE>
//
//  NOTES:
//   All the assertion macros involving <C>errno</C> described above should be
//   used with care since not all functions set it. Moreover, some functions set
//   <C>errno</C> only on a specific operating system. If an application wants
//   to use any of these macros, it must include "Config/MxConfig.h" first.
//
// Location:
//   Basic/MxAssert.h
//
//==EDOC========================================================================
#define MX_ASSERT_PERROR_EX(string)

//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Real-time macro asserting on errno.
//
// Description:
//  Real-time macro asserting on errno.
//
//  NOTES:
//   All the assertion macros involving <C>errno</C> described above should be
//   used with care since not all functions set it. Moreover, some functions set
//   <C>errno</C> only on a specific operating system. If an application wants
//   to use any of these macros, it must include "Config/MxConfig.h" first.
//
// Location:
//   Basic/MxAssert.h
//
//==EDOC========================================================================
#define MX_ASSERT_PERROR_RT

//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Extended real-time macro asserting on errno.
//
// Parameters:
//      string:
//          String to print out.
//
// Description:
//  Extended real-time macro asserting on errno.
//
//    <CODE>
//      MX_ASSERT_PERROR_RT_EX("send() failed!");
//    </CODE>
//
//  Note:
//   All the assertion macros involving <C>errno</C> described above should be
//   used with care since not all functions set it. Moreover, some functions set
//   <C>errno</C> only on a specific operating system. If an application wants
//   to use any of these macros, it must include "Config/MxConfig.h" first.
//
// Location:
//   Basic/MxAssert.h
//
//==EDOC========================================================================
#define MX_ASSERT_PERROR_RT_EX(string)

//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Used to declare variables used in assert macros.
//
// Parameters:
//      assertion:
//          Variable to declare.
//
// Description:
//  Used to declare variables used in assertion macros only. This is to
//  avoid annoying compiler warnings for unused variables.
//
//    <CODE>
//      MX_ASSERT_ONLY(bool bValid = false);
//      MX_ASSERT(bValid);
//    </CODE>
//
//  NOTES:
//      If an application wants to use any of these macros, it must
//      include "Config/MxConfig.h" first.
//
// Location:
//   Basic/MxAssert.h
//
//==EDOC========================================================================
#define MX_ASSERT_ONLY

//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Used to declare variables used in real time assert macros.
//
// Parameters:
//      assertion:
//          Variable to declare.
//
// Description:
//  Used to declare variables used in real time assertion macros only. This is
//  to avoid annoying compiler warnings for unused variables.
//
//    <CODE>
//      MX_ASSERT_ONLY_RT(bool bValid = false);
//      MX_ASSERT_RT(bValid);
//    </CODE>
//
//  NOTES:
//      If an application wants to use any of these macros, it must
//      include "Config/MxConfig.h" first.
//
// Location:
//   Basic/MxAssert.h
//
//==EDOC========================================================================
#define MX_ASSERT_ONLY_RT

#endif // #if 0
