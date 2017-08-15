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
#ifndef MXG_IPRIVATESCEAUTHENTICATION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IPRIVATESCEAUTHENTICATION_H

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
class CString;
class IPrivateSceBaseComponent;
class IPrivateSceChallengeSource;
class ISipClientEventControl;
class ISipDigestClientAuthSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IPrivateSceAuthentication);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IPrivateSceAuthentication
//========================================
//
// Description:
//   This is a private interface for the internal UA mechanism. It MUST NOT be
//   used externally. Please use ISceUserAuthentication instead.
//
//   This interface permits to a component to ask to the CSceAutentication
//   object to manage the received digest challenges.
//
//#The IPrivateSceAuthentication is an ECOM object#
//
//  The IPrivateSceAuthentication is an ECOM object that is accessed
//  through the following ECOM identifier:
//
//  <B>Interface Id: IID_IPrivateSceAuthentication</B>
//
// Location:
//   SceCoreComponents/IPrivateSceAuthentication.h
//
//==============================================================================
class IPrivateSceAuthentication : public IEComUnknown
{
//-- Friend Declarations.
//------------------------

//-- New types within class namespace.
//-------------------------------------
public:
protected:
private:

//-- Published Interface.
//------------------------
public:
    MX_DECLARE_ECOM_GETIID(IPrivateSceAuthentication);

    //==========================================================================
    //==
    //==  EvCredentialsRequired
    //==
    //==========================================================================
    //
    //  Summary:
    //      Handles an incoming challenge when new credentials are required.
    //
    //  Parameters:
    //    pComponent:
    //      The IPrivateSceChallengeSource of the component that received the
    //      challenge.
    //
    //    rstrProtocol:
    //       The protocol for which the credentials are associated.
    //
    //    rstrRealm:
    //       The realm for which the credentials are associated.
    //
    //    opqSourceData:
    //       Opaque data used by pComponent to identify this specific
    //       transaction. Must be passed back unmodified to pComponent when
    //       calling SetCredentials or CancelAuthentication.
    //
    //  Description:
    //      Informs the user authentication that credentials are needed for this
    //      component.
    //
    //      Depending on whether or not credential information is found for this
    //      realm and protocol, one of SetCredentials or CancelAuthentication
    //      MUST be called on pComponent.
    //
    //
    //  Implementation's Notes:
    //      This method looks for the requested credentials for the realm
    //      associated to the challenge in the internal challenge vector and
    //      then authenticates the credentials using the digest client
    //      authentication service. When no credentials are found, the
    //      EvChallenged event is fired through the IUaSspAuthenticationMgr
    //      manager.
    //
    //
    //  See Also:
    //      IPrivateSceBaseComponent
    //
    //==========================================================================
    virtual void EvCredentialsRequired(IN IPrivateSceChallengeSource* pComponent,
                                       IN const CString& rstrProtocol,
                                       IN const CString& rstrRealm,
                                       IN mxt_opaque opqSourceData) = 0;


    //==========================================================================
    //==
    //==  EvInvalidCredentials
    //==
    //==========================================================================
    //
    //  Summary:
    //      Handle credentials rejection.
    //
    //  Parameters:
    //    pComponent:
    //      The IPrivateSceChallengeSource of the component on which credential
    //      rejection occured.
    //
    //    rstrProtocol:
    //       The protocol for which the credentials are associated.
    //
    //    rstrRealm:
    //       The realm for which the credentials are associated.
    //
    //    opqSourceData:
    //       Opaque data used by pComponent to identify this specific
    //       transaction. Must be passed back unmodified to pComponent when
    //       calling SetCredentials or CancelAuthentication.
    //
    //  Description:
    //      Reports that existing credential information failed to grant access
    //      to a resource.
    //
    //      The implementation should remove any cached information for the
    //      specified realm/protocol and reget the information from the user to
    //      try again.
    //
    //      Depending on whether or not credential information is found for this
    //      realm and protocol, one of SetCredentials or CancelAuthentication
    //      MUST be called on pComponent.
    //
    //  Implementation's Notes:
    //
    //      This method looks for the requested credentials for the specified
    //      realm in the internal challenge vector and then authenticates the
    //      credentials using the digest client authentication service. When
    //      authentication fails, credentials are removed from the internal
    //      vector and EvCredentialsRequired is called. When no credentials are
    //      found EvCredentialsRequired is called.
    //
    //  See Also:
    //      IPrivateSceBaseComponent
    //
    //==========================================================================
    virtual void EvInvalidCredentials(IN IPrivateSceChallengeSource* pComponent,
                                      IN const CString& rstrProtocol,
                                      IN const CString& rstrRealm,
                                      IN mxt_opaque opqSourceData) = 0;

// BRCM_CUSTOM - [add] Add server digest authentication support
    //==========================================================================
    //==
    //==  EvSvrAuthenticationFailed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Handle server authentication rejection.
    //
    //  Parameters:
    //    pComponent:
    //      The IPrivateSceChallengeSource of the component on which the
    //      server authentication rejection occured.
    //
    //    rstrProtocol:
    //      The protocol for which the credentials are associated.
    //
    //  Description:
    //      Reports that the server fails to authenticate itself to the client
    //      by providing an invalid "response-digest" in a 2xx response.
    //
    //==========================================================================
    virtual void EvSvrAuthenticationFailed(IN IPrivateSceChallengeSource* pComponent,
                                           IN const CString& rstrProtocol) = 0;
// BRCM_CUSTOM - [end] Add server digest authentication support

// BRCM_CUSTOM - [add] Add authentication info sharing support
    //==========================================================================
    //==
    //==  EvAuthenticationSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      Handle received successful authentication response.
    //
    //  Parameters:
    //    pComponent:
    //      The IPrivateSceChallengeSource of the component on which a
    //      successful response is received.
    //
    //    rstrProtocol:
    //      The protocol for which the credentials are associated.
    //
    //    opqChallengeId:
    //      The identifier representing the last updated challenge information
    //      based on the received successful authentication response. Note that
    //      this ID is only valid within the callback's context.
    //
    //  Description:
    //      Reports that a successful authentication response is received and
    //      the stack service has finished updating its internal challenge
    //      info.
    //
    //==========================================================================
    virtual void EvAuthenticationSuccess(IN IPrivateSceChallengeSource* pComponent,
                                         IN const CString& rstrProtocol,
                                         IN mxt_opaque opqChallengeId ) = 0;

    //==========================================================================
    //==
    //==  EvNonceEmployed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Handle nonce consumption in outgoing requests.
    //
    //  Parameters:
    //    pComponent:
    //      The IPrivateSceChallengeSource of the component on which the
    //      nonce was consumed.
    //
    //    opqChallengeId:
    //      The identifier representing the last updated challenge information
    //
    //  Description:
    //      Reports that a request has been updated to employ a nonce value.
    //
    //==========================================================================    
    virtual void EvNonceEmployed(IN IPrivateSceChallengeSource* pComponent,
                                 IN mxt_opaque opqChallengeId) = 0;
// BRCM_CUSTOM - [end] Add authentication info sharing support

//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IPrivateSceAuthentication() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IPrivateSceAuthentication() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IPrivateSceAuthentication(IN const IPrivateSceAuthentication& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IPrivateSceAuthentication& operator=(IN const IPrivateSceAuthentication& rSrc);


//-- Hidden Data Members.
//------------------------
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_IPRIVATESCEAUTHENTICATION_H
