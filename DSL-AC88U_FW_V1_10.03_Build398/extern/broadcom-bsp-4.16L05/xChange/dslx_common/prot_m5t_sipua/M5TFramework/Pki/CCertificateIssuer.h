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
//   Media5 reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CCERTIFICATEISSUER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATEISSUER_H
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
#if defined(MXD_PKI_CCERTIFICATEISSUER_INCLUDE)
    #include MXD_PKI_CCERTIFICATEISSUER_INCLUDE
#else
    #error "No implementation defined for CCertificateIssuer!!!"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif // #ifndef MXG_CPKI_H

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CCertificateIssuer
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class managing the certificate issuer related identifiers and values.
//
// Description:
//   CCertificateIssuer is the class that manages the certificate issuer related
//   identifiers and values.
//
// Location:
//   Pki/CCertificateIssuer.h
//
// See Also:
//
//==EDOC========================================================================
class CCertificateIssuer : public MXD_PKI_CCERTIFICATEISSUER_CLASSNAME
{

// Published Enum
//----------------
public:
    // Summary:
    //  Defines the possible names in the issuer.

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //-------------------
    CCertificateIssuer();

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateIssuer(IN const CCertificateIssuer* pCertificateIssuer);

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateIssuer(IN const CCertificateIssuer& rCertificateIssuer);

    // Summary:
    // Destructor.
    //-------------
    virtual ~CCertificateIssuer();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateIssuer& operator=(IN const CCertificateIssuer& rCertificateIssuer);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CCertificateIssuer& rCertificateIssuer) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CCertificateIssuer& rCertificateIssuer) const;

    // << Accessors >>
    //-----------------

    // Summary:
    // Gets the specified name.
    //--------------------------
    mxt_result GetName(IN EName eName, OUT CString* pstrName, OUT bool* pbExist) const;

    // Summary:
    //  Gets the vector of all names.
    mxt_result GetNames(IN EName eName, OUT CVector<CString>* pvecstrNames) const;

// Hidden Methods
//----------------
protected:
private:

// Hidden Data Members
//---------------------
protected:

};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==    CCertificateIssuer
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CCertificateIssuer::CCertificateIssuer()
:   MXD_PKI_CCERTIFICATEISSUER_CLASSNAME()
{
}

//==SDOC========================================================================
//==
//==    CCertificateIssuer
//==
//==============================================================================
//
//  Parameters:
//      pCertificateIssuer:
//          Pointer to the CCertificateIssuer to compare.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CCertificateIssuer::CCertificateIssuer(IN const CCertificateIssuer* pCertificateIssuer)
:   MXD_PKI_CCERTIFICATEISSUER_CLASSNAME(pCertificateIssuer)
{
}

//==SDOC========================================================================
//==
//==    CCertificateIssuer
//==
//==============================================================================
//
//  Parameters:
//      rCertificateIssuer:
//          Reference to the CCertificateIssuer to compare.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CCertificateIssuer::CCertificateIssuer(IN const CCertificateIssuer& rCertificateIssuer)
:   MXD_PKI_CCERTIFICATEISSUER_CLASSNAME(rCertificateIssuer)
{
}

//==SDOC========================================================================
//==
//==    ~CCertificateIssuer
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CCertificateIssuer::~CCertificateIssuer()
{
}

//==SDOC========================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rCertificateIssuer:
//          Reference to the CCertificateIssuer to assign.
//
//  Returns:
//      A reference to the assigned issuer.
//
//  Description:
//      Assigns the right hand issuer to the left hand one.
//
//==EDOC========================================================================
inline
CCertificateIssuer& CCertificateIssuer::operator=(IN const CCertificateIssuer& rCertificateIssuer)
{
    MXD_PKI_CCERTIFICATEISSUER_CLASSNAME::operator=(rCertificateIssuer);
    return *this;
}

//==SDOC========================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rCertificateIssuer:
//          Reference to the CCertificateIssuer to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==EDOC========================================================================
inline
bool CCertificateIssuer::operator==(IN const CCertificateIssuer& rCertificateIssuer) const
{
    return MXD_PKI_CCERTIFICATEISSUER_CLASSNAME::operator==(rCertificateIssuer);
}

//==SDOC========================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rCertificateIssuer:
//          Reference to the CCertificateIssuer to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==EDOC========================================================================
inline
bool CCertificateIssuer::operator!=(IN const CCertificateIssuer& rCertificateIssuer) const
{
    return !(MXD_PKI_CCERTIFICATEISSUER_CLASSNAME::operator==(rCertificateIssuer));
}

//==SDOC========================================================================
//==
//==    GetName
//==
//==============================================================================
//
//  Parameters:
//      eName:
//          Name to get in the issuer.
//
//      pstrName:
//          Pointer to contain the string of the name.
//
//      pbExist:
//          Pointer to contain true if the requested name exists, false
//          otherwise.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      Gets the specified name from the issuer.
//
//==EDOC========================================================================
inline
mxt_result CCertificateIssuer::GetName(IN EName eName,
                                       OUT CString* pstrName,
                                       OUT bool* pbExist) const
{
    return MXD_PKI_CCERTIFICATEISSUER_CLASSNAME::GetName(eName, OUT pstrName, OUT pbExist);
}

//==============================================================================
//==
//==    GetNames
//==
//==============================================================================
//
//  Parameters:
//      eName:
//          Name to get in the issuer.
//
//      pvecstrNames:
//          Pointer to a vector containing the names. MUST not be NULL.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      Gets all the names corresponding to the EName from the issuer.
//
//==============================================================================
inline
mxt_result CCertificateIssuer::GetNames(IN EName eName, OUT CVector<CString>* pvecstrNames) const
{
    return MXD_PKI_CCERTIFICATEISSUER_CLASSNAME::GetNames(eName, pvecstrNames);
}
MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CCERTIFICATEISSUER_H

