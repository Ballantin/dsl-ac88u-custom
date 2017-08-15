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
#ifndef MXG_ISCEBASICEXTENSIONCONTROL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEBASICEXTENSIONCONTROL_H
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

#ifndef MXG_SIPMETHODTYPES_H
#include "SceSipTypes/SipMethodTypes.h"
#endif

#ifndef MXG_SIPOPTIONTAGS_H
#include "SceSipTypes/SipOptionTags.h"
#endif

#ifndef MXG_ISCEUSEREVENTMGR_H
#include "SceCoreComponents/ISceUserEventMgr.h"
#endif

#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CHeaderList;
class CSceCallerPreferences;
class CSceSipCapabilities;
class CSipMessageBody;
class CSipPacket;
class ISceBasicExtensionMgr;
class ISceSipPacketObserver;
class ISceUserConfig;
class ISipContext;
class ISipServerEventControl;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceBasicExtensionControl);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceBasicExtensionControl
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines basic extension mechanisms implemented by all
//   components.
//
// Description:
//  This interface defines basic extension mechanisms implemented by all
//  components.
//
// Location:
//   SceCoreComponents/ISceBasicExtensionControl.h
//
// See Also:
//   ISceSipPacketObserver, ISceBasicExtensionMgr
//
//==============================================================================
class ISceBasicExtensionControl : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceBasicExtensionControl);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetSipPacketObserver
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the SIP packet observer.
    //
    //  Parameters:
    //    pObserver:
    //      Pointer to the observer. The interface supports the configuration of
    //      a non-NULL and a NULL manager. Configuring a NULL manager prevents
    //      further events from being reported.
    //
    //  Description:
    //      Configures the SIP packet observer.
    //
    // See Also:
    //   ISceSipPacketObserver
    //
    //==========================================================================
    virtual void SetSipPacketObserver(IN ISceSipPacketObserver* pObserver) = 0;

    //==========================================================================
    //==
    //==  SetBasicExtensionManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the basic extension manager.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the manager. The interface supports the configuration of
    //      a non-NULL and a NULL manager. Configuring a NULL manager prevents
    //      further events from being reported.
    //
    //  Description:
    //      Configures the basic extension manager.
    //
    //==========================================================================
    virtual void SetBasicExtensionManager(IN ISceBasicExtensionMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  RejectRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Executes the default request rejection for that component.
    //
    //  Parameters:
    //    pUserConfig:
    //      The user configuration that accompanied the EvUnhandledRequest event.
    //      May be NULL.
    //
    //    pServerEventCtrl:
    //      The server event control that accompanied the EvUnhandledRequest
    //      event. Must not be NULL.
    //
    //    rRequest:
    //      The request that accompanied the EvUnhandledRequest event.
    //
    //  Returns:
    //      - resS_OK: the response was properly sent.
    //      - resFE_INVALID_ARGUMENT: pServerEventCtrl is NULL or rRequest is
    //        not a request.
    //      - resFE_FAIL: unable to send the response.
    //
    //  Description:
    //      Falls back to the default behaviour of the component for unhandled
    //      requests. This method must only be called when EvUnhandledRequest is
    //      called for a request that the ISceUnknownRequestMgr does not
    //      want to manage.
    //
    //      This method computes an appropriate response according to the
    //      user configuration, its configured capabilities and the capabilities
    //      requested in the request.
    //
    //      Note that if the configured ISceBasicExtensionMgr of this
    //      component is NULL, the behaviour of the component is to call this
    //      method when an unhandled request is received.
    //
    //  See Also:
    //      ISceBasicExtensionMgr::EvUnhandledRequest
    //
    //==========================================================================
    virtual mxt_result RejectRequest(IN ISceUserConfig* pUserConfig,
                                     IN ISipServerEventControl* pServerEventCtrl,
                                     IN const CSipPacket& rRequest) = 0;

    //==========================================================================
    //==
    //==  SendRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a request with the component.
    //
    //  Parameters:
    //    eMethodType:
    //      The method of the request to send.
    //
    //    opqTransaction:
    //      Opaque identifier that the application can use to match responses to
    //      that request.
    //
    //    pExtraHeaders:
    //      Headers to add to the request. May be NULL.
    //
    //    pMessageBody:
    //      Message body to add to the request. May be NULL.
    //
    //    pCallerPreferences:
    //      The caller preferences to add to the request. Ownership is taken.
    //      May be NULL.
    //
    //  Returns:
    //      - resS_OK: the request is successfully sent.
    //      - resFE_INVALID_ARGUMENT: the request is of an unallowed type.
    //      - resFE_INVALID_STATE: the component is unable to send requests.
    //
    //  Description:
    //      This method uses the principal context of a component to send a
    //      request not managed by the component itself. That request must have
    //      no impact on dialog management. In other words, the method must not
    //      be INVITE, BYE, CANCEL, UPDATE or SUBSCRIBE. The component will only
    //      generate headers that are independent of the method. If there are
    //      headers that are mandatory for the specific method type the
    //      application sends through this method, they must be passed to the
    //      extra headers parameter by the application. For instance, the
    //      'Event' and 'Subscription-State' headers for NOTIFY requests
    //      must be passed through the extra headers parameter.
    //
    //      If the method succeeds, the responses (0 to many provisional and 1
    //      final) for this request will be reported to the
    //      ISceBasicExtensionMgr::EvResponse method. The opaque
    //      transaction passed to this method will be given back to the event.
    //
    //      Note that a ISceBasicExtensionMgr must be configured in
    //      order to receive the responses for the requests sent through this
    //      method.
    //
    //  See Also:
    //      ISceBasicExtensionMgr
    //
    //==========================================================================
    virtual mxt_result SendRequest(IN ESipMethodType eMethodType,
                                   IN mxt_opaque opqTransaction,
                                   IN TO CHeaderList* pExtraHeaders,
                                   IN TO CSipMessageBody* pMessageBody,
                                   IN TO CSceCallerPreferences* pCallerPreferences) = 0;


    //==========================================================================
    //==
    //==  ReIssueRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reissues the request related to the last response received.
    //
    //  Returns:
    //      - resS_OK: The request has been reissued.
    //      - resFE_INVALID_STATE: There is currently no request that can be
    //        reissued.
    //
    //  Description:
    //      This method reissues the request related to the last response
    //      received.  It can be used only in the ISceSipPacketObserver::EvReceivedResponse
    //      and the ISceBasicExtensionMgr::EvResponse methods.
    //
    //  See Also:
    //      ISceSipPacketObserver::EvReceivedResponse,
    //      ISceBasicExtensionMgr::EvResponse
    //
    //==========================================================================
    virtual mxt_result ReIssueRequest() = 0;

    //==========================================================================
    //==
    //==  GetLastReceivedPacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the last packet received by the component.
    //
    //  Parameters:
    //    rpPacket:
    //      Reference to a CSipPacket pointer. The pointer will be set to NULL
    //      if no SIP packet was received by the component yet. When returning a
    //      non-NULL pointer, a reference to the CSipPacket will have been
    //      counted. The caller must thus call Release() on the packet once it
    //      is done with it.
    //
    //  Description:
    //      Gets the last SIP packet received by the component.
    //
    //==========================================================================
    virtual void GetLastReceivedPacket(OUT const CSipPacket*& rpPacket) = 0;

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   Authentication mechanisms supported in the SIP Client Engine.
    //
    // Description:
    //  This enumeration defines the authentication mechanisms supported in the
    //  SIP Client Engine.
    //
    //==========================================================================
    enum ESceAuthenticationServices
    {
        // Description:
        // This value defines the MD5 digest authentication mechanism.
        eAUTHSVC_DIGEST_MD5 = 0x00000001,

        // Description:
        // This value is a bitmask of all supported digest authentication
        // mechanisms. It can be used to disable ALL mechanisms at once.
        eAUTHSVC_DIGEST_ALL = eAUTHSVC_DIGEST_MD5
    };

    //==========================================================================
    //==
    //==  EnableAuthenticationCallback
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables or disables the authentication callback.
    //
    //  Parameters:
    //    bEnable:
    //      true to enable the callback, false otherwise.
    //
    //    uBitSetDisabledAuth:
    //      A bitset created from values of ESceAuthenticationServices, which
    //      identifies the authentication mechanisms that should no longer be
    //      handled by the M5T SIP Client Engine. Note that this parameter is
    //      ignored when bEnable is false.
    //
    //  Description:
    //      Configures whether or not the authentication callback should be
    //      called when the component is not able to handle the requested
    //      authentication mechanism. The authentication callback is
    //      ISceBasicExtensionMgr::EvChallenged.
    //
    //      Furthermore, uBitSetDisabledAuth allows the caller to disable some
    //      or all of the supported authentication mechanisms, allowing the user
    //      to take over the authentication management for these specific
    //      mechanisms.
    //
    //      It is important to call EnableAuthenticationCallback BEFORE a user
    //      configuration object is set on the component, otherwise the
    //      configuration may not take effect.
    //
    //  See Also:
    //      ISceBasicExtensionMgr::EvChallenged, AcceptChallenge,
    //      RejectChallenge
    //
    //==========================================================================
    virtual void EnableAuthenticationCallback(IN bool bEnable,
                                              IN uint32_t uBitSetDisabledAuth) = 0;

    //==========================================================================
    //==
    //==  AcceptChallenge
    //==
    //==========================================================================
    //
    //  Summary:
    //      Specifies that the application accepts the challenge reported
    //      through the ISceBasicExtensionMgr::EvChallenged callback.
    //
    //  Parameters:
    //    opqChallenge:
    //      The opaque value provided in ISceBasicExtensionMgr::EvChallenged.
    //
    //  Description:
    //      Specifies that the application accepts the challenge reported
    //      through the ISceBasicExtensionMgr::EvChallenged callback.
    //
    //      Once the application called this method, it must make sure to monitor
    //      the ISceSipPacketObserver::EvSendingRequest event for the re-issued
    //      request. The application will need to add its credentials for the
    //      challenge in this event.
    //
    //  Warning:
    //      After receiving the ISceBasicExtensionMgr::EvChallenged event, the
    //      application MUST call AcceptChallenge OR RejectChallenge to ensure
    //      proper operation of the component. Failure to do so will prevent
    //      the component from being released.
    //
    //  See Also:
    //      ISceBasicExtensionMgr::EvChallenged,
    //      ISceSipPacketObserver::EvSendingRequest,
    //      RejectChallenge
    //
    //==========================================================================
    virtual void AcceptChallenge(IN mxt_opaque opqChallenge) = 0;

    //==========================================================================
    //==
    //==  RejectChallenge
    //==
    //==========================================================================
    //
    //  Summary:
    //      Specifies that the application rejects the challenge reported
    //      through the ISceBasicExtensionMgr::EvChallenged callback.
    //
    //  Parameters:
    //    opqChallenge:
    //      The opaque value provided in ISceBasicExtensionMgr::EvChallenged.
    //
    //  Description:
    //      Specifies that the application rejects the challenge reported
    //      through the ISceBasicExtensionMgr::EvChallenged callback.
    //
    //      Once the application called this method, it can forget about the
    //      challenge.
    //
    //  Warning:
    //      After receiving the ISceBasicExtensionMgr::EvChallenged event, the
    //      application MUST call AcceptChallenge OR RejectChallenge to ensure
    //      proper operation of the component. Failure to do so will prevent
    //      the component from being released.
    //
    //  See Also:
    //      ISceBasicExtensionMgr::EvChallenged,
    //      ISceSipPacketObserver::EvSendingRequest,
    //      AcceptChallenge
    //
    //==========================================================================
    virtual void RejectChallenge(IN mxt_opaque opqChallenge) = 0;

    //==========================================================================
    //==
    //==  GetSipContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gives access to the ISipContext of a component.
    //
    //  Parameters:
    //    ppContext:
    //      The ISipContext used by the component. Can be NULL if none is used.
    //      When non-NULL, a reference will be given for the ISipContext so it
    //      has to be released when its use is over.
    //
    //  Description:
    //      Gives access to the ISipContext used by the component.
    //
    //  Warning:
    //      Accessing and using the ISipContext of a component is very sensitive
    //      and care must be taken in order to avoid creating synchronization
    //      problems between the application and the component. Please contact
    //      M5T and explain what you want to achieve with the ISipContext before
    //      using this method.
    //
    //==========================================================================
    virtual void GetSipContext(OUT ISipContext** ppContext) = 0;

// BRCM_CUSTOM - [add] Add authentication info sharing support
    //==========================================================================
    //==
    //==  AddChallengeInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Update the component's internal challenge info by adding the
    //      provided challenge. If the component already contains a challenge
    //      with the same realm, the challenge will be updated with the new
    //      info from the provided challenge.
    //
    //  Parameters:
    //      opqChallengeId:
    //       The identifier representing the last updated challenge information.
    //
    //  Description:
    //      Gets the SIP capabilities of this component.
    //
    //      The object contains information on SIP extensions and method support
    //      on the local endpoint as well as on the remote endpoint.
    //
    //==========================================================================
    virtual void AddChallengeInfo(IN mxt_opaque opqChallengeId) = 0;

    //==SDOC====================================================================
    //==
    //==  SetChallengeCount
    //==
    //==========================================================================
    //
    //  Summary:
    //      Set the nonce-count of the component's internal challenge only if it
    //      contains a challenge with the same nonce and realm of the ones in
    //      the provided challenge.
    //
    //  Parameters:
    //    rChallenge:
    //      Challenge which contains the nonce and realm to perform the
    //      matching.
    //
    //    uNonceCount:
    //      The nonce-count to be updated in the component's internal challenge.
    //
    //  Returns:
    //      False if the component does not contain any internal challenge that
    //      matches with the provided challenge.
    //      True if the nonce-count is updated successfully.
    //
    //==EDOC====================================================================
    virtual bool SetChallengeCount(IN mxt_opaque opqChallengeId,
                                   IN unsigned int uNonceCount) = 0;

    //==SDOC====================================================================
    //==
    //==  SetChallengeType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Set the challenge type of the component's internal challenge only
    //      if it contains a challenge with the same realm and nonce of the
    //      ones in the provided challenge.
    //
    //  Parameters:
    //    rChallenge:
    //      Challenge which contains the realm to perform the matching.
    //
    //    uChallengeType:
    //      The challenge tyep to be updated in the component's internal
    //      challenge.
    //
    //  Returns:
    //      False if the component does not contain any internal challenge that
    //      matches with the provided challenge.
    //      True if the challenge type is updated successfully.
    //
    //==EDOC====================================================================
    virtual bool SetChallengeType(IN mxt_opaque opqChallengeId,
                                  IN unsigned int uChallengeType) = 0;

    //==SDOC====================================================================
    //==
    //==  ResetChallengeInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Resets the component's challenge information.
    //
    //  Parameters:
    //
    //  Description:
    //      Resets the component's authentication state by clearing out all
    //      of its challenge information.
    //
    //==EDOC====================================================================                                  
    virtual void ResetChallengeInfo() = 0;
// BRCM_CUSTOM - [end] Add authentication info sharing support

    //==========================================================================
    //==
    //==  SetTransactionStats
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the transaction statistics for this component.
    //
    //  Parameters:
    //      pTransactionStats:
    //       The transaction statistics.
    //
    //       MUST NOT be NULL.
    //
    //       ISipTransactionStatistics MUST be queriable on this pointer.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT:
    //       pTransactionStats is NULL or ISipTransactionStatistics could not be
    //       queried on it.
    //
    //      resFE_INVALID_STATE:
    //       The ISipContext is already created or a transaction statistics has
    //       already been set.
    //
    //      resS_OK:
    //       The pointer was kept and the ISipStatisticsSvc will be attached to
    //       the context when created.
    //
    //  Description:
    //      This allows the statistics service to be attached to the ISipContext
    //      that will be created by this component.
    //
    //      This method should be called by the user of a component only if it
    //      wants to have the statistics reported through the interface
    //      ISipTransactionStatistics. Not calling this methods will have the
    //      effect of not attaching the ISipStatisticsSvc to the ISipContext
    //      created by the component.
    //
    //      This method MUST be called before a context is created by a
    //      component.
    //
    //==========================================================================
    virtual mxt_result SetTransactionStats(IN IEComUnknown* pTransactionStats) = 0;

    //==========================================================================
    //==
    //==  GetTransactionStats
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides access to the transaction statistics of the component.
    //
    //  Parameters:
    //      ppTransactionStats:
    //       Pointer to a pointer to a transaction statistics. This pointer is
    //       set to the value of the set transaction statistics. Can return NULL
    //       if no transaction statistics was set. If it returns non-NULL, a
    //       reference will be counted, as per the ECOM rules.
    //
    //       The ISipTransactionStatistics interface can be queried on
    //       ppTransactionStats.
    //
    //  Description:
    //      Provides access to the transaction statistics of the component.
    //
    //  See Also:
    //      SetTransactionStats
    //
    //==========================================================================
    virtual void GetTransactionStats(OUT IEComUnknown** ppTransactionStats) const = 0;

    //==========================================================================
    //==
    //==  GetSceSipCapabilities
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the SIP capabilities of this component.
    //
    //  Description:
    //      Gets the SIP capabilities of this component.
    //
    //      The object contains information on SIP extensions and method support
    //      on the local endpoint as well as on the remote endpoint.
    //
    //==========================================================================
    virtual const CSceSipCapabilities& GetSceSipCapabilities() const = 0;

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  IsOptionSupported
    //==
    //==========================================================================
    // <FLAG Deprecated in 2.1.2>
    //
    //  Summary:
    //      Returns whether the given option is supported in the current dialog.
    //
    //  Parameters:
    //      eTag:
    //       The option to check the support of.
    //
    //  Returns:
    //      True if the option is supported in the current dialog, false
    //      otherwise.
    //
    //  Description:
    //      Returns whether the option is supported by both sides of the dialog.
    //
    //      This method should not be used any longer.
    //      GetSceSipCapabilities().IsSupported(eTag) should be used instead to
    //      achieve the same result.
    //
    //  See Also:
    //      GetSceSipCapabilities, CSceSipCapabilities
    //
    //==========================================================================
    virtual bool IsOptionSupported(IN ESipOptionTag eTag) = 0;


    //==========================================================================
    //==
    //==  IgnoreUserEvent
    //==
    //==========================================================================
    //  Summary:
    //      Allows to define specific user events that will be ignored by the
    //      component.
    //
    //  Parameters:
    //      eUserEvent:
    //       The user event to ignore. 
    //
    //  Description:
    //      This method allows to specify specific user events that will be
    //      ignored by the component.
    //
    //  See Also:
    //      IsUserEventEnabled
    //
    //==========================================================================
    virtual void IgnoreUserEvent(IN ISceUserEventMgr::EUserEvent eUserEvent) = 0;

    //==========================================================================
    //==
    //==  IsUserEventEnabled
    //==
    //==========================================================================
    //  Summary:
    //      Checks if the given user even is enabled or if it must be ignored.
    //
    //  Parameters:
    //      eUserEvent:
    //       The user event to check.
    //
    //  Returns:
    //      True if the user event is allowed on the component.
    //      False otherwise.
    //
    //  Description:
    //      This method check if the given user event is allowed on the
    //      component or if it must be ignored. Specific user events can be
    //      disabled using the IgnoreUserEvent method.
    //
    //  See Also:
    //      IgnoreUserEvent
    //
    //==========================================================================
    virtual bool IsUserEventEnabled(IN ISceUserEventMgr::EUserEvent eUserEvent) = 0;

//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  GetComponentOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the application set component opaque.
    //
    //  Returns:
    //      The application set component opaque or NULL if none is set.
    //
    //  Description:
    //      Returns the application set component opaque.
    //
    //      The application opaque is set through each component-specific
    //      interface (e.g. IUaSspCall, IUaSspRegistration, ISceSubscriber,
    //      etc.).
    //
    //==========================================================================
    virtual mxt_opaque GetComponentOpaque() = 0;

    //==========================================================================
    //==
    //==  ForceSipTransport
    //==
    //==========================================================================
    //
    //  Summary:
    //      Forces a component-specific SIP transport.
    //
    //  Parameters:
    //    eTransport:
    //      The forced SIP transport to use to send packets to the next-hop URI.
    //      Set to any valid ESipTransport value to activate.
    //
    //  Returns:
    //      resS_OK if the transport is set, failure if the component internal
    //      context is active.
    //
    //  Description:
    //      Forces a component-specific SIP transport. Default value if
    //      eINVALID, meaning the SIP transport will not be forced.
    //
    //      For server components (like CSceNotifier and CUaSspCall), it is the
    //      responsibility of the handler manager (IUaSspNewCallMgr and
    //      ISceNewNotifierMgr) to verify the transport the request is received
    //      on. If the transport matches what the application wants to support,
    //      transport will need to be overridden using this method. If the
    //      request is not received on a transport that the application
    //      supports, it should simply not set the new component's manager, so
    //      that the request can be handled by another handler if available or
    //      will be automatically replied with a failure response by the default
    //      handler.
    //
    //      Can ONLY be set before the component internal context is created.
    //
    //  #WARNING#
    //      The component linked to this extension interface will force outgoing
    //      requests to be sent on the specified transport, overriding any and
    //      all correct processing done to choose a transport, including RFC3263
    //      processing.
    //
    //      As such, this method MUST only be used in very specific cases. The
    //      normal way to force a transport is to specify a transport parameter
    //      in the request or route URI instead.
    //
    //  See Also:
    //      ESipTransport, GetSupportedTransport
    //
    //==========================================================================
    virtual mxt_result ForceSipTransport(IN ESipTransport eTransport) = 0;

    //==========================================================================
    //==
    //==  GetForcedTransport
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the component-specific forced transport.
    //
    //  Returns:
    //      The forced transport for this component or eINVALID if not forced.
    //
    //  Description:
    //      Gets the forced transport for this SIP Client Engine component.
    //
    //  See Also:
    //      ESipTransport, SetSupportedTransport
    //
    //==========================================================================
    virtual ESipTransport GetForcedTransport() = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceBasicExtensionControl() {}

    // Summary:
    //  Destructor.
    virtual ~ISceBasicExtensionControl() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceBasicExtensionControl(IN const ISceBasicExtensionControl& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceBasicExtensionControl& operator=(IN const ISceBasicExtensionControl& rSrc);

//-- Hidden Data Members.
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEBASICEXTENSIONCONTROL_H

