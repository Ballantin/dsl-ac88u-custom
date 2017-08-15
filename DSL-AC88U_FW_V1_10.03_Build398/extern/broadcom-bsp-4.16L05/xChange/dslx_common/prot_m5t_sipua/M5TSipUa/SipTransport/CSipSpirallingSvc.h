//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPSPIRALLINGSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSPIRALLINGSVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSIPTRANSPORTSVC_H
#include "SipTransport/CSipTransportSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipConnectionSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipSpirallingSvc
//========================================
//
// Description:
//   This class is responsible to detect when a SIP packet is about to be sent
//   in loopback to a local listening address. When this condition is detected,
//   this service breaks the transport service chain and gives the packet
//   directly to the Transport Manager. This mechanism is helpful to speed up
//   packet processing since it avoids serializing, parsing, sending to the
//   network, and receiving the packets from the network.
//
//   Note: The current implementation of the Spiralling Service traps packets
//   that are designated to any listening addresses of the SIP stack. When a SIP
//   packet is trapped by the Spiralling Service, its local and peer
//   addresses/ports are replaced by the original destination address/port. This
//   behaviour works fine when only one local address is configured in the SIP
//   stack. However, this may lead to unpredictable behaviours when more that
//   one local address is configured.
//
// Location:
//   SipTransport/CSipSpirallingSvc.h
//
// See Also:
//   CSipTransportSvc
//
//==EDOC========================================================================
class CSipSpirallingSvc : public CSipTransportSvc
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
    CSipSpirallingSvc();

    // Summary:
    //  Initializes the spiralling service as a SipTransport service chain link.
    //---------------------------------------------------------------------------
    mxt_result Initialize(IN CSipTransportSvc*  pNextSender,
                          IN CSipTransportSvc*  pNextReceiver,
                          IN CSipTransportSvc&  rStackSideSvc,
                          IN CSipConnectionSvc& rCxSvc);

    // Summary:
    //  Initializes the service chain.
    //---------------------------------
    virtual mxt_result Initialize(IN TO CSipTransportSvc* pNextSender,
                                  IN CSipTransportSvc* pNextReceiver);

    //-- << CSipTransportSvc >>
    //---------------------------

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
    //  Releases the resources used by the service.
    //----------------------------------------------
    virtual void ReleaseService();

//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Begins shutdown on the current object.
    //-----------------------------------------
    virtual mxt_result SpecificShutdown(IN ISipTransportUser* pUser,
                                        IN mxt_opaque         opq);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipSpirallingSvc(IN const CSipSpirallingSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipSpirallingSvc& operator=(IN const CSipSpirallingSvc& rSrc);

    //-- << Startup mechanism >>
    //----------------------------

    static mxt_result InitializeCSipSpirallingSvc();

protected:
    // Summary:
    //  Destructor. Must call ReleaseService instead of directly access this
    //  destructor.
    //-----------------------------------------------------------------------
    virtual ~CSipSpirallingSvc();

//-- Hidden Data Members
//------------------------
private:

    // The stack side service (the one that sends the SIP packet to the core).
    //-------------------------------------------------------------------------
    CSipTransportSvc* m_pStackSideSvc;

    // The connection service.
    //-------------------------
    CSipConnectionSvc* m_pCxSvc;
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSPIRALLINGSVC_H

