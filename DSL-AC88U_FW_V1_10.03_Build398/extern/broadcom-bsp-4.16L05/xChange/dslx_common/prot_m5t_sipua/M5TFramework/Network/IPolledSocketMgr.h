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
#ifndef MXG_IPOLLEDSOCKETMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IPOLLEDSOCKETMGR_H
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

#if !defined(MXD_OS_SYMBIAN) && \
     defined(MXD_NETWORK_ENABLE_SUPPORT)

#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h" // for mxt_hSocket
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IPolledSocketMgr
//========================================
//<GROUP NETWORK_CLASSES>
//
// Summary:
//   Interface used for CPollSocket.
//
// Description:
//   This interface is to be used with the class CPollSocket. When registering
//   a socket to CPollSocket, the user must provide this type of interface.
//   Then, on a subsequent call to CPollSocket::Poll, this allows the class
//   CPollSocket to call back the user who registered a socket when there are
//   events detected on the socket.
//
// Location:
//   Network/IPolledSocketMgr.h
//
// See Also:
//   CPollSocket
//
//==============================================================================
class IPolledSocketMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==========================================================================
    //==
    //==  EvPolledSocketMgrEventDetected
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager about newly detected events on a socket.
    //
    //  Parameters:
    //      hSocket:
    //          The socket handle that has data ready to receive.
    //
    //      uEvents:
    //          A bit field that indicates which events were detected.
    //
    //      opq:
    //          The opaque value provided at registration.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - resS_OK
    //
    //  Description:
    //      This method is called when at least one event was detected on a
    //      socket. Three different events may be detected: readability,
    //      writability and in exception.
    //
    //      Note: the parameter hSocket is there for flexibility. It gives the
    //      user the possibility to use the same IPolledSocketMgr for a group
    //      of socket handles.
    //
    //  See Also:
    //      Network/CPollSocket.h
    //
    //
    //==========================================================================
    virtual void EvPolledSocketMgrEventDetected(IN mxt_hSocket hSocket,
                                                IN unsigned int uEvents,
                                                IN mxt_opaque opq) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //==========================================================================
    //==
    //==  IPolledSocketMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Default Constructor.
    //
    //==========================================================================
    IPolledSocketMgr() {}

    //==========================================================================
    //==
    //==  ~IPolledSocketMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==========================================================================
    virtual ~IPolledSocketMgr() {}
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_OS_SYMBIAN) &&
       //      defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_IPOLLEDSOCKETMGR_H
