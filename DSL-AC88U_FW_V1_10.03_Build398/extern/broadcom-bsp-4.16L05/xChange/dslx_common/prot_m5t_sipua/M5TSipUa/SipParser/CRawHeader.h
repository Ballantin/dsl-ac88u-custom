//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CRAWHEADER_H
#define MXG_CRAWHEADER_H

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


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CRawHeader
//========================================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   This class represents the most basic form of SIP header. This is composed
//   of the header name and the header value. This class handles the unparsed
//   form of a header, directly received from the network.
//
//   #Reception of RAW data#
//
//   The CRawHeader is able to cope with the reception of a stream of bytes that
//   will eventually be split into single headers (CRLF-terminated lines). The
//   AppendRawData() method can be used repeatedly to handle the buffering of
//   bytes up to the CRLF. The IsComplete() method notifies the user if the Raw
//   header considers it has enough data to be useable.
//
// Location:
//   SipParser/CRawHeader.h
//
//==EDOC========================================================================
class CRawHeader
{
//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CRawHeader();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CRawHeader(IN const CRawHeader& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CRawHeader();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CRawHeader& operator=(IN const CRawHeader& rSrc);

    //-- << Public Methods >>
    //--------------------------------------------

    // Summary:
    //  Buffers characters of the header's name and body, up to the terminating
    //  CRLF.
    //--------------------------------------------------------------------------
    mxt_result AppendRawData(INOUT const char*& rpcPos);

    // Summary:
    //  Provides access to the header's name.
    //----------------------------------------
    const CToken& GetName() const;

    // <COMBINE CRawHeader::GetName@const>
    //------------------------------------
    CToken& GetName();

    // Summary:
    //  Provides access to the header's body.
    //----------------------------------------
    const CString& GetBody() const;

    // <COMBINE CRawHeader::GetBody@const>
    //------------------------------------
    CString& GetBody();

    // Summary:
    //  Returns true if the raw header is considered complete, and as such
    //  futher calls to AppendRawData will fail.
    //---------------------------------------------------------------------
    bool IsComplete() const;

    // Summary:
    //  Returns the object to initial state.
    //---------------------------------------
    void Reset();


//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Returns true if the appended raw data is terminated by a CRLF.
    //-----------------------------------------------------------------
    bool HasLineTerminator() const;

    // Summary:
    //  When the body contains the name:body data, this method parses the name
    //  and adjusts the body accordingly. Is is called automatically by
    //  AppendRawData().
    //-------------------------------------------------------------------------
    mxt_result Commit();


private:


//-- Hidden Data Members
//------------------------
protected:

    // Name of the header.
    //---------------------
    CToken m_tokName;

    // Contents of the body.
    //-----------------------
    CString m_strBody;

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
//      Name of the header.
//
//  Description:
//      Provides access to the header's name.
//
//  See Also:
//      GetBody
//
//==EDOC========================================================================
inline const CToken& CRawHeader::GetName() const
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
//      Name of the header.
//
//  Description:
//      Provides access to the header's name.
//
//  See Also:
//      GetBody
//
//==EDOC========================================================================
inline CToken& CRawHeader::GetName()
{
    return m_tokName;
}

//==SDOC========================================================================
//==
//==  GetBody
//==
//==============================================================================
//
//  Returns:
//      Body of the header.
//
//  Description:
//      Provides access to the header's body.
//
//  See Also:
//      GetName
//
//==EDOC========================================================================
inline const CString& CRawHeader::GetBody() const
{
    return m_strBody;
}

//==SDOC========================================================================
//==
//==  GetBody
//==
//==============================================================================
//
//  Returns:
//      Body of the header.
//
//  Description:
//      Provides access to the header's body.
//
//  See Also:
//      GetName
//
//==EDOC========================================================================
inline CString& CRawHeader::GetBody()
{
    return m_strBody;
}

//==SDOC========================================================================
//==
//==  IsComplete
//==
//==============================================================================
//
//  Returns:
//      True if the header is considered complete. This means that the header
//      has a non-empty name.
//
//  Description:
//      Checks if the header is complete.
//
//  See Also:
//      AppendRawData
//
//==EDOC========================================================================
inline bool CRawHeader::IsComplete() const
{
    return !m_tokName.IsEmpty();
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CRAWHEADER_H

