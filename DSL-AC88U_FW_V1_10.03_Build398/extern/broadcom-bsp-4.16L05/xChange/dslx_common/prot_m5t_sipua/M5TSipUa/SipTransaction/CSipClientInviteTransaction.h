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
#ifndef MXG_CSIPCLIENTINVITETRANSACTION_H
#define MXG_CSIPCLIENTINVITETRANSACTION_H

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
//== Class: CSipClientInviteTransaction
//========================================
//
// Description:
//   This class implements the client-invite transaction state machine as per
//   RFC 3261, figure 5 (TU stands for Transaction User):
//
//  <CODE>
//                               |INVITE from TU
//             Timer A fires     |INVITE sent
//             Reset A,          V                      Timer B fires
//             INVITE sent +-----------+                or Transport Err.
//               +---------|           |---------------+inform TU
//               |         |  eCALLING |               |
//               +-------->|           |-------------->|
//                         +-----------+ 2xx           |
//                            |  |       2xx to TU     |
//                            |  |1xx                  |
//    300-699 +---------------+  |1xx to TU            |
//   ACK sent |                  |                     |
//resp. to TU |  1xx             V                     |
//            |  1xx to TU  -----------+               |
//            |  +---------|           |               |
//            |  |         |ePROCEEDING|-------------->|
//            |  +-------->|           | 2xx           |
//            |            +-----------+ 2xx to TU     |
//            |       300-699    |                     |
//            |       ACK sent,  |                     |
//            |       resp. to TU|                     |
//            |                  |                     |      NOTE:
//            |  300-699         V                     |
//            |  ACK sent  +-----------+Transport Err. |  transitions
//            |  +---------|           |Inform TU      |  labeled with
//            |  |         | eCOMPLETED|-------------->|  the event
//            |  +-------->|           |               |  over the action
//            |            +-----------+               |  to take
//            |              ^   |                     |
//            |              |   | Timer D fires       |
//            +--------------+   | -                   |
//                               |                     |
//                               V                     |
//                         +-----------+               |
//                         |           |               |
//                         |eTERMINATED|<--------------+
//                         |           |
//                         +-----------+
//  </CODE>
//
// Location:
//   SipTransaction/CSipClientInviteTransaction.h
//
// See Also:
//
//==============================================================================
class CSipClientInviteTransaction : public CSipTransaction
{
//-- Friend Declarations
//------------------------

protected:
    // This enum encompasses all possible states for client INVITE state
    // machine defined in RFC 3261, section 17. The states are mapping with
    // RFC 3261 state naming (except for ePREINIT and eALL).
    //----------------------------------------------------------------------
    enum EState
    {
        ePREINIT,    // used to trap unitialized state machines
        eCALLING,
        ePROCEEDING,
        eCOMPLETED,
        eTERMINATED,
        eAllStates   // always last
    };

    // This enum includes all timers needed by client INVITE state machine
    // defined in RFC 3261, section 17. The timers are mapping with RFC 3261
    // timer naming.
    //-----------------------------------------------------------------------
    enum ETimerId
    {
        eTimerA,
        eTimerB,
        eTimerD
    };


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    // Constructor.
    //--------------
    CSipClientInviteTransaction(IN ISipTransactionUser* pTU,
                                IN const CSipPacket& rPacket,
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

    virtual mxt_result StopRetransmission();

    // Summary:
    //  Sets the TimerB timeout.
    //---------------------------
    static void SetTimerB(unsigned int uTimeoutMs);

    // Summary:
    //  Gets the TimerB timeout.
    //---------------------------
    static unsigned int GetTimerB();

    // Summary:
    //  Sets the TimerD timeout.
    //---------------------------
    static void SetTimerD(unsigned int uTimeoutMs);

    // Summary:
    //  Gets the TimerD timeout.
    //---------------------------
    static unsigned int GetTimerD();

//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CSipClientInviteTransaction();

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
    //   This method constructs and sends the ACK request for negative final
    //   responses.
    //-----------------------------------------------------------------------
    mxt_result SendAck(IN const CSipPacket& rResponse, IN bool bAckRetransmission);

    // Summary:
    //  This method automatically sends an ACK to final negative response,
    //  reports EvRequestFailed to the application, and changes the state of
    //  the transaction to eCOMPLETED.
    //-----------------------------------------------------------------------
    void EvRequestFailedHelper(IN const CSipPacket& rPacket);

    //  Summary:
    //   This method updates the state machine IAW the requested transition.
    //-----------------------------------------------------------------------
    mxt_result ChangeState(IN const EState eState);

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

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipClientInviteTransaction(IN const CSipClientInviteTransaction& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipClientInviteTransaction& operator=(IN const CSipClientInviteTransaction& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    // This is the original INVITE sent that created the transaction.
    //----------------------------------------------------------------
    const CSipPacket* m_pOriginalRequest;

    // This is the state machine's current state.
    //--------------------------------------------
    EState m_eState;

    // The current value used for Timer B.
    unsigned int m_uTBMs;

    // The current value used for Timer D.
    unsigned int m_uTDMs;

    // The global value used for TB.
    static unsigned int ms_uGlobalTBMs;

    // The global value used for TD.
    static unsigned int ms_uGlobalTDMs;

    // The state names.
    //------------------
    static const char* const ms_aszStateNames[];

    // The timer names.
    //------------------
    static const char* const ms_aszTimerNames[];
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  SetTimerB
//==
//==============================================================================
//
//  Parameters:
//    uTimeoutMs:
//      The timer B timeout value to set.
//
//  Description:
//      This method overrides the timer B timeout value. If the value is 0, the 
//      default behaviour is used when starting the timer.
//
//==EDOC========================================================================
inline void CSipClientInviteTransaction::SetTimerB(unsigned int uTimeoutMs)
{
    ms_uGlobalTBMs = uTimeoutMs;
}

//==SDOC========================================================================
//==
//==  GetTimerB
//==
//==============================================================================
//
//  Returns:
//      The timer B timeout value.
//
//  Description:
//      This method returns the overridden timer B timeout value. If the value
//      is 0, the default behaviour is used when starting the timer.
//
//==EDOC========================================================================
inline unsigned int CSipClientInviteTransaction::GetTimerB()
{
    return ms_uGlobalTBMs;
}

//==SDOC========================================================================
//==
//==  SetTimerD
//==
//==============================================================================
//
//  Parameters:
//    uTimeoutMs:
//      The timer D timeout value to set.
//
//  Description:
//      This method overrides the timer D timeout value. If the value is 0, the 
//      default behaviour is used when starting the timer.
//
//==EDOC========================================================================
inline void CSipClientInviteTransaction::SetTimerD(unsigned int uTimeoutMs)
{
    ms_uGlobalTDMs = uTimeoutMs;
}

//==SDOC========================================================================
//==
//==  GetTimerD
//==
//==============================================================================
//
//  Returns:
//      The timer D timeout value.
//
//  Description:
//      This method returns the overridden timer D timeout value. If the value
//      is 0, the default behaviour is used when starting the timer.
//
//==EDOC========================================================================
inline unsigned int CSipClientInviteTransaction::GetTimerD()
{
    return ms_uGlobalTDMs;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCLIENTINVITETRANSACTION_H

