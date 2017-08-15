//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPSERVEREVENTCONTROL_H
#define MXG_ISIPSERVEREVENTCONTROL_H

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
class CHeaderList;
class CSipMessageBody;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipServerEventControl);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipServerEventControl
//========================================
// <GROUP SIPCORE_CLASSES>
//
// Description:
//  This interface is provided to managers that are receiving stack events
//  associated with a server transaction. It is provided as a parameter to such
//  events.
//
//  A final response MUST be sent using this interface or the
//  ISipDialogServerEventControl interface before the context generating the
//  event can be cleared and released.
//
//  This interface can be used to send multiple provisional (1xx) responses and
//  only one final response.
//
//  This interface is an ECOM interface and as such ECOM reference rules apply.
//
// Location:
//   SipCore/ISipServerEventControl.h
//
//==EDOC========================================================================
class ISipServerEventControl: public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipServerEventControl);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  SendResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a response for a request received on this transaction.
    //
    //  Parameters:
    //    uCode:
    //      The response code to use in the sent response.
    //
    //    szReason:
    //      The reason phrase to use in the sent response. If NULL, the default
    //      reason phrase for the specified uCode is used.
    //
    //    pExtraHeaders:
    //      Additional headers to put in the sent response. It can be NULL.
    //      Ownership is TAKEN.
    //
    //    pMessageBody:
    //      Message-body to put in the sent response. It can be NULL.
    //      Ownership is TAKEN.
    //
    //  Returns:
    //      - resFE_FAIL: The owner service is done with this request context
    //                    meaning that the final response was probably already
    //                    sent.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: The response was blocked by one of
    //                                      the services. The application MUST
    //                                      act as if the response was sent
    //                                      (resS_OK returned) if this code is
    //                                      returned for a final response.
    //
    //      - resS_OK: The response was successfully sent.
    //
    //  Description:
    //      Creates a response with the described status code and reason phrase
    //      for the request received on the server transaction. It will then
    //      add the extra headers and content information if any is provided in
    //      parameter.
    //
    //      This method can be called as often as desired for provisional
    //      responses (1xx status code) and MUST be called once for final
    //      response (status code <= 200). Once SendResponse has been
    //      successfully called to send a final response, the manager should
    //      release its reference to this interface.
    //
    //      A final response could be blocked by one of the services. In such
    //      case, the application MUST act as if the response was sent (i.e.
    //      release this ISipServerEventControl if a reference is held on it).
    //      The stack will clean its internal resources as if resS_OK was
    //      returned if this case happens.
    //
    //==EDOC====================================================================
    virtual mxt_result SendResponse(IN unsigned int uCode,
                                    IN const char* szReason,
                                    IN TO CHeaderList* pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody) = 0;

    //==SDOC====================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the user-specified opaque parameter associated with this
    //      transaction.
    //
    //  Returns:
    //      The opaque parameter.
    //
    //  Description:
    //      Gets the user-specified opaque parameter associated with this
    //      transaction.
    //
    //      This is the opaque parameter that is set through
    //      ISipServerEventControl::SetOpaque.
    //
    //  See Also:
    //      SetOpaque
    //
    //==EDOC====================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==SDOC====================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the user-specified opaque parameter associated with this
    //      transaction.
    //
    //  Parameters:
    //    opq:
    //      The opaque parameter.
    //
    //  Description:
    //      Sets the user-specified opaque parameter associated with this
    //      transaction.
    //
    //      This is the opaque parameter that is obtained through
    //      ISipServerEventControl::GetOpaque.
    //
    //  See Also:
    //      GetOpaque
    //
    //==EDOC====================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void SetSendOption(IN bool bToNetwork) = 0;
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

//-- Hidden Methods
//-------------------
protected:

    //-- << Protected Constructors / Destructors >>
    //-------------------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipServerEventControl(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipServerEventControl(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipServerEventControl(IN const ISipServerEventControl& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipServerEventControl& operator=(IN const ISipServerEventControl& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSERVEREVENTCONTROL_H

