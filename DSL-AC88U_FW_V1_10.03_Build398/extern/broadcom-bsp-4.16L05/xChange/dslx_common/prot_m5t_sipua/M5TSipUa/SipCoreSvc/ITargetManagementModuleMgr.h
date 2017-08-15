//==============================================================================
//==============================================================================
//
//              Copyright(c) 2010 Media5 Corporation ("Media5")
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

#ifndef MXG_ITARGETMODULEMANAGEMENTMGR_H
#define MXG_ITARGETMODULEMANAGEMENTMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class ISipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ITargetManagementModuleMgr
//==============================================================================
//
// Description:
//   The ITargetManagementModuleMgr interface is the interface used by
//   CTargetManagementModule to report events to the application.
//
// Location:
//   SipCoreSvc/ITargetManagementModuleMgr.h
//
//==============================================================================
class ITargetManagementModuleMgr
{

//-- Published Interface
public:

    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  EvTargetIsAvailable
    //==
    //==========================================================================
    //
    //  Summary:
    //      Called when a target becomes available.
    //
    //  Parameters:
    //    pCSipContext:
    //      The associated context.
    //    eTargetTransport:
    //      The transport to be used.
    //    rTargetAddr:
    //      The target address.
    //
    //  Description:
    //      Notifies the manager that the target associated with the 
    //      ISipContext is available.
    //
    //==========================================================================
    virtual void EvTargetIsAvailable(IN ISipContext* pCSipContext,
                                     IN ESipTransport eTargetTransport,
                                     IN const CSocketAddr& rTargetAddr) = 0;

    //==========================================================================
    //==
    //==  EvTargetIsUnavailable
    //==
    //==========================================================================
    //
    //  Summary:
    //      Called when a target becomes unavailable.
    //
    //  Parameters:
    //    pCSipContext:
    //      The associated context.
    //    eTargetTransport:
    //      The transport of the unavailable target.
    //    rTargetAddr:
    //      The unavailable target address.
    //
    //  Description:
    //      Notifies the manager that the target associated with the 
    //      ISipContext is unavailable.
    //
    //==========================================================================
    virtual void EvTargetIsUnavailable(IN ISipContext* pCSipContext,
                                       IN ESipTransport eTargetTransport,
                                       IN const CSocketAddr& rTargetAddr) = 0;

//-- Hidden Methods
protected:

    //-- << Protected Constructors / Destructors >>

    // Summary:
    //  Default Constructor.
    ITargetManagementModuleMgr(){}

    // Summary:
    //  Destructor.
    virtual ~ITargetManagementModuleMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ITargetManagementModuleMgr(IN const ITargetManagementModuleMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ITargetManagementModuleMgr& operator=(IN const ITargetManagementModuleMgr& rSrc);


//-- Hidden Data Members
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ITARGETMODULEMANAGEMENTMGR_H
