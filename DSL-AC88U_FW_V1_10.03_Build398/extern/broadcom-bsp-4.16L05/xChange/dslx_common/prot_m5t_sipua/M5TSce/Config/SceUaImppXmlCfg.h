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
#ifndef MXG_SCEUAIMPPXMLCFG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SCEUAIMPPXMLCFG_H
//M5T_INTERNAL_USE_END

// If the compiler complains that it cannot include the file below, it may be
// because:
//
//     1 : You have not created this file to configure your application for
//         using this package. The documentation included with this package
//         explains how the configuration of the various M5T products works.
//         Please refer to this documentation and create "PreSceUaImppXmlCfg.h".
//
//     2 : You have created "PreSceUaImppXmlCfg.h" but the compiler cannot
//         find it. This file must be located in a directory found within the
//         include path defined for this build. Note that other M5T header files
//         are included by specifying the package name and filename (e.g.:
//         "SipCore/ISipContext.h"), which permits setting the include path to
//         the "Source" directory of the package only, while
//         "PreSceUaImppXmlCfg.h" is included without any root directory.
//
//------------------------------------------------------------------------------
#include "PreSceUaImppXmlCfg.h"

MX_NAMESPACE_START(MXD_GNS)

// Below this is the documentation of the various configuration macros
// available.
#if 0 // for documentation

//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Enables the inclusion of "PostSceUaImppXmlCfg.h" right at the end of
//  SceUaImppXmlCfg.h.
//
// Description:
//  Enables the inclusion of "PostSceUaImppXmlCfg.h" right at the end of
//  SceUaImppXmlCfg.h. "PostSceUaImppXmlCfg.h" is an application provided file
//  that can contain additional configuration options to possibly override the
//  configuration found in PreSceUaImppXmlCfg.h and SceUaImppXmlCfg.h.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h or in your makefile.
//
// See Also:
//  PreSceUaImppXmlCfg.h
//
//==============================================================================
#define MXD_POST_SCEUAIMPPXMLCFG


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the EPidfNamespace type defined in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the EPidfNamespace type defined in "SceUaImppXml/PidfData.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove values.
//
//  The following must be considered when overriding this type:
//
//    - This is a linear enum: the first enum value must be 0 and each following
//      value must be its previous value plus 1.
//    - ePIDFNS_OTHER_VALUE must be the last value of this enum and must equal
//      the last enum's value plus 1.
//    - UA-IMPP also holds a list of SPidfData corresponding to each enum value.
//      This enum and the list of SPidfData must always be synchronized, that
//      is, the corresponding SPidfData must be located at the same position as
//      its corresponding enum. When overriding this type, the user must also
//      override the list of SPidfData with
//      MXD_SCE_OVERRIDE_DEFAULT_NS_PREFIXES_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated type found in "SceUaImppXml/PidfData.h" with its
//      own version of this type, including the corresponding SPidfData array.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFNAMESPACE enum EPidfNamespace {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint32_t values defined for the activities elements in
//  "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint32_t values defined for the activities elements in
//  "SceUaImppXml/PidfData.h". To do this, the current definitions must be
//  copied and embedded within this define, and then modified to add or remove
//  values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFACT_LAST_VALUE must be defined and equal to last pidf activities
//      element's value constant shifted left by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint32_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint32_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFACT_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint32_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFACT \
    const uint32_t uPIDFACT_CUSTOM1 = 1; \
    const uint32_t uPIDFACT_CUSTOM2 = uPIDFACT_CUSTOM1 << 1; \
    ... \
    const uint32_t uPIDFACT_LAST_VALUE = uPIDFACT_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the activities SPidfData array corresponding to
//  the MXD_SCE_OVERRIDE_PIDFACT values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFACT is defined. It
//  allows the user to update the activities SPidfData array corresponding to
//  the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint32_t values.
//
//  When you define this, you must copy the definition of g_stPIDFACTIVITIES
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFACT_SPIDFDATA.
//
//  uPIDFACT_LAST_VALUE MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFACT_SPIDFDATA \
      const SPidfData g_stPIDFACTIVITIES[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint64_t values defined for the mood elements in
//  "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint64_t values defined for the mood elements in
//  "SceUaImppXml/PidfData.h". To do this, the current definitions must be
//  copied and embedded within this define, and then modified to add or remove
//  values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFMOOD_LAST_VALUE must be defined and equal to last pidf mood
//      element's value constant shifted left by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint64_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint64_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFMOOD_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint64_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFMOOD \
    const uint64_t uPIDFMOOD_CUSTOM1 = 1; \
    const uint64_t uPIDFMOOD_CUSTOM2 = uPIDFMOOD_CUSTOM1 << 1; \
    ... \
    const uint64_t uPIDFMOOD_LAST_VALUE = uPIDFMOOD_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the mood SPidfData array corresponding to the
//  MXD_SCE_OVERRIDE_PIDFMOOD values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFMOOD is defined. It
//  allows the user to update the mood SPidfData array corresponding to
//  the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint64_t values.
//
//  When you define this, you must copy the definition of g_stPIDFMOOD
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFMOOD_SPIDFDATA.
//
//  uPIDFMOOD_LAST_VALUE MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFMOOD_SPIDFDATA \
      const SPidfData g_stPIDFMOOD[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the EPidfPlaceIsAudio type defined in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the EPidfPlaceIsAudio type defined in "SceUaImppXml/PidfData.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove values.
//
//  The following must be considered when overriding this type:
//
//    - This type must be ordered alphabetically with regards to case.
//    - This is a linear enum: the first enum value must be 0 and each following
//      value must be its previous value plus 1.
//    - ePIDFAUDIO_OTHER_NAME must be the last value of this enum and must
//      equal the last enum's value plus 1.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the enumeration
//      values following the added value must be updated.
//    - When removing an item, the items following the removed item must all
//      have their value updated accordingly.
//    - UA-IMPP also holds an array of SPidfData corresponding to each enum
//      value. This enum and the SPidfData array must always be synchronized,
//      that is, the corresponding SPidfData must be located at the same
//      position as its corresponding enum (i.e.: it can be accessed in the
//      array with the enum value). When overriding this type, the user must
//      also override the SPidfData array with
//      MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated type found in "SceUaImppXml/PidfData.h" with its
//      own version of this type, including the corresponding SPidfData array.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO enum EPidfPlaceIsAudio {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the place-is audio SPidfData array corresponding
//  to the MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO enumeration.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO is
//  defined. It allows the user to update the place-is audio SPidfData array
//  corresponding to the updated enumeration.
//
//  The place-is audio element names (m_pszName member of SPidfData) must always
//  be placed in alphabetical order within that array. Furthermore, their order
//  must correspond to the order of the EPidfPlaceIsAudio enumeration.
//
//  When you define this, you must copy the definition of g_stPIDFPLACEISAUDIO
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO_SPIDFDATA.
//
//  ePIDFAUDIO_OTHER_NAME MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO_SPIDFDATA \
      const SPidfData g_stPIDFPLACEISAUDIO[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the EPidfPlaceIsVideo type defined in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the EPidfPlaceIsVideo type defined in "SceUaImppXml/PidfData.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove values.
//
//  The following must be considered when overriding this type:
//
//    - This type must be ordered alphabetically with regards to case.
//    - This is a linear enum: the first enum value must be 0 and each following
//      value must be its previous value plus 1.
//    - ePIDFVIDEO_OTHER_NAME must be the last value of this enum and must
//      equal the last enum's value plus 1.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the enumeration
//      values following the added value must be updated.
//    - When removing an item, the items following the removed item must all
//      have their value updated accordingly.
//    - UA-IMPP also holds an array of SPidfData corresponding to each enum
//      value. This enum and the SPidfData array must always be synchronized,
//      that is, the corresponding SPidfData must be located at the same
//      position as its corresponding enum (i.e.: it can be accessed in the
//      array with the enum value). When overriding this type, the user must
//      also override the SPidfData array with
//      MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated type found in "SceUaImppXml/PidfData.h" with its
//      own version of this type, including the corresponding SPidfData array.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO enum EPidfPlaceIsVideo {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the place-is video SPidfData array corresponding
//  to the MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO enumeration.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO is
//  defined. It allows the user to update the place-is video SPidfData array
//  corresponding to the updated enumeration.
//
//  The place-is video element names (m_pszName member of SPidfData) must always
//  be placed in alphabetical order within that array. Furthermore, their order
//  must correspond to the order of the EPidfPlaceIsVideo enumeration.
//
//  When you define this, you must copy the definition of g_stPIDFPLACEISVIDEO
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO_SPIDFDATA.
//
//  ePIDFVIDEO_OTHER_NAME MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO_SPIDFDATA \
      const SPidfData g_stPIDFPLACEISVIDEO[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the EPidfPlaceIsText type defined in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the EPidfPlaceIsText type defined in "SceUaImppXml/PidfData.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove values.
//
//  The following must be considered when overriding this type:
//
//    - This type must be ordered alphabetically with regards to case.
//    - This is a linear enum: the first enum value must be 0 and each following
//      value must be its previous value plus 1.
//    - ePIDFTEXT_OTHER_NAME must be the last value of this enum and must
//      equal the last enum's value plus 1.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the enumeration
//      values following the added value must be updated.
//    - When removing an item, the items following the removed item must all
//      have their value updated accordingly.
//    - UA-IMPP also holds an array of SPidfData corresponding to each enum
//      value. This enum and the SPidfData array must always be synchronized,
//      that is, the corresponding SPidfData must be located at the same
//      position as its corresponding enum (i.e.: it can be accessed in the
//      array with the enum value). When overriding this type, the user must
//      also override the SPidfData array with
//      MXD_SCE_OVERRIDE_PIDFPLACEISTEXT_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated type found in "SceUaImppXml/PidfData.h" with its
//      own version of this type, including the corresponding SPidfData array.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFPLACEISTEXT enum EPidfPlaceIsText {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the place-is text SPidfData array corresponding
//  to the MXD_SCE_OVERRIDE_PIDFPLACEISTEXT enumeration.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFPLACEISTEXT is
//  defined. It allows the user to update the place-is text SPidfData array
//  corresponding to the updated enumeration.
//
//  The place-is text element names (m_pszName member of SPidfData) must always
//  be placed in alphabetical order within that array. Furthermore, their order
//  must correspond to the order of the EPidfPlaceIsText enumeration.
//
//  When you define this, you must copy the definition of g_stPIDFPLACEISTEXT
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFPLACEISTEXT_SPIDFDATA.
//
//  ePIDFTEXT_OTHER_NAME MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFPLACEISTEXT_SPIDFDATA \
      const SPidfData g_stPIDFPLACEISTEXT[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint64_t values defined for the place-type elements in
//  "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint64_t values defined for the place-type elements in
//  "SceUaImppXml/PidfData.h". To do this, the current definitions must be
//  copied and embedded within this define, and then modified to add or remove
//  values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFPLACETYPE_LAST_VALUE must be defined and equal to last pidf
//      place-type element's value constant shifted left by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint64_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint64_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFPLACETYPE_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint64_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFPLACETYPE \
    const uint64_t uPIDFPLACETYPE_CUSTOM1 = 1; \
    const uint64_t uPIDFPLACETYPE_CUSTOM2 = uPIDFPLACETYPE_CUSTOM1 << 1; \
    ... \
    const uint64_t uPIDFPLACETYPE_LAST_VALUE = uPIDFPLACETYPE_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the place-type SPidfData array corresponding to
//  the MXD_SCE_OVERRIDE_PIDFPLACETYPE values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFPLACETYPE is defined. It
//  allows the user to update the place-type SPidfData array corresponding to
//  the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint64_t values.
//
//  When you define this, you must copy the definition of g_stPIDFPLACETYPE
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFPLACETYPE_SPIDFDATA.
//
//  uPIDFPLACETYPE_LAST_VALUE MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFPLACETYPE_SPIDFDATA \
      const SPidfData g_stPIDFPLACETYPE[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint32_t values defined for the privacy elements in
//  "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint32_t values defined for the privacy elements in
//  "SceUaImppXml/PidfData.h". To do this, the current definitions must be
//  copied and embedded within this define, and then modified to add or remove
//  values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFPRIVACY_LAST_VALUE must be defined and equal to last pidf privacy
//      element's value constant shifted left by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint32_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint32_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFPRIVACY_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint32_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFPRIVACY \
    const uint32_t uPIDFPRIVACY_CUSTOM1 = 1; \
    const uint32_t uPIDFPRIVACY_CUSTOM2 = uPIDFPRIVACY_CUSTOM1 << 1; \
    ... \
    const uint32_t uPIDFPRIVACY_LAST_VALUE = uPIDFPRIVACY_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the privacy SPidfData array corresponding to
//  the MXD_SCE_OVERRIDE_PIDFPRIVACY values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFPRIVACY is defined. It
//  allows the user to update the privacy SPidfData array corresponding to
//  the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint32_t values.
//
//  When you define this, you must copy the definition of g_stPIDFPRIVACY
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFPRIVACY_SPIDFDATA.
//
//  uPIDFPRIVACY_LAST_VALUE MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFPRIVACY_SPIDFDATA \
      const SPidfData g_stPIDFPRIVACY[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the EPidfRelationship type defined in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the EPidfRelationship type defined in "SceUaImppXml/PidfData.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove values.
//
//  The following must be considered when overriding this type:
//
//    - This type must be ordered alphabetically with regards to case.
//    - This is a linear enum: the first enum value must be 0 and each following
//      value must be its previous value plus 1.
//    - ePIDFRELATIONSHIP_OTHER_NAME must be the last value of this enum and must
//      equal the last enum's value plus 1.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the enumeration
//      values following the added value must be updated.
//    - When removing an item, the items following the removed item must all
//      have their value updated accordingly.
//    - UA-IMPP also holds an array of SPidfData corresponding to each enum
//      value. This enum and the SPidfData array must always be synchronized,
//      that is, the corresponding SPidfData must be located at the same
//      position as its corresponding enum (i.e.: it can be accessed in the
//      array with the enum value). When overriding this type, the user must
//      also override the SPidfData array with
//      MXD_SCE_OVERRIDE_PIDFRELATIONSHIP_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated type found in "SceUaImppXml/PidfData.h" with its
//      own version of this type, including the corresponding SPidfData array.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFRELATIONSHIP enum EPidfRelationship {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the relationship SPidfData array corresponding
//  to the MXD_SCE_OVERRIDE_PIDFRELATIONSHIP enumeration.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFRELATIONSHIP is
//  defined. It allows the user to update the relationship SPidfData array
//  corresponding to the updated enumeration.
//
//  The relationship element names (m_pszName member of SPidfData) must always
//  be placed in alphabetical order within that array. Furthermore, their order
//  must correspond to the order of the EPidfRelationship enumeration.
//
//  When you define this, you must copy the definition of g_stPIDFRELATIONSHIP
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFRELATIONSHIP_SPIDFDATA.
//
//  ePIDFRELATIONSHIP_OTHER_NAME MUST have an invalid corresponding value
//  (e.g. : "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFRELATIONSHIP_SPIDFDATA \
      const SPidfData g_stPIDFRELATIONSHIP[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the EPidfServiceClass type defined in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the EPidfServiceClass type defined in "SceUaImppXml/PidfData.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove values.
//
//  The following must be considered when overriding this type:
//
//    - This type must be ordered alphabetically with regards to case.
//    - This is a linear enum: the first enum value must be 0 and each following
//      value must be its previous value plus 1.
//    - ePIDFSERVICECLASS_OTHER_NAME must be the last value of this enum and must
//      equal the last enum's value plus 1.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the enumeration
//      values following the added value must be updated.
//    - When removing an item, the items following the removed item must all
//      have their value updated accordingly.
//    - UA-IMPP also holds an array of SPidfData corresponding to each enum
//      value. This enum and the SPidfData array must always be synchronized,
//      that is, the corresponding SPidfData must be located at the same
//      position as its corresponding enum (i.e.: it can be accessed in the
//      array with the enum value). When overriding this type, the user must
//      also override the SPidfData array with
//      MXD_SCE_OVERRIDE_PIDFSERVICECLASS_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated type found in "SceUaImppXml/PidfData.h" with its
//      own version of this type, including the corresponding SPidfData array.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVICECLASS enum EPidfServiceClass {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the service-class SPidfData array corresponding
//  to the MXD_SCE_OVERRIDE_PIDFSERVICECLASS enumeration.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFSERVICECLASS is
//  defined. It allows the user to update the service-class SPidfData array
//  corresponding to the updated enumeration.
//
//  The service-class element names (m_pszName member of SPidfData) must always
//  be placed in alphabetical order within that array. Furthermore, their order
//  must correspond to the order of the EPidfServiceClass enumeration.
//
//  When you define this, you must copy the definition of g_stPIDFSERVICECLASS
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFSERVICECLASS_SPIDFDATA.
//
//  ePIDFSERVICECLASS_OTHER_NAME MUST have an invalid corresponding value
//  (e.g. : "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVICECLASS_SPIDFDATA \
      const SPidfData g_stPIDFSERVICECLASS[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the EPidfSphere type defined in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the EPidfSphere type defined in "SceUaImppXml/PidfData.h". To
//  do this, the current definition of the type must be copied and embedded
//  within this define, and then modified to add or remove values.
//
//  The following must be considered when overriding this type:
//
//    - This type must be ordered alphabetically with regards to case.
//    - This is a linear enum: the first enum value must be 0 and each following
//      value must be its previous value plus 1.
//    - ePIDFSPHERE_OTHER_NAME must be the last value of this enum and must
//      equal the last enum's value plus 1.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the enumeration
//      values following the added value must be updated.
//    - When removing an item, the items following the removed item must all
//      have their value updated accordingly.
//    - UA-IMPP also holds an array of SPidfData corresponding to each enum
//      value. This enum and the SPidfData array must always be synchronized,
//      that is, the corresponding SPidfData must be located at the same
//      position as its corresponding enum (i.e.: it can be accessed in the
//      array with the enum value). When overriding this type, the user must
//      also override the SPidfData array with
//      MXD_SCE_OVERRIDE_PIDFSPHERE_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated type found in "SceUaImppXml/PidfData.h" with its
//      own version of this type, including the corresponding SPidfData array.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSPHERE enum EPidfSphere {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the sphere SPidfData array corresponding to the
//  MXD_SCE_OVERRIDE_PIDFSPHERE enumeration.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFSPHERE is
//  defined. It allows the user to update the sphere SPidfData array
//  corresponding to the updated enumeration.
//
//  The sphere element names (m_pszName member of SPidfData) must always
//  be placed in alphabetical order within that array. Furthermore, their order
//  must correspond to the order of the EPidfSphere enumeration.
//
//  When you define this, you must copy the definition of g_stPIDFSPHERE
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFSPHERE_SPIDFDATA.
//
//  ePIDFSPHERE_OTHER_NAME MUST have an invalid corresponding value
//  (e.g. : "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSPHERE_SPIDFDATA \
      const SPidfData g_stPIDFSPHERE[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint32_t values defined for the servcaps duplex elements in
//  "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint32_t values defined for the servcaps duplex elements in
//  "SceUaImppXml/PidfData.h". To do this, the current definitions must be
//  copied and embedded within this define, and then modified to add or remove
//  values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFSERVCAPSDUPLEX_LAST_VALUE must be defined and equal to last pidf
//      servcaps duplex element's value constant shifted left by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint32_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint32_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint32_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX \
    const uint32_t uPIDFSERVCAPSDUPLEX_CUSTOM1 = 1; \
    const uint32_t uPIDFSERVCAPSDUPLEX_CUSTOM2 = uPIDFSERVCAPSDUPLEX_CUSTOM1 << 1; \
    ... \
    const uint32_t uPIDFSERVCAPSDUPLEX_LAST_VALUE = uPIDFSERVCAPSDUPLEX_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the servcaps duplex SPidfData array corresponding
//  to the MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX is
//  defined. It allows the user to update the servcaps duplex SPidfData array
//  corresponding to the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint32_t values.
//
//  When you define this, you must copy the definition of g_stPIDFSERVCAPSDUPLEX
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX_SPIDFDATA.
//
//  uPIDFSERVCAPSDUPLEX_LAST_VALUE MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX_SPIDFDATA \
      const SPidfData g_stPIDFSERVCAPSDUPLEX[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint32_t values defined for the servcaps class elements in
//  "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint32_t values defined for the servcaps class elements in
//  "SceUaImppXml/PidfData.h". To do this, the current definitions must be
//  copied and embedded within this define, and then modified to add or remove
//  values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFSERVCAPSCLASS_LAST_VALUE must be defined and equal to last pidf
//      servcaps class element's value constant shifted left by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint32_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint32_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint32_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS \
    const uint32_t uPIDFSERVCAPSCLASS_CUSTOM1 = 1; \
    const uint32_t uPIDFSERVCAPSCLASS_CUSTOM2 = uPIDFSERVCAPSCLASS_CUSTOM1 << 1; \
    ... \
    const uint32_t uPIDFSERVCAPSCLASS_LAST_VALUE = uPIDFSERVCAPSCLASS_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the servcaps class SPidfData array corresponding
//  to the MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS is
//  defined. It allows the user to update the servcaps class SPidfData array
//  corresponding to the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint32_t values.
//
//  When you define this, you must copy the definition of g_stPIDFSERVCAPSCLASS
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS_SPIDFDATA.
//
//  uPIDFSERVCAPSCLASS_LAST_VALUE MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS_SPIDFDATA \
      const SPidfData g_stPIDFSERVCAPSCLASS[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint32_t values defined for the servcaps event-packages
//  elements in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint32_t values defined for the servcaps event-packages
//  elements in "SceUaImppXml/PidfData.h". To do this, the current definitions
//  must be copied and embedded within this define, and then modified to add or
//  remove values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFSERVCAPSEVENTPKG_LAST_VALUE must be defined and equal to last pidf
//      servcaps event-packages element's value constant shifted left by one
//      bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint32_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint32_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint32_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//    - Modifying these values may require modifications to ESipEventType.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
// See Also:
//  MXD_OVERRIDE_TYPE_EVENT_TYPE.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG \
    const uint32_t uPIDFSERVCAPSEVENTPKG_CUSTOM1 = 1; \
    const uint32_t uPIDFSERVCAPSEVENTPKG_CUSTOM2 = uPIDFSERVCAPSEVENTPKG_CUSTOM1 << 1; \
    ... \
    const uint32_t uPIDFSERVCAPSEVENTPKG_LAST_VALUE = uPIDFSERVCAPSEVENTPKG_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the servcaps event-packages SPidfData array
//  corresponding to the MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG is
//  defined. It allows the user to update the servcaps event-packages SPidfData
//  array corresponding to the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint32_t values.
//
//  When you define this, you must copy the definition of g_stPIDFSERVCAPSEVENTPKG
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG_SPIDFDATA.
//
//  uPIDFSERVCAPSEVENTPKG_LAST_VALUE MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG_SPIDFDATA \
      const SPidfData g_stPIDFSERVCAPSEVENTPKG[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint32_t values defined for the servcaps methods elements in
//  "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint32_t values defined for the servcaps methods elements in
//  "SceUaImppXml/PidfData.h". To do this, the current definitions must be
//  copied and embedded within this define, and then modified to add or remove
//  values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFSERVCAPSMETHOD_LAST_VALUE must be defined and equal to last pidf
//      servcaps methods element's value constant shifted left by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint32_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint32_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint32_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//    - Modifying these values may require modifications to ESipMethodType.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
// See Also:
//  MXD_OVERRIDE_TYPE_METHOD_TYPE.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS \
    const uint32_t uPIDFSERVCAPSMETHOD_CUSTOM1 = 1; \
    const uint32_t uPIDFSERVCAPSMETHOD_CUSTOM2 = uPIDFSERVCAPSMETHOD_CUSTOM1 << 1; \
    ... \
    const uint32_t uPIDFSERVCAPSMETHOD_LAST_VALUE = uPIDFSERVCAPSMETHOD_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the servcaps methods SPidfData array corresponding
//  to the MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS is
//  defined. It allows the user to update the servcaps methods SPidfData array
//  corresponding to the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint32_t values.
//
//  When you define this, you must copy the definition of g_stPIDFSERVCAPSMETHOD
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS_SPIDFDATA.
//
//  uPIDFSERVCAPSMETHOD_LAST_VALUE MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS_SPIDFDATA \
      const SPidfData g_stPIDFSERVCAPSMETHOD[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint32_t values defined for the servcaps extensions elements
//  in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint32_t values defined for the servcaps extensions elements
//  in "SceUaImppXml/PidfData.h". To do this, the current definitions must be
//  copied and embedded within this define, and then modified to add or remove
//  values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFSERVCAPSEXT_LAST_VALUE must be defined and equal to last pidf
//      servcaps extensions element's value constant shifted left by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint32_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint32_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint32_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//    - Modifying these values may require modifications to ESipOptionTag.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
// See Also:
//  MXD_OVERRIDE_TYPE_OPTION_TAG.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT \
    const uint32_t uPIDFSERVCAPSEXT_CUSTOM1 = 1; \
    const uint32_t uPIDFSERVCAPSEXT_CUSTOM2 = uPIDFSERVCAPSEXT_CUSTOM1 << 1; \
    ... \
    const uint32_t uPIDFSERVCAPSEXT_LAST_VALUE = uPIDFSERVCAPSEXT_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the servcaps extensions SPidfData array
//  corresponding to the MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT is
//  defined. It allows the user to update the servcaps extensions SPidfData
//  array corresponding to the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint32_t values.
//
//  When you define this, you must copy the definition of g_stPIDFSERVCAPSEXT
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT_SPIDFDATA.
//
//  uPIDFSERVCAPSEXT_LAST_VALUE MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT_SPIDFDATA \
      const SPidfData g_stPIDFSERVCAPSEXT[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint32_t values defined for the servcaps actor elements
//  in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint32_t values defined for the servcaps actor elements
//  in "SceUaImppXml/PidfData.h". To do this, the current definitions must be
//  copied and embedded within this define, and then modified to add or remove
//  values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFSERVCAPSACTOR_LAST_VALUE must be defined and equal to last pidf
//      servcaps actor element's value constant shifted left by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint32_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint32_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint32_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR \
    const uint32_t uPIDFSERVCAPSACTOR_CUSTOM1 = 1; \
    const uint32_t uPIDFSERVCAPSACTOR_CUSTOM2 = uPIDFSERVCAPSACTOR_CUSTOM1 << 1; \
    ... \
    const uint32_t uPIDFSERVCAPSACTOR_LAST_VALUE = uPIDFSERVCAPSACTOR_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the servcaps actor SPidfData array
//  corresponding to the MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR is
//  defined. It allows the user to update the servcaps actor SPidfData
//  array corresponding to the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint32_t values.
//
//  When you define this, you must copy the definition of g_stPIDFSERVCAPSACTOR
//  defined in "SceUaImppXml/PidfData.cpp" and embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR_SPIDFDATA.
//
//  uPIDFSERVCAPSACTOR_LAST_VALUE MUST have an invalid corresponding value (e.g. :
//  "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR_SPIDFDATA \
      const SPidfData g_stPIDFSERVCAPSACTOR[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the EPidfServCapsPriority type defined in
//  "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the EPidfServCapsPriority type defined in
//  "SceUaImppXml/PidfData.h". To do this, the current definition of the type
//  must be copied and embedded within this define, and then modified to add or
//  remove values.
//
//  The following must be considered when overriding this type:
//
//    - This type must be ordered alphabetically with regards to case.
//    - This is a linear enum: the first enum value must be 0 and each following
//      value must be its previous value plus 1.
//    - ePIDFSERVCAPSPRIO_OTHER_NAME must be the last value of this enum and
//      must equal the last enum's value plus 1.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the enumeration
//      values following the added value must be updated.
//    - When removing an item, the items following the removed item must all
//      have their value updated accordingly.
//    - UA-IMPP also holds an array of SPidfData corresponding to each enum
//      value. This enum and the SPidfData array must always be synchronized,
//      that is, the corresponding SPidfData must be located at the same
//      position as its corresponding enum (i.e.: it can be accessed in the
//      array with the enum value). When overriding this type, the user must
//      also override the SPidfData array with
//      MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated type found in "SceUaImppXml/PidfData.h" with its
//      own version of this type, including the corresponding SPidfData array.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY enum EPidfServCapsPriority {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the servcaps priority SPidfData array
//  corresponding to the MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY enumeration.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY is
//  defined. It allows the user to update the servcaps priority SPidfData array
//  corresponding to the updated enumeration.
//
//  The servcaps priority element names (m_pszName member of SPidfData) must
//  always be placed in alphabetical order within that array. Furthermore, their
//  order must correspond to the order of the EPidfServCapsPriority enumeration.
//
//  When you define this, you must copy the definition of
//  g_stPIDFSERVCAPSPRIORITY defined in "SceUaImppXml/PidfData.cpp" and embed it
//  into the definition of MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY_SPIDFDATA.
//
//  ePIDFSERVCAPSPRIO_OTHER_NAME MUST have an invalid corresponding value
//  (e.g. : "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY_SPIDFDATA \
      const SPidfData g_stPIDFSERVCAPSPRIORITY[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint32_t values defined for the devcaps mobility elements
//  in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint32_t values defined for the devcaps mobility elements
//  in "SceUaImppXml/PidfData.h". To do this, the current definitions must be
//  copied and embedded within this define, and then modified to add or remove
//  values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFDEVCAPSMOBILITY_LAST_VALUE must be defined and equal to last pidf
//      devcaps mobility element's value constant shifted left by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint32_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint32_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint32_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY \
    const uint32_t uPIDFDEVCAPSMOBILITY_CUSTOM1 = 1; \
    const uint32_t uPIDFDEVCAPSMOBILITY_CUSTOM2 = uPIDFDEVCAPSMOBILITY_CUSTOM1 << 1; \
    ... \
    const uint32_t uPIDFDEVCAPSMOBILITY_LAST_VALUE = uPIDFDEVCAPSMOBILITY_CUSTOM1 << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the devcaps mobility SPidfData array
//  corresponding to the MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY is
//  defined. It allows the user to update the devcaps mobility SPidfData
//  array corresponding to the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint32_t values.
//
//  When you define this, you must copy the definition of
//  g_stPIDFDEVCAPSMOBILITY defined in "SceUaImppXml/PidfData.cpp" and embed it
//  into the definition of MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY_SPIDFDATA.
//
//  uPIDFDEVCAPSMOBILITY_LAST_VALUE MUST have an invalid corresponding value
//  (e.g. : "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY_SPIDFDATA \
      const SPidfData g_stPIDFDEVCAPSMOBILITY[] = {...};


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Overrides the uint32_t values defined for the network-availability network
//  elements in "SceUaImppXml/PidfData.h".
//
// Description:
//  Overrides the uint32_t values defined for the network-availability network
//  elements in "SceUaImppXml/PidfData.h". To do this, the current definitions
//  must be copied and embedded within this define, and then modified to add or
//  remove values.
//
//  The following must be considered when overriding the values:
//
//    - This type must be ordered alphabetically with regards to case.
//    - Each value must be a single bit bitmask, the first value being 1 and
//      next values being the previous value shifted left by one bit.
//    - uPIDFOMANETWORKAVAILABILITY_LAST_VALUE must be defined and equal to last
//      pidf network-availability network element's value constant shifted left
//      by one bit.
//    - When inserting an item, it must be positioned in alphabetical order
//      within the existing list of items. This means that the bit position of
//      all following items must be updated.
//    - When removing an item, the items following the removed item must all
//      have their bit position updated accordingly.
//    - The UA-IMPP also holds a list of SPidfData corresponding to each bitset
//      value. The uint32_t list and the SPidfData array must always be
//      synchronized, that is, the corresponding SPidfData value must be located
//      at the same position as its corresponding uint32_t value. When
//      overriding this type, the user must also override the SPidfData array
//      with MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY_SPIDFDATA.
//    - When M5T provides updates to UA-IMPP, the user is responsible to
//      synchronize the updated uint32_t list found in "SceUaImppXml/PidfData.h"
//      with its own version of this list, including the corresponding
//      SPidfData.
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY \
    const uint32_t uPIDFOMANETWORKAVAILABILITY_CUSTOM1 = 1; \
    const uint32_t uPIDFOMANETWORKAVAILABILITY_CUSTOM2 = uPIDFOMANETWORKAVAILABILITY_CUSTOM1 << 1; \
    ... \
    const uint32_t uPIDFOMANETWORKAVAILABILITY_LAST_VALUE = uPIDFOMANETWORKAVAILABILITY_CUSTOMX << 1;


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Allows the user to update the network-availability network SPidfData array
//  corresponding to the MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY values.
//
// Description:
//  This macro MUST be defined when MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY
//  is defined. It allows the user to update the network-availability network
//  SPidfData array corresponding to the updated values.
//
//  The SPidfData m_pszName values must always be placed in alphabetical order
//  within that array. Furthermore, their order must correspond to the order of
//  the uint32_t values.
//
//  When you define this, you must copy the definition of
//  g_stPIDFOMANETWORKAVAILABILITY defined in "SceUaImppXml/PidfData.cpp" and
//  embed it into the definition of
//  MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY_SPIDFDATA.
//
//  uPIDFOMANETWORKAVAILABILITY_LAST_VALUE MUST have an invalid corresponding value
//  (e.g. : "{"", ""}").
//
// Location:
//  Define this in PreSceUaImppXmlCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY_SPIDFDATA \
      const SPidfData g_stPIDFOMANETWORKAVAILABILITY[] = {...};


#endif // #if 0 for documentation.


MX_NAMESPACE_END(MXD_GNS)


#if defined(MXD_POST_SCEUAIMPPXMLCFG)
#include "PostSceUaImppXmlCfg.h"
#endif

#endif //-- #ifndef MXG_SCEUAIMPPXMLCFG_H

