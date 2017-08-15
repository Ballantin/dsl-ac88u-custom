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
#ifndef MXG_IPRIVATESCEBASECOMPONENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IPRIVATESCEBASECOMPONENT_H

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

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class ISipClientEventControl;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IPrivateSceBaseComponent);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IPrivateSceBaseComponent
//==============================================================================
//
// Description:
//   This is a private interface for the internal UA mechanism. It MUST NOT be
//   used externally. Please use ISceBasicExtensionMgr instead.
//
//   This interface defines the different methods needed by internal objects
//   and/or API.
//
//#The IPrivateSceBaseComponent is an ECOM object#
//
//  The IPrivateSceBaseComponent is an ECOM object that is
//  accessed through the following ECOM identifier:
//
//  <B>Interface Id: IID_IPrivateSceBaseComponent</B>
//
// Location:
//   SceCoreComponents/IPrivateSceBaseComponent.h
//
//==============================================================================
class IPrivateSceBaseComponent : public IEComUnknown
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    MX_DECLARE_ECOM_GETIID(IPrivateSceBaseComponent);

    //==========================================================================
    //==
    //==  ReIssueRequestHelper
    //==
    //==========================================================================
    //
    //  Summary:
    //      Method called when a request needs to be re-issued.
    //
    //  Parameters:
    //    pClientEventCtrl:
    //      The client event control that needs to be re-issued.
    //
    //  Description:
    //      Method called when a request needs to be re-issued. Reasons to
    //      re-issue a request includes redirection or authentication. Other
    //      reasons may be valid to re-issue a request.
    //
    //      Note that the restrictions that apply to
    //      ISipClientEventControl::ReIssueRequest also apply to this method.
    //      Namely, this method cannot be used if the method type of the
    //      request, the extra headers it attached to the original request or
    //      the content-info attached to the original request need to be
    //      changed.
    //
    //      Also note that in order to use this method, objects need to be given
    //      the ISipClientEventControl which is under the control of the
    //      component. In other words, the component needs to determine that a
    //      client event control may need to be re-issued and give it to another
    //      object which will call back this method if appropriate. If it is not
    //      appropriate to re-issue the request, the other object must call
    //      CallNextEvent() directly on the client event control.
    //
    //      ReIssueRequestHelper is needed because the component may decide that
    //      ultimately, there is no longer a point in having that transaction
    //      succeed (e.g.: The INVITE was canceled by the user). Hence, the
    //      external object must not call ReIssueRequest on its own on the
    //      client event control. There is no need for the same thing with
    //      CallNextEvent() because this will trigger the next failure event
    //      which will be properly handled by the component itself.
    //
    //  See Also:
    //      CSceBaseComponent
    //
    //==========================================================================
    virtual void ReIssueRequestHelper(IN ISipClientEventControl* pClientEventCtrl) = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IPrivateSceBaseComponent() {}

    // Summary:
    //  Destructor.
    virtual ~IPrivateSceBaseComponent() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IPrivateSceBaseComponent(IN const IPrivateSceBaseComponent& rSrc);

    // Summary:
    //  Assignment Operator.
    IPrivateSceBaseComponent& operator=(IN const IPrivateSceBaseComponent& rSrc);


//-- Hidden Data Members
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_IPRIVATESCEBASECOMPONENT_H

