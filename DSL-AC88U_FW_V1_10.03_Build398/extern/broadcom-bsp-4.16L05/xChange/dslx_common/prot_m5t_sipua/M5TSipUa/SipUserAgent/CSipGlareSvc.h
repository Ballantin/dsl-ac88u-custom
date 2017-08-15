//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_CSIPGLARESVC_H
#define MXG_CSIPGLARESVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT to be able to \
use this class.
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

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISIPGLARESVC_H
#include "SipUserAgent/ISipGlareSvc.h"
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
//== Class: CSipGlareSvc
//========================================
//
// Description:
//   This class is the implementation of the ISipGlareSvc interface. Direct
//   access to this class is prohibited, it should be accessed through the ECOM
//   mechanism via the ISipContext.
//
// Location:
//   SipUserAgentSvc/CSipGlareSvc.h
//
// See Also:
//   IGlareSvcMgr
//
//==EDOC========================================================================
class CSipGlareSvc : private CEComDelegatingUnknown,
                     private ISipCoreSvc,
                     private CSipReqCtxCoreSvc,
                     private ISipGlareSvc,
                     private CEventDriven
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipGlareSvc();

    static void FinalizeCSipGlareSvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor. Used through factory method local to this class.
    //------------------------------------------------------------------------
    CSipGlareSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipGlareSvc();

    //-- << ISipGlareSvc methods >>
    //------------------------------

    // Summary:
    //  Configures the event manager associated with this service.
    //------------------------------------------------------------
    virtual mxt_result SetManager(IN ISipGlareMgr* pMgr);

    //-- << ISipCoreSvc methods >>
    //-----------------------------

    // Summary:
    //  Obtain an instance of an ISipReqCtxCoreSvc to attach to a request
    //  context.
    //----------------------------------------------------------------------
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                               IN const CSipPacket& rPacket,
                                               IN EDirection        eDirection);

    // Summary:
    //  Gives to which degree the service is a good candidate to own a request
    // context.
    //-------------------------------------------------------------------------
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                               IN const CSipPacket& rPacket);

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

    //-- << ISipReqCtxCoreSvc methods >>
    //-----------------------------------

    // Summary:
    //  Informs the request context core service of a received packet.
    //-------------------------------------------------------------------
    virtual void OnPacketReceived(IN    ISipRequestContext& rRequestContext,
                                  IN    const CSipPacket&   rPacket,
                                  INOUT CSipCoreEventList&  rEventList);

    // Summary:
    //  Modify a packet about to be sent.
    //--------------------------------------
    virtual mxt_result UpdatePacket(IN    ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    // Summary:
    //  Add a reference on this request context core service.
    //----------------------------------------------------------
    virtual unsigned int AddRcsRef();

    // Summary:
    //  Remove a reference on this request context core service.
    //-------------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();

    //-- << ECOM mechanism >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << CEventDriven >>
    //-----------------------

    // Summary:
    //  Notify the manager that a new timer elapsed or was stopped.
    //--------------------------------------------------------------
    virtual void EvTimerServiceMgrAwaken( IN bool bStopped,
                                          IN unsigned int uTimer,
                                          IN mxt_opaque opq );

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

//-- Hidden Methods
//-------------------
protected:

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipGlareSvc(IN const CSipGlareSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipGlareSvc& operator=(IN const CSipGlareSvc& rSrc);

    //-- << Helpers >>
    //-----------------

//-- Hidden Data Members
//------------------------
protected:
private:

    enum ECallIdState
    {
        eUNINITIALIZED,
        eCALL_ID_OWNER,
        eNOT_CALL_ID_OWNER
    };

    enum ETimingState
    {
        eNOT_TIMING,
        eTIMING,
        eOTHER_TRANSACTION_WHILE_TIMING
    };

    // Whether or not we are the owner of the Call-ID. Needed to start the
    // appropriate timer.
    //---------------------------------------------------------------------
    ECallIdState m_eCallIdState;

    // Whether or not we are timing and whether or not another request was 
    // exchanged since we are timing.
    //------------------------------------------------------------------------
    ETimingState m_eTimingState;

    // Pointer to the manager.
    //-------------------------
    ISipGlareMgr* m_pMgr;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPGLARESVC_H

