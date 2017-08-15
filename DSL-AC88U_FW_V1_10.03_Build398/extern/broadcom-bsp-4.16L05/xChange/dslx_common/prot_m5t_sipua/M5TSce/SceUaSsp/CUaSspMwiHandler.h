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
#ifndef MXG_CUASSPMWIHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUASSPMWIHANDLER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEREQUESTHANDLER_H
#include "SceCore/ISceRequestHandler.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSipPacket;
class ISceUserConfig;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaSspMwiHandler
//========================================
//
// Description:
//  This handler manages the reception of out-of-dialog NOTIFY for the
//  "message-summary" event type.
//
//  It forwards the unsollicited NOTIFY to the configured component in the user
//  configuration object.
//
// Location:
//   UaSsp/CUaSspMwiHandler.h
//
//==============================================================================
class CUaSspMwiHandler : private CEComDelegatingUnknown,
                         private ISceRequestHandler

{
//-- Friend Declarations.
    friend class CSceUaSspInitializer;

//-- Published Interface.
public:

//-- Hidden Methods.
protected:
private:

    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << ISceRequesthandler >>
    virtual mxt_result HandleRequest(IN const CSipPacket& rRequest,
                                     IN IEComUnknown* pUserConfig);


    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaSspMwiHandler(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CUaSspMwiHandler();

    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaSspMwiHandler();
    static void FinalizeCUaSspMwiHandler();

    //-- << Startup mechanism >>

    static mxt_result Startup();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaSspMwiHandler(IN const CUaSspMwiHandler& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaSspMwiHandler& operator=(IN const CUaSspMwiHandler& rSrc);


//-- Hidden Data Members.
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CUASSPMWIHANDLER_H

