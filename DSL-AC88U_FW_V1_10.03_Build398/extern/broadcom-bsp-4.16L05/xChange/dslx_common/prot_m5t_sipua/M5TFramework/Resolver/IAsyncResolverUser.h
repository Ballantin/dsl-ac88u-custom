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
#ifndef MXG_IASYNCRESOLVERUSER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCRESOLVERUSER_H
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
                                 // MXD_ENUM_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_CAP_ENABLE_SUPPORT)

#ifndef MXG_CDNSPACKET_H
#include "Resolver/CDnsPacket.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CString;

//==============================================================================
//== Class: IAsyncResolverUser
//==============================================================================
//<GROUP RESOLVER_CLASSES>
//
// Summary:
//   Interface of the asynchronous resolver user.
//
// Description:
//   This interface needs to be implemented by users of the asynchronous
//   resolver, in order to get the answers to their DNS requests.
//
// Location:
//   Resolver/IAsyncResolverUser.h
//
// See also:
//  CAsyncResolver.
//
//==============================================================================
class IAsyncResolverUser
{
//-- Published Interface
public:
    //==========================================================================
    //==
    //==  EvAsyncResolverUserResponseReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notification of the reception of a response to either an A or AAAA
    //      question.
    //
    //  Parameters:
    //      rlstAddresses:
    //       The list of addresses associated with the queried name.
    //
    //      opq:
    //       The opaque parameter passed as an argument.
    //
    //  Description:
    //      Notifies the user that either an A or AAAA question has been
    //      answered. It may contain both IPv4 and IPv6 addresses, depending on
    //      the DNS database state. It may also be empty, meaning the DNS
    //      question could not be answered.
    //
    //==========================================================================
    virtual void EvAsyncResolverUserResponseReceived(IN CList<CSocketAddr>& rlstAddresses, IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  EvAsyncResolverUserResponseReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notification of the reception of a response to a PTR question.
    //
    //  Parameters:
    //      rlststrNames:
    //       The list of names associated with the queried address.
    //
    //      opq:
    //       The opaque parameter passed as an argument.
    //
    //  Description:
    //      Notifies the user that a PTR question has been answered. It may be
    //      empty, meaning the DNS question could not be answered.
    //
    //==========================================================================
    virtual void EvAsyncResolverUserResponseReceived(IN CList<CString>& rlststrNames, IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  EvAsyncResolverUserResponseReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notification of the reception of a response to an SRV question.
    //
    //  Parameters:
    //      rlstServices:
    //       The list of service records associated with the queried service.
    //
    //      opq:
    //       The opaque parameter passed as an argument.
    //
    //  Description:
    //      Notifies the user that an SRV question has been answered. It may be
    //      empty, meaning the DNS question could not be answered.
    //
    //==========================================================================
    virtual void EvAsyncResolverUserResponseReceived(IN CList<CDnsPacket::SSrvRecordData>& rlstServices,
                                                     IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  EvAsyncResolverUserResponseReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notification of the reception of a response to a NAPTR question.
    //
    //  Parameters:
    //      rlstPointers:
    //       The list of naming authority pointer records associated with the
    //       queried domain.
    //
    //      opq:
    //       The opaque parameter passed as an argument.
    //
    //  Description:
    //      Notifies the user that a NAPTR question has been answered. It may be
    //      empty, meaning the DNS question could not be answered.
    //
    //==========================================================================
    virtual void EvAsyncResolverUserResponseReceived(IN CList<CDnsPacket::SNaptrRecordData>& rlstPointers,
                                                     IN mxt_opaque opq) = 0;

#if defined(MXD_ENUM_ENABLE_SUPPORT)
    //==========================================================================
    //==
    //==  EvAsyncResolverUserResponseReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notification of the reception of a response to a NAPTR (ENUM)
    //      question.
    //
    //  Parameters:
    //      rlstUris:
    //       The list of ENUM URI records associated with the queried domain.
    //
    //      opq:
    //       The opaque parameter passed as an argument.
    //
    //  Description:
    //      Notifies the user that a NAPTR (ENUM) question has been answered. It
    //      may be empty, meaning the DNS question could not be answered.
    //
    //==========================================================================
    virtual void EvAsyncResolverUserResponseReceived(IN CList<CDnsPacket::SEnumUriData>& rlstUris, IN mxt_opaque opq) = 0;
#endif

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IAsyncResolverUser(){};

    // Summary:
    //  Destructor.
    virtual ~IAsyncResolverUser(){};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IAsyncResolverUser(IN const IAsyncResolverUser& rSrc);

    // Summary:
    //  Assignment Operator.
    IAsyncResolverUser& operator=(IN const IAsyncResolverUser& rSrc);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     defined(MXD_CAP_ENABLE_SUPPORT)
#endif //-- #ifndef MXG_IASYNCRESOLVERUSER_H


