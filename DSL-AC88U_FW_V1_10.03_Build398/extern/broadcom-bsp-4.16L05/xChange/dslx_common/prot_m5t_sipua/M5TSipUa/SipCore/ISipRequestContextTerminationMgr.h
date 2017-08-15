//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPREQUESTCONTEXTTERMINATIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPREQUESTCONTEXTTERMINATIONMGR_H
//M5T_INTERNAL_USE_END

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

#if !defined(MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT to use this interface.
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
//== Class: ISipRequestContextTerminationMgr
//==============================================================================
//<GROUP PackageName>
//
// Description:
//   This interface allows to be notified when all the request contexts for a
//   context are terminated.
//
// Location:
//   SipCore/ISipRequestContextTerminationMgr.h
//
//==============================================================================
class ISipRequestContextTerminationMgr
{
//-- Published Interface
public:

    //==========================================================================
    //==
    //==  EvAllRequestContextTerminatedForContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates that all the request contexts for the given context are
    //      now terminated.
    //
    //  Parameters:
    //      pContext:
    //       The SIP context for which there is no more ongoing transaction.
    //
    //  Description:
    //      Indicates that all the request contexts (active transactions) for
    //      the given context are now terminated.
    //
    //==========================================================================
    virtual void EvAllRequestContextTerminatedForContext(IN ISipContext* pContext) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    ISipRequestContextTerminationMgr() {}
    virtual ~ISipRequestContextTerminationMgr() {}
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREQUESTCONTEXTTERMINATIONMGR_H


