//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_IMSPOFFERANSWERSESSIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IMSPOFFERANSWERSESSIONMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_STATUSCODE_H
#include "SipParser/StatusCode.h" //-- uNOT_ACCEPTABLE_HERE
#endif


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

//==============================================================================
//== Class: IMspOfferAnswerSessionMgr
//========================================
//
// Description:
//  The IMspOfferAnswerSessionMgr interface defines the APIs that allow the
//  IMspOfferAnswerSession to communicate with the entity that manages it.
//
//  The responsibilities of the manager are as follows:
//
//      - Keep a coherent view of the offer/answer model. As an example, the
//        manager must not provide the MSP with an offer if there is already an
//        offer being prepared.
//
//      - Retrieve SDP information from SIP packets.
//
//      - Decrypt SDP information if needed.
//
//      - Verify signature of SDP information if required.
//
//      - Provide raw SDP information to the MSP Offer Answer Session through
//        HandleSdp
//
//      - Send SDP packets to the peer when handling EvNewSdpToSend
//
//      - Manage the SIP negotiation at the SIP Supported and Require headers
//        level.
//
//
// Location:
//   SceMsp/IMspOfferAnswerSessionMgr.h
//
// See Also:
//   IMspOfferAnswerSession
//
//==============================================================================
class IMspOfferAnswerSessionMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    // Summary:
    //  The type of message in which the payload should be sent when calling
    //  EvNewSdpToSend.
    // Description:
    //  This type is used to help the IMspOfferAnswerSessionMgr to choose a
    //  type of packet to use to send the payload in EvNewSdpToSend.
    //  eINVITE_RINGING informs the local user about the incoming session.
    //  Since there are potential pre-conditions to session setup to handle,
    //  the MSP has the responsibility to inform its user when the local user
    //  should be notified of the incoming session. This type is useful only
    //  when performing server session negotiation (when receiving an
    //  INVITE/reINVITE).
    //------------------------------------------------------------------------
    enum EMessageTypeForPayload
    {
        // Summary:
        //  Send the payload in a 183 response to the received INVITE.
        //-------------------------------------------------------------
        eINVITE_SESSION_PROGRESS,

        // Summary:
        //  Buffer the payload for a 180 response to the received INVITE. When
        //  the 180 response is sent, the next sent response must be a final
        //  response.
        //---------------------------------------------------------------------
        eINVITE_RINGING,

        // Summary:
        //  Send the payload in a 200 response to the received INVITE.
        //-------------------------------------------------------------
        eINVITE_OK,

        // Summary:
        //  The packet used to send the payload is chosen automatically.
        //---------------------------------------------------------------
        eAUTOMATIC
    };

    //==========================================================================
    //==
    //==  EvNewSdpToSend
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the manager that there is new SDP information to send.
    //
    //  Parameters:
    //      pSdpSession:
    //       The SDP for the session.
    //
    //      pSdpEarlySession:
    //       The SDP for the early-session.
    //
    //       Can be NULL.
    //
    //       MUST be NULL after EvNewSdpToSend is called with eINVITE_RINGING or
    //       IMspOfferAnswerSession::RemoteUserNotified is reported.
    //
    //      eMessageType:
    //       Informs the manager about the way the payload has to be sent.
    //
    //       Unless there is knowledge that the offer was received in an INVITE
    //       request (eOFFER_LONG is received in
    //       IMspOfferAnswerSession::HandleSdp), an SDP answer should always be
    //       sent using eAUTOMATIC.
    //
    //      uBitSetAdditionalRequiredExtension:
    //       A bitset containing additional extensions to add in a Require
    //       header for this message.
    //
    //       MUST be 0 or a bitset of ESipOptionTag values.
    //
    //  Returns:
    //      - resS_OK:
    //          A SIP message will be sent.
    //      - resSW_WARNING:
    //          EvNewSdpToSend was called to buffer no payload and no additional
    //          required extension. The call to this method has been ignored.
    //      - resFE_INVALID_STATE:
    //          The IMspOfferAnswerSessionMgr is in an incorrect state to send the
    //          SIP message.
    //      - resFE_FAIL:
    //          An error happened when sending the SIP message. Some of the
    //          messages could have been sent.
    //
    //  Description:
    //      This method is called by the MSP Offer Answer Session when SDP
    //      information is ready to be sent on the network. Depending on the
    //      specific offer/answer scenario, the manager must send the SDP
    //      packets as soon as possible.
    //
    //      There MUST be no early-session in all calls to this method after
    //      IMspOfferAnswerSession::RemoteUserAccepted or
    //      IMspOfferAnswerSession::LocalUserAccepted has been called.
    //
    //  #Expected behaviour:#
    //
    //    INVITE processing as a UAS :
    //      - All calls to EvNewSdpToSend with eINVITE_SESSION_PROGRESS, when
    //        done before EvNewSdpToSend is called with eINVITE_RINGING, result
    //        in a 183 being sent.
    //
    //      - For an original INVITE, a call to EvNewSdpToSend with
    //        eINVITE_RINGING, results in the SDP being buffered until the call
    //        has knowledge that the local user has been notified of the call.
    //        When it happens, the payload is sent in a 180. After a 180 is
    //        sent, no provisional response can be sent for the current INVITE.
    //
    //      - It is optional to send a 180 for an INVITE request.
    //
    //      - If EvNewSdpToSend is called for a 180 (for answer preview or
    //        reliable 180) but no 180 is sent by the call, the payload for the
    //        180 is used for the 200.
    //
    //      - For an original INVITE, a call to EvNewSdpToSend with eINVITE_OK
    //        or eAUTOMATIC, results in the SDP being buffered until the call is
    //        accepted by the user. When accepted, the buffered data will be
    //        used to send a 200 OK response to the INVITE.
    //
    //      - For a reINVITE, a call to EvNewSdpToSend with eINVITE_OK or
    //        eAUTOMATIC results in the payload being sent in a 200 to the
    //        reINVITE. Note that for a reINVITE, it is the responsibility of
    //        the IMspOfferAnswerSession to send the 200 response. This means
    //        that if a 200 response was not already sent, EvNewSdpToSend has
    //        to be called to send the 200 even if no SDP has to be sent (it is
    //        called without payload in this case).
    //
    //      - When sending SDP answers in unreliable 1xx, the MSP can send only
    //        ONE SDP answer for the INVITE. If the user performs something
    //        before the call is accepted and the MSP wants to send a new offer,
    //        it MUST wait for the call to be accepted before trying to send the
    //        offer. The MSP is notified of this when LocalUserAccepted is
    //        called. An answer that is contained in an unreliable 1xx response
    //        will also be present in the 2xx response, if sent.
    //
    //      - When using reliable 1xx responses, the MSP can send new offers in
    //        a reliable 1xx when there is no negotiation left for the payload
    //        type. It can do this until the 180 response is sent.
    //
    //      - If an offer has been received in an UPDATE request, the answer
    //        will get sent in the 2xx to the UPDATE request.
    //
    //    INVITE processing as a UAC :
    //      - The call to this method for the
    //        IMspOfferAnswerSession::PrepareOffer call will send the original
    //        INVITE request.
    //
    //      - After the INVITE has been sent, a new offer is sent in an UPDATE
    //        request when there is no active negotiation and the peer supports
    //        UPDATE requests.
    //
    //      - An offer is NEVER sent in a PRACK. This is because a PRACK request
    //        cannot be rejected : "If the PRACK does match an unacknowledged
    //        reliable provisional response, it MUST be responded to with a 2xx
    //        response." (RFC3262 section 3, 11th paragraph).
    //
    //      - If IMspOfferAnswerSession::HandleSdp has been called for
    //        non-preview offers, answers for the offers will be sent in a PRACK
    //        request or in an ACK request, depending if the offers were
    //        received in a reliable 1xx or in a 2xx response. If there was many
    //        offers, all the answers for these must be given in the same
    //        EvNewSdpToSend call.
    //
    //      - If an offer has been received in an UPDATE request, the answer
    //        will get sent in the 2xx to the UPDATE request.
    //
    //    Established call :
    //      - If IMspOfferAnswerSession::HandleSdp was called for a received
    //        UPDATE, a call to this method will send a 200 response.
    //
    //      - If IMspOfferAnswerSession::HandleSdp or
    //        IMspOfferAnswerSession::PrepareOffer was called for a received
    //        reINVITE the remaining will be processed as an INVITE processing
    //        as a UAS.
    //
    //      - If IMspOfferAnswerSession::HandleSdp or
    //        IMspOfferAnswerSession::PrepareOffer was not called, a call to
    //        this method will send an INVITE or UPDATE request to the peer, if
    //        none is currently processing. The request to be sent will get
    //        chosen from the configuration and the peer's supported methods. An
    //        INVITE will be sent if we are configured to send reINVITE requests
    //        or if the remote UA does not support UPDATE requests.
    //
    //      - Note that an empty reINVITE or UPDATE cannot be sent.
    //
    //==========================================================================
    virtual mxt_result EvNewSdpToSend(IN TO CBlob* pSdpSession,
                                      IN TO CBlob* pSdpEarlySession,
                                      IN EMessageTypeForPayload eMessageType,
                                      IN uint32_t uBitSetAdditionalRequiredExtension) = 0;


    enum ENegotiationFailureReason
    {
        // Summary:
        //  The offer was presented as an eOFFER_SHORT but generating the
        //  answer would take too long.
        //----------------------------------------------------------------
        eREQUIRE_LONG_TRANSACTION,

        // Summary:
        //  Syntax error in the SDP session.
        //-----------------------------------
        eSYNTAX_ERROR_SDP_SESSION,

        // Summary:
        //  Syntax error in the early session.
        //-------------------------------------
        eSYNTAX_ERROR_SDP_EARLY_SESSION,

        // Summary:
        //  Incompatible network protocols, most likely in a c= line.
        //--------------------------------------------------------------
        eINCOMPATIBE_NETWORK_PROTOCOL,

        // Summary:
        //  Incompatible address format, most likely in a c= line.
        //-----------------------------------------------------------
        eINCOMPATIBE_ADDRESS_FORMAT,

        // Summary:
        //  Incompatible transport protocol, most likely in a m= line.
        //-------------------------------------------------------------
        eINCOMPATIBE_TRANSPORT_PROTOCOL,

        // Summary:
        //  Unrecognized or imcompatible bandwidth units.
        //------------------------------------------------
        eINCOMPATIBE_BANDWIDTH_UNITS,

        // Summary:
        //  No common media type found.
        //------------------------------
        eINCOMPATIBLE_MEDIA_TYPE,

        // Summary:
        //  No common media format found.
        //--------------------------------
        eINCOMPATIBLE_MEDIA_FORMAT,

        // Summary:
        //  Not enough bandwidth.
        //------------------------
        eINSUFFICIENT_BANDWIDTH,
    };


    //==========================================================================
    //==
    //==  EvNegotiationFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports a negotiation failure when handling a received offer.
    //
    //  Parameters:
    //      eReason:
    //       The reason why there was a failure.
    //
    //      uFailureSipStatusCode:
    //       The status code of the failure response to send. This MUST be a
    //       valid failure SIP response (>= 300 and < 700). The default value is
    //       488. If an invalid value is given, 488 will be used.
    //
    //  Description:
    //      This event can be reported when the MSP Offer Answer Session is
    //      managing an offer from a peer and this offer either:
    //
    //      - Contains syntax error
    //
    //      - Requires an long amount of time to process and the offer was
    //        provided as eOFFER_SHORT offer.
    //
    //      - Too limitative with regards to the peer capabilities. The MSP
    //        Offer Answer Session is not able to find common media/media format
    //        or other SDP parameters are not understood.
    //
    //     This event is only reported after a call to
    //     IMspOfferAnswerSession::HandleSdp that provides an offer with
    //     eOfferType being either eOFFER_SHORT or eOFFER_LONG.
    //
    //==========================================================================
    virtual void EvNegotiationFailure(IN ENegotiationFailureReason eReason,
                                      IN uint16_t uFailureSipStatusCode = uNOT_ACCEPTABLE_HERE) = 0;


    //==========================================================================
    //==
    //==  EvTerminalFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports a terminal error on the MSP.
    //
    //  Description:
    //      This event reports a terminal error on the MSP, either at the
    //      offer/answer level or at any other level (media processing,
    //      resources overload, etc).
    //
    //      This will have the effect of immediately terminating the signaling
    //      session associated with this MSP.
    //
    //      Note that the manager still must call
    //      IMspOfferAnswerSession::TerminateSession before releasing the MSP
    //      object.
    //
    //==========================================================================
    virtual void EvTerminalFailure() = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IMspOfferAnswerSessionMgr(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IMspOfferAnswerSessionMgr(){}


private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IMspOfferAnswerSessionMgr(IN const IMspOfferAnswerSessionMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IMspOfferAnswerSessionMgr& operator=(IN const IMspOfferAnswerSessionMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_IMSPOFFERANSWERSESSIONMGR_H

