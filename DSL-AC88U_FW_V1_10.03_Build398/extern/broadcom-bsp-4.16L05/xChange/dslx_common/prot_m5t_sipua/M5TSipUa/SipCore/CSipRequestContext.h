//==============================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T").
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
//==============================================================================
#ifndef MXG_CSIPREQUESTCONTEXT_H
#define MXG_CSIPREQUESTCONTEXT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
// For MXD_SIPSTACK_ENABLE_LOCALLY_GENERATED_ERROR_RESPONSE_DIFFERENTIATOR
#include "Config/SipStackCfg.h"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CSIPCOREEVENTLIST_H
#include "SipCore/CSipCoreEventList.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISIPCLIENTEVENTCONTROL_H
#include "SipCore/ISipClientEventControl.h"
#endif

#ifndef MXG_ISIPCLIENTTRANSACTION_H
#include "SipCore/ISipClientTransaction.h"
#endif

#ifndef MXG_ISIPREQUESTCONTEXT_H
#include "SipCore/ISipRequestContext.h"
#endif

#ifndef MXG_ISIPSERVEREVENTCONTROL_H
#include "SipCore/ISipServerEventControl.h"
#endif

#ifndef MXG_ISIPTRANSACTIONUSER_H
#include "SipTransaction/ISipTransactionUser.h"
#endif

#ifndef MXG_ISIPTRANSPORTUSER_H
#include "SipTransport/ISipTransportUser.h"
#endif

#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING
    #ifndef MXG_CWATCHEDINSTANCE_H
    #include "SipTransport/CWatchedInstance.h"
    #endif
#endif

#ifndef MXG_ISIPCORECONFIG_H
#include "SipCore/ISipCoreConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipForkedDialogGrouper;
class CMarshaler;
class CHeaderList;
class CSipMessageBody;
class CSipPacket;
class CToken;
class ISipClientSvc;
class ISipReqCtxConnectionSvc;
class ISipReqCtxCoreSvc;
class ISipRequestContextMgr;
class ISipTransactionMgr;
class ISipTransportMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipRequestContext
//========================================
//
// Description:
//   This class represents a transaction client or server. It uses the core and
//   connection request context services to create and analyse requests and
//   their responses.
//
//   It is also responsible for managing the CANCEL associated with the
//   transaction it manages.
//
//   It is the implementation of the ISipRequestContext, ISipClientEventControl,
//   ISipClientTransaction, and ISipServerEventControl interfaces and it is
//   through these interfaces that the application interacts with it.
//
//   Services that handle incoming requests or generate outgoing requests are
//   responsible for creating it.
//
//   This class also implements the ISipTransactionUser interface to communicate
//   with the transaction manager.
//
//   Finally, it also implements the ISipTransportUser interface to communicate
//   with the transport layer.
//
// Location:
//   SipCore/CSipRequestContext.h
//
// See Also:
//   ISipAppTransaction, ISipTransactionUser, ISipTransportUser
//
//==EDOC========================================================================
class CSipRequestContext : private CEventDriven,
                           private CEComDelegatingUnknown,
                           private ISipClientEventControl,
                           private ISipClientTransaction,
                           private ISipRequestContext,
                           private ISipServerEventControl,
                           private ISipTransactionUser,
                           private ISipTransportUser
#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING
                           , public CWatchedInstance<CSipRequestContext>
#endif
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
#ifdef MXD_SIPSTACK_ENABLE_SIPREQUEST_CONTEXT_INSTANCE_TRACKING
    friend class CInstanceTracker;      // To access the data members
#endif
#if defined(MXD_SIPSTACK_ENABLE_DEPRECATED_SIPCONTEXT_INSTANCE_TRACKING) || \
    defined(MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING)
    friend class CDeprecatedInstanceTracker; // To cast back to ISipContext
#endif
    //M5T_INTERNAL_USE_END

//-- Published Interface
//------------------------
public:
    //-- << Stereotype >>
    //--------------------------------------------

    // Summary:
    //  Initializes the transaction and transport managers used by all
    //  instances of this class.
    //-----------------------------------------------------------------
    static void Initialize(IN ISipTransactionMgr& rTransactionMgr,
                           IN ISipTransportMgr& rTransportMgr);

    // Summary:
    //  Gets the number of CSipRequestContexts in the stack.
    //-------------------------------------------------------
    static unsigned int GetActiveReqCtxCount();

#if defined(MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT)
    // Summary:
    //  Sets the manager for the request context events.
    static void SetRequestContextManager(ISipRequestContextMgr* pMgr);
#endif

    // Summary:
    //  Sets the 503 Service Unavailable response handling mode.
    static void SetServiceUnavailableHandlingMode(IN ISipCoreConfig::EServiceUnavailableHandling eServiceUnavailableHandlingMode);

    // Summary:
    //  Gets the 503 Service Unavailable response handling mode.
    static ISipCoreConfig::EServiceUnavailableHandling GetServiceUnavailableHandlingMode();

//-- Hidden Methods
//-------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipRequestContext();

    static void FinalizeCSipRequestContext();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipRequestContext(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipRequestContext();


    //-- << Helper methods >>
    //--------------------------------------------
    mxt_result UpdatePacketHelper(IN bool bAsynchronous, INOUT CSipPacket& rPacket);

    mxt_result UpdatePacketWithCoreSvc(IN bool bAsynchronous,
                                       INOUT CSipPacket& rPacket);

    mxt_result UpdatePacketWithConnectionSvc(INOUT CSipPacket& rPacket);

#if defined(MXD_SIPSTACK_ENABLE_LOCALLY_GENERATED_ERROR_RESPONSE_DIFFERENTIATOR)
    // Summary:
    //   Helper method to add the custom header to expected packets when it is
    //   required.
    //--------------------------------------------------------------------------
    void AddDifferentiatorHeaderHelper(INOUT CSipPacket& rPacket);
#endif

    // Position meaning for the bits in the m_bsState bitset.
    //--------------------------------------------------------
    enum EState
    {
        // This is set when sending a packet with the transport manager.
        //---------------------------------------------------------------
        eSEND_WITH_TRANSPORT = 0x0001,
        // This is set when sending a packet with the transaction manager.
        //-----------------------------------------------------------------
        eSEND_WITH_TRANSACTION = 0x0002,
        // This is set when we are asked to send a CANCEL but have not received
        // a provisional response yet.
        //----------------------------------------------------------------------
        eSEND_CANCEL = 0x0004,
        // This is set when we sent a CANCEL and are waiting for the final
        // response for the cancelled request.
        //-----------------------------------------------------------------
        eCANCEL_SENT = 0x0008,
        // This is set when a request context core service needs asynchronous
        // process to update a packet.
        //--------------------------------------------------------------------
        eUPDATING_WITH_CORE_SVC = 0x0010,
        // This is set when a request context connection service needs
        // asynchronous process to update a packet.
        //-------------------------------------------------------------
        eUPDATING_WITH_CX_SVC = 0x0020,
        // This is set when a request was updated through UpdatePacket. This
        // cannot be true when eSERVER is true but both can be false when the
        // request context has not processed a packet yet.
        //--------------------------------------------------------------------
        eCLIENT = 0x0040,
        // This is set when CreateServerTransaction is called or when
        // UpdatePacket is first called with a response. This cannot be true
        // when eCLIENT is true but both can be false when the request context
        // has not processed a packet yet.
        //---------------------------------------------------------------------
        eSERVER = 0x0080,
        // This is set when a final success response is already in progress.
        // This means that UpdatePacket has been successfully called for a
        // final success response.
        //-------------------------------------------------------------------
        eFINAL_SUCCESS_RESPONSE_SENT = 0x0100,
        // This is set when a final failure response is already in progress.
        // This means that UpdatePacket has been successfully called for a
        // final failure response.
        //-------------------------------------------------------------------
        eFINAL_FAILURE_RESPONSE_SENT = 0x0200,
        // Set when the request context is currently processing a packet.
        //----------------------------------------------------------------
        ePROCESSING_PACKET = 0x0400,

        // Set when the packet is to be handled asynchronously (
        // resSW_SIPCORE_WAIT_ASYNC_RESPONSE
        eSENDING_PACKET_ASYNCHRONOUSLY = 0x0800,

        // This is set when a retransmission is sent.
        eSENDING_RETRANSMISSION = 0x1000
    };

    mxt_result SendPacketHelper(IN CSipPacket& rPacket,
                                IN EState eSendThrough,
                                IN bool bIsRetransmission = false);

    mxt_result DoSendPacket(IN CSipPacket& rPacket);

    void OnResponseReceivedHelper(IN const CSipPacket& rPacket,
                                  IN mxt_opaque opqTransaction);

    void CreateCancelHelper(OUT CSipPacket*& rpCancel);

    void ClearReIssueData();

    mxt_result CancelRequestHelper();

    mxt_result SendPacketFailureHelper(IN CSipPacket& rPacket, IN unsigned int uCode, IN const char* szReason);

    void SendBufferedPacketHelper();

    mxt_result RequestFailedHelper(IN const CSipPacket& rPacket,
                                   IN mxt_opaque opqTransaction,
                                   IN mxt_result result);

    mxt_result ResponseFailedHelper(IN mxt_opaque opqTransaction,
                                    IN mxt_result result);

    mxt_result TimeoutLastRequestSent();

#if defined(MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT)
    void ReportEvCreated();
    void ReportEvCompleted();
#endif

    //-- << Bitset methods >>
    //--------------------------------------------

    bool IsSet(IN EState eState) const;

    void Set(IN EState eState);

    void Reset(IN EState eState);


    //-- << Messaging methods >>
    //--------------------------------------------

    mxt_result InternalEvCommandResult(IN mxt_result result);


    //-- << CEventDriven Interface >>.
    //---------------------------------

    // Summary:
    //  Processes the message queue.
    //-------------------------------
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    //-- << CEComDelegatingUnknown Interface >>.
    //-------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << ISipClientEventControl Interface >>.
    //-------------------------------------------

    virtual mxt_result CallNextClientEvent();

    virtual mxt_result ReIssueRequest(
                                  IN mxt_opaque opqTransaction,
                                  OUT ISipClientTransaction*& rpNewTransaction);

    virtual mxt_result ClearClientEvents();

    virtual mxt_opaque GetOpaque();

    virtual void SetOpaque(IN mxt_opaque opq);

    virtual mxt_result RemoveReIssueMessageBody(OUT GO CSipMessageBody** ppMessageBody);

    virtual mxt_result OverrideReIssueMessageBody(IN TOA CSipMessageBody* pMessageBody);

    virtual mxt_result RemoveReIssueExtraHeaders(OUT GO CHeaderList** ppExtraHeaders);

    virtual mxt_result OverrideReIssueExtraHeaders(IN TOA CHeaderList* pExtraHeaders);


    //-- << ISipClientTransaction Interface >>.
    //------------------------------------------
    virtual mxt_result CancelRequest(IN CHeaderList* pExtraHeaders,
                                     IN CSipMessageBody* pMessageBody);

    virtual void SetCancelRequestBehavior(IN ECancelRequestBehavior eCancelBehavior);

    //-- << ISipRequestContext Interface >>.
    //---------------------------------------

    virtual mxt_result Initialize(IN IEComUnknown* pContext);

    virtual mxt_result UpdatePacket(INOUT CSipPacket& rPacket);

    virtual void OnPacketUpdated(IN mxt_result asyncResult,
                                 INOUT CSipPacket& rPacket);

    virtual mxt_result EnableForkedDialogGrouper(IN CSipForkedDialogGrouper* pForkedDialogGrouper);

    virtual mxt_result SendPacketThroughTransaction(IN CSipPacket& rPacket);

    virtual mxt_result SendPacketThroughTransport(IN CSipPacket& rPacket);

    virtual mxt_result SendRetransmissionThroughTransport(IN CSipPacket& rPacket);

    virtual void ProcessEvents(IN const CSipPacket& rPacket);

    virtual mxt_result CreateServerTransaction(
                IN ISipTransactionMgr::ETransactionRole eMergedRequestDetection,
                IN const CSipPacket& rPacket);

    virtual void SetOwner(IN ISipReqCtxCoreSvc* pOwner);

    virtual void StoreReIssueData(IN ISipClientSvc& rClientSvc,
                                  IN const CToken& rMethod,
                                  IN CHeaderList* pExtraHeaders,
                                  IN CSipMessageBody* pMessageBody,
                                  IN mxt_opaque opqService);

    virtual CVector<ISipReqCtxCoreSvc*>& GetCoreSvcVector();

    virtual CVector<ISipReqCtxConnectionSvc*>& GetConnectionSvcVector();

    virtual void ClearEvents();

    virtual void MuteIncomingCancel(IN bool bMuteIncomingCancel);

// BRCM_CUSTOM - [add] Add TIP support per 3GPP TS 22.173
    virtual mxt_opaque GetClientEventDataOpaque();

    virtual void SetClientEventDataOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add TIP support per 3GPP TS 22.173
// BRCM_CUSTOM - [add] Add CCTK GUID opaque
    virtual mxt_opaque GetGuidOpaque();

    virtual void SetGuidOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
    virtual mxt_opaque GetTosOpaque();

    virtual void SetTosOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add TOS config support

    virtual void SetTransactionStatistics(
                                    IN ISipTransactionStatistics* pTransStats);

    virtual mxt_result Terminate();

    virtual const CSipPacket* GetLastPacketSent() const;

    virtual void GetLastPacketReceived(OUT const CSipPacket*& rpLastPacketReceived) const;

    //-- << ISipServerEventControl Interface >>.
    //-------------------------------------------

    virtual mxt_result SendResponse(IN unsigned int uCode,
                                    IN const char* szReason,
                                    IN CHeaderList* pExtraHeaders,
                                    IN CSipMessageBody* pMessageBody);

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void SetSendOption(IN bool bToNetwork);
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    //-- << ISipTransactionUser Interface >>
    //--------------------------------------------

    virtual void EvRequestProgress(IN const CSipPacket& rPacket,
                                   IN mxt_opaque opqTransaction);

    virtual void EvRequestSuccess(IN const CSipPacket& rPacket,
                                  IN mxt_opaque opqTransaction);

    virtual void EvRequestFailed(IN const CSipPacket& rPacket,
                                 IN mxt_opaque opqTransaction,
                                 IN mxt_result result);

    virtual void EvRequestCancelled(IN const CSipPacket& rPacket,
                                    IN mxt_opaque opqTransaction);

    virtual void EvResponseSuccess(IN mxt_opaque opqTransaction);

    virtual void EvResponseFailed(IN mxt_opaque opqTransaction,
                                  IN mxt_result result);

    virtual void EvServerTransactionTerminated(IN mxt_opaque opqTransaction);


    //-- << ISipTransportUser Interface >>.
    //--------------------------------------

    virtual void EvCommandResult(IN mxt_result res,
                                 IN mxt_opaque opqSenderParam);

    void DispatchResponseToCoreSvcsHelper(IN const CSipPacket& rResponse);

#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING
    //-- << CWatchedInstance Mechanism >>
    //-------------------------------------

    // Summary:
    //  Gets a CBlob containing information about the CSipRequestContext still
    //  allocated.
    static void Dump(OUT CBlob& rblobRequestContextDump);
#endif // #if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipRequestContext(IN const CSipRequestContext& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipRequestContext& operator=(IN const CSipRequestContext& rSrc);


//-- Hidden Data Members
//------------------------
protected:
    enum EMessageID
    {
        eEVCOMMANDRESULT,
        eREPORTEVCOMPLETED
    };

    enum ETimerID
    {
        eCANCELLED_REQUEST_TIMEOUT
    };

    struct SBufferedEvent
    {
        enum EFunctionID
        {
            eEVREQUEST_PROGRESS,
            eEVREQUEST_SUCCESS,
            eEVREQUEST_FAILED,
            ePROCESS_EVENTS
        };

        EFunctionID m_eFunction;
        CMarshaler m_params;
    };

    // The transaction manager that will be used by all request contexts.
    //--------------------------------------------------------------------
    static ISipTransactionMgr* ms_pTransactionMgr;

    // The transport manager that will be used by all request contexts.
    //------------------------------------------------------------------
    static ISipTransportMgr* ms_pTransportMgr;

#if defined(MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT)
    // The request context manager where the request context related events
    // are reported.
    static ISipRequestContextMgr* ms_pReqCtxMgr;
#endif

    // The 503 Service Unavailable response handling mode.
    static ISipCoreConfig::EServiceUnavailableHandling ms_eServiceUnavailableHandlingMode;

    // The request context core services interested in this transaction.
    //-------------------------------------------------------------------
    CVector<ISipReqCtxCoreSvc*> m_vecpCoreSvc;

    // The request context connection services interested in this transaction.
    //-------------------------------------------------------------------------
    CVector<ISipReqCtxConnectionSvc*> m_vecpCxSvc;

    // This index is used to find which request context service requested
    // asynchronous update. This is used with the eUPDATING_WITH_CORE_SVC or
    // eUPDATING_WITH_CX_SVC bitset to indicate whether the index is in the
    // m_vecCoreSvc or m_vecCxSvc vector.
    //-----------------------------------------------------------------------
    unsigned int m_uSvcIndex;

    // The request connection service that is responsible for this
    // transaction.
    //-------------------------------------------------------------
    ISipReqCtxCoreSvc* m_pOwner;

    // The dialog grouper. It is set only once when sending an initial request
    // that can potentially fork. When set, responses that can come from
    // forking are passed to this forking grouper.
    //-------------------------------------------------------------------------
    CSipForkedDialogGrouper* m_pForkedDialogGrouper;

    // The object that will store events that request context core services
    // want to report to the application.
    //----------------------------------------------------------------------
    CSipCoreEventList m_eventList;

    CSipPacket* m_pLastPacketSent;

    const CSipPacket* m_pLastPacketReceived;

    mxt_opaque m_opqTransaction;

    // CANCEL transaction handle.
    mxt_opaque m_opqCancelTransaction;

    // Reissue data.
    //---------------
    ISipClientSvc* m_pClientSvc;
    CToken* m_pMethod;
    CHeaderList* m_pExtraHeaders;
    CSipMessageBody* m_pMessageBody;

    CVector<SBufferedEvent*> m_vecpstBufferedEventsQueue;

    // Buffered packet to send when sending could not be done because
    // asynchronous processing of a previous packet was not over.
    //----------------------------------------------------------------
    CVector<CSipPacket*> m_vecpBufferedResponsesToSend;

    // Cancel Extra headers and message-body. These need to be cached in case
    // that the CANCEL request is buffered.
    //------------------------------------------------------------------------
    CHeaderList* m_pCancelExtraHeaders;
    CSipMessageBody* m_pCancelMessageBody;

    uint16_t m_bsState;

    static unsigned int ms_uReqCtxCount;

    mxt_opaque m_opqApplicationData;

// BRCM_CUSTOM - [add] Add TIP support per 3GPP TS 22.173
    mxt_opaque m_opqTip;
// BRCM_CUSTOM - [end] Add TIP support per 3GPP TS 22.173
// BRCM_CUSTOM - [add] Add CCTK GUID opaque
    mxt_opaque m_opqGuid;
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
    mxt_opaque m_opqTos;
// BRCM_CUSTOM - [end] Add TOS config support
// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    bool m_bToNetwork;
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    // Pointer to the transaction statistics.
    //----------------------------------------
    ISipTransactionStatistics* m_pTransStats;

    bool m_bIncomingCancelMuted;

    mxt_opaque m_opqService;

    // Counter used to know how many times send is called to only release that
    // many references.

    unsigned int m_uSendCount;

    // Pointer to the transaction timers.
    const ISipTransactionMgr::STransactionsTimers* m_pstTransactionTimers;

#if defined(MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT)
    // Context that created the request context.  This value is used only when
    // the request context manager is called.
    ISipContext* m_pContext;

    // Tells if the EvCreated event has been called.
    bool m_bEvCreatedReported;
#endif

private:
    ECancelRequestBehavior m_eCancelBehavior;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetActiveReqCtxCount
//==
//==============================================================================
//
//  Returns:
//      The number of CSipRequestContexts in the stack.
//
//  Description:
//      Gets the number of CSipRequestContexts currently instanced.
//
//==EDOC========================================================================
inline unsigned int CSipRequestContext::GetActiveReqCtxCount()
{
    return ms_uReqCtxCount;
}

#if defined(MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT)
//==============================================================================
//==
//==  SetRequestContextManager
//==
//==============================================================================
//
//  Parameters:
//    pMgr:
//      The request context manager.
//
//  Description:
//      Sets the manager for the request context events.
//
//==============================================================================
inline void CSipRequestContext::SetRequestContextManager(IN ISipRequestContextMgr* pMgr)
{
    ms_pReqCtxMgr = pMgr;
}
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT)

//==============================================================================
//==
//==  SetServiceUnavailableHandlingMode
//==
//==============================================================================
//
//  Parameters:
//    eServiceUnavailableHandlingMode:
//      The 503 Service Unavailable response handling mode.
//
//  Description:
//      Sets the 503 Service Unavailable response handling mode. This mode
//      will tell the SIP Stack how to process a 503 Service Unavailable
//      response.
//
//      This mode affects the SIP Stack globally.
//
//==============================================================================
inline void CSipRequestContext::SetServiceUnavailableHandlingMode(IN ISipCoreConfig::EServiceUnavailableHandling eServiceUnavailableHandlingMode)
{
    ms_eServiceUnavailableHandlingMode = eServiceUnavailableHandlingMode;
}

//==============================================================================
//==
//==  GetServiceUnavailableHandlingMode
//==
//==============================================================================
//
//  Returns:
//      The 503 Service Unavailable response handling mode.
//
//  Description:
//      Gets the 503 Service Unavailable response handling mode.
//
//==============================================================================
inline ISipCoreConfig::EServiceUnavailableHandling CSipRequestContext::GetServiceUnavailableHandlingMode()
{
    return ms_eServiceUnavailableHandlingMode;
}

inline bool CSipRequestContext::IsSet(IN EState eState) const
{
    return (m_bsState & static_cast<uint16_t>(eState)) != 0;
}

inline void CSipRequestContext::Set(IN EState eState)
{
    m_bsState = (m_bsState | static_cast<uint16_t>(eState));
}

inline void CSipRequestContext::Reset(IN EState eState)
{
    m_bsState = (m_bsState & (~ static_cast<uint16_t>(eState)));
}

inline void CSipRequestContext::MuteIncomingCancel(IN bool bMuteIncomingCancel)
{
    m_bIncomingCancelMuted = bMuteIncomingCancel;
}

// BRCM_CUSTOM - [add] Add TIP support per 3GPP TS 22.173
inline mxt_opaque CSipRequestContext::GetClientEventDataOpaque()
{
    return m_opqTip;
}

inline void CSipRequestContext::SetClientEventDataOpaque(IN mxt_opaque opq)
{
    m_opqTip = opq;
}
// BRCM_CUSTOM - [end] Add TIP support per 3GPP TS 22.173

// BRCM_CUSTOM - [add] Add CCTK GUID opaque
inline mxt_opaque CSipRequestContext::GetGuidOpaque()
{
    return m_opqGuid;
}

inline void CSipRequestContext::SetGuidOpaque(IN mxt_opaque opq)
{
    m_opqGuid = opq;
}
// BRCM_CUSTOM - [end] Add CCTK GUID opaque

// BRCM_CUSTOM - [add] Add TOS config support
inline mxt_opaque CSipRequestContext::GetTosOpaque()
{
    return m_opqTos;
}

inline void CSipRequestContext::SetTosOpaque(IN mxt_opaque opq)
{
    m_opqTos = opq;
}
// BRCM_CUSTOM - [end] Add TOS config support

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
inline void CSipRequestContext::SetSendOption(IN bool bToNetwork)
{
    m_bToNetwork = bToNetwork;
}
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

inline void CSipRequestContext::SetCancelRequestBehavior(IN ECancelRequestBehavior eCancelBehavior)
{
    m_eCancelBehavior = eCancelBehavior;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREQUESTCONTEXT_H

