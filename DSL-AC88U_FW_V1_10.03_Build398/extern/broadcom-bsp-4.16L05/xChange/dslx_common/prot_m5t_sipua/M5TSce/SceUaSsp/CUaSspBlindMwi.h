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
#ifndef MXG_CUASSPBLINDMWI_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUASSPBLINDMWI_H

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

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_IUASSPMWI_H
#include "SceUaSsp/IUaSspMwi.h"
#endif

#ifndef MXG_IUASSPMWIMGR_H
#include "SceUaSsp/IUaSspMwiMgr.h" // IUaSspMwiMgr::EStatus
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CMessageSummary;
class ISipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaSspBlindMwi
//========================================
//
// Description:
//   This class implements the unsolicited MWI handling. It waits for
//  unsolicited NOTIFY to arrive and reports them to its manager.
//
// #Notes:#
//   This class does not overload GetMainContext(). This method cannot be used
//   to send request through the ISceBasicExtensionControl since this object
//   does not create a dialog. Instead, the application can use
//   ISceOodRequestSender to do the same.
//
// Location:
//   SceUaSsp/CUaSspBlindMwi.h
//
//==============================================================================
class CUaSspBlindMwi : private CSceBaseComponent,
                       private IUaSspMwi
{
//-- Friend Declarations.
    friend class CSceUaSspInitializer;

//-- Published Interface.
public:

//-- Hidden Methods.
protected:
private:
    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaSspBlindMwi();
    static void FinalizeCUaSspBlindMwi();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaSspBlindMwi(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CUaSspBlindMwi();

    //-- << CSceBaseComponent >>

    virtual uint32_t GetSupportedServicesBitset() const;

    //-- << CEComDelegatingUnknown >>

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << IUaSspMwi >>

    virtual mxt_result SetManager(IN IUaSspMwiMgr* pMgr);
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);
    virtual void SetOpaque(IN mxt_opaque opq);
    virtual mxt_opaque GetOpaque();
    virtual mxt_result Subscribe(IN TO CSceCallerPreferences* pCallerPreferences);
    virtual mxt_result Unsubscribe();
    virtual void Terminate();
    virtual mxt_result HandleIncomingNotify(IN const CSipPacket& rRequest);

    //-- << ISipGenericMgr >>

    virtual void EvRequest(IN ISipGenericSvc* pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket& rRequest);

    virtual void EvProgress(IN ISipGenericSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipGenericSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipGenericSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    //-- << Helper methods >>

    // Summary:
    //  Parses the message-summary into the NOTIFY request and takes the action
    //  related to the result.
    mxt_result GetMessageSummaryHelper(IN const CSipPacket& rNotify,
                                       IN ISipServerEventControl* pServerEventControl,
                                       OUT GO CMessageSummary*& rpMessageSummary);

    // Summary:
    // Create and configure a new context.
    mxt_result CreateAndConfigNewContext();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaSspBlindMwi(IN const CUaSspBlindMwi& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaSspBlindMwi& operator=(IN const CUaSspBlindMwi& rSrc);


//-- Hidden Data Members.
protected:
private:

    // The manager to report the events to.
    IUaSspMwiMgr* m_pMgr;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CUASSPBLINDMWI_H

