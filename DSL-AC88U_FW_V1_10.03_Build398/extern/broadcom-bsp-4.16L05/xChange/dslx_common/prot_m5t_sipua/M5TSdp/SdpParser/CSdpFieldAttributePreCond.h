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
#ifndef MXG_CSDPFIELDATTRIBUTEPRECOND_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEPRECOND_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif

// BRCM_CUSTOM - [add] Precondition support
#ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support

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
//== Class: CSdpFieldAttributePreCond
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of the precondition field attribute.
//
// Description:
//   This class is an abstraction of the precondition field attribute in SDP
//   from RFC 3312.
//
//   <PRE>
//      desired-status     =  "a=des:" precondition-type
//                             SP strength-tag SP status-type
//                             SP direction-tag
//      current-status     =  "a=curr:" precondition-type
//                            SP status-type SP direction-tag
//      confirm-status     =  "a=conf:" precondition-type
//                            SP status-type SP direction-tag
//      precondition-type  =  "qos" | token
//      strength-tag       =  ("mandatory" | "optional" | "none"
//                         =  | "failure" | "unknown")
//      status-type        =  ("e2e" | "local" | "remote")
//      direction-tag      =  ("none" | "send" | "recv" | "sendrecv")
//   </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributePreCond.h
//
//==============================================================================
class CSdpFieldAttributePreCond : public CSdpParser
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Destructor.
    virtual ~CSdpFieldAttributePreCond();

    // Summary:
    //  Copy Constructor.
    CSdpFieldAttributePreCond(IN const CSdpFieldAttributePreCond& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributePreCond& operator=(IN const CSdpFieldAttributePreCond& rSrc);

    // Summary:
    //  Comparison operator.
    bool operator==(IN const CSdpFieldAttributePreCond& rFrom) const;

    //-- << CSdpParser interface >>

    // Summary:
    //  Parses the data.
    virtual EParserResult Parse(INOUT const char*& rpszStartPosition,
                                OUT mxt_result& rres);

    // Summary:
    //  Validates the parsed data.
    virtual bool Validate();

    // Summary:
    //  Resets the data in the parser.
    virtual void Reset();

    //-- << Stereotype >>

    // Summary:
    //  Serializes the value into the blob.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Gets the Precondition type.
    const char* GetPrecondType() const;

    // Summary:
    //  Sets the Precondition type.
    void SetPrecondType(IN const char* pszValue);

    // Summary:
    //  Gets the Status tag.
    const CSdpParser::EPreCondStatusTag GetStatusTag() const;

    // Summary:
    //  Sets the Status tag.
    void SetStatusTag(IN CSdpParser::EPreCondStatusTag ePreCondStatusTag);

    // Summary:
    //  Gets the Direction tag.
    const CSdpParser::EPreCondDirectionTag GetDirectionTag() const;

    // Summary:
    //  Sets the Direction tag.
    void SetDirectionTag(IN CSdpParser::EPreCondDirectionTag ePreCondDirectionTag);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:

    // Summary:
    //  Constructor.
    CSdpFieldAttributePreCond(IN EPreCondField ePreCondField);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSdpFieldAttributePreCond();


//-- Hidden Data Members
private:
    // EPreCondField field type.
    EPreCondField m_ePreCondField;

protected:
    // Precondition type.
    CString m_strPrecondType;

    // Precondition Strength parameter. This attribute is only used for DES
    // field. See CSdpFieldAttributePreCondDes.
    EPreCondStrengthTag m_ePreCondStrength;

    // Precondition Status parameter.
    EPreCondStatusTag m_ePreCondStatus;

    // Precondition Direction parameter.
    EPreCondDirectionTag m_ePreCondDirection;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetPrecondType
//==
//==============================================================================
//
//  Returns:
//      A string that contains the precondition type.
//
//  Description:
//      Returns a string that contains the precondition type.
//
//==============================================================================
inline const char* CSdpFieldAttributePreCond::GetPrecondType() const
{
    return m_strPrecondType.CStr();
}

//==============================================================================
//==
//==  SetPrecondType
//==
//==============================================================================
//
//  Parameters:
//    pszValue:
//      String that contains the precondition type.
//
//  Description:
//      Sets the string that contains the precondition type.
//
//==============================================================================
inline void CSdpFieldAttributePreCond::SetPrecondType(IN const char* pszValue)
{
    m_strPrecondType = pszValue;
}

//==============================================================================
//==
//==  GetStatusTag
//==
//==============================================================================
//
//  Returns:
//      The status parameter.
//
//  Description:
//      Returns the status parameter.
//
//==============================================================================
inline const CSdpParser::EPreCondStatusTag CSdpFieldAttributePreCond::GetStatusTag() const
{
    return m_ePreCondStatus;
}

//==============================================================================
//==
//==  SetStatusTag
//==
//==============================================================================
//
//  Parameters:
//    ePreCondStatusTag:
//      The precondition status parameter.
//
//  Description:
//      Sets the precondition status parameter.
//
//==============================================================================
inline void CSdpFieldAttributePreCond::SetStatusTag(IN CSdpParser::EPreCondStatusTag ePreCondStatusTag)
{
    m_ePreCondStatus = ePreCondStatusTag;
}

//==============================================================================
//==
//==  GetDirectionTag
//==
//==============================================================================
//
//  Returns:
//      The direction tag.
//
//  Description:
//      Returns the direction tag.
//
//==============================================================================
inline const CSdpParser::EPreCondDirectionTag CSdpFieldAttributePreCond::GetDirectionTag() const
{
    return m_ePreCondDirection;
}

//==============================================================================
//==
//==  SetDirectionTag
//==
//==============================================================================
//
//  Parameters:
//    ePreCondDirectionTag:
//      The precondition direction tag.
//
//  Description:
//      Sets the precondition direction tag.
//
//==============================================================================
inline void CSdpFieldAttributePreCond::SetDirectionTag(IN CSdpParser::EPreCondDirectionTag ePreCondDirectionTag)
{
    m_ePreCondDirection = ePreCondDirectionTag;
}

MX_NAMESPACE_END(MXD_GNS)

// BRCM_CUSTOM - [add] Precondition support
#endif //-- #ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support

#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTEPRECOND_H

