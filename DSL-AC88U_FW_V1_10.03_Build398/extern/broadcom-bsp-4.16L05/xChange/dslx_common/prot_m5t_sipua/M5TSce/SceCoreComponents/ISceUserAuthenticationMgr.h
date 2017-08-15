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
#ifndef MXG_ISCEUSERAUTHENTICATIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEUSERAUTHENTICATIONMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
//-------------------------
class ISceUserAuthentication;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceUserAuthenticationMgr
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines the client authentication events that can be
//   reported to the application.
//
// Description:
//  This interface defines the client authentication events that can be reported
//  to the application.
//
//  This interface can be used to report authentication events for multiple
//  protocols. The rstrProtocol parameter of EvChallenged identifies the
//  protocol that is currently being challenged. As an example, for a challenged
//  SIP request, rstrProtocol will be "SIP", while for a challenged XCAP
//  request, rstrProtocol will be "HTTP" since XCAP simply uses over HTTP.
//
// Location:
//   SceCoreComponents/ISceUserAuthenticationMgr.h
//
// See Also:
//   ISceUserAuthentication
//
//==============================================================================
class ISceUserAuthenticationMgr
{
//-- Friend Declarations.
//------------------------


//-- Published Interface.
//------------------------
public:
    //==========================================================================
    //==
    //==  EvChallenged
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that some of the actions taken by the SIP Client Engine are
    //      being challenged.
    //
    //  Parameters:
    //    pAuth:
    //      Pointer to the ISceUserAuthentication to use for providing
    //      authentication information. This is never NULL.
    //
    //    rstrRealm:
    //      The realm for which the user is being challenged.
    //
    //    rstrProtocol:
    //      The protocol for which the user is being challenged.
    //
    //  Description:
    //      This event is reported by the ISceUserAuthentication when it
    //      detects that a request it sent was challenged for a realm for which
    //      it has no working credentials.
    //
    //      The implementation should check for proper credentials (whether by
    //      database lookup, asking the current user, etc.) and then provide the
    //      credentials by calling
    //      ISceUserAuthentication::AddCredentials. If no credentials
    //      are available, ISceUserAuthentication::CancelAuthentication
    //      must be called instead.
    //
    //      When calling ISceUserAuthentication::AddCredentials, the
    //      implementation has the choice to specify whether the provided
    //      credential applies to only this protocol or to any protocol using
    //      digest authentication for the specified realm. 
    //
    //  See Also:
    //      ISceUserAuthentication::AddCredentialsA,
    //      ISceUserAuthentication::CancelAuthenticationA
    //
    //==========================================================================
    virtual void EvChallenged(IN ISceUserAuthentication* pAuth,
                              IN const CString& rstrRealm,
                              IN const CString& rstrProtocol) = 0;

// BRCM_CUSTOM - [add] Add server digest authentication support
    //==========================================================================
    //==
    //==  EvSvrAuthenticationFailed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the server fails to authenticate itself.
    //
    //  Parameters:
    //    pAuth:
    //      Pointer to the ISceUserAuthentication to use for providing
    //      authentication information. This is never NULL.
    //
    //    rstrProtocol:
    //      The protocol for which the server fails to authenticate itself.
    //
    //  Description:
    //      This event is reported by the ISceUserAuthentication when it
    //      detects that the "response-digest" received from the authentication
    //      server is invalid. The application can either ignore this event
    //      or re-authenticate the server again by sending a branch new request.
    //
    //==========================================================================
    virtual void EvSvrAuthenticationFailed(IN ISceUserAuthentication* pAuth,
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
    //      Reports that a successful authentication response is received and
    //      the stack has finished updating its credentials based on this new
    //      value.
    //
    //  Parameters:
    //    pAuth:
    //      Pointer to the ISceUserAuthentication to use for providing
    //      authentication information. This is never NULL.
    //
    //    rstrProtocol:
    //      The protocol for which the response is received.
    //
    //    opqChallengeId:
    //      The identifier representing the last updated challenge information
    //      based on the received response value. Note that this ID is only
    //      valid within the callback's context. The application can call
    //      ISceUserAuthentication::CloneChallengeInfo() to create a duplicate
    //      of the challenge info.
    //
    //  Description:
    //      This event is reported by the ISceUserAuthentication when it
    //      detects that a successful authentication response is received. The
    //      application can either ignore this event or update other component's
    //      challenge information by calling
    //      ISceBasicExtensionControl::AddChallengeInfo() with the challenge
    //      identifier.
    //
    //==========================================================================
    virtual void EvAuthenticationSuccess(IN ISceUserAuthentication* pAuth,
                                         IN const CString& rstrProtocol,
                                         IN mxt_opaque opqChallengeId) = 0;
   //==========================================================================
    //==
    //==  EvNonceEmployed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a request has been updated with a nonce value 
    //
    //  Parameters:
    //    pAuth:
    //      Pointer to the ISceUserAuthentication to use for providing
    //      authentication information. This is never NULL.
    //
    //    opqChallengeId:
    //      The identifier representing the last updated challenge information
    //      based on the updated value. Note that this ID is only
    //      valid within the callback's context. The application can call
    //      ISceUserAuthentication::CloneChallengeInfo() to create a duplicate
    //      of the challenge info.
    //
    //  Description:
    //      This event is reported by the ISceUserAuthentication when it
    //      detects that a request is sent with a nonce value.
    //
    //      The application should check to see if the nonce has changed
    //      and update the nonce count by calling 
    //      ISceBasicExtensionControl::SetChallengeCount(). The application
    //      can also choose to update other component's challenge information 
    //      by calling ISceBasicExtensionControl::AddChallengeInfo() with the 
    //      challenge identifier.
    //
    //==========================================================================
    virtual void EvNonceEmployed(IN ISceUserAuthentication* pAuth,
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
    ISceUserAuthenticationMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceUserAuthenticationMgr() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceUserAuthenticationMgr(IN const ISceUserAuthenticationMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceUserAuthenticationMgr& operator=(IN const ISceUserAuthenticationMgr& rSrc);

//-- Hidden Data Members.
//------------------------
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEUSERAUTHENTICATIONMGR_H

