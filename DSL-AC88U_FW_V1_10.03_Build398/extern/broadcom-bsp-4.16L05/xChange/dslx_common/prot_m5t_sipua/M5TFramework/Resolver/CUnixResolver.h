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
#ifndef MXG_CUNIXRESOLVER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUNIXRESOLVER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT
                                 // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_CAP_ENABLE_SUPPORT
                                 // MXD_SERVICING_THREAD_ENABLE_SUPPORT
#endif

#if !defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT) && \
    defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_CAP_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    (defined(MXD_OS_LINUX) || defined (MXD_OS_SOLARIS) || defined (MXD_OS_DARWIN))

#ifndef MXG_CResolverCore_H
#include "Resolver/CResolverCore.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUnixResolver
//==============================================================================
//
// Summary:
//   This is the Unix resolver core.
//
// Description:
//   This class implements the resolver core using the Unix DNS API.
//
//   Note: this API is supported by Linux and Solaris.
//
// Location:
//   Resolver/CUnixResolver.h
//
//==============================================================================
class CUnixResolver : public CResolverCore
{
    //-- Friend Declarations
    friend class CAsyncResolver;

//-- Hidden Methods
protected:

    // Summary:
    //  Default Constructor.
    CUnixResolver();

    // Summary:
    //  Destructor.
    virtual ~CUnixResolver();

    // -- << Linux API Resolver Core >>.
    mxt_result CompressName(IN CString& rstrName, INOUT void* pBuffer);
    mxt_result ExpandName(INOUT void* pBuffer, OUT CString& rstrName);
    void Send(IN mxt_opaque opq);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUnixResolver(IN const CUnixResolver& rSrc);

    // Summary:
    //  Assignment Operator.
    CUnixResolver& operator=(IN const CUnixResolver& rSrc);

};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if !defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT) &&
       //--     defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //--     defined(MXD_CAP_ENABLE_SUPPORT) &&
       //--     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //--     (defined(MXD_OS_LINUX) || defined (MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN))

//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CUNIXRESOLVER_H


