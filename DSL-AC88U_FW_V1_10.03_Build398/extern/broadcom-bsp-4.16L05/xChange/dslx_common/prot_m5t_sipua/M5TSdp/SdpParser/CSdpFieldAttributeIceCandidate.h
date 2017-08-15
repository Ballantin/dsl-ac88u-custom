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
#ifndef MXG_CSDPFIELDATTRIBUTEICECANDIDATE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEICECANDIDATE_H

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

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CFQDN_H
#include "Network/CFqdn.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CBlob;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
static const char* const gs_apszCANDIDATE_TYPE_HOST = "host";
static const char* const gs_apszCANDIDATE_TYPE_SERVER_REFLEXIVE = "srflx";
static const char* const gs_apszCANDIDATE_TYPE_PEER_REFLEXIVE = "prflx";
static const char* const gs_apszCANDIDATE_TYPE_RELAYED = "relay";

static const uint32_t gs_uRTP_COMPONENT_ID = 1;
static const uint32_t gs_uRTCP_COMPONENT_ID = 2;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CSdpFieldAttributeIceCandidate
//==============================================================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   Implements the ice-candidate attribute.
//
// Description:
//   This class is an abstraction of an ice-candidate. It follows
//   the BNF notation described in the draft-ietf-mmusic-ice-19.
//
// From draft-ietf-mmusic-ice-19:
// <PRE>
//   candidate-attribute   = "candidate" ":" foundation SP component-id SP
//                           transport SP
//                           priority SP
//                           connection-address SP     ;from RFC 4566
//                           port         ;port from RFC 4566
//                           SP cand-type
//                           [SP rel-addr]
//                           [SP rel-port]
//                           *(SP extension-att-name SP
//                                extension-att-value)
//
//   foundation            = 1*32ice-char
//   component-id          = 1*5DIGIT
//   transport             = "UDP" / transport-extension
//   transport-extension   = token              ; from RFC 3261
//   priority              = 1*10DIGIT
//   cand-type             = "typ" SP candidate-types
//   candidate-types       = "host" / "srflx" / "prflx" / "relay" / token
//   rel-addr              = "raddr" SP connection-address
//   rel-port              = "rport" SP port
//   extension-att-name    = byte-string    ;from RFC 4566
//   extension-att-value   = byte-string
// </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeIceCandidate.h
//
//==============================================================================
class CSdpFieldAttributeIceCandidate : public CSdpParser
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    struct SExtensionAtt
    {
        SExtensionAtt& operator=(IN const SExtensionAtt& rSrc)
        {
            m_strExtensionName = rSrc.m_strExtensionName;
            m_strExtensionValue = rSrc.m_strExtensionValue;

            return *this;
        }

        bool operator==(IN const SExtensionAtt& rFrom) const
        {
            return m_strExtensionName == rFrom.m_strExtensionName &&
                   m_strExtensionValue == rFrom.m_strExtensionValue;
        }

        bool operator!=(IN const SExtensionAtt& rFrom) const
        {
            return !(operator==(rFrom));
        }

        CString m_strExtensionName;
        CString m_strExtensionValue;
    };
//M5T_INTERNAL_USE_END
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeIceCandidate();

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeIceCandidate();

    // Summary: Copy Constructor.
    CSdpFieldAttributeIceCandidate(IN const CSdpFieldAttributeIceCandidate& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributeIceCandidate& operator=(IN const CSdpFieldAttributeIceCandidate& rSrc);

    // Summary: Comparison Operator.
    bool operator==(IN const CSdpFieldAttributeIceCandidate& rFrom) const;


    //-- << CSdpParser interface >>
    // Summary:
    //   Parses all the needed information for this field.
    virtual EParserResult Parse(INOUT const char*& rpszStartPosition,
                                OUT mxt_result& rres);

    // Summary:
    //  Returns true if data members are valid
    virtual bool Validate();

    // Summary:
    //  Resets this object.
    virtual void Reset();

    //-- << Stereotype >>

    // Summary:
    //  Serializes the value into the blob.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Returns a reference to the foundation string.
    const CString& GetFoundation() const;

    // Summary:
    //  Returns a reference to the foundation string.
    CString& GetFoundation();

    // Summary:
    //  Returns the component ID.
    uint32_t GetComponentId() const;

    // Summary:
    //  Sets the component ID.
    void SetComponentId(IN uint32_t uComponentId);

    // Summary:
    //  Gets a reference to the transport string.
    const CString& GetTransport() const;

    // Summary:
    //  Gets a reference to the transport string.
    CString& GetTransport();

    // Summary:
    //  Gets the priority
    uint64_t GetPriority() const;

    // Summary:
    //  Sets the priority
    void SetPriority(IN uint64_t uPriority);

    // Summary:
    //  Gets a reference to the candidate string.
    const CString& GetCandidate() const;

    // Summary:
    //  Gets a reference to the candidate string.
    CString& GetCandidate();

    // Summary:
    //  Gets the connection address.
    void GetConnectionAddr(INOUT const CSocketAddr** ppAddr, INOUT const CFqdn** ppFqdnAddr) const;

    // Summary:
    //  Sets the connection address.
    void SetConnectionAddr(IN CSocketAddr addr);

    // Summary:
    //  Gets the related connection address.
    void GetRelAddr(INOUT const CSocketAddr** ppAddr, INOUT const CFqdn** ppFqdnAddr) const;

    // Summary:
    //  Sets the related connection address.
    void SetRelAddr(IN CSocketAddr addr);

    // Summary:
    //  Gets a reference to the list of extension attributes.
    const CList<SExtensionAtt>& GetExtensionAttr() const;

    // Summary:
    //  Gets a reference to the list of extension attributes.
    CList<SExtensionAtt>& GetExtensionAttr();

    // Summary:
    //  Sets the port in the microliteport extension.
    void SetMicroLitePort(IN uint16_t uPort);

    // Summary:
    //  Gets the port from the microliteport extension.
    mxt_result GetMicroLitePort(OUT uint16_t& ruPort) const;

//M5T_INTERNAL_USE_BEGIN
protected:
    // Summary:
    //  Parse related attributes.
    CSdpParser::EParserResult ParseOptionalAttributes(INOUT const char*& rpszStartPosition,
                                                      OUT mxt_result& rres);

//-- Hidden Data Members
private:
    // Contains the foundation.
    CString m_strFoundation;

    // Contains the component-id (up to 5 digits).
    uint32_t m_uComponentId;

    // Contains the transport.
    CString m_strTransport;

    // Contains the priority (up to 10 digits).
    uint64_t m_uPriority;

    // Contains the connection-address (if it's a FQDN only).
    CFqdn m_fqdnConnectionAddr;

    // Contains the connection-address if it's a IP address (if it's a IP address).
    CSocketAddr m_ipConnectionAddr;

    // Contains the candidate-types.
    CString m_strCandidateType;

    // Contains the transport addresses related to the candidate (if it's a FQDN).
    CFqdn m_fqdnRelAddr;

    // Contains the transport addresses related to the candidate (if it's a IP address).
    CSocketAddr m_ipRelAddr;

    // List of extension attributes.
    CList<SExtensionAtt> m_lststExtensionAtt;

//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetFoundation
//==
//==============================================================================
//
//  Returns:
//      a reference to the foundation.
//
//  Description:
//      Returns a reference to the foundation string.
//
//==============================================================================
inline const CString& CSdpFieldAttributeIceCandidate::GetFoundation() const
{
    return m_strFoundation;
}

//==============================================================================
//==
//==  GetFoundation
//==
//==============================================================================
//
//  Returns:
//      a reference to the foundation.
//
//  Description:
//      Returns a reference to the foundation string.
//
//==============================================================================
inline CString& CSdpFieldAttributeIceCandidate::GetFoundation()
{
    return m_strFoundation;
}

//==============================================================================
//==
//==  GetComponentId
//==
//==============================================================================
//
//  Returns:
//      the component ID.
//
//  Description:
//      Returns the component ID.
//
//==============================================================================
inline uint32_t CSdpFieldAttributeIceCandidate::GetComponentId() const
{
    return m_uComponentId;
}

//==============================================================================
//==
//==  SetComponentId
//==
//==============================================================================
//
//  Parameters:
//      uComponentId:
//       The component-ID.
//
//  Description:
//      Sets the component-ID. It must not exeed 5 digits to be valid.
//
//==============================================================================
inline void CSdpFieldAttributeIceCandidate::SetComponentId(IN uint32_t uComponentId)
{
    m_uComponentId = uComponentId;
}

//==============================================================================
//==
//==  GetTransport
//==
//==============================================================================
//
//  Returns:
//      a reference to the transport string.
//
//  Description:
//      Returns a reference to the transport string.
//
//==============================================================================
inline const CString& CSdpFieldAttributeIceCandidate::GetTransport() const
{
    return m_strTransport;
}

//==============================================================================
//==
//==  GetTransport
//==
//==============================================================================
//
//  Returns:
//      a reference to the transport string.
//
//  Description:
//      Returns a reference to the transport string.
//
//==============================================================================
inline CString& CSdpFieldAttributeIceCandidate::GetTransport()
{
    return m_strTransport;
}

//==============================================================================
//==
//==  GetPriority
//==
//==============================================================================
//
//  Returns:
//      the priority.
//
//  Description:
//      Returns the priority.
//
//==============================================================================
inline uint64_t CSdpFieldAttributeIceCandidate::GetPriority() const
{
    return m_uPriority;
}

//==============================================================================
//==
//==  SetPriority
//==
//==============================================================================
//
//  Parameters:
//      uPriority:
//       The priority to set.
//
//  Description:
//      Sets the priority.
//
//==============================================================================
inline void CSdpFieldAttributeIceCandidate::SetPriority(IN uint64_t uPriority)
{
    m_uPriority = uPriority;
}

//==============================================================================
//==
//==  GetCandidate
//==
//==============================================================================
//
//  Returns:
//      a reference to the candidate string.
//
//  Description:
//      Returns a reference to the candidate string.
//
//==============================================================================
inline const CString& CSdpFieldAttributeIceCandidate::GetCandidate() const
{
    return m_strCandidateType;
}

//==============================================================================
//==
//==  GetCandidate
//==
//==============================================================================
//
//  Returns:
//      a reference to the candidate string.
//
//  Description:
//      Returns a reference to the candidate string.
//
//==============================================================================
inline CString& CSdpFieldAttributeIceCandidate::GetCandidate()
{
    return m_strCandidateType;
}

//==============================================================================
//==
//==  SetConnectionAddr
//==
//==============================================================================
//
//  Parameters:
//      addr:
//       The connection address to set.
//
//  Description:
//      Sets the connection address.
//
//==============================================================================
inline void CSdpFieldAttributeIceCandidate::SetConnectionAddr(IN CSocketAddr addr)
{
    m_ipConnectionAddr = addr;
}

//==============================================================================
//==
//==  SetConnectionAddr
//==
//==============================================================================
//
//  Parameters:
//      addr:
//       The connection address to set.
//
//  Description:
//      Sets the connection address.
//
//==============================================================================
inline void CSdpFieldAttributeIceCandidate::SetRelAddr(IN CSocketAddr addr)
{
    m_ipRelAddr = addr;
}

//==============================================================================
//==
//==  GetExtensionAttr
//==
//==============================================================================
//
//  Returns:
//      a reference to the list of extension attributes.
//
//  Description:
//      Returns a reference to the list of extension attributes.
//
//==============================================================================
inline const CList<CSdpFieldAttributeIceCandidate::SExtensionAtt>& CSdpFieldAttributeIceCandidate::GetExtensionAttr() const
{
    return m_lststExtensionAtt;
}

//==============================================================================
//==
//==  GetExtensionAttr
//==
//==============================================================================
//
//  Returns:
//      a reference to the list of extension attributes.
//
//  Description:
//      Returns a reference to the list of extension attributes.
//
//==============================================================================
inline CList<CSdpFieldAttributeIceCandidate::SExtensionAtt>& CSdpFieldAttributeIceCandidate::GetExtensionAttr()
{
    return m_lststExtensionAtt;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTEICECANDIDATE_H

