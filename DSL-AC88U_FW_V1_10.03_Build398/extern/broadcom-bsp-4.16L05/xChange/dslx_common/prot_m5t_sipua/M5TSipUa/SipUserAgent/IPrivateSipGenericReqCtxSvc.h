//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_IPRIVATESIPGENERICREQCTXSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IPRIVATESIPGENERICREQCTXSVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_ISIPREQCTXCORESVC_H
#include "SipCore/ISipReqCtxCoreSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipGenericSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IPrivateSipGenericReqCtxSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IPrivateSipGenericReqCtxSvc
//==============================================================================
//
// Description:
//   This interface is for the stack's internal use only. It renders the
//   ISipReqCtxCoreSvc interface available through the ECom mechanism. It is
//   used by the CSipGenericSvc to set required information on the request
//   context service it creates.
//
//   This interface is an ECOM interface and as such ECOM reference rules apply.
//
// Location:
//   SipCore/IPrivateSipGenericReqCtxSvc.h
//
// See Also:
//   ISipReqCtxCoreSvc
//
//==============================================================================
class IPrivateSipGenericReqCtxSvc : public IEComUnknown,
                                    public ISipReqCtxCoreSvc
{
//-- Friend Declarations
//-----------------------

//-- New types within class namespace
//------------------------------------
public:
protected:
private:

//-- Published Interface
//-----------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IPrivateSipGenericReqCtxSvc);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetParentSvc
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the request context service's parent service.
    //
    //  Parameters:
    //    rParentSvc:
    //      Reference to the parent CSipSipGenericSvc instance.
    //
    //  Description:
    //      Sets the request context service's required attributes. This is a
    //      CSipGenericSvc instance.
    //
    //      Note that this method MUST be called for the request context service
    //      to be able to perform its task.
    //
    //==========================================================================
    virtual void SetParentSvc(IN CSipGenericSvc& rParentSvc) = 0;


//-- Hidden Methods
//------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IPrivateSipGenericReqCtxSvc() {};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IPrivateSipGenericReqCtxSvc() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IPrivateSipGenericReqCtxSvc(IN const IPrivateSipGenericReqCtxSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IPrivateSipGenericReqCtxSvc& operator=(IN const IPrivateSipGenericReqCtxSvc& rSrc);

//-- Hidden Data Members
//-----------------------
protected:
private:
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IPRIVATESIPGENERICREQCTXSVC_H

