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
#ifndef MXG_ISIPOUTBOUNDCONNECTIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPOUTBOUNDCONNECTIONSVC_H
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

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class ISipOutboundConnectionMgr;
template <class _Type> class CVector;
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipOutboundConnectionSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipOutboundConnectionSvc
//==============================================================================
// <FLAG New in 4.1.6>
//<GROUP SIPCORESVC_CLASSES>
//
// Description:
//  This connection service allows the application to manage outbound persistent
//  connections (flows) when acting as a user-agent entity. The service selects
//  the flow to use when sending a request and updates the SIP packets as
//  described in the draft-ietf-sip-outbound-15. The application can use this
//  service to associate flows with a context. It is possible to hold multiple
//  flows to the same domain. As such, the service allows the application to
//  specify which flow must be used when sending a request to a domain name for
//  which there are more than one flow.
//
//  The responsibilities of the service are:
//      - Add the instance-id to REGISTER Contact headers.
//      - Add the reg-id to REGISTER Contact headers.
//      - Add the 'ob' parameter to dialog forming requests such as INVITE and
//      SUBSCRIBE.
//      - Add the 'path' and 'outbound' option-tags in Supported headers.
//      - Tell the application when outbound is supported by the peer.
//      - Allow failover using another outbound connection.
//
//  Outbound mechanism offers redundancy that occurs when a user registers to
//  multiple proxies within the same domain. As an example, let's pretend that
//  there are three proxies within the domain a.com: reg1.a.com, reg2.a.com and
//  reg3.a.com. A user-agent registers to the three proxies through a persistent
//  connection established to each proxy. The address-of-record (AOR) of the
//  user is the same on all three proxies. When the user wants to place an
//  outgoing call or send a request, it sends the request to a.com. The request
//  could be routed to one of the three proxies as they all serve the same
//  domain, which is a.com. If an outbound connection to one of the Proxies is
//  down, then the next available connection could be used.
//
//  The outbound service can be attached AFTER the server location or on a
//  context for which no server location service is attached. The behavior
//  changes depending on where the service is attached. Attaching the outbound
//  service on a context without the server location service allows to bypass
//  DNS resolving and use an outbound connection directly. Attaching the service
//  after the server locator allows to use the DNS resolving before selecting an
//  outbound connection.
//
//  The service can be configured with an ordered list of persistent connections
//  to use. This list is used only when the outbound service is attached on a
//  context without the server location. Such configuration allows to bypass
//  DNS lookup and use an outbound connection directly. When outbound
//  connections are configured, the content of the target (Request-URI or top
//  Route) is ignored. The first available flow is selected and the peer address
//  of the packet is set with the peer address of the selected flow. This means
//  that sometimes a request can be sent on a flow that does not match the
//  target of the request to send. This can happen if the outbound proxy does
//  not record-route. For example let's say that the user-agent entity sends an
//  INVITE request that is routed to P1 and then P2. The first index in the flow
//  list would be P1. If P1 does not add itself as a Record-Route (but P2 does),
//  the next requests sent on the dialog will be sent to P1 but with P2 being
//  the target. When in this mode, the service also takes care of the failover.
//  If there was a failure on the outbound flow selected to send the request,
//  the next available flow will be selected. That failover is done by the
//  service. The service simply blocks the request from being sent when there is
//  no flow available (or when the entire flows in the list have been tried).
//
//  The outbound service fails to send a request when no flows list is set and
//  no server location service is attached on the context.
//
//  Attaching the service AFTER the server locator allows load balancing as the
//  DNS tells which target to try first when sending the request. In such mode,
//  the failover is handled by the server locator. The service acts as the
//  persistent connection service to the exception that it handles all
//  parameters and headers related to outbound. When in this mode, setting a
//  flow list has no effect as the list is not used at all (DNS resolving tells
//  which outbound connection to use).
//
//  If a DNS lookup is done and no persistent connection could be found in the
//  flow list, then the stack could require to create a persistent connection.
//  This is true only for non-REGISTER requests. For REGISTER requests, no
//  persistent connection creation is requested by the stack. This connection
//  creation process is necessary to be compliant to the
//  draft-ietf-sip-outbound-15 section-4.3.
//
//  The persistent connection service MUST not be attached to a context on which
//  the outbound connection service is also attached. It is also the
//  responsibility of the application to not attach the server location after
//  the outbound service. The application is responsible to add pre-loaded Route
//  headers to the requests sent through persistent connections.
//
//#The ISipOutboundConnectionSvc is an ECOM object#
//
//  The ISipOutboundConnectionSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipOutboundConnectionSvc</B>
//
//  <B>Interface Id: IID_ISipOutboundConnectionSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipOutboundConnectionSvc.h
//
// See Also:
//   ISipOutboundConnectionMgr
//
//==============================================================================
class ISipOutboundConnectionSvc : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipOutboundConnectionSvc);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the manager.
    //
    //  Parameters:
    //      pMgr:
    //       The manager. It cannot be NULL.
    //
    //  Returns:
    //      resS_OK: manager is set.
    //      resFE_INVALID_PARAM: manager is NULL.
    //
    //  Description:
    //      Configures the manager of the outbound connection service.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN ISipOutboundConnectionMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetPersistentConnectionsPreferredOrder
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the list of persistent connections to use.
    //
    //  Parameters:
    //      rvecConnections:
    //       The list of persistent connections identified through their opaque
    //       identifier.
    //
    //  Returns:
    //      resS_OK: list properly set.
    //      resFE_FAIL: the list could not be set.
    //
    //  Description:
    //      Sets the ordered list of persistent connections to use. This list is
    //      used only when the outbound connection service is attached on a
    //      context for which there is no server location service. If the
    //      outbound connection service is attached AFTER the server location
    //      service, then the list is not considered and the service does DNS
    //      resolving to find a persistent connection.
    //
    //      The first connection in the list is used to send REGISTER requests.
    //      All other connections are not considered to send REGISTER requests.
    //      The list is used to send requests other than REGISTERs and to do
    //      failover when some flows cannot be used because they are not
    //      currently connected or a failure has occurred when sending a request
    //      over a flow. Using a list allows to do redundancy.
    //
    //      The service only tries to use a connection configured in the list.
    //      When all connections in the list have been tried, the service will
    //      block packets from being sent through this service. The application
    //      will be able to send packets only when at least one connection of
    //      the list is re-connected again.
    //
    //==========================================================================
    virtual mxt_result SetPersistentConnectionsPreferredOrder(IN CVector<mxt_opaque>& rvecConnections) = 0;

    //==========================================================================
    //==
    //==  GetPersistentConnectionsPreferredOrder
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the list of persistent connections.
    //
    //  Returns:
    //      The persistent connections list.
    //
    //  Description:
    //      Gets the list of persistent connections.
    //
    //==========================================================================
    virtual const CVector<mxt_opaque>& GetPersistentConnectionsPreferredOrder() const = 0;

    //==========================================================================
    //==
    //==  ConnectionCreationResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Continues the processing to send the request over the newly created
    //      flow.
    //
    //  Parameters:
    //      opqConnection:
    //       The persistent connection identifier
    //
    //      opqId:
    //       An opaque value that must be the same as the one given by the
    //       ISipOutboundConnectionMgr::EvConnectionNeeded method event.
    //
    //      resCreation:
    //       resS_OK: The connection was created. The process of sending the
    //       request continues.
    //       resFE_FAIL: The connection failed to be created or the application
    //       does not want to create one. The request will fail to be sent.
    //
    //  Returns:
    //      resS_OK: the processing continues.
    //      resFE_FAIL: there is no persistent connection matching this opaque
    //      or the service is not waiting for a connection creation
    //
    //  Description:
    //      This method must be called when the application returned
    //      eCREATE_CONNECTION_ASYNCHRONOUSLY upon the reporting of the event
    //      ISipOutboundConnectionMgr::EvConnectionNeeded.
    //
    //      It must be called when the application has determined if the
    //      connection could be created or not. The connection has succeeded to
    //      be created when the event
    //      ISipPersistentConnectionList::EvEstablished is reported after that
    //      CSipPersistentConnectionList::Establish method has been called.
    //
    //      The connection has failed to be created when the event
    //      ISipPersistentConnectionMgr::EvErrorOnConnection is called or
    //      when the application determnined that no connection could be created
    //      for this remote destination.
    //
    //  See also:
    //      ISipOutboundConnectionMgr::EvConnectionNeeded
    //
    //==========================================================================
    virtual mxt_result ConnectionCreationResult(IN mxt_opaque opqConnection,
                                                IN mxt_opaque opqId,
                                                IN mxt_result resCreation) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    ISipOutboundConnectionSvc() {}
    virtual ~ISipOutboundConnectionSvc() {}
//M5T_INTERNAL_USE_END
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPOUTBOUNDCONNECTIONSVC_H
