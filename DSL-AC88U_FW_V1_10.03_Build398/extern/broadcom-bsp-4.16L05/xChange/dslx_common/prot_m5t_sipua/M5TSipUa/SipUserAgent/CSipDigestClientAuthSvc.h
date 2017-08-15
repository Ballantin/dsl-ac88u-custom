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
#ifndef MXG_CSIPDIGESTCLIENTAUTHSVC_H
#define MXG_CSIPDIGESTCLIENTAUTHSVC_H

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
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif


//-- Parameters used by Value
//-----------------------------


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

#ifndef MXG_ISIPDIGESTCLIENTAUTHSVC_H
#include "SipUserAgent/ISipDigestClientAuthSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CGenParamList;
class CSipHeader;
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipDigestClientAuthSvc
//=============================================
//
//  Description:
//      The CSipDigestClientAuthSvc is the class that implements the interface
//      ISipDigestClientAuthSvc. The application has to create an instance of
//      CSipDigestClientAuthSvc when it wants to add an ISipDigestClientAuthSvc
//      interface to a context.
//
//      This service helps the application authenticate itself to a remote party
//      that challenges it. This service monitors transactions for 401
//      (Unauthorized) and 407 (Proxy Authentication Required) responses and
//      then generates events to the application when required.  It supports
//      Digest authentication.
//
//      The service is mostly a helper, it does not send responses by itself,
//      and can never be the owner of a request context.  It does not interact
//      with other services either.  It is a stateful service in the sense that
//      previous challenges are kept until Reset is called upon the service.
//      Furthermore, it remembers if it has processed a 200 OK response in order
//      to send a different event if a challenge is received after that, but it
//      is not interested in any data other than those concerning authentication
//      headers.
//
//      OnPacketReceived is the method used when a response is received.  If the
//      response is a 401 or 407 and contains correct authentication headers, an
//      event is sent to the application through the manager.  The application
//      then has to get the challenges and set the relevant data prior to
//      sending another request (except the ACK, which must pass through the
//      service before setting the new challenge data).
//
//      UpdatePacket is the method that is used to add the requested
//      authentication to outgoing request, after the challenge data has been
//      filled by the application as mentioned above.
//
//      See RFC 2617 for further details on the rules to follow when using
//      Digest authentication.
//
//      This service generates events to the application through the
//      ISipDigestClientAuthMgr interface.
//
//      To add a CSipDigestClientAuthSvc to a SIP context, the application has
//      to create a new instance through the <C>ISipContext::AttachService</C>
//      method.
//
//      To access a CSipDigestClientAuthSvc interface from an ISipContext, the
//      application must call the <C>ISipContext::QueryIf()</C> method with
//      IID_ISipDigestClientAuthSvc as the "iid" parameter.
//
// Location:
//   SipUserAgent/CSipDigestClientAuthSvc.h
//
// See Also:
//   ISipContext::AttachService, ISipDigestClientAuthSvc,
//   ISipDigestClientAuthMgr, ISipCoreSvc, ISipReqCtxCoreSvc
//
//==============================================================================
class CSipDigestClientAuthSvc : private CEComDelegatingUnknown,
                                private ISipCoreSvc,
                                private CSipReqCtxCoreSvc,
                                private ISipDigestClientAuthSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

public:
    //-- << Global extensions API >>
    //-- Only accessed by CSipCoreConfig.
    static void SetClientAuthUnknownQopBehavior(IN ISipDigestClientAuthSvc::EUnknownQopBehavior eBehavior);

// Hidden Methods
//-------------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipDigestClientAuthSvc();

    static void FinalizeCSipDigestClientAuthSvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor. Used through CreateInstance.
    //----------------------------------------------------
    CSipDigestClientAuthSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipDigestClientAuthSvc();

    //-- << ISipDigestClientAuthSvc interface >>
    //---------------------------------------------------

    // Summary:
    //   Configures the event manager associated with this service manager.
    //--------------------------------------------------------------------
    virtual mxt_result SetManager(IN ISipDigestClientAuthMgr* pMgr);

    // Summary:
    //   Configures the maximum number of consecutive challenges the service
    //   may handle before reporting EvAuthenticationLoop.
    //-----------------------------------------------------------------------
    virtual mxt_result SetLoopThreshold(IN unsigned int uMax);


    // Summary:
    //   Sets the username and the hash of A1 for a specific realm.
    //---------------------------------------------------------------
    virtual mxt_result Authenticate(IN const CString& rstrRealm,
                                    IN const CString& rstrUsername,
                                    IN const CString& rstrHashA1);

    // Summary:
    //   Gets the challenges that currently require authentication, if any.
    //----------------------------------------------------------------------
    virtual const CVector<SChallengeData*>& GetChallenges() const;

    // Summary:
    //   Clears the authentication state.
    //------------------------------------
    virtual void Reset(IN ISipDigestClientAuthSvc::EResetBehaviour eResetBehaviour = ISipDigestClientAuthSvc::eRESET_ALL);

    //  Summary:
    //   Sets the service's behaviour for using the route URI instead of the
    //   request URI.
    //-----------------------------------------------------------------------
    virtual void SetRouteUriAuthentication(IN bool bUseRouteUri);

    // Summary:
    //   Updates the Authorization header(s).
    //----------------------------------------
    virtual mxt_result UpdateDigest(INOUT CSipPacket& rPacket);

    //  Summary:
    //   Enables or disables the Auth-Int authentification method.
    virtual void EnableQualityOfProtectionAuthInt(IN bool bEnable);

    virtual void EnableMutualAuthentication(IN bool bEnable);

// BRCM_CUSTOM - [end] Add authentication info sharing support
    virtual void UpdateChallenge(IN const SChallengeData& rChallenge);
    
    virtual bool SetChallengeCount(IN const SChallengeData& rChallenge,
                                   IN unsigned int uNonceCount);

    virtual bool SetChallengeType(IN const SChallengeData& rChallenge,
                                  IN unsigned int uChallengeType);

    virtual void NotifyMgrNonceEmployed(IN const CSipPacket& rPacket);

// BRCM_CUSTOM - [end] Add authentication info sharing support

    //-- << ISipCoreSvc methods >>
    //------------------------------

    // Summary:
    //   Obtain an instance of an ISipReqCtxCoreSvc to attach to a request
    //   context.
    //---------------------------------------------------------------------
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                               IN const CSipPacket& rPacket,
                                               IN EDirection        eDirection);

    // Summary:
    //   Gives to which degree the service is a good candidate to own a request
    //   context.
    //--------------------------------------------------------------------------
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                               IN const CSipPacket& rPacket);

    // Summary:
    //   Return the ISipReqCtxCoreSvc instance to use as the owner for a request
    //   context.
    //--------------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();

    //-- << ISipReqCtxCoreSvc methods >>
    //------------------------------------

    // Summary:
    //   Informs the request context core service of a received packet.
    //------------------------------------------------------------------
    virtual void OnPacketReceived(IN    ISipRequestContext& rRequestContext,
                                  IN    const CSipPacket&   rPacket,
                                  INOUT CSipCoreEventList&  rEventList);

    // Summary:
    //   Modify a packet about to be sent.
    //-------------------------------------
    virtual mxt_result UpdatePacket(IN    ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    // Summary:
    //   Let the service call an event on the application.
    //-----------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque          opqEvent,
                           IN const CSipPacket&   rPacket);

    // Summary:
    //   Let the service clear state associated with an event in which the
    //   application has no interest.
    //--------------------------------------------------------------------
    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque          opqEvent);

    // Summary:
    //   Add a reference on this request context core service.
    //---------------------------------------------------------
    virtual unsigned int AddRcsRef();

    // Summary:
    //   Remove a reference on this request context core service.
    //------------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();

    //-- << ECOM mechanism >>
    //-------------------------
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << Helper >>
    //--------------------------------------------

    // Summary:
    //  Parses the qop parameter and verifies which qop is supported.
    //----------------------------------------------------------------
    void ParseQopParameter(IN const CString& strQopNoQuotes,
                           OUT bool& rbAuthSupported,
                           OUT bool& rbAuthIntSupported);

    // Summary:
    //  Computes the responses to challenges and adds the Authorization headers.
    //---------------------------------------------------------------------------
    mxt_result AddAuthenticationResponse(INOUT CSipPacket& rPacket);


    // Enum kept to manage incoming events from the network. Those events are
    // the reception of responses.
    //------------------------------------------------------------------------
    enum EClientAuthEvent
    {
        eAUTHENTICATION_LOOP,
        eCREDENTIALS_REQUIRED,
        eINVALID_CREDENTIALS,
        eCREDENTIALS_EXIST,
        eINVALID_RESPONSE_CREDENTIALS
// BRCM_CUSTOM - [add] Add server digest authentication support
        ,eSVR_AUTHENTICATION_FAILED
// BRCM_CUSTOM - [end] Add server digest authentication support
// BRCM_CUSTOM - [add] Add authentication info sharing support
        ,eAUTHENTICATION_SUCCESS
// BRCM_CUSTOM - [end] Add authentication info sharing support
    };

    struct SEventData
    {
        SEventData(IN EClientAuthEvent eEvent)
        :   m_eEvent(eEvent),
            m_opqData(MX_VOIDPTR_TO_OPQ(NULL))
        {}

        EClientAuthEvent m_eEvent;
        mxt_opaque m_opqData;
    };

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------

    CSipDigestClientAuthSvc(const CSipDigestClientAuthSvc& rSource);

    CSipDigestClientAuthSvc& operator=(const CSipDigestClientAuthSvc& rSource);

    // Summary:
    //  Gets and checks for the presence of a Realm.
    //-----------------------------------------------
    mxt_result GetRealm(IN  const CGenParamList& rParamList,
                        OUT CString&             rstrRealm);

    // Summary:
    //  Gets and checks for the presence of a Nonce.
    //-----------------------------------------------
    mxt_result GetNonce(IN  const CGenParamList& rParamList,
                        OUT CString&             rstrNonce);

    // Summary:
    //  Gets and checks for the presence of a Algorithm.
    //---------------------------------------------------
    mxt_result GetAlgorithm(IN  const CGenParamList& rParamList,
                            OUT CString&             rstrAlgorithm);

    // Summary:
    //  Gets and checks for the presence of a Qop.
    //---------------------------------------------
    mxt_result GetQop(IN  const CGenParamList& rParamList,
                      OUT CString&             rstrQop,
                      OUT bool&                rbSupportAuth,
                      OUT bool&                rbSupportAuthInt);

    // Summary:
    //  Gets and checks for the presence of a Opaque.
    //------------------------------------------------
    mxt_result GetOpaque(IN  const CGenParamList& rParamList,
                         OUT CString&             rstrOpaque);

    // Summary:
    //  Generate parameters added to the Authorization or Proxy-Authorization
    //  header.
    //------------------------------------------------------------------------
    static GO CGenParamList* GenerateParamListForAuthHeader(IN SChallengeData& rData,
                                                            IN const CString& rstrDigestUri,
                                                            IN const CString& rstrResponse);

    // Summary:
    //  Sets a new nonce in rData.
    void SetNonce(IN const CString& rstrNewNonce,
                  INOUT SChallengeData& rstData);

    // Summary:
    //  Validate server response credentials after receive an ok authentication
    //  response
    mxt_result ValidateResponseCredentials(IN const CSipHeader* pSipHeader,
                                           IN const CBlob* pblobPayload,
                                           INOUT CVector<bool>& rvecbFoundHeader,
                                           INOUT CVector<mxt_result>& rvecresFoundHeaders);

#if defined (MXD_S_IMS_TEST)
    GO CGenParamList* GenerateParamListForAuthHeader();
#endif // #if defined (MXD_S_IMS_TEST)

// Hidden Data Members
//-------------------------
protected:
private:

    // The list of all challenges - authentified by the application or not.
    // Ownership is contained herein.
    //----------------------------------------------------------------------
    CVector<SChallengeData*> m_vecpstAllChallenges;

    // The list of challenges received in the last response and still not
    // authentified. Items are copies of pointers whose ownership is kept by
    // m_vecpstAllChallenges.
    //-----------------------------------------------------------------------
    CVector<SChallengeData*> m_vecpstLastChallenges;

    // The event manager.
    //--------------------
    ISipDigestClientAuthMgr* m_pMgr;

    unsigned int m_uLoopThreshold;
    unsigned int m_uCurrentLoop;

    CString m_strInvalidCredentialRealm;

    // This boolean flag is used to determine if the TOPMOST-ROUTE URI should be
    // used for authentication purposes instead of the REQUEST-URI.
    //---------------------------------------------------------------------------
    bool m_bUseRouteUri;

    // This boolean flag is used to determine if the Auth-Int should be used or
    // ignored for authentification purposes. The default value is set to true.
    bool m_bQopAuthIntEnabled;

    // The behavior of the ISipDigestClientAuthSvc upon reception of an
    // unknown qop parameter value in the WWW-Authenticate or Proxy-Authenticate
    // header. For more information see ISipDigestClientAuthSvc::EUnknownQopBehavior.
    static ISipDigestClientAuthSvc::EUnknownQopBehavior m_seUnknownQopBehavior;

    // This boolean flag is used to determine if the response authentication
    // should be used. The default value is set to false.
    bool m_bResponseAuthenticationEnabled;
};


MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSIPDIGESTCLIENTAUTHSVC_H

