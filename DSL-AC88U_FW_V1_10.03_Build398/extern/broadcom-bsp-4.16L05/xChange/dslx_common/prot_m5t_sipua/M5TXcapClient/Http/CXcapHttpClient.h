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
#ifndef CXCAPHTTPCLIENT_H
//M5T_INTERNAL_USE_BEGIN
#define CXCAPHTTPCLIENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_XCAPCLIENTCFG_H
#include "Config/XcapClientCfg.h"
#endif

#if !defined(MXD_XCAP_LIBCURL_ENABLE_SUPPORT)

#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#ifndef MXG_IXCAPHTTPCLIENT_H
#include "Http/IXcapHttpClient.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CMimeObject;
class CTlsContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CXcapHttpClient
//==============================================================================
//<GROUP CXcapHttpClient>
//
// Summary:
//   This class is the empty implementation of the simple abstraction layer for
//   XCAP.
//
// Description:
//   This class is the empty implementation of the interface defined in
//   IXcapHttpClient.h. This implementation is used only if
//   MXD_XCAP_LIBCURL_ENABLE_SUPPORT is not defined in PreXcapClientCfg.h and
//   MUST be implemented by the customer to support his own HTTP stack. 
//
// #Note:#
//   This class is not implemented, it is only a skeleton. The user needs to
//   implement this class in order to use the Xcap client's functionality.
//
//   A Curl HTTP stack implementation is also available if
//   MXD_XCAP_LIBCURL_ENABLE_SUPPORT is defined in PreXcapClientCfg.h.
//
// Location:
//   Http/CXcapHttpClient.h
//
//==============================================================================
class CXcapHttpClient : protected CEComUnknown,
                        protected IXcapHttpClient
{
//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:

    //-- << Initialization mechanism >>
    static mxt_result InitializeCXcapHttpClient();
    static void FinalizeCXcapHttpClient();


    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComUnknown >>
    MX_DECLARE_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << IXCapHttpClient >>

    virtual mxt_result Activate(IN IEComUnknown* pThread);

    virtual mxt_result SetManager(IN IXcapHttpClientMgr* pMgr);

    virtual void SetProxy(IN const char* pszAddress,
                          IN unsigned short uPort,
                          IN bool bUseTls);

    virtual mxt_result SendGet(IN mxt_opaque opqRequest,
                               IN const char* pszAbsoluteUrl,
                               IN const char* pszIfNoneMatchHeaderValue);

    virtual mxt_result SendPut(IN mxt_opaque opqRequest,
                               IN const char* pszAbsoluteUrl,
                               IN TOA CMimeObject* pMimeObject,
                               IN const char* pszIfMatchHeaderValue,
                               IN const char* pszIfNoneMatchHeaderValue);

    virtual mxt_result SendDelete(IN mxt_opaque opqRequest,
                                  IN const char* pszAbsoluteUrl,
                                  IN const char* pszIfMatchHeaderValue);

    virtual mxt_result Authenticate(IN const char* pszRealm,
                                    IN const char* pszUserName,
                                    IN const char* pszA1Hash);

    virtual void SetTlsContext(IN TOA CTlsContext* pTlsContext);

    virtual void SetRedirectionFollowup(IN unsigned int uMaxNumRedirection);


//-- Hidden Methods
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CXcapHttpClient(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CXcapHttpClient();

    //-- << Other methods >>

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CXcapHttpClient(IN const CXcapHttpClient& rSrc);

    // Summary:
    //  Assignment Operator.
    CXcapHttpClient& operator=(IN const CXcapHttpClient& rSrc);


//-- Hidden Data Members
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if !defined(MXD_XCAP_LIBCURL_ENABLE_SUPPORT)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef CXCAPHTTPCLIENT_H

