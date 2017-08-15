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
#ifndef MXG_CSIPREFERRERSVC_H
#define MXG_CSIPREFERRERSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT, to be \
able to use this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_ISIPREFEREEMGR_H
#include "SipUserAgent/ISipReferrerMgr.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSIPCLIENTSVC_H
#include "SipUserAgent/CSipClientSvc.h"
#endif

#ifndef MXG_ISIPREFERRERSVC_H
#include "SipUserAgent/ISipReferrerSvc.h"
#endif

#ifndef MXG_ISIPSUBSCRIBERMGR_H
#include "SipUserAgent/ISipSubscriberMgr.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CEComUnknown;
class CHeaderList;
class CSipStatusLine;
class IEComUnknown;
class ISipRequestContext;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipReferrerSvc
//========================================
//
// Description:
//   Implementation of the ISipReferrerSvc.
//
// Location:
//   SipUserAgent/CSipReferrerSvc.h
//
// See Also:
//   ISipReferrerSvc
//
//==EDOC========================================================================
class CSipReferrerSvc : private CSipClientSvc,
                        private CSipReqCtxCoreSvc,
                        private ISipSubscriberMgr,
                        private ISipReferrerSvc,
                        private ISipCoreSvc
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

    static mxt_result InitializeCSipReferrerSvc();

    static void FinalizeCSipReferrerSvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipReferrerSvc(IN IEComUnknown& rOuterIEComUnknow);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipReferrerSvc();


    //-- << CSipClientSvc Interface >>
    //--------------------------------------------

    virtual void GetRequestContext(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   OUT ISipRequestContext*& rpRequestContext);

    virtual void RequestContextInError(IN ISipRequestContext& rRequestContext);


    //-- << IEComUnknown mechanism >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << ISipClientSvc Interface >>
    //--------------------------------------------

    // Summary:
    //  Send a request.
    //------------------
    virtual mxt_result SendRequest(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   IN TO CHeaderList* pExtraHeaders,
                                   IN TO CSipMessageBody* pMessageBody,
                                   IN mxt_opaque opqService,
                                   OUT ISipClientTransaction*& rpClientTransaction,
                                   OUT CSipPacket** ppPacket,
                                   IN ISipForkedDialogGrouperMgr* pGrouperMgr = NULL,
                                   IN const CToken* pCreatingDialogRequestMethod = NULL);

    //-- << ISipCoreSvc Interface >>
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

    virtual void OnPacketReadyToSend(IN ISipRequestContext& rRequestContext,
                                     INOUT CSipPacket& rPacket);

    // Summary:
    //  Let the service call an event on the application.
    //----------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    // Summary:
    //  Let the service clear the state associated with an event in which
    //  the application has no interest.
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


    //-- << ISipSubscriberMgr Interface >>
    //--------------------------------------------

    // Summary:
    //  A provisional response (1xx) was received for a SUBSCRIBE request sent
    //  by the ISipSubscriberSvc.
    //-------------------------------------------------------------------------
    virtual void EvProgress(IN ISipSubscriberSvc* pSvc,
                            IN ISipClientEventControl* pClientEventControl,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId,
                            IN const CSipPacket& rResponse);

    // Summary:
    //  A success response (2xx) was received for a SUBSCRIBE request sent by
    //  the ISipSubscriberSvc.
    //------------------------------------------------------------------------
    virtual void EvSuccess(IN ISipSubscriberSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse);

    // Summary:
    //  An "Interval Too Small" (423) was received for a SUBSCRIBE request sent
    //  by the ISipSubscriberSvc.
    //--------------------------------------------------------------------------
    virtual void EvIntervalTooSmall(
                                 IN ISipSubscriberSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN unsigned int uMinExpirationSec,
                                 IN const CString& rstrEvent,
                                 IN const CString& rstrId,
                                 IN const CSipPacket& rResponse);

    // Summary:
    //  A failure (<= 3xx) was received for a SUBSCRIBE request sent by the
    //  ISipSubscriberSvc.
    //----------------------------------------------------------------------
    virtual void EvFailure(IN ISipSubscriberSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse);

    // Summary:
    //  A NOTIFY request was received for a valid active subscription.
    //-----------------------------------------------------------------
    virtual void EvNotified(IN ISipSubscriberSvc* pSvc,
                            IN ISipServerEventControl* pServerEventControl,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId,
                            IN const CSipPacket& rNotify);

    // Summary:
    //  A final NOTIFY was received for a valid active subscription.
    //---------------------------------------------------------------
    virtual void EvTerminated(IN ISipSubscriberSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN const CSipPacket& rNotify);

    // Summary:
    //  An invalid NOTIFY was received.
    //----------------------------------
    virtual void EvInvalidNotify(IN ISipSubscriberSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rNotify,
                                 IN mxt_result reason);

    // Summary:
    //  The subscription is about to expire.
    //---------------------------------------
    virtual void EvExpiring(IN ISipSubscriberSvc* pSvc,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId);

    // Summary:
    //  The subscription is expired.
    //-------------------------------
    virtual void EvExpired(IN ISipSubscriberSvc* pSvc,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId);


    //-- << ISipReferrerSvc Interface >>
    //--------------------------------------------

    // Summary:
    //  Configures the manager of this service.
    //------------------------------------------
    virtual mxt_result SetManager(IN ISipReferrerMgr* pMgr);

    // Summary:
    //  Sets the time to warn the application before a subscription expires.
    //-----------------------------------------------------------------------
    virtual void SetExpiringThreshold(IN unsigned int uThresholdSec);

    // Summary:
    //  Send a REFER request.
    //------------------------
    virtual mxt_result Refer(IN    const CNameAddr&        rReferToAddr,
                             IN    mxt_opaque              opqTransaction,
                             IN TO CHeaderList*            pExtraHeaders,
                             IN TO CSipMessageBody*        pMessageBody,
                             OUT   mxt_opaque&             ropqReferId,
                             OUT   ISipClientTransaction*& rpTransaction);

    // Summary:
    //  Extends the subscription for that REFER.
    //-------------------------------------------
    virtual mxt_result Refresh(IN mxt_opaque opqReferId,
                               IN unsigned int uExpirationSec,
                               IN mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               OUT ISipClientTransaction*& rpTransaction);

    // Summary:
    //  Terminates the subscription for that REFER.
    //----------------------------------------------
    virtual mxt_result Terminate(IN mxt_opaque opqReferId,
                                 IN mxt_opaque opqTransaction,
                                 IN TO CHeaderList* pExtraHeaders,
                                 IN TO CSipMessageBody* pMessageBody,
                                 OUT ISipClientTransaction*& rpTransaction);


    //-- << Helper Methods >>
    //--------------------------------------------

    void ProcessEventHelper(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent,
                            IN const CSipPacket* pPacket);

    unsigned int GetReferId(IN const CString& rstrId) const;

    void NotifyReceived( IN ISipServerEventControl* pServerEventControl,
                         IN const CString& rstrId,
                         IN const CSipPacket& rNotify );

    // Summary:
    //  Return the subscription state from a SIP packet.
    ISipReferrerMgr::EState GetSubscriptionStateHelper(IN const CSipPacket& rNotify);


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipReferrerSvc(IN const CSipReferrerSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipReferrerSvc& operator=(IN const CSipReferrerSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    ISipReferrerMgr* m_pMgr;

    // The aggregated ISipSubscriberReferSvc.
    //----------------------------------------
    IEComUnknown* m_pSubscriberSvc;

    // This service only allows to send one REFER at a time in order to prevent
    // receiving NOTIFY without id parameter for two different REFER requests.
    // If we do not care about that restriction, we can use a CVector of
    // m_pReferRequestContext along with a corresponding CVector of ReferIdx
    // (or a CVector of a structure containing both).
    //--------------------------------------------------------------------------
    ISipRequestContext* m_pReferRequestContext;

    // This data member represents the index in the m_vecstrEventIds vector that
    // corresponds to the REFER request sent by the m_pReferRequestContext. It
    // has significance only when m_pReferRequestContext is not NULL.
    //-------------------------------------------------------------------------
    unsigned int m_uCurrentReferIdx;

    CVector<CString> m_vecstrEventIds;

    // The index in the m_vecstrEventIds that can receive NOTIFY without id.
    // There is only one possible in a dialog because NOTIFY to a second REFER
    // in a dialog MUST put the id parameter. This parameter is tricky and
    // here's some useful info about how it works. When no REFER subscription
    // can receive an empty NOTIFY without id, it is set to an invalid index.
    // When we send a REFER and this index is invalid, we set it to the index
    // for the new id. This way, NOTIFY without id will be mapped to this id.
    // If this index is valid and we receive a NOTIFY with an id that
    // corresponds to that index, it means that the notifier is using the id
    // and hence we will not receive a NOTIFY without id. This way, we can
    // reset the index and terminate the subscription without id. When the
    // subscription expires or is about to expire, RFC3515 mandates that the id
    // be used in the SUBSCRIBE request that terminates or refreshes the
    // subscription. This way, the notifier should put the id in the NOTIFY and
    // hence the subscription without id will be terminated. Finally, if the
    // REFER request is rejected, both the subscription with and without the id
    // will be terminated so the subscription without id can be reused for a
    // REFER request that will succeed.
    //--------------------------------------------------------------------------
    unsigned int m_uEmptyIdIdx;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREFERRERSVC_H

