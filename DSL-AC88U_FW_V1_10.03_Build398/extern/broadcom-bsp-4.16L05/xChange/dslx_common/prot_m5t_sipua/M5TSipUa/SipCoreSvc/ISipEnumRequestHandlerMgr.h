//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPENUMREQUESTHANDLERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPENUMREQUESTHANDLERMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#ifndef MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT to be able to use this class.
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_NETWORKTOOLS_H
#include "Network/NetworkTools.h"
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

//==SDOC========================================================================
//== Class: ISipEnumRequestHandlerMgr
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//   This interface is used by the CSipEnumRequestHandler to notify the calling
//   object that the asynchronous query is complete.
//
// Location:
//   SipCoreSvc/ISipEnumRequestHandlerMgr.h
//
// See Also:
//   GetEnumUri, CSipEnumRequestHandler, CSipEnumSvc
//
//==EDOC========================================================================
class ISipEnumRequestHandlerMgr
{
//-- Friend Declarations
//------------------------

//-- New types within class namespace
//-----------------------------------
public:
protected:
private:

//-- Published Interface
//------------------------
public:
    //-- << Stereotype >>
    //---------------------

    //==SDOC====================================================================
    //==
    //==  OnGetEnumUriAResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the GetEnumUriA result.
    //
    //  Parameters:
    //      plstEnumUri:
    //       A pointer to a CList of SEnumUri structures containing the URIs
    //       that correspond to the supplied tel-URI. Ownership is taken.
    //
    //      opq:
    //       A mxt_opaque, given previously to the GetEnumUriA method of
    //       CSipEnumRequestHandler.
    //
    //      res:
    //       The result of the GetEnumUri function.
    //
    //  Description:
    //      Notifies the manager that the query is complete. The ownership of
    //      the list is given back to the manager.
    //
    //  See Also:
    //      CSipEnumRequestHandler, GetEnumUri
    //
    //==EDOC====================================================================
    virtual void OnGetEnumUriAResult(IN TO CList<SEnumUri>* plstEnumUri,
                                     IN    mxt_opaque       opq,
                                     IN    mxt_result       res) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors >>
    //-------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipEnumRequestHandlerMgr() {};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipEnumRequestHandlerMgr() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipEnumRequestHandlerMgr(IN const ISipEnumRequestHandlerMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipEnumRequestHandlerMgr& operator=(IN const ISipEnumRequestHandlerMgr& rSrc);

//-- Hidden Data Members
//------------------------
protected:
private:
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPENUMREQUESTHANDLERMGR_H

