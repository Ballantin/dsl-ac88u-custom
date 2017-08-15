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
#ifndef MXG_ISIPSERVERMONITORSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPSERVERMONITORSVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_ISIPSERVERMONITORMGR_H
#include "SipCoreSvc/ISipServerMonitorMgr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class ISipServerMonitor;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipServerMonitorSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipServerMonitorSvc
//==============================================================================
//<GROUP SipCoreSvc>
//
// Description:
//  This service is used in conjunction with the SIP Server Monitor component.
//  It is used to provide information regarding the status of a primary server.
//
//  More precisely, this service informs its user when it detects that the
//  primary server is down AND when this context was using the primary server.
//  Moreover, it also informs its user when the primary server is available
//  AND this context is using the backup server.
//
//  The information is provided to allow the user to transit context using the
//  primary server to the backup server when it is detected that the primary
//  server is down. Once it is detected that the primary server is again online,
//  the user can then transit contexts that were using the backup server back to
//  the primary server.
//
//  #Transition from primary to backup server#
//  There are two different ways that a context using the primary server can be
//  informed that it must transit the context to the backup server.
//
//  - EvPrimaryServerFailure is reported by a context when it failed to get a
//    service from the primary server. This is a client event.
//
//  - EvSwitchToServer(eSERVER_BACKUP) is reported by a context when another
//    context failed to get a service from the primary server. This is a
//    spontaneous event.
//
//  #Transition from backup to primary server#
//  There is only a unique way for the service to report that the context must
//  be transited from the backup to the primary server. This is reported using
//  EvSwitchToServer(eSERVER_PRIMARY). This event is a spontaneous event and is
//  reported when a context successfully receives a service from the primary
//  server.
//
//  #How to transition a context from one server to the other#
//  How to transition a context from one server to the other depends on what the
//  context was used for. Here are some general guidelines, which may need to be
//  updated according to the reason why the context is used.
//
//  - If a dialog was established on the context, then the user should
//    correctly terminate its dialog if the dialog was established with the
//    backup server. In any case, it must also Clear (ISipContext::Clear) and
//    release its context. After this, it should create and configure a new
//    context and re-establish the dialog with the specified server.
//
//  - If no dialog was established on the context, then the user can possibly
//    reuse this context and transit it to the specified server. This can be done
//    in different ways, which really depends on the type of context. An option
//    to transit the context would be to update its current target
//    (ISipUserAgentSvc::SetCurrentTarget) or to update its pre-loaded route
//    (ISipUserAgent::SetPreLoadedRoute).
//
// #Using this service:#
//
//   To use this service, the application must create an ISipServerMonitor
//   component (CLSID_CSipServerMonitor) and configure it with a primary and
//   backup server. Then every time the application attaches this service to
//   a context and wants the server status to be monitored, it MUST call the
//   SetServerMonitor method on this service to associate it with the server
//   monitor component.
//
//   This service should be attached after the ISipDigestClientAuthSvc service
//   to allow authentication to be attempted on reception of 401 and 407
//   responses. It should also be attached after the ISipRedirectionSvc to allow
//   trying the 3XX response provided contacts. Otherwise, the primary server
//   will be declared as unavailable when it most probably should not have
//   been.
//
//   The application can have more than one server monitor component active
//   simultaneously to monitor multiple primary servers. When multiple server
//   monitor components are present, they are STANDALONE and DO NOT communicate
//   with one another in any way.
//
//   A single server monitor component can monitor many SIP contexts at once, as
//   long as all monitored contexts use the same primary and backup servers.
//
//#The ISipServerMonitorSvc is an ECOM object#
//
//  The ISipServerMonitorSvc is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipServerMonitorSvc</B>
//
//  <B>Interface Id: IID_ISipServerMonitorSvc</B>
//
//  The ISipServerMonitorSvc is an aggregated object. It cannot work standalone
//  and MUST be attached to an ISipContext.
//
// Location:
//   SipCoreSvc/ISipServerMonitorSvc.h
//
// See also:
//   ISipServerMonitor, ISipServerMonitorMgr
//
//==============================================================================
class ISipServerMonitorSvc : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipServerMonitorSvc);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Assigns a manager for the server monitor service.
    //
    //  Parameters:
    //      pMgr:
    //       The Server monitor service manager. MUST NOT be NULL.
    //
    //  Returns:
    //      - resS_OK: Success.
    //      - Other return codes: Failure.
    //
    //  Description:
    //      Assigns a manager for the server monitor service.
    //
    //      Note that a manager MUST be associated with this service.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN ISipServerMonitorMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetServerMonitor
    //==
    //==========================================================================
    //
    //  Summary:
    //      Assigns a server monitor component to the service.
    //
    //  Parameters:
    //      pServerMonitor:
    //       The server monitor component that will monitor servers for this
    //       service. MUST NOT be NULL.
    //
    //  Returns:
    //      - resS_OK: Success.
    //      - Other return codes: Failure.
    //
    //  Description:
    //      Assigns a server monitor component to the service.
    //
    //      Note that a server monitor MUST be associated with this service.
    //
    //==========================================================================
    virtual mxt_result SetServerMonitor(IN ISipServerMonitor* pServerMonitor) = 0;

//M5T_INTERNAL_USE_BEGIN

    //==========================================================================
    //==
    //==  ReportEvSwitchToServer
    //==
    //==========================================================================
    //
    //  #Warning:#
    //      This method is part of the private interface between the SIP server
    //      monitoring component and the SIP server monitoring service. IT IS
    //      NOT meant to be called by the application.
    //
    //  Summary:
    //      Tells the service to report the EvSwitchToServer event to its
    //      manager.
    //
    //  Parameters:
    //      eServer:
    //       The target server for the switch.
    //
    //  Description:
    //      Tells the service to report the EvSwitchToServer event to its
    //      manager.
    //
    //==========================================================================
    virtual void ReportEvSwitchToServer(IN ISipServerMonitorMgr::EServerType eServer) = 0;

//-- Hidden Interface
protected:

    //-- << Constructors / Destructors / Operators >>
    ISipServerMonitorSvc() {}
    virtual ~ISipServerMonitorSvc() {}

private:

    //-- << Deactivated Constructors / Operators >>
    ISipServerMonitorSvc(IN const ISipServerMonitorSvc& rFrom);
    ISipServerMonitorSvc& operator=(IN const ISipServerMonitorSvc& rFrom);

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSERVERMONITORSVC_H
