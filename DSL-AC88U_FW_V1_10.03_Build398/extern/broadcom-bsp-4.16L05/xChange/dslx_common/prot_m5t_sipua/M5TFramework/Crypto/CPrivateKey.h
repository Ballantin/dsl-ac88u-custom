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
#ifndef MXG_CPRIVATEKEY_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CPRIVATEKEY_H
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
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_PRIVATEKEY_NONE
#endif

#if !defined(MXD_CRYPTO_PRIVATEKEY_NONE)

#if defined(MXD_CRYPTO_PRIVATEKEY_INCLUDE)
    #include MXD_CRYPTO_PRIVATEKEY_INCLUDE
#else
    #error "No implementation defined for CPrivateKey!!!"
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
class CBlob;
class IPassPhrase;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CPrivateKey
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages the private key for all algorithms.
//
// Description:
//   CPrivateKey is the class that manages the private key for all algorithms.
//   The private key is mostly used for authentication purposes.
//
// Location:
//   Crypto/CPrivateKey.h
//
// See Also:
//   CPublicKey
//
//==EDOC========================================================================
class CPrivateKey : public MXD_CRYPTO_PRIVATEKEY_CLASSNAME
{
//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //==SDOC====================================================================
    //==
    //==    CPrivateKey
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
    CPrivateKey();

    //==SDOC====================================================================
    //==
    //==    CPrivateKey
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
    CPrivateKey(IN const CPrivateKey* pKey);

    //==SDOC====================================================================
    //==
    //==    CPrivateKey
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
    CPrivateKey(IN const CPrivateKey& rKey);

    //==SDOC====================================================================
    //==
    //==    ~CPrivateKey
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
    virtual ~CPrivateKey();

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
    CPrivateKey& operator=(IN const CPrivateKey& rKey);

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
    bool operator==(IN const CPrivateKey& rKey) const;

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
    bool operator!=(IN const CPrivateKey& rKey) const;

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
    //      pszPassPhrase:
    //          Pointer to the passphrase to restore the key.
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
                       IN EAlgorithm eAlgorithm,
                       IN const char* pszPassPhrase);

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
    //      pPassPhrase:
    //          Passphrase interface used to parse the object.
    //
    //      opqPassPhraseParameter:
    //          Parameter to pass to the passphrase interface.
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
                       IN EAlgorithm eAlgorithm,
                       IN IPassPhrase* pPassPhrase,
                       IN mxt_opaque opqPassPhraseParameter);


    //==SDOC====================================================================
    //==
    //==    RestoreDer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Restores a private key for its serialized state DER format.
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
    //      Restores a private key for its serialized state PEM format.
    //
    //  Parameters:
    //      pblobKey:
    //          Pointer to the blob containing the key to restore.
    //
    //      pszPassPhrase:
    //          Passphrase used to serialize.
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
    mxt_result RestorePem(IN const CBlob* pblobKey, IN const char* pszPassPhrase);

    //==SDOC====================================================================
    //==
    //==    RestorePem
    //==
    //==========================================================================
    //
    //  Summary:
    //      Restores a private key for its serialized state PEM format.
    //
    //  Parameters:
    //      pblobKey:
    //          Pointer to the blob containing the key to restore.
    //
    //      pPassPhrase:
    //          Passphrase interface used to parse the object.
    //
    //      opqPassPhraseParameter:
    //          Parameter to pass to the passphrase interface.
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
    mxt_result RestorePem(IN const CBlob* pblobKey,
                          IN IPassPhrase* pPassPhrase,
                          IN mxt_opaque opqPassPhraseParameter);

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
    //      pszPassPhrase:
    //          Pointer to the passphrase to use to serialize the object.
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
    mxt_result Store(OUT CBlob* pblobKey,
                     IN EEncoding eEncoding,
                     IN const char* pszPassPhrase) const;

    //==SDOC====================================================================
    //==
    //==    StoreDer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Stores a private key to its serialized state DER format.
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
    //      Stores a private key to its serialized state PEM format.
    //
    //  Parameters:
    //      pblobKey:
    //          Pointer to the blob to contain the key.
    //
    //      pszPassPhrase:
    //          Passphrase used to serialize.
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
    mxt_result StorePem(OUT CBlob* pblobKey, IN const char* pszPassPhrase) const;

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
CPrivateKey::CPrivateKey()
: MXD_CRYPTO_PRIVATEKEY_CLASSNAME()
{
}

inline
CPrivateKey::CPrivateKey(IN const CPrivateKey* pKey)
: MXD_CRYPTO_PRIVATEKEY_CLASSNAME(pKey)
{
}

inline
CPrivateKey::CPrivateKey(IN const CPrivateKey& rKey)
: MXD_CRYPTO_PRIVATEKEY_CLASSNAME(rKey)
{
}

inline
CPrivateKey::~CPrivateKey()
{
}

inline
CPrivateKey& CPrivateKey::operator=(IN const CPrivateKey& rKey)
{
    MXD_CRYPTO_PRIVATEKEY_CLASSNAME::operator=(rKey);
    return *this;
}

inline
bool CPrivateKey::operator==(IN const CPrivateKey& rKey) const
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::operator==(rKey);
}

inline
bool CPrivateKey::operator!=(IN const CPrivateKey& rKey) const
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::operator!=(rKey);
}

inline
mxt_result CPrivateKey::Restore(IN const CBlob* pblobKey,
                                IN EAlgorithm eAlgorithm,
                                IN const char* pszPassPhrase)
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::Restore(pblobKey,
                                                    eAlgorithm,
                                                    NULL,
                                                    MX_VOIDPTR_TO_OPQ(const_cast<char*>(pszPassPhrase)));
}

inline
mxt_result CPrivateKey::Restore(IN const CBlob* pblobKey,
                                IN EAlgorithm eAlgorithm,
                                IN IPassPhrase* pPassPhrase,
                                IN mxt_opaque opqPassPhraseParameter)
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::Restore(pblobKey, eAlgorithm, pPassPhrase, opqPassPhraseParameter);
}

inline
mxt_result CPrivateKey::RestoreDer(IN const CBlob* pblobKey, IN EAlgorithm eAlgorithm)
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::RestoreDer(pblobKey, eAlgorithm);
}

inline
mxt_result CPrivateKey::RestorePem(IN const CBlob* pblobKey, IN const char* pszPassPhrase)
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::RestorePem(pblobKey, pszPassPhrase);
}

inline
mxt_result CPrivateKey::RestorePem(IN const CBlob* pblobKey,
                                   IN IPassPhrase* pPassPhrase,
                                   IN mxt_opaque opqPassPhraseParameter)
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::RestorePem(pblobKey, pPassPhrase, opqPassPhraseParameter);
}

inline
mxt_result CPrivateKey::Store(OUT CBlob* pblobKey,
                              IN EEncoding eEncoding,
                              IN const char* pszPassPhrase) const
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::Store(pblobKey, eEncoding, pszPassPhrase);
}

inline
mxt_result CPrivateKey::StoreDer(OUT CBlob* pblobKey) const
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::StoreDer(pblobKey);
}

inline
mxt_result CPrivateKey::StorePem(OUT CBlob* pblobKey, IN const char* pszPassPhrase) const
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::StorePem(pblobKey, pszPassPhrase);
}

inline
mxt_result CPrivateKey::GetAlgorithm(OUT EAlgorithm* peAlgorithm) const
{
    return MXD_CRYPTO_PRIVATEKEY_CLASSNAME::GetAlgorithm(peAlgorithm);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_PRIVATEKEY_NONE)

#endif // MXG_CPRIVATEKEY_H

