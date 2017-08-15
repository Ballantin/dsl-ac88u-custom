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
#ifndef MXG_CSIPPRIVACYSVC_H
#define MXG_CSIPPRIVACYSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_PRIVACY_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_PRIVACY_SVC_SUPPORT to be able \
to use this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CSEMAPHORE_H
#include "Kernel/CSemaphore.h" // CBinarySemaphore
#endif


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISIPPRIVACYSVC_H
#include "SipUserAgent/ISipPrivacySvc.h"
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


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipPrivacySvc
//========================================
//
// Description:
//   This class implements the ISipPrivacySvc interface. It provides privacy to
//   a user agent. Note that the application must handle privacy for its From,
//   Call-ID, and Referred-By headers by properly setting them (in the
//   ISipUserAgentSvc for the From and Contact).
//
//   Usage of this service should be coupled with usage of a privacy service
//   provider.
//
//   This service has been implemented following RFC 3323, "A Privacy Mechanism
//   for the Session Initiation Protocol (SIP)".
//
// Location:
//   SipUserAgent/CSipPrivacySvc.h
//
// See Also:
//   ISipPrivacySvc
//
//==EDOC========================================================================
class CSipPrivacySvc : private CEComDelegatingUnknown,
                       private CEventDriven,
                       private ISipPrivacySvc,
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

    static mxt_result InitializeCSipPrivacySvc();

    static void FinalizeCSipPrivacySvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipPrivacySvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipPrivacySvc();


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


    //-- << ISipPrivacySvc >>
    //--------------------------------------------

    // Summary:
    //  Configures the manager associated with this service.
    //-----------------------------------------------------
    virtual mxt_result SetManager(IN ISipPrivacyMgr* pMgr);


    // Summary:
    //  Sets the privacy service provider that should be used to guarantee
    //  privacy for all contexts.
    //---------------------------------------------------------------------
    virtual mxt_result SetSharedPrivacyService(
                                            IN const CSipUri& rPrivacyService);


    // Summary:
    //  Sets a privacy service provider that should only be used to guarantee
    //  privacy for this context.
    //------------------------------------------------------------------------
    virtual mxt_result SetInstancePrivacyService(
                                            IN const CSipUri& rPrivacyService);


    // Summary:
    //  Enables or disables the usage of the Proxy-Require header containing
    //  the 'privacy' option-tag.
    //-----------------------------------------------------------------------
    virtual mxt_result AddProxyRequire(IN bool bAddHeader);


    // Summary:
    //  Sets the privacy types requested to the privacy service provider.
    //--------------------------------------------------------------------
    virtual mxt_result SetPrivacyType(IN unsigned int uPrivacyValues);
    
    // Summary:
    //  Enables the service's header removal.
    //----------------------------------------
    virtual void EnableHeaderRemoval(IN bool bPrivacy);

    //  Summary:
    //      Enables or disables this service.
    virtual void EnableService(IN bool bEnable);

    //-- << ISipCoreSvc >>
    //--------------------------------------------

    // Summary:
    //  Obtain an instance of an ISipReqCtxCoreSvc to attach to a request
    //  context.
    //--------------------------------------------------------------------
    virtual
    ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                IN EDirection eDirection);

    // Summary:
    //  Gives to which degree the service is a good candidate to own a request
    //  context.
    //-------------------------------------------------------------------------
    virtual
    EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    // Summary:
    //  Return the ISipReqCtxCoreSvc instance to use as the owner for a request
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
    //  Informs the request context core service of a received packet.
    //-----------------------------------------------------------------
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    // Summary:
    //  Modify a packet about to be sent.
    //------------------------------------
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    // Summary:
    //  Let the service call an event on the application.
    //----------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    // Summary:
    //  Let the service clear the state associated with an event in which the
    //  application has no interest.
    //-----------------------------------------------------------------------
    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    // Summary:
    //  Add a reference on this request context core service.
    //--------------------------------------------------------
    virtual unsigned int AddRcsRef();

    // Summary:
    //  Remove a reference on this request context core service.
    //-----------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();


    //-- << IServerLocatorMgr >>
    //--------------------------------------------

    // Summary:
    //  Notify the Mgr that resolution of the tree is completed.
    //-----------------------------------------------------------
    virtual void OnAllTargetsResolved(IN TO CList<SNaptrRecord>* plst,
                                      IN mxt_opaque opq);

    // Summary:
    //  Notify the Mgr that resolution is completed.
    //-----------------------------------------------
    virtual void OnTargetResolved(IN TO CList<SNaptrRecord>* plst,
                                  IN mxt_opaque opq);

    // Summary:
    //  Notify the Mgr that resolution is completed.
    //-----------------------------------------------
    virtual void OnTargetResolved(IN TO CList<SSrvRecord>* plst,
                                  IN mxt_opaque opq);

    // Summary:
    //  Notify the Mgr that resolution is completed.
    //-----------------------------------------------
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
    CSipPrivacySvc(IN const CSipPrivacySvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipPrivacySvc& operator=(IN const CSipPrivacySvc& rSrc);


    //-- << Helpers >>
    //--------------------------------------------

    // Summary:
    //  This method returns the privacy service already resolved.
    //------------------------------------------------------------
    CVector<CSocketAddr>& GetCommonPrivacyServices();

    // Summary:
    //  De-marshals the parameters needed for the processing of 
    //  OnAllTargetsResolved.
    void InternalOnAllTargetsResolved(CMarshaler* pParams);

    // Summary:
    //  Does the processing of OnAllTargetsResolved on the core thread.
    void OnAllTargetsResolvedHelper(IN TOA CList<SNaptrRecord>* plst,
                                    IN mxt_opaque opq);

    // Summary:
    //  This method adds a priv-value to the string if the service is
    //  configured to add it.
    //----------------------------------------------------------------
    void AddPrivValueIfEnabled(IN EPrivValue eValue,
                               IN const char* const szValueToAdd,
                               INOUT CString& rstrDestination);

    // Summary:
    //  Searches for the given address in the given vector.
    static bool IsAddressInVector(IN const CSocketAddr& rAddressToSearch,
                                  IN const CVector<CSocketAddr>& rvecToSearchIn);


//-- Hidden Data Members
//------------------------
protected:
private:

    // This is the manager of this service. Events will be called on it.
    //-------------------------------------------------------------------
    ISipPrivacyMgr* m_pMgr;

    // This member tells if a Proxy-Require header with the 'privacy'
    // option-tag must be added to outgoing requests.
    // Default is true.
    //----------------------------------------------------------------
    bool m_bAddProxyRequireHeader;
    
    // This member tells if certain packet headers must be automatically
    // stripped from the outgoing packet.
    //--------------------------------------------------------------------
    bool m_bRemovePrivateHeaders;

    // This contains the privacy header values that will be added to the
    // Privacy header added to the outgoing requests.
    //-------------------------------------------------------------------
    unsigned int m_uPrivacyHeaderValues;

    // This is the instance list. It will be NULL until
    // SetInstancePrivacyService is called on the instance.
    //------------------------------------------------------
    CVector<CSocketAddr>* m_pvecInstancePrivacyServices;

    // This is the object that will be resolving the FQDNs set as privacy
    // service.
    //--------------------------------------------------------------------
    CServerLocator* m_pServerLocator;

    // This tells if ReleaseInstance must be called after a list has been
    // resolved.
    //--------------------------------------------------------------------
    bool m_bMustReleaseInstance;

    // Tells if the shared list is currently resolving.
    //--------------------------------------------------
    static bool ms_bSharedListCurrentlyResolving;

    // The list of sockets associated with the shared privacy service resolved.
    //------------------------------------------------------------------------
    static CVector<CSocketAddr>* ms_pvecCommonPrivacyServices;


    // The service enable flag.
    bool m_bServiceEnable;

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    mxt_opaque m_opqDnsUser;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPPRIVACYSVC_H

