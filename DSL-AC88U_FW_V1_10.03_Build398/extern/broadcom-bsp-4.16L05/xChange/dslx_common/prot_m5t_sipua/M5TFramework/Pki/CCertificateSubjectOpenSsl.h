//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CCERTIFICATESUBJECTOPENSSL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATESUBJECTOPENSSL_H

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
#include "Config/FrameworkCfg.h" // MXD_PKI_OPENSSL
#endif

#if defined(MXD_PKI_OPENSSL)

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif // #ifndef MXG_CPKI_H

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif
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
#ifndef MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME
    #define MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME CCertificateSubjectOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==SDOC========================================================================
//== Class: CCertificateSubjectOpenSsl
//========================================
//
// Summary:
//   Class that manages the certificate subject related identifiers and values.
//
// Description:
//   CCertificateSubject is the class that manages the certificate subject
//   related identifiers and values using Open SSL.
//
// Location:
//   Pki/CCertificateSubjectOpenSsl.h
//
// See Also:
//
//==EDOC========================================================================
class CCertificateSubjectOpenSsl
{
    friend class CCertificateOpenSsl;

// Published Enum
//----------------
public:
    // Summary:
    //  Defines the possible names in the Subject.
    enum EName
    {
        // Common name
        eCOMMON_NAME = NID_commonName,
        // Country name.
        eCOUNTRY_NAME = NID_countryName,
        // Locality name.
        eLOCALITY_NAME = NID_localityName,
        // State or province name.
        eSTATE_OR_PROVINCE_NAME = NID_stateOrProvinceName,
        // Organization name.
        eORGANIZATION_NAME = NID_organizationName,
        // Organizational unit name.
        eORGANIZATIONAL_UNIT_NAME = NID_organizationalUnitName
    };

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //-------------------
    CCertificateSubjectOpenSsl();

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateSubjectOpenSsl(IN const CCertificateSubjectOpenSsl* pCertificateSubject);

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateSubjectOpenSsl(IN const CCertificateSubjectOpenSsl& rCertificateSubject);

    // Summary:
    // Destructor.
    //-------------
    virtual ~CCertificateSubjectOpenSsl();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateSubjectOpenSsl& operator=(IN const CCertificateSubjectOpenSsl& rCertificateSubject);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CCertificateSubjectOpenSsl& rCertificateSubject) const;

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

    mxt_result Initialize(IN X509* pEvpX509);

    void Enter() const;
    void Exit() const;

// Hidden Data Members
//---------------------
protected:

    CCrypto* m_pCrypto;

    X509* m_pEvpX509;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==    CCertificateSubjectOpenSsl
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CCertificateSubjectOpenSsl::CCertificateSubjectOpenSsl()
:   m_pCrypto(NULL),
    m_pEvpX509(NULL)
{
    m_pCrypto = CCrypto::Instance();
}

//==SDOC========================================================================
//==
//==    CCertificateSubjectOpenSsl
//==
//==============================================================================
//
//  Parameters:
//      pCertificateSubject:
//          Pointer to the CCertificateSubjectOpenSsl to compare.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CCertificateSubjectOpenSsl::CCertificateSubjectOpenSsl(IN const CCertificateSubjectOpenSsl* pCertificateSubject)
:   m_pCrypto(NULL),
    m_pEvpX509(NULL)
{
    m_pCrypto = CCrypto::Instance();

    if (pCertificateSubject)
    {
        *this = *pCertificateSubject;
    }
    else
    {
        MX_ASSERT(pCertificateSubject);
    }
}

//==SDOC========================================================================
//==
//==    CCertificateSubjectOpenSsl
//==
//==============================================================================
//
//  Parameters:
//      rCertificateSubject:
//          Reference to the CCertificateSubjectOpenSsl to compare.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CCertificateSubjectOpenSsl::CCertificateSubjectOpenSsl(IN const CCertificateSubjectOpenSsl& rCertificateSubject)
:   m_pCrypto(NULL),
    m_pEvpX509(NULL)
{
    m_pCrypto = CCrypto::Instance();

    *this = rCertificateSubject;
}

//==SDOC========================================================================
//==
//==    ~CCertificateSubjectOpenSsl
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CCertificateSubjectOpenSsl::~CCertificateSubjectOpenSsl()
{
    if (m_pEvpX509)
    {
        X509_free(m_pEvpX509);
        m_pEvpX509 = NULL;
    }

    m_pCrypto = NULL;
}

inline
void CCertificateSubjectOpenSsl::Enter() const
{
    m_pCrypto->Enter();
}

inline
void CCertificateSubjectOpenSsl::Exit() const
{
    m_pCrypto->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_PKI_OPENSSL)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATESUBJECTOPENSSL_H

