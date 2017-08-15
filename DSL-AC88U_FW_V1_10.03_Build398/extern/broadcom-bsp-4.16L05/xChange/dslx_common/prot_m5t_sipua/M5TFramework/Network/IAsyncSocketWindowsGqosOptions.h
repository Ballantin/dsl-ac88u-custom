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
#ifndef MXG_IASYNCSOCKETWINDOWSGQOSOPTIONS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCSOCKETWINDOWSGQOSOPTIONS_H
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
// This interface uses the FLOWSPEC datatype which is only define on Windows
// i386 platforms. Therefore, for this interface to compile on other platforms
// the FLOWSPEC datatype is defined here as mxt_opaque when required.
#if !defined(MXD_OS_WINDOWS) && \
    !defined(FLOWSPEC)
#define FLOWSPEC mxt_opaque
#endif

MX_DECLARE_ECOM_IID(IAsyncSocketWindowsGqosOptions);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncSocketWindowsGqosOptions
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Interface defining the socket options that are related to quality of
//    service. This interface is specialized for the Windows GQOS flowspec
//    option.
//
//  Description:
//    This interface defines the socket options that are related to quality of
//    service. This interface is specialized for the Windows GQOS flowspec
//    option.
//
//  Location:
//    Network/IAsyncSocketWindowsGqosOptions.h
//
//  See Also:
//    IAsyncSocketUdpOptions
//    IAsyncSocketTcpOptions
//    IAsyncSocketBufferSizeOptions
//    IAsyncSocketQualityOfServerOptions
//
//==============================================================================
class IAsyncSocketWindowsGqosOptions : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncSocketWindowsGqosOptions);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetWindowsReceivingFlowspec
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the receiving flowspec (Windows only).
    //
    //  Parameters:
    //    pReceivingFlowspec:
    //      The value of the receiving flowspec.
    //
    //  Returns:
    //    resS_OK: The option has been successfully set.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method sets the receiving flowspec (Windows only). This method
    //    can be called even if the socket is not bound yet.
    //
    //==========================================================================
    virtual mxt_result SetWindowsReceivingFlowspec(IN FLOWSPEC* pReceivingFlowspec) = 0;

    //==========================================================================
    //==
    //==  SetWindowsSendingFlowspec
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the sending flowspec (Windows only).
    //
    //  Parameters:
    //    pSendingFlowspec:
    //      The value of the sending flow spec.
    //
    //  Returns:
    //    resS_OK: The option has been successfully set.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method sets the sending flowspec (Windows only).  This method
    //    can be called even if the socket is not bound yet.
    //
    //==========================================================================
    virtual mxt_result SetWindowsSendingFlowspec(IN FLOWSPEC* pSendingFlowspec) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncSocketQualityOfServiceOptions
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
    IAsyncSocketWindowsGqosOptions() {}

    //==========================================================================
    //==
    //==  ~IAsyncSocketQualityOfServiceOptions
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
    virtual ~IAsyncSocketWindowsGqosOptions() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncSocketWindowsGqosOptions(const IAsyncSocketWindowsGqosOptions& from);
    IAsyncSocketWindowsGqosOptions& operator=(const IAsyncSocketWindowsGqosOptions& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) && 
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_IASYNCSOCKETWINDOWSGQOSOPTIONS_H
