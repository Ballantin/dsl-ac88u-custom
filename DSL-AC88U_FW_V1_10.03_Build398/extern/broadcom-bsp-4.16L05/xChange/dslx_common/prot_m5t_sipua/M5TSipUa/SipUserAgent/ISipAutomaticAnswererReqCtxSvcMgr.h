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
#ifndef MXG_ISIPAUTOMATICANSWERERREQCTXSVCMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPAUTOMATICANSWERERREQCTXSVCMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class ISipReqCtxCoreSvc;
class ISipRequestContext;
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipAutomaticAnswererReqCtxSvcMgr
//==============================================================================
//<GROUP SipUserAgent>
//
// Summary:
//   This manager reports the a result of the response sending.
//
// Description:
//  This manager can be set on CSipAutomaticAnswererReqCtxSvc. It is used to
//  report that a response has been automatically sent on the request context
//  created by CSipAutomaticAnswererReqCtxSvc.
//
// Location:
//   SipUserAgent/ISipAutomaticAnswererReqCtxSvcMgr.h
//
// See Also:
//   CSipAutomaticAnswererReqCtxSvc
//
//==============================================================================
class ISipAutomaticAnswererReqCtxSvcMgr
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  EvResponseSent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells that a response has been sent.
    //
    //  Parameters:
    //      pReqCtxCoreSvc:
    //       The request context core service that issued the response.
    //
    //       Cannot be NULL.
    //
    //      pRequestContext:
    //       The request context on which the response was sent.
    //
    //       Cannot be NULL.
    //
    //      rRequest:
    //       The request that has been answered.
    //
    //      uResponseCode:
    //       The response code used to answer the request.
    //
    //      pszReason:
    //       The reason used to answer the request.
    //
    //       Can be NULL.
    //
    //      opqTransaction:
    //       The opaque value that has been passed to the
    //       CSipAutomaticAnswererReqCtxSvc.
    //
    //  Description:
    //      This event is called when the CSipAutomaticAnswererReqCtxSvc has
    //      sent the response using the response code (and possibly the reason)
    //      provided by the user of CSipAutomaticAnswererReqCtxSvc.
    //
    //      The implementation of this event can report an event on the
    //      application or do nothing. It depends on the service that implements
    //      this event. For example, the session service should report the
    //      EvInvalidInvite event.
    //
    //==========================================================================
    virtual void EvResponseSent(IN ISipReqCtxCoreSvc* pReqCtxCoreSvc,
                                IN ISipRequestContext* pRequestContext,
                                IN const CSipPacket& rRequest,
                                IN uint16_t uResponseCode,
                                IN const char* const pszReason,
                                IN mxt_opaque opqTransaction) = 0;

    //==========================================================================
    //==
    //==  EvResponseNotSent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells that an error happened and that no response has been sent.
    //
    //  Parameters:
    //      pReqCtxCoreSvc:
    //       The request context core service that issued the response.
    //
    //       Cannot be NULL.
    //
    //      pRequestContext:
    //       The request context on which the response would have been sent.
    //
    //       Might be NULL.
    //
    //      rRequest:
    //       The request that would have been answered.
    //
    //      uResponseCode:
    //       The response code that would have been used to answer the request.
    //
    //      pszReason:
    //       The reason that would have been used to answer the request.
    //
    //       Can be NULL.
    //
    //      opqTransaction:
    //       The opaque value that has been passed to the
    //       CSipAutomaticAnswererReqCtxSvc.
    //
    //  Description:
    //      This event is called when the CSipAutomaticAnswererReqCtxSvc faced
    //      an error that prevented it to send its response.
    //
    //      If a reference is kept on this instance, it should be released.
    //
    //==========================================================================
    virtual void EvResponseNotSent(IN ISipReqCtxCoreSvc* pReqCtxCoreSvc,
                                   IN ISipRequestContext* pRequestContext,
                                   IN const CSipPacket& rRequest,
                                   IN uint16_t uResponseCode,
                                   IN const char* const pszReason,
                                   IN mxt_opaque opqTransaction) = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipAutomaticAnswererReqCtxSvcMgr(){};

    // Summary:
    //  Destructor.
    virtual ~ISipAutomaticAnswererReqCtxSvcMgr(){};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipAutomaticAnswererReqCtxSvcMgr(IN const ISipAutomaticAnswererReqCtxSvcMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipAutomaticAnswererReqCtxSvcMgr& operator=(IN const ISipAutomaticAnswererReqCtxSvcMgr& rSrc);


//-- Hidden Data Members
protected:
private:
};


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_ISIPAUTOMATICANSWERERREQCTXSVCMGR_H
