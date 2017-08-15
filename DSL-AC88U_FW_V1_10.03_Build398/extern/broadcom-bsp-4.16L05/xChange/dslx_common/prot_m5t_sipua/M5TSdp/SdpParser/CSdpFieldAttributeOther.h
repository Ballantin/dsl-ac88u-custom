//==============================================================================
//==============================================================================
//
//   Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSDPFIELDATTRIBUTEOTHER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEOTHER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SDP Parser Configuration
#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h"
#endif

//-- Data Member
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif



//-- Interface Realized & Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif



//-- Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)



//-- Forward Declarations Inside of the Namespace
class CBlob;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//==  Class: CSdpFieldAttributeOther
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of an attribute-other.
//
// Description:
//   This class is an abstraction of an attribute-other (other
//   than those that are recognized) in a SDP packet.  It follows the BNF
//   notation described in RFC 2327.
//
//  RFC 2327 BNF:
//  <PRE>
//      attribute-other    =     (att-field ":" att-value) | att-field
//      att-value          =     byte-string
//      att-field          =     1*(alpha-numeric)
//      byte-string        =     1*(0x01..0x09|0x0b|0x0c|0x0e..0xff)
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeOther.h
//
// See Also:
//
//==============================================================================
class CSdpFieldAttributeOther : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>
    // Summary: Default constructor.
    CSdpFieldAttributeOther();

    // Summary: Copy constructor.
    CSdpFieldAttributeOther(IN const CSdpFieldAttributeOther& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeOther();

    // Summary
    //  Comparison operator.
    bool operator==(IN const CSdpFieldAttributeOther& rFrom) const;

    // Summary: Assignment operator.
    CSdpFieldAttributeOther& operator=(IN const CSdpFieldAttributeOther& rFrom);

    //-- << Stereotype >>

    // Summary:
    //   Gets the name.
    const char* GetName() const;

    // Summary:
    //   Gets the value.
    const char* GetValue() const;

    // Summary:
    //   Generates the data blob from the data members.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //   Sets the name.
    void SetName(IN const char* pszName);

    // Summary:
    //   Sets the value.
    void SetValue(IN const char* pszValue);

    //-- Virtual functions
    // Summary:
    //  Parses all the needed information for this field.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    //   Validates the parsed data.
    bool Validate();

    // Summary:
    //   Resets the data in the parser.
    void Reset();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:

// Hidden Data Members
protected:
private:
    CString m_strName;
    CString m_strValue;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  SetName
//==
//==============================================================================
//
//  Parameters:
//    pszName:
//      The name of the attribute-other.
//
//  Description:
//    Sets the name of the attribute-other
//
//==============================================================================
inline void CSdpFieldAttributeOther::SetName(IN const char* pszName)
{
    m_strName = pszName;
}

//==============================================================================
//==
//==  GetName
//==
//==============================================================================
//
//  Returns:
//    The name of the attribute-other.
//
//  Description:
//    Gets the name of the attribute-other
//
//==============================================================================
inline const char* CSdpFieldAttributeOther::GetName() const
{
    return m_strName.CStr();
}

//==============================================================================
//==
//==  SetValue
//==
//==============================================================================
//
//  Parameters:
//    pszValue:
//      The value of the attribute-other.
//
//  Description:
//    Sets the value of the attribute-other
//
//==============================================================================
inline void CSdpFieldAttributeOther::SetValue(IN const char* pszValue)
{
    m_strValue = pszValue;
}

//==============================================================================
//==
//==  GetValue
//==
//==============================================================================
//
//  Returns:
//    The value of the attribute-other.
//
//  Description:
//    Gets the value of the attribute-other
//
//==============================================================================
inline const char* CSdpFieldAttributeOther::GetValue() const
{
    return m_strValue.CStr();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTEOTHER_H
