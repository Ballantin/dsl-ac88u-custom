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
#ifndef MXG_CSCEOODREQUESTSENDER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEOODREQUESTSENDER_H

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

#ifndef MXG_ISCEOODREQUESTSENDER_H
#include "SceCoreComponents/ISceOodRequestSender.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSceCallerPreferences;
class ISipClientTransaction;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceOodRequestSender
//==============================================================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   Implementation of the ISceOodRequestSender ECom interface.
//
// Description:
//   This component is the implementation of the ISceOodRequestSender
//   ECom interface.
//
// Location:
//   SceCoreComponents/CSceOodRequestSender.h
//
// See Also:
//   ISceOodRequestSender, CSceBaseComponent
//
//==============================================================================
class CSceOodRequestSender : private CSceBaseComponent,
                             private ISceOodRequestSender
{
//-- Friend Declarations.
    friend class CSceCoreComponentsInitializer;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceOodRequestSender(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CSceOodRequestSender();

    //-- << CSceBaseComponent interface >>

    virtual void ReceiveResponseHelper(IN ISipClientEventControl* pClientEventCtrl,
                                       IN const CSipPacket& rResponse);

    virtual void TransactionChanged(IN ISipClientTransaction* pOldTransaction,
                                    IN ISipClientTransaction* pNewTransaction);

    virtual void OnUserAgentServiceConfigured(IN mxt_result res,
                                              IN TO CTelUri* pTelUri,
                                              IN TO CList<CSipUri>* plstSipUris);
    
    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN


    //-- << CEComUnknown methods >>

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << ISceOodRequestSender interface >>

    virtual mxt_result SetOodRequestSenderManager(IN ISceOodRequestSenderMgr* pMgr);

    virtual mxt_result SetConfig(IN ISceUserConfig* pConfig);

    virtual void GetConfig(OUT ISceUserConfig*& rpConfig);

    virtual void SetOpaque(IN mxt_opaque opq);

    virtual mxt_opaque GetOpaque();

    virtual mxt_result SetPeerAddr(IN const CNameAddr& rPeerAddr);

// BRCM_CUSTOM - [add] Add force destination cfg support
    virtual mxt_result SetTargetAddr(IN const CNameAddr& tTargetAddr );
// BRCM_CUSTOM - [end] Add force destination cfg support

    virtual mxt_result SendOodRequest(IN ESipMethodType eMethodType,
                                      IN mxt_opaque opqTransaction,
                                      IN TO CHeaderList* pExtraHeaders,
                                      IN TO CSipMessageBody* pMessageBody,
                                      IN TO CSceCallerPreferences* pCallerPreferences);


private:
    //-- << Initialization mechanism >>

    static mxt_result InitializeCSceOodRequestSender();

    static void FinalizeCSceOodRequestSender();

    // Summary:
    // Create and configure a new context.
    mxt_result CreateNewContext();


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceOodRequestSender(IN const CSceOodRequestSender& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceOodRequestSender& operator=(IN const CSceOodRequestSender& rSrc);


//-- Hidden Data Members
protected:
private:
    ISceOodRequestSenderMgr* m_pMgr;

    CList<ISipClientTransaction*> m_lstpOodTransactions;

    bool m_bWaitOnUserAgentConfigurationToSendRequest;

    // Peer address 
    CNameAddr* m_pPeerAddr;

    CMarshaler m_delayedRequestParameters;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCEOODREQUESTSENDER_H

