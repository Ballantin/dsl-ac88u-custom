//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPASYNCSOCKETFACTORYCONFIGURATIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPASYNCSOCKETFACTORYCONFIGURATIONMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IASYNCSOCKETFACTORYCONFIGURATIONMGR_H
#include "Network/IAsyncSocketFactoryConfigurationMgr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IAsyncSocket;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipAsyncSocketFactoryConfigurationMgr
//==============================================================================
//
// Description:
//    This class implements a SIP Socket Factory configuration manager. It
//    implements the IAsyncSocketFactoryConfigurationMgr interface and is
//    automatically registered when calling CSipStackInitializer::Initialize().
//
//    The EvConfigurationRequested method configures SIP sockets. It MUST only
//    be called through the Socket Factory, via the
//    CAsyncSocketFactory::CallConfigurationMgr() method. Sockets need to be
//    configured after being created, and once accepted by a server socket.
//
//    This configuration manager handles configuration of SIP sockets over UDP,
//    TCP and TLS.
//
// Location:
//   SipTransport/CSipAsyncSocketFactoryConfigurationMgr.h
//
//==============================================================================
class CSipAsyncSocketFactoryConfigurationMgr : public IAsyncSocketFactoryConfigurationMgr
{
    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

public:
    // Summary:
    //  Default Constructor.
    CSipAsyncSocketFactoryConfigurationMgr();

    // Summary:
    //  Destructor.
    virtual ~CSipAsyncSocketFactoryConfigurationMgr();

    //-- Inherited from IAsyncSocketFactoryConfigurationMgr.
    mxt_result EvConfigurationRequested(IN const char* const* apszType,
                                        IN unsigned int uTypeSize,
                                        INOUT IAsyncSocket* pAsyncSocket);

private:
    //-- Startup mechanism.
    static mxt_result InitializeCSipAsyncSocketFactoryConfigurationMgr();
    static void FinalizeCSipAsyncSocketFactoryConfigurationMgr();
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPASYNCSOCKETFACTORYCONFIGURATIONMGR_H


