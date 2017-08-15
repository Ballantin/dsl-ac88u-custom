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
#ifndef MXG_CSCEREFERHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEREFERHANDLER_H

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


#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEREQUESTHANDLER_H
#include "SceCore/ISceRequestHandler.h"
#endif

#ifndef MXG_ISCEREFERHANDLER_H
#include "SceCoreComponents/ISceReferHandler.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class ISceNewReferrerMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceReferHandler
//========================================
//
// Description:
//  This class is the request handler that is responsible to handle incoming
//  REFER requests that do not match any existing dialog. It handles all REFER
//  requests.
//
// Location:
//   SceCoreComponents/CSceReferHandler.h
//
// See Also:
//   ISceNewReferrerMgr, ISceReferrer
//
//==============================================================================
class CSceReferHandler : private CEComDelegatingUnknown,
                         private ISceRequestHandler,
                         private ISceReferHandler
{
//-- Friend Declarations.
    friend class CSceCoreComponentsInitializer;


//-- Published Interface.
public:


//-- Hidden Methods.
protected:
private:

    //-- << ISceRequesthandler >>
    virtual mxt_result HandleRequest(IN const CSipPacket& rRequest,
                                     IN IEComUnknown* pUserConfig);

    //-- << ISceReferHandler >>
    virtual void SetManager(IN ISceNewReferrerMgr* pMgr);


    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceReferHandler(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceReferHandler();


    //-- << Initialization mechanism >>
    static mxt_result InitializeCSceReferHandler();
    static void FinalizeCSceReferHandler();

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
    CSceReferHandler(IN const CSceReferHandler& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceReferHandler& operator=(IN const CSceReferHandler& rSrc);


//-- Hidden Data Members.
protected:
private:
    // Manager to report the events to.
    ISceNewReferrerMgr*   m_pNewReferrerMgr;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCEREFERHANDLER_H

