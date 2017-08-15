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
#ifndef MXG_CSCEREQUESTDISPATCHER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEREQUESTDISPATCHER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSipPacket;
class ISceRequestHandler;
class ISceUserConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceRequestDispatcher
//========================================
//
// Description:
//
//  The SCE Request Dispatcher manages a list of
//  ISceRequestHandler. Incoming requests that do not match a dialog are
//  provided to the request dispatcher with or without a user configuration
//  object associated with the request.
//
//  When receiving such a request, the request dispatcher will show the request
//  to the ISceRequestHandler in its list in the reverse order they were
//  registered (LIFO order) until one request handler informs the request
//  dispatcher that the request was handled.
//
// Location:
//   SceCore/CSceRequestDispatcher.h
//
//==============================================================================
class CSceRequestDispatcher
{
//-- Friend Declarations.


//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceRequestDispatcher();

    // Summary:
    //  Destructor.
    virtual ~CSceRequestDispatcher();

    // Summary:
    //  Registers a request handler.
    mxt_result RegisterRequestHandler(IN ISceRequestHandler* pHandler);

    // Summary:
    //  Gets an handler interface.
    template<class _Type> mxt_result GetHandlerInterface(OUT _Type** ppInterface)
    {
        if (ppInterface != NULL)
        {
            return GetHandlerInterface((*ppInterface)->GetIID(), OUT reinterpret_cast<void**>(ppInterface));
        }

        return resFE_INVALID_ARGUMENT;
    }

    // Summary:
    //  Gets an handler interface.
    mxt_result GetHandlerInterface(IN mxt_iid iidRequested, OUT void** ppInterface) const;

    // Summary:
    //  Informs the dispatcher that a new request that did not match any
    //  existing dialog has been received.
    void HandleUnmatchedRequest(IN const CSipPacket& rRequest,
                                IN IEComUnknown* pUserConfig);

    // Summary:
    //  Releases all registered request handlers.
    void ReleaseRequestHandlers();


//-- Hidden Methods.
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceRequestDispatcher(IN const CSceRequestDispatcher& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceRequestDispatcher& operator=(IN const CSceRequestDispatcher& rSrc);


//-- Hidden Data Members.
protected:
private:
    CVector<ISceRequestHandler*> m_vecRequestHandlers;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSCEREQUESTDISPATCHER_H
