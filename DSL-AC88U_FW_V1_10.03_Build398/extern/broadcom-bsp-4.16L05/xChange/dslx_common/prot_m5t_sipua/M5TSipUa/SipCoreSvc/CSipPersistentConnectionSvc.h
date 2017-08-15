//==============================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_CSIPPERSISTENTCONNECTIONSVC_H
#define MXG_CSIPPERSISTENTCONNECTIONSVC_H

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

#ifndef MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT to be able to use \
this class.
#endif

//-- Data Members
#ifndef MXG_CSIPPERSISTENTCONNECTIONLIST_H
#include "SipCoreSvc/CSipPersistentConnectionList.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#include "SipCore/CSipReqCtxConnectionSvc.h"
#endif

#ifndef MXG_ISIPPERSISTENTCONNECTIONSVC_H
#include "SipCoreSvc/ISipPersistentConnectionSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipPersistentConnectionSvc
//========================================
//
// Description:
//   This class implements the ISipConnectionSvc interface in order to provide
//   access to an CSipReqCtxServerLocationSvc instance in the form of a
//   ISipReqCtxConnectionSvc pointer, to the requesting CSipContext or
//   CSipRequestContext.
//
// Location:
//   SipCore/CSipPersistentConnectionSvc.h
//
// See Also:
//   ISipPersistentConnectionSvc, ISipConnectionSvc, CEComDelegatingUnknown,
//   ISipReqCtxConnectionSvc
//
//==============================================================================
class CSipPersistentConnectionSvc : private CEComDelegatingUnknown,
                                    private ISipConnectionSvc,
                                    private CSipReqCtxConnectionSvc,
                                    private ISipPersistentConnectionSvc
{
//-- Friend Declarations
    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

//-- Published Interface
public:
    //-- << Stereotype >>

    // Summary:
    //  Sets the persistent connections list.
    static void Initialize(IN CSipPersistentConnectionList& rList);

//-- Hidden Methods
private:
    //-- << Startup mechanism >>
    static mxt_result InitializeCSipPersistentConnectionSvc();
    static void FinalizeCSipPersistentConnectionSvc();


    //-- << CEComDelegatingUnknown >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors >>
    CSipPersistentConnectionSvc(IEComUnknown& rOuterIEComUnknown);
    virtual ~CSipPersistentConnectionSvc();


    //-- << CEComDelegatingUnknown Interface >>.

    // The following statement is essential, it provides the default and unique
    // implementation of the IEComUnknown interface that every other interfaces
    // inherit from.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //-- Override CEComDelegatingUnknown
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
    //  Updates rPacket with the resolved target.
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    // Summary:
    //  Destroys this instance of ISipReqCtxConnectionSvc.
    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

#if defined(MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
    //-- << ISipPersistentConnectionSvc Interface >>.
    virtual mxt_result SetSwitchbackPersistentConnection(IN ISipSwitchbackPersistentConnection* pSwitchbackPersistentConnection);

#endif // #if defined(MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)

    // Summary:
    //  Gets a the opaque identifier of the persistent connection that is used for this context.
    mxt_opaque GetConnectionOpaque();

    // Summary:
    // Sets the opaque identifier of the persistent connection to be used for this context.
    void SetConnectionOpaque(IN mxt_opaque opqConnection);

private:
    //-- Deactivated Constructors / Destructors / Operators
    CSipPersistentConnectionSvc(const CSipPersistentConnectionSvc& rSrc);
    CSipPersistentConnectionSvc& operator=(const CSipPersistentConnectionSvc& rSrc);

//-- Hidden Data Members
private:
    static CSipPersistentConnectionList* ms_pPersistentConnectionList;

#if defined(MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
    // The switchback persistent connection.
    ISipSwitchbackPersistentConnection* m_pSwitchbackPersistentConnection;
#endif // #if defined(MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)

    mxt_opaque m_opqConnection;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  ISipPersistentConnectionSvc  ===========================================
//==============================================================================

inline
mxt_opaque CSipPersistentConnectionSvc::GetConnectionOpaque()
{
    return m_opqConnection;
}

inline
void CSipPersistentConnectionSvc::SetConnectionOpaque(IN mxt_opaque opqConnection)
{
    m_opqConnection = opqConnection;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSIPPERSISTENTCONNECTIONSVC_H
