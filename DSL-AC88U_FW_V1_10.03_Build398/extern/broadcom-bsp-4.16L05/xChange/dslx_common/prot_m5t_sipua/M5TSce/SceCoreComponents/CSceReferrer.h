//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CSCEREFERRER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEREFERRER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCECORECOMPONENTSCFG_H
#include "Config/SceCoreComponentsCfg.h"
#endif


#if !defined(MXD_SCE_ENABLE_REFERRER_COMPONENT_SUPPORT)
#error You must define MXD_SCE_ENABLE_REFERRER_COMPONENT_SUPPORT to be able to \
use this class.
#endif


#ifndef MXG_CSCEBASECOMPONENT_H
#include "SceCoreComponents/CSceBaseComponent.h"
#endif

#ifndef MXG_ISCEREFERRER_H
#include "SceCoreComponents/ISceReferrer.h"
#endif

#ifndef MXG_ISIPREFEREEMGR_H
#include "SipUserAgent/ISipRefereeMgr.h"
#endif

#ifndef MXG_ISIPREFERRERMGR_H
#include "SipUserAgent/ISipReferrerMgr.h"
#endif

#ifndef MXG_CSIPSTATUSLINE_H
#include "SipParser/CSipStatusLine.h"
#endif

#ifndef MXG_ISIPNOTIFIERSVC_H
#include "SipUserAgent/ISipNotifierSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class ISceReferrerMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceReferrer
//========================================
//
// Description:
//   This class implements the functionnality of ISceReferrer. It is used to
//   manage a client or a server REFER process.
//
//   It notifies its configured ISceReferrerMgr about the REFER progress.
//
// Location:
//   SceCoreComponents/CSceReferrer.h
//
// See Also:
//   ISceReferrer
//
//==============================================================================
class CSceReferrer : private CSceBaseComponent,
                     private ISceReferrer,
                     private ISipRefereeMgr,
                     private ISipReferrerMgr
{
//-- Friend Declarations.
    friend class CSceCoreComponentsInitializer;

//-- New types within class namespace.
public:
protected:
private:

    enum EShutdownEvent
    {
        // No event.
        eEV_NO_EVENT,

        // Initiates the shutdown process.
        eEV_START_SHUTDOWN,

        // All the pending NOTIFY transactions have finished.
        eEV_ALL_TRANSACTIONS_HAVE_ENDED,

        // The transaction for the final NOTIFY has finished.
        eEV_FINAL_NOTIFY_TRANSACTION_ENDED,

        // Execute next MSF transition automatically.
        eEV_CONTINUE
    };
    //-- << Internal data types >>


//-- Published Interface.
public:


//-- Hidden Methods.
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceReferrer(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceReferrer();


private:

    //-- << Initialization mechanism >>
    static mxt_result InitializeCSceReferrer();

    static void FinalizeCSceReferrer();


    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << ISceReferrer >>
    virtual mxt_result SetManager(IN ISceReferrerMgr* pMgr);

    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);

    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);

    virtual void SetOpaque(IN mxt_opaque opq);

    virtual mxt_opaque GetOpaque();

    virtual void SetInvalidTargetDialogReferBehaviour(IN EInvalidReferBehaviour eBehaviour);

    virtual mxt_result Refer(IN IEComUnknown* pTargetDialog,
                             IN IEComUnknown* pReplaceDialog,
                             IN bool bEarlyOnly,
                             IN TOA CSceCallerPreferences* pCallerPreferences);

    virtual mxt_result Refer(IN IEComUnknown* pTargetDialog,
                             IN const CNameAddr& rDestinationAddr,
                             IN TOA CSceCallerPreferences* pCallerPreferences);

    virtual mxt_result AcceptRefer();

    virtual mxt_result RejectRefer();

    virtual mxt_result NotifyReferProgress(IN uint16_t uStatusCode,
                                           IN bool bFinal);

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void TerminateImmediately();
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    virtual mxt_result HandleIncomingRefer(IN const CSipPacket& rRequest);

    virtual mxt_result InternalTerminate();


    //-- << ISipRefereeMgr >>
    virtual void EvReferred(IN ISipRefereeSvc* pSvc,
                            IN ISipServerEventControl* pServerEventControl,
                            IN mxt_opaque opqReferId,
                            IN const CNameAddr& rReferToAddr,
                            IN const CSipPacket& rRefer);

    virtual void EvRefreshed(IN ISipRefereeSvc* pSvc,
                             IN ISipServerEventControl* pServerEventControl,
                             IN mxt_opaque opqReferId,
                             IN unsigned int uExpirationSec,
                             IN const CSipPacket& rSubscribe);

    virtual void EvTerminated(IN ISipRefereeSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN mxt_opaque opqReferId,
                              IN const CSipPacket& rSubscribe);

    virtual void EvInvalidRequest(IN ISipRefereeSvc* pSvc,
                                  IN mxt_opaque opqApplicationData,
                                  IN const CSipPacket& rRequest,
                                  IN mxt_result reason);

    virtual void EvProgress(IN ISipRefereeSvc* pSvc,
                            IN ISipClientEventControl* pClientEventControl,
                            IN mxt_opaque opqReferId,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipRefereeSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN mxt_opaque opqReferId,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipRefereeSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN mxt_opaque opqReferId,
                           IN const CSipPacket& rResponse);

    virtual void EvExpired(IN ISipRefereeSvc* pSvc,
                           IN mxt_opaque opqReferId);


    //-- << ISipReferrerMgr >>
    virtual void EvReferProgress(IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse);

    virtual void EvReferSuccess(IN ISipReferrerSvc* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN mxt_opaque opqReferId,
                                IN const CSipPacket& rResponse);

    virtual void EvReferFailure(IN ISipReferrerSvc* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN mxt_opaque opqReferId,
                                IN const CSipPacket& rResponse);

    virtual void EvSubscribeProgress(
                                 IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse);

    virtual void EvSubscribeSuccess(
                                 IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse);

    virtual void EvIntervalTooSmall(IN ISipReferrerSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventControl,
                                    IN unsigned int uMinExpirationSec,
                                    IN mxt_opaque opqReferId,
                                    IN const CSipPacket& rResponse);

    virtual void EvSubscribeFailure(IN ISipReferrerSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventControl,
                                    IN mxt_opaque opqReferId,
                                    IN const CSipPacket& rResponse);

    virtual void EvReferStatus(IN ISipReferrerSvc* pSvc,
                               IN ISipServerEventControl* pServerEventControl,
                               IN mxt_opaque opqReferId,
                               IN EState eState,
                               IN const CSipStatusLine& rStatus,
                               IN const CHeaderList* pContentHeaders,
                               IN const CSipPacket& rNotify);

    virtual void EvInvalidNotify(IN ISipReferrerSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rNotify,
                                 IN mxt_result reason);

    virtual void EvExpiring(IN ISipReferrerSvc* pSvc,
                            IN mxt_opaque opqReferId);

    virtual void EvExpired(IN ISipReferrerSvc* pSvc,
                           IN mxt_opaque opqReferId);

    //-- << CSceBaseComponent >>
    virtual uint32_t GetSupportedServicesBitset() const;

    virtual void OnUserAgentServiceConfigured(IN mxt_result res,
                                              IN TO CTelUri* pTelUri,
                                              IN TO CList<CSipUri>* plstSipUris);

    virtual void OnInitialServerTransactionError();

    virtual void OnUaCapabilitiesChanged(IN const CSceSipFeatureSet* pNewCapabilities);
    virtual void OnUserEventTerminateUser();
// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void OnUserEventTerminateUserSilent();
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    //-- << Helper methods >>

    // Summary:
    //  Initiates or continue the shutdown process on the component.
// BRCM_CUSTOM - [mod] Add Timer F expiration handling support per IMS
    mxt_result Shutdown(EShutdownEvent eEvent, IN bool bSilent = false);
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    // Summary:
    //  Shutdown steps.
    mxt_result CallEvTerminated();
    mxt_result SendFinalNotify();

    // Summary:
    //  Creates the ISipContext for this component.
    mxt_result CreateContext();

    // Summary:
    //  Configures the services this component directly manage.
    mxt_result ConfigureServices();

    // Summary:
    //  Fetches the dialog descriptor from a component.
    mxt_result GetDialogDescriptors(IN IEComUnknown* pComponent,
                                    OUT const CToken*& rpCallId,
                                    OUT const CString*& rpstrLocalTag,
                                    OUT const CString*& rpstrRemoteTag,
                                    INOUT CNameAddr* pReferToNameAddr = NULL);

    // Summary:
    //  Tells if the component is being terminated.
    bool ComponentIsTerminating();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceReferrer(IN const CSceReferrer& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceReferrer& operator=(IN const CSceReferrer& rSrc);


//-- Hidden Data Members
protected:
private:

    // The manager to which we will report refer related events.
    ISceReferrerMgr* m_pMgr;

    // The component has received a valid REFER.
    bool m_bReferServer;

    // This flag indicates that there is an active subscription.
    // During the shutdown, it allows to know if it is required to
    // send a final NOTIFY.
    bool m_bActiveSubscription;

    // This structure keeps data needed to send a REFER request..
    struct SReferData
    {
        // The extra headers to add to the request.
        CHeaderList* m_pExtraHeaders;

        // The value of the Refer-To header.
        CNameAddr* m_pReferTo;

        // The caller preferences to add to the request.
        CSceCallerPreferences* m_pCallerPreferences;

        SReferData()
        :   m_pExtraHeaders(NULL),
            m_pReferTo(NULL),
            m_pCallerPreferences(NULL)
        {}

        ~SReferData();
    };

    // This is the data that is kept while the ISipUserAgentSvc is configured
    // when acting as the referrer.
    SReferData* m_pstReferData;

    // This is the ID of the REFER handled as a referee.
    mxt_opaque m_opqRefereeReferId;

    struct SShutdownData
    {
        // The status-line to send in the final NOTIFY.
        CSipStatusLine m_statusLine;

        SShutdownData()
        :   m_statusLine()
        {
        }

        ~SShutdownData()
        {
        }
    };

    SShutdownData* m_pstShutdownData;

    enum EShutdownState
    {
        // The component is running and shutdown has not been initiated.
        eSTATE_NOT_TERMINATING,

        // Shutdown has started.
        // The component is waiting for final response(s) to on-going NOTIFY(ies).
        eSTATE_WAITING_NOTIFY_RESPONSES,

        // Shutdown has started.
        // The component is waiting for the resonse to the final NOTIFY.
        eSTATE_WAITING_FINAL_NOTIFY_RESPONSE,

        // Tells the manager that the component has been terminated.
        eSTATE_MGR_NOTIFY,

        // Shutdown has been completed and the manager has been notified.
        eSTATE_TERMINATED
    };

    // The current shutdown state.
    EShutdownState m_eShutdownState;

    // It is possible that we have more that one NOTIFY request sent that were
    // not yet answered. Moreover, it is possible that the responses are
    // received out of order. Thus we simply keep the number of pending NOTIFY
    // to terminate only when we have no more pending NOTIFY.
    // Note that it is currently impossible to have more than one NOTIFY
    // request at a time but this member is kept for future use (when this is
    // permitted by the SIP-UA).
    unsigned int m_uNumPendingNotify;

    // The behaviour of this component when a REFER request with an invalid or
    // no Target-Dialog header is received.
    EInvalidReferBehaviour m_eBehaviour;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  ComponentIsTerminating
//==
//==============================================================================
//
//  Returns:
//      true: if the componenet is being terminated.
//      false: otherwise.
//
//  Description:
//      Tells if this component is terminating.
//
//==============================================================================
inline bool CSceReferrer::ComponentIsTerminating()
{
    return (m_eShutdownState != eSTATE_NOT_TERMINATING);
}

//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================

inline void CSceReferrer::SetInvalidTargetDialogReferBehaviour(IN EInvalidReferBehaviour eBehaviour)
{
    m_eBehaviour = eBehaviour;
}

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCEREFERRER_H
