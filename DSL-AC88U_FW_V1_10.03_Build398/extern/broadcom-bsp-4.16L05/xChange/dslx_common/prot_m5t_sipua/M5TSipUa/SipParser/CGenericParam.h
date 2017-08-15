//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CGENERICPARAM_H
#define MXG_CGENERICPARAM_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CTOKEN_H
#include "SipParser/CToken.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
// Class: CGenericParam
//=======================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   This class abstracts all SIP parameters. Currently three charsets are
//   supported: eCS_SIPHEADER_PARAM, eCS_SIPURI_PARAM, and eCS_TELURI_PARAM.
//
//   The parameters are implemented by using CTokens. The name part of the all
//   parameters types is always a token, and the value part is either a token, a
//   host, and in the case of eCS_SIP_HEADER, may be a quoted string.
//
//   Note that the name and value members are tokens and should be mapped to the
//   same character set. Otherwise, the behaviour is undefined.
//
//   Refer to the CToken documentation for an overview of the charsets.
//
//  <CODE>
//      RFC 3261 ABNF
//          For eCS_SIP_HEADER:
//          generic-param  =  token [ EQUAL gen-value ]
//          gen-value      =  token / host / quoted-string
//
//          For eCS_SIPURI_PARAM:
//          other-param       =  pname [ "=" pvalue ]
//          pname             =  1*paramchar
//          pvalue            =  1*paramchar
//
//          For eCS_TELURI_PARAM:
//          par                  = parameter / extension / isdn-subaddress
//          isdn-subaddress      = ";isub=" 1*uric
//          extension            = ";ext=" 1*phonedigit
//          context              = ";phone-context=" descriptor
//          parameter            = ";" pname ["=" pvalue ]
//          pname                = 1*( alphanum / "-" )
//          pvalue               = 1*paramchar
//          paramchar            = param-unreserved / unreserved / pct-encoded
//          unreserved           = alphanum / mark
//          mark                 = "-" / "_" / "." / "!" / "~" / "*" /
//                                 "'" / "(" / ")"
//          pct-encoded          = "%" HEXDIG HEXDIG
//          param-unreserved     = "[" / "]" / "/" / ":" / "&" / "+" / "$"
//          phonedigit           = DIGIT / [ visual-separator ]
//          phonedigit-hex       = HEXDIG / "*" / "#" / [ visual-separator ]
//          visual-separator     = "-" / "." / "(" / ")"
//          alphanum             = ALPHA / DIGIT
//          reserved             = ";" / "/" / "?" / ":" / "@" / "&" /
//                                 "=" / "+" / "$" / ","
//          uric                 = reserved / unreserved / pct-encoded
//
//  </CODE>
//
// Location:
//   SipParser/CGenericParam.h
//
// See Also:
//   CToken
//
//==============================================================================
class CGenericParam
{
//-- New types definition
//-----------------------
public:
    // Description:
    //  Indicate which character set to use for the CGenericParam.
    enum ECharSet
    {
        // Description:
        //  See CToken::ECharSet
        eCS_SIP_HEADER = CToken::eCS_SIPHEADER_PARAM,
        // Description:
        //  See CToken::ECharSet
        eCS_SIPURI_PARAM = CToken::eCS_SIPURI_PARAM,
        // Description:
        //  See CToken::ECharSet
        eCS_TELURI_PARAM = CToken::eCS_TELURI_PARAM
    };

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CGenericParam(IN ECharSet eCharSet);

    // Summary:
    //  Constructor.
    //---------------
    CGenericParam( IN ECharSet eCharSet,
                   IN const CString& rstrName,
                   IN const CString& rstrValue );

    // Summary:
    //  Constructor.
    //---------------
    CGenericParam(IN const CToken& rTokName, IN const CToken& rTokValue);

    // Summary:
    //  Constructor.
    //---------------
    CGenericParam(IN const CGenericParam& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CGenericParam();

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CGenericParam& operator=(IN const CGenericParam& rSrc);

    // Summary:
    //  Comparison Operator.
    //-----------------------
    bool operator==(IN const CGenericParam& rSrc) const;

    // Summary:
    //  Inequality Operator.
    //-----------------------
    bool operator!=(IN const CGenericParam& rSrc) const;


    //-- << Public methods >>
    //--------------------------------------------

    // Implementation Note: The non-const versions are acceptable as long as
    // the charset cannot be changed during the lifetime of a CToken, otherwise
    // we may end up with a m_tokName and m_tokValue token that don't have the
    // same charset.
    //--------------------------------------------------------------------------

    // Summary:
    //  Provides access to the parameter's name.
    //-------------------------------------------
    const CToken& GetName() const;

    // <COMBINE CGenericParam::GetName@const>
    CToken& GetName();

    // Summary:
    //  Provides access to the parameter's value.
    //--------------------------------------------
    const CToken& GetValue() const;

    // <COMBINE CGenericParam::GetValue@const>
    CToken& GetValue();

    // Summary:
    //  Parses the parameter, including name and optional value.
    //-----------------------------------------------------------
    mxt_result Parse(INOUT const char*& rpcPos);

    // Summary:
    //  Resets this object.
    //----------------------
    void Reset();

    // Summary:
    //  Outputs the parameter. It outputs the value if non-empty. It outputs
    //  the escaped characters if the token type supports it.
    //-----------------------------------------------------------------------
    void Serialize(INOUT CBlob& rBlob) const;

    //-- << Targetted helpers >>
    //--------------------------------------------

    // Summary:
    //  Returns true if the parameter's value starts with the branch magic
    //  cookie.
    //---------------------------------------------------------------------
    bool ViaBranchStartsWithMagicCookie() const;


//-- Hidden Methods
//-------------------
protected:

private:
    // Summary:
    //  Default Constructor.
    //-----------------------
    CGenericParam();

//-- Hidden Data Members
//------------------------
protected:
    // Name of the parameter.
    //------------------------
    CToken m_tokName;

    // Optional value of this parameter. If the value type is quoted string,
    // the quotes are included in this string. This member also handles the
    // token and host forms of the parameter value.
    //-----------------------------------------------------------------------
    CToken m_tokValue;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetName
//==
//==============================================================================
//
//  Returns:
//      Name of the parameter.
//
//  Description:
//      Provides access to the parameter name.
//
//  See Also:
//      GetValue
//
//==EDOC========================================================================
inline CToken& CGenericParam::GetName()
{
    return m_tokName;
}

//==SDOC========================================================================
//==
//==  GetName
//==
//==============================================================================
//
//  Returns:
//      Name of the parameter.
//
//  Description:
//      Provides access to the parameter name.
//
//  See Also:
//      GetValue
//
//==EDOC========================================================================
inline const CToken& CGenericParam::GetName() const
{
    return m_tokName;
}

//==SDOC========================================================================
//==
//==  GetValue
//==
//==============================================================================
//
//  Returns:
//      Value of the parameter.
//
//  Description:
//      Provides access to the parameter value.
//
//  See Also:
//      GetName
//
//==EDOC========================================================================
inline CToken& CGenericParam::GetValue()
{
    return m_tokValue;
}

//==SDOC========================================================================
//==
//==  GetValue
//==
//==============================================================================
//
//  Returns:
//      Value of the parameter.
//
//  Description:
//      Provides access to the parameter value.
//
//  See Also:
//      GetName
//
//==EDOC========================================================================
inline const CToken& CGenericParam::GetValue() const
{
    return m_tokValue;
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CGENERICPARAM_H
