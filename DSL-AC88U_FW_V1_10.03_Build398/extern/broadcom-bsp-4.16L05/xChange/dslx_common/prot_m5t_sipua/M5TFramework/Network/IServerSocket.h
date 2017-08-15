//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_ISERVERSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISERVERSOCKET_H
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
#ifndef MXG_ISOCKET_H
#include "Network/ISocket.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class IIoSocket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IServerSocket
//========================================
//<GROUP NETWORK_CLASSES>
//
// Summary:
//      Interface defining the methods needed for a server socket.
//
// Description:
//      The IServerSocket interface defines the methods needed for a server type
//      of socket. Note that this interface is available only to
//      connection-oriented sockets types.
//
// Location:
//   Network/IServerSocket.h
//
// See Also:
//   IClientSocket, IIoSocket, ISocket
//
//==EDOC========================================================================
class IServerSocket : public ISocket
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //==SDOC====================================================================
    //==
    //==  Accept
    //==
    //==========================================================================
    //
    //  Summary:
    //      Accepts an incoming connection attempt.
    //
    //  Parameters:
    //    ppIoSocket:
    //      OUT parameter used to receive the pointer to a IIoSocket
    //      representing the accepted connection. Note that the caller will
    //      receive the ownership of this pointer. This means that it will be
    //      the responsibility of the caller to delete this pointer by calling
    //      Release on it (unless the ownership of this pointer is given to some
    //      other entity).
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      Accepts an incoming connection attempt on a listening
    //      connection-oriented socket.
    //
    //      Please take note of the ownership note given in the parameter
    //      description.
    //
    //  See Also:
    //      Listen
    //
    //
    //==EDOC====================================================================
    virtual mxt_result Accept(OUT GO IIoSocket** ppIoSocket) = 0;


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
    //      Contains the local address where to bind.  May be NULL is the user
    //      does not care about the interface or the the local port.
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
    //==  Listen
    //==
    //==========================================================================
    //
    //  Summary:
    //      Listens on the socket for incoming connection attempts.
    //
    //  Parameters:
    //    uMaxPendingConnection:
    //      Maximum number of pending connections accepted.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      Sets the socket to listen for incoming connection attempts. Once a
    //      connection attempt is detected, Accept must be called in order to
    //      fully establish the connection.
    //
    //  See Also:
    //      Accept
    //
    //==EDOC====================================================================
    virtual mxt_result Listen(IN unsigned int uMaxPendingConnection = 5) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //==SDOC====================================================================
    //==
    //==  IServerSocket
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
    IServerSocket(){};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~IServerSocket(){};
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_ISERVERSOCKET_H
