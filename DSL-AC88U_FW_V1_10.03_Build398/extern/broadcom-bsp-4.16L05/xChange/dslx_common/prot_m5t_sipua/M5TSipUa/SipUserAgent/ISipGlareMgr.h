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
#ifndef MXG_ISIPGLAREMGR_H
#define MXG_ISIPGLAREMGR_H

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


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class ISipGlareSvc;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipGlareMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   The Glare manager is the interface through which the Glare core service
//   talks to the application.  The application is informed through this 
//   interface that it is time to reissue a request that was in glare if it is
//   still needed.
//
// Location:
//   SipUserAgent/ISipGlareMgr.h
//
// See Also:
//   ISipGlareSvc
//
//==EDOC========================================================================
class ISipGlareMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << Stereotype >>
    //---------------------

    //==SDOC====================================================================
    //==
    //==  EvReadyToRetry
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the application that it is time to retry a request that
    //      received a 491 Request Pending response.
    //
    //  Parameters:
    //    pGlareSvc:
    //      Interface on which the glare occurred.
    //
    //    bOtherRequestSeenSinceGlare:
    //      True when new, non-ACK requests have been sent or received on the
    //      context since the 491 response was received, false otherwise.
    //
    //  Description:
    //      Informs the application that the last request that received a 491
    //      response can be retried.
    //
    //      This event will be generated at a random time between 0 and 2
    //      seconds after the reception of a 491 when the context is not the
    //      owner of its Call-ID. This is the case when the context was a UAS
    //      for the first request on it.
    //
    //      When the context is the owner of the Call-ID (it was a UAC on the
    //      first request in the context), the event will occur at a random
    //      time between 2.1 and 4 seconds.
    //
    //      On reception of a 491 response to a sent request:
    //        - The service that issued the request generates a failure.
    //
    //          In the processing of the failure event, the application needs
    //          to identify that the response code is a 491 and take note
    //          of any information that can be relevant to the reissuing of the
    //          request at a later time. Examples of relevant information are
    //          the ISipClientControlEvent pointer and application state
    //          information.
    //
    //        - When EvReadyToRetry is generated, the application must decide if
    //          it still wants to send the request that glared. It can
    //          regenerate the request or optionally use the saved
    //          ISipClientEventControl::ReIssueRequest to do so. When
    //          bOtherRequestSeenSinceGlare is true, other requests have been
    //          exchanged on the context since the request in glare (the new
    //          requests may be related or not to the request in glare). In this
    //          case, it may be irrelevant to send the request again.
    //
    //      For instance, let's say that the application is sending a re-INVITE
    //      for a session timer refresh and that at the same time the remote is
    //      sending one to put the application on hold. The application
    //      generates a 491 for the received re-INVITE and the remote does the
    //      same. Assuming that the application is the owner of the Call-ID,
    //      the glare service starts a random timer between 2.1 to 4 seconds.
    //      While the local timer is elapsing, the remote retries
    //      its hold re-INVITE (its retry timer will be between 0 and 2
    //      seconds). In this particular scenario, when the local timer fires
    //      and EvReadyToRetry is generated, there would be no point in sending
    //      session-timer refresh INVITE since the context would have been
    //      refreshed by the received hold re-INVITE.
    //
    //==EDOC====================================================================
    virtual void EvReadyToRetry(IN ISipGlareSvc* pGlareSvc,
                                IN bool bOtherRequestSeenSinceGlare) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipGlareMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipGlareMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipGlareMgr(IN const ISipGlareMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipGlareMgr& operator=(IN const ISipGlareMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPGLAREMGR_H

