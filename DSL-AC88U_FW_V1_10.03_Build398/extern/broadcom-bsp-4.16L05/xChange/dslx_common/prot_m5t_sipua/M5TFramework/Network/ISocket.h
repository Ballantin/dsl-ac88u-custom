//==============================================================================
//==============================================================================
//
//     Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_ISOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISOCKET_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSocketAddr;


//==============================================================================
//== Class: ISocket
//========================================
//<GROUP NETWORK_CLASSES>
//
// Summary:
//   Interface offering common functionalities to all types of sockets.
//
// Description:
//   The ISocket interface is the base interface shared by all socket classes.
//   This interface offers the common functionality associated to all types of
//   sockets.
//
// Location:
//   Network/ISocket.h
//
// See Also:
//   IClientSocket, IServerSocket, IIoSocket
//
//==============================================================================
class ISocket
{
// Enum Types Definition
//-----------------------
public:

    //<GROUP NETWORK_ENUMS>
    // Summary:
    //     Possible behaviours when closing.
    //
    // Description:
    //     Possible behaviours when closing.
    //-----------------------------------
    enum ECloseBehavior
    {
        // Description:
        // The close operation returns immediately and the socket
        // will be closed gracefully in the background. After
        // the close operation no more receives and sends can be
        // issued on the socket. The data in the send buffer is
        // sent and followed by FIN.
        eGRACEFUL,

        // Description:
        // The socket will be forcefully closed. This means that all the queued
        // data that has not yet been sent will be discarded.  A RST is sent
        // to the peer and will cause a resFE_MITOSFW_NET_SOCK_CONNECTIONLOST
        // on the peer.
        eFORCE
    };

    //<GROUP NETWORK_ENUMS>
    // Summary:
    //     Possible protocol families.
    //
    // Description:
    //     Possible protocol families.
    //-----------------------------
    enum EProtocolFamily
    {
        // Description:
        // This defines the IPv4 protocol familly.
        eINTERNET_V4,
        // Description:
        // This defines the IPv6 protocol familly.
        eINTERNET_V6,
        // Description:
        // Unkown protocol family.
        eUNKNOWN
    };


    //<GROUP NETWORK_ENUMS>
    // Summary:
    //     This defines the type of transport associated with a socket.
    //
    // Description:
    //     This defines the type of transport associated with a socket.
    //------------------------------------------------------------
    enum ESocketType
    {
        // Description:
        // This defines a two-way, sequenced, reliable,
        // connection-based byte-stream. An example of this is TCP.
        eSTREAM,

        // Description:
        // This defines a connectionless, unreliable
        // transport of messages of a fixed maximum length. An
        // example of this is UDP.
        eDATAGRAM,

        // Description:
        // This defines a two-way, sequenced, reliable,
        // connection-based transmission of datagrams. An example
        // of this is SCTP.
        eSEQPACKET
    };


//-- Published Interface
//------------------------
public:

    //==========================================================================
    //==
    //==  Close
    //==
    //==========================================================================
    //
    //  Summary:
    //      Closes a socket.
    //
    //  Parameters:
    //      eBehavior:
    //          How the socket is to be closed, as defined by the
    //          ECloseBehavior enumeration.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is used to close a socket. This puts the socket into the
    //      state it was just after its allocation. After this call, all system
    //      resources associated to the socket must have been released. Note
    //      that a socket can be reused after it has been closed. In such a
    //      case, the socket options are all back to their default values. In
    //      other words, calling Close() on a socket has the effect of resetting
    //      the socket options to their default values after it is closed.
    //
    //==========================================================================
    virtual mxt_result Close(IN ECloseBehavior eBehavior) = 0;


    //==========================================================================
    //==
    //==  GetLocalAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the local address to which the socket is bound.
    //
    //  Parameters:
    //    pLocalAddress:
    //      OUT parameter that will contain the address and port information.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - resS_OK
    //
    //  Description:
    //      This method is used to retrieve the address and port to which the
    //      socket has been bound. It will return an error until the socket is
    //      bound to an interface with the Bind function.
    //
    //==========================================================================
    virtual mxt_result GetLocalAddress(OUT CSocketAddr* pLocalAddress) const = 0;


    //==========================================================================
    //==
    //==  GetProtocolFamily
    //==
    //==========================================================================
    // <FLAG Deprecated since v2.1.4>
    //
    //  Summary:
    //      Retrieves the protocol family of this socket.
    //
    //  Parameters:
    //    peProtocolFamily:
    //      OUT parameter that receives the protocol family, as defined in the
    //      EProtocolFamily enumeration.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - resS_OK
    //
    //  Description:
    //      This method is used to retrieve the protocol family associated to
    //      the socket. This method is deprecated and GetAddressFamily should
    //      be used instead.
    //
    //==========================================================================
    virtual mxt_result GetProtocolFamily(OUT EProtocolFamily* peProtocolFamily) const = 0;

    //==========================================================================
    //==
    //==  GetAddressFamily
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the address family of this socket.
    //
    //  Parameters:
    //    peAddressFamily:
    //      OUT parameter that receives the address family, as defined in the
    //      CSocketAddr::EAddressFamily enumeration.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - resS_OK
    //
    //  Description:
    //      This method is used to retrieve the address family associated to
    //      the socket.
    //
    //==========================================================================
    virtual mxt_result GetAddressFamily(OUT CSocketAddr::EAddressFamily* peAddressFamily) const = 0;



    //==========================================================================
    //==
    //==  GetSocketType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves information about the socket and its transport.
    //
    //  Parameters:
    //    peSocketType:
    //      OUT parameter that receives the socket type information, as defined
    //      in the ESocketType enumeration.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - resS_OK
    //
    //  Description:
    //      This method is used to retrieve information about this socket and
    //      its transport.
    //
    //==========================================================================
    virtual mxt_result GetSocketType(OUT ESocketType* peSocketType) const = 0;


    //==========================================================================
    //==
    //==  Release
    //==
    //==========================================================================
    //
    //  Summary:
    //      Deletes this socket.
    //
    //  Returns:
    //      Always zero.
    //
    //  Description:
    //      This method is used to delete a socket. This has become necessary in
    //      order to be able to delete the socket from one of its interfaces
    //      (instead of directly calling the class that implements these
    //      interfaces). This is called by the user of the socket instead of the
    //      delete operator.
    //
    //      Note that this can be called if the socket is not closed yet. In
    //      such a case, the socket will be closed automatically before the
    //      socket is deleted.
    //
    //      The return value is always zero. We eventually plan to implement
    //      reference counting on the sockets, and the reference count number
    //      will be returned instead of zero when calling Release.
    //
    //==========================================================================
    virtual uint32_t Release() = 0;


    //==========================================================================
    //==
    //==  Set8021QUserPriority
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the 802.1Q user priority for this socket.
    //
    //  Parameters:
    //    bEnable:
    //      Enables 802.1Q on this socket.
    //
    //    uUserPriority:
    //      The user priority, from 0 to 7.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method sets the 802.1Q user priority for this socket. Note that
    //      this is not supported on all operating systems. This works correctly
    //      with the internal linux distribution of Mediatrix and with the
    //      modified Windriver IP stack.
    //
    //      This method is not used by MxSF.
    //
    //==========================================================================
    virtual mxt_result Set8021QUserPriority( IN bool bEnable,
                                             IN uint8_t uUserPriority) = 0;


    //==========================================================================
    //==
    //==  SetTos
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the type of service (TOS) associated with this socket.
    //
    //  Parameters:
    //    uTos:
    //      TOS byte for this socket.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      Configures the socket to include a specific TOS byte on the data it
    //      sends.
    //
    //      This method is not used by MxSF.
    //
    //==========================================================================
    virtual mxt_result SetTos(IN uint8_t uTos) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  ISocket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Default Constructor.
    //
    //==========================================================================
    ISocket(){};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISocket(){};

private:
    // Deactivated Constructors / Destructors / Operators
    ISocket(const ISocket& from);
    ISocket& operator=(const ISocket& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_ISOCKET_H
