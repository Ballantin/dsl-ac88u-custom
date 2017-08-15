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
#ifndef MXG_CSDPFIELDATTRIBUTEMID_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEMID_H

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
//==  Class: CSdpFieldAttributeMid
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of the mid attribute.
//
// Description:
//   This class is an abstraction of the mid attribute in SDP.
//   The mid field attribute is used to identify a media stream within a session
//   description. It follows the BNF notation described in RFC 3388.
//
//   <PRE>
//    mid-attribute      = "a=mid:" identification-tag
//    identification-tag = token
//   </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeMid.h
//
//==============================================================================
class CSdpFieldAttributeMid : public CSdpParser
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeMid();

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeMid();

    // Summary: Copy Constructor.
    CSdpFieldAttributeMid(IN const CSdpFieldAttributeMid& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributeMid& operator=(IN const CSdpFieldAttributeMid& rSrc);

    // Summary: Comparison operator.
    bool operator==(IN const CSdpFieldAttributeMid& rFrom) const;

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
    //  Gets the value.
    const char* GetValue() const;

    // Summary:
    //  Serializes the value into the blob.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Sets the value.
    void SetValue(IN const char* szValue);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

//-- Hidden Data Members
protected:
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
//      The mid value.
//
//  Description:
//      Returns the mid value.
//
//==============================================================================
inline const char* CSdpFieldAttributeMid::GetValue() const
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
//    szValue:
//      The mid value to set.
//
//  Description:
//      Sets the mid value.
//
//==============================================================================
inline void CSdpFieldAttributeMid::SetValue(IN const char* szValue)
{
    m_strValue = szValue;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTEMID_H

