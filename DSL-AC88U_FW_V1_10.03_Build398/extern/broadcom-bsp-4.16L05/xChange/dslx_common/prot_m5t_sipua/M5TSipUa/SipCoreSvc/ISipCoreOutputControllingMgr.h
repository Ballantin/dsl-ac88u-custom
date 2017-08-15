//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_ISIPCOREOUTPUTCONTROLLINGMGR_H
#define MXG_ISIPCOREOUTPUTCONTROLLINGMGR_H

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


#ifndef MXD_SIPSTACK_ENABLE_SIP_CORE_OUTPUT_CONTROLLING_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_CORE_OUTPUT_CONTROLLING_SVC_SUPPORT to be able to use \
this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class CString;
class ISipCoreOutputControllingSvc;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipCoreOutputControllingMgr
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//   This interface is used by the output controlling service
//   (ISipCoreOutputControllingSvc) when asked by the context to
//   update a packet.
//
//   This interface is used to notify the application that a packet should
//   be updated. This happens when the output controlling service
//   sees its turn come up in the service list attached to the context.
//   The update may be related to the network interface to use when sending
//   the packet. It may also be related to some headers.
//
// Location:
//   SipCoreSvc/ISipCoreOutputControllingMgr.h
//
// See Also:
//   ISipCoreOutputControllingSvc, CSipPacket
//
//==EDOC========================================================================
class ISipCoreOutputControllingMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    
    // Description:
    //  Indicates how the packet is updated.
    enum EUpdatingSynchronization
    {
        // Description:
        //  The packet is updated or will not be updated at all.
        eSYNCHRONOUSLY_UPDATED,

        // Description:
        //  The packet is not yet updated but will be at a later time.
        eASYNCHRONOUSLY_UPDATED
    };

    //==========================================================================
    //==
    //==  EvUpdatePacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a packet should be updated.
    //
    //  Parameters:
    //    pSvc:
    //      The service currently managing the packet update.
    //
    //    transactionOpaque:
    //      The opaque parameter associated with the transaction.
    //
    //    opqId:
    //      An opaque value associated with the current updated packet.
    //      This value must be passed to the
    //      ISipCoreOutputControllingSvc::PacketAsynchronouslyUpdated method
    //      when the packet is updated asynchronously. It is mandatory
    //      that the SAME opaque value be passed along the corresponding
    //      updated packet.
    //
    //    rPacket:
    //      The packet to update. If the updating is done asynchronously, the
    //      SAME packet (not a copy) must be passed to
    //      ISipCoreOutputControllingSvc::PacketAsynchronouslyUpdated with its
    //      corresponding opaque value.
    //      The packet is only guaranteed to be valid for the duration of
    //      the call. If the manager needs it for longer, for instance when
    //      doing asynchronous updates, a reference must be obtained and
    //      later released.
    //
    //    reSynchronization:
    //      This parameter must be set by the manager. It indicates
    //      to the calling service whether the update will be
    //      completed at a later time.
    //
    //      eSYNCHRONOUSLY_UPDATED:
    //          The packet is updated or will not be updated at all
    //          (synchronous update).
    //          In this case, there is no need to call
    //          PacketAsynchronouslyUpdated on the service
    //          (doing so would fail).
    //
    //      eASYNCHRONOUSLY_UPDATED:
    //          The packet is not yet updated but will be at a later time
    //          (asynchronous update). In this case, once the packet
    //          is updated, PacketAsynchronouslyUpdated MUST be
    //          called on the service. This must be done with the same
    //          packet and opaque value (opId) as passed to this method.
    //          Note that until PacketUpdated is called, no
    //          processing is done on the packet by the M5T stack.
    //
    //    rresUpdate:
    //      This parameter must be set by the manager if reSynchronization
    //      is set to eSYNCHRONOUSLY_UPDATED. It indicates if the packet
    //      update was successful. This parameter is ignored if the update
    //      is asynchronous. The possible results are as follows:
    //      resS_OK:
    //          The update has been successful and the packet should be
    //          sent on the network.
    //      resFE_FAIL:
    //          The update has failed for some undisclosed reason.
    //          For instance, it could be because there are no routes for
    //          the packet source and destination. In this case,
    //          the packet is not sent on the network. If it is a
    //          response, it is dropped. If it is a request,
    //          a 503 Service Unavailable final negative response is
    //          automatically generated.
    //
    //  Description:
    //      Notifies the application that an outoing packet should be updated.
    //
    //      If the update is done synchronously, there is nothing special to
    //      do other than set the reSynchronization parameter to
    //      eSYNCHRONOUSLY_UPDATED and return.
    //
    //      If the manager does not want to update the packet, it should do
    //      nothing but set reSynchronization to eSYNCHRONOUSLY_UPDATED.
    //
    //      If the update is done asynchronously, the manager must
    //      set the reSynchronization parameter to eASYNCHRONOUSLY_UPDATED
    //      before returning from this call. Then, after the packet is
    //      updated, the manager must call PacketAsynchronouslyUpdated
    //      on the service. It is mandatory that the SAME opaque ID (opqId)
    //      and packet be passed back to the service in the call to
    //      ISipCoreOutputControllingSvc::PacketAsynchronouslyUpdated.
    //
    //      Note that when such an asynchronous update occurs,
    //      internal processing of the packet is interrupted. For instance,
    //      the associated context stops forwarding the packet to
    //      downstream services, etc. Consequently, each call to
    //      this method which returns eASYNCHRONOUSLY_UPDATED MUST
    //      be matched to a call to PacketAsynchronouslyUpdated on the
    //      service.
    //
    //      One important caveat of the output controlling service is
    //      when multiple packets are being processed asynchronously by
    //      the application. In this case, it is possible for the
    //      application to effectively change the order packets are
    //      sent onto the network by calling PacketAsynchronouslyUpdated
    //      on out-of-order packets. Typically, this should not occur and
    //      an application should only do so very cautiously.
    //
    //      Indeed, if packets are sent out of order the CSeq header should
    //      be directly massaged by the manager. This is particularly true
    //      when the requests are sent in a dialog or if the CSeq value has
    //      a special meaning even outside a dialog (e.g., REGISTER). In
    //      these cases, if the requests are sent out of order with
    //      unchanged CSeq headers, the server will refuse the requests
    //      with a lower CSeq than the last request received.
    //      Note that this can also happen when mixing synchronous and
    //      asynchronous updates. In this case, a packet updated
    //      synchronously could be sent to the network before a packet
    //      being asynchronously updated.
    //      Generally, if possible, it is simpler for an application to
    //      update packets sequentially and not mix synchronous and
    //      asynchronous processing. A good way to avoid disordering
    //      packets would be to queue them in a FIFO queue, along with
    //      their corresponding opaque ID.
    //
    //      The rPacket parameter is only guaranteed to be valid for the
    //      duration of the call. Thus, for aysnchronous updates, a manager
    //      will add a reference to the packet before returning. Later,
    //      once the packet is updated, the manager will call
    //      PacketAsynchronouslyUpdated on the service and then
    //      release its reference on the packet.
    //
    //      Possible usage of the output controlling service includes
    //      verifying if the network interface chosen to send the packet
    //      is the one the application would also choose. If not, it could
    //      do some modifications to the packet such as changing the
    //      listening address and port for RTP and the address in the Via,
    //      From, and Contact headers. It could also update the corresponding
    //      configuration in the ISipUserAgentSvc of the context.
    //
    //      Care should be taken while updating any of the headers. Some
    //      headers mean different things depending in which request or
    //      response they are. For example, the Contact header has a special
    //      meaning in REGISTER requests and responses.
    //
    //  See Also:
    //      ISipCoreOutputControllingSvc::PacketAsynchronouslyUpdated,
    //      CSipPacket, CSipPacket::GetLocalAddr
    //
    //==========================================================================
    virtual void
        EvUpdatePacket( IN ISipCoreOutputControllingSvc* pSvc,
                        IN mxt_opaque transactionOpaque,
                        IN mxt_opaque opqId,
                        INOUT CSipPacket& rPacket,
                        OUT EUpdatingSynchronization& reSynchronization,
                        OUT mxt_result& rresUpdate) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipCoreOutputControllingMgr(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipCoreOutputControllingMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipCoreOutputControllingMgr(IN const ISipCoreOutputControllingMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipCoreOutputControllingMgr& operator=(
                                IN const ISipCoreOutputControllingMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCOREOUTPUTCONTROLLINGMGR_H

