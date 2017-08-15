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
#ifndef MXG_ISCEREQUESTHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEREQUESTHANDLER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
//-------------------------
class CSipPacket;
class ISceUserConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceRequestHandler);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceRequestHandler
//========================================
//<GROUP TID_PACKAGE_SCECORE_CLASSES>
//
// Summary:
//  Defines the API to receive SIP requests that did not match an existing
//  dialog.
//
// Description:
//  The request handler interface defines the API to receive SIP requests that
//  did not match an existing dialog. Such requests are presented to registered
//  request handlers by the Client Engine. As soon as one request handler
//  returns resS_OK, the processing is interrupted and the Client Engine assumes
//  the packet is now managed by the handler.
//
//  A set of default ISceRequestHandler are automatically registered by
//  the M5T components compiled with the Client Engine. The application using
//  the Client Engine can also register its own handlers by using
//  ISceCoreConfig::RegisterRequestHandler.
//
// Location:
//   SceCore/ISceRequestHandler.h
//
// See Also:
//   ISceCoreConfig::RegisterRequestHandler
//
//==============================================================================
class ISceRequestHandler : public IEComUnknown
{
//-- Friend Declarations.
//------------------------


//-- Published Interface.
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceRequestHandler);
    //M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  HandleRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Presents the handler with an incoming SIP request.
    //
    //  Parameters:
    //    rRequest:
    //      The request.
    //
    //    pUserConfig:
    //      User configuration associated with this request. Can be NULL, in
    //      which case no user was matched to this request.
    //
    //  Returns:
    //      - resS_OK: The request was either handled by the request handler or
    //        dispatched to some other component for further handling.
    //      - resFE_NOT_IMPLEMENTED: The request handler does not know how to
    //        handle the incoming request.
    //
    //  Description:
    //      This interface is used by the CSceRequestDispatcher to
    //      dispatch incoming requests to the proper component. The
    //      implementation must check the CSipPacket to determine if it knows
    //      how to handle it or if it knows how to create a component that will
    //      handle this request.
    //
    //  See Also:
    //      CSceRequestDispatcher
    //
    //==========================================================================
    virtual mxt_result HandleRequest(IN const CSipPacket& rRequest,
                                     IN IEComUnknown* pUserConfig) = 0;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceRequestHandler() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceRequestHandler() {}

private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceRequestHandler(IN const ISceRequestHandler& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceRequestHandler& operator=(IN const ISceRequestHandler& rSrc);

//-- Hidden Data Members.
//------------------------
protected:
private:
//M5T_INTERNAL_USE_END

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEREQUESTHANDLER_H

