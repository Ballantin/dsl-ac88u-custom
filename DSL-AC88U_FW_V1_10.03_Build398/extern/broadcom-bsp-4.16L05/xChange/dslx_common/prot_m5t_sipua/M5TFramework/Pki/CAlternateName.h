//==============================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_CALTERNATENAME_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CALTERNATENAME_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_PKI_NONE
#endif

#if !defined(MXD_PKI_NONE)

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif // #ifndef MXG_CPKI_H

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif // #ifndef MXG_CSTRING_H

//-- Interface Realized and/or Parent
//-------------------------------------
#if defined(MXD_PKI_CALTERNATENAME_INCLUDE)
    #include MXD_PKI_CALTERNATENAME_INCLUDE
#else
    #error "No implementation defined for CAlternateName!!!"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CALTERNATENAME)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CALTERNATENAMESTUB_H
    #include "PkiStubs/CAlternateNameStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==============================================================================
//== Class: CAlternateName
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   CAlternateName is the base class for the CIssuerAlternateName and
//   CSubjectAlternateName extensions.
//
// Description:
//   CAlternateName is the base class for the CIssuerAlternateName and
//   CSubjectAlternateName extensions. It is a container of names that may be
//   of multiple types.
//
// Location:
//   Pki/CAlternateName.h
//
// See Also:
//   CIssuerAlternateName, CSubjectAlternateName, CCertificateExtension
//
//==============================================================================
class CAlternateName : public MXD_PKI_CALTERNATENAME_CLASSNAME
{
// Published Enum
//----------------
public:

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //--------------
    CAlternateName();

    // Summary:
    // Copy constructor.
    //-------------------
    CAlternateName(IN const CAlternateName* pAlternateName);

    // Summary:
    // Copy constructor.
    //-------------------
    CAlternateName(IN const CAlternateName& rAlternateName);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CAlternateName();

    // Summary:
    // Assignment operator.
    //----------------------
    CAlternateName& operator=(IN const CAlternateName& rAlternateName);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CAlternateName& rAlternateName) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CAlternateName& rAlternateName) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the number of name entries.
    //-----------------------------------
    mxt_result GetNameCount(OUT unsigned int* puNameCount) const;

    // Summary:
    //  Gets the type of a name entry.
    //---------------------------------
    mxt_result GetNameType(IN unsigned int uName, OUT ENameType* peNameType) const;

    // Summary:
    //  Gets the value of a name of type eNAME_TYPE_DNS_NAME.
    //  An error is returned if there is a type mismatch.
    //---------------------------------------------------------
    mxt_result GetDnsName(IN unsigned int uName, OUT CString* pstrDnsName) const;

    // Summary:
    //  Gets the value of a name of type eNAME_TYPE_IP_ADDRESS.
    //  An error is returned if there is a type mismatch.
    //----------------------------------------------------------
    mxt_result GetIpAddress(IN unsigned int uName, OUT CString* pstrIpAddress) const;

    // Summary:
    //  Gets the value of the name of type eNAME_TYPE_RFC_822_NAME (aka email).
    //  An error is returned if there is a type mismatch.
    //--------------------------------------------------------------------------
    mxt_result GetRfc822Name(IN unsigned int uName, OUT CString* pstrRfc822Name) const;

    // Summary:
    //  Gets the value of a name of type eNAME_TYPE_UNIFORM_RESOURCE_IDENTIFIER.
    //  An error is returned if there is a type mismatch.
    //--------------------------------------------------------------------------
    mxt_result GetUniformResourceIdentifier(IN unsigned int uName,
                                            OUT CString* pstrUniformResourceIdentifier) const;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:
private:

// Hidden Data Members
//---------------------
protected:
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==    CAlternateName
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==============================================================================
inline CAlternateName::CAlternateName()
:   MXD_PKI_CALTERNATENAME_CLASSNAME()
{
}

//==============================================================================
//==
//==    CAlternateName
//==
//==============================================================================
//
//  Parameters:
//      pAlternateName:
//          Pointer to the CAlternateName to copy.
//
//  Description:
//      Copy constructor.
//
//==============================================================================
inline CAlternateName::CAlternateName(IN const CAlternateName* pAlternateName)
:   MXD_PKI_CALTERNATENAME_CLASSNAME(pAlternateName)
{
}

//==============================================================================
//==
//==    CAlternateName
//==
//==============================================================================
//
//  Parameters:
//      rAlternateName:
//          Reference to the CAlternateName to copy.
//
//  Description:
//      Copy constructor.
//
//==============================================================================
inline CAlternateName::CAlternateName(IN const CAlternateName& rAlternateName)
:   MXD_PKI_CALTERNATENAME_CLASSNAME(&rAlternateName)
{
}

//==============================================================================
//==
//==    ~CAlternateName
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline CAlternateName::~CAlternateName()
{
}

//==============================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rAlternateName:
//          Reference to the CAlternateName to copy.
//
//  Returns:
//      Reference to the assigned CAlternateName.
//
//  Description:
//      Assigns the right hand object to the left hand one.
//
//==============================================================================
inline CAlternateName& CAlternateName::operator=(IN const CAlternateName& rAlternateName)
{
    MXD_PKI_CALTERNATENAME_CLASSNAME::operator=(rAlternateName);
    return *this;
}

//==============================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rAlternateName:
//          Reference to the CAlternateName to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==============================================================================
inline bool CAlternateName::operator!=(IN const CAlternateName& rAlternateName) const
{
    return MXD_PKI_CALTERNATENAME_CLASSNAME::operator!=(rAlternateName);
}

//==============================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rAlternateName:
//          Reference to the CAlternateName to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==============================================================================
inline 
bool CAlternateName::operator==(IN const CAlternateName& rAlternateName) const
{
    return MXD_PKI_CALTERNATENAME_CLASSNAME::operator==(rAlternateName);
}

//==============================================================================
//==
//==    GetNameCount
//==
//==============================================================================
//
//  Parameters:
//      puNameCount:
//          Pointer to hold the name count.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the number of name items inside this object.
//
//==============================================================================
inline 
mxt_result CAlternateName::GetNameCount(OUT unsigned int* puNameCount) const
{
    return MXD_PKI_CALTERNATENAME_CLASSNAME::GetNameCount(OUT puNameCount);
}

//==============================================================================
//==
//==    GetNameType
//==
//==============================================================================
//
//  Parameters:
//      uName:
//          Index of the name.
//
//      peNameType:
//          The type of the name at uIndex.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the type of name stored at the specified index.
//
//==============================================================================
inline 
mxt_result CAlternateName::GetNameType(IN unsigned int uName, OUT ENameType* peNameType) const
{
    return MXD_PKI_CALTERNATENAME_CLASSNAME::GetNameType(uName, OUT peNameType);
}

//==============================================================================
//==
//==    GetDnsName
//==
//==============================================================================
//
//  Parameters:
//      uName:
//          Index of the name.
//
//      pstrDnsName:
//          Pointer to contain the DNS name.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the DNS name from the object.
//
//==============================================================================
inline 
mxt_result CAlternateName::GetDnsName(IN unsigned int uName,
                                      OUT CString* pstrDnsName) const
{
    return MXD_PKI_CALTERNATENAME_CLASSNAME::GetDnsName(uName, OUT pstrDnsName);
}

//==============================================================================
//==
//==    GetIpAddress
//==
//==============================================================================
//
//  Parameters:
//      uName:
//          Index of the name.
//
//      pstrIpAddress:
//          Pointer to contain the IP address.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the IP address from the object.
//
//==============================================================================
inline 
mxt_result CAlternateName::GetIpAddress(IN unsigned int uName,
                                        OUT CString* pstrIpAddress) const
{
    return MXD_PKI_CALTERNATENAME_CLASSNAME::GetIpAddress(uName, OUT pstrIpAddress);
}

//==============================================================================
//==
//==    GetRfc822Name
//==
//==============================================================================
//
//  Parameters:
//      uName:
//          Index of the name.
//
//      pstrRfc822Name:
//          Pointer to contain the RFC 822 name.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the RFC 822 name from the object.
//
//==============================================================================
inline 
mxt_result CAlternateName::GetRfc822Name(IN unsigned int uName, OUT CString* pstrRfc822Name) const
{
    return MXD_PKI_CALTERNATENAME_CLASSNAME::GetRfc822Name(uName, OUT pstrRfc822Name);
}

//==============================================================================
//==
//==    GetUniformResourceIdentifier
//==
//==============================================================================
//
//  Parameters:
//      uName:
//          Index of the name.
//
//      pstrUniformResourceIdentifier:
//          Pointer to contain the uniform resource identifier.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the uniform resource identifier from the object.
//
//==============================================================================
inline 
mxt_result CAlternateName::GetUniformResourceIdentifier(IN unsigned int uName,
                                                        OUT CString* pstrUniformResourceIdentifier) const
{
    return MXD_PKI_CALTERNATENAME_CLASSNAME::GetUniformResourceIdentifier(uName, OUT pstrUniformResourceIdentifier);
}
#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CALTERNATENAME)

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_PKI_NONE)

#endif // MXG_CALTERNATENAME_H

