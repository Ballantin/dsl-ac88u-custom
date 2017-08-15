//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_IASYNCTLSSERVERSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCTLSSERVERSOCKET_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CTlsContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncTlsServerSocket);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncTlsServerSocket
//==============================================================================
//<GROUP TLS_CLASSES>
//
// Summary:
//    Interface defining the basic methods accessible on asynchronous TLS
//    server sockets.
//
// Description:
//    Interface defining the most basic methods accessible on asynchronous TLS
//    server sockets. This means the retrieving and setting of the TLS Context
//    that will be given to the accepted sockets.
//
// Location:
//   Tls/IAsyncTlsServerSocket.h
//
// See Also:
//   CAsyncTlsServerSocket
//
//==============================================================================
class IAsyncTlsServerSocket : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncTlsServerSocket);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  GetAcceptedTlsContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a copy of the CTlsContext that will be used for accepted TLS
    //      sockets.
    //
    //  Parameters:
    //   pAcceptedTlsContext:
    //    Pointer to a CTlsContext object that will receive the copy. It cannot
    //    be NULL.
    //
    //  Returns:
    //      resS_OK: The CTlsContext has been successfully retrieved.
    //      resFE_INVALID_ARGUMENT: pAcceptedTlsContext is NULL.
    //
    //  Description:
    //      Gets a copy of the CTlsContext that will be used for accepted TLS
    //      sockets.
    //
    //==========================================================================
    virtual mxt_result GetAcceptedTlsContext(OUT CTlsContext* pAcceptedTlsContext) const = 0;

    //==========================================================================
    //==
    //==  SetAcceptedTlsContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the current TLS context that will be used on accepted TLS
    //      sockets.
    //
    //  Parameters:
    //    pTlsContext:
    //      The new TLS context to use. It cannot be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: pTlsContext is NULL.
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      Sets the current TLS context that will be used on accepted TLS
    //      sockets.
    //
    //==========================================================================
    virtual mxt_result SetAcceptedTlsContext(IN const CTlsContext* pTlsContext) = 0;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    //==========================================================================
    //==
    //==  IAsyncTlsServerSocket
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
    IAsyncTlsServerSocket() {};

    //==========================================================================
    //==
    //==  ~IAsyncTlsServerSocket
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
    virtual ~IAsyncTlsServerSocket() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    IAsyncTlsServerSocket(IN const IAsyncTlsServerSocket& rSrc);
    IAsyncTlsServerSocket& operator=(IN const IAsyncTlsServerSocket& rSrc);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IASYNCTLSSERVERSOCKET_H

