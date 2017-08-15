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
#ifndef MXG_CSCENOTIFIERHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCENOTIFIERHANDLER_H

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

#ifndef MXG_ISCENOTIFIERHANLDER_H
#include "SceCoreComponents/ISceNotifierHandler.h"
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
//== Class: CSceNotifierHandler
//========================================
//
// Description:
//  This class is the request handler that is responsible to handle incoming
//  SUBSCRIBE requests that do not match any existing dialog. It handles all
//  SUBSCRIBE requests.
//
// Location:
//   SceCoreComponents/CSceNotifierHandler.h
//
// See Also:
//   ISceNewNotifierMgr, ISceNotifier
//
//==============================================================================
class CSceNotifierHandler : private CEComDelegatingUnknown,
                            private ISceRequestHandler,
                            private ISceNotifierHandler
{
//-- Friend Declarations.
    friend class CSceCoreComponentsInitializer;


//-- Published Interface.
public:

//-- Hidden Methods.
protected:
private:
    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //-- << ISceRequesthandler >>
    virtual mxt_result HandleRequest(IN const CSipPacket& rRequest,
                                     IN IEComUnknown* pUserConfig);

    //-- << ISceNotifierHandler >>
    virtual void SetManager(IN ISceNewNotifierMgr* pNewNotifierMgr);


    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceNotifierHandler(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceNotifierHandler();


    //-- << Initialization mechanism >>
    static mxt_result InitializeCSceNotifierHandler();
    static void FinalizeCSceNotifierHandler();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << Startup mechanism >>

    static mxt_result Startup();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceNotifierHandler(IN const CSceNotifierHandler& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceNotifierHandler& operator=(IN const CSceNotifierHandler& rSrc);


//-- Hidden Data Members.
protected:
private:
    // Manager to report the events to.
    ISceNewNotifierMgr*   m_pNewNotifierMgr;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCENOTIFIERHANDLER_H

