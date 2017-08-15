//==============================================================================
//==============================================================================
//
//      Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPOPTIONTAGSMGR_H
#define MXG_ISIPOPTIONTAGSMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT \
    to be able to use this class.
#endif

//-- Data Members.

//-- Parameters used by Value.

//-- Interface Realized and/or Parent.


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class ISipOptionTagsSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipOptionTagsMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
// <FLAG New in 4.1.8>
//
// Description:
//  The Option Tags manager is the interface through which the Option Tags
//  service reports events to the application. The Option Tags service informs
//  the application through this interface of incoming requests with Require
//  header values that does not match the application's supported capabilities.
//
// Location:
//   SipUserAgent/ISipOptionTagsMgr.h
//
// See Also:
//   ISipOptionTagsMgr
//
//==============================================================================
class ISipOptionTagsMgr
{
//-- Friend Declarations.


//-- Published Interface.
public:

    //==========================================================================
    //==
    //==  EvUnsupportedTag
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a SIP request has been automatically rejected.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipOptionTagsSvc that refused the request.
    //
    //    opqApplicationData:
    //      The application opaque data.
    //
    //    rRequest:
    //      The rejected SIP request.
    //
    //  Description:
    //      Informs the manager when the ISipOptionTagsSvc automatically
    //      refuses an incoming request with a 420 response. The application
    //      can clear/release the context upon reception of this event.
    //
    //==========================================================================
    virtual void EvUnsupportedTag(IN ISipOptionTagsSvc* pSvc,
                                  IN mxt_opaque opqApplicationData,
                                  IN const CSipPacket& rRequest) = 0;


//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipOptionTagsMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISipOptionTagsMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipOptionTagsMgr(IN const ISipOptionTagsMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipOptionTagsMgr& operator=(IN const ISipOptionTagsMgr& rSrc);

//-- Hidden Data Members.
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPOPTIONTAGSMGR_H

