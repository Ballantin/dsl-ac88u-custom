//==============================================================================
//==============================================================================
//
//      Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPOPTIONTAGSSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPOPTIONTAGSSVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT \
    to be able to use this class.
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class ISipOptionTagsMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipOptionTagsSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipOptionTagsSvc
//========================================
// <GROUP SIPCORESVC_CLASSES>
// <FLAG New in 4.1.8>
//
// Description:
//   The option tags service when attached to a ISipContext is used to
//   automatically add the Supported headers in outgoing requests and 2xx
//   responses. Also, it handles the incoming requests Require headers and
//   automatically answers a 420 Bad Extension if a Required tags is not
//   currently supported.
//
//
//#The ISipOptionTagsSvc is an ECOM object#
//
//  The ISipOptionTagsSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_ISipOptionTagsSvc</B>
//
//  <B>Interface Id: IID_ISipOptionTagsSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipOptionTagsSvc.h
//
//==============================================================================
class ISipOptionTagsSvc : public IEComUnknown
{
//-- Friend Declarations.

//-- Published Interface.
public:

    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipOptionTagsSvc);
    //M5T_INTERNAL_USE_END

    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the manager associated with the service.
    //
    //  Parameters:
    //    pMgr:
    //      The manager to set. If NULL, the service will still take actions,
    //      but will not report them.
    //
    //  Description:
    //      Sets the manager associated with the service.
    //
    //==========================================================================
    virtual void SetManager(IN ISipOptionTagsMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetCustomSupportedExtensions
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets custom supported extensions.
    //
    //  Parameters:
    //    plststrCustomExtensions:
    //      The custom extensions to set on this service. Can be NULL, in
    //      which case the default extensions set on the ISipCoreConfig
    //      interface will be used.
    //
    //  Description:
    //      Sets custom supported extensions. This will replace the old custom
    //      extension list if any was set.
    //
    //  #Warning:#
    //      When a custom extension list is set, the service does not use
    //      the configured global extension list. If the service must also
    //      support them, they must be within the list or set, one after the
    //      others, using AddSupportedExtension.
    //
    //  See Also:
    //      AddSupportedExtension, RemoveSupportedExtension
    //
    //==========================================================================
    virtual void SetCustomSupportedExtensions(IN TOA CList<CString>* plststrCustomExtensions) = 0;

    //==========================================================================
    //==
    //==  AddSupportedExtension
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a custom supported extension.
    //
    //  Parameters:
    //    rstrExtension:
    //      The custom extension to add.
    //
    //  Returns:
    //     resS_OK: The extension was added.
    //     resFE_FAIL: The extension was already present.
    //
    //  Description:
    //      Adds a custom supported extension to the list.
    //
    //  #Warning:#
    //      When a custom extension is set, the service does not use
    //      the configured global extension list. If the service must also
    //      support them, they must be set, one after the others, using this
    //      method.
    //
    //  See Also:
    //      RemoveSupportedExtension
    //
    //==========================================================================
    virtual mxt_result AddSupportedExtension(IN const CString& rstrExtension) = 0;

    //==========================================================================
    //==
    //==  RemoveSupportedExtension
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes a custom supported extension.
    //
    //  Parameters:
    //    rstrExtension:
    //      The custom extension to remove.
    //
    //  Returns:
    //     resS_OK: The extension was removed.
    //     resFE_FAIL: THe extension was not found.
    //
    //  Description:
    //      Removes a custom supported extension from the list. To remove all
    //      the custom extensions at the same time, use
    //      SetCustomSupportedExtensions.
    //
    //  #Warning:#
    //      When one or more custom extension is set, the service does not use
    //      the configured global extension list. If the service must also
    //      support them, they must be set, one after the others, using this
    //      AddSupportedExtension.
    //
    //  See Also:
    //      SetCustomSupportedExtensions, AddSupportedExtension
    //
    //==========================================================================
    virtual mxt_result RemoveSupportedExtension(IN const CString& rstrExtension) = 0;


    // Summary:
    //  Enumeration defining the incoming and outgoing behavior of the service
    //  about which packets to handle.
    enum EBehavior
    {
        // Description:
        //  No packet will be checked, effectively disabling the service.
        //  Valid for both incoming and outgoing behaviors.
        eBEHAVIOR_HANDLE_NO_PACKET,

        // Description:
        //  Only requests will be checked. 
        //  Valid for both incoming and outgoing behaviors.
        eBEHAVIOR_HANDLE_REQUESTS_ONLY,

        // Description:
        //  Requests and 2xx responses will be checked.
        //  Only valid for an outgoing behavior.
        //  If set for an incoming behavior, it will have the same behavior
        //  as eBEHAVIOR_HANDLE_REQUESTS_ONLY.
        eBEHAVIOR_HANDLE_REQUESTS_AND_2XX_RESPONSES,
    };

    //==========================================================================
    //==
    //==  SetSupportedHeaderAddition
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the outgoing supported header addition behavior.
    //
    //  Parameters:
    //    eOutgoingBehavior:
    //      The behavior to set. This is an outgoing behavior.
    //
    //  Description:
    //      Sets the outgoing supported header addition behavior. This will tell
    //      the service in which outgoing packets to add the supported headers.
    //
    //      Default is eBEHAVIOR_HANDLE_REQUESTS_AND_2XX_RESPONSES.
    //
    //==========================================================================
    virtual void SetSupportedHeaderAddition(IN EBehavior eOutgoingBehavior) = 0;

    //==========================================================================
    //==
    //==  SetRequireHeaderVerification
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the require header verification behavior.
    //
    //  Parameters:
    //    eIncomingBehavior:
    //      The behavior to set. This is an incoming behavior.
    //
    //  Description:
    //      Sets the incoming require header verification behavior. This will
    //      tell the service in which incoming packets to verify the require
    //      headers against the local supported extensions.
    //
    //      Default is eBEHAVIOR_HANDLE_REQUESTS_ONLY.
    //
    //==========================================================================
    virtual void SetRequireHeaderVerification(IN EBehavior eIncomingBehavior) = 0;


//-- Hidden Methods.
protected:
    // Summary:
    //  Default Constructor.
    ISipOptionTagsSvc() {}

    // Summary:
    //  Destructor.
    virtual ~ISipOptionTagsSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipOptionTagsSvc(IN const ISipOptionTagsSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipOptionTagsSvc& operator=(IN const ISipOptionTagsSvc& rSrc);
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPOPTIONTAGSSVC_H

