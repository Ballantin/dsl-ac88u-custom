//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation.
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
//==EDOC========================================================================
#ifndef MXG_CSIPREQCTXSERVERLOCATIONSVC_H
#define MXG_CSIPREQCTXSERVERLOCATIONSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#ifndef MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT to be able to use this \
class.
#endif



//-- Data Members
//-----------------
#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif
#ifndef MXG_NETWORKTOOLS_H
#include "Network/NetworkTools.h"
#endif
#ifndef MXG_ISIPSERVERLOCATIONSVC_H
#include "SipCoreSvc/ISipServerLocationSvc.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#include "SipCore/CSipReqCtxConnectionSvc.h"
#endif

#ifndef MXG_CSERVERLOCATOR_H
#include "SipCoreSvc/CServerLocator.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CMarshaler;
class CSipPacket;
class CSipServerLocationSvc;
class ISipRequestContext;
class ISipTransactionStatistics;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==SDOC========================================================================
//== Class: CSipReqCtxServerLocationSvc
//========================================
//
// Description:
//   This class implements the ISipReqCtxConnectionSvc interface, which is used
//   by the ISipRequestContext to interact with the connection services. It is
//   obtained by the request context through the ISipConnectionSvc interface.
//
//   When it receives an UpdatePacket request for the first time, the
//   CSipReqCtxServerLocationSvc tries to discover a destination by using the
//   information already put in the packet. It may need to perform asynchronous
//   resolving on the information found in the headers to obtain a list of
//   numeric targets. Once it is resolved, the same target is reused for every
//   Update Packet query until the end of the request context or an
//   OnSendFailure event is received. The latter removes the target from the
//   internal target list so another target is tried on the next call to
//   UpdatePacket. If there are no more targets or if it was impossible to
//   resolve to a numeric target, it is impossible to update the packet.
//
// Location:
//   SipCore/CSipReqCtxServerLocationSvc.h
//
// See Also:
//   ISipReqCtxConnectionSvc, IServerLocatorMgr, CServerLocator
//
//==EDOC========================================================================
class CSipReqCtxServerLocationSvc : public CSipReqCtxConnectionSvc,
                                    public IServerLocatorMgr,
                                    public CEventDriven
{
//-- Friend Declarations
//------------------------
    //M5T_INTERNAL_USE_BEGIN
    friend class CSipServerLocationSvc;
    //M5T_INTERNAL_USE_END

    enum
    {
        eMSG_RESOLVED
    };

//-- Published Interface
//------------------------
public:

    // Summary:
    //   Sets the UDP maximum size threshold (above which packets
    //   are first sent using TCP).
    //---------------------------------------------------------------
    static void SetUdpMaxSizeThreshold(IN unsigned uSize);

    // Summary:
    //   Gets the UDP maximum size threshold (above which packets
    //   are first sent using TCP).
    //---------------------------------------------------------------
    static unsigned int GetUdpMaxSizeThreshold();

    // Summary:
    //   Performs the clean up of the NAPTR record list.
    //---------------------------------------------------------------
    static void NaptrListCleanup(INOUT CList<SNaptrRecord>& rlstNaptrRecord);

    // Summary:
    //  Removes broadcast address from the packet.
    static void ResetBroadcastAddress(INOUT CSipPacket& rPacket);

    // Summary:
    //  Sorts the NAPTR record list.
    static void StatelessModeListModifier(INOUT CList<SNaptrRecord>& rlstNaptrRecord,
                                          IN const CSipPacket& rPacket);

    static void SortSrvRecords(INOUT CList<SSrvRecord>& rlstSrvRecord,
                               IN const CString& rstrBranch);

    //-- << ISipReqCtxConnectionSvc >>
    //--------------------------------------------

    // Summary:
    //  Method called after an updated packet has not been successfully sent.
    //------------------------------------------------------------------------
    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    // Summary:
    //  Updates rPacket with the resolved target.
    //--------------------------------------------
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    // Summary:
    //  Destroys this instance of ISipReqCtxConnectionSvc.
    //-----------------------------------------------------
    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

    //-- << IServerLocatorMgr >>
    //--------------------------------------------

    // Summary:
    //  Method called when the asynchronous resolution is completed.
    //---------------------------------------------------------------
    virtual void OnAllTargetsResolved(IN TO CList<SNaptrRecord>* plst,
                                      IN mxt_opaque opq);
    virtual void OnTargetResolved(IN TO CList<SNaptrRecord>* plst,
                                  IN mxt_opaque opq);
    virtual void OnTargetResolved(IN TO CList<SSrvRecord>* plst,
                                  IN mxt_opaque opq);
    virtual void OnTargetResolved(IN TO CList<CSocketAddr>* plst,
                                  IN mxt_opaque opq);


//-- Hidden Methods
//-------------------
protected:
    // Inherited from IMessageServiceMgr.
    //------------------------------------
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor. Kept private so it can only be created through the
    //  CSipServerLocationSvc::GetReqCtxConnectionSvcInstance method.
    //--------------------------------------------------------------------------
    CSipReqCtxServerLocationSvc(IN CSipServerLocationSvc* pServerLocationSvc,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                                IN ISipTransactionStatistics* pTransStats,
                                IN mxt_opaque opqDnsUser);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipReqCtxServerLocationSvc();

    virtual void SetReqCtxServerLocationStatelessMode(IN bool bIsStateless);

    // Summary:
    //  Sets the mxt_PFNServerLocationListModifier member.
    //-----------------------------------------------------
    virtual void SetServerLocationListModifiers(
                                    IN mxt_PFNServerLocationListModifier pfn,
                                    IN mxt_opaque opqCallbackParameter);


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipReqCtxServerLocationSvc(IN const CSipReqCtxServerLocationSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipReqCtxServerLocationSvc& operator=(
                                IN const CSipReqCtxServerLocationSvc& rSrc);

    struct SNaptrOpaque
    {
        CList<SNaptrRecord>* m_plstNaptr;
        mxt_opaque m_opq;

        SNaptrOpaque(CList<SNaptrRecord>* plstNaptr = NULL,
                     mxt_opaque opq = 0)
        : m_plstNaptr(plstNaptr),
          m_opq(opq)
        {}
    };

    struct SSrvOpaque
    {
        CList<SSrvRecord>* m_plstSrv;
        mxt_opaque m_opq;

        SSrvOpaque( CList<SSrvRecord>* plstSrv = NULL,
                    mxt_opaque opq = 0 )
        : m_plstSrv(plstSrv),
          m_opq(opq)
        {}
    };

    // Summary:
    //  Private method that truly performs the packet update.
    //--------------------------------------------------------
    mxt_result UpdateLocation(INOUT CSipPacket& rPacket);

    // Summary:
    //  Private method that performs the packet update for a branch in TopVia.
    //-------------------------------------------------------------------------
    mxt_result UpdateBranch(INOUT CSipPacket& rPacket);

    // Summary:
    //  Internal processing of OnAllTargetsResolved.
    void OnAllTargetsResolvedHelper(IN CSipPacket& rPacket,
                                    IN CList<SNaptrRecord>& rlstNaptrRecord);

    // Summary:
    //  De-marshaling of the parameters posted through OnAllTargetsResolved.
    void InternalOnAllTargetsResolved(IN CMarshaler* pParameter);

//-- Hidden Data Members
//------------------------
protected:
    CServerLocator* m_pServerLocator;
    CList<SNaptrRecord>* m_plststNaptrRecord;
    bool m_bInFailOver;
    mxt_PFNServerLocationListModifier m_pfnListModifier;
    mxt_opaque m_opqListModifierParameter;
    bool m_bStateless;

    // Parent service, used to access m_lastLocalAddrUsed.
    //-----------------------------------------------------
    CSipServerLocationSvc* m_pServerLocationSvc;

    // Flag to indicate UDP has already been tried. Used
    // in TCP overrides.
    //-----------------------------------------------------
    bool m_bUdpAlreadyTried;

    static unsigned ms_uUdpMaxSizeThreshold;

    // Pointer to persistent connection list.
    CSipPersistentConnectionList* m_pPersistentConnList;

private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC====================================================================
//==
//==  GetUdpMaxSizeThreshold
//==
//==========================================================================
//
//  Returns:
//      The maximum packet size in bytes.
//
//  Description:
//   Gets the UDP maximum size threshold (above which packets
//   are first sent using TCP).
//
//==EDOC====================================================================
inline
unsigned int CSipReqCtxServerLocationSvc::GetUdpMaxSizeThreshold()
{
    return ms_uUdpMaxSizeThreshold;
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREQCTXSERVERLOCATIONSVC_H
