//==============================================================================
//==============================================================================
//
//      Copyright(c) 2001 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPSESSIONTIMERSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSESSIONTIMERSVC_H
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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_TIMER_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_SESSION_TIMER_SVC_SUPPORT to be \
able to use this class.
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

#ifndef MXG_ISIPSESSIONTIMERSVC_H
#include "SipUserAgent/ISipSessionTimerSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class ISipSessionTimerMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipSessionTimerSvc
//=============================================
//
//  Description:
//      The CSipSessionTimerSvc is the class that implements the interface of
//      ISipSessionTimerSvc. The application has to create an instance of
//      CSipSessionTimerSvc via the SIP context when it wants to add an
//      ISipSessionTimerSvc interface to a context.
//
//      This service allows the application to manage the session timer
//      capability. It takes care of Session-Expires and Min-SE headers in
//      requests and responses. It generates events to the application when
//      required.
//
//      When used with the client behaviour, it updates requests through
//      UpdatePacket. A header Supported containing the value "timer" is
//      systematically added.  In INVITE and UPDATE requests, Min-SE and
//      Session-Expires are added if necessary from the rules of RFC 4028
//      or because the application wants to do so - it can set this
//      preference through the SetSessionTimerDemanded method. The application 
//      may set a preference for the refresher through SetRefresher, but the
//      refresher parameter is not added to the Session-Expires headers as it is
//      not recommended by the RFC.
//
//      Responses are processed in OnPacketReceived. 422 and 200 responses 
//      update session-timer values when applicable. A 200 response also
//      triggers the start of a timer, which fires after half of the current
//      Session-Expires value if the refresher is the application, or two thirds
//      of the same value if the refresher is the remote party, except if the 
//      last request sent was other than INVITE or UPDATE. If a BYE was  
//      previously sent, the timer was stopped when updating the request through
//      UpdatePacket.
//
//      When used with the server behaviour, requests are processed in
//      OnPacketReceived. In INVITE and UPDATE requests, Min-SE and Session-
//      Expires, if present, are compared to current settings and the latter
//      updated if applicable.  If Session-Expires is lower than the required
//      Min-SE value set by the application using SetMinSESec (default 90
//      seconds), a 422 response is automatically returned by the service and an
//      event signaled to the application. If the Supported header is not
//      present, the service manages the session-timer capability and acts as 
//      the refresher if the application wants to do so - again, it can set this
//      preference through the SetSessionTimerDemanded method - or if there is a
//      Session-Expires header in the request, which means that a proxy wants to
//      use the session-timer capability.
//
//      200 responses are updated through UpdatePacket. As for the 200 responses
//      received, those sent also trigger the start of timers that follow the
//      same rule: fire after half of the current Session-Expires value if the
//      refresher is the application, or two thirds of the same value if the 
//      refresher is the remote party, except if the last request sent was other 
//      than INVITE or UPDATE. If a BYE was previously received, the timer was 
//      stopped upon receiving the request.
//
//      When the timer fires, if the refresher is the application, the timer is
//      restarted until two-thirds of Session-Expires - so that the session is
//      considered expired if it has not been refreshed beforehand - and an
//      EvSessionMustRefresh event is sent to the application. It is the
//      latter's responsibility to send a re-INVITE or an UPDATE request in
//      order to refresh the session.
//
//      If the remote party is the refresher and the session has not been 
//      refreshed on time, an EvSessionExpires event is sent to the application.
//      It is the latter's responsibility to send a BYE request.
//
//      When, after any request, a 408 or 481 response is received, the session
//      is considered expired.  An EvSessionExpired event is then sent to the
//      application and the timer is stopped.
//
//      This implementation is compliant with RFC 4028.
//
//  Location:
//      SipUserAgent/CSipSessionTimerSvc.h
//
//  See Also:
//      ISipCoreSvc, ISipUserAgentSvc
//
//==============================================================================
class CSipSessionTimerSvc : private CEComDelegatingUnknown,
                            private CEventDriven,
                            private ISipCoreSvc,
                            private CSipReqCtxCoreSvc,
                            private ISipSessionTimerSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:
//M5T_INTERNAL_USE_BEGIN
    // Summary:
    //   Internal method that sets the minimum Min-SE. This method SHOULD NOT
    //   be used. Its use is at the caller's risks. As per RFC3261, the value
    //   is supposed to be 90 seconds (which it is by default).
    //--------------------------------------------------------------------------
    static void ForceMinMinSESec(IN unsigned int uMinMinSESec);
//M5T_INTERNAL_USE_END


//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipSessionTimerSvc();

    static void FinalizeCSipSessionTimerSvc();


    //-- << ECOM mechanism >>
    //------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << ISipSessionTimerSvc interface >>
    //--------------------------------------------

    // Summary:
    //   Configures the event manager associated with this service.
    //-------------------------------------------------------------
    virtual mxt_result SetManager(IN ISipSessionTimerMgr* pMgr);

    // Summary:
    //   Sets whether or not Session-Expires and Min-SE headers must be added
    //   to requests.
    //--------------------------------------------------------------------
    virtual void SetSessionTimerDemanded(IN bool bSEDemanded    = false,
                                         IN bool bMinSEDemanded = false);

    // Summary:
    //   Configures the Session-Expires required value.
    //-------------------------------------------------
    virtual mxt_result SetSessionExpiresSec(
               IN unsigned int uSessionExpires = uDEFAULT_SESSION_EXPIRES_SEC);

    // Summary:
    //   Configures the Min-SE required value.
    //----------------------------------------
    virtual mxt_result SetMinSESec(IN unsigned int uMinSE = uDEFAULT_MIN_SE_SEC);

    // Summary:
    //   Configures the refresher required value.
    //-------------------------------------------
    virtual void SetRefresher(ERefresher eRefresher = eNONE);


    // Summary:
    //   Gets the current Session-Expires value.
    //-------------------------------------------
    virtual unsigned int GetSessionExpiresSec();

    // Summary:
    //   Gets the current Min-SE value.
    //----------------------------------
    virtual unsigned int GetMinSESec();

    //  Summary:
    //      Sets thresholds before which the application is notified of
    //      upcoming or effective session expiration.
    //---------------------------------------------------------------------
    virtual mxt_result SetExpirationThresholds(IN unsigned int uRefreshThresholdSec,
                                               IN unsigned int uExpiredThresholdSec);
    // Summary:
    //   Resets the service to its initial values.
    //---------------------------------------------
    virtual void Reset();

    // Summary:
    //   Resets the session timer when a 488 response is received to a
    //   refresh request if called before EvSessionExpired() is reported.
    //------------------------------------------------------------------
    virtual mxt_result ResetSessionTimer();

    virtual void SetMissingSessionExpiresInAnswerBehaviour(IN EMissingSessionExpireBehaviour eBehaviour);

    //-- << ECOM mechanism >>
    //-------------------------
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- << CEventDriven >>
    //-----------------------
    virtual void EvTimerServiceMgrAwaken(IN bool         bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque   opq);

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //   Default Constructor.
    //------------------------
    CSipSessionTimerSvc(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //   Destructor.
    //---------------
    virtual ~CSipSessionTimerSvc();

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
    //--------------------------------------------------------------------
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                                  IN const CSipPacket& rPacket);

    // Summary:
    //   Returns the ISipReqCtxCoreSvc instance to use as the owner for a 
    //   request context.
    //--------------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();

    //-- << ISipReqCtxCoreSvc >>
    //---------------------------

    // Summary:
    //   Notifies the request context core service of a received packet.
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
    //   Lets the service clear the state associated with an event in which
    //   the application has no interest.
    //-------------------------------------------------------------------
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

    //-- << Helpers >>
    //------------------

    // Summary:
    //   Adds a Min-SE header to a list of headers.
    //----------------------------------------------
    void AddMinSEHeader(CHeaderList& rHeaderList);

    // Summary:
    //   Adds a Require header to a list of headers.
    //-----------------------------------------------
    void AddRequireHeader(CHeaderList& rHeaderList);

    // Summary:
    //   Adds a Session-Expires header to a list of headers.
    //-------------------------------------------------------
    void AddSessionExpiresHeader(CHeaderList& rHeaderList,
                                 const char*  szRefresher);

    // Summary:
    //   Adds a Supported header to a list of headers.
    //-------------------------------------------------
    void AddSupportedHeader(CHeaderList& rHeaderList);

    // Summary:
    //   Handles expiration timers.
    //-------------------------------------------
    void ResetExpirationTimers(IN unsigned int uExpirationSec);

    // Summary:
    //   Helper to calculate timer delays.
    //-------------------------------------------
    unsigned int CalculateTimerDelay(
                            IN unsigned int uExpirationMs,
                            IN unsigned int uThresholdMs,
                            IN bool bRefresh);


    // Summary:
    //  Returns the corresponding string of the role that correspond to the
    //  state.
    const char* GetRefresherString(ERefresher eRefresher);

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //   Copy Constructor.
    //---------------------
    CSipSessionTimerSvc(IN const CSipSessionTimerSvc& rSrc);

    // Summary:
    //   Assignment Operator.
    //------------------------
    CSipSessionTimerSvc& operator=(IN const CSipSessionTimerSvc& rSrc);


// Hidden Data Members
//-------------------------
protected:
    static const unsigned int uREFRESH_TIMER_ID;
    static const unsigned int uEXPIRED_TIMER_ID;

    // Enum kept to manage incoming events from the network.
    //-------------------------------------------------------
    enum ESessionTimerEvent
    {
        eSESS_INTERVAL_TOO_SHORT_RECV,
        eSESS_INTERVAL_TOO_SHORT_SENT
    };

    // The event manager.
    //--------------------
    ISipSessionTimerMgr* m_pMgr;

    // The application's choice of refresher.
    //----------------------------------------
    ERefresher m_eDesiredRefresher;

    // The current (negotiated) refresher.
    //-------------------------------------
    ERefresher m_eCurrentRefresher;

    // Current (negotiated) values (in seconds).
    //-------------------------------------------
    unsigned int m_uCurrentSESec;
    unsigned int m_uCurrentMinSESec;

    // Whether there's a current value of the corresponding variables.
    //-----------------------------------------------------------------
    bool m_bCurrentSE;
    bool m_bCurrentMinSE;

    // True if remote party supports session-timer.
    //----------------------------------------------
    bool m_bSupported;

    // Expirations thresholds.
    //----------------------------
    unsigned int m_uRefreshThresholdMs;
    unsigned int m_uExpiredThresholdMs;

    // Tells if the Session-Expires header is necessary in order to refresh
    // the timer.
    EMissingSessionExpireBehaviour m_eMissingSessionExpiresInAnswer;

private:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline void CSipSessionTimerSvc::SetMissingSessionExpiresInAnswerBehaviour(IN EMissingSessionExpireBehaviour eBehaviour)
{
    m_eMissingSessionExpiresInAnswer = eBehaviour;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSIPSESSIONTIMERSVC09_H

