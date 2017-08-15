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
#ifndef MXG_ISCEREFERHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEREFERHANDLER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCECORECOMPONENTSCFG_H
#include "Config/SceCoreComponentsCfg.h"
#endif


#if !defined(MXD_SCE_ENABLE_REFERRER_COMPONENT_SUPPORT)
#error You must define MXD_SCE_ENABLE_REFERRER_COMPONENT_SUPPORT to be able to \
use this class.
#endif


#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class ISceNewReferrerMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceReferHandler);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceReferHandler
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface provides configuration methods for the REFER handler object.
//
// Description:
//  This interface provides configuration methods for the REFER handler object.
//
//  This interface is accessible through ISceCoreConfig::GetHandlerInterface.
//
// Location:
//   SceCoreComponents/ISceReferHandler.h
//
// See Also:
//   ISceReferrer, ISceNewReferrerMgr
//
//==============================================================================
class ISceReferHandler : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceReferHandler);
    //M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures a manager that is informed when a new ISceReferrer
    //      component is created.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the manager. 
    //
    //      NULL to reset the manager.
    //
    //  Description:
    //      Sets the new referrer manager on which the refer handler reports
    //      its events.
    //
    //      When the manager of the ISceNotifierHandler is NULL, incoming
    //      out-of-dialog REFERs are not handled by this handler.
    //
    //==========================================================================
    virtual void SetManager(IN ISceNewReferrerMgr* pMgr) = 0;


//-- Hidden Methods.
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceReferHandler() {}

    // Summary:
    //  Destructor.
    virtual ~ISceReferHandler() {}



private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceReferHandler(IN const ISceReferHandler& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceReferHandler& operator=(IN const ISceReferHandler& rSrc);


//-- Hidden Data Members.
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEREFERHANDLER_H

