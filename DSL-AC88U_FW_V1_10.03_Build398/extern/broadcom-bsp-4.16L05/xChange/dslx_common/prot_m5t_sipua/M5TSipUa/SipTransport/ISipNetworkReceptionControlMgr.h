//==============================================================================
//==============================================================================
//
//          Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPNETWORKRECEPTIONCONTROLMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPNETWORKRECEPTIONCONTROLMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
#error You MUST define MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT in your PreSipStackCfg.h \
    in order to use this object.
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSocketAddr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipNetworkReceptionControlMgr
//==============================================================================
// <FLAG New in 4.1.8>
// <GROUP SIPTRANSPORT_CLASSES>
//
// Summary:
//  Informs the application about events related to the reception of network
//  data.
//
// Description:
//  This manager allows an application to be notified about different network
//  data reception related events.
//
//  Currently, the only event reported is when a peer is being blocked by the
//  Peer Flood Blocking feature.
//
//  The event reported on this interface is invoked from the context of the
//  SipTransport thread.
//
// Location:
//   SipTransport/ISipNetworkReceptionControlMgr.h
//
//==============================================================================
class ISipNetworkReceptionControlMgr
{
//-- Friend Declarations.

//-- New types within class namespace.
public:
protected:
private:

//-- Published Interface
public:
    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  EvPeerBlocked
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a peer has been blocked.
    //
    //  Parameters:
    //      rPeerAddr:
    //       The connection's peer IP address and port.
    //
    //  Description:
    //      Informs the application that a peer has been blocked as a result of
    //      it sending too many data packets. This event is for information
    //      ONLY and the application MUST NOT do any kind of lengthy processing
    //      within it or it would compromise the SIP stack's ability to handle
    //      received packets.
    //
    //      If this event is being received too many times for too many peers,
    //      the peer blocking feature may not properly be configured.
    //
    //      The event is invoked from the context of the SipTransport thread.
    //
    //==========================================================================
    virtual void EvPeerBlocked(IN const CSocketAddr& rPeerAddr) = 0;

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipNetworkReceptionControlMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISipNetworkReceptionControlMgr() {}

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods.
protected:
private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipNetworkReceptionControlMgr(IN const ISipNetworkReceptionControlMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipNetworkReceptionControlMgr& operator=(IN const ISipNetworkReceptionControlMgr& rSrc);


//-- Hidden Data Members.
protected:
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPNETWORKRECEPTIONCONTROLMGR_H

