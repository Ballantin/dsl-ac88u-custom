//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_CDATE_H
#define MXG_CDATE_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

//-- Parameters used by value
//-----------------------------
#ifndef MXG_CTIME_H
#include "Time/CTime.h"
#endif

//-- Data Members
//-----------------


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
// BRCM_CUSTOM - [add] Relax SIP Date check
// Flag to lax date checking to allow single digit Date format,
// as opposed to the 2-digit format required in RFC3261.
//--------------------------------------------------------------
extern bool g_bDATE_GMT_FORMAT_PER_RFC_3261;
// BRCM_CUSTOM - [end] Relax SIP Date check

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CDate
//=================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   This class represents the value of a Date header field. It corresponds to
//   the SIP-date construct of the RFC 3261 BNF.
//
// RFC 3261 BNF:
//<CODE>
//    Date          =  "Date" HCOLON SIP-date
//    SIP-date      =  rfc1123-date
//    rfc1123-date  =  wkday "," SP date1 SP time SP "GMT"
//    date1         =  2DIGIT SP month SP 4DIGIT
//                     ; day month year (e.g., 02 Jun 1982)
//    time          =  2DIGIT ":" 2DIGIT ":" 2DIGIT
//                     ; 00:00:00 - 23:59:59
//    wkday         =  "Mon" / "Tue" / "Wed"
//                     / "Thu" / "Fri" / "Sat" / "Sun"
//    month         =  "Jan" / "Feb" / "Mar" / "Apr"
//                     / "May" / "Jun" / "Jul" / "Aug"
//                     / "Sep" / "Oct" / "Nov" / "Dec"
//</CODE>
//
// Location:
//   SipParser/CDate.h
//
//==============================================================================
class CDate
{
//-- New types definition
//-------------------------
public:

    // Description:
    //  Structure that contains a date and time.
    struct SDateTime
    {
        uint8_t  m_uSec;       // Seconds after the minute - [0,59].
        uint8_t  m_uMin;       // Minutes after the hour - [0,59].
        uint8_t  m_uHour;      // Hours since midnight - [0,23].
        uint8_t  m_uDay;       // Day of the month - [1,31].
        uint8_t  m_uMonth;     // Months since January - [0,11].
        uint16_t m_uYear;      // Years.
        uint8_t  m_uDayOfWeek; // Days since Sunday - [0,6].

//M5T_INTERNAL_USE_BEGIN
        SDateTime()
        :   m_uSec(0),
            m_uMin(0),
            m_uHour(0),
            m_uDay(0),
            m_uMonth(0),
            m_uYear(0),
            m_uDayOfWeek(0)
        {};
//M5T_INTERNAL_USE_END
    };

//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CDate();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CDate();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CDate& operator=(const CDate& rSrc);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CDate& rSrc) const;

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator!=(IN const CDate& rSrc) const;

    //-- << Public Methods >>
    //--------------------------------------------

    // Summary:
    //  Provides access to the SDateTime structure.
    //----------------------------------------------
    const SDateTime& GetDateTime() const;

    // <COMBINE CDate::GetDateTime@const>
    //------------------------------------
    SDateTime& GetDateTime();

    // Summary:
    //  Gets the year.
    //----------------
    uint16_t GetYear() const;

    // Summary:
    //  Gets the month.
    //-----------------
    uint8_t GetMonth() const;

    // Summary:
    //  Gets the day.
    //---------------
    uint8_t GetDayOfMonth() const;

    // Summary:
    //  Gets the day of the week.
    //----------------------------
    CTime::EDayOfWeek GetDayOfWeek() const;

    // Summary:
    //  Gets the hours.
    //-----------------
    uint8_t GetHours() const;

    // Summary:
    //  Gets the minutes.
    //--------------------
    uint8_t GetMinutes() const;

    // Summary:
    //  Gets the seconds.
    //--------------------
    uint8_t GetSeconds() const;

    // Resets date members to initial state.
    //---------------------------------------
    void Reset();

    // Returns true if the date is already set.
    //------------------------------------------
    bool IsSet() const;

    // Summary:
    //  Parses the date.
    //-------------------
    mxt_result Parse(INOUT const char*& rpcPos);

    // Summary:
    //  Outputs the date.
    //--------------------
    virtual void Serialize(INOUT CBlob& rBlob) const;

//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Helper Methods >>
    //-------------------------

    // Summary:
    //  Parses the day.
    //------------------
    mxt_result ParseDay(INOUT const char*& rpcPos);

    // Summary:
    //  Parses the date.
    //-------------------
    mxt_result ParseDate(INOUT const char*& rpcPos);

    // Summary:
    //  Parses the time.
    //-------------------
    mxt_result ParseTime(INOUT const char*& rpcPos);

//-- Hidden Data Members
//------------------------
protected:
private:

    // Date time structure.
    //----------------------
    SDateTime m_stDateTime;

    // Days of week array.
    //---------------------
    static const char* const ms_aszDaysOfWeek[];

    // Months array.
    //---------------
    static const char* const ms_aszMonths[];

public:
    // Description:
    //  Contains the name of the GMT time zone.
    static const char* const ms_szDATE_GMT;

    // Description:
    //  Constant containing the day's name.
    static const char* const ms_szDAY_SUNDAY;
    //<COMBINE ms_szDAY_SUNDAY>
    static const char* const ms_szDAY_MONDAY;
    //<COMBINE ms_szDAY_SUNDAY>
    static const char* const ms_szDAY_TUESDAY;
    //<COMBINE ms_szDAY_SUNDAY>
    static const char* const ms_szDAY_WEDNESDAY;
    //<COMBINE ms_szDAY_SUNDAY>
    static const char* const ms_szDAY_THURSDAY;
    //<COMBINE ms_szDAY_SUNDAY>
    static const char* const ms_szDAY_FRIDAY;
    //<COMBINE ms_szDAY_SUNDAY>
    static const char* const ms_szDAY_SATURDAY;

    // Description:
    //  Constant containing the month's name.
    static const char* const ms_szMONTH_JANUARY;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_FEBRUARY;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_MARCH;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_APRIL;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_MAY;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_JUNE;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_JULY;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_AUGUST;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_SEPTEMBER;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_OCTOBER;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_NOVEMBER;
    //<COMBINE ms_szMONTH_JANUARY>
    static const char* const ms_szMONTH_DECEMBER;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  ~CDate
//==
//==============================================================================
//
//  Description:
//      destructor.
//
//==EDOC========================================================================
inline CDate::~CDate()
{
}

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rSrc:
//      Source against which to compare.
//
//  Returns:
//      True if the addr-spec parts are not equivalent.
//
//  Description:
//      Comparison operator. Compares the SDateTime struct member.
//
//==EDOC========================================================================
inline bool CDate::operator!=(IN const CDate& rSrc) const
{
    return !(*this == rSrc);
}

//==SDOC========================================================================
//==
//==  GetDateTime
//==
//==============================================================================
//
//  Returns:
//      SDateTime
//          A reference on the date time structure.
//
//  Description:
//      Provides access to the SDateTime structure.
//
//==EDOC========================================================================
inline const CDate::SDateTime& CDate::GetDateTime() const
{
    return m_stDateTime;
}

//==SDOC========================================================================
//==
//==  GetDateTime
//==
//==============================================================================
//
//  Returns:
//      A reference on the date time structure.
//
//  Description:
//      Provides access to the SDateTime structure.
//
//==EDOC========================================================================
inline CDate::SDateTime& CDate::GetDateTime()
{
    return m_stDateTime;
}

//==SDOC========================================================================
//==
//==  GetYear
//==
//==============================================================================
//
//  Returns:
//      The year value.
//
//  Description:
//      Returns the year.
//
//==EDOC========================================================================
inline uint16_t CDate::GetYear() const
{
    return m_stDateTime.m_uYear;
}

//==SDOC========================================================================
//==
//==  GetMonth
//==
//==============================================================================
//
//  Returns:
//      The month value (1 to 12).
//
//  Description:
//      Returns the month.
//
//==EDOC========================================================================
inline uint8_t CDate::GetMonth() const
{
    return m_stDateTime.m_uMonth + 1;
}

//==SDOC========================================================================
//==
//==  GetDayOfMonth
//==
//==============================================================================
//
//  Returns:
//      The day of the month value.
//
//  Description:
//      Returns the day of the month (1 to 31).
//
//==EDOC========================================================================
inline uint8_t CDate::GetDayOfMonth() const
{
    return m_stDateTime.m_uDay;
}

//==SDOC========================================================================
//==
//==  GetDayOfWeek
//==
//==============================================================================
//
//  Returns:
//      The day of the week value.
//
//  Description:
//      Returns the day of the week.
//
//==EDOC========================================================================
inline CTime::EDayOfWeek CDate::GetDayOfWeek() const
{
    return static_cast<CTime::EDayOfWeek>(m_stDateTime.m_uDayOfWeek);
}

//==SDOC========================================================================
//==
//==  GetHours
//==
//==============================================================================
//
//  Returns:
//      The hours value.
//
//  Description:
//      Returns the hours.
//
//==EDOC========================================================================
inline uint8_t CDate::GetHours() const
{
        return m_stDateTime.m_uHour;
}

//==SDOC========================================================================
//==
//==  GetMinutes
//==
//==============================================================================
//
//  Returns:
//      The minutes value.
//
//  Description:
//      Returns the minutes.
//
//==EDOC========================================================================
inline uint8_t CDate::GetMinutes() const
{
        return m_stDateTime.m_uMin;
}

//==SDOC========================================================================
//==
//==  GetSeconds
//==
//==============================================================================
//
//  Returns:
//      The seconds value.
//
//  Description:
//      Returns the seconds.
//
//==EDOC========================================================================
inline uint8_t CDate::GetSeconds() const
{
        return m_stDateTime.m_uSec;
}

//==SDOC========================================================================
//==
//==  IsSet
//==
//==============================================================================
//
//  Returns:
//      True if the date is already set.
//
//  Description:
//      Checks if the date has been set.
//
//==EDOC========================================================================
inline bool CDate::IsSet() const
{
    return (m_stDateTime.m_uYear  != 0 &&
            m_stDateTime.m_uDay   != 0);
}

MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CDATE_H
