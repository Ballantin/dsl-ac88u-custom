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
#ifndef MXG_ISIPKEEPALIVEMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPKEEPALIVEMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#ifndef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT to be able to use \
this class.
#endif

#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" // ESipTransport
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSocketAddr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipKeepAliveMgr
//==============================================================================
//<GROUP SipTransport>
//
// Summary:
//  Informs the application about flow events.
//
// Description:
//  This manager reports events related to each flow created through the
//  persistent connections list. It reports events telling that the flow has
//  succeeded or that a failure has occurred on the flow. The manager of this
//  event can use these events to determine the timers to use before
//  reconnecting.
//
//  The events reported on this interface are invoked from the context of the
//  SipTransport thread.
//
//  This interface is internal to the SIP stack and should not be used by the
//  application.
//
// Location:
//   SipTransport/ISipKeepAliveMgr.h
//
//==============================================================================
class ISipKeepAliveMgr
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  EvFlowSucceeded
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells that the first keep alive has succeeded.
    //
    //  Parameters:
    //      eTransport:
    //       The connection's transport ID.
    //
    //      rLocalAddr:
    //       The connection's local IP address and port.
    //
    //      rPeerAddr:
    //       The connection's peer IP address and port.
    //
    //  Description:
    //      Informs the application that the first keep alive sent to the server
    //      has been successful (a response has been received from the server).
    //      The "ping" request sent has been responded in time by a "pong". For
    //      reliable transports, the "ping" is a double CRLF and the "pong" is a
    //      single CRLF.
    //
    //      This event is useful to determine the value to configure the flow
    //      recovery timer. This is to be compliant to the section 4.5 "Flow
    //      Recovery" in the second paragraph:
    //
    //      "..a flow is considered successful if outbound registration
    //       succeeded, and if keep alives are in use on this flow, at least one
    //       subsequent keep alive response was received."
    //
    //==========================================================================
    virtual void EvFlowSucceeded(IN ESipTransport eTransport,
                                 IN const CSocketAddr& rLocalAddr,
                                 IN const CSocketAddr& rPeerAddr) = 0;

    //==========================================================================
    //==
    //==  EvFlowFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports a flow failure
    //
    //  Parameters:
    //      eTransport:
    //       The connection's transport ID.
    //
    //      rLocalAddr:
    //       The connection's local IP address and port.
    //
    //      rPeerAddr:
    //       The connection's peer IP address and port.
    //
    //  Description:
    //      Notifies that the flow has failed to receive a keep alive response
    //      in time. The manager should close the flow upon receiving this
    //      event.
    //
    //==========================================================================
    virtual void EvFlowFailure(IN ESipTransport eTransport,
                               IN const CSocketAddr& rLocalAddr,
                               IN const CSocketAddr& rPeerAddr) = 0;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipKeepAliveMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISipKeepAliveMgr() {}
private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipKeepAliveMgr(IN const ISipKeepAliveMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipKeepAliveMgr& operator=(IN const ISipKeepAliveMgr& rSrc);


//-- Hidden Data Members
protected:
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPKEEPALIVEMGR_H

