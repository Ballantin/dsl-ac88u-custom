//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2008 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSIPPACKETPARSER_H
#define MXG_CSIPPACKETPARSER_H

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
#ifndef MXG_CHEADERLIST_H
#include "SipParser/CHeaderList.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSIPMESSAGEBODY_H
#include "SipParser/CSipMessageBody.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CRequestLine;
class CSipStatusLine;
class CRawHeader;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipPacketParser
//========================================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   This class handles entire SIP packets, except for the optional payload. The
//   packet-origination use case typically sets a request or status line, uses
//   the header list to set the data, and then serializes the packet. The
//   packet reception use case repeatedly uses the AppendRawData() method,
//   commits the raw headers, and then works with the header list to parse the
//   received headers.
//
//   This class does not parse the optional payload ([message-body]), but
//   carries and serializes it.
//
//      <CODE>
//      RFC 3261 ABNF:
//          generic-message  =  start-line
//                              *message-header
//                              CRLF
//                              [ message-body ]
//      </CODE>
//
//
// Location:
//   SipParser/CSipPacketParser.h
//
// See Also:
//   CHeaderList, CRawHeader, CSipHeader, CSipStatusLine, CRequestLine
//
//==EDOC========================================================================
class CSipPacketParser
{
//--New type definition
//---------------------
public:
    // Description:
    //  Structure containing the temporary list of raw headers used when
    //  collecting received data.
    struct SRawData
    {
        // Description:
        // Contains the raw start line.
        CRawHeader* m_pRawStartLine;

        // Description:
        //  Vector of CRawHeader.
        CVector<CRawHeader*>* m_pvecpRawData;
    };

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipPacketParser();

    // Summary:
    //  Copy constructor.
    //--------------------
    CSipPacketParser(IN const CSipPacketParser& rSrc);

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSipPacketParser& operator=(IN const CSipPacketParser& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipPacketParser();


    //-- << Public Methods >>
    //--------------------------------------------

    // Summary:
    //  Returns true if the packet is a request.
    //-------------------------------------------
    bool IsRequest() const;

    // Summary:
    //  Returns true if the packet is a response.
    //--------------------------------------------
    bool IsResponse() const;

    // Summary:
    //  Provides access to the request line if available.
    //----------------------------------------------------
    const CRequestLine* GetRequestLine() const;

    // <COMBINE CSipPacketParser::GetRequestLine@const>
    //-------------------------------------------------
    CRequestLine* GetRequestLine();

    // Summary:
    //  Sets the request line. Converts this packet to a request.
    //------------------------------------------------------------
    void SetRequestLine(IN TO CRequestLine* pRequestLine);

    // Summary:
    //  Provides access to the status line if available.
    //---------------------------------------------------
    const CSipStatusLine* GetStatusLine() const;

    // <COMBINE CSipPacketParser::GetStatusLine@const>
    //-------------------------------------------------
    CSipStatusLine* GetStatusLine();

    // Summary:
    //  Sets the status line. Converts this packet to a response.
    //------------------------------------------------------------
    void SetStatusLine(IN TO CSipStatusLine* pStatusLine);

    // Summary:
    //  Returns true if the packet is locally generated.
    //---------------------------------------------------
    bool IsLocallyGenerated() const;

    // Summary:
    //  Sets the local packet generation indicator.
    //---------------------------------------------------
    void SetLocallyGenerated(IN bool bLocallyGenerated);

    // Summary:
    //  Clears all data back to its initial state.
    //---------------------------------------------
    void Reset();

    //-- << Packet reception >>
    //--------------------------------------------

    // Summary:
    //  Buffers socket-received data. It also tries to parse the start line if
    //  not set. Input must be a NULL-terminated string.
    //-------------------------------------------------------------------------
    mxt_result AppendRawData(INOUT const char*& rpcPos);

    // Summary:
    //  Provides access to the list of buffered headers.
    //---------------------------------------------------
    mxt_result GetRawDataList(OUT SRawData*& rpstRawData) const;

    // Summary:
    //  Instructs CSipPacketParser to use its list of buffered raw headers and
    //  build its CHeaderList with them.
    //-------------------------------------------------------------------------
    mxt_result
        CommitRawDataList(OUT CVector<GO CSipHeader*>* pvecpRefusedRawHeaders = NULL);


    //-- << Header list manipulation >>
    //--------------------------------------------

    // Summary:
    //  Provides access to the header list.
    //--------------------------------------
    const CHeaderList& GetHeaderList() const;

    // <COMBINE CSipPacketParser::GetHeaderList@const>
    //------------------------------------------------
    CHeaderList& GetHeaderList();


    //-- << Payload access >>
    //--------------------------------------------

    // Summary:
    //  Provides access to the optional payload.
    //-------------------------------------------
    const CBlob* GetPayload() const;

    // <COMBINE CSipPacketParser::GetPayload@const>
    //---------------------------------------------
    CBlob* GetPayload();

    // Summary:
    //  Sets the optional payload.
    //-----------------------------
    void SetPayload( IN TO CBlob* pPayload );

    // Summary:
    //  Gets the message-body.
    //-------------------------
    const CSipMessageBody* GetSipMessageBody() const;

    // Summary:
    //  Gets the message-body.
    //-------------------------
    CSipMessageBody* GetSipMessageBody();

    // Summary:
    //  Sets the message-body.
    //-------------------------
    void SetSipMessageBody(IN TO CSipMessageBody* pMessageBody);

    // Summary:
    //  Creates the message-body, it can be NULL.
    const CSipMessageBody* CreateSipMessageBody() const;


    //-- << Packet creation >>
    //--------------------------------------------

    // Summary:
    //  Outputs the packet's content into the buffer.
    //------------------------------------------------
    void Serialize( INOUT CBlob& rBlob ) const;


//-- Hidden Methods
//-------------------
protected:
    // Summary:
    //  Returns true if the pointed data looks like a response start-line.
    //---------------------------------------------------------------------
    bool TestResponse(IN const char* pcPos) const;

    // Summary:
    //  Attemps to parse the start-line.
    //-----------------------------------
    mxt_result TryToParseStartLine();


private:
    // Summary:
    //  Attemps to buffer the start-line.
    //------------------------------------
    mxt_result AccumulateStartLine(INOUT const char*& rpcPos);

//-- Hidden Data Members
//------------------------
protected:

    // Points to the request line if the packet is a request.
    //--------------------------------------------------------
    CRequestLine* m_pRequestLine;

    // Points to the status line if the packet is a response.
    //--------------------------------------------------------
    CSipStatusLine* m_pStatusLine;

    // Headers found in the packet.
    //------------------------------
    CHeaderList m_headerList;

    // Contains the payload, can be NULL.
    //------------------------------------
    CBlob* m_pPayload;

    // The message-body, can be NULL in which case we must use m_pPayload if
    // non-NULL.
    //-----------------------------------------------------------------------
    mutable CSipMessageBody* m_pMessageBody;

    // Structure containing temporary list of raw headers used when
    // collecting received data.  Will also contain the raw start line
    // until it gets parsed.
    //-----------------------------------------------------------------
    SRawData m_stRawData;

    // This flag indicates that the packet is generated locally by the SIP stack.
    //----------------------------------------------------------------------------
    bool m_bLocallyGenerated;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetHeaderList
//==
//==============================================================================
//
//  Returns:
//      Reference on the header list.
//
//  Description:
//      Provides access to the header list.
//
//==EDOC========================================================================
inline const CHeaderList& CSipPacketParser::GetHeaderList() const
{
    return m_headerList;
}

//==SDOC========================================================================
//==
//==  GetHeaderList
//==
//==============================================================================
//
//  Returns:
//      Reference on the header list.
//
//  Description:
//      Provides access to the header list.
//
//==EDOC========================================================================
inline CHeaderList& CSipPacketParser::GetHeaderList()
{
    return m_headerList;
}

//==SDOC========================================================================
//==
//==  IsRequest
//==
//==============================================================================
//
//  Returns:
//      True if the packet is a request.
//
//  Description:
//      Verifies if this packet's start-line is a request-line. A return of
//      false does not necessarily mean that the packet is a response.
//
//==EDOC========================================================================
inline bool CSipPacketParser::IsRequest() const
{
    return m_pRequestLine != NULL;
}

//==SDOC========================================================================
//==
//==  IsResponse
//==
//==============================================================================
//
//  Returns:
//      True if the packet is a response.
//
//  Description:
//      Verifies if this packet's start-line is a status-line. A return of
//      false does not necessarily mean that the packet is a request.
//
//==EDOC========================================================================
inline bool CSipPacketParser::IsResponse() const
{
    return m_pStatusLine != NULL;
}

//==============================================================================
//==
//==  IsLocallyGenerated
//==
//==============================================================================
//
//  Returns:
//      - true when the packet is locally generated.
//      - false otherwise.
//
//  Description:
//      This method indicates when a packet is locally generated.
//
//==============================================================================
inline bool CSipPacketParser::IsLocallyGenerated() const
{
    return m_bLocallyGenerated;
}

//==============================================================================
//==
//==  SetLocallyGenerated
//==
//==============================================================================
//
//  Parameters:
//      bLocallyGenerated:
//          true when the packet is locally generated, false otherwise.
//
//  Description:
//      This method allows to set this packet as locally generated.
//
//==============================================================================
inline void CSipPacketParser::SetLocallyGenerated(IN bool bLocallyGenerated)
{
    m_bLocallyGenerated = bLocallyGenerated;
}

//==SDOC========================================================================
//==
//==  GetPayload
//==
//==============================================================================
//
//  Returns:
//      Constant pointer to the payload. The provided CBlob is NULL-terminated.
//      However, the NULL terminators are not counted in the CBlob's size. The
//      uncounted NULLs allow for text-based parsing, without the need to create
//      a copy of the CBlob whose only purpose would be to add the NULL
//      terminators.
//
//  Description:
//      Provides access to the payload.
//
//  Warning:
//      If you create a copy of the CBlob, the NULL terminators are NOT added to
//      the resulting copy.
//
//  See Also:
//      GetPayload
//
//==EDOC========================================================================
inline const CBlob* CSipPacketParser::GetPayload() const
{
    return m_pPayload;
}

//==SDOC========================================================================
//==
//==  GetPayload
//==
//==============================================================================
//
//  Returns:
//      Pointer to the modifiable payload. The provided CBlob is NULL-
//      terminated. However, the NULL terminators are not counted in the CBlob's
//      size. The uncounted NULLs allow for text-based parsing, without the need
//      to create a copy of the CBlob whose only purpose would be to add the
//      NULL terminators.
//
//  Description:
//      Provides modifiable access to the payload.
//
//  Warning:
//      If you create a copy of the CBlob, the NULL terminators are NOT added to
//      the resulting copy.
//
//  See Also:
//      GetPayload
//
//==EDOC========================================================================
inline CBlob* CSipPacketParser::GetPayload()
{
    return m_pPayload;
}


//==SDOC========================================================================
//==
//==  SetPayload
//==
//==============================================================================
//
//  Parameters:
//    pPayload:
//      Payload to set. Ownership is taken.
//
//  Description:
//      Sets the payload. If a previous payload is set, it is released and
//      replaced.
//
//  See Also:
//      GetPayload
//
//==EDOC========================================================================
inline void CSipPacketParser::SetPayload(IN TO CBlob* pPayload)
{
    if (m_pPayload != pPayload)
    {
        MX_DELETE(m_pPayload);
        m_pPayload = TO pPayload;
    }
}

//==SDOC========================================================================
//==
//==  GetSipMessageBody
//==
//==============================================================================
//
//  Summary:
//      Gets the message-body.
//
//  Returns:
//      The message-body. It can be NULL.
//
//  Description:
//      Gets the message-body associated with this packet.
//
//      This method returns NULL until CreateSipMessageBody is called for this
//      instance. After CreateSipMessageBody is called, this method might still
//      return NULL, depending on the content of the packet.
//
//==EDOC========================================================================
inline const CSipMessageBody* CSipPacketParser::GetSipMessageBody() const
{
    return m_pMessageBody;
}

//==SDOC========================================================================
//==
//==  GetSipMessageBody
//==
//==============================================================================
//
//  Summary:
//      Gets the message-body.
//
//  Returns:
//      The message-body. It can be NULL.
//
//  Description:
//      Gets the message-body associated with this packet.
//
//      This method returns NULL until CreateSipMessageBody is called for this
//      instance. After CreateSipMessageBody is called, this method might still
//      return NULL, depending on the content of the packet.
//
//==EDOC========================================================================
inline CSipMessageBody* CSipPacketParser::GetSipMessageBody()
{
    return m_pMessageBody;
}

//==SDOC========================================================================
//==
//==  SetSipMessageBody
//==
//==============================================================================
//
//  Parameters:
//    pMessageBody:
//      Message-body to set. Ownership is TAKEN.
//
//  Description:
//      Sets the message-body. If a previous payload is set, it is released and
//      replaced.
//
//  See Also:
//      GetSipMessageBody
//
//==EDOC========================================================================
inline void CSipPacketParser::SetSipMessageBody(IN TO CSipMessageBody* pMessageBody)
{
    if (m_pMessageBody != pMessageBody)
    {
        MX_DELETE(m_pMessageBody);
        m_pMessageBody = TO pMessageBody;
    }
}

//==SDOC========================================================================
//==
//==  GetRequestLine
//==
//==============================================================================
//
//  Returns:
//      Constant reference to the request line. It can be NULL.
//
//  Description:
//      Provides access to the request-line.
//
//  See Also:
//      SetRequestLine, AppendRawData
//
//==EDOC========================================================================
inline const CRequestLine* CSipPacketParser::GetRequestLine() const
{
    return m_pRequestLine;
}

//==SDOC========================================================================
//==
//==  GetRequestLine
//==
//==============================================================================
//
//  Returns:
//      Constant reference to the request line. It can be NULL.
//
//  Description:
//      Provides access to the request-line.
//
//  See Also:
//      SetRequestLine, AppendRawData
//
//==EDOC========================================================================
inline CRequestLine* CSipPacketParser::GetRequestLine()
{
    return m_pRequestLine;
}

//==SDOC========================================================================
//==
//==  GetStatusLine
//==
//==============================================================================
//
//  Returns:
//      Constant pointer to the status-line. It can be NULL.
//
//  Description:
//      Provides access to the status-line.
//
//  See Also:
//      SetStatusLine, AppendRawData
//
//==EDOC========================================================================
inline const CSipStatusLine* CSipPacketParser::GetStatusLine() const
{
    return m_pStatusLine;
}

//==SDOC========================================================================
//==
//==  GetStatusLine
//==
//==============================================================================
//
//  Returns:
//      Constant pointer to the status-line. It can be NULL.
//
//  Description:
//      Provides access to the status-line.
//
//  See Also:
//      SetStatusLine, AppendRawData
//
//==EDOC========================================================================
inline CSipStatusLine* CSipPacketParser::GetStatusLine()
{
    return m_pStatusLine;
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPPACKETPARSER_H
