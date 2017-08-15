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
#ifndef MXG_ISCESHUTDOWNMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCESHUTDOWNMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceShutdownMgr
//========================================
//<GROUP TID_PACKAGE_SCECORE_CLASSES>
//
// Summary:
//  Defines the interface of a system shutdown manager.
//
// Description:
//  The ISceShutdownMgr defines the interface of a system shutdown
//  manager. This interface is used to report the EvOnShutdownCompleted event
//  on the correct manager.
//
//  Any class that wants to be reported the asynchroneous shutdown
//  completion event MUST implement this interface and provide it to the
//  ISceCoreConfig::ShutdownA() method.
//
//  If asynchronous shutdown is not necessary or the class initiating it does
//  not want to be told about the process completion, it can directly call
//  ISceCoreConfig::ShutdownA() with no parameter.
//
// Location:
//   SceCore/ISceShutdownMgr.h
//
//==============================================================================
class ISceShutdownMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==========================================================================
    //==
    //==  EvOnShutdownCompleted
    //==
    //==========================================================================
    //
    //  Summary:
    //      Event reported after the shutdown process is completed.
    //
    //  Parameters:
    //    opqShutdown:
    //      Opaque value passed to the ISceCoreConfig::Shutdown() method.
    //
    //  Description:
    //      This event is reported to its manager when the shutdown process is
    //      completed. The opaque value passed to ISceCoreConfig::Shutdown()
    //      is returned to the manager through this event.
    //
    //==========================================================================
    virtual void EvOnShutdownCompleted(IN mxt_opaque opqShutdown) = 0;


//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceShutdownMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceShutdownMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceShutdownMgr(IN const ISceShutdownMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceShutdownMgr& operator=(IN const ISceShutdownMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCESHUTDOWNMGR_H

