//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CASYMMETRICKEYMOCANASS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYMMETRICKEYMOCANASS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_PUBLICKEY_MOCANA_SS, MXD_CRYPTO_PRIVATEKEY_MOCANA_SS
#endif

#if defined (MXD_CRYPTO_PUBLICKEY_MOCANA_SS ) || defined (MXD_CRYPTO_PRIVATEKEY_MOCANA_SS)


#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif

#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

extern "C"
{
    #include "crypto/pubcrypto.h"
}

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBlob;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CAsymmetricKeyMocanaSs
//==============================================================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   This is the base class for asymmetric keys using the Mocana SS library.
//
// Description:
//   This class encpasulates the Mocana SS-specific library functions into a
//   generic interface.
//
// Location:
//   Crypto/CAsymmetricKeyMocanaSs.h
//
//==============================================================================
class CAsymmetricKeyMocanaSs
{
//-- New types within class namespace
public:

    //<GROUP CRYPTO_ENUMS>
    //
    // Summary:
    // Defines the various algorithms a key can be used for.
    //-------------------------------------------------------
    enum EAlgorithm
    {
        // Description:
        // Unknown algorithm.
        eALGORITHM_UNKNOWN = 0,
        // Description:
        // RSA algorithm.
        eALGORITHM_RSA = 1
    };

    // Summary:
    // Defines the types of encoding the key can be in.
    //-------------------------------------------------------
    enum EEncoding
    {
        // Description:
        // DER encoding.
        eENCODING_DER,
        // Description:
        // PEM encoding.
        eENCODING_PEM
    };

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //   Assignment operator.
    CAsymmetricKeyMocanaSs& operator=(IN const CAsymmetricKeyMocanaSs& rKey);

    // Summary:
    //   Equality operator.
    bool operator==(IN const CAsymmetricKeyMocanaSs& rKey) const;

    // Summary:
    //   Inequality operator.
    bool operator!=(IN const CAsymmetricKeyMocanaSs& rKey) const;

//M5T_INTERNAL_USE_BEGIN
    mxt_result Restore(IN const CBlob* pblobKey,
                       IN EAlgorithm eAlgorithm);
    mxt_result RestoreDer(IN const CBlob* pblobKey, IN EAlgorithm eAlgorithm);
    mxt_result RestorePem(IN const CBlob* pblobKey);

    mxt_result Store(OUT CBlob* pblobKey, IN EEncoding eEncoding) const;
    mxt_result StoreDer(OUT CBlob* pblobKey) const;
    mxt_result StorePem(OUT CBlob* pblobKey) const;
    mxt_result GetAlgorithm(OUT EAlgorithm* peAlgorithm) const;
//M5T_INTERNAL_USE_END

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    // Summary:
    //  Default Constructor.
    CAsymmetricKeyMocanaSs();

    // Summary:
    //  Copy Constructor, pointer version.
    CAsymmetricKeyMocanaSs(IN const CAsymmetricKeyMocanaSs* pKey);

    // Summary:
    //  Copy Constructor, reference version.
    CAsymmetricKeyMocanaSs(IN const CAsymmetricKeyMocanaSs& rKey);

    // Summary:
    //  Destructor.
    virtual ~CAsymmetricKeyMocanaSs();

    // Summary:
    //   cast operator.
    operator RSAKey* () const;

    // Summary:
    //   cast operator.
    operator AsymmetricKey* () const;

    // Summary:
    //   Releases the internal structure.
    mxt_result Release();

    // Summary:
    //   Initializes the internal structure.
    mxt_result Reset();

//-- Hidden Data Members
protected:
    // The internal structure defining the key.
    AsymmetricKey* m_pstKey;

    // The big numbers allocation queue.
    vlong* m_pQueue;

    // The key in binary format.
    CBlob m_blobKey;

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined (MXD_CRYPTO_PUBLICKEY_MOCANA_SS ) || defined (MXD_CRYPTO_PRIVATEKEY_MOCANA_SS)

#endif //-- #ifndef MXG_CASYMMETRICKEYMOCANASS_H


