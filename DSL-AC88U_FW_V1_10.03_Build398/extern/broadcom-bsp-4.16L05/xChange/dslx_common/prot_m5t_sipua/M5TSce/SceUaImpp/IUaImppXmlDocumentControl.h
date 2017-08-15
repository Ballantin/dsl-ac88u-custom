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
#ifndef MXG_IUAIMPPXMLDOCUMENTCONTROL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPXMLDOCUMENTCONTROL_H

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

#if !defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)
#error You must define MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT to be able to use this class.
#endif

#ifndef MXG_IUAIMPPXMLDOCUMENTCONTROLMGR_H
#include "SceUaImpp/IUaImppXmlDocumentControlMgr.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CBlob;
class CString;
class ISceUserConfig;
class ISceXmlDocument;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IUaImppXmlDocumentControl);

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppXmlDocumentControl
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Description:
//
//  The IUaImppXmlDocumentControl defines the API to control the local
//  representation of a document that is stored on a XCAP server.
//
//  An XML Document Control object (or an object that implements this interface)
//  offers the application different mechanisms to manage an XML document stored
//  on an XCAP server.
//
//    - It allows to get the document from the server and thus have a local
//      copy of the document that is stored on the server;
//    - It allow to create a local representation of a document and then
//      upload it on the server;
//    - It tracks modifications done to the local copy of the document. The
//      modifications done to the document can later be applied to the document
//      held by the server;
//    - It allows polling the server for detecting document changes done on
//      the server;
//    - It allows subscribing to the server (using SIP) to be notified when
//      the document on the server changes. It is also able to receive patch
//      operations from the server in order to update the document without
//      having to do a get of the complete document;.
//    - It allows getting the document from the server without overwriting the
//      local document. This can be useful for merge operations.
//
//  An XML Document Control object pretty much reuses the basic concepts of a
//  source control or document control system. The XML Document Control has a
//  local representation of a document held by the server. The local document
//  has a state that depends on its difference with the server's version and how
//  this difference was achieved. The enumeration
//  IUaImppXmlDocumentControlMgr::EDocumentState enumerates the different state
//  that a local document can have.
//
// Location:
//   SceUaImpp/IUaImppXmlDocumentControl.h
//
// See Also:
//   IUaImppXmlDocumentControlMgr
//
//==============================================================================
class IUaImppXmlDocumentControl : public IEComUnknown
{
//-- Friend Declarations.

//-- Public Custom Types.
public:
    // Documentation:
    //  This enumeration defines the possible behavior for detecting that the
    //  state of the document on the server has changed.
    enum EStateSynchronizationMode
    {
        // Description:
        //  The application must manually refresh the state of the document
        //  using SynchronizeState. No subscription is established to detect
        //  that the document has changed.
        eSTATESYNCHMODE_MANUAL,

        // Description:
        //  The component must try to establish a subscription to be notified
        //  of changes done to the document on the XCAP server.
        eSTATESYNCHMODE_SUBSCRIBE,
    };

    // Documentation:
    //  This enumeration defines the possible behavior for sending the XML
    //  document changes to the remote XCAP server.
    enum EXcapUpdateMode
    {
        // Description:
        //  The full XML document will always be sent to the server, regradless
        //  of the changes. This mode requires more bandwith but less runtime
        //  memory and potentially will avoid most compatibility problems that
        //  could occur with XCAP servers that are not fully compliant.
        eXCAPUPDATEMODE_FULL,

        // Description:
        //  Only the XCAP patches reflecting the changes in the XML document
        //  will be sent to the server. This mode requires more runtime memory
        //  but less bandwith. However, compatibility problems can occur with
        //  XCAP servers that are not fully compliant.
        eXCAPUPDATEMODE_PARTIAL,
    };

//-- Published Interface.
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaImppXmlDocumentControl);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager for this object.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the object's manager.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_INVALID_ARGUMENT: pMgr cannot be NULL.
    //
    //  Description:
    //      Configures the manager for this object.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN IUaImppXmlDocumentControlMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures this object with a user configuration object.
    //
    //  Parameters:
    //    pConfig:
    //      Pointer to the ECOM configuration object for this user.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_INVALID_ARGUMENT: pConfig cannot be NULL.
    //      - resFE_INVALID_STATE: Configuration already set.
    //      - Other: Method failure.
    //
    //  Description:
    //      Configures this object with a user configuration object.
    //
    //==========================================================================
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig) = 0;

    //==========================================================================
    //==
    //==  GetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with this object.
    //
    //==========================================================================
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig) = 0;

    //==========================================================================
    //==
    //==  SetDocumentInformation
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the information about this document.
    //
    //  Parameters:
    //    documentClassId:
    //      The classId to create the document with.
    //
    //    pszAuid:
    //      The identifier of the document.
    //
    //    pszDocName:
    //      The name of the document.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_INVALID_ARGUMENT: Arguments cannot be NULL.
    //      - resFE_INVALID_STATE: Information already set.
    //      - Other: Method failure.
    //
    //  Description:
    //      Sets the information about this document.
    //
    //      This should only be called for a generic XML Document Control
    //      instance, one created using CreateEComInstance with
    //      CLSID_CXmlDocumentControl. If any other CLSID were used, then the
    //      information is aleady set.
    //
    //==========================================================================
    virtual mxt_result SetDocumentInformation(IN mxt_clsid documentClassId,
                                              IN const char* pszAuid,
                                              IN const char* pszDocName) = 0;

    //==========================================================================
    //==
    //==  OverrideBaseTracingNodes
    //==
    //==========================================================================
    //
    //  Summary:
    //      Overrides the default tracing nodes.
    //
    //  Parameters:
    //    rNewChildNode:
    //      The new component node to use for tracing.
    //
    //    rNewChildEComNode:
    //      The new ECOM node to use for tracing.
    //
    //  Description:
    //      This method overrides the normal tracing nodes with new ones to use.
    //      This enables the derived child components to specifically trace all
    //      the different calls to their own XML Document Control with different
    //      nodes.
    //
    //      This should only be called for a generic XML Document Control
    //      instance, one created using CreateEComInstance with
    //      CLSID_CXmlDocumentControl. If any other CLSID were used, then the
    //      information is aleady set.
    //
    //==========================================================================
    virtual void OverrideBaseTracingNodes(IN STraceNode& rNewChildNode,
                                          IN STraceNode& rNewChildEComNode) = 0;

    //==========================================================================
    //==
    //==  GetTraceNode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the tracing nodes.
    //
    //  Returns:
    //      The node to use for tracing.
    //
    //  Description:
    //      This method returns the correct node to use for tracing in the
    //      child component. This is especially useful when the child's tracing
    //      nodes are overriden.
    //
    //==========================================================================
    virtual STraceNode& GetTraceNode() = 0;

    //==========================================================================
    //==
    //==  GetEComTraceNode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the ECOM tracing nodes.
    //
    //  Returns:
    //      The node to use for ECOM tracing.
    //
    //  Description:
    //      This method returns the correct node to use for ECOM tracing in the
    //      child component. This is especially useful when the child's tracing
    //      nodes are overriden.
    //
    //==========================================================================
    virtual STraceNode& GetEComTraceNode() = 0;

    //==========================================================================
    //==
    //==  SetStateSynchronizationMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets how the component detects that the state of the document has
    //      changed.
    //
    //  Parameters:
    //    eMode:
    //      The synchronization mode to set.
    //
    //  Returns:
    //      - resS_OK: The mode change is effective immidiately.
    //      - resSW_ASYNC_PROCESSING: The mode needs asynchronous processing
    //                                before the change is effective.
    //      - Failure code: An error occured.
    //
    //  Description:
    //      Configures how the component detects that the document held on the
    //      server was changed in order to update the state of the local
    //      document.
    //
    //      By default, the component uses the eSTATESYNCHMODE_MANUAL mode.
    //
    //      <I>
    //      Applications should consider the bandwidth usage overhead of each
    //      synchronization mechanism before selecting which one to use for a
    //      document.
    //
    //      - Documents that are kept on the local device for a short period
    //        of time may benefit from manual synchronization, if any
    //        synchronization mechanism is needed.
    //      - Documents that are kept on the local device for a long period of
    //        time, and which can be modified by other devices, will benefit
    //        from subscribing to the state of the document on the server to
    //        detect changes done to the document by other devices.
    //
    //      Users of this interface must keep in mind that the
    //      eSTATESYNCHMODE_MANUAL mode only incurs bandwidth overhead when
    //      calling SynchronizeState, and this overhead consist of an outgoing
    //      HTTP request and an incoming response (with possibly the full XCAP
    //      document if the server does not support sending entity-tags in
    //      failure responses).
    //
    //      The subscription mode incurs bandwidth overhead as soon as
    //      SetStateSynchronizationMode(eSTATESYNCHMODE_SUBSCRIBE) is called. A
    //      SIP subscription is established with the server, meaning an exchange
    //      of at least 8 SIP messages (SUBSCRIBE, 200 OK to SUBSCRIBE, NOTIFY,
    //      200 OK to NOTIFY, final SUBSCRIBE, 200 OK to final SUBSCRIBE, final
    //      NOTIFY and 200 OK to final NOTIFY) depending on the number of NOTIFY
    //      messages received during the length of time the subscription is
    //      active.
    //      </I>
    //
    //  See Also:
    //      EStateSynchronizationMode
    //
    //==========================================================================
    virtual mxt_result SetStateSynchronizationMode(IN EStateSynchronizationMode eMode) = 0;


    //==========================================================================
    //==
    //==  SetXmlDocumentUpdateMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets how the component sends the XML document to the XCAP server.
    //
    //  Parameters:
    //    eMode:
    //      The document update mode to set.
    //
    //  Returns:
    //      - resS_OK: The mode change is effective immidiately.
    //      - resFE_INVALID_STATE: The mode was not changed because the
    //                             component already operates in the requested
    //                             mode.
    //
    //  Description:
    //      Configures how the component sends the XML document to the XCAP
    //      server. This only affects the sending of XCAP updates to the server,
    //      not the way it receives them.
    //
    //      By default, the component operates in the eXCAPUPDATEMODE_PARTIAL
    //      mode.
    //
    //      <I>
    //      Applications should consider the bandwidth usage overhead of each
    //      XCAP update mechanism before selecting which one to use for a
    //      document.
    //
    //      - Documents that contains very little of information or that
    //        will change often will benefit from a full update mode.
    //      - Documents that contains a lot of information or that will
    //        not change often will benefit from the partial update mode.
    //
    //      Users of this interface must keep in mind that the
    //      eXCAPUPDATEMODE_FULL mode only incurs bandwidth overhead when
    //      calling SynchronizeToServer, and this overhead consist of an
    //      outgoing HTTP request with the full document as payload and an
    //      incoming response. This mode is also less likely to have
    //      compatibility problems with non-fully compliant XCAP server.
    //
    //      The partial mode incurs bandwidth overhead for each changes done to
    //      the document. In this case, the overhead consists of an outgoing
    //      HTTP request with a single change as payload and an incoming
    //      response for each changes. This mode may have compatibility problems
    //      with non-fully compliant XCAP server.
    //      </I>
    //
    //  See Also:
    //      EXmlDocumentUpdateMode
    //
    //==========================================================================
    virtual mxt_result SetXcapUpdateMode(IN EXcapUpdateMode eMode) = 0;

    //==========================================================================
    //==
    //==  SaveDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //      Saves the current version of the document to a blob.
    //
    //  Parameters:
    //    rBlob:
    //      Reference to a blob, which at exit of this method will contain the
    //      XML document in text format.
    //
    //    rstrEntityTag:
    //      Reference to the entity tag, which at the exit of this method is set
    //      with a string that provides information about the version of this
    //      document. This must ideally be saved with the blob if
    //      RestoreDocument is to be later called. Can return an empty string if
    //      the document is not current.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Saves the current version of the document to a blob. This can be
    //      used to save the document to a file for storage while the
    //      application is not running, or for periodic backup of the document.
    //
    //  See Also:
    //      RestoreDocument
    //
    //==========================================================================
    virtual mxt_result SaveDocument(INOUT CBlob& rBlob, OUT CString& rstrEntityTag) = 0;

    //==========================================================================
    //==
    //==  RestoreDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //      Restores a document from file or from memory.
    //
    //  Parameters:
    //    pBlobContent:
    //      Pointer to a CBlob which holds the data that corresponds to the
    //      document. It MUST not be NULL terminated.
    //
    //    pszEntityTag:
    //      Pointer to the entity tag that was previously saved with the
    //      document. Can be NULL if SaveDocument returned an empty entity tag.
    //
    //    bModified:
    //      True if the document was modified by an external mechanism outside
    //      of the control of this component. As an example, if the file was
    //      saved on a file system and was then modified using an external file
    //      editor, this parameter would be set to true.
    //
    //  Returns:
    //      resS_OK if the method succeeds, a failure otherwise.
    //
    //  Description:
    //      Restores a document that was previously saved using SaveDocument.
    //
    //      pszEntityTag must correspond to the entity tag returned by
    //      SaveDocument.
    //
    //  See Also:
    //      SaveDocument
    //
    //==========================================================================
    virtual mxt_result RestoreDocument(IN const CBlob* pBlobContent,
                                       IN const char* pszEntityTag,
                                       IN bool bModified) = 0;

    //==========================================================================
    //==
    //==  SynchronizeState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Synchronizes the state of the document with the XCAP server.
    //
    //  Returns:
    //      - resSW_ASYNC_PROCESSING: The event will be reported after
    //                                asynchronous processing.
    //      - Any error code: An error occured and the even will not be
    //                        reported.
    //
    //  Description:
    //      Synchronizes the state of the document with the XCAP server.
    //
    //      This has the effect of querying the server for the current document
    //      version, but no document is actually downloaded from the server.
    //
    //      This method will return resSW_ASYNC_PROCESSING when it was able to
    //      start the processing to query the server.
    //      IUaImppXmlDocumentControlMgr::EvSynchronizeStateResult() is then
    //      reported once synchronization succeeds or fails. No event is
    //      reported if this method returns a failure result.
    //
    //      Note that if the component detects that the state of the document
    //      has changed as a result of calling SynchronizeState, EvDocumentState
    //      will be reported *before* EvSynchronizeStateResult.
    //
    //      When IUaImppXmlDocumentControlMgr::EvSynchronizeStateResult()
    //      returns a resFE_NOT_IMPLEMENTED result, this method should not be
    //      used anymore. GetDocumentPreview should be used instead.
    //
    //      In any case, if this method is still used, it may report
    //      IUaImppXmlDocumentControlMgr::EvDocumentPreview() right after the
    //      normal IUaImppXmlDocumentControlMgr::EvSynchronizeStateResult() if
    //      a document is now in state MERGE or OUT OF DATE. At this point, the
    //      preview MUST be dealt with in the same fashion as if
    //      GetDocumentPreview() was called.
    //
    //      <I>
    //      SynchronizeState is done by sending a GET request to the XCAP server
    //      with a "If-None-Match: *" header. This has the effect of trying to
    //      get the entity-tag of the document on the server without actually
    //      getting the document itself.
    //
    //      The XML Document Control expects the server to answer with a
    //      negative final response that includes the entity-tag of the document
    //      held by the server. However there are some servers that do not
    //      include this entity-tag in their responses, which prevents
    //      SynchronizeState from detecting the state of the document on the
    //      server. Under such condition, EvSynchronizeStateResult will report
    //      resFE_NOT_IMPLEMENTED.
    //
    //      When EvSynchronizeStateResult reports resFE_NOT_IMPLEMENTED, it
    //      means that the only way to know the actual state of the document on
    //      the server is to get the document from the server. This means the
    //      user of this interface has to rely on GetDocumentPreview to detect
    //      changes done on the server.
    //
    //      Any call done to SynchronizeState *after* it has reported
    //      EvSynchronizeStateResult with resFE_NOT_IMPLEMENTED will result in a
    //      conditional GET operation that will report a preview document if the
    //      document was changed on the server, in which case
    //      EvSynchronizeStateResult will report resSW_WARNING to indicate that
    //      IUaImppXmlDocumentControlMgr::EvDocumentPreview will be reported
    //      next *even if GetDocumentPreview was not called*.
    //      </I>
    //
    //==========================================================================
    virtual mxt_result SynchronizeState() = 0;

    //==========================================================================
    //==
    //==  GetCurrentState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the current document state.
    //
    //  Returns:
    //      The current document state.
    //
    //  Description:
    //      Returns the current document state.
    //
    //==========================================================================
    virtual IUaImppXmlDocumentControlMgr::EDocumentState GetCurrentState() = 0;

    //==========================================================================
    //==
    //==  GetDocumentPreview
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allows the application to preview the version of the document
    //      currently stored on the XCAP server.
    //
    //  Returns:
    //      - resS_OK: The event was already reported before returning.
    //      - resSW_ASYNC_PROCESSING: The event will be reported after
    //                                asynchronous processing.
    //      - Any error code: An error occured and the even will not be
    //                        reported.
    //
    //  Description:
    //      Allows the application to preview the version of the document
    //      currently stored on the XCAP server.
    //
    //      This method will have different behavior depending on how
    //      SetDocumentSubscriptionMode was set, on whether or not the server
    //      sent partial document updates to this component and on the current
    //      state of the document.
    //
    //      - If the document is in the state eSTATE_OUTOFDATE and if no XCAP
    //        patch operations were received, then the component gets a fresh
    //        copy of the document from the server and returns it to the
    //        application.
    //
    //      - If the document is in the state eSTATE_OUTOFDATE and if XCAP
    //        patch operations were received, the component applies the patches
    //        to a copy of the local document and returns it to the application.
    //
    //      - If the document is in the state eSTATE_UNKNOWN, eSTATE_MERGE or
    //        eSTATE_MODIFIED, then the component gets a fresh copy of the
    //        document from the server and returns it to the application. Note
    //        that while doing so, the component may find out that the document
    //        was changed on the server, thus it is possible that
    //        EvDocumentState is reported before EvDocumentPreview is reported.
    //
    //      - If the document is in the state eSTATE_CURRENT, then the
    //        component performs a conditional get to retreive the document only
    //        if it was changed on the server. If the document was changed, a
    //        document state change event is reported and then the updated
    //        document is provided to the application. If the document was not
    //        changed, no state change event is reported and the current
    //        document is provided to the application.
    //
    //      This method will return resS_OK if it was able to start the
    //      processing to query the server.
    //      IUaImppXmlDocumentControlMgr::EvDocumentPreview() is then
    //      reported once the operation succeeds or fails. No event is
    //      reported if this method returns a failure result.
    //
    //      Once this component has fetched or created a preview document for
    //      the application, the application may choose to use this preview as
    //      the local copy, in which case it can call ApplyDocumentPreview. Note
    //      that this overwrites the current local document with the preview
    //      document. If the application does not wish to override the local
    //      document with the preview, it should simply call
    //      ReleasePreviewDocument to have the component release the reference
    //      it has on the preview document. Note that the application can keep a
    //      reference to the preview document for as long as it wants, but must
    //      eventually release its reference to reclaim memory.
    //
    //  See Also:
    //      ApplyDocumentPreview
    //
    //==========================================================================
    virtual mxt_result GetDocumentPreview() = 0;

    //==========================================================================
    //==
    //==  ApplyDocumentPreview
    //==
    //==========================================================================
    //
    //  Summary:
    //      Applies the document preview to the local document.
    //
    //  Returns:
    //      - resS_OK: The document preview was applied successfully.
    //      - Any error code: An error occured.
    //
    //  Description:
    //      Applies the document preview to the local document. This effectively
    //      overwrites the local document.
    //
    //  See Also:
    //      GetDocumentPreview
    //
    //==========================================================================
    virtual mxt_result ApplyDocumentPreview() = 0;

    //==========================================================================
    //==
    //==  ReleaseDocumentPreview
    //==
    //==========================================================================
    //
    //  Summary:
    //      Releases the document preview.
    //
    //  Returns:
    //      - resS_OK: The document preview was released successfully.
    //      - Any error code: An error occured.
    //
    //  Description:
    //      Releases the document preview resources.
    //
    //  See Also:
    //      GetDocumentPreview
    //
    //==========================================================================
    virtual mxt_result ReleaseDocumentPreview() = 0;


    // Description:
    //  Defines the behavior of the component when synchronizing from or to the
    //  server.
    enum ESynchOption
    {
        // Description:
        //  Synchronization must take place normally. This mode ensures safe
        //  synchronization and minimizes the chances of data loss.
        eSYNCH_NORMAL,

        // Description:
        //  Forces the synchronization even if the document is in a state where
        //  it could lead to data loss.
        eSYNCH_FORCE
    };

    //==========================================================================
    //==
    //==  SynchronizeFromServer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the local document to be in synch with the document found on
    //      the XCAP server.
    //
    //  Parameters:
    //    eOption:
    //      Behavior to have if the document is in eSTATE_UNKNOWN,
    //      eSTATE_NOTONSERVER, eSTATE_MODIFIED or eSTATE_MERGE. If called with
    //      eSYNCH_NORMAL and the document is in one of the above state, the
    //      method will automatically fail. If called with eSYNCH_FORCE, the
    //      method will get the document from the server and overwrite the local
    //      document irrespective of the local document state.
    //
    //  Returns:
    //      - resS_OK: The event was already reported before returning.
    //      - resSW_ASYNC_PROCESSING: The event will be reported after
    //                                asynchronous processing.
    //      - Any error code: An error occured and the even will not be
    //                        reported.
    //
    //
    //  Description:
    //      Synchronizes the local document with the version from the server.
    //
    //      Depending on various parameters such as the eOption parameter, the
    //      value set in SetStateSynchronizationMode and the type of information
    //      sent by the server, this method may initiate a network access to the
    //      XCAP server.
    //
    //      When this method returns resSW_ASYNC_PROCESSING, an event
    //      EvSynchronizeFromServerResult() will be later reported to the
    //      application once this action completes or fails. No event is
    //      reported if this method returns a failure result.
    //
    //      An application that wants to see or present the document from the
    //      server without overwriting the local document should use
    //      GetDocumentPreview.
    //
    //      #Behavior Information#
    //      This method will take the following actions depending on the
    //      document state and on various parameters of this object.
    //
    //      - Calling this method with eOption set to eSYNCH_FORCE will force
    //        a GET operation on the server. The server's version of the
    //        document is then used to overwrite the local document without any
    //        consideration being done on the state of the local document.
    //        The local document is however preserved if the GET operation
    //        returns no document (i.e.: the document does not exist on the
    //        server).
    //      - Calling this method with eOption set to eSYNCH_NORMAL will
    //        result in an error if the local document is eSTATE_UNKNOWN,
    //        eSTATE_NOTONSERVER eSTATE_MODIFIED or eSTATE_MERGE.
    //      - Calling this method with eOption set to eSYNCH_NORMAL while the
    //        local document is in eSTATE_CURRENT will result in a conditional
    //        GET operation on the server. The version from the server will be
    //        downloaded only if it is different than the local document.
    //      - Calling this method with eOption set to eSYNCH_NORMAL while the
    //        local document is in eSTATE_OUTOFDATE will have different behavior
    //        depending on the value set in SetStateSynchronizationMode. If the
    //        synchronization mode is set to eSTATESYNCHMODE_MANUAL or if set to
    //        eSTATESYNCHMODE_SUBSCRIBE AND the server provided no XCAP patch
    //        operations in its notifications, then a conditional GET operation
    //        is performed on the server to retreive the latest version of the
    //        document, which is used to overwrite the local document. If the
    //        synchronization mode is set to eSTATESYNCHMODE_SUBSCRIBE AND the
    //        server provided XCAP patch operations in its notifications, then
    //        the XCAP patches are applied to the local document and no network
    //        access is performed.
    //
    //  See Also:
    //      SynchronizeToServer, GetDocumentPreview
    //
    //==========================================================================
    virtual mxt_result SynchronizeFromServer(IN ESynchOption eOption = eSYNCH_NORMAL) = 0;


    //==========================================================================
    //==
    //==  SynchronizeToServer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the document found on the XCAP server to be in synch with
    //      the local document.
    //
    //  Parameters:
    //    eOption:
    //      Behavior to have if the document is in eSTATE_UNKNOWN,
    //      eSTATE_CURRENT eSTATE_OUTOFDATE or eSTATE_MERGE. If called with
    //      eSYNCH_NORMAL and the document is in one of the above states, the
    //      method will automatically fail. If called with eSYNCH_FORCE, the
    //      method will overwrite the server's document with the local version
    //      without regards to the local document state.
    //
    //  Returns:
    //      - resSW_ASYNC_PROCESSING: The event will be reported after
    //                                asynchronous processing.
    //      - Any error code: An error occured and the even will not be
    //                        reported.
    //
    //
    //  Description:
    //      Synchronizes the server document with the local version.
    //
    //      When this method returns resSW_ASYNC_PROCESSING, an event
    //      EvSynchronizeToServerResult() will be later reported to the
    //      application once this action completes or fails. No event is
    //      reported if this method returns a failure result.
    //
    //      An application that wants to see or present the document from the
    //      server without overwriting the local document should use
    //      GetDocumentPreview.
    //
    //      #Behavior Information#
    //      This method will take the following actions depending on the
    //      document state and on various parameters of this object.
    //
    //      - Calling this method with eOption set to eSYNCH_FORCE will force
    //        a PUT operation on the server. The server's version of the
    //        document is then overwritten with the local document without any
    //        consideration being done on the state of the local document.
    //      - Calling this method with eOption set to eSYNCH_NORMAL will
    //        result in an error if the local document is in eSTATE_UNKNOWN,
    //        eSTATE_CURRENT eSTATE_OUTOFDATE or eSTATE_MERGE.
    //      - Calling this method with eOption set to eSYNCH_NORMAL while the
    //        local document is in eSTATE_MODIFIED will result in one or more
    //        conditional PUT operations on the server. The version on the
    //        server will be updated only if it corresponds to the original
    //        local version (the version that was modified). If the conditional
    //        PUT operation fails because the document on the server was updated
    //        by some other entity, the operation will fail and the document'
    //        state will transit to eSTATE_MERGE.
    //
    //      SynchronizeToServer may take some time to report
    //      EvSynchronizeToServerResult, as it may be sending multiple requests
    //      to the XCAP server if it operates in the partial update mode. Only
    //      the modifications that were done on the local document at the time
    //      SynchronizeToServer was called will be sent to the server. Any
    //      additional modifications done between the call to
    //      SynchronizeToServer and the event reporting that the synchronization
    //      was completed (EvSynchronizeToServerResult) will not be sent to the
    //      server, and the document will remain in the eSTATE_MODIFIED state.
    //
    //  See Also:
    //      SynchronizeFromServer, GetDocumentPreview
    //
    //==========================================================================
    virtual mxt_result SynchronizeToServer(IN ESynchOption eOption = eSYNCH_NORMAL) = 0;

    //==========================================================================
    //==
    //==  DeleteServerDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //      Deletes the server document.
    //
    //  Returns:
    //      - resSW_ASYNC_PROCESSING: The event will be reported after
    //                                asynchronous processing.
    //      - Any error code: An error occured and the even will not be
    //        reported.
    //
    //  Description:
    //      Deletes the server document.
    //
    //      When this method returns resSW_ASYNC_PROCESSING, an event
    //      EvDeleteServerDocumentResult() will be later reported to the
    //      application once this action completes or fails. No event is
    //      reported if this method returns a failure result.
    //
    //      If the application wants to delete the local document as well,
    //      it simply needs to release the reference on this component.
    //
    //==========================================================================
    virtual mxt_result DeleteServerDocument() = 0;

    //==========================================================================
    //==
    //==  GetDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the local document.
    //
    //  Parameters:
    //    rpDocument:
    //      The local document returned. This object is an ECOM and as such,
    //      it's internal reference count was incremented. This reference MUST
    //      be released once it is it not needed anymore.
    //
    //  Description:
    //      Gets the local document.
    //
    //      This enables the application to directly modify the document outside
    //      of the current object interface.
    //
    //      Take note that the pointer returned will be obsolete and MUST not be
    //      used once :
    //         SaveDocument(...) is called;
    //         ApplyReviewDocument() is called;
    //         IUaImppXmlDocumentControlMgr::EvSynchronizeFromServerResult(...)
    //             is reported with a resS_OK result;
    //         IUaImppXmlDocumentControlMgr::EvDeleteServerDocumentResult(...);
    //             is reported with a resS_OK result AND
    //             DeleteServerDocument(...) had true as parameter.
    //
    //      If you still wish to do modifications on the document after those
    //      time, GetDocument MUST be used again in order to get a pointer to
    //      the new document.
    //
    //==========================================================================
    virtual void GetDocument(OUT ISceXmlDocument*& rpDocument) = 0;

    //==========================================================================
    //==
    //==  QuerySubscriber
    //==
    //==========================================================================
    //
    //  Summary:
    //      Queries the XCAP Subscriber component for the requested extension
    //      interface.
    //
    //  Parameters:
    //    iidRequested:
    //      The requested interface identification. Note that
    //      IID_ISceSubscriber cannot be requested as it must stay internal.
    //
    //    ppInterface:
    //      The returned ECOM interface requested. Can be NULL if the
    //      interface could not be found.
    //
    //  Returns:
    //      resS_OK if the query is successful, an error otherwise.
    //
    //  Description:
    //      Queries the XCAP Subscriber component for the requested extension
    //      interface.
    //
    //      Note that this returns an interface to a component that is used
    //      internally by the XML Document Control component. Only the extension
    //      interfaces should be used on this internal component in order to
    //      avoid any coordination issues between the internal subscriber
    //      component and the XML Document Control component. The use of the
    //      ISceSubscriber interface of this internal component is undefined and
    //      will most probably yield random results.
    //
    //      Can only be called after SetConfiguration, otherwise
    //      resFE_INVALID_STATE will be returned along with a NULL pointer.
    //
    //==========================================================================
    virtual mxt_result QuerySubscriber(IN mxt_iid iidRequested, OUT void** ppInterface) = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods.
protected:
    // Summary:
    //  Default Constructor.
    IUaImppXmlDocumentControl() {}

    // Summary:
    //  Destructor.
    virtual ~IUaImppXmlDocumentControl() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaImppXmlDocumentControl(IN const IUaImppXmlDocumentControl& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaImppXmlDocumentControl& operator=(IN const IUaImppXmlDocumentControl& rSrc);


//-- Hidden Data Members.
protected:
private:

//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUAIMPPXMLDOCUMENTCONTROL_H

