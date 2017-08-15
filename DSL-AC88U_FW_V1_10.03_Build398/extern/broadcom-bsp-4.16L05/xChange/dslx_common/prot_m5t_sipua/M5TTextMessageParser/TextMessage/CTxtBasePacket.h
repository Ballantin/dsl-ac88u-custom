//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CTXTBASEPACKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTXTBASEPACKET_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#ifndef MXG_TEXTMESSAGECFG_H
#include "Config/TextMessageCfg.h"
#endif

#ifndef MXG_CTXTHEADERLIST_H
#include "TextMessage/CTxtHeaderList.h"
#endif

#if defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT) && \
    defined(MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT)
    #ifndef MXG_CSUBALLOCATOR_H
    #include "Cap/CSubAllocator.h"
    #endif
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBlob;
class CSocketAddr;
class CString;
class CSubAllocator;
class CTxtHeader;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CTxtBasePacket
//========================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 10>
//
// Summary:
//  This class handles parsing and storage of a basic packet structure for
//  text messages.
//
// Description:
//  This class handles parsing and storage of a basic packet structure for
//  text messages.
//
//  This class is meant to be the base class of more protocol specific classes.
//  For example, one shall implement a CSipPacket inheriting from this class.
//
//  This class also is the starting point to create other parser classes.
//
// Location:
//   TextMessage/CTxtBasePacket.h
//
//==============================================================================
class CTxtBasePacket
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors >>

    // Summary:
    //  Default Constructor.
    CTxtBasePacket(IN bool bUseSubAllocation = false);

    // Summary:
    //  Copy Constructor.
    CTxtBasePacket(IN const CTxtBasePacket& rSrc);

    //-- << Destructor >>

    // Summary:
    //  Destructor.
    virtual ~CTxtBasePacket();

    //-- << Operators >>

    // Summary:
    //  Assignment Operator.
    CTxtBasePacket& operator=(IN const CTxtBasePacket& rSrc);

    // Summary:
    //  Equality Operator.
    bool operator==(IN const CTxtBasePacket& rSrc) const;

    // Summary:
    //  Inequality Operator.
    bool operator!=(IN const CTxtBasePacket& rSrc) const;

    //-- << Accessors >>

    // Summary:
    //  Gets the type of packet as a string.
    virtual const char* GetPacketType() const;

    // Summary:
    //  Returns the peer address to which the packet will be sent or from which
    //  the packet has been received. May not apply to all child packet types.
    CSocketAddr& GetPeerAddress();

    //<COMBINE CTxtBasePacket::GetPeerAddress>
    const CSocketAddr& GetPeerAddress() const;

    // Summary:
    //  Returns the local address from which the packet will be sent or on which
    //  the packet has been received.
    CSocketAddr& GetLocalAddress();

    //<COMBINE CTxtBasePacket::GetLocalAddress>
    const CSocketAddr& GetLocalAddress() const;

    // Summary:
    //  Gets the header list.
    CTxtHeaderList& GetHeaderList();

    //<COMBINE CTxtBasePacket::GetHeaderList>
    const CTxtHeaderList& GetHeaderList() const;

    // Summary:
    //  Gets a specific header.
    CTxtHeader* GetHeader(IN const char* pszName);

    //<COMBINE CTxtBasePacket::GetHeader@IN const char*>
    const CTxtHeader* GetHeader(IN const char* pszName) const;

    //-- << Modifiers >>

    // Summary:
    //  Sets the peer address to which the packet will be sent or from which
    //  the packet has been received. May not apply to all child packet types.
    void SetPeerAddress(IN CSocketAddr& rPeerAddress);

    // Summary:
    //  Sets the local address from which the packet will be sent or on which
    //  the packet has been received. May not apply to all packet types.
    void SetLocalAddress(IN CSocketAddr& rLocalAddress);

    // Summary:
    //  Appends a non-MIME header to the packet.
    CTxtHeader* AddHeader(IN const char* pszHeaderName,
                          IN CTxtHeaderList::EInsertPosition ePosition);

    // Summary:
    //  Resets the packet's content.
    virtual void Reset();

    //-- << Parsing >>

    // Summary:
    //  Builds a packet from the data store in the rpszPacketText parameter.
    virtual mxt_result Parse(INOUT const char*& rpszPacketText);

    //-- << Serialization >>

    // Summary:
    //  Outputs the packet's content into the provided blob.
    virtual void Serialize(INOUT CBlob& rBlob);

//M5T_INTERNAL_USE_BEGIN
    //-- << Helpers >>

#if defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT) && \
    defined(MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT)
    // Summary:
    //  Retrieves the statistics of the subAllocator.
    void DumpAllocatorStatistics(OUT CString& rstrOutput);
#endif
//M5T_INTERNAL_USE_END

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:

    //-- << Parser Factory >>

    // Summary:
    //  Creates a new CTxtHeaderList object.
    virtual void CreateHeaderList(OUT GO CTxtHeaderList*& rpHeaderList) const;

    // Summary:
    //  Releases a CTxtHeaderList object allocated with CreateHeaderList.
    void ReleaseHeaderList(IN TOA CTxtHeaderList* pHeaderList);

    //-- << Helpers >>

    // Summary:
    //  Allocates a CSocketAddr object using the allocator or not depending on
    //  m_bUseSubAllocator.
    GO CSocketAddr* AllocateSocketAddr() const;

    // Summary:
    //  Releases a CSocketAddr object allocated with AllocateSocketAddr.
    void ReleaseSocketAddr(IN TOA CSocketAddr* pSocketAddr) const;

    // Summary:
    //  Allocates uSize bytes of memory using the allocator or not depending on
    //  m_bUseSubAllocator.
    GO uint8_t* AllocateMemory(IN unsigned int uSize) const;

    // Summary:
    //  Releases a memory block allocated with AllocateMemory.
    void ReleaseMemory(IN TOA uint8_t* pBuffer) const;

private:

    //-- << Deactivated Constructors / Destructors / Operators >>

//-- Hidden Data Members
protected:

    // Indicates if the SubAllocator is to be used.
    bool m_bUseSubAllocator;

    // The suballocator. It is protected so child classes can access it.
    CSubAllocator* m_pAllocator;

private:

    // The local address of the packet.
    CSocketAddr* m_pLocalAddress;

    // The peer address of the packet.
    CSocketAddr* m_pRemoteAddress;

    // The HeaderList.
    CTxtHeaderList* m_pHeaderList;

    // The packet type descriptor for the base packet.
    static const char* const ms_pszPACKET_TYPE;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  OPERATORS  =============================================================
//==============================================================================

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          The Base packet to which this instance is compared.
//
//  Returns:
//      - true: otherwise.
//      - false: when both packets are equivalent.
//
//  Description:
//      Inequality Operator..
//
//==============================================================================
inline bool CTxtBasePacket::operator!=(IN const CTxtBasePacket& rSrc) const
{
    return !(*this == rSrc);
}

//==============================================================================
//====  ACCESSORS METHODS  =====================================================
//==============================================================================

//==============================================================================
//==
//==  GetPacketType
//==
//==============================================================================
//
//  Returns:
//      A string that describes the type of packet (protocol) of this instance.
//
//  Description:
//      Gets the type of packet as a string. The child classes are expected to
//      override this method and return a short description string that
//      identifies their supported protocol. For example a child CSipPacket
//      could return SIP, a child CHttpPacket could return HTTP and a child
//      CCpimPacket could return CPIM.
//
//      This implementation returns BASE to identify this as the base class for
//      packets.
//
//==============================================================================
inline const char* CTxtBasePacket::GetPacketType() const
{
    return ms_pszPACKET_TYPE;
}

//--<COMBINE CTxtBasePacket::GetPeerAddress>
inline const CSocketAddr& CTxtBasePacket::GetPeerAddress() const
{
    // const cast is used to reuse the implementation of the non-const version of this method.
    return const_cast<CTxtBasePacket*>(this)->GetPeerAddress();
}

//--<COMBINE CTxtBasePacket::GetLocalAddress>
inline const CSocketAddr& CTxtBasePacket::GetLocalAddress() const
{
    // const cast is used to reuse the implementation of the non-const version of this method.
    return const_cast<CTxtBasePacket*>(this)->GetLocalAddress();
}

//--<COMBINE CTxtBasePacket::GetHeaderList>
inline const CTxtHeaderList& CTxtBasePacket::GetHeaderList() const
{
    // const cast is used to reuse the implementation of the non-const version of this method.
    return const_cast<CTxtBasePacket*>(this)->GetHeaderList();
}

//==============================================================================
//==
//==  DumpAllocatorStatistics
//==
//==============================================================================
//
//  Parameters:
//      rstrOutput:
//          The string into which the statistics are dumped.
//
//  Description:
//      This method retrieves the statistics of the subAllocator. It can do so
//      only if MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT is defined.
//
//==============================================================================
#if defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT) && \
    defined(MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT)
inline void CTxtBasePacket::DumpAllocatorStatistics(OUT CString& rstrOutput)
{
    if (m_pAllocator != NULL)
    {
        m_pAllocator->DumpStatistics(OUT rstrOutput);
    }
}
#endif

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTXTBASEPACKET_H

