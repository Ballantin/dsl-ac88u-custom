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
#ifndef MXG_CSIPSESSIONTRANSACTIONUACINVITE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSESSIONTRANSACTIONUACINVITE_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT to be able to use this class.
#endif

#ifndef MXG_CSIPSESSIONTRANSACTION_H
#include "SipUserAgent/CSipSessionTransaction.h"
#endif

#ifndef MXG_CSIPCLIENTSVC_H
#include "SipUserAgent/CSipClientSvc.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_STATUSCODE_H
#include "SipParser/StatusCode.h"
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
//== Class: CSipSessionTransactionUacInvite
//==============================================================================
//<GROUP SipUserAgent>
//
// Summary:
//   The transaction used to handle a client INVITE request.
//
// Description:
//  Handles a client INVITE transaction from the sending of the INVITE to the
//  sending of the ACK (inclusively).
//
//  The responsibilities of this class are:
//  - Manage the client transaction state
//  - Report events related to such transaction on the ISipSessionMgr
//  - Manage the ISipUserAgentSvc
//  - Create and manage the ISipRequestContext of the INVITE and of the ACK for
//    their lifetime
//  - Report the dialog state changes to the ISipSessionTransactionController
//
// Location:
//   SipUserAgent/CSipSessionTransactionUacInvite.h
//
// See Also:
//   CSipSessionTransaction, CSipSessionSvc
//
//==============================================================================
class CSipSessionTransactionUacInvite : private CSipSessionTransaction,
                                        private CSipClientSvc,
                                        private CEventDriven
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

    enum EObjectState
    {
        // Description:
        //  Tells if this INVITE is establishing a session dialog.
        eSTATE_ORIGINAL_INVITE = 1,

        // Description:
        //  Tells if the session dialog is currently established.
        eSTATE_SESSION_DIALOG_ESTABLISHED = eSTATE_ORIGINAL_INVITE * 2,

        // Description:
        //  Tells if we already called EvTerminateSessionDialog. This boolean is
        //  needed because IsSessionDialogNeeded might be called between the
        //  time we called EvTerminateSessionDialog and the time we call 
        //  EvTransactionTerminated. In this time, we need to return false in
        //  IsSessionDialogNeeded. The boolean is reset when we reissue a
        //  request.
        eSTATE_TERMINATED_DIALOG = eSTATE_SESSION_DIALOG_ESTABLISHED * 2,

        // Description:
        //  Tells if this object already completed a transaction. This is used
        //  to know if the a client transaction is a reissue of the original
        //  one.
        eSTATE_TERMINATED = eSTATE_TERMINATED_DIALOG * 2
    };

    // This is the value to use when no ESessionDialogTerminationType is set.
    static const int m_nESESSIONDIALOGTERMINATIONTYPE_INVALID_VALUE;

//-- Published Interface
public:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipSessionTransactionUacInvite();

    static void FinalizeCSipSessionTransactionUacInvite();


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
    //  Constructor.
    CSipSessionTransactionUacInvite(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipSessionTransactionUacInvite();

    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //-- << CEComDelegatingUnknown >>

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << ISipSessionTransaction >>

    virtual void SetSessionDialogState(IN ESessionDialogState eSessionDialogState);

    virtual mxt_result AllowSendingRequest(IN ESipMethod eMethod);

    virtual bool CanSendRequest(IN ESipMethod eMethod);

    virtual mxt_result AllowReceivingRequest(IN ESipMethod eMethod,
                                             IN const CSipPacket& rPacket,
                                             OUT uint16_t& ruResponseRejectedCode,
                                             OUT GO CHeaderList** ppResponseExtraHeaders);

    virtual bool CanHandlePacket(IN ESipMethod eMethod,
                                 IN const CSipPacket& rPacket);

    virtual bool IsSessionDialogNeeded(IN ISipSessionTransactionController::ESessionDialogTerminationType eType);

    virtual mxt_result Invite(IN ISipForkedDialogGrouperMgr* pForkedDialogGrouperMgr,
                              IN mxt_opaque opqTransaction,
                              IN TOA CHeaderList* pExtraHeaders,
                              IN TOA CSipMessageBody* pMessageBody,
                              OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result Ack(IN TOA CHeaderList* pExtraHeaders,
                           IN TOA CSipMessageBody* pMessageBody);

    virtual void Clear();

    //-- << CSipClientSvc >>

    virtual void GetRequestContext(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   OUT ISipRequestContext*& rpRequestContext);

    virtual void RequestContextInError(IN ISipRequestContext& rRequestContext);

    virtual void GetParentSipContext(OUT IPrivateSipContext*& rpSipContext);

    //-- << ISipReqCtxCoreSvc >>

    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    virtual void OnPacketReadyToSend(IN ISipRequestContext& rRequestContext,
                                     INOUT CSipPacket& rPacket);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    //-- << CEventDriven >>

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    //-- << Internal methods >>

    // Summary:
    //  Creates and initializes m_pRequestContext.
    void CreateRequestContext(IN mxt_opaque opqTransaction);

    // Summary:
    //  Processes the event triggered by the reception of a response.
    void ProcessEvent(IN ESipStatusClass eStatusClass);

    // Summary:
    //  Returns the value of the branch parameter of the Via header.
    static const CString* GetViaBranchValue(IN const CSipPacket& rPacket);

    // Summary:
    //  Sends the first ACK request through the transport.
    mxt_result SendUpdatedFirstAck(IN CSipPacket& rAck);

    // Summary:
    //  Completes pending actions, terminates this object and tells the
    //  controller that we are done.
    void Terminate();

    // Summary:
    //  Sets the specified state.
    void SetState(IN EObjectState eState);

    // Summary:
    //  Resets the specified state.
    void ResetState(IN EObjectState eState);

    // Summary:
    //  Tells if the specified state is set.
    bool IsStateSet(IN EObjectState eState) const;

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipSessionTransactionUacInvite();

    // Summary:
    //  Copy Constructor.
    CSipSessionTransactionUacInvite(IN const CSipSessionTransactionUacInvite& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipSessionTransactionUacInvite& operator=(IN const CSipSessionTransactionUacInvite& rSrc);


//-- Hidden Data Members
protected:
private:

    // This is a bitset of EObjectState values.
    int m_nObjectState;

    // When different from m_nESESSIONDIALOGTERMINATIONTYPE_INVALID_VALUE, it
    // means that IsSessionDialogNeeded has been called and that we returned
    // true. In this case, we must call
    // ISipSessionTransactionController::EvTerminateSessionDialog with the value
    // of this member when the session dialog is not needed anymore.
    int m_nIsSessionDialogNeededValue;

    // This is the value of the branch in the INVITE. It is only set when the
    // 2xx is received on the original context or when a response is received on
    // a supplemental context. It is deleted when the timer to accept 2xx
    // retransmissions fires. We keep this value to match the 2xx
    // retransmissions that might be received. We cannot use the 2xx response
    // from the m_pRequestContext because it will change when the ACK is sent.
    CString* m_pstrInviteBranchValue;

    // The request context from the UAC INVITE transaction.
    ISipRequestContext* m_pRequestContext;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  SetState
//==
//==============================================================================
//
//  Parameters:
//      eState:
//       The state to set.
//
//  Description:
//      Sets the specified state.
//
//==============================================================================
inline void CSipSessionTransactionUacInvite::SetState(IN EObjectState eState)
{
    m_nObjectState = m_nObjectState | static_cast<int>(eState);
}

//==============================================================================
//==
//==  ResetState
//==
//==============================================================================
//
//  Parameters:
//      eState:
//       The state to reset.
//
//  Description:
//      Resets the specified state.
//
//==============================================================================
inline void CSipSessionTransactionUacInvite::ResetState(IN EObjectState eState)
{
    m_nObjectState = m_nObjectState & (~static_cast<int>(eState));
}

//==============================================================================
//==
//==  IsStateSet
//==
//==============================================================================
//
//  Parameters:
//      eState:
//       The state to search for.
//
//  Returns:
//      true:
//       eState is set.
//
//      false:
//       eState is not set.
//
//  Description:
//      Tells if the specified state is set.
//
//==============================================================================
inline bool CSipSessionTransactionUacInvite::IsStateSet(IN EObjectState eState) const
{
    return (m_nObjectState & static_cast<int>(eState)) == static_cast<int>(eState);
}


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSIPSESSIONTRANSACTIONUACINVITE_H

