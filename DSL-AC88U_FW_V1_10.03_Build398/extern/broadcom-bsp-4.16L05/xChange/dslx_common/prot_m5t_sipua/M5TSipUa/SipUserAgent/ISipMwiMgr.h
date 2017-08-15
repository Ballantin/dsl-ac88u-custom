//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2002-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPMWIMGR_H
#define MXG_ISIPMWIMGR_H

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


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CMessageSummary;
class CSipPacket;
class ISipMwiSvc;
class ISipClientEventControl;
class ISipServerEventControl;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipMwiMgr
//=============================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The Message Waiting Indication Event Manager is the interface through which
//  the MWI service talks to the application. The SIP Stack informs the
//  application through this interface of the progress for SUBSCRIBE requests
//  and reception of NOTIFY requests.
//
// Location:
//   SipUserAgent/ISipMwiMgr.h
//
// See Also:
//  ISipMwiSvc
//
//==============================================================================
class ISipMwiMgr
{
// Friend Declarations
//-------------------------

// Published Interface
//-------------------------
public:

    //==========================================================================
    //==
    //==  EvNotified
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application of a change to the message-box's state.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipMwiSvc managing MWI messages.
    //
    //    pServerEventCtrl:
    //      Interface to the server event control for this server transaction.
    //
    //    rPacket:
    //      The NOTIFY request.
    //
    //    pMessageSummary:
    //      The NOTIFY request's payload (message summary).
    //
    //  Description:
    //      Notifies the application that the state of the message-box has
    //      changed.  This usually means that a message has been added or
    //      deleted from the message-box.
    //
    //      When receiving a NOTIFY with no payload and it is not the first
    //      NOTIFY, a 200 OK response is returned without this event being
    //      called.
    //
    //==========================================================================
    virtual void EvNotified(IN ISipMwiSvc* pSvc,
                            IN ISipServerEventControl* pServerEventCtrl,
                            IN const CSipPacket& rPacket,
                            IN TO CMessageSummary* pMessageSummary) = 0;

    //==========================================================================
    //==
    //==  EvShutdown
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that the message-box is about to be closed.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipMwiSvc managing MWI messages.
    //
    //    pServerEventCtrl:
    //      Interface to the server event control for this server transaction.
    //
    //    rPacket:
    //      The NOTIFY request.
    //
    //    pMessageSummary:
    //      The NOTIFY request's payload (message summary).
    //
    //  Description:
    //      Notifies the application that the subscription has been terminated
    //      by the remote server because it went offline.
    //
    //==========================================================================
    virtual void EvShutdown(IN ISipMwiSvc* pSvc,
                            IN ISipServerEventControl* pServerEventCtrl,
                            IN const CSipPacket& rPacket,
                            IN TO CMessageSummary* pMessageSummary) = 0;

    //==========================================================================
    //==
    //==  EvInvalidNotify
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that an invalid NOTIFY request has been
    //      received.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipMwiSvc managing MWI messages.
    //
    //    opqApplicationData:
    //      Data set by the application in another event for this same request.
    //      When this is the first event for this request, it is set to 0.
    //
    //    rPacket:
    //      The NOTIFY request received.
    //
    //    res:
    //      - resFE_UNKNOWN_EVENT: The Event header was missing or for an event
    //      type other than "message-summary". A "489 Bad Event" response has
    //      already been sent.
    //
    //      - resFE_UNKNOWN_SUBSCRIPTION: The NOTIFY does not correspond to a
    //      REFER sent by this service. A "481 Subscription does not exist"
    //      response has already been sent.
    //
    //      - resFE_EXPIRED_SUBSCRIPTION: The NOTIFY has been received after
    //      another NOTIFY with "subscription-state" "terminated" or after the
    //      expiration time. A "481 Subscription does not exist" response has
    //      already been sent.
    //
    //      - resFE_MISSING_HEADER: The received NOTIFY did not contain the
    //      mandatory "Subscription-State" header.
    //
    //      - resFE_INVALID_CONTENT: The received NOTIFY did not contain a valid
    //      "application/simple-message-summary".
    //
    //  Description:
    //      Notifies the application that an invalid NOTIFY request has been
    //      received and what is the error code.
    //
    //==========================================================================
    virtual void EvInvalidNotify(IN ISipMwiSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rPacket,
                                 IN mxt_result res) = 0;

    //==========================================================================
    //==
    //==  EvProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application of the progress of an outgoing
    //      subscription.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipMwiSvc managing MWI messages.
    //
    //    pClientEventCtrl:
    //      Interface to the client event control for this client transaction.
    //
    //    rPacket:
    //      The provisional response received.
    //
    //  Description:
    //      Notifies the application that a response has been received for the
    //      SUBSCRIBE request that was just sent.
    //
    //==========================================================================
    virtual void EvProgress(IN ISipMwiSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  EvSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application of the success of an outgoing subscription.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipMwiSvc managing Mwi messages.
    //
    //    pClientEventCtrl:
    //      Interface to the client event control for this client transaction.
    //
    //    rPacket:
    //      The success final response received.
    //
    //  Description:
    //      Notifies the application that a response has been received for the
    //      SUBSCRIBE request that was just sent.
    //
    //==========================================================================
    virtual void EvSuccess(IN ISipMwiSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  EvFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application of the failure of an outgoing subscription.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipMwiSvc managing MWI messages.
    //
    //    pClientEventCtrl:
    //      Interface to the client event control for this client transaction.
    //
    //    rPacket:
    //      The failure final response received.
    //
    //  Description:
    //      Informs the application that a response was received for the
    //      SUBSCRIBE request that was just sent. When a response to a
    //      subscription refresh has a status code other than a 481, the
    //      subscription remains active and a refresh can still be retried until
    //      the subscription expires.
    //
    //==========================================================================
    virtual void EvFailure(IN ISipMwiSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  EvIntervalTooSmall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application of the failure of an outgoing subscription
    //      because the interval is too small.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipMwiSvc managing MWI messages.
    //
    //    pClientEventCtrl:
    //      Interface to the client event control for this client transaction.
    //
    //    uMinExpirationSec:
    //      The value of the Min-Expires header found in the 423 response.
    //
    //    rPacket:
    //      The response received.
    //
    //  Description:
    //      Notifies the application that a response indicating an interval too
    //      small has been received.
    //
    //==========================================================================
    virtual void EvIntervalTooSmall(
                                   IN ISipMwiSvc* pSvc,
                                   IN ISipClientEventControl* pClientEventCtrl,
                                   IN unsigned int uMinExpirationSec,
                                   IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  EvExpiring
    //==
    //==========================================================================
    //
    //  Summary:
    //      The subscription is about to expire.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipMwiSvc managing MWI messages.
    //
    //  Description:
    //      This event is reported when the subscription is about to expire.
    //      This means that the application should refresh the subscription by
    //      re-subscribing.
    //
    //==EDOC====================================================================
    virtual void EvExpiring(IN ISipMwiSvc* pSvc) = 0;

    //==SDOC====================================================================
    //==
    //==  EvExpired
    //==
    //==========================================================================
    //
    //  Summary:
    //      The subscription has expired.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipMwiSvc managing MWI messages.
    //
    //  Description:
    //      This event is reported when the subscription has expired. The
    //      application must no longer expect any notification about the state
    //      of the message-box.
    //
    //==EDOC====================================================================
    virtual void EvExpired(IN ISipMwiSvc* pSvc) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipMwiMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipMwiMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipMwiMgr(IN const ISipMwiMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipMwiMgr& operator=(IN const ISipMwiMgr& rSrc);

//-- Hidden Data Members
//------------------------
protected:
private:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPMWIMGR_H

