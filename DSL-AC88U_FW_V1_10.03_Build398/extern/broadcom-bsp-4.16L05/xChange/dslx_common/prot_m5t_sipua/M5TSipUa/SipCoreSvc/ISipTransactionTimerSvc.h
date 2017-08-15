//==============================================================================
//==============================================================================
//
//              Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPTRANSACTIONTIMERSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPTRANSACTIONTIMERSVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_ISIPTRANSACTIONMGR_H
#include "SipTransaction/ISipTransactionMgr.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_TIMER_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_TIMER_SVC_SUPPORT to be able to use \
this class.
#endif
MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(ISipTransactionTimerSvc);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipTransactionTimerSvc
//==============================================================================
// <GROUP SIPUSERAGENT_CLASSES>  
//
// Description:
//      This service allows to configure some of the SIP transaction timers
//      per transaction. These timers are passed to transaction at its 
//      creation by the context. 
//
//#The ISipTransactionTimerSvc is an ECOM object#
//
//  The ISipTransactionTimerSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipTransactionTimerSvc</B>
//
//  <B>Interface Id: IID_ISipTransactionTimerSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipTransactionTimerSvc.h
//
//==============================================================================
class ISipTransactionTimerSvc : public IEComUnknown
{

//-- Published Interface
public:

    //-- << Stereotype >>

    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipTransactionTimerSvc);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetT1
    //==
    //==========================================================================
    //
    //  Parameters:
    //      uT1Ms:
    //       The value to use for T1 in ms.
    //
    //  Description:
    //      Changes the initial retransmission interval for unreliable 
    //      transports per transaction.
    //      It corresponds to the T1 timer defined in section A at the end 
    //      of RFC 3261.
    //
    //      By default, it is set to uDEFAULT_T1_MS ms. Note that RFC 3261 
    //      allows implementations to lower or increase this value though it 
    //      does not recommend lowering it.
    //
    //      Note that when modifying this value, you also need to call SetT2 
    //      and SetTimeoutTimer unless you want to affect the number of 
    //      retransmissions.
    //
    //  See Also:
    //      SetT2
    //
    //==========================================================================
    virtual void SetT1(IN unsigned int uT1Ms) = 0;

    //==========================================================================
    //==
    //==  SetT2
    //==
    //==========================================================================
    //
    //  Parameters:
    //    uT2Ms:
    //      The value to use for T2 in ms.
    //
    //  Description:
    //      Changes the maximum retransmission interval for unreliable 
    //      transports per transaction.
    //      It corresponds to the T2 timer defined in section A at the 
    //      end of RFC 3261.
    //
    //      By default, it is set to uDEFAULT_T2_MS ms. Retransmissions of 
    //      certain types of SIP packets (namely final responses to INVITE and
    //      requests other than INVITE or ACK) are retransmitted at an initial
    //      rate of T1 ms, doubling each time until the interval reaches T2 ms.
    //      Then, the interval stays at T2 ms until the timeout timer starts or
    //      an ACK or response is received.
    //
    //      Note that when modifying this value, you also need to call SetT1 
    //      and SetTimeoutTimer unless you want to affect the number of 
    //      retransmissions.
    //
    //  See Also:
    //      SetT1
    //
    //==========================================================================
    virtual void SetT2(IN unsigned int uT2Ms) = 0;

    //==========================================================================
    //==
    //==  SetT4
    //==
    //==========================================================================
    //
    //  Parameters:
    //    uT4Ms:
    //      The value to use for T4 in ms.
    //
    //  Description:
    //      Changes the maximum duration a message remains in the network for 
    //      unreliable transports 
    //      per transaction. It corresponds to the T4 timer defined in section 
    //      A at the end of RFC 3261.      
    //
    //==========================================================================
    virtual void SetT4(IN unsigned int uT4Ms) = 0;

    //==========================================================================
    //==
    //==  SetTB
    //==
    //==========================================================================
    //
    //  Parameters:
    //    uTBMs:
    //      The value to use for Timer B in ms.
    //
    //  Description:
    //      Changes the INVITE transaction timeout per transaction.
    //      It corresponds to the Timer B defined in section A at the end of
    //      RFC 3261.
    //
    //==========================================================================
    virtual void SetTB(IN unsigned int uTBMs) = 0;

    //==========================================================================
    //==
    //==  SetTD
    //==
    //==========================================================================
    //
    //  Parameters:
    //    uTDMs:
    //      The value to use for Timer D in ms.
    //
    //  Description:
    //      Changes the wait time for response retransmission per transaction.
    //      It corresponds to the Timer D defined in section A at the end of
    //      RFC 3261.
    //
    //==========================================================================
    virtual void SetTD(IN unsigned int uTDMs) = 0;      

    //==========================================================================
    //==
    //==  GetT1
    //==
    //==========================================================================
    //
    //  Returns:
    //      The configured T1 value.
    //
    //  Description:
    //      Gets the initial retransmission interval for unreliable transports 
    //      per transaction. It corresponds to the T1 timer defined in section 
    //      A at the end of RFC 3261.
    //
    //      By default, it is set to uDEFAULT_T1_MS ms.
    //
    //  See Also:
    //      SetT1
    //
    //==========================================================================
    virtual unsigned int GetT1() = 0;

    //==========================================================================
    //==
    //==  GetT2
    //==
    //==========================================================================
    //
    //  Returns:
    //      The configured T2 value.
    //
    //  Description:
    //      Gets the maximum retransmission interval for unreliable transports 
    //      per transaction. It corresponds to the T2 timer defined in section 
    //      A at the end of RFC 3261.
    //
    //      By default, it is set to uDEFAULT_T2_MS ms.
    //
    //  See Also:
    //      SetT2
    //
    //==========================================================================
    virtual unsigned int GetT2() = 0;

    //==========================================================================
    //==
    //==  GetT4
    //==
    //==========================================================================
    //
    //  Returns:
    //      The configured T4 value.
    //
    //  Description:
    //      Gets the maximum duration a message remains in the network for 
    //      unreliable transports per transaction. It corresponds to the T4 
    //      timer defined in section A at the end of RFC 3261.
    //
    //      By default, it is set to uDEFAULT_T4_MS ms.
    //
    //  See Also:
    //      SetT4
    //
    //==========================================================================
    virtual unsigned int GetT4() = 0;

    //==========================================================================
    //==
    //==  GetTransactionTimers
    //==
    //==========================================================================
    //
    //  Returns:
    //      The pointer to transaction timers struct that contain the T1, T2  
    //      and T4 timers.
    //
    //  Description:
    //      Gets the transaction timers configured for the transactions created 
    //      by the context.
    //
    //==========================================================================
    virtual const ISipTransactionMgr::STransactionsTimers* GetTransactionTimers() = 0;

    //==========================================================================
    //==
    //==  GetTB
    //==
    //==========================================================================
    //
    //  Returns:
    //      The configured Timer B value.
    //
    //  Description:
    //      Gets the INVITE transaction timeout per transaction. 
    //      It corresponds to the timer B defined in section A at the end of
    //      RFC 3261.
    //
    //      By default, it is set to timeout timer.
    //
    //  See Also:
    //      SetTB
    //
    //==========================================================================
    virtual unsigned int GetTB() = 0;

    //==========================================================================
    //==
    //==  GetTD
    //==
    //==========================================================================
    //
    //  Returns:
    //      The configured Timer D value.
    //
    //  Description:
    //      Gets the wait time for response retransmission per transaction. 
    //      It corresponds to the timer D defined in section A at the end of
    //      RFC 3261.
    //
    //      By default, it is set to timeout timer.
    //
    //  See Also:
    //      SetTD
    //
    //==========================================================================
    virtual unsigned int GetTD() = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipTransactionTimerSvc() {};

    // Summary:
    //  Destructor.
    virtual ~ISipTransactionTimerSvc() {};
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipTransactionTimerSvc(IN const ISipTransactionTimerSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipTransactionTimerSvc& operator=(IN const ISipTransactionTimerSvc& rSrc);
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTRANSACTIONTIMERSVC_H

