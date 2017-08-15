//==============================================================================
//==============================================================================
//
//          Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCEUSERAUTHENTICATION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEUSERAUTHENTICATION_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
//-------------------------
class ISceUserAuthenticationMgr;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceUserAuthentication);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceUserAuthentication
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface describes the authentication operations that are possible for
//   a user.
//
// Description:
//  This interface describes the authentication operations that are possible for
//  a user.
//
//  This interface is accessible through the ISceUserConfig object.
//
//  It is through this interface that the application sets the credentials
//  associated with a realm for a specific user.
//
//  In order to be notified of incoming challenges, the application must
//  configure a manager through the SetManager API.
//
//  However, a manager is not needed for the authentication process to be
//  successful. If no manager is set, the authentication credentials MUST be
//  added BEFORE receiving the challenge.
//
// Location:
//   SceCore/ISceUserAuthentication.h
//
// See Also:
//   ISceUserConfig
//
//==============================================================================
class ISceUserAuthentication : public IEComUnknown
{
//-- Friend Declarations.
//------------------------

//-- Published Interface.
//------------------------
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceUserAuthentication);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager through which authentication events are
    //      reported.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the manager. It cannot be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: pMgr is NULL. The manager is not changed
    //          in this case.
    //      - resFE_INVALID_STATE: The manager is already configured.
    //      - resS_OK: Success.
    //
    //  Description:
    //      Sets the interface to which events are reported by this
    //      ISceUserAuthentication instance.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN ISceUserAuthenticationMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  AddCredentials
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the credentials associated with a specific realm.
    //
    //  Parameters:
    //    rstrRealm:
    //      The realm string.
    //
    //    rstrUsername:
    //      The username used within that realm.
    //
    //    rstrPassword:
    //      The password used with the provided username.
    //
    //    rstrProtocol:
    //      The protocol for which these credentials are valid. For these
    //      credentials to be used with all protocols the special protocol "ALL"
    //      must be set.
    //
    //  Description:
    //      Configures the authentication mechanism with credentials to
    //      use for a specific realm. If a credential already exists for this
    //      realm, it is replaced with the provided one.
    //
    //      If there were some unanswered challenges for the specific realm and
    //      protocol pair for which the credentials are provided, then the
    //      challenges are automatically answered with the updated credentials.
    //
    //      The configured credentials are reused when receiving a challenge
    //      for the specified realm.
    //
    //      Note that setting rstrProtocol to "ALL" allows the SIP Client Engine
    //      to reuse the password for any protocols that it integrates. The
    //      password will be used based on the realm provided in the challenge.
    //
    //==========================================================================
    virtual void AddCredentials(IN const CString& rstrRealm,
                                IN const CString& rstrUsername,
                                IN const CString& rstrPassword,
                                IN const CString& rstrProtocol) = 0;

    //==========================================================================
    //==
    //==  CancelAuthentication
    //==
    //==========================================================================
    //
    //  Summary:
    //      Abandons trying to provide credentials for a received challenge.
    //
    //  Parameters:
    //    rstrRealm:
    //      The realm for which the authentication is to be cancelled.
    //
    //    rstrProtocol:
    //      The protocol for which the authentication is to be cancelled.
    //
    //  Description:
    //      This method must be called when an application has no credentials to
    //      provide for a specific realm. The normal failure management takes
    //      over for the requests that have been sent.
    //
    //      All challenges for this specific realm are cancelled.
    //
    //==========================================================================
    virtual void CancelAuthentication(IN const CString& rstrRealm,
                                      IN const CString& rstrProtocol) = 0;

// BRCM_CUSTOM - [add] Add authentication info sharing support 
    //==========================================================================
    //==
    //==  CloneChallengeInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Duplicate the given challenge identified by "opqChallengeId".
    //
    //  Parameters:
    //    opqChallengeId:
    //      The identifier of the challenge to be duplicated.
    //
    //  Returns:
    //      The identifier of the duplicated challenge. The application must
    //      call DeleteChallengeInfo() when the duplcated challenge is not
    //      needed anymore.
    //
    //  Description:
    //      This method duplicates a given challenge that can be used later
    //      (e.g. by ISceBasicExtensionControl::AddChallengeInfo()).
    //
    //==========================================================================
    virtual mxt_opaque CloneChallengeInfo(IN mxt_opaque opqChallengeId) = 0;

    //==========================================================================
    //==
    //==  DeleteChallengeInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destroy the previously allocated challenge info created by
    //      CloneChallengeInfo().
    //
    //  Parameters:
    //    opqChallengeId:
    //      The identifier of the challenge to be deleted. Note this identifier
    //      will become invalid after this method is called.
    //
    //  Description:
    //      This method destroys the given challenge when it is not needed.
    //
    //==========================================================================
    virtual void DeleteChallengeInfo(IN mxt_opaque opqChallengeId) = 0;

    //==========================================================================
    //==
    //==  IsChallengeInfoEqual
    //==
    //==========================================================================
    //
    //  Summary:
    //      Compare if the two provided challenge info contain the same
    //      information.
    //
    //  Parameters:
    //    opqChallengeIdFrom:
    //      The identifier of the challenge to be compared from.
    //
    //    opqChallengeIdTo:
    //      The identifier of the challenge to be compared to.
    //
    //  Returns:
    //      True if the two challenge info contain the same information.
    //
    //==========================================================================
    virtual bool IsChallengeInfoEqual(IN mxt_opaque opqChallengeIdFrom,
                                      IN mxt_opaque opqChallengeIdTo) = 0;

    //==========================================================================
    //==
    //==  GetChallengeNonceInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieve the nonce value from the challenge
    //
    //  Parameters:
    //    opqChallengeIdFrom:
    //      The identifier of the challenge to find the nonce info of.
    //    
    //    rstrNonce:
    //      The nonce in the challenge
    //
    //==========================================================================
    virtual void GetChallengeNonceInfo(IN mxt_opaque opqChallengeIdFrom,
                                      OUT CString& rstrNonce) = 0;


// BRCM_CUSTOM - [end] Add authentication info sharing support

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   The behavior of the SIP Client Engine can take upon reception
    //   of a stale NONCE authentication challenge before re-issuing the request.
    //
    // Description:
    //  This type defines the potential behavior of the SIP Client Engine can
    //  take upon reception of a stale NONCE authentication challenge re-issuing
    //  the request.
    //
    //==========================================================================
    enum EStaleNonceBehavior
    {
        // Description:
        // The SIP Client Engine does not take any action and simply re-issue
        // the request with the newly re-calculated credentials.
        // This is the default value.
        eSTALENONCE_NO_ACTION,

        // Description:
        // The SIP Client Engine will reset all credentials the user might have
        // configured in order to re-issue an empty request. Doing so will force
        // the server/proxy to challenge it again from scratch. This is useful
        // when the server/proxy have problem accepting new credentials after
        // reporting a stale NONCE.
        eSTALENONCE_RESET_CREDENTIALS,
    };

    //==========================================================================
    //==
    //==  SetStaleNonceBehavior
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the stale NONCE behavior.
    //
    //  Parameters:
    //    eBehavior:
    //      The stale NONCE behavior to set.
    //
    //  Description:
    //      Sets the stale NONCE behavior.
    //
    //      The default value is eSTALENONCE_NO_ACTION.
    //
    //  See Also:
    //      EStaleNonceBehavior, GetStaleNonceBehavior
    //
    //==========================================================================
    virtual void SetStaleNonceBehavior(IN EStaleNonceBehavior eBehavior) = 0;

    //==========================================================================
    //==
    //==  GetStaleNonceBehavior
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the stale NONCE behavior.
    //
    //  Returns:
    //      The configured stale NONCE behavior.
    //
    //  Description:
    //      Gets the stale NONCE behavior.
    //
    //      The default value is eSTALENONCE_NO_ACTION.
    //
    //  See Also:
    //      EStaleNonceBehavior, SetStaleNonceBehavior
    //
    //==========================================================================
    virtual EStaleNonceBehavior GetStaleNonceBehavior() const = 0;

    //==========================================================================
    //==
    //==  SetAuthenticationLoopThreshold
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the maximum number of consecutive challenges the user
    //      may handle before canceling the authentication process.
    //
    //  Parameters:
    //    uMax:
    //      The maximum number of consecutive challenges the user may handle.
    //
    //  Returns:
    //      - resFE_FAIL: uMax is too low. Must be higher than 2.
    //      - resS_OK: Maximum correctly configured.
    //
    //  Description:
    //      This configures the maximum number of consecutive 401/407
    //      challenges the user may handle before canceling the authentication
    //      process for this user. 401/407 challenges are considered
    //      consecutive when they are not separated by a success response (2xx)
    //      for a given component.
    //
    //      If ISceBasicExtensionControl::EnableAuthenticationCallback is used,
    //      this configuration value will have no effect whatsoever and it will
    //      be the application responsibility to detect authentication loop and
    //      cancel the authentication process if it happens.
    //
    //      The default value is 10 when this configuration method is not
    //      called.
    //
    //  See Also:
    //      GetAuthenticationLoopThreshold,
    //      ISceBasicExtensionControl::EnableAuthenticationCallback
    //
    //==========================================================================
    virtual mxt_result SetAuthenticationLoopThreshold(unsigned int uMax) = 0;

    //==========================================================================
    //==
    //==  GetAuthenticationLoopThreshold
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured maximum number of consecutive challenges the
    //      user may handle before canceling the authentication process.
    //
    //  Returns:
    //      The maximum number of consecutive challenges the user may handle.
    //
    //  Description:
    //      Gets the configured maximum number of consecutive 401/407
    //      challenges the user may handle before canceling the authentication
    //      process for this user.
    //
    //  See Also:
    //      SetAuthenticationLoopThreshold
    //
    //==========================================================================
    virtual unsigned int GetAuthenticationLoopThreshold() const = 0;

//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceUserAuthentication() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceUserAuthentication() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceUserAuthentication(IN const ISceUserAuthentication& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceUserAuthentication& operator=(IN const ISceUserAuthentication& rSrc);


//-- Hidden Data Members.
//------------------------
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEUSERAUTHENTICATION_H
