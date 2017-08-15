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
#ifndef MXG_ISCEUSERCONFIGTERMINATION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IISCEUSERCONFIGTERMINATION_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
//-------------------------


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(ISceUserConfigTermination);


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceUserConfigTermination
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface is used in the termination process of the ISceUserConfig.
//
// Description:
//  This interface is used in the termination process of the ISceUserConfig.
//  This interface needs to be implemented by configurations aggregated to a
//  ISceUserConfig that can break a circular reference on the ISceUSerConfig.
//
// Location:
//   SceCoreComponents/ISceUserConfigTermination.h
//
//==============================================================================
class ISceUserConfigTermination : public IEComUnknown
{
//-- Friend Declarations.
//------------------------


//-- Published Interface.
//------------------------
public:
    MX_DECLARE_ECOM_GETIID(ISceUserConfigTermination);


    //==========================================================================
    //==
    //==  OnUserConfigTermination
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells the config that the ISceUserConfig is terminating.
    //
    //  Description:
    //      This method is called when the only references left on the
    //      ISceUserConfig are circular references. When this method is called
    //      and the configuration is responsible for a circular reference, it
    //      MUST make sure that the reference cycle will be broken.
    //
    //==========================================================================
    virtual void OnUserConfigTermination() = 0;

//-- Hidden Methods.
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceUserConfigTermination() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceUserConfigTermination() {}

private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceUserConfigTermination(IN const ISceUserConfigTermination& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceUserConfigTermination& operator=(IN const ISceUserConfigTermination& rSrc);


//-- Hidden Data Members.
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_ISCEUSERCONFIGTERMINATION_H

