//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==EDOC========================================================================
#ifndef MXG_CSIPUAASSERTEDIDENTITYSVC_H
#define MXG_CSIPUAASSERTEDIDENTITYSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_UA_ASSERTED_IDENTITY_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_UA_ASSERTED_IDENTITY_SVC_SUPPORT to be able to use \
this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_CUNCMP_H
//-- Used to render the CSocketAddr uncomparable type comparable in a CVector.
#include "Cap/CUncmp.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_METHOD_H
#include "SipParser/Method.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CSEMAPHORE_H
#include "Kernel/CSemaphore.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISIPUAASSERTEDIDENTITYSVC_H
#include "SipUserAgent/ISipUaAssertedIdentitySvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_CSERVERLOCATOR_H
#include "SipCoreSvc/CServerLocator.h" // IServerLocatorMgr
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CServerLocator;
class CSipHeader;
class ISipUaAssertedIdentityMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipUaAssertedIdentitySvc
//========================================
//<FLAG Updated Behavior as of 4.1.8>
//
// Description:
//   This class implements the ISipUaAssertedIdentitySvc interface. It provides
//   Network Asserted Identity management to a user agent.
//
//   This service should be the only one that adds P-Preferred-Identity headers
//   to packets. The service will remove the P-Preferred-Identity headers it has
//   not inserted into the packet.
//
//   This service is implemented following the RFC 3325, "Private Extensions to
//   the Session Initiation Protocol (SIP) for Asserted Identity within Trusted
//   Networks".
//
//   This service has been updated to follow draft-ietf-sipping-update-pai-09.
//   As per this draft, all methods except ACK and CANCEL are allowed to have
//   either a P-Preferred-Identity or a P-Asserted-Identity header. Even though
//   it is now possible, as per the draft, to add the P-Asserted-Identity header
//   in requests as well, the stack will not add it on its own, the user must do
//   it. As per the draft, only one asserted identity header
//   (P-Asserted-Identity or P-Preferred-Identity) can be present per request
//   sent. It is up to the user to respect this.
//
// Location:
//   SipUserAgent/CSipUaAssertedIdentitySvc.h
//
//==EDOC========================================================================
class CSipUaAssertedIdentitySvc : private CEComDelegatingUnknown,
                                  private CEventDriven,
                                  private ISipUaAssertedIdentitySvc,
                                  private ISipCoreSvc,
                                  private CSipReqCtxCoreSvc,
                                  private IServerLocatorMgr
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Hidden Methods
//-------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipUaAssertedIdentitySvc();

    static void FinalizeCSipUaAssertedIdentitySvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipUaAssertedIdentitySvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipUaAssertedIdentitySvc();


    //-- << CEComUnknown >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << CEventDriven >>
    //--------------------------------------------

    // Summary:
    //  Process message queue.
    //-------------------------
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    virtual void ReleaseInstance();


    //-- << ISipUaAssertedIdentitySvc >>
    //--------------------------------------------

    // Summary:
    //  Configures the manager associated with this service.
    //-----------------------------------------------------
    virtual mxt_result SetManager( IN ISipUaAssertedIdentityMgr* pMgr );

    // Summary:
    //  Sets the trusted proxy for all contexts containing the
    //  ISipUaAssertedIdentitySvc.
    //---------------------------------------------------------
    virtual mxt_result SetSharedTrustedProxy(IN const CSipUri& rTrustedProxy);


    //  Summary:
    //      Sets the address of the trusted proxy servers that will be used by
    //      all instances of this service.
    //-------------------------------------------------------------------------
    virtual mxt_result SetSharedTrustedProxy(IN const CVector<CSipUri>& rvecTrustedProxies);

    // Summary:
    //  Sets the trusted proxy for this context only.
    //-----------------------------------------------
    virtual mxt_result SetInstanceTrustedProxy(IN const CSipUri& rTrustedProxy);

    //  Summary:
    //      Sets the address of the trusted proxy servers that will be used by
    //      this instance of this service.
    //--------------------------------------------------------------------------
    virtual mxt_result SetInstanceTrustedProxy(IN const CVector<CSipUri>& rvecTrustedProxies);

    // Summary:
    //  Sets the identities that are added to the P-Preferred-Identity of
    //  the outgoing requests.
    //-----------------------------------------------------------------------
    virtual  mxt_result SetPreferredIdentities(IN const CNameAddr* pSipIdentity,
                                               IN const CNameAddr* pTelIdentity);

    virtual void IgnoreUnknownUri(IN bool bEnable);

    //-- << ISipCoreSvc >>
    //--------------------------------------------

    // Summary:
    //  Obtains an instance of an ISipReqCtxCoreSvc to attach to a request
    //  context.
    //--------------------------------------------------------------------
    virtual
    ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                IN EDirection eDirection);

    // Summary:
    //  Establishes the confidence level for which the service is a good
    //  candidate to own a request context.
    //-------------------------------------------------------------------
    virtual
    EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    // Summary:
    //  Returns the ISipReqCtxCoreSvc instance to use as the owner for a request
    //  context.
    //--------------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();


    //-- << ISipReqCtxCoreSvc >>
    //--------------------------------------------

    // Summary:
    //  Notifies the request context core service of a received packet.
    //-----------------------------------------------------------------
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    // Summary:
    //  Modifies a packet about to be sent.
    //------------------------------------
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    // Summary:
    //  Lets the service call an event on the application.
    //----------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    // Summary:
    //  Lets the service clear the state associated with an event in which the
    //  application has no interest.
    //------------------------------------------------------------------------
    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    // Summary:
    //  Adds a reference on this request context core service.
    //--------------------------------------------------------
    virtual unsigned int AddRcsRef();

    // Summary:
    //  Removes a reference from this request context core service.
    //-----------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();


    //-- << IServerLocatorMgr >>
    //--------------------------------------------

    // Summary:
    //  Notifies the manager that resolution of the tree is completed.
    //----------------------------------------------------------------
    virtual void OnAllTargetsResolved(IN TO CList<SNaptrRecord>* plst,
                                      IN mxt_opaque opq);

    // Summary:
    //  Notifies the manager that resolution is completed.
    //----------------------------------------------------
    virtual void OnTargetResolved(IN TO CList<SNaptrRecord>* plst,
                                  IN mxt_opaque opq);

    // Summary:
    //  Notifies the manager that resolution is completed.
    //----------------------------------------------------
    virtual void OnTargetResolved(IN TO CList<SSrvRecord>* plst,
                                  IN mxt_opaque opq);

    // Summary:
    //  Notifies the manager that resolution is completed.
    //----------------------------------------------------
    virtual void OnTargetResolved(IN TO CList<CSocketAddr>* plst,
                                  IN mxt_opaque opq);


//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipUaAssertedIdentitySvc(IN const CSipUaAssertedIdentitySvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipUaAssertedIdentitySvc& operator=(
                                    IN const CSipUaAssertedIdentitySvc& rSrc);


    //-- << Helpers >>
    //--------------------------------------------

    // Summary:
    //  Tells whether or not the P-Preferred-Identity can be added to the
    //  request.
    //-----------------------------------------------------------------
    bool IsPPreferredIdentityApplicable( ESipMethod eMethod );

    // Summary:
    //  Gets the trusted proxy IP address list shared by all the services.
    //--------------------------------------------------------------------
    CVector< CUncmp<CSocketAddr> >& GetCommonTrustedProxies();

    // Summary:
    //  Calls the P-Asserted-Identity header event on the manager.
    //------------------------------------------------------------
    void CallPAssertedIdentityEvent( IN const CSipPacket& rPacket,
                                     IN const CSipHeader& rPAssertedIdentity,
                                     IN ISipRequestContext& rRequestContext);

    // Summary:
    //  De-marshals the parameters needed for the processing of
    //  OnAllTargetsResolved.
    void InternalOnAllTargetsResolved(CMarshaler* pParams);

    // Summary:
    //  This method is used by the CAP data types to compare CSocketAddr objects.
    //----------------------------------------------------------------------------
    static int CompareCSocketAddr(IN const CUncmp<CSocketAddr>& rOneElement,
                                  IN const CUncmp<CSocketAddr>& rOtherElement,
                                  IN mxt_opaque opq);

    // Summary:
    //  Does the processing of OnAllTargetsResolved on the core thread.
    void OnAllTargetsResolvedHelper(IN TOA CList<SNaptrRecord>* plst,
                                    IN mxt_opaque opq);

//-- Hidden Data Members
//------------------------
protected:
private:

    // The manager of this service.
    //------------------------------
    ISipUaAssertedIdentityMgr* m_pMgr;

    // The SIP or SIPS URI identity that must be inserted in the
    // P-Preferred-Identity header.
    //-----------------------------------------------------------
    CNameAddr* m_pSipIdentity;

    // The telephone URI identity that must be inserted in the
    // P-Preferred-Identity header.
    //---------------------------------------------------------
    CNameAddr* m_pTelIdentity;

    // This is the list that will point to instance list. Will be NULL until
    // SetInstanceTrustedProxy is called on the instance. The CUncmp wrapper
    // class is used so the CSocketAddr becomes a comparable type.
    //-----------------------------------------------------------------------
    CVector< CUncmp<CSocketAddr> >* m_pvecInstanceTrustedProxies;

    // This is the temporary list of the instance list. It will be NULL unless
    // there is a DNS resolution for the instance trusted proxie in progress.
    // The CUncmp wrapper class is used so the CSocketAddr becomes a comparable
    // type.
    //--------------------------------------------------------------------------
    CVector< CUncmp<CSocketAddr> >* m_pvecTemporaryInstanceTrustedProxies;

    // This will count the number of DNS request that are in progress for the
    // instance list.
    //------------------------------------------------------------------------
    unsigned int m_uInstanceDnsRequestCount;

    // This is the object that will be resolving the FQDNs set as trusted
    // proxy.
    //--------------------------------------------------------------------
    CServerLocator* m_pServerLocator;

    // This tells if ReleaseInstance must be called after a list has been
    // resolved.
    //--------------------------------------------------------------------
    bool m_bMustReleaseInstance;

    // Tells if the shared list is currently resolving.
    //--------------------------------------------------
    static bool ms_bSharedListCurrentlyResolving;

    // This will count the number of DNS request that are in progress for the
    // common list.
    //------------------------------------------------------------------------
    static unsigned int ms_uCommonDnsRequestCount;

    // The list of socket associated to the shared trusted proxy resolved. The
    // CUncmp wrapper class is used so the CSocketAddr becomes a comparable
    // type.
    //-------------------------------------------------------------------------
    static CVector< CUncmp<CSocketAddr> >* ms_pvecCommonTrustedProxies;

    // Implements uri processing as specified in RFC5876.
    bool m_bEnableAlternativeUriProcess;
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    mxt_opaque m_opqDnsUser;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  IsPPreferredIdentityApplicable
//==
//==============================================================================
//<FLAG Updated Behavior as of 4.1.8>
//
//  Parameters:
//    eMethod:
//      The method of the request.
//
//  Returns:
//      true if the P-Preferred-Identity header can be added to the request as
//      per draft-ietf-sipping-update-pai-09. This overrides the behavior of
//      RFC 3325.
//
//      false if the P-Preferred-Identity header is not applicable to the method
//      of the request.
//
//  Description:
//      This method tells if the P-Preferred-Identity should be added to the
//      request containing the method.
//
//      From draft-ietf-sipping-update-pai-09 :
//      <code>
//      Header field         where   proxy   ACK  BYE  CAN  INV  OPT  REG  PUB
//      ------------         -----   -----   ---  ---  ---  ---  ---  ---  ---
//      P-Preferred-Identity          adr     -    o    -    o    o    o    o
//
//                                           SUB  NOT  REF  INF  UPD  PRA  MES
//                                           ---  ---  ---  ---  ---  ---  ---
//                                            o    o    o    o    o    o    o
//      </code>
//
//      Note that this method is also applicable for the P-Asserted-Identity
//      header, that has the same application as per
//      draft-ietf-sipping-update-pai-09 :
//      <code>
//      Header field         where   proxy   ACK  BYE  CAN  INV  OPT  REG  PUB
//      ------------         -----   -----   ---  ---  ---  ---  ---  ---  ---
//      P-Asserted-Identity           adr     -    o    -    o    o    o    o
//
//                                           SUB  NOT  REF  INF  UPD  PRA  MES
//                                           ---  ---  ---  ---  ---  ---  ---
//                                            o    o    o    o    o    o    o
//      </code>
//
//  #Notes:#
//      This method checks against the methods that are NOT
//      PPreferredIdentityApplicable according to
//      draft-ietf-sipping-update-pai-09. As per the draft, this means that all
//      other methods are by default applicable for P-Preferred-Identity or
//      P-Asserted-Identity.
//
//==EDOC========================================================================
inline bool CSipUaAssertedIdentitySvc::IsPPreferredIdentityApplicable(ESipMethod eMethod)
{
    // From RFC 3325 :
    // "Header field         where   proxy   ACK  BYE  CAN  INV  OPT  REG  PUB
    //  ------------         -----   -----   ---  ---  ---  ---  ---  ---  ---
    //  P-Asserted-Identity           adr     -    o    -    o    o    o    o
    //
    //                                       SUB  NOT  REF  INF  UPD  PRA  MES
    //                                       ---  ---  ---  ---  ---  ---  ---
    //                                        o    o    o    o    o    o    o"
    return eMethod != eSIP_METHOD_ACK &&
           eMethod != eSIP_METHOD_CANCEL;
}

inline void CSipUaAssertedIdentitySvc::IgnoreUnknownUri(IN bool bEnable)
{
    m_bEnableAlternativeUriProcess = bEnable;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPUAASSERTEDIDENTITYSVC_H

