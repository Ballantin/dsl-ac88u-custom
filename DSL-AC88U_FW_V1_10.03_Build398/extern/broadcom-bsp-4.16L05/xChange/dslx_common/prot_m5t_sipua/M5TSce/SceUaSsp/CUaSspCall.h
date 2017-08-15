//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
// Portions of this software Copyright (c) 2007-2012 Broadcom
// 
//==============================================================================

#ifndef MXG_CUASSPCALL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUASSPCALL_H

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

#ifndef MXG_IUASSPCALL_H
#include "SceUaSsp/IUaSspCall.h"
#endif

#ifndef MXG_IMSPOFFERANSWERSESSIONMGR_H
#include "SceMsp/IMspOfferAnswerSessionMgr.h"
#endif

#ifndef MXG_ISIPSESSIONMGR_H
#include "SipUserAgent/ISipSessionMgr.h"
#endif

#ifndef MXG_ISIPRELIABLEPROVISIONALRESPONSEMGR_H
#include "SipUserAgent/ISipReliableProvisionalResponseMgr.h"
#endif

#ifndef MXG_ISIPUPDATEMGR_H
#include "SipUserAgent/ISipUpdateMgr.h"
#endif

#ifndef MXG_ISIPSESSIONTIMERMGR_H
#include "SipUserAgent/ISipSessionTimerMgr.h"
#endif

#ifndef MXG_ISIPTRANSFERMGR07_H
#include "SipUserAgent/ISipTransferMgr07.h"
#endif

#ifndef MXG_ISIPREPLACESMGR_H
#include "SipUserAgent/ISipReplacesMgr.h"
#endif

#ifndef MXG_ISIPGLAREMGR_H
#include "SipUserAgent/ISipGlareMgr.h"
#endif

#ifndef MXG_IMSPOFFERANSWERSESSION_H
#include "SceMsp/IMspOfferAnswerSession.h"
#endif

#ifndef MXG_METHOD_H
#include "SipParser/Method.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSceCallerPreferences;
class IMspOfferAnswerSession;
class IUaSspCallConfig;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaSspCall
//========================================
//
// Description:
//   This class manages SIP call functionnalities and interacts with an
//   IMspOfferAnswerSession.
//
//   It notifies its manager when a significant event happens on the call.
//
// Location:
//   SceUaSsp/CUaSspCall.h
//
// See Also:
//   IUaSspCall
//
//==============================================================================
class CUaSspCall : private CSceBaseComponent,
                   private IUaSspCall,
                   private IMspOfferAnswerSessionMgr,
                   private ISipSessionMgr,
                   private ISipReliableProvisionalResponseMgr,
                   private ISipUpdateMgr,
                   private ISipSessionTimerMgr,
                   private ISipTransferMgr07,
                   private ISipReplacesMgr,
                   private ISipGlareMgr
{
//-- Friend Declarations.
    friend class CSceUaSspInitializer;

//-- New types within class namespace.
public:
protected:
private:

    //-- << Internal data types >>

    // This enum identifies an action that must be done when a request is
    // received. These values are passed through the ISipContext opaque value.
    enum ERequestAction
    {
        // Invalid value. MUST be the first entry.
        eINVALID_ACTION,
        // The request will be rejected with a 500 "Wrong Configuration".
        eREJECT_WRONG_CONFIG
    };


    // This enum identifies an offer/answer state. These are kept into members.
    enum EOfferAnswerState
    {
        // Description:
        //  The media related to the Content-Disposition is disabled.
        eDISABLED_MEDIA,
        // Description:
        //  There is no active offer/answer.
        eIDLE_NEGOTIATION,
        // Description:
        //  The INVITE offer/answer has been completed.
        //  No more offer/answer can be initiated on the INVITE transaction until the end of the transaction.
        eINVITE_NEGOTIATION_COMPLETED,
        // Description:
        //  We asked the IMspOfferAnswerSession for an offer to send into an
        //  INVITE.
        eOFFERER_PREPARING_OFFER_FOR_INVITE,
        // Description:
        //  We asked the IMspOfferAnswerSession for an offer to send into
        //  the next reliable response to INVITE.
        eOFFERER_PREPARING_OFFER_FOR_NEXT_RELIABLE_RESPONSE,

        // Description:
        //  We asked the MSP to prepare an offer that has been sent in the
        //  INVITE and we are now expecting for the answer in a reliable
        //  response (2xx or 1xx if supported).
        eOFFERER_CALL_WAITING_ANSWER_IN_RELIABLE_RESPONSE,
        // Description:
        //  The MSP asked us to send an offer which has been sent in the INVITE
        //  and we are now expecting for the answer in a reliable response (2xx
        //  or 1xx if supported).
        eOFFERER_MSP_WAITING_ANSWER_IN_RELIABLE_RESPONSE,
        // Description:
        //  We asked the MSP to prepare an offer that has been sent in the 2xx
        //  to the INVITE and we are now expecting for the answer in the ACK.
        eOFFERER_CALL_WAITING_ANSWER_IN_ACK,
        // Description:
        //  The MSP asked us to send an offer which has been sent in the 2xx to
        //  the INVITE and we are now expecting for the answer in the ACK.
        eOFFERER_MSP_WAITING_ANSWER_IN_ACK,
        // Description:
        //  We asked the MSP to prepare an offer that has been sent in a
        //  reliable provisional response and we are now expecting for the
        //  answer in the PRACK.
        eOFFERER_CALL_WAITING_ANSWER_IN_PRACK,
        // Description:
        //  The MSP asked us to send an offer which has been sent in a reliable
        //  provisional response and we are now expecting for the answer in the
        //  PRACK.
        eOFFERER_MSP_WAITING_ANSWER_IN_PRACK,
        // Description:
        //  We sent an offer into an UPDATE and we are expecting for the answer
        //  into the UPDATE response.
        eOFFERER_WAITING_ANSWER_IN_2XX_TO_UPDATE,

        // Description:
        //  We sent an empty INVITE so we are waiting for an offer in the next
        //  reliable response.
        eANSWERER_WAITING_OFFER_IN_NEXT_RELIABLE_RESPONSE,

        // Description:
        //  We received an offer in an INVITE and we are waiting for the
        //  IMspOfferAnswerSession to give us an answer to send into a reliable
        //  provisional response.
        eANSWERER_PREPARING_ANSWER_FOR_RELIABLE_RESPONSE,
        // Description:
        //  We received an offer in a 2xx for an INVITE and we are waiting for
        //  the IMspOfferAnswerSession to give us an answer to send into the
        //  ACK.
        eANSWERER_PREPARING_ANSWER_FOR_ACK,
        // Description:
        //  We received an offer in a reliable provisional response for an
        //  INVITE and we are waiting for the IMspOfferAnswerSession to give us
        //  an answer to send into the PRACK.
        eANSWERER_PREPARING_ANSWER_FOR_PRACK,
        // Description:
        //  We received an offer in a PRACK and we are waiting for the
        //  IMspOfferAnswerSession to give us an answer to send into the 2xx to
        //  the PRACK.
        eANSWERER_PREPARING_ANSWER_FOR_2XX_TO_PRACK,
        // Description:
        //  We received an offer in an UPDATE and we are waiting for the
        //  IMspOfferAnswerSession to give us an answer to send into the 2xx to
        //  the UPDATE.
        eANSWERER_PREPARING_ANSWER_FOR_2XX_TO_UPDATE
    };


    // Description:
    //  These enum values are used to give information on a client transaction.
    enum EClientTransactionOpaque
    {
        // Description:
        //  No information.
        eCLIENT_INFO_NONE = 0,
        // Description:
        //  The sent request is a BYE request.
        eCLIENT_INFO_BYE_REQUEST = 1,
        // Description:
        //  The sent request is a final NOTIFY.
        eCLIENT_INFO_FINAL_NOTIFICATION = eCLIENT_INFO_BYE_REQUEST * 2,
        // Description:
        //  The request has been sent to complete a shutdown step. A final
        //  response is needed for the request before the next step is
        //  processed.
        eCLIENT_INFO_REQUEST_FOR_SHUTDOWN = eCLIENT_INFO_FINAL_NOTIFICATION * 2,
        // Description:
        //  The request sent is an original INVITE.
        eCLIENT_INFO_ORIGINAL_INVITE = eCLIENT_INFO_REQUEST_FOR_SHUTDOWN * 2
    };

//-- Published Interface.
public:


//-- Hidden Methods.
protected:


    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaSspCall(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CUaSspCall();

private:


    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaSspCall();

    static void FinalizeCUaSspCall();


    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- << IUaSspCall >>
    virtual mxt_result SetManager(IN IUaSspCallMgr* pMgr);

    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);

    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);

    virtual mxt_result SetMspSession(IN IEComUnknown* pMspSession);

    virtual void GetMspSession(OUT IEComUnknown** ppMspSession);

    virtual void SetOpaque(IN mxt_opaque opq);

    virtual mxt_opaque GetOpaque();

    virtual mxt_result MakeCall(IN const CNameAddr& rPeerAddr,
                                IN TO CSceCallerPreferences* pCallerPreferences);

    virtual void TerminateCall();

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void TerminateImmediately();
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    virtual void Abort();

    virtual mxt_result CallUserNotified();

    virtual mxt_result AcceptCall();

    virtual mxt_result ForwardCall(IN const CNameAddr& rRemoteUri);

    virtual mxt_result RejectCall(IN uint16_t uStatusCode = uBUSY_HERE,
                                  IN const char* pszReasonPhrase = NULL,
                                  IN unsigned int uRetryAfter = 0);

    virtual mxt_result Transfer(IN const CNameAddr& rTransferTarget);

    virtual mxt_result Transfer(IN IUaSspCall* pCallWithTransferTarget);

    virtual mxt_result DeclineTransfer();

    virtual mxt_result AcceptTransfer(IN IUaSspCall* pCreatedCall);

    virtual mxt_result HandleIncomingCall(IN const CSipPacket& rRequest);

    virtual mxt_result HandleForkedResponse(IN const CSipPacket& rResponse,
                                            IN ISipUserAgentSvc* pOriginalUserAgentSvc,
                                            IN IMspOfferAnswerSession* pOriginalMsp,
                                            IN bool bSessionOfferInInvite,
                                            IN bool bEarlySessionOfferInInvite);

    virtual void GetUserAgentSvc(OUT ISipUserAgentSvc*& rpUaSvc);

    virtual mxt_result ReportTransferProgress(IN const CSipStatusLine& rContentStatusLine);

    virtual mxt_result SetTransfereeNotifier(IN IUaSspCall* pTransfereeNotifier);

    virtual mxt_result InternalTerminateCall(IUaSspCallMgr::ETerminationReason eTerminationReason);

    virtual bool IsTerminating();

    virtual bool IsInEarlyDialog();

    virtual mxt_result InternalMakeCall(IN const CNameAddr& rPeerAddr,
                                        IN TO CSceCallerPreferences* pCallerPreferences,
                                        IN TOA CHeaderList* pExtraHeaders = NULL);


    //-- << IMspOfferAnswerSessionMgr >>
    virtual mxt_result EvNewSdpToSend(IN TO CBlob* pSdpSession,
                                      IN TO CBlob* pSdpEarlySession,
                                      IN EMessageTypeForPayload eMessageType,
                                      IN uint32_t uBitSetAdditionalRequiredExtension);

    virtual void EvNegotiationFailure(IN ENegotiationFailureReason eReason,
                                      IN uint16_t uFailureSipStatusCode);

    virtual void EvTerminalFailure();


    //-- << ISipSessionMgr >>
    virtual void EvInvited(IN ISipSessionSvc* pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket& rRequest);

    virtual void EvReInvited(IN ISipSessionSvc* pSvc,
                             IN ISipServerEventControl* pServerEventCtrl,
                             IN const CSipPacket& rRequest);

    virtual void EvAcknowledged(IN ISipSessionSvc* pSvc,
                                IN mxt_opaque opqApplicationData,
                                IN const CSipPacket& rRequest);

    virtual void EvTerminated(IN ISipSessionSvc* pSvc,
                              IN ISipServerEventControl* pServerEventCtrl,
                              IN const CSipPacket& rRequest);

    virtual void EvInviteCancelled(IN ISipSessionSvc* pSvc,
                                   IN ISipServerEventControl* pServerEventCtrl,
                                   IN const CSipPacket& rRequest);

    virtual void EvReInviteCancelled(IN ISipSessionSvc* pSvc,
                                     IN ISipServerEventControl* pServerEventCtrl,
                                     IN const CSipPacket& rRequest);

    virtual void EvInvalidInvite(IN ISipSessionSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rRequest,
                                 IN mxt_result resReason);

    virtual void EvProgress(IN ISipSessionSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipSessionSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipSessionSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvTerminationProgress(IN ISipSessionSvc* pSvc,
                                       IN ISipClientEventControl* pClientEventCtrl,
                                       IN const CSipPacket& rResponse);

    virtual void EvTerminationSuccess(IN ISipSessionSvc* pSvc,
                                      IN ISipClientEventControl* pClientEventCtrl,
                                      IN const CSipPacket& rResponse);

    virtual void EvTerminationFailure(IN ISipSessionSvc* pSvc,
                                      IN ISipClientEventControl* pClientEventCtrl,
                                      IN const CSipPacket& rResponse);

    virtual void EvNewSessionNeededForOriginalInviteResponse(IN ISipSessionSvc* pSvc,
                                                             IN ISipClientEventControl* pClientEventCtrl,
                                                             IN const CSipPacket& rPacket);

    virtual void EvInviteSuccessResponseTimeout(IN ISipSessionSvc* pSvc,
                                                IN mxt_opaque opq,
                                                IN const CSipPacket& rRequest);


    //-- << ISipReliableProvisionalResponseMgr >>
    virtual void EvReliableResponseReceived(IN ISipReliableProvisionalResponseSvc* pSvc,
                                            IN ISipClientEventControl* pClientEventCtrl,
                                            IN const CSipPacket& rPacket);

    virtual void EvResponseAcknowledged(IN ISipReliableProvisionalResponseSvc* pSvc,
                                        IN ISipServerEventControl* pServerEventCtrl,
                                        IN const CSipPacket& rPacket);

    virtual void EvInvalidPrack(IN ISipReliableProvisionalResponseSvc* pSvc,
                                IN mxt_opaque opqApplicationData,
                                IN const CSipPacket& rPacket,
                                IN mxt_result resReason);

    virtual void EvReliableProvisionalResponseTimeout(IN ISipReliableProvisionalResponseSvc* pSvc,
                                                      IN mxt_opaque opqTransaction);

    virtual void EvPrackProgress(IN ISipReliableProvisionalResponseSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventCtrl,
                                 IN const CSipPacket& rPacket);

    virtual void EvPrackSuccess(IN ISipReliableProvisionalResponseSvc* pSvc,
                                IN ISipClientEventControl* pClientEventCtrl,
                                IN const CSipPacket& rPacket);

    virtual void EvPrackFailure(IN ISipReliableProvisionalResponseSvc* pSvc,
                                IN ISipClientEventControl* pClientEventCtrl,
                                IN const CSipPacket& rPacket);


    //-- << ISipUpdateMgr >>
    virtual void EvUpdated(IN ISipUpdateSvc* pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket& rUpdateRequest);

    virtual void EvInvalidUpdate(IN ISipUpdateSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rRequest,
                                 IN mxt_result resReason);

    virtual void EvProgress(IN ISipUpdateSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipUpdateSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipUpdateSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);


    //-- << ISipSessionTimerMgr >>
    virtual void EvSessionIntervalTooShortSent(IN ISipSessionTimerSvc* pSvc,
                                               IN mxt_opaque opqApplicationData,
                                               IN const CSipPacket& rRequest);

    virtual void EvSessionIntervalTooShortRecv(IN ISipSessionTimerSvc* pSvc,
                                               IN ISipClientEventControl* pClientEventCtrl,
                                               IN const CSipPacket& rRequest);

    virtual void EvSessionMustRefresh(IN ISipSessionTimerSvc* pSvc);

    virtual void EvSessionExpired(IN ISipSessionTimerSvc* pSvc);


    //-- << ISipTransferMgr07 >>
    virtual void EvReferProgress(IN ISipTransferSvc07* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN const CSipPacket& rResponse);

    virtual void EvReferSuccess(IN ISipTransferSvc07* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN const CSipPacket& rResponse);

    virtual void EvReferFailure(IN ISipTransferSvc07* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN const CSipPacket& rResponse);

    virtual void EvProgressReport(IN ISipTransferSvc07* pSvc,
                                  IN ISipServerEventControl* pServerEventControl,
                                  IN ISipReferrerMgr::EState eState,
                                  IN const CSipStatusLine& rStatus,
                                  IN const CHeaderList* pContentHeaders,
                                  IN const CSipPacket& rNotifyRequest);

    virtual void EvFinalReport(IN ISipTransferSvc07* pSvc,
                               IN ISipServerEventControl* pServerEventControl,
                               IN const CSipStatusLine& rStatus,
                               IN const CHeaderList* pContentHeaders,
                               IN const CSipPacket& rNotifyRequest);

    virtual void EvReportingExpired(IN ISipTransferSvc07* pSvc);

    virtual void EvTransferred(IN ISipTransferSvc07* pSvc,
                               IN ISipServerEventControl* pServerEventControl,
                               IN const CNameAddr& rTarget,
                               IN const CSipPacket& rReferRequest);

    virtual void EvNotifyProgress(IN ISipTransferSvc07* pSvc,
                                  IN ISipClientEventControl* pClientEventControl,
                                  IN const CSipPacket& rResponse);

    virtual void EvNotifySuccess(IN ISipTransferSvc07* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN const CSipPacket& rResponse);

    virtual void EvNotifyFailure(IN ISipTransferSvc07* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN const CSipPacket& rResponse);

    virtual void EvFinalStatusRequired(IN ISipTransferSvc07* pSvc,
                                       IN const CSipPacket* pSubscribe);

    virtual void EvInvalidRequest(IN ISipTransferSvc07* pSvc,
                                  IN mxt_opaque opqApplicationData,
                                  IN const CSipPacket& rRequest,
                                  IN mxt_result resError);


    //-- << ISipReplacesMgr >>
    virtual void EvReplaces(IN ISipReplacesSvc* pSvc,
                            IN ISipUserAgentSvc* pReplacedUaSvc,
                            IN bool bEarlyOnly,
                            IN const CSipPacket& rRequest,
                            INOUT mxt_opaque& rApplicationData);

    virtual void EvInvalidReplaces(IN ISipReplacesSvc* pSvc,
                                   IN mxt_opaque opqApplicationData,
                                   IN const CSipPacket& rRequest,
                                   IN mxt_result res);


    //-- << ISipGlareMgr >>
    virtual void EvReadyToRetry(IN ISipGlareSvc* pGlareSvc,
                                IN bool bOtherRequestSeenSinceGlare);

    //-- << CSceBaseComponent >>
    virtual uint32_t GetSupportedServicesBitset() const;

    virtual void ReIssueRequestHelper(IN ISipClientEventControl* pClientEventCtrl);

    virtual void EvRequest(IN ISipGenericSvc* pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket& rRequest);

    virtual void TransactionChanged(IN ISipClientTransaction* pOldTransaction,
                                    IN ISipClientTransaction* pNewTransaction);

    virtual void EvRedirected(IN ISipRedirectionSvc* pSvc,
                              IN ISipClientEventControl* pClientEventCtrl,
                              IN const CSipPacket& rPacket);

    virtual void OnUserAgentServiceConfigured(IN mxt_result res,
                                              IN TO CTelUri* pTelUri,
                                              IN TO CList<CSipUri>* plstSipUris);

    virtual void OnInitialServerTransactionError();

    virtual void OnUaCapabilitiesChanged(IN const CSceSipFeatureSet* pNewCapabilities);

    virtual void OnUserEventTerminateUser();

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void OnUserEventTerminateUserSilent();
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS
// BRCM_CUSTOM - [add] override CSceBaseComponent::EvInvalidAssertedIdentity 
    virtual void EvInvalidAssertedIdentity(IN ISipUaAssertedIdentitySvc* pSvc,
                                           IN const CSipPacket& rRequest,
                                           IN mxt_opaque opqApplicationData,
                                           IN mxt_result resReason);
// BRCM_CUSTOM - [end] override CSceBaseComponent::EvInvalidAssertedIdentity 

    //-- << Helper methods >>

    // Summary:
    //  Creates the ISipContext for this instance.
    mxt_result CreateContext(IN ISceUserConfig* pUserConfig);

    // Summary:
    //  Configure the services directly supported by this class.
    mxt_result ConfigureServices(IN IUaSspCallConfig* pCallConfig);

    // Summary:
    //  Rejects the initial INVITE and terminates this object. It verifies if
    //  the state is correct and terminates the call as if it has been
    //  requested by the application.
    mxt_result CallRejectedByApplication(IN unsigned int uStatusCode,
                                         IN const char* pszReasonPhrase,
                                         IN TO CHeaderList* pExtraHeaders,
                                         IN IUaSspCallMgr::ETerminationReason eReason);

    // Summary:
    //  Rejects the initial INVITE and terminates this object.
    void RejectInitialInvite(IN unsigned int uStatusCode,
                             IN const char* pszReasonPhrase,
                             IN TO CHeaderList* pExtraHeaders,
                             IN IUaSspCallMgr::ETerminationReason eReason);

    // Summary:
    //  Starts the shutdown if not already terminating.
    mxt_result StartShutdown(IN IUaSspCallMgr::ETerminationReason eReason);

    // Summary:
    //  Processes the next shutdown step.
    void ExecuteNextShutdownStep();

    // Summary:
    //  Process the shutdown steps.
    mxt_result InitialShutdownStep();
    mxt_result EndMspNegotiations();
    mxt_result PreTerminateMspSession();
    mxt_result RejectPendingRequests();
    mxt_result TerminateOutgoingRequests();
    mxt_result CompleteTransferorTransfer();
    mxt_result EndTransfereeNotification();
    mxt_result WaitForFinalInviteAckIfNeeded();
    mxt_result SendBye();
    mxt_result TerminateMspSession();
    mxt_result CallEvTerminated();

    // Summary:
    //  Makes sure that the negotiations are over.
    void EndMspNegotiationHelper(IN EOfferAnswerState eCurrentState,
                                 OUT EOfferAnswerState& reNextState,
                                 OUT bool& rbNeedToCancelOffers);

    // Summary:
    //  Creates a SDP CSipMessageBody.
    GO CSipMessageBody* CreateMessageBody(IN const char* pszContentTypeMType,
                                          IN const char* pszContentTypeMSubType,
                                          IN const char* pszContentDisposition,
                                          IN TO CBlob* pSdp);

    // Summary:
    //  Adds a body to the CSipMessageBody.
    void AddMessageBody(IN TO CSipMessageBody* pMessageBodyToAdd,
                        INOUT GO CSipMessageBody*& rpContainerBody);

    // Summary:
    //  Gets the next state for a received INVITE.
    mxt_result GetNextStateForInvite(IN const CBlob* pSdp,
                                     IN EOfferAnswerState eCurrentState,
                                     OUT EOfferAnswerState& reNextState,
                                     OUT bool& rbIsInternalGlaring,
                                     OUT IMspOfferAnswerSession::ESdpType& reSdpType,
                                     OUT unsigned int& ruSipErrorCode,
                                     OUT GO CHeaderList*& rpExtraHeaders);

    // Summary:
    //  Gets the next state for a received reliable response for an INVITE.
    mxt_result GetNextStateForInviteReliableResponse(INOUT const CBlob*& rpSdp,
                                                     IN EOfferAnswerState eCurrentState,
                                                     IN EOfferAnswerState eStateToAnswer,
                                                     OUT EOfferAnswerState& reNextState,
                                                     OUT IMspOfferAnswerSession::ESdpType& reSdpType,
                                                     OUT bool& rbReceivedOffer);

    // Summary:
    //  Gets the next state for a received PRACK.
    mxt_result GetNextStateForPrack(IN const CBlob* pSdp,
                                    IN EOfferAnswerState eCurrentState,
                                    OUT EOfferAnswerState& reNextState,
                                    OUT IMspOfferAnswerSession::ESdpType& reSdpType,
                                    OUT bool& rbReceivedOffer);

    // Summary:
    //  Gets the next state for a received UPDATE.
    mxt_result GetNextStateForUpdate(IN const CBlob* pSdp,
                                     IN EOfferAnswerState eCurrentState,
                                     OUT EOfferAnswerState& reNextState,
                                     OUT bool& rbIsInternalGlaring,
                                     OUT IMspOfferAnswerSession::ESdpType& reSdpType,
                                     OUT bool& rbReceivedOffer,
                                     OUT unsigned int& ruSipErrorCode,
                                     OUT GO CHeaderList*& rpExtraHeaders);

    // Summary:
    //  Gets the next state for a received 2xx to UPDATE.
    mxt_result GetNextStateForUpdate2xx(IN const CBlob* pSdp,
                                        IN EOfferAnswerState eCurrentState,
                                        OUT EOfferAnswerState& reNextState,
                                        OUT IMspOfferAnswerSession::ESdpType& reSdpType);

    // Summary:
    //  Gets the packet sdp payload.
    mxt_result GetPacketPayload(IN const CSipPacket& rPacket,
                                OUT const CBlob*& rpSessionSdp,
                                OUT const CBlob** ppEarlySessionSdp);

    // Summary:
    //  Extracts buffered data to send an INVITE response.
    mxt_result UnbufferInviteResponse(IN const uint16_t uInviteResponseCode,
                                      IN const bool bInCall,
                                      INOUT EOfferAnswerState& reNextSessionState,
                                      INOUT EOfferAnswerState& reNextEarlySessionState,
                                      INOUT GO CSipHeader*& rpAdditionalRequire,
                                      OUT GO CSipMessageBody*& rpInviteResponseBody);

    // Summary:
    //  Gets the session message body from a message body.
    GO CSipMessageBody* ExtractSessionMessageBody(IN TOA CSipMessageBody* pMessageBody);

    // Summary:
    //  Gets the next state to have after EvNewSdpToSend is processed and adds
    //  the payload in the CSipMessageBody where it should be.
    mxt_result EvNewSdpToSendHelper(IN EOfferAnswerState eCurrentState,
                                    IN ESipStatusClass eInviteResponseClass,
                                    IN TO CSipMessageBody* pMessageBody,
                                    INOUT GO CSipMessageBody*& rpAckBody,
                                    INOUT GO CSipMessageBody*& rpInviteBody,
                                    INOUT GO CSipMessageBody*& rpPrackBody,
                                    INOUT GO CSipMessageBody*& rpUpdateBody,
                                    INOUT GO CSipMessageBody*& rpInviteResponseBody,
                                    INOUT GO CSipMessageBody*& rpPrackResponseBody,
                                    INOUT GO CSipMessageBody*& rpUpdateResponseBody,
                                    OUT EOfferAnswerState& reNextState);

    // Summary:
    //  Gets the next state when an empty INVITE is about to be sent.
    mxt_result GetNextStateForSentEmptyInvite(IN EOfferAnswerState eCurrentState,
                                              OUT EOfferAnswerState& reNextState);

    // Summary:
    //  Gets the next state when an empty INVITE reliable response (2xx or
    //  reliable 1xx) is about to be sent.
    mxt_result GetNextStateForSentEmptyInviteRelResponse(IN EOfferAnswerState eCurrentState,
                                                         IN bool b2xxResponse,
                                                         OUT EOfferAnswerState& reNextState);

    // Summary:
    //  Rejects an offer coming from a request.
    void EvNegotiationFailureHelper(IN EOfferAnswerState eCurrentState,
                                    IN uint16_t uFailureSipStatusCode,
                                    IN const CHeaderList& rExtraHeaders,
                                    OUT EOfferAnswerState& reNextState,
                                    OUT bool& rbRejectInitialInvite);

    // Summary:
    //  Sets the peer capabilities on the IMspOfferAnswerSession.
    mxt_result SetPeerSipCapabilities(IN const CSipPacket& rPacket,
                                      OUT GO CSipHeader*& rpHdrUnsupported);

    // Summary:
    //  Processes the completion of the transfer procedure when a transfer try
    //  is over.
    void TransferorTransferCompleted(IN IUaSspCallMgr::ETransferReport eTransferResult);

    // Summary:
    //  Sends a NOTIFY for transfer progress.
    mxt_result SendTransferNotify(IN const CSipStatusLine& rContentStatusLine,
                                  IN bool bSendFinalNotify,
                                  IN int nAdditionalClientInfo = 0);

    // Summary:
    //  Processes the completion of the transfer procedure when a transfer in
    //  which we are acting as the transferee is over.
    void TransfereeTransferCompleted(IN IUaSspCallMgr::ETransferReport eTransferResult);

    // Summary:
    //  Refreshes the session using the proper request with regards to the
    //  current configuration.
    void RefreshSession();

    // Summary:
    //  Handles a received OPTIONS request.
    void HandleOptionsRequest(IN ISipServerEventControl* pServerEventCtrl,
                              IN const CSipPacket& rRequest,
                              IN TO ISceGenericEventInfo* pGenInfo);

    // Summary:
    //  Reports the EvCalled event on the manager.
    void ReportEvCalledHelper(IN const CSipPacket& rRequest,
                              IN ISceGenericEventInfo* pGenInfo,
                              IN IUaSspCall* pReplacedCall,
                              IN bool bIsOptionsRequest);

    // Summary:
    //  Transforms the ISipServerEventControl in a reliable
    //  ISipServerEventControl if it is needed.
    mxt_result TransformServerEvCtrlIfNeeded(INOUT unsigned int& ruSipErrorCode,
                                             INOUT const char*& rpszErrorMessage,
                                             OUT GO CHeaderList*& rpExtraHeaders);

    // Summary:
    //  Sets the specified bitmask in the bitset.
    void SetInBitSet(IN int nBitMask,
                     INOUT int& rnBitSet) const;

    // Summary:
    //  Sets the specified bitmask in the bitset.
    void SetInBitSet(IN int nBitMask,
                     INOUT mxt_opaque& ropqBitSet) const;

    // Summary:
    //  Resets the specified bitmask in the bitset.
    void ResetInBitSet(IN int nBitMask,
                       INOUT int& rnBitSet) const;

    // Summary:
    //  Resets the specified bitmask in the bitset.
    void ResetInBitSet(IN int nBitMask,
                       INOUT mxt_opaque& ropqBitSet) const;

    // Summary:
    //  Tells if the specified bitmask is present in the bitset.
    bool IsInBitSet(IN int nBitMask,
                    IN int nBitSet) const;

    // Summary:
    //  Tells if the specified bitmask is present in the bitset.
    bool IsInBitSet(IN int nBitMask,
                    IN mxt_opaque opqBitSet) const;

    // Summary:
    //  Verifies the validity of the Expires header.
    mxt_result VerifyExpiresHeader(IN const CSipPacket& rRequest);

    // Summary:
    //  Helper that aborts the component.
    void InternalAbort(IUaSspCallMgr::ETerminationReason eAbortReason);

// BRCM_CUSTOM - [add] Add connected ID support per RFC 4916
    // Summary:
    //  Verifies that if both parties support "connected ID",
    //  then checks if the local address needs to be updated,
    //  and updates if so.
    void ProcessOutConnectedIdUpdate(IN const CSipPacket& rRequest);

    // Summary:
    //  Updates remote address if both parties support "connected ID".
    void ProcessInConnectedIdUpdate(IN const CSipPacket& rRequest);
// BRCM_CUSTOM - [end] Add connected ID support per RFC 4916

    // Summary:
    //  Tells if an offer/answer negotiation is in progress or not.
    bool IsOfferAnswerNegotiationInProgress() const;

    // Summary:
    //  Tells if a INVITE or UPDATE transaction is in progress or not.
    bool IsInviteOrUpdateInProgress() const;

    // Summary:
    //  Create the Referred-By header and add it in the header list.
    void GenerateReferredByHeader(INOUT CHeaderList*& rpExtraHeaders);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaSspCall(IN const CUaSspCall& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaSspCall& operator=(IN const CUaSspCall& rSrc);


//-- Hidden Data Members
protected:
private:

// BRCM_CUSTOM - [add] Add connected ID support per RFC 4916
    // Tells if both parties support "connected-ID".
    bool m_bConnectedIdSupported;
// BRCM_CUSTOM - [end] Add connected ID support per RFC 4916

    // The manager to which we will report call related events.
    IUaSspCallMgr* m_pMgr;

    // The object managing the media negociation (e.g.: SDP).
    IMspOfferAnswerSession* m_pMspSession;

    // This is the object through which an INVITE is cancelled.
    ISipClientTransaction* m_pInviteClientTransaction;

    // This is the object from which responses for the INVITE are sent.
    ISipServerEventControl* m_pInviteServerEventControl;

    // Description:
    //  This enum describes how an ISipServerEventControl is reliable.
    enum EReliability
    {
        // Description:
        //  No 1xx response can be sent reliably with the
        //  ISipServerEventControl.
        e100REL_DISABLED,
        // Description:
        //  The ISipServerEventControl cannot currently send reliable
        //  provisional responses but it can be changed if reliability is
        //  needed.
        e100REL_CHANGE_WHEN_NEEDED,
        // Description:
        //  The ISipServerEventControl sends all its provisional responses
        //  reliably.
        e100REL_ENABLED
    };

    // Tells if m_pInviteServerEventControl is reliable.
    EReliability m_eReliabilityOfInviteServerEvCtrl;

    // This is the object through which responses are sent for INVITE.
    ISipServerEventControl* m_pPrackServerEventControl;

    // This is the object through which responses are sent for UPDATE.
    ISipServerEventControl* m_pUpdateServerEventControl;

    // Tells if the UPDATE method can be used.
    bool m_bUpdateSupported;

    // session media offer/answer negotiation state.
    EOfferAnswerState m_eSessionNegotiationState;

    // early-session media offer/answer negotiation state.
    EOfferAnswerState m_eEarlySessionNegotiationState;

    enum ECallState
    {
        // Tells that we are currently sending a 180. This is only set when
        // CallUserNotified is called until the 180 is sent.
        eSTATE_SENDING_180 = 1,
        // Tells whether the local user has been notified for this INVITE or
        // not. This state is only true between the time a 180 is sent to an
        // INVITE and the ACK is received for a sent 2xx for the INVITE.
        eLOCAL_USER_NOTIFIED = eSTATE_SENDING_180 * 2,
        // Tells whether the remote user has been notified for this client
        // INVITE or not. This is only true when there is a client INVITE
        // transaction and we were notified for it.
        eREMOTE_USER_NOTIFIED = eLOCAL_USER_NOTIFIED * 2,
        // Tells if this object is the caller. If not present, this object is
        // the callee.
        // This is used to calculate retry time for glaring when we are not
        // using the ISipGlareSvc.
        eCALL_ID_OWNER = eREMOTE_USER_NOTIFIED * 2,
        // Tells that an OPTIONS request has been received OOD and is being processed.
        ePROCESSING_OOD_OPTIONS_REQUEST = eCALL_ID_OWNER * 2,
        // Will be set if we sent the original INVITE and if it contained a
        // session offer.
        eORIGINAL_INVITE_CONTAINED_SESSION_OFFER = ePROCESSING_OOD_OPTIONS_REQUEST * 2,
        // Will be set if we sent the original INVITE and if it contained an
        // early-session offer.
        eORIGINAL_INVITE_CONTAINED_EARLY_SESSION_OFFER = eORIGINAL_INVITE_CONTAINED_SESSION_OFFER * 2,
        // Tells if a client INVITE is currently in progress.
        eCLIENT_INVITE_IN_PROGRESS = eORIGINAL_INVITE_CONTAINED_EARLY_SESSION_OFFER * 2,
        // Tells whether we currently are in a call or not. Will be true after
        // initial INVITE has succeeded (2xx sent or received) and before call
        // is terminated (before BYE is sent).
        eIN_CALL = eCLIENT_INVITE_IN_PROGRESS * 2,
        // Tells that this call object is inside a SIP dialog.
        eIN_INVITE_DIALOG = eIN_CALL * 2,
        // Tells that this call cannot be terminated until the ACK request to
        // the INVITE is received or a 2xx response timeout occurs.
        eCANNOT_TERMINATE_UNTIL_INVITE_ACK_REQUEST_RECEIVED = eIN_INVITE_DIALOG * 2,
// BRCM_CUSTOM - [add] Two-step BYE process
        eCANNOT_TERMINATE_UNTIL_BYE_RESENT = eCANNOT_TERMINATE_UNTIL_INVITE_ACK_REQUEST_RECEIVED * 2,
// BRCM_CUSTOM - [end] Two-step BYE process
        // Tells if IUaSspCallMgr::EvTerminated has already been called.
// BRCM_CUSTOM - [mod] Two-step BYE process
        eTERMINATED = eCANNOT_TERMINATE_UNTIL_BYE_RESENT * 2,
// BRCM_CUSTOM - [end] Two-step BYE process
        // Tells if the Abort method was called.
        eABORTED = eTERMINATED * 2
    };

    // Value giving the current state. State is a bitset of ECallState.
    int m_nState;

    // Buffered data.

    struct SBufferedPayload
    {
        // Description:
        //  The session payload.
        const CBlob* m_pSessionSdp;

        // Description:
        //  The early-session payload.
        const CBlob* m_pEarlySessionSdp;

        // Description:
        //  The packet containing the pointed payloads.
        const CSipPacket* m_pContainerPacket;

        SBufferedPayload();

        ~SBufferedPayload();
    };

    // This is the payload from the original incoming INVITE.
    SBufferedPayload* m_pstBufferedInvitePayload;

    // This is the payload that will be sent into an original INVITE response.
    struct SBufferedInviteResponsePayload
    {
        // Description:
        //  The session state after the payload for the 180 is sent.
        EOfferAnswerState m_e180SessionNextState;

        // Description:
        //  The early-session state after the payload for the 180 is sent.
        EOfferAnswerState m_e180EarlySessionNextState;

        // Description:
        //  The payload to send into the 180 response.
        CSipMessageBody* m_p180Payload;

        // Description:
        //  Additional Require headers to send in the 180 response.
        CSipHeader* m_p180AdditionalRequiredExtension;

        // Description:
        //  The session state after the payload for the 200 is sent.
        EOfferAnswerState m_e200SessionNextState;

        // Description:
        //  The payload to send into the 200 response.
        CSipMessageBody* m_p200Payload;

        // Description:
        //  Additional Require headers to send in the 200 response.
        CSipHeader* m_p200AdditionalRequiredExtension;

        SBufferedInviteResponsePayload();

        ~SBufferedInviteResponsePayload();
    };

    // This is needed to be able to compile with MSVC6 in which it is not
    // possible for a struct declared in a class to access the enumeration
    // values that are declared as private or protected in the same class.
    friend struct SBufferedInviteResponsePayload;

    SBufferedInviteResponsePayload* m_pstBufferedInviteResponsePayload;

    // This is the payload that will be sent into an INVITE.
    // It is stored here because we are waiting for an ACK for a
    // previous INVITE.
    struct SBufferedPayloadToSendInNextInvite
    {
        // Description:
        //  The session payload.
        CBlob* m_pSessionSdp;

        // The type of message to be used to send the payload.
        EMessageTypeForPayload m_eMessageType;

        //  Additional Require headers to be included in the packet.
        uint32_t m_uBitSetAdditionalRequiredExtension;
    };

    SBufferedPayloadToSendInNextInvite* m_pPayloadToSendInInvite;

    // Tells if we must refresh the session (session-timers) when
    // EvReadyToRetry is called.
    bool m_bMustRefresh;

    // This struct holds data to fix internal glaring conditions.
    struct SInternalGlaringData
    {
        // Description:
        //  The session state after the payload is processed.
        EOfferAnswerState m_eSessionNextState;

        // The session payload.
        const CBlob* m_pSessionPayload;

        // The type of SDP that has been received. Must be ignored if
        // m_pSessionPayload is NULL.
        IMspOfferAnswerSession::ESdpType m_eSessionSdpType;

        // The packet containing the payloads.
        const CSipPacket* m_pContainerPacket;

        SInternalGlaringData();

        ~SInternalGlaringData();
    };

    // This is needed to be able to compile with MSVC6 in which it is not
    // possible for a struct declared in a class to access the enumeration
    // values that are declared as private or protected in the same class.
    friend struct SInternalGlaringData;

    // Holds the offers that were received while we were preparing an offer.
    // This payload will get processed when EvNewSdpToSend is processed.
    SInternalGlaringData* m_pstInternalGlaringData;


    // Tells whether this call is currently transferring the call. When true,
    // this call is acting as a transferor in a transfer scenario from
    // transferor to transferee.
    bool m_bIsTransferor;


    // Transfer data when this user agent acts as the transferee and this call
    // is with the transferor.
    struct STransfereeNotifierData
    {
        // Description:
        //  This is the name-addr where we are transferred to.
        CNameAddr* m_pAddrToTransferTo;

        // Description:
        //  Tells if the transfer request has been accepted.
        bool m_bAcceptedTransfer;

        // Description:
        //  This tells if we are currently waiting for a response to a NOTIFY.
        bool m_bPendingNotify;

        // Description:
        //  When non-NULL, represents a buffered final notification to be sent
        //  when the current NOTIFY is over.
        CSipStatusLine* m_pBufferedFinalNotify;

        // Description:
        //  The additional bitset to add to the NOTIFY client transaction.
        int m_nBufferedFinalNotifyAdditionalClientInfo;

        enum EFinalNotifyStatus
        {
            // Description:
            //  No final NOTIFY has been sent yet.
            eFINAL_NOTIFY_NOT_SENT,
            // Description:
            //  A final NOTIFY has been sent but not answered yet.
            eFINAL_NOTIFY_SENT,
            // Description:
            //  A final NOTIFY ws sent and the final response for it has been
            //  received.
            eFINAL_NOTIFY_COMPLETED
        };

        // Description:
        //  Gives the state of the final notification.
        EFinalNotifyStatus m_eFinalNotifyStatus;

        // Description:
        //  Tells whether IUaSspCallMgr::EvTransfereeTransferResult has been
        //  called or not.
        bool m_bManagerNotifiedOfCompletion;

        // Description:
        //  Referred-By header to copy from the REFER request to the INVITE
        //  request.
        CSipHeader* m_pReferredByHeader;

        STransfereeNotifierData();

        ~STransfereeNotifierData();
    };

    // This is the data needed when we are in a transfer scenario in which we
    // are the transferee and the peer of the call is the transferor.
    STransfereeNotifierData* m_pstTransfereeNotifierData;


    // This is the IUaSspCall on which we must send our session progress when
    // we are a call that has been created for a transfer and we are
    // transferee. This is the call with the transferor while the current call
    // is with the transfer target.
    IUaSspCall* m_pTransfereeNotifier;


    // This is the IUaSspCall that this call replaces. When AcceptCall is
    // called, this call is terminated.
    IUaSspCall* m_pReplacedCall;

    // This is the caller preferences to apply to the current outgoing INVITE.
    CSceCallerPreferences* m_pInviteCallerPreferences;

    // This holds data needed to handle a supplemental response on a newly
    // created CUaSspCall.
    struct SSupplementalCallData
    {
        // Description:
        //  The response creating a new dialog.
        const CSipPacket* m_pSupplementalResponse;

        // Description:
        //  The ISipSessionSvc instance from which
        //  EvNewSessionNeededForOriginalInviteResponse was called.
        ISipSessionSvc* m_pOriginatorSessionSvc;

        // Description:
        //  The IMspOfferAnswerSession of the originator call.
        IMspOfferAnswerSession* m_pOriginalMsp;

        // Description:
        //  Tells if there was a session offer in the INVITE.
        bool m_bSessionOfferInInvite;

        // Description:
        //  Tells if there was an early-session offer in the INVITE.
        bool m_bEarlySessionOfferInInvite;

        SSupplementalCallData();

        ~SSupplementalCallData();
    };

    // This holds the data that must be used when the ISipUserAgentSvc is
    // correctly configured for a supplemental context. It is set in
    // HandleForkedResponse and used in OnUserAgentServiceConfigured.
    SSupplementalCallData* m_pstSupplementalCallData;

    // Description:
    //  These are the shutdown steps. They are executed in the order that they
    //  are declared in this enum. The step is automatically changed each time
    //  that Shutdown is called.
    enum EShutdownStep
    {
        // Description:
        //  We are not currently terminating.
        eSTEP_NOT_TERMINATING,
        // Description:
        //  The first step of the shutdown. Will execute things that can or
        //  must be done before the shutdown process.
        eSTEP_INITIATE_SHUTDOWN,
        // Description:
        //  This step makes sure that all pending offers are cancelled on the
        //  IMspOfferAnswerSession.
        eSTEP_COMPLETE_NEGOTIATION,
        // Description:
        //  Tells the MSP that we will terminate soon.
        eSTEP_PRETERMINATE_MSP_SESSION,
        // Description:
        //  This step rejects all pending requests with a 487 Request
        //  Terminated.
        eSTEP_REJECT_PENDING_REQUESTS,
        // Description:
        //  This step cancels outgoing INVITE requests and waits for outgoing
        //  requests to end.
        eSTEP_TERMINATE_OUTGOING_REQUESTS,
        // Description:
        //  Make sure that we complete the transfer when acting as transferor.
        eSTEP_COMPLETE_TRANSFEROR_TRANSFER,
        // Description:
        //  Make sure that we sent a final NOTIFY if we are a call from
        //  transferee to transferor.
        eSTEP_END_TRANSFEREE_NOTIFICATION,
        // Description:
        //  This step makes sure that, if we have to, we wait for the final
        //  ACK to a server INVITE before going to the next step.
        eSTEP_WAIT_FOR_FINAL_INVITE_ACK_IF_NEEDED,
        // Description:
        //  This step terminates the SIP dialog by sending a BYE.
        eSTEP_SEND_BYE,
// BRCM_CUSTOM - [add] Two-step BYE process
        // Description:
        //  This step terminates the SIP dialog by resending a BYE with no extra headers.
        eSTEP_RESEND_BYE,
// BRCM_CUSTOM - [end] Two-step BYE process
        // Description:
        //  This step terminates and releases the IMspOfferAnswerSession.
        eSTEP_TERMINATE_MSP_SESSION,
        // Description:
        //  This step releases the ISipContext.
        eSTEP_RELEASE_CONTEXT,
        // Description:
        //  This step calls the event.
        eSTEP_CALL_EVTERMINATED,
        // Description:
        //  This gives the value of the last shutdown step. It has the same
        //  value than the last step to execute.
        eSTEP_LAST_SHUTDOWN_STEP = eSTEP_CALL_EVTERMINATED
    };

    // This is the shutdown step. It gives the next step to process when doing
    // shutdown. This must only be used when we are terminating.
    EShutdownStep m_eShutdownStep;

    // Description:
    //  This structure contains data that might be used by the shutdown
    //  procedure.
    struct SShutdownData
    {
        // Description:
        //  The final Status-Code that is sent for the INVITE request. When 0,
        //  the default value is used.
        unsigned int m_uInviteFinalStatusCode;

        // Description:
        //  The Reason-Phrase that is sent in the final response to the INVITE.
        //  NULL for default (default value depends on
        //  m_uInviteFinalStatusCode).
        CString* m_pstrInviteFinalReasonPhrase;

        // Description:
        //  The extra headers that are added to the final response of the
        //  INVITE.
        CHeaderList* m_pInviteFinalResponseExtraHeaders;

        SShutdownData();

        ~SShutdownData();
    };

    // Extra Headers to be sent with the INVITE.
    CHeaderList* m_pExtraHeaders;

    // Contains data that can be used in shutdown.
    SShutdownData* m_pstShutdownData;

    // The termination reason.
    IUaSspCallMgr::ETerminationReason m_eTerminationReason;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  SBufferedPayload METHODS  ==============================================
//==============================================================================

//==============================================================================
//==
//==  SBufferedPayload
//==
//==============================================================================
//
//  Description:
//      Default constructor.
//
//==============================================================================

inline CUaSspCall::SBufferedPayload::SBufferedPayload()
:   m_pSessionSdp(NULL),
    m_pEarlySessionSdp(NULL),
    m_pContainerPacket(NULL)
{
}


//==============================================================================
//====  SBufferedInviteResponsePayload METHODS  ================================
//==============================================================================

//==============================================================================
//==
//==  SBufferedInviteResponsePayload
//==
//==============================================================================
//
//  Description:
//      Default constructor.
//
//==============================================================================
inline CUaSspCall::SBufferedInviteResponsePayload::SBufferedInviteResponsePayload()
:   m_e180SessionNextState(eIDLE_NEGOTIATION),
    m_e180EarlySessionNextState(eIDLE_NEGOTIATION),
    m_p180Payload(NULL),
    m_p180AdditionalRequiredExtension(NULL),
    m_e200SessionNextState(eIDLE_NEGOTIATION),
    m_p200Payload(NULL),
    m_p200AdditionalRequiredExtension(NULL)
{
}


//==============================================================================
//====  SInternalGlaringData METHODS  ==========================================
//==============================================================================

//==============================================================================
//==
//==  SInternalGlaringData
//==
//==============================================================================
//
//  Description:
//      Default constructor.
//
//==============================================================================
inline CUaSspCall::SInternalGlaringData::SInternalGlaringData()
:   m_eSessionNextState(eIDLE_NEGOTIATION),
    m_pSessionPayload(NULL),
    m_eSessionSdpType(IMspOfferAnswerSession::eOFFER_SHORT),
    m_pContainerPacket(NULL)
{
}


//==============================================================================
//====  STransfereeNotifierData METHODS  =======================================
//==============================================================================

//==============================================================================
//==
//==  STransfereeNotifierData
//==
//==============================================================================
//
//  Description:
//      Default constructor.
//
//==============================================================================
inline CUaSspCall::STransfereeNotifierData::STransfereeNotifierData()
:   m_pAddrToTransferTo(NULL),
    m_bAcceptedTransfer(false),
    m_bPendingNotify(false),
    m_pBufferedFinalNotify(NULL),
    m_nBufferedFinalNotifyAdditionalClientInfo(0),
    m_eFinalNotifyStatus(eFINAL_NOTIFY_NOT_SENT),
    m_bManagerNotifiedOfCompletion(false),
    m_pReferredByHeader(NULL)
{
}


//==============================================================================
//====  SSupplementalCallData METHODS  =========================================
//==============================================================================

//==============================================================================
//==
//==  SSupplementalCallData
//==
//==============================================================================
//
//  Description:
//      Default constructor.
//
//==============================================================================
inline CUaSspCall::SSupplementalCallData::SSupplementalCallData()
:   m_pSupplementalResponse(NULL),
    m_pOriginatorSessionSvc(NULL),
    m_pOriginalMsp(NULL),
    m_bSessionOfferInInvite(false),
    m_bEarlySessionOfferInInvite(false)
{
}


//==============================================================================
//====  SShutdownData METHODS  =================================================
//==============================================================================

//==============================================================================
//==
//==  SShutdownData
//==
//==============================================================================
//
//  Description:
//      Default constructor.
//
//==============================================================================
inline CUaSspCall::SShutdownData::SShutdownData()
:   m_uInviteFinalStatusCode(0),
    m_pstrInviteFinalReasonPhrase(NULL),
    m_pInviteFinalResponseExtraHeaders(NULL)
{
}

//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  SetInBitSet
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to set in opq.
//
//      rnBitSet:
//       The value in which we set nBitMask.
//
//  Description:
//      Sets the specified bitmask in the bitset.
//
//==============================================================================
inline void CUaSspCall::SetInBitSet(IN int nBitMask,
                                    INOUT int& rnBitSet) const
{
    rnBitSet = rnBitSet | nBitMask;
}

//==============================================================================
//==
//==  SetInBitSet
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to set in opq.
//
//      ropqBitSet:
//       The value in which we set nBitMask.
//
//  Description:
//      Sets the specified bitmask in the bitset.
//
//==============================================================================
inline void CUaSspCall::SetInBitSet(IN int nBitMask,
                                    INOUT mxt_opaque& ropqBitSet) const
{
    SetInBitSet(nBitMask,
                INOUT reinterpret_cast<int&>(ropqBitSet));
}


//==============================================================================
//==
//==  ResetInBitSet
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to reset in opq.
//
//      rnBitSet:
//       The value in which we set nBitMask.
//
//  Description:
//      Resets the specified bitmask in the bitset.
//
//==============================================================================
inline void CUaSspCall::ResetInBitSet(IN int nBitMask,
                                      INOUT int& rnBitSet) const
{
    rnBitSet = rnBitSet & (~nBitMask);
}


//==============================================================================
//==
//==  ResetInBitSet
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to reset in opq.
//
//      ropqBitSet:
//       The value in which we set nBitMask.
//
//  Description:
//      Resets the specified bitmask in the bitset.
//
//==============================================================================
inline void CUaSspCall::ResetInBitSet(IN int nBitMask,
                                      INOUT mxt_opaque& ropqBitSet) const
{
    ResetInBitSet(nBitMask,
                  INOUT reinterpret_cast<int&>(ropqBitSet));
}


//==============================================================================
//==
//==  IsInBitSet
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to search in opq.
//
//      nBitSet:
//       The value in which we look if nBitMask is present.
//
//  Returns:
//      true:
//       nBitMask is present in nBitSet.
//
//      false:
//       nBitMask is not present in nBitSet.
//
//  Description:
//      Verifies if a bitmask value is present in a specified bitset.
//
//==============================================================================
inline bool CUaSspCall::IsInBitSet(IN int nBitMask,
                                   IN int nBitSet) const
{
    return (nBitSet & nBitMask) == nBitMask;
}


//==============================================================================
//==
//==  IsInBitSet
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to search in opq.
//
//      opqBitSet:
//       The value in which we look if nBitMask is present.
//
//  Returns:
//      true:
//       nBitMask is present in nBitSet.
//
//      false:
//       nBitMask is not present in nBitSet.
//
//  Description:
//      Verifies if a bitmask value is present in a specified bitset.
//
//==============================================================================
inline bool CUaSspCall::IsInBitSet(IN int nBitMask,
                                   IN mxt_opaque opqBitSet) const
{
    return IsInBitSet(nBitMask, MX_OPQ_TO_INT32(opqBitSet));
}

//==============================================================================
//==
//==  IsNegotiationInProgress
//==
//==============================================================================
//
//  Returns:
//      True if an offer/answer negotiation is in progress, false otherwise.
//
//  Description:
//      Tells if an offer/answer negotiation is in progress or not.
//
//==============================================================================
inline bool CUaSspCall::IsOfferAnswerNegotiationInProgress() const
{
    return (m_eSessionNegotiationState != eIDLE_NEGOTIATION &&
            m_eSessionNegotiationState != eINVITE_NEGOTIATION_COMPLETED);
}

//==============================================================================
//==
//==  IsInviteOrUpdateInProgress
//==
//==============================================================================
//
//  Returns:
//      True if a INVITE or UPDATE transaction is in progress, false otherwise.
//
//  Description:
//      Tells if a INVITE or UPDATE transaction is in progress or not.
//
//==============================================================================
inline bool CUaSspCall::IsInviteOrUpdateInProgress() const
{
    return (IsInBitSet(eCLIENT_INVITE_IN_PROGRESS, m_nState) ||
            m_pInviteServerEventControl != NULL ||
            m_pUpdateServerEventControl != NULL);
}


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CUASSPCALL_H
