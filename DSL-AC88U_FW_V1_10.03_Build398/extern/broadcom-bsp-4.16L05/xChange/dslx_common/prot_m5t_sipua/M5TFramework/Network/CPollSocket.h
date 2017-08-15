//==============================================================================
//==============================================================================
//
//     Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CPOLLSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CPOLLSOCKET_H
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
#include "Config/FrameworkCfg.h"
#endif

//-- The CPollSocket class does not exist under Symbian.
//-- It is replaced by a similar CPollRequestStatus class.
#if !defined(MXD_OS_SYMBIAN) && \
     defined(MXD_NETWORK_ENABLE_SUPPORT)

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_IPOLLEDSOCKETMGR_H
#include "Network/IPolledSocketMgr.h"
#endif

#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif

#if defined ( MXD_OS_VXWORKS )
    #include <sys/times.h>
    #include <selectLib.h>
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
const unsigned int uSOCKET_ALL_EVENTS = 7;
const unsigned int uSOCKET_IN_EXCEPTION = 1;
const unsigned int uSOCKET_READABLE = 2;
const unsigned int uSOCKET_WRITABLE = 4;
//M5T_INTERNAL_USE_END
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CPollSocket
//========================================
//<GROUP NETWORK_CLASSES>
//
// Summary:
//   Class providing polled events detection on multiple sockets.
//
// Description:
//   This class provides support for polled events detection on multiple sockets
//   and notification of these events to the manager associated with the socket
//   on which the event was detected. Three different events may be detected:
//   readability, writability and in exception.
//
//   A socket must be registered before CPollSocket polls it for new events.
//   This is done with a call to RegisterSocket. The socket handle, the
//   manager and an optional opaque value must be provided.
//
//   Once registered, it is possible to enable the detection of specific events.
//   This is done with a call to EnableEventDetection.
//
//   It is also possible to disable the detection of specific events.
//   This is done with a call to DisableEventDetection.
//
//   A socket must be unregistered when events detection is no longer required.
//   This is done with a call to UnregisterSocket. The socket handle and the
//   to event to unregister must be supplied.
//
//   The method Poll must be called to poll each registered socket. The class
//   will go through its sockets identifying socket handles where events are
//   available. If an event is detected for a socket, the method Poll will
//   call IPolledSocketMgr::EvPolledSocketMgrEventDetected to notify the manager
//   about the new events that have been detected. It is possible to pass a
//   timeout to the method Poll. If the timeout is different than 0, then the
//   Poll method will block until an event is detected or the timeout is
//   reached.
//
//   This class has no thread nor concurency protection. It is the user's
//   responsability to manage that. It was decided to give this flexibility.
//   It prevents the performance costs of entering/exiting critical sections
//   systematically while there is a possibility that the user calls all
//   CPollSocket methods from the same thread.
//
//   The main reason for the presence of this class is to regroup network
//   reception for a group of sockets into the same thread instead of creating
//   sockets all having their own thread.
//
// Location:
//   Network/CPollSocket
//
// See Also:
//   IPolledSocketMgr
//
//==============================================================================
class CPollSocket
{
//M5T_INTERNAL_USE_BEGIN
private:
    // New type definition - internal use only
    struct SPolledSocket
    {
        mxt_hSocket hSocket;
        IPolledSocketMgr* pPolledSocketMgr;
        mxt_opaque opq;

        unsigned int uEventsDetected;
        unsigned int uEventsToDetect;
    };
//M5T_INTERNAL_USE_END

// Published Interface
//---------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CPollSocket();

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CPollSocket();

    // Summary:
    //  Returns the number of sockets currently registered.
    //-----------------------------------------------------
    unsigned int GetRegisteredSocketCount();

    // Summary:
    //  Adds a socket handle / Polled socket manager association in the list of
    //  polled sockets.
    //-------------------------------------------------------------------------
    mxt_result RegisterSocket(IN mxt_hSocket hSocket,
                              IN IPolledSocketMgr* pPolledSocketMgr,
                              IN mxt_opaque opq = MX_INT32_TO_OPQ(0));

    // Summary:
    //  Enables the detection of specific events for a socket.
    //--------------------------------------------------------
    mxt_result EnableEventsDetection(IN mxt_hSocket hSocket,
                                     IN unsigned int uEvents);

    // Summary:
    //  Gets the events that are currently being detected.
    //----------------------------------------------------
    mxt_result GetEventsDetectionState(IN mxt_hSocket hSocket,
                                       OUT unsigned int* puEvents);

    // Summary:
    //  Disables the detection of specific events for a socket.
    //---------------------------------------------------------
    mxt_result DisableEventsDetection(IN mxt_hSocket hSocket,
                                      IN unsigned int uEvents);

    // Summary:
    //  Removes a socket handle from the list of polled sockets.
    //----------------------------------------------------------
    mxt_result UnregisterSocket(IN mxt_hSocket hSocket,
                                OUT mxt_opaque* popq = NULL);

    // Summary:
    // This method is time critical and must be called periodically
    // to detect events on registered sockets.
    //----------------------------------------------------------------
    mxt_result Poll(IN uint64_t uTimeoutMs = 0);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
//-------------------
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------
    CPollSocket(IN const CPollSocket& rSrc);
    CPollSocket& operator=(IN const CPollSocket& rSrc);
    CPollSocket& operator==(IN const CPollSocket& rSrc);

    //-- << Helpers >>
    //-----------------
    unsigned int Find(IN const mxt_hSocket hSocket);
    mxt_hSocket FindHighestFd();

//-- Hidden Data Members
//------------------------
protected:

private:

    // The list of sockets to poll.
    CVector<SPolledSocket> m_vecstPolledSocket;

    // File descriptor information useful to select.
    fd_set m_exceptSet;
    fd_set m_readSet;
    fd_set m_writeSet;
    mxt_hSocket m_hSocketHighestFd;
    bool m_bSocketUnregistered;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//==
//==  GetRegisteredSocketCount
//==
//==============================================================================
//
//  Returns:
//      The number of registered sockets
//
//  Description:
//      Gets the number of registered sockets in the poll.
//
//==============================================================================
inline
unsigned int CPollSocket::GetRegisteredSocketCount()
{
    return m_vecstPolledSocket.GetSize();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_OS_SYMBIAN) &&
       //      defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_CPOLLSOCKET_H
