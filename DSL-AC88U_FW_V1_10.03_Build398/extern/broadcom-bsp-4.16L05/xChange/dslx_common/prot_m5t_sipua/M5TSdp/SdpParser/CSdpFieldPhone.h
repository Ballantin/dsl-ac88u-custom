//==============================================================================
//==============================================================================
//
//   Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSDPFIELDPHONE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDPHONE_H

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

// Data Member
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

// Interface Realized & Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif



// Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)


// Forward Declarations Inside of the Namespace
class CBlob;
//M5T_INTERNAL_USE_END


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//==  Class: CSdpFieldPhone
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a session-name-field.
//
// Description:
//   This class is an abstraction of a session-name-field in a SDP packet.
//
//   The "p=" field is the phone number.  It follows the BNF that is described
//   in RFC 2327 and is an optional item.
//
//  RFC 2327 BNF:
//   <PRE>
//      phone-fields =      *("p=" phone-number CRLF)
//      phone-number =      phone | phone "(" email-safe ")" |
//                          email-safe "<" phone ">"
//      phone        =      "+" POS-DIGIT 1*(space | "-" | DIGIT)
//                          ;there must be a space or hyphen between the
//                          ;international code and the rest of the number.
//      email-safe   =      safe | space | tab
//      safe         =      alpha-numeric |
//                          "'" | "'" | "-" | "." | "/" | ":" | "?" | """ |
//                          "#" | "$" | "&" | "*" | ";" | "=" | "@" | "[" |
//                          "]" | "^" | "_" | "`" | "{" | "|" | "}" | "+" |
//                          "~" | "
//      alpha-numeric =     ALPHA | DIGIT
//      ALPHA         =     "a"|"b"|"c"|"d"|"e"|"f"|"g"|"h"|"i"|"j"|"k"|
//                          "l"|"m"|"n"|"o "|"p"|"q"|"r"|"s"|"t"|"u"|"v"|
//                          "w"|"x"|"y"|"z"|"A"|"B"|"C "|"D"|"E"|"F"|"G"|
//                          "H"|"I"|"J"|"K"|"L"|"M"|"N"|"O"|"P"|" Q"|"R"|
//                          "S"|"T"|"U"|"V"|"W"|"X"|"Y"|"Z"
//      DIGIT        =      "0" | POS-DIGIT
//      POS-DIGIT    =      "1"|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9"
//
//   </PRE>
//
// Location:
//   SdpParser/CSdpFieldPhone.h
//
// See Also:
//
//==============================================================================
class CSdpFieldPhone : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldPhone();

    // Summary: Copy constructor.
    CSdpFieldPhone(IN const CSdpFieldPhone& from);

    // Summary: Destructor.
    virtual ~CSdpFieldPhone();

    // Summary: Comparison operator.
    bool operator==(IN const CSdpFieldPhone& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpFieldPhone& operator=(IN const CSdpFieldPhone& from);

    // Summary:
    //   Gets the phone number.
    const char* GetPhone() const;

    // Summary:
    //   Generates the data blob from the data members.
    void        Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //   Sets the phone number.
    void SetPhone(IN const char* szPhone);

    //-- Virtual functions

    // Summary:
    //   Parses all the needed information for this field.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    //   Checks the validity of the parsed data.
    bool Validate();

    // Summary:
    //   Resets all the data members.
    void Reset();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:

// Hidden Data Members
protected:
private:
    CString m_strPhone;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetPhone
//==
//==============================================================================
//
//  Returns:
//      The phone number.
//
//  Description:
//      Returns the phone number (i.e. the p= field).
//
//==============================================================================
inline const char* CSdpFieldPhone::GetPhone() const
{
    return m_strPhone.CStr();
}

//==============================================================================
//==
//==  SetPhone
//==
//==============================================================================
//
//  Parameters:
//    szPhone:
//      The phone number.
//
//  Description:
//      Sets the phone number (i.e. the p= field).
//
//==============================================================================
inline void CSdpFieldPhone::SetPhone(IN const char* szPhone)
{
    m_strPhone = szPhone;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDPHONE_H
