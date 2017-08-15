//==============================================================================
//==============================================================================
//
//      Copyright(c) 2007 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==============================================================================

//==============================================================================
//
// Portions of this software Copyright (c) 2008-2011 Broadcom
// 
//==============================================================================

#ifndef MXG_IUASSPREGISTRATIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPREGISTRATIONMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CNameAddr;
class IUaSspRegistration;
class ISceSubscriber;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspRegistrationMgr
//========================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   This is the interface through which the registration component reports its
//   events.
//
// Description:
//  This is the interface through which the registration component reports its
//  events about the registration state of a user.
//
// Location:
//   UaSsp/IUaSspRegistrationMgr.h
//
// See Also:
//   IUaSspRegistration, IUaSspRegistrationConfig
//
//==============================================================================
class IUaSspRegistrationMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==========================================================================
    //<GROUP TID_PACKAGE_UASSP_STRUCTSENUMS>
    //
    // Summary:
    //   The various registration statuses that can be reported by the
    //   registration component.
    //
    // Description:
    //  This enumeration describes the various registration statuses that can be
    //  reported by the registration component.
    //
    //==========================================================================
    //
    //------------------------------------------------------------------------
    enum EStatus
    {
        // Description:
        // The user is unregistered.
        eUNREGISTERED,

        // Description:
        // The user is on probation. The component will retry registering after
        // a network provided timeout.
        eUNREGISTERED_ONPROBATION,

        // Description:
        // The user is unregistered and the component is trying to register.
        eUNREGISTERED_TRYING,

        // Description:
        // The user is registered.
        eREGISTERED,

        // Description:
        // The user is registered, and the registration is being refreshed.
        eREGISTERED_REFRESHING,

        // Description:
        // The registration has failed and cannot be retried successfully. This
        // usually denotes configuration problems either on the local device or
        // on the server. The registration should not be retried with the
        // current parameters.
        eREGISTRATION_FAILED,

        // Description:
        // The registration was forcefully rejected by the network and cannot
        // be retried.
        eREGISTRATION_FAILED_REJECTED,

        // Description:
        // The registration was terminated as a result to calling
        // IUaSspRegistration::Unregister or IUaSspRegistration::Terminate.
        eTERMINATED

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
        ,eUNREGISTERED_TIMEOUT
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS
    };


    //==========================================================================
    //==
    //==  EvRegistrationStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the registration status.
    //
    //  Parameters:
    //      pRegistration:
    //       The registration component reporting this event.
    //
    //      eStatus:
    //       The status of the registration.
    //
    //  Description:
    //      Reports the registration status of the user. This event can be
    //      reported even if the registration status does not change. For
    //      instance, the eREGISTERED status can be reported once the first
    //      registration succeeds and each time the registration is successfully
    //      refreshed.
    //
    //      When eStatus is eUNREGISTERED, the registration component
    //      will try to re-register the user at the specified interval.
    //
    //      When eStatus is either eTERMINATED or eREGISTRATION_FAILED, the
    //      registration component does not attempt periodic registration
    //      retries.
    //
    //      It is possible for a registration to move directly from the state
    //      eREGISTERED to eUNREGISTERED or eREGISTRATION_FAILED without
    //      reporting any other events.
    //
    //  See Also:
    //      IUaSspRegistrationConfig
    //
    //==========================================================================
    virtual void EvRegistrationStatus(IN IUaSspRegistration* pRegistration,
                                      IN EStatus eStatus) = 0;

// BRCM_CUSTOM - [add] Notify mgr when registration is about to expire and
//                     and will be refreshed.
    //==========================================================================
    //==
    //==  EvExpiring
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the registration is going to be refreshed
    //
    //  Parameters:
    //      pRegistration:
    //       The registration component reporting this event.
    //
    //  Description:
    //      This event notifies the application that the registration is going
    //      to be refrehsed.
    //
    //==========================================================================                                      
    virtual void EvExpiring(IN IUaSspRegistration* pRegistration) = 0;
// BRCM_CUSTOM - [end] Notify mgr when registration is about to expire and
//                     and will be refreshed.    

// BRCM_CUSTOM - [add] P-Associated-URI handling support
    //==========================================================================
    //==
    //==  EvPauReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the reception of a P-Associated-URI in a successful
    //      registration.
    //
    //  Parameters:
    //      pRegistration:
    //       The registration component reporting this event.
    //
    //      raddrPau:
    //       The topmost address in the P-Associated-URI header.
    //
    //  Description:
    //      This event notifies the application a P-Assoicated-URI is received
    //      upon a successful registration.
    //
    //==========================================================================
    virtual void EvPauReceived(IN IUaSspRegistration* pRegistration,
                               IN const CNameAddr& raddrPau) = 0;
// BRCM_CUSTOM - [end] P-Associated-URI handling support

    //==========================================================================
    //==
    //==  EvInternalSubscriberCreated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the creation of the internal subscriber component.
    //
    //  Parameters:
    //      pRegistration:
    //       The registration component reporting this event.
    //
    //      pSubscriber:
    //       The newly created subscriber component.
    //
    //  Description:
    //      Reports the creation of the internal subscriber component to the
    //      application. This event is meant to let the application set the
    //      internal subscriber component's configuration before its SIP
    //      context gets created.
    //
    //      The application MUST be cautious NOT to modify the subscriber
    //      component's manager. The subscriber component's manager MUST be the
    //      registration component to ensure proper behaviour of the "reg-event"
    //      feature.
    //
    //      This event is ALWAYS reported when the internal subscriber component
    //      is created.
    //
    //      If the application has no use for this event, it can safely
    //      implement it has an empty method. The example below depicts this.
    //
    //      <CODE>
    //      void CSomeApplicationClass::EvInternalSubscriberCreated(IN IUaSspRegistration* pRegistration,
    //                                                              IN ISceSubscriber* pSubscriber)
    //      {
    //          MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pRegistration);
    //          MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pSubscriber);
    //      }
    //      </CODE>
    //
    //==========================================================================
    virtual void EvInternalSubscriberCreated(IN IUaSspRegistration* pRegistration,
                                             IN ISceSubscriber* pSubscriber) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IUaSspRegistrationMgr(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IUaSspRegistrationMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IUaSspRegistrationMgr(IN const IUaSspRegistrationMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IUaSspRegistrationMgr& operator=(IN const IUaSspRegistrationMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPREGISTRATIONMGR_H

