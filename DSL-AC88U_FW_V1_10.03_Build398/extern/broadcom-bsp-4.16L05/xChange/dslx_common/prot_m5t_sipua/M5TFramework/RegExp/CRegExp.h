//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CREGEXP_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CREGEXP_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
    #include "Config/FrameworkCfg.h" // for MXD_REGEXP_ENABLE_SUPPORT
#endif

#if defined(MXD_REGEXP_ENABLE_SUPPORT)

#include "regex.h"

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CString;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CRegExp
//========================================
//<GROUP REG_EXP_CLASSES>
//
// Summary:
//   This class handles POSIX Extended Regular Expressions.
//
// Description:
//   This class abstracts calls to a regular expression library that supports
//   POSIX Extended Regular Expressions.
//
// Location:
//   RegExp/CRegExp.h
//
//==EDOC========================================================================
class CRegExp
{
//-- New types within class namespace
public:
    //<GROUP REG_EXP_STRUCTS>
    // Summary:
    //  Structure representing a sub-expression of a regular expression.
    struct SSubExpression
    {
        // Description:
        // The first character in a sub-expression.
        char* pcExpression;
        // Description:
        // The sub-expression's size.
        unsigned int uExpressionSize;
    };

//-- Published Interface
public:
    //-- << Stereotype >>

    // Summary:
    //  Performs a simple validation that a string matches a regular expression.
    static mxt_result Find(IN const char* pszRegExp,
                           IN bool bCaseSensitivity,
                           IN const char* pszString);

    // Summary:
    //  Finds regular expression patterns and returns sub-expression matches.
    static mxt_result FindSubExpressions(IN const char* pszRegExp,
                                         IN bool bCaseSensitivity,
                                         IN const char* pszString,
                                         IN unsigned int uSubExpCapacity,
                                         OUT unsigned int* puSubExpSize,
                                         OUT SSubExpression* pastSubExpressions);

    // Summary:
    //  Finds regular expression patterns and formats them following a
    //  specified format.
    static mxt_result FindAndReplace(IN const char* pszRegExp,
                                     IN bool bCaseSensitivity,
                                     IN const char* pszString,
                                     IN const char* pszFormat,
                                     OUT CString* pstrResult);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CRegExp() {};

    // Summary:
    //  Destructor.
    ~CRegExp() {};

    //-- << Helper Methods >>

    // Summary:
    //  Compiles the regular expression and fills m_stRegExp.
    static mxt_result CompileRegularExpression(IN const char* pszRegExp,
                                               IN bool bCaseSensitivity,
                                               INOUT regex_t* pstRegExp);

    // Summary:
    //  Executes the regular expression on the supplied string.
    static mxt_result ExecuteRegularExpression(IN const char* pszString,
                                               IN const regex_t* pstRegExp,
                                               OUT unsigned int* puMatchesSize = NULL,
                                               OUT GO regmatch_t** pstMatches = NULL);

    // Summary:
    //  Replaces back-references with sub-expressions.
    static mxt_result ReplaceBackReferences(IN const char* pszFormat,
                                            IN unsigned int uSubExpSize,
                                            IN SSubExpression* pastSubExpressions,
                                            OUT CString* pstrResult);

    // Summary:
    //  Fills a SSubExpression structure with the sub-expressions returned from
    //  the regular expression library.
    static mxt_result FillSubExpressions(IN const regex_t* pstRegExp,
                                         IN const char* pszString,
                                         IN unsigned int uSubExpCapacity,
                                         OUT unsigned int* puSubExpSize,
                                         OUT SSubExpression* pastSubExpressions);

#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // warning or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_REGEXP_ENABLE_SUPPORT)

#endif // #ifndef MXG_CREGEXP_H


