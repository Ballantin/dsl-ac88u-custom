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
#ifndef MXG_CUAIMPPXCAPCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPXCAPCONFIG_H

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
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEUSERCONFIGTERMINATION_H
#include "SceCoreComponents/ISceUserConfigTermination.h"
#endif

#ifndef MXG_IUAIMPPXCAPCONFIG_H
#include "SceUaImpp/IUaImppXcapConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//== Class: CUaImppXcapConfig
//========================================
//
// Description:
//  This class implements the user configuration options for Xcap.
//
// Location:
//   UaImpp/CUaImppXcapConfig.h
//
// See Also:
//   IUaImppXcapConfig
//
//==============================================================================
class CUaImppXcapConfig : protected CEComDelegatingUnknown,
                          protected IUaImppXcapConfig,
                          protected ISceUserConfigTermination
{
//-- Friend Declarations.
    friend class CSceUaImppInitializer;

//-- Published Interface.
public:


//-- Hidden Methods.
protected:

    //-- << IUaImppXcapConfig >>
    virtual mxt_result GetXcapClient(OUT IXcapClient** ppXcapClient);
    virtual mxt_result GetXcapDocumentSubscriber(OUT IUaImppXcapDocumentSubscriber** ppComponent);
    virtual mxt_result SetXcapNotifier(IN TOA CNameAddr* pResourceAor,
                                       IN TOA IUri* pNotifierRequestUri);
    virtual mxt_result GetXcapNotifier(OUT const CNameAddr** ppResourceAor,
                                       OUT const IUri** ppNotifierRequestUri);
    virtual mxt_result SetSubscriptionExpirationTime(IN uint32_t uNbSeconds);
    virtual uint32_t GetSubscriptionExpirationTime() const;

    //-- << ISceUserConfigTermination >>
    virtual void OnUserConfigTermination();

    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaImppXcapConfig();
    static void FinalizeCUaImppXcapConfig();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaImppXcapConfig(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CUaImppXcapConfig();


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaImppXcapConfig(IN const CUaImppXcapConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaImppXcapConfig& operator=(IN const CUaImppXcapConfig& rSrc);


//-- Hidden Data Members.
protected:
private:
    // This config's Xcap client.
    IXcapClient* m_pXcapClient;

    // This config's Xcap document subscriber.
    IUaImppXcapDocumentSubscriber* m_pXcapDocumentSubscriber;

    // The notifier's address of record.
    CNameAddr* m_pResourceAor;

    // The notifier's request uri.
    IUri* m_pNotifierRequestUri;

    // The subscription expiration time.
    uint32_t m_uSubscriptionExpirationTime;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)


#endif //-- #if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CUAIMPPXCAPCONFIG_H

//M5T_INTERNAL_USE_END
