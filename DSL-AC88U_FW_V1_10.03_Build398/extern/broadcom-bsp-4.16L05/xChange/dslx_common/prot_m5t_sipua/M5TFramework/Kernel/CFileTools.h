//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CFILETOOLS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CFILETOOLS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Framework Configuration
//---------------------------
#ifndef MXG_FRAMEWORKCFG_H
    #include "Config/FrameworkCfg.h" // MXD_FILESYSTEM_PROPRIETARY_IMPLEMENTATION
#endif

#if !defined(MXD_FILESYSTEM_PROPRIETARY_IMPLEMENTATION)

#ifndef MXG_CFILE_H
#include "Kernel/CFile.h"
#endif

// CFileTools is supported only on those operating systems.
#if defined(MXD_OS_WINDOWS)    || \
    defined(MXD_OS_WINDOWS_CE) || \
    defined(MXD_OS_LINUX)      || \
    defined(MXD_OS_SOLARIS)    || \
    defined(MXD_OS_DARWIN)     || \
    defined(MXD_OS_VXWORKS)    || \
    defined(MXD_OS_SYMBIAN)    || \
    defined(MXD_OS_NUCLEUS)    || \
    defined(MXD_OS_ANDROID)

//-- Data Members
//-----------------

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CFileTools
//========================================
//<GROUP KERNEL_CLASSES>
//
// Summary:
//  Tools for file management.
//
// Description:
//  CFileTools is the class that provides a common interface for file management
//  on all operating systems. The file must not be opened when using these
//  methods.
//
// Location:
//   Kernel/CFileTools.h
//
//==EDOC========================================================================
class CFileTools
{
//-- Friend Declarations
//------------------------

//-- Published Interface
//------------------------
public:

    // Summary:
    //  Removes a file or a directory.
    static mxt_result Remove(IN const char* pszPathName);

    // Summary:
    //  Renames a file. It will also move it to another directory
    //  if the path has been changed.
    static mxt_result Rename(IN const char* pszOldPathName, IN const char* pszNewPathName);

    // Summary:
    //  Changes the mode of a file.
    static mxt_result ChangeMode(IN const char* pszPathName, IN CFile::EMode eMode);

    // Summary:
    //  Creates a directory.
    static mxt_result CreateNewDir(IN const char* pszPathName);

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CFileTools();

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CFileTools();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CFileTools(IN const CFileTools& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CFileTools& operator=(IN const CFileTools& rSrc);

#if defined(MXD_OS_WINDOWS_CE)
    // Summary:
    // Converts a string to a unicode string.
    static mxt_result ConvertToUnicode(IN const char* pszString, OUT GO wchar_t*& rpawcString);
#endif

//M5T_INTERNAL_USE_BEGIN
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

//-- Hidden Data Members
//------------------------
protected:
private:

//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_OS_WINDOWS)    ||
       //     defined(MXD_OS_WINDOWS_CE) ||
       //     defined(MXD_OS_LINUX)      ||
       //     defined(MXD_OS_SOLARIS)    ||
       //     defined(MXD_OS_DARWIN)     ||
       //     defined(MXD_OS_VXWORKS)    ||
       //     defined(MXD_OS_SYMBIAN)    ||
       //     defined(MXD_OS_NUCLEUS)    ||
       //     defined(MXD_OS_ANDROID)

#endif // #if !defined(MXD_FILESYSTEM_PROPRIETARY_IMPLEMENTATION)

#endif //-- #ifndef MXG_CFILETOOLS_H
