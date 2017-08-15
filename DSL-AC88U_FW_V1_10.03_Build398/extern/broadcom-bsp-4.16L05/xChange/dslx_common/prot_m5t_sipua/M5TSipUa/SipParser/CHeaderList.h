//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CHEADERLIST_H
#define MXG_CHEADERLIST_H

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
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_HEADERDEFINITION_H
#include "SipParser/HeaderDefinition.h" // ESipHeaderType
#endif

#ifndef MXG_CSIPHEADER_H
#include "SipParser/CSipHeader.h" // uHEADERS_PARSE_ALL
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
//== Class: CHeaderList
//========================================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   This class manages a list of headers. It is responsible for parsing the
//   headers when required. It serves as a container for the headers. It also
//   handles the caching of headers and serializes multi-headers.
//
// Location:
//   SipParser/CHeaderList.h
//
// See Also:
//   CSipHeader
//
//==EDOC========================================================================
class CHeaderList
{
//-- New types definition
//------------------------
protected:
    // This enum is used with the Insert method.
    //-------------------------------------------
    enum EInsertPosition
    {
        // PostFix list.
        //---------------
        eAT_END,

        // Prefix list.
        //--------------
        eAT_BEGINNING,
    };

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CHeaderList();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CHeaderList(IN const CHeaderList& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CHeaderList();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CHeaderList& operator=(IN const CHeaderList& rSrc);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CHeaderList& rSrc) const;

    // Summary:
    //  Inequality operator.
    //-----------------------
    bool operator!=(IN const CHeaderList& rSrc) const;

    //-- << Packet reception >>
    //--------------------------------------------

    // Summary:
    //  Buffers received data. Allows to parse headers without requiring a
    //  start-line. Input must be a NULL-terminated string.
    //---------------------------------------------------------------------
    mxt_result AppendRawData(INOUT const char*& rpcPos);

    // Summary:
    //  Provides access to the list of buffered headers.
    //---------------------------------------------------
    mxt_result GetRawDataList(OUT CVector<CRawHeader*>*& ppvecpRawData) const;

    // Summary:
    //  Instructs to use its list of buffered raw headers and build the
    //  CHeaderList with them.
    //------------------------------------------------------------------
    mxt_result
        CommitRawDataList(OUT CVector<GO CSipHeader*>* pvecpRefusedRawHeaders = NULL);

    //-- << Header list manipulation >>
    //--------------------------------------------

    // Summary:
    //  Adds the header list at the end of the list.
    //-----------------------------------------------
    mxt_result Append(IN TO CHeaderList* pNewHeaderList);

    // Summary:
    //  Appends the header list and replaces the headers that have the same
    //  type as the one in the passed list.
    //----------------------------------------------------------------------
    void ReplaceHeaders(IN const CHeaderList& rNewHeaderList);

    // Summary:
    //  Adds the header at the end of the list.
    //------------------------------------------
    mxt_result Append(IN TO CSipHeader* pNewHeader,
                      IN bool bTakeOwnershipOnSuccess = false);

    // Summary:
    //  Inserts the header at the start of the list.
    //-----------------------------------------------
    mxt_result Prepend(IN TO CSipHeader* pNewHeader,
                       IN bool bTakeOwnershipOnSuccess = false);

    // Summary:
    //  Replaces all the headers of a type with a new header.
    //--------------------------------------------------------
    mxt_result ReplaceHeaderTypeWith(IN TO CSipHeader* pHeaderToAdd);


    // Summary:
    //  Removes the n'th header of the specified type.
    //------------------------------------------------
    mxt_result RemoveHeader(IN ESipHeaderType eHeader, IN unsigned int uIndex);

    // Summary:
    //  Removes all headers of the specified type.
    //---------------------------------------------
    mxt_result RemoveHeaderType(IN ESipHeaderType eHeader, OUT GO CSipHeader** ppSipHeader = NULL);

    // <COMBINE CHeaderList::RemoveHeader@IN ESipHeaderType@IN unsigned int>
    //---------------------------------------------------------------
    mxt_result RemoveHeader(IN const CString& rstrName, IN unsigned int uIndex);

    // Summary:
    //  Removes all headers of the specified type.
    //---------------------------------------------
    mxt_result RemoveHeaderType(IN const CString& rstrName);

    // Summary:
    //  Sorts the headers using the defined header order.
    //----------------------------------------------------
    void Sort();


    // Summary:
    //  Returns a parsed version of a CSipHeader chain.
    //--------------------------------------------------
    const CSipHeader* Get(IN ESipHeaderType eType,
                          OUT mxt_result& rres,
                          IN unsigned int uMaxToParse = uHEADERS_PARSE_ALL) const;

    // <COMBINE CHeaderList::Get@IN ESipHeaderType@OUT mxt_result&@IN unsigned int@const>
    CSipHeader* Get(IN ESipHeaderType eType,
                    OUT mxt_result& rres,
                    IN unsigned int uMaxToParse = uHEADERS_PARSE_ALL);

    // Summary:
    //  Returns a parsed version of a CSipHeader chain.
    //--------------------------------------------------
    const CSipHeader* Get(IN ESipHeaderType eType,
                          INOUT mxt_result* pres = NULL,
                          IN unsigned int uMaxToParse = uHEADERS_PARSE_ALL) const;

    // <COMBINE CHeaderList::Get@IN ESipHeaderType@INOUT mxt_result*@IN unsigned int@const>
    CSipHeader* Get(IN ESipHeaderType eType,
                    INOUT mxt_result* pres = NULL,
                    IN unsigned int uMaxToParse = uHEADERS_PARSE_ALL);

    // Summary:
    //  Returns a parsed version of a CSipHeader chain.
    //--------------------------------------------------
    const CSipHeader* Get(IN const CString& rstrName,
                          OUT mxt_result& rres,
                          IN unsigned int uMaxToParse = uHEADERS_PARSE_ALL) const;

    // <COMBINE CHeaderList::Get@IN const CString&@OUT mxt_result&@IN unsigned int@const>
    CSipHeader* Get(IN const CString& rstrName,
                    OUT mxt_result& rres,
                    IN unsigned int uMaxToParse = uHEADERS_PARSE_ALL);

    // Summary:
    //  Returns a parsed version of a CSipHeader chain.
    //--------------------------------------------------
    const CSipHeader* Get(IN const CString& rstrName,
                          INOUT mxt_result* pres = NULL,
                          IN unsigned int uMaxToParse = uHEADERS_PARSE_ALL) const;

    // <COMBINE CHeaderList::Get@IN const CString&@INOUT mxt_result*@IN unsigned int@const>
    CSipHeader* Get(IN const CString& rstrName,
                    INOUT mxt_result* pres = NULL,
                    IN unsigned int uMaxToParse = uHEADERS_PARSE_ALL);

    // Summary:
    //  Returns the number of header types that are available in the list.
    //---------------------------------------------------------------------
    unsigned int GetNbHeaderTypes() const;

    // Summary:
    //  Returns true if the list contains nothing.
    //---------------------------------------------
    bool IsEmpty() const;

    // Summary:
    //  Resets the header list to its initial state.
    //-----------------------------------------------
    void Reset();

    // Summary:
    //  Outputs the headers.
    //-----------------------
    void Serialize(IN IUri::EUriType eUriType,
                   INOUT CBlob& rBlob) const;



//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Internally removes the selected header.
    //------------------------------------------
    mxt_result RemoveHeader(IN unsigned int uTypeIndex,
                            IN unsigned int uHeaderIndex);

    // Summary:
    //  Returns the header type index.
    //---------------------------------
    mxt_result FindTypeIndex(IN ESipHeaderType eType,
                             OUT unsigned int& ruIndex) const;

    // Summary:
    //  Returns the header type index.
    //---------------------------------
    mxt_result FindTypeIndex(IN const CString& rstrName,
                             OUT unsigned int& ruIndex) const;

    // Summary:
    //  Inserts a header at the specified position in the chain.
    //-----------------------------------------------------------
    mxt_result Insert(IN TO CSipHeader* pNewHeader,
                      IN EInsertPosition eInsertPos,
                      IN bool bTakeOwnershipOnSuccess = false);

    static int SortCompare(IN CSipHeader* const& rpOneElement,
                           IN CSipHeader* const& roOtherElement,
                           IN  mxt_opaque opq);

private:


//-- Hidden Data Members
//------------------------
protected:
    // The list of headers. Each index contains a pointer to a CSipHeader which
    // is a chain of headers of the same type. These may be raw or parsed.
    //--------------------------------------------------------------------------
    CVector<CSipHeader*> m_vecpHeaders;

    // Used to synchronize the access made by const methods on m_vecHeaders.
    //----------------------------------------------------------------------
    CMutex m_mutexHeaders;

    // The temporary list of raw headers.
    //------------------------------------
    CVector<CRawHeader*>* m_pvecpRawData;

    // Temporary state marker used when collecting received raw data. Will be
    // set to true if a CR has been found after a header's CRLF. If true,
    // finding a LF after that means that the double CRLF has been found.
    //------------------------------------------------------------------------
    bool m_bFoundCR;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetNbHeaderTypes
//==
//==============================================================================
//
//  Returns:
//      Number of different header types.
//
//  Description:
//      Counts the number of header types that are supported.
//
//==EDOC========================================================================
inline unsigned int CHeaderList::GetNbHeaderTypes() const
{
    return m_vecpHeaders.GetSize();
}

//==SDOC========================================================================
//==
//==  IsEmpty
//==
//==============================================================================
//
//  Returns:
//      True if the list is empty.
//
//  Description:
//      Returns true if the list is empty.
//
//==EDOC========================================================================
inline bool CHeaderList::IsEmpty() const
{
    return m_vecpHeaders.IsEmpty();
}

//==SDOC========================================================================
//==
//==  Get
//==
//==============================================================================
//
//  Parameters:
//    eType:
//      Type of headers to parse and return.
//
//    rres:
//      Reference to a mxt_result where the result is set.
//
//      resFE_SIPPARSER_HEADERLIST_NOT_FOUND
//          Could not find headers of the specified type.
//
//      resFE_UNEXPECTED
//          Headers of the eHDR_EXTENSION type are distinguished by their name.
//          Use Get@IN const CString& rstrName@OUT const CSipHeader*&
//          rpParsedHeader@IN unsigned int uMaxToParse instead.
//
//      This method can also set rres to any value that the
//      CSipHeader::ParseRawData() method may return. The most frequent are:
//
//      resSI_SIPPARSER_DATACONT
//          Success, uMaxToParse headers have been parsed, more headers are
//          available.
//
//      resS_OK
//          Success, all available headers have been parsed.
//
//      resSW_SIPPARSER_HEADER_NOT_ENOUGH_HEADERS
//          Parsing did not encounter any error, but less than uMaxToParse
//          headers are available.
//
//    uMaxToParse:
//      Maximum number of times that the parse operation is launched on the
//      CSipHeader's next headers. This value is only an indication of the
//      maximum effort that must be put forth by the CSipHeader before returning
//      the list.
//
//  Returns:
//      Reference on the pointer to the header list. It points on a
//      CSipHeader chain if the method is successful.
//
//      Note that this method can return a NULL CSipHeader pointer even if 
//      the returned mxt_result is a success code. This happens if there is no
//      more data to parse and the header has not been found.
//      Also, a non NULL CSipHeader pointer can be returned even though 
//      the returned mxt_result is a failure code, i.e., this occurs when 
//      the header is present but the parsing failed.
//
//  Description:
//      This method commands the parsing of a type of header. It is used to
//      retrieve a list of headers of the specified type, in parsed format.
//      Refer to CSipHeader::ParseRawData for more details on SIP header
//      parsing.
//
//  See Also:
//      CSipHeader::ParseRawData.
//
//==EDOC========================================================================
inline const CSipHeader* CHeaderList::Get(IN ESipHeaderType eType,
                                          OUT mxt_result& rres,
                                          IN unsigned int uMaxToParse) const
{
   return Get(eType, &rres, uMaxToParse);
}

inline CSipHeader* CHeaderList::Get(IN ESipHeaderType eType,
                                    OUT mxt_result& rres,
                                    IN unsigned int uMaxToParse)
{
   return Get(eType, &rres, uMaxToParse);
}

//==SDOC========================================================================
//==
//==  Get
//==
//==============================================================================
//
//  Parameters:
//    rstrName:
//      Name of the headers to parse and return.
//
//    rres:
//      Reference to a mxt_result where the result is set.
//
//      resFE_SIPPARSER_HEADERLIST_NOT_FOUND
//          Could not find headers of the specified type.
//
//      resFE_UNEXPECTED
//          The requested header name is invalid.
//
//      This method can also be set to any value that the
//      CSipHeader::ParseRawData() method may return. The most frequent are:
//
//      resSI_SIPPARSER_DATACONT
//          Success, uMaxToParse headers have been parsed, more headers are
//          available.
//
//      resS_OK
//          Success, all available headers have been parsed.
//
//      resSW_SIPPARSER_HEADER_NOT_ENOUGH_HEADERS
//          Parsing did not encounter any error, but less than uMaxToParse
//          headers are available.
//
//    uMaxToParse:
//      Maximum number of times that the parse operation is launched on the
//      CSipHeader's next headers. This value is only an indication of the
//      maximum effort that must be put forth by the CSipHeader before returning
//      the list.
//
//  Returns:
//      Reference on the pointer to the header list. It points on a
//      CSipHeader chain if the method is successful.
//
//      Note that this method can return a NULL CSipHeader pointer even if 
//      the returned mxt_result is a success code. This happens if there is no
//      more data to parse and the header has not been found.
//      Also, a non NULL CSipHeader pointer can be returned even though 
//      the returned mxt_result is a failure code, i.e., this occurs when 
//      the header is present but the parsing failed.
//
//  Description:
//      This method commands the parsing of a type of header. It is used to
//      retrieve a list of headers of the specified type, in parsed format.
//      Refer to CSipHeader::ParseRawData for more details on SIP header
//      parsing.
//
//  See Also:
//      CSipHeader::ParseRawData.
//
//==EDOC========================================================================
inline const CSipHeader* CHeaderList::Get(IN const CString& rstrName,
                                          OUT mxt_result& rres,
                                          IN unsigned int uMaxToParse) const
{
   return Get(rstrName, &rres, uMaxToParse);
}

inline CSipHeader* CHeaderList::Get(IN const CString& rstrName,
                                    OUT mxt_result& rres,
                                    IN unsigned int uMaxToParse)
{
   return Get(rstrName, &rres, uMaxToParse);
}

//==SDOC========================================================================
//==
//==  Append
//==
//==============================================================================
//
//  Parameters:
//    pNewHeader:
//      Header to add in the list.
//
//    bTakeOwnershipOnSuccess:
//      - false: Default value. Ownership of pNewHeader is always taken.
//      - true: Ownership is taken if the return value is a success only.
//
//  Returns:
//      See CHeaderList::Insert().
//
//  Description:
//      Adds the given header in the header list. The header is added at the end
//      of the list, except if other headers of the same type are present, in
//      which case the new header is appended to the last of these. The append
//      mechanism uses the CSipHeader's next header feature.
//
//  See Also:
//      Prepend
//
//==EDOC========================================================================
inline mxt_result CHeaderList::Append(IN TO CSipHeader* pNewHeader,
                                      IN bool bTakeOwnershipOnSuccess)
{
    return Insert(IN TO pNewHeader, eAT_END, bTakeOwnershipOnSuccess);
}

//==SDOC========================================================================
//==
//==  Prepend
//==
//==============================================================================
//
//  Parameters:
//    pNewHeader:
//      Header to add in the list.
//
//    bTakeOwnershipOnSuccess:
//      - false: Default value. Ownership of pNewHeader is always taken.
//      - true: Ownership is taken if the return value is a success only.
//
//  Returns:
//      See CHeaderList::Insert().
//
//  Description:
//      Adds the given header in the header list. The header is inserted at the
//      start of the list, except if other headers of the same type are present,
//      in which case the new header is inserted at the start of these. The
//      Prepend mechanism uses the CSipHeader's next header feature.
//
//  See Also:
//      Prepend
//
//==EDOC========================================================================
inline mxt_result CHeaderList::Prepend(IN TO CSipHeader* pNewHeader,
                                       IN bool bTakeOwnershipOnSuccess)
{
    return Insert(IN TO pNewHeader, eAT_BEGINNING, bTakeOwnershipOnSuccess);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CHEADERLIST_H

