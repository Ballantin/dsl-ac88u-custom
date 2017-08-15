//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2001 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==============================================================================
//
// Portions of this software Copyright (c) 2008-2012 Broadcom
// 
//==============================================================================
#ifndef MXG_ISIPREGISTRATIONSVC_H
#define MXG_ISIPREGISTRATIONSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT to be \
able to use this class.
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
class ISipRegistrationMgr;
class CSipHeader;
class CSipMessageBody;
class CHeaderList;
class ISipClientTransaction;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipRegistrationSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipRegistrationSvc
//=============================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The registration service offers the interface to manage registrations to a
//  SIP registrar server. It notifies its manager when one or more registered
//  contacts is about to expire or has expired.
//
//  This service monitors the expiration of the registrations that have been
//  registered through it. It will notify its manager with
//  ISipRegistrationMgr::EvExpiring a few seconds before a registration is about
//  to expire, in order to allow to refresh it before it is discarded by the
//  server. It will also report ISipRegistrationMgr::EvExpired when a
//  registration becomes invalid on the server.
//
//  The number of seconds between ISipRegistrationMgr::EvExpiring and the real
//  registration expiration can be configured through SetExpiringThreshold.
//
//  This service will not initiate registration refreshes on its own. This must
//  always be initiated by the user of this service.
//
//  This service supports first and third-party registrations. The From and To
//  headers of the registrations sent by this service are the local address and
//  remote address information configured in the ISipUserAgentSvc that is
//  attached to the same ISipContext to which the registration service is
//  attached. The registrar address is the Request-URI configured in the
//  ISipUserAgentSvc that is attached to the same ISipContext.
//
//  This service reports events to the application through the
//  ISipRegistrationMgr interface.
//
//#The ISipRegistrationSvc is an ECOM object#
//
//  The ISipRegistrationSvc is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipRegistrationSvc</B>
//
//  <B>Interface Id: IID_ISipRegistrationSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
//
//  Note:
//      There can be some confusion regarding the relation between the contact
//      URL given in the call to ISipUserAgentSvc::AddLocalContact and
//      the contacts that are added to registrations through this interface.
//      The rContactUrl parameter of ISipUserAgentSvc::AddLocalContact is not
//      used at all by the ISipRegistrationSvc, but it is still required to be
//      the valid current request-URI of the local user.
//
// Updated Behaviour:
//
//  The behaviour of the registration service has changed from MxSF v3.x.
//
//  - In v4.0, the service does not support values other than an integral number
//    of seconds in the Expires header and in the expires parameter of a Contact
//    header received from the registrar, as defined in RFC 3261.
//
//    If no valid expiration value is found in the packet received from the
//    registrar, the default value of 3600 seconds (1 hour) is taken.
//
//  - In v4.0, the service does NOT generate EvExpiring if the registrar returns
//    expiration times that are lower than the threshold.  The next event that
//    the manager receives for the corresponding registrations is EvExpired.
//
//  - In v4.0, if there are more than one of the same contact header found in
//  the success response from the registrar, only the last found is used to
//  set the expiration value of the registration.
//
// Location:
//   SipUserAgent/ISipRegistrationSvc.h
//
// See Also:
//  ISipRegistrationMgr
//
//==============================================================================
class ISipRegistrationSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipRegistrationSvc);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager of this service.
    //
    //  Parameters:
    //    pMgr:
    //      The manager. It must not be NULL.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT if pMgr is NULL.  The manager is not
    //          changed in this case.
    //
    //      resS_OK otherwise.
    //
    //  Description:
    //      Configures the manager that will receive the events from this
    //      service.
    //
    //      Note that a manager MUST be associated with this service before it
    //      is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipRegistrationMgr* pMgr) = 0;


    //  Summary:
    //    Possible threshold types.
    //
    //  Description:
    //   These are the possible threshold types that can be used to calculate
    //   the time for EvExpiring.
    enum EThresholdType
    {
        //  Description:
        //   Fixed value threshold.
        eTHRESHOLD_FIXED,

        //  Description:
        //   Dynamic value threshold.
        eTHRESHOLD_DYNAMIC
    };

    //==SDOC====================================================================
    //==
    //==  SetExpiringThreshold
    //==
    //==========================================================================
    // <FLAG Updated behavior in 4.1.4.4>
    //
    //  Summary:
    //      Sets the delay before expiration at which the EvExpiring event will
    //      fire.
    //
    //  Parameters:
    //    uDelayInS:
    //      The delay between the EvExpiring and the expiration in seconds.
    //
    //    eThresholdType:
    //      The threshold type to use. Default value is eTHRESHOLD_FIXED.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service. No manager is associated with the service if none
    //          was set with SetManager or if Clear was called on the Context.
    //
    //      resFE_FAIL if a processing error occurred.
    //
    //      resS_OK otherwise.
    //
    //  Description:
    //      This method configures the delay between when the
    //      ISipRegistrationMgr::EvExpiring is generated and the moment a
    //      binding expires. By default, this delay is 60 seconds.
    //
    //      This method affects only future registrations. This means that if
    //      there is an active registration while this method is called, the
    //      EvExpiring event is called according to the previous threshold
    //      value.
    //
    //      If the expires value is smaller than or equal to the threshold, the
    //      EvExpiring event is never called, EvExpired is instead called
    //      directly.
    //
    //      If randomized re-registration time is used, then the threshold and
    //      EThresholdType set via this method would have no effect.
    //
    // Updated Behaviour:
    //
    //      The behaviour of the registration service has changed from MxSF v3.x.
    //
    //      In v4.0, the service does NOT generate EvExpiring if the registrar
    //      returns expiration times that are lower than the threshold.  The
    //      next event that the manager will receive for the corresponding
    //      registrations is EvExpired.
    //
    //  See Also:
    //      ISipRegistrationMgr::EvExpiring
    //
    //==EDOC====================================================================
    virtual mxt_result SetExpiringThreshold(IN unsigned int uDelayInS,
                                            IN EThresholdType eThresholdType = eTHRESHOLD_FIXED) = 0;


    //==SDOC====================================================================
    //==
    //==  Add
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a REGISTER request with multiple Contact headers.
    //
    //  Parameters:
    //    pContacts:
    //      The contacts to register with the remote server.  Adding multiple
    //      contacts in one call can be done by chaining multiple contact
    //      headers in the CSipHeader object.  The raw header of all the
    //      headers in the chain is ignored and removed.
    //
    //      Ownership of the object is always taken (even if this method fails).
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership of the object is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the REGISTER request to be sent. It can be NULL.
    //      Ownership of the object is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT if the header pContacts is not a Contact
    //          header.
    //
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service, if a final response to a previous request has not
    //          been received, or if AddLocalRegistration was previously called
    //          on this service.
    //          No manager is associated with the service if none
    //          was set with SetManager or if Clear was called on the Context.
    //
    //      resFE_SIPCORE_PACKET_BLOCKED One of the service synchronously
    //           blocked the packet. No additional event will be reported.
    //
    //      resS_OK if the request was correcty sent.
    //
    //      resFE_FAIL otherwise.
    //
    //  Description:
    //      This method sends a REGISTER to create a binding on the SIP
    //      registrar server.
    //
    //      The REGISTER request contains all contact headers as provided in
    //      the pContacts parameter. No 'expires' parameter is added by this
    //      service, it is the user's responsibility to add one if needed.
    //
    //      If the request is successful (the remote server sent a 2xx
    //      response), this service monitors the registrations expiration
    //      and notifies its manager when it is about to expire or when it has
    //      actually expired. This service also updates all the registration
    //      expirations for all the contacts contained in the response and that
    //      have been registered through this service.
    //
    //      There can never be two REGISTER requests at the same time.  That
    //      means that you have to wait for a FINAL response (through
    //      ISipRegistrationMgr::EvResponse) before calling this method again.
    //      If you call this method before receiving a final response, the
    //      method fails.
    //
    //      Note that the service does NOT generate any EvExpiring event if a
    //      registration value contained in the response is shorter than the
    //      value set with SetExpiringThreshold.  The next event generated for
    //      the corresponding contact is EvExpired if it has not been
    //      registered again.
    //
    //      Also note that if there are more than one of the same contact
    //      header found in the success response from the registrar, only the
    //      last found is used to set the expiration value of the
    //      registration.
    //
    //      This method is mutually exclusive with AddLocalRegistration:
    //      only one of these can be called on any given instance of
    //      this service.
    //
    //  See Also:
    //      ISipRegistrationMgr::EvResponse
    //
    //==EDOC====================================================================
    virtual mxt_result Add(IN TO CSipHeader* pContacts,
                           IN mxt_opaque opqTransaction,
                           IN TO CHeaderList* pExtraHeaders,
                           IN TO CSipMessageBody* pMessageBody,
                           OUT ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  AddLocalRegistration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a REGISTER request with multiple Contact headers whose
    //      local address field will be filled automatically based
    //      on the actual interface used to send the packet.
    //
    //  Parameters:
    //    pContacts:
    //      The contacts to register with the remote server.  Adding multiple
    //      contacts in one call can be done by chaining multiple contact
    //      headers in the CSipHeader object.  The raw header of all the
    //      headers in the chain is ignored and removed. The local address
    //      field of all contacts is filled automatically.
    //
    //      Ownership of the object is always taken (even if this method fails).
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership of the object is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the REGISTER request to be sent. It can be NULL.
    //      Ownership of the object is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT if the header pContacts is not a Contact
    //          header.
    //
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service, if a final response to a previous request has not
    //          been received, or if Add was already called on this service.
    //          No manager is associated with the service if none
    //          was set with SetManager or if Clear was called on the Context.
    //
    //      resFE_SIPCORE_PACKET_BLOCKED One of the service synchronously
    //           blocked the packet. No additional event will be reported.
    //
    //      resS_OK if the request was correcty sent.
    //
    //      resFE_FAIL otherwise.
    //
    //  Description:
    //      This method notifies the service to take responsability
    //      for correctly updating the contacts address field.
    //      It is otherwise identical to the Add method.
    //
    //      If the address of a contact was previously set, it is overriden.
    //
    //      AddLocalRegistration is mutually exclusive with Add: only one
    //      of these methods can be called on any given instance of
    //      this service. This implies that creating locally-answered 3rd
    //      party registration is not possible with AddLocalRegistration:
    //      it is meant for true local registrations only.
    //      If both local and remote registrations must be handled
    //      simultaneously, two different instances of the service must be
    //      used and hence two different SIP contexts must be created.
    //      This is a change of behaviour from the previous stack version.
    //
    //  See Also:
    //      ISipRegistrationSvc::Add
    //
    //==EDOC====================================================================
    virtual mxt_result AddLocalRegistration(IN TO CSipHeader* pContacts,
                                            IN mxt_opaque opqTransaction,
                                            IN TO CHeaderList* pExtraHeaders,
                                            IN TO CSipMessageBody* pMessageBody,
                                            OUT ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  Remove
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes one or more registration binding from the registrar server.
    //
    //  Parameters:
    //    pContacts:
    //      The contacts to remove from the remote server.  Removing multiple
    //      contacts in one call can be done by chaining multiple contact
    //      headers in the CSipHeader object.  The raw header of all the
    //      headers in the chain is ignored and removed. It must not be NULL.
    //
    //      Ownership is always taken (even if this method fails).
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership of the object is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the REGISTER request to be sent. It can be NULL.
    //      Ownership of the object is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT if the header pContacts is not a Contact
    //          header or if pContacts is NULL.
    //
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service or if a final response to a previous request has not
    //          been received. No manager is associated with the service if none
    //          was set with SetManager or if Clear was called on the Context.
    //
    //      resFE_SIPCORE_PACKET_BLOCKED One of the service synchronously
    //           blocked the packet. No additional event will be reported.
    //
    //      resS_OK if the request was correcty sent.
    //
    //      resFE_FAIL otherwise.
    //
    //  Description:
    //      This method sends a REGISTER to remove one or more bindings from
    //      the SIP registrar server.
    //
    //      The REGISTER request contains the contact headers as provided in
    //      the pContacts parameter. This method removes all the 'expires'
    //      parameter already in the contacts header's parameter and ensures
    //      that all contact headers include an 'expires=0' parameter.
    //
    //      If the request is successful (the remote server sent a 2xx
    //      response), this service stops monitoring the registration
    //      expirations for the specified contacts, if they previously had been
    //      registered through this service. This service also updates all
    //      the registration expirations for all the contacts contained in the
    //      response, that are not included in pContacts and that have been
    //      registered through this service.
    //
    //      There can never be two REGISTER requests at the same time.  That
    //      means that you have to wait for a FINAL response (through
    //      ISipRegistrationMgr::EvResponse) before calling this method again.
    //      If you call this method before receiving a final response, the
    //      method fails.
    //
    //      Note that the service does NOT generate any EvExpiring event if a
    //      registration value contained in the response is shorter than the
    //      value set with SetExpiringThreshold.  The next event generated for
    //      the corresponding contact is EvExpired if it has not been
    //      registered again.
    //
    //      Also take note that if there are more than one of the same contact
    //      header found in the success response from the registrar, only the
    //      last found is used to set the expiration value of the
    //      registration.
    //
    //  See Also:
    //      ISipRegistrationMgr::EvResponse
    //
    //==EDOC====================================================================
    virtual mxt_result Remove(IN TO CSipHeader* pContacts,
                              IN mxt_opaque opqTransaction,
                              IN TO CHeaderList* pExtraHeaders,
                              IN TO CSipMessageBody* pMessageBody,
                              OUT ISipClientTransaction*& rpTransaction) = 0;


    //==SDOC====================================================================
    //==
    //==  Clear
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes all registration bindings from the registrar server.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership of the object is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the REGISTER request to be sent. It can be NULL.
    //      Ownership of the object is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service or if a final response to a previous request has not
    //          been received. No manager is associated with the service if none
    //          was set with SetManager or if Clear was called on the Context.
    //
    //      resFE_SIPCORE_PACKET_BLOCKED One of the service synchronously
    //           blocked the packet. No additional event will be reported.
    //
    //      resS_OK if the request was correcty sent.
    //
    //      resFE_FAIL otherwise.
    //
    //  Description:
    //      This method sends a REGISTER to remove ALL bindings from the
    //      registrar server. The REGISTER request contains a contact header
    //      with the wildcard value '*' and an Expire header with value 0, hence
    //      requesting the remote server to unbind the registration for every
    //      contact corresponding to this user.
    //
    //      If the request is successful (the remote server sent a 2xx
    //      response), this service stops monitoring the registration
    //      expirations for all the contacts registered through this service.
    //
    //      There can never be two REGISTER requests at the same time.  That
    //      means that you have to wait for a FINAL response (through
    //      ISipRegistrationMgr::EvResponse) before calling this method again.
    //      If you call this method before receiving a final response, the
    //      method fails.
    //
    //  See Also:
    //      ISipRegistrationMgr::EvResponse
    //
    //==EDOC====================================================================
    virtual mxt_result Clear(IN mxt_opaque opqTransaction,
                             IN TO CHeaderList* pExtraHeaders,
                             IN TO CSipMessageBody* pMessageBody,
                             OUT ISipClientTransaction*& rpTransaction) = 0;


    //==========================================================================
    //==
    //==  GetList
    //==
    //==========================================================================
    //
    //  Summary:
    //      Requests the list of registered contacts from the registrar server.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership of the object is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the REGISTER request to be sent. It can be NULL.
    //      Ownership of the object is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service or if a final response to a previous request was not
    //          received. No manager is associated with the service if none
    //          was set with SetManager or if Clear was called on the Context.
    //
    //      resFE_SIPCORE_PACKET_BLOCKED One of the service synchronously
    //           blocked the packet. No additional event will be reported.
    //
    //      resS_OK if the request was correcty sent.
    //
    //      resFE_FAIL otherwise.
    //
    //  Description:
    //      This method sends a REGISTER request without any contact.
    //      The response from the server, if successful, should contain a list
    //      of the currently registered contacts.
    //
    //      If the request is successful (the remote server sent a 2xx
    //      response), this service updates all the registration expirations for
    //      all the contacts contained in the response and that have been
    //      registered through this service.
    //
    //      There can never be two REGISTER requests at the same time.  That
    //      means that you have to wait for a FINAL response (through
    //      ISipRegistrationMgr::EvResponse) before calling this method again.
    //      If you call this method before receiving a final response, the
    //      method fails.
    //
    //      Note that NO EvExpiring event is generated by the service if a
    //      registration value contained in the response is shorter than the
    //      value set with SetExpiringThreshold.  The next event generated for
    //      the corresponding contact is EvExpired if it has not been
    //      registered again.
    //
    //      Also take note that if there are more than one of the same contact
    //      header found in the success response from the registrar, only the
    //      last found is used to set the expiration value of the registration.
    //
    //  See Also:
    //      ISipRegistrationMgr::EvResponse
    //
    //==========================================================================
    virtual mxt_result GetList(IN mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               OUT ISipClientTransaction*& rpTransaction) = 0;


    //==========================================================================
    //==
    //==  UpdateContact
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the contacts expiration value for one contact managed by
    //      this service.
    //
    //  Parameters:
    //      pContactHdr:
    //          The contact on which to perform the update. Note that only one
    //          contact can be given at a time. The provided CSipHeader MUST be
    //          of type eHDR_CONTACT. The ownership is taken.
    //
    //      uNewExpirationS:
    //          The new expiration value to assign to the given contact. If 0,
    //          uDEFAULT_EXPIRES_VALUE_IN_S is used.
    //
    //  Returns:
    //      - resS_OK when this method has been completed correctly.
    //      - resFE_INVALID_ARGUMENT when rpContactHdr is NULL or is not of type
    //        eHDR_CONTACT.
    //      - resFE_FAIL otherwise.
    //
    //  Description:
    //      This method updates the expiration value of the one contact given as
    //      a parameter provided that it is managed by this service. This
    //      method's behaviour regarding timers is the same as that of the Add
    //      method except that it will NEVER send a REGISTER request AND the
    //      provided contact MUST be already known by the service.
    //
    //      This method only supports integral number values of expires
    //      parameter, as defined in RFC 3261.
    //
    //      Timers associated with this contact will be restarted appropriately.
    //      If the new expiration value is below the expiring threshold, the
    //      EvExpiring event will not be reported.
    //
    //      The expected use case of this method is a contact expiration update
    //      after reception of a shortened reg-info event.
    //
    //  See also:
    //      Add,
    //      ISipRegistrationMgr::EvExpiring, ISipRegistrationMgr::EvExpired
    //
    //==========================================================================
    virtual mxt_result UpdateContact(IN TO CSipHeader* pContactHdr,
                                     IN unsigned int uNewExpirationS) = 0;

    // Summary:
    //  This defines the matching mode for contacts received from the server.
    //
    // Description:
    //  RFC 3261 states that contacts received from the registrar in a response
    //  MUST match the contacts sent in the REGISTER request as per Contact
    //  header matching rules.
    //
    //  However, this is sometimes too strict and the ISipRegisterSvc allows an
    //  application to loosen the matching rules by using one or more of the
    //  values in the EContactMatchingType enumeration.
    //
    //  This enumeration is a bitset. Values can be combined to achieve the
    //  required behaviour. Note that there is no point in combining any values
    //  with eMATCH_URI since it already checks everything.
    //
    // #Default Behaviour:#
    //  The matching default behaviour is eMATCH_USERNAME | eMATCH_HOST_PORT
    //  combination. This provides a looser matching than specified in RFC 3261.
    //
    enum EContactMatchingType
    {
        // Description:
        //  Match the complete contact's URI including any URI parameters,
        //  if any as per RFC3261, sections "10.2.4 Refreshing Bindings" and
        //  "19.1.4 URI Comparison".
        //
        //  This type of matching, is incompatible with GRUU since the server
        //  MAY add GRUU specific information to the username.
        eMATCH_URI = 0x1,

        // Description:
        //  Match only the host port part of the contact's SIP URI.
        eMATCH_HOST_PORT = 0x2,

        // Description:
        //  Match only the username of the contact's SIP URI.
        //
        //  This type of matching, if used alone, will be incompatible with GRUU
        //  since the server MAY add GRUU specific information to the username.
        eMATCH_USERNAME = 0x4,

        // Description:
        //  Match the default (absent) port to the numeric value.
        //  This matches Test@example.com to Test@example.com:5060 only usable 
        //  when either eMATCH_URI is defined or when eMATCH_HOST_PORT is 
        //  defined. If only eMATCH_USERNAME is defined then this has no effect
        // as the port is not verified in this configuration.
        eMATCH_DEFAULT_PORT = 0x8,
    };


    //==============================================================================
    //==
    //==  SetContactMatchingType
    //==
    //==============================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Sets the contact's matching behaviour.
    //
    //  Parameters:
    //      uContactMatchingType:
    //       One or more of the EContactMatchingType enumeration values to
    //       describe the expected behaviour.
    //
    //  Returns:
    //      - Success: The matching type has been modified.
    //      - Failure: The requested matching type is invalid.
    //
    //  Description:
    //      Sets the matching behaviour for contacts received in responses from
    //      the server. See the EContactMatchingType enumeration documentation
    //      for details.
    //
    //  #Backward Compatibility Note:#
    //      Applications that use the SetIgnoreContactParamInSuccessResponse are
    //      encouraged to use this new method instead. In order to obtain the
    //      same behaviour as for the:
    //
    //      - SetIgnoreContactParamInSuccessResponse(false) call, the
    //        application will use SetContactMatchingType(eMATCH_URI);
    //      - SetIgnoreContactParamInSuccessResponse(true) call, the application
    //        will use SetContactMatchingType(eMATCH_USERNAME|eMATCH_HOST_PORT).
    //
    //  See also:
    //      GetContactMatchingType, EContactMatchingType
    //
    //==============================================================================
    virtual mxt_result SetContactMatchingType(IN unsigned int uContactMatchingType) = 0;

    //==============================================================================
    //==
    //==  GetContactMatchingType
    //==
    //==============================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Returns the contact's matching behaviour.
    //
    //  Returns:
    //      An EContactMatchingType enumeration that specifies the current
    //      contact matching behaviour.
    //
    //  Description:
    //      Returns the matching behaviour for contacts received in responses
    //      from the server. See the EContactMatchingType enumeration
    //      documentation for details.
    //
    //  See also:
    //      SetContactMatchingType, EContactMatchingType
    //
    //==============================================================================
    virtual unsigned int GetContactMatchingType() const = 0;

    //==============================================================================
    //==
    //==  SetRandomReReg
    //==
    //==============================================================================
    //
    //  Summary:
    //      Sets the use of randomized re-registration time
    //
    //  Parameters:
    //      bRandomReReg:
    //
    //  Returns:
    //      - Success: The use of randomized re-registration time has been modified
    //
    //  Description:
    //      If enabled, re-registration attempt will use randomly selected 
    //      values between 60% and 90% of the registration expiration time 
    //      as returned in the previous 200 OK
    //      Note when randomized re-registration time is used, the threshold and
    //      EThresholdType set via SetExpiringThreshold has no effect.
    //
    //==============================================================================
    virtual mxt_result SetRandomReReg(IN bool bRandomReReg) = 0;

    //==============================================================================
    //==
    //==  SetIgnoreContactParamInSuccessResponse
    //==
    //==============================================================================
    // <FLAG Deprecated in 4.1.8>
    //
    //  Parameters:
    //      bIgnoreParam:
    //       True to ignore parameters added in the contact of the REGISTER's
    //       response. False otherwise.
    //
    //  Description:
    //      Parameters added in response to the contact are usually ignored as
    //      stated in section 19.1.4 of RFC3261. The exceptions are the maddr,
    //      transport, user, ttl and method parameters.
    //      If true is passed in parameter, no parameter will be validated
    //      in the contact returned in the REGISTER's response, even the
    //      parameters of the exception. If false, the parameter will be
    //      validated as stated in RFC3261.
    //
    //      This method is now deprecated in favor of the more versatile
    //      SetContactMatchingType and GetContactMatchingType methods.
    //
    //  See also:
    //      SetContactMatchingType, GetContactMatchingType
    //
    //==============================================================================
    virtual void SetIgnoreContactParamInSuccessResponse(IN bool bIgnoreParam) = 0;

    //==========================================================================
    //==
    //==  SetRegId
    //==
    //==========================================================================
    // <FLAG New in 4.1.6>
    //
    //  Parameters:
    //      uRegId:
    //       The reg-id param used when sending a REGISTER request over an
    //       outbound connection.
    //
    //  Description:
    //      Sets the reg-id to add in the contact headers of REGISTER requests
    //      when sent over an outbound connection. There is one reg-id per
    //      outbound persistent connection.
    //
    //      This value is only added to Contacts when the REGISTER is sent over
    //      an outbound persistent connection.
    //
    //  See Also:
    //      ISipOutboundConnectionSvc
    //
    //==========================================================================
    virtual void SetRegId(IN unsigned int uRegId) = 0;

    //==========================================================================
    //==
    //==  GetRegId
    //==
    //==========================================================================
    // <FLAG New in 4.1.6>
    //
    //  Returns:
    //      The reg-id parameter.
    //
    //  Description:
    //      Gets the reg-id to add in the contact headers of REGISTER requests
    //      when sent over an outbound connection.
    //
    //  See Also:
    //      ISipOutboundConnectionSvc
    //
    //==========================================================================
    virtual unsigned int GetRegId() const = 0;

    //==============================================================================
    //==
    //==  GetRegistrationExpiration()
    //==
    //==============================================================================
    //
    //  Summary:
    //      Returns the amount of time before the registration is considered 
    //      expiring.
    //
    //  Returns:
    //      m_uExpirationTimeMs / 1000
    //
    //  Description:
    //      This method returns the expiration time in seconds.
    //
    //==============================================================================
    virtual unsigned int GetRegistrationExpiration() const = 0;

    //==============================================================================
    //==
    //==  GetReRegistrationTime()
    //==
    //==============================================================================
    //
    //  Summary:
    //      Returns the amount of time before the re-registration takes place
    //
    //  Returns:
    //      the time before re-registration takes place
    //
    //  Description:
    //      This method returns the re-registration time in seconds
    //
    //==============================================================================
    virtual unsigned int GetReRegistrationTime() const = 0;


//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipRegistrationSvc(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipRegistrationSvc() {}


private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    ISipRegistrationSvc(IN const ISipRegistrationSvc& rFrom);

    ISipRegistrationSvc& operator=(IN const ISipRegistrationSvc& rFrom);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPREGISTRATIONSVC_H

