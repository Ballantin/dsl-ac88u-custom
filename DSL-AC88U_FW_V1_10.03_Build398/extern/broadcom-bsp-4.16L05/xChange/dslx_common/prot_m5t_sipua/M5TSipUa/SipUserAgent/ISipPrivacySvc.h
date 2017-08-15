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
#ifndef MXG_ISIPPRIVACYSVC_H
#define MXG_ISIPPRIVACYSVC_H

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
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipUri;
class ISipPrivacyMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipPrivacySvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipPrivacySvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  This service implements user-level privacy functions as per RFC 3323.
//
//  The service removes all headers that may contain or give a hint of
//  private information. These headers are Call-Info, Reply-To, User-Agent,
//  Organization, Server, Subject, In-Reply-To, and Warning.
//
//  It is important to know that other headers may contain private information.
//  Some of these headers are From, Call-ID, and Referred-By. It is the
//  application's responsability to properly set the value of these headers to
//  be sure they do not contain private information. Some of these headers are
//  set through ISipUserAgentSvc. Note that the default Call-ID value generated
//  by the ISipUserAgentSvc does not contain any private information, so it can
//  be kept as is to guarantee its privacy.
//
//  When using this service, the application should be cautious when adding
//  headers. It must make sure that it does not include private information in
//  added headers.
//
//  Also, other elements of a packet must give informations on how the current
//  dialog should be routed. Theses elements are the Contact header, the Via
//  header, and the session information in the SDP. These must contain valid
//  informations and cannot be anonymized by the local UA. To transform these
//  elements, the user agent must use an intermediary. This intermediary is a
//  privacy service provider (also named a privacy service). To anonymize the
//  headers, the user agent must request the 'header' privacy type. To anonymize
//  the SDP informations, the user agent must request the 'session' privacy
//  type. Note that when 'session' privacy type is requested, the SDP payload
//  MUST NOT be encrypted.
//
//  This service allows to set a privacy service provider that will provide
//  further privacy if requested. The privacy type handled by the privacy
//  service provider is configurable via the SetPrivacyType method. By default,
//  the 'header' and 'id' privacy types are requested. An application should
//  couple the use of this service with the use of a privacy service provider.
//
//  Note that the application must route the request to the privacy service
//  provider. This should be done by setting a pre-loaded route to the
//  privacy service provider. The top-most Route header of the pre-loaded route
//  must contain the privacy service provider URI. Also, if a user agent is
//  sending a request directly to a privacy service, it SHOULD include a
//  Proxy-Require header containing the 'privacy' option-tag, especially when
//  the 'critical' priv-value is present in the Privacy header. Configuring
//  the Proxy-Require header can be done by using the AddProxyRequire method.
//  The header is added by default.
//
//  RFC 3323 states that to certify privacy while receiving a request, the user
//  agent can "procure an 'anonymous callback' URI from the third-party
//  service and [...] distribute this as an address-of-record. A privacy service
//  provider might offer these anonymous callback URIs to users in the same way
//  that an ordinary SIP service provider grants addresses-of-record. The user
//  would then register their normal address-of-record as a contact address with
//  the third-party service." This means that requests should only come from the
//  privacy service provider.
//
//  RFC 3323 also gives other alternatives. One of them is that the "user agent
//  could send REGISTER requests through a privacy service with a request for
//  'user' level privacy. This will allow the privacy service to insert
//  anonymous Contact header URIs. Requests sent to the user's conventional
//  address-of-record would then reach the user's devices without revealing any
//  usable contact addresses."
//
//  In any way, to have privacy guaranteed, requests must be received from the
//  privacy service provider. The service warns the application through the
//  ISipPrivacyMgr::EvUncertifiedPrivacy event if a request has been received
//  from a source other than the configured privacy service provider. The
//  application can then choose to reject the request, accept it with user
//  approval, or redirect it to its 'anonymous callback' URI.
//
//  Note that private information is transmitted in transactions between
//  the user agent and the privacy service provider, so RFC 3323 highly 
//  recommends "that user agents use network or transport layer security, such 
//  as TLS, when contacting a privacy service."
//
//  This service generates events to the application through the ISipPrivacyMgr
//  interface.
//
//  IMPORTANT: this service must be the LAST added to the context. Doing
//  otherwise may lead to privacy issues.
//
//#The ISipPrivacySvc is an ECOM object#
//
//  The ISipPrivacySvc is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipPrivacySvc</B>
//
//  <B>Interface Id: IID_ISipPrivacySvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipPrivacySvc.h
//
// See Also:
//   ISipPrivacyMgr, ISipUserAgentSvc::SetPreloadedRoute
//
//==EDOC========================================================================
class ISipPrivacySvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipPrivacySvc);
    //M5T_INTERNAL_USE_END

    // Summary:
    //  Requested privacy level.
    //
    // Description:
    //  Bitset describing the privacy level that can be requested. To get a
    //  complete description of each level, please refer to RFC 3323 sections
    //  4.2 and 5, or refer to RFC 3325.
    //-----------------------------------------------------------------------
    enum EPrivValue
    {
        // RFC 3323 'header' priv-value. Anonymize headers that cannot be
        // anonymized by the local user agent (for example, Contact and Via).
        //--------------------------------------------------------------------
        eHEADER = 1,            // 1
        
        // RFC 3323 'session' priv-value. Anonymize sessions created by the
        // message. For example, modify IP addresses in the SDP payload. Note
        // that RFC 3323 states that "when session privacy is requested, user
        // agents MUST NOT encrypt SDP bodies in messages."
        //--------------------------------------------------------------------
        eSESSION = eHEADER * 2, // 2

        // RFC 3323 'user' priv-value. Anonymize headers that should be
        // anonymized by the local user agent. This includes what the
        // ISipPrivacySvc does and anonyzation of other headers like the From,
        // Call-ID, and Referred-By header. RFC 3323 states that this privacy
        // type MAY be used for REGISTER requests but SHOULD NOT be used for
        // other requests.
        //---------------------------------------------------------------------
        eUSER = eSESSION * 2,   // 4
        
        
        // RFC 3323 'none' priv-value. No anonymization must be done on the
        // outgoing requests. Note that this privacy type cannot be set with
        // other types.
        //-------------------------------------------------------------------
        eNONE = eUSER * 2,      // 8

        // RFC 3323 'critical' priv-value. The requested privacy type MUST be
        // done. The request must fail otherwise. This privacy type cannot be
        // set with eNONE and cannot be set alone. Note that this privacy type
        // cannot be correctly managed for responses but it will be inserted
        // anyhow.
        //---------------------------------------------------------------------
        eCRITICAL = eNONE * 2,  // 16

        // RFC 3325 'id' priv-value. The Network Asserted Identity must be kept
        // private outside of the Trust Domain to which the user has
        // authenticated itself.
        //---------------------------------------------------------------------
        eID = eCRITICAL * 2,    // 32

        // Not a Privacy header value : internal use only.
        //-------------------------------------------------
        eINVALID = eID * 2      // 64
    };

    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager associated with this instance.
    //
    //  Parameters:
    //    pMgr:
    //      The manager. It must not be NULL.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT:
    //          pMgr is NULL. The manager is not changed in this case.
    //
    //      resS_OK:
    //          Otherwise.
    //
    //  Description:
    //      Configures the manager that will receive the events generated by
    //      this service.
    //
    //      Note that a manager MUST be associated with this service before it
    //      is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipPrivacyMgr* pMgr) = 0;


    //==SDOC====================================================================
    //==
    //==  SetSharedPrivacyService
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the address of the privacy service provider used by all
    //      instances of this service.
    //
    //  Parameters:
    //    rPrivacyService:
    //      The SIP-URI / SIPS-URI of the server that provides privacy to the
    //      local UA.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      This service has no manager. Maybe it was not set or Clear was
    //      called on the context.
    //
    //    resFE_INVALID_STATE:
    //      This service already has its own instance list so modifying the
    //      shared list would not affect the current service behaviour. Updating
    //      was not done.
    //
    //    resFE_INVALID_STATE:
    //      The privacy service provider list is currently used. The main cause
    //      would be that resolving is currently processed for the service list.
    //
    //    resS_OK:
    //      Resolving has begun for the privacy service provider URI passed to
    //      the method.
    //
    //  Description:
    //      Sets the address of the privacy service provider. It is through this
    //      privacy service provider that outgoing requests and responses may be
    //      further anonymized.
    //
    //      This method configures all existing and future instances of this
    //      service with rPrivacyService, but does not affect the instances that
    //      had their privacy service provider configured through
    //      SetInstancePrivacyService.
    //
    //      If the system only uses a single privacy service provider, you
    //      should use SetSharedPrivacyService once in the system as DNS
    //      resolving for the server is done just once. If the system
    //      simultaneously uses more than one privacy service provider, you have
    //      to use SetInstancePrivacyService each time this service is
    //      attached to a SIP Context.
    //
    //      This method resolves the SIP-URI / SIPS-URI to one or more IP
    //      addresses.
    //
    //      If requests are received from a server other than the privacy
    //      service, EvUncertifiedPrivacy is reported to the application.
    //
    //      A pre-loaded route SHOULD be set by the application, with the first
    //      route pointing to the privacy service provider. This is to guarantee
    //      that requests are always sent first to the privacy service server,
    //      which can then further anonymize the requests and responses. Doing
    //      otherwise would compromise privacy.
    //
    //  See Also:
    //      ISipPrivacyMgr::EvUncertifiedPrivacy, SetInstancePrivacyService
    //
    //==EDOC====================================================================
    virtual mxt_result SetSharedPrivacyService(
                                        IN const CSipUri& rPrivacyService) = 0;


    //==SDOC====================================================================
    //==
    //==  SetInstancePrivacyService
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the address of the privacy service provider used by this 
    //      instance of this service.
    //
    //  Parameters:
    //    rPrivacyService:
    //      The SIP-URI / SIPS-URI of the server that will provide privacy to
    //      this context.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      This service has no manager. Maybe it was not set or Clear was
    //      called on the context.
    //
    //    resFE_INVALID_STATE:
    //      The privacy service provider list is currently used. The main cause
    //      would be that resolving is currently processed for the service list.
    //
    //    resS_OK:
    //      Resolving has begun for the privacy service provider URI passed to
    //      the method.
    //
    //  Description:
    //      Sets the address of the privacy service provider. It is through this
    //      privacy service provider that outgoing requests and responses may be
    //      further anonymized.
    //
    //      This method overrides, for this instance only, all previous and
    //      future configuration done through SetSharedPrivacyService. That is,
    //      this service always uses its instance configuration first, and
    //      if none is set, then it uses the shared configuration.
    //
    //      If the system only uses a single privacy service provider, you
    //      should use SetSharedPrivacyService once in the system as DNS
    //      resolving for the server is done just once. If the system
    //      simultaneously uses more than one privacy service provider, you have
    //      to use SetInstancePrivacyService each time this service is
    //      attached to a SIP Context.
    //
    //      This method resolves the SIP-URI / SIPS-URI to one or more IP
    //      addresses.
    //
    //      If requests are received from a server other than the configured
    //      privacy service, EvUncertifiedPrivacy is reported to the
    //      application.
    //
    //      The application SHOULD set a pre-loaded route, with the first
    //      route pointing to the privacy service provider. This guarantees
    //      that requests are always sent first to the privacy service server,
    //      which can then further anonymize the requests and responses. Doing
    //      otherwise would compromise privacy.
    //
    //  See Also:
    //      ISipPrivacyMgr::EvUncertifiedPrivacy, SetSharedPrivacyService
    //
    //==EDOC====================================================================
    virtual mxt_result SetInstancePrivacyService(
                                        IN const CSipUri& rPrivacyService) = 0;


    //==SDOC====================================================================
    //==
    //==  AddProxyRequire
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables or disables the usage of the Proxy-Require header containing
    //      the 'privacy' option-tag.
    //
    //  Parameters:
    //    rPrivacyService:
    //      true if a Proxy-Require header with the 'privacy' option-tag must be
    //      added to outgoing requests.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      This service has no manager. Maybe it was not set or Clear was
    //      called on the context.
    //
    //    resS_OK:
    //      The configuration has been set.
    //
    //  Description:
    //      This method permits to configure whether a Proxy-Require with
    //      the 'privacy' option-tag must be added to outgoing requests.
    //
    //      RFC 3323 states that the header SHOULD be added especially when the
    //      'critical' privacy value is present in the Privacy header.
    //
    //      The header is added by default.
    //
    //      This configuration only affects the instance upon which it is
    //      called.
    //
    //  See Also:
    //      SetPrivacyType
    //
    //==EDOC====================================================================
    virtual mxt_result AddProxyRequire(IN bool bAddHeader) = 0;


    //==SDOC====================================================================
    //==
    //==  SetPrivacyType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the privacy types requested to the privacy service provider.
    //
    //  Parameters:
    //    uPrivacyValues:
    //      The privacy header values that must be requested to the privacy
    //      service provider. These values are added to a Privacy header
    //      into outgoing packets. Many option-tags can be set by using the '|'
    //      operator.
    //
    //      Since no option-tag must be added with the 'none' privacy type,
    //      setting eNONE with other privacy type returns an error.
    //
    //      Setting only eCRITICAL also fails.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      This service has no manager. Maybe it was not set or Clear was
    //      called on the context.
    //
    //    resFE_INVALID_ARGUMENT:
    //      uPrivacyValues is invalid. Either eNONE is present with other
    //      option-tags, eCRITICAL is present alone, or the values were not taken
    //      from EPrivValue.
    //
    //    resS_OK:
    //      otherwise.
    //
    //  Description:
    //      This method permits to change the privacy header values that must be
    //      added to the Privacy header of outgoing packets (requests and
    //      responses).
    //
    //      By default, the 'header' and 'id' privacy types are requested.
    //
    //      RFC 3323 states that no other option-tag must be present with the
    //      "none" option-tag. To be compliant with that, when a packet already
    //      contains a Privacy header and this service updates it, the service
    //      replaces all the privacy types of the header by 'none' if the
    //      configured privacy type is eNONE. For example, "Privacy: abc" is
    //      replaced by "Privacy: none" when the configured privacy type is
    //      eNONE.
    //
    //      Also, if the configured privacy type is not eNONE, the privacy
    //      type(s) is appended to the existing one. For example,
    //      "Privacy: abc" becomes "Privacy: abc;header;session" with
    //      eHEADER | eSESSION configured as privacy types.
    //
    //      Note that the Privacy header is not added to CANCEL requests.
    //
    //      This configuration only affects the instance upon that it is
    //      called.
    //
    //  Example:
    //    The following shows how this method should be called.
    //  <CODE>
    //
    //        // Enable "header", "session" and "critical" option-tags
    //        pSvc->SetPrivacyType(eHEADER | eSESSION | eCRITICAL);
    //
    //  </CODE>
    //
    //  See Also:
    //      ISipPrivacySvc::EPrivValue
    //
    //==EDOC====================================================================
    virtual mxt_result SetPrivacyType(IN unsigned int uPrivacyValues) = 0;
    
    //==SDOC====================================================================
    //==
    //==  EnableHeaderRemoval
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables the service's removal of certain headers.
    //
    //  Parameters:
    //    bPrivacy:
    //      True to activate service privacy, false to deactivate it.
    //
    //  Description:
    //      This method is used to activate or deactivate the service's innate 
    //      behaviour of stripping specific SIP headers automatically:
    //        - Call info;
    //        - Organization;
    //        - Reply to;
    //        - User agent;
    //        - In reply to (request only);
    //        - Subject (request only);
    //        - Server (response only);
    //        - Warning (response only).
    //
    //      In certain circumstances, when an application wants to downgrade 
    //      the privacy level requested from the server, it may  want to keep
    //      these headers. It must call this method and set bPrivacy to false.
    //
    //  See also:
    //      SetPrivacyType
    //
    //==EDOC====================================================================
    virtual void EnableHeaderRemoval(IN bool bPrivacy) = 0;

    //==SDOC====================================================================
    //==
    //==  EnableService
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables or disables this service.
    //
    //  Parameters:
    //    bEnable:
    //      True to enable the service, false to disable it.
    //
    //  Description:
    //      This method allows enabling/disabling this service in order to
    //      dynamically control the inclusion of Privacy headers to SIP requests
    //      and responses.
    //
    //      When the service is disabled, the outgoing packet is not updated
    //      (i.e. no privacy is requested nor applied) and the incoming requests
    //      are not inspected to see if they come from a privacy service.
    //
    //      By default, this service is enabled.
    //
    //  See also:
    //      SetPrivacyType
    //
    //==EDOC====================================================================
    virtual void EnableService(IN bool bEnable) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipPrivacySvc(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipPrivacySvc(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipPrivacySvc(IN const ISipPrivacySvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipPrivacySvc& operator=(IN const ISipPrivacySvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPPRIVACYSVC_H

