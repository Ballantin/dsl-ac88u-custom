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
#ifndef MXG_CCERTIFICATESUBJECT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATESUBJECT_H
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
#if defined(MXD_PKI_CCERTIFICATESUBJECT_INCLUDE)
    #include MXD_PKI_CCERTIFICATESUBJECT_INCLUDE
#else
    #error "No implementation defined for CCertificateSubject!!!"
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

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CCERTIFICATESUBJECT)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CCERTIFICATESUBJECTSTUB_H
    #include "PkiStubs/CCertificateSubjectStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC========================================================================
//== Class: CCertificateSubject
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class that manages the certificate subject related identifiers and values.
//
// Description:
//   CCertificateSubject is the class that manages the certificate subject
//   related identifiers and values.
//
// Location:
//   Pki/CCertificateSubject.h
//
// See Also:
//
//==EDOC========================================================================
class CCertificateSubject : public MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME
{
//M5T_INTERNAL_USE_BEGIN
    friend class CCertificate;
//M5T_INTERNAL_USE_END

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
    //-------------------
    CCertificateSubject();

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateSubject(IN const CCertificateSubject* pCertificateSubject);

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateSubject(IN const CCertificateSubject& rCertificateSubject);

    // Summary:
    // Destructor.
    //-------------
    virtual ~CCertificateSubject();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateSubject& operator=(IN const CCertificateSubject& rCertificateSubject);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CCertificateSubject& rCertificateSubject) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CCertificateSubject& rCertificateSubject) const;

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
//==    CCertificateSubject
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CCertificateSubject::CCertificateSubject()
:   MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME()
{
}

//==SDOC========================================================================
//==
//==    CCertificateSubject
//==
//==============================================================================
//
//  Parameters:
//      pCertificateSubject:
//          Pointer to the CCertificateSubject to compare.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CCertificateSubject::CCertificateSubject(IN const CCertificateSubject* pCertificateSubject)
:   MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME(pCertificateSubject)
{
}

//==SDOC========================================================================
//==
//==    CCertificateSubject
//==
//==============================================================================
//
//  Parameters:
//      rCertificateSubject:
//          Reference to the CCertificateSubject to compare.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CCertificateSubject::CCertificateSubject(IN const CCertificateSubject& rCertificateSubject)
:   MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME(rCertificateSubject)
{
}

//==SDOC========================================================================
//==
//==    ~CCertificateSubject
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CCertificateSubject::~CCertificateSubject()
{
}

//==SDOC========================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rCertificateSubject:
//          Reference to the CCertificateSubject to assign.
//
//  Returns:
//      A reference to the assigned subject.
//
//  Description:
//      Assigns the right hand subject to the left hand one.
//
//==EDOC========================================================================
inline
CCertificateSubject& CCertificateSubject::operator=(IN const CCertificateSubject& rCertificateSubject)
{
    MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME::operator=(rCertificateSubject);
    return *this;
}

//==SDOC========================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rCertificateSubject:
//          Reference to the CCertificateSubject to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==EDOC========================================================================
inline
bool CCertificateSubject::operator==(IN const CCertificateSubject& rCertificateSubject) const
{
    return MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME::operator==(rCertificateSubject);
}

//==SDOC========================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rCertificateSubject:
//          Reference to the CCertificateSubject to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==EDOC========================================================================
inline
bool CCertificateSubject::operator!=(IN const CCertificateSubject& rCertificateSubject) const
{
    return !(MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME::operator==(rCertificateSubject));
}

//==SDOC========================================================================
//==
//==    GetName
//==
//==============================================================================
//
//  Parameters:
//      eName:
//          Name to get in the subject
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
//      Gets the specified name from the subject.
//
//==EDOC========================================================================
inline
mxt_result CCertificateSubject::GetName(IN EName eName,
                                        OUT CString* pstrName,
                                        OUT bool* pbExist) const
{
    return MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME::GetName(eName, pstrName, pbExist);
}

//==============================================================================
//==
//==    GetNames
//==
//==============================================================================
//
//  Parameters:
//      eName:
//          Name to get in the subject
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
//      Gets all the names corresponding to the EName from the subject.
//
//==============================================================================
inline
mxt_result CCertificateSubject::GetNames(IN EName eName, OUT CVector<CString>* pvecstrNames) const
{
    return MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME::GetNames(eName, pvecstrNames);
}
#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CCERTIFICATESUBJECT)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CCERTIFICATESUBJECT_H

