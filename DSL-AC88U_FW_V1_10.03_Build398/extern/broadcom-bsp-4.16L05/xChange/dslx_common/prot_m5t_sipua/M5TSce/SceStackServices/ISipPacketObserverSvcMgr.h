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
#ifndef MXG_ISIPPACKETOBSERVERSVCMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPPACKETOBSERVERSVCMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class ISipClientEventControl;
class ISipServerEventControl;
class ISipPacketObserverSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipPacketObserverSvcMgr
//========================================
//<GROUP TID_PACKAGE_SCESTACKSERVICES_CLASSES>
//
// Description:
//  This interface defines callbacks used by the ISipPacketObserverSvc to report
//  its events.
//
// Location:
//   SceStackServices/ISipPacketObserverSvcMgr.h
//
// See Also:
//   ISipPacketObserverSvc
//
//==============================================================================
class ISipPacketObserverSvcMgr
{
//-- Friend Declarations.

//-- Published Interface.
public:

    //==========================================================================
    //==
    //==  EvRequestReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a request has been received.
    //
    //  Parameters:
    //      pSvc:
    //       Pointer to the service that reports the event.
    //
    //      pServerEventCtrl:
    //       The server event control associated with the request.
    //
    //      rPacketIn:
    //       Reference to the received request. This packet has not been handled
    //       by the stack yet and must not be modified.
    //
    //  Description:
    //      Reports that a request has been received. The packet has not yet
    //      been handled by the stack, thus the manager must not modify it.
    //
    //==========================================================================
    virtual void EvRequestReceived(IN ISipPacketObserverSvc* pSvc,
                                   IN ISipServerEventControl* pServerEventCtrl,
                                   IN const CSipPacket& rPacketIn) = 0;

    //==========================================================================
    //==
    //==  EvResponseReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a response has been received.
    //
    //  Parameters:
    //      pSvc:
    //       Pointer to the service that reports the event.
    //
    //      pClientEventCtrl:
    //       The client event control associated with the request.
    //
    //      rPacketIn:
    //       Reference to the received response. This packet has not been handled
    //       by the stack yet and must not be modified.
    //
    //  Description:
    //      Reports that a response has been received. The packet has not yet
    //      been handled by the stack, thus the manager must not modify it.
    //
    //==========================================================================
    virtual void EvResponseReceived(IN ISipPacketObserverSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CSipPacket& rPacketIn) = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipPacketObserverSvcMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISipPacketObserverSvcMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipPacketObserverSvcMgr(IN const ISipPacketObserverSvcMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipPacketObserverSvcMgr& operator=(IN const ISipPacketObserverSvcMgr& rSrc);

//-- Hidden Data Members.
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_ISIPPACKETOBSERVERSVCMGR_H
