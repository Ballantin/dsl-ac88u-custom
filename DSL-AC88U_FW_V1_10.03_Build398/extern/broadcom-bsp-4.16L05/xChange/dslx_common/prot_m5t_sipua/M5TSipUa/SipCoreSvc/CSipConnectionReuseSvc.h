//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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

//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CSIPCONNECTIONREUSESVC_H
#define MXG_CSIPCONNECTIONREUSESVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#ifndef MXD_SIPSTACK_ENABLE_SIP_CONNECTION_REUSE_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_CONNECTION_REUSE_SVC_SUPPORT to be able to use \
this class.
#endif

//-- Data Members
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSIPPACKET_H
#include "SipTransport/CSipPacket.h"
#endif

#ifndef MXG_ISIPREQUESTCONTEXT_H
#include "SipCore/ISipRequestContext.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCONNECTIONREUSESVC_H
#include "SipCoreSvc/ISipConnectionReuseSvc.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_ISIPREQCTXCONNECTIONSVC_H
#include "SipCore/ISipReqCtxConnectionSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipConnectionReuseSvc
//==============================================================================
//
// Description:
//   This class implements the ISipConnectionReuseSvc interface.
//
//   It is a Connection Service and a Core Service. The implementation of
//   ISipReqCtxCoreSvc is provided by CSipConnectionReuseReqCtxCoreSvc instead
//   of being provided by this class because some of the methods of the
//   ISipReqCtxConnectionSvc and ISipReqCtxCoreSvc interfaces have identical
//   signatures, such as UpdatePacket.
//
//   This class must be used without the ISipServerlocatorSvc.
//
// Location:
//    SipCoreSvc/CSipConnectionReuseSvc.h
//
//==============================================================================
class CSipConnectionReuseSvc : private CEComDelegatingUnknown,
                               private ISipConnectionReuseSvc,
                               private ISipConnectionSvc,
                               private ISipReqCtxConnectionSvc,
                               private ISipCoreSvc
{
//-- Friend Declarations

    friend class CSipStackInitializer;

//-- Published Interface
public:

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //-- << ISipConnectionReuseSvc Interface >>.

    //  Summary:
    //   Sets the local address.
    virtual void SetLocalAddress(IN const CSocketAddr& rLocalAddress);

    //  Summary:
    //   Sets the peer address.
    virtual void SetPeerAddress(IN const CSocketAddr& rPeerAddress);

    // Summary:
    //   Sets the transport.
    virtual void SetTransport(IN ESipTransport eTransport);

    //  Summary:
    //   Gets the local address.
    virtual const CSocketAddr& GetLocalAddress();

    //  Summary:
    //   Gets the peer address.
    virtual const CSocketAddr& GetPeerAddress();

    //  Summary:
    //   Gets the transport.
    virtual ESipTransport GetTransport();



// Hidden Methods
private:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipConnectionReuseSvc();

    static void FinalizeCSipConnectionReuseSvc();

    // << CEComDelegatingUnknown >>
    // << ECom Pattern >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << ISipConnectionSvc Interface >>.

    // Summary:
    //  Queries the ISipReqCtxConnectionSvc instance.
    virtual ISipReqCtxConnectionSvc* GetReqCtxConnectionSvcInstance(
                                  IN const CSipPacket& rPacket,
                                  IN ISipConnectionSvc::EDirection eDirection);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    virtual void ClearConnectionSvc();

    //-- << ISipReqCtxConnectionSvc Interface >>.

    // Summary:
    //  Method called after an updated packet was successfully sent.
    virtual void OnSendSuccess(IN ISipRequestContext& rRequestContext,
                               IN const CSipPacket& rPacketSent);

    // Summary:
    //  Method called after an updated packet was not successfully sent.
    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    // Summary:
    //  Updates rPacket with the resolved target.
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    // Summary:
    //  Destroys this instance of ISipReqCtxConnectionSvc.
    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

    //-- << ISipCoreSvc >>

    //  Summary:
    //      Obtain an instance of an ISipReqCtxCoreSvc to attach to a request
    //      context.
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                    IN const CSipPacket& rPacket,
                                    IN ISipCoreSvc::EDirection eDirection);

    //  Summary:
    //      Gives to which degree the service is a good candidate to own a
    //      request context.
    virtual ISipCoreSvc::EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                                IN const CSipPacket& rPacket);

    //  Summary:
    //      Return the ISipReqCtxCoreSvc instance to use as the owner for a
    //      request context.
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    virtual mxt_result ClearCoreSvc();


//-- Hidden Methods
protected:
private:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSipConnectionReuseSvc();

    // Summary:
    //  Constructor.
    CSipConnectionReuseSvc(IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipConnectionReuseSvc();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // GCC 2.7.2 does not like empty private copy constructors (when building in debug).
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)

    // Summary:
    //  Copy Constructor.
    CSipConnectionReuseSvc(IN const CSipConnectionReuseSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipConnectionReuseSvc& operator=(IN const CSipConnectionReuseSvc& rSrc);

#endif

// Friends declarations

//-- Hidden Data Members
protected:
private:

    CSocketAddr m_localAddr;
    CSocketAddr m_peerAddr;
    ESipTransport m_eTransport;

};

inline void CSipConnectionReuseSvc::SetLocalAddress(IN const CSocketAddr& rLocalAddr)
{
    m_localAddr = rLocalAddr;
}

inline void CSipConnectionReuseSvc::SetPeerAddress(IN const CSocketAddr& rPeerAddress)
{
    m_peerAddr = rPeerAddress;
}

inline void CSipConnectionReuseSvc::SetTransport(IN ESipTransport eTransport)
{
    m_eTransport = eTransport;
}

inline const CSocketAddr& CSipConnectionReuseSvc::GetLocalAddress()
{
    return m_localAddr;
}

inline const CSocketAddr& CSipConnectionReuseSvc::GetPeerAddress()
{
    return m_peerAddr;
}

inline ESipTransport CSipConnectionReuseSvc::GetTransport()
{
    return m_eTransport;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCONNECTIONREUSESVC_H
//M5T_INTERNAL_USE_END
