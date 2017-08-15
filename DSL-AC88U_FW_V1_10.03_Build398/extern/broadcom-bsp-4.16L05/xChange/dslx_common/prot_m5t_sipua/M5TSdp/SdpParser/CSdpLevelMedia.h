//==============================================================================
//==============================================================================
//
//   Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_CSDPLEVELMEDIA_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPLEVELMEDIA_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SDP Parser Configuration
#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h"  // MXD_SDP_ENABLE_T38_SUPPORT,
                                  // MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT
#endif


//-- Data Members
#ifndef MXG_CPAIR_H
#include "Cap/CPair.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTECRYPTO_H
#include "SdpParser/CSdpFieldAttributeCrypto.h"
#endif

#ifndef MXG_CSDPFIELDMEDIAANNOUNCEMENT_H
#include "SdpParser/CSdpFieldMediaAnnouncement.h"
#endif

#ifndef MXG_CSDPFIELDCONNECTIONDATA_H
#include "SdpParser/CSdpFieldConnectionData.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEFMTP_H
#include "SdpParser/CSdpFieldAttributeFmtp.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEKEYMGMT_H
#include "SdpParser/CSdpFieldAttributeKeyMgmt.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTERTPMAP_H
#include "SdpParser/CSdpFieldAttributeRtpmap.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEPTIME_H
#include "SdpParser/CSdpFieldAttributePtime.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTESILENCESUPP_H
#include "SdpParser/CSdpFieldAttributeSilenceSupp.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEOTHER_H
#include "SdpParser/CSdpFieldAttributeOther.h"
#endif

#ifndef MXG_CSDPFMTPREDUNDANCY_H
#include "SdpParser/CSdpFmtpRedundancy.h"
#endif

#ifndef MXG_CSDPFMTPTELEVENT_H
#include "SdpParser/CSdpFmtpTelEvent.h"
#endif

// BRCM_CUSTOM - [add] Video support
#if defined(MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT)
   #ifndef MXG_CSDPFMTPH263_H
   #include "SdpParser/CSdpFmtpH263.h"
   #endif

   #ifndef MXG_CSDPFMTPH264_H
   #include "SdpParser/CSdpFmtpH264.h"
   #endif
#endif
// BRCM_CUSTOM - [end] Video support

#if defined( MXD_SDP_ENABLE_T38_SUPPORT )
#ifndef MXG_CSDPFIELDATTRIBUTEFILLBITREMOVAL_H
#include "SdpParser/CSdpFieldAttributeFillBitRemoval.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEMAXBITRATE_H
#include "SdpParser/CSdpFieldAttributeMaxBitRate.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEMAXDATAGRAM_H
#include "SdpParser/CSdpFieldAttributeMaxDatagram.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTET38ERRORCONTROL_H
#include "SdpParser/CSdpFieldAttributeT38ErrorControl.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTET38FACSIMILEMAXBUFFER_H
#include "SdpParser/CSdpFieldAttributeT38FacsimileMaxBuffer.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTET38FACSIMILERATEMGMNT_H
#include "SdpParser/CSdpFieldAttributeT38FacsimileRateMgmnt.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTETRANSCODING_H
#include "SdpParser/CSdpFieldAttributeTranscoding.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTETRANSCODINGMMR_H
#include "SdpParser/CSdpFieldAttributeTranscodingMMR.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTETRANSCODINGJBIG_H
#include "SdpParser/CSdpFieldAttributeTranscodingJBIG.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEVERSION_H
#include "SdpParser/CSdpFieldAttributeVersion.h"
#endif

#endif // MXD_SDP_ENABLE_T38_SUPPORT

#ifndef MXG_CSDPFIELDATTRIBUTERTCP_H
#include "SdpParser/CSdpFieldAttributeRtcp.h"
#endif

#ifndef MXG_CSDPKEYMANAGEMENTPARAMETER_H
#include "SdpParser/CSdpKeyManagementParameter.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEMID_H
#include "SdpParser/CSdpFieldAttributeMid.h"
#endif

// BRCM_CUSTOM - [add] Precondition support
#ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support
#ifndef MXG_CSDPFIELDATTRIBUTEPRECONDDES_H
#include "SdpParser/CSdpFieldAttributePreCondDes.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEPRECONDCURR_H
#include "SdpParser/CSdpFieldAttributePreCondCurr.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEPRECONDCONF_H
#include "SdpParser/CSdpFieldAttributePreCondConf.h"
#endif
// BRCM_CUSTOM - [add] Precondition support
#endif //-- #ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

#ifndef MXG_CSDPFIELDATTRIBUTEICEPWD_H
#include "SdpParser/CSdpFieldAttributeIcePwd.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEICEUSERFRAG_H
#include "SdpParser/CSdpFieldAttributeIceUserFrag.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEICECANDIDATE_H
#include "SdpParser/CSdpFieldAttributeIceCandidate.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEICEREMOTECANDIDATES_H
#include "SdpParser/CSdpFieldAttributeIceRemoteCandidates.h"
#endif

#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)


//-- Parameters used by Value


//-- Interface Realized and/or Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif


MX_NAMESPACE_START(MXD_GNS)



//-- Forward Declarations
class CSdpLevelSession;
class CBlob;

//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
static const char* const gs_apszRTCP_RR = "RR:0";
static const char* const gs_apszRTCP_RS = "RS:0";

#if defined(MXD_SDP_ENABLE_MPTIME)
static const unsigned int gs_uMPTIME_HYPHEN = (unsigned int) (0);
#endif
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//==  Class: CSdpLevelMedia
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a media-descriptions part.
//
// Description:
//   This class is an abstraction of a media-descriptions part in a
//   SDP packet.  When the description for T.38 is used, this class is also
//   necessary to Set and Get T.38 information.  It follows the BNF that is
//   described in RFC 2327.
//
//  RFC 2327 BNF:
//   <PRE>
//      media-descriptions =  *( <Link CSdpFieldMediaAnnouncement, media-field>
//                               information-field
//                               <Link CSdpFieldConnectionData, *(connection-field)>
//                               bandwidth-fields
//                               key-field
//                               attribute-fields )
//      attribute-fields   =    *("a=" attribute CRLF)
//      attribute          =     <Link CSdpFieldAttributeRtpmap, attribute-rtpmap> /
//                               <Link CSdpFieldAttributeFmtp, attribute-fmtp> /
//                               <Link CSdpFieldAttributeKeyMgmt, attribute-key-mgmt> /
//                               <Link CSdpFieldAttributePtime, attribute-ptime> /
//                               <Link CSdpFieldAttributeFillBitRemoval, attribute-fill-bit-removal> /
//                               <Link CSdpFieldAttributeMaxBitRate, attribute-max-bit-rate> /
//                               <Link CSdpFieldAttributeMaxDatagram, attribute-max-datagram> /
//                               <Link CSdpFieldAttributeT38ErrorControl, attribute-t38-error-control> /
//                               <Link CSdpFieldAttributeT38FacsimileMaxBuffer, attribute-t38-facsimile-max-buffer> /
//                               <Link CSdpFieldAttributeT38FacsimileRateMgmnt, attribute-t38-Facsimile-rate-mgmnt> /
//                               <Link CSdpFieldAttributeTranscodingMMR, attribute-transcoding-mmr> /
//                               <Link CSdpFieldAttributeTranscodingJBIG, attribute-transcoding-jbig> /
//                               <Link CSdpFieldAttributeVersion, attribute-version> /
//                               <Link CSdpFieldAttributeOther, attribute-other>
//
//  </PRE>
//
//  Fields with no link are not implemented yet, they are ignored when they
//  are parsed.
//
// Location:
//   SdpParser/CSdpLevelMedia.h
//
// See Also:
//   CSdpFieldMediaAnnouncement, CSdpFieldConnectionData, CSdpFieldAttributeFmtp,
//   CSdpFieldAttributeKeyMgmt, CSdpFieldAttributeRtpmap, CSdpFieldAttributePtime,
//   CSdpFieldAttributeFillBitRemoval, CSdpFieldAttributeMaxBitRate,
//   CSdpFieldAttributeMaxDatagram, CSdpFieldAttributeT38ErrorControl,
//   CSdpFieldAttributeT38FacsimileMaxBuffer,
//   CSdpFieldAttributeT38FacsimileRateMgmnt,
//   CSdpFieldAttributeTranscodingMMR, CSdpFieldAttributeTranscodingJBIG,
//   CSdpFieldAttributeVersion, CSdpFieldAttributePreCondDes,
//   CSdpFieldAttributePreCondConf, CSdpFieldAttributePreCondCurr
//
//==============================================================================
class CSdpLevelMedia : public CSdpParser
{
//-- Friend Declarations


//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpLevelMedia();

    // Summary: Copy constructor.
    CSdpLevelMedia(IN const CSdpLevelMedia& rFrom);

    // Summary: Destructor.
    virtual ~CSdpLevelMedia();

    // Summary: Comparison operator.
    bool operator==(IN const CSdpLevelMedia& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpLevelMedia& operator=(IN const CSdpLevelMedia& rFrom);

    //-- << Get methods >>

    // Summary:
    //   Provides access to the media announcement stored in the session.
    CSdpFieldMediaAnnouncement& GetMediaAnnouncement();
    const CSdpFieldMediaAnnouncement& GetMediaAnnouncement() const;

    // Summary:
    //   Gets the number of connection data. Default value if no data.
    uint32_t GetNbConnectionDatas() const;

    // Summary:
    //   Gets the connection data at the specified index.
    CSdpFieldConnectionData& GetConnectionData(IN uint16_t uIndex);
    // <COMBINE CSdpLevelMedia::GetConnectionData>
    const CSdpFieldConnectionData& GetConnectionData(IN uint16_t uIndex) const;

    // Summary:
    //   Provides the exact number of connection data.
    uint32_t GetNbExplicitConnectionDatas() const;

    // Summary:
    //   Gets the connection data at the specified index.
    CSdpFieldConnectionData& GetExplicitConnectionData(IN uint16_t uIndex);
    // <COMBINE CSdpLevelMedia::GetExplicitConnectionData>
    const CSdpFieldConnectionData& GetExplicitConnectionData(IN uint16_t uIndex) const;

    // Summary:
    //   Adds a new association to an opaque for a payload type.
    void AddPayloadTypeToOpaqueAssociation(IN uint32_t uPayloadType,
                                           IN mxt_opaque opq);

    // Summary:
    //   Gets the association to an opaque for a payload type.
    bool GetPayloadTypeToOpaqueAssociation(IN uint32_t uPayloadType,
                                           OUT mxt_opaque& ropq) const;

    // Summary:
    //   Removes the association to an opaque for a payload type.
    void RemovePayloadTypeToOpaqueAssociation(IN uint32_t uPayloadType);

    // Summary:
    //   Gets the number of Fmtp in the media.
    uint32_t GetNbFmtps() const;

    // Summary:
    //   Gets the Fmtp attribute at the specified index.
    CSdpFieldAttributeFmtp& GetFmtp(IN uint16_t uIndex);
    const CSdpFieldAttributeFmtp& GetFmtp(IN uint16_t uIndex) const;

    // Summary:
    //   Gets the Fmtp attribute from the compression algorithm.
    CSdpFieldAttributeFmtp*
        GetFmtpFromEncoding(IN ERtpCompressionAlgorithm eEncoding);
    const CSdpFieldAttributeFmtp*
        GetFmtpFromEncoding(IN ERtpCompressionAlgorithm eEncoding) const;

    // Summary:
    //   Gets the Fmtp attribute from the compression algorithm name.
    CSdpFieldAttributeFmtp* GetFmtpFromEncoding(IN const char* pszEncodingName);
    const CSdpFieldAttributeFmtp*
        GetFmtpFromEncoding(IN const char* pszEncodingName) const;

    // Summary:
    //   Gets the Fmtp attribute from the payload type.
    CSdpFieldAttributeFmtp* GetFmtpFromPayloadType(IN uint32_t uPayloadType);
    const CSdpFieldAttributeFmtp* GetFmtpFromPayloadType(IN uint32_t uPayloadType) const;

    // Summary:
    //   Gets the Fmtp redundancy attribute.
    CSdpFmtpRedundancy* GetFmtpRedundancy();
    const CSdpFmtpRedundancy* GetFmtpRedundancy() const;

    // Summary:
    //   Gets the Fmtp telephone event attribute.
    CSdpFmtpTelEvent* GetFmtpTelEvent();
    const CSdpFmtpTelEvent* GetFmtpTelEvent() const;

// BRCM_CUSTOM - [add] Video support
#if defined(MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT)
    CSdpFmtpH263* GetFmtpH263();
    const CSdpFmtpH263* GetFmtpH263() const;

    CSdpFmtpH264* GetFmtpH264();
    const CSdpFmtpH264* GetFmtpH264() const;
#endif
// BRCM_CUSTOM - [end] Video support

    // Summary:
    //   Gets the number of key management attributes.
    uint32_t GetNbKeyMgmt() const;

    // Summary:
    //   Gets the number of key management parameters.
    uint32_t GetNbKeyMgmtParam() const;

    // Summary:
    //   Gets the key management at the specified index.
    CSdpFieldAttributeKeyMgmt& GetKeyMgmt(IN uint16_t uIndex);
    const CSdpFieldAttributeKeyMgmt& GetKeyMgmt(IN uint16_t uIndex) const;

    // Summary:
    //   Gets the key management parameter at the specified index.
    CSdpKeyManagementParameter& GetKeyMgmtParam(IN uint16_t uIndex);
    const CSdpKeyManagementParameter& GetKeyMgmtParam(IN uint16_t uIndex) const;

    // Summary:
    //   Gets the number of rtp maps.
    uint32_t GetNbRtpmaps() const;

    // Summary:
    //   Gets the rtp map at the specified index.
    CSdpFieldAttributeRtpmap& GetRtpmap(IN uint16_t uIndex);
    const CSdpFieldAttributeRtpmap& GetRtpmap(IN uint16_t uIndex) const;

    // Summary:
    //   Gets the last seen ptimes in the parsing process.
    CSdpFieldAttributePtime&       GetPtime();
    const CSdpFieldAttributePtime& GetPtime() const;

    // Summary:
    //   Returns the number of parsed ptimes.
    unsigned int GetNbParsedPtimes() const;

    // Summary:
    //   Gets the number of Crypto stored in the session.
    uint32_t GetNbCrypto() const;

    // Summary:
    //   Gets the Crypto stored at the specified index.
    CSdpFieldAttributeCrypto& GetCrypto(IN unsigned int uIndex);
    const CSdpFieldAttributeCrypto& GetCrypto(IN unsigned int uIndex) const;

    // Summary:
    //   Adds one crypto attribute in the media.
    void AddCrypto(IN const CSdpFieldAttributeCrypto& rCrypto);

    // Summary:
    //   Removes the crypto attribute in the media at the specified index.
    void RemoveCrypto(IN unsigned int uIndex);

    // Summary:
    //   Inserts the crypto attribute in the media at the specified index.
    void InsertCrypto(IN unsigned int                    uIndex,
                      IN const CSdpFieldAttributeCrypto& rCrypto);


#if defined( MXD_SDP_ENABLE_T38_SUPPORT )

    // Summary:
    //  Gets the fillbit removal field attribute.
    CSdpFieldAttributeFillBitRemoval& GetFillBitRemoval();
    const CSdpFieldAttributeFillBitRemoval& GetFillBitRemoval() const;

    // Summary:
    //  Gets the maximum bitrate field attribute.
    CSdpFieldAttributeMaxBitRate& GetMaxBitRate();
    const CSdpFieldAttributeMaxBitRate& GetMaxBitRate() const;

    // Summary:
    //  Gets the maximum datagram field attribute.
    CSdpFieldAttributeMaxDatagram& GetMaxDatagram();
    const CSdpFieldAttributeMaxDatagram& GetMaxDatagram() const;

    // Summary:
    //  Gets the T38 error control field attribute.
    CSdpFieldAttributeT38ErrorControl& GetT38ErrorControl();
    const CSdpFieldAttributeT38ErrorControl& GetT38ErrorControl() const;

    // Summary:
    //  Gets the T38 Facsimile maximum buffer field attribute.
    CSdpFieldAttributeT38FacsimileMaxBuffer& GetT38FacsimileMaxBuffer();
    const CSdpFieldAttributeT38FacsimileMaxBuffer& GetT38FacsimileMaxBuffer() const;

    // Summary:
    //  Gets the T38 Facsimile rate management field attribute.
    CSdpFieldAttributeT38FacsimileRateMgmnt& GetT38FacsimileRateMgmnt();
    const CSdpFieldAttributeT38FacsimileRateMgmnt& GetT38FacsimileRateMgmnt() const;

    // Summary:
    //  Gets the transcoding MMR field attribute.
    CSdpFieldAttributeTranscodingMMR& GetTranscodingMMR();
    const CSdpFieldAttributeTranscodingMMR& GetTranscodingMMR() const;

    // Summary:
    //  Gets the transcoding JBIG field attribute.
    CSdpFieldAttributeTranscodingJBIG& GetTranscodingJBIG();
    const CSdpFieldAttributeTranscodingJBIG& GetTranscodingJBIG() const;

    // Summary:
    //  Gets the version field attribute.
    CSdpFieldAttributeVersion& GetVersion();
    const CSdpFieldAttributeVersion& GetVersion() const;

#endif // MXD_SDP_ENABLE_T38_SUPPORT

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
    // Summary:
    //  Returns the ice-ufrag attribute.
    CSdpFieldAttributeIceUserFrag& GetIceUserFragment();
    const CSdpFieldAttributeIceUserFrag& GetIceUserFragment() const;

    // Summary:
    //  Returns the ice-pwd attribute.
    CSdpFieldAttributeIcePwd& GetIcePassword();
    const CSdpFieldAttributeIcePwd& GetIcePassword() const;

    // Summary:
    //  The attribute a=ice-mismatch is present.
    bool IsIceMismatch() const;

    // Summary:
    //   Returns a reference to the list of ICE candidate.
    CVector<CSdpFieldAttributeIceCandidate>& GetIceCandidate();
    const CVector<CSdpFieldAttributeIceCandidate>& GetIceCandidate() const;

    // Summary:
    //   Returns a reference to the ICE remote-candidates attribute.
    CSdpFieldAttributeIceRemoteCandidates& GetIceRemoteCandidatesAttribute();
    const CSdpFieldAttributeIceRemoteCandidates& GetIceRemoteCandidatesAttribute() const;

    // Summary:
    //   Returns true if the media contains at least one ICE attribute.
    bool IsIceAttributePresent() const;

    // Summary:
    //   Validates that the IP address used by the media is also an ICE candidate.
    bool ValidateIceCandidates() const;

    // Summary:
    //  Checks if RTCP is disabled for this media.
    bool IsRtcpDeactivated(OUT bool* pbRrPresent = NULL,
                           OUT bool* pbRsPresent = NULL) const;

// BRCM_CUSTOM - [mod] FindCandidate with component ID
    // Summary:
    //  Searches for a candidate with the corresponding address, port, or component ID.
    const CSdpFieldAttributeIceCandidate* FindCandidate(IN const char* pszAddress,
                                                        IN unsigned int uPort,
                                                        IN uint32_t uComponentId = 0xFFFFFFFF) const;                                                     
// BRCM_CUSTOM - [end] FindCandidate with component ID    

    // Summary:
    //  Checks that all candidates have the microliteport extension.
    bool IsMicroLitePortPresent() const;

#if defined(MXD_SDP_ENABLE_ICE_LITE_WITHOUT_CC_NEGOTIATION)
    // Summary:
    //  Specify which address family must be prioritized for the default 
    //  destination when sending an offer.
    void SetMicroLiteDefaultFamily(IN CSocketAddr::EAddressFamily eFamily);
#endif
#endif

    // Summary:
    //  Searches the payload type from the encoding name in the rtpmap
    //  attributes.
    uint32_t GetPayloadTypeFromEncoding(IN const char* pszEncodingName) const;

    // Summary:
    //  Searches the payload type from the encoding name in the rtpmap
    //  attributes.
    void GetPayloadTypeFromEncoding(IN const char* pszEncodingName,
                                    OUT CString& rstrPayloadType ) const;

    // Summary:
    //  Searches the encoding name from the payload type in the rtpmap
    //  attributes.
    void GetEncodingNameFromPayloadType(IN uint32_t uPayloadType,
                                        OUT CString& rstr) const;

    // Summary:
    //  Searches the encoding name from the payload type in the rtpmap
    //  attributes.
    void GetEncodingNameFromPayloadType(IN const CString& rstrPayloadType,
                                        OUT CString& rstr) const;

    // Summary:
    //  Finds the rtpmap index for the given encoding name.
    int FindRtpMapIndexByEncodingName(IN const char* pszEncodingName) const;

    // Summary:
    //   Verifies whether or not the direction or the session is set to send only.
    bool IsSendOnly() const;

    // Summary:
    //   Verifies whether or not the direction is set to send only.
    bool IsExplicitSendOnly() const;

    // Summary:
    //   Verifies whether or not the direction or the session is set to receive
    //   only.
    bool IsRecvOnly() const;

    // Summary:
    //   Verifies whether or not the direction is set to receive only.
    bool IsExplicitRecvOnly() const;

    // Summary:
    //   Verifies whether or not the direction or the session is set to send and
    //   receive.
    bool IsSendRecv() const;

    // Summary:
    //   Verifies whether or not the direction is set to send and receive.
    bool IsExplicitSendRecv() const;

    // Summary:
    //   Verifies whether or not the direction is set to inactive.
    bool IsInactive() const;

    // Summary:
    //   Verifies whether or not the direction is set to inactive or if the
    //   session is inactive.
    bool IsExplicitInactive() const;

    // Summary:
    //   Gets the direction stored in the media level or in the session.
    EAttributeType GetDirection() const;

    // Summary:
    //   Gets the direction stored in the media level.
    EAttributeType GetExplicitDirection() const;

    // Summary:
    //   Gets the number of unknown field attributes.
    uint32_t GetNbOtherAttributes() const;

    // Summary:
    //   Gets the unknown field attribute at the specified index.
    CSdpFieldAttributeOther& GetOtherAttribute(IN uint16_t uIndex);
    const CSdpFieldAttributeOther& GetOtherAttribute(IN uint16_t uIndex) const;

    // Summary:
    //   Gets the unknown field attributes.
    CVector<CSdpFieldAttributeOther>& GetOtherAttributes();

    // Summary:
    //   Generates the data blob from the data members.
    void Serialize(INOUT CBlob& rBlob) const;


    // Summary:
    //   Gets the Rtcp field attribute.
    const CSdpFieldAttributeRtcp& GetSdpFieldAttributeRtcp() const;

    // Summary:
    //  Returns if a rtcp-mux attribute is present.
    bool IsRtcpMuxPresent() const;

    //-- << Set methods >>

    // Summary:
    //   Sets the session level.
    void SetSession(IN CSdpLevelSession* pSession);

    // Summary:
    //   Gets the session lvel.
    CSdpLevelSession* GetSession();
    const CSdpLevelSession* GetSession() const;

    // Summary:
    //   Sets the media announcement field.
    void
    SetMediaAnnouncement(IN CSdpFieldMediaAnnouncement& rMediaAnnouncement);

    // Summary:
    //   Clears all connection data of the media level.
    void ClearExplicitConnectionDatas();

    // Summary:
    //   Adds a connection data at the media level.
    void AddConnectionData(IN const CSdpFieldConnectionData& rConnectionData);

    // Summary:
    //   Adds a rtp map.
    void AddRtpmap(IN const CSdpFieldAttributeRtpmap& rRtpmap);

    // Summary:
    //   Removes a rtp map at the specified index.
    void RemoveRtpmap(IN uint16_t uIndex);

    // Summary:
    //   Inserts a rtp map at the specified index.
    void InsertRtpmap(IN uint16_t                        uIndex,
                      IN const CSdpFieldAttributeRtpmap& rRtpmap);

    // Summary:
    //   Adds a fmtp.
    void AddFmtp(IN const CSdpFieldAttributeFmtp& rFmtp);

    // Summary:
    //   Removes a fmtp at the specified index.
    void RemoveFmtp(IN uint16_t uIndex);

    // Summary:
    //   Adds a key management at the media level.
    void AddKeyMgmt(
        IN const CSdpFieldAttributeKeyMgmt& rKeyMgmt,
        IN const CSdpFieldAttributeKeyMgmt::EKeyManagementAttributeRole eRole =
            CSdpFieldAttributeKeyMgmt::eBOTH);

    // Summary:
    //   Adds a key management paremeter in the media.
    void AddKeyMgmtParam(IN const CSdpKeyManagementParameter& rKeyMgmtParam);

    // Summary:
    //   Removes fmtp from encoding from the specified compression algorithm.
    bool RemoveFmtpFromEncoding(IN ERtpCompressionAlgorithm eEncoding);

    // Summary:
    //   Removes fmtp from encoding from the specified encoding name.
    bool RemoveFmtpFromEncoding(IN const char* pszEncodingName);

    // Summary:
    //   Removes fmtp from the specified payload type.
    bool RemoveFmtpFromPayloadType(IN uint32_t uPayloadType);

    // Summary:
    //   Removes fmtp redundancy from the media.
    bool RemoveFmtpRedundancy();

    // Summary:
    //   Removes fmtp telephone event from the media.
    bool RemoveFmtpTelEvent();

// BRCM_CUSTOM - [add] Video support
#if defined(MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT)
    bool RemoveFmtpH263();

    bool RemoveFmtpH264();
#endif
// BRCM_CUSTOM - [end] Video support

    // Summary:
    //   Removes the key management at the specified index.
    void RemoveKeyMgmt(IN uint16_t uIndex);

    // Summary:
    //   Removes the key management parameter at the specified index.
    void RemoveKeyMgmtParam(IN uint16_t uIndex);

    // Summary:
    //   Sets the ptime.
    void SetPtime(IN CSdpFieldAttributePtime& rPacketTime);

#if defined MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT
    // Summary:
    //  Gets the silence suppression attribute from this media.
    const CSdpFieldAttributeSilenceSupp& GetSilenceSuppressionSupport() const;

    // Summary:
    //  Gets the silence suppression attribute from this media.
    CSdpFieldAttributeSilenceSupp& GetSilenceSuppressionSupport();
#endif

#if defined( MXD_SDP_ENABLE_T38_SUPPORT )

    // Summary:
    //   Sets the fill bit removal field attribute.
    void
    SetFillBitRemoval(IN CSdpFieldAttributeFillBitRemoval& rFillBitRemoval);

    // Summary:
    //   Sets the maximum bit rate field attribute.
    void SetMaxBitRate(CSdpFieldAttributeMaxBitRate& rMaxBitRate);

    // Summary:
    //   Sets the maximum datagram field attribute.
    void SetMaxDatagram(CSdpFieldAttributeMaxDatagram& rMaxDatagram);

    // Summary:
    //   Sets the T38 error control field attribute.
    void
    SetT38ErrorControl(CSdpFieldAttributeT38ErrorControl& rT38ErrorControl);

    // Summary:
    //   Sets the T38 Facsimile maximum buffer field attribute.
    void
    SetT38FacsimileMaxBuffer(
                           CSdpFieldAttributeT38FacsimileMaxBuffer& rMaxBuffer);

    // Summary:
    //   Sets the T38 Facsimile rate management field attribute.
    void
    SetT38FacsimileRateMgmnt(
                           CSdpFieldAttributeT38FacsimileRateMgmnt& rRateMgmnt);

    // Summary:
    //   Sets the transcoding MMR field attribute.
    void SetTranscodingMMR(CSdpFieldAttributeTranscodingMMR& rTranscodingMMR);

    // Summary:
    //   Sets the transcoding JBIG field attribute.
    void
    SetTranscodingJBIG(CSdpFieldAttributeTranscodingJBIG& rTranscodingJBIG);

    // Summary:
    //   Sets the version field attribute.
    void SetVersion(CSdpFieldAttributeVersion& rVersion);

    // Summary:
    //  Sets the T.38 boolean encoding method for the T38FaxFillBitRemoval,
    //  T38FaxTranscodingMMR, and T38FaxTranscodingJBIG attributes.
    void SetT38BooleanEncoding(bool bT38ImplicitEncoding);
#endif // MXD_SDP_ENABLE_T38_SUPPORT


#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
    // Summary:
    //  Sets the a=ice-mismatch attribute.
    void SetIceMismatch(IN bool bIceMismatch);
#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

    // Summary:
    //   Sets the send only flag.
    void SetSendOnly(IN bool bSendOnly);

    // Summary:
    //   Sets the receive only flag.
    void SetRecvOnly(IN bool bRecvOnly);

    // Summary:
    //   Sets the send and receive flag.
    void SetSendRecv(IN bool bSendRecv);

    // Summary:
    //   Sets the inactive flag.
    void SetInactive(IN bool bInactive);

    // Summary:
    //   Sets the direction flag.
    void SetDirection(IN EAttributeType eDirection);

    // Summary:
    //   Sets the sending direction.
    void SetSendDirection(IN bool bSend);

    // Summary:
    //   Adds an unknown attribute in the media.
    void AddOtherAttribute(IN CSdpFieldAttributeOther& rOtherAttribute);

    // Summary:
    //   Sets the rtcp field attribute.
    void SetSdpFieldAttributeRtcp(IN CSdpFieldAttributeRtcp& rRtcpAttribute);

    // Summary:
    // Gets the information field, i.e. "i=" field.
    const CString& GetInformation() const;

    // Summary:
    // Sets the information field, i.e. "i=" field.
    void SetInformation(IN const char* szInformation);

    // Summary:
    // Gets the Encryption key field, i.e. "k=" field.
    const CString& GetEncryptionKey() const;

    // Summary:
    // Sets the Encryption key field, i.e. "k=" field.
    void SetEncryptionKey(IN const char* szEncryptionKey);

    // Summary:
    // Gets the vector of Bandwidth field, i.e. "b=" field.
    CVector<CString>& GetBandwidth();

    // Summary:
    // Gets the vector of Bandwidth field, i.e. "b=" field.
    const CVector<CString>& GetBandwidth() const;

    // Summary:
    //  Gets the mid attribute field, i.e. "a=mid:" field.
    CSdpFieldAttributeMid& GetMid();

    // Summary:
    //  Gets the mid attribute field, i.e. "a=mid:" field.
    const CSdpFieldAttributeMid& GetMid() const;

// BRCM_CUSTOM - [add] Precondition support
#ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support
    // Summary:
    //  Gets the vector of DES precondition. i.e. "a=des" field.
    CVector<CSdpFieldAttributePreCondDes>& GetDesPreCondVector();

    // Summary:
    //  Gets the vector of DES precondition. i.e. "a=des" field.
    const CVector<CSdpFieldAttributePreCondDes>& GetDesPreCondVector() const;

    // Summary:
    //  Gets the vector of CURR precondition. i.e. "a=curr" field.
    CVector<CSdpFieldAttributePreCondCurr>& GetCurrPreCondVector();

    // Summary:
    //  Gets the vector of CURR precondition. i.e. "a=curr" field.
    const CVector<CSdpFieldAttributePreCondCurr>& GetCurrPreCondVector() const;

    // Summary:
    //  Gets the vector of CONF precondition. i.e. "a=conf" field.
    CVector<CSdpFieldAttributePreCondConf>& GetConfPreCondVector();

    // Summary:
    //  Gets the vector of CONF precondition. i.e. "a=conf" field.
    const CVector<CSdpFieldAttributePreCondConf>& GetConfPreCondVector() const;
// BRCM_CUSTOM - [add] Precondition support
#endif //-- #ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support

    // Summary:
    //  Returns if a rtcp-mux attribute is present.
    void SetRtcpMux(IN bool bRtcpMux);

#if defined(MXD_SDP_ENABLE_MPTIME)
    // Summary:
    //  Gets the vector of mptime.
    CVector<unsigned int>& GetMptimeVector();

    // Summary:
    //  Gets the vector of mptime.
    const CVector<unsigned int>& GetMptimeVector() const;
#endif

    //-- << CSdpParser Interface >>

    // Summary:
    //   Parses all the needed information for this field.
    EParserResult Parse(INOUT  const char*& rpszStartPosition, OUT mxt_result& rres);

    // Summary:
    //   Parses all the needed information for the key management.
    EParserResult ParseKeyMgmt(IN  const char*& rpszStartPosition,
                               OUT mxt_result& rres);

    // Summary:
    //   Checks the validity of the parsed data.
    bool Validate();

    // Summary:
    //   Resets all the data members.
    void Reset();

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
#if defined(MXD_SDP_ENABLE_MPTIME)
    // Summary:
    //  Parse mptime attribute.
    CSdpParser::EParserResult ParseMptime(IN const char*& rpszStartPosition,
                                          OUT mxt_result& rres);

    // Summary:
    //  Serialize the mptime attribute.
    void SerializeMptime(INOUT CBlob& rBlob) const;
#endif

    // Summary:
    //  Second pass parsing, will use the specialized fmtp classes when
    //  appropriate.
    mxt_result ReParseFmtps();


    // Summary:
    //  Gets the encoding type represented by the payload type beginning the
    //  string. Returns eUNKNOWN_RTP_COMPRESSION_ALGORITHM if no rtpmap maps
    //  the payload to an encoding name or if the encoding name is unknown.
    ERtpCompressionAlgorithm
        GetNextEncoding(IN const char* pszStartPosition) const;

    // Summary:
    //  Gets the vector index of the fmtp attribute with the payload type
    //  mapping to the encoding name.
    uint32_t GetFmtpIndexFromEncoding(IN const char* pszEncodingName) const;

    // Summary:
    //  Gets the vector index of the fmtp attribute for the payload type.
    uint32_t GetFmtpIndexFromPayloadType(IN uint32_t uPayloadType) const;

    // Summary:
    //  Compares m_vecpFmtps against the passed vector.
    bool CompareFmtps(IN const CVector<CSdpFieldAttributeFmtp*>& rvecpFrom) const;

    // Summary:
    //  Copies the passed vector into m_vecpFmtps.
    void CopyFmtpsFrom(IN const CVector<CSdpFieldAttributeFmtp*>& rvecpFrom);

    // Summary:
    //  Copies the key management attributes and parameters.
    void CopyKeyMgmtsFrom(
                    IN const CVector<CSdpFieldAttributeKeyMgmt*>& rvecpFrom,
                    IN const CVector<CSdpKeyManagementParameter*>& rvecpFromParam);

    // Summary:
    //  Copies the key management attributes and parameters.
    void CopyKeyMgmtsFrom(
                    IN const CVector<CSdpFieldAttributeKeyMgmt*>& rvecpFrom,
                    IN const CVector<CSdpKeyManagementParameter*>& rvecpFromParam) const;


//-- Hidden Data Members
protected:

    CSdpLevelSession* m_pSession;

    CSdpFieldMediaAnnouncement m_mediaAnnouncement;

    CVector<CSdpFieldConnectionData> m_vecConnectionDatas;

    // Allows the association of a payload type in the media to an opaque that
    // is or was in the local capabilities. An association may be retrieved with
    // a call to GetPayloadTypeToOpaqueAssociation.
    CVector<CPair<uint32_t, mxt_opaque> > m_vecPayloadTypeToOpaqueAssociations;

    CVector<CSdpFieldAttributeRtpmap> m_vecRtpmaps;

    CVector<CSdpFieldAttributeFmtp*> m_vecpFmtps;

    CVector<CSdpFieldAttributeKeyMgmt*> m_vecpKeyMgmt;

    // Key management parameters for the key management attributes.
    CVector<CSdpKeyManagementParameter*> m_vecpKeyMgmtParam;

#if defined MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT
    // Silence suppression attribute.
    CSdpFieldAttributeSilenceSupp m_silenceSupp;
#endif

    CVector<CSdpFieldAttributeCrypto> m_vecCrypto;

    CSdpFieldAttributePtime m_packetTime;

#if defined( MXD_SDP_ENABLE_T38_SUPPORT )
    CSdpFieldAttributeFillBitRemoval m_fillBitRemoval;

    CSdpFieldAttributeMaxBitRate m_maxBitRate;

    CSdpFieldAttributeMaxDatagram m_maxDatagram;

    CSdpFieldAttributeT38ErrorControl m_t38ErrorControl;

    CSdpFieldAttributeT38FacsimileMaxBuffer m_t38FacsimileMaxBuffer;

    CSdpFieldAttributeT38FacsimileRateMgmnt m_t38FacsimileRateMgmnt;

    CSdpFieldAttributeTranscodingMMR m_transcodingMMR;

    CSdpFieldAttributeTranscodingJBIG m_transcodingJBIG;

    CSdpFieldAttributeVersion m_version;
#endif // MXD_SDP_ENABLE_T38_SUPPORT

    EAttributeType m_eDirection;

    bool m_bSendDirection;

    // Contains unsupported attribute field. i.e. begin with "a=".
    CVector<CSdpFieldAttributeOther> m_vecOtherAttributes;

    // Contains information field, i.e. "i=" field.
    CString                     m_strInformation;

    // Contains bandwidth fields, i.e. "b=" field.
    CVector <CString>           m_vecstrBandwidth;

    // Contains key field, i.e. "k=" field.
    CString                     m_strEncryptionKey;

    // Contains RTCP port field, i.e. "a=rtcp:" field.
    CSdpFieldAttributeRtcp m_rtcpAttribute;

    // Keeps count of the number of ptimes present, although we only keep the
    // last one.
    unsigned int m_uNbParsedPTimes;

    // Contains mid attribute field. i.e. begin with "a=mid:".
    CSdpFieldAttributeMid m_midAttribute;

// BRCM_CUSTOM - [add] Precondition support
#ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support
    // Contains precondition field DES. i.e. begin with "a=des:".
    CVector<CSdpFieldAttributePreCondDes> m_vecDesPreCond;

    // Contains precondition field CURR. i.e. begin with "a=curr:".
    CVector<CSdpFieldAttributePreCondCurr> m_vecCurrPreCond;

    // Contains precondition field CONF. i.e. begin with "a=conf:".
    CVector<CSdpFieldAttributePreCondConf> m_vecConfPreCond;
// BRCM_CUSTOM - [add] Precondition support
#endif //-- #ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support

    // Tells if the rtcp-mux attribute is present or not.
    bool m_bRtcpMux;

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
    // The ice-ufrag attribute.
    CSdpFieldAttributeIceUserFrag m_iceUserFragment;

    // The ice-pwd attribute.
    CSdpFieldAttributeIcePwd m_icePassword;

    // The ice-mismatch attribute.
    bool m_bIceMismatch;

    // The list of ICE candidate.
    CVector<CSdpFieldAttributeIceCandidate> m_vecIceCandidate;

    // The ICE remote-candidates attribute.
    CSdpFieldAttributeIceRemoteCandidates m_iceRemoteCandidates;

#endif

#if defined(MXD_SDP_ENABLE_MPTIME)
    CVector<unsigned int> m_vecuMptime;
#endif

//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetNbParsedPtimes const
//==
//==============================================================================
//
//  Returns:
//      Number of ptimes encountered while parsing.
//
//  Description:
//      Provides access to the number of parsed ptimes. This is only an
//      indication of how many ptimes were seen in the parsing process. Only the
//      last ptime found is kept and returned by GetPtime().
//
//  See Also:
//      GetPtime
//
//==============================================================================
inline unsigned int CSdpLevelMedia::GetNbParsedPtimes() const
{
    return m_uNbParsedPTimes;
}

//==============================================================================
//==
//==  GetMediaAnnouncement
//==
//==============================================================================
//
//  Returns:
//      Reference to the internal CSdpFieldMediaAnnouncement.
//
//  Description:
//      Provides access to the media announcement stored in the session.
//
//  See Also:
//      SetMediaAnnouncement
//
//==============================================================================
inline CSdpFieldMediaAnnouncement& CSdpLevelMedia::GetMediaAnnouncement()
{
    return m_mediaAnnouncement;
}

// <COMBINE CSdpLevelMedia::GetMediaAnnouncement>
inline
const CSdpFieldMediaAnnouncement& CSdpLevelMedia::GetMediaAnnouncement() const
{
    return m_mediaAnnouncement;
}

//==============================================================================
//==
//==  GetNbConnectionDatas const
//==
//==============================================================================
//
//  Returns:
//      The number of connection data.
//
//  Description:
//      Provides the number of connection data. In the case the media does not
//      have any "c=" field, the default value in the session is used.
//
//  See Also:
//      GetNbExplicitConnectionDatas
//
//==============================================================================
inline uint32_t CSdpLevelMedia::GetNbConnectionDatas() const
{
    int32_t nNb = m_vecConnectionDatas.GetSize();

    if (nNb == 0)
    {
        // In case the media doesn't have any "c=" field, we'll take the
        // default value in the session.
        nNb = 1;
    }

    return nNb;
}

//==============================================================================
//==
//==  GetNbExplicitConnectionDatas const
//==
//==============================================================================
//
//  Returns:
//      The number of connection data.
//
//  Description:
//      Provides the number of connection data. In the case the media does not
//      have any "c=" field, the value 0 is returned.
//
//  See Also:
//      GetNbConnectionDatas
//
//==============================================================================
inline uint32_t CSdpLevelMedia::GetNbExplicitConnectionDatas() const
{
    return m_vecConnectionDatas.GetSize();
}

//==============================================================================
//==
//==  GetNbRtpmaps const
//==
//==============================================================================
//
//  Returns:
//      The number of Rtp maps.
//
//  Description:
//      Provides the number of rtp maps stored in the session.
//
//  See Also:
//      GetNbConnectionDatas
//
//==============================================================================
inline uint32_t CSdpLevelMedia::GetNbRtpmaps() const
{
    return m_vecRtpmaps.GetSize();
}

//==============================================================================
//==
//==  GetNbCrypto const
//==
//==============================================================================
//
//  Returns:
//      The number of CSdpFieldAttributeCrypto stored.
//
//  Description:
//      Provides the number of Crypto stored in the session.
//
//  See Also:
//      GetNbConnectionDatas
//
//==============================================================================
inline uint32_t CSdpLevelMedia::GetNbCrypto() const
{
    return m_vecCrypto.GetSize();
}

//==============================================================================
//==
//==  GetCrypto
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Index of the element stored in the Crypto vector.
//
//  Returns:
//      Reference to the requested CSdpFieldAttributeCrypto.
//
//  Description:
//      Provides one Crypto stored in the session.
//
//==============================================================================
inline CSdpFieldAttributeCrypto& CSdpLevelMedia::GetCrypto(IN unsigned int uIndex)
{
    return m_vecCrypto[uIndex];
}

// <COMBINE CSdpLevelMedia::GetCrypto>
inline const CSdpFieldAttributeCrypto&
CSdpLevelMedia::GetCrypto(IN unsigned int uIndex) const
{
    return m_vecCrypto[uIndex];
}

//==============================================================================
//==
//==  AddCrypto
//==
//==============================================================================
//
//  Parameters:
//    rCrypto:
//      Reference to a CSdpFieldAttributeCrypto to store in the session.
//
//  Description:
//      Appends the provided Crypto in the session at the end of the
//      vector, then calls Validate.
//
//==============================================================================
inline
void CSdpLevelMedia::AddCrypto(IN const CSdpFieldAttributeCrypto& rCrypto)
{
    m_vecCrypto.Insert(m_vecCrypto.GetEndIndex(), 1, rCrypto);
    Validate();
}

//==============================================================================
//==
//==  InsertCrypto
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Position in the vector where to insert the new Crypto.
//    rCrypto:
//      Reference to a CSdpFieldAttributeCrypto to store in the session.
//
//  Description:
//      Inserts the provided Crypto in the session at the provided index in the
//      vector.
//
//==============================================================================
inline
void CSdpLevelMedia::InsertCrypto(IN unsigned int                    uIndex,
                                  IN const CSdpFieldAttributeCrypto& rCrypto)
{
    m_vecCrypto.Insert(uIndex, 1, rCrypto);
}

//==============================================================================
//==
//==  RemoveCrypto
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Position in the vector where to delete the new Crypto.
//
//  Description:
//      Erases the Crypto stored at the provided index.
//
//==============================================================================
inline void CSdpLevelMedia::RemoveCrypto(IN unsigned int uIndex)
{
    m_vecCrypto.Erase(uIndex);
}

//==============================================================================
//==
//==  GetRtpmap
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Index of the element stored in the RtpMaps vector.
//
//  Returns:
//      Reference to the requested CSdpFieldAttributeRtpmap.
//
//  Description:
//      Provides one RtpMap stored in the session.
//
//==============================================================================
inline CSdpFieldAttributeRtpmap& CSdpLevelMedia::GetRtpmap(IN uint16_t uIndex)
{
    return m_vecRtpmaps[uIndex];
}

// <COMBINE CSdpLevelMedia::GetRtpmap>
inline const CSdpFieldAttributeRtpmap& CSdpLevelMedia::GetRtpmap(IN uint16_t uIndex) const
{
    return m_vecRtpmaps[uIndex];
}

//==============================================================================
//==
//==  GetNbFmtps const
//==
//==============================================================================
//
//  Returns:
//    The number of Fmtps attributes in the media.
//
//  Description:
//      Gets the number of Fmtps attributes in the media.
//
//==============================================================================
inline uint32_t CSdpLevelMedia::GetNbFmtps() const
{
    return m_vecpFmtps.GetSize();
}

//==============================================================================
//==
//==  GetFmtp
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Index of the element stored in the Fmtps vector.
//
//  Returns:
//    Reference to the requested CSdpFieldAttributeFmtp.
//
//  Description:
//      Gets the Fmtps attributes in the media at the provided index.
//
//==============================================================================
inline CSdpFieldAttributeFmtp& CSdpLevelMedia::GetFmtp(IN uint16_t uIndex)
{
    MX_ASSERT(m_vecpFmtps[uIndex] != NULL);
    return *m_vecpFmtps[uIndex];
}

// <COMBINE CSdpLevelMedia::GetFmtp>
inline const CSdpFieldAttributeFmtp& CSdpLevelMedia::GetFmtp(IN uint16_t uIndex) const
{
    MX_ASSERT(m_vecpFmtps[uIndex] != NULL);
    return *m_vecpFmtps[uIndex];
}

//==============================================================================
//==
//==  GetFmtpFromEncoding
//==
//==============================================================================
//
//  Parameters:
//    eEncoding:
//      The compression algorithm used from ERtpCompressionAlgorithm enum.
//
//  Returns:
//      The first fmtp attribute found for payload type corresponding to the
//      encoding name.
//
//      NULL if the encoding name was not found in a rtpmap.
//
//      NULL if the encoding name was found in a rtpmap but a fmtp attribute was
//      not found with the corresponding payload type.
//
//  Description:
//      This method returns the first fmtp attribute with the same encoding
//      name.
//
//  See Also:
//      GetFmtpFromPayloadType
//
//==============================================================================
inline CSdpFieldAttributeFmtp*
    CSdpLevelMedia::GetFmtpFromEncoding(IN ERtpCompressionAlgorithm eEncoding)
{
    return
        GetFmtpFromEncoding( GetRtpCompressionAlgorithmStrFromId( eEncoding ) );
}

// <combine CSdpLevelMedia::GetFmtpFromEncoding@IN ERtpCompressionAlgorithm>
inline const CSdpFieldAttributeFmtp* CSdpLevelMedia::GetFmtpFromEncoding(
                                    IN ERtpCompressionAlgorithm eEncoding) const
{
    // We const_cast only to call the implemented method. The returned value
    // will be const anyway.
    return const_cast<CSdpLevelMedia*>(this)->GetFmtpFromEncoding(eEncoding);
}

//==============================================================================
//==
//==  GetFmtpRedundancy
//==
//==============================================================================
//
//  Returns:
//      Pointer to the internal GetFmtpRedundancy.
//
//  Description:
//      Provides access to the Fmtp redundancy stored in the session.
//
//  See Also:
//      GetFmtpFromEncoding
//
//==============================================================================
inline CSdpFmtpRedundancy* CSdpLevelMedia::GetFmtpRedundancy()
{
    return
        static_cast<CSdpFmtpRedundancy*>(GetFmtpFromEncoding(CSdpParser::eRED));
}

// <COMBINE CSdpLevelMedia::GetFmtpRedundancy>
inline const CSdpFmtpRedundancy* CSdpLevelMedia::GetFmtpRedundancy() const
{
    return static_cast<const CSdpFmtpRedundancy*>(
                                         GetFmtpFromEncoding(CSdpParser::eRED));
}

//==============================================================================
//==
//==  GetFmtpTelEvent
//==
//==============================================================================
//
//  Returns:
//      Pointer to the internal CSdpFmtpTelEvent.
//
//  Description:
//      Provides access to the Fmtp Telephone Event stored in the session.
//
//  See Also:
//      GetFmtpFromEncoding
//
//==============================================================================
inline CSdpFmtpTelEvent* CSdpLevelMedia::GetFmtpTelEvent()
{
    return static_cast<CSdpFmtpTelEvent*>(
                            GetFmtpFromEncoding(CSdpParser::eTELEPHONE_EVENT));
}

// <COMBINE CSdpLevelMedia::GetFmtpTelEvent>
inline const CSdpFmtpTelEvent* CSdpLevelMedia::GetFmtpTelEvent() const
{
    return static_cast<const CSdpFmtpTelEvent*>(
                            GetFmtpFromEncoding(CSdpParser::eTELEPHONE_EVENT));
}

// BRCM_CUSTOM - [add] Video support
#if defined(MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT)
inline CSdpFmtpH263* CSdpLevelMedia::GetFmtpH263()
{
    return static_cast<CSdpFmtpH263*>(
                            GetFmtpFromEncoding(CSdpParser::eH263));
}

inline const CSdpFmtpH263* CSdpLevelMedia::GetFmtpH263() const
{
    return static_cast<const CSdpFmtpH263*>(
                            GetFmtpFromEncoding(CSdpParser::eH263));
}

inline CSdpFmtpH264* CSdpLevelMedia::GetFmtpH264()
{
    return static_cast<CSdpFmtpH264*>(
                            GetFmtpFromEncoding(CSdpParser::eH264));
}

inline const CSdpFmtpH264* CSdpLevelMedia::GetFmtpH264() const
{
    return static_cast<const CSdpFmtpH264*>(
                            GetFmtpFromEncoding(CSdpParser::eH264));
}
#endif
// BRCM_CUSTOM - [end] Video support

//==============================================================================
//==
//==  GetNbKeyMgmt const
//==
//==============================================================================
//
//  Returns:
//    The number of key management attributes in the media.
//
//  Description:
//      Gets the number of key management attributes in the media.
//
//==============================================================================
inline uint32_t CSdpLevelMedia::GetNbKeyMgmt() const
{
    return m_vecpKeyMgmt.GetSize();
}

//==============================================================================
//==
//==  GetNbKeyMgmtParam const
//==
//==============================================================================
//
//  Returns:
//    The number of key management parameters in the media.
//
//  Description:
//      Gets the number of key management parameters in the media.
//
//==============================================================================
inline uint32_t CSdpLevelMedia::GetNbKeyMgmtParam() const
{
    return m_vecpKeyMgmtParam.GetSize();
}

//==============================================================================
//==
//==  AddKeyMgmt
//==
//==============================================================================
//
//  Parameters:
//    rKeyMgmt:
//      The key management attribute to add.
//
//    eRole:
//      The role that the key management attribute will use.
//
//  Description:
//      Adds a key management attribute to the media. Note that any role
//      previously set is overidden.
//
//==============================================================================
inline void CSdpLevelMedia::AddKeyMgmt(
           IN const CSdpFieldAttributeKeyMgmt& rKeyMgmt,
           IN const CSdpFieldAttributeKeyMgmt::EKeyManagementAttributeRole eRole)
{
    CSdpFieldAttributeKeyMgmt* pKeyMgmt = rKeyMgmt.GenerateCopy();
    pKeyMgmt->SetKeyManagementRole(eRole);

    m_vecpKeyMgmt.Insert(m_vecpKeyMgmt.GetEndIndex(),
                         1,
                         pKeyMgmt);
    Validate();
}

//==============================================================================
//==
//==  AddKeyMgmtParam
//==
//==============================================================================
//
//  Parameters:
//    rKeyMgmtParam:
//      The key management parameter.
//
//  Description:
//      Adds a key management parameter to the media.
//
//==============================================================================
inline void CSdpLevelMedia::AddKeyMgmtParam(IN const CSdpKeyManagementParameter& rKeyMgmtParam)
{
    m_vecpKeyMgmtParam.Insert(m_vecpKeyMgmtParam.GetEndIndex(),
                              1,
                              rKeyMgmtParam.GenerateCopy());
    Validate();
}

//==============================================================================
//==
//==  GetKeyMgmt
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Index of the key management attribute to get.
//
//  Returns:
//      The key management attribute.
//
//  Description:
//      Gets the key management attribute at the specified index.
//
//==============================================================================
inline CSdpFieldAttributeKeyMgmt& CSdpLevelMedia::GetKeyMgmt(IN uint16_t uIndex)
{
    return *(m_vecpKeyMgmt[uIndex]);
}

// <COMBINE CSdpLevelMedia::GetKeyMgmt>
inline const CSdpFieldAttributeKeyMgmt& CSdpLevelMedia::GetKeyMgmt(IN uint16_t uIndex) const
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
//      Index of the key management parameter to get.
//
//  Returns:
//      The key management parameter.
//
//  Description:
//      Gets the key management parameter for the specified index.
//
//==============================================================================
inline CSdpKeyManagementParameter& CSdpLevelMedia::GetKeyMgmtParam(IN uint16_t uIndex)
{
    return *(m_vecpKeyMgmtParam[uIndex]);
}

// <COMBINE CSdpLevelMedia::GetKeyMgmtParam>
inline const CSdpKeyManagementParameter& CSdpLevelMedia::GetKeyMgmtParam(IN uint16_t uIndex) const
{
    return *(m_vecpKeyMgmtParam[uIndex]);
}


// <combine CSdpLevelMedia::GetFmtpFromEncoding@IN const char*>
inline const CSdpFieldAttributeFmtp*
    CSdpLevelMedia::GetFmtpFromEncoding(IN const char* pszEncodingName) const
{
    // We const_cast only to call the implemented method. The returned value
    // will be const anyway.
    return
        const_cast<CSdpLevelMedia*>(this)->GetFmtpFromEncoding(pszEncodingName);
}

// <combine CSdpLevelMedia::GetFmtpFromPayloadType@IN uint32_t>
inline const CSdpFieldAttributeFmtp*
    CSdpLevelMedia::GetFmtpFromPayloadType(IN uint32_t uPayloadType) const
{
    // We const_cast only to call the implemented method. The returned value
    // will be const anyway.
    return
        const_cast<CSdpLevelMedia*>(this)->GetFmtpFromPayloadType(uPayloadType);
}

//==============================================================================
//==
//==  GetPtime
//==
//==============================================================================
//
//  Returns:
//      Last ptime found seen in the parsing process.
//
//  Description:
//      Provides access to the last seen ptimes in the parsing process.
//
//  See Also:
//      GetNbParsedPtimes
//
//==============================================================================
inline CSdpFieldAttributePtime& CSdpLevelMedia::GetPtime()
{
    return m_packetTime;
}

// <COMBINE CSdpLevelMedia::GetPtime>
inline const CSdpFieldAttributePtime& CSdpLevelMedia::GetPtime() const
{
    return m_packetTime;
}


#if defined( MXD_SDP_ENABLE_T38_SUPPORT )
//==============================================================================
//==
//==  GetFillBitRemoval
//==
//==============================================================================
//
//  Returns:
//      Reference to the fill bit removal field attribute.
//
//  Description:
//      Gets the fill bit removal field attribute.
//
//==============================================================================
inline CSdpFieldAttributeFillBitRemoval& CSdpLevelMedia::GetFillBitRemoval()
{
    return m_fillBitRemoval;
}

// <COMBINE CSdpLevelMedia::GetPtime>
inline const CSdpFieldAttributeFillBitRemoval&
CSdpLevelMedia::GetFillBitRemoval() const
{
    return m_fillBitRemoval;
}

//==============================================================================
//==
//==  GetMaxBitRate
//==
//==============================================================================
//
//  Returns:
//      Reference to the max bitrate field attribute.
//
//  Description:
//      Gets the max bitrate field attribute.
//
//==============================================================================
inline CSdpFieldAttributeMaxBitRate& CSdpLevelMedia::GetMaxBitRate()
{
    return m_maxBitRate;
}

// <COMBINE CSdpLevelMedia::GetMaxBitRate>
inline const CSdpFieldAttributeMaxBitRate& CSdpLevelMedia::GetMaxBitRate() const
{
    return m_maxBitRate;
}

//==============================================================================
//==
//==  GetMaxDatagram
//==
//==============================================================================
//
//  Returns:
//      Reference to the max datagram field attribute.
//
//  Description:
//      Gets the max datagram field attribute.
//
//==============================================================================
inline CSdpFieldAttributeMaxDatagram& CSdpLevelMedia::GetMaxDatagram()
{
    return m_maxDatagram;
}

// <COMBINE CSdpLevelMedia::GetMaxDatagram>
inline
const CSdpFieldAttributeMaxDatagram& CSdpLevelMedia::GetMaxDatagram() const
{
    return m_maxDatagram;
}

//==============================================================================
//==
//==  GetT38ErrorControl
//==
//==============================================================================
//
//  Returns:
//      Reference to the T38 error control field attribute.
//
//  Description:
//      Gets the T38 error control field attribute.
//
//==============================================================================
inline CSdpFieldAttributeT38ErrorControl& CSdpLevelMedia::GetT38ErrorControl()
{
    return m_t38ErrorControl;
}

// <COMBINE CSdpLevelMedia::GetT38ErrorControl>
inline const CSdpFieldAttributeT38ErrorControl&
CSdpLevelMedia::GetT38ErrorControl() const
{
    return m_t38ErrorControl;
}

//==============================================================================
//==
//==  GetT38FacsimileMaxBuffer
//==
//==============================================================================
//
//  Returns:
//      Reference to the T38 facsimile max buffer field attribute.
//
//  Description:
//      Gets the T38 facsimile max buffer field attribute.
//
//==============================================================================
inline CSdpFieldAttributeT38FacsimileMaxBuffer&
CSdpLevelMedia::GetT38FacsimileMaxBuffer()
{
    return m_t38FacsimileMaxBuffer;
}

// <COMBINE CSdpLevelMedia::GetT38FacsimileMaxBuffer>
inline const CSdpFieldAttributeT38FacsimileMaxBuffer&
CSdpLevelMedia::GetT38FacsimileMaxBuffer() const
{
    return m_t38FacsimileMaxBuffer;
}

//==============================================================================
//==
//==  GetT38FacsimileRateMgmnt
//==
//==============================================================================
//
//  Returns:
//      Reference to the T38 facsimile rate management field attribute.
//
//  Description:
//      Gets the T38 facsimile rate management field attribute.
//
//==============================================================================
inline CSdpFieldAttributeT38FacsimileRateMgmnt&
CSdpLevelMedia::GetT38FacsimileRateMgmnt()
{
    return m_t38FacsimileRateMgmnt;
}

// <COMBINE CSdpLevelMedia::GetT38FacsimileRateMgmnt>
inline const CSdpFieldAttributeT38FacsimileRateMgmnt&
CSdpLevelMedia::GetT38FacsimileRateMgmnt() const
{
    return m_t38FacsimileRateMgmnt;
}

//==============================================================================
//==
//==  GetTranscodingMMR
//==
//==============================================================================
//
//  Returns:
//      Reference to the transcoding MMR field attribute.
//
//  Description:
//      Gets the transcoding MMR field attribute.
//
//==============================================================================
inline CSdpFieldAttributeTranscodingMMR& CSdpLevelMedia::GetTranscodingMMR()
{
    return m_transcodingMMR;
}

// <COMBINE CSdpLevelMedia::GetTranscodingMMR>
inline const CSdpFieldAttributeTranscodingMMR&
CSdpLevelMedia::GetTranscodingMMR() const
{
    return m_transcodingMMR;
}

//==============================================================================
//==
//==  GetTranscodingJBIG
//==
//==============================================================================
//
//  Returns:
//      Reference to the transcoding JBIG field attribute.
//
//  Description:
//      Gets the transcoding JBIG field attribute.
//
//==============================================================================
inline CSdpFieldAttributeTranscodingJBIG& CSdpLevelMedia::GetTranscodingJBIG()
{
    return m_transcodingJBIG;
}

// <COMBINE CSdpLevelMedia::GetTranscodingJBIG>
inline const CSdpFieldAttributeTranscodingJBIG&
CSdpLevelMedia::GetTranscodingJBIG() const
{
    return m_transcodingJBIG;
}

//==============================================================================
//==
//==  GetVersion
//==
//==============================================================================
//
//  Returns:
//      Reference to the version field attribute.
//
//  Description:
//      Gets the version field attribute.
//
//==============================================================================
inline CSdpFieldAttributeVersion& CSdpLevelMedia::GetVersion()
{
    return m_version;
}

// <COMBINE CSdpLevelMedia::GetVersion>
inline const CSdpFieldAttributeVersion& CSdpLevelMedia::GetVersion() const
{
    return m_version;
}
#endif //--#if defined( MXD_SDP_ENABLE_T38_SUPPORT )

//==============================================================================
//==
//==  GetNbOtherAttributes
//==
//==============================================================================
//
//  Returns:
//      Number of other field attributes.
//
//  Description:
//      Gets the number of other field attributes.
//
//==============================================================================
inline uint32_t CSdpLevelMedia::GetNbOtherAttributes() const
{
    return m_vecOtherAttributes.GetSize();
}

//==============================================================================
//==
//==  GetOtherAttribute
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Index of the unknown attribute to get.
//
//  Returns:
//      Reference to the requested unknown attribute.
//
//  Description:
//      Gets the unknown attribute at the specified index.
//
//==============================================================================
inline
CSdpFieldAttributeOther& CSdpLevelMedia::GetOtherAttribute(IN uint16_t uIndex)
{
    return m_vecOtherAttributes[uIndex];
}

// <COMBINE CSdpLevelMedia::GetOtherAttribute>
inline const CSdpFieldAttributeOther&
CSdpLevelMedia::GetOtherAttribute(IN uint16_t uIndex) const
{
    return m_vecOtherAttributes[uIndex];
}

//==============================================================================
//==
//==  GetOtherAttributes
//==
//==============================================================================
//
//  Returns:
//      Reference to the unknown attribute vector.
//
//  Description:
//      Gets the unknown attribute vector.
//
//==============================================================================
inline CVector<CSdpFieldAttributeOther>& CSdpLevelMedia::GetOtherAttributes()
{
    return m_vecOtherAttributes;
}

//==============================================================================
//==
//==  GetSdpFieldAttributeRtcp
//==
//==============================================================================
//
//  Returns:
//      Reference to the rtcp attribute.
//
//  Description:
//      Gets the unknown rtcp attribute (i.e. the "a=rtcp:" field).
//
//==============================================================================
inline
const CSdpFieldAttributeRtcp& CSdpLevelMedia::GetSdpFieldAttributeRtcp() const
{
    return m_rtcpAttribute;
}

//==============================================================================
//==
//==  SetSession
//==
//==============================================================================
//
//  Parameters:
//    pSession:
//      Pointer to the CSdpLevelSession to set.
//
//  Description:
//      Sets the associated level session to the level media.
//
//==============================================================================
inline void CSdpLevelMedia::SetSession(IN CSdpLevelSession* pSession)
{
    MX_ASSERT (pSession != NULL);
    m_pSession = pSession;
}

//==============================================================================
//==
//==  GetSession
//==
//==============================================================================
//
//  Returns:
//      Pointer to a CSdpLevelSession.
//
//  Description:
//      Gets the level session associated with this level media.
//
//==============================================================================
inline CSdpLevelSession* CSdpLevelMedia::GetSession()
{
    return m_pSession;
}

// <COMBINE CSdpLevelMedia::GetSession>
inline const CSdpLevelSession* CSdpLevelMedia::GetSession() const
{
    return m_pSession;
}

//==============================================================================
//==
//==  SetMediaAnnouncement
//==
//==============================================================================
//
//  Parameters:
//      rMediaAnnouncement:
//          Reference to a CSdpFieldMediaAnnouncement.
//
//  Description:
//      Sets the MediaAnnouncement.
//
//==============================================================================
inline void CSdpLevelMedia::SetMediaAnnouncement(
                              IN CSdpFieldMediaAnnouncement& rMediaAnnouncement)
{
    m_mediaAnnouncement = rMediaAnnouncement;
}

//==============================================================================
//==
//==  ClearExplicitConnectionDatas
//==
//==============================================================================
//
//  Description:
//      Clears all the explicit connection data.
//
//==============================================================================
inline void CSdpLevelMedia::ClearExplicitConnectionDatas()
{
    m_vecConnectionDatas.EraseAll();
}

//==============================================================================
//==
//==  AddConnectionData
//==
//==============================================================================
//
//  Parameters:
//      rConnectionData:
//          Reference to a CSdpFieldConnectionData.
//
//  Description:
//      Adds the connection data and calls Validate.
//
//==============================================================================
inline void CSdpLevelMedia::AddConnectionData(
                              IN const CSdpFieldConnectionData& rConnectionData)
{
    m_vecConnectionDatas.Insert(m_vecConnectionDatas.GetEndIndex(),
                                1,
                                rConnectionData);
    Validate();
}

//==============================================================================
//==
//==  AddRtpmap
//==
//==============================================================================
//
//  Parameters:
//      rRtpmap:
//          Reference to a CSdpFieldAttributeRtpmap.
//
//  Description:
//      Adds a RtpMap to the list and calls Validate.
//
//==============================================================================
inline
void CSdpLevelMedia::AddRtpmap(IN const CSdpFieldAttributeRtpmap& rRtpmap)
{
    m_vecRtpmaps.Insert(m_vecRtpmaps.GetEndIndex(), 1, rRtpmap);
    Validate();
}

//==============================================================================
//==
//==  InsertRtpmap
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          Position index in the RtpMaps vector.
//      rRtpmap:
//          Reference to a CSdpFieldAttributeRtpmap.
//
//  Description:
//      Inserts the given CSdpFieldAttributeRtpmap at the given index uIndex in
//      the RtpMaps vector.
//
//==============================================================================
inline
void CSdpLevelMedia::InsertRtpmap(IN uint16_t                        uIndex,
                                  IN const CSdpFieldAttributeRtpmap& rRtpmap)
{
    m_vecRtpmaps.Insert(uIndex, 1, rRtpmap);
}

//==============================================================================
//==
//==  RemoveRtpmap
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          Position index in the RtpMaps vector.
//
//  Description:
//      Erases the element located at index uIndex from the RtpMaps vector then
//      calls Validate.
//
//==============================================================================
inline void CSdpLevelMedia::RemoveRtpmap(IN uint16_t uIndex)
{
    m_vecRtpmaps.Erase(uIndex);
    Validate();
}

//==============================================================================
//==
//==  RemoveFmtp
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          Position index in the Fmtps vector.
//
//  Description:
//      Erases the element located at index uIndex from the Fmtps vector.
//
//==============================================================================
inline void CSdpLevelMedia::RemoveFmtp(IN uint16_t uIndex)
{
    MX_ASSERT(m_vecpFmtps[uIndex] != NULL);
    MX_DELETE(m_vecpFmtps[uIndex]);
    m_vecpFmtps.Erase(uIndex);
}

//==============================================================================
//==
//==  RemoveFmtpFromEncoding
//==
//==============================================================================
//
//  Parameters:
//    eEncoding:
//      The encoding used in the rtpmap to map the payload type.
//
//  Returns:
//      true if a fmtp attribute for the encoding name was removed from the
//      list.
//
//      false if the encoding name was found in a rtpmap but a fmtp
//      attribute was not found with the corresponding payload type.
//
//  Description:
//      This method removes the first fmtp attribute using the same encoding
//      name.
//
//      If the encoding name to be searched is static, use
//      RemoveFmtpFromPayloadType instead since no rtpmap is necessary for these
//      encoding names.
//
//  See Also:
//      RemoveFmtpFromPayloadType
//
//==============================================================================
inline bool CSdpLevelMedia::RemoveFmtpFromEncoding(IN ERtpCompressionAlgorithm eEncoding)
{
    return RemoveFmtpFromEncoding(
                            GetRtpCompressionAlgorithmStrFromId( eEncoding ) );
}

//==============================================================================
//==
//==  RemoveFmtpRedundancy
//==
//==============================================================================
//
//  Returns:
//      true if a fmtp attribute eRED was removed from the list.
//
//      false if the encoding name was not found in a rtpmap.
//
//  Description:
//      This method removes FMTP Redundancy.
//
//  See Also:
//      RemoveFmtpFromPayloadType, RemoveFmtpRedundancy, RemoveFmtpTelEvent
//
//==============================================================================
inline bool CSdpLevelMedia::RemoveFmtpRedundancy()
{
    return RemoveFmtpFromEncoding(CSdpParser::eRED);
}

//==============================================================================
//==
//==  RemoveFmtpTelEvent
//==
//==============================================================================
//
//  Returns:
//      true if a fmtp attribute eTELEPHONE_EVENT was removed from the list.
//
//      false if the encoding name was not found in a rtpmap.
//
//  Description:
//      This method removes telephone events.
//
//  See Also:
//      RemoveFmtpFromPayloadType, RemoveFmtpRedundancy, RemoveFmtpTelEvent
//
//==============================================================================
inline bool CSdpLevelMedia::RemoveFmtpTelEvent()
{
    return RemoveFmtpFromEncoding(CSdpParser::eTELEPHONE_EVENT);
}

//==============================================================================
//==
//==  RemoveKeyMgmt
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//        Index of the key management to remove.
//
//  Description:
//      Removes the key management attribute at the specified index.
//
//==============================================================================
inline void CSdpLevelMedia::RemoveKeyMgmt(IN uint16_t uIndex)
{
    MX_ASSERT(uIndex < m_vecpKeyMgmt.GetSize());
    MX_DELETE(m_vecpKeyMgmt[uIndex]);
    m_vecpKeyMgmt.Erase(uIndex);
}

//==============================================================================
//==
//==  RemoveKeyMgmtParam
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          Index of the key management parameter to remove.
//
//  Description:
//      Removes the key management parameter at the specified index.
//
//==============================================================================
inline void CSdpLevelMedia::RemoveKeyMgmtParam(IN uint16_t uIndex)
{
    MX_ASSERT(uIndex < m_vecpKeyMgmtParam.GetSize());
    MX_DELETE(m_vecpKeyMgmtParam[uIndex]);
    m_vecpKeyMgmtParam.Erase(uIndex);
}

//==============================================================================
//==
//==  SetPtime
//==
//==============================================================================
//
//  Parameters:
//      rPacketTime:
//          Reference to the packet time attribute.
//
//  Description:
//      Sets the internal CSdpFieldAttributePtime.
//
//==============================================================================
inline void CSdpLevelMedia::SetPtime(IN CSdpFieldAttributePtime& rPacketTime)
{
    m_packetTime = rPacketTime;
}

#if defined( MXD_SDP_ENABLE_T38_SUPPORT )
//==============================================================================
//==
//==  SetFillBitRemoval
//==
//==============================================================================
//
//  Parameters:
//      rFillBitRemoval:
//          Reference to the fill bit removal attribute.
//
//  Description:
//      Sets the internal CSdpFieldAttributeFillBitRemoval.
//
//==============================================================================
inline void CSdpLevelMedia::SetFillBitRemoval(
                           IN CSdpFieldAttributeFillBitRemoval& rFillBitRemoval)
{
    m_fillBitRemoval = rFillBitRemoval;
}

//==============================================================================
//==
//==  SetMaxBitRate
//==
//==============================================================================
//
//  Parameters:
//      rMaxBitRate:
//          Reference to the maximum bit rate attribute.
//
//  Description:
//      Sets the internal CSdpFieldAttributeMaxBitRate.
//
//==============================================================================
inline
void CSdpLevelMedia::SetMaxBitRate(CSdpFieldAttributeMaxBitRate& rMaxBitRate)
{
    m_maxBitRate = rMaxBitRate;
}

//==============================================================================
//==
//==  SetMaxDatagram
//==
//==============================================================================
//
//  Parameters:
//      rMaxBitRate:
//          Reference to the maximum datagram attribute.
//
//  Description:
//      Sets the internal CSdpFieldAttributeMaxDatagram.
//
//==============================================================================
inline
void CSdpLevelMedia::SetMaxDatagram(CSdpFieldAttributeMaxDatagram& rMaxDatagram)
{
    m_maxDatagram = rMaxDatagram;
}

//==============================================================================
//==
//==  SetT38ErrorControl
//==
//==============================================================================
//
//  Parameters:
//      rT38ErrorControl:
//          Reference to the T38 error control attribute.
//
//  Description:
//      Sets the internal CSdpFieldAttributeT38ErrorControl.
//
//==============================================================================
inline void CSdpLevelMedia::SetT38ErrorControl(
                            CSdpFieldAttributeT38ErrorControl& rT38ErrorControl)
{
    m_t38ErrorControl = rT38ErrorControl;
}

//==============================================================================
//==
//==  SetT38FacsimileMaxBuffer
//==
//==============================================================================
//
//  Parameters:
//      rMaxBuffer:
//          Reference to the T38 facsimile maximum buffer attribute.
//
//  Description:
//      Sets the internal CSdpFieldAttributeT38FacsimileMaxBuffer.
//
//==============================================================================
inline void CSdpLevelMedia::SetT38FacsimileMaxBuffer(
                            CSdpFieldAttributeT38FacsimileMaxBuffer& rMaxBuffer)
{
    m_t38FacsimileMaxBuffer = rMaxBuffer;
}

//==============================================================================
//==
//==  SetT38FacsimileRateMgmnt
//==
//==============================================================================
//
//  Parameters:
//      rRateMgmnt:
//          Reference to the T38 facsimile rate management attribute.
//
//  Description:
//      Sets the internal CSdpFieldAttributeT38FacsimileRateMgmnt.
//
//==============================================================================
inline void CSdpLevelMedia::SetT38FacsimileRateMgmnt(
                            CSdpFieldAttributeT38FacsimileRateMgmnt& rRateMgmnt)
{
    m_t38FacsimileRateMgmnt = rRateMgmnt;
}

//==============================================================================
//==
//==  SetTranscodingMMR
//==
//==============================================================================
//
//  Parameters:
//      rTranscodingMMR:
//          Reference to the transcoding MMR attribute.
//
//  Description:
//      Sets the internal CSdpFieldAttributeTranscodingMMR.
//
//==============================================================================
inline void CSdpLevelMedia::SetTranscodingMMR(
                              CSdpFieldAttributeTranscodingMMR& rTranscodingMMR)
{
    m_transcodingMMR = rTranscodingMMR;
}

//==============================================================================
//==
//==  SetTranscodingJBIG
//==
//==============================================================================
//
//  Parameters:
//      rTranscodingJBIG:
//          Reference to the transcoding JBIG attribute.
//
//  Description:
//      Sets the internal CSdpFieldAttributeTranscodingJBIG.
//
//==============================================================================
inline void CSdpLevelMedia::SetTranscodingJBIG(
                            CSdpFieldAttributeTranscodingJBIG& rTranscodingJBIG)
{
    m_transcodingJBIG = rTranscodingJBIG;
}

//==============================================================================
//==
//==  SetVersion
//==
//==============================================================================
//
//  Parameters:
//      rVersion:
//          Reference to the version attribute.
//
//  Description:
//      Sets the internal CSdpFieldAttributeVersion.
//
//==============================================================================
inline void CSdpLevelMedia::SetVersion(CSdpFieldAttributeVersion& rVersion)
{
    m_version = rVersion;
}

//==============================================================================
//==
//==  SetT38BooleanEncoding
//==
//==============================================================================
//
//  Parameters:
//      bT38ImplicitEncoding:
//          Indicates if the encoding method is the implicit method (true)
//          or the explicit method (false).
//
//  Description:
//      Sets the T.38 boolean encoding method for the T38FaxFillBitRemoval,
//      T38FaxTranscodingMMR, and T38FaxTranscodingJBIG attributes.
//
//==============================================================================
inline void CSdpLevelMedia::SetT38BooleanEncoding(bool bT38ImplicitEncoding)
{
    m_fillBitRemoval.SetImplicitEncoding(bT38ImplicitEncoding);
    m_transcodingMMR.SetImplicitEncoding(bT38ImplicitEncoding);
    m_transcodingJBIG.SetImplicitEncoding(bT38ImplicitEncoding);
}
#endif //--#if defined( MXD_SDP_ENABLE_T38_SUPPORT )

//==============================================================================
//==
//==  SetSendOnly
//==
//==============================================================================
//
//  Parameters:
//      bSendOnly:
//          Indicates if the direction is send only (true) or not (false).
//
//  Description:
//      Sets the direction property to send only.
//
//==============================================================================
inline void CSdpLevelMedia::SetSendOnly(IN bool bSendOnly)
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
//          Indicates if the direction is receive only (true) or not (false).
//
//  Description:
//      Sets the direction property to receive only.
//
//==============================================================================
inline void CSdpLevelMedia::SetRecvOnly(IN bool bRecvOnly)
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
//==  SetRecvOnly
//==
//==============================================================================
//
//  Parameters:
//      bSendRecv:
//          Indicates that the direction is both send and receive (true) or not
//          (false).
//
//  Description:
//      Sets the direction property to send and receive.
//
//==============================================================================
inline void CSdpLevelMedia::SetSendRecv(IN bool bSendRecv)
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
//          Indicates that the direction is currently inactive.
//
//  Description:
//      Sets the direction property to inactive.
//
//==============================================================================
inline void CSdpLevelMedia::SetInactive(IN bool bInactive)
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
//          Indicates the direction to be set.
//
//  Description:
//      Sets the direction property according to the EAttributeType input.
//
//==============================================================================
inline void CSdpLevelMedia::SetDirection(IN EAttributeType eDirection)
{
    MX_ASSERT(eDirection == eUNKNOWN_ATTRIBUTE_TYPE ||
        eDirection == eINACTIVE || eDirection == eRECVONLY ||
        eDirection == eSENDONLY || eDirection == eSENDRECV);
    m_eDirection = eDirection;

    if (eDirection != eUNKNOWN_ATTRIBUTE_TYPE)
    {
        m_bSendDirection = true;
    }
}

//==============================================================================
//==
//==  SetSendDirection
//==
//==============================================================================
//
//  Parameters:
//      bSend:
//          Indicates the send direction.
//
//  Description:
//      Sets the send direction flags.
//
//==============================================================================
inline void CSdpLevelMedia::SetSendDirection(IN bool bSend)
{
    m_bSendDirection = bSend;
}

//==============================================================================
//==
//==  AddOtherAttribute
//==
//==============================================================================
//
//  Parameters:
//      rOtherAttribute:
//          unknown attribute to add.
//
//  Description:
//      Adds a new unknown attribute at the end of the vector.
//
//==============================================================================
inline void CSdpLevelMedia::AddOtherAttribute(IN CSdpFieldAttributeOther& rOtherAttribute)
{
    m_vecOtherAttributes.Insert(m_vecOtherAttributes.GetEndIndex(),
                                1,
                                rOtherAttribute);
    Validate();
}

//==============================================================================
//==
//==  SetSdpFieldAttributeRtcp
//==
//==============================================================================
//
//  Parameters:
//      rRtcpAttribute:
//          Sdp field attribute to set.
//
//  Description:
//      Sets a new Sdp field attribute.
//
//==============================================================================
inline void CSdpLevelMedia::SetSdpFieldAttributeRtcp(
                                      IN CSdpFieldAttributeRtcp& rRtcpAttribute)
{
    m_rtcpAttribute = rRtcpAttribute;
}


//==============================================================================
//==
//==  GetInformation const
//==
//==============================================================================
//
//  Returns:
//      Constant reference to the information string member.
//
//  Description:
//      Gets the information string member.
//
//==============================================================================
inline const CString& CSdpLevelMedia::GetInformation() const
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
//          Information to set.
//
//  Description:
//      Sets the information string member.
//
//==============================================================================
inline void CSdpLevelMedia::SetInformation(IN const char* szInformation)
{
    m_strInformation = szInformation;
}

//==============================================================================
//==
//==  GetEncryptionKey const
//==
//==============================================================================
//
//  Returns:
//      Constant reference to the encryption key string.
//
//  Description:
//      Gets the encryption key string.
//
//==============================================================================
inline const CString& CSdpLevelMedia::GetEncryptionKey() const
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
//          The encryption key string to set.
//
//  Description:
//      Sets the encryption key string.
//
//==============================================================================
inline void CSdpLevelMedia::SetEncryptionKey(IN const char* szEncryptionKey)
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
//      Reference to the vector of bandwidth fields.
//
//  Description:
//      Gets the vector of bandwidth fields (i.e. "b=" fields).
//
//==============================================================================
inline CVector<CString>& CSdpLevelMedia::GetBandwidth()
{
    return m_vecstrBandwidth;
}

// <COMBINE CSdpLevelMedia::GetBandwidth>
inline const CVector<CString>& CSdpLevelMedia::GetBandwidth() const
{
    return m_vecstrBandwidth;
}

#if defined MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT
//==============================================================================
//==
//==  GetSilenceSuppressionSupport
//==
//==============================================================================
//
//  Returns:
//      A const reference to the silence suppression attribute.
//
//  Description:
//      Returns the silence suppression attribute found in the media.
//      This attribute may be empty or invalid. It is up to the application to
//      validate the contents of the attribute.
//
//  Notes:
//      Use GetSilenceSupp returning a non const reference to set the value of
//      the CSdpFieldAttributeSilenceSupp in the CSdpLevelMedia.
//
//==============================================================================
inline
CSdpFieldAttributeSilenceSupp& CSdpLevelMedia::GetSilenceSuppressionSupport()
{
    return m_silenceSupp;
}

// <COMBINE CSdpLevelMedia::GetSilenceSuppressionSupport>
inline
const CSdpFieldAttributeSilenceSupp& CSdpLevelMedia::GetSilenceSuppressionSupport() const
{
    return m_silenceSupp;
}
#endif //-- #if defiend MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT

//==============================================================================
//==
//==  GetMid
//==
//==============================================================================
//
//  Returns:
//      The mid attribute field.
//
//  Description:
//      Returns the mid attribute field.
//
//==============================================================================
inline CSdpFieldAttributeMid& CSdpLevelMedia::GetMid()
{
    return m_midAttribute;
}

// <COMBINE CSdpLevelMedia::GetMid>
inline const CSdpFieldAttributeMid& CSdpLevelMedia::GetMid() const
{
    return m_midAttribute;
}

// BRCM_CUSTOM - [add] Precondition support
#ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support
//==============================================================================
//==
//==  GetDesPreCondVector
//==
//==============================================================================
//
//  Returns:
//      The DES precondition vector.
//
//  Description:
//      Returns the DES precondition vector.
//
//==============================================================================
inline CVector<CSdpFieldAttributePreCondDes>& CSdpLevelMedia::GetDesPreCondVector()
{
    return m_vecDesPreCond;
}

// <COMBINE CSdpLevelMedia::GetDesPreCondVector>
inline const CVector<CSdpFieldAttributePreCondDes>& CSdpLevelMedia::GetDesPreCondVector() const
{
    return m_vecDesPreCond;
}

//==============================================================================
//==
//==  GetCurrPreCondVector
//==
//==============================================================================
//
//  Returns:
//      The CURR precondition vector.
//
//  Description:
//      Returns the CURR precondition vector.
//
//==============================================================================
inline CVector<CSdpFieldAttributePreCondCurr>& CSdpLevelMedia::GetCurrPreCondVector()
{
    return m_vecCurrPreCond;
}

// <COMBINE CSdpLevelMedia::GetCurrPreCondVector>
inline const CVector<CSdpFieldAttributePreCondCurr>& CSdpLevelMedia::GetCurrPreCondVector() const
{
    return m_vecCurrPreCond;
}

//==============================================================================
//==
//==  GetConfPreCondVector
//==
//==============================================================================
//
//  Returns:
//      The CONF precondition vector.
//
//  Description:
//      Returns the CONF precondition vector.
//
//==============================================================================
inline CVector<CSdpFieldAttributePreCondConf>& CSdpLevelMedia::GetConfPreCondVector()
{
    return m_vecConfPreCond;
}

// <COMBINE CSdpLevelMedia::GetConfPreCondVector>
inline const CVector<CSdpFieldAttributePreCondConf>& CSdpLevelMedia::GetConfPreCondVector() const
{
    return m_vecConfPreCond;
}
// BRCM_CUSTOM - [add] Precondition support
#endif // MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
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
inline CSdpFieldAttributeIceUserFrag& CSdpLevelMedia::GetIceUserFragment()
{
    return m_iceUserFragment;
}

// <COMBINE CSdpLevelMedia::GetIceUserFragment>
inline const CSdpFieldAttributeIceUserFrag& CSdpLevelMedia::GetIceUserFragment() const
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
inline CSdpFieldAttributeIcePwd& CSdpLevelMedia::GetIcePassword()
{
    return m_icePassword;
}

// <COMBINE CSdpLevelMedia::GetIcePassword>
inline const CSdpFieldAttributeIcePwd& CSdpLevelMedia::GetIcePassword() const
{
    return m_icePassword;
}

//==============================================================================
//==
//==  IsIceMismatch
//==
//==============================================================================
//
//  Returns:
//      - true: If a a=ice-mismatch is present.
//      - false: Otherwise.
//
//  Description:
//      Returns true if a a=ice-mismatch is present in the media.
//
//==============================================================================
inline bool CSdpLevelMedia::IsIceMismatch() const
{
    return m_bIceMismatch;
}

//==============================================================================
//==
//==  SetIceMismatch
//==
//==============================================================================
//
//  Parameters:
//      bIceMismatch:
//       Whether or not the a=ice-mismatch is present in the media level.
//
//
//  Description:
//      Sets whether or not the a=ice-mismatch is present in the media level.
//
//==============================================================================
inline void CSdpLevelMedia::SetIceMismatch(IN bool bIceMismatch)
{
    m_bIceMismatch = bIceMismatch;
}

//==============================================================================
//==
//==  GetIceCandidate
//==
//==============================================================================
//
//  Returns:
//      A reference to the vector of ICE candidate.
//
//  Description:
//      Returns a reference to the vector of ICE candidate.
//
//  See Also:
//      ValidateIceCandidates
//
//==============================================================================
inline CVector<CSdpFieldAttributeIceCandidate>& CSdpLevelMedia::GetIceCandidate()
{
    return m_vecIceCandidate;
}

// <COMBINE CSdpLevelMedia::GetIceCandidate>
inline const CVector<CSdpFieldAttributeIceCandidate>& CSdpLevelMedia::GetIceCandidate() const
{
    return m_vecIceCandidate;
}

//==============================================================================
//==
//==  GetIceRemoteCandidatesAttribute
//==
//==============================================================================
//
//  Returns:
//      A reference to ICE remote-candidates attribute.
//
//  Description:
//      Returns a reference to the ICE remote-candidates attribute.
//
//==============================================================================
inline CSdpFieldAttributeIceRemoteCandidates& CSdpLevelMedia::GetIceRemoteCandidatesAttribute()
{
    return m_iceRemoteCandidates;
}

// <COMBINE CSdpLevelMedia::GetIceRemoteCandidatesAttribute>
inline const CSdpFieldAttributeIceRemoteCandidates& CSdpLevelMedia::GetIceRemoteCandidatesAttribute() const
{
    return m_iceRemoteCandidates;
}

//==============================================================================
//==
//==  IsIceAttributePresent
//==
//==============================================================================
//
//  Returns:
//      true if the media contains at least one ICE related attributes.
//
//  Description:
//      Returns true if the media contains at least one of the ICE attributes:
//      ice-ufrag, ice-pwd, candidate, remote-candidates or ice-mismatch.
//
//==============================================================================
inline bool CSdpLevelMedia::IsIceAttributePresent() const
{
    return m_iceUserFragment.IsValid() ||
           m_icePassword.IsValid() ||
           m_bIceMismatch ||
           m_vecIceCandidate.GetSize() > 0 ||
           m_iceRemoteCandidates.IsValid();
}
#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

//==============================================================================
//==
//==  SetRtcpMux
//==
//==============================================================================
//
//  Parameters:
//      bRtcpMux:
//       true if the rtcp-mux attribute is present.
//
//  Description:
//      Sets if a rtcp-mux attribute is present in the media or not.
//
//==============================================================================
inline void CSdpLevelMedia::SetRtcpMux(IN bool bRtcpMux)
{
    m_bRtcpMux = bRtcpMux;
}

//==============================================================================
//==
//==  IsRtcpMuxPresent
//==
//==============================================================================
//
//  Returns:
//      - true if rtcp-mux attribute is present.
//      - false if it is not present.
//
//  Description:
//      Returns if the rtcp-mux attribute is present or not.
//
//==============================================================================
inline bool CSdpLevelMedia::IsRtcpMuxPresent() const
{
    return m_bRtcpMux;
}

#if defined(MXD_SDP_ENABLE_MPTIME)
//==============================================================================
//==
//==  GetMptimeVector
//==
//==============================================================================
//
//  Returns:
//      Reference to the vector of mptime.
//
//  Description:
//    Gets the vector of the elements of the attribute mptime.
//
//==============================================================================
inline CVector<unsigned int>& CSdpLevelMedia::GetMptimeVector()
{
    return m_vecuMptime;
}


//==============================================================================
//==
//==  GetMptimeVector
//==
//==============================================================================
//
//  Returns:
//      Reference to the vector of mptime.
//
//  Description:
//    Gets the vector of the elements of the attribute mptime.
//
//==============================================================================
inline const CVector<unsigned int>& CSdpLevelMedia::GetMptimeVector() const
{
    return m_vecuMptime;
}
#endif
MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSDPLEVELMEDIA_H
