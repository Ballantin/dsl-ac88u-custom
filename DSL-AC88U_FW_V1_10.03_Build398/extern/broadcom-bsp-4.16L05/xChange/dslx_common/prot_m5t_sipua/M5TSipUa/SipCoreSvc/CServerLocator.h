//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CSERVERLOCATOR_H
#define MXG_CSERVERLOCATOR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

// Data Member

// Forward Declarations Outside of the Namespace
#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_NETWORKTOOLS_H
#include "Network/NetworkTools.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_IURI_H
#include "SipParser/IUri.h"
#endif

#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h"
#endif

#ifndef MXG_ISIPCORECONFIG_H
#include "SipCore/ISipCoreConfig.h"
#endif

#ifndef MXG_ISIPTRANSACTIONSTATISTICS_H
#include "SipTransaction/ISipTransactionStatistics.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
#if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)
class CMailboxUri;
#endif
class CMarshaler;
class CSipUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
const char* const szURIPARAM_TRANSPORT_TLS_NAPTR = "SIPS+D2T";
const char* const szURIPARAM_TRANSPORT_TCP_NAPTR = "SIP+D2T";
const char* const szURIPARAM_TRANSPORT_UDP_NAPTR = "SIP+D2U";

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
// Contains the DNS query types supported by SIP.
// The values are bitmaskable except for eNONE.
enum ESipDnsQuery
{
    eNONE = 0,
    eNAPTR = 1,
    eSRV = 2
};

//==SDOC========================================================================
//== Class: IServerLocatorMgr
//========================================
//
// Description:
//   This interface is used by the CServerLocator to notify the calling object
//   that the asynchronous resolution is complete. It also allows the
//   CServerLocator to give back the ownership of the list to its manager.
//
//   The behaviour of the server locator is conformant with RFC 3263, which
//   defines DNS resolution for SIP endpoints.
//
// Location:
//   SipCore/CServerLocator.h
//
// See Also:
//   CServerLocator
//
//==EDOC========================================================================
class IServerLocatorMgr
{
// Published Interface
//---------------------
public:

    //==SDOC====================================================================
    //==
    //==  OnAllTargetsResolved
    //==
    //==========================================================================
    //
    //  Summary
    //      Notifies the manager that resolution of the tree is completed.
    //
    //  Parameters:
    //    lst:
    //      A list of NAPTR records containing the resolution result.
    //
    //    opq:
    //      A mxt_opaque, given previously to the ResolveAllA method of
    //      CServerLocator.
    //
    //  Description:
    //      Notifies the manager that resolution is completed. The ownership of
    //      the list is given back to the manager.
    //
    //==EDOC====================================================================
    virtual void OnAllTargetsResolved(IN TO CList<SNaptrRecord>* plst,
                                      IN mxt_opaque opq) = 0;

    //==SDOC====================================================================
    //==
    //==  OnTargetResolved
    //==
    //==========================================================================
    //
    //  Summary
    //      Notifies the manager that resolution is complete.
    //
    //  Parameters:
    //    lst:
    //      A list of NAPTR records containing the resolution result.
    //
    //    opq:
    //      A mxt_opaque, given previously to the ResolveA method of
    //      CServerLocator.
    //
    //  Description:
    //      Notifies the manager that resolution is complete. The ownership of
    //      the list is given back to the manager.
    //
    //==EDOC====================================================================
    virtual void OnTargetResolved(IN TO CList<SNaptrRecord>* plst,
                                  IN mxt_opaque opq) = 0;
    //==SDOC====================================================================
    //==
    //==  OnTargetResolved
    //==
    //==========================================================================
    //
    //  Summary
    //      Notifies the manager that resolution is complete.
    //
    //  Parameters:
    //    lst:
    //      A list of SRV records containing the resolution result.
    //
    //    opq:
    //      A mxt_opaque, given previously to the ResolveA method of
    //      CServerLocator.
    //
    //  Description:
    //      Notifies the manager that resolution is complete. The ownership of
    //      the list is given back to the manager.
    //
    //==EDOC====================================================================
    virtual void OnTargetResolved(IN TO CList<SSrvRecord>* plst,
                                  IN mxt_opaque opq) = 0;
    //==SDOC====================================================================
    //==
    //==  OnTargetResolved
    //==
    //==========================================================================
    //
    //  Summary
    //      Notifies the manager that resolution is complete.
    //
    //  Parameters:
    //    lst:
    //      A list of CSocketAddrs containing the resolution result.
    //
    //    opq:
    //      A mxt_opaque, given previously to the ResolveA method of
    //      CServerLocator.
    //
    //  Description:
    //      Notifies the manager that resolution is complete. The ownership of
    //      the list is given back to the manager.
    //
    //==EDOC====================================================================
    virtual void OnTargetResolved(IN TO CList<CSocketAddr>* plst,
                                  IN mxt_opaque opq) = 0;


protected:
    // << Constructors / Destructors >>
    //----------------------------------
    IServerLocatorMgr() {}
    virtual ~IServerLocatorMgr() {}
};

//==============================================================================
//== Class: IServerLocatorSimpleMgr
//========================================
//
// Description:
//   This interface is used by the CServerLocator to notify the calling object
//   that the asynchronous resolution is complete. It also allows the
//   CServerLocator to give back the ownership of the list to its manager.
//
//   The behaviour of the server locator complies with with RFC 3263, which
//   defines DNS resolution for SIP endpoints.
//
// Location:
//   SipCore/CServerLocator.h
//
// See Also:
//   CServerLocator
//
//==============================================================================
class IServerLocatorSimpleMgr
{

// New Types.
public:
    // The simple DNS result structure corresponding to a peer.
    struct SSimpleDnsResult
    {
        // The peer socket address.
        CSocketAddr m_socketAddr;

        // The transport associated with this address.
        ESipTransport m_eTransport;
    };
    
// Published Interface.
public:

    //==========================================================================
    //==
    //==  OnAllTargetsResolved
    //==
    //==========================================================================
    //
    //  Summary
    //      Notifies the manager that resolution of the list is completed.
    //
    //  Parameters:
    //    plststResults:
    //      A list of structures containing the resolution result. The results
    //      are added by order of priority and/or as they appear in the DNS
    //      response.
    //
    //    opq:
    //      The opaque given previously to the ResolveUriSimple method of
    //      CServerLocator.
    //
    //  Description:
    //      Notifies the manager that resolution is completed. The ownership of
    //      the list is given to the manager.
    //
    //==========================================================================
    virtual void OnAllTargetsResolved(IN TOA CList<SSimpleDnsResult>* plststResults,
                                      IN mxt_opaque opq) = 0;

protected:
    // << Constructors / Destructors >>
    IServerLocatorSimpleMgr() {}
    virtual ~IServerLocatorSimpleMgr() {}
};

// Mechanism to permit the stubbing of CServerLocator.
//----------------------------------------------------
#if defined(MXD_STUB_CSERVERLOCATOR)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CSERVERLOCATORSTUB_H
    #include "SipCoreSvcStubs/CServerLocatorStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else



class CThreadSwitcher : public CEventDriven,
                        public IServerLocatorMgr
{
private:
    enum
    {
        eMSG_RESOLVE_ALL,
        eMSG_RESOLVE_NAPTR,
        eMSG_RESOLVE_SRV,
        eMSG_RESOLVE_A
    };

    struct SQueryData
    {
        // The simple manager to call back. 
        IServerLocatorSimpleMgr* m_pMgr;

        // The application opaque.
        mxt_opaque m_opq;
    };

// Published Interface
public:
    // << Constructors / Destructors >>

    CThreadSwitcher();

    // << Stereotype >>

    // Summary:
    //  Creates a query data opaque to give to the server locator.
    mxt_opaque CreateQueryData(IN IServerLocatorSimpleMgr* pMgr,
                               IN mxt_opaque opq);

    // Summary:
    //  Releases a query data opaque.
    void ReleaseQueryData(IN mxt_opaque opqQueryData);

    //-- << IServerLocatorMgr Interface >>

    virtual void OnAllTargetsResolved(IN TO CList<SNaptrRecord>* plst,
                                      IN mxt_opaque opq);
    virtual void OnTargetResolved(IN TO CList<SNaptrRecord>* plststNaptrRecords,
                                  IN mxt_opaque opq);
    virtual void OnTargetResolved(IN TO CList<SSrvRecord>* plststSrvRecords,
                                  IN mxt_opaque opq);
    virtual void OnTargetResolved(IN TO CList<CSocketAddr>* plstSocketAddr,
                                  IN mxt_opaque opq);

    // Helpers
    // This one is public because it can be called from the Server Locator when the resolution is synchronous.
    void InternalOnAllTargetsResolved(IN TO CList<SNaptrRecord>* plst,
                                      IN mxt_opaque opq);

// Hidden Methods.
protected:
    virtual ~CThreadSwitcher();

    // Inherited from IMessageServiceMgr.
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    // Helpers
    void InternalOnTargetResolved(IN TO CList<SNaptrRecord>* plststNaptrRecords,
                                  IN mxt_opaque opq);
    void InternalOnTargetResolved(IN TO CList<SSrvRecord>* plststSrvRecords,
                                  IN mxt_opaque opq);
    void InternalOnTargetResolved(IN TO CList<CSocketAddr>* plstSocketAddr,
                                  IN mxt_opaque opq);

private:
    // Deactivated Constructors / Destructors / Operators

    CThreadSwitcher(IN const CThreadSwitcher& rFrom);
    CThreadSwitcher& operator=(IN const CThreadSwitcher& rFrom);

// Hidden Data Members.
protected:
private:
};

//==SDOC========================================================================
//== Class: CServerLocator
//========================================
//
// Description:
//   This class implements RFC 3263 to resolve targets from URIs, and also adds
//   an asynchronous interface to query the resolver. The asynchronous query
//   always produces a list that may be empty.
//
// Location:
//   SipCore/CServerLocator.h
//
// See Also:
//   IServerLocatorMgr, CEventDriven, NetworkTools
//
//==EDOC========================================================================
class CServerLocator : public CEventDriven
{
    enum
    {
        eMSG_RESOLVE_ALL_URI,
        eMSG_RESOLVE_URI,
        eMSG_RESOLVE_NAPTR,
        eMSG_RESOLVE_SRV1,
        eMSG_RESOLVE_SRV2,
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
        eMSG_SET_DNS_USER_OPQ,
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
        eMSG_RESOLVE_HOST_NAME
    };

//-- Friend Declarations
//------------------------

    friend class CSipStackInitializer;


// Published Interface
//-------------------------
public:
    // << Constructors / Destructors >>
    //--------------------------------------------

    CServerLocator(IN ISipTransactionStatistics* pTransStats = NULL);

    // << Stereotype >>
    //--------------------------------------------

    // Summary:
    //  Asynchronously resolves a URI by using RFC 3263.
    //---------------------------------------------------
    mxt_result ResolveAllA(IN TO IUri* pUri,
                           IN TO CList<SNaptrRecord>* plstRecord,
                           IN IServerLocatorMgr* pMgr,
                           IN mxt_opaque opq);

    // Summary:
    //  Asynchronously resolves a URI by using RFC 3263.
    //---------------------------------------------------
    mxt_result ResolveA(IN TO CSipUri* pSipUri,
                        IN TO CList<SNaptrRecord>* plstRecord,
                        IN IServerLocatorMgr* pMgr,
                        IN mxt_opaque opq);

    // Summary:
    //  Asynchronously resolves a domain by using NAPTR query.
    //---------------------------------------------------------
    mxt_result ResolveA(IN const CString* pstrQuery,
                        IN TO CList<SNaptrRecord>* plstRecord,
                        IN IServerLocatorMgr* pMgr,
                        IN mxt_opaque opq);

    // Summary:
    //  Asynchronously resolves a service-protocol-domain trio by using SRV
    //  query.
    //----------------------------------------------------------------------
    mxt_result ResolveA(IN const CString* pstrService,
                        IN const CString* pstrProtocol,
                        IN const CString* pstrDomain,
                        IN TO CList<SSrvRecord>* plstRecord,
                        IN IServerLocatorMgr* pMgr,
                        IN mxt_opaque opq);

    // Summary:
    //  Asynchronously resolves a service-protocol-domain constructed query
    //  string by using SRV query.
    //----------------------------------------------------------------------
    mxt_result ResolveA(IN const CString* pstrQuery,
                        IN TO CList<SSrvRecord>* plstRecord,
                        IN IServerLocatorMgr* pMgr,
                        IN mxt_opaque opq);

    // Summary:
    //  Asynchronously resolves a FQDN by using A/AAAA query.
    //--------------------------------------------------------
    mxt_result ResolveA(IN const CString* pstrQuery,
                        IN TO CList<CSocketAddr>* plstRecord,
                        IN IServerLocatorMgr* pMgr,
                        IN mxt_opaque opq);

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    void SetDnsUserOpaqueA(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

    // Summary:
    //  Sets the supported SIP transport.
    //------------------------------------
    static void SetSupportedSipTransport(IN int nTransportBitset);

    // Summary:
    //  Sets the type(s) of DNS queries supported by the application.
    //----------------------------------------------------------------
    static void SetSupportedDnsQueries(IN int nDnsQueryBitset);

    // Summary:
    //  Sets the failover mode describing what needs to be done when sending
    //  fails.
    //-----------------------------------------------------------------------
    static void SetFailoverMode(IN ISipCoreConfig::EFailoverMode eFailoverMode);

    // Summary:
    // Gets the supported IP versions.
    //---------------------------------
    static ISipCoreConfig::EIpVersionConfig GetSupportedIpVersion();

    // Sets the supported IP versions.
    //---------------------------------
    static void SetSupportedIpVersion(IN ISipCoreConfig::EIpVersionConfig eIpVersion);

    // Summary:
    //  Synchronously verifies if the URI is a valid IP address
    //  and asynchronously resolves it otherwise.
    mxt_result ResolveUri(IN TOA IUri* pUri,
                          IN TOA CList<SNaptrRecord>* plstRecord,
                          IN IServerLocatorMgr* pMgr,
                          IN mxt_opaque opq,
                          IN bool bResolveAll,
                          OUT GO CList<SNaptrRecord>*& rplstRecord);

    // Summary:
    //  Synchronously verifies if the URI is a valid IP address
    //  and asynchronously resolves it otherwise.
    mxt_result ResolveUriSimple(IN TOA IUri* pUri,
                                IN IServerLocatorSimpleMgr* pMgr,
                                IN mxt_opaque opq);
    
// Hidden Methods
//-------------------------
protected:
    virtual ~CServerLocator();

    // Inherited from IMessageServiceMgr.
    //------------------------------------
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

#if defined(MXD_TRACE8_ENABLE_SUPPORT)
    // Summary:
    //  Traces an NAPTR record list contents.
    //----------------------------------------
    void TraceNaptrRecord(IN CList<SNaptrRecord>& rlststRecord) const;

    // Summary:
    //  Traces an SRV record list contents.
    //--------------------------------------
    void TraceSrvRecord(IN CList<SSrvRecord>& rplststRecord,
                        IN bool bSuppressEntryTrace = false) const;

    // Summary:
    //  Traces a CSocketAddr record list contents.
    //---------------------------------------------
    void TraceCSocketAddrRecord(IN CList<CSocketAddr>& rplstRecord,
                                IN bool bSuppressEntryTrace = false) const;

#endif // #if defined(MXD_TRACE8_ENABLE_SUPPORT)


private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCServerLocator();


    //-- << Helper methods >>
    //--------------------------------------------

    mxt_result ResolveAll(IN const IUri& rUri,
                          OUT CList<SNaptrRecord>& rlststNaptr);
    mxt_result Resolve(IN const CSipUri& rSipUri,
                       OUT CList<SNaptrRecord>& rlstRecord);
#if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)
    mxt_result Resolve(IN const CMailboxUri& rMailboxUri,
                       OUT CList<SNaptrRecord>& rlstRecord);
#endif
    mxt_result Resolve(IN const CString& rstrQuery,
                       OUT CList<SNaptrRecord>& rlstRecord);
    mxt_result Resolve(IN const CString& rstrService,
                       IN const CString& rstrProtocol,
                       IN const CString& rstrDomain,
                       OUT CList<SSrvRecord>& rlstRecord);
    mxt_result Resolve(IN const CString& rstrQuery,
                       OUT CList<SSrvRecord>& rlstRecord);
    mxt_result Resolve(IN const CString& rstrQuery,
                       OUT CList<CSocketAddr>& rlstRecord);
    // Summary:
    //  Private method to generate a SRV record containing phony information.
    //------------------------------------------------------------------------
    void GeneratePhonySrvRecord(IN const CString& rstrTarget,
                                IN uint16_t uPort,
                                OUT SSrvRecord& rstSrvRecord);
    // Summary:
    //  Private method to generate a NAPTR record containing phony information.
    //--------------------------------------------------------------------------
    void GeneratePhonyNaptrRecord(IN IUri::EUriType eType,
                                  IN const CString& rstrDomain,
                                  IN const CString& rstrTransport,
                                  OUT SNaptrRecord& rstNaptrRecord);

    // Summary:
    //  Private method to generate a NAPTR record and resolve the SRV record.
    //------------------------------------------------------------------------
    void GenerateNaptrAndResolveSrv(IN IUri::EUriType eType,
                                    IN const CString& rstrDomain,
                                    IN const char* pszTransport,
                                    OUT SNaptrRecord& rstNaptrRecord,
                                    OUT CList<SNaptrRecord>& rlststRecord);

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    CServerLocator(IN const CServerLocator& rFrom);
    CServerLocator& operator=(IN const CServerLocator& rFrom);

    // Summary:
    //   Notifies the statistics container of an event.
    //--------------------------------------------------
    void UpdateDnsQueryStats(IN bool bRes);

    // Summary:
    //   Verifies if the transport is supported.
    //---------------------------------------------------------
    bool IsTransportSupported(IN bool bIsSecured, IN const CString& rstrTransport);

    // Summary:
    //   Generates a full NAPTR record with an embedded SRV record.
    //---------------------------------------------------------------
    void SimulateNaptrRecord(IN IUri::EUriType eType,
                             IN const CString& rstrDomain,
                             IN uint16_t uPort,
                             IN const CString& rstrTransport,
                             IN const CList<CSocketAddr>& lstSocketAddr,
                             INOUT CList<SNaptrRecord>& rlststRecord);

    // Summary:
    //  Extract the service and protocol part from the SRV query.
    mxt_result ParseSrvQuery(IN const CString& rstrQuery,
                             OUT CString& rstrService,
                             OUT CString& rstrProtocol);

// Hidden Data Members
//-------------------------
protected:
private:
    // Contains a bitmap of ESipTransport.
    //-------------------------------------
    static int ms_nSupportedTransport;

    // Contains a bitmap of ESipDnsQuery.
    //------------------------------------
    static int ms_nSupportedDnsQueries;

    // The failover mode.
    //--------------------
    static ISipCoreConfig::EFailoverMode ms_eFailoverMode;

    // Contains an EIpVersionConfig value.
    //-------------------------------------
    static ISipCoreConfig::EIpVersionConfig ms_eSupportedIPVersion;

    // Pointer to the statistics container.
    //--------------------------------------
    ISipTransactionStatistics* m_pTransStats;

    // The instance of the thread switcher class.
    CThreadSwitcher* m_pThreadSwitcher;

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    mxt_opaque m_opqDnsUser;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC====================================================================
//==
//==  GetSupportedIpVersion
//==
//==========================================================================
//
//  Returns:
//      The version(s) that are currently supported.
//
//  Description:
//      This gets the supported IP version.
//
//  See Also:
//      EIpVersionConfig, ISipCoreConfig
//
//==EDOC====================================================================
inline ISipCoreConfig::EIpVersionConfig CServerLocator::GetSupportedIpVersion()
{
    return ms_eSupportedIPVersion;
}

//==SDOC========================================================================
//==
//==  UpdateDnsQueryStats
//==
//==============================================================================
//
//  Parameters:
//    bRes:
//      Indicates whether the DNS query failed or succeeded.
//
//  Description:
//     Notifies the configured statistics container that a DNS query has been 
//     done.
//
//==EDOC========================================================================
inline void CServerLocator::UpdateDnsQueryStats(IN bool bRes)
{
    // Notify the statistics container.
    //----------------------------------
    if (m_pTransStats != NULL)
    {
        m_pTransStats->NotifyDnsQueryResult(bRes);
    }
}

#endif // #if defined(MXD_STUB_CSERVERLOCATOR)

MX_NAMESPACE_END(MXD_GNS)


#endif // #ifndef MXG_CSERVERLOCATOR_H
//M5T_INTERNAL_USE_END

