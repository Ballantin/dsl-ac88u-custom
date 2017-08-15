//==============================================================================
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
//==============================================================================
#ifndef MXG_ISIPDIGESTCLIENTAUTHMGR_H
#define MXG_ISIPDIGESTCLIENTAUTHMGR_H

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

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT to be able to use this \
class.
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
class CString;
class ISipClientEventControl;
class ISipDigestClientAuthSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipDigestClientAuthMgr
//===========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   The ISipDigestClientAuthMgr interface is the interface through which the
//   ISipDigestClientAuthSvc reports events to the application. The SIP stack
//   uses this interface to inform the application that the user is being
//   challenged by a remote party to authenticate itself.
//
// Location:
//   SipUserAgent/ISipDigestClientAuthMgr.h
//
//  See Also:
//   ISipDigestClientAuthSvc
//
//==============================================================================
class ISipDigestClientAuthMgr
{
// Friend Declarations
//-------------------------

// Published Interface
//-------------------------
public:
    //==========================================================================
    //==
    //==  EvCredentialsRequired
    //==
    //==========================================================================
    //
    //  Summary:
    //      One or more remote servers requested authentication and the service
    //      does not have the corresponding credentials.
    //
    //  Parameters:
    //    pSvc:
    //      The service that has detected the challenge and will be handling it.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rResponse:
    //      The 401 or 407 response that contains the challenge(s).
    //
    //  Description:
    //      Informs the application that one or more remote servers have
    //      requested authentication from the local user agent.
    //
    //      This event is reported when the service has no credentials for one
    //      or more challenges received. The manager must therefore provide
    //      authentication information for the request to progress further.
    //
    //      The manager can access the list of challenges by using the
    //      ISipDigestClientAuthSvc::GetChallenges method. This list contains
    //      ALL challenges received, including the ones for which the service
    //      may have valid credentials. The m_strHashA1 member of
    //      SChallengeData may be used to find the challenges for which there
    //      are no available credentials yet. Calling m_strHashA1.IsEmpty()
    //      returns true for challenges for which the service has no
    //      credentials.
    //
    //      Once the manager has obtained the credentials for a challenge, it
    //      must use the ISipDigestClientAuthSvc::Authenticate method to
    //      configure the credentials. This should be called once for each
    //      challenges for which the service does not have corresponding
    //      credentials.
    //
    //      After all credentials are provided, the manager should use
    //      ISipClientEventControl::ReIssueRequest to retry the request with the
    //      new credentials.
    //
    //  See Also:
    //      ISipDigestClientAuthSvc::GetChallenges,
    //      ISipDigestClientAuthSvc::Authenticate,
    //      ISipClientEventControl::ReIssueRequest
    //
    //==========================================================================
    virtual void EvCredentialsRequired(
                                    IN ISipDigestClientAuthSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CSipPacket& rResponse) = 0;



    //==========================================================================
    //==
    //==  EvCredentialsExist
    //==
    //==========================================================================
    //
    //  Summary:
    //      One ore more servers requested authentication and the service
    //      already has corresponding credentials.
    //
    //  Parameters:
    //    pSvc:
    //      The service that has detected the challenge and will be handling it.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rResponse:
    //      The 401 or 407 response that contains the challenge(s).
    //
    //  Description:
    //      Informs the application that one or more remote servers have
    //      requested authentication from the local user agent.
    //
    //      This event is reported when the service has the corresponding
    //      credentials for all the challenges received. It is possible that the
    //      credentials currently used by the service are invalid, however
    //      depending on the network setup, it may be impossible to detect
    //      invalid credentials. Thus, the manager should call
    //      ISipClientEventControl::ReIssueRequest() and rely on
    //      EvInvalidCredentials and EvAuthLoop for detecting authentication
    //      problems.
    //
    //  See Also:
    //      ISipClientEventControl::ReIssueRequest
    //
    //==========================================================================
    virtual void EvCredentialsExist(IN ISipDigestClientAuthSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvInvalidCredentials
    //==
    //==========================================================================
    //
    //  Summary:
    //      The service has detected credentials that are always rejected.
    //
    //  Parameters:
    //    pSvc:
    //      The service that has detected the challenge and will be handling it.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rstrRealm:
    //      The realm for which the credentials are invalid.
    //
    //    rResponse:
    //      The 401 or 407 response that contains the challenge(s).
    //
    //  Description:
    //      The authentication service has detected that the currently
    //      configured credentials for the realm rstrRealm are invalid. This
    //      situation is detected when the service receives a challenge twice in
    //      a row from the same realm.
    //
    //      The manager should either provide new credentials for the specified
    //      realm or call ISipClientEventControl::CallNextClientEvent() to
    //      abandon trying to send the request.
    //
    //  See Also:
    //      ISipClientEventControl::CallNextClientEvent,
    //      ISipDigestClientAuthSvc::Authenticate
    //
    //==========================================================================
    virtual void EvInvalidCredentials(
                                    IN ISipDigestClientAuthSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CString& rstrRealm,
                                    IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvAuthLoop
    //==
    //==========================================================================
    //
    //  Summary:
    //      The service has detected a possible authentication loop.
    //
    //  Parameters:
    //    pSvc:
    //      The service that has detected the authentication loop.
    //
    //      pClientEventCtrl:
    //       The interface to access client event related functionality.
    //
    //      rResponse:
    //       The 401 or 407 response that contains the challenge(s).
    //
    //  Description:
    //      The service has detected too many consecutive challenges that may
    //      lead to a possible authentication loop. This situation can occur
    //      with multiple proxies that have short nonce lifetime.
    //
    //      When this situation occurs, the manager stops trying to send this
    //      request by calling ISipClientEventControl::CallNextClientEvent.
    //
    //      If the manager was caching the user credentials and using them
    //      throughout more than one context, it should clear its cache for all
    //      the realms that were challenging the current request. Use
    //      ISipDigestClientAuthSvc::GetChallenges to access this list.
    //
    //  See Also:
    //      ISipDigestClientAuthSvc::GetChallenges,
    //      ISipDigestClientAuthSvc::SetLoopThreshold.
    //
    //==========================================================================
    virtual void EvAuthLoop(IN ISipDigestClientAuthSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvInvalidResponseCredentials
    //==
    //==========================================================================
    //
    //  Summary:
    //      The service has detected invalid or missing credentials in the 2xx
    //      response.
    //
    //  Parameters:
    //    pSvc:
    //      The service that has detected the invalid response credentials.
    //
    //      pClientEventCtrl:
    //       The interface to access client event related functionality.
    //
    //      rvecresReason:
    //       List of results or reasons for all servers and proxy
    //       authentication.
    //
    //       The list contains the results in this order:
    //          - Results for Authentication-Info, in header order.
    //          - Results for Proxy-Authentication-Info, in header order.
    //          - Other results.
    //
    //       For each header, one of these results will be set:
    //          - resS_OK: The header and its authentication data is valid
    //                     (corresponding server or the proxy is successfully
    //                     authenticated).
    //
    //          - resSW_WARNING: The header has authentication data but it does
    //                           not correspond to any sent challenge. The
    //                           header was ignored.
    //
    //          - resFE_UNKNOWN_QOP: The header did not contain a qop.
    //
    //          - resFE_FAIL: The header did not contain valid credentials for
    //                        the corresponding server or proxy.
    //
    //       These additional results can be found after header information:
    //          - resFE_NOT_FOUND: No header associated with sent credentials
    //                             (Authentication-Info for Authorization or
    //                             Proxy-Authentication-Info for
    //                             Proxy-Authorization) was found in the
    //                             received response. This result is inserted in
    //                             the order of the sent credentials headers
    //                             (regardless of their type (i.e. Authorization
    //                             or Proxy-Authorization)).
    //
    //      rResponse:
    //       The 2xx response containing invalid credentials or no credentials
    //       at all.
    //
    //  Description:
    //      Informs the application that it might not be the right server which
    //      sent the response.
    //
    //      This event is reported when the service receives a 2xx response to a
    //      request but that authentication informations in the packet is
    //      invalid or missing.
    //
    //      Note that the service that sent the initial request sends a final
    //      event after this event. The manager MUST call
    //      ISipClientEventControl::CallNextClientEvent() to receive the next
    //      event (for the success response) or
    //      ISipClientEventControl::ClearClientEvents() to clear all other
    //      events.
    //
    //      This method will only be reported if
    //      ISipDigestClientAuthSvc::EnableMutualAuthentication has been called
    //      with true.
    //
    //  See Also:
    //      ISipDigestClientAuthSvc::EnableMutualAuthentication,
    //      ISipClientEventControl::CallNextClientEvent() and
    //      ISipClientEventControl::ClearClientEvents()
    //
    //==========================================================================
    virtual void EvInvalidResponseCredentials(IN ISipDigestClientAuthSvc* pSvc,
                                              IN ISipClientEventControl* pClientEventCtrl,
                                              IN const CVector<mxt_result>& rvecresReason,
                                              IN const CSipPacket& rResponse) = 0;

// BRCM_CUSTOM - [add] Add server digest authentication support
    //==========================================================================
    //==
    //==  EvSvrAuthenticationFailed
    //==
    //==========================================================================
    //
    //  Summary:
    //      The service has detected a failed server authentication.
    //
    //  Parameters:
    //      pSvc:
    //       The service that has detected the failed server authentication.
    //
    //      pClientEventCtrl:
    //       The interface to access client event related functionality.
    //
    //      rResponse:
    //       The 2xx response that contains the response-digest.
    //
    //  Description:
    //      The service has detected a failed server authentication, i.e.
    //      an invalid response-digest, in the 2xx response received from the
    //      authentication server.
    //
    //==========================================================================
    virtual void EvSvrAuthenticationFailed(IN ISipDigestClientAuthSvc* pSvc,
                                           IN ISipClientEventControl* pClientEventCtrl,
                                           IN const CSipPacket& rResponse) = 0;
// BRCM_CUSTOM - [end] Add server digest authentication support

// BRCM_CUSTOM - [add] Add authentication info sharing support
    //==========================================================================
    //==
    //==  EvAuthenticationSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      The service has an authentication successful response.
    //
    //  Parameters:
    //      pSvc:
    //       The service that has recieved an authentication successful response.
    //
    //      pClientEventCtrl:
    //       The interface to access client event related functionality.
    //
    //      rResponse:
    //       The 2xx response that contains the credentials.
    //
    //==========================================================================
    virtual void EvAuthenticationSuccess(IN ISipDigestClientAuthSvc* pSvc,
                                         IN ISipClientEventControl* pClientEventCtrl,
                                         IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvNonceEmployed
    //==
    //==========================================================================
    //
    //  Summary:
    //      The service has detected nonce usage in a request.
    //
    //  Parameters:
    //      pSvc:
    //       The service that has added the nonce value.
    //
    //      rRequest:
    //       The 2xx response that contains the credentials.
    //
    //==========================================================================
    virtual void EvNonceEmployed(IN ISipDigestClientAuthSvc* pSvc,
                                         IN const CSipPacket& rRequest) = 0;

// BRCM_CUSTOM - [end] Add authentication info sharing support

// Hidden Methods
//----------------
protected:
    // << Constructors >>
    //--------------------
    ISipDigestClientAuthMgr() {}

    // << Destructors >>
    //-------------------
    virtual ~ISipDigestClientAuthMgr() {}

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    ISipDigestClientAuthMgr(IN const ISipDigestClientAuthMgr& rSource);

    ISipDigestClientAuthMgr& operator=(
                                     IN const ISipDigestClientAuthMgr& rSource);
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPDIGESTCLIENTAUTHMGR_H

