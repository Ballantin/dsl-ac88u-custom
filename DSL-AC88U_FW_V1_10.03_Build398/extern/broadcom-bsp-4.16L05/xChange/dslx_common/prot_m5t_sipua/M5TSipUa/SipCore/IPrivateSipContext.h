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
//   form whatsoever without written prior approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_IPRIVATESIPCONTEXT_H
#define MXG_IPRIVATESIPCONTEXT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------

#ifndef MXG_ISIPCONTEXT_H
#include "SipCore/ISipContext.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipForkedDialogGrouper;
class CSipPacket;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IPrivateSipContext);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IPrivateSipContext
//========================================
//
// Description:
//   This interface is internal to the SIP stack and should not be used by stack
//   users. The ISipContext interface should be used instead.
//
//   It is used by the CSipForkedDialogGrouper object to dispatch requests and
//   responses on the appropriate dialog in case of forking.
//
//#The IPrivateSipContext is an ECOM object#
//
//  The IPrivateSipContext is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipContext</B>
//
//  <B>Interface Id: IID_IPrivateSipContext</B>
//
// Location:
//   SipCore/IPrivateSipContext.h
//
// See Also:
//   ISipContext
//
//==EDOC========================================================================
class IPrivateSipContext : public ISipContext
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IPrivateSipContext);
    //M5T_INTERNAL_USE_END


    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  HandlePacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tries to handle an incoming packet.
    //
    //  Parameters:
    //    rPacket:
    //      The packet that was just received.
    //
    //  Returns:
    //    resS_OK: The packet has been handled by the context.
    //
    //    resFE_FAIL: The incoming packet has not been handled.
    //
    //    resFE_DUPLICATE: Unable to create the server transaction, as it
    //    already exists.
    //
    //  Description:
    //      This method is used by the forked dialog grouper to have the SIP
    //      context and the services it holds process the incoming packet.
    //
    //      The result returned by this method usually depends on the services
    //      that are attached to the SIP context.
    //
    //      When calling this on a newly created SIP context that has stateful
    //      services attached to it (UA service or stateful proxy service), it
    //      is possible that resFE_DUPLICATE is returned, in which case the
    //      context should be released immediately. This usually happens when
    //      the time between a request and its retransmissions is too short.
    //
    //==EDOC====================================================================
    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  SetForkedDialogGrouper
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the forked dialog grouper to use in this context.
    //
    //  Parameters:
    //    pGrouper:
    //      A pointer to the forked dialog grouper. It can be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: Attempting to set a non NULL forked dialog
    //      grouper while there is already one.
    //
    //      - resS_OK: The forked dialog grouper is properly set or reset.
    //
    //  Description:
    //      Sets the forked dialog grouper in this context.
    //
    //      By default, contexts are created without a forked dialog grouper.
    //      When a service issues a request that can be forked by downstream
    //      proxies, it must enable a forked dialog grouper on the created
    //      ISipRequestContext. The created CSipForkedDialogGrouper will call
    //      this method on its originator context through this method when the
    //      request is sent on the ISipRequestContext. When forked dialogs can
    //      no longer be created, the CSipForkedDialogGrouper calls this method
    //      again but this time with NULL as a parameter.
    //
    //      This method is also called when a newly created ISipContext handles
    //      a forked packet through ISipForkedDialogGrouper::ContextCreated.
    //      Again, this method is called with NULL when the
    //      CSipForkedDialogGrouper is not needed for the ISipContext anymore.
    //
    //      CSipForkedDialogGrouper should be the only class calling this
    //      method.
    //
    //      The method will fail when attempting to set a non-NULL forked dialog
    //      grouper pointer and there is already another one. This is because
    //      the context cannot be used to create a dialog with two different
    //      requests at the same time. When this method fails, the service must
    //      report a failure and act as if the request could not be sent.
    //
    //==========================================================================
    virtual mxt_result SetForkedDialogGrouper(IN CSipForkedDialogGrouper* pGrouper) = 0;

    //==SDOC====================================================================
    //==
    //==  GetForkedDialogGrouper
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the forked dialog grouper for this context.
    //
    //  Parameters:
    //    rpGrouper:
    //      Returns a pointer to the forked dialog grouper for this context. It
    //      can be NULL when no dialog grouper is currently set. A reference is
    //      counted for this parameter when non NULL.
    //
    //  Description:
    //      Returns the forked dialog grouper for this context.
    //
    //==EDOC====================================================================
    virtual void GetForkedDialogGrouper(OUT CSipForkedDialogGrouper*& rpGrouper) = 0;

    //==========================================================================
    //==
    //==  GetReqCtxCoreSvc
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves a vector of interested services for a particular SIP
    //      transaction.
    //
    //  Parameters:
    //    rPacket:
    //      The packet that is creating the transaction.
    //
    //    eDirection:
    //      Identifies whether the packet was just received or is about to be
    //      sent.
    //
    //    rvecpSvc:
    //      Vector containing the services that are interested in this
    //      transaction.
    //
    //  Description:
    //      This method is used to retrieve a ISipReqCtxCoreSvc instance of all
    //      services that are interested in the transaction that is about to
    //      take place.
    //
    //  See Also:
    //      GetReqCtxConnectionSvc
    //
    //==========================================================================
    virtual void GetReqCtxCoreSvc(IN const CSipPacket& rPacket,
                                  IN ISipCoreSvc::EDirection eDirection,
                                  INOUT CVector<ISipReqCtxCoreSvc*>& rvecpSvc) = 0;

    //==========================================================================
    //==
    //==  GetReqCtxConnectionSvc
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves a vector of interested connection services for a new SIP
    //      transaction.
    //
    //  Parameters:
    //    rPacket:
    //      The packet that is about to be sent or received.
    //
    //    eDirection:
    //      Identifies whether the packet was just received or is about to be
    //      sent.
    //
    //    rvecpSvc:
    //      Vector containing the services that are interested in this
    //      transaction.
    //
    //  Description:
    //      This method is used to retrieve a ISipReqCtxConnectionSvc instance
    //      of all services that are interested in the transaction that is about
    //      to take place.
    //
    //  See Also:
    //      GetReqCtxCoreSvc
    //
    //==========================================================================
    virtual void GetReqCtxConnectionSvc(IN const CSipPacket& rPacket,
                                        IN ISipConnectionSvc::EDirection eDirection,
                                        INOUT CVector<ISipReqCtxConnectionSvc*>& rvecpSvc) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IPrivateSipContext() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IPrivateSipContext() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IPrivateSipContext(IN const IPrivateSipContext& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IPrivateSipContext& operator=(IN const IPrivateSipContext& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IPRIVATESIPCONTEXT_H

