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
//   form whatsoever without written prior approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSIPSUBSCRIBERSVC_H
#define MXG_CSIPSUBSCRIBERSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT to be \
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

#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_CSIPCLIENTSVC_H
#include "SipUserAgent/CSipClientSvc.h"
#endif

#ifndef MXG_ISIPSUBSCRIBERSVC_H
#include "SipUserAgent/ISipSubscriberSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif


//-- Inline Usage.
//-----------------
#ifndef MXG_ISIPREQUESTCONTEXT_H
#include "SipCore/ISipRequestContext.h"
#endif

#ifndef MXG_CGENPARAMLIST_H
#include "SipParser/CGenParamList.h"
#endif

MX_NAMESPACE_START(MXD_GNS)
class CHeaderList;
class CSipMessageBody;
class CSipHeader;
class CSipPacket;
class ISipClientTransaction;
class ISipRequestContext;
class ISipSubscriberMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipSubscriberSvc
//========================================
// <FLAG New in 4.1.4>
//
// Description:
//   Implementation of ISipSubscriberSvc. 
//
// Warning:
//   This class replaces the old class with the same name, which 
//   has been deprecated and renamed CSipSubscriberBaseSvc 
//   for clarity.
//
// Location:
//   SipUserAgent/CSipSubscriberSvc.h
//
// See Also:
//   ISipSubscriberSvc
//
//==EDOC========================================================================
class CSipSubscriberSvc : protected CSipClientSvc,
                          private CEventDriven,
                          private CSipReqCtxCoreSvc,
                          protected ISipSubscriberSvc,
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

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipSubscriberSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipSubscriberSvc();

    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipSubscriberSvc();

    static void FinalizeCSipSubscriberSvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Stereotype >>
    //--------------------------------------------

    //-- << ISipSubscriberSvc interface >>
    //--------------------------------------------

    // Summary:
    //  Configures the manager of this service.
    //------------------------------------------
    virtual mxt_result SetManager(IN ISipSubscriberMgr* pMgr);

    // Summary:
    //  Sets the time to warn the application before a subscription expires.
    //-----------------------------------------------------------------------
    virtual mxt_result SetExpiringThreshold(
                                   IN const CString& rstrEvent,
                                   IN unsigned int uThresholdSec,
                                   IN EThresholdType eThresholdType = eTHRESHOLD_FIXED);

    // Summary:
    //  Creates a subscription without sending a SUBSCRIBE request.
    //--------------------------------------------------------------
    virtual mxt_result CreateSubscription(
                                    IN    const CString& rstrEvent,
                                    IN    const CString& rstrId,
                                    IN    unsigned int   uExpirationSec,
                                    IN    EType          eType,
                                    IN TO CGenParamList* pCustomParameters);

    // Summary:
    //  Terminates a subscription without sending a SUBSCRIBE request.
    //----------------------------------------------------------------
    virtual mxt_result AbortSubscription(IN const CString& rstrEvent,
                                         IN const CString& rstrId);

    // Summary:
    //  Sends a SUBSCRIBE request to establish a subscription.
    //--------------------------------------------------------
    virtual mxt_result Subscribe(
                            IN    const CString&          rstrEvent,
                            IN    const CString&          rstrId,
                            IN    unsigned int            uExpirationSec,
                            IN    mxt_opaque              opqTransaction,
                            IN TO CHeaderList*            pExtraHeaders,
                            IN TO CSipMessageBody*        pMessageBody,
                            IN TO CGenParamList*          pCustomParameters,
                            OUT   ISipClientTransaction*& rpTransaction);

    // Summary:
    //  Sends a SUBSCRIBE request only to obtain a single NOTIFY.
    //------------------------------------------------------------
    virtual mxt_result Fetch(
                             IN    const CString&          rstrEvent,
                             IN    const CString&          rstrId,
                             IN    mxt_opaque              opqTransaction,
                             IN TO CHeaderList*            pExtraHeaders,
                             IN TO CSipMessageBody*        pMessageBody,
                             IN TO CGenParamList*          pCustomParameters,
                             OUT   ISipClientTransaction*& rpTransaction);

    // Summary:
    //  Extends an active subscription.
    //----------------------------------
    virtual mxt_result Refresh(IN const CString& rstrEvent,
                               IN const CString& rstrId,
                               IN unsigned int uExpirationSec,
                               IN mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               OUT ISipClientTransaction*& rpTransaction,
                               IN TOA CGenParamList* pCustomParameters);

    // Summary:
    //  Terminates an active subscription.
    //------------------------------------
    virtual mxt_result Terminate(IN const CString& rstrEvent,
                                 IN const CString& rstrId,
                                 IN mxt_opaque opqTransaction,
                                 IN TO CHeaderList* pExtraHeaders,
                                 IN TO CSipMessageBody* pMessageBody,
                                 OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result AddEvent(IN const CString& rstrEventType, 
                                IN unsigned int uDefaultExpiration);

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
    //-----------------------------------------------------------------------
    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    // Summary:
    //  Sends a response to a received request.
    //-----------------------------------------
    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket,
                                    IN unsigned int        uCode,
                                    IN const char*         szReason,
                                    IN TO CHeaderList*     pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody);

    // Summary:
    //  Notifies that a CANCEL has been received for a pending request.
    //-----------------------------------------------------------------
    virtual void OnRequestCancelled(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket);

    virtual void OnPacketReadyToSend(IN ISipRequestContext& rRequestContext,
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
    //  Removes a reference on this request context core service.
    //-----------------------------------------------------------
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
    //--------------------------------------------------------------------
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                                   IN const CSipPacket& rPacket,
                                                   IN EDirection eDirection);

    // Summary:
    //  Establishes the confidence level for which the service is a good
    //  candidate to own a request context.
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
    mxt_result AddSubscription(IN    const CToken&  rtokEvent,
                               IN    const CString& rstrId,
                               IN    unsigned int   uExpirationSec,
                               IN    EType          eType,
                               IN TO CGenParamList* pCustomParameters);

    struct SSubscription;

    // Summary:
    //  Returns a pointer to the subscription described by this ID. NULL if not
    //  found.
    //--------------------------------------------------------------------------
    SSubscription* GetSubscription(IN const CToken& rtokEvent,
                                   IN const CString& rstrId);

    // Summary:
    //  Returns a pointer to the subscription corresponding to the "id"
    //  parameter of the "Event" header in the header list.
    //------------------------------------------------------------------
    SSubscription* GetSubscription(IN const CHeaderList& rHdrs,
                                   OUT mxt_result* pRes = NULL);

    // Summary:
    //  Returns a pointer to the subscription that has this request context as
    //  its m_pSubscribeReqCtx data member.
    //-------------------------------------------------------------------------
    SSubscription* GetSubscription(IN const ISipRequestContext& rReqCtx);

    mxt_result SendSubscribeHelper(IN    const CToken&           rtokEvent,
                                   IN    mxt_opaque              opq,
                                   IN    const CString&          rstrId,
                                   IN TO CSipHeader*             pExpires,
                                   IN TO CHeaderList*            pExtraHeaders,
                                   IN TO CSipMessageBody*        pMessageBody,
                                   IN    CGenParamList*          pCustomParameters,
                                   OUT   ISipClientTransaction*& rpTransaction);

    void ProcessEventHelper(IN ISipRequestContext& rRequestContext,
                            IN unsigned int uEvent,
                            IN const CSipPacket* pPacket);

    GO CSipHeader* GetEventHelper(IN const CString& rstrId);

    // Summary:
    //   Resets the expiration of a subscription and handles related
    //   timers.
    //---------------------------------------------------------------
    void ResetExpiration(IN SSubscription* pstSubscription,
                         IN uint64_t uExpirationMs,
                         IN bool bAbsoluteTime = false);
    // Summary:
    //   This method is a helper that calls the Valdiate method on the UA.
    //---------------------------------------------------------------------
    mxt_result CheckUAConfiguration();

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipSubscriberSvc(IN const CSipSubscriberSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipSubscriberSvc& operator=(IN const CSipSubscriberSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    struct SSubscription
    {
        enum EDialogState
        {
            eNOT_IN_DIALOG,
            eESTABLISHING_DIALOG,
            eIN_DIALOG
        };

        // Timer ID used for this subscription.
        //----------------------------------------
        unsigned int m_uTimerId;

        // Indicates if the next timer event should be EvExpiring. 
        // This member is not strictly necessary because we can
        // infer this information from the current time and 
        // m_uExpirationTimeMs.
        // However, doing so could lead to EvExpiring being skipped 
        // in certain situations, like in overload conditions, in 
        // tests and when debugging (i.e., the overall behavior 
        // becomes less sensitive to timings).
        //--------------------------------------------------------------
        bool m_bEvExpiring;

        CToken m_tokEvent;

        CString m_strId;

        EType m_eType;

        bool m_bIdUsed;

        EDialogState m_eDialogState;

        uint64_t m_uExpirationTimeMs;
        uint64_t m_uExpiringThresholdMs;

        ISipRequestContext* m_pSubscribeReqCtx;

        bool m_bUnsubscribed;

        unsigned m_uRequestedExpiration;

        CGenParamList* m_pCustomParameters;

        SSubscription(IN    unsigned int   uTimerId,
                      IN    const CToken&  rtokEvent,
                      IN    const CString& rstrId,
                      IN    uint64_t       uExpiringThresholdMs,
                      IN    EType          eType,
                      IN TO CGenParamList* pCustomParameters)
        : m_uTimerId(uTimerId),
          m_bEvExpiring(false),
          m_tokEvent(rtokEvent),
          m_strId(rstrId),
          m_eType(eType),
          m_bIdUsed(false),
          m_eDialogState(eNOT_IN_DIALOG),
          m_uExpirationTimeMs(0),
          m_uExpiringThresholdMs(uExpiringThresholdMs),
          m_pSubscribeReqCtx(NULL),
          m_bUnsubscribed(false),
          m_uRequestedExpiration(0),
          m_pCustomParameters(pCustomParameters)
        {
        }

        ~SSubscription()
        {
            if (m_pSubscribeReqCtx != NULL)
            {
                // If the owner is not already NULL, this call will force the
                // associated transaction to timeout.
                //------------------------------------------------------------
                m_pSubscribeReqCtx->SetOwner(NULL);

                // Release the no longer needed request context service.
                //-------------------------------------------------------
                m_pSubscribeReqCtx->ReleaseIfRef();
                m_pSubscribeReqCtx = NULL;
            }
            MX_DELETE(m_pCustomParameters);
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
        eOTHER_EVENT
    };

    // This is the id of the timer used to report EvExpiring and EvExpired
    // events.
    //---------------------------------------------------------------------
    enum ETimerId
    {
        eTIMER_SUBSCRIPTION
    };
        
    ISipSubscriberMgr* m_pMgr;

    CVector<SSubscription*> m_vecSubscriptions;

    // Information about supported events.
    //---------------------------------------
    struct SEventInfo
    {
        unsigned int m_uDefaultExpirationSec;
        unsigned int m_uExpiringThresholdMs;

        SEventInfo(unsigned int uDefaultExpirationSec, 
                   unsigned int uExpiringThresholdMs = 0)
        :   m_uDefaultExpirationSec(uDefaultExpirationSec),
            m_uExpiringThresholdMs(uExpiringThresholdMs)
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

    // Next timer ID to be used for subscriptions.
    //------------------------------------------------
    unsigned int m_uNextTimerId;

    // The threshold type.
    //---------------------
    EThresholdType m_eThresholdType;
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSUBSCRIBERSVC_H
