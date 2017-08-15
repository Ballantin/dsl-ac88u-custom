//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_PKGID_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_PKGID_H
//M5T_INTERNAL_USE_END

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#if !defined(MXD_PKG_ID_OVERRIDE)

//==SDOC========================================================================
//<GROUP BASIC_ENUMS>
//
// Summary:
//  Defines the possible packages for the Package ID of the mxt_result type.
//
// Description:
//  This enumeration defines the possible packages with result values throughout
//  the M5T offered products.
//
//  The values in this enumeration can not form a bit mask, but are instead
//  sequential values.
//
//  The package IDs are meant to be augmented by the user. The application must
//  define MXD_PKG_ID_OVERRIDE if it wants to reuse the result mechanism for its
//  own functions/methods.
//
//  Location:
//    Defined in Basic/PkgId.h but must include Config/MxConfig.h to
//    access this.
//
// See Also:
//  MXD_PKG_ID_OVERRIDE
//
//==EDOC========================================================================
typedef enum
{
    // Description:
    // First package ID. Reserved for future use.
    //-- 0, Reserved for future use
    eMX_PKG_NONE = 0,
    // Description:
    // Package ID for the assertion mechanism.
    //-- 1 Reserved for the MxAssert module.
    eMX_PKG_ASSERTION = 1,
    // Description:
    // Package ID for the Framework package.
    //-- 2 - Framework
    eMX_PKG_MITOSFW,
    // Description:
    // Package ID for the RTP package.
    //-- 3 - RTP
    eMX_PKG_RTP,
    // Description:
    // Package ID for the SRTP package.
    //-- 4
    eMX_PKG_SRTP,
    // Description:
    // Package ID for the SIP package.
    //-- 5
    eMX_PKG_SIP,
    // Description:
    // Package ID for the SIP parser package.
    //-- 6
    eMX_PKG_SIP_PARSER,
    // Description:
    // Package ID for the SIP transport package.
    //-- 7
    eMX_PKG_SIPTRANSPORT,
    // Description:
    // Package ID for the SIP transaction package.
    //-- 8
    eMX_PKG_SIPTRANSACTION,
    // Description:
    // Package ID for the SIP core package.
    //-- 9
    eMX_PKG_SIPCORE,
    // Description:
    // Package ID for the SIP core SVC package.
    //-- 10
    eMX_PKG_SIPCORESVC,
    // Description:
    // Package ID for the SIP user agent package.
    //-- 11
    eMX_PKG_SIPUSERAGENT,
    // Description:
    // Package ID for the SIP proxy package.
    //-- 12
    eMX_PKG_SIP_PROXY,
    // Description:
    // Package ID for the MIX package.
    //-- 13
    eMX_PKG_MIX,
    // Description:
    // Package ID for the MIX SNMP agent adaptor package.
    //-- 14
    eMX_PKG_MIX_SNMP_AGENT_ADAPTOR,
    // Description:
    // Package ID for the MIX persistence manager package.
    //-- 15
    eMX_PKG_MIX_PERSISTENCE_MANAGER,
    // Description:
    // Package ID for the STUN package.
    //-- 16
    eMX_PKG_STUN,
    // Description:
    // Package ID for the TestFramework package.
    //-- 17
    eMX_PKG_TFW,
    // Description:
    // Package ID for the Mikey package.
    //-- 18
    eMX_PKG_MIKEY,
    // Description:
    // Package ID for the Sigcomp package.
    //-- 19
    eMX_PKG_SIGCOMP,
    // Description:
    // Package ID for the SIP UA components package.
    //-- 20
    eMX_PKG_SIPUACOMPONENTS,
    // Description:
    // Package ID for the SIP server components package.
    //-- 21
    eMX_PKG_SIPSERVERCOMPONENTS,
    // Description:
    // Package ID for the SDP package.
    //-- 22
    eMX_PKG_SDP,
    // Description:
    // Package ID for the MTEI.
    //-- 23
    eMX_PKG_MTEI,
    // Description:
    // Package ID for the Command Digits Interface.
    //-- 24
    eMX_PKG_COMMAND_DIGITS_INTERFACE,
    // Description:
    // Package ID for the FXS Callmanager.
    //-- 25
    eMX_PKG_FXSCALLMANAGER,
    // Description:
    // Package ID for the UA SSP Integration.
    //-- 26
    eMX_PKG_UASSPINTEGRATION,
    // Description:
    // Package ID for the UA SSP.
    //-- 27
    eMX_PKG_UASSP,
    // Description:
    // Package ID for the TR-111 part 2 server.
    //-- 28
    eMX_PKG_TR_111_PART_2_SERVER,
    // Description:
    // Package ID for the ZRTP package.
    //-- 29
    eMX_PKG_ZRTP,
    // Description:
    // Package ID for the HTTP package.
    //-- 30
    eMX_PKG_HTTP,
    // Description:
    // Package ID for the PSEM package.
    //-- 31
    eMX_PKG_PSEM,
    // Description:
    // Package ID for the ICE package.
    //-- 32
    eMX_PKG_ICE,
    // Description:
    // Indicates the size of the EMxPackageId enum.
    //-- This one should follow all package designations.
    eMX_PKG_SIZE,
    // Description:
    // Package ID representing all packages.
    eMX_PKG_ALL = eMX_PKG_SIZE
} EMxPackageId;
#endif // defined(MXD_PKG_ID_OVERRIDE)


#endif // #ifndef MXG_PKGID_H
