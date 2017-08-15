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
#ifndef MXG_CSDPFIELDATTRIBUTECRYPTO_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTECRYPTO_H

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

#ifndef MXG_CCRYPTOKEYPARAMLIST_H
#include "SdpParser/CCryptoKeyParamList.h"
#endif

#ifndef MXG_CCRYPTOSESSIONPARAMLIST_H
#include "SdpParser/CCryptoSessionParamList.h"
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
//==  Class: CSdpFieldAttributeCrypto
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of an attribute-crypto.
//
// Description:
//   This class is an abstraction of an attribute-crypto
//   in a SDP packet.  It follows the BNF notation described in the
//   draft-ietf-mmusic-sdescriptions-09.
//
//  <PRE>
//   draft-ietf-mmusic-sdescriptions-09 ABNF:
//
//   "crypto:" tag 1*WSP crypto-suite 1*WSP key-params *(1*WSP session-param)
//
//   tag              = 1*ALPHANUM
//   crypto-suite     = 1*(ALPHA / DIGIT / "_")
//
//   key-params       = key-param *(";" key-param)
//   key-param        = key-method ":" key-info
//   key-method       = "inline" / key-method-ext
//   key-method-ext   = 1*(ALPHA / DIGIT / "_")
//   key-info         = %x21-3A / %x3C-7E ; visible (printing) characters
//                                        ; except semi-colon
//   session-param    = 1*(VCHAR)         ; visible (printing) characters
//
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeCrypto.h
//
// See Also:
//
//==============================================================================
class CSdpFieldAttributeCrypto : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>
    // Summary: Default constructor.
    CSdpFieldAttributeCrypto();

    // Summary: Copy constructor.
    CSdpFieldAttributeCrypto(IN const CSdpFieldAttributeCrypto& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeCrypto();

    // Summary: Comparison operator.
    bool operator==(IN const CSdpFieldAttributeCrypto& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpFieldAttributeCrypto& operator=(IN const CSdpFieldAttributeCrypto& rFrom);

    //-- << Stereotype >>

    // Summary:
    //   Gets the Tag name of the key.
    const char* GetTag() const;

    // Summary:
    //   Gets the CryptoSuite name.
    const char* GetCryptoSuite() const;

    // Summary:
    //   Gets the KeyParamList.
    CCryptoKeyParamList* GetKeyParams();

    // Summary:
    //   Gets the KeyParamList.
    const CCryptoKeyParamList* GetKeyParams() const;

    // Summary:
    //   Gets the SessionParamList.
    CCryptoSessionParamList* GetSessionParams();

    // Summary:
    //   Gets the SessionParamList.
    const CCryptoSessionParamList* GetSessionParams() const;

    // Summary:
    //   Generates the data blob from the data members.
    void  Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //   Sets the Tag name.
    void SetTag(IN const char* pszTag);

    // Summary:
    //   Sets the CryptoSuite name.
    void SetCryptoSuite(IN const char* pszCyptoSuite);

    //-- Virtual functions
    // Summary:
    //   Parses all the needed information for this field.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);
    // Summary:
    //   Checks the validity of the parsed data.
    bool Validate();

    // Summary:
    //   Resets all the data member.
    void Reset();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:

// Hidden Data Members
protected:
private:
    CString m_strTag;
    CString m_strCryptoSuite;
    CCryptoKeyParamList m_vKeyParams;
    CCryptoSessionParamList m_vSessionParams;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetTag
//==
//==============================================================================
//
//  Returns:
//      The Tag name of the key.
//
//  Description:
//      Returns the Tag name of the key.
//
//==============================================================================
inline const char*
CSdpFieldAttributeCrypto::GetTag() const
{
    return m_strTag.CStr();
}

//==============================================================================
//==
//==  GetCryptoSuite
//==
//==============================================================================
//
//  Returns:
//      The CryptoSuite name.
//
//  Description:
//      Returns the CryptoSuite name.
//
//==============================================================================
inline const char*
CSdpFieldAttributeCrypto::GetCryptoSuite() const
{
    return m_strCryptoSuite.CStr();
}

//==============================================================================
//==
//==  SetTag
//==
//==============================================================================
//
//  Parameters:
//    pszTag:
//      Name of the key.
//
//  Returns:
//      None.
//
//  Description:
//      This method sets the Tag name.
//
//==============================================================================
inline void CSdpFieldAttributeCrypto::SetTag(IN const char* pszTag)
{
    m_strTag = pszTag;
}

//==============================================================================
//==
//==  SetCryptoSuite
//==
//==============================================================================
//
//  Parameters:
//    pszCyptoSuite:
//      CryptoSuite.
//
//  Returns:
//      None.
//
//  Description:
//      This method sets the CryptoSuite name.
//
//==============================================================================
inline void CSdpFieldAttributeCrypto::SetCryptoSuite(IN const char* pszCyptoSuite)
{
    m_strCryptoSuite = pszCyptoSuite;
}

//==============================================================================
//==
//==  GetKeyParams
//==
//==============================================================================
//
//  Returns:
//      KeyParamsList*.
//
//  Description:
//      Returns a pointer to the KeyParamList.
//
//==============================================================================
inline CCryptoKeyParamList* CSdpFieldAttributeCrypto::GetKeyParams()
{
    return &m_vKeyParams;
}

// <COMBINE CSdpFieldAttributeCrypto::GetKeyParams>
inline const CCryptoKeyParamList* CSdpFieldAttributeCrypto::GetKeyParams() const
{
    return &m_vKeyParams;
}

//==============================================================================
//==
//==  GetSessionParams
//==
//==============================================================================
//
//  Returns:
//      SessionParamList*.
//
//  Description:
//      Returns a pointer to the SessionParamList.
//
//==============================================================================
inline CCryptoSessionParamList* CSdpFieldAttributeCrypto::GetSessionParams()
{
    return &m_vSessionParams;
}

// <COMBINE CSdpFieldAttributeCrypto::GetSessionParams>
inline const CCryptoSessionParamList* CSdpFieldAttributeCrypto::GetSessionParams() const
{
    return &m_vSessionParams;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTECRYPTO_H
