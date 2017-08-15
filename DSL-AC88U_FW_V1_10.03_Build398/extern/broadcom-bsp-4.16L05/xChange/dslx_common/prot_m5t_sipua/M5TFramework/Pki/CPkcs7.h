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
#ifndef MXG_CPKCS7_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CPKCS7_H
//M5T_INTERNAL_USE_ENDS

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

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CPKCS7)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CPKCS7STUB_H
    #include "PkiStubs/CPkcs7Stub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC========================================================================
//== Class: CPkcs7
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class that manages the creation and parsing of PKCS#7 v1.5 formatted buffer.
//
// Description:
//   CPkcs7 is the class that manages the creation and parsing of PKCS#7 v1.5
//   formatted buffer. PKCS#7 is a file format used to store various
//   cryptographic contents. Currently, only certificates are retrieved.
//
// Location:
//   Pki/CPkcs7.h
//
// See Also:
//   CCertificate
//
//==EDOC========================================================================
class CPkcs7
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //--------------
    CPkcs7();

    // Summary:
    // Copy constructor.
    //-------------------
    CPkcs7(IN const CPkcs7* pPkcs7);

    // Summary:
    // Copy constructor.
    //-------------------
    CPkcs7(IN const CPkcs7& rPkcs7);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CPkcs7();

    // Summary:
    // Assignment operator.
    //----------------------
    CPkcs7& operator=(IN const CPkcs7& rPkcs7);

    // Summary:
    //  Retrieves the certificates contained within this CPkcs7 instance.
    //--------------------------------------------------------------------
    mxt_result GetCertificates(OUT CVector<CCertificate>* pvecCertificates) const;

    // Summary:
    //  Restores a PKCS#7 structure from its serialized state.
    //---------------------------------------------------------
    mxt_result Restore(IN const uint8_t* puPkcs7,
                       IN unsigned int uPkcs7Size);

    // Summary:
    //  Restores a PKCS#7 structure from its serialized state.
    //---------------------------------------------------------
    mxt_result Restore(IN const CBlob* pblobPkcs7);

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

    CVector<CCertificate> m_vecCertificates;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
inline
void CPkcs7::Enter() const
{
    CPki::Instance()->Enter();
}

inline
void CPkcs7::Exit() const
{
    CPki::Instance()->Exit();
}
//M5T_INTERNAL_USE_END

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CPKCS7)

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_OPENSSL)

#endif // MXG_CPKCS7_H

