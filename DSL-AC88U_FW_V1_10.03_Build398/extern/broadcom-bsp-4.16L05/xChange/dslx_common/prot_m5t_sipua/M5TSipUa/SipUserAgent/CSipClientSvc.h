//==============================================================================
//==============================================================================
//
//   Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPCLIENTSVC_H
#define MXG_CSIPCLIENTSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members


//-- Parameters used by Value

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCLIENTSVC_H
#include "SipCore/ISipClientSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class IPrivateSipContext;
class ISipRequestContext;
class ISipForkedDialogGrouperMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipClientSvc
//========================================
//
// Description:
//   This abstract base class is implemented by every core services that are
//   sending requests. Its main method <C>SendRequest</C> is called by child
//   classes in a generic way to send the requests. Note that it inherits from
//   CEComDelegatingUnknown but it is the child classes' responsibility to
//   override ECOM methods.
//
// Location:
//   SipUserAgent/CSipClientSvc.h
//
// See Also:
//   CEComDelegatingUnknown, ISipClientSvc
//
//==============================================================================
class CSipClientSvc : protected CEComDelegatingUnknown,
                      protected ISipClientSvc
{
//-- Friend Declarations


//-- Published Interface
public:

    // Summary:
    //  Replaces the Contact header(s) by a dummy Contact.
    static void ReplaceContactWithDummy(INOUT CHeaderList& rHeaderList);

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipClientSvc(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipClientSvc();


    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  GetRequestContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Obtains a request context to send a request with the specified
    //      method.
    //
    //  Parameters:
    //    opqTransaction:
    //      Opaque data set by the application that needs to accompany any
    //      feedback about this request.
    //
    //    rMethod:
    //      The method of the request.
    //
    //    rpRequestContext:
    //      A request context to send the request. It can be NULL if the service
    //      does not want to send the specified request at this moment. A
    //      reference MUST be counted for this return, that is the caller of
    //      this class releases a reference on the return when it no longer 
    //      needs it (unless the return is NULL obviously).
    //
    //  Description:
    //      This method is called by the CSipClientSvc::SendRequest
    //      implementation to obtain a request context to send a request with
    //      the specified method.
    //
    //      If the service is busy or cannot send a request with the specified
    //      method, it should return NULL. Otherwise, it should create a new
    //      request context and store it as if the request was sent directly
    //      through its interface. For instance, in the session service
    //      implementation of this method, if the rMethod is "INVITE", it should
    //      create a request context to send an INVITE request and return it
    //      unless it already has a context sending or receiving an INVITE.
    //
    //  See Also:
    //      SendRequest
    //
    //==========================================================================
    virtual void GetRequestContext(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   OUT ISipRequestContext*& rpRequestContext) = 0;

    //==========================================================================
    //==
    //==  RequestContextInError
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the child class that the request context in parameter is no
    //      longer valid.
    //
    //  Parameters:
    //    rRequestContext:
    //      The invalid request context.
    //
    //  Description:
    //      This method is called in SendRequest when GetRequestContext has
    //      returned a valid pointer but SendRequest is about to return an
    //      error. This means that the request context cannot be used to send
    //      the request for whatever reason. It could be that the transaction
    //      could not be created in the transaction manager, a connection
    //      service has forbidden the request for the specified target, or any
    //      other reason.
    //
    //      This method is useful only for child services that keep the request
    //      context they return through GetRequestContext in their internal
    //      state. This is needed because SendRequest can be called directly by
    //      another request context in the ReIssueRequest method. In this case,
    //      the child class that would keep the request context it returned in
    //      its GetRequestContext implementation would have no way to know that
    //      SendRequest returned an error and the request context must be
    //      released. This method is there only to provide that information.
    //
    //      A typical implementation would verify that the address of the 
    //      request context in parameter corresponds to a request context that 
    //      the object stores in its internal state. If it is the case, the
    //      implementation would reset that state by releasing its reference on 
    //      that context and putting the pointer value to NULL or removing it 
    //      from a list of contexts.
    //
    //  See Also:
    //      GetRequestContext
    //
    //==========================================================================
    virtual void RequestContextInError(IN ISipRequestContext& rRequestContext) = 0;

    // Summary:
    //  Gives access to the IPrivateSipContext related to this ISipClientSvc.
    virtual void GetParentSipContext(OUT IPrivateSipContext*& rpSipContext);


    //-- << IEComUnknown mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    // ISipClientSvc is not an interface that can be queried.

    //-- << ISipClientSvc methods >>
    virtual mxt_result SendRequest(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   IN TOA CHeaderList* pExtraHeaders,
                                   IN TOA CSipMessageBody* pMessageBody,
                                   IN mxt_opaque opqService,
                                   OUT ISipClientTransaction*& rpClientTransaction,
                                   OUT CSipPacket** ppPacket,
                                   IN ISipForkedDialogGrouperMgr* pGrouperMgr = NULL,
                                   IN const CToken* pCreatingDialogRequestMethod = NULL);

    virtual void ClearReIssueData(IN mxt_opaque opqService);


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipClientSvc(IN const CSipClientSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipClientSvc& operator=(IN const CSipClientSvc& rSrc);


//-- Hidden Data Members
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCLIENTSVC_H
