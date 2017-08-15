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
#ifndef MXG_CCERTIFICATENAMEMOCANASS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATENAMEMOCANASS_H

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

#if defined(MXD_PKI_MOCANA_SS)
//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif // #ifndef MXG_CPKI_H

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

//-- Parameters used by Value
//-----------------------------
extern "C" {
#ifndef __CA_MGMT_HEADER__
#include "crypto/ca_mgmt.h"
#endif
}

#ifndef MXG_MXTRACEPKI_H
#include "Pki/MxTracePki.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CCERTIFICATEISSUER_CLASSNAME
    #define MXD_PKI_CCERTIFICATEISSUER_CLASSNAME CCertificateNameMocanaSs
#endif

#ifndef MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME
    #define MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME CCertificateNameMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CCertificateNameMocanaSs
//========================================
//
// Summary:
//   Class managing the certificate issuer and subject related identifiers and
//   values using Nocana SS.
//
// Description:
//   CCertificateNameMocanaSs is the class that manages the certificate issuer
//   and subject related identifiers and values.
//
// Location:
//   Pki/CCertificateNameMocanaSs.h
//
// See Also:
//
//==============================================================================
class CCertificateNameMocanaSs
{
    friend class CCertificateMocanaSs;
    friend class CCertificateChainMocanaSs;
    friend class CAsyncTlsSocket;

// Published Enum
//----------------
public:

    // Summary:
    //  Defines the possible names in the issuer.
    enum EName
    {
        // Common name.
        eCOMMON_NAME,
        // Country name.
        eCOUNTRY_NAME,
        // Locality name.
        eLOCALITY_NAME,
        // State or province name.
        eSTATE_OR_PROVINCE_NAME,
        // Organization name.
        eORGANIZATION_NAME,
        // Organizational unit name.
        eORGANIZATIONAL_UNIT_NAME
    };

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //-------------------
    CCertificateNameMocanaSs();

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateNameMocanaSs(IN const CCertificateNameMocanaSs* pCertificateName);

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateNameMocanaSs(IN const CCertificateNameMocanaSs& rCertificateName);

    // Summary:
    // Destructor.
    //-------------
    virtual ~CCertificateNameMocanaSs();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateNameMocanaSs& operator=(IN const CCertificateNameMocanaSs& rCertificateName);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CCertificateNameMocanaSs& rCertificateName) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the specified name.
    //--------------------------
    mxt_result GetName(IN EName eName, OUT CString* pstrName, OUT bool* pbExist) const;

    // Summary:
    //  Gets the vector of all names.
    mxt_result GetNames(IN EName eName, OUT CVector<CString>* pvecstrNames) const;

// Hidden Methods
//----------------
protected:
private:
    // Summary:
    //  Initializes the class instance with the certDistinguishedName
    //  structure provided.
    //----------------------------------------------------------------
    mxt_result SetRelativeNames(IN certDistinguishedName* pstCertDistinguishedName);

    // Summary:
    //  Initializes one of the name attribute of this class instance with the
    //  information provided.
    //------------------------------------------------------------------------
    mxt_result SetName(IN EName eName, uint8_t* puValue, uint32_t uValueLen);

    // Summary:
    //  Compares this certificate name object with provided one.
    //-----------------------------------------------------------
    bool Compare(IN const CCertificateNameMocanaSs& rCertificateName,
                       IN bool bCompareUninitialized) const;

    // Summary:
    //  Matches this certificate name with the provided one using it's initialized attribute.
    //----------------------------------------------------------------------------------------
    mxt_result IsMatching(IN CCertificateNameMocanaSs* pCertificateName, OUT bool* pbMatching) const;

// Hidden Data Members
//---------------------
protected:

    // Country name attribute.
    CString m_strCountryName;

    // Locality name attribute.
    CString m_strLocalityName;

    // State or province name attribute.
    CString m_strStateProvinceName;

    // Organization name attribute.
    CString m_strOrganizationName;

    // Organization unit name attribute.
    CString m_strOrganizationUnitName;

    // Common name attribute.
    CVector<CString> m_vecstrCommonName;

private:
    static unsigned int ms_uATTRIBUTE_OID_LENGTH;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==    CCertificateIssuer
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CCertificateNameMocanaSs::CCertificateNameMocanaSs()
{
    MX_TRACE6(0,
              &g_stFrameworkPki,
              "CCertificateNameMocanaSs(%p)::CCertificateNameMocanaSs()",
              this);

    MX_TRACE7(0,
              &g_stFrameworkPki,
              "CCertificateNameMocanaSs(%p)::CCertificateNameMocanaSsExit()",
              this);
}

//==============================================================================
//==
//==    CCertificateNameMocanaSs
//==
//==============================================================================
//
//  Parameters:
//      pCertificateNameMocanaSs:
//          Pointer to the CCertificateIssuerMocanaSs to compare.
//
//  Description:
//      Copy constructor.
//
//==============================================================================
inline
CCertificateNameMocanaSs::CCertificateNameMocanaSs(IN const CCertificateNameMocanaSs* pCertificateName)
{
    MX_TRACE6(0,
              &g_stFrameworkPki,
              "CCertificateNameMocanaSs(%p)::CCertificateNameMocanaSs(%p)",
              this,
              pCertificateName);

    if (pCertificateName)
    {
        *this = *pCertificateName;
    }
    else
    {
        MX_ASSERT(pCertificateName);
    }

    MX_TRACE7(0,
              &g_stFrameworkPki,
              "CCertificateNameMocanaSs(%p)::CCertificateNameMocanaSsExit()",
              this);
}

//==============================================================================
//==
//==    CCertificateNameMocanaSs
//==
//==============================================================================
//
//  Parameters:
//      rCertificateName:
//          Reference to the CCertificateNameMocanaSs to compare.
//
//  Description:
//      Copy constructor.
//
//==============================================================================
inline
CCertificateNameMocanaSs::CCertificateNameMocanaSs(IN const CCertificateNameMocanaSs& rCertificateName)
{
    MX_TRACE6(0,
              &g_stFrameworkPki,
              "CCertificateNameMocanaSs(%p)::CCertificateNameMocanaSs(%p)",
              this,
              &rCertificateName);

    *this = rCertificateName;

    MX_TRACE7(0,
              &g_stFrameworkPki,
              "CCertificateNameMocanaSs(%p)::CCertificateNameMocanaSsExit()",
              this);
}

//==============================================================================
//==
//==    ~CCertificateNameMocanaSs
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline
CCertificateNameMocanaSs::~CCertificateNameMocanaSs()
{
    MX_TRACE6(0,
              &g_stFrameworkPki,
              "CCertificateNameMocanaSs(%p)::~CCertificateNameMocanaSs()",
              this);

    MX_TRACE7(0,
              &g_stFrameworkPki,
              "CCertificateNameMocanaSs(%p)::~CCertificateNameMocanaSsExit()",
              this);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_PKI_MOCANA_SS)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATENAMEMOCANASS_H

