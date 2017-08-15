//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPENUMSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPENUMSVC_H
//M5T_INTERNAL_USE_END

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

#ifndef MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT to be able to use this class.
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPENUMREQUESTHANDLERMGR_H
#include "SipCoreSvc/ISipEnumRequestHandlerMgr.h"
#endif

#ifndef MXG_ISIPENUMSVC_H
#include "SipCoreSvc/ISipEnumSvc.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipEnumSvc
//========================================
//
// Description:
//   This class is the implementation of the ISipEnumSvc interface. The purpose
//   of this service is to update outgoing SIP requests that hold a TEL request
//   URI with the SIP-URI obtained from a RFC 3761 ENUM query through a DNS
//   server. The service may also be configured to remove the Route header
//   and/or update the To header with the resulting ENUM SIP-URI.
//
// Location:
//   SipCoreSvc/CSipEnumSvc.h
//
// See Also:
//   CSipEnumRequestHandler
//
//==EDOC========================================================================
class CSipEnumSvc : private CEventDriven,
                    private CEComDelegatingUnknown,
                    private ISipCoreSvc,
                    private CSipReqCtxCoreSvc,
                    private ISipEnumRequestHandlerMgr,
                    private ISipEnumSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

//-- New types within class namespace
//-----------------------------------
public:
protected:
    enum
    {
        eON_GET_ENUM_URI_A_RESULT
    };

    struct SRequestData
    {
        ISipRequestContext* m_pRequestContext;
        CSipPacket*         m_pPacket;
    };

private:

//-- Hidden Methods
//-------------------
protected:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipEnumSvc();

    static void FinalizeCSipEnumSvc();

    //-- << ECOM mechanism >>
    //-------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor. Used through the factory method local to this
    //  class.
    //---------------------------------------------------------------------
    CSipEnumSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipEnumSvc();

    //-- << ISipEnumSvc method >>
    //-----------------------------

    // Summary:
    //  Configures the service behaviour.
    //------------------------------------
    virtual void SetServiceBehaviour(IN int nEnumSvcBehaviourBitset);

    //-- << ISipCoreSvc methods >>
    //------------------------------

    //  Summary:
    //   Obtains an instance of an ISipReqCtxCoreSvc to attach to a request
    //   context.
    //----------------------------------------------------------------------
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                               IN const CSipPacket& rPacket,
                                               IN EDirection        eDirection);

    // Summary:
    //  Gives the degree to which the service is a good candidate to own a
    //  request context.
    //---------------------------------------------------------------------
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                               IN const CSipPacket& rPacket);

    // Summary:
    //  Returns the ISipReqCtxCoreSvc instance to use as the owner for a
    //  request context.
    //-------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();

    //-- << ISipReqCtxCoreSvc methods >>
    //------------------------------------

    // Summary:
    //  Notifies the request context core service of a received packet.
    //------------------------------------------------------------------
    virtual void OnPacketReceived(IN    ISipRequestContext& rRequestContext,
                                  IN    const CSipPacket&   rPacket,
                                  INOUT CSipCoreEventList&  rEventList);

    // Summary:
    //  Modifies a packet about to be sent.
    //--------------------------------------
    virtual mxt_result UpdatePacket(IN    ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    // Summary:
    //  Adds a reference on this request context core service.
    //---------------------------------------------------------
    virtual unsigned int AddRcsRef();

    // Summary:
    //  Removes a reference on this request context core service.
    //------------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();

    //-- << ISipEnumRequestHandlerMgr methods >>
    //--------------------------------------------

    // Summary:
    //  Reports the GetEnumUriA result.
    //----------------------------------
    virtual void OnGetEnumUriAResult(IN TO CList<SEnumUri>* plstEnumUri,
                                     IN    mxt_opaque       opq,
                                     IN    mxt_result       res);

    //-- << ECOM mechanism >>
    //-------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- << CEventDriven >>
    //-----------------------

    //-- << Inherited from IMessageServiceMgr >>
    //--------------------------------------------
    virtual void EvMessageServiceMgrAwaken(IN bool         bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler*  pParameter);

    //-- << Messaging methods >>
    //----------------------------
    void InternalOnGetEnumUriAResult(IN CMarshaler* pParameter);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipEnumSvc(IN const CSipEnumSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipEnumSvc& operator=(IN const CSipEnumSvc& rSrc);

//-- Hidden Data Members
//------------------------
protected:
    // The eEnumServiceBehaviour bitset.
    //-----------------------------------
    int m_nEnumSvcBehaviourBitset;

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    mxt_opaque m_opqDnsUser;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPENUMSVC_H

