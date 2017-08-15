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
#ifndef MXG_CSCEBASECOMPONENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEBASECOMPONENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#ifndef MXG_IPRIVATESCECHALLENGESOURCE_H
#include "SceCoreComponents/IPrivateSceChallengeSource.h"
#endif

#ifndef MXG_ISCEUSERCONFIG_H
#include "SceCoreComponents/ISceUserConfig.h" // inline usage
#endif

#ifndef MXG_ISCEBASICEXTENSIONCONTROL_H
#include "SceCoreComponents/ISceBasicExtensionControl.h" // implemented interface
#endif

#ifndef MXG_IPRIVATESCEBASECOMPONENT_H
#include "SceCoreComponents/IPrivateSceBaseComponent.h" // implemented interface
#endif

#ifndef MXG_ISCEPERSISTENTCONNECTIONOBSERVER_H
#include "SceCoreComponents/IScePersistentConnectionObserver.h"
#endif

#ifndef MXG_ISCEUACAPABILITIESCONFIG_H
#include "SceCoreComponents/ISceUaCapabilitiesConfig.h"
#endif

#ifndef MXG_ISCEUSERIDENTITYCONFIG_H
#include "SceCoreComponents/ISceUserIdentityConfig.h" // implemented interface
#endif

#ifndef MXG_CSCESIPCAPABILITIES_H
#include "SceSipTypes/CSceSipCapabilities.h"
#endif

// SipCore.
#ifndef MXG_ISIPCONTEXT_H
#include "SipCore/ISipContext.h"
#endif

// SipCoreSvc Includes.
#ifndef MXG_ISIPSERVERLOCATIONSVC_H
#include "SipCoreSvc/CSipServerLocationSvc.h"
#endif

#ifndef MXG_ISIPCOREOUTPUTCONTROLLINGMGR_H
#include "SipCoreSvc/ISipCoreOutputControllingMgr.h"
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
    #ifndef MXG_ISIPENUMREQUESTHANDLERMGR_H
    #include "SipCoreSvc/ISipEnumRequestHandlerMgr.h"
    #endif
#endif

// SipParser Includes.
#ifndef MXG_CSIPURI_H
#include "SipParser/CSipUri.h"
#endif

#ifndef MXG_HEADERDEFINITION_H
#include "SipParser/HeaderDefinition.h"
#endif

#ifndef MXG_IURI_H
#include "SipParser/IUri.h"
#endif

// SipUserAgent Includes.
#ifndef MXG_ISIPGENERICMGR_H
#include "SipUserAgent/ISipGenericMgr.h"
#endif

#ifndef MXG_ISIPDIGESTCLIENTAUTHMGR_H
#include "SipUserAgent/ISipDigestClientAuthMgr.h"
#endif

#ifndef MXG_ISIPUAASSERTEDIDENTITYMGR_H
#include "SipUserAgent/ISipUaAssertedIdentityMgr.h"
#endif

#ifndef MXG_ISIPPRIVACYMGR_H
#include "SipUserAgent/ISipPrivacyMgr.h"
#endif

#ifndef MXG_ISIPREDIRECTIONMGR_H
#include "SipUserAgent/ISipRedirectionMgr.h"
#endif

#ifndef MXG_ISIPUSERAGENTSVC_H
#include "SipUserAgent/ISipUserAgentSvc.h"
#endif

// Sip Stack services
#ifndef MXG_ISIPPACKETOBSERVERSVCMGR_H
#include "SceStackServices/ISipPacketObserverSvcMgr.h"
#endif

#ifndef MXG_ISIPGENERICCLIENTAUTHENTICATIONMGR_H
#include "SceStackServices/ISipGenericClientAuthenticationMgr.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h" // base class
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISCEUSEREVENTMGR_H
#include "SceCoreComponents/ISceUserEventMgr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.

// SceCore.
class ISceUserConfig;

// SceCoreComponents.
class CSceCallerPreferences;
// BRCM_CUSTOM - [add] Add TIP support per 3GPP TS 22.173
class CSceClientEventData;
// BRCM_CUSTOM - [end] Add TIP support per 3GPP TS 22.173
class CSceServerEventData;
class CSceSipFeatureSet;
class IPrivateSceAuthentication;
class ISceGenericEventInfo;

// SipCore.
class ISipClientEventControl;
class ISipClientTransaction;
class ISipServerEventControl;

// SipParser.
class CHeaderList;
class CTelUri;

// SipTransaction
class ISipTransactionStatistics;

// SipTransport.
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceBaseComponent
//========================================
//
// Description:
//  This is the base component from which all other M5T components should
//  inherit. It defines basic operations and offers common support for a limited
//  set of features.
//
//  This service can attach and manage the following stack services:
//
//      - ISipUserAgentSvc
//      - ISipGenericSvc (rejecting requests only)
//      - ISipRedirectionSvc
//      - ISipDigestClientAuthSvc
//      - ISipPrivacySvc
//      - ISipUaAssertedIdentitySvc
//      - ISipServerLocationSvc
//      - ISipConnectionBlacklistSvc
//      - ISipPersistentConnectionSvc
//      - ISipSummetricUdpSvc
//      - ISipOutputControllingSvc
//
//  The service will only allow to attach the user agent service if a user
//  configuration object has been configured, in which case the service is
//  configured with the information from the configuration object.
//
//  The default implementation of ISipGenericMgr::EvRequest is the following:
//      - If no user configuration object was associated with the request, it is
//        rejected with a 404
//      - Subscribe to unregistered events are answered with a 489
//      - Notify requests are answered with a 481
//      - Cancel requests that do not match an existing transaction are answered
//        with a 481
//      - All other requests are answered with a 501
//
//  This default behavior can be overriden if a component wants to perform
//  specific treatment for a generic request.
//
//  This class is a non instantiable ECOM object. It implements the
//  ISceBasicExtensionControl interface. Although it has no pure virtual
//  methods, it has no CreateInstance method and no associated class ID. Hence
//  it cannot be created directly by the ECOM mechanism.
//
// Location:
//   SceCoreComponents/CSceBaseComponent.h
//
//==============================================================================
class CSceBaseComponent : protected CEComDelegatingUnknown,
                          protected CEventDriven,
                          protected ISceBasicExtensionControl,
                          protected ISceUaCapabilitiesConfig,
                          protected IPrivateSceBaseComponent,
                          protected IPrivateSceChallengeSource,
                          protected ISipGenericMgr,
                          protected ISipRedirectionMgr,
                          protected ISipDigestClientAuthMgr,
                          protected ISipPrivacyMgr,
                          protected ISipUaAssertedIdentityMgr,
                          protected ISipCoreOutputControllingMgr,
#if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
                          protected ISipEnumRequestHandlerMgr,
#endif
                          // This cannot be private as it should, because
                          // child component won't be able to register
                          // as observer.
                          protected IScePersistentConnectionObserver,
                          protected ISipPacketObserverSvcMgr,
                          protected ISipGenericClientAuthenticationMgr,
                          protected ISceUserEventMgr,
                          protected ISceUserIdentityConfig
{
//-- Friend Declarations.

//-- Public Custom Types
public:
    // Summary:
    //  This enumeration defines the message ids that are used with the event
    //  driven's message service.
    //
    // Description:
    //  These message IDs are reserved and used by the base components. For
    //  proper behaviour, child components must ensure that the base component
    //  will be notified for these messages.
    //
    //  A child component can start its own custom message IDs from the
    //  eCHILD_COMPONENT_FIRST_MESSAGE_ID value. This value is not used by the
    //  base component and will always be out of the base component's reserved
    // IDs range.
    enum EMessageIds
    {
#if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
        // Description:
        //  Message ID for asynchronous processing of ENUM resolution results.
        eENUM_URI_RESOLVED,
#endif

        // Description:
        //  This is the first message ID that child components can use for their
        //  custom message IDs.
        //
        //  It MUST always be the last value of the enumeration.
        eCHILD_COMPONENT_FIRST_MESSAGE_ID
    };

    // Summary:
    //  This enumeration defines the timer IDs that are used with the event
    //  driven's timer service.
    //
    // Description:
    //  These timer IDs are reserved and used by the base components. For
    //  proper behaviour, child components must ensure that the base component
    //  will be notified for these timers.
    //
    //  A child component can start its own custom timer IDs from the
    //  eCHILD_COMPONENT_FIRST_TIMER_ID value. This value is not used by the
    //  base component and will always be out of the base component's reserved
    //  IDs.
    enum ETimerIds
    {
        //-- The base component has no reserved timer IDs at the moment.

        // Description:
        //  This is the first timer ID that child components can use for their
        //  custom timers.
        //
        //  It MUST always be the last value of the enumeration.
        eCHILD_COMPONENT_FIRST_TIMER_ID
    };

    // Used to search bodies with GetBodies. See GetBodies for a detailed
    // description of the member use.
    struct SSearchedBody
    {
        // The Content-Type m-type of the body to search.
        const char* m_pszContentTypeMType;

        // The Content-Type m-subtype of the body to search.
        const char* m_pszContentTypeMSubType;

        // The Content-Disposition of the body to search. NULL to ignore.
        const char* m_pszContentDisposition;

        // A pointer to the pointer where to set the found body, if any.
        // When non-NULL, this first found body corresponding to the other
        // members will be set.
        // When NULL, the other members will be used to tell that the body is
        // supported.
        const CBlob** m_ppFoundBody;

        SSearchedBody();
    };
private:
    // This is an internal struct used for the authentication mechanism.
    struct SSourceData
    {
        // The service that has detected the challenge and will be handling it.
        ISipDigestClientAuthSvc* m_pSvc;

        // The interface to access client event related functionality.
        ISipClientEventControl* m_pClientEventCtrl;
    };

public:

    // Summary:
    //  Searches for the described bodies.
    static mxt_result GetBodies(IN const CSipMessageBody& rMessageBody,
                                IN STraceNode& rTraceNode,
                                INOUT CVector<SSearchedBody>& rvecSearchedBody);

//-- Hidden methods.
protected:
    // Summary:
    //  Callback to modify the location list returned by the ISipLocationSvc.
    static void ServerLocationListModifierCallback(IN const CHostPort& rFqdn,
                                                   INOUT CList<SNaptrRecord>& rlstNaptrRecord,
                                                   IN mxt_opaque opq);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceBaseComponent(IN IEComUnknown* pOuterIEComUnknown,
                      IN STraceNode& rChildNode,
                      IN STraceNode& rChildEComNode);

    // Summary:
    //  Destructor.
    virtual ~CSceBaseComponent();


    //-- << Configuration >>

    // Summary:
    //  Sets user configuration.
    mxt_result SetUserConfig(IN ISceUserConfig* pConfig);

    // Summary:
    //  Gets user configuration.
    void GetUserConfig(OUT ISceUserConfig*& rpConfig) const;

// BRCM_CUSTOM - [add] Add connected ID support per RFC 4916
    //  Gets user configuration
    void GetUserIdentityConfigUnknown(OUT IEComUnknown*& rpIdentityConfigUnknown) const;
// BRCM_CUSTOM - [end] Add connected ID support per RFC 4916

    // Summary:
    //  Gets the SIP packet observer.
    void GetSipPacketObserver(OUT ISceSipPacketObserver*& rpObserver) const;

    // Summary:
    //  Gets the basic extension manager.
    void GetBasicExtensionManager(OUT ISceBasicExtensionMgr*& rpMgr) const;

    // Summary:
    //  Queries the user configuration directly.
    template<class _Type> mxt_result QueryUserConfig(OUT _Type*& rpInterface)
    {
        rpInterface = NULL;
        if (m_pUserConfig != NULL)
        {
            return m_pUserConfig->QueryIf(rpInterface->GetIID(),
                                          reinterpret_cast<void**>(&rpInterface));
        }

        return resFE_FAIL;
    }

    //-- << Capabilities Management >>

    // Summary:
    //  Gets the capabilities for inspection and modification.
    CSceSipCapabilities& GetSceSipCapabilities();

    // Summary:
    //  Updates the capabilities supported by the peer with the ones found in
    //  the SIP packet.
    virtual mxt_result UpdatePeerCapabilities(IN const CSipPacket& rPacket,
                                              OUT GO CSipHeader*& rpHdrUnsupported);

    // Summary:
    //  Verifies that the required peer capabilities are supported locally.
    virtual mxt_result VerifyRequiredPeerCapabilities(IN const CSipPacket& rPacket,
                                                      OUT GO CSipHeader*& rpHdrUnsupported) const;

    // Summary:
    //  Verifies that the required peer capabilities are supported locally.
    virtual mxt_result VerifyServerRequest(IN const CSipPacket& rPacket,
                                           IN ISipServerEventControl* pServerEventControl);


    enum EStackServices
    {
        // This maps to no service. Useful when conditional expressions are used
        // within OR '|' bitset logical operations.
        eSVC_NONE                               = 0x00000000,

        // This service is always required for digest authentication.
        eSVC_DIGESTCLIENTAUTH                   = 0x00000001,

        // Always required for supporting extension mechanisms, sending any
        // type of request from a component.
        eSVC_GENERIC                            = 0x00000002,

        // Call component only.
        eSVC_GLARE                              = 0x00000004,

        // Notifier component only.
        eSVC_NOTIFIER                           = 0x00000010,

        // Required for all components, except registration.
        eSVC_PRIVACY                            = 0x00000020,

        // Publisher components only.
        eSVC_PUBLISH                            = 0x00000040,

        // Required for all components.
        eSVC_REDIRECTION                        = 0x00000080,

        // CSceReferrer
        eSVC_REFEREE                            = 0x00000100,

        // CSceReferrer
        eSVC_REFERRER                           = 0x00000200,

        // Registration component only.
        eSVC_REGISTRATION                       = 0x00000400,

        // Call component only.
        eSVC_RELIABLEPROVISIONALRESPONSE        = 0x00000800,

        // Call component only.
        eSVC_REPLACES                           = 0x00001000,

        // Call component only.
        eSVC_SESSION                            = 0x00002000,

        // Call component only.
        eSVC_SESSIONTIMER                       = 0x00004000,

        // Subscriber component.
        eSVC_SUBSCRIBER                         = 0x00008000,

        // Call component.
        eSVC_TRANSFER07                         = 0x00010000,

        // All components, except registration.
        eSVC_UAASSERTEDIDENTITY                 = 0x00020000,

        // Call component.
        eSVC_UPDATE                             = 0x00040000,

        // Required for all components.
        eSVC_USERAGENT                          = 0x00080000,

        // Required for all components.
        eSVC_CONNECTIONBLACKLIST                = 0x00100000,

        // Required for all components.
        eSVC_COREOUTPUTCONTROLLING              = 0x00200000,

        // Required for all components.
        eSVC_ENUM                               = 0x00400000,

        // Required for all components.
        eSVC_PERSISTENTCONNECTION               = 0x00800000,

        // Required for all components.
        eSVC_SERVERLOCATION                     = 0x01000000,

        // Required for all components.
        eSVC_SYMMETRICUDP                       = 0x02000000,

        // Required for all components.
        eSVC_DESTINATIONSELECTION               = 0x04000000,

        // Required for all components.
        eSVC_VIAMANAGEMENT                      = 0x08000000,

        // Required in some components when configured.
        eSVC_STATISTICS                         = 0x10000000,

        // Required for all components.
        eSVC_TRANSPORTCONFIGURATION             = 0x20000000,

        // All components.
        eSVC_ALL                                = 0xFFFFFFFF
    };


    //-- << Services Management >>

    // Summary:
    //  Gets a bitset of the services actively supported by that component.
    virtual uint32_t GetSupportedServicesBitset() const;

    // Summary:
    //  Attaches all services managed by this base component.
    virtual mxt_result AttachServices(IN uint32_t uServices);


    //-- << Helper methods >>

    // Summary:
    //  Configures the User-Agent service for a component that will be
    //  initiating a request.
    virtual mxt_result ConfigureUserAgentSvc(IN const CNameAddr& rRemoteAddr,
                                             IN const IUri* pCurrentTarget);

    // Summary:
    //  Configures the User Agent local address according to the Identity
    //  configuration.
    virtual mxt_result ConfigureUserAgentLocalAddress(IN ISipUserAgentSvc* pUaSvc,
                                                      IN ISceUserIdentityConfig::EPrivacyMode eComponentPrivMode);

    // Summary:
    //  Configures the User-Agent service for a component that is receiving a
    //  request.
    virtual mxt_result ConfigureUserAgentSvc(IN const CSipPacket& rIncomingRequest);


    // Summary:
    //  Configures the user agent service after enum resolution was performed.
    virtual mxt_result ClientUpdateUserAgentSvcHelper(IN const IUri* pCurrentTarget);

    // Summary:
    //  Configures the request target for client type of components.
    virtual mxt_result ClientUpdateRequestTargetHelper(IN const IUri* pCurrentTarget);

    // Summary:
    //  Configures the pre-loaded route for client type of components.
    virtual mxt_result ClientUpdatePreLoadedRouteHelper();

    // Summary:
    //  Configures the contact for client type of components.
    virtual mxt_result ClientUpdateContactHelper();

    // Summary:
    //  Updates the contact header after a privacy configuration change.
    mxt_result UpdateUaContact(IN ISipUserAgentSvc* pUaSvc);

    // Description:
    //     Contact types that can be built by BuildContactHelper.
    enum EContactType
    {
        eDEVICE_CONTACT,
        eUSER_CONTACT
    };

    // Summary:
    //  Builds a contact list according to parameters.
    virtual GO CSipHeader* BuildContactHelper(IN EContactType eContactType,
                                              IN IUri::EUriType eUriType) const;

    // Summary:
    //  Builds a pre-loaded route list according to parameters.
    virtual GO CSipHeader* BuildPreLoadedRouteHelper(IN IUri::EUriType eUriType) const;

    // Summary:
    //  Validates the URI scheme used in the provided CNameAddr URI and adjusts it
    //  if needed.
    virtual mxt_result ValidateUriTypeHelper(IN IUri::EUriType eUriType,
                                             INOUT CNameAddr& rNameAddr) const;

    // Summary:
    //  Method called when an unhandled request is received by the base
    //  component.
    virtual void UnhandledRequestHelper(IN ISipServerEventControl* pServerEventCtrl,
                                        IN const CSipPacket& rRequest);

    // Summary:
    //  Method called when a response is received by the base component.
    virtual void ReceiveResponseHelper(IN ISipClientEventControl* pClientEventCtrl,
                                       IN const CSipPacket& rResponse);

    // Summary:
    //  Method called when a transaction is replaced by another.
    virtual void TransactionChanged(IN ISipClientTransaction* pOldTransaction,
                                    IN ISipClientTransaction* pNewTransaction);

    // Summary:
    //  Obtain the private authentication interface or call the next client
    //  event if none is available.
    virtual void GetPrivateAuthHelper(IN ISipClientEventControl* pClientEventCtrl,
                                      OUT IPrivateSceAuthentication*& rpPrivateAuth);

    // Summary:
    //  Sends a generic request.
    virtual mxt_result SendRequestHelper(IN ESipMethodType eMethodType,
                                         IN mxt_opaque opqTransaction,
                                         IN TO CHeaderList* pExtraHeaders,
                                         IN TO CSipMessageBody* pMessageBody,
                                         IN TO CSceCallerPreferences* pCallerPreferences,
                                         OUT ISipClientTransaction*& rpTransactions);

    // Summary:
    //  Recursively scans all the message bodies to verify if all mandatory
    //  bodies are supported.
    virtual bool AreSupportedMediaTypes(IN const CSipMessageBody* pMessageBody);

    // Summary:
    //  Updates the current preloaded route and sets it on the User-Agent.
    virtual void UpdatePreLoadedRoute();

    // Summary:
    //  Makes sure to handle the initial server request correctly.
    mxt_result HandleInitialServerRequest(IN const CSipPacket& rPacket);

    // Summary:
    //  This method must be implemented by child components to be notified of
    //  an initial Server transaction error.
    virtual void OnInitialServerTransactionError();

// BRCM_CUSTOM - [add] Add TIP support per 3GPP TS 22.173
    // Summary:
    //  The contents of the client event data are placed into generic event info.
    void HandleClientEventData(IN const CSceClientEventData& rClientEventData,
                               GO ISceGenericEventInfo*& rpGenericEventInfo);
// BRCM_CUSTOM - [end] Add TIP support per 3GPP TS 22.173

    // Summary:
    //  Deletes the server event data received.
    void DeleteServerEventData(IN TO mxt_opaque opqEventData);

    // Summary:
    //  Handles the server event data received.
    mxt_result HandleServerEventData(IN TO mxt_opaque opqEventData,
                                     IN ISipServerEventControl* pServerEventControl,
                                     OUT GO ISceGenericEventInfo** ppGenericEventInfo = NULL);

    // Summary:
    //  Handles the server event data received.
    mxt_result HandleServerEventData(IN CSceServerEventData* pServerEventData,
                                     IN ISipServerEventControl* pServerEventControl,
                                     OUT ISceGenericEventInfo** ppGenericEventInfo = NULL);

    // Summary:
    //  This method must be implemented by child components to be notified of
    //  termination of the asynchronous processing.
    virtual void OnUserAgentServiceConfigured(IN mxt_result res,
                                              IN TO CTelUri* pTelUri,
                                              IN TO CList<CSipUri>* plstSipUris);

    // Summary:
    //  Adds all caller preferences header to include in the outgoing request.
    mxt_result AddCallerPrefs(IN const CSceCallerPreferences* pCallerPreferences,
                              INOUT CHeaderList& rExtraHeaders);

    //-- << Context management >>

    // Summary:
    //  Gets the internal SIP context. Optimized internal version
    void GetInternalSipContext(OUT ISipContext*& rpContext) const;
    
    // Summary:
    //  Creates the internal SIP Context.
    virtual mxt_result CreateInternalSipContext();

    // Summary:
    //  Releases the internal SIP Context.
    virtual void ReleaseInternalSipContext();

    // Summary:
    //  Returns whether the context is valid without having to manage references.
    bool IsInternalSipContextValid() const;

    template<class _Type> mxt_result QueryInternalSipContext(OUT _Type*& rpInterface)
    {
        rpInterface = NULL;
        if (m_pContext != NULL)
        {
            return m_pContext->QueryIf(rpInterface->GetIID(),
                                       reinterpret_cast<void**>(&rpInterface));
        }

        return resFE_FAIL;
    }

    // Summary:
    // This method must be implemented by child component to be notified that
    // the user has been terminated.
    virtual void OnUserEventTerminateUser();

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void OnUserEventTerminateUserSilent();
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    // Summary:
    // Sets the application opaque.
    void SetApplicationOpaque(mxt_opaque opq);

    // Summary:
    // Gets the application opaque.
    mxt_opaque GetApplicationOpaque();

    //-- << ECOM mechanism >>

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //-- << CEComUnknown methods >>

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- << CEventDriven mechanism >>

#if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
    //-- Currently only the ENUM service needs this method. If it is deactivated
    //-- Do not overload it. Child classes can still overload it if required.
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessageId,
                                           IN CMarshaler* pParameter);
#endif

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    //-- << ISceBasicExtensionControl Implementation >>

    virtual void SetSipPacketObserver(IN ISceSipPacketObserver* pObserver);

    virtual void SetBasicExtensionManager(IN ISceBasicExtensionMgr* pMgr);

    virtual mxt_result RejectRequest(IN ISceUserConfig* pUserConfig,
                                     IN ISipServerEventControl* pServerEventCtrl,
                                     IN const CSipPacket& rRequest);

    virtual mxt_result SendRequest(IN ESipMethodType eMethodType,
                                   IN mxt_opaque opqTransaction,
                                   IN TO CHeaderList* pExtraHeaders,
                                   IN TO CSipMessageBody* pMessageBody,
                                   IN TO CSceCallerPreferences* pCallerPreferences);

    virtual mxt_result ReIssueRequest();

    virtual void GetLastReceivedPacket(OUT const CSipPacket*& rpPacket);

    virtual void EnableAuthenticationCallback(IN bool bEnable,
                                              IN uint32_t uBitSetDisabledAuth);
    virtual void AcceptChallenge(IN mxt_opaque opqChallenge);
    virtual void RejectChallenge(IN mxt_opaque opqChallenge);
    virtual void GetSipContext(OUT ISipContext** ppContext);

    virtual mxt_result SetTransactionStats(IN IEComUnknown* pTransactionStats);

    virtual void GetTransactionStats(OUT IEComUnknown** ppTransactionStats) const;

    virtual const CSceSipCapabilities& GetSceSipCapabilities() const;

// BRCM_CUSTOM - [add] Add authentication info sharing support
    virtual void AddChallengeInfo(IN mxt_opaque opqChallengeId);

    virtual bool SetChallengeCount(IN mxt_opaque opqChallengeId,
                                   IN unsigned int uNonceCount);

    virtual bool SetChallengeType(IN mxt_opaque opqChallengeId,
                                  IN unsigned int uChallengeType);
                                  
    virtual void ResetChallengeInfo();
// BRCM_CUSTOM - [end] Add authentication info sharing support

    virtual bool IsOptionSupported(IN ESipOptionTag eTag);

    virtual void IgnoreUserEvent(ISceUserEventMgr::EUserEvent eUserEvent);

    virtual bool IsUserEventEnabled(ISceUserEventMgr::EUserEvent eUserEvent);

    virtual mxt_opaque GetComponentOpaque();

    virtual mxt_result ForceSipTransport(IN ESipTransport eTransport);
    virtual ESipTransport GetForcedTransport();
    
    //-- << ISceUaCapabilitiesConfig implementation >>

    virtual mxt_result SetUaCapabilities(IN TO CSceSipFeatureSet* pCapabilities);
    virtual const CSceSipFeatureSet* GetUaCapabilities() const;

    //-- << IPrivateSceBaseComponent implementation >>

    virtual void ReIssueRequestHelper(IN ISipClientEventControl* pClientEventCtrl);

    //-- << IPrivateSceChallengeSource implementation >>

    virtual mxt_result SetCredentials(IN const CString& strRealm,
                                      IN const CString& strUsername,
                                      IN const CString& strPassword,
                                      IN mxt_opaque opqSourceData);

    virtual mxt_result CancelAuthentication(IN const CString& strRealm,
                                            IN mxt_opaque opqSourceData);


    //-- << ISipGenericMgr Implementation >>

    virtual void EvRequest(IN ISipGenericSvc*         pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket&       rRequest);

    virtual void EvProgress(IN ISipGenericSvc*         pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket&       rResponse);

    virtual void EvSuccess(IN ISipGenericSvc*         pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket&       rResponse);

    virtual void EvFailure(IN ISipGenericSvc*         pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket&       rResponse);


    //   << ISipDigestClientAuthMgr Implementation >>

    virtual void EvCredentialsRequired(IN ISipDigestClientAuthSvc* pSvc,
                                       IN ISipClientEventControl* pClientEventCtrl,
                                       IN const CSipPacket& rResponse);
    virtual void EvCredentialsExist(IN ISipDigestClientAuthSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CSipPacket& rResponse);
    virtual void EvInvalidCredentials(IN ISipDigestClientAuthSvc* pSvc,
                                      IN ISipClientEventControl* pClientEventCtrl,
                                      IN const CString& rstrRealm,
                                      IN const CSipPacket& rResponse);
    virtual void EvAuthLoop(IN ISipDigestClientAuthSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse);
// BRCM_CUSTOM - [add] Add server digest authentication support
    virtual void EvSvrAuthenticationFailed(IN ISipDigestClientAuthSvc* pSvc,
                                           IN ISipClientEventControl* pClientEventCtrl,
                                           IN const CSipPacket& rResponse);
// BRCM_CUSTOM - [end] Add server digest authentication support
// BRCM_CUSTOM - [add] Add authentication info sharing support
    virtual void EvAuthenticationSuccess(IN ISipDigestClientAuthSvc* pSvc,
                                         IN ISipClientEventControl* pClientEventCtrl,
                                         IN const CSipPacket& rResponse);

    virtual void EvNonceEmployed(IN ISipDigestClientAuthSvc* pSvc,
                                 IN const CSipPacket& rRequest);

// BRCM_CUSTOM - [end] Add authentication info sharing support
    virtual void EvInvalidResponseCredentials(IN ISipDigestClientAuthSvc* pSvc,
                                              IN ISipClientEventControl* pClientEventCtrl,
                                              IN const CVector<mxt_result>& rvecresReason,
                                              IN const CSipPacket& rResponse);

    //   << ISipRedirectionMgr Implementation >>

    virtual void EvRedirected(IN ISipRedirectionSvc* pSvc,
                              IN ISipClientEventControl* pClientEventCtrl,
                              IN const CSipPacket& rPacket );


    //-- << ISipPrivacyMgr Implementation >>

    virtual void EvUncertifiedPrivacy(IN ISipPrivacySvc* pSvc,
                                      IN const CSipPacket& rRequest,
                                      INOUT mxt_opaque& ropqApplicationData);

    virtual void EvPrivacyDnsResolutionCompleted(IN ISipPrivacySvc* pSvc,
                                                 IN bool bSharedTrustedPrivacy);

    //   << ISipUaAssertedIdentityMgr Implementation >>

    virtual void EvUntrustedProxy(IN ISipUaAssertedIdentitySvc* pSvc,
                                  IN const CSipPacket& rRequest,
                                  INOUT mxt_opaque& rApplicationData);

    virtual void EvUntrustedProxy(IN ISipUaAssertedIdentitySvc* pSvc,
                                  IN ISipClientEventControl* pClientEventCtrl,
                                  IN const CSipPacket& rResponse );

    virtual void EvAssertedIdentity(IN ISipUaAssertedIdentitySvc* pSvc,
                                    IN const CNameAddr* pSipIdentity,
                                    IN const CNameAddr* pTelIdentity,
                                    IN bool bPrivacyRequested,
                                    IN const CSipPacket& rRequest,
                                    INOUT mxt_opaque& rApplicationData );

    virtual void EvAssertedIdentity(IN ISipUaAssertedIdentitySvc* pSvc,
                                    IN const CNameAddr* pSipIdentity,
                                    IN const CNameAddr* pTelIdentity,
                                    IN bool bPrivacyRequested,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CSipPacket& rResponse );

    virtual void EvInvalidAssertedIdentity(IN ISipUaAssertedIdentitySvc* pSvc,
                                           IN const CSipPacket& rRequest,
                                           IN mxt_opaque opqApplicationData,
                                           IN mxt_result resReason );

    virtual void EvInvalidAssertedIdentity(IN ISipUaAssertedIdentitySvc* pSvc,
                                           IN ISipClientEventControl* pClientEventCtrl,
                                           IN const CSipPacket& rResponse,
                                           IN mxt_result resReason );

    virtual void EvTrustedProxyDnsResolutionCompleted(IN ISipUaAssertedIdentitySvc* pSvc,
                                                      IN bool bSharedTrustedProxy);

    //   << ISipCoreOutputControllingMgr Implementation >>

    virtual void EvUpdatePacket(IN ISipCoreOutputControllingSvc* pSvc,
                                IN mxt_opaque transactionOpaque,
                                IN mxt_opaque opqId,
                                INOUT CSipPacket& rPacket,
                                OUT EUpdatingSynchronization& reSynchronization,
                                OUT mxt_result& rresUpdate);

     //-- << ISceUserEventMgr >>

    virtual void EvUserConfigEvent(IN ISceUserEventMgr::EUserEvent eEvent,
                                   IN IEComUnknown* pUserConfig,
                                   IN mxt_opaque opqEvent);

    //   << ISipEnumRequestHandlerMgr Implementation >>
#if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
    virtual void OnGetEnumUriAResult(IN TO CList<SEnumUri>* plstEnumUri,
                                     IN    mxt_opaque       opq,
                                     IN    mxt_result       res);

    void InternalOnGetEnumUriAResult(IN CMarshaler* pParameter);
#endif

    //   << IScePersistentConnectionObserver Implementation >>

    // Summary:
    //  This method must be implemented by child components to be notified of
    //  a persistent connection establishment.
    virtual void OnEvConnectionEstablished(IN ISceUserConfig* pUserConfig);

    // Summary:
    //  This method must be implemented by child components to be notified of
    //  a persistent connection error on connection.
    virtual void OnEvErrorOnConnection(IN ISceUserConfig* pUserConfig);

    // Summary:
    //  This method must be implemented by child components to be notified of
    //  a persistent connection termination.
    virtual void OnEvConnectionTerminated(IN ISceUserConfig* pUserConfig);

    //   << ISipPacketObserverSvcMgr Implementation >>

    virtual void EvRequestReceived(IN ISipPacketObserverSvc* pSvc,
                                   IN ISipServerEventControl* pServerEventCtrl,
                                   IN const CSipPacket& rPacketIn);
    virtual void EvResponseReceived(IN ISipPacketObserverSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CSipPacket& rPacketIn);

    //   << ISipGenericClientAuthenticationMgr Implementation >>

    virtual void EvChallenged(IN ISipGenericClientAuthenticationSvc* pSvc,
                              IN ISipClientEventControl* pClientEventControl,
                              IN const CSipPacket& rChallenge);

    //   << ISceUserIdentityConfig Implementation >>

    virtual mxt_result SetPreferredIdentity(IN TOA CNameAddr* pSipIdentity,
                                            IN TOA CNameAddr* pTelIdentity);

    virtual void GetPreferredIdentity(OUT const CNameAddr*& rpSipIdentity,
                                      OUT const CNameAddr*& rpTelIdentity) const;

    virtual void SetPrivacyMode(IN EPrivacyMode eMode);

    virtual EPrivacyMode GetPrivacyMode() const;

    virtual ENetworkAssertedIdMode GetNetworkAssertedIdentityMode() const;

    virtual void SetNetworkAssertedIdentityMode(IN ENetworkAssertedIdMode eMode);

    // Summary:
    //  This methods must be implemented by child components to be notified of
    //  changes in the capabilities.
    virtual void OnUaCapabilitiesChanged(IN const CSceSipFeatureSet* pNewCapabilities);

    // Summary:
    //  This methods enables a child component to change the capabilities set by the user or set its own.
    CSceSipFeatureSet* GetUaCapabilitiesHelper();

    // Summary:
    //  Method to tell if the component's persistent connections is established or not.
    bool IsPersistentConnectionEstablished();

    // Summary:
    //  Overrides the default tracing nodes.
    void OverrideBaseTracingNodes(IN STraceNode& rNewChildNode,
                                  IN STraceNode& rNewChildEComNode);

    // Summary:
    //  Adds the UA capabilities in the provided contact header using the proper
    //  method for the contact type.
    void AddCapabilitiesToContactHelper(IN EContactType eContactType,
                                        INOUT CSipHeader*& rpContact) const;

    // Summary:
    //  Updates the UA capabilities on the provided vector of contacts.
    void UpdateUaCapabilitiesHelper(IN EContactType eContactType,
                                    IN const CSceSipFeatureSet* pNewUaCapabilities,
                                    INOUT CVector<CSipHeader*>& rvecpHeaders) const;

    // Summary:
    //  Updates the UA capabilities on the provided contacts.
    void UpdateUaCapabilitiesHelper(IN EContactType eContactType,
                                    IN const CSceSipFeatureSet* pNewUaCapabilities,
                                    INOUT CSipHeader*& pHeaders) const;

    // Summary:
    //  Notifies the ISceBasicExtensionMgr that a ISipContext has been created.
    void NotifyExtensionMgrOfCreatedSipContext();

    // Summary:
    //  Gets the old call back and parameters used by the ISipServerLocationSvc.
    void GetOldServerLocationSvcCallback(OUT mxt_PFNServerLocationListModifier& rpfnOldCallback,
                                         OUT mxt_opaque& ropqOldParameter);

    // Summary:
    //  Gets the current User Identity Privacy Mode.
    ISceUserIdentityConfig::EPrivacyMode GetUserIdentityPrivacyMode() const;

    // Summary:
    //  Gets the tracing nodes.
    STraceNode& GetTraceNode();

    // Summary:
    //  Gets the ECOM tracing nodes.
    STraceNode& GetEComTraceNode();

private:
    //-- << IScePersistentConnectionObserver >>
    virtual void EvConnectionEstablished(IN ISceUserConfig* pUserConfig);

    virtual void EvErrorOnConnection(IN ISceUserConfig* pUserConfig);

    virtual void EvConnectionTerminated(IN ISceUserConfig* pUserConfig);

    //-- << Helper methods >>

    // Summary:
    //  Attaches the user agent service.
    mxt_result AttachUserAgentSvc();

    // Summary:
    //  Attaches the client digest authentication service.
    mxt_result AttachAuthSvc();

    // Summary:
    //  Attaches the redirection service.
    void AttachRedirectionSvc();

    // Summary:
    //  Attaches the generic service.
    void AttachGenericSvc();

    // Summary:
    //  Attaches the UA Asserted Identity service.
    mxt_result AttachUaAssertedIdSvc();

    // Summary:
    //  Attaches the privacy service.
    mxt_result AttachPrivacySvc();

    // Summary:
    //  Configures the privacy service.
    mxt_result ConfigurePrivacySvc(ISipPrivacySvc* pPrivacySvc,
                                   ISceUserIdentityConfig::EPrivacyMode eMode);

    // Summary:
    //  Attaches the statistics service.
    void AttachStatisticsSvc();

    // Summary:
    //  Attaches the output controlling service.
    void AttachOutputControllingSvc();

    GO CSipHeader* BuildUserContactHelper(IN IUri::EUriType eUriType) const;
    GO CSipHeader* BuildDeviceContactHelper(IN IUri::EUriType eUriType) const;

    // Summary:
    //  Internal method to check all asynchronous possiblities,
    //  (i.e. ENUM resolution, persistent connections establishment, etc.)
    //  after ConfigureUserAgentSvc has been called by the child component.
    mxt_result ConfigureUserAgentSvcHelper(IN const IUri* pCurrentTarget,
                                           IN bool bCalledFromApplication);

    // Summary:
    //  Generate caller pref headers.
    mxt_result GenerateCallerPrefHeaders(IN const CVector<const CSceSipFeatureSet*>& rvecpSets,
                                         IN ESipHeaderType eHdrType,
                                         OUT CSipHeader*& rpHeader);

    // Summary:
    //  Handles the privacy-related server event data.
    mxt_result HandlePrivacyServerEventData(IN CSceServerEventData* pServerEventData,
                                            IN ISipServerEventControl* pServerEventControl);

    // Summary:
    //  Handles the UaAssertedIdentity-related server event data.
    mxt_result HandleAssertedIdentityServerEventData(IN CSceServerEventData* pServerEventData,
                                                     IN ISipServerEventControl* pServerEventControl,
                                                     OUT ISceGenericEventInfo** ppGenericEventInfo);

    // Summary:
    //  Handles the caller preferences processing.
    void HandleCallerPreferencesHelper(IN const CSipPacket& rPacket);

    // Summary:
    //  Helper to make sure all the resolutions are completed before going on.
    void PrivacyAssertedIdDnsResolutionCompletedHelper();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceBaseComponent(IN const CSceBaseComponent& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceBaseComponent& operator=(IN const CSceBaseComponent& rSrc);


//-- Hidden Data Members.
protected:
private:
    // The trace node of the child class. This is used in all traces of this
    // class (except ECOM related traces) to help isolate the traces of a
    // specific component type.
    STraceNode* m_pstTraceNode;

    // The ECOM trace node of the child class. This is used in all ECOM traces
    // of this class to help isolate the traces of a specific component type.
    STraceNode* m_pstEcomTraceNode;

    // Pointer to the context object. This is an ECOM and we are keeping a
    // reference to it.
    ISipContext* m_pContext;

    // Pointer to the user configuration object. This is an ECOM and we are
    // keeping a reference to it. It can be NULL throughout the life of this
    // object, which has impact on how ISipGenericMgr::EvRequest is managed.
    ISceUserConfig* m_pUserConfig;

    // The capabilities manager. This is initialized with the configuration
    // found in ISceCoreConfig, but can later be updated by the component
    // inheriting from this class.
    CSceSipCapabilities m_capsMgr;

    // Pointer to the interface where every packet sent and received are reported
    // before being processed. This interface is optional.
    ISceSipPacketObserver* m_pSipPacketObserver;

    // Pointer to the last received packet.
    const CSipPacket* m_pLastReceivedPacket;

    // Pointer to the interface where basic extension events are reported. This
    // includes reception of responses for requests sent through the
    // ISceBasicExtensionControl interface and requests not directly managed by
    // the component. This interface is optional.
    ISceBasicExtensionMgr* m_pBasicExtensionMgr;

    // The list of transactions sent through the ISceBasicExtensionControl
    // interface of this component. It is kept so the component can know
    // whether responses need to be reported through its m_pBasicExtensionMgr
    // or not. Only responses for transactions in this list are reported.
    CList<ISipClientTransaction*> m_lstpTransactions;

    // Tells if the asserted identity host is currently being resolved by the
    // ISipUaAssertedIdentitySvc.
    bool m_bAssertedIdResolving;

    // Tells if the asserted identity service is attached and the route to
    // the trusted server is Ok.
    bool m_bAssertedIdSvcReady;

    // Tells if the privacy host is currently being resolved by the
    // ISipPrivacySvc.
    bool m_bPrivacyResolving;

    // Tells that the Privacy service is attached and the preloaded route
    // is Ok.
    bool m_bPrivacySvcReady;

    // Identity config override.
    IEComUnknown* m_pUserIdentityConfigUnknown;

    // This is the transaction statistics to be set on the ISipTransactionSvc.
    ISipTransactionStatistics* m_pTransactionStats;

    // Tells if the persistent connection is connected or not.
    bool m_bIsPersistentConnectionEstablished;

    // EvUpdatePacket temporary data member.
    mxt_opaque m_opqEvUpdatePacketId;
    CSipPacket* m_pBufferedOutgoingPacket;

    // HandleInitialServerRequest temporary data member.
    const CSipPacket* m_pBufferedIncomingPacket;

    // ConfigureUserAgentSvc temporary data member.
    IUri* m_pConfigureUaCurrentTarget;

    // OnUserAgentServiceConfigured temporary data member.
    mxt_result m_resConfigureUserAgentSvc;
    CTelUri* m_pTelUri;
    CList<CSipUri>* m_plstSipUris;

    // The current capabilities, either from the user config or local.
    CSceSipFeatureSet* m_pUaCapabilities;

    // The current caller preferences.
    CSceCallerPreferences* m_pCallerPrefs;

    // The current status of the generic authentication feature.
    bool m_bAuthenticationCallbackEnabled;
    uint32_t m_uDisabledAuthenticationMechanism;

    // Application set opaque.
    mxt_opaque m_opqApplication;

// BRCM_CUSTOM - [mod] Add Timer F expiration handling support per IMS
    // Ignored user event mask.
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS    
    int32_t m_nUserEventMask;

    // The forced transport to send packets on.
    ESipTransport m_eForcedTransport;

    // The old call back and parameters for the ISipServerLocationSvc.
    mxt_PFNServerLocationListModifier m_pfnOldLocationListCallback;
    mxt_opaque m_opqOldCallbackParameter;

    // Current client event control, used to reissue requests.
    ISipClientEventControl* m_pCurrentRequestClientEventCtrl;

    // The current preferred Identities.
    bool m_bPreferedIdentityOverriden;

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  SSearchedBody METHODS  =================================================
//==============================================================================

//==============================================================================
//==
//==  SSearchedBody
//==
//==============================================================================
//
//  Description:
//      Default constructor.
//
//==============================================================================

inline CSceBaseComponent::SSearchedBody::SSearchedBody()
:   m_pszContentTypeMType(NULL),
    m_pszContentTypeMSubType(NULL),
    m_pszContentDisposition(NULL),
    m_ppFoundBody(NULL)
{
}

//==============================================================================
//====  CSceBaseComponent METHODS  =============================================
//==============================================================================

//==========================================================================
//==
//==  GetUserConfig
//==
//==========================================================================
//
//  Parameters:
//    rpConfig:
//      Pointer to a user configuration object. If the pointer returned is not
//      NULL, its reference will have been increased by one. The caller must
//      release this reference when it is done with the object.
//
//  Description:
//      Provides access the the user configuration object associated with this
//      component.
//
//  See Also:
//      SetUserConfig
//
//==========================================================================
inline void CSceBaseComponent::GetUserConfig(OUT ISceUserConfig*& rpConfig) const
{
    rpConfig = m_pUserConfig;
    if (rpConfig != NULL)
    {
        rpConfig->AddIfRef();
    }
}

// BRCM_CUSTOM - [add] Add connected ID support per RFC 4916
//==========================================================================
//==
//==  GetUserIdentityConfigUnknown
//==
//==========================================================================
//
//  Parameters:
//    rpIdentityConfigUnknown:
//      Pointer to a user identity configuration override object. If the 
//      pointer returned is not NULL, its reference will have been increased 
//      by one. The caller must release this reference when it is done with 
//      the object.
//
//  Description:
//      Provides access to the user identity configuration object associated 
//      with this component.
//
//  See Also:
//      N/A
//
//==========================================================================
inline void CSceBaseComponent::GetUserIdentityConfigUnknown(OUT IEComUnknown*& rpIdentityConfigUnknown) const
{
    // ECom unknown for the identity config override.
    rpIdentityConfigUnknown = m_pUserIdentityConfigUnknown;
    if (rpIdentityConfigUnknown != NULL)
    {
        rpIdentityConfigUnknown->AddIfRef();
    }
}
// BRCM_CUSTOM - [end] Add connected ID support per RFC 4916

//==========================================================================
//==
//==  GetInternalSipContext
//==
//==========================================================================
//
//  Parameters:
//    rpContext:
//      Pointer to the SIP context object. If the pointer returned is not
//      NULL, its reference will have been increased by one. The caller must
//      release this reference when it is done with the object.
//
//  Description:
//      Provides access the the SIP context object associated with this
//      component.
//
//==========================================================================
inline void CSceBaseComponent::GetInternalSipContext(OUT ISipContext*& rpContext) const
{
    rpContext = m_pContext;
    if (rpContext != NULL)
    {
        rpContext->AddIfRef();
    }
}

//==============================================================================
//==
//==  IsInternalSipContextValid
//==
//==============================================================================
//
//  Returns:
//      True if the context is valid, false otherwise.
//
//  Description:
//      Check if the context is valid. A valid context is a non-null m_pContext.
//
//==============================================================================
inline bool CSceBaseComponent::IsInternalSipContextValid() const
{
    return m_pContext != NULL;
}

//==============================================================================
//==
//==  GetSipPacketObserver
//==
//==============================================================================
//
//  Parameters:
//    rpObserver:
//      Pointer to the SIP packet observer. May be NULL.
//
//  Description:
//      Provides access to the sent packet manager associated with this
//      component.
//
//  See Also:
//      SetSentPacketManager
//
//==============================================================================
inline void CSceBaseComponent::GetSipPacketObserver(OUT ISceSipPacketObserver*& rpObserver) const
{
    rpObserver = m_pSipPacketObserver;
}

//==========================================================================
//==
//==  GetBasicExtensionManager
//==
//==========================================================================
//
//  Parameters:
//    rpMgr:
//      Pointer to the basic extension manager. May be NULL.
//
//  Description:
//      Provides access to the basic extension manager associated with this
//      component.
//
//  See Also:
//      SetBasicExtensionManager
//
//==========================================================================
inline void CSceBaseComponent::GetBasicExtensionManager(OUT ISceBasicExtensionMgr*& rpMgr) const
{
    rpMgr = m_pBasicExtensionMgr;
}

//==========================================================================
//==
//==  GetApplicationOpaque
//==
//==========================================================================
//
//  Returns:
//      The application opaque data set.
//
//  Description:
//      Gets the component application opaque.
//
//==========================================================================
inline mxt_opaque CSceBaseComponent::GetApplicationOpaque()
{
    return m_opqApplication;
}

//==========================================================================
//==
//==  GetSceSipCapabilities
//==
//==========================================================================
//
//  Returns:
//      The capabilities manager.
//
//  Description:
//      Provides access to the capabilities manager.
//
//==========================================================================
inline CSceSipCapabilities& CSceBaseComponent::GetSceSipCapabilities()
{
    return m_capsMgr;
}

//==========================================================================
//==
//==  GetUaCapabilitiesHelper
//==
//==========================================================================
//
//  Returns:
//      The current capabilities, either from the user config or local.
//
//  Description:
//      Provides access to the current UA capabilities.
//
//==========================================================================
inline CSceSipFeatureSet* CSceBaseComponent::GetUaCapabilitiesHelper()
{
    return m_pUaCapabilities;
}

//==========================================================================
//==
//==  IsPersistentConnectionEstablished
//==
//==========================================================================
//
//  Returns:
//      True if the persistent connection is established , false otherwise.
//
//  Description:
//      Tells if the component's persistent connection is established
//      or not.
//
//==========================================================================
inline bool CSceBaseComponent::IsPersistentConnectionEstablished()
{
    return m_bIsPersistentConnectionEstablished;
}

//==========================================================================
//==
//==  OverrideBaseTracingNodes
//==
//==========================================================================
//
//  Parameters:
//    rNewChildNode:
//      The new component node to use for tracing.
//
//    rNewChildEComNode:
//      The new ECOM node to use for tracing.
//
//  Description:
//      This method overrides the normal tracing nodes with new ones to use.
//      This enables the derived child components to specifically trace all
//      the different calls to their own base component with different nodes.
//
//==========================================================================
inline void CSceBaseComponent::OverrideBaseTracingNodes(IN STraceNode& rNewChildNode,
                                                        IN STraceNode& rNewChildEComNode)
{
    m_pstTraceNode = &rNewChildNode;
    m_pstEcomTraceNode = &rNewChildEComNode;
}

//==============================================================================
//==
//==  GetTraceNode
//==
//==============================================================================
//
//  Returns:
//      The node to use for tracing.
//
//  Description:
//      This method returns the correct node to use for tracing in the
//      child component. This is especially useful when the child's tracing
//      nodes can be overridden.
//
//  See Also:
//      CSceBaseComponent
//
//==============================================================================
inline STraceNode& CSceBaseComponent::GetTraceNode()
{
    return *m_pstTraceNode;
}

//==============================================================================
//==
//==  GetEComTraceNode
//==
//==============================================================================
//
//  Returns:
//      The node to use for ECOM tracing.
//
//  Description:
//      This method returns the correct node to use for ECOM tracing in the
//      child component. This is especially useful when the child's tracing
//      nodes can be overridden.
//
//  See Also:
//      CSceBaseComponent
//
//==============================================================================
inline STraceNode& CSceBaseComponent::GetEComTraceNode()
{
    return *m_pstEcomTraceNode;
}

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSCEBASECOMPONENT_H
