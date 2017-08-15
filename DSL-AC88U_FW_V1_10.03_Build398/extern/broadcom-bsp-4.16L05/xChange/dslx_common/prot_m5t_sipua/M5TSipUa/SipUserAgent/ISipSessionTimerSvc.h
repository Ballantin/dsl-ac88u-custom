//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2001 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPSESSIONTIMERSVC_H
#define MXG_ISIPSESSIONTIMERSVC_H

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
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class ISipSessionTimerMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipSessionTimerSvc);
//M5T_INTERNAL_USE_END

const unsigned int uDEFAULT_MIN_SE_SEC = 90;
const unsigned int uDEFAULT_SESSION_EXPIRES_SEC = 1800;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipSessionTimerSvc
//=============================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The session-timer service is used to give a keep alive mechanism to the
//  session described in the parent context.  It negotiates a session
//  duration by inspecting and modifying INVITE requests and their responses
//  (sent by other services).  It then warns the application (through the
//  ISipSessionTimerMgr interface) when it is time to refresh the session in
//  order to keep it alive or when it is time to tear down the session because
//  it has expired.
//
//  This implementation is compliant with RFC 4028.
//
//  This service manager generates events to the application through the
//  ISipSessionTimerMgr interface.
//
// #The ISipSessionTimerSvc is an ECOM object#
//
//  The ISipSessionTimerSvc is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipSessionTimerSvc</B>
//
//  <B>Interface Id: IID_ISipSessionTimerSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipSessionTimerSvc.h
//
//==============================================================================
class ISipSessionTimerSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipSessionTimerSvc);
    //M5T_INTERNAL_USE_END

    // Description:
    //  Enum to determine who must refresh the session.
    enum ERefresher
    {
        // Description:
        //  No one will refresh.
        eNONE,
        // Description:
        //  The local entity will refresh the session.
        eLOCAL,
        // Description:
        //  The remote end will refresh the session.
        eREMOTE
    };

    //-- << Stereotype >>
    //---------------------

    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the event manager associated with this service.
    //
    //  Parameters:
    //    pMgr:
    //      The event manager. It must not be NULL.
    //
    //  Description:
    //      Configures the event manager that will receive the events generated
    //      by this service.
    //
    //      Note that an event manager MUST be associated with this service
    //      before it is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipSessionTimerMgr* pMgr) = 0;


    //==========================================================================
    //==
    //==  SetSessionTimerDemanded
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets whether or not Session-Expires and Min-SE headers must be added
    //      to requests.
    //
    //  Parameters:
    //    bSEDemanded:
    //      True if the application wants to add a Session-Expires header to
    //      requests, false if it just wants to signal that the session timer
    //      capability is supported.
    //
    //    bMinSE:
    //      True if the application wants to add a Min-SE header to requests,
    //      false otherwise.
    //
    //  Description:
    //      This method can be used to set whether or not Session-Expires and 
    //      Min-SE headers must be added to requests.
    //
    //==========================================================================
    virtual void SetSessionTimerDemanded(IN bool bSEDemanded    = false,
                                         IN bool bMinSEDemanded = false) = 0;

    //==========================================================================
    //==
    //==  SetSessionExpiresSec
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the Session-Expires required value.
    //
    //  Parameters:
    //    uSessionExpiresSec:
    //      The maximum required amount of time (in seconds) after which the
    //      session should be considered expired if not refreshed. Default to 30
    //      minutes.
    //
    //  Returns:
    //    resSW_WARNING:
    //      uSessionExpiresSec is lower than the required Min-SE value.
    //
    //    resS_OK:
    //      Otherwise.
    //
    //  Description:
    //      This method can be used to set the Session-Expires required value.
    //      If the value passed in parameter is lower than the required Min-SE
    //      value (set with SetMinSESec), it is set to the Min-SE value.
    //      Otherwise, it is set to the value passed in parameter.
    //
    //  Note:
    //      If SetMinSESec() is called after this method with a minimal value
    //      lower than uSessionExpiresSec, the Session-Expires value will be
    //      changed to the new minimum. For that reason, it is reccomended to
    //      call this method afterSetMinSESec().
    //
    //  See Also:
    //      SetMinSESec
    //
    //==========================================================================
    virtual mxt_result SetSessionExpiresSec(
                               IN unsigned int uSessionExpiresSec = uDEFAULT_SESSION_EXPIRES_SEC) = 0;

    //==========================================================================
    //==
    //==  SetMinSESec
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the Min-SE required value.
    //
    //  Parameters:
    //    uMinSESec:
    //      The minimum required amount of time (in seconds) after which the
    //      session should be considered expired if not refreshed.  Default to
    //      90 seconds. It cannot be less than 90 seconds.
    //
    //  Returns:
    //    resSW_WARNING:
    //      uMinSESec is higher than the required Session-Expires 
    //      value or lower than 90 seconds.
    //
    //    resS_OK:
    //      Otherwise.
    //
    //  Description:
    //      This method can be used to set the Min-SE required value.  If the
    //      value passed in parameter is higher than the required Session-Expires
    //      value (set with SetSessionExpiresSec), the Session-Expires value is
    //      set to the new minimum.  If the value is lower than 90 seconds, it
    //      is set to 90 seconds.
    //
    //  Note:
    //      This method should be called before SetSessionExpiresSec() since 
    //      setting the minimum value has an impact on the validity of the
    //      Session-Expires value.
    //
    //  See Also:
    //      SetSessionExpiresSec
    //
    //==========================================================================
    virtual mxt_result SetMinSESec(IN unsigned int uMinSESec = uDEFAULT_MIN_SE_SEC) = 0;

    //==========================================================================
    //==
    //==  SetRefresher
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the required refresher.
    //
    //  Parameters:
    //    eRefresher:
    //      The party the application wants to act as the refresher, if any.
    //
    //  Description:
    //      This method can be used to set the required refresher.
    //
    //==========================================================================
    virtual void SetRefresher(ERefresher eRefresher = eNONE) = 0;

    //==========================================================================
    //==
    //==  GetSessionExpiresSec
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the current Session-Expires value.
    //
    //  Returns:
    //      The current maximum amount of time (in seconds) after which the
    //      session should be considered expired if not refreshed.
    //
    //  Description:
    //      This method can be used to get the current Session-Expires value.
    //
    //==========================================================================
    virtual unsigned int GetSessionExpiresSec() = 0;

    //==========================================================================
    //==
    //==  GetMinSESec
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the current Min-SE value.
    //
    //  Returns:
    //      The current minimum amount of time (in seconds) after which the
    //      session should be considered expired if not refreshed.
    //
    //  Description:
    //      This method can be used to get the Min-SE current value.
    //
    //==========================================================================
    virtual unsigned int GetMinSESec() = 0;

    //==SDOC====================================================================
    //==
    //==  SetExpirationThresholds
    //==
    //==========================================================================
    // <FLAG Updated behavior in 4.1.4>
    //
    //  Summary:
    //      Sets thresholds before which the application is notified of
    //      upcoming or effective session expiration.
    //
    //  Parameters:
    //    uRefreshThresholdSec:
    //      The time before expiration at which EvSessionMustRefresh
    //      is called (in seconds). 0 means to keep the default.
    //    uExpiredThresholdSec:
    //      The time before expiration at which EvSessionExpired
    //      is called (in seconds). 0 means to keep the default.
    //
    //  Returns:
    //    resFE_INVALID_ARGUMENT:
    //      uExpiredThresholdSec is greater than or equal to 
    //      uRefreshThresholdSec.
    //    resFE_INVALID_STATE:
    //      If the manager is not set.
    //
    //  Description:
    //      Sets the amount of time that should remain to a session before
    //      EvSessionMustRefresh and EvSessionExpired are called for that 
    //      session.
    //
    //      Note this service will notify its manager of required 
    //      refreshes only if the refresher is local (via 
    //      EvSessionMustRefresh). Yet, it will notify it of expirations 
    //      (via EvSessionExpired) regardless of the refresher status.
    //
    //      Note that in RFC 4028 it is RECOMMENDED, regarding refreshes 
    //      and expiration:
    //      - Refreshes (Section 7.2): should be sent after one half the 
    //        session interval has elapsed, by the refresher.
    //      - Expiration (Section 10): BYE should be sent one third of the 
    //        session interval or 32 seconds, whichever is lower, before 
    //        expiration, by the side not performing refreshes.
    //
    //      Thus, by default EvSessionMustRefresh will be called after
    //      half the session interval has elapsed but only if the 
    //      refresher is local.
    //      EvSessionExpired will be called after two thirds of the session 
    //      interval has elapsed if it is 96 seconds or less. If it is 
    //      more than 96 seconds, EvSessionExpired will be called 32 seconds 
    //      before expiration.
    //      This is in line with RFC 4028 and should be 
    //      adequate for most applications.
    //
    //      However, this method is provided to modify this default 
    //      behaviour. The parameters are used to tell the service
    //      how long before expiration it wants to be notified.
    //
    //      Changing expiration thresholds does not affect the current
    //      session, if any is already established. It affects the 
    //      next session established.
    //
    //  Updated behavior:
    //      Previously, EvSessionExpired was fired by default after
    //      two thirds of the session interval, regardless of the 32
    //      seconds rule. For instance, a session with an interval of
    //      180 seconds would receive EvSessionExpired after 120 seconds
    //      and not 148 seconds. This has been fixed and the behaviour is 
    //      now aligned with RFC 4028.
    //
    //  See Also:
    //      ISipSessionTimerMgr::EvSessionMustRefresh, 
    //      ISipSessionTimerMgr::EvSessionExpired 
    //
    //==EDOC====================================================================
    virtual mxt_result SetExpirationThresholds(IN unsigned int uRefreshThresholdSec,
                                               IN unsigned int uExpiredThresholdSec) = 0;

    //==========================================================================
    //==
    //==  Reset
    //==
    //==========================================================================
    //
    //  Summary:
    //      Resets the service to its initial values.
    //
    //  Description:
    //      Resets the service to its initial values.  Useful when the
    //      application wants to renegotiate session expiration when refreshing
    //      the session.
    //
    //==========================================================================
    virtual void Reset() = 0;

    //==========================================================================
    //==
    //==  ResetSessionTimer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Resets the session timer to avoid generating EvSessionExpired at 2/3
    //      of the negotiated session-expires.
    //
    //  Returns:
    //      - resFE_UNEXPECTED: The refresher is not set or is remote.
    //      - resFE_INVALID_STATE: The function is called before the session-expires
    //                             parameters are negotiated or after the context
    //                             has been cleared.
    //      - resS_OK: The function has restarted the session timer
    //                 successfully.
    //
    //  Description:
    //      Re-starts the session timers using the values negotiated when the
    //      last 2xx response was received.
    //
    //      This method should be called only when a final negative response is being
    //      received to an INVITE or UPDATE sent to refresh the session timers. In such
    //      case, the event EvFailure is triggered on the ISipSessionMgr.
    //
    //      This feature allows to handle specific scenarios, such as receiving
    //      a 488 during a session timer refresh. Calling this method will avoid
    //      generating the event EvSessionExpired at 2/3 of the negotiated
    //      session-expires values.
    //
    //      This method only re-starts the timers when at least one 2xx to an
    //      INVITE has been received. It fails if called on the non-refresher side
    //      or after that the context has been cleared.
    //
    //==========================================================================
    virtual mxt_result ResetSessionTimer() = 0;

    // Summary:
    //  Possible behaviour to set with SetMissingSessionExpiresInAnswerBehaviour.
    //
    // Description:
    //  Indicates the behaviour when receiving a 200 OK without the Session-Expires
    //  header.
    enum EMissingSessionExpireBehaviour
    {
        // Description:
        //  200 OK without Session-Expires will always refresh the session.
        eRefreshSession,

        // Description:
        //  200 OK without Session-Expires stops the timer. Hence no more 
        //  EvSessionMustRefresh and EvSessionExpired are reported.
        eStopSessionTimer    
    };

    //==============================================================================
    //==
    //==  SetMissingSessionExpiresInAnswerBehaviour
    //==
    //==============================================================================
    //
    //  Parameters:
    //      eBehaviour:
    //       - eRefreshSession: 200 OK without Session-Expires will always refresh
    //                          the session
    //       - eStopSessionTimer: 200 OK without Session-Expires stops the timer.
    //                            Hence no more EvSessionMustRefresh and 
    //                            EvSessionExpired are reported.
    //
    //  Description:
    //      If eRefreshSession is passed, the service will reset the timer (hence
    //      reports EvMustRefresh when the timer fires) on the reception of all
    //      200 OK. If eStopSessionTimer, the timer will be reset only if the
    //      200 OK contains the header Session-Expires. By default, the value is
    //      eRefreshSession.
    //
    //==============================================================================
    virtual void SetMissingSessionExpiresInAnswerBehaviour(IN EMissingSessionExpireBehaviour eBehaviour) = 0;

// Hidden Methods
//-------------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipSessionTimerSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipSessionTimerSvc() {}

private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipSessionTimerSvc(IN const ISipSessionTimerSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipSessionTimerSvc& operator=(IN const ISipSessionTimerSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPSESSIONTIMERSVC_H

