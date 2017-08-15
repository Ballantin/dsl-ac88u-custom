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
#ifndef MXG_SIPMEDIATYPES_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SIPMEDIATYPES_H
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
#include "Config/SceCoreCfg.h" // MXD_OVERRIDE_TYPE_MEDIA_TYPE
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
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

#if !defined(MXD_OVERRIDE_TYPE_MEDIA_TYPE) && defined(MXD_OVERRIDE_TYPE_MEDIA_TYPE_STRINGS)
#error Must synchronize type and strings.
#endif

#if defined(MXD_OVERRIDE_TYPE_MEDIA_TYPE) && !defined(MXD_OVERRIDE_TYPE_MEDIA_TYPE_STRINGS)
#error Must synchronize type and strings.
#endif


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if !defined (MXD_OVERRIDE_TYPE_MEDIA_TYPE)

//<GROUP TID_PACKAGE_SCECORE_STRUCTSENUMS>
// Summary:
//  Defines various media types most likely to be used along with SIP.
//
// Description:
//  This type defines currently registered media types at the IANA.
//
//  It is allowed to add non-IANA supported types, but such items must be
//  clearly indicated.
//
//  The list of all registered types can be found at:
//  http://www.iana.org/assignments/media-types/index.html.
//
//  This type must be ordered alphabetically without regards to case.
enum ESipMediaType
{
    //M5T_INTERNAL_USE_BEGIN
    eMEDIATYPE_FIRST                              = 0x00000001, // First
    //M5T_INTERNAL_USE_END

    eMEDIATYPE_APPLICATION_CONFERENCE_INFO_XML    = 0x00000001, // [RFC4575]
    eMEDIATYPE_APPLICATION_CPL_XML                = 0x00000002, // [RFC3880]
    eMEDIATYPE_APPLICATION_DIALOG_INFO_XML        = 0x00000004, // [RFC4235]
    eMEDIATYPE_APPLICATION_IM_ISCOMPOSING_XML     = 0x00000008, // [RFC3994]
    eMEDIATYPE_APPLICATION_KPML_REQUEST_XML       = 0x00000010, // [RFC4730]

    eMEDIATYPE_APPLICATION_KPML_RESPONSE_XML      = 0x00000020, // [RFC4730]
    eMEDIATYPE_APPLICATION_PIDF_XML               = 0x00000040, // [RFC3863]
    eMEDIATYPE_APPLICATION_PIDF_DIFF_XML          = 0x00000080, // [RFC5262]
    eMEDIATYPE_APPLICATION_PKCS10                 = 0x00000100, // [RFC2311]
    eMEDIATYPE_APPLICATION_PKCS7_MIME             = 0x00000200, // [RFC2311]

    eMEDIATYPE_APPLICATION_PKCS7_SIGNATURE        = 0x00000400, // [RFC2311]
    eMEDIATYPE_APPLICATION_POC_SETTINGS_XML       = 0x00000800, // [RFC4354]
    eMEDIATYPE_APPLICATION_REGINFO_XML            = 0x00001000, // [RFC3680]
    eMEDIATYPE_APPLICATION_RESOURCE_LISTS_XML     = 0x00002000, // [RFC4826]
    eMEDIATYPE_APPLICATION_RLMI_XML               = 0x00004000, // [RFC4662]

    eMEDIATYPE_APPLICATION_RLS_SERVICES_XML       = 0x00008000, // [RFC4826]
    eMEDIATYPE_APPLICATION_SDP                    = 0x00010000, // [RFC4566]
    eMEDIATYPE_APPLICATION_SIMPLE_FILTER_XML      = 0x00020000, // [RFC4661]
    eMEDIATYPE_APPLICATION_SIMPLE_MESSAGE_SUMMARY = 0x00040000, // [RFC3842]
    eMEDIATYPE_APPLICATION_WATCHERINFO_XML        = 0x00080000, // [RFC3858]

    eMEDIATYPE_APPLICATION_XCAP_DIFF_XML          = 0x00100000, // [draft-ietf-simple-xcap-diff-09]
    eMEDIATYPE_MESSAGE_CPIM                       = 0x00200000, // [RFC3862]
    eMEDIATYPE_MESSAGE_SIP                        = 0x00400000, // [RFC3261]
    eMEDIATYPE_MESSAGE_SIPFRAG                    = 0x00800000, // [RFC3420]
    eMEDIATYPE_MULTIPART_ALTERNATIVE              = 0x01000000, // [RFC2045,RFC2046]

    eMEDIATYPE_MULTIPART_MIXED                    = 0x02000000, // [RFC2045,RFC2046]
    eMEDIATYPE_MULTIPART_RELATED                  = 0x04000000, // [RFC2387]
    eMEDIATYPE_MULTIPART_SIGNED                   = 0x08000000, // [RFC1847]

    //M5T_INTERNAL_USE_BEGIN
    eMEDIATYPE_LAST                               = 0x10000000, // Last Item
    //M5T_INTERNAL_USE_END
};

#else
MXD_OVERRIDE_TYPE_MEDIA_TYPE
#endif

//M5T_INTERNAL_USE_BEGIN

// Summary:
//  Initializes the Sip Media Types.
//-----------------------------------
mxt_result InitializeSipMediaTypes();

// Summary:
//  Finalizes the Sip Media Types.
//---------------------------------
void FinalizeSipMediaTypes();

// Description:
//  This structure contains all the identifiers for a SIP media-type.
//  Identifiers are in a C-style string.
//--------------------------------------------------------------------
struct SSipMediaType
{
    // Description:
    //  The "m-type" part of the SIP media-type.
    //-------------------------------------------
    const char* m_szMType;

    // Description:
    //  The "m-subtype" part of the SIP media type.
    //----------------------------------------------
    const char* m_szMSubType;
};

// Description:
//  This structure contains all the identifiers for a SIP media-type.
//  Identifiers are in a CString.
//--------------------------------------------------------------------
struct SSipMediaTypeStr
{
    // Summary:
    //  Constructor.
    //---------------------------
    SSipMediaTypeStr(IN const char* const pszMType = "",
                     IN const char* const pszMSubType = "");

    // Description:
    //  The "m-type" part of the SIP media-type.
    //-------------------------------------------
    CString m_strMType;

    // Description:
    //  The "m-subtype" part of the SIP media type.
    //----------------------------------------------
    CString m_strMSubType;
};

extern const SSipMediaType g_astSIPMEDIATYPE[];
extern CVector<SSipMediaTypeStr>* g_pvecstSIPMEDIATYPE;

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SIPMEDIATYPES_H

