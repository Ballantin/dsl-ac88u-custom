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
#ifndef MXG_CSIPTARGETMANAGEMODULE_H
#define MXG_CSIPTARGETMANAGEMODULE_H

#if defined(MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT)

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


#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h"
#endif

#ifndef MXG_ISIPCONTEXTOBSERVER_H
#include "SipCore/ISipContextObserver.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

class ITargetManagementModuleMgr;
class ISipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//== Class: CTargetManagementModule
//==============================================================================
//
// Description:
//   This class implements an auxiliary module that allows to correlate target
//   addresses with the contexts that are using them. This allows to generate 
//   specific events  when a blacklisted-address becomes available. In order to
//   do so, the application has to create associations between the contexts
//   and the target addresses calling the AssociateContextToTarget() method.
//
// Location:
//  /SipCoreSvc/CTargetManagementModule.h
//
// See Also:
//  /SipCoreSvc/CSipTargetReuseSvc
//
//==============================================================================
class CTargetManagementModule: public ISipContextObserver
{
//-- New type definition
public:
//-- New type definition
public:
    // The structure representing target
    struct STargetDef
    {
        CSocketAddr*                m_psTargetAddress;          // The target IP address and port
        ESipTransport               m_eTransport;               // The transport to be used for this target.
        bool                        m_bAvailable;               // True if the target is available.
    };

    // The structure that associates the SipContext with a list of targets.
    struct SContextTargets
    {
        ISipContext*                m_pContext;                 // Context.
        CList<STargetDef>*          m_plststAssociatedTargets;  // List of associated targets
    };

//-- Friend Declarations


//-- Public Methods
public:
    // << Constructors / Destructors >>
    CTargetManagementModule();
    virtual ~CTargetManagementModule();

    // Summary:
    //  Associates an ISipContext instance with a target.
    mxt_result AssociateContextToTarget(IN IEComUnknown* pContext,
                                        IN ESipTransport eTargetTransport,
                                        TOA CSocketAddr* pTargetAddr);
    // Summary:
    //  Dissociates an ISipContext instance from an specific target.
    mxt_result DissociateContextFromTarget(IN IEComUnknown* pContext,
                                           IN ESipTransport eTargetTransport,
                                           TOA CSocketAddr* pTargetAddr);
    // Summary:
    //  Dissociates an ISipContext from all its targets.
    mxt_result DissociateContext(IN IEComUnknown* pContext);

    // Summary:
    //  Notifies the module manager that the target is available.
    mxt_result TargetIsAvailable(IN ESipTransport eTargetTransport,
                                 IN CSocketAddr& rTargetAddr);
    // Summary:
    //  Notifies the module manager that the target is unavailable.
    mxt_result TargetIsUnavailable(IN ESipTransport eTargetTransport,
                                   IN CSocketAddr& rTargetAddr);
    // Summary:
    //  Checks if the given target has a higher priority than the current
    //  one for at least one context.
    bool IsHigherPriorityTargetSomewhere(IN ESipTransport eTargetTransport,
                                         IN CSocketAddr& rTargetAddr);  
    // Summary:
    //  Sets the service manager.
    void SetManager(ITargetManagementModuleMgr* pMgr);

    //-- << ISipContextObserver Interface >>.
    
    virtual void EvContextCleared(ISipContext* pContext);


// Hidden Methods
private:
    mxt_result FindContext(IN IEComUnknown* pContext,
                           OUT unsigned int& ruContextIndex);

    mxt_result FindTarget(IN unsigned int uContextIndex,
                          IN ESipTransport eTargetTransport,
                          IN CSocketAddr* pTargetAddr,
                          OUT unsigned int& ruTargetIndex);

    mxt_result AddTarget(IN unsigned int uContextIndex,
                         IN ESipTransport eTargetTransport,
                         TOA CSocketAddr* pTargetAddr);

    mxt_result RemoveTarget(IN unsigned int uContextIndex,
                            IN unsigned int uTargetIndex);

// Hidden Data Members
private:
    ITargetManagementModuleMgr* m_pMgr;                     // The service manager.
    CList<SContextTargets>*     m_plststContexts;           // The list of contexts.
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  SetManager
//==
//==============================================================================
//
//  Parameters:
//    pMgr:
//      The manager.
//
//  Description:
//      Sets the manager of this service.
//
//==============================================================================
inline void CTargetManagementModule::SetManager(ITargetManagementModuleMgr* pMgr)
{
    m_pMgr = pMgr;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT)

#endif // #ifndef MXG_CSIPTARGETMANAGEMODULE_H


