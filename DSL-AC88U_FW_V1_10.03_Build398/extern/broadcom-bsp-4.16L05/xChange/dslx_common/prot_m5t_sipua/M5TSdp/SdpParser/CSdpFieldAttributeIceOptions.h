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
#ifndef MXG_CSDPFIELDATTRIBUTEICEOPTIONS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEICEOPTIONS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h"
#endif

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CBlob;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CSdpFieldAttributeIceOptions
//==============================================================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   Implements the ice-options attribute.
//
// Description:
//   This class is an abstraction of an ice-options. It follows the
//   BNF notation described in the draft-ietf-mmusic-ice-19.
//
// From draft-ietf-mmusic-ice-19:
// <PRE>
//   ice-options           = "ice-options" ":" ice-option-tag
//                             0*(SP ice-option-tag)
//   ice-option-tag        = 1*ice-char
// </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeIceOptions.h
//
//==============================================================================
class CSdpFieldAttributeIceOptions : public CSdpParser
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeIceOptions();

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeIceOptions();

    // Summary: Copy Constructor.
    CSdpFieldAttributeIceOptions(IN const CSdpFieldAttributeIceOptions& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributeIceOptions& operator=(IN const CSdpFieldAttributeIceOptions& rSrc);

    // Summary: Comparison Operator.
    bool operator==(IN const CSdpFieldAttributeIceOptions& rFrom) const;


    //-- << CSdpParser interface >>
    // Summary:
    //   Parses all the needed information for this field.
    virtual EParserResult Parse(INOUT const char*& rpszStartPosition,
                                OUT mxt_result& rres);

    // Summary:
    //  Returns true if data members are valid.
    virtual bool Validate();

    // Summary:
    //  Resets this object.
    virtual void Reset();

    //-- << Stereotype >>

    // Summary:
    //  Serializes the value into the blob.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Gets the ice-options tag list.
    CList<CString>& GetOptionTagsList();
    const CList<CString>& GetOptionTagsList() const;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Data Members
private:
    CList<CString> m_lststrIceOptionTag;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetOptionTagsList
//==
//==============================================================================
//
//  Returns:
//      The list of ICE options tag.
//
//  Description:
//      Returns the list of ICE options tag.
//
//==============================================================================
inline CList<CString>& CSdpFieldAttributeIceOptions::GetOptionTagsList()
{
    return m_lststrIceOptionTag;
}

//==============================================================================
//==
//==  GetOptionTagsList
//==
//==============================================================================
//
//  Returns:
//      The list of ICE options tag.
//
//  Description:
//      Returns the list of ICE options tag.
//
//==============================================================================
inline const CList<CString>& CSdpFieldAttributeIceOptions::GetOptionTagsList() const
{
    return m_lststrIceOptionTag;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTEICEOPTIONS_H

