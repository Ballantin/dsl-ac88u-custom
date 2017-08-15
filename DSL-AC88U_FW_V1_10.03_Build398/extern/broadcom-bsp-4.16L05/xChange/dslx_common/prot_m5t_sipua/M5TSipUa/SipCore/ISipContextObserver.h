//==============================================================================
//==============================================================================
//
//              Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPCONTEXTOBSERVER_H
#define MXG_ISIPCONTEXTOBSERVER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipPacket;
class ISipContext;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipContextObserver
//==============================================================================
//
// Description:
//
//  The SIP Observer is used to report to its managers different events
//  related to the context.
//
//  The observer supports the registration of many managers, which are stored
//  in a vector. If a manager is terminated before the context, the manager
//  must unregister itself in order to avoid a call to undefined pointer.
//
// Location:
//   SipCore/ISipContextObserver.h
//
//==============================================================================
class ISipContextObserver
{
//-- Friend Declarations


//-- Published Interface
public:

    //==========================================================================
    //==
    //==  EvContextCleared
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a service has been cleared.
    //
    //  Parameters:
    //    pContext:
    //      Pointer to the context.
    //
    //  Description:
    //      This is called to report to the registered managers that the context
    //      has been cleared and it must not be used anymore.
    //
    //
    //==========================================================================
    virtual void EvContextCleared(ISipContext* pContext) = 0;


//-- Hidden Methods
protected:

    // Summary:
    //  Default Constructor.
    ISipContextObserver() {}

    // Summary:
    //  Destructor.
    virtual ~ISipContextObserver() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>


    // Summary:
    //  Copy Constructor.
    ISipContextObserver(IN const ISipContextObserver& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipContextObserver& operator=(IN const ISipContextObserver& rSrc);


//-- Hidden Data Members
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCONTEXTOBSERVER_H

