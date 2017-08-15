//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPCOREOUTPUTCONTROLLINGSVC_H
#define MXG_ISIPCOREOUTPUTCONTROLLINGSVC_H

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
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipCoreOutputControllingMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID( ISipCoreOutputControllingSvc );
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipCoreOutputControllingSvc
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//   The output controlling service is a connection service that notifies its
//   manager when asked to update a packet that is about to be sent.
//   The manager can then update the packet in any way it sees fit.
//
//   The order in which this service is attached to the context is important. 
//   This will decide at what moment the service will consult its manager. For
//   example, if the ISipCoreOutputControllingSvc is attached after all the
//   connection services, the manager will see the packet after all the
//   connection services have modified it.
//
//#The ISipCoreOutputControllingSvc is an ECOM object#
//
//  The ISipCoreOutputControllingSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipCoreOutputControllingSvc</B>
//
//  <B>Interface Id: IID_ISipCoreOutputControllingSvc</B>
//
//  A user can query the ISipContext to which this service is attached by 
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipCoreOutputControllingSvc.h
//
// See Also:
//   ISipCoreOutputControllingMgr
//
//==EDOC========================================================================
class ISipCoreOutputControllingSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipCoreOutputControllingSvc);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager associated with this service.
    //
    //  Parameters:
    //    pMgr:
    //      The manager. It must not be NULL.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT:
    //          pMgr is NULL.
    //
    //      resS_OK:
    //          Otherwise.
    //
    //  Description:
    //      Configures the manager that will receive the events generated by
    //      this service.
    //
    //      Note that a manager MUST be associated with this service before it
    //      is used. If it has no configured managers, this service will 
    //      block every outgoing packet.
    //
    //  See also:
    //      ISipCoreOutputControllingMgr
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipCoreOutputControllingMgr* pMgr) = 0;

    //==SDOC====================================================================
    //==
    //==  PacketAsynchronouslyUpdated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies this service that a packet is ready to be sent 
    //      after an asynchronous update by the application.
    //
    //  Parameters:
    //    opqId:
    //      The opaque value received in
    //      ISipCoreOutputControllingMgr::EvUpdatePacket as the opqId parameter.
    //      This parameter MUST match a previous opaque ID given to the
    //      manager.
    //
    //    pPacket:
    //      A pointer to the updated packet. It MUST point to the 
    //      SAME packet (not a copy) that was passed in
    //      ISipCoreOutputControllingMgr::EvUpdatePacket with the SAME
    //      corresponding opaque value.
    //
    //      After this method is called, the pointer must no longer be used.
    //
    //    resUpdate:
    //      Result of the packet update. Possible values are as follows:
    //      resS_OK:
    //          The update has been successful and the the packet should
    //          be sent on the network.
    //      resFE_FAIL:
    //          The update has failed for some undisclosed reason. 
    //          For instance, it could be because there are no routes for 
    //          the packet source and destination. In this case,
    //          the packet is not sent on the network. If it is a 
    //          response, it is dropped. If it is a request, 
    //          a 503 Service Unavailable final negative response is 
    //          automatically generated. 
    //
    //  Returns:
    //      resFE_INVALID_STATE:
    //          Invalid packet. EvUpdatePacket was not called with this
    //          packet or the packet was already updated synchronously.
    //
    //      resFE_INVALID_ARGUMENT :
    //          - pPacket is NULL. 
    //          - opqId is 0. The opaque ID MUST BE THE SAME as was passed
    //            to the manager via EvUpdatePacket.
    //
    //      resS_OK :
    //          The updated packet is accepted by this service and will 
    //          continue being processed by the context.
    //
    //  Description:
    //      Notifies this service that a packet is ready to be sent 
    //      after an asynchronous update by the application.
    //      For instance, the application could have chosen an
    //      interface on which the packet should be sent. It could also have
    //      updated the payload of the packet or fixed its RTP port. Moreover,
    //      it could have updated the From, Contact, and Via headers of the 
    //      packet and updated the ISipUserAgentSvc configuration of these 
    //      headers if required.
    //
    //      This method must be called successfully for every call to
    //      ISipCoreOutputControllingMgr::EvUpdatePacket in which the manager
    //      returns ISipCoreOutputControllingMgr::eASYNCHRONOUSLY_UPDATED.
    //
    //  See also:
    //      ISipCoreOutputControllingMgr::EvUpdatePacket
    //
    //==EDOC====================================================================
    virtual
        mxt_result PacketAsynchronouslyUpdated(IN mxt_opaque opqId,
                                               IN TO CSipPacket* pPacket,
                                               IN mxt_result resUpdate) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipCoreOutputControllingSvc(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipCoreOutputControllingSvc(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipCoreOutputControllingSvc(IN const ISipCoreOutputControllingSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipCoreOutputControllingSvc& operator=(
                                IN const ISipCoreOutputControllingSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCOREOUTPUTCONTROLLINGSVC_H

