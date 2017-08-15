//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPKEEPALIVESVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPKEEPALIVESVC_H



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

#ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT

//-- Data Members

//-- Interface Realized and/or Parent
#ifndef MXG_CSIPTRANSPORTSVC_H
#include "SipTransport/CSipTransportSvc.h"
#endif

#ifndef MXG_ISIPTRANSPORTUSR_H
#include "SipTransport/ISipTransportUser.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class ISipKeepAliveMgr;
class ISipKeepAliveExtensionMgr;
class CSipConnectionSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipKeepAliveSvc
//==============================================================================
//
// Description:
//   This class implements the SIP KeepAlive service as defined by the
//   draft-ietf-sip-outbound-15.
//   In the case of streaming transports (Tcp or Tls), this service is
//   responsable of performs the keep alive using Crlf "ping" - "pong"
//   mechanims.
//   In the case of Udp, the keep alive is performed by the application using
//   STUN binding requests. This class allows the application to send the
//   STUN "ping" packets. It also informs the application when the STUN
//   response has arrived.
//
// Location:
//   SipTransport/CSipKeepAliveSvc.h
//
//==============================================================================
class CSipKeepAliveSvc : public CSipTransportSvc,
                         private CEventDriven,
                         // Inherits from ISipTransportUser to get the sending
                         // result of a ping.
                         protected ISipTransportUser
{
//-- Published Interface
public:
    // Summary:
    //  Initializes CSipKeepAliveSvc.
    mxt_result InitializeKeepAliveSvc();

    // Summary:
    //  Sets the keep alive manager.
    void SetKeepAliveMgr(IN ISipKeepAliveMgr* pMgr);

    // Summary:
    //  Sets the keep alive extension manager.
    void SetKeepAliveExtensionMgr(IN ISipKeepAliveExtensionMgr* pMgr);

    // Summary:
    //  Enables/Disables the Crlf keep alive mechanism.
    void EnableCrlfKeepAlive(IN bool bEnable);

    // Summary:
    //  Starts the Crlf keep alive mechanism for the specified connection.
    mxt_result StartCrlfKeepAlive(IN ESipTransport eTransport,
                                  IN const CSocketAddr& rLocalAddr,
                                  IN const CSocketAddr& rPeerAddr,
                                  IN unsigned int uIntervalS,
                                  IN unsigned int uServerResponseTimeS,
                                  IN unsigned int uDestinationId);
    // Summary:
    //  Sends the specified data directly to the network.
    mxt_result SendDataToNetwork(IN ESipTransport eTransport,
                                 IN const CSocketAddr& rLocalAddr,
                                 IN const CSocketAddr& rPeerAddr,
                                 IN TOA const CBlob* pData,
                                 IN ISipTransportUser* pUser,
                                 IN mxt_opaque opq,
                                 IN unsigned int uDestinationId);

    //-- << CSipTransportSvc >>.

    // Summary:
    //  Initializes the parser service as a SipTransport service chain link.
    virtual mxt_result Initialize(IN CSipTransportSvc* pNextSender,
                                  IN CSipTransportSvc* pNextReceiver);
    // Summary:
    //  Handles incoming packets.
    void ReceivedFromNetwork(INOUT CSipPacket* pPacket,
                             IN CSipClientSocket* pSocket);

    // Summary:
    //  Service handler for reporting closed sockets.
    void SocketClosed(IN CSipClientSocket& rSocket,
                      IN ISipTransportObserver::EClosureType eClosureType);

    // Summary:
    //  Releases the resources used by the service.
    void ReleaseService();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSipKeepAliveSvc(CSipConnectionSvc* pSipConnectionSvc);

    // Summary:
    //  Destructor.
    virtual ~CSipKeepAliveSvc();

    // Private const, enums and structs
private:
    enum ECrlfKeepAliveState
    {
        eINIT,                      // The connection has been added to the list. Send first ping.
        eSEND_PING,                 // Send ping.
        eWAITING_FIRST_PONG,        // Waits for the first pong.
        eWAITING_PONG,              // Wait pong.
        eCRLF_FAILED                // Failed: the attended pong was not received.
    };


    struct SCrlfKeepAliveConnStatus
    {
        CSipClientSocket* m_pSipSocket;           // The socket used by this connection.
        ECrlfKeepAliveState  m_eState;            // CrlfKeepAlive state.
        uint32_t m_uNextEventTimeMS;              // Time of the next event associated to this connection.
        uint32_t m_uNextPingTimeMS;               // Next ping time.
        uint32_t m_uKeepAliveIntervalMS;          // Configured ping interval. See note below.
        uint32_t m_uMaxServerResponseTimeMS;      // Configured max response time.
    };

    // Note: When a ping is sent, the time when the next ping should be sent is also computed and
    // saved in the structure. In this way, the interval between pings will not be affected by
    // the server response time.


//-- Hidden Methods
protected:
private:


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipKeepAliveSvc(IN const CSipKeepAliveSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipKeepAliveSvc& operator=(IN const CSipKeepAliveSvc& rSrc);

    // Summary:
    //  Sends a "ping" (a double CRLF) on the specified connection.
    void SendCrlf(IN SCrlfKeepAliveConnStatus*  pstCrlfCrl);

    // Summary:
    //  Informs the manager about a missing "pong" response.
    void CrlfKeepAliveTimeout(IN SCrlfKeepAliveConnStatus*  pstCrlfCtrl);

    // Summary:
    //  Manages events for the superviced CRLF keep alive connections.
    void ManageCrlfKeepAliveConnections();

    // Summary:
    //  Returns the system up time in 32 bits format.
    uint32_t GetCurrentTimeMark();

    // Summary:
    //  Returns a random value equal in the interval 0.8 to 1.0 times the referenc value.
    uint32_t RandomizeInterval(IN uint32_t uReValue);

    // Summary:
    //  Searches for the specified socket in the list of supervised connections.
    CSipKeepAliveSvc::SCrlfKeepAliveConnStatus* SearchForSocket(IN CSipClientSocket* pSocket,
                                                                INOUT unsigned int* puIndex = NULL);

    // Summary:
    //  Checks if a received UPD packet is a STUN binding response.
    bool IsStunBindingResponse();


    //-- << CSipTransportSvc >>.

    virtual mxt_result SpecificShutdown(IN ISipTransportUser* pUser,
                                        IN mxt_opaque opq);
    //-- << CEventDriven >>.

    void EvTimerServiceMgrAwaken(IN bool bStopped,
                                 IN unsigned int uTimer,
                                 IN mxt_opaque opq);

    //-- <<ISipTransportUser>
    virtual void EvCommandResult(IN mxt_result res, IN mxt_opaque opq);

// Friends declarations

//-- Hidden Data Members
protected:
private:

    bool m_bEnableCrLf;                                             // Keep alive service CRLF enable flag.
    bool m_bShutDown;                                               // Shut down flag;
    uint32_t m_uNextEventTime;                                      // Time of the next event.
    uint32_t m_uCurrentTime;                                        // Current time.
    ISipKeepAliveMgr* m_pKeepAliveMgr;                              // Manager (EvFlowSucceded and EvFlowFailure)
    ISipKeepAliveExtensionMgr* m_pKeepAliveExtensionMgr;            // Extension manager (EvDataReceived)
    CSipConnectionSvc* m_pConnectionSvc;                            // Connection service.
    CList<SCrlfKeepAliveConnStatus>  m_lstCrlfKeepAliveConnStatus;  // List of connections supervised w/CRLF.
};

//==============================================================================
//==
//==  SetKeepAliveMgr
//==
//==============================================================================
//
//  Parameters:
//      pMgr:
//       Pointer to the manager.
//
//  Description:
//      Sets keep alive manager. EvFlowSucceded and EvFlowFailure events will
//      be reported to this manager.
//
//==============================================================================
inline
void CSipKeepAliveSvc::SetKeepAliveMgr(IN ISipKeepAliveMgr* pMgr)
{
    m_pKeepAliveMgr = pMgr;
}

//==============================================================================
//==
//==  SetKeepAliveExtensionMgr
//==
//==============================================================================
//
//  Parameters:
//      pMgr:
//       Pointer to the manager.
//
//  Description:
//      Sets keep alive extension manager. This manager is used to send and
//      receive non SIP data. It can be use to send and receive STUN keep alive
//      packets.
//
//==============================================================================
inline
void CSipKeepAliveSvc::SetKeepAliveExtensionMgr(IN ISipKeepAliveExtensionMgr* pMgr)
{
    m_pKeepAliveExtensionMgr = pMgr;
}


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif // --#ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
#endif //-- #ifndef MXG_CSIPKEEPALIVESVC_H

