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
#ifndef MXG_CSCEDEFAULTHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEDEFAULTHANDLER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSCEBASECOMPONENT_H
#include "SceCoreComponents/CSceBaseComponent.h"
#endif

#ifndef MXG_ISCEREQUESTHANDLER_H
#include "SceCore/ISceRequestHandler.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceDefaultHandler
//========================================
//
// Description:
//  This default handler is always registered as the first handler. It is thus
//  called last in case no other handler wanted to handle the incoming request.
//
//  It sends negative final responses to incoming requests.
//
//  If it sees a request without any user configuration object associated to it,
//  it sends a 404 response to the request, which informs the other peer that
//  the user was not found.
//
//  If it sees a request with a user configuration object associated to it, it
//  sends a 501 response which inform that the method is not supported.
//
//  This handler is particular because it is also a component. Usually, handlers
//  dispatch packets to either an existing component or create a new one for it.
//  However, since this handler is intended to only reject unsupported requests
//  or requests for unsupported users, it does not need to create state in a
//  component and can always reuse the same one. But since usually a component
//  is configured with a single user, this class also needed to implement the
//  component to override that behavior and enable the same component being used
//  for multiple users.
//
//  This class is the implementation of a component and thus is an ECOM object.
//  This means that object of this class are never directly accessed. They are
//  accessed through the ECOM interface that they implement. For this object it
//  is the ISceRequestHandler and the ISceBasicExtensionControl. Further more,
//  object of this classe are always automatically created by the SIP Client
//  Engine and thus the application never needs to create objects of this class.
//
// Location:
//   SceCoreComponents/CSceDefaultHandler.h
//
// See Also:
//   ISceRequesthandler
//
//==============================================================================
class CSceDefaultHandler : private CSceBaseComponent,
                           private ISceRequestHandler
{
//-- Friend Declarations.
    friend class CSceCoreComponentsInitializer;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceDefaultHandler(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceDefaultHandler();


    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN


    //-- << CEComDelegatingUnknown Interface >>
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << CSceBaseComponent Override >>

    virtual uint32_t GetSupportedServicesBitset() const;

    virtual void UnhandledRequestHelper(IN ISipServerEventControl* pServerEventCtrl,
                                        IN const CSipPacket& rRequest);

    virtual void OnUserAgentServiceConfigured(IN mxt_result res,
                                              IN TO CTelUri* pTelUri,
                                              IN TO CList<CSipUri>* plstSipUris);

    //   << ISceRequestHandler Interface >>
    virtual mxt_result HandleRequest(IN const CSipPacket& rRequest,
                                     IN IEComUnknown* pUserConfig);


private:
    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Initialization mechanism >>

    static mxt_result InitializeCSceDefaultHandler();

    static void FinalizeCSceDefaultHandler();

    //-- << Startup mechanism >>

    static mxt_result Startup();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceDefaultHandler(IN const CSceDefaultHandler& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceDefaultHandler& operator=(IN const CSceDefaultHandler& rSrc);


//-- Hidden Data Members.
protected:
private:
    // This data member is used only to pass the user received in HandleRequest
    // to UnhandledRequestHelper. It is always reset before
    // UnhandledRequestHelper exits.
    ISceUserConfig* m_pCurrentUserConfig;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCEDEFAULTHANDLER_H

