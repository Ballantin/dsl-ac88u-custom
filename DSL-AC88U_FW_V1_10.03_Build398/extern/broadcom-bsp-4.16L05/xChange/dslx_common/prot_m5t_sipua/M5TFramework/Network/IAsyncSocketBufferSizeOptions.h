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
#ifndef MXG_IASYNCSOCKETBUFFERSIZEOPTIONS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCSOCKETBUFFERSIZEOPTIONS_H
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
MX_DECLARE_ECOM_IID(IAsyncSocketBufferSizeOptions);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncSocketBufferSizeOptions
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Interface defining the socket options that are related to buffer size.
//
//  Description:
//    This interface defines the socket options that are related to buffer size.
//
//  Location:
//    Network/IAsyncSocketBufferSizeOptions.h
//
//  See Also:
//    IAsyncSocketUdpOptions
//    IAsyncSocketTcpOptions
//    IAsyncSocketQualityOfServerOptions
//    IAsyncSocketWindowsGqosOptions
//
//==============================================================================
class IAsyncSocketBufferSizeOptions : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncSocketBufferSizeOptions);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetReceiveBufferSize
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets a socket level option which determines the maximum socket
    //    receive buffer size.
    //
    //  Parameters:
    //    uSize:
    //      The size of the receive buffer.
    //
    //  Returns:
    //    resS_OK: The receive buffer size has been successfully set.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method sets a socket level option which determines the maximum
    //    socket receive buffer size in bytes. Usually the default value of
    //    this option is adequate; only experts should change this value.
    //    This method can be called even if the socket is not bound yet.
    //
    //  Notes:
    //    This option is target dependent. If it is compiled for a target that
    //    does not support the option (like Nucleus), the execution of this
    //    method will output an error trace before returning with resS_OK.
    //
    //==========================================================================
    virtual mxt_result SetReceiveBufferSize(IN unsigned int uSize) = 0;

    //==========================================================================
    //==
    //==  SetTransmitBufferSize
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets a socket level option which determines the maximum socket
    //    transmit buffer size.
    //
    //  Parameters:
    //    uSize:
    //      The size of the transmit buffer.
    //
    //  Returns:
    //    resS_OK: The transmit buffer size has been successfully set.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method sets a socket level option which determines the maximum
    //    socket transmit buffer size in bytes. Usually the default value of
    //    this option is adequate; only experts should change this value.
    //    This method can be called even if the socket is not bound yet.
    //
    //  Notes:
    //    This option is target dependent. If it is compiled for a target that
    //    does not support the option (like Nucleus), the execution of this
    //    method will output an error trace before returning with resS_OK.
    //
    //==========================================================================
    virtual mxt_result SetTransmitBufferSize(IN unsigned int uSize) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncSocketBufferSizeOptions
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
    IAsyncSocketBufferSizeOptions() {}

    //==========================================================================
    //==
    //==  ~IAsyncSocketBufferSizeOptions
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
    virtual ~IAsyncSocketBufferSizeOptions() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncSocketBufferSizeOptions(const IAsyncSocketBufferSizeOptions& from);
    IAsyncSocketBufferSizeOptions& operator=(const IAsyncSocketBufferSizeOptions& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)


#endif // MXG_IASYNCSOCKETBUFFERSIZEOPTIONS_H
