//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPTRANSACTIONUSER_H
#define MXG_ISIPTRANSACTIONUSER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipTransactionUser
//========================================
//
// Description:
//  Representation of RFC 3261's Transaction User. Events for a transaction are 
//  reported to the SipCore through this interface. Transaction progress events 
//  for client and server transactions are generated on this interface.
//
// Location:
//   SipTransaction/ISipTransactionUser.h
//
// See Also:
//   ISipTransactionMgr
//
//==EDOC========================================================================
class ISipTransactionUser
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  EvRequestProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the Transaction User of a provisional response reception 
    //      for an outgoing request.
    //
    //  Parameters:
    //    rPacket:
    //      The packet received for an outgoing request.
    //
    //    opqTransaction:
    //      The handle of the transaction.
    //
    //  Description:
    //      Notifies the Transaction User of a provisional response reception 
    //      for an outgoing request.
    //
    //  See Also:
    //      EvRequestSuccess
    //
    //==EDOC====================================================================
    virtual void EvRequestProgress(IN const CSipPacket&    rPacket,
                                   IN mxt_opaque           opqTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  EvRequestSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the Transaction User that a 2xx response has been received
    //      for an outgoing request.
    //
    //  Parameters:
    //    rPacket:
    //      The packet received for an outgoing request.
    //
    //    opqTransaction:
    //      The handle of the transaction.
    //
    //  Description:
    //      Notifies the Transaction User that a 2xx response has been received
    //      for an outgoing request. In case of an INVITE transaction, the
    //      Transaction User has to send an ACK. The ACK must be sent through
    //      the SipTransport.
    //
    //  See Also:
    //      ISipTransportMgr::SendA
    //
    //==EDOC====================================================================
    virtual void EvRequestSuccess(IN const CSipPacket&    rPacket,
                                  IN mxt_opaque           opqTransaction) = 0;


    //==SDOC====================================================================
    //==
    //==  EvRequestFailed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the Transaction User that a final negative response has 
    //      been either received or generated locally.
    //
    //  Parameters:
    //    rPacket:
    //      The packet responsible for this event. This packet can ben either
    //      received by the remote party or locally generated.
    //
    //    opqTransaction:
    //      The handle of the transaction.
    //
    //    result:
    //      resFE_ABORT:
    //          An internal error has occurred.
    //
    //      resSI_SIPTRANSACTION_FINAL_NEGATIVE_RESPONSE:
    //          A final negative response has been received by the remote party.
    //
    //  Description:
    //      Notifies the Transaction User that a final negative response has 
    //      been either received or generated locally regarding the current
    //      transaction. In the first case,
    //      resSI_SIPTRANSACTION_FINAL_NEGATIVE_RESPONSE is returned, otherwise
    //      resFE_ABORT is returned.
    //
    //==EDOC====================================================================
    virtual void EvRequestFailed(IN const CSipPacket&    rPacket,
                                 IN mxt_opaque           opqTransaction,
                                 IN mxt_result           result) = 0;

    //==SDOC====================================================================
    //==
    //==  EvRequestCancelled
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the Transaction User that the transaction has been
    //      cancelled.
    //
    //  Parameters:
    //    rPacket:
    //      The CANCEL request.
    //
    //   opqTransaction:
    //     The handle of the transaction being cancelled.
    //
    //  Description:
    //      Notifies the Transaction User that the transaction has been 
    //      cancelled and that it has to create a new transaction for this 
    //      CANCEL.
    //
    //  See Also:
    //      ISipTransactionMgr::CreateTransaction
    //
    //==EDOC====================================================================
    virtual void EvRequestCancelled(IN const CSipPacket& rPacket,
                                    IN mxt_opaque opqTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  EvResponseSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the Transaction User that a response sending succeeded on a
    //      server transaction.
    //
    //  Parameters:
    //      opqTransaction:
    //       The handle of the transaction.
    //
    //  Description:
    //      Notifies the Transaction User that a response sending succeeded on a
    //      server transaction.
    //
    //      It is possible to receive a EvResponseFailed event after the success 
    //      event if a transport error occurs afterwards (TCP connection drops 
    //      or UDP ICMP error).
    //
    //==EDOC====================================================================
    virtual void EvResponseSuccess(IN mxt_opaque opqTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  EvResponseFailed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the Transaction User that a response sending failed on a
    //      server transaction.
    //
    //  Parameters:
    //      opqTransaction:
    //       The handle of the transaction.
    //
    //      result:
    //       The result describing the reason of the failure.
    //
    //  Description:
    //      Notifies the Transaction User upon an error reception for an 
    //      outgoing response sent on a server transaction.
    //
    //      It is possible to receive this event multiple times for a single
    //      send of a packet.
    //
    //==EDOC====================================================================
    virtual void EvResponseFailed(IN mxt_opaque opqTransaction,
                                  IN mxt_result result) = 0;

    //==========================================================================
    //==
    //==  EvServerTransactionTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the Transaction User that the transaction is terminated.
    //
    //  Parameters:
    //      opqTransaction:
    //       The handle of the transaction.
    //
    //  Description:
    //      Notifies the Transaction User that the server transaction object
    //      is now in the Terminated state.
    //
    //==========================================================================
    virtual void EvServerTransactionTerminated(IN mxt_opaque opqTransaction) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipTransactionUser(){};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipTransactionUser(){};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------


    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipTransactionUser(IN const ISipTransactionUser& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipTransactionUser& operator=(IN const ISipTransactionUser& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTRANSACTIONUSER_H

