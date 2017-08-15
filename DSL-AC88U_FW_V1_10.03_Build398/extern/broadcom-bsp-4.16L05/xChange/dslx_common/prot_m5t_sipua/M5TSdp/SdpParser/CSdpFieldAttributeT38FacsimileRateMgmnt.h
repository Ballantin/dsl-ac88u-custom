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
#ifndef MXG_CSDPFIELDATTRIBUTET38FACSIMILERATEMGMNT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTET38FACSIMILERATEMGMNT_H

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


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
const char* const szLOCALTCF        = "localTcf";
const char* const szTRANSFERREDTCF  = "transferredTCF";
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CSdpFieldAttributeT38FacsimileRateMgmnt
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of an
//   attribute-t38-Facsimile-rate-mgmnt.
//
// Description:
//   This class is an abstraction of an attribute-t38-Facsimile-rate-mgmnt in a
//   SDP packet.
//
//   The parsing of this attribute-t38-Facsimile-rate-mgmnt is a specific case
//   of an attribute.  The basic BNF that an attribute can have is described in
//   CSdpFieldAttributeOther.
//
//  <PRE>
//      attribute-t38-Facsimile-rate-mgmnt = "T38FaxRateManagement:" byte-string
//      byte-string                        = 1*(0x01..0x09|0x0b|0x0c|0x0e..0xff)
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeT38FacsimileRateMgmnt.h
//
// See Also:
//
//==============================================================================
class CSdpFieldAttributeT38FacsimileRateMgmnt : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeT38FacsimileRateMgmnt();

    // Summary: Copy constructor.
    CSdpFieldAttributeT38FacsimileRateMgmnt(IN const CSdpFieldAttributeT38FacsimileRateMgmnt& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeT38FacsimileRateMgmnt();

    //  Summary:
    //   Comparison operator.
    bool     operator==(IN const CSdpFieldAttributeT38FacsimileRateMgmnt& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpFieldAttributeT38FacsimileRateMgmnt& operator=(IN const CSdpFieldAttributeT38FacsimileRateMgmnt& rFrom);

    //-- << Stereotype >>

    // Summary:
    //   Gets facsimile rate management.
    const char* GetFacsimileRateMgmnt() const;

    // Summary:
    //   Generates the data blob from the data members.
    void        Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //   Sets facsimile rate management.
    void SetFacsimileRateMgmnt(IN const char* pszFaxRate);

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
    CString m_strFaxRate;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetFacsimileRateMgmnt
//==
//==============================================================================
//
//  Returns:
//      The attribute T38FacsimileRateMgmnt.
//
//  Description:
//      Returns the T38FacsimileRateMgmnt attribute.
//
//==============================================================================
inline const char*
CSdpFieldAttributeT38FacsimileRateMgmnt::GetFacsimileRateMgmnt() const
{
    return m_strFaxRate.CStr();
}

//==============================================================================
//==
//==  SetFacsimileRateMgmnt
//==
//==============================================================================
//
//  Parameters:
//      pszFaxRate:
//       The value of the attribute T38FacsimileRateMgmnt to set.
//
//  Description:
//      Sets the T38FacsimileRateMgmnt attribute.
//
//==============================================================================
inline void
CSdpFieldAttributeT38FacsimileRateMgmnt::SetFacsimileRateMgmnt(IN const char* pszFaxRate)
{
    m_strFaxRate = pszFaxRate;
}

MX_NAMESPACE_END(MXD_GNS)


#endif //#if defined(MXD_SDP_ENABLE_T38_SUPPORT)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTET38FACSIMILERATEMGMNT_H

