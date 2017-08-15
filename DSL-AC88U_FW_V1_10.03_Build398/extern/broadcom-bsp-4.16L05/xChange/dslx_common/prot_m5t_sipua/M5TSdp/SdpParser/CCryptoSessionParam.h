//==============================================================================
//==============================================================================
//
//   Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CCRYPTOSESSIONPARAM_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCRYPTOSESSIONPARAM_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


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
//==  Class: CCryptoSessionParam
//=========================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//      This class implements session parameters.
//
// Description:
//      This class implements session parameters, separated by space.
//      The ABNF of the draft-ietf-mmusic-sdescriptions-09 have been simplified.
//      No validation is made on the multiple possibilities of parameters
//      as specified in the draft. This is the responsibility of the users to
//      validate the parameters.
//
//  <CODE>
//      draft-ietf-mmusic-sdescriptions-09 ABNF:
//
//          token [ EQUAL gen-value ]
//          token = 1*(VCHAR)  ;visible chars [RFC2234]
//          gen-value = 1*(VCHAR)  ;visible chars [RFC2234]
//  </CODE>
//
// Location:
//   SdpParser/CCryptoSessionParam.h
//
// See Also:
//
//==============================================================================
class CCryptoSessionParam : public CSdpParser
{
//-- Published Interface

public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CCryptoSessionParam();

    // Summary: Copy constructor.
    CCryptoSessionParam(IN const CCryptoSessionParam& rCryptoSessionParam);

    // Summary: Destructor.
    ~CCryptoSessionParam();

    // Summary: Assignment operator.
    CCryptoSessionParam& operator=(IN const CCryptoSessionParam& rFrom);

    //  Summary:
    //    Comparison operator.
    bool operator==(IN const CCryptoSessionParam& rFrom) const;

    //-- << Public methods >>

    //-- << Stereotype >>

    // Summary:
    //   Gets the name of the key.
    const char*  GetName() const;

    // Summary:
    //   Sets the name of the key.
    void SetName(IN const char* pszKey);

    // Summary:
    //   Gets the value of the key.
    const char*  GetValue() const;

    // Summary:
    //   Sets the value of the key.
    void SetValue(IN const char* pszValue);

    // Summary:
    //  Parses the parameters list beginning at rpszStartPosition. cSeparator is the
    //  character that separates the parameters.
    EParserResult Parse(INOUT const char*& rpszStartPosition, OUT mxt_result& rres);

    // Summary:
    //  Inserts cSeparator after each parameter, except for last parameter.
    const void Serialize(CBlob& rBlob);

    // Summary:
    //  Returns true if data members are valid
    bool Validate();

    // Summary:
    //  Resets this object.
    void Reset();

//M5T_INTERNAL_USE_BEGIN
private:
    CString m_strName;
    CString m_strValue;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetName
//==
//==============================================================================
//
//  Returns:
//      The Name of the key.
//
//  Description:
//      Returns the Name of the key.
//
//==============================================================================
inline const char* CCryptoSessionParam::GetName() const
{
    return m_strName.CStr();
}


//==============================================================================
//==
//==  SetName
//==
//==============================================================================
//
//  Parameters:
//    pszKey:
//      Name of the key.
//
//  Returns:
//      None.
//
//  Description:
//      This method sets the Name of the key.
//
//==============================================================================
inline void CCryptoSessionParam::SetName(IN const char* pszKey)
{
    m_strName = pszKey;
}

//==============================================================================
//==
//==  GetValue
//==
//==============================================================================
//
//  Returns:
//      The Value of the key.
//
//  Description:
//      Returns the Value of the key.
//
//==============================================================================
inline const char* CCryptoSessionParam::GetValue() const
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
//    pszValue:
//      Value of the key.
//
//  Returns:
//      None.
//
//  Description:
//      This method sets the Value of the key.
//
//==============================================================================
inline void CCryptoSessionParam::SetValue(IN const char* pszValue)
{
    m_strValue = pszValue;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //#ifndef MXG_CCRYPTOSESSIONPARAM_H
