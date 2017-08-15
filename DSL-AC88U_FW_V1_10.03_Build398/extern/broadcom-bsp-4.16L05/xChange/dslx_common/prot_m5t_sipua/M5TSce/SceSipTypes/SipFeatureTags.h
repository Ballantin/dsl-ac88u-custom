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
#ifndef MXG_SIPFEATURETAGS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SIPFEATURETAGS_H
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
#include "Config/SceCoreCfg.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// Make sure that either both options are not defined or that both options are
// defined.

#if !defined(MXD_EXTEND_TYPE_FEATURE_TAG) && defined(MXD_EXTEND_TYPE_FEATURE_TAG_STRINGS)
#error Must synchronize type and strings.
#endif

#if defined(MXD_EXTEND_TYPE_FEATURE_TAG) && !defined(MXD_EXTEND_TYPE_FEATURE_TAG_STRINGS)
#error Must synchronize type and strings.
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//<GROUP TID_PACKAGE_SCECORE_STRUCTSENUMS>
// Summary:
//  Defines SIP feature tags.
//
// Description:
//  This enumeration describes the various IANA registered SIP feature tags.
//  These can be found at http://www.iana.org/assignments/media-feature-tags.
//
//  It is allowed to add non-IANA supported types, but such items must be
//  clearly indicated.
//
//  This type is extendable by defining MXD_EXTEND_TYPE_FEATURE_TAG.
//
//  See Also:
//      MXD_EXTEND_TYPE_FEATURE_TAG
//
enum ESipFeatureTag
{
    //M5T_INTERNAL_USE_BEGIN
    eSIPFEATURETAG_FIRST,      // First
    //M5T_INTERNAL_USE_END

    // Description:
    // This feature tag indicates that the device supports audio as a
    // streaming media type.
    //
    // This feature tag is defined in RFC 3840
    eSIPFEATURETAG_AUDIO = eSIPFEATURETAG_FIRST,

    // Description:
    // This feature tag indicates that the device supports application as a
    // streaming media type. This feature tag exists primarily for
    // completeness. Since so many MIME types are underneath application,
    // indicating the ability to support applications provides little useful
    // information.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_APPLICATION,

    // Description:
    // This feature tag indicates that the device supports data as a
    // streaming media type.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_DATA,

    // Description:
    // This feature tag indicates that the device supports control as a
    // streaming media type.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_CONTROL,

    // Description:
    // This feature tag indicates that the device supports video as a
    // streaming media type.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_VIDEO,

    // Description:
    // This feature tag indicates that the device supports text as a
    // streaming media type.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_TEXT,

    // Description:
    // The sip.automata feature tag is a boolean value that indicates
    // whether the UA represents an automata (such as a voicemail server,
    // conference server, IVR, or recording device) or a human.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_AUTOMATA,

    // Description:
    // This feature tag indicates the setting, business or personal, in
    // which a communications device is used.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_CLASS,

    // Description:
    // The sip.duplex media feature tag indicates whether a communications
    // device can simultaneously send and receive media ("full"), alternate
    // between sending and receiving ("half"), can only receive
    // ("receive-only"), or only send ("send-only").
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_DUPLEX,

    // Description:
    // The sip.mobility feature tag indicates whether the device is fixed
    // (meaning that it is associated with a fixed point of contact with the
    // network), or mobile (meaning that it is not associated with a fixed
    // point of contact). Note that cordless phones are fixed, not mobile,
    // based on this definition.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_MOBILITY,

    // Description:
    // The sip.description feature tag provides a textual description of the
    // device.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_DESCRIPTION,

    // Description:
    // Each feature tag value indicates a SIP event package supported by a
    // SIP UA. The values for this tag equal the event package names that
    // are registered by each event package.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_EVENTS,

    // Description:
    // The sip.priority feature tag indicates the call priorities the device
    // is willing to handle. A value of X means that the device is willing
    // to take requests with priority X and higher. This does not imply that
    // a phone has to reject calls of lower priority. As always, the
    // decision on handling such calls is a matter of local policy.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_PRIORITY,

    // Description:
    // Each value of the sip.methods (note the plurality) feature tag
    // indicates a SIP method supported by this UA. In this case,
    // "supported" means that the UA can receive requests with this method.
    // In that sense, it has the same connotation as the Allow header field.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_METHODS,

    // Description:
    // Each value of the sip.extensions feature tag is a SIP extension (each
    // of which is defined by an option-tag registered with IANA) that is
    // understood by the UA. Understood, in this context, means that the
    // option tag would be included in a Supported header field in a
    // request.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_EXTENSIONS,

    // Description:
    // Each value of the sip.schemes (note the plurality) media feature tag
    // indicates a URI scheme that is supported by a UA. Supported implies,
    // for example, that the UA would know how to handle a URI of that
    // scheme in the Contact header field of a redirect response.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_SCHEMES,

    // Description:
    // This feature tag indicates the type of entity that is available at
    // this URI.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_ACTOR,

    // Description:
    // This feature tag indicates that the UA is a conference server, also
    // known as a focus, and will mix together the media for all calls to
    // the same URI.
    //
    // This feature tag is defined in RFC 3840.
    eSIPFEATURETAG_ISFOCUS,

    // Description:
    //  From RFC 3840:
    //
    //  <CODE>
    //  For the base tags - that is, those feature tags documented in this
    //  specification (sip.audio, sip.automata, sip.class, sip.duplex,
    //  sip.data, sip.control, sip.mobility, sip.description, sip.events,
    //  sip.priority, sip.methods, sip.extensions, sip.schemes,
    //  sip.application, sip.video, <B>language</B>, type, sip.isfocus, sip.actor
    //  and sip.text), the leading "sip.", if present, is stripped.
    //  </CODE>
    eSIPFEATURETAG_LANGUAGE,

    // Description:
    //  From RFC 3840:
    //
    //  <CODE>
    //  For the base tags - that is, those feature tags documented in this
    //  specification (sip.audio, sip.automata, sip.class, sip.duplex,
    //  sip.data, sip.control, sip.mobility, sip.description, sip.events,
    //  sip.priority, sip.methods, sip.extensions, sip.schemes,
    //  sip.application, sip.video, language, <B>type</B>, sip.isfocus, sip.actor
    //  and sip.text), the leading "sip.", if present, is stripped.
    //  </CODE>
    eSIPFEATURETAG_TYPE,

    // Description:
    // The feature tag is a boolean flag. When set, it indicates that the
    // device is incapable of terminating a session autonomously.
    //
    // This feature tag is defined in RFC 4235.
    eSIPFEATURETAG_BYELESS,

    // Description:
    // This feature tag contains one of three tokens indicating if the
    // device is rendering any media from the current session ("yes"), none
    // of the media from the current session ("no"), or if this status is
    // not known to the device ("unknown").
    //
    // This feature tag is defined in RFC 4235.
    eSIPFEATURETAG_RENDERING,

    // Description:
    // This feature tag indicates that the device supports message as a
    // streaming media type.
    //
    // This feature tag is defined in RFC 4569.
    eSIPFEATURETAG_MESSAGE,

    // Description:
    // This feature tag uniquely indentifies a specific UA instance. 
    //
    // This feature tag is defined in RFC5626.
    eSIPFEATURETAG_INSTANCE,

    //M5T_INTERNAL_USE_BEGIN
    // This is where extensions to this enum are added.
    MXD_EXTEND_TYPE_FEATURE_TAG

    eSIPFEATURETAG_LAST,     // Last Item

    // This is the base value for unknown tags that are imported in from a param
    // list. Unknown tags will be added with an ID starting at this value and
    // incrementing by one each time a new unknown tag is added.
    eSIPFEATURETAG_UNKNOWN

    //M5T_INTERNAL_USE_END
};

//M5T_INTERNAL_USE_BEGIN

// Summary:
//  Initializes the Sip feature tags.
mxt_result InitializeSipFeatureTags();

// Summary:
//  Finalizes the Sip feature tags.
void FinalizeSipFeatureTags();


extern const char* const g_aszSIPFEATURETAG[];
extern CVector<CString>* g_pvecstrSIPFEATURETAG;

//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SIPOPTIONTAGS_H

