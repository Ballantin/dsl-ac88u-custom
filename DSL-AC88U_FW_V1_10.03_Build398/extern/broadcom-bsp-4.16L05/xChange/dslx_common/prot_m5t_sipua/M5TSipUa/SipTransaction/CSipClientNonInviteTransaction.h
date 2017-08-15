//==============================================================================
//==============================================================================
//
//      Copyright(c) 2004-2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_CSIPCLIENTNONINVITETRANSACTION_H
#define MXG_CSIPCLIENTNONINVITETRANSACTION_H

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
//== Class: CSipClientNonInviteTransaction
//========================================
//
// Description:
//   This class implements the non-INVITE client transaction as per RFC 3261,
//   section 17.1.2, figure 6 (TU stands for Transaction User).
//
//  <CODE>
//
//                                 |Request from TU
//                                 |send request
//             Timer E             V
//             send request  +-----------+
//                 +---------|           |-------------------+
//                 |         |  Trying   |  Timer F          |
//                 +-------->|           |  or Transport Err.|
//                           +-----------+  inform TU        |
//              200-699         |  |                         |
//              resp. to TU     |  |1xx                      |
//              +---------------+  |resp. to TU              |
//              |                  |                         |
//              |   Timer E        V       Timer F           |
//              |   send req +-----------+ or Transport Err. |
//              |  +---------|           | inform TU         |
//              |  |         |Proceeding |------------------>|
//              |  +-------->|           |-----+             |
//              |            +-----------+     |1xx          |
//              |              |      ^        |resp to TU   |
//              | 200-699      |      +--------+             |
//              | resp. to TU  |                             |
//              |              |                             |
//              |              V                             |
//              |            +-----------+                   |
//              |            |           |                   |
//              |            | Completed |                   |
//              |            |           |                   |
//              |            +-----------+                   |
//              |              ^   |                         |
//              |              |   | Timer K                 |
//              +--------------+   | -                       |
//                                 |                         |
//                                 V                         |
//           NOTE:           +-----------+                   |
//                           |           |                   |
//       transitions         | Terminated|<------------------+
//       labeled with        |           |
//       the event           +-----------+
//       over the action
//       to take
//  </CODE>
//
// Location:
//   SipTransaction/CSipClientNonInviteTransaction.h
//
// See Also:
//
//==============================================================================
class CSipClientNonInviteTransaction : public CSipTransaction
{
//-- Friend Declarations
//------------------------

protected:
    // This enum encompasses all possible states for client non-INVITE state
    // machine defined in RFC-3261, section 17. The states are mapping with
    // RFC-3261 state naming (except for ePREINIT and eALL).
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

    // This enum includes all timers needed by client non-INVITE state machine
    // defined in RFC-3261, section 17. The timers are mapping with RFC-3261
    // timer naming.
    //-------------------------------------------------------------------------
    enum ETimerId
    {
        eTimerE,
        eTimerF,
        eTimerK
    };


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipClientNonInviteTransaction(IN ISipTransactionUser* pTransactionUser,
                                   IN const CSipPacket&    rPacket,
                                   IN ESipMethod           eMethod,
                                   IN ISipTransactionStatistics* pTransStats,
                                   IN const ISipTransactionMgr::STransactionsTimers* pstTransactionTimers);

    //-- << CSipTransaction >>.
    //----------------------------

    // Summary:
    //  Sends the packet on the network and makes the state machine to transit.
    //--------------------------------------------------------------------------
    mxt_result Send(IN const CSipPacket& rPacket,
                    IN bool bNotifyTransactionUser,
                    IN bool bRetransmission,
                    IN bool bSourceIsTransaction);

    // Summary:
    //  Sets the TimerF timeout.
    //---------------------------
    static void SetTimerF(unsigned int uTimeoutMs);

    // Summary:
    //  Gets the TimerF timeout.
    //---------------------------
    static unsigned int GetTimerF();

//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipClientNonInviteTransaction();

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
    CSipClientNonInviteTransaction(IN const CSipClientNonInviteTransaction& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipClientNonInviteTransaction& operator=(IN const CSipClientNonInviteTransaction& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    // This is the state machine's current state.
    //--------------------------------------------
    EState m_eState;

    // The state names.
    //------------------
    static const char* const ms_aszStateNames[];

    // The timer names.
    //------------------
    static const char* const ms_aszTimerNames[];

    // Timer F timeout.
    //------------------
    static unsigned int ms_uTimerFMs;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  SetTimerF
//==
//==============================================================================
//
//  Parameters:
//    uTimeoutMs:
//      The timer F timeout value to set.
//
//  Description:
//      This method overrides the timer F timeout value. If the value is 0, the 
//      default behaviour is used when starting the timer.
//
//==EDOC========================================================================
inline void CSipClientNonInviteTransaction::SetTimerF(unsigned int uTimeoutMs)
{
    ms_uTimerFMs = uTimeoutMs;
}

//==SDOC========================================================================
//==
//==  GetTimerF
//==
//==============================================================================
//
//  Returns:
//      The timer F timeout value.
//
//  Description:
//      This method returns the overridden timer F timeout value. If the value
//      is 0, the default behaviour is used when starting the timer.
//
//==EDOC========================================================================
inline unsigned int CSipClientNonInviteTransaction::GetTimerF()
{
    return ms_uTimerFMs;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCLIENTNONINVITETRANSACTION_H

