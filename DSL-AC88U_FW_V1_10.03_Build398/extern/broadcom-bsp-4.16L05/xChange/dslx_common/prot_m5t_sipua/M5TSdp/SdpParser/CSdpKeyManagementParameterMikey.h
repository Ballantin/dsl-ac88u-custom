//==============================================================================
//==============================================================================
//
//   Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSDPKEYMANAGEMENTPARAMETERMIKEY_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPKEYMANAGEMENTPARAMETERMIKEY_H

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

#if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)

#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif
//-- Data Members

//-- Parameters used by Value

//-- Interface Realized and/or Parent
#ifndef MXG_CSDPKEYMANAGEMENTPARAMETER_H
#include "SdpParser/CSdpKeyManagementParameter.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class IMikeyCryptoSession;
class IMikeyKey;
class IMikeySecurityPolicy;
class IMikeySecurityPolicyCapabilities;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSdpKeyManagementParameterMikey
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class is a container object used to store the media level parameters
//   used to generate a MIKEY message.
//
// Description:
//   This class is a container object used to store the media level parameters
//   used to generate a MIKEY message.
//
// Location:
//   SdpParser/CKeyMikeyManagementParameters.h
//
// See Also:
//   CKeyManagementParameters
//
//==============================================================================
class CSdpKeyManagementParameterMikey : public CSdpKeyManagementParameter
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSdpKeyManagementParameterMikey();

    // Summary:
    //  Copy Constructor.
    CSdpKeyManagementParameterMikey(IN const CSdpKeyManagementParameterMikey& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CSdpKeyManagementParameterMikey();

    // Summary:
    //  Assignment Operator.
    CSdpKeyManagementParameterMikey& operator=(IN const CSdpKeyManagementParameterMikey& rSrc);

    // Summary:
    //  Comparison operator
    bool operator==(IN const CSdpKeyManagementParameterMikey& rFrom) const;

    //-- << Stereotype >>

    // Summary:
    //  Generates a copy of the object.
    virtual GO CSdpKeyManagementParameter* GenerateCopy() const;

    // Summary:
    //  Gets the MIKEY key for the associated stream direction.
    mxt_result GetTek(IN CSdpParser::ERtpStreamDirection eDirection,
                      IN unsigned int uTekLength,
                      OUT IMikeyKey** ppTekKey) const;

    // SSRC negotiation and collision avoidance.

    // Summary:
    //  Gets the SSRC value for the associated stream direction.
    mxt_result GetSsrc(IN CSdpParser::ERtpStreamDirection eDirection,
                       OUT uint32_t* puSsrc) const;

    // Summary:
    //  Sets the SSRC value for the associated stream direction.
    mxt_result SetOutgoingSsrc(IN uint32_t uSsrc);

    // Summary:
    //  Gets the security policy for the associated stream direction.
    mxt_result GetSecurityPolicy(IN CSdpParser::ERtpStreamDirection eDirection,
                                 OUT IMikeySecurityPolicy** ppSecurityPolicy) const;

    // Summary:
    //  Gets the crypto session for the associated stream direction.
    mxt_result GetCryptoSession(IN CSdpParser::ERtpStreamDirection eDirection,
                                OUT IMikeyCryptoSession** ppCryptoSession) const;

    // Summary:
    //  Gets the security policy capabilities for the associated stream
    //  direction.
    mxt_result GetSecurityPolicyCapabilities(IN CSdpParser::ERtpStreamDirection eDirection,
                                             OUT IMikeySecurityPolicyCapabilities** ppSecurityPolicyCapabilities) const;

    // Summary:
    //  Sets the crypto session for the associated stream direction.
    mxt_result SetCryptoSession(IN CSdpParser::ERtpStreamDirection eDirection,
                                IN IMikeyCryptoSession* pCryptoSession);

    // Summary:
    //  Sets the security policy for the associated stream direction.
    mxt_result SetSecurityPolicy(IN CSdpParser::ERtpStreamDirection eDirection,
                                 IN IMikeySecurityPolicy* pSecurityPolicy);

    // Summary:
    //  Sets the security policy capabilities for the associated stream
    //  direction.
    mxt_result SetSecurityPolicyCapabilities(IN CSdpParser::ERtpStreamDirection eDirection,
                                             IN IMikeySecurityPolicyCapabilities* pSecurityPolicyCapabilities);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    void Reset();

//-- Hidden Data Members
protected:
private:
    IMikeyCryptoSession* m_apCryptoSession[2];
    uint32_t m_uOutgoingSsrc;
    IMikeySecurityPolicyCapabilities* m_apSecurityPolicyCapabilities[2];
    IMikeySecurityPolicy* m_apSecurityPolicy[2];
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GenerateCopy
//==
//==============================================================================
//
//  Returns:
//      A copy of this object. Ownership is given.
//  Description:
//      Creates a copy of this object.
//
//==============================================================================
inline GO CSdpKeyManagementParameter*
CSdpKeyManagementParameterMikey::GenerateCopy() const
{
    return MX_NEW(CSdpKeyManagementParameterMikey)(*this);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)
#endif //-- #ifndef MXG_CSDPKEYMANAGEMENTPARAMETERMIKEY_H



