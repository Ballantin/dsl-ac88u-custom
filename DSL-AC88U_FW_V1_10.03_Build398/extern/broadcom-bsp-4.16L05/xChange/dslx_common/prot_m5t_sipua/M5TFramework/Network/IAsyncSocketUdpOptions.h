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
#ifndef MXG_IASYNCSOCKETUDPOPTIONS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCSOCKETUDPOPTIONS_H
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
MX_DECLARE_ECOM_IID(IAsyncSocketUdpOptions);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncSocketUdpOptions
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Interface defining the options that are configurable on a UDP socket.
//
//  Description:
//    This interface defines the options that are configurable on a UDP socket.
//    There is one configurable option: the broadcast option. The reuse address
//    option is automatically set and is not configurable.
//
//  Location:
//    Network/IAsyncSocketUdpOptions.h
//
//  See Also:
//    IAsyncSocketTcpOptions
//    IAsyncSocketBufferSizeOptions
//    IAsyncSocketQualityOfServerOptions
//    IAsyncSocketWindowsGqosOptions
//
//==============================================================================
class IAsyncSocketUdpOptions : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncSocketUdpOptions);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetBroadcast
    //==
    //==========================================================================
    //
    //  Summary:
    //    Enables/disables the UDP broadcast option.
    //
    //  Parameters:
    //    bEnable:
    //      If true, enable the UDP broadcast messages on the socket.
    //
    //  Returns:
    //    resS_OK: The option has been successfully enabled.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to enable/disable the UDP socket option to
    //    broadcast messages on the socket. This method can be called even
    //    if the socket is not bound yet.
    //
    //==========================================================================
    virtual mxt_result SetBroadcast(IN bool bEnable) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncSocketUdpOptions
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
    IAsyncSocketUdpOptions() {}

    //==========================================================================
    //==
    //==  ~IAsyncSocketUdpOptions
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
    virtual ~IAsyncSocketUdpOptions() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncSocketUdpOptions(const IAsyncSocketUdpOptions& from);
    IAsyncSocketUdpOptions& operator=(const IAsyncSocketUdpOptions& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_IASYNCSOCKETUDPOPTIONS_H
