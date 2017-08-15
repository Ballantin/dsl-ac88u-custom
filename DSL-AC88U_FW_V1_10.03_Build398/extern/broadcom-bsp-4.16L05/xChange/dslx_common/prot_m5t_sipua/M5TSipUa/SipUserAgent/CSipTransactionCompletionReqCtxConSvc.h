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
#ifndef MXG_CSIPTRANSACTIONCOMPLETIONREQCTXCONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPTRANSACTIONCOMPLETIONREQCTXCONSVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_COMPLETION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_COMPLETION_SVC_SUPPORT to be able \
to use this class.
#endif

#ifndef MXG_ISIPREQCTXCONNECTIONSVC_H
#include "SipCore/ISipReqCtxConnectionSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipTransactionCompletionSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipTransactionCompletionReqCtxConSvc
//==============================================================================
//
// Description:
//   This class is the request context connection service part of the
//   CSipTransactionCompletionSvc. It monitors responses that are sent for
//   received requests.
//
// Location:
//   SipUserAgent/CSipTransactionCompletionReqCtxConSvc.h
//
// See Also:
//   CSipTransactionCompletionSvc
//
//==============================================================================
class CSipTransactionCompletionReqCtxConSvc : public ISipReqCtxConnectionSvc
{

//-- Published Interface
public:

    // Summary:
    //   Constructor.
    //----------------
    CSipTransactionCompletionReqCtxConSvc(
        IN CSipTransactionCompletionSvc* pParentSvc);

    //-- << ISipReqCtxConnectionSvc >>
    virtual void OnSendSuccess(IN ISipRequestContext& rRequestContext,
                               IN const CSipPacket& rPacketSent);

    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

//-- Hidden Methods
protected:
private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipTransactionCompletionReqCtxConSvc(IN const CSipTransactionCompletionReqCtxConSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipTransactionCompletionReqCtxConSvc& operator=(IN const CSipTransactionCompletionReqCtxConSvc& rSrc);

    // Summary:
    //  Default constructor.
    //-----------------------
    CSipTransactionCompletionReqCtxConSvc();

//-- Hidden Data Members
protected:
private:

    // Pointer to the parent service.
    //--------------------------------
    CSipTransactionCompletionSvc* m_pParentSvc;

    // Indicates that a packet has already been seen in UpdatePacket.
    //----------------------------------------------------------------
    bool m_bPacketAlreadyUpdated;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPTRANSACTIONCOMPLETIONREQCTXCONSVC_H
