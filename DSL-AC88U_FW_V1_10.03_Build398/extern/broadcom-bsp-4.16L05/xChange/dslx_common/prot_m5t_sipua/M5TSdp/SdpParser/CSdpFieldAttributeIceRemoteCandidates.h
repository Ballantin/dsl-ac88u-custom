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
#ifndef MXG_CSDPFIELDATTRIBUTEICEREMOTECANDIDATES_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEICEREMOTECANDIDATES_H

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

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CFQDN_H
#include "Network/CFqdn.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CBlob;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CSdpFieldAttributeIceRemoteCandidates
//==============================================================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   Implements the ICE remote-candidates attribute.
//
// Description:
//   This class is an abstraction of an ice-remote-candidates. It
//   follows the BNF notation described in the draft-ietf-mmusic-ice-19.
//
// From draft-ietf-mmusic-ice-19:
// <PRE>
//   remote-candidate-att = "remote-candidates" ":" remote-candidate
//                           0*(SP remote-candidate)
//   remote-candidate = component-ID SP connection-address SP port
// </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeIceRemoteCandidates.h
//
//==============================================================================
class CSdpFieldAttributeIceRemoteCandidates : public CSdpParser
{
//-- Published Interface
public:

    // Container for ICE remote candidate attributes
    class CIceRemoteCandidates
    {
    //-- Friend Declarations
        friend class CSdpFieldAttributeIceRemoteCandidates;

    //-- Published Interface
    public:
        //-- << Constructors / Destructors / Operators >>

        // Summary:
        //  Default Constructor.
        CIceRemoteCandidates();

        // Summary:
        //  Destructor.
        virtual ~CIceRemoteCandidates();

        // Summary:
        //  Copy Constructor.
        CIceRemoteCandidates(IN const CIceRemoteCandidates& rSrc);

        // Summary:
        //  Assignment Operator.
        CIceRemoteCandidates& operator=(IN const CIceRemoteCandidates& rFrom);

        // Summary:
        //  Comparison Operator.
        bool operator==(IN const CIceRemoteCandidates& rFrom) const;

        //-- << Stereotype >>

        // Summary:
        //  Returns the component ID.
        uint32_t GetComponentId() const;

        // Summary:
        //  Sets the component ID.
        void SetComponentId(IN uint32_t uComponentId);

        // Summary:
        //  Gets the connection address.
        void GetConnectionAddr(INOUT const CSocketAddr** ppAddr, INOUT const CFqdn** ppFqdnAddr);

        // Summary:
        //  Sets the connection address.
        void SetConnectionAddr(IN CSocketAddr addr);

        // Summary:
        //  Sets the connection fqdn.
        void SetConnectionFqdn(IN CFqdn fqdn);

        //M5T_INTERNAL_USE_BEGIN
        //-- Hidden Data Members
    private:

        // Contains the component-id (up to 5 digits).
        uint32_t m_uComponentId;

        // Contains the connection-address (if it's a FQDN only).
        CFqdn m_fqdnConnectionAddr;

        // Contains the connection-address if it's a IP address (if it's a IP address).
        CSocketAddr m_ipConnectionAddr;
        //M5T_INTERNAL_USE_END
    };

    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeIceRemoteCandidates();

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeIceRemoteCandidates();

    // Summary: Copy Constructor.
    CSdpFieldAttributeIceRemoteCandidates(IN const CSdpFieldAttributeIceRemoteCandidates& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributeIceRemoteCandidates& operator=(IN const CSdpFieldAttributeIceRemoteCandidates& rFrom);

    // Summary: Comparison Operator.
    bool operator==(IN const CSdpFieldAttributeIceRemoteCandidates& rFrom) const;

    //-- << CSdpParser interface >>
    // Summary:
    //   Parses all the needed information for this field.
    virtual EParserResult Parse(INOUT const char*& rpszStartPosition,
                                OUT mxt_result& rres);

    // Summary:
    //  Returns true if data members are valid.
    virtual bool Validate();

    // Summary:
    //  Resets this object.
    virtual void Reset();

    //-- << Stereotype >>

    // Summary:
    //  Serializes the value into the blob.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Gets the ICE remote candidates vector.
    CVector<CIceRemoteCandidates*>& GetIceRemoteCandidates();
    const CVector<CIceRemoteCandidates*>& GetIceRemoteCandidates() const;

//-- Helpers
private:
    void ClearRemoteCandidates();

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Data Members
private:
    // ICE remote candidates vector.
    CVector<CIceRemoteCandidates*> m_vecpIceRemoteCandidates;
    //M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  CIceRemoteCandidates::CIceRemoteCandidates
//==
//==============================================================================
//
//  Description:
//      Default constructor.
//
//==============================================================================
inline CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates::CIceRemoteCandidates()
:   m_uComponentId(0xFFFFFFFF)
{

}

//==============================================================================
//==
//==  CIceRemoteCandidates::~CIceRemoteCandidates
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates::~CIceRemoteCandidates()
{

}

//==============================================================================
//==
//==  CIceRemoteCandidates::CIceRemoteCandidates
//==
//==============================================================================
//
//  Description:
//      Copy constructor.
//
//==============================================================================
inline CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates::CIceRemoteCandidates(IN const CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates& rSrc)
{
    *this = rSrc;
}

//==============================================================================
//==
//==  CIceRemoteCandidates::operator=
//==
//==============================================================================
//
//  Parameters:
//    rFrom
//      The right operand of the assignment (to copy in *this).
//
//  Returns:
//      A reference to this, to enable concatenation.
//
//  Description:
//      Assignment operator
//
//==============================================================================
inline CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates& CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates::operator=(IN const CIceRemoteCandidates& rFrom)
{
    if (this != &rFrom)
    {
        m_uComponentId = rFrom.m_uComponentId;
        m_fqdnConnectionAddr = rFrom.m_fqdnConnectionAddr;
        m_ipConnectionAddr = rFrom.m_ipConnectionAddr;
    }
    return *this;
}

//==============================================================================
//==
//==  CIceRemoteCandidates::operator==
//==
//==============================================================================
//
//  Parameters:
//    rFrom
//      The right operand of the comparison.
//
//  Returns:
//      true if attributes are identical.
//
//  Description:
//      Comparison operator
//
//==============================================================================
inline bool CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates::operator==(IN const CIceRemoteCandidates& rFrom) const
{
    return m_uComponentId == rFrom.m_uComponentId &&
           m_fqdnConnectionAddr == rFrom.m_fqdnConnectionAddr &&
           m_ipConnectionAddr == rFrom.m_ipConnectionAddr;
}


//==============================================================================
//==
//==  CIceRemoteCandidates::GetComponentId
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
inline uint32_t CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates::GetComponentId() const
{
    return m_uComponentId;
}

//==============================================================================
//==
//==  CIceRemoteCandidates::SetComponentId
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
inline void CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates::SetComponentId(IN uint32_t uComponentId)
{
    m_uComponentId = uComponentId;
}

//==============================================================================
//==
//==  CIceRemoteCandidates::SetConnectionAddr
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
inline void CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates::SetConnectionAddr(IN CSocketAddr addr)
{
    m_ipConnectionAddr = addr;
}

//==============================================================================
//==
//==  CIceRemoteCandidates::SetConnectionFqdn
//==
//==============================================================================
//
//  Parameters:
//      fqdn:
//       The connection FQDN to set.
//
//  Description:
//      Sets the connection FQDN.
//
//==============================================================================
inline void CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates::SetConnectionFqdn(IN CFqdn fqdn)
{
    m_fqdnConnectionAddr = fqdn;
}

//==============================================================================
//==
//==  GetIceRemoteCandidates
//==
//==============================================================================
//
//  Returns:
//      The CIceRemoteCandidates vector.
//
//  Description:
//      Returns the CIceRemoteCandidates vector.
//
//==============================================================================
inline CVector<CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates*>& CSdpFieldAttributeIceRemoteCandidates::GetIceRemoteCandidates()
{
    return m_vecpIceRemoteCandidates;
}

//==============================================================================
//==
//==  GetIceRemoteCandidates
//==
//==============================================================================
//
//  Returns:
//      The CIceRemoteCandidates vector.
//
//  Description:
//      Returns the CIceRemoteCandidates vector.
//
//==============================================================================
inline const CVector<CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates*>& CSdpFieldAttributeIceRemoteCandidates::GetIceRemoteCandidates() const
{
    return m_vecpIceRemoteCandidates;
}



MX_NAMESPACE_END(MXD_GNS)

#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTEICEREMOTECANDIDATES_H

