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
#ifndef MXG_CSDPFIELDATTRIBUTETRANSCODING_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTETRANSCODING_H

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
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif



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
//==  Class: CSdpFieldAttributeTranscoding
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of an attribute-transcoding.
//
// Description:
//   This class is an abstraction of an attribute-transcoding in a SDP packet.
//
//   This class is no longer part of T.38 and is not used by others in the
//   whole SdpParser package.  It will eventually be removed.
//
// Location:
//   SdpParser/CSdpFieldAttributeTranscoding.h
//
// See Also:
//
//==============================================================================
class CSdpFieldAttributeTranscoding : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeTranscoding();

    // Summary: Copy Constructor.
    CSdpFieldAttributeTranscoding(IN const CSdpFieldAttributeTranscoding& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeTranscoding();

    // Summary:
    //   Assignment operator.
    CSdpFieldAttributeTranscoding& operator=(IN const CSdpFieldAttributeTranscoding& rFrom);

    //-- << Stereotype >>

    // Summary:
    //   Gets the transcoding attribute.
    const char* GetTranscoding() const;

    // Summary:
    //   Generates the data blob from the data members.
    void        Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //   Sets the transcoding attribute.
    void SetTranscoding(IN const char* pszTranscoding);

    //-- Virtual functions

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
    CString m_strTranscoding;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetTranscoding
//==
//==============================================================================
//
//  Returns:
//      The transcoding attribute.
//
//  Description:
//      Returns the transcoding attribute.
//
//==============================================================================
inline const char* CSdpFieldAttributeTranscoding::GetTranscoding() const
{
    return m_strTranscoding.CStr();
}

//==============================================================================
//==
//==  SetTranscoding
//==
//==============================================================================
//
//  Parameters:
//      pszTranscoding:
//       The value of the attribute transcoding to set.
//
//  Description:
//      Sets the transcoding attribute.
//
//==============================================================================
inline void
CSdpFieldAttributeTranscoding::SetTranscoding(IN const char* pszTranscoding)
{
    m_strTranscoding = pszTranscoding;
}

MX_NAMESPACE_END(MXD_GNS)


#endif //#if defined(MXD_SDP_ENABLE_T38_SUPPORT)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTETRANSCODING_H

