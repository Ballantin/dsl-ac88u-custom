//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation
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
//==EDOC========================================================================
#ifndef MXG_ICLIENTSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ICLIENTSOCKET_H
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


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IIOSOCKET_H
#include "Network/IIoSocket.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IClientSocket
//========================================
//<GROUP NETWORK_CLASSES>
//
// Summary:
//      Interface defining the methods associated with a client socket.
//
// Description:
//      The IClientSocket interface defines the methods associated with a client
//      socket. A client socket can be used to send and receive data, as defined
//      in the IIoSocket interface, and it can also connect to a remote address.
//
//      This interface only defines the Bind and Connect methods. Bind is used
//      to bind the socket to a local address.Connect is used to connect
//      a socket to a remote address. Note that some types of sockets must be
//      connected in order to send and receive data (sockets of type eSTREAM and
//      eSEQPACKET) while others can be used unconnected (sockets of type
//      eDATAGRAM).
//
// Location:
//   Network/IClientSocket.h
//
// See Also:
//   ISocket, IServerSocket, IIoSocket
//
//==EDOC========================================================================
class IClientSocket : public IIoSocket
{
//-- Published Interface
//------------------------
public:
    //==SDOC====================================================================
    //==
    //==  Bind
    //==
    //==========================================================================
    //
    //  Summary:
    //      Binds the socket to a local address.
    //
    //  Parameters:
    //    pLocalAddress:
    //      Contains the local address where to bind.  May be NULL if the user
    //      does not care about the interface or the the local port. If the
    //      parameter is not NULL and its port is set to 0, the automatic port
    //      allocation feature will also be used.
    //
    //    pEffectiveLocalAddress:
    //      On output, contains the effective local address where the socket is
    //      bound.  May be NULL if the user does not care.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      Used to associated a local address to the socket.  The user MUST
    //      call this method following a call to Create.
    //
    //==EDOC====================================================================
    virtual mxt_result Bind(IN const CSocketAddr* pLocalAddress,
                            OUT CSocketAddr* pEffectiveLocalAddress) = 0;


    //==SDOC====================================================================
    //==
    //==  Connect
    //==
    //==========================================================================
    //
    //  Summary:
    //      Connects the socket to a remote address.
    //
    //  Parameters:
    //    pPeerAddress:
    //      Remote address where the socket is to be connected.
    //
    //  Returns:
    ///      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is used to connect the socket to a remote address. Once
    //      a socket is connected, Send and Recv can be used to exchange
    //      data with the remote peer.
    //
    //==EDOC====================================================================
    virtual mxt_result Connect(IN const CSocketAddr* pPeerAddress) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //==SDOC====================================================================
    //==
    //==  IClientSocket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Default Constructor.
    //
    //==EDOC====================================================================
    IClientSocket(){};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~IClientSocket(){};
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_ICLIENTSOCKET_H
