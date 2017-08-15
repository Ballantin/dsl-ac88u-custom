//==============================================================================
//==============================================================================
//
//     Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CUDPTRACING_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUDPTRACING_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_CAP_ENABLE_SUPPORT) && \
    defined(MXD_ECOM_ENABLE_SUPPORT)

#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif

#ifndef MXG_IASYNCRESOLVERUSER_H
#include "Resolver/IAsyncResolverUser.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CUdpSocket;
class CSocketAddr;
class CFqdn;

//==============================================================================
//== Class: CUdpTracing
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
// Summary :
//  UDP trace output handler.
//
// Description:
//  This static class is used to add an output handler to the tracing
//  mechanism.  This output handler sends traces as clear text on a network
//  using a UDP socket.
//
// NOTE: For the UDP output handler to work, you must define
//   MXD_TRACE_MAX_NB_OF_OUTPUT_HANDLERS with a value of at least 2, in your
//   PreMxConfig.h.
//
// Location:
//   Network/CUdpTracing.h
//
// See Also:
//   MxTrace, CUdpSocket
//
//==============================================================================
class CUdpTracing : private IAsyncResolverUser
{

//-- Published Interface
//------------------------
public:

    // Summary :
    //  Initializes the socket and registers the output handler.
    //-----------------------------------------------------------
    static mxt_result Initialize(IN const CFqdn* pRemoteFqdn,
                                 IN const CSocketAddr* pLocalAddr = NULL,
                                 IN unsigned int uFqdnValidationPeriodMs = 30000);

    // Summary :
    //  Closes the socket and unregisters the output handler.
    //--------------------------------------------------------
    static mxt_result Finalize();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:

    //-- << Constructors / Destructors / Operators >>.

    // Summary:
    //   Default constructor.
    CUdpTracing();

    // Summary:
    //  Destructor.
    ~CUdpTracing();

private:

    // Summary:
    //  Initializes the CUdpTracing instance.
    mxt_result InitializeInstance(IN const CFqdn* pRemoteFqdn,
                                  IN const CSocketAddr* pLocalAddr,
                                  IN unsigned int uFqdnValidationPeriodMs);

    // Summary:
    //  UDP trace output handler.
    static void TraceToUdp(IN EMxTraceLevel eLevel,
                           IN uint32_t uTraceUniqueId,
                           IN const char* pszTrace,
                           IN int nMsgSize);

    // Summary:
    //  Helper for UDP trace output handler.
    void TraceToUdpInstance(IN EMxTraceLevel eLevel,
                            IN uint32_t uTraceUniqueId,
                            IN const char* pszTrace,
                            IN int nMsgSize);

    // Summary:
    //  Returns the unique instance of this class.
    static CUdpTracing* GetInstance();

    //-- << IAsyncResolverUser >>.

    virtual void EvAsyncResolverUserResponseReceived(IN CList<CSocketAddr>& rlstAddresses,
                                                     IN mxt_opaque opq);
    virtual void EvAsyncResolverUserResponseReceived(IN CList<CString>& rlststrNames,
                                                     IN mxt_opaque opq);
    virtual void EvAsyncResolverUserResponseReceived(IN CList<CDnsPacket::SSrvRecordData>& rlstServices,
                                                     IN mxt_opaque opq);
    virtual void EvAsyncResolverUserResponseReceived(IN CList<CDnsPacket::SNaptrRecordData>& rlstPointers,
                                                     IN mxt_opaque opq);
#if defined(MXD_ENUM_ENABLE_SUPPORT)
    virtual void EvAsyncResolverUserResponseReceived(IN CList<CDnsPacket::SEnumUriData>& rlstUris, IN mxt_opaque opq);
#endif

#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // warning or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif

    //-- << Deactivated Constructors / Destructors / Operators >>.

    // Summary:
    //  Copy Constructor.
    CUdpTracing(IN const CUdpTracing& rSrc);

    // Summary:
    //  Assignment Operator.
    CUdpTracing& operator=(IN const CUdpTracing& rSrc);

//-- Hidden Data Members
//------------------------
private:
    CUdpSocket* m_pUdpTraceSocket;
    CFqdn* m_pRemoteFqdn;
    CSocketAddr* m_pRemoteAddr;
    uint64_t m_uLastFqdnValidationMs;
    CMutex m_mutex;
    unsigned int m_uFqdnValidationPeriodMs;
    bool m_bIsTracing;

    // This is a singleton, this points to the single instance.
    static CUdpTracing* ms_pInstance;

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) ...

#endif // MXG_CUDPTRACING_H
