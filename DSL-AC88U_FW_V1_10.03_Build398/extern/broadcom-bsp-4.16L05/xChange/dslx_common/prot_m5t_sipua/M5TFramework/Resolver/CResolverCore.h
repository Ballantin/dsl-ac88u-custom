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
#ifndef MXG_CRESOLVERCORE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CRESOLVERCORE_H
//M5T_INTERNAL_USE_END
 
//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_SERVICING_THREAD_ENABLE_SUPPORT,
                                 // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_CAP_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_CAP_ENABLE_SUPPORT)

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CDNSPACKET_H
#include "Resolver/CDnsPacket.h"
#endif

#ifndef MXG_CQUEUE_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CAsyncResolver;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//== Class: CResolverCore
//==============================================================================
//<GROUP RESOLVER_CLASSES>
//
// Summary:
//   This is the abstract base class of all resolver core implementations.
//
// Description:
//   This is the abstract base class of all resolver core implementations. It
//   provides:
//      - generic message processing and parsing
//      - use of the operating system's API asynchronously
//      - pending query cancellation
//      - batch response to identical questions: instead of queuing queries, it
//      keeps a list of notifications when questions are identical.
//
//   All but the portable resolver implementation use a synchronous DNS API to
//   perform DNS queries. In order to provide asynchronous DNS operations using
//   a synchronous API, CResolverCore queues the queries and then sends them
//   synchronously using the API. The portable resolver uses the queue but sends
//   them as they come in, since this implementation uses the Framework's
//   asynchronous socket API.
//
// Location:
//   Resolver/CResolverCore.h
//
//==============================================================================
class CResolverCore : protected CEventDriven
{
//-- New types within class namespace
protected:
//M5T_INTERNAL_USE_BEGIN
    enum EMessageId
    {
        eCANCEL,
        eQUERY,
        eSEND,
        eLAST // always need to be last for child to have a starting value.
    };
//M5T_INTERNAL_USE_END

    // Summary:
    //   This is the query item structure.
    struct STransaction
    {
        // The question that will make up the query.
        CDnsPacket::SQuestion* m_pstQuestion;
        // The query ID.
        uint16_t m_uId;
        // The list of opaques to return to the asynchronous resolver.
        CVector<mxt_opaque> m_vecopq;

#if defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT)
        // The currently used nameserver.
        unsigned int m_uNameServer;
        // The number of wraps made around the nameserver list.
        unsigned int m_uWraps;
#endif

        // Summary:
        //  Default Constructor.
        STransaction()
         :   m_uId(0)
        {
        }
    };

//-- Published Interface
public:
    // Summary:
    //   Cancels a pending query.
    virtual void CancelA(IN CDnsPacket::SQuestion& rstQuestion, IN mxt_opaque opq);

    // Summary:
    //   Schedules a query to be sent on the network.
    virtual mxt_result QueryA(IN CDnsPacket::SQuestion& rstQuestion, IN mxt_opaque opq);

    //-- << CEventDriven >>.

    // Summary:
    //   Activates the resolver core.
    virtual mxt_result Activate(IN IEComUnknown* pResolverCoreThread);

    // Summary:
    //   Returns a pointer to the Servicing Thread. AddIfRef is already called.
    virtual mxt_result GetServicingThread(OUT IEComUnknown** ppEComUnknown) const;

    // Summary:
    //   Frees the resolver core's resources.
    virtual void Release();

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CResolverCore();

    // Summary:
    //  Destructor.
    virtual ~CResolverCore();

    //-- << DNS API interface >>.

    //=====================================================================
    //==
    //==  Send
    //==
    //=====================================================================
    //
    //  Parameters:
    //      opq:
    //          opaque value related to the query to send.
    //
    //  Summary:
    //      Sends a query on the network.
    //
    //  Description:
    //      Sends a query on the network.
    //
    //=====================================================================
    virtual void Send(IN mxt_opaque opq) = 0;

    // Summary:
    //   Dequeues a pending query.
    virtual bool Dequeue(OUT CVector<mxt_opaque>& rvecopq);
    // Summary:
    //   Enqueues a pending query.
    virtual bool Enqueue(IN TO CDnsPacket::SQuestion* pstQuestion, IN mxt_opaque opq);

    //-- << Packet parsing >>.

    // Summary:
    //   Compresses a domain name.
    virtual mxt_result CompressName(IN CString& rstrName, INOUT void* pBuffer);
    // Summary:
    //   Expands a domain name.
    virtual mxt_result ExpandName(INOUT void* pBuffer, OUT CString& rstrName);
    // Summary:
    //   Parses a DNS packet.
    virtual mxt_result Parse(IN void* pBuffer, OUT CDnsPacket& rResponse);
    // Summary:
    //   Parses an A record.
    TO CSocketAddr* ParseARecord(IN void* pBuffer);
    // Summary:
    //   Parses a AAAA record.
    TO CSocketAddr* ParseAAAARecord(IN void* pBuffer);
    // Summary:
    //   Parses a CNAME record.
    TO CString* ParseCnameRecord(IN void* pBuffer);
    // Summary:
    //   Parses a DNS packet header.
    virtual mxt_result ParseHeader(INOUT void* pBuffer, OUT CDnsPacket& rResponse);
    // Summary:
    //   Parses an NAPTR record.
    TO CDnsPacket::SNaptrRecordData* ParseNaptrRecord(IN void* pBuffer);
    // Summary:
    //   Parses an NS record.
    TO CString* ParseNsRecord(IN void* pBuffer);
    // Summary:
    //   Parses a question record.
    virtual TO CDnsPacket::SQuestion* ParseQuestion(INOUT void* pBuffer);
    // Summary:
    //   Parses a PTR record.
    TO CString* ParsePtrRecord(IN void* pBuffer);
    // Summary:
    //   Parses a response record.
    virtual TO CDnsPacket::SRecord* ParseRecord(INOUT void* pBuffer, IN uint64_t uTtlBaselineMs);
    // Summary:
    //   Parses an SOA record.
    TO CDnsPacket::SSoaRecordData* ParseSoaRecord(IN void* pBuffer);
    // Summary:
    //   Parses an SRV record.
    TO CDnsPacket::SSrvRecordData* ParseSrvRecord(IN void* pBuffer);

    //-- << CEventDriven >>.

    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    virtual void InternalCancelA(IN CMarshaler* pParams);
    virtual void InternalQueryA(IN CMarshaler* pParams);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CResolverCore(IN const CResolverCore& rSrc);

    // Summary:
    //  Assignment Operator.
    CResolverCore& operator=(IN const CResolverCore& rSrc);


//-- Hidden Data Members
protected:
    // The query queue. This is in fact a vector, since query cancellation needs
    // random access to the queue.
    CVector<STransaction*> m_vecpstTransactions;

    // Flag stating that a query is in progress.
    bool m_bQueryInProgress;

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     defined(MXD_CAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CRESOLVERCORE_H


