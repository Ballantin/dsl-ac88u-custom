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
#ifndef MXG_ISCENOTIFIERHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCENOTIFIERHANDLER_H
//M5T_INTERNAL_USE_END

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
class ISceNewNotifierMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceNotifierHandler);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceNotifierHandler
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface provides configuration methods for the NotifierHandler
//   object.
//
// Description:
//  This interface provides configuration methods for the NotifierHandler object.
//
//  This interface is accessible through ISceCoreConfig::GetHandlerInterface.
//
// Location:
//   SceCoreComponents/ISceNotifierHandler.h
//
// See Also:
//   ISceNotifier, ISceNewNotifierMgr
//
//==============================================================================
class ISceNotifierHandler : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceNotifierHandler);
    //M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the new notifier manager.
    //
    //  Parameters:
    //    pNewNotifierMgr:
    //      The ISceNewNotifierMgr instance to use.
    //
    //      NULL to reset the manager.
    //
    //  Description:
    //      Sets the new notifier manager on which the notifier handler reports
    //      its events.
    //
    //      This method MUST be called with a non-NULL pointer before
    //      registering the notifier handler.
    //
    //      When the manager of the ISceNotifierHandler is NULL, the incoming
    //      SUBSCRIBE is not handled by this handler.
    //
    //  See Also:
    //      ISceNewNotifierMgr
    //
    //==========================================================================
    virtual void SetManager(IN ISceNewNotifierMgr* pNewNotifierMgr) = 0;


//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceNotifierHandler() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceNotifierHandler() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceNotifierHandler(IN const ISceNotifierHandler& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceNotifierHandler& operator=(IN const ISceNotifierHandler& rSrc);


//-- Hidden Data Members.
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCENOTIFIERHANDLER_H
