//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==EDOC========================================================================
#ifndef MXG_CSIPSTATELESSDIGESTSERVERAUTHSVC_H
#define MXG_CSIPSTATELESSDIGESTSERVERAUTHSVC_H

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
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_HEADERDEFINITION_H
#include "SipParser/HeaderDefinition.h" // enum ESipHeaderType
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPSTATELESSDIGESTSERVERAUTHSVC_H
#include "SipCoreSvc/ISipStatelessDigestServerAuthSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CGenParamList;
class CSipHeader;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipStatelessDigestServerAuthSvc
//========================================
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
//#The CSipStatelessDigestServerAuthSvc is an ECOM object#
//
//  The CSipStatelessDigestServerAuthSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipStatelessDigestServerAuthSvc</B>
//
//  <B>Interface Id: IID_CSipStatelessDigestServerAuthSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/CSipStatelessDigestServerAuthSvc.h
//
//==EDOC========================================================================
class CSipStatelessDigestServerAuthSvc : private CEComDelegatingUnknown,
                                         private ISipCoreSvc,
                                         private CSipReqCtxCoreSvc,
                                         private ISipStatelessDigestServerAuthSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipStatelessDigestServerAuthSvc();

    static void FinalizeCSipStatelessDigestServerAuthSvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor. Used through the factory method local to this
    //  class.
    //---------------------------------------------------------------------
    CSipStatelessDigestServerAuthSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipStatelessDigestServerAuthSvc();


    //-- << ISipStatelessDigestServerAuthSvc Interface >>.
    //-----------------------------------------------------

    // Summary:
    //   Configures the realm supported by this device.
    //--------------------------------------------------
    virtual void SetSupportedRealm(IN const CString& rstrRealm);

    // Summary:
    //   Configures the time before authentication information becomes stale.
    //------------------------------------------------------------------------
    virtual void SetAuthLifetime(IN unsigned int uExpirationMin);

    // Summary:
    //   Configures the quality of protection sent in challenges.
    //------------------------------------------------------------
    virtual mxt_result SetRequestedQualityOfProtection(
                                               IN unsigned int uQopOptions);

    // Summary:
    //   Configures the private key used in nonce encoding.
    //------------------------------------------------------
    virtual void SetPrivateKey(IN const CString& rstrPrivateKey);

    // Summary:
    //   Configures whether or not the presence of the qop parameter in the
    //   authorization header must be checked and its value validated, for
    //   backward compatibility purposes.
    //----------------------------------------------------------------------
    virtual void QopMustBePresent(IN bool bPresent);

// BRCM_CUSTOM - [add] IMRG: generate AUTH header for remote user.    
    // Summary:
    //   create SIP challenge header.
    //-------------------------------------
    virtual void CreateChallengeHeader(IN const CSipPacket&  rRequest,
                                       IN ESipHeaderType  eHeaderType,
                                       IN EAlgorithm      eAlgorithm,
                                       IN bool            bIsStale,
                                       OUT CSipHeader**   ppAuthHeader);
// BRCM_CUSTOM - [end] IMRG: generate AUTH header for remote user.

    // Summary:
    //   Challenges the specified request.
    //-------------------------------------
    virtual mxt_result ChallengeRequest(IN const CSipPacket&  rRequest,
                                        IN unsigned int uChallengeType,
                                        IN bool               bIsStale);

    // Summary:
    //   Challenges the specified request.
    //-------------------------------------
    virtual mxt_result ChallengeRequest(IN const CSipPacket&  rRequest,
                                        IN unsigned int uChallengeType,
                                        IN EAlgorithm         eAlgorithm,
                                        IN const CString&     rstrNonce,
                                        IN bool               bIsStale);

    //  Summary:
    //      Rejects the specified request.
    virtual mxt_result RejectRequest(IN const CSipPacket& rRequest,
                                     IN unsigned int uResponseType,
                                     IN const char* pszReasonPhrase);

    // Summary:
    //   Verifies if the SIP packet contains credentials, and the validity of
    //   some of them.
    //------------------------------------------------------------------------
    virtual mxt_result ContainsCredentials(IN const CSipPacket& rRequest,
                                           OUT CString& rstrUsername);

    // Summary:
    //   Verifies the credentials found in the SIP packet.
    //-----------------------------------------------------
    virtual mxt_result VerifyAuthentication(IN const CSipPacket& rRequest,
                                            IN const CString&    rstrHashA1);

    //  Summary:
    //    Retrieves the MD5 hash of A1 when the MD5 algorithm is used.
    //-----------------------------------------------------------------
    virtual mxt_result GetMd5AlgoHash(IN  const CString& rstrUsername,
                                      IN  const CString& rstrPassword,
                                      OUT       CString& rstrHashA1) const;

    //-- << ISipCoreSvc methods >>
    //-----------------------------

    //  Summary:
    //    Obtains an instance of an ISipReqCtxCoreSvc to attach to a request
    //    context.
    //-----------------------------------------------------------------------
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                               IN const CSipPacket& rPacket,
                                               IN EDirection        eDirection);

    //  Summary:
    //    Establishes the confidence level for which the service is a good
    //    candidate to own a request context.
    //---------------------------------------------------------------------
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                               IN const CSipPacket& rPacket);

    //  Summary:
    //    Returns the ISipReqCtxCoreSvc instance to use as the owner for a
    //    request context.
    //---------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();

    //-- << ISipReqCtxCoreSvc methods >>
    //-----------------------------------

    //  Summary:
    //    Notifies the service that the application is done updating the packet.
    //--------------------------------------------------------------------------
    virtual mxt_result OnPacketUpdated(
                                   IN    ISipRequestContext& rRequestContext,
                                   INOUT CSipPacket&         rPacket);

    //  Summary:
    //    Adds a reference on this request context core service.
    //-----------------------------------------------------------
    virtual unsigned int AddRcsRef();

    //  Summary:
    //    Removes a reference on this request context core service.
    //--------------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();

    //-- << ECOM mechanism >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipStatelessDigestServerAuthSvc(
                             IN const CSipStatelessDigestServerAuthSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipStatelessDigestServerAuthSvc& operator=(
                             IN const CSipStatelessDigestServerAuthSvc& rSrc);

    //-- << Helpers >>
    //-----------------

    // Summary:
    //  Generates the nonce added to the WWW-Authenticate or Proxy-Authenticate
    //  header.
    //--------------------------------------------------------------------------
    void GenerateNonce(IN  const CSipPacket& rRequest,
                       OUT CString&          rstrNonce);

    // Summary:
    //  Generates the parameters added to the WWW-Authenticate or
    //  Proxy-Authenticate header.
    //------------------------------------------------------------
    GO CGenParamList* GenerateParamListForAuthHeader(
                                                   IN EAlgorithm     eAlgorithm,
                                                   IN const CString& rstrNonce,
                                                   IN bool           bIsStale);

    // Summary:
    //  Checks whether the packet is a request and not a CANCEL (or an ACK if
    //  bAckAccepted is false).
    //------------------------------------------------------------------------
    static bool IsValidRequest(IN const CSipPacket& rRequest,
                               IN bool              bAckAccepted = false);

    // Summary:
    //  Creates and returns a timestamp for the current time.
    //--------------------------------------------------------
    static uint64_t GetCurrentTimestamp();

    // Summary:
    //  Returns the (Proxy-)Authorization header corresponding to the realm, if
    //  applicable.
    //--------------------------------------------------------------------------
    CSipHeader* GetAuthHeader(IN  const CHeaderList& rHeaderList,
                              IN  ESipHeaderType     eHeaderType,
                              OUT unsigned int& ruIndex);

    // Summary:
    //  Creates the nonce from data passed in parameters.
    //----------------------------------------------------
    void CreateNonce(IN  const char*    szTimestamp,
                     IN  const CString& rstrRandom,
                     IN  const CString& rstrCallId,
                     OUT       CString& rstrNonce);

    // Summary:
    //  Creates a challenge response from a request containing no or invalid
    //  credentials.
    //-----------------------------------------------------------------------
    GO CSipPacket* CreateResponse(IN ESipHeaderType    eHeader,
                                  IN EAlgorithm        eAlgorithm,
                                  IN const CString&    rstrNonce,
                                  IN bool              bIsStale,
                                  IN const CSipPacket& rRequest,
                                  IN unsigned int      uCode,
                                  IN const char*       szReason);

    mxt_result SendResponse(IN const CSipPacket& rRequest,
                            IN CSipPacket& rResponse);

//-- Hidden Data Members
//------------------------
protected:
private:

    // The combined requested quality of protection.
    //-----------------------------------------------
    unsigned int m_uQopRequested;

    // Whether the quality of protection must be present in the request or not.
    //--------------------------------------------------------------------------
    bool m_bQopMustBePresent;

    // The nonce validity delay in minutes.
    //--------------------------------------
    unsigned int m_uExpirationMin;

    // The realm supported by the instance of the class.
    //---------------------------------------------------
    CString m_strSupportedRealm;

    // The private key used for nonce generation.
    //--------------------------------------------
    CString m_strPrivateKey;
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSTATELESSDIGESTSERVERAUTHSVC_H

