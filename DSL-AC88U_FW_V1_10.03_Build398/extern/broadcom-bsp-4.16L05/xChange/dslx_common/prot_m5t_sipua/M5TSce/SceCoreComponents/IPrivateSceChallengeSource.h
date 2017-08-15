//==============================================================================
//==============================================================================
//
//          Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_IPRIVATESCECHALLENGESOURCE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IPRIVATESCECHALLENGESOURCE_H

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

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IPrivateSceChallengeSource);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IPrivateSceChallengeSource
//========================================
//
// Description:
//   This is a private interface for the internal UA mechanism. It MUST NOT be
//   used externally. Please use ISceUserAuthentication instead.
//
//   This interface defines the different methods called during authentication.
//
//#The IPrivateSceChallengeSource is an ECOM object#
//
//  The IPrivateSceChallengeSource is an ECOM object that is accessed
//  through the following ECOM identifier:
//
//  <B>Interface Id: IID_IPrivateSceChallengeSource</B>
//
// Location:
//   SceCoreComponents/IPrivateSceChallengeSource.h
//
//==============================================================================
class IPrivateSceChallengeSource : public IEComUnknown
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
    MX_DECLARE_ECOM_GETIID(IPrivateSceChallengeSource);

    //==========================================================================
    //==
    //==  SetCredentials
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides the credentials to use for this challenge.
    //
    //  Parameters:
    //    strRealm:
    //      The realm for this challenge.
    //
    //    strUsername:
    //       The username for this challenge.
    //
    //    strPassword:
    //       The password for this challenge.
    //
    //    opqSourceData:
    //       The opaque data passed to
    //       IPrivateSceAuthentication::EvCredentialsRequired or
    //       IPrivateSceAuthentication::EvInvalidCredentials must be passed back
    //       unmodified through this parameter.
    //
    //  Description:
    //      Provides the credentials and extra data to be used for this
    //      challenge. This method should reissue the request using the provided
    //      credentials.
    //
    //  See Also:
    //      IPrivateSceBaseComponent
    //
    //==========================================================================
    virtual mxt_result SetCredentials(IN const CString& strRealm,
                                      IN const CString& strUsername,
                                      IN const CString& strPassword,
                                      IN mxt_opaque opqSourceData) = 0;


    //==========================================================================
    //==
    //==  CancelAuthentication
    //==
    //==========================================================================
    //
    //  Summary:
    //      Cancels the authentication for a challenge.
    //
    //  Parameters:
    //    rstrRealm:
    //       The realm for which to cancel authentication.
    //
    //    opqSourceData:
    //       The opaque data passed to
    //       IPrivateSceAuthentication::EvCredentialsRequired or
    //       IPrivateSceAuthentication::EvInvalidCredentials must be passed back
    //       unmodified through this parameter.
    //
    //  Description:
    //      Cancels the authentication for a challenge. This method should
    //      cancel the request.
    //
    //  See Also:
    //      IPrivateSceBaseComponent
    //
    //==========================================================================
    virtual mxt_result CancelAuthentication(IN const CString& strRealm,
                                            IN mxt_opaque opqSourceData) = 0;


//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IPrivateSceChallengeSource() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IPrivateSceChallengeSource() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IPrivateSceChallengeSource(IN const IPrivateSceChallengeSource& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IPrivateSceChallengeSource& operator=(IN const IPrivateSceChallengeSource& rSrc);


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

#endif //-- #ifndef MXG_IPRIVATESCECHALLENGESOURCE_H
