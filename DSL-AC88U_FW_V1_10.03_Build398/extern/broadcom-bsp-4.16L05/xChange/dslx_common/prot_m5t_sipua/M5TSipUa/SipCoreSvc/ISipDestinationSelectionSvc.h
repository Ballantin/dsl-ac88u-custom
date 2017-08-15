//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPDESTINATIONSELECTIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPDESTINATIONSELECTIONSVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_DESTINATION_SELECTION_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_DESTINATION_SELECTION_SVC_SUPPORT to be able to use \
this class.
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipDestinationSelectionSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipDestinationSelectionSvc
//==============================================================================
//<GROUP SIPCORESVC_CLASSES>
// <FLAG New in 4.1.6>
//
// Summary:
//   This service is responsible to set the next-hop URI in the packet.
//
// Description:
//   This service sets the next-hop URI, by either using the Request-URI or the
//   URI in the Route header.
//
//   NOTE: it is mandatory to attach this service on a context and to attach it
//   prior to attaching the server location service (if applicable).
//
//#The ISipDestinationSelectionSvc is an ECOM object#
//
//  The ISipDestinationSelectionSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipDestinationSelectionSvc</B>
//
//  <B>Interface Id: IID_ISipDestinationSelectionSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipDestinationSelectionSvc.h
//
//==============================================================================
class ISipDestinationSelectionSvc : public IEComUnknown
{
//-- Published Interface
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipDestinationSelectionSvc);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  ForceDestination
    //==
    //==========================================================================
    //
    //  Summary:
    //      Forces a destination to be used.
    //
    //  Parameters:
    //    rDestinationUri:
    //      The destination.
    //
    //  Description:
    //      Forces the context to use the specified destination when sending
    //      requests. The forced URI bypasses the next hop URI specified in the
    //      top Route or in the Request-Line (if there is no Route). The SIP
    //      protocol specifies that such Route or Request-Line must be used as
    //      the next target for requests.
    //
    //      The forced destination will be used as the next hop uri. This means
    //      that TLS sockets will use this URI to compare against the common
    //      name (CN) of the TLS certificate when validating the handshake.
    //
    //      WARNING:
    //      This method should be used with care as it breaks the SIP standards.
    //
    //==========================================================================
    virtual void ForceDestination(IN const CSipUri& rForcedDestinationUri) = 0;

//M5T_INTERNAL_USE_BEGIN
    //==============================================================================
    //==
    //==  SetDestinationId
    //==
    //==============================================================================
    //
    //  Summary:
    //      Sets the destination ID for the context associated with the service.
    //
    //  Parameters:
    //      uDestinationId:
    //       The destination ID.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method. This method could be
    //      deprecated (and removed) in a near future.
    //
    //      Sets the destination ID for the context associated with the service. 
    //      The destination ID will be set in each packet that will be sent. If a
    //      persistent connection has been established with the same ID, this will
    //      force the packet to use this socket. Packet that don't have the same ID
    //      cannot use the persistent connection. By default, the destination ID of 
    //      all context and persistent connection is 0.
    //
    //==============================================================================
    virtual void SetDestinationId(IN unsigned int uDestinationId) = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipDestinationSelectionSvc(){}

    // Summary:
    //  Destructor.
    virtual ~ISipDestinationSelectionSvc(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipDestinationSelectionSvc(IN const ISipDestinationSelectionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipDestinationSelectionSvc& operator=(IN const ISipDestinationSelectionSvc& rSrc);

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPDESTINATIONSELECTIONSVC_H


