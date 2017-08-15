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
#ifndef MXG_ISIPOUTBOUNDCONNECTIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPOUTBOUNDCONNECTIONMGR_H
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


#ifndef MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT to be able to use \
this class.
#endif

#ifndef MXG_CSIPTRANSPORTOOLS_H
#include "SipTransport/CSipTransportTools.h" // ESipTransport
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipPacket;
class CSocketAddr;
class ISipOutboundConnectionSvc;
class ISipClientEventControl;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipOutboundConnectionMgr
//==============================================================================
// <FLAG New in 4.1.6>
//<GROUP SIPCORESVC_CLASSES>
//
// Description:
//   This is the outbound connection manager. It is used by the stack to inform
//   the application when another peer requires the outbound extension. It
//   reports REGISTER responses that contain the Require header with the
//   'outbound' value. It is also used by the stack to request the application
//   to create a persistent connection.
//
// Location:
//   SipCoreSvc/ISipOutboundConnectionMgr.h
//
// See Also:
//   ISipOutboundConnectionSvc
//
//==============================================================================
class ISipOutboundConnectionMgr
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  EvOutboundRequiredByPeer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells the application that the peer supports outbound.
    //
    //  Parameters:
    //      pSvc:
    //       The outbound connection service.
    //
    //    pClientEventCtrl:
    //      The interface to the client event control for this transaction.
    //
    //      rPacket:
    //       The REGISTER response containing the Require:outbound header.
    //
    //      eTransport:
    //       The transport of the connection.
    //
    //      rLocalAddr:
    //       The local address of the connection.
    //
    //      rPeerAddr:
    //       The peer address of the connection.
    //
    //      uFlowTimer:
    //       The value of the Flow-Timer header. Can be zero if not present in
    //       the REGISTER response.
    //
    //  Description:
    //      Informs the application that a REGISTER 2xx response containing the
    //      Require header with 'outbound' value has been received. This usually
    //      means that the application can start sending keep alives to the peer
    //      when this event is reported. Depending on which transport the
    //      flow is for, the application can either start the
    //      CRLF or STUN keep alives.
    //
    //      CRLF keep alives can be started by the application using the
    //      StartKeepAlive method on the persistent connection list. The stack
    //      does not do any STUN keep alive on itself, it is the responsibility
    //      of the application. As such, the application can use the Send method
    //      on the persistent connection list and the ISipKeepAliveExtensionMgr.
    //
    //  See also:
    //      CSipPersistentConnectionList::StartKeepAlive,
    //      CSipPersistentConnectionList::Send,
    //      ISipKeepAliveExtensionMgr
    //
    //==========================================================================
    virtual void EvOutboundRequiredByPeer(IN ISipOutboundConnectionSvc* pSvc,
                                          IN ISipClientEventControl* pClientEventCtrl,
                                          IN const CSipPacket& rPacket,
                                          IN ESipTransport eTransport,
                                          IN const CSocketAddr& rLocalAddr,
                                          IN const CSocketAddr& rPeerAddr,
                                          IN unsigned int uFlowTimer) = 0;

    // Offers the flexibility to create or not a persistent connection.
    enum EConnectionCreation
    {
        // Used when the application wants to create a flow asynchronously.
        eCREATE_CONNECTION_ASYNCHRONOUSLY,
        // Used when the application does not want to create a flow.
        eDO_NOT_CREATE_CONNECTION
    };

    //==========================================================================
    //==
    //==  EvConnectionNeeded
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells that the stack requests a flow to be created.
    //
    //  Parameters:
    //      pSvc:
    //       The outbound connection service.
    //
    //      opqId:
    //       An opaque value that must be passed to the
    //       ISipOutboundConnectionSvc::ConnectionCreationResult method when a
    //       connection is asynchronously created. It is mandatory that the
    //       SAME opaque value is passed with the corresponding connection
    //       creation.
    //
    //      rRequest:
    //       The request that triggered the persistent connection creation.
    //
    //      eTransport:
    //       The transport of the connection.
    //
    //      rLocalAddr:
    //       The local address of the connection.
    //
    //      rPeerAddr:
    //       The peer address of the connection.
    //
    //      reConnectionCreation:
    //       eCREATE_CONNECTION_ASYNCHRONOUSLY:
    //          Must be set to this value if the application wants to create a
    //          flow. When the application uses this value, it must not call the
    //          method ConnectionCreationResult on the outbound service.
    //
    //       eDO_NOT_CREATE_CONNECTION:
    //          Must be set to this value if this application does not want to
    //          create a flow. In this case, the request fails to be sent.
    //
    //  Description:
    //      Informs the application that a flow related to outbound needs to be
    //      created. It is possible that some flows are created with the
    //      assistance of the application when issuing an outbound request for
    //      which there is no persistent connection. This is to be compliant as
    //      per draft-ietf-sip-outbound-15 section 4.3 when sending non-REGISTER
    //      requests.
    //
    //      This event can be called only when the outbound service is attached
    //      after the server location service. It can happen when an application
    //      sends a request other than REGISTER for which there is no existing
    //      flow. The application can decide when this event is reported to
    //      create or not a connection. The connection should be created using
    //      the CSipPersistentConnectionList::Establish method.
    //
    //      Before calling the ConnectionCreationResult method on the outbound
    //      service, the application must wait for the EvEstablished or
    //      EvErrorOnConnection event on the ISipPersistentConnectionList.
    //
    //  See also:
    //      ISipOutboundConnectionSvc::ConnectionCreationResult
    //
    //==========================================================================
    virtual void EvConnectionNeeded(IN ISipOutboundConnectionSvc* pSvc,
                                    IN mxt_opaque opqId,
                                    IN const CSipPacket& rRequest,
                                    IN ESipTransport eTransport,
                                    IN const CSocketAddr& rLocalAddr,
                                    IN const CSocketAddr& rPeerAddr,
                                    OUT EConnectionCreation& reConnectionCreation) = 0;


//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipOutboundConnectionMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISipOutboundConnectionMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipOutboundConnectionMgr(IN const ISipOutboundConnectionMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipOutboundConnectionMgr& operator=(IN const ISipOutboundConnectionMgr& rSrc);


//-- Hidden Data Members
protected:
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPOUTBOUNDCONNECTIONMGR_H
