//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CMESSAGESUMMARY_H
#define MXG_CMESSAGESUMMARY_H

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

#ifndef MXG_CVLIST_H
#include "Cap/CVList.h"
#endif

#ifndef MXG_CHEADERLIST_H
#include "SipParser/CHeaderList.h"
#endif


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class IUri;
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==SDOC========================================================================
//== Class: CMessageSummary
//========================================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   The CMessageSummary class is used to store, parse, and serialize the
//   Message waiting indicator message-summary construct. The message-summary
//   construct is the default, text-based structure for the MWI NOTIFY body.
//   This implementation is based on draft-ietf-sipping-mwi-04.txt.
//
//   In order for this class to parse correctly a buffer of bytes, the last byte
//   of the buffer must be a NULL byte ('\0').
//
//
//  <CODE>
//    draft-ietf-sipping-mwi-04.txt ABNF:
//
//      messsage-summary = msg-status-line CRLF
//                         [msg-account CRLF]
//                         [*(msg-summary-line CRLF)]
//                         [ *opt-msg-headers ]
//
//      msg-status-line  = "Messages-Waiting" HCOLON msg-status
//      msg-status = "yes" / "no"
//      msg-account = "Message-Account" HCOLON Account-URI
//      Account-URI = SIP-URI / SIPS-URI / absoluteURI
//
//      msg-summary-line = message-context-class HCOLON newmsgs SLASH oldmsgs
//                         [ LPAREN new-urgentmsgs SLASH old-urgentmsgs RPAREN ]
//
//      newmsgs = msgcount
//      oldmsgs = msgcount
//      new-urgentmsgs = msgcount
//      old-urgentmsgs  = msgcount
//      msgcount = 1*DIGIT   ; MUST NOT exceed 2^32-1
//
//    RFC 3458 ABNF:
//      message-context-class =  (   "voice-message"
//                                  / "fax-message"
//                                  / "pager-message"
//                                  / "multimedia-message"
//                                  / "text-message"
//                                  / "none"
//                               )
//  </CODE>
//
// Location:
//   SipParser/CMessageSummary.h
//
// See Also:
//   SSummaryLine, CHeaderList, IUri
//
//==EDOC========================================================================
class CMessageSummary
{
//-- Published Interface
//------------------------
public:

//-- Public structure
//---------------------

//==SDOC========================================================================
//== Struct: SSummaryLine
//========================================
//
// Description:
//   This structure contains the data to represent the msg-summary-line ABNF
//   construct.
//
//  <CODE>
//    draft-ietf-sipping-mwi-04.txt ABNF:
//      msg-summary-line = message-context-class HCOLON newmsgs SLASH oldmsgs
//                    [ LPAREN new-urgentmsgs SLASH old-urgentmsgs RPAREN ]
//
//      newmsgs = msgcount
//      oldmsgs = msgcount
//      new-urgentmsgs = msgcount
//      old-urgentmsgs  = msgcount
//      msgcount = 1*DIGIT   ; MUST NOT exceed 2^32-1
//
//    RFC 3458 ABNF:
//      message-context-class =  (   "voice-message"
//                                  / "fax-message"
//                                  / "pager-message"
//                                  / "multimedia-message"
//                                  / "text-message"
//                                  / "none"
//                               )
//  </CODE>
//
// Location:
//   SipParser/CMessageSummary.h
//
// See Also:
//   CMessageSummary
//
//==EDOC========================================================================
struct SSummaryLine
{
    // Contains the message-context-class construct.
    //-----------------------------------------------
    CString m_strMessageContextClass;

    // Contains the newmsgs construct.
    //---------------------------------
    uint32_t m_uNewMsgs;

    // Contains the oldmsgs construct.
    //---------------------------------
    uint32_t m_uOldMsgs;

    // Contains the new-urgentmsgs construct.
    //----------------------------------------
    uint32_t m_uNewUrgentMsgs;

    // Contains the old-urgentmsgs construct.
    //----------------------------------------
    uint32_t m_uOldUrgentMsgs;

    // Summary:
    //  Constructor, initializes all numbers to 0.
    //---------------------------------------------
    SSummaryLine()
    :   m_uNewMsgs(0),
        m_uOldMsgs(0),
        m_uNewUrgentMsgs(0),
        m_uOldUrgentMsgs(0)
    {
    }
};

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CMessageSummary();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CMessageSummary(IN const CMessageSummary& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CMessageSummary();

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CMessageSummary& operator=(IN const CMessageSummary& rSrc);


    //-- << Public methods >>
    //--------------------------------------------

    // Summary:
    //  Returns true if the msg-status is "yes".
    //-------------------------------------------
    bool GetMsgStatus() const;

    // Summary:
    //  Sets the msg-status. "yes" is true, "no" is false.
    //-----------------------------------------------------
    void SetMsgStatus(IN bool bMessages);

    // Summary:
    //  Provides access to the optional Account-URI.
    //-----------------------------------------------
    const IUri* GetMsgAccount() const;

    // <COMBINE CMessageSummary::GetMsgAccount@const>
    //-----------------------------------------------
    IUri* GetMsgAccount();

    // Summary:
    //  Sets the Account-URI.
    //------------------------
    void SetMsgAccount(IN TO IUri* pAccountUri);

    // Summary:
    //  Provides access to the optional [*(msg-summary-line CRLF)].
    //--------------------------------------------------------------
    const CVList<SSummaryLine>& GetMsgSummaryLines() const;

    // <COMBINE CMessageSummary::GetMsgSummaryLines@const>
    //----------------------------------------------------
    CVList<SSummaryLine>& GetMsgSummaryLines();

    // Summary:
    //  Provides access to the optional header list.
    //-----------------------------------------------
    const CHeaderList& GetHeaderList() const;

    // <COMBINE CMessageSummary::GetHeaderList@const>
    //-----------------------------------------------
    CHeaderList& GetHeaderList();

    // Summary:
    //  Parses the data buffer, storing parsed data into the internal data
    //  members.
    //---------------------------------------------------------------------
    mxt_result Parse(INOUT const char*& rpcPos);

    // Summary:
    //  Returns all data members to their initial state.
    //---------------------------------------------------
    void Reset();

    // Summary:
    //  Outputs the contents of the data members into the output buffer.
    //-------------------------------------------------------------------
    void Serialize(INOUT CBlob& rBlob) const;


//-- Hidden Methods
//-------------------
protected:
    // Summary:
    //  Parses the newmsgs SLASH oldmsgs construct. (uint32_t slash uint32_t).
    //-------------------------------------------------------------------------
    mxt_result ParseNumberPair( INOUT const char*& rpcPos,
                                OUT uint32_t& ruFirst,
                                OUT uint32_t& ruSec ) const;

    // Summary:
    //  Attempts to parse the optional Account-URI.
    //----------------------------------------------
    mxt_result TryToParseMsgAccount(INOUT const char*& rpcPos);

    // Summary:
    //  Attempts to parse the optional list of summary lines.
    //--------------------------------------------------------
    mxt_result TryToParseSummaryLines(INOUT const char*& rpcPos);

    // Summary:
    //  Attempts to parse the optional headers.
    //------------------------------------------
    mxt_result TryToParseHeaders(INOUT const char*& rpcPos);


private:


//-- Hidden Data Members
//------------------------
protected:

    // True if the msg-status is "yes", false otherwise.
    //---------------------------------------------------
    bool m_bMsgStatus;

    // Pointer to the Account-URI.
    //-----------------------------
    IUri* m_pAccount;

    // Implementation note: CVList was chosen because we don't care about the
    // loss of performance when inserting items, and it will provide code reuse
    // for the reset and copy method.
    //--------------------------------------------------------------------------

    // List of summary-lines ([*(msg-summary-line CRLF)]).
    //-----------------------------------------------------
    CVList<SSummaryLine> m_vlststSumLines;

    // List of headers ([ *opt-msg-headers ]).
    //-----------------------------------------
    CHeaderList m_headerList;


public:
    // RFC3458 message-context-class value "voice-message".
    //------------------------------------------------------
    static const char* const ms_szMWI_MSG_CONTEXT_CLASS_VOICE;

    // RFC3458 message-context-class value "fax-message".
    //----------------------------------------------------
    static const char* const ms_szMWI_MSG_CONTEXT_CLASS_FAX;

    // RFC3458 message-context-class value "pager-message".
    //------------------------------------------------------
    static const char* const ms_szMWI_MSG_CONTEXT_CLASS_PAGER;

    // RFC3458 message-context-class value "multimedia-message".
    //-----------------------------------------------------------
    static const char* const ms_szMWI_MSG_CONTEXT_CLASS_MULTIMEDIA;

    // RFC3458 message-context-class value "text-message".
    //-----------------------------------------------------
    static const char* const ms_szMWI_MSG_CONTEXT_CLASS_TEXT;

    // RFC3458 message-context-class value "none".
    //---------------------------------------------
    static const char* const ms_szMWI_MSG_CONTEXT_CLASS_NONE;

    // Draft-ietf-sipping-mwi-04.txt msg-status value "yes".
    //-------------------------------------------------------
    static const char* const ms_szMWI_YES;

    // Draft-ietf-sipping-mwi-04.txt msg-status value "no".
    //------------------------------------------------------
    static const char* const ms_szMWI_NO;

    // Start of the msg-status-line.
    //-------------------------------
    static const char* const ms_szMWI_MESSAGES_WAITING;

    // Start of the msg-account.
    //---------------------------
    static const char* const ms_szMWI_MESSAGE_ACCOUNT;

    // Length of szMWI_MESSAGES_WAITING.
    //-----------------------------------
    static const uint8_t ms_uMWI_MESSAGES_WAITING_LEN_IN_BYTES;

    // Length of szMWI_MESSAGE_ACCOUNT.
    //-----------------------------------
    static const uint8_t ms_uMWI_MESSAGE_ACCOUNT_LEN_IN_BYTES;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetMsgStatus
//==
//==============================================================================
//
//  Returns:
//      True if the msg-status is "yes", false otherwise.
//
//  Description:
//      Provides access to the msg-status value. True means that the set or
//      parsed msg-status is "yes". Any other msg-status is processed as "no".
//
//  See Also:
//      SetMsgStatus
//
//==EDOC========================================================================
inline bool CMessageSummary::GetMsgStatus() const
{
    return m_bMsgStatus;
}

//==SDOC========================================================================
//==
//==  SetMsgStatus
//==
//==============================================================================
//
//  Parameters:
//    bMessages:
//      The msg-status. True means that messages are present.
//
//  Description:
//      Sets the msg-status value.
//
//  See Also:
//      GetMsgStatus
//
//==EDOC========================================================================
inline void CMessageSummary::SetMsgStatus(IN bool bMessages)
{
    m_bMsgStatus = bMessages;
}

//==SDOC========================================================================
//==
//==  GetMsgAccount
//==
//==============================================================================
//
//  Returns:
//      Pointer to the Account-URI. It may be NULL.
//
//  Description:
//      Provides access to the optional Account-URI.
//
//  See Also:
//      SetMsgAccount
//
//==EDOC========================================================================
inline const IUri* CMessageSummary::GetMsgAccount() const
{
    return m_pAccount;
}

//==SDOC========================================================================
//==
//==  GetMsgAccount
//==
//==============================================================================
//
//  Returns:
//      Pointer to the Account-URI. It may be NULL.
//
//  Description:
//      Provides access to the optional Account-URI.
//
//  See Also:
//      SetMsgAccount
//
//==EDOC========================================================================
inline IUri* CMessageSummary::GetMsgAccount()
{
    return m_pAccount;
}

//==SDOC========================================================================
//==
//==  GetMsgSummaryLines
//==
//==============================================================================
//
//  Returns:
//      Constant reference to the list of summary lines.
//
//  Description:
//      Provides access to the list of summary lines.
//
//==EDOC========================================================================
inline const CVList<CMessageSummary::SSummaryLine>&
CMessageSummary::GetMsgSummaryLines() const
{
    return m_vlststSumLines;
}

//==SDOC========================================================================
//==
//==  GetMsgSummaryLines
//==
//==============================================================================
//
//  Returns:
//      Modifiable reference to the list of summary lines.
//
//  Description:
//      Provides access to the list of summary lines.
//
//==EDOC========================================================================
inline CVList<CMessageSummary::SSummaryLine>&
CMessageSummary::GetMsgSummaryLines()
{
    return m_vlststSumLines;
}

//==SDOC========================================================================
//==
//==  GetHeaderList
//==
//==============================================================================
//
//  Returns:
//      Constant reference to the list of headers.
//
//  Description:
//      Provides access to the list of headers.
//
//==EDOC========================================================================
inline const CHeaderList& CMessageSummary::GetHeaderList() const
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
//      Modifiable reference to the list of headers.
//
//  Description:
//      Provides access to the list of headers.
//
//==EDOC========================================================================
inline CHeaderList& CMessageSummary::GetHeaderList()
{
    return m_headerList;
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CMESSAGESUMMARY_H

