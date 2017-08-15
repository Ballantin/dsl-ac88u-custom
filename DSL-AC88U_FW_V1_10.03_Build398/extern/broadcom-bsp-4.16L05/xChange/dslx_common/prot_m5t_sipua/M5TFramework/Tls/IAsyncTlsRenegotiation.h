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
#ifndef MXG_IASYNCTLSRENEGOTIATION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCTLSRENEGOTIATION_H
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
class IAsyncTlsRenegotiationMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncTlsRenegotiation);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncTlsRenegotiation
//==============================================================================
//<GROUP TLS_CLASSES>
//
// Summary:
//    Interface defining the methods that perform and control TLS
//    credentials renegotiation on a TLS socket.
//
// Description:
//    Interface defining the methods that perform and control TLS
//    credentials renegotiation on a TLS socket. This means performing manual
//    renegotiation, setting the parameters needed to perform automatic
//    renegotiation, and setting the manager for the interface.
//
//    Events related to this interface are reported through the
//    IAsyncTlsRenegotiationMgr interface.
//
// Location:
//   Tls/IAsyncTlsRenegotiation.h
//
// See Also:
//   IAsyncTlsRenegotiationMgr, CAsyncTlsSocket
//
//==============================================================================
class IAsyncTlsRenegotiation : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncTlsRenegotiation);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  RenegotiateA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Manually starts the renegotiation of the security parameters.
    //
    //  Parameters:
    //    uTimeoutMs:
    //      The maximum time period allowed for renegotiation. Once the timeout
    //      has elapsed, an error is triggered.
    //
    //  Returns:
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      Manually starts the renegotiation of the security parameters.
    //
    // See Also:
    //   EnableAllRenegotiationNotifications
    //
    //==========================================================================
    virtual mxt_result RenegotiateA(IN uint64_t uTimeoutMs) = 0;

    //==========================================================================
    //==
    //==  EnableAllRenegotiationNotifications
    //==
    //==========================================================================
    //
    //  Summary:
    //      Controls event notifications through the
    //      IAsyncTlsRenegotiationMgr::EvAsyncTlsRenegotiationMgrRenegotiated
    //      manager.
    //
    //  Parameters:
    //    bEnable: 
    //      - false: The event is only reported when RenegotiateA is used
    //               locally.
    //      - true: All renegotiation events are reported including:
    //              - Locally initiated manual renegotiation through RenegotiateA.
    //              - Locally initiated auto renegotiation
    //              - Any renegotiation initiated by peer

    //              
    //  Returns:
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      Controls events notifications through the
    //      IAsyncTlsRenegotiationMgr::EvAsyncTlsRenegotiationMgrRenegotiated
    //      manager when renegotiation succeeds. When disabled, only
    //      the manual renegotiation event (when RenegotiateA is used) is
    //      reported. Not calling this method has the same behavior as using
    //      it with a false argument.
    //
    // See Also:
    //   RenegotiateA
    //
    //==========================================================================
    virtual mxt_result EnableAllRenegotiationNotifications(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  SetAutoRenegotiationThresholdInByte
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the maximum number of bytes allowed between renegotiations.
    //
    //  Parameters:
    //    uByte:
    //      The maximum number of bytes that may be exchanged in one direction
    //      before automatic renegotiation starts ('0' disables automatic
    //      renegotiation).
    //
    //  Returns:
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      The user can specify the maximum number of bytes that may be
    //      received in a direction before automatic renegotiation is triggered.
    //      Set it to 0 to disable it.
    //
    // See Also:
    //   EnableAllRenegotiationNotifications
    //
    //==========================================================================
    virtual mxt_result SetAutoRenegotiationThresholdInByte(IN uint32_t uByte) = 0;

    //==========================================================================
    //==
    //==  SetAutoRenegotiationThresholdInTimeMs
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the maximum timeout allowed between renegotiations.
    //
    //  Parameters:
    //    uTimeMs:
    //      The maximum time allowed between renegotiations ('0' disables
    //      automatic renegotiation).
    //
    //  Returns:
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      The user can specify the interval after which the automatic
    //      renegotiation is triggered. Set it to 0 to disable it.
    //
    // See Also:
    //   EnableAllRenegotiationNotifications
    //
    //==========================================================================
    virtual mxt_result SetAutoRenegotiationThresholdInTimeMs(IN uint64_t uTimeMs) = 0;

    //==========================================================================
    //==
    //==  SetAutoRenegotiationTimeoutMs
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the maximum timeout to wait for renegotiation to succeed.
    //
    //  Parameters:
    //    uTimeoutMs:
    //      The maximum number of ms to wait until the renegotiation fails
    //      ('0' disables the timeout).
    //
    //  Returns:
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      The user can specify the maximum time period allowed for
    //      automatic renegotiation to succeed. Once the timeout has elapsed, an
    //      error is triggered. Set it to 0 to disable it.
    //
    //==========================================================================
    virtual mxt_result SetAutoRenegotiationTimeoutMs(IN uint64_t uTimeoutMs) = 0;

    //==========================================================================
    //==
    //==  SetAsyncTlsSocketRenegotiationMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the TLS socket's renegotiation manager.
    //
    //  Parameters:
    //    pAsyncTlsRenegotiationMgr:
    //      Socket manager to set. It cannot be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: pAsyncTlsSocketMgr is NULL.
    //      - resFE_INVALID_STATE: The TLS socket's renegotiation manager is
    //                             already set.
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      Sets the TLS socket's renegotiation manager.
    //
    //==========================================================================
    virtual mxt_result SetAsyncTlsRenegotiationMgr(IN IAsyncTlsRenegotiationMgr* pAsyncTlsRenegotiationMgr) = 0;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    //==========================================================================
    //==
    //==  IAsyncTlsRenegotiation
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
    IAsyncTlsRenegotiation() {};

    //==========================================================================
    //==
    //==  ~IAsyncTlsRenegotiation
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
    virtual ~IAsyncTlsRenegotiation() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    IAsyncTlsRenegotiation(IN const IAsyncTlsRenegotiation& rSrc);
    IAsyncTlsRenegotiation& operator=(IN const IAsyncTlsRenegotiation& rSrc);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IASYNCTLSRENEGOTIATION_H

