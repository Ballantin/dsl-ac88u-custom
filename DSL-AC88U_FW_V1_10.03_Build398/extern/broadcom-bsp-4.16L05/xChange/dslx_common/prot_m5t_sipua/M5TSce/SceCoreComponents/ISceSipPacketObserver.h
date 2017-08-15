//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCESIPPACKETOBSERVER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCESIPPACKETOBSERVER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
//-------------------------
class CSipPacket;
class ISceBasicExtensionControl;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceSipPacketObserver
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines the callbacks used by the SIP Client Engine
//   components when a SIP packet is being sent or received.
//
// Description:
//  This interface defines the callbacks used by the SIP Client Engine
//  components when a SIP packet is being sent or received.
//
// Location:
//   SceCoreComponents/ISceSipPacketObserver.h
//
// See Also:
//   ISceBasicExtensionControl
//
//==============================================================================
class ISceSipPacketObserver
{
//-- Friend Declarations.
//------------------------

//-- Published Interface.
//------------------------
public:

    //==========================================================================
    //==
    //==  EvSendingRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      The component is about to send a request.
    //
    //  Parameters:
    //    pComponent:
    //      The component sending the request.
    //
    //    rPacketOut:
    //      The request that is about to be sent.
    //
    //  Description:
    //      This callback informs the user that a component is about to send a
    //      request. The callback implementation can customize the request
    //      before it is sent. This event is not called for retransmissions of a
    //      packet.
    //
    //      This event can be reported more than once for a given packet. This
    //      will happen if the request being sent is entering failover, where
    //      the request failed to be sent to an initial target and is being sent
    //      to a new one. Thus, the event is reported once for each target, as
    //      this may impact the contact and via headers used in the request.
    //
    //  See Also:
    //      EvSendingResponse
    //
    //==========================================================================
    virtual void EvSendingRequest(IN ISceBasicExtensionControl* pComponent,
                                  INOUT CSipPacket& rPacketOut) = 0;

    //==========================================================================
    //==
    //==  EvSendingResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      The component is about to send a response.
    //
    //  Parameters:
    //    pComponent:
    //      The component sending the response.
    //
    //    rPacketOut:
    //      The response that is about to be sent.
    //
    //  Description:
    //      This callback informs the user that a component is about to send a
    //      response. The callback implementation can customize the response
    //      before it is sent. This event is not called for retransmissions of a
    //      packet.
    //
    //      This event can be reported more than once for a given packet. This
    //      will happen if the response being sent is entering failover, where
    //      the response failed to be sent to an initial target and is being
    //      sent to a new one. Thus, the event is reported once for each target,
    //      as this may impact the contact used in the response.
    //
    //  See Also:
    //      EvSendingRequest
    //
    //==========================================================================
    virtual void EvSendingResponse(IN ISceBasicExtensionControl* pComponent,
                                   INOUT CSipPacket& rPacketOut) = 0;

    //==========================================================================
    //==
    //==  EvReceivedRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      The component has received a request.
    //
    //  Parameters:
    //    pComponent:
    //      The component receiving the request.
    //
    //    rPacketIn:
    //      The incoming request.
    //
    //  Description:
    //      This callback informs the user of the reception of a SIP request by
    //      the component. At the time this event is reported, the component
    //      will not have yet completely handled the request. It is possible
    //      that the request will later be automatically rejected (if malformed
    //      or unexpected), automatically accepted, or an event can be reported
    //      to the user, allowing it to accept or reject the request.
    //
    //      rPacketIn is const and must thus not be modified as it is shared
    //      across multiple layers of the SIP Client Engine.
    //
    //  See Also:
    //      EvReceivedResponse
    //
    //==========================================================================
    virtual void EvReceivedRequest(IN ISceBasicExtensionControl* pComponent,
                                   IN const CSipPacket& rPacketIn) = 0;

    //==========================================================================
    //==
    //==  EvReceivedResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      The component has received a response.
    //
    //  Parameters:
    //    pComponent:
    //      The component receiving the response.
    //
    //    rPacketIn:
    //      The incoming response.
    //
    //  Description:
    //      This callback informs the user of the reception of a SIP response by
    //      the component. At the time this event is reported, the component
    //      will not have yet completely handled the response. It is possible
    //      that the response will later be ignored (if malformed or unexpected)
    //      or be automatically handled by the component, in which case another
    //      more detailled event may or may not be reported to the user.
    //
    //      rPacketIn is const and must thus not be modified as it is shared
    //      across multiple layers of the SIP Client Engine.
    //
    //  See Also:
    //      EvReceivedRequest
    //
    //==========================================================================
    virtual void EvReceivedResponse(IN ISceBasicExtensionControl* pComponent,
                                    IN const CSipPacket& rPacketIn) = 0;


//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceSipPacketObserver() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceSipPacketObserver() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceSipPacketObserver(IN const ISceSipPacketObserver& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceSipPacketObserver& operator=(IN const ISceSipPacketObserver& rSrc);

//-- Hidden Data Members.
//------------------------
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCESIPPACKETOBSERVER_H
