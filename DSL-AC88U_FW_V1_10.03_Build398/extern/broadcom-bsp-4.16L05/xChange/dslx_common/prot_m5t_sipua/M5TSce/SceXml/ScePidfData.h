//==SDOC========================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_SCEPIDFDATA_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SCEPIDFDATA_H
//M5T_INTERNAL_USE_END
//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPEVENTTYPES_H
#include "SceSipTypes/SipEventTypes.h"
#endif

#ifndef MXG_SIPMETHODTYPES_H
#include "SceSipTypes/SipMethodTypes.h"
#endif

#ifndef MXG_SIPOPTIONTAGS_H
#include "SceSipTypes/SipOptionTags.h"
#endif

#ifndef MXG_UAIMPPXMLDICTIONNARY_H
#include "SceXml/UaImppXmlDictionary.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// Make sure that either both options are defined or not defined.

#if !defined(MXD_SCE_OVERRIDE_PIDFNAMESPACE) && defined(MXD_SCE_OVERRIDE_DEFAULT_NS_PREFIXES_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFNAMESPACE) && !defined(MXD_SCE_OVERRIDE_DEFAULT_NS_PREFIXES_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFACT) && defined(MXD_SCE_OVERRIDE_PIDFACT_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFACT) && !defined(MXD_SCE_OVERRIDE_PIDFACT_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFMOOD) && defined(MXD_SCE_OVERRIDE_PIDFMOOD_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFMOOD) && !defined(MXD_SCE_OVERRIDE_PIDFMOOD_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO) && defined(MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO) && !defined(MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO) && defined(MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO) && !defined(MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFPLACEISTEXT) && defined(MXD_SCE_OVERRIDE_PIDFPLACEISTEXT_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFPLACEISTEXT) && !defined(MXD_SCE_OVERRIDE_PIDFPLACEISTEXT_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFPLACETYPE) && defined(MXD_SCE_OVERRIDE_PIDFPLACETYPE_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFPLACETYPE) && !defined(MXD_SCE_OVERRIDE_PIDFPLACETYPE_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFPRIVACY) && defined(MXD_SCE_OVERRIDE_PIDFPRIVACY_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFPRIVACY) && !defined(MXD_SCE_OVERRIDE_PIDFPRIVACY_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFRELATIONSHIP) && defined(MXD_SCE_OVERRIDE_PIDFRELATIONSHIP_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFRELATIONSHIP) && !defined(MXD_SCE_OVERRIDE_PIDFRELATIONSHIP_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFSERVICECLASS) && defined(MXD_SCE_OVERRIDE_PIDFSERVICECLASS_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFSERVICECLASS) && !defined(MXD_SCE_OVERRIDE_PIDFSERVICECLASS_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFSPHERE) && defined(MXD_SCE_OVERRIDE_PIDFSPHERE_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFSPHERE) && !defined(MXD_SCE_OVERRIDE_PIDFSPHERE_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX) && defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX) && !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS) && defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS) && !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG) && defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG) && !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS) && defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS) && !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT) && defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT) && !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR) && defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR) && !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY) && defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY) && !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY) && defined(MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY) && !defined(MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY_SPIDFDATA)
#error Must synchronize type and strings.
#endif


#if !defined(MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY) && defined(MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY_SPIDFDATA)
#error Must synchronize type and strings.
#endif

#if defined(MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY) && !defined(MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY_SPIDFDATA)
#error Must synchronize type and strings.
#endif


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//@@TID_PACKAGE_SCEXML_PIDF_DATA
//<TITLE PIDF Data>
//<GROUP TID_PACKAGE_SCEXML_STRUCTSENUMS>

//<TITLE SceXml PIDF SPidfData>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  This type defines a basic structure to represent a single element in a PIDF
//  document. An element consist of a element name and its associated
//  namespace.
struct SPidfData
{
    // Description:
    //  The element name or value.
    const char* m_pszName;

    // Description:
    //  The element namespace.
    const char* m_pszNamespace;
};


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  XML BASIC DEFINITION ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
extern const char* const g_apszXMLBOOLEANVALUE[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  STATUS/BASIC DEFINITION ================================================
//==============================================================================

//<TITLE SceXml PIDF Basic Status>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  This enumeration defines the possible values associated with the service's
//  basic status. The basic status is defined in RFC 3863 as a basic element in
//  the status element.
//
//  This enum cannot be extended.
//
// See Also:
//  GetPidfBasicStatus.
enum EPidfBasicStatus
{
    // Description:
    //  This status is used to report that the service is not working or
    //  the user of this service is unwilling to use it at the moment.
    ePIDFBASIC_CLOSED,

    // Description:
    //  This status is used to report that the service is working and is
    //  available. The user associated with this service is most likely
    //  reachable through the service using the Contact information.
    ePIDFBASIC_OPEN,

    // Description:
    //  The basic element contains another value. Other values are not permitted
    //  by RFC3863 so it should be consideres as an error.
    ePIDFBASIC_OTHER_VALUE
};

//M5T_INTERNAL_USE_BEGIN
extern const char* const g_apszPIDFBASICSTATUS[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  ACTIVITIES DEFINITION ==================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFACT)

//<TITLE SceXml PIDF Activities>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  activities element. The activities element is defined in RFC 4480. Note that
//  many elements can be present in a single activities element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFACT_LAST_VALUE must be equal to two times (2*) the last pidf activities
//  element's value constant.
//
// See Also:
//  GetPidfActivity.
const uint32_t uPIDFACT_APPOINTMENT             = 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_AWAY                    = uPIDFACT_APPOINTMENT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_BREAKFAST               = uPIDFACT_AWAY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_BUSY                    = uPIDFACT_BREAKFAST << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_DINNER                  = uPIDFACT_BUSY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_HOLIDAY                 = uPIDFACT_DINNER << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_IN_TRANSIT              = uPIDFACT_HOLIDAY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_LOOKING_FOR_WORK        = uPIDFACT_IN_TRANSIT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_MEAL                    = uPIDFACT_LOOKING_FOR_WORK << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_MEETING                 = uPIDFACT_MEAL << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_ON_THE_PHONE            = uPIDFACT_MEETING << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_OTHER                   = uPIDFACT_ON_THE_PHONE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_PERFORMANCE             = uPIDFACT_OTHER << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_PERMANENT_ABSENCE       = uPIDFACT_PERFORMANCE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_PLAYING                 = uPIDFACT_PERMANENT_ABSENCE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_PRESENTATION            = uPIDFACT_PLAYING << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_SHOPPING                = uPIDFACT_PRESENTATION << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_SLEEPING                = uPIDFACT_SHOPPING << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_SPECTATOR               = uPIDFACT_SLEEPING << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_STEERING                = uPIDFACT_SPECTATOR << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_TRAVEL                  = uPIDFACT_STEERING << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_TV                      = uPIDFACT_TRAVEL << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_UNKNOWN                 = uPIDFACT_TV << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_VACATION                = uPIDFACT_UNKNOWN << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_WORKING                 = uPIDFACT_VACATION << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFACT_APPOINTMENT>
const uint32_t uPIDFACT_WORSHIP                 = uPIDFACT_WORKING << 1;

//M5T_INTERNAL_USE_BEGIN
const uint32_t uPIDFACT_LAST_VALUE              = uPIDFACT_WORSHIP << 1;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFACT
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFACTIVITIES[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  MOOD DEFINITION ========================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFMOOD)
//<TITLE SceXml PIDF Mood>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  mood element. The mood element is defined in RFC 4480. Note that many
//  elements can be present in a single mood element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFMOOD_LAST_VALUE must be equal to two times (2*) the last pidf mood
//  element's name constant.
//
// See Also:
//  GetPidfMood.
const uint64_t uPIDFMOOD_AFRAID             = 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_AMAZED             = uPIDFMOOD_AFRAID << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_ANGRY              = uPIDFMOOD_AMAZED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_ANNOYED            = uPIDFMOOD_ANGRY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_ANXIOUS            = uPIDFMOOD_ANNOYED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_ASHAMED            = uPIDFMOOD_ANXIOUS << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_BORED              = uPIDFMOOD_ASHAMED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_BRAVE              = uPIDFMOOD_BORED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_CALM               = uPIDFMOOD_BRAVE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_COLD               = uPIDFMOOD_CALM << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_CONFUSED           = uPIDFMOOD_COLD << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_CONTENTED          = uPIDFMOOD_CONFUSED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_CRANKY             = uPIDFMOOD_CONTENTED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_CURIOUS            = uPIDFMOOD_CRANKY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_DEPRESSED          = uPIDFMOOD_CURIOUS << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_DISAPPOINTED       = uPIDFMOOD_DEPRESSED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_DISGUSTED          = uPIDFMOOD_DISAPPOINTED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_DISTRACTED         = uPIDFMOOD_DISGUSTED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_EMBARRASSED        = uPIDFMOOD_DISTRACTED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_EXCITED            = uPIDFMOOD_EMBARRASSED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_FLIRTATIOUS        = uPIDFMOOD_EXCITED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_FRUSTRATED         = uPIDFMOOD_FLIRTATIOUS << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_GRUMPY             = uPIDFMOOD_FRUSTRATED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_GUILTY             = uPIDFMOOD_GRUMPY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_HAPPY              = uPIDFMOOD_GUILTY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_HOT                = uPIDFMOOD_HAPPY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_HUMBLED            = uPIDFMOOD_HOT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_HUMILIATED         = uPIDFMOOD_HUMBLED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_HUNGRY             = uPIDFMOOD_HUMILIATED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_HURT               = uPIDFMOOD_HUNGRY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_IMPRESSED          = uPIDFMOOD_HURT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_IN_AWE             = uPIDFMOOD_IMPRESSED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_IN_LOVE            = uPIDFMOOD_IN_AWE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_INDIGNANT          = uPIDFMOOD_IN_LOVE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_INTERESTED         = uPIDFMOOD_INDIGNANT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_INVINCIBLE         = uPIDFMOOD_INTERESTED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_JEALOUS            = uPIDFMOOD_INVINCIBLE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_LONELY             = uPIDFMOOD_JEALOUS << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_MEAN               = uPIDFMOOD_LONELY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_MOODY              = uPIDFMOOD_MEAN << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_NERVOUS            = uPIDFMOOD_MOODY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_NEUTRAL            = uPIDFMOOD_NERVOUS << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_OFFENDED           = uPIDFMOOD_NEUTRAL << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_OTHER              = uPIDFMOOD_OFFENDED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_PLAYFUL            = uPIDFMOOD_OTHER << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_PROUD              = uPIDFMOOD_PLAYFUL << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_RELIEVED           = uPIDFMOOD_PROUD << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_REMORSEFUL         = uPIDFMOOD_RELIEVED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_RESTLESS           = uPIDFMOOD_REMORSEFUL << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_SAD                = uPIDFMOOD_RESTLESS << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_SARCASTIC          = uPIDFMOOD_SAD << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_SERIOUS            = uPIDFMOOD_SARCASTIC << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_SHOCKED            = uPIDFMOOD_SERIOUS << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_SHY                = uPIDFMOOD_SHOCKED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_SICK               = uPIDFMOOD_SHY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_SLEEPY             = uPIDFMOOD_SICK << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_STRESSED           = uPIDFMOOD_SLEEPY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_SURPRISED          = uPIDFMOOD_STRESSED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_THIRSTY            = uPIDFMOOD_SURPRISED << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_UNKNOWN            = uPIDFMOOD_THIRSTY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFMOOD_AFRAID>
const uint64_t uPIDFMOOD_WORRIED            = uPIDFMOOD_UNKNOWN << 1;

//M5T_INTERNAL_USE_BEGIN
const uint64_t uPIDFMOOD_LAST_VALUE         = uPIDFMOOD_WORRIED << 1;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFMOOD
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFMOOD[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  PLACE-IS DEFINITION ====================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO)
//<TITLE SceXml PIDF Place-Is Audio>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  place-is audio element. The place-is audio element is defined in RFC 4480 as
//  an audio element in a place-is element. Note that only one of the element
//  can be present in a single place-is audio element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  This is a linear enum: the first enum value must be 0 and each following
//  value must be its previous value plus 1.
//
//  ePIDFAUDIO_OTHER_NAME must be the last value of this enum and must equal
//  the last enum's value plus 1.
//
// See Also:
//  GetPidfPlaceIsAudio.
enum EPidfPlaceIsAudio
{
    ePIDFAUDIO_NOISY,
    ePIDFAUDIO_OK,
    ePIDFAUDIO_QUIET,
    ePIDFAUDIO_UNKNOWN,

    ePIDFAUDIO_OTHER_NAME
};

#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFPLACEISAUDIO
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFPLACEISAUDIO[];
//M5T_INTERNAL_USE_END


#if !defined(MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO)
//<TITLE SceXml PIDF Place-Is Video>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  place-is video element. The place-is video element is defined in RFC 4480 as
//  a video element in a place-is element. Note that only one of the element
//  can be present in a single place-is video element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  This is a linear enum: the first enum value must be 0 and each following
//  value must be its previous value plus 1.
//
//  ePIDFVIDEO_OTHER_NAME must be the last value of this enum and must equal
//  the last enum's value plus 1.
//
// See Also:
//  GetPidfPlaceIsVideo.
enum EPidfPlaceIsVideo
{
    ePIDFVIDEO_DARK,
    ePIDFVIDEO_OK,
    ePIDFVIDEO_TOOBRIGHT,
    ePIDFVIDEO_UNKNOWN,

    ePIDFVIDEO_OTHER_NAME
};
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFPLACEISVIDEO
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFPLACEISVIDEO[];
//M5T_INTERNAL_USE_END


#if !defined(MXD_SCE_OVERRIDE_PIDFPLACEISTEXT)
//<TITLE SceXml PIDF Place-Is Text>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  place-is text element. The place-is text element is defined in RFC 4480 as
//  a text element in a place-is element. Note that only one of the element can
//  be present in a single place-is text element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  This is a linear enum: the first enum value must be 0 and each following
//  value must be its previous value plus 1.
//
//  ePIDFTEXT_OTHER_NAME must be the last value of this enum and must equal
//  the last enum's value plus 1.
//
// See Also:
//  GetPidfPlaceIsText.
enum EPidfPlaceIsText
{
    ePIDFTEXT_INAPPROPRIATE,
    ePIDFTEXT_OK,
    ePIDFTEXT_UNCOMFORTABLE,
    ePIDFTEXT_UNKNOWN,

    ePIDFTEXT_OTHER_NAME
};
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFPLACEISTEXT
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFPLACEISTEXT[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  PLACE-TYPE DEFINITION ==================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFPLACETYPE)
//<TITLE SceXml PIDF Place-Type>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  place-type element. The place-type element is defined in RFC 4480. Note
//  that many elements can be present in a single place-type element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFPLACETYPE_LAST_VALUE must be equal to two times (2*) the last pidf
//  place-type element's name constant.
//
// See Also:
//  GetPidfPlaceType.
const uint64_t uPIDFPLACETYPE_AIRCRAFT              = 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_AIRPORT               = uPIDFPLACETYPE_AIRCRAFT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_ARENA                 = uPIDFPLACETYPE_AIRPORT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_AUTOMOBILE            = uPIDFPLACETYPE_ARENA << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_BANK                  = uPIDFPLACETYPE_AUTOMOBILE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_BAR                   = uPIDFPLACETYPE_BANK << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_BICYLE                = uPIDFPLACETYPE_BAR << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_BUS                   = uPIDFPLACETYPE_BICYLE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_BUS_STATION           = uPIDFPLACETYPE_BUS << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_CAFE                  = uPIDFPLACETYPE_BUS_STATION << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_CLASSROOM             = uPIDFPLACETYPE_CAFE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_CLUB                  = uPIDFPLACETYPE_CLASSROOM << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_CONSTRUCTION          = uPIDFPLACETYPE_CLUB << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_CONVENTION_CENTER     = uPIDFPLACETYPE_CONSTRUCTION << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_GOVERNMENT            = uPIDFPLACETYPE_CONVENTION_CENTER << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_HOSPITAL              = uPIDFPLACETYPE_GOVERNMENT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_HOTEL                 = uPIDFPLACETYPE_HOSPITAL << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_INDUSTRIAL            = uPIDFPLACETYPE_HOTEL << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_LIBRARY               = uPIDFPLACETYPE_INDUSTRIAL << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_MOTORCYCLE            = uPIDFPLACETYPE_LIBRARY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_OFFICE                = uPIDFPLACETYPE_MOTORCYCLE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_OTHER                 = uPIDFPLACETYPE_OFFICE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_OUTDOORS              = uPIDFPLACETYPE_OTHER << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_PARKING               = uPIDFPLACETYPE_OUTDOORS << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_PLACE_OF_WORSHIP      = uPIDFPLACETYPE_PARKING << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_PRISON                = uPIDFPLACETYPE_PLACE_OF_WORSHIP << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_PUBLIC                = uPIDFPLACETYPE_PRISON << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_PUBLIC_TRANSPORT      = uPIDFPLACETYPE_PUBLIC << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_RESIDENCE             = uPIDFPLACETYPE_PUBLIC_TRANSPORT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_RESTAURANT            = uPIDFPLACETYPE_RESIDENCE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_SCHOOL                = uPIDFPLACETYPE_RESTAURANT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_SHOPPING_AREA         = uPIDFPLACETYPE_SCHOOL << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_STADIUM               = uPIDFPLACETYPE_SHOPPING_AREA << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_STORE                 = uPIDFPLACETYPE_STADIUM << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_STREET                = uPIDFPLACETYPE_STORE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_THEATER               = uPIDFPLACETYPE_STREET << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_TRAIN                 = uPIDFPLACETYPE_THEATER << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_TRAIN_STATION         = uPIDFPLACETYPE_TRAIN << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_TRUCK                 = uPIDFPLACETYPE_TRAIN_STATION << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_UNDERWAY              = uPIDFPLACETYPE_TRUCK << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_UNKNOWN               = uPIDFPLACETYPE_UNDERWAY << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_WAREHOUSE             = uPIDFPLACETYPE_UNKNOWN << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_WATER                 = uPIDFPLACETYPE_WAREHOUSE << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPLACETYPE_AIRCRAFT>
const uint64_t uPIDFPLACETYPE_WATERCRAFT            = uPIDFPLACETYPE_WATER << 1;


//M5T_INTERNAL_USE_BEGIN
const uint64_t uPIDFPLACETYPE_LAST_VALUE            = uPIDFPLACETYPE_WATERCRAFT << 1;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFPLACETYPE
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFPLACETYPE[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  PRIVACY DEFINITION =====================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFPRIVACY)
//<TITLE SceXml PIDF Privacy>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a privacy
//  element. The privacy element is defined in RFC 4480. Note that many
//  elements can be present in a single privacy element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFPRIVACY_LAST_VALUE must be equal to two times (2*) the last pidf
//  privacy element's name constant.
//
// See Also:
//  GetPidfPlaceType.
const uint32_t uPIDFPRIVACY_AUDIO       = 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPRIVACY_AUDIO>
const uint32_t uPIDFPRIVACY_TEXT        = uPIDFPRIVACY_AUDIO << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPRIVACY_AUDIO>
const uint32_t uPIDFPRIVACY_UNKNOWN     = uPIDFPRIVACY_TEXT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFPRIVACY_AUDIO>
const uint32_t uPIDFPRIVACY_VIDEO       = uPIDFPRIVACY_UNKNOWN << 1;

//M5T_INTERNAL_USE_BEGIN
const uint32_t uPIDFPRIVACY_LAST_VALUE  = uPIDFPRIVACY_VIDEO << 1;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFPRIVACY
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFPRIVACY[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  RELATIONSHIP DEFINITION ================================================
//==============================================================================


#if !defined(MXD_SCE_OVERRIDE_PIDFRELATIONSHIP)
//<TITLE SceXml PIDF Relationship>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  relationship element. The relationship element is defined in RFC 4480. Note
//  that only one of the element can be present in a single relationship
//  element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  This is a linear enum: the first enum value must be 0 and each following
//  value must be its previous value plus 1.
//
//  ePIDFRELATIONSHIP_OTHER_NAME must be the last value of this enum and must
//  equal the last enum's value plus 1.
//
// See Also:
//  GetPidfRelationship.
enum EPidfRelationship
{
    ePIDFRELATIONSHIP_ASSISTANT,
    ePIDFRELATIONSHIP_ASSOCIATE,
    ePIDFRELATIONSHIP_FAMILY,
    ePIDFRELATIONSHIP_FRIEND,
    ePIDFRELATIONSHIP_OTHER,
    ePIDFRELATIONSHIP_SELF,
    ePIDFRELATIONSHIP_SUPERVISOR,
    ePIDFRELATIONSHIP_UNKNOWN,

    ePIDFRELATIONSHIP_OTHER_NAME
};
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFRELATIONSHIP
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFRELATIONSHIP[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  SERCIVE-CLASS DEFINITION ===============================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFSERVICECLASS)
//<TITLE SceXml PIDF Service Class>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  service-class element. The service-class element is defined in RFC 4480.
//  Note that only one of the element can be present in a single service-class
//  element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  This is a linear enum: the first enum value must be 0 and each following
//  value must be its previous value plus 1.
//
//  ePIDFSERVICECLASS_OTHER_NAME must be the last value of this enum and must
//  equal the last enum's value plus 1.
//
// See Also:
//  GetPidfServiceClass.
enum EPidfServiceClass
{
    ePIDFSERVICECLASS_COURIER,
    ePIDFSERVICECLASS_ELECTRONIC,
    ePIDFSERVICECLASS_FREIGHT,
    ePIDFSERVICECLASS_IN_PERSON,
    ePIDFSERVICECLASS_POSTAL,
    ePIDFSERVICECLASS_UNKNOWN,

    ePIDFSERVICECLASS_OTHER_NAME
};
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFSERVICECLASS
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFSERVICECLASS[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  SPHERE DEFINITION ======================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFSPHERE)
//<TITLE SceXml PIDF Sphere>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  sphere element. The sphere element is defined in RFC 4480. Note that only
//  one of the element can be present in a single sphere element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  This is a linear enum: the first enum value must be 0 and each following
//  value must be its previous value plus 1.
//
//  ePIDFSPHERE_OTHER_NAME must be the last value of this enum and must
//  equal the last enum's value plus 1.
//
// See Also:
//  GetPidfSphere.
enum EPidfSphere
{
    ePIDFSPHERE_HOME,
    ePIDFSPHERE_UNKNOWN,
    ePIDFSPHERE_WORK,

    ePIDFSPHERE_OTHER_NAME
};
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFSPHERE
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFSPHERE[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  USER-INPUT DEFINITION ==================================================
//==============================================================================

//<TITLE SceXml PIDF User-Input>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the possible values of the user-input element. The
//  user-input element is defined in RFC 4480.
//
//  This type cannot be extended.
//
// See Also:
//  GetPidfUserInputValue.
enum EPidfUserInputValue
{
    // Description:
    //  "active".
    ePIDFUSERINPUT_ACTIVE,
    // Description:
    //  "idle".
    ePIDFUSERINPUT_IDLE,

    // Description:
    //  A value different from the predefined values. Other values are not
    //  permitted by RFC4480 so it should be considered as an error.
    ePIDFUSERINPUT_OTHER_NAME
};

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFUSERINPUT[];
//M5T_INTERNAL_USE_END

//==============================================================================
//====  SERVICE CAPS - Duplex ==================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX)
//<TITLE SceXml PIDF Service Capabilities Duplex>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  supported or notsupported element present in a duplex element of a
//  servcaps element. These elements are defined in RFC 5196. Note that many
//  elements can be present in a single supported or notsupported element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFSERVCAPSDUPLEX_LAST_VALUE must be equal to two times (2*) the last pidf
//  servcaps duplex element's name constant.
//
// See Also:
//  GetPidfServCapsDuplex.
const uint32_t uPIDFSERVCAPSDUPLEX_FULL         = 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSDUPLEX_FULL>
const uint32_t uPIDFSERVCAPSDUPLEX_HALF         = uPIDFSERVCAPSDUPLEX_FULL << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSDUPLEX_FULL>
const uint32_t uPIDFSERVCAPSDUPLEX_RECEIVEONLY  = uPIDFSERVCAPSDUPLEX_HALF << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSDUPLEX_FULL>
const uint32_t uPIDFSERVCAPSDUPLEX_SENDONLY     = uPIDFSERVCAPSDUPLEX_RECEIVEONLY << 1;

//M5T_INTERNAL_USE_BEGIN
const uint32_t uPIDFSERVCAPSDUPLEX_LAST_VALUE   = uPIDFSERVCAPSDUPLEX_SENDONLY << 1;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFSERVCAPSDUPLEX
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFSERVCAPSDUPLEX[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  SERVICE CAPS - Class ===================================================
//==============================================================================


#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS)
//<TITLE SceXml PIDF Service Capabilities Class>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  supported or notsupported element present in a class element of a
//  servcaps element. These elements are defined in RFC 5196. Note that many
//  elements can be present in a single supported or notsupported element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFSERVCAPSCLASS_LAST_VALUE must be equal to two times the last pidf
//  servcaps class element's name constant.
//
// See Also:
//  GetPidfServCapsClass.
const uint32_t uPIDFSERVCAPSCLASS_BUSINESS      = 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSDUPLEX_FULL>
const uint32_t uPIDFSERVCAPSCLASS_PERSONAL      = uPIDFSERVCAPSCLASS_BUSINESS << 1;

//M5T_INTERNAL_USE_BEGIN
const uint32_t uPIDFSERVCAPSCLASS_LAST_VALUE    = uPIDFSERVCAPSCLASS_PERSONAL << 1;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFSERVCAPSCLASS
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFSERVCAPSCLASS[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  SERVICE CAPS - Event ===================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG)
//<TITLE SceXml PIDF Service Capabilities Event Packages>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  supported or notsupported element present in an event-packages element of a
//  servcaps element. These elements are defined in RFC 5196. Note that many
//  elements can be present in a single supported or notsupported element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFSERVCAPSEVENTPKG_LAST_VALUE must be equal to two times the last pidf
//  servcaps event-packages element's name constant.
//
// Warning:
//  Modifying these values may require modifications to SceSipTypes/SipEventTypes.
//
// See Also:
//  GetPidfServCapsEventPkg.
const uint32_t uPIDFSERVCAPSEVENTPKG_CONFERENCE         = eEVENTTYPE_CONFERENCE;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_DIALOG             = eEVENTTYPE_DIALOG;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_KPML               = eEVENTTYPE_KPML;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_MESSAGE_SUMMARY    = eEVENTTYPE_MESSAGE_SUMMARY;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_POC_SETTINGS       = eEVENTTYPE_POC_SETTINGS;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_PRESENCE           = eEVENTTYPE_PRESENCE;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_PRESENCE_WINFO     = eEVENTTYPE_PRESENCE_WINFO;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_REFER              = eEVENTTYPE_REFER;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_REG                = eEVENTTYPE_REG;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_SIEMENS_RTP_STATS  = eEVENTTYPE_SIEMENS_RTP_STATS;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_SPIRITS_INDPS      = eEVENTTYPE_SPIRITS_INDPS;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_SPIRITS_USER_PROF  = eEVENTTYPE_SPIRITS_USER_PROF;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_WINFO              = eEVENTTYPE_WINFO;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEVENTPKG_CONFERENCE>
const uint32_t uPIDFSERVCAPSEVENTPKG_XCAP_DIFF          = eEVENTTYPE_XCAP_DIFF;

//M5T_INTERNAL_USE_BEGIN
const uint32_t uPIDFSERVCAPSEVENTPKG_LAST_VALUE        = eEVENTTYPE_LAST;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFSERVCAPSEVENTPKG
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFSERVCAPSEVENTPKG[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  SERVICE CAPS - Methods =================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS)
//<TITLE SceXml PIDF Service Capabilities Methods>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  supported or notsupported element present in a methods element of a
//  servcaps element. These elements are defined in RFC 5196. Note that many
//  elements can be present in a single supported or notsupported element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFSERVCAPSMETHOD_LAST_VALUE must be equal to two times the last pidf
//  servcaps method element's name constant.
//
// Warning:
//  Modifying these values may require modifications to SceSipTypes/SipMethodTypes.
//
// See Also:
//  GetPidfServCapsMethod.
const uint32_t uPIDFSERVCAPSMETHOD_ACK            = eMETHODTYPE_ACK;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_BYE            = eMETHODTYPE_BYE;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_CANCEL         = eMETHODTYPE_CANCEL;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_INFO           = eMETHODTYPE_INFO;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_INVITE         = eMETHODTYPE_INVITE;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_MESSAGE        = eMETHODTYPE_MESSAGE;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_NOTIFY         = eMETHODTYPE_NOTIFY;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_OPTIONS        = eMETHODTYPE_OPTIONS;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_PING           = eMETHODTYPE_PING;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_PRACK          = eMETHODTYPE_PRACK;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_PUBLISH        = eMETHODTYPE_PUBLISH;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_REFER          = eMETHODTYPE_REFER;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_REGISTER       = eMETHODTYPE_REGISTER;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_SERVICE        = eMETHODTYPE_SERVICE;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_SUBSCRIBE      = eMETHODTYPE_SUBSCRIBE;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSMETHOD_ACK>
const uint32_t uPIDFSERVCAPSMETHOD_UPDATE         = eMETHODTYPE_UPDATE;

//M5T_INTERNAL_USE_BEGIN
const uint32_t uPIDFSERVCAPSMETHOD_LAST_VALUE     = eMETHODTYPE_LAST;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFSERVCAPSMETHODS
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFSERVCAPSMETHODS[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  SERVICE CAPS - Extensions ==============================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT)
//<TITLE SceXml PIDF Service Capabilities Extensions>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  supported or notsupported element present in an extensions element of a
//  servcaps element. These elements are defined in RFC 5196. Note that many
//  elements can be present in a single supported or notsupported element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFSERVCAPSEXT_LAST_VALUE must be equal to two times the last pidf
//  servcaps extensions element's name constant.
//
// Warning:
//  Modifying these values may require modifications to SceSipTypes/SipOptionTags.
//
// See Also:
//  GetPidfServCapsExtension.
const uint32_t uPIDFSERVCAPSEXT_REL100              = eOPTIONTAG_100REL;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_ANSWERMODE          = eOPTIONTAG_ANSWERMODE;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_EARLY_SESSION       = eOPTIONTAG_EARLY_SESSION;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_EVENTLIST           = eOPTIONTAG_EVENTLIST;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_FROM_CHANGE         = eOPTIONTAG_FROM_CHANGE;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_GRUU                = eOPTIONTAG_GRUU;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_HISTINFO            = eOPTIONTAG_HISTINFO;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_JOIN                = eOPTIONTAG_JOIN;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_NOREFERSUB          = eOPTIONTAG_NOREFERSUB;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_OUTBOUND            = eOPTIONTAG_OUTBOUND;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_PATH                = eOPTIONTAG_PATH;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_PRECONDITION        = eOPTIONTAG_PRECONDITION;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_PREF                = eOPTIONTAG_PREF;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_PRIVACY             = eOPTIONTAG_PRIVACY;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_REPLACES            = eOPTIONTAG_REPLACES;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_RESOURCE_PRIORITY   = eOPTIONTAG_RESOURCE_PRIORITY;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_SDP_ANAT            = eOPTIONTAG_SDP_ANAT;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_SEC_AGREE           = eOPTIONTAG_SEC_AGREE;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_TDIALOG             = eOPTIONTAG_TDIALOG;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSEXT_REL100>
const uint32_t uPIDFSERVCAPSEXT_TIMER               = eOPTIONTAG_TIMER;

//M5T_INTERNAL_USE_BEGIN
const uint32_t uPIDFSERVCAPSEXT_LAST_VALUE          = eOPTIONTAG_LAST;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFSERVCAPSEXT
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFSERVCAPSEXT[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  SERVICE CAPS - Actor ===================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR)
//<TITLE SceXml PIDF Service Capabilities Actor>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  supported or notsupported element present in an actor element of a
//  servcaps element. These elements are defined in RFC 5196. Note that many
//  elements can be present in a single supported or notsupported element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFSERVCAPSACTOR_LAST_VALUE must be equal to two times the last pidf
//  servcaps actor element's name constant.
//
// See Also:
//  GetPidfServCapsActor.
const uint32_t uPIDFSERVCAPSACTOR_ATTENDANT     = 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSACTOR_ATTENDANT>
const uint32_t uPIDFSERVCAPSACTOR_INFORMATION   = uPIDFSERVCAPSACTOR_ATTENDANT << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSACTOR_ATTENDANT>
const uint32_t uPIDFSERVCAPSACTOR_MSG_TAKER     = uPIDFSERVCAPSACTOR_INFORMATION << 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFSERVCAPSACTOR_ATTENDANT>
const uint32_t uPIDFSERVCAPSACTOR_PRINCIPAL     = uPIDFSERVCAPSACTOR_MSG_TAKER << 1;

//M5T_INTERNAL_USE_BEGIN
const uint32_t uPIDFSERVCAPSACTOR_LAST_VALUE    = uPIDFSERVCAPSACTOR_PRINCIPAL << 1;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFSERVCAPSACTOR
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFSERVCAPSACTOR[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  SERVICE CAPS - Priority ================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY)
//<TITLE SceXml PIDF Service Capabilities Priority>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  supported or notsupported element present in a priority element of a
//  servcaps element. These elements are defined in RFC 5196. Note that only
//  one of the element can be present in a single supported or notsupported
//  element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  This is a linear enum: the first enum value must be 0 and each following
//  value must be its previous value plus 1.
//
//  ePIDFSERVCAPSPRIO_OTHER_NAME must be the last value of this enum and must
//  equal the last enum's value plus 1.
//
// See Also:
//  GetPidfServCapsPriority.
enum EPidfServCapsPriority
{
    ePIDFSERVCAPSPRIO_LOWERTHAN,
    ePIDFSERVCAPSPRIO_HIGHERTHAN,
    ePIDFSERVCAPSPRIO_EQUALS,
    ePIDFSERVCAPSPRIO_RANGE,

    ePIDFSERVCAPSPRIO_OTHER_NAME
};
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFSERVCAPSPRIORITY
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFSERVCAPSPRIORITY[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  DEVICE CAPS - Mobility =================================================
//==============================================================================

#if !defined(MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY)
//<TITLE SceXml PIDF Device Capabilities Mobility>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  supported or notsupported element present in a mobility element of a
//  devcaps element. These elements are defined in RFC 5196. Note that many
//  elements can be present in a single supported or notsupported element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFDEVCAPSMOBILITY_LAST_VALUE must be equal to two times the last pidf
//  devcaps mobility element's name constant.
//
// See Also:
//  GetPidfDevCapsMobility.
const uint32_t uPIDFDEVCAPSMOBILITY_FIXED       = 1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFDEVCAPSMOBILITY_FIXED>
const uint32_t uPIDFDEVCAPSMOBILITY_MOBILE      = uPIDFDEVCAPSMOBILITY_FIXED << 1;

//M5T_INTERNAL_USE_BEGIN
const uint32_t uPIDFDEVCAPSMOBILITY_LAST_VALUE  = uPIDFDEVCAPSMOBILITY_MOBILE << 1;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFDEVCAPSMOBILITY
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFDEVCAPSMOBILITY[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  OMA - active and terminated ============================================
//==============================================================================

//<TITLE SceXml PIDF OMA>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the possible values of some elements defined in
//  oma-sup-xsd_prs_pidf_omapres-v1_0_1-20061128-a. The possible values are
//  "active" and "terminated".
//
//  This type cannot be extended.
//
// See Also:
//  GetPidfOmaActiveTerminated.
enum EPidfOmaActiveTerminated
{
    // Description:
    //  "active".
    ePIDFOMAACTIVETERMINATED_ACTIVE,
    // Description:
    //  "terminated".
    ePIDFOMAACTIVETERMINATED_TERMINATED,

    // Description:
    //  A value different from the predefined values. Other values are not
    //  permitted by oma-sup-xsd_prs_pidf_omapres-v1_0_1-20061128-a so it should
    //  be consideres as an error.
    ePIDFOMAACTIVETERMINATED_OTHER_VALUE
};
//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFOMAACTIVETERMINATED[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  OMA - Network Availability =============================================
//==============================================================================


#if !defined(MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY)
//<TITLE SceXml PIDF OMA Network Availability>
//<GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
//
// Description:
//  These values represent the name of the possible elements to be in a
//  network element present in a network-availability element. These elements
//  are defined in oma-sup-xsd_prs_pidf_omapres-v1_0_1-20061128-a. Note that
//  many elements can be present in a single network element.
//
//  This type must be ordered alphabetically with regards to case.
//
//  Each value must be a single bit bitmask (the first value being 1 and next
//  values being 2 times the previous value).
//
//  uPIDFOMANETWORKAVAILABILITY_LAST_VALUE must be equal to two times the name
//  constant for the last network-availability element's network element.
//
// See Also:
//  GetPidfOmaNetworkAvailability.
const uint32_t uPIDFOMANETWORKAVAILABILITY_ACTIVE =     1;
// <GROUP TID_PACKAGE_SCEXML_PIDF_DATA>
// <COMBINE uPIDFOMANETWORKAVAILABILITY_ACTIVE>
const uint32_t uPIDFOMANETWORKAVAILABILITY_TERMINATED = uPIDFOMANETWORKAVAILABILITY_ACTIVE << 1;

//M5T_INTERNAL_USE_BEGIN
const uint32_t uPIDFOMANETWORKAVAILABILITY_LAST_VALUE = uPIDFOMANETWORKAVAILABILITY_TERMINATED << 1;
//M5T_INTERNAL_USE_END
#else
//M5T_INTERNAL_USE_BEGIN
MXD_SCE_OVERRIDE_PIDFOMANETWORKAVAILABILITY
//M5T_INTERNAL_USE_END
#endif

//M5T_INTERNAL_USE_BEGIN
extern const SPidfData g_stPIDFOMANETWORKAVAILABILITY[];
//M5T_INTERNAL_USE_END


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================



MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SCEPIDFDATA_H

