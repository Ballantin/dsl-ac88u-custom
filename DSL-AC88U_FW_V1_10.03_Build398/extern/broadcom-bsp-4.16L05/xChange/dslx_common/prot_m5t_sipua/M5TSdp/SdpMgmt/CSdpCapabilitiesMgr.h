//==============================================================================
//==============================================================================
//
//   Copyright(c) 2001 Mediatrix Telecom, Inc. ("Mediatrix")
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
#ifndef MXG_CSDPCAPABILITIESMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPCAPABILITIESMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h" // MXD_SDP_ENABLE_SRTP_SUPPORT,
                                 // MXD_SDP_ENABLE_REDUNDANCY_FMTP_ATTRIBUTE,
                                 // MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE
                                 // MXD_SDP_KEY_MANAGEMENT_NEGOTIATION_ENABLE_SUPPORT
                                 // MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE
                                 // MXD_SDP_MP4V_ES_FMTP_ATTRIBUTE_ENABLE_SUPPORT
                                 // MXD_SDP_H263_FMTP_ATTRIBUTE_ENABLE_SUPPORT
                                 // MXD_SDP_H264_FMTP_ATTRIBUTE_ENABLE_SUPPORT
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif


//-- Data Members
#ifndef MXG_CSDPFIELDATTRIBUTERTPMAP_H
#include "SdpParser/CSdpFieldAttributeRtpmap.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTECRYPTO_H
#include "SdpParser/CSdpFieldAttributeCrypto.h"
#endif

//-- Interface Realized and/or Parent
#if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)
#if defined (MXD_PKI_ENABLE_SUPPORT)
    #if !defined (MXD_CCERTIFICATECHAIN_H)
    #include "Pki/CCertificateChain.h"
    #endif //-- !defined (MXD_CCERTIFICATECHAIN_H)
#endif //-- #if defined (MXD_PKI_ENABLE_SUPPORT)
#endif //-- #if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)
//-- Inline Usage
#ifndef MXG_CSDPLEVELMEDIA_H
#include "SdpParser/CSdpLevelMedia.h"
#endif

#ifndef MXG_CSDPLEVELSESSION_H
#include "SdpParser/CSdpLevelSession.h"
#endif

#ifndef MXG_CSDPPACKET_H
#include "SdpParser/CSdpPacket.h"
#endif

#ifndef MXG_CSDPFIELDPHONE_H
#include "SdpParser/CSdpFieldPhone.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEGROUP_H
#include "SdpParser/CSdpFieldAttributeGroup.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEKEYMGMT_H
#include "SdpParser/CSdpFieldAttributeKeyMgmt.h"
#endif

//-- Template types
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
#if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)
class CSdpFieldAttributeKeyMgmtMikey;
class CSdpKeyManagementParameterMikey;
class IMikey;
class IMikeyCryptoSession;
class IMikeyCryptoSessionBundle;
class IMikeyIdentity;
class IMikeyKey;
class IMikeySecurityPolicy;

#endif
//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

const int nINVALID_INDEX = -1;

#if defined(MXD_SDP_ENABLE_ICE_LITE_WITHOUT_CC_NEGOTIATION)
static const char* const pszDEFAULT_MICROICELITE_UFRAG = "microlite";
static const char* const pszDEFAULT_MICROICELITE_PWD = "microlitemicrolitemicrolite";
#endif
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CSdpCapabilitiesMgr
//============================
//<GROUP TID_PACKAGE_SDPMGMT_CLASSES>
//
// Summary:
//   This class is used to find and manage the common capabilities between a
//   UAC and a UAS.
//
// Description:
//   The CSdpCapabilitiesMgr class is used to find and manage the common
//   capabilities between a UAC and a UAS. This negotiation process is described
//   in the "An Offer/Answer Model with SDP" Internet-Draft.
//
//   In this process, two UAs are involved in negotiating the media capabilities
//   through SDP. One of the entities is the offerer. The offerer is the UA that
//   is the first to offer a SDP packet to the other UA. The other entity is the
//   answerer. The answerer receives the offer and provides an answer to the
//   offerer. This answer has a matching media stream for each one in the offer,
//   indicating if the stream is accepted and a list of codecs to use for each
//   stream.
//
// #Initiating a Session#
//   When the application is initiating a session, it can choose to be the
//   offerer or the answerer. If it acts as the offerer, the application must
//   include an SDP packet into the INVITE it is sending to the remote UA. The
//   remote UA must then include the answer SDP packet into its 200 OK response.
//
//   If the application wants to act as the answerer, it must not include
//   any SDP packet into the INVITE. It is up to the remote UA to include
//   an offer in its 200 OK response. After receiving an offer into a 200 OK
//   response, the application must include its answer into the following ACK
//   request.
//
// #Remote UA Initiates a Session#
//   A remote User-Agent initiating a session has the same choice as described
//   in the previous section; it can choose to be the offerer or the answerer.
//   In either case, when the local application wants to accept the incoming
//   session, it MUST include an SDP packet into its 200 OK response. If the
//   initiating UA did not include an SDP packet in the INVITE, then the local
//   application acts as the offerer. If an SDP packet was included in the
//   INVITE, then the application acts as the answerer.
//
// #Acting as the Offerer#
//   When the application is acting as the offerer, it must include its
//   full capability set into the SDP packet it sends.
//
// #Acting as the Answerer#
//   The answerer must take the capabilities received from the offerer (the
//   offer) and match those with its own local capabilities. The union from both
//   of these capabilities yields the answer SDP packet. The main responsibility
//   of the CSdpCapabilitiesMgr class is to do just that: to generate a proper
//   answer according to the rules defined in SIP.
//
// #Session Modifications (re-INVITE)#
//   After a session is established, it is possible to send a re-INVITE to
//   modify a session and its associated capabilities. The same pattern as when
//   initiating a session is used, but the offerer takes the accepted answer as
//   the base of the new SDP packet to send.
//
// #Streams#
//   A stream in the context of the CSdpCapabilitiesMgr corresponds to an "m="
//   line of a session description. A session description can contain more than
//   one stream, as clearly defined in the SDP RFC 2327.
//
//   The SDP manager does the following to help strictly follow the SIP and
//   SDP RFCs:
//
//      -   It automatically aligns the media streams description.
//      -   An unsupported stream automatically has its port set to zero.
//      -   Only the codecs supported by both parties are part of the
//          resulting negotiation.
//      -   Rtpmap attributes are always added, helping for the support of
//          dynamic payload types.
//
// #VAD Support for G.723, G.729 and G.729E#
//   A stream sporting an rtpmap for G.723, G.729 or G.729E may also include an
//   a=fmtp attribute for negotiation of the VAD annex. The CSdpCapabilitiesMgr
//   can understand this fmtp attribute for both G.723 and G.729 and negotiate it
//   following local configuration and received offer. By default this is turned
//   off, see SetVadNegotiation() for more information.
//
// #fmtp attribute for telephone-event payload#
//   A stream including an rtpmap for "telephone-event" may also include an
//   a=fmtp attribute for negotiation of the supported events. When the fmtp
//   attribute is absent (but the rtpmap is present), the events 0 through 15
//   are considered supported (as per RFC 2833). Otherwise, the
//   CSdpCapabilitiesMgr can understand this fmtp attribute and send an answer
//   that enables only events supported by both parties. The telephone-event
//   support is turned off until a "telephone-event" rtpmap is added to a media
//   line in the local config. When the rtpmap is in a media line of the local
//   config, the local event support can be specified by adding a
//   "telephone-event" fmtp attribute to the media line. To be able to use
//   telephone event, you must also define
//   MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE in PreSdpParserCfg.h.
//
// #fmtp attribute for redundancy ("red") payload#
//   A stream including an rtpmap for "red" may also include an a=fmtp attribute
//   for negotiation of the supported payload types in redundancy. When the fmtp
//   attribute is absent (but the rtpmap is present), the payload types that are
//   in the media line are considered supported (as per RFC 2198). Otherwise, the
//   CSdpCapabilitiesMgr can understand this fmtp attribute and send an answer
//   that contains only payload types supported in redundancy by both parties.
//   The redundancy support is turned off until a "red" rtpmap is added to a
//   media line in the local config. When the rtpmap is in a media line of the
//   local config, the local redundancy payload type support can be specified by
//   adding a "red" fmtp attribute. To be able to use redundancy event, you must
//   also define MXD_SDP_ENABLE_REDUNDANCY_FMTP_ATTRIBUTE in PreSdpParserCfg.h.
//
// #How the media are matched when keymanagement is present#
//   An offer is received with mikey set at the media level.
//   The local capabilities manager first checks each media that it
//   supports. It tries to find a media that has the same codec and the
//   mikey attribute. If it does not find one, it rechecks every media but
//   it uses the mikey attribute that has been set at the session level
//   (if present) as if it was set in the media.
//
//   Note that when a mikey attribute is added to the local capabilities
//   manager, it is not mandatory to use mikey for that media. If an offer
//   is received that contains the same codec but without the mikey attribute,
//   it matches the local media even if mikey is locally set on that media.
//   The generated answer does not contain mikey though.
//
// Location:
//   SdpManagement/CSdpCapabilitiesMgr.h
//
// Example:
// #Generating an Offer#
//   In this example, the application is sending an offer. Keep in mind
//   that an offer can be sent either in an INVITE request or in a 200 OK to an
//   INVITE that did not contain any SDP in its payload.
//
//  <CODE>
//    CSdpCapabilitiesMgr localCapabilities;
//
//    // Adds a RTP audio stream with support for only one codec. The client
//    // should add all streams it is willing to support along with all the
//    // codecs for each stream. We also set the RTP published for this stream
//    unsigned int uStreamIdx;
//    localCapabilities.AddRtpAudioStream(unRtpPort, OUT unStreamIdx);
//
//    // The stream was added. Add the payloads that the application supports on
//    // this stream. In this case, we simply add PCMU support.
//    unsigned int uPayloadTypeIdx;
//    localCapabilities.AddPayloadType(unStreamIdx, CSdpParser::ePCMU,
//                                                  unPayloadTypeIdx);
//
//    // Create an SDP packet from the capabilities we have given to the SDP
//    // Caps manager
//    CSdpPacket resultPacket;
//    localCapabilities.CreateSdpPacket(szLocalAddr, szConnAddr, szVersion,
//                                                               resultPacket);
//    // Put the SDP Packet into a blob. The SDP packet will be ready to be
//    // sent.
//    CBlob* pContent = MX_NEW(CBlob);
//    *pContent << resultPacket;
//
//  </CODE>
//
// #Receiving an Answer#
//   After sending an offer, the application will eventually receive an answer.
//   The application can verify that the answer was properly generated following
//   the proper rules defined in the SIP specification. This is done by using
//   VerifyAnswer.
//
//  <CODE>
//    // we assume that the offer is contained in the SDP Caps manager
//    // instance "sdpOffer"
//
//    CSdpPacket receivedPacket;
//    const char* szReceivedSdpText =
//        reinterpret_cast<const char*>(pReceivedSdpBlob->GetFirstIndexPtr());
//    mxt_result res;
//    receivedPacket.Parse(szReceivedText, res);
//
//    CSdpCapabilitiesMgr sdpAnswer;
//
//    if (MX_RIS_S(res))
//    {
//        // If there was no parsing error, save the SDP packet into the new SDP
//        // caps manager instance.
//        sdpAnswer.CopyCapsFromPacket(receivedPacket);
//    }
//
//    // Check if the SDP in the answer is aligned with the offer.
//    if (sdpOffer.VerifyAnswer(remoteCapabilities))
//    {
//        // The response received is aligned with the capabilities sent in the
//        // INVITE so we can proceed with connection. We should now ACK the
//        // response and cache the localCapabilities.
//    }
//    else
//    {
//        // The remote party failed to align its capabilities to ours, adding
//        // or removing media streams (m= lines). Hence, we should ACK and BYE
//        // the call.
//    }
//
//  </CODE>
//
// #Generating an Answer#
//   In this example, the application is sending an answer. An answer can be
//   sent in a 200 OK response to an INVITE that contained an SDP payload or it
//   can be generated in an ACK request when the initial INVITE did not contain
//   any SDP.
//
//  <CODE>
//    CSdpCapabilitiesMgr localCapabilities;
//
//    // Adds a RTP audio stream with support for only one codec. The client
//    // should add all streams it is willing to support along with all the
//    // codecs for each stream.
//    unsigned int uStreamIdx;
//    localCapabilities.AddRtpAudioStream(unRtpPort, OUT unStreamIdx);
//
//    unsigned int uPayloadTypeIdx;
//    localCapabilities.AddPayloadType(unStreamIdx, CSdpParser::ePCMU,
//        unPayloadTypeIdx);
//
//    // Parse the offer and keep it in an SDP caps manager instance.
//    const char* szSdpText =
//        reinterpret_cast<const char*>(pReceivedContent->Begin());
//    CSdpPacket remoteSdpPacket;
//    mxt_result res;
//    remoteSdpPacket.Parse(szSdpText, res);
//
//    CSdpCapabilitiesMgr offerSdpCaps;
//
//    if (MX_RIS_S(res))
//    {
//        // If there was no parsing error, save the SDP packet into the new SDP
//        // caps manager instance.
//        offerSdpCaps.CopyCapsFromPacket(remoteSdpPacket);
//    }
//
//    // This SDP caps instance will hold the answer.
//    CSdpCapabilitiesMgr answer;
//    answer.GenerateAnswer(offerSdpCaps, localCapabilities);
//
//    unsigned int uSupportedStreamIdx;
//    if (answer.GetFirstSupportedStream(OUT unSupportedStreamIdx))
//    {
//        // There is compatible media stream so we can warn the user that a new
//        // call arrived and we should cache the answer. This answer can be
//        // re-used to generate a re-INVITE.
//    }
//    else
//    {
//        // Unable to generate a proper answer because there are no compatible
//        // streams.
//    }
//
//  </CODE>
//
// #Enabling G.729/G.723 VAD negotiation#
//   In this example, the application wants to configure the local capabilities
//   to negotiate VAD for G.729 and G.723. First it needs to enable the feature,
//   then it adds the fmtp attribute to the selected media line.
//
//  <CODE>
//    CSdpCapabilitiesMgr localCapabilities;
//
//    // Adds a RTP audio stream with support for only one codec. The client
//    // should add all streams it is willing to support along with all the
//    // codecs for each stream.
//    uint32_t unStreamIdx;
//    localCapabilities.AddRtpAudioStream(uRtpPort, OUT uStreamIdx);
//
//    // Add rtpmaps
//    uint32_t uPayloadTypeIdx;
//    localCapabilities.AddPayloadType(uStreamIdx, CSdpParser::eG729,
//        uPayloadTypeIdx);
//    localCapabilities.AddPayloadType(uStreamIdx, CSdpParser::eG723,
//        uPayloadTypeIdx);
//    localCapabilities.AddPayloadType(uStreamIdx, CSdpParser::eG729E,
//        uPayloadTypeIdx);
//
//    // Enable SdpCapsMgr to understand the fmtp attribute for VAD.
//    CSdpCapabilitiesMgr::SetVadNegotiation(true);
//
//    // Now that the feature is enabled, update the media line with explicit annex=yes
//    localCapabilities.SetStreamVadSupport(CSdpParser::eG729, uStreamIndex, true);
//    localCapabilities.SetStreamVadSupport(CSdpParser::eG723, uStreamIndex, true);
//
//    // Now that the feature is enabled, update the media line with explicit annex=no
//    localCapabilities.SetStreamVadSupport(CSdpParser::eG729E, uStreamIndex, false);
//  </CODE>
//
// #Finding out if G.729/G.723 VAD was negotiated during the SDP merge#
//   In this example, the application wants to know if the merged capabilities
//   include VAD support for G.729. First it needs to find the media line it
//   wants to use, then verify if VAD is enabled for that stream.
//
//  <CODE>
//    // ... assuming offer was received
//    CSdpCapabilitiesMgr answer;
//    answer.GenerateAnswer(offerSdpCaps, localCapabilities);
//
//    uint32_t uSupportedStreamIdx;
//    if (answer.GetFirstSupportedStream(OUT uSupportedStreamIdx))
//    {
//        // Assuming the first stream is an audio line that contains a G729 rtpmap
//        if(answer.IsVadSupportedInStream(CSdpParser::eG729, uSupportedStreamIdx))
//        {
//          // Enable VAD for G.729 in application.
//        }
//    }
//  </CODE>
//
// #Enabling telephone-event fmtp negotiation#
//   In this example, the application wants to configure the local capabilities
//   to negotiate the telephone-event fmtp attribute for events 0 through 15,
//   and 21. To be able to use telephone event, you must also define
//   MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE in PreSdpParserCfg.h.
//
//  <CODE>
//    CSdpCapabilitiesMgr localCapabilities;
//
//    // Add rtpmap
//    uint32_t uPayloadTypeIdx;
//    localCapabilities.AddPayloadType(uStreamIdx, CSdpParser::eTELEPHONE_EVENT,
//        uPayloadTypeIdx);
//
//    // The local configuration for the first stream
//    CSdpFmtpTelEvent localTelEventFmtp;
//
//    // Enable events 0 through 15 we can use the eDTMF group to do this
//    localTelEventFmtp.SetTelEventSupport(CSdpFmtpTelEvent::eDTMF, true);
//
//    // Enable event 21 directly
//    localTelEventFmtp.SetTelEventSupport(21, true);
//
//    localCapabilities.ReplaceFmtpTelEvent(IN uStreamIdx,
//                                          IN uPayloadTypeIdx,
//                                          INOUT localTelEventFmtp);
//
//    // Now SdpCapsManager will correctly negotiate telephone-events.
//  </CODE>
//
// #Finding out which telephone-events are supported in an offer or answer#
//   In this example, the application wants to know if the merged capabilities
//   (answer) support the telephone-event 15 in the first media line. To be
//   able to use telephone event, you must also define
//   MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE in PreSdpParserCfg.h.
//
//  <CODE>
//    // ... assuming offer was received
//    CSdpCapabilitiesMgr answer;
//    answer.GenerateAnswer(offerSdpCaps, localCapabilities);
//
//    // If there is no fmtp attribute, telephone-events are not supported.
//    if (answer.GetFmtpTelEvent(0) != NULL)
//    {
//        if (answer.GetFmtpTelEvent(0)->IsTelephoneEventSupported(15) == true)
//        {
//            // Enable event 15 in application.
//        }
//    }
//  </CODE>
//
// #Enabling T.38 media#
//  In this example, the application wants to send a T.38 media offer to the
//  remote side. To be able to do this, you must first define
//  MXD_SDP_ENABLE_T38_SUPPORT in PreSdpParserconfig.h.
//
//  <CODE>
//      CSdpCapabilitiesMgr localCaps;
//      unsigned int uStreamIndex = 0;
//
//      CSdpLevelMedia t38Media;
//      CSdpFieldMediaAnnouncement t38Announcement;
//      CSdpFieldAttributeMaxBitRate maxBitRate;
//      CSdpFieldAttributeVersion faxVersion;
//      CSdpFieldAttributeT38FacsimileRateMgmnt faxRateMgmnt;
//      CSdpFieldAttributeT38FacsimileMaxBuffer faxMaxBuffer;
//      CSdpFieldAttributeMaxDatagram maxDatagram;
//      CSdpFieldAttributeT38ErrorControl faxUdpErrorControl;
//
//      //Set up the capacities of the T.38 stream.
//      maxBitRate.SetMaxBitRate(14400);
//      faxVersion.SetVersion(0);
//      faxRateMgmnt.SetFacsimileRateMgmnt("transferredTCF");
//      faxMaxBuffer.SetMaxBuffer(72);
//      maxDatagram.SetMaxDatagram(316);
//      faxUdpErrorControl.SetErrorControl("t38UDPRedundancy");
//
//      // Configure our media type.
//      t38Announcement.SetMediaTypeId(CSdpParser::eIMAGE);
//      t38Announcement.SetTransportPort(5004);
//      t38Announcement.SetTransportProtocolId(CSdpParser::eUDPTL);
//      t38Announcement.AddMediaFormat("t38");
//      t38Announcement.Validate();
//
//      // Set the configured values in the media.
//      t38Media.SetMediaAnnouncement(t38Announcement);
//      t38Media.SetMaxBitRate(maxBitRate);
//      t38Media.SetMaxDatagram(maxDatagram);
//      t38Media.SetVersion(faxVersion);
//      t38Media.SetT38ErrorControl(faxUdpErrorControl);
//      t38Media.SetT38FacsimileRateMgmnt(faxRateMgmnt);
//      t38Media.SetT38FacsimileMaxBuffer(faxMaxBuffer);
//      t38Media.SetSession(localCaps.GetSdpSession());
//      t38Media.Validate();
//
//      localCaps.AddStream(t38Media, OUT uStreamIndex);
//  </CODE>
//
// #Generating an answer with ANAT grouping#
//  The ANAT group must be present in the local caps when generating an answer
//  from an ANAT-grouped offer.
//
//  <CODE>
//      CSdpFieldAttributeGroup localGroup;
//      localGroup.SetSemantic(CSdpParser::ms_apszGroupingMethodMap[CSdpParser::eANAT]);
//
//      CSdpCapabilitiesMgr localCaps;
//      localCaps.AddGroup(localGroup);
//  </CODE>
//
//==============================================================================
class CSdpCapabilitiesMgr
{
//-- Friend Declarations

    // Friend class used by CSdpCapabilitiesMgr
    friend class CSdpParserInitializer;


//-- Published Interface
public:

//-- Published enum

// From RFC3551.

//M5T_INTERNAL_USE_BEGIN
    // <FLAG Deprecated since 1.7.7>
    enum ECapsMgrPayloadType
    {
        eSDP_CAPS_MGR_PAYLOAD_TYPE_G723 = 4,
        eSDP_CAPS_MGR_PAYLOAD_TYPE_G729 = 18
    };

#if defined(MXD_SDP_ENABLE_T38_SUPPORT)
    enum ECapsMgrT38MediaState
    {
        // T38 media is disabled and will not be negotiated.
        eSDP_CAPS_MGR_T38_MEDIA_DISABLED,

        // T38 media is enabled, but will only be negotiated if alone in
        // answers and offers will only include supported T38 media.
        eSDP_CAPS_MGR_T38_MEDIA_ENABLED_ONLY_T38,

        // T38 media is enabled, and will be negotiated along every other
        // supported media.
        eSDP_CAPS_MGR_T38_MEDIA_ENABLED_WITH_OTHER_MEDIA
    };
#endif
//M5T_INTERNAL_USE_END

//-- Published Interface

    // Summary: Default constructor.
    CSdpCapabilitiesMgr();

    // Summary: Copy Constructor.
    CSdpCapabilitiesMgr(IN const CSdpCapabilitiesMgr& rSource);

    // Summary: Destructor.
    virtual ~CSdpCapabilitiesMgr();

    // Summary:
    //  Assignment operator.
    CSdpCapabilitiesMgr& operator=(IN const CSdpCapabilitiesMgr& rSource);

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    //  Generates an answer SDP from an offer and the local capabilities.
    void GenerateAnswer(IN const CSdpCapabilitiesMgr& rOffer,
                        IN const CSdpCapabilitiesMgr& rLocalCaps,
                        OUT mxt_result* pres = NULL);

    // Summary:
    //  Verifies that the received answer is coherent with the offer.
    bool VerifyAnswer(INOUT CSdpCapabilitiesMgr& rAnswer,
                      OUT mxt_result* pres = NULL);
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

    // Summary:
    //  Configures the manager with a stream the application is willing to
    //  support.
    void AddStream(IN  CSdpParser::EMediaType         eMediaType,
                   IN  CSdpParser::ETransportProtocol eProtocol,
                   IN  int nPort,
                   OUT unsigned int& ruStreamIndex);

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    // Configures the manager with a stream the application is willing to
    // support
    void AddStream(IN  const char* szMediaType,
                   IN  const char* szProtocol,
                   IN  int nPort,
                   OUT unsigned int& ruStreamIndex);
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

    // Summary:
    //  Configures the manager with a stream the application is willing to
    //  support.
    void AddStream(IN  const CSdpLevelMedia& rMedia,
                   OUT unsigned int& ruStreamIndex);

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    //  Configures the manager with an audio stream the application is
    //  willing to support.
    void AddRtpAudioStream(IN  int nPort,
                           OUT unsigned int& ruStreamIndex);
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

    // Summary:
    //  Retrieves the number of streams configured in the SDP session.
    unsigned int GetNbStreams() const;

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    //  Verifies if a specific stream is supported (non-zero port).
    bool IsStreamSupported(IN unsigned int uStreamIndex) const;

    // Summary:
    //  Verifies if a specific stream is supported (non-zero port).
    bool IsStreamSupported(IN const CSdpLevelMedia& rStream) const;
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

    // Summary:
    //  Retrieves the media type of a stream (audio, video, data, etc.).
    CSdpParser::EMediaType GetStreamType(IN unsigned int uStreamIndex) const;

    // Summary:
    //  Retrieves the type of transport used for a specific stream.
    CSdpParser::ETransportProtocol GetStreamTransportProtocol(IN unsigned int uStreamIndex) const;

    // Summary:
    //  Retrieves a stream.
    const CSdpLevelMedia& GetStream(IN unsigned int uStreamIndex) const;

    // Summary:
    //  Retrieves a stream.
    CSdpLevelMedia& GetStream(IN unsigned int uStreamIndex);

    // Summary:
    //  Retrieves the port number associated with a specific stream.
    int GetStreamPort(IN unsigned int uStreamIndex) const;

    // Summary:
    //  Retrieves the IP address associated with a stream.
    bool GetStreamAddr(IN unsigned int uStreamIndex,
                       OUT CString& rstrAddr) const;

    // Summary:
    //  Finds the first supported stream (non-zero port).
    bool GetFirstSupportedStream(OUT unsigned int& ruStreamIndex) const;

    // Summary:
    //  Sets the port number associated with a stream.
    void SetStreamPort(IN unsigned int uStreamIndex,
                       IN int nPort);

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    //  Disables the use of a stream.
    void DisableStream(IN unsigned int uStreamIndex);
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

    // Summary:
    //  Disables the use of a stream.
    void DisableStream(IN CSdpLevelMedia& rStream);

    // Payload types management

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    //  Adds a supported RTP payload type to a stream.
    void AddPayloadType(
                       IN  unsigned int uStreamIndex,
                       IN  CSdpParser::ERtpCompressionAlgorithm eRtpAlgorithm,
                       OUT unsigned int& ruPayloadIndex,
                       IN  int nPayloadTypeNumber = -1,
                       IN  bool bNoRtpmapOnStatic = false );
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

    // Summary:
    //  Adds a supported RTP payload type to a stream.
    void AddPayloadType(IN  unsigned int uStreamIndex,
                        IN  const CSdpFieldAttributeRtpmap& rRtpAlgorithm,
                        OUT unsigned int& ruPayloadIndex,
// BRCM_CUSTOM - [add] Add support for 'rtpmap' exclusion on static payload
                        IN  bool bNoRtpmapOnStatic = false );
// BRCM_CUSTOM - [end] Add support for 'rtpmap' exclusion on static payload

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    //  Adds a key management attribute and parameter to the media.
    void AddKeyMgmtAttribute(
        IN unsigned int uStreamIndex,
        IN CSdpFieldAttributeKeyMgmt& rKeyMgmt,
        IN const CSdpKeyManagementParameter& rKeyParam,
        IN const CSdpFieldAttributeKeyMgmt::EKeyManagementAttributeRole eRole = CSdpFieldAttributeKeyMgmt::eBOTH);

    // Summary:
    //  Adds a key management attribute and parameter to the session level.
    void AddKeyMgmtAttribute(
        IN CSdpFieldAttributeKeyMgmt& rKeyMgmt,
        IN const CSdpKeyManagementParameter& rKeyParam,
        IN const CSdpFieldAttributeKeyMgmt::EKeyManagementAttributeRole eRole = CSdpFieldAttributeKeyMgmt::eBOTH);
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

    // Summary:
    //  Adds a generic media format supported by the stream.
    void AddMediaFormat(IN  unsigned int uStreamIndex,
                        IN  const char* szFormat,
                        OUT unsigned int& ruFormatIndex);

    // Summary:
    //  Retrieves the number of payloads a stream has.
    unsigned int GetNbPayloadTypes(IN unsigned int uStreamIndex) const;

    // Summary:
    //  Retrieves a vector containing all the supported RTP payloads of a
    //  stream.
    void GetPayloadTypes(IN  unsigned int uStreamIndex,
                         OUT CVector<CSdpFieldAttributeRtpmap>& rvecAlgo) const;

    // Summary:
    //  Retrieves the payload found at a specific index of a stream.
    void GetPayloadType(IN  unsigned int uStreamIndex,
                        IN  unsigned int uPayloadIndex,
                        OUT CSdpFieldAttributeRtpmap& rRtpAlgo) const;

    // Summary:
    //  Removes the specified payload type from the specified stream.
    bool RemovePayloadType(IN uint32_t uStreamIndex,
                           IN uint32_t uPayloadTypeIndex);

    // Summary:
    //  Removes all payload types from the specified stream.
    bool RemoveAllPayloadTypes(IN uint32_t uStreamIndex);


    // Summary:
    //  Adds a supported crypto attribute to a stream.
    bool AddCryptoAttribute(IN unsigned int uStreamIndex,
                            IN const CSdpFieldAttributeCrypto& rCryptoAtt);

    // Summary:
    //  Retrieves a vector containing all the supported crypto
    //  RTP payloads of a stream.
    bool GetCryptoAttributes(
                       IN unsigned int uStreamIndex,
                       OUT CVector<CSdpFieldAttributeCrypto>& rvecCrypto) const;

    // Summary:
    //  Retrieves the payload found at a specific index of a stream.
    bool GetCryptoAttribute(IN  unsigned int uStreamIndex,
                            IN unsigned int uCryptoAttIndex,
                            OUT CSdpFieldAttributeCrypto& rCryptoAtt) const;

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    //  Sets the stream's ptime attribute. Set to nSDP_CAPS_MGR_INVALID_PTIME to
    //  disable use of ptime attribute.
    void SetStreamPtimeMs( IN uint32_t uStreamIndex, IN int32_t nPtime );

    // Summary:
    //  Returns the stream's ptime attribute. Value nSDP_CAPS_MGR_INVALID_PTIME
    //  means it is disabled.
    int32_t GetStreamPtimeMs( IN uint32_t uStreamIndex ) const;

    // Summary:
    //  Enables processing of fmtp attribute for G.729 annexb and G.723 annexa.
    static void SetVadNegotiation(IN bool bEnable);

    // Summary:
    //  Fetches current setting for processing of fmtp attribute for G.729
    //  annexb and G.723 annexa.
    static bool GetVadNegotiation();

    // Summary:
    //  Sets fmtp attribute for G.729 annexb and G.723 annexb.
    //  Deprecated since 1.7.7
    void SetStreamVadSupport(IN ECapsMgrPayloadType ePayloadType,
                             IN uint32_t uStreamIndex,
                             IN bool bEnable);

    // Summary:
    //  Sets fmtp attribute for G.729 annexb and G.723 annexb.
    void SetStreamVadAttribute(IN CSdpParser::ERtpCompressionAlgorithm eCompressionAlgorithm,
                               IN uint32_t uStreamIndex,
                               IN bool bEnable);

    // Summary:
    //  Gets fmtp setting for G.729 annexb and G.723 annexa using specified
    //  stream index.
    //  Deprecated since 1.7.7
    bool IsVadSupportedInStream(IN ECapsMgrPayloadType ePayloadType,
                                IN uint32_t uStreamIndex) const;

    // Summary:
    //  Gets fmtp setting for G.729 annexb and G.723 annexa using specified
    //  stream index.
    bool IsVadSupportedInStream(IN const CString& rstrEncodingName,
                                IN int nPayloadType,
                                IN uint32_t uStreamIndex ) const;

    // Summary:
    //  Gets fmtp setting for G.729 annexb and G.723 annexa in specified
    //  stream.
    //  Deprecated since 1.7.7
    bool IsVadSupportedInStream(IN ECapsMgrPayloadType ePayloadType,
                                IN const CSdpLevelMedia& rStream) const;

    // Summary:
    //  Gets fmtp setting for G.729 annexb and G.723 annexa in specified
    //  stream.
    bool IsVadSupportedInStream(IN const CString& rstrEncodingName,
                                IN int nPayloadType,
                                IN const CSdpLevelMedia& rStream) const;

    // Summary:
    //  Adds a G.729 or G.723 VAD CSdpFieldAttributeFmtp instance into the
    //  specified stream.
    //  Deprecated since 1.7.7
    void AddVadFmtp(IN ECapsMgrPayloadType ePayloadType,
                    IN bool bEnable,
                    INOUT CSdpLevelMedia& rStream) const;

    // Summary:
    //  Adds a G.729 or G.723 VAD CSdpFieldAttributeFmtp instance into the
    //  specified stream.
    void AddVadFmtp(IN const CString& rstrEncodingName,
                    IN int nPayloadType,
                    IN bool bEnable,
                    INOUT CSdpLevelMedia& rStream) const;

    // Summary:
    //  Removes all VAD fmtps from stream if found (for the specified payload
    //  type).
    void RemoveVadFmtp(IN const CString& rstrEncodingName,
                       IN int nPayloadType,
                       INOUT CSdpLevelMedia& rStream) const;
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

#if defined MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT
    // Summary:
    //  Enables processing of silence suppression attribute for PCMU and PCMA.
    static void SetSilenceSuppressionNegotiation(IN bool bEnable);

    // Summary:
    //  Fetches current setting for processing of silence suppression attribute
    //  for PCMU and PCMA.
    static bool GetSilenceSuppressionNegotiation();

    // Summary:
    //  Sets silence suppression attribute for PCMU and PCMA.
    void SetStreamSilenceSuppressionSupport( IN uint32_t uStreamIndex,
                                             IN bool bEnable );

    // Summary:
    // Checks if silence suppression is supported for the stream.
    bool IsSilenceSuppressionSupportedInStream( IN uint32_t uStreamIndex ) const;

    // Summary:
    // Checks if silence suppression is supported for the stream.
    bool IsSilenceSuppressionSupportedInStream( IN const CSdpLevelMedia& rStream ) const;
#endif //-- #if defined MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT

#ifdef MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE
    // Summary:
    //  Removes the telephone-event fmtp attribute from the selected stream.
    bool RemoveFmtpTelEvent( IN uint32_t uStreamIndex );

    // Summary:
    //  Replaces the fmtp attribute for telephone-event in the selected stream.
    bool ReplaceFmtpTelEvent( IN uint32_t uStreamIndex,
                              IN uint32_t uPayloadIndex,
                              INOUT CSdpFmtpTelEvent& rFmtpTelEvent );

    // Summary:
    //  Replaces the fmtp attribute for telephone-event in the selected stream.
    bool ReplaceFmtpTelEvent( IN uint32_t uStreamIndex,
                              IN const CSdpFmtpTelEvent& rFmtpTelEvent );

    // Summary:
    //  Gets the fmtp attribute used for telephone-event.
    CSdpFmtpTelEvent* GetFmtpTelEvent( IN uint32_t uStreamIndex );

    // Summary:
    //  Gets the fmtp attribute used for telephone-event.
    const CSdpFmtpTelEvent* GetFmtpTelEvent( IN uint32_t uStreamIndex ) const;
#endif //#ifdef MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE

#ifdef MXD_SDP_ENABLE_REDUNDANCY_FMTP_ATTRIBUTE
    // Summary:
    //  Removes the redundancy fmtp attribute from the selected stream.
    bool RemoveFmtpRedundancy( IN uint32_t uStreamIndex );

    // Summary:
    //  Replaces the fmtp attribute for redundancy in the selected stream.
    bool ReplaceFmtpRedundancy( IN uint32_t uStreamIndex,
                                IN uint32_t uPayloadIndex,
                                INOUT CSdpFmtpRedundancy& rFmtpRed );

    // Summary:
    //  Replaces the fmtp attribute for redundancy in the selected stream.
    bool ReplaceFmtpRedundancy(IN uint32_t uStreamIndex,
                               IN const CSdpFmtpRedundancy& rFmtpRed);

    // Summary:
    //  Gets the fmtp attribute used for redundancy.
    CSdpFmtpRedundancy* GetFmtpRedundancy( IN uint32_t uStreamIndex );

    // Summary:
    //  Gets the fmtp attribute used for redundancy.
    const CSdpFmtpRedundancy* GetFmtpRedundancy(
                                            IN uint32_t uStreamIndex ) const;
#endif // #ifdef MXD_SDP_ENABLE_REDUNDANCY_FMTP_ATTRIBUTE

#if defined(MXD_SDP_ENABLE_T38_SUPPORT)
    // Summary:
    //  Configures the T.38 enabled state.
    static void EnableT38(IN ECapsMgrT38MediaState eEnabledState);

    // Summary:
    //  Checks if T.38 is active. This should always be called on an answer
    //  generated locally or by the peer.
    bool IsT38Enabled();

    // Summary:
    //  Sets the T.38 boolean encoding method. Updates the T.38 boolean
    //  encoding method of the T.38 streams.
    //------------------------------------------------------------------
    void SetT38BooleanEncoding(bool bT38BooleanImplicitEncoding);

    // Summary:
    //  Indicates if the T.38 boolean encoding is set to implicit.
    //-------------------------------------------------------------
    bool IsT38BooleanImplicitEncoding();
#endif

    // Copy method

    // Summary:
    //  Transfers the capabilities represented in an SDP packet to the manager.
    void CopyCapsFromPacket(IN const CSdpPacket& rPacket);

    // Summary:
    //  Transfers the capabilities of the manager to an SDP packet.
    void CopyCapsToPacket(INOUT CSdpPacket& rPacket) const;


    // Other Methods

    // Summary:
    //  Creates an SDP packet from the capabilities contained in the manager.
    void CreateSdpPacket(IN  const char* szLocalIpAddress,
                         IN  const char* szConnectionIpAddress,
                         IN  const char* szVersion,
                         OUT CSdpPacket& rSdpPacket,
                         OUT mxt_result* pres = NULL);

    // Summary:
    //  Creates an SDP packet from the capabilities contained in the manager.
    void CreateSdpPacket(IN  const char* szLocalIpAddress,
                         IN  const char* szVersion,
                         OUT CSdpPacket& rSdpPacket);

    // Summary:
    //  Clears all capabilities contained in the manager.
    void Reset();

    // Summary:
    //  Returns current rtpmaps configuration.
    static unsigned int GetMaxAnswerRtpMaps();

    // Summary:
    //  Configures the maximum number of rtpmaps per media line to return in
    //  the answers.
    static void SetMaxAnswerRtpMaps(IN unsigned int uMaxRtpMaps);

    // Summary:
    //  Sets the usage of local payload type numbers in the generated answers.
    static void UseLocalPayloadTypesInAnswer(IN bool bUseLocalPayloadTypes);

    // Summary:
    //  Sets the priority to use for the payload types in the generated answers.
    static void UseLocalPayloadTypePriorityInAnswer(IN bool bUseLocalPayloadTypePriority);

    // Summary:
    //  Reorders payload types in local capabilities order.
    void ReorderInLocalPayloadTypePriorityOrder(IN unsigned int uResultStreamIdx,
                                                IN const CSdpCapabilitiesMgr& rLocalCaps);

    // Summary:
    //  Adds optional SDP phone-fields into the session.
    void AddPhone(IN const CSdpFieldPhone& rPhone);

    // Summary:
    //  Retrieves a phone field.
    const CSdpFieldPhone& GetPhone(IN unsigned int uPhoneIndex) const;

    // Summary:
    //  Retrieves the number of phone fields configured in the SDP session.
    unsigned int GetNbPhones() const;

    // Summary:
    //  Gets the pointer to the level session within the Caps manager.
    CSdpLevelSession* GetSdpSession();

    // Summary:
    //  Gets the pointer to the level session within the Caps manager.
    const CSdpLevelSession* GetSdpSession() const;

#if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)
    // Summary:
    //  Copies the MIKEY attributes from the target manager.
    void CopyMikeyAttributes(IN const CSdpCapabilitiesMgr& rSource);

    // Summary:
    //  Resets the MIKEY data.
    void ResetMikey();

    // Summary:
    //  Sets the IMikey interface used for this manager.
    mxt_result SetMikey(IN IMikey* pMikey);

    // Summary:
    //  Sets the keys to send with MIKEY.
    mxt_result SetMikeyKeys(CVector<IMikeyKey*>* pVecpKeys);

    // Summary:
    //  Sets the peer identity needed to handle a MIKEY message.
    mxt_result SetPeerIdentity(IN IMikeyIdentity* pPeerIdentity);

#if defined (MXD_PKI_ENABLE_SUPPORT)
    // Summary:
    //  Sets the peer's certificate chain needed to handle some MIKEY messages.
    mxt_result SetPeerCertificate(IN const CCertificateChain* pCertificateChain);
#endif
#endif //-- #if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)

    // Summary:
    //  Adds a group to the session.
    void AddGroup(IN const CSdpFieldAttributeGroup& rGroup);

#if defined(MXD_SDP_ENABLE_ICE_LITE_WITHOUT_CC_NEGOTIATION)
    // Summary:
    //  Sets the default destination address family
    void SetMicroLiteDefaultFamily(IN CSdpParser::EAddressType eAddressFamily);
#endif

    // Summary:
    //  Finds the rtpmap index for the given payload type in the given stream.
    int FindRtpmap(IN const CSdpLevelMedia& rStream, IN int nPayloadType) const;

    // Summary:
    //  Finds the rtpmap index for the given encoding name in the given stream.
    int FindRtpmap(IN const CSdpLevelMedia& rStream, IN const char* pszEncodingName) const;

    // Summary:
    //  Finds the RTP compression algo associated with a static paylaod type.
    static CSdpParser::ERtpCompressionAlgorithm FindAlgorithm(IN int nPayloadId);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
#ifdef MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE
    // Summary:
    //  Adds the appropriate telephone-event rtpmap and fmtp attribute when
    //  necessary.
    bool NegotiateFmtpTelEvent( IN const CSdpLevelMedia& rOfferStream,
                                IN const CSdpLevelMedia& rLocalStream,
                                IN const CSdpFieldAttributeRtpmap& rRtpMap,
                                IN unsigned int uResultStreamIdx );
#endif // #ifdef MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE

#ifdef MXD_SDP_ENABLE_REDUNDANCY_FMTP_ATTRIBUTE
    // Summary:
    //  Adds the appropriate redundancy fmtp attribute when necessary.
    void AddFmtpRedundancy( IN const CSdpLevelMedia& rOfferStream,
                            IN const CSdpLevelMedia& rLocalStream,
                            IN uint32_t uRedundancyRtpPayload,
                            INOUT CSdpLevelMedia& rResultStream ) const;

    // Summary:
    //  Fills the redundancy fmtp attribute with the payload types from the
    //  media line.
    void FillFmtpRedundancy( IN const CSdpFieldMediaAnnouncement& rMediaLine,
                             IN uint32_t uMediaFormatToIgnore,
                             INOUT CSdpFmtpRedundancy& rFmtpRedundancy ) const;
#endif // MXD_SDP_ENABLE_REDUNDANCY_FMTP_ATTRIBUTE

#if defined MXD_SDP_H264_FMTP_ATTRIBUTE_ENABLE_SUPPORT
    // Summary:
    //  Negotiates any H.264 FMTP attributes.
    void NegotiateFmtpH264( IN const CSdpLevelMedia& rOfferStream,
                            IN const CSdpLevelMedia& rLocalStream,
                            IN int nOfferRtpmapIdx,
                            IN int nLocalRtpmapIdx,
                            IN unsigned int uResultStreamIdx );
#endif

#if defined MXD_SDP_H263_FMTP_ATTRIBUTE_ENABLE_SUPPORT
    // Summary:
    //  Negotiates any H.263 FMTP attributes.
    void NegotiateFmtpH263( IN const CSdpLevelMedia& rOfferStream,
                            IN const CSdpLevelMedia& rLocalStream,
                            IN int nOfferRtpmapIdx,
                            IN int nLocalRtpmapIdx,
                            IN unsigned int uResultStreamIdx );
#endif

#ifdef MXD_SDP_MP4V_ES_FMTP_ATTRIBUTE_ENABLE_SUPPORT
    // Summary:
    //  Negotiates any MP4V-ES FMTP attributes.
    void NegotiateFmtpMp4Ves(IN const CSdpLevelMedia& rOfferStream,
                             IN const CSdpLevelMedia& rLocalStream,
                             IN int nOfferRtpmapIdx,
                             IN int nLocalRtpmapIdx,
                             IN unsigned int uResultStreamIdx);
#endif

#ifdef MXD_SDP_ENABLE_G7221_FMTP_ATTRIBUTE
    // Summary:
    //  Negotiates any G.722.1 FMTP attributes.
    void NegotiateFmtpG7221(IN const CSdpLevelMedia& rOfferStream,
                            IN const CSdpLevelMedia& rLocalStream,
                            IN int nOfferRtpmapIdx,
                            IN int nLocalRtpmapIdx,
                            IN unsigned int uResultStreamIdx);
#endif // #ifdef MXD_SDP_ENABLE_G7221_FMTP_ATTRIBUTE
#ifdef MXD_SDP_ENABLE_AMR_FMTP_ATTRIBUTE
    // Summary:
    //  Negotiates any AMR FMTP attributes.
    void NegotiateFmtpAMR(IN const CSdpLevelMedia& rOfferStream,
                          IN const CSdpLevelMedia& rLocalStream,
                          IN int nOfferRtpmapIdx,
                          IN int nLocalRtpmapIdx,
                          IN unsigned int uResultStreamIdx,
                          IN CSdpParser::ERtpCompressionAlgorithm eAlgorithm);
#endif // #ifdef MXD_SDP_ENABLE_AMR_FMTP_ATTRIBUTE

#if defined(MXD_SDP_ENABLE_ISAC_FMTP_ATTRIBUTE)
    // Summary:
    //  Negotiates any ISAC FMTP attributes.
    void NegotiateFmtpIsac(IN const CSdpLevelMedia& rOfferStream,
                           IN const CSdpLevelMedia& rLocalStream,
                           IN int nOfferRtpmapIdx,
                           IN int nLocalRtpmapIdx,
                           IN unsigned int uResultStreamIdx);
#endif

#if defined(MXD_SDP_ENABLE_ILBC_FMTP_ATTRIBUTE)
    // Summary:
    //  Adds any ILBC FMTP attributes from the local caps.
    void NegotiateFmtpIlbc(IN const CSdpLevelMedia& rOfferStream,
                           IN const CSdpLevelMedia& rLocalStream,
                           IN int nOfferRtpmapIdx,
                           IN int nLocalRtpmapIdx,
                           IN unsigned int uResultStreamIdx);
#endif

    virtual void Copy(IN const CSdpCapabilitiesMgr& rSource);

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    //  This function verifies the compatibility of the given media stream with
    //  the given local caps manager and adds the appropriate response to this
    //  caps manager.
    mxt_result AddCompatibilityAnswerStream(IN const CSdpLevelMedia&      rInitiatorStream,
                                            IN const CSdpCapabilitiesMgr& rLocalCaps);

    // Summary:
    //  Adds any attributes that are compatible from the offered sessions
    //  to this capabilities manager.
    virtual void AddCompatibilitySessionAttributes(
                                   IN const CSdpLevelSession* pOfferSession,
                                   IN const CSdpLevelSession* pLocalSession);

    // Summary:
    //  This method handles the negotiation of any RTP/AVP payloads for the
    //  media.
    virtual bool NegotiateRtpAvpMedia(
                                    IN const CSdpLevelMedia&      rOfferStream,
                                    IN const CSdpCapabilitiesMgr& rLocalCaps,
                                    IN unsigned int uResultStreamIdx,
                                    OUT unsigned int& ruLocalStreamIndex);

#if defined(MXD_SDP_ENABLE_T38_SUPPORT)
    // Summary:
    //  This method handles the negotiation of any T.38 attributes for the
    //  media.
    virtual bool NegotiateT38Media(
                                    IN const CSdpLevelMedia&      rOfferStream,
                                    IN const CSdpCapabilitiesMgr& rLocalCaps,
                                    IN unsigned int uResultStreamIdx,
                                    OUT unsigned int& ruLocalStreamIndex);
#endif

    // Summary:
    //   This method returns the negotiated stream direction, with regards to
    //   the offer and the local attribute.
    virtual CSdpParser::EAttributeType GetAnswerDirectionAttribute(IN CSdpParser::EAttributeType eOffer,
                                                                   IN CSdpParser::EAttributeType eLocal);

    // Summary:
    //  Sets the Stream direction attribute for the media.
    virtual void SetAnswerStreamDirection(
                                    IN const CSdpLevelMedia&      rOfferStream,
                                    IN const CSdpLevelMedia&      rLocalStream,
                                    IN unsigned int uResultStreamIdx);

    // Summary:
    //  Verifies if the two streams are compatible.
    virtual bool VerifyStreamCompatibility(
                                   IN const CSdpLevelMedia& rLocal,
                                   IN CSdpLevelMedia& rMixedRemote,
                                   IN bool bRemoveMediaFormat);

    // Summary:
    //  Returns true if the stream contains T38 fax over udptl transport protocol.
    static bool IsT38Fax(IN const CSdpFieldMediaAnnouncement& rMediaAnn);

    // Summary:
    //  Finds the payload type in this object that is compatible with the one
    //  specified by the media stream and the payload index.
    virtual bool FindCompatiblePayload(IN const CSdpLevelMedia& rOfferStream,
                                       IN unsigned int uPayloadIndex,
                                       IN CList<unsigned int>& rlstMatchedLocalRtpmap,
                                       OUT unsigned int& ruLocalStreamIndex,
                                       OUT int& rnLocalRtpmapIndex,
                                       OUT int& rnOfferRtpmapIndex,
                                       OUT int& rnLocalCryptoIndex,
                                       OUT int& rnOfferCryptoIndex,
                                       OUT int& rnLocalKeyMgmtIndex,
                                       OUT int& rnOfferKeyMgmtIndex,
                                       OUT bool& rbKeyMgmtIndexFoundAtSessionLevel,
                                       OUT int& rnLocalKeyMgmtParamIndex,
                                       OUT int& rnOfferKeyMgmtParamIndex,
                                       OUT bool& rbKeyMgmtParamIndexFoundAtSessionLevel) const;

    // Summary:
    //  Verifies if the rtpmap in the stream matches the rtpmap in parameter.
    virtual bool IsCompatibleRtpmap(IN const CSdpLevelMedia& rLocalStream,
                                    IN unsigned int uLocalRtpmapIndex,
                                    IN const CSdpFieldAttributeRtpmap& rOfferRtpmap,
                                    IN const CSdpLevelMedia& rOfferStream) const;

    // Summary:
    //  Verifies if the direction in the stream is supported for the encoding.
    virtual bool IsDirectionSupported(IN const CSdpLevelMedia& rOfferStream,
                                      IN const CSdpLevelMedia& rLocalStream,
                                      IN const CSdpFieldAttributeRtpmap& rRtpmap) const;

    // Summary:
    //  Returns true if a stream as already been selected for the group the
    //  stream belong to.
    bool IsAStreamAlreadySelectedForGroup(IN const CSdpLevelMedia& rOfferStream);
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

private:
    //-- << Startup mechanism >>

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    //  Removes all VAD fmtps from stream if found (for the specified payload
    //  type).
    //  Deprecated since 1.7.7
    void RemoveVadFmtp(IN ECapsMgrPayloadType ePayloadType,
                       INOUT CSdpLevelMedia& rStream) const;
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

    // Summary:
    //  Generates the default RtpMap
    virtual
    void GenerateDefaultRtpMap( IN CSdpParser::ERtpCompressionAlgorithm eCodec,
                                OUT CSdpFieldAttributeRtpmap& rRtpMap ) const;

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
    // Summary:
    //  Give the name of the payload.
    //  Deprecated since 1.7.7
    const char* GetPayloadName(IN ECapsMgrPayloadType ePayloadType) const;

    // Summary:
    // Give the number of the payload.
    // <FLAG Deprecated since 1.7.7>
    const char* GetPayloadNumber(IN ECapsMgrPayloadType ePayloadType) const;

    // Summary:
    //  Verifies if the CSdpFieldAttributeFmtp parameter contains the
    //  expected specialized fmtp type. If yes, the _Type parameter is set to
    //  point at the specialized fmtp. If no, the _Type parameter is set to
    //  NULL.
    template<class _Type>
    void GetSpecializedFmtpType(IN CSdpFieldAttributeFmtp::EFmtpType eExpectedFmtpType,
                                IN const CSdpFieldAttributeFmtp* pFmtp,
                                OUT _Type*& rpSpecializedFmtp);
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif
// BRCM_CUSTOM - [end] Custom SDP generator support

#if defined(MXD_SDP_ENABLE_T38_SUPPORT)
    // Summary:
    //  Handles the T.38 media in an answer or offer.
    void HandleT38Media(IN CSdpLevelSession& rSession) const;
#endif

#if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)

    // Summary:
    //  Creates a MIKEY message for each MIKEY key management attribute.
    mxt_result CreateMikeyKeyMgmt(IN CSdpLevelSession& rSession);

    CSdpParser::ERtpStreamDirection GetDirection(IN unsigned int uDirection);

    // Summary:
    //  Parses each MIKEY message for each MIKEY key management attribute.
    mxt_result ParseMikeyKeyMgmt(IN const CSdpLevelSession& rRemoteSession);

    // Summary:
    //  Initializes the MIKEY data in the MIKEY key management attribute.
    mxt_result InitializeMikeyDataToKeyMgmt(INOUT CSdpFieldAttributeKeyMgmtMikey* pMgmtMikey,
                                            OUT IMikeyCryptoSessionBundle** ppBundle);

    // Summary:
    //  Creates the necessary crypto sessions for each RTP stream.
    mxt_result CreateCryptoSession(IN bool bResponse,
                                   IN unsigned int uStream,
                                   IN CSdpParser::ERtpStreamDirection eDirection,
                                   IN IMikeyCryptoSessionBundle* pCsb,
                                   IN CSdpKeyManagementParameterMikey* pMikeyParam,
                                   IN IMikeySecurityPolicy* pPolicy) const;

    // Summary:
    //  Sets the data taken from a MIKEY message into the MIKEY key management
    //  parameter objects.
    mxt_result SetCryptoSessionData(IN unsigned int uKeyMgmtIndex,
                                    IN CSdpLevelMedia* pMedia,
                                    IN CVector<IMikeyCryptoSession*>* pvecpCryptoSessions,
                                    IN unsigned int uCurrentCryptoSessionHandled);

    // Summary:
    //  Parse the necessary information in order to be able to get the
    //  crypto session bundle ID.
    mxt_result ParseCryptoSessionBundleId(
                             IN CSdpFieldAttributeKeyMgmtMikey& rAttributeMikey);
#endif

    // Summary:
    //  Finds the matching key management attributes in a session.
    void FindSessionKeyMgmtMatch(IN const CSdpLevelSession* pLocalSession,
                                 IN const CSdpLevelSession* pOfferSession,
                                 OUT int& rnLocalKeyMgmtIdx,
                                 OUT int& rnOfferKeyMgmtIdx,
                                 OUT int& rnLocalKeyMgmtParamIdx,
                                 OUT int& rnOfferKeyMgmtParamIdx) const;

    // Summary:
    //  Converts EAttributeType to index.
    int DirectionAttributeTypeToIndex(IN CSdpParser::EAttributeType eDirection) const;

#if defined(MXD_SDP_KEY_MANAGEMENT_NEGOTIATION_ENABLE_SUPPORT)
    // Summary:
    //  Updates the Key Management role as an offer is being generated.
    void UpdateKeyMgmtRole();

    // Summary:
    //  Returns the number of key management that were effectively offered in
    //  the Session level.
    unsigned int GetNbOfferedKeyMgmt(IN const CSdpLevelSession* pSession);

    // Summary:
    //  Returns the number of key management that were effectively offered in
    //  the Media level.
    unsigned int GetNbOfferedKeyMgmt(IN const CSdpLevelMedia* pMedia);
#endif //#if defined(MXD_SDP_KEY_MANAGEMENT_NEGOTIATION_ENABLE_SUPPORT)

    // Summary:
    //  Verify if the key management present in each stream match.
#if defined (MXD_SDP_KEY_MANAGEMENT_NEGOTIATION_ENABLE_SUPPORT)
    mxt_result VerifyKeyMgmt(IN CSdpLevelMedia& rLocalMedia,
                             IN CSdpLevelMedia& rAnswerMedia,
                             INOUT bool& rbKeyManagementHandled);
#endif //#if defined (MXD_SDP_KEY_MANAGEMENT_NEGOTIATION_ENABLE_SUPPORT)

    void FindAlgorithm(IN int nPayloadId,
                       OUT CSdpParser::ERtpCompressionAlgorithm& rCodec,
                       OUT mxt_result* pres = NULL);

#ifdef MXD_SDP_ICE_ENABLE_SUPPORT
    // Summary:
    //  Validates that the media streams have compatible IP version.
    bool VerifyIpVersionInIceCandidate(IN CSdpLevelMedia& rLocal,
                                       IN CSdpLevelMedia& rAnswer,
                                       IN uint8_t uComponentId);
#endif

#if defined(MXD_SDP_ENABLE_ICE_LITE_WITHOUT_CC_NEGOTIATION)
    // Summary:
    //  Configures the default destination using configured ICE candidate.
    void ConfigureDefaultDestination(IN CSdpLevelSession& rSession);

    // Summary:
    //  Adds ice-lite attribute.
    void AddIceLiteAttribute(IN CSdpLevelSession& rSession);

    // Summary:
    //  Adds ice-ufrag attribute.
    void AddIceUfragAttribute(IN CSdpLevelSession& rSession);

    // Summary:
    //  Adds ice-pwd attribute.
    void AddIcePwdAttribute(IN CSdpLevelSession& rSession);

    // Summary:
    //  Gets the address of the default candidate to use as the default address.
    mxt_result GetDefaultIceCandidateAddress(IN CSdpLevelMedia& rMedia,
                                             IN uint32_t uComponentId,
                                             INOUT CSocketAddr& rAddress);

    // Summary:
    //  Gets the highest priority candidate.
    int FindHighestPriorityCandidate(IN CSdpParser::EAddressType eFamily,
                                     IN uint32_t uComponentId,
                                     IN const CVector<CSdpFieldAttributeIceCandidate>& rvecCandidates);

    // Summary:
    //  Gets the candidate list sorted by priority.
    void GetCandidates(IN const CSdpLevelMedia& rLocalStream,
                       IN uint32_t uComponentId,
                       OUT CVector<const CSdpFieldAttributeIceCandidate*>& vecpLocalCandidate);

    // Summary:
    //  Adds the local candidate that match the candidates of the offer into the answer stream.
    mxt_result NegotiateIceCandidates(IN const CSdpLevelMedia& rOfferStream,
                                      IN const CSdpLevelMedia& rLocalStream,
                                      IN uint32_t uComponentId,
                                      INOUT CSdpLevelMedia& rAnswerStream);

    // Summary:
    //  Helper to convert address family.
    CSdpParser::EAddressType ConvertFamily(IN CSocketAddr::EAddressFamily eFamilly);

    // Summary:
    //  Sets the default destination address.
    void SetDefaultDestination(IN CSdpLevelSession& rLevelSession,
                               IN unsigned int uStreamIndex,
                               IN const CSocketAddr& rAddress,
                               IN bool bSetAtSessionLevel);

    // Summary:
    //  Sets the RTCP address of the media stream.
    void SetRtcpDefaultDestination(IN CSdpLevelSession& rLevelSession,
                                   IN unsigned int uStreamIndex,
                                   IN const CSocketAddr& rAddress);

    // Summary:
    //  Deactivates the RTCP stream by adding the bandwidth modifiers.
    void DeactivateRtcpStream(INOUT CSdpLevelMedia& rStream);

    // Summary:
    //  Sets the candidate as a remote-candidate in the stream.
    void SetRemoteCandidate(IN const CSdpFieldAttributeIceCandidate& rCandidate,
                            INOUT CSdpLevelMedia& rMedia);

    // Summary:
    //  Removes the specified candidates.
    void RemoveIceCandidate(IN CSdpLevelMedia& rMedia,
                            IN CSdpParser::EAddressType eFamily,
                            IN uint32_t uComponentId);

#endif //#if defined(MXD_SDP_ENABLE_ICE_LITE_WITHOUT_CC_NEGOTIATION)

//-- Hidden Data Members
protected:
    static CVector<CSdpFieldAttributeRtpmap>* ms_pvecSdpRtpmaps;

    // Maximum number of rtpmaps that we will include in an answer that we send.
    static unsigned int ms_uMaxAnswerRtpMaps;

private:
    CSdpLevelSession* m_pLevelSession;

    struct SRtpAlgorithm
    {
        int m_nPayloadType;
        int m_nClockRate;
        int m_nEncodingParams;
    };

    // Struct that describe payload info that will be needed to construct the
    // controller stream with the good payload info in good order.
    struct SPayloadInfo
    {
        // Rtpmap to be uses by the controller stream.
        CSdpFieldAttributeRtpmap m_rtpmap;

        // Media announcement value to be uses by the controller stream.
        const char* m_pszAnnouncement;

        // Position to places the payload into the controller stream.
        unsigned int m_uPosition;
    };

#if defined(MXD_SDP_ENABLE_T38_SUPPORT)
    static ECapsMgrT38MediaState ms_eT38EnabledState;
    bool m_bT38ImplicitBooleanEncoding;
#endif

#if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)
    IMikey* m_pMikey;
    IMikeyIdentity* m_pPeerIdentity;
    CVector<IMikeyKey*> m_vecpMikeyKeys;
    bool m_bSwapStreamDirections;
    bool m_bSwapDirectionsSet;
#if defined (MXD_PKI_ENABLE_SUPPORT)
    CCertificateChain* m_pPeerCertificateChain;
#endif
#endif //-- #if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)

    // Variable to check if this caps manager is generating an answer or a
    // response. true if it is an answer, false if an offer.
    bool m_bAnswer;

    // +1 because eUNKNOWN_RTP_COMPRESSION_ALGORITHM is in in the Algo Map array
    static SRtpAlgorithm const ms_astRtpAlgorithmMap[CSdpParser::eUNKNOWN_RTP_COMPRESSION_ALGORITHM+1];

    // If true, G729 annexb fmtp and G723 annexa are considered. If true,
    // answers will always contain the Vad fmtp attribute. If false, they are
    // completely ignored in offers and not set in answers.
    static bool ms_bEnableVadNegotiation;

    // If true, GenerateAnswer will use the local payload types in the answers.
    // When false, GenerateAnswer will use the offer payload types in the
    // answers. The default value is false.
    static bool ms_bUseLocalPayloadTypes;

    // If true, GenerateAnswer will use the priority order of the local payload
    // types in the answers.  If false, GenerateAnswer will use the priority
    // order of the offer payload types in the answers.  The default value is
    // false, as recommended by the RFC 3264, section 6.1.
    static bool ms_bUseLocalPayloadTypePriority;

#ifdef MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT
    // If true, the silence suppression attribute is considered. If false,
    // it will always be turned off.
    static bool ms_bEnableSilenceSuppNegotiation;
#endif

    // Stores streams that has been selected in a group.
    CVector<CSdpFieldAttributeGroup> m_vecSelectedInGroup;

#if defined(MXD_SDP_ENABLE_ICE_LITE_WITHOUT_CC_NEGOTIATION)
    CSdpParser::EAddressType m_ePreferedMicroliteFamily;
#endif
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  AddStream
//==
//==============================================================================
//
//  Parameters:
//     rMedia:
//        A reference to the SDP object that implements the media level. This
//        can be more flexible (but more complicated) than adding a stream with
//        the first version of AddStream.
//
//     ruStreamIndex:
//        The index associated with the stream that was added. This index can
//        later be used to reference this stream.
//
//  Description:
//      AddStream is used to configure the CSdpCapabilitiesMgr instance with an
//      additional stream that the application is willing to support. This has
//      the effect of adding an "m=" line to the session description that is
//      generated.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::AddStream(
                                       IN  const CSdpLevelMedia& rMedia,
                                       OUT unsigned int& ruStreamIndex)
{
    ruStreamIndex = static_cast<unsigned int>(-1);
    m_pLevelSession->AddMedia(rMedia);
    ruStreamIndex = GetNbStreams() - 1;
}

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
//==============================================================================
//==
//==  AddRtpAudioStream
//==
//==============================================================================
//
//  Parameters:
//     nPort:
//        The port number where the application is willing to receive this audio
//        stream.
//
//     ruStreamIndex:
//        The index associated with the stream that was added. This index can
//        later be used to reference this stream.
//
//  Description:
//      Adds a stream of type CSdpParser::eAUDIO running over the
//      CSdpParser::eRTPAVP protocol. This is just a quicker way to configure
//      the manager for telephony applications that mainly only use the audio
//      media over RTP.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::AddRtpAudioStream(
                                               IN  int nPort,
                                               OUT unsigned int& ruStreamIndex)
{
    ruStreamIndex = static_cast<unsigned int>(-1);
    AddStream(CSdpParser::eAUDIO, CSdpParser::eRTPAVP, nPort, ruStreamIndex);
}
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

//==============================================================================
//==
//==  GetNbStreams
//==
//==============================================================================
//
//  Returns:
//      The number of streams for this session.
//
//  Description:
//      Retrieves the number of streams currently configured for this session.
//      Note that all streams are included in the count, even disabled streams
//      with a port of zero.
//
//==============================================================================
inline unsigned int CSdpCapabilitiesMgr::GetNbStreams() const
{
    return m_pLevelSession->GetNbMedias();
}

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
//==============================================================================
//==
//==  IsStreamSupported
//==
//==============================================================================
//
//  Parameters:
//      uStreamIndex:
//          The index of the stream to query.
//
//  Returns:
//      True if the stream found at unStreamIndes is supported.
//
//  Description:
//      This method queries the manager to know if the stream at uStreamIndex
//      is supported (port is different than 0).
//
//==============================================================================
inline bool CSdpCapabilitiesMgr::IsStreamSupported(
                                             IN unsigned int uStreamIndex) const
{
    MX_ASSERT(uStreamIndex < GetNbStreams());

    return IsStreamSupported(GetStream(uStreamIndex));
}

//==============================================================================
//==
//==  IsStreamSupported
//==
//==============================================================================
//
//  Parameters:
//      rStream:
//          The stream to verify.
//
//  Returns:
//      True if the stream is supported (port is different than zero).
//
//  Description:
//      This method queries the CSdpLevelMedia instance to know if the media
//      it describes is supported. A supported stream has a non-zero port.
//
//==============================================================================
inline bool CSdpCapabilitiesMgr::IsStreamSupported(
                                         IN const CSdpLevelMedia& rStream) const
{
    return rStream.GetMediaAnnouncement().GetTransportPort() != 0;
}
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

//==============================================================================
//==
//==  GetStreamType
//==
//==============================================================================
//
//  Parameters:
//      uStreamIndex:
//          The index of the stream to query.
//
//  Returns:
//      The type of media associated with this stream.
//
//  Description:
//      Queries the type of media associated with the specified stream.
//
//==============================================================================
inline CSdpParser::EMediaType CSdpCapabilitiesMgr::GetStreamType(
                                           IN unsigned int uStreamIndex) const
{
    MX_ASSERT(uStreamIndex < GetNbStreams());

    return GetStream(uStreamIndex).GetMediaAnnouncement().GetMediaTypeId();
}

//==============================================================================
//==
//==  GetStreamTransportProtocol
//==
//==============================================================================
//
//  Parameters:
//      uStreamIndex:
//          The index of the stream to query.
//
//  Returns:
//      The transport type for the specified stream.
//
//  Description:
//      Queries the type of transport associated with a stream.
//
//==============================================================================
inline
CSdpParser::ETransportProtocol CSdpCapabilitiesMgr::GetStreamTransportProtocol(
                                           IN unsigned int uStreamIndex) const
{
    MX_ASSERT(uStreamIndex < GetNbStreams());

    return
        GetStream(uStreamIndex).GetMediaAnnouncement().GetTransportProtocolId();
}

//==============================================================================
//==
//==  GetStream
//==
//==============================================================================
//
//  Parameters:
//      uStreamIndex:
//          The index of the stream to retrieve.
//
//  Returns:
//      A const reference to the CSdpLevelMedia instance associated with
//      the stream.
//
//  Description:
//      Retrieves the CSdpLevelMedia instance associated with a specific stream.
//      By using this instance, the application can have a better control on
//      the content and values of the media description.
//
//==============================================================================
inline const CSdpLevelMedia& CSdpCapabilitiesMgr::GetStream(
                                           IN unsigned int uStreamIndex) const
{
    MX_ASSERT(uStreamIndex < GetNbStreams());

    return m_pLevelSession->GetMedia(uint16_t(uStreamIndex));
}

//==============================================================================
//==
//==  GetStream
//==
//==============================================================================
//
//  Parameters:
//      uStreamIndex:
//          The index of the stream to retrieve.
//
//  Returns:
//      A reference to the CSdpLevelMedia instance associated with the stream.
//
//  Description:
//      Retrieves the CSdpLevelMedia instance associated with a specific stream.
//      By using this instance, the application can have a better control on
//      the content and values of the media description.
//
//==============================================================================
inline CSdpLevelMedia& CSdpCapabilitiesMgr::GetStream(
                                                   IN unsigned int uStreamIndex)
{
    MX_ASSERT(uStreamIndex < GetNbStreams());

    return m_pLevelSession->GetMedia(uint16_t(uStreamIndex));
}

//==============================================================================
//==
//==  GetStreamPort
//==
//==============================================================================
//
//  Parameters:
//      uStreamIndex:
//          The index of the stream to query.
//
//  Returns:
//      The port number associated with the specified stream.
//
//  Description:
//      Retrieves the port associated with a specific stream. Note that a port
//      of zero means that the stream is disabled.
//
//==============================================================================
inline int CSdpCapabilitiesMgr::GetStreamPort(IN unsigned int uStreamIndex) const
{
    MX_ASSERT(uStreamIndex < GetNbStreams());

    return GetStream(uStreamIndex).GetMediaAnnouncement().GetTransportPort();
}

//==============================================================================
//==
//==  SetStreamPort
//==
//==============================================================================
//
//  Parameters:
//      uStreamIndex:
//          The index of the stream.
//
//      nPort:
//          The port associated with the specified stream.
//
//  Returns:
//      The port number associated with the specified stream.
//
//  Description:
//      Sets the port associated with a specific stream.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::SetStreamPort(IN unsigned int uStreamIndex,
                                               IN int nPort)
{
    MX_ASSERT(uStreamIndex < GetNbStreams());

    GetStream(uStreamIndex).GetMediaAnnouncement().SetTransportPort(nPort);
}

//==============================================================================
//==
//==  AddMediaFormat
//==
//==============================================================================
//
//  Parameters:
//     uStreamIndex:
//        The index of the stream to which the format is to be added.
//
//     szFormat:
//        The format string to add.
//
//     ruFormatIndex:
//        The index of the new format in the specified stream.
//
//  Description:
//      This method adds a supported 'format' to the specified stream. 'format'
//      is defined in the SDP specification. This must not be used to add
//      RTP payload types.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::AddMediaFormat(IN  unsigned int uStreamIndex,
                                                IN  const char* szFormat,
                                                OUT unsigned int& ruFormatIndex)
{
    ruFormatIndex = static_cast<unsigned int>(-1);

    MX_ASSERT(uStreamIndex < GetNbStreams());

    CSdpFieldMediaAnnouncement& rMediaAnn =
        GetStream(uStreamIndex).GetMediaAnnouncement();

    rMediaAnn.AddMediaFormat(szFormat);
    ruFormatIndex = rMediaAnn.GetNbMediaFormats() - 1;
}

//==============================================================================
//==
//==  GetNbPayloadTypes
//==
//==============================================================================
//
//  Parameters:
//     uStreamIndex:
//        The index of the stream to query.
//
//  Returns:
//      The number of payloads associated with the specified stream.
//
//  Description:
//      Retrieves the number of payloads associated with a specific stream.
//
//==============================================================================
inline unsigned int CSdpCapabilitiesMgr::GetNbPayloadTypes(
                                           IN unsigned int uStreamIndex) const
{
    MX_ASSERT(uStreamIndex < GetNbStreams());

    return GetStream(uStreamIndex).GetMediaAnnouncement().GetNbMediaFormats();
}

//==============================================================================
//==
//==  CreateSdpPacket
//==
//==============================================================================
//
//  Parameters:
//     szLocalIpAddress:
//        The address of the local host. This address is used to create
//        the 'o=' line of the SDP packet.
//
//     szVersion:
//        The version of the SDP packet to be created. This is used to
//        create the 'o=' line of the session description. This version can
//        be incremented every time the session description changes.
//
//     rSdpPacket:
//        The instance where the SDP packet is to be created.
//
//  Description:
//      This method was deprecated, please use
//      CreateSdpPacket@IN  const char*@IN  const char*@IN  const char*@OUT CSdpPacket&@OUT mxt_result*
//      instead.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::CreateSdpPacket(
                                               IN  const char* szLocalIpAddress,
                                               IN  const char* szVersion,
                                               OUT CSdpPacket& rSdpPacket)
{
    rSdpPacket.Reset();
    CreateSdpPacket(szLocalIpAddress, szLocalIpAddress, szVersion, rSdpPacket);
}

//==============================================================================
//==
//==  GetSdpSession
//==
//==============================================================================
//
//  Returns:
//      Pointer to the SDP session descriptor. It cannot be NULL.
//
//  Description:
//      Provides access to the session descriptor data member.
//
//==============================================================================
inline CSdpLevelSession* CSdpCapabilitiesMgr::GetSdpSession()
{
    return m_pLevelSession;
}

//==============================================================================
//==
//==  GetSdpSession
//==
//==============================================================================
//
//  Returns:
//      Pointer to the SDP session descriptor. It cannot be NULL.
//
//  Description:
//      Provides access to the session descriptor data member.
//
//==============================================================================
inline const CSdpLevelSession* CSdpCapabilitiesMgr::GetSdpSession() const
{
    return m_pLevelSession;
}

//==============================================================================
//==
//==  GetMaxAnswerRtpMaps
//==
//==============================================================================
//
//  Returns:
//      Returns the maximum number of rtpmaps to set in an answer that is sent.
//
//  Description:
//      Provides access to the max number of rtpmaps output in answers. A
//      value of 0 means all supported rtpmaps are output from the offer.
//
//  WARNING:
//      This method is not thread safe.
//
//==============================================================================
inline unsigned int CSdpCapabilitiesMgr::GetMaxAnswerRtpMaps()
{
    return ms_uMaxAnswerRtpMaps;
}

//==============================================================================
//==
//==  SetMaxAnswerRtpMaps
//==
//==============================================================================
//
//  Parameters:
//    uMaxRtpMaps:
//      Maximum number of rtpmaps put in an answer that is sent.
//
//  Description:
//      Use this method to control the number of rtpmaps that you want to put
//      into an answer. By default, all supported rtpmaps in the offer are
//      echoed in the answer. Set a value of 0 to echo all rtpmaps from the
//      offer.
//
//  WARNING:
//      This method is not thread safe.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::SetMaxAnswerRtpMaps(IN unsigned int uMaxRtpMaps)
{
    ms_uMaxAnswerRtpMaps = uMaxRtpMaps;
}

//==============================================================================
//==
//==  UseLocalPayloadTypesInAnswer
//==
//==============================================================================
//
//  Parameters:
//    bUseLocalPayloadTypes:
//      true to use local payload type numbers in the generated answers.
//
//      false to use the offer payload type numbers in the generated answers.
//      This is the default value.
//
//  Description:
//      Set from where the payload type numbers must come.
//
//      Note that RFC 3264 states that "if a particular codec was referenced with
//      a specific payload type number in the offer, that same payload type
//      number SHOULD be used for that codec in the answer" (RFC 3264, section
//      6.1, 5th paragraph).
//
//  WARNING:
//      This method is NOT thread safe.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::UseLocalPayloadTypesInAnswer(
                                                IN bool bUseLocalPayloadTypes)
{
    ms_bUseLocalPayloadTypes = bUseLocalPayloadTypes;
}

//==============================================================================
//==
//==  UseLocalPayloadTypePriorityInAnswer
//==
//==============================================================================
//
//  Parameters:
//    bUseLocalPayloadTypePriority:
//      True to use local priority order for the payload types in the generated
//      answers. False to use the offer priority order for the payload
//      types in the generated answers.
//
//  Description:
//      This function sets priority to use for the payload types in the
//      generated answers. The default value is to use the priority order
//      of the offer, as the RFC 3264 states in the section 6.1:
//      "Although the answerer MAY list the formats in their desired order of
//      preference, it is RECOMMENDED that unless there is a specific reason,
//      the answerer list formats in the same relative order they were
//      present in the offer.".
//
//  WARNING:
//      This method is NOT thread safe.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::UseLocalPayloadTypePriorityInAnswer(IN bool bUseLocalPayloadTypePriority)
{
    ms_bUseLocalPayloadTypePriority = bUseLocalPayloadTypePriority;
}

// BRCM_CUSTOM - [add] Custom SDP generator support
#if !defined(MXD_BRCM_SDP_GENERATOR)
// BRCM_CUSTOM - [end] Custom SDP generator support
//==============================================================================
//==
//==  SetVadNegotiation
//==
//==============================================================================
//
//  Parameters:
//    bEnable:
//      Activation status. True means enabled.
//
//  Description:
//      Enables processing of fmtp attribute for G.729 annexb and G.723 annexa.
//      When disabled, annexa for G.723 and annexb for G.729 are completely
//      ignored in incoming offers, and not included in answers. If enabled,
//      these fmtps are negotiated properly.
//
//      Once you have enabled the feature, you must then add the fmtp attribute
//      to the media lines by using the SetStreamVadSupport() method.
//
//      RFC 3555 section 4.1.9 specifies the "annexb" parameter and its values
//      for G.729. RFC3555 section 3 states that "Any payload-format-specific
//      parameters go in the SDP "a=fmtp" attribute". RFC 3555 section 4.1.3
//      specifies use of the "annexa" parameter for G.723.
//
//      For both codecs, here is the Table of negotiation results:
//
//      Offer       LocalCfg    Answer
//      ---------   --------    --------
//      ABSENT      yes         yes    (RFC 3555 4.1.9: ABSENT maps to "yes")
//      ABSENT      no          no
//
//      yes         yes         yes
//      yes         no          no
//
//      no          yes         no
//      no          no          no
//
//  See Also:
//      GetVadNegotiation, SetStreamVadSupport
//
//==============================================================================
inline void CSdpCapabilitiesMgr::SetVadNegotiation(IN bool bEnable)
{
    ms_bEnableVadNegotiation = bEnable;
}

//==============================================================================
//==
//==  GetVadNegotiation
//==
//==============================================================================
//
//  Returns:
//      Current activity status of the VAD negotiation feature.
//
//  Description:
//      Fetches current setting for processing of fmtp attribute for G.729 annexb
//      and G.723 annexa.
//
//  See Also:
//      SetVadNegotiation
//
//==============================================================================
inline bool CSdpCapabilitiesMgr::GetVadNegotiation()
{
    return ms_bEnableVadNegotiation;
}
// BRCM_CUSTOM - [add] Custom SDP generator support
#endif // MXD_BRCM_SDP_GENERATOR
// BRCM_CUSTOM - [end] Custom SDP generator support

#ifdef MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT
//==============================================================================
//==
//==  SetSilenceSuppressionNegotiation
//==
//==============================================================================
//
//  Parameters:
//      bEnable:
//          Enables the negotiation of the silence suppression attribute.
//
//  Description:
//      Enables the processing of the silence suppression attribute as described
//      in RFC 3108. The silence suppression attribute is used to indicate
//      silence suppression support for PCMU and PCMA. Capabilities are
//      negotiated as follows:
//
//      Table of negotiation results:
//
//      Offer       LocalCfg    Answer
//      ---------   --------    --------
//      ABSENT      on          off     (ABSENT maps to "off")
//      ABSENT      off         off
//
//      on          on          on
//      on          off         off
//
//      off         on          off
//      off         off         off
//
//  See Also:
//      GetSilenceSuppressionNegotiation
//
//==============================================================================
inline void CSdpCapabilitiesMgr::SetSilenceSuppressionNegotiation(IN bool bEnable)
{
    ms_bEnableSilenceSuppNegotiation = bEnable;
}

//==============================================================================
//==
//==  GetSilenceSuppressionNegotiation
//==
//==============================================================================
//
//  Returns:
//      Current activity status of the silence suppression negotiation feature.
//
//  Description:
//      Fetches the current setting for processing of silence suppression
//      attribute.
//
//  See Also:
//      SetSilenceSuppressionNegotiation
//
//==============================================================================
inline bool CSdpCapabilitiesMgr::GetSilenceSuppressionNegotiation()
{
    return ms_bEnableSilenceSuppNegotiation;
}

//==============================================================================
//==
//==  IsSilenceSuppressionSupportedInStream
//==
//==============================================================================
//
//  Parameters:
//      rStream:
//          Stream to validate for the silence suppression attribute.
//
//  Returns:
//      True if silence suppression is currently supported in the stream, false
//      otherwise.
//
//  Description:
//      Gets the state of silence suppression for the media.
//
//  See Also:
//      GetSilenceSuppressionNegotiation, SetSilenceSuppressionNegotiation
//
//==============================================================================
inline bool CSdpCapabilitiesMgr::IsSilenceSuppressionSupportedInStream( IN const CSdpLevelMedia& rStream ) const
{
    return rStream.GetSilenceSuppressionSupport().IsOn();
}

#endif //-- #ifdef MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT

#ifdef MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE
//==============================================================================
//==
//==  GetFmtpTelEvent
//==
//==============================================================================
//
//  Parameters:
//    uStreamIndex:
//      The index of the stream to which append the fmtp attribute.
//
//      The index MUST be smaller than GetNbStreams().
//
//  Returns:
//    The first telephone-event fmtp attribute found in the selected stream.
//
//    NULL if no telephone-event fmtp attribute was found in the selected
//    stream.
//
//  Description:
//      Gets the telephone-event fmtp attribute in the selected stream.
//
//==============================================================================
inline const CSdpFmtpTelEvent* CSdpCapabilitiesMgr::GetFmtpTelEvent(
                                            IN uint32_t uStreamIndex ) const
{
    return (const_cast<CSdpCapabilitiesMgr*>(this))->GetFmtpTelEvent(
        uStreamIndex);
}
#endif // #ifdef MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE

#ifdef MXD_SDP_ENABLE_REDUNDANCY_FMTP_ATTRIBUTE
//==============================================================================
//==
//==  GetFmtpRedundancy
//==
//==============================================================================
//
//  Parameters:
//    uStreamIndex:
//      The index of the stream to which append the fmtp attribute.
//
//      The index MUST be smaller than GetNbStreams().
//
//  Returns:
//    The first redundancy fmtp attribute found in the selected stream.
//
//    NULL if no redundancy fmtp attribute was found in the selected stream.
//
//  Description:
//      Gets the redundancy fmtp attribute in the selected stream.
//
//==============================================================================
inline const CSdpFmtpRedundancy* CSdpCapabilitiesMgr::GetFmtpRedundancy(
                                                IN uint32_t uStreamIndex ) const
{
    return (const_cast<CSdpCapabilitiesMgr*>(this))->GetFmtpRedundancy(
        uStreamIndex);
}
#endif // #ifdef MXD_SDP_ENABLE_REDUNDANCY_FMTP_ATTRIBUTE

#if defined(MXD_SDP_ENABLE_T38_SUPPORT)
//==============================================================================
//==
//==  EnableT38
//==
//==============================================================================
//
//  Parameters:
//   eEnabledState:
//     - eSDP_CAPS_MGR_T38_MEDIA_DISABLED: T.38 is disabled;
//     - eSDP_CAPS_MGR_T38_MEDIA_ENABLED_ONLY_T38 : T.38 is negotiated alone
//                                                  only;
//     - eSDP_CAPS_MGR_T38_MEDIA_ENABLED_WITH_OTHER_MEDIA: T.38 is negotiated
//                                                         alone or with other
//                                                         media.
//
// Description:
//  Configures whether or not the offers/answers created with all instances
//  include T.38 capabilities.
//
//  When eEnabledState is eSDP_CAPS_MGR_T38_MEDIA_ENABLED_WITH_OTHER_MEDIA,
//  all offers/answers include T.38 capabilities along with any other
//  media streams configured by the user.
//
//  When eEnabledState is eSDP_CAPS_MGR_T38_MEDIA_ENABLED_ONLY_T38,
//  all offers only include T.38 capabilities and T.38 media is
//  deactivated if other media streams are present in an answer.
//
//  When eEnabledState is eSDP_CAPS_MGR_T38_MEDIA_DISABLED, all offers/answers
//  do not include any T.38 capabilities and only include the other
//  configured media streams.
//
//  If this method is not called, all the SDP capabilities managers
//  assume that T.38 is not enabled.
//==============================================================================
inline void CSdpCapabilitiesMgr::EnableT38(IN ECapsMgrT38MediaState eEnabledState)
{
    ms_eT38EnabledState = eEnabledState;
}

//==============================================================================
//==
//==  IsT38BooleanImplicitEncoding
//==
//==============================================================================
//
//  Returns:
//      True if the T.38 boolean encoding is set to implicit. False if the T.38
//      boolean encoding is set to explicit.
//
//  Description:
//      Indicates if the T.38 boolean encoding is set to implicit.
//
//==============================================================================
inline bool CSdpCapabilitiesMgr::IsT38BooleanImplicitEncoding()
{
    return m_bT38ImplicitBooleanEncoding;
}
#endif

//==============================================================================
//==
//==  AddPhone
//==
//==============================================================================
//
//  Parameters:
//     rPhone:
//        A reference to the CSdpFieldPhone object that implements the phone
//        number.
//
//  Description:
//      AddPhone adds a "p=" line to the session description that is generated.
//      It can be called multiple times to add more than one "p=" lines.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::AddPhone(IN const CSdpFieldPhone& rPhone)
{
   m_pLevelSession->AddPhone(rPhone);
}

//==============================================================================
//==
//==  GetNbPhones
//==
//==============================================================================
//
//  Returns:
//      The number of phone fields for this session.
//
//  Description:
//      Retrieves the number of phone fields currently configured for this session.
//
//==============================================================================
inline unsigned int CSdpCapabilitiesMgr::GetNbPhones() const
{
    return m_pLevelSession->GetNbPhones();
}

//==============================================================================
//==
//==  GetPhone
//==
//==============================================================================
//
//  Parameters:
//      uPhoneIndex:
//          The index of the phone field to retrieve.
//
//  Returns:
//      A reference to the CSdpFieldPhone instance.
//
//  Description:
//      Retrieves the CSdpFieldPhone instance associated with a specific index.
//
//==============================================================================
inline const CSdpFieldPhone&
CSdpCapabilitiesMgr::GetPhone(IN unsigned int uPhoneIndex) const
{
    MX_ASSERT(uPhoneIndex < GetNbPhones());

    // BRCM_CUSTOM - [mod] Suppress warning
    return m_pLevelSession->GetPhone( static_cast<uint16_t> (uPhoneIndex));
    // BRCM_CUSTOM - [end] Suppress warning
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
inline void CSdpCapabilitiesMgr::AddGroup(
                                      IN const CSdpFieldAttributeGroup& rGroup)
{
    m_pLevelSession->AddGroup(rGroup);
}

#if defined(MXD_SDP_ENABLE_ICE_LITE_WITHOUT_CC_NEGOTIATION)
//==============================================================================
//==
//==  AddIceLiteAttribute
//==
//==============================================================================
//
//  Parameters:
//      rSession:
//       Reference to the level session.
//
//  Description:
//      Adds the ice-lite attribute to the level session.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::AddIceLiteAttribute(IN CSdpLevelSession& rSession)
{
    rSession.SetIceLite(true);
}

//==============================================================================
//==
//==  ConvertFamily
//==
//==============================================================================
//
//  Parameters:
//      eFamilly:
//       The address family in the CSocketAddr::EAddressFamily format.
//
//  Returns:
//      The address family in the CSdpParser::EAddressType format
//
//  Description:
//      Converts the address family from the CSocketAddr::EAddressFamily to the
//      CSdpParser::EAddressType.
//
//==============================================================================
inline CSdpParser::EAddressType CSdpCapabilitiesMgr::ConvertFamily(IN CSocketAddr::EAddressFamily eFamilly)
{
    return eFamilly == CSocketAddr::eINET ? CSdpParser::eIP4 : CSdpParser::eIP6;
}

//==============================================================================
//==
//==  SetMicroLiteDefaultFamily
//==
//==============================================================================
//
//  Parameters:
//      eAddressFamily:
//       The default address family to use.
//
//  Description:
//      Sets the MicroLite default family to use for default destination
//      address.
//
//==============================================================================
inline void CSdpCapabilitiesMgr::SetMicroLiteDefaultFamily(IN CSdpParser::EAddressType eAddressFamily)
{
    m_ePreferedMicroliteFamily = eAddressFamily;
}

#endif //#if defined(MXD_SDP_ENABLE_ICE_LITE_WITHOUT_CC_NEGOTIATION)

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSDPCAPABILITIESMGR_H
