//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPDIALOGMATCHER_H
#define MXG_ISIPDIALOGMATCHER_H

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
class CSipDialogMatcherList;
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipDialogMatcher);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipDialogMatcher
//========================================
// <GROUP SIPCORE_CLASSES>
//
// Description:
//   This interface is used by the CSipDialogMatcherList to handle the packet
//   only when its dialog identification matches that of this object.
//
// Location:
//   SipCore/ISipDialogMatcher.h
//
// See Also:
//   CSipDialogMatcherList
//
//==EDOC========================================================================
class ISipDialogMatcher: public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipDialogMatcher);
    //M5T_INTERNAL_USE_END

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  OnPacketReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Handles the packet with the parent context if it belongs to this
    //      dialog.
    //
    //  Parameters:
    //    rPacket:
    //      The received packet to handle.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The dialog state does not enable this service
    //      to handle this packet. It probably means that the packet belongs to
    //      another dialog.
    //
    //      - resFE_FAIL: The packet belongs to this dialog but none of the
    //      services attached to the same context as this service were able to
    //      handle the packet.
    //
    //      - resS_OK: The packet was correctly handled. Either the service that
    //      handled it automatically processed it or an event was reported
    //      through its manager interface to inform the application of the
    //      action to take. It can also be that the packet was a retransmission
    //      of a packet handled by a transaction attached to the same context, 
    //      in which case the packet is silently igored.
    //
    //  Description:
    //      This method verifies if the received packet belongs to the dialog
    //      this service handles, in which case the packet is handled by its
    //      parent SIP Context.
    //
    //      Dialog matching rules are based on RFC 3261, section 12.2.2. The
    //      Call-ID header, the To header Tag parameter, and the From header Tag
    //      parameter are used. Note that when this service is unitialized, it
    //      considers that it can handle any packet.
    //
    //      After determining that the packet belongs to this dialog, the
    //      implementation should verify that the CSeq number is not out of
    //      order. If it is, it should create a request context and a
    //      CSipUserAgentReqCtxSvc to put as its owner and make that request
    //      context handle the packet. The CSipUserAgentReqCtxSvc automatically
    //      answers with a "500 CSeq Number Out of Order" response. Note that
    //      out of order CSeq should apply only to requests within a dialog.
    //
    //      If the packet's CSeq number is in sequence or the packet belongs to
    //      this dialog, it is given to the parent ISipContext::OnPacketReceived
    //      method. An owner is determined and the packet is given to it.
    //
    //==EDOC====================================================================
    virtual mxt_result OnPacketReceived(IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  SetDialogMatcherList
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the dialog matcher list.
    //
    //  Parameters:
    //    rDialogMatcherList
    //      A reference to the dialog matcher list.
    //
    //  Description:
    //      Sets the dialog matcher list that is used to keep a list of active
    //      dialogs for services that implement that interface.
    //
    //      When adding the user agent service or the session stateful proxy
    //      service to a context, the application is responsible to call this 
    //      method by passing the proper dialog matcher list. This is generally 
    //      done in the creation of the new SIP context through the 
    //      EvOnPacketReceived event or when creating a context to send a new 
    //      request.
    //
    //      Note that for most cases, applications should support only one
    //      instance of the dialog matcher list. The CDialogMatcherList should
    //      be created in the initialization process of the application and be
    //      shared with every SIP context through this method. Therefore, 
    //      releasing the CDialogMatcherList should be done in the application
    //      finalization. In that scenario, the same instance is passed through
    //      this method after the context was created and the required services
    //      attached. The dialog matcher list can also be configured in the 
    //      application initialization through the SetDefaultDialogMatcherList 
    //      method of the ISipCoreConfig.
    //
    //      However, for some reasons, the implementation may wish to support 
    //      more than one dialog matcher list. In that case, the application is
    //      responsible to give the received CSipPacket to the proper dialog
    //      matcher list through the EvOnPacketReceived event. As a guide to
    //      determine to which dialog the packet must be given, the following is 
    //      what the application has to look for:
    //
    //          - Request: the source of the packet (both IP and port)
    //          - Response: the topmost Via header (both host and port)
    //
    //  See Also:
    //      GetDialogMatcherList, CSipDialogMatcherList
    //
    //==EDOC====================================================================
    virtual void SetDialogMatcherList(
                            IN CSipDialogMatcherList& rDialogMatcherList) = 0;

    //==SDOC====================================================================
    //==
    //==  GetDialogMatcherList
    //==
    //==========================================================================
    //
    //  Summary:
    //      Obtains the dialog matcher list.
    //
    //  Returns:
    //      Pointer to the configured CSipDialogMatcherList. It can return NULL
    //      when called before SetDialogMatcherList or when
    //      ISipCoreConfig::SetDefaultDialogMatcherList has not been called.
    //
    //  Description:
    //      Returns a pointer to configured CSipDialogMatcherList.
    //
    //  See Also:
    //      SetDialogMatcherList, CSipDialogMatcherList
    //
    //==EDOC====================================================================
    virtual CSipDialogMatcherList* GetDialogMatcherList() = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipDialogMatcher() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipDialogMatcher() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipDialogMatcher(IN const ISipDialogMatcher& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipDialogMatcher& operator=(IN const ISipDialogMatcher& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPDIALOGMATCHER_H

