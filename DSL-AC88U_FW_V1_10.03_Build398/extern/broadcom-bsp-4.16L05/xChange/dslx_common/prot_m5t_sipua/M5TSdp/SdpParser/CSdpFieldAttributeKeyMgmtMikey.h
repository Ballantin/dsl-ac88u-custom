//==============================================================================
//==============================================================================
//
//   Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSDPFIELDATTRIBUTEKEYMGMTMIKEY_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEKEYMGMTMIKEY_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SDP Parser Configuration
#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h"
#endif

#if defined(MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)

///-- Data Members
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

//-- Parameters used by Value
#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif

#ifndef MXG_CSDPFIELDATTRIBUTEKEYMGMT_H
#include "SdpParser/CSdpFieldAttributeKeyMgmt.h"
#endif

#ifndef MXG_CSDPKEYMANAGEMENTPARAMETERMIKEY_H
#include "SdpParser/CSdpKeyManagementParameterMikey.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IMikey;
class IMikeyCryptoSessionBundle;
class IMikeyKey;
class IMikeyMessage;
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
//==  Class: CSdpFieldAttributeKeyMgmtMikey
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a MIKEY key-mgmt-attribute.
//
// Description:
//   This class is an abstraction of a MIKEY key-mgmt-attribute in a SDP packet.
//
//  <PRE>
//     key-mgmt-attribute = key-mgmt-att-field ":" key-mgmt-att-value
//
//     key-mgmt-att-field = "key-mgmt"
//     key-mgmt-att-value = 0*1SP prtcl-id SP keymgmt-data
//
//     prtcl-id     = "mikey"
//
//     keymgmt-data = base64
//     SP           = 0x20
//  </PRE>
//
//  The attribute contains the Base64 encoded MIKEY message.
//
//  A CSdpFieldAttributeKeyMgmt class that returns a type eMIKEY from the
//  GetKeyManagementProtocol method can be safely used as a
//  CSdpFieldAttributeKeyMgmtMikey without any side effects.
//
// Location:
//   SdpParser/CSdpFieldAttributeKeyMgmtMikey.h
//
//==============================================================================
class CSdpFieldAttributeKeyMgmtMikey : public CSdpFieldAttributeKeyMgmt
{
// Enumerations
public:
//M5T_INTERNAL_USE_BEGIN
    enum EMikeyState
    {
        eEMPTY,         // No transaction has yet been started. This will
                        // automatically generate an initiation message.
        eINITIATED,     // We have Generated an initiation message
        eRECEIVED,      // We have received an initiation message
        eERRORMESSAGE,  // We have generated or have received an error message.
    };

    enum EMikeyIdentity
    {
        eINITIATOR,
        eRESPONDER
    };
//M5T_INTERNAL_USE_END

// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeKeyMgmtMikey();

    // Summary: Constructor.
    CSdpFieldAttributeKeyMgmtMikey(IN const CSdpFieldAttributeKeyMgmt& rFrom);

    // Summary: Copy constructor.
    CSdpFieldAttributeKeyMgmtMikey(IN const CSdpFieldAttributeKeyMgmtMikey& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeKeyMgmtMikey();

    // Summary: Assignment operator.
    CSdpFieldAttributeKeyMgmtMikey& operator=(IN const CSdpFieldAttributeKeyMgmtMikey& rFrom);

    // Summary: Comparison operator.
    bool operator==(IN const CSdpFieldAttributeKeyMgmtMikey& rFrom) const;

    //-- << Stereotype >>

    // Summary:
    //  Gets the error data from a received MIKEY error message.
    mxt_result GetErrorData(OUT mxt_result* presMikeyError);

    // Summary:
    //  Gets the stored IMikey interace.
    mxt_result GetMikey(OUT IMikey** ppMikey);

    // Summary:
    //  Gets the crypto session bundle stored.
    mxt_result GetMikeyCryptoSessionBundle(OUT IMikeyCryptoSessionBundle** ppCryptoSessionBundle);

    // Summary:
    //  Gets the list of SDP key mgmt IDs from the general extension data from
    //  the attribute.
    mxt_result GetSdpKeyMgmtIds(OUT CBlob* pData);

    // Summary:
    //  Checks if the MIKEY key management is currently sending a response.
    bool IsResponse();

    // Summary:
    //  Parses the MIKEY message.
    mxt_result ParseMikey(OUT mxt_result *presMikeyError);

    // Summary:
    //  Serializes the MIKEY message.
    void Serialize();

    // Summary:
    //  Sets the error data to send a MIKEY error message.
    mxt_result SetErrorData(IN mxt_result resMikeyError);

    // Summary:
    //  Sets the stored IMikey interace.
    mxt_result SetMikey(IN IMikey* pMikey);

    // Summary:
    //  Sets the crypto session bundle for the attribute.
    mxt_result SetMikeyCryptoSessionBundle(IN IMikeyCryptoSessionBundle* pCryptoSessionBundle);

    // Summary:
    //  Sets the general extension data for the attribute.
    mxt_result SetSdpKeyMgmtIds(IN const CBlob* pData);

    // Summary:
    //  Validates that the General Extension is valid.
    mxt_result ValidateSdpKeyMgmtIds(IN CBlob* pData);

    // Summary:
    //  Gives the crypto session bundle ID.
    mxt_result GetMikeyCryptoSessionBundleId(INOUT uint32_t* puId);

    // Summary:
    //  Parses the CMikeyMessage from the value stored in the member.
    mxt_result ParseMikeyMessage();

    //==========================================================================
    //==== CSdpFieldAttributeKeyMgmt methods ===================================
    //==========================================================================

    // Summary:
    //  Generates a copy of the object.
    virtual GO CSdpFieldAttributeKeyMgmt* GenerateCopy() const;

    // Summary:
    //  Generates key management parameter.
    virtual GO CSdpKeyManagementParameter* GenerateParameter() const;

    // Summary:
    //  Resets this object.
    virtual void Reset();

    // Summary:
    //  Outputs the data members to a blob.
    virtual void Serialize(INOUT CBlob& rBlob) const;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:
    // Summary:
    //  Creates a CMikeyMessage.
    mxt_result CreateInternalMessage();

// Hidden Data Members
protected:
private:
    EMikeyState m_eInternalState;
    IMikey* m_pMikey;
    IMikeyMessage* m_pMikeyMessage;
    IMikeyCryptoSessionBundle* m_pCryptoSessionBundle;
    bool m_bMutualAuthenticationRequired;

    mxt_result m_resMikeyError;

    CBlob m_blobSdpKeyMgmtIds;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
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
//      Creates a copy of this object
//
//==============================================================================
inline GO CSdpFieldAttributeKeyMgmt*
CSdpFieldAttributeKeyMgmtMikey::GenerateCopy() const
{
    return MX_NEW(CSdpFieldAttributeKeyMgmtMikey)(*this);
}

//==============================================================================
//==
//==  GenerateParameter
//==
//==============================================================================
//
//  Returns:
//      A key management parameter for this object.
//
//  Description:
//      Creates a CSdpKeyManagementParameter of the appropriate type for this
//      key attribute.
//
//==============================================================================
inline GO CSdpKeyManagementParameter* CSdpFieldAttributeKeyMgmtMikey::GenerateParameter() const
{
    return MX_NEW(CSdpKeyManagementParameterMikey);
}


//==============================================================================
//==
//==  IsResponse
//==
//==============================================================================
//
//  Returns:
//      -true: This is a response to an initiation.
//      -false: Not answering an initiation.
//
//  Description:
//      Validates if this attribute is currently capable of sending a response.
//
//==============================================================================
inline bool CSdpFieldAttributeKeyMgmtMikey::IsResponse()
{
    return m_eInternalState == eRECEIVED;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined (MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTEKEYMGMTMIKEY_H
