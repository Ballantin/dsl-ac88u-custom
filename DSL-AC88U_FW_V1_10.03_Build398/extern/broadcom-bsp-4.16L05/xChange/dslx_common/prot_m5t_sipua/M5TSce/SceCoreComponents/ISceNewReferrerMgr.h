//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCENEWREFERRERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCENEWREFERRERMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCECORECOMPONENTSCFG_H
#include "Config/SceCoreComponentsCfg.h"
#endif


#if !defined(MXD_SCE_ENABLE_REFERRER_COMPONENT_SUPPORT)
#error You must define MXD_SCE_ENABLE_REFERRER_COMPONENT_SUPPORT to be able to \
use this class.
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSipPacket;
class ISceUserConfig;
class ISceReferrer;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISceNewReferrerMgr
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface provides notification of the creation of a new referrer
//   object in server mode.
//
// Description:
//  This interface provides notification of the creation of a new referrer object
//  in server mode. This means that the referrer object was created as a result
//  of a out-of-dialog refer attempt from another entity.
//
// Location:
//   SceCoreComponents/ISceNewReferrerMgr.h
//
// See Also:
//   ISceReferHandler, ISceReferrer, ISceReferrerMgr
//
//==EDOC========================================================================
class ISceNewReferrerMgr
{
//-- Friend Declarations.


//-- Published Interface.
public:

    //==========================================================================
    //==
    //==  EvNewReferrerObject
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a new referrer object was created.
    //
    //  Parameters:
    //    pReferrer:
    //      The new referrer object. Will never be NULL. The implementation must
    //      keep a reference to this referrer by using AddIfRef.
    //
    //    pUserConfig:
    //      User configuration object associated with the referred user. This can
    //      be NULL if no matching local user was found.
    //
    //    rPacket:
    //      Reference to the incoming SIP packet that created the new referrer.
    //
    //  Description:
    //      Reports to the application that a new referrer object was created to
    //      handle an incoming REFER request.
    //
    //      The implementation of this event must do the following,
    //      synchronously:
    //      - If pUserConfig is NULL and the implementation wants to handle
    //        incoming REFERs for unregistered users, then it should associate a
    //        user configuration object with the referrer by using
    //        ISceReferrer::SetConfiguration. The proper user configuration object
    //        can be found or created based on the information held by rPacket.
    //        If pUserConfig is NULL and the implementation does not set a valid
    //        user configuration object, the REFER will be automatically
    //        rejected with a "404 User Not Found" response.
    //      - The implementation must set a valid ISceReferrerMgr by using
    //        ISceReferrer::SetManager.
    //
    //      The implementation must not perform any other action on pReferrer
    //      within the context of this event.
    //
    //  See Also:
    //      ISceReferrerMgr
    //
    //==========================================================================
    virtual void EvNewReferrerObject(IN ISceReferrer* pReferrer,
                                     IN ISceUserConfig* pUserConfig,
                                     IN const CSipPacket& rPacket) = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceNewReferrerMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISceNewReferrerMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceNewReferrerMgr(IN const ISceNewReferrerMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceNewReferrerMgr& operator=(IN const ISceNewReferrerMgr& rSrc);


//-- Hidden Data Members.
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCENEWREFERRERMGR_H

