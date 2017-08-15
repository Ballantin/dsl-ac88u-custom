//==SDOC========================================================================
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
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPCONTEXT_H
#define MXG_ISIPCONTEXT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipContextObserver;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipContext);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipContext
//========================================
// <GROUP SIPCORE_CLASSES>
//
// Description:
//
//  The SIP context is an important concept to grasp to correctly use the stack.
//
//  The SIP context defines the scope, features, and capabilities of an
//  application level SIP service. It is only through an ISipContext and stack
//  services attached to it that the application can take any SIP actions on the
//  network.
//
//  A SIP context is required by a client-based application to offer the
//  following services:
//
//      - Making or receiving a SIP call.
//      - Registering a user.
//      - Subscribing to a specific event package.
//      - Starting an IM session
//
//  A server-based application would also need a SIP context for the following
//  application level services:
//
//      - Proxying one or more received SIP request and its
//        responses.
//      - Redirecting a received SIP request.
//      - Receiving a registration request.
//
//  A Back-to-Back User-Agent application would also use two SIP contexts, one
//  for each side of the call.
//
//  Note that the Server location service MUST always be attached to a context.
//  This is necessary to send a packet with a context even if no name
//  resolution is necessary. This is because that service is also responsible
//  of selecting the right transport protocol and managing reconnection.
//
//#The ISipContext and the Stack Services#
//
//  A SIP context alone is not very useful, as it mainly defines a framework
//  upon which different services can be attached to offer various SIP
//  functionality.
//
//  By calling AttachService, the application configures the SIP context with
//  various stack services and, as such, defines the features and scope of the 
//  SIP context.
//
//  As an example, if an application attaches the SIP User-Agent service to a
//  SIP context, then its scope is that of a User-Agent. The application would
//  also attach additional stack services to the context to enable various
//  functionality on the context, such as providing credentials to a server,
//  managing session timers, and placing a call.
//
//  After the application has attached services to a context, it can access them
//  through the standard ECOM mechanism to initiate different SIP actions.
//
//  Which action is possible on a SIP context thus depends on the services that
//  have been attached to it. This is also true of the events reported back to
//  the application, as these are generated by the services and not the SIP
//  context.
//
//  The lifetime of a SIP context depends on the services that have been
//  attached to it. A SIP context used to place a call is generally good only
//  for the duration of the dialog established by the call, while other SIP
//  contexts, such as a context configured with a stateless proxy service, can
//  be used for as long as the application wants.
//
//#Services Ordering#
//
//  All services attached to a SIP context get to see all packets associated
//  with this context. When doing so, the services may want to report an event
//  to the application, in which case the event is enqueued into an event queue.
//  Once all services have seen the packet, the event queue is processed. For
//  server events (reported upon receiving a request), the stack reports all
//  events from the event queue, one after the other. For client events
//  (reported upon receiving a response to a request), the stack reports only
//  the first event from the event queue.
//
//  There is a correlation between the order in which services are attached to a
//  SIP context and the order of generated events. Services attached first get
//  to report their event first to the application. The M5T SIP
//  Programmer's Guide further discusses this issue and provides general
//  guidance on ordering the services to suit most applications.
//
//#The ISipContext is an ECOM object#
//
//  The ISipContext is an ECOM object that is created and accessed through the
//  following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipContext</B>
//
//  <B>Interface Id: IID_ISipContext</B>
//
//  The ISipContext aggregates all the services that are attached to it. This
//  means that it supports the interfaces (through <C>QueryIf()</C>) of all the
//  services that have been attached to it.
//
// Location:
//   SipCore/ISipContext.h
//
// See Also:
//   All services found in the SipUserAgent and SipProxy packages.
//
//==EDOC========================================================================
class ISipContext : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipContext);
    //M5T_INTERNAL_USE_END

    //-- << Service Management >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  AttachService
    //==
    //==========================================================================
    //
    //  Summary:
    //      Attaches a service to the context.
    //
    //  Parameters:
    //    classId:
    //      The class id of the service to attach to the context.
    //
    //  Returns:
    //      resS_OK: The service has been properly added to the context.
    //
    //      resFE_FAIL: The service has not been added either because it failed 
    //      to be created or it did not implement at least one of the 
    //      ISipConnectionSvc or the ISipCoreSvc interfaces.
    //
    //  Description:
    //      This method is used to attach a service to the SIP context.
    //
    //      After this method returns success, the various interfaces supported
    //      by the service can be accessed by calling <C>QueryIf</C> on the
    //      SIP context.
    //
    //==EDOC====================================================================
    virtual
    mxt_result AttachService(IN mxt_clsid classId) = 0;


    //==SDOC====================================================================
    //==
    //==  OnPacketReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tries to handle an incoming packet.
    //
    //  Parameters:
    //    rPacket:
    //      The packet that was just received.
    //
    //  Returns:
    //    resS_OK: The packet has been handled by the context.
    //
    //    resFE_FAIL: The incoming packet has not been handled.
    //
    //    resFE_DUPLICATE: Unable to create the server transaction, as it 
    //    already exists.
    //
    //
    //  Description:
    //      This method is used to have the SIP context and the services it
    //      holds process the incoming packet.
    //
    //      The result returned by this method usually depends on the services
    //      that are attached to the SIP context.
    //
    //      When calling this on a newly created SIP context that has stateful
    //      services attached to it (UA service or stateful proxy service), it
    //      is possible that resFE_DUPLICATE is returned, in which case the
    //      context should be released immediately. This usually happens when
    //      the time between a request and its retransmissions is too short.
    //
    //==EDOC====================================================================
    virtual mxt_result OnPacketReceived(IN const CSipPacket& rPacket) = 0;


    //==SDOC====================================================================
    //==
    //==  Clear
    //==
    //==========================================================================
    //
    //  Summary:
    //      Prepares the context to be deleted.
    //
    //  Returns:
    //    resSW_SIPCORE_ACTIVE_DIALOG:
    //      A dialog was active when this method was called. The application
    //      should have terminated the dialog before calling this method. The
    //      dialog has been terminated.
    //
    //    resS_OK:
    //      No dialog was active when this method was called.
    //
    //  Description:
    //      This method must be called before the last reference kept on the
    //      context by the application is released. It prepares the context to
    //      be deleted.
    //
    //      Normally, this method should be called when no requests or dialogs
    //      are pending. However, the method prepares the context to be released
    //      even if there are pending requests or dialogs.
    //
    //      After this method is called, the service managers are no longer
    //      notified about anything.
    //
    //      If the application still has unused ISipServerEventControl or
    //      ISipClientEventControl, it MUST use them AND release their reference
    //      in order to allow the context to be deleted.In particular, a final
    //      response MUST be sent on any ISipServerEventControl related to this
    //      ISipContext BEFORE this method is called."
    //
    //      Note that the reference kept on the ISipContext MUST still be
    //      released even if this method is called.
    //
    //  See Also:
    //      ISipServerEventControl, ISipClientEventControl
    //
    //==EDOC====================================================================
    virtual mxt_result Clear() = 0;


    //==SDOC====================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Associates opaque data to this SIP context.
    //
    //  Parameters:
    //    opaque:
    //      The opaque data. The stack never uses this data.
    //
    //  Description:
    //      Allows the user of the SIP context to associate data with this
    //      context. This is opaque data to the SIP context and is never used or
    //      interpreted.
    //
    //  See Also:
    //      GetOpaque
    //
    //
    //==EDOC====================================================================
    virtual void SetOpaque(IN mxt_opaque opaque) = 0;


    //==SDOC====================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets current opaque data associated with this SIP context.
    //
    //  Returns:
    //      Previously associated opaque data. Opaque with a value of zero if no
    //      opaque data had been previously set.
    //
    //  Description:
    //      Allows the SIP context user to access the opaque data associated
    //      with this SIP context.
    //
    //  See Also:
    //      SetOpaque
    //
    //==EDOC====================================================================
    virtual mxt_opaque GetOpaque() = 0;


    //==========================================================================
    //==
    //==  SetEntityId
    //==
    //==========================================================================
    //
    //  Summary:
    //      Associates an entity ID with this SIP context.
    //
    //  Parameters:
    //    uEntityId:
    //      The entity ID.
    //
    //  Description:
    //      Allows the user of the SIP context to assign it an entity ID.
    //      An entity ID is used to associate a listening port with a SIP context
    //      via the given "entity". Packets sent from this SIP context will
    //      only use a listening interface with the same entity ID to send the 
    //      packet. If no listening interface is configured with the same entity
    //      ID, the packet will fail to be sent. The VIA and the contact will
    //      be automatically updated according to the selected interface.
    //      By default, the entity ID is set to 0.
    //
    //      The entity ID should be set immediately after creation of the
    //      SipContext and never modified again.
    //
    //  See Also:
    //      GetEntityId, ISipCoreConfig::SetEntityId
    //
    //==========================================================================
    virtual void SetEntityId(IN unsigned int uEntityId) = 0;


    //==========================================================================
    //==
    //==  GetEntityId
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the current entity ID associated with this SIP context.
    //
    //  Returns:
    //      The entity ID of this SIP context.
    //
    //  Description:
    //      Allows the user to access the entity ID associated
    //      with this SIP context.
    //
    //  See Also:
    //      SetEntityId
    //
    //==========================================================================
    virtual unsigned int GetEntityId() = 0;

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    virtual mxt_opaque GetDnsUserOpaque() = 0;

    virtual void SetDnsUserOpaque(IN mxt_opaque opq) = 0;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

// BRCM_CUSTOM - [add] Add CCTK GUID opaque
    virtual mxt_opaque GetGuidOpaque() = 0;

    virtual void SetGuidOpaque(IN mxt_opaque opq) = 0;
// BRCM_CUSTOM - [end] Add CCTK GUID opaque

// BRCM_CUSTOM - [add] Add TOS config support
    virtual mxt_opaque GetTosOpaque() = 0;

    virtual void SetTosOpaque(IN mxt_opaque opq) = 0;
// BRCM_CUSTOM - [end] Add TOS config support

    //==========================================================================
    //==
    //==  AddObserver
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds the given context observer to the list of observers.
    //
    //  Returns:
    //      resS_OK if the observer was added to the list.
    //      resFE_FAIL: if the observer is already in the list.
    //
    //  Description:
    //      Adds the given observer to this context observer list.
    //
    //  See Also:
    //      RemoveObserver
    //
    //==========================================================================
    virtual mxt_result AddObserver(ISipContextObserver* pObserver) = 0;

    //==========================================================================
    //==
    //==  RemoveObserver
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes the given context observer from the list of observers.
    //
    //  Returns:
    //      resS_OK if the observer was removed from the list.
    //      resFE_FAIL: if the given observer was not found in the list.
    //
    //  Description:
    //      Removes the given observer from this context observer list.
    //
    //  See Also:
    //      AddObserver
    //
    //==========================================================================
    virtual mxt_result RemoveObserver(ISipContextObserver* pObserver) = 0;

//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipContext() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipContext() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------


    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipContext(IN const ISipContext& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipContext& operator=(IN const ISipContext& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCONTEXT_H
