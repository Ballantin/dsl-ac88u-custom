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
#ifndef MXG_CSIPSTATUSLINE_H
#define MXG_CSIPSTATUSLINE_H

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
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_STATUSCODE_H
#include "SipParser/StatusCode.h" // ESipStatusClass
#endif


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


//==SDOC========================================================================
//== Class: CSipStatusLine
//======================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   The CSipStatusLine class handles the storage, parsing, and serialization of
//   the status-line construct. It is made up of a status code and reason
//   phrase.
//
//   The M5T SIP stack does not store the SIP-Version. This stack only supports 
//   SIP/2.0 is supported.
//
//  <CODE>
//      RFC 3261 ABNF:
//          Status-Line =  SIP-Version SP Status-Code SP Reason-Phrase CRLF
//          SIP-Version =  "SIP" "/" 1*DIGIT "." 1*DIGIT
//          Status-Code =  Informational
//                         /   Redirection
//                         /   Success
//                         /   Client-Error
//                         /   Server-Error
//                         /   Global-Failure
//                         /   extension-code
//          extension-code  =  3DIGIT
//          Reason-Phrase   =  *(reserved / unreserved / escaped
//                               / UTF8-NONASCII / UTF8-CONT / SP / HTAB)
//  </CODE>
//
// Location:
//   SipParser/CSipStatusLine.h
//
//==EDOC========================================================================
class CSipStatusLine
{
// Published Interface
//-------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipStatusLine();

    // Summary:
    //  Copy constructor.
    //--------------------
    CSipStatusLine(IN const CSipStatusLine& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipStatusLine();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSipStatusLine& operator=(IN const CSipStatusLine& rSrc);

    // Summary:
    //  Returns the status class.
    //----------------------------
    ESipStatusClass GetClass() const;

    // Summary:
    //  Provides access to the status code.
    //--------------------------------------
    uint16_t GetCode() const;

    // Summary:
    //  Provides access to the reason phrase.
    //----------------------------------------
    const CString& GetPhrase() const;

    // Summary:
    //  Parses the status-line.
    //--------------------------
    mxt_result Parse(INOUT const char*& rpcPos);

    // Summary:
    //  Reinitializes data members.
    //------------------------------
    void Reset();

    // Summary:
    //  Outputs the contained data into the buffer.
    //----------------------------------------------
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Sets the status code and reason phrase.
    //------------------------------------------
    void Set(IN uint16_t uCode, IN const CString& rstrPhrase = CString());


// Hidden Methods
//-------------------------
protected:
private:


// Hidden Data Members
//-------------------------
protected:

    // Status code.
    //--------------
    uint16_t m_uStatusCode;

    // Reason Phrase.
    //----------------
    CString  m_strPhrase;


private:
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline ESipStatusClass CSipStatusLine::GetClass() const
{
    return MxGetSipStatusClass(m_uStatusCode);
}

inline uint16_t CSipStatusLine::GetCode() const
{
    return m_uStatusCode;
}

inline const CString& CSipStatusLine::GetPhrase() const
{
    return m_strPhrase;
}


MX_NAMESPACE_END(MXD_GNS)


#endif // #ifndef MXG_CSIPSTATUSLINE_H

