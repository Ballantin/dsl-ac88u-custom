//==============================================================================
//==============================================================================
//
//          Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCENOTIFIERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCENOTIFIERMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPEVENTTYPES_H
#include "SceSipTypes/SipEventTypes.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CNameAddr;
class CSipMessageBody;
class ISceNotifier;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceNotifierMgr
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This is the interface through which the Notifier component reports its
//   events about the state of a subscription.
//
// Description:
//  This is the interface through which the Notifier component reports its
//  events about the state of a subscription.
//
// Location:
//   SceCoreComponents/ISceNotifierMgr.h
//
// See Also:
//  ISceNotifier
//
//==============================================================================
class ISceNotifierMgr
{
//-- Friend Declarations.

//-- Published Interface.
public:


    //==========================================================================
    //==
    //==  EvNewSubscription
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a new subscription request was received.
    //
    //  Parameters:
    //    pNotifier:
    //      The notifier component reporting this event.
    //
    //    rFromAddr:
    //      The From address found in the subscription request.
    //
    //    rToAddr:
    //      The To address found in the subscription request.
    //
    //    eEventType:
    //      The event type that identifies the type of resource to which 
    //      subscribe.
    //
    //    uRequestedExpirationSec:
    //      The requested subscription expiration time. It can be shortened by
    //      the application when calling ISceNotifier::AcceptSubscription.
    //
    //    pBody:
    //      Pointer to the SUBSCRIBE payload. Can be NULL if no payload was
    //      present.
    //
    //    pAdditionalParameter:
    //      Additional parameters in relation to the subscription request. Can
    //      be NULL if no additional parameters are present.
    //
    //      This returned parameter is an ECOM object that can be queried for
    //      all the interfaces that need to be watched or processed by the
    //      application from the Base Component or from specific component
    //      interfaces.
    //
    //      If it is NULL, it means no additional parameters could be returned
    //      to the application, either because there was a processing problem
    //      or that no server event yielded information worth returning.
    //
    //  Description:
    //      Reports the reception of a new subscription request.
    //
    //      The manager must either call ISceNotifier::AcceptSubscription or
    //      ISceNotifier::RejectSubscription after receiving this event.
    //
    //      If the manager calls ISceNotifier::AcceptSubscription, it must also
    //      immediately send a NOTIFY request using ISceNotifier::Notify.
    //
    //      If the manager is unable to make an immediate decision as to whether
    //      or not it should accept a subscription, it must still call
    //      ISceNotifier::AcceptSubscription with the eState parameter set to
    //      ISceNotifier::ePENDING, and it still must issue a single NOTIFY
    //      (possibly with no payload) using the ISceNotifier::Notify API, again
    //      with eState set to ePENDING.
    //
    //==========================================================================
    virtual void EvNewSubscription(IN ISceNotifier* pNotifier,
                                   IN const CNameAddr& rFromAddr,
                                   IN const CNameAddr& rToAddr,
                                   IN ESipEventType eEventType,
                                   IN uint32_t uRequestedExpirationSec,
                                   IN const CSipMessageBody* pBody,
                                   IN IEComUnknown* pAdditionalParameter) = 0;


    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   Possible ways and reasons a subscription is terminated.
    //
    // Description:
    //  This enumeration defines the possible ways and reasons a subscription is
    //  terminated when reporting EvSubscriptionTerminated.
    //
    //==========================================================================
    // 
    enum ETerminationReason
    {
        // Description:
        // The subscription was terminated by the peer subscriber that sent an
        // unsubscribe request.
        eTERMINATE_UNSUBSCRIBED,
        
        // Description:
        // The subscription was terminated by lack of subscription refresh
        // within the proper time frame.
        eTERMINATE_TIMEOUT
    };

    //==========================================================================
    //==
    //==  EvSubscriptionTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the subscription was terminated by the peer.
    //
    //  Parameters:
    //    pNotifier:
    //      The notifier component reporting the event.
    //
    //    eReason:
    //      The reason for the subscription termination.
    //
    //  Description:
    //      Reports that the subscribtion has been terminated. Note that a final
    //      NOTIFY request must be sent by calling
    //      ISceNotifier::TerminateSubscription after this event is reported.
    //
    //      This event is not reported when the manager spontaneously terminates
    //      a subscription by calling ISceNotifier::TerminateSubscription.
    //
    //      Please note that this event reports the end of a subscription but
    //      not the end of the notifier component itself. The manager must not
    //      release the ISceNotifier until it has received the
    //      EvNotifierTerminated event.
    //
    //  See Also:
    //      ETerminationReason
    //
    //==========================================================================
    virtual void EvSubscriptionTerminated(IN ISceNotifier* pNotifier,
                                          IN ETerminationReason eReason) = 0;

    //==========================================================================
    //==
    //==  EvNotifierTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the notifier component has terminated and can be
    //      released.
    //
    //  Parameters:
    //    pNotifier:
    //      The notifier component reporting this event.
    //
    //  Description:
    //      Reports that a notifier component has terminated its management of a
    //      subscription. The manager of the ISceNotifier must always wait for
    //      this event before releasing its reference it has on the ISceNotifier.
    //
    //      This is always the last event reported by the notifier component and
    //      it is always reported.
    //
    //==========================================================================
    virtual void EvNotifierTerminated(IN ISceNotifier* pNotifier) = 0;


//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceNotifierMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISceNotifierMgr() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceNotifierMgr(IN const ISceNotifierMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceNotifierMgr& operator=(IN const ISceNotifierMgr& rSrc);

//-- Hidden Data Members.
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCENOTIFIERMGR_H

