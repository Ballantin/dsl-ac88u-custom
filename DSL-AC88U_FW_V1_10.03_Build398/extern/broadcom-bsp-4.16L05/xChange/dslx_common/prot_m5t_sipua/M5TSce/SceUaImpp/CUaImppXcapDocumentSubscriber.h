//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CUAIMPPXCAPDOCUMENTSUBSCRIBER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPXCAPDOCUMENTSUBSCRIBER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCEUAIMPPCFG_H
#include "Config/SceUaImppCfg.h"
#endif

#if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCESUBSCRIBERMGR_H
#include "SceCoreComponents/ISceSubscriberMgr.h"
#endif

#ifndef MXG_IUAIMPPXCAPDOCUMENTSUBSCRIBER_H
#include "SceUaImpp/IUaImppXcapDocumentSubscriber.h"
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
//== Class: CUaImppXcapDocumentSubscriber
//==============================================================================
//
// Description:
//   This class implements the IUaImppXcapDocumentSubscriber interface. It is
//   used to monitor changes on selected resources using the event mechanism
//   (SUBSCRIBE and NOTIFY requests).
//
// Location:
//   SceUaImpp/CUaImppXcapDocumentSubscriber.h
//
// See Also:
//   IUaImppXcapDocumentSubscriber
//
//==============================================================================
class CUaImppXcapDocumentSubscriber : protected CEComDelegatingUnknown,
                                      protected ISceSubscriberMgr,
                                      protected IUaImppXcapDocumentSubscriber
{
//-- Friend Declarations
    friend class CSceUaImppInitializer;

//-- New types within class namespace
public:
protected:

    struct SMonitoringInfo
    {
        // The URI of the document to monitor. Must be an XCAP URI to the
        // document or to an element within a document. It can either be an
        // absolute URI or a relative URI.
        const char* m_pszDocUri;

        // The manager insterested in the state of m_pszDocUri.
        IUaImppXcapDocumentSubscriberMgr* m_pMgr;

        SMonitoringInfo()
        :   m_pszDocUri(NULL),
            m_pMgr(NULL)
        {}
    };


private:

//-- Published Interface
public:

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaImppXcapDocumentSubscriber(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CUaImppXcapDocumentSubscriber();


    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaImppXcapDocumentSubscriber();

    static void FinalizeCUaImppXcapDocumentSubscriber();


    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << ISceSubscriberMgr >>
    virtual void EvSubscriptionStatus(IN ISceSubscriber* pSubscriber,
                                      IN ESubscriptionStatus eStatus);

    virtual void EvResourceStateUpdate(IN ISceSubscriber* pSubscriber,
                                       IN ISipServerEventControl* pNotifyServerEventCtrl,
                                       IN const CSipMessageBody& rPayload,
                                       IN bool bReceivedFinalNotify);

    virtual void EvWatcherInfo(IN ISceSubscriber* pSubscriber,
                               IN const SWatcherInfo& rWatcherInfo,
                               IN bool bFlushWatcherInfoTable);

    virtual void EvWatcherInfoUpdateDone();

    virtual void EvResourceListUpdate(IN const char* pszResourceListUri,
                                      IN CVector<CXmlElement*>& rvecpListDisplayName,
                                      IN bool bFullState);

    virtual void EvResourceListUpdateDone();

    virtual void EvResourceListItemUpdate(IN ISceSubscriber* pSubscriber,
                                          IN const SResourceListResourceInfo* pResourceInfo);


    //-- << IUaImppXcapDocumentSubscriber >>
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);

    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);

    virtual void SetOpaque(IN mxt_opaque opq);

    virtual mxt_opaque GetOpaque();

    virtual ISceSubscriberMgr::ESubscriptionStatus GetSubscriptionState();

    virtual mxt_result SetNotifier(IN TOA CNameAddr* pResourceAor,
                                   IN TOA IUri* pNotifierRequestUri);

    virtual mxt_result MonitorDocument(IN const char* pszDocUri,
                                       IN IUaImppXcapDocumentSubscriberMgr* pMgr);

    virtual mxt_result UnmonitorDocument(IN const char* pszDocUri);


    //-- << Helper methods >>

    // Summary:
    //  Creates a ISceSubscriber object.
    mxt_result CreateSubscriber();

    // Summary:
    //  Generates a SUBSCRIBE "xcap-diff" payload from the list in parameter.
    static GO CSipMessageBody* GeneratePayload(IN const CVector<SMonitoringInfo*>& rvecpstMonitoredDoc);


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaImppXcapDocumentSubscriber(IN const CUaImppXcapDocumentSubscriber& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaImppXcapDocumentSubscriber& operator=(IN const CUaImppXcapDocumentSubscriber& rSrc);


//-- Hidden Data Members
protected:
private:

    // The ISceUserConfig associated with this object.
    ISceUserConfig* m_pUserConfig;

    // MXI_DESIGN_NOTES (2010-03-30): Remove this when 25271 will be fixed.
    // The opaque for this instance.
    mxt_opaque m_opq;

    // The aggregated ISceSubscriber instance.
    IEComUnknown* m_pSubscriber;

    // The address of the resource to which the component must subscribe.
    // Will be used in the To header. NULL means default
    CNameAddr* m_pResourceAor;

    // The address of the resource to which the component must subscribe.
    // Will be used in the request-URI. Can be NULL, in which case the URI
    // found in m_pResourceAor will be used instead. This member is ignored
    // when m_pResourceAor is NULL.
    IUri* m_pNotifierRequestUri;

    // This is the list of monitored documents.
    CVector<SMonitoringInfo*>* m_pvecpstMonitoredDoc;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_SCE_ENABLE_XCAP_SUPPORT)


#endif //-- #ifndef MXG_CUAIMPPXCAPDOCUMENTSUBSCRIBER_H

//M5T_INTERNAL_USE_END
