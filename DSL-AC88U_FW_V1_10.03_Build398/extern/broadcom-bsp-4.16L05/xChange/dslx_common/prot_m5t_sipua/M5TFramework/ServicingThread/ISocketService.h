//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISOCKETSERVICE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISOCKETSERVICE_H
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
#include "Config/FrameworkCfg.h" // MXD_SERVICING_THREAD_ENABLE_SUPPORT
                                 // MXD_NETWORK_ENABLE_SUPPORT
#endif

#if !defined(MXD_OS_SYMBIAN) && \
     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
     defined(MXD_NETWORK_ENABLE_SUPPORT)


#ifndef MXG_CPOLLSOCKET_H
#include "Network/CPollSocket.h"
#endif

//-- Data Members
//-----------------

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class ISocketServiceMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISocketService);
//M5T_INTERNAL_USE_END
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISocketService
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   Provides support for the detection of events that occur on a socket.
//
// Description:
//   The socket service provides support for the detection of events that occur
//   on a socket. Three different types of events may be detected: socket
//   readability, socket writability, and socket exceptions.
//
//   Before receiving notifications about the detection of one or more events
//   that occur on a socket, the socket must first be registered. The
//   registration associates a manager and an optional opaque value with the
//   socket being registered.
//
//   From now on, events to be detected may then be enabled. It is also
//   possible to disable the detection of events.
//
//   The associated socket manager is notified when enabled events are
//   detected. Following notification, the detected events are automatically
//   disabled.
//
//   For example, suppose that readability and writability events detection
//   is enabled. Upon socket readability detection, the readability event
//   detection gets disabled. The writability event detection is still enabled
//   because it has not been detected.
//
//   The interface iidSocketService (ISocketService) provides methods to
//   register and unregister sockets and methods to enable and disable events
//   detection on registered sockets. The uEvents parameter is a bitfield that
//   specifies which events must be enabled or disabled.
//
//   Notifications are sent to the manager through the method
//   ISocketServiceMgr::EvSocketServiceMgrAwaken. The manager must overload this
//   method to receive notifications.
//
//   Note: This interface is not available on Symbian. Please see
//         IRequestStatusService, which must be used instead.
//
// Location:
//   ServicingThread/ISocketService.h
//
// See Also:
//   IActivationService, IMessageService, ISocketServiceMgr, ITimerService
//
//==EDOC========================================================================
class ISocketService : public IEComUnknown
{
// Published Interface
//---------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISocketService);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  RegisterSocket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers a socket.
    //
    //  Parameters:
    //    hSocket:
    //      The identifier of the socket that must be registered.
    //
    //    pManager:
    //      The manager to be notified.
    //
    //    opq:
    //      An opaque value associated with the socket.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Registers a socket. The registration associates a manager and an
    //      optional opaque value with the socket being registered.
    //
    //==EDOC====================================================================
    virtual mxt_result RegisterSocket(IN mxt_hSocket hSocket,
                                      IN ISocketServiceMgr* pManager,
                                      IN mxt_opaque opq = MX_INT32_TO_OPQ(0)) = 0;

    //==SDOC====================================================================
    //==
    //==  EnableEventsDetection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables the detection of events.
    //
    //  Parameters:
    //    hSocket:
    //      The identifier of the socket.
    //
    //    uEvents:
    //      The events that are to be enabled.
    //      Possible events are:
    //          uSOCKET_IN_EXCEPTION
    //          uSOCKET_READABLE
    //          uSOCKET_WRITABLE
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Enables the detection of specific events for a socket.
    //      Updates the events to be detected. Only events that are true are
    //      enabled. Events that are false are not disabled.
    //
    //      For example, with a new socket hSocket
    //          EnableEventDetection(hSocket, 0x06);
    //          EnableEventDetection(hSocket, 0x01);
    //      this results in all events being enabled.
    //
    //==EDOC====================================================================
    virtual mxt_result EnableEventsDetection(IN mxt_hSocket hSocket,
                                             IN unsigned int uEvents) = 0;

    //==SDOC====================================================================
    //==
    //==  DisableEventsDetection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Disables the detection of events.
    //
    //  Parameters:
    //    hSocket:
    //      The identifier of the socket.
    //
    //    uEvents:
    //      The events that are to be disabled.
    //      Possible events are:
    //          uSOCKET_IN_EXCEPTION
    //          uSOCKET_READABLE
    //          uSOCKET_WRITABLE
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Disables the detection of specific events for a socket.
    //      Updates the events to be detected. Only events that are true are
    //      disabled. Events that are false are not enabled.
    //
    //      For example, with a new socket hSocket
    //          DisableEventDetection(hSocket, 0x06);
    //          DisableEventDetection(hSocket, 0x01);
    //      this results in all events being disabled.
    //
    //==EDOC====================================================================
    virtual mxt_result DisableEventsDetection(IN mxt_hSocket hSocket,
                                              IN unsigned int uEvents) = 0;

    //==SDOC====================================================================
    //==
    //==  UnregisterSocket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Unregisters a socket.
    //
    //  Parameters:
    //    hSocket:
    //      The identifier of the socket that must be unregistered.
    //
    //    popq:
    //      On exit, contains the opaque value associated with the socket. It
    //      may be NULL if the value should just be discarded.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Unregisters a socket for states changes detection notification.
    //
    //==EDOC====================================================================
    virtual mxt_result UnregisterSocket(IN mxt_hSocket hSocket,
                                        OUT mxt_opaque* popq = NULL) = 0;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // << Constructors / Destructors >>
    //----------------------------------
    ISocketService() {}
    virtual ~ISocketService() {}
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_OS_SYMBIAN) &&
       //      defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //      defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_ISOCKETSERVICE_H

