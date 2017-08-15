//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSIPAUTOMATICANSWERERREQCTXCORESVC_H
#define MXG_CSIPAUTOMATICANSWERERREQCTXCORESVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT) && \
    !defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT or \
MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT to be able to use \
this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class IPrivateSipContext;
class ISipAutomaticAnswererReqCtxSvcMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipAutomaticAnswererReqCtxSvc
//========================================
//
// Description:
//   This class only sends provided final response code and reason to any
//   request for which it is the owner.
//
// Location:
//   SipUserAgent/CSipAutomaticAnswererReqCtxSvc.h
//
// See Also:
//   CSipUserAgentSvc
//
//==EDOC========================================================================
class CSipAutomaticAnswererReqCtxSvc : public CSipReqCtxCoreSvc
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipAutomaticAnswererReqCtxSvc(IN IPrivateSipContext* pParentSipContext,
                                   IN uint16_t uResponseCode,
                                   IN const char* const pszReason = NULL,
                                   IN TOA CHeaderList* pExtraHeaders = NULL,
                                   IN ISipAutomaticAnswererReqCtxSvcMgr* pMgr = NULL,
                                   IN mxt_opaque opqTransaction = MX_VOIDPTR_TO_OPQ(NULL));


    //-- << ISipReqCtxCoreSvc Interface >>
    //--------------------------------------------

    // Summary:
    //  Manages the initial packet of a request context.
    //--------------------------------------------------
    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    // Summary:
    //  Notifies the request context core service of a received packet.
    //-----------------------------------------------------------------
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    // Summary:
    //  Notifies the service that the application is done updating the packet.
    //------------------------------------------------------------------------
    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    // Summary:
    //  Sends a response to a received request.
    //-----------------------------------------
    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket& rPacket,
                                    IN unsigned int uCode,
                                    IN const char* szReason,
                                    IN TO CHeaderList* pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody);

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
    //-------------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipAutomaticAnswererReqCtxSvc();


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipAutomaticAnswererReqCtxSvc(IN const CSipAutomaticAnswererReqCtxSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipAutomaticAnswererReqCtxSvc& operator=(IN const CSipAutomaticAnswererReqCtxSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:

    // A reference to the parent SIP context of the CSipUserAgentSvc that
    // created this object. This is needed to obtain services list.
    IPrivateSipContext* m_pParentSipContext;

    // A reference count used to determine when to destroy this object.
    //------------------------------------------------------------------
    unsigned int m_uRefCount;

    // The response code to use when generating the response.
    uint16_t m_uResponseCode;

    // The reason to use when generating the response.
    const char* const m_pszReason;

    // The extra to add to the response.
    CHeaderList* m_pExtraHeaders;

    // The manager. Can be NULL.
    ISipAutomaticAnswererReqCtxSvcMgr* m_pMgr;

    // The opaque value returned to m_pMgr.
    mxt_opaque m_opqTransaction;

private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPAUTOMATICANSWERERREQCTXCORESVC_H

