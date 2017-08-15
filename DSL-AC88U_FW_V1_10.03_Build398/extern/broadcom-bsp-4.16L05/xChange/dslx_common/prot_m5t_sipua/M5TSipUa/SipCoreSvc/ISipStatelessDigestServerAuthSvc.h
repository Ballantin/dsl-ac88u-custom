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
#ifndef MXG_ISIPSTATELESSDIGESTSERVERAUTHSVC_H
#define MXG_ISIPSTATELESSDIGESTSERVERAUTHSVC_H

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


#ifndef MXD_SIPSTACK_ENABLE_SIP_STATELESS_DIGEST_SERVER_AUTH_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_STATELESS_DIGEST_SERVER_AUTH_SVC_SUPPORT to be able to \
use this class.
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
class CString;
class CSipPacket;
// BRCM_CUSTOM - [add] IMRG: generate AUTH header for remote user.
class CSipHeader;
// BRCM_CUSTOM - [end] IMRG: generate AUTH header for remote user.

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipStatelessDigestServerAuthSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipStatelessDigestServerAuthSvc
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//  The digest server authentication service is a stateless service that allows
//  to verify that incoming packets have been issued by a specific,
//  authenticated user.
//
//  A manager of this service should provide incoming packets to this service in
//  order to assert that the packet is correctly authenticated. This service
//  reports an event to its manager to let it know how to further proceed.
//
//  It can be used to send both 401 and 407 responses.
//
//  This service supports the MD5 algorithm along with "auth" and "auth-int"
//  quality of protection (qop).
//
//  In order to support the MD5-sess algorithm, the application must configure
//  the authentication lifetime to zero with SetAuthenticationLifetime. It must
//  also challenge incoming requests with the ChallengeRequest method that takes
//  a packet, algorithm, and nonce as parameters.
//
//  The MD5-sess algorithm can be used with backend authentication services like
//  diameter or radius.
//
//#The ISipStatelessDigestServerAuthSvc is an ECOM object#
//
//  The ISipStatelessDigestServerAuthSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipStatelessDigestServerAuthSvc</B>
//
//  <B>Interface Id: IID_ISipStatelessDigestServerAuthSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipStatelessDigestServerAuthSvc.h
//
//==EDOC========================================================================
class ISipStatelessDigestServerAuthSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//   Public Types
//-----------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipStatelessDigestServerAuthSvc);
    //M5T_INTERNAL_USE_END

    // Description:
    //  Indicates the quality of protection requested in challenges. This
    //  is an optional parameter in RFC 2617 to remain backward compatible
    //  with RFC 2069.
    enum EQualityOfProtection
    {
        // Description:
        //  Don't use authentication.
        eNone = 0x00000000,

        // Description:
        //  Use the authentication scheme.
        eAuth = 0x00000001,

        // Description:
        //  Use the authentication with integrity protection.
        eAuthInt = 0x00000002,

        // Description:
        // The combination of all possible qops, for validation purposes.
        eQopMask = 0x00000003
    };

    // Description:
    //  Indicates the algorithm to use for the challenge.
    enum EAlgorithm
    {
        // Description:
        //  Use the MD5 algorithm.
        eMD5,
        // Description:
        //  Use the MD5-session algorithm.
        eMD5_Session
    };

//-- Published Interface
//------------------------
public:
    //==SDOC====================================================================
    //==
    //==  SetSupportedRealm
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the realm supported by this device.
    //
    //  Parameters:
    //    strRealm:
    //      The realm supported by this service.
    //
    //  Description:
    //      This method configures the realm for which this service manages
    //      authentication. Incoming requests are searched for authentication
    //      information for this specific realm.
    //
    //      No default configuration exists for this setting, it MUST be called
    //      with the proper realm for this service to work correctly.
    //
    //==EDOC====================================================================
    virtual void SetSupportedRealm(IN const CString& rstrRealm) = 0;

    //==SDOC====================================================================
    //==
    //==  SetAuthLifetime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the time before authentication information becomes stale.
    //
    //  Parameters:
    //    uExpirationMin:
    //      The time, in minutes, that the stack should consider a nonce valid.
    //      If zero, the stack considers all nonces it receives as valid and
    //      never re-challenges a request because of stale authentication
    //      information.
    //
    //  Description:
    //      This method allows to configure the length of time that
    //      authentication information provided by the stack is to be considered
    //      valid. This serves as a mean to periodically re-challenge incoming
    //      requests, which makes some attacks somewhat harder.
    //
    //      If uExpirationMin is configured to zero, this servive does not
    //      manage stale authentication information. The application may do so
    //      if it wants.
    //
    //      This configures the time that a generated nonce is to remain valid.
    //      When this service receives a request that uses a nonce that has been
    //      generated by the stack but is too old, then a new challenge is
    //      automatically issued.
    //
    //      The default value for this is 5 minutes.
    //
    //==EDOC====================================================================
    virtual void SetAuthLifetime(IN unsigned int uExpirationMin) = 0;

    //==SDOC====================================================================
    //==
    //==  SetRequestedQualityOfProtection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the quality of protection sent in challenges.
    //
    //  Parameters:
    //    bsQopOptions:
    //      A bitset combination of the EQualityOfProtection values. eNone can
    //      only be used alone.
    //
    //  Returns:
    //    resS_OK:
    //      When the bitset contains accepted values.
    //
    //    resFE_INVALID_ARGUMENT:
    //      Otherwise.
    //
    //  Description:
    //      Configures the quality of protection requested in challenges. This
    //      is an optional parameter in RFC 2617 to remain backward compatible
    //      with RFC 2069.
    //
    //      When eNone is used, no qop option is sent in the challenge.
    //
    //  See Also:
    //      QopMustBePresent
    //
    //==EDOC====================================================================
    virtual
    mxt_result SetRequestedQualityOfProtection(IN unsigned int bsQopOptions) = 0;

    //==SDOC====================================================================
    //==
    //==  QopMustBePresent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures whether or not the presence of the qop parameter in the
    //      authorization header must be checked and its value validated, for
    //      backward compatibility purposes.
    //
    //  Parameters:
    //    bPresent:
    //      True if the qop parameter must be present, false otherwise.
    //
    //  Description:
    //      Configures whether or not the presence of the qop parameter in the
    //      authorization header must be checked and its value validated, for
    //      backward compatibility purposes.
    //
    //      In the affirmative, if the proper quality of protection is not met,
    //      the request is rejected with a "403 Forbidden" response.
    //
    //      Implementors must know that calling QopMustBePresent with true
    //      breaks the backward compatibility with RFC 2069.
    //
    //  See Also:
    //      SetRequestedQualityOfProtection
    //
    //==EDOC====================================================================
    virtual void QopMustBePresent(IN bool bPresent) = 0;

    //==SDOC====================================================================
    //==
    //==  SetPrivateKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the private key used in nonce encoding.
    //
    //  Parameters:
    //    rstrPrivateKey:
    //      The private key used in nonce encoding.
    //
    //  Description:
    //      Configures the private key used in nonce encoding.
    //
    //==EDOC====================================================================
    virtual void SetPrivateKey(IN const CString& rstrPrivateKey) = 0;

    // BRCM_CUSTOM - [add] IMRG: generate AUTH header for remote user.

    //==SDOC========================================================================
    //==
    //==  ChallengeRequest
    //==
    //==============================================================================
    //
    //  Parameters:
    //    rRequest:
    //      The request to challenge.
    //
    //    uChallengeType:
    //      The challenge type (the response's status code: uUNAUTHORIZED or
    //      uPROXY_AUTHENTICATION_REQUIRED).
    //
    //    bIsStale:
    //      Whether or not there is a stale nonce in the request.
    //
    //  Returns:
    //    resFE_INVALID_ARGUMENT:
    //      The packet is not a valid request.
    //
    //    resFE_INVALID_STATE:
    //      The parent context is not set.
    //
    //    resFE_FAIL:
    //      The challenge could not be sent by the transport layer for some reason.
    //
    //    resS_OK:
    //      The challenge has been successfully sent.
    //
    //  Description:
    //      Issues a challenge to the specified request. The response sent depends
    //      on the challenge type passed in parameter.
    //
    //      This method can only be used to issue challenges with the MD5 algorithm.
    //      The other version of ChallengeRequest must be used to challenge requests
    //      with the MD5-sess algorithm.
    //
    //      ACK and CANCEL requests must never be challenged.
    //
    //==EDOC========================================================================
    virtual void CreateChallengeHeader(IN const CSipPacket&  rRequest,
                                       IN ESipHeaderType  eHeaderType,
                                       IN EAlgorithm      eAlgorithm,
                                       IN bool            bIsStale,
                                       OUT CSipHeader**   ppAuthHeader) = 0;
    // BRCM_CUSTOM - [end] IMRG: generate AUTH header for remote user.                                   

    //==SDOC====================================================================
    //==
    //==  ChallengeRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Challenges the specified request.
    //
    //  Parameters:
    //    rRequest:
    //      The request to challenge.
    //
    //    uChallengeType:
    //      The challenge type (the response's status code: uUNAUTHORIZED or
    //      uPROXY_AUTHENTICATION_REQUIRED).
    //
    //    bIsStale:
    //      Whether or not there was a stale nonce in the request.
    //
    //  Returns:
    //    resFE_INVALID_ARGUMENT:
    //      The packet is not a valid request.
    //
    //    resFE_INVALID_STATE:
    //      The parent context is not set.
    //
    //    resFE_FAIL:
    //      The challenge could not be sent by the transport layer for some
    //      reason.
    //
    //    resS_OK:
    //      The challenge has been successfully sent.
    //
    //  Description:
    //      Issues a challenge to the specified request.
    //
    //      This method can only be used to issue challenges with the MD5
    //      algorithm. The other version of ChallengeRequest must be used to
    //      challenge requests with the MD5-sess algorithm.
    //
    //      ACK and CANCEL requests must never be challenged.
    //
    //==EDOC====================================================================
    virtual mxt_result ChallengeRequest(IN const CSipPacket& rRequest,
                                        IN unsigned int uChallengeType,
                                        IN bool bIsStale) = 0;

    //==SDOC====================================================================
    //==
    //==  ChallengeRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Challenges the specified request.
    //
    //  Parameters:
    //    rRequest:
    //      The request to challenge.
    //
    //    uChallengeType:
    //      The challenge type (the response's status code: uUNAUTHORIZED or
    //      uPROXY_AUTHENTICATION_REQUIRED).
    //
    //    eAlgorithm:
    //      The algorithm to use for the challenge.
    //
    //    rstrNonce:
    //      The new nonce to use for the challenge.
    //
    //    bIsStale:
    //      Whether or not there was a stale nonce in the request.
    //
    //  Returns:
    //    resFE_INVALID_ARGUMENT:
    //      The challenge type is wrong or the packet is not a valid request.
    //
    //    resFE_INVALID_STATE:
    //      The parent context is not set.
    //
    //    resFE_FAIL:
    //      The challenge could not be sent by the transport layer for some
    //      reason.
    //
    //    resS_OK:
    //      The challenge has been successfully sent.
    //
    //  Description:
    //      Challenges the specified request with the specified algorithm and
    //      nonce.
    //
    //      This method MUST be used when SetAuthenticationLifetime has been
    //      configured with a value of zero. This means the user of the service
    //      (or some other mechanism) is responsible for generating the nonce.
    //
    //==EDOC====================================================================
    virtual mxt_result ChallengeRequest(IN const CSipPacket& rRequest,
                                        IN unsigned int uChallengeType,
                                        IN EAlgorithm eAlgorithm,
                                        IN const CString& rstrNonce,
                                        IN bool bIsStale) = 0;

//==========================================================================
    //==
    //==  RejectRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Rejects the specified request.
    //
    //  Parameters:
    //    rRequest:
    //      The request to challenge.
    //
    //    uResponseType:
    //      The response type; the response's status code MUST be uFORBIDDEN
    //      otherwise the method will fail with resFE_INVALID_ARGUMENT.
    //
    //    pszReasonPhrase:
    //      Reason phrase to use in the sent response. If NULL, the default reason
    //      phrase for the specified uStatusCode is used.
    //
    //  Returns:
    //    resFE_INVALID_ARGUMENT:
    //      Response type is wrong or the packet is not a valid request.
    //
    //    resFE_INVALID_STATE:
    //      The parent context is not set.
    //
    //    resFE_FAIL:
    //      The response could not be sent by the transport layer for some
    //      reason.
    //
    //    resS_OK:
    //      The response was successfully sent.
    //
    //  Description:
    //      This method will have the effect of sending a final negative
    //      response to the received request. The received packet is then
    //      dropped and no further processing can be made on it.
    //
    //==========================================================================
    virtual mxt_result RejectRequest(IN const CSipPacket& rRequest,
                                     IN unsigned int uResponseType,
                                     IN const char* pszReasonPhrase) = 0;

    //==SDOC====================================================================
    //==
    //==  ContainsCredentials
    //==
    //==========================================================================
    //
    //  Summary:
    //      Verifies if the SIP packet contains credentials, and the validity of
    //      some of them.
    //
    //  Parameters:
    //    rRequest:
    //      The request that contains the credentials for the realm managed by
    //      this object.
    //
    //    rstrUsername:
    //      The username included in the request for the realm managed by this
    //      object.
    //
    //  Returns:
    //    resS_OK:
    //      The packet contains credentials.
    //
    //    resFE_FAIL:
    //      The packet does not contain credentials, or no (Proxy-)Authorization
    //      header with the right realm has been found.
    //
    //    resFE_SIPCORESVC_STALE_NONCE:
    //      The packet contains credentials but the nonce is stale.
    //
    //    resFE_INVALID_ARGUMENT:
    //      One of the packet's parameters is wrong or missing, or the packet
    //      contains some error.
    //
    //  Description:
    //      Checks if the SIP packet contains credentials, and if so, also
    //      checks whether or not the nonce is stale and has been generated
    //      locally. Also checks the presence of the username, cnonce, and
    //      nonce-count (nc) parameters when required; the presence and validity
    //      of the realm, algorithm, and qop (quality of protection) parameters;
    //      and the presence of the nonce plus its not being stale.
    //
    //  See Also:
    //      VerifyAuthentication
    //
    //==EDOC====================================================================
    virtual mxt_result
    ContainsCredentials(IN  const CSipPacket& rRequest,
                        OUT CString&          rstrUsername) = 0;

    //==SDOC====================================================================
    //==
    //==  VerifyAuthentication
    //==
    //==========================================================================
    //
    //  Summary:
    //      Verifies the credentials found in the SIP packet.
    //
    //  Parameters:
    //    rRequest:
    //      The request that contains the credentials for this realm.
    //
    //    rstrHashA1:
    //      Hash of A1, as specified in RFC 2617. See below for more
    //      information.
    //
    //  Returns:
    //    resFE_ABORT:
    //       A problem has been encountered when computing the checksum.
    //
    //    resFE_INVALID_ARGUMENT:
    //      One of the packet's other parameters is wrong or missing or the
    //      packet contains some error.
    //
    //    resFE_FAIL:
    //      The response is invalid.
    //
    //    resS_OK:
    //      Authentication has succeeded.
    //
    //  Description:
    //      Verifies the credentials found in the packet.
    //
    //      The string rstrHashA1 must be built as follows (refer to RFC 2617
    //      for more details).
    //
    //      <CODE>
    //          MD5HASH(username ":" realm ":" password)
    //      </CODE>
    //
    //      An hash of A1 can easily be obtained if the password is available in
    //      clear text through the GetMd5AlgoHash.
    //
    //      Currently, only the verification of the algorithm MD5 is supported.
    //
    //      Applications should consider storing hashes of user password instead
    //      of directly storing the passwords in clear text. Storing a hash of
    //      A1 instead of storing the password is usually the proper thing to do
    //      when not using some form of authentication back-end.
    //
    //  See Also:
    //      GetMd5AlgoHash, RemoveVerifiedHeader
    //
    //==EDOC====================================================================
    virtual mxt_result VerifyAuthentication(IN const CSipPacket& rRequest,
                                            IN const CString&    rstrHashA1) = 0;

    //==SDOC====================================================================
    //==
    //==  GetMd5AlgoHash
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the MD5 hash of A1 when the MD5 algorithm is used.
    //
    //  Parameters:
    //    rstrUsername:
    //      The username.
    //
    //    rstrPassword:
    //      The password.
    //
    //    rstrHash:
    //      String that contains the hash upon return.
    //
    //  Returns:
    //      See MxCalculateMd5Checksum in SipParser/Authentication.cpp
    //
    //  Description:
    //      Generates a hash of A1 according to RFC 2617. The realm used is the
    //      realm configured through SetSupportedRealm.
    //
    //  See Also:
    //      SipParser/Authentication.cpp: MxCalculateMd5Checksum
    //
    //==EDOC====================================================================
    virtual mxt_result GetMd5AlgoHash(IN  const CString& rstrUsername,
                                      IN  const CString& rstrPassword,
                                      OUT CString&       rstrHashA1) const = 0;

//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipStatelessDigestServerAuthSvc(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipStatelessDigestServerAuthSvc(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipStatelessDigestServerAuthSvc(
                               IN const ISipStatelessDigestServerAuthSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipStatelessDigestServerAuthSvc& operator=(
                               IN const ISipStatelessDigestServerAuthSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSTATELESSDIGESTSERVERAUTHSVC_H

