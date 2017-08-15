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
#ifndef MXG_CUAIMPPPRESENCESUBSCRIBER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPPRESENCESUBSCRIBER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCESUBSCRIBER_H
#include "SceCoreComponents/ISceSubscriber.h"
#endif

#ifndef MXG_ISCESUBSCRIBERMGR_H
#include "SceCoreComponents/ISceSubscriberMgr.h"
#endif

#ifndef MXG_IUAIMPPPRESENCESUBSCRIBER_H
#include "SceUaImpp/IUaImppPresenceSubscriber.h"
#endif

#ifndef MXG_IPIDFDOCUMENT_H
#include "SceXml/IScePidfDocument.h"
#endif


#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSceXmlPatchOperations;
class ISceUserConfig;
class IUaImppPresenceSubscriberMgr;
class IScePidfDocument;
class IXmlDocument;

//==============================================================================
//== Class: CUaImppPresenceSubscriber
//==============================================================================
//<GROUP PackageName>
//
// Summary:
//   This class is used to subscribe to the presence of a peer.
//
// Description:
//   This class implements the Presence Subscriber. It is used to subscribe to
//   the presence of a peer.
//
// Location:
//   SceUaImpp/CUaImppPresenceSubscriber.h
//
// See Also:
//   IUaImppPresenceSubscriber
//
//==============================================================================
class CUaImppPresenceSubscriber : private CEComDelegatingUnknown,
                                  private IUaImppPresenceSubscriber,
                                  private ISceSubscriberMgr

{
//-- Friend Declarations
    friend class CSceUaImppInitializer;

//-- Published Interface.
public:

//-- Hidden Methods.
protected:
private:

    //-- << Initialization mechanism >>

    static mxt_result InitializeCUaImppPresenceSubscriber();
    static void FinalizeCUaImppPresenceSubscriber();

    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComDelegatingUnknown >>

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result InitializeInstance();

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- << IUaImppPresenceSubscriber >>

    virtual mxt_result SetManager(IN IUaImppPresenceSubscriberMgr* pMgr);
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);
    virtual void SetOpaque(IN mxt_opaque opq);
    virtual mxt_opaque GetOpaque();
    virtual mxt_result Subscribe(IN TOA CNameAddr* pResourceAor,
                                 IN TOA CSipMessageBody* pPayload,
                                 IN TOA CSceCallerPreferences* pCallerPreferences);
    virtual mxt_result Refresh(IN TOA CSipMessageBody* pPayload);
    virtual mxt_result Unsubscribe();

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
                                      IN CVector<CXmlElement*>& rvpListDisplayName,
                                      IN bool bFullState);
    virtual void EvResourceListUpdateDone();
    virtual void EvResourceListItemUpdate(IN ISceSubscriber* pSubscriber,
                                          IN const SResourceListResourceInfo* pResourceInfo);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaImppPresenceSubscriber(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CUaImppPresenceSubscriber();

private:

    // Summary:
    //  Enumeration used to specify if the subscription URI corresponds to a single
    //  resource or to a resource list.
    enum ESubscriptionType
    {
        // Description:
        //  No notification has been received yet, the type of subscription
        //  remains undefined.
        eSUBSCRIPTION_TYPE_NOT_DEFINED,

        // Description:
        //  Indicates that the subscription URI corresponds to a single resource.
        eSUBSCRITION_SINGLE_RESOURCE,

        // Description:
        //  Indicates that the subscription URI corresponds to a resource list.
        eSUBSCRITION_RESOURCE_LIST
    };

    // Summary:
    //  This enumeration allows to specify the type of document contained in
    //  a SIP payload body.
    //   Note: In the case of a subscription to a resource list, the expected
    //    document type is "application/rlmi+xml", sometimes, this document
    //    can be included inside a MIME-Multipart payload.
    //    The "rlmi" documents are managed by the Subscriber component and for
    //    this reason it not included in this enumeration.
    enum EDocumentType
    {
        eDOCUMENT_UNKNOWN        = 0,
        eDOCUMENT_PIDF_XML,
        eDOCUMENT_PIDF_DIFF_XML,
        eDOCUMENT_PIDF_FULL_XML,
    };

    // Summary:
    //  This enumeration defines the tags that can be associated to presence
    //  information items. These tags allow to control the Subscriber behavior.
    enum EPresenceInfoTag
    {
        eITEM_NEW,
        eITEM_UPDATED,
        eITEM_TO_BE_UPDATED,
        eITEM_TO_BE_DELETED
    };

    // Summary:
    //  This structure is used to hold the basic subscription list information.
    struct SListSubscription
    {
          CString m_strResourceListUri;
          CString m_strResourceListName;
          EPresenceInfoTag m_eListItemTag;

          SListSubscription()
              : m_strResourceListUri(),
                m_strResourceListName(),
                m_eListItemTag(eITEM_NEW)
          {
          }
          ~SListSubscription()
          {
          }
    };

    // This is needed to be able to compile with MSVC6 in which it is not
    // possible for a struct declared in a class to access the enumeration
    // values that are declared as private or protected in the same class.
    friend struct SListSubscription;

    // Summary:
    //  This structure holds the information associated to each resource defined
    //  in a received rlmi list. Each instance can represent a single resource item
    //  or an (embedded) resource list. The pointer m_psEmbeddedListSubscription
    //  can be used to distinguish both cases: it will be non NULL only when the
    //  the represented item is a resource list.
    struct SPresenceInfo
    {
        // Description:
        //  Pointer to the associated pidf document, it remains NULL when the
        //  instance respresents a resource list.
        IScePidfDocument*      m_pPidfDocument;

        // Description:
        //  The resource list where this resource is defined.
        SListSubscription*  m_psListSubscription;

        // Description:
        //  The resource list defined by this resource. It is used only when the resourse
        //  is a resource list, otherwise it remains NULL
        SListSubscription*  m_psEmbeddedListSubscription;

        // Description:
        //  Defines the expected version of a pidf-diff document.
        unsigned int        m_uExpectedPartialPidfVersion;

        // Description:
        //  Resource URI.
        CString             m_strResourceUri;

        // Description:
        //  Resource name.
        CString             m_strResourceName;

        // Description:
        //  Subscription status ("active", "pending" or "terminate") of the resource.
        ESubscriptionStatus m_eSubscriptionStatus;

        // Description:
        //  Resource Tag.
        EPresenceInfoTag    m_eItemTag;

        SPresenceInfo()
            : m_pPidfDocument(NULL),
              m_psListSubscription(NULL),
              m_psEmbeddedListSubscription(NULL),
              m_uExpectedPartialPidfVersion(1),
              m_strResourceUri(),
              m_strResourceName(),
              m_eSubscriptionStatus(ISceSubscriberMgr::eUNSUBSCRIBED),
              m_eItemTag(eITEM_NEW)
        {
        }

        ~SPresenceInfo()
        {
            if (m_pPidfDocument != NULL)
            {
                m_pPidfDocument->ReleaseIfRef();
            }
        }
    };

    // This is needed to be able to compile with MSVC6 in which it is not
    // possible for a struct declared in a class to access the enumeration
    // values that are declared as private or protected in the same class.
    friend struct SPresenceInfo;

//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaImppPresenceSubscriber(IN const CUaImppPresenceSubscriber& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaImppPresenceSubscriber& operator=(IN const CUaImppPresenceSubscriber& rSrc);

    //-- << Helper methods >>

    // Summary:
    //  Parse a Pidf or Pidf-full document.
    mxt_result ParsePidfDocument(IN const CSipMessageBody& rPayload,
                                 OUT IScePidfDocument*& rpPidfDocument);

    // Summary:
    //  Parse a Pidf-diff document.
    mxt_result ParsePartialPidfDocument(IN const CSipMessageBody& rPayload,
                                        INOUT SPresenceInfo*& rpPresenceInformationItem);

    // Summary:
    //  Looks for a given subscription list into the subscription list vector.
    SListSubscription* SearchSubscriptionList(IN const char* pszResourceListUri,
                                              INOUT unsigned* puIndex = NULL);

    // Summary:
    //  Adds a new item to the subcription list vector.
    SListSubscription* AddSubscriptionList(IN const char* pszResourceListUri,
                                           IN const char* pszResourceListName);

    // Summary:
    //  Removes all the items from the vector of subscription lists.
    void RemoveAllSubscriptionLists();

    // Summary:
    //  Removes the specified list from the vector of resource lists.
    mxt_result RemoveSubscriptionList(IN const char* pszResourceListUri);

    // Summary:
    //  Adds a new presence information item to the list.
    mxt_result AddPresenceInfoItem(IN const SResourceListResourceInfo*& rpResourceInfo,
                                   INOUT SPresenceInfo*& rpPresenceInfoItem);

    // Summary:
    //  Inserts the presence information item in the list.
    mxt_result InsertPresenceInfoItem(INOUT SPresenceInfo*& rpsPresenceInfoItem);

    //  Summary:
    //      Removes all the items from the subscription list vector.
    void RemoveAllPresenceInfoItems();

    // Summary:
    //  Updates a presence information item.
    mxt_result UpdatePresenceInfoItem(IN const SResourceListResourceInfo*& rpResourceInfo,
                                      INOUT SPresenceInfo*& rpsPresenceInfoItem);


    // Summary:
    //  Searches a presence information item in the list.
    SPresenceInfo* SearchPresenceInfoItem(IN const char* pszResourceUri,
                                          IN const SListSubscription* pSubscriptionList = NULL,
                                          INOUT unsigned* puIndex = NULL);
    // Summary:
    //  Searches a presence information item that has the specified tag.
    SPresenceInfo* SearchPresenceInfoItemTag(IN const EPresenceInfoTag eItemTag,
                                             IN const SListSubscription* pSubscriptionList,
                                             INOUT unsigned* puIndex);

    // Summary:
    //  Removes the presence information items whose status is "terminated"
    //  or whose tags are eITEM_TO_BE_UPDATED.
    void CleanUpPresenceInfoList();

    // Summary:
    //  Sets the m_eItemTag member of the presence information items wiht eItemTag.
    void SetPresenceInfoItemTags(IN const EPresenceInfoTag eItemTag,
                                 IN const SListSubscription* pSubscriptionList = NULL);
    // Summary:
    //  Determines the document type that is in the specified payload.
    EDocumentType GetDocumentType(IN const CSipMessageBody& rPayload);

    // Summary:
    //  Returns the payload of the active instance of a resource.
    const CSipMessageBody* GetInstancePayload(IN const SResourceListResourceInfo*& rpResourceInfo,
                                              IN SPresenceInfo*& rpsPresenceInfoItem);

    // Summary:
    //  Verify the pidf document version.
    mxt_result VerifyPidfDocumentVersion(INOUT SPresenceInfo*& rpPresenceInformationItem,
                                         IN CUaImppPresenceSubscriber::EDocumentType eDocumentType,
                                         IN IXmlDocument*& rpPidfDocument);
    // Summary:
    //  Process a received generic pidf document.
    mxt_result ProcessPidfPayload(IN const CSipMessageBody& rPayload,
                                  INOUT SPresenceInfo*& rpPresenceInformationItem);


//-- Hidden Data Members
protected:
private:

    // The aggregated object.
    IEComUnknown* m_pSubscriberUnknown;

    // The ISceSubscriber interface of the aggregated object.
    ISceSubscriber* m_pSubscriber;

    // The manager to report the events to.
    IUaImppPresenceSubscriberMgr* m_pMgr;

    // The type of subscription (to a single resource or to a
    // resource list).
    ESubscriptionType m_TypeOfSubscription;

    // This is the subscription list vector that holds the very basic information
    // about the subscription lists. The first entry always corresponds to
    // the rlmi list contained in the root multipar/related MIME body.
    // The  additional entries corresponds to the embedded lists that are
    // referenced by the root list.
    CVector<SListSubscription*>  m_vecSubscriptionList;

    // This is the list which holds the presence information for all the resources.
    // Each entry in the list may represent a single resource or a reference
    // to an embedded resource list.
    CVector<SPresenceInfo*> m_vecpsPresenceInfo;

    // The patchOperation object acting as the PatchMgr of the document. The
    // instance of CSceXmlPatchOperations is created on demand, when the first
    // pidf-diff document is received.
    CSceXmlPatchOperations* m_pXmlPatchOperation;

    // Pointer to the structure describing the subscription list that
    // is currently processed. It is set to NULL if no list is being processed.
    // This pointer is set when EvResourceListUpdate is called, and it
    // is reset to NULL when all the list items have been processed.
    SListSubscription* m_psCurrentlyProcessedSubscriptionList;

    // Indicates if the subscription list that is currently processed
    // holds a full state presence information or not. If  the pointer
    // m_psCurrentlyProcessedSubscriptionList is NULL, the value of this flag
    // remains meaningless.
    bool m_bCurrentProcessedListFullState;

    // Indicates that a refresh NOTIFY is required. This flag is usually set when a
    // version problem is detected working with pidf-diff document.
    bool m_bRefreshRequired;
};

MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CUAIMPPPRESENCESUBSCRIBER_H

//M5T_INTERNAL_USE_END
