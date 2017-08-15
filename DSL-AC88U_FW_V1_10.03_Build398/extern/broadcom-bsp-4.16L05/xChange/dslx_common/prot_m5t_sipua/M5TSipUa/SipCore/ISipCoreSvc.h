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
#ifndef MXG_ISIPCORESVC_H
#define MXG_ISIPCORESVC_H

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
class ISipReqCtxCoreSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID( ISipCoreSvc );
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==SDOC========================================================================
//== Class: ISipCoreSvc
//========================================
//
// Description:
//   This interface is used by the CSipContext and the CSipRequestContext to
//   obtain core services interested in a client or server transaction. This
//   interface is an ECOM interface but it is cannot be queried outside of the
//   CSipContext. The CSipContext knows all the core services it aggregates
//   through their IEComUnknown interface and through this interface.
//   ISipCoreSvc allows the CSipContext to ask the core services whether they
//   would like to handle a server transaction, ask them for a request context
//   core service instance to handle client transaction they issue, and obtain a
//   request context core service instance to observe the progression of
//   transaction that they do not handle.
//
//   Services aggregated to the CSipContext must implement either this interface
//   or the ISipConnectionSvc interface.
//
//
// Location:
//   SipCore/ISipCoreSvc.h
//
// See Also:
//   ISipReqCtxCoreSvc, CSipContext, CSipRequestContext, ISipConnectionSvc
//
//==EDOC========================================================================
class ISipCoreSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipCoreSvc);
    //M5T_INTERNAL_USE_END

    // This enum helps prioritize owner selection among services. The lower the
    // enum value is, the higher is the priority. The selection algorithm
    // implemented in the CSipContext does not look at any specific value
    // except eCANNOT_HANDLE which means that the service cannot be put as the
    // owner for the transaction. The CSipContext asks every core service for
    // its confidence level. It then selects the lower confidence level
    // obtained and, among the services that returned that confidence level, it
    // selects the first one to do so. If the confidence level is not
    // eCANNOT_HANDLE, it use that core service as the owner.
    //--------------------------------------------------------------------------
    enum EOwnerConfidenceLevel
    {
        // The service knows how to process this packet and is in a state where
        // it can handle this packet with success. The service also knows that
        // it can handle this packet in a better way than services returning
        // eEXPECTED.
        //----------------------------------------------------------------------
        eMUST_HANDLE,

        // The service knows how to process this packet and is in a state where
        // it can handle this packet with success.
        //----------------------------------------------------------------------
        eEXPECTED,

        // The service knows how to process this packet but is in a state where
        // it would automatically fail the packet. A service returning this
        // level of confidence can be used if no other service returned an
        // eEXPECTED level of confidence.
        //----------------------------------------------------------------------
        eUNEXPECTED,

        // The service can process this packet in a generic way. This means
        // that it will probably only present the packet to the application and
        // let the application decide what to do with it.
        //----------------------------------------------------------------------
        eDEFAULT,

        // The service does not understand that packet and does not wish to own
        // the request context that will process it.
        //----------------------------------------------------------------------
        eCANNOT_HANDLE,
    };

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
    //==  GetReqCtxCoreSvcInstance
    //==
    //==========================================================================
    //
    //  Summary:
    //      Obtains an instance of an ISipReqCtxCoreSvc to attach to a request
    //      context.
    //
    //  Parameters:
    //    rPacket:
    //      The initial packet for the request context to create.
    //
    //    eDirection:
    //      Whether the packet is received or to be sent.
    //
    //  Returns:
    //      A pointer to the ISipReqCtxCoreSvc to attach to the request context.
    //      It can be NULL.
    //
    //  Description:
    //      Obtains an instance of an ISipReqCtxCoreSvc to attach to a request
    //      context that manages the packet in parameter. Note that the request
    //      context also manages all responses, timeouts, and network errors
    //      associated with the packet in parameter. Hence, services interested
    //      in responses, timeouts, or network errors associated with the packet
    //      must also give an appropriate ISipReqCtxCoreSvc even if they are not
    //      interested in the request itself. For instance, the client 
    //      authentication service has no interest in an initial outgoing
    //      INVITE, but since it has interest in its response when it is a 401
    //      or a 407, it returns a non-NULL ISipReqCtxCoreSvc.
    //
    //      The service can use whatever algorithm it wants to decide what kind
    //      of instance to return.
    //
    //      If the service is not interested in the request, it can return a
    //      NULL pointer. It should also return NULL if it is the owner of the
    //      request context for that type of transaction.
    //
    //      The service could also implement the ISipReqCtxCoreSvc interface and
    //      return a pointer to itself. This would be the case, for instance, if
    //      the service can play its part in managing this packet but does not
    //      require any particular state for that type of transaction.
    //
    //      The ISipReqCtxCoreSvc could also be implemented by another object
    //      that is created by the service.
    //
    //      Note that when the return value is non-NULL, the caller MUST
    //      increment the reference count (through a call to AddRcsRef()). It
    //      must do so even if it does not plan to keep that reference (in which
    //      case it would call ReleaseRcsRef() immediately) because there is a
    //      possibility that the ISipReqCtxCoreSvc points to a newly created
    //      object that is not referenced by any other object and that object
    //      would never be removed from the memory.
    //
    //  See Also:
    //      ISipReqCtxCoreSvc
    //
    //==EDOC====================================================================
    virtual
    ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                IN EDirection eDirection) = 0;

    //==SDOC====================================================================
    //==
    //==  GetOwnerConfidenceLevel
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gives to which degree the service is a good candidate to own a
    //      request context.
    //
    //  Parameters:
    //    rPacket:
    //      The received packet that is handled by the request context to create.
    //
    //  Returns:
    //      An enum value used to prioritize one service against another to
    //      become the owner of the request context to create.
    //
    //  Description:
    //      Returns an enum that can be used to determine which service has the
    //      most specific knowledge about how to handle the request in
    //      parameter.
    //
    //      The enums are ordered from the more specific to the least specific.
    //      The highest value is eCANNOT_HANDLE and is returned when the service
    //      knows that it cannot own the request context that handles the
    //      packet.
    //
    //      The four other values defined are eDEFAULT, which means that the
    //      service can process the packet in a generic way if no other service
    //      knows how to process it; eUNEXPECTED, which means that the packet is
    //      of a kind understood by the service but that the service is in a 
    //      state where that packet should not arrive; eEXPECTED, which  means 
    //      that the packet is understood and arrives while the service is in a 
    //      state where that packet is acceptable; and finally eMUST_HANDLE when
    //      the service knows that it can handle the request more specifically 
    //      than any other service.
    //
    //  See Also:
    //      EOwnerConfidenceLevel
    //
    //==EDOC====================================================================
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                              IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  GetOwnerInstance
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the ISipReqCtxCoreSvc instance to use as the owner for a
    //      request context.
    //
    //  Parameters:
    //    rPacket:
    //      The received packet that is handled by the request context service
    //      to obtain.
    //
    //    rpSipReqCtxCoreSvc:
    //      Reference to the ISipReqCtxCoreSvc that owns a CSipRequestContext.
    //      A reference is counted, it is up to the caller to release the
    //      reference when done with it.
    //
    //  Description:
    //      This method is similar to the GetReqCtxCoreSvcInstance method except
    //      that it gets the reference to the request context service that
    //      acts as the owner of the CSipRequestContext.
    //
    //      In client case, the owner is generated by the service that was used
    //      to send the request.
    //
    //      In server case, the owner is determined by calling
    //      GetOwnerConfidenceLevel on each core service in the CSipContext and
    //      choosing the one with the highest priority.
    //
    //  See Also:
    //      GetOwnerConfidenceLevel
    //
    //==EDOC====================================================================
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc) = 0;

    //==SDOC====================================================================
    //==
    //==  ClearCoreSvc
    //==
    //==========================================================================
    //
    //  Summary:
    //      Warns the service that the context is about to be destroyed.
    //
    //  Returns:
    //    resSW_SIPCORE_ACTIVE_DIALOG :
    //      The service was responsible for a dialog in the dialog matcher list
    //      when this method was called.
    //
    //    resS_OK :
    //      The service was not responsible for a dialog in the dialog matcher
    //      list.
    //
    //  Description:
    //      This method is called when the application calls Clear on the
    //      context. This means that the application is ready to release the
    //      context and that it no longer wants to be notified for this context.
    //
    //      Implementations should reset their manager and remove the service
    //      from the CSipDialogMatcherList, if applicable. If the service
    //      removes itself from the CSipDialogMatcherList, it must return
    //      resSW_SIPCORE_ACTIVE_DIALOG to warn the application that Clear
    //      was called on the context when there was still an active dialog,
    //      which should not be done.
    //
    //      The service must make sure that all the known references on a
    //      service or the context are released (or will be when the current
    //      transaction ends) because these references prevent the context's 
    //      deletion and cause a memory leak.
    //
    //      The service must NOT automatically send a response if its manager
    //      has been warned of a request since the manager must still have the
    //      ISipServerEventControl to do so by itself.
    //
    //  See Also:
    //      ISipContext::Clear
    //
    //==EDOC====================================================================
    virtual mxt_result ClearCoreSvc() = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipCoreSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipCoreSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipCoreSvc(IN const ISipCoreSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipCoreSvc& operator=(IN const ISipCoreSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCORESVC_H

