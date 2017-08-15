//==============================================================================
//==============================================================================
//
//   Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSDPFIELDMEDIAANNOUNCEMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDMEDIAANNOUNCEMENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Member
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif



//-- Interface Realized & Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif



//-- Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)



//-- Forward Declarations Inside of the Namespace
class CBlob;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//==  Class: CSdpFieldMediaAnnouncement
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a media-field.
//
// Description:
//   This class is an abstraction of a media-field in a SDP packet.
//
//   A session description may contain a number of media descriptions,
//   each media description starts with an "m=" field.  Currently defined
//   medias are "audio", "video", "application", "data", and "control".
//
//   It follows the BNF described in RFC 2327.
//
//  RFC 2327 BNF:
//  <PRE>
//      media-field     =   "m=" media space port ["/" integer]
//                          space proto 1*(space fmt) CRLF
//      media           =   1*(alpha-numeric)
//      fmt             =   1*(alpha-numeric)
//      proto           =   1*(alpha-numeric)
//      port            =   1*(DIGIT)
//      space           =   " "
//      alpha-numeric   =   ALPHA | DIGIT
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldMediaAnnouncement.h
//
// See Also:
//
//==============================================================================
class CSdpFieldMediaAnnouncement : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldMediaAnnouncement();

    // Summary: Copy constructor.
    CSdpFieldMediaAnnouncement(IN const CSdpFieldMediaAnnouncement& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldMediaAnnouncement();

    // Summary: Comparison Operator.
    bool operator==(IN const CSdpFieldMediaAnnouncement& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpFieldMediaAnnouncement& operator=(IN const CSdpFieldMediaAnnouncement& rFrom);

    //-- Get functions

    // Summary:
    //   Gets the media type ID.
    EMediaType          GetMediaTypeId() const;

    // Summary:
    //   Gets the media type string.
    const char*         GetMediaTypeString() const;

    // Summary:
    //   Gets the transport port.
    int32_t             GetTransportPort() const;

    // Summary:
    //   Gets the number of tranpsport ports.
    int16_t             GetNbTransportPorts() const;

    // Summary:
    //   Gets the transport protocol ID.
    ETransportProtocol  GetTransportProtocolId() const;

    // Summary:
    //   Gets the transport protocol string.
    const char*         GetTransportProtocolString() const;

    // Summary:
    //   Gets the number of media formats.
    uint32_t            GetNbMediaFormats() const;

    // Summary:
    //   Gets the media format at the specified index.
    const char*         GetMediaFormat(IN uint16_t uIndex) const;

    // Summary:
    //   Serializes the specified CBlob.
    void                Serialize(INOUT CBlob& rBlob) const;

    //-- Set functions

    // Summary:
    //   Sets the media type ID.
    void SetMediaTypeId(IN EMediaType eMediaType);

    // Summary:
    //   Sets the media type string.
    void SetMediaTypeString(IN const char* pszMediaType);

    // Summary:
    //   Sets the transport port.
    void SetTransportPort(IN int32_t nTransportPort);

    // Summary:
    //   Sets the number of transport ports.
    void SetNbTransportPorts(IN int16_t nNbTransportPorts);

    // Summary:
    //   Sets the transport protocol ID.
    void SetTransportProtocolId(IN ETransportProtocol eTransportProtocol);

    // Summary:
    //   Sets the transport protocol string.
    void SetTransportProtocolString(IN const char* pszTransportProtocol);

    // Summary:
    //   Adds a media format at the end of the list.
    void AddMediaFormat(IN const char* pszMediaFormat);

    // Summary:
    //   Sets the media format at the specified index.
    void SetMediaFormat(IN uint16_t    uIndex,
                        IN const char* pszMediaFormat);

    // Summary:
    //   Inserts a media format at the specified index.
    void InsertMediaFormat(IN uint16_t    uIndex,
                           IN const char* pszMediaFormat);

    // Summary:
    //   Removes the media format at the specified index.
    void RemoveMediaFormat(IN uint16_t uIndex);

    //-- Virtual functions

    // Summary:
    //   Parses all the needed information for this field.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    //   Checks the validity of the parsed data.
    bool Validate();

    // Summary:
    //   Resets all the data members.
    void Reset();

    //-- Helpers
    // Summary:
    //   Parses a string containing ONLY digits.
    static EParserResult ParseMediaFormat(IN const char* pszMediaFormat,
                                          OUT uint32_t& ruMediaFormat);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    bool IsUdpBasedTransport() const;
    bool IsRtpTransport() const;

private:

//-- Hidden Data Members
protected:
private:
    EMediaType          m_eMediaType;
    CString              m_strMediaType;
    int32_t               m_nTransportPort;
    int16_t               m_nNbTransportPorts;
    ETransportProtocol  m_eTransportProtocol;
    CString              m_strTransportProtocol;
    CVector <CString>    m_vecMediaFormats;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetMediaTypeId
//==
//==============================================================================
//
//  Returns:
//      The field connection data CSdpParser::ENetworkType.
//
//  Description:
//      Returns the network type ID.
//
//==============================================================================
inline CSdpParser::EMediaType CSdpFieldMediaAnnouncement::GetMediaTypeId() const
{
    return m_eMediaType;
}

//==============================================================================
//==
//==  GetMediaTypeString
//==
//==============================================================================
//
//  Returns:
//      The network type string.
//
//  Description:
//      Returns the network type string.
//
//==============================================================================
inline const char* CSdpFieldMediaAnnouncement::GetMediaTypeString() const
{
    return m_strMediaType.CStr();
}

//==============================================================================
//==
//==  GetTransportPort
//==
//==============================================================================
//
//  Returns:
//      The transport port.
//
//  Description:
//      Returns the value of the transport port.
//
//==============================================================================
inline int32_t CSdpFieldMediaAnnouncement::GetTransportPort() const
{
    return m_nTransportPort;
}

//==============================================================================
//==
//==  GetNbTransportPorts
//==
//==============================================================================
//
//  Returns:
//      The number of transport port.
//
//  Description:
//      Returns the number of transport ports.
//
//==============================================================================
inline int16_t CSdpFieldMediaAnnouncement::GetNbTransportPorts() const
{
    return m_nNbTransportPorts;
}

//==============================================================================
//==
//==  GetTransportProtocolId
//==
//==============================================================================
//
//  Returns:
//      The transport protocol ID.
//
//  Description:
//      Returns the transport protocol ID.
//
//==============================================================================
inline CSdpParser::ETransportProtocol 
CSdpFieldMediaAnnouncement::GetTransportProtocolId() const
{
    return m_eTransportProtocol;
}

//==============================================================================
//==
//==  GetTransportProtocolString
//==
//==============================================================================
//
//  Returns:
//      The transport protocol string.
//
//  Description:
//      Returns the transport protocol string.
//
//==============================================================================
inline const char* CSdpFieldMediaAnnouncement::GetTransportProtocolString() const
{
    return m_strTransportProtocol.CStr();
}

//==============================================================================
//==
//==  GetNbMediaFormats
//==
//==============================================================================
//
//  Returns:
//      The number of media formats.
//
//  Description:
//      Returns the number of media formats.
//
//==============================================================================
inline uint32_t CSdpFieldMediaAnnouncement::GetNbMediaFormats() const
{
    return m_vecMediaFormats.GetSize();
}

//==============================================================================
//==
//==  GetMediaFormat
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Index of the desired media format.
//
//  Returns:
//      The media formats at the specified index.
//
//  Description:
//      Returns the media format at the specified index.
//
//==============================================================================
inline const char*
CSdpFieldMediaAnnouncement::GetMediaFormat(IN uint16_t uIndex) const
{
    return m_vecMediaFormats[uIndex].CStr();
}

//==============================================================================
//==
//==  SetMediaTypeId
//==
//==============================================================================
//
//  Parameters:
//      eMediaType:
//       The media type ID to set.
//
//  Description:
//      Sets the media type ID.
//
//==============================================================================
inline void CSdpFieldMediaAnnouncement::SetMediaTypeId(IN EMediaType eMediaType)
{
    m_eMediaType = eMediaType;
    m_strMediaType = GetMediaTypeStrFromId(eMediaType);
}

//==============================================================================
//==
//==  SetMediaTypeString
//==
//==============================================================================
//
//  Parameters:
//      pszMediaType:
//       The media type string to set.
//
//  Description:
//      Sets the media type string.
//
//==============================================================================
inline void
CSdpFieldMediaAnnouncement::SetMediaTypeString(IN const char* pszMediaType)
{
    m_strMediaType = pszMediaType;
    m_eMediaType = GetMediaTypeIdFromStr(pszMediaType);
}

//==============================================================================
//==
//==  SetTransportPort
//==
//==============================================================================
//
//  Parameters:
//      nTransportPort:
//       The transport port to set.
//
//  Description:
//      Sets the transport port for the media announcement field.
//
//==============================================================================
inline void
CSdpFieldMediaAnnouncement::SetTransportPort(IN int32_t nTransportPort)
{
    m_nTransportPort = nTransportPort;
}

//==============================================================================
//==
//==  SetTransportPort
//==
//==============================================================================
//
//  Parameters:
//      nNbTransportPorts:
//       The number of transport ports to set.
//
//  Description:
//      Sets the number of transport ports for the media announcement field.
//
//==============================================================================
inline void
CSdpFieldMediaAnnouncement::SetNbTransportPorts(IN int16_t nNbTransportPorts)
{
    m_nNbTransportPorts = nNbTransportPorts;
}

//==============================================================================
//==
//==  SetTransportProtocolId
//==
//==============================================================================
//
//  Parameters:
//      eTransportProtocol:
//       The transport protocol ID to set.
//
//  Description:
//      Sets the ETransportProtocol for the media announcement field.
//
//==============================================================================
inline void
CSdpFieldMediaAnnouncement::SetTransportProtocolId(IN ETransportProtocol eTransportProtocol)
{
    m_eTransportProtocol = eTransportProtocol;
    m_strTransportProtocol = GetTransportProtoStrFromId(eTransportProtocol);
}

//==============================================================================
//==
//==  SetTransportProtocolString
//==
//==============================================================================
//
//  Parameters:
//      pszTransportProtocol:
//       The transport protocol string to set.
//
//  Description:
//      Sets the transport protocol string for the media announcement field.
//
//==============================================================================
inline void
CSdpFieldMediaAnnouncement::SetTransportProtocolString(IN const char* pszTransportProtocol)
{
    m_strTransportProtocol = pszTransportProtocol;
    m_eTransportProtocol = GetTransportProtoIdFromStr(pszTransportProtocol);
}

//==============================================================================
//==
//==  AddMediaFormat
//==
//==============================================================================
//
//  Parameters:
//      pszMediaFormat:
//       The media format to append.
//
//  Description:
//      Adds one media format at the end of the list for the media announcement
//      field.
//
//==============================================================================
inline void
CSdpFieldMediaAnnouncement::AddMediaFormat(IN const char* pszMediaFormat)
{
    MX_ASSERT (pszMediaFormat != NULL);
    m_vecMediaFormats.Insert(m_vecMediaFormats.GetEndIndex(),
                             1,
                             pszMediaFormat);
}

//==============================================================================
//==
//==  SetMediaFormat
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//       Index of the media format to set.
//      pszMediaFormat:
//       The media format to set.
//
//  Description:
//      Sets one media format at the specified index in the list.
//
//==============================================================================
inline
void CSdpFieldMediaAnnouncement::SetMediaFormat(IN uint16_t    uIndex,
                                                IN const char* pszMediaFormat)
{
    MX_ASSERT(pszMediaFormat != NULL);
    m_vecMediaFormats[uIndex] = pszMediaFormat;
}

//==============================================================================
//==
//==  InsertMediaFormat
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//       Where to insert the new media format.
//      pszMediaFormat:
//       The media format to insert.
//
//  Description:
//      Inserts one media format at the specified index in the list.
//
//==============================================================================
inline
void CSdpFieldMediaAnnouncement::InsertMediaFormat(IN uint16_t    uIndex,
                                                   IN const char* pszMediaFormat)
{
    MX_ASSERT(pszMediaFormat != NULL);
    m_vecMediaFormats.Insert(uIndex, 1, pszMediaFormat);
}

//==============================================================================
//==
//==  RemoveMediaFormat
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//       The index of the media format to remove.
//
//  Description:
//      Removes one media format at the specified index in the media
//      announcement field.
//
//==============================================================================
inline void CSdpFieldMediaAnnouncement::RemoveMediaFormat(IN uint16_t uIndex)
{
    m_vecMediaFormats.Erase(uIndex);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDMEDIAANNOUNCEMENT_H
