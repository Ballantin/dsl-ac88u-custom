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
#ifndef MXG_CSIPTRANSACTIONTIMERSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPTRANSACTIONTIMERSVC_H
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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_TIMER_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_TIMER_SVC_SUPPORT to be able \
to use this class.
#endif

#ifndef MXG_ISIPTRANSACTIONTIMERSVC_H
#include "SipCoreSvc/ISipTransactionTimerSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
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
//== Class: CSipTransactionTimerSvc
//==============================================================================
//
// Description:
//   This is the implementation of the ISipTransactionTimerSvc interface. 
//
// Location:
//   SipUserAgent/CSipTransactionTimerSvc.h
//
// See Also:
//   ISipTransactionTimerSvc
//
//==============================================================================
class CSipTransactionTimerSvc : private CEComDelegatingUnknown,
                                private ISipTransactionTimerSvc,
                                private ISipCoreSvc,
                                private CSipReqCtxCoreSvc
{
//-- Friend Declarations
    friend class CSipStackInitializer;

//-- New types within class namespace
public:

    
//-- Published Interface  
    //-- << CEComUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested, OUT void** ppInterface);

//-- Hidden Methods
protected:
private:

    //-- << Constructors / Destructors / Operators >>
    // Summary:
    //  Default Constructor.
    CSipTransactionTimerSvc();

    // Summary:
    //  Default Constructor.
    CSipTransactionTimerSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipTransactionTimerSvc();

    //-- << ISipTransactionTimerSvc >>

    virtual void SetT1(IN unsigned int uT1Ms);
    virtual unsigned int GetT1();

    virtual void SetT2(IN unsigned int uT2Ms);
    virtual unsigned int GetT2();

    virtual void SetT4(IN unsigned int uT4Ms);
    virtual unsigned int GetT4();

    virtual void SetTB(IN unsigned int uTBMs);
    virtual unsigned int GetTB();

    virtual void SetTD(IN unsigned int uTDMs);
    virtual unsigned int GetTD();

    virtual const ISipTransactionMgr::STransactionsTimers* GetTransactionTimers();
  
    //-- << ISipCoreSvc >>
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                        IN ISipCoreSvc::EDirection eDirection);

    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    virtual void GetOwnerInstance(IN const CSipPacket& rPacket, OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    virtual mxt_result ClearCoreSvc();

    //-- << ISipReqCtxCoreSvc Interface >>
    virtual unsigned int AddRcsRef();    
    virtual unsigned int ReleaseRcsRef();
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext, INOUT CSipPacket& rPacket);

   
    //-- << Startup mechanism >>
    static mxt_result InitializeCSipTransactionTimerSvc();

    static void FinalizeCSipTransactionTimerSvc();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown, OUT CEComUnknown** ppCEComUnknown);

    //-- << Helpers >>

   
//-- Hidden Data Members
protected:
private:

    // The current current transaction timers struct.
    ISipTransactionMgr::STransactionsTimers  m_stTransactionTimers;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================



MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPTRANSACTIONTIMERSVC_H

