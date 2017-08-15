//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPSERVERSOCKET_H
#define MXG_CSIPSERVERSOCKET_H

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
#include "Config/SipStackCfg.h" // MXD_SIPSTACK_ENABLE_TLS
#endif

//-- Data Members
//-----------------
#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" //-- ESipTransport
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IASYNCSOCKETMGR_H
#include "Network/IAsyncSocketMgr.h"
#endif

#ifndef MXG_IASYNCSERVERSOCKETMGR_H
#include "Network/IAsyncServerSocketMgr.h"
#endif

//-- Template class
//------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class IAsyncSocket;
class IAsyncServerSocket;
class ISipServerSocketMgr;
class CSipTransportSvc;
class ISipTransportUser;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipServerSocket
//========================================
//
// Description:
//   Encapsulates a TCP or TLS server socket that is being listened on for
//   connection requests. This class manages the acceptation process of a remote
//   connection request that is asynchronous.
//
// Location:
//   SipTransport/CSipServerSocket.h
//
//==EDOC========================================================================
class CSipServerSocket : public IAsyncSocketMgr,
                         public IAsyncServerSocketMgr
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
    //  Constructor.
    //---------------
    CSipServerSocket(IN  CSipTransportSvc&    rNetworkSideSvc,
                     IN  ESipTransport        eTransport,
                     IN  const CSocketAddr&   rLocalAddr,
                     IN  ISipTransportUser*   pTransportUser,
                     IN  mxt_opaque           opq,
                     OUT mxt_result&          rres);

    // Summary:
    //  Destructor.
    //--------------
    ~CSipServerSocket();

    // Summary:
    //  Forces to close the embedded asynchronous socket.
    //----------------------------------------------------
    mxt_result Close();

    // Summary:
    //  Returns whether or not the socket is available (i.e., not closing).
    //----------------------------------------------------------------------
    bool IsAvailable() const;

    //  Summary:
    //  Returns the local IP and port address.
    //-----------------------------------------
    const CSocketAddr& GetLocalAddr() const;

    //  Summary:
    //  Returns the transport.
    //-------------------------
    ESipTransport GetTransport() const;

    //-- << IAsyncSocketMgr >>
    //--------------------------

    // Summary:
    //  Used to notify the socket that an error occurred.
    //----------------------------------------------------
    virtual void EvAsyncSocketMgrErrorDetected(IN mxt_opaque opq,
                                               IN mxt_result res);

    // Summary:
    //  Used to notify the socket that it has been closed locally.
    //-------------------------------------------------------------
    void EvAsyncSocketMgrClosed(IN mxt_opaque opq);

    // Summary:
    //  Used to notify the socket that it has been closed remotely.
    //--------------------------------------------------------------
    void EvAsyncSocketMgrClosedByPeer(IN mxt_opaque opq);

    //-- << IAsyncServerSocketMgr interface >>
    //------------------------------------------

    // Summary:
    //  Notifies that the server socket has been bound.
    //--------------------------------------------------
    void EvAsyncServerSocketMgrBound(IN mxt_opaque opq,
                                     IN CSocketAddr* pEffectiveLocalAddress);

    // Summary:
    //  Called when the transport layer notifies that a new connection is
    //  requested on this socket.
    //--------------------------------------------------------------------
    void EvAsyncServerSocketMgrConnectionRequested(IN mxt_opaque opqServerSocketOpaque);

    // Summary:
    //  Called when the SipNetwork accepted a connection request and now the
    //  whole acceptation process is complete.
    //-----------------------------------------------------------------------
    void EvAsyncServerSocketMgrConnectionAccepted(IN mxt_opaque         opqServerSocketOpaque,
                                                  IN TO IAsyncIoSocket* pAsyncIoSocket);

    // Summary:
    //  Called when the SipNetwork accepted a connection request and the
    //  acceptation process failed for whatever reason.
    //-------------------------------------------------------------------
    void EvAsyncServerSocketMgrConnectionFailed(IN mxt_opaque opqServerSocketOpaque,
                                                IN mxt_result res);
//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipServerSocket();

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipServerSocket();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipServerSocket(IN const CSipServerSocket& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipServerSocket& operator=(IN const CSipServerSocket& rSrc);

//-- Hidden Data Members
//------------------------
protected:
private:
    // Description:
    //  Allows to discriminate which socket can be used and which cannot.
    //--------------------------------------------------------------------
    bool m_bClosing;

    // The interface on which to accept the requested connection.
    //------------------------------------------------------------
    IAsyncSocket* m_pAsyncSocket;
    IAsyncServerSocket* m_pAsyncServerSocket;

    // The local IP and port address.
    //--------------------------------
    CSocketAddr m_localAddr;

    // The transport protocol.
    //-------------------------
    ESipTransport m_eTransport;

    // The service to which client sockets report network events.  Passed as
    // a parameter to the CSipClientSocket constructor.
    //-----------------------------------------------------------------------
    static CSipTransportSvc* ms_pNetworkSideSvc;

    // Transport user.
    ISipTransportUser* m_pTransportUser;

    // Opaque to pass in the EvCommandResult to the transport user.
    mxt_opaque m_opqTransportUser;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetLocalAddr
//==
//==============================================================================
//
//  Returns:
//       The local address.
//
//  Description:
//      Returns the local address.
//==EDOC=======================================================================
inline
const CSocketAddr& CSipServerSocket::GetLocalAddr() const
{
    return m_localAddr;
}

//==SDOC========================================================================
//==
//==  GetTransport
//==
//==============================================================================
//
//  Returns:
//    The transport protocol.
//
//  Description:
//    Returns the transport protocol.
//==EDOC=======================================================================
inline ESipTransport CSipServerSocket::GetTransport() const
{
    return m_eTransport;
}

//==SDOC========================================================================
//==
//==  IsAvailable
//==
//==============================================================================
//
//  Returns:
//    True if the socket is available, false otherwise.
//
//  Description:
//    Returns the state of the socket (either available or closing).
//==EDOC=======================================================================
inline bool CSipServerSocket::IsAvailable() const
{
    return (m_bClosing == false);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSERVERSOCKET_H

