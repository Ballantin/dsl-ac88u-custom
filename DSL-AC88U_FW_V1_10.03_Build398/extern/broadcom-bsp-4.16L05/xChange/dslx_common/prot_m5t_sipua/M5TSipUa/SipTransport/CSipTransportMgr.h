//==SDOC========================================================================
//==============================================================================
//
//   Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_CSIPTRANSPORTMGR_H
#define MXG_CSIPTRANSPORTMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SIP Stack Global Configuration
//-----------------------------------
#ifndef MXG_SIPSTACKCFG_H
// MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT
//-------------------------------------------------
#include "Config/SipStackCfg.h"
#endif

#ifndef MXG_ISIPTRANSPORTMGR_H
#include "SipTransport/ISipTransportMgr.h"
#endif

#ifndef MXG_ISIPTRANSPORTUSER_H
#include "SipTransport/ISipTransportUser.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CSIPTRANSPORTSVC_H
#include "SipTransport/CSipTransportSvc.h"
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)
#ifndef MXG_ISIPCORECONFIG_H
#include "SipCore/ISipCoreConfig.h"  // ISipCoreConfig::ESpirallingSvcState
#endif
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CMarshaler;
class CSipConnectionSvc;
class CSipKeepAliveSvc;
class CSipParserSvc;
#if defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
class CSipNetworkReceptionControlSvc;
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
class CSipPacketReceptionControlSvc;
#endif
class CSipSpirallingSvc;
class ISipTransportObserver;

//BRCM_CUSTOM - [begin] Add custom transport service.
class CBrcmTransportSvc;
//BRCM_CUSTOM - [end] Add custom transport service.

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipTransportMgr
//========================================
//
// Description:
//  This class implements ISipTransportMgr and inherits from CSipTransportSvc,
//  by its nature of being the core-side SipTransport service. It deals with SIP
//  packet transmission requests and it is initiating the hopping between the
//  service chain links. It also creates the chain in the proper order,
//  initializing the services with the specified parameters it received from the
//  core side of the stack.
//
// Location:
//   SipTransportMgr/CSipTransportMgr.h
//
// See Also:
//   CSipTransportMgr,CSipTransportSvc
//
//==EDOC========================================================================
class CSipTransportMgr : public ISipTransportMgr,
                         private ISipTransportUser,
                         private CSipTransportSvc,
                         private CEventDriven
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipTransportMgr();


    //-- << ISipTransportMgr Interface >>
    //--------------------------------------------

    virtual mxt_result Initialize();

    virtual void AddObserverA(
              IN ISipTransportObserver*                         pObserver,
              IN ISipTransportObserver::EInsertObserverPriority ePriority =
                    ISipTransportObserver::eLOW_PRIORITY_OBSERVER);

    virtual void RemoveObserverA(IN ISipTransportObserver* pObserver);

    virtual void ConnectA(IN const CSocketAddr& rLocalAddr,
                          IN const CSocketAddr& rPeerAddr,
                          IN ESipTransport eTransport,
                          IN const CString& rstrPeerHostName,
                          IN ISipTransportUser* pUser,
                          IN mxt_opaque opqUserData,
// BRCM_CUSTOM - [mod] Add user opaque to persistent connection.
                          IN unsigned int uDestinationId,
                          IN mxt_opaque opqUser = 0,
                          IN bool bCreateCnx = true);
// BRCM_CUSTOM - [end] Add user opaque to persistent connection.

    virtual void DisconnectA(IN const CSocketAddr& rLocalAddr,
                             IN const CSocketAddr& rPeerAddr,
                             IN ESipTransport eTransport,
                             IN ISipTransportUser* pUser,
                             IN mxt_opaque opqUserData,
                             IN unsigned int uDestinationId);

    virtual mxt_result GetLocalPortS(INOUT CSocketAddr& rLocalAddr,
                                     IN const CSocketAddr& rPeerAddr,
                                     IN ESipTransport eTransport,
                                     IN unsigned int uDestinationId);

    virtual mxt_result ListenA(IN ESipTransport      eTransport,
                               IN const CSocketAddr& rLocalAddress,
                               IN ISipTransportUser* pUser,
                               IN mxt_opaque         opq);

    virtual void StopListeningA(IN ESipTransport      eTransport,
                                IN const CSocketAddr& rLocalAddress,
                                IN ISipTransportUser* pUser,
                                IN mxt_opaque         opq);

    virtual void SendA(IN const CSipPacket&  rPacketOut,
                       IN ISipTransportUser* pUser,
                       IN mxt_opaque         opq);

    virtual void ShutdownA(IN ISipTransportUser& rUser,
                           IN mxt_opaque         opq);

#if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)
    // Summary
    //  Set the spiralling service state.
    //------------------------------------
    static void SetSpirallingSvcState(IN ISipCoreConfig::ESpirallingSvcState eState);
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)

    virtual void ReHandleEvPacketReceivedA(IN const CSipPacket& rPacket);

    virtual void AddListenAddressInParserSvcA(IN const CString& rstrAddress,
                                              IN uint16_t uPort,
                                              IN const CVector<CString>* pvecFqdn);

    virtual void RemoveListenAddressInParserSvcA(IN const CString& rstrAddress,
                                                 IN uint16_t uPort,
                                                 IN const CVector<CString>* pvecFqdn);

    void Release();

//BRCM_CUSTOM - [begin] Add custom transport service.
    static void SetBrcmTransportSvc(IN TO CBrcmTransportSvc* pSvc);
    
    static CBrcmTransportSvc* GetBrcmTransportSvc();
//BRCM_CUSTOM - [end] Add custom transport service.

#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

    virtual void SetKeepAliveMgrA(IN ISipKeepAliveMgr* pMgr);

    virtual void SetKeepAliveExtensionMgrA(IN ISipKeepAliveExtensionMgr* pMgr);

    virtual void SendA(IN ESipTransport eTransport,
                       IN const CSocketAddr& rLocalAddr,
                       IN const CSocketAddr& rPeerAddr,
                       IN const uint8_t* puData,
                       IN unsigned int uSize,
                       IN ISipTransportUser* pUser,
                       IN mxt_opaque opq,
                       IN unsigned int uDestinationId);

    virtual void EnableOutboundCrlfKeepAliveA(IN bool bEnable);

    virtual void StartOutboundCrlfKeepAliveA(IN ESipTransport eTransport,
                                             IN const CSocketAddr& rLocalAddr,
                                             IN const CSocketAddr& rPeerAddr,
                                             IN unsigned int uIntervalS,
                                             IN unsigned int uServerResponseTimeS,
                                             IN ISipTransportUser* pUser,
                                             IN mxt_opaque opq,
                                             IN unsigned int uDestinationId);

#endif //-- #if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
    virtual void SetNetworkReceptionControlMgrA(IN ISipNetworkReceptionControlMgr* pMgr);

    virtual void SetPeerFloodBlockingA(IN uint32_t uRawPacketCountLimit,
                                       IN uint32_t uReceptionWindowSizeMs,
                                       IN uint32_t uBlockingTimeoutMs);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
    virtual void SetPacketReceptionControlMgrA(IN ISipPacketReceptionControlMgr* pMgr);
    virtual void EnablePeerFilteringA(IN bool bEnable);
    virtual void ModifyApprovedPeerListA(IN const CSocketAddr& rPeerAddr, IN bool bIsApproved);
    virtual void RemoveAllApprovedPeersA();
    virtual void GetApprovedPeerList(OUT CVector<CSocketAddr>& rvecApprovedPeers);
    virtual void ModifyAllowedMethodListA(IN const char* pszMethod, IN bool bIsAllowed);
    virtual void RemoveAllAllowedMethodsA();
    virtual void GetAllowedMethodList(OUT CVector<CString>& rvecstrAllowedMethods);
#endif


//-- Hidden Methods
protected:

    // Summary:
    //  Destructor. Must call ReleaseService instead of directly access this
    //  destructor.
    //-----------------------------------------------------------------------
    virtual ~CSipTransportMgr();


private:

    //-- << ISipTransportUser >>.
    //----------------------------

    // Summary:
    //  Notifies the transport user of the result of an operation.
    //-------------------------------------------------------------
    virtual void EvCommandResult(IN mxt_result res,
                                 IN mxt_opaque opq);


    //-- << CSipTransportSvc Methods >>.
    //-----------------------------------

    virtual void ReceivedFromNetwork(IN CSipPacket* pPacket,
                                     IN CSipClientSocket* pSocket);

    virtual void SocketConnected(IN CSipClientSocket& rSocket);

    virtual void SocketError(IN CSipClientSocket& rSocket,
                             IN mxt_result result);

    virtual void SocketClosed(IN CSipClientSocket&                   rSocket,
                              IN ISipTransportObserver::EClosureType eClosureType);

    virtual mxt_result SpecificShutdown(IN ISipTransportUser* pUser,
                                        IN mxt_opaque opq);

    virtual void ReleaseService();


    //-- << Internal methods >>.
    //---------------------------

    // Summary:
    //  Adds a transport observer to the list of observers - internal method.
    //------------------------------------------------------------------------
    void InternalAddObserverA(IN CMarshaler* pParams);

    // Summary:
    //  Removes a transport observer from the list of observers - internal
    //  method.
    //---------------------------------------------------------------------
    void InternalRemoveObserverA(IN CMarshaler* pParams);

    // Summary:
    //  Adds a new persistent connection or reestablishes a lost connection -
    //  internal method.
    //------------------------------------------------------------------------
    void InternalConnectA(IN CMarshaler* pParams);

    // Summary:
    //  Removes a persistent connection - internal method.
    //-----------------------------------------------------
    void InternalDisconnectA(IN CMarshaler* pParams);

    // Summary:
    //  Asks the service chain to create a listening connection for the given
    //  address and port - internal method.
    //------------------------------------------------------------------------
    void InternalListenA(IN CMarshaler* pParams);

    // Summary:
    //  Asks the service chain to end a listening connection for the given
    //  address and port - internal method.
    //---------------------------------------------------------------------
    void InternalStopListeningA(IN CMarshaler* pParams);

    // Summary:
    //  Asks the service chain to send a message included in the packet -
    //  internal method.
    //--------------------------------------------------------------------
    void InternalSendA(IN CMarshaler* pParams);

    // Summary:
    //  Shuts down the service chain - internal method.
    //--------------------------------------------------
    void InternalShutdownA(IN CMarshaler* pParams);

    // Summary:
    //   Gets the local port of a connection.
    //-------------------------------------------
    void InternalGetLocalPortS(IN CMarshaler* pParams);

    // Summary:
    //  Completes the shutdown process and releases the object.
    //----------------------------------------------------------
    void CompleteShutdown(IN ISipTransportUser* pUser,
                          IN mxt_opaque         opq);

    // Summary:
    //  Sends a message to asynchronously warn the transport user that shutdown
    //  has been terminated.
    //--------------------------------------------------------------------------
    void ShutdownCompletedA(IN ISipTransportUser* pUser,
                            IN mxt_opaque         opq);

    // Summary:
    //  Notifies the transport user that shutdown has been completed or releases
    //  the service if there is no transport user to warn.
    void InternalShutdownCompletedA(IN CMarshaler* pParams);

    // Summary:
    //  Shows the SIP packet to the list of observers.
    void InternalReHandleEvPacketReceivedA(IN CMarshaler* pParams);

    // Summary:
    //  Passes the address to add to the ParserSvc.
    void InternalAddListenAddressInParserSvcA(IN CMarshaler* pParams);

    // Summary:
    //  Passes the address to remove to the ParserSvc.
    void InternalRemoveListenAddressInParserSvcA(IN CMarshaler* pParams);

#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

    // Summary:
    //  Set the keep alive service manager.
    void InternalSetKeepAliveMgrA(IN CMarshaler* pParams);

    // Summary:
    //  Set the keep alive service extension manager.
    void InternalSetKeepAliveExtensionMgrA(IN CMarshaler* pParams);

    // Summary:
    //  Starts the Crlf keep alive mechanism on the specified connection.
    void InternalStartOutboundCrlfKeepAliveA(IN CMarshaler* pParams);

    // Summary:
    //  Sends an application packet to the network using an UDP transport.
    void InternalSendExtensionA(IN CMarshaler* pParams);

    // Summary:
    //  Enables the crlf keep alive mechanism
    void InternalEnableOutboundCrlfKeepAliveA(IN CMarshaler* pParams);

#endif //-- #if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
    // Summary:
    //  Set the network reception control service manager.
    void InternalSetNetworkReceptionControlMgrA(IN CMarshaler* pParams);

    // Summary:
    //  Configures the CSipNetworkReceptionControlSvc peer blocking option.
    void InternalSetPeerFloodBlockingA(IN CMarshaler* pParams);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
    // Summary:
    //  Set the packet reception control service manager.
    void InternalSetPacketReceptionControlMgrA(IN CMarshaler* pParams);

    // Summary:
    //  Set the packet reception control service manager.
    void InternalEnablePeerFilteringA(IN CMarshaler* pParams);

    // Summary:
    //  Modify the CSipPacketReceptionControlSvc peer filtering option approved peers list.
    void InternalModifyApprovedPeerListA(IN CMarshaler* pParams);

    // Summary:
    //  Removes all entries in the CSipPacketReceptionControlSvc peer filtering option approved peers list.
    void InternalRemoveAllApprovedPeersA(IN CMarshaler* pParams);

    // Summary:
    //  Gets the CSipPacketReceptionControlSvc peer filtering option approved peers list.
    void InternalGetApprovedPeerList(IN CMarshaler* pParams);

    // Summary:
    //  Modify the CSipPacketReceptionControlSvc peer filtering option allowed methods list.
    void InternalModifyAllowedMethodListA(IN CMarshaler* pParams);

    // Summary:
    //  Removes all entries in the CSipPacketReceptionControlSvc peer filtering option allowed methods list.
    void InternalRemoveAllAllowedMethodsA(IN CMarshaler* pParams);

    // Summary:
    //  Gets the CSipPacketReceptionControlSvc peer filtering option allowed method list.
    void InternalGetAllowedMethodList(IN CMarshaler* pParams);
#endif

    //-- << CEventDriven >>.

    // Summary:
    //  Called when an event is sent to the transport manager, when an
    //  asynchronous method has been called.  The corresponding internal
    //  method is then called.
    virtual void EvMessageServiceMgrAwaken(IN bool         bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler*  pParameter);

    //-- << Helper methods >>.
    // Summary:
    //  Show the packet to the list of transport observers.
    bool DispatchPacketToObservers(IN const CSipPacket& rPacket);

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipTransportMgr(IN const CSipTransportMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipTransportMgr& operator=(IN const CSipTransportMgr& rSrc);

    //-- << Startup mechanism >>
    //----------------------------

    static mxt_result InitializeCSipTransportMgr();

//-- Hidden Data Members
//------------------------
protected:
private:
    // Asynchrous events IDs.
    enum EMsgNumber
    {
        eADDOBSA,
        eREMOVEOBSA,
        eCONNECTA,
        eDISCONNECTA,
        eLISTENA,
        eSTOPLISTENINGA,
        eSENDA,
        eSETPRINCIPALBUFFERA,
        eSHUTDOWNA,
        eSHUTDOWNCOMPLETEDA,
        eGET_LOCAL_PORT,
        eREHANDLE_EVPACKETRECEIVEDA,
        eADDLOCALADDRESSINPARSERSVCA,
        eREMOVELOCALADDRESSINPARSERSVCA,
#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)
        eENABLE_OUTBOUND_CRLF_KEEP_ALIVE,
        eSETKEEPALIVEMGRA,
        eSETKEEPALIVEEXTMGRA,
        eSTARTOUTBOUNDCRLFKEEPALIVE,
        eSEND_EXTENSIONA,
#endif //-- #if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)
#if defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
        eSETNETWORKRECEPTIONCONTROLMGRA,
        eSET_PEER_FLOOD_BLOCKINGA,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
        eSETPACKETRECEPTIONCONTROLMGRA,
        eENABLEPEERFILTERINGA,
        eMODIFYAPPROVEDPEERLISTA,
        eREMOVEALLAPPROVEDPEERSA,
        eGETAPPROVEDPEERLIST,
        eMODIFYALLOWEDMETHODLISTA,
        eREMOVEALLALLOWEDMETHODSA,
        eGETALLOWEDMETHODLIST,
#endif
    };

    // The list of transport observers.
    CVector<ISipTransportObserver*> m_vecpObs;

    // The connection service.
    CSipConnectionSvc* m_pCxSvc;

    // The parser service.
    CSipParserSvc* m_pParserSvc;

#if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)
    // The spiralling service.
    CSipSpirallingSvc* m_pSpirallingSvc;

    // Whether or not the spiralling service is enabled.
    static bool ms_bSpirallingSvcEnable;
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)

//BRCM_CUSTOM - [begin] Add custom transport service.
    static CBrcmTransportSvc* ms_pBrcmSvc;
//BRCM_CUSTOM - [end] Add custom transport service.

#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)
    // The Keep Alive service.
    CSipKeepAliveSvc* m_pKeepAliveSvc;
#endif //-- #if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
    // The network reception control service.
    CSipNetworkReceptionControlSvc* m_pNetworkReceptionControlSvc;
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
    // The packet reception control service.
    CSipPacketReceptionControlSvc* m_pPacketReceptionControlSvc;
#endif

    // Tell that we are shuting down the stack.
    bool m_bShuttingDown;

    // Synchronize access to m_bShuttingDown.
    CMutex m_shuttingDownAccess;

    // Tells how many services need to complete shutdown before this object can
    // terminate its own shutdown.
    unsigned int m_uNbOfServicesToShutdown;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  ShutdownCompletedA
//==
//==============================================================================
//
//  Description:
//      Asynchronously warns the transport user that shutdown has been
//      completed.
//
//==EDOC========================================================================
inline void CSipTransportMgr::ShutdownCompletedA(IN ISipTransportUser* pUser,
                                                 IN mxt_opaque         opq)
{
    // The objects only calls this when shutting down.
    //-------------------------------------------------
    MX_ASSERT_ONLY( m_shuttingDownAccess.Lock(); )
    MX_ASSERT( m_bShuttingDown );
    MX_ASSERT_ONLY( m_shuttingDownAccess.Unlock(); )

    CMarshaler* pParams = CPool<CMarshaler>::New();

    *pParams << pUser << opq;

    PostMessage(false, eSHUTDOWNCOMPLETEDA, IN TO pParams);
}

//==SDOC========================================================================
//==
//==  Release
//==
//==============================================================================
//
//  Description:
//      Releases the resources used by this object.
//
//==EDOC========================================================================
inline void CSipTransportMgr::Release()
{
    CEventDriven::Release();
}

#if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)
//==SDOC========================================================================
//==
//==  SetSpirallingSvcState
//==
//==============================================================================
//
//  Parameters:
//    eState:
//      The new state of the Spiralling Service.
//
//  Description:
//      Configures whether or not the Spiralling Service is active. By default,
//      the service is active.
//
//==EDOC========================================================================
inline void CSipTransportMgr::SetSpirallingSvcState(IN ISipCoreConfig::ESpirallingSvcState eState)
{
    ms_bSpirallingSvcEnable = (eState == ISipCoreConfig::eACTIVE);
}
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)

//BRCM_CUSTOM - [begin] Add custom transport service.
//==SDOC========================================================================
//==
//==  SetBrcmTransportSvc
//==
//==============================================================================
//
//  Parameters:
//    pSvc:
//      The custom transport service to be inserted to the transport chain.
//      Ownership is taken by the function.
//
//  Description:
//      Set the custom transport service.  NULL if no custom transport service
//      to be inserted.  This function needs to be called before the stack
//      starts up.
//
//==EDOC========================================================================
inline void CSipTransportMgr::SetBrcmTransportSvc(IN TO CBrcmTransportSvc* pSvc)
{
    ms_pBrcmSvc = pSvc;
}

//==SDOC========================================================================
//==
//==  GetBrcmTransportSvc
//==
//==============================================================================
//
//  Return:
//      Pointer of the custom transport service instance.
//
//  Description:
//      Retrieve the custom transport service instance.  NULL if no custom
//      transport service has been inserted.
//
//==EDOC========================================================================
inline CBrcmTransportSvc* CSipTransportMgr::GetBrcmTransportSvc()
{
    return ms_pBrcmSvc;
}
//BRCM_CUSTOM - [end] Add custom transport service.

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSIPTRANSPORTMGR_H

