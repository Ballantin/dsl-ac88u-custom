//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_SOCKETOPTIONS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SOCKETOPTIONS_H
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
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_IPV6_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
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
//====  NEW METHOD PROTOTYPES  =================================================
//==============================================================================

// Summary:
//  Sets the 8021Q user priority option.
//----------------------------------------
mxt_result SetSockOpt8021QUserPriority(IN mxt_hSocket hSocket, IN bool bEnable, IN uint8_t uUserPriority);

// Summary:
//  Sets the allow any source option.
//-------------------------------------
mxt_result SetSockOptAllowAnySource(IN mxt_hSocket hSocket, IN bool bEnable);

// Summary:
//  Sets the socket operation to blocking/non-blocking.
//------------------------------------------------------
mxt_result SetSockOptBlocking(IN mxt_hSocket hSocket, IN bool bEnable);

// Summary:
//  Sets the socket linger option.
//------------------------------------------------------
mxt_result SetSockOptLinger(IN mxt_hSocket hSocket, IN bool bOnOff, IN unsigned int uLinger);

// Summary:
//  Sets the socket's broadcast option.
//--------------------------------------
mxt_result SetSockOptBroadcast(IN mxt_hSocket hSocket, IN bool bEnable);

// Summary:
//  Sets the socket's broadcast option.
//--------------------------------------
mxt_result SetSockOptKeepAliveEnable(IN mxt_hSocket hSocket, IN bool bEnable);

// Summary:
//  Enables/disables the Nagle algorithm.
//----------------------------------------
mxt_result SetSockOptNagle(IN mxt_hSocket hSocket, IN bool bEnable);

// Summary:
//  Sets the socket's receive buffer size option.
//------------------------------------------------
mxt_result SetSockOptReceiveBufferSize(IN mxt_hSocket hSocket, IN unsigned int uSize);

// Summary:
//  Sets the reuse address option.
//---------------------------------
mxt_result SetSockOptReuseAddress(IN mxt_hSocket hSocket, IN bool bEnable);

// Summary:
//  Sets the no sigpipe option.
//---------------------------------
mxt_result SetSockOptNoSigPipe(IN mxt_hSocket hSocket, IN bool bEnable);

// Summary:
//  Sets the socket's type of service ( TOS byte ).
//--------------------------------------------------
mxt_result SetSockOptTos(IN mxt_hSocket hSocket, IN uint8_t uTos);

// Summary:
//  Sets the socket's transmit buffer size option.
//-------------------------------------------------
mxt_result SetSockOptTransmitBufferSize(IN mxt_hSocket hSocket, IN unsigned int uSize);

// Summary:
//  Enables/Disables UDP checksum.
//---------------------------------
mxt_result SetSockOptUdpChecksum(IN mxt_hSocket hSocket, IN bool bEnable);

#if defined(MXD_IPV6_ENABLE_SUPPORT)
// Summary:
//  Sets the IPv6 outgoing unicast hop limit.
//--------------------------------------------
mxt_result SetSockOptIpv6UnicastHops(IN mxt_hSocket hSocket, IN int nHops);
#endif

// Summary:
//  Gets the error status of the socket.
//---------------------------------------
mxt_result GetSockOptError(IN mxt_hSocket hSocket, OUT unsigned int* puError);

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_SOCKETOPTIONS_H
