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
#ifndef MXG_CCERTIFICATEDESCRIPTORMOCANASS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATEDESCRIPTORMOCANASS_H

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
#include "Config/FrameworkCfg.h" // MXD_PKI_MOCANA_SS
#endif

#if defined(MXD_PKI_MOCANA_SS)

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif

#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

extern "C" {
#ifndef __CA_MGMT_HEADER__
#include "crypto/ca_mgmt.h"
#endif

#ifndef __ABSSTREAM_H__
#include "common/absstream.h"
#endif

#ifndef __MEMFILE_H__
#include "common/memfile.h"
#endif

#ifndef __TREE_H__
#include "common/tree.h"
#endif

#ifndef __PARSEASN1_H__
#include "asn1/parseasn1.h"
#endif
}
//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CPrivateKey;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CCertificateDescriptorMocanaSs
//========================================
//
// Summary:
//   Class that maintain a counted CertDescriptor.
//
// Description:
//   Maintains a counted CertDescriptor and related attribute needed to access
//   certificate attributes with Mocanan SS.
//
// Location:
//   Pki/CCertificateDescriptorMocanaSs.h
//
// See Also:
//  CCertificateChain
//
//==EDOC========================================================================
class CCertificateDescriptorMocanaSs
{

    friend class CCertificateMocanaSs;
    friend class CCertificateExtensionMocanaSs;
    friend class CAuthorityKeyIdentifierMocanaSs;
    friend class CAlternateNameMocanaSs;
    friend class CKeyUsageMocanaSs;
    friend class CExtendedKeyUsageMocanaSs;

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //  Summary:
    //   Constructor.
    //----------------
    CCertificateDescriptorMocanaSs();

    //  Summary:
    //   Destructor.
    //---------------------
    virtual ~CCertificateDescriptorMocanaSs();

    //  Summary:
    //   Returns a copy of the internal certificate structure.
    mxt_result Copy(INOUT certDescriptor* pstDescriptor);

    //  Summary:
    //   Increment reference on the class instance.
    //----------------------------------------------
    void IncrementRef();

    //  Summary:
    //   Decrement reference on the class instance.
    //----------------------------------------------
    void DecrementRef();

    //  Summary:
    //   Initialize the attribute members with the information provided.
    //-------------------------------------------------------------------
    mxt_result Initialize(IN const CBlob& rPemCertificate, IN const CPrivateKey* pPrivateKey);

    //  Summary:
    //   Initialize the m_blobKeyDer attribute member and some fields of 
    //   m_CertDescriptor with the information provided.
    //-------------------------------------------------------------------
    mxt_result Initialize(IN const CPrivateKey* pPrivateKey);

// Hidden Methods
//----------------
protected:
private:

    //  Summary:
    //   Copy constructor.
    //---------------------
    CCertificateDescriptorMocanaSs(IN const CCertificateDescriptorMocanaSs* pCertificateChain);

    //  Summary:
    //   Copy constructor.
    //---------------------
    CCertificateDescriptorMocanaSs(IN const CCertificateDescriptorMocanaSs& rCertificateChain);

    //  Summary:
    //   Assignment operator.
    //------------------------
    CCertificateDescriptorMocanaSs& operator=(IN const CCertificateDescriptorMocanaSs& rCertificateChain);

    void Enter() const;
    void Exit() const;


// Data Members
//---------------------
private:
    CBlob m_blobCertificateDer;
    CBlob m_blobKeyDer;
    certDescriptor m_CertDescriptor;
    ASN1_ITEM* m_pRootItem;
    ASN1_ITEM* m_pCertificateBeginning;
    ASN1_ITEM* m_pExtensionsBeginning;
    MemFile m_mf;
    CStream m_cs;
    uint32_t m_uCounted;
    CCrypto* m_pCrypto;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
//==============================================================================
//==
//==  Copy
//==
//==============================================================================
//
//  Summary:
//      Returns a copy of the internal certificate structure.
//
//  Parameters:
//      ppstDescriptor:
//       The pointer to the internal structure. It must not be NULL and must
//       point to an empty memory allocation the size of certDescriptor.
//
//  Returns:
//      resS_OK: The copy has been succesful.
//      resFE_INVALID_ARGUMENT: ppstDescriptor is NULL.
//
//  Description:
//      Returns a copy of the internal certificate structure.
//
//==============================================================================
inline 
mxt_result CCertificateDescriptorMocanaSs::Copy(INOUT certDescriptor* pstDescriptor)
{

    mxt_result res = resS_OK;

    if (pstDescriptor == NULL)
    {
        res = resFE_INVALID_ARGUMENT;
    }
    else
    {
        memcpy(pstDescriptor, &m_CertDescriptor, sizeof(m_CertDescriptor));
    }

    return res;
}

inline
void CCertificateDescriptorMocanaSs::Enter() const
{
    m_pCrypto->Enter();
}

inline
void CCertificateDescriptorMocanaSs::Exit() const
{
    m_pCrypto->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_MOCANA_SS)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATEDESCRIPTORMOCANASS_H

