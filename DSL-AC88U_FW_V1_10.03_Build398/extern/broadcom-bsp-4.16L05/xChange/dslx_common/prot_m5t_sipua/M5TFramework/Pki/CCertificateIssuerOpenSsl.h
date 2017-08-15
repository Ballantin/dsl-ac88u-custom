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
#ifndef MXG_CCERTIFICATEISSUEROPENSSL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATEISSUEROPENSSL_H

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
#include "Config/FrameworkCfg.h"
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
#ifndef MXD_PKI_CCERTIFICATEISSUER_CLASSNAME
    #define MXD_PKI_CCERTIFICATEISSUER_CLASSNAME CCertificateIssuerOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CCertificateIssuerOpenSsl
//========================================
//
// Summary:
//   Class managing the certificate issuer related identifiers and values using
//   open SSL.
//
// Description:
//   CCertificateIssuer is the class that manages the certificate issuer related
//   identifiers and values.
//
// Location:
//   Pki/CCertificateIssuerOpenSsl.h
//
// See Also:
//
//==EDOC========================================================================
class CCertificateIssuerOpenSsl
{
    friend class CCertificateOpenSsl;

// Published Enum
//----------------
public:
    // Summary:
    //  Defines the possible names in the issuer.
    enum EName
    {
        // Common name.
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
    CCertificateIssuerOpenSsl();

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateIssuerOpenSsl(IN const CCertificateIssuerOpenSsl* pCertificateIssuer);

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateIssuerOpenSsl(IN const CCertificateIssuerOpenSsl& rCertificateIssuer);

    // Summary:
    // Destructor.
    //-------------
    virtual ~CCertificateIssuerOpenSsl();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateIssuerOpenSsl& operator=(IN const CCertificateIssuerOpenSsl& rCertificateIssuer);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CCertificateIssuerOpenSsl& rCertificateIssuer) const;

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
//==    CCertificateIssuer
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CCertificateIssuerOpenSsl::CCertificateIssuerOpenSsl()
:   m_pCrypto(NULL),
    m_pEvpX509(NULL)
{
    m_pCrypto = CCrypto::Instance();
}

//==SDOC========================================================================
//==
//==    CCertificateIssuerOpenSsl
//==
//==============================================================================
//
//  Parameters:
//      pCertificateIssuerOpenSsl:
//          Pointer to the CCertificateIssuerOpenSsl to compare.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CCertificateIssuerOpenSsl::CCertificateIssuerOpenSsl(IN const CCertificateIssuerOpenSsl* pCertificateIssuer)
:   m_pCrypto(NULL),
    m_pEvpX509(NULL)
{
    m_pCrypto = CCrypto::Instance();

    if (pCertificateIssuer)
    {
        *this = *pCertificateIssuer;
    }
    else
    {
        MX_ASSERT(pCertificateIssuer);
    }
}

//==SDOC========================================================================
//==
//==    CCertificateIssuerOpenSsl
//==
//==============================================================================
//
//  Parameters:
//      rCertificateIssuer:
//          Reference to the CCertificateIssuerOpenSsl to compare.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CCertificateIssuerOpenSsl::CCertificateIssuerOpenSsl(IN const CCertificateIssuerOpenSsl& rCertificateIssuer)
:   m_pCrypto(NULL),
    m_pEvpX509(NULL)
{
    m_pCrypto = CCrypto::Instance();

    *this = rCertificateIssuer;
}

//==SDOC========================================================================
//==
//==    ~CCertificateIssuerOpenSsl
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CCertificateIssuerOpenSsl::~CCertificateIssuerOpenSsl()
{
    if (m_pEvpX509)
    {
        X509_free(m_pEvpX509);
        m_pEvpX509 = NULL;
    }

    m_pCrypto = NULL;
}

inline
void CCertificateIssuerOpenSsl::Enter() const
{
    m_pCrypto->Enter();
}

inline
void CCertificateIssuerOpenSsl::Exit() const
{
    m_pCrypto->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_PKI_OPENSSL)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATEISSUEROPENSSL_H

