//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_CSIPPACKET_H
#define MXG_CSIPPACKET_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


//-- Data Members
//-----------------
#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif

#ifndef MXG_CTOKEN_H
#include "SipParser/CToken.h"
#endif

#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" //-- ESipTransport
#endif

#ifndef MXG_CATOMIC_H
#include "Kernel/CAtomic.h"
#endif
//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSIPPACKETPARSER_H
#include "SipParser/CSipPacketParser.h"
#endif

#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPPACKET_INSTANCE_TRACKING
    #ifndef MXG_CWATCHEDINSTANCE_H
    #include "SipTransport/CWatchedInstance.h"
    #endif
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipMessageBody;
class CString;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
const uint32_t gs_SIP_PACKET_INVALID_DSCP_VALUE = 0xFFFFFFFF;
// BRCM_CUSTOM - [add] Add per-user DNS lookup support / Add CCTK GUID opaque
// Note: This constant must be equal to CCTK_INVALID_HANDLE in cctkApi.h.
const unsigned int uINVALID_CCTK_GUID = 0xCAFEBEEF;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support / Add CCTK GUID opaque

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipPacket
//========================================
// <GROUP SIPTRANSPORT_CLASSES>
//
//
// Description:
//   This class is used as a container for a packet sent and received. It is
//   derived from the CSipPacketParser class, which is used to parse a raw
//   header, or on the contrary, to serialize a series of headers and a payload.
//   The present class also adds other useful data such as the local and remote
//   IP addresses and ports and the transport used.
//
//   The SIP stack follows a mechanism to process incoming and outgoing packets.
//   When a packet is received from the network, only one owner can access it.
//   Once the SipParser determines that it has received the whole packet, it
//   becomes a const instance and it can be shared and reference counted
//   throughout the SIP stack and possibly the application. The same mechanism
//   is also true when sending a packet. There is only one owner that can access
//   the packet until it is complete and ready to be sent. Once every services
//   have updated the packet, it becomes const and can be shared throughout
//   several threads.
//
//   Constant methods that can modify the packet headers are synchronized
//   internally so different threads accessing them do not need to bother with
//   synchronization.
//
//   The following is the general SIP stack usage of SIP packets:
//
//   - SIP packets are never shared or reference counted until they are
//   complete.
//   - Only a complete SIP packet can be shared or reference counted.
//   - When shared, a SIP packet is always const.
//
// Location:
//   SipTransport/CSipPacket.h
//
//==EDOC========================================================================
class CSipPacket : public CSipPacketParser
#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPPACKET_INSTANCE_TRACKING
                   , public CWatchedInstance<CSipPacket>
#endif
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
#if defined(MXD_SIPSTACK_ENABLE_SIPPACKET_INSTANCE_TRACKING) || \
    defined(MXD_SIPSTACK_ENABLE_SIPREQUEST_CONTEXT_INSTANCE_TRACKING)
    friend class CInstanceTracker;      // To access the Dump method.
#endif

#if defined(MXD_SIPSTACK_ENABLE_DEPRECATED_SIPPACKET_INSTANCE_TRACKING) || \
    defined(MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING)
    friend class CDeprecatedInstanceTracker; // To access the Dump method.
    friend class CSipRequestContext; // To access the DumpInfo method.
#endif
    //M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipPacket();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipPacket(IN const CSipPacket& rSrc);

    // Summary:
    //  Constructor for outgoing requests.
    //-------------------------------------
    CSipPacket(IN const CString& rstrMethod,
               IN TO CHeaderList* pExtraHeaders = NULL,
               IN TO CSipMessageBody* pMessageBody = NULL);

    // Description:
    //  Indicate whetever or not the Record route must be copied.
    enum ERecordRouteProcessing
    {
        // Use this value when the response can establish a dialog. Note that
        // this value should be used even when responding to a request that is
        // inside a dialog when this request could establish a dialog (eg
        // re-INVITE). It would enable a re-INVITE to re-establish a dialog if
        // dialog state was lost on the server side.
        //---------------------------------------------------------------------
        eCopyRecordRoute,
        // Use this value when the response cannot establish a dialog. Eg:
        // OPTIONS, REGISTER, BYE.
        //-----------------------------------------------------------------
        eDoNotCopyRecordRoute
    };

    // Summary:
    //  Constructor for outgoing responses.
    //--------------------------------------
    CSipPacket(IN const CSipPacket& rRequest,
               IN unsigned int uCode,
               IN const char* szReason,
               IN ERecordRouteProcessing eRrProcessing,
               IN TO CHeaderList* pExtraHeaders = NULL,
               IN TO CSipMessageBody* pMessageBody = NULL,
               IN bool bSetViaRawHeaderToNull = true);

    // Summary:
    //  Operator.
    //------------
    CSipPacket& operator=(IN const CSipPacket& rSrc);

    //-- << Public Methods >>
    //-------------------------

    // Summary:
    //  Sets whether or not to automatically put the User-Agent header in
    //  requests.
    //--------------------------------------------------------------------
    static void AddUserAgentToRequests(IN bool bAdd);

// BRCM_CUSTOM [add] Add User-Agent header to responses
    static void AddUserAgentToResponses(IN bool bAdd);
// BRCM_CUSTOM [end] Add User-Agent header to responses

    // Summary:
    //  Sets whether or not to automatically put the Server header in
    //  responses.
    //----------------------------------------------------------------
    static void AddServerToResponses(IN bool bAdd);

    // Summary:
    //  Sets the identity of this entity to put in the User-Agent and the
    //  Server headers when generating requests and responses respectively.
    //----------------------------------------------------------------------
    static void SetEntityId(IN const CString& rstrUserAgentId,
                            IN const CString& rstrServerId);

    // Summary:
    //  Returns the local IP and port address.
    //-----------------------------------------
    const CSocketAddr& GetLocalAddr() const;

    // <COMBINE CSipPacket::GetLocalAddr@const>
    CSocketAddr& GetLocalAddr();

    // Summary:
    //  Sets the local IP and port address.
    //--------------------------------------
    void SetLocalAddr(IN const CSocketAddr& rAddr);

    // Summary:
    //  Returns the peer IP and port address.
    //----------------------------------------
    const CSocketAddr& GetPeerAddr() const;

    // Summary:
    //  Sets the peer IP and port address.
    //-------------------------------------
    void SetPeerAddr(IN const CSocketAddr& rAddr);

    // Summary:
    //  Returns the transport protocol ID.
    //-------------------------------------
    ESipTransport GetTransport() const;

    // Summary:
    //  Sets the transport protocol ID.
    //----------------------------------
    void SetTransport(IN ESipTransport transport);

    // Summary:
    //  Gets the next-hop URI.
    //-------------------------
    const CSipUri& GetNextHopUri() const;

    // Summary:
    //  Sets the next-hop URI.
    //-------------------------
    void SetNextHopUri(IN const CSipUri& rNextHopUri);

    // Summary:
    //  Gets the next-hop URI hostname.
    //----------------------------------
    const CString& GetNextHopUriHostname() const;

    // Summary
    //  Set the TLS authentication.
    //------------------------------
    void SetReceivedOnAuthenticatedConnection(IN bool bPeerAuth);

    // Summary:
    //  Tells if the TLS connection associated with this packet authenticated
    //  the remote peer.
    //------------------------------------------------------------------------
    bool IsReceivedOnAuthenticatedConnection() const;

    // Summary:
    //  True if a new connection is allowed, false otherwise.
    //--------------------------------------------------------
    bool IsNewConnectionAllowed() const;

    // Summary:
    //  Sets whether or not a new connection is allowed.
    //---------------------------------------------------
    void SetAllowNewConnection(IN bool bAllow);

    // Summary:
    //  Sets the maximum size of the serialized packet.
    //---------------------------------------------------
    void SetMaxSize(IN unsigned int uMaxSize);

    // Summary:
    //  Gets the maximum size of the serialized packet.
    //---------------------------------------------------
    unsigned int GetMaxSize() const;

    // Summary:
    //  Adds a reference to the object.
    //----------------------------------
    unsigned int AddRef() const;

    // Summary:
    //  Releases a reference to the object.
    //--------------------------------------
    unsigned int Release() const;

    // Summary:
    //  Sets the value to put in the Max-Forwards header to put in the requests
    //  created by this class.
    //--------------------------------------------------------------------------
    static void SetMaxForwards(IN unsigned int uMaxForwards);

    // Summary:
    //  Tells if the packet has been handled by the statistics container.
    //--------------------------------------------------------------------
    bool IsStatHandledForReception() const;

    // Summary:
    //  Sets the value to tell if the packet has been handled in the statistics
    //  container.
    //--------------------------------------------------------------------------
    void SetStatHandledForReception(IN bool bHandled) const;

    // Summary:
    //  Gets the destination ID.
    unsigned int GetDestinationId() const;

    // Summary:
    //  Sets the destination ID.
    void SetDestinationId(IN unsigned int uDestinationId);

    // Summary:
    //  Gets the DSCP value.
    uint32_t GetDscp() const;

    // Summary:
    //  Sets the DSCP value.
    void SetDscp(IN uint32_t uDscp);

// BRCM_CUSTOM [add] Add CCTK GUID opaque
    void SetGuidOpaque(IN mxt_opaque opq);

    mxt_opaque GetGuidOpaque() const;
// BRCM_CUSTOM [end] Add CCTK GUID opaque
// BRCM_CUSTOM [add] Add TOS config support
    void SetTosOpaque(IN mxt_opaque opq);

    mxt_opaque GetTosOpaque() const;
// BRCM_CUSTOM [end] Add TOS config support
// BRCM_CUSTOM [add] Add Timer F expiration handling support per IMS
    void SetSendOption(IN bool bToNetwork);

    bool GetSendOption() const;
// BRCM_CUSTOM [end] Add Timer F expiration handling support per IMS

// BRCM_CUSTOM - [add] Configure P-Access-Network-Info header
    static void SetPaniHdr( IN const CString& rstrPaniHdr );
// BRCM_CUSTOM - [end] Configure P-Access-Network-Info header

// BRCM_CUSTOM [mod] Moved to public for the failover change
    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipPacket();

//-- Hidden Methods
//-------------------
protected:
// BRCM_CUSTOM [end] Moved to public for the failover change

    // Summary:
    //  Inserts a numeric To tag if none is present.
    //-----------------------------------------------
    void InsertToTag();

    // Summary:
    //  Hashes the From, To, CSeq, Call-ID, and top Via branch param found in
    //  this packet.
    //------------------------------------------------------------------------
    unsigned int Hash() const;

#if defined(MXD_SIPSTACK_ENABLE_SIPPACKET_INSTANCE_TRACKING) || \
    defined(MXD_SIPSTACK_ENABLE_SIPREQUEST_CONTEXT_INSTANCE_TRACKING)
    // Summary:
    //  Gets a CBlob containing information about this CSipPacket.
    //-------------------------------------------------------------
    void DumpInfo(OUT CBlob& rblobPacketDump) const;
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIPPACKET_INSTANCE_TRACKING) ||
       //     defined(MXD_SIPSTACK_ENABLE_SIPREQUEST_CONTEXT_INSTANCE_TRACKING)
private:

    //-- << CWatchedInstance Mechanism >>
    //--------------------------------------

#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPPACKET_INSTANCE_TRACKING
    // Summary:
    //  Gets a CBlob containing information about all the CSipPacket still
    //  allocated.
    static void Dump(OUT CBlob& rblobPacketDump);
#endif // #if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPPACKET_INSTANCE_TRACKING

#if defined(MXD_SIPSTACK_ENABLE_DEPRECATED_SIPPACKET_INSTANCE_TRACKING) || \
    defined(MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING)
    // Summary:
    //  Gets a CBlob containing information about this CSipPacket.
    void DeprecatedDumpInfo(OUT CBlob& rblobPacketDump) const;
#endif // #if defined(MXD_SIPSTACK_ENABLE_DEPRECATED_SIPPACKET_INSTANCE_TRACKING) ||
       //     defined(MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING)

    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipPacket();

    static void FinalizeCSipPacket();


//-- Hidden Data Members
//------------------------
protected:
    // Synchronization of the entity id.
    //-----------------------------------
    static CMutex* ms_pmutexEntityId;

    // Bitset indicating which header to automatically put in request and
    // responses generated.
    //--------------------------------------------------------------------
    static uint8_t ms_bsHeaders;

    // The string to put in User-Agentheader of packets produced by this entity.
    //--------------------------------------------------------------------------
    static CToken* ms_pUserAgentId;

    // The string to put in User-Agentheader of packets produced by this entity.
    //--------------------------------------------------------------------------
    static CToken* ms_pServerId;

    // The local IP and port address.
    //---------------------------------
    CSocketAddr m_localAddr;

    // The peer IP and port address.
    //--------------------------------
    CSocketAddr m_peerAddr;

    // The transport protocol ID.
    //----------------------------
    ESipTransport m_eTransport;

    // The Next-hop Uri.
    //-------------------
    CSipUri m_nextHopUri;

#ifdef MXD_SIPSTACK_ENABLE_TLS
    // Tells if this packet was received over an authenticated TLS connection.
    //-------------------------------------------------------------------------
    bool m_bTlsPeerAuthentication;
#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS

    // Whether a new connection is allowed or not.
    //---------------------------------------------
    bool m_bAllowNewConnection;

// BRCM_CUSTOM [add] Add CCTK GUID opaque
    // This opaque stores the GUID of a CCTK resource.
    //-------------------------------------------------
    mxt_opaque m_opqGuid;
// BRCM_CUSTOM [end] Add CCTK GUID opaque
// BRCM_CUSTOM [add] Add TOS config support
    // This opaque stores the TOS value associated to this packet.
    //-------------------------------------------------------------
    mxt_opaque m_opqTos;
// BRCM_CUSTOM [end] Add TOS config support
// BRCM_CUSTOM [add] Add Timer F expiration handling support per IMS
    // Tells if the packet is supposed to be sent over the network.
    //--------------------------------------------------------------
    bool m_bToNetwork;
// BRCM_CUSTOM [end] Add Timer F expiration handling support per IMS

    // The maximum allowed size of the serialized packet.
    //-----------------------------------------------------
    unsigned int m_uMaxSize;

    // Tells if the packet has been handled by the statistics container.
    //-------------------------------------------------------------------
    mutable bool m_bStatHandledForReception;

    // The number of references to the packet. When this reaches 0, the object
    // deletes itself.
    //-------------------------------------------------------------------------
    mutable CAtomicValue<uint32_t> m_uRefCount;

    // The number to put in the Max-Forwards header of requests created.
    //-------------------------------------------------------------------
    static uint32_t ms_uMaxForwards;

    // Used be the connection service to send the packet on certain socket only.
    unsigned int m_uDestinationId;

    // The DSCP value to set on the socket before sending this packet.
    uint32_t m_uDscp;

// BRCM_CUSTOM - [add] Configure P-Access-Network-Info header
    // The string to put in P-Access-Network-Info header of packets produced by 
    // this entity.
    //--------------------------------------------------------------------------
    static CToken* ms_pPaniHdr;
// BRCM_CUSTOM - [end] Configure P-Access-Network-Info header
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetLocalAddr
//==
//==============================================================================
//
//  Returns:
//      The local address.
//
//  Description:
//      Returns the local address.
//==EDOC========================================================================
inline const CSocketAddr& CSipPacket::GetLocalAddr() const
{
    return m_localAddr;
}

//==SDOC========================================================================
//==
//==  GetLocalAddr
//==
//==============================================================================
//
//  Returns:
//      The local address.
//
//  Description:
//      Returns the local address.
//
//==EDOC========================================================================
inline CSocketAddr& CSipPacket::GetLocalAddr()
{
    return m_localAddr;
}

//==SDOC========================================================================
//==
//==  SetLocalAddr
//==
//==============================================================================
//
//  Parameters:
//    rAddr:
//      The local address to be stored.
//
//  Description:
//      Sets the local address.
//==EDOC========================================================================
inline void CSipPacket::SetLocalAddr(IN const CSocketAddr& rAddr)
{
    m_localAddr.SetAddress(rAddr);
}

//==SDOC========================================================================
//==
//==  GetPeerAddr
//==
//==============================================================================
//
//  Returns:
//      The peer address.
//
//  Description:
//      Returns the peer address.
//==EDOC========================================================================
inline const CSocketAddr& CSipPacket::GetPeerAddr() const
{
    return m_peerAddr;
}

//==SDOC========================================================================
//==
//==  SetPeerAddr
//==
//==============================================================================
//
//  Parameters:
//    rAddr:
//      The peer address to be stored.
//
//  Description:
//      Sets the peer address.
//==EDOC========================================================================
inline void CSipPacket::SetPeerAddr(IN const CSocketAddr& rAddr)
{
    m_peerAddr.SetAddress(rAddr);
}

//==SDOC========================================================================
//==
//==  GetTransport
//==
//==============================================================================
//
//  Returns:
//      The transport protocol ID.
//
//  Description:
//      Returns the transport protocol ID.
//==EDOC========================================================================
inline ESipTransport CSipPacket::GetTransport() const
{
    return m_eTransport;
}

//==SDOC========================================================================
//==
//==  SetTransport
//==
//==============================================================================
//
//  Parameters:
//    transport:
//      The transport protocol ID to be stored.
//
//  Description:
//      Sets the transport protocol ID.
//==EDOC========================================================================
inline void CSipPacket::SetTransport(IN ESipTransport eTransport)
{
    m_eTransport = eTransport;
}

//==SDOC========================================================================
//==
//==  GetNextHopUri
//==
//==============================================================================
//
//  Returns:
//      The next-hop URI.
//
//  Description:
//      Gets the next-hop URI.
//
//  See Also:
//      SetNextHopUri
//
//==EDOC========================================================================
inline const CSipUri& CSipPacket::GetNextHopUri() const
{
    return m_nextHopUri;
}

//==SDOC========================================================================
//==
//==  SetNextHopUri
//==
//==============================================================================
//
//  Parameters:
//    rNextHopUri:
//      The next-hop URI.
//
//  Description:
//      Sets the next-hop URI.
//
//  See Also:
//      GetNextHopUri
//
//==EDOC========================================================================
inline void CSipPacket::SetNextHopUri(IN const CSipUri& rNextHopUri)
{
    m_nextHopUri = rNextHopUri;
}

//==SDOC========================================================================
//==
//==  GetNextHopUriHostname
//==
//==============================================================================
//
//  Returns:
//      The hostname.
//
//  Description:
//      Gets the next-hop URI hostname.
//
//  See Also:
//      GetNextHopUri
//
//==EDOC========================================================================
inline const CString& CSipPacket::GetNextHopUriHostname() const
{
    return m_nextHopUri.GetHostPort().GetHost();
}

//==SDOC========================================================================
//==
//==  SetReceivedOnAuthenticatedConnection
//==
//==============================================================================
//
//  Parameters:
//    bAuth:
//      Boolean telling whether or not TLS authentication has been used.
//
//  Description:
//      Sets the TLS peer authentication. True if TLS authentication has been
//      used on the socket that received this packet, otherwise false.
//
//  See Also:
//      IsReceivedOnAuthenticatedConnection
//
//==EDOC========================================================================
inline void CSipPacket::SetReceivedOnAuthenticatedConnection(IN bool bPeerAuth)
{
#ifdef MXD_SIPSTACK_ENABLE_TLS
    m_bTlsPeerAuthentication = bPeerAuth;
#else // #ifdef MXD_SIPSTACK_ENABLE_TLS
    bPeerAuth = bPeerAuth; // Hide compiler warning.
#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS
}

//==SDOC========================================================================
//==
//==  IsReceivedOnAuthenticatedConnection
//==
//==============================================================================
//
//  Returns:
//      The TLS authentication value.
//
//  Description:
//      Tells whether or not the remote peer has been authenticated when
//      creating the TLS connection used to receive this packet.
//
//  See Also:
//      SetReceivedOnAuthenticatedConnection
//
//==EDOC========================================================================
inline bool CSipPacket::IsReceivedOnAuthenticatedConnection() const
{
#ifdef MXD_SIPSTACK_ENABLE_TLS
    return m_bTlsPeerAuthentication;
#else // #ifdef MXD_SIPSTACK_ENABLE_TLS
    return false;
#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS
}

//==SDOC========================================================================
//==
//==  IsNewConnectionAllowed
//==
//==============================================================================
//
//  Returns:
//      True if new connection allowed, false otherwise.
//
//  Description:
//      Returns whether or not a new connection is allowed.
//==EDOC========================================================================
inline bool CSipPacket::IsNewConnectionAllowed() const
{
    return m_bAllowNewConnection;
}

//==SDOC========================================================================
//==
//==  SetAllowNewConnection
//==
//==============================================================================
//
//  Parameters:
//    bAllow:
//      The value to be stored.
//
//  Description:
//      Sets whether or not a new connection is allowed.
//==EDOC========================================================================
inline void CSipPacket::SetAllowNewConnection(IN bool bAllow)
{
    m_bAllowNewConnection = bAllow;
}

//==SDOC========================================================================
//==
//==  SetMaxSize
//==
//==============================================================================
//
//  Parameters:
//    uMaxSize:
//      The value to be stored.
//
//  Description:
//      Sets the maximum serialzed size of the packet.
//      Note that this maximum size is not enforced by this class and should
//      instead be enforced by users.
//==EDOC========================================================================
inline void CSipPacket::SetMaxSize(IN unsigned int uMaxSize)
{
    m_uMaxSize = uMaxSize;
}

//==SDOC========================================================================
//==
//==  GetMaxSize
//==
//==============================================================================
//
//  Returns:
//      The maximum size of the packet.
//
//  Description:
//      Gets the maximum serialzed size of the packet.
//      Note that this maximum size is not enforced by this class and should
//      instead be enforced by users.
//      By default, the maximum size of a packet is very large (i.e. infinite
//      for all practical purposes).
//==EDOC========================================================================
inline unsigned int CSipPacket::GetMaxSize() const
{
    return m_uMaxSize;
}


//==SDOC========================================================================
//==
//==  IsStatHandledForReception
//==
//==============================================================================
//
//  Returns:
//      True if the packet has been handled by the statistics container.
//
//  Description:
//      Tells whether or not the packet has been handled by the statistics
//      container.
//
//==EDOC========================================================================
inline bool CSipPacket::IsStatHandledForReception() const
{
    return m_bStatHandledForReception;
}


//==SDOC========================================================================
//==
//==  SetStatHandledForReception
//==
//==============================================================================
//
//  Parameters:
//    bHandled:
//      True if the packet has been handled by the statistics container.
//
//  Description:
//      Sets whether or not the packet has been handled by the statistics
//      container.
//
//==EDOC========================================================================
inline void CSipPacket::SetStatHandledForReception(IN bool bHandled) const
{
    m_bStatHandledForReception = bHandled;
}

//==============================================================================
//==
//==  GetDestinationId
//==
//==============================================================================
//
//  Summary:
//      Gets the destination ID.
//
//  Returns:
//      The destination ID.
//
//  Description:
//      Returns the destination ID of the packet.
//
//==============================================================================
inline unsigned int CSipPacket::GetDestinationId() const
{
    return m_uDestinationId;
}

//==============================================================================
//==
//==  SetDestinationId
//==
//==============================================================================
//
//  Summary:
//      Sets the destination ID.
//
//  Parameters:
//    uDestinationId:
//      The destination ID.
//
//  Description:
//      Sets the destination ID of the packet.
//
//==============================================================================
inline void CSipPacket::SetDestinationId(IN unsigned int uDestinationId)
{
    m_uDestinationId = uDestinationId;
}

//==============================================================================
//==
//==  GetDscp
//==
//==============================================================================
//
//  Summary:
//      Gets the DSCP value.
//
//  Returns:
//      The DSCP value.
//
//  Description:
//      Returns the DSCP value to use when sending this packet.
//
//==============================================================================
inline uint32_t CSipPacket::GetDscp() const
{
    return m_uDscp;
}

//==============================================================================
//==
//==  SetDscp
//==
//==============================================================================
//
//  Summary:
//      Sets the DSCP value.
//
//  Parameters:
//    uDscp:
//      The DSCP value.
//
//  Description:
//      Sets the DSCP value to set on the socket before sending the packet.
//
//==============================================================================
inline void CSipPacket::SetDscp(IN uint32_t uDscp)
{
    m_uDscp = uDscp;
}

// BRCM_CUSTOM [add] Add CCTK GUID opaque
//==============================================================================
//==
//==  SetGuidOpaque
//==
//==============================================================================
//
//  Parameters:
//    opq:
//      CCTK GUID value associated to the packet.
//
//  Description:
//      Sets the CCTK GUID value associated to the packet.
//
//===============================================================================
inline void CSipPacket::SetGuidOpaque(IN mxt_opaque opq)
{
    m_opqGuid = opq;
}

//==============================================================================
//==
//==  GetGuidOpaque
//==
//==============================================================================
//
//  Returns:
//      CCTK GUID value of the packet.
//
//  Description:
//      Retrieves the CCTK GUID value associated to the packet.
//
//==============================================================================
inline mxt_opaque CSipPacket::GetGuidOpaque() const
{
    return m_opqGuid;
}
// BRCM_CUSTOM [end] Add CCTK GUID opaque

// BRCM_CUSTOM [add] Add TOS config support
//==============================================================================
//==
//==  SetTosOpaque
//==
//==============================================================================
//
//  Parameters:
//    opq:
//      TOS value associated to the packet.
//
//  Description:
//      Sets the TOS value associated to the packet.
//
//==============================================================================
inline void CSipPacket::SetTosOpaque(IN mxt_opaque opq)
{
    m_opqTos = opq;
}

//==============================================================================
//==
//==  GetTosOpaque
//==
//==============================================================================
//
//  Returns:
//      TOS value of the packet.
//
//  Description:
//      Retrieves the TOS value associated to the packet.
//
//==============================================================================
inline mxt_opaque CSipPacket::GetTosOpaque() const
{
    return m_opqTos;
}
// BRCM_CUSTOM [end] Add TOS config support

// BRCM_CUSTOM [add] Add Timer F expiration handling support per IMS
//==SDOC========================================================================
//==
//==  SetSendOption
//==
//==============================================================================
//
//  Parameters:
//    bToNetwork:
//      True to specify this packet is supposed to be send over the network.
//
//  Description:
//      Sets the send option value associated to the packet.
//
//==EDOC========================================================================
inline void CSipPacket::SetSendOption(IN bool bToNetwork)
{
    m_bToNetwork = bToNetwork;
}

//==SDOC========================================================================
//==
//==  GetSendOption
//==
//==============================================================================
//
//  Returns:
//      True if the packet is supposed to be send over the network.
//
//  Description:
//      Retrieves the send option value associated to the packet.
//
//==EDOC========================================================================
inline bool CSipPacket::GetSendOption() const
{
    return m_bToNetwork;
}
// BRCM_CUSTOM [end] Add Timer F expiration handling support per IMS

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPPACKET_H

