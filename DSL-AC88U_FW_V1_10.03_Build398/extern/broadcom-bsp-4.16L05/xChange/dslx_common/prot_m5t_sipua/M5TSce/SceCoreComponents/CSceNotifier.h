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
#ifndef MXG_CSCENOTIFIER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCENOTIFIER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSCEBASECOMPONENT_H
#include "SceCoreComponents/CSceBaseComponent.h"
#endif

#ifndef MXG_ISCENOTIFIER_H
#include "SceCoreComponents/ISceNotifier.h"
#endif

#ifndef MXG_ISCENOTIFIERMGR_H
#include "SceCoreComponents/ISceNotifierMgr.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISIPNOTIFIERMGR_H
#include "SipUserAgent/ISipNotifierMgr.h"
#endif

#ifndef MXG_ISIPNOTIFIERSVC_H
#include "SipUserAgent/ISipNotifierSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceNotifier
//========================================
//
// Description:
//   The Notifier can be used to notify user about any event type. This
//   component will automatically be created upon the successful reception
//   of a SUBSCRIBE request and will from this point forward manage the event
//   type the request subscribed to.
//
// Location:
//   SceCoreComponents/CSceNotifier.h
//
// See Also:
//   ISceNotifier, ISceNotifierMgr
//
//==============================================================================
class CSceNotifier : private CSceBaseComponent,
                     private ISceNotifier,
                     private ISipNotifierMgr
{
//-- Friend Declarations.
    friend class CSceCoreComponentsInitializer;

//-- Hidden Methods.
private:

    //-- << Initialization mechanism >>
    static mxt_result InitializeCSceNotifier();

    static void FinalizeCSceNotifier();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceNotifier(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CSceNotifier();

    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << ISceNotifier >>
    virtual mxt_result SetManager(IN ISceNotifierMgr* pMgr);

    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);

    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);

    virtual void SetOpaque(IN mxt_opaque opq);

    virtual mxt_opaque GetOpaque();

    virtual void ConfigureComponent(IN uint32_t uDefaultExpirationTimeSec = 3600,
                                    IN uint32_t uMinimumExpiresSec = 60,
                                    IN uint32_t uRetryDelaySec = 60,
                                    IN uint32_t uNumberOfRetries = 5);

    virtual mxt_result AcceptSubscription(IN ESubscriptionState eState,
                                          IN uint32_t uExpirationSec);

    virtual mxt_result RejectSubscription(IN uint16_t uResponseCode,
                                          IN TO CHeaderList* pExtraHeaders);

    virtual mxt_result Notify(IN ESubscriptionState eState,
                              IN TO CSipMessageBody* pBody);

    virtual mxt_result TerminateSubscription(IN ETerminatedReason eReason,
                                             IN TO CSipMessageBody* pBody,
                                             IN unsigned int* puRetryAfterSec);

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void TerminateImmediately();
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    virtual void OverrideTracingNodes(IN STraceNode& rNewComponentNode,
                                      IN STraceNode& rNewEComNode);

    virtual mxt_result HandleIncomingSubscribe(IN const CSipPacket& rRequest);

    virtual void TerminateNotifier();

    //-- << ISipNotifierMgr >>
    virtual void EvSubscribed(IN ISipNotifierSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN unsigned int uExpirationSec,
                              IN const CSipPacket& rSubscribe);

    virtual void EvFetched(IN ISipNotifierSvc* pSvc,
                           IN ISipServerEventControl* pServerEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rSubscribe);

    virtual void EvRefreshed(IN ISipNotifierSvc* pSvc,
                             IN ISipServerEventControl* pServerEventControl,
                             IN const CString& rstrEvent,
                             IN const CString& rstrId,
                             IN unsigned int uExpirationSec,
                             IN const CSipPacket& rSubscribe);

    virtual void EvTerminated(IN ISipNotifierSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN const CSipPacket& rSubscribe);

    virtual void EvInvalidSubscribe(IN ISipNotifierSvc* pSvc,
                                    IN mxt_opaque opqApplicationData,
                                    IN const CSipPacket& rSubscribe,
                                    IN mxt_result reason);

    virtual void EvProgress(IN ISipNotifierSvc* pSvc,
                            IN ISipClientEventControl* pClientEventControl,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipNotifierSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipNotifierSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse);

    virtual void EvExpired(IN ISipNotifierSvc* pSvc,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId);


    //-- << CEventDriven >>

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    // << CSceBaseComponent >>
    virtual void OnUaCapabilitiesChanged(IN const CSceSipFeatureSet* pNewCapabilities);

    virtual uint32_t GetSupportedServicesBitset() const;

    virtual void OnUserEventTerminateUser();

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void OnUserEventTerminateUserSilent();
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    // << Helpers >>
    ISipNotifierSvc::EReason ConvertToSipReason(ETerminatedReason eReason);
    ISipNotifierSvc::EState ConvertToSipState(ESubscriptionState eState);

    void ReleaseContext();

    bool SendNotifyHelper(IN bool bResetRetries);

    void TerminateRetriesHelper(IN bool bResetBody);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceNotifier(IN const CSceNotifier& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceNotifier& operator=(IN const CSceNotifier& rSrc);


//-- Hidden Data Members.
private:
    // The notifier manager.
    ISceNotifierMgr* m_pMgr;

    // The default expiration time to use. Default is 3600 seconds.
    uint32_t m_uDefaultExpirationTimeSec;

    // The default expiration time to use. Default is 60 seconds.
    uint32_t m_uMinimumExpiresSec;

    // The retry delay to use. Default is 60 seconds.
    uint32_t m_uRetryDelaySec;

    // The number of retries to do before failing.  Default is 5 retries.
    uint32_t m_uNumberOfRetries;

    // The number of retries left before failing.
    uint32_t m_uRetriesLeft;

    // Subscription event.
    CString m_strEvent;

    // Subscription Id.
    CString m_strId;

    // The subscription state.
    ESubscriptionState m_eState;

    // The subscription body.
    CSipMessageBody* m_pBody;

    // The ongoing Termination transaction.
    ISipClientTransaction* m_pTerminationTransaction;

    // Temporary ServerEventControl pointer to answer the subscribe request.
    ISipServerEventControl* m_pServerEventControl;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCENOTIFIER_H

