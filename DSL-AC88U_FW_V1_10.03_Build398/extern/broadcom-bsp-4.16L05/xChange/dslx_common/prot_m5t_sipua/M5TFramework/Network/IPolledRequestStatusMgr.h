//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation
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
#ifndef MXG_IPOLLEDREQUESTSTATUSMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IPOLLEDREQUESTSTATUSMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_OS_SYMBIAN
                                 // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_ECOM_ENABLE_SUPPORT
#endif

#if defined(MXD_OS_SYMBIAN) && \
    defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_ECOM_ENABLE_SUPPORT)

//-- Data Members
//-----------------

//-- Interface Realized and/or Parent
//-------------------------------------
#include <e32cmn.h> // TRequestStatus

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------


//==SDOC========================================================================
//== Class: IPolledRequestStatusMgr
//========================================
//<GROUP NETWORK_CLASSES>
//<FLAG Symbian OS ONLY>
//
// Summary:
//   Interface used for CPollRequestStatus.
//
// Description:
//   This interface is to be used with the class CPollRequestStatus. When
//   registering a request status to CPollRequestStatus, the user must provide
//   this type of interface.  Then, on a subsequent call to
//   CPollRequestStatus::Poll,this allows the class CPollRequestStatus to
//   call back the user who registered a TRequestStatus once the request
//   completed.
//
// Location:
//   Network/IPolledRequestStatusMgr.h
//
// See Also:
//   CPollRequestStatus
//
//==EDOC========================================================================
class IPolledRequestStatusMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //==SDOC====================================================================
    //==
    //==  EvPolledRequestStatusMgrEventDetected
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager about newly detected request completion.
    //
    //  Parameters:
    //      pRequestStatus:
    //          The request status for the completed request.
    //
    //      opq:
    //          The opaque value provided at registration.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - resS_OK
    //
    //  Description:
    //      This method is called when completion of a request is detected.
    //
    //      Note: the parameter pRequestStatus is there for flexibility. It
    //      gives the user the possibility to use the same
    //      IPolledRequestStatusMgr for a group of request status handles.
    //
    //  See Also:
    //      Network/CPollRequestStatus.h
    //
    //
    //==EDOC====================================================================
    virtual void EvPolledRequestStatusMgrEventDetected(IN TRequestStatus* pRequestStatus,
                                                       IN mxt_opaque opq) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //==SDOC====================================================================
    //==
    //==  IPolledRequestStatusMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Constructor.
    //
    //==EDOC====================================================================
    IPolledRequestStatusMgr() {}

    //==SDOC====================================================================
    //==
    //==  ~IPolledRequestStatusMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==EDOC====================================================================
    virtual ~IPolledRequestStatusMgr() {}
//M5T_INTERNAL_USE_END
};


MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_OS_SYMBIAN) && 
       //     defined(MXD_NETWORK_ENABLE_SUPPORT) && 
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_IPOLLEDREQUESTSTATUSMGR_H
