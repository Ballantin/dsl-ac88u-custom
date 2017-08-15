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
#ifndef MXG_CSIPREFEREESVC_H
#define MXG_CSIPREFEREESVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT to be able \
to use this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_CMAP_H
#include "Cap/CVector.h"
#endif

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

#ifndef MXG_ISIPNOTIFIERMGR_H
#include "SipUserAgent/ISipNotifierMgr.h"
#endif

#ifndef MXG_ISIPREFEREESVC_H
#include "SipUserAgent/ISipRefereeSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CEComUnknown;
class CString;
class IEComUnknown;
class ISipRefereeMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipRefereeSvc
//========================================
//
// Description:
//   Implementation of the ISipRefereeSvc.
//
// Location:
//   SipUserAgent/CSipRefereeSvc.h
//
// See Also:
//   ISipRefereeSvc
//
//==EDOC========================================================================
class CSipRefereeSvc : private CEComDelegatingUnknown,
                       private ISipCoreSvc,
                       private CSipReqCtxCoreSvc,
                       private ISipNotifierMgr,
                       private ISipRefereeSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Inner Classes
//------------------------

//M5T_INTERNAL_USE_BEGIN
    struct SReferIdInformation
    {
        // Whether a final notification was sent.
        //----------------------------------------------
        bool m_bFinalStatusSent;

        uint64_t m_uReferId;

        SReferIdInformation(uint64_t uReferId = 0)
        :   m_bFinalStatusSent(false),
            m_uReferId(uReferId)
        {
        }
    };
//M5T_INTERNAL_USE_END



//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipRefereeSvc();

    static void FinalizeCSipRefereeSvc();


    //-- << ECOM mechanism >>
    //-------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // waring or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif
//M5T_INTERNAL_USE_END

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipRefereeSvc(IN IEComUnknown& rOuterIEComUnknow);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipRefereeSvc();


    //-- << ECOM interface >>
    //-------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    // Summary:
    //  Obtain the request interface.
    //--------------------------------
    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << ISipCore interface >>
    //--------------------------------------------

    // Summary:
    //  Obtain an instance of an ISipReqCtxCoreSvc to attach to a request
    //  context.
    //--------------------------------------------------------------------
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                                   IN const CSipPacket& rPacket,
                                                   IN EDirection eDirection);

    // Summary:
    //  Gives to which degree the service is a good candidate to own a request
    //  context.
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


    //-- << ISipReqCtxCoreSvc Interface >>
    //--------------------------------------------

    // Summary:
    //  Manage the initial packet of a request context.
    //--------------------------------------------------
    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    // Summary:
    //  Informs the request context core service of a received packet.
    //-----------------------------------------------------------------
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    // Summary:
    //  Inform the service that the application is done updating the packet.
    //-----------------------------------------------------------------------
    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    // Summary:
    //  Send a response to a received request.
    //-----------------------------------------
    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket,
                                    IN unsigned int uCode,
                                    IN const char* szReason,
                                    IN TO CHeaderList*     pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody);

    // Summary:
    //  Inform that a CANCEL was received for a pending request.
    //-----------------------------------------------------------
    virtual void OnRequestCancelled(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket);

    // Summary:
    //  Let the service call an event on the application.
    //----------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    // Summary:
    //  Let the service clear state associated to an event in which the
    //  application has no interest.
    //------------------------------------------------------------------
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


    //-- << ISipNotifierMgr Interface >>
    //--------------------------------------------

    // Summary:
    //  A SUBSCRIBE request for a new subscription was received.
    //-----------------------------------------------------------
    virtual void EvSubscribed(IN ISipNotifierSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN unsigned int uExpirationSec,
                              IN const CSipPacket& rSubscribe);

    // Summary:
    //  A SUBSCRIBE request to fetch the current state was received.
    //---------------------------------------------------------------
    virtual void EvFetched(IN ISipNotifierSvc* pSvc,
                           IN ISipServerEventControl* pServerEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rSubscribe);

    // Summary:
    //  A SUBSCRIBE request to refresh an active subscription was received.
    //----------------------------------------------------------------------
    virtual void EvRefreshed(IN ISipNotifierSvc* pSvc,
                             IN ISipServerEventControl* pServerEventControl,
                             IN const CString& rstrEvent,
                             IN const CString& rstrId,
                             IN unsigned int uExpirationSec,
                             IN const CSipPacket& rSubscribe);

    // Summary:
    //  A SUBSCRIBE request to terminate an active/pending subscription was
    //  received.
    //----------------------------------------------------------------------
    virtual void EvTerminated(IN ISipNotifierSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN const CSipPacket& rSubscribe);

    // Summary:
    //  A SUBSCRIBE request was automatically rejected by this service.
    //------------------------------------------------------------------
    virtual void EvInvalidSubscribe(IN ISipNotifierSvc* pSvc,
                                    IN mxt_opaque opqApplicationData,
                                    IN const CSipPacket& rSubscribe,
                                    IN mxt_result reason);

    // Summary:
    //  A provisional response (1xx) was received for a NOTIFY request sent by
    //  the ISipNotifierSvc.
    //-------------------------------------------------------------------------
    virtual void EvProgress(IN ISipNotifierSvc* pSvc,
                            IN ISipClientEventControl* pClientEventControl,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId,
                            IN const CSipPacket& rResponse);

    // Summary:
    //  A success response (2xx) was received for a NOTIFY request sent by the
    //  ISipNotifierSvc.
    //-------------------------------------------------------------------------
    virtual void EvSuccess(IN ISipNotifierSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse);

    // Summary:
    //  A failure (<= 3xx) was received for a NOTIFY request sent by the
    //  ISipNotifierSvc.
    //-------------------------------------------------------------------
    virtual void EvFailure(IN ISipNotifierSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse);

    // Summary:
    //  The subscription is expired.
    //-------------------------------
    virtual void EvExpired(IN ISipNotifierSvc* pSvc,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId);


    //-- << ISipRefereeSvc Interface >>
    //--------------------------------------------

    // Summary:
    //  Configures the manager of this service.
    //------------------------------------------
    virtual mxt_result SetManager(IN ISipRefereeMgr* pMgr);

    // Summary:
    //  Send a NOTIFY for a received REFER.
    //--------------------------------------
    virtual mxt_result SendReferralStatus(IN mxt_opaque opqReferId,
                                          IN ISipNotifierSvc::EState eSubscriptionState,
                                          IN unsigned int uExpirationSec,
                                          IN const CSipStatusLine& rContentStatusLine,
                                          IN const CHeaderList* pContentHeaders,
                                          IN mxt_opaque opqTransaction,
                                          IN TO CHeaderList* pExtraHeaders,
                                          OUT ISipClientTransaction*& rpTransaction);

    // Summary:
    //  Send a final NOTIFY for a received REFER.
    //--------------------------------------------
    virtual mxt_result SendFinalReferralStatus(IN mxt_opaque opqReferId,
                                               IN ISipNotifierSvc::EReason eReason,
                                               IN unsigned int uRetryAfterSec,
                                               IN const CSipStatusLine& rContentStatusLine,
                                               IN const CHeaderList* pContentHeaders,
                                               IN mxt_opaque opqTransaction,
                                               IN TO CHeaderList* pExtraHeaders,
                                               OUT ISipClientTransaction*& rpTransaction);

    // Summary:
    //  Configures whether or not the stack will use the "id" parameter in the
    //  Event header of NOTIFY requests for first REFER request.
    //-------------------------------------------------------------------------
    virtual void ConfigureNotifyIdParameterUsage(EIdParameterUsage eIdParamUsage);

    //-- << Helper Methods >>
    //--------------------------------------------

    // Summary:
    //  Gets the REFER state.
    //--------------------------------------------------
    bool* GetReferState(IN mxt_opaque opqReferId);

    // Summary:
    //  Converts a received empty string to the id of the first REFER.
    //-----------------------------------------------------------------
    mxt_opaque ConvertEmptyStringToFirstReferId(IN const CString& rstrId);

    static GO CSipMessageBody* CreateContentInfo(
                                    IN const CSipStatusLine& rContentStatusLine,
                                    IN const CHeaderList* pContentHeaders);

    // Summary:
    //  Converts a subscription 'id' to the string equivalent.
    //---------------------------------------------------------
    void GenerateEventId(IN mxt_opaque opqReferId, OUT CString& rstrId);


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipRefereeSvc(IN const CSipRefereeSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipRefereeSvc& operator=(IN const CSipRefereeSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    // Aggregated ISipNotifierSvc. It will be responsible for sending NOTIFYs
    // and receiving SUBSCRIBE requests.
    //------------------------------------------------------------------------
    IEComUnknown* m_pNotifierSvc;

    ISipRefereeMgr* m_pMgr;

    // Handles if NOTIFY requests to the first REFER add the id parameter in
    // the Event header.
    //-----------------------------------------------------------------------
    EIdParameterUsage m_eIdParamUsage;

    // The first accepted REFER opaque to which we will send NOTIFYs.
    //----------------------------------------------------------------
    uint64_t m_uFirstSentRefer;

    // Map between refer IDs and their associated state.
    //---------------------------------------------------
    CVector<SReferIdInformation*> m_vecReferIdInformation;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline void CSipRefereeSvc::ConfigureNotifyIdParameterUsage(EIdParameterUsage eIdParamUsage)
{
    m_eIdParamUsage = eIdParamUsage;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREFEREESVC_H

