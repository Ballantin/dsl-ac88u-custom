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
#ifndef MXG_CRESOLVERCACHE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CRESOLVERCACHE_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT
                                 // MXD_RESOLVER_CACHE_ENABLE_SUPPORT
#endif

#if defined (MXD_RESOLVER_CACHE_ENABLE_SUPPORT) && defined (MXD_CAP_ENABLE_SUPPORT)

#ifndef MXG_CAATREE_H
#include "Cap/CAATree.h"
#endif

#ifndef MXG_CDNSPACKET_H
#include "Resolver/CDnsPacket.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CAsyncResolver;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CResolverCache
//==============================================================================
//<GROUP RESOLVER_CLASSES>
//
// Summary:
//   This class provides an abstraction to a DNS resolver cache.
//
// Description:
//   This class is a DNS resolver cache. It complies to RFCs 1035 and 2308. The
//   cache data structure is two-fold: a tree for efficient search and an LRU
//   list for removal of items when the cache is full. Each leaf in the tree is
//   chained in a double-linked LRU list, in order to remove the least recently
//   used cached answer when the cache is full. The capacity of the cache is
//   defined by MXD_RESOLVER_CACHE_CAPACITY.
//
//   Note that the cache cannot be instantiated by any other means than through
//   the asynchronous resolver.
//
//   The following diagram illustrates the cache data structure:
//  <CODE>
//  legend:
//        * : tree structure
//       -> : links in the list and to nodes in the tree
//       <=>: this shows that elements are the same (e.g. A1 in the tree
//            occupies the same memory space as A1 in the linked list.
//
//                                        +----+
//        ++=============================>| A1 |
//        ||                              +----+
//        ||                               *  *
//        ||                        ********  *******
//        ||                        *               *
//        ||                     +----+           +----+
//        ||          ++========>| A2 |    ++====>| A4 |
//        ||          ||         +----+    ||     +----+
//        ||          ||          *        ||
//        ||          ||   ********        ||
//        ||          ||   *               ||
//        ||          || +----+            ||
//        ||          || | A3 |<=++        ||
//        ||          || +----+  ||        ||
//        ||          ||         ||        ||
//        V           V          V         V
//      +----+ <-- +----+ <-- +----+ <-- +----+
//      | A1 |     | A2 |     | A3 |     | A4 |
//      +----+ --> +----+ --> +----+ --> +----+
//      Newest                           Oldest
//  </CODE>
//
// Location:
//   Resolver/CResolverCache.h
//
//==============================================================================
class CResolverCache
{
//-- Friend Declarations
friend class CAsyncResolver;

//-- Published Interface
public:
    // Summary:
    //   Cache sorting method.
    static int Compare(IN const CUncmp<CDnsPacket>& rLhs, IN const CUncmp<CDnsPacket>& rRhs, IN mxt_opaque opq);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    // Summary:
    //  Default Constructor.
    CResolverCache();

    // Summary:
    //  Destructor.
    virtual ~CResolverCache();

    // Summary:
    //  Queries the cache for a response.
    mxt_result Query(IN CDnsPacket::SQuestion& rstQuestion, OUT CDnsPacket*& rpResponse);

    // Summary:
    //  Updates the cache with a response.
    mxt_result Update(IN CDnsPacket& rResponse);

    // Summary:
    //  Allows to store non-authoritative responses in cache.
    void EnableCacheNonAuthoritativeResponse();

    // Summary:
    //  Disallows to store non-authoritative responses in cache.
    void DisableCacheNonAuthoritativeResponse();

    // Summary:
    //  Clears the cache.
    void Clear();

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CResolverCache(IN const CResolverCache& rSrc);

    // Summary:
    //  Assignment Operator.
    CResolverCache& operator=(IN const CResolverCache& rSrc);


//-- Hidden Data Members
protected:
    // The cache data structure.
    CAATree<CUncmp<CDnsPacket> > m_cache;
    // The comparison item used for cache update.
    static CDnsPacket::SQuestion ms_stComparisonItem;
    // MRU end of the linked list.
    CUncmp<CDnsPacket>* m_pResolverCacheNewestItem;
    // LRU end of the linked list.
    CUncmp<CDnsPacket>* m_pResolverCacheOldestItem;
    // Cache use protection.
    CMutex m_resolverCacheMutex;
    // Whether or not to cache non-authoritative responses.
    bool m_bCacheNonAuthoritativeResponse;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//==
//==  EnableCacheNonAuthoritativeResponse
//==
//==============================================================================
//
//  Description:
//      Allows to store non-authoritative responses in cache. By default,
//      non-authoritative responses are cached.
//
//==============================================================================
inline void CResolverCache::EnableCacheNonAuthoritativeResponse()
{
    m_bCacheNonAuthoritativeResponse = true;
}

//==============================================================================
//==
//==  DisableCacheNonAuthoritativeResponse
//==
//==============================================================================
//
//  Description:
//      Disallows to store non-authoritative responses in cache. By default,
//      non-authoritative responses are cached.
//
//==============================================================================
inline void CResolverCache::DisableCacheNonAuthoritativeResponse()
{
    m_bCacheNonAuthoritativeResponse = false;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined (MXD_RESOLVER_CACHE_ENABLE_SUPPORT) && defined (MXD_CAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CRESOLVERCACHE_H


