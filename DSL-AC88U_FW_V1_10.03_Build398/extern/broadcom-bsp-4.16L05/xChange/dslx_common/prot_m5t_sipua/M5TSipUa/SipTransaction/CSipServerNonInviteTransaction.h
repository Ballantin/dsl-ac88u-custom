//==============================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2010 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_CSIPSERVERNONINVITETRANSACTION_H
#define MXG_CSIPSERVERNONINVITETRANSACTION_H

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

//==============================================================================
//== Class: CSipServerNonInviteTransaction
//========================================
//
// Description:
//   This class implements the non-INVITE server transaction as per RFC 3261,
//   section 17.2.2, figure 8 (TU stands for Transaction User).
//
//  <CODE>
//
//                                |Request received
//                                |pass to TU
//                                V
//                          +-----------+
//                          |           |
//                          | Trying    |-------------+
//                          |           |             |
//                          +-----------+             |200-699 from TU
//                                |                   |send response
//                                |1xx from TU        |
//                                |send response      |
//                                |                   |
//             Request            V      1xx from TU  |
//             send response+-----------+send response|
//                 +--------|           |--------+    |
//                 |        | Proceeding|        |    |
//                 +------->|           |<-------+    |
//          +<--------------|           |             |
//          |Trnsprt Err    +-----------+             |
//          |Inform TU            |                   |
//          |                     |                   |
//          |                     |200-699 from TU    |
//          |                     |send response      |
//          |  Request            V                   |
//          |  send response+-----------+             |
//          |      +--------|           |             |
//          |      |        | Completed |<------------+
//          |      +------->|           |
//          +<--------------|           |
//          |Trnsprt Err    +-----------+
//          |Inform TU            |
//          |                     |Timer J fires
//          |                     |-
//          |                     |
//          |                     V
//          |               +-----------+
//          |               |           |
//          +-------------->| Terminated|
//                          |           |
//                          +-----------+
//
//  </CODE>
//
// Location:
//   SipTransaction/CSipServerNonInviteTransaction.h
//
// See Also:
//
//==============================================================================
class CSipServerNonInviteTransaction : public CSipTransaction
{
//-- Friend Declarations
//------------------------

protected:
    // This enum encompasses all possible states for server non-INVITE state
    // machine defined in RFC 3261, section 17. The states are mapping with
    // RFC 3261 state naming (except for ePREINIT and eALL).
    //-----------------------------------------------------------------------
    enum EState
    {
        ePREINIT,    // used to trap unitialized state machines
        eTRYING,
        ePROCEEDING,
        eCOMPLETED,
        eTERMINATED,
        eAllStates   // always last
    };


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipServerNonInviteTransaction(
               IN ISipTransactionUser* pTU,
               IN const CSipPacket& rPacket,
               IN ESipMethod eMethod,
               IN ISipTransactionMgr::ETransactionRole eMergedRequestDetection,
               IN ISipTransactionStatistics* pTransStats,
               IN const ISipTransactionMgr::STransactionsTimers* pstTransactionTimers);


    //-- << CSipTransaction >>
    //----------------------------

    // Summary:
    //  Sends the packet on the network and makes the state machine transit.
    //-----------------------------------------------------------------------
    mxt_result Send(IN const CSipPacket& rPacket,
                    IN bool bNotifyTransactionUser,
                    IN bool bRetransmission,
                    IN bool bSourceIsTransaction);

    // Summary:
    //  Sets the TimerJ timeout.
    //---------------------------
    static void SetTimerJ(unsigned int uTimeoutMs);

    // Summary:
    //  Gets the TimerJ timeout.
    //---------------------------
    static unsigned int GetTimerJ();

//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipServerNonInviteTransaction();

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
    //--------------------------------------
    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    // Summary:
    //  This method starts the cleanup of its Transaction User.
    //----------------------------------------------------------
    virtual void ReleasionFirstStep();

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipServerNonInviteTransaction(
                                 IN const CSipServerNonInviteTransaction& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipServerNonInviteTransaction& operator=(
                                 IN const CSipServerNonInviteTransaction& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    // This is the state machine's current state.
    //--------------------------------------------
    EState m_eState;

    // This is the ID associated with Timer J.
    //-----------------------------------------
    static const unsigned int ms_uTimerJId;

    // The state names.
    //------------------
    static const char* const ms_aszStateNames[];

    // The name for Timer J.
    //-----------------------
    static const char* const ms_szTimerJ;

    // Timer J timeout.
    //------------------
    static unsigned int ms_uTimerJMs;
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
//      This sets the original state for a server transaction.
//
//==EDOC========================================================================
inline void CSipServerNonInviteTransaction::ProcessOriginalRequest(
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
//==  SetTimerJ
//==
//==============================================================================
//
//  Parameters:
//    uTimeoutMs:
//      The timer J timeout value to set.
//
//  Description:
//      This method overrides the timer J timeout value. If the value is 0, the 
//      default behaviour is used when starting the timer.
//
//==EDOC========================================================================
inline void CSipServerNonInviteTransaction::SetTimerJ(unsigned int uTimeoutMs)
{
    ms_uTimerJMs = uTimeoutMs;
}

//==SDOC========================================================================
//==
//==  GetTimerJ
//==
//==============================================================================
//
//  Returns:
//      The timer J timeout value.
//
//  Description:
//      This method returns the overridden timer J timeout value. If the value
//      is 0, the default behaviour is used when starting the timer.
//
//==EDOC========================================================================
inline unsigned int CSipServerNonInviteTransaction::GetTimerJ()
{
    return ms_uTimerJMs;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSERVERNONINVITETRANSACTION_H

