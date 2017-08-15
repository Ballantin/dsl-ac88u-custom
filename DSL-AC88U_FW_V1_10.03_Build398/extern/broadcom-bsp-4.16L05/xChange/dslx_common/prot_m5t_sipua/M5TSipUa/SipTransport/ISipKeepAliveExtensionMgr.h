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
#ifndef MXG_ISIPKEEPALIVEEXTENSIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPKEEPALIVEEXTENSIONMGR_H
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
//== Class: ISipKeepAliveExtensionMgr
//==============================================================================
// <FLAG New in 4.1.6>
//<GROUP SipTransport>
//
// Summary:
//  Informs the application about flow events related to the reception of data.
//
// Description:
//  This manager allows an application to see packets received from the network
//  that are not meant for the SIP stack. Such example could be STUN responses
//  received over a connected UDP socket created by the SIP stack. The
//  application should be prepared to receive such STUN responses if it sends
//  STUN requests through the CSipPersistentConnectionList::Send interface.
//
//  The event reported on this interface is invoked from the context of the
//  SipTransport thread.
//
// Location:
//   SipTransport/ISipKeepAliveExtensionMgr.h
//
// See Also:
//   ISipOutboundConnectionSvc, ISipTransportMgr::SendA
//
//==============================================================================
class ISipKeepAliveExtensionMgr
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
    //==  EvDataReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that non SIP data has been received.
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
    //      puData:
    //       A pointer to the received data.
    //
    //      uSize:
    //       The size of the received data.
    //
    //  Description:
    //      Informs the application that data non related to a SIP packet has
    //      been received. For the time being, only STUN responses (UDP
    //      datagrams) are reported through this event. STUN responses are
    //      likely to happen only if the application has previously sent STUN
    //      binding requests.
    //
    //      All UDP datagrams beginning with either 0 or 1 as the first byte is
    //      reported through this event. If, for some reason, the server sends
    //      data which is not STUN compliant and this data begins with 0 or 1,
    //      then this event is reported. It is recommended that the application
    //      verifies the validity of the STUN response before processing it.
    //
    //==========================================================================
    virtual void EvDataReceived(IN ESipTransport eTransport,
                                IN const CSocketAddr& rLocalAddr,
                                IN const CSocketAddr& rPeerAddr,
                                IN uint8_t* puData,
                                IN unsigned int uSize) = 0;

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipKeepAliveExtensionMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISipKeepAliveExtensionMgr() {}

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipKeepAliveExtensionMgr(IN const ISipKeepAliveExtensionMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipKeepAliveExtensionMgr& operator=(IN const ISipKeepAliveExtensionMgr& rSrc);


//-- Hidden Data Members
protected:
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPKEEPALIVEEXTENSIONMGR_H

