//==============================================================================
//==============================================================================
//
//          Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPNETWORKRECEPTIONCONTROLSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPNETWORKRECEPTIONCONTROLSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
#error You MUST define MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT in your PreSipStackCfg.h \
in order to use this object.
#endif

//-- Data Members.

//-- Interface Realized and/or Parent
#ifndef MXG_CSIPTRANSPORTSVC_H
#include "SipTransport/CSipTransportSvc.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class ISipNetworkReceptionControlMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipNetworkReceptionControlSvc
//==============================================================================
// <FLAG New in 4.1.8>
//
// Description:
//   This class implements a network-side control mechanism responsible to make
//   sure packet reception is only possible when the SipStack and/or the
//   application using it can process them.
//
//   It is possible using this transport service to block peers that repeatedly
//   sends packets in a sort of message flood attack for a certain time.
//
// Location:
//   SipTransport/CSipNetworkReceptionControlSvc.h
//
//==============================================================================
class CSipNetworkReceptionControlSvc : public CSipTransportSvc,
                                       private CEventDriven
{
//-- New Types.
protected:
private:
    struct SPeerInformation
    {
        // The peer address.
        CSocketAddr m_peerAddr;

        // The peer information identifier.
        uint32_t m_uId;

        // The first window start time.
        uint64_t m_uFirstStartTimeMs;

        // First half window reception count.
        uint32_t m_uFirstReceptionCount;

        // Second half window reception count.
        uint32_t m_uSecondReceptionCount;

        //-- MXI_DESIGN_NOTES(2011-02-17):
        //--  There is no need to keep a count for the third half window because when we are counting for the third half,
        //--  the first window is completed and we already know if we should have blocked the peer or not for this window.
        //--  Hence, when moving to the third half, the first half has no use anymore, so the second half becomes the
        //--  first half and the third becomes the second one.

        // Tells if the connection is blocked or not.
        bool m_bIsBlocked;
    };

//-- Published Interface.
public:

    // Summary:
    //  Sets the reception control service manager.
    void SetNetworkReceptionControlMgr(IN ISipNetworkReceptionControlMgr* pMgr);

    // Summary:
    //  Configures the Peer Flood Blocking option.
    void SetPeerFloodBlocking(IN uint32_t uRawPacketCountLimit,
                              IN uint32_t uReceptionWindowSizeMs,
                              IN uint32_t uBlockingTimeoutMs);

    //-- << CSipTransportSvc >>.

    // Summary:
    //  Initializes the parser service as a SipTransport service chain link.
    virtual mxt_result Initialize(IN CSipTransportSvc* pNextSender,
                                  IN CSipTransportSvc* pNextReceiver);

    // Summary:
    //  Handles incoming packets.
    void ReceivedFromNetwork(IN CSipPacket* pPacket,
                             IN CSipClientSocket* pSocket);

    // Summary:
    //  Releases the resources used by the service.
    void ReleaseService();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSipNetworkReceptionControlSvc();

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Destructor. Must call ReleaseService instead of directly access this
    //  destructor.
    virtual ~CSipNetworkReceptionControlSvc();

private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipNetworkReceptionControlSvc(IN const CSipNetworkReceptionControlSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipNetworkReceptionControlSvc& operator=(IN const CSipNetworkReceptionControlSvc& rSrc);

    //-- << CSipTransportSvc >>.

    virtual mxt_result SpecificShutdown(IN ISipTransportUser* pUser,
                                        IN mxt_opaque opq);

    //-- << CEventDriven >>.

    void EvTimerServiceMgrAwaken(IN bool bStopped,
                                 IN unsigned int uTimer,
                                 IN mxt_opaque opq);

    //-- << Helpers >>.

    // Summary:
    //  Manages the connections.
    SPeerInformation* ManageConnections(IN CSipClientSocket& rSocket, OUT bool& rbAlreadyBlocked);

    // Summary:
    //  Returns the peer information or creates it if needed.
    SPeerInformation* GetPeerInformation(IN CSipClientSocket& rSocket);

    // Summary:
    //  Returns the peer information associated with the provided identifier.
    CSipNetworkReceptionControlSvc::SPeerInformation* FindPeerInformation(IN uint32_t uIdentifier);

    // Summary:
    //  Returns whether or not the peer has exceeded the configured threshold count.
    bool CheckThresholdCount(CSipNetworkReceptionControlSvc::SPeerInformation* pstPeerInfo);

// Friends declarations.

//-- Hidden Data Members.
protected:
private:

    // The service manager.
    ISipNetworkReceptionControlMgr* m_pMgr;

    // Tells that the shutdown is in progress or not.
    bool m_bShutDown;

    // The threshold maximum data packet count for which to block packets.
    uint32_t m_uRawPacketCountLimit;

    // The reception window size in milliseconds to consider if a peer should be blocked or not.
    uint32_t m_uReceptionWindowSizeMs;

    // The block timeout in milliseconds.
    uint32_t m_uBlockTimeoutMs;

    // The list of peers.
    CVector<SPeerInformation> m_vecstPeers;

    // The last identifier used.
    static uint32_t ms_uLastIdentifier;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  CheckThresholdCount
//==
//==============================================================================
//
//  Parameters:
//    pstPeerInfo:
//      The peer information to process.
//
//  Return:
//      If the threshold count was exceeded or not.
//
//  Description:
//      Returns whether or not the peer has exceeded the configured threshold
//      count.
//
//==============================================================================
inline bool CSipNetworkReceptionControlSvc::CheckThresholdCount(CSipNetworkReceptionControlSvc::SPeerInformation* pstPeerInfo)
{
    bool bBlockPeer = false;

    // Check for threshold count... Regardless of the time, both together MUST not exceed the count.
    if ((pstPeerInfo->m_uFirstReceptionCount + pstPeerInfo->m_uSecondReceptionCount) > m_uRawPacketCountLimit)
    {
        // Threshold count exceeded, block the peer.
        bBlockPeer = true;
    }

    return bBlockPeer;
}

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSIPNETWORKRECEPTIONCONTROLSVC_H
