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
#ifndef MXG_CSIPSESSIONTRANSACTION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSESSIONTRANSACTION_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPSESSIONTRANSACTION_H
#include "SipUserAgent/ISipSessionTransaction.h"
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
//== Class: CSipSessionTransaction
//==============================================================================
//<GROUP SipUserAgent>
//
// Summary:
//  Base class that offers method implementations that can be reused by the
//  session transactions.
//
// Description:
//  This class offers basic method implementations that can be reused by some
//  of the transaction child classes. This class is abstract and cannot be
//  instanciated at runtime.
//
// Location:
//   SipUserAgent/CSipSessionTransaction.h
//
// See Also:
//   ISipSessionTransactionMgr, CSipSessionSvc
//
//==============================================================================
class CSipSessionTransaction : protected CSipReqCtxCoreSvc,
                               protected ISipSessionTransaction
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipSessionTransaction();

    // Summary:
    //  Destructor.
    virtual ~CSipSessionTransaction();

    //-- << ISipSessionTransaction >>

    virtual void SetController(IN ISipSessionTransactionController* pSessionTransactionController);

    virtual void SetSessionDialogState(IN ISipSessionTransaction::ESessionDialogState eSessionDialogState);

    virtual void GetReqCtxCoreSvc(OUT ISipReqCtxCoreSvc*& rpReqCtxCoreSvc);

    virtual bool CanSendRequest(IN ESipMethod eMethod);

    virtual bool CanHandlePacket(IN ESipMethod eMethod,
                                 IN const CSipPacket& rPacket);

    virtual bool IsSessionDialogNeeded(IN ISipSessionTransactionController::ESessionDialogTerminationType eType);

    virtual mxt_result Invite(IN ISipForkedDialogGrouperMgr* pForkedDialogGrouperMgr,
                              IN mxt_opaque opqTransaction,
                              IN TOA CHeaderList* pExtraHeaders,
                              IN TOA CSipMessageBody* pMessageBody,
                              OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result Ack(IN TOA CHeaderList* pExtraHeaders,
                           IN TOA CSipMessageBody* pMessageBody);

    virtual mxt_result Bye(IN mxt_opaque opqTransaction,
                           IN TOA CHeaderList* pExtraHeaders,
                           IN TOA CSipMessageBody* pMessageBody,
                           OUT ISipClientTransaction*& rpTransaction);

    //-- << ISipReqCtxCoreSvc >>

    virtual unsigned int AddRcsRef();

    virtual unsigned int ReleaseRcsRef();

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipSessionTransaction(IN const CSipSessionTransaction& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipSessionTransaction& operator=(IN const CSipSessionTransaction& rSrc);


//-- Hidden Data Members
protected:
    // The transaction controller. Other ECom interfaces can be queried on this
    // pointer.
    ISipSessionTransactionController* m_pSessionTransactionController;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSIPSESSIONTRANSACTION_H

