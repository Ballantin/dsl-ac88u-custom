//==============================================================================
//==============================================================================
//
//   Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSDPFIELDATTRIBUTEGROUP_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEGROUP_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
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
//==  Class: CSdpFieldAttributeGroup
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of the group attribute.
//
// Description:
//   This class is an abstraction of the group attribute in SDP.
//   The group field attribute is used to group together different media
//   streams. It follows the BNF notation described in RFC 3388 and RFC 4091.
//   As of now, only ANAT semantic is supported.
//
//   <PRE>
//        group-attribute    = "a=group:" semantics
//                             *(space identification-tag)
//        semantics          = "ANAT"
//   </PRE>
//
//
// Location:
//   SdpParser/CSdpFieldAttributeGroup.h
//
//==============================================================================
class CSdpFieldAttributeGroup : public CSdpParser
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeGroup();

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeGroup();

    // Summary: Copy Constructor.
    CSdpFieldAttributeGroup(IN const CSdpFieldAttributeGroup& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributeGroup& operator=(IN const CSdpFieldAttributeGroup& rSrc);

    // Summary: Comparison operator.
    bool operator==(IN const CSdpFieldAttributeGroup& rFrom) const;

    //-- << CSdpParser interface >>

    // Summary:
    //   Parses the data.
    virtual EParserResult Parse(INOUT const char*& rpszStartPosition,
                                OUT mxt_result& rres);

    // Summary:
    //   Validates the parsed data.
    virtual bool Validate();

    // Summary:
    //   Resets the data in the parser.
    virtual void Reset();

    //-- << Stereotype >>

    // Summary:
    //  Serializes the value into the blob.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Gets the Semantic value.
    const char* GetSemantic() const;

    // Summary:
    //  Gets the list of Identification in the group.
    CList<CString>& GetIdentificationList();

    // Summary:
    //  Gets the list of Identification in the group.
    const CList<CString>& GetIdentificationList() const;

    // Summary:
    //  Sets the Semantic value.
    void SetSemantic(IN const char* szValue);

    // Summary:
    //  Returns true if the Mid attribute is member of the group field.
    bool IsMember(IN const CString& strMid) const;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:

//-- Hidden Data Members
protected:
private:
    CString m_strSemantic;
    CList<CString> m_lststrId;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetSemantic
//==
//==============================================================================
//
//  Returns:
//      The semantic value.
//
//  Description:
//      Returns the semantic value.
//
//==============================================================================
inline const char* CSdpFieldAttributeGroup::GetSemantic() const
{
    return m_strSemantic.CStr();
}

//==============================================================================
//==
//==  SetSemantic
//==
//==============================================================================
//
//  Parameters:
//    szValue:
//      The semantic value to set.
//
//  Description:
//      Sets the semantic value.
//
//==============================================================================
inline void CSdpFieldAttributeGroup::SetSemantic(IN const char* szValue)
{
    m_strSemantic = szValue;
}

//==============================================================================
//==
//==  GetIdentificationList
//==
//==============================================================================
//
//  Returns:
//    A reference to the list of identification tags.
//
//  Description:
//    Returns a reference to the list of identification tags.
//
//==============================================================================
inline CList<CString>& CSdpFieldAttributeGroup::GetIdentificationList()
{
    return m_lststrId;
}

//==============================================================================
//==
//==  GetIdentificationList
//==
//==============================================================================
//
//  Returns:
//    A const reference to the list of identification tags.
//
//  Description:
//    Returns a const reference to the list of identification tags.
//
//==============================================================================
inline const CList<CString>& CSdpFieldAttributeGroup::GetIdentificationList() const
{
    return m_lststrId;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTEGROUP_H

