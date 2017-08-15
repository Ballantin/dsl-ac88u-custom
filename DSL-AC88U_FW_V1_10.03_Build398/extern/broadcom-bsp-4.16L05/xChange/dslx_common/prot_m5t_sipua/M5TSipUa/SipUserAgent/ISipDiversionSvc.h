//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPDIVERSIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPDIVERSIONSVC_H
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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_DIVERSION_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_DIVERSION_SVC_SUPPORT to be able to use \
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
MX_DECLARE_ECOM_IID(ISipDiversionSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipDiversionSvc
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//   The diversion service when attached to a UAC is used to copy Diversion
//   headers present in a 3xx to the following INVITEs. Note that the Diversion
//   headers will be copied until a final success response is received. No
//   method on this service needs to be called for this behaviour, all that is
//   needed is to attach the service to a context.
//
//   When attached to a UAS the diversion service can be used to automatically
//   add a Diversion header to every outgoing 3xx. To do so simply call the
//   SetReason method with a not NULL reason. To stop adding the header call
//   SetReason with NULL.
//
//   This service is the implementation of draft-levy-sip-diversion-08 for the
//   UAC side.
//
//#The ISipDiversionSvc is an ECOM object#
//
//  The ISipDiversionSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_ISipDiversionSvc</B>
//
//  <B>Interface Id: IID_ISipDiversionSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipDiversionSvc.h
//
//==============================================================================
class ISipDiversionSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------

//-- Published Interface
//------------------------
public:

    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipDiversionSvc);
    //M5T_INTERNAL_USE_END

    //-- << Stereotype >>
    //--------------------------------------------

    //==========================================================================
    //==
    //==  SetReason
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the reason associated with the diversion.
    //
    //  Parameters:
    //    pszReason:
    //      The reason to set in the Diversion header. If NULL the service 
    //      stops adding Diversion header to outgoing 3xx.
    //
    //  Description:
    //      Sets the reason associated with the diversion. This sets the value
    //      of the reason parameter found in a Diversion header. After Calling
    //      this method a Diversion header is added to every outgoing 3xx.
    //
    //==========================================================================
    virtual void SetReason(IN const char* pszReason) = 0;


//-- Hidden Methods
//-------------------
protected:
    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipDiversionSvc() {};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipDiversionSvc() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipDiversionSvc(IN const ISipDiversionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipDiversionSvc& operator=(IN const ISipDiversionSvc& rSrc);
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPDIVERSIONSVC_H

