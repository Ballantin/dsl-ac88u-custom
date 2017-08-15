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
#ifndef MXG_CSDPFIELDATTRIBUTEICEUSERFRAG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEICEUSERFRAG_H

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
//== Class: CSdpFieldAttributeIceUserFrag
//==============================================================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   Implements the ice-ufrag attribute.
//
// Description:
//   This class is an abstraction of an ice-ufrag. It follows the 
//   BNF notation described in the draft-ietf-mmusic-ice-19.
//
// From draft-ietf-mmusic-ice-19:
// <PRE>
//     ice-ufrag-att         = "ice-ufrag" ":" ufrag
//     ufrag                 = 4*256ice-char
// </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeIceUserFrag.h
//
//==============================================================================
class CSdpFieldAttributeIceUserFrag : public CSdpFieldAttributeIceSingleTokenBase
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSdpFieldAttributeIceUserFrag();

    // Summary:
    //  Destructor.
    virtual ~CSdpFieldAttributeIceUserFrag();

    // Summary:
    //  Copy Constructor.
    CSdpFieldAttributeIceUserFrag(IN const CSdpFieldAttributeIceUserFrag& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributeIceUserFrag& operator=(IN const CSdpFieldAttributeIceUserFrag& rSrc);

    // Summary:
    //  Comparison Operator.
    bool operator==(IN const CSdpFieldAttributeIceUserFrag& rFrom) const;

    //-- << Stereotype >>

    // Summary:
    //  Serializes the value into the blob.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Gets the UserFrag.
    const char* GetUserFrag() const;

    // Summary:
    //  Sets the UserFrag.
    void SetUserFrag(IN const char* pszUserFrag);


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
//==  GetUserFrag
//==
//==============================================================================
//
//  Returns:
//      The user fragment.
//
//  Description:
//      Returns the user fragment.
//
//==============================================================================
inline const char* CSdpFieldAttributeIceUserFrag::GetUserFrag() const
{
    return GetValue();
}

//==============================================================================
//==
//==  SetUserFrag
//==
//==============================================================================
//
//  Parameters:
//      pszUserFrag:
//       The user fragment to set.
//
//  Description:
//      Sets the user fragment.
//
//==============================================================================
inline void CSdpFieldAttributeIceUserFrag::SetUserFrag(IN const char* pszUserFrag)
{
    SetValue(pszUserFrag);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTEICEUSERFRAG_H

