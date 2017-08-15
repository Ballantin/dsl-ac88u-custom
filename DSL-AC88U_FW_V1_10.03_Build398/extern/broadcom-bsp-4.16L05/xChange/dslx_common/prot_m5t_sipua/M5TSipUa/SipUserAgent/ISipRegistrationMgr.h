//==SDOC========================================================================
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
//   Media5 reserve all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserve the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPREGISTRATIONMGR_H
#define MXG_ISIPREGISTRATIONMGR_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT to be \
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
class ISipRegistrationSvc;
class ISipClientEventControl;
class CSipHeader;
class CSipPacket;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipRegistrationMgr
//===========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The Registration Manager is the interface through which the Registration
//  Service talks to the application. The ISipRegistrationSvc informs the 
//  application through this interface of the progress of the REGISTER
//  requests it has sent and their expirations.
//
// Location:
//   SipUserAgent/ISipRegistrationMgr.h
//
// See Also:
//  ISipRegistrationSvc
//==============================================================================
class ISipRegistrationMgr
{
// Friend Declarations
//-------------------------

// Published Interface
//-------------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      A provisional response ( < 200 ) to a REGISTER request has been
    //      received by the ISipRegistrationSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipRegistrationSvc that is managing this response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rPacket:
    //      The response packet.
    //
    //  Description:
    //      Notifies the registration manager that a provisional response has
    //      been received for the last outgoing REGISTER request sent through
    //      the registration service.
    //
    //      The state of the registration service does not change after the
    //      reception of this provisional response: it still waits for a final
    //      response.
    //
    //==EDOC====================================================================
    virtual void EvProgress(IN ISipRegistrationSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      A success response ( 2xx ) to a REGISTER request has been received 
    //      by the ISipRegistrationSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipRegistrationSvc that is managing this response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rPacket:
    //      The response packet.
    //
    //  Description:
    //      Notifies the registration manager that a success response has been
    //      received for the last outgoing REGISTER request sent through the
    //      registration service. Some validation is performed on the response
    //      packet to guarantee that when this event is reported the packet will
    //      be RFC3261 compliant. This means that after a call to
    //      ISipRegistrationSvc::Add(), the response packet will contain all
    //      sent contacts. After a call to ISipRegistrationSvc::Remove(), the
    //      response will not contain any of the sent contacts. After a call to
    //      ISipRegistrationSvc::Clear(), the response packet will not contain
    //      any contacts at all. And, after a call to
    //      ISipRegistrationSvc::GetList(), the packet may contain contacts. If
    //      any of the above conditions are not met,
    //      ISipRegistrationMgr::EvFailure will be reported instead.
    //
    //      After this success response, the registration service keeps track of
    //      registered contacts and warns the manager when they are about to
    //      expire and when they are expired.
    //
    //  See Also:
    //      ISipRegistrationMgr::EvFailure
    //
    //==EDOC====================================================================
    virtual void EvSuccess(IN ISipRegistrationSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvFailure
    //==
    //==========================================================================
    // <FLAG Updated behavior in 4.1.4>
    //
    //  Summary:
    //      A failure response ( >= 300 ) to a REGISTER request has been 
    //      received by the ISipRegistrationSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipRegistrationSvc that is managing this response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rPacket:
    //      The response packet.
    //
    //  Description:
    //      Notifies the registration manager that a failure response has been
    //      received for the last outgoing REGISTER request sent through the
    //      registration service.
    //
    //      After this failure response, the registration service acts as if
    //      the last REGISTER request was not sent.
    //
    //    #Updated behaviour:#
    //
    //      The registration service's behaviour regarding the reporting of this
    //      event has changed in the following ways with respect to SIP-UA 4.1.3.
    //
    //      Generally this event will be reported for failure responses but it
    //      may now also be reported for success responses. This situation will
    //      occur in one of the following cases:
    //
    //      - The last request sent tried to register new contacts and the 200
    //        response received did not contain those "to be" registered
    //        contacts.
    //      - The last request sent tried to remove some of the existing
    //        registered contacts and the 200 response contained one or more of
    //        those contacts.
    //      - The last request sent tried to clear all registrations and the 200
    //        response contained contacts.
    //      - Any one of the 200 response contacts causes a parser error.
    //
    //==EDOC====================================================================
    virtual void EvFailure(IN ISipRegistrationSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvExpiring
    //==
    //==========================================================================
    //
    //  Summary:
    //      One or more contacts are about to expire.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipRegistrationSvc that is managing the contacts.
    //
    //    pContacts:
    //      The list of contacts that are about to expire.  If there are many
    //      contacts, they will be chained in the CSipHeader object.
    //
    //  Description:
    //      Notifies the registration manager that the contacts in the parameter
    //      are about to expire. If the manager wants to keep the contacts
    //      registered to the remote server, it should register them again by
    //      calling the Add method of the Registration Service of this context.
    //      If the manager fails to do so, EvExpired will be issued later, the
    //      amount of time being dependent on
    //      ISipRegistrationSvc::SetExpiringThreshold's configuration.
    //
    //  See Also:
    //      ISipRegistrationSvc, EvExpired
    //==========================================================================
    virtual void EvExpiring(IN ISipRegistrationSvc* pSvc,
                            IN TO CSipHeader* pContacts) = 0;

    //==========================================================================
    //==
    //==  EvExpired
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that these contacts are expired.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipRegistrationSvc that is managing the contacts.
    //
    //    pContacts:
    //      A list of contacts that are expired.  If there are many contacts,
    //      they will be chained in the CSipHeader object.
    //
    //  Description:
    //      Notifies the registration manager that the contacts in the parameter
    //      are expired. The manager should consider that these are no longer
    //      registered to the remote server. Usually, the manager should
    //      not let that event occur. If it wants to unregister contacts, it
    //      should not wait until their expiration but rather explicitly
    //      unregister them with the Remove method of the registration service
    //      interface. If the manager wants to keep the contacts registered to
    //      the remote server, it should register them again by calling the Add
    //      method of the Registration Service of this context. This
    //      event should only occur when the manager was unable to refresh or
    //      remove its registration due to some remote server error and when the
    //      contacts were received with a smaller expiration time than the
    //      threshold of the registration service.
    //
    //  See Also:
    //      ISipRegistrationSvc, EvExpiring
    //==========================================================================
    virtual void EvExpired(IN ISipRegistrationSvc* pSvc,
                           IN TO CSipHeader* pContacts) = 0;

// Hidden Methods
//-------------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipRegistrationMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipRegistrationMgr() {}


private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    ISipRegistrationMgr( const ISipRegistrationMgr& rFrom );
    ISipRegistrationMgr& operator=( const ISipRegistrationMgr& rFrom );

// Hidden Data Members
//-------------------------
protected:
private:

};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPREGISTRATIONMGR_H

