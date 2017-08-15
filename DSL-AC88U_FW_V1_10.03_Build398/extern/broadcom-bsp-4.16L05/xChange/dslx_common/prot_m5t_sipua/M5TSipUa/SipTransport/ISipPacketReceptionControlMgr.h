//==============================================================================
//==============================================================================
//
//          Copyright(c) 2011 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPPACKETRECEPTIONCONTROLMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPPACKETRECEPTIONCONTROLMGR_H
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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
#error You MUST define MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT in your PreSipStackCfg.h \
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
//== Class: ISipPacketReceptionControlMgr
//==============================================================================
// <FLAG New in 4.1.8>
// <GROUP SIPTRANSPORT_CLASSES>
//
// Summary:
//  Informs the application about events related to the reception of SIP packet
//  data.
//
// Description:
//  This manager allows an application to be notified about different SIP packet
//  data reception related events.
//
//  Currently, the only event reported is when an unapproved peer packet has
//  been received and discarded.
//
//  The event reported on this interface is invoked from the context of the
//  SipTransport thread.
//
// Location:
//   SipTransport/ISipPacketReceptionControlMgr.h
//
//==============================================================================
class ISipPacketReceptionControlMgr
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
    //==  EvUnapprovedPeerPacketReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that an unapproved peer packet has been received and
    //      discarded.
    //
    //  Parameters:
    //      rPeerAddr:
    //       The peer IP address and port.
    //
    //      rstrMethod:
    //       The method of the SIP packet received.
    //
    //  Description:
    //      Informs the application that an unapproved peer packet has been
    //      received and discarded by the SIP stack. This event is for
    //      information ONLY and the application MUST NOT do any kind of
    //      lengthy processing within it or it would compromise the SIP stack's
    //      ability to handle received packets.
    //
    //      This event can help the application to dynamically change the list
    //      of allowed request and/or approved peers.
    //
    //      The event is invoked from the context of the SipTransport thread.
    //
    //==========================================================================
    virtual void EvUnapprovedPeerPacketReceived(IN const CSocketAddr& rPeerAddr, IN const CString& rstrMethod) = 0;

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipPacketReceptionControlMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISipPacketReceptionControlMgr() {}

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods.
protected:
private:


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipPacketReceptionControlMgr(IN const ISipPacketReceptionControlMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipPacketReceptionControlMgr& operator=(IN const ISipPacketReceptionControlMgr& rSrc);


//-- Hidden Data Members.
protected:
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPPACKETRECEPTIONCONTROLMGR_H

