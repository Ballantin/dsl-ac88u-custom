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
#ifndef MXG_CTCPSOCKETOPTIONS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTCPSOCKETOPTIONS_H
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
#include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_ECOM_ENABLE_SUPPORT)

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif
#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif
#ifndef MXG_IASYNCSOCKETBUFFERSIZEOPTIONS_H
#include "Network/IAsyncSocketBufferSizeOptions.h"
#endif
#ifndef MXG_IASYNCSOCKETQUALITYOFSERVICEOPTIONS_H
#include "Network/IAsyncSocketQualityOfServiceOptions.h"
#endif
#ifndef MXG_IASYNCSOCKETTCPOPTIONS_H
#include "Network/IAsyncSocketTcpOptions.h"
#endif
#ifndef MXG_IASYNCSOCKETWINDOWSGQOSOPTIONS_H
#include "Network/IAsyncSocketWindowsGqosOptions.h"
#endif
#ifndef MXG_ITCPSOCKETOPTIONSCONFIGURE_H
#include "Network/ITcpSocketOptionsConfigure.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class IAsyncSocket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CTcpSocketOptions
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    An ECOM object that can be used to configure TCP socket options.
//
//  Description:
//    An ECOM object that can be used to configure TCP socket options. Until a
//    TCP socket is specified via ITcpSocketOptionsConfigure, options set via
//    other interfaces are cached.
//
//  Location:
//    Network/CTcpSocketOptions.h
//
//==============================================================================
class CTcpSocketOptions : public CEComDelegatingUnknown,
                          public IAsyncSocketBufferSizeOptions,
                          public IAsyncSocketQualityOfServiceOptions,
                          public IAsyncSocketTcpOptions,
#if defined(MXD_OS_WINDOWS) && \
    defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
                          public IAsyncSocketWindowsGqosOptions,
#endif
                          public ITcpSocketOptionsConfigure
{
//-- Published Interface
public:
    // Summary:
    // This is the ECOM Creation handler.
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    // The following statement is essential, it provide the default and unique
    // implementation of the IEComUnknown interface that every other interfaces
    // inherit from.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    // Inherited from CEComDelegatingUnknown.

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    // Inherited from IAsyncSocketBufferSizeOptions.

    virtual mxt_result SetReceiveBufferSize(IN unsigned int uSize);
    virtual mxt_result SetTransmitBufferSize(IN unsigned int uSize);

    // Inherited from IAsyncSocketQualityOfServiceOptions.

    virtual mxt_result Set8021QUserPriority(IN bool bEnable,
                                            IN uint8_t uUserPriority);
    virtual mxt_result SetTos(IN uint8_t uTos);

    // Inherited from IAsyncSocketWindowsGqosOptions.
//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_OS_WINDOWS) && \
    defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
//M5T_INTERNAL_USE_END
    virtual mxt_result SetWindowsReceivingFlowspec(IN FLOWSPEC* pReceivingFlowspec);
    virtual mxt_result SetWindowsSendingFlowspec(IN FLOWSPEC* pSendingFlowspec);
//M5T_INTERNAL_USE_BEGIN
#endif
//M5T_INTERNAL_USE_END

    // Inherited from IAsyncSocketTcpOptions

    virtual mxt_result SetKeepAlive(IN bool bEnable);
    virtual mxt_result SetNagle(IN bool bEnable);
    virtual void SetConnectTimeoutMs(IN uint64_t uConnectTimeoutMs);
    virtual mxt_result SetBackgroundVoipSocket(IN bool bEnable);

    // Inherited from ITcpSocketOptionsConfigure.

    virtual mxt_result ApplyOptions(IN IAsyncSocket* pIAsyncSocket);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
private:
    friend mxt_result InitializeCTcpSocketOptions();
    friend void FinalizeCTcpSocketOptions();

    CTcpSocketOptions(IEComUnknown* pOuterIEComUnknown = NULL);
    virtual ~CTcpSocketOptions();
    CTcpSocketOptions(const CTcpSocketOptions& from);
    CTcpSocketOptions& operator=(const CTcpSocketOptions& from);

    // Support for IAsyncSocketBufferSizeOptions.

    void ApplyAsyncSocketBufferSizeOptionsCache(IN IAsyncSocket* pIAsyncSocket);

    // Support for IAsyncSocketQualityOfServiceOptions.

    void ApplyAsyncSocketQualityOfServiceOptionsCache(IN IAsyncSocket* pIAsyncSocket);

    // Support for IAsyncSocketTcpOptions.

    void ApplyAsyncSocketTcpOptionsCache(IN IAsyncSocket* pIAsyncSocket);

//-- Hidden Data Members
private:

    CMutex m_mutex;

    // Cache for IAsyncSocketBufferSizeOptions.
    bool m_bReceiveBufferSizeCache;
    unsigned int m_uReceiveBufferSizeSizeCache;
    bool m_bTransmitBufferSizeCache;
    unsigned int m_uTransmitBufferSizeSizeCache;

    // Cache for IAsyncSocketQualityOfServiceOptions.
    bool m_b8021QUserPriorityCache;
    bool m_b8021QUserPriorityEnableCache;
    uint8_t m_u8021QUserPriorityUserPriorityCache;
    bool m_bTosCache;
    unsigned int m_uTosTosCache;

    // Cache for IAsyncSocketWindowsGqosOptions.
#if defined(MXD_OS_WINDOWS) && \
    defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
    bool m_bWindowsReceivingFlowspecCache;
    FLOWSPEC* m_pWindowsReceivingFlowspecReceivingFlowspecCache;

    bool m_bWindowsSendingFlowspecCache;
    FLOWSPEC* m_pWindowsSendingFlowspecSendingFlowspecCache;
#endif

    // Cache for IAsyncSocketTcpOptions.
    bool m_bKeepAliveCache;
    bool m_bKeepAliveEnableCache;
    bool m_bNagleCache;
    bool m_bNagleEnableCache;
    bool m_bConnectTimeoutCache;
    uint64_t m_uConnectTimeoutCacheMs;
    bool m_bBackgroundVoipSocketCache;
    bool m_bBackgroundVoipSocketEnableCache;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_CTCPSOCKETOPTIONS_H
