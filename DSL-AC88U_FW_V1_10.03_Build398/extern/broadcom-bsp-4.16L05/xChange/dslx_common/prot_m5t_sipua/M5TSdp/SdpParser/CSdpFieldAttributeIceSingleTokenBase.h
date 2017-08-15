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
#ifndef MXG_CSDPFIELDATTRIBUTEICESINGLETOKENBASE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEICESINGLETOKENBASE_H

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
//==  Class: CSdpFieldAttributeIceSingleTokenBase
//==============================================================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   Base class for single token ICE attribute.
//
// Description:
//   This base class is an abstraction of some of single token field of
//   ICE. It follows the BNF notation described in the draft-ietf-mmusic-ice-19.
//
// Location:
//   SdpParser/CSdpFieldAttributeIceSingleTokenBase.h
//
//==============================================================================
class CSdpFieldAttributeIceSingleTokenBase : public CSdpParser
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeIceSingleTokenBase();

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeIceSingleTokenBase();

    // Summary: Copy Constructor.
    CSdpFieldAttributeIceSingleTokenBase(IN const CSdpFieldAttributeIceSingleTokenBase& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributeIceSingleTokenBase& operator=(IN const CSdpFieldAttributeIceSingleTokenBase& rSrc);

    // Summary: Comparison Operator.
    bool operator==(IN const CSdpFieldAttributeIceSingleTokenBase& rFrom) const;

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
    virtual void Serialize(INOUT CBlob& rBlob) const;

//M5T_INTERNAL_USE_BEGIN
protected:
    // Summary:
    //  Gets the value.
    const char* GetValue() const;

    // Summary:
    //  Sets the value.
    void SetValue(IN const char* pszUserFrag);


//-- Hidden Data Members

private:
    CString m_strValue;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetValue
//==
//==============================================================================
//
//  Returns:
//      The user fragment.
//
//  Description:
//      Returns the user fragment.
//
//==============================================================================
inline const char* CSdpFieldAttributeIceSingleTokenBase::GetValue() const
{
    return m_strValue.CStr();
}

//==============================================================================
//==
//==  SetValue
//==
//==============================================================================
//
//  Parameters:
//      pszValue:
//       The user fragment to set.
//
//  Description:
//      Sets the user fragment.
//
//==============================================================================
inline void CSdpFieldAttributeIceSingleTokenBase::SetValue(IN const char* pszValue)
{
    m_strValue = pszValue;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTEICESINGLETOKENBASE_H

