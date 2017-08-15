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
#ifndef MXG_CSDPFMTPTELEVENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFMTPTELEVENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members


//-- Parameters used by Value


//-- Interface Realized and/or Parent
#ifndef MXG_CSDPFIELDATTRIBUTEFMTP_H
#include "SdpParser/CSdpFieldAttributeFmtp.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// Draft-ietf-avt-rfc2833bis-09 section 2.3.1: Values range from 0-255.
const unsigned int uCSDPFMTPTELEVENT_NB_TEL_EVENTS = 256;
//M5T_INTERNAL_USE_END


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CSdpFmtpTelEvent
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a telephone-event fmtp attribute.
//
// Description:
//   This class is an abstraction of a telephone-event fmtp
//   attribute in a SDP packet as per draft-ietf-avt-rfc2833bis-09.
//
//   The format of this fmtp attribute is based on the format of
//   CSdpFieldAttributeFmtp, but is more specific in its
//   format-specific-parameters.
//
//  <PRE>
//      tel-event-fmtp-attribute    = "fmtp:" format tel-event-parameters
//      format                      = token
//      tel-event-parameters        = tel-event-param *( "," tel-event-param )
//      tel-event-param             = tel-event [ "-" tel-event ]
//      tel-event                   = 1*DIGIT
//  </PRE>
//
// Location:
//   SdpParser/CSdpFmtpTelEvent.h
//
// See Also:
//   CSdpFieldAttributeFmtp
//
//==============================================================================
class CSdpFmtpTelEvent : public CSdpFieldAttributeFmtp
{
//-- Published Interface
public:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default destructor.
    CSdpFmtpTelEvent();

    // Summary: Copy constructor.
    CSdpFmtpTelEvent(IN const CSdpFmtpTelEvent& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFmtpTelEvent();

    // Summary:
    //  Assignment operator.
    CSdpFmtpTelEvent& operator=(IN const CSdpFmtpTelEvent& rFrom);


    //-- << Public interface >>

    // Telephone event control.
    enum ETelEventGroup
    {
        // Events 0 through 15 inclusive.
        eDTMF,

        // Event 16 only.
        eFLASH,

        // Events 32 through 36 inclusive and event 49.
        eBASIC_FAXMODEM,
    };

    // Summary:
    //  Generates a copy of the current object.
    virtual GO CSdpFieldAttributeFmtp* GenerateCopy() const;

    // Summary:
    //  Verifies if the given telephone event is supported.
    bool IsTelephoneEventSupported(IN uint8_t uTelEvent) const;

    // Summary:
    //  Verifies if any telephone events are supported.
    bool IsAnyTelephoneEventSupported() const;

    // Summary:
    //  Sets support for a single telephone event.
    void SetTelEventSupport(IN uint8_t uEvent, IN bool bEnable);

    // Summary:
    //  Helper that sets support for a group of telephone events.
    void SetTelEventSupport(IN ETelEventGroup eGroup, IN bool bEnable);

    // Summary:
    //  Merges the telephone-events supported by both parties into this one.
    bool MergeTelEventFmtp(IN const CSdpFmtpTelEvent& rFrom1,
                           IN const CSdpFmtpTelEvent& rFrom2);


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
    bool operator==(IN const CSdpFmtpTelEvent& rFrom) const;


    // MXI_DESIGN_NOTES (2005-08-05): This is to avoid the method to be
    // called by the application. The method could be called if the object is
    // cast into a CSdpFieldAttributeFmtp. A better way to do this would be not
    // to use CSdpFieldAttributeFmtp as basic class (a common class should be
    // the basic class of both).

    // Summary:
    //  Set the value of the Fmtp field attribute to the string.
    virtual void SetValue(IN const char* pszValue);


//-- Hidden Data Members
private:

    // 0-based array of supported Telephone events. Each index, when set to
    // true, represents the corresponding draft-ietf-avt-rfc2833bis-09.txt
    // telephone event.
    bool* m_pabTelEvent;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  IsTelephoneEventSupported
//==
//==============================================================================
//
//  Returns:
//      true if the event is present in the fmtp attribute.
//
//      false otherwise.
//
//  Description:
//      Sees if the event is present in the fmtp attribute.
//
//==============================================================================
inline bool CSdpFmtpTelEvent::IsTelephoneEventSupported(IN uint8_t uTelEvent) const
{
    return m_pabTelEvent[uTelEvent];
}


//==============================================================================
//==
//==  SetTelEventSupport
//==
//==============================================================================
//
//  Parameters:
//    uEvent:
//      Number of the event, as per draft-ietf-avt-rfc2833bis-09.txt. Valid
//      values are in the range 0-255.
//
//    bEnable:
//      Set to true to enable event.
//
//  Description:
//      Sets support for a single telephone event.
//
//  See Also:
//      IsTelephoneEventSupported
//
//==============================================================================
inline void CSdpFmtpTelEvent::SetTelEventSupport(IN uint8_t uEvent,
                                                 IN bool bEnable)
{
    // m_pabTelEvent indexing is 0-based.
    m_pabTelEvent[uEvent] = bEnable;
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
inline GO CSdpFieldAttributeFmtp* CSdpFmtpTelEvent::GenerateCopy() const
{
    return MX_NEW(CSdpFmtpTelEvent)(*this);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFMTPTELEVENT_H

