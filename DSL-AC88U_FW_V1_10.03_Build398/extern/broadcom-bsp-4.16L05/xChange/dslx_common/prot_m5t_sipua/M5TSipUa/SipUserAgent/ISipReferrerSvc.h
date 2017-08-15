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
#ifndef MXG_ISIPREFERRERSVC_H
#define MXG_ISIPREFERRERSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT to be able \
to use this class.
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
class CHeaderList;
class CNameAddr;
class CSipMessageBody;
class ISipClientTransaction;
class ISipReferrerMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipReferrerSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipReferrerSvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   This interface is used to act as a referrer as defined in RFC 3515. It lets
//   the application send a REFER request and receive the proper notification of
//   the request progress.
//
//   It reports events received to the application through the ISipReferrerMgr
//   interface.
//
//#The ISipReferrerSvc is an ECOM object#
//
//  The ISipReferrerSvc is an ECOM object that is created and accessed through
//  the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipReferrerSvc</B>
//
//  <B>Interface Id: IID_ISipReferrerSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipReferrerSvc.h
//
// See Also:
//   ISipReferrerMgr
//
//==EDOC========================================================================
class ISipReferrerSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipReferrerSvc);
    //M5T_INTERNAL_USE_END

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
    virtual mxt_result SetManager(IN ISipReferrerMgr* pMgr) = 0;

    //==SDOC====================================================================
    //==
    //==  SetExpiringThreshold
    //==
    //==========================================================================
    // <FLAG Updated behavior in 4.1.4>
    //
    //  Summary:
    //      Sets the time to warn the application before a subscription expires.
    //
    //  Parameters:
    //    uThresholdSec:
    //      The time, in seconds, before expiration time that the EvExpiring is
    //      fired.
    //
    //  Description:
    //      Sets the amount of time that should remain to a subscription before
    //      EvExpiring is called for that subscription.
    //
    //      This method affects only future subscriptions. This means that if
    //      there is an active subscription while this method is called, the
    //      EvExpiring event is called according to the previous threshold
    //      value.
    //
    //      If the expires value is smaller than or equal to the threshold, the
    //      EvExpiring event is never called, EvExpired is instead called
    //      directly.
    //
    //  See Also:
    //      Refresh, Terminate
    //
    //==EDOC====================================================================
    virtual void SetExpiringThreshold(IN unsigned int uThresholdSec) = 0;

    //==SDOC====================================================================
    //==
    //==  Refer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a REFER request.
    //
    //  Parameters:
    //    rReferToAddr:
    //      The address to put in the Refer-To header.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership of this parameter
    //      is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the REFER request to be sent. It Can be NULL, which
    //      means that no message-body is added to the request. Ownership
    //      of this parameter is TAKEN. RFC 3515 does not put any semantics in
    //      this content.
    //
    //    ropqReferId:
    //      The id for that REFER request. It is useful when there are more than
    //      one REFER in the same context. This id is found in every event
    //      related to that REFER reported by the ISipReferrerMgr. Note that
    //      this is an OUT parameter determined by the service. This id is
    //      also used as a parameter to the Refresh or the Terminate methods
    //      when needing to refresh or terminate the subscription associated
    //      with this refer. Note that this id is different from the id
    //      parameter of the Event header found in NOTIFY requests received and
    //      SUBSCRIBE requests sent for this REFER.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //      - resFE_FAIL: The REFER request could not be sent.
    //
    //      - resFE_INVALID_STATE: The attached ISipUserAgentSvc is incorrectly
    //                             configured.
    //                             For instance, the contact list could be empty.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //      - resS_OK: The REFER request has been successfully sent.
    //
    //  Description:
    //      Sends a REFER request. This REFER request contains a Refer-To
    //      header with the rReferToAddr. The progress of the REFER request is
    //      reported through the ISipReferrerMgr. At least EvReferSuccess or
    //      EvReferFailed is reported. When the REFER succeeds, the
    //      progression of the referral is reported through EvReferStatus.
    //      The subscription is terminated either when EvReferStatus is
    //      reported with eState set to eTERMINATED or when EvExpired is
    //      reported. Before a subscription terminates, the application can
    //      either decide to terminate it by calling Terminate or extend it by
    //      calling Refresh.
    //
    //  See Also:
    //      ISipReferrerMgr, Terminate, Refresh
    //
    //==EDOC====================================================================
    virtual mxt_result Refer(IN    const CNameAddr&        rReferToAddr,
                             IN    mxt_opaque              opqTransaction,
                             IN TO CHeaderList*            pExtraHeaders,
                             IN TO CSipMessageBody*        pMessageBody,
                             OUT   mxt_opaque&             ropqReferId,
                             OUT   ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  Refresh
    //==
    //==========================================================================
    //
    //  Summary:
    //      Extends the subscription for that REFER.
    //
    //  Parameters:
    //    opqReferId:
    //      The opaque id corresponding to the REFER subscription to extend.
    //
    //    uExpirationSec:
    //      The maximum time, in seconds, for which the subscription is
    //      considered active without refreshing. When 0, the default expiration
    //      of 60 seconds is used.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership of this parameter
    //      is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the SUBSCRIBE request to be sent. It can be NULL, which
    //      means that no message-body is added to the request. Ownership
    //      of this parameter is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id does not correspond to any
    //      subscription in this object.
    //
    //      - resFE_FAIL: The SUBSCRIBE request could not be sent.
    //
    //      - resS_OK: The SUBSCRIBE request has been successfully sent.
    //
    //  Description:
    //      This method sends a SUBSCRIBE request to extend the subscription
    //      created by the REFER it sent. It can be used when the subscription
    //      is about to expire and the final notification has not been received.
    //
    //      Note that when the application is no longer interested in the result
    //      of the referral process, it should call Terminate.
    //
    //  See Also:
    //      Terminate
    //
    //==EDOC====================================================================
    virtual mxt_result Refresh(IN mxt_opaque opqReferId,
                               IN unsigned int uExpirationSec,
                               IN mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               OUT ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates the subscription for that REFER.
    //
    //  Parameters:
    //    opqReferId:
    //      The opaque id corresponding to the REFER subscription to terminate.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership of this parameter
    //      is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the SUBSCRIBE request to be sent. It can be NULL, which
    //      means that no message-body is added to the request. Ownership
    //      of this parameter is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id does not correspond to any
    //      subscription in this object.
    //
    //      - resFE_FAIL: The SUBSCRIBE request could not be sent.
    //
    //      - resS_OK: The SUBSCRIBE request has been successfully sent.
    //
    //  Description:
    //      This method sends a SUBSCRIBE request to immediately terminate the
    //      subscription created by the REFER it sent. It can be used when the
    //      application is no longer interested in the result of the referral
    //      process.
    //
    //  See Also:
    //      Refresh
    //
    //==EDOC====================================================================
    virtual mxt_result Terminate(IN mxt_opaque opqReferId,
                                 IN mxt_opaque opqTransaction,
                                 IN TO CHeaderList* pExtraHeaders,
                                 IN TO CSipMessageBody* pMessageBody,
                                 OUT ISipClientTransaction*& rpTransaction) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipReferrerSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipReferrerSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipReferrerSvc(IN const ISipReferrerSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipReferrerSvc& operator=(IN const ISipReferrerSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREFERRERSVC_H

