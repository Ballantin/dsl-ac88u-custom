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
#ifndef MXG_CSCESIPFEATURESET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCESIPFEATURESET_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPFEATURETAGS_H
#include "SceSipTypes/SipFeatureTags.h"
#endif

#ifndef MXG_CTOKEN_H
#include "SipParser/CToken.h"
#endif

#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CGenParamList;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceSipFeatureSet
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This class manages a list of SIP features tag and their values as defined in
//   RFC 3840.
//
// Description:
//  This class manages a list of SIP features tag and their values as defined in
//  RFC 3840.
//
//  The following example is used to highlight what is a feature set, a feature
//  tag and a feature tag value.
//
//  <CODE>
//  ;+featuretag1="a,b,c,d";+featuretag2;+featuretag3="\<Some String\>";+featuretag4="#1:12"
//  </CODE>
//
//      - +featuretag1, featuretag2, featuretag3, featuretag4 are all feature
//        tags.
//      - "a,b,c,d", "\<Some String\>" and "#1:12" are feature tag values.
//        +featuretag2 has an implicit value of "TRUE".
//      - The sum of all feature tags and their values is the feature set.
//
//  This class allows to set boolean feature tags, token feature tags, string
//  feature tags and numerical feature tags.
//
//  In the following examples, the tags are presented as they would be encoded
//  within a parameter list which would be part of a Contact, Accept-Contact or
//  Reject-Contact header.
//
//#Boolean Feature Tags Examples#
//  <CODE>
//  ;audio;video;+newtag;message="FALSE"
//  </CODE>
//
//  If the feature is set as supported, only the feature name will be set in the
//  parameter list. If the feature is set as not being supported, the feature
//  value is set to "FALSE".
//
//  Boolean feature tags can be set only once, as the application either
//  supports it or not.
//
//#Token Feature Tags Examples#
//  <CODE>
//  ;methods="INVITE,BYE,CANCEL,ACK";events="!presence,dialog";+sometag="abc,!def"
//  </CODE>
//
//  If the feature tag parameter is set as being supported, it is simply set in
//  the list of values associated with the feature tag. If it is not supported,
//  it is prefixed with a "!" and added to the list of values associated with
//  the feature tag.
//
//  Multiple tokens can be assigned to a feature tag that is defined as a list
//  of token.
//
//
//#String Feature Tags#
//  <CODE>
//  ;description="\<My home PC\>";+sometag="\<Some other string\>"
//  </CODE>
//
//  Only a single string can be associated with a feature tag. Note that the
//  CSceSipFeatureSet will automatically manage the "<" and ">" characters that
//  must enclose a string.
//
//#Numerical Feature Tags#
//  <CODE>
//      ;+rangetag="#1.25:10.75";+operatortag="#<=125";+newtag="#=455"
//  </CODE>
//
//  Multiple numerical values can be associated with a single feature tag.
//
//#Note on feature tags names#
//  All feature tags defined in RFC 3840 are encoded without any special
//  prefixes. As an example: ;audio;class="business"
//
//  Other feature tags are always prefixed with a "+" character. As an example:
//  <CODE>
//  ;+sometag="token";+sip.byeless
//  </CODE>
//
//  Furthermore, the eSIPTAG_BYELESS, eSIPTAG_RENDERING and eSIPTAG_MESSAGE are
//  all prefixed by a "+sip.".
//
// Location:
//   SipParser/CSceSipFeatureSet.h
//
//==============================================================================
class CSceSipFeatureSet
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceSipFeatureSet();

    // Summary:
    //  Destructor.
    virtual ~CSceSipFeatureSet();

    // Summary:
    //  Copy Constructor.
    CSceSipFeatureSet(IN const CSceSipFeatureSet& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceSipFeatureSet& operator=(IN const CSceSipFeatureSet& rSrc);

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   The potential comparison operators possible to use.
    //
    // Description:
    //  This enumeration defines potential comparison operators used when
    //  expressing preferences or capabilities associated with numerical values.
    //
    //-- Note to developpers:
    //--   Make sure to keep the gs_aszOPERATORS array in sync with this
    //--   enumeration.
    //==========================================================================
    enum ETagOperator
    {
        // Description:
        // Equal operator, which will be output as tagname="#=value".
        eOPERATOR_EQUAL,

        // Description:
        // Equal or smaller than operator, which will be output as
        // tagname="#<=value".
        eOPERATOR_SMALLER_OR_EQUAL,

        // Description:
        // Equal or greater than operator, which will be output as
        // tagname="#>=value".
        eOPERATOR_GREATER_OR_EQUAL
    };

    // Summary:
    //  Adds a feature tag with a boolean value to the feature set.
    mxt_result AddFeatureTag(IN ESipFeatureTag eTag,
                             IN bool bSupported);

    // Summary:
    //  Adds a feature tag with a token value to the feature set.
    mxt_result AddFeatureTag(IN ESipFeatureTag eTag,
                             IN const CToken& rToken,
                             IN bool bSupported);

    // Summary:
    //  Adds a feature tag with a string value to the feature set.
    mxt_result AddFeatureTag(IN ESipFeatureTag eTag,
                             IN const CString& rstrString);

    // Summary:
    //  Adds a feature tag with a numerical range value to the feature set.
    mxt_result AddFeatureTag(IN ESipFeatureTag eTag,
                             IN const CToken& rRangeStart,
                             IN const CToken& rRangeStop);

    // Summary:
    //  Adds a feature tag with an operator and numerical value to the feature
    //  set.
    mxt_result AddFeatureTag(IN ESipFeatureTag eTag,
                             IN ETagOperator eOperator,
                             IN const CToken& rNumber);

    // Summary:
    //  Removes a specific feature tag with its value.
    mxt_result RemoveFeatureTag(IN ESipFeatureTag eTag);

    // Summary:
    //  Removes a specific token value from a token feature tag.
    mxt_result RemoveTokenFromFeatureTag(IN ESipFeatureTag eTag,
                                         IN const CToken& rToken);

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   Return codes for the IsFeatureTagSupported method.
    //
    // Description:
    //  This enumeration is strictly used as a return code by all the
    //  IsFeatureTagSupported methods.
    //
    //==========================================================================
    enum EFeatureTagSupport
    {
        // Description:
        // The feature tag is supported.
        eSUPPORTED,

        // Description:
        // The feature tag was explicitly reported as unsupported.
        eUNSUPPORTED,

        // Description:
        // The feature tag was not found. It is thus unknown if it is supported
        // or not.
        eUNKNOWN
    };

    // Summary:
    //  Verifies if a boolean feature tag is supported by the feature set.
    EFeatureTagSupport IsFeatureTagSupported(IN ESipFeatureTag eTag) const;

    // Summary:
    //  Verifies if a token or numerical feature tag is supported by the
    //  feature set.
    EFeatureTagSupport IsFeatureTagSupported(IN ESipFeatureTag eTag,
                                             IN const CToken& rToken) const;

    // Summary:
    //  Verifies if a string feature tag is present and returns the found
    //  string in rString.
    EFeatureTagSupport IsFeatureTagSupported(IN ESipFeatureTag eTag,
                                             OUT CString& rString) const;

    // Summary:
    //  Verifies if there is a common match between two feature sets.
    unsigned int Match(IN const CSceSipFeatureSet& rFrom) const;

    // Summary:
    //  Verifies if this feature set contains all feature tags present in an
    //  other feature set.
    bool ContainsAllFeatureTags(IN const CSceSipFeatureSet& rFeatureSet) const;

    // Summary:
    //  Imports the feature tags found in a SIP parameter list.
    mxt_result ImportFeatureTags(IN const CGenParamList& rParamList);

    // Summary:
    //  Exports the configured feature tags to a SIP parameter list.
    mxt_result ExportFeatureTags(INOUT CGenParamList& rParamList) const;

    // Summary:
    //  Sets the ;require parameter in the feature set.
    void SetRequire(IN bool bRequire = true);

    // Summary:
    //  Sets the ;explicit parameter in the feature set.
    void SetExplicit(IN bool bExplicit = true);

    // Summary:
    //  Gets whether or not the require flag is set in the feature set.
    bool GetRequire() const;

    // Summary:
    //  Gets whether or not the explicit flag is set in the feature set.
    bool GetExplicit() const;


//-- Hidden Methods
protected:
private:

    //M5T_INTERNAL_USE_BEGIN
    // Description:
    //    Defines the data types of a feature tag.
    enum EFeatureTagDataType
    {
        // Description:
        //    The data of the feature tag is boolean (i.e. TRUE or FALSE).
        eSIPTAGTYPE_BOOLEAN,

        // Description:
        //    The data is a list of one or more tokens.
        eSIPTAGTYPE_TOKEN,

        // Description:
        //    The data is a free format string.
        eSIPTAGTYPE_STRING,

        // Description:
        //    The data of the feature tag is numeric (this includes operators and ranges).
        eSIPTAGTYPE_NUMERIC,
    };

    // Description:
    //    Defines a feature tag.
    struct SFeatureTag
    {
        // Description:
        //     The tag's associated data type.
        EFeatureTagDataType m_eDataType;

        // Description:
        //     The tag's support status.
        EFeatureTagSupport m_eTagSupported;

        // Description:
        //     The tag's name as it appears in a param list.
        CString m_strTagName;

        // Description:
        //     The tag's token list. Used for data types TOKEN, STRING and
        //     NUMERIC. Tokens are kept in CStrings to ease the list
        //     maintenance since CToken does not have a default constructor and
        //     would require to keep a list of pointers to heap allocated
        //     objects. This would complexify copying this structure.
        CList<CString> m_lststrValue;

        //-- Constructor to initialize the structure to default values.
        SFeatureTag();

        //-- Destructor to ensure proper destruction of the structure.
        ~SFeatureTag();
    };

    // This is needed to be able to compile with MSVC6 in which it is not
    // possible for a struct declared in a class to access the enumeration
    // values that are declared as private or protected in the same class.
    friend struct SFeatureTag;

    // Summary:
    //  Checks if a given tag exists in the feature set
    bool DoesTagExists(IN ESipFeatureTag eTag,
                       OUT const SFeatureTag** ppstTag = NULL) const;

    // Summary:
    //  Checks if a given tag is of the type specified.
    bool IsTagOfGivenType(IN ESipFeatureTag eTag,
                          IN EFeatureTagDataType eTagType) const;

    // Summary:
    //  This helper compares a given token to see if it is in the supported
    //  values.
    EFeatureTagSupport IsFeatureTagSupportedTokenHelper(IN const SFeatureTag*& rpstTag,
                                                        IN const CToken& rToken) const;

    // Summary:
    //  This helper compares a given numeric token to see if it is in the
    //  supported values, taking into accounts any inequality operators and
    //  ranges it encounters.
    EFeatureTagSupport IsFeatureTagSupportedNumericHelper(IN const SFeatureTag*& rpstTag,
                                                          IN const CToken& rToken) const;

    // Summary:
    //  Returns the ETagOperator value that corresponds to a given operator in
    //  string form.
    int DecodeOperator(IN const CString& rstrOperator) const;

    // Summary:
    //  Extracts the operator and value parts from a numeric token.
    bool ExtractOperatorAndValue(INOUT CToken& rNumericToken,
                                 OUT int& rnOperator,
                                 OUT CToken& rValueToken);

    // Summary:
    //  Extracts the range start and end value from a numeric token.
    bool ExtractRangeStartAndEnd(INOUT CToken& rNumericToken,
                                 OUT CToken& rRangeStart,
                                 OUT CToken& rRangeEnd);

    // Summary:
    //  Indicates whether two boolean feature tags match.
    bool MatchBooleanHelper(IN const SFeatureTag& rstTag1,
                            IN const SFeatureTag& rstTag2) const;

    // Summary:
    //  Indicates whether two string feature tags match.
    bool MatchStringHelper(IN const SFeatureTag& rstTag1,
                           IN const SFeatureTag& rstTag2) const;

    // Summary:
    //  Indicates whether two token feature tags match.
    bool MatchTokenHelper(IN const SFeatureTag& rstTag1,
                          IN const SFeatureTag& rstTag2) const;

    // Summary:
    //  Indicates whether two numeric feature tags match.
    bool MatchNumericHelper(IN const SFeatureTag& rstTag1,
                            IN const SFeatureTag& rstTag2) const;

//-- Hidden Data Members
protected:
private:

    // The ";require" parameter is set.
    bool m_bRequire;

    // The ";explicit" parameter is set.
    bool m_bExplicit;

    // Description:
    //     This map holds the content of the feature set.
    CMap<int, SFeatureTag> m_mapFeatureSet;

    //M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  SetRequire
//==
//==============================================================================
//
//  Parameters:
//      bRequire:
//          Indicates if the require parameter is set (true) or not (false).
//
//  Description:
//      Sets the require parameter in the feature set.
//
//      This parameter is defined in RFC 3841 and is used in the Accept-Contact
//      header. This parameter is used to force contacts that do not match the
//      Accept-Contact to be rejected from the potential list of destination
//      when applying the caller preferences algorithm.
//
//      When both the require and explicit parameter are present, it forces a
//      contact to have a 100% match against the Accept-Contact in order to be
//      considered in the target-set.
//
//  See Also:
//      GetRequire, SetExplicit
//
//==============================================================================
inline void CSceSipFeatureSet::SetRequire(IN bool bRequire)
{
    m_bRequire = bRequire;
}


//==============================================================================
//==
//==  SetExplicit
//==
//==============================================================================
//
//  Parameters:
//      bExplicit:
//          Indicates if the explicit parameter is set (true) or not (false).
//
//  Description:
//      Sets the explicit parameter in the feature set.
//
//      This parameter is defined in RFC 3841 and is used in the Accept-Contact
//      header. This parameter is used to force contacts that do not match the
//      Accept-Contact to be tried last over the ones that match the
//      Accept-Contact header.
//
//      When both the require and explicit parameter are present, it forces a
//      contact to have a 100% match against the Accept-Contact in order to be
//      considered in the target-set.
//
//  See Also:
//      GetExplicit, SetRequire
//
//==============================================================================
inline void CSceSipFeatureSet::SetExplicit(IN bool bExplicit)
{
    m_bExplicit = bExplicit;
}


//==============================================================================
//==
//==  GetRequire
//==
//==============================================================================
//
//  Returns:
//      True if the require parameter is set in the feature-set. False
//      otherwise.
//
//  Description:
//      Returns whether or not the require parameter is set in the feature set.
//
//  See Also:
//      SetRequire, GetExplicit
//
//==============================================================================
inline bool CSceSipFeatureSet::GetRequire() const
{
    return m_bRequire;
}


//==============================================================================
//==
//==  GetExplicit
//==
//==============================================================================
//
//  Returns:
//      True if the explicit parameter is set in the feature-set. False
//      otherwise.
//
//  Description:
//      Returns whether or not the explicit parameter is set in the feature set.
//
//  See Also:
//      SetExplicit, GetRequire
//
//==============================================================================
inline bool CSceSipFeatureSet::GetExplicit() const
{
    return m_bExplicit;
}


//==============================================================================
//==
//==  SFeatureTag
//==
//==============================================================================
//
//  Description:
//      Default constructor.
//
//==============================================================================
inline CSceSipFeatureSet::SFeatureTag::SFeatureTag()
:   m_eDataType(CSceSipFeatureSet::eSIPTAGTYPE_BOOLEAN),
    m_eTagSupported(eUNKNOWN)
{
}


//==============================================================================
//==
//==  ~SFeatureTag
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline CSceSipFeatureSet::SFeatureTag::~SFeatureTag()
{
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCESIPFEATURESET_H

