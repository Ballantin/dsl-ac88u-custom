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
#ifndef MXG_CSIPSTACKMONITOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSTACKMONITOR_H

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

#if !defined(MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT) ||\
    !defined(MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT and \
MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT to be able to use this class.
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_ISIPREQUESTCONTEXTMGR_H
#include "SipCore/ISipRequestContextMgr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class ISipContext;
class ISipRequestContext;
class ISipRequestContextTerminationMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipStackMonitor
//==============================================================================
//
// Summary:
//     This class is a monitoring tool for the on-going activity in the stack.
//
// Description:
//     This class is a monitoring tool for the on-going activity in the stack.
//
//     The current implementation only monitors the on-going transactions and
//     associates them with a context. This is used in the CSipContextTerminator
//     feature.
//
//     Eventually, it shall evolve to monitor many more things and possibly
//     replace the deprecated instance tracking system.
//
// Location:
//   SipCore/CSipStackMonitor.h
//
// See Also:
//   ISipRequestContextMgr
//
//==============================================================================
class CSipStackMonitor: public ISipRequestContextMgr
{
//-- Published Interface
public:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSipStackMonitor();

    // Summary:
    //  Destructor.
    virtual ~CSipStackMonitor();

    //-- Public Interface
    void GetActiveRequestContexts(IN ISipContext* pContext, OUT CList<ISipRequestContext*>& rlstpActiveReqCtx) const;
    void SetRequestContextTerminationMgr(IN ISipContext* pContext, IN ISipRequestContextTerminationMgr* pMgr);

//-- Hidden Methods
protected:

    //-- Inherited from ISipRequestContextMgr.
    virtual void EvCreated(IN ISipRequestContext* pReqCtx, IN ISipContext* pContext);
    virtual void EvCompleted(IN ISipRequestContext* pReqCtx, IN ISipContext* pContext);

private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipStackMonitor(IN const CSipStackMonitor& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipStackMonitor& operator=(IN const CSipStackMonitor& rSrc);

// Friends declarations

//-- Hidden Data Members
protected:
private:

    struct SContext
    {
        ISipRequestContextTerminationMgr* m_pReqCtxTerminationMgr;
        CList<ISipRequestContext*> m_lstpReqCtx;

        SContext()
        :   m_pReqCtxTerminationMgr(NULL)
        {
        }
    };

#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
    // GCC 2.7.2 does not support default parameters in templates,
    // in which case all the parameters must be specified.
    CMap<ISipContext*, SContext, CAATree<CMapPair<ISipContext*, SContext> > > m_mapContexts;
#else
    CMap<ISipContext*, SContext> m_mapContexts;
#endif
};

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSIPSTACKMONITOR_H

