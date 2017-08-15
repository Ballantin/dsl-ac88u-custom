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
#ifndef MXG_CCERTIFICATECHAINBASE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATECHAINBASE_H

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

#if !defined(MXD_PKI_NONE)

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif
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

//==SDOC========================================================================
//== Class: CCertificateChain
//========================================
//
// Summary:
//   Class that manages an ordered list of certificates.
//
// Description:
//   CCertificateChain common implementation.
//
// Location:
//   Pki/CCertificateChainBase.h
//
// See Also:
//  CCertificateChain
//
//==EDOC========================================================================
class CCertificateChainBase
{


// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //  Summary:
    //   Constructor.
    //----------------
    CCertificateChainBase();

    //  Summary:
    //   Copy constructor.
    //---------------------
    CCertificateChainBase(IN const CCertificateChainBase* pCertificateChain);

    //  Summary:
    //   Copy constructor.
    //---------------------
    CCertificateChainBase(IN const CCertificateChainBase& rCertificateChain);

    //  Summary:
    //   Destructor.
    //---------------------
    virtual ~CCertificateChainBase();

    //  Summary:
    //   Assignment operator.
    //------------------------
    CCertificateChainBase& operator=(IN const CCertificateChainBase& rCertificateChain);

    //  Summary:
    //   Comparison operator.
    //------------------------
    bool operator==(IN const CCertificateChainBase& rCertificateChain) const;

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
    // Summary:
    //  Locks the CCrypto Mutex.
    //---------------------------
    void Lock() const;

    // Summary:
    //  Unlocks the CCrypto Mutex.
    //-----------------------------
    void Unlock() const;

private:

    // Summary:
    //  Adds a list of certificates to an existing chain.
    //----------------------------------------------------
    mxt_result AddCertificateChain(IN bool bAddRootCertificate,
                                   IN const CVector<CCertificate>* pvecCertificatesToAdd,
                                   IN const CVector<CCertificate>* pvecCertificateChain,
                                   OUT CVector<CCertificate>* pvecNewCertificateChain,
                                   OUT unsigned int* puLeftOverCertificates);

// Hidden Data Members
//---------------------
protected:

    CVector<CCertificate> m_vecCertificates;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

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
void CCertificateChainBase::Clear()
{
    Lock();
    /**/
    /**/    m_vecCertificates.EraseAll();
    /**/
    Unlock();
}

//==SDOC========================================================================
//==
//==    Clear
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
bool CCertificateChainBase::IsEmpty()
{
    return m_vecCertificates.IsEmpty();
}

inline
void CCertificateChainBase::Lock() const
{
    CCrypto::Instance()->Enter();
}

inline
void CCertificateChainBase::Unlock() const
{
    CCrypto::Instance()->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATECHAINBASE_H

