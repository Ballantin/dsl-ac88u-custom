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
#ifndef MXG_CSIPSERVERMONITORSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSERVERMONITORSVC_H

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

#if defined(MXD_SIPSTACK_ENABLE_SIP_SERVER_MONITOR_SVC_SUPPORT)

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPSERVERMONITORSVC_H
#include "SipCoreSvc/ISipServerMonitorSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipServerMonitorSvc
//==============================================================================
//
// Description:
//   This class implements the service that need to be attached to a context for
//   it to be notified of the communication status with the primary server.
//
// Location:
//   SipCoreSvc/CSipServerMonitorSvc.h
//
// See Also:
//   ISipServerMonitorSvc
//
//==============================================================================
class CSipServerMonitorSvc : public CEComDelegatingUnknown,
                             protected ISipCoreSvc,
                             protected CSipReqCtxCoreSvc,
                             protected ISipServerMonitorSvc
{
//-- Published Interface
public:

    //-- << Static Initialization >>

    static mxt_result InitializeCSipServerMonitorSvc();
    static void FinalizeCSipServerMonitorSvc();

    //-- << ECom Pattern >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //-- << Inherited from CEComDelegatingUnknown >>

    virtual mxt_result InitializeInstance();
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << Inherited from ISipCoreSvc >>

    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                        IN EDirection eDirection);

    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    virtual mxt_result ClearCoreSvc();

    //-- << Inherited from CSipReqCtxCoreSvc >>

    virtual void OnPacketReceived(IN    ISipRequestContext& rRequestContext,
                                  IN    const CSipPacket&   rPacket,
                                  INOUT CSipCoreEventList&  rEventList);

    virtual mxt_result UpdatePacket(IN    ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque          opqEvent,
                           IN const CSipPacket&   rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque          opqEvent);

    virtual unsigned int AddRcsRef();
    virtual unsigned int ReleaseRcsRef();

    //-- << Inherited from ISipServerMonitorSvc >>

    virtual mxt_result SetManager(IN ISipServerMonitorMgr* pMgr);

    virtual mxt_result SetServerMonitor(IN ISipServerMonitor* pServerMonitor);

    virtual void ReportEvSwitchToServer(IN ISipServerMonitorMgr::EServerType eServer);

//-- Hidden Methods
protected:

    // Event IDs
    enum EServerMonitorEvents
    {
        eEV_PRIMARY_SERVER_FAILURE
    };

    //-- << Inherited from CEComDelegatingUnknown >>

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

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
    CSipServerMonitorSvc(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipServerMonitorSvc();

    //-- << Deactivated Constructors / Operators >>

    // Summary:
    //  Copy constructor.
    CSipServerMonitorSvc(IN const CSipServerMonitorSvc& rFrom);

    // Summary:
    //  Assignment Operator.
    CSipServerMonitorSvc& operator=(IN const CSipServerMonitorSvc& rFrom);

// Friends declarations

//-- Hidden Data Members
protected:
private:

    // Manager.
    ISipServerMonitorMgr* m_pMgr;

    // Server Monitor component.
    ISipServerMonitor* m_pServerMonitor;

    // Last packet sent.
    CSipPacket* m_pLastPacketSent;
};

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #if defined(MXD_SIPSTACK_ENABLE_SIP_SERVER_MONITOR_SVC_SUPPORT)

#endif //-- #ifndef MXG_CSIPSERVERMONITORSVC_H
