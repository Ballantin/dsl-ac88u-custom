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
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CPUBLICKEYMOCANASS_H
#define MXG_CPUBLICKEYMOCANASS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_PUBLICKEY_MOCANA_SS
#endif

#if defined(MXD_CRYPTO_PUBLICKEY_MOCANA_SS)

#ifndef MXG_CASYMMETRICKEYMOCANASS_H
#include "Crypto/CAsymmetricKeyMocanaSs.h"
#endif

#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif

#ifndef __PUBCRYPTO_HEADER__
extern "C"
{
    #include "crypto/pubcrypto.h"
}
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CBlob;
class CPrivateKey;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#ifndef MXD_CRYPTO_PUBLICKEY_CLASSNAME
    #define MXD_CRYPTO_PUBLICKEY_CLASSNAME CPublicKeyMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CPublicKeyMocanaSs : public CAsymmetricKeyMocanaSs
{
//-- Friend Declarations
friend class CRsaMocanaSs;

// Published Interface.
public:

    // << Constructors / Destructors >>.
    CPublicKeyMocanaSs();
    CPublicKeyMocanaSs(IN const CPublicKeyMocanaSs* pKey);
    CPublicKeyMocanaSs(IN const CPublicKeyMocanaSs& rKey);
    virtual ~CPublicKeyMocanaSs();

    // Summary:
    //  Initialize the m_pstKey attribute with the information provided.
    mxt_result SetKey(IN AsymmetricKey* pKey);

    // Summary:
    //  Retreives the information contained in the m_pstKey attribute.
    mxt_result GetKey(IN AsymmetricKey* pKey) const;

// Hidden Methods.
protected:
    // Summary:
    //   Generates a new RSA key.
    mxt_result GenerateKey(IN AsymmetricKey* pstPrivateKey);
};

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_CRYPTO_PUBLICKEY_MOCANA_SS)

#endif // MXG_CPUBLICKEYMOCANASS_H
//M5T_INTERNAL_USE_END
