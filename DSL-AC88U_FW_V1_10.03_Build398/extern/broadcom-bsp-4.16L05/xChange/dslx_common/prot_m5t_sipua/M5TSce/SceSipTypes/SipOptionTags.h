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
#ifndef MXG_SIPOPTIONTAGS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SIPOPTIONTAGS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCECORECFG_H
#include "Config/SceCoreCfg.h" // MXD_OVERRIDE_TYPE_OPTION_TAG
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
//-------------------------
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// Make sure that either both options are not defined or that both options are
// defined.
//-----------------------------------------------------------------------------

#if !defined(MXD_OVERRIDE_TYPE_OPTION_TAG) && defined(MXD_OVERRIDE_TYPE_OPTION_TAG_STRINGS)
#error Must synchronize type and strings.
#endif

#if defined(MXD_OVERRIDE_TYPE_OPTION_TAG) && !defined(MXD_OVERRIDE_TYPE_OPTION_TAG_STRINGS)
#error Must synchronize type and strings.
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if !defined(MXD_OVERRIDE_TYPE_OPTION_TAG)

//<GROUP TID_PACKAGE_SCECORE_STRUCTSENUMS>
// Summary:
//  Defines SIP option tags.
//
// Description:
//  This type defines the currently registered SIP options tag at the IANA.
// 
//  It is allowed to add non-IANA supported types, but such items must be
//  clearly indicated.
// 
//  The list of registered option tags can be found at:
//  http://www.iana.org/assignments/sip-parameters.
//
//  This type must be ordered alphabetically without regards to case.
//
//  This type can be overriden by defining MXD_OVERRIDE_TYPE_OPTION_TAG.
//
//  See Also:
//      MXD_OVERRIDE_TYPE_OPTION_TAG
enum ESipOptionTag
{
    //*********
    // WARNING:
    // ********* 
    // When updating this table, you may also have to update the PIDF values
    // associated with this event. See uPIDFSERVCAPSEXT_XXX in CPidfData.h.


    //M5T_INTERNAL_USE_BEGIN
    eOPTIONTAG_FIRST              = 0x00000001,      // First
    //M5T_INTERNAL_USE_END

    eOPTIONTAG_100REL             = 0x00000001,      // [RFC3262]
    eOPTIONTAG_ANSWERMODE         = 0x00000002,      // [RFC5373]
    eOPTIONTAG_EARLY_SESSION      = 0x00000004,      // [RFC3959]
    eOPTIONTAG_EVENTLIST          = 0x00000008,      // [RFC4662]
    eOPTIONTAG_FROM_CHANGE        = 0x00000010,      // [RFC4916]
    eOPTIONTAG_GRUU               = 0x00000020,      // [RFCYYYY] draft-ietf-sip-gruu-14
    eOPTIONTAG_HISTINFO           = 0x00000040,      // [RFC4244]
    eOPTIONTAG_JOIN               = 0x00000080,      // [RFC3911]
    eOPTIONTAG_NOREFERSUB         = 0x00000100,      // [RFC4488]
    eOPTIONTAG_OUTBOUND           = 0x00000200,      // [RFC5626]
    eOPTIONTAG_PATH               = 0x00000400,      // [RFC3327]
    eOPTIONTAG_PRECONDITION       = 0x00000800,      // [RFC3312] 
    eOPTIONTAG_PREF               = 0x00001000,      // [RFC3840]
    eOPTIONTAG_PRIVACY            = 0x00002000,      // [RFC3323]
    eOPTIONTAG_REPLACES           = 0x00004000,      // [RFC3891]
    eOPTIONTAG_RESOURCE_PRIORITY  = 0x00008000,      // [RFC4412]
    eOPTIONTAG_SDP_ANAT           = 0x00010000,      // [RFC4092] 
    eOPTIONTAG_SEC_AGREE          = 0x00020000,      // [RFC3329]
    eOPTIONTAG_TDIALOG            = 0x00040000,      // [RFC4538]
    eOPTIONTAG_TIMER              = 0x00080000,      // [RFC4028]

    //M5T_INTERNAL_USE_BEGIN
    eOPTIONTAG_LAST               = 0x00100000       // Last Item
    //M5T_INTERNAL_USE_END
};
#else
// Use the updated type instead, which is defined within
// MXD_OVERRIDE_TYPE_OPTION_TAG.
//-------------------------------------------------------
MXD_OVERRIDE_TYPE_OPTION_TAG
#endif

//M5T_INTERNAL_USE_BEGIN

// Summary:
//  Initializes the Sip option tags.
//-----------------------------------
mxt_result InitializeSipOptionTags();

// Summary:
//  Finalizes the Sip option tags.
//---------------------------------
void FinalizeSipOptionTags();

extern const char* const g_aszSIPOPTIONTAG[];
extern CVector<CString>* g_pvecstrSIPOPTIONTAG;

//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SIPOPTIONTAGS_H

