//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2004-2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation. ("Media5")
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
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity
//   of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CINSTANCETRACKER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CINSTANCETRACKER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif
#if defined MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if defined MXD_SIPSTACK_ENABLE_SIPCONTEXT_INSTANCE_TRACKING         || \
    defined MXD_SIPSTACK_ENABLE_SIPPACKET_INSTANCE_TRACKING          || \
    defined MXD_SIPSTACK_ENABLE_SIPREQUEST_CONTEXT_INSTANCE_TRACKING || \
    defined MXD_SIPSTACK_ENABLE_SIPTRANSACTION_INSTANCE_TRACKING

#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPCONTEXT_INSTANCE_TRACKING || \
    defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPPACKET_INSTANCE_TRACKING || \
    defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING || \
    defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPTRANSACTION_INSTANCE_TRACKING
#error The standard instance tracking and the deprecated instance tracking are both enabled.
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CBlob_H
#include "Cap/CBlob.h"
#endif

#include "Kernel/CMemoryAllocator.h"
MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class ISipContext;
class ISipRequestContext;
class CSipPacket;
class CSipTransaction;
class ISipUserAgentSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
const size_t uINSTANCE_TRACKER_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS =  250;
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CInstanceTracker
//========================================
// <GROUP SIPTRANSPORT_CLASSES>
//
// Description:
//   This class gives static access to all watched instances.
//   Using this class it is possible to get information about still allocated
//   instances to help debug memory leaks. The following information can be
//   accessed using this class:
//
//      - The number of still allocated watched instances.
//      - A vector containg pointers to still allocated watched instances
//      - A CBlob object containing information about still allocated watched
//        instances.
//
// Location:
//   Tools/CInstanceTracker.h
//
//==EDOC========================================================================
class CInstanceTracker
{

//-- Published Interface
//------------------------
public:

#if defined MXD_SIPSTACK_ENABLE_SIPCONTEXT_INSTANCE_TRACKING
    // Summary:
    //  Gets the number of ISipContext still allocated.
    //--------------------------------------------------
    static unsigned int DebugGetNumContextsLeft();

    // Summary:
    //  Gets the vector of ISipContext still allocated.
    //--------------------------------------------------
    static uint16_t DebugGetContextTable(OUT CVector<ISipContext*>& rvecpContexts);

    // Summary:
    //  Gets a CBlob containing information about the ISipContext still
    //  allocated.
    //------------------------------------------------------------------
    static void DebugGetContextDump(OUT CBlob& rblobContextDump);
#endif // #if defined MXD_SIPSTACK_ENABLE_SIPCONTEXT_INSTANCE_TRACKING

    #if defined MXD_SIPSTACK_ENABLE_SIPPACKET_INSTANCE_TRACKING
    // Summary:
    //  Gets the number of CSipPacket still allocated.
    //-------------------------------------------------
    static unsigned int DebugGetNumPacketsLeft();

    // Summary:
    //  Gets the vector of CSipPacket still allocated.
    //-------------------------------------------------
    static unsigned int DebugGetPacketTable(OUT CVector<CSipPacket*>& rvecpPacket);

    // Summary:
    //  Gets a CBlob containing information about the CSipPacket still
    //  allocated.
    //-----------------------------------------------------------------
    static void DebugGetPacketDump(OUT CBlob& rblobPacketDump);
#endif // #if defined MXD_SIPSTACK_ENABLE_SIPPACKET_INSTANCE_TRACKING

#if defined MXD_SIPSTACK_ENABLE_SIPREQUEST_CONTEXT_INSTANCE_TRACKING
    // Summary:
    //  Gets the number of ISipRequestContext still allocated.
    //---------------------------------------------------------
    static unsigned int DebugGetNumRequestContextsLeft();

    // Summary:
    //  Gets the vector of ISipRequestContext still allocated.
    //---------------------------------------------------------
    static unsigned int DebugGetRequestContextTable(OUT CVector<ISipRequestContext*>& rvecpRequestContext);

    // Summary:
    //  Gets a CBlob containing information about the ISipRequestContext still
    //  allocated.
    //-------------------------------------------------------------------------
    static void DebugGetRequestContextDump(OUT CBlob& rblobRequestContextDump);
#endif // #if defined MXD_SIPSTACK_ENABLE_SIPREQUEST_CONTEXT_INSTANCE_TRACKING

#if defined MXD_SIPSTACK_ENABLE_SIPTRANSACTION_INSTANCE_TRACKING
    // Summary:
    //  Gets the number of CSipTransaction still allocated.
    //------------------------------------------------------
    static unsigned int DebugGetNumTransactionsLeft();

    // Summary:
    //  Gets the vector of CSipTransaction still allocated.
    //------------------------------------------------------
    static unsigned int DebugGetTransactionTable(OUT CVector<CSipTransaction*>& rvecpTransaction);

    // Summary:
    //  Gets a CBlob containing information about the CSipTransaction still
    //  allocated.
    //----------------------------------------------------------------------
    static void DebugGetTransactionDump(OUT CBlob& rblobTransactionDump);
#endif // #if defined MXD_SIPSTACK_ENABLE_SIPTRANSACTION_INSTANCE_TRACKING

    //-- Hidden Methods
//-------------------
private:
#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // waring or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GNU 2.7.2 complains about private destructor with no friends.
        //M5T_INTERNAL_USE_BEGIN
        friend class Foo;
        //M5T_INTERNAL_USE_END
    #endif
#endif
    static CMutex  ms_mutex;

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CInstanceTracker();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CInstanceTracker();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CInstanceTracker(IN const CInstanceTracker& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CInstanceTracker& operator=(IN const CInstanceTracker& rSrc);

    public:
    //==SDOC====================================================================
    //==
    //== SMemoryRecords
    //==
    //==========================================================================
    //
    // Description:
    //  This structure is used to report some information about the instances
    //  of allocated memory blocks.
    //
    //==EDOC====================================================================
    struct SMemoryRecords
#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT)
        : public CMemoryAllocator::IMemoryBlockAccumulator
#endif
    {
        // Summary:
        //  Default Constructor.
        //-----------------------
        SMemoryRecords();

#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT)

        // Summary:
        //  Virtual destructor to avoid the warning caused by having
        //  another virtual function.
        //-----------------------------------------------------------
        virtual ~SMemoryRecords()
        {
        }

        // Count of total allocated memory blocks.
        size_t m_uNumberOfAllocatedMemoryBlocks;

        // Memory block type which must be accumulated.
        const char* m_szTypeName;

        // Array of up to
        // MXD_FRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS of
        // currently allocated memory blocks. If there are less than
        // MXD_FRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS
        // allocated blocks, the m_uNumberOfAllocatedMemoryBlocks describes the
        // number of blocks in the array.
        CMemoryAllocator::CMemoryBlock* m_astMemoryBlocks
            [uINSTANCE_TRACKER_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS];
    private:

        // Summary:
        //  Implementation of the
        //  CMemoryAllocator::IMemoryBlockAccumulator::Accumulate method.
        //--------------------------------------------------------------
        virtual void Accumulate(INOUT CMemoryAllocator::CMemoryBlock* pMemoryBlock);
#endif //#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT)
    };


};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined MXD_SIPSTACK_ENABLE_SIPCONTEXT_INSTANCE_TRACKING         ||
       //     defined MXD_SIPSTACK_ENABLE_SIPPACKET_INSTANCE_TRACKING          ||
       //     defined MXD_SIPSTACK_ENABLE_SIPREQUEST_CONTEXT_INSTANCE_TRACKING ||
       //     defined MXD_SIPSTACK_ENABLE_SIPTRANSACTION_INSTANCE_TRACKING

#endif // #if defined MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT
#endif //#ifndef MXG_CINSTANCETRACKER_H
