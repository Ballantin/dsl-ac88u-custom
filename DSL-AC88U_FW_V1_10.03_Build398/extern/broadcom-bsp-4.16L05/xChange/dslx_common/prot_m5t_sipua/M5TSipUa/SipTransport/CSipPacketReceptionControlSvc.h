//==============================================================================
//==============================================================================
//
//          Copyright(c) 2011 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPPACKETRECEPTIONCONTROLSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPPACKETRECEPTIONCONTROLSVC_H

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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
#error You MUST define MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT in your PreSipStackCfg.h \
in order to use this object.
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_CSIPTRANSPORTSVC_H
#include "SipTransport/CSipTransportSvc.h"
#endif

//-- Data Members.
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class ISipPacketReceptionControlMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipPacketReceptionControlSvc
//==============================================================================
// <FLAG New in 4.1.8>
//
// Description:
//   This class implements a stack-side control mechanism responsible to make
//   sure packet reception is only possible when the SipStack and/or the
//   application using it can process them.
//
//   It is possible using this transport service to block peers that are not
//   approved to send requests/responses to the stack/application. It is also
//   possible to always allow certain types of requests to pass through in order
//   to check if the peer should be approved or not.
//
// Location:
//   SipTransport/CSipPacketReceptionControlSvc.h
//
//==============================================================================
class CSipPacketReceptionControlSvc : public CSipTransportSvc
{
//-- New Types.
protected:
private:

//-- Published Interface.
public:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSipPacketReceptionControlSvc();

    // << CSipPacketReceptionControlSvc >>

    // Summary:
    //  Sets the reception control service manager.
    void SetPacketReceptionControlMgr(IN ISipPacketReceptionControlMgr* pMgr);

    // << Peer Filtering option >>

    // Summary:
    //  Enables the Peer Filtering option.
    void EnablePeerFiltering(IN bool bEnable);

    // Summary:
    //  Modifies the list of approved peers.
    void ModifyApprovedPeerList(IN const CSocketAddr& rPeerAddr, IN bool bIsApproved);

    // Summary:
    //  Removes all peers from the list of approved peers.
    void RemoveAllApprovedPeers();

    // Summary:
    //  Gets the list of approved peers.
    void GetApprovedPeerList(OUT CVector<CSocketAddr>& rvecApprovedPeers);

    // Summary:
    //  Modifies the list of allowed methods.
    void ModifyAllowedMethodList(IN TOA CString* pstrMethod, IN bool bIsAllowed);

    // Summary:
    //  Removes all methods from the list of allowed methods.
    void RemoveAllAllowedMethods();

    // Summary:
    //  Gets the list of allowed methods.
    void GetAllowedMethodList(OUT CVector<CString>& rvecstrAllowedMethods);

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

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Destructor. Must call ReleaseService instead of directly access this
    //  destructor.
    virtual ~CSipPacketReceptionControlSvc();

private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipPacketReceptionControlSvc(IN const CSipPacketReceptionControlSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipPacketReceptionControlSvc& operator=(IN const CSipPacketReceptionControlSvc& rSrc);

    //-- << CSipTransportSvc >>.

    virtual mxt_result SpecificShutdown(IN ISipTransportUser* pUser,
                                        IN mxt_opaque opq);

    //-- << Helpers >>.


// Friends declarations.

//-- Hidden Data Members.
protected:
private:

    // The service manager.
    ISipPacketReceptionControlMgr* m_pMgr;

    // Tells if the shutdown is in progress or not.
    bool m_bShutDown;

    // Tells if the Peer Filtering option is enabled or not.
    bool m_bPeerFilteringEnabled;

    // The list of allowed methods. Using string allows the Peer Filtering option to allow unknown SIP methods.
    CVector<CString*> m_vecpstrAllowedMethods;

    // The list of approved peers.
    CVector<CSocketAddr> m_vecApprovedPeers;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSIPPACKETRECEPTIONCONTROLSVC_H
