//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPSESSIONTRANSACTIONUASINVITE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSESSIONTRANSACTIONUASINVITE_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#include "SipCore/CSipReqCtxConnectionSvc.h"
#endif

#ifndef MXG_CSIPSESSIONTRANSACTION_H
#include "SipUserAgent/CSipSessionTransaction.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipSessionTransactionUasInvite
//==============================================================================
//<GROUP SipUserAgent>
//
// Summary:
//  The transaction used to handle server INVITE requests.
//
// Description:
//  Handles a server INVITE transaction from the reception of the INVITE to the
//  reception of the ACK or the timeout of the 2xx response.
//
//  The responsibilities of this class are:
//  - Manage the UAS INVITE transaction state
//  - Report events related to such transaction on the session manager
//  - Manage the ISipUserAgentSvc (except for Establish and TerminateUsage)
//  - Retransmit the 2xx responses
//  - Wait for the ACK once the 2xx has been sent
//  - Create and manage the INVITE ISipRequestContext for its lifetime
//  - Report the session dialog modifications to the controller
//
// Location:
//   SipUserAgent/CSipSessionTransactionUasInvite.h
//
// See Also:
//   CSipSessionTransaction, CSipSessionSvc
//
//==============================================================================
class CSipSessionTransactionUasInvite : private CEComDelegatingUnknown,
                                        private CEventDriven,
                                        private CSipReqCtxConnectionSvc,
                                        private CSipSessionTransaction
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

    // This is the value to use when no ESessionDialogTerminationType is set.
    static const int m_nESESSIONDIALOGTERMINATIONTYPE_INVALID_VALUE;

//-- Published Interface
public:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipSessionTransactionUasInvite();

    static void FinalizeCSipSessionTransactionUasInvite();


//-- Hidden Methods
protected:
private:
    //-- << Constructors / Destructors / Operators >>

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // waring or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif
//M5T_INTERNAL_USE_END

    // Summary:
    //  Default Constructor.
    CSipSessionTransactionUasInvite(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipSessionTransactionUasInvite();

    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //-- << CEComDelegatingUnknown methods >>

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- << CEventDriven >>

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    //-- << ISipReqCtxConnectionSvc >>

    virtual void OnSendSuccess(IN ISipRequestContext& rRequestContext,
                               IN const CSipPacket& rPacketSent);

    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

    //-- << ISipReqCtxCoreSvc >>

    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket& rPacket,
                                    IN unsigned int uCode,
                                    IN const char* pszReason,
                                    IN TOA CHeaderList* pExtraHeaders,
                                    IN TOA CSipMessageBody* pMessageBody);

    virtual void OnRequestCancelled(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket& rPacket);

    virtual void OnPacketReadyToSend(IN ISipRequestContext& rRequestContext,
                                     INOUT CSipPacket& rPacket);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    //-- << ISipSessionTransaction >>

    virtual void SetSessionDialogState(IN ESessionDialogState eSessionDialogState);

    virtual mxt_result AllowSendingRequest(IN ESipMethod eMethod);

    virtual mxt_result AllowReceivingRequest(IN ESipMethod eMethod,
                                             IN const CSipPacket& rPacket,
                                             OUT uint16_t& ruResponseRejectedCode,
                                             OUT GO CHeaderList** ppResponseExtraHeaders);

    virtual bool CanHandlePacket(IN ESipMethod eMethod,
                                 IN const CSipPacket& rPacket);

    virtual bool IsSessionDialogNeeded(IN ISipSessionTransactionController::ESessionDialogTerminationType eType);

    virtual void Clear();

    //-- << Helper methods >>

    // Summary:
    //  Processes states changes for an event.
    void ProcessEvent(IN ESipMethod eRequestMethod);

    // Summary:
    //  Completes pending actions, terminates this object and tells the
    //  controller that we are done.
    void Terminate();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipSessionTransactionUasInvite(IN const CSipSessionTransactionUasInvite& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipSessionTransactionUasInvite& operator=(IN const CSipSessionTransactionUasInvite& rSrc);


//-- Hidden Data Members
protected:
private:
    // The session dialog state.
    ESessionDialogState m_eSessionDialogState;

    // The request context of the transaction.
    ISipRequestContext* m_pRequestContext;

    // The timers.
    enum ETimer
    {
        // Used to retransmit 2xx responses.
        eTIMER_2XX_RETRANSMISSION,
        // Used to wait for the ACK request reception.
        eTIMER_ACK_WAITING
    };

    // The state of the Uas INVITE transaction.
    enum ETransactionState
    {
        // The INVITE has been received and no response has been sent yet.
        eUAS_INVITING,
        // A provisional response has been sent.
        eUAS_PROCEEDING,
        // A 2xx response has been sent but the 2xx retransmission timer is
        // not started.
        eUAS_ACCEPTING,
        // A 2xx response has been sent and the 2xx retransmission timer is
        // started.
        eUAS_ACCEPTED,
        // The transaction is currently terminated (Terminate has been called).
        eUAS_TERMINATING
    };

    // The state of the transaction.
    ETransactionState m_eTransactionState;

    // When different from m_nESESSIONDIALOGTERMINATIONTYPE_INVALID_VALUE, it
    // means that IsSessionDialogNeeded has been called and that we returned
    // true. In this case, we must call
    // ISipSessionTransactionController::EvTerminateSessionDialog with the value
    // of this member when the session dialog is not needed anymore.
    int m_nIsSessionDialogNeededValue;

    // The state of the final response updating versus clearing.
    enum EFinalResponseUpdateState
    {
        // No final response is currently being updated.
        eIDLE,
        // Final response is currently being updated.
        eUPDATING,
        // Final response is currently being updated and Clear was called.
        eCLEAR_UPDATING
    };

    // The state of the final response updating versus clearing. This is used as
    // we do not want to release the request context (set the owner NULL) while
    // the transaction is cleared and there is a final response being updated by
    // the request context. Setting the owner to NULL when there is a final
    // response buffered would forbid the request to be sent.
    EFinalResponseUpdateState m_eUpdateState;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSIPSESSIONTRANSACTIONUASINVITE_H

