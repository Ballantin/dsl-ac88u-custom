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
#ifndef MXG_IASYNCTLSSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCTLSSOCKET_H
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
class CTlsSession;
class IAsyncTlsSocketMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncTlsSocket);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncTlsSocket
//==============================================================================
//<GROUP TLS_CLASSES>
//
// Summary:
//    Interface defining the basic methods accessible on asynchronous TLS
//    sockets.
//
// Description:
//    Interface defining the most basic methods accessible on asynchronous TLS
//    sockets. This means retrieving and setting the TLS Context, setting
//    the TLS Session, setting the TLS asynchronous socket's manager, and
//    completing the peer credentials approval.
//
//    Events related to this interface are reported through the
//    IAsyncTlsSocketMgr interface.
//
// Location:
//   Tls/IAsyncTlsSocket.h
//
// See Also:
//   IAsyncTlsSocketMgr, CAsyncTlsSocket
//
//==============================================================================
class IAsyncTlsSocket : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncTlsSocket);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  GetTlsContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a copy of the CTlsContext used by this context.
    //
    //  Parameters:
    //   pTlsContext:
    //    Pointer to a CTlsContext object that will receive the copy.
    //
    //  Returns:
    //      resS_OK: The CTlsContext has been successfully retrieved.
    //      resFE_INVALID_ARGUMENT: pTlsContext is NULL.
    //
    //  Description:
    //      Gets a copy of the CTlsContext used by this socket.
    //
    //==========================================================================
    virtual mxt_result GetTlsContext(OUT CTlsContext* pTlsContext) const = 0;

    //==========================================================================
    //==
    //==  SetTlsContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the current TLS context.
    //
    //  Parameters:
    //    pTlsContext:
    //      The new TLS context to use. It cannot be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: pTlsContext is NULL.
    //      - resFE_INVALID_STATE: A renegotiation is in progress.
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      This method sets the current TLS context. The TLS context is used
    //      for TLS handshaking. Thus, it is important to set it before any TLS
    //      action is performed. Failure to do so will result in an error being
    //      reported.
    //
    //==========================================================================
    virtual mxt_result SetTlsContext(IN const CTlsContext* pTlsContext) = 0;

    //==========================================================================
    //==
    //==  SetTlsSession
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the TLS session used for TLS handshaking.
    //
    //  Parameters:
    //    pTlsSession:
    //      The session used for TLS handshaking. It cannot be NULL
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: pTlsSession is NULL.
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      To initiate handskaking, the TLS protocol allows the client to send
    //      a TLS session identifier, which bypassses the key negotiation phase.
    //      This is a speed up trick. This call must be done prior to TLS
    //      handshaking to be effective.
    //
    //==========================================================================
    virtual mxt_result SetTlsSession(IN const CTlsSession* pTlsSession) = 0;

    //==========================================================================
    //==
    //==  SetAsyncTlsSocketMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the TLS socket's manager.
    //
    //  Parameters:
    //    pAsyncTlsSocketMgr:
    //      Socket manager to set. It cannot be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: pAsyncTlsSocketMgr is NULL.
    //      - resFE_INVALID_STATE: The TLS socket's manager is already set.
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      Sets the TLS socket's manager.
    //
    //==========================================================================
    virtual mxt_result SetAsyncTlsSocketMgr(IN IAsyncTlsSocketMgr* pAsyncTlsSocketMgr) = 0;

    //==========================================================================
    //==
    //==  TlsHandshakingApprovalCompletedA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies that the handshaking approval is complete.
    //
    //  Parameters:
    //    resApproval:
    //      mxt_result containing the approval.
    //
    //  Returns:
    //      - resS_OK: Always.
    //
    //  Description:
    //      Notifies that the handshaking approval is complete. This means that
    //      the application has validated the peer credentials. These
    //      credentials are provided by the
    //      IAsyncTlsSocketMgr::EvAsyncTlsSocketMgrTlsHandshakingCompletedAwaitingApproval
    //      event. In most cases, this method will be called from that event
    //      handler.
    //
    //==========================================================================
    virtual mxt_result TlsHandshakingApprovalCompletedA(IN mxt_result resApproval) = 0;

    //==========================================================================
    //==
    //==  GetUsedCipher
    //==
    //==========================================================================
    //
    //  Summary:
    //      Get the used cipher name.
    //
    //  Parameters:
    //    rstrCipherName:
    //      Parameter used to return the cipher name.
    //
    //  Returns:
    //      - resS_OK: Method succeeded and returned a value.
    //      - resFE_NOT_IMPLEMENTED: Method is not implemented for the SSL 
    //        library used.
    //      - resFE_INVALID_STATE: The SSL session is not in a valid state.
    //
    //  Description:
    //      Returns the cipher name used inside the SSL session.
    //
    //==========================================================================
    virtual mxt_result GetUsedCipher(OUT CString& rstrCipherName) const = 0;

    //==========================================================================
    //==
    //==  GetUsedCipherStrength
    //==
    //==========================================================================
    //
    //  Summary:
    //      Get the used cipher strength.
    //
    //  Parameters:
    //    ruCipherStrengthBits:
    //      Parameter used to return the cipher strength.
    //
    //  Returns:
    //      - resS_OK: Method succeeded and returned a value.
    //      - resFE_NOT_IMPLEMENTED: Method is not implemented for the SSL 
    //        library used.
    //      - resFE_INVALID_STATE: The SSL session is not in a valid state.
    //
    //  Description:
    //      Returns the cipher strength used inside the SSL session.
    //
    //==========================================================================
    virtual mxt_result GetUsedCipherStrength(OUT unsigned int& ruCipherStrengthBits) const = 0;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    //==========================================================================
    //==
    //==  IAsyncTlsSocket
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
    IAsyncTlsSocket() {};

    //==========================================================================
    //==
    //==  ~IAsyncTlsSocket
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
    virtual ~IAsyncTlsSocket() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    IAsyncTlsSocket(IN const IAsyncTlsSocket& rSrc);
    IAsyncTlsSocket& operator=(IN const IAsyncTlsSocket& rSrc);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IASYNCTLSSOCKET_H

