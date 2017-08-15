//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CPUBLICKEY_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CPUBLICKEY_H
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
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_PUBLICKEY_NONE
#endif

#if !defined(MXD_CRYPTO_PUBLICKEY_NONE)

#if defined(MXD_CRYPTO_PUBLICKEY_INCLUDE)
    #include MXD_CRYPTO_PUBLICKEY_INCLUDE
#else
    #error "No implementation defined for CPublicKey!!!"
#endif

//-- Data Members
//-----------------

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
//== Class: CPublicKey
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages the public key for all algorithms.
//
// Description:
//   CPublicKey is the class that manages the public key for all algorithms.
//   The public key is mostly used for authentication purposes.
//
// Location:
//   Crypto/CPublicKey.h
//
// See Also:
//   CPrivateKey
//
//==EDOC========================================================================
class CPublicKey : public MXD_CRYPTO_PUBLICKEY_CLASSNAME
{
//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //==SDOC====================================================================
    //==
    //==    CPublicKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Default constructor.
    //
    //==EDOC====================================================================
    CPublicKey();

    //==SDOC====================================================================
    //==
    //==    CPublicKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Copy constructor.
    //
    //  Parameters:
    //      pKey:  Pointer to the key from which to copy.
    //
    //  Description:
    //      Copy constructor.
    //
    //==EDOC====================================================================
    CPublicKey(IN const CPublicKey* pKey);

    //==SDOC====================================================================
    //==
    //==    CPublicKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Copy constructor.
    //
    //  Parameters:
    //      rKey:  Reference to the key from which to copy.
    //
    //  Description:
    //      Copy constructor.
    //
    //==EDOC====================================================================
    CPublicKey(IN const CPublicKey& rKey);

    //==SDOC====================================================================
    //==
    //==    ~CPublicKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==EDOC====================================================================
    virtual ~CPublicKey();

    //==SDOC====================================================================
    //==
    //==    operator=
    //==
    //==========================================================================
    //
    //  Summary:
    //      Assignment operator.
    //
    //  Parameters:
    //      rKey:
    //          Reference to the key to assign.
    //
    //  Returns:
    //      A reference to the assigned key.
    //
    //  Description:
    //      Assigns the right hand key to the left hand one.
    //
    //==EDOC====================================================================
    CPublicKey& operator=(IN const CPublicKey& rKey);

    //==SDOC====================================================================
    //==
    //==    operator==
    //==
    //==========================================================================
    //
    //  Summary:
    //      Comparison operator.
    //
    //  Parameters:
    //      rKey:
    //          Reference to the key with which to compare.
    //
    //  Returns:
    //      True if both objects are the same, false otherwise.
    //
    //  Description:
    //      Verifies that both objects are equal.
    //
    //==EDOC====================================================================
    bool operator==(IN const CPublicKey& rKey) const;

    //==SDOC====================================================================
    //==
    //==    operator!=
    //==
    //==========================================================================
    //
    //  Summary:
    //      Different than algorithm.
    //
    //  Parameters:
    //      rKey:
    //          Reference to the key to copy.
    //
    //  Returns:
    //      True if both objects are different, false otherwise.
    //
    //  Description:
    //      Verifies if both objects are different.
    //
    //==EDOC====================================================================
    bool operator!=(IN const CPublicKey& rKey) const;

    //==SDOC====================================================================
    //==
    //==    Restore
    //==
    //==========================================================================
    //
    //  Summary:
    //      Restores a key from its serialized state.
    //
    //  Parameters:
    //      pblobKey:
    //          Pointer to the blob containing the key to restore.
    //
    //      eAlgorithm:
    //          The algorithm for which the key is used.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT
    //      - resFE_FAIL
    //      - resS_OK
    //
    //  Description:
    //      Restores a key from its serialized state.
    //
    //==EDOC====================================================================
    mxt_result Restore(IN const CBlob* pblobKey,
                       IN EAlgorithm eAlgorithm);

    //==SDOC====================================================================
    //==
    //==    RestoreDer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Restores a public key for its serialized state DER format.
    //
    //  Parameters:
    //      pblobKey:
    //          Pointer to the blob containing the key to restore.
    //
    //      eAlgorithm:
    //          The algorithm for which the key is used.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT
    //      - resFE_FAIL
    //      - resS_OK
    //
    //  Description:
    //      Restores a key from its serialized state.
    //
    //==EDOC====================================================================
    mxt_result RestoreDer(IN const CBlob* pblobKey,
                          IN EAlgorithm eAlgorithm);

    //==SDOC====================================================================
    //==
    //==    RestorePem
    //==
    //==========================================================================
    //
    //  Summary:
    //      Restores a public key for its serialized state PEM format.
    //
    //  Parameters:
    //      pblobKey:
    //          Pointer to the blob containing the key to restore.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT
    //      - resFE_FAIL
    //      - resS_OK
    //
    //  Description:
    //      Restores a key from its serialized state.
    //
    //==EDOC====================================================================
    mxt_result RestorePem(IN const CBlob* pblobKey);

    //==SDOC====================================================================
    //==
    //==    Store
    //==
    //==========================================================================
    //
    //  Summary:
    //      Stores a key to its serialized state.
    //
    //  Parameters:
    //      pblobKey:
    //          Pointer to the blob to contain the key.
    //
    //      eEncoding:
    //          Encoding type to use to serialize the key.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT
    //      - resFE_FAIL
    //      - resS_OK
    //
    //  Description:
    //      Stores a key in its serialized state.
    //
    //==EDOC====================================================================
    mxt_result Store(OUT CBlob* pblobKey, IN EEncoding eEncoding) const;

    //==SDOC====================================================================
    //==
    //==    StoreDer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Stores a public key to its serialized state DER format.
    //
    //  Parameters:
    //      pblobKey:
    //          Pointer to the blob to contain the key.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT
    //      - resFE_INVALID_STATE
    //      - resFE_FAIL
    //      - resS_OK
    //
    //  Description:
    //      Stores a key in its serialized state.
    //
    //==EDOC====================================================================
    mxt_result StoreDer(OUT CBlob* pblobKey) const;

    //==SDOC====================================================================
    //==
    //==    StorePem
    //==
    //==========================================================================
    //
    //  Summary:
    //      Stores a public key to its serialized state PEM format.
    //
    //  Parameters:
    //      pblobKey:
    //          Pointer to the blob to contain the key.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT
    //      - resFE_INVALID_STATE
    //      - resFE_FAIL
    //      - resS_OK
    //
    //  Description:
    //      Stores a key in its serialized state.
    //
    //==EDOC====================================================================
    mxt_result StorePem(OUT CBlob* pblobKey) const;

    //==SDOC====================================================================
    //==
    //==    GetAlgorithm
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the crypto algorithm associated with the key.
    //
    //  Parameters:
    //      peAlgorithm:
    //          Pointer to contain the algorithm.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT
    //      - resFE_INVALID_STATE
    //      - resS_OK
    //
    //  Description:
    //      Gets the algorithm for which this key is used.
    //
    //==EDOC====================================================================
    mxt_result GetAlgorithm(OUT EAlgorithm* peAlgorithm) const;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CPublicKey::CPublicKey()
: MXD_CRYPTO_PUBLICKEY_CLASSNAME()
{
}

inline
CPublicKey::CPublicKey(IN const CPublicKey* pKey)
: MXD_CRYPTO_PUBLICKEY_CLASSNAME(pKey)
{
}

inline
CPublicKey::CPublicKey(IN const CPublicKey& rKey)
: MXD_CRYPTO_PUBLICKEY_CLASSNAME(rKey)
{
}

inline
CPublicKey::~CPublicKey()
{
}

inline
CPublicKey& CPublicKey::operator=(IN const CPublicKey& rKey)
{
    MXD_CRYPTO_PUBLICKEY_CLASSNAME::operator=(rKey);
    return *this;
}

inline
bool CPublicKey::operator==(IN const CPublicKey& rKey) const
{
    return MXD_CRYPTO_PUBLICKEY_CLASSNAME::operator==(rKey);
}

inline
bool CPublicKey::operator!=(IN const CPublicKey& rKey) const
{
    return MXD_CRYPTO_PUBLICKEY_CLASSNAME::operator!=(rKey);
}

inline
mxt_result CPublicKey::Restore(IN const CBlob* pblobKey, IN EAlgorithm eAlgorithm)
{
    return MXD_CRYPTO_PUBLICKEY_CLASSNAME::Restore(pblobKey, eAlgorithm);
}

inline
mxt_result CPublicKey::RestoreDer(IN const CBlob* pblobKey, IN EAlgorithm eAlgorithm)
{
    return MXD_CRYPTO_PUBLICKEY_CLASSNAME::RestoreDer(pblobKey, eAlgorithm);
}

inline
mxt_result CPublicKey::RestorePem(IN const CBlob* pblobKey)
{
    return MXD_CRYPTO_PUBLICKEY_CLASSNAME::RestorePem(pblobKey);
}

inline
mxt_result CPublicKey::Store(OUT CBlob* pblobKey, IN EEncoding eEncoding) const
{
    return MXD_CRYPTO_PUBLICKEY_CLASSNAME::Store(pblobKey, eEncoding);
}

inline
mxt_result CPublicKey::StoreDer(OUT CBlob* pblobKey) const
{
    return MXD_CRYPTO_PUBLICKEY_CLASSNAME::StoreDer(pblobKey);
}

inline
mxt_result CPublicKey::StorePem(OUT CBlob* pblobKey) const
{
    return MXD_CRYPTO_PUBLICKEY_CLASSNAME::StorePem(pblobKey);
}

inline
mxt_result CPublicKey::GetAlgorithm(OUT EAlgorithm* peAlgorithm) const
{
    return MXD_CRYPTO_PUBLICKEY_CLASSNAME::GetAlgorithm(peAlgorithm);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_PUBLICKEY_NONE)

#endif // MXG_CPUBLICKEY_H

