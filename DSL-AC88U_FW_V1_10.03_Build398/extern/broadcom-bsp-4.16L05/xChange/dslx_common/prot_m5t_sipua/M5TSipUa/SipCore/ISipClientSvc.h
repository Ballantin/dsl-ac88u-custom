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
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPCLIENTSVC_H
#define MXG_ISIPCLIENTSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CHeaderList;
class CSipMessageBody;
class CSipPacket;
class CToken;
class ISipClientTransaction;
class ISipForkedDialogGrouperMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipClientSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipClientSvc
//========================================
//
// Description:
//   This interface is used by the ISipRequestContext to provide re-issuing
//   feature. When a core service can send requests and wants to use the
//   re-issuing feature, it must implement this interface and pass it to the
//   ISipRequestContext along with the method, extra headers, and content
//   information for the request.
//
//   Although this interface inherits from IEComUnknown, the core services that 
//   implement it should not be able to query it. This is because the interface
//   is always given to the ISipRequestContext directly by the object that
//   implements it. Furthermore, there could be many objects implementing that
//   interface aggregated in the same CSipContext and hence, only one of those
//   objects could return its interface.
//
//   The interface inherits from IEComUnknown nevertheless because the reference
//   counting mechanism is useful to the ISipRequestContext (because it
//   keeps a reference to that object for a certain time). Moreover, since this
//   interface is always implemented by core services objects, they already need
//   to implement the ECOM base class.
//
// Location:
//   SipCore/ISipClientSvc.h
//
// See Also:
//   ISipRequestContext, ISipCoreSvc
//
//==EDOC========================================================================
class ISipClientSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipClientSvc);
    //M5T_INTERNAL_USE_END

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  SendRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a request.
    //
    //  Parameters:
    //    opqTransaction:
    //      The opaque parameter given back to the application when the stack
    //      has feedback for this transaction.
    //
    //    rMethod:
    //      The method of the request to send.
    //
    //    pExtraHeaders:
    //      Additional headers to put in the request. Ownership is TAKEN. It can
    //      be NULL, which means that the request contains only headers created
    //      by the stack.
    //
    //    pMessageBody:
    //      Message-body and content related headers to put in the request.
    //      Ownership is TAKEN. It can be NULL, which means that no content is
    //      added to the request and only the Content-Length header is added as
    //      a content related header and its value is 0.
    //
    //    opqService:
    //      Opaque usable by the service sending the packet.
    //      This value is saved as part of the re-issue data and is
    //      passed back to this method if the request needs to be  
    //      re-issued. 
    //      It is possible this opaque will point to dynamically allocated
    //      memory. In this case, the memory can be freed when the 
    //      re-issue data is no longer needed. For convenience, services
    //      can depend on ClearReIssueData being called back when this
    //      happens.
    //
    //    rpClientTransaction:
    //      The interface provided to the application so it can interact with
    //      the SIP core package.
    //
    //    ppPacket:
    //      Returns the packet that was sent. NULL in case of failure.
    //      If a packet is returned, a reference is counted that must
    //      be released by the client.
    //
    //    pGrouperMgr:
    //      If the request can fork, this is the manager notified about the
    //      forking of the sent request.
    //
    //      NULL when the request cannot fork.
    //
    //    pCreatingDialogRequestMethod:
    //      This parameter is ignored when pGrouperMgr is NULL.
    //
    //      When the request can fork, this parameter tells how forking can
    //      happen.
    //
    //      When NULL, it means that forking will be detected by a response
    //      received for the request sent by this method (e.g. this is NULL for
    //      an INVITE request).
    //
    //      Otherwise, this parameter must be the name as the method on which
    //      forking can happen (e.g. this parameter points to the "NOTIFY" method
    //      when this method sends a SUBSCRIBE).
    //
    //  Returns:
    //      - resS_OK: The request is being sent. Feedback is provided as a
    //      response to the request through the service manager.
    //
    //      - resFE_FAIL: Unable to send the request. No other feedback is
    //      provided for this request.
    //
    //  Description:
    //      This method is used by the ISipRequestContext to retry the request 
    //      when ReIssueRequest is called.
    //
    //  See Also:
    //      ISipRequestContext
    //
    //==EDOC====================================================================
    virtual mxt_result SendRequest(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   IN TO CHeaderList* pExtraHeaders,
                                   IN TO CSipMessageBody* pMessageBody,
                                   IN mxt_opaque opqService,
                                   OUT ISipClientTransaction*& rpClientTransaction,
                                   OUT CSipPacket** ppPacket,
                                   IN ISipForkedDialogGrouperMgr* pGrouperMgr = NULL,
                                   IN const CToken* pCreatingDialogRequestMethod = NULL) = 0;

    //==SDOC====================================================================
    //==
    //==  ClearReIssueData
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the service that re-issue data is no longer needed.
    //
    //  Parameters:
    //    opqService:
    //      Opaque given by the service to the ISipRequestContext when 
    //      re-issue data was stored. If the opaque points to dynamicaly
    //      allocated memory, it should be freed.
    //
    //  Description:
    //      This method is used by the ISipRequestContext to inform the
    //      service that the re-issue data is no longer needed.
    //      This method is called even if opqService is NULL.
    //
    //  See Also:
    //      SendRequest, ISipRequestContext
    //
    //==EDOC====================================================================
    virtual void ClearReIssueData(IN mxt_opaque opqService) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipClientSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipClientSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipClientSvc(IN const ISipClientSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipClientSvc& operator=(IN const ISipClientSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCLIENTSVC_H

