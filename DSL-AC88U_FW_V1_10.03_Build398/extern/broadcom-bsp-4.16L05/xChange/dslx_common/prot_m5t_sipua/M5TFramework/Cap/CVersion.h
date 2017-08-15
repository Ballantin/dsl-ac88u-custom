//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CVERSION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CVERSION_H
//M5T_INTERNAL_USE_END

//==============================================================================
//==== INCLUDE FILES ===========================================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)

MX_NAMESPACE_START (MXD_GNS)
//==SDOC========================================================================
//== Class: CVersion
//========================================
//<GROUP CAP_CLASSES>
//
// Summary:
//  Class used for version checking.
//
// Description:
//  This is the class used for version checking. The version of the application
//  can be set and modified for comparison with other CVersion objects.
//
// Location:
//   Cap/CVersion.h
//
//==EDOC========================================================================
class CVersion  // DO NOT heritate from anything: sizeof (CVersion) MUST be == sizeof (uint32_t)
{
//==============================================================================
//==== PUBLIC METHODS ==========================================================
//==============================================================================
    public:
        // Constructors / Destructors

        //  Summary:
        //   Basic constructor.
        //----------------------
        CVersion();

        //  Summary:
        //   Copy constructor.
        //----------------------
        CVersion(IN const CVersion& rVersion);

        //  Summary:
        //   Constructor.
        //----------------
        CVersion(IN const char* pszVersion);

        //  Summary:
        //   Destructor.
        //---------------
        ~CVersion();  // DO NOT set destructor virtual: sizeof (CVersion) MUST be == sizeof (uint32_t)

        // Accessors
        // Summary:
        //  Gets the major version number.
        uint8_t GetMajor()   const;

        // Summary:
        //  Gets the minor version number.
        uint8_t GetMinor()   const;

        // Summary:
        //  Gets the release version number.
        uint8_t GetRelease() const;

        // Summary:
        //  Gets the build version number.
        uint8_t GetBuild()   const;

        // Summary:
        //  Sets the major version number.
        void SetMajor   (IN uint8_t unMajor);

        // Summary:
        //  Sets the minor version number.
        void SetMinor   (IN uint8_t unMinor);

        // Summary:
        //  Sets the release version number.
        void SetRelease (IN uint8_t unRelease);

        // Summary:
        //  Sets the build version number.
        void SetBuild   (IN uint8_t unBuild);

        // Summary:
        //  Gets the version number in string format.
        void GetStr (OUT char* pszVersion, IN unsigned int uCapacity ) const;

        // Summary:
        //  Sets the version number in string format.
        bool SetStr (IN const char* pszVersion);

        // Operators
        // Summary:
        //  Assignment operator.
        CVersion& operator=  (IN const CVersion& rVersion);

        // Summary:
        //  Equal to operator.
        bool      operator== (IN const CVersion& rVersion) const;

        // Summary:
        //  Different than operator.
        bool      operator!= (IN const CVersion& rVersion) const;

        // Summary:
        //  Less than operator.
        bool      operator<  (IN const CVersion& rVersion) const;

        // Summary:
        //  Less than or equal to operator.
        bool      operator<= (IN const CVersion& rVersion) const;

        // Summary:
        //  Greater than operator.
        bool      operator>  (IN const CVersion& rVersion) const;

        // Summary:
        //  Greater than or equal to operator.
        bool      operator>= (IN const CVersion& rVersion) const;

        // Helpers
        // Summary:
        //  Determines if the current version number is contained within the
        //  passed version numbers.
        bool IsInRange (IN CVersion rVersion1, IN CVersion rVersion2) const;  // Boundaries included

//M5T_INTERNAL_USE_BEGIN
//==============================================================================
//==== PRIVATE DATA ============================================================
//==============================================================================
    private:
        uint8_t m_unMajor;
        uint8_t m_unMinor;
        uint8_t m_unRelease;
        uint8_t m_unBuild;
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END (MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif  // MXG_CVERSION_H

