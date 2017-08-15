//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_IUAIMPPPAGECONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPPAGECONFIG_H

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
MX_DECLARE_ECOM_IID(IUaImppPageConfig);

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppPageConfig
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//   This interface defines the user configuration options for instant messaging
//   page receiver.
//
// Description:
//   This interface defines the user configuration options for instant messaging
//   page receiver.
//
// Location:
//   UaImppp/IUaImppPageConfig.h
//
// See Also:
//   IUaImppPageReceiver, IUaImppPageReceiverMgr
//
//==============================================================================
class IUaImppPageConfig : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:

//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaImppPageConfig);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetComponent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the component that will be handling received instant
    //      message.
    //
    //  Parameters:
    //    pComponent:
    //      Pointer to the PageReceiver component.
    //
    //  Description:
    //      Configures the component that will be handling received instant
    //      messages.
    //
    //      This method is internal to M5T and the UA IMPP and not meant for use
    //      by an external application.
    //
    //  See Also:
    //      GetComponent
    //
    //==========================================================================
    virtual void SetComponent(IN IEComUnknown* pComponent) = 0;

    //==========================================================================
    //==
    //==  GetComponent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the component that will handle received instant messages.
    //
    //  Parameters:
    //    ppComponent:
    //      Double pointer to the component. If it returns non-NULL, a reference
    //      will have been counted on the object.
    //
    //  Description:
    //      Gets the component that will handle received instant messages.
    //
    //      This method is internal to M5T and the UA IMPP and not meant for use
    //      by an external application.
    //
    //  See Also:
    //      SetComponent
    //
    //==========================================================================
    virtual void GetComponent(OUT IEComUnknown** ppComponent) = 0;
    

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaImppPageConfig() {}

    // Summary:
    //  Destructor.
    virtual ~IUaImppPageConfig() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaImppPageConfig(IN const IUaImppPageConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaImppPageConfig& operator=(IN const IUaImppPageConfig& rSrc);


//-- Hidden Data Members.
protected:
private:

//M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_IUAIMPPPAGECONFIG_H

