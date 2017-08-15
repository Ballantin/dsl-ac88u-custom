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
#ifndef MXG_IUAIMPPXMLDOCUMENTCONTROLMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPXMLDOCUMENTCONTROLMGR_H

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

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class IUaImppXmlDocumentControl;
class IEComUnknown;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppXmlDocumentControlMgr
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Description:
//   This is the interface through which the IUaImppXmlDocumentControl
//   reports events regarding the various asynchronous operations.
//
// Location:
//   SceUaImpp/IUaImppXmlDocumentControlMgr.h
//
// See Also:
//   IUaImppXmlDocumentControl
//
//==============================================================================
class IUaImppXmlDocumentControlMgr
{
//-- Friend Declarations.


//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>

    //  Summary:
    //   This enum includes the different states of the document that can be
    //   reported through IUaImppXmlDocumentControlMgr::EvDocumentState or
    //   IUaImppXmlDocumentControl::GetCurrentState.
    //
    //  Description:
    //   This enum includes the different states of the document that can be
    //   reported through IUaImppXmlDocumentControlMgr::EvDocumentState or
    //   IUaImppXmlDocumentControl::GetCurrentState.
    enum EDocumentState
    {
        //  Description:
        //   The document's state is unknown because the component has not
        //   synchronized with the server or that an unexpected error happened
        //   and the component could not recover.
        eSTATE_UNKNOWN,

        //  Description:
        //   Server and local document match.
        eSTATE_CURRENT,

        //  Description:
        //   The server document was the only one that was modified.
        eSTATE_OUTOFDATE,

        //  Description:
        //   The local document was the only one that was modified.
        eSTATE_MODIFIED,

        //  Description:
        //   Server and local document were modified. 
        eSTATE_MERGE,

        //  Description:
        //   The local document does not exist on the server.
        eSTATE_NOTONSERVER,

        //  Description:
        //   The document does not exist anywhere.
        eSTATE_DOESNOTEXIST
    };


    //==========================================================================
    //==
    //==  EvDocumentState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the new state of the document.
    //
    //  Parameters:
    //    pCtrl:
    //      The interface that reported the event.
    //
    //    eState:
    //      The new document state.
    //
    //  Description:
    //      Reports the new state of the document. This event will be reported
    //      to the application anytime the state of the document changes. The
    //      manager must thus be prepared to handle multiple events at any time.
    //
    //==========================================================================
    virtual void EvDocumentState(IN IUaImppXmlDocumentControl* pCtrl,
                                 IN EDocumentState eState) = 0;


    //==========================================================================
    //==
    //==  EvSynchronizeStateResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of the SynchronizeState method.
    //
    //  Parameters:
    //    pCtrl:
    //      The interface that reported the event.
    //
    //    res:
    //      The result of the action.
    //
    //  Description:
    //      Reports the result of the SynchronizeState method. It is only
    //      reported after the application calls
    //      IUaImppXmlDocumentControl::SynchronizeState.
    //
    //      When this event returns a resFE_NOT_IMPLEMENTED result,
    //      IUaImppXmlDocumentControl::SynchronizeState should not be used
    //      anymore. IUaImppXmlDocumentControl::GetDocumentPreview should be
    //      used instead.
    //
    //      After reporting a resFE_NOT_IMPLEMENTED result, it is very much
    //      possible for this event to report resSW_WARNING results, indicating
    //      that EvDocumentPreview() will be called right after each time.
    //      The preview document MUST be dealt with just like any other time.
    //
    //  See also:
    //      IUaImppXmlDocumentControl::SynchronizeState
    //
    //==========================================================================
    virtual void EvSynchronizeStateResult(IN IUaImppXmlDocumentControl* pCtrl,
                                          IN mxt_result res) = 0;


    //==========================================================================
    //==
    //==  EvDocumentPreview
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the server document preview.
    //
    //  Parameters:
    //    pCtrl:
    //      The interface that reported the event.
    //
    //    pDocument:
    //      The server document preview. Can be NULL if the preview mechanism
    //      failed to create the preview document.
    //
    //  Description:
    //      Reports the server document preview. It is reported after the
    //      application calls IUaImppXmlDocumentControl::GetDocumentPreview or
    //      after EvSynchronizeStateResult reports a resSW_WARNING result.
    //
    //  See also:
    //      IUaImppXmlDocumentControl::GetDocumentPreview
    //
    //==========================================================================
    virtual void EvDocumentPreview(IN IUaImppXmlDocumentControl* pCtrl,
                                   IN IEComUnknown* pDocument) = 0;


    //==========================================================================
    //==
    //==  EvSynchronizeFromServerResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of the SynchronizeFromServer method.
    //
    //  Parameters:
    //    pCtrl:
    //      The interface that reported the event.
    //
    //    res:
    //      The result of the action.
    //
    //  Description:
    //      Reports the result of the SynchronizeFromServer method. It is only
    //      reported after the application calls
    //      IUaImppXmlDocumentControl::SynchronizeFromServer.
    //
    //  See also:
    //      IUaImppXmlDocumentControl::SynchronizeFromServer
    //
    //==========================================================================
    virtual void EvSynchronizeFromServerResult(IN IUaImppXmlDocumentControl* pCtrl,
                                               IN mxt_result res) = 0;


    //==========================================================================
    //==
    //==  EvSynchronizeToServerResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of the SynchronizeToServer method.
    //
    //  Parameters:
    //    pCtrl:
    //      The interface that reported the event.
    //
    //    res:
    //      The result of the action.
    //
    //  Description:
    //      Reports the result of the SynchronizeToServer method. It is only
    //      reported after the application calls
    //      IUaImppXmlDocumentControl::SynchronizeToServer.
    //
    //  See also:
    //      IUaImppXmlDocumentControl::SynchronizeToServer
    //
    //==========================================================================
    virtual void EvSynchronizeToServerResult(IN IUaImppXmlDocumentControl* pCtrl,
                                             IN mxt_result res) = 0;

    //==========================================================================
    //==
    //==  EvDeleteServerDocumentResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of the DeleteServerDocument method.
    //
    //  Parameters:
    //    pCtrl:
    //      The interface that reported the event.
    //
    //    res:
    //      The result of the action.
    //
    //  Description:
    //      Reports the result of the DeleteServerDocument method. It is only
    //      reported after the application calls
    //      IUaImppXmlDocumentControl::DeleteServerDocument.
    //
    //  See also:
    //      IUaImppXmlDocumentControl::DeleteServerDocument
    //
    //==========================================================================
    virtual void EvDeleteServerDocumentResult(IN IUaImppXmlDocumentControl* pCtrl,
                                              IN mxt_result res) = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods.
protected:

    // Summary:
    //  Default Constructor.
    IUaImppXmlDocumentControlMgr() {}

    // Summary:
    //  Destructor.
    virtual ~IUaImppXmlDocumentControlMgr() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaImppXmlDocumentControlMgr(IN const IUaImppXmlDocumentControlMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaImppXmlDocumentControlMgr& operator=(IN const IUaImppXmlDocumentControlMgr& rSrc);


//-- Hidden Data Members.
protected:
private:

//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUAIMPPXMLDOCUMENTCONTROLMGR_H

