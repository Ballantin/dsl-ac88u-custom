//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPCONNECTIONSVC_H
#define MXG_ISIPCONNECTIONSVC_H

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


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipReqCtxConnectionSvc;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID( ISipConnectionSvc );
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipConnectionSvc
//========================================
//
// Description:
//   This interface is used by the CSipContext and the CSipRequestContext to
//   obtain connection services interested in a client or server transaction.
//   This interface is an ECOM interface but it is cannot be queried outside the 
//   CSipContext. The CSipContext knows all the connection services it
//   aggregates through their IEComUnknown interface and through this interface.
//
//   ISipConnectionSvc allows the CSipContext to ask the connection services for
//   a request context connection service instance to attach to a newly created
//   request context.
//
//   Services aggregated to the CSipContext must implement either this interface
//   or the ISipCoreSvc interface.
//
// Location:
//   SipCore/ISipConnectionSvc.h
//
// See Also:
//   ISipReqCtxConnectionSvc, CSipContext, CSipRequestContext, ISipCoreSvc
//
//==EDOC========================================================================
class ISipConnectionSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipConnectionSvc);
    //M5T_INTERNAL_USE_END

    // This enum indicates in which direction is the SIP packet: either sent to
    // the network or received from it.
    //--------------------------------------------------------------------------
    enum EDirection
    {
        // The packet is received from the network.
        //------------------------------------------
        eINCOMING,
        // The packet is sent on the network.
        //------------------------------------
        eOUTGOING,
    };

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  GetReqCtxConnectionSvcInstance
    //==
    //==========================================================================
    //
    //  Summary:
    //      Obtains an instance of an ISipReqCtxConnectionSvc to attach to a
    //      request context.
    //
    //  Parameters:
    //    rPacket:
    //      The initial packet for the request context to create.
    //
    //    eDirection:
    //      Whether the packet is received or to be sent.
    //
    //  Returns:
    //      A pointer to the ISipReqCtxConnectionSvc to attach to the request
    //      context. It can be NULL.
    //
    //  Description:
    //      Obtains an instance of an ISipReqCtxConnectionSvc to attach to a
    //      request context that manages the packet in parameter. Note that
    //      the request context also manages all responses, timeouts, and
    //      network errors associated with the packet in parameter. Hence,
    //      services interested in responses, timeouts, or network errors
    //      associated with the packet must also give an appropriate
    //      ISipReqCtxConnectionSvc even if they are not interested in the
    //      request itself.
    //
    //      The service can use whatever algorithm it wants to decide what kind
    //      of instance to return.
    //
    //      If the service is not interested in the request, it can return a
    //      NULL pointer.
    //
    //      The service could also implement the ISipReqCtxConnectionSvc
    //      interface and return a pointer to itself. This would be the case, 
    //      for instance, if the service can play its part in managing this 
    //      packet but does not require any particular state for that type of
    //      transaction.
    //
    //      The ISipReqCtxConnectionSvc could also be implemented by another
    //      object that is created by the service.
    //
    //      Note that when the return value is non-NULL, the caller MUST
    //      increment the reference count (through a call to AddRcRef()). It
    //      must do so even if it does not plan to keep that reference (in which
    //      case it would call ReleaseRcRef() immediately) because there is a
    //      possibility that the ISipReqCtxConnectionSvc points to a newly
    //      created object not referenced by any other object and that
    //      object would never be removed from the memory.
    //
    //  See Also:
    //      ISipReqCtxConnectionSvc
    //
    //==EDOC====================================================================
    virtual ISipReqCtxConnectionSvc*
    GetReqCtxConnectionSvcInstance(IN const CSipPacket& rPacket,
                                   IN EDirection eDirection) = 0;


    //==SDOC====================================================================
    //==
    //==  ClearConnectionSvc
    //==
    //==========================================================================
    //
    //  Summary:
    //      Warns the service that the context is about to be destroyed.
    //
    //  Description:
    //      This method is called when the application calls Clear on the
    //      context. This means that the application is ready to release the
    //      context and that it no longer wants to be notified for this context.
    //
    //      Implementations should reset their manager.
    //
    //      The service must make sure that all the known references on a
    //      service or the context are released (or will be when the current
    //      transaction ends) because these references will prevent the
    //      context's deletion and will cause a memory leak.
    //
    //  See Also:
    //      ISipContext::Clear
    //
    //==EDOC====================================================================
    virtual void ClearConnectionSvc() = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipConnectionSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipConnectionSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipConnectionSvc(IN const ISipConnectionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipConnectionSvc& operator=(IN const ISipConnectionSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCONNECTIONSVC_H

