//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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
// Portions of this software Copyright (c) 2009-2011 Broadcom
// 
//==============================================================================

#ifndef MXG_ISIPUSERAGENTSVC_H
#define MXG_ISIPUSERAGENTSVC_H

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


#ifndef MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT to be able to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CGenParamList;
class CNameAddr;
class CSipHeader;
class CSipPacket;
class CSocketAddr;
class CString;
class IUri;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipUserAgentSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipUserAgentSvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   This interface is responsible to keep the dialog state for User Agent
//   contexts. When using User Agent services like the session service or the
//   generic service, the ISipUserAgentSvc MUST be added to the context except
//   for cases provided below:
//      - The context is used to handle out-of-dialog or unsupported requests.
//      - When the same context is used to handle every blind NOTIFY requests
//        related to MWI for instance.
//
//   This service is responsible for keeping dialog identifiers like From, To,
//   and Call-ID headers and putting them in requests to send. This service also
//   determines whether or not a packet belongs to this dialog. However, the
//   dialog creation is dealt with automatically by the stack and the
//   application does not have to bother with that(except when using the
//   generic service).
//
//   When issuing a request, the application must configure this service with at
//   least a target, the Contact (if the request to send can establish a
//   dialog), the preloaded Route (unless the default empty Route is satisfying),
//   and the From header. The service takes care of generating a unique Call-ID,
//   a unique From Tag, a starting CSeq number, and an empty To tag. However,
//   the application could force their value to something it specifies by
//   calling the various configuration methods of this service. Note that once
//   a request has been issued, the application can still change the dialog
//   identifiers like the From Tag, To Tag, Call-ID, or CSeq number. However, it
//   should refrain to do so as it could lead to unexpected behaviour like the
//   remote peer not recognizing the request it receives or the local
//   application not recognizing the requests received from the remote peer.
//
//   When the service is attached to a context that is created to receive a
//   request, the dialog identifiers configure themselves when the dialog is
//   established. The application only needs to configure a Contact (unless the
//   application does not intend to create a dialog on that context).
//
//==EDOC========================================================================
class ISipUserAgentSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipUserAgentSvc);
    //M5T_INTERNAL_USE_END

    // This enum indicates if the target headers (of the actual current target
    // that will be overwritten by the new current target) must be preserved
    // and appended to the target headers of the new current target.
    //-------------------------------------------------------------------------
    enum ETargetHeaderOption
    {
        // The target headers of the actual current target are appended to the
        // target headers of the new current target.
        //---------------------------------------------------------------------
        ePRESERVE_TARGET_HEADERS,

        // The target headers of the actual current target are cleared. They
        // are not appended to the target headers of the new current target.
        //-------------------------------------------------------------------
        eCLEAR_TARGET_HEADERS
    };

    //-- << Dialog Id setters >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  AddLocalContact
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds Contact header(s) in the internal Contact list.
    //
    //  Parameters:
    //    pHdrContact:
    //      The Contact header to add.
    //
    //      If pHdrContact contains next headers, all headers will be inserted
    //      in the list.
    //
    //      If pHdrContact contains a SIP-URI containing parameters other than
    //      "transport", their value may be overridden.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The header in parameter is not a valid
    //        Contact header.
    //
    //      - resS_OK: The Contact header has been successfully added.
    //
    //  Description:
    //      Adds a new Contact header in the local Contact header list. The
    //      Contact list is used by services to determine the Contact to put in
    //      the SIP requests and responses.
    //
    //      The Contact is selected by the SIP stack from the Contact list
    //      according to the local address used to send the SIP packet.
    //      Therefore, if the Contact is a FQDN, it must match one of the FQDNs
    //      added with the local addresses. If the Contact is an IP address, it
    //      must match one of the local IP addresses or address families.
    //
    //      If the application knows which local address the SIP stack uses to
    //      reach the peer assigned to the SIP context, it should add a single
    //      Contact, i.e., the Contact associated with the local address that is
    //      used. Otherwise, if the application never knows which local address
    //      is used to reach the peer, it should add a Contact for every local
    //      address configured in the SIP stack. Refer to
    //      ISipCoreConfig::AddLocalAddress for more details about local
    //      addresses.
    //
    //      It is important to understand that if <C>pHdrContact</C> contains
    //      raw data, this data is used without modification when creating
    //      SIP packets for the associated context. Refer to CSipHeader for
    //      more details on Raw versus Parsed header.
    //
    //      When the SIP-UA cannot find any configured contacts to match a
    //      network interface, the service will use the first contact that is
    //      available. This allows the application to easily override the
    //      automatic contact selection.
    //
    //  See Also:
    //      GetLocalContact, GetLocalContactVector
    //
    //==EDOC====================================================================
    virtual mxt_result AddLocalContact(IN TOS CSipHeader* pHdrContact) = 0;

    //==SDOC====================================================================
    //==
    //==  SetPreloadedRoute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Set the Route header to put in the out-of-dialog requests sent.
    //
    //  Parameters:
    //    pPreloadedRoute:
    //      The Route headers to put in out-of-dialog client requests. It can be
    //      NULL, which means that no preloaded Route is used. The ownership of
    //      this parameter is taken by this method unless an error is returned.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: When the pPreloadedRoute is not NULL and
    //      at least one of the headers it contains is not a Route header.
    //      Ownership of the parameter is not taken in this case.
    //
    //      - resS_OK: The preloaded route has been successfully updated.
    //      Ownership of the parameter is taken.
    //
    //  Description:
    //      Sets the Route headers to put in out-of-dialog requests sent by
    //      contexts that use this ISipUserAgentSvc.
    //
    //      This Route is used only when no dialog is established yet (such as
    //      when sending a dialog establishing INVITE or sending a REGISTER).
    //      When a dialog is established, the rules of RFC 3261 dictate how the
    //      Route is computed according to requests and responses that
    //      established the dialog.
    //
    //      It is important to understand that if <C>pPreloadedRoute</C>
    //      contains raw data, this data is used whithout modification when
    //      creating SIP packets for the associated context. Refer to CSipHeader
    //      for more details on Raw versus Parsed header.
    //
    //  See Also:
    //      GetPreloadedRoute
    //
    //==EDOC====================================================================
    virtual mxt_result SetPreloadedRoute(IN TO CSipHeader* pPreloadedRoute) = 0;

    //==========================================================================
    //==
    //==  SetEstablishedRoute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the Route header(s) placed in requests sent within an
    //      established dialog.
    //
    //  Parameters:
    //    pEstablishedRoute:
    //      The Route headers to set as an established route. This replaces the
    //      previously established route. It can be NULL, in which case the
    //      established route is simply removed. The ownership of this parameter
    //      is taken by this method unless an error is returned.
    //
    //  Returns:
    //    - resFE_INVALID_ARGUMENT: When the pEstablishedRoute is not NULL and
    //      at least one of the headers it contains is not a Route header.
    //      Ownership of the parameter is not taken in this case.
    //
    //    - resS_OK: The route has been successfully updated.
    //      Ownership of the parameter is taken.
    //
    //  Description:
    //      Sets the Route headers placed in requests sent within an established
    //      dialog, for this context.
    //
    //      When a dialog is established, the rules of RFC 3261 dictate how the
    //      Route is computed according to requests and responses that
    //      established the dialog.
    //
    //      It is important to understand that if <C>pEstablishedRoute</C>
    //      contains raw data, this data is used without modification when
    //      creating SIP packets for the associated context. Refer to CSipHeader
    //      for more details on Raw versus Parsed header.
    //
    //  Warning:
    //      This API is usually not used by the user of the stack as the
    //      established route is automatically managed. Care must be taken as
    //      changing this has important impacts on routing of future requests of
    //      this dialog and it breaks the SIP 3261 compliance.
    //
    //  See Also:
    //      SetPreloadedRoute, GetEstablishedRoute
    //
    //==========================================================================
    virtual mxt_result SetEstablishedRoute(IN TOS CSipHeader* pEstablishedRoute) = 0;

    //==SDOC====================================================================
    //==
    //==  SetCurrentTarget
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the current target where to send the requests.
    //
    //  Parameters:
    //    pUri:
    //      The URI to put in Request-URI of outgoing requests. It must not be
    //      NULL.
    //
    //    eTargetHeaderOption:
    //      Whether the headers of the actual current target are
    //      appended to the headers of the new current target or not.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The URI in parameter is NULL.
    //
    //      - resFE_INVALID_ARGUMENT: eTargetHeaderOption is equal to
    //        ePRESERVE_TARGET_HEADERS and the current target contains headers
    //        but pUri is not a SIP or SIPS URI so the headers cannot be
    //        copied.
    //
    //      - resS_OK: The target has been successfully updated.
    //
    //  Description:
    //      This method should be called in two distinct occasions. The first
    //      occasion is when creating a context to send requests. The
    //      application would create the context and attach the appropriate
    //      services including the ISipUserAgentSvc and it would then call this
    //      method with a valid URI to use in the Request-URI of requests to
    //      send.
    //
    //      The second occasion where this method is called is when a target
    //      refresh request or response is received. Services that handle such
    //      requests are responsible to call that method themselves and the
    //      application does not need to bother with that. The only exception is
    //      when the application handles target refresh request or responses
    //      through the ISipGenericSvc. In this case, it must call this method
    //      itself with the URI found in the Contact of the request or the
    //      response received.
    //
    //      The headers of the current target are used in the next request to
    //      the specified target. As long as they are not changed, these
    //      headers are used in every request sent to the specified target.
    //      When calling this method, these headers can be preserved and
    //      appended to the headers of pUri (by specifying
    //      ePRESERVE_TARGET_HEADERS). The headers cannot be preserved if the
    //      type of the pUri is not SIP or SIPS.
    //
    //      The URI set in this method is used as the request-URI when creating
    //      a request with CreateBasicRequest. It can be consulted through
    //      GetCurrentTarget.
    //
    //  See Also:
    //      GetCurrentTarget, CreateBasicRequest
    //
    //==EDOC====================================================================
    virtual mxt_result SetCurrentTarget(
        IN const IUri* pUri,
        IN ETargetHeaderOption eTargetHeaderOption = eCLEAR_TARGET_HEADERS) = 0;

    //==SDOC====================================================================
    //==
    //==  SetCallId
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the Call-ID for this context.
    //
    //  Parameters:
    //    rCallID:
    //      The Call-ID to put in outgoing requests and that must be found in
    //      incoming requests.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The header in parameter is not a valid
    //      Call-ID header.
    //
    //      - resSW_DIALOG_NOT_REGISTERED: All the processing was done to change
    //      the Call-ID, but the dialog was already established and Clear was
    //      called on the context so the new Call-ID was not registered in the
    //      dialog matcher list. It means that no request matching the dialog
    //      will be received. Note that the previous Call-ID was unregistered
    //      from the dialog matcher list when Clear was called, so no further
    //      information will be received from it either.
    //
    //      - resS_OK: The Call-ID has been successfully updated.
    //
    //  Description:
    //      This parameter has two usages. The first usage is the Call-ID header
    //      of requests sent through the context to which this user agent
    //      service is attached. It is used as the Call-ID header of requests
    //      created with CreateBasicRequest.
    //
    //      The second usage is to match the Call-ID of incoming packets when
    //      this dialog is in the eIN_DIALOG state.
    //
    //      One should not change this parameter after a dialog has been
    //      established on this user agent service as it has the effect of
    //      breaking the dialog matching mechanism both at the local application
    //      and the remote peer.
    //
    //      This parameter does not need to be configured by the application. By
    //      default, the stack generates a new random Call-ID for each new
    //      user agent service instance. It is also automatically updated with
    //      the information in the parameter to Establish when this method is
    //      called. The default Call-ID can still be accessed through the
    //      GetCallId method.
    //
    //      It is important to understand that if <C>rCallID</C> contains raw
    //      data, this data is used whithout modification when creating SIP
    //      packets for the associated context. Refer to CSipHeader for more
    //      details on Raw versus Parsed header.
    //
    //  See Also:
    //      GetCallId, CreateBasicRequest, CSipDialogMatcherList
    //
    //==EDOC====================================================================
    virtual mxt_result SetCallId(IN const CSipHeader& rCallID) = 0;

    //==SDOC====================================================================
    //==
    //==  SetRemoteAddr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the address to put in the To headers of outgoing requests.
    //
    //  Parameters:
    //    rNameAddr:
    //      The named-addr to put in the To headers of outgoing requests.
    //
    //  Description:
    //      Sets the name addr to put in the To header of requests created with
    //      CreateBasicRequest.
    //
    //      This parameter is not used in dialog matching. However, the
    //      application should be aware that changing this parameter while the
    //      user agent service is in the eIN_DIALOG state can cause
    //      interoperability failure with older user agent that comply with RFC
    //      2543 instead of RFC 3261. This is because this parameter was used in
    //      dialog matching then.
    //
    //      Note that this parameter is not directly used as the named-addr in
    //      the From header of the response sent. Instead the From header of the
    //      received request is copied into responses.
    //
    //      This parameter does not have a default value and must be set when
    //      the user agent service is attached to a context created to send a
    //      request. It is not necessary to set its value if the context is used
    //      only to receive requests until a dialog (early or not) is
    //      established on it (through the Establish method). When Establish is
    //      called, this parameter is overriden with the information in the
    //      packet in parameter.
    //
    //      This parameter can be obtained through GetRemoteAddr.
    //
    //  See Also:
    //      GetRemoteAddr, CreateBasicRequest, Establish
    //
    //==EDOC====================================================================
    virtual void SetRemoteAddr(IN const CNameAddr& rNameAddr) = 0;

    //==SDOC====================================================================
    //==
    //==  SetRemoteTag
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the tag parameter to put in the To header of requests sent.
    //
    //  Parameters:
    //    rstrTag:
    //      The tag parameter to put in the To header of requests sent.
    //
    //  Description:
    //      Sets the tag parameter to put in the To header of requests created
    //      with CreateBasicRequest.
    //
    //      This parameter is also used to match the dialog for incoming packets
    //      when the user agent service is in the eIN_DIALOG state. When in such
    //      a state, its value must match the tag of the To header of incoming
    //      responses or the tag of the From header of incoming requests. If it
    //      does not match, the dialog does not match and the packet must be
    //      handled by another context.
    //
    //      The exception to this matching rule is when a forking request is
    //      sent and a forked packet is received. In this case, and when the
    //      dialog state is in eOUT_OF_DIALOG, the remote tag parameter is not
    //      used to match the dialog.
    //
    //      One should not change this parameter after a dialog has been
    //      established on this user agent service as it has the effect of
    //      breaking the dialog matching mechanism both at the local application
    //      and the remote peer.
    //
    //      This parameter does not need to be configured by the application. It
    //      is automatically updated with the information in the parameter to
    //      Establish when this method is called.
    //
    //      This parameter can be obtained through GetRemoteTag. It is also
    //      contained in the parameters returned by
    //      GetRemoteDescriptorParameters unless it is an empty string.
    //
    //  See Also:
    //      GetRemoteTag, GetRemoteDescriptorParameters, CreateBasicRequest,
    //      Establish.
    //
    //==EDOC====================================================================
    virtual void SetRemoteTag(IN const CString& rstrTag) = 0;

    //==SDOC====================================================================
    //==
    //==  SetRemoteDescriptorParameters
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the parameters to put in the To header of requests sent
    //      including the Tag parameter.
    //
    //  Parameters:
    //    pParameters:
    //      The list of parameters to put in the To header of requests to send.
    //      Ownership is taken unless an error is returned.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The parameter list contains invalid
    //      parameters. Ownership of the paramter list is not taken.
    //
    //      - resS_OK: The parameter list has been successfully updated.
    //
    //  Description:
    //      Sets the header parameters to put in the To header of requests
    //      created with CreateBasicRequest.
    //
    //      These parameters include the Tag parameter. The remote Tag parameter
    //      usage is described in SetRemoteTag.
    //
    //      One should not change the Remote tag with this method once a dialog
    //      is established as with the SetRemoteTag method. Other parameters can
    //      be changed but be aware that changing such parameters could lead to
    //      interoperability problems with non-compliant peers.
    //
    //      This parameter list does not need to be configured by the
    //      application. It is automatically updated with the information in the
    //      parameter to Establish when this method is called.
    //
    //      This parameter list can be obtained through
    //      GetRemoteDescriptorParameters.
    //
    //
    //  See Also:
    //      SetRemoteTag, GetRemoteDescriptorParameters
    //
    //==EDOC====================================================================
    virtual mxt_result SetRemoteDescriptorParameters(
                                          IN TO CGenParamList* pParameters) = 0;

    //==SDOC====================================================================
    //==
    //==  SetLocalAddr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the address to put in the From headers of outgoing requests.
    //
    //  Parameters:
    //    rNameAddr:
    //      The named-addr to put in the From headers of outgoing requests.
    //
    //  Description:
    //      Sets the name addr to put in the From headers of requests created
    //      with CreateBasicRequest.
    //
    //      This parameter is not used in dialog matching. However, the
    //      application should be aware that changing this parameter while the
    //      user agent service is in the eIN_DIALOG state can cause
    //      interoperability failure with older user agent that comply with RFC
    //      2543 instead of RFC 3261. This is because this parameter was used in
    //      dialog matching then.
    //
    //      Note that this parameter is not directly used as the named-addr in
    //      the To header of the response sent. Instead, the To header of the
    //      received request is copied into responses.
    //
    //      When a context is established, the information in the packet in
    //      parameter to the Establish method is used. This parameter must be
    //      valid for CreateBasicRequest to succeed.
    //
    //      This parameter can be obtained through GetLocalAddr.
    //
    //  See Also:
    //      GetLocalAddr, CreateBasicRequest, Establish
    //
    //==EDOC====================================================================
    virtual void SetLocalAddr(IN const CNameAddr& rNameAddr) = 0;

    //==SDOC====================================================================
    //==
    //==  SetLocalTag
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the tag parameter to put in the From header of requests sent.
    //
    //  Parameters:
    //    rstrTag:
    //      The tag parameter to put in the From header of requests sent.
    //
    //  Description:
    //      Sets the tag parameter to put in the From header of requests created
    //      with CreateBasicRequest.
    //
    //      This parameter is also used to match the dialog for incoming packets
    //      when the user agent service is in the eIN_DIALOG state. When in such
    //      a state, its value must match the tag of the From header of incoming
    //      responses or the tag of the To header of incoming requests. If it
    //      does not match, the dialog does not match and the packet must be
    //      handled by another context.
    //
    //      One should not change this parameter after a dialog has been
    //      established on this user agent service as it has the effect of
    //      breaking the dialog matching mechanism both at the local application
    //      and the remote peer.
    //
    //      This parameter does not need to be configured by the application. A
    //      new random value is generated for every new user agent service
    //      instance. The value is also updated from the packet in parameter
    //      when Establish is called.
    //
    //      This parameter can be obtained through GetLocalTag. It is also
    //      contained in the parameters returned by
    //      GetLocalDescriptorParameters unless it is an empty string.
    //
    //  See Also:
    //      GetLocalTag, GetLocalDescriptorParameters, CreateBasicRequest,
    //      Establish.
    //
    //==EDOC====================================================================
    virtual void SetLocalTag(IN const CString& rstrTag) = 0;

    //==SDOC====================================================================
    //==
    //==  SetLocalDescriptorParameters
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the parameters to put in the From header of requests sent
    //      including the Tag parameter.
    //
    //  Parameters:
    //    pParameters:
    //      The list of parameters to put in the From header of requests to
    //      send. Ownership is taken unless an error is returned.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The parameter list contains invalid
    //      parameters. Ownership of the parameter list is not taken.
    //
    //      - resS_OK: The parameter list has been successfully updated.
    //
    //  Description:
    //      Sets the header parameters to put in the From header of requests
    //      created with CreateBasicRequest.
    //
    //      These parameters include the Tag parameter. The local Tag parameter
    //      usage is described in SetLocalTag.
    //
    //      One should take the same care not to change the local tag with this
    //      method once a dialog is established as with the SetLocalTag method.
    //      Other parameters can be changed but be aware that changing such
    //      parameters could lead to interoperability problems with non
    //      compliant peers.
    //
    //      This parameter list does not need to be configured by the
    //      application. It is automatically updated with the information in the
    //      parameter to Establish when this method is called.
    //
    //      This parameter list can be obtained through
    //      GetLocalDescriptorParameters.
    //
    //      Note that these parameters are not used to create the To header
    //      parameters of the responses sent. The To header of the responses
    //      sent is copied from the request received, adding a Tag parameter if
    //      none is present.
    //
    //  See Also:
    //      SetLocalTag, GetLocalDescriptorParameters
    //
    //==EDOC====================================================================
    virtual mxt_result SetLocalDescriptorParameters(
                                          IN TO CGenParamList* pParameters) = 0;

    //==SDOC====================================================================
    //==
    //==  SetLocalSequenceNumber
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the sequence number to put in the requests to send.
    //
    //  Parameters:
    //    uSequenceNumber:
    //      The sequence number to put in the requests to send.
    //
    //  Description:
    //      Sets the sequence number put in the CSeq header of the requests
    //      created with CreateBasicRequest.
    //
    //      This parameter is not used locally to match the dialog. However, one
    //      should take care when changing this parameter. If its value is
    //      lowered after a request has been sent, the remote peer will probably
    //      consider the requests it receives out of order and answer them with
    //      a 500 failure response.
    //
    //      This parameter is increased by one each time CreateBasicRequest is
    //      called successfully.
    //
    //      By default, the user agent service initializes this value with a
    //      random number between 1 and (2^31 - 1) inclusively.
    //
    //      This value is accessible through GetLocalSequenceNumber.
    //
    //  See Also:
    //      GetLocalSequenceNumber, CreateBasicRequest
    //
    //==EDOC====================================================================
    virtual void SetLocalSequenceNumber(IN uint32_t uSequenceNumber) = 0;

    //==SDOC====================================================================
    //==
    //==  SetRemoteSequenceNumber
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the sequence number expected in the next request received.
    //
    //  Parameters:
    //    uSequenceNumber:
    //      The expected remote sequence number.
    //
    //  Description:
    //      Sets the sequence number that is expected in the CSeq of the next
    //      request received on this dialog.
    //
    //      This parameter has meaning only when the state is eIN_DIALOG. When
    //      the state is eOUT_OF_DIALOG, it has absolutely no effect. When the
    //      state is eIN_DIALOG, the user agent service automatically rejects
    //      requests for which the CSeq is smaller than or equal to this
    //      parameter.
    //
    //      The application does not need to set this parameter. By default, it
    //      is initialized with the value in the CSeq header of the packet in
    //      parameter to Establish, when this method is called with a received
    //      request or with the CSeq value of the first request received when
    //      eIN_DIALOG is true.
    //
    //      If one changes its value after it has been initialized, it could
    //      lead the application to either accept out of order requests from the
    //      remote peer or reject its valid requests as out of order requests.
    //
    //      This value can be obtained through GetRemoteSequenceNumber.
    //
    //  See Also:
    //      Establish, GetRemoteSequenceNumber
    //
    //==EDOC====================================================================
    virtual void SetRemoteSequenceNumber(IN uint32_t uSequenceNumber) = 0;

    //==SDOC====================================================================
    //==
    //==  SetRemoteSequenceNumber64bits
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the sequence number expected in the next request received.
    //
    //  Parameters:
    //    uSequenceNumber:
    //      The expected remote sequence number.
    //
    //  Description:
    //      Sets the sequence number that is expected in the CSeq of the next
    //      request received on this dialog.
    //
    //      This parameter has meaning only when the state is eIN_DIALOG. When
    //      the state is eOUT_OF_DIALOG it has absolutely no effect. When the
    //      state is eIN_DIALOG, the user agent service automatically
    //      rejects requests for which the CSeq is smaller than or equal to this
    //      parameter.
    //
    //      The application does not need to set this parameter. By default it
    //      is initialized with the value in the CSeq header of the packet in
    //      parameter to Establish when this method is called with a received
    //      request or with the CSeq value of the first request received when
    //      eIN_DIALOG is true.
    //
    //      If one changes its value after it has been initialized, it could
    //      lead the application to either accept out of order requests from the
    //      remote peer or reject its valid requests as out of order requests.
    //
    //      This value can be obtained through GetRemoteSequenceNumber.
    //
    //  See Also:
    //      Establish, GetRemoteSequenceNumber
    //
    //==EDOC====================================================================
    virtual void SetRemoteSequenceNumber64bits(IN uint64_t uSequenceNumber) = 0;

    //==SDOC====================================================================
    //==
    //==  AcceptOutOfOrderRemoteRequests
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the sequence number expected in the next request received.
    //
    //  Parameters:
    //    bAccept:
    //      True to accept out of order remote requests or false to reject
    //      them.
    //
    //  Description:
    //      This method can be used to force the user agent service to accept
    //      remote requests regardless of the CSeq number set inside. In some
    //      cases, a remote server could crash and try to properly keep a
    //      subscription alive but it will reset the CSeq number to a lower
    //      value.
    //
    //      By default the stack automatically rejects out of order requests.
    //
    //  NOTES:
    //      Setting this to true can lead to some unforeseen application
    //      behaviour and will render a UA non conformant to RFC 3261.
    //
    //  See Also:
    //      SetRemoteSequenceNumber, GetRemoteSequenceNumber
    //
    //==EDOC====================================================================
    virtual void AcceptOutOfOrderRemoteRequests(IN bool bAccept) = 0;

    //-- << Dialog Id Getters >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  GetLocalContact
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the Contact to put in Contact refresh requests and responses
    //      sent.
    //
    //  Returns:
    //      The Contact header to put in Contact refresh requests and responses.
    //
    //  Description:
    //      Gets the Contact used by services to determine the Contact to put in
    //      the Contact refresh requests and responses to those requests. This
    //      method is usually used by services that send or receive target
    //      refresh requests or responses, but the application must set this
    //      parameter value through AddLocalContact.
    //
    //  See Also:
    //      AddLocalContact, GetLocalContactVector
    //
    //==EDOC====================================================================
    virtual const CSipHeader& GetLocalContact() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetLocalContactVector
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured Contact list.
    //
    //  Returns:
    //      The vector of the Contact header.
    //
    //  Description:
    //      Gets the Contact vector used by services to determine the Contact
    //      to put in the Contact refresh requests and responses to those
    //      requests. This method should be used by the application to
    //      change a Contact, however Contact addition should be made through
    //      the AddLocalContact method.
    //
    //  See Also:
    //      AddLocalContact, GetLocalContact
    //
    //==EDOC====================================================================
    virtual CVector<CSipHeader*>& GetLocalContactVector() = 0;

    //==SDOC====================================================================
    //==
    //==  GetPreloadedRoute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the Route header(s) to put in the out-of-dialog requests sent.
    //
    //  Returns:
    //      The Route headers to put in out-of-dialog client requests. It can be
    //      NULL, which means that no preloaded Route is used.
    //
    //  Description:
    //      Gets the Route headers to put in the out-of-dialog requests sent by
    //      contexts that use this ISipUserAgentSvc.
    //
    //      This Route is used only when no dialog is established yet (such as
    //      when sending a dialog establishing INVITE or sending a REGISTER).
    //      When a dialog is established, the rules of RFC 3261 dictate how the
    //      Route is computed according to the requests and responses that
    //      established the dialog.
    //
    //  See Also:
    //      SetPreloadedRoute, GetEstablishedRoute
    //
    //==EDOC====================================================================
    virtual const CSipHeader* GetPreloadedRoute() const = 0;


    //==========================================================================
    //==
    //==  GetEstablishedRoute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the Route header(s) placed in requests sent within an
    //      established dialog.
    //
    //  Returns:
    //      The Route headers placed in in-dialog requests. It can be NULL,
    //      which means that no Route was yet established within the dialog.
    //
    //  Description:
    //      Gets the Route headers placed in in-dialog requests sent by
    //      contexts that use this ISipUserAgentSvc.
    //
    //      When a dialog is established, the rules of RFC 3261 dictate how the
    //      established Route is computed according to the requests and
    //      responses that established the dialog.
    //
    //  See Also:
    //      GetPreloadedRoute, SetEstablishedRoute
    //
    //==========================================================================
    virtual const CSipHeader* GetEstablishedRoute() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetCurrentTarget
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the current target where to send the requests.
    //
    //  Returns:
    //      The URI to put in the Request-URI of outgoing requests. It can be
    //      NULL.
    //
    //  Description:
    //      Gets the URI used as the Request-URI when creating a request with
    //      CreateBasicRequest. It returns NULL if the current target is
    //      uninitialized.
    //
    //      It can be set through SetCurrentTarget.
    //
    //  See Also:
    //      CreateBasicRequest, SetCurrentTarget
    //
    //==EDOC====================================================================
    virtual const IUri* GetCurrentTarget() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetCallId
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the Call-ID for this context.
    //
    //  Returns:
    //      The Call-ID to put in outgoing requests and that must be found in
    //      incoming requests.
    //
    //  Description:
    //      This parameter has two usages. The first usage is the Call-ID header
    //      of requests sent through the context to which this user agent
    //      service is attached. It is put as the Call-ID header of requests
    //      created with CreateBasicRequest.
    //
    //      The second usage is to match the Call-ID of incoming packets when
    //      this dialog is in the eIN_DIALOG state.
    //
    //      By default, the stack generates a new random Call-ID for each new
    //      user agent service instance. It is also automatically updated with
    //      the information in the parameter to Establish when this method
    //      is called.
    //
    //      The Call-ID can also be set through SetCallId.
    //
    //  See Also:
    //      CreateBasicRequest, CSipDialogMatcherList, SetCallId
    //
    //==EDOC====================================================================
    virtual const CSipHeader& GetCallId() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetRemoteAddr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the address to put in the To headers of outgoing requests.
    //
    //  Returns:
    //      The named-addr to put in the To headers of outgoing requests. It can
    //      be invalid.
    //
    //  Description:
    //      Gets the name addr to put in the To header of requests created with
    //      CreateBasicRequest.
    //
    //      This parameter is not used in dialog matching.
    //
    //      Note that this parameter is not directly used as the named-addr
    //      in the From header of the response sent. The From header of the
    //      received request is rather copied into responses.
    //
    //      This parameter does not have a default value and must be set when
    //      the user agent service is attached to a context created to send a
    //      request. It is not necessary to set its value if the context is
    //      used only to receive requests until a dialog (early or not) is
    //      established on it (through the Establish method). When Establish is
    //      called, this parameter is overriden with the information in the
    //      packet in parameter. To send an out-of-dialog request (including one
    //      that can establish a new dialog), the application must set this
    //      parameter through SetRemoteAddr.
    //
    //      This method returns an invalid CNameAddr if this parameter is
    //      uninitialized.
    //
    //  See Also:
    //      SetRemoteAddr, CreateBasicRequest, Establish
    //
    //==EDOC====================================================================
    virtual const CNameAddr& GetRemoteAddr() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetRemoteTag
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the tag parameter to put in the To header of requests sent.
    //
    //  Returns:
    //      The tag parameter to put in the To header of requests sent. It can
    //      be an empty string, which means that no tag is put in requests sent.
    //
    //  Description:
    //      Gets the tag parameter to put in the To header of requests created
    //      with CreateBasicRequest.
    //
    //      This parameter is also used to match the dialog for incoming packets
    //      when the user agent service is in the eIN_DIALOG state. When in such
    //      state, its value must match the tag of the To header of incoming
    //      responses or the tag of the From header of incoming requests. If it
    //      does not match, the dialog does not match and the packet must be
    //      handled by another context.
    //
    //      The exception to this matching rule is when a forking request is
    //      sent and a forked packet is received. In this case, and when the
    //      dialog state is in eOUT_OF_DIALOG, the remote tag parameter is not
    //      used to match the dialog.
    //
    //      This parameter does not need to be configured by the application. It
    //      is automatically updated with the information in the parameter to
    //      Establish when this method is called.
    //
    //      This parameter can be set through SetRemoteTag.
    //
    //      It is contained in the parameters returned by
    //      GetRemoteDescriptorParameters unless it is an empty string.
    //
    //  See Also:
    //      SetRemoteTag, GetRemoteDescriptorParameters, CreateBasicRequest,
    //      Establish.
    //
    //==EDOC====================================================================
    virtual const CString& GetRemoteTag() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetRemoteDescriptorParameters
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the parameters to put in the To header of requests sent
    //      including the Tag parameter.
    //
    //  Returns:
    //      A pointer to the list of parameters to put in the To header of
    //      requests to send. It can be NULL, which means that no parameter
    //      would be added in the To header.
    //
    //  Description:
    //      Gets the header parameters to put in the To header of requests
    //      created with CreateBasicRequest.
    //
    //      These parameters include the Tag parameter. The remote Tag parameter
    //      usage is described in GetRemoteTag.
    //
    //      This parameter list does not need to be configured by the
    //      application. It is automatically updated with the information in the
    //      parameter to Establish when this method is called.
    //
    //      This parameter list can be set through
    //      SetRemoteDescriptorParameters.
    //
    //
    //  See Also:
    //      SetRemoteTag, SetRemoteDescriptorParameters
    //
    //==EDOC====================================================================
    virtual const CGenParamList* GetRemoteDescriptorParameters() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetLocalAddr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the address to put in the From headers of outgoing requests.
    //
    //  Returns:
    //      The named-addr to put in the From headers of outgoing requests. It
    //      can be invalid.
    //
    //  Description:
    //      Gets the name addr to put in the From header of requests created
    //      with CreateBasicRequest.
    //
    //      This parameter is not used in dialog matching.
    //
    //      Note that this parameter is not directly used as the named-addr
    //      in the To header of the response sent. The To header of the received
    //      request is rather copied into the responses.
    //
    //      When a context is established, the information in the packet in
    //      parameter to the Establish method is used. This parameter must be
    //      valid for CreateBasicRequest to succeed.
    //
    //      This parameter can be set through SetLocalAddr.
    //
    //  See Also:
    //      SetLocalAddr, CreateBasicRequest, Establish
    //
    //==EDOC====================================================================
    virtual const CNameAddr& GetLocalAddr() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetLocalTag
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the tag parameter to put in the From header of requests sent.
    //
    //  Returns:
    //      The tag parameter to put in the From header of requests sent. It can
    //      be an empty string, which means that no tag is put in the From
    //      header.
    //
    //  Description:
    //      Gets the tag parameter to put in the From header of requests created
    //      with CreateBasicRequest.
    //
    //      This parameter is also used to match the dialog for incoming packets
    //      when the user agent service is in the eIN_DIALOG state. When in such
    //      a state, its value must match the tag of the From header of incoming
    //      responses or the tag of the To header of incoming requests. If it
    //      does not match, the dialog does not match and the packet must be
    //      handled by another context.
    //
    //      This parameter does not need to be configured by the application. A
    //      new random value is generated for every new user agent service
    //      instance. The value is also updated from the packet in parameter
    //      when Establish is called.
    //
    //      This parameter can be set through SetLocalTag.
    //
    //      It is contained in the parameters returned by
    //      GetLocalDescriptorParameters unless it is an empty string.
    //
    //  See Also:
    //      SetLocalTag, GetLocalDescriptorParameters, CreateBasicRequest,
    //      Establish.
    //
    //==EDOC====================================================================
    virtual const CString& GetLocalTag() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetLocalDescriptorParameters
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the parameters to put in the From header of requests sent
    //      including the Tag parameter.
    //
    //  Returns:
    //      A pointer to the list of parameters to put in the From header of
    //      requests to send. It can be NULL, which means that no parameters are
    //      put in the From header.
    //
    //  Description:
    //      Gets the parameters to put in the From header of requests created
    //      with CreateBasicRequest.
    //
    //      These parameters include the Tag parameter. The local Tag parameter
    //      usage is described in GetLocalTag.
    //
    //      This parameter list does not need to be configured by the
    //      application. It is automatically updated with the information in the
    //      parameter to Establish when this method is called.
    //
    //      This parameter list can be set through
    //      SetLocalDescriptorParameters.
    //
    //
    //  See Also:
    //      GetLocalTag, SetLocalDescriptorParameters
    //
    //==EDOC====================================================================
    virtual const CGenParamList* GetLocalDescriptorParameters() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetLocalSequenceNumber
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the sequence number to put in the requests to send.
    //
    //  Returns:
    //    uSequenceNumber:
    //      The sequence number to put in the requests to send.
    //
    //  Description:
    //      Gets the sequence number put in the CSeq header of the next requests
    //      created with CreateBasicRequest.
    //
    //      This parameter is not used locally to match the dialog.
    //
    //      This parameter is increased by one each time CreateBasicRequest is
    //      called successfully.
    //
    //      By default, the user agent service initializes this value with a
    //      random number between 1 and (2^31 - 1) inclusively.
    //
    //      This value can be set through SetLocalSequenceNumber.
    //
    //  See Also:
    //      SetLocalSequenceNumber, CreateBasicRequest
    //
    //==EDOC====================================================================
    virtual uint32_t GetLocalSequenceNumber() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetRemoteSequenceNumber
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the sequence number expected in the next request received.
    //
    //  Returns:
    //      The expected remote sequence number.
    //
    //  Description:
    //      Gets the sequence number that is expected in the CSeq of the next
    //      request received on this dialog.
    //
    //      This parameter has meaning only when the state is eIN_DIALOG. When
    //      the state is eOUT_OF_DIALOG, it has absolutely no effect. When the
    //      state is eIN_DIALOG, the user agent service automatically rejects
    //      requests for which the CSeq is smaller than or equal to this
    //      parameter.
    //
    //      The application does not need to set this parameter. By default it
    //      is initialized with the value in the CSeq header of the packet in
    //      parameter to Establish when this method is called with a received
    //      request or a sent response. Otherwise, it is initialized with the
    //      CSeq value of the first request received when eIN_DIALOG is true.
    //
    //      If one changes its value after it has been initialized, it could
    //      lead the application to either accept out-of-order requests from the
    //      remote peer or reject its valid requests as out-of-order requests.
    //
    //      This value can be set through SetRemoteSequenceNumber.
    //
    //  See Also:
    //      Establish, SetRemoteSequenceNumber
    //
    //==EDOC====================================================================
    virtual uint32_t GetRemoteSequenceNumber() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetRemoteSequenceNumber64bits
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the sequence number expected in the next request received.
    //
    //  Returns:
    //      The expected remote sequence number.
    //
    //  Description:
    //      Gets the sequence number that is expected in the CSeq of the next
    //      request received on this dialog.
    //
    //      This parameter has meaning only when the state is eIN_DIALOG. When
    //      the state is eOUT_OF_DIALOG it has absolutely no effect. When the
    //      state is eIN_DIALOG, the user agent service automatically
    //      rejects requests for which the CSeq is smaller than or equal to this
    //      parameter.
    //
    //      The application does not need to set this parameter. By default it
    //      is initialized with the value in the CSeq header of the packet in
    //      parameter to Establish when this method is called with a received
    //      request or a sent response. Otherwise, it is initialized with
    //      the CSeq value of the first request received when eIN_DIALOG is
    //      true.
    //
    //      If one changes its value after it has been initialized, it could lead
    //      the application to either accept out of order requests from the
    //      remote peer or reject its valid requests as out of order requests.
    //
    //      This value can be set through SetRemoteSequenceNumber.
    //
    //  See Also:
    //      Establish, SetRemoteSequenceNumber
    //
    //==EDOC====================================================================
    virtual uint64_t GetRemoteSequenceNumber64bits() const = 0;

    //==========================================================================
    //==
    //==  SetInstanceId
    //==
    //==========================================================================
    //<FLAG New in 4.1.6>
    //
    //  Parameters:
    //      rstrInstanceId:
    //       The instance-id.
    //
    //  Description:
    //      Sets the instance-id used to add in Contacts of REGISTER requests
    //      sent over an outbound connection. This instance-id is unique to each
    //      user-agent.
    //
    //  See Also:
    //      ISipOutboundConnectionSvc
    //
    //==========================================================================
    virtual void SetInstanceId(IN const CString& rstrInstanceId) = 0;

    //==========================================================================
    //==
    //==  GetInstanceId
    //==
    //==========================================================================
    //<FLAG New in 4.1.6>
    //
    //  Returns:
    //      The instance-id.
    //
    //  Description:
    //      Gets the instance-id of this user-agent.
    //
    //==========================================================================
    virtual const CString& GetInstanceId() const = 0;

    //-- << Dialog State >>
    //--------------------------------------------

    // Description:
    //  Indicate if it's a received or a sent packet.
    enum EPacketDirection
    {
        // Description:
        //  Packet was received.
        eRECEIVED,
        // Description:
        //  Packet was sent.
        eSENT,
    };

    // Description:
    //  Indicate the state of the dialog.
    enum EState
    {
        // Description:
        //  There is at least one active dialog usage on this context.
        eIN_DIALOG,
        // Description:
        //  Sent a request that permits the peer to create the dialog with a request.
        eWAITING_FOR_DIALOG_COMPLETION,
        // Description:
        //  There is no dialog usage on this context.
        eOUT_OF_DIALOG
    };

    //==SDOC====================================================================
    //==
    //==  Establish
    //==
    //==========================================================================
    //
    //  Summary:
    //      Establishes a dialog with a packet sent or received.
    //
    //  Parameters:
    //    rPacket:
    //      The packet that establishes the dialog.
    //
    //    eDirection:
    //      eSENT when the packet is sent to a remote peer, eRECEIVED when the
    //      packet is received from a remote peer.
    //
    //  Returns:
    //      - resFE_FAIL: A dialog has already been established on this service
    //      AND the dialog identifiers of the packet in parameter do not match
    //      those of this dialog. Multiple calls to establish happen for
    //      instance when doing call transfer with REFER on an existing INVITE
    //      dialog. In this case, the dialog identifiers of the 2xx to the REFER
    //      or those of the NOTIFY MUST match the dialog identifiers of the
    //      INVITE dialog. If they do not match, the REFER or the NOTIFY do not
    //      belong to that dialog in the first place and must be handled by
    //      another context.
    //
    //      This value is also returned for an attempt to establish a dialog
    //      with a '100 Trying' or a '>=300' response.
    //
    //      - resSW_DIALOG_NOT_REGISTERED: All the processing was done to
    //      establish the dialog but the dialog was not registered in the dialog
    //      matcher list. This happens only when Clear was called on the
    //      context, so the service will be destroyed soon. It means that no
    //      request matching the dialog will be received.
    //
    //      - resS_OK: The dialog is properly initialized.
    //
    //  Description:
    //      This method MUST be called when a packet that establishes a dialog
    //      is sent or received. Normally, it is called by the service that
    //      handles this type of dialog (EventSvc, SessionSvc).
    //
    //      If the application implements a new type of request that establishes
    //      a dialog through the GenericSvc, it MUST call this method itself.
    //      The GenericSvc never calls this method when used as a UAC. It calls
    //      this method as a UAS when its ISipDialogServerEventCtrl interface
    //      is used to send a response for a received request.
    //
    //      Once this method is called, this service registers itself with
    //      the CSipDialogMatcherList if it is not already done. It also
    //      switches to a state where it only accepts requests and responses
    //      for which the dialog identifiers (local tag, remote tag, Call-ID)
    //      match those of the packet in parameter. It also automatically
    //      manages out-of-order CSeq number in received requests on this dialog.
    //
    //      Note that this service has no knowledge of when the dialog
    //      terminates. The entity that called this method is also responsible
    //      to call TerminateUsage in order to terminate the dialog.
    //
    //      In the particular case of the GenericSvc, it is the application's
    //      responsibility to terminate the dialog, even when the service itself
    //      called this method. The call was made on behalf of the application
    //      and it is considered as if the application made it itself.
    //
    //  See Also:
    //      TerminateUsage, EstablishForSentRequest
    //
    //==EDOC====================================================================
    virtual mxt_result Establish(IN const CSipPacket& rPacket,
                                 IN EPacketDirection eDirection) = 0;

    //==SDOC====================================================================
    //==
    //==  EstablishForSentRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Establishes a dialog for a request to be sent.
    //
    //  Returns:
    //      - resFE_FAIL: no dialog matcher list has been initialized.
    //
    //      - resSW_DIALOG_NOT_REGISTERED: All the processing was done to
    //      establish the dialog but the dialog was not registered in the dialog
    //      matcher list. This happens only when Clear was called on the
    //      context, so the service will be destroyed soon. It means that no
    //      request matching the dialog will be received anymore.
    //
    //      - resS_OK: The dialog is properly created.
    //
    //  Description:
    //      This method MUST be called when a request to be sent needs to
    //      establish a dialog. This method should only be called when requests
    //      should be accepted between the time the request is sent and the time
    //      its final response is received (SUBSCRIBE for example). This method
    //      should only be used in the services managing this type of method. If
    //      the application implements a new type of request that establishes a
    //      dialog through the GenericSvc, it MUST call this method itself. The
    //      GenericSvc never calls this method.
    //
    //      Once this method is called, this service registers itself with
    //      the CSipDialogMatcherList if it is not already done. It also
    //      switches to a state where it only accepts requests and responses
    //      for which the dialog identifiers (local tag, remote tag, Call-ID)
    //      match those configured in the service. Note that every request or
    //      response with the same local tag and Call-ID matches this dialog
    //      until one is received. When so, only requests and responses with the
    //      same dialog identifiers are accepted. This service also
    //      automatically manages out-of-order CSeq number in received requests
    //      on this dialog.
    //
    //      Note that this service has no knowledge of when the dialog
    //      terminates. The entity that called this method is also responsible
    //      to call TerminateUsage in order to terminate the dialog.
    //
    //      The CompleteDialogData method must be used when all dialog
    //      informations are known. This means when the first of a request
    //      received for the same dialog or a non-100 response to the request
    //      that was sent.
    //
    //  See Also:
    //      TerminateUsage, Establish
    //
    //==EDOC====================================================================
    virtual mxt_result EstablishForSentRequest() = 0;

    //==SDOC====================================================================
    //==
    //==  CompleteDialogData
    //==
    //==========================================================================
    //
    //  Summary:
    //      Completes the dialog information with the received packet.
    //
    //  Parameters:
    //    rReceivedPacket:
    //      The received packet from which the dialog data is completed.
    //
    //  Returns:
    //      - resFE_FAIL: no dialog matcher list has been initialized.
    //
    //      - resFE_FAIL: the packet does not match the current dialog.
    //
    //      - resFE_FAIL: one or more mandatory headers are missing to get the
    //      complete dialog informations. These headers are Call-ID, From, To,
    //      and CSeq.
    //
    //      - resS_OK: the dialog information has been correctly initialized.
    //
    //  Description:
    //      This method MUST be called when a packet is received on the dialog
    //      initialized with EstablishForSentRequest.
    //
    //      This method completes and updates the current dialog information
    //      into the ISipUserAgentSvc.
    //
    //  See Also:
    //      EstablishForSentRequest
    //
    //==EDOC====================================================================
    virtual mxt_result CompleteDialogData(
                                    IN const CSipPacket& rReceivedPacket ) = 0;

    //==SDOC====================================================================
    //==
    //==  UpdateRoute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the Route set of a dialog.
    //
    //  Parameters:
    //    rPacket:
    //      The packet that refreshes the Route.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: There is no dialog established in this
    //      context.
    //
    //      - resFE_INVALID_ARGUMENT: The packet in parameter is not a 2xx
    //      response to an INVITE.
    //
    //      - resS_OK: The Route has been properly updated.
    //
    //  Description:
    //      This method updates the Route set with the Record-Route headers
    //      found in the packet in parameter. This method only makes sense when
    //      receiving a 2xx to an INVITE.
    //
    //      In most cases, the Route cannot be changed once a dialog is
    //      established and, since Establish already takes care of updating the
    //      Route set, there is no need to call this method. The only exception
    //      is when an early dialog becomes confirmed as per RF C3261 terms.
    //      This object makes no distinction between early and confirmed dialogs.
    //      The dialog is established when it enters the early state and it is
    //      still established when it becomes confirmed. However, since on this
    //      occasion the Route set can be updated, a method to do so is needed.
    //      Hence, this method should only be called when a dialog transitions
    //      from early to confirmed.
    //
    //      It is invalid to call this method if the dialog state is
    //      eOUT_OF_DIALOG. In that case, only the Preloaded Route is used and
    //      the Route has no meaning.
    //
    //      The application does not need to bother with this as it is
    //      automatically handled by the ISipSessionSvc.
    //
    //  See Also:
    //      Establish, GetState, ISipSessionSvc
    //
    //==EDOC====================================================================
    virtual mxt_result UpdateRoute(IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  TerminateUsage
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates a usage of a dialog.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: There is no dialog established in this
    //      context.
    //
    //      - resSW_DIALOG_ALREADY_UNREGISTERED: Processing has been done but
    //      the dialog was already unregistered from the dialog matcher list.
    //      This happens only when Clear was called on the Context.
    //
    //      - resS_OK: The dialog usage has been successfully terminated. Note
    //      that there could still be other usages, hence it does not mean that
    //      the dialog is terminated.
    //
    //  Description:
    //      This method must be called when a usage of a dialog is terminated.
    //      It could be when a final negative response for an INVITE is received
    //      after a provisional response that established a dialog or when a BYE
    //      request is received to name a few.
    //
    //      This method MUST be called exactly once for each call to Establish.
    //      Calling it more often has an error returned. Calling it less often
    //      would mean that a dialog was established but never terminated.
    //      This is not an acceptable usage of dialogs.
    //
    //  See Also:
    //      Establish
    //
    //==EDOC====================================================================
    virtual mxt_result TerminateUsage() = 0;

    //==SDOC====================================================================
    //==
    //==  GetState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Obtains the dialog state.
    //
    //  Returns:
    //      - eIN_DIALOG: There is at least one active dialog usage on this
    //      context.
    //
    //      - eWAITING_FOR_DIALOG_COMPLETION: Sent a request that permits the
    //      peer to create the dialog with a request. For example, sent a
    //      SUBSCRIBE, for which a NOTIFY can be received before a response.
    //
    //      - eOUT_OF_DIALOG: There is no dialog usage on this context.
    //
    //  Description:
    //      Returns the dialog current state.
    //
    //  See Also:
    //      Establish, TerminateUsage
    //
    //==EDOC====================================================================
    virtual EState GetState() const = 0;

// BRCM_CUSTOM - [add] Control To-tag matching choices
    //==SDOC====================================================================
    //==
    //==  SetToTagMatchPerRfc3261
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configure To-tag matching option.
    //
    //  Parameters:
    //    bEnable:
    //      True to enable to-tag matching per RFC3261.
    //
    //  Description:
    //      This function can be used to disable the to-tag matching in the
    //      responses.
    //
    //      IMPORTANT : This variable MUST NOT be used to be RFC 3261 compliant
    //                  (RFC3261 section 8.2.6.2, second paragraph).
    //
    //      Setting this function to false disables the to-tag matching in the
    //      responses, and matches responses with the request even if the to-tag
    //      are different.  To-tag matching is enabled by default.
    //
    //      WARNING : Setting this function to false :
    //          - breaks the forking request support
    //          - breaks the PRACK functionnality when provisional responses are
    //            received with different to-tags. Only the first to-tag will be
    //            used in PRACK requests.
    //          - breaks the UPDATE functionnality when an update is received
    //            with a different to-tag than the first 1xx response. Also,
    //            only the first 1xx to-tag will be used in sent UPDATE requests.
    //
    //  See Also:
    //      IsToTagMatchPerRfc3261
    //
    //==EDOC====================================================================
    virtual void SetToTagMatchPerRfc3261(IN bool bEnable) = 0;
    //==SDOC====================================================================
    //==
    //==  IsToTagMatchPerRfc3261
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the To-tag matching option.
    //
    //  Returns:
    //    The To-tag matching option
    //
    //  Description:
    //      Returns the to-tag matching option. If the to-tag is to be matched
    //      in order for a dialog to be identified this will return true(RFC 3261
    //      compliant). When false, the to-tag is not considered(not RFC 3261 
    //      compliant) and responses will be matched with requests even if the 
    //      to-tags are different.
    //
    //  See Also:
    //      SetToTagMatchPerRfc3261
    //
    //==EDOC====================================================================
    virtual bool IsToTagMatchPerRfc3261() const = 0;
// BRCM_CUSTOM - [end] Control To-tag matching choices


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipUserAgentSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipUserAgentSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipUserAgentSvc(IN const ISipUserAgentSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipUserAgentSvc& operator=(IN const ISipUserAgentSvc& rSrc);
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPUSERAGENTSVC_H
