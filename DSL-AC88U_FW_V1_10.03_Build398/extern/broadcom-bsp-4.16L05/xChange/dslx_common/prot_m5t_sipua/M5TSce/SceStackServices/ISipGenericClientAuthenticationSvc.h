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
#ifndef MXG_ISIPGENERICCLIENTAUTHENTICATIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPGENERICCLIENTAUTHENTICATIONSVC_H

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
class ISipGenericClientAuthenticationMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(ISipGenericClientAuthenticationSvc);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipGenericClientAuthenticationSvc
//========================================
//<GROUP TID_PACKAGE_SCESTACKSERVICES_CLASSES>
//
// Description:
//  This interface defines the API of the generic client authentication extension
//  service. This custom SIP stack service must be attached to a SIP context
//  after the ISipDigestClientAuthSvc service or in place of that service.
//
//  This extension service does not do much. It only monitors the incoming
//  responses for 401 and 407 responses and reports them to its manager. It does
//  not handle the authentication by itself. The manager must handle the
//  authentication through the client event control and the
//  ISipCoreOutputControllingSvc.
//
// Location:
//   SceStackServices/ISipGenericClientAuthenticationSvc.h
//
// See Also:
//   ISipPacketObserverSvcMgr
//
//==============================================================================
class ISipGenericClientAuthenticationSvc : public IEComUnknown
{
//-- Friend Declarations.

//-- Published Interface.
public:
    MX_DECLARE_ECOM_GETIID(ISipGenericClientAuthenticationSvc);

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which events are reported for this
    //      service.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. It must not be NULL.
    //
    //  Returns:
    //      resS_OK: The manager has been set.
    //      resFE_INVALID_ARGUMENT: Could not set a NULL manager.
    //
    //  Description:
    //      Configures the manager to which events are reported for this
    //      SIP Stack service.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN ISipGenericClientAuthenticationMgr* pMgr) = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipGenericClientAuthenticationSvc() {}

    // Summary:
    //  Destructor.
    virtual ~ISipGenericClientAuthenticationSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipGenericClientAuthenticationSvc(IN const ISipGenericClientAuthenticationSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipGenericClientAuthenticationSvc& operator=(IN const ISipGenericClientAuthenticationSvc& rSrc);

//-- Hidden Data Members.
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_ISIPGENERICCLIENTAUTHENTICATIONSVC_H
