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
#ifndef MXG_ITCPSOCKETOPTIONSCONFIGURE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ITCPSOCKETOPTIONSCONFIGURE_H
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

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class IAsyncSocket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ITcpSocketOptionsConfigure);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ITcpSocketOptionsConfigure
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Interface allowing the client to provide the necessary configuration
//    information required by objects engaged in setting TCP socket options.
//
//  Description:
//    This interface allows the client to provide the necessary configuration
//    information required by objects engaged in setting TCP Socket options.
//
//  Location:
//    Network/ITcpSocketOptionsConfigure.h
//
//==============================================================================
class ITcpSocketOptionsConfigure : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ITcpSocketOptionsConfigure);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //== ApplyOptions
    //==
    //==========================================================================
    //
    //  Summary:
    //    Apply the options to be used, stored in the instance of this interface,
    //    on the socket provided as argument.
    //
    //  Parameters:
    //    pIAsyncSocket
    //      The pointer to an asynchronous socket.
    //
    //  Returns:
    //    resS_OK: The socket options have been successfully set.
    //    resFE_MITOSFW_ECOM_NOINTERFACE
    //
    //  Description:
    //    Apply the options to be used, stored in the instance of this interface,
    //    on the socket provided as argument.
    //
    //==========================================================================
    virtual mxt_result ApplyOptions(IN IAsyncSocket* pIAsyncSocket) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  ITcpSocketOptionsConfigure
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
    ITcpSocketOptionsConfigure() {}

    //==========================================================================
    //==
    //==  ~ITcpSocketOptionsConfigure
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
    virtual ~ITcpSocketOptionsConfigure() {}

private:
    // Deactivated Constructors / Destructors / Operators
    ITcpSocketOptionsConfigure(const ITcpSocketOptionsConfigure& from);
    ITcpSocketOptionsConfigure& operator=(const ITcpSocketOptionsConfigure& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) && 
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_ITCPSOCKETOPTIONSCONFIGURE_H
