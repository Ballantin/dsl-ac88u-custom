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
#ifndef MXG_ISIPPACKETOBSERVERSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPPACKETOBSERVERSVC_H

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
class ISipPacketObserverSvcMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(ISipPacketObserverSvc);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipPacketObserverSvc
//========================================
//<GROUP TID_PACKAGE_SCESTACKSERVICES_CLASSES>
//
// Description:
//  This interface defines the API of the packet observer services for the SIP
//  Stack. This service MUST be the first service attached to a SIP context to
//  ensure that it can report its events before any other services. It will 
//  report its events through the ISipPacketObserverSvcMgr interface. 
//
// Location:
//   SceStackServices/ISipPacketObserverSvc.h
//
// See Also:
//   ISipPacketObserverSvcMgr
//
//==============================================================================
class ISipPacketObserverSvc : public IEComUnknown
{
//-- Friend Declarations.

//-- Published Interface.
public:
    MX_DECLARE_ECOM_GETIID(ISipPacketObserverSvc);

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
    virtual mxt_result SetManager(IN ISipPacketObserverSvcMgr* pMgr) = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipPacketObserverSvc() {}

    // Summary:
    //  Destructor.
    virtual ~ISipPacketObserverSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipPacketObserverSvc(IN const ISipPacketObserverSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipPacketObserverSvc& operator=(IN const ISipPacketObserverSvc& rSrc);

//-- Hidden Data Members.
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_ISIPPACKETOBSERVERSVC_H
