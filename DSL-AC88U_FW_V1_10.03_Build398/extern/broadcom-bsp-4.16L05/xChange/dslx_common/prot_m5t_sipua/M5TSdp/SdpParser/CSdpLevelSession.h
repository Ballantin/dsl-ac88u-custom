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
#ifndef MXG_CSDPLEVELSESSION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPLEVELSESSION_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h"
#endif

// Data Member
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSDPFIELDPROTOCOLVERSION_H
#include "SdpParser/CSdpFieldProtocolVersion.h"
#endif

#ifndef MXG_CSDPFIELDORIGIN_H
#include "SdpParser/CSdpFieldOrigin.h"
#endif

#ifndef MXG_CSDPFIELDSESSIONNAME_H
#include "SdpParser/CSdpFieldSessionName.h"
#endif

#ifndef MXG_CSDPFIELDCONNECTIONDATA_H
#include "SdpParser/CSdpFieldConnectionData.h"
#endif

#ifndef MXG_CSDPFIELDTIME_H
#include "SdpParser/CSdpFieldTime.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEOTHER_H
#include "SdpParser/CSdpFieldAttributeOther.h"
#endif

#ifndef MXG_CSDPLEVELMEDIA_H
#include "SdpParser/CSdpLevelMedia.h"
#endif

#ifndef MXG_CSDPFIELDPHONE_H
#include "SdpParser/CSdpFieldPhone.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEKEYMGMT_H
#include "SdpParser/CSdpFieldAttributeKeyMgmt.h"
#endif

#ifndef MXG_CSDPKEYMANAGEMENTPARAMETER_H
#include "SdpParser/CSdpKeyManagementParameter.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEGROUP_H
#include "SdpParser/CSdpFieldAttributeGroup.h"
#endif

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

#ifndef MXG_CSDPFIELDATTRIBUTEICEPWD_H
#include "SdpParser/CSdpFieldAttributeIcePwd.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEICEUSERFRAG_H
#include "SdpParser/CSdpFieldAttributeIceUserFrag.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEICEOPTIONS_H
#include "SdpParser/CSdpFieldAttributeIceOptions.h"
#endif

#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

// Interface Realized & Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif



// Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)



// Forward Declarations Inside of the Namespace
class CBlob;
//M5T_INTERNAL_USE_END


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//==  Class: CSdpLevelSession
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of the level-session.
//
// Description:
//   This class is an abstraction of the level-session part of a SDP packet.
//   It is used to Set and Get several fields defined in the BNF of RFC 2327.
//
//   These fields are: Protocol Version, Origin, Time, Attributes and
//   Media Announcements.  There can be several types of attributes.
//
// RFC 2327 BNF:
//   <PRE>
//      level-session =  <Link CSdpFieldProtocolVersion, proto-version>
//                       <Link CSdpFieldOrigin, origin-field>
//                       <Link CSdpFieldSessionName, session-name-field>
//                       <Link CSdpFieldAttributeKeyMgmt, key-management-field>
//                       information-field
//                       uri-field
//                       email-fields
//                       phone-fields
//                       <Link CSdpFieldConnectionData, connection-field>
//                       bandwidth-fields
//                       <Link CSdpFieldTime, time-fields>
//                       key-field
//                       <Link CSdpFieldAttributeOther, attribute-fields>
//                       <Link CSdpLevelMedia, media-descriptions>
//
//   </PRE>
//
//  The parser also supports group field attribute implemented by
//  CSdpFieldAttributeGroup. It is defined in RFC 3388.
//
//  Fields with no links are not implemented yet, they are ignored when they
//  are parsed.
//
// Location:
//   SdpParser/CSdpLevelSession.h
//
// See Also:
//   CSdpFieldProtocolVersion, CSdpFieldOrigin, CSdpFieldSessionName,
//   CSdpFieldConnectionData, CSdpFieldAttributeOther, CSdpLevelMedia,
//   CSdpFieldAttributeGroup
//
//==============================================================================
class CSdpLevelSession : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpLevelSession();

    // Summary: Copy constructor.
    CSdpLevelSession(IN const CSdpLevelSession& rFrom);

    // Summary: Destructor.
    virtual ~CSdpLevelSession();

    // Summary: Comparison operator.
    bool operator==(IN const CSdpLevelSession& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpLevelSession& operator=(IN const CSdpLevelSession& rFrom);

    //-- << Get functions >>

    // Summary:
    //  Returns the protocol version, i.e. "v=" field.
    CSdpFieldProtocolVersion&   GetProtocolVersion();

    // Summary:
    //  Returns the origin, i.e. "o=" field.
    CSdpFieldOrigin&            GetOrigin();

    // Summary:
    //  Returns the session name, i.e. "s=" field.
    CSdpFieldSessionName&       GetSessionName();

    // Summary:
    //  Returns if the connection data is valid or not, i.e. "c=" field.
    bool                        IsValidConnectionData() const;

    // Summary:
    //  Returns the connection data, i.e. "c=" field.
    CSdpFieldConnectionData&    GetConnectionData();

    // Summary:
    //  Returns the number of time fields, i.e. "t=".
    uint32_t                    GetNbTimes() const;

    // Summary:
    //  Returns the time at the specified index.
    CSdpFieldTime&              GetTime(IN uint16_t uIndex);

    // Summary:
    //  The attribute "a=sendonly" was present.
    bool                        IsSendOnly() const;

    // Summary:
    //  The attribute "a=recvonly" was present.
    bool                        IsRecvOnly() const;

    // Summary:
    //  The attribute "a=sendrecv" was present.
    bool                        IsSendRecv() const;

    // Summary:
    //  The attribute "a=inactive" was present.
    bool                        IsInactive() const;

    // Summary:
    //  Gets the direction attribute
    //--(that is the one of the previous 4 methods that returns true if any does).
    EAttributeType GetDirection() const;

    // Summary:
    //  Returns the number of unknown attributes.
    uint32_t                        GetNbOtherAttributes() const;

    // Summary:
    //  Returns the unknown attribute at the specified index.
    CSdpFieldAttributeOther&        GetOtherAttribute(IN uint16_t uIndex);

    // Summary:
    //  Returns the number of level medias.
    uint32_t                        GetNbMedias() const;

    // Summary:
    //  Returns the level media at the specified index.
    CSdpLevelMedia&                 GetMedia(IN uint16_t uIndex);

    // Summary
    //   Returns the protocol version, i.e. "v=" field.
    const CSdpFieldProtocolVersion&   GetProtocolVersion() const;

    // Summary:
    //  Returns the origin, i.e. "o=" field.
    const CSdpFieldOrigin&            GetOrigin() const;

    // Summary:
    //  Returns the session name, i.e. "s=" field.
    const CSdpFieldSessionName&       GetSessionName() const;

    // Summary:
    //  Returns the connection data, i.e. "c=" field.
    const CSdpFieldConnectionData&    GetConnectionData() const;

    // Summary:
    //  Returns the time at the specified index.
    const CSdpFieldTime&              GetTime(IN uint16_t uIndex) const;

    // Summary:
    //  Returns the unknown attribute at the specified index.
    const CSdpFieldAttributeOther&    GetOtherAttribute(IN uint16_t uIndex) const;

    // Summary:
    //  Returns the level media at the specified index.
    const CSdpLevelMedia&             GetMedia(IN uint16_t uIndex) const;

    // Summary:
    //  Serializes the data contained in the level session.
    void                              Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Returns the number of phone fields.
    uint32_t                    GetNbPhones() const;

    // Summary:
    //  Returns the phone field at the specified index.
    const CSdpFieldPhone&       GetPhone(IN uint16_t uIndex) const;

    // Summary:
    //  Gets the number of key management attributes.
    uint32_t GetNbKeyMgmt() const;

    // Summary:
    //  Gets the number of key management parameters.
    uint32_t GetNbKeyMgmtParam() const;

    // Summary:
    //  Gets the key management attribute at the specified index.
    CSdpFieldAttributeKeyMgmt& GetKeyMgmt(IN uint16_t uIndex);
    const CSdpFieldAttributeKeyMgmt& GetKeyMgmt(IN uint16_t uIndex) const;

    // Summary:
    //  Gets the key management attribute parameter at the specified index.
    CSdpKeyManagementParameter& GetKeyMgmtParam(IN uint16_t uIndex);
    const CSdpKeyManagementParameter& GetKeyMgmtParam(IN uint16_t uIndex) const;

    // Summary:
    //  Gets the group attribute parameter at the specified index.
    CSdpFieldAttributeGroup* GetGroup(IN uint16_t uIndex);
    const CSdpFieldAttributeGroup* GetGroup(IN uint16_t uIndex) const;

    // Summary:
    //  Gets the number of group attributes.
    uint32_t GetNbGroup() const;

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
    // Summary:
    //  The attribute a=ice-lite is present.
    bool IsIceLite() const;

    // Summary:
    //  Returns the ice-ufrag attribute.
    CSdpFieldAttributeIceUserFrag& GetIceUserFragment();
    const CSdpFieldAttributeIceUserFrag& GetIceUserFragment() const;

    // Summary:
    //  Returns the ice-pwd attribute.
    CSdpFieldAttributeIcePwd& GetIcePassword();
    const CSdpFieldAttributeIcePwd& GetIcePassword() const;

    // Summary:
    //  Returns the ice-options attribute.
    CSdpFieldAttributeIceOptions& GetIceOptions();
    const CSdpFieldAttributeIceOptions& GetIceOptions() const;

    // Summary:
    //  Returns true if at least one ICE attribute is present at the session
    //  or media level.
    bool IsIceAttributePresent() const;
#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

    //-- << Set functions >>

    // Summary:
    //  Sets the protocol version, i.e. "v=" field.
    void SetProtocolVersion(IN CSdpFieldProtocolVersion& rProtocolVersion);

    // Summary:
    //  Sets the origin, i.e. "o=" field.
    void SetOrigin(IN const CSdpFieldOrigin& rOrigin);

    // Summary:
    //  Sets the session name, i.e. "s=" field.
    void SetSessionName(IN CSdpFieldSessionName& rSessionName);

    // Summary:
    //  Sets the connection data, i.e. "c=" field.
    void SetConnectionData(IN const CSdpFieldConnectionData& rConnectionData);

    // Summary:
    //  Adds a time field, i.e. "t=".
    void AddTime(IN CSdpFieldTime& rTime);

    // Summary:
    //  Sets an attribute to "sendonly" value.
    void SetSendOnly(IN bool bSendOnly);

    // Summary:
    //  Sets an attribute to "recvonly" value.
    void SetRecvOnly(IN bool bRecvOnly);

    // Summary:
    //  Sets an attribute to "sendrecv" value.
    void SetSendRecv(IN bool bSendRecv);

    // Summary:
    //  Sets an attribute to "inactive" value.
    void SetInactive(IN bool bInactive);

    // Summary:
    //  Sets the direction attribute
    //-- (same as previous 4 methods).
    void SetDirection(IN EAttributeType eDirection);

    // Summary:
    //  Sets whether or not to output the stream direction attributes when
    //  generating the packet.
    void SetSendDirection(IN bool bSend);

    // Summary:
    //  Adds an unknown attribute.
    void AddOtherAttribute(IN CSdpFieldAttributeOther& rOtherAttribute);

    // Summary:
    //  Adds a level media.
    void AddMedia(IN const CSdpLevelMedia& rMedia);

    // Summary:
    //  Inserts a level media at the specified index.
    void InsertMedia(IN uint32_t uIndex, IN const CSdpLevelMedia& rMedia);

    // Summary:
    //  Removes the level media at the specified index.
    void RemoveMedia(IN uint16_t uIndex);

    // Summary:
    //  Adds a phone field, i.e. "p=" field.
    void AddPhone(IN const CSdpFieldPhone& rPhone);

    // Summary:
    //  Gets the information field, i.e. "i=" field.
    const CString& GetInformation() const;

    // Summary:
    //  Sets the information field, i.e. "i=" field.
    void SetInformation(IN const char* szInformation);

#if defined(MXD_SDP_ENABLE_T38_SUPPORT)
    // Summary:
    //  Sets the T.38 boolean encoding method of the T.38 streams.
    //-------------------------------------------------------------
    void SetT38BooleanEncoding(bool bT38ImplicitEncoding);
#endif // MXD_SDP_ENABLE_T38_SUPPORT

    // Summary:
    //  Gets the URI field, i.e. "u=" field.
    const CString& GetUri() const;

    // Summary:
    //  Sets the URI field, i.e. "u=" field.
    void SetUri(IN const char* szUri);

    // Summary:
    //  Gets the vector of Email field, i.e. "e=" field.
    CVector<CString>& GetEmail();

    // Summary:
    //  Gets the vector of Email field, i.e. "e=" field.
    const CVector<CString>& GetEmail() const;

    // Summary:
    //  Gets the Encryption key field, i.e. "k=" field.
    const CString& GetEncryptionKey() const;

    // Summary:
    //  Sets the Encryption key field, i.e. "k=" field.
    void SetEncryptionKey(IN const char* szEncryptionKey);

    // Summary:
    //  Adds a key management to the session.
    void AddKeyMgmt(
        IN const CSdpFieldAttributeKeyMgmt& rKeyMgmt,
        IN const CSdpFieldAttributeKeyMgmt::EKeyManagementAttributeRole eRole =
            CSdpFieldAttributeKeyMgmt::eBOTH);

    // Summary:
    //  Adds a key management parameter to the session.
    void AddKeyMgmtParam(IN const CSdpKeyManagementParameter& rKeyMgmtParam);

    // Summary:
    //  Adds a group attribute field to the session.
    void AddGroup(IN const CSdpFieldAttributeGroup& rGroup);

    // Summary:
    //  Removes a group attribute field from the session.
    void RemoveGroup(IN uint16_t uIndex);

    // Summary:
    //  Removes the key management from the session.
    void RemoveKeyMgmt(IN uint16_t uIndex);

    // Summary:
    //  Removes the key management parameter from the session.
    void RemoveKeyMgmtParam(IN uint16_t uIndex);

    // Summary:
    // Gets the vector of Bandwidth field, i.e. "b=" field.
    CVector<CString>& GetBandwidth();

    // Summary:
    // Gets the vector of Bandwidth field, i.e. "b=" field.
    const CVector<CString>& GetBandwidth() const;

    // Summary:
    //  Checks that the group field attributes are valid.
    void ValidateGrouping();

    // Summary:
    //  Updates identification in group field attributes to match
    //  accepted medias.
    void UpdateGroupsIds();

    // Summary:
    //  Tells if the Id is present in a media and that media port is not 0.
    bool FindIdInMedias(IN const CString& strId, IN const bool bCheckDeactivatedMedia = false);

    // Summary:
    //  Returns true if the stream is the preferred one in the group
    //  it belongs.
    bool IsStreamPreferred(IN const CSdpLevelMedia& rOfferStream) const;

    // Summary:
    //  Returns the group in which the Mid is found.
    const CSdpFieldAttributeGroup* FindGroupOfAMid(IN const CString& strMid) const;

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
    // Summary:
    //  Sets if an a=ice-lite attrbute is present.
    void SetIceLite(IN bool bIsIceLite);
#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

    //-- << Virtual functions >>
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

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:

    EParserResult ParseAttribute(IN const char*& rpszStartPosition, OUT mxt_result& rres);
    EParserResult ParseTime(IN const char*& rpszStartPosition, OUT mxt_result& rres);
    EParserResult ParseMediaAnnouncement(IN const char*& rpszStartPosition, OUT mxt_result& rres);
    EParserResult ParsePhone(IN const char*& rpStartPosition, OUT mxt_result& rres);

    EParserResult ParseKeyMgmt(IN  const char*& rpszStartPosition,
                               OUT mxt_result& rres);

    // Summary:
    //  Copies the key management attributes and their related parameters.
    void CopyKeyMgmtsFrom(
                    IN const CVector<CSdpFieldAttributeKeyMgmt*>& rvecpFrom,
                    IN const CVector<CSdpKeyManagementParameter*>& rvecpFromParam);

    // Summary:
    //  Copies the key management attributes and their related parameters.
    void CopyKeyMgmtsFrom(
                    IN const CVector<CSdpFieldAttributeKeyMgmt*>& rvecpFrom,
                    IN const CVector<CSdpKeyManagementParameter*>& rvecpFromParam) const;

// Hidden Data Members
protected:
    // Contains protocol field, i.e. "v=" field.
    CSdpFieldProtocolVersion    m_protocolVersion;

    // Contains origin field, i.e. "o=" field.
    CSdpFieldOrigin             m_origin;

    // Contains session name field, i.e. "s=" field.
    CSdpFieldSessionName        m_sessionName;

    // Contains connection field, i.e. "c=" field.
    CSdpFieldConnectionData     m_connectionData;

    // Contains time fields , i.e. "t=" field.
    // with their corresponding time zone and repeat fields.
    CVector <CSdpFieldTime>     m_vecTimes;

    // Direction attribute e.g. a=sendrecv.
    EAttributeType              m_eDirection;

    // Key management attribute e.g. a=key-mgmt.
    CVector<CSdpFieldAttributeKeyMgmt*> m_vecpKeyMgmt;

    // Key management parameters for the key management attribute.
    CVector<CSdpKeyManagementParameter*> m_vecpKeyMgmtParam;

    // If set to true, we will serialize the direction attribute contained in
    // m_eDirection.
    bool                        m_bSendDirection;

    // Contains unsupported attribute fields. i.e. Begins with "a=".
    CVector <CSdpFieldAttributeOther> m_vecOtherAttributes;

    // Contains the media descriptions. i.e. Begins with "m=".
    CVector <CSdpLevelMedia>    m_vecMedias;

    // Contains the phone fields, i.e. "p=" field.
    CVector <CSdpFieldPhone>    m_vecPhones;

    // Contains the information fields, i.e. "i=" field.
    CString                     m_strInformation;

    // Contains the URI fields, i.e. "u=" field.
    CString                     m_strUri;

    // Contains the email fields, i.e. "e=" field.
    CVector <CString>           m_vecstrEmail;

    // Contains the key field, i.e. "k=" field.
    CString                     m_strEncryptionKey;

    // Contains the bandwidth fields, i.e. "b=" field.
    CVector <CString>           m_vecstrBandwidth;

    // Contains the group field i.e. Begins with "a=group:".
    CVector <CSdpFieldAttributeGroup> m_vecGroupAttributes;

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
    // If true, a ice-lite parameter is present.
    bool m_bIceLite;

    // The ice-ufrag attribute.
    CSdpFieldAttributeIceUserFrag m_iceUserFragment;

    // The ice-pwd attribute.
    CSdpFieldAttributeIcePwd m_icePassword;

    // The ice-options attribute.
    CSdpFieldAttributeIceOptions m_iceOptions;

    // If true, contains at least one ICE attribute.
    bool m_bIsIceSupported;
#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetProtocolVersion
//==
//==============================================================================
//
//  Returns:
//      Reference to the requested CSdpFieldProtocolVersion.
//
//  Description:
//      Returns the protocol version, i.e. "v=" field.
//
//==============================================================================
inline CSdpFieldProtocolVersion&
CSdpLevelSession::GetProtocolVersion()
{
    return m_protocolVersion;
}

// <COMBINE CSdpLevelSession::GetProtocolVersion>
inline const CSdpFieldProtocolVersion&
CSdpLevelSession::GetProtocolVersion() const
{
    return m_protocolVersion;
}

//==============================================================================
//==
//==  GetOrigin
//==
//==============================================================================
//
//  Returns:
//      Reference to the requested CSdpFieldOrigin.
//
//  Description:
//      Returns the origin, i.e. "o=" field.
//
//==============================================================================
inline CSdpFieldOrigin&
CSdpLevelSession::GetOrigin()
{
    return m_origin;
}

// <COMBINE CSdpLevelSession::GetOrigin>
inline const CSdpFieldOrigin&
CSdpLevelSession::GetOrigin() const
{
    return m_origin;
}

//==============================================================================
//==
//==  GetSessionName
//==
//==============================================================================
//
//  Returns:
//      Reference to the requested CSdpFieldSessionName.
//
//  Description:
//      Returns the session name, i.e. "s=" field.
//
//==============================================================================
inline CSdpFieldSessionName&
CSdpLevelSession::GetSessionName()
{
    return m_sessionName;
}

// <COMBINE CSdpLevelSession::GetSessionName>
inline const CSdpFieldSessionName&
CSdpLevelSession::GetSessionName() const
{
    return m_sessionName;
}

//==============================================================================
//==
//==  IsValidConnectionData
//==
//==============================================================================
//
//  Returns:
//      - True: Tthe connection data is valid
//      - False: The connection data is not valid
//
//  Description:
//      Verifies whether or not the connection data is valid, i.e. "c=" field.
//
//==============================================================================
inline bool
CSdpLevelSession::IsValidConnectionData() const
{
    return m_connectionData.IsValid();
}

//==============================================================================
//==
//==  GetConnectionData
//==
//==============================================================================
//
//  Returns:
//      Reference to the CSdpFieldConnectionData.
//
//  Description:
//      Returns the connection data, i.e. "c=" field.
//
//==============================================================================
inline CSdpFieldConnectionData&
CSdpLevelSession::GetConnectionData()
{
    return m_connectionData;
}

// <COMBINE CSdpLevelSession::GetConnectionData>
inline const CSdpFieldConnectionData&
CSdpLevelSession::GetConnectionData() const
{
    return m_connectionData;
}

//==============================================================================
//==
//==  GetNbTimes
//==
//==============================================================================
//
//  Returns:
//      Number of time fields.
//
//  Description:
//      Returns the number of time fields, i.e. "t=".
//
//==============================================================================
inline uint32_t CSdpLevelSession::GetNbTimes() const
{
    return m_vecTimes.GetSize();
}

//==============================================================================
//==
//==  GetTime
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          index of the times vector.
//
//  Returns:
//      Reference to the CSdpFieldTime.
//
//  Description:
//      Returns the time at the specified index.
//
//==============================================================================
inline CSdpFieldTime&
CSdpLevelSession::GetTime(IN uint16_t uIndex)
{
    return m_vecTimes[uIndex];
}

// <COMBINE CSdpLevelSession::GetTime>
inline const CSdpFieldTime&
CSdpLevelSession::GetTime(IN uint16_t uIndex) const
{
    return m_vecTimes[uIndex];
}

//==============================================================================
//==
//==  IsSendOnly
//==
//==============================================================================
//
//  Returns:
//      - True: The attribute "a=sendonly" is present.
//      - False: The attribute "a=sendonly" is not present.
//
//  Description:
//      Verifies if the attribute "a=sendonly" is present.
//
//==============================================================================
inline bool CSdpLevelSession::IsSendOnly() const
{
    return m_eDirection == eSENDONLY;
}

//==============================================================================
//==
//==  IsRecvOnly
//==
//==============================================================================
//
//  Returns:
//      - True: The attribute "a=recvonly" is present.
//      - False: The attribute "a=recvonly" is not present.
//
//  Description:
//      Verifies if the attribute "a=recvonly" is present.
//
//==============================================================================
inline bool CSdpLevelSession::IsRecvOnly() const
{
    return m_eDirection == eRECVONLY;
}

//==============================================================================
//==
//==  IsSendRecv
//==
//==============================================================================
//
//  Returns:
//      - True: The attribute "a=sendrecv" is present.
//      - False: The attribute "a=sendrecv" is not present.
//
//  Description:
//      Verifies if the attribute "a=sendrecv" is present.
//
//==============================================================================
inline bool CSdpLevelSession::IsSendRecv() const
{
    return m_eDirection == eSENDRECV || m_eDirection == eUNKNOWN_ATTRIBUTE_TYPE;
}

//==============================================================================
//==
//==  IsInactive
//==
//==============================================================================
//
//  Returns:
//      - True: The attribute "a=inactive" is present.
//      - False: The attribute "a=inactive" is not present.
//
//  Description:
//      Verifies if the attribute "a=inactive" is present.
//
//==============================================================================
inline bool CSdpLevelSession::IsInactive() const
{
    return m_eDirection == eINACTIVE;
}

//==============================================================================
//==
//==  GetDirection
//==
//==============================================================================
//
//  Returns:
//      The direction attribute.
//
//  Description:
//      Gets the direction attribute.
//
//  See Also:
//     IsSendOnly, IsRecvOnly, IsSendRecv, IsInactive
//
//==============================================================================
inline CSdpParser::EAttributeType CSdpLevelSession::GetDirection() const
{
    return m_eDirection;
}

//==============================================================================
//==
//==  GetNbOtherAttributes
//==
//==============================================================================
//
//  Returns:
//      Number of unknown attributes.
//
//  Description:
//      Returns the number of unknown attributes.
//
//==============================================================================
inline uint32_t CSdpLevelSession::GetNbOtherAttributes() const
{
    return m_vecOtherAttributes.GetSize();
}

//==============================================================================
//==
//==  GetOtherAttribute
//==
//==============================================================================
//
//  Returns:
//      Reference to the requested CSdpFieldAttributeOther.
//
//  Parameters:
//    uIndex:
//      The index of the attribute to retrieve.
//
//  Description:
//      Gets the unknown attribute at the specified index.
//
//==============================================================================
inline CSdpFieldAttributeOther&
CSdpLevelSession::GetOtherAttribute(IN uint16_t uIndex)
{
    return m_vecOtherAttributes[uIndex];
}

// <COMBINE CSdpLevelSession::GetOtherAttribute>
inline const CSdpFieldAttributeOther&
CSdpLevelSession::GetOtherAttribute(IN uint16_t uIndex) const
{
    return m_vecOtherAttributes[uIndex];
}

//==============================================================================
//==
//==  GetNbMedias
//==
//==============================================================================
//
//  Returns:
//    The number of level medias.
//
//  Description:
//      Gets the number of level medias.
//
//==============================================================================
inline uint32_t CSdpLevelSession::GetNbMedias() const
{
    return m_vecMedias.GetSize();
}

//==============================================================================
//==
//==  GetMedia
//==
//==============================================================================
//
//  Returns:
//      Reference to the requested CSdpLevelMedia.
//
//  Parameters:
//    uIndex:
//      The index of the media to retrieve.
//
//  Description:
//      Gets the level media at the specified index.
//
//==============================================================================
inline CSdpLevelMedia&
CSdpLevelSession::GetMedia(IN uint16_t uIndex)
{
    return m_vecMedias[uIndex];
}

// <COMBINE CSdpLevelSession::GetMedia>
inline const CSdpLevelMedia&
CSdpLevelSession::GetMedia(IN uint16_t uIndex) const
{
    return m_vecMedias[uIndex];
}

//==============================================================================
//==
//==  GetNbKeyMgmt
//==
//==============================================================================
//
//  Returns:
//    The number of key management attributes in the session.
//
//  Description:
//      Gets the number of key management attributes in the session.
//
//==============================================================================
inline uint32_t CSdpLevelSession::GetNbKeyMgmt() const
{
    return m_vecpKeyMgmt.GetSize();
}

//==============================================================================
//==
//==  GetNbKeyMgmtParam
//==
//==============================================================================
//
//  Returns:
//    The number of key management parameters in the session.
//
//
//  Description:
//      Gets the number of key management parameters in the session.
//
//==============================================================================
inline uint32_t CSdpLevelSession::GetNbKeyMgmtParam() const
{
    return m_vecpKeyMgmtParam.GetSize();
}

//==============================================================================
//==
//==  GetKeyMgmt
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the attribute to retrieve.
//
//  Description:
//      Gets the specified key management attribute from the session.
//
//==============================================================================
inline CSdpFieldAttributeKeyMgmt& CSdpLevelSession::GetKeyMgmt(IN uint16_t uIndex)
{
    return *(m_vecpKeyMgmt[uIndex]);
}

// <COMBINE CSdpLevelSession::GetKeyMgmt>
inline const CSdpFieldAttributeKeyMgmt& CSdpLevelSession::GetKeyMgmt(IN uint16_t uIndex) const
{
    return *(m_vecpKeyMgmt[uIndex]);
}

//==============================================================================
//==
//==  GetKeyMgmtParam
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the attribute parameter to retrieve.
//
//  Description:
//      Gets the specified key management parameter from the session.
//
//==============================================================================
inline CSdpKeyManagementParameter& CSdpLevelSession::GetKeyMgmtParam(IN uint16_t uIndex)
{
    return *(m_vecpKeyMgmtParam[uIndex]);
}

// <COMBINE CSdpLevelSession::GetKeyMgmtParam>
inline const CSdpKeyManagementParameter& CSdpLevelSession::GetKeyMgmtParam(IN uint16_t uIndex) const
{
    return *(m_vecpKeyMgmtParam[uIndex]);
}

//==============================================================================
//==
//==  SetProtocolVersion
//==
//==============================================================================
//
//  Parameters:
//    rProtocolVersion:
//      Reference to the CSdpFieldProtocolVersion to set.
//
//  Description:
//      Sets the protocol version, i.e. "v=" field.
//
//==============================================================================
inline void
CSdpLevelSession::SetProtocolVersion(IN CSdpFieldProtocolVersion& rProtocolVersion)
{
    m_protocolVersion = rProtocolVersion;
}

//==============================================================================
//==
//==  SetOrigin
//==
//==============================================================================
//
//  Parameters:
//    rOrigin:
//      Reference to the CSdpFieldOrigin to set.
//
//  Description:
//      Sets the origin, i.e. "o=" field.
//
//==============================================================================
inline void CSdpLevelSession::SetOrigin(IN const CSdpFieldOrigin& rOrigin)
{
    m_origin = rOrigin;
}

//==============================================================================
//==
//==  SetSessionName
//==
//==============================================================================
//
//  Parameters:
//    rOrigin:
//      Reference to the CSdpFieldSessionName to set.
//
//  Description:
//      Sets the session name, i.e. "s=" field.
//
//==============================================================================
inline void CSdpLevelSession::SetSessionName(IN CSdpFieldSessionName& rSessionName)
{
    m_sessionName = rSessionName;
}

//==============================================================================
//==
//==  SetConnectionData
//==
//==============================================================================
//
//  Parameters:
//      rConnectionData:
//        Reference to the CSdpFieldConnectionData to set.
//
//  Description:
//      Sets the connection data, i.e. "c=" field.
//
//==============================================================================
inline void
CSdpLevelSession::SetConnectionData(IN const CSdpFieldConnectionData& rConnectionData)
{
    m_connectionData = rConnectionData;
}

//==============================================================================
//==
//==  AddTime
//==
//==============================================================================
//
//  Parameters:
//      rTime:
//        Reference to the CSdpFieldTime to add.
//
//  Description:
//      Adds a time field, i.e. "t=".  More than one can be
//      added to the same session.
//
//==============================================================================
inline void CSdpLevelSession::AddTime(IN CSdpFieldTime& rTime)
{
    m_vecTimes.Insert(m_vecTimes.GetEndIndex(), 1, rTime);
}

//==============================================================================
//==
//==  SetSendOnly
//==
//==============================================================================
//
//  Parameters:
//      bSendOnly:
//        - True to set the sendonly value.
//        - False otherwise.
//
//  Description:
//      Sets an attribute to "sendonly" value.
//
//==============================================================================
inline void CSdpLevelSession::SetSendOnly(IN bool bSendOnly)
{
    if (bSendOnly)
    {
        m_eDirection = eSENDONLY;
        m_bSendDirection = true;
    }
    else if (m_eDirection == eSENDONLY)
    {
        m_eDirection = eUNKNOWN_ATTRIBUTE_TYPE;
        m_bSendDirection = false;
    }
}

//==============================================================================
//==
//==  SetRecvOnly
//==
//==============================================================================
//
//  Parameters:
//      bRecvOnly:
//        - True to set the recvonly value.
//        - False otherwise.
//
//  Description:
//      Sets an attribute to "recvonly" value.
//
//==============================================================================
inline void CSdpLevelSession::SetRecvOnly(IN bool bRecvOnly)
{
    if (bRecvOnly)
    {
        m_eDirection = eRECVONLY;
        m_bSendDirection = true;
    }
    else if (m_eDirection == eRECVONLY)
    {
        m_eDirection = eUNKNOWN_ATTRIBUTE_TYPE;
        m_bSendDirection = false;
    }
}

//==============================================================================
//==
//==  SetSendRecv
//==
//==============================================================================
//
//  Parameters:
//      bSendRecv:
//        - True to set the sendrecv value.
//        - False otherwise.
//
//  Description:
//      Sets an attribute to "sendrecv" value.
//
//==============================================================================
inline void CSdpLevelSession::SetSendRecv(IN bool bSendRecv)
{
    if (bSendRecv)
    {
        m_eDirection = eSENDRECV;
        m_bSendDirection = true;
    }
    else if (m_eDirection == eSENDRECV)
    {
        m_eDirection = eUNKNOWN_ATTRIBUTE_TYPE;
        m_bSendDirection = false;
    }
}

//==============================================================================
//==
//==  SetInactive
//==
//==============================================================================
//
//  Parameters:
//      bInactive:
//        - True to set the inactive value.
//        - False otherwise.
//
//  Description:
//      Sets an attribute to "inactive" value.
//
//==============================================================================
inline void CSdpLevelSession::SetInactive(IN bool bInactive)
{
    if (bInactive)
    {
        m_eDirection = eINACTIVE;
        m_bSendDirection = true;
    }
    else if (m_eDirection == eINACTIVE)
    {
        m_eDirection = eUNKNOWN_ATTRIBUTE_TYPE;
        m_bSendDirection = false;
    }
}

//==============================================================================
//==
//==  SetDirection
//==
//==============================================================================
//
//  Parameters:
//      eDirection:
//        Direction attribute (EAttributeType)
//
//  Description:
//      Sets the direction attribute
//
//  See Also:
//      SetSendOnly, SetRecvOnly, SetSendRecv, SetInactive
//
//==============================================================================
inline void CSdpLevelSession::SetDirection(IN EAttributeType eDirection)
{
    MX_ASSERT(eDirection == eUNKNOWN_ATTRIBUTE_TYPE ||
        eDirection == eSENDRECV || eDirection == eSENDONLY ||
        eDirection == eRECVONLY || eDirection == eINACTIVE);

    m_eDirection = eDirection;

    if (eDirection != eUNKNOWN_ATTRIBUTE_TYPE)
    {
        m_bSendDirection = true;
    }
}

//==============================================================================
//==
//==  AddOtherAttribute
//==
//==============================================================================
//
//  Parameters:
//      rOtherAttribute:
//        Reference to the CSdpFieldAttributeOther to add.
//
//  Description:
//      Adds an unknown attribute. More than one can be added to the same
//      session.
//
//==============================================================================
inline void
CSdpLevelSession::AddOtherAttribute(IN CSdpFieldAttributeOther& rOtherAttribute)
{
    m_vecOtherAttributes.Insert(m_vecOtherAttributes.GetEndIndex(),
                                1,
                                rOtherAttribute);
}

//==============================================================================
//==
//==  AddOtherAttribute
//==
//==============================================================================
//
//  Parameters:
//      rMedia:
//        Reference to the CSdpLevelMedia to add.
//
//  Description:
//      Adds a level media. More than one can be added to the same
//      session.
//
//==============================================================================
inline void CSdpLevelSession::AddMedia(IN const CSdpLevelMedia& rMedia)
{
    m_vecMedias.Insert(m_vecMedias.GetEndIndex(), 1, rMedia);
    m_vecMedias[m_vecMedias.GetSize() - 1].SetSession(this);
}

//==============================================================================
//==
//==  InsertMedia
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//        Index of where to insert the media.
//
//      rMedia:
//        Reference of the CSdpLevelMedia to insert.
//
//  Description:
//      Inserts a level media at the specified index.
//
//==============================================================================
inline void CSdpLevelSession::InsertMedia( IN uint32_t uIndex,
                                           IN const CSdpLevelMedia& rMedia )
{
    m_vecMedias.Insert(uIndex, 1, rMedia);
    m_vecMedias[uIndex].SetSession(this);
}

//==============================================================================
//==
//==  RemoveMedia
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//        Index to remove from the media vector.
//
//  Description:
//      Removes the level media at the specified index.
//
//==============================================================================
inline void CSdpLevelSession::RemoveMedia(IN uint16_t uIndex)
{
    if (uIndex < m_vecMedias.GetSize())
    {
        m_vecMedias.Erase(uIndex);
    }
}

//==============================================================================
//==
//==  AddPhone
//==
//==============================================================================
//
//  Parameters:
//      rPhone:
//        Reference to the CSdpFieldPhone to add.
//
//  Description:
//      Adds a phone field, i.e. "p=" field. More than one can be
//      added to the same session.
//
//==============================================================================
inline void CSdpLevelSession::AddPhone(IN const CSdpFieldPhone& rPhone)
{
    m_vecPhones.Insert(m_vecPhones.GetEndIndex(), 1, rPhone);
}

//==============================================================================
//==
//==  GetNbPhones
//==
//==============================================================================
//
//  Returns:
//      Number of phone fields.
//
//  Description:
//      Returns the number of phone fields, i.e. "p=".
//
//==============================================================================
inline uint32_t CSdpLevelSession::GetNbPhones() const
{
    return m_vecPhones.GetSize();
}

//==============================================================================
//==
//==  GetPhone
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//        Index of the requested phone field.
//
//  Returns:
//      Reference to the CSdpFieldPhone requested
//
//  Description:
//      Returns the phone field at the specified index.
//
//==============================================================================
inline const CSdpFieldPhone&
CSdpLevelSession::GetPhone(IN uint16_t uIndex) const
{
    return m_vecPhones[uIndex];
}

//==============================================================================
//==
//==  GetInformation const
//==
//==============================================================================
//
//  Returns:
//      CString reference to the information field.
//
//  Description:
//      Gets the information field, i.e. "i=" field.
//
//==============================================================================
inline const CString& CSdpLevelSession::GetInformation() const
{
    return m_strInformation;
}

//==============================================================================
//==
//==  SetInformation
//==
//==============================================================================
//
//  Parameters:
//      szInformation:
//        The information field content to set.
//
//  Description:
//      Sets the information field, i.e. "i=" field.
//
//==============================================================================
inline void CSdpLevelSession::SetInformation(IN const char* szInformation)
{
    m_strInformation = szInformation;
}

//==============================================================================
//==
//==  GetUri const
//==
//==============================================================================
//
//  Returns:
//      CString reference to the URI field.
//
//  Description:
//      Gets the URI field, i.e. "u=" field.
//
//==============================================================================
inline const CString& CSdpLevelSession::GetUri() const
{
    return m_strUri;
}

//==============================================================================
//==
//==  SetUri
//==
//==============================================================================
//
//  Parameters:
//      szUri:
//        The URI field content to set.
//
//  Description:
//      Sets the URI field, i.e. "u=" field.
//
//==============================================================================
inline void CSdpLevelSession::SetUri(IN const char* szUri)
{
    m_strUri = szUri;
}

//==============================================================================
//==
//==  GetEmail
//==
//==============================================================================
//
//  Returns:
//      Reference to the vector of email field.
//
//  Description:
//      Gets the vector of Email field, i.e. "e=" field.
//
//==============================================================================
inline CVector<CString>& CSdpLevelSession::GetEmail()
{
    return m_vecstrEmail;
}

// <COMBINE CSdpLevelSession::GetEmail>
inline const CVector<CString>& CSdpLevelSession::GetEmail() const
{
    return m_vecstrEmail;
}

//==============================================================================
//==
//==  GetEncryptionKey
//==
//==============================================================================
//
//  Returns:
//      Reference to the CString encryption key.
//
//  Description:
//      Gets the Encryption key field, i.e. "k=" field.
//
//==============================================================================
inline const CString& CSdpLevelSession::GetEncryptionKey() const
{
    return m_strEncryptionKey;
}

//==============================================================================
//==
//==  SetEncryptionKey
//==
//==============================================================================
//
//  Parameters:
//      szEncryptionKey:
//        The Encryption key field content to set.
//
//  Description:
//      Sets the Encryption key field, i.e. "k=" field.
//
//==============================================================================
inline void CSdpLevelSession::SetEncryptionKey(IN const char* szEncryptionKey)
{
    m_strEncryptionKey = szEncryptionKey;
}

//==============================================================================
//==
//==  GetBandwidth
//==
//==============================================================================
//
//  Returns:
//      Reference to the bandwidth vector.
//
//  Description:
//      Gets the vector of Bandwidth field, i.e. "b=" field.
//
//==============================================================================
inline CVector<CString>& CSdpLevelSession::GetBandwidth()
{
    return m_vecstrBandwidth;
}

// <COMBINE CSdpLevelSession::GetBandwidth>
inline const CVector<CString>& CSdpLevelSession::GetBandwidth() const
{
    return m_vecstrBandwidth;
}

//==============================================================================
//==
//==  GetGroup
//==
//==============================================================================
//
//  Returns:
//      The specified group from the session. NULL is returned if the
//      index is greater than or equal to the number of groups.
//
//  Parameters:
//    uIndex:
//      The index of the attribute to retrieve.
//
//  Description:
//      Gets the specified group from the session. uIndex must be smaller
//      than the actual number of groups. NULL is returned otherwise.
//
//==============================================================================
inline CSdpFieldAttributeGroup* CSdpLevelSession::GetGroup(IN uint16_t uIndex)
{
    return &m_vecGroupAttributes[uIndex];
}

// <COMBINE CSdpLevelSession::GetGroup>
inline const CSdpFieldAttributeGroup* CSdpLevelSession::GetGroup(IN uint16_t uIndex) const
{
    return &m_vecGroupAttributes[uIndex];
}

//==============================================================================
//==
//==  GetNbGroup
//==
//==============================================================================
//
//  Returns:
//      The number of group attributes in the session.
//
//  Description:
//      Returns the number of group attributes in the session.
//
//==============================================================================
inline uint32_t CSdpLevelSession::GetNbGroup() const
{
    return m_vecGroupAttributes.GetSize();
}

//==============================================================================
//==
//==  AddGroup
//==
//==============================================================================
//
//  Parameters:
//    rGroup:
//      The group attribute field to add to the session.
//
//  Description:
//      Adds a "a=group:" field to the session. More than one can be
//      added to the same session.
//
//==============================================================================
inline void CSdpLevelSession::AddGroup(IN const CSdpFieldAttributeGroup& rGroup)
{
    m_vecGroupAttributes.Append(rGroup);
}

//==============================================================================
//==
//==  RemoveGroup
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the attribute to remove.
//
//  Description:
//      Removes a "a=group:" field from the session.
//
//==============================================================================
inline void CSdpLevelSession::RemoveGroup(IN uint16_t uIndex)
{
    m_vecGroupAttributes.Erase(uIndex);
}

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
//==============================================================================
//==
//==  IsIceLite
//==
//==============================================================================
//
//  Returns:
//      - true: If an a=ice-lite attribute is present.
//      - false: Otherwise.
//
//  Description:
//      Returns whether or not an a=ice-lite attribute is present.
//
//==============================================================================
inline bool CSdpLevelSession::IsIceLite() const
{
    return m_bIceLite;
}

//==============================================================================
//==
//==  SetIceLite
//==
//==============================================================================
//
//  Parameters:
//      bIsIceLite:
//       Whether or not an a=ice-lite attribute is present.
//
//  Description:
//      Sets whether or not an a=ice-lite attribute is present.
//
//==============================================================================
inline void CSdpLevelSession::SetIceLite(IN bool bIsIceLite)
{
    m_bIceLite = bIsIceLite;
}

//==============================================================================
//==
//==  GetIceUserFragment
//==
//==============================================================================
//
//  Returns:
//      A reference to CSdpFieldAttributeIceUserFrag.
//
//  Description:
//      Returns a reference to the ICE user fragment.
//
//==============================================================================
inline CSdpFieldAttributeIceUserFrag& CSdpLevelSession::GetIceUserFragment()
{
    return m_iceUserFragment;
}

// <COMBINE CSdpLevelSession::GetIceUserFragment>
inline const CSdpFieldAttributeIceUserFrag& CSdpLevelSession::GetIceUserFragment() const
{
    return m_iceUserFragment;
}

//==============================================================================
//==
//==  GetIcePassword
//==
//==============================================================================
//
//  Returns:
//      A reference to CSdpFieldAttributeIcePwd.
//
//  Description:
//       Returns a reference to the ICE password.
//
//==============================================================================
inline CSdpFieldAttributeIcePwd& CSdpLevelSession::GetIcePassword()
{
    return m_icePassword;
}

// <COMBINE CSdpLevelSession::GetIcePassword>
inline const CSdpFieldAttributeIcePwd& CSdpLevelSession::GetIcePassword() const
{
    return m_icePassword;
}

//==============================================================================
//==
//==  GetIceOptions
//==
//==============================================================================
//
//  Returns:
//      A reference to CSdpFieldAttributeIceOptions.
//
//  Description:
//       Returns a reference to the ICE options.
//
//==============================================================================
inline CSdpFieldAttributeIceOptions& CSdpLevelSession::GetIceOptions()
{
    return m_iceOptions;
}

// <COMBINE CSdpLevelSession::GetIceOptions>
inline const CSdpFieldAttributeIceOptions& CSdpLevelSession::GetIceOptions() const
{
    return m_iceOptions;
}

//==============================================================================
//==
//==  IsIceAttributePresent
//==
//==============================================================================
//
//  Returns:
//      true if the session or medias contain at least one ICE related
//      attributes.
//
//  Description:
//      Returns true if the media contains at least one of the ICE attributes:
//      ice-lite, ice-ufrag, ice-pwd, ice-options, candidate or
//      remote-candidates.
//
//==============================================================================
inline bool CSdpLevelSession::IsIceAttributePresent() const
{
    return m_bIsIceSupported;
}

#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPLEVELSESSION_H
