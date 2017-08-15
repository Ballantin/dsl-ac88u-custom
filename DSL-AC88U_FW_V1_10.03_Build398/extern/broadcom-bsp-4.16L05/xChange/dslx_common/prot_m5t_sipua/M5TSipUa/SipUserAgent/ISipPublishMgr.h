//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPPUBLISHMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPPUBLISHMGR_H
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
// MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT to be \
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
class ISipPublishSvc;
class ISipClientEventControl;
class CSipPacket;
class CString;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipPublishMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//
//  The Publish Manager is the interface through which the publish service talks
//  to the application. The ISipPublishSvc informs the application through this
//  interface of the progress of the PUBLISH requests it has sent.
//
// Location:
//   SipUserAgent/ISipPublishMgr.h
//
// See Also:
//   ISipPublishSvc
//
//==EDOC========================================================================
class ISipPublishMgr
{
//-- Friend Declarations
//------------------------

//-- New types within class namespace
//-----------------------------------
public:
protected:
private:

//-- Published Interface
//------------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      A provisional response (1xx) to a PUBLISH request is received.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipPublishSvc that is managing this response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rResponse:
    //      The response packet.
    //
    //  Description:
    //      Notifies the publish manager that a provisional response has been
    //      received for the last outgoing PUBLISH request sent through the
    //      publish service.
    //
    //      The state of the publish service does not change after the reception
    //      of this provisional response: it still waits for a final response.
    //
    //==EDOC====================================================================
    virtual void EvProgress(IN ISipPublishSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      A success response (2xx) to a PUBLISH request has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipPublishSvc that is managing this response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rResponse:
    //      The response packet.
    //
    //  Description:
    //      Notifies the publish manager that a success response has been
    //      received for the last outgoing PUBLISH request sent through the
    //      publish service.
    //
    //      After this success response, the publish service keeps track of the
    //      entity-tag and warns the manager when the publication is about to
    //      expire and when it has expired.
    //
    //==EDOC====================================================================
    virtual void EvSuccess(IN ISipPublishSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      A failure response (>= 300) to a PUBLISH request has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipPublishSvc that is managing this response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rResponse:
    //      The response packet.
    //
    //  Description:
    //      Notifies the publish manager that a failure response has been
    //      received for the last outgoing PUBLISH request sent through the
    //      publish service.
    //
    //      After this failure response, the publish service acts as if the
    //      last PUBLISH request was not sent.
    //
    //==EDOC====================================================================
    virtual void EvFailure(IN ISipPublishSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvExpiresIntervalTooBrief
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a 423 final negative response has been
    //      received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    uExpiresS:
    //      The minimal expiration time accepted by the server.
    //
    //    rResponse:
    //      The actual response.
    //
    //  Description:
    //      Notifies the publish manager that a 423 response has been received,
    //      meaning that the requested publication expiration time is too brief
    //      for the server. The application must therefore issue another Publish
    //      request with a higher Expires value.
    //
    //      When reporting this event, the ISipPublishSvc will have adjusted the
    //      application requested expiration time to be equal to the minimal
    //      expiration time accepted by the server. The stack user receiving
    //      this event has the choice to abandon the publication, try it again
    //      with the minimal expiration time by calling ReIssueRequest on
    //      pClientEventCtrl, or call again Publish with the expiration time it
    //      sees fit.
    //
    //  See Also:
    //      ISipPublishSvc::Publish, ISipClientEventControl::ReIssueRequest
    //
    //==========================================================================
    virtual void EvExpiresIntervalTooBrief(
                                   IN ISipPublishSvc* pSvc,
                                   IN ISipClientEventControl* pClientEventCtrl,
                                   IN unsigned int uExpiresS,
                                   IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvConditionalRequestFailed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a 412 final negative response has been
    //      received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    uExpiresS:
    //      The value of the Expires header.
    //
    //    rResponse:
    //      The actual response.
    //
    //  Description:
    //      Notifies the publish manager that a 412 final negative response has
    //      been received, indicating that the precondition given for the 
    //      request has failed. This means there is no matching event state at
    //      the ESC to be refreshed, modified, or removed. The EPA SHOULD 
    //      perform an initial publication. Before reporting this event, the 
    //      publish service clears its entity-tag.
    //
    //  See Also:
    //      ISipPublishSvc::Publish, ISipClientEventControl::ReIssueRequest
    //
    //==========================================================================
    virtual void EvConditionalRequestFailed(
                                   IN ISipPublishSvc* pSvc,
                                   IN ISipClientEventControl* pClientEventCtrl,
                                   IN unsigned int uExpiresS,
                                   IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvProtocolError
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that an invalid 200 response has been 
    //      received for a publication.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rResponse:
    //      The actual response.
    //
    //  Description:
    //      Notifies the publish manager that an invalid 2xx response has been
    //      received. This response either contains no SIP-ETag header or
    //      multiple headers of such type. According to the SIP specification,
    //      it is a must to have one and only one SIP-ETag header per 2xx
    //      response.
    //
    //      The publish service clears its entity-tag and stops any timer
    //      (related to EvExpiring and EvExpired) it holds before reporting this
    //      event.
    //
    //  See Also:
    //      ISipPublishSvc::Publish, ISipClientEventControl::ReIssueRequest
    //
    //==========================================================================
    virtual void EvProtocolError(IN ISipPublishSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventCtrl,
                                 IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvExpiring
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that the publication is about to expire.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipPublishSvc that is managing the contacts.
    //
    //  Description:
    //      Notifies the publish manager that the publication is about to expire.
    //      If the manager wants to keep this publication active on the server,
    //      it should refresh it by calling the Refresh method of the publish
    //      service. If the manager fails to do so, EvExpired is issued later,
    //      the amount of time being dependent on
    //      ISipPublishSvc::SetExpiringThreshold's configuration.
    //
    //  See Also:
    //      ISipPublishSvc, EvExpired, Refresh
    //==========================================================================
    virtual void EvExpiring(IN ISipPublishSvc* pSvc) = 0;

    //==========================================================================
    //==
    //==  EvExpired
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that the publication has expired.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipPublishSvc that is managing the publication.
    //
    //  Description:
    //      Notifies the publish manager that the publication managed by the
    //      publish service has expired. The manager should consider that this
    //      entity-tag is no longer published to the server. Usually, the
    //      manager should not let that event occur. If it wants to remove a
    //      publication, it should not wait until its expiration, but rather
    //      explicitly remove it with the Remove method of the publish service
    //      interface.
    //
    //      If the manager wants to keep the publication to the server, it
    //      should publish it again by calling the Publish method of the publish
    //      service. This event should only occur when the manager was unable to
    //      refresh or remove its publication due to some remote server error
    //      and when the publication was received with a smaller expiration time
    //      than the threshold of the publish service.
    //
    //  See Also:
    //      ISipPublishSvc, EvExpiring
    //==========================================================================
    virtual void EvExpired(IN ISipPublishSvc* pSvc) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipPublishMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipPublishMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipPublishMgr(IN const ISipPublishMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipPublishMgr& operator=(IN const ISipPublishMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPPUBLISHMGR_H

