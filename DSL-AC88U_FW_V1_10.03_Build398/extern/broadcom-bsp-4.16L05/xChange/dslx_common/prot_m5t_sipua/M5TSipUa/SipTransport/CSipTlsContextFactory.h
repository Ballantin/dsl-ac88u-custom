//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_CSIPTLSCONTEXTFACTORY_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPTLSCONTEXTFACTORY_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h" // MXD_SIPSTACK_ENABLE_TLS
#endif

#if defined(MXD_SIPSTACK_ENABLE_TLS)

//-- Interface Realized and/or Parent.
#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISIPTLSCONTEXTFACTORY_H
#include "SipTransport/ISipTlsContextFactory.h"
#endif

//-- Data Members.
#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CTLSCONTEXT_H
#include "Tls/CTlsContext.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipTlsContextFactory
//==============================================================================
//
// Description:
//   This is the implementation of the ISipTlsContextFactory interface. Please
//   refer to the ISipTlsContextFactory documentation for more information on
//   how to use the interface and the object that implements it.
//
// Location:
//   SipTransport/CSipTlsContextFactory.h
//
// See Also:
//   ISipTlsContextFactory, CTlsContext, ISipCoreConfig
//
//==============================================================================
class CSipTlsContextFactory : private ISipTlsContextFactory,
                              private CEComUnknown,
                              private CEventDriven
{
//-- Friend Declarations
    friend class CSipStackInitializer;

//-- New types within class namespace
private:
    enum ETlsContextSocketAction
    {
        eSET_DEFAULT_CLIENT_CONTEXT,
        eSET_DEFAULT_SERVER_CONTEXT,
        eADD_CLIENT_CONTEXT,
        eADD_SERVER_CONTEXT,
        eUPDATE_CLIENT_CONTEXT,
        eUPDATE_SERVER_CONTEXT,
        eREMOVE_CLIENT_CONTEXT,
        eREMOVE_SERVER_CONTEXT
    };

//-- Hidden Methods
private:
    //-- << Constructors / Destructors / Operators >>
    CSipTlsContextFactory();
    virtual ~CSipTlsContextFactory();
    CSipTlsContextFactory(IN const CSipTlsContextFactory& rSrc);
    CSipTlsContextFactory& operator=(IN const CSipTlsContextFactory& rSrc);

    //-- Inherited from ISipTlsContextFactory
    mxt_result SetDefaultTlsClientContextS(IN const CTlsContext& rTlsContext);
    mxt_result SetDefaultTlsServerContextS(IN const CTlsContext& rTlsContext);
    mxt_result AddTlsClientContextS(IN const CString& rstrHostname,
                                    IN const CTlsContext& rTlsContext);
    mxt_result AddTlsServerContextS(IN const CSocketAddr& rLocalAddr,
                                    IN const CTlsContext& rTlsContext);
    mxt_result UpdateTlsClientContextS(IN const CString& rstrHostname,
                                       IN const CTlsContext& rTlsContext);
    mxt_result UpdateTlsServerContextS(IN const CSocketAddr& rLocalAddr,
                                       IN const CTlsContext& rTlsContext);
    mxt_result RemoveTlsClientContextS(IN const CString& rstrHostname);
    mxt_result RemoveTlsServerContextS(IN const CSocketAddr& rLocalAddr);
    mxt_result GetDefaultTlsClientContextS(OUT CTlsContext& rTlsContext) const;
    mxt_result GetDefaultTlsServerContextS(OUT CTlsContext& rTlsContext) const;
    mxt_result GetTlsClientContextS(IN const CString& rstrHostname,
                                    OUT CTlsContext& rTlsContext) const;
    mxt_result GetTlsServerContextS(IN const CSocketAddr& rAddr,
                                    OUT CTlsContext& rTlsContext) const;

    //-- Inherited from CEventDriven
    void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                   IN unsigned int uMessage,
                                   IN CMarshaler* pParameter);

    //-- << ECOM mechanism >>.
    static mxt_result InitializeCSipTlsContextFactory();
    static void FinalizeCSipTlsContextFactory();
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComUnknown >>
    MX_DECLARE_IECOMUNKNOWN
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- Helper methods.
    void InternalSetDefaultTlsClientContextS(IN CMarshaler* pParameter);
    void InternalSetDefaultTlsServerContextS(IN CMarshaler* pParameter);
    void InternalAddTlsClientContextS(IN CMarshaler* pParameter);
    void InternalAddTlsServerContextS(IN CMarshaler* pParameter);
    void InternalUpdateTlsClientContextS(IN CMarshaler* pParameter);
    void InternalUpdateTlsServerContextS(IN CMarshaler* pParameter);
    void InternalRemoveTlsClientContextS(IN CMarshaler* pParameter);
    void InternalRemoveTlsServerContextS(IN CMarshaler* pParameter);
    void InternalGetDefaultTlsClientContextS(IN CMarshaler* pParameter);
    void InternalGetDefaultTlsServerContextS(IN CMarshaler* pParameter);
    void InternalGetTlsClientContextS(IN CMarshaler* pParameter);
    void InternalGetTlsServerContextS(IN CMarshaler* pParameter);
    void SetTlsContextInExistingSockets(IN const CTlsContext& rTlsContext,
                                        IN ETlsContextSocketAction eAction,
                                        IN const CSocketAddr& rAddress = CSocketAddr::eINET_ANY,
                                        IN const CString& rStrHostname = "");

//-- Hidden Data Members
private:
    // The default client context.
    CTlsContext m_clientContext;

    // The default server context.
    CTlsContext m_serverContext;

    // The list of client contexts regrouped by host name.
    //-- Some compilers do not support well default instances
    //-- of template arguments. Explicitly tell the type of container
    //-- used by the CMap.
    CMap<CString,
         CTlsContext*,
         CAATree<CMapPair<CString, CTlsContext*> >
        > m_mapClientContexts;

    // The list of server contexts regrouped by local IP address.
    //-- A CString is used to store the IP address since the < and > operators
    //-- are unimplemented in CSocketAddr.
    CMap<CString,
         CTlsContext*,
         CAATree<CMapPair<CString, CTlsContext*> >
        > m_mapServerContexts;
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- defined(MXD_SIPSTACK_ENABLE_TLS)

#endif //-- #ifndef MXG_CSIPTLSCONTEXTFACTORY_H
//M5T_INTERNAL_USE_END
