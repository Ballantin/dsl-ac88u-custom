/*****************************************************************************
* Copyright 2006 - 2007 Broadcom.  All rights reserved.
*
* This program is the proprietary software of Broadcom and/or
* its licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY
* WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF
* THE SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
* 1. This program, including its structure, sequence and organization,
*    constitutes the valuable trade secrets of Broadcom, and you shall use
*    all reasonable efforts to protect the confidentiality thereof, and to
*    use this information only in connection with your use of Broadcom
*    integrated circuit products.
* 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
*    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
*    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*    IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS
*    FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS,
*    QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU
*    ASSUME THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
* 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
*    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
*    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
*    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
*    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
*    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*****************************************************************************/



#ifndef MXG_PREMXCONFIG_H
#define MXG_PREMXCONFIG_H

#if 0
#include <cctkCfg.h>
#else
#define CCTKCFG_TRACE_ALL 1
#define CCTKCFG_TRACE_MODE CCTKCFG_TRACE_ALL
#define CCTKCFG_ASSERT_ALL 1
#define CCTKCFG_ASSERT_MODE CCTKCFG_ASSERT_ALL
#define CCTKCFG_MEMORY_MONITORING 1
#define CCTKCFG_DEBUG 1
#endif

/* OS configuration */
#if defined( MXD_OS_VXWORKS )
   /* VxWorks configuration */
   #define MXD_COMPILER_GNU_GCC
   #if !defined( MXD_VXWORKS_VERSION )
      #error VXWORKS VERSION not defined
   #endif
   #define MXD_ARCH_MIPS
   #define MXD_BIG_ENDIAN
#elif defined( MXD_OS_BOS_ECOS )
   /* eCos configuration */
   #define MXD_COMPILER_GNU_GCC
   #define MXD_ARCH_MIPS
   #define MXD_BIG_ENDIAN
#endif

#if defined( MXD_OS_WINDOWS ) || defined( _WIN32 )
   #define MXD_WINDOWS_VERSION                     0x0500
#endif

/* Tracing support */
#undef MXD_TRACE_ENABLE_ALL
#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
   #define MXD_TRACE_ENABLE_ALL
   #define MXD_TRACE_MAX_NB_OF_OUTPUT_HANDLERS     2
   #define MXD_TRACE_BUFFER_CAPACITY               2048
#endif

/* Assert support */
#undef MXD_ASSERT_ENABLE_ALL
#if ( CCTKCFG_ASSERT_MODE == CCTKCFG_ASSERT_ALL )
   #define MXD_ASSERT_ENABLE_ALL
#else
   #define MXD_DISABLE_EXTERNAL_ASSERT_OVERRIDE
#endif
#define MXD_ASSERT_CALL_STACK_TRACE_DISABLE

/* Memory monitoring support */
#undef MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT
#undef MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT
#if CCTKCFG_MEMORY_MONITORING
   #define MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT
   #define MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT

   #undef MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT
   #undef MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT
   #undef MXD_BRCM_MEMORY_ALLOCATOR_WRAPPER_SUPPORT
   #if CCTKCFG_DEBUG
      #define MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT
      #define MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT
      #define MXD_BRCM_MEMORY_ALLOCATOR_WRAPPER_SUPPORT
   #endif
#endif

/* Package ID override */
#define MXD_PKG_ID_OVERRIDE
typedef enum
{
   eMX_PKG_NONE = 0,                   /* -- 0, Reserved for future use */
   eMX_PKG_ASSERTION = 1,              /* -- 1 Reserved for the MxAssert module. */
   eMX_PKG_MITOSFW,                    /* -- 2 - Framework */
   eMX_PKG_RTP,                        /* -- 3 - RTP */
   eMX_PKG_SRTP,                       /* -- 4 */
   eMX_PKG_SIP,                        /* -- 5 */
   eMX_PKG_SIP_PARSER,                 /* -- 6 */
   eMX_PKG_SIPTRANSPORT,               /* -- 7 */
   eMX_PKG_SIPTRANSACTION,             /* -- 8 */
   eMX_PKG_SIPCORE,                    /* -- 9 */
   eMX_PKG_SIPCORESVC,                 /* -- 10 */
   eMX_PKG_SIPUSERAGENT,               /* -- 11 */
   eMX_PKG_SIP_PROXY,                  /* -- 12 */
   eMX_PKG_MIX,                        /* -- 13 */
   eMX_PKG_MIX_SNMP_AGENT_ADAPTOR,     /* -- 14 */
   eMX_PKG_MIX_PERSISTENCE_MANAGER,    /* -- 15 */
   eMX_PKG_STUN,                       /* -- 16 */
   eMX_PKG_TFW,                        /* -- 17 */
   eMX_PKG_MIKEY,                      /* -- 18 */
   eMX_PKG_SIGCOMP,                    /* -- 19 */
   eMX_PKG_SIPUACOMPONENTS,            /* -- 20 */
   eMX_PKG_SIPSERVERCOMPONENTS,        /* -- 21 */
   eMX_PKG_SDP,                        /* -- 22 */
   eMX_PKG_MTEI,                       /* -- 23 */
   eMX_PKG_COMMAND_DIGITS_INTERFACE,   /* -- 24 */
   eMX_PKG_FXSCALLMANAGER,             /* -- 25 */
   eMX_PKG_UASSPINTEGRATION,           /* -- 26 */
   eMX_PKG_UASSP,                      /* -- 27 */
   eMX_PKG_TR_111_PART_2_SERVER,       /* -- 28 */
   eMX_PKG_ZRTP,                       /* -- 29 */

   eMX_PKG_CCTK,                       /* -- 30 */

   eMX_PKG_SIZE,                       /* -- This one should follow all package 
                                       **    designations.
                                       */
   eMX_PKG_ALL = eMX_PKG_SIZE
} EMxPackageId;

/* More configuration in PostMxConfig.h */
#define MXD_POST_CONFIG

/* Other configurations */
#define MXD_ENABLE_NAMESPACE

#endif // #ifndef MXG_PREMXCONFIG_H
