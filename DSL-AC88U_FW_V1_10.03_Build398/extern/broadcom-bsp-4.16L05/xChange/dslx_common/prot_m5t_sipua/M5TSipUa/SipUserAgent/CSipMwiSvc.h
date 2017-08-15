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
#ifndef MXG_CSIPMWISVC_H
#define MXG_CSIPMWISVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_MWI_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_MWI_SVC_SUPPORT to be able to \
use this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
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

#ifndef MXG_ISIPSUBSCRIBERMGR_H
#include "SipUserAgent/ISipSubscriberMgr.h"
#endif

#ifndef MXG_ISIPMWISVC_H
#include "SipUserAgent/ISipMwiSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CEComUnknown;
class IEComUnknown;
class ISipClientTransaction;
class ISipMwiMgr;
class CMessageSummary;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipMwiSvc
//=============================================
//
// Description:
//   Implementation of the ISipMwiSvc.
//
// Location:
//   UserAgentSvc/CSipMwiSvc.h
//
// See Also:
//   ISipRefererSvc
//
//==EDOC========================================================================
class CSipMwiSvc : private CEComDelegatingUnknown,
                   private ISipCoreSvc,
                   private ISipSubscriberMgr,
                   private ISipMwiSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:

    // Summary
    // Subscription state.
    //---------------------
    enum ESubscriptionState
    {
        eACTIVE,
        ePENDING,
        eTERMINATED,
        eINVALID,
        eNONE
    };

//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipMwiSvc();

    static void FinalizeCSipMwiSvc();


    //-- << ECOM mechanism >>
    //------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipMwiSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipMwiSvc();

    //-- << ECOM mechanism >>
    //------------------------
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << ISipCoreSvc Interface >>
    //--------------------------------

    // Summary:
    //  A pointer to the ISipReqCtxCoreSvc to attach to the request context.
    //  Can be NULL.
    //-----------------------------------------------------------------------
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

    //-- << ISipSubscriberMgr Interface >>
    //-------------------------------------

    // Summary:
    //   A provisional response (1xx) was received for a SUBSCRIBE request sent
    //   by the ISipSubscriberSvc.
    //--------------------------------------------------------------------------
    virtual void EvProgress(IN ISipSubscriberSvc*  pSvc,
                            IN ISipClientEventControl* pClientEventControl,
                            IN const CString&          rstrEvent,
                            IN const CString&          rstrId,
                            IN const CSipPacket&       rResponse);

    // Summary:
    //   A success response (2xx) was received for a SUBSCRIBE request sent by
    //   the ISipSubscriberSvc.
    //--------------------------------------------------------------------------
    virtual void EvSuccess(IN ISipSubscriberSvc*      pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString&          rstrEvent,
                           IN const CString&          rstrId,
                           IN const CSipPacket&       rResponse);

    // Summary:
    //   A failure (<= 3xx) was received for a SUBSCRIBE request sent by the
    //   ISipSubscriberSvc.
    //--------------------------------------------------------------------------
    virtual void EvFailure(IN ISipSubscriberSvc*      pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString&          rstrEvent,
                           IN const CString&          rstrId,
                           IN const CSipPacket&       rResponse);

    // Summary:
    //   An "Interval Too Small" (423) was received for a SUBSCRIBE request sent
    //   by the ISipSubscriberSvc.
    //--------------------------------------------------------------------------
    virtual void EvIntervalTooSmall(
                                 IN ISipSubscriberSvc*      pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN unsigned int            uMinExpirationSec,
                                 IN const CString&          rstrEvent,
                                 IN const CString&          rstrId,
                                 IN const CSipPacket&       rResponse);

    // Summary:
    //   A NOTIFY request was received for a valid active subscription.
    //------------------------------------------------------------------
    virtual void EvNotified(IN ISipSubscriberSvc*      pSvc,
                            IN ISipServerEventControl* pServerEventControl,
                            IN const CString&          rstrEvent,
                            IN const CString&          rstrId,
                            IN const CSipPacket&       rNotify);

    // Summary:
    //   A final NOTIFY was received for a valid active subscription.
    //----------------------------------------------------------------
    virtual void EvTerminated(IN ISipSubscriberSvc*      pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString&          rstrEvent,
                              IN const CString&          rstrId,
                              IN const CSipPacket&       rNotify);

    // Summary:
    //   An invalid NOTIFY was received.
    //-----------------------------------
    virtual void EvInvalidNotify(IN ISipSubscriberSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket&  rNotify,
                                 IN mxt_result         reason);

    // Summary:
    //   The subscription is about to expire.
    //----------------------------------------
    virtual void EvExpiring(IN ISipSubscriberSvc* pSvc,
                            IN const CString&     rstrEvent,
                            IN const CString&     rstrId);

    // Summary:
    //   The subscription is expired.
    //--------------------------------
    virtual void EvExpired(IN ISipSubscriberSvc* pSvc,
                           IN const CString&     rstrEvent,
                           IN const CString&     rstrId);

    //-- << ISipMwiSvc interface >>
    //-------------------------------

    // Summary:
    //   Configures the event manager associated with this service.
    //------------------------------------------------------------
    virtual mxt_result SetManager(IN ISipMwiMgr* pMgr);

    // Summary:
    //  Sets the time to warn the application before a subscription expires.
    //-----------------------------------------------------------------------
    virtual void SetExpiringThreshold(IN unsigned int uThresholdSec);

    // Summary:
    //   Subscribes to a message-box state.
    //--------------------------------------
    virtual mxt_result Subscribe(
                        IN    unsigned int            uExpirationSec,
                        IN    mxt_opaque              opqTransaction,
                        OUT   ISipClientTransaction*& rpTransaction,
                        IN TO CHeaderList*            pExtraHeaders = NULL);

    // Summary:
    //   Removes a subscription to a message-box state.
    //--------------------------------------------------
    virtual mxt_result Unsubscribe(IN mxt_opaque opqTransaction,
                                   OUT ISipClientTransaction*& rpTransaction,
                                   IN TO CHeaderList* pExtraHeaders = NULL);

    //-- << Helpers >>
    //------------------
    void EvNotifiedHelper(IN bool                    bTerminated,
                          IN ISipServerEventControl* pServerEventControl,
                          IN const CSipPacket&       rNotify);

    mxt_result AddAcceptHeader(INOUT CHeaderList& rExtraHeaders);

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipMwiSvc(IN const CSipMwiSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipMwiSvc& operator=(IN const CSipMwiSvc& rSrc);

    // Summary:
    // Return the subscription state from a SIP packet.
    //--------------------------------------------------
    const CSipMwiSvc::ESubscriptionState GetSubscriptionStateHelper(IN const CSipPacket& rNotify);

    // Summary:
    // Return the message summary from a SIP packet.
    //-----------------------------------------------
    mxt_result GetMessageSummaryHelper(IN const CSipPacket& rNotify,
                                       IN ISipServerEventControl* pServerEventControl,
                                       OUT GO CMessageSummary*& rpMessageSummary);

//-- Hidden Data Members
//------------------------
protected:
private:
    // Aggregated ISipSubscriberSvc. It will be responsible for sending
    // SUBSCRIBE and receiving NOTIFY requests.
    //------------------------------------------------------------------
    IEComUnknown* m_pSubscriberSvc;

    ISipMwiMgr* m_pMgr;

    // Whether the first NOTIFY was received or not.
    //-----------------------------------------------
    bool m_bInitialSynchronization;

    // Whether the first SUBSCRIBE was sent or not.
    //----------------------------------------------
    bool m_bSubscribed;

    // States whether a final response has been received for a subscription
    // refresh.
    //----------------------------------------------------------------------
    bool m_bRefreshing;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSIPMWISVC_H

