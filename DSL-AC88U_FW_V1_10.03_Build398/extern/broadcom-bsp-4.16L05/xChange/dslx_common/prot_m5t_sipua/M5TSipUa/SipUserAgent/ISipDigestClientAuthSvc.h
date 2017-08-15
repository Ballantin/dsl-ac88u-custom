//==============================================================================
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
//==============================================================================
#ifndef MXG_ISIPDIGESTCLIENTAUTHSVC_H
#define MXG_ISIPDIGESTCLIENTAUTHSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT to be able to use this \
class.
#endif


//-- Data Members
//-----------------

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class ISipDigestClientAuthMgr;
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipDigestClientAuthSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipDigestClientAuthSvc
//=============================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   This service helps the application authenticate itself to a remote
//   party that challenges it. It monitors transactions for 401 (Unauthorized)
//   and 407 (Proxy Authentication Required) responses and then generates events
//   to the application when required. It supports Digest authentication.
//
//   Adding an ISipDigestClientAuthSvc to a SIP context makes it possible to
//   easily respond to challenges for any SIP request, even one that does not
//   exist yet!
//
//   If an application chooses not to add this service to a context, then it is
//   up to the application to detect challenges and to properly build the
//   credentials for the next packet to be sent. This interface simply makes
//   this process much easier.
//
//   This service generates events to the application through the
//   ISipDigestClientAuthMgr interface.
//
//#The ISipDigestClientAuthMgr is an ECOM object#
//
//  The ISipDigestClientAuthMgr is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipDigestClientAuthMgr</B>
//
//  <B>Interface Id: IID_ISipDigestClientAuthMgr</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
//  Location:
//      SipUserAgent/ISipDigestClientAuthSvc.h
//
//  See Also:
//      ISipDigestClientAuthMgr
//
//==============================================================================
class ISipDigestClientAuthSvc : public IEComUnknown
{
// Friend Declarations
//-------------------------

// Published Interface
//-------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipDigestClientAuthSvc);
    //M5T_INTERNAL_USE_END

    // Description:
    //  This structure contains parameters used in Proxy-Authenticate,
    //  WWW-Authenticate, Proxy-Authorization and Authorization headers.
    struct SChallengeData
    {
        // M5T_INTERNAL_USE_BEGIN
        SChallengeData()
        :   m_pstrLastSentUriParamValue(NULL),
            m_uNonceCount(0),
            m_uChallengeType(0xFFFFFFFF),
            m_bQopAuthSupported(false),
            m_bQopAuthIntSupported(false),
            m_bSuccessfullyAuthenticated(false)
        {
        }

// BRCM_CUSTOM - [add] Add authentication info sharing support
        SChallengeData(const SChallengeData& rSrc)
        : m_strRealm(rSrc.m_strRealm),
          m_strOpaque(rSrc.m_strOpaque),
          m_strNonce(rSrc.m_strNonce),
          m_strAlgorithm(rSrc.m_strAlgorithm),
          m_strQop(rSrc.m_strQop),
          m_strUsername(rSrc.m_strUsername),
          m_strHashA1(rSrc.m_strHashA1),
          m_strCNonce(rSrc.m_strCNonce),
          m_strAuthHeader(rSrc.m_strAuthHeader),
          m_strDigestUri(rSrc.m_strDigestUri),
          m_strMd5SessHashA1(rSrc.m_strMd5SessHashA1),
          m_uNonceCount(rSrc.m_uNonceCount),
          m_uChallengeType(rSrc.m_uChallengeType),
          m_bQopAuthSupported(rSrc.m_bQopAuthSupported),
          m_bQopAuthIntSupported(rSrc.m_bQopAuthIntSupported),
          m_bSuccessfullyAuthenticated(rSrc.m_bSuccessfullyAuthenticated)
        {
            if (rSrc.m_pstrLastSentUriParamValue != NULL)
            {
               m_pstrLastSentUriParamValue = MX_NEW(CString)(*rSrc.m_pstrLastSentUriParamValue);
            }
            else
            {
               m_pstrLastSentUriParamValue = NULL;
            }
        }
// BRCM_CUSTOM - [end] Add authentication info sharing support

        ~SChallengeData()
        {
            MX_DELETE(m_pstrLastSentUriParamValue);
            m_pstrLastSentUriParamValue = NULL;
        }
        // M5T_INTERNAL_USE_END

        // "realm" parameter.
        //--------------------
        CString m_strRealm;

        // "opaque" parameter.
        //---------------------
        CString m_strOpaque;

        // "nonce" parameter.
        //--------------------
        CString m_strNonce;

        // "algorithm" parameter.
        //------------------------
        CString m_strAlgorithm;

        // "qop" parameter.
        //------------------
        CString m_strQop;

        // "username" parameter.
        //-----------------------
        CString m_strUsername;

        // Hashing of username, realm and password used in "response"
        // parameter.
        //------------------------------------------------------------
        CString m_strHashA1;

        // "cnonce" parameter.
        //---------------------
        CString m_strCNonce;

        // For a Proxy-Authorization or Authorization header, all the
        // digest-response parameters of the header. For a Proxy-Authenticate
        // or WWW-Authenticate header, all the digest-cln parameters of the
        // header. This is used to send the same credentials in the INVITE than
        // in the ACK.
        CString m_strAuthHeader;

// BRCM_CUSTOM - [add] Add server digest authentication support
        // "digest-uri-value" of a previously sent packet.
        //-------------------------------------------------
        CString m_strDigestUri;
// BRCM_CUSTOM - [end] Add server digest authentication support

// BRCM_CUSTOM - [add] Add authentication info sharing support
        // Part of the hashed A1 value for MD5-sess of a previously sent packet.
        //-----------------------------------------------------------------------
        CString m_strMd5SessHashA1;
// BRCM_CUSTOM - [end] Add authentication info sharing support

        // M5T_INTERNAL_USE_BEGIN
        // This member is used to keep the value of the uri parameter sent in
        // the last Proxy-Authenticate or WWW-Authenticate header. The quotation
        // marks surrounding the parameter value have been removed. This member
        // is used in digest mutual authentication. This member is only used
        // when mutual authentication is enabled.
        CString* m_pstrLastSentUriParamValue;
        // M5T_INTERNAL_USE_END

        // "nc" parameter.
        //-----------------
        unsigned int m_uNonceCount;

        // Challenge type (user agent, proxy).
        //-------------------------------------
        unsigned int m_uChallengeType;

        // Whether "auth" is included in qop.
        //------------------------------------
        bool m_bQopAuthSupported;

        // Whether "auth-int" is included in qop.
        //----------------------------------------
        bool m_bQopAuthIntSupported;

        // Whether authentication succeeded.
        //-----------------------------------
        bool m_bSuccessfullyAuthenticated;
    };

    // Description:
    //  The different possible behaviors of the ISipDigestClientAuthSvc upon
    //  reception of an unknown qop parameter value in the WWW-Authenticate
    //  or Proxy-Authenticate header.
    enum EUnknownQopBehavior
    {
        // Description:
        //  Upon reception of an unknown qop value the request will fail and a
        //  re-issued request will not contain any response for the received
        //  challenge. This is the default behavior.
        eFAIL_ON_UNKNOWN_QOP,

        // Description:
        //  Upon reception of an unknown qop value the ISipDigestClientAuthSvc will
        //  act exactly as if it did not receive any qop parameter value in the
        //  WWW-Authenticate or Proxy-Authenticate header. This means that when
        //  the request is reissued the challenge response will be generated as
        //  if no qop parameter was present in the challenge.
        //
        //  Please note the following RFC quotations before the behavior is
        //  chosen:
        //
        //  RFC 2617 section 3.2.1 states that: "Unrecognized options MUST be
        //  ignored."
        //
        //  While RFC3261 section 22.4 bullet 8 states that: "If a client
        //  receives a "qop" parameter in a challenge header field, it MUST
        //  send the "qop" parameter in any resulting authorization header
        //  field."
        eIGNORE_UNKNOWN_QOP
    };

    // Description:
    //  The behaviour to adopt when resetting the credentials.
    enum EResetBehaviour
    {
        // Description:
        //  Reset all credentials.
        eRESET_ALL,
        // Description:
        //  Reset all credentials except those associated to the last challenged received.
        eKEEP_LAST_CHALLENGE
    };

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the event manager associated with this service manager.
    //
    //  Parameters:
    //    pMgr:
    //      The event manager. Must not be NULL.
    //
    //  Description:
    //      Configures the event manager that receives the events generated
    //      by this service manager.
    //
    //      Note that an event manager MUST be associated with this service
    //      manager before it is used.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN ISipDigestClientAuthMgr* pMgr) = 0;


    //==========================================================================
    //==
    //==  SetLoopThreshold
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the maximum number of consecutive challenges the service
    //      may handle before reporting EvAuthLoop.
    //
    //  Parameters:
    //    uMax:
    //      The maximum number of consecutive challenges the service may handle.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: uMax is too low. Must be higher than 2.
    //
    //      - resS_OK: Maximum correctly configured.
    //
    //  Description:
    //      This configures the maximum number of consecutive 401/407 challenges
    //      the service may handle before reporting the EvAuthLoop event to the
    //      manager. 401/407 challenges are considered consecutive when they are
    //      not separated by a success response (2xx).
    //
    //      A loop is detected when the uMax consecutive 401/407 challenges
    //      are received. For instance, if uMax is 10, when the 10th 401/407
    //      response is received since either the first or last 2xx response,
    //      the EvAuthLoop event is issued.
    //
    //      The default value is 10 when this configuration method is not
    //      called.
    //
    //==========================================================================
    virtual mxt_result SetLoopThreshold(unsigned int uMax) = 0;

    //==========================================================================
    //==
    //==  Authenticate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the user name and hash of A1 for a specific realm.
    //
    //  Parameters:
    //    rstrRealm:
    //      The realm for which the user name and hash of A1 are set.
    //
    //    rstrUserName:
    //      The user name that should be used to authenticate the user.
    //
    //    rstrHashA1:
    //      The hash of A1 containing the password corresponding to that user
    //      name.
    //
    //  Returns:
    //     resFE_FAIL:
    //       No data was present for this realm.
    //
    //     resFE_DUPLICATE:
    //       Credentials were already present for this realm, which means they
    //       had been set previously and possibly a request was sent containing
    //       them.
    //
    //     resS_OK:
    //       Credentials were added successfully for this realm.
    //
    //  Description:
    //      This must be called by the application to set authentication
    //      information after the service manager has generated the
    //      EvAuthenticationRequired event.
    //
    //      It configures the client authentication service with the user name
    //      and hash of A1 it should use to authenticate the user at the
    //      remote party.
    //
    //      It returns an error if there was no challenge received in the
    //      parent context or a warning if the user name and hash of A1 had
    //      previously been tried unsuccessfully.
    //
    //      Every future requests in the parent context will include credentials
    //      based on these user name and hash of A1 until a new user name and a
    //      new hash of A1 are set through this method.
    //
    //      After configuring the service manager with a user name and a hash of
    //      A1, the application must re-send the request that was challenged,
    //      which will then contain the proper credentials.
    //
    //==========================================================================
    virtual mxt_result Authenticate(IN const CString& rstrRealm,
                                    IN const CString& rstrUsername,
                                    IN const CString& rstrHashA1) = 0;

    //==========================================================================
    //==
    //== GetChallenges
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the challenges that currently require authentication, if any.
    //
    //  Returns:
    //      Challenges that currently require authentication.
    //
    //  Description:
    //      Gets the challenges that currently require authentication, if any.
    //      Must not be called if no challenge was received. The application
    //      is informed via an event EvAuthRequired or EvChallengedAfterSuccess
    //      on interface ISipDigestClientAuthMgr that a challenge was received.
    //
    //==========================================================================
    virtual const CVector<SChallengeData*>& GetChallenges() const = 0;

    //==========================================================================
    //==
    //==  Reset
    //==
    //==========================================================================
    //
    //  Summary:
    //      Clears the authentication state.
    //
    //  Parameters:
    //      eResetBehaviour:
    //       The behaviour to adopt when resetting the credentials.
    //
    //  Description:
    //      The client authentication service automatically reuses the last
    //      successful credentials for any new request that is sent. In some
    //      circumstances, this behaviour is not desirable.
    //
    //      When called with eResetBehaviour set to eKEEP_LAST_CHALLENGE, the
    //      Reset() method clears any credentials, hashes of A1 and
    //      authentication state except for the last challenge received. This
    //      will have the effect that new requests sent on the dialog will
    //      contain only the last credentials.
    //
    //      The best moment to Reset() the credentials is before sending a new
    //      request.
    //
    //==========================================================================
    virtual void Reset(IN ISipDigestClientAuthSvc::EResetBehaviour eResetBehaviour = ISipDigestClientAuthSvc::eRESET_ALL) = 0;

    //==========================================================================
    //==
    //==  SetRouteUriAuthentication
    //==
    //==========================================================================
    // <FLAG new in 4.1.4>
    //
    //  Summary:
    //      Sets the service's behaviour for using the route URI instead of the
    //      request URI.
    //
    //  Parameters:
    //      bUseRouteUri:
    //       True to have the authentication service use the route.
    //
    //  Description:
    //      Sets the service's behaviour for choosing which URI it will use in
    //      the Proxy-Authorization header. If false, the service will use the
    //      default request URI of the SIP packet before any route headers
    //      are applied to it. This is the default behaviour. If set to true
    //      the URI in the Proxy-Authorization header will contain the URI set
    //      in the first Route header. When no route header can be found and
    //      bUseRouteUri is true, the default behaviour will be used. That is
    //      the packet's REQUEST-URI will be used to perform the authentication.
    //
    //==========================================================================
    virtual void SetRouteUriAuthentication(IN bool bUseRouteUri) = 0;

    //==========================================================================
    //==
    //==  EnableQualityOfProtectionAuthInt
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the quality of protection sent in challenges.
    //
    //  Parameters:
    //    bEnable:
    //      A boolean to enable or disable the auth-int authentication method.
    //
    //  Description:
    //      Configures the usage of the auth-int authentication method.
    //
    //      As explained in RFC 2617, disabling the auth-int will change
    //      the way that A2 is computed.
    //
    //      Also, if a challenge has qop=auth,auth-int and auth-int is
    //      disabled, the client will only set a qop=auth in the Authorization
    //      header.
    //
    //==========================================================================
    virtual void EnableQualityOfProtectionAuthInt(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  EnableMutualAuthentication
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures support for mutual digest authentication.
    //
    //  Parameters:
    //    bEnable:
    //      - true: enables 2xx response inspection for server authentication
    //              information when the request included credentials.
    //      - false: disables 2xx response inspection of server authentication
    //               information.
    //
    //  Description:
    //      This method enables inspection of received 2xx responses for
    //      server/proxy authentication information as explained in RFC 2617
    //      section 3.2.3.
    //
    //      Server and/or proxy authentication information is found in
    //      Authentication-Info and/or Proxy-Authentication-Info header(s) of
    //      2xx responses.
    //
    //      When enabled, ISipDigestClientAuthMgr::EvInvalidResponseCredentials
    //      is reported when:
    //          - no authentication information is found in a 2xx response for a
    //            request containing credentials.
    //          - invalid authentication information has been received in a 2xx
    //            response for a request containing credentials.
    //
    //      If the server/proxy is successfully authenticated, the value of the
    //      "nextnonce" parameter in the Authentication-Info or
    //      Proxy-Authentication-Info header will be used in the next sent
    //      request as the "nonce" value (as defined in section 3.2.3 of
    //      RFC 2617).
    //
    //      IMPORTANT: 1xx and failure negative responses are not authenticated
    //                 since RFC 3261 states that the Authentication-Info header
    //                 is only applicable for 2xx responses (it is considered
    //                 that the Proxy-Authentication-Info header (only defined
    //                 in RFC 2617) is the same).
    //
    //==========================================================================
    virtual void EnableMutualAuthentication(IN bool bEnable) = 0;

    //===========================================================================
    //==
    //==  UpdateDigest
    //==
    //===========================================================================
    //
    //  Parameters:
    //      rPacket:
    //       The packet for which to update the digest.
    //
    //  Returns:
    //      - resS_OK:
    //          The digest has been updated.
    //      - resFE_FAIL:
    //          The digestion computation failed.
    //
    //  Description:
    //      This method updates the Authorization header response and removes any
    //      preceding ones. A typical case for using this feature is when the
    //      auth-int quality of protection is applied to create the response and 
    //      the payload has been modified afterwards (e.g. by the 
    //      ISipCoreOutputControllingMgr). The digest has to be recalculated for 
    //      the authentication to succeed.
    //
    //===========================================================================
    virtual mxt_result UpdateDigest(INOUT CSipPacket& rPacket) = 0;

// BRCM_CUSTOM - [add] Add authentication info sharing support
    //==SDOC====================================================================
    //==
    //==  UpdateChallenge
    //==
    //==========================================================================
    //
    //  Summary:
    //      Update the service's challenge vector with the provided challenge.
    //
    //  Parameters:
    //    rChallenge:
    //      Challenge to be added (or updated) in the service's challenge
    //      vector.
    //
    //  Description:
    //      Add the provided challenge to the service's challenge vector if
    //      the provided challenge contains a realm which does not match any
    //      the challenge vector has. Otherwise, the specific challenge in the
    //      challenge vector will be updated with the provided challenge.
    //
    //==EDOC====================================================================
    virtual void UpdateChallenge(IN const SChallengeData& rChallenge) = 0;

    //==SDOC====================================================================
    //==
    //==  SetChallengeCount
    //==
    //==========================================================================
    //
    //  Summary:
    //      Set the nonce-count of the service's challenge only if it contains
    //      the same nonce and realm of the ones in the provided challenge.
    //
    //  Parameters:
    //    rChallenge:
    //      Challenge which contains the nonce and realm to perform the
    //      matching.
    //
    //    uNonceCount:
    //      The nonce-count to be updated in the service's challenge.
    //
    //  Returns:
    //      False if the service's challenge vector does not contain any
    //      challenge that matches with the provided challenge.
    //      True if the nonce-count is updated successfully.
    //
    //==EDOC====================================================================
    virtual bool SetChallengeCount(IN const SChallengeData& rChallenge,
                                   IN unsigned int uNonceCount) = 0;

    //==SDOC====================================================================
    //==
    //==  SetChallengeType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Set the challenge type of the service's challenge only if it
    //      contains the same realm and nonce of the ones in the provided
    //      challenge.
    //
    //  Parameters:
    //    rChallenge:
    //      Challenge which contains the nonce and realm to perform the
    //      matching.
    //
    //    uChallengeType:
    //      The challenge type to be updated in the service's challenge.
    //
    //  Returns:
    //      False if the service's challenge vector does not contain any
    //      challenge that matches with the provided challenge.
    //      True if the challenge type is updated successfully.
    //
    //==EDOC====================================================================
    virtual bool SetChallengeType(IN const SChallengeData& rChallenge,
                                  IN unsigned int uChallengeType) = 0;
// BRCM_CUSTOM - [end] Add authentication info sharing support

// Hidden Methods
//-------------------------
protected:
    // << Constructors / Destructors >>
    //----------------------------------
    ISipDigestClientAuthSvc() {}

    virtual ~ISipDigestClientAuthSvc() {}

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    ISipDigestClientAuthSvc(IN const ISipDigestClientAuthSvc& rSource);

    ISipDigestClientAuthSvc& operator=(IN const ISipDigestClientAuthSvc& rSource);

// Hidden Data Members
//-------------------------
protected:
private:
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPDIGESTCLIENTAUTHSVC_H

