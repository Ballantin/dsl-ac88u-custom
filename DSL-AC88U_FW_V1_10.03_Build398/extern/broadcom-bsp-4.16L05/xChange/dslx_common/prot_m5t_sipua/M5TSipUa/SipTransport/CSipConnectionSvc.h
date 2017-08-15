//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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
//==============================================================================
//
// Portions of this software Copyright (c) 2008-2011 Broadcom
// 
//==============================================================================

#ifndef MXG_CSIPCONNECTIONSVC_H
#define MXG_CSIPCONNECTIONSVC_H

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


//-- Data Members
//-----------------
#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CSIPCLIENTSOCKET_H
#include "SipTransport/CSipClientSocket.h"
#endif

#ifndef MXG_CSIPSERVERSOCKET_H
#include "SipTransport/CSipServerSocket.h"
#endif



//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSIPTRANSPORTSVC_H
#include "SipTransport/CSipTransportSvc.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

struct SConnectionItem
{
    // Time stamp of the last activity that occured on the connection.
    //-----------------------------------------------------------------
    uint64_t m_uLastActivityMs;

    // The position of the connection in the internal LRU list structure.
    //--------------------------------------------------------------------
    CListBase::SPtr m_element;
};


//==SDOC========================================================================
//== Class: CSipConnectionSvc
//========================================
//
// Description:
//  This class interfaces with the network layer. It creates, operates, times
//  out, and closes connections.
//
// ClientConnectionList:
//
//     CMap         CList[1]
//
// +--------+       +--------+
// |  key[2]|------>| socket |<--------------------------------------------+
// +--------+       +--------+                                             |
// +--------+                                                              |
// |        |                                                              |
// +--------+                                                              |
// +--------+                                                              |
// |        |                                                              |
// +--------+                                                              |
// +--------+       +--------+       +--------+       +--------+           |
// |        |------>| socket |------>| socket |------>| socket |           |
// +--------+       +--------+       +--------+       +--------+           |
//                      ^                ^                ^                |
//                      |                |                |                |
// LRUList:             |                |                |                |
//         CList        |                |                |                |
//                  +--------+       +--------+       +--------+      +--------+
//                  | socket |------>| socket |------>| socket |----->| socket |
//                  +--------+       +--------+       +--------+      +--------+
//
// ServerConnectionList / ListeningClientConnectionList:
//
// +--------+ +--------+
// |  key[2]| | socket |
// +--------+ +--------+
// +--------+ +--------+
// |        | |        |
// +--------+ +--------+
// +--------+ +--------+
// |        | |        |
// +--------+ +--------+
//
// [1]: handles collisions on peer addresses.  Items are differentiated by local addresses
// [2]: peer address + port + transport
//
// Location:
//   SipTransport/CSipConnectionSvc.h
//
// See Also:
//   CSipTransportSvc
//
//==EDOC========================================================================
class CSipConnectionSvc : public CSipTransportSvc,
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
    CSipConnectionSvc();

    // Summary:
    //  Initializes the connection service as a SipTransport service chain link.
    //---------------------------------------------------------------------------
    mxt_result Initialize(IN CSipTransportSvc* pNextSender,
                          IN CSipTransportSvc* pNextReceiver,
                          IN CSipTransportSvc& rNetworkSideSvc);

    // Summary:
    //  Sets the mandatory LRU connection list parameters.
    //-----------------------------------------------------
    static mxt_result SetConnectionParameters(IN unsigned int uMinConnections,
                                              IN unsigned int uMaxConnections,
                                              IN unsigned int uMaxIdleTimeS);

    // Summary:
    //  Handles incoming packets.
    //----------------------------
    virtual void ReceivedFromNetwork(IN CSipPacket*       pPacket,
                                     IN CSipClientSocket* pSocket);

    // Summary:
    //  Handles outgoing packets.
    //----------------------------
    virtual mxt_result SendToNetwork(IN CSipPacket&        rPacketOut,
                                     IN CSipClientSocket*  pSource,
                                     IN ISipTransportUser* pUser,
                                     IN mxt_opaque         opq);

    // Summary:
    //  Creates a server socket, in accordance with the given parameters.
    //--------------------------------------------------------------------
    mxt_result ListenOnNetwork(IN ESipTransport eTransport,
                               IN CSocketAddr&  rLocalAddress,
                               IN ISipTransportUser* pTransportUser,
                               IN mxt_opaque opq);

    // Summary:
    //  Removes a server socket, in accordance with the given parameters.
    //--------------------------------------------------------------------
    mxt_result StopListeningOnNetwork(IN ESipTransport eTransport,
                                      IN CSocketAddr&  rLocalAddress);

    // Summary:
    //  Creates a persistent client socket, in accordance with the given
    //  parameters.
    //-------------------------------------------------------------------
    mxt_result Connect(IN CSocketAddr&  rLocalAddr,
                       IN CSocketAddr&  rPeerAddr,
                       IN ESipTransport eTransport,
                       IN CString&      rstrPeerHostName,
// BRCM_CUSTOM - [mod] Add user opaque to persistent connection.
                       IN unsigned int  uDestinationId,
                       IN mxt_opaque    opqUser = 0,
                       IN bool          bCreateCnx = true);
// BRCM_CUSTOM - [end] Add user opaque to persistent connection.

    // Summary:
    //  Deletes a persistent client socket, in accordance with the given
    //  parameters.
    //-------------------------------------------------------------------
    mxt_result Disconnect(IN CSocketAddr&  rLocalAddr,
                          IN CSocketAddr&  rPeerAddr,
                          IN ESipTransport eTransport,
                          IN unsigned int  uDestinationId);

    // Summary:
    //  Gets the local port of an existing connection.
    //  The connection should have the specified local interface,
    //  peer address and transport.
    //-------------------------------------------------------------------
    mxt_result GetLocalPort(INOUT CSocketAddr& rLocalAddr,
                            IN const CSocketAddr& rPeerAddr,
                            IN ESipTransport eTransport,
                            IN unsigned int  uDestinationId);

    //-- << Connection lists management>>.
    //-------------------------------------

    // Summary:
    //  Finds, or creates if absent, a client connection according to the
    //  supplied packet parameters.
    //--------------------------------------------------------------------
    mxt_result GetClientConnection(IN  const CSocketAddr& rLocalAddr,
                                   IN  const CSocketAddr& rPeerAddr,
                                   IN  ESipTransport      eTransport,
#ifdef MXD_SIPSTACK_ENABLE_TLS
                                   IN  const CString&     rstrPeerHostName,
#endif
                                   IN  bool               bNewConnectionAllowed,
                                   IN  unsigned int       uDestinationId,
                                   OUT CSipClientSocket*& rpSocket);

    //-- << Socket Management >>.
    //----------------------------

    // Summary:
    //  Updates the connection lists, with respect to socket closure.
    //----------------------------------------------------------------
    virtual void SocketClosed(IN CSipClientSocket&                   rSocket,
                              IN ISipTransportObserver::EClosureType eClosureType);

    // Summary:
    //  Updates the connection lists, with respect to a socket being connected
    //  with the destination.
    //-------------------------------------------------------------------------
    void SocketConnected(IN TO CSipClientSocket& rSocket);

    // Summary:
    //  Updates the connection lists, with respect to operating errors on the
    //  given socket.
    //------------------------------------------------------------------------
    void SocketError(IN CSipClientSocket& rSocket, IN mxt_result result);

    // Summary:
    //  Service handler for reporting network error.
    //-----------------------------------------------
    virtual void SocketError(IN CSipServerSocket& rSocket,
                             IN mxt_result        result);

    // Summary:
    //  Service handler for reporting closed sockets.
    //------------------------------------------------
    void SocketClosed(IN CSipServerSocket& rSocket);

    // Summary:
    //  Releases the resources used by the service.
    //----------------------------------------------
    virtual void ReleaseService();

    // Summary:
    //  Hashes the address and transport ID into a key for the map.
    unsigned int Hash(IN const CSocketAddr&  rAddr,
                      IN const ESipTransport eTransportId);

    // Summary :
    //  Tells if all sockets are closed.
    //-----------------------------------
    bool AreAllSocketsClosed();

    // Summary:
    //  Returns the client socket listening to the supplied address.
    //---------------------------------------------------------------
    unsigned int GetListeningClientConnection(IN  const CSocketAddr& rLocalAddr,
                                              IN  ESipTransport      eTransport,
                                              OUT CSipClientSocket*& rpSocket,
                                              IN bool bCheckAvailability = false);

    // Summary:
    //  Returns the server socket listening to the supplied address.
    //---------------------------------------------------------------
    unsigned int GetServerConnection(IN  const CSocketAddr& rLocalAddr,
                                     OUT CSipServerSocket*& rpSocket);

    // Summary:
    //  Returns true if the client connection is present in the list.
    //----------------------------------------------------------------
    bool IsClientConnectionActive(IN const CSipClientSocket& rSocket);

//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Destructor. Must call ReleaseService instead of directly access this
    //  destructor.
    //-----------------------------------------------------------------------
    virtual ~CSipConnectionSvc();


    //-- << CEventDriven >>.
    //-----------------------

    void EvTimerServiceMgrAwaken(IN bool bStopped,
                                 IN unsigned int uTimer,
                                 IN mxt_opaque opq);

private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipConnectionSvc();


    //-- << CSipTransportSvc >>.
    //----------------------------

    // Summary:
    //  Begins shutdown on the current object.
    //-----------------------------------------
    virtual mxt_result SpecificShutdown(IN ISipTransportUser* pUser,
                                        IN mxt_opaque         opq);


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipConnectionSvc(IN const CSipConnectionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipConnectionSvc& operator=(IN const CSipConnectionSvc& rSrc);

    //-- << CSipTransportSvc >>
    //---------------------------
    mxt_result Initialize(IN CSipTransportSvc* pNextSender,
                          IN CSipTransportSvc* pNextReceiver);

    //-- << Connection lists management>>.
    //-------------------------------------

    // Summary:
    //  Inserts a new connection in the list.
    //----------------------------------------
    mxt_result InsertClientConnection(IN TO CSipClientSocket* pSocket,
                                      IN bool bTimed = true);

    // Summary:
    //  Removes an existing client connection.
    //-----------------------------------------
    mxt_result RemoveClientConnection(IN CSipClientSocket& rSocket);

    // Summary:
    //  Removes an existing listening client connection.
    //---------------------------------------------------
    mxt_result RemoveListeningClientConnection(IN CSipClientSocket& rSocket);

    // Summary:
    //  Removes an existing server connection.
    //-----------------------------------------
    mxt_result RemoveServerConnection(IN CSipServerSocket& rSocket);

    // Summary:
    //  Resets the connection's idle time.
    //-------------------------------------
    void NetworkActivity(CSipClientSocket& rSocket);

    // Summary:
    //  Updates the client connections and the list timer.
    //-----------------------------------------------------
// BRCM_CUSTOM - [mod] Add flag to update timer in case there is only
//                     1 user in the list
    void ManageClientConnections(IN bool bUpdateTimer = false);
// BRCM_CUSTOM - [end] Add flag to update timer in case there is only
//                     1 user in the list

    // Summary:
    //  Reports that shutdown has been completed to the transport user if
    //  needed.
    //--------------------------------------------------------------------
    void ReportCompletedShutDownIfNeeded();

    // Summary:
    //  Removes a connection from the LRU list.
    //------------------------------------------
    void RemoveLruItem(IN CSipClientSocket& rSocket);


//-- Hidden Data Members
//------------------------
protected:
private:
    // The maximal idle time threshold.
    //----------------------------------
    static unsigned int ms_uMaxIdleTimeMs;

    // The minimal number of active connections.
    //-------------------------------------------
    static unsigned int ms_uMinConnections;

    // The maximum number of active connections.
    //-------------------------------------------
    static unsigned int ms_uMaxConnections;

    // The network side service (the one that interacts directly with the SIP
    // sockets).
    //------------------------------------------------------------------------
    CSipTransportSvc* m_pNetworkSideSvc;

    // The LRU list is used to monitor the connections' idle time. This list
    // only monitors connected sockets.
    //
    // Note: GetFirstIndex() is the position of the LRU connection and
    // conversely, GetLastIndex() is the position of the MRU position.
    //-----------------------------------------------------------------
    CList<CSipClientSocket*> m_lstpLruConnections;

    // Client connection list.
    //-------------------------
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
    // GCC 2.7.2 does not support default parameters in templates,
    // in which case all the parameters must be specified.
    CMap<unsigned int, CList<CSipClientSocket*>*,
        CAATree<CMapPair<unsigned int, CList<CSipClientSocket*>*> > > m_mapClientConnections;
#else
    CMap<unsigned int, CList<CSipClientSocket*>* > m_mapClientConnections;
#endif

    // Server connection list.
    //-------------------------
    CList<CSipServerSocket*> m_lstpServerConnections;

    // List of client socket acting as a server sockets for connection-less
    // transports.
    //----------------------------------------------------------------------
    CList<CSipClientSocket*> m_lstpListeningClientConnections;

    // The reference to the LRU connection.
    //--------------------------------------
    CSipClientSocket* m_pLruConnection;

    // Tell that we are shutting down the stack.
    //-------------------------------------------
    bool m_bShuttingDown;

    // The transport user to warn when shutting down was completed.
    //--------------------------------------------------------------
    ISipTransportUser* m_pShuttingDownUser;

    // The opaque data to return to the transport user when shutting down was
    // completed.
    //------------------------------------------------------------------------
    mxt_opaque m_opqShuttingDownData;

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


inline
mxt_result CSipConnectionSvc::Initialize(IN CSipTransportSvc* pNextSender,
                                         IN CSipTransportSvc* pNextReceiver)
{
    return CSipTransportSvc::Initialize(pNextSender,
                                        pNextReceiver);
}

inline bool CSipConnectionSvc::AreAllSocketsClosed()
{
    return (m_mapClientConnections.GetSize() == 0 &&
            m_lstpServerConnections.GetSize() == 0 &&
            m_lstpListeningClientConnections.GetSize() == 0);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCONNECTIONSVC_H
