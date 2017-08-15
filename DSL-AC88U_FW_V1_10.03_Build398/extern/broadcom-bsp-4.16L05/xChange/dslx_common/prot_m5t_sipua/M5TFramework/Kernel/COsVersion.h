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
#ifndef MXG_COSVERSION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_COSVERSION_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#if !defined(MXD_OS_NONE)

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//== Class: COsVersion
//==============================================================================
//<GROUP KERNEL_CLASSES>
//
// Summary:
//   This class gives access to operating system version.
//
// Description:
//   This class gets the operating system version at run time.
//
// Location:
//   Kernel/COsVersion.h
//
//==============================================================================
class COsVersion
{

//-- New types within class namespace
//-----------------------------------
public:

    //<GROUP KERNEL_ENUMS>
    //
    // Summary:
    //  Defines supported OS versions.
    //
    // Description:
    //  Defines supported OS versions.
    enum EOsVersion
    {
        // Description:
        // No OS version was found
        eNO_VERSION_FOUND,
        // Description:
        // OS version is Windows NT.
        eWINDOWS_NT,
        // Description:
        // OS version is Windows XP.
        eWINDOWS_XP,
        // Description:
        // OS version is Windows 2000.
        eWINDOWS_2K,
        // Description:
        // OS version is Windows 2003.
        eWINDOWS_2003_PLUS,
        // Description:
        // OS version is Windows Vista or more recent.
        eWINDOWS_VISTA,
        // Description:
        // OS version is Linux 2.4 kernel.
        eLINUX_24,
        // Description:
        // OS version is Linux 2.6 kernel.
        eLINUX_26,
        // Description:
        // OS version is Solaris 8.
        eSOLARIS_8,
        // Description:
        // OS version is Solaris 10.
        eSOLARIS_10,
        // Description:
        // OS version is unsupported.
        eUNSUPPORTED_OS
    };
protected:
private:

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //==========================================================================
    //==
    //==   COsVersion
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Default constructor.
    //
    //==========================================================================
    COsVersion();

    //==========================================================================
    //==
    //==   ~COsVersion
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==========================================================================
    virtual ~COsVersion();

    // Summary:
    //   Retrieves the version of the operating system.
    //------------------------------------------------------------------
    static EOsVersion GetOsVersion();

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    COsVersion(IN const COsVersion& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    COsVersion& operator=(IN const COsVersion& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_OS_NONE)

#endif //-- #ifndef MXG_COSVERSION_H


