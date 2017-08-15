//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_IASYNCSOCKETTCPOPTIONS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCSOCKETTCPOPTIONS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_ECOM_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_ECOM_ENABLE_SUPPORT)

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncSocketTcpOptions);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncSocketTcpOptions
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Interface defining the options that are configurable on a TCP socket.
//
//  Description:
//    This interface defines the options that are configurable on a TCP socket.
//    There are two configurable options: the keepalive and the Nagle algorithm
//    options. The reuse address option is automatically set and is not
//    configurable.
//
//  Location:
//    Network/IAsyncSocketTcpOptions.h
//
//  See Also:
//    IAsyncSocketUdpOptions
//    IAsyncSocketBufferSizeOptions
//    IAsyncSocketQualityOfServerOptions
//    IAsyncSocketWindowsGqosOptions
//
//==============================================================================
class IAsyncSocketTcpOptions : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncSocketTcpOptions);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetKeepAlive
    //==
    //==========================================================================
    //
    //  Summary:
    //    Enables/disables the TCP keep-alive option.
    //
    //  Parameters:
    //    bEnable:
    //      If true, enables the TCP keep-alive messages on the socket.
    //
    //  Returns:
    //    resS_OK: The option has been successfully enabled.
    //    resFE_NOT_IMPLEMENTED: This option cannot be set on a TCP server
    //                           socket.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to enable/disable the TCP socket option to
    //    send keep-alive messages on an idle TCP connection, using the
    //    operating system's default settings.
    //
    //==========================================================================
    virtual mxt_result SetKeepAlive(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  SetNagle
    //==
    //==========================================================================
    //
    //  Summary:
    //    Enables/disables the TCP Nagle algorithm option.
    //
    //  Parameters:
    //    bEnable:
    //      If true, enables the TCP Nagle algorithm on the socket.
    //
    //  Returns:
    //    resS_OK: The option has been successfully enabled.
    //    resFE_NOT_IMPLEMENTED: This option cannot be set on a TCP server
    //                           socket.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to enable/disable the TCP Nagle algorithm option.
    //    The Nagle algorithm controls how the TCP stream is packed into
    //    TCP fragment.
    //
    //==========================================================================
    virtual mxt_result SetNagle(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  SetConnectTimeoutMs
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the connect timeout in ms.
    //
    //  Parameters:
    //    uConnectTimeoutMs:
    //      The connect timeout.
    //
    //  Description:
    //      Sets the connect timeout in ms. If a TCP socket is not connected when
    //      the timeout occurs, an error is reported to the manager. If the OS
    //      timer is smaller than this value, it has priority. Does nothing if
    //      set on a TCP server socket.
    //
    //==========================================================================
    virtual void SetConnectTimeoutMs(IN uint64_t uConnectTimeoutMs) = 0;

    //==========================================================================
    //==
    //==  SetBackgroundVoipSocket
    //==
    //==========================================================================
    //
    //  Summary:
    //    Enables/disables the TCP background VOIP socket support.
    //
    //  Parameters:
    //    bEnable:
    //      If true, enables the TCP background VOIP socket support on the
    //      socket.
    //
    //  Returns:
    //    resS_OK if the option has been successfully configured, a failure
    //    otherwise.
    //
    //  Description:
    //    This method is used to enable/disable the TCP background VOIP socket
    //    support. A background socket is registered to be awaken if data is
    //    received on it while the application is in the background. This is
    //    only supported on iOS and will do nothing on other platforms.
    //
    //    Also, it is important to note that the configured value may be
    //    buffered until it can be used (when the TCP server socket is bound
    //    or when the TCP client socket is connected). As such, it is possible
    //    an error can occur when the buffered value will be set for real. In
    //    all other cases, the real result of the operation will be returned
    //    directly.
    //
    //==========================================================================
    virtual mxt_result SetBackgroundVoipSocket(IN bool bEnable) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncSocketTcpOptions
    //==
    //==========================================================================
    //
    //  Summary:
    //    Constructor.
    //
    //  Description:
    //    Default Constructor.
    //
    //==========================================================================
    IAsyncSocketTcpOptions() {}

    //==========================================================================
    //==
    //==  ~IAsyncSocketTcpOptions
    //==
    //==========================================================================
    //
    //  Summary:
    //    Destructor.
    //
    //  Description:
    //    Destructor.
    //
    //==========================================================================
    virtual ~IAsyncSocketTcpOptions() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncSocketTcpOptions(const IAsyncSocketTcpOptions& from);
    IAsyncSocketTcpOptions& operator=(const IAsyncSocketTcpOptions& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_IASYNCSOCKETTCPOPTIONS_H
