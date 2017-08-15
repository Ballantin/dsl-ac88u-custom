//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSTRINGHELPER_H
#define MXG_CSTRINGHELPER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_RESULTIDSIPPARSER_H
#include "SipParser/ResultIdSipParser.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CStringHelper
//========================================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//
// Location:
//   SipParser/CStringHelper.h
//
// See Also:
//
//==EDOC========================================================================
class CStringHelper
{
//-- New types definition
//------------------------
public:
    // Description:
    //  Indicate whetever or not LWS is allowed. See ConditionalSkipLWS.
    enum EAllowLws
    {
        // LWS is not allowed when calling ConditionalSkipLWS.
        //-----------------------------------------------------
        eNO_LWS,

        // LWS is allowed when calling ConditionalSkipLWS.
        //-------------------------------------------------
        eALLOW_LWS
    };

//-- Published Interface
//------------------------
public:
    //-- << Get methods >>
    //---------------------

    // Summary:
    //  True if the specified string is made up of ALPHA characters.
    //---------------------------------------------------------------
    static bool IsAlpha(IN const char* szString);

    // Summary:
    //  True if the specified string is made up of alphanum characters.
    //------------------------------------------------------------------
    static bool IsAlphaNum(IN const char* szString);

    // Summary:
    //  True if the specified string is made up of LHEX or HEXDIG characters.
    //------------------------------------------------------------------------
    static bool IsHexadecimal(IN const char* szString);

    // Summary:
    //  True if the specified string is made up of '+' or '-' signs and DIGIT
    //  characters.
    //------------------------------------------------------------------------
    static bool IsNumeric(IN const char* szString);

    // Summary:
    //  True if the specified string is made up of DIGIT and one DOT
    //  characters.
    //---------------------------------------------------------------
    static bool IsFloat(IN const char* szString);

    // Summary:
    //  True if the specified string is made up of DIGIT characters.
    //---------------------------------------------------------------
    static bool IsDigit(IN const char* szString);

    // Summary:
    //  Returns true if pcPos points to a CRLF with no WSP after.
    //------------------------------------------------------------
    static bool IsLineTerminator(IN const char* pcPos);

    // Summary:
    //  Returns the number of digits that make up the LWS starting at pcPos.
    //-----------------------------------------------------------------------
    static unsigned int IsLWS(IN const char* pcPos);

    // Summary:
    //  Advances rpcPos over any encountered WSP.
    //--------------------------------------------
    static unsigned int SkipWSP(INOUT const char*& rpcPos);

    // Summary:
    //  Advances the pointer beyond any LWS up to the next non-LWS character.
    //  Returns either resSI_SIPPARSER_DATACONT or resS_OK.
    //------------------------------------------------------------------------
    static mxt_result SkipLWS(INOUT const char*& rpcPos);

    // Summary:
    //  Advances the pointer beyond a specified character, plus any following
    //  optional LWS.
    //------------------------------------------------------------------------
    static mxt_result SkipToData(INOUT const char*& rpcPos, IN char cSkip);

    // << Escaping methods for Uris >>
    //---------------------------------

    // Summary:
    //  Converts from char to "% HEX HEX".
    //-------------------------------------
    static void EscapeChar(IN char c, OUT char* pcBuf);

    // Summary:
    //  Converts from "% HEX HEX" to char.
    //-------------------------------------
    static char GetEscaped(INOUT const char*& rpcPos);

    // Summary:
    //  Removes visual separators from a phone number.
    //-------------------------------------------------
    static void RemoveVisualSeparators(INOUT CString& rstrPhoneNumber);

    // Summary:
    //  Replaces non displayable characters with '.'.
    //------------------------------------------------
    static void AdaptForDisplay(INOUT CString& rstrString);

    // << quoted-string management helpers >>
    //----------------------------------------

    //  Summary:
    //   Converts a normal string to an RFC 3261 quoted-string.
    //----------------------------------------------------------
    static mxt_result StringToQuotedString(INOUT CString& rStr);

    //  Summary:
    //   Converts an RFC 3261 quoted-string to a normal string.
    //----------------------------------------------------------
    static mxt_result QuotedStringToString(INOUT CString& rStr);

    // Summary:
    //  Converts an hexadecimal string into a binary blob.
    static mxt_result ConvertFromHexAscii(IN const CString& rstrAscii,
                                          OUT CBlob& rBlobBinary);
    // Summary:
    //  Converts a binary blob into an hexadecimal string.
    static void ConvertToHexAscii(IN const CBlob& rBlobBinary,
                                  OUT CString& rstrAscii);

    //  Summary:
    //  Determines if the given string is made of qdtext or quoted pair.
    //-------------------------------------------------------------------
    static bool IsStringQdTextOrQuotedPair(IN const CString& rStr);

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Deactivated Constructor.
    //---------------------------
    CStringHelper();

    // Summary:
    //  Deactivated Destructor.
    //--------------------------
    virtual ~CStringHelper();

private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------
    CStringHelper(IN const CStringHelper& rSrc);
    CStringHelper& operator=(IN const CStringHelper& rSrc);

    // Summary:
    //  Determines if the given character is a qdtext.
    //-------------------------------------------------
    static unsigned int IsQdText(IN const char* pszString);

    // Summary:
    //  Determines if the given character is a quoted-pair.
    //------------------------------------------------------
    static bool IsQuotedPair(IN const char* pszString);

    //  Summary:
    //    Determines whether or not the given character is UTF8-NONASCII.
    //--------------------------------------------------------------------
    static bool IsUtf8NonAscii(IN const char cChar);

//-- Hidden Data Members
//------------------------
protected:
private:
public:

    // The carriage return character (0x0A).
    //---------------------------------------
    static const char ms_cCR;

    // The linefeed character (0x0D).
    //--------------------------------
    static const char ms_cLF;

    // The whitespace character (0x20).
    //----------------------------------
    static const char ms_cSP;

    // The NULL character (0x00).
    //----------------------------------
    static const char ms_cNUL;

    // The tabulation character (0x09).
    //----------------------------------
    static const char ms_cTAB;

    // The SIP line terminating pair.
    //--------------------------------
    static const char* const ms_pszCRLF;

    // The SIP line double terminating pair.
    //---------------------------------------
    static const char* const ms_pszDOUBLECRLF;

    // The dash-boundary pair.
    //-------------------------
    static const char* const ms_pszDASHBOUNDARY;

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
//==SDOC========================================================================
//==
//==  MxStringHelperIsUnescapable
//==
//==============================================================================
//
//  Summary:
//    Determines whether or not the given character is unescapable.
//
//  Parameters:
//    cChar:
//      The character to verify.
//
//  Returns:
//      true if the given character is an unescapable one or false otherwise.
//
//  Description:
//      This method verifies whether or not the given character is an
//      unescapable character. An unescapable character is a character that 
//      cannot be placed alone in a quoted-string or escaped in a quoted-pair.
//      According to RFC 3261, only CR and LF are unescapable characters.
//
//==EDOC========================================================================
inline bool MxStringHelperIsUnescapable(IN const char cChar)
{
    return ((cChar == CStringHelper::ms_cCR) || (cChar == CStringHelper::ms_cLF));
}

//==SDOC========================================================================
//==
//==  MxStringHelperIsEscapedOnly
//==
//==============================================================================
//
//  Summary:
//    Determines whether or not the given character MUST be in escaped form in
//    the quoted-string.
//
//  Parameters:
//    cChar:
//      The character to verify.
//
//  Returns:
//      true if the given character is a character that must be escaped or false
//      otherwise.
//
//  Description:
//      This method verifies whether or not the given character is a character
//      that MUST be escaped in a quoted-string. According to RFC 3261,
//      only double-quote (") and back-slash (\) are characters that need to be
//      escaped in a quoted-string.
//
//==EDOC========================================================================
inline bool MxStringHelperIsEscapedOnly(IN const char cChar)
{
    return ((cChar == '"') || (cChar == '\\'));
}

//==SDOC========================================================================
//==
//==  IsCtrl
//==
//==============================================================================
//
//  Summary:
//    Returns true if the ASCII code of the specified character is between
//    0x00 and 0x20 or is 0x7f.
//
//  Parameters:
//    cChar:
//      The character to verify.
//
//  Returns:
//      true if the given character is a control character or false
//      otherwise.
//
//  Description:
//      This method verifies whether or not the given character is a control
//      character. A control character is a character that is in the range 0x01 
//      to 0x19 (inclusively) or equal 0x7F. These characters are also called
//      non-printable characters.
//
//==EDOC========================================================================
inline bool MxStringHelperIsCtrl(IN const char cChar)
{
    return ((cChar > 0x00) && (cChar < 0x20)) || (cChar == 0x7F);
}

//==SDOC========================================================================
//==
//==  MxStringHelperUpdateReturn
//==
//==============================================================================
//
//  Summary:
//      Checks if pcPos points to the end of string and returns the correct
//      result.
//
//  mxt_result.
//  Parameters:
//    pcPos:
//      Position where to check the character.
//
//  Returns:
//      resSI_SIPPARSER_DATACONT :
//          pcPos does not point to the end of line character '\0'.
//
//      resS_OK :
//          pcPos points to the end of line character '\0'.
//
//  Description:
//      Checks if pcPos points to the end of string and returns the correct
//      mxt_result.
//
//==EDOC========================================================================
inline mxt_result MxStringHelperUpdateReturn(IN const char* pcPos)
{
    return (*pcPos == CStringHelper::ms_cNUL) ?
        static_cast<mxt_result>(resS_OK) :
        static_cast<mxt_result>(resSI_SIPPARSER_DATACONT);
}

//==SDOC========================================================================
//==
//==  MxStringHelperIsEscaped
//==
//==============================================================================
//
//  Summary:
//      True if the character is escaped in the "% HEXDIG HEXDIG" form.
//
//  Parameters:
//    pcPos:
//      Position from which to start.
//
//  Returns:
//      True if pcPos points to an escaped character.
//
//  Description:
//      Checks whether or not the pointer points to an escaped character.
//
//  <CODE>
//      RFC 3261 ABNF
//          escaped     =  "%" HEXDIG HEXDIG
//
//      RFC 2234 ABNF
//          HEXDIG         =  DIGIT / "A" / "B" / "C" / "D" / "E" / "F"
//
//          Note that RFC 2234 states that all strings enclosed in
//          quotation-marks are case-insensitive.
//  </CODE>
//
//  See Also:
//      GetEscaped
//
//==EDOC========================================================================
inline bool MxStringHelperIsEscaped(IN const char* pcPos)
{
    MX_ASSERT(pcPos != NULL);
    return *pcPos == '%' && isxdigit(*(pcPos + 1)) && isxdigit(*(pcPos + 2));
}

//==SDOC========================================================================
//==
//==  MxStringHelperConditionalSkipLWS
//==
//==============================================================================
// <GROUP SIPPARSER_FUNCTIONS>
//
//  Summary:
//      SkipLWS only if eAllowLws is eALLOW_LWS. Otherwise just checks
//      (*rpcPos).
//
//  Parameters:
//    eAllowLws:
//      If eALLOW_LWS, SkipLWS is called. If eNO, only *rpcPos is checked.
//
//    rpcPos:
//      Position where to start skipping. It is updated while skipping occurs.
//
//  Returns:
//      resSI_SIPPARSER_DATACONT :
//          Data continues after the optional LWS.
//
//      resS_OK :
//          Data ends after the optional LWS.
//
//  Description:
//      If eAllowLws is eALLOW_LWS, SkipLWS is called to advance rpcPos. If not,
//      this method just checks the character pointed to by rpcPos.
//
//  See Also:
//      SkipLWS, UpdateReturn
//
//==EDOC========================================================================
inline mxt_result MxStringHelperConditionalSkipLWS(
                                        IN CStringHelper::EAllowLws eAllowLws,
                                        INOUT const char*& rpcPos)
{
    return eAllowLws == CStringHelper::eALLOW_LWS ?
                CStringHelper::SkipLWS(INOUT rpcPos) :
                MxStringHelperUpdateReturn(rpcPos);
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSTRINGHELPER_H

