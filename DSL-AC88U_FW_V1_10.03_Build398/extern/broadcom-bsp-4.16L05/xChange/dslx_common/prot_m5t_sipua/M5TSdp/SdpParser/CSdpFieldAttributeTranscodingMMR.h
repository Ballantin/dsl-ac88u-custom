//==============================================================================
//==============================================================================
//
//   Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSDPFIELDATTRIBUTETRANSCODINGMMR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTETRANSCODINGMMR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SDP Parser Configuration
#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h" // MXD_SDP_ENABLE_T38_SUPPORT
#endif


// We compile this only if it was requested to include the T.38 classes.
#if defined(MXD_SDP_ENABLE_T38_SUPPORT)


// Data Member

// Interface Realized & Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif



// Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)



// Forward Declarations Inside of the Namespace
class CBlob;
//M5T_INTERNAL_USE_END


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//==  Class: CSdpFieldAttributeTranscodingMMR
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a attribute-transcoding-mmr.
//
// Description:
//   This class is an abstraction of a attribute-transcoding-mmr
//   field in a SDP packet.
//
//   The parsing of this attribute-transcoding-mmr is a specific case of an
//   attribute.  The basic BNF that an attribute can have is described in
//   CSdpFieldAttributeOther.
//
//  <PRE>
//      attribute-transcoding-mmr = "T38FaxTranscodingMMR:" [bit]
//      bit                       =   space "0" / "1"
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeTranscodingMMR.h
//
// See Also:
//
//==============================================================================
class CSdpFieldAttributeTranscodingMMR : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeTranscodingMMR();

    // Summary: Copy constructor.
    CSdpFieldAttributeTranscodingMMR(IN const CSdpFieldAttributeTranscodingMMR& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeTranscodingMMR();

    //  Summary:
    //   Comparison operator.
    bool operator==(IN const CSdpFieldAttributeTranscodingMMR& rFrom) const;

    // Summary:
    //  Assignment operator.
    CSdpFieldAttributeTranscodingMMR& operator=(IN const CSdpFieldAttributeTranscodingMMR& rFrom);

    //-- << Get functions >>

    // Summary:
    //  Indicates if the T38FaxTranscodingMMR attribute is enabled
    //  or disabled.
    bool IsTranscodingMMR() const;

    // Summary:
    //  Indicates if the T38FaxTranscodingMMR attribute is encoded
    //  with the implicit method or the explicit method.
    bool IsImplicitTranscodingMMR() const;

    // Summary:
    //  Generates the data blob from the data members.
    void Serialize(INOUT CBlob& rBlob) const;

    //-- << Set functions >>

    // Summary:
    //  Enables the T38FaxTranscodingMMR attribute. Sets the encoding
    //  method to implicit. This method was deprecated. Use the
    //  SetTranscodingMMR(bool) method.
    void SetTranscodingMMR();

    // Summary:
    //  Enables or disables the T38FaxTranscodingMMR attribute. Sets the
    //  encoding method to explicit. This method was deprecated. Use the
    //  SetImplicitEncoding(bool) and SetTranscodingMMR(bool) methods.
    void SetExplicitTranscodingMMR(IN bool bSupported);

    // Summary:
    //  Enables or disables the T38FaxTranscodingMMR attribute.
    void SetTranscodingMMR(bool bEnable);

    // Summary:
    //  Sets the encoding method for the T38FaxTranscodingMMR attribute.
    void SetImplicitEncoding(bool bImplicitEncoding);

    //-- << Virtual functions >>

    // Summary:
    //  Parses all the needed information for this field. An error
    //  is signaled in 'rres' if the data couldn't be parsed or
    //  if an EOL wasn't found at the end of the data.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    //  Sets the value of the flag 'm_bIsValid' by checking the
    //  validity of the parsed data and returns this value.
    bool Validate();

    // Summary:
    //  Resets all the data members, to be ready for another call to
    //  Parse. Disables the T38FaxTranscodingMMR attribute. Sets
    //  the encoding method to implicit.
    void Reset();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:

// Hidden Data Members
protected:
private:

    // Indicates if the T38FaxTranscodingMMR attribute is enabled
    // or disabled.
    //------------------------------------------------------------
    bool m_bTranscodingMMR;

    // Indicates if the T38FaxTranscodingMMR is encoded with the
    // implicit or explicit method.
    //-----------------------------------------------------------
    bool m_bImplicitEncoding;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  CSdpFieldAttributeTranscodingMMR::IsTranscodingMMR
//==
//==============================================================================
//
//  Returns:
//      True if the T38FaxTranscodingMMR attribute is enabled. False
//      otherwise.
//
//  Description:
//      Indicates if the T38FaxTranscodingMMR attribute is enabled
//      or disabled.
//
//==EDOC========================================================================
inline bool CSdpFieldAttributeTranscodingMMR::IsTranscodingMMR() const
{
    return m_bTranscodingMMR;
}

//==SDOC========================================================================
//==
//==  CSdpFieldAttributeTranscodingMMR::IsImplicitTranscodingMMR
//==
//==============================================================================
//
//  Returns:
//      True if the T38FaxTranscodingMMR attribute is encoded with the
//      implicit method. False if the T38FaxTranscodingMMR attribute is
//      encoded with the explicit method.
//
//  Description:
//      Indicates if the T38FaxTranscodingMMR attribute is encoded
//      with the implicit method or the explicit method.
//
//==EDOC========================================================================
inline bool CSdpFieldAttributeTranscodingMMR::IsImplicitTranscodingMMR() const
{
    return m_bImplicitEncoding;
}

//==SDOC========================================================================
//==
//==  CSdpFieldAttributeTranscodingMMR::SetTranscodingMMR
//==
//==============================================================================
//
//  Description:
//      Enables the T38FaxTranscodingMMR attribute. Sets the encoding
//      method to implicit. This method was deprecated. Use the
//      SetTranscodingMMR(bool) method.
//
//==EDOC========================================================================
inline void CSdpFieldAttributeTranscodingMMR::SetTranscodingMMR()
{
    m_bTranscodingMMR = true;
    m_bImplicitEncoding = true;
}

//==SDOC========================================================================
//==
//==  CSdpFieldAttributeTranscodingMMR::SetExplicitTranscodingMMR
//==
//==============================================================================
//
//  Parameters:
//    bSupported:
//      Indicates if the T38FaxTranscodingMMR attribute is enabled or
//      disabled.
//
//  Description:
//      Enables or disables the T38FaxTranscodingMMR attribute. Sets the
//      encoding method to explicit. This method was deprecated. Use the
//      SetImplicitEncoding(bool) and SetTranscodingMMR(bool) methods.
//
//==EDOC========================================================================
inline void CSdpFieldAttributeTranscodingMMR::SetExplicitTranscodingMMR(IN bool bSupported)
{
    m_bImplicitEncoding = false;
    m_bTranscodingMMR = bSupported;
}

//==SDOC========================================================================
//==
//==  CSdpFieldAttributeTranscodingMMR::SetTranscodingMMR
//==
//==============================================================================
//
//  Parameters:
//    bEnable:
//      Indicates if the T38FaxTranscodingMMR attribute is enabled or
//      disabled.
//
//  Description:
//      Enables or disables the T38FaxTranscodingMMR attribute.
//
//==EDOC========================================================================
inline void CSdpFieldAttributeTranscodingMMR::SetTranscodingMMR(bool bEnable)
{
    m_bTranscodingMMR = bEnable;
}

//==SDOC========================================================================
//==
//==  CSdpFieldAttributeTranscodingMMR::SetImplicitEncoding
//==
//==============================================================================
//
//  Parameters:
//    bImplicitEncoding:
//      Indicates if the T38FaxTranscodingMMR attribute is encoded with
//      the implicit method (true) or the explicit method (false).
//
//  Description:
//      Sets the encoding method for the T38FaxTranscodingMMR attribute.
//
//==EDOC========================================================================
inline void CSdpFieldAttributeTranscodingMMR::SetImplicitEncoding(bool bImplicitEncoding)
{
    m_bImplicitEncoding = bImplicitEncoding;
}

MX_NAMESPACE_END(MXD_GNS)


#endif //#if defined(MXD_SDP_ENABLE_T38_SUPPORT)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTETRANSCODINGMMR_H

