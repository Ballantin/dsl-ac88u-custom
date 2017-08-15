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
#ifndef MXG_CSDPFIELDATTRIBUTET38FACSIMILEMAXBUFFER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTET38FACSIMILEMAXBUFFER_H

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



// interface Realized & Parent
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
//==  Class: CSdpFieldAttributeT38FacsimileMaxBuffer
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of an attribute-t38-facsimile-max-buffer.
//
// Description:
//   This class is an abstraction of an attribute-t38-facsimile-max-buffer in a
//   SDP packet.
//
//   The parsing of this attribute-t38-facsimile-max-buffer is a specific case
//   of an attribute.  The basic BNF that an attribute can have is described in
//   CSdpFieldAttributeOther.
//
//  <PRE>
//      attribute-t38-facsimile-max-buffer =    "T38FaxMaxBuffer:" byte-string
//      byte-string                        =    1*(0x01..0x09|0x0b|0x0c|0x0e..0xff)
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeT38FacsimileMaxBuffer.h
//
// See Also:
//
//==============================================================================
class CSdpFieldAttributeT38FacsimileMaxBuffer : public CSdpParser
{
// Published interface
public:
    //-- << Constructors / Destructors / Operators >>
    // Summary: Default Constructor.
    CSdpFieldAttributeT38FacsimileMaxBuffer();

    // Summary: Copy Constructor.
    CSdpFieldAttributeT38FacsimileMaxBuffer(IN const CSdpFieldAttributeT38FacsimileMaxBuffer& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeT38FacsimileMaxBuffer();

    //  Summary:
    //   Comparison operator.
    bool operator==(IN const CSdpFieldAttributeT38FacsimileMaxBuffer& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpFieldAttributeT38FacsimileMaxBuffer& operator=(IN const CSdpFieldAttributeT38FacsimileMaxBuffer& rFrom);

    //-- << Stereotype >>

    // Summary:
    //  Gets the maximum buffer.
    const int32_t   GetMaxBuffer() const;

    // Summary:
    //   Generates the data blob from the data members.
    void            Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //   Sets the maximum buffer.
    void SetMaxBuffer(IN const int nMaxBuffer);

    // Summary:
    //   Parses all the needed information for this field.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    //   Checks the validity of the parsed data.
    bool Validate();

    // Summary:
    //   Resets all the data members.
    void Reset();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:

// Hidden Data Members
protected:
private:
    int32_t m_nMaxBuffer;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetMaxBuffer
//==
//==============================================================================
//
//  Returns:
//      The maximum buffer attribute.
//
//  Description:
//      Returns the T38 Facsimile maximum buffer.
//
//==============================================================================
inline const int32_t CSdpFieldAttributeT38FacsimileMaxBuffer::GetMaxBuffer() const
{
    return m_nMaxBuffer;
}

//==============================================================================
//==
//==  SetMaxBuffer
//==
//==============================================================================
//
//  Parameters:
//      nMaxBuffer:
//       The value of the attribute T38FacsimilieMaxBuffer to set.
//
//  Description:
//      Sets the max buffer T38FacsimilieMaxBuffer.
//
//==============================================================================
inline void CSdpFieldAttributeT38FacsimileMaxBuffer::SetMaxBuffer(IN const int nMaxBuffer)
{
    m_nMaxBuffer = nMaxBuffer;
}

MX_NAMESPACE_END(MXD_GNS)


#endif //#if defined(MXD_SDP_ENABLE_T38_SUPPORT)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTET38FACSIMILEMAXBUFFER_H

