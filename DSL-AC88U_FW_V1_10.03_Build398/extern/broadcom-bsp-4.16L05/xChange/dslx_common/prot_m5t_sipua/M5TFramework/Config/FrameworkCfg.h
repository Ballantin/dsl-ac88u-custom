//==============================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_FRAMEWORKCFG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_FRAMEWORKCFG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================
//
// If the compiler complains that it can not include the file below, it may be
// because:
//
//     1 : You have not created this file to configure your application for
//         using this package. The documentation included with this package
//         explains how the configuration of the various M5T products works.
//         Please refer to this documentation and create "PreFrameworkCfg.h".
//
//     2 : You have created "PreFrameworkCfg.h" but it is not found by the
//         compiler. This file must be located in a directory found within the
//         include path defined for this build. Note that other M5T header
//         files are included by specifying the package name and filename
//         (e.g.: "SipUserAgent/IContext.h"), which permits setting the include
//         path to the "Source" directory of the package only, while
//         "PreFrameworkCfg.h" is included without any root directory.
//
//------------------------------------------------------------------------------
#include "PreFrameworkCfg.h"

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#if !defined(MXD_uCSTRING_BLOCK_LENGTH)
#define MXD_uCSTRING_BLOCK_LENGTH 32
#endif

#if !defined(MXD_SERVICING_THREAD_MAX_CONSECUTIVE_ITERATIONS)
#define MXD_SERVICING_THREAD_MAX_CONSECUTIVE_ITERATIONS 0
#endif

//M5T_INTERNAL_USE_BEGIN
#if defined (MXD_OPENSSL_FIPS_140_2_ENABLE_SUPPORT)
    // OpenSSL macro to enable FIPS mode.
    #define OPENSSL_FIPS
    #define MXD_INITIALIZER_PARAM_OPENSSL_ACTIVATE_FIPS_140_2_MODE "fips_140_2"
#endif

#if defined(MXD_TLS_OPENSSL)
    #define MXD_TLS_CTLS_INCLUDE "Tls/CTlsOpenSsl.h"
    #define MXD_TLS_CTLSSESSION_INCLUDE "Tls/CTlsSessionOpenSsl.h"
    #define MXD_TLS_CASYNCTLSSOCKET_INCLUDE "Tls/CAsyncTlsSocketOpenSsl.h"
    #define MXD_TLS_CASYNCTLSSERVERSOCKET_INCLUDE "Tls/CAsyncTlsServerSocketOpenSsl.h"

    #if defined(MXD_TLS_MOCANA_SS) || \
        defined(MXD_TLS_NONE)
        #define MXD_MULTIPLE_TLS
    #endif
#elif defined(MXD_TLS_MOCANA_SS)
    #define MXD_TLS_CTLS_INCLUDE "Tls/CTlsMocanaSs.h"
    #define MXD_TLS_CTLSSESSION_INCLUDE "Tls/CTlsSessionMocanaSs.h"
    #define MXD_TLS_CASYNCTLSSOCKET_INCLUDE "Tls/CAsyncTlsSocketMocanaSs.h"
    #define MXD_TLS_CASYNCTLSSERVERSOCKET_INCLUDE "Tls/CAsyncTlsServerSocketMocanaSs.h"

    #if defined (MXD_TLS_MOCANA_SS_INIT_STACK)
        #if !defined MXD_TLS_MOCANA_SS_MAX_CLIENT_CONNECTIONS
        #define MXD_TLS_MOCANA_SS_MAX_CLIENT_CONNECTIONS 10
        #endif

        #if !defined MXD_TLS_MOCANA_SS_MAX_SERVER_CONNECTIONS
        #define MXD_TLS_MOCANA_SS_MAX_SERVER_CONNECTIONS 1
        #endif
    #endif

    #if defined(MXD_TLS_OPENSSL) || \
        defined(MXD_TLS_NONE)
        #define MXD_MULTIPLE_TLS
    #endif
#else
    #define MXD_TLS_NONE
#endif

#if defined(MXD_MULTIPLE_TLS)
    #error "Only one of MXD_TLS_XXX must be defined."
#endif


#if defined(MXD_CRYPTO_ALL_NONE)
    #define MXD_CRYPTO_AES_NONE
    #define MXD_CRYPTO_DIFFIEHELLMAN_NONE
    #define MXD_CRYPTO_MD5_NONE
    #define MXD_CRYPTO_MD5MAC_NONE
    #define MXD_CRYPTO_RSA_NONE
    #define MXD_CRYPTO_SHA1_NONE
    #define MXD_CRYPTO_SHA1MAC_NONE
    #define MXD_CRYPTO_SHA2_NONE
    #define MXD_CRYPTO_SHA2MAC_NONE
    #define MXD_CRYPTO_SECUREPRNG_NONE
    #define MXD_CRYPTO_SECURESEED_NONE
    #define MXD_CRYPTO_PRIVATEKEY_NONE
    #define MXD_CRYPTO_PUBLICKEY_NONE
    #define MXD_CRYPTO_UUIDGENERATOR_NONE
    #define MXD_CRYPTO_BASE64_NONE

    #if defined(MXD_CRYPTO_ALL_MITOSFW) || \
        defined(MXD_CRYPTO_ALL_OPENSSL) || \
        defined(MXD_CRYPTO_ALL_MOCANA_SS) || \
        defined(MXD_CRYPTO_ALL_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_ALL
    #endif

#elif defined(MXD_CRYPTO_ALL_MITOSFW)
    #define MXD_CRYPTO_AES_MITOSFW
    #define MXD_CRYPTO_DIFFIEHELLMAN_NONE
    #define MXD_CRYPTO_MD5_MITOSFW
    #define MXD_CRYPTO_MD5MAC_MITOSFW
    #define MXD_CRYPTO_RSA_NONE
    #define MXD_CRYPTO_SHA1_MITOSFW
    #define MXD_CRYPTO_SHA1MAC_MITOSFW
    #define MXD_CRYPTO_SHA2_MITOSFW
    #define MXD_CRYPTO_SHA2MAC_MITOSFW
    #define MXD_CRYPTO_SECUREPRNG_MITOSFW
    #define MXD_CRYPTO_SECURESEED_MITOSFW
    #define MXD_CRYPTO_PRIVATEKEY_NONE
    #define MXD_CRYPTO_PUBLICKEY_NONE
    #define MXD_CRYPTO_UUIDGENERATOR_MITOSFW
    #define MXD_CRYPTO_BASE64_MITOSFW

    #if defined(MXD_CRYPTO_ALL_NONE) || \
        defined(MXD_CRYPTO_ALL_OPENSSL) || \
        defined(MXD_CRYPTO_ALL_MOCANA_SS) || \
        defined(MXD_CRYPTO_ALL_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_ALL
    #endif

#elif defined(MXD_CRYPTO_ALL_OPENSSL)
    #define MXD_CRYPTO_AES_OPENSSL
    #define MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL

// MD5 is not FIPS compliant (however, HMAC-MD5 is enabled through flags.
#if defined MXD_OPENSSL_FIPS_140_2_ENABLE_SUPPORT
    #define MXD_CRYPTO_MD5_MITOSFW
#else
    #define MXD_CRYPTO_MD5_OPENSSL
#endif

    #define MXD_CRYPTO_MD5MAC_OPENSSL
    #define MXD_CRYPTO_RSA_OPENSSL
    #define MXD_CRYPTO_SHA1_OPENSSL
    #define MXD_CRYPTO_SHA1MAC_OPENSSL
    #define MXD_CRYPTO_SHA2_OPENSSL
    #define MXD_CRYPTO_SHA2MAC_OPENSSL
    #define MXD_CRYPTO_PRIVATEKEY_OPENSSL
    #define MXD_CRYPTO_PUBLICKEY_OPENSSL
    // The MITOS secure seed is needed on OS which do not provide a random
    // source.
    #define MXD_CRYPTO_SECURESEED_MITOSFW
    #define MXD_CRYPTO_SECUREPRNG_OPENSSL
    #define MXD_CRYPTO_UUIDGENERATOR_MITOSFW
    #define MXD_CRYPTO_BASE64_MITOSFW

    #if defined(MXD_CRYPTO_ALL_NONE) || \
        defined(MXD_CRYPTO_ALL_MITOSFW) || \
        defined(MXD_CRYPTO_ALL_MOCANA_SS) || \
        defined(MXD_CRYPTO_ALL_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_ALL
    #endif

#elif defined (MXD_CRYPTO_ALL_MOCANA_SS)
    #define MXD_CRYPTO_AES_MOCANA_SS
    #define MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS
    #define MXD_CRYPTO_MD5_MITOSFW
    #define MXD_CRYPTO_MD5MAC_MITOFW
    #define MXD_CRYPTO_RSA_MOCANA_SS
    #define MXD_CRYPTO_SHA1_MOCANA_SS
    #define MXD_CRYPTO_SHA1MAC_MITOSFW
    #define MXD_CRYPTO_SHA2_MITOSFW
    #define MXD_CRYPTO_SHA2MAC_MITOSFW
    #define MXD_CRYPTO_SECUREPRNG_MOCANA_SS
    #define MXD_CRYPTO_SECURESEED_MOCANA_SS
    #define MXD_CRYPTO_PRIVATEKEY_MOCANA_SS
    #define MXD_CRYPTO_PUBLICKEY_MOCANA_SS
    #define MXD_CRYPTO_UUIDGENERATOR_MITOSFW

    #if defined(MXD_CRYPTO_ALL_NONE) || \
        defined(MXD_CRYPTO_ALL_MITOSFW) || \
        defined(MXD_CRYPTO_ALL_OPENSSL) || \
        defined(MXD_CRYPTO_ALL_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_ALL
   #endif

#elif defined(MXD_CRYPTO_ALL_OVERRIDE)
    #define MXD_CRYPTO_AES_OVERRIDE
    #define MXD_CRYPTO_DIFFIEHELLMAN_OVERRIDE
    #define MXD_CRYPTO_MD5_OVERRIDE
    #define MXD_CRYPTO_MD5MAC_OVERRIDE
    #define MXD_CRYPTO_RSA_OVERRIDE
    #define MXD_CRYPTO_SHA1_OVERRIDE
    #define MXD_CRYPTO_SHA1MAC_OVERRIDE
    #define MXD_CRYPTO_SHA2_OVERRIDE
    #define MXD_CRYPTO_SHA2MAC_OVERRIDE
    #define MXD_CRYPTO_SECUREPRNG_OVERRIDE
    #define MXD_CRYPTO_SECURESEED_OVERRIDE
    #define MXD_CRYPTO_PRIVATEKEY_OVERRIDE
    #define MXD_CRYPTO_PUBLICKEY_OVERRIDE
    #define MXD_CRYPTO_UUIDGENERATOR_OVERRIDE

    #if defined(MXD_CRYPTO_ALL_NONE) || \
        defined(MXD_CRYPTO_ALL_MITOSFW) || \
        defined(MXD_CRYPTO_ALL_MOCANA_SS) || \
        defined(MXD_CRYPTO_ALL_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_ALL
    #endif
#endif

#if defined(MXD_MULTIPLE_CRYPTO_ALL)
    #error "Only one of MXD_CRYPTO_ALL_XXX must be defined."
#endif

#if defined(MXD_CRYPTO_AES_NONE)
    #if defined(MXD_CRYPTO_AES_MITOSFW) || \
        defined(MXD_CRYPTO_AES_OPENSSL) || \
        defined(MXD_CRYPTO_AES_MOCANA_SS) || \
        defined(MXD_CRYPTO_AES_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_AES
    #endif

#elif defined(MXD_CRYPTO_AES_MITOSFW)
    #if defined(MXD_CRYPTO_AES_NONE) || \
        defined(MXD_CRYPTO_AES_OPENSSL) || \
        defined(MXD_CRYPTO_AES_MOCANA_SS) || \
        defined(MXD_CRYPTO_AES_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_AES
    #endif

    #define MXD_CRYPTO_AES_INCLUDE "Crypto/CAesMitosFw.h"

#elif defined(MXD_CRYPTO_AES_OPENSSL)
    #if defined(MXD_CRYPTO_AES_NONE) || \
        defined(MXD_CRYPTO_AES_MITOSFW) || \
        defined(MXD_CRYPTO_AES_MOCANA_SS) || \
        defined(MXD_CRYPTO_AES_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_EAS
    #endif

    #define MXD_CRYPTO_AES_INCLUDE "Crypto/CAesMitosFw.h"

#elif defined(MXD_CRYPTO_AES_MOCANA_SS)
    #if defined(MXD_CRYPTO_AES_NONE) || \
        defined(MXD_CRYPTO_AES_MITOSFW)|| \
        defined(MXD_CRYPTO_AES_OVERRIDE)|| \
        defined(MXD_CRYPTO_AES_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_AES
    #endif

    #define MXD_CRYPTO_AES_INCLUDE "Crypto/CAesMitosFw.h"

#elif defined(MXD_CRYPTO_AES_OVERRIDE)
    #if defined(MXD_CRYPTO_AES_NONE) || \
        defined(MXD_CRYPTO_AES_MITOSFW)|| \
        defined(MXD_CRYPTO_AES_OPENSSL)|| \
        defined(MXD_CRYPTO_AES_MOCANA_SS)
        #define MXD_MULTIPLE_CRYPTO_AES
    #endif

    #if !defined(MXD_CRYPTO_AES_INCLUDE)
        #error "MXD_CRYPTO_AES_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_AES_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_AES)
    #error "Only one of MXD_CRYPTO_AES_XXX must be defined."
#endif

#if defined(MXD_CRYPTO_DIFFIEHELLMAN_NONE)
    #if defined(MXD_CRYPTO_DIFFIEHELLMAN_MITOSFW) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_DIFFIEHELLMAN
    #endif

#elif defined(MXD_CRYPTO_DIFFIEHELLMAN_MITOSFW)
    #if defined(MXD_CRYPTO_DIFFIEHELLMAN_NONE) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_DIFFIEHELLMAN
    #endif

    #error "No MitosFw implementation exists for DiffieHellman!"

#elif defined(MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL)
    #if defined(MXD_CRYPTO_DIFFIEHELLMAN_NONE) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_MITOSFW) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_DIFFIEHELLMAN
    #endif

    #define MXD_CRYPTO_DIFFIEHELLMAN_INCLUDE "Crypto/CDiffieHellmanOpenSsl.h"


#elif defined(MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS)
    #if defined(MXD_CRYPTO_DIFFIEHELLMAN_NONE) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_MITOSFW) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_OVERRIDE) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_DIFFIEHELLMAN
    #endif

    #define MXD_CRYPTO_DIFFIEHELLMAN_INCLUDE "Crypto/CDiffieHellmanMocanaSs.h"

#elif defined(MXD_CRYPTO_DIFFIEHELLMAN_OVERRIDE)
    #if defined(MXD_CRYPTO_DIFFIEHELLMAN_NONE) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_MITOSFW) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL) || \
        defined(MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS)
        #define MXD_MULTIPLE_CRYPTO_DIFFIEHELLMAN
    #endif

    #if !defined(MXD_CRYPTO_DIFFIEHELLMAN_INCLUDE)
        #error "MXD_CRYPTO_DIFFIEHELLMAN_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_DIFFIEHELLMAN_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_DIFFIEHELLMAN)
    #error "Only one of MXD_CRYPTO_DIFFIEHELLMAN_XXX must be defined."
#endif

#if defined(MXD_CRYPTO_MD5_NONE)
    #if defined(MXD_CRYPTO_MD5_MITOSFW) || \
        defined(MXD_CRYPTO_MD5_OPENSSL) || \
        defined(MXD_CRYPTO_MD5_MOCANA_SS) || \
        defined(MXD_CRYPTO_MD5_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_MD5
    #endif

#elif defined(MXD_CRYPTO_MD5_MITOSFW)
    #if defined(MXD_CRYPTO_MD5_NONE) || \
        defined(MXD_CRYPTO_MD5_OPENSSL) || \
        defined(MXD_CRYPTO_MD5_MOCANA_SS) || \
        defined(MXD_CRYPTO_MD5_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_MD5
    #endif

    #define MXD_CRYPTO_MD5_INCLUDE "Crypto/CMd5MitosFw.h"

#elif defined(MXD_CRYPTO_MD5_OPENSSL)
    #if defined(MXD_CRYPTO_MD5_NONE) || \
        defined(MXD_CRYPTO_MD5_MITOSFW) || \
        defined(MXD_CRYPTO_MD5_MOCANA_SS) || \
        defined(MXD_CRYPTO_MD5_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_MD5
    #endif

    #define MXD_CRYPTO_MD5_INCLUDE "Crypto/CMd5OpenSsl.h"

#elif defined(MXD_CRYPTO_MD5_MOCANA_SS)
    #if defined(MXD_CRYPTO_MD5_NONE) || \
        defined(MXD_CRYPTO_MD5_MITOSFW) || \
        defined(MXD_CRYPTO_MD5_OPENSSL) || \
        defined(MXD_CRYPTO_MD5_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_MD5
    #endif

    #error "No Mocana SS implementation exists for MD5!"

#elif defined(MXD_CRYPTO_MD5_OVERRIDE)
    #if defined(MXD_CRYPTO_MD5_NONE) || \
        defined(MXD_CRYPTO_MD5_MITOSFW) || \
        defined(MXD_CRYPTO_MD5_OPENSSL)|| \
        defined(MXD_CRYPTO_MD5_MOCANA_SS)
        #define MXD_MULTIPLE_CRYPTO_MD5
    #endif

    #if !defined(MXD_CRYPTO_MD5_INCLUDE)
        #error "MXD_CRYPTO_MD5_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_MD5_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_MD5)
    #error "Only one of MXD_CRYPTO_MD5_XXX must be defined."
#endif

#if defined(MXD_CRYPTO_MD5MAC_NONE)
    #if defined(MXD_CRYPTO_MD5MAC_MITOSFW) || \
        defined(MXD_CRYPTO_MD5MAC_OPENSSL) || \
        defined(MXD_CRYPTO_MD5MAC_MOCANA_SS) || \
        defined(MXD_CRYPTO_MD5MAC_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_MD5MAC
    #endif

#elif defined(MXD_CRYPTO_MD5MAC_MITOSFW)
    #if defined(MXD_CRYPTO_MD5MAC_NONE) || \
        defined(MXD_CRYPTO_MD5MAC_OPENSSL) || \
        defined(MXD_CRYPTO_MD5MAC_MOCANA_SS) || \
        defined(MXD_CRYPTO_MD5MAC_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_MD5MAC
    #endif

    #define MXD_CRYPTO_MD5MAC_INCLUDE "Crypto/CMd5MacMitosFw.h"

#elif defined(MXD_CRYPTO_MD5MAC_OPENSSL)
    #if defined(MXD_CRYPTO_MD5MAC_NONE) || \
        defined(MXD_CRYPTO_MD5MAC_MITOSFW) || \
        defined(MXD_CRYPTO_MD5MAC_MOCANA_SS) || \
        defined(MXD_CRYPTO_MD5MAC_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_MD5MAC
    #endif

    #define MXD_CRYPTO_MD5MAC_INCLUDE "Crypto/CMd5MacOpenSsl.h"

#elif defined(MXD_CRYPTO_MD5MAC_MOCANA_SS)
    #if defined(MXD_CRYPTO_MD5MAC_NONE) || \
        defined(MXD_CRYPTO_MD5MAC_MITOSFW) || \
        defined(MXD_CRYPTO_MD5MAC_OVERRIDE)|| \
        defined(MXD_CRYPTO_MD5MAC_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_MD5MAC
    #endif

    #error "No Mocana SS implementation exists for MD5 MAC!"

#elif defined(MXD_CRYPTO_MD5MAC_OVERRIDE)
    #if defined(MXD_CRYPTO_MD5MAC_NONE) || \
        defined(MXD_CRYPTO_MD5MAC_MITOSFW) || \
        defined(MXD_CRYPTO_MD5MAC_MOCANA_SS) || \
        defined(MXD_CRYPTO_MD5MAC_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_MD5MAC
    #endif

    #if !defined(MXD_CRYPTO_MD5MAC_INCLUDE)
        #error "MXD_CRYPTO_MD5MAC_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_MD5MAC_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_MD5MAC)
    #error "Only one of MXD_CRYPTO_MD5MAC_XXX must be defined."
#endif

#if !defined(MXD_CRYPTO_MD5MAC_NONE) && \
    defined(MXD_CRYPTO_MD5_NONE)
    #error "A MD5 implementation must be specified for MD5 MAC!"
#endif

#if defined(MXD_CRYPTO_RSA_NONE)
    #if defined(MXD_CRYPTO_RSA_MITOSFW) || \
        defined(MXD_CRYPTO_RSA_OPENSSL) || \
        defined(MXD_CRYPTO_RSA_MOCANA_SS) || \
        defined(MXD_CRYPTO_RSA_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_RSA
    #endif

#elif defined(MXD_CRYPTO_RSA_MITOSFW)
    #if defined(MXD_CRYPTO_RSA_NONE) || \
        defined(MXD_CRYPTO_RSA_OPENSSL) || \
        defined(MXD_CRYPTO_RSA_MOCANA_SS)
        defined(MXD_CRYPTO_RSA_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_RSA
    #endif

    #error "No MitosFw implementation exists for RSA!"

#elif defined(MXD_CRYPTO_RSA_OPENSSL)
    #if defined(MXD_CRYPTO_RSA_NONE) || \
        defined(MXD_CRYPTO_RSA_MITOSFW) || \
        defined(MXD_CRYPTO_RSA_MOCANA_SS) || \
        defined(MXD_CRYPTO_RSA_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_RSA
    #endif

    #define MXD_CRYPTO_RSA_INCLUDE "Crypto/CRsaOpenSsl.h"

#elif defined(MXD_CRYPTO_RSA_MOCANA_SS)
    #if defined(MXD_CRYPTO_RSA_NONE) || \
        defined(MXD_CRYPTO_RSA_MITOSFW) || \
        defined(MXD_CRYPTO_RSA_OVERRIDE)|| \
        defined(MXD_CRYPTO_RSA_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_RSA
    #endif

    #define MXD_CRYPTO_RSA_INCLUDE "Crypto/CRsaMocanaSs.h"

#elif defined(MXD_CRYPTO_RSA_OVERRIDE)
    #if defined(MXD_CRYPTO_RSA_NONE) || \
        defined(MXD_CRYPTO_RSA_MITOSFW) || \
        defined(MXD_CRYPTO_RSA_OPENSSL)|| \
        defined(MXD_CRYPTO_RSA_MOCANA_SS)
        #define MXD_MULTIPLE_CRYPTO_RSA
    #endif

    #if !defined(MXD_CRYPTO_RSA_INCLUDE)
        #error "MXD_CRYPTO_RSA_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_RSA_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_RSA)
    #error "Only one of MXD_CRYPTO_RSA_XXX must be defined."
#endif

#if defined(MXD_CRYPTO_SHA1_NONE)
    #if defined(MXD_CRYPTO_SHA1_MITOSFW) || \
        defined(MXD_CRYPTO_SHA1_OPENSSL) || \
        defined(MXD_CRYPTO_SHA1_MOCANA_SS) || \
        defined(MXD_CRYPTO_SHA1_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA1
    #endif

#elif defined(MXD_CRYPTO_SHA1_MITOSFW)
    #if defined(MXD_CRYPTO_SHA1_NONE) || \
        defined(MXD_CRYPTO_SHA1_OPENSSL) || \
        defined(MXD_CRYPTO_SHA1_MOCANA_SS) || \
        defined(MXD_CRYPTO_SHA1_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA1
    #endif

    #define MXD_CRYPTO_SHA1_INCLUDE "Crypto/CSha1MitosFw.h"

#elif defined(MXD_CRYPTO_SHA1_OPENSSL)
    #if defined(MXD_CRYPTO_SHA1_NONE) || \
        defined(MXD_CRYPTO_SHA1_MITOSFW) || \
        defined(MXD_CRYPTO_SHA1_MOCANA_SS) || \
        defined(MXD_CRYPTO_SHA1_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA1
    #endif

    #define MXD_CRYPTO_SHA1_INCLUDE "Crypto/CSha1OpenSsl.h"

#elif defined(MXD_CRYPTO_SHA1_MOCANA_SS)
    #if defined(MXD_CRYPTO_SHA1_NONE) || \
        defined(MXD_CRYPTO_SHA1_MITOSFW) || \
        defined(MXD_CRYPTO_SHA1_OVERRIDE)|| \
        defined(MXD_CRYPTO_SHA1_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_SHA1
    #endif

    #define MXD_CRYPTO_SHA1_INCLUDE "Crypto/CSha1MocanaSs.h"

#elif defined(MXD_CRYPTO_SHA1_OVERRIDE)
    #if defined(MXD_CRYPTO_SHA1_NONE) || \
        defined(MXD_CRYPTO_SHA1_MITOSFW) || \
        defined(MXD_CRYPTO_SHA1_OPENSSL)|| \
        defined(MXD_CRYPTO_SHA1_MOCANA_SS)
        #define MXD_MULTIPLE_CRYPTO_SHA1
    #endif

    #if !defined(MXD_CRYPTO_SHA1_INCLUDE)
        #error "MXD_CRYPTO_SHA1_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_SHA1_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_SHA1)
    #error "Only one of MXD_CRYPTO_SHA1_XXX must be defined."
#endif

#if defined(MXD_CRYPTO_SHA1MAC_NONE)
    #if defined(MXD_CRYPTO_SHA1MAC_MITOSFW) || \
        defined(MXD_CRYPTO_SHA1MAC_OPENSSL) || \
        defined(MXD_CRYPTO_SHA1MAC_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA1MAC
    #endif

#elif defined(MXD_CRYPTO_SHA1MAC_MITOSFW)
    #if defined(MXD_CRYPTO_SHA1MAC_NONE) || \
        defined(MXD_CRYPTO_SHA1MAC_OPENSSL) || \
        defined(MXD_CRYPTO_SHA1MAC_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA1MAC
    #endif

    #define MXD_CRYPTO_SHA1MAC_INCLUDE "Crypto/CSha1MacMitosFw.h"

#elif defined(MXD_CRYPTO_SHA1MAC_OPENSSL)
    #if defined(MXD_CRYPTO_SHA1MAC_NONE) || \
        defined(MXD_CRYPTO_SHA1MAC_MITOSFW) || \
        defined(MXD_CRYPTO_SHA1MAC_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA1MAC
    #endif

    #define MXD_CRYPTO_SHA1MAC_INCLUDE "Crypto/CSha1MacOpenSsl.h"

#elif defined(MXD_CRYPTO_SHA1MAC_OVERRIDE)
    #if defined(MXD_CRYPTO_SHA1MAC_NONE) || \
        defined(MXD_CRYPTO_SHA1MAC_MITOSFW) || \
        defined(MXD_CRYPTO_SHA1MAC_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_SHA1MAC
    #endif

    #if !defined(MXD_CRYPTO_SHA1MAC_INCLUDE)
        #error "MXD_CRYPTO_SHA1MAC_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_SHA1MAC_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_SHA1MAC)
    #error "Only one of MXD_CRYPTO_SHA1MAC_XXX must be defined."
#endif

#if !defined(MXD_CRYPTO_SHA1MAC_NONE) && \
    defined(MXD_CRYPTO_SHA1_NONE)
    #error "A SHA-1 implementation must be specified for SHA-1 MAC!"
#endif

#if defined(MXD_CRYPTO_SHA2_NONE)
    #if defined(MXD_CRYPTO_SHA2_MITOSFW) || \
        defined(MXD_CRYPTO_SHA2_OPENSSL) || \
        defined(MXD_CRYPTO_SHA2_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA2
    #endif

#elif defined(MXD_CRYPTO_SHA2_MITOSFW)
    #if defined(MXD_CRYPTO_SHA2_NONE) || \
        defined(MXD_CRYPTO_SHA2_OPENSSL) || \
        defined(MXD_CRYPTO_SHA2_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA2
    #endif

    #define MXD_CRYPTO_SHA2_INCLUDE "Crypto/CSha2MitosFw.h"

#elif defined(MXD_CRYPTO_SHA2_OPENSSL)
    #if defined(MXD_CRYPTO_SHA2_NONE) || \
        defined(MXD_CRYPTO_SHA2_MITOSFW) || \
        defined(MXD_CRYPTO_SHA2_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA2
    #endif

    #define MXD_CRYPTO_SHA2_INCLUDE "Crypto/CSha2OpenSsl.h"

#elif defined(MXD_CRYPTO_SHA2_OVERRIDE)
    #if defined(MXD_CRYPTO_SHA2_NONE) || \
        defined(MXD_CRYPTO_SHA2_MITOSFW) || \
        defined(MXD_CRYPTO_SHA2_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_SHA2
    #endif

    #if !defined(MXD_CRYPTO_SHA2_INCLUDE)
        #error "MXD_CRYPTO_SHA2_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_SHA2_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_SHA2)
    #error "Only one of MXD_CRYPTO_SHA2_XXX must be defined."
#endif

#if defined(MXD_CRYPTO_SHA2MAC_NONE)
    #if defined(MXD_CRYPTO_SHA2MAC_MITOSFW) || \
        defined(MXD_CRYPTO_SHA2MAC_OPENSSL) || \
        defined(MXD_CRYPTO_SHA2MAC_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA2MAC
    #endif

#elif defined(MXD_CRYPTO_SHA2MAC_MITOSFW)
    #if defined(MXD_CRYPTO_SHA2MAC_NONE) || \
        defined(MXD_CRYPTO_SHA2MAC_OPENSSL) || \
        defined(MXD_CRYPTO_SHA2MAC_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA2MAC
    #endif

    #define MXD_CRYPTO_SHA2MAC_INCLUDE "Crypto/CSha2MacMitosFw.h"

#elif defined(MXD_CRYPTO_SHA2MAC_OPENSSL)
    #if defined(MXD_CRYPTO_SHA2MAC_NONE) || \
        defined(MXD_CRYPTO_SHA2MAC_MITOSFW) || \
        defined(MXD_CRYPTO_SHA2MAC_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SHA2MAC
    #endif

    #define MXD_CRYPTO_SHA2MAC_INCLUDE "Crypto/CSha2MacOpenSsl.h"

#elif defined(MXD_CRYPTO_SHA2MAC_OVERRIDE)
    #if defined(MXD_CRYPTO_SHA2MAC_NONE) || \
        defined(MXD_CRYPTO_SHA2MAC_MITOSFW) ||\
        defined(MXD_CRYPTO_SHA2MAC_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_SHA2MAC
    #endif

    #if !defined(MXD_CRYPTO_SHA2MAC_INCLUDE)
        #error "MXD_CRYPTO_SHA2MAC_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_SHA2MAC_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_SHA2MAC)
    #error "Only one of MXD_CRYPTO_SHA2MAC_XXX must be defined."
#endif

#if !defined(MXD_CRYPTO_SHA2MAC_NONE) && \
    defined(MXD_CRYPTO_SHA2_NONE)
    #error "A SHA-2 implementation must be specified for HMAC SHA-2!"
#endif

#if defined(MXD_CRYPTO_SECUREPRNG_NONE)
    #if defined(MXD_CRYPTO_SECUREPRNG_MITOSFW) || \
        defined(MXD_CRYPTO_SECUREPRNG_OPENSSL) || \
        defined(MXD_CRYPTO_SECUREPRNG_MOCANA_SS) || \
        defined(MXD_CRYPTO_SECUREPRNG_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SECUREPRNG
    #endif

#elif defined(MXD_CRYPTO_SECUREPRNG_MITOSFW)
    #if defined(MXD_CRYPTO_SECUREPRNG_NONE) || \
        defined(MXD_CRYPTO_SECUREPRNG_OPENSSL) || \
        defined(MXD_CRYPTO_SECUREPRNG_MOCANA_SS) || \
        defined(MXD_CRYPTO_SECUREPRNG_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SECUREPRNG
    #endif

    #define MXD_CRYPTO_SECUREPRNG_INCLUDE "Crypto/CSecurePrngMitosFw.h"

#elif defined(MXD_CRYPTO_SECUREPRNG_OPENSSL)
    #if defined(MXD_CRYPTO_SECUREPRNG_NONE) || \
        defined(MXD_CRYPTO_SECUREPRNG_MITOSFW) || \
        defined(MXD_CRYPTO_SECUREPRNG_MOCANA_SS) || \
        defined(MXD_CRYPTO_SECUREPRNG_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SECUREPRNG
    #endif

    #define MXD_CRYPTO_SECUREPRNG_INCLUDE "Crypto/CSecurePrngOpenSsl.h"

#elif defined(MXD_CRYPTO_SECUREPRNG_MOCANA_SS)
    #if defined(MXD_CRYPTO_SECUREPRNG_NONE) || \
        defined(MXD_CRYPTO_SECUREPRNG_MITOSFW)|| \
        defined(MXD_CRYPTO_SECUREPRNG_OPENSSL) || \
        defined(MXD_CRYPTO_SECUREPRNG_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SECUREPRNG
    #endif

    #define MXD_CRYPTO_SECUREPRNG_INCLUDE "Crypto/CSecurePrngMocanaSs.h"

#elif defined(MXD_CRYPTO_SECUREPRNG_OVERRIDE)
    #if defined(MXD_CRYPTO_SECUREPRNG_NONE) || \
        defined(MXD_CRYPTO_SECUREPRNG_MITOSFW) || \
        defined(MXD_CRYPTO_SECUREPRNG_OPENSSL) || \
        defined(MXD_CRYPTO_SECUREPRNG_MOCANA_SS)
        #define MXD_MULTIPLE_CRYPTO_SECUREPRNG
    #endif

    #if !defined(MXD_CRYPTO_SECUREPRNG_INCLUDE)
        #error "MXD_CRYPTO_SECUREPRNG_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_SECUREPRNG_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_SECUREPRNG)
    #error "Only one of MXD_CRYPTO_SECUREPRNG_XXX must be defined."
#endif

#if defined(MXD_CRYPTO_SECUREPRNG_MITOSFW) && defined(MXD_CRYPTO_SHA1_NONE)
    #error "A SHA-1 implementation must be specified for the MitosFw implementation of SecurePrng!"
#endif

#if defined(MXD_CRYPTO_SECUREPRNG_OPENSSL) && defined(MXD_CRYPTO_SHA1_NONE)
    #error "A SHA-1 implementation must be specified for the OpenSSL implementation of SecurePrng!"
#endif

#if defined(MXD_CRYPTO_SECURESEED_NONE)
    #if defined(MXD_CRYPTO_SECURESEED_MITOSFW) || \
        defined(MXD_CRYPTO_SECURESEED_MOCANA_SS) || \
        defined(MXD_CRYPTO_SECURESEED_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SECURESEED
    #endif

#elif defined(MXD_CRYPTO_SECURESEED_MITOSFW)
    #if defined(MXD_CRYPTO_SECURESEED_NONE) || \
        defined(MXD_CRYPTO_SECURESEED_MOCANA_SS) || \
        defined(MXD_CRYPTO_SECURESEED_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SECURESEED
    #endif

    #define MXD_CRYPTO_SECURESEED_INCLUDE "Crypto/CSecureSeedMitosFw.h"

#elif defined(MXD_CRYPTO_SECURESEED_OPENSSL)
    #error "No OpenSSL implementation exists for SecureSeed!"

#elif defined(MXD_CRYPTO_SECURESEED_OVERRIDE)
    #if defined(MXD_CRYPTO_SECURESEED_NONE) || \
        defined(MXD_CRYPTO_SECURESEED_MITOSFW) || \
        defined(MXD_CRYPTO_SECURESEED_MOCANA_SS)
        #define MXD_MULTIPLE_CRYPTO_SECURESEED
    #endif

    #if !defined(MXD_CRYPTO_SECURESEED_INCLUDE)
        #error "MXD_CRYPTO_SECURESEED_INCLUDE not defined!"
    #endif

#elif defined(MXD_CRYPTO_SECURESEED_MOCANA_SS)
    #if defined(MXD_CRYPTO_SECURESEED_NONE) || \
        defined(MXD_CRYPTO_SECURESEED_MITOSFW)|| \
        defined(MXD_CRYPTO_SECURESEED_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_SECURESEED
    #endif

    #define MXD_CRYPTO_SECURESEED_INCLUDE "Crypto/CSecureSeedMocanaSs.h"

#else
    #define MXD_CRYPTO_SECURESEED_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_SECURESEED)
    #error "Only one of MXD_CRYPTO_SECURESEED_XXX must be defined."
#endif

#if defined(MXD_CRYPTO_SECURESEED_MITOSFW) && defined(MXD_CRYPTO_SHA1_NONE)
    #error "A SHA-1 implementation must be specified for the MitosFw implementation of SecureSeed!"
#endif

#if defined(MXD_CRYPTO_PRIVATEKEY_NONE)
    #if defined(MXD_CRYPTO_PRIVATEKEY_MITOSFW) || \
        defined(MXD_CRYPTO_PRIVATEKEY_OPENSSL) || \
        defined(MXD_CRYPTO_PRIVATEKEY_MOCANA_SS) || \
        defined(MXD_CRYPTO_PRIVATEKEY_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_PRIVATEKEY
    #endif

#elif defined(MXD_CRYPTO_PRIVATEKEY_MITOSFW)
    #if defined(MXD_CRYPTO_PRIVATEKEY_NONE) || \
        defined(MXD_CRYPTO_PRIVATEKEY_OPENSSL) || \
        defined(MXD_CRYPTO_PRIVATEKEY_MOCANA_SS) || \
        defined(MXD_CRYPTO_PRIVATEKEY_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_PRIVATEKEY
    #endif

    #error "No MitosFw implementation exists for CPrivateKey!"

#elif defined(MXD_CRYPTO_PRIVATEKEY_OPENSSL)
    #if defined(MXD_CRYPTO_PRIVATEKEY_NONE) || \
        defined(MXD_CRYPTO_PRIVATEKEY_MITOSFW) || \
        defined(MXD_CRYPTO_PRIVATEKEY_MOCANA_SS) || \
        defined(MXD_CRYPTO_PRIVATEKEY_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_PRIVATEKEY
    #endif

    #define MXD_CRYPTO_PRIVATEKEY_INCLUDE "Crypto/CPrivateKeyOpenSsl.h"

#elif defined(MXD_CRYPTO_PRIVATEKEY_MOCANA_SS)
    #if defined(MXD_CRYPTO_PRIVATEKEY_NONE) || \
        defined(MXD_CRYPTO_PRIVATEKEY_MITOSFW) || \
        defined(MXD_CRYPTO_PRIVATEKEY_OVERRIDE)|| \
        defined(MXD_CRYPTO_PRIVATEKEY_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_PRIVATEKEY
    #endif

    #define MXD_CRYPTO_PRIVATEKEY_INCLUDE "Crypto/CPrivateKeyMocanaSs.h"

#elif defined(MXD_CRYPTO_PRIVATEKEY_OVERRIDE)
    #if defined(MXD_CRYPTO_PRIVATEKEY_NONE) || \
        defined(MXD_CRYPTO_PRIVATEKEY_MITOSFW) || \
        defined(MXD_CRYPTO_PRIVATEKEY_MOCANA_SS) || \
        defined(MXD_CRYPTO_PRIVATEKEY_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_PRIVATEKEY
    #endif

    #if !defined(MXD_CRYPTO_PRIVATEKEY_INCLUDE)
        #error "MXD_CRYPTO_PRIVATEKEY_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_PRIVATEKEY_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_PRIVATEKEY)
    #error "Only one of MXD_CRYPTO_PRIVATEKEY_XXX must be defined."
#endif

#if defined(MXD_CRYPTO_PUBLICKEY_NONE)
    #if defined(MXD_CRYPTO_PUBLICKEY_MITOSFW) || \
        defined(MXD_CRYPTO_PUBLICKEY_OPENSSL) || \
        defined(MXD_CRYPTO_PUBLICKEY_MOCANA_SS) || \
        defined(MXD_CRYPTO_PUBLICKEY_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_PUBLICKEY
    #endif

#elif defined(MXD_CRYPTO_PUBLICKEY_MITOSFW)
    #if defined(MXD_CRYPTO_PUBLICKEY_NONE) || \
        defined(MXD_CRYPTO_PUBLICKEY_OPENSSL) || \
        defined(MXD_CRYPTO_PUBLICKEY_MOCANA_SS) || \
        defined(MXD_CRYPTO_PUBLICKEY_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_PUBLICKEY
    #endif

    #error "No MitosFw implementation exists for CPublicKey!"

#elif defined(MXD_CRYPTO_PUBLICKEY_OPENSSL)
    #if defined(MXD_CRYPTO_PUBLICKEY_NONE) || \
        defined(MXD_CRYPTO_PUBLICKEY_MITOSFW) || \
        defined(MXD_CRYPTO_PUBLICKEY_MOCANA_SS) || \
        defined(MXD_CRYPTO_PUBLICKEY_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_PUBLICKEY
    #endif

    #define MXD_CRYPTO_PUBLICKEY_INCLUDE "Crypto/CPublicKeyOpenSsl.h"

#elif defined(MXD_CRYPTO_PUBLICKEY_MOCANA_SS)
    #if defined(MXD_CRYPTO_PUBLICKEY_NONE) || \
        defined(MXD_CRYPTO_PUBLICKEY_MITOSFW) || \
        defined(MXD_CRYPTO_PUBLICKEY_OVERRIDE)|| \
        defined(MXD_CRYPTO_PUBLICKEY_OPENSSL)
        #define MXD_MULTIPLE_CRYPTO_PUBLICKEY
    #endif

    #define MXD_CRYPTO_PUBLICKEY_INCLUDE "Crypto/CPublicKeyMocanaSs.h"

#elif defined(MXD_CRYPTO_PUBLICKEY_OVERRIDE)
    #if defined(MXD_CRYPTO_PUBLICKEY_NONE) || \
        defined(MXD_CRYPTO_PUBLICKEY_MITOSFW) || \
        defined(MXD_CRYPTO_PUBLICKEY_OPENSSL)|| \
        defined(MXD_CRYPTO_PUBLICKEY_MOCANA_SS)
        #define MXD_MULTIPLE_CRYPTO_PUBLICKEY
    #endif

    #if !defined(MXD_CRYPTO_PUBLICKEY_INCLUDE)
        #error "MXD_CRYPTO_PUBLICKEY_INCLUDE not defined!"
    #endif

#else
    #define MXD_CRYPTO_PUBLICKEY_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_PUBLICKEY)
    #error "Only one of MXD_CRYPTO_PUBLICKEY_XXX must be defined."
#endif

#if defined(MXD_MXD_CRYPTO_RSA_OPENSSL) && \
    (!defined(MXD_CRYPTO_PUBLICKEY_OPENSSL) || \
     !defined(MXD_CRYPTO_PRIVATEKEY_OPENSSL))

     #error "The OpenSSL implementation of CRsa needs CPublicKey and CPrivateKey to be implemented using OpenSSL as well!"
#endif

#if defined(MXD_CRYPTO_UUIDGENERATOR_NONE)
    #if defined(MXD_CRYPTO_UUIDGENERATOR_MITOSFW) || \
        defined(MXD_CRYPTO_UUIDGENERATOR_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_UUIDGENERATOR
    #endif

#elif defined(MXD_CRYPTO_UUIDGENERATOR_MITOSFW)
    #if defined(MXD_CRYPTO_UUIDGENERATOR_NONE) || \
        defined(MXD_CRYPTO_UUIDGENERATOR_OVERRIDE)
        #define MXD_MULTIPLE_CRYPTO_UUIDGENERATOR
    #endif

    #define MXD_CRYPTO_UUIDGENERATOR_INCLUDE "Crypto/CUuidGeneratorMitosFw.h"

#elif defined(MXD_CRYPTO_UUIDGENERATOR_OPENSSL)
    #error "No OpenSSL implementation exists for UuidGenerator!"

#elif defined(MXD_CRYPTO_UUIDGENERATOR_OVERRIDE)
    #if defined(MXD_CRYPTO_UUIDGENERATOR_NONE) || \
        defined(MXD_CRYPTO_UUIDGENERATOR_MITOSFW)
        #define MXD_MULTIPLE_CRYPTO_UUIDGENERATOR
    #endif

    #if !defined(MXD_CRYPTO_UUIDGENERATOR_INCLUDE)
        #error "MXD_CRYPTO_UUIDGENERATOR_INCLUDE not defined!"
    #endif
#elif defined(MXD_CRYPTO_UUIDGENERATOR_MOCANASS)
    #error "No MocanaSS implementation exists for UuidGenerator!"

#else
    #define MXD_CRYPTO_UUIDGENERATOR_NONE
#endif

#if defined(MXD_CRYPTO_BASE64_NONE)
    #if defined(MXD_CRYPTO_BASE64_MITOSFW)
        #define MXD_MULTIPLE_CRYPTO_BASE64
    #endif

#elif defined(MXD_CRYPTO_BASE64_MITOSFW)
    #if defined(MXD_CRYPTO_BASE64_NONE)
        #define MXD_MULTIPLE_CRYPTO_BASE64
    #endif
#else
    #define MXD_CRYPTO_BASE64_NONE
#endif

#if defined(MXD_MULTIPLE_CRYPTO_BASE64)
    #error "Only one of MXD_CRYPTO_BASE64_XXX must be defined."
#endif

//  These defines allows for the correct initialisation of the different crypto
//  engines.  They are NOT exclusive, as different algorithms can use different
//  crypto engines, which must be initialised.
//  These defines are only for internal use, and should not be defined manually.
#if defined(MXD_CRYPTO_AES_MITOSFW) || \
    defined(MXD_CRYPTO_DIFFIEHELLMAN_MITOSFW) || \
    defined(MXD_CRYPTO_MD5_MITOSFW) || \
    defined(MXD_CRYPTO_MD5MAC_MITOSFW) || \
    defined(MXD_CRYPTO_RSA_MITOSFW) || \
    defined(MXD_CRYPTO_SHA1_MITOSFW) || \
    defined(MXD_CRYPTO_SHA1MAC_MITOSFW) || \
    defined(MXD_CRYPTO_SHA2_MITOSFW) || \
    defined(MXD_CRYPTO_SHA2MAC_MITOSFW) || \
    defined(MXD_CRYPTO_SECUREPRNG_MITOSFW) || \
    defined(MXD_CRYPTO_SECURESEED_MITOSFW) || \
    defined(MXD_CRYPTO_PRIVATEKEY_MITOSFW) || \
    defined(MXD_CRYPTO_PUBLICKEY_MITOSFW)|| \
    defined(MXD_CRYPTO_UUIDGENERATOR_MITOSFW)

    #if !defined(MXD_CRYPTO_USE_ENGINE_MITOSFW)
        #define MXD_CRYPTO_USE_ENGINE_MITOSFW
    #else
        #error "MXD_CRYPTO_USE_ENGINE_MITOSFW can only be defined by FrameworkCfg.h!"
    #endif
#endif

#if defined(MXD_CRYPTO_AES_OPENSSL) || \
    defined(MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL) || \
    defined(MXD_CRYPTO_MD5_OPENSSL) || \
    defined(MXD_CRYPTO_MD5MAC_OPENSSL) || \
    defined(MXD_CRYPTO_RSA_OPENSSL) || \
    defined(MXD_CRYPTO_SHA1_OPENSSL) || \
    defined(MXD_CRYPTO_SHA1MAC_OPENSSL) || \
    defined(MXD_CRYPTO_SHA2_OPENSSL) || \
    defined(MXD_CRYPTO_SHA2MAC_OPENSSL) || \
    defined(MXD_CRYPTO_SECUREPRNG_OPENSSL) || \
    defined(MXD_CRYPTO_PRIVATEKEY_OPENSSL) || \
    defined(MXD_CRYPTO_PUBLICKEY_OPENSSL)

    #if !defined(MXD_CRYPTO_USE_ENGINE_OPENSSL)
        #define MXD_CRYPTO_USE_ENGINE_OPENSSL
    #else
        #error "MXD_CRYPTO_USE_ENGINE_OPENSSL can only be defined by FrameworkCfg.h!"
    #endif

#endif

#if defined(MXD_CRYPTO_AES_MOCANA_SS) || \
    defined(MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS) || \
    defined(MXD_CRYPTO_MD5_MOCANA_SS) || \
    defined(MXD_CRYPTO_MD5MAC_MOCANA_SS) || \
    defined(MXD_CRYPTO_RSA_MOCANA_SS) || \
    defined(MXD_CRYPTO_SHA1_MOCANA_SS) || \
    defined(MXD_CRYPTO_SECUREPRNG_MOCANA_SS) || \
    defined(MXD_CRYPTO_SECURESEED_MOCANA_SS) || \
    defined(MXD_CRYPTO_PRIVATEKEY_MOCANA_SS) || \
    defined(MXD_CRYPTO_PUBLICKEY_MOCANA_SS)

    #if !defined(MXD_CRYPTO_USE_ENGINE_MOCANA_SS)
        #define MXD_CRYPTO_USE_ENGINE_MOCANA_SS
    #else
        #error "MXD_CRYPTO_USE_ENGINE_MOCANA_SS can only be defined by FrameworkCfg.h!"
    #endif

    #if defined (MXD_OS_WINDOWS)
        #define __WIN32_RTOS__
    #elif defined (MXD_OS_LINUX)
        #define __LINUX_RTOS__
    #elif defined (MXD_OS_VXWORKS)
        #define __VXWORKS_RTOS__
    #endif

#endif

#if defined(MXD_CRYPTO_USE_ENGINE_OPENSSL) && \
    defined (MXD_CRYPTO_USE_ENGINE_MOCANA_SS)
    #error "cannot use MXD_CRYPTO_USE_ENGINE_OPENSSL and  MXD_CRYPTO_USE_ENGINE_MOCANA_SS simultaneously!"
#endif

#if defined(MXD_CRYPTO_AES_OVERRIDE) || \
    defined(MXD_CRYPTO_DIFFIEHELLMAN_OVERRIDE) || \
    defined(MXD_CRYPTO_MD5_OVERRIDE) || \
    defined(MXD_CRYPTO_MD5MAC_OVERRIDE) || \
    defined(MXD_CRYPTO_RSA_OVERRIDE) || \
    defined(MXD_CRYPTO_SHA1_OVERRIDE) || \
    defined(MXD_CRYPTO_SHA1MAC_OVERRIDE) || \
    defined(MXD_CRYPTO_SHA2_OVERRIDE) || \
    defined(MXD_CRYPTO_SHA2MAC_OVERRIDE) || \
    defined(MXD_CRYPTO_SECUREPRNG_OVERRIDE) || \
    defined(MXD_CRYPTO_SECURESEED_OVERRIDE) || \
    defined(MXD_CRYPTO_PRIVATEKEY_OVERRIDE) || \
    defined(MXD_CRYPTO_PUBLICKEY_OVERRIDE)|| \
    defined(MXD_CRYPTO_UUIDGENERATOR_OVERRIDE)

    #if !defined(MXD_CRYPTO_USE_ENGINE_OVERRIDE)
        #define MXD_CRYPTO_USE_ENGINE_OVERRIDE
    #else
        #error "MXD_CRYPTO_USE_ENGINE_OVERRIDE can only be defined by FrameworkCfg.h!"
    #endif
#endif

#if defined(MXD_PKI_OPENSSL) || \
    defined(MXD_PKI_MOCANA_SS)
    #if !defined(MXD_CRYPTO_RSA_OPENSSL) && \
        !defined(MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL) && \
        !defined(MXD_CRYPTO_RSA_MOCANA_SS)
        #error "Sorry, PKI implemented only for MXD_CRYPTO_RSA_OPENSSL and MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL!"
    #endif
#endif

#if defined(MXD_PKI_OPENSSL)
    #if defined(MXD_PKI_NONE) || \
        defined(MXD_PKI_MOCANA_SS) || \
        defined(MXD_PKI_OVERRIDE)
        #define MXD_MULTIPLE_PKI
    #endif

    #define MXD_PKI_CCERTIFICATE_INCLUDE "Pki/CCertificateOpenSsl.h"
    #define MXD_PKI_CERTIFICATECHAIN_INCLUDE "Pki/CCertificateChainOpenSsl.h"
    #define MXD_PKI_CCERTIFICATECHAINVALIDATION_INCLUDE "Pki/CCertificateChainValidationOpenSsl.h"
    #define MXD_PKI_CCERTIFICATEISSUER_INCLUDE "Pki/CCertificateIssuerOpenSsl.h"
    #define MXD_PKI_CCERTIFICATESUBJECT_INCLUDE "Pki/CCertificateSubjectOpenSsl.h"
    #define MXD_PKI_CCERTIFICATEEXTENSION_INCLUDE "Pki/CCertificateExtensionOpenSsl.h"
    #define MXD_PKI_CBASICCONSTRAINTS_INCLUDE "Pki/CBasicConstraintsOpenSsl.h"
    #define MXD_PKI_CKEYUSAGE_INCLUDE "Pki/CKeyUsageOpenSsl.h"
    #define MXD_PKI_CAUTHORITYKEYIDENTIFIER_INCLUDE "Pki/CAuthorityKeyIdentifierOpenSsl.h"
    #define MXD_PKI_CALTERNATENAME_INCLUDE "Pki/CAlternateNameOpenSsl.h"
    #define MXD_PKI_CEXTENDEDKEYUSAGE_INCLUDE "Pki/CExtendedKeyUsageOpenSsl.h"
    #define MXD_PKI_CSUBJECTKEYIDENTIFIER_INCLUDE "Pki/CSubjectKeyIdentifierOpenSsl.h"

#elif defined(MXD_PKI_MOCANA_SS)
    #if defined(MXD_PKI_NONE) || \
        defined(MXD_PKI_OPENSSL) || \
        defined(MXD_PKI_OVERRIDE)
        #define MXD_MULTIPLE_PKI
    #endif

    #define MXD_PKI_CCERTIFICATE_INCLUDE "Pki/CCertificateMocanaSs.h"
    #define MXD_PKI_CERTIFICATECHAIN_INCLUDE "Pki/CCertificateChainMocanaSs.h"
    #define MXD_PKI_CCERTIFICATECHAINVALIDATION_INCLUDE "Pki/CCertificateChainValidationMocanaSs.h"
    #define MXD_PKI_CCERTIFICATEISSUER_INCLUDE "Pki/CCertificateNameMocanaSs.h"
    #define MXD_PKI_CCERTIFICATESUBJECT_INCLUDE "Pki/CCertificateNameMocanaSs.h"
    #define MXD_PKI_CCERTIFICATEEXTENSION_INCLUDE "Pki/CCertificateExtensionMocanaSs.h"
    #define MXD_PKI_CBASICCONSTRAINTS_INCLUDE "Pki/CBasicConstraintsMocanaSs.h"
    #define MXD_PKI_CKEYUSAGE_INCLUDE "Pki/CKeyUsageMocanaSs.h"
    #define MXD_PKI_CAUTHORITYKEYIDENTIFIER_INCLUDE "Pki/CAuthorityKeyIdentifierMocanaSs.h"
    #define MXD_PKI_CALTERNATENAME_INCLUDE "Pki/CAlternateNameMocanaSs.h"
    #define MXD_PKI_CEXTENDEDKEYUSAGE_INCLUDE "Pki/CExtendedKeyUsageMocanaSs.h"
    #define MXD_PKI_CSUBJECTKEYIDENTIFIER_INCLUDE "Pki/CSubjectKeyIdentifierMocanaSs.h"

#elif defined(MXD_PKI_OVERRIDE)
    #if defined(MXD_PKI_NONE) || \
        defined(MXD_PKI_OPENSSL) || \
        defined(MXD_PKI_MOCANA_SS)
        #define MXD_MULTIPLE_PKI
    #endif

    #if !defined(MXD_PKI_CCERTIFICATE_INCLUDE)
        #error "MXD_PKI_CCERTIFICATE_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CERTIFICATECHAIN_INCLUDE)
        #error "MXD_PKI_CERTIFICATECHAIN_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CCERTIFICATECHAINVALIDATION_INCLUDE)
        #error "MXD_PKI_CCERTIFICATECHAINVALIDATION_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CCERTIFICATEISSUER_INCLUDE)
        #error "MXD_PKI_CCERTIFICATEISSUER_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CCERTIFICATESUBJECT_INCLUDE)
        #error "MXD_PKI_CCERTIFICATESUBJECT_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CCERTIFICATEEXTENSION_INCLUDE)
        #error "MXD_PKI_CCERTIFICATEEXTENSION_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CBASICCONSTRAINTS_INCLUDE)
        #error "MXD_PKI_CBASICCONSTRAINTS_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CKEYUSAGE_INCLUDE)
        #error "MXD_PKI_CKEYUSAGE_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CAUTHORITYKEYIDENTIFIER_INCLUDE)
        #error "MXD_PKI_CAUTHORITYKEYIDENTIFIER_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CALTERNATENAME_INCLUDE)
        #error "MXD_PKI_CALTERNATENAME_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CEXTENDEDKEYUSAGE_INCLUDE)
        #error "MXD_PKI_CEXTENDEDKEYUSAGE_INCLUDE not defined!"
    #endif
    #if !defined(MXD_PKI_CSUBJECTKEYIDENTIFIER_INCLUDE)
        #error "MXD_PKI_CSUBJECTKEYIDENTIFIER_INCLUDE not defined!"
    #endif
#else
    #define MXD_PKI_NONE
#endif

#if defined(MXD_MULTIPLE_PKI)
    #error "Only one of MXD_PKI_XXX must be defined."
#endif

#if defined(MXD_SNTP_CLIENT_ENABLE_SUPPORT) && !defined(MXD_NETWORK_ENABLE_SUPPORT)
    #error "MXD_NETWORK_ENABLE_SUPPORT must be defined to enable SNTP support!"
#endif

#if defined(MXD_SYSLOG_ENABLE_SUPPORT) && !defined(MXD_NETWORK_ENABLE_SUPPORT)
    #error "MXD_NETWORK_ENABLE_SUPPORT must be defined to enable Syslog support!"
#endif

#if defined(MXD_KERBEROS_ENABLE_SUPPORT) && !defined(MXD_NETWORK_ENABLE_SUPPORT)
    #error "MXD_NETWORK_ENABLE_SUPPORT must be defined to enable Kerberos support!"
#endif

#if defined(MXD_IPV6_ENABLE_SUPPORT) && !defined(MXD_NETWORK_ENABLE_SUPPORT)
    #error "MXD_NETWORK_ENABLE_SUPPORT must be defined to enable IPv6 support!"
#endif

#if defined(MXD_IPV6_SCOPE_ID_MANDATORY_IN_ALL_ADDRESSES) && !defined(MXD_IPV6_ENABLE_SUPPORT)
    #error "MXD_IPV6_ENABLE_SUPPORT must be defined to mandate IPv6 scope in IPv6 addresses!"
#endif

#if !defined(MXD_THREAD_PRIORITY_EXTENSION)
    #define MXD_THREAD_PRIORITY_EXTENSION
#else
    #if defined(MXD_OS_VXWORKS)
        #if !defined(MXD_astVXWORKS_THREAD_SCHEDULING_INFO)
            #error "MXD_astVXWORKS_THREAD_SCHEDULING_INFO must be defined if MXD_THREAD_PRIORITY_EXTENSION is defined."
        #endif

    #elif defined(MXD_OS_LINUX)   || \
          defined(MXD_OS_ANDROID) || \
          defined(MXD_OS_SOLARIS) || \
          defined(MXD_OS_DARWIN)  || \
          defined(MXD_OS_ECOS)    || \
          defined(MXD_OS_NUCLEUS)
        #if !defined(MXD_astPOSIX_THREAD_SCHEDULING_INFO)
            #error "MXD_astPOSIX_THREAD_SCHEDULING_INFO must be defined if MXD_THREAD_PRIORITY_EXTENSION is defined."
        #endif

    #elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
        #if !defined(MXD_astWINDOWS_THREAD_SCHEDULING_INFO)
            #error "MXD_astWINDOWS_THREAD_SCHEDULING_INFO must be defined if MXD_THREAD_PRIORITY_EXTENSION is defined."
        #endif
    #endif
#endif

#if defined(MXD_OS_VXWORKS)
    #if !defined(MXD_astVXWORKS_THREAD_SCHEDULING_INFO)
        #define MXD_astVXWORKS_THREAD_SCHEDULING_INFO \
                {{255},    {200},   {150},    {100},  {55}}
            //  {eLOWEST,  eLOW,    eNORMAL,  eHIGH,  eHIGHEST};
    #endif

#elif defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
    #if !defined(MXD_astPOSIX_THREAD_SCHEDULING_INFO)
        #define MXD_astPOSIX_THREAD_SCHEDULING_INFO     \
            {                                           \
                {SCHED_OTHER, 0},   /*eLOWEST */        \
                {SCHED_OTHER, 0},   /*eLOW    */        \
                {SCHED_OTHER, 0},   /*eNORMAL */        \
                {SCHED_OTHER, 0},   /*eHIGH   */        \
                {SCHED_OTHER, 0}    /*eHIGHEST*/        \
            };
    #endif

#elif defined(MXD_OS_NUCLEUS)
    #if !defined(MXD_astPOSIX_THREAD_SCHEDULING_INFO)
        #define MXD_astPOSIX_THREAD_SCHEDULING_INFO     \
            {                                           \
                {SCHED_RR, 0},      /*eLOWEST */        \
                {SCHED_RR, 55},     /*eLOW    */        \
                {SCHED_RR, 100},    /*eNORMAL */        \
                {SCHED_RR, 150},    /*eHIGH   */        \
                {SCHED_RR, 200}     /*eHIGHEST*/        \
            };
    #endif

#elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    #if !defined(MXD_astWINDOWS_THREAD_SCHEDULING_INFO)
        #define MXD_astWINDOWS_THREAD_SCHEDULING_INFO               \
            {                                                       \
                {THREAD_PRIORITY_LOWEST},           /*eLOWEST */    \
                {THREAD_PRIORITY_BELOW_NORMAL},     /*eLOW    */    \
                {THREAD_PRIORITY_NORMAL},           /*eNORMAL */    \
                {THREAD_PRIORITY_ABOVE_NORMAL},     /*eHIGH   */    \
                {THREAD_PRIORITY_HIGHEST}           /*eHIGHEST*/    \
            }
    #endif

#elif defined(MXD_OS_SYMBIAN)
    #if !defined(MXD_astSYMBIAN_THREAD_SCHEDULING_INFO)
        #define MXD_astSYMBIAN_THREAD_SCHEDULING_INFO \
                {{-20},    {-10},   {0},    {10},  {20}}
            //  {eLOWEST,  eLOW,    eNORMAL,  eHIGH,  eHIGHEST};
    #endif

// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    #if !defined(MXD_astECOS_THREAD_SCHEDULING_INFO)
        #define MXD_astECOS_THREAD_SCHEDULING_INFO       \
            {                                            \
                {MTA_TASK_BACKGROUND},   /*eLOWEST */    \
                {MTA_TASK_MANAGEMENT},   /*eLOW    */    \
                {MTA_TASK_DATA},         /*eNORMAL */    \
                {MTA_TASK_RTP_DATA},     /*eHIGH   */    \
                {MTA_TASK_HARDWARE}      /*eHIGHEST*/    \
            }
    #endif
// BRCM_CUSTOM - [end] - Add ECOS support.

#endif

// DNS resolver configuration.
//-----------------------------
#if defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT)
    // The portable resolver only supports UDP.
    //------------------------------------------
    #undef MXD_MAX_DNS_PACKET_LENGTH
    #define MXD_MAX_DNS_PACKET_LENGTH 512
#elif !defined (MXD_MAX_DNS_PACKET_LENGTH)
    #define MXD_MAX_DNS_PACKET_LENGTH 512
#endif

// DNS resolver cache configuration.
//-----------------------------------
#if defined (MXD_RESOLVER_CACHE_ENABLE_SUPPORT)
    #if !defined (MXD_RESOLVER_CACHE_CAPACITY)
    #define MXD_RESOLVER_CACHE_CAPACITY 100
    #endif
    // 1 hour.
    //---------
    #if !defined (MXD_RESOLVER_CACHE_POSITIVE_MAX_TTL_S)
    #define MXD_RESOLVER_CACHE_POSITIVE_MAX_TTL_S 3600
    #endif
    // 5 minutes (RFC-1035).
    //-----------------------
    #if !defined (MXD_RESOLVER_CACHE_NEGATIVE_MAX_TTL_S)
    #define MXD_RESOLVER_CACHE_NEGATIVE_MAX_TTL_S 300 // 5 minutes (RFC-1035)
    #endif
#endif

// IPv6 configuration.
//---------------------
// BRCM_CUSTOM - [mod] Add ECOS support.
#if defined(MXD_IPV6_ENABLE_SUPPORT)
    #if !defined(MXD_OS_LINUX) && \
        !defined(MXD_OS_WINDOWS) && \
        !defined(MXD_OS_WINDOWS_CE) && \
        !defined(MXD_OS_BOS_ECOS) && \
        (!defined(MXD_OS_VXWORKS) || (defined(MXD_OS_VXWORKS) && (MXD_OS_VXWORKS < MXD_OS_VXWORKS_5_5)))
// BRCM_CUSTOM - [end] Add ECOS support.
        #error "IPv6 is supported only on Linux, Windows, Windows CE and VxWorks 5.5!"
    #endif
#endif

// ENUM configuration.
//---------------------
#if defined(MXD_ENUM_ENABLE_SUPPORT) && !defined(MXD_REGEXP_ENABLE_SUPPORT)
    #error "MXD_REGEXP_ENABLE_SUPPORT must be enabled for ENUM support."
#endif

// Thread stack info support.
//----------------------------
#if defined(MXD_THREAD_STACK_INFO_ENABLE_SUPPORT) && defined(MXD_COMPILER_WINSCW)
    #error "The thread stack info mechanism is not supported on the Symbian emulator"
#endif

// Native Atomic Operations support.
//-----------------------------------
#if defined(MXD_ATOMIC_NATIVE_ENABLE_SUPPORT)
#if (!defined(MXD_OS_WINDOWS) || !defined(MXD_COMPILER_MS_VC) || !defined(MXD_ARCH_IX86)) && \
    (!defined(MXD_OS_LINUX) || !defined(MXD_COMPILER_GNU_GCC) || !defined(MXD_ARCH_IX86)) && \
    (!defined(MXD_OS_LINUX) || !defined(MXD_COMPILER_GNU_GCC) || !defined(MXD_ARCH_AMD64)) && \
    (!defined(MXD_OS_LINUX) || !defined(MXD_COMPILER_GNU_GCC) || !defined(MXD_ARCH_PPC))
    #error "Native atomic operations are not supported on the current platform."
#endif
#endif

// Portable resolver configuration.
#if defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT)
#if !defined MXD_PORTABLE_RESOLVER_MAX_RETRANSMISSIONS
#define MXD_PORTABLE_RESOLVER_MAX_RETRANSMISSIONS 5
#endif
#if !defined (MXD_PORTABLE_RESOLVER_RETRANSMISSION_TIMEOUT_MS)
#define MXD_PORTABLE_RESOLVER_RETRANSMISSION_TIMEOUT_MS 1000
#endif
#endif // #if defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT)

// Cap Sub-allocator support
#if !defined(MXD_CAP_ENABLE_SUPPORT) && \
    defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT)
#error MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT requires MXD_CAP_ENABLE_SUPPORT.
#endif

#if (!defined(MXD_CAP_ENABLE_SUPPORT) || \
     !defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT)) && \
    defined(MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT)
#error MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT requires MXD_CAP_ENABLE_SUPPORT and MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT.
#endif

#if !defined(MXD_CAP_SUBALLOCATOR_DEFAULT_MEMORY_BLOCK_SIZE_IN_BYTES)
    #define MXD_CAP_SUBALLOCATOR_DEFAULT_MEMORY_BLOCK_SIZE_IN_BYTES 2048
#endif

#if !defined(MXD_NETWORK_ASYNCSOCKET_CONNECT_TIMEOUT_MS)
    #define MXD_NETWORK_ASYNCSOCKET_CONNECT_TIMEOUT_MS MX_MAKEUINT64(0xFFFFFFFF, 0xFFFFFFFF)
#endif

#if !defined(MXD_MINIMAL_ALIGNMENT_IN_BYTES)
    #if defined(MXD_DATA_MODEL_ILP32)
        #define MXD_MINIMAL_ALIGNMENT_IN_BYTES 4
    #else
        #define MXD_MINIMAL_ALIGNMENT_IN_BYTES 8
    #endif
#endif

// PostFrameworkCfg
#if defined(MXD_POST_FRAMEWORKCFG)
#include "PostFrameworkCfg.h"
#endif


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

// Doc-O-Matic documentation section
#if 0

//M5T_INTERNAL_USE_BEGIN
//-- Stubbing mechanism is for internal use only. Thus, it is not documented in
//-- the API reference document.

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the stubbing of certain classes.
//
// Description:
//  Enables the stubbing of certain classes.  To stub a class, you must also
//  define it's corresponding MXD_STUB_XXX. The stubbing mechanism allows to
//  replace the default implementation of a class.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  MXD_STUB_CALTERNATENAME
//
//==EDOC========================================================================
#define MXD_ENABLE_STUBBING

//<GROUP CONFIG_MACROS>
//<TITLE Stubbing macros>
//==SDOC========================================================================
//
// Summary:
//  Enables the stubbing of a particular class.
//
// Description:
//  Enables the stubbing of a particular class.  To stub a class, you must
//  define MXD_ENABLE_STUBBING as well as its corresponding MXD_STUB_XXX. The
//  following classes can be stubbed.  Beside each is specified the location of
//  the stub file that must contain the new implementation of the class.
//      - CAlternateName              PkiStubs/CAlternateNameStub.h
//      - CBasicConstraints           PkiStubs/CBasicConstraintsStub.h
//      - CCertificate                PkiStubs/CCertificateStub.h
//      - CCertificateChain           PkiStubs/CCertificateChainStub.h
//      - CCertificateChainValidation PkiStubs/CCertificateChainValidationStub.h
//      - CCertificateExtention       PkiStubs/CCertificateExtensionStub.h
//      - CCertificateSubject         PkiStubs/CCertificateSubjectStub.h
//      - CExtendedKeyUsage           PkiStubs/CExtendedKeyUsageStub.h
//      - CIssuerAlternateName        PkiStubs/CIssuerAlternateNameStub.h
//      - CKeyUsage                   PkiStubs/CKeyUsageStub.h
//      - CNetscapeCertificateType    PkiStubs/CNetscapeCertificateTypeStub.h
//      - CPkcs7                      PkiStubs/CPkcs7Stub.h
//      - CPkcs12                     PkiStubs/CPkcs12Stub.h
//      - CSubjectAlternateName       PkiStubs/CSubjectAlternateNameStub.h
//      - CTimer                      KernelStubs/CTimerStub.h
//      - CTlsContext                 TlsStubs/CTlsContextStub.h
//      - NetworkTools                NetworkStubs/CNetworkToolsStub.h
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  MXD_ENABLE_STUBBING
//
//==EDOC========================================================================
#define MXD_STUB_CALTERNATENAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CAUTHORITYKEYIDENTIFIER

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CCERTIFICATE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CCERTIFICATECHAIN

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CCERTIFICATECHAINVALIDATION

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CCERTIFICATEEXTENSION

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CCERTIFICATESUBJECT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CSUBJECTALTERNATENAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CSUBJECTKEYIDENTIFIER

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CTIMER

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CTLSCONTEXT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CBASICCONSTRAINTS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CEXTENDEDKEYUSAGE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CISSUERALTERNATENAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CKEYUSAGE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CNETSCAPECERTIFICATETYPE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_NETWORKTOOLS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CPKCS7

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_STUB_CALTERNATENAME>
#define MXD_STUB_CPKCS12

//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Sets the size of the memory under VxWorks.
//
// Description:
//  Sets the size of the memory available under VxWorks. This must be specified
//  manually since there is no reliable way to get the memory size under that OS.
//  The value specified is a number of bytes.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_SYSTEM_MEMORY_SIZE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the portable DNS resolver.
//
// Description:
//  Enables a DNS resolver that is portable across all platforms supported by
//  the Framework.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT

//============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Defines the portable DNS resolver maximum number of retransmissions.
//
// Description:
//  Defines the number of times a nameserver will be retried prior to
//  considering that the query has failed. The default value is 5. Each server
//  is tried once, and then the first one is retried. The query is considered
//  retransmitted only when a nameserver is retried (the portable resolver
//  wrapped around the list).
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//============================================================================
#define MXD_PORTABLE_RESOLVER_MAX_RETRANSMISSIONS

//============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Defines the portable DNS resolver timeout prior to retransmitting a query.
//
// Description:
//  Defines the portable DNS resolver timeout prior to transmitting a query to a
//  less preferred nameserver (or to wrap around when the least preferred
//  nameserver has timed out). The default value is 1000 ms (1 second).
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//============================================================================
#define MXD_PORTABLE_RESOLVER_RETRANSMISSION_TIMEOUT_MS

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the DNS resolver's caching mechanism.
//
// Description:
//  Enables a DNS resolver's caching mechanism. This applies to all DNS
//  queries.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_RESOLVER_CACHE_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the DNS resolver's host file mechanism.
//
// Description:
//  Enables the DNS resolver's host file mechanism. This applies to all DNS
//  queries. The host file has priority over the DNS cache and the resolver.
//
//  Please refer to the CHostFile documentation for further information.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_RESOLVER_HOST_FILE_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Sets the maximum number of concurrent cached DNS answers.
//
// Description:
//  Sets the maximum number of answers concurrently cached in memory. When this
//  number is reached, the least recently used answer is dropped from the cache
//  to make room for the newest answer.
//
//  An expired cached answer remains in the cache until it is accessed again,
//  then it is removed and a query is made.
//
//  Defaults to 100.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_RESOLVER_CACHE_CAPACITY

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Maximum TTL value (in seconds) for positive DNS answers.
//
// Description:
//  Sets the maximum value (in seconds) a positive DNS answer can take when it
//  is cached.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//  Defaults to one hour.
//
//==EDOC========================================================================
#define MXD_RESOLVER_CACHE_POSITIVE_MAX_TTL_S

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Maximum TTL value (in seconds) for negative DNS answers.
//
// Description:
//  Sets the maximum value (in seconds) a negative DNS answer can take when it
//  is cached.
//
//  Defaults to 5 minutes.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_RESOLVER_CACHE_NEGATIVE_MAX_TTL_S

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Refresh the cache entry only after a complete expiration of its records.
//
// Description:
//  By default, the cache response entry is refreshed when one of its records
//  expires. By enabling this macro, the cache waits until every entry's records
//  are expired before refreshing.
//
//  Effective only if MXD_RESOLVER_CACHE_ENABLE_SUPPORT is defined.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_RESOLVER_CACHE_REFRESH_ONLY_ON_ALL_RECORDS_EXPIRED

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the support for Kerberos.
//
// Description:
//  Enables the support for Kerberos in the Framework using the MIT Kerberos
//  library.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_KERBEROS_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the support for Time.
//
// Description:
//  Enables the support for Time in the Framework.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_TIME_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the support for CFile traces.
//
// Description:
//  Enables the support for CFile traces in the Framework.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_CFILE_TRACES_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the support for the SNTP Client.
//
// Description:
//  Enables the support for the SNTP Client in the Framework.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_SNTP_CLIENT_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the support for the Servicing Thread feature.
//
// Description:
//  Enables the support for the Servicing Thread in the Framework. It also
//  controls asynchronous sockets since these are based on the Servicing Thread.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_SERVICING_THREAD_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the support for the Servicing Thread Watchdog Monitor.
//
// Description:
//  Enables the support for the Servicing Thread Watchdog monitoring in
//  the Framework. Servicing threads can register to the watchdog to be
//  monitored. Events about monitored threads are sent to all watchdog monitor
//  observers registered to the watchdog.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the support for Cap containers.
//
// Description:
//  Enables the support for Cap containers in the Framework.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_CAP_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the support for the ECOM feature.
//
// Description:
//  Enables the support for the ECOMs in the Framework.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_ECOM_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the support for the Event Notifier feature.
//
// Description:
//  Enables the support for the Event Notifier in the Framework.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_EVENT_NOTIFIER_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the use of the generic signal handler.
//
// Description:
//  By defining MXD_FAULT_HANDLER_ENABLE_SUPPORT, the application overrides the
//  current behaviour of the signals defined below with an internal function.
//  This function outputs information corresponding to the signal received and
//  dumps the current call stack of the application. Once the call stack is
//  dumped, the prevous behaviour of the signal is restored and the signal is
//  emitted again. For VxWorks exceptions, a hook is added to output the
//  exception received and dumps the call stack before the exception is
//  processed normally.
//
//  This is used for debugging purposes to output the signal information and the
//  call stack when a signal that would normally terminate the application is
//  received. This way, it is possible to determine what caused the signal to be
//  emitted when it is not emitted by a user like CTRL+c or a kill -Signal PID.
//
//  Linux:
//  This functionality is triggered by the following signals: SIGQUIT, SIGILL,
//  SIGABRT, SIGFPE, SIGSEGV, SIGBUS, SIGSYS, and SIGTRAP. The SIGPIPE exception
//  is blocked on Linux as it normally terminates the program. It is emitted
//  when a program writes to a socket that has no reader.
//
//  VxWorks:
//  This functionality is triggered by the following signals: SIGQUIT, SIGILL,
//  SIGABRT, SIGFPE, SIGSEGV, SIGBUS, SIGTRAP, and generic exceptions.
//
// Location:
//  Define this in "PreFrameworkConfig.h".
//
// See Also:
//  PreFrameworkConfig.
//
//==EDOC========================================================================
#define MXD_FAULT_HANDLER_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Adds the Framework error message strings to the compiled code.
//
// Description:
//  When this preprocessor macro is defined, the messages associated with the
//  results used by the Framework are put in the compiled code and can then
//  be returned by MX_RGET_MSG_STR.  If this macro is not defined,
//  MxResultGetMsgStr always returns an empty string for the Framework
//  error codes. This macro has no effect when MXD_RESULT_ENABLE_ERROR_MESSAGES
//  is not defined.
//
// Location:
//  Define this in "PreFrameworkCfg.h".
//
// See Also:
//  MXD_RESULT_ENABLE_ERROR_MESSAGES.
//
//==EDOC========================================================================
#define MXD_RESULT_ENABLE_MITOSFW_ERROR_MESSAGES

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the inclusion of "PostFrameworkCfg.h".
//
// Description:
//  Enables the inclusion of "PostFrameworkCfg.h" right at the end of
//  FrameworkCfg.h. "PostFrameworkCfg.h" is an application-provided file that
//  can contain additional configuration options to possibly override the
//  configuration found in PreFrameworkCfg.h and FrameworkCfg.h.
//
// Location:
//  Define this in PreFrameworkCfg.h or in your makefile.
//
// See Also:
//  PreFrameworkCfg.h
//
//==EDOC========================================================================
#define MXD_POST_FRAMEWORKCFG

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Disables the CString reference counting mechanism.
//
// Description:
//  This is used to disable the reference counting mechanism of CString objects.
//  When this is defined, copying one CString into another CString yields
//  CStrings each having their own buffer holding the character string.
//
// Warning:
//  By default, the CString objects implement a reference counting mechanism
//  where two CString objects can share the same character buffer. Access to
//  this buffer is not synchronized but the reference count is synchronized.
//
//  MXD_STRING_DISABLE_REFCOUNT yields an application that uses more memory.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_STRING_DISABLE_REFCOUNT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enable the CString minimum static buffer.
//
// Description:
//  This define is used to enable the CString minimum static buffer. This means
//  that all CStrings will contain a buffer inside the class of size
//  MXD_uCSTRING_BLOCK_LENGTH. This is useful when lots of small CString
//  objects are allocated, since it bypasses dynamic memory allocation as long
//  as the string stored in the CString stays smaller than
//  MXD_uCSTRING_BLOCK_LENGTH characters.
//
// Warning:
//  MXD_STRING_DISABLE_REFCOUNT needs to be defined for the CString minimum
//  static buffer feature to work. Should it be not defined when
//  MXD_CSTRING_MINIMUM_BUFFER is defined, MXD_STRING_DISABLE_REFCOUNT will be
//  automatically defined.
//
// Warning:
//  By default, the CString objects use dynamic memory allocation to store
//  strings. Enabling MXD_CSTRING_MINIMUM_BUFFER will make CString objects that
//  store small strings faster, but will use more memory, since a buffer of
//  size MXD_uCSTRING_BLOCK_LENGTH is allocated for every CString object. If
//  the string stored exceeds MXD_CSTRING_MINIMUM_BUFFER characters, dynamic
//  memory allocation is used.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CSTRING_MINIMUM_BUFFER

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Defines the minimal allocation block size for the CString object.
//
// Description:
//  The CString class holds a character buffer to keep its associated string.
//  When this buffer is allocated or when it has to be resized to a bigger size,
//  the CString allocates a number of chunks of MXD_uCSTRING_BLOCK_LENGTH bytes
//  to fit the character string.
//
//  As an example, when compiled with the default setting of 32, a one character
//  string uses 32 bytes while a 33 character long string takes 64 bytes, and so
//  on.
//
//  The default value is 32.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
//==EDOC========================================================================
#define MXD_uCSTRING_BLOCK_LENGTH 32

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Implements the debugging method PrintTree.
//
// Description:
//  When this flag is defined, the following debugging method is available:
//  <CODE>
//      void CAATree::PrintTree(IN CAATreeNode* pNode, IN int nIndent)
//  </CODE>
//
//  This method prints all nodes from the "pNode" subtree. If the tree root node
//  is used as "pNode", the entire CAATree is printed.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CAATREE_ENABLE_DEBUG

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Adds debugging information into the CMarshaler class.
//
// Description:
//  This flag affects the behaviour of the CMarshaler class.
//
//  Class CMarshaler: when an element is marshaled using the "<<" operator, an
//  additional constant value indicating the type of the element is inserted
//  with the element. When the element is extracted, the constant value is also
//  extracted and checked. This allows to validate that the right overloaded
//  ">>" operator has been used.
//
// Warning:
//  Make sure the assertion mechanism is active when you use this flag.
//
// See Also:
//  CMarshaler.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CMARSHALER_ENABLE_DEBUG

//<GROUP CONFIG_MACROS>
//<TITLE Crypto engine configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of all algorithms in the Crypto folder for
//  the given engine.
//
// Description:
//  These defines provide control on how the Crypto folder is compiled.
//  MXD_CRYPTO_ALL_XXX defines are mutually exclusive and at least one MUST be
//  defined for the framework to compile.
//      - MXD_CRYPTO_ALL_NONE: All Crypto implementation files are not compiled.
//      - MXD_CRYPTO_ALL_MITOSFW: All Crypto algorithms are provided by the
//        Framework itself.
//      - MXD_CRYPTO_ALL_OPENSSL: All Crypto algorithms use the OpenSSL library.
//        Requires wcecompat.lib when used with Windows CE.
//      - MXD_OPENSSL_FIPS_140_2_ENABLE_SUPPORT: enables the OpenSSL library in FIPS
//        mode. In order to set the OpenSSL library in FIPS mode, the string
//        MXD_INITIALIZER_PARAM_OPENSSL_ACTIVATE_FIPS_140_2_MODE needs to be
//        passed to the CFrameworkInitializer::Initialize() method.
//      - MXD_CRYPTO_ALL_MOCANA_SS: All Crypto algorithms use the Mocana SS
//        library.
//      - MXD_CRYPTO_ALL_OVERRIDE: All Crypto algorithms use implementations
//        provided via the overloading mechanism.
//  #Code size considerations#
//   If only a few algorithms are required, you can simply remove unused files
//   from the build tree or make sure your linker strips the unused symbols
//   from your executable.
//  #Overriding crypto implementation#
//   If, for any reason, one of the above crypto configurations is not suitable
//   for your particular needs, an override mechanism is at your disposal.  To
//   use it, you need to create a .h and a .cpp files that implement your
//   algorithm.  The class of your implementation must at least offer the
//   methods of the interface for that particular algorithm.  You then have to
//   specify the .h file of your implementation via the MXD_CRYPTO_XXX_INCLUDE
//   macro, and either the MXD_CRYPTO_XXX_OVERRIDE or MXD_CRYPTO_ALL_OVERRIDE
//   macro. You can replace only the algorithm(s) you really need to be
//   different(s) and leave all others unchanged.
//   One good reason for doing this would be to have an optimized implementation
//   of AES for your particular product inside a DSP for example.
//
//  Note:
//   These defines replace the previous MXD_CRYPTO_ENGINE_XXX, which are no
//   longer available.  It is important that you now use the appropriate
//   MXD_CRYPTO_ALL_XXX define to enable all algorithms using the same crypto
//   engine, or any combination of MXD_CRYPTO_XALGOX_XXX to choose the crypto
//   engine for a particular algorithm.  The MXD_CRYPTO_ALL_XXX defines have the
//   same behaviour as the corresponding MXD_CRYPTO_ENGINE_XXX had.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
// See Also:
//  <LINK MXD_CRYPTO_AES_NONE, AES crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_BASE64_NONE, Base64 crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_DIFFIEHELLMAN_NONE, Diffie-Hellman crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_MD5_NONE, MD5 crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_MD5MAC_NONE, MD5 MAC crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_PRIVATEKEY_NONE, CPrivateKey crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_PUBLICKEY_NONE, CPublicKey crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_RSA_NONE, RSA crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_SECUREPRNG_NONE, SecurePrng crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_SECURESEED_NONE, SecureSeed crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_SHA1_NONE, SHA-1 crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_SHA1MAC_NONE, SHA-1 MAC crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_SHA2_NONE, SHA-2 crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_SHA2MAC_NONE, SHA-2 MAC crypto algorithm configuration macros>,
//  <LINK MXD_CRYPTO_UUIDGENERATOR_NONE, Uuid Generator crypto algorithm configuration macros>
//
//==EDOC========================================================================
#define MXD_CRYPTO_ALL_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_ALL_NONE>
#define MXD_CRYPTO_ALL_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_ALL_NONE>
#define MXD_CRYPTO_ALL_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_ALL_NONE>
#define MXD_OPENSSL_FIPS_140_2_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_ALL_NONE>
#define MXD_INITIALIZER_PARAM_OPENSSL_ACTIVATE_FIPS_140_2_MODE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_ALL_NONE>
#define MXD_CRYPTO_ALL_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_ALL_NONE>
#define MXD_CRYPTO_ALL_OVERRIDE

//<GROUP CONFIG_MACROS>
//<TITLE AES crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the AES algorithm in the Crypto folder
//  for the given engine.
//
// Description:
//  These defines provide control on how the AES algorithm is compiled.
//  MXD_CRYPTO_AES_XXX defines are mutually exclusive (except for
//  MXD_CRYPTO_AES_INCLUDE) and at least one MUST be defined for the framework
//  to compile.
//      - MXD_CRYPTO_AES_NONE: AES implementation files are not compiled.
//      - MXD_CRYPTO_AES_MITOSFW: The AES algorithm is provided by the
//        Framework itself.
//      - MXD_CRYPTO_AES_OPENSSL: The AES algorithm uses the OpenSSL library.
//        Requires wcecompat.lib when used with Windows CE.
//      - MXD_CRYPTO_AES_MOCANA_SS: The AES algorithm uses the Mocana SS
//        library.
//      - MXD_CRYPTO_AES_OVERRIDE: The AES algorithm uses an implementation
//        provided via the overloading mechanism.
//      - MXD_CRYPTO_AES_INCLUDE: This define is used to specify the location
//        of the .h file used when overriding the AES algorithm.
//      - MXD_CRYPTO_AES_CLASSNAME: This define is used to specify the name of
//        the class used when overriding the AES algorithm. If that class is not
//        inside the MXD_GNS namespace, specify the namespace as well
//        (i.e. #define MXD_CRYPTO_AES_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_AES_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_AES_NONE>
#define MXD_CRYPTO_AES_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_AES_NONE>
#define MXD_CRYPTO_AES_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_AES_NONE>
#define MXD_CRYPTO_AES_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_AES_NONE>
#define MXD_CRYPTO_AES_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_AES_NONE>
#define MXD_CRYPTO_AES_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_AES_NONE>
#define MXD_CRYPTO_AES_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE Diffie-Hellman crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the Diffie-Hellman algorithm in the
//  Crypto folder for the given engine.
//
// Description:
//  These defines provide control on how the Diffie-Hellman algorithm is
//  compiled. MXD_CRYPTO_DIFFIEHELLMAN_XXX defines are mutually exclusive
//  (except for MXD_CRYPTO_DIFFIEHELLMAN_INCLUDE) and at least one MUST be
//  defined for the framework to compile.
//      - MXD_CRYPTO_DIFFIEHELLMAN_NONE: Diffie-Hellman implementation files
//        are not compiled.
//      - MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL: The Diffie-Hellman algorithm uses
//        the OpenSSL library. Requires wcecompat.lib when used with Windows CE.
//      - MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS: The Diffie-Hellman algorithm uses
//        the Mocana SS library.
//      - MXD_CRYPTO_DIFFIEHELLMAN_OVERRIDE: The Diffie-Hellman algorithm uses
//        an implementation provided via the overloading mechanism.
//      - MXD_CRYPTO_DIFFIEHELLMAN_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the Diffie-Hellman
//        algorithm.
//      - MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME: This define is used to specify
//        the name of the class used when overriding the Diffie-Hellman
//        algorithm. If that class is not inside the MXD_GNS namespace, specify
//        the namespace as well
//        (i.e. #define MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_DIFFIEHELLMAN_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_DIFFIEHELLMAN_NONE>
#define MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_DIFFIEHELLMAN_NONE>
#define MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_DIFFIEHELLMAN_NONE>
#define MXD_CRYPTO_DIFFIEHELLMAN_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_DIFFIEHELLMAN_NONE>
#define MXD_CRYPTO_DIFFIEHELLMAN_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_DIFFIEHELLMAN_NONE>
#define MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE MD5 crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the MD5 algorithm in the Crypto folder
//  for the given engine.
//
// Description:
//  These defines provide control on how the MD5 algorithm is compiled.
//  MXD_CRYPTO_MD5_XXX defines are mutually exclusive (except for
//  MXD_CRYPTO_MD5_INCLUDE) and at least one MUST be defined for the framework
//  to compile.
//      - MXD_CRYPTO_MD5_NONE: The MD5 implementation files are not compiled.
//      - MXD_CRYPTO_MD5_MITOSFW: The MD5 algorithm is provided by the
//        Framework itself.
//      - MXD_CRYPTO_MD5_OPENSSL: The MD5 algorithm uses the OpenSSL library.
//        Requires wcecompat.lib when used with Windows CE.
//      - MXD_CRYPTO_MD5_MOCANA_SS: The MD5 algorithm is undefined for use with
//        the Mocana SS library. Use the Framework algorithm.
//      - MXD_CRYPTO_MD5_OVERRIDE: The MD5 algorithm uses an implementation
//        provided via the overloading mechanism.
//      - MXD_CRYPTO_MD5_INCLUDE: This define is used to specify the location
//        of the .h file used when overriding the MD5 algorithm.
//      - MXD_CRYPTO_MD5_CLASSNAME: This define is used to specify the name of
//        the class used when overriding the MD5 algorithm. If that class is not
//        inside the MXD_GNS namespace, specify the namespace as well
//        (i.e. #define MXD_CRYPTO_MD5_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_MD5_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5_NONE>
#define MXD_CRYPTO_MD5_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5_NONE>
#define MXD_CRYPTO_MD5_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5_NONE>
#define MXD_CRYPTO_MD5_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5_NONE>
#define MXD_CRYPTO_MD5_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5_NONE>
#define MXD_CRYPTO_MD5_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5_NONE>
#define MXD_CRYPTO_MD5_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE MD5 MAC crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the MD5 MAC algorithm in the Crypto
//  folder for the given engine.
//
// Description:
//  These defines provide control on how the MD5 MAC algorithm is compiled.
//  MXD_CRYPTO_MD5MAC_XXX defines are mutually exclusive (except for
//  MXD_CRYPTO_MD5MAC_INCLUDE) and at least one MUST be defined for the
//  framework to compile.
//      - MXD_CRYPTO_MD5MAC_NONE: The MD5 MAC implementation files are not
//        compiled.
//      - MXD_CRYPTO_MD5MAC_MITOSFW: The MD5 MAC algorithm is provided by the
//        Framework itself.
//      - MXD_CRYPTO_MD5MAC_OPENSSL: The MD5 MAC algorithm uses the OpenSSL
//        library. Requires wcecompat.lib when used with Windows CE.
//      - MXD_CRYPTO_MD5MAC_MOCANA_SS: The MD5 MAC algorithm is undefined for
//        use with the Mocana SS library. Use the Framework algorithm.
//      - MXD_CRYPTO_MD5MAC_OVERRIDE: The MD5 MAC algorithm uses an
//        implementation provided via the overloading mechanism.
//      - MXD_CRYPTO_MD5MAC_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the MD5 MAC algorithm.
//      - MXD_CRYPTO_MD5MAC_CLASSNAME: This define is used to specify the name
//        of the class used when overriding the MD5 MAC algorithm. If that class
//        is not inside the MXD_GNS namespace, specify the namespace as well
//        (i.e. #define MXD_CRYPTO_MD5MAC_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_MD5MAC_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5MAC_NONE>
#define MXD_CRYPTO_MD5MAC_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5MAC_NONE>
#define MXD_CRYPTO_MD5MAC_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5MAC_NONE>
#define MXD_CRYPTO_MD5MAC_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5MAC_NONE>
#define MXD_CRYPTO_MD5MAC_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5MAC_NONE>
#define MXD_CRYPTO_MD5MAC_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_MD5MAC_NONE>
#define MXD_CRYPTO_MD5MAC_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE RSA crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the RSA algorithm in the Crypto folder
//  for the given engine.
//
// Description:
//  These defines provide control on how the RSA algorithm is compiled.
//  MXD_CRYPTO_RSA_XXX defines are mutually exclusive (except for
//  MXD_CRYPTO_RSA_INCLUDE) and at least one MUST be defined for the framework
//  to compile.
//      - MXD_CRYPTO_RSA_NONE: The RSA implementation files are not compiled.
//      - MXD_CRYPTO_RSA_OPENSSL: The RSA algorithm uses the OpenSSL library.
//        Requires wcecompat.lib when used with Windows CE.
//      - MXD_CRYPTO_RSA_MOCANA_SS: The RSA algorithm uses the Mocana SS
//        library.
//      - MXD_CRYPTO_RSA_OVERRIDE: The RSA algorithm uses an implementation
//        provided via the overloading mechanism.
//      - MXD_CRYPTO_RSA_INCLUDE: This define is used to specify the location
//        of the .h file used when overriding the RSA algorithm.
//      - MXD_CRYPTO_RSA_CLASSNAME: This define is used to specify the name of
//        the class used when overriding the RSA algorithm. If that class is not
//        inside the MXD_GNS namespace, specify the namespace as well
//        (i.e. #define MXD_CRYPTO_RSA_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_RSA_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_RSA_NONE>
#define MXD_CRYPTO_RSA_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_RSA_NONE>
#define MXD_CRYPTO_RSA_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_RSA_NONE>
#define MXD_CRYPTO_RSA_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_RSA_NONE>
#define MXD_CRYPTO_RSA_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_RSA_NONE>
#define MXD_CRYPTO_RSA_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_RSA_NONE>
#define MXD_CRYPTO_RSA_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE SHA-1 crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the SHA-1 algorithm in the Crypto folder
//  for the given engine.
//
// Description:
//  These defines provide control on how the SHA-1 algorithm is compiled.
//  MXD_CRYPTO_SHA1_XXX defines are mutually exclusive (except for
//  MXD_CRYPTO_SHA1_INCLUDE) and at least one MUST be defined for the framework
//  to compile.
//      - MXD_CRYPTO_SHA1_NONE: The SHA-1 implementation files are not compiled.
//      - MXD_CRYPTO_SHA1_MITOSFW: The SHA-1 algorithm is provided by the
//        Framework itself.
//      - MXD_CRYPTO_SHA1_OPENSSL: The SHA-1 algorithm uses the OpenSSL library.
//        Requires wcecompat.lib when used with Windows CE.
//      - MXD_CRYPTO_SHA1_MOCANA_SS: The SHA-1 algorithm uses the Mocana SS
//        library.
//      - MXD_CRYPTO_SHA1_OVERRIDE: The SHA-1 algorithm uses an implementation
//        provided via the overloading mechanism.
//      - MXD_CRYPTO_SHA1_INCLUDE: This define is used to specify the location
//        of the .h file used when overriding the SHA-1 algorithm.
//      - MXD_CRYPTO_SHA1_CLASSNAME: This define is used to specify the name of
//        the class used when overriding the SHA-1 algorithm. If that class is
//        not inside the MXD_GNS namespace, specify the namespace as well
//        (i.e. #define MXD_CRYPTO_SHA1_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_SHA1_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1_NONE>
#define MXD_CRYPTO_SHA1_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1_NONE>
#define MXD_CRYPTO_SHA1_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1_NONE>
#define MXD_CRYPTO_SHA1_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1_NONE>
#define MXD_CRYPTO_SHA1_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1_NONE>
#define MXD_CRYPTO_SHA1_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1_NONE>
#define MXD_CRYPTO_SHA1_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE SHA-1 MAC crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the SHA-1 MAC algorithm in the Crypto
//  folder for the given engine.
//
// Description:
//  These defines provide control on how the SHA-1 MAC algorithm is compiled.
//  MXD_CRYPTO_SHA1MAC_XXX defines are mutually exclusive (except for
//  MXD_CRYPTO_SHA1MAC_INCLUDE) and at least one MUST be defined for the
//  framework to compile.
//      - MXD_CRYPTO_SHA1MAC_NONE: The SHA-1 MAC implementation files are not
//        compiled.
//      - MXD_CRYPTO_SHA1MAC_MITOSFW: The SHA-1 MAC algorithm is provided by the
//        Framework itself.
//      - MXD_CRYPTO_SHA1MAC_OPENSSL: The SHA-1 MAC algorithm uses the OpenSSL
//        library. Requires wcecompat.lib when used with Windows CE.
//      - MXD_CRYPTO_SHA1MAC_OVERRIDE: The SHA-1 MAC algorithm uses an
//        implementation provided via the overloading mechanism.
//      - MXD_CRYPTO_SHA1MAC_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the SHA-1 MAC algorithm.
//      - MXD_CRYPTO_SHA1MAC_CLASSNAME: This define is used to specify the name
//        of the class used when overriding the SHA-1 MAC algorithm. If that
//        class is not inside the MXD_GNS namespace, specify the namespace as
//        well (i.e. #define MXD_CRYPTO_SHA1MAC_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_SHA1MAC_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1MAC_NONE>
#define MXD_CRYPTO_SHA1MAC_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1MAC_NONE>
#define MXD_CRYPTO_SHA1MAC_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1MAC_NONE>
#define MXD_CRYPTO_SHA1MAC_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1MAC_NONE>
#define MXD_CRYPTO_SHA1MAC_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA1MAC_NONE>
#define MXD_CRYPTO_SHA1MAC_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE SHA-2 crypto algorithm configuration macros>
//==============================================================================
//
// Summary:
//  Enables/disables the compilation of the SHA-2 algorithm (using 256 bits
//  only for now) in the Crypto folder for the given engine.
//
// Description:
//  These defines provide control on how the SHA-2 algorithm is compiled.
//  MXD_CRYPTO_SHA2_xxx defines are mutually exclusive (except for
//  MXD_CRYPTO_SHA2_INCLUDE and MXD_CRYPTO_SHA2_CLASSNAME) and at least
//  one MUST be defined in order for the Framework to compile.
//
//      - MXD_CRYPTO_SHA2_NONE: The SHA-2 implementation files are not
//        compiled.
//
//      - MXD_CRYPTO_SHA2_MITOSFW: The SHA-2 algorithm is provided by the
//        Framework itself.
//
//      - MXD_CRYPTO_SHA2_OPENSSL: The SHA-2 algorithm is provided by the
//        OpenSsl library.
//
//      - MXD_CRYPTO_SHA2_OVERRIDE: The SHA-2 algorithm uses an implementation
//        provided via the overloading mechanism.
//
//      - MXD_CRYPTO_SHA2_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the SHA-2 algorithm.
//
//      - MXD_CRYPTO_SHA2_CLASSNAME: This define is used to specify the name of
//        the class used when overriding the SHA-2 algorithm.
//        If that class is not inside the MXD_GNS namespace, specify the
//        namespace as well.
//        (i.e. #define MXD_CRYPTO_SHA2_CLASSNAME NameSpace::ClassName)
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==============================================================================
#define MXD_CRYPTO_SHA2_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA2_NONE>
#define MXD_CRYPTO_SHA2_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA2_NONE>
#define MXD_CRYPTO_SHA2_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA2_NONE>
#define MXD_CRYPTO_SHA2_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA2_NONE>
#define MXD_CRYPTO_SHA2_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA2_NONE>
#define MXD_CRYPTO_SHA2_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE SHA-2 MAC crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the SHA-2 MAC algorithm in the Crypto
//  folder for the given engine.
//
// Description:
//  These defines provide control on how the SHA-2 MAC algorithm is compiled.
//  MXD_CRYPTO_SHA2MAC_xxx defines are mutually exclusive (except for
//  MXD_CRYPTO_SHA2MAC_INCLUDE) and at least one MUST be
//  defined in order for the Framework to compile.
//
//      - MXD_CRYPTO_SHA2MAC_NONE: The SHA-2 MAC implementation files are
//        not compiled.
//
//      - MXD_CRYPTO_SHA2MAC_MITOSFW: The SHA-2 MAC algorithm is provided by
//        the Framework itself.
//
//      - MXD_CRYPTO_SHA2MAC_OPENSSL: The SHA-2 MAC algorithm is provided by
//        the OpenSSL library.
//
//      - MXD_CRYPTO_SHA2MAC_OVERRIDE: The SHA-2 MAC algorithm uses an
//        implementation provided via the overloading mechanism.
//
//      - MXD_CRYPTO_SHA2MAC_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the SHA-2 MAC
//        algorithm.
//
//      - MXD_CRYPTO_SHA2MAC_CLASSNAME: This define is used to specify the
//        name of the class used when overriding the SHA-2 MAC algorithm.
//        If that class is not inside the MXD_GNS namespace, specify the
//        namespace as well.
//        (i.e. #define MXD_CRYPTO_SHA2MAC_CLASSNAME NameSpace::ClassName)
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_SHA2MAC_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA2MAC_NONE>
#define MXD_CRYPTO_SHA2MAC_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA2MAC_NONE>
#define MXD_CRYPTO_SHA2MAC_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA2MAC_NONE>
#define MXD_CRYPTO_SHA2MAC_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA2MAC_NONE>
#define MXD_CRYPTO_SHA2MAC_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SHA2MAC_NONE>
#define MXD_CRYPTO_SHA2MAC_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE SecurePrng crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the SecurePrng algorithm in the Crypto
//  folder for the given engine.
//
// Description:
//  These defines provide control on how the SecurePrng algorithm is compiled.
//  MXD_CRYPTO_SECUREPRNG_XXX defines are mutually exclusive (except for
//  MXD_CRYPTO_SECUREPRNG_INCLUDE) and at least one MUST be defined for the
//  framework to compile.
//      - MXD_CRYPTO_SECUREPRNG_NONE: The SecurePrng implementation files are
//        not compiled.
//      - MXD_CRYPTO_SECUREPRNG_MITOSFW: The SecurePrng algorithm is provided
//        by the Framework itself.
//      - MXD_CRYPTO_SECUREPRNG_MOCANA_SS: The SecurePrng algorithm uses the
//        Mocana SS library.
//      - MXD_CRYPTO_SECUREPRNG_OVERRIDE: The SecurePrng algorithm uses an
//        implementation provided via the overloading mechanism.
//      - MXD_CRYPTO_SECUREPRNG_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the SecurePrng algorithm.
//      - MXD_CRYPTO_SECUREPRNG_CLASSNAME: This define is used to specify the
//        name of the class used when overriding the SecurePrng algorithm. If
//        that class is not inside the MXD_GNS namespace, specify the namespace
//        as well (i.e.
//        #define MXD_CRYPTO_SECUREPRNG_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_SECUREPRNG_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECUREPRNG_NONE>
#define MXD_CRYPTO_SECUREPRNG_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECUREPRNG_NONE>
#define MXD_CRYPTO_SECUREPRNG_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECUREPRNG_NONE>
#define MXD_CRYPTO_SECUREPRNG_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECUREPRNG_NONE>
#define MXD_CRYPTO_SECUREPRNG_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECUREPRNG_NONE>
#define MXD_CRYPTO_SECUREPRNG_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE SecureSeed crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the SecureSeed algorithm in the Crypto
//  folder for the given engine.
//
// Description:
//  These defines provide control on how the SecureSeed algorithm is compiled.
//  MXD_CRYPTO_SECURESEED_XXX defines are mutually exclusive (except for
//  MXD_CRYPTO_SECURESEED_INCLUDE) and at least one MUST be defined for the
//  framework to compile.
//      - MXD_CRYPTO_SECURESEED_NONE: The SecureSeed implementation files are
//        not compiled.
//      - MXD_CRYPTO_SECURESEED_MITOSFW: The SecureSeed algorithm is provided
//        by the Framework itself.
//      - MXD_CRYPTO_SECURESEED_MOCANA_SS: The SecureSeed algorithm uses the
//        Mocana SS library.
//      - MXD_CRYPTO_SECURESEED_OVERRIDE: The SecureSeed algorithm uses an
//        implementation provided via the overloading mechanism.
//      - MXD_CRYPTO_SECURESEED_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the SecureSeed algorithm.
//      - MXD_CRYPTO_SECURESEED_CLASSNAME: This define is used to specify the
//        name of the class used when overriding the SecureSeed algorithm.
//        If that class is not inside the MXD_GNS namespace, specify the
//        namespace as well
//        (i.e. #define MXD_CRYPTO_SECURESEED_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_SECURESEED_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECURESEED_NONE>
#define MXD_CRYPTO_SECURESEED_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECURESEED_NONE>
#define MXD_CRYPTO_SECURESEED_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECURESEED_NONE>
#define MXD_CRYPTO_SECURESEED_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECURESEED_NONE>
#define MXD_CRYPTO_SECURESEED_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECURESEED_NONE>
#define MXD_CRYPTO_SECURESEED_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE CPrivateKey crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the CPrivateKey algorithm in the Crypto
//  folder for the given engine.
//
// Description:
//  These defines provide control on how the CPrivateKey algorithm is compiled.
//  MXD_CRYPTO_PRIVATEKEY_XXX defines are mutually exclusive (except for
//  MXD_CRYPTO_PRIVATEKEY_INCLUDE) and at least one MUST be defined for the
//  framework to compile.
//      - MXD_CRYPTO_PRIVATEKEY_NONE: The CPrivateKey implementation files are
//        not compiled.
//      - MXD_CRYPTO_PRIVATEKEY_OPENSSL: The CPrivateKey algorithm uses the
//        OpenSSL library. Requires wcecompat.lib when used with Windows CE.
//      - MXD_CRYPTO_PRIVATEKEY_MOCANA_SS: The CPrivateKey algorithm uses the
//        Mocana SS library.
//      - MXD_CRYPTO_PRIVATEKEY_OVERRIDE: The CPrivateKey algorithm uses an
//        implementation provided via the overloading mechanism.
//      - MXD_CRYPTO_PRIVATEKEY_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the CPrivateKey algorithm.
//      - MXD_CRYPTO_PRIVATEKEY_CLASSNAME: This define is used to specify the
//        name of the class used when overriding the CPrivateKey algorithm.
//        If that class is not inside the MXD_GNS namespace, specify the
//        namespace as well
//        (i.e. #define MXD_CRYPTO_PRIVATEKEY_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_PRIVATEKEY_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PRIVATEKEY_NONE>
#define MXD_CRYPTO_PRIVATEKEY_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PRIVATEKEY_NONE>
#define MXD_CRYPTO_PRIVATEKEY_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PRIVATEKEY_NONE>
#define MXD_CRYPTO_PRIVATEKEY_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PRIVATEKEY_NONE>
#define MXD_CRYPTO_PRIVATEKEY_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PRIVATEKEY_NONE>
#define MXD_CRYPTO_PRIVATEKEY_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PRIVATEKEY_NONE>
#define MXD_CRYPTO_PRIVATEKEY_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE CPublicKey crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the CPublicKey algorithm in the Crypto
//  folder for the given engine.
//
// Description:
//  These defines provide control on how the CPublicKey algorithm is compiled.
//  MXD_CRYPTO_PUBLICKEY_xxx defines are mutually exclusive (except for
//  MXD_CRYPTO_PUBLICKEY_INCLUDE) and at least one MUST be defined for the
//  framework to compile.
//      - MXD_CRYPTO_PUBLICKEY_NONE: The CPublicKey implementation files are
//        not compiled.
//      - MXD_CRYPTO_PUBLICKEY_OPENSSL: The CPublicKey algorithm uses the
//        OpenSSL library. Requires wcecompat.lib when used with Windows CE.
//      - MXD_CRYPTO_PUBLICKEY_MOCANA_SS: The CPublicKey algorithm uses the
//        Mocana SS library.
//      - MXD_CRYPTO_PUBLICKEY_OVERRIDE: The CPublicKey algorithm uses an
//        implementation provided via the overloading mechanism.
//      - MXD_CRYPTO_PUBLICKEY_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the CPublicKey algorithm.
//      - MXD_CRYPTO_PUBLICKEY_CLASSNAME: This define is used to specify the
//        name of the class used when overriding the CPublicKey algorithm.
//        If that class is not inside the MXD_GNS namespace, specify the
//        namespace as well
//        (i.e. #define MXD_CRYPTO_PUBLICKEY_CLASSNAME NameSpace::ClassName).
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_PUBLICKEY_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PUBLICKEY_NONE>
#define MXD_CRYPTO_PUBLICKEY_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PUBLICKEY_NONE>
#define MXD_CRYPTO_PUBLICKEY_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PUBLICKEY_NONE>
#define MXD_CRYPTO_PUBLICKEY_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PUBLICKEY_NONE>
#define MXD_CRYPTO_PUBLICKEY_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PUBLICKEY_NONE>
#define MXD_CRYPTO_PUBLICKEY_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_PUBLICKEY_NONE>
#define MXD_CRYPTO_PUBLICKEY_CLASSNAME

//<GROUP CONFIG_MACROS>
//<TITLE Base64 crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the Base64 algorithm in the Crypto
//  folder for the given engine.
//
// Description:
//  These defines provide control on how the Base64 algorithm is compiled.
//  MXD_CRYPTO_BASE64_XXX defines are mutually exclusive and at least one
//  MUST be defined for the framework to compile.
//      - MXD_CRYPTO_BASE64_NONE: The Base64 implementation files are not
//        compiled.
//      - MXD_CRYPTO_BASE64_MITOSFW: The Base64 algorithm is provided by the
//        Framework itself.
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_BASE64_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_BASE64_NONE>
#define MXD_CRYPTO_BASE64_MITOSFW

//<GROUP CONFIG_MACROS>
//<TITLE Uuid Generator crypto algorithm configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of the UuidGenerator algorithm in the
//  Crypto folder for the given engine.
//
// Description:
//  These defines provide control on how the UUID generator is compiled.
//  MXD_CRYPTO_UUIDGENERATOR_XXX defines are mutually exclusive and at least one
//  MUST be defined for the framework to compile.
//      - MXD_CRYPTO_UUIDGENERATOR_NONE: The Uuid Generator implementation files
//        are not compiled.
//      - MXD_CRYPTO_UUIDGENERATOR_MITOSFW: The UuidGenerator algorithm is
//        provided by the Framework itself.
//      - MXD_CRYPTO_UUIDGENERATOR_OVERRIDE: The UuidGenerator algorithm uses an
//        implementation provided via the overloading mechanism.
//      - MXD_CRYPTO_UUIDGENERATOR_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the UuidGenerator
//        algorithm.
//      - MXD_CRYPTO_UUIDGENERATOR_CLASSNAME: This define is used to specify the
//        name of the class used when overriding the UuidGenerator algorithm.
//        If that class is not inside the MXD_GNS namespace, specify the
//        namespace as well.
//        (i.e. #define MXD_CRYPTO_UUIDGENERATOR_CLASSNAME NameSpace::ClassName)
//
// See Also:
//  <LINK MXD_CRYPTO_ALL_NONE, Crypto engine configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_UUIDGENERATOR_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECURESEED_NONE>
#define MXD_CRYPTO_UUIDGENERATOR_MITOSFW

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECURESEED_NONE>
#define MXD_CRYPTO_UUIDGENERATOR_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECURESEED_NONE>
#define MXD_CRYPTO_UUIDGENERATOR_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_SECURESEED_NONE>
#define MXD_CRYPTO_UUIDGENERATOR_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_CRYPTO_UUIDGENERATOR_NONE>
#define MXD_CRYPTO_UUIDGENERATOR_MITOSFW

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Disables all AES supported modes except for CTR.
//
// Description:
//  This disables all AES supported modes except for CTR. This configuration is
//  valid for MXD_CRYPTO_AES_MITOSFW only.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_AES_CTR_MODE_ONLY

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Unrolls loops in aes_core.c.
//
// Description:
//  This define allows to unroll loops in the file aes_core.c.
//  MXD_CRYPTO_AES_MITOSFW must be defined to have this optimization turned on.
//  This configuration is valid for MXD_CRYPTO_AES_MITOSFW only.
//
// See Also:
//  <LINK MXD_CRYPTO_AES_NONE, AES crypto algorithm configuration macros>
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_AES_CORE_UNROLL

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Allows relocating the table of constants used by the AES algorithm.
//
// Description:
//  When this switch is defined, all accesses to the constant table used by
//  the AES algorithm are done through a base pointer. This thus allows
//  relocating the constant table at run time.
//
//  In order to ease this relocation, this switch also enables the definition of
//  two new global variables, g_pstAesConstTable and g_stAesConstTableSize,
//  which respectively hold the address of the constant table and its size. This
//  configuration is valid for MXD_CRYPTO_AES_MITOSFW only.
//
// Warning:
//  Every access to the AES constants needs to dereference a pointer instead of
//  being resolved at compile time when
//  MXD_CRYPTO_AES_CORE_ALLOW_CONSTANTS_TABLE_RELOCATION is defined.
//  Consequently, defining this switch decreases the performance of the AES
//  algorithm and is not recommended until you really need it.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_AES_CORE_ALLOW_CONSTANTS_TABLE_RELOCATION

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Disables the support for messages longer than 512 MB in the M5T Framework
//  implementation of SHA-1.
//
// Description:
//  This define disables the support for messages longer that 2^32 bits (512 MB)
//  in the M5T Framework SHA-1 implementation. Without this define, the allowed
//  length for the input messages is 2^64 bits. The performance of the SHA-1
//  algorithm is improved on 32 bits architectures when long messages are
//  disabled because a 32 bits integer can be used to store the message length
//  instead of a 64 bits integer.
//
//  This define affects only the SHA-1 implementation of the M5T Framework, not
//  the OpenSSL or custom ones.
//
// See Also:
//  <LINK MXD_CRYPTO_SHA1_NONE, SHA-1 crypto algorithm configuration macros>,
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_CRYPTO_SHA1_DISABLE_LONG_MESSAGES

///////////////////////////////////////////////////////////////////////////////

//<GROUP CONFIG_MACROS>
//<TITLE Pki configuration macros>
//==SDOC========================================================================
//
// Summary:
//  Enables/disables the compilation of PKI for the given engine.
//
// Description:
//  These defines provide control on how PKI is compiled.
//  MXD_CRYPTO_AES_XXX defines are mutually exclusive (except for
//  MXD_PKI_XXX_INCLUDE) and at least one MUST be defined for the Framework
//  to compile.
//      - MXD_PKI_NONE: PKI implementation files are not compiled.
//      - MXD_PKI_OPENSSL: PKI uses the OpenSSL library.
//        MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL and MXD_CRYPTO_RSA_OPENSSL
//        must also be defined when defining MXD_PKI_OPENSSL.
//        Requires wcecompat.lib when used with Windows CE.
//      - MXD_PKI_MOCANA_SS: PKI uses the Mocana SS library.
//        MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS and MXD_CRYPTO_RSA_MOCANA_SS
//        must also be defined when defining MXD_PKI_OPENSSL.
//      - MXD_PKI_OVERRIDE: PKI uses an implementation
//        provided via the overloading mechanism.
//      - MXD_PKI_CCERTIFICATE_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the CCertificate class.
//      - MXD_PKI_CCERTIFICATE_CLASSNAME: This define is used to specify the
//        name of the class used when overriding the CCertificate class.
//        If that class is not inside the MXD_GNS namespace, specify
//        the namespace as well (i.e. MXD_PKI_CCERTIFICATE_CLASSNAME
//        NameSpace::ClassName).
//      - MXD_PKI_CERTIFICATECHAIN_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the CCertificateChain
//        class.
//      - MXD_PKI_CCERTIFICATECHAIN_CLASSNAME: This define is used to specify
//        the name of the class used when overriding the CCertificateChain class.
//        If that class is not inside the MXD_GNS namespace, specify
//        the namespace as well (i.e. #define
//        MXD_PKI_CCERTIFICATECHAIN_CLASSNAME NameSpace::ClassName).
//      - MXD_PKI_CCERTIFICATECHAINVALIDATION_INCLUDE: This define is used to
//        specify the location of the .h file used when overriding the
//        CCertificateChainValidation class.
//      - MXD_PKI_CCERTIFICATECHAINVALIDATION_CLASSNAME: This define is used
//        to specify the name of the class used when overriding the
//        CCertificateChainValidation class. If that class is not inside the MXD_GNS
//        namespace, specify the namespace as well (i.e. #define
//        MXD_PKI_CCERTIFICATECHAINVALIDATION_CLASSNAME NameSpace::ClassName).
//      - MXD_PKI_CCERTIFICATEISSUER_INCLUDE: This define is used to specify the
//        location of the .h file used when overriding the CCertificateIssuer
//        class.
//      - MXD_PKI_CCERTIFICATEISSUER_CLASSNAME: This define is used to specify
//        the name of the class used when overriding the CCertificateIssuer
//        class. If that class is not inside the MXD_GNS namespace,
//        specify the namespace as well (i.e. #define
//        MXD_PKI_CCERTIFICATEISSUER_CLASSNAME NameSpace::ClassName).
//      - MXD_PKI_CCERTIFICATESUBJECT_INCLUDE: This define is used to specify
//        the location of the .h file used when overriding the
//        CCertificateSubject class.
//      - MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME: This define is used to specify
//        the name of the class used when overriding the CCertificateSubject
//        class. If that class is not inside the MXD_GNS namespace,
//        specify the namespace as well (i.e. #define
//        MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME NameSpace::ClassName).
//      - MXD_PKI_CCERTIFICATEEXTENSION_INCLUDE: This define is used to specify
//        the location of the .h file used when overriding the
//        CCertificateExtension class.
//      - MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME: This define is used to
//        specify the name of the class used when overriding the
//        CCertificateExtension class. If that class is not inside the MXD_GNS
//        namespace, specify the namespace as well (i.e. #define
//        MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME NameSpace::ClassName).
//      - MXD_PKI_CBASICCONSTRAINTS_INCLUDE: This define is used to specify
//        the location of the .h file used when overriding the
//        CBasicConstraints class.
//      - MXD_PKI_CBASICCONSTRAINTS_CLASSNAME: This define is used to
//        specify the name of the class used when overriding the
//        CBasicConstraints class. If that class is not inside the MXD_GNS
//        namespace, specify the namespace as well (i.e. #define
//        MXD_PKI_CBASICCONSTRAINTS_CLASSNAME NameSpace::ClassName).
//      - MXD_PKI_CKEYUSAGE_INCLUDE: This define is used to specify
//        the location of the .h file used when overriding the
//        CKeyUsage class.
//      - MXD_PKI_CKEYUSAGE_CLASSNAME: This define is used to
//        specify the name of the class used when overriding the
//        CKeyUsage class. If that class is not inside the MXD_GNS
//        namespace, specify the namespace as well (i.e. #define
//        MXD_PKI_CKEYUSAGE_CLASSNAME NameSpace::ClassName).
//      - MXD_PKI_CAUTHORITYKEYIDENTIFIER_INCLUDE: This define is used to
//        specify the location of the .h file used when overriding the
//        CAuthorityKeyIdentifier class.
//      - MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME: This define is used to
//        specify the name of the class used when overriding the
//        CAuthorityKeyIdentifier class. If that class is not inside the
//        MXD_GNS namespace, specify the namespace as well (i.e. #define
//        MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME NameSpace::ClassName).
//      - MXD_PKI_CALTERNATENAME_INCLUDE: This define is used to
//        specify the location of the .h file used when overriding the
//        CAlternateName class.
//      - MXD_PKI_ALTERNATENAME_CLASSNAME: This define is used to
//        specify the name of the class used when overriding the
//        CAlternateName class. If that class is not inside the
//        MXD_GNS namespace, specify the namespace as well (i.e. #define
//        MXD_PKI_ALTERNATENAME_CLASSNAME NameSpace::ClassName).
//      - MXD_PKI_CEXTENDEDKEYUSAGE_INCLUDE: This define is used to
//        specify the location of the .h file used when overriding the
//        CExtendedKeyUsage class.
//      - MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME: This define is used to
//        specify the name of the class used when overriding the
//        CExtendedKeyUsage class. If that class is not inside the
//        MXD_GNS namespace, specify the namespace as well (i.e. #define
//        MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME NameSpace::ClassName).
//      - MXD_PKI_CSUBJECTKEYIDENTIFIER_INCLUDE: This define is used to
//        specify the location of the .h file used when overriding the
//        CSubjectKeyIdentifier class.
//      - MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME: This define is used to
//        specify the name of the class used when overriding the
//        CSubjectKeyIdentifier class. If that class is not inside the
//        MXD_GNS namespace, specify the namespace as well (i.e. #define
//        MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME NameSpace::ClassName).
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==EDOC========================================================================
#define MXD_PKI_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATE_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATE_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CERTIFICATECHAIN_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATECHAIN_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATECHAINVALIDATION_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATECHAINVALIDATION_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATEISSUER_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATEISSUER_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATESUBJECT_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATESUBJECT_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATEEXTENSION_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CBASICCONSTRAINTS_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CBASICCONSTRAINTS_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CKEYUSAGE_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CKEYUSAGE_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CAUTHORITYKEYIDENTIFIER_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CALTERNATENAME_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CALTERNATENAME_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CEXTENDEDKEYUSAGE_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CSUBJECTKEYIDENTIFIER_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_PKI_NONE>
#define MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME

///////////////////////////////////////////////////////////////////////////////

//<GROUP CONFIG_MACROS>
//<TITLE Tls configuration macros>
//==============================================================================
//
// Summary:
//  Enables/disables the compilation of the TLS support located in the
//  Tls folder.
//
// Description:
//  This define provides control on how TLS is compiled. MXD_TLS_XXX defines
//  are mutually exclusive (except for MXD_TLS_XXX_INCLUDE and
//  MXD_TLS_XXX_CLASSNAME) and at least one MUST be defined for the Framework to
//  compile.
//
//  - MXD_TLS_NONE: TLS support is not compiled.
//  - MXD_TLS_OPENSSL: TLS support is compiled using the OpenSsl library.
//  - MXD_TLS_MOCANA_SS: TLS support is compiled using the Mocana SS library.
//  - MXD_TLS_CTLS_INCLUDE: This define is used to specify
//    the location of the .h file used when overriding the CTls class.
//  - MXD_TLS_CTLS_CLASSNAME: This define is used to specify the name of the
//    class used when overriding the CTls class. If that class is not inside
//    the MXD_GNS namespace, specify the namespace as well (i.e. #define
//    MXD_TLS_CTLS_CLASSNAME NameSpace::ClassName).
//  - MXD_TLS_CTLSSESSION_INCLUDE: This define is used to specify
//    the location of the .h file used when overriding the CTlsSession class.
//  - MXD_TLS_CTLSSESSION_CLASSNAME: This define is used to specify the name of
//    the class used when overriding the CTlsSession class. If that class is
//    not insidethe MXD_GNS namespace, specify the namespace as well
//    (i.e. #define MXD_TLS_CTLSSESSION_CLASSNAME NameSpace::ClassName).
//  - MXD_TLS_CASYNCTLSSOCKET_INCLUDE: This define is used to specify
//    the location of the .h file used when overriding the CAsyncTlsSocket
//    class.
//  - MXD_TLS_CASYNCTLSSERVERSOCKET_INCLUDE: This define is used to specify
//    the location of the .h file used when overriding the CAsyncTlsServerSocket
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==============================================================================
#define MXD_TLS_NONE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TLS_NONE>
#define MXD_TLS_OPENSSL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TLS_NONE>
#define MXD_TLS_MOCANA_SS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TLS_NONE>
#define MXD_TLS_CTLS_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TLS_NONE>
#define MXD_TLS_CTLS_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TLS_NONE>
#define MXD_TLS_CTLSSESSION_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TLS_NONE>
#define MXD_TLS_CTLSSESSION_CLASSNAME

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TLS_NONE>
#define MXD_TLS_CASYNCTLSSOCKET_INCLUDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TLS_NONE>
#define MXD_TLS_CASYNCTLSSERVERSOCKET_INCLUDE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the Framework to start up the Mocana SS stack.
//
// Description:
//  When defined, it enables the Framework to start up and shut down the Mocana
//  SS stack in asynchronous mode. It uses the following two macros to start up
//  the stack:
//
//  MXD_TLS_MOCANA_SS_MAX_CLIENT_CONNECTIONS is used to set the maximum number
//  of client connections. When not defined, it defaults to 10.
//
//  MXD_TLS_MOCANA_SS_MAX_SERVER_CONNECTIONS is used to set the maximum number
//  of client connections. When not defined, it defaults to 1.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
// See Also:
//  PreFrameworkCfg.h
//
//==EDOC========================================================================
#define MXD_TLS_MOCANA_SS_INIT_STACK

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TLS_MOCANA_SS_INIT_STACK>
#define MXD_TLS_MOCANA_SS_MAX_CLIENT_CONNECTIONS

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TLS_MOCANA_SS_INIT_STACK>
#define MXD_TLS_MOCANA_SS_MAX_SERVER_CONNECTIONS

///////////////////////////////////////////////////////////////////////////////

//M5T_INTERNAL_USE_BEGIN
//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Please don't use this.
//
// Description:
//  Please don't use this.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
// See Also:
//  PreFrameworkCfg.h
//
//==EDOC========================================================================
#define MXD_PROPRIETARY_IMPLEMENTATION

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Please don't use this.
//
// Description:
//  Please don't use this.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
// See Also:
//  PreFrameworkCfg.h
//
//==EDOC========================================================================
#define MXD_FILESYSTEM_PROPRIETARY_IMPLEMENTATION
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the compilation of the XML support found in the Xml folder. XML is
//  disabled by default.
//
// Description:
//  This permits the compilation of the XML support found in the Xml folder.
//  When this is not defined, the XML specific implementation files found in Xml
//  are not compiled. XML is disabled by default. While one XML implementation
//  should be enabled at all times, it is possible to enable both of them at the
//  same time.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
// See Also:
//  PreFrameworkCfg.h
//
//==EDOC========================================================================
#define MXD_XML_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the compilation of the XML Expat parser support found in the Xml
//  folder. XML Expat parser support is disabled by default.
//
// Description:
//  This permits the compilation of the XML Expat parser support found in the
//  Xml folder. When this is not defined, the XML Expat parser specific
//  implementation files found in Xml are not compiled. XML Expat Parser is
//  disabled by default. When disabled, it allows the user to implement the
//  IXmlParser interface specifically for another XML parser and use it
//  transparently within the XML specific files.
//
// Warning:
//  If disabled, another custom parser implementation MUST be provided and
//  compiled for the rest of the XML files to work properly. This custom
//  implementation MUST implement the IXmlParser interface, MUST use the
//  IXmlParserMgr interface, and its ECOM CLSID MUST be defined as
//  CLSID_CXmlParser. Failure to comply will most likely result in a crash.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
// See Also:
//  PreFrameworkCfg.h
//
//==EDOC========================================================================
#define MXD_XML_PARSER_EXPAT_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the compilation of the deprecated XML support found in the Xml
//  folder. Deprecated XML support is disabled by default.
//
// Description:
//  This permits the compilation of the deprecated XML support found in the Xml
//  folder. When this is not defined, the deprecated XML specific implementation
//  files found in Xml are not compiled. Deprecated XML is disabled by default.
//  While one XML implementation should be enabled at all times, it is possible
//  to enable both of them at the same time.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
// See Also:
//  PreFrameworkCfg.h
//
//==EDOC========================================================================
#define MXD_XML_DEPRECATED_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the compilation of the General Quality of Service support. GQoS is
//  disabled by default. This is valid only when MXD_OS_WINDOWS is defined.
//
// Description:
//  This allows the compilation of the GQoS support located in the Network
//  folder. When this is not defined, the GQoS specific implementation located
//  in the Network is not compiled. GQoS is disabled by default. The GQoS API
//  works for layer 2 and layer 3 under Windows 2000, but only for layer 3 under
//  Windows XP SP2. Administrative rights are not needed for it to work.
//
//  Note:
//   Regression have been experienced under Windows Vista, even though Microsoft
//   states that it sill supports this API.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
// See Also:
//  PreFrameworkCfg.h
//
//==EDOC========================================================================
#define MXD_OS_WINDOWS_ENABLE_GQOS_QOS

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the compilation of the Traffic Control Quality of Service support.
//  TC is disabled by default. This is valid only when MXD_OS_WINDOWS is defined.
//
// Description:
//  This allows the compilation of the TC support located in the Network folder.
//  When this is not defined, the TC specific implementation located in the
//  Network is not compiled. TC is disabled by default. Traffic Control works
//  for layer 2 and layer 3 tagging in both Windows 2000 and Windows XP.
//  Administrative rights are required for it to work.
//
//  Note:
//   Under Windows Vista, the User Account Control needs to be disabled in order
//   to use this API.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
// See Also:
//  PreFrameworkCfg.h
//
//==EDOC========================================================================
#define MXD_OS_WINDOWS_ENABLE_TC_QOS

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Defines the maximum number of consecutive messages or timer events
//  processed in CServicingThread when there are socket events to handle.
//
// Description:
//  In CServicingThread::Activate the number of consecutive messages or timer
//  events processed while there are socket events to handle is limited to the
//  number defined through this constant if this constant is not zero.
//
//  If the constant is set to zero, all messages and timer events will be
//  processed before processing socket events.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
//==EDOC========================================================================
#define MXD_SERVICING_THREAD_MAX_CONSECUTIVE_ITERATIONS 0

//==============================================================================
//==== THREAD SCHEDULING POLICY AND PRIORITY DEFINITIONS  ======================
//==============================================================================

//<GROUP CONFIG_MACROS>
//<TITLE Thread scheduling policy and priority definitions>
//==SDOC========================================================================
//
// Summary:
//  Sets the scheduling policy and priority that is used by CThread.
//
// Description:
//  These defines provide a way to control how the scheduling works in different
//  OSs:
//
//      VxWorks:
//          Priorities in VxWorks are defined from 0 (highest) to 255 (lowest).
//
//          The following is the default scheduling priority definition for
//          VxWorks:
//              #define MXD_astVXWORKS_THREAD_SCHEDULING_INFO
//                  {
//                      {255},   /*eLOWEST */
//                      {200},   /*eLOW    */
//                      {150},   /*eNORMAL */
//                      {100},   /*eHIGH   */
//                      {55}     /*eHIGHEST*/
//                  }
//
//      Posix:
//          The scheduling policy attribute describes how the thread is
//          scheduled for execution relative to the other threads in the
//          program. A thread has one of the following scheduling policies:
//
//          - SCHED_FIFO (first-in/first-out (FIFO))
//               The highest priority thread runs until it blocks. If there is
//               more than one thread with the same priority and that priority
//               is the highest among other threads, the first thread to begin
//               running continues until it blocks. If a thread with this policy
//               becomes ready and it has a higher priority than the currently
//               running thread, then it pre-empts the current thread and begins
//               running immediately.
//
//          - SCHED_RR (round-robin (RR))
//              The highest priority thread runs until it blocks; however,
//              threads of equal priority, if that priority is the highest among
//              other threads, are time sliced (time slicing is a mechanism that
//              ensures that every thread is allowed time to execute by
//              pre-empting running threads at fixed intervals). If a thread with
//              this policy becomes ready and it has a higher priority than the
//              currently running thread, then it pre-empts the current thread
//              and begins running immediately.
//
//          - SCHED_OTHER (also known as SCHED_FG_NP)
//              All threads are time sliced. Under this policy, all threads
//              receive some scheduling regardless of priority. Therefore,
//              no thread is completely denied execution time. Nevertheless,
//              higher priority of another scheduling policy can receive more
//              execution time. Threads with the default scheduling policy
//              can be denied execution time by FIFO or RR threads.
//
//              The thread priority MUST be zero at all times for SCHED_OTHER.
//              Setting any other thread priority will result in the thread
//              being created with a thread scheduling policy equal to 0.
//              Refer to POSIX documentation for more details.
//
//          The following is the default scheduling priority definition for
//          Linux:
//  <CODE>
//              #define MXD_astPOSIX_THREAD_SCHEDULING_INFO
//                  {
//                       {SCHED_OTHER, 0},  /*eLOWEST */
//                       {SCHED_OTHER, 0},  /*eLOW    */
//                       {SCHED_OTHER, 0},  /*eNORMAL */
//                       {SCHED_OTHER, 0},  /*eHIGH   */
//                       {SCHED_OTHER, 0}   /*eHIGHEST*/
//                  }
//  </CODE>
//
//          The following is the default scheduling priority definition for
//          Nucleus:
//  <CODE>
//              #define MXD_astPOSIX_THREAD_SCHEDULING_INFO
//                  {
//                      {SCHED_RR, 255},  /*eLOWEST */
//                      {SCHED_RR, 200},  /*eLOW    */
//                      {SCHED_RR, 150},  /*eNORMAL */
//                      {SCHED_RR, 100},  /*eHIGH   */
//                      {SCHED_RR, 55}    /*eHIGHEST*/
//                  }
//  </CODE>
//
//          The following is the default scheduling priority definition for
//          Windows:
//  <CODE>
//              #define MXD_astWINDOWS_THREAD_SCHEDULING_INFO
//                  {
//                      {THREAD_PRIORITY_LOWEST},            /*eLOWEST */
//                      {THREAD_PRIORITY_BELOW_NORMAL},      /*eLOW    */
//                      {THREAD_PRIORITY_NORMAL},            /*eNORMAL */
//                      {THREAD_PRIORITY_ABOVE_NORMAL},      /*eHIGH   */
//                      {THREAD_PRIORITY_HIGHEST}            /*eHIGHEST*/
//                  }
//  </CODE>
//
// Warning:
//  Posix threads under Linux: as SCHED_FIFO and SCHED_RR processes can pre-empt
//  other processes forever, only root processes are allowed to activate these
//  policies.
//
//-- The backslashes in the following paragraph are required to prevent
//-- doc-o-matic autolinking the TO word to the TO macro defined elsewhere in
//-- the Framework.
//
//  FOR ALL FRAMEWORK VERSIONS GREATER THAN 2.0.7.11, THE DEFAULT POSIX THREAD
//  SCHEDULER UNDER THE LINUX OPERATING SYSTEM IS DEFINED \TO "SCHED_OTHER". IF
//  A USER WANTS \TO HAVE THE EXACT SAME BEHAVIOUR AND PERFORMANCE AS BEFORE, THE
//  MXD_astPOSIX_THREAD_SCHEDULING_INFO MUST BE SET \TO THE "ROUND-ROBIN" THREAD
//  POLICY AS IN THE FOLLOWING EXAMPLE:
//
//  <CODE>
//              #define MXD_astPOSIX_THREAD_SCHEDULING_INFO
//                  {
//                      {SCHED_RR, 10},     /*eLOWEST */
//                      {SCHED_RR, 30},     /*eLOW    */
//                      {SCHED_RR, 50},     /*eNORMAL */
//                      {SCHED_RR, 70},     /*eHIGH   */
//                      {SCHED_RR, 90}      /*eHIGHEST*/
//                  }
//  </CODE>
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default behaviour.
//
//==EDOC========================================================================
#define MXD_astVXWORKS_THREAD_SCHEDULING_INFO

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_astVXWORKS_THREAD_SCHEDULING_INFO>
#define MXD_astPOSIX_THREAD_SCHEDULING_INFO

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_astVXWORKS_THREAD_SCHEDULING_INFO>
#define MXD_astWINDOWS_THREAD_SCHEDULING_INFO

// BRCM_CUSTOM - [begin] - Add ECOS support.
//<COMBINE MXD_astVXWORKS_THREAD_SCHEDULING_INFO>
#define MXD_astECOS_THREAD_SCHEDULING_INFO
// BRCM_CUSTOM - [end] - Add ECOS support.

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Allows the addition of scheduling priorities.
//
// Description:
//  Allows the addition of scheduling priorities. By default, the following
//  scheduling priorities are defined in CThread.h:
//
//  <CODE>
//      enum EPriority
//      {
//          eLOWEST,
//          eLOW,
//          eNORMAL,
//          eHIGH,
//          eHIGHEST,
//          MXD_THREAD_PRIORITY_EXTENSION
//          eTHREAD_PRIORITY_ENUM_SIZE
//      };
//  </CODE>
//
//  These scheduling priorities are used in the calls to the
//  CThread::StartThread method. eTHREAD_PRIORITY_ENUM_SIZE is only used to
//  indicate the number of scheduling priorities. eTHREAD_PRIORITY_ENUM_SIZE
//  must never be used when calling the CThread::StartThread method (or any
//  other method with a CThread::EPriority parameter).
//
//  By default, in FrameworkCfg.h, MXD_THREAD_PRIORITY_EXTENSION is defined to
//  nothing. The default scheduling priorities are thus:
//
//  <CODE>
//      enum EPriority
//      {
//          eLOWEST,
//          eLOW,
//          eNORMAL,
//          eHIGH,
//          eHIGHEST,
//          eTHREAD_PRIORITY_ENUM_SIZE
//      };
//  </CODE>
//
//  It is possible to define additional scheduling priorities by using
//  MXD_THREAD_PRIORITY_EXTENSION. For example, if MXD_THREAD_PRIORITY_EXTENSION
//  is defined as:
//
//  <CODE>
//      #define MXD_THREAD_PRIORITY_EXTENSION eAVERAGE, eCRITICAL,
//  </CODE>
//
//  The scheduling priorities are then:
//
//  <CODE>
//      enum EPriority
//      {
//          eLOWEST,
//          eLOW,
//          eNORMAL,
//          eHIGH,
//          eHIGHEST,
//          eAVERAGE,
//          eCRITICAL,
//          eTHREAD_PRIORITY_ENUM_SIZE
//      };
//  </CODE>
//
//  Note the comma after each additional scheduling. Each additional scheduling
//  priority must be followed by a comma.
//
//  If needed, MXD_THREAD_PRIORITY_EXTENSION must be redefined in
//  PreFrameworkCfg.h. When redefining MXD_THREAD_PRIORITY_EXTENSION, the
//  following defines must also be redefined in PreFrameworkCfg.h:
//
//      - MXD_astVXWORKS_THREAD_SCHEDULING_INFO
//      - MXD_astPOSIX_THREAD_SCHEDULING_INFO
//      - MXD_astWINDOWS_THREAD_SCHEDULING_INFO
//
//  FrameworkCfg.h contains the default definition for these defines. Refer to
//  these definitions as a guideline on how to define these defines.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default behaviour.
//
//==============================================================================
#define MXD_THREAD_PRIORITY_EXTENSION

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables stack usage information.
//
// Description:
//  Enables support for detecting stack usage in CThread objects.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_THREAD_STACK_INFO_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Configures the default stack size for a new thread created with CThread.
//
// Description:
//  When this macro is defined, it should specify the number of bytes that
//  should form the stack of a new thread when it is not specified in the call
//  to StartThread.
//
//  This parameter is used only on VxWorks and Windows and when
//  MXD_THREAD_FIXED_STACK_SIZE_ENABLE_SUPPORT is defined. It has no effect in
//  other cases
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
// See Also:
//  CThread::StartThread
//
//==EDOC========================================================================
#define MXD_DEFAULT_THREAD_STACK_SIZE

//==============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Forces the stack size of new threads to be fixed.
//
// Description:
//  When this macro is defined, a fixed stack size will be specified for each
//  new thread, and the autogrow feature will be disabled.
//
//  This parameter is used only on Linux and Solaris. It has no effects on
//  other operating systems.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
// See Also:
//  CThread::StartThread
//
//==============================================================================
#define MXD_THREAD_FIXED_STACK_SIZE_ENABLE_SUPPORT

//==============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Configures the stack size for the VxTaskTerm under VxWorks.
//
// Description:
//  When this macro is defined, it specifies the size of the stack, in
//  bytes, of the VxTaskTerm thread. This thread is started automatically on
//  VxWorks when calling CFrameworkInitializer::Initialize().
//
//  This parameter is used only on VxWorks.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==============================================================================
#define MXD_VXTASKTERM_STACK_SIZE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Configures the default value of the buffer offset when stack usage
//  information is enabled.
//
// Description:
//  When this macro is defined, it should specify the number of bytes that are
//  substracted from the specified stack size when filling the stack with a
//  known pattern. Let the following be a diagram of the stack of a CThread:
//
//  <CODE>
//  |--------------|  <- 1. End of stack
//  | offset       |
//  |--------------|  <- 2. Maximum stack usage detection threshold
//  | Filled with  |
//  | a known      |
//  | pattern      |
//  |--------------|  <- 3. Point in the stack where the operating system gives
//  | OS specific  |        back control in CThread::ThreadEntry.
//  |--------------|  <- 4. Start of stack
//  </CODE>
//
//  The total stack size of the thread is 1-4, but one cannot know exactly
//  how much stack is used once the code gets in CThread::ThreadEntry because
//  the operating system uses some of the stack for different purposes.
//  Therefore, you need to act as if the stack was smaller to avoid buffer
//  overflow while filling with the known pattern. The define
//  MXD_DEFAULT_THREAD_STACK_INFO_BUFFER_OFFSET tells how much smaller the
//  stack is.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
// See Also:
//  MXD_THREAD_STACK_INFO_ENABLE_SUPPORT
//
//==EDOC========================================================================
#define MXD_DEFAULT_THREAD_STACK_INFO_BUFFER_OFFSET 256

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables IPv6 support.
//
// Description:
//  Enables support for IPv6.  This is currently implemented for the following
//  OSes:
//      - Linux 2.4 and up
//      - Windows XP SP2 and up (including 2003 server)
//      - Windows CE 4.1 and up
//
//  You must manually enable IPv6 support in your OS before you can use this
//  define. For instructions on how to enable IPv6 in your OS, see the following
//  links:
//      - Windows (all versions):
//        http://www.microsoft.com/technet/itsolutions/network/ipv6/ipv6faq.mspx
//      - Linux (all versions):
//        http://tldp.org/HOWTO/Linux+IPv6-HOWTO/
//
//  It is also recommended that you familiarize yourself with the following RFCs:
//      - RFC 2460: Internet Protocol, Version 6 (IPv6) Specification
//      - RFC 2462: IPv6 Stateless Address Autoconfiguration
//      - RFC 3041: Extensions to IPv6 Address Autoconfiguration
//      - RFC 3513: IPv6 Addressing Architecture
//      - RFC 3596: DNS Extensions to Support IP Version 6
//      - RFC 3879: Deprecating Site Local Addresses
//      - RFC 4007: IPv6 Scoped Address Architecture
//      - RFC 4038: Application Aspects of IPv6 Transition
//      - RFC 4193: Unique Local IPv6 Unicast Addresses
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_IPV6_ENABLE_SUPPORT

//==============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Always specifies IPv6 scope.
//
// Description:
//  Always specifies the scope of an IPv6 address. The scope is explicitly
//  specified regardless of the type of the IPv6 address.
//
//  The scope is specified following RFC 4007 specification.
//
//  MXD_IPV6_ENABLE_SUPPORT MUST be defined to use this macro.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==============================================================================
#define MXD_IPV6_SCOPE_ID_MANDATORY_IN_ALL_ADDRESSES

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables support for regular expressions.
//
// Description:
//  Enables support for regular expressions in the Framework.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_REGEXP_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables support for ENUM.
//
// Description:
//  Enables support for ENUM in the Framework, as defined in RFC 3761.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_ENUM_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Configures the maximal number of non-terminal NAPTRs in an single ENUM
//  request.
//
// Description:
//  This define specifies the maximum number of non-terminal NAPTRs to lookup
//  in a single ENUM request. Setting it to 0 effectively disables support for
//  non-terminal NAPTRs. The default value is 5, as suggested in the
//  draft-ietf-enum-experiences-05.txt document.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_ENUM_NAPTR_MAX_NON_TERMINAL 5

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//      Enables the native support for atomic operations. By default, native
//      atomic operations support is disabled.
//
// Description:
//      When this preprocessor macro is defined, native atomic operations are
//      enabled in the CAtomic.h classes. If this macro is defined, and
//      native atomic operations is not currently supported on the configured
//      platform, a compile time error is generated. Refer to the
//      CAtomicOperations class for more information on which operations are
//      supported on which platform.
//
// Location:
//      Define this in "PreFrameworkCfg.h" or in your makefile for special
//      builds.
//
// See Also
//      CAtomicOperations
//
//==EDOC========================================================================
#define MXD_ATOMIC_NATIVE_ENABLE_SUPPORT

//==============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the compilation of the Network support found in the Network folder.
//  Network is disabled by default.
//
// Description:
//  This permits the compilation of the Network support found in the Network
//  folder. This folder includes support for UDP and TCP synchonous and
//  asynchronous sockets, as well as name resolution support.
//  When this is not defined, the Network specific implementation files
//  found in the Network folder are not compiled. Network is disabled by
//  default.
//
// Location:
//  Define this in PreFrameworkCfg.h to change the default value.
//
//==============================================================================
#define MXD_NETWORK_ENABLE_SUPPORT


//==============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the Cap sub-allocator feature.
//
// Description:
//  The Cap package offers a sub-allocator. This sub-allocator pre-allocates
//  a big memory block of
//  MXD_CAP_SUBALLOCATOR_DEFAULT_MEMORY_BLOCK_SIZE_IN_BYTES or bigger and then
//  sub-allocates memory within that memomry block. When memory blocks are
//  filled, new ones are created.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==============================================================================
#define MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT

//==============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the Cap sub-allocator statistics feature.
//
// Description:
//  The Cap package offers a sub-allocator. This sub-allocator computes a few
//  statistics that can be printed to screen. These statistics are tailored to
//  help optimizing the MXD_CAP_SUBALLOCATOR_DEFAULT_MEMORY_BLOCK_SIZE_IN_BYTES
//  value for the specific system in which it is used.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==============================================================================
#define MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT

//==============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Defines the default size of the sub-allocator's memory blocks.
//
// Description:
//  This macro specifies the number of bytes that the sub-allocator must
//  minimally allocate when it needs a new memory block. This defaults to
//  2048 bytes.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==============================================================================
#define MXD_CAP_SUBALLOCATOR_DEFAULT_MEMORY_BLOCK_SIZE_IN_BYTES 2048

//==============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Configures the connect timeout of an asynchronous TCP socket.
//
// Description:
//  This configures a timeout value in milliseconds for an asynchronous TCP
//  socket connect operation. If the OS timer is smaller than this value, it has
//  priority.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==============================================================================
#define MXD_NETWORK_ASYNCSOCKET_CONNECT_TIMEOUT_MS

//==============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Defines the minimal alignment in bytes that must be used by the Framework.
//
// Description:
//  This macro defines the minimal alignment in bytes that must be used by the
//  Framework and by other application components. For example, this macro is
//  used by the Framework memory allocator to ensure that allocated memory is
//  always aligned adequately. Failing to do so and depending on the target
//  architecture, unaligned memory accesses may result in notable performance
//  degradation or even a crash. The default value is set to 4 for 32 bits
//  platforms and to 8 for 64 bits platforms.
//
// Location:
//  Define this in PreFrameworkCfg.h.
//
//==============================================================================
#define MXD_MINIMAL_ALIGNMENT_IN_BYTES 4

//==============================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the support to automatically get the system time upon creation of a
//  CTime class instance.
//
// Description:
//  Enables the support to automatically get the system time upon creation of a
//  CTime class instance.
//
//  When this macro is defined, CTime class instances will always get the
//  system time from the system automatically instead of using the internal
//  Framework time base.
//
//  This macro is only supported on LINUX, WINDOWS and VXWORKS.
//  A compilation error will be reported for unsupported platforms.
//
// Location:
//  Define this in "PreFrameworkCfg.h" or in your makefile for special builds.
//
//==============================================================================
#define MXD_CTIME_USE_SYSTEM_TIME_ENABLE_SUPPORT

#endif // #if 0

#endif //-- #ifndef MXG_FRAMEWORKCFG_H

