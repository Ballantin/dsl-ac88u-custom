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
#ifndef MXG_IUASSPMWI_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPMWI_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif



MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSceCallerPreferences;
class CSipPacket;
class ISceUserConfig;
class IUaSspMwiMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IUaSspMwi);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspMwi
//========================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   This interface defines the possible operations on components offering the
//   MWI service.
//
// Description:
//  This interface defines the possible operations on components offering the
//  MWI service. As with all SIP Client Engine components, the MWI service must
//  be configured before it is used. A MWI configuration object must be set
//  through SetConfiguration and a manager must be set through SetManager.
//
//  This interface is implemented by more than one component. It is however
//  possible that some methods are implemented by one component and not by
//  another.
//
//#Subscribing MWI Component#
//
//  When using this component, the user of the component must call Subscribe
//  when ready to start the subscription process. The MWI component will
//  subcribe to the MWI server and periodically refresh its subscription until
//  either Unsubscribe is called or an unrecoverable error is detected.
//
//  The component will report its subscription status through
//  IUaSspMwiMgr::EvSubscriptionStatus, while it will report the actual state of
//  the mailbox through IUaSspMwiMgr::EvMailboxStatus.
//
//#Unsolicited MWI Component#
//
//  When using this component, there is no need to call Subscribe or
//  Unsubscribe. The MWI component will be in a state where it will monitor
//  unsolicited notifications from the network.
//
//  This component will not report any IUaSspMwiMgr::EvSubscriptionStatus
//  events (besides the EvSubscriptionStatus(eTERMINATED) that is called when
//  Terminate was completed), while it will still report the actual state of the
//  mailbox through IUaSspMwiMgr::EvMailboxStatus.
//
//  Here is the lifecycle of this component:
//      - It is created. The reference on the component is kept until
//        EvSubscriptionStatus(eTERMINATED) is called on the manager;
//      - SetManager and SetConfiguration is called on it;
//      - It notifies its manager about the received unsolicited MWI
//        notifications;
//      - Terminate is called on it;
//      - EvSubscriptionStatus(eTERMINATED) is called on the manager;
//      - The reference on the component is released.
//
//  Note that the reference on the unsolicited MWI component must only be
//  released after EvSubscriptionStatus(eTERMINATED) is called on its manager.
//
// Location:
//   UaSsp/IUaSspMwi.h
//
// See Also:
//   IUaSspMwiMgr, IUaSspMwiConfig
//
//==============================================================================
class IUaSspMwi : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaSspMwi);
//M5T_INTERNAL_USE_END


    //   << Mwi Management >>

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which events are reported for this
    //      MWI object.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. It must not be NULL.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT:
    //       pMgr is NULL.
    //
    //      resS_OK:
    //       The manager has been set.
    //
    //  Description:
    //      Configures the manager to which events are reported for this
    //      MWI object.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN IUaSspMwiMgr* pMgr) = 0;


    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the MWI object with a user configuration object.
    //
    //  Parameters:
    //    pConfig:
    //      Pointer to the ECOM configuration object for this user.
    //
    //  Returns:
    //      resS_OK:
    //       The configuration has been set successfully.
    //
    //  Description:
    //      Configures the MWI object with a configuration object.
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
    //      Gets the MWI's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the MWI.
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
    //      Sets an opaque paramerer to the IUaSspMwi.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the IUaSspMwi. It can be used by the
    //      application to store any information.
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
    //      Gets the opaque parameter to the IUaSspMwi.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the IUaSspMwi. It can be used by the
    //      application to store any information.
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
    //      Subscribes the user to the MWI service, allowing the reception of
    //      MWI events.
    //
    //  Parameters:
    //      pCallerPreferences:
    //          The caller preferences to apply to the outgoing subscribe
    //          request.
    //
    //          Ownership is taken. May be NULL.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_NOT_IMPLEMENTED: Normal return code for the unsolicited MWI
    //          component.
    //      - resFE_INVALID_ARGUMENT: pResourceAor or uSubscriptionPeriodS 
    //          cannot be 0.
    //      - resFE_INVALID_STATE: Configuration not completed.
    //      - Other: Method failure.
    //
    //  Description:
    //      Subscribes the user to the MWI service, allowing the reception of 
    //      MWI events.
    //
    //      This method always fails on the unsolicited MWI component.
    //      
    //  See Also:
    //      Unsubscribe
    //
    //==========================================================================
    virtual mxt_result Subscribe(IN TO CSceCallerPreferences* pCallerPreferences) = 0;
    
    
    //==========================================================================
    //==
    //==  Unsubscribe
    //==
    //==========================================================================
    //
    //  Summary:
    //      Unsubscribes the user from the MWI service.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_NOT_IMPLEMENTED: Normal return code for the unsolicited MWI
    //          component.
    //      - resFE_INVALID_STATE: Configuration not completed.
    //      - Other: Method failure.
    //
    //  Description:
    //      Unsubscribes the user from the MWI service.
    //
    //      This method always fails on the unsolicited MWI component.
    //
    //  See Also:
    //      Subscribe
    //
    //==========================================================================
    virtual mxt_result Unsubscribe() = 0;

    //==========================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates the subscription without un-subscribing. 
    //
    //  Description:
    //      Terminates the subscription without un-subscribing. 
    //      This is used in cases where the application MUST shut down 
    //      immediately or cannot un-subscribe.
    //
    //      The manager must expect a call to EvSubscriptionStatus(eTERMINATED)
    //      while this method is called, at which time the manager can release
    //      the reference it has on the MWI component.
    //
    //  See Also:
    //      Subscribe, Unsubscribe
    //
    //==========================================================================
    virtual void Terminate() = 0;

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  HandleIncomingNotify
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the MWI to proceed with an incoming NOTIFY.
    //
    //  Parameters:
    //      rRequest:
    //       The request to handle.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT:
    //       rRequest is not a NOTIFY request.
    //
    //      resFE_FAIL:
    //       Cannot call HandleIncomingNotify at this time.
    //          
    //      resS_OK:
    //       NOTIFY is being handled.
    //
    //  Description:
    //      This method must be called after the MWI has been configured with
    //      its user configuration object (see SetConfiguration) and with its
    //      manager (see SetManager).
    //
    //      It informs the MWI that it can proceed with the incoming NOTIFY
    //      request.
    //
    //      This method does not have to be called by the application. It is
    //      called automatically by the UaSspMwiHandler.
    //
    //  See Also:
    //      SetConfiguration, SetManager
    //
    //==========================================================================
    virtual mxt_result HandleIncomingNotify(IN const CSipPacket& rRequest) = 0;
//M5T_INTERNAL_USE_END

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaSspMwi() {}

    // Summary:
    //  Destructor.
    virtual ~IUaSspMwi() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaSspMwi(IN const IUaSspMwi& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaSspMwi& operator=(IN const IUaSspMwi& rSrc);


//-- Hidden Data Members.
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPMWI_H

