//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSCEUSERAUTHENTICATION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEUSERAUTHENTICATION_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEUSERAUTHENTICATION_H
#include "SceCoreComponents/ISceUserAuthentication.h"
#endif

#ifndef MXG_IPRIVATESCEAUTHENTICATION_H
#include "SceCoreComponents/IPrivateSceAuthentication.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class ISceUserAuthenticationMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceUserAuthentication
//========================================
//
// Summary:
//   This class is the central point where all the authentication intelligence
//   resides.
//
// Description:
//   This class is the central point where all the authentication intelligence
//   resides. All components requiring authentication use this class to
//   determine what action is necessary to take. The components are
//   communicating with this class through the IPrivateSceAuthentication
//   interface. When it determines that the application needs to provide
//   credentials for a specific realm, the EvChallenged event is reported on the
//   application. The application then has the choice to provide the credentials
//   or to cancel the authentication process if none can be provided. This class
//   holds all the credentials provided by the application and all the pending
//   challenges received from the remote peers.
//
// Location:
//   SceCoreComponents/CSceUserAuthentication.h
//
//==============================================================================
class CSceUserAuthentication : private CEComDelegatingUnknown,
                               private ISceUserAuthentication,
                               private IPrivateSceAuthentication
{
//-- Friend Declarations
//------------------------
    friend class CSceCoreComponentsInitializer;

//-- New types within class namespace
//-----------------------------------
public:
protected:
private:
    // The credentials for a specific realm, it consists of a username and of a
    // hash of A1 string.
    //--------------------------------------------------------------------------
    struct SCredential
    {
        CString m_strRealm;
        CString m_strUsername;
        CString m_strPassword;
        CString m_strProtocol;
    };

    // A challenge received from one of the UA SSP components.
    //---------------------------------------------------------
    struct SChallenge
    {
        CString m_strRealm;
        CString m_strProtocol;
        IPrivateSceChallengeSource* m_pComponent;
        mxt_opaque m_opqSourceData;
    };

//-- Published Interface
//------------------------
public:
//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSceUserAuthentication(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSceUserAuthentication();

    //-- << Initialization mechanism >>
    //----------------------------------

    static mxt_result InitializeCSceUserAuthentication();

    static void FinalizeCSceUserAuthentication();

    //-- << ECOM mechanism >>
    //------------------------
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComDelegatingUnknown >>
    //--------------------------------------------
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << ISceUserAuthentication >>
    //--------------------------------------------

    virtual mxt_result SetManager(IN ISceUserAuthenticationMgr* pMgr);
    virtual void AddCredentials(IN const CString& rstrRealm,
                                IN const CString& rstrUsername,
                                IN const CString& rstrPassword,
                                IN const CString& rstrProtocol);
    virtual void CancelAuthentication(IN const CString& rstrRealm,
                                      IN const CString& rstrProtocol);
// BRCM_CUSTOM - [add] Add authentication info sharing support 
    virtual mxt_opaque CloneChallengeInfo(IN mxt_opaque opqChallengeId);

    virtual void DeleteChallengeInfo(IN mxt_opaque opqChallengeId);

    virtual bool IsChallengeInfoEqual(IN mxt_opaque opqChallengeIdFrom,
                                      IN mxt_opaque opqChallengeIdTo);

    virtual void GetChallengeNonceInfo(IN mxt_opaque opqChallengeIdFrom,
                                      OUT CString& rstrNonce);
// BRCM_CUSTOM - [end] Add authentication info sharing support
    virtual void SetStaleNonceBehavior(IN EStaleNonceBehavior eBehavior);
    virtual EStaleNonceBehavior GetStaleNonceBehavior() const;
    virtual mxt_result SetAuthenticationLoopThreshold(unsigned int uMax);
    virtual unsigned int GetAuthenticationLoopThreshold() const;

    //-- << IPrivateSceAuthentication >>
    //--------------------------------------------

    virtual void EvCredentialsRequired(IN IPrivateSceChallengeSource* pComponent,
                                       IN const CString& rstrProtocol,
                                       IN const CString& rstrRealm,
                                       IN mxt_opaque opqSourceData);
    virtual void EvInvalidCredentials(IN IPrivateSceChallengeSource* pComponent,
                                      IN const CString& rstrProtocol,
                                      IN const CString& rstrRealm,
                                      IN mxt_opaque opqSourceData);
// BRCM_CUSTOM - [add] Add server digest authentication support
    virtual void EvSvrAuthenticationFailed(IN IPrivateSceChallengeSource* pComponent,
                                           IN const CString& rstrProtocol);
// BRCM_CUSTOM - [end] Add server digest authentication support
// BRCM_CUSTOM - [add] Add authentication info sharing support
    virtual void EvAuthenticationSuccess(IN IPrivateSceChallengeSource* pComponent,
                                         IN const CString& rstrProtocol,
                                         IN mxt_opaque opqChallengeId);

    virtual void EvNonceEmployed(IN IPrivateSceChallengeSource* pComponent,
                                 IN mxt_opaque opqChallengeId);
// BRCM_CUSTOM - [end] Add authentication info sharing support

    //-- << Helpers >>
    //--------------------------------------------

    // Summary:
    //  Finds credentials from the internal vector.
    //---------------------------------------------
    SCredential* FindCredential(IN const CString& rstrRealm,
                                IN const CString& rstrProtocol);

    // Summary:
    //  Authenticates every ongoing challenge with the provided credentials.
    //----------------------------------------------------------------------
    void AnswerChallenges(IN const SCredential& rstCredential);


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSceUserAuthentication(IN const CSceUserAuthentication& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSceUserAuthentication& operator=(IN const CSceUserAuthentication& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
    //-- << Message Numbers >>.
    //--------------------------
    enum
    {
        eMSG_ADD_CREDENTIALS = 0,
        eMSG_CANCEL_AUTHENTICATION,
        eMSG_SET_OPAQUE,
        eMSG_GET_OPAQUE
    };

    // The authentication manager.
    //-----------------------------
    ISceUserAuthenticationMgr* m_pMgr;

    // The credentials saved for this user.
    //--------------------------------------
    CVector<SCredential*> m_vecpstCredentials;

    // The pending challenges we have for this user.
    //-----------------------------------------------
    CVector<SChallenge*> m_vecpstChallenges;

    // The stale NONCE behavior.
    EStaleNonceBehavior m_eBehavior;

    // The configured authentication loop threshold.
    unsigned int m_uAuthenticationLoopThreshold;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCEUSERAUTHENTICATION_H


