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
#ifndef MXG_SIPMETHODTYPES_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SIPMETHODTYPES_H
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
#include "Config/SceCoreCfg.h" // MXD_OVERRIDE_TYPE_METHOD_TYPE
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

#if !defined(MXD_OVERRIDE_TYPE_METHOD_TYPE) && defined(MXD_OVERRIDE_TYPE_METHOD_TYPE_STRINGS)
#error Must synchronize type and strings.
#endif

#if defined(MXD_OVERRIDE_TYPE_METHOD_TYPE) && !defined(MXD_OVERRIDE_TYPE_METHOD_TYPE_STRINGS)
#error Must synchronize type and strings.
#endif


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if !defined (MXD_OVERRIDE_TYPE_METHOD_TYPE)

//<GROUP TID_PACKAGE_SCECORE_STRUCTSENUMS>
// Summary:
//  Defines all known SIP methods.
//
// Description:
//  This type defines all known SIP methods.
// 
//  The definitions of ESipMethod in the M5T SIP stack are not used directly at
//  the Client Engine level as the values of ESipMethod are not defined as
//  bitsets.
// 
//  ESipMethod defines its types as eSIP_METHOD_XYZ while ESipMethodType defines
//  its types as eMETHODTYPE_XYZ.
// 
//  This type must be ordered alphabetically without regards to case.
// 
enum ESipMethodType
{

    //*********
    // WARNING:
    // ********* 
    // When updating this table, you may also have to update the PIDF values
    // associated with this event. See uPIDFSERVCAPSMETHOD__XXX in CPidfData.h.


    //M5T_INTERNAL_USE_BEGIN
    eMETHODTYPE_FIRST           = 0x00000001,  // First item.
    //M5T_INTERNAL_USE_END

    eMETHODTYPE_ACK             = 0x00000001,  // RFC 3261 (SIP).
    eMETHODTYPE_BYE             = 0x00000002,  // RFC 3261 (SIP).
    eMETHODTYPE_CANCEL          = 0x00000004,  // RFC 3261 (SIP).
    eMETHODTYPE_INFO            = 0x00000008,  // RFC 2976 (The SIP INFO Method).
    eMETHODTYPE_INVITE          = 0x00000010,  // RFC 3261 (SIP).
    eMETHODTYPE_MESSAGE         = 0x00000020,  // RFC 3428 (SIP Extension for Instant Messaging).
    eMETHODTYPE_NOTIFY          = 0x00000040,  // RFC 3265 (SIP-Specific Event Notification).
    eMETHODTYPE_OPTIONS         = 0x00000080,  // RFC 3261 (SIP).
    eMETHODTYPE_PING            = 0x00000100,  // draft-sen-midcom-fw-nat-02.txt. This draft is currently outdated and has been removed from the IETF site.
    eMETHODTYPE_PRACK           = 0x00000200,  // RFC 3262 (PRACK).
    eMETHODTYPE_PUBLISH         = 0x00000400,  // RFC 3903 (Session Initiation Protocol (SIP) Extension for Event State Publication)
    eMETHODTYPE_REFER           = 0x00000800,  // RFC 3515 (The SIP Refer Method).
    eMETHODTYPE_REGISTER        = 0x00001000,  // RFC 3261 (SIP).
    eMETHODTYPE_SERVICE         = 0x00002000,  // draft-deason-sip-soap-00.txt. This draft is currently outdated and has been removed from the IETF site.
    eMETHODTYPE_SUBSCRIBE       = 0x00004000,  // RFC 3265 (SIP-Specific Event Notification).
    eMETHODTYPE_UPDATE          = 0x00008000,  // RFC 3311 (SIP UPDATE Method).

    //M5T_INTERNAL_USE_BEGIN
    eMETHODTYPE_LAST            = 0x00010000   // Last Item
    //M5T_INTERNAL_USE_END
};

#else
MXD_OVERRIDE_TYPE_METHOD_TYPE
#endif

//M5T_INTERNAL_USE_BEGIN

// Summary:
//  Initializes the Sip Method Types.
//------------------------------------
mxt_result InitializeSipMethodTypes();

// Summary:
//  Finalizes the Sip Method Types.
//----------------------------------
void FinalizeSipMethodTypes();

extern const char* const g_aszSIPMETHODTYPE[];
extern CVector<CString>* g_pvecstrSIPMETHODTYPE;

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SIPMETHODTYPES_H

