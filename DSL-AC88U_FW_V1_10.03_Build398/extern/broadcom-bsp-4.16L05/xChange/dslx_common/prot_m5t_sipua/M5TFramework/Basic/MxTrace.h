//==============================================================================
//==============================================================================
//
//     Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==============================================================================
#ifndef MXG_MXTRACE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXTRACE_H
//M5T_INTERNAL_USE_END


// Verifies that this header file is included by MxConfig.h only. This
// verification is performed because we state that the tracing mechanism
// configuration is done through the PreMxConfig.h, which is only included by
// MxConfig.h. If we were allowed the direct inclusion of MxTrace.h, we would
// loose the configuration done in PreMxConfig.h.
//-----------------------------------------------------------------------------
#if !defined(MXG_MXCONFIG_H)
    #error "This file must be included through MxConfig.h only!"
#endif

#ifndef MXG_PKGID_H
#include "Basic/PkgId.h"
#endif

//==============================================================================
//====  DOCUMENTATION  =========================================================
//==============================================================================

//@@GENERAL_TRACING_CONFIGURATION
//<GROUP FRAMEWORK>
//<TOPICORDER 0>
//<TITLE General Tracing Configuration>
//==============================================================================
//
// Description:
//
//  The Framework offers an extensive tracing mechanism with its various
//  software packages. The goals of the tracing mechanism are to:
//    - Provide multiple tracing levels. Each level can be enabled or disabled
//      at compile time. When a level is disabled at compile time, the
//      associated tracing macro expands to nothing. When a level is enabled at
//      compile time, the level can be enabled or disabled at run time.
//    - Provide the possibility to create any number of types or subtypes of
//      traces using tracing nodes. Each trace node can be enabled or disabled
//      at run time.
//    - Augment the traces with meta-information (task id, time stamp, etc.).
//    - Use a format to ease automatic parsing.
//    - Be flexible and extensible.
//
//  The tracing mechanism is made up of five main parts:
//    - Trace level part that can be configured at compile time and run time.
//    - Trace node part that can only be configured at run time.
//    - Trace output handlers that can be configured at compile time and run
//      time.
//    - Trace format handlers that can be configured at compile time and run
//      time.
//    - Trace call stack handlers that can be configured at compile time and
//      run time.
//
//  Before describing the details of each of the above building blocks, it is
//  important to highlight how the tracing mechanism determines if a trace has
//  to be outputted. Two conditions MUST be present to have a trace displayed.
//  The level of the tracing macro used and the node used as a parameter of the
//  tracing macro MUST be enabled. There is no direct relationship between
//  trace's levels and the trace nodes in the sense that any level can be
//  applied to any trace node.
//
//  The following paragraphs present a detailed description of the tracing
//  mechanism main parts.
//
//  #Tracing Levels#
//  The framework defines ten tracing levels ordered from eLEVEL0 (Highest
//  severity) to eLEVEL9 (lowest severity). There are two complementary macros
//  that regroup all or none of the levels. See the complete list of levels in
//  the EMxTraceLevel documentation.
//
//  Tracing levels can be configured at compile time using various pre-processor
//  defines. Tracing levels are also configurable at run time using different
//  methods. The compile time and run time configuration capabilities of the
//  tracing levels help to better suit any application needs.
//
//  Compile time configuration is possible to determine which trace levels are
//  compiled-in using a list of pre-processor defines. Each level is controlled
//  independently but it is also possible to control multiple levels at once.
//  The pre-processor define of a given level activates three tracing macros.
//  For example, MXD_TRACE0_ENABLE_SUPPORT enables MX_TRACE0, MX_TRACE0_HEX, and
//  MX_TRACE0_IS_ENABLED. The complete list of pre-processor defines is as
//  follows:
//          - MXD_TRACE0_ENABLE_SUPPORT
//          - MXD_TRACE1_ENABLE_SUPPORT
//          - MXD_TRACE2_ENABLE_SUPPORT
//          - MXD_TRACE3_ENABLE_SUPPORT
//          - MXD_TRACE4_ENABLE_SUPPORT
//          - MXD_TRACE5_ENABLE_SUPPORT
//          - MXD_TRACE6_ENABLE_SUPPORT
//          - MXD_TRACE7_ENABLE_SUPPORT
//          - MXD_TRACE8_ENABLE_SUPPORT
//          - MXD_TRACE9_ENABLE_SUPPORT
//          - MXD_TRACEX_ENABLE_SUPPORT
//          - MXD_TRACE_ENABLE_ALL
//          - MXD_TRACE_MAX_LEVEL
//
//  Run time configuration of the tracing levels is possible regardless of the
//  compile time configuration used. At run time, each level can be controlled
//  independently on a per node basis or for all the nodes in a subtree. It is
//  also possible to select multiple levels at the same time. The following is
//  the list of methods available for run time configuration of the tracing
//  levels:
//          - MxTraceEnableNodeLevel
//          - MxTraceDisableNodeLevel
//          - MxTraceEnableNodeMaxLevels
//          - MxTraceGetNodeLevelsEnabled
//
//  #Tracing Nodes#
//  A tracing node (STraceNode) is located in a tree and represents a logical
//  entity to regroup traces. A node can have zero or more child and it has
//  its own state that is independent from the state of other nodes. Like the
//  trace levels, a node can be configured to be enabled or disabled. Any number
//  of nodes can exist in the system.
//
//  Since there is no limit on the number of tracing nodes, access to a node is
//  designed differently for the configuration of its state and for issuing a
//  trace. When issuing a trace, the tracing node variable is used directly.
//  Programmatically, the visibility of a trace node variable is meant to be
//  restricted to the part of code using it. On the other hand, enabling or
//  disabling a tracing node might come from a totally different module in the
//  program where the tracing node variable is not available. Instead, referring
//  a node is possible using a string in the form of a path like in directory
//  trees. This explains why the nodes MUST be grouped in a tree structure.
//
//  The tree is built at run time with registration and un-registration methods.
//  At the beginning, only the tracing mechanism root node exists. The root node
//  is the only one that needs to be published so children can be attached to
//  it. The root node is accessible through its node variable, which is called
//  g_stTraceRoot. Children are added during the initialization process to
//  create the tree. Usually, children of the root node represent high-level
//  logical grouping. For example, the Framework node and SIP stack node are
//  children of the root node. In turn, these high-level grouping can contain
//  children representing more precise grouping. For example, the Framework has
//  many folders: Network, Kernel, etc. The Kernel folder in turn has many
//  classes CMutex, CThread, etc. The methods for registering and un-registering
//  nodes are listed below:
//          - MxTraceRegisterNode
//          - MxTraceUnregisterNode
//
//  The nodes allowed to trace are configured at run time using two methods.
//  Each node can be controlled independently. The tree structure gives
//  additional flexibility to the run time configuration methods through a flag
//  indicating that recursion is required. The string used to refer to the root
//  node is "/". The methods for enabling and disabling nodes are listed below:
//          - MxTraceEnableNode
//          - MxTraceDisableNode
//
//  #Trace Output Handlers#
//  The following output handlers are available to send the traces
//  to different mediums.
//          - MxTraceToStderr
//          - MxTraceToStdout
//          - MxTraceToDebugger
//
//  An application can also define its own output handler by defining a
//  function that follows the mxt_PFNTraceOutputHandler typedef and calling
//  MxTraceAddOutputHandler or MxTraceSetNewOutputHandler or by overriding
//  the default handler with MXD_TRACE_OUTPUT_HANDLER_OVERRIDE.
//
//  #Trace Format Handlers#
//  The format handler is the function that processes the trace and puts it in
//  a pre-defined format before the result is given to the actual output
//  handler. The Framework provides a default format handler. Refer to
//  mxt_PFNTraceFormatHandler for a complete description of the default
//  format handler. The following methods provide run time control over the
//  default format handler configuration:
//      - MxTraceEnableDefaultFields: Enables default tracing fields.
//      - MxTraceEnableSyslogFields: Enables syslog tracing fields.
//      - MxTraceEnableAllFields: Enables all tracing fields.
//      - MxTraceDisableAllFields: Disables all tracing fields.
//      - MxTraceEnableField: Enables a specific tracing field.
//      - MxTraceDisableField: Disables a specific tracing field.
//
//  The default format handler can be overridden with an application format
//  handler by defining MXD_TRACE_FORMAT_HANDLER_OVERRIDE to a function that
//  follows the mxt_PFNTraceFormatHandler type.
//
//  <CODE>
//     // This code should be included in the PreMxConfig.h configuration file.
//     //----------------------------------------------------------------------
//     #define MXD_TRACE_FORMAT_HANDLER_OVERRIDE MyFormatHandler.
//  </CODE>
//
//  It can also be overriden at run time using the MxTraceSetNewFormatHandler
//  method.
//
//  #Call Stack Handlers#
//  For a limited OS/Architecture combinations, it is possible to print the
//  function call stack traces. Call stack handlers are used for this purpose.
//  They can be configured at compile time using the
//  MXD_TRACE_CALLSTACK_HANDLER_OVERRIDE and MXD_TRACE_ENABLE_MACRO_CALLSTACK
//  or at run time using MxTraceSetNewCallStackHandler.
//
//  Finally, the framework offers other compile time and run time configuration
//  macros and methods.
//
//  #Other Compile Time Configuration Macros#
//      - MXD_TRACE_BUFFER_CAPACITY: Defines the maximal trace size.
//      - MXD_TRACE_PROGNAME: Defines a program name to be prefixed to traces.
//      - MXD_TRACE_BACKTRACE_CAPACITY: Defines the backtrace buffer for Linux.
//      - In addition, a new trace buffering mechanism may be defined with one
//        of the following macros: MXD_TRACE_USE_STACK_BUFFER,
//        MXD_TRACE_USE_DYNAMIC_ALLOC_BUFFER, MXD_TRACE_USE_MEMORY_QUEUE_BUFFER.
//
//  #M5T product's internal usage of the tracing system#
//  The Framework and all other M5T products use the tracing system to ease
//  debugging and support. All products use the trace system in the same way.
//
//  - All traces use trace nodes specific to the product:
//    - The Framework uses the trace nodes detailed in
//      <LINK g_stFramework, "g_stFramework and descendant tracing nodes">
//  - Trace levels are used as follows:
//  <TABLE 40c%,60c% noborder>
//    Level                          Type
//    =====                          ====
//    0                              Unused (Reserved for future use)
//    <b>1</b>                       <b>Critical situations (imminent crash)</b>
//    <b>2</b>                       <b>Error situations (failed operation)</b>
//    3                              Unused (Reserved for future use)
//    <b>4</b>                       <b>Basic Debugging information</b>
//    5                              Unused (Reserved for future use)
//    <b>6</b>                       <b>Debug Method/Function Entry</b>
//    <b>7</b>                       <b>Debug Method/Function Exit</b>
//    <b>8</b>                       <b>Extra Debugging information</b>
//    9                              Unused (Reserved for future use)
//  </TABLE>
//
//  #Using Framework Tracing Mechanism in Your Application#
//  If you would like to use the framework tracing mechanism to output traces
//  from your own application, you need to decide how to categorize your traces,
//  for example by package, by folder, by module, or by any other criterion,
//  then create your tracing hierarchy and attach it to the root node. This way,
//  you have full control on your customized tracing mechanism.
//
//==============================================================================

//==============================================================================
//====  CONFIGURATION  =========================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN

// Note: the following configuration defines are documented in MxConfig.h
#if !defined(MXD_TRACE_MAX_NB_OF_OUTPUT_HANDLERS)
    #define MXD_TRACE_MAX_NB_OF_OUTPUT_HANDLERS 1
#endif

#if !defined(MXD_TRACE_BUFFER_CAPACITY)
    #define MXD_TRACE_BUFFER_CAPACITY 512
#endif

#if !defined(MXD_TRACE_BACKTRACE_CAPACITY)
    #define MXD_TRACE_BACKTRACE_CAPACITY 128
#endif

#if defined(MXD_TRACE_ENABLE_ALL)
    #undef  MXD_TRACE_MAX_LEVEL
    #undef  MXD_TRACEX_ENABLE_SUPPORT

    #define MXD_TRACE_MAX_LEVEL 9
    #define MXD_TRACEX_ENABLE_SUPPORT
#endif

// MXD_TRACE_ENABLE_MACRO_CALLSTACK depends on MXD_TRACEX_ENABLE_SUPPORT
// make sure it is present.
#if defined(MXD_TRACE_ENABLE_MACRO_CALLSTACK) && \
    !defined(MXD_TRACE_CALLSTACK_HANDLER_OVERRIDE)
    #undef  MXD_TRACEX_ENABLE_SUPPORT
    #define MXD_TRACEX_ENABLE_SUPPORT
#endif


#if defined(MXD_TRACE_MAX_LEVEL)
    #undef MXD_TRACE0_ENABLE_SUPPORT
    #undef MXD_TRACE1_ENABLE_SUPPORT
    #undef MXD_TRACE2_ENABLE_SUPPORT
    #undef MXD_TRACE3_ENABLE_SUPPORT
    #undef MXD_TRACE4_ENABLE_SUPPORT
    #undef MXD_TRACE5_ENABLE_SUPPORT
    #undef MXD_TRACE6_ENABLE_SUPPORT
    #undef MXD_TRACE7_ENABLE_SUPPORT
    #undef MXD_TRACE8_ENABLE_SUPPORT
    #undef MXD_TRACE9_ENABLE_SUPPORT
    #if (MXD_TRACE_MAX_LEVEL >= 0)
        #define MXD_TRACE0_ENABLE_SUPPORT
    #endif
    #if (MXD_TRACE_MAX_LEVEL >= 1)
        #define MXD_TRACE1_ENABLE_SUPPORT
    #endif
    #if (MXD_TRACE_MAX_LEVEL >= 2)
        #define MXD_TRACE2_ENABLE_SUPPORT
    #endif
    #if (MXD_TRACE_MAX_LEVEL >= 3)
        #define MXD_TRACE3_ENABLE_SUPPORT
    #endif
    #if (MXD_TRACE_MAX_LEVEL >= 4)
        #define MXD_TRACE4_ENABLE_SUPPORT
    #endif
    #if (MXD_TRACE_MAX_LEVEL >= 5)
        #define MXD_TRACE5_ENABLE_SUPPORT
    #endif
    #if (MXD_TRACE_MAX_LEVEL >= 6)
        #define MXD_TRACE6_ENABLE_SUPPORT
    #endif
    #if (MXD_TRACE_MAX_LEVEL >= 7)
        #define MXD_TRACE7_ENABLE_SUPPORT
    #endif
    #if (MXD_TRACE_MAX_LEVEL >= 8)
        #define MXD_TRACE8_ENABLE_SUPPORT
    #endif
    #if (MXD_TRACE_MAX_LEVEL >= 9)
        #define MXD_TRACE9_ENABLE_SUPPORT
    #endif
#endif

// Determine if the tracing mechanism is enabled or not based on the configuration
// defines.
#undef MXD_TRACE_ENABLE_SUPPORT
#if defined(MXD_TRACE0_ENABLE_SUPPORT) || \
    defined(MXD_TRACE1_ENABLE_SUPPORT) || \
    defined(MXD_TRACE2_ENABLE_SUPPORT) || \
    defined(MXD_TRACE3_ENABLE_SUPPORT) || \
    defined(MXD_TRACE4_ENABLE_SUPPORT) || \
    defined(MXD_TRACE5_ENABLE_SUPPORT) || \
    defined(MXD_TRACE6_ENABLE_SUPPORT) || \
    defined(MXD_TRACE7_ENABLE_SUPPORT) || \
    defined(MXD_TRACE8_ENABLE_SUPPORT) || \
    defined(MXD_TRACE9_ENABLE_SUPPORT) || \
    defined(MXD_TRACEX_ENABLE_SUPPORT )
    #define MXD_TRACE_ENABLE_SUPPORT
#endif

// Default behavior is to use the stack buffer.
#if defined(MXD_TRACE_ENABLE_SUPPORT)            && \
    !defined(MXD_TRACE_USE_MEMORY_QUEUE_BUFFER)  && \
    !defined(MXD_TRACE_USE_STACK_BUFFER)         && \
    !defined(MXD_TRACE_USE_DYNAMIC_ALLOC_BUFFER) && \
    !defined(MXD_TRACE_BUFFER_OVERRIDE)
    #define MXD_TRACE_USE_STACK_BUFFER
#endif

// Default behaviour is to have nodes enabled by default.
#if !defined(MXD_TRACE_NODES_ENABLED_AT_REGISTRATION )
    #define MXD_TRACE_NODES_ENABLED_AT_REGISTRATION  true
#endif
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//<GROUP BASIC_ENUMS>
//
// Summary:
//  Defines the possible trace levels.
//
// Description:
//  This enumeration defines the possible levels associated with each trace. The
//  levels are predefined and are not meant to be augmented by the application.
//
//  Of course, a trace cannot belong to the level eLEVEL_NONE.
//
//  Location:
//    Defined in Basic/MxTrace.h but must include Config/MxConfig.h to
//    access this.
//
//  See also:
//      <LINK GENERAL_TRACING_CONFIGURATION, General tracing configuration>,
//      MXD_TRACE0_ENABLE_SUPPORT, MxTraceEnableNodeMaxLevels,
//      MxTraceEnableNodeLevel
//
//==============================================================================
typedef enum
{
    //Description:
    // None level. Should never be used.
    eLEVEL_NONE =   0x00000000,
    //Description:
    // Tracing level 0.
    eLEVEL0     =   0x00000001,
    //Description:
    // Tracing level 1.
    eLEVEL1     =   0x00000002,
    //Description:
    // Tracing level 2.
    eLEVEL2     =   0x00000004,
    //Description:
    // Tracing level 3.
    eLEVEL3     =   0x00000008,
    //Description:
    // Tracing level 4.
    eLEVEL4     =   0x00000010,
    //Description:
    // Tracing level 5.
    eLEVEL5     =   0x00000020,
    //Description:
    // Tracing level 6.
    eLEVEL6     =   0x00000040,
    //Description:
    // Tracing level 7.
    eLEVEL7     =   0x00000080,
    //Description:
    // Tracing level 8.
    eLEVEL8     =   0x00000100,
    //Description:
    // Tracing level 9.
    eLEVEL9     =   0x00000200,
    //Description:
    // Tracing level ALL. Used to specify all levels at once.
    eLEVEL_ALL  =   (int)0xFFFFFFFF
} EMxTraceLevel;

//==============================================================================
//<GROUP BASIC_ENUMS>
//
// Summary:
//  Defines the possible trace fields.
//
// Description:
//  This enumeration defines the possible fields associated with each trace. The
//  fields are predefined and are not meant to be augmented by the application.
//  The fields can be enabled and disabled at run time with the functions
//  MxTraceEnableField and MxTraceDisableField.
//
//  Location:
//    Defined in Basic/MxTrace.h but must include Config/MxConfig.h to
//    access this.
//
//  See also:
//      MxTraceEnableField, MxTraceDisableField, MxTraceEnableAllFields
//
//==============================================================================
typedef enum
{
    // Description:
    // Syslog Type Level field.
    eMXTFIELD_PRI,
    // Description:
    // Syslog Date and Time field.
    eMXTFIELD_DATE_TIME,
    // Description:
    // Hostname field.
    eMXTFIELD_HOSTNAME,
    // Description:
    // Tracing level.
    eMXTFIELD_TRACINGLEVEL,
    // Description:
    // Program name field.
    eMXTFIELD_PROGNAME,
    // Description:
    // Process ID field.
    eMXTFIELD_PROCESSID,
    // Description:
    // Thread name field.
    eMXTFIELD_THREADID,
    // Description:
    // Timestamp field.
    eMXTFIELD_TIME_STAMP,
    // Description:
    // Sequence number field.
    eMXTFIELD_SEQUENCE_NO,
    // Description:
    // Last field. Used as an index only.
    eMXTFIELD_LAST_FIELD
} EMxTraceField;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_TRACE_ENABLE_SUPPORT)
//M5T_INTERNAL_USE_BEGIN
typedef struct STraceNode
{
#if defined(__cplusplus)
    operator STraceNode*()
    {
        return this;
    }
#endif
    bool                m_bEnabled;
    EMxTraceLevel       m_eLevelEnabled;
    struct STraceNode*  m_pstNextNode;
    struct STraceNode*  m_pstChildNode;
    const char*         m_pszName;
} STraceNode;

#else
typedef uint8_t STraceNode;

#endif // #if defined(MXD_TRACE_ENABLE_SUPPORT)

#ifdef __cplusplus
extern "C" {
#endif

extern STraceNode g_stTraceRoot;

#ifdef __cplusplus
}
#endif
//M5T_INTERNAL_USE_END

//==============================================================================
//==
//==  mxt_PFNTraceFormatHandler
//==
//==============================================================================
//<GROUP BASIC_TYPES>
//
//  Summary:
//      Defines the prototype for a trace format handler.
//
//  Parameters:
//      eLevel:
//          The trace's tracing level from the enumeration EMxTraceLevel.
//      uTraceUniqueId:
//          The trace's unique Id.
//      pszTraceData:
//          A pointer to the buffer to be used for tracing.
//      pszMsgFormat:
//          A pointer to a null-terminated character string that contains the
//          'printf' format of the user's trace. This parameter should NOT be
//          NULL.
//      args:
//          The variable list of arguments associated with the format in
//          pszMsgFormat.
//      uTraceSequenceNumber:
//          The sequence number for the trace that is to be formatted. The
//          sequence number allows to detect missing traces.
//
//  Returns:
//    Size of the data actually written in the buffer.
//
//  Description:
//      This data type defines a function prototype for a trace format handler,
//      which is responsible for building the actual trace to output by
//      mxt_PFNTraceOutputHandler. It is recommended that all trace format
//      handlers check if pszTraceData is not NULL before attempting to use it.
//
//  #Default format handler#
//  The default format handler prepares traces with the following format:
//
//  <CODE>
//      \<01\> \<02\> \<03\> |\<04\>|\<05\>|\<06\>|\<07\>|\<08\>|\<09\>|\<10\>
//  </CODE>
//
//  The numeric values are printed in decimal (base 10), except if otherwise
//  noted.
//
//  The separator "|" is MANDATORY even if a field is disabled. It allows to
//  keep track of what is being traced.
//
//  The two digit number inside the \<00\> refers to the position of a given
//  field inside the entire trace.
//
//  The default trace format handler is adapted to support the SYSLOG format.
//  The SYSLOG format requires that a trace is made up of a header followed by
//  trace data. Therefore, with respect to the SYSLOG format, the fields 01, 02,
//  and 03 make up the SYSLOG header while the following "|\<04\>...\<11\>"
//  consists in the SYSLOG trace data. Again, the space (' ') is used as
//  separator between the fields 01-02 and 02-03 to follow the SYSLOG format.
//
//  #\<01\> - Syslog PRI#
//  The Syslog PRI is calculated according to RFC 3164: Facility * 8 + Severity.
//  The Facilty is always user-level messages (1). The syslog severity is
//  calculated from the trace level (EMxTraceLevel). For example, from eLEVEL6,
//  this yields PRI <14>, since (Facility * 8) + Severity = (1 * 8) + 6 = 14.
//  The following is a list of all the possible PRIs for all trace levels:
//       - eLEVEL0 ->    <8>,
//       - eLEVEL1 ->    <9>,
//       - eLEVEL2 ->    <10>,
//       - eLEVEL3 ->    <11>,
//       - eLEVEL4 ->    <12>,
//       - eLEVEL5 ->    <13>,
//       - eLEVEL6 ->    <14>,
//       - eLEVEL7 ->    <15>,
//       - eLEVEL8 ->    <15>,
//       - eLEVEL9 ->    <15>.
//
//  This field is enabled by default.
//
//  #\<02\> - Local Date and Time#
//  Local Time and Date in Syslog format ("Mmm dd hh:mm:ss"). This field is not
//  enabled by default.
//
//  #\<03\> - Hostname #
//  The local hostname or IP address. This field is not enabled by default. See
//  how to enable/disable it below with eMXTFIELD_HOSTNAME.
//
//  #\<04\> - Tracing level #
//  The tracing level used to output the trace. This field is enabled by default
//  See how to enable/disable it below with eMXTFIELD_TRACINGLEVEL.
//
//  #\<05\> - Program Name#
//  An optional global program name that can be defined using the preprocessor
//  macro MXD_TRACE_PROGNAME, exactly as \<11\>. This field is not enabled by
//  default.
//
//  #\<06\> - Process ID#
//  The process ID, in hexadecimal. Note that it is not available on every
//  operating system and yields 0 in these cases. This field is not enabled by
//  default.
//
//  #\<07\> - Thread ID#
//  The thread ID, in hexadecimal. Note that it is not available on every
//  operating system and yields 0 in these cases. This field is enabled by
//  default.
//
//  #\<08\> - System Timestamp#
//  The time stamp. It is a 64-bits integer that gives the time the system is
//  up, in milliseconds. This field is enabled by default. See how to
//  enable/disable it below with eMXTFIELD_TIME_STAMP.
//
//  #\<09\> - Sequence Number#
//  The sequence number. This number grows by one for each trace, and so each
//  trace has a different number. This field is enabled by default.
//
//  #\<10\> - Actual Trace#
//  The trace, with a newline appended. This field is always present.
//
//  All the field can be enabled or disabled using the following functions:
//
//  - MxTraceEnableField
//  - MxTraceDisableField
//  - MxTraceEnableDefaultFields
//  - MxTraceEnableSyslogFields
//  - MxTraceEnableAllFields
//  - MxTraceDisableAllFields
//  - MxTraceFieldIsEnabled
//
//  The hostname field can be changed with the following function:
//  MxTraceSetHostNameField
//
//  Location:
//    Defined in Basic/MxTrace.h but must include Config/MxConfig.h to
//    access this.
//
// See Also:
//  <LINK GENERAL_TRACING_CONFIGURATION, General Tracing Configuration>
//
//==============================================================================
typedef int (*mxt_PFNTraceFormatHandler)(IN EMxTraceLevel eLevel,
                                         IN uint32_t uTraceUniqueId,
                                         INOUT char* pszTraceData,
                                         IN const char* pszMsgFormat,
                                         IN va_list args,
                                         IN unsigned int uTraceSequenceNumber);

//==============================================================================
//==
//==  mxt_PFNTraceGenericFormatHandler
//==
//==============================================================================
//<GROUP BASIC_TYPES>
//
//  Summary:
//      Defines the prototype for a generic format handler.
//
//  Description:
//      This data type defines a function prototype for a generic format
//      handler.  For instance, this format handler can be used to format time,
//      in which case it is responsible for getting and formatting the actual
//      time stamp that is set in the trace itself.
//
//  Parameters:
//     pszBuffer: The buffer where to write the formatted text.
//     nBufferSize: The maximum number of bytes available for the formatted
//     text.
//
//  Returns:
//     The number of bytes actually written to the buffer, excluding the
//     null-terminator.
//
//  Location:
//    Defined in Basic/MxTrace.h but must include Config/MxConfig.h to
//    access this.
//
//==============================================================================
typedef int (*mxt_PFNTraceGenericFormatHandler)(INOUT char* pszBuffer,
                                                IN int nBufferSize);

//==============================================================================
//==
//==  mxt_PFNTraceOutputHandler
//==
//==============================================================================
//<GROUP BASIC_TYPES>
//
//  Summary:
//      Defines the prototype for a trace output handler.
//
//  Description:
//      This data type defines a function prototype for a trace output handler,
//      which is responsible for outputting the trace.
//
//  Parameters:
//        eLevel: The trace's tracing level.
//uTraceUniqueId: The trace's unique ID.
//      pszTrace: A pointer to a null-terminated character string that contains
//                the whole trace with a newline.
//      nMsgSize: The trace length, excluding the null-terminator.
//
//  Location:
//    Defined in Basic/MxTrace.h but must include Config/MxConfig.h to
//    access this.
//
//==============================================================================
typedef void (*mxt_PFNTraceOutputHandler)(IN EMxTraceLevel eLevel,
                                          IN uint32_t uTraceUniqueId,
                                          IN const char* pszTrace,
                                          IN int nMsgSize);

//==============================================================================
//==
//==  mxt_PFNTraceCallStackHandler
//==
//==============================================================================
//<GROUP BASIC_TYPES>
//
//  Summary:
//      Defines the prototype for a call stack trace handler.
//
//  Description:
//      This data type defines a function prototype for a call stack handler,
//      which is responsible to output a set of traces that shows the contents
//      of the call stack.
//
//  Parameters:
//         eLevel: The trace's tracing level (may be a combination of one or
//                 more levels), from the enumeration EMxTraceLevel.
// uTraceUniqueId: The trace's unique ID.
//             p1: Pointer to contextual data.
//
//  Location:
//    Defined in Basic/MxTrace.h but must include Config/MxConfig.h to
//    access this.
//
//==============================================================================
typedef void (*mxt_PFNTraceCallStackHandler)(IN EMxTraceLevel eLevel,
                                             IN uint32_t uTraceUniqueId,
                                             IN void *p1);

//==============================================================================
//====  FORWARD DECLARATIONS  ==================================================
//==============================================================================

// Summary:
//  mxt_PFNTraceOutputHandler for writing traces to the standard error.
//------------------------------------------------------------------------------
void MxTraceToStderr(IN EMxTraceLevel eLevel,
                     IN uint32_t uTraceUniqueId,
                     IN const char* pszTrace,
                     IN int nMsgSize);

// Summary:
//  mxt_PFNTraceOutputHandler for writing traces to the standard output.
//------------------------------------------------------------------------------
void MxTraceToStdout(IN EMxTraceLevel eLevel,
                     IN uint32_t uTraceUniqueId,
                     IN const char* pszTrace,
                     IN int nMsgSize);

// Summary:
//  mxt_PFNTraceOutputHandler for writing traces to a debugger.
//------------------------------------------------------------------------------
void MxTraceToDebugger(IN EMxTraceLevel eLevel,
                       IN uint32_t uTraceUniqueId,
                       IN const char* pszTrace,
                       IN int nMsgSize);

#ifdef __cplusplus
extern "C" {
#endif

// Summary:
//  Configures a new format handler.
//------------------------------------------------------------------------------
mxt_PFNTraceFormatHandler MxTraceSetNewFormatHandler(IN mxt_PFNTraceFormatHandler pfnHandler);

// Summary:
//  Configures a new time stamp format handler.
//------------------------------------------------------------------------------
mxt_PFNTraceGenericFormatHandler MxTraceSetTimeFormatHandler(IN mxt_PFNTraceGenericFormatHandler pfnNewHandler);

// Summary:
//  Configures a new system time format handler.
//------------------------------------------------------------------------------
mxt_PFNTraceGenericFormatHandler MxTraceSetSysTimeFormatHandler(IN mxt_PFNTraceGenericFormatHandler pfnNewHandler);

// Summary:
//  Configures a new call stack trace handler.
//------------------------------------------------------------------------------
mxt_PFNTraceCallStackHandler MxTraceSetNewCallStackHandler(IN mxt_PFNTraceCallStackHandler pfnNewHandler);

// Summary:
//  Sets a new output handler to replace the previous ones.
//------------------------------------------------------------------------------
void MxTraceSetNewOutputHandler(IN mxt_PFNTraceOutputHandler pfnNewHandler);

// Summary:
//  Appends the given handler to the list of trace output handlers.
//------------------------------------------------------------------------------
bool MxTraceAddOutputHandler(IN mxt_PFNTraceOutputHandler pfnNewHandler);

// Summary:
//  Removes each occurrence of the given function pointer in the list of
//  registered trace output handlers.
//------------------------------------------------------------------------------
void MxTraceRemoveOutputHandler(IN mxt_PFNTraceOutputHandler pfnHandlerToRemove);

// Summary:
//  Enables a tracing field
//------------------------------------------------------------------------------
bool MxTraceEnableField(IN EMxTraceField eTraceField);

// Summary:
//  Disables a tracing field
//------------------------------------------------------------------------------
bool MxTraceDisableField(IN EMxTraceField eTraceField);

// Summary:
//  Enables default tracing fields.
//------------------------------------------------------------------------------
bool MxTraceEnableDefaultFields();

// Summary:
//  Enables syslog tracing fields.
//------------------------------------------------------------------------------
bool MxTraceEnableSyslogFields();

// Summary:
//  Enables all tracing fields.
//------------------------------------------------------------------------------
bool MxTraceEnableAllFields();

// Summary:
//  Disables all tracing fields.
//------------------------------------------------------------------------------
bool MxTraceDisableAllFields();

// Summary:
//  Returns true if a field is active, false if it is not.
//------------------------------------------------------------------------------
bool MxTraceFieldIsEnabled(IN EMxTraceField eTraceField);

// Summary:
//  Sets the local hostname field.
//------------------------------------------------------------------------------
bool MxTraceSetHostNameField(IN const char* pszDestinationAddress);

// Summary:
//  Gets the tracing levels currently enabled.
//------------------------------------------------------------------------------
EMxTraceLevel MxTraceGetLevelsEnabled();

// Summary:
//  Adds a tracing level.
//------------------------------------------------------------------------------
void MxTraceEnableLevel(IN EMxTraceLevel eLevel);

// Summary:
//  Removes a tracing level.
//------------------------------------------------------------------------------
void MxTraceDisableLevel(IN EMxTraceLevel eLevel);

// Summary:
//  Adds all tracing levels less than or equal to the level passed.
//------------------------------------------------------------------------------
void MxTraceEnableMaxLevels(IN EMxTraceLevel eLevel);

// Summary:
//  Registers a tracing node.
//--------------------------
bool MxTraceRegisterNode(IN STraceNode* pstParentNode,
                         IN STraceNode* pstNode,
                         IN const char* pszNodeName);

// Summary:
//  Unregisters a tracing node.
//----------------------------
bool MxTraceUnregisterNode(IN STraceNode* pstParentNode,
                           IN STraceNode* pstNode);

// Summary:
//  Enables a tracing node.
//--------------------------
bool MxTraceEnableNode(IN const char* pszTraceNodePath, IN bool bRecursive);

// Summary:
//  Disables a tracing node.
//--------------------------
bool MxTraceDisableNode(IN const char* pszTraceNodePath, IN bool bRecursive);

// Summary:
//  Adds a tracing level for this node.
bool MxTraceEnableNodeLevel(IN const char* pszTraceNodePath, IN EMxTraceLevel eLevel, IN bool bRecursive);

// Summary:
//  Removes a tracing level for this node.
bool MxTraceDisableNodeLevel(IN const char* pszTraceNodePath, IN EMxTraceLevel eLevel, IN bool bRecursive);

// Summary:
//  Adds all tracing levels less than or equal to the level passed for this node.
bool MxTraceEnableNodeMaxLevels(IN const char* pszTraceNodePath,IN EMxTraceLevel eLevel, IN bool bRecursive);

// Summary:
//  Gets the tracing levels currently enabled for this node.
EMxTraceLevel MxTraceGetNodeLevelsEnabled(IN const char* pszTraceNodePath);

// Summary:
//  Prints all enabled trace nodes of the tree.
//---------------------------------------------
void MxTracePrintTree();

#if defined(MXD_TRACE_ENABLE_SUPPORT)

//M5T_INTERNAL_USE_BEGIN
// The implementation of the following functions is in the MxTraceKernel.cpp file.
#if defined(MXD_TRACE_USE_MEMORY_QUEUE_BUFFER)

#if !defined(MXD_TRACE_QUEUE_SIZE)
    #define MXD_TRACE_QUEUE_SIZE 100*1024
#endif

    #define MX_TRACE_MEMORY_QUEUE_PREFIX_SIZE (sizeof(uint16_t) + sizeof(uint32_t))
    #if defined(MXD_TRACE_USE_EXTERNAL_THREAD)
    //M5T_INTERNAL_USE_END
        // Summary:
        //  Calls the output handler.
        //----------------------------------------------------------------------
        unsigned int MxTraceCallOutputHandler(unsigned int uNumberOfMessages);
//M5T_INTERNAL_USE_BEGIN
    #else
//M5T_INTERNAL_USE_END
        // Summary:
        //  Sets the internal thread number of messages outputted per period.
        //----------------------------------------------------------------------
        void MxTraceSetThreadMessagesPerPeriod(unsigned int uTraceMessagesPerPeriod);
        // Summary:
        //  Sets the internal thread waiting period.
        //----------------------------------------------------------------------
        void MxTraceSetThreadPeriodMs(unsigned int uTracePeriodMs);
//M5T_INTERNAL_USE_BEGIN
    #endif

#endif //#if defined(MXD_TRACE_USE_MEMORY_QUEUE_BUFFER)
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INTERNAL METHODS  ======================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
extern mxt_PFNTraceCallStackHandler g_pfnTraceCallStackHandler;

// Macro to validate if a trace is allowed or not.
#undef MX_TRACE_IS_ENABLED
#define MX_TRACE_IS_ENABLED(eLevel, pstNode)  (((pstNode)->m_eLevelEnabled & (eLevel)) && (pstNode)->m_bEnabled)

void MxTracePrivateVaList(IN EMxTraceLevel eLevel,
                          IN uint32_t uTraceUniqueId,
                          IN const char* pszMsgFormat,
                          IN va_list args);

void MxTraceHexPrivate(IN EMxTraceLevel eLevel,
                       IN uint32_t uTraceUniqueId,
                       IN const uint8_t* puIn,
                       IN unsigned int uInSize);

#undef  MX_TRACE_INTERNAL_INLINE_MACRO
#define MX_TRACE_INTERNAL_INLINE_MACRO(eLevel)                                  \
    if (MX_TRACE_IS_ENABLED(eLevel, pstNode))                                   \
    {                                                                           \
        va_list args;                                                           \
        va_start(args, pszMsgFormat);                                           \
        MxTracePrivateVaList(eLevel, uTraceUniqueId, pszMsgFormat, args);       \
        va_end(args);                                                           \
    }

#undef  MX_TRACE_INTERNAL_INLINE_MACRO_HEX
#define MX_TRACE_INTERNAL_INLINE_MACRO_HEX(eLevel)                              \
    if (MX_TRACE_IS_ENABLED(eLevel, pstNode))                                   \
    {                                                                           \
        if (pszMsgFormat != NULL)                                               \
        {                                                                       \
            va_list args;                                                       \
            va_start(args, pszMsgFormat);                                       \
            MxTracePrivateVaList(eLevel, uTraceUniqueId, pszMsgFormat, args);   \
            va_end(args);                                                       \
        }                                                                       \
        MxTraceHexPrivate(eLevel, uTraceUniqueId, puIn, uInSize);               \
    }

//==============================================================================
//====  INTERNAL INLINE METHODS  ===============================================
//==============================================================================
#if defined(MXD_COMPILER_GNU_GCC) && \
    !defined(MXD_OS_VXWORKS)
#define MX_ATTRIBUTE_PRINTF(x, y) __attribute__ ((format (printf, x, y)))
#else
#define MX_ATTRIBUTE_PRINTF(x, y)
#endif

// This method is created to be called only from the MxTraceHexPrivate.
// The purpose of this method is to avoid to go through MX_TRACE(...) macro
// each time MxTraceHexPrivate outputs a line because we would be evaluating
// the condition that allows the trace everytime. Doing the test only once in
// MX_TRACE_INTERNAL_INLINE_MACRO_HEX is enough so we only convert the variable
// arguments in "..." to a va_list.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTracePrivate(IN EMxTraceLevel eLevel,
                    IN uint32_t uTraceUniqueId,
                    IN const char* pszMsgFormat, ...);

// Generic internal tracing function.
MX_ATTRIBUTE_PRINTF(4, 5)
void MxTrace(IN EMxTraceLevel eLevel,
             IN uint32_t uTraceUniqueId,
             IN STraceNode* pstNode,
             IN const char* pszMsgFormat, ...);

// Generic internal tracing function for Hex.
MX_ATTRIBUTE_PRINTF(6, 7)
void MxTraceHex(IN EMxTraceLevel eLevel,
                IN uint32_t uTraceUniqueId,
                IN STraceNode* pstNode,
                IN const uint8_t* puIn,
                IN unsigned int uInSize,
                IN const char* pszMsgFormat, ...);

// Level 0 internal tracing function.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTrace0(IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN const char* pszMsgFormat, ...);

MX_ATTRIBUTE_PRINTF(5, 6)
void MxTrace0Hex(IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const uint8_t* puIn,
                 IN unsigned int uInSize,
                 IN const char* pszMsgFormat, ...);

// Level 1 internal tracing function.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTrace1(IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN const char* pszMsgFormat, ...);

MX_ATTRIBUTE_PRINTF(5, 6)
void MxTrace1Hex(IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const uint8_t* puIn,
                 IN unsigned int uInSize,
                 IN const char* pszMsgFormat, ...);

// Level 2 internal tracing function.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTrace2(IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN const char* pszMsgFormat, ...);

MX_ATTRIBUTE_PRINTF(5, 6)
void MxTrace2Hex(IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const uint8_t* puIn,
                 IN unsigned int uInSize,
                 IN const char* pszMsgFormat, ...);

// Level 3 internal tracing function.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTrace3(IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN const char* pszMsgFormat, ...);

MX_ATTRIBUTE_PRINTF(5, 6)
void MxTrace3Hex(IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const uint8_t* puIn,
                 IN unsigned int uInSize,
                 IN const char* pszMsgFormat, ...);

// Level 4 internal tracing function.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTrace4(IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN const char* pszMsgFormat, ...);

MX_ATTRIBUTE_PRINTF(5, 6)
void MxTrace4Hex(IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const uint8_t* puIn,
                 IN unsigned int uInSize,
                 IN const char* pszMsgFormat, ...);

// Level 5 internal tracing function.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTrace5(IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN const char* pszMsgFormat, ...);

MX_ATTRIBUTE_PRINTF(5, 6)
void MxTrace5Hex(IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const uint8_t* puIn,
                 IN unsigned int uInSize,
                 IN const char* pszMsgFormat, ...);

// Level 6 internal tracing function.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTrace6(IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN const char* pszMsgFormat, ...);

MX_ATTRIBUTE_PRINTF(5, 6)
void MxTrace6Hex(IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const uint8_t* puIn,
                 IN unsigned int uInSize,
                 IN const char* pszMsgFormat, ...);

// Level 7 internal tracing function.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTrace7 (IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN const char* pszMsgFormat, ...);

MX_ATTRIBUTE_PRINTF(5, 6)
void MxTrace7Hex(IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const uint8_t* puIn,
                 IN unsigned int uInSize,
                 IN const char* pszMsgFormat, ...);

// Level 8 internal tracing function.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTrace8(IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN const char* pszMsgFormat, ...);

MX_ATTRIBUTE_PRINTF(5, 6)
void MxTrace8Hex(IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const uint8_t* puIn,
                 IN unsigned int uInSize,
                 IN const char* pszMsgFormat, ...);

// Level 9 internal tracing function.
MX_ATTRIBUTE_PRINTF(3, 4)
void MxTrace9(IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN const char* pszMsgFormat, ...);

MX_ATTRIBUTE_PRINTF(5, 6)
void MxTrace9Hex(IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const uint8_t* puIn,
                 IN unsigned int uInSize,
                 IN const char* pszMsgFormat, ...);

#undef MX_ATTRIBUTE_PRINTF

#endif //#if defined(MXD_TRACE_ENABLE_SUPPORT)

#if defined(MXD_TRACE_ENABLE_MACRO_CALLSTACK)

// Generic internal callstack tracing function.
void MxTraceCallStack(IN EMxTraceLevel eLevel, IN uint32_t uTraceUniqueId, IN STraceNode* pstNode);

// Forward declare generic internal callstack tracing function for all threads.
// Implementation in MxTraceKernel.cpp
void MxTraceAllCallStacks(IN EMxTraceLevel eLevel, IN uint32_t uTraceUniqueId, IN STraceNode* pstNode);

// Generic internal callstack tracing with coredump, when possible.
void MxTraceCallStackCoreDump(IN EMxTraceLevel eLevel, IN uint32_t uTraceUniqueId, IN STraceNode* pstNode, IN void *p1);

#endif // #if defined(MXD_TRACE_ENABLE_MACRO_CALLSTACK)

#ifdef __cplusplus
}
#endif


//==============================================================================
//====  TRACING MACROS  ========================================================
//==============================================================================

// Ugly patch to remove trace strings from object files when traces are disabled.
// DIAB and MSVC are not ISOC99 compliant regarding ellipsis usage in macro
// definitions.  We found the redirecting macro to sizeof workaround on the web.
// Unfortunately, the same strategy generates a warning under gcc. However, gcc
// allows to do it the right way. This patch also generates a warning with Code
// Composer Studio (cl6x compiler) where a different approach allows to remove
// the warning. Since C functions cannot have 0 unnamed parameter, we simply
// define a first parameter to be named followed by the ellipsis ("...") which
// is valid with all tracing macros mapping on MX_TRACE_EMPTY because they all
// have at least have 1 parameter.

#if defined(MXD_COMPILER_TI_CL6X)
    static void MxTraceEmptyImplementation(int nDiscard, ...) {}
    #define MX_TRACE_EMPTY  MxTraceEmptyImplementation
#elif (defined (MXD_COMPILER_MS_VC) && (MXD_COMPILER_MS_VC < 1600)) || defined(MXD_COMPILER_DIAB)
    // Starting MSVC 2010, this patch is not necessary. Moreover, if this patch
    // is applied, we get an internal compiler error.
    #define MX_TRACE_EMPTY  sizeof
#else
    #if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        #define MX_TRACE_EMPTY_ARGS(args...)
    #else
        #define MX_TRACE_EMPTY_ARGS(args,...)
    #endif

    #define MX_TRACE_EMPTY  MX_TRACE_EMPTY_ARGS
#endif

#undef MX_TRACE
#undef MX_TRACE_HEX
#if defined(MXD_TRACEX_ENABLE_SUPPORT)
    #define MX_TRACE        MxTrace
    #define MX_TRACE_HEX    MxTraceHex
#else
    #define MX_TRACE        MX_TRACE_EMPTY
    #define MX_TRACE_HEX    MX_TRACE_EMPTY
#endif

#undef MX_TRACE0
#undef MX_TRACE0_HEX
#undef MX_TRACE0_IS_ENABLED
#if defined(MXD_TRACE0_ENABLE_SUPPORT)
    #define MX_TRACE0                       MxTrace0
    #define MX_TRACE0_HEX                   MxTrace0Hex
    #define MX_TRACE0_IS_ENABLED(pstNode)   MX_TRACE_IS_ENABLED(eLEVEL0, pstNode)
#else
    #define MX_TRACE0                       MX_TRACE_EMPTY
    #define MX_TRACE0_HEX                   MX_TRACE_EMPTY
    #define MX_TRACE0_IS_ENABLED(pstNode)   false
#endif

#undef MX_TRACE1
#undef MX_TRACE1_HEX
#undef MX_TRACE1_IS_ENABLED
#if defined(MXD_TRACE1_ENABLE_SUPPORT)
    #define MX_TRACE1                       MxTrace1
    #define MX_TRACE1_HEX                   MxTrace1Hex
    #define MX_TRACE1_IS_ENABLED(pstNode)   MX_TRACE_IS_ENABLED(eLEVEL1, pstNode)
#else
    #define MX_TRACE1                       MX_TRACE_EMPTY
    #define MX_TRACE1_HEX                   MX_TRACE_EMPTY
    #define MX_TRACE1_IS_ENABLED(pstNode)   false
#endif

#undef MX_TRACE2
#undef MX_TRACE2_HEX
#undef MX_TRACE2_IS_ENABLED
#if defined(MXD_TRACE2_ENABLE_SUPPORT)
    #define MX_TRACE2                       MxTrace2
    #define MX_TRACE2_HEX                   MxTrace2Hex
    #define MX_TRACE2_IS_ENABLED(pstNode)   MX_TRACE_IS_ENABLED(eLEVEL2, pstNode)
#else
    #define MX_TRACE2                       MX_TRACE_EMPTY
    #define MX_TRACE2_HEX                   MX_TRACE_EMPTY
    #define MX_TRACE2_IS_ENABLED(pstNode)   false

#endif

#undef MX_TRACE3
#undef MX_TRACE3_HEX
#undef MX_TRACE3_IS_ENABLED
#if defined(MXD_TRACE3_ENABLE_SUPPORT)
    #define MX_TRACE3                       MxTrace3
    #define MX_TRACE3_HEX                   MxTrace3Hex
    #define MX_TRACE3_IS_ENABLED(pstNode)   MX_TRACE_IS_ENABLED(eLEVEL3, pstNode)
#else
    #define MX_TRACE3                       MX_TRACE_EMPTY
    #define MX_TRACE3_HEX                   MX_TRACE_EMPTY
    #define MX_TRACE3_IS_ENABLED(pstNode)   false
#endif

#undef MX_TRACE4
#undef MX_TRACE4_HEX
#undef MX_TRACE4_IS_ENABLED
#if defined(MXD_TRACE4_ENABLE_SUPPORT)
    #define MX_TRACE4                       MxTrace4
    #define MX_TRACE4_HEX                   MxTrace4Hex
    #define MX_TRACE4_IS_ENABLED(pstNode)   MX_TRACE_IS_ENABLED(eLEVEL4, pstNode)
#else
    #define MX_TRACE4                       MX_TRACE_EMPTY
    #define MX_TRACE4_HEX                   MX_TRACE_EMPTY
    #define MX_TRACE4_IS_ENABLED(pstNode)   false
#endif

#undef MX_TRACE5
#undef MX_TRACE5_HEX
#undef MX_TRACE5_IS_ENABLED
#if defined(MXD_TRACE5_ENABLE_SUPPORT)
    #define MX_TRACE5                       MxTrace5
    #define MX_TRACE5_HEX                   MxTrace5Hex
    #define MX_TRACE5_IS_ENABLED(pstNode)   MX_TRACE_IS_ENABLED(eLEVEL5, pstNode)
#else
    #define MX_TRACE5                       MX_TRACE_EMPTY
    #define MX_TRACE5_HEX                   MX_TRACE_EMPTY
    #define MX_TRACE5_IS_ENABLED(pstNode)   false
#endif

#undef MX_TRACE6
#undef MX_TRACE6_HEX
#undef MX_TRACE6_IS_ENABLED
#if defined(MXD_TRACE6_ENABLE_SUPPORT)
    #define MX_TRACE6                       MxTrace6
    #define MX_TRACE6_HEX                   MxTrace6Hex
    #define MX_TRACE6_IS_ENABLED(pstNode)   MX_TRACE_IS_ENABLED(eLEVEL6, pstNode)
#else
    #define MX_TRACE6                       MX_TRACE_EMPTY
    #define MX_TRACE6_HEX                   MX_TRACE_EMPTY
    #define MX_TRACE6_IS_ENABLED(pstNode)   false
#endif

#undef MX_TRACE7
#undef MX_TRACE7_HEX
#undef MX_TRACE7_IS_ENABLED
#if defined(MXD_TRACE7_ENABLE_SUPPORT)
    #define MX_TRACE7                       MxTrace7
    #define MX_TRACE7_HEX                   MxTrace7Hex
    #define MX_TRACE7_IS_ENABLED(pstNode)   MX_TRACE_IS_ENABLED(eLEVEL7, pstNode)
#else
    #define MX_TRACE7                       MX_TRACE_EMPTY
    #define MX_TRACE7_HEX                   MX_TRACE_EMPTY
    #define MX_TRACE7_IS_ENABLED(pstNode)   false
#endif

#undef MX_TRACE8
#undef MX_TRACE8_HEX
#undef MX_TRACE8_IS_ENABLED
#if defined(MXD_TRACE8_ENABLE_SUPPORT)
    #define MX_TRACE8                       MxTrace8
    #define MX_TRACE8_HEX                   MxTrace8Hex
    #define MX_TRACE8_IS_ENABLED(pstNode)   MX_TRACE_IS_ENABLED(eLEVEL8, pstNode)
#else
    #define MX_TRACE8                       MX_TRACE_EMPTY
    #define MX_TRACE8_HEX                   MX_TRACE_EMPTY
    #define MX_TRACE8_IS_ENABLED(pstNode)   false
#endif

#undef MX_TRACE9
#undef MX_TRACE9_HEX
#undef MX_TRACE9_IS_ENABLED
#if defined(MXD_TRACE9_ENABLE_SUPPORT)
    #define MX_TRACE9                       MxTrace9
    #define MX_TRACE9_HEX                   MxTrace9Hex
    #define MX_TRACE9_IS_ENABLED(pstNode)   MX_TRACE_IS_ENABLED(eLEVEL9, pstNode)
#else
    #define MX_TRACE9                       MX_TRACE_EMPTY
    #define MX_TRACE9_HEX                   MX_TRACE_EMPTY
    #define MX_TRACE9_IS_ENABLED(pstNode)   false
#endif

#undef MX_TRACE_CALLSTACK
#undef MX_TRACE_ALL_CALLSTACKS
#undef MX_TRACE_CALLSTACK_COREDUMP
#if defined(MXD_TRACE_ENABLE_MACRO_CALLSTACK)
    #define MX_TRACE_CALLSTACK              MxTraceCallStack
    #define MX_TRACE_ALL_CALLSTACKS         MxTraceAllCallStacks
    #define MX_TRACE_CALLSTACK_COREDUMP     MxTraceCallStackCoreDump
#else
    #define MX_TRACE_CALLSTACK              MX_TRACE_EMPTY
    #define MX_TRACE_ALL_CALLSTACKS         MX_TRACE_EMPTY
    #define MX_TRACE_CALLSTACK_COREDUMP     MX_TRACE_EMPTY
#endif

//M5T_INTERNAL_USE_END

#if 0

//==============================================================================
//==
//== STraceNode
//==
//==============================================================================
//<GROUP BASIC_STRUCTS>
//
//  Summary:
//      Defines the structure of the tracing tree node.
//
//  Description:
//      Defines the structure that is used to build the tracing tree. This tree
//      is used to show/hide traces according to the user's configuration. A
//      tracing node must be initialized and registered using the
//      MxTraceRegisterNode method.
//
//  See Also:
//      MxTraceRegisterNode, MxTraceUnregisterNode, MxTraceEnableNode,
//      MxTraceDisableNode
//
//  Location:
//    Defined in Basic/MxTrace.h but must include Config/MxConfig.h to
//    access this.
//
//==============================================================================
struct STraceNode
{
    // Description:
    // The node state.
    bool                m_bEnabled;
    // Description:
    // Node specific tracing level.
    EMxTraceLevel       m_eLevelEnabled;
    // Description:
    // The next node with same level.
    struct STraceNode*  m_pstNextNode;
    // Description:
    // The child node.
    struct STraceNode*  m_pstChildNode;
    // Description:
    // The node name.
    const char*         m_pszName;
};

//<GROUP BASIC_MACROS>
//<TITLE Tracing macros>
//==============================================================================
//
// Summary:
//   Standard tracing macros.
//
//  Parameters:
//      eLevel:
//          The level to use to issue the trace. Only present in MX_TRACE.
//
//      uTraceUniqueId:
//          The unique ID associated with the trace.
//
//      pstNode:
//          The node associated with the trace.
//
//      pszMsgFormat:
//          The trace message format as in the format argument of printf.
//
//      va_args:
//          The variable arguments corresponding to the specifier in
//          pszMsgFormat as with printf.
//
// Description:
//  This is the standard tracing macro. One different macro exists for each
//  tracing level from 0 to 9. When a level is enabled at compile time with the
//  corresponding pre-processor define (MXD_TRACE0_ENABLE_SUPPORT), if that
//  level is enabled at run time and the node pointed by pstNode is enabled, the
//  trace is sent to the format handler and then to the output handler.
//
//  Also, when a level is enabled at compile time, the parameters passed to the
//  macro are evaluated and the associated code is part of the code size. On the
//  other hand, when the level is disabled, the trace is taken out from the
//  program by the compiler and there is no code size penalty for the trace.
//
//  There is a particular case for the macro using eLevel as its first
//  parameter. This macro is useful but only in rare circumstances. It must be
//  used sparingly as it cannot be taken out by the compiler even if the level
//  passed in eLevel is disabled, except if the entire tracing mechanism is
//  disabled.
//
//  Typical usage is shown in the following code example:
//
//  <CODE>
//  // Example feature initialization code
//  STraceNode g_stExampleNode;
//  MxTraceRegisterNode (&g_stTraceRoot, &g_stExampleNode, "TEST_NODE");
//
//  ...
//
//  // Example feature execution
//  MX_TRACE0(0,&g_stExampleNode,"Trace situation 1");
//
//  unsigned int uIt = 0;
//  for (uIt = 0; uIt < 10; uIt++)
//  {
//      MX_TRACE7(0,&g_stExampleNode,"Trace situation 2 iteration %d", uIt);
//  }
//
//  ...
//
//  // Example feature finalization code
//  STraceNode g_stExampleNode;
//  MxTraceUnregisterNode (&g_stTraceRoot, &g_stExampleNode);
//  </CODE>
//
// See Also:
//  <LINK GENERAL_TRACING_CONFIGURATION, General Tracing Configuration>
//
//==============================================================================
#define MX_TRACE(IN EMxTraceLevel eLevel,
                 IN uint32_t uTraceUniqueId,
                 IN STraceNode* pstNode,
                 IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
// <COMBINE MX_TRACE>
#define MX_TRACE0(IN uint32_t uTraceUniqueId,
                  IN STraceNode* pstNode,
                  IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
// <COMBINE MX_TRACE>
#define MX_TRACE1(IN uint32_t uTraceUniqueId,
                  IN STraceNode* pstNode,
                  IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
// <COMBINE MX_TRACE>
#define MX_TRACE2(IN uint32_t uTraceUniqueId,
                  IN STraceNode* pstNode,
                  IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
// <COMBINE MX_TRACE>
#define MX_TRACE3(IN uint32_t uTraceUniqueId,
                  IN STraceNode* pstNode,
                  IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
// <COMBINE MX_TRACE>
#define MX_TRACE4(IN uint32_t uTraceUniqueId,
                  IN STraceNode* pstNode,
                  IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
// <COMBINE MX_TRACE>
#define MX_TRACE5(IN uint32_t uTraceUniqueId,
                  IN STraceNode* pstNode,
                  IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
// <COMBINE MX_TRACE>
#define MX_TRACE6(IN uint32_t uTraceUniqueId,
                  IN STraceNode* pstNode,
                  IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
// <COMBINE MX_TRACE>
#define MX_TRACE7(IN uint32_t uTraceUniqueId,
                  IN STraceNode* pstNode,
                  IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
// <COMBINE MX_TRACE>
#define MX_TRACE8(IN uint32_t uTraceUniqueId,
                  IN STraceNode* pstNode,
                  IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
// <COMBINE MX_TRACE>
#define MX_TRACE9(IN uint32_t uTraceUniqueId,
                  IN STraceNode* pstNode,
                  IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<TITLE Hexadecimal tracing macros>
//==============================================================================
//
// Summary:
//   Hexadecimal tracing macros.
//
//  Parameters:
//      eLevel:
//          The level to use to issue the trace. Only present in MX_TRACE_HEX.
//
//      uTraceUniqueId:
//          The unique ID associated with the trace.
//
//      pstNode:
//          The node associated with the trace.
//
//      puIn:
//          A pointer to the begining of hexadecimal data to trace.
//
//      uInSize:
//          The size, in bytes, of the buffer pointed by puIn.
//
//      pszMsgFormat:
//          The trace message format as in the format argument of printf.
//
//      va_args:
//          The variable arguments corresponding to the specifier in
//          pszMsgFormat as with printf.
//
// Description:
//  This is the hexadecimal tracing macro. One different macro exists for each
//  tracing level from 0 to 9. The hexadecimal macros follow the same rules as
//  the MX_TRACE macros with the exception they trace additional uInSize bytes
//  of hexadecimal data pointed by puIn.
//
//  Typical usage is shown in the following code example:
//
//  <CODE>
//  // Example feature initialization code
//  STraceNode g_stExampleNode;
//  MxTraceRegisterNode(&g_stTraceRoot, &g_stExampleNode, "TEST_NODE");
//
//  ...
//
//  // Example feature execution
//  uint32_t auHexData[] = {1,2,3,4};
//  MX_TRACE2_HEX(0, &g_stExampleNode, auHexData,sizeof(auHexData),"Trace situation 1");
//
//  unsigned int uIt = 0;
//  for (uIt = 0; uIt < 10; uIt++)
//  {
//      MX_TRACE8_HEX(0, &g_stExampleNode, auHexData, sizeof(auHexData),"Trace situation 2 iteration %d", uIt);
//  }
//
//  ...
//
//  // Example feature finalization code
//  STraceNode g_stExampleNode;
//  MxTraceUnregisterNode (&g_stTraceRoot, &g_stExampleNode);
//  </CODE>
//
// See Also:
//  <LINK GENERAL_TRACING_CONFIGURATION, General Tracing Configuration>
//
//==============================================================================
#define MX_TRACE_HEX(IN EMxTraceLevel eLevel,
                     IN uint32_t uTraceUniqueId,
                     IN STraceNode* pstNode,
                     IN const uint8_t* puIn,
                     IN unsigned int uInSize,
                     IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_HEX>
#define MX_TRACE0_HEX(IN uint32_t uTraceUniqueId,
                      IN STraceNode* pstNode,
                      IN const uint8_t* puIn,
                      IN unsigned int uInSize,
                      IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_HEX>
#define MX_TRACE1_HEX(IN uint32_t uTraceUniqueId,
                      IN STraceNode* pstNode,
                      IN const uint8_t* puIn,
                      IN unsigned int uInSize,
                      IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_HEX>
#define MX_TRACE2_HEX(IN uint32_t uTraceUniqueId,
                      IN STraceNode* pstNode,
                      IN const uint8_t* puIn,
                      IN unsigned int uInSize,
                      IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_HEX>
#define MX_TRACE3_HEX(IN uint32_t uTraceUniqueId,
                      IN STraceNode* pstNode,
                      IN const uint8_t* puIn,
                      IN unsigned int uInSize,
                      IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_HEX>
#define MX_TRACE4_HEX(IN uint32_t uTraceUniqueId,
                      IN STraceNode* pstNode,
                      IN const uint8_t* puIn,
                      IN unsigned int uInSize,
                      IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_HEX>
#define MX_TRACE5_HEX(IN uint32_t uTraceUniqueId,
                      IN STraceNode* pstNode,
                      IN const uint8_t* puIn,
                      IN unsigned int uInSize,
                      IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_HEX>
#define MX_TRACE6_HEX(IN uint32_t uTraceUniqueId,
                      IN STraceNode* pstNode,
                      IN const uint8_t* puIn,
                      IN unsigned int uInSize,
                      IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_HEX>
#define MX_TRACE7_HEX(IN uint32_t uTraceUniqueId,
                      IN STraceNode* pstNode,
                      IN const uint8_t* puIn,
                      IN unsigned int uInSize,
                      IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_HEX>
#define MX_TRACE8_HEX(IN uint32_t uTraceUniqueId,
                      IN STraceNode* pstNode,
                      IN const uint8_t* puIn,
                      IN unsigned int uInSize,
                      IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_HEX>
#define MX_TRACE9_HEX(IN uint32_t uTraceUniqueId,
                      IN STraceNode* pstNode,
                      IN const uint8_t* puIn,
                      IN unsigned int uInSize,
                      IN const char* pszMsgFormat, ...)

//<GROUP BASIC_MACROS>
//<TITLE Tracing level activation check macros>
//==============================================================================
//
//  Summary:
//   Tracing level activation check macros.
//
//  Parameters:
//      pstNode:
//          A pointer to the node to evalutate.
//
//  Returns:
//      True if the level and the node are enabled, false otherwise.
//
// Description:
//  This is a helper tracing macro. One different macro exists for each tracing
//  level from 0 to 9. When a level is enabled at compile time with the
//  corresponding pre-processor define (MXD_TRACE0_ENABLE_SUPPORT), if that
//  level is enabled at run time and the node pointed by pstNode is enabled, the
//  macro returns true.
//
//  This macro can be very helpful because it gives the possibility to validate
//  the condition used to determine if the trace is outputted before calling a
//  tracing macro. In the case it is not outputted, the processing needed to
//  prepare the data to trace can be skipped.
//
//  When a level is disabled at compile time, some compiler might even take the
//  code inside the "if" statement out from the object code when compiling with
//  the right optimization level.
//
//  Typical usage is shown in the following code example:
//
//  <CODE>
//  // Example feature initialization code
//  STraceNode g_stExampleNode;
//  MxTraceRegisterNode (&g_stTraceRoot, &g_stExampleNode, "TEST_NODE");
//
//  ...
//
//  // Example feature execution
//  MX_TRACE0(0, &g_stExampleNode,"Trace situation 1");
//
//  if (MX_TRACE7_IS_ENABLED(g_stExampleNode))
//  {
//      // Decrypt buffer to access text data that is needed to
//      // issue the trace. The decryption is costly, we want to
//      // do it only if the trace will be outputted because it
//      // is not part of normal processing.
//      Decrypt(puCipherText, pszPlainText);
//
//      MX_TRACE7(0, &g_stExampleNode,"Trace encrypted text: %s", pszPlainText);
//  }
//
//  ...
//
//  // Example feature finalization code
//  STraceNode g_stExampleNode;
//  MxTraceUnregisterNode (&g_stTraceRoot, &g_stExampleNode);
//  </CODE>
//
// See Also:
//  <LINK GENERAL_TRACING_CONFIGURATION, General Tracing Configuration>
//
//==============================================================================
#define MX_TRACE0_IS_ENABLED(STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE0_IS_ENABLED>
#define MX_TRACE1_IS_ENABLED(STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE0_IS_ENABLED>
#define MX_TRACE2_IS_ENABLED(STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE0_IS_ENABLED>
#define MX_TRACE3_IS_ENABLED(STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE0_IS_ENABLED>
#define MX_TRACE4_IS_ENABLED(STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE0_IS_ENABLED>
#define MX_TRACE5_IS_ENABLED(STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE0_IS_ENABLED>
#define MX_TRACE6_IS_ENABLED(STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE0_IS_ENABLED>
#define MX_TRACE7_IS_ENABLED(STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE0_IS_ENABLED>
#define MX_TRACE8_IS_ENABLED(STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE0_IS_ENABLED>
#define MX_TRACE9_IS_ENABLED(STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<TITLE Call stack tracing macros>
//==============================================================================
//
// Summary:
//   Call stack tracing macros.
//
// Parameters:
//      eLevel:
//          The level to use to issue the trace.
//
//      uTraceUniqueId:
//          The unique ID associated with the trace.
//
//      pstNode:
//          The node associated with the trace.
//
//      p1:
//          Always set to NULL. Only present in MX_TRACE_CALLSTACK_COREDUMP.
//
// Description:
//   These special tracing macros produce (for a limited OS/Architecture
//   combination) a series of traces showing the function call stack. The actual
//   trace contains the appropriate function name and/or address, as in the
//   example below.
//
//   <CODE>
//      |Start dumping call stack
//      |Build/TestFw21.exe [0x10075370]
//      |Build/TestFw21.exe [0x10078210]
//      |Build/TestFw21.exe [0x10076c8c]
//      |Build/TestFw21.exe [0x10077374]
//      |Build/TestFw21.exe(main+0x188) [0x10077b6c]
//      |/lib/libc.so.6(__libc_start_main+0x144) [0xfd35be4]
//      |End dumping call stack
//   </CODE>
//
//   The tracing function behind this macro might not be able to print
//   all the function names. You may need to have the map file in hand
//   (and/or the program disassembly) to interpret the call stack trace.
//
//  #WARNING:#
//  Calling MX_TRACE_CALLSTACK may slow the execution of your code and change
//  the application timing. It should only be used in appropriate situations.
//
//   MX_TRACE_CALLSTACK shows the call stack of the thread currently executing.
//
//   MX_TRACE_ALL_CALLSTACK shows the call stack of all the threads currently in
//   the system.
//
//   MX_TRACE_CALLSTACK_COREDUMP shows the state of the registers and the call
//  stack of the thread currently executing.
//
// See Also:
//  <LINK GENERAL_TRACING_CONFIGURATION, General Tracing Configuration>
//
//==============================================================================
#define MX_TRACE_CALLSTACK(IN EMxTraceLevel eLevel,
                           IN uint32_t uTraceUniqueId,
                           IN STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_CALLSTACK>
#define MX_TRACE_ALL_CALLSTACKS(IN EMxTraceLevel eLevel,
                                IN uint32_t uTraceUniqueId,
                                IN STraceNode* pstNode)

//<GROUP BASIC_MACROS>
//<COMBINE MX_TRACE_CALLSTACK>
#define MX_TRACE_CALLSTACK_COREDUMP(IN EMxTraceLevel eLevel,
                                    IN uint32_t uTraceUniqueId,
                                    IN STraceNode* pstNode,
                                    IN void *p1)

#endif // #if 0

#endif // #ifndef MXG_MXTRACE_H
