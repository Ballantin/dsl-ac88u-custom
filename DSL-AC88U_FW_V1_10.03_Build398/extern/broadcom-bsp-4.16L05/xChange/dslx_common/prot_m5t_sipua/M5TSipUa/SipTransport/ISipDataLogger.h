//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPDATALOGGER_H
#define MXG_ISIPDATALOGGER_H

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
class CBlob;
class CSipPacket;
class CSocketAddr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipDataLogger
//========================================
// <GROUP SIPTRANSPORT_CLASSES>
//
// Description:
//   This is the interface that allows access to the SIP data that is being sent
//   to the network and received from the network. This data is usually used for
//   logging incoming and outgoing SIP traffic.
//
//   The SIP data is provided by the stack in two different formats.
//
//#Raw Data:#
//
//   Raw data is provided through the LogRawData API of this interface. This API
//   is called by the stack whenever data is actually sent on the network or
//   received from the network. The data is provided as a buffer of bytes
//   through a CBlob instance.
//
//   Users implementing this API must be aware that it is possible the raw data
//   can potentially contain non-printable characters. The payload SIP can
//   transport is not limited to text, it can very well be binary.
//
//#SIP Packet:#
//
//   The CSipPacket is provided through the LogSipPacket API of this interface.
//   This API is called by the stack on the following occasions:
//
//      - Upon the parsing of a complete and valid SIP packet and its payload. A
//      valid SIP packet has valid From, To, Call-Id, CSeq, and top Via headers.
//      In TCP, the packet must also have a valid Content-Length header. The SIP
//      packet must not be bigger than the configured maximal packet size, and
//      then same holds true for the payload. See
//      ISipCoreConfig::SetMaxReceivePacketSize and
//      ISipCoreConfig::SetMaxPayloadSize.
//
//      - When a SIP packet is associated with a new or existing connection and
//      it is being prepared to be sent on the network. Note that the packet is
//      traced <B>before</B> actually being sent. It is still possible that the
//      packet is not sent on the network, or it may be delayed before it is
//      sent.
//
//   One cannot assume that no packets were received when LogSipPacket is not
//   called, as packets with some level of syntax error are not logged through
//   this. Moreover, when LogSipPacket is called for an outgoing SIP packet, one
//   cannot assume that the packet is or will actually be sent on the network.
//   LogRawData should be used to have this level of precision.
//
// Location:
//   SipTransport/ISipDataLogger.h
//
//==EDOC========================================================================
class ISipDataLogger
{
//-- Friend Declarations
//------------------------

//-- New types within class namespace
//-----------------------------------
public:
    // This dictates the direction of the packet upon being logged.
    //--------------------------------------------------------------
    enum EDirection
    {
        // Received raw data or packet.
        //------------------------------
        eRECEIVED,

        // Sent raw data or packet.
        //--------------------------
        eSENT,

        // Packet sent in loopback.
        //--------------------------
        eLOOPBACK,
    };

protected:
private:

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipDataLogger(){};

    //==SDOC====================================================================
    //==
    //==  LogRawData
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports to the user that data has been received or sent.
    //
    //  Parameters:
    //      eDir:
    //       The direction of the packet being logged.
    //
    //      rLocalAddr:
    //       The local address associated with the raw data.
    //
    //      rPeerAddr:
    //       The peer address associated with the raw data.
    //
    //      rRawData:
    //       The raw data coming from or going on the network.
    //
    //      uSize:
    //       The size of the raw data.
    //
    //  Description:
    //      This method is called to let the implementing class log the raw data
    //      coming from or going on the network. In the case of data
    //      transmission, this method is called when the transmission is
    //      successful. It can be incomplete and then uSize is set accordingly.
    //
    //==EDOC====================================================================
    virtual void LogRawData(IN EDirection eDir,
                            IN const CSocketAddr& rLocalAddr,
                            IN const CSocketAddr& rPeerAddr,
                            IN const CBlob& rRawData,
                            IN unsigned int uSize) = 0;

    //==SDOC====================================================================
    //==
    //==  LogSipPacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports to the user that a SIP packet has been received or sent.
    //
    //  Parameters:
    //      eDir:
    //       The direction of the packet being logged.
    //
    //      rPacket:
    //       The packet to be logged.
    //
    //  Description:
    //      This method is called to let the implementing class log the packet.
    //      Note that when receiving a SIP packet, this is called only for
    //      complete and well-formed packets. See class description for more
    //      information.
    //
    //==EDOC====================================================================
    virtual void LogSipPacket(IN EDirection eDir,
                              IN const CSipPacket& rPacket) = 0;


//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipDataLogger(){};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipDataLogger(IN const ISipDataLogger& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipDataLogger& operator=(IN const ISipDataLogger& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPDATALOGGER_H


