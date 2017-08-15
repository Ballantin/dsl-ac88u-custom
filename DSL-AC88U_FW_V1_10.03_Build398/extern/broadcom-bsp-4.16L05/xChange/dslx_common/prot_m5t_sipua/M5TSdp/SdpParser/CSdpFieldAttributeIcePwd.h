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
#ifndef MXG_CSDPFIELDATTRIBUTEICEPWD_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEICEPWD_H

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

#ifndef MXG_CSDPFIELDATTRIBUTEICESINGLETOKENBASE_H
#include "SdpParser/CSdpFieldAttributeIceSingleTokenBase.h"
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
//== Class: CSdpFieldAttributeIcePwd
//==============================================================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   Implements the ice-pwd attribute.
//
// Description:
//   This class is an abstraction of an ice-pwd. It follows the 
//   BNF notation described in the draft-ietf-mmusic-ice-19.
//
// From draft-ietf-mmusic-ice-19:
// <PRE>
//     ice-pwd-att           = "ice-pwd" ":" password
//     password              = 22*256ice-char
// </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeIcePwd.h
//
//==============================================================================
class CSdpFieldAttributeIcePwd : public CSdpFieldAttributeIceSingleTokenBase
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSdpFieldAttributeIcePwd();

    // Summary:
    //  Destructor.
    virtual ~CSdpFieldAttributeIcePwd();

    // Summary:
    //  Copy Constructor.
    CSdpFieldAttributeIcePwd(IN const CSdpFieldAttributeIcePwd& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributeIcePwd& operator=(IN const CSdpFieldAttributeIcePwd& rSrc);

    // Summary:
    //  Comparison Operator.
    bool operator==(IN const CSdpFieldAttributeIcePwd& rFrom) const;

    //-- << Stereotype >>

    // Summary:
    //  Serializes the value into the blob.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Gets the UserFrag.
    const char* GetPassword() const;

    // Summary:
    //  Sets the UserFrag.
    void SetPassword(IN const char* pszPwd);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Data Members
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetPassword
//==
//==============================================================================
//
//  Returns:
//      The password.
//
//  Description:
//      Returns the password.
//
//==============================================================================
inline const char* CSdpFieldAttributeIcePwd::GetPassword() const
{
    return GetValue();
}

//==============================================================================
//==
//==  SetPassword
//==
//==============================================================================
//
//  Parameters:
//      pszPwd:
//       The password to set.
//
//  Description:
//      Sets the password.
//
//==============================================================================
inline void CSdpFieldAttributeIcePwd::SetPassword(IN const char* pszPwd)
{
    SetValue(pszPwd);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTEICEPWD_H

