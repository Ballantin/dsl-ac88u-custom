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
//==============================================================================
//
// Portions of this software Copyright (c) 2008-2012 Broadcom
// 
//==============================================================================
#ifndef MXG_IUASSPREGISTRATION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPREGISTRATION_H
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
class ISceUserConfig;
class IUaSspRegistrationMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IUaSspRegistration);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspRegistration
//========================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   The registration interface is used to register a user on the network.
//
// Description:
//  The registration component is used to register a user on the network.
//
//  Before the registration component is used, it must be properly configured.
//  SetManager must be called to configure the manager to which events about
//  this registration component will be reported. SetConfiguration must also be
//  called to associate the IUaSspRegistrationConfig object with this component.
//
//  Once configured, Register can be called to start the registration
//  procedures on the network.
//
//  This component will periodically try to refresh the registration it manages
//  according to the negotiated expiration delay with the server. If an error
//  occurs, the component will periodically try to re-register the user unless
//  an unrecoverable error is detected, in which case
//  IUaSspRegistrationMgr::EvRegistrationStatus will specify
//  eREGISTRATION_FAILED.
//
//  This component supports the configuration of more than one registrar. The
//  initial registration procedures always begin with the first registrar in the
//  registrar list. If this registrar fails to respond, the next registrar is
//  tried and so on, until a registrar accepts the registration request, or
//  until a registrar reports a final negative response that tells this
//  component that it cannot register at all. If refreshing a registration
//  fails, the component will iterate through the list of registrars until it
//  either finds a working registrar or returns to the last working registrar,
//  at which time the component will take a pause, as configured
//  through IUaSspRegistrationConfig::SetRegistrationRetryDelay.
//
//  Once this component has reported the eREGISTRATION_FAILED or eTERMINATED
//  event, the component can no longer be used and should be released.
//
//  The registration component can be configured to monitor its own registration
//  state as seen by the registrar by calling
//  IUaSspRegistrationConfig::SetRegistrationStateMonitoringMode. When
//  registration monitoring is enabled, the component will subscribe to its own
//  registration state, allowing the detection of network initiated
//  de-registration. If the registration component detects a network initiated
//  de-registration, it tries to re-register if the unregistration event
//  permits so.
//
//  Unregister can be used when the corresponding user's registration must be
//  removed. This will also force unsubscription of registration state
//  monitoring if it was enabled.
//
// Location:
//   UaSsp/IUaSspRegistration.h
//
// See Also:
//   IUaSspRegistrationConfig, IUaSspRegistrationMgr
//
//==============================================================================
class IUaSspRegistration : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:

//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaSspRegistration);
//M5T_INTERNAL_USE_END


    //   << Registration Management >>

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which events are reported for this
    //      registration.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. It must not be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT:
    //         pMgr is NULL.
    //      - resS_OK:
    //         The manager has been set.
    //
    //  Description:
    //      Configures the manager to which events are reported for this
    //      registration object.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN IUaSspRegistrationMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the registration object with a user configuration object.
    //
    //  Parameters:
    //    pConfig:
    //      Pointer to the ECOM configuration object for this user.
    //
    //  Returns:
    //      - resS_OK:
    //         The configuration has been set correctly.
    //      - resFE_FAIL:
    //         A problem occurred when trying to set the configuration.
    //      - resFE_INVALID_ARGUMENT:
    //         Parameter is NULL, or it was already set.
    //      - resFE_INVALID_STATE:
    //         The registrar list is NULL or empty.
    //
    //  Description:
    //      Configures the registration with a configuration object.
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
    //      Gets the registration's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the registration.
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
    //      Sets an opaque paramerer to the registration.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the registration. It can be used by
    //      the application to store any information.
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
    //      Gets the opaque parameter to the registration.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the registration. It can be used by
    //      the application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==========================================================================
    //==
    //==  Register
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers the user to a SIP registrar.
    //
    //  Returns:
    //      - resS_OK:
    //         The registration request has been sent.
    //      - resFE_FAIL:
    //         The registration request could not be sent.
    //      - resFE_INVALID_STATE:
    //         Configuration is not set or already registered.
    //
    //  Description:
    //      Registers the user to a SIP registrar.
    //
    //  See Also:
    //      Unregister
    //
    //==========================================================================
    virtual mxt_result Register() = 0;

    //==========================================================================
    //==
    //==  Refresh
    //==
    //==========================================================================
    //
    //  Summary:
    //      Refreshes the current user registration.
    //
    //  Returns:
    //      - resS_OK:
    //         The registration request has been sent.
    //      - resFE_FAIL:
    //         The registration request could not be sent.
    //      - resFE_INVALID_STATE:
    //         Configuration is not set or the user is not registered yet.
    //
    //  Description:
    //      Refreshes the current user registration.
    //
    //  See Also:
    //      Register, Unregister
    //
    //==========================================================================
    virtual mxt_result Refresh() = 0;

    //==========================================================================
    //==
    //==  Unregister
    //==
    //==========================================================================
    //
    //  Summary:
    //      Unregisters the user from its SIP registrar.
    //
    //  Returns:
    //      - resS_OK:
    //         The unregistration request has been sent.
    //      - resFE_FAIL:
    //         The unregistration request could not be sent.
    //      - resFE_INVALID_STATE:
    //         Configuration is not set or still not registered.
    //
    //  Description:
    //      Unregisters the user from its SIP registrar.
    //
    //  See Also:
    //      Register
    //
    //==========================================================================
    virtual mxt_result Unregister() = 0;

    //==========================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Immediately terminates the registration component.
    //
    //  Description:
    //      Immediately terminates the registration component. The registration
    //      and the subscription that this registration component is handling
    //      are immediately removed without sending anything on the network (no
    //      unREGISTER or unSUBSCRIBE is sent).
    //
    //      The manager must expect a call to EvRegistrationStatus(eTERMINATED)
    //      while this method is called, at which time the manager can release
    //      the reference it has on the registration component.
    //
    //      This method should only be used on abnormal scenarios where the
    //      user somehow has knowledge that the registration and the
    //      subscription (if applicable) are terminated or when it knows that
    //      an unREGISTER and unSUBSCRIBE request would fail. The Unregister
    //      method should otherwise be used.
    //
    //  See Also:
    //      Unregister
    //
    //==========================================================================
    virtual void Terminate() = 0;

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    //==========================================================================
    //==
    //==  TerminateImmediately
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminate this call component immediately and silently.
    //
    //==========================================================================
    virtual void TerminateImmediately() = 0;
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  QuerySubscriber
    //==
    //==========================================================================
    //
    //  Summary:
    //      Queries the internal reg-event subscriber component for the
    //      requested extension interface.
    //
    //  Parameters:
    //    iidRequested:
    //      The requested interface identification. Note that
    //      IID_ISceSubscriber must not be requested as it must stay internal.
    //
    //    ppInterface:
    //      The returned ECOM interface requested. Can be NULL if the
    //      interface could not be found.
    //
    //  Returns:
    //      resS_OK if the query is successful, an error otherwise.
    //
    //  Description:
    //      Queries the internal reg-event subscriber component for the
    //      requested extension interface.
    //
    //      Note that this returns an interface to a component that is used
    //      internally by the registration component. Only the extension
    //      interfaces should be used on this internal component in order to
    //      avoid any coordination issues between the internal subscriber
    //      component and the registration component. The use of the
    //      ISceSubscriber interface of this internal component is undefined and
    //      will most probably yield random results.
    //
    //      This can only be used if the correct configuration is set on the
    //      registration component to use its internal subscriber component.
    //
    //      Can only be called after SetConfiguration, otherwise
    //      resFE_INVALID_STATE will be returned along with a NULL pointer.
    //
    //==========================================================================
    virtual mxt_result QuerySubscriber(IN mxt_iid iidRequested,
                                       OUT void** ppInterface) = 0;
//M5T_INTERNAL_USE_END

// BRCM_CUSTOM - [add] Expose internal SIP context
    //==========================================================================
    //==
    //==  RegistrationContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the registration component sip-context for the
    //      requested extension interface.
    //
    //  Parameters:
    //    ppContext:
    //      The returned ECOM interface requested. Can be NULL if the
    //      interface could not be found.
    //
    //  Returns:
    //      resS_OK if the query is successful, an error otherwise.
    //
    //  Description:
    //      Retrieves the registration component sip-context for the
    //      requested extension interface.
    //
    //==========================================================================
    virtual mxt_result RegistrationContext(OUT ISipContext** ppContext) = 0;
// BRCM_CUSTOM - [end] Expose internal SIP context

// BRCM_CUSTOM - [add] Add external failover support
    //==============================================================================
    //==
    //==  UpdateRegistrar
    //==
    //==============================================================================
    //
    //  Description:
    //      Updates the User-Agent Service current target to the current registrar.
    //
    //==============================================================================
    virtual void UpdateRegistrar() = 0;
// BRCM_CUSTOM - [end] Add external failover support

    //==========================================================================
    //==
    //==  QuerySubscriber
    //==
    //==========================================================================
    //
    //  Summary:
    //      Queries the internal reg-event subscriber component for the
    //      requested extension interface.
    //
    //  Parameters:
    //    ppInterface:
    //      The returned ECOM interface requested. Can be NULL if the
    //      interface could not be found.
    //
    //  Returns:
    //      resS_OK if the query is successful, an error otherwise.
    //
    //  Description:
    //      Queries the internal reg-event subscriber component for the
    //      requested extension interface.
    //
    //      Note that this returns an interface to a component that is used
    //      internally by the registration component. Only the extension
    //      interfaces should be used on this internal component in order to
    //      avoid any coordination issues between the internal subscriber
    //      component and the registration component. The use of the
    //      ISceSubscriber interface of this internal component is undefined and
    //      will most probably yield random results.
    //
    //      This can only be used if the correct configuration is set on the
    //      registration component to use its internal subscriber component.
    //
    //      Can only be called after SetConfiguration, otherwise
    //      resFE_INVALID_STATE will be returned along with a NULL pointer.
    //
    //      A reference is automaticaly added on success. The user is
    //      responsible to release it when this interface is no longer required.
    //
    //==========================================================================
    template<class _Type> mxt_result QuerySubscriber(OUT _Type** ppInterface)
    {
        // Has to be inlined here because Visual C++ 6.0 does not like out of class
        // inlined specialized template method.
        if (ppInterface != NULL)
        {
            return QuerySubscriber((*ppInterface)->GetIID(),
                                   OUT reinterpret_cast<void**>(ppInterface));
        }
        return resFE_INVALID_ARGUMENT;
    }

// BRCM_CUSTOM - [add] Add query for registration expiration time
    //==========================================================================
    //==
    //==  GetRegExpirationTime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the registration expiration time.  The time returned is the 
    //      time remaining before the registration will expire. 
    //
    //  Returns:
    //      The registration expiration time in seconds.
    //
    //==========================================================================
    virtual uint32_t GetRegExpirationTime() = 0;
// BRCM_CUSTOM - [end] Add query for registration expiration time


// BRCM_CUSTOM - [add] Add query for re-registration time
    //==========================================================================
    //==
    //==  GetReRegTime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the re-registration time.  The time returned is the time 
    //      remaining before the registration will be refreshed.  If random  
    //      re-registration is configured, then the re-registration time would  
    //      be calculated based on randomly selected value between 60% and 90% of  
    //      the registration expiration time as returned in the previous 200 OK.
    //
    //  Returns:
    //      The re-registration time in seconds.
    //
    //==========================================================================
    virtual uint32_t GetReRegTime() = 0;
// BRCM_CUSTOM - [end] Add query for re-registration time


//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaSspRegistration() {}

    // Summary:
    //  Destructor.
    virtual ~IUaSspRegistration() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaSspRegistration(IN const IUaSspRegistration& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaSspRegistration& operator=(IN const IUaSspRegistration& rSrc);


//-- Hidden Data Members.

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPREGISTRATION_H
