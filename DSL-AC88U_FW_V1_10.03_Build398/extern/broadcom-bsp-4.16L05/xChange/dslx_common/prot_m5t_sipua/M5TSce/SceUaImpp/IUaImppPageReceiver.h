//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_IUAIMPPPAGERECEIVER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPPAGERECEIVER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif



MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class IUaImppPageReceiverMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IUaImppPageReceiver);

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppPageReceiver
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//    The UA IMPP Page Receiver component allows an application to receive and
//    handle page mode IM requests.
//
// Description:
//   The UA IMPP Page Receiver component allows an application to receive and
//   handle page mode IM requests. More specifically, for the SIP protocol, this
//   component handles all incoming SIP MESSAGE requests for a specific user.
//
//   The SIP MESSAGE request allows page mode instant messaging only. Thus, a
//   SIP session is not established between the two peers when they exchange
//   such instant messages. This means that two MESSAGE requests sent to the
//   same destination may end up reaching, and being handled by, different
//   endpoints. This behavior depends on the network configuration and on how
//   proxies in the path handle the request.
//
//   This component allows the application to handle the reception of MESSAGE
//   requests for a single SIP user. Note that a single instance of this
//   component can handle all MESSAGE requests from different peers for a single
//   local user.
//
//   This component does not automatically send responses to received MESSAGE
//   requests. Its manager must use the SendResponse API to do so. Only final
//   responses (greater or equal to 200) should be sent to MESSAGE requests.
//
// Location:
//   SceUaImpp/IUaImppPageReceiver.h
//
//==============================================================================
class IUaImppPageReceiver : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaImppPageReceiver);
    //M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which events are reported for this object.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. Can be NULL.
    //
    //  Returns:
    //      - resS_OK: Manager set correctly.
    //
    //  Description:
    //      Configures the manager to which this object reports events.
    //
    //      If set to NULL no more events will be reported and a 500 response
    //      will automatically be sent.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN IUaImppPageReceiverMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the component with a user configuration object.
    //
    //  Parameters:
    //      pConfig:
    //       Pointer to the ECOM configuration object for this user.
    //
    //  Returns:
    //      - resS_OK: Configuration correctly set.
    //      - resFE_INVALID_ARGUMENT: Config is NULL, invalid or already set.
    //      - resFE_FAIL: Other errors.
    //
    //  Description:
    //      Configures this component with a user configuration object. This
    //      component will handle all MESSAGE requests for this user.
    //
    //==========================================================================
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig) = 0;

    //==========================================================================
    //==
    //==  GetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the component's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the component.
    //      
    //==========================================================================
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig) = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an opaque paramerer to the component.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the component. It can be used by the
    //      application to store any information.
    //
    //==========================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;


    //==========================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the opaque parameter to the component.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the component. It can be used by
    //      the application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==========================================================================
    //==
    //==  SendResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a response to a previously received MESSAGE request.
    //
    //  Parameters:
    //    opqTransaction:
    //      The opaque transaction identifier received in
    //      IUaImppPageReceiverMgr::EvMessageReceived. After a call to this
    //      method opqTransaction will not be usable anymore. This means that
    //      there can be only one SendResponse with an opaque.
    //
    //    uCode:
    //      The SIP response code to send. The application should send a 200 if
    //      the payload was successfully parsed and understood. Otherwise, a 400
    //      class response should be sent.
    //
    //    szReason:
    //      The reason string to send. Can be NULL, in which case the default
    //      reason phrase is used.
    //
    //  Returns:
    //      - resS_OK: Response sent.
    //      - resFE_FAIL: The response could not be sent.
    //
    //  Description:
    //      Sends a response to a received MESSAGE request. Since the UA IMPP
    //      Page Receiver component delegates the handling of the MESSAGE
    //      payload to the application, this method allows the application to
    //      send an error in case an error is encountered while parsing the
    //      payload.
    //
    //  See Also:
    //      IUaImppPageReceiverMgr::EvMessageReceived
    //
    //==========================================================================
    virtual mxt_result SendResponse(IN mxt_opaque opqTransaction,
                                    IN unsigned int uCode,
                                    IN const char* szReason) = 0;

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  HandleIncomingMessage
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the PageReceiver to proceed with an incoming MESSAGE.
    //
    //  Parameters:
    //      rRequest:
    //       The request to handle.
    //
    //  Returns:
    //      - resFE_FAIL:
    //         rRequest is not handled.
    //
    //      - resS_OK:
    //         rRequest is being handled.
    //
    //  Description:
    //      This method must be called after the PageReceiver has been
    //      configured with its user configuration object (see SetConfiguration)
    //      and with its manager (see SetManager).
    //
    //      It informs the PageReceiver that it can proceed with the incoming
    //      MESSAGE request.
    //
    //      This method does not have to be called by the application. It is
    //      called automatically by the UaSImppPageReceiverHandler.
    //
    //  See Also:
    //      SetConfiguration, SetManager
    //
    //==========================================================================
    virtual mxt_result HandleIncomingMessage(IN const CSipPacket& rRequest) = 0;
//M5T_INTERNAL_USE_END


//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IUaImppPageReceiver(){};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IUaImppPageReceiver(){};


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IUaImppPageReceiver(IN const IUaImppPageReceiver& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IUaImppPageReceiver& operator=(IN const IUaImppPageReceiver& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_IUAIMPPPAGERECEIVER_H

