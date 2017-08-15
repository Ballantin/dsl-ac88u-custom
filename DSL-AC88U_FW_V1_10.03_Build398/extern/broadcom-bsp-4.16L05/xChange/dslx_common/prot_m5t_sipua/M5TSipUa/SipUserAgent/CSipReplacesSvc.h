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
#ifndef MXG_CSIPREPLACESSVC_H
#define MXG_CSIPREPLACESSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT to be able \
to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPREPLACESSVC_H
#include "SipUserAgent/ISipReplacesSvc.h"
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
//== Class: CSipReplacesSvc
//========================================
//
// Description:
//   This class is used to manage dialog replacement possibilities.  It checks
//   the presence of the Replaces header in incoming INVITE requests, and
//   whether or not its parameters correspond to an existing dialog data.
//
// Location:
//   SipUserAgent/CSipReplacesSvc.h
//
// See Also:
//   ISipCoreSvc, ISipUserAgentSvc, ISipDialogMatcher
//
//==EDOC========================================================================
class CSipReplacesSvc : private CEComDelegatingUnknown,
                        private ISipCoreSvc,
                        private CSipReqCtxCoreSvc,
                        private ISipReplacesSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:

//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipReplacesSvc();

    static void FinalizeCSipReplacesSvc();


    //-- << ECOM mechanism >>
    //------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //   Default Constructor.
    //------------------------
    CSipReplacesSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //   Destructor.
    //---------------
    virtual ~CSipReplacesSvc();


    //-- << ISipReplacesSvc Interface >>
    //--------------------------------------------

    // Summary:
    //   Configures the event manager associated with this service manager.
    //--------------------------------------------------------------------
    virtual mxt_result SetManager(IN ISipReplacesMgr* pMgr);


    //-- << ECOM mechanism >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << ISipCoreSvc Interface >>
    //--------------------------------

    // Summary:
    //   A pointer to the ISipReqCtxCoreSvc to attach to the request context.
    //   It can be NULL.
    //------------------------------------------------------------------------
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                                   IN const CSipPacket& rPacket,
                                                   IN EDirection eDirection);

    // Summary:
    //   Establishes the confidence level for which the service is a good
    //   candidate to own a request context.
    //--------------------------------------------------------------------------
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                                  IN const CSipPacket& rPacket);

    // Summary:
    //   Returns the ISipReqCtxCoreSvc instance to use as the owner for a request
    //   context.
    //--------------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();


    //-- << ISipReqCtxCoreSvc Interface >>
    //--------------------------------------------

    // Summary:
    //   Informs the request context core service of a received packet.
    //------------------------------------------------------------------
    virtual void OnPacketReceived(IN    ISipRequestContext& rRequestContext,
                                  IN    const CSipPacket&   rPacket,
                                  INOUT CSipCoreEventList&  rEventList);

    // Summary:
    //   Modifies a packet about to be sent.
    //-------------------------------------
    virtual mxt_result UpdatePacket(IN    ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket&         rPacket);

    // Summary:
    //   Lets the service call an event on the application.
    //-----------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque          opqEvent,
                           IN const CSipPacket&   rPacket);

    // Summary:
    //   Lets the service clear the state associated with an event in which the
    //   application has no interest.
    //------------------------------------------------------------------------
    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque          opqEvent);

    // Summary:
    //   Adds a reference on this request context core service.
    //---------------------------------------------------------
    virtual unsigned int AddRcsRef();

    // Summary:
    //   Removes a reference on this request context core service.
    //------------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();

    // Summary:
    //   Adds a Supported header to a list of headers.
    //-------------------------------------------------
    void AddSupportedHeader(INOUT CSipPacket& rPacket);

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //   Copy Constructor.
    //---------------------
    CSipReplacesSvc(IN const CSipReplacesSvc& rSrc);

    // Summary:
    //   Assignment Operator.
    //------------------------
    CSipReplacesSvc& operator=(IN const CSipReplacesSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:

    // The event manager.
    //--------------------
    ISipReplacesMgr* m_pMgr;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREPLACESSVC_H

