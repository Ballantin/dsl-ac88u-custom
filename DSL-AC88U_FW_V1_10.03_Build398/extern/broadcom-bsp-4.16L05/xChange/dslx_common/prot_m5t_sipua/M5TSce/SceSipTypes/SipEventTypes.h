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
#ifndef MXG_SIPEVENTTYPES_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SIPEVENTTYPES_H
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
#include "Config/SceCoreCfg.h" // MXD_OVERRIDE_TYPE_EVENT_TYPE
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

#if !defined(MXD_OVERRIDE_TYPE_EVENT_TYPE) && defined(MXD_OVERRIDE_TYPE_EVENT_TYPE_STRINGS)
#error Must synchronize type and strings.
#endif

#if defined(MXD_OVERRIDE_TYPE_EVENT_TYPE) && !defined(MXD_OVERRIDE_TYPE_EVENT_TYPE_STRINGS)
#error Must synchronize type and strings.
#endif


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if !defined(MXD_OVERRIDE_TYPE_EVENT_TYPE)

//<GROUP TID_PACKAGE_SCECORE_STRUCTSENUMS>
// Summary:
//  Defines SIP event types
//
// Description:
//  This type defines the currently registered SIP event types at the IANA.
// 
//  It is allowed to add non-IANA supported types, but such items must be
//  clearly indicated.
// 
//  The list of registered types can be found at:
//  http://www.iana.org/assignments/sip-events
//
//  This type must be ordered alphabetically without regards to case.
//
enum ESipEventType
{

    //*********
    // WARNING:
    // ********* 
    // When updating this table, you may also have to update the PIDF values
    // associated with this event. See uPIDFSERVCAPSEVENTPKG_XXX in
    // SceXml/ScePidfData.h.


    //M5T_INTERNAL_USE_BEGIN
    eEVENTTYPE_FIRST              = 0x00000001,   // First
    //M5T_INTERNAL_USE_END

    eEVENTTYPE_CONFERENCE         = 0x00000001,   // [RFC4575] 
    eEVENTTYPE_DIALOG             = 0x00000002,   // [RFC4235] 
    eEVENTTYPE_KPML               = 0x00000004,   // [RFC4730]
    eEVENTTYPE_MESSAGE_SUMMARY    = 0x00000008,   // [RFC3842]
    eEVENTTYPE_POC_SETTINGS       = 0x00000010,   // [RFC4354]
    eEVENTTYPE_PRESENCE           = 0x00000020,   // [RFC3856]
    eEVENTTYPE_PRESENCE_WINFO     = 0x00000040,   // [RFC3856] [RFC3857]
    eEVENTTYPE_REFER              = 0x00000080,   // [RFC3515]
    eEVENTTYPE_REG                = 0x00000100,   // [RFC3680]
    eEVENTTYPE_SIEMENS_RTP_STATS  = 0x00000200,   // IANA Registered without RFC
    eEVENTTYPE_SPIRITS_INDPS      = 0x00000400,   // [RFC3910]
    eEVENTTYPE_SPIRITS_USER_PROF  = 0x00000800,   // [RFC3910]
    eEVENTTYPE_WINFO              = 0x00001000,   // (template event)[RFC3857]
    eEVENTTYPE_XCAP_DIFF          = 0x00002000,   // [draft-ietf-sip-xcapevent-04]

    //M5T_INTERNAL_USE_BEGIN
    eEVENTTYPE_LAST               = 0x00004000    // Last Item
    //M5T_INTERNAL_USE_END
};

#else
MXD_OVERRIDE_TYPE_EVENT_TYPE
#endif

//M5T_INTERNAL_USE_BEGIN

// Summary:
//  Initializes the Sip Event Types.
//-----------------------------------
mxt_result InitializeSipEventTypes();

// Summary:
//  Finalizes the Sip Event Types.
//---------------------------------
void FinalizeSipEventTypes();

extern const char* const g_aszSIPEVENTTYPE[];
extern CVector<CString>* g_pvecstrSIPEVENTTYPE;

//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SIPEVENTTYPES_H

