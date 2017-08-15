//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2001-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPSESSIONTIMERMGR_H
#define MXG_ISIPSESSIONTIMERMGR_H

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


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipClientEventControl;
class ISipServerEventControl;
class ISipSessionTimerSvc;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipSessionTimerMgr
//=============================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The session timer manager is the interface through which the session timer
//  service talks to the application. The SIP stack notifies the application 
//  through this interface that a session needs to be refreshed or that it has 
//  expired.
//
// Location:
//   SipUserAgent/ISipSessionTimerMgr.h
//
// See Also:
//  ISipSessionTimerSvc
//
//==============================================================================
class ISipSessionTimerMgr
{
//-- Friend Declarations
//-------------------------

//-- Published Interface
//-------------------------
public:
    //==========================================================================
    //==
    //==  void EvSessionIntervalTooShortSent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that the refresh rate requested by the 
    //      remote party for this session is too short.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the response.
    //
    //    opqApplicationData:
    //      Data set by the application in another event for this same request.
    //      When this is the first event for this request, it is set to 0.
    //
    //    rRequest:
    //      The response.
    //
    //  Description:
    //      Notifies the application that the session interval requested by the
    //      remote party was shorter than the configured minimum and that, as a
    //      result, the service has issued a 422 response code through the
    //      request context associated with the transaction.  Therefore, there's
    //      no need for the application to send a response, it has already been
    //      sent by the service using ISipServerEventCtrl::SendResponse.
    //
    //  See Also:
    //      ISipSessionTimerSvc, ISipServerEventControl::SendResponse
    //
    //==========================================================================
    virtual void EvSessionIntervalTooShortSent(
                                       IN ISipSessionTimerSvc* pSvc,
                                       IN mxt_opaque           opqApplicationData,
                                       IN const CSipPacket&    rRequest) = 0;

    //==========================================================================
    //==
    //==  void EvSessionIntervalTooShortRecv
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a 422 response has been received, 
    //      meaning that the refresh rate requested by the application for this 
    //      session is too short for the remote party.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rRequest:
    //      The response.
    //
    //  Description:
    //      Notifies the application that a 422 response has been received, 
    //      meaning that the refresh rate requested by the application for this 
    //      session is too short for the remote party.  The application must 
    //      therefore issue another INVITE or UPDATE request with a higher 
    //      Session-Expires value.
    //
    //  See Also:
    //      ISipSessionTimerSvc, ISipSessionTimerSvc::SetSessionExpiresSec
    //
    //==========================================================================
    virtual void EvSessionIntervalTooShortRecv(
                                   IN ISipSessionTimerSvc*    pSvc,
                                   IN ISipClientEventControl* pClientEventCtrl,
                                   IN const CSipPacket&       rRequest) = 0;

    //==========================================================================
    //==
    //==  void EvSessionMustRefresh
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that it must refresh the session.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the response.
    //
    //  Description:
    //      Notifies the application that the session needs to be refreshed. If
    //      session-timer negotiation has determined that the local user-agent
    //      was responsible for doing the refreshes, this method is called
    //      when half the time negotiated for the session duration has elapsed.
    //
    //      In order to keep the session alive, when this method is called, the
    //      application should send an INVITE or an UPDATE request to the remote
    //      user agent.
    //
    //  See Also:
    //      ISipSessionTimerSvc
    //
    //==========================================================================
    virtual void EvSessionMustRefresh(IN ISipSessionTimerSvc* pSvc) = 0;

    //==========================================================================
    //==
    //==  void EvSessionExpired
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that it should consider the session expired.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the response.
    //
    //  Description:
    //      Notifies the application that the session should be considered
    //      expired. This method is called when two thirds of the time
    //      negotiated for the session has elapsed. When this method is called,
    //      it means that the user-agent responsible to do refreshes (either
    //      the local or the remote) failed to send the INVITE or UPDATE request
    //      in time.
    //
    //      The application should tear down the session (by releasing the
    //      context.)
    //
    //  See Also:
    //      ISipSessionTimerSvc
    //
    //==========================================================================
    virtual void EvSessionExpired(IN ISipSessionTimerSvc* pSvc) = 0;

//-- Hidden Methods
//-------------------------
protected:
    //-- << Constructors >>
    //--------------------
    ISipSessionTimerMgr() {}

    //-- << Destructors >>
    //-------------------
    virtual ~ISipSessionTimerMgr() {}

private:

    //-- Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    ISipSessionTimerMgr(const ISipSessionTimerMgr& rSrc);
    ISipSessionTimerMgr& operator=(const ISipSessionTimerMgr& rSrc);

//-- Hidden Data Members
//-------------------------
protected:
private:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPSESSIONTIMERMGR_H

