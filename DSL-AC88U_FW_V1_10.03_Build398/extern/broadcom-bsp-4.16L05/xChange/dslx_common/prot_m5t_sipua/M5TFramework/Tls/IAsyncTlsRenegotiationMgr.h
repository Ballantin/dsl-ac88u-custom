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
#ifndef MXG_IASYNCTLSRENEGOTIATIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCTLSRENEGOTIATIONMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Interface Realized and/or Parent

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncTlsRenegotiationMgr
//==============================================================================
//<GROUP TLS_CLASSES>
//
// Summary:
//    Interface that receives event notifications from the IAsyncTlsRenegotiation
//    interface.
//
// Description:
//    Interface that receives event notifications from the IAsyncTlsRenegotiation
//    interface. All events are reported asynchronously with respect to the
//    manager's execution context.
//
// Location:
//   Tls/IAsyncTlsRenegotiationMgr.h
//
// See Also:
//   IAsyncTlsRenegotiation, CAsyncTlsSocket
//
//==============================================================================
class IAsyncTlsRenegotiationMgr
{
//-- Published Interface
public:
    //==========================================================================
    //==
    //==  EvAsyncTlsRenegotiationMgrRenegotiated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the successful renegotiation.
    //
    //  Parameters:
    //    opq:
    //      Opaque value associated with the socket.
    //
    //  Description:
    //      This event is generated upon the successful completion of the
    //      renegotiation.
    //    
    //      Depending on the setting of
    //      IAsyncTlsRenegotiation::EnableAllRenegotiationNotifications, it may be
    //      reported only for manual renegotiation (explicit call to
    //      IAsyncTlsRenegotiation::RenegotiateA) or for all renegotiations.
    //
    //      Unsuccessful renegotiation attempts are reported through the
    //      IAsyncSocketMgr::EvAsyncSocketMgrErrorDetected event.
    //
    //
    //  See Also:
    //      IAsyncTlsRenegotiation::RenegotiateA,
    //      IAsyncTlsRenegotiation::EnableAllRenegotiationNotifications,
    //      IAsyncSocketMgr::EvAsyncSocketMgrErrorDetected
    //
    //==========================================================================
    virtual void EvAsyncTlsRenegotiationMgrRenegotiated(IN mxt_opaque opq) = 0;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    //==========================================================================
    //==
    //==  IAsyncTlsRenegotiationMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //    Default Constructor.
    //
    //  Description:
    //    Default Constructor.
    //
    //==========================================================================
    IAsyncTlsRenegotiationMgr() {};

    //==========================================================================
    //==
    //==  ~IAsyncTlsRenegotiationMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //    Destructor.
    //
    //  Description:
    //    Destructor.
    //
    //==========================================================================
    virtual ~IAsyncTlsRenegotiationMgr() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    IAsyncTlsRenegotiationMgr(IN const IAsyncTlsRenegotiationMgr& rSrc);
    IAsyncTlsRenegotiationMgr& operator=(IN const IAsyncTlsRenegotiationMgr& rSrc);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IASYNCTLSRENEGOTIATIONMGR_H

