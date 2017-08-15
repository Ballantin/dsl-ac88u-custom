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
#ifndef MXG_ISIPSERVERMONITOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPSERVERMONITOR_H
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

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CHostPort;
class CSipPacket;
class ISipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipServerMonitor);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipServerMonitor
//==============================================================================
//<GROUP SipCoreSvc>
//
// Description:
//   This interface defines the public API of the SIP server monitor component
//   of the SIP stack.
//
//   It is used to configure the server monitoring feature by providing the
//   primary and backup server addresses through the Configure method.
//
//   The following shows how to create the SIP server monitor component.
//   <CODE>
//   CHostPort primaryServer; // This holds the primary server address.
//   CHostPort backupServer;  // This holds the backup server address.
//
//   ISipServerMonitor* pServerMonitor = NULL;
//   CreateEComInstance(CLSID_CSipServerMonitor,
//                      NULL,
//                      OUT &pServerMonitor);
//   if (pServerMonitor != NULL)
//   {
//       pServerMonitor->Configure(primaryServer, backupServer);
//   }
//
//   // NOTE: Keep the pServerMonitor pointer until it is no longer needed.
//
//   // And release to free its resources.
//   pServerMonitor->ReleaseIfRef();
//   pServerMonitor = NULL;
//   </CODE>
//
//#The ISipServerMonitor is an ECOM object#
//
//  The ISipServerMonitor is an ECOM object that is created and accessed through
//  the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipServerMonitor</B>
//
//  <B>Interface Id: IID_ISipServerMonitor</B>
//
// Location:
//   SipCoreSvc/ISipServerMonitor.h
//
// See Also:
//   ISipServerMonitorSvc
//
//==============================================================================
class ISipServerMonitor : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipServerMonitor);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  Configure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the primary and backup server addresses used by the server
    //      monitoring component.
    //
    //  Parameters:
    //      rPrimaryServer:
    //       The host:port construct that describes the primary server's
    //       address. The host part can either be an IP address or a FQDN. Refer
    //       to the method's description for details.
    //
    //      rBackupServer:
    //       The host:port construct that describes the backup server's address.
    //       The host part can either be an IP address or a FQDN. Refer
    //       to the method's description for details.
    //
    //  Returns:
    //      - resS_OK: Success.
    //      - Other return codes: Failure.
    //
    //  Description:
    //      This method allows the application to set up the server monitoring
    //      feature by specifying the primary and backup server it will use.
    //
    //      Once configured, the server monitoring component will inform the
    //      application, through the SIP contexts that use the
    //      ISipServerMonitorSvc service, when it needs to switch between its
    //      primary and backup server.
    //
    //      The host part of rPrimaryServer and rBackupServer parameters can
    //      either be an IP address or a FQDN and must be set to the addresses
    //      used by the application. The server monitor feature will use these
    //      values as follows:
    //          - When an IP address is provided, it is used as is.
    //          - When a FQDN address is provided, it is NOT resolved to an IP
    //            address. It is used as is.
    //
    //      The server monitor will detect the usage of the primary and backup
    //      server only based on the address types that were configured through
    //      this method. As an example, if an application configures its primary
    //      server to be "primary.example.com" and sends a request to 
    //      "192.168.0.1" (assuming this is the resolved address of 
    //      "primary.example.com"), the server monitor will NOT detect that the
    //      request was sent to the primary server, as both addresses do not
    //      literally match.
    //
    //      To ensure that the server monitor can do its work correctly, the
    //      application must ensure that it uses the proper pre-loaded route or
    //      request-URI.
    //
    //==========================================================================
    virtual mxt_result Configure(IN const CHostPort& rPrimaryServer,
                                 IN const CHostPort& rBackupServer) = 0;

//M5T_INTERNAL_USE_BEGIN

    //-- << Private interface between service and component >>

    //==========================================================================
    //==
    //==  RegisterSipContext
    //==
    //==========================================================================
    //
    //  #Warning:#
    //      This method is part of the private interface between the SIP server
    //      monitoring component and the SIP server monitoring service. IT IS
    //      NOT meant to be called by the application unless it knows what it
    //      is doing.
    //
    //  Summary:
    //      Registers a SIP context with the server monitoring component.
    //
    //  Parameters:
    //      pSipContext:
    //       The context to register with the server monitoring component.
    //
    //  Returns:
    //      - resS_OK: Success.
    //      - Other return codes: Failure.
    //
    //  Description:
    //      This method allows a SIP context to get associated with the server
    //      monitoring component and be notified from that point onward when it
    //      should switch between the primary and backup server.
    //
    //  See also:
    //      ISipServerMonitorSvc
    //
    //==========================================================================
    virtual mxt_result RegisterSipContext(IN ISipContext* pSipContext) = 0;

    //==========================================================================
    //==
    //==  UnregisterSipContext
    //==
    //==========================================================================
    //
    //  #Warning:#
    //      This method is part of the private interface between the SIP server
    //      monitoring component and the SIP server monitoring service. IT IS
    //      NOT meant to be called by the application unless it knows what it
    //      is doing.
    //
    //  Summary:
    //      Unregisters a SIP context from the server monitoring component.
    //
    //  Parameters:
    //      pSipContext:
    //       The context to unregister from the server monitoring component.
    //
    //  Returns:
    //      - resS_OK: Success.
    //      - Other return codes: Failure.
    //
    //  Description:
    //      This method allows a SIP context to get disassociated from the
    //      server monitoring component. From that point onward, the context
    //      will no longer be notified if it should switch between the primary
    //      and backup server.
    //
    //  See also:
    //      ISipServerMonitorSvc
    //
    //==========================================================================
    virtual mxt_result UnregisterSipContext(IN ISipContext* pSipContext) = 0;

    //==========================================================================
    //==
    //==  OnResponseReceived
    //==
    //==========================================================================
    //
    //  #Warning:#
    //      This method is part of the private interface between the SIP server
    //      monitoring component and the SIP server monitoring service. IT IS
    //      NOT meant to be called by the application unless it knows what it
    //      is doing.
    //
    //  Summary:
    //      Notifies the server monitoring component of the reception of a new
    //      response.
    //
    //  Parameters:
    //      pSipContext:
    //       The context on which the response has been received.
    //
    //      rRequest:
    //       The request for which a response has been received.
    //
    //      rResponse:
    //       The received response.
    //
    //  Returns:
    //      - resFE_ABORT: Bad response (>= 300) received for the sent request
    //                     while communicating with primary server.
    //      - resS_OK: Good response (= 2XX) received for the sent request while
    //                 communicating with any server OR
    //                 bad response (>= 300) received for the sent request while
    //                 communicating with the backup server.
    //
    //  Description:
    //      This method is used by the server monitoring service to provide all
    //      response, it receives, to the server monitoring component for
    //      further analysis.
    //
    //      If the response is a failure and the context is communicating with
    //      the primary server, resFE_ABORT is returned. In all other cases,
    //      resS_OK is returned.
    //
    //      When resFE_ABORT is returned, an event will be fired on all contexts
    //      currently using the primary server (except the one provided as
    //      parameter) to request switching to the BACKUP server.
    //
    //      When resS_OK is returned and the context is communicating with the
    //      primary server, all contexts currently using the backup server will
    //      receive a notification to switch back to the primary server.
    //
    //      In all other cases, no contexts are notified.
    //
    //  See also:
    //      ISipServerMonitorSvc
    //
    //==========================================================================
    virtual mxt_result OnResponseReceived(IN ISipContext* pSipContext,
                                          IN const CSipPacket& rRequest,
                                          IN const CSipPacket& rResponse) = 0;

//-- Hidden Interface
protected:

    //-- << Constructors / Destructors / Operators >>
    ISipServerMonitor() {}
    virtual ~ISipServerMonitor() {}

private:

    //-- << Deactivated Constructors / Operators >>
    ISipServerMonitor(IN const ISipServerMonitor& rFrom);
    ISipServerMonitor& operator=(IN const ISipServerMonitor& rFrom);

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSERVERMONITOR_H
