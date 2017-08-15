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
#ifndef MXG_IUASSPNEWCALLMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPNEWCALLMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSipPacket;
class ISceUserConfig;
class IUaSspCall;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspNewCallMgr
//========================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   This interface provides notification of the creation of a new call object
//   in server mode.
//
// Description:
//  This interface provides notification of the creation of a new call object in
//  server mode. This means that the call object was created as a result of a
//  call attempt from another entity.
//
// Location:
//   UaSsp/IUaSspNewCallMgr.h
//
// See Also:
//   IUaSspCallHandler, IUaSspCall, IUaSspCallMgr
//
//==============================================================================
class IUaSspNewCallMgr
{
//-- Friend Declarations.


//-- Published Interface.
public:


    //==========================================================================
    //==
    //==  EvNewCallObject
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a new call object was created.
    //
    //  Parameters:
    //    pCall:
    //      The new call object. Will never be NULL. The implementation must
    //      keep a reference to this call by using AddIfRef.
    //
    //    pUserConfig:
    //      User configuration object associated with the called user. This can
    //      be NULL if no matching local user was found.
    //
    //    rPacket:
    //      Reference to the incoming SIP packet that created the new call.
    //
    //  Description:
    //      Reports to the application that a new call object was created to
    //      handle an incoming call.
    //
    //      The implementation of this event must do the following,
    //      synchronously:
    //      - If pUserConfig is NULL and the implementation wants to handle
    //        incoming calls for unregistered users, then it should associate a
    //        user configuration object with the call by using
    //        IUaSspCall::SetConfiguration. The proper user configuration object
    //        can be found or created based on the information held by rPacket.
    //        If pUserConfig is NULL and the implementation does not set a valid
    //        user configuration object, the call will be automatically rejected
    //        with a "404 User Not Found" response.
    //      - The implementation must set a valid IUaSspCallMgr by using
    //        IUaSspCall::SetManager.
    //
    //      The implementation must not perform any other action on pCall.
    //      Moreover, the implementation must not yet notify the local user of
    //      the reception of a new call, it must wait for
    //      IUaSspCallMgr::EvCalled.
    //
    //      After this event is reported, the call will either progress or be
    //      rejected, resulting in either IUaSspCallMgr::EvCalled or
    //      IUaSspCallMgr::EvTerminated being reported to the manager of the
    //      call.
    //
    //      Any call reported here will eventually result in
    //      IUaSspCallMgr::EvTerminated.
    //
    //      The call can be automatically rejected for various reasons:
    //          - No user configuration object was configured
    //          - The caller required an unsupported extension
    //          - The call is not from a trusted source or does not go through
    //            a trusted proxy
    //          - The new call is the result of a retransmission of an INVITE
    //            request that is already being managed by another call object.
    //
    //  See Also:
    //      IUaSspCallMgr
    //
    //==========================================================================
    virtual void EvNewCallObject(IN IUaSspCall* pCall,
                                 IN ISceUserConfig* pUserConfig,
                                 IN const CSipPacket& rPacket) = 0;


//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaSspNewCallMgr() {}

    // Summary:
    //  Destructor.
    virtual ~IUaSspNewCallMgr() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaSspNewCallMgr(IN const IUaSspNewCallMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaSspNewCallMgr& operator=(IN const IUaSspNewCallMgr& rSrc);


//-- Hidden Data Members.
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPNEWCALLMGR_H
