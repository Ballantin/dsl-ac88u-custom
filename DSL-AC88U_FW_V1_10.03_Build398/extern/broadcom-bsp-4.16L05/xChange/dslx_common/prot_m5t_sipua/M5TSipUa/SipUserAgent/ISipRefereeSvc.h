//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPREFEREESVC_H
#define MXG_ISIPREFEREESVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT to be able \
to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_ISIPNOTIFIERSVC_H
#include "SipUserAgent/ISipNotifierSvc.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CHeaderList;
class CSipStatusLine;
class ISipRefereeMgr;
class ISipClientTransaction;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipRefereeSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipRefereeSvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   This interface is used to act as a referee as defined in RFC 3515. It lets
//   the application receive a REFER request and send the proper notification of
//   the request progress.
//
//   It reports events received to the application through the ISipRefereeMgr
//   interface.
//
//#The ISipRefereeSvc is an ECOM object#
//
//  The ISipRefereeSvc is an ECOM object that is created and accessed through
//  the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipRefereeSvc</B>
//
//  <B>Interface Id: IID_ISipRefereeSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipRefereeSvc.h
//
// See Also:
//   ISipRefereeMgr
//
//==EDOC========================================================================
class ISipRefereeSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipRefereeSvc);
    //M5T_INTERNAL_USE_END

    // Description:
    //  Determines if the referee service should add the 'id' parameter to the
    //  Event header.
    //------------------------------------------------------------------------
    enum EIdParameterUsage
    {
        // Description:
        //  Parameter is always present.
        eID_PARAM_ALWAYS_PRESENT,
        // Description:
        //  Parameter is absent for first REFER.
        eID_PARAM_ABSENT_FOR_FIRST_REFER
    };

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager of this service.
    //
    //  Parameters:
    //    pMgr:
    //      The manager. It must not be NULL.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT if pMgr is NULL.  The manager is not 
    //          changed in this case.
    //
    //      resS_OK otherwise.
    //
    //  Description:
    //      Configures the manager that will receive the events from this
    //      service.
    //
    //      Note that a manager MUST be associated with this service before it
    //      is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipRefereeMgr* pMgr) = 0;

    //==SDOC====================================================================
    //==
    //==  SendReferralStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a NOTIFY for a received REFER.
    //
    //  Parameters:
    //    opqReferId:
    //      The opaque id that accompanied the received REFER.
    //
    //    eSubscriptionState:
    //      The state of the subscription to put in the Subscription-State
    //      header of the NOTIFY to send. It can be either ePENDING or eACTIVE.
    //
    //    uExpirationSec:
    //      The maximum time, in seconds, for which the subscription is 
    //      considered active without refreshing. When 0, the current remaining 
    //      expiration for that subscription is used. This value is put in the 
    //      "Subscription-State" header's "expires" parameter of the NOTIFY 
    //      request sent. The application should not use 0 the first time it 
    //      calls this method since the initial expiration is set really high 
    //      so the application can decrease it to what it wants. This expiration
    //      should be set to the maximum time expected to obtain a final status
    //      for the referred request.
    //
    //    rContentStatusLine:
    //      The status-line to put in the content of the NOTIFY request. It is
    //      the actual referral status. Note that it can be a final response
    //      when other responses are still expected (because the application
    //      will retry the referred request for instance). When no other
    //      responses are expected, SendFinalReferralStatus should be used
    //      instead.
    //
    //    pContentHeaders:
    //      Headers to put in the content after the status-line. RFC 3515 does
    //      not define semantics for these headers. However, care should be
    //      taken when populating this parameter. Refer to the security section
    //      of RFC 3515 for details. It can be NULL, which means that only the
    //      request-line is sent in the content.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership of this parameter
    //      is TAKEN. This parameter puts the extra headers in the NOTIFY 
    //      request. To put headers in the content of the NOTIFY, use the 
    //      pContentHeaders parameter.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id does not correspond to any REFER
    //      received in this object or the expiration time is higher than the
    //      remaining time for that subscription.
    //
    //      - resFE_INVALID_STATE: The id corresponds to a subscription that is
    //         expired. Use SendFinalReferralStatus instead to send a final NOTIFY.
    //         Note that this is the case when receiving a SUBSCRIBE with a
    //         "Expires" header value of 0
    //      OR the attached ISipUserAgentSvc is incorrectly 
    //         configured. 
    //         For instance, the contact list could be empty.
    //
    //      - resFE_FAIL: The NOTIFY request could not be sent.
    //
    //      - resS_OK: The NOTIFY request has been successfully sent.
    //
    //  Description:
    //      Sends a NOTIFY request for an active or pending subscription. The
    //      "message/sipfrag" content mandated by RFC 3515 is created from
    //      the rContentStatusLine and the pContentHeaders parameters.
    //
    //      The application should use a non 0 uExpirationSec the first time it
    //      calls this method in order to set the initial expiration time for
    //      the subscription. Otherwise, the default expiration time is
    //      used. Note that once an expiration time is set, the referee cannot
    //      increase it. The subscription can only be extended by the referrer.
    //
    //      When the application lowers the expiration time by using a non NULL
    //      value in the uExpirationSec parameter, the expiration time is
    //      lowered if the NOTIFY request receives a success response.
    //
    //  See Also:
    //      SendFinalReferralStatus
    //
    //==EDOC====================================================================
    virtual mxt_result SendReferralStatus(IN mxt_opaque opqReferId,
                                          IN ISipNotifierSvc::EState eSubscriptionState,
                                          IN unsigned int uExpirationSec,
                                          IN const CSipStatusLine& rContentStatusLine,
                                          IN const CHeaderList* pContentHeaders,
                                          IN mxt_opaque opqTransaction,
                                          IN TO CHeaderList* pExtraHeaders,
                                          OUT ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  SendFinalReferralStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a final NOTIFY for a received REFER.
    //
    //  Parameters:
    //    opqReferId:
    //      The opaque id that accompanied the received REFER.
    //
    //    eReason:
    //      The "reason" parameter to put in the "Subscription-State" header.
    //      When the value is eNO_REASON, the parameter is not put in the
    //      "Subscription-State" header.
    //
    //    uRetryAfterSec:
    //      The number of seconds to put in the "retry-after" parameter of the
    //      "Subscription-State" header. When the value is 0, no "retry-after"
    //      parameter is put. Note that RFC 3265 defines semantics for this
    //      parameter only if the eReason parameter has the value eNO_REASON,
    //      ePROBATION, or eGIVEUP.
    //
    //    rContentStatusLine:
    //      The status-line to put in the content of the NOTIFY request. It is
    //      the actual referral status. Note that this response can be either
    //      final or provisional. The fact that this NOTIFY is final simply
    //      indicates to the referrer that the referee no longer informs it
    //      of refer request progress.
    //
    //    pContentHeaders:
    //      Headers to put in the content after the status-line. RFC 3515 does
    //      not define semantics for these headers. However, care should be
    //      taken when populating this parameter. Refer to the security section
    //      of RFC 3515 for details. It can be NULL, which means that only the
    //      request-line is sent in the content.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership of this parameter 
    //      is TAKEN. This parameter puts the extra headers in the NOTIFY 
    //      request. To put headers in the content of the NOTIFY, use the 
    //      pContentHeaders parameter.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id does not correspond to any
    //      received REFER in this object.
    //
    //      - resFE_INVALID_STATE: The attached ISipUserAgentSvc is incorrectly
    //      configured. For instance, the contact list could be empty.
    //
    //      - resFE_FAIL: The NOTIFY request could not be sent.
    //
    //      - resS_OK: The NOTIFY request has been successfully sent.
    //
    //  Description:
    //      Sends a NOTIFY with the "Subscription-State" header set to 
    //      "terminated". The "message/sipfrag" content mandated by RFC 3515 is
    //      created from the rContentStatusLine and the pContentHeaders 
    //      parameters.
    //
    //      This method should be called when the event EvExpired is called by
    //      the ISipRefereeMgr.
    //
    //      It should also be called when accepting a SUBSCRIBE received through
    //      the EvTerminated event of the ISipRefereeMgr.
    //
    //      Finally, it should be called when the application knows that it will
    //      no longer report status for this referral.
    //
    //  See Also:
    //      SendReferralStatus
    //
    //==EDOC====================================================================
    virtual mxt_result SendFinalReferralStatus(IN mxt_opaque opqReferId,
                                               IN ISipNotifierSvc::EReason eReason,
                                               IN unsigned int uRetryAfterSec,
                                               IN const CSipStatusLine& rContentStatusLine,
                                               IN const CHeaderList* pContentHeaders,
                                               IN mxt_opaque opqTransaction,
                                               IN TO CHeaderList* pExtraHeaders,
                                               OUT ISipClientTransaction*& rpTransaction) = 0;

    //==========================================================================
    //==
    //==  ConfigureNotifyIdParameterUsage
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures whether the stack uses the "id" parameter in
    //      the Event header of NOTIFY requests for the first REFER request.
    //
    //  Parameters:
    //    eIdParamUsage:
    //      - eID_PARAM_ALWAYS_PRESENT:
    //          The id parameter is added to all NOTIFYs. This is the default
    //          behaviour.
    //
    //      - eID_PARAM_ABSENT_FOR_FIRST_REFER:
    //          The id parameter in the NOTIFYs to the first REFER are not set.
    //
    //  Description:
    //      Configures whether the stack uses the "id" parameter in
    //      the Event header of the NOTIFY it sends after a subscription is
    //      created by receiving a REFER. This is used only for the NOTIFY(s)
    //      associated with the first REFER received by this service.
    //
    //      RFC 3515 states that the NOTIFY(s) associated with the first
    //      REFER sent on a dialog MAY contain an "id" parameter in the Event
    //      header, and the NOTIFY(s) associated with any additional REFER
    //      after the first one MUST contain this parameter. The default
    //      behavior of the stack is to always include the parameter for all
    //      NOTIFY(s) it sends, however this can sometimes cause
    //      interoperability issues. An application can use this method to
    //      configure whether the stack includes the "id" parameter
    //      for the NOTIFY(s) associated with the first REFER it has
    //      received. All NOTIFYs associated with subsequent REFERs will always
    //      contain the "id" parameter in the Event header.
    //
    //  See Also:
    //      ISipRefereeSvc
    //
    //==========================================================================
    virtual void ConfigureNotifyIdParameterUsage(IN EIdParameterUsage eIdParamUsage) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipRefereeSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipRefereeSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipRefereeSvc(IN const ISipRefereeSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipRefereeSvc& operator=(IN const ISipRefereeSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREFEREESVC_H
