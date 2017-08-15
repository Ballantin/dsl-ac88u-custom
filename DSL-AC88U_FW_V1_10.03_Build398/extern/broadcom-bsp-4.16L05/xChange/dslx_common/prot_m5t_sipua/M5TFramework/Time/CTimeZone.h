//==SDOC========================================================================
//==============================================================================
//
//   Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//   Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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


#ifndef MXG_CTIMEZONE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTIMEZONE_H
//M5T_INTERNAL_USE_END

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
#ifndef MXG_CSEMAPHORE_H
#include "Kernel/CSemaphore.h"
#endif
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

// Interface Realized & Parent
//--------------------------------------

//==============================================================================
//====  MEDIATRIX NAME SPACE  ==================================================
//==============================================================================

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace
//-----------------------------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//
//  class CTimeZone
//
//==============================================================================
//<GROUP TIME_CLASSES>
//
//  Summary:
//      Implements the conversion between time zones and the handling of
//      daylight saving time.
//
//  Description:
//      CTimeZone does the conversion between UTC time/date and other time
//      zones. It also takes care of the daylight saving time.
//
//      The CTimeZone needs to have the time zone string to do this convertion.
//      This string follows the POSIX TZ string format standard.
//
//      STDOFFSET[DLST[OFFSET][,([Jn]|M)[START[/TIME][,(J|M)[END[/TIME]]]]]
//
//      STD and DLST
//          Three or more characters that are the designation for the
//          standard (STD) or alternate daylight saving time (DLST)
//          time zone.  Only STD is required.  If DLST is not supplied,
//          the daylight saving time does not apply to the locale.
//          Lower and upper case letters are allowed.  Any characters,
//          except digits, leading colon (:), comma (,), minus (-),
//          plus (+), and ASCII NUL, are allowed.
//
//      OFFSET
//          Indicates the value to be added to the local time to arrive at
//          UTC.  The offset has the format h[h][:m[m][:s[s]]].  If no
//          offset is supplied for DLST, the alternate time is assumed
//          to be one hour ahead of standard time.  One or more digits can
//          be used; the value is always interpreted as a decimal number.
//          The hour value must be between 0 and 24.  The minute and seconds
//          values, if present, must be between 0 and 59.  If preceded by a
//          minus (-), the time zone is east of the prime meridian, otherwise
//          it is west, which can be indicated by the preceding plus sign (+)
//
//      START and END
//          Indicates when to change to and return from the daylight saving
//          time.  The START argument is the date when the change from the
//          standard to the daylight saving time occurs; the END argument is the
//          date for changing back.  If START and END are not specified, the
//          default is the US Daylight saving time start and end dates. The
//          format for start and end must be one of the following:
//
//          n, where n is the number of days since the start of the year from
//          0 to 365.  This n must contain the leap year day if the current
//          year is a leap year.  With this format, it is the user's
//          responsibility to determine all the leap year details.  This was not
//          really shown in the time zone string format at the top of the
//          header because it was simpler for the user.  If you look, you will
//          see that Jn is optional; when Jn is not there, the current case
//          applies.  It is possible to refer to February 29.
//
//          Jn, where n is the number of days since the start of the year from
//          1 to 365.  Leap days are not counted.  The TIME parameter has
//          the same format as OFFSET but there can be no leading minus (-)
//          or plus (-) sign.  If TIME is not specified, the default is
//          02:00:00.  For example, February 28 is day 59, March 1 is day 60.
//          It is impossible to refer to the occasional February 29 explicitly.
//
//          Mx[x].y.z, where x is the month, y is a week count and z is the
//          day of the week starting at 0 (Sunday).  When y is 5, it refers
//          to the last z day of month x which may occur in either the fourth
//          or fifth week.  Week 1 is the first week in which the z day occurs.
//          Day 0 is Sunday. As an example, M10.5.0 is the Sunday of the fifth
//          week of october.  As in Jn, the parameter may also be supplied.
//          If not supplied, the default is 02:00:00.
//          1 <= x <= 12, 1 <= y <= 5, 0 <= z <= 6
//
//  Published Interface:
//
//==EDOC========================================================================
class CTimeZone
{
//M5T_INTERNAL_USE_BEGIN
    struct SLimit;
    struct SM;
//M5T_INTERNAL_USE_END

// Published Interface
//-------------------------
public:
    // << Constructors / Destructors >>
    //---------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CTimeZone();

    // Summary:
    //  Constructor. Builds time zone using the string.
    //--------------------------------------------------
    CTimeZone(IN const char* pszTimeZone);

    // Summary:
    //  Copy constructor.
    //---------------
    CTimeZone(IN const CTimeZone& rTimeZone);

    // Summary:
    //  Destrcutor.
    //---------------
    virtual ~CTimeZone();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CTimeZone operator=(IN const CTimeZone& rTimeZone);

    // Summary:
    //  Gets the time zone.
    //-----------------------
    const char* GetTimeZone() const;

    // Summary:
    //  Sets the time zone.
    //-----------------------
    mxt_result SetTimeZone(IN const char* pszTimeZone);

    // Summary:
    //  Converts the time from local to UTC.
    //----------------------------------------
    void ConvertFromLocaleToUTC(IN OUT uint32_t& ruJulianDateDay,
                                IN OUT uint32_t& ruJulianTimeMs) const;

    // Summary:
    //  Converts the time from local to UTC.
    //----------------------------------------
    void ConvertFromLocaleToUTC(IN OUT uint16_t& ruYear,
                                IN OUT uint16_t& ruMonth,
                                IN OUT uint16_t& ruDay,
                                IN OUT uint16_t& ruHour,
                                IN OUT uint16_t& ruMinute,
                                IN OUT uint16_t& ruSecond,
                                IN OUT uint16_t& ruMs) const;

    // Summary:
    //  Converts the time from UTC to local.
    //----------------------------------------
    void ConvertFromUTCToLocale(IN OUT uint32_t& ruJulianDateDay,
                                IN OUT uint32_t& ruJulianTimeMs) const;

    // Summary:
    //  Converts the time from UTC to local.
    //----------------------------------------
    void ConvertFromUTCToLocale(IN OUT uint16_t& ruYear,
                                IN OUT uint16_t& ruMonth,
                                IN OUT uint16_t& ruDay,
                                IN OUT uint16_t& ruHour,
                                IN OUT uint16_t& ruMinute,
                                IN OUT uint16_t& ruSecond,
                                IN OUT uint16_t& ruMs) const;

    // Summary:
    //  Verifies if the daylight saving time is in effect.
    //-----------------------------------------------------
    bool IsDayLightSavingInEffect(IN uint32_t uJulianDateDay,
                                  IN uint32_t uJulianTimeMs,
                                  IN bool bUTC) const;

    // Summary:
    //  Verifies if the daylight saving time is in effect.
    //-----------------------------------------------------
    bool IsDayLightSavingInEffect(IN uint16_t uYear,
                                  IN uint16_t uMonth,
                                  IN uint16_t uDay,
                                  IN uint16_t uHour,
                                  IN uint16_t uMinute,
                                  IN uint16_t uSecond,
                                  IN uint16_t uMs,
                                  IN bool bUTC) const;

    // Summary:
    //  Verifies if the time zone is valid.
    //--------------------------------------
    bool IsValid();

    // Summary:
    //  Verifies if the supplied year is a leap year.
    //------------------------------------------------
    static bool IsLeapYear(IN uint16_t uYear);

//M5T_INTERNAL_USE_BEGIN
// Internal Methods Definition
//-----------------------------
protected:

    void DetermineJulianLimits(IN uint16_t uYear);

    uint32_t GetJulianDate(uint16_t uYear, SLimit& rstLimit);
    uint32_t GetJulianDateForJ0(uint16_t uYear, uint16_t uJ0);
    uint32_t GetJulianDateForJ1(uint16_t uYear, uint16_t uJ1);
    uint32_t GetJulianDateForM(uint16_t uYear, SM& rstM);

// Private Data Members
//---------------------
private:

    struct STimeZoneTime
    {
        uint16_t m_uHour;
        uint16_t m_uMinute;
        uint16_t m_uSecond;
    };
    struct STimeZone
    {
        CString m_strTimeZone;
        int32_t m_nOffsetMs; // the value in seconds to be added to the
                             // local time to arrive to UTC.  Can also be
                             // negative.
    };
    enum ELimitType
    {
        eJ0, // J0 means a day count without concern for leap days,
             // the user must take care of the leap day by himself.
        eJ1, // J1 means a day count with concern for leap days,
             // the user does not have to care about leap days, the
             // client will do it by itself.
        eM
    };
    struct SM
    {
        uint16_t m_uMonth; // 1 to 12
        uint16_t m_uWeek; // 1 to 5
        uint16_t m_uDayOfWeek; // 0 to 6, 0 being Sunday, 1 being Monday,...
    };
    struct SLimit
    {
        ELimitType m_eLimitType;

        union
        {
            uint16_t m_uJ;
            SM m_stM;
        };

        uint32_t m_uTimeLimitMs; // from the parsing

        // We require to limit when the start daylight saving time limit
        // is after than the end daylight saving time limit,
        // when the start daylight saving time limit is before the
        // end daylight saving time limit, both limits will have the same
        // value
        uint32_t m_uJulianLimit1; // determined later
        uint32_t m_uJulianLimit2; // determined later
    };
    struct SDaylightSavingTime
    {
        bool m_bValid;

        STimeZone m_stTimeZone;
        SLimit m_stStart;
        SLimit m_stEnd;

        uint16_t m_uJulianLimitsDeterminedForYear;
    };

    CBinarySemaphore m_semInternalAccessProtection;

    CString m_strTimeZone;
    STimeZone m_stStandardTime;
    SDaylightSavingTime m_stDaylightSavingTime;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetTimeZone
//==
//==============================================================================
//
//  Returns:
//      The time zone
//
//  Description:
//      Gets the time zone stored in this CTimeZone object.
//
//==EDOC========================================================================
inline
const char* CTimeZone::GetTimeZone() const
{
    return m_strTimeZone.CStr();
}


//==SDOC========================================================================
//==
//==  IsValid
//==
//==============================================================================
//
//  Returns:
//      True if the time zone is valid, false otherwise.
//
//  Description:
//      Verifies if the time zone is valid.
//
//==EDOC========================================================================
inline
bool CTimeZone::IsValid()
{
    return !m_strTimeZone.IsEmpty();
}

//==============================================================================
//====  MEDIATRIX NAME SPACE  ==================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // MXD_TIME_ENABLE_SUPPORT
#endif // MXG_CTIMEZONE_H
