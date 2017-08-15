//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPPUBLISHSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPPUBLISHSVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
// MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT to be \
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
class ISipPublishMgr;
class CString;
class CHeaderList;
class CSipMessageBody;
class ISipClientTransaction;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipPublishSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipPublishSvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//
//  This Publish service implements RFC 3903. It offers the interface to
//  manage a publication from an Event Publication Agent (EPA) to an event state
//  compositor (ESC).
//
//  It monitors the expiration of any publication and notifies the manager via
//  the ISipPublishMgr::EvExpiring event a few seconds before expiring. It also
//  reports the ISipPublishMgr::EvExpired event when the publication has expired.
//  The number of seconds between ISipPublishMgr::EvExpiring and the publication
//  expiration can be configured through the SetExpiringThreshold method.
//
//  This service does not initiate publication refreshes, modifications, or
//  removals on its own. This must always be initiated by the user of this
//  service. The service is not responsible to manage message bodies, it is the
//  application's responsibility.
//
//  The application has the flexibility to get and set the entity-tag held by
//  this service. This allows the application to place any entity-tag it wants
//  in a PUBLISH request. For instance, this is useful after the application has
//  rebooted and wants to issue a PUBLISH request to an already established
//  publication with a specific server.
//
//  This service only maintains one entity-tag, hence one entity-tag per context.
//  If the application wants to issue another publication using a different
//  entity-tag, it must create another context.
//
//  This service reports events to the application through the ISipPublishMgr
//  interface.
//
//#The ISipPublishSvc is an ECOM object#
//
//  The ISipPublishSvc is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipPublishSvc</B>
//
//  <B>Interface Id: IID_ISiPublishSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipPublishSvc.h
//
// See Also:
//   ISipPublishMgr
//
//==EDOC========================================================================
class ISipPublishSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------

//-- New types within class namespace
//-----------------------------------
public:
protected:
private:

//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipPublishSvc);
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
    //      resFE_INVALID_ARGUMENT if pMgr is NULL. The manager is not 
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
    virtual mxt_result SetManager(IN ISipPublishMgr* pMgr) = 0;

    //==SDOC====================================================================
    //==
    //==  SetExpiringThreshold
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the delay before expiration at which the EvExpiring event
    //      fires.
    //
    //  Parameters:
    //    uDelayInS:
    //      The delay between the EvExpiring and the expiration in seconds.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service. No manager is associated with the service if none was 
    //          set with SetManager or if Clear was called on the IContext
    //          interface.
    //
    //      resS_OK otherwise.
    //
    //  Description:
    //      Configures the delay between the moment ISipPublishMgr::EvExpiring
    //      event is generated and the publication expires. By default, this
    //      delay is 60 seconds. Note that the event EvExpiring is
    //      generated as soon as the time before expiration becomes lower than
    //      that threshold. The exception to that rule is that even if the ESC
    //      returns an expiration time that is lower than the threshold, this
    //      service does NOT generate an EvExpiring event. Instead, the manager
    //      of the publish service is warned only by an EvExpired event
    //      when the published information has expired. Also, if the threshold
    //      is set to 0, only the EvExpired event is generated.
    //
    //      This method affects only future publications. This means that if
    //      there is an active publication while this method is called, the
    //      EvExpiring event is called according to the previous threshold
    //      value.
    //
    //      If the expires value is smaller than or equal to the threshold, the
    //      EvExpiring event is never called, EvExpired is instead called
    //      directly.
    //
    //  See Also:
    //      ISipPublishMgr::EvExpiring
    //
    //==EDOC====================================================================
    virtual mxt_result SetExpiringThreshold(IN unsigned int uDelayInS) = 0;

    //==SDOC====================================================================
    //==
    //==  SetDefaultExpiration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the default expiration value for a publication.
    //
    //
    //  Parameters:
    //    uDefaultExpirationInS:
    //      The default expiration value.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service. No manager is associated with the service if none was 
    //          set with SetManager or if Clear was called on the IContext
    //          interface.
    //
    //      resS_OK otherwise.
    //
    //  Description:
    //      This function configures the default lifetime for a published event.
    //      The default expiration value is by default set to 1 hour. This 
    //      default value is only used by the Publish service if the Expires 
    //      header is invalid or missing in a 2xx response to a PUBLISH request.
    //
    //  See Also:
    //      ISipPublishMgr::EvExpiring, ISipPublishMgr::EvExpired
    //
    //==EDOC====================================================================
    virtual mxt_result SetDefaultExpiration(
                                     IN unsigned int uDefaultExpirationInS) = 0;

    //==SDOC====================================================================
    //==
    //==  GetEntityTag
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the entity-tag corresponding the the current publication.
    //
    //  Parameters:
    //      rstrEntityTag:
    //       The entity-tag found in the SIP-ETag header.
    //
    //  Returns:
    //      resS_OK if a 2xx response has been received and it contained an
    //          entity-tag or if an entity-tag was set with the SetEntityTag
    //          method.
    //
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service. No manager is associated with the service if none was 
    //          set with SetManager or if Clear was called on the IContext 
    //          interface.
    //
    //  Description:
    //      Retrieves the entity-tag set via the SetEntityTag method or
    //      received in a 2xx response after a PUBLISH request has been issued
    //      by this service.
    //
    //  See Also:
    //      ISipPublishMgr::EvSuccess, SetEntityTag
    //
    //==EDOC====================================================================
    virtual mxt_result GetEntityTag(OUT CString& rstrEntityTag) = 0;

    //==SDOC====================================================================
    //==
    //==  SetEntityTag
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the entity-tag to use in the next PUBLISH requests.
    //
    //  Parameters:
    //      rstrEntityTag:
    //       The entity-tag to put in SIP-If-Match headers.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service. No manager is associated with the service if none was 
    //          set with SetManager or if Clear was called on the IContext
    //          interface.
    //
    //      resFE_INVALID_ARGUMENT if the rstrEntityTag param is empty
    //
    //      resS_OK otherwise.
    //
    //  Description:
    //        Sets the entity-tag to be used in the next PUBLISH requests. This
    //        is true for the Publish, Modify, Refresh, and Remove methods.
    //
    //  See Also:
    //      GetEntityTag, Publish, Modify, Refresh, Remove
    //
    //==EDOC====================================================================
    virtual mxt_result SetEntityTag(IN const CString& rstrEntityTag) = 0;

    //==SDOC====================================================================
    //==
    //==  Publish
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends an initial publication to an ESC.
    //
    //  Parameters:
    //      opqTransaction:
    //       An opaque parameter to the transaction.
    //
    //      uExpiresS:
    //       The number of seconds that this publication is to remain on the
    //       server. Note that the server may override this in its response.
    //       This value is later used when refresing or modifying a publication.
    //
    //      rstrEvent:
    //       The value of the Event header. This same value is later used for
    //       refreshes and modifications to the publication.
    //
    //      pExtraHeaders:
    //       Extra SIP headers to send with the request. It can be NULL. 
    //       Ownership of the object is TAKEN.
    //
    //      pMessageBody:
    //       A pointer to the description of the message body that is put in the
    //       PUBLISH request to be sent. According to the specification,
    //       an initial PUBLISH MUST contain a message body. Ownership of the 
    //       object is TAKEN.
    //
    //      rpTransaction:
    //       Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service or if a final response to a previous request has not
    //          been received. No manager is associated with the service if none
    //          was set with SetManager or if Clear was called on the IContext
    //          interface.
    //
    //      resS_OK if the request has been correctly sent.
    //
    //      resFE_FAIL otherwise.
    //
    //  Description:
    //      This method implements the PUBLISH request initial operation for a
    //      publication from an EPA to an ESC.
    //
    //      The Publish method API allows an initial PUBLISH with a NULL message
    //      body. According to RFC 3903, it MUST include a message body with
    //      a published event name and MAY have an Expires header field. The
    //      message body contains the state of the publication.
    //
    //      If the publish succeeds, the server issues a 2xx response with
    //      an entity-tag and an Expires header. If, for any reason, the Expires
    //      header is missing in the 2xx response, the service assumes a default
    //      value of 1 hour for its publication expiration.
    //
    //  See Also:
    //      ISipPublishMgr, Modify, Refresh, Remove
    //
    //==EDOC====================================================================
    virtual mxt_result Publish(IN mxt_opaque opqTransaction,
                               IN unsigned int uExpiresS,
                               IN const CString& rstrEvent,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               OUT ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  Modify
    //==
    //==========================================================================
    //
    //  Summary:
    //      Modifies the publication.
    //
    //  Parameters:
    //      opqTransaction:
    //       An opaque parameter to the transaction.
    //
    //      pExtraHeaders:
    //       Extra SIP headers to send with the request. It can be NULL. 
    //       Ownership of the object is TAKEN.
    //
    //      pMessageBody:
    //       A pointer to the description of the message body that is put in the
    //       PUBLISH request to be sent. According to the specification, an
    //       initial PUBLISH MUST contain a message body. Ownership of the 
    //       object is TAKEN.
    //
    //      rpTransaction:
    //       Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service, if a final response to a previous request has not been
    //          received, or if an initial PUBLISH request has not been sent. No
    //          manager is associated with the service if none was set with
    //          SetManager or if Clear was called on the IContext interface.
    //
    //      resS_OK if the request has been correctly sent.
    //
    //      resFE_FAIL otherwise.
    //
    //  Description:
    //      This method implements the PUBLISH request modify operation for a
    //      publication from an EPA to an ESC. Its purpose is to change the
    //      state of an existing publication.
    //
    //      The entity-tag received in a 2xx response for a previous PUBLISH
    //      request or set via the SetEntityTag method is placed in the 
    //      SIP-If-Match header of the PUBLISH request issued by this method. 
    //      The event name and the expires value are also placed in the headers 
    //      of this PUBLISH modify message.
    //
    //      The Modify method API allows a modify operation with a NULL message
    //      body. According to RFC 3903, it MUST contain a message body.
    //      This message body holds the state information for the publication.
    //
    //  See Also:
    //      ISipPublishMgr, Publish, Refresh, Remove
    //
    //==EDOC====================================================================
    virtual mxt_result Modify(IN mxt_opaque opqTransaction,
                              IN TO CHeaderList* pExtraHeaders,
                              IN TO CSipMessageBody* pMessageBody,
                              OUT ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  Refresh
    //==
    //==========================================================================
    //
    //  Summary:
    //      Refreshes the publication expiration value
    //
    //  Parameters:
    //      opqTransaction:
    //       An opaque parameter to the transaction.
    //
    //      rstrEntityTag:
    //       The value of the SIP-If-Match header.
    //
    //      pExtraHeaders:
    //       Extra SIP headers to send with the request. It can be NULL. 
    //       Ownership of the object is TAKEN.
    //
    //      rpTransaction:
    //       Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service, if a final response to a previous request has not been
    //          received, or if an initial PUBLISH request has not been sent. No
    //          manager is associated with the service if none was set with
    //          SetManager or if Clear was called on the IContext interface.
    //
    //      resS_OK if the request has been correctly sent.
    //
    //      resFE_FAIL otherwise.
    //
    //  Description:
    //      This method implements the PUBLISH request refresh operation for a
    //      publication from an EPA to an ESC. Its purpose is to extend the
    //      expiration value for an existing publication.
    //
    //      The entity-tag received in a 2xx response for a previous PUBLISH
    //      request or set via the SetEntityTag method is placed in the 
    //      SIP-If-Match header of the PUBLISH request issued by this method. 
    //      The event name and the expires value are also placed in the headers 
    //      of this PUBLISH refresh message.
    //
    //  See Also:
    //      ISipPublishMgr, Publish, Modify, Remove
    //
    //==EDOC====================================================================
    virtual mxt_result Refresh(IN mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               OUT ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  Remove
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes the publication by sending a PUBLISH request with an Expires
    //      header with a value of zero.
    //
    //  Parameters:
    //      opqTransaction:
    //       An opaque parameter to the transaction.
    //
    //      pExtraHeaders:
    //       Extra SIP headers to send with the request. It can be NULL. 
    //       Ownership of the object is TAKEN.
    //
    //      rpTransaction:
    //       Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_STATE if there is no manager associated with this
    //          service, if a final response to a previous request has not been
    //          received, or if an initial PUBLISH request has not been sent. No
    //          manager is associated with the service if none was set with
    //          SetManager or if Clear was called on the IContext interface.
    //
    //      resS_OK if the request was correctly sent.
    //
    //      resFE_FAIL otherwise.
    //
    //  Description:
    //      This method implements the PUBLISH request remove operation for a
    //      publication from an EPA to an ESC. Removing a publication from
    //      an Esc is done as a refresh with an expires value set to 0.
    //      The last entity-tag received in 2xx or set via SetEntityTag is
    //      also placed in the SIP-If-Match header to match the publication at
    //      the ESC level.
    //
    //  See Also:
    //      ISipPublishMgr, Publish, Modify, Refresh
    //
    //==EDOC====================================================================
    virtual mxt_result Remove(IN mxt_opaque opqTransaction,
                              IN TO CHeaderList* pExtraHeaders,
                              OUT ISipClientTransaction*& rpTransaction) = 0;


//-- Hidden Methods
//-------------------
protected:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipPublishSvc(){}

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipPublishSvc(){}

private:

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipPublishSvc(IN const ISipPublishSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipPublishSvc& operator=(IN const ISipPublishSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPPUBLISHSVC_H

