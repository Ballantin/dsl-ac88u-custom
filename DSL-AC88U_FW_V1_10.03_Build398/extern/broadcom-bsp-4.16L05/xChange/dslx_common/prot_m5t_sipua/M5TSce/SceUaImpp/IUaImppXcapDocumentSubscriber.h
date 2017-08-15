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
#ifndef MXG_IUAIMPPXCAPDOCUMENTSUBSCRIBER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPXCAPDOCUMENTSUBSCRIBER_H

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

#if !defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)
#error You must define MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT to be able to use this class.
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_ISCESUBSCRIBERMGR_H
#include "SceCoreComponents/ISceSubscriberMgr.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CNameAddr;
class ISceUserConfig;
class IUaImppXcapDocumentSubscriberMgr;
class IUri;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IUaImppXcapDocumentSubscriber);

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppXcapDocumentSubscriber
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Description:
//  The XCAP Document Subscriber monitors one or more XCAP documents and reports
//  events to different managers when a document is changed and a manager is
//  interested in its modifications.
//
//  This component implements draft-ietf-sip-xcapevent-04 and
//  draft-ietf-simple-xcap-diff-09.
//
//== MXI_TBD (2008-09-30): Update above references when they become RFC.
//
// Location:
//   SceUaImpp/IUaImppXcapDocumentSubscriber.h
//
// See Also:
//   IUaImppXcapDocumentSubscriberMgr
//
//==============================================================================
class IUaImppXcapDocumentSubscriber : public IEComUnknown
{
//-- Friend Declarations


//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaImppXcapDocumentSubscriber);
//M5T_INTERNAL_USE_END

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
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an opaque paramerer to this object.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      Sets an opaque parameter to this object. It can be used by the
    //      application to store any information.
    //
    //==========================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the opaque parameter from this object.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      Gets the opaque parameter from this object. It can be used by
    //      the application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==========================================================================
    //==
    //==  GetSubscriptionState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the subscription state.
    //
    //  Returns:
    //      The subscription state.
    //
    //  Description:
    //      Gets the subscription state of this subscriber component.
    //
    //==========================================================================
    virtual ISceSubscriberMgr::ESubscriptionStatus GetSubscriptionState() = 0;

    //==========================================================================
    //==
    //==  SetNotifier
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the server where to subscribe for XCAP document state.
    //
    //  Parameters:
    //    pResourceAor:
    //      The address of the resource to which the component must subscribe.
    //      Will be used in the To header. Ownership is taken. Cannot be NULL.
    //
    //    pNotifierRequestUri:
    //      The address of the resource to which the component must subscribe.
    //      Will be used in the request-URI. Can be NULL, in which case the URI
    //      found in pResourceAor will be used instead. Ownership is taken.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Overrides the server to use to monitor XCAP documents. By default,
    //      the server set in IUaImppXcapConfig is used, but it can be possible
    //      for applications to monitor XCAP documents using some other notifier
    //      by using this method.
    //
    //      This method can only be used before it establishes any subscription.
    //      It will return an error if used after MonitorDocument is called.
    //
    //==========================================================================
    virtual mxt_result SetNotifier(IN TOA CNameAddr* pResourceAor,
                                   IN TOA IUri* pNotifierRequestUri) = 0;

    //==========================================================================
    //==
    //==  MonitorDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //      Asks the component to monitor a document.
    //
    //  Parameters:
    //    pszDocUri:
    //      The URI of the document to monitor. Must be an XCAP URI to the
    //      document or to an element within a document. It can either be an
    //      absolute URI or a relative URI. Must not be NULL. the pointed
    //      C-style string must remain valid as long as the document is
    //      monitored.
    //
    //    pMgr:
    //      Pointer to the manager that is to be notified of changes done to the
    //      document. Must not be NULL.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Starts monitoring a document or an element within an XCAP document.
    //
    //      Events regarding the state of the document are reported to the
    //      configured manager.
    //
    //      It is not possible to have more than one manager that monitors the
    //      same document. The second manager to request this will get an error
    //      when calling MonitorDocument.
    //
    //      The default event is xcap-diff in "xcap-patching" mode.
    //
    //  See Also:
    //      UnmonitorDocument
    //
    //==========================================================================
    virtual mxt_result MonitorDocument(IN const char* pszDocUri,
                                       IN IUaImppXcapDocumentSubscriberMgr* pMgr) = 0;


    //==========================================================================
    //==
    //==  UnmonitorDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //      Stops monitoring a document.
    //
    //  Parameters:
    //    pszDocUri:
    //      Pointer to the URI that was monitored.
    //
    //  Returns:
    //      resS_OK upon success.
    //
    //      resSW_WARNING if the document is not monitored.
    //
    //      An error otherwise.
    //
    //  Description:
    //      Stops monitoring a document that was previously monitored using
    //      MonitorDocument.
    //
    //      The event EvTerminated is always reported when this method is
    //      called. This is always the last event reported to the manager.
    //
    //  See Also:
    //      MonitorDocument
    //
    //==========================================================================
    virtual mxt_result UnmonitorDocument(IN const char* pszDocUri) = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
protected:
    // Summary:
    //  Default Constructor.
    IUaImppXcapDocumentSubscriber(){}

    // Summary:
    //  Destructor.
    virtual ~IUaImppXcapDocumentSubscriber(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaImppXcapDocumentSubscriber(IN const IUaImppXcapDocumentSubscriber& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaImppXcapDocumentSubscriber& operator=(IN const IUaImppXcapDocumentSubscriber& rSrc);


//-- Hidden Data Members
protected:
private:

//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUAIMPPXCAPDOCUMENTSUBSCRIBER_H

