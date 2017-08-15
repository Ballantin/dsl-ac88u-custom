//==============================================================================
//==============================================================================
//
//              Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPTRANSPORTCONFIGURATIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPTRANSPORTCONFIGURATIONSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(ISipTransportConfigurationSvc);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipTransportConfigurationSvc
//========================================
//<GROUP TID_PACKAGE_SCESTACKSERVICES_CLASSES>
//
// Description:
//  This interface defines the API of the transport configuration service. It
//  can be attached anywhere and serves to configure the transport layer and/or
//  the socket layer with additional information to send the SIP packets. This
//  service has no manager and as such does not report any event.
//
// Location:
//   SceStackServices/ISipTransportConfigurationSvc.h
//
// See Also:
//
//==============================================================================
class ISipTransportConfigurationSvc : public IEComUnknown
{
//-- Friend Declarations.

//-- Published Interface.
public:
    MX_DECLARE_ECOM_GETIID(ISipTransportConfigurationSvc);

    //==========================================================================
    //==
    //==  SetDscp
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the DSCP (Differentiated Service Code Point or QoS Packet
    //      marking) to use for the following packets sending.
    //
    //  Parameters:
    //      uDscp:
    //       The DSCP value to set.
    //
    //  Description:
    //      Sets the DSCP (Differentiated Service Code Point or QoS Packet
    //      marking) to use for the following packets sending. The maximum
    //      value of the DSCP is 63 since it uses 6 bits in the IP packet.
    //
    //      This method can be called multiple times through the life of a
    //      single context if the DSCP value can change depending on the
    //      packets to send on that context. The last value set is always the
    //      one used until a new value is set or the context is released.
    //
    //==========================================================================
    virtual void SetDscp(IN uint32_t uDscp) = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipTransportConfigurationSvc() {}

    // Summary:
    //  Destructor.
    virtual ~ISipTransportConfigurationSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipTransportConfigurationSvc(IN const ISipTransportConfigurationSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipTransportConfigurationSvc& operator=(IN const ISipTransportConfigurationSvc& rSrc);

//-- Hidden Data Members.
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_ISIPTRANSPORTCONFIGURATIONSVC_H
