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
#ifndef MXG_ISIPREDIRECTIONSVC_H
#define MXG_ISIPREDIRECTIONSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT to be \
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
class CSipHeader;
class ISipRedirectionMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipRedirectionSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipRedirectionSvc
//=============================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The Redirection Service is used to help the application manage recursive
//  searches for a remote user. When the application sends a request and
//  receives a 3xx class response, this service manages the various contacts
//  that are received in such a response and it lets the application know
//  through an event that there are other locations where the remote user can be
//  reached.
//
//  This service manages the contact list in such a way as to avoid falling
//  in an endless redirection loop. Thus, when searching for a user and
//  receiving multiple 3xx responses, this service only keeps the contacts that
//  were not tried in its contact list. All contacts where the application has
//  sent a request are removed from the list.
//
//  To reach a contact that is not in the contacts list of the redirection
//  service, the application can use the SetCurrentTarget method on the user
//  agent service and then re-issue the request. The request is then sent
//  to the contact location as set in the current target.
//
//  The Redirection Service can handle 3xx responses for any type of request.
//
//  This service reports events to the application through the
//  ISipRedirectionMgr interface.
//
//#The ISipRedirectionSvc is an ECOM object#
//
//  The ISipRedirectionSvc is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipRedirectionSvc</B>
//
//  <B>Interface Id: IID_ISipRedirectionSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Updated Behaviour:
//
//  The behaviour of the redirection service has changed from MxSF v3.x.
//
//  - In v4.0, the contacts are removed from the list when a request is sent
//    using this contact as Request-URI.
//
//  - In v4.0, when a contact is received twice in a 3xx response, the contact
//    version with the maximum qvalue is kept.  When there is no qvalue in
//    a contact, 1.0 is taken as qvalue.
//
//  - The responses received from requests made inside of a dialog are not
//    monitored by the service.
//
//  - Even if a 2xx response is received, the contacts that were not tried are
//    no longer cleared so this service sends an EvRedirected as soon
//    as it receives a negative response to a request made outside of a dialog,
//    if there are still contacts to try.
//
// Location:
//   SipUserAgent/ISipRedirectionSvc.h
//
// See Also:
//   ISipRedirectionMgr.h
//
//==============================================================================
class ISipRedirectionSvc : public IEComUnknown
{
// Friend Declarations
//-------------------------

// Published Interface
//-------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipRedirectionSvc);
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
    //      Configures the manager that will receive the events generated by
    //      this service.
    //
    //      Note that a manager MUST be associated with this service before it 
    //      is used. If no manager is associated with the service, the service 
    //      will not monitor any request nor response.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager( IN ISipRedirectionMgr* pMgr ) = 0;

    //==========================================================================
    //==
    //==  UseNextContact
    //==
    //==========================================================================
    //
    //  Summary:
    //      Makes the parent context use the Contact header with the highest
    //      'q value' for the request-URI of future requests.
    //
    //  Returns:
    //      resFE_INVALID_STATE if the manager was not set OR
    //                          if Clear was called on the context OR
    //                          if there is no other contact to try OR
    //                          if there is no ISipUserAgentSvc attached to the
    //                              context to which this service is attached.
    //
    //      resFE_FAIL if changing the request-URI failed.
    //
    //      resS_OK if the request-URI was successfully changed.
    //
    //  Description:
    //      This method updates the state of the parent context so it uses the
    //      URL of the contact that has the highest 'q value' as the request-URI
    //      of future requests. It returns an error if there are no contacts to
    //      use. This error condition usually happens if there were no
    //      redirections or if all possible contacts were already tried.
    //
    //      Usually, after calling UseNextContact, the application calls
    //      ReIssueRequest on the ISipClientEventControl pointer given by the
    //      EvRedirected event to re-send the request to the new request-URI.
    //
    //  See Also:
    //      UseContact
    //==========================================================================
    virtual mxt_result UseNextContact() = 0;

    //==========================================================================
    //==
    //==  UseContact
    //==
    //==========================================================================
    //
    //  Summary:
    //      Makes the parent context use the URI at position uContactIndex in
    //      the order of preference for the request-URI of future requests.
    //
    //  Parameters:
    //    uContactIndex:
    //      The index of the contact to use in the vector of contacts. 0-based
    //      where 0 refers to the header containing all the others.
    //
    //  Returns:
    //      resFE_INVALID_STATE if the manager was not set OR
    //                          if Clear was called on the context OR
    //                          if there is no other contact to try OR
    //                          if there is no ISipUserAgentSvc attached to the
    //                              context to which this service is attached.
    //
    //      resFE_INVALID_ARGUMENT if uContactIndex is not a valid index.
    //
    //      resFE_FAIL if changing the request-URI failed.
    //
    //      resS_OK if the request-URI was successfully changed.
    //
    //  Description:
    //      This method updates the state of the parent context so it uses the
    //      chosen URI as the request-URI of future requests.
    //
    //      It returns an error if there is no such contact to use, that is the
    //      index is greater than the number given by
    //      CSipHeader::GetNbNextHeaders on the CSipHeader given by GetContacts
    //      or if there is no contact to try.
    //
    //      The application should call this method only if it has positive
    //      knowledge that the preferred contact is not the target to which it
    //      wants to send a request. Called with a parameter of 0, this method
    //      is totally equivalent to UseNextContact.
    //
    //      Usually, after calling UseContact, the application calls
    //      ReIssueRequest on the ISipClientEventControl pointer given by the
    //      EvRedirected event to re-send the request to the new request-URI.
    //
    //  See Also:
    //      UseNextContact, GetContacts
    //==========================================================================
    virtual mxt_result UseContact( IN unsigned int uContactIndex ) = 0;

    //==========================================================================
    //==
    //==  GetContacts
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the list of possible contacts where the parent context's
    //      target could be reached.
    //
    //  Returns:
    //      A header that contains the contacts ordered by decreasing
    //      preference. If there are more than one contact, they are chained in
    //      the header and they can be accessed via
    //      <C>CSipHeader::GetNextHeader</C>.
    //
    //      NULL if there is no URI to try.
    //
    //      Note that a copy of the header chain must be done if the application
    //      wants to keep a modified copy of the chain.
    //
    //  Description:
    //      This method returns the list of contacts where the target of the
    //      parent context could be reached. It is ordered by decreasing 'q'
    //      value so the first contact in the list is the preferred one.
    //
    //      If there is no such contact, NULL is returned. In this case, calling
    //      UseNextContact or UseContact yields an error.
    //
    //  See Also:
    //      UseContact, UseNextContact
    //==========================================================================
    virtual const CSipHeader* GetContacts() = 0;

    //==========================================================================
    //==
    //==  RemoveContact
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes the specified contact from the list.
    //
    //  Parameters:
    //    uContactIndex:
    //      The specified index in the contact list. 0-based where 0 refers to
    //      the header containing all the others.
    //
    //  Returns:
    //      resFE_INVALID_STATE if the manager was not set OR
    //                          if Clear was called on the context OR
    //                          if there is no contact to try.
    //
    //      resFE_INVALID_ARGUMENT if uContactIndex is invalid.
    //
    //      resS_OK otherwise.
    //
    //  Description:
    //      This method removes the contact from the list of contacts to try
    //      given by GetContacts. The contacts that have been removed are no
    //      longer suggested.
    //
    //      If the specified index does not exist or if there is no contact to
    //      try, the method returns an error.
    //
    //  See Also:
    //      GetContacts
    //==========================================================================
    virtual mxt_result RemoveContact( IN unsigned int uContactIndex ) = 0;


// Hidden Methods
//-------------------------
protected:
    // << Constructors / Destructors >>
    //--------------------------------------------
    ISipRedirectionSvc() {}

    virtual ~ISipRedirectionSvc() {}

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    ISipRedirectionSvc( const ISipRedirectionSvc& rFrom );
    ISipRedirectionSvc& operator=( const ISipRedirectionSvc& rFrom );

// Hidden Data Members
//-------------------------
protected:
private:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPREDIRECTIONSVC_H

