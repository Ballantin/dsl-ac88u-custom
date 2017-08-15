//==============================================================================
//==============================================================================
//
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
#ifndef MXG_IUASSPCALLEVENTINFO_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPCALLEVENTINFO_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CNameAddr;
class IUaSspCall;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IUaSspCallEventInfo);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspCallEventInfo
//==============================================================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   This interface presents IUaSspCall specific information that can be
//   reported to the application when a IUaSspCall instance handles incoming
//   requests or responses.
//
// Description:
//  This interface presents IUaSspCall specific information that can be reported
//  to the application when a IUaSspCall instance handles incoming requests or
//  responses.
//
//  This interface is queryable through the non-NULL IEComUnknown given in some
//  of the IUaSspCallMgr events. When a query for this interface on an
//  IEComUnknown interface returns NULL, it means that the event has no
//  IUaSspCall specific information to give.
//
// Location:
//   SceUaSsp/IUaSspCallEventInfo.h
//
//==============================================================================
class IUaSspCallEventInfo : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaSspCallEventInfo);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  GetReplacedCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the IUaSspCall instance that should be replaced by the
    //      IUaSspCall instance generating the event.
    //
    //  Parameters:
    //      rpReplacedCall:
    //       NULL:
    //          The call that generated the event should not replace any call.
    //
    //       non-NULL:
    //          The call that generated the event replaces rpReplacedCall.
    //
    //          rpReplacedCall will be automatically terminated and EvTerminated
    //          will be reported when the call generating the event is accepted
    //          by the implementation, using IUaSspCall::AcceptCall.
    //
    //          If the application wants the call generating the event to
    //          further progress, it must create and associate an MSP offer
    //          answer session object to the call generating the event. The MSP
    //          offer answer session object should be configured to use the same
    //          hardware resources as the ones that were used for
    //          rpReplacedCall.
    //
    //  Description:
    //      This method returns an IUaSspCall instance that is replaced by the
    //      IUaSspCall event generating the event.
    //
    //      The instance returned by this method will be automatically
    //      terminated when the IUaSspCall generating the event is accepted (i.e.
    //      when IUaSspCall::AcceptCall is called on it).
    //
    //  See Also:
    //      IUaSspCallMgr::EvCalled
    //
    //==========================================================================
    virtual void GetReplacedCall(OUT IUaSspCall*& rpReplacedCall) = 0;

    //==========================================================================
    //==
    //==  IsOptionsRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates if the event is reported for an OPTIONS request.
    //
    //  Returns:
    //      - true, the event is for an OPTIONS request.
    //      - false, the event is not for an OPTIONS request.
    //
    //  Description:
    //      This method indicates if the event is for an OPTIONS request. See
    //      the IUaSspCallMgr::EvCalled documentation for details on how to
    //      handle the event when this method returns true.
    //
    //  See Also:
    //      IUaSspCallMgr::EvCalled
    //
    //==========================================================================
    virtual bool IsOptionsRequest() const = 0;

    //==========================================================================
    //==
    //==  GetAlertInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gives access to the Alert-Info header content.
    //
    //  Returns:
    //      The Alert-Info header. NULL if here is no Alert-Info in the request.
    //      This pointer is not valid if there is no reference left on
    //      IUaSspCallEventInfo.
    //
    //  Description:
    //      Gives access to the Alert-Info header. The Alert-Info is usually
    //      used to play a distinctive ring when receiving an incoming INVITE
    //      that creates a new call.
    //
    //      Here are examples of Alert-Info headers:
    //      - Alert-Info: <file://Bellcore-dr1>
    //      - Alert-Info: <http://www.example.com/sounds/moo.wav>
    //      - Alert-Info: <sip:bob@sip.net>
    //
    //  See Also:
    //      IUaSspCallMgr::EvCalled
    //
    //==========================================================================
    virtual const CNameAddr* GetAlertInfo() const = 0;

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  SetReplacedCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an IUaSspCall instance that should be replaced by the
    //      IUaSspCall instance generating the event.
    //
    //  Parameters:
    //      pReplacedCall:
    //       NULL:
    //          The call that generated the event should not replace any call.
    //
    //       non-NULL:
    //          The call that generated the event replaces pReplacedCall.
    //
    //          pReplacedCall will be automatically terminated and EvTerminated
    //          will be reported when the call generating the event is accepted
    //          by the implementation, using IUaSspCall::AcceptCall.
    //
    //          If the application wants the call generating the event to
    //          further progress, it must create and associate an MSP offer
    //          answer session object to the call generating the event. The MSP
    //          offer answer session object should be configured to use the same
    //          hardware resources as the ones that were used for pReplacedCall.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      This method sets an IUaSspCall instance that is replaced by the
    //      IUaSspCall event generating the event.
    //
    //      The instance set by this method must be automatically terminated
    //      when the IUaSspCall generating the event is accepted (ie. when
    //      IUaSspCall::AcceptCall is called on it).
    //
    //  See Also:
    //      IUaSspCallMgr::EvCalled
    //
    //==========================================================================
    virtual void SetReplacedCall(IN IUaSspCall* pReplacedCall) = 0;

    //==========================================================================
    //==
    //==  SetIsOptionsRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allows the Call component to indicate that the event is reported
    //      for an OPTIONS request.
    //
    //  Parameters:
    //      bIsOptionsRequest:
    //          - true, the event is for an OPTIONS request.
    //          - false, the event is not for an OPTIONS request.
    //
    //  Description:
    //      This method allows the Call component to indicate that the event is
    //      reported for an OPTIONS request.
    //
    //      It is meant for internal use only, the application should not use it.
    //
    //  See Also:
    //      IUaSspCallMgr::EvCalled
    //
    //==========================================================================
    virtual void SetIsOptionsRequest(IN bool bIsOptionsRequest) = 0;

    //==========================================================================
    //==
    //==  SetAlertInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allows the Call component to indicate that a request contained an
    //      Alert-Info header.
    //
    //  Parameters:
    //      rNameAddr:
    //       The Alert-Info header.
    //
    //  Description:
    //      This method allows the Call component to indicate that a request
    //      contained an Alert-Info header.
    //
    //      It is meant for internal use only, the application should not use
    //      it.
    //
    //  See Also:
    //      IUaSspCallMgr::EvCalled
    //
    //==========================================================================
    virtual void SetAlertInfo(IN const CNameAddr& rNameAddr) = 0;
//M5T_INTERNAL_USE_END

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    IUaSspCallEventInfo() {}
    virtual ~IUaSspCallEventInfo() {}
//M5T_INTERNAL_USE_END
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPCALLEVENTINFO_H


