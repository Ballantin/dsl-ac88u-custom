//==============================================================================
//==============================================================================
//
//     Copyright(c) 2007 Media5 Corporation. ("Media5")
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
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_SCECORECFG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SCECORECFG_H
//M5T_INTERNAL_USE_END

// If the compiler complains that it cannot include the file below, it may be
// because:
//
//     1 : You have not created this file to configure your application for
//         using this package. The documentation included with this package
//         explains how the configuration of the various M5T products works.
//         Please refer to this documentation and create "PreSceCoreCfg.h".
//
//     2 : You have created "PreSceCoreCfg.h" but the compiler cannot
//         find it. This file must be located in a directory found within the
//         include path defined for this build. Note that other M5T header files
//         are included by specifying the package name and filename (e.g.:
//         "SipCore/ISipContext.h"), which permits setting the include path to
//         the "Source" directory of the package only, while "PreSceCoreCfg.h"
//         is included without any root directory.
//
//------------------------------------------------------------------------------
#include "PreSceCoreCfg.h"


MX_NAMESPACE_START(MXD_GNS)

//M5T_INTERNAL_USE_BEGIN
// The topics for these variables are defined below.
#if !defined(MXD_EXTEND_TYPE_FEATURE_TAG)
    #define MXD_EXTEND_TYPE_FEATURE_TAG
#endif

#if !defined(MXD_EXTEND_TYPE_FEATURE_TAG_STRINGS)
    #define MXD_EXTEND_TYPE_FEATURE_TAG_STRINGS
#endif
//M5T_INTERNAL_USE_END

// Below this is the documentation of the various configuration macros
// available.
//---------------------------------------------------------------------
#if 0

//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Enables the inclusion of "PostSceCoreCfg.h" right at the end of
//  SceCoreCfg.h.
//
// Description:
//  Enables the inclusion of "PostSceCoreCfg.h" right at the end of
//  SceCoreCfg.h. "PostSceCoreCfg.h" is an application provided file that can
//  contain additional configuration options to possibly override the
//  configuration found in PreSceCoreCfg.h and SceCoreCfg.h.
//
// Location:
//  Define this in PreSceCoreCfg.h or in your makefile.
//
// See Also:
//  PreSceCoreCfg.h
//
//==============================================================================
#define MXD_POST_SCECORECFG


//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Overrides the ESipOptionTag type defined in "SipTypes/SipOptionTags.h".
//
// Description:
//  Overrides the ESipOptionTag type defined in "SipTypes/SipOptionTags.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove option tags.
//
//  The following must be considered when overriding this type:
//
//    - eOPTIONTAG_FIRST must always be present and share the same value as
//      the first item.
//    - Except for eOPTIONTAG_FIRST that must hold the same value as the first
//      item, it is important that each value of this type is mapped to a
//      single bit.
//    - It is important that the bit position associated with each item
//      corresponds to a single bit shift left of the previous item.
//    - eOPTIONTAG_LAST must always be present and have its own bit value,
//      which is a single bit shift left of the previous item.
//    - Inserted items must be positionned in alphabetical order within the
//      existing list of items. This means the bit position of all following
//      items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The SIP Client Engine also holds a list of strings corresponding to each
//      option tag. This enum and the list of string must always be
//      synchronized, that is, the corresponding string must be located at the
//      same position as its corresponding enum. When overriding this type, the
//      user must also override the list of strings with
//      MXD_OVERRIDE_TYPE_OPTION_TAG_STRINGS.
//    - When M5T provides updates to the SIP Client Engine, the user is
//      responsible to synchronize the updated type found in
//      "SipTypes/SipOptionTags.h" with its own version of this type,
//      including the corresponding strings.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
//------------------------------------------------------------------------------
#define MXD_OVERRIDE_TYPE_OPTION_TAG enum ESipOptionTag {...};

//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Allows the user to update the option-tag strings corresponding to the
//  MXD_OVERRIDE_TYPE_OPTION_TAG enumeration.
//
// Description:
//  This macro MUST be defined when MXD_OVERRIDE_TYPE_OPTION_TAG is defined. It
//  allows the user to update the option-tag strings corresponding to the
//  updated enumeration.
//
//  The option-tags must always be placed in alphabetical order within that
//  array. Furthermore, their order must correspond to the order of the
//  ESipOptionTag enumeration.
//
//  When you define this, you must copy the definition of g_aszSIPOPTIONTAG
//  defined in SipTypes/SipOptionTags.cpp and embed it into the definition of
//  MXD_OVERRIDE_TYPE_OPTION_TAG_STRINGS.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
//------------------------------------------------------------------------------
#define MXD_OVERRIDE_TYPE_OPTION_TAG_STRINGS \
      const char* const g_aszSIPOPTIONTAG[] = {...};

//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Overrides the ERequestDispositionDirective type defined in
//  "SipTypes/SipRequestDispositionDirective.h".
//
// Description:
//  Overrides the ERequestDispositionDirective type defined in
//  "SipTypes/SipRequestDispositionDirective.h". To do this, the current
//  definition of the type must be copied and embedded within this define, and
//  then modified to add or remove request disposition directives.
//
//  The following must be considered when overriding this type:
//
//    - eDIRECTIVE_FIRST must always be present and share the same value as
//      the first item.
//    - Except for eDIRECTIVE_FIRST that must hold the same value as the first
//      item, it is important that each value of this type is mapped to a
//      single bit.
//    - It is important that the bit position associated with each item
//      corresponds to a single bit shift left of the previous item.
//    - eDIRECTIVE_LAST must always be present and have its own bit value,
//      which is a single bit shift left of the previous item.
//    - Inserted items should be placed at the end of the enumeration just
//      before eDIRECTIVE_LAST. This means that the bit position of all following
//      items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The SIP Client Engine also holds a list of strings corresponding to each
//      request disposition directive. This enum and the list of strings must
//      always be synchronized, that is, the corresponding string must be located
//      at the same position as its corresponding enum. When overriding this
//      type, the user must also override the list of strings with
//      MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE_STRINGS.
//    - When M5T provides updates to the SIP Client Engine, the user is
//      responsible to synchronize the updated type found in
//      "SipTypes/SipRequestDispositionDirective.h" with its own version of this
//      type, including the corresponding strings.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
//------------------------------------------------------------------------------
#define MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE \
      enum ERequestDispositionDirective {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Allows the user to update the request disposition directive strings
//  corresponding to the MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE
//  enumeration.
//
// Description:
//  This macro MUST be defined when MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE
//  is defined. It allows the user to update the request disposition directive strings
//  corresponding to the updated enumeration.
//
//  The request disposition directives must always be placed in the same order
//  as the order of the ERequestDispositionDirective enumeration.
//
//  When you define this, you must copy the definition of g_aszSIP_REQUEST_DISPOSITION_DIRECTIVE
//  defined in "SipTypes/SipRequestDispositionDirective.cpp" and embed it into the
//  definition of MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE_STRINGS.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE_STRINGS \
      const char* const g_aszSIP_REQUEST_DISPOSITION_DIRECTIVE[] = {...};

//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Extends the ESipFeatureTag type defined in "SipTypes/SipFeatureTags.h".
//
// Description:
//  Extends the ESipFeatureTag type defined in "SipTypes/SipFeatureTags.h". You
//  only need to add your extension tags in the define using the enumeration
//  syntax (i.e. comma-separated list). Remember to also define
//  MXD_EXTEND_TYPE_FEATURE_TAG_STRINGS.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
//------------------------------------------------------------------------------
#define MXD_EXTEND_TYPE_FEATURE_TAG \
      eSIPFEATURETAG_YOUREXTENSION_1, eSIPFEATURETAG_YOUREXTENSION_2, ...,


//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Allows the user to extend the feature tag strings corresponding to the
//  MXD_EXTEND_TYPE_FEATURE_TAG enumeration.
//
// Description:
//  This macro MUST be defined when MXD_EXTEND_TYPE_FEATURE_TAG is defined. It
//  allows the user to extend the feature tag strings corresponding to the
//  extended enumeration.
//
//  The feature tags extension strings order must correspond to the order of the
//  ESipFeatureTag extension enumeration values order.
//
//  The feature tags strings used in this table MUST use the exact syntax that
//  will be used in SIP packets. That is if the string is not registered as not
//  needing it, the "+" prefix MUST appear in the string. See RFC 3840 for
//  prefixing rules.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
// See Also:
//  MXD_EXTEND_TYPE_FEATURE_TAG
//------------------------------------------------------------------------------
#define MXD_EXTEND_TYPE_FEATURE_TAG_STRINGS \
      "+sip.yourextension1", "+sip.yourextension2", "...",


//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Overrides the ESipMediaType type defined in "SipTypes/SipMediaTypes.h"
//
// Description:
//  Overrides the ESipMediaType type defined in "SipTypes/SipMediaTypes.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove media types.
//
//  The following must be considered when overriding this type:
//
//    - eMEDIATYPE_FIRST must always be present and share the same value as
//      the first item.
//    - Except for eMEDIATYPE_FIRST that must hold the same value as the first
//      item, it is important that each value of this type is mapped to a
//      single bit.
//    - It is important that the bit position associated with each item
//      corresponds to a single bit shift left of the previous item.
//    - eMEDIATYPE_LAST must always be present and have its own bit value,
//      which is a single bit shift left of the previous item. Note that
//      eMEDIATYPE_LAST value must not go beyond an unsigned int.
//    - Inserted items must be positioned in alphabetical order within the
//      existing list of items. This means the bit position of all following
//      items must be updated. See the example where all xyz_NEWTYPE were placed
//      within the enum.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The SIP Client Engine also holds a list of strings corresponding to each
//      media type. This enum and the list of strings must always be
//      synchronized, that is, the corresponding string must be located at the
//      same position as its corresponding enum. When overriding this type, the
//      user must also override the list of strings with
//      MXD_OVERRIDE_TYPE_MEDIA_TYPE_STRINGS.
//    - When M5T provides updates to the SIP Client Engine, the user is
//      responsible to synchronize the updated type found in
//      "SipTypes/SipMediaTypes.h" with its own version of this type,
//      including the corresponding strings.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
//------------------------------------------------------------------------------
#define MXD_OVERRIDE_TYPE_MEDIA_TYPE enum ESipMediaType {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Allows the user to update the media type strings corresponding to the
//  MXD_OVERRIDE_TYPE_MEDIA_TYPE enumeration.
//
// Description:
//  This macro MUST be defined when MXD_OVERRIDE_TYPE_MEDIA_TYPE is defined. It
//  allows the user to update the media type strings corresponding to the
//  updated enumeration.
//
//  The media types must always be placed in alphabetical order within that
//  array. Furthermore, their order must correspond to the order of the
//  ESipMediaType enumeration.
//
//  When you define this, you must copy the definition of g_astSIPMEDIATYPE
//  found in SipTypes/SipMediaTypes.cpp and embed it into the definition of
//  MXD_OVERRIDE_TYPE_MEDIA_TYPE_STRINGS.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
//------------------------------------------------------------------------------
#define MXD_OVERRIDE_TYPE_MEDIA_TYPE_STRINGS \
      const SSipMediaType g_astSIPMEDIATYPE[] = {...};

//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Overrides the ESipMethodType type defined in "SipTypes/SipMethodTypes.h".
//
// Description:
//  Overrides the ESipMethodType type defined in "SipTypes/SipMethodTypes.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove media types.
//
//  The following must be considered when overriding this type:
//
//    - eMETHODTYPE_FIRST must always be present and share the same value as
//      the first item.
//    - Except for eMETHODTYPE_FIRST that must hold the same value as the first
//      item, it is important that each value of this type is mapped to a
//      single bit.
//    - It is important that the bit position associated with each item
//      corresponds to a single bit shift left of the previous item.
//    - eMETHODTYPE_LAST must always be present and have its own bit value,
//      which is a single bit shift left of the previous item.
//    - Inserted items must be positioned in alphabetical order within the
//      existing list of items. This means the bit position of all following
//      items must be updated. See the example where eMETHODTYPE_NEWMETHOD is
//      placed within the enum.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The SIP Client Engine also holds a list of strings corresponding to each
//      media type. This enum and the list of strings must always be
//      synchronized, that is, the corresponding string must be located at the
//      same position as its corresponding enum. When overriding this type, the
//      user must also override the list of strings with
//      MXD_OVERRIDE_TYPE_METHOD_TYPE_STRINGS.
//    - When M5T provides updates to the SIP Client Engine, the user is
//      responsible to synchronize the updated type found in
//      "SipTypes/SipMethodTypes.h" with its own version of this type,
//      including the corresponding strings.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
//------------------------------------------------------------------------------
#define MXD_OVERRIDE_TYPE_METHOD_TYPE enum ESipMethodType {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Allows the user to update the media type strings corresponding to the
//  MXD_OVERRIDE_TYPE_METHOD_TYPE enumeration.
//
// Description:
//  This macro MUST be defined when MXD_OVERRIDE_TYPE_METHOD_TYPE is defined. It
//  allows the user to update the media type strings corresponding to the
//  updated enumeration.
//
//  The method types must always be placed in alphabetical order within that
//  array. Furthermore, their order must correspond to the order of the
//  ESipMethodType enumeration.
//
//  When you define this, you must copy the definition of g_aszSIPMETHODTYPE
//  found in SipTypes/SipMethodTypes.cpp and embed it into the definition of
//  MXD_OVERRIDE_TYPE_METHOD_TYPE_STRINGS.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
//------------------------------------------------------------------------------
#define MXD_OVERRIDE_TYPE_METHOD_TYPE_STRINGS \
      const char* const g_aszSIPMETHODTYPE[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Overrides the ESipEventType type defined in "SipTypes/SipEventTypes.h".
//
// Description:
//  Overrides the ESipEventType type defined in "SipTypes/SipEventTypes.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove event types.
//
//  The following must be considered when overriding this type:
//
//    - eEVENTTYPE_FIRST must always be present and share the same value as
//      the first item.
//    - Except for eEVENTTYPE_FIRST that must hold the same value as the first
//      item, it is important that each value of this type is mapped to a
//      single bit.
//    - It is important that the bit position associated with each item
//      corresponds to a single bit shift left of the previous item.
//    - eEVENTTYPE_LAST must always be present and have its own bit value,
//      which is a single bit shift left of the previous item.
//    - Inserted items must be positioned in alphabetical order within the
//      existing list of items. This means the bit position of all following
//      items must be updated. See the example where all eEVENTTYPE_NEWEVENT
//      were placed within the enum.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The SIP Client Engine also holds a list of strings corresponding to each
//      event type. This enum and the list of strings must always be
//      synchronized, that is, the corresponding string must be located at the
//      same position as its corresponding enum. When overriding this type, the
//      user must also override the list of strings with
//      MXD_OVERRIDE_TYPE_EVENT_TYPE_STRINGS.
//    - When M5T provides updates to the SIP Client Engine, the user is
//      responsible to synchronize the updated type found in
//      "SipTypes/SipEventTypes.h" with its own version of this type,
//      including the corresponding strings.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
//------------------------------------------------------------------------------
#define MXD_OVERRIDE_TYPE_EVENT_TYPE enum ESipEventType {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Allows the user to update the event type strings corresponding to the
//  MXD_OVERRIDE_TYPE_EVENT_TYPE enumeration.
//
// Description:
//  This macro MUST be defined when MXD_OVERRIDE_TYPE_EVENT_TYPE is defined. It
//  allows the user to update the event type strings corresponding to the
//  updated enumeration.
//
//  The event types must always be placed in alphabetical order within that
//  array. Furthermore, their order must correspond to the order of the
//  ESipEventType enumeration.
//
//  When you define this, you must copy the definition of g_aszSIPEVENTTYPE
//  found in SipTypes/SipEventTypes.cpp and embed it into the definition of
//  MXD_OVERRIDE_TYPE_EVENT_TYPE_STRINGS.
//
// Location:
//  Define this in PreSceCoreCfg.h.
//
// #Note:#
//  For an example on how to override this define, please refer to the Advanced
//  Topic section of the M5T SIP Client Engine v2.0 – Programmer’s Guide.
//
//------------------------------------------------------------------------------
#define MXD_OVERRIDE_TYPE_EVENT_TYPE_STRINGS \
      const char* const g_aszSIPEVENTTYPE[] = {...};

#endif // #if 0 for documentation.

MX_NAMESPACE_END(MXD_GNS)


#if defined(MXD_POST_SCECORECFG)
#include "PostSceCoreCfg.h"
#endif


#endif // #ifndef MXG_SCECORECFG_H


