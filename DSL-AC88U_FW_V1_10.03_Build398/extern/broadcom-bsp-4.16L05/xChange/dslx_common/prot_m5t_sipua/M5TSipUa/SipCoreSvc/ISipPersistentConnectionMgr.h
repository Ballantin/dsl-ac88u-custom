//==============================================================================
//==============================================================================
//
//   Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPPERSISTENTCONNECTIONMGR_H
#define MXG_ISIPPERSISTENTCONNECTIONMGR_H

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


#ifndef MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT to be able to use \
this class.
#endif

#if (defined(MXD_SIPSTACK_ENABLE_DEPRECATED_PERSISTENT_CONNECTION_SWITCHBACK) && \
     defined(MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT))
#error You can not define \
MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT and \
MXD_SIPSTACK_ENABLE_DEPRECATED_PERSISTENT_CONNECTION_SWITCHBACK at the same time.
#endif


//-- Data Members


//-- Parameters used by Value


//-- Interface Realized and/or Parent


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSocketAddr;
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipPersistentConnectionMgr
//========================================
//
// Description:
//   The ISipPersistentConnectionMgr interface is the interface through
//   which the CSipPersistentConnectionList reports events to the application
//   about persistent connections.
//
// Location:
//   SipCoreSvc/ISipPersistentConnectionMgr.h
//
//==============================================================================
class ISipPersistentConnectionMgr
{
//-- Friend Declarations

//-- Published Interface
public:

    //==========================================================================
    //==
    //==  EvConnectionEstablished
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a connection was established.
    //
    //  Parameters:
    //    opqConnectionHandle:
    //      The connection handle received in the call to Establish.
    //
    //    rLocalAddr:
    //      The effective local address used for the connection.
    //      If the local address was fully specified (address and port)
    //      in the call to Establish, rLocalAddr simply returns this
    //      same information. If however the port or address were
    //      left unspecified, rLocalAddr contains the local address
    //      and port that were actually used.
    //
    //    rPeerAddr:
    //      The remote peer address. When a FQDN is used this address is
    //      assigned by the stack following a DNS query.
    //
    //  Description:
    //      Reports the successful establishment of a persistent connection.
    //      This is called each time a persistent connection is either
    //      established for the first time, or re-established after the
    //      connection went down.
    //
    //==========================================================================
    virtual void EvConnectionEstablished(
                                IN mxt_opaque opqConnectionHandle,
                                IN const CSocketAddr& rLocalAddr,
                                IN const CSocketAddr& rPeerAddr) = 0;

    //==========================================================================
    //==
    //==  EvConnectionTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the termination of a persistent connection.
    //
    //  Parameters:
    //    opqConnectionHandle:
    //      The connection handle received in the call to Establish.
    //
    //  Description:
    //      Event reporting the successful termination of a persistent
    //      connection. This is reported after calling Terminate.
    //
    //==========================================================================
    virtual void EvConnectionTerminated(IN mxt_opaque opqConnectionHandle) = 0;

    //==========================================================================
    //==
    //==  EvErrorOnConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports a failure on a persistent connection.
    //
    //  Parameters:
    //    opqConnectionHandle:
    //      The connection handle received in the call to Establish.
    //
    //    resConnectionError:
    //      The error that occurred while trying to establish or
    //      re-establish the persistent connection.
    //
    //      No error when the connection has been terminated by the peer.
    //
    //  Description:
    //      Reports that the stack was not able to establish or re-establish a
    //      persistent connection.
    //
    //      This event is reported each time the stack tries to establish or
    //      re-establish a connection, thus it can be called multiple times for
    //      a single connection, until either the connection succeeds or
    //      Terminate is called for this connection.
    //
    //      This event is also reported when an error occurs on an established
    //      connection.
    //
    //==========================================================================
    virtual void EvErrorOnConnection(IN mxt_opaque opqConnectionHandle,
                                     IN mxt_result resConnectionError) = 0;

    //==========================================================================
    //==
    //==  EvSipErrorOnConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports a SIP failure on a persistent connection.
    //
    //  Parameters:
    //    opqConnectionHandle:
    //      The connection handle received in the call to Establish.
    //
    //    rSentPacket:
    //      The packet that failed to be sent.
    //
    //    resConnectionError:
    //      The error that occurred at the SIP level on the persistent 
    //      connection :
    //        resFE_SIPTRANSPORT_MTU_SIZE_ERROR:
    //          Indicates the packet could not be sent because its serialized
    //          size exceeds its maximum allowed size.
    //
    //        resFE_SIPTRANSACTION_SHUTDOWN :
    //          The shutdown process was initiated.
    //
    //        resFE_ABORT 
    //          Transport error (e.g. ICMP error, timeout, etc.)
    //
    //        Other failures
    //          See errors code that can be reported through EvCommandResult.
    //
    //  Description:
    //      Informs that the last packet has not been successfully sent. It can
    //      be because the connection does not exist, an ICMP error was received
    //      or a timeout occurred.
    //
    //  Note:
    //      Can be safely ignored.
    //
    //  See also:
    //      CSipPersistentConnectionList::Establish,
    //      CSipPersistentConnectionList::Disconnect
    //      ISipCoreUser::EvCommandResult
    //
    //==========================================================================
    virtual void EvSipErrorOnConnection(IN mxt_opaque opqConnectionHandle, 
                                        IN const CSipPacket& rSentPacket,
                                        IN mxt_result resConnectionError) = 0;

#ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
    //==========================================================================
    //==
    //==  EvStartKeepAliveResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the StartKeepAlive result.
    //
    //  Parameters:
    //      opqConnectionHandle:
    //       The connection handle.
    //
    //      res:
    //       StartKeepAlive result. It can be resS_OK, if the service was
    //       successfully started or resFE_FAIL is there was an error.
    //
    //  Description:
    //      Informs the manager about the StartKeepAlive() result.
    //
    //      This event is useful to determine if the Keep Alive mechanim has
    //      been successfully started for the specified connection or, if
    //      has failed, to know the error code.
    //
    //==========================================================================
    virtual void EvStartKeepAliveResult(IN mxt_opaque opqConnectionHandle,
                                        IN mxt_result res) = 0;

    //==========================================================================
    //==
    //==  EvSendResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of a sending operation on a flow.
    //
    //  Parameters:
    //      opqConnectionHandle:
    //       The persistent connection opaque identifier.
    //
    //      opq:
    //       The opaque application data passed to the persistent connection
    //       list when calling Send.
    //
    //      res:
    //       resS_OK: The sending was successful.
    //       resFE_FAIL: The sending has failed. The application should consider
    //       this as a flow failure.
    //
    //  Description:
    //      Notifies the application about the sending result after the
    //      application has requested some data to be sent on a flow.
    //
    //      The application should consider a flow to have failed when a failure
    //      result code is returned by this event. In such a case, the application
    //      should call EvFlowFailure on the persistent connection list.
    //
    //  See also:
    //      CSipPersistentConnectionList::Send
    //      CSipPersistentConnectionList::EvFlowFailure
    //
    //==========================================================================
    virtual void EvSendResult(IN mxt_opaque opqConnectionHandle,
                              IN mxt_opaque opq,
                              IN mxt_result res) = 0;
#endif // -- #ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT

//-- Hidden Methods
protected:

    //-- << Protected Constructors / Destructors >>

    // Summary:
    //  Default Constructor.
    ISipPersistentConnectionMgr(){}

    // Summary:
    //  Destructor.
    virtual ~ISipPersistentConnectionMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipPersistentConnectionMgr(
                                IN const ISipPersistentConnectionMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipPersistentConnectionMgr& operator=(
                                IN const ISipPersistentConnectionMgr& rSrc);


//-- Hidden Data Members
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPPERSISTENTCONNECTIONMGR_H

