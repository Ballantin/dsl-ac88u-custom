//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CREQUESTLINE_H
#define MXG_CREQUESTLINE_H

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
#ifndef MXG_METHOD_H
#include "SipParser/Method.h" // ESipMethod
#endif

#ifndef MXG_CHOSTPORT_H
#include "SipParser/CHostPort.h" // ms_uUSE_SIP_DEFAULT_DONT_OUTPUT_PORT
#endif

#ifndef MXG_CSIPURI_H
#include "SipParser/CSipUri.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class IUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CRequestLine
//======================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   The CRequestLine class handles the storage, parsing, and serialization of
//   the request-line construct. It is composed of a method and a Request-URI.
//
//   The M5T SIP stack does not store the SIP-Version. It only supports SIP/2.0.
//
//  <CODE>
//      RFC 3261 ABNF:
//          Request-Line   =  Method SP Request-URI SP SIP-Version CRLF
//          Request-URI    =  SIP-URI / SIPS-URI / absoluteURI
//          SIP-Version =  "SIP" "/" 1*DIGIT "." 1*DIGIT
//  </CODE>
//
// Location:
//   SipParser/CRequestLine.h
//
// See Also:
//   Method.h
//
//==EDOC========================================================================
class CRequestLine
{
// Published Interface
//-------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CRequestLine();

    // Summary:
    //  Copy constructor.
    //--------------------
    CRequestLine(IN const CRequestLine& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CRequestLine();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CRequestLine& operator=(IN const CRequestLine& rSrc);

    // Summary:
    //  Provides access to the Request-URI and casts it to CSipUri if
    //  applicable.
    //----------------------------------------------------------------
    inline const CSipUri* GetSipUri() const;

    // <COMBINE CRequestLine::GetSipUri@const>
    //----------------------------------------------------------------
    inline CSipUri* GetSipUri();

    // Summary:
    //  Provides access to the Request-URI.
    //--------------------------------------
    const IUri* GetUri() const;

    // <COMBINE CRequestLine::GetUri@const>
    //-------------------------------------
    IUri* GetUri();

    // Summary:
    //  Converts the method to its enum form.
    //----------------------------------------
    ESipMethod GetMethod() const;

    // Summary:
    //  Provides access to the method data member.
    //---------------------------------------------
    const CToken& GetMethodToken() const;

    // <COMBINE CRequestLine::GetMethodToken@const>
    //----------------------------------------
    CToken& GetMethodToken();

    // Summary:
    //  Parses the Request-Line.
    //---------------------------
    mxt_result Parse(INOUT const char*& rpcPos);

    // Summary:
    //  Resets the method and Request-URI.
    //-------------------------------------
    void Reset();

    // Summary:
    //  Sets the Request-URI as a SIPURI.
    //------------------------------------
    void SetSipUri( IN const CString& rstrHost,
                    IN uint16_t uPort = CHostPort::ms_uUSE_SIP_DEFAULT_DONT_OUTPUT_PORT,
                    IN const CString& rstrUser = CString(),
                    IN CSipUri::ESecurityFlag eSecured = CSipUri::eUNSECURE );

    // Summary:
    //  Sets the Request-URI. Takes ownership of the pointer.
    //--------------------------------------------------------
    void SetRequestUri(IN TO IUri* pUri);

    // Summary:
    //  Outputs the data members into the buffer.
    //--------------------------------------------
    void Serialize(INOUT CBlob& rBlob) const;


// Hidden Methods
//-------------------------
protected:
private:
    // Summary:
    //  This is a helper for the GetSipUri method. It provides access to
    //  the Request-URI and casts it to CSipUri if applicable.
    //-------------------------------------------------------------------
    CSipUri* InternalGetSipUri() const;

// Hidden Data Members
//-------------------------
protected:

    // Method.
    //---------
    CToken m_tokMethod;

    // Request-URI.
    //--------------
    IUri* m_pUri;


private:
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetMethod
//==
//==============================================================================
//
//  Returns:
//      Method enum.
//
//  Description:
//      Converts the method string to its enumerate equivalent.
//
//  See Also:
//      MxConvertSipMethod
//
//==EDOC========================================================================
inline ESipMethod CRequestLine::GetMethod() const
{
    return MxConvertSipMethod(m_tokMethod.GetString());
}


//==SDOC========================================================================
//==
//==  GetUri
//==
//==============================================================================
//
//  Returns:
//      Pointer to the Request-URI. It may be NULL.
//
//  Description:
//      Provides access to the Request-URI.
//
//  See Also:
//      SetUri, GetSipUri, SetSipUri
//
//==EDOC========================================================================
inline const IUri* CRequestLine::GetUri() const
{
    return m_pUri;
}

//==SDOC========================================================================
//==
//==  GetSipUri
//==
//==============================================================================
//
//  Returns:
//      Constant pointer to the Request-URI, if it is a SIP or SIPS URI. It is
//      NULL otherwise.
//
//  Description:
//      Provides access to the Request-URI in the CSipUri form.
//
//  See Also:
//      SetSipUri, GetUri, SetUri
//
//==EDOC========================================================================
inline const CSipUri* CRequestLine::GetSipUri() const
{
    return InternalGetSipUri();
}

//==SDOC========================================================================
//==
//==  GetSipUri
//==
//==============================================================================
//
//  Returns:
//      Modifiable pointer to the Request-URI, if it is a SIP or SIPS URI. NULL
//      otherwise.
//
//  Description:
//      Provides access to the Request-URI in the CSipUri form.
//
//  See Also:
//      SetSipUri, GetUri, SetUri
//
//==EDOC========================================================================
inline CSipUri* CRequestLine::GetSipUri()
{
    return InternalGetSipUri();
}

//==SDOC========================================================================
//==
//==  GetUri
//==
//==============================================================================
//
//  Returns:
//      Pointer to the Request-URI. It may be NULL.
//
//  Description:
//      Provides access to the Request-URI.
//
//  See Also:
//      SetUri, GetSipUri, SetSipUri
//
//==EDOC========================================================================
inline IUri* CRequestLine::GetUri()
{
    return m_pUri;
}

//==SDOC========================================================================
//==
//==  GetMethodToken
//==
//==============================================================================
//
//  Returns:
//      Constant reference on the method.
//
//  Description:
//      Provides access to the method.
//
//==EDOC========================================================================
inline const CToken& CRequestLine::GetMethodToken() const
{
    return m_tokMethod;
}

//==SDOC========================================================================
//==
//==  GetMethodToken
//==
//==============================================================================
//
//  Returns:
//      Modifiable reference on the method.
//
//  Description:
//      Provides access to the method.
//
//==EDOC========================================================================
inline CToken& CRequestLine::GetMethodToken()
{
    return m_tokMethod;
}


MX_NAMESPACE_END(MXD_GNS)


#endif // #ifndef MXG_CREQUESTLINE_H
