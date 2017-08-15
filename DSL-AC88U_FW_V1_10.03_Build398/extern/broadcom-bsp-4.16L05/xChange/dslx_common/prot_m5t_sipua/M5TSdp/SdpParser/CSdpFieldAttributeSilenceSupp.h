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
#ifndef MXG_CSDPFIELDATTRIBUTESILENCESUPP_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTESILENCESUPP_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h"  // MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT
#endif

#if defined (MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT)

//-- Data Members
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

//-- Parameters used by Value


//-- Interface Realized and/or Parent
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
//==  Class: CSdpFieldAttributeSilenceSupp
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements the parsing and serializing of the silence
//   suppression attribute.
//
// Description:
//   This class is used to handle the parsing and serializing of the silence
//   suppression attribute defined in RFC 3108.
//
//   The attribute follows the following ABNF:
//   a=silenceSupp: \<silenceSuppEnable\> \<silenceTimer\> \<suppPref\>
//                  \<sidUse\> \<fxnslevel\>
//
//   The validation of this attribute is done only on the on/off part of the
//   attriute. The rest of the validation is done by the application itself.
//
// Location:
//   SdpParser/CSdpFieldAttributeSilenceSupp.h
//
//==============================================================================
class CSdpFieldAttributeSilenceSupp : public CSdpParser
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeSilenceSupp();

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeSilenceSupp();

    // Summary: Copy Constructor.
    CSdpFieldAttributeSilenceSupp(IN const CSdpFieldAttributeSilenceSupp& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributeSilenceSupp& operator=(IN const CSdpFieldAttributeSilenceSupp& rSrc);

    //  Summary:
    //   Comparison operator.
    bool operator==(IN const CSdpFieldAttributeSilenceSupp& rSrc) const;

    //-- << Public Methods >>

    // Summary:
    //  Parses the data. Can return any type of EParserResult.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    //  Serializes the data to a CBlob.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Validates and checks the validity of the parsed data.
    virtual bool Validate();

    // Summary:
    //  Resets the data in the parser.
    virtual void Reset();

    // Summary:
    // Verifies if the attribute is on.
    bool        IsOn() const;

    // Summary:
    //  Gets the attribute string.
    const char* GetValue() const;

    // Summary:
    // Sets the attribute on or off.
    void        SetValue(IN bool bOn);

    // Summary:
    //  Sets the attribute to the specified string.
    void        SetValue(IN const char* szValue);


//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:


//-- Hidden Data Members
protected:
    CString m_strValue;
    bool m_bIsOn;
private:

//M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  IsOn
//==
//==============================================================================
//
//  Returns:
//    True if the first two characters of the string are equal to "on", false
//    otherwise.
//
//  Description:
//      Checks if the silence suppression attribute is supported.
//
//==============================================================================
inline bool CSdpFieldAttributeSilenceSupp::IsOn() const
{
    return m_bIsOn;
}

//==============================================================================
//==
//==  SetValue
//==
//==============================================================================
//
//  Parameters:
//    szValue:
//      The string to set to the attribute value.
//
//  Description:
//      Sets the string to the attribute. If the string does not begin with
//      either "on" or "off", the attribute is invalid. The rest of the
//      string is considered free text.
//
//==============================================================================
inline
void CSdpFieldAttributeSilenceSupp::SetValue(IN const char* szValue)
{
    m_strValue = szValue;
    Validate();
}

//==============================================================================
//==
//==  GetValue
//==
//==============================================================================
//
//  Returns:
//    szValue:
//      The string set to the attribute value.
//
//  Description:
//      Gets the string set in the attribute.
//
//==============================================================================
inline
const char* CSdpFieldAttributeSilenceSupp::GetValue() const
{
    return m_strValue.CStr();
}

//==============================================================================
//==
//==  Reset
//==
//==============================================================================
//
//  Description:
//      Resets the data members.
//
//==============================================================================
inline
void CSdpFieldAttributeSilenceSupp::Reset()
{
    CSdpParser::Reset();
    m_strValue.EraseAll();
    m_bIsValid = false;
    m_bIsOn = false;
}


MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined (MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTESILENCESUPP_H

