//==SDOC========================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPDIALOGSERVEREVENTCTRL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPDIALOGSERVEREVENTCTRL_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

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
MX_DECLARE_ECOM_IID(ISipDialogServerEventCtrl);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipDialogServerEventCtrl
//==============================================================================
// <FLAG New in 4.1.3>
//
// Description:
//  This interface is provided to managers that are receiving stack events
//  associated with a server transaction. It is NOT provided as a parameter to
//  such events. It MUST be queried on the ISipServerEventCtrl interface that is
//  given to the event. It is used to establish a dialog upon sending a response.
//
//  To access this interface, the following MUST be done inside an event handler
//  that receives an ISipServerEventCtrl interface pointer.
//
//  A final response MUST be sent using this interface or the
//  ISipServerEventControl interface before the context generating the
//  event can be cleared and released.
//
//  <CODE>
//  ISipServerEventControl*    pServerEvCtrl; // Assume this is a valid pointer.
//  ISipDialogServerEventCtrl* pDialogSrvEvCtrl = NULL;
//
//  pServerEvCtrl->QueryIf(OUT &pDialogSrvEvCtrl);
//
//  if (pDialogSrvEvCtrl != NULL)
//  {
//      pDialogSrvEvCtrl->SendDialogEstablishingResponse(200, szOK, NULL, NULL);
//      pDialogSrvEvCtrl->ReleaseIfRef();
//      pDialogSrvEvCtrl = NULL;
//  }
//  </CODE>
//
//  This interface is an ECOM interface and as such ECOM reference rules apply.
//
// Location:
//   SipCore/ISipDialogServerEventCtrl.h
//
// See Also:
//   ISipServerEventCtrl
//
//==============================================================================
class ISipDialogServerEventCtrl : public IEComUnknown
{
//-- Friend Declarations
//-----------------------

//-- New types within class namespace
//------------------------------------
public:
protected:
private:

//-- Published Interface
//-----------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipDialogServerEventCtrl);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  SendDialogEstablishingResponse
    //==
    //==========================================================================
    //  <FLAG New in 4.1.3>
    //
    //  Summary:
    //      Sends a response for a request received on this transaction and
    //      establishes a dialog with that response.
    //
    //  Parameters:
    //    uCode:
    //      The response code to use in the sent response.
    //
    //    pszReason:
    //      The reason phrase to use in the sent response. If NULL, the default
    //      reason phrase for the specified uCode is used.
    //
    //    pExtraHeaders:
    //      Additional headers to put in the sent response. It can be NULL.
    //      Ownership is TAKEN.
    //
    //    pMessageBody:
    //      SIP message body (payload) to put in the sent response. It can be
    //      NULL. Ownership is TAKEN.
    //
    //  Returns:
    //       - resS_OK: The response is being sent.
    //
    //       - resSW_DIALOG_NOT_REGISTERED: The dialog is not established
    //                                      but the response is being sent.
    //
    //       - resFE_FAIL: The response could not be sent or a final response
    //                     was already sent on this transaction.
    //
    //       - resFE_SIPCORE_PACKET_BLOCKED: The response was blocked by one of
    //                                       the services. The application MUST
    //                                       act as if the response was sent
    //                                       (resS_OK returned) if this code is
    //                                       returned for a final response.
    //
    //  Description:
    //      Sends a response for a request received on this transaction and
    //      establishes a dialog with that response.
    //
    //      It uses the ISipServerEventControl::SendResponse method to send a
    //      response with the given status code, reason phrase, extra headers,
    //      and message body. It also performs the extra steps required to
    //      establish a dialog. The application is responsible to call the
    //      ISipUserAgentSvc::TerminateUsage() method once the established
    //      dialog is over.
    //
    //      This method must only be called when the response sent is to
    //      establish a dialog. When the response MUST not establish a dialog,
    //      the ISipServerEventControl::SendResponse method SHOULD be used
    //      directly. Once SendDialogEstablishingResponse has been successfully
    //      called to send a final response, the manager should release its
    //      reference to this interface.
    //
    //      If the dialog can not be established, the call is exactly as if
    //      ISipServerEventControl::SendResponse was called instead. In this
    //      case, the caller receives resSW_DIALOG_NOT_REGISTERED as the
    //      return value.
    //
    //  See also:
    //      ISipServerEventControl, ISipUserAgentSvc
    //
    //==EDOC====================================================================
    virtual mxt_result SendDialogEstablishingResponse(IN unsigned int uCode,
                                                      IN const char* pszReason,
                                                      IN TO CHeaderList* pExtraHeaders,
                                                      IN TO CSipMessageBody* pMessageBody) = 0;

//-- Hidden Methods
//------------------
protected:
    //-- << Protected Constructors / Destructors / Operators >>
    //----------------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipDialogServerEventCtrl() {};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipDialogServerEventCtrl() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipDialogServerEventCtrl(IN const ISipDialogServerEventCtrl& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipDialogServerEventCtrl& operator=(IN const ISipDialogServerEventCtrl& rSrc);

//-- Hidden Data Members
//-----------------------
protected:
private:
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPDIALOGSERVEREVENTCTRL_H

