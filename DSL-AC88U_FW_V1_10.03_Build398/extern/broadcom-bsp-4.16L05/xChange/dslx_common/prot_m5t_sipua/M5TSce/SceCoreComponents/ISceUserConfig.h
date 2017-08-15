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

//==============================================================================
//
// Portions of this software Copyright (c) 2008-2011 Broadcom
// 
//==============================================================================

#ifndef MXG_ISCEUSERCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEUSERCONFIG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
//-------------------------
class CNameAddr;
class CSipHeader;
class CString;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceUserConfig);

// This is the default value used for the local Contact identity.
const char* const szDEFAULT_LOCAL_CONTACT_IDENTITY_VALUE = "InvalidUser";
//M5T_INTERNAL_USE_END


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceUserConfig
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines the general configuration options for a single user
//   of the system.
//
// Description:
//  This interface defines the general configuration options for a single user
//  of the system.
//
//  The only mandatory item to configure is the user's Address of Record, using
//  SetUserAddress.
//
//  However, most deployments will not work properly unless authentication is
//  configured by querying the ISceUserAuthentication interface.
//
//  Contact and Via headers are managed internally by all components according
//  to the configured local addresses at the stack level. However, the Contact
//  header can be overidden by the application by using SetContact.
//
//  The algorithm for a component to select a Contact is as follows:
//
//     1- If privacy is requested, use a temporary GRUU. If no such GRUU is
//        available, go to option 3.
//     2- If no privacy is requested, use a public GRUU if one is available.
//     3- If no public GRUU is available, use an application configured contact.
//     4- If no Contact is configured, build one using a persistent
//        connection established by the SCE.
//     5- If there is none, build the Contact according to the local address
//        configuration done at the stack level.
//
// Location:
//   SceCoreComponents/ISceUserConfig.h
//
// See Also:
//  ISipCoreConfig::AddLocalAddress, ISipCoreConfig::ListenA
//
//==============================================================================
class ISceUserConfig : public IEComUnknown
{
//-- Friend Declarations.
//------------------------


//-- Published Interface.
//------------------------
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceUserConfig);
//M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  SetInstanceId
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the instance ID of this user-agent.
    //
    //  Parameters:
    //    rstrInstanceId:
    //      The full instance ID, including the URN scheme. Note that no
    //      validation is performed. It is the user's responsiblility to provide
    //      a valid instance ID.
    //
    //  Description:
    //      Configures the instance ID of this user-agent on this device. This
    //      instance ID is used to uniquely identify this specific user on a
    //      specific device.
    //
    //      The instance ID should be a UUID URN as a string. This ID must
    //      always be the same for this UA/device combination, that is, the same
    //      ID must be used for the same user on a specific device. The
    //      application must ensure that this ID is the same across reboots
    //      for this user/device combination.
    //
    //      If the user uses a different device to access SIP services, the
    //      instance ID must be different. If a different user uses this device
    //      to access SIP services, the instance ID must be different.
    //
    //      An example of a UUID URN follows:
    //
    //      <CODE>
    //      urn:uuid:0C67446E-F1A1-11D9-94D3-000A95A0E128
    //      </CODE>
    //
    //      The text following the "urn:uuid:" part consists of the unique
    //      identifier. Note that this method expects to receive the full URN,
    //      including the "urn:uuid:" parts.
    //
    //      It is not mandatory to configure an instance ID, however some
    //      functionalities like GRUU require an instance ID to be configured in
    //      order to be enabled.
    //
    //  See Also:
    //      GetInstanceId, ISceCoreConfig::GenerateInstanceId
    //
    //==========================================================================
    virtual void SetInstanceId(IN const CString& rstrInstanceId) = 0;

    //==========================================================================
    //==
    //==  GetInstanceId
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the configured instance ID.
    //
    //  Returns:
    //      The string holding the instance ID.
    //
    //  Description:
    //      This method gets the instance ID that was previously configured with
    //      SetInstanceId. If no instance ID was previously configured,
    //      rstrInstanceId is set to an empty string.
    //
    //  See Also:
    //      SetInstanceId, ISceCoreConfig::GenerateInstanceId
    //
    //==========================================================================
    virtual const CString& GetInstanceId() const = 0;

    //==========================================================================
    //==
    //==  SetUserAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the user's public address or Address of Record (AoR).
    //
    //  Parameters:
    //      pNameAddr:
    //       The user's address.
    //
    //  Description:
    //      It sets the user's public address. This address is also known as an
    //      Address of Record (AoR). This address usually points to a server
    //      that knows how to contact the user at its current address.
    //
    //      This address is used to populate the From header of requests
    //      initiated by this user.
    //
    //      If this address is not configured, it defaults to
    //      "sip:user@host.invalid".
    //
    //  See Also:
    //      GetUserAddress
    //
    //==========================================================================
    virtual void SetUserAddress(IN const CNameAddr& rNameAddr) = 0;

    //==========================================================================
    //==
    //==  GetUserAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the user address.
    //
    //  Returns:
    //      The user address.
    //
    //  Description:
    //      Sets the user's public address. This is the AoR of the user.
    //
    //  See Also:
    //      SetUserAddress
    //
    //==========================================================================
    virtual const CNameAddr& GetUserAddress() const = 0;

    //==========================================================================
    //==
    //==  SetPreLoadedRoute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the pre-loaded route to use for this user.
    //
    //  Parameters:
    //      pRouteHeaders:
    //       The route, this can be a set of Route headers. Ownership of this
    //       pointer is TAKEN. It can be set to NULL.
    //
    //  Returns:
    //      - resS_OK: The pre-loaded route is set successfully.
    //      - resFE_INVALID_ARGUMENT: The header type is different from
    //           eHDR_ROUTE.
    //
    //  Description:
    //      This method sets a pre-loaded Route consisting of one or more Route
    //      headers. All Route headers specified here are added to every
    //      out-of-dialog outgoing requests before being sent on the network. A
    //      pre-loaded route is only used until a dialog is established. Route
    //      headers are added to in-dialog requests only if the servers between
    //      the local user and the peer did Record-Routing, and this
    //      "in-dialog" route can possibly be different from the pre-loaded
    //      route.
    //
    //      Note that the loose-routing parameter "lr" is not automatically
    //      added to the route headers. The application is responsible to add
    //      such a parameter as part of the URI if it wants to do loose routing.
    //      By default, if no lr parameter is specified, the UA SSP does strict
    //      routing as defined in RFC 2543. It is recommended to do loose
    //      routing as it follows the latest standards defined in RFC 3261.
    //
    //  See Also:
    //      GetPreLoadedRoute
    //
    //==========================================================================
    virtual mxt_result SetPreLoadedRoute(IN TO CSipHeader* pRouteHeaders) = 0;

    //==========================================================================
    //==
    //==  GetPreLoadedRoute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the pre-loaded route.
    //
    //  Returns:
    //      The pre-loaded route. It can be NULL.
    //
    //  Description:
    //      Gets the pre-loaded route established through SetPreLoadedRoute.
    //
    //  See Also:
    //      SetPreLoadedRoute
    //
    //==========================================================================
    virtual const CSipHeader* GetPreLoadedRoute() const = 0;

    //==========================================================================
    //==
    //==  SetServiceRoute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the service route to use for this user.
    //
    //  Parameters:
    //      pRouteHeaders:
    //       The service route, as Route headers or Service-Route headers. In
    //       case of Service-Route header, the header's type is changed for
    //       Route header. Ownership of this pointer is TAKEN. It can be set to
    //       NULL.
    //
    //  Returns:
    //      - resS_OK: The service route is set successfully.
    //      - resFE_INVALID_ARGUMENT: The header type is different from eHDR_ROUTE
    //           and eHDR_SERVICE_ROUTE.
    //
    //  Description:
    //      Configures a service route to use for outgoing requests outside of a
    //      dialog.
    //
    //      The service route that a user must use is usually received in a
    //      successful response to a registration request. Within the Client
    //      Engine, the Registration component is responsible to set the Service
    //      Route of a user by using this API.
    //
    //  See Also:
    //      GetServiceRoute
    //
    //==========================================================================
    virtual mxt_result SetServiceRoute(IN TO CSipHeader* pRouteHeaders) = 0;

    //==========================================================================
    //==
    //==  GetServiceRoute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured service route.
    //
    //  Returns:
    //      The service route. It can be NULL.
    //
    //  Description:
    //      Gets the configured service-route as Route headers.
    //
    //  See Also:
    //      SetServiceRoute
    //
    //==========================================================================
    virtual const CSipHeader* GetServiceRoute() const = 0;


    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   The potential treatment the SIP Client Engine can take upon reception
    //   of a Service-Route header.
    //
    // Description:
    //  This type defines the potential treatment the SIP Client Engine can take
    //  upon reception of a Service-Route header.
    //
    //==========================================================================
    enum EServiceRouteTreatment
    {
        // Description:
        // Ignore the Service-Route headers.
        //-----------------------------------
        eSR_IGNORE,

        // Description:
        // Uses the Service-Route to replace the pre-loaded route configured for
        // this user. Note that this setting does not actually mean that the
        // reception of a Service-Route will change the pre-loaded route set
        // with SetPreLoadedRoute. It simply means that when building the Route
        // header, only the service route will be used.
        //----------------------------------------------------------------------
        eSR_OVERRIDE_PRE_LOADED_ROUTE,

        // Description:
        // Appends the Service-Route to the pre-loaded route. Note that this
        // setting does not actually mean that the reception of a
        // Service-Route will change the pre-loaded route set with
        // SetPreLoadedRoute. It simply means that when building the Route 
        // header, the sum of the pre-loaded route and the configured 
        // Service-Route will be used.
        //---------------------------------------------------------------------
        eSR_APPEND_TO_PRE_LOADED_ROUTE
    };


    //==========================================================================
    //==
    //==  SetServiceRouteTreatment
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures how the Client Engine must manage Service-Route headers
    //      for this user.
    //
    //  Parameters:
    //    eTreatment:
    //      The treatment to apply to the service route header when
    //      BuildPreLoadedRoute is called.
    //
    //  Description:
    //      This method configures how to create the final pre-loaded route to
    //      place in outgoing requests.
    //
    //      eTreatment does not have any effect on the reception of a
    //      Service-Route and on the Service-Route itself. The treatment is
    //      always applied at run-time when the route is built. Thus,
    //      the actual pre-loaded route and service-route configurations are
    //      always kept fully intact in the user configuration object.
    //
    //      The default route treatment is: eSR_IGNORE.
    //
    //  See Also:
    //      SetPreLoadedRoute, SetServiceRoute
    //
    //==========================================================================
    virtual void SetServiceRouteTreatment(IN EServiceRouteTreatment eTreatment) = 0;

    //==========================================================================
    //==
    //==  GetServiceRouteTreatment
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns how the Client Engine must manage Service-Route headers
    //      for this user.
    //
    //  Returns:
    //      The treatment to apply to the service route header.
    //
    //  Description:
    //      This method returns how the final pre-loaded route to place in
    //      outgoing requests will be created.
    //
    //      eTreatment does not have any effect on the reception of a
    //      Service-Route and on the Service-Route itself. The treatment is
    //      always applied at run-time when the route is built. Thus,
    //      the actual pre-loaded route and service-route configurations are
    //      always kept fully intact in the user configuration object.
    //
    //      The default route treatment is: eSR_IGNORE.
    //
    //  See Also:
    //      SetPreLoadedRoute, SetServiceRoute
    //
    //==========================================================================
    virtual EServiceRouteTreatment GetServiceRouteTreatment() const = 0;

    //==========================================================================
    //==
    //==  SetLocalContactIdentity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the username to use in Contacts generated by the M5T
    //      Client Engine.
    //
    //  Parameters:
    //    rstrUserPart:
    //      Username as a string.
    //
    //  Description:
    //      Configures the username to use in Contacts generated 
    //      by the M5T Client Engine. This username is used when building a
    //      Contact list from the network interfaces configured in M5T SIP Stack.
    //
    //      This configured username is not used if SetForcedContact forces the
    //      use of an application specified Contact. The username is usually
    //      automatically configured by the M5T Client Engine.
    //
    //      The default value is "InvalidUser". This default value is
    //      automatically updated to a unique random string when the user config
    //      is associated with a registration component. The local contact
    //      identity is however not updated by the registration component if its
    //      value does not correspond to the default value. This allows setting
    //      application defined local contact identity before the user
    //      configuration object is associated with a registration component.
    //
    //      Applications calling SetLocalContactIdentity must ensure that the
    //      string they set is unique to each user configuration object it
    //      creates when they are using the registration component.
    //
    //      Applications that are not using the registration component must set
    //      a local contact identity.
    //
    //  See Also:
    //      GetLocalContactIdentity
    //
    //==========================================================================
    virtual void SetLocalContactIdentity(IN const CString& rstrUserPart) = 0;

    //==========================================================================
    //==
    //==  GetLocalContactIdentity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured username used for M5T Client Engine generated
    //      Contacts.
    //
    //  Returns:
    //      The configured username.
    //
    //  Description:
    //      Provides access to the configured username, which is used when the
    //      M5T Client Engine builds a Contact list.
    //
    //  See Also:
    //      SetLocalContactIdentity
    //
    //==========================================================================
    virtual const CString& GetLocalContactIdentity() const = 0;

    //==========================================================================
    //==
    //==  SetForcedContact
    //==
    //==========================================================================
    //
    //  Summary:
    //      Forces the contact associated with this user.
    //
    //  Parameters:
    //      pContactHdr:
    //       The contact address. Ownership of this pointer is ALWAYS TAKEN.
    //
    //        NULL can be used to delete the current contact.
    //
    //  Returns:
    //      - resS_OK: The forced contact is set successfully.
    //      - resFE_INVALID_ARGUMENT: The header type is different from
    //           eHDR_CONTACT.
    //
    //  Description:
    //      This method forces the Contact address used to populate Contacts
    //      headers.
    //
    //      The M5T Client Engine automatically generates and manages the
    //      Contact header associated with each user of the system. It is
    //      however possible to force the usage of a specific contact, thus
    //      overriding the automatic contact generation.
    //
    //      This method allows the application to force the use of a Contact by
    //      the M5T Client Engine components. This allows the application to set
    //      the Contact to the public address of a NAT in order to allow the NAT
    //      to forward packets to the SIP stack.
    //
    //  See Also:
    //      GetForcedContact
    //
    //==========================================================================
    virtual mxt_result SetForcedContact(IN TOA CSipHeader* pContactHdr) = 0;

    //==========================================================================
    //==
    //==  GetForcedContact
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the Contact configured with SetForcedContact.
    //
    //  Returns:
    //      The Contact header. May be NULL if no Contact was configured.
    //
    //  Description:
    //      Gets the Contact header that has been configured with
    //      SetForcedContact.
    //
    //  See Also:
    //      SetForcedContact
    //
    //==========================================================================
    virtual const CSipHeader* GetForcedContact() const = 0;

    //==========================================================================
    //==
    //==  SetForceDefaultPortInContact
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets if the contact port to always appear or not.
    //
    //  Parameters:
    //      bForce:
    //       Tells if the contact port will be forced to appear or not.
    //
    //  Description:
    //      This method sets the option to force the Contact port to always
    //      appear or not in the contact URI.
    //
    //      Default is false.
    //
    //==========================================================================
    virtual void SetForceDefaultPortInContact(IN bool bForce) = 0;

    //==========================================================================
    //==
    //==  GetForceDefaultPortInContact
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets if the contact port to always appear or not.
    //
    //  Returns:
    //      Tells if the contact port will be forced to appear or not.
    //
    //  Description:
    //      This method gets the option to force the Contact port to always
    //      appear or not in the contact URI.
    //
    //==========================================================================
    virtual bool GetForceDefaultPortInContact() = 0;

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   The behaviour that the components must exhibit when trying to send a
    //   request to a TEL URI.
    //
    // Description:
    //  This enumeration defines the behaviour that the components associated
    //  with this user configuration must exhibit when trying to send a request
    //  to a TEL URI.
    //
    //==========================================================================
    enum ETelUriBehavior
    {
        // Description:
        // Resolves the TEL URI to a SIP or SIPS URI by performing an ENUM
        // query. If the query fails, the request is forwarded all the same to
        // the next hop if a pre-loaded route is available. If no pre-loaded
        // route exists, the request fails.
        //---------------------------------------------------------------------
        eTELURI_RESOLVE,
        
        
        // Description:
        // Does not try to resolve the TEL URI and forwards the request to
        // the next hop if a pre-loaded route is available. If no pre-loaded
        // route is available, the request fails.
        //---------------------------------------------------------------------
        eTELURI_DONTRESOLVE
    };

    //==========================================================================
    //==
    //==  SetTelUriBehavior
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the behaviour of components when they have to send a
    //      request to a TEL URI.
    //
    //  Parameters:
    //    eBehavior:
    //      The behaviour that the components must have.
    //
    //  Description:
    //      Configures the behaviour of the components associated with this user
    //      configuration object when they have to send a SIP request to a TEL
    //      URI.
    //      
    //      The default behaviour is eTELURI_DONTRESOLVE.
    //
    //  See Also:
    //      ETelUriBehavior, GetTelUriBehavior
    //
    //==========================================================================
    virtual void SetTelUriBehavior(IN ETelUriBehavior eBehavior) = 0;

    //==========================================================================
    //==
    //==  GetTelUriBehavior
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the current behaviour of components when they have to send a
    //      request to a TEL URI.
    //
    //  Returns:
    //      The current behaviour that the components must have.
    //
    //  Description:
    //      Returns the current behaviour of the components associated with this user
    //      configuration object when they have to send a SIP request to a TEL
    //      URI.
    //      
    //      The default behaviour is eTELURI_DONTRESOLVE.
    //
    //  See Also:
    //      ETelUriBehavior, SetTelUriBehavior
    //
    //==========================================================================
    virtual ETelUriBehavior GetTelUriBehavior() const = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an opaque data.
    //
    //  Parameters:
    //      opq:
    //       The opaque data to store in this object.
    //
    //  Description:
    //      Sets the opaque data stored in this object.
    //
    //==========================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets an opaque data.
    //
    //  Returns:
    //      The opaque data stored in this object.
    //
    //  Description:
    //      Returns the opaque data stored in this object.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    virtual mxt_opaque GetDnsUserOpaque() const = 0;

    virtual void SetDnsUserOpaque(IN mxt_opaque opq) = 0;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

// BRCM_CUSTOM - [add] Add support for appending @host to Call-Id
    virtual void SetHostInCallId(IN bool bEnable) = 0;

    virtual bool IsHostInCallId() const = 0;
// BRCM_CUSTOM - [end] Add support for appending @host to Call-Id

// BRCM_CUSTOM - [add] Add support for disabling symmetric UDP
    virtual void SetSymmetricUdp(IN bool bEnable) = 0;

    virtual bool IsSymmetricUdp() const = 0;
// BRCM_CUSTOM - [end] Add support for disabling symmetric UDP

// BRCM_CUSTOM - [add] Control To-tag matching choices
    virtual void SetToTagMatch(IN bool bEnable) = 0;

    virtual bool IsToTagMatch() const = 0;
// BRCM_CUSTOM - [end] Control To-tag matching choices

// BRCM_CUSTOM - [add] Control behavior when no P-Associated-URI is received
    virtual void SetNoPauUnreg(IN bool bEnable) = 0;

    virtual bool IsNoPauUnreg() const = 0;
// BRCM_CUSTOM - [end] Control behavior when no P-Associated-URI is received

// BRCM_CUSTOM - [add] Do not force validation of the P-Associated-URI content
    virtual void SetPauNoValidate(IN bool bEnable) = 0;

    virtual bool IsPauNoValidate() const = 0;
// BRCM_CUSTOM - [end] Do not force validation of the P-Associated-URI content

// BRCM_CUSTOM - [add] Ignore invalid uri in P-Asserted-Identity header
    virtual void SetPaiIgnoreInvalidUri(IN bool bEnable) = 0;
    
    virtual bool IsPaiIgnoreInvalidUri() const = 0;
// BRCM_CUSTOM - [add] Ignore invalid uri in P-Asserted-Identity header

// BRCM_CUSTOM - [add] Control registration failover behavior on all responses
    virtual void SetRegFvrAllRsp(IN bool bEnable) = 0;

    virtual bool IsRegFvrAllRsp() const = 0;
// BRCM_CUSTOM - [end] Control registration failover behavior on all responses


// BRCM_CUSTOM - [add] Control registration failover behavior on standard
//                     set of failover responses ( 408, 500, 504, 600)
   virtual void SetRegFvrOnStdErr(IN bool bEnable) = 0;

   virtual bool IsRegFvrOnStdErr() const = 0;
// BRCM_CUSTOM - [end] Control registration failover behavior on standard
//                     set of failover responses ( 408, 500, 504, 600)

// BRCM_CUSTOM - [add] Control registration failover behavior on 305 error response
   virtual void SetRegFvrOn305(IN bool bEnable) = 0;

   virtual bool IsRegFvrOn305() const = 0;
// BRCM_CUSTOM - [end] Control registration failover behavior on 305 error response

// BRCM_CUSTOM - [add] Control whether to apply registration failover on 403
//               Note that if Retry-After is included in the 403 response or 
//               pre-configured, then it will override the RegEndOn403 behavior
    virtual void SetRegEndOn403(IN bool bEnable) = 0;

    virtual bool IsRegEndOn403() const = 0;
// BRCM_CUSTOM - [end] Control whether to apply registration failover on 403

// BRCM_CUSTOM - [add] Control reg-event interpretation of 'rejected' as
//                     'deactivated'
    virtual void SetRejCtcAsDeactCtc(IN bool bEnable) = 0;

    virtual bool IsRejCtcAsDeactCtc() const = 0;
// BRCM_CUSTOM - [end] Control reg-event interpretation of 'rejected' as
//                     'deactivated'

// BRCM_CUSTOM - [add] Control subscription failure re-attempt behavior on
//                     500 response
    virtual void SetSubFail500(IN bool bEnable) = 0;

    virtual bool IsSubFail500() const = 0;
// BRCM_CUSTOM - [end] Control subscription failure re-attempt behavior on
//                     500 response

// BRCM_CUSTOM - [add] Control whether to maintain subscription on refresh failures
    virtual void SetKeepSubOnRefreshFail(IN bool bEnable) = 0;

    virtual bool IsKeepSubOnRefreshFail() const = 0;
// BRCM_CUSTOM - [end] Control whether to maintain subscription on refresh failures

// BRCM_CUSTOM - [add] Control whether to silently terminate active calls
    virtual void SetActCallNoSilentTerm(IN bool bEnable) = 0;

    virtual bool IsActCallNoSilentTerm() const = 0;
// BRCM_CUSTOM - [end] Control whether to silently terminate active calls

// BRCM_CUSTOM - [add] Control whether to include the default (5060 or 5061) port in contact header
    virtual void IncludeDefaultPortInContactHeader(IN bool bEnable) = 0;

    virtual bool IncludeDefaultPortInContactHeader() const = 0;
// BRCM_CUSTOM - [end] Control whether to include the default (5060 or 5061) port in contact header

// BRCM_CUSTOM - [add] Control whether to include the default (5060 or 5061) port in via header
    virtual void IncludeDefaultPortInViaHeader(IN bool bEnable) = 0;

    virtual bool IncludeDefaultPortInViaHeader() const = 0;
// BRCM_CUSTOM - [end] Control whether to include the default (5060 or 5061) port in via header

    //==========================================================================
    //==
    //==  SetDscp
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the DSCP to use at the network layer by the packets exchanged
    //      for this user.
    //
    //  Parameters:
    //      uDscp:
    //       The DSCP value to use.
    //
    //  Description:
    //      Sets the DSCP to use at the network layer by the packets exchanged
    //      for this user. The maximum value of the DSCP is 63 since it uses 6
    //      bits in the IP packet.
    //
    //==========================================================================
    virtual void SetDscp(IN uint32_t uDscp) = 0;

    //==========================================================================
    //==
    //==  GetDscp
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the DSCP used at the network layer by the packets exchanged
    //      for this user.
    //
    //  Returns:
    //       The DSCP value used.
    //
    //  Description:
    //      Gets the DSCP used at the network layer by the packets exchanged
    //      for this user. The maximum value of the DSCP is 63 since it uses 6
    //      bits in the IP packet.
    //
    //==========================================================================
    virtual uint32_t GetDscp() = 0;

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  IncrementNumCircularReferences
    //==
    //==========================================================================
    //
    //  Summary:
    //      Increments the number of circular references on the ISceUserConfig.
    //
    //  Returns:
    //      The new number of circular references on the ISceUserConfig.
    //
    //  Description:
    //      This method is for internal use only.
    //
    //      Increments the number of circular references on the ISceUserConfig.
    //
    //      When the ISceUserConfig reference count is decremented to the
    //      number of circular references on it, it will break the circular
    //      references by calling
    //      ISceUserConfigTermination::OnUserConfigTermination on the
    //      configurations it owns.
    //
    //      A circular reference MUST be broken when
    //      ISceUserConfigTermination::OnUserConfigTermination is called on an
    //      aggregated configuration object.
    //
    //      This MUST be used, for example, when a config has a reference to a
    //      component, the component has a reference to the config and the
    //      component only releases the config in its destructor.
    //
    //==========================================================================
    virtual unsigned int IncrementNumCircularReferences() = 0;
//M5T_INTERNAL_USE_END

//-- Hidden Methods.
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceUserConfig() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceUserConfig() {}

private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceUserConfig(IN const ISceUserConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceUserConfig& operator=(IN const ISceUserConfig& rSrc);


//-- Hidden Data Members.
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEUSERCONFIG_H

