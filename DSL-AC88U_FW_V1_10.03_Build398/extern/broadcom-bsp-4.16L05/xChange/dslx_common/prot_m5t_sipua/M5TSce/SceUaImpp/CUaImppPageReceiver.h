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
#ifndef MXG_CUAIMPPPAGERECEIVER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPPAGERECEIVER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSCEBASECOMPONENT_H
#include "SceCoreComponents/CSceBaseComponent.h"
#endif

#ifndef MXG_IUAIMPPPAGERECEIVER_H
#include "SceUaImpp/IUaImppPageReceiver.h"
#endif



MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSipPacket;
class IUaImppPageReceiverMgr;

//==============================================================================
//== Class: CUaImppPageReceiver
//========================================
//
// Description:
//   This class implements the Page Receiver. It waits for MESSAGE requests to
//   arrive and reports them to its manager.
//
// Location:
//   SceUaImpp/CUaImppPageReceiver.h
//
//
//==============================================================================
class CUaImppPageReceiver : private CSceBaseComponent,
                            private IUaImppPageReceiver
{
//-- Friend Declarations.
    friend class CSceUaImppInitializer;


//-- Published Interface.
public:

//-- Hidden Methods.
protected:
private:

    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaImppPageReceiver();
    static void FinalizeCUaImppPageReceiver();


    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- << ISipGenericMgr Implementation >>
    virtual void EvRequest(IN ISipGenericSvc*         pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket&       rRequest);


    //-- << IUaImppPageReceiver >>

    virtual mxt_result SetManager(IN IUaImppPageReceiverMgr* pMgr);
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);
    virtual void SetOpaque(IN mxt_opaque opq);
    virtual mxt_opaque GetOpaque();
    virtual mxt_result SendResponse(IN mxt_opaque opqTransaction,
                                    IN unsigned int uCode,
                                    IN const char* szReason);
    virtual mxt_result HandleIncomingMessage(IN const CSipPacket& rRequest);

    // Summary:
    //  Default Constructor.
    CUaImppPageReceiver(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CUaImppPageReceiver();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaImppPageReceiver(IN const CUaImppPageReceiver& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaImppPageReceiver& operator=(IN const CUaImppPageReceiver& rSrc);
    
    // Summary:
    // Create and configure a new context
    mxt_result CreateNewContext();


//-- Hidden Data Members.
protected:
private:

    // The manager to report the events to.
    IUaImppPageReceiverMgr* m_pMgr;
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CUAIMPPPAGERECEIVER_H

//M5T_INTERNAL_USE_END
