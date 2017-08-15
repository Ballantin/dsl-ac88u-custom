//==============================================================================
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
//==============================================================================
#ifndef MXG_CSIPSERVERINVITETRANSACTION_H
#define MXG_CSIPSERVERINVITETRANSACTION_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSIPPACKET_H
#include "SipTransport/CSipPacket.h"
#endif

// ISipTransactionMgr::ETransactionRole
#ifndef MXG_ISIPTRANSACTIONMGR_H
#include "SipTransaction/ISipTransactionMgr.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSIPTRANSACTION_H
#include "SipTransaction/CSipTransaction.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipServerInviteTransaction
//========================================
//
// Description:
//   This class partially implements the INVITE server transaction as per
//   RFC 3261, section 17.2.1, figure 7 (TU stands for Transaction User).
//
//   The major tweak from RFC 3261 is the addition of the Succeeded state.
//   Consequent to the reception of a 2xx on the Proceeding state, the machine
//   transits to the Succeeded state instead of the Terminated state.
//
//   Also note that the Transaction User never sends the 100 Trying. It is
//   always done by the transaction 200 ms after receiving the inital INVITE
//   request.
//
//  <CODE>
//
//                    |INVITE
//                    |pass INV to TU
// INVITE             V
// send response+-----------+
//     +--------|           |--------+101-199 from TU
//     |        | Proceeding|        |send response
//     +------->|           |<-------+
//              |           |
// send 100     |           |
// after 200ms  |           |  2xx from TU
//     +--------|           |  send response                         INVITE
//     |        |           |----------------------->+-------------+ Do nothing
//     +------->+-----------+                        |             |----------+
// 300-699 from TU |     |Transport Err.             | Succeeded   |          |
// send response   |     |Inform TU                  |             |<---------+
//                 |     +--------------------->+    +-------------+
//                 |                            |           |
// INVITE          V          Timer G fires     |           | Timer H fires
// send response+-----------+ send response     |           | Inform TU
//     +--------|           |--------+          +<----------+
//     |        | Completed |        |          |
//     +------->|           |<-------+          |
//              +-----------+                   |
//                 |     |                      |
//             ACK |     |                      |
//             -   |     +--------------------->+
//                 |        Timer H fires       |
//                 V        or Transport Err.   |
//              +-----------+  Inform TU        |
//              |           |                   |
//              | Confirmed |                   |
//              |           |                   |
//              +-----------+                   |
//                    |                         |
//                    |Timer I fires            |
//                    |-                        |
//                    |                         |
//                    V                         |
//              +-----------+                   |
//              |           |                   |
//              | Terminated|<------------------+
//              |           |
//              +-----------+
//
//  </CODE>
//
// Location:
//   SipTransaction/CSipServerInviteTransaction.h
//
// See Also:
//
//==EDOC========================================================================
class CSipServerInviteTransaction : public CSipTransaction
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


protected:
    // This enum encompasses all possible states for server INVITE state
    // machine defined in RFC 3261, section 17. The states are mapping with
    // RFC 3261 state naming (except for ePREINIT, eSUCCEEDED and eALL).
    //----------------------------------------------------------------------
    enum EState
    {
        ePREINIT,    // used to trap unitialized state machines
        ePROCEEDING,
        eSUCCEEDED,
        eCOMPLETED,
        eCONFIRMED,
        eTERMINATED,
        eAllStates   // always last
    };

    // This enum includes all timers needed by server INVITE state machine
    // defined in RFC 3261, section 17. The timers are mapping with RFC 3261
    // timer naming.
    //-------------------------------------------------------------------------
    enum ETimerId
    {
        eTimerG,
        eTimerH,
        eTimerI,
        // This is the 200-ms timer prior to sending the 100 Trying.
        //-----------------------------------------------------------
        eTimerTrying
    };


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipServerInviteTransaction(
               IN ISipTransactionUser* pTransactionUser,
               IN const CSipPacket& rPacket,
               IN ISipTransactionMgr::ETransactionRole eMergedRequestDetection,
               IN ISipTransactionStatistics* pTransStats,
               IN const ISipTransactionMgr::STransactionsTimers* pstTransactionTimers);


    //-- << CSipTransaction >>.
    //----------------------------

    // Summary:
    //  Sends the packet on the network and makes the state machine transit.
    //-----------------------------------------------------------------------
    mxt_result Send(IN const CSipPacket& rPacket,
                    IN bool bNotifyTransactionUser,
                    IN bool bRetransmission,
                    IN bool bSourceIsTransaction);

    // Summary:
    //  Sets the TimerH timeout.
    //---------------------------
    static void SetTimerH(unsigned int uTimeoutMs);

    // Summary:
    //  Gets the TimerH timeout.
    //---------------------------
    static unsigned int GetTimerH();

// BRCM_CUSTOM - [begin] Control branch-id matching choices in ACK
#ifndef MXD_SIPSTACK_ENABLE_SERVER_INVITE_SAME_BRANCH_ACK
    // Summary:
    //  Sets the variable for bypassing the branch-id checking in ACK.
    //-----------------------------------------------------------------
    static void SetBypassAckBranchIdCheck(IN bool bBypass);
#endif
// BRCM_CUSTOM - [end] Control branch-id matching choices in ACK

//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipServerInviteTransaction();

    // Summary:
    //  Entry point to notify the state machine of a packet reception.
    //-----------------------------------------------------------------
    virtual void InternalEvProcessPacketA(IN const CSipPacket& rPacket);

    // Summary:
    //  Gives feedback to the Transaction User upon sending result.
    //--------------------------------------------------------------
    virtual void InternalEvCommandResultA(IN mxt_result res,
                                          IN const SAsyncTransportData* pstAsyncData);

    //  Summary:
    //   This method updates the state machine IAW the requested transition.
    //-----------------------------------------------------------------------
    mxt_result ChangeState(IN const EState eState);

    // Summary:
    //  Sets the original state for a server transaction.
    //----------------------------------------------------
    virtual void ProcessOriginalRequest(IN const CSipPacket& rPacket);

    virtual void EvTransportError(IN mxt_result res);

    //-- << CEventDriven >>
    //-----------------------

    // Summary:
    //  Entry point for all timers events.
    //-------------------------------------
    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    // Summary:
    //  This method starts the cleanup of its Transaction User.
    //----------------------------------------------------------
    virtual void ReleasionFirstStep();

    //  Summary:
    //   This method constructs and sends the 100 Trying response.
    //-------------------------------------------------------------
    void SendTrying(IN const CSipPacket* pRequest);

    // Summary:
    //  Tells whether or not the packet matches the server transaction
    //  according to RFC 2543 matching rules.
    //-----------------------------------------------------------------
    virtual bool MatchRfc2543Helper(IN const CSipPacket& rPacket);

    // Summary:
    //  Tells whether or not the packet matches the server transaction
    //  according to RFC 3261 matching rules.
    //-----------------------------------------------------------------
    virtual bool MatchRfc3261ServerHelper(IN const CSipPacket& rPacket,
                                          IN const CString* pstrTopViaBranch);

private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipServerInviteTransaction();

    static void FinalizeCSipServerInviteTransaction();


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipServerInviteTransaction(IN const CSipServerInviteTransaction& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipServerInviteTransaction& operator=(
                                    IN const CSipServerInviteTransaction& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    // This is the INVITE packet that creates this server transaction. It is
    // used to create the 100 Trying response.
    //-----------------------------------------------------------------------
    const CSipPacket* m_pOriginalPacket;

    // This is the state machine's current state. This variable MUST be
    // accessed synchronously when writing with the pStateMutex variable.
    //---------------------------------------------------------------------
    EState m_eState;

    // Protects the state
    //--------------------
    static CMutex* ms_pStateMutex;

    // The state names.
    //------------------
    static const char* const ms_aszStateNames[];

    // The timer names.
    //------------------
    static const char* const ms_aszTimerNames[];

    // Timer H timeout.
    //------------------
    static unsigned int ms_uTimerHMs;

// BRCM_CUSTOM - [begin] Control branch-id matching choices in ACK
#ifndef MXD_SIPSTACK_ENABLE_SERVER_INVITE_SAME_BRANCH_ACK
    // This variable can be used to bypass the branch-id checking in ACK.
    //
    // IMPORTANT : This variable MUST NOT be used to be RFC 3261 compliant (RFC3261
    // section 8.1.1.7, third paragraph).
    //
    // Setting this variable to false bypasses the checking of branch-id in the
    // received ACK and allows the received ACK and its corresponding INVITE having
    // the same branch-id in their Via headers.
    //------------------------------------------------------------------------------
    static bool ms_bCheckAckBranchIdPerRfc3261;
#endif
// BRCM_CUSTOM - [end] Control branch-id matching choices in ACK
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  ProcessOriginalRequest
//==
//==============================================================================
//
//  Parameters:
//      rPacket:
//       The received packet that needs to be dealt within the state machine.
//
//  Description:
//      Sets the original state for a server transaction.
//
//==EDOC========================================================================
inline void CSipServerInviteTransaction::ProcessOriginalRequest(
                                                   IN const CSipPacket& rPacket)
{
    // Because of the fact that InternalEvProcessPacketA can be called from a
    // synchronous or asynchronous context, count a reference on the packet as
    // it will be released before InternalEvProcessPacketA terminates its
    // execution.
    //-------------------------------------------------------------------------
    rPacket.AddRef();
    InternalEvProcessPacketA(rPacket);
}

//==SDOC========================================================================
//==
//==  SetTimerH
//==
//==============================================================================
//
//  Parameters:
//    uTimeoutMs:
//      The Timer H timeout value to set.
//
//  Description:
//      This method overrides the Timer H timeout value. If the value is 0, the 
//      default behaviour is used when starting the timer.
//
//==EDOC========================================================================
inline void CSipServerInviteTransaction::SetTimerH(unsigned int uTimeoutMs)
{
    ms_uTimerHMs = uTimeoutMs;
}

//==SDOC========================================================================
//==
//==  GetTimerH
//==
//==============================================================================
//
//  Returns:
//      The Timer H timeout value.
//
//  Description:
//      This method returns the overridden Timer H timeout value. If the value
//      is 0, the default behaviour is used when starting the timer.
//
//==EDOC========================================================================
inline unsigned int CSipServerInviteTransaction::GetTimerH()
{
    return ms_uTimerHMs;
}

// BRCM_CUSTOM - [begin] Control branch-id matching choices in ACK
#ifndef MXD_SIPSTACK_ENABLE_SERVER_INVITE_SAME_BRANCH_ACK
inline void CSipServerInviteTransaction::SetBypassAckBranchIdCheck(IN bool bBypass)
{
    ms_bCheckAckBranchIdPerRfc3261 = !bBypass;
}
#endif
// BRCM_CUSTOM - [end] Control branch-id matching choices in ACK

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSERVERINVITETRANSACTION_H

