//==============================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPFORKEDDIALOGGROUPER_H
#define MXG_ISIPFORKEDDIALOGGROUPER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members


//-- Parameters used by Value


//-- Interface Realized and/or Parent


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
//== Class: ISipForkedDialogGrouper
//========================================
// <GROUP SIPCORE_CLASSES>
//
// Description:
//   This interface regroups multiple dialogs created from a single request that
//   has forked.
//
//   The services issuing requests that can create a dialog and that can fork is
//   automatically enabling an object that implements this interface on the
//   request's ISipRequestContext.
//
//   When forking happens, the application gets this object through the
//   ISipForkedDialogGrouperMgr::EvNewDialogNeeded method. The manager should
//   then create and configure a new ISipContext and call ContextCreated on this
//   object with that ISipContext and the packet passed in the EvNewDialogNeeded
//   method.
//
//   The object is reference counted.
//
//   Note that this object is not synchronized and must be accessed only in the
//   context of the core thread.
//
// Location:
//   SipCore/ISipForkedDialogGrouper.h
//
// See Also:
//   ISipForkedDialogGrouperMgr
//
//==============================================================================
class ISipForkedDialogGrouper
{
//-- Friend Declarations


//-- Published Interface
public:
    //-- << Public methods >>

    //==========================================================================
    //==
    //==  ContextCreated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Dispatches the packet resulting from forking to the newly created
    //      context.
    //
    //  Parameters:
    //    rContext:
    //      The newly created context.
    //
    //    rPacket:
    //      The received packet. MUST be the same instance as the one given in
    //      EvNewDialogNeeded (i.e. not a copy of the packet).
    //
    //    opqTransaction:
    //      Application data to associate with the created transaction. This is
    //      opaque to the service.
    //
    //  Returns:
    //      - resS_OK: The packet is properly processed by the context.
    //
    //      - resFE_FAIL: The packet could not be processed by the context. This
    //      could happen if the context in parameter is not a newly created
    //      context or if the service that originated the forked request is
    //      not attached to the context.
    //
    //      - resFE_DUPLICATE: Unable to create the server transaction, as it
    //      already exists. The context should be released immediately.
    //
    //  Description:
    //      This method should be called by the ISipForkedDialogGrouperMgr when
    //      EvNewDialogNeeded is called. The manager should create a new
    //      ISipContext and attach the appropriate services to it. It should
    //      then call this method with that ISipContext and the CSipPacket
    //      received in parameter of EvNewDialogNeeded on the
    //      ISipForkedDialogGrouper instance received in parameter of
    //      EvNewDialogNeeded.
    //
    //      Note that this method is not synchronized and must be accessed only
    //      in the context of the core thread.
    //
    //==========================================================================
    virtual mxt_result ContextCreated(IN ISipContext& rContext,
                                      IN const CSipPacket& rPacket,
                                      IN mxt_opaque opqTransaction) = 0;

    //==========================================================================
    //==
    //==  AddRef
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a reference on this object.
    //
    //  Returns:
    //      The current number of references.
    //
    //  Description:
    //      Adds a reference on this object. The added reference must be
    //      released in order to let this object destroy itself.
    //
    //      Note that this method is not synchronized and must be accessed only
    //      in the context of the core thread.
    //
    //==========================================================================
    virtual unsigned int AddRef() = 0;

    //==========================================================================
    //==
    //==  ReleaseRef
    //==
    //==========================================================================
    //
    //  Summary:
    //      Releases a reference on this object.
    //
    //  Returns:
    //      The current number of references.
    //
    //  Description:
    //      Releases a reference on this object. When the reference count
    //      reaches 0, the object deletes itself.
    //
    //      Note that this method is not synchronized and must be accessed only
    //      in the context of the core thread.
    //
    //==========================================================================
    virtual unsigned int ReleaseRef() = 0;


//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipForkedDialogGrouper() {};

    // Summary:
    //  Destructor.
    virtual ~ISipForkedDialogGrouper() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipForkedDialogGrouper(IN const ISipForkedDialogGrouper& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipForkedDialogGrouper& operator=(IN const ISipForkedDialogGrouper& rSrc);


//-- Hidden Data Members
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPFORKEDDIALOGGROUPER_H

