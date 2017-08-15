//==============================================================================
//==============================================================================
//
//              Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_IASYNCTLSOPENSSL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCTLSOPENSSL_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
    #include "Config/FrameworkCfg.h"    // MXD_TLS_OPENSSL 
#endif


//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#if defined (MXD_TLS_OPENSSL)

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncTlsOpenSsl);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncTlsOpenSsl
//==============================================================================
//<GROUP TLS_CLASSES>
//
// Summary:
//    Interface defining the basic methods accessible on asynchronous TLS
//    sockets using the openSsl implementation.
//
// Description:
//    Interface defining the most basic methods accessible on asynchronous TLS
//    sockets using the openSsl implementation.
//
// Location:
//   Tls/IAsyncTlsOpenSsl.h
//
//==============================================================================
class IAsyncTlsOpenSsl : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncTlsOpenSsl);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  GetSsl
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the OpenSsl SSL object for this socket.
    //
    //  Parameters:
    //      ppSsl:
    //          The OpenSsl SSL object's pointer when resS_OK is returned.
    //
    //  Returns:
    //      - resS_OK: The SSL object has successfully been retrieved.
    //      - resFE_INVALID_ARGUMENT: ppSsl is NULL.
    //      - resFE_INVALID_STATE: The OpenSsl SSL object is not available.
    //                             This occurs before the TLS connection is
    //                             established.
    //
    //  Description:
    //      Gets the OpenSsl SSL object for this socket.
    //
    //==========================================================================
    virtual mxt_result GetSsl(OUT const SSL** ppSsl) const = 0;


//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    //==========================================================================
    //==
    //==  IAsyncTlsOpenSsl
    //==
    //==========================================================================
    //
    //  Summary:
    //    Default Constructor.
    //
    //  Description:
    //    Default Constructor.
    //
    //==========================================================================
    IAsyncTlsOpenSsl() {};

    //==========================================================================
    //==
    //==  ~IAsyncTlsOpenSsl
    //==
    //==========================================================================
    //
    //  Summary:
    //    Destructor.
    //
    //  Description:
    //    Destructor.
    //
    //==========================================================================
    virtual ~IAsyncTlsOpenSsl() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    IAsyncTlsOpenSsl(IN const IAsyncTlsOpenSsl& rSrc);
    IAsyncTlsOpenSsl& operator=(IN const IAsyncTlsOpenSsl& rSrc);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined (MXD_TLS_OPENSSL)

#endif //-- #ifndef MXG_IASYNCTLSOPENSSL_H

