//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPTRANSPORTOBSERVER_H
#define MXG_ISIPTRANSPORTOBSERVER_H

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


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" // ETransport
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class CSocketAddr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipTransportObserver
//========================================
// <GROUP SIPTRANSPORT_CLASSES>
//
// Description:
//   This interface defines the necessary APIs used by the transport manager to
//   notify interested modules of various events detected on the SIP transport
//   module.
//
// Location:
//   SipTransport/ISipTransportObserver.h
//
// See Also:
//   ISipTransportMgr
//
//==EDOC========================================================================
class ISipTransportObserver
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    // Description:
    //  Indicates the priority of the observer.
    enum EInsertObserverPriority
    {
        // Observer will be added in low priority.
        //-----------------------------------------
        eLOW_PRIORITY_OBSERVER,

        // Observer will be added in high priority.
        //------------------------------------------
        eHIGH_PRIORITY_OBSERVER
    };

    // Description:
    //  Indicates the closure type.
    enum EClosureType
    {
        // Description:
        //  Closed locally.
        eLOCAL,
        // Description:
        //  Closed remotely.
        eREMOTE
    };

    //==SDOC====================================================================
    //==
    //==  EvPacketReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Callback used by the transport manager when a SIP packet is
    //      received.
    //
    //  Parameters:
    //    rPacket:
    //      The received SIP packet.
    //
    //    bHandled:
    //      True when the packet is handled by a previously called observer,
    //      false otherwise.
    //
    //  Returns:
    //      Must return resSW_SIPTRANSPORT_PACKET_UNHANDLED if the
    //      implementation did not handle the packet.
    //
    //      Must return resS_OK if the implementation has accepted to handle the
    //      supplied packet.  The implementation of this event must never return
    //      resS_OK when bHandled is true.
    //
    //  Description:
    //      Event reporting the reception of a SIP packet to all observers.
    //      bHandled is used to let the observers after the one that actually
    //      manages the incoming packet know that a packet has been received, 
    //      but they do not need to handle it.
    //
    //==EDOC====================================================================
    virtual mxt_result EvPacketReceived(IN const CSipPacket& rPacket,
                                        IN bool              bHandled) = 0;


    //==SDOC====================================================================
    //==
    //==  EvTransportError
    //==
    //==========================================================================
    //
    //  Summary:
    //      Callback used by the transport manager when a network level error
    //      is detected.
    //
    //  Parameters:
    //
    //    res:
    //      The result corresponding to the actual network error.
    //
    //    rSource:
    //      Source address of the socket in error.
    //
    //    rDestination:
    //      Remote host where the socket was connected or trying to connect to.
    //
    //    eTransport:
    //      Associated transport.
    //
    //  Description:
    //      This event is generated when a network error is detected. The
    //      connection upon which the error occurred is automatically closed 
    //      when the error is detected.
    //
    //==EDOC====================================================================
    virtual void EvTransportError(IN       mxt_result    res,
                                  IN const CSocketAddr&  rSource,
                                  IN const CSocketAddr&  rDestination,
                                  IN       ESipTransport eTransport) = 0;


    //==SDOC====================================================================
    //==
    //==  EvConnectionClosed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Callback used by the transport manager to report the graceful
    //      termination of a connection.
    //
    //  Parameters:
    //    rSource:
    //      The source address of the connection.
    //
    //    rDestination:
    //      The peer address of the connection.
    //
    //    eTransport:
    //      The associated transport.
    //
    //    eClosureType:
    //      eLOCAL if locally initiated
    //      eREMOTE if initiated by the peer
    //
    //  Description:
    //      This event is reported by the transport manager after a socket is
    //      closed either locally or by the peer.
    //
    //==EDOC====================================================================
    virtual void EvConnectionClosed(IN const CSocketAddr&  rSource,
                                    IN const CSocketAddr&  rDestination,
                                    IN       ESipTransport eTransport,
                                    IN       EClosureType  eClosureType) = 0;


    //==SDOC====================================================================
    //==
    //==  EvConnectionEstablished
    //==
    //==========================================================================
    //
    //  Summary:
    //      Callback used by the transport manager to report the successful
    //      creation of a persistent connection.
    //
    //  Parameters:
    //    rSource:
    //      The source address of the connection.
    //
    //    rDestination:
    //      The peer address of the connection.
    //
    //    eTransport:
    //      The associated transport.
    //
    //  Description:
    //      This event is reported by the transport manager after a socket is
    //      successfully created for a persistent connection.
    //
    //==EDOC====================================================================
    virtual void EvConnectionEstablished(IN const CSocketAddr&  rSource,
                                         IN const CSocketAddr&  rDestination,
                                         IN       ESipTransport eTransport) = 0;


    //==SDOC====================================================================
    //==
    //==  EvObserverRemoved
    //==
    //==========================================================================
    //
    //  Summary:
    //      Callback used by the transport manager to report that the transport
    //      observer has been removed from its internal list and will no longer
    //      be notified of any event as a result.
    //
    //  Description:
    //      This event is reported by the transport manager when the transport
    //      manager shuts down or is asked to remove a transport manager from
    //      its internal list.  Therefore, it indicates that the transport
    //      observer has been removed from its list and will no longer be
    //      notified of any upcoming event as a result.
    //
    //==EDOC====================================================================
    virtual void EvObserverRemoved() = 0;



//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipTransportObserver(){};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipTransportObserver(){};


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipTransportObserver(IN const ISipTransportObserver& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipTransportObserver& operator=(IN const ISipTransportObserver& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTRANSPORTOBSERVER_H

