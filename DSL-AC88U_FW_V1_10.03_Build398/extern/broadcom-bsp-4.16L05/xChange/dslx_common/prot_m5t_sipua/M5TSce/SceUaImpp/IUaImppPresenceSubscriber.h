//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_IUAIMPPPRESENCESUBSCRIBER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPPRESENCESUBSCRIBER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CNameAddr;
class CSceCallerPreferences;
class ISceUserConfig;
class IUaImppPresenceSubscriberMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IUaImppPresenceSubscriber);

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppPresenceSubscriber
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//    This interface defines the possible operations on the presence subscriber.
//
// Description:
//  This interface defines the possible operations on the presence subscriber.
//
//  The presence subscriber allows to subscribe to the presence state of one or
//  more resources using the SIP SUBSCRIBE request. It supports subscribing to a
//  single resource (as per RFC 3265) or to a resource list (as per RFC 4662)
//  using the same Subscribe() API.
//
//  The presence subscriber moreover supports receiving PIDF payloads as per RFC
//  3863 and partial PIDF as per RFCs 5262 and 5263.
//
//  The presence subscriber reports events about the subscription state and the
//  presence information to its user through the IUaImppPresenceSubscriberMgr.
//
//  The events reported by the presence subscriber will depend on the type of
//  subscription being created:
//
//      - When a subscribing to a single resource, the presence subscriber
//        will use EvSubscriptionStatus to report the subscription state, and it
//        will use EvPresenceStatusUpdate to report the presence state of the
//        peer.
//
//      - When subscribing to a resource list, the presence subscriber will
//        use EvSubscriptionStatus to report the status of the subscription to
//        the list of resource, and it will use three more events to report the
//        status of elements in the resource list. EvNewPresenceElement is used
//        to report that a new element is now being subscribed to as part of the
//        resource list. EvUpdatedPresenceElement is reported whenever the
//        subscription or presence state of a resource on the resource list
//        changes. Finally, EvDeletedPresenceElement is reported when the
//        subscriber detects that the server stopped reporting state information
//        about a specific resource.
//
//  The presence subscriber builds on top of the generic subscriber
//  (ISceSubscriber) and thus inherits from its basic behavior:
//
//      - It automatically manages subscription refreshes;
//      - It automatically retries to subscribe upon failure;
//      - It will abandon trying to subscribe when it thinks that it really
//        cannot establish a subscription, in which case it will report that
//        the subscription is in the eSUBSCRIPTION_FAILED state;
//      - The user can use the Unsubscribe API in order to unsubscribe. The user
//        must then wait for the subscription to get into the eTERMINATED state
//        before releasing the component implementing this interface;
//      - Whenever the presence subscriber reports that the subscription is in
//        the eSUBSCRIPTION_FAILED, eTERMINATED or eUSER_TERMINATED state, the
//        user must release the component implementing this interface, as these
//        are terminal state changes and the component can no longer be used;
//
// Location:
//   UaImpp/IUaImppPresenceSubscriber.h
//
// See Also:
//   IUaImppPresenceSubscriberMgr
//
//==============================================================================
class IUaImppPresenceSubscriber : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaImppPresenceSubscriber);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which events are reported for this oject.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. It must not be NULL.
    //
    //  Returns:
    //      - resS_OK: Manager set correctly.
    //      - resFE_INVALID_ARGUMENT: pMgr is NULL.
    //
    //  Description:
    //      Configures the manager to which this object reports events.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN IUaImppPresenceSubscriberMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the component with a user configuration object.
    //
    //  Parameters:
    //      pConfig:
    //       Pointer to the ECOM configuration object for this user.
    //
    //  Returns:
    //      - resS_OK: Configuration correctly set.
    //      - resFE_INVALID_ARGUMENT: Configuration is NULL
    //      - resFE_INVALID_STATE: Configuration is already set.
    //      - resFE_FAIL: Configuration is invalid.
    //
    //  Description:
    //      Configures this component with a user configuration object.
    //
    //==========================================================================
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig) = 0;

    //==========================================================================
    //==
    //==  GetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the component's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the component.
    //
    //==========================================================================
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig) = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an opaque paramerer to the presence subscriber.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the presence subscriber. It can be
    //      used by the application to store any information.
    //
    //==========================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the opaque parameter to the presence subscriber.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the presence subscriber. It can be
    //      used by the application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==========================================================================
    //==
    //==  Subscribe
    //==
    //==========================================================================
    //
    //  Summary:
    //      Subscribes to the presence state of a peer.
    //
    //  Parameters:
    //    pResourceAor:
    //      The address of the peer. Ownership is taken.
    //
    //    pPayload:
    //      Pointer to a payload to send along with the subscribe request. This
    //      can be used to install a notification filter. See ISceFilterDocument
    //      for more information. Can be NULL, in which case no payload is sent.
    //      Ownership is taken.
    //
    //    pCallerPreferences:
    //      Optional caller preferences to add to the outgoing SUBSCRIBE
    //      request. Can be NULL if not caller preferences are to be sent.
    //      Ownership is taken.
    //
    //  Returns:
    //      - resS_OK: Subscribe successful.
    //      - resFE_INVALID_ARGUMENT: pResourceAor cannot be NULL.
    //      - resFE_INVALID_STATE: Configiguration not completed.
    //      - Other: Method failure.
    //
    //  Description:
    //      Subscribes to the presence state of a peer.
    //
    //      The event about the subscription state are later reported through
    //      IUaImppPresenceSubscriberMgr::EvSubscriptionStatus, while the
    //      presence status of the user is reported through
    //      IUaImppPresenceSubscriberMgr::EvPresenceStatusUpdate.
    //
    //  See Also:
    //      Unsubscribe, IUaImppPresenceSubscriberMgr
    //
    //==========================================================================
    virtual mxt_result Subscribe(IN TOA CNameAddr* pResourceAor,
                                 IN TOA CSipMessageBody* pPayload,
                                 IN TOA CSceCallerPreferences* pCallerPreferences) = 0;


    //==SDOC====================================================================
    //==
    //==  Refresh
    //==
    //==========================================================================
    //
    //  Summary:
    //      Forces a subscription refresh.
    //
    //  Parameters:
    //    pPayload:
    //      Pointer to the payload to send with the refresh. This can be used to
    //      install a notification filter document. See ISceFilterDocument. Can
    //      be NULL, in which case the refresh is sent without any payload.
    //      Ownership is taken.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Forces a subscription refresh. This will usually be used to update a
    //      filter on the presence server.
    //
    //  See Also:
    //      Subscribe, ISceFilterDocument
    //
    //==EDOC====================================================================
    virtual mxt_result Refresh(IN TOA CSipMessageBody* pPayload) = 0;


    //==========================================================================
    //==
    //==  Unsubscribe
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates the subscription to the peer's presence state.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_INVALID_STATE: Configiguration not completed.
    //      - Other: Method failure.
    //
    //  Description:
    //      Terminates the subscription to the peer's presence state.
    //
    //      The application must wait for the final
    //      IUaImppPresenceSubscriberMgr::EvSubscriptionStatus(eTERMINATED)
    //      before releasing this component.
    //
    //  See Also:
    //      Subscribe
    //
    //==========================================================================
    virtual mxt_result Unsubscribe() = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IUaImppPresenceSubscriber() {};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IUaImppPresenceSubscriber() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IUaImppPresenceSubscriber(IN const IUaImppPresenceSubscriber& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IUaImppPresenceSubscriber& operator=(IN const IUaImppPresenceSubscriber& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUAIMPPPRESENCESUBSCRIBER_H

