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
#ifndef MXG_IUAIMPPXCAPDOCUMENTSUBSCRIBERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPXCAPDOCUMENTSUBSCRIBERMGR_H

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


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CXmlElement;
class IUaImppXcapDocumentSubscriber;
class IXmlDocument;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppXcapDocumentSubscriberMgr
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Description:
//
//  This is the interface through which the IUaImppXcapDocumentSubscriber
//  reports events regarding the modifications detected on a XCAP document.
//
// Location:
//   SceUaImpp/IUaImppXcapDocumentSubscriberMgr.h
//
// See Also:
//   IUaImppXcapDocumentSubscriber
//
//==============================================================================
class IUaImppXcapDocumentSubscriberMgr
{
//-- Friend Declarations


//-- Published Interface
public:


    //==========================================================================
    //==
    //==  EvTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports a final event to the manager.
    //
    //  Parameters:
    //    pComponent:
    //      The component reporting this event.
    //
    //  Description:
    //    Reports a final event to the manager.
    //    
    //    This event is always the last event reported by the
    //    IUaImppXcapDocumentSubscriber component to this manager. It can be
    //    reported by the IUaImppXcapDocumentSubscriber on two different
    //    occasions:
    //    
    //      - The user has called
    //        IUaImppXcapDocumentSubscriberMgr::UnmonitorDocument. EvTerminated
    //        is then reported to signal that no more events will be sent to
    //        this manager for the specified document. This can be useful when
    //        the events are handled asynchronously.
    //    
    //      - The associated IUaImppXcapDocumentSubscriber is being terminated
    //        and must thus inform its users that it will no longer be able to
    //        report events regarding the specified URI.
    //      
    //==========================================================================
    virtual void EvTerminated(IN IUaImppXcapDocumentSubscriber* pComponent,
                              IN const char* pszMonitoredDocumentUri) = 0;


    //==========================================================================
    //==
    //==  EvDocumentChanged
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the document or some information pertaining to it has
    //      changed.
    //
    //  Parameters:
    //    pComponent:
    //      The component reporting this event.
    //
    //    pXcapDiffDocument:
    //      The document containing pDocumentElement.
    //
    //    pDocumentElement:
    //      Pointer to the \<document\> element that provides information about
    //      the updated document.
    //
    //      The \<document\> element is defined in draft-ietf-simple-xcap-diff.
    //
    //      To use this pointer AFTER this method has ended, a reference MUST
    //      be kept on pXcapDiffDocument. The reference must be released when
    //      the element is not used anymore.
    //
    //  Description:
    //      Reports that the document or some information pertaining to it has
    //      changed.
    //
    //      The information about the document is provided in pDocumentElement.
    //
    //==========================================================================
    virtual void EvDocumentChanged(IN IUaImppXcapDocumentSubscriber* pComponent,
                                   IN IXmlDocument* pXcapDiffDocument,
                                   IN const CXmlElement* pDocumentElement) = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
protected:
    // Summary:
    //  Default Constructor.
    IUaImppXcapDocumentSubscriberMgr(){}

    // Summary:
    //  Destructor.
    virtual ~IUaImppXcapDocumentSubscriberMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaImppXcapDocumentSubscriberMgr(IN const IUaImppXcapDocumentSubscriberMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaImppXcapDocumentSubscriberMgr& operator=(IN const IUaImppXcapDocumentSubscriberMgr& rSrc);


//-- Hidden Data Members
protected:
private:

//M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUAIMPPXCAPDOCUMENTSUBSCRIBERMGR_H

