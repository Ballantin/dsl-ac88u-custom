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
#ifndef MXG_ISIPCONNECTIONBLACKLISTMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPCONNECTIONBLACKLISTMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
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
//== Class: ISipConnectionBlacklistMgr
//==============================================================================
// <GROUP SIPCORESVC_CLASSES>
//
// Summary:
//   The connection blacklist manager that informs the application about the
//   insertion or removal of IP addresses from the blacklist.
//
// Description:
//   The connection blacklist manager informs the application when an IP
//   address is added to the blacklist or removed from it. This manager is
//   optional for the application. It can be configured using the SetManager
//   method of CSipConnectionBlacklist.
//
// Location:
//   SipCoreSvc/ISipConnectionBlacklistMgr.h
//
// See Also:
//   ISipCoreConfig, CSipConnectionBlacklist
//
//==============================================================================
class ISipConnectionBlacklistMgr
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
    //==  EvAddressBlacklisted
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs that a given address has been inserted into the blacklist.
    //
    //  Parameters:
    //      rPeerAddr:
    //       The address of the remote peer that was blacklisted.
    //
    //      eTransport:
    //       The transport protocol for the blacklisted address.
    //
    //  Description:
    //      This event is invoked when a new address for a given transport
    //      protocol is inserted into the blacklist.  It can be used by
    //      the application to update its subscriptions for example.
    //
    //  Note:
    //      The rPeerAddr parameter is only valid for the duration of this
    //      method.
    //
    //==========================================================================
    virtual void EvAddressBlacklisted(IN const CSocketAddr& rPeerAddr,
                                      IN ESipTransport eTransport) = 0;

    //==========================================================================
    //==
    //==  EvBlacklistDurationCompleted
    //==
    //==========================================================================
    //
    //  Summary:
    //      Invoked when the address has completed its penalty in the blacklist.
    //
    //  Parameters:
    //      rPeerAddr:
    //       The address of the remote peer that was blacklisted.
    //
    //      eTransport:
    //       The transport protocol for the blacklisted address.
    //
    //  Description:
    //      This event is invoked when the address has completed its penalty
    //      time in the blacklist. This event can occur either when the address
    //      was removed from the blacklist or when the duration of the penalty
    //      timer has elapsed.
    //
    //      Such an event can be useful to the application to determine when a
    //      blacklisted IP address has become available. That means the SIP
    //      stack will try to reuse this address when a packet needs to be sent
    //      to the host that maps with this IP.
    //
    //  Note:
    //      The rPeerAddr parameter is only valid for the duration of this
    //      method.
    //
    //==========================================================================
    virtual void EvBlacklistDurationCompleted(IN const CSocketAddr& rPeerAddr,
                                              IN ESipTransport eTransport) = 0;


//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipConnectionBlacklistMgr(){}

    // Summary:
    //  Destructor.
    virtual ~ISipConnectionBlacklistMgr(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipConnectionBlacklistMgr(IN const ISipConnectionBlacklistMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipConnectionBlacklistMgr& operator=(IN const ISipConnectionBlacklistMgr& rSrc);


//-- Hidden Data Members
protected:
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCONNECTIONBLACKLISTMGR_H
