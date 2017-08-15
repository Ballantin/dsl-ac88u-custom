//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_CCERTIFICATECHAIN_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATECHAIN_H
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
#if defined(MXD_PKI_CERTIFICATECHAIN_INCLUDE)
    #include MXD_PKI_CERTIFICATECHAIN_INCLUDE
#else
    #error "No implementation defined for CCertificateChain!!!"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif // #ifndef MXG_CPKI_H
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif // #ifndef MXG_CVECTOR_H
#ifndef MXG_CCERTIFICATE_H
#include "Pki/CCertificate.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

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
    defined(MXD_STUB_CCERTIFICATECHAIN)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CCERTIFICATECHAINSTUB_H
    #include "PkiStubs/CCertificateChainStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC========================================================================
//== Class: CCertificateChain
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class that manages an ordered list of certificates.
//
// Description:
//   CCertificateChain is the class that manages a list of ordered certificates.
//   The certificate chain is transmitted to a remote peer when this peer wants
//   to authenticate the local user/device.
//
//   A certificate chain is formed from the following certificates:
//      - A personal certificate, which identifies the local user or device.
//      - Zero or more intermediate certificates, which are certificates that
//        identify intermediary Certificate Authorities between the root
//        certificate and the personal certificate.
//      - Zero or one root certificate, provided by a trusted third-party
//        Certificate Authority (CA).
//
//   The ordered list of certificates, starting from the personal certificate
//   and ending at the root CA, usually denotes a delegation of trust where the
//   root CA trusts intermediate CA1, which in turn trusts intermediate CA2,
//   which in turn trusts intermediate CA3, and so on, up to the intermediate
//   CA that has issued the personal certificate to the local user or device,
//   which trusts the user's/device's identity.
//
//   Another method is to pass a list of certificates and let the API construct
//   the certificate chain. It orders the certificates with its issuer and
//   places it correctly in the list.
//
//   Certificates can be signed with two different signature algorithms: DSA and
//   RSA. The negotiated cipher suite determines whether a DSA or RSA signed
//   personal certificate is needed to identify the local user.
//   CCertificateChain does not allow multiple personal certificates to be
//   added.
//
//   It is important that the personal certificate loaded into the certificate
//   chain is configured with its corresponding private key, as this is used to
//   prove (by signing) that this is the real owner of the personal certificate.
//
//   The root certificate and intermediate certificates must be properly ordered
//   when they are pushed into the certificate chain with the Extend API. The
//   personal certificate must be inserted first, followed by the issuer of
//   the previous certificates up to the root CA. In other words, the
//   certificates are inserted from the nearest to the farthest, with the
//   personal certificate being inserted first and the root being inserted last.
//
//   The loaded intermediate certificates do not have to be configured with a
//   private key, which is usually inaccessible.
//
// Example:
//  <CODE>
//      \Extend(PersonalCa)
//      \Extend(CaN-1DescendantCaN)
//      ...
//      \Extend(Ca1DescendantCa2)
//      \Extend(Ca0DescendantCa1)
//      \Extend(Ca0)
//  </CODE>
//
//
// Location:
//   Pki/CCertificateChain.h
//
// See Also:
//  CCertificate
//
//==EDOC========================================================================
class CCertificateChain : public MXD_PKI_CCERTIFICATECHAIN_CLASSNAME
{

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //  Summary:
    //   Constructor.
    //----------------
    CCertificateChain();

    //  Summary:
    //   Copy constructor.
    //---------------------
    CCertificateChain(IN const CCertificateChain* pCertificateChain);

    //  Summary:
    //   Copy constructor.
    //---------------------
    CCertificateChain(IN const CCertificateChain& rCertificateChain);

    //  Summary:
    //   Destructor.
    //---------------------
    virtual ~CCertificateChain();

    //  Summary:
    //   Assignment operator.
    //------------------------
    CCertificateChain& operator=(IN const CCertificateChain& rCertificateChain);

    //  Summary:
    //   Comparison operator.
    //------------------------
    bool operator==(IN const CCertificateChain& rCertificateChain) const;

    // Summary:
    //  Removes all certificates from the certificate chain.
    //-------------------------------------------------------
    void Clear();

    // Summary:
    //  Inserts a certificate within the certificate chain.
    //------------------------------------------------------
    mxt_result Extend(IN const CCertificate* pCertificate,
                      IN bool bInsertRootCertificate = true);

    // Summary:
    //  Adds new certificates to the certificate chain.
    //--------------------------------------------------
    mxt_result Extend(IN const CVector<CCertificate>* pvecCertificates,
                      IN bool bInsertRootCertificate = true,
                      IN bool bEnforceRelation = true);

    // Summary:
    //  Gets the list of certificates configured in this chain.
    //----------------------------------------------------------
    mxt_result GetCertificates(OUT CVector<CCertificate>* pvecCertificates) const;

    // Summary:
    //  Gets the certificate of the end entity.
    //----------------------------------------------------------
    mxt_result GetEndEntityCertificate(OUT CCertificate* pCertificate) const;

    // Summary:
    //  Returns whether or not the certificate chain is empty.
    //---------------------------------------------------------
    bool IsEmpty();

    // Summary:
    //  Returns and removes the lowest PKI hierarchical certificate.
    //---------------------------------------------------------------
    mxt_result RemoveLowest(OUT CCertificate* pCertificate);

    // Summary:
    //  Returns and removes the highest PKI hierarchical certificate.
    //----------------------------------------------------------------
    mxt_result RemoveHighest(OUT CCertificate* pCertificate);

    // Summary:
    //  Displays the information for each certificate in the chain.
    //--------------------------------------------------------------
    mxt_result DisplayCertificateChain();

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
//==    CCertificateChain
//==
//==============================================================================
//
//  Description:
//      Basic constructor
//
//==EDOC========================================================================
inline
CCertificateChain::CCertificateChain()
:   MXD_PKI_CCERTIFICATECHAIN_CLASSNAME()
{
}

//==SDOC========================================================================
//==
//==    CCertificateChain
//==
//==============================================================================
//
//  Parameters:
//      pCertificateChain:
//          Pointer to a CCertificateChain.
//
//  Description:
//      Initializes this certificate chain with the pointed certificate chain.
//
//==EDOC========================================================================
inline
CCertificateChain::CCertificateChain(IN const CCertificateChain* pCertificateChain)
:   MXD_PKI_CCERTIFICATECHAIN_CLASSNAME(pCertificateChain)
{

}

//==SDOC========================================================================
//==
//==    CCertificateChain
//==
//==============================================================================
//
//  Parameters:
//      rCertificateChain:
//          Reference to a CCertificateChain.
//
//  Description:
//      Initializes this certificate chain with the referenced certificate
//      chain.
//
//==EDOC========================================================================
inline
CCertificateChain::CCertificateChain(IN const CCertificateChain& rCertificateChain)
:   MXD_PKI_CCERTIFICATECHAIN_CLASSNAME(rCertificateChain)
{

}

//==SDOC========================================================================
//==
//==    ~CCertificateChain
//==
//==============================================================================
//
//  Description:
//      Basic destructor
//
//==EDOC========================================================================
inline
CCertificateChain::~CCertificateChain()
{

}
//==SDOC========================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rCertificateChain:
//          Referenced certificate chain.
//
//  Returns:
//      Reference to the current CCertificateChain.
//
//  Description:
//      Sets the current certificate chain equal to the referenced one. This is
//      not a deep copy of the referenced chain.
//
//==EDOC========================================================================
inline
CCertificateChain& CCertificateChain::operator=(IN const CCertificateChain& rCertificateChain)
{
    MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::operator=(rCertificateChain);
    return *this;
}

//==SDOC========================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rCertificateChain:
//          Referenced certificate chain.
//
//  Returns:
//      true if both equal, false otherwise.
//
//  Description:
//      Compares the internal vectors of both certificate chains to see if they
//      are equal.
//
//==EDOC========================================================================
inline
bool CCertificateChain::operator==(IN const CCertificateChain& rCertificateChain) const
{
    return MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::operator==(rCertificateChain);
}


//==SDOC========================================================================
//==
//==    Clear
//==
//==============================================================================
//
//  Description:
//      Clears all certificates in the certificate chain.
//
//==EDOC========================================================================
inline
void CCertificateChain::Clear()
{
    MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::Clear();
}

//==SDOC========================================================================
//==
//==    Extend
//==
//==============================================================================
//
//  Parameters:
//      pCertificate:
//          Pointer to the certificate to append to the chain.
//
//      bInsertRootCertificate:
//          bool to determine whether or not to insert a root certificate into
//          the chain.
//
//  Returns:
//      resS_OK if the certificate has been successfully added, it is already in
//      the chain, or it is a root certificate pertaining to the current chain
//      and the bInsertRootCertificate flag is false. If the flag is true, the
//      root certificate is added as any other certificate. Returns mxt_result
//      error code otherwise.
//
//  Description:
//      Inserts a certificate at the end of the certificate chain.
//      Only a higher PKI level certificate can be added with this method.
//
//      A valid certificate is always appended to an empty chain. If a chain is
//      already present, the certificate is added only if it issued the
//      previous certificate.
//
//==EDOC========================================================================
inline
mxt_result CCertificateChain::Extend(IN const CCertificate* pCertificate,
                                     IN bool bInsertRootCertificate)
{
    return MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::Extend(pCertificate, bInsertRootCertificate);
}

//==SDOC========================================================================
//==
//==    Extend
//==
//==============================================================================
//
//  Parameters:
//      pvecCertificates:
//          Pointer to a vector containing the certificates to add.
//
//      bInsertRootCertificate:
//          bool to determine whether or not to insert the root certificates
//          into the chain.
//
//      bEnforceRelation:
//          bool flag to determine how to handle possible multiple chains.
//
//  Returns:
//      resS_OK if successful, mxt_result error code otherwise.
//
//  Description:
//      Adds any certificate in the vector to the current certificate chain.
//
//      Any root certificate is ignored if the bInsertRootCertificate is set to
//      false.
//
//      If the chain is empty, it builds the chain from the list. If there are
//      multiple initial chains, then the method fails and no certificate chain
//      is created.
//
//      In the case of an already existing chain, the bEnforceRelation
//      determines the handling of multiple chains. If the flag is set to false,
//      it extracts the items to append to the certificate chain and adds them
//      to the current chain. If it is set to true, it extends the chain only if
//      all certificates in the vector descend from the last certificate
//      currently in the chain. Certificates already in the current chain are
//      ignored. In both cases, if a lower PKI level is in the vector, the
//      function fails and the chain remains unchanged.
//
//      Any invalid certificate causes an error.
//
//==EDOC========================================================================
inline
mxt_result CCertificateChain::Extend(IN const CVector<CCertificate>* pvecCertificates,
                                     IN bool bInsertRootCertificate,
                                     IN bool bEnforceRelation)
{
    return MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::Extend(
                                    pvecCertificates,
                                    bInsertRootCertificate,
                                    bEnforceRelation);
}

//==SDOC========================================================================
//==
//==    GetCertificates
//==
//==============================================================================
//
//  Parameters:
//      pvecCertificates:
//          Vector to hold the certificate chain.
//
//  Returns:
//      resS_OK if successful, resFE_INVALID_ARGUMENT otherwise.
//
//  Description:
//      Gets the list of certificates configured in this chain.
//      The returned vector contains the ordered certificate list.
//
//==EDOC========================================================================
inline
mxt_result CCertificateChain::GetCertificates(OUT CVector<CCertificate>* pvecCertificates) const
{
    return MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::GetCertificates(
                                 pvecCertificates);
}

//==SDOC========================================================================
//==
//==    GetEndEntityCertificate
//==
//==============================================================================
//
//  Parameters:
//      pCertificate:
//          Pointer to the certificate.
//
//  Returns:
//      resS_OK
//      resFE_INVALID_ARGUMENT if the pointer is NULL
//      resSI_FALSE if there is no end entity certificate
//
//  Description:
//      Gets the certificate of the end entity. The end entity is the first
//      certificate in the chain, the one that is verified by all the other
//      certificates in the chain.
//
//==EDOC========================================================================
inline
mxt_result CCertificateChain::GetEndEntityCertificate(OUT CCertificate* pCertificate) const
{
    return MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::GetEndEntityCertificate(pCertificate);
}

//==SDOC========================================================================
//==
//==    IsEmpty
//==
//==============================================================================
//
//  Returns:
//      True if the chain is empty, false otherwise.
//
//  Description:
//      Returns whether or not the certificate chain is empty.
//
//==EDOC========================================================================
inline
bool CCertificateChain::IsEmpty()
{
    return MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::IsEmpty();
}

//==SDOC========================================================================
//==
//==    RemoveLowest
//==
//==============================================================================
//
//  Parameters:
//      pCertificate:
//          Pointer to hold the removed certificate.
//
//  Returns:
//      resS_OK if successful, resFE_INVALID_STATE otherwise.
//
//  Description:
//      Removes the lowest PKI hierarchical certificate in the list. If the
//      pCertificate pointer is NULL, then the certificate is simply discarded;
//      otherwise, it is copied before being removed.
//
//==EDOC========================================================================
inline
mxt_result CCertificateChain::RemoveLowest(OUT CCertificate* pCertificate)
{
    return MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::RemoveLowest(pCertificate);
}

//==SDOC========================================================================
//==
//==    RemoveHighest
//==
//==============================================================================
//
//  Parameters:
//      pCertificate:
//          Pointer to hold the removed certificate.
//
//  Returns:
//      resS_OK if successful, resFE_INVALID_STATE otherwise.
//
//  Description:
//      Removes the highest PKI hierarchical certificate in the list. If the
//      pCertificate pointer is NULL, then the certificate is simply discarded;
//      otherwise, it is copied before being removed.
//
//==EDOC========================================================================
inline
mxt_result CCertificateChain::RemoveHighest(OUT CCertificate* pCertificate)
{
    return MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::RemoveHighest(pCertificate);
}

//==SDOC========================================================================
//==
//==    DisplayCertificateChain
//==
//==============================================================================
//
//  Returns:
//      resS_OK if there is a non-empty certificate chain to display and that
//      all certificates are valid, an error otherwise.
//
//  Description:
//      Displays the information of the certificate chain. The chain is
//      displayed from the lowest PKI hierarchical certificate to the highest.
//
//==EDOC========================================================================
inline
mxt_result CCertificateChain::DisplayCertificateChain()
{
    return MXD_PKI_CCERTIFICATECHAIN_CLASSNAME::DisplayCertificateChain();
}

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CCERTIFICATECHAIN)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CCERTIFICATECHAIN_H

