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
#ifndef MXG_CUASSPCALLHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUASSPCALLHANDLER_H

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

#ifndef MXG_IUASSPCALLHANLDER_H
#include "SceUaSsp/IUaSspCallHandler.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaSspCallHandler
//========================================
//
// Description:
//  This class is the request handler that is responsible to handle incoming
//  INVITE and OPTIONS requests that do not match any existing dialog. It
//  handles all INVITE and OPTIONS requests.
//
// Location:
//   UaSsp/CUaSspCallHandler.h
//
// See Also:
//   IUaSspNewCallMgr, IUaSspCall
//
//==============================================================================
class CUaSspCallHandler : private CEComDelegatingUnknown,
                          private ISceRequestHandler,
                          private IUaSspCallHandler
{
//-- Friend Declarations.
    friend class CSceUaSspInitializer;


//-- Published Interface.
public:


//-- Hidden Methods.
protected:
private:

    //-- << ISceRequesthandler >>
    virtual mxt_result HandleRequest(IN const CSipPacket& rRequest,
                                     IN IEComUnknown* pUserConfig);

    //-- << IUaSspCallHandler >>
    virtual void SetManager(IN IUaSspNewCallMgr* pNewCallMgr);


    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaSspCallHandler(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CUaSspCallHandler();


    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaSspCallHandler();
    static void FinalizeCUaSspCallHandler();

    //-- << ECOM mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << Startup mechanism >>

    static mxt_result Startup();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaSspCallHandler(IN const CUaSspCallHandler& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaSspCallHandler& operator=(IN const CUaSspCallHandler& rSrc);


//-- Hidden Data Members.
protected:
private:
    // Manager to report the events to.
    IUaSspNewCallMgr*   m_pNewCallMgr;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CUASSPCALLHANDLER_H

