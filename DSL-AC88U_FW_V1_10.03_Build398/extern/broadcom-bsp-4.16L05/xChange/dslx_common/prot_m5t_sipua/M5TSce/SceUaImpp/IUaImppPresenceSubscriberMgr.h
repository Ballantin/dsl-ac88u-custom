//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_IUAIMPPPRESENCESUBSCRIBERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPPRESENCESUBSCRIBERMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_ISCESUBSCRIBERMGR_H
#include "SceCoreComponents/ISceSubscriberMgr.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipMessageBody;
class IUaImppPresenceSubscriber;
class IScePidfDocument;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IUaImppPresenceSubscriberMgr
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//   This interface defines the event that the presence subscriber can return to
//   the application.
//
// Description:
//   This interface defines the event that the presence subscriber can return to
//   the application.
//
// Location:
//   UaImpp/IUaImppPresenceSubscriberMgr.h
//
// See Also:
//   IUaImppPresenceSubscriber
//
//==EDOC========================================================================
class IUaImppPresenceSubscriberMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==========================================================================
    //==
    //==  EvSubscriptionStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the subscription status.
    //
    //  Parameters:
    //    pSubscriber:
    //      Pointer to the component reporting this event.
    //
    //    eStatus:
    //      The actual subscription state.
    //
    //  Description:
    //      Reports the subscription status.
    //
    //  See Also:
    //      IN ISceSubscriberMgr::ESubscriptionStatus
    //
    //==========================================================================
    virtual void EvSubscriptionStatus(IN IUaImppPresenceSubscriber* pSubscriber,
                                      IN ISceSubscriberMgr::ESubscriptionStatus eStatus) = 0;

    //==========================================================================
    //==
    //==  EvPresenceStatusUpdate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the presence status of a peer resulting from a subscription
    //      to a single peer.
    //
    //  Parameters:
    //    pSubscriber:
    //      Pointer to the component reporting this event.
    //
    //    pszResourceListUri:
    //      Pointer to the uri of the list onto which the watched resource
    //      belongs. Can be NULL if the subscription was not established to a
    //      resource list.
    //
    //    pPidfDocument:
    //      The PIDF document that was updated.
    //
    //  Description:
    //      Reports the presence status of a peer resulting from a subscription
    //      to a single peer.
    //
    //      This is reported only when the subscriber establishes a subscription
    //      to a single peer. See EvUpdatedPresenceElement, EvNewPresenceElement
    //      and EvDeletedPresenceElement for events reported when subscribing to
    //      a resource list.
    //
    //==========================================================================
    virtual void EvPresenceStatusUpdate(IN IUaImppPresenceSubscriber* pSubscriber,
                                        IN IScePidfDocument* pPidfDocument) = 0;


    //==SDOC====================================================================
    //==
    //==  EvUpdatedPresenceElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the presence state or subscription state of a resource
    //      from a resource list was updated.
    //
    //  Parameters:
    //    pSubscriber:
    //      Pointer to the component reporting this event.
    //
    //    pszResourceUri:
    //      Pointer to the URI of the resource. Will never be NULL.
    //
    //    pszResourceListUri:
    //      Pointer to the URI of the list to which the resource identified by
    //      pszResourceListUri was assigned. Will never be NULL.
    //
    //    eSubscriptionState:
    //      The state of the subscription to the resource.
    //
    //    pPidfDocument:
    //      Pointer to the PIDF document that holds the presence information for
    //      this resource. Can be NULL if no information is yet available or if
    //      this remote subscription is a subscription to an other resource list.
    //
    //  Description:
    //      Reports that the presence state or subscription state of a resource
    //      from a resource list was updated.
    //
    //
    //==EDOC====================================================================
    virtual void EvUpdatedPresenceElement(IN IUaImppPresenceSubscriber* pSubscriber,
                                          IN const char* pszResourceUri,
                                          IN const char* pszResourceListUri,
                                          IN ISceSubscriberMgr::ESubscriptionStatus eSubscriptionState,
                                          IN IScePidfDocument* pPidfDocument) = 0;

    //==SDOC====================================================================
    //==
    //==  EvNewPresenceElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a new presence element is available as a result of
    //      subscribing to a list of resources.
    //
    //  Parameters:
    //    pSubscriber:
    //      Pointer to the component reporting this event.
    //
    //    pszResourceUri:
    //      Pointer to the URI of the resource. Will never be NULL.
    //
    //    pszResourceListUri:
    //      Pointer to the URI of the list to which the resource identified by
    //      pszResourceListUri was assigned. Will never be NULL.
    //
    //    peSubscriptionState:
    //      Pointer to the state of the subscription to the resource. Can be
    //      NULL if no information is yet available.
    //
    //    pPidfDocument:
    //      Pointer to the PIDF document that holds the presence information for
    //      this resource. Can be NULL if no information is yet available or if
    //      this remote subscription is a subscription to an other resource list.
    //
    //  Description:
    //      Reports that a new presence element is available as a result of
    //      subscribing to a list of resources.
    //
    //==EDOC====================================================================
    virtual void EvNewPresenceElement(IN IUaImppPresenceSubscriber* pSubscriber,
                                      IN const char* pszResourceUri,
                                      IN const char* pszResourceListUri,
                                      IN ISceSubscriberMgr::ESubscriptionStatus* peSubscriptionState,
                                      IN IScePidfDocument* pPidfDocument) = 0;


    //==SDOC====================================================================
    //==
    //==  EvDeletedPresenceElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the subscriber detected that a previously reported
    //      element was removed from the list subscription.
    //
    //  Parameters:
    //    pSubscriber:
    //      Pointer to the component reporting this event.
    //
    //    pszResourceUri:
    //      Pointer to the URI of the resource. Will never be NULL.
    //
    //    pszResourceListUri:
    //      Pointer to the URI of the list to which the resource identified by
    //      pszResourceListUri was assigned. Will never be NULL.
    //
    //    pPidfDocument:
    //      Pointer to the last presence information held by the subscriber for
    //      this element. Can be NULL if no presence information was ever
    //      received or if this element corresponds to a subscription to an
    //      other resource list.
    //
    //  Description:
    //      Reports that the subscriber detected that a previously reported
    //      element was removed from the list subscription.
    //
    //==EDOC====================================================================
    virtual void EvDeletedPresenceElement(IN IUaImppPresenceSubscriber* pSubscriber,
                                          IN const char* pszResourceUri,
                                          IN const char* pszResourceListUri,
                                          IN IScePidfDocument* pPidfDocument) = 0;


//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IUaImppPresenceSubscriberMgr() {};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IUaImppPresenceSubscriberMgr() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IUaImppPresenceSubscriberMgr(IN const IUaImppPresenceSubscriberMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IUaImppPresenceSubscriberMgr& operator=(IN const IUaImppPresenceSubscriberMgr& rSrc);


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

#endif //-- #ifndef MXG_IUAIMPPPRESENCESUBSCRIBERMGR_H

