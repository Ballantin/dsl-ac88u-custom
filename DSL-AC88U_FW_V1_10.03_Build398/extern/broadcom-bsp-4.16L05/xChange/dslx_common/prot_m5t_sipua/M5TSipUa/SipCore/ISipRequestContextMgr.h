//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPREQUESTCONTEXTMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPREQUESTCONTEXTMGR_H
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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT) 
#error You must define MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT to be able to use this class.
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class ISipContext;
class ISipRequestContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipRequestContextMgr
//==============================================================================
//<GROUP PackageName>
//
// Description:
//   This interface contains events to report about the status of the request
//   contexts and its transaction.
//
// Location:
//   SipCore/ISipRequestContextMgr.h
//
//==============================================================================
class ISipRequestContextMgr
{
//-- Published Interface
public:

    //==========================================================================
    //==
    //==  EvCreated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates that a transaction for the given request context has been
    //      created.
    //
    //  Parameters:
    //      pReqCtx:
    //       The request context that created a transaction.
    //
    //      pContext:
    //       The SIP context that created the request context.
    //
    //  Description:
    //      Indicates that a transaction has been created for the request context.
    //
    //  See Also:
    //      EvCompleted
    //
    //==========================================================================
    virtual void EvCreated(IN ISipRequestContext* pReqCtx, IN ISipContext* pContext) = 0;

    //==========================================================================
    //==
    //==  EvCompleted
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates that no further action is needed for the transaction of
    //      the given request context.
    //
    //  Parameters:
    //      pReqCtx:
    //       The request context of the transaction.
    //
    //      pContext:
    //       The SIP context that created the request context.
    //
    //  Description:
    //      Indicates that no further action is needed for the transaction of
    //      the given request context.  For example, a request context for a
    //      server transaction is considered as processed after having sent a
    //      2xx response.  Note that the request context and the SIP
    //      transaction will still be alive some time however, in order to
    //      catch the retransmissions.
    //
    //  See Also:
    //      EvCreated
    //
    //==========================================================================
    virtual void EvCompleted(IN ISipRequestContext* pReqCtx, IN ISipContext* pContext) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    ISipRequestContextMgr() {}
    virtual ~ISipRequestContextMgr() {}
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREQUESTCONTEXTMGR_H


