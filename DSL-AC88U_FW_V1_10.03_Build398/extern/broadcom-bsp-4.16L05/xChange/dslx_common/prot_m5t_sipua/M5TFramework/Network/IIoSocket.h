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
#ifndef MXG_IIOSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IIOSOCKET_H
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
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IIoSocket
//========================================
//<GROUP NETWORK_CLASSES>
//
// Summary:
//   Interface defining the methods used by a socket to send and receive data.
//
// Description:
//      The IIoSocket interface defines the methods accessible on a socket that
//      can be used to send and receive data.
//
// Location:
//   Network/IIoSocket.h
//
// See Also:
//   IClientSocket, IServerSocket, ISocket
//
//==EDOC========================================================================
class IIoSocket : public ISocket
{
//-- Published Interface
//------------------------
public:
    //==SDOC====================================================================
    //==
    //==  GetPeerAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the remote address to which the socket is connected.
    //
    //  Parameters:
    //    pPeerAddress:
    //      OUT parameter used to receive the address.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - resS_OK
    //
    //  Description:
    //      This method is used to retrieve the address to which the socket is
    //      currently connected. It can return an error for unconnected sockets.
    //
    //==EDOC====================================================================
    virtual mxt_result GetPeerAddress(OUT CSocketAddr* pPeerAddress) const = 0;

    //==SDOC====================================================================
    //==
    //==  Recv
    //==
    //==========================================================================
    //
    //  Summary:
    //      Receives data from a connected socket.
    //
    //  Parameters:
    //    pData:
    //      Blob object that will be filled with the data received on the
    //      socket.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is used to receive data from a connected socket.
    //
    //      The maximum number of bytes that can be received is defined by the
    //      blob's capacity. The user of the socket might want to resize the
    //      blob to a specific size before calling Recv, as its capacity will
    //      not be changed by this call.
    //
    //      When the received data is too big to fit in the Blob, no error will
    //      be returned.  In UDP, the remaining data is discarded. In TCP, the
    //      remaining data is available by making another call to Recv.
    //
    //  See Also:
    //      - IIoSocket::Recv
    //      - IIoSocket::RecvFrom
    //
    //==EDOC====================================================================
    virtual mxt_result Recv(OUT CBlob* pData) = 0;

    //==SDOC====================================================================
    //==
    //==  Recv
    //==
    //==========================================================================
    //
    //  Summary:
    //      Receives data from a connected socket.
    //
    //  Parameters:
    //    puData:
    //      Pointer to a buffer where the received data is to be stored.
    //
    //    uCapacity:
    //      The maximum size available in the buffer pointed to by puData.
    //
    //    puSize:
    //      OUT parameter that contains the number of bytes actually written in
    //      the buffer.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is used to receive data from a connected socket.
    //
    //      When the received data is too big to fit in puData, no error will
    //      be returned, and puSize will be set to uCapacity.  In UDP, the
    //      remaining data is discarded.  In TCP, the remaining data is
    //      available by making another call to Recv.
    //
    //  See Also:
    //      - IIoSocket::Recv
    //      - IIoSocket::RecvFrom
    //
    //==EDOC====================================================================
    virtual mxt_result Recv(OUT uint8_t* puData,
                            IN unsigned int uCapacity,
                            OUT unsigned int* puSize) = 0;

    //==SDOC====================================================================
    //==
    //==  RecvFrom
    //==
    //==========================================================================
    //
    //  Summary:
    //      Receives data from a non-connected socket.
    //
    //  Parameters:
    //    pData:
    //      Blob object that will be filled with the data received on the
    //      socket.
    //
    //    pPeerAddress:
    //      Address from which the data was received.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is used to receive data from an unconnected socket. This
    //      should be called only when notified that there is data available on
    //      the socket.
    //
    //      When the received data is too big to fit in the Blob, no error will
    //      be returned.  In UDP, the remaining data is discarded. In TCP, the
    //      remaining data is available by making another call to Recv.
    //
    //  See Also:
    //      - IIoSocket::RecvFrom
    //      - IIoSocket::Recv
    //
    //==EDOC====================================================================
    virtual mxt_result RecvFrom(OUT CBlob* pData, OUT CSocketAddr* pPeerAddress) = 0;

    //==SDOC====================================================================
    //==
    //==  RecvFrom
    //==
    //==========================================================================
    //
    //  Summary:
    //      Receives data from a non-connected socket.
    //
    //  Parameters:
    //    puData:
    //      Pointer to a buffer where the received data is to be stored.
    //
    //    uCapacity:
    //      The maximum size available in the buffer pointed to by puData.
    //
    //    puSize:
    //      OUT parameter that contains the number of bytes actually written in
    //      the buffer.
    //
    //    pPeerAddress:
    //      Address from which the data was received.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is used to receive data from an unconnected socket. This
    //      should be called only when notified that there is data available on
    //      the socket.
    //
    //      When the received data is too big to fit in puData, no error will
    //      be returned, and puSize will be set to uCapacity.  In UDP, the
    //      remaining data is discarded.  In TCP, the remaining data is
    //      available by making another call to Recv.
    //
    //  See Also:
    //      - IIoSocket::RecvFrom
    //      - IIoSocket::Recv
    //
    //==EDOC====================================================================
    virtual mxt_result RecvFrom(OUT uint8_t* puData,
                                IN unsigned int uCapacity,
                                OUT unsigned int* puSize,
                                OUT CSocketAddr* pPeerAddress) = 0;


    //==SDOC====================================================================
    //==
    //==  Send
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends data on a connected socket.
    //
    //  Parameters:
    //    pData:
    //      Pointer to a blob object that contains the data to be sent.
    //
    //    puSizeSent:
    //      On output, will contain the number of bytes that were actually sent.
    //      The value returned in this parameter is not reliable if the method
    //      returns anything else than resS_OK.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is used to send data on a connected socket.
    //
    //      An application MUST not rely on the value of puSizeSent when this
    //      method returns anything else than resS_OK.
    //
    //  See Also:
    //     - IIoSocket::Send
    //     - IIoSocket::SendTo
    //
    //==EDOC====================================================================
    virtual mxt_result Send(IN const CBlob* pData,
                            OUT unsigned int* puSizeSent) = 0;


    //==SDOC====================================================================
    //==
    //==  Send
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends data on a connected socket.
    //
    //  Parameters:
    //    puData:
    //      Pointer to a buffer containing the data to send.
    //
    //    uSize:
    //      The number of bytes in the buffer that must be sent.
    //
    //    puSizeSent:
    //      On output, will contain the number of bytes that were actually sent.
    //      The value returned in this parameter is not reliable if the method
    //      returns anything else than resS_OK.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is used to send data on a connected socket.
    //
    //      An application MUST not rely on the value of puSizeSent when this
    //      method returns anything else than resS_OK.
    //
    //  See Also:
    //      - IIoSocket::Send
    //      - IIoSocket::SendTo
    //
    //==EDOC====================================================================
    virtual mxt_result Send(IN const uint8_t* puData,
                            IN unsigned int uSize,
                            OUT unsigned int* puSizeSent) = 0;


    //==SDOC====================================================================
    //==
    //==  SendTo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends data on a non-connected socket.
    //
    //  Parameters:
    //    pData:
    //      Pointer to a blob containing the data to be sent.
    //
    //    puSizeSent:
    //      On output, will contain the number of bytes that were actually sent.
    //      The value returned in this parameter is not reliable if the method
    //      returns anything else than resS_OK.
    //
    //    pPeerAddress:
    //      Address to which the data must be sent.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is used to send data on a non-connected socket.
    //
    //      An application MUST not rely on the value of puSizeSent when this
    //      method returns anything else than resS_OK.
    //
    //  See Also:
    //      - IIoSocket::SendTo
    //      - IIoSocket::Send
    //
    //==EDOC====================================================================
    virtual mxt_result SendTo(IN const CBlob* pData,
                              OUT unsigned int* puSizeSent,
                              IN const CSocketAddr* pPeerAddress) = 0;

    //==SDOC====================================================================
    //==
    //==  SendTo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends data on a non-connected socket.
    //
    //  Parameters:
    //    puData:
    //      Pointer to a buffer containing the data to send.
    //
    //    uSize:
    //      The number of bytes in the buffer that must be sent.
    //
    //    puSizeSent:
    //      On output, will contain the number of bytes that were actually sent.
    //      The value returned in this parameter is not reliable if the method
    //      returns anything else than resS_OK.
    //
    //    pPeerAddress:
    //      Address to which the data must be sent.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is used to send data on a non-connected socket.
    //
    //      An application MUST not rely on the value of puSizeSent when this
    //      method returns anything else than resS_OK.
    //
    //  See Also:
    //      - IIoSocket::SendTo
    //      - IIoSocket::Send
    //
    //==EDOC====================================================================
    virtual mxt_result SendTo(IN const uint8_t* puData,
                              IN unsigned int uSize,
                              OUT unsigned int* puSizeSent,
                              IN const CSocketAddr* pPeerAddress) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //==SDOC====================================================================
    //==
    //==  IIoSocket
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
    IIoSocket(){};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~IIoSocket(){};
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_IIOSOCKET_H
