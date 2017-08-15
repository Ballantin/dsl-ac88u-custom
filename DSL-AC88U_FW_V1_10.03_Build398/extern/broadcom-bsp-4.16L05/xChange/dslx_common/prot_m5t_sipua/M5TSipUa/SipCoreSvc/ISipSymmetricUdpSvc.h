//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPSYMMETRICUDPSVC_H
#define MXG_ISIPSYMMETRICUDPSVC_H

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


#ifndef MXD_SIPSTACK_ENABLE_SIP_SYMMETRIC_UDP_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_SYMMETRIC_UDP_SVC_SUPPORT to be able to use this class.
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
class CSocketAddr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipSymmetricUdpSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipSymmetricUdpSvc
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//   This service is used to override the network-level local port used to 
//   send packets over UDP. When attached to a context, the service will use
//   the first UDP listening port of the network interface selected to send the
//   packet. 
//
//   This service is mostly used when Proxies and Registrars expect SIP
//   traffic to come from a certain port, the most common example being the same
//   as the listening port, e.g. 5060.
//
//   This service needs to be the last connection service attached on a SIP
//   context to work properly. If attached anywhere else, the changes made to
//   the SIP packets will be overridden by other services.
//
// Location:
//   SipCoreSvc/ISipSymmetricUdpSvc.h
//
// See Also:
//   SipCoreSvc/CSymmetricUdpSvc.h
//
//==EDOC========================================================================
class ISipSymmetricUdpSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------

//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipSymmetricUdpSvc);
    //M5T_INTERNAL_USE_END

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipSymmetricUdpSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipSymmetricUdpSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipSymmetricUdpSvc(IN const ISipSymmetricUdpSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipSymmetricUdpSvc& operator=(IN const ISipSymmetricUdpSvc& rSrc);
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSYMMETRICUDPSVC_H

