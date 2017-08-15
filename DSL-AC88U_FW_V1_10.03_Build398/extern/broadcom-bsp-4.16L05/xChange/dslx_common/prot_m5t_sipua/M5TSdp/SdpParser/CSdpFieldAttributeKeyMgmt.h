//==============================================================================
//==============================================================================
//
//   Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSDPFIELDATTRIBUTEKEYMGMT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEKEYMGMT_H

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

///-- Data Members
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

//-- Parameters used by Value
#include "Cap/CBlob.h"


//-- Interface Realized and/or Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif

#ifndef MXG_CSDPKEYMANAGEMENTPARAMETER_H
#include "SdpParser/CSdpKeyManagementParameter.h"
#endif


MX_NAMESPACE_START(MXD_GNS)



//-- Forward Declarations

//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//==  Class: CSdpFieldAttributeKeyMgmt
//==============================================================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a key-mgmt-attribute.
//
// Description:
//   This class is an abstraction of an key-mgmt-attribute in a SDP packet.
//
//  <PRE>
//     key-mgmt-attribute = key-mgmt-att-field ":" key-mgmt-att-value
//
//     key-mgmt-att-field = "key-mgmt"
//     key-mgmt-att-value = 0*1SP prtcl-id SP keymgmt-data
//
//     prtcl-id     = KMPID
//                    ; e.g. "mikey"
//
//     keymgmt-data = base64
//     SP           = 0x20
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeKeyMgmt.h
//
//==============================================================================
class CSdpFieldAttributeKeyMgmt : public CSdpParser
{
// Published Interface
public:
    // Summary:
    //  Defines the supported types of key management protocols.
    //
    // Description:
    //  Defines the types of key management protocols that are supported by this
    //  object and by its child classes.
    enum EKeyManagementProtocol
    {
        // Description:
        //  Generic describes all types of key management protocols that are
        //  different from MIKEY.
        eGENERIC,
        // Description:
        //  The MIKEY key exchange protocol.
        eMIKEY
    };

    // Summary:
    //  Tells in which scenario the key management attribute is used.
    //
    // Description:
    //  Tells in which scenario the key management attribute is used.
    enum EKeyManagementAttributeRole
    {
        // Description:
        //  The key management attribute is never used.
        eNONE,
        // Description:
        //  The key management attribute is used for offers only.
        eOFFER,
        // Description:
        //  The key management attribute is used for answers only.
        eANSWER,
        // Description:
        //  The key management attribute is used for offers and answers.
        eBOTH
    };

public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default construtor.
    CSdpFieldAttributeKeyMgmt();

    // Summary: Copy constructor.
    CSdpFieldAttributeKeyMgmt(IN const CSdpFieldAttributeKeyMgmt& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeKeyMgmt();

    // Summary: Comparison Operator.
    bool operator==(IN const CSdpFieldAttributeKeyMgmt& rFrom) const;

    // Summary:
    //  Assignment operator.
    CSdpFieldAttributeKeyMgmt& operator=(IN const CSdpFieldAttributeKeyMgmt& rFrom);

    // Summary:
    // Gets the protocol string identifier.
    const char* GetProtocolId() const;

    // Summary:
    // Gets the value of the key management attribute.
    const char* GetValue() const;

    // Summary:
    // Gets the type of the key management protocol.
    EKeyManagementProtocol GetKeyManagementProtocol() const;

    // Summary:
    // Sets the protocol string identifier.
    void SetProtocolId(IN const char* szFormat);

    // Summary:
    // Sets the value of the key management attribute.
    void SetValue(IN const char* szValue);

    // Summary:
    // Gets the role where this key management attribute is used.
    EKeyManagementAttributeRole GetKeyManagementRole() const;

    // Summary:
    // Sets the role where this key management attribute is used.
    void SetKeyManagementRole(IN const EKeyManagementAttributeRole eRole);

    //-- Virtual functions

    // Summary:
    //  Parses the parameters list beginning at rpszStartPosition.
    virtual EParserResult Parse(INOUT const char*& rpszStartPosition,
                                OUT mxt_result& rRes);

    // Summary:
    //  Outputs the data members to a blob.
    virtual void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Returns true if data members are valid.
    virtual bool Validate();

    // Summary:
    //  Resets this object.
    virtual void Reset();

    // Summary:
    //  Generates a copy of the object.
    virtual GO CSdpFieldAttributeKeyMgmt* GenerateCopy() const;

    // Summary:
    //  Generates a key management parameter.
    virtual GO CSdpKeyManagementParameter* GenerateParameter() const;


//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:


// Hidden Data Members
protected:
    EKeyManagementProtocol m_eManagerType;
private:
    CString m_strProtocolId;
    CString m_strValue;

    // If this value is set to false, this attribute will not be serialized.
    bool m_bIsActive;

    mutable EKeyManagementAttributeRole m_eRole;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetProtocolId
//==
//==============================================================================
//
//  Returns:
//      The protocol ID.
//
//  Description:
//      Returns the protocol ID.
//
//==============================================================================
inline const char* CSdpFieldAttributeKeyMgmt::GetProtocolId() const
{
    return m_strProtocolId.CStr();
}

//==============================================================================
//==
//==  GetValue
//==
//==============================================================================
//
//  Returns:
//      The char representation of the key management attribute.
//
//  Description:
//      Gets char representation of the key management attribute.
//
//==============================================================================
inline const char*
CSdpFieldAttributeKeyMgmt::GetValue() const
{
    return m_strValue.CStr();
}

//==============================================================================
//==
//==  SetProtocolId
//==
//==============================================================================
//
//  Description:
//      Sets the protocol ID.
//
//  Parameters:
//    szFormat
//      The protocol ID.
//
//==============================================================================
inline void CSdpFieldAttributeKeyMgmt::SetProtocolId(IN const char* szFormat)
{
    m_strProtocolId = szFormat;
}

//==============================================================================
//==
//==  SetValue
//==
//==============================================================================
//
//  Description:
//      Sets the value of the key.
//
//  Parameters:
//    szValue
//      The key value.
//
//==============================================================================
inline void CSdpFieldAttributeKeyMgmt::SetValue(IN const char* szValue)
{
    m_strValue = szValue;
}

//==============================================================================
//==
//==  GenerateCopy
//==
//==============================================================================
//
//  Returns:
//      A copy of this object. Ownership is given.
//
//  Description:
//      Creates a copy of this object
//
//==============================================================================
inline GO CSdpFieldAttributeKeyMgmt*
CSdpFieldAttributeKeyMgmt::GenerateCopy() const
{
    return MX_NEW(CSdpFieldAttributeKeyMgmt)(*this);
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
//      Generates a CSdpKeyManagementParameter of the appropriate type for this
//      key attribute.
//
//==============================================================================
inline GO CSdpKeyManagementParameter* CSdpFieldAttributeKeyMgmt::GenerateParameter() const
{
    return MX_NEW(CSdpKeyManagementParameter);
}

//==============================================================================
//==
//==  GetKeyManagementProtocol
//==
//==============================================================================
//
//  Returns:
//      The type of key management protocol.
//
//  Description:
//      Gets the type of key management protocol that this object represents.
//
//==============================================================================
inline CSdpFieldAttributeKeyMgmt::EKeyManagementProtocol CSdpFieldAttributeKeyMgmt::GetKeyManagementProtocol() const
{
    return m_eManagerType;
}

//==============================================================================
//==
//==  GetKeyManagementRole
//==
//==============================================================================
//
//  Returns:
//      The key management role.
//
//  Description:
//      Gets the key management role where the attribute will be effective.
//
//==============================================================================
inline CSdpFieldAttributeKeyMgmt::EKeyManagementAttributeRole
    CSdpFieldAttributeKeyMgmt::GetKeyManagementRole() const
{
    return m_eRole;
}

//==============================================================================
//==
//==  SetKeyManagementRole
//==
//==============================================================================
//
//  Parameter:
//    eRole:
//      The key management role.
//
//  Description:
//      Sets the key management role where the attribute will be effective.
//
//==============================================================================
inline void CSdpFieldAttributeKeyMgmt::SetKeyManagementRole(IN const EKeyManagementAttributeRole eRole)
{
    m_eRole = eRole;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTEKEYMGMT_H
