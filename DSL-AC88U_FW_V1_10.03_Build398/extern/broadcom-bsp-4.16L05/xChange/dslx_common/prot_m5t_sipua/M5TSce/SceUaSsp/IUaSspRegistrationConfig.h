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
// Portions of this software Copyright (c) 2008-2011 Broadcom
// 
//==============================================================================

#ifndef MXG_IUASSPREGISTRATIONCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPREGISTRATIONCONFIG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CHostPort;
class CSipUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IUaSspRegistrationConfig);

// The default subscription period in seconds.
const unsigned int g_uSCE_REG_EVENT_SUBSCRIPTION_PERIOD_S = 3761;
const unsigned int g_uSCE_REG_EVENT_DEFAULT_SUBSCRIPTION_RETRY_DELAY_S = 60;

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspRegistrationConfig
//========================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   This interface defines the user configuration options for registration
//   management.
//
// Description:
//  This interface defines the user configuration options for registration
//  management.
//
// Location:
//   SceUaSsp/IUaSspRegistrationConfig.h
//
// See Also:
//   IUaSspRegistration, IUaSspRegistrationMgr
//
//==============================================================================
class IUaSspRegistrationConfig : public IEComUnknown
{
//-- Friend Declarations.

//-- Published Interface.
public:

//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaSspRegistrationConfig);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  AddRegistrar
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures a registrar address.
    //
    //  Parameters:
    //    pRegistrar:
    //      Pointer to the registrar address. Ownership is TAKEN.
    //
    //  Returns:
    //      - resS_OK:
    //         The registrar has been successfully added.
    //      - resFE_FAIL:
    //         The registrar could not be added.
    //
    //  Description:
    //      Configures the address of the SIP registrar, where REGISTER
    //      requests will be sent. The new registrar will be added at the end
    //      of the list.
    //
    //      Note that the CHostPort class can be configured with either an IPv6
    //      address, IPv4 address or FQDN.
    //
    //      This method MUST NOT be called when this IUaSspRegistrationConfig is
    //      associated to a IUaSspRegistration component. In the case where a
    //      registration component is associated to this configuration, the
    //      component must be completely terminated before this method is
    //      called.
    //
    //  See Also:
    //      SetRegistrarList, GetRegistrarList
    //
    //==========================================================================
    virtual mxt_result AddRegistrar(IN TO CHostPort* pRegistrar) = 0;

    //==========================================================================
    //==
    //==  SetRegistrarList
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures a list of registrars.
    //
    //  Parameters:
    //    pvecpRegistrar:
    //      Pointer to the list of registrars. Note that the CVector must have
    //      the ownership of the CHostPort objects it contains.
    //
    //      NULL to reset the list of registrars.
    //
    //  Description:
    //      Configures a list of registrar. When the list is non-NULL and not
    //      empty, the behaviour of the registration component is to try to
    //      register to a single registrar among the list of available
    //      registrars. When unable to register to a registrar, it tries the
    //      next one in the list. If all registrars in the list have been tried,
    //      starting from the index of the last working registrar, the component
    //      pauses for a time and then starts again registering at the current
    //      registrar index.
    //
    //      The registrars configured in this list must all be ready to handle
    //      registrations from the same users, specifically for the same AOR.
    //      The AOR being registered by the registration component is not
    //      updated when trying a different server, only the request-URI.
    //
    //      This method MUST NOT be called when this IUaSspRegistrationConfig is
    //      associated to a IUaSspRegistration component. In the case where a
    //      registration component is associated to this configuration, the
    //      component must be completely terminated before this method is
    //      called.
    //
    //  See Also:
    //      AddRegistrar, GetRegistrarList
    //
    //==========================================================================
    virtual void SetRegistrarList(IN TO CVector<CHostPort*>* pvecpRegistrar) = 0;

    //==========================================================================
    //==
    //==  GetRegistrarList
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the list of registrars.
    //
    //  Returns:
    //      A vector holding all registrars addresses.
    //
    //  Description:
    //      Gets the list of configured registrars.
    //
    //      This returned list MUST NOT be modified when this
    //      IUaSspRegistrationConfig is associated to a IUaSspRegistration
    //      component. In the case where a registration component is associated
    //      to this configuration, the component must be completely terminated
    //      before the list is modified.
    //
    //  See Also:
    //      SetRegistrarList, AddRegistrar
    //
    //==========================================================================
    virtual CVector<CHostPort*>* GetRegistrarList() = 0;

    //==========================================================================
    //==
    //==  SetRegistrationRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the time to wait after a failed registration attempt and
    //      the next one.
    //
    //  Parameters:
    //    uNbSeconds:
    //      The number of seconds to wait.
    //
    //  Description:
    //      Configures the number of seconds to wait after a failed registration
    //      attempt and the next one.
    //
    //      This parameter is used differently depending on whether a single
    //      registrar is configured (SetRegistrar) or more than one is
    //      configured (SetRegistrarList).
    //
    //      If a single registrar is configured, the registration component will
    //      wait for uNbSeconds after a registration failure or timeout before
    //      it tries again to register.
    //
    //      If more than one registrar is configured, the registration component
    //      will wait for uNbSeconds after it has unsuccessfully iterated
    //      through the whole registrar list. This means that after each
    //      unsuccessful attempt, the next registrar is immediately tried,
    //      unless this registrar was the first one unsuccessfully contacted
    //      while iterating in the list.
    //
    //      This defaults to 60 seconds.
    //
    //  See Also:
    //      GetRegistrationRetryDelay, SetRegistrar, SetRegistrarList
    //
    //==========================================================================
    virtual void SetRegistrationRetryDelay(IN uint32_t uNbSeconds) = 0;

    //==========================================================================
    //==
    //==  GetRegistrationRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured registration retry delay.
    //
    //  Returns:
    //      The registration retry delay.
    //
    //  Description:
    //      Gets the configured registration retry delay.
    //
    //  See Also:
    //      SetRegistrationRetryDelay
    //
    //==========================================================================
    virtual uint32_t GetRegistrationRetryDelay() const = 0;

    //==========================================================================
    //==
    //==  SetRegistrationExpirationTime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the requested expiration time in register requests.
    //
    //  Parameters:
    //    uNbSeconds:
    //      The number of seconds to set as expiration time of registrations.
    //
    //  Description:
    //      Configures the expiration time to request in register requests.
    //
    //      It is possible for a server to reject the proposed registration
    //      expiration time if it is too short, in which case the registration
    //      component uses the shortest possible value supported by the server.
    //
    //      It is possible for a server to accept the registration while
    //      reducing the expiration time, in which case the registration
    //      component refreshes the registration based on the server provided
    //      expiration time.
    //
    //      If the expiration value is set to zero, then the expires parameter
    //      will not be sent inside the Contact header.
    //
    //      This defaults to 3600 seconds (one hour).
    //
    //  See Also:
    //      GetRegistrationExpirationTime
    //
    //==========================================================================
    virtual void SetRegistrationExpirationTime(IN uint32_t uNbSeconds) = 0;

    //==========================================================================
    //==
    //==  GetRegistrationExpirationTime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured registration expiration time.
    //
    //  Returns:
    //      The registration expiration time.
    //
    //  Description:
    //      Gets the configured registration expiration time.
    //
    //  See Also:
    //      SetRegistrationExpirationTime
    //
    //==========================================================================
    virtual uint32_t GetRegistrationExpirationTime() const = 0;

    //==========================================================================
    //==
    //==  SetRegistrationExpiringThreshold
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the maximum delay between when the IUaSspRegistration
    //      component tries to refresh the registration and the expiration in
    //      seconds.
    //
    //  Parameters:
    //    uNbSeconds:
    //      The number of seconds to set as expiring threshold time of
    //      registrations.
    //
    //  Description:
    //      Configures the maximum delay between when the IUaSspRegistration
    //      component tries to refresh the registration and the expiration in
    //      seconds.
    //
    //      Note that the amount of time before the component refreshes the
    //      registration is not a fixed amount of time and is calculated as
    //      follow. Assume that S_Expiration is the expiration time returned by
    //      the server. If S_Expiration / 2 is less than the configured
    //      threshold, the component will refresh the registration
    //      (S_Expiration / 2) seconds before the registration expires. If
    //      S_Expiration / 2 is greater than the configured threshold, then the
    //      component will refresh the registration "threshold" seconds before
    //      the registration expires.
    //
    //      This defaults to 600 seconds.
    //
    //  See Also:
    //      GetRegistrationExpirationTime
    //
    //==========================================================================
    virtual void SetRegistrationExpiringThreshold(IN uint32_t uNbSeconds) = 0;

    //==========================================================================
    //==
    //==  GetRegistrationExpiringThreshold
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured registration expiring threshold time.
    //
    //  Returns:
    //      The registration expiring threshold time.
    //
    //  Description:
    //      Gets the configured registration expiring threshold time.
    //
    //  See Also:
    //      SetRegistrationExpirationTime
    //
    //==========================================================================
    virtual uint32_t GetRegistrationExpiringThreshold() const = 0;
// BRCM_CUSTOM - [begin]
    //==SDOC====================================================================
    //==
    //==  SetGruuEnable
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enable/disable GRUU support on registration
    //
    //  Parameters:
    //    bGruuEnable:
    //      TRUE to enable GRUU support on registration.
    //
    //  Returns:
    //      N/A
    //
    //  Description:
    //      Implemented to resolve inter-op issues. Copy the GRUU provided by 
    //        the network on registration into outgoing packet's Contact headers, 
    //        if registration GRUU support is enabled 
    //
    //  See Also:
    //      N/A
    //
    //==EDOC====================================================================
    virtual void SetGruuEnable(IN bool bGruuEnable) = 0;

    //==SDOC====================================================================
    //==
    //==  GetGruuEnable
    //==
    //==========================================================================
    //
    //  Summary:
    //      Get the GRUU configuration flag
    //
    //  Parameters:
    //      None
    //
    //  Returns:
    //      TRUE if GRUU support has been enabled
    //
    //  Description:
    //      Implemented to resolve inter-op issues. Copy the GRUU provided by 
    //        the network on registration into outgoing packet's Contact headers, 
    //        if registration GRUU support is enabled 
    //
    //  See Also:
    //      N/A
    //
    //==EDOC====================================================================
    virtual bool GetGruuEnable() const = 0;
// BRCM_CUSTOM - [end]
// BRCM_CUSTOM - [begin] Control contact header matching choices
    //==SDOC====================================================================
    //==
    //==  SetStrictContactMatch
    //==
    //==========================================================================
    //
    //  Summary:
    //      Set the strict contact header matching option.
    //
    //  Parameters:
    //    bStrict:
    //      TRUE if strict contact header matching is enabled.
    //
    //  Returns:
    //      N/A
    //
    //  Description:
    //      Implemented to resolve inter-op issues.  Contact header check is 
    //      too strict for some register/proxy servers.
    //
    //  See Also:
    //      N/A
    //
    //==EDOC====================================================================
    virtual void SetStrictContactMatch(IN bool bStrict) = 0;

    //==SDOC====================================================================
    //==
    //==  GetStrictContactMatch
    //==
    //==========================================================================
    //
    //  Summary:
    //      Return the strict contact header matching option.
    //
    //  Parameters:
    //      N/A
    //
    //  Returns:
    //      TRUE if strict contact header matching is enabled.
    //
    //  Description:
    //      Implemented to resolve inter-op issues.  Contact header check is 
    //      too strict for some register/proxy servers.
    //
    //  See Also:
    //      N/A
    //
    //==EDOC====================================================================
    virtual bool GetStrictContactMatch() const = 0;
// BRCM_CUSTOM - [end] Control contact header matching choices


    //==========================================================================
    //<GROUP TID_PACKAGE_UASSP_STRUCTSENUMS>
    //
    // Summary:
    //   The registration monitoring mode.
    //
    // Description:
    //  The registration monitoring mode.
    //
    //==========================================================================
    enum ERegistrationMonitoringMode
    {
        // Description:
        //  No registration state monitoring. The registration component is not
        //  subscribed to its own registration state.
        eMONITORING_NONE,

        // Description:
        //  To perform registration state monitoring, the registration
        //  component subscribes to its own registration state. This
        //  allows the component to be notified of network initiated
        //  de-registrations and changes in registration expiration.
        eMONITORING_STATE
    };

    //==========================================================================
    //==
    //==  SetRegistrationStateMonitoringMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the registration state monitoring mode.
    //
    //  Parameters:
    //    eMonitoringMode: The monitoring mode.
    //
    //  Description:
    //      Sets the registration state monitoring mode.
    //
    //      This defaults to eMONITORING_STATE.
    //
    //  See Also:
    //      GetRegistrationStateMonitoringMode
    //
    //==========================================================================
    virtual void SetRegistrationStateMonitoringMode(IN ERegistrationMonitoringMode eMonitoringMode) = 0;

    //==========================================================================
    //==
    //==  GetRegistrationStateMonitoringMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the registration state monitoring mode.
    //
    //  Returns:
    //      The registration state monitoring mode.
    //
    //  Description:
    //      Gets the registration state monitoring mode.
    //
    //  See Also:
    //      SetRegistrationStateMonitoringMode
    //
    //==========================================================================
    virtual ERegistrationMonitoringMode GetRegistrationStateMonitoringMode() const = 0;

    //==========================================================================
    //==
    //==  SetRegistrationStateSubscriptionDuration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the registration state subscription duration.
    //
    //  Parameters:
    //    uNbSeconds:
    //      The number of seconds to set as duration time of subscription to
    //      the registration state.
    //
    //  Description:
    //      Sets the registration state subscription duration.
    //
    //      This defaults to 3761 seconds.
    //
    //  See Also:
    //      GetRegistrationStateSubscriptionDuration
    //
    //==========================================================================
    virtual void SetRegistrationStateSubscriptionDuration(IN uint32_t uNbSeconds) = 0;

    //==========================================================================
    //==
    //==  GetRegistrationStateSubscriptionDuration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the registration state subscription duration.
    //
    //  Returns:
    //      The duration time of subscription to the registration state.
    //
    //  Description:
    //      Gets the registration state subscription duration.
    //
    //  See Also:
    //      SetRegistrationStateSubscriptionDuration
    //
    //==========================================================================
    virtual uint32_t GetRegistrationStateSubscriptionDuration() const = 0;


    // Description:
    //  This enumeration defines how the registration component must handle its
    //  subscription to its registration state when failing to re-register.
    enum ESubscriptionBehaviorOnError
    {
        // Description:
        //  When the registration component receives any negative failure
        //  response, it will not assume that the subscription is terminated.
        //  The subscription will terminate when the registration component
        //  terminates or when the peer notifier sends a final notify.
        eREGERROR_DEFAULT,


        // Description:
        //  When the registration component receives a 408 (Request Timeout)
        //  response or a 500 (Server Internal Error) response or a 504 (Server
        //  Time-Out) response to a re-registration, it will consider the
        //  subscription as terminated and will silently remove it (it will not
        //  send an unsubscribe request). The next time the component
        //  successfully registers, it will re-subscribe to its registration
        //  state.
        eREGERROR_IMS
    };

    //==========================================================================
    //==
    //==  SetSubscriptionBehaviorOnError
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the subscription behavior on re-registration error.
    //
    //  Description:
    //      Sets the subscription behavior on re-registration error.
    //
    //      The default behavior is eREGERROR_DEFAULT.
    //
    //==========================================================================
    virtual void SetSubscriptionBehaviorOnError(IN ESubscriptionBehaviorOnError eBehavior) = 0;

    //==========================================================================
    //==
    //==  GetSubscriptionBehaviorOnError
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the configured subscription behavior on re-registration
    //      error.
    //
    //  Description:
    //      Returns the configured subscription behavior on re-registration
    //      error.
    //
    //==========================================================================
    virtual ESubscriptionBehaviorOnError GetSubscriptionBehaviorOnError() const = 0;

    //==========================================================================
    //==
    //== SetRegistrationStateSubscriptionRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the registration state subscription retry delay.
    //
    //  Parameters:
    //      uRetryDelayS: The number of seconds to delay.
    //
    //  Description:
    //      Configures the number of seconds to wait after a failed registration
    //      subscription and the next one.
    //
    //      This defaults to 60 seconds.
    //
    //  See Also:
    //      GetRegistrationStateSubscriptionRetryDelay
    //
    //==========================================================================
    virtual void SetRegistrationStateSubscriptionRetryDelay(IN uint32_t uRetryDelayS) = 0;

    //==========================================================================
    //==
    //== GetRegistrationStateSubscriptionRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the registration state subscription retry delay.
    //
    //  Returns:
    //      The registration state retry delay
    //
    //  Description:
    //      Gets the configured registration state subscription retry delay.
    //
    //  See Also:
    //      SetRegistrationStateSubscriptionRetryDelay
    //
    //==========================================================================
    virtual uint32_t GetRegistrationStateSubscriptionRetryDelay() const = 0;

// BRCM_CUSTOM - [begin] Control use of randomized re-registration time
    //==SDOC====================================================================
    //==
    //==  SetRandomReReg
    //==
    //==========================================================================
    //
    //  Summary:
    //      Set the Randomize Re-Registratoin Time option.
    //
    //  Parameters:
    //    bRandom:
    //      TRUE if randomize re-registration time is enabled.
    //
    //  Returns:
    //      N/A
    //
    //  Description:
    //      If enabled, re-registration attempt will use randomly selected 
    //      values between 60% and 90% of the registration expiration time 
    //      as returned in the previous 200 OK
    //
    //  See Also:
    //      GetRandomReReg
    //
    //==EDOC====================================================================
    virtual void SetRandomReReg(IN bool bRandom) = 0;

    //==SDOC====================================================================
    //==
    //==  GetRandomReReg
    //==
    //==========================================================================
    //
    //  Summary:
    //      Return the Randomize Re-Registratoin Time option.
    //
    //  Parameters:
    //      N/A
    //
    //  Returns:
    //      TRUE if randomize re-registration time is enabled.
    //
    //  Description:
    //      Gets the configured randomize re-registration time option
    //
    //  See Also:
    //      SetRandomReReg
    //
    //==EDOC====================================================================
    virtual bool GetRandomReReg() const = 0;
// BRCM_CUSTOM - [end] Control use of randomized re-registration time

// BRCM_CUSTOM - [add] Control configurable retry-after timer on 403 response
    //==========================================================================
    //==
    //==  SetRegistrationRetryTimerOn403
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the registration retry timer to use on 403 response.
    //
    //  Parameters:
    //    uRetryTimer:
    //      The registration retry timer value to use for 403
    //
    //  Description:
    //      Configures the registration retry timer (in seconds) to use on 403
    //      response if the Retry-After header is not included.
    //      When the timer is set, it will override the RegEndOn403 behavior,
    //      so registration will not end on 403.
    //
    //  See Also:
    //      GetRegistrationRetryTimerOn403
    //
    //==========================================================================
    virtual void SetRegistrationRetryTimerOn403(IN uint32_t uRetryTimer) = 0;

    //==========================================================================
    //==
    //==  GetRegistrationRetryTimerOn403
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the registration retry timer to use on 403 response.  
    //      When the timer is set, it will override the RegEndOn403 behavior
    //      which would have prevented registration failover on 403.
    //
    //  Returns:
    //      The registration retry timer value in seconds
    //
    //  Description:
    //      Gets the registration retry timer (in seconds) to use on 403 
    //      response if the Retry-After header is not included.
    //      When the timer is set, it will override the RegEndOn403 behavior,
    //      so registration will not end on 403.
    //
    //  See Also:
    //      SetRegistrationRetryTimerOn403
    //
    //==========================================================================
    virtual uint32_t GetRegistrationRetryTimerOn403() const = 0;
// BRCM_CUSTOM - [end] Control configurable retry-after timer on 403 response

// BRCM_CUSTOM - [add] Silent termination on registration timeout
    //==SDOC====================================================================
    //==
    //==  SetRegTimeoutSilentTerm
    //==
    //==========================================================================
    //
    //  Summary:
    //      Set the registration termination on timeout option.
    //
    //  Parameters:
    //    bRegTerm:
    //      TRUE if silent termination on registration timeout is enabled.
    //
    //  Returns:
    //      N/A
    //
    //  Description:
    //      If enabled, registration timeout will lead to the silent termination
    //      of the user and all of it's child components.
    //
    //  See Also:
    //      GetRegTimeoutSilentTerm
    //
    //==EDOC====================================================================
    virtual void SetRegTimeoutSilentTerm(IN bool bRegTerm) = 0;

    //==SDOC====================================================================
    //==
    //==  GetRegTimeoutSilentTerm
    //==
    //==========================================================================
    //
    //  Summary:
    //      Return the registration termination on timeout option.
    //
    //  Parameters:
    //      N/A
    //
    //  Returns:
    //      TRUE if silent termination on registration timeout is enabled.
    //
    //  Description:
    //      Gets the configured registration termination on timeout option.
    //
    //  See Also:
    //      SetRegTimeoutSilentTerm
    //
    //==EDOC====================================================================
    virtual bool GetRegTimeoutSilentTerm() const = 0;
// BRCM_CUSTOM - [end] Silent termination on registration timeout


//-- Hidden Methods.
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaSspRegistrationConfig() {}

    // Summary:
    //  Destructor.
    virtual ~IUaSspRegistrationConfig() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaSspRegistrationConfig(IN const IUaSspRegistrationConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaSspRegistrationConfig& operator=(IN const IUaSspRegistrationConfig& rSrc);


//-- Hidden Data Members.
protected:
private:


};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPREGISTRATIONCONFIG_H
