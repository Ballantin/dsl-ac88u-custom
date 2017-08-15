//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_ISIPCLIENTEVENTCONTROL_H
#define MXG_ISIPCLIENTEVENTCONTROL_H

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
class ISipClientTransaction;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipClientEventControl);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipClientEventControl
//========================================
// <GROUP SIPCORE_CLASSES>
//
// Summary:
//  Interface for managing UAC events, reported upon the reception of a
//  response.
//
// Description:
//  This interface is provided as a parameter to all client events reported by
//  stack services. Client events are reported upon the reception of a response
//  to an outgoing request.
//
//  It is through this interface that the manager to which the client event is
//  reported can specify how it has handled the event and how it wants the stack
//  to proceed with it.
//
//  Refer to the M5T SIP Stack Programmer's Guide for more information on how to
//  use this interface while processing client events.
//
//  This is an ECOM interface and as such it is reference counted.
//
// Location:
//   SipCore/ISipClientEventControl.h
//
// See Also:
//   ISipClientTransaction
//
//==EDOC========================================================================
class ISipClientEventControl: public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipClientEventControl);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  CallNextClientEvent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Calls the next event in the event list if any.
    //
    //  Returns:
    //       - resS_OK: Next event was successfully called.
    //
    //       - resFE_FAIL: No more event in event list.
    //
    //  Description:
    //      Calls the next event in the event list. The manager can use this
    //      method when it receives an event and it is not able to take an
    //      appropriate action on it. For instance, it could do so when the
    //      authentication service ask for credentials for a realm that is
    //      unknown to the manager. In this case, CallNextClientEvent would call
    //      the failure event on the service that issued the request and the
    //      application could then treat the response as a general failure case.
    //
    //  See Also:
    //      ClearClientEvent, ReIssueRequest
    //
    //==EDOC====================================================================
    virtual mxt_result CallNextClientEvent() = 0;


    //==SDOC====================================================================
    //==
    //==  ReIssueRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retries sending the request on a new client transaction.
    //
    //  Parameters:
    //    rpNewTransaction:
    //      The new transaction on which the retried request is being sent. Can
    //      be NULL when the request cannot be sent.
    //
    //  Returns:
    //       - resS_OK: New client transaction is being processed.
    //
    //       - resFE_FAIL: Unable to create new transaction or no final
    //         responses were yet received.
    //
    //  Description:
    //      Creates a new client transaction and use it to send the request sent
    //      on this transaction once again. The request will be sent with the
    //      same method, extra headers and content info as the first one but
    //      services state should have changed and they will modify the request
    //      accordingly. For instance, the redirection service could use a new
    //      request-URI or the authentication service could use new credentials.
    //
    //      This method can only be called when a final response has been
    //      received on the client transaction. When the manager receives a
    //      final response on a transaction, it has to decide whether it wants
    //      to use the re-issuing feature for that transaction or not. Good
    //      candidates for re-issuing are challenge for username and password
    //      known to the application, redirection, session-timer interval too
    //      short or any failure response for which the application knows how to
    //      (and wants to) change its services state so the request may succeed
    //      the next time. The manager cannot use this method if it needs to
    //      change the method, the extra headers or the content information of
    //      the request in order to make it succeed. If the manager determines
    //      that it does not need to call this method (either because it does
    //      not know how to modify state to make the request succeed, because it
    //      needs to change the method, the extra headers or the content
    //      information of the request or simply because the request already
    //      succeeded) it must call ClearClientEvent() on this transaction in
    //      order to free all resources associated with it.
    //
    //  See Also:
    //      ClearClientEvents
    //
    //==EDOC====================================================================
    virtual mxt_result ReIssueRequest(IN mxt_opaque opqTransaction,
                                      OUT ISipClientTransaction*& rpNewTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  ClearClientEvents
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the transaction that the application is not interested in
    //      further event processing.
    //
    //  Returns:
    //       - resS_OK: The event list has been cleared.
    //
    //  Description:
    //      Clears the event list and releases the resources associated
    //      with the event and possibly with the transaction.
    //
    //  See Also:
    //      ReIssueRequest, CallNextClientEvent
    //
    //==EDOC====================================================================
    virtual mxt_result ClearClientEvents() = 0;

    //==SDOC====================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the user-specified opaque parameter associated with this
    //      transaction.
    //
    //  Returns:
    //      The opaque parameter.
    //
    //  Description:
    //      Gets the user-specified opaque parameter associated with this
    //      transaction.
    //
    //      This is the opaque parameter that is passed to the function that
    //      has initiated this client transaction. The same opaque parameter can
    //      also be accessed through ISipClientTransaction::GetOpaque.
    //
    //      It can be changed after the client transaction is created by using
    //      SetOpaque or ISipClientTransaction::SetOpaque.
    //
    //  See Also:
    //      SetOpaque
    //      ISipClientTransaction::GetOpaque
    //      ISipClientTransaction::SetOpaque
    //
    //==EDOC====================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==SDOC====================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets a user-specified opaque parameter associated with this
    //      transaction.
    //
    //  Parameters:
    //    opq:
    //      The opaque value used to set the parameter.
    //
    //  Description:
    //      Sets a user-specified opaque parameter associated with this
    //      transaction.
    //
    //      This opaque parameter is the same as the one that can be configured
    //      through the ISipClientTransaction interface. Updating it from one
    //      interface impacts the opaque parameter accessible by both
    //      interfaces, as they relate to the same transaction.
    //
    //  See Also:
    //      GetOpaque
    //      ISipClientTransaction::GetOpaque
    //      ISipClientTransaction::SetOpaque
    //
    //==EDOC====================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  RemoveReIssueMessageBody
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes the CSipMessageBody that was to be sent in a re-issued
    //      request.
    //
    //  Parameters:
    //    ppMessageBody:
    //      A pointer to fill with the CSipMessageBody that was to be sent in a
    //      re-issued request if ReIssueRequest had been called.
    //
    //      Filled with NULL when there is none or when this method fails.
    //
    //      NULL if the old CSipMessageBody object has no use.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: This object is not in a state where re-issue
    //        data is stored.
    //      - resS_OK: The CSipMessageBody has been removed.
    //
    //  Description:
    //      Removes the CSipMessageBody that was to be sent in a re-issued
    //      request.
    //
    //      This method permits to remove the CSipMessageBody to send. It could
    //      be used for example to modify the CSipMessageBody and to set it
    //      again with OverrideReIssueMessageBody.
    //
    //  See Also:
    //      OverrideReIssueMessageBody, ReIssueRequest
    //
    //==========================================================================
    virtual mxt_result RemoveReIssueMessageBody(OUT GO CSipMessageBody** ppMessageBody) = 0;

    //==========================================================================
    //==
    //==  OverrideReIssueMessageBody
    //==
    //==========================================================================
    //
    //  Summary:
    //      Changes the CSipMessageBody to be sent in re-issued requests.
    //
    //  Parameters:
    //    pMessageBody:
    //      The new CSipMessageBody to sent in re-issued requests.
    //
    //      NULL to send none.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: This object is not in a state where re-issue
    //        data is stored.
    //      - resS_OK: The CSipMessageBody has been overriden.
    //
    //  Description:
    //      Permits to change the CSipMessageBody to be sent in re-issued
    //      requests.
    //
    //  See Also:
    //      RemoveReIssueMessageBody, ReIssueRequest
    //
    //==========================================================================
    virtual mxt_result OverrideReIssueMessageBody(IN TOA CSipMessageBody* pMessageBody) = 0;

    //==========================================================================
    //==
    //==  RemoveReIssueExtraHeaders
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes the extra headers that were to be sent in a re-issued
    //      request.
    //
    //  Parameters:
    //    ppExtraHeaders:
    //      A pointer to fill with the extra headers that were to be sent in a
    //      re-issued request if ReIssueRequest had been called.
    //
    //      Filled with NULL when there is none or when this method fails.
    //
    //      NULL if the old extra headers have no use.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: This object is not in a state where re-issue
    //        data is stored.
    //      - resS_OK: The extra headers have been removed.
    //
    //  Description:
    //      Removes the extra headers that were to be sent in a re-issued
    //      request.
    //
    //      This method permits to remove the extra headers to send. It could be
    //      used for example to modify the extra headers and to set it again
    //      with OverrideReIssueExtraHeaders.
    //
    //  See Also:
    //      OverrideReIssueExtraHeaders, ReIssueRequest
    //
    //==========================================================================
    virtual mxt_result RemoveReIssueExtraHeaders(OUT GO CHeaderList** ppExtraHeaders) = 0;

    //==========================================================================
    //==
    //==  OverrideReIssueExtraHeaders
    //==
    //==========================================================================
    //
    //  Summary:
    //      Changes the extra headers to be sent in re-issued requests.
    //
    //  Parameters:
    //    pExtraHeaders:
    //      The new extra headers to be sent in re-issued requests.
    //
    //      NULL to send none.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: This object is not in a state where re-issue
    //        data is stored.
    //      - resS_OK: The extra headers have been overriden.
    //
    //  Description:
    //      Permits to change the extra headers to be sent in re-issued
    //      requests.
    //
    //  See Also:
    //      RemoveReIssueExtraHeaders, ReIssueRequest
    //
    //==========================================================================
    virtual mxt_result OverrideReIssueExtraHeaders(IN TOA CHeaderList* pExtraHeaders) = 0;

// BRCM_CUSTOM - [add] Add TIP support per 3GPP TS 22.173
    //==SDOC====================================================================
    //==
    //==  GetClientEventDataOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the opaque client event data parameter.
    //
    //  Returns:
    //      The opaque parameter.
    //
    //  Description:
    //      This opaque is a pointer to the client event data class. It is used 
    //      for storage of data that is received on client events. Specifically, 
    //      this opaque is used to pass callee id information. The opaque 
    //      parameter transports information from EvAssertedIdentity to 
    //      EvSuccess.
    //
    //  See Also:
    //      BRCMClientEventData.h
    //      BRCMClientEventData.cpp
    //      SetClientEventDataOpaque
    //
    //==EDOC====================================================================
    virtual mxt_opaque GetClientEventDataOpaque() = 0;

    //==SDOC====================================================================
    //==
    //==  SetClientEventDataOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the opaque client event data parameter.
    //
    //  Parameters:
    //    opq:
    //      The opaque value used to set the parameter.
    //
    //  Description:
    //      This opaque should be a pointer to the client event data class. It is
    //      used for storage of data that is received on client events.
    //      Specifically, this opaque is used to pass callee id information. The 
    //      opaque parameter transports the data from EvAssertedIdentity to 
    //      EvSuccess.
    //
    //
    //  See Also:
    //      BRCMClientEventData.h
    //      BRCMClientEventData.cpp
    //      GetClientEventDataOpaque
    //
    //==EDOC====================================================================
    virtual void SetClientEventDataOpaque(IN mxt_opaque opq) = 0;
// BRCM_CUSTOM - [end] Add TIP support per 3GPP TS 22.173

//-- Hidden Methods
//-------------------
protected:

    //-- << Protected Constructors / Destructors >>
    //-------------------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipClientEventControl(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipClientEventControl(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipClientEventControl(IN const ISipClientEventControl& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipClientEventControl& operator=(IN const ISipClientEventControl& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCLIENTEVENTCONTROL_H

