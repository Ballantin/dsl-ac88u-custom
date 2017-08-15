//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPSERVERMONITORMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPSERVERMONITORMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSipPacket;
class ISipServerMonitorSvc;
class ISipClientEventControl;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipServerMonitorMgr
//==============================================================================
//<GROUP SipCoreSvc>
//
// Description:
//   This interface defines the events that are reported by the
//   SIP Server Monitor service.
//
// Location:
//   SipCoreSvc/ISipServerMonitorMgr.h
//
// See Also:
//   ISipServerMonitorSvc
//
//==============================================================================
class ISipServerMonitorMgr
{
//-- Published Interface
public:

    // Description:
    //  Indicates the different server types.
    enum EServerType
    {
        // Description:
        //  Primary server.
        eSERVER_PRIMARY,

        // Description:
        //  Backup server.
        eSERVER_BACKUP
    };

    //==========================================================================
    //==
    //==  EvSwitchToServer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the manager that the ISipContext associated with this
    //      service should be transferred to another server.
    //
    //  Parameters:
    //    pSvc:
    //      The service that is reporting the event.
    //
    //    eServer:
    //      The server to which the context should be transferred.
    //
    //  Description:
    //      Informs the manager that the ISipContext associated with this
    //      service should be transferred to another server.
    //
    //      "Transferred" here means that the context must either be terminated
    //      and re-created using the specified server, or if the context has no
    //      established dialogs, it can possibly be updated in order to start
    //      using the specified server.
    //
    //  Note:
    //      It is possible, under specific conditions, that this event is
    //      reported multiple times for a context before it had the time to
    //      switch to the specified server. This can happen when switching from
    //      the backup to the primary server, while the application terminates
    //      its "relationship" (dialog, registration, etc) with the backup
    //      server.
    //
    //      To minimize this possiblity, applications should minimize SIP
    //      exchanges with the backup server before switching to the primary
    //      server.
    //
    //  See Also:
    //      EvPrimaryServerFailure, ISipServerMonitorSvc
    //
    //==========================================================================
    virtual void EvSwitchToServer(IN ISipServerMonitorSvc* pSvc,
                                  IN EServerType eServer) = 0;

    //==========================================================================
    //==
    //==  EvPrimaryServerFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the manager that communications with the primary server
    //      have been interrupted.
    //
    //  Parameters:
    //    pSvc:
    //      The server monitoring service for which the event is reported.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rResponse:
    //      The response that reported the communication failure.
    //
    //  Description:
    //      Informs the manager that communications with the primary server
    //      have been interrupted.
    //
    //      This event indicates that the last client operation failed to obtain
    //      a service from the primary server. The application MAY call
    //      ISipClientEventCtrl::CallNextClientEvent to continue processing the
    //      client events on the context. 
    //
    //      If the application DOES NOT call 
    //      ISipClientEventCtrl::CallNextClientEvent it MUST do one of the
    //      following when handling this event:
    //
    //      - If the context is used to manage an established dialog, e.g. the
    //        event is reported for an in-dialog client request, the application
    //        MUST clear and release the context and create a new one that will
    //        use the backup server.
    //
    //      - If the context is used to manage out-of-dialog client requests,
    //        including original INVITE requests, the application can simply
    //        update the context's target server and retry.
    //
    //  See Also:
    //      EvSwitchToServer, ISipServerMonitorSvc
    //
    //==========================================================================
    virtual void EvPrimaryServerFailure(IN ISipServerMonitorSvc* pSvc,
                                        IN ISipClientEventControl* pClientEventCtrl,
                                        IN const CSipPacket& rResponse) = 0;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Interface
protected:

    //-- << Constructors / Destructors / Operators >>
    ISipServerMonitorMgr() {}
    virtual ~ISipServerMonitorMgr() {}

private:

    //-- << Deactivated Constructors / Operators >>
    ISipServerMonitorMgr(IN const ISipServerMonitorMgr& rFrom);
    ISipServerMonitorMgr& operator=(IN const ISipServerMonitorMgr& rFrom);

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSERVERMONITORMGR_H
