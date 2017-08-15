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
#ifndef MXG_CSDPFIELDTIME_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDTIME_H

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

// Data Member
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

// Interface Realized & Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

//-- Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)



//-- Forward Declarations Inside of the Namespace
class CBlob;
//M5T_INTERNAL_USE_END


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//==  Class: CSdpFieldTime
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of the time-fields.
//
// Description:
//   This class is an abstraction of the time-fields in a SDP packet.
//   It follows in part the BNF described in RFC 2327.
//
//   "t=" fields specify the start and stop times for a conference
//   session.
//
//  RFC 2327 BNF:
//  <PRE>
//      time-fields         =     1*( "t=" start-time space stop-time
//                                *(CRLF repeat-fields) CRLF)
//                                [zone-adjustments CRLF]
//      repeat-fields       =     "r=" repeat-interval space typed-time
//                                1*(space typed-time)
//      zone-adjustments    =     time space ["-"] typed-time
//                                *(space time space ["-"] typed-time)
//      start-time          =     time | "0"
//      stop-time           =     time | "0"
//      time                =     POS-DIGIT 9*(DIGIT)
//      repeat-interval     =     typed-time
//      typed-time          =     1*(DIGIT) [fixed-len-time-unit]
//      fixed-len-time-unit =     "d" | "h" | "m" | "s"
//      space               =     " "
//      POS-DIGIT           =     "1"|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9"
//
//  </PRE>
//
//
// Location:
//   SdpParser/CSdpFieldTime.h
//
// See Also:
//
//==============================================================================
class CSdpFieldTime : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldTime();

    // Summary: Copy constructor.
    CSdpFieldTime(IN const CSdpFieldTime& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldTime();

    // Summary: Comparison Operator.
    bool operator==(IN const CSdpFieldTime& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpFieldTime& operator=(IN const CSdpFieldTime& rFrom);

    //-- Get functions

    // Summary:
    //   Gets the start time.
    const char* GetStartTime() const;

    // Summary:
    //   Gets the stop time.
    const char* GetStopTime() const;

    // Summary:
    //   Gets the vector of Repeat Time field, i.e. "r=" field.
    CVector<CString>& GetRepeatTime();
    const CVector<CString>& GetRepeatTime() const;

    // Summary:
    //   Gets the Time zone field, i.e. "z=" field.
    const CString& GetTimeZone() const;

    // Summary:
    //   Generates the data blob from the data members.
    void        Serialize(INOUT CBlob& rBlob) const;

    //-- Set functions

    // Summary:
    //   Sets the start time.
    void SetStartTime(IN const char* pszStartTime);

    // Summary:
    //   Sets the stop time.
    void SetStopTime(IN const char* pszStopTime);

    // Summary:
    //   Sets the Time zone field, i.e. "z=" field.
    void SetTimeZone(IN const char* szTimeZone);

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
    CString m_strStartTime;
    CString m_strStopTime;

    // Contains repeat fields, i.e. "r=" field.
    CVector <CString> m_vecstrRepeatTime;

    // Contains time zone field, i.e. "z=" field.
    CString m_strTimeZone;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetStartTime
//==
//==============================================================================
//
//  Returns:
//      The start-time value.
//
//  Description:
//      Returns the start-time value in the t= field.
//
//==============================================================================
inline const char*
CSdpFieldTime::GetStartTime() const
{
    return m_strStartTime.CStr();
}

//==============================================================================
//==
//==  GetStopTime
//==
//==============================================================================
//
//  Returns:
//      The stop-time value.
//
//  Description:
//      Returns the stop-time value in the t= field.
//
//==============================================================================
inline const char*
CSdpFieldTime::GetStopTime() const
{
    return m_strStopTime.CStr();
}

//==============================================================================
//==
//==  GetRepeatTime
//==
//==============================================================================
//
//  Returns:
//      The repeat-fields value.
//
//  Description:
//      Returns the repeat-fields value in the r= field.
//
//==============================================================================
inline CVector<CString>& CSdpFieldTime::GetRepeatTime()
{
    return m_vecstrRepeatTime;
}

// <COMBINE CSdpFieldTime::GetRepeatTime>
inline const CVector<CString>& CSdpFieldTime::GetRepeatTime() const
{
    return m_vecstrRepeatTime;
}

//==============================================================================
//==
//==  GetTimeZone
//==
//==============================================================================
//
//  Returns:
//      The timezone value.
//
//  Description:
//      Returns the timezone value in the z= field.
//
//==============================================================================
inline const CString& CSdpFieldTime::GetTimeZone() const
{
    return m_strTimeZone;
}

//==============================================================================
//==
//==  SetStartTime
//==
//==============================================================================
//
//  Parameters:
//    pszStartTime:
//      The start-time value.
//
//  Description:
//      Sets the start-time value in the t= field.
//
//==============================================================================
inline void CSdpFieldTime::SetStartTime(IN const char* pszStartTime)
{
    m_strStartTime = pszStartTime;
}

//==============================================================================
//==
//==  SetStopTime
//==
//==============================================================================
//
//  Parameters:
//    pszStopTime:
//      The stop-time value.
//
//  Description:
//      Sets the stop-time value in the t= field.
//
//==============================================================================
inline void CSdpFieldTime::SetStopTime(IN const char* pszStopTime)
{
    m_strStopTime = pszStopTime;
}

//==============================================================================
//==
//==  SetTimeZone
//==
//==============================================================================
//
//  Parameters:
//    szTimeZone:
//      The timezone value.
//
//  Description:
//      Sets the timezone value in the z= field.
//
//==============================================================================
inline void CSdpFieldTime::SetTimeZone(IN const char* szTimeZone)
{
    m_strTimeZone = szTimeZone;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDTIME_H
