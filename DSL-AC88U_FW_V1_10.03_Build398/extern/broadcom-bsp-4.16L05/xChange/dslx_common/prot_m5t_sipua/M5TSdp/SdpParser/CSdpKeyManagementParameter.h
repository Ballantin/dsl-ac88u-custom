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
#ifndef MXG_CSDPKEYMANAGEMENTPARAMETER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPKEYMANAGEMENTPARAMETER_H

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


//-- Data Members


//-- Parameters used by Value


//-- Interface Realized and/or Parent


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
//==  Class: CSdpKeyManagementParameter
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements the base class for handling parameters related to key
//   management attribute.
//
// Description:
//   This class is the base class for handling parameters related to key
//   management attribute. It permits an application to set capabilities
//   pertaining to the handling of key management attributes.
//
//   It is possible for a media to have key management parameters even though
//   the key management attribute is located at the session level. This is the
//   case for certain key management protocols (such as MIKEY) which can handle
//   the media info even if at the session level.
//
// Location:
//   SdpParser/CSdpKeyManagementParameter.h
//
// See Also:
//   CSdpMikeyKeyManagementParameter
//
//==============================================================================
class CSdpKeyManagementParameter
{
//-- Published enums
//M5T_INTERNAL_USE_BEGIN
public:
    enum EKeyManagementType
    {
        eKEY_MGMT_MIKEY,
        eKEY_MGMT_UNKNOWN = 0xFF
    };
//M5T_INTERNAL_USE_END
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpKeyManagementParameter();

    // Summary:
    //  Constructor with specific EKeyManagementType.
    CSdpKeyManagementParameter(IN EKeyManagementType eType);

    // Summary: Copy Constructor.
    CSdpKeyManagementParameter(IN const CSdpKeyManagementParameter& rSrc);

    // Summary: Destructor.
    virtual ~CSdpKeyManagementParameter();

    // Summary: Comparison operator.
    bool operator==(IN const CSdpKeyManagementParameter& rSrc) const;

    //-- << Stereotype >>

    // Summary:
    //  Generates a copy of the object.
    virtual GO CSdpKeyManagementParameter* GenerateCopy() const;

    // Summary:
    //  Gets the type of key management parameter.
    EKeyManagementType GetType() const;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Assignment Operator.
    CSdpKeyManagementParameter& operator=(IN const CSdpKeyManagementParameter& rSrc);


//-- Hidden Data Members
protected:
private:
    // Type of parameter object.
    EKeyManagementType m_eType;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==============================================================================
//==
//==  CSdpKeyManagementParameter
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==============================================================================
inline CSdpKeyManagementParameter::CSdpKeyManagementParameter():
m_eType(eKEY_MGMT_UNKNOWN)
{

}

//==============================================================================
//==
//==  CSdpKeyManagementParameter
//==
//==============================================================================
//
//  Parameters:
//      eType:
//          The type of parameter
//
//  Description:
//      Constructor.
//
//==============================================================================
inline CSdpKeyManagementParameter::CSdpKeyManagementParameter(IN EKeyManagementType eType) :
  m_eType(eType)
{
}

//==============================================================================
//==
//==  CSdpKeyManagementParameter
//==
//==============================================================================
//
//  Description:
//      Copy constructor.
//
//==============================================================================
inline CSdpKeyManagementParameter::CSdpKeyManagementParameter(IN const CSdpKeyManagementParameter& rSrc) :
  m_eType(rSrc.m_eType)
{
}

//==============================================================================
//==
//==  ~CSdpKeyManagementParameter
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline CSdpKeyManagementParameter::~CSdpKeyManagementParameter()
{
}

//==============================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          Parameter to compare with.
//
//  Returns:
//      true if both are equal, false otherwise.
//
//  Description:
//      Comparison operator.
//
//==============================================================================
inline bool CSdpKeyManagementParameter::operator==(IN const CSdpKeyManagementParameter& rSrc) const
{
    return m_eType == rSrc.m_eType;
}

//==============================================================================
//==
//==  GetType
//==
//==============================================================================
//
//  Returns:
//      The type of key management parameter.
//
//  Description:
//      Returns the type of the key management parameter.
//
//==============================================================================
inline
CSdpKeyManagementParameter::EKeyManagementType CSdpKeyManagementParameter::GetType() const
{
    return m_eType;
}

//==============================================================================
//==
//==  GenerateCopy
//==
//==============================================================================
//
//  Returns:
//      A copy of this key management parameter. Ownership is given.
//
//  Description:
//      Creates a copy of this key management parameter.
//
//==============================================================================
inline GO CSdpKeyManagementParameter*
CSdpKeyManagementParameter::GenerateCopy() const
{
    return MX_NEW(CSdpKeyManagementParameter)(*this);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSDPKEYMANAGEMENTPARAMETER_H



