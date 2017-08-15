//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPMESSAGEBODY_H
#define MXG_CSIPMESSAGEBODY_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h" // Data member
#endif

#ifndef MXG_CTOKEN_H
#include "SipParser/CToken.h" // Data member
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CHeaderList;
class CBlob;
class CSipHeader;
class CSipPacketParser;
class CString;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipMessageBody
//=========================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//  The CSipMessageBody is used to represent one or more bodies that may be
//  included by a SIP packet. This class is used when sending and receiving SIP
//  packets.
//
//  One or more body can be configured in an instance of this class. When a
//  single body is configured, this class represents this body as it was
//  configured. For example, a body of type "application/sdp" is represented as
//  "application/sdp". When multiple bodies are configured in an instance of
//  this class, they are then represented as MIME objects.
//
//  In order to parse or serialize a MIME object, the application MUST use the
//  CSipPacketParser class. These methods are respectively CreateSipMessageBody
//  and Serialize.
//
//  #Creating Bodies#
//
//  There are two ways to add a body to an instance of this class. The first is
//  AddBody, which takes a CBlob as a parameter. This method adds a body to the
//  level that is being represented by this instance. The second AddBody method
//  takes a CSipMessageBody as a parameter. This additional body is inserted as
//  an "inner" body to the current body. Refer to the example section to
//  understand this concept of "inner" body and insertion at the current level.
//
//  When configuring bodies, this class automatically manages the Content-Length
//  header according to the size of the configured bodies. The user of this
//  class never has to configure a Content-Length header.
//
//  #Accessing Existing Bodies#
//
//  To access the bodies, you first need to parse them. This is done by calling
//  CSipPacket::CreateSipMessageBody, which calls ParseBody, giving it the
//  packet to parse. Once this is done, GetNumberOfBodies gives the number of
//  bodies at the current level. GetExternalMimeHeaders returns the headers that
//  are external to the current level. Once the number of bodies is known, the
//  application can get the MIME headers and content associated with each of
//  them. The content is obtained via GetBlobBody and the headers via
//  GetMimeHeaders. The application is responsible to parse the bodies obtained
//  via the GetBlobBody method. It can use whatever means to parse the body. If
//  GetBlobBody returns NULL for a specific index, it means that there is an
//  inner message-body. The message-body is obtained via GetSipMessageBody. In
//  case there no inner body for a specific index, GetSipMessageBody returns
//  NULL and GetBlobBody non-NULL. Refer to the example section to understand
//  this concept of "inner" body and fetching from the current level.
//
//  MIME is defined in RFCs 2045, 2046, and 2047.
//
//
// Example:
//   Assume you need to create the following body for an INVITE to be sent:
//
//  <CODE>
//   // Sample INVITE taken from RFC 3893.
//   //------------------------------------
//
//   INVITE sip:bob@example.net SIP/2.0
//   Via: SIP/2.0/UDP pc33.example.com;branch=z9hG4bKnashds8
//   To: Bob <sip:bob@example.net>
//   From: Alice <sip:alice@example.com>;tag=1928301774
//   Call-ID: a84b4c76e66710
//   CSeq: 314159 INVITE
//   Max-Forwards: 70
//   Date: Thu, 21 Feb 2002 13:02:03 GMT
//   Contact: <sip:alice@pc33.example.com>
//   Content-Type: multipart/mixed; boundary=unique-boundary-1
//
//   --unique-boundary-1
//
//   Content-Type: application/sdp
//   Content-Length: 147
//
//   v=0
//   o=UserA 2890844526 2890844526 IN IP4 example.com
//   s=Session SDP
//   c=IN IP4 pc33.example.com
//   t=0 0
//   m=audio 49172 RTP/AVP 0
//   a=rtpmap:0 PCMU/8000
//
//   --unique-boundary-1
//   Content-Type: multipart/signed;
//     protocol="application/pkcs7-signature";
//     micalg=sha1; boundary=boundary42
//   Content-Length: 608
//
//   --boundary42
//   Content-Type: message/sipfrag
//   Content-Disposition: aib; handling=optional
//
//   From: Alice <sip:alice@example.com>
//   To: Bob <sip:bob@example.net>
//   Contact: <sip:alice@pc33.example.com>
//   Date: Thu, 21 Feb 2002 13:02:03 GMT
//   Call-ID: a84b4c76e66710
//   CSeq: 314159 INVITE
//
//   --boundary42
//   Content-Type: application/pkcs7-signature; name=smime.p7s
//   Content-Transfer-Encoding: base64
//   Content-Disposition: attachment; filename=smime.p7s;
//      handling=required
//
//   ghyHhHUujhJhjH77n8HHGTrfvbnj756tbB9HG4VQpfyF467GhIGfHfYT6
//   4VQpfyF467GhIGfHfYT6jH77n8HHGghyHhHUujhJh756tbB9HGTrfvbnj
//   n8HHGTrfvhJhjH776tbB9HG4VQbnj7567GhIGfHfYT6ghyHhHUujpfyF4
//   7GhIGfHfYT64VQbnj756
//
//   --boundary42--
//
//   --unique-boundary-1--
//
//  </CODE>
//
//   The following pseudo-code would need to be implemented:
//
//  <CODE>
//
//   1- Create 3 CSipMessageBody instances:
//      -> bodyInner2, for the SIP Authenticated Body (sipfrag and signature)
//      -> bodyInner, for the SDP and the signed AIB
//      -> bodyOuter, that will contain all bodies.
//
//   2- Start by configuring bodyInner with its two bodies:
//      -> bodySipFrag, Create a new message body for the sipfrag.
//      -> Create CSipHeader for Content-Type (pHdrContentTypeSipFrag)
//      -> Configure pHdrContentTypeSipFrag with "message/sipfrag"
//      -> bodySipFrag.AddBody(blobSipFrag, TO pHdrContentTypeSipFrag);
//
//      -> bodySignature, Create a new message body for the signature.
//      -> Create CSipHeader for Content-Type (pHdrContentTypeSignature)
//      -> Configure hdrContentTypeSipFrag with "application/pkcs7-signature"
//      -> Configure additional parameters in pHdrContentTypeSignature.
//      -> Create CHeaderList that will contain extra header to add as
//         extra header (pExtraHeader).
//      -> bodySignature, Create a new message body for the signature.
//      -> bodySignature.AddBody(bodySignature,
//                               TO pHdrContentTypeSignature,
//                               TO pExtraHeader);
//
//      -> bodyInner2.AddBody(bodySipFrag);
//      -> bodyInner2.AddBody(bodySignature);
//
//   3- Set the Content-Type of bodyInner2:
//      -> Create CSipHeader for Content-Type (pHdrContentTypeInner)
//      -> Configure pHdrContentTypeInner with "multipart/signed"
//      -> Configure additional parameters in pHdrContentTypeInner
//      -> bodyInner2.SetExternalMimeHeaders(TO pHdrContentTypeInner);
//
//   4- Configure bodyInner with its two bodies:
//      -> bodySdp, Create a new message body for the SDP.
//      -> Create CSipHeader for Content-Type (pHdrContentTypeSdp)
//      -> Configure pHdrContentTypeSdp with "application/sdp"
//      -> bodySdp.AddBody(blobSDP, TO pHdrContentTypeSdp);
//
//      -> bodyInner.AddBody(bodySdp);
//      -> bodyInner.AddBody(bodyInner2);
//
//   5- Add the bodyInner and bodyInner2 to the bodyOuter.
//      -> bodyOuter.AddBody(bodyInner);
//      -> bodyOuter.AddBody(bodyInner2);
//
//   6- Set the Content-Type of bodyOuter:
//      -> Create CSipHeader for Content-Type (pHdrContentTypeOuter)
//      -> Configure hdrContentTypeOuter with "multipart/mixed"
//      -> bodyOuter.SetExternalMimeHeaders(TO pHdrContentTypeOuter);
//
//   7- Pass bodyOuter as a parameter to the ISipSessionSvc::Invite() to send
//      the proper payload.
//
//  </CODE>
//
//   Assume you need to parse the same body for an INVITE that has been
//   received. The following pseudo-code would need to be implemented:
//
//  <CODE>
//
//   1- Parse the message-body:
//      -> body <- rPacket.CreateSipMessageBody();
//      -> body.GetExternalMimeHeaders();
//
//   2- Get the number of bodies:
//      -> body.GetNumberOfBodies() which returns 2
//
//   3- Get the body and MIME headers for first index starting at zero
//      -> body.GetBlobBody(0)
//      -> body.GetMimeHeaders(0)
//
//   4- Get the body for second index:
//      -> body.GetBlobBody(1) but it returns NULL meaning that it is an inner
//      body at the current level.
//      -> body.GetSipMessageBody(1) which gives innerBody
//
//   5- Get the external MIME headers for this inner body:
//      -> innerBody.GetExternalMimeHeaders();.
//
//   6- Get the innerBody bodies and headers:
//      -> innerBody.GetNumberOfBodies() which also returns 2
//      -> innerBody.GetMimeHeaders(0);
//      -> innerBody.GetBlobBody(0)
//      -> innerBody.GetMimeHeaders(1);
//      -> innerBody.GetBlobBody(1)
//
//  </CODE>
//
//  <CODE>
//    RFC 2045 ABNF:
//
//      attribute := token
//                   ; Matching of attributes
//                   ; is ALWAYS case-insensitive.
//
//      composite-type := "message" / "multipart" / extension-token
//
//      content := "Content-Type" ":" type "/" subtype
//                 *(";" parameter)
//                 ; Matching of media type and subtype
//                 ; is ALWAYS case-insensitive.
//
//      description := "Content-Description" ":" *text
//
//      discrete-type := "text" / "image" / "audio" / "video" /
//                       "application" / extension-token
//
//      encoding := "Content-Transfer-Encoding" ":" mechanism
//
//      entity-headers := [ content CRLF ]
//                        [ encoding CRLF ]
//                        [ id CRLF ]
//                        [ description CRLF ]
//                        *( MIME-extension-field CRLF )
//
//      extension-token := ietf-token / x-token
//
//      hex-octet := "=" 2(DIGIT / "A" / "B" / "C" / "D" / "E" / "F")
//                   ; Octet must be used for characters > 127, =,
//                   ; SPACEs or TABs at the ends of lines, and is
//                   ; recommended for any character not listed in
//                   ; RFC 2049 as "mail-safe".
//
//      iana-token := <A publicly-defined extension token. Tokens
//                    of this form must be registered with IANA
//                    as specified in RFC 2048.>
//
//      ietf-token := <An extension token defined by a
//                    standards-track RFC and registered
//                    with IANA.>
//
//      id := "Content-ID" ":" msg-id
//
//      mechanism := "7bit" / "8bit" / "binary" /
//                   "quoted-printable" / "base64" /
//                   ietf-token / x-token
//
//      MIME-extension-field := <Any RFC 822 header field which
//                              begins with the string
//                             "Content-">
//
//      MIME-message-headers := entity-headers
//                              fields
//                              version CRLF
//                              ; The ordering of the header
//                              ; fields implied by this BNF
//                              ; definition should be ignored.
//
//      MIME-part-headers := entity-headers
//                           [fields]
//                           ; Any field not beginning with
//                           ; "content-" can have no defined
//                           ; meaning and may be ignored.
//                           ; The ordering of the header
//                           ; fields implied by this BNF
//                           ; definition should be ignored.
//
//      parameter := attribute "=" value
//
//      ptext := hex-octet / safe-char
//
//      qp-line := *(qp-segment transport-padding CRLF)
//                 qp-part transport-padding
//
//      qp-part := qp-section
//                 ; Maximum length of 76 characters
//
//      qp-section := [*(ptext / SPACE / TAB) ptext]
//
//      qp-segment := qp-section *(SPACE / TAB) "="
//                    ; Maximum length of 76 characters
//
//      quoted-printable := qp-line *(CRLF qp-line)
//
//      safe-char := <any octet with decimal value of 33 through
//                   60 inclusive, and 62 through 126>
//                   ; Characters not listed as "mail-safe" in
//                   ; RFC 2049 are also not recommended.
//
//      subtype := extension-token / iana-token
//
//      token := 1*<any (US-ASCII) CHAR except SPACE, CTLs,
//               or tspecials>
//
//      transport-padding := *LWSP-char
//                           ; Composers MUST NOT generate
//                           ; non-zero length transport
//                           ; padding, but receivers MUST
//                           ; be able to handle padding
//                           ; added by message transports.
//
//      tspecials :=  "(" / ")" / "<" / ">" / "@" /
//                    "," / ";" / ":" / "\" / <">
//                    "/" / "[" / "]" / "?" / "="
//                    ; Must be in quoted-string,
//                    ; to use within parameter values
//
//      type := discrete-type / composite-type
//
//      value := token / quoted-string
//
//      version := "MIME-Version" ":" 1*DIGIT "." 1*DIGIT
//
//      x-token := <The two characters "X-" or "x-" followed, with
//                 no  intervening white space, by any token>
//
//    RFC 2392 ABNF:
//
//      msg-id    = url-addr-spec
//
//      url-addr-spec = addr-spec  ; URL encoding of RFC 822 addr-spec
//
//    RFC 2046 ABNF:
//
//      boundary := 0*69<bchars> bcharsnospace
//
//      bchars := bcharsnospace / " "
//
//      bcharsnospace := DIGIT / ALPHA / "'" / "(" / ")" /
//                     "+" / "_" / "," / "-" / "." /
//                      "/" / ":" / "=" / "?"
//
//      dash-boundary := "--" boundary
//                      ; boundary taken from the value of
//                      ; boundary parameter of the
//                      ; Content-Type field.
//
//      multipart-body := [preamble CRLF]
//                       dash-boundary transport-padding CRLF
//                       body-part *encapsulation
//                       close-delimiter transport-padding
//                       [CRLF epilogue]
//
//      transport-padding := *LWSP-char
//                          ; Composers MUST NOT generate
//                          ; non-zero length transport
//                          ; padding, but receivers MUST
//                          ; be able to handle padding
//                          ; added by message transports.
//
//      encapsulation := delimiter transport-padding
//                      CRLF body-part
//
//      delimiter := CRLF dash-boundary
//
//      close-delimiter := delimiter "--"
//
//      preamble := discard-text
//
//      epilogue := discard-text
//
//      discard-text := *(*text CRLF) *text
//                     ; May be ignored or discarded.
//
//      body-part := MIME-part-headers [CRLF *OCTET]
//                  ; Lines in a body-part must not start
//                  ; with the specified dash-boundary and
//                  ; the delimiter must not appear anywhere
//                  ; in the body part.  Note that the
//                  ; semantics of a body-part differ from
//                  ; the semantics of a message, as
//                  ; described in the text.
//
//      OCTET := <any 0-255 octet value>
//
//    RFC 1847 ABNF: this RFC defines the Content-Type headers for multipart
//    signed and encrypted contents
//
//      Definition of Multipart/Signed
//
//      (1)  MIME type name: multipart
//      (2)  MIME subtype name: signed
//      (3)  Required parameters: boundary, protocol, and micalg
//
//      boundary := "boundary" "=" value
//      parameter := "protocol" "=" value
//      value := <"> type "/" subtype <">
//      parameter := "micalg" "=" value
//
//      Definition of Multipart/Encrypted
//
//      (1)  MIME type name: multipart
//      (2)  MIME subtype name: encrypted
//      (3)  Required parameters: boundary, protocol
//
//      boundary := "boundary" "=" value
//      parameter := "protocol" "=" value
//      value := <"> type "/" subtype <">
//
//  </CODE>
//
// Location:
//   SipParser/CSipMessageBody.h
//
//==============================================================================
class CSipMessageBody
{
//-- Friend Declarations
//------------------------
// Needed so the CSipPacketParser can access SetBoundary, ParseBody, Serialize
// and non-const GetExternalMimeHeaders protected methods.
//-----------------------------------------------------------------------------
friend class CSipPacketParser;

//-- New types within class namespace
//-----------------------------------
public:
protected:
private:

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipMessageBody();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipMessageBody(IN const CSipMessageBody& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipMessageBody();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSipMessageBody& operator=(IN const CSipMessageBody& rSrc);

    // Summary:
    //  Appends a body at the level represented by this instance.
    //------------------------------------------------------------
    mxt_result AddBody(IN TO CBlob* pBlobBody,
                       IN TO CSipHeader* pHeaderContentType,
                       IN TO CHeaderList* pAdditionalMimeHeaders = NULL);

    // Summary:
    //  Appends an inner body at the level represented by this instance.
    //-------------------------------------------------------------------
    mxt_result AddBody(IN TO CSipMessageBody* pInnerBody);

    // Summary:
    //  Configures the information pertaining to the body at this level.
    //-------------------------------------------------------------------
    mxt_result SetExternalMimeHeaders(IN TO CSipHeader* pHeaderContentType,
                                      IN TO CHeaderList* pAdditionalMimeHeaders = NULL);

    // Summary:
    //  Provides access to the mime headers for this level.
    //------------------------------------------------------
    const CHeaderList* GetExternalMimeHeaders() const;

    // Summary:
    //  Gets the body for this level.
    //--------------------------------
    const CBlob* GetBlobBody() const;

    // Summary:
    //  Gets the body for this level.
    //--------------------------------
    CBlob* GetBlobBody();

    // Summary:
    //  Gets the number of bodies held by this level.
    //------------------------------------------------
    unsigned int GetNumberOfBodies() const;

    // Summary:
    //  Gets the mime headers associated with the specified body.
    //------------------------------------------------------------
    const CHeaderList* GetMimeHeaders(IN unsigned int uIndex) const;

    // Summary:
    //  Gets the body at unIndex, assuming it is a blob.
    //---------------------------------------------------
    const CBlob* GetBlobBody(IN unsigned int uIndex) const;

    // Summary:
    //  Gets the body at unIndex, assuming it is a recursive CSipMessageBody.
    //------------------------------------------------------------------------
    const CSipMessageBody* GetSipMessageBody(IN unsigned int uIndex) const;

    // Summary:
    //  Clears all data back to its initial state.
    //---------------------------------------------
    void Reset();

    // Summary:
    //  If the Content-Type of this body is multipart, sets the
    //  outer level boundary and serializes this body information
    //  into a blob.
    void Serialize(INOUT CBlob& rBlob);

//-- Hidden Methods
//-------------------
protected:

    // <COMBINE CSipMessageBody::GetExternalMimeHeaders>
    //---------------------------------------------------
    CHeaderList* GetExternalMimeHeaders();

    // Summary:
    //  Sets the boundary to use for this level.
    //-------------------------------------------
    void SetBoundary(IN const CToken& rTokBoundary);

    // Summary:
    //  Gets the boundary used for this level.
    //-----------------------------------------
    const CToken& GetBoundary();

    // Summary:
    //  Parses the body of a SIP packet.
    //-----------------------------------
    mxt_result ParseBody(IN const CSipPacketParser& rPacket);

    // Summary:
    //  Serializes this body information into a blob.
    //------------------------------------------------
    void SerializeHelper(INOUT CBlob& rBlob) const;

    // Summary:
    //  Outputs the --boundary with -- at the end if specified.
    //---------------------------------------------------------
    void OutputBoundaryHelper(IN bool bOutputTerminalHyphens,
                              INOUT CBlob& rBlob) const;

    // Summary:
    //  Parses the specified payload and builds the inner bodies recursively.
    //------------------------------------------------------------------------
    mxt_result SetNextMessageBodies(IN CSipMessageBody* pCurrentMessageBody,
                                    IN const CBlob* pPayload,
                                    INOUT unsigned int& ruIndex);

    // Summary:
    //  Finds the first occurrence of the boundary.
    //---------------------------------------------
    mxt_result FindBoundaryDelimiterHelper(IN CSipMessageBody* pCurrentMessageBody,
                                           IN const CBlob* pPayload,
                                           INOUT unsigned int& ruIndex,
                                           OUT unsigned int& ruSizeToRemove,
                                           OUT bool& rbFoundTerminalHyphens);

private:


//-- Hidden Data Members
//------------------------
protected:
    // The list of bodies. If size is non-zero, it means that this message-body
    // has one or multiple inner bodies.
    //--------------------------------------------------------------------------
    CVector<CSipMessageBody*> m_vecpMessageBody;

    // The MIME headers list. It could be the headers external to this
    // message-body. It is the case if m_pBlobBody has a NULL value. If
    // m_pBlobBody has a non-NULL value, it means that the headers are
    // associated to the current blob at the same level. Settings of theses
    // headers are left to the application.
    // Headers can be of any type, most common type are:
    // - Content-Type
    // - Content-Length
    // - MIME-Version
    // - Content-Description
    // - Content-Disposition
    // - Content-ID
    // - Content-Encoding
    // - Content-Language
    // - Content-Transfer-Encoding
    CHeaderList* m_pHdrList;

    // The body of this message, i.e the raw data. If NULL, it means that this
    // message have inner bodies. In that case, the size of m_vecpMessageBody is
    // greater than zero. Otherwise, it has no inner bodies and the application
    // is responsible to parse this body.
    //--------------------------------------------------------------------------
    CBlob* m_pBlobBody;

    // The boundary of this message-body.
    //------------------------------------
    CToken m_tokBoundary;

private:

public:

    // The default value for most outer body boundary, this is "level-0".
    //--------------------------------------------------------------------
    static const char* const ms_pszBOUNDARY_OUTER_LEVEL;

    // The "multipart" Content-Type media-type value.
    //------------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_MULTIPART_MEDIA_TYPE;

    // The "message" Content-Type media-type value.
    //----------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_MESSAGE_MEDIA_TYPE;

    // The "text" Content-Type media-type value.
    //-------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_TEXT_MEDIA_TYPE;

    // The "application" Content-Type media-type value.
    //--------------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_APPLICATION_MEDIA_TYPE;

    // The "rfc822" Content-Type media-subtype value.
    //------------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_RFC822_MEDIA_SUBTYPE;

    // The "digest" Content-Type media-subtype value.
    //------------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_DIGEST_MEDIA_SUBTYPE;

    // The "plain" Content-Type media-subtype value.
    //-----------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_PLAIN_MEDIA_SUBTYPE;

    // The "sdp" Content-Type media-subtype value.
    //---------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_SDP_MEDIA_SUBTYPE;

    // The "signed" Content-Type media-subtype value.
    //------------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_SIGNED_MEDIA_SUBTYPE;

    // The "sip" Content-Type media-subtype value.
    //---------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_SIP_MEDIA_SUBTYPE;

    // The "pkcs7-signature" Content-Type media-subtype value.
    //---------------------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_PKCS7_SIGNATURE_MEDIA_SUBTYPE;

    // The "mixed" Content-Type media-subtype value.
    //-----------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_MIXED_MEDIA_SUBTYPE;

    // The "sipfrag" Content-Type media-subtype value.
    //-------------------------------------------------
    static const char* const ms_pszCONTENT_TYPE_SIPFRAG_MEDIA_SUBTYPE;


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetExternalMimeHeaders
//==
//==============================================================================
//
//  Returns:
//      The headers list associated with the current level of this message-body.
//      It could be NULL if no headers are set or none are found when parsing.
//
//  Description:
//      Gets the headers that are external to this message-body. It could be
//      NULL if no headers are parsed or set.
//
//  See Also:
//      SetExternalMimeHeaders
//
//==EDOC========================================================================
inline const CHeaderList* CSipMessageBody::GetExternalMimeHeaders() const
{
    return m_pHdrList;
}

//==SDOC========================================================================
//==
//==  GetExternalMimeHeaders
//==
//==============================================================================
//
//  Returns:
//      The headers list associated with the current level of this message-body.
//      It could be NULL if no headers are set or none are found when parsing.
//
//  Description:
//      Gets the headers that are external to this message-body. It could be
//      NULL if no headers are parsed or set.
//
//  See Also:
//      SetExternalMimeHeaders
//
//==EDOC========================================================================
inline CHeaderList* CSipMessageBody::GetExternalMimeHeaders()
{
    return m_pHdrList;
}

//==SDOC========================================================================
//==
//==  GetBlobBody
//==
//==============================================================================
//
//  Returns:
//      The blob body or NULL if there is no content
//
//  Description:
//      Gets the body for this level. The application is responsible to parse
//      this header.
//
//==EDOC========================================================================
inline const CBlob* CSipMessageBody::GetBlobBody() const
{
    return m_pBlobBody;
}

//==SDOC========================================================================
//==
//==  GetBlobBody
//==
//==============================================================================
//
//  Returns:
//      The blob body or NULL if there is no content.
//
//  Description:
//      Gets the body for this level. The application is responsible to parse
//      this header.
//
//==EDOC========================================================================
inline CBlob* CSipMessageBody::GetBlobBody()
{
    return m_pBlobBody;
}

//==SDOC========================================================================
//==
//==  GetNumberOfBodies
//==
//==============================================================================
//
//  Returns:
//      The number of bodies attached to another nested level. It could be zero
//      if no bodies are nested to this message-body.
//
//  Description:
//      Gets the number of nested bodies in this message-body. It could be zero,
//      in that case GetBlobBody it returns non-NULL.
//
//  See Also:
//      GetBlobBody
//
//==EDOC========================================================================
inline unsigned int CSipMessageBody::GetNumberOfBodies() const
{
    return m_vecpMessageBody.GetSize();
}

//==SDOC========================================================================
//==
//==  GetBoundary
//==
//==============================================================================
//
//  Returns:
//      The boundary, can be empty if not previously set.
//
//  Description:
//      Gets the boundary used for this level. This method has no use for the
//      application, it should not be called.
//
//==EDOC========================================================================
inline const CToken& CSipMessageBody::GetBoundary()
{
    return m_tokBoundary;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPMESSAGEBODY_H

