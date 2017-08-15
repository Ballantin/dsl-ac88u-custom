//==============================================================================
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
//==============================================================================
#ifndef MXG_ISCEOODREQUESTSENDERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEOODREQUESTSENDERMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipPacket;
class ISceOodRequestSender;
class ISceUserConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ClassName
//==============================================================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   Manager for the ISceOodRequestSender component.
//
// Description:
//   This is the interface through which the ISceOodRequestSender reports
//   the responses received.
//
// Location:
//   SceCoreComponents/ISceOodRequestSenderMgr.h
//
// See Also:
//   ISceOodRequestSender
//
//==============================================================================
class ISceOodRequestSenderMgr
{
//-- Published Interface
public:

    //==========================================================================
    //==
    //==  EvOodResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager that a response for a request it sent was
    //      received.
    //
    //  Parameters:
    //    pComponent:
    //      The component on which the request was sent.
    //
    //    pUserConfig:
    //      The user configuration used to send the request. Cannot be NULL.
    //
    //    opqTransaction:
    //      The opaque identifier that the application passed to SendOodRequest.
    //
    //    rResponse:
    //      The received response.
    //
    //  Description:
    //      This method is called by a component when a response for a request
    //      sent through ISceOodRequestSender::SendOodRequest is
    //      received.
    //
    //      If the response in parameter is provisional (< 200), the manager
    //      must be prepared to receive this event again for the same request
    //      (that is with the same opqTransaction parameter).
    //
    //      If the response in parameter is final (>= 200), the manager will not
    //      receive this event again for this opqTransaction.
    //
    //  See Also:
    //      ISceBasicExtensionControl
    //
    //==========================================================================
    virtual void EvOodResponse(IN ISceOodRequestSender* pComponent,
                               IN ISceUserConfig* pUserConfig,
                               IN mxt_opaque opqTransaction,
                               IN const CSipPacket& rResponse) = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceOodRequestSenderMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISceOodRequestSenderMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceOodRequestSenderMgr(IN const ISceOodRequestSenderMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceOodRequestSenderMgr& operator=(IN const ISceOodRequestSenderMgr& rSrc);


//-- Hidden Data Members
protected:
private:
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEOODREQUESTSENDERMGR_H


