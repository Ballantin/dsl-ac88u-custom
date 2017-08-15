//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPMWISVC_H
#define MXG_ISIPMWISVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_MWI_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_MWI_SVC_SUPPORT to be able to \
use this class.
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
class CHeaderList;
class ISipClientTransaction;
class ISipMwiMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipMwiSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipMwiSvc
//=============================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The Message Waiting Indication service is used to subscribe to the state of
//  a message-box account held by a server. When the state of the message-box
//  account changes, whether because a message was added or removed, the
//  subscriber is notified of such change. This allows the application to notify
//  a user in some way that the state of his message-box has changed.
//
//  This service is the implementation of the "message-summary" event package
//  for RFC 3265. As such, this service sends the SUBSCRIBE SIP request to
//  subscribe to the state of a message-box and expects to receive NOTIFY
//  requests when the message-box state has changed.
//
//  This service generates events to the application through the ISipMwiMgr
//  interface.
//
// #The ISipMwiSvc is an ECOM object#
//
//  The ISipMwiSvc is an ECOM object that is created and accessed through the
//  following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipMwiSvc</B>
//
//  <B>Interface Id: IID_ISipMwiSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//  SipUserAgent/ISipMwiSvc.h
//
// See Also:
//  CSipMwiSvc, ISipMwiMgr
//
//==============================================================================
class ISipMwiSvc : public IEComUnknown
{
// Friend Declarations
//-------------------------

// Published Interface
//-------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipMwiSvc);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the event manager associated with this service.
    //
    //  Parameters:
    //    pMgr:
    //      The event manager. Must not be NULL.
    //
    //  Description:
    //      Configures the event manager that will receive the events generated
    //      by this service.
    //
    //      Note that an event manager MUST be associated with this service
    //      before it is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipMwiMgr* pMgr) = 0;

    //==SDOC====================================================================
    //==
    //==  SetExpiringThreshold
    //==
    //==========================================================================
    // <FLAG Updated behavior in 4.1.4>
    //
    //  Summary:
    //      Sets the time to warn the application before a subscription expires.
    //
    //  Parameters:
    //    uThresholdSec:
    //      The time, in seconds, before expiration time that the EvExpiring is
    //      fired.
    //
    //  Description:
    //      Sets the amount of time that should remain to a subscription before
    //      EvExpiring is called for that subscription.  If not called, the
    //      default value is one minute (60 seconds).
    //
    //      This method affects only future subscritions. This means that if
    //      there is an active subscription while this method is called, the
    //      EvExpiring event is called according to the previous threshold
    //      value.
    //
    //      If the expires value is smaller than or equal to the threshold, the
    //      EvExpiring event is never called, EvExpired is instead called
    //      directly.
    //
    //  See Also:
    //      Subscribe, Unsubscribe
    //
    //==EDOC====================================================================
    virtual void SetExpiringThreshold(IN unsigned int uThresholdSec) = 0;

    //==========================================================================
    //==
    //==  Subscribe
    //==
    //==========================================================================
    //
    //  Summary:
    //      Subscribes to a message-box state.
    //
    //  Parameters:
    //    uExpirationSec:
    //      The maximum time, in seconds, for which the subscription is
    //      considered active without refreshing. When set to 0, the default
    //      expiration is used, which is 3600 seconds (one hour).
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. Can be NULL, which means
    //      that no extra headers are added. Ownership of this parameter is
    //      TAKEN.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      The event manager has not been set yet or Clear has been called on
    //      the context.
    //
    //    resFE_FAIL:
    //      The SUBSCRIBE request could not be sent.
    //
    //    resFE_SIPCORE_PACKET_BLOCKED:
    //      One of the service synchronously blocked the packet. No additional
    //      event will be reported.
    //
    //    resS_OK:
    //      The SUBSCRIBE request was successfully sent.
    //
    //  Description:
    //      This method is used to subscribe the state of a message-box. This
    //      has the effect of sending a SUBSCRIBE request to the SIP request-URI
    //      currently configured in the context to which this service is
    //      attached.
    //
    //      When an expiration of 0 is specified, an Unsubscribe or a Fetch is
    //      performed instead of a Subscribe or a Refresh.
    //
    //      Notification about the request's progress is received through the
    //      ISipMwiMgr interface.
    //
    //      Upon receiving a final negative response other than a 481 on a
    //      subscription refresh (i.e. Subscribe has been called while the
    //      subscription is still active and EvFailure has been called on the
    //      ISipMwiMgr), the subscription is still considered active and a
    //      refresh can therefore be retried until the subscription expires.
    //
    //  See Also:
    //      ISipMwiMgr
    //
    //==========================================================================
    virtual mxt_result Subscribe(
                       IN    unsigned int            uExpirationSec,
                       IN    mxt_opaque              opqTransaction,
                       OUT   ISipClientTransaction*& rpTransaction,
                       IN TO CHeaderList*            pExtraHeaders = NULL) = 0;

    //==SDOC====================================================================
    //==
    //==  Unsubscribe
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes a subscription to a message-box state, or performs a Fetch
    //      if the subscription does not already exist.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. Can be NULL, which means
    //      that no extra headers are added. Ownership of this parameter is
    //      TAKEN.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      The event manager has not been set yet or Clear has been called on
    //      the context.
    //
    //    resFE_FAIL:
    //      The SUBSCRIBE request could not be sent.
    //
    //    resFE_SIPCORE_PACKET_BLOCKED:
    //      One of the service synchronously blocked the packet.
    //      No additional event will be reported.
    //
    //    resS_OK:
    //      The SUBSCRIBE request is successfully sent.
    //
    //  Description:
    //      This method is used to remove a subscription to the state of a
    //      message-box. This has the effect of sending a SUBSCRIBE request to
    //      the SIP request-URI currently configured in the context to which
    //      this service is attached, with an expiration value of zero.
    //
    //      Notification about the request's progress is received through the
    //      ISipMwiMgr interface.
    //
    //  See Also:
    //      ISipMwiMgr
    //
    //==EDOC====================================================================
    virtual mxt_result Unsubscribe(
                       IN    mxt_opaque              opqTransaction,
                       OUT   ISipClientTransaction*& rpTransaction,
                       IN TO CHeaderList*            pExtraHeaders = NULL) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipMwiSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipMwiSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipMwiSvc(IN const ISipMwiSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipMwiSvc& operator=(IN const ISipMwiSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPMWISVC_H

