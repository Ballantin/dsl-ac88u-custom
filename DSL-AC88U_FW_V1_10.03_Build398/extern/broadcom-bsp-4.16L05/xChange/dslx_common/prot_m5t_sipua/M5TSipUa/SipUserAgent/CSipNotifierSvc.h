//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPNOTIFIERSVC_H
#define MXG_CSIPNOTIFIERSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT to be able \
to use this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif


//-- Parameters used by Value
//-----------------------------

#ifndef MXG_CTOKEN_H
#include "SipParser/CToken.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSIPCLIENTSVC_H
#include "SipUserAgent/CSipClientSvc.h"
#endif

#ifndef MXG_ISIPNOTIFIERSVC_H
#include "SipUserAgent/ISipNotifierSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif


//-- Inline Usage.
//-----------------
#ifndef MXG_ISIPREQUESTCONTEXT_H
#include "SipCore/ISipRequestContext.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CGenParamList;
class CHeaderList;
class CSipMessageBody;
class CSipHeader;
class CSipPacket;
class ISipClientTransaction;
class ISipNotifierMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipNotifierSvc
//========================================
// <FLAG New in 4.1.4>
//
// Description:
//   Implementation for the ISipNotifierSvc.
//
// Warning:
//   This class replaces the old class with the same name, which
//   has been deprecated and renamed CSipSubscriberBaseSvc
//   for clarity.
//
// Location:
//   SipUserAgent/CSipNotifierSvc.h
//
// See Also:
//   ISipNotifierSvc
//
//==EDOC========================================================================
class CSipNotifierSvc : protected CSipClientSvc,
                        private CEventDriven,
                        private CSipReqCtxCoreSvc,
                        protected ISipNotifierSvc,
                        protected ISipCoreSvc
{
//-- Friend Declarations
//------------------------
//M5T_INTERNAL_USE_BEGIN
friend class CSipStackInitializer;
//M5T_INTERNAL_USE_END

//-- Published Interface
//------------------------
public:

    // Summary:
    //  Sets whether or not the handling of bad events is done internally.
    //---------------------------------------------------------------------
    static void SetExternalHandlingOfBadEvents(IN bool bHandleExternally);

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipNotifierSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipNotifierSvc();

    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipNotifierSvc();

    static void FinalizeCSipNotifierSvc();

    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Stereotype >>
    //--------------------------------------------

    //-- << ISipNotifierSvc interface >>
    //--------------------------------------------

    virtual mxt_result SetManager(IN ISipNotifierMgr* pMgr);

    virtual mxt_result AddEvent(IN const CString& rstrEvent,
                                IN unsigned int uDefaultExpiration);

    virtual mxt_result SetMinimumExpiration(
                                IN const CString& rstrEvent,
                                IN unsigned int uMinExpirationSec);
 
    virtual mxt_result CreateSubscription(IN const CString& rstrEvent,
                                          IN const CString& rstrId,
                                          IN unsigned int uExpirationSec,
                                          IN EType eType);

    virtual mxt_result ExtendImplicitSubscription(
                                            IN const CString& rstrEvent,
                                            IN const CString& rstrId,
                                            IN unsigned int uExpirationSec);

    virtual mxt_result Notify(IN    const CString& rstrEvent,
                              IN    const CString& rstrId,
                              IN    EState eSubscriptionState,
                              IN    unsigned int uExpirationSec,
                              IN    mxt_opaque opqTransaction,
                              IN TO CHeaderList* pExtraHeaders,
                              IN TO CSipMessageBody* pMessageBody,
                              IN TO CGenParamList* pCustomParameters,
                              OUT   ISipClientTransaction*& rpTransaction);

    virtual mxt_result Terminate(IN    const CString& rstrEvent,
                                 IN    const CString& rstrId,
                                 IN    EReason eReason,
                                 IN    unsigned int uRetryAfterSec,
                                 IN    mxt_opaque opqTransaction,
                                 IN TO CHeaderList* pExtraHeaders,
                                 IN TO CSipMessageBody* pMessageBody,
                                 IN TO CGenParamList* pCustomParameters,
                                 OUT   ISipClientTransaction*& rpTransaction);

    virtual void GetCurrentSubscriptions(OUT CVector<SSubscriptionInfo>& rvecstSubscriptions);

private:
    //-- << CEventDriven interface >>
    //--------------------------------------------

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);


    //-- << ISipReqCtxCoreSvc interface >>
    //--------------------------------------------

    // Summary:
    //  Manages the initial packet of a request context.
    //---------------------------------------------------
    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    // Summary:
    //  Informs the request context core service of a received packet.
    //-----------------------------------------------------------------
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    // Summary:
    //  Informs the service that the application is done updating the packet.
    //------------------------------------------------------------------------
    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    // Summary:
    //  Sends a response to a received request.
    //------------------------------------------
    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket,
                                    IN unsigned int uCode,
                                    IN const char* szReason,
                                    IN TO CHeaderList*     pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody);

    // Summary:
    //  Informs that a CANCEL was received for a pending request.
    //------------------------------------------------------------
    virtual void OnRequestCancelled(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket);

    virtual void OnPacketReadyToSend(IN ISipRequestContext& rRequestContext,
                                     INOUT CSipPacket& rPacket);

    // Summary:
    //  Lets the service call an event on the application.
    //-----------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    // Summary:
    //  Lets the service clear the state associated with an event in which the
    //  application has no interest.
    //-------------------------------------------------------------------------
    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    // Summary:
    //  Adds a reference on this request context core service.
    //---------------------------------------------------------
    virtual unsigned int AddRcsRef();

    // Summary:
    //  Removes a reference on this request context core service.
    //------------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();


    //-- << CSipClientSvc interface >>
    //--------------------------------------------

    virtual void GetRequestContext(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   OUT ISipRequestContext*& rpRequestContext);

    virtual void RequestContextInError(IN ISipRequestContext& rRequestContext);


    //-- << ISipClientSvc interface >>
    //--------------------------------------------

    // Summary:
    //  Sends a request.
    //-------------------
    virtual mxt_result SendRequest(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   IN TO CHeaderList* pExtraHeaders,
                                   IN TO CSipMessageBody* pMessageBody,
                                   IN mxt_opaque opqService,
                                   OUT ISipClientTransaction*& rpClientTransaction,
                                   OUT CSipPacket** ppPacket,
                                   IN ISipForkedDialogGrouperMgr* pGrouperMgr = NULL,
                                   IN const CToken* pCreatingDialogRequestMethod = NULL);

    //-- << ECOM interface >>
    //-------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << ISipCoreSvc interface >>
    //--------------------------------------------

    // Summary:
    //  Obtains an instance of an ISipReqCtxCoreSvc to attach to a request
    //  context.
    //---------------------------------------------------------------------
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
    //  Returns the ISipReqCtxCoreSvc instance to use as the owner for a request
    //  context.
    //--------------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();


    //-- << Helper methods >>
    //--------------------------------------------

    // Summary:
    //  Creates the subscription information in the subscription vector if it
    //  does not exist.
    //--------------------------------------------------------------------------
    mxt_result AddSubscription(IN const CToken& rtokEvent,
                               IN const CString& rstrId,
                               IN unsigned int uExpirationSec,
                               IN EType eType);

    struct SSubscription;

    // Summary:
    //  Returns a pointer to the subscription described by this ID. NULL if not
    //  found.
    //--------------------------------------------------------------------------
    SSubscription* GetSubscription(IN const CToken& rtokEvent,
                                   IN const CString& rstrId);

    // Summary:
    //  Returns a pointer to the subscription described by the ID in the Event
    //  header of this header list. NULL if not found.
    //-------------------------------------------------------------------------
    SSubscription* GetSubscription(IN const CHeaderList& rHdrs);

    // Summary:
    //  Returns a pointer to the subscription that has this request context as
    //  its m_pNotifyReqCtx data member.
    //-------------------------------------------------------------------------
    SSubscription* GetSubscription(IN const ISipRequestContext& rReqCtx);

    mxt_result SendNotifyHelper(
                            IN    mxt_opaque              opq,
                            IN    const CToken&           rtokEvent,
                            IN    const CString&          rstrId,
                            IN    const CString&          rstrSubscriptionState,
                            IN TO CGenParamList*          pSubStateParams,
                            IN TO CHeaderList*            pExtraHeaders,
                            IN TO CSipMessageBody*        pMessageBody,
                            IN TO CGenParamList*          pCustomParameters,
                            OUT   ISipClientTransaction*& rpClientTransaction);

    void ProcessEventHelper(IN ISipRequestContext& rRequestContext,
                            IN unsigned int uEvent,
                            IN const CSipPacket* pPacket);

    void VerifyExpiration();

    // Create the allow event.
    //-------------------------
    GO CSipHeader* GetAllowEvents();

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipNotifierSvc(IN const CSipNotifierSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipNotifierSvc& operator=(IN const CSipNotifierSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    struct SSubscription
    {
        CToken m_tokEvent;

        CString m_strId;

        EType m_eType;

        uint64_t m_uExpirationTimeMs;

        unsigned int m_uAttemptedExpiration;

        ISipRequestContext* m_pNotifyReqCtx;

        bool m_bInDialog;

        bool m_bIsTerminating;

        SSubscription(IN const CToken& rtokEvent,
                      IN const CString& rstrId,
                      IN uint64_t uExpirationTimeMs,
                      IN EType eType)
        : m_tokEvent(rtokEvent),
          m_strId(rstrId),
          m_eType(eType),
          m_uExpirationTimeMs(uExpirationTimeMs),
          m_uAttemptedExpiration(0),
          m_pNotifyReqCtx(NULL),
          m_bInDialog(false),
          m_bIsTerminating(false) {}

        ~SSubscription()
        {
            if (m_pNotifyReqCtx != NULL)
            {
                m_pNotifyReqCtx->ReleaseIfRef();
                m_pNotifyReqCtx = NULL;
            }
        }
    private:
        // Summary:
        //  Default Constructor.
        //-----------------------
        SSubscription();
    };

    enum
    {
        ePROVISIONAL_RESPONSE,
        eSUCCESS_RESPONSE,
        eOTHER_EVENT
    };

    ISipNotifierMgr* m_pMgr;

    CVector<SSubscription*> m_vecSubscriptions;

    // Information about supported events.
    //---------------------------------------
    struct SEventInfo
    {
        unsigned int m_uDefaultExpirationSec;
        unsigned int m_uMinExpirationSec;

        SEventInfo(unsigned int uDefaultExpirationSec, 
                   unsigned int uMinExpirationSec = 0)
        :   m_uDefaultExpirationSec(uDefaultExpirationSec),
            m_uMinExpirationSec(uMinExpirationSec)
        {
        }
    };

    // Map to hold the event types, with their default expiration.
    // A CToken is used instead of a CString because comparisons
    // must be case-insensitive.
    //---------------------------------------------------------------
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
    // GCC 2.7.2 does not support default parameters in templates,
    // in which case all the parameters must be specified.
    CMap<CToken, SEventInfo*, CAATree<CMapPair<CToken, SEventInfo*> > > m_mapEvents2Info;
#else
    CMap<CToken, SEventInfo*> m_mapEvents2Info;
#endif

    // Tells if the bad events are handled externally or not.
    //--------------------------------------------------------
    static bool m_bExternalHandleBadEvents;
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPNOTIFIERSVC_H
