//==============================================================================
//==============================================================================
//
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_CCRYPTO_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCRYPTO_H

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
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif

#if defined(MXD_CRYPTO_USE_ENGINE_OPENSSL)
    #include "openssl/evp.h" // OpenSSL_add_all_algorithms
    #include "openssl/rand.h"
    #include "openssl/opensslv.h" // OpenSSL version

    #define MXD_CRYPTO_OPENSSL_VERSION OPENSSL_VERSION_NUMBER
    #define MXD_CRYPTO_OPENSSL_VERSION_0_9_8 0x00908000L // Use the version naming of OpenSSL.

#if defined(MXD_CRYPTO_AES_OPENSSL)
    #include "openssl/aes.h"
#endif

#if defined(MXD_CRYPTO_MD5_OPENSSL)
    #include "openssl/md5.h"
#endif

#if defined(MXD_CRYPTO_SHA1_OPENSSL) ||\
    defined(MXD_CRYPTO_SHA2_OPENSSL)
    #include "openssl/sha.h"
#endif

#if defined(MXD_CRYPTO_MD5MAC_OPENSSL) || \
    defined(MXD_CRYPTO_SHA1MAC_OPENSSL) || \
    defined(MXD_CRYPTO_SHA2MAC_OPENSSL)
    #include "openssl/hmac.h"
#endif

#if defined(MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL) && \
    defined(MXD_CRYPTO_RSA_OPENSSL)
    #include "openssl/err.h"
    #include "openssl/pem.h"
    #include "openssl/pkcs12.h"
    #include "openssl/x509v3.h"
#endif

#endif // #if defined(MXD_CRYPTO_USE_ENGINE_OPENSSL)

#if defined(MXD_CRYPTO_USE_ENGINE_MOCANA_SS)

    #include "common/mtypes.h"
    #include "common/merrors.h"

// There is compilation issues when using the Mocana SS library under VxWorks.
// They both define the MALLOC and FREE macros, but eventually either redefine
// them without checking if they are already defined, and even when the file
// dependencies work out and the macros do not get redefined in the same
// dependency chain, they get defined to functions with diffrent signatures.
//
// The workaround is to avoid the inclusion of mrtos.h when the Mocana memory
// debugger is not used and to define macros that will not generate a function
// signature clash.
#if defined (__ENABLE_MOCANA_DEBUG_MEMORY__)
    #include "common/mrtos.h"
    #define MOCANA_MALLOC(X) dbg_malloc(X, (ubyte *)__FILE__, __LINE__)
    #define MOCANA_FREE(X)   dbg_free((void *)(X), (ubyte *)__FILE__, __LINE__)

#else

    typedef void* RTOS_MUTEX;

    #define MOCANA_MALLOC malloc
    #define MOCANA_FREE free
#endif

extern "C"
{
    #include "crypto/hw_accel.h"
    #include "common/debug_console.h"
    #include "common/mocana.h"

    #include "common/vlong.h"
    #include "common/random.h"

    #include "crypto/rsa.h"

#if defined (MXD_TLS_MOCANA_SS)
    #include "common/vlong.h"
    #include "crypto/pubcrypto.h"
    #include "crypto/ca_mgmt.h"
#endif

#if defined(MXD_CRYPTO_AES_MOCANA_SS)
    #include "crypto/aes.h"
#endif

#if defined(MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS)
    #include "crypto/dh.h"
#endif
#if defined(MXD_CRYPTO_MD5_MOCANA_SS)
    #include "crypto/md5.h"
#endif
#if defined(MXD_CRYPTO_MD5MAC_MOCANA_SS)
    #include "crypto/sha1.h"
    #include "crypto/crypto.h"
    #include "crypto/hmac.h"
#endif

#if defined(MXD_CRYPTO_SHA1_MOCANA_SS)
    #include "crypto/sha1.h"
#endif

#if defined(MXD_CRYPTO_PRIVATEKEY_MOCANA_SS)
    #include "crypto/pkcs8.h"
#endif
#if defined(MXD_CRYPTO_PUBLICKEY_MOCANA_SS)
    #include "crypto/pkcs8.h"
#endif
} // extern "C"
#endif // #if defined (MXD_CRYPTO_USE_ENGINE_MOCANA_SS)

//-- Data Members
#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif // #ifndef MXG_CMUTEX_H

//M5T_INTERNAL_USE_END

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CCrypto
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Used to initialize the crypto library.
//
// Description:
//   CCrypto is a class that is only used to initialize the crypto library.
//
// Location:
//   Crypto/CCrypto.h
//
// See Also:
//
//==============================================================================
class CCrypto
{
// Published Interface.
public:
    // Summary:
    //  Returns the unique instance of CCrypto
    static CCrypto* Instance();

#if defined(MXD_CRYPTO_USE_ENGINE_MOCANA_SS)
    // Summary:
    //   Mocana SS tracing callback.
    static void MocanaLog(int nModule, int nLevel, int8_t* szMsg);
#endif

    // Summary:
    //  Enters the concurrency protection domain.
    void Enter();

    // Summary:
    //  Exits the concurrency protection domain.
    void Exit();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:

    // << Constructors / Destructors >>.
    CCrypto();
    virtual ~CCrypto();

private:

#if defined(MXD_CRYPTO_USE_ENGINE_OPENSSL)
    // Summary:
    //  This is an internal static callback that is called by the TLS
    //  stack to manage multi-threading.
    static unsigned long GetThreadId();

    // Summary:
    //  This is an internal static callback that is called by the TLS
    //  stack to manage mutexes.
    static void MutexHandler(int nMode,
                             int nMutex, // Zero based offset
                             const char* pszFile,
                             int nLine);
#endif // #if defined(MXD_CRYPTO_USE_ENGINE_OPENSSL)

    static mxt_result InitializeCCrypto();
    static void FinalizeCCrypto();

// Friends declarations.
    friend mxt_result InitializeCCrypto();
    friend void FinalizeCCrypto();

// Hidden Data Members.
protected:
private:
#if defined(MXD_CRYPTO_USE_ENGINE_OPENSSL)

    // Summary:
    //  Contains the number of mutexes required by the OpenSSL stack.
    unsigned int m_uMutexCount;

    // Summary:
    //  Contains the mutex that is used to synchronize multiple concurrent
    //  thread accesses to the OpenSSL stack. The size of the array is
    //  contained within m_uTlsMutexCount.
    CMutex* m_pmutex;
#endif // #if defined(MXD_CRYPTO_USE_ENGINE_OPENSSL)

#if !defined(MXD_OS_NONE)
    // Summary:
    //  Protection against multiple concurrent accesses to internal variables.
    CMutex m_mutexConcurrencyProtection;
#endif
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==   Enter
//==
//==============================================================================
//
//  Description:
//    Enters the concurrency protection domain. Note that this method reserves
//    the system wide crypto critical section. Exit from it quickly by using the
//    method Exit.
//
//  See also:
//      Exit
//
//==============================================================================
inline
void CCrypto::Enter()
{
#if !defined(MXD_OS_NONE)
    m_mutexConcurrencyProtection.Lock();
#endif
}

//==============================================================================
//==
//==   Exit
//==
//==============================================================================
//
//  Description:
//      Exits the concurrency protection domain.
//
//  See also:
//      Enter
//
//==============================================================================
inline
void CCrypto::Exit()
{
#if !defined(MXD_OS_NONE)
    m_mutexConcurrencyProtection.Unlock();
#endif
}

MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_CCRYPTO_H
