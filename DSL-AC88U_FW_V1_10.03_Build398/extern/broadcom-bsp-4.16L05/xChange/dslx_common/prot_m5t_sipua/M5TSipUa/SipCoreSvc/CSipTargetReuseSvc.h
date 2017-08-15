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
#ifndef MXG_CSIPTARGETREUSESVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPTARGETREUSESVC_H
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


#ifndef MXD_SIPSTACK_ENABLE_SIP_TARGET_REUSE_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_TARGET_REUSE_SVC_SUPPORT \
to be able to use this class.
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#include "SipCore/CSipReqCtxConnectionSvc.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_CSIPREQCTXSERVERLOCATIONSVC_H
#include "SipCoreSvc/CSipReqCtxServerLocationSvc.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CSERVERLOCATOR_H
#include "SipCoreSvc/CServerLocator.h"
#endif

#ifndef MXG_ISIPTARGETREUSESVC_H
#include "SipCoreSvc/ISipTargetReuseSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

class CSipUri;
class ISipRequestContext;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//== Class: CSipTargetReuseSvc
//==============================================================================
//
// Description:
//   This class implements the ISipTargetReuseSvc functionality.
//
//   Direct access to this class is prohibited, it should be accessed through
//   the ECOM mechanism via the CSipContext.
//
// Location:
//   SipCoreSvc/CSipTargetReuseSvc.h
//
// See Also:
//   ISipTargetReuseSvc, ISipTargetReuseMgr
//
//==============================================================================
class CSipTargetReuseSvc : public CEComDelegatingUnknown,
                           public CSipReqCtxConnectionSvc,
                           public ISipTargetReuseSvc,
                           public ISipConnectionSvc,
                           public IServerLocatorMgr,
                           public CEventDriven
{
//-- Friend Declarations

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

private:
    enum
    {
        eMSG_RESOLVED
    };

//-- Hidden Methods
private:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipTargetReuseSvc();

    static void FinalizeCSipTargetReuseSvc();

    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result InitializeInstance();

    void UninitializeInstance(OUT bool* pbDeleteThis);

    // << Constructors / Destructors >>
    CSipTargetReuseSvc(IN IEComUnknown& rOuterIEComUnknown);
    virtual ~CSipTargetReuseSvc();


    //-- << CEComDelegatingUnknown >>

    // The following statement is essential, it provide the default and unique
    // implementation of the IEComUnknown interface that every other interface
    // inherit from.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);



    //-- << ISipConnectionSvc >>

    virtual ISipReqCtxConnectionSvc* GetReqCtxConnectionSvcInstance(IN const CSipPacket& rPacket,
                                                                    IN ISipConnectionSvc::EDirection eDirection);

    virtual void ClearConnectionSvc();

    //-- << ISipReqCtxConnectionSvc >>

    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

    //-- << IServerLocatorMgr >>

    virtual void OnAllTargetsResolved(IN TO CList<SNaptrRecord>* plst,
                                      IN mxt_opaque opq);
    virtual void OnTargetResolved(IN TO CList<SNaptrRecord>* plst,
                                  IN mxt_opaque opq);
    virtual void OnTargetResolved(IN TO CList<SSrvRecord>* plst,
                                  IN mxt_opaque opq);
    virtual void OnTargetResolved(IN TO CList<CSocketAddr>* plst,
                                  IN mxt_opaque opq);


    //-- << ISipTargetReuseSvc >>

    virtual mxt_result GetCurrentNextHop(OUT CSipUri & rCurrentTarget);

    virtual mxt_result GetResolvedTargets(OUT CList<SResolvedTarget>& rlststTargets);

    virtual mxt_result SetResolvedTargets(TOA CList<SResolvedTarget>* plststTargets);

    virtual mxt_result GetCurrentTarget(OUT ESipTransport& reTransport,
                                        OUT CSocketAddr& rTargetAddr);

    virtual mxt_result SetCurrentTargetIndex(IN unsigned int uIndex);

    virtual mxt_result UseHighestPriorityTarget();

    virtual bool IsHigherPriorityTarget(IN ESipTransport eTransport,
                                        IN const CSocketAddr& rTargetAddr);

    virtual bool IsHigherPriorityTarget(IN unsigned int uIndex);

    virtual mxt_result GetCurrentTargetIndex(OUT unsigned int& ruIndex);

    virtual void SetManager(IN ISipTargetReuseMgr* pMgr);

    virtual mxt_result SetTargetListModifier(IN mxt_PFNTargetListModifier pfnNew,
                                             IN mxt_opaque opqCallbackParameter,
                                             OUT mxt_PFNTargetListModifier& rpfnPrevious,
                                             OUT mxt_opaque& ropqPreviousParameter);

    // Summary:
    //  Configures the transaction statistics to be used by this service.
    void SetTransactionStatistics(IN ISipTransactionStatistics* pTransStats);

    //-- << IMessageServiceMgr >>
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

private:

    // Deactivated Constructors / Destructors / Operators
    CSipTargetReuseSvc(const CSipTargetReuseSvc& rFrom);
    CSipTargetReuseSvc& operator=(const CSipTargetReuseSvc& rFrom);

    // Summary:
    //  Triggers a new SipUri resolution.
    mxt_result ResolveNewUri(IN ISipRequestContext* pRequestContext,
                             INOUT CSipPacket* pPacket);

    // Summary:
    //  Creates the resolved target list.
    mxt_result CreateResolvedTargetList(CList<SNaptrRecord>& rlstNaptrRecord);

    // Summary:
    //  Gets the SIP transport from the NAPTR record.
    ESipTransport GetNaptrRecordTransport(IN const SNaptrRecord& rstNaptrRecord);


    // Summary:
    //  Selects the target to try.
    mxt_result SelectTarget(bool bDoNotReuse = false);


    // Summary:
    //  Gets the index of the given target address and transmport.
    mxt_result GetTargetIndex(IN ESipTransport eTransport,
                              IN const CSocketAddr& rTargetAddr,
                              OUT unsigned int& ruIndex);

    // Summary:
    //  Private method that truly does the packet update.
    mxt_result UpdateLocation(INOUT CSipPacket& rPacket);

    // Summary:
    //  Demarshalling of the parameters posted through OnAllTargetsResolved.
    void InternalOnAllTargetsResolved(IN CMarshaler* pParameter);

    // Summary:
    //  Internal processing of OnAllTargetsResolved.
    void OnAllTargetsResolvedHelper(IN CSipPacket* pPacket,
                                    TO CList<SNaptrRecord>* plstNaptrRecord);

    // Summary:
    //  Sets the current target.
    mxt_result SetCurrentTarget(unsigned uIndex);

    // Summary:
    //  Checks that the list of resolved targets is not empty.
    bool ResolvedTargetListIsValid();

    // Summary:
    //  Checks if the request next hop URI is the same as the one
    //  currently configured.
    bool IsSameRequestNextHopUri(IN CSipPacket& rPacket);

    // Summary:
    //  Compares the two targets.
    bool TargetsAreEqual(SResolvedTarget* pstTarget1,
                         SResolvedTarget* pstTarget2);
    // Summary:
    //  Copies the source target list into the destination one.
    void CopyTargetList(IN CList<SResolvedTarget>& rlststTargetsSrc,
                        OUT CList<SResolvedTarget>& rlststTargetsDest);

    // Summary:
    //  Checks if a failover flag related to the given context is in
    //  the list.
    bool ChkFailover(IN ISipRequestContext& rRequestContext,
                     bool bRemoveAllRelated = false);

    // Summary:
    //  Compares the packet peer address with the current target.
    bool CheckPacketPeerAdrr(IN const CSipPacket& rPacket);

    // Summary:
    //  Moves the targets whose transport is UDP to the end of the list.
    void SortTargetListUdpLast(IN unsigned int uIndex);

    // Summary:
    //  Updates the next hop URI and report changes to the manager.
    void UpdateCurrentNextHopUri(IN IUri* pNewNextHopUri);

// Hidden Data Members
protected:
private:
    ISipTargetReuseMgr*         m_pMgr;                                 // The service manager.
    CServerLocator*             m_pServerLocator;                       // Service locator.
    CSipUri*                    m_pCurrentNextHopUri;                   // The resolved next hop URI.
    CList<SResolvedTarget>*     m_plststResolvedTargets;                // The sorted list of targets.
    SResolvedTarget*            m_pstCurrentTarget;                     // The current target.
    mxt_PFNTargetListModifier   m_pfnListModifier;                      // The target list modifier callback
    mxt_opaque                  m_opqCallbackParameter;                 // List modifier callback opaque
    ISipTransactionStatistics*  m_pTransStats;                          // Statistisc object.
    CList<mxt_opaque>*          m_plstopqFailOver;                      // Fail over list.
    bool                        m_bUdpAlreadyTried;                     // Flag to indicate UDP has already tried.
                                                                        // Used in TCP overrides.
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)


#endif // #ifndef MXG_CSIPTARGETREUSESVC_H


