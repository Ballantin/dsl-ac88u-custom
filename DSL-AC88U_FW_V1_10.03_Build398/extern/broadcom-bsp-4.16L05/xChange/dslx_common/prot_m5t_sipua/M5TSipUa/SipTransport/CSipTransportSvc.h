//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSIPTRANSPORTSVC_H
#define MXG_CSIPTRANSPORTSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members
//-----------------
#ifndef CBLOB_H
#include "Cap/CBlob.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_ISIPTRANSPORTOBSERVER_H
#include "SipTransport/ISipTransportObserver.h"
#endif

#ifndef MXG_CSIPSERVERSOCKET_H
#include "SipTransport/CSipServerSocket.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class CSipClientSocket;
class ISipTransportUser;
class IEComUnknown;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipTransportSvc
//========================================
//
// Description:
//   The transport service represents the base class of all SIP transport
//   services.
//
//   A transport service usually represents a single feature implemented at the
//   transport layer. A single instance of a service is created at the SIP
//   transport level and is shared across all connections. As such, a transport
//   service must not keep the connection-specific state but it can keep the
//   state that can apply equally across all existing connections.
//
//   Each instance of the different connection services are placed in a linked
//   list, and this list is what is actually shared across all connections. Both
//   ends of this list can serve as entry points in the list. The core side of
//   this list corresponds to the first service used when sending data on the
//   network and to the last service to be used for data that has been received
//   from the network. The network side corresponds to the first service used
//   when receiving network data and the last service used when sending data on
//   the network.
//
//   The first service on the core side of the list is a special service called
//   the core side service. Likewise, the first service on the network side is
//   the network side service. Both of these services have special
//   responsibilities.
//
//<CODE>
//
//  CORE <-> CoreSideSvc <-> SomeSvc1 <-> SomeSvc2 <-> ... <-> NetworkSideSvc <-> NETWORK
//
//</CODE>
//
//   When raw data is received on the network side, it transits through the
//   service chain and, at some point, a service interprets this raw data as a
//   SIP packet. After this point, what transits through the services is a SIP
//   packet instead of raw data. Likewise, when sending a SIP packet on the
//   core side, the services transit the packet towards the network side and at
//   some point, the SIP packet is transformed into raw data, where it continues
//   its transition towards the network side.
//
// Location:
//   SipTransport/CSipTransportSvc.h
//
//==EDOC========================================================================
class CSipTransportSvc
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipTransportSvc();

    // Summary:
    //  Shuts down the services, freeing resources.
    //----------------------------------------------
    mxt_result Shutdown(IN ISipTransportUser* pUser,
                        IN mxt_opaque         opq);


    //-- << Reception >>.
    //--------------------

    // Summary:
    //  Service handler for complete SIP packet received from the network.
    //---------------------------------------------------------------------
    virtual void ReceivedFromNetwork(IN CSipPacket*       pPacket,
                                     IN CSipClientSocket* pSocket);

    //-- << Transmission >>.
    //-----------------------

    //  Summary:
    //    Service handler for sending complete SIP packet on the network.
    //--------------------------------------------------------------------
    virtual mxt_result SendToNetwork(IN CSipPacket&        rPacketOut,
                                     IN CSipClientSocket*  pSource,
                                     IN ISipTransportUser* pUser,
                                     IN mxt_opaque         opq);


    //-- << Socket Management >>.
    //----------------------------

    //  Summary:
    //    Service handler for reporting connected sockets.
    //-----------------------------------------------------
    virtual void SocketConnected(IN CSipClientSocket& rSocket);

    //  Summary:
    //    Service handler for reporting network error.
    //-------------------------------------------------
    virtual void SocketError(IN CSipClientSocket& rSocket,
                             IN mxt_result        result);

    //  Summary:
    //    Service handler for reporting closed sockets.
    //--------------------------------------------------
    virtual void SocketClosed(IN CSipClientSocket&                   rSocket,
                              IN ISipTransportObserver::EClosureType eClosureType);

    //  Summary:
    //    Service handler for reporting network error.
    //-------------------------------------------------
    virtual void SocketError(IN CSipServerSocket& rSocket,
                             IN mxt_result        result);

    //  Summary:
    //    Service handler for reporting closed sockets.
    //--------------------------------------------------
    virtual void SocketClosed(IN CSipServerSocket& rSocket);
    
    //==SDOC====================================================================
    //==
    //==  ReleaseService
    //==
    //==========================================================================
    //
    //  Summary:
    //      Releases the resources used by the service.
    //
    //  Description:
    //      This method initiates the resource release mechanism. If the
    //      object needs a release mechanism to release itself, it should
    //      begin in this method. If there is no release mechanism for the
    //      object, it must delete itself here.
    //
    //==EDOC====================================================================
    virtual void ReleaseService() = 0;

//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Destructor. Must call ReleaseService instead of directly access this
    //  destructor.
    //-----------------------------------------------------------------------
    virtual ~CSipTransportSvc();

    // Summary:
    //  Initializes the service chain.
    //---------------------------------
    virtual mxt_result Initialize(IN TO CSipTransportSvc* pNextSender,
                                  IN CSipTransportSvc* pNextReceiver);


private:

    //==SDOC====================================================================
    //==
    //==  SpecificShutdown
    //==
    //==========================================================================
    //
    //  Summary:
    //      Begins shutdown on the current object.
    //
    //  Parameters:
    //    pUser:
    //      The transport user that has to be notified when shutdown is
    //      completed.
    //
    //    opq:
    //      The opaque data that has to be returned into the EvCommandResult
    //      method.
    //
    //  Returns:
    //    resS_OK:
    //      The shutdown process is properly started.
    //
    //    resFE_SIPTRANSPORT_SHUTDOWN:
    //      The shutdown process is already started.
    //
    //  Description:
    //      This method initiates the specific object shutdown mechanism. In
    //      this method, the object starts the shutdown of the objects on which
    //      is has ownership.
    //
    //      When the shutdown mechanism is completed, the object must call
    //      EvCommandResult on pUser, with resS_OK and the opaque value taken
    //      from this method.
    //
    //      If there is no shutdown mechanism to initiate, the method must only
    //      call EvCommandResult on pUser with resS_OK and the opaque value
    //      taken from this method.
    //
    //      Each call to this method MUST eventually result in a call to
    //      EvCommandResult. This is true even when the method fails (in which
    //      case EvCommandResult must be called with the error result returned
    //      by this method).
    //
    //      IMPORTANT: this method MUST NOT release the instance. It must only
    //      shut it down.
    //
    //==EDOC====================================================================
    virtual mxt_result SpecificShutdown(IN ISipTransportUser* pUser,
                                        IN mxt_opaque         opq) = 0;

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipTransportSvc(IN const CSipTransportSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipTransportSvc& operator=(IN const CSipTransportSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:

    //  The next element of the service chain.
    //-----------------------------------------
    CSipTransportSvc* m_pNextSender;

    //  The previous element of the service chain.
    //---------------------------------------------
    CSipTransportSvc* m_pNextReceiver;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPTRANSPORTSVC_H

