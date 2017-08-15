//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CTELURI_H
#define MXG_CTELURI_H

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

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IURI_H
#include "SipParser/IUri.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;
class CGenParamList;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CTelUri
//======================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   The CTelUri class is used to store, parse, and serialize tel URI.
//
//   A tel URI is made up of a global-number or local-number and a parameter
//   list. A local-number MUST include a phone-context parameter.
//
//  <CODE>
//    RFC 3966:
//        telephone-uri        = "tel:" telephone-subscriber
//        telephone-subscriber = global-number / local-number
//        global-number        = global-number-digits *par
//        local-number         = local-number-digits *par context *par
//        par                  = parameter / extension / isdn-subaddress
//        isdn-subaddress      = ";isub=" 1*uric
//        extension            = ";ext=" 1*phonedigit
//        context              = ";phone-context=" descriptor
//        descriptor           = domainname / global-number-digits
//        global-number-digits = "+" *phonedigit DIGIT *phonedigit
//        local-number-digits  =
//           *phonedigit-hex (HEXDIG / "*" / "#")*phonedigit-hex
//        domainname           = *( domainlabel "." ) toplabel [ "." ]
//        domainlabel          = alphanum
//                               / alphanum *( alphanum / "-" ) alphanum
//        toplabel             = ALPHA / ALPHA *( alphanum / "-" ) alphanum
//        parameter            = ";" pname ["=" pvalue ]
//        pname                = 1*( alphanum / "-" )
//        pvalue               = 1*paramchar
//        paramchar            = param-unreserved / unreserved / pct-encoded
//        unreserved           = alphanum / mark
//        mark                 = "-" / "_" / "." / "!" / "~" / "*" /
//                               "'" / "(" / ")"
//        pct-encoded          = "%" HEXDIG HEXDIG
//        param-unreserved     = "[" / "]" / "/" / ":" / "&" / "+" / "$"
//        phonedigit           = DIGIT / [ visual-separator ]
//        phonedigit-hex       = HEXDIG / "*" / "#" / [ visual-separator ]
//        visual-separator     = "-" / "." / "(" / ")"
//        alphanum             = ALPHA / DIGIT
//        reserved             = ";" / "/" / "?" / ":" / "@" / "&" /
//                               "=" / "+" / "$" / ","
//        uric                 = reserved / unreserved / pct-encoded
//  </CODE>
//
//   WARNING: The 'isdn-subaddress' parameter (isub=) is not fully supported
//   in the actual implementation of the CTelUri. This is caused by the
//   'reserved' characters that can not be stored in the CToken of the
//   CGenericParam. The 'isdn-subaddress' parameter may only be used by filling
//   it with standard SipUri parameter characters. Moreover, the actual
//   implementation can not guarantee that the 'isdn-subaddress' or 'extension'
//   (ext=) parameters appear first before the 'context' (phone-context=)
//   parameter. See RFC 3966 Section 3 (URI Syntax) for more information.
//
// Location:
//   SipParser/CTelUri.h
//
// See Also:
//   IUri, CSipUri, CAbsoluteUri, CToken, CHostPort, CGenParamList,
//   CGenericParam
//
//==EDOC========================================================================
class CTelUri : public IUri
{
//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CTelUri();

    // Summary:
    //  Copy constructor.
    //--------------------
    CTelUri(IN const CTelUri& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CTelUri();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CTelUri& operator=(IN const CTelUri& rSrc);

    // Summary:
    //  Comparison operator. Compares as per RFC 3966 section 4.
    //-----------------------------------------------------------
    bool operator==(IN const CTelUri& rSrc) const;

    // Summary:
    //  Comparison operator. Compares as per RFC 3966 section 4.
    //-----------------------------------------------------------
    bool operator!=(IN const CTelUri& rSrc) const;

    //-- << Public Methods >>
    //--------------------------------------------

    // Summary:
    //  Returns true if the telephone-subscriber is a global phone number.
    //---------------------------------------------------------------------
    bool IsGlobalPhoneNumber() const;

    // Summary:
    //  Gets the phone number.
    //-------------------------
    const CToken* GetPhoneNumber() const;

    // <COMBINE CTelUri::GetPhoneNumber@const>
    //-----------------------------------------
    CToken* GetPhoneNumber();

    // Summary:
    //  Gets the phone context. (Only when IsGlobalPhoneNumber() == false)
    //---------------------------------------------------------------------
    const CToken* GetPhoneContext() const;

    // <COMBINE CTelUri::GetPhoneContext@const>
    //------------------------------------------
    CToken* GetPhoneContext();

    // Summary:
    //  Sets this URI with specified global phone number.
    //----------------------------------------------------
    mxt_result SetGlobalNumber(IN const CString& rstrPhoneNumber);


    // Summary:
    //  Sets this URI with specified local phone number and phone context.
    //---------------------------------------------------------------------
    mxt_result SetLocalNumber(IN const CString& rstrPhoneNumber,
                              IN const CString& rstrPhoneContext);

    // Summary:
    //  Provides access to the optional parameter list.
    //--------------------------------------------------
    const CGenParamList* GetParamList() const;

    // <COMBINE CTelUri::GetParamList@const>
    //--------------------------------------
    CGenParamList* GetParamList();

    // Summary:
    //  Sets the optional parameter list.
    //------------------------------------
    void SetParamList(IN TO CGenParamList* pParamList);


    //-- << IUri >>
    //--------------------------------------------

    // Summary:
    //  Generates a copy of this URI.
    //--------------------------------
    virtual GO IUri* GenerateCopy() const;

    // Summary:
    //  Returns the URI's scheme. In this case TEL.
    //----------------------------------------------
    virtual const char* GetScheme() const;

    // Summary:
    //  Returns the URI type, TEL.
    //-----------------------------
    virtual EUriType GetUriType() const;

    // Summary:
    //  Compares the given URI with this instance using applicable RFC rules.
    //  For this URI type, compares as per RFC3966 section 4 rules.
    //------------------------------------------------------------------------
    virtual bool IsEquivalent(IN const IUri& rSrc) const;

    // Summary:
    //  Parses a byte string into useable data.
    //------------------------------------------
    virtual mxt_result Parse(IN ESpecialCharactersAllowed eAllowSpecials,
                             INOUT const char*& rpcPos);

    // Summary:
    //  Reinitializes the instance.
    //------------------------------
    virtual void Reset();

    // Summary:
    //  Outputs the data member in a format that is ready to be sent on the
    //  network.
    //----------------------------------------------------------------------
    virtual void Serialize(INOUT CBlob& rBlob) const;

//-- Hidden Methods
//-------------------
protected:
private:

//-- Hidden Data Members
//------------------------

protected:

    // The 'telephone-subscriber' part of the URI indicates the number.
    //------------------------------------------------------------------
    CToken m_tokPhoneNumber;

    // 'uri-parameters' construct.
    //-----------------------------
    CGenParamList* m_pParamList;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//    rSrc:
//      Source against which to compare.
//
//  Returns:
//      True if the URIs are equivalent.
//
//  Description:
//      Comparison operator.
//
//  See Also:
//      operator!=, IsEquivalent
//
//==EDOC========================================================================
inline bool CTelUri::operator==(IN const CTelUri& rSrc) const
{
    return IsEquivalent(rSrc);
}

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rSrc:
//      Source against which to compare.
//
//  Returns:
//      True if the URIs are not equivalent.
//
//  Description:
//      Comparison operator.
//
//  See Also:
//      operator==, IsEquivalent
//
//==EDOC========================================================================
inline bool CTelUri::operator!=(IN const CTelUri& rSrc) const
{
    return !IsEquivalent(rSrc);
}

//==SDOC========================================================================
//==
//==  GetScheme
//==
//==============================================================================
//
//  Returns:
//      URI Scheme.
//
//  Description:
//      Returns the scheme TEL.
//
//==EDOC========================================================================
inline const char* CTelUri::GetScheme() const
{
    return szURISCHEME_TEL;
}

//==SDOC========================================================================
//==
//==  GetUriType
//==
//==============================================================================
//
//  Returns:
//      URI type.
//
//  Description:
//      Returns the TEL type.
//
//==EDOC========================================================================
inline IUri::EUriType CTelUri::GetUriType() const
{
    return IUri::eTEL;
}

//==SDOC========================================================================
//==
//==  IsGlobalPhoneNumber
//==
//==============================================================================
//
//  Returns:
//      True when the telephone-subscriber is a global phone number.
//
//  Description:
//      Checks if the telephone-subscriber is a global phone number.
//
//==EDOC========================================================================
inline bool CTelUri::IsGlobalPhoneNumber() const
{
    return m_tokPhoneNumber.GetString().GetSize() > 0 &&
           m_tokPhoneNumber.GetString().GetAt(0) == '+';
}

//==SDOC========================================================================
//==
//==  GetPhoneNumber
//==
//==============================================================================
//
//  Returns:
//      Pointer to the Phone number.
//
//  Description:
//      Provides access to the telephone-subscriber phone number.
//
//==EDOC========================================================================
inline const CToken* CTelUri::GetPhoneNumber() const
{
    return &m_tokPhoneNumber;
}

//==SDOC========================================================================
//==
//==  GetPhoneNumber
//==
//==============================================================================
//
//  Returns:
//      Pointer to the Phone number.
//
//  Description:
//      Provides access to the telephone-subscriber phone number.
//
//==EDOC========================================================================
inline CToken* CTelUri::GetPhoneNumber()
{
    return &m_tokPhoneNumber;
}

//==SDOC========================================================================
//==
//==  GetParamList
//==
//==============================================================================
//
//  Returns:
//      Pointer to the parameter list. It may be NULL.
//
//  Description:
//      Provides access to the parameter list.
//
//==EDOC========================================================================
inline CGenParamList* CTelUri::GetParamList()
{
    return m_pParamList;
}

//==SDOC========================================================================
//==
//==  GetParamList
//==
//==============================================================================
//
//  Returns:
//      Pointer to the parameter list. It may be NULL.
//
//  Description:
//      Provides access to the parameter list.
//
//==EDOC========================================================================
inline const CGenParamList* CTelUri::GetParamList() const
{
    return m_pParamList;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTELURI_H
