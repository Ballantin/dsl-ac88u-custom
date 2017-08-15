//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_CSIPCORECONFIG_H
#define MXG_CSIPCORECONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
// MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT
// MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT
// MXD_SIPSTACK_ENABLE_DEFAULT_DATA_LOGGER.
// MXD_SIPSSTACK_ENABLE_TLS.
// MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT
//-----------------------------------------------------------
#include "Config/SipStackCfg.h"
#endif


//-- Data Members.
//-----------------
#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

//-- Parameters used by Value.
//-----------------------------
#ifndef MXG_VERSIONSIPSTACK_H
#include "Config/VersionSipStack.h"
#endif


//-- Interface Realized and/or Parent.
//-------------------------------------
#ifndef MXG_ISIPCORECONFIG_H
#include "SipCore/ISipCoreConfig.h"
#endif

#ifndef MXG_IMESSAGESERVICEMGR_H
#include "ServicingThread/IMessageServiceMgr.h"
#endif

#ifndef MXG_ITIMERSERVICEMGR_H
#include "ServicingThread/ITimerServiceMgr.h"
#endif

#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#ifndef MXG_ISIPTRANSPORTMGR_H
#include "SipTransport/ISipTransportMgr.h"
#endif

#ifndef MXG_ISIPTRANSPORTUSER_H
#include "SipTransport/ISipTransportUser.h"
#endif

#ifdef MXD_SIPSTACK_ENABLE_TLS
#ifndef MXD_ISIPTLSCONTEXTFACTORY_H
#include "SipTransport/ISipTlsContextFactory.h"
#endif
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
//-------------------------
class CMarshaler;
class CSipConnectionBlacklist;
class CSipDialogMatcherList;
class CSipEntity;
class CSipNetworkInterfaceList;
class CSipPersistentConnectionList;
class CSipNetworkInterfaceList;
class CSipTransactionMgr;
class CSipTransportMgr;
class CSipPersistentConnectionList;
class CSipEntity;
class CSipDefaultDataLogger;
class ISipDataLogger;
#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)
    class ISipKeepAliveMgr;
    class ISipKeepAliveExtensionMgr;
#endif

#if defined(MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT)
    class CTargetManagementModule;
#endif

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipCoreConfig
//========================================
//
// Description:
//   This class implements the ISipCoreConfig interface. It offers some more
//   APIs than the ones defined in the ISipCoreConfig interface, which are used
//   privately within the SipCore.
//
// Location:
//   SipCore/CSipCoreConfig.h
//
// See Also:
//   ISipCoreConfig
//
//==EDOC========================================================================
class CSipCoreConfig : protected ISipCoreConfig,
                       protected IMessageServiceMgr,
                       protected ITimerServiceMgr,
                       protected CEComUnknown,
                       protected ISipTransportUser
{
//-- Friend Declarations.
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    friend class CSipUserAgentSvc;                // ms_pNetworkInterfaceList
    friend class CSipProxyHelper;                 // ms_pNetworkInterfaceList
    friend class CSipNetworkInterfaceList;        // ms_eAddressTypePreference
                                                  // ms_pPersistentConnectionList
    friend class CSipReqCtxServerLocationSvc;     // ms_pNetworkInterfaceList
    friend class CSipPersistentConnectionList;    // ms_pNetworkInterfaceList
    friend class CSipSymmetricUdpSvc;             // ms_pNetworkInterfaceList
    friend class CSipRegistrationSvc;             // ms_pNetworkInterfaceList
    friend class CSipViaManagementSvc;            // ms_pNetworkInterfaceList
#ifdef MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT
    friend class CSipOutboundConnectionSvc;
#endif
    //M5T_INTERNAL_USE_END


//-- Published Interface.
//------------------------
public:

    // Summary:
    //  Returns the current ISipDataLogger instance.
    //-----------------------------------------------
    static ISipDataLogger* GetCurrentSipDataLogger();

    // Summary:
    //  Disables all nodes with a given name. Set pszNodeName = "FeatureECOM"
    //  to disable all Feature ECOM traces.
    //-------------------------------------------------------------------------
    static mxt_result DisableNamedNodes(IN STraceNode* pstParentNode,
                                        IN const char* pszNodeName,
                                        IN bool bRecursive);

    // Summary:
    //  Returns if a 64bits sequence number is supported.
    //---------------------------------------------------
    static bool GetCSeq64BitsSupport();

#if defined(MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT)
    // Summary:
    //  Sets if the handling of bad events in NotifierSvc
    //  is done internally or not.
    //----------------------------------------------------
    static void SetNotifierSvcInternalHandlingOfBadEvents(IN bool bHandleInternally);
#endif
#ifdef MXD_SIPSTACK_ENABLE_TLS

    // Summary:
    //  For internal use only. To release the Tls Context
    //  Factory.
    //----------------------------------------------------
    static void ReleaseTlsContextFactory();

#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS

    //M5T_INTERNAL_USE_BEGIN
    // Summary:
    //  Returns a pointer to CTransportMgr. This function is only for test and it
    //  must not be used by the application.
    static ISipTransportMgr* GetTransportMgr();

    static CSipDialogMatcherList* GetDefaultDialogMatcherList();
    //M5T_INTERNAL_USE_END

//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipCoreConfig(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipCoreConfig();


    //-- << Helper methods >>
    //------------------------

    // Summary:
    //  Checks whether or not the thread supports servicing thread interfaces.
    //-------------------------------------------------------------------------
    mxt_result CheckThreadInterfaces(IN IEComUnknown* pThread);

    // Messages.
    //-----------
    enum EMessage
    {
        eSET_COMMA_SEPARATED_HEADER,
        eSET_HEADER_FORM_PREFERENCE,
        eSET_UA_RESPONSE_MULTIPLE_VIAS_CHECK,
        eIGNORE_TOP_VIA_SENT_BY_IN_RECEIVED_RESPONSES,
        eSET_PRINCIPAL_BUFFER_SIZE,
        eSET_MAX_SEND_BUFFER_SIZE,
// BRCM_CUSTOM [begin] - Bypass SubjectAltName check
#ifdef MXD_SIPSTACK_ENABLE_TLS
        eSET_BYPASS_SUBJECT_ALT_NAME_CHECK,
#endif
// BRCM_CUSTOM [end] - Bypass SubjectAltName check
        eSET_MAX_PAYLOAD_SIZE,
        eSET_MAX_RECEIVE_PACKET_SIZE,
        eSET_UDP_MAX_SIZE_THRESHOLD,
        eSET_CONNECTION_PARAMETERS,
        eSET_PACKET_INSPECTOR_CALLBACK,
        eSET_PACKET_MODIFIER_CALLBACK,
        eSET_T1,
        eSET_T2,
        eSET_T4,
        eSET_TIMEOUT_TIMER,
        eSET_TIMER_B,
        eSET_TIMER_D,
        eSET_TIMER_F,
        eSET_TIMER_H,
        eSET_TIMER_J,
// BRCM_CUSTOM - [begin] Control branch-id matching choices in ACK
#ifndef MXD_SIPSTACK_ENABLE_SERVER_INVITE_SAME_BRANCH_ACK
        eSET_BYPASS_ACK_BRANCH_ID_CHECK,
#endif
// BRCM_CUSTOM - [end] Control branch-id matching choices in ACK
        eSET_CORE_USER,
#if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
        eSET_ENUM_E164_ZONE_SUFFIX,
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
        eSET_SIP_DATA_LOGGER,
        eSET_SUPPORTED_SIP_TRANSPORT,
        eSET_SUPPORTED_DNS_QUERIES,
        eSET_FAILOVER_MODE,
        eSET_SUPPORTED_64BITS_SEQUENCE,
#if defined (MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
        eADD_SWITCHBACK_CONNECTION,
        eREMOVE_SWITCHBACK_CONNECTION,
#endif //-- #if defined (MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
        eSET_SUPPORTED_IP_VERSION,
        eADD_LOCAL_ADDRESS,
        eREMOVE_LOCAL_ADDRESS,
        eLISTEN,
        eSTOP_LISTENING,
        eGET_NETWORK_IF_LIST,
#ifdef MXD_SIPSTACK_ENABLE_TLS
        eGET_TLS_CONTEXT_FACTORY,
        eSET_HANDSHAKE_VALIDATOR_CALLBACK,
        eSET_CACHED_TLS_SESSION_LIST_MAX_SIZE,
#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS
        eSHUTDOWN_COMPLETED,
        eSET_ENTITY_ID,
        eSET_SOCKET_CLOSURE_TYPE,
        eFORCE_VISIBLE_LOCAL_ADDRESS,
#if defined MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT
        eSET_REGISTRATION_DEFAULT,
#endif
#if defined MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT
        eSET_DEFAULT_CALL_ID_LENGTH,
        eGET_DEFAULT_CALL_ID_LENGTH,
#endif
    };

    // Summary:
    //  Posts the synchronous message to the specified thread.
    //---------------------------------------------------------
    mxt_result PostSyncMessage(IN IEComUnknown* pThread,
                               IN EMessage EMessage,
                               IN TO CMarshaler* pParams);

    // Summary:
    //  Posts the message to the specified thread.
    //---------------------------------------------
    mxt_result PostMessage(IN IEComUnknown* pThread,
                           IN bool bWaitCompletion,
                           IN EMessage eMessage,
                           IN TO CMarshaler* pParams);


    //-- << Messaging methods >>
    //---------------------------

    void InternalSetCommaSeparatedHeader(IN CMarshaler* pParams);

    void InternalSetHeaderFormPreference(IN CMarshaler* pParams);

    void InternalSetUaResponseMultipleViasCheck(IN CMarshaler* pParams);

    void InternalIgnoreTopViaSentByInReceivedResponses(IN CMarshaler* pParams);

    void InternalSetPrincipalBufferSize(IN CMarshaler* pParams);

    void InternalSetMaxSendBufferSize(IN CMarshaler* pParams);

// BRCM_CUSTOM [begin] - Bypass SubjectAltName check
#ifdef MXD_SIPSTACK_ENABLE_TLS
    void InternalSetBypassSubjectAltNameCheck(IN CMarshaler* pParams);
#endif
// BRCM_CUSTOM [end] - Bypass SubjectAltName check

    void InternalSetMaxPayloadSize(IN CMarshaler* pParams);

    void InternalSetMaxReceivePacketSize(IN CMarshaler* pParams);

#ifdef MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT
    void InternalSetUdpMaxSizeThreshold(IN CMarshaler* pParams);
#endif

    void InternalSetConnectionParameters(IN CMarshaler* pParams);

    void InternalSetPacketInspectorCallback(IN CMarshaler* pParams);

    void InternalSetPacketModifierCallback(IN CMarshaler* pParams);

    void InternalSetT1(IN CMarshaler* pParams);

    void InternalSetT2(IN CMarshaler* pParams);

    void InternalSetT4(IN CMarshaler* pParams);

    void InternalSetTimeoutTimer(IN CMarshaler* pParams);

    void InternalSetTimerB(IN CMarshaler* pParams);

    void InternalSetTimerD(IN CMarshaler* pParams);

    void InternalSetTimerF(IN CMarshaler* pParams);

    void InternalSetTimerH(IN CMarshaler* pParams);

    void InternalSetTimerJ(IN CMarshaler* pParams);

// BRCM_CUSTOM - [begin] Control branch-id matching choices in ACK
#ifndef MXD_SIPSTACK_ENABLE_SERVER_INVITE_SAME_BRANCH_ACK
    void InternalSetBypassAckBranchIdCheck(IN CMarshaler* pParams);
#endif
// BRCM_CUSTOM - [end] Control branch-id matching choices in ACK

    void InternalSetCoreUser(IN CMarshaler* pParams);

#if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
    void InternalSetEnumE164ZoneSuffix(IN CMarshaler* pParams);
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)

    void InternalSetSipDataLogger(IN CMarshaler* pParams);

    void InternalSetSupportedSipTransport(IN CMarshaler* pParams);

    void InternalSetSupportedDnsQueries(IN CMarshaler* pParams);

    void InternalSetFailoverMode(IN CMarshaler* pParams);

    void InternalSetCSeq64BitsSupport(IN CMarshaler* pParams);

#if defined (MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
    void InternalAddSwitchbackPersistentConnection(IN CMarshaler* pParams);
    void InternalRemoveSwitchbackPersistentConnection(IN CMarshaler* pParams);
#endif //-- #if defined (MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)

    void InternalSetSupportedIpVersion(IN CMarshaler* pParams);

    void InternalAddLocalAddress(IN CMarshaler* pParams);

    void InternalRemoveLocalAddress(IN CMarshaler* pParams);

    void InternalListenA(IN CMarshaler* pParams);

    void InternalStopListeningA(IN CMarshaler* pParams);

    void InternalGetNetworkInterfaceList(IN CMarshaler* pParams);

#ifdef MXD_SIPSTACK_ENABLE_TLS
    void InternalGetTlsContextFactory(IN CMarshaler* pParams);
    void InternalSetHandshakeValidatorCallback(IN CMarshaler* pParams);
    void InternalSetTlsSessionCacheMaxSize(IN CMarshaler* pParams);
#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS

    void InternalShutdownCompleted();

    void InternalSetEntityId(IN CMarshaler* pParams);

    void InternalSetSocketClosureType(IN CMarshaler* pParams);

    void InternalForceVisibleLocalAddress(IN CMarshaler* pParams);

#if defined MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT
    void InternalSetDefaultRegistrationContactMatchingType(IN CMarshaler* pParams);
#endif
#if defined MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT
    void InternalSetDefaultCallIdLength(IN CMarshaler* pParams);
    void InternalGetDefaultCallIdLength(IN CMarshaler* pParams);
#endif

    //-- << CEComUnknown Interface >>
    //--------------------------------

    MX_DECLARE_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << ISipCoreConfig >>
    //------------------------

    //-- << Threading Initialization >>
    //----------------------------------

    virtual mxt_result SetCoreThread(IN IEComUnknown* pThread);

    virtual mxt_result GetCoreThread(OUT IEComUnknown** ppThread) const;

    virtual mxt_result SetTransportThread(IN IEComUnknown* pThread);

    virtual mxt_result GetTransportThread(OUT IEComUnknown** ppThread) const;

    virtual mxt_result SetDnsResolverThread(IN IEComUnknown* pThread);

    virtual mxt_result GetDnsResolverThread(OUT IEComUnknown** ppThread) const;

    //-- << Parser Configuration >>
    //------------------------------

    virtual mxt_result SetCommaSeparatedHeader(
                                        IN ESipHeaderType eHeader,
                                       IN ECommaSeparatedHeaderConfig eConfig);

    virtual mxt_result SetHeaderFormPreference(
                                          IN EHeaderFormPreference ePreference);

    virtual void SetApplicationId(IN const CString& rstrUserAgentId,
                                  IN const CString* pstrServerId = NULL);

    virtual void AddStackVersionToId(IN bool bAdd);

    virtual void AddUserAgentToRequests(IN bool bAdd);

// BRCM_CUSTOM [add] Add User-Agent header to responses
    virtual void AddUserAgentToResponses(IN bool bAdd);
// BRCM_CUSTOM [end] Add User-Agent header to responses

    virtual void AddServerToResponses(IN bool bAdd);

    virtual void SetMaxForwards(IN unsigned int uMaxForwards);

    virtual mxt_result SetUaResponseMultipleViasCheck(IN bool bCheckMultipleVias,
                                                      IN TO CVector<CString>* pvecstrProxyIds = NULL);

    virtual mxt_result IgnoreTopViaSentByInReceivedResponses(IN bool bIgnore);

#ifdef MXD_SIPSTACK_ENABLE_SIP_VIA_MANAGEMENT_SVC_SUPPORT
    virtual void AddViaRportParam(IN bool bAddRport);
#endif

    mxt_result ForceVisibleLocalAddress(IN mxt_opaque opqAddress,
                                        IN TOA CHostPort* pVisibleAddress);

    virtual mxt_result UpdateParserGrammar(IN const unsigned int uIndex,
                                           IN const bool bValue,
                                           IN const CToken::ECharSet eCharSet);

    //-- << Transport Configuration >>
    //---------------------------------

    virtual mxt_result SetPrincipalBufferSize(IN unsigned int uSize);

    virtual mxt_result SetMaxSendBufferSize(IN unsigned int uSize);

// BRCM_CUSTOM [begin] - Bypass SubjectAltName check
#ifdef MXD_SIPSTACK_ENABLE_TLS
    virtual mxt_result SetBypassSubjectAltNameCheck(IN bool bBypass);
#endif
// BRCM_CUSTOM [end] - Bypass SubjectAltName check

    virtual mxt_result SetMaxPayloadSize(IN unsigned int uSize);

    virtual mxt_result SetMaxReceivePacketSize(IN unsigned int uSize);

#ifdef MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT
    // Note: the server location service uses the value specified by this
    // method to determine the maximum size a packet should have,
    // given its transport protocol. When the server location service
    // is not enabled, the maximum size defaults to infinite. However,
    // this should not happen under normal circumstances because all use
    // cases require this service to be enabled.
    //---------------------------------------------------------------------
    virtual void SetUdpMaxSizeThreshold(IN unsigned uSize);
#endif

    virtual mxt_result SetConnectionParameters(
                                       IN unsigned int uNumConnectionTarget,
                                       IN unsigned int uMaxNumberOfConnections,
                                       IN unsigned int uInactivityTimeoutSec);

    virtual mxt_result SetPacketInspectorCallback(
                    IN PFNTransportPacketInspector pfnTransportPacketInspector);

    virtual mxt_result SetPacketModifierCallback(
                    IN PFNTransportPacketModifier pfnTransportPacketModifier);

    virtual void SetSipDataLogger(IN ISipDataLogger* pDataLogger);

    virtual void SetSupportedSipTransport(IN int nTransportBitset);

    virtual void SetSupportedDnsQueries(IN int nDnsQueryBitset);

    virtual void SetFailoverMode(IN EFailoverMode eFailoverMode);

    virtual void SetCSeq64BitsSupport(IN bool bSupported);

#if defined (MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
    virtual mxt_result AddSwitchbackPersistentConnection(OUT ISipSwitchbackPersistentConnection*& rpSwitckBackManagement);
    virtual mxt_result RemoveSwitchbackPersistentConnection(IN ISipSwitchbackPersistentConnection* pSwitckBackManagement);
#endif //-- #if defined (MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)

    virtual EIpVersionConfig GetSupportedIpVersion();
    virtual void SetSupportedIpVersion(IN EIpVersionConfig eIpVersion);

#ifdef MXD_SIPSTACK_ENABLE_TLS

    virtual mxt_result GetTlsContextFactory(OUT ISipTlsContextFactory*& rpTlsContextFactory);

    virtual mxt_result SetHandshakeValidatorCallback(
                                 IN PFNHandshakeValidator pfnHandshakeValidator,
                                 IN bool bOverrideDefaultBehaviour);

    virtual mxt_result SetTlsSessionCacheMaxSize(IN uint32_t uTlsSessionCacheMaxSize);
#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS

#if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)
    virtual mxt_result SetSpirallingSvcState(IN ESpirallingSvcState eState);
#endif //-- #if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

    virtual mxt_result SetKeepAliveExtensionMgrA(IN ISipKeepAliveExtensionMgr* pMgr);

    virtual mxt_result EnableOutboundCrlfKeepAliveA(IN bool bEnable);

#endif //#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
    virtual mxt_result SetNetworkReceptionControlMgrA(IN ISipNetworkReceptionControlMgr* pMgr);

    virtual mxt_result SetPeerFloodBlockingA(IN uint32_t uRawPacketCountLimit,
                                             IN uint32_t uReceptionWindowSizeMs,
                                             IN uint32_t uBlockingTimeoutMs);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
    virtual mxt_result SetPacketReceptionControlMgrA(IN ISipPacketReceptionControlMgr* pMgr);
    virtual mxt_result EnablePeerFilteringA(IN bool bEnable);
    virtual mxt_result AddApprovedPeerA(IN const CSocketAddr& rPeerAddr);
    virtual mxt_result RemoveApprovedPeerA(IN const CSocketAddr& rPeerAddr);
    virtual mxt_result RemoveAllApprovedPeersA();
    virtual void GetApprovedPeerList(OUT CVector<CSocketAddr>& rvecApprovedPeers);
    virtual mxt_result AddAllowedMethodA(IN const char* pszMethod);
    virtual mxt_result RemoveAllowedMethodA(IN const char* pszMethod);
    virtual mxt_result RemoveAllAllowedMethodsA();
    virtual void GetAllowedMethodList(OUT CVector<CString>& rvecstrAllowedMethods);
#endif

    virtual mxt_result SetSocketClosureType(IN ISocket::ECloseBehavior eCloseBehavior);

    virtual mxt_result CloseAllConnections(IN CSocketAddr* pRemoteAddress);

// BRCM_CUSTOM - [add] Configure P-Access-Network-Info header
    virtual void SetPaniHdr(IN const CString& rstrPaniHdr);
// BRCM_CUSTOM - [end] Configure P-Access-Network-Info header

    //-- << Transaction Configuration >>
    //-----------------------------------

    virtual mxt_result SetT1(IN unsigned int uT1Ms);

    virtual mxt_result SetT2(IN unsigned int uT2Ms);

    virtual mxt_result SetT4(IN unsigned int uT4Ms);

    virtual mxt_result SetTimeoutTimer(IN unsigned int uTimeoutTimerMs);

    virtual mxt_result SetTimerB(IN unsigned int uTimerMs);

    virtual mxt_result SetTimerD(IN unsigned int uTimerMs);

    virtual mxt_result SetTimerF(IN unsigned int uTimerMs);

    virtual mxt_result SetTimerH(IN unsigned int uTimerMs);

    virtual mxt_result SetTimerJ(IN unsigned int uTimerMs);

// BRCM_CUSTOM - [begin] Control branch-id matching choices in ACK
#ifndef MXD_SIPSTACK_ENABLE_SERVER_INVITE_SAME_BRANCH_ACK
    virtual mxt_result SetBypassAckBranchIdCheck(IN bool bBypass);
#endif
// BRCM_CUSTOM - [end] Control branch-id matching choices in ACK

    //-- << Core Configuration >>
    //----------------------------

    virtual mxt_result SetCoreUser(IN ISipCoreUser* pUser);

    //-- << ENUM Configuration >>
    //----------------------------

#if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
    virtual void SetEnumE164ZoneSuffix(IN const CString& rstrE164ZoneSuffix);
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)

    //-- << Access to Specific Stack Modules >>
    //------------------------------------------

    virtual mxt_result AddTransportObserverA(
        IN ISipTransportObserver*                           pObserver,
        IN ISipTransportObserver::EInsertObserverPriority   ePriority =
            ISipTransportObserver::eLOW_PRIORITY_OBSERVER);

    virtual mxt_result RemoveTransportObserverA(
        IN ISipTransportObserver*                           pObserver);

#ifdef MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT

    virtual CSipPersistentConnectionList* GetPersistentConnectionList();

#endif // #ifdef MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT

    virtual void SetDefaultDialogMatcherList(
                            IN CSipDialogMatcherList& rDialogMatcherList);

#ifdef MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT
    virtual void SetConnectionBlacklistInstance(TO CSipConnectionBlacklist* pBlacklist);
    virtual CSipConnectionBlacklist* GetConnectionBlacklistInstance();
#endif // #ifdef MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT

#if defined(MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT)
    // Summary:
    //  Returns an instance of the Target Management Module.
    virtual CTargetManagementModule* GetTargetManagementModuleInstance();
#endif


#ifdef MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT
    virtual CSipUaHelpers* GetUaHelpers() const;
#endif

#ifdef MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT
    virtual CSipStackMonitor* GetSipStackMonitor() const;
#endif

    virtual const char* GetVersion();

    //-- << Startup and Shutdown >>
    //------------------------------

    virtual mxt_result Startup();

    virtual void ShutdownA();

    //-- << Network accessibility >>
    //-------------------------------

    virtual mxt_result AddLocalAddress(
                    IN    const CSocketAddr&                 rAddr,
                    IN TO const CVector<CString>*            pvecStrFqdn,
                    IN TO const CVector<SAccessibleNetwork>* pvecDestinations,
                    OUT   mxt_opaque&                        ropqAddress);

    virtual mxt_result RemoveLocalAddress(IN mxt_opaque opqAddress);

    virtual mxt_result ListenA(IN  mxt_opaque    opqAddress,
                               IN  uint16_t      uPort,
                               IN  ESipTransport eTransport,
                               IN  ISipCoreUser* pCoreUser,
                               IN  mxt_opaque    opqCallBackParam,
                               OUT mxt_opaque&   ropqListen);

    virtual mxt_result StopListeningA(IN mxt_opaque    opqListen,
                                      IN ISipCoreUser* pCoreUser,
                                      IN mxt_opaque    opqCallBackParam);

    virtual mxt_result GetNetworkInterfaceList(OUT CVector<SNetworkIf>& rvecstNetworkIf);

    virtual const CSipNetworkInterfaceList* GetNetworkInterfaceListInstance();

    virtual void SetViaAddressType(IN EAddressTypePreference ePreference);

    virtual mxt_result SetEntityId(IN mxt_opaque opqListen,
                                   IN unsigned int uEntityId);

    //-- << Core Service Configuration >>
    //------------------------------------

#if defined(MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT)
    virtual mxt_result AddSupportedExtension(IN const CString& rstrExtension);
    virtual mxt_result RemoveSupportedExtension(IN const CString& rstrExtension);
    virtual void GetSupportedExtensions(OUT const CList<CString>*& rplstExtension) const;
#endif

    //-- << User Agent Services Configuration >>
    //------------------------------------
#if defined MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT
    virtual mxt_result SetDefaultRegistrationContactMatchingType(IN unsigned int uContactMatchingType);
#endif
#if defined MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT
    virtual mxt_result SetDefaultCallIdLength(IN unsigned int uCallIdlength);
    virtual unsigned int GetDefaultCallIdLength();
#endif

    //-- << ISipDigestClientAuthSvc Configuration >>
    //-----------------------------------------------
#if defined(MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT)
    virtual void SetClientAuthUnknownQopBehavior(IN ISipDigestClientAuthSvc::EUnknownQopBehavior eBehavior);
#endif

    //-- << Request Handling Configuration >>
    //----------------------------------------

    virtual void SetServiceUnavailableHandlingMode(IN EServiceUnavailableHandling eServiceUnavailableHandlingMode);
    virtual EServiceUnavailableHandling GetServiceUnavailableHandlingMode() const;

    //-- << IMessageServiceMgr >>
    //----------------------------

    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    //-- << ISipTransportUser >>
    //---------------------------

    virtual void EvCommandResult(IN mxt_result res,
                                 IN mxt_opaque opq);

    //-- << ITimerServiceMgr >>
    //--------------------------
    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimerId,
                                         IN mxt_opaque opq);

private:
    //-- << Startup mechanism >>
    //---------------------------

    static mxt_result InitializeCSipCoreConfig();

    static void FinalizeCSipCoreConfig();

    //-- << ECOM mechanism >>
    //------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipCoreConfig(IN const CSipCoreConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipCoreConfig& operator=(IN const CSipCoreConfig& rSrc);


//-- Hidden Data Members.
//------------------------
protected:

    enum
    {
        eNETWORK_INTERFACELIST_SHUTDOWN,
        eTRANSPORTMGR_SHUTDOWN,
        eTRANSACTIONMGR_SHUTDOWN,
        eSIPCORECONFIG_SHUTDOWN
};

    static CSipTransportMgr* ms_pTransportMgr;

    static CSipTransactionMgr* ms_pTransactionMgr;

    static CSipPersistentConnectionList* ms_pPersistentConnectionList;

    static CSipEntity* ms_pEntity;

    static CSipNetworkInterfaceList* ms_pNetworkInterfaceList;

    static bool ms_bAddStackVersion;

    static CString* ms_pstrUserAgentId;

    static CString* ms_pstrServerId;

    static CSipDialogMatcherList* ms_pDefaultDialogMatcherList;

    static ISipCoreConfig::EAddressTypePreference ms_eAddressTypePreference;

#if defined(MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT)
    static CTargetManagementModule* ms_pTargetManagementModule;
#endif

#ifdef MXD_SIPSTACK_ENABLE_DEFAULT_DATA_LOGGER
    static CSipDefaultDataLogger* ms_pDefaultDataLogger;
#endif
    static ISipDataLogger* ms_pCurrentDataLogger;

    static bool ms_b64bitsSequenceNumberSupported;

#ifdef MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT
    static CSipConnectionBlacklist* ms_pConnectionBlacklist;
#endif

#ifdef MXD_SIPSTACK_ENABLE_TLS
    static ISipTlsContextFactory* ms_pTlsContextFactory;
#endif

    static const unsigned int ms_uM5T_BANNER_LENGTH;
    static char* ms_pszM5t_banner;

#ifdef MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT
    static CSipUaHelpers* ms_pUaHelpers;
#endif

#ifdef MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT
    static CSipStackMonitor* ms_pStackMonitor;
#endif

#if defined (MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
    static CVector<ISipSwitchbackPersistentConnection*>* ms_pvecpSwitchback;
#endif
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
inline const char* CSipCoreConfig::GetVersion()
{
    return szVERSION_SIPSTACK;
}

inline mxt_result CSipCoreConfig::PostSyncMessage( IN IEComUnknown* pThread,
                                                   IN EMessage eMessage,
                                                   IN TO CMarshaler* pParams )
{
    return PostMessage(pThread, true, eMessage, IN TO pParams);
}

//M5T_INTERNAL_USE_BEGIN
//==============================================================================
//==
//==  GetDefaultDialogMatcherList
//==
//==============================================================================
//
//  Summary:
//      Gets the default dialog matcher list.
//
//  Returns:
//      The defaut dialog matcher list.
//
//      NULL when SetDefaultDialogMatcherList was not called yet or after
//      the SIP-UA's shutdown is completed.
//
//  Description:
//      Returns the default dialog matcher list that is used when creating
//      ISipUserAgentSvc or ISipSessionStatefulProxySvc.
//
//      The dialog matcher list is used to keep a list of active dialogs
//      for the above services.
//
//      Care must be taken on the actions done on the returned
//      CSipDialogMatcherList as it is an object used by the SIP SDK to do
//      dialog matching. The object should only be consulted and not changed to
//      avoid assertions or any other failure.
//
//      NOTE: The application is responsible to create and release the
//      CSipDialogMatcherList. The created CSipDialogMatcherList is set to the
//      SIP SDK by using ISipCoreUser::SetDefaultDialogMatcherList.
//
//  See Also:
//      SetDefaultDialogMatcherList, CSipDialogMatcherList,
//      ISipUserAgentSvc, ISipSessionStatefulProxySvc
//
//==============================================================================
inline CSipDialogMatcherList* CSipCoreConfig::GetDefaultDialogMatcherList()
{
    return ms_pDefaultDialogMatcherList;
}
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//==
//==  GetCurrentSipDataLogger
//==
//==============================================================================
//
//  Returns:
//      A pointer to the current ISipDataLogger instance. It is NULL when no
//      data logger is set.
//
//  Description:
//      Returns the current ISipDataLogger instance.
//
// See Also:
//      ISipCoreConfig::SetSipDataLogger@IN ISipDataLogger* pDataLogger
//
//==EDOC========================================================================
inline ISipDataLogger* CSipCoreConfig::GetCurrentSipDataLogger()
{
    return ms_pCurrentDataLogger;
}

//==SDOC========================================================================
//==
//==  GetCSeq64BitsSupport
//==
//==============================================================================
//
//  Returns:
//      True if a 64bits sequence number is supported. False for a 32 bits
//      sequence number.
//
//  Description:
//      Returns if a 64bits sequence number is supported.
//
//==EDOC========================================================================
inline bool CSipCoreConfig::GetCSeq64BitsSupport()
{
    return ms_b64bitsSequenceNumberSupported;
}

#ifdef MXD_SIPSTACK_ENABLE_TLS
//==SDOC========================================================================
//==
//==  ReleaseTlsContextFactory
//==
//==============================================================================
// <FLAG New in 4.1.4>
//
//  Description:
//      Releases the TLS context factory if the pointer is not NULL.
//
//      This method must not be used by the application. It is for internal use
//      only. Must be used only when ShutdownA is not called on the core config
//      to avoid a context factory leak.
//
// See Also:
//      CSipCoreConfig::GetTlsContextFactory()
//
//==EDOC========================================================================
inline void CSipCoreConfig::ReleaseTlsContextFactory()
{
    if (ms_pTlsContextFactory != NULL)
    {
        ms_pTlsContextFactory->ReleaseIfRef();
        ms_pTlsContextFactory = NULL;
    }
}
#endif //MXD_SIPSTACK_ENABLE_TLS

#ifdef MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT
inline CSipStackMonitor* CSipCoreConfig::GetSipStackMonitor() const
{
    return ms_pStackMonitor;
}
#endif // #ifdef MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCORESERVICE_H
