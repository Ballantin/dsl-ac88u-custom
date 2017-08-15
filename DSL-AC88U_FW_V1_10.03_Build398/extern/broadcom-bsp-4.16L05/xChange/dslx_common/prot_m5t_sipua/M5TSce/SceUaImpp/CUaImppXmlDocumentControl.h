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
#ifndef MXG_CUAIMPPXMLDOCUMENTCONTROL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPXMLDOCUMENTCONTROL_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCEUAIMPPCFG_H
#include "Config/SceUaImppCfg.h"
#endif

#if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h" // base class
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_IPRIVATESCECHALLENGESOURCE_H
#include "SceCoreComponents/IPrivateSceChallengeSource.h" // implemented interface
#endif

#ifndef MXG_ISCEUSERCONFIG_H
#include "SceCoreComponents/ISceUserConfig.h" // inline usage
#endif

#ifndef MXG_IUAIMPPXCAPDOCUMENTSUBSCRIBERMGR_H
#include "SceUaImpp/IUaImppXcapDocumentSubscriberMgr.h" // implemented interface
#endif

#ifndef MXG_IUAIMPPXMLDOCUMENTCONTROL_H
#include "SceUaImpp/IUaImppXmlDocumentControl.h" // implemented interface
#endif

#ifndef MXG_IXCAPREQUESTMGR_H
#include "Xcap/IXcapRequestMgr.h" // implemented interface
#endif

#ifndef MXG_IXMLDOCUMENTMGR_H
#include "Xml/IXmlDocumentMgr.h" // implemented interface
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h" // Member
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h" // Member
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSceXcapPatchOperations;
class CSubAllocator;
class ISceXmlDocument;
class IXcapClient;
class IUaImppXcapDocumentSubscriber;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaImppXmlDocumentControl
//==============================================================================
//
// Description:
//   This class implements the IUaImppXmlDocumentControl interface. It is
//   used to access and synchronize a local and remote XML document.
//
// Location:
//   SceUaImppXml/CUaImppXmlDocumentControl.h
//
//==============================================================================
class CUaImppXmlDocumentControl : protected CEComDelegatingUnknown,
                                  protected CEventDriven,
                                  protected IUaImppXmlDocumentControl,
                                  protected IXcapRequestMgr,
                                  protected IXmlDocumentMgr,
                                  protected IUaImppXcapDocumentSubscriberMgr,
                                  protected IPrivateSceChallengeSource
{
//-- Friend Declarations.
    friend class CUaImppXmlDocumentControlFactory;

//-- Public Custom Types.
public:
private:

    // Summary:
    //  This enumeration defines the synchronization method IDs that are used
    //  with the Xcap client's Get and Put operation.
    //
    // Description:
    //  This enumeration defines the synchronization method IDs that are used
    //  with the Xcap client's Get and Put operation.
    enum ESyncMethodIds
    {
        // Description:
        //  A Get operation was used to synchronize the states.
        eSYNCHRONIZE_STATE,

        // Description:
        //  A Get operation was used to synchronize the documents.
        eSYNCHRONIZE_FROM_SERVER,

        // Description:
        //  A Put operation was used to synchronize the documents.
        eSYNCHRONIZE_TO_SERVER,

        // Description:
        //  A Get operation was used to preview the server document.
        eSYNCHRONIZE_PREVIEW,

        // Description:
        //  A Delete operation was used to delete the server document.
        eSYNCHRONIZE_DELETE,
    };


    // Summary:
    //  This is an internal struct used for the XCAP mechanism.
    //
    // Description:
    //  This is an internal struct used for the XCAP mechanism.
    struct SXCapOperationData
    {
        // Description:
        // The method used.
        ESyncMethodIds m_eMethod;

        // Description:
        // The condition given. Empty if none.
        CString* m_pstrCondition;

        // Description:
        // Tells if the operation used XCAP patches or not.
        bool m_bUsingPatches;

        // Description:
        // The number of patch left to send before reporting the result.
        unsigned int m_uNbPatchLeft;
    };

    // This is an internal struct used for the Preview mechanism.
    struct SPreviewDocument
    {
        // The preview document.
        ISceXmlDocument* m_pDocument;

        // The preview document's eTag. Empty if none.
        CString m_strETag;
    };

//-- Published interface.
public:

//-- Hidden methods.
protected:
    //-- << Helper methods >>

    // Summary:
    //   Creates and sets the Xml Document Control instance.
    static mxt_result CreateObject(IN IEComUnknown* pOuterIEComUnknown,
                                   IN mxt_clsid documentClassId,
                                   IN const char* pszAuid,
                                   IN const char* pszDocName,
                                   IN STraceNode& rNewChildNode,
                                   IN STraceNode& rNewChildEComNode,
                                   OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaImppXmlDocumentControl(IN IEComUnknown* pOuterIEComUnknown,
                              IN STraceNode& rChildNode,
                              IN STraceNode& rChildEComNode);

    // Summary:
    //  Destructor.
    virtual ~CUaImppXmlDocumentControl();

    //-- << CEComUnknown methods >>

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- << IUaImppXmlDocumentControl Implementation >>

    virtual mxt_result SetManager(IN IUaImppXmlDocumentControlMgr* pMgr);

    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);

    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);

    virtual mxt_result SetDocumentInformation(IN mxt_clsid documentClassId,
                                              IN const char* pszAuid,
                                              IN const char* pszDocName);

    virtual void OverrideBaseTracingNodes(IN STraceNode& rNewChildNode,
                                          IN STraceNode& rNewChildEComNode);

    virtual STraceNode& GetTraceNode();

    virtual STraceNode& GetEComTraceNode();

    virtual mxt_result SetStateSynchronizationMode(IN EStateSynchronizationMode eMode);

    virtual mxt_result SetXcapUpdateMode(IN EXcapUpdateMode eMode);

    virtual mxt_result SaveDocument(INOUT CBlob& rBlob, OUT CString& rstrEntityTag);

    virtual mxt_result RestoreDocument(IN const CBlob* pBlobContent,
                                       IN const char* pszEntityTag,
                                       IN bool bModified);

    virtual mxt_result SynchronizeState();

    virtual IUaImppXmlDocumentControlMgr::EDocumentState GetCurrentState();

    virtual mxt_result GetDocumentPreview();

    virtual mxt_result ApplyDocumentPreview();

    virtual mxt_result ReleaseDocumentPreview();

    virtual mxt_result SynchronizeFromServer(IN ESynchOption eOption = eSYNCH_NORMAL);

    virtual mxt_result SynchronizeToServer(IN ESynchOption eOption = eSYNCH_NORMAL);

    virtual mxt_result DeleteServerDocument();

    virtual void GetDocument(OUT ISceXmlDocument*& rpDocument);

    virtual mxt_result QuerySubscriber(IN mxt_iid iidRequested, OUT void** ppInterface);

    //-- << IXcapRequestMgr Implementation >>

    virtual void EvGetResult(IN mxt_opaque opqAppTransactionId,
                             IN unsigned int uResponseCode,
                             IN TOA CMimeObject* pMimeObject,
                             IN const char* pszEntityTag,
                             IN IXmlDocument* pErrorDocument);

    virtual void EvPutResult(IN mxt_opaque opqAppTransactionId,
                             IN unsigned int uResponseCode,
                             IN const char* pszEntityTag,
                             IN IXmlDocument* pErrorDocument);

    virtual void EvDeleteResult(IN mxt_opaque opqAppTransactionId,
                                IN unsigned int uResponseCode,
                                IN const char* pszEntityTag,
                                IN IXmlDocument* pErrorDocument);

    virtual void EvAuthenticationRequired(IN const char* pszRealm);

    //-- << IXmlDocumentMgr implementation >>

    virtual void EvChangedDocument(IN IXmlDocument* pDocument);

    //-- << IUaImppXcapDocumentSubscriberMgr implementation >>

    virtual void EvTerminated(IN IUaImppXcapDocumentSubscriber* pComponent,
                              IN const char* pszMonitoredDocumentUri);

    virtual void EvDocumentChanged(IN IUaImppXcapDocumentSubscriber* pComponent,
                                   IN IXmlDocument* pXcapDiffDocument,
                                   IN const CXmlElement* pDocumentElement);

    //-- << IPrivateSceChallengeSource implementation >>

    virtual mxt_result SetCredentials(IN const CString& strRealm,
                                      IN const CString& strUsername,
                                      IN const CString& strPassword,
                                      IN mxt_opaque opqSourceData);

    virtual mxt_result CancelAuthentication(IN const CString& strRealm,
                                            IN mxt_opaque opqSourceData);



private:
    //-- << Helper methods >>

    //  Summary:
    //   Changes the state of the document and reports it to the manager.
    void ChangeStateHelper(IUaImppXmlDocumentControlMgr::EDocumentState eState);

    //  Summary:
    //   Verifies and updates the state of the document according to the server result.
    mxt_result VerifyStateServerHelper(IN unsigned int uResponseCode,
                                       IN const char* pszEntityTag,
                                       IN ESyncMethodIds eSynchMethod);

    //  Summary:
    //   Verifies and updates the state of the document according to the local result.
    void VerifyStateLocalHelper();

    //  Summary:
    //   Creates a new document and returns it.
    bool CreateDocumentHelper(OUT GO ISceXmlDocument*& rpDocument);

    //  Summary:
    //   Creates a preview document and returns its IEComUnknown interface.
    bool CreatePreviewDocumentHelper(OUT GO IEComUnknown*& rpEComUnknown,
                                     IN TOA CMimeObject* pMimeToParse = NULL,
                                     IN bool bApplyPatches = false);

    //  Summary:
    //   Applies the server patches to the given document.
    mxt_result ApplyServerPatchesHelper(INOUT ISceXmlDocument*& rpDocument);

    //  Summary:
    //   Release the server patches and optionnally release the container as well.
    void ReleaseServerPatchesHelper(IN bool bReleaseContainer = false);

    //  Summary:
    //   Sets this object as the internal document manager.
    void SetInternalDocumentMgrHelper();

    //  Summary:
    //   Sends an HTTP request.
    mxt_result SendHttpRequestHelper(IN TOA SXCapOperationData* pstXcapData,
                                     IN TOA CString* pstrNodeSelector,
                                     IN TOA CMimeObject* pPayload);

    //  Summary:
    //   Sends the next Xcap patch HTTP request.
    mxt_result SendNextXcapPatchHelper(IN unsigned int uNbPatch);

    //  Summary:
    //   Deletes the XCAP operation data.
    void DeleteXcapOperationData(IN TOA SXCapOperationData* pstXcapData);

    //  Summary:
    //   Allocates memory for a new buffered eTag.
    void AllocateBufferedEtag(OUT GO char*& rpszDestination,
                              IN const char* pszSource);

    //  Summary:
    //   Releases memory of a buffered eTag.
    void ReleaseBufferedEtag(IN char* pszEtag);

    //  Summary:
    //   Resets the XCAP patches.
    void ResetXcapPatchesHelper();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaImppXmlDocumentControl(IN const CUaImppXmlDocumentControl& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaImppXmlDocumentControl& operator=(IN const CUaImppXmlDocumentControl& rSrc);


//-- Hidden Data Members.
protected:
    // The pointer on the document.
    // This is protected so that the child class can access it
    // and query the specialized interface on it.
    ISceXmlDocument* m_pDocument;

private:
    // The trace node of the child class. This is used in all traces of this
    // class (except ECOM related traces) to help isolate the traces of a
    // specific component type.
    STraceNode* m_pstTraceNode;

    // The ECOM trace node of the child class. This is used in all ECOM traces
    // of this class to help isolate the traces of a specific component type.
    STraceNode* m_pstEcomTraceNode;

    // Pointer to the user configuration object. This is an ECOM and we are
    // keeping a reference to it. It can be NULL throughout the life of this
    // object.
    ISceUserConfig* m_pUserConfig;

    // Manager to notify about the various events of this class.
    IUaImppXmlDocumentControlMgr* m_pMgr;

    // Document state.
    IUaImppXmlDocumentControlMgr::EDocumentState m_eState;

    // The XML document class ID.
    mxt_clsid m_documentClassId;

    // The XML document XCAP ID.
    CString m_strAuid;

    // The litteral XML document name.
    CString m_strDocName;

    // The document URI to monitor.
    CString* m_pstrDocUri;

    // The XML document XCAP Entity Tag.
    CString m_strETag;

    // Tells if the document is modified or not.
    bool m_bIsDocModified;

    // The synchronization mode to use.
    EStateSynchronizationMode m_eMode;

    // The Xcap Client to use.
    IXcapClient* m_pXcapClient;

    // The Xcap Document Subscriber to use.
    IUaImppXcapDocumentSubscriber* m_pXcapDocSubscriber;

    // The preview document information.
    SPreviewDocument* m_pstPreview;

    // The vector of received patch operations.
    CVector<const CXmlElement*>* m_pvecPatchOps;

    // The vector of pointers to the received XCAP eTags. See m_pSubAllocator.
    CVector<char*>* m_pvecXcapEtag;

    // The sub-allocator used to keep the received eTags.
    CSubAllocator* m_pSubAllocator;

    // Tells if the server supports sending the etag with failure responses as it should.
    bool m_bUnsupportedFailureEtag;

    // The XCAP patch operations for the internal document.
    CSceXcapPatchOperations* m_pXcapPatchOps;

    // Tells if the uninitialize should be delayed or not.
    bool m_bDelayedUninitialize;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)

//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CUAIMPPXMLDOCUMENTCONTROL_H
