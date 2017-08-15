//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation. ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSIPPARSERSVC_H
#define MXG_CSIPPARSERSVC_H

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


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSIPPACKETPARSER_H
#include "SipParser/CSipPacketParser.h"
#endif

#ifndef MXG_CAATREE_H
#include "Cap/CAATree.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSIPTRANSPORTSVC_H
#include "SipTransport/CSipTransportSvc.h"
#endif

#ifndef MXG_CSIPCLIENTSOCKET_H
#include "SipTransport/CSipClientSocket.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CRawHeader;
class CSipHeader;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

typedef void (*PFNTransportPacketInspector)(INOUT CSipPacketParser::SRawData*&
                                                rpsRawData);
typedef void (*PFNTransportPacketModifier)(INOUT CSipPacket& rPacket);

//==SDOC========================================================================
//== Class: CSipParserSvc
//========================================
//
// Description:
//   This class parses the packets received by the sockets using the SIP parser.
//   If the packet is a request and contains errors, then a 404 Bad Request
//   response or another error, if applicable, is built from the packet and sent
//   back to the peer. If the packet is a response and contains errors, it is
//   dropped. Finally, if the packet is correct, it is transferred to the
//   transport manager so that it can show it to the list of transport
//   observers.
//
// Location:
//   SipTransport/CSipParserSvc.h
//
// See Also:
//   CSipTransportSvc
//
//==EDOC========================================================================
class CSipParserSvc : public CSipTransportSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

//-- New types within class namespace
//-----------------------------------
public:
protected:
private:
    enum ePARSERSVC_STATE
    {
        ePARSERSVC_STATE_INIT,
        ePARSERSVC_STATE_HEADERS,
        ePARSERSVC_STATE_PAYLOAD,
        ePARSERSVC_STATE_PAYLOAD_ERROR,
        ePARSERSVC_STATE_FINAL_ERROR,
        ePARSERSVC_STATE_FINAL_SUCCESS,
    };

    struct SStreamReceptionState
    {
        CSipPacket*      m_pCurrentPacket;
        uint32_t         m_uPayloadByteToReceive;
        ePARSERSVC_STATE m_eState;
        uint32_t         m_uReceivedHeadersByte;
    };

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipParserSvc();

    // Summary:
    //  Initializes the connection service as a SipTransport service chain link.
    //--------------------------------------------------------------------------
    mxt_result Initialize(IN CSipTransportSvc* pNextSender,
                          IN CSipTransportSvc* pNextReceiver,
                          IN CSipTransportSvc& rNetworkSideSvc);

    // Summary:
    //  Sets the maximum size of the payload allowed.
    //------------------------------------------------
    static void SetMaxPayloadSize(IN unsigned int uSize);

    // Summary:
    //  Returns the maximum size of the payload allowed.
    //---------------------------------------------------
    static unsigned int GetMaxPayloadSize();

    // Summary:
    //  Sets the maximum receivable size of SIP packet that can be accepted
    //  by the stack.
    //----------------------------------------------------------------------
    static void SetMaxReceivePacketSize(IN unsigned uSize);

    // Summary:
    //  Gets the maximum receivable size of SIP packet that can be accepted
    //  by the stack.
    //----------------------------------------------------------------------
    static unsigned int GetMaxReceivePacketSize();

    // Summary:
    //  Sets the multiple Via headers check in incoming responses destined
    //  to user-agents.
    static mxt_result SetUaResponseMultipleViasCheck(IN bool bCheckMultipleVias,
                                                     IN TO CVector<CString>* pvecstrProxyIds = NULL);

    // Summary:
    //  Allows to ignore the Via sent-by validation in responses.
    static void IgnoreTopViaSentByInReceivedResponses(IN bool bIgnore);

    // Summary:
    //  Verifies that a packet was generated by a local proxy and updates the
    //  packet accordingly.
    static mxt_result UpdatePacketInfoFromViaBranch(IN const CString& rstrProxyId,
                                                    INOUT CSipPacket& rPacket,
                                                    IN bool bFromProxy = true);

    // Summary:
    //  Generates a via key.
    static void GenerateViaKey(IN const CString& rstrProxyId,
                               IN const CSipPacket& rPacket,
                               IN const uint8_t* puViaInfo,
                               IN unsigned int uViaInfoLength,
                               OUT CBlob& rResult);

    // Summary:
    //  Adds addresses in the list of listening addresses.
    void AddListenAddress(IN CString& rstrAddress,
                          IN uint16_t uPort,
                          IN const CVector<CString>* pvecFqdn);

    // Summary:
    //  Removes the addresse from the listening addresses if no longer used.
    void RemoveListenAddress(IN CString& rstrAddress,
                             IN uint16_t uPort,
                             IN const CVector<CString>* pvecFqdn);

    //-- << CSipTransportSvc >>.
    //---------------------------

    // Summary:
    //  Handles incoming packets.
    //----------------------------
    virtual void ReceivedFromNetwork(IN CSipPacket*       pPacket,
                                     IN CSipClientSocket* pSocket);

    // Summary:
    //  Handles outgoing packets.
    //----------------------------
    virtual mxt_result SendToNetwork(IN CSipPacket&        rPacketOut,
                                     IN CSipClientSocket*  pSource,
                                     IN ISipTransportUser* pUser,
                                     IN mxt_opaque         opq);

    //  Summary:
    //    Service handler for reporting network error.
    //-------------------------------------------------
    virtual void SocketError(IN CSipClientSocket& rSocket,
                             IN mxt_result        result);

    //  Summary:
    //    Service handler for reporting closed sockets.
    //--------------------------------------------------
    virtual void SocketClosed(IN CSipClientSocket&                   rSocket,
                              IN ISipTransportObserver::EClosureType eClosureType);

    // Summary:
    //  Releases the resources used by the service.
    //----------------------------------------------
    virtual void ReleaseService();

    // Summary:
    //  Sets the type of the sequence number supported.
    //--------------------------------------------------
    static void SetCSeq64BitsSupport(IN bool bSupported);

    //-- << Application Callbacks >>.
    //--------------------------------

    // Summary:
    //  Sets the packet inspector method (callback).
    //-----------------------------------------------
    static void SetPacketInspectorCB(PFNTransportPacketInspector pfnTransportPacketInspector);

    // Summary:
    //  Sets the packet modifier method (callback).
    //----------------------------------------------
    static void SetPacketModifierCB(PFNTransportPacketModifier pfnTransportPacketModifier);

    // Summary:
    //  Returns true when the socket received a fragmented packet.
    //-------------------------------------------------------------
    static bool IsReceivedPacketFragmented(IN CSipClientSocket& rSocket);

//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Destructor. Must call ReleaseService instead of directly access this
    //  destructor.
    //-----------------------------------------------------------------------
    virtual ~CSipParserSvc();

    // Summary:
    //  Releases any CSipParserSvc specific information.
    //---------------------------------------------------
    void ReleaseSvcInfo(INOUT CSipClientSocket& rSocket);

private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipParserSvc();
    static void FinalizeCSipParserSvc();


    //-- << CSipTransportSvc >>.
    //---------------------------

    // Summary:
    //  Initializes the parser service as a SipTransport service chain link.
    //-----------------------------------------------------------------------
    mxt_result Initialize(IN CSipTransportSvc* pNextSender,
                          IN CSipTransportSvc* pNextReceiver);

    // Summary:
    //  Begins shutdown on the current object.
    //-----------------------------------------
    virtual mxt_result SpecificShutdown(IN ISipTransportUser* pUser,
                                        IN mxt_opaque         opq);


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipParserSvc(IN const CSipParserSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipParserSvc& operator=(IN const CSipParserSvc& rSrc);

    // Summary:
    //  Handles the SIP stream packet reception.
    //-------------------------------------------
    void HandleStreamReception(IN CSipClientSocket& rSocket);

    // Summary:
    //  Handles the SIP datagram packet reception.
    //---------------------------------------------
    void HandleDatagramReception(IN CSipClientSocket& rSocket);

    // Summary:
    //  Generates a final negative response with respect to the supplied
    //  request, result code, and error phrase.
    //-------------------------------------------------------------------
    void CreateFinalNegativeResponse(IN  mxt_result   result,
                                     IN  CSipPacket&  rRequest,
                                     IN  CString&     rStrErrorPhrase,
                                     OUT CSipPacket*& pResponse);

    // Summary:
    //  Extracts the payload of the SIP packet from the buffer.
    //----------------------------------------------------------
    void FetchPayload(IN CSipPacket&  rPacket,
                      IN uint8_t*     puRawPacketEnd,
                      INOUT uint8_t*& rpuRawPacketIndex,
                      INOUT uint32_t& ruContentLeftToFetch);

    // Summary:
    //  Created a new SIP packet.
    //----------------------------
    static CSipPacket* CreateSipPacket(IN CSipClientSocket& rSocket);

    // Summary:
    //  Retrieves a stream reception state structure associated with the
    //  client socket.
    //-------------------------------------------------------------------
    static SStreamReceptionState* GetStreamReceptionState(IN CSipClientSocket& rSocket);

    // Summary:
    //  Resets the stream reception state structure associated with the
    //  client socket.
    //-----------------------------------------------------------------
    static void ResetStreamReceptionState(IN CSipClientSocket& rSocket, INOUT SStreamReceptionState* pstState);

    // Summary:
    //  Handles parsing errors and sends back a response if applicable.
    //------------------------------------------------------------------
    void HandleParsingError(IN CSipPacket&       rPacket,
                            IN CSipClientSocket& rSocket,
                            IN CString&          rStrErrorPhrase,
                            IN mxt_result        result);

    // Summary:
    //   Validates a mandatory header and, if not valid, fills the error phrase
    //   to put in the message body.
    //--------------------------------------------------------------------------
    mxt_result ValidateHeader(IN CSipPacket&    rPacket,
                              IN CHeaderList&   rHeaderList,
                              IN ESipHeaderType eSipHeaderType,
                              OUT CString&      rStrErrorPhrase);

    // Summary:
    //   Validates the mandatory headers and, if any are not valid, sends back
    //   the error phrase to put in the message body.
    //-------------------------------------------------------------------------
    mxt_result ValidateHeaders(IN CSipPacket& rPacket,
                               OUT CString&   rStrErrorPhrase);

    // Summary:
    //   Verifies the presence of the Content-Length header and validates the
    //   content of the buffer against it.
    //------------------------------------------------------------------------
    mxt_result VerifyContentLength(IN CSipPacket&       rPacket,
                                   IN CSipClientSocket& rSocket,
                                   IN uint8_t*          puRawPacketEnd,
                                   INOUT uint8_t*&      rpuRawPacketIndex,
                                   OUT uint32_t&        uContentLength);

    // Summary:
    //   Checks for the "sent-by" parameter of the topmost Via header, and adds
    //   a "received" parameter to the header if needed as per RFC 3261.
    //--------------------------------------------------------------------------
    void VerifySentByParam(IN CSipPacket& rPacket);

    // Summary:
    //  Checks that Via sent by correspond to configured interfaces.
    mxt_result ValidateViaSentBy(IN CSipHeader* pHeader);


//-- Hidden Data Members
//------------------------
protected:
private:
    struct SListenAddress
    {
        SListenAddress(IN const CString* pstrAddress,
                       IN uint16_t uPort)
        :   m_pstrAddress(pstrAddress),
            m_uPort(uPort),
            m_uRefCount(1)
        {};

        bool operator==(const SListenAddress& rElement) const
        {
            // Performs a case insensitive comparison.
            return (rElement.m_pstrAddress->CaseInsCmp(*m_pstrAddress) == 0 && m_uPort == rElement.m_uPort);
        };

        bool operator<(const SListenAddress& rElement) const;

        bool operator!=(const SListenAddress& rElement) const
        {
            return !operator==(rElement);
        };

        // The ownership is kept outside of the struct. Hence, the string must
        // be allocated and delete before and after the struct allocation
        // and destruction.
        const CString* m_pstrAddress;
        uint16_t m_uPort;
        uint8_t m_uRefCount;
    };

    //  The maximum payload size.
    //----------------------------
    static unsigned int ms_uMaxPayloadSize;

    //  The maximum receivable size of SIP packet.
    //---------------------------------------------
    static unsigned int ms_uMaxReceivePacketSize;

    // Check for multiples via headers?
    static bool ms_bCheckMultipleVias;

    // Ignore or validate the Via sent-by.
    static bool ms_bIgnoreViaSentBy;

    // Vector containing the IDs proxies which can generate additional
    // via headers.
    static CVector<CString>* ms_pvecstrProxyIds;

    // Contains the list of local addresses. Used to validate via headers.
    CAATree<SListenAddress> m_localAddresses;

    //-- << Application callbacks >>.
    //--------------------------------

    //  The packet inspector method pointer.
    //---------------------------------------
    static PFNTransportPacketInspector ms_pfnTransportPacketInspector;

    //  The packet modifier method pointer.
    //--------------------------------------
    static PFNTransportPacketModifier ms_pfnTransportPacketModifier;

    // The network side service (the one that interacts directly with the SIP
    // sockets).
    //------------------------------------------------------------------------
    CSipTransportSvc* m_pNetworkSideSvc;

    // A flag to know if 64 bits sequence number is supported.
    //---------------------------------------------------------
    static bool ms_b64bitsSequenceNumberSupported;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  Initialize
//==
//==============================================================================
//
//  Parameters:
//      pNextSender:
//       The next element of the chain.
//
//      pNextReceiver:
//       The previous element of the chain (next in the reverse direction).
//
//  Returns:
//       An error if the initialization is not correctly performed.
//
//  Description:
//       Initializes the parser service as a SipTransport service chain link.
//
//==EDOC========================================================================
inline
mxt_result CSipParserSvc::Initialize(IN CSipTransportSvc* pNextSender,
                                     IN CSipTransportSvc* pNextReceiver)
{
    return CSipTransportSvc::Initialize(pNextSender, pNextReceiver);
}

//==SDOC========================================================================
//==
//==  SetPacketInspectorCB
//==
//==============================================================================
//
//  Parameters:
//      PFNTransportPacketInspector:
//       The packet inspector method (pointer).
//
//  Description:
//      Sets the packet inspector method (callback) for incoming packets.
//
//==EDOC========================================================================
inline void CSipParserSvc::SetPacketInspectorCB(
                   IN PFNTransportPacketInspector pfnTransportPacketInspector)
{
    ms_pfnTransportPacketInspector = pfnTransportPacketInspector;
}

//==SDOC========================================================================
//==
//==  SetPacketModifierCB
//==
//==============================================================================
//
//  Parameters:
//      pfnTransportPacketModifier:
//       The packet modifier method (pointer).
//
//  Description:
//      Sets the packet modifier method (callback) for incoming packets.
//
//==EDOC========================================================================
inline void CSipParserSvc::SetPacketModifierCB(
                   IN PFNTransportPacketModifier pfnTransportPacketModifier)
{
    ms_pfnTransportPacketModifier = pfnTransportPacketModifier;
}

//==SDOC========================================================================
//==
//==  IsReceivedPacketFragmented
//==
//==============================================================================
//
//  Parameters:
//      rSocket:
//       The socket source.
//
//  Description:
//      Returns true when the socket received a fragmented packet.
//
//==EDOC========================================================================
inline bool CSipParserSvc::IsReceivedPacketFragmented(IN CSipClientSocket& rSocket)
{
    return (GetStreamReceptionState(rSocket)->m_eState != CSipParserSvc::ePARSERSVC_STATE_INIT);
}

//==SDOC========================================================================
//==
//==  SetMaxPayloadSize
//==
//==============================================================================
//
//  Parameters:
//      uSize:
//       The maximum payload size.
//
//  Description:
//      Sets the maximum size of the payload allowed.
//
//==EDOC========================================================================
inline
void CSipParserSvc::SetMaxPayloadSize(IN unsigned int uSize)
{
    ms_uMaxPayloadSize = uSize;
}

//==SDOC========================================================================
//==
//==  GetMaxPayloadSize
//==
//==============================================================================
//
//  Returns:
//       The maximum payload size.
//
//  Description:
//      Gets the maximum size of the payload allowed.
//
//==EDOC========================================================================
inline unsigned int CSipParserSvc::GetMaxPayloadSize()
{
    return ms_uMaxPayloadSize;
}

//==SDOC========================================================================
//==
//==  SetMaxReceivePacketSize
//==
//==============================================================================
//
//  Parameters:
//      uSize:
//       The size in bytes of the receiving packet.
//
//  Description:
//      Sets the maximum receivable size of SIP packets that can be accepted by
//      the stack.
//
//==EDOC========================================================================
inline
void CSipParserSvc::SetMaxReceivePacketSize(IN unsigned uSize)
{
    ms_uMaxReceivePacketSize = uSize;
}

//==SDOC========================================================================
//==
//==  GetMaxReceivePacketSize
//==
//==============================================================================
//
//  Returns:
//      The size in bytes of the receiving packet.
//
//  Description:
//      Gets the maximum receivable size of SIP packets that can be accepted by
//      the stack.
//
//==EDOC========================================================================
inline unsigned int CSipParserSvc::GetMaxReceivePacketSize()
{
    return ms_uMaxReceivePacketSize;
}

//==SDOC========================================================================
//==
//==  SetCSeq64BitsSupport
//==
//==============================================================================
//
//  Parameters:
//    bSupported:
//      True when we want a 64bits sequence number supported. False for a
//      32 bits.
//
//  Description:
//      Sets the type of the sequence number supported.
//
//==EDOC========================================================================
inline void CSipParserSvc::SetCSeq64BitsSupport(IN bool bSupported)
{
    ms_b64bitsSequenceNumberSupported = bSupported;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSipParserSvc_H
