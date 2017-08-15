//==============================================================================
//==============================================================================
//
//          Copyright(c) 2011 Media5 Corporation. ("Media5")
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
#ifndef MXG_CSIPUAFORKEDDIALOGGROUPER_H
#define MXG_CSIPUAFORKEDDIALOGGROUPER_H

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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT to be \
    able to use this class.
#endif

//-- Data Members.

//-- Parameters used by Value.

//-- Interface Realized and/or Parent.
#ifndef MXG_CSIPFORKEDDIALOGGROUPER_H
#include "SipCore/CSipForkedDialogGrouper.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class ISipUserAgentSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipUaForkedDialogGrouper
//========================================
//
// Description:
//   This class regroups contexts representing dialogs created from a unique
//   request that has been forked for a UA device.
//
// Location:
//   SipUserAgent/CSipUaForkedDialogGrouper.h
//
// See Also:
//   CSipForkedDialogGrouper
//
//==============================================================================
class CSipUaForkedDialogGrouper : public CSipForkedDialogGrouper
{
//-- Friend Declarations.

//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipUaForkedDialogGrouper(IN ISipForkedDialogGrouperMgr& rMgr,
                              IN IPrivateSipContext& rOriginatorContext,
// BRCM_CUSTOM - [add] Control To-tag matching choices
                              IN bool bMatchToTagPerRfc3261,
// BRCM_CUSTOM - [end] Control To-tag matching choices
                              IN const CToken* pCreatingDialogRequestMethod = NULL);

    //-- << ISipForkedDialogGrouper Interface >>

    virtual mxt_result ContextCreated(IN ISipContext& rContext,
                                      IN const CSipPacket& rPacket,
                                      IN mxt_opaque opqTransaction);

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Destructor. Protected because this object is reference counted and is a
    //  CEventDriven.
    virtual ~CSipUaForkedDialogGrouper();

private:

    //-- << Helper methods >>

    // Summary:
    //  Copies the data from the original ISipUserAgentSvc to the new one.
    mxt_result CopyDialogData(IN ISipUserAgentSvc* pNewUserAgentSvc, IN ISipUserAgentSvc* pOriginalUserAgentSvc);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipUaForkedDialogGrouper(IN const CSipUaForkedDialogGrouper& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipUaForkedDialogGrouper& operator=(IN const CSipUaForkedDialogGrouper& rSrc);


//-- Hidden Data Members.
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPUAFORKEDDIALOGGROUPER_H

