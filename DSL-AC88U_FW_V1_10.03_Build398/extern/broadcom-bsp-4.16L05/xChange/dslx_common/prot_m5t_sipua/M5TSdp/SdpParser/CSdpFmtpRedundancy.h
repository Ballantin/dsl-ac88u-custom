//==============================================================================
//==============================================================================
//
//   Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSDPFMTPREDUNDANCY_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFMTPREDUNDANCY_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


//-- Parameters used by Value


//-- Interface Realized and/or Parent
#ifndef MXG_CSDPFIELDATTRIBUTEFMTP_H
#include "SdpParser/CSdpFieldAttributeFmtp.h"
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
//==  Class: CSdpFmtpRedundancy
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a redundancy fmtp attribute.
//
// Description:
//   This class is an abstraction of a redundancy fmtp (RFC 2198)
//   attribute in a SDP packet.
//
//   The format of this fmtp attribute is based on the format of
//   CSdpFieldAttributeFmtp, but is more specific in its
//   format-specific-parameters.
//
//  Derived from RFC 2198 and RFC 2327:
//  <PRE>
//      redundancy-fmtp-attribute  = "fmtp:" format redundancy-parameters
//      format                     = token
//      redundancy-parameters      = format *( "/" format )
//  </PRE>
//
// Location:
//   SdpParser/CSdpFmtpRedundancy.h
//
// See Also:
//   CSdpFieldAttributeFmtp
//
//==============================================================================
class CSdpFmtpRedundancy : public CSdpFieldAttributeFmtp
{
//-- Published Interface
public:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default destructor.
    CSdpFmtpRedundancy();

    // Summary: Copy constructor.
    CSdpFmtpRedundancy(IN const CSdpFmtpRedundancy& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFmtpRedundancy();

    // Summary:
    //  Assignment operator.
    CSdpFmtpRedundancy& operator=(IN const CSdpFmtpRedundancy& rFrom);


    //-- << Public interface >>

    // Summary:
    //  Generates a copy of the current object.
    virtual GO CSdpFieldAttributeFmtp* GenerateCopy() const;

    // Summary:
    //  Returns the media formats specified in the fmtp field attribute.
    CVector<CString>& GetRedundancyMediaFormats();
    const CVector<CString>& GetRedundancyMediaFormats() const;


    // Summary:
    //  Sets this redundancy fmtp attribute to have the media format contained
    //  in both source fmtp attributes in the order found in rFrom1.
    bool MergeRedundancyFmtp(IN const CSdpFmtpRedundancy& rFrom1,
                             IN const CSdpFmtpRedundancy& rFrom2);


    //-- << CSdpFieldAttributeFmtp >>

    // Summary:
    //  Serializes the Fmtp field value in m_strValue and returns its value.
    //  Also used by Serialize to add the format-specific-parameters to the
    //  blob.
    virtual const char* GetValue() const;


    //-- << CSdpParser >>

    // Summary:
    //  Parses the data. Can return any type of EParserResult.
    virtual EParserResult Parse(INOUT const char*& rpszStartPosition,
                                OUT mxt_result& rres);

    // Summary:
    //  Validates and checks the validity of the parsed data.
    virtual bool Validate();

    // Summary:
    //  Resets the data in the parser.
    virtual void Reset();


//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:

    // Summary: Comparison operator.
    bool operator==(IN const CSdpFmtpRedundancy& rFrom) const;


    // MXI_DESIGN_NOTES (2005-07-05): this is to avoid the method to be
    // called by the application. The method could be called if the object is
    // casted into a CSdpFieldAttributeFmtp. A better way to do this would be
    // not to use CSdpFieldAttributeFmtp as basic class (a common class should
    // be the basic class of both).

    // Summary:
    //  Set the value of the Fmtp field attribute to the string.
    virtual void SetValue(IN const char* pszValue);


//-- Hidden Data Members
private:

    // This is the list of redundancy media formats that may be used in
    // redondancy.
    CVector<CString> m_vecstrRedundancyMediaFormatList;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetRedundancyMediaFormats
//==
//==============================================================================
//
//  Returns:
//      The media formats.
//
//  Description:
//      Returns the media formats specified in the fmtp field attribute.
//
//==============================================================================
inline CVector<CString>& CSdpFmtpRedundancy::GetRedundancyMediaFormats()
{
    return m_vecstrRedundancyMediaFormatList;
}

// <COMBINE CSdpFmtpRedundancy::GetRedundancyMediaFormats>
inline const CVector<CString>&
    CSdpFmtpRedundancy::GetRedundancyMediaFormats() const
{
    return m_vecstrRedundancyMediaFormatList;
}

//==============================================================================
//==
//==  GenerateCopy
//==
//==============================================================================
//
//  Returns:
//      A copy of the current object.
//
//  Description:
//      Returns a copy of the current object.
//
//  Warning:
//      Gives ownership of the new object.
//
//==============================================================================
inline GO CSdpFieldAttributeFmtp* CSdpFmtpRedundancy::GenerateCopy() const
{
    return MX_NEW(CSdpFmtpRedundancy)(*this);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFMTPREDUNDANCY_H

