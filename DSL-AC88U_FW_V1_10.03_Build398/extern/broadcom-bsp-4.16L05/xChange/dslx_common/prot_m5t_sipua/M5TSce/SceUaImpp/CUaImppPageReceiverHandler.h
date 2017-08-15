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
#ifndef MXG_CUAIMPPPAGERECEIVERHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPPAGERECEIVERHANDLER_H

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


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaImppPageReceiverHandler
//========================================
//
// Description:
//  This handler manages the reception of out-of-dialog MESSAGE.
//
//  It forwards the MESSAGE to the configured component in the user
//  configuration object.
//
// Location:
//   UaImpp/CUaImppPageReceiverHandler.h
//
//==============================================================================
class CUaImppPageReceiverHandler : private CEComDelegatingUnknown,
                                   private ISceRequestHandler

{
//-- Friend Declarations.
    friend class CSceUaImppInitializer;

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
    CUaImppPageReceiverHandler(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CUaImppPageReceiverHandler();

    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaImppPageReceiverHandler();
    static void FinalizeCUaImppPageReceiverHandler();

    //-- << Startup mechanism >>

    static mxt_result Startup();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaImppPageReceiverHandler(IN const CUaImppPageReceiverHandler& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaImppPageReceiverHandler& operator=(IN const CUaImppPageReceiverHandler& rSrc);


//-- Hidden Data Members.
protected:
private:


};


MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CUAIMPPPAGERECEIVERHANDLER_H

//M5T_INTERNAL_USE_END
