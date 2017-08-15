//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCENEWNOTIFIERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCENEWNOTIFIERMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class ISceNotifier;
class ISceUserConfig;
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceNewNotifierMgr
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This manager interface defines the events that are reported to the M5T SIP
//   Client Engine user when a new subscription request is received.
//
// Description:
//  This manager interface defines the events that are reported to the M5T SIP
//  Client Engine user when a new subscription request is received.
//
// Location:
//   SceCoreComponents/ISceNewNotifierMgr.h
//
// See Also:
//   ISceNotifier
//
//==============================================================================
class ISceNewNotifierMgr
{
//-- Friend Declarations.


//-- Published Interface.
public:


    //==========================================================================
    //==
    //==  EvNewNotifierObject
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager that a new notifier component has been created.
    //
    //  Parameters:
    //    pNotifier:
    //      The new notifier component.
    //
    //    pUserConfig:
    //      The user configuration object that was associated with the new
    //      component. Can be NULL, in which case it is up to the manager to
    //      associate a user configuration object with the component.
    //
    //    rSubscribeRequest:
    //      Reference to the incoming SUBSCRIBE request.
    //
    //  Description:
    //      Reports that a new notifier component has been created.
    //      
    //      The implementation of this event must do the following:
    //      - If pUserConfig is NULL and the implementation wants to handle
    //        the incoming subscription request for unregistered users, then it
    //        should associate a user configuration object with the component
    //        by using ISceNotifier::SetConfiguration. The proper user
    //        configuration object can be found or created based on the
    //        information held by rSubscribeRequest. If pUserConfig is NULL and
    //        the implementation does not set a valid user configuration object,
    //        the subscription request will be automatically rejected with a
    //        "404 User not found" response.
    //      - The implementation must set a valid ISceNotifierMgr by
    //        using ISceNotifier::SetManager.
    //      
    //      The implementation must not perform any other action on pNotifier
    //      within the context of this event.
    //
    //      After this event is reported, the subscription will either progress
    //      or be rejected, resulting in either ISceNotifierMgr::EvSubscribed or
    //      ISceNotifierMgr::EvNotifierTerminated being reported to the
    //      configured notifier manager.
    //
    //      The subscription request can be automatically rejected, after the 
    //      component is created, for various reasons:
    //          - No user configuration object was configured
    //          - The subscription required an unsupported extension
    //          - The request is not from a trusted source or does not go through 
    //            a trusted proxy
    //          - The new subscription request is the result of a
    //            retransmission of a SUBSCRIBE request that is already being
    //            managed by another notifier component.
    //
    //  See Also:
    //      ISceNotifier, ISceNotifierMgr
    //
    //==========================================================================
    virtual void EvNewNotifierObject(IN ISceNotifier* pNotifier,
                                     IN ISceUserConfig* pUserConfig,
                                     IN const CSipPacket& rSubscribeRequest) = 0;





//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceNewNotifierMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISceNewNotifierMgr() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceNewNotifierMgr(IN const ISceNewNotifierMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceNewNotifierMgr& operator=(IN const ISceNewNotifierMgr& rSrc);


//-- Hidden Data Members.
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCENEWNOTIFIERMGR_H
