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
#ifndef MXG_SCETYPESCONVERTER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SCETYPESCONVERTER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_METHOD_H
#include "SipParser/Method.h"
#endif

#ifndef MXG_SIPEVENTTYPES_H
#include "SceSipTypes/SipEventTypes.h"
#endif

#ifndef MXG_SIPMEDIATYPES_H
#include "SceSipTypes/SipMediaTypes.h"
#endif

#ifndef MXG_SIPMETHODTYPES_H
#include "SceSipTypes/SipMethodTypes.h"
#endif

#ifndef MXG_SIPOPTIONTAGS_H
#include "SceSipTypes/SipOptionTags.h"
#endif

#ifndef MXG_SIPFEATURETAGS_H
#include "SceSipTypes/SipFeatureTags.h"
#endif

#ifndef MXG_SIPREQUESTDISPOSITIONDIRECTIVES_H
#include "SceSipTypes/SipRequestDispositionDirective.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
//-------------------------


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//====  PUBLIC METHODS  ========================================================
//==============================================================================

// Summary:
//  Gets the enum corresponding to the string passed in parameter.
//-----------------------------------------------------------------
ESipOptionTag GetOptionTag(IN const char* szTag);

// Summary:
//  Gets the string corresponding to the enum passed in parameter.
//-----------------------------------------------------------------
const CString& GetOptionTag(IN ESipOptionTag eTag);

// Summary:
//  Gets the enum corresponding to the string passed in parameter.
//-----------------------------------------------------------------
ESipEventType GetEventType(IN const char* szEvent);

// Summary:
//  Gets the string corresponding to the enum passed in parameter.
//-----------------------------------------------------------------
const CString& GetEventType(IN ESipEventType eEvent);

// Summary:
//  Gets the enum corresponding to the media descriptors passed in parameter.
//----------------------------------------------------------------------------
ESipMediaType GetMediaType(IN const char* szMediaMType,
                           IN const char* szMediaMSubType);

// Summary:
//  Gets the "m-type" string corresponding to the enum passed in parameter.
//--------------------------------------------------------------------------
const CString& GetMediaMType(IN ESipMediaType eMedia);

// Summary:
//  Gets the "m-subtype" string corresponding to the enum passed in parameter.
//-----------------------------------------------------------------------------
const CString& GetMediaMSubType(IN ESipMediaType eMedia);

// Summary:
//  Gets the enum corresponding to the string passed in parameter.
//-----------------------------------------------------------------
ESipMethodType GetMethodType(IN const char* szMethod);

// Summary:
//  Gets the string corresponding to the enum passed in parameter.
//-----------------------------------------------------------------
const CString& GetMethodType(IN ESipMethodType eMethod);

// Summary:
//  Gets the Client Engine method corresponding to the stack method.
//-------------------------------------------------------------------
ESipMethodType ConvertMethodType(IN ESipMethod eMethod);

// Summary:
//  Gets the stack method corresponding to the Client Engine method.
//-------------------------------------------------------------------
ESipMethod ConvertMethodType(IN ESipMethodType eMethodType);

// Summary:
//  Returns the feature tag enum corresponding to szTag.
ESipFeatureTag GetFeatureTag(IN const char* szTag);

// Summary:
//  Returns the feature tag as a null-terminated string corresponding to
//  eTag.
const CString& GetFeatureTag(IN ESipFeatureTag eTag);

// Summary:
//  Returns the request disposition directive enum corresponding to szDirective.
ERequestDispositionDirective GetRequestDispositionDirective(IN const char* szDirective);

// Summary:
//  Returns the request disposition directive as a null-terminated string
//  corresponding to eDirective.
const CString& GetRequestDispositionDirective(IN ERequestDispositionDirective eDirective);


//-- Internal functions.

//M5T_INTERNAL_USE_BEGIN

// Summary:
//  Returns a linear value (used as an index) from a bitmask value in which
//  there is only one bit set.
unsigned int GetLinearEnumFromBitmaskInt(IN int nEnumBitmask);

// Summary:
//  Returns a linear value (used as an index) from a bitmask value in which
//  there is only one bit set.
unsigned int GetLinearEnumFromBitmask32(IN uint32_t uEnumBitmask);

// Summary:
//  Returns a linear value (used as an index) from a bitmask value in which
//  there is only one bit set.
unsigned int GetLinearEnumFromBitmask64(IN const uint64_t& ruEnumBitmask);

//M5T_INTERNAL_USE_END


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  INTERNAL METHODS  ======================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN

//==============================================================================
//==
//==  GetLinearEnumFromBitmaskInt
//==
//==============================================================================
//
//  Parameters:
//    uEnumBitmask:
//      The enum value (a bitmask of one bit).
//
//  Returns:
//      The linear enum value corresponding to uEnumBitmask.
//
//  Description:
//      Gets the linear enum value from a bitmask enum value.
//      Internal function ONLY.
//
//==============================================================================
inline unsigned int GetLinearEnumFromBitmaskInt(IN int nEnumBitmask)
{
    return GetLinearEnumFromBitmask32(static_cast<unsigned int>(nEnumBitmask));
}

//M5T_INTERNAL_USE_END


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SCETYPESCONVERTER_H

