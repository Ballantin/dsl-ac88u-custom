//==============================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==============================================================================
//==============================================================================
#ifndef MXG_ISIPPERSISTENTCONNECTIONSVC_H
#define MXG_ISIPPERSISTENTCONNECTIONSVC_H

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

#ifndef MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT to be able to use \
this class.
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipPersistentConnectionSvc);

#if defined(MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
//M5T_INTERNAL_USE_BEGIN
// Tells the context to use the highest priority switchback persistent
// connection that is available.
const mxt_opaque opqHIGHEST_PRIORITY_CONNECTION = 0;
//M5T_INTERNAL_USE_END
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
class ISipSwitchbackPersistentConnection;

//==============================================================================
//== Class: ISipPersistentConnectionSvc
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//  This service enables the support of persistent connections for the SIP
//  Context upon which it is attached. The requests sent by this service will
//  use a persistent connection only if the next hop URI (the top-most Route or
//  the request-URI) corresponds to an established persistant connection.
//
//  Applications can create and manage their own connections with the
//  CSipPersistentConnectionList.
//
//  When this service is attached to a SIP Context, it will override DNS SRV
//  resolving and use the resolved address used by the persistent connection
//  instead.
//
//  This service can be used in two different modes: using the persistent
//  connection list (CSipPersistentConnectionList) or the other mode using the
//  switchback persistent connection management
//  (ISipSwitchbackPersistentConnection).
//
//  When the service is using the persistent connection list, it MUST be
//  attached to the SIP Context after the ISipServerLocationSvc, as it may have
//  to override the next hop address in order to use the persistent connection.
//
//  When the service is using the switchback persistent connection management,
//  the ISipServerLocationSvc MUST NOT be attached to the SIP Context.
//
//#The ISipPersistentConnectionSvc is an ECOM object#
//
//  The ISipPersistentConnectionSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipPersistentConnectionSvc</B>
//
//  <B>Interface Id: IID_ISipPersistentConnectionSvc</B>
//
//  A user can query the ISipContext to which this service is attached simply
//  by calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipPersistentConnectionSvc.h
//
// See Also:
//  CSipPersistentConnectionList
//
//==============================================================================
class ISipPersistentConnectionSvc : public IEComUnknown
{
//-- Friend Declarations

//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipPersistentConnectionSvc);
#if defined(MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
    //==========================================================================
    //==
    //==  SetSwitchbackPersistentConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the switchback persistent connection instance.
    //
    //  Parameters:
    //      pSwitchbackPersistentConnection:
    //       The switchback persistent connection.
    //
    //  Returns:
    //      - resS_OK: Set was successful.
    //      - resFE_INVALID_ARGUMENT: pSwitchbackPersistentConnection is NULL.
    //
    //  Description:
    //      If the switchback persistent connection is set then the service uses
    //      this instance to find a connection to be used by the context. The
    //      ISipServerLocatorSvc MUST NOT be attached on such context using the
    //      ISipSwitchbackPersistentConnection.
    //
    //      If no switchback persistent connection is specified then the default
    //      behavior remains. The CSipPersistentConnectionList will choose the
    //      persistent connection to use.
    //
    //==========================================================================
    virtual mxt_result SetSwitchbackPersistentConnection(IN ISipSwitchbackPersistentConnection* pSwitchbackPersistentConnection) = 0;
#endif // #if defined(MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)

    //==========================================================================
    //==
    //==  GetConnectionOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //    Gets the opaque of the switchback persistent connection that is used
    //    for this context.
    //
    //  Returns:
    //      - The switchback persistent connection opaque identifier that is
    //        used for this context.
    //
    //  Description:
    //      Gets the switchback persistent connection opaque identifier that is
    //      used for this context.
    //
    //      If the opaque identifier is opqHIGHEST_PRIORITY_CONNECTION, then the
    //      highest priority established and verified connection is used.
    //
    //==========================================================================
    virtual mxt_opaque GetConnectionOpaque() = 0;

    //==========================================================================
    //==
    //==  SetConnectionOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the opaque of the persistent connection to be used for this
    //    context.
    //
    //  Parameters:
    //    opqConnection:
    //      The opaque identifier of the persistent connection to be used for
    //      this context (given by CSipPersistentConnectionList::Establish).
    //
    //      When SetSwitchbackPersistentConnection has been called with a
    //      non-NULL ISipSwitchbackPersistentConnection, the opaque value given
    //      here is the opaque for the switchback persistent connection (given
    //      by ISipSwitchbackPersistentConnectionMgr::EvConnectionCreated for
    //      example).
    //
    //  Description:
    //      Sets the persistent connection opaque identifier to be used for this
    //      context.
    //
    //      When SetSwitchbackPersistentConnection has been called with a
    //      non-NULL ISipSwitchbackPersistentConnection, the opaque value given
    //      here is the opaque for the switchback persistent connection. In this
    //      mode, if the opaque identifier is opqHIGHEST_PRIORITY_CONNECTION,
    //      then the highest priority established and verified connection is
    //      used.
    //
    //==========================================================================
    virtual void SetConnectionOpaque(IN mxt_opaque opqConnection) = 0;

//M5T_INTERNAL_USE_END

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipPersistentConnectionSvc() {}

    // Summary:
    //  Destructor.
    virtual ~ISipPersistentConnectionSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipPersistentConnectionSvc(IN const ISipPersistentConnectionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipPersistentConnectionSvc& operator=(IN const ISipPersistentConnectionSvc& rSrc);
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPPERSISTENTCONNECTIONSVC_H
