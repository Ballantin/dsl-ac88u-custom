//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPCONNECTIONREUSESVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPCONNECTIONREUSESVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#ifndef MXD_SIPSTACK_ENABLE_SIP_CONNECTION_REUSE_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_CONNECTION_REUSE_SVC_SUPPORT to be able to use \
this class.
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" //-- ESipTransport
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSocketAddr;
//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipConnectionReuseSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipConnectionReuseSvc
//==============================================================================
//<GROUP SIPCORESVC_CLASSES>
//
// Description:
//  This connection service allows the application to reuse a connected socket
//  for sending requests that was created from an incoming request. This service
//  is used to limit the creation of new connections to the peer that sent a
//  request.
//
//  This is to reuse the initial TLS connection created by a peer when sending
//  requests inside the same context. The service sends the request to the 
//  address of the last received request. If this service is not used and the
//  contact info of the peer is not set properly, the SIP stack will create
//  a new connection to the peer.
//
//  This service MUST NOT be used with the ISipServerLocationSvc. The
//  ISipServerLocationSvc will effectively override what this service does.
//  It will also conflict with the ISipPersistentConnectionSvc. The last service 
//  added will have precedence over the first one as they both manipulate the
//  same data.
//
//  Please note that if this service is attached, there will be no fallback
//  tried by the SIP stack.
//
//  Another use of this service can be to set the local and peer addresses to be
//  used for sending outgoing requests from this service using the
//  SetLocalAddress and the SetPeerAddress methods. These can be retrieved
//  from another such service using the GetLocalAddress and GetPeerAddress
//  methods.
//
//#The ISipConnectionReuseSvc is an ECOM object#
//
//  The ISipConnectionReuseSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipConnectionReuseSvc</B>
//
//  <B>Interface Id: IID_ISipConnectionReuseSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipConnectionReuseSvc.h
//
//==============================================================================
class ISipConnectionReuseSvc : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipConnectionReuseSvc);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetLocalAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the local address.
    //
    //  Parameters:
    //      rLocalAddress:
    //       The local address.
    //
    //  Description:
    //      Sets the local address to be used to send a request to a peer.
    //
    //  NOTES:
    //      Both the peer and the local address must be set to a valid value
    //      and the transport to modify the outgoing packet.
    //
    //==========================================================================
    virtual void SetLocalAddress(IN const CSocketAddr& rLocalAddress) = 0;

    //==========================================================================
    //==
    //==  SetPeerAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the peer address.
    //
    //  Parameters:
    //      rPeerAddress:
    //       The peer address.
    //
    //  Description:
    //      Sets the peer address to be used to send a request to a peer.
    //
    //  NOTES:
    //      Both the peer and the local address must be set to a valid value
    //      and the transport to modify the outgoing packet.
    //
    //==========================================================================
    virtual void SetPeerAddress(IN const CSocketAddr& rPeerAddress) = 0;

    //==========================================================================
    //==
    //==  SetTransport
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the transport.
    //
    //  Parameters:
    //      eTransport:
    //       The transport.
    //
    //  Description:
    //      Sets the transport for this service to use.
    //
    //  NOTES:
    //      Both the peer and the local address must be set to a valid value
    //      and the transport to modify the outgoing packet.
    //
    //==========================================================================
    virtual void SetTransport(IN ESipTransport eTransport) = 0;

    //==========================================================================
    //==
    //==  GetLocalAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the local address.
    //
    //  Returns:
    //      The local address.
    //
    //  Description:
    //      Gets the local address to be used to send a request to a peer.
    //
    //==========================================================================
    virtual const CSocketAddr& GetLocalAddress() = 0;

    //==========================================================================
    //==
    //==  GetPeerAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the peer address.
    //
    //  Returns:
    //      The peer address.
    //
    //  Description:
    //      Gets the peer address to be used to send a request to a peer.
    //
    //==========================================================================
    virtual const CSocketAddr& GetPeerAddress() = 0;

    //==========================================================================
    //==
    //==  GetTransport
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the transport.
    //
    //  Parameters:
    //      eTransport:
    //       The transport.
    //
    //  Description:
    //      Gets the transport for this service.
    //
    //==========================================================================
    virtual ESipTransport GetTransport() = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    ISipConnectionReuseSvc() {}
    virtual ~ISipConnectionReuseSvc() {}
//M5T_INTERNAL_USE_END
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCONNECTIONREUSESVC_H
