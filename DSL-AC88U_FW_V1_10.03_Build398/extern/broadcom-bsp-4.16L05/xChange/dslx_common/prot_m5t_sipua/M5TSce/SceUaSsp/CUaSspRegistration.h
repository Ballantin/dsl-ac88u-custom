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
#ifndef MXG_CUASSPREGISTRATION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUASSPREGISTRATION_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CSCEBASECOMPONENT_H
#include "SceCoreComponents/CSceBaseComponent.h"
#endif

#ifndef MXG_ISCEUSER_H
#include "SceCore/ISceUser.h"
#endif

#ifndef MXG_ISCESUBSCRIBERMGR_H
#include "SceCoreComponents/ISceSubscriberMgr.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CREGINFO_H
#include "SipParser/CReginfo.h"
#endif

#ifndef MXG_ISIPREGISTRATIONMGR_H
#include "SipUserAgent/ISipRegistrationMgr.h"
#endif

#ifndef MXG_IUASSPREGISTRATION_H
#include "SceUaSsp/IUaSspRegistration.h"
#endif

#ifndef MXG_IUASSPREGISTRATIONCONFIG_H
#include "SceUaSsp/IUaSspRegistrationConfig.h"
#endif

#ifndef MXG_IUASSPREGISTRATIONMGR_H
#include "SceUaSsp/IUaSspRegistrationMgr.h" // IUaSspRegistrationMgr::EStatus
#endif

#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" //-- ESipTransport
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CSceSipFeatureSet;
class CSipMessageBody;
class CToken;
class ISceSubscriber;
class ISipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaSspRegistration
//========================================
//
// Description:
//   This class implements the registration handling. It can also be configured
//   to subscribes to the reg info events as per RFC 3680. It tries to re-register
//   every time a registration is about to expire. If a previous registration
//   has failed, it tries to send a periodic registration at an interval that is
//   configurable by the application. When unregistering, it reports the
//   transaction termination on a shutdown callback.
//
// Location:
//   UaSsp/CUaSspRegistration.h
//
//==============================================================================
class CUaSspRegistration : private CSceBaseComponent,
                           private IUaSspRegistration,
                           private ISipRegistrationMgr,
                           private ISceUser,
                           private ISceSubscriberMgr
{
//-- Friend Declarations.
    friend class CSceUaSspInitializer;

//-- New types within class namespace.
public:
protected:
private:

//-- Published Interface.
public:

//-- Hidden Methods.
protected:
private:
    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaSspRegistration();

    static void FinalizeCUaSspRegistration();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaSspRegistration(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CUaSspRegistration();

    //-- << CSceBaseComponent >>
    virtual uint32_t GetSupportedServicesBitset() const;

    virtual void OnEvConnectionEstablished(IN ISceUserConfig* pUserConfig);

    virtual void OnEvErrorOnConnection(IN ISceUserConfig* pUserConfig);

    virtual void OnEvConnectionTerminated(IN ISceUserConfig* pUserConfig);

    virtual void OnUaCapabilitiesChanged(IN const CSceSipFeatureSet* pNewCapabilities);

    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << IUaSspRegistration >>
// BRCM_CUSTOM - [begin]
    virtual void SetGruuEnable(IN bool bEnableGruu);
// BRCM_CUSTOM - [end]

    virtual mxt_result SetManager(IN IUaSspRegistrationMgr* pMgr);

    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);

    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);

    virtual void SetOpaque(IN mxt_opaque opq);

    virtual mxt_opaque GetOpaque();

    virtual mxt_result Register();

    virtual mxt_result Refresh();

    virtual mxt_result Unregister();

    virtual void Terminate();

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void TerminateImmediately();
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    virtual mxt_result QuerySubscriber(IN mxt_iid iidRequested,
                                       OUT void** ppInterface);

// BRCM_CUSTOM - [addd] Expose internal SIP context
    virtual mxt_result RegistrationContext(OUT ISipContext** ppContext);
// BRCM_CUSTOM - [end] Expose internal SIP context

// BRCM_CUSTOM - [add] Add query for registration expiration time
    virtual uint32_t GetRegExpirationTime();
// BRCM_CUSTOM - [end] Add query for registration expiration time

// BRCM_CUSTOM - [add] Add query for re-registration time
    virtual uint32_t GetReRegTime();
// BRCM_CUSTOM - [end] Add query for re-registration time

    //-- << ISipRegistrationMgr >>
    virtual void EvProgress(IN ISipRegistrationSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipRegistrationSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipRegistrationSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvExpiring(IN ISipRegistrationSvc* pSvc,
                            IN TO CSipHeader* pContacts);

    virtual void EvExpired(IN ISipRegistrationSvc* pSvc,
                           IN TO CSipHeader* pContacts);

    //-- << CEventDriven >>
    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    //-- << ISceUser >>
    virtual mxt_result CanHandleRequest(IN const CSipPacket& rRequest,
                                        OUT uint16_t& ruResponseCode,
                                        OUT IEComUnknown*& rpUserConfig);

    //-- << ISceSubscriberMgr >>
    virtual void EvSubscriptionStatus(IN ISceSubscriber* pSubscriber,
                                      IN ESubscriptionStatus eStatus);

    virtual void EvResourceStateUpdate(IN ISceSubscriber* pSubscriber,
                                       IN ISipServerEventControl* pNotifyServerEventCtrl,
                                       IN const CSipMessageBody& rPayload,
                                       IN bool bReceivedFinalNotify = false);

    virtual void EvWatcherInfo(IN ISceSubscriber* pSubscriber,
                               IN const SWatcherInfo& rWatcherInfo,
                               IN bool bFlushWatcherInfoTable);

    virtual void EvWatcherInfoUpdateDone();

    virtual void EvResourceListUpdate(IN const char* pszResourceListUri,
                                      IN CVector<CXmlElement*>& rvpListDisplayName,
                                      IN bool bFullState);

    virtual void EvResourceListUpdateDone();

    virtual void EvResourceListItemUpdate(IN ISceSubscriber* pSubscriber,
                                          IN const SResourceListResourceInfo* pResourceInfo);

    //-- << Helper methods >>

    // Summary:
    //  Creates a ISceSubscriber component.
    mxt_result CreateSubscriber(IN ISceUserConfig* pUserConfig);

    // Summary:
    //  Sends a REGISTER request.
    mxt_result SendRegister();

// BRCM_CUSTOM - [mod] Add external failover support
    // Summary:
    //  Updates the current target of the User-Agent service.
    virtual void UpdateRegistrar();
// BRCM_CUSTOM - [end] Add external failover support

    // Summary:
    //  Updates the remote and local target of the User-Agent service.
    void UpdateUserAgent(IN const CNameAddr& rNameAddr);

    // Summary:
    //  Updates the current service route in the user configuration.
    void UpdateServiceRoute(IN const CSipHeader* pHdrServiceRoute);

    // Summary:
    //  Subscribes to the reginfo events.
    void Subscribe();

    // Summary:
    //  Handles the received NOTIFY which contains reginfo payload.
    void ReginfoEventHelper(IN const CBlob* pMessage);

    // Summary:
    //  Validates the reginfo payload contact (AOR).
    CReginfo::SContact* ValidateContact(IN CReginfo::SRegistration* pstRegistration);

    // Summary:
    //  Handles the contact information.
    void HandleContact(IN CReginfo::SContact* pstContact,
                       IN bool bRegistrationTerminated);

    // Summary:
    //  Handles the expired event.
    void EvExpiredHelper();

    // Summary:
    //  Handles the expiring event.
    void EvExpiringHelper();

    // Summary:
    //  Handles the registration failure event.
    IUaSspRegistrationMgr::EStatus RegistrationFailureHelper();

    // Summary:
    //  Invalidates the GRUUs set so far.
    void InvalidateGruus();

    // Summary:
    //  Tries to match the user, listen addresses or FQDNs and port with the incoming request's URI.
    bool TryRequestMatch(IN ESipTransport eTransport,
                         IN const CSipUri*& rpSipRequestUri,
                         IN ISceUserConfig* pUserConfig);

    // Summary:
    //  Updates the internal registrar index to the next valid index to register to.
    bool UpdateToNextRegistrarIndex(OUT bool& rbWaitbeforeTrying);

    // Summary:
    //  Sets the current registrar status.
    void SetRegistrarStatus(IN IUaSspRegistrationMgr::EStatus eStatus);

    // Summary:
    //  Gets the current registrar status.
    IUaSspRegistrationMgr::EStatus GetRegistrarStatus();

    // Summary:
    //  Sets the specified bitmask in the internal bitset.
    void SetInInternalState(IN uint32_t nBitMask);

    // Summary:
    //  Resets the specified bitmask in the internal bitset.
    void ResetInInternalState(IN uint32_t nBitMask);

    // Summary:
    //  Tells if the specified bitmask is present in the internal bitset.
    bool IsInInternalState(IN uint32_t nBitMask);

    // Sets the expiration time in the expires header.
    void SetExpirationTime(IN uint32_t uExpirationTime);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaSspRegistration(IN const CUaSspRegistration& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaSspRegistration& operator=(IN const CUaSspRegistration& rSrc);


//-- Hidden Data Members.
protected:
private:

    // This structure keeps the internal registrar information.
    struct SRegistrarInfo
    {
        // MXI_DESIGN_NOTES (2009-03-16):
        // The reason why the registrar hostport is copied here is that the configuration
        // one may be changed at any time. Since many internal members depends on a static
        // vector, information must be copied here. This way, there is no possibility to
        // crash the component because the configuration vector changed.

        // The registrar's address.
        CHostPort m_hostPort;

        // The registrar status.
        IUaSspRegistrationMgr::EStatus m_eStatus;
    };

    // This enumeration describes the various registration states that needs
    // to be kept in order to do the right things at the right time.
    enum EInternalState
    {
        // Nothing interesting happening.
        eNONE                               = 0,
        // A client transaction is underway and the response must be received before sending another request.
        eCLIENT_TRANSACTION_UNDERWAY        = 1,
        // The component is terminating and must prevent any user actions.
        eCOMPONENT_TERMINATED               = eCLIENT_TRANSACTION_UNDERWAY * 2,
        // EvExpiring has been called.
        eEVEXPIRING_CALLED                  = eCOMPONENT_TERMINATED * 2,
        // Persistent connections are enabled.
        ePERSISTENT_CONNECTIONS_ENABLED     = eEVEXPIRING_CALLED * 2,
        // A registration is currently active.
        eREGISTRATION_ACTIVE                = ePERSISTENT_CONNECTIONS_ENABLED * 2,
        // Secure mode (SIPS) must be used / is in use.
        eSECURE_MODE                        = eREGISTRATION_ACTIVE * 2,
        // A subscription is currently active.
        eSUBSCRIPTION_ACTIVE                = eSECURE_MODE * 2,
        // Terminate the component as soon as possible.
        eTERMINATE_WHEN_POSSIBLE            = eSUBSCRIPTION_ACTIVE * 2,
        // Tells that a registration response is awaited by the component.
        eWAITING_REGISTRATION_RESPONSE      = eTERMINATE_WHEN_POSSIBLE * 2,
        // Tells that an unregistration response is awaited by the component.
        eWAITING_UNREGISTRATION_RESPONSE    = eWAITING_REGISTRATION_RESPONSE * 2
    };

    // The internal state bitset holder as defined in EInternalState.
    uint32_t m_uInternalStateHolder;

    // The registration configuration used to get the related data when
    // creating the SIP context to register.
    IUaSspRegistrationConfig* m_pRegConfig;

    // The registration manager on which the UA SSP reports events to the
    // application.
    IUaSspRegistrationMgr* m_pMgr;

    // The contacts the registration component need to register.
    CSipHeader* m_pHdrContact;

    // The index of the registrar value in the vector currently being used.
    uint32_t m_uRegistrarIndex;

    // The index of the registrar value in the vector last successfully
    // registered to.
    uint32_t m_uLastRegisteredIndex;

    // Last expiration time which was used, either from configuration
    // or from a 423 response.
    CToken* m_pExpirationTime;

    // Vector of status for each registrar.
    CVector<SRegistrarInfo>* m_pvecRegistrar;

    // The subscriber component if configured to monitor the registration state.
    ISceSubscriber* m_pSubscriber;

    // Current state of the subscription.
    ESubscriptionStatus m_eSubscriptionStatus;

// BRCM_CUSTOM - [begin] Flag showing if GRUU support has been enabled
    bool m_bGruuEnabled;
//
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  SetRegistrarStatus
//==
//==============================================================================
//
//  Parameters:
//      eStatus:
//       The status to set for this registrar.
//
//  Description:
//      Sets the registrar status.
//
//==============================================================================
inline void CUaSspRegistration::SetRegistrarStatus(IN IUaSspRegistrationMgr::EStatus eStatus)
{
    m_pvecRegistrar->GetAt(m_uRegistrarIndex).m_eStatus = eStatus;
}

//==============================================================================
//==
//==  GetRegistrarStatus
//==
//==============================================================================
//
//  Returns:
//      The status of this registrar.
//
//  Description:
//      Gets the registrar status.
//
//==============================================================================
inline IUaSspRegistrationMgr::EStatus CUaSspRegistration::GetRegistrarStatus()
{
    return m_pvecRegistrar->GetAt(m_uRegistrarIndex).m_eStatus;
}

//==============================================================================
//==
//==  SetInInternalState
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to set.
//
//  Description:
//      Sets the specified bitmask in the internal bitset.
//
//==============================================================================
inline void CUaSspRegistration::SetInInternalState(IN uint32_t uBitMask)
{
    m_uInternalStateHolder = m_uInternalStateHolder | uBitMask;
}

//==============================================================================
//==
//==  ResetInInternalState
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to reset.
//
//  Description:
//      Resets the specified bitmask in the internal bitset.
//
//==============================================================================
inline void CUaSspRegistration::ResetInInternalState(IN uint32_t uBitMask)
{
    m_uInternalStateHolder = m_uInternalStateHolder & (~uBitMask);
}

//==============================================================================
//==
//==  IsInInternalState
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to search.
//
//  Returns:
//      true:
//       nBitMask is present in the internal bitset.
//
//      false:
//       nBitMask is not present in the internal bitset.
//
//  Description:
//      Verifies if a bitmask value is present in the internal bitset.
//
//==============================================================================
inline bool CUaSspRegistration::IsInInternalState(IN uint32_t uBitMask)
{
    return (m_uInternalStateHolder & uBitMask) == uBitMask;
}


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CUASSPREGISTRATION_H

