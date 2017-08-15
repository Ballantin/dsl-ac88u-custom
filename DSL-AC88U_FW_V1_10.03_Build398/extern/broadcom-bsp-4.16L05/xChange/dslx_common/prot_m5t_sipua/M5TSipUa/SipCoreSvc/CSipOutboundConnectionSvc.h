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
#ifndef MXG_CSIPOUTBOUNDCONNECTIONSVC_H
#define MXG_CSIPOUTBOUNDCONNECTIONSVC_H

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

#ifndef MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT to be able to use \
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

#ifndef MXG_ISIPOUTBOUNDCONNECTIONSVC_H
#include "SipCoreSvc/ISipOutboundConnectionSvc.h"
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
class CSipPersistentConnectionList;
class CSipOutboundReqCtxCoreSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipOutboundConnectionSvc
//==============================================================================
//
// Description:
//   This class implements the ISipOutboundConnectionSvc interface.
//
//   It is a Connection Service and a Core Service. The implementation of
//   ISipReqCtxCoreSvc is provided by CSipOutboundReqCtxCoreSvc instead of
//   being provided by this class because some of the methods of the
//   ISipReqCtxConnectionSvc and ISipReqCtxCoreSvc interfaces have identical
//   signatures, such as UpdatePacket. This class creates an instance of
//   the CSipOutboundReqCtxCoreSvc when a REGISTER request is to be sent.
//
//   When used without a ServerLocator service and updating a packet, the service
//   verifies the state of the preferred persistent connection. If it is not
//   connected, it implements failover and uses the next available connection.
//
//   It adds the outbound and path option tags to requests, if they are not
//   already present. In REGISTER requests, it adds reg-id and instance-id
//   parameters to the CONTACT header.
//
//   When attached after the Server Locator service, this service requests the
//   application to create a persistent connection when sending a non-REGISTER
//   request for which there is no persistent connection existing.
//
// Location:
//    SipCoreSvc/CSipOutboundConnectionSvc.h
//
//==============================================================================
class CSipOutboundConnectionSvc : private CEComDelegatingUnknown,
                                  private ISipOutboundConnectionSvc,
                                  private ISipConnectionSvc,
                                  private ISipReqCtxConnectionSvc,
                                  private ISipCoreSvc
{
//-- Friend Declarations

    friend class CSipStackInitializer;
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
    // GCC 2.7.2 does not handle well the overridden QueryIf when implemented by
    // multiple interfaces, in which case the following class needs to specify
    // on which private interface the QueryIf must be called.
    friend class CSipOutboundReqCtxCoreSvc;
#endif

//-- Published Interface
public:

    // Summary:
    //  Returns the ISipOutboundConnectionMgr manager.
    ISipOutboundConnectionMgr* GetManager();

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

// Hidden Methods
private:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipOutboundConnectionSvc();

    static void FinalizeCSipOutboundConnectionSvc();

    // << CEComDelegatingUnknown >>
    // << ECom Pattern >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << ISipOutboundConnectionSvc Interface >>.

    virtual mxt_result SetManager(IN ISipOutboundConnectionMgr* pMgr);

    virtual mxt_result SetPersistentConnectionsPreferredOrder(IN CVector<mxt_opaque>& rvecConnections);

    virtual const CVector<mxt_opaque>& GetPersistentConnectionsPreferredOrder() const;

    virtual mxt_result ConnectionCreationResult(IN mxt_opaque opqConnection,
                                                IN mxt_opaque opqId,
                                                IN mxt_result resCreation);

    //-- << ISipConnectionSvc Interface >>.

    virtual ISipReqCtxConnectionSvc* GetReqCtxConnectionSvcInstance(IN const CSipPacket& rPacket,
                                                                    IN ISipConnectionSvc::EDirection eDirection);

    virtual void ClearConnectionSvc();

    //-- << ISipReqCtxConnectionSvc Interface >>.

    virtual void OnSendSuccess(IN ISipRequestContext& rRequestContext,
                               IN const CSipPacket& rPacketSent);

    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

    //-- << ISipCoreSvc >>

    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                        IN ISipCoreSvc::EDirection eDirection);

    virtual ISipCoreSvc::EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    virtual mxt_result ClearCoreSvc();

    // Helpers

    // Summary:
    //  Inserts a Supported header into the header chain.
    void InsertSupportedHeader(IN const char* pszHeader,
                               INOUT CSipHeader* pHeader);

    // Summary:
    //  Updated the packet with the correct peer address for the specified
    // persistent connection.
    mxt_result UpdatePacketHelper(IN mxt_opaque opqConnection,
                                  INOUT CSipPacket& rPacket);

//-- Hidden Methods
protected:
private:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSipOutboundConnectionSvc(IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipOutboundConnectionSvc();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipOutboundConnectionSvc(IN const CSipOutboundConnectionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipOutboundConnectionSvc& operator=(IN const CSipOutboundConnectionSvc& rSrc);

// Friends declarations

//-- Hidden Data Members
protected:
private:

    // This struct is used as the mxt_opaque passed to ConnectionCreationResult
    // when the application is asked to create a new flow.
    struct SPacketUpdatedParams
    {
        ISipRequestContext* m_pRequestContext;
        CSipPacket* m_pPacket;

        SPacketUpdatedParams(IN ISipRequestContext* pRequestContext,
                             IN CSipPacket* pPacket)
        {
            m_pRequestContext = pRequestContext;
            m_pRequestContext->AddIfRef();

            m_pPacket = pPacket;
            m_pPacket->AddRef();
        }

        ~SPacketUpdatedParams()
        {
            m_pPacket->Release();
            m_pPacket = NULL;

            m_pRequestContext->ReleaseIfRef();
            m_pRequestContext = NULL;
        }

        // Default constructor must be disabled to force users to use the
        // provided constructor.
        private:
        SPacketUpdatedParams(){};
    };

    // Outbound connection manager.
    ISipOutboundConnectionMgr* m_pMgr;

    // Stores the index of the next persistent connection to try.
    unsigned int m_uNextConnectionIndex;

    // Cached list of persistent connections.
    CSipPersistentConnectionList* m_pPersistentConnectionList;

    // Configured list of preferred connections.
    CVector<mxt_opaque> m_vecConnections;

};

//==============================================================================
//==
//==  GetManager
//==
//==============================================================================
//
//  Returns:
//      A pointer to the ISipOutboundConnectionMgr manager. It can be NULL.
//
//  Description:
//      Retrieves the ISipOutboundConnectionMgr manager.
//
//==============================================================================
inline
ISipOutboundConnectionMgr* CSipOutboundConnectionSvc::GetManager()
{
    return m_pMgr;
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPOUTBOUNDCONNECTIONSVC_H

