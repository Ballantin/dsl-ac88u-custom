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
#ifndef MXG_CUASSPMWI_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUASSPMWI_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
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

#ifndef MXG_ISCESUBSCRIBERMGR_H
#include "SceCoreComponents/ISceSubscriberMgr.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSceCallerPreferences;
class CBlob;
class CMessageSummary;
class CSipPacket;
class ISceSubscriber;
class ISceUserConfig;
class IUaSspMwiConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaSspMwi
//========================================
//
// Description:
//   This class implements the MWI handling. It uses the ISceSubscriber
//   component to subscribe to the message-summary event. Upon reception of a
//   NOTIFY for this event, it notifies its manager.
//
// Location:
//   SceUaSsp/CUaSspMwi.h
//
// See also:
//   SceCoreComponents/ISceSubscriber.h
//
//==============================================================================
class CUaSspMwi : private CEComDelegatingUnknown,
                  private IUaSspMwi,
                  private ISceSubscriberMgr
{
//-- Friend Declarations.
    friend class CSceUaSspInitializer;

//-- New types within class namespace.
public:
protected:
private:

//-- Published Interface.
public:

//-- Hidden Methods.
protected:
private:
    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaSspMwi();
    static void FinalizeCUaSspMwi();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaSspMwi(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CUaSspMwi();

    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

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

    //-- << ISceSubscriberMgr >>
    virtual void EvSubscriptionStatus(IN ISceSubscriber* pSubscriber,
                                      IN ESubscriptionStatus eStatus);

    virtual void EvResourceStateUpdate(IN ISceSubscriber* pSubscriber,
                                       IN ISipServerEventControl* pNotifyServerEventCtrl,
                                       IN const CSipMessageBody& rPayload,
                                       IN bool bReceivedFinalNotify);

    virtual void EvResourceListUpdateDone();

    virtual void EvWatcherInfo(IN ISceSubscriber* pSubscriber,
                               IN const SWatcherInfo& rWatcherInfo,
                               IN bool bFlushWatcherInfoTable);

    virtual void EvWatcherInfoUpdateDone();

    virtual void EvResourceListUpdate(IN const char* pszResourceListUri,
                                      IN CVector<CXmlElement*>& rvpListDisplayName,
                                      IN bool bFullState);

    virtual void EvResourceListItemUpdate(IN ISceSubscriber* pSubscriber,
                                          IN const SResourceListResourceInfo* pResourceInfo);

    //-- << Helper methods >>

    // Summary:
    //  Parses the message-summary into the payload and notify the
    //  manager if necessary.
    mxt_result ReportEvMailboxStatusHelper(IN const CBlob* pMessage);


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaSspMwi(IN const CUaSspMwi& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaSspMwi& operator=(IN const CUaSspMwi& rSrc);


//-- Hidden Data Members.
protected:
private:

    // The manager to report the events to.
    IUaSspMwiMgr* m_pMgr;

    // The MWI configuration.
    IUaSspMwiConfig* m_pMwiConfig;

    // The subscriber component used.
    IEComUnknown* m_pSubscriberIEComUnknown;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CUASSPMWI_H

