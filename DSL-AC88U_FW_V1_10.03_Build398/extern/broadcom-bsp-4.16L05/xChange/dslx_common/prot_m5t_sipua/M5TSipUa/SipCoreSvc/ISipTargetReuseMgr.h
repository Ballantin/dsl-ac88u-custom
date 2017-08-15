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

#ifndef MXG_ISIPTARGETREUSEMGR_H
#define MXG_ISIPTARGETREUSEMGR_H

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


#ifndef MXD_SIPSTACK_ENABLE_SIP_TARGET_REUSE_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_TARGET_REUSE_SVC_SUPPORT \
to be able to use this class.
#endif

#ifndef MXG_ISIPTARGETREUSESVC_H
#include "SipCoreSvc/ISipTargetReuseSvc.h"
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
//== Class: ISipReuseTargetMgr
//==============================================================================
//
// Description:
//   The ISipReuseTargetMgr interface is the interface used by
//   CSipTargetReuseSvc to report events to the application.
//
// Location:
//   SipCoreSvc/ISipTargetReuseMgr.h
//
//==============================================================================
class ISipTargetReuseMgr
{

//-- Published Interface
public:

    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  EvNewNextHopUri
    //==
    //==========================================================================
    //
    //  Summary:
    //      Called when a new next hop URI needs to be resolved.
    //
    //  Parameters:
    //    pReuseTargetSvc:
    //      The calling ISipTargetReuseSvc service.
    //    rNextHopUri:
    //      The new next hop URI to be resolved.
    //
    //  Description:
    //      This event is called to report that the next hop URI of a request
    //      of Via header is different from the current one.
    //      Before this event is reported, the SipTargetReuse service resets
    //      the resolved targets. However, the previous selected target is
    //      saved by the service. When the new next hop URI is resolved, if
    //      the saved target is found in the new list, it will be selected and
    //      reused.
    //
    //==========================================================================
    virtual void EvNewNextHopUri(IN ISipTargetReuseSvc* pReuseTargetSvc,
                                 IN const CSipUri& rNextHopUri) = 0;

    //==========================================================================
    //==
    //==  EvDnsResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Called when a DNS resolution is completed.
    //
    //  Parameters:
    //    pReuseTargetSvc:
    //      The calling ISipTargetReuseSvc service.
    //    rResolvedUri:
    //      The resolved URI.
    //    rlststTargets:
    //      The list of resolved targets. Each entry of the list contains the
    //      solved address and transport.
    //      If this list is empty, it means that either the DNS request failed
    //      or did not return any result.
    //    pstPreviousTarget:
    //      The previous selected target. It is NULL for the first DNS
    //      resolution on a SIP context.
    //
    //  Description:
    //      Event reporting the end of a DNS resolution for a new next hop URI.
    //      A DNS resolution is done each time a packet is sent to an address
    //      for which RFC3263 tells to do a resolution. The application could
    //      modify the resolved target list in this method.
    //      If the previous selected target is found in the list, it will be
    //      used and the targets with higher priority will be ignored. The
    //      application can always force to use the highest priority target,
    //      calling the UseHighestPriorityTarget(), or a given target calling
    //      SetCurrentTarget().
    //      If the current target is not found in the list, the highest
    //      priority target will be selected as the new selected target.
    //      This method must be synchronous.
    //
    //==========================================================================
    virtual void EvDnsResult(IN ISipTargetReuseSvc* pReuseTargetSvc,
                             IN const CSipUri& rResolvedUri,
                             IN const CList<ISipTargetReuseSvc::SResolvedTarget>& rlststTargets,
                             IN const ISipTargetReuseSvc::SResolvedTarget* pstPreviousTarget) = 0;


    //==========================================================================
    //==
    //==  EvNewTarget
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a new target has been set.
    //
    //  Parameters:
    //    pReuseTargetSvc:
    //      The calling ISipTargetReuseSvc service.
    //    eTransport:
    //      The transport on which the packet will be sent.
    //    rTargetAddr:
    //      The address to which the packet will be sent.
    //
    //  Description:
    //      This event reports that the target of a context has changed.
    //      This method is called by the SipTargetReuse sevice before sending a
    //      packet to give the application a last chance to change the target.
    //
    //==========================================================================
    virtual void EvNewTarget(IN ISipTargetReuseSvc* pReuseTargetSvc,
                             IN ESipTransport eTransport,
                             IN const CSocketAddr& rTargetAddr) = 0;

    //==========================================================================
    //==
    //==  EvTargetUnreachable
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a packet cannot be sent to a target.
    //
    //  Parameters:
    //    pReuseTargetSvc:
    //      The calling ISipTargetReuseSvc service.
    //    eTransport:
    //      The transport used to try to reach the target.
    //    rUnreachableTargetAddr:
    //      The address that cannot be reached.
    //    resCause:
    //      Error code indicating the cause of the failure.
    //      resFE_SIPTRANSPORT_MTU_SIZE_ERROR:
    //        Indicates the packet could not be sent because its serialized 
    //        size exceeds its maximum allowed size 
    //        (see CSipPacket::SetMaxSize).
    //
    //  Description:
    //      Reports that a packet cannot be sent to the specified target.
    //      The TargetReuseSvc will change the selected target, choosing the one
    //      that follows in priority from the list. The new selected target will
    //      be reporterd to the application by EvNewTarget.
    //      The implementation of this method can override the resolved targets.
    //      Overriding the current target will change the target that will be
    //      considered as failed by the service.
    //
    //==========================================================================
    virtual void EvTargetUnreachable(IN ISipTargetReuseSvc* pReuseTargetSvc,
                                     IN ESipTransport eTransport,
                                     IN const CSocketAddr& rUnreachableTargetAddr,
                                     IN mxt_result resCause) = 0;

//-- Hidden Methods
protected:

    //-- << Protected Constructors / Destructors >>

    // Summary:
    //  Default Constructor.
    ISipTargetReuseMgr(){}

    // Summary:
    //  Destructor.
    virtual ~ISipTargetReuseMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipTargetReuseMgr(IN const ISipTargetReuseMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipTargetReuseMgr& operator=(IN const ISipTargetReuseMgr& rSrc);


//-- Hidden Data Members
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTARGETREUSEMGR_H

