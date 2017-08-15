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
#ifndef MXG_ISIPPRIVACYMGR_H
#define MXG_ISIPPRIVACYMGR_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_PRIVACY_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_PRIVACY_SVC_SUPPORT to be able \
to use this class.
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
class ISipPrivacySvc;
class ISipServerEventControl;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipPrivacyMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  ISipPrivacyMgr is the interface through which the ISipPrivacySvc reports
//  events to the application.
//
//  ISipPrivacySvc reports an event when a request is received from an IP
//  address that differs from the configured privacy service provider.
//
// Location:
//   SipUserAgent/ISipPrivacyMgr.h
//
// See Also:
//   ISipPrivacySvc
//
//==EDOC========================================================================
class ISipPrivacyMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  EvUncertifiedPrivacy
    //==
    //==========================================================================
    //
    //  Summary:
    //      A request has been received from another host than the configured
    //      privacy service provider.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing privacy.
    //
    //    rRequest:
    //      The request that was received from another host than the configured
    //      privacy service.
    //
    //    ropqApplicationData:
    //      Application data opaque to the service. This parameter is an INOUT
    //      parameter. It is used to correlate the events reported by multiple
    //      services for a unique received request. If the application has
    //      already received an event for that request through another manager
    //      interface, rApplicationData equals the value stored in it by the
    //      application. Otherwise, if it is the first event reported for this
    //      received request, rApplicationData is set to 0. The value of
    //      rApplicationData when EvUncertifiedPrivacy returns is accessible
    //      through the GetOpaque() method of the ISipServerEventControl 
    //      interface that accompanies the request when the owner service issues
    //      its event. This opaque data should be used to store the state 
    //      indicating that the privacy may be compromised by accepting this 
    //      request and being able to act accordingly when the session manager 
    //      receives the event containing the ISipServerEventControl. Note that
    //      this event cannot be processed asynchronously since the opaque 
    //      application data is passed by value to the ISipServerEventControl
    //      interface.
    //
    //  Description:
    //      This event means that a request has been received from another IP
    //      address than the resolved configured privacy service provider. This
    //      means that by accepting the request, the user agent could send
    //      identity-sensitive informations on the network.
    //
    //      When receiving this event, the application should send a final
    //      negative response to the request. The application should redirect
    //      the request (send a 3xx response) to its 'anonymous callback' URI
    //      if it knows it.
    //
    //      The address of the entity from which the packet was received can be
    //      retreived by rRequest.GetPeerAddr().
    //
    //  See Also:
    //      ISipPrivacySvc, ISipPrivacySvc::SetSharedPrivacyService,
    //      ISipPrivacySvc::SetInstancePrivacyService, CSipPacket::GetPeerAddr
    //
    //==EDOC====================================================================
    virtual void EvUncertifiedPrivacy(IN ISipPrivacySvc* pSvc,
                                      IN const CSipPacket& rRequest,
                                      INOUT mxt_opaque& ropqApplicationData) = 0;

    //==SDOC====================================================================
    //==
    //==  EvPrivacyDnsResolutionCompleted
    //==
    //==========================================================================
    //
    //  Summary:
    //      The privacy list has been resolved.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing privacy.
    //
    //    bSharedTrustedPrivacy:
    //      - true: this event is for a SetSharedPrivacyService call.
    //      - false: this event is for a SetInstancePrivacyService call.
    //
    //  Description:
    //      This method is called when the DNS resolution of the trusted hosts
    //      URI list is completed. It will be called for either the shared or
    //      instance trusted hosts as indicated by the bSharedTrustedPrivacy
    //      parameter.
    //
    //      Before this event is reported to the application, all other events
    //      of this manager SHOULD be considered unreliable. Once this
    //      event has been reported at least once for either the shared or
    //      instance trusted proxy list, all events are reliable.
    //
    //      This means that when the shared trusted hosts are set and the
    //      EvPrivacyDnsResolutionCompleted event has been reported once,
    //      changing the instance trusted host will NOT render the events
    //      unreliable. Those events could be reported while the DNS resolution
    //      for that new list is in progress and they will be based on the
    //      shared list until the DNS resolution is completed. 
    //
    //      The events reliability is based on the fact that there is at least
    //      one trusted host IP address list set. It does not matter if it is
    //      the shared or the instance one. As long as there is a trusted host
    //      IP address list set, the events are reliable according to that list.
    //
    //      The rule of thumb is: once the EvPrivacyDnsResolutionCompleted
    //      event has been reported, the service's events are reliable.
    //
    //==EDOC====================================================================
    virtual void EvPrivacyDnsResolutionCompleted(IN ISipPrivacySvc* pSvc,
                                                 IN bool bSharedTrustedPrivacy) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipPrivacyMgr(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipPrivacyMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipPrivacyMgr(IN const ISipPrivacyMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipPrivacyMgr& operator=(IN const ISipPrivacyMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPPRIVACYMGR_H

