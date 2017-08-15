//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==EDOC========================================================================
#ifndef MXG_CPKCS12_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CPKCS12_H
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

#if defined(MXD_PKI_OPENSSL)

//-- Data Members
//-----------------
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif // #ifndef MXG_CVECTOR_H
#ifndef MXG_CCERTIFICATE_H
#include "Pki/CCertificate.h"
#endif // #ifndef MXG_CCERTIFICATE_H
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
class CCertificateChain;
class CPrivateKey;
class IPassPhrase;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CPKCS12)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CPKCS12STUB_H
    #include "PkiStubs/CPkcs12Stub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC========================================================================
//== Class: CPkcs12
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class that manages the creation and parsing of PKCS#12 v1.0 formatted
//   buffers.
//
// Description:
//   CPkcs12 is the class that manages the creation and parsing of PKCS#12 v1.0
//   formatted buffers. PKCS#12 is a file format used to manage personal
//   information. In other words, a personal certificate, a personal private
//   key, and possibly one or more intermediate certificates.
//
// NOTE:
//   For this class to work correctly under OpenSSL, RC2 and 3DES must have been
//   included within the library. They should be enabled if no-rc2 and no-des
//   are absent from the command line arguments passed to Configure.
//
// Location:
//   Pki/CPkcs12.h
//
// See Also:
//   CCertifcateChain
//
//==EDOC========================================================================
class CPkcs12
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //-------------------
    CPkcs12();

    // Summary:
    // Copy constructor.
    //-------------------
    CPkcs12(IN const CPkcs12* pPkcs12);

    // Summary:
    // Copy constructor.
    //-------------------
    CPkcs12(IN const CPkcs12& rPkcs12);

    // Summary:
    // Destructor.
    //-------------------
    virtual ~CPkcs12();

    // Summary:
    // Assignment operator.
    //----------------------
    CPkcs12& operator=(IN const CPkcs12& rPkcs12);

    // Summary:
    //  Retrieves the certificate chain information associated with this
    //  CPkcs12 instance.
    //-------------------------------------------------------------------
    mxt_result GetCertificateChain(OUT CCertificateChain* pCertificateChain) const;

    // Summary:
    //  Restores a PKCS#12 structure from its serialized state.
    //----------------------------------------------------------
    mxt_result Restore(IN const uint8_t* puPkcs12,
                       IN unsigned int uPkcs12Size,
                       IN const char* pszPassPhrase);

    // Summary:
    //  Restores a PKCS#12 structure from its serialized state.
    //----------------------------------------------------------
    mxt_result Restore(IN const CBlob* pblobPkcs12,
                       IN const char* pszPassPhrase);

    // Summary:
    //  Restores a PKCS#12 structure from its serialized state.
    //----------------------------------------------------------
    mxt_result Restore(IN const uint8_t* puPkcs12,
                       IN unsigned int uPkcs12Size,
                       IN IPassPhrase* pPassPhrase,
                       IN mxt_opaque opqPassPhraseParameter);

    // Summary:
    //  Restores a PKCS#12 structure from its serialized state.
    //----------------------------------------------------------
    mxt_result Restore(IN const CBlob* pblobPkcs12,
                       IN IPassPhrase* pPassPhrase,
                       IN mxt_opaque opqPassPhraseParameter);

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:
private:

    void Enter() const;
    void Exit() const;

// Hidden Data Members
//---------------------
protected:
private:

    CVector<CCertificate> m_vecIntermediateCertificates;
    CCertificate* m_pPersonalCertificate;
    CPrivateKey* m_pPrivateKey;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
inline
void CPkcs12::Enter() const
{
    CPki::Instance()->Enter();
}

inline
void CPkcs12::Exit() const
{
    CPki::Instance()->Exit();
}
//M5T_INTERNAL_USE_END

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CPKCS12)

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_OPENSSL)

#endif // MXG_CPKCS12_H

