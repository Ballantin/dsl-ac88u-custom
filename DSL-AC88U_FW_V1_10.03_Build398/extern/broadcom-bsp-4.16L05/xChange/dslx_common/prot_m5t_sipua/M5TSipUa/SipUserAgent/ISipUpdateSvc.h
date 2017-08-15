//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPUPDATESVC_H
#define MXG_ISIPUPDATESVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT to be able \
to use this class.
#endif


//-- Data Member
//---------------


//-- Interface Realized and Parent
//----------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


//-- Parameter by value
//----------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CHeaderList;
class CSipMessageBody;
class ISipClientTransaction;
class ISipUpdateMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipUpdateSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipUpdateSvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The update service implements RFC 3311. It is used to send an UPDATE
//  request within a dialog (early or confirmed) to update session parameters
//  without impacting the dialog state itself.
//
//  Under some circumstances, it is recommended not to send UPDATE requests
//  containing an offer; for instance, when a pending offer is not answered yet.
//  For more details on those circumstances, refer to RFC 3311 Section 5.1
//  (both caller and callee). The service sends an UPDATE request even if
//  the application has a pending offer (other than UPDATE) for which no answer
//  has been sent or received. It is the application's responsibility to manage
//  the offer found in an UPDATE request when there is already a pending one.
//
//  The application must not send any UPDATE request while there is a pending
//  incoming UPDATE request for which no final response has been provided yet.
//  On the other hand, if M5T SIP UA receives an UPDATE request and it is
//  waiting for the final response of an outgoing one, it automatically sends a
//  500 response. If it receives an UPDATE request while an UPDATE request was
//  received but not answered yet, it automatically responds with a 491
//  response.
//
//  The application should add an Allow header containing the UPDATE request
//  when sending an INVITE or a reliable provisional response to a pending
//  INVITE. Doing so notifies the remote end that the local party supports the
//  UPDATE mechanism.
//
//  Using the update service is done in collaboration with the session
//  service and possibly the reliable response service (PRACK). PRACK can be
//  used in conjunction with the UPDATE service to send offers in reliable
//  provisional responses.
//
//  The application should only send UPDATE requests containing an offer when a
//  pending INVITE is underway. Otherwise, it should use a re-INVITE instead.
//  This is a requirement (conformance item 7) of RFC 3311 section 5.1.
//
//  This service generates events to the application through the ISipUpdateMgr
//  interface.
//
//#The ISipUpdateSvc is an ECOM object#
//
//  The ISipUpdateSvc is an ECOM object that is created and accessed through the
//  following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipUpdateSvc</B>
//
//  <B>Interface Id: IID_ISipUpdateSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//  SipUserAgent/ISipUpdateSvc.h
//
// See Also:
//  ISipSessionSvc::Invite
//
//==EDOC========================================================================
class ISipUpdateSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipUpdateSvc);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager associated with this service.
    //
    //  Parameters:
    //    pMgr:
    //      The manager. It must not be NULL.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT if pMgr is NULL. The manager is not changed
    //          in this case.
    //
    //      resS_OK otherwise.
    //
    //  Description:
    //      Configures the manager that will receive the events generated by
    //      this service.
    //
    //      Note that a manager MUST be associated with this service before it
    //      is used. If no manager is associated with the service, the service
    //      automatically responds a 500 response to any received response.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager( IN ISipUpdateMgr* pMgr ) = 0;


    //==SDOC====================================================================
    //==
    //==  Update
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends an UPDATE request.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. Any Contact header in
    //      this list is replaced by the local contact of the ISipUserAgentSvc
    //      attached to the context. It can be NULL. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      Payload to include with the UPDATE. It can be NULL. Ownership is
    //      TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager to this service. This
    //                              could happen when no manager has been set
    //                              on the service or when Clear was called on
    //                              the context
    //                          OR if there is no dialog
    //                          OR if the local contact list is empty.
    //
    //      resFE_FAIL if there is no ISipUserAgentSvc attached to the context
    //                 OR there is already an UPDATE request underway either
    //                      UAC or UAS
    //                 OR the request could not be sent
    //                 OR the attached user agent service is incorrectly
    //                      configured (e.g.: the contact list is empty).
    //
    //      resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //      resS_OK if the request has been sent.
    //
    //  Description:
    //      This method is used to send an UPDATE request to the remote target.
    //      Sending the request is possible only if the dialog is established
    //      between the UA and the peer. The application should only send
    //      UPDATE requests containing an offer when the pending original
    //      INVITE is underway. Otherwise, it should use a re-INVITE instead.
    //
    //      While the INVITE request is not completed, an UPDATE request
    //      containing an offer should be sent only when a previous offer was
    //      generated (in the INVITE request) or received (in a reliable
    //      provisional response to the INVITE) and responded (in a reliable
    //      provisional response or in a PRACK). Refer to section 5.1 of RFC
    //      3311 for more information on when an UPDATE request can be sent.
    //      This method sends an UPDATE request even if the application has
    //      a pending offer (other than UPDATE) for which no answer has been
    //      sent or received. It is the application's responsibility to manage
    //      the offer found in an UPDATE request when there is already a pending
    //      one.
    //
    //      The Contact header found in the ISipUserAgentSvc is automatically
    //      added to the UPDATE request.
    //
    //      Notifications about the request's progress are received through the
    //      ISipUpdateMgr interface.
    //
    //  See Also:
    //      ISipUpdateMgr::EvProgress, ISipUpdateMgr::EvSuccess,
    //      ISipUpdateMgr::EvFailure, ISipUserAgentSvc::GetState, RFC 3311
    //
    //==EDOC====================================================================
    virtual mxt_result Update( IN mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               OUT ISipClientTransaction*& rpTransaction ) = 0;


//-- Hidden Methods
//-------------------
protected:
    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipUpdateSvc() {}

    // Summary:
    //  Default Destructor.
    //----------------------
    virtual ~ISipUpdateSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipUpdateSvc(IN const ISipUpdateSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipUpdateSvc& operator=(IN const ISipUpdateSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPUPDATESVC_H

