/**********************************************************************************
** Copyright © 2007-2009 Broadcom
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This file contains the pre M5T Framework configuration definitions.
**
***********************************************************************************/

#ifndef MXG_PREFRAMEWORKCFG_H
#define MXG_PREFRAMEWORKCFG_H

#include <cctkCfg.h>

/* For monitoring memory usage */
#undef MXD_EVENT_NOTIFIER_ENABLE_SUPPORT
#if CCTKCFG_MEMORY_MONITORING
   #define MXD_EVENT_NOTIFIER_ENABLE_SUPPORT
#endif

/* M5T portable resolver support */
#if defined( MXD_OS_ANDROID )
   /* Enable portable resolver because Android NetBSD resolver cannot support
   ** SRV/NAPTR queries.
   */
   #define MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT
#endif

/* TLS support */
#if CCTKCFG_TLS
   /* For Vxworks 6.3, use OpennSSL in PCD 3.3 */
   #if ( defined( MXD_OS_VXWORKS ) && ( MXD_VXWORKS_VERSION >= 0x060300 ) ) ||     \
       defined( MXD_OS_BOS_ECOS )
      #define MXD_CRYPTO_AES_MITOSFW
      #define MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL
      #define MXD_CRYPTO_MD5_MITOSFW
      #define MXD_CRYPTO_MD5MAC_OPENSSL
      #define MXD_CRYPTO_RSA_OPENSSL
      #define MXD_CRYPTO_SHA1_MITOSFW
      #define MXD_CRYPTO_SHA1MAC_MITOSFW
      #define MXD_CRYPTO_PRIVATEKEY_OPENSSL
      #define MXD_CRYPTO_PUBLICKEY_OPENSSL
      /* SecurePrng and SecureSeed are only implemented with engine MiotosFw,
      ** but are needed when using engine OpenSSL.
      */
      #define MXD_CRYPTO_SECUREPRNG_MITOSFW
      #define MXD_CRYPTO_SECURESEED_MITOSFW
      #define MXD_CRYPTO_UUIDGENERATOR_MITOSFW
      #define MXD_CRYPTO_BASE64_MITOSFW
   #else
      #define MXD_CRYPTO_ALL_OPENSSL
   #endif
   #define MXD_PKI_ENABLE_SUPPORT
   #define MXD_PKI_OPENSSL
   #define MXD_TLS_ENABLE_SUPPORT
   #define MXD_TLS_OPENSSL
#else
   #define MXD_CRYPTO_AES_NONE
   #define MXD_CRYPTO_DIFFIEHELLMAN_NONE
   #define MXD_CRYPTO_MD5_MITOSFW
   #define MXD_CRYPTO_MD5MAC_NONE
   #define MXD_CRYPTO_RSA_NONE
   #define MXD_CRYPTO_SHA1_MITOSFW
   #define MXD_CRYPTO_SHA1MAC_MITOSFW
   #define MXD_CRYPTO_SECUREPRNG_MITOSFW
   #define MXD_CRYPTO_SECURESEED_MITOSFW
   #define MXD_CRYPTO_UUIDGENERATOR_MITOSFW
   #define MXD_CRYPTO_BASE64_MITOSFW
#endif

/* IPv6 support */
#undef MXD_IPV6_ENABLE_SUPPORT
#if CCTKCFG_IPV6
   #define MXD_IPV6_ENABLE_SUPPORT
#endif

/* Tel-URI/ENUM support */
#undef MXD_REGEXP_ENABLE_SUPPORT
#undef MXD_ENUM_ENABLE_SUPPORT
#if ( CCTKCFG_TEL_MODE == CCTKCFG_TEL_ALL )
   #define MXD_REGEXP_ENABLE_SUPPORT
   #define MXD_ENUM_ENABLE_SUPPORT
#endif

#define MXD_TIME_ENABLE_SUPPORT
#define MXD_SERVICING_THREAD_ENABLE_SUPPORT
#define MXD_CAP_ENABLE_SUPPORT
#define MXD_ECOM_ENABLE_SUPPORT
#define MXD_NETWORK_ENABLE_SUPPORT
#define MXD_XML_ENABLE_SUPPORT
#define MXD_XML_PARSER_EXPAT_ENABLE_SUPPORT
#define MXD_XML_DEPRECATED_ENABLE_SUPPORT

/* --- Override default config here --- */
#if defined( MXD_OS_VXWORKS )
   #define MXD_SYSTEM_MEMORY_SIZE                  0x00010000
#endif

/* Resolver Cache Support */
#if CCTKCFG_RESOLVER_CACHE_ENABLE
   #define MXD_RESOLVER_CACHE_ENABLE_SUPPORT
#endif

#endif /* #ifndef MXG_PREFRAMEWORKCFG_H */
