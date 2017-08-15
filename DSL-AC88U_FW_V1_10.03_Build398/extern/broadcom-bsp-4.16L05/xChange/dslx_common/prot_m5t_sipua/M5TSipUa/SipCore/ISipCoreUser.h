//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPCOREUSER_H
#define MXG_ISIPCOREUSER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipCoreUser
//========================================
// <GROUP SIPCORE_CLASSES>
//
// Description:
//  This interface represents, for the SIP stack, the user of the SipCore module.
//  It is used to report the reception of incoming SIP packets, the shutdown
//  completion, and the asynchronous result of the ListenA and StopListeningA
//  methods.
//
//  This interface is implemented by the application using the M5T SIP Stack.
//
// Location:
//   SipCore/ISipCoreUser.h
//
//==EDOC========================================================================
class ISipCoreUser
{
//-- Published Interface
//------------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvOnPacketReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      A packet has been received by the stack.
    //
    //  Parameters:
    //    rPacket:
    //      The incoming packet. The event implementation must properly manage
    //      the packet's reference count.
    //
    //  Description:
    //      This event is reported to the SipCore user when a new incoming
    //      packet is received and was not handled by the lower layers of the
    //      stack.
    //
    //      Upon receiving a SIP packet, the application can choose to have the
    //      packet handled by UA or proxy service, usually based on the
    //      request-URI and the existence of a dialog for this packet.
    //      Furthermore, it can use the existing authentication service for
    //      verifying the credentials associated with the packet.
    //
    //      The following are usually handled by the lower layers of the stack:
    //          - Request retransmissions
    //          - Response retransmissions, except 200 OK to INVITEs
    //          - Incoming CANCEL requests
    //          - Incoming ACK to negative final responses
    //
    //      Upon reception of this event, the SipCore user usually implements
    //      the following steps:
    //
    //          - Authentication: The user can choose or not to verify the
    //            authentication information received with the packet. This
    //            optional step is done with the stateless digest server
    //            authentication service (ISipStatelessDigestServerAuthSvc).
    //
    //          - Existing dialog verification: If the authentication
    //            information is valid or if the user decided not to verify it,
    //            it should then check if the incoming packet matches an
    //            existing dialog. This is done with the dialog matcher list
    //            service (CSipDialogMatcherList).
    //
    //          - If the packet does not match an existing dialog, the user can
    //            choose how to handle the packet. It usually does so based on 
    //            the request-URI of incoming requests. For users that support 
    //            both proxy and UA services, it can choose to proxy the request
    //            or handle it as a UA. This decision is really application
    //            specific.
    //
    //      This event is always reported within the implementor's SipCore
    //      thread, thus it usually requires no synchronization and can be
    //      handled synchronously.
    //
    //==EDOC====================================================================
    virtual void EvOnPacketReceived(IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  EvShutdownCompleted
    //==
    //==========================================================================
    //
    //  Summary:
    //      The ISipCoreConfig shutdown process has been completed.
    //
    //  Description:
    //      The shutdown process of the ISipCoreConfig object has been completed.
    //      This means that the stack has been stopped.
    //
    //==EDOC====================================================================
    virtual void EvShutdownCompleted() = 0;

    //==SDOC====================================================================
    //==
    //==  EvCommandResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the core user of the result of an operation.
    //
    //  Parameters:
    //    res:
    //      The result code.  For further details, see the async method
    //      reporting this event upon completion or error.
    //
    //    opq:
    //      Opaque parameter given to the core config.
    //
    //  Description:
    //      This event is reported by the SIP stack to a specific core user
    //      when an operation completes, successfully or not.
    //
    //  See Also:
    //      ISipCoreConfig::ListenA, ISipCoreConfig::StopListeningA
    //
    //==EDOC====================================================================
    virtual void EvCommandResult(IN mxt_result res, IN mxt_opaque opq) = 0;


//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipCoreUser(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipCoreUser(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipCoreUser(IN const ISipCoreUser& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipCoreUser& operator=(IN const ISipCoreUser& rSrc);
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCOREUSER_H

