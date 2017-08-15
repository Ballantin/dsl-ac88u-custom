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
#ifndef MXG_ISOCKETSERVICEMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISOCKETSERVICEMGR_H
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

//-- Data Members
//-----------------

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISocketServiceMgr
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   This is the interface that must be implemented to use ISocketService.
//
// Description:
//   This is the interface that must be implemented to use ISocketService.
//
// Location:
//   ServicingThread/ISocketServiceMgr.h
//
// See Also:
//   ISocketService
//
//==EDOC========================================================================
class ISocketServiceMgr
{
// Published Interface
//---------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvSocketServiceMgrAwaken
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager about newly detected events on a socket.
    //
    //  Parameters:
    //    hSocket:
    //      The identifier of the socket.
    //
    //    uEvents:
    //      The events detected.
    //      Possible events are:
    //          uSOCKET_IN_EXCEPTION
    //          uSOCKET_READABLE
    //          uSOCKET_WRITABLE
    //
    //    opq:
    //      The opaque value provided at registration.
    //
    //  Description:
    //      The socket is registered and the detection of one or more events is
    //      enabled. At least one of these events has just been detected. From
    //      now on, these specific events will no longer be detected. They need
    //      to be re-enabled before they may be detected again.
    //
    //      For example:
    //          A socket is registered for readability and writability
    //          detection. At one point, the manager gets notified about the
    //          socket readability. Following this notification, the manager is
    //          never notified again about the readability of the socket. It
    //          may still however be notified of the writability of the socket.
    //
    //==EDOC====================================================================
    virtual void EvSocketServiceMgrAwaken(IN mxt_hSocket hSocket,
                                          IN unsigned int uEvents,
                                          IN mxt_opaque opq) = 0;
//M5T_INTERNAL_USE_BEGIN
protected:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  ISocketServiceMgr
    //==
    //==========================================================================
    //
    // Summary:
    //  Constructor.
    //
    // Description:
    //  Constructor.
    //
    //==EDOC====================================================================
    ISocketServiceMgr() {}

    //==SDOC====================================================================
    //==
    //==  ~ISocketServiceMgr
    //==
    //==========================================================================
    //
    // Summary:
    //  Destructior.
    //
    // Description:
    //  Destructior.
    //
    //==EDOC====================================================================
    virtual ~ISocketServiceMgr() {}

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_OS_SYMBIAN) &&
       //      defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //      defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_ISOCKETSERVICEMGR_H

