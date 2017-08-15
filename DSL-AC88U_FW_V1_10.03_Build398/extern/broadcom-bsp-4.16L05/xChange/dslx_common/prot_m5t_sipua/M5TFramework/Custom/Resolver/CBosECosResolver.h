/**********************************************************************************
** Copyright © 2008 Broadcom
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This file defines the ECOS resolver core.
**
***********************************************************************************/

#ifndef MXG_CBOSECOSRESOLVER_H
#define MXG_CBOSECOSRESOLVER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT
                                 // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_CAP_ENABLE_SUPPORT
                                 // MXD_SERVICING_THREAD_ENABLE_SUPPORT
#endif

#if !defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT) && \
    defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_CAP_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_OS_BOS_ECOS)

#ifndef MXG_CResolverCore_H
#include "Resolver/CResolverCore.h"
#endif

#include <dns.h>

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CBosECosResolver
//==============================================================================
//<GROUP Resolver>
//
// Summary:
//   This is the eCos resolver core.
//
// Description:
//   This class implements the resolver core using the eCos DNS API.
//
//   Note: this API is supported by eCos.
//
// Location:
//   Custom/Resolver/CBosECosResolver.h
//
//==============================================================================
class CBosECosResolver : public CResolverCore
{
    //-- Friend Declarations
    friend class CAsyncResolver;

//M5T_INTERNAL_USE_BEGIN
//-- New types within class namespace
public:
    struct SHostRecord
    {
        int m_nNumIpAddr;
        BOS_IP_ADDRESS* m_pIpAddrs;
    };

    struct SResRecord
    {
        CDnsPacket::EType m_eType;
        CString m_strName;
        void* m_pvoidData;
    };
    
//-- Hidden Methods
protected:

    // Summary:
    //  Default Constructor.
    CBosECosResolver();

    // Summary:
    //  Destructor.
    virtual ~CBosECosResolver();

    // -- << eCos API Resolver Core >>.
    mxt_result Parse(IN void* pBuffer, OUT CDnsPacket& rResponse);
    TO CSocketAddr* ParseARecord(IN void* pBuffer);
    TO CSocketAddr* ParseAAAARecord(IN void* pBuffer);
    TO CDnsPacket::SNaptrRecordData* ParseNaptrRecord(IN void* pBuffer);
    TO CString* ParsePtrRecord(IN void* pBuffer);
    TO CDnsPacket::SSrvRecordData* ParseSrvRecord(IN void* pBuffer);
    void Send(IN mxt_opaque opq);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CBosECosResolver(IN const CBosECosResolver& rSrc);

    // Summary:
    //  Assignment Operator.
    CBosECosResolver& operator=(IN const CBosECosResolver& rSrc);

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if !defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT) &&
       //--     defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //--     defined(MXD_CAP_ENABLE_SUPPORT) &&
       //--     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //--     defined(MXD_OS_BOS_ECOS)

#endif //-- #ifndef MXG_CBOSECOSRESOLVER_H


