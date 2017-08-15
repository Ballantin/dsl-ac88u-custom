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
#ifndef MXG_IPRIVATESIPUSERAGENTSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IPRIVATESIPUSERAGENTSVC_H
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
#ifndef MXG_METHOD_H
#include "SipParser/Method.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_ISIPUSERAGENTSVC_H
#include "SipUserAgent/ISipUserAgentSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IPrivateSipUserAgentSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IPrivateSipUserAgentSvc
//========================================
//
// Description:
//   This interface provides access to private CSipUserAgent methods to the SIP
//   stack services. Applications must not query this interface as it is
//   designed for the stack internal use only.
//
//==EDOC========================================================================
class IPrivateSipUserAgentSvc : public ISipUserAgentSvc
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IPrivateSipUserAgentSvc);
    //M5T_INTERNAL_USE_END

    //-- << Internal Methods >>
    //---------------------------

    //==SDOC====================================================================
    //==
    //==  CreateBasicRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets basic information for the request in parameter.
    //
    //  Parameters:
    //    rPacket:
    //      The request to update.
    //
    //  Returns:
    //      - resFE_FAIL: The packet in parameter could not be properly updated.
    //
    //      - resS_OK: The packet in parameter was properly updated.
    //
    //  Description:
    //      This method should never be used directly by users of the stack. It
    //      should only be used by the user agent based services of the stack.
    //
    //      This method takes the information held by this service and sets it
    //      into the packet in parameter. This includes the request-URI, the
    //      topmost Via minus the branch ID and the transport, the From and To
    //      headers including tags, the Call-ID header, the Route headers, and
    //      the CSeq header.
    //
    //      Also, if there are headers in the Request-URI (set with
    //      SetCurrentTarget), they will replace all the headers of the same
    //      type in rPacket but will not update the user agent service. The
    //      headers will remain in the Request-URI of the service but will not
    //      be included in the Request-URI of the packet.
    //
    //  See Also:
    //      SetCurrentTarget
    //
    //==EDOC====================================================================
    virtual mxt_result CreateBasicRequest(INOUT CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  UpdateCurrentContact
    //==
    //==========================================================================
    //
    //  Parameters:
    //    rPacket:
    //      SIP packet where to update the Contact header.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The attached ISipUserAgentSvc is incorrectly
    //      configured. For instance, the contact list could be empty.
    //
    //      - resS_OK: The packet in parameter has been correctly updated.
    //
    //  Description:
    //      The request context core services use this method when an
    //      OnPacketReadyToSend event is fired and when the Contact header of
    //      the outgoing packet needs to be updated.
    //
    //      The Contact header is updated by using the following logic:
    //      1.  Find a Contact with the associated FQDN:
    //          a.  Find, in the routing table, the FQDN corresponding to the
    //              local address of the provided packet.
    //          b.  Find a Contact that has in its host part this FQDN.
    //          c.  If more than one contact is found, use the first one.
    //          d.  If no contact is found, check for an associated contact with
    //              an IP addresses (2.).
    //      2.  Find a contact with the associated IP address:
    //          a.  Get all Contacts that share the same address family as the 
    //              local address in their host portion.
    //          b.  If there is only one Contact in this group, this is the
    //              Contact to use.
    //          c.  If there are no Contacts sharing the same address family, do
    //              not modify anything and return without updating the current
    //              contact.
    //          d.  If there are more than one contact in this group:
    //              i.  Compare the host part of each contact against the local
    //                  IP address.
    //              ii. Use the Contact that shares the same IP address.
    //              iii.If no Contact shares the same IP address, the first
    //                  Contact of the group is the Contact associated with
    //                  this address.
    //
    //      Only the host part of the Contact header is updated. Parameters are
    //      left as is, if any.
    //
    //==EDOC====================================================================
    virtual mxt_result UpdateCurrentContact(INOUT CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  Validate
    //==
    //==========================================================================
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The local contact list is emtpy.
    //
    //      - resS_OK: Service is correctly configured.
    //
    //  Description:
    //      This method ensures that the user agent service is properly 
    //      configured.
    //      It checks that the vector of local contacts is not empty.
    //
    //==EDOC====================================================================
    virtual mxt_result Validate() = 0;

    //==SDOC====================================================================
    //==
    //==  ResetValidationSucceeded
    //==
    //==========================================================================
    //
    //  Description:
    //      Reset the boolean which assumes validation succeeded (for test 
    //      purposes).
    //
    //==EDOC==================================================================== 
    virtual void ResetValidationSucceeded() = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IPrivateSipUserAgentSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IPrivateSipUserAgentSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IPrivateSipUserAgentSvc(IN const IPrivateSipUserAgentSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IPrivateSipUserAgentSvc& operator=(IN const IPrivateSipUserAgentSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IPRIVATESIPUSERAGENTSVC_H

