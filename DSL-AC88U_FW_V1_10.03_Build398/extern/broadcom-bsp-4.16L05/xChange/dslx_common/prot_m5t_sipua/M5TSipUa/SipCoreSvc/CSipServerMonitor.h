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
#ifndef MXG_CSIPSERVERMONITOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSERVERMONITOR_H

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

#ifndef MXG_ISIPSERVERMONITOR_H
#include "SipCoreSvc/ISipServerMonitor.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_ISIPSERVERMONITORMGR_H
#include "SipCoreSvc/ISipServerMonitorMgr.h"
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
//== Class: CSipServerMonitor
//==============================================================================
//
// Description:
//    This class is the Server monitoring component. It allows contexts to
//    register for notifications about the comunication state with the primary
//    server.
//
// Location:
//   SipCoreSvc/CSipServerMonitor.h
//
// See Also:
//   ISipServerMonitor
//
//==============================================================================
class CSipServerMonitor : public CEComDelegatingUnknown,
                          protected ISipServerMonitor
{
//-- Published Interface
public:

    //-- << Static initialization >>

    static mxt_result InitializeCSipServerMonitor();
    static void FinalizeCSipServerMonitor();

    //-- << ECom Pattern >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //-- << Inherited from CEComDelegatingUnknown >>
    virtual mxt_result InitializeInstance();
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << Inherited from ISipServerMonitor >>.

    virtual mxt_result Configure(IN const CHostPort& rPrimaryServer,
                                 IN const CHostPort& rBackupServer);

    virtual mxt_result RegisterSipContext(IN ISipContext* pSipContext);

    virtual mxt_result UnregisterSipContext(IN ISipContext* pSipContext);

    virtual mxt_result OnResponseReceived(IN ISipContext* pSipContext,
                                          IN const CSipPacket& rRequest,
                                          IN const CSipPacket& rResponse);

//-- Hidden Methods
protected:

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
    CSipServerMonitor(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipServerMonitor();

    //-- << Deactivated Constructors / Operators >>

    // Summary:
    //  Copy constructor.
    CSipServerMonitor(IN const CSipServerMonitor& rFrom);

    // Summary:
    //  Assignment Operator.
    CSipServerMonitor& operator=(IN const CSipServerMonitor& rFrom);

// Friends declarations

//-- Hidden Data Members
protected:
private:

    // Primary Server Host Port.
    CHostPort* m_pPrimaryHostPort;

    // Backup Server Host Port.
    CHostPort* m_pBackupHostPort;

    // List of registered context.
    CList<ISipContext*> m_listpSipContexts;

    // List of registered context using the primary server.
    CList<ISipContext*> m_listpSipContextsOnPrimaryServer;

    // List of registered context using the backup server.
    CList<ISipContext*> m_listpSipContextsOnBackupServer;

    // The server on which newly registered contexts are considered.
    ISipServerMonitorMgr::EServerType m_eCurrentServer;
};

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #if defined(MXD_SIPSTACK_ENABLE_SIP_SERVER_MONITOR_SVC_SUPPORT)

#endif //-- #ifndef MXG_CSIPSERVERMONITOR_H
