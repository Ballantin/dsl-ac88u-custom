//==SDOC========================================================================
//==============================================================================
//
//   Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation (“Media5”)
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CTIME_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTIME_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_TIME_ENABLE_SUPPORT
#endif

#if defined(MXD_TIME_ENABLE_SUPPORT)

// Data Member
//-----------------------------------------
#ifndef MXG_CTIMEZONE_H
#include "Time/CTimeZone.h"
#endif
#ifndef MXG_CALIGNEDVARIABLESTORAGE_H
#include "Basic/CAlignedVariableStorage.h"
#endif
#ifndef MXG_CSEMAPHORE_H
#include "Kernel/CSemaphore.h"
#endif

// Interface Realized & Parent
//--------------------------------------
#include <time.h>

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace
//-----------------------------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_END

//<GROUP TIME_VARIABLES>
//
// Summary:
//  Julian representation of January 1st, 1900.
const uint32_t uJAN_1ST_1900 = 2415021;

//<GROUP TIME_VARIABLES>
//
// Summary:
//  Julian representation of January 1st, 2000.
const uint32_t uJAN_1ST_2000 = 2451545;

//<GROUP TIME_VARIABLES>
//
// Summary:
//  Number of milliseconds per day.
const uint32_t uMS_PER_DAY = (24 * 60 * 60 * 1000);

//<GROUP TIME_VARIABLES>
//
// Summary:
//  Number of seconds per day.
const uint32_t uSEC_PER_DAY = (24 * 60 * 60);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  Class: CTime
//==
//==============================================================================
//<GROUP TIME_CLASSES>
//
//  Summary:
//      Class for retrieving the date and time.
//
//  Description:
//      CTime returns the current date and time.
//
//      The default time when it is not possible to get the current date and
//      time starts at 2000 January 1ST, time 0:0:0.  Remember that this time
//      might be readjusted if you set a time zone.
//
//  Published Interface:
//
//==EDOC========================================================================
class CTime
{
// Enumeration
//-------------
public:
    //<GROUP TIME_ENUMS>
    //
    // Summary:
    //  Enumeration with the days of the week.
    enum EDayOfWeek
    {
        // Description:
        // Sunday.
        eSUNDAY,
        // Description:
        //Monday.
        eMONDAY,
        // Description:
        // Tuesday.
        eTUESDAY,
        // Description:
        // Wednesday
        eWEDNESDAY,
        // Description:
        // Thursday
        eTHURSDAY,
        // Description:
        // Friday.
        eFRIDAY,
        // Description:
        // Saturday.
        eSATURDAY
    };

// Published Interface
//-------------------------
public:
    // << Constructors / Destructors >>
    //---------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CTime(bool bPinTime = true);

    // Summary:
    //  Copy constructor.
    //--------------------
    CTime(IN const CTime& rTime);

    // Summary:
    //  Destructor.
    //---------------
    virtual ~CTime();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CTime operator=(IN const CTime& rTime);

    // Summary:
    //  Less than operator.
    //----------------------
    bool operator<(IN const CTime& rTime) const;

    // Summary:
    //  Greater than operator.
    //-------------------------
    bool operator>(IN const CTime& rTime) const;

    // Summary:
    //  Less than or equal to operator.
    //----------------------------------
    bool operator<=(IN const CTime& rTime) const;

    // Summary:
    //  Greater than or equal to.
    //----------------------------
    bool operator>=(IN const CTime& rTime) const;

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CTime& rTime) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CTime& rTime) const;

    //-----------------------------
    //  Accessor methods
    //-----------------------------

    // Summary:
    //  Gets the date.
    //-----------------
    bool GetDate(OUT uint16_t& ruYear,
                 OUT uint16_t& ruMonth,
                 OUT uint16_t& ruDay,
                 IN bool bUTC = false) const;

    // Summary:
    //  Gets the date and time.
    //--------------------------
    bool GetDateAndTime(OUT uint16_t& ruYear,
                        OUT uint16_t& ruMonth,
                        OUT uint16_t& ruDay,
                        OUT uint16_t& ruHour,
                        OUT uint16_t& ruMinute,
                        OUT uint16_t& ruSecond,
                        OUT uint16_t& ruMs,
                        IN  bool bUTC = false) const;

    // Summary:
    //  Gets the day of week.
    //------------------------
    CTime::EDayOfWeek GetDayOfWeek(IN bool bUTC) const;

    // Summary:
    //  Gets the Julian date and time.
    //---------------------------------
    bool GetJulianDateAndTime(OUT uint32_t& ruJulianDateDay,
                              OUT uint32_t& ruJulianTimeMs,
                              IN  bool bUTC = false) const;

    // Summary:
    //  Gets the time.
    //-----------------
    bool GetTime(OUT uint16_t& ruHour,
                 OUT uint16_t& ruMinute,
                 OUT uint16_t& ruSecond,
                 OUT uint16_t& ruMs,
                 IN bool bUTC = false) const;

    // Summary:
    //  Returns whether or not daylight saving time is in effect.
    //------------------------------------------------------------
    bool IsDayLightSavingInEffect() const;

    // Summary:
    //  Returns whether or not it is a leap year.
    //--------------------------------------------
    bool IsLeapYear(IN bool bUTC = false) const;

    // Summary:
    //  Pins the time in the current CTime.
    //--------------------------------------
    void PinTime();

    // Summary:
    //  Sets the date.
    //-----------------
    void SetDate(IN uint16_t uYear,
                 IN uint16_t uMonth,
                 IN uint16_t uDay,
                 IN bool bUTC = false);

    // Summary:
    //  Sets the date and time.
    //--------------------------
    void SetDateAndTime(IN uint16_t uYear,
                        IN uint16_t uMonth,
                        IN uint16_t uDay,
                        IN uint16_t uHour,
                        IN uint16_t uMinute,
                        IN uint16_t uSecond,
                        IN uint16_t uMs,
                        IN bool bUTC = false);

    // Summary:
    //  Sets the Julian date and time.
    //---------------------------------
    void SetJulianDateAndTime(IN uint32_t uJulianDateDay,
                              IN uint32_t uJulianTimeMs,
                              IN bool bUTC = false);

    // Summary:
    //  Sets the time.
    //-----------------
    void SetTime(IN uint16_t uHour,
                 IN uint16_t uMinute,
                 IN uint16_t uSecond,
                 IN uint16_t uMs,
                 IN bool bUTC = false);

    // Summary:
    //  Sets the current location's time zone.
    //-----------------------------------------
    bool SetTimeZone(IN const char* pszTimeZone);

    // static methods

    // Summary:
    //  Returns the time related to GMT in a tm structure.
    //-----------------------------------------------------
    static bool GetGmtTime(OUT struct tm& rTime);

    // Summary:
    //  Gets the system time zone.
    //-----------------------------
    static CString GetSystemTimeZone();

    // Summary:
    //  Sets the system time.
    //------------------------
    static void SetSystemTime(IN const CTime& rTime);

    // Summary:
    //  Sets the system time using the OS time of day.
    static mxt_result SetSystemTime();

    // Summary:
    //  Gets the system time using the OS time of day.
    static mxt_result GetTimeFromSystem(OUT  CTime& rTime);

    // Summary:
    //  Gets the system time using the OS time of day.
    static mxt_result GetTimeFromSystem(OUT uint16_t& ruYear,
                                        OUT uint16_t& ruMonth,
                                        OUT uint16_t& ruDay,
                                        OUT uint16_t& ruHour,
                                        OUT uint16_t& ruMinute,
                                        OUT uint16_t& ruSecond,
                                        OUT uint16_t& ruMs);

    // Summary:
    //  Sets the system time zone.
    //-----------------------------
    static bool SetSystemTimeZone(IN const char* pszTimeZone);

    // really useful methods

    // Summary:
    //  Gets the date in Julian format.
    //----------------------------------
    static uint32_t GetJulianDate(IN uint16_t uYear,
                                  IN uint16_t uMonth,
                                  IN uint16_t uDay);

    // Summary:
    //  Gets the date in Gregorian format.
    //-------------------------------------
    static void GetGregorianDate(IN uint32_t uJulian,
                                 OUT uint16_t* puYear,
                                 OUT uint16_t* puMonth,
                                 OUT uint16_t* puDay);

//M5T_INTERNAL_USE_BEGIN
private:

    static void PinTime(OUT uint32_t& ruJulianDateDay,
                        OUT uint32_t& ruJulianTimeMs,
                        OUT bool& rbDefaultTime);

    static void GetInternalTime(OUT uint32_t& ruJulianDateDay,
                                OUT uint32_t& ruJulianTimeMs,
                                OUT bool& rbDefaultTime);

    static mxt_result InitializeCTime();
    static void FinalizeCTime();

// Friends declarations
//----------------------
    friend mxt_result InitializeCTime();
    friend void FinalizeCTime();

// Private Data Members
//---------------------
private:

    CTimeZone m_timeZone;
    uint32_t m_uJulianDateDay;
    uint32_t m_uJulianTimeMs;
    bool m_bDefaultDateAndTime; // true if the time was never set

    static CAlignedVariableStorage<CBinarySemaphore> ms_internalAccessProtection;

    static CAlignedVariableStorage<CTimeZone> ms_timeZone;

    static uint32_t ms_uJulianDateDay;
    static uint32_t ms_uJulianTimeMs;
    static uint64_t ms_uLastSynchronizationTimeMs;

    static bool ms_bDefaultDateAndTime; // true if the time was never set
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  operator=
//==
//==============================================================================
//
//  Parameters:
//      rTime:
//          Reference to the Ctime object to assign.
//
//  Returns:
//      A reference to the assigned CTime.
//
//  Description:
//      Assigns the right hand CTime to the left hand one.
//
//==EDOC========================================================================
inline
CTime CTime::operator=(IN const CTime& rTime)
{
    m_uJulianDateDay = rTime.m_uJulianDateDay;
    m_uJulianTimeMs = rTime.m_uJulianTimeMs;
    m_bDefaultDateAndTime = rTime.m_bDefaultDateAndTime;
    return *this;
}

//==SDOC========================================================================
//==
//==  operator<
//==
//==============================================================================
//
//  Parameters:
//      rTime:
//          Reference to the CTime object to compare.
//
//  Returns:
//      True if the left hand CTime is less than the right hand one, false
//      otherwise.
//
//  Description:
//      Checks whether or not the left hand CTime is less than the right hand
//      one.
//
//==EDOC========================================================================
inline
bool CTime::operator<(IN const CTime& rTime) const
{
   return (m_uJulianDateDay < rTime.m_uJulianDateDay ||
           m_uJulianDateDay == rTime.m_uJulianDateDay && m_uJulianTimeMs < rTime.m_uJulianTimeMs);
}

//==SDOC========================================================================
//==
//==  operator>
//==
//==============================================================================
//
//  Parameters:
//      rTime:
//          Reference to the CTime object to compare.
//
//  Returns:
//      True if the left hand CTime is greater than the right hand one, false
//      otherwise.
//
//  Description:
//      Checks whether or not the left hand CTime is greater than the right hand
//      one.
//
//==EDOC========================================================================
inline
bool CTime::operator>(IN const CTime& rTime) const
{
   return (m_uJulianDateDay > rTime.m_uJulianDateDay ||
           m_uJulianDateDay == rTime.m_uJulianDateDay && m_uJulianTimeMs > rTime.m_uJulianTimeMs);
}

//==SDOC========================================================================
//==
//==  operator<=
//==
//==============================================================================
//
//  Parameters:
//      rTime:
//          Reference to the CTime object to compare.
//
//  Returns:
//      True if the left hand CTime is less than or equal to the right hand one,
//      false otherwise.
//
//  Description:
//      Checks whether or not the left hand CTime is less than or equal to the
//      right hand one.
//
//==EDOC========================================================================
inline
bool CTime::operator<=(IN const CTime& rTime) const
{
   return (m_uJulianDateDay < rTime.m_uJulianDateDay ||
           m_uJulianDateDay == rTime.m_uJulianDateDay && m_uJulianTimeMs <= rTime.m_uJulianTimeMs);
}

//==SDOC========================================================================
//==
//==  operator<=
//==
//==============================================================================
//
//  Parameters:
//      rTime:
//          Reference to the CTime object to compare.
//
//  Returns:
//      True if the left hand CTime is greater than or equal to the right hand
//      one, false otherwise.
//
//  Description:
//      Checks whether or not the left hand CTime is greater than or equal to
//      the right hand one.
//
//==EDOC========================================================================
inline
bool CTime::operator>=(IN const CTime& rTime) const
{
   return (m_uJulianDateDay > rTime.m_uJulianDateDay ||
           m_uJulianDateDay == rTime.m_uJulianDateDay && m_uJulianTimeMs >= rTime.m_uJulianTimeMs);
}

//==SDOC========================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//      rTime:
//          Reference to the CTime object to compare.
//
//  Returns:
//      True if both CTime objects are equal, false otherwise.
//
//  Description:
//      Checks whether or not both CTime objects are equal to one another.
//
//==EDOC========================================================================
inline
bool CTime::operator==(IN const CTime& rTime) const
{
   return (m_uJulianDateDay == rTime.m_uJulianDateDay && m_uJulianTimeMs == rTime.m_uJulianTimeMs);
}

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rTime:
//          Reference to the CTime object to compare.
//
//  Returns:
//      True if both CTime objects are different, false otherwise.
//
//  Description:
//      Checks whether or not both CTime objects are different to one another.
//
//==EDOC========================================================================
inline
bool CTime::operator!=(IN const CTime& rTime) const
{
   return !(*this == rTime);
}

//==============================================================================
//====  MEDIATRIX NAME SPACE  ==================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // MXD_TIME_ENABLE_SUPPORT
#endif // MXG_CTIME_H

