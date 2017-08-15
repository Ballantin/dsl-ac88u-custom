//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPSESSIONTRANSACTIONUASBYE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSESSIONTRANSACTIONUASBYE_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CSIPSESSIONTRANSACTION_H
#include "SipUserAgent/CSipSessionTransaction.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipSessionTransactionUasBye
//==============================================================================
//<GROUP SipUserAgent>
//
// Summary:
//   The transaction used to handle a server BYE request.
//
// Description:
//  Handles a server BYE transaction from the reception of the BYE to the
//  sending of the final response.
//
//  The responsibilities of this class are:
//  - Manage the server transaction state
//  - Report events related to such transaction on the ISipSessionMgr
//  - Manage the ISipUserAgentSvc
//  - Create and manage the BYE's ISipRequestContext for its lifetime
//  - Report the dialog state changes to the ISipSessionTransactionController
//
// Location:
//   SipUserAgent/CSipSessionTransactionUasBye.h
//
// See Also:
//   CSipSessionTransaction, CSipSessionSvc
//
//==============================================================================
class CSipSessionTransactionUasBye : private CEComDelegatingUnknown,
                                     private CSipSessionTransaction
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipSessionTransactionUasBye();

    static void FinalizeCSipSessionTransactionUasBye();

//-- Hidden Methods
protected:
private:
    //-- << Constructors / Destructors / Operators >>

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // waring or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif
//M5T_INTERNAL_USE_END

    // Summary:
    //  Default Constructor.
    CSipSessionTransactionUasBye(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipSessionTransactionUasBye();

    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //-- << CEComDelegatingUnknown methods >>

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << ISipSessionTransaction >>

    virtual mxt_result AllowSendingRequest(IN ESipMethod eMethod);

    virtual mxt_result AllowReceivingRequest(IN ESipMethod eMethod,
                                             IN const CSipPacket& rPacket,
                                             OUT uint16_t& ruResponseRejectedCode,
                                             OUT GO CHeaderList** ppResponseExtraHeaders);

    virtual void Clear();

    //-- << ISipReqCtxCoreSvc >>

    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket& rPacket,
                                    IN unsigned int uCode,
                                    IN const char* szReason,
                                    IN TOA CHeaderList* pExtraHeaders,
                                    IN TOA CSipMessageBody* pMessageBody);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipSessionTransactionUasBye(IN const CSipSessionTransactionUasBye& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipSessionTransactionUasBye& operator=(IN const CSipSessionTransactionUasBye& rSrc);


//-- Hidden Data Members
protected:
private:

    // The state of the final response being updated.
    enum EFinalResponseUpdateState
    {
        // No final response is currently being updated.
        eIDLE,
        // Final response is currently being updated.
        eUPDATING,
        // Final response is currently being updated and clear was called.
        eCLEAR_UPDATING
    };

    // The state of the final response being updated. This is used to avoid
    // calling EvTransactionTerminated on the controller when Clear is called
    // while a final response is buffered. This is to make sure that
    // EvTransactionTerminated is not called twice (it would also be in
    // OnPacketUpdated). We are NOT resetting m_pSessionTransactionController in
    // Clear because we want to call EvTerminateSessionDialog on it when
    // OnPacketUpdated is called if the updating response is a success.
    EFinalResponseUpdateState m_eUpdateState;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSIPSESSIONTRANSACTIONUASBYE_H

