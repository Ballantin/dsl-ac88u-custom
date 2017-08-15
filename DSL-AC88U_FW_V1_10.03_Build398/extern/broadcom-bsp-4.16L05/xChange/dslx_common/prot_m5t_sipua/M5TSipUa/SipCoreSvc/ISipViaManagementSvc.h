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
#ifndef MXG_ISIPVIAMANAGEMENTSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPVIAMANAGEMENTSVC_H
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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_VIA_MANAGEMENT_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_VIA_MANAGEMENT_SVC_SUPPORT to be able to use \
this class.
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipViaManagementSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipViaManagementSvc
//==============================================================================
//<GROUP SIPCORESVC_CLASSES>
// <FLAG New in 4.1.6>
//
// Summary:
//   This service is responsible to set the top-most Via header.
//
// Description:
//   This service sets the top-most Via header in the packet, if it is not
//   already present.
//
//   NOTE: This service is mandatory. It must be the last connection service 
//   attached to a context. There are exceptions to that rule. The
//   ISipCoreOutputControllingSvc can be attached after this service if the
//   application wants to do modifications inside the SIP packet after all
//   normal SIP stack services updated it. Also, the ISipSymmetricUdpSvc must be
//   attached after this service to send SIP packets using the UDP listening
//   socket.
//
//#The ISipViaManagementSvc is an ECOM object#
//
//  The ISipViaManagementSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipViaManagementSvc</B>
//
//  <B>Interface Id: IID_ISipViaManagementSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipViaManagementSvc.h
//
//==============================================================================
class ISipViaManagementSvc : public IEComUnknown
{
public:

    //  Summary:
    //    Possible via management modes.
    //
    //  Description:
    //   These are the possible via management modes that can be set using
    //   ISipViaManagementSvc::SetViaManagementSvcMode.
    enum EViaManagementMode
    {
        //  Description:
        //   Stateful mode: Via management service will fail-over when the stack
        //   is not able to send requests. This is the default mode.
        eSTATEFUL,

        //  Description:
        //   Stateles mode: Disable the fail-over mechanism.
        eSTATELESS
    };
//-- Published Interface
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipViaManagementSvc);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetViaManagementSvcMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the Via management service mode.
    //
    //  Parameters:
    //      eMode:
    //       The udpated mode.
    //
    //  Description:
    //      Sets the Via management service mode. When the service is stateless,
    //      the branch parameter remains unchanged when this service is used by
    //      a proxy implementation.
    //
    //==========================================================================
    virtual void SetViaManagementSvcMode(EViaManagementMode eMode) = 0;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipViaManagementSvc(){}

    // Summary:
    //  Destructor.
    virtual ~ISipViaManagementSvc(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipViaManagementSvc(IN const ISipViaManagementSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipViaManagementSvc& operator=(IN const ISipViaManagementSvc& rSrc);

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPVIAMANAGEMENTSVC_H


