//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_RESULT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_RESULT_H
//M5T_INTERNAL_USE_END

// Verify that this header file is included by MxConfig.h only. This
// verification is performed because we state that the result mechanism
// configuration is done through the PreMxConfig.h, which is only included by
// MxConfig.h. If we were to allow the direct inclusion of Result.h, we would
// loose the configuration done in PreMxConfig.h.
//-----------------------------------------------------------------------------
#if !defined(MXG_MXCONFIG_H)
    #error This file must be included through MxConfig.h only!
#endif

#ifndef MXG_PKGID_H
#include "Basic/PkgId.h"
#endif

#include "Basic/CAlignedVariableStorage.h"

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
// Level (Internal used only)
//------------------------------------------------------------------------------
#define MX_RESULT_LEVEL_BIT_POSITION    30
#define MX_RESULT_LEVEL_NB_OF_BITS      2
#define MX_RESULT_LEVEL_INFO            0
#define MX_RESULT_LEVEL_INFO_VALUE      ( MX_RESULT_LEVEL_INFO << \
                                          MX_RESULT_LEVEL_BIT_POSITION )
#define MX_RESULT_LEVEL_WARNING         1
#define MX_RESULT_LEVEL_WARNING_VALUE   ( MX_RESULT_LEVEL_WARNING << \
                                          MX_RESULT_LEVEL_BIT_POSITION )
#define MX_RESULT_LEVEL_ERROR           2
// Some compilers do not allow shifting on a signed value if it changes the
// sign. That is why the unsigned cast is done. This however changes the macro
// from signed to unsigned and gives compilation warnings. That is why a second
// cast in signed int is done.
#define MX_RESULT_LEVEL_ERROR_VALUE     (int) ( ((unsigned int) MX_RESULT_LEVEL_ERROR) << \
                                          MX_RESULT_LEVEL_BIT_POSITION )
#define MX_RESULT_LEVEL_CRITICAL        3
#define MX_RESULT_LEVEL_CRITICAL_VALUE  (int) ( ((unsigned int) MX_RESULT_LEVEL_CRITICAL) << \
                                          MX_RESULT_LEVEL_BIT_POSITION )
#define MX_RESULT_LEVEL_MASK        (int) ( (( ((unsigned int) 1) << MX_RESULT_LEVEL_NB_OF_BITS)-1) << \
                                      MX_RESULT_LEVEL_BIT_POSITION )

// Package ID (Internal used only)
//------------------------------------------------------------------------------
#define MX_RESULT_PKG_ID_BIT_POSITION   14
#define MX_RESULT_PKG_ID_NB_OF_BITS     10
#define MX_RESULT_PKG_MASK          ( ((1 << MX_RESULT_PKG_ID_NB_OF_BITS)-1) << \
                                      MX_RESULT_PKG_ID_BIT_POSITION )

// Code ID (Internal used only)
//------------------------------------------------------------------------------
#define MX_RESULT_CODE_ID_BIT_POSITION  0
#define MX_RESULT_CODE_ID_NB_OF_BITS    14
#define MX_RESULT_CODE_ID_MASK      ( ((1 << MX_RESULT_CODE_ID_NB_OF_BITS)-1) << \
                                      MX_RESULT_CODE_ID_BIT_POSITION )

// Maximum number of shared code IDs (Internal used only)
//------------------------------------------------------------------------------
#define MX_RESULT_NB_MAX_SHARED_CODE_ID     1024

// Maximum number of private code IDs for each pkg (Internal use only)
//------------------------------------------------------------------------------
#define MX_RESULT_NB_MAX_PRIVATE_CODE_ID_PER_PKG    \
            ((1<<MX_RESULT_CODE_ID_NB_OF_BITS)-MX_RESULT_NB_MAX_SHARED_CODE_ID)
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//<GROUP BASIC_ENUMS>
//
// Summary:
//  Defines the shared success-info IDs.
//
// Description:
//  This enumeration defines the shared success-info code IDs.
//
//  The first value has to be resS_OK.  The success-information codes are meant
//  to carry additional information when a function/method succeeds.  It is
//  recommended to use the shared resS_OK result when no additional information
//  needs to be returned.
//
//  Location:
//    Defined in Basic/Result.h but must include Config/MxConfig.h to access
//    this.
//
// See Also:
//  mxt_result
//
//==EDOC========================================================================
enum EMxResultSharedSuccessInfoCodeId
{
    // Description:
    // First success-info result code. Used as an index only.
    resSI_FIRST          = MX_RESULT_LEVEL_INFO_VALUE,            // LL = 00, PkgId = 0, Code ID = 0

    //-- resS_OK is a shared success code id without information. Code ID 0 is
    //-- reserved for this purpose. It must be set to 0, otherwize some other
    //-- part of the code won't work.

    // Description:
    // Success-info code result indicating sucessfull completion.
    resS_OK             = resSI_FIRST,
    // Description:
    // Success-info code result indicating a TRUE value.
    resSI_TRUE,
    // Description:
    // Success-info code result indicating a FALSE value.
    resSI_FALSE,
    // Description:
    // Last success-info result code. Used as an index only.
    resSI_LAST          = resSI_FALSE
};

//==SDOC========================================================================
//<GROUP BASIC_ENUMS>
//
// Summary:
//  This enumeration defines the shared success-warning code IDs.
//
// Description:
//  This enumeration defines the shared success-warning code IDs.
//
//  The success-warning codes are meant to carry additional information when a
//  function/method succeeds, but with the addition of raising a yellow flag.
//
//  Location:
//    Defined in Basic/Result.h but must include Config/MxConfig.h to access
//    this.
//
// See Also:
//  mxt_result
//
//==EDOC========================================================================
enum EMxResultSharedSuccessWarningCodeId
{
    //-- LEV = 01, PkgId = 0, Code ID = 1

    // Description:
    // First success-warning result code. Used as an index only.
    resSW_FIRST         = (MX_RESULT_LEVEL_WARNING_VALUE | 1),
    // Description:
    // Generic warning.
    resSW_WARNING = resSW_FIRST,
    // Description:
    // No operation has been performed.
    resSW_NOTHING_DONE,
    //Description:
    // An asynchronous operation is in progress.
    resSW_ASYNC_PROCESSING,
    // Description:
    // Last success-warning result code. Used as an index only.
    resSW_LAST          = resSW_ASYNC_PROCESSING
};

//==SDOC========================================================================
//<GROUP BASIC_ENUMS>
//
// Summary:
//  This enumeration defines the shared fail-error code IDs.
//
// Description:
//  This enumeration defines the shared fail-error code IDs.
//
//  The fail-error codes are meant to carry additional information when a
//  function/method fails or encounters an unhandled condition. If no additional
//  information is to be retuned, it is recommended to use the shared resFE_FAIL
//  return code ID.
//
//  Location:
//    Defined in Basic/Result.h but must include Config/MxConfig.h to access
//    this.
//
// See Also:
//  mxt_result
//
//==EDOC========================================================================
enum EMxResultSharedFailErrorCodeId
{
    //-- LEV = 10, PkgId = 0, Code ID = 1

    // Description:
    // First fail-error result code. Used as an index only.
    resFE_FIRST         = (int)(MX_RESULT_LEVEL_ERROR_VALUE | 1),
    // Description:
    // Fail-error code indicating a failure.
    resFE_FAIL          = resFE_FIRST,
    // Description:
    // Fail-error code result indicating an invalid state.
    resFE_INVALID_STATE,
    // Description:
    // Fail-error code result indicating an invalid argument.
    resFE_INVALID_ARGUMENT,
    // Description:
    // Fail-error code result indicating a non-implemented feature.
    resFE_NOT_IMPLEMENTED,
    // Description:
    // Fail-error code result indicating a NULL pointer.
    resFE_NULL_POINTER,
    // Description:
    // Fail-error code result indicating an unexpected error.
    resFE_UNEXPECTED,
    // Description:
    // Fail-error code result indicating an out of memory error.
    resFE_OUT_OF_MEMORY,
    // Description:
    // Fail-error code result indicating an access denied error.
    resFE_ACCESS_DENIED,
    // Description:
    // Fail-error code result indicating a duplicate error.
    resFE_DUPLICATE,
    // Description:
    // Fail-error code result indicating an abort error.
    resFE_ABORT,
    // Description:
    // Fail-error code result indicating a timeout condition.
    resFE_TIMEOUT,
    // Description:
    // Fail-error code result indicating an element not found error.
    resFE_NOT_FOUND,
    // Description:
    // Last fail-error result code. Used as an index only.
    resFE_LAST           = resFE_NOT_FOUND
};

//==SDOC========================================================================
//<GROUP BASIC_ENUMS>
//
// Summary:
//  This enumeration defines the shared fail-critical code IDs.
//
// Description:
//  This enumeration defines the shared fail-critical code IDs.
//
//  The fail-critical codes are meant to carry additional information when a
//  function/method encounters an unhandled condition that can leave the system
//  unstable or the execution can not resume without possible consequences.  It
//  is also used to notify the caller of unrecoverable conditions.
//
//  Location:
//    Defined in Basic/Result.h but must include Config/MxConfig.h to
//    access this.
//
// See Also:
//  mxt_result
//
//==EDOC========================================================================
enum EMxResultSharedFailCriticalCodeId
{
    //-- LEV = 11, PkgId = 0, Code ID = 1

    // Description:
    // First fail-critical result code. Used as an index only.
    resFC_FIRST          = (int)(MX_RESULT_LEVEL_CRITICAL_VALUE | 1),
    // Description:
    // Generic critical failure.
    resFC_CRITICAL = resFC_FIRST,
    // Description:
    // Last fail-critical result code. Used as an index only.
    resFC_LAST           = resFC_CRITICAL
};

//M5T_INTERNAL_USE_BEGIN

// Used to build msg tables and to index into them ( for internaly used only ).
//------------------------------------------------------------------------------
enum EMxResultCodeIdType
{
    eMX_RESULT_CODE_ID_TYPE_SUCCESS_INFORMATION = 0,
    eMX_RESULT_CODE_ID_TYPE_SUCCESS_WARNING,
    eMX_RESULT_CODE_ID_TYPE_FAIL_ERROR,
    eMX_RESULT_CODE_ID_TYPE_FAIL_CRITICAL,
    eMX_RESULT_CODE_ID_TYPE_SIZE
};

//==============================================================================
//====  FORWARD DECLARATIONS  ==================================================
//==============================================================================

#if defined (MXD_RESULT_ENABLE_ERROR_MESSAGES)

// Pure C code doesn't allow forward declaration
#if defined (__cplusplus)

struct SResultPkgMsg;
// For internal use only.  Useless when error message support is disabled.
void MxResultMapMsg( IN EMxPackageId ePkgId,
                     IN SResultPkgMsg* pstResultPkgMsg,
                     IN EMxResultCodeIdType eCodeIdType );

#endif // #if defined (__cplusplus)

#endif // #if defined (MXD_RESULT_ENABLE_ERROR_MESSAGES)


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
typedef int32_t mxt_result;

#if defined (MXD_RESULT_ENABLE_ERROR_MESSAGES)

// Pure C code doesn't allow methods in structures
#if defined (__cplusplus)

// Msg structure (for internal use only).  Useless when error message support is disabled.
struct SResultPkgMsg
{
    uint16_t           m_uNbMsg;     // Number of msgs in a package.
    const char* const* m_ppszMsgTbl; // Pointer to table of msgs for this package.

    //==========================================================================
    //==
    //==  SResultPkgMsg
    //==
    //==========================================================================
    //
    //  Summary:
    //    Constructor that initializes messages for a package.
    //
    //  Parameters:
    //    ePkgId:
    //      Package ID.
    //
    //    uNbMsg:
    //      Number of messages.
    //
    //    apszMsg:
    //      Pointer to messages table. Table type must be: char* table[uNbMsg].
    //
    //  Returns:
    //    None
    //
    //  Description:
    //    Initializes object members.
    //
    //  Note:
    //
    //==========================================================================
    SResultPkgMsg( IN EMxPackageId ePkgId,
                   IN uint16_t uNbMsg,
                   IN const char* const* apszMsg,
                   IN EMxResultCodeIdType eCodeIdType )
    {
        MX_ASSERT( ePkgId < eMX_PKG_SIZE );
        m_uNbMsg = uNbMsg;
        m_ppszMsgTbl = apszMsg;
        MxResultMapMsg( ePkgId, this, eCodeIdType );
    }
};

// A compiler error was found with the Tornado 2.2 compiler for MIPS
// (GNU CPP version 2.96-mips3264-010729 (2.96+) VxWorks 5.5). With this
// compiler, when compiling the pkg_Framework 2.1 for VxWorks 5.5 and MIPS,
// the following error occurs:
//
//     ..\Sources\Basic\Result.h:361: syntax error before `;
//
// We found that by defining MXD_ENABLE_NAMESPACE the problem disappears. But
// it must be possible to compile the application without enabling the
// namespaces.
//
// When the namespaces are disabled, MX_NAMESPACE and MXD_GNS are both expanded
// to nothing. So MX_NAMESPACE(MXD_GNS) is simply expanded to nothing. We found
// out that by adding a space between "MX_NAMESPACE(MXD_GNS)" and
// "CAlignedVariableStorage" the problem disappears. This is certainly a
// compiler problem and it is probably related to macro expansion.
typedef MX_NAMESPACE(MXD_GNS) CAlignedVariableStorage<SResultPkgMsg> SResultPkgMsgAlignedStorage;

#endif // #if defined (__cplusplus)

#endif // #if defined (MXD_RESULT_ENABLE_ERROR_MESSAGES)

#define MX_RGET_PKG_BASE_SUCCESS_INFO_CODE_ID( ePkgId )                         \
            (                                                                   \
              ( ( ePkgId * ( 1 << MX_RESULT_PKG_ID_BIT_POSITION ) ) +           \
                MX_RESULT_NB_MAX_SHARED_CODE_ID                                 \
              ) |                                                               \
              MX_RESULT_LEVEL_INFO_VALUE                                        \
            )

#define MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID( ePkgId )                      \
            (                                                                   \
              ( ( ePkgId * ( 1 << MX_RESULT_PKG_ID_BIT_POSITION ) ) +           \
                MX_RESULT_NB_MAX_SHARED_CODE_ID                                 \
              ) |                                                               \
              MX_RESULT_LEVEL_WARNING_VALUE                                     \
            )

#define MX_RGET_PKG_BASE_FAIL_ERROR_CODE_ID( ePkgId )                           \
            (                                                                   \
              (  ( ePkgId * ( 1 << MX_RESULT_PKG_ID_BIT_POSITION ) ) +          \
                 MX_RESULT_NB_MAX_SHARED_CODE_ID                                \
              ) |                                                               \
              MX_RESULT_LEVEL_ERROR_VALUE                                       \
            )

#define MX_RGET_PKG_BASE_FAIL_CRITICAL_CODE_ID( ePkgId )                        \
            (                                                                   \
              (  ( ePkgId * ( 1 << MX_RESULT_PKG_ID_BIT_POSITION ) ) +          \
                 MX_RESULT_NB_MAX_SHARED_CODE_ID                                \
              ) |                                                               \
              MX_RESULT_LEVEL_CRITICAL_VALUE                                    \
            )

#if defined (MXD_RESULT_ENABLE_ERROR_MESSAGES)

#define MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN( ePkgId )                           \
            MX_R_PKG_MSG_TBL_BEGIN(                                             \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_SUCCESS_INFORMATION )

#define MX_R_PKG_SUCCESS_INFO_MSG_TBL_END( ePkgId )                             \
            MX_R_PKG_MSG_TBL_END(                                               \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_SUCCESS_INFORMATION )

#define MX_R_PKG_SUCCESS_INFO_MSG_TBL_INITIALIZE( ePkgId )                      \
            MX_R_PKG_MSG_TBL_INITIALIZE(                                        \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_SUCCESS_INFORMATION )

#define MX_R_PKG_SUCCESS_INFO_MSG_TBL_FINALIZE( ePkgId )                        \
            MX_R_PKG_MSG_TBL_FINALIZE(                                          \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_SUCCESS_INFORMATION )

#define MX_R_PKG_SUCCESS_WARNING_MSG_TBL_BEGIN( ePkgId )                        \
            MX_R_PKG_MSG_TBL_BEGIN(                                             \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_SUCCESS_WARNING )

#define MX_R_PKG_SUCCESS_WARNING_MSG_TBL_END( ePkgId )                          \
            MX_R_PKG_MSG_TBL_END(                                               \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_SUCCESS_WARNING )

#define MX_R_PKG_SUCCESS_WARNING_MSG_TBL_INITIALIZE( ePkgId )                   \
            MX_R_PKG_MSG_TBL_INITIALIZE(                                        \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_SUCCESS_WARNING )

#define MX_R_PKG_SUCCESS_WARNING_MSG_TBL_FINALIZE( ePkgId )                     \
            MX_R_PKG_MSG_TBL_FINALIZE(                                          \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_SUCCESS_WARNING )

#define MX_R_PKG_FAIL_ERROR_MSG_TBL_BEGIN( ePkgId )                             \
            MX_R_PKG_MSG_TBL_BEGIN(                                             \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_FAIL_ERROR )

#define MX_R_PKG_FAIL_ERROR_MSG_TBL_END( ePkgId )                               \
            MX_R_PKG_MSG_TBL_END(                                               \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_FAIL_ERROR )

#define MX_R_PKG_FAIL_ERROR_MSG_TBL_INITIALIZE( ePkgId )                        \
            MX_R_PKG_MSG_TBL_INITIALIZE(                                        \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_FAIL_ERROR )

#define MX_R_PKG_FAIL_ERROR_MSG_TBL_FINALIZE( ePkgId )                          \
            MX_R_PKG_MSG_TBL_FINALIZE(                                          \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_FAIL_ERROR )

#define MX_R_PKG_FAIL_CRITICAL_MSG_TBL_BEGIN( ePkgId )                          \
            MX_R_PKG_MSG_TBL_BEGIN(                                             \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_FAIL_CRITICAL )

#define MX_R_PKG_FAIL_CRITICAL_MSG_TBL_END( ePkgId )                            \
            MX_R_PKG_MSG_TBL_END(                                               \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_FAIL_CRITICAL )


#define MX_R_PKG_FAIL_CRITICAL_MSG_TBL_INITIALIZE( ePkgId )                     \
            MX_R_PKG_MSG_TBL_INITIALIZE(                                        \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_FAIL_CRITICAL )

#define MX_R_PKG_FAIL_CRITICAL_MSG_TBL_FINALIZE( ePkgId )                       \
            MX_R_PKG_MSG_TBL_FINALIZE(                                          \
                        ePkgId,                                                 \
                        eMX_RESULT_CODE_ID_TYPE_FAIL_CRITICAL )

#define MX_R_PKG_MSG_TBL_BEGIN( ePkgId, eCodeIdType )                           \
const char* const apszMsg##ePkgId##eCodeIdType[] =                              \
{

#define MX_R_PKG_MSG_TBL_END( ePkgId, eCodeIdType )                             \
};                                                                              \
SResultPkgMsgAlignedStorage g_resultMsgTbl##ePkgId##eCodeIdType;                \
SResultPkgMsg* g_pstResultMsgTbl##ePkgId##eCodeIdType = NULL;

#define MX_R_PKG_MSG_TBL_INITIALIZE( ePkgId, eCodeIdType )                      \
g_pstResultMsgTbl##ePkgId##eCodeIdType =                                        \
    new (g_resultMsgTbl##ePkgId##eCodeIdType.Instance())                        \
         SResultPkgMsg(ePkgId,                                                  \
                       MX_SIZEOFARRAY(apszMsg##ePkgId##eCodeIdType),            \
                       apszMsg##ePkgId##eCodeIdType,                            \
                       eCodeIdType )

#define MX_R_PKG_MSG_TBL_FINALIZE( ePkgId, eCodeIdType )                        \
g_resultMsgTbl##ePkgId##eCodeIdType.Destruct();                                 \
g_pstResultMsgTbl##ePkgId##eCodeIdType = NULL


#endif // #if defined (MXD_RESULT_ENABLE_ERROR_MESSAGES)

#define MX_RIS_S(res)               ((int32_t)res >= 0)
#define MX_RIS_F(res)               ((int32_t)res <  0)
#define MX_RIS_SI(res)                                                          \
          ( (res & MX_RESULT_LEVEL_MASK ) == MX_RESULT_LEVEL_INFO_VALUE )
#define MX_RIS_SW(res)                                                          \
        ( (res & MX_RESULT_LEVEL_MASK ) == MX_RESULT_LEVEL_WARNING_VALUE )
#define MX_RIS_FE(res)                                                          \
        ( (res & MX_RESULT_LEVEL_MASK ) == MX_RESULT_LEVEL_ERROR_VALUE )
#define MX_RIS_FC(res)                                                          \
        ( (res & MX_RESULT_LEVEL_MASK ) == MX_RESULT_LEVEL_CRITICAL_VALUE )
#define MX_RGET_CID(res) (uint32_t)( (res >> MX_RESULT_CODE_ID_BIT_POSITION ) & \
        ((uint32_t)MX_RESULT_CODE_ID_MASK >> MX_RESULT_CODE_ID_BIT_POSITION ) )
#define MX_RGET_LEV(res) (uint32_t)( (res >> MX_RESULT_LEVEL_BIT_POSITION ) &   \
          ((uint32_t)MX_RESULT_LEVEL_MASK >> MX_RESULT_LEVEL_BIT_POSITION ) )
#define MX_RGET_PID(res) (uint32_t)( (res >> MX_RESULT_PKG_ID_BIT_POSITION ) &  \
            ((uint32_t)MX_RESULT_PKG_MASK >> MX_RESULT_PKG_ID_BIT_POSITION ) )

#if defined(__cplusplus)
extern "C"
{
#endif
mxt_result MxRGetWorstOf(IN mxt_result resOne, IN mxt_result resTwo);
#if defined(__cplusplus)
}
#endif

#define MX_RGET_WORST_OF(resOne, resTwo) MxRGetWorstOf(resOne, resTwo)

#define MX_RGET_MSG_STR(res) MxResultGetMsgStr(res)

//==============================================================================
//====  FUNCTION PROTOTYPE ( internal used only )===============================
//==============================================================================

// Gets the message string associated with the result (pkgID and codeID).
//------------------------------------------------------------------------------
#if defined(__cplusplus)
extern "C"
{
#endif
const char* MxResultGetMsgStr( IN mxt_result res );
#if defined(__cplusplus)
}
#endif

//M5T_INTERNAL_USE_END

// Doc-O-Matic documentation section
#if 0

//==SDOC========================================================================
//<GROUP BASIC_TYPES>
//
// Summary:
//  Data type used to store result information.
//
// Description:
//
//  The mxt_result data type, which is defined on a signed integer, is used as a
//  standard mechanism for methods to return a result or error. There are
//  three distinct usable fields in a mxt_result: The level, the package ID, and
//  the result code ID. A field has been reserved for future use.  See below for
//  the definition of those fields.
//
//  The result mechanism is defined to add very little overhead while returning
//  a lot of information. There is also a list of generic error messages that
//  are not associated directly with a package, therefore avoiding redundant
//  messages.
//
//  Information messages can also be associated with every error message in case
//  there is the need to output readable information. See MX_RGET_MSG_STR for
//  more details.
//
//  The result mechanism is also designed so it can be used in a user
//  application. To do so, the package ID enumeration must be augmented. New
//  error codes can also be created to fit the application needs.  See
//  MXD_PKG_ID_OVERRIDE and MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN.
//
// #mxt_result definition:#
//
//  The mxt_result is defined as one signed 32 bits integer.
//
//  <CODE>
//     MSB                                                         LSB
//     3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//     1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//    +---+-----------+-------------------+--------------------------+
//    |LEV| Reserved  |    Package ID     |           Code ID        |
//    +---+-----------+-------------------+--------------------------+
//
//    Bits
//    31-30:  Level
//            Bits 31 30 definition:
//                  0  0 - SUCCESS-INFORMATION
//                  0  1 - SUCCESS-WARNING
//                  1  0 - FAIL-ERROR
//                  1  1 - FAIL-CRITICAL
//
//            SUCCESS-INFORMATION: Operation Succeeded and Information may be
//                                 returned.
//                                 i.e.: resS_OK (CodeID = 0, no info)
//                                       resSI_TRUE (CodeID > 0, info = TRUE)
//                                       resSI_FALSE (CodeID > 0, info = FALSE)
//
//            SUCCESS-WARNING:     Operation Succeeded and a Warning is returned.
//                                 i.e.: resSW_MEMORY_LOW
//
//            FAIL-ERROR:          Operation Failed and the Error cause is
//                                 returned.
//                                 i.e.: resFE_INVALID_ARGUMENT
//
//            FAIL_CRITICAL:       Operation Failed and a Critical error cause is
//                                 returned.
//                                 i.e.: resFC_DATA_CORRUPTION
//
//    29-24:  Reserved
//            Reserved for future use (Shall always be 0)
//
//    23-14:  Package ID (10 bits, up to 1024 packages)
//            Package ID is unique for each package.
//
//    13-0:   Code ID (14 bits, up to 16384 codes for each level)
//            Codes between 0 and MX_RESULT_NB_MAX_SHARED_CODE_ID - 1 ( 0-1023 )
//            are reserved for shared code IDs.
//            This range of codes are not redefined in each package. Instead, they
//            are defined only once and can be used in any packages. Code ID codes
//            starting from MX_RESULT_NB_MAX_SHARED_CODE_ID can be defined for
//            each package and each level.
//  </CODE>
//
// #Result code IDs:#
//
// Result code IDs shall be defined in an enum as described below:
//
//  resLEVEL_PKGNAME_MSG
//
//  res: (lower case)
//      mxt_result standard prefix.
//
//  LEVEL: (upper case)
//   SI_ for Success Information (note that S_OK is an exception)
//   SW_ for Success Warning
//   FE_ for Fail Error
//   FC_ for Fail Critical
//
//  PKGNAME: (upper case)
//   Package name, with regards to EMxPackageId nomenclature (omitted for
//   shared code ID).
//
//  MSG: (upper case)
//  Result message. Each word shall be separated with an underscore.
//
//  Code ID = 0 is reserved for resS_OK. This allows a faster detection of a
//  SUCCESS without information.  A result of that constraint is that other
//  shared code IDs must start with code ID = 1 instead of code ID = 0.
//  Note that shared messages table must also start with an empty msg (matches
//  code ID 0 which is not used) to make code ID 1 match with msg 1 and so on.
//
//  Note:   Since declaration result codes for each package are based on an
//          enum, the enum type must be int32_t.
//
// Location:
//    Defined in Basic/Result.h but must include Config/MxConfig.h to
//    access this.
//
// See Also:
//  MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID,
//  MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN,
//  MX_RIS_S
//
//==EDOC========================================================================
typedef int32_t mxt_result;

//<GROUP BASIC_MACROS>
//<TITLE Macros to get the base result code id>
//==SDOC========================================================================
//
// Summary:
//  Macro used to get the lower value of the result code ID enumeration for a
//  unique package ID.
//
// Parameters:
//  ePkgId: Package ID for which to get the lower value of the result code.
//
// Description:
//  The MX_RGET_PKG_BASE_ macros are used to get the lower value of the result
//  code ID enumeration for a unique package ID.  The package ID EMxPackageId
//  must be passed to it so base codes can be unique throughout the system.
//
// Location:
//    Defined in Basic/Result.h but must include Config/MxConfig.h to access
//    this.
//
// See Also:
//  mxt_result, MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN
//
//==EDOC========================================================================
#define MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID(ePkgId)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID>
#define MX_RGET_PKG_BASE_FAIL_ERROR_CODE_ID(ePkgId)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID>
#define MX_RGET_PKG_BASE_FAIL_CRITICAL_CODE_ID(ePkgId)

//<GROUP BASIC_MACROS>
//<TITLE Result code messages table definition macros>
//==SDOC========================================================================
//
// Summary:
//  Macros used to automatically generate messages table initialization for a
//  package.
//
// Parameters:
//  ePkgId: Package ID for which to initialize the message table.
//
// Description:
//  These macros automatically generate messages table initialization for a
//  package. The _MSG_TBL_BEGIN and _MSG_TBL_END macros must be used in pairs.
//  The semicolon must not be used in the messages definition or at the end of
//  the macro.
//  Note that these macros are not defined when the application is compiled
//  without support for error messages (i.e. when MXD_RESULT_ENABLE_ERROR_MESSAGE
//  is not defined).
//
//  <CODE>
//    Example (init 3 info-success result code msgs to PKG_PACKAGE:
//      MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN(eMX_PKG_PACKAGE)
//      "This SUCCESS INFO result code msg matches with the first result code ID",
//      "This SUCCESS INFO result code msg matches with the second result code ID",
//      "This SUCCESS INFO result code msg matches with the third result code ID"
//      MX_R_PKG_SUCCESS_INFO_MSG_TBL_END(eMX_PKG_PACKAGE)
//  </CODE>
//
// Location:
//    Defined in Basic/Result.h but must include Config/MxConfig.h to
//    access this.
//
// See Also:
//  mxt_result, MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID
//
//==EDOC========================================================================
#define MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN(ePkgId)

//<GROUP BASIC_MACROS>
//<COMBINE MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN>
#define MX_R_PKG_SUCCESS_INFO_MSG_TBL_END(ePkgId)

//<GROUP BASIC_MACROS>
//<COMBINE MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN>
#define MX_R_PKG_SUCCESS_WARNING_MSG_TBL_BEGIN(ePkgId)

//<GROUP BASIC_MACROS>
//<COMBINE MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN>
#define MX_R_PKG_SUCCESS_WARNING_MSG_TBL_END(ePkgId)

//<GROUP BASIC_MACROS>
//<COMBINE MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN>
#define MX_R_PKG_FAIL_ERROR_MSG_TBL_BEGIN(ePkgId)

//<GROUP BASIC_MACROS>
//<COMBINE MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN>
#define MX_R_PKG_FAIL_ERROR_MSG_TBL_END(ePkgId)

//<GROUP BASIC_MACROS>
//<COMBINE MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN>
#define MX_R_PKG_FAIL_CRITICAL_MSG_TBL_BEGIN(ePkgId)

//<GROUP BASIC_MACROS>
//<COMBINE MX_R_PKG_SUCCESS_INFO_MSG_TBL_BEGIN>
#define MX_R_PKG_FAIL_CRITICAL_MSG_TBL_END(ePkgId)

//<GROUP BASIC_MACROS>
//<TITLE Result manipulation macros>
//==SDOC========================================================================
//
// Summary:
//  Result manipulation macro.
//
// Parameters:
//  res: Result on which to perform the manipulation.
//
// Description:
//  The following result manipulation macros are available within the framework.
//  Note that if an application wants to use any of these macros, it must
//  include "Config/MxConfig.h" first.
//
//  #MX_RIS_S#
//    Validation macro.  Returns true if the result given in parameter is a
//    success code.
//
//    <CODE>
//      bool bValidation  = MX_RIS_S(res);
//    </CODE>
//
//  #MX_RIS_F#
//    Validation macro.  Returns true if the result given in parameter is a
//    fail code.
//
//    <CODE>
//      bool bValidation  = MX_RIS_F(res);
//    </CODE>
//
//  #MX_RIS_SI#
//    Validation macro.  Returns true if the result given in parameter is a
//    success-info code.
//
//    <CODE>
//      bool bValidation  = MX_RIS_SI(res);
//    </CODE>
//
//  #MX_RIS_SW#
//    Validation macro.  Returns true if the result given in parameter is a
//    success-warning code.
//
//    <CODE>
//      bool bValidation  = MX_RIS_SW(res);
//    </CODE>
//
//  #MX_RIS_FE#
//    Validation macro.  Returns true if the result given in parameter is a
//    fail-error code.
//
//    <CODE>
//      bool bValidation  = MX_RIS_FE(res);
//    </CODE>
//
//  #MX_RIS_FC#
//    Validation macro.  Returns true if the result given in parameter is a
//    fail-critical code.
//
//    <CODE>
//      bool bValidation  = MX_RIS_FC(res);
//    </CODE>
//
//  #MX_RGET_LEV#
//    Macro to get the level of a result.
//
//  #MX_RGET_PID#
//    Macro to get the package ID of a result.
//
//  #MX_RGET_CID#
//    Macro to get the code ID of a result.
//
//  #MX_RGET_MSG_STR#
//    Macro to get the result message string.
//
//  #MX_RGET_WORST_OF#
//    Macro to get the worst between two results.  The comparison is done using
//    the level.  If both results have the same level, the first one is always
//    returned.
//
// Note:
//  The validation macros must never be called to validate the reverse
//  condition.  Always use MX_RIS_F instead of !MX_RIS_S.  This is to make the
//  code more readable.
//
// Location:
//    Defined in Basic/Result.h but must include Config/MxConfig.h to
//    access this.
//
// See Also:
//  mxt_result
//
//==EDOC========================================================================
#define MX_RIS_S(res)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RIS_S>
#define MX_RIS_SI(res)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RIS_S>
#define MX_RIS_SW(res)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RIS_S>
#define MX_RIS_F(res)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RIS_S>
#define MX_RIS_FE(res)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RIS_S>
#define MX_RIS_FC(res)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RIS_S>
#define MX_RGET_LEV(res)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RIS_S>
#define MX_RGET_PID(res)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RIS_S>
#define MX_RGET_CID(res)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RIS_S>
#define MX_RGET_MSG_STR(res)

//<GROUP BASIC_MACROS>
//<COMBINE MX_RIS_S>
#define MX_RGET_WORST_OF(resOne, resTwo)

#endif // #if 0 --> Documentation section.

#endif // #ifndef MXG_RESULT_H
