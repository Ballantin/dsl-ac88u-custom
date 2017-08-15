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
#ifndef MXG_CSIPSESSIONTRANSACTIONUACBYE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSESSIONTRANSACTIONUACBYE_H
//M5T_INTERNAL_USE_END

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

#ifndef MXG_STATUSCODE_H
#include "SipParser/StatusCode.h" // ESipStatusClass
#endif

#ifndef MXG_CSIPCLIENTSVC_H
#include "SipUserAgent/CSipClientSvc.h"
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
//== Class: CSipSessionTransactionUacBye
//==============================================================================
//<GROUP SipUserAgent>
//
// Summary:
//   The transaction used to handle a client BYE request.
//
// Description:
//  Handles a client BYE transaction from the sending of the BYE to the
//  reception of the final response.
//
//  The responsibilities of this class are:
//  - Manage the client transaction state
//  - Report events related to such transaction on the ISipSessionMgr
//  - Manage the ISipUserAgentSvc
//  - Create and manage the ISipRequestContext of the BYE  for its lifetime
//  - Report the dialog state changes to the ISipSessionTransactionController
//
// Location:
//   SipUserAgent/CSipSessionTransactionUacBye.h
//
// See Also:
//   CSipSessionTransaction, CSipSessionSvc
//
//==============================================================================
class CSipSessionTransactionUacBye : private CSipClientSvc,
                                     private CSipSessionTransaction
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

    enum EObjectState
    {
        // Description:
        //  Clear has been called on the object. This value is there to avoid
        //  calling EvTransactionTerminated again on the controller if/when a
        //  final response is received.
        eSTATE_CLEARED = 1,

        // Description:
        //  A final response has been received. When in this state, the request
        //  can be re-issued.
        eSTATE_TERMINATED = eSTATE_CLEARED * 2
    };

//-- Published Interface
public:

    // Summary:
    //  Sets the received INVITE rejection code.
    static void SetReceivedInviteRejectResponse(IN uint16_t uRejectCode);

    //-- << Startup mechanism >>

    static mxt_result InitializeCSipSessionTransactionUacBye();

    static void FinalizeCSipSessionTransactionUacBye();

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
    CSipSessionTransactionUacBye(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipSessionTransactionUacBye();

    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //-- << CEComDelegatingUnknown >>

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << ISipSessionTransaction >>

    virtual mxt_result AllowSendingRequest(IN ESipMethod eMethod);

    virtual mxt_result AllowReceivingRequest(IN ESipMethod eMethod,
                                             IN const CSipPacket& rPacket,
                                             OUT uint16_t& ruResponseRejectedCode,
                                             OUT GO CHeaderList** ppResponseExtraHeaders);

    virtual mxt_result Bye(IN mxt_opaque opqTransaction,
                           IN TOA CHeaderList* pExtraHeaders,
                           IN TOA CSipMessageBody* pMessageBody,
                           OUT ISipClientTransaction*& rpTransaction);

    virtual void Clear();

    //-- << CSipClientSvc >>

    virtual void GetRequestContext(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   OUT ISipRequestContext*& rpRequestContext);

    virtual void RequestContextInError(IN ISipRequestContext& rRequestContext);

    virtual void GetParentSipContext(OUT IPrivateSipContext*& rpSipContext);

    //-- << ISipReqCtxCoreSvc >>

    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    //-- << Internal methods >>

    // Summary:
    //  Processes the event triggered by the reception of a response.
    void ProcessEvent(IN ESipStatusClass eStatusClass,
                      IN ISipRequestContext& rRequestContext);

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
    CSipSessionTransactionUacBye();

    // Summary:
    //  Copy Constructor.
    CSipSessionTransactionUacBye(IN const CSipSessionTransactionUacBye& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipSessionTransactionUacBye& operator=(IN const CSipSessionTransactionUacBye& rSrc);


//-- Hidden Data Members
protected:
private:

    // This is a bitset of EObjectState.
    int m_nObjectState;

    // The INVITE rejection code.
    static uint16_t ms_uInviteRejectCode;
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
inline void CSipSessionTransactionUacBye::SetState(IN EObjectState eState)
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
inline void CSipSessionTransactionUacBye::ResetState(IN EObjectState eState)
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
inline bool CSipSessionTransactionUacBye::IsStateSet(IN EObjectState eState) const
{
    return (m_nObjectState & static_cast<int>(eState)) == static_cast<int>(eState);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSESSIONTRANSACTIONUACBYE_H

