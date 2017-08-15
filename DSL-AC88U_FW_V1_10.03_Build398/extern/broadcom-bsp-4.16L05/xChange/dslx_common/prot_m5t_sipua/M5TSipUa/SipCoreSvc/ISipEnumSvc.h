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
#ifndef MXG_ISIPENUMSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPENUMSVC_H
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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT to be able to use \
this class.
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipEnumSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipEnumSvc
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//   This service is responsible to replace the tel-URI of outgoing requests
//   with the sip-URI resulting of an ENUM request as per RFC 3761.
//
//#The ISipEnumSvc is an ECOM object#
//
//  The ISipEnumSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_ISipEnumSvc</B>
//
//  <B>Interface Id: IID_ISipEnumSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipEnumSvc.h
//
//==EDOC========================================================================
class ISipEnumSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------

//-- New types within class namespace
//-----------------------------------
public:
    // Description:
    //  Contains the behaviour option of the ENUM service. The values are
    //  bitmaskable except for eNONE. 
    //
    // See also: 
    //  SetServiceBehaviour.
    enum EEnumServiceBehaviour
    {
        // Description:
        //  None of the following option.
        eNONE             = 0,
        // Description:
        //  Removes the Route header after having replaced the request-URI.
        eREMOVE_ROUTE     = 1,
        // Description:
        //  Updates the To header with the resulting ENUM sip-URI.
        eUPDATE_TO_HEADER = 2,
    };

//-- Published Interface
//------------------------
public:

    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipEnumSvc);
    //M5T_INTERNAL_USE_END

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  SetServiceBehaviour
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the service behaviour.
    //
    //  Parameters:
    //    nEnumSvcBehaviourBitset:
    //      The ENUM service behaviour. This value can be a concatenation of
    //      various values created by using the bitwise "OR" operator on the
    //      EEnumServiceBehaviour enumeration.
    //
    //  Description:
    //      Configures how the ENUM service behaves. The following options may be
    //      selected:
    //          - eREMOVE_ROUTE: Removes the Route header after having replaced
    //            the request-URI.
    //          - eUPDATE_TO_HEADER: Updates the To header with the resulting
    //            ENUM sip-URI.
    //
    //      By default, the above options are unselected.
    //
    //==EDOC====================================================================
    virtual void SetServiceBehaviour(IN int nEnumSvcBehaviourBitset) = 0;

//-- Hidden Methods
//-------------------
protected:
    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipEnumSvc() {};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipEnumSvc() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipEnumSvc(IN const ISipEnumSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipEnumSvc& operator=(IN const ISipEnumSvc& rSrc);
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPENUMSVC_H

