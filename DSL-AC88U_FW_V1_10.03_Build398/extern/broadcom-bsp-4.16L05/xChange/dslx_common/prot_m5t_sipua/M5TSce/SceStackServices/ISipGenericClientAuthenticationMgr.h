//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPGENERICCLIENTAUTHENTICATIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPGENERICCLIENTAUTHENTICATIONMGR_H

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
class ISipClientEventControl;
class ISipServerEventControl;
class ISipGenericClientAuthenticationSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipGenericClientAuthenticationMgr
//========================================
//<GROUP TID_PACKAGE_SCESTACKSERVICES_CLASSES>
//
// Description:
//  This interface defines callbacks used by the
//  ISipGenericClientAuthenticationSvc to report its events.
//
// Location:
//   SceStackServices/ISipPacketObserverSvcMgr.h
//
// See Also:
//   ISipPacketObserverSvc
//
//==============================================================================
class ISipGenericClientAuthenticationMgr
{
//-- Friend Declarations.

//-- Published Interface.
public:

    //==========================================================================
    //==
    //==  EvChallenged
    //==
    //==========================================================================
    //
    //  Summary:
    //      Event reported when the ISipGenericClientAuthenticationSvc detects
    //      an unhandled 401 or 407 response.
    //
    //  Parameters:
    //    pSvc:
    //      The service reporting the event.
    //
    //    pClientEventControl:
    //      The M5T SIP-UA client event control that must be used to
    //      progress further with the challenge.
    //
    //    rChallenge:
    //      The SIP packet, which is either a 401 or 407 response that should
    //      contain all necessary challenge information.
    //
    //  Description:
    //      This event is reported only when the application instructs the basic
    //      extension control to report it (see
    //      ISceBasicExtensionControl::EnableAuthenticationCallback). Otherwise,
    //      the ISipGenericClientAuthenticationSvc service is not attached to
    //      the SIP context, thus this event cannot be reported.
    //
    //      Note that this event can be reported for challenges that the SIP
    //      stack is able to handle. This will occur when the
    //      EnableAuthenticationCallback method enables the callback while
    //      disabling default authentication methods.
    //
    //      This event will also be reported if the authentication callbacks
    //      are enabled with the default authentication methods also enabled and
    //      the application rejects a challenge through the regular interfaces.
    //
    //  See Also:
    //      ISipGenericClientAuthenticationSvc
    //
    //==========================================================================
    virtual void EvChallenged(IN ISipGenericClientAuthenticationSvc* pSvc,
                              IN ISipClientEventControl* pClientEventControl,
                              IN const CSipPacket& rChallenge) = 0;


//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipGenericClientAuthenticationMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISipGenericClientAuthenticationMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipGenericClientAuthenticationMgr(IN const ISipGenericClientAuthenticationMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipGenericClientAuthenticationMgr& operator=(IN const ISipGenericClientAuthenticationMgr& rSrc);

//-- Hidden Data Members.
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_ISIPGENERICCLIENTAUTHENTICATIONMGR_H
