//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1999 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_CMEMORYALLOCATOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMEMORYALLOCATOR_H
//M5T_INTERNAL_USE_END

// Verify that this header file is included by MxConfig.h only. This
// verification is performed because we state that the memory allocator
// configuration is done through PreMxConfig.h, which is only included by
// MxConfig.h. If we were to allow the direct inclusion of CMemoryAllocator.h,
// we would loose the configuration done in PreMxConfig.h.
//-----------------------------------------------------------------------------
#if !defined(MXG_MXCONFIG_H)
    #error This file must be included through MxConfig.h only!
#endif

// BRCM_CUSTOM - [add] Broadcom code allocated memory support
#if defined(MXD_BRCM_MEMORY_ALLOCATOR_WRAPPER_SUPPORT) && \
    (!defined(MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT) || \
     !defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT) || \
     !defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT))
    #error MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT, \
           MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT, and \
           MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT \
           must be defined to enable MXD_BRCM_MEMORY_ALLOCATOR_WRAPPER_SUPPORT.
#endif
// BRCM_CUSTOM - [end] Broadcom code allocated memory support

#if defined(MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT)
    #ifndef MXG_CALIGNEDVARIABLESTORAGE_H
        #include "Basic/CAlignedVariableStorage.h"
    #endif

    #if defined(MXD_MEMORY_ALLOCATOR_PROTECTION_ENABLE_SUPPORT) || \
        defined(MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT) || \
        defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT) || \
        defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)

        #ifndef MXG_CMUTEX_H
            #include "Kernel/CMutex.h"
        #endif
    #endif
#endif

#ifndef MXG_MXMACRO_H
    #include "Basic/MxMacro.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

#if defined(MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// Make sure that if we have enabled the memory pooling that we have at least
// one pool configured. The default pool is empty and will never be choosen
// for allocation.
#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)
    #if !defined(MXD_astMEMORY_ALLOCATOR_POOL_INFO)
        #define MXD_astMEMORY_ALLOCATOR_POOL_INFO   \
        {                                           \
            {0, 0, 0, NULL, NULL}                   \
        }
    #endif //!defined(MXD_astMEMORY_ALLOCATOR_POOL_INFO)
#endif // #if defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)

//<GROUP KERNEL_VARIABLES>
//
// Description:
// Set interval between memory blocks.
static const unsigned int uMEMORY_BLOCKS_INTERVALS = 32;

//==SDOC========================================================================
//
//  class CMemoryAllocator
//
//==============================================================================
//<GROUP KERNEL_CLASSES>
//
//  Summary:
//      Class used to override new and delete default operators.
//
//  Description:
//      This class is used to override the new and delete default operators.
//      It is also used to access memory usage information.
//
//==EDOC========================================================================
class CMemoryAllocator
{
//M5T_INTERNAL_USE_BEGIN
// Friends declarations.
//-----------------------
    friend mxt_result InitializeCMemoryAllocator();
    friend void FinalizeCMemoryAllocator();


// Forward declarations.
//-----------------------
    class CMemoryBlockList;
    class CMemoryBlockAllocatorBase;
//M5T_INTERNAL_USE_END

public:
    class CMemoryBlock;

// Type definitions.
//-------------------

    //==SDOC====================================================================
    //==
    //==  IMemoryBlockAccumulator
    //==
    //==========================================================================
    //<GROUP KERNEL_CLASSES>
    //
    //  Summary:
    //      Interface defining an accumulator of CMemoryBlocks.
    //
    //  Description:
    //      This interface defines a simple CMemoryBlock accumulator. The
    //      implementation of this interface MUST be prepared to receive
    //      multiple successive calls to the Accumulate method, accept and
    //      process each memory block in an implementation dependent way.
    //
    //  See also:
    //      CMemoryBlock, CMemoryBlockList::Enumerate
    //
    //==EDOC====================================================================
    class IMemoryBlockAccumulator
    {
    public:
        //==SDOC================================================================
        //==
        //==  Accumulate
        //==
        //======================================================================
        //
        //  Parameters:
        //      pMemoryBlock:
        //       A pointer to the memory blocks being accumulated.
        //
        //  Summary:
        //      Notification function which receives the memory blocks being
        //      enumerated.
        //
        //  Description:
        //      This notification function is the callback function called by
        //      the CMemoryAllocator::EnumMemoryBlocks method while enumerating
        //      through the list of currently allocated memory blocks.
        //
        //  See also:
        //      CMemoryBlock, CMemoryAllocator::EnumMemoryBlocks
        //
        //==EDOC================================================================
        virtual void Accumulate(INOUT CMemoryBlock* pMemoryBlock) = 0;

//M5T_INTERNAL_USE_BEGIN
    protected:
        // Summary:
        //  Constructor.
        //---------------
        IMemoryBlockAccumulator() {};

        // Summary:
        //  Destructor.
        //--------------
        virtual ~IMemoryBlockAccumulator() {};
//M5T_INTERNAL_USE_END
    };

    //==SDOC====================================================================
    //==
    //==  SMemoryStatistics
    //==
    //==========================================================================
    //<GROUP KERNEL_STRUCTS>
    //
    //  Summary:
    //      Structure to store memory statistics.
    //
    //  Description:
    //      This structure is used to store memory statistics.
    //
    //==EDOC====================================================================
    struct SMemoryStatistics
    {
        //  Description:
        // A uint32_t value representing the total number of bytes allocated.
        uint32_t m_uAllocatedMemory;
        //  Description:
        // An unsigned int value representing the number of calls the new
        // operator.
        unsigned int m_uNewCallCount;
        //  Description:
        // An unsigned int value representing the number of calls to the
        // delete operator.
        unsigned int m_uDeleteCallCount;
        //  Description:
        // A uint32_t value representing the size in bytes of the biggest
        // allocated memory block.
        uint32_t m_uBiggestMemoryBlock;
        //  Description:
        // A uint32_t value representing the peak memory usage in bytes.
        uint32_t m_uPeakMemoryUsage;
        //  Description:
        // A uint32_t value representing the upper limit of allocated
        // memory used to notify the eEN_CMEMORYALLOCATOR_TRACKING event
        // observer.
        uint32_t m_uHighWaterMark;
        //  Description:
        // A uint32_t value representing the lower limit of allocated memory
        // used to notify the eEN_CMEMORYALLOCATOR_TRACKING event observer.
        uint32_t m_uLowWaterMark;
        //  Description:
        // An array of unsigned int of size uMEMORY_BLOCKS_INTERVALS.  Each
        // value in the array represents the number of allocated blocks whose
        // size falls between 2^(index-1) and 2^index.
        unsigned int m_auMemoryBlocksCount[uMEMORY_BLOCKS_INTERVALS];
    };

#if defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)

    //==SDOC====================================================================
    //==
    //==  SMemoryBlockExtraInfo
    //==
    //==========================================================================
    //<GROUP KERNEL_STRUCTS>
    //
    //  Summary:
    //      Structure used to store extra information about an allocated memory
    //      block.
    //
    //  Description:
    //      This structure holds extra information about an allocated memory
    //      block. This information is not crucial for the memory allocation
    //      mechanism to function, it can however be very useful for debugging
    //      purposes. The information includes the file and line at which the
    //      allocation was done and the type of object allocated.
    //
    //  See also:
    //      SMemoryBlockHeader
    //
    //==EDOC====================================================================
    struct SMemoryBlockExtraInfo
    {
// BRCM_CUSTOM - [add] Suppress VC8 L4 warnings
        inline explicit SMemoryBlockExtraInfo( void ) {};
// BRCM_CUSTOM - [end] Suppress VC8 L4 warnings

        // Summary :
        //  Constructor for inline construction of a SMemoryBlockInfo.
        //---------------------------------------------------------
        inline explicit SMemoryBlockExtraInfo(IN char const* szType,
                                              IN char const* szFilename,
                                              IN uint16_t uFlags,
                                              IN uint32_t m_uLineNumber);

        //  Description:
        // A string representing the Type of data contained in the memory block.
        char const* m_pszType;
        //  Description:
        // The filename of the file in which the memory allocation was done.
        char const* m_pszFilename;

        //==SDOC================================================================
        //==
        //==  EMemoryBlockFlags
        //======================================================================
        //<GROUP KERNEL_ENUMS>
        //
        // Summary:
        //  This enumeration describes the different bits in the m_uFlags
        //  bitfield.
        //
        //==EDOC================================================================
        enum EMemoryBlockFlags
        {
            //  Description:
            // Flag used to mark the memory block as temporarily untracked.
            eMEMORY_BLOCK_TEMPORARILY_UNTRACKED_FLAG = 0x0001,
            //  Description:
            // Flag used to mark the memory block as pooled.
            eMEMORY_BLOCK_POOLED                     = 0x0002
// BRCM_CUSTOM - [add] Broadcom code allocated memory support
#if defined(MXD_BRCM_MEMORY_ALLOCATOR_WRAPPER_SUPPORT)
            //  Description:
            // Flag used to mark the memory block as allocated by BRCM code.
            ,eMEMORY_BLOCK_BRCM_ALLOCATED            = 0x0004
#endif
// BRCM_CUSTOM - [end] Broadcom code allocated memory support
            // Add more flags here.
        };

        //  Description:
        // Bitfield of flags used to collect various information about the
        // memory block.
        uint16_t m_uFlags;
        //  Description:
        // The line number in the file at which the memory allocation was done.
        uint16_t m_uLineNumber;
    };

#endif // #if defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)


    //==SDOC====================================================================
    //==
    //==  SMemoryBlockHeader
    //==
    //==========================================================================
    //<GROUP KERNEL_STRUCTS>
    //
    //  Summary:
    //      Structure used to store information about an allocated memory block.
    //
    //  Description:
    //      This structure holds information about an allocated memory block.
    //      The information held in this header includes crucial memory
    //      management information as well as optionally including extra
    //      debugging information.
    //
    //  See also:
    //      SMemoryBlockExtraInfo, SMemoryBlock
    //
    //==EDOC====================================================================
    struct SMemoryBlockHeader
    {
#if defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)
        //  Description:
        // Extra information about the memory block.
        SMemoryBlockExtraInfo m_stExtraInformation;
#endif

    //-- The following begin and end are for the documentation software. It has
    //-- a problem parsing the following #ifdef that causes documentation errors..

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)
    private:
        // Here we give access to the next and previous pointers only to the
        // CMemoryBlockList class.
        friend class CMemoryBlockList;

        // Pointer to the next block of memory. NULL if this is the last block.
        CMemoryBlock* m_pNextMemoryBlock;
        // Pointer to the previous block of memory. NULL if this is the first
        // block.
        CMemoryBlock* m_pPreviousMemoryBlock;

    public:
#endif
//M5T_INTERNAL_USE_END

        //  Description:
        // The size of the block.
        size_t m_uBlockSize;
        //  Description:
        // The high double word of the start-of-block signature.
        uint32_t m_uHighBlockStartSignature;
        //  Description:
        // The low double word of the start-of-block signature.
        uint32_t m_uLowBlockStartSignature;
    };

    //==SDOC====================================================================
    //==
    //==  CMemoryBlock
    //==
    //==========================================================================
    //<GROUP KERNEL_CLASSES>
    //
    //  Summary:
    //      Class used to store an allocated memory block.
    //
    //  Description:
    //      This class holds an allocated memory block and it's header. The
    //      m_blockHeader variable holds the header of memory block and the
    //      m_uStartOfBlock variable marks the beginning of the user-accessible
    //      block.
    //
    //      The public interface that is provided is safe to use by outsiders of
    //      this class provided they don't try to modify the pointer returned
    //      by GetPointer.
    //
    //  See also:
    //      SMemoryBlockExtraInfo
    //
    //==EDOC====================================================================
    class CMemoryBlock
    {
    public:

#if defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)

        // Summary:
        //  Returns the filename in which the memory block was allocated.
        //----------------------------------------------------------------
        char const* GetFilename() const;

        // Summary:
        //  Returns a zero-terminated string containing the type of the
        //  allocated memory block.
        //--------------------------------------------------------------
        char const* GetType() const;

        // Summary:
        //  Returns the line number at which the memory block was allocated.
        //-------------------------------------------------------------------
        uint16_t GetLineNumber() const;

        // Summary:
        //  Returns whether the memory block was temporarily marked as
        //  untracked.
        //-------------------------------------------------------------
        bool IsTemporarilyUntrackedFlagSet() const;

        // Summary:
        //  Sets whether the memory block is temporarily marked as
        //  untracked.
        //---------------------------------------------------------
        void SetTemporarilyUntrackedFlag(IN bool bIsSet);

        // Summary:
        //  Returns whether the memory block is currently marked as
        //  being pooled in a memory pool.
        //----------------------------------------------------------
        bool IsPooled() const;

        // Summary:
        //  Sets whether the memory block is marked as a pooled memory block.
        //--------------------------------------------------------------------
        void SetPooled(IN bool bIsPooled);


// BRCM_CUSTOM - [add] Broadcom code allocated memory support
#if defined(MXD_BRCM_MEMORY_ALLOCATOR_WRAPPER_SUPPORT)
        // Summary:
        //  Returns whether the memory block is allocated by BRCM code.
        //--------------------------------------------------------------
        bool IsBrcmAllocated() const;
#endif
// BRCM_CUSTOM - [end] Broadcom code allocated memory support

#endif // #if defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)


        // Summary:
        //  Returns the size of the allocated memory block.
        //--------------------------------------------------
        size_t GetSize() const;

        // Summary:
        //  Returns the pointer of the user-block.
        //-----------------------------------------
        void* GetPointer();

//M5T_INTERNAL_USE_BEGIN
    private:

        // Make the parent friend of this class to allow it to modify the
        // private members.
        friend class CMemoryAllocator;
        friend class CMemoryBlockList;
        friend class CMemoryBlockAllocatorBase;

        // A signature that is used for realignment of memory block to deallocate.
        // This allow the deallocation of a memory block when a pointer is not
        // equal to the first byte of the memory block.
        static const uint32_t ms_uALIGNMENT_SIGNATURE_HIGH;
        static const uint32_t ms_uALIGNMENT_SIGNATURE_LOW;

#if defined(MXD_MEMORY_ALLOCATOR_BOUND_CHECK_ENABLE_SUPPORT)
        static const uint32_t ms_uMAGIC_WORD;
#endif

        // Summary:
        //  Method used to initialize the CMemoryBlock. This method needs to be
        //  called before the CMemoryBlock is used.
        //----------------------------------------------------------------------
        void Initialize(IN size_t uSize);

        // Header of the memory block, used by the memory management mechanisms.
        SMemoryBlockHeader m_stBlockHeader;
        // A dummy variable marking the beginning of the memory block.
        uint8_t m_auBlock[1];
//M5T_INTERNAL_USE_END
    };


#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)

    //==SDOC====================================================================
    //==
    //== PFNAllocateFunction
    //==
    //==========================================================================
    //
    // Parameters:
    //  IN size_t uSize
    //   The size of the requested block of memory.
    //
    // Returns:
    //  void*
    //   A pointer to the requested memory block.
    //
    // Description:
    //  This is a type describing a function pointer used to allocate a block of
    //  memory according to a specified size.
    //
    //==EDOC====================================================================
    typedef GO void* (*PFNAllocateFunction)(IN size_t uSize);

    //==SDOC====================================================================
    //==
    //== PFNDeallocateFunction
    //==
    //==========================================================================
    //
    // Parameters:
    //  IN TOA void* pvoidBlock
    //   A pointer to the block of memory to deallocate.
    //
    // Description:
    //  This type describes a function pointer to a function used to deallocate
    //  a block of memory.
    //
    //==EDOC====================================================================
    typedef void (*PFNDeallocateFunction)(IN TOA void* pvoidBlock);

    //==SDOC====================================================================
    //==
    //== SMemoryPoolInfo
    //==
    //==========================================================================
    //<GROUP KERNEL_STRUCTS>
    //
    // Description:
    //  This structure describes the parameters of a memory pool. A memory pool
    //  is an entity which can store blocks that are no longer used for later
    //  use when requested by the program.
    //
    //  To enable memory pooling you need to define the
    //  MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT compiler flag and
    //  configure the memory pooling using the MXD_astMEMORY_ALLOCATOR_POOL_INFO
    //  compiler flag according to the specific format described with its
    //  documentation.
    //
    //  See Also:
    //   MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT
    //   MXD_astMEMORY_ALLOCATOR_POOL_INFO
    //   CPooledMemoryBlockAllocator
    //
    //==EDOC====================================================================
    struct SMemoryPoolInfo
    {
        //  Description:
        // The lower bound size that the memory pool can accept (exclusive).
        size_t m_uBlockSizeLowerBound;

        //  Description:
        // The upper bound size that the memory pool can accept (inclusive).
        size_t m_uBlockSizeUpperBound;

        //  Description:
        // The capacity of the pool.
        size_t m_uCapacity;

        //  Description:
        // A pointer to the allocation function be used with this pool.
        PFNAllocateFunction m_pfnAllocate;
        //  Description:
        // A pointer to the deallocation function to be used with this pool.
        PFNDeallocateFunction m_pfnDeallocate;
    };

#endif //#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)

//M5T_INTERNAL_USE_BEGIN
private:

#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)

    //==SDOC====================================================================
    //==
    //== CMemoryBlockList
    //==
    //==========================================================================
    //<GROUP KERNEL_CLASSES>
    //
    // Summary:
    //  Class used to store a list of CMemoryBlock instances.
    //
    // Description
    //  This class is a CMemoryBlock linked list implementation which will use
    //  special knowledge of the CMemoryBlock to link them using already
    //  allocated memory.
    //
    //==EDOC====================================================================
    class CMemoryBlockList
    {
    public:

        // Summary:
        //  Constructor for the CMemoryBlockList class.
        //----------------------------------------------
        explicit CMemoryBlockList(IN CMutex* pSharedMutex);

        // Summary:
        //  Destructor for the CMemoryBlockList class.
        //---------------------------------------------
        ~CMemoryBlockList();

        // Summary:
        //  Method used to insert a memory block in the memory block list.
        //-----------------------------------------------------------------
        void Insert(IN CMemoryBlock* pMemoryBlock);

        // Summary:
        //  Method used to remove a memory block from the memory block list.
        //-------------------------------------------------------------------
        void Remove(IN CMemoryBlock* pMemoryBlock);

        // Summary:
        //  Method used to pop a memory block from the memory block list.
        //----------------------------------------------------------------
        CMemoryBlock* Pop();

        // Summary:
        //  Method used to retrieve the size of the memory block list.
        //-------------------------------------------------------------
        size_t GetSize() const;

        // Summary:
        //  Method used to enumerate through all the memory blocks in the
        //  memory block list.
        //----------------------------------------------------------------
        void Enumerate(IN IMemoryBlockAccumulator* pAccumulator);

        // Summary:
        //  This method locks the list to allow multiple manipulations without
        //  any chance to have any synchronization issues.
        //---------------------------------------------------------------------
        void Lock();

        // Summary:
        //  This method unlocks the list.
        //--------------------------------
        void Unlock();

    private:

        // Summary:
        //  Internal lock method which can be used to lock the list to retrieve
        //  synchronized data without requiring a non-const object.
        //----------------------------------------------------------------------
        void Lock() const;

        // Summary:
        //  Internal unlock method which can be used to unlock the list after
        //  retrieving synchronized data without requiring a non-const object.
        //---------------------------------------------------------------------
        void Unlock() const;

        // A mutex to protect access to the list of memory blocks. This mutex
        // is not owned by the CMemoryBlockList and can be shared to allow
        // the mutex to protect multiple resources. This member variable is
        // declared as mutable so that the list can internally lock the mutex
        // without requiring a non-const object.
        mutable CMutex* m_pMutex;

        // A pointer to the first block in the list of memory blocks.
        CMemoryBlock* m_pMemoryBlockHead;

        // A variable used to hold the current size of the CMemoryBlockList
        size_t m_uSize;
    };

#endif //#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT) ||
       //    defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)

    //==SDOC====================================================================
    //==
    //== IMemoryTracker
    //==
    //==========================================================================
    //<GROUP KERNEL_CLASSES>
    //
    // Description:
    //  This class defines the interface for a memory tracker which gets
    //  notified when blocks are allocated/deallocated.
    //
    //==EDOC====================================================================
    class IMemoryTracker
    {
    public:
        //==SDOC================================================================
        //==
        //== MemoryAllocated
        //==
        //======================================================================
        //
        // Parameters:
        //  pMemoryBlock
        //   The memory block that was just allocated.
        //
        // Description:
        //  Notification to the memory tracker that a memory block was
        //  allocated.
        //
        //==EDOC================================================================
        virtual void MemoryAllocated(IN CMemoryBlock* pMemoryBlock) = 0;

        //==SDOC================================================================
        //==
        //== MemoryDeallocated
        //==
        //======================================================================
        //
        // Parameters:
        //  pMemoryBlock
        //   The memory block that is about to be deallocated.
        //
        // Description:
        //  Notification to the memory tracker that a memory block is about to
        //  get deallocated.
        //
        //==EDOC================================================================
        virtual void MemoryDeallocated(IN CMemoryBlock* pMemoryBlock) = 0;

    protected:
        // Summary:
        //  Constructor.
        //---------------
        IMemoryTracker() {};

        // Summary:
        //  Destructor.
        //--------------
        virtual ~IMemoryTracker() {};
    };


#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT)

    //==SDOC====================================================================
    //==
    //== CMemoryTracker
    //==
    //==========================================================================
    //<GROUP KERNEL_CLASSES>
    //
    // Description:
    //  This class is the default memory tracker for the CMemoryAllocator. When
    //  the statistics or memory tracking features of the CMemoryAllocator class
    //  are enabled (MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT or
    //  MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT), this class is
    //  responsible for keeping track of the memory blocks allocated as well as
    //  keeping statistics about memory allocation.
    //
    // See Also:
    //  IMemoryTracker
    //  MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT
    //  MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT
    //
    //==EDOC====================================================================
    class CMemoryTracker
    :   public IMemoryTracker
#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT)
    ,   private CMemoryBlockList
#endif
    {
    public:
        // Summary:
        //  Constructor for the CMemoryTracker object.
        //---------------------------------------------
        explicit CMemoryTracker(IN CMutex* pMutex);

#if defined(MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT)

        // Summary :
        //  Gets a structure containing current memory statistics.
        //---------------------------------------------------------
        void GetMemoryStatistics(OUT SMemoryStatistics* pstMemoryStatistics);

        // Summary :
        //  Sets the structure containing current memory statistics.
        //-----------------------------------------------------------
        void SetMemoryStatistics(IN SMemoryStatistics const* pstMemoryStatistics);

        // Summary :
        //  Enables/Disables memory statistics acquisition.
        //--------------------------------------------------
        void SetMemoryStatisticsEnabled(IN bool bEnabled);

#endif // #if defined(MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT)

#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT)

        // Summary:
        //  Function used to enumerate through all the blocks currently tracked
        //  by the CMemoryTracker.
        //----------------------------------------------------------------------
        void Enumerate(IN IMemoryBlockAccumulator* pAccumulator);
#endif

    private:

        // << IMemoryTracker >>
        virtual void MemoryAllocated(IN CMemoryBlock* pMemoryBlock);
        virtual void MemoryDeallocated(IN CMemoryBlock* pMemoryBlock);

#if defined(MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT)

        // The memory statistics.
        SMemoryStatistics m_stMemoryStatistics;
        // A flag setting whether the statistics are enabled or not.
        bool m_bMemoryStatisticsEnabled;
        // A mutex to control access to certain resources.
        CMutex* m_pMutex;
#endif
    };

#endif //#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT) ||
       //    defined(MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT)

    //==SDOC====================================================================
    //==
    //== CMemoryBlockAllocatorBase
    //==
    //==========================================================================
    //<GROUP KERNEL_CLASSES>
    //
    // Description:
    //  This class is a base class to be used by all types of block allocators
    //  used by the CMemoryAllocator. It provides some services to the block
    //  allocators such as the initialization of an allocated memory block.
    //
    //  Note that it is mandatory for a memory block to be allocated and
    //  deallocated with the same memory block allocator.
    //
    // See Also:
    //  CMemoryBlock, CDefaultMemoryBlockAllocator, CPooledMemoryBlockAllocator
    //
    //==EDOC====================================================================
    class CMemoryBlockAllocatorBase
    {
    public:

        // Summary:
        //  Constructor for the CMemoryBlockAllocatorBase class.
        //-------------------------------------------------------
        explicit CMemoryBlockAllocatorBase(IN IMemoryTracker* pMemoryTracker);

        // Summary:
        //  Virtual destructor necessary to avoid warnings on certain platforms.
        //----------------------------------------------------------------------
        virtual ~CMemoryBlockAllocatorBase();

        //==SDOC================================================================
        //==
        //== Allocate
        //==
        //======================================================================
        //
        //  Parameters:
        //   IN size_t uSize
        //    Specifies the requested size for the allocated memory block. Note
        //    that the allocator has to allocate at least the amount requested
        //    but can allocate more if it chooses to do so.
        //
        //  Returns:
        //   GO CMemoryBlock*
        //    A pointer to the newly allocated memory block.
        //
        //  Description:
        //   This method is responsible for allocating a CMemoryBlock with the
        //   specified user-space size. The allocated block needs to be
        //   deallocated using the same memory block allocator's Deallocate
        //   method.
        //
        //  See Also:
        //   Deallocate
        //
        //==EDOC================================================================
        virtual GO CMemoryBlock* Allocate(IN size_t uSize) = 0;

        //==SDOC================================================================
        //==
        //== Deallocate
        //==
        //======================================================================
        //
        //  Parameters:
        //   IN TOA CMemoryBlock* pMemoryBlock
        //    Specifies the memory block to be deallocated.
        //
        //  Description:
        //   This method is used to deallocate a block of memory allocated using
        //   the Allocate method of the same memory block allocator.
        //
        //  See Also:
        //   Allocate
        //
        //==EDOC================================================================
        virtual void Deallocate(IN TOA CMemoryBlock* pMemoryBlock) = 0;

        //==SDOC================================================================
        //==
        //== IsValidAllocator
        //==
        //======================================================================
        //
        // Parameters:
        //  IN size_t uSize
        //   Specifies the size of the block which will be tested for compliance
        //   against the allocator.
        //
        // Returns:
        //  bool
        //   Returns true if the specified block size can be handled
        //   by this specific memory block allocator. Returns false
        //   if the block size cannot be handled by the block allocator.
        //
        // Description
        //  This method is used to check whether a specific memory block size
        //  can be handled by the memory block allocator.
        //
        //==EDOC================================================================
        virtual bool IsValidAllocator(IN size_t uSize) = 0;

    protected:

        // Summary:
        //  Method used to initialize a specified memory block of a specified
        //  size.
        //--------------------------------------------------------------------
        void InitializeMemoryBlock(IN size_t uSize, INOUT CMemoryBlock* pMemoryBlock);

        // A memory tracker that is used by the memory block allocator to
        // raise notifications that a memory block was allocated or deallocated.
        IMemoryTracker* m_pMemoryTracker;
    };

    //==SDOC====================================================================
    //==
    //== CDefaultMemoryBlockAllocator
    //==
    //==========================================================================
    //<GROUP KERNEL_CLASSES>
    //
    // Description:
    //  This class implements the default memory block allocator which is used
    //  when no other memory block allocators can be used. Is uses malloc and
    //  free to allocate memory.
    //
    //
    //==EDOC====================================================================
    class CDefaultMemoryBlockAllocator : public CMemoryBlockAllocatorBase
    {
    public:

        // Summary:
        //  Constructor for the CDefaultMemoryBlockAllocator.
        //----------------------------------------------------
        explicit CDefaultMemoryBlockAllocator(IN IMemoryTracker* pMemoryTracker);

        // << CMemoryBlockAllocatorBase >>
        virtual GO CMemoryBlock* Allocate(IN size_t uSize);
        virtual void Deallocate(TOA CMemoryBlock* pMemoryBlock);
        virtual bool IsValidAllocator(IN size_t uSize);
    };

#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)

    //==SDOC====================================================================
    //==
    //== CPooledMemoryBlockAllocator
    //==
    //==========================================================================
    //<GROUP KERNEL_CLASSES>
    //
    // Description:
    //  This class implements a block allocator which pools unneeded memory and
    //  reuses it when a memory of a similar block size is requested.
    //
    // See Also:
    //  MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT
    //  MXD_astMEMORY_ALLOCATOR_POOL_INFO
    //  CMemoryBlockAllocatorBase
    //  SMemoryPoolInfo
    //
    //==EDOC====================================================================
    class CPooledMemoryBlockAllocator : public CMemoryBlockAllocatorBase
    {
    public:

        // Summary:
        //  Constructor for the CPooledMemoryBlockAllocator class.
        //---------------------------------------------------------
        explicit CPooledMemoryBlockAllocator(IN CMutex* pMutex,
                                             IN IMemoryTracker* pMemoryTracker,
                                             IN SMemoryPoolInfo const& stPoolInfo);

        // << CMemoryBlockAllocatorBase >>
        virtual GO CMemoryBlock* Allocate(IN size_t uSize);
        virtual void Deallocate(TOA CMemoryBlock* pMemoryBlock);
        virtual bool IsValidAllocator(IN size_t uSize);

        // Summary:
        //  This function is used to enumerate all the currently pooled memory
        //  blocks.
        //---------------------------------------------------------------------
        void EnumeratePooledMemoryBlocks(IN IMemoryBlockAccumulator* pMemoryBlockAccumulator);

    private:

        // This member contains the configuration of this memory pool.
        SMemoryPoolInfo m_stMemoryPoolInfo;

        // List of memory blocks currently in the pool.
        CMemoryBlockList m_memoryPool;

        // Synchronization object to ensure that the pool is thread-safe.
        CMutex* m_pMutex;
    };

#endif // #if defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)
//M5T_INTERNAL_USE_END

//-- Published Interface.
//------------------------
public:

    // Summary :
    //  Get a pointer to the CMemoryBlock associated with an allocated memory
    //  block.
    //------------------------------------------------------------------------
    static CMemoryBlock* GetMemoryBlock(IN void* pvoidBlock);

    // Summary :
    //  Allocates a block of memory.
    //-------------------------------
    static void* GO Allocate(IN size_t uSize);

#if defined (MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)

    // Summary :
    //  Allocates a block of memory and stores extra information about
    //  the allocation.
    //-------------------------------
    static void* GO Allocate(IN size_t uSize,
                             IN SMemoryBlockExtraInfo const& rExtraInfo);

#endif // #if defined (MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)

    // Summary :
    //  Frees a block of memory.
    //---------------------------
    static void Deallocate(IN TOA void* pvoidBlock);

    // Summary :
    //  Frees a block of memory.
    //---------------------------
    static void Deallocate(IN TOA const void* pvoidBlock);

#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT)

    // Summary:
    //  Enumerates through the currently allocated list of memory blocks.
    //--------------------------------------------------------------------
    static void EnumMemoryBlocks(IN IMemoryBlockAccumulator* pAccumulator);

#endif

    // Summary :
    //  Gets a structure containing current memory statistics.
    //---------------------------------------------------------
    static void GetMemoryStatistics(OUT SMemoryStatistics* pstMemoryStatistics);

    // Summary :
    //  Resets the structure containing current memory statistics.
    //-------------------------------------------------------------
    static void ResetMemoryStatistics();

    // Summary :
    //  Sets the structure containing current memory statistics.
    //-------------------------------------------------------------
    static void SetMemoryStatistics(IN SMemoryStatistics* pstMemoryStatistics);

    // Summary :
    //  Enables memory statistics acquisition.
    //----------------------------------------
    static void EnableMemoryStatistics();

    // Summary :
    //  Disables memory statistics acquisition.
    //----------------------------------------
    static void DisableMemoryStatistics();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//---------------
private:

    // Summary:
    //  Method used to initialize the CMemoryAllocator.
    //--------------------------------------------------
    static mxt_result InitializeCMemoryAllocator();

    // Summary:
    //  Method used to finalize the CMemoryAllocator.
    //------------------------------------------------
    static void FinalizeCMemoryAllocator();

    // Summary:
    //  This helper function is used to retrieve the correct memory block
    //  allocator for the given memory block size.
    //--------------------------------------------------------------------
    static CMemoryBlockAllocatorBase* GetMemoryBlockAllocator(IN size_t uSize);


#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // warning or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif

    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------
    CMemoryAllocator() {}
    ~CMemoryAllocator() {}


// Hidden Data Members
//---------------------
private:

#if defined(MXD_MEMORY_ALLOCATOR_PROTECTION_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)
    static CAlignedVariableStorage<CMutex> ms_mutex;
#endif

#if defined(MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT)

    static CAlignedVariableStorage<CMemoryTracker> ms_memoryTrackerStorage;
    static CMemoryTracker* ms_pMemoryTracker;
#endif

#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT)

    // Contains the configuration information for the memory pooling.
    static const SMemoryPoolInfo ms_astMemoryAllocatorPoolInfos[];

    // This member holds the count of memory pool used in the configuration.
    static const size_t ms_uMemoryPoolCount;

    // This is the storage location for in-place allocation of the pooled memory
    // allocators.
    static void* ms_aPooledAllocatorsStorage[];

    // A pointer to the list of pooled memory block allocators.
    static CPooledMemoryBlockAllocator* ms_pPooledMemoryAllocators;

#endif

    // Storage to hold the default allocator.
    static CAlignedVariableStorage<CDefaultMemoryBlockAllocator>
                                            ms_defaulMemoryAllocatorStorage;

    // Total count of all memory allocators.
    static const size_t ms_uMemoryAllocatorCount;

    // List of all memory block allocators.
    static CMemoryBlockAllocatorBase* ms_apMemoryAllocators[];
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

#if defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)

//==SDOC========================================================================
//==
//==  GetFilename
//==
//==============================================================================
//
//  Returns:
//      char const*
//       Null-terminated string containing the filename in which the
//       CMemoryBlock was allocated.
//
//  Description:
//      This method returns a null-terminated string representing the filename
//      of the file in which the CMemoryBlock was allocated.
//
//==EDOC========================================================================
inline char const* CMemoryAllocator::CMemoryBlock::GetFilename() const
{
    return m_stBlockHeader.m_stExtraInformation.m_pszFilename;
}

//==SDOC========================================================================
//==
//==  GetType
//==
//==============================================================================
//
//  Returns:
//      char const*
//       Null-terminated string containing the type of the object contained in
//       the allocated memory block.
//
//  Description:
//      This method returns a null-terminated string containing the type of the
//      object contained in the allocated memory block.
//
//==EDOC========================================================================
inline char const* CMemoryAllocator::CMemoryBlock::GetType() const
{
    return m_stBlockHeader.m_stExtraInformation.m_pszType;
}

//==SDOC========================================================================
//==
//==  GetLineNumber
//==
//==============================================================================
//
//  Returns:
//      uint16_t
//       The line number at which this memory block was allocated.
//
//  Description:
//      This method returns the line number at which this memory block was
//      allocated.
//
//==EDOC========================================================================
inline uint16_t CMemoryAllocator::CMemoryBlock::GetLineNumber() const
{
    return m_stBlockHeader.m_stExtraInformation.m_uLineNumber;
}

//==SDOC========================================================================
//==
//==  IsTemporarilyUntrackedFlagSet
//==
//==============================================================================
//
//  Returns:
//      bool
//       True if the "Temporarily Untracked" flag is set for this memory block.
//       False otherwise.
//
//  Description:
//      This method returns whether the "Temporarily Untracked" flag is set for
//      this memory block.
//
//==EDOC========================================================================
inline bool CMemoryAllocator::CMemoryBlock::IsTemporarilyUntrackedFlagSet() const
{
    // Bitwise AND to check if the bit is set.
    return (m_stBlockHeader.m_stExtraInformation.m_uFlags &
            CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_TEMPORARILY_UNTRACKED_FLAG) ==
            CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_TEMPORARILY_UNTRACKED_FLAG;
}

//==SDOC========================================================================
//==
//==  SetTemporarilyUntrackedFlag
//==
//==============================================================================
//
//  Parameters:
//      bIsSet:
//       The new value for the "Temporarily Untracked" flag.
//
//  Description:
//      This method is used to mark the memory block as Temporarily Untracked.
//      This allows the user to filter existing memory blocks as they are
//      enumerated from the CMemoryAllocator::EnumMemoryBlock method.
//
//==EDOC========================================================================
inline void CMemoryAllocator::CMemoryBlock::SetTemporarilyUntrackedFlag(IN bool bIsSet)
{
    if (bIsSet)
    {
        // Bitwise OR the bitmask with the bitfield to set the flag.
        m_stBlockHeader.m_stExtraInformation.m_uFlags |=
            CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_TEMPORARILY_UNTRACKED_FLAG;
    }
    else
    {
        // Bitwise AND the one's complement of the bitmask with the bitfield to
        // reset the flag.
        m_stBlockHeader.m_stExtraInformation.m_uFlags &=
            ~CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_TEMPORARILY_UNTRACKED_FLAG;
    }
}

//==SDOC========================================================================
//==
//== IsPooled
//==
//==============================================================================
//
//  Returns:
//   bool
//    True if the "Is Pooled" flag is set for this memory block.
//    False otherwise.
//
//  Description:
//      This method returns whether the "Is Pooled" flag is set for
//      this memory block.
//
//==EDOC========================================================================
inline bool CMemoryAllocator::CMemoryBlock::IsPooled() const
{
    // Bitwise AND to check if the bit is set.
    return (m_stBlockHeader.m_stExtraInformation.m_uFlags &
            CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_POOLED) ==
            CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_POOLED;
}

//==SDOC========================================================================
//==
//== SetPooled
//==
//==============================================================================
//
//  Parameters:
//      bIsPooled:
//       The new value for the "Is Pooled" flag.
//
//  Description:
//      This method is used to mark the memory block when it is put in a memory
//      pool. This allows the user to filter pooled memory blocks as they are
//      enumerated from the CMemoryAllocator::EnumMemoryBlock method.
//
//==EDOC========================================================================
inline void CMemoryAllocator::CMemoryBlock::SetPooled(IN bool bIsPooled)
{
    if (bIsPooled)
    {
        // Bitwise OR the bitmask with the bitfield to set the flag.
        m_stBlockHeader.m_stExtraInformation.m_uFlags |=
            CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_POOLED;
    }
    else
    {
        // Bitwise AND the one's complement of the bitmask with the bitfield to
        // reset the flag.
        m_stBlockHeader.m_stExtraInformation.m_uFlags &=
            ~CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_POOLED;
    }
}

// BRCM_CUSTOM - [add] Broadcom code allocated memory support
#if defined(MXD_BRCM_MEMORY_ALLOCATOR_WRAPPER_SUPPORT)
//==SDOC========================================================================
//==
//==  IsBrcmAllocated
//==
//==============================================================================
//
//  Returns:
//      bool
//       True if the the memory block is allocated by BRCM code.
//       False otherwise.
//
//  Description:
//      This method returns whether the memory block is allocated by BRCM code.
//
//==EDOC========================================================================
inline bool CMemoryAllocator::CMemoryBlock::IsBrcmAllocated() const
{
    // Bitwise AND to check if the bit is set.
    return (m_stBlockHeader.m_stExtraInformation.m_uFlags &
            CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_BRCM_ALLOCATED) ==
            CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_BRCM_ALLOCATED;
}
#endif
// BRCM_CUSTOM - [end] Broadcom code allocated memory support

#endif //#if defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)

//==SDOC========================================================================
//==
//==  GetSize
//==
//==============================================================================
//
//  Returns:
//      size_t
//       The number of bytes that this memory block reserves for user
//       accessible memory.
//
//  Description:
//      This method returns the size in bytes that this memory block reserves
//      for user accessible memory.
//
//==EDOC========================================================================
inline size_t CMemoryAllocator::CMemoryBlock::GetSize() const
{
    return m_stBlockHeader.m_uBlockSize;
}

//==SDOC========================================================================
//==
//==  GetPointer
//==
//==============================================================================
//
//  Returns:
//      void*
//       The user-space pointer to the memory location returned by
//       CMemoryAllocator::Allocate.
//
//  Description:
//      Method returning the pointer to the user-space memory location
//      of the CMemoryBlock.
//
//==EDOC========================================================================
inline void* CMemoryAllocator::CMemoryBlock::GetPointer()
{
    return m_auBlock;
}
//M5T_INTERNAL_USE_BEGIN
//==SDOC========================================================================
//==
//== Initialize
//==
//==============================================================================
//
//  Parameters:
//      uSize
//       The size of the memory block.
//
//  Description
//      This method is used to initialize a CMemoryBlock, it should be
//      called before using the CMemoryBlock.
//
//==EDOC========================================================================
inline void CMemoryAllocator::CMemoryBlock::Initialize(IN size_t uSize)
{
    //Reset the header.
    memset(this, 0, sizeof(SMemoryBlockHeader));

    // Stores the memory block size.
    m_stBlockHeader.m_uBlockSize = uSize;

    // Stores the memory block alignment signature.
    m_stBlockHeader.m_uHighBlockStartSignature = ms_uALIGNMENT_SIGNATURE_HIGH;
    m_stBlockHeader.m_uLowBlockStartSignature = ms_uALIGNMENT_SIGNATURE_LOW;

#if defined(MXD_MEMORY_ALLOCATOR_BOUND_CHECK_ENABLE_SUPPORT)
    // Adds Bound Checker magic word. The magic word is added at the end of
    // the memory block.
    //---------------------------------------------------------------------
    // Adds the magic word at the end of the block.
    void* pvoidEndMagicWord = &m_auBlock[uSize];
    *reinterpret_cast<mxt_UNALIGNED_UINT32*>(pvoidEndMagicWord) = ms_uMAGIC_WORD;
#endif

}
//M5T_INTERNAL_USE_END
#if defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)

//==SDOC========================================================================
//==
//==  SMemoryBlockExtraInfo
//==
//==============================================================================
//
//  Parameters:
//      szType:
//       A null-terminated string representing the type of object allocated.
//
//      szFilename:
//       A null-terminated string containing the filename of the file in which
//       the allocation was done.
//
//      uFlags:
//       Flags to be set in the SMemoryBlockExtraInfo.
//
//      uLineNumber:
//       An integer representing the line number at which the allocation was
//       done.
//
//  Description:
//      Inline Constructor for an SMemoryBlockInfo structure. This constructor
//      allows the complete initialization of a SMemoryBlockInfo structure to
//      allow ease of use in a Macro.
//
//==EDOC========================================================================
inline CMemoryAllocator::SMemoryBlockExtraInfo::SMemoryBlockExtraInfo(IN char const* szType,
                                                                      IN char const* szFilename,
                                                                      IN uint16_t uFlags,
                                                                      IN uint32_t uLineNumber)
:   m_pszType(szType),
    m_pszFilename(szFilename),
    m_uFlags(uFlags)
{
    //Since we only have 16 bits to store the line number, make sure to store
    //the maximum (0xFFFF) if we go over that.
    if (uLineNumber < 0xFFFF)
    {
        m_uLineNumber = static_cast<uint16_t>(uLineNumber);
    }
    else
    {
        m_uLineNumber = 0xFFFF;
    }
}

#endif // #if defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)

//==SDOC========================================================================
//==
//==  Deallocate
//==
//==============================================================================
//
//  Parameters:
//      pvoidBlock:
//       A pointer to the memory block to free.
//
//  Description:
//      This method is used to free a block of memory that was allocated with
//      Allocate. The provided pointer will be walked up until the
//      original pointer is found.
//
//  See also:
//      Allocate
//
//==EDOC========================================================================
inline void CMemoryAllocator::Deallocate(IN TOA const void* pvoidBlock)
{
    Deallocate(const_cast<void*>(pvoidBlock));
}

//==============================================================================
//====   MACRO DEFINITION  =====================================================
//==============================================================================

template<class _Type>
inline void MX_DELETE_DestructBasicType(IN TOA const _Type* pInstance)
{
    // The operator delete simply ignores a NULL pointer.
    if (pInstance != NULL)
    {
        // Free the allocated memory.
        CMemoryAllocator::Deallocate(pInstance);
    }
}

inline void MX_DELETE_Destruct(IN TOA const void* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const bool* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const char* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const signed char* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const unsigned char* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const signed short* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const unsigned short* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const signed int* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const unsigned int* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const signed long* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const unsigned long* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

#if defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    //-- Use 64 bits instead of long long since it is not supported.
    inline void MX_DELETE_Destruct(IN TOA const __int64* pInstance)
    {
        MX_DELETE_DestructBasicType(pInstance);
    }

    inline void MX_DELETE_Destruct(IN TOA const unsigned __int64* pInstance)
    {
        MX_DELETE_DestructBasicType(pInstance);
    }
#else
    //-- Long Long
    inline void MX_DELETE_Destruct(IN TOA const signed long long* pInstance)
    {
        MX_DELETE_DestructBasicType(pInstance);
    }

    inline void MX_DELETE_Destruct(IN TOA const unsigned long long* pInstance)
    {
        MX_DELETE_DestructBasicType(pInstance);
    }
#endif //defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)

inline void MX_DELETE_Destruct(IN TOA const float* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const double* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

inline void MX_DELETE_Destruct(IN TOA const long double* pInstance)
{
    MX_DELETE_DestructBasicType(pInstance);
}

template<class _Type>
inline void MX_DELETE_Destruct(IN TOA const _Type** ppInstance)
{
    MX_DELETE_DestructBasicType(ppInstance);
}

//M5T_INTERNAL_USE_BEGIN

// This is a little trick to fool the compiler to indirectly call
// _Type::~_Type. The problem is that when _Type::~Type is
// protected and code inside _Type does a MX_DELETE(this), the
// compiler generates errors. This is caused by the redefinition of the
// macro MX_DELETE which calls the function MX_DELETE_Destruct and it
// is no friend of _Type.
template<class _Type>
class CCallDestructor : public _Type
{
    // This declaration removes the need to have a default constructor in class
    // _Type when it has a specialized constructor (one which has parameters).
    //-----------------------------------------------------------------------
    CCallDestructor()
    {
    }
};

//M5T_INTERNAL_USE_END

template<class _Type>
inline void MX_DELETE_Destruct(IN TOA const _Type* pInstance)
{
    // The operator delete simply ignores a NULL pointer.
    if (pInstance != NULL)
    {
        // Please, see note above CCallDestructor for more information.
        {
            const CCallDestructor<_Type>* pConstCallDestructor =
                reinterpret_cast<const CCallDestructor<_Type>*>(pInstance);

            CCallDestructor<_Type>* pCallDestructor =
                const_cast<CCallDestructor<_Type>*>(pConstCallDestructor);
            MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pCallDestructor);
            pCallDestructor->~CCallDestructor<_Type>();
        }

        // Free the allocated memory.
        CMemoryAllocator::Deallocate(pInstance);
    }
}

template<class _Type>
inline void MX_DELETE_ARRAY_Destruct(IN TOA const _Type* pArray)
{
    // The operator delete simply ignores a NULL pointer.
    if (pArray != NULL)
    {
        // Retrieves the real address of the array.
        const void* pvoidArray =
            reinterpret_cast<const uint8_t*>(pArray) - sizeof(uint32_t);

        // Retrieves the size of the array from the first four bytes.
        uint32_t uSize = *reinterpret_cast<const uint32_t*>(pvoidArray);

        // For all elements, calls the destructor. In the reverse order.
        while (uSize--)
        {
            pArray[uSize].~_Type();
        }

        // Free the allocated memory.
        CMemoryAllocator::Deallocate(pvoidArray);
    }
}

template<class _Type>
inline _Type* GO MX_NEW_ARRAY_Construct(IN uint32_t uSize, INOUT void* pvoidArray)
{
    // Keeps the size of the array within the first four bytes.
    *reinterpret_cast<uint32_t*>(pvoidArray) = uSize;

    // Keeps a pointer to the first instance.
    _Type* pFirstInstance =
        reinterpret_cast<_Type*>(reinterpret_cast<uint8_t*>(pvoidArray) +
                                 sizeof(uint32_t));

    // Points to the first instance.
    _Type* pInstance = pFirstInstance;

    // For all elements, calls the placement new.
    while (uSize--)
    {
        new (pInstance++) _Type;
    }

    return pFirstInstance;
}

//M5T_INTERNAL_USE_BEGIN

#if defined MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT

// When we want to capture extra information about the allocation locus and type
// it has to be done in a macro expansion to allow for capturing the right file
// and line number.
#define MX_NEW(varType) new (MX_NAMESPACE(MXD_GNS)CMemoryAllocator::Allocate(  \
    sizeof(varType),                                                           \
    MX_NAMESPACE(MXD_GNS)CMemoryAllocator::SMemoryBlockExtraInfo(#varType, __FILE__, 0, __LINE__))) \
    (varType)

#define MX_NEW_ARRAY(varType, uSize)                      \
    MX_NAMESPACE(MXD_GNS)MX_NEW_ARRAY_Construct<varType>( \
    uSize,                                                \
    MX_NAMESPACE(MXD_GNS)CMemoryAllocator::Allocate(      \
        sizeof(uint32_t) + sizeof(varType) * (uSize),    \
        MX_NAMESPACE(MXD_GNS)CMemoryAllocator::SMemoryBlockExtraInfo(#varType, __FILE__, 0, __LINE__)))


#else // #if defined MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT

#define MX_NEW(varType) new (MX_NAMESPACE(MXD_GNS)CMemoryAllocator::Allocate( \
    sizeof(varType))) (varType)

#define MX_NEW_ARRAY(varType, uSize)                                            \
    MX_NAMESPACE(MXD_GNS)MX_NEW_ARRAY_Construct<varType>(                       \
    uSize,                                                                  \
    MX_NAMESPACE(MXD_GNS)CMemoryAllocator::Allocate(sizeof(uint32_t) + sizeof(varType) * (uSize)))

#endif // #if defined MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT

#define MX_DELETE(pInstance) MX_NAMESPACE(MXD_GNS)MX_DELETE_Destruct(pInstance)
#define MX_DELETE_ARRAY(pArray) MX_NAMESPACE(MXD_GNS)MX_DELETE_ARRAY_Destruct(pArray)

//M5T_INTERNAL_USE_END

// Overhead size of each memory block.
#if defined(MXD_MEMORY_ALLOCATOR_BOUND_CHECK_ENABLE_SUPPORT)
// Size of header + Bound Checksum
const size_t g_uMEMORY_BLOCK_OVERHEAD_SIZE =
    sizeof(MX_NAMESPACE(MXD_GNS)CMemoryAllocator::SMemoryBlockHeader) + sizeof(uint32_t);
#else // #if defined(MXD_MEMORY_ALLOCATOR_BOUND_CHECK_ENABLE_SUPPORT)
// Size of header
const size_t g_uMEMORY_BLOCK_OVERHEAD_SIZE =
    sizeof(MX_NAMESPACE(MXD_GNS)CMemoryAllocator::SMemoryBlockHeader);
#endif // #if defined(MXD_MEMORY_ALLOCATOR_BOUND_CHECK_ENABLE_SUPPORT)

#else // #if defined(MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT)

//<GROUP KERNEL_VARIABLES>
//
// Description:
// Overhead size of each memory block.
// There is no memory block overhead when the memory allocator is not enabled.
const size_t g_uMEMORY_BLOCK_OVERHEAD_SIZE = 0;

//M5T_INTERNAL_USE_BEGIN
#define MX_DELETE(pInstance) delete pInstance
#define MX_DELETE_ARRAY(pArray) delete [] pArray
#define MX_NEW(varType) new varType
#define MX_NEW_ARRAY(varType, uSize) new varType[uSize]
//M5T_INTERNAL_USE_END

// Report configuration errors at compile time
#if defined(MXD_MEMORY_ALLOCATOR_PROTECTION_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_BOUND_CHECK_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT) || \
    defined(MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT)
    #error MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT must be defined to enable\
           either MXD_MEMORY_ALLOCATOR_PROTECTION_ENABLE_SUPPORT, \
           MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT, \
           MXD_MEMORY_ALLOCATOR_BOUND_CHECK_ENABLE_SUPPORT, \
           MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT or \
           MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT.
#endif

#endif // #if defined(MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT)

MX_NAMESPACE_END(MXD_GNS)

// BRCM_CUSTOM - [add] Broadcom code allocated memory support
#if defined(MXD_BRCM_MEMORY_ALLOCATOR_WRAPPER_SUPPORT)

#define BRCM_NEW(varType) new (MX_NAMESPACE(MXD_GNS)CMemoryAllocator::Allocate(  \
    sizeof(varType),                                                             \
    MX_NAMESPACE(MXD_GNS)CMemoryAllocator::SMemoryBlockExtraInfo(                \
        #varType,                                                                \
        __FILE__,                                                                \
        MX_NAMESPACE(MXD_GNS)CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_BRCM_ALLOCATED, \
        __LINE__))) (varType)

#define BRCM_NEW_ARRAY(varType, uSize)                    \
    MX_NAMESPACE(MXD_GNS)MX_NEW_ARRAY_Construct<varType>( \
    uSize,                                                \
    MX_NAMESPACE(MXD_GNS)CMemoryAllocator::Allocate(      \
        sizeof(uint32_t) + sizeof(varType) * (uSize),     \
        MX_NAMESPACE(MXD_GNS)CMemoryAllocator::SMemoryBlockExtraInfo( \
            #varType,                                     \
            __FILE__,                                     \
            MX_NAMESPACE(MXD_GNS)CMemoryAllocator::SMemoryBlockExtraInfo::eMEMORY_BLOCK_BRCM_ALLOCATED, \
            __LINE__)))

#else //#if defined(MXD_BRCM_MEMORY_ALLOCATOR_WRAPPER_SUPPORT)

#define BRCM_NEW(varType) MX_NEW(varType)
#define BRCM_NEW_ARRAY(varType, uSize) MX_NEW_ARRAY(varType, uSize)

#endif //#if defined(MXD_BRCM_MEMORY_ALLOCATOR_WRAPPER_SUPPORT)

#define BRCM_DELETE(pInstance) MX_DELETE(pInstance)
#define BRCM_DELETE_ARRAY(pArray) MX_DELETE_ARRAY(pArray)
// BRCM_CUSTOM - [end] Broadcom code allocated memory support

#if 0
//==SDOC========================================================================
//<GROUP KERNEL_MACROS>
//
// Summary:
//      Creates an object.
//
//  Parameters:
//      varType:
//       Type of the object to create.
//
//  Returns:
//      A pointer to the created object.
//
// Description:
//      This macro creates an object of type varType.  That object must later
//      be deleted with MX_DELETE.
//      This macro replaces the new operator and should be used even if the
//      memory allocator is not enabled.
//
//      Two implementations of the macro are provided. When
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT is defined, the macro uses the
//      memory allocator. Otherwise, it uses the global delete operator. The
//      utilisation of the memory allocator centralizes the memory management
//      process. This gives us the ability to gather statistics, to perform
//      bound checking and memory leak detection.
//
// Warning:
//     For templated classes with multiple arguments (i.e. CMap<Item1, Item2>),
//     typedef MUST be used in order to use MX_NEW with these classes.
//     Otherwise, compilation errors will occur.
//
// Location:
//      Defined in "CMemoryAllocator.h".
//
// See Also:
//      MX_DELETE_ARRAY, MX_DELETE, MX_NEW_ARRAY,
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT.
//
//==EDOC========================================================================
#define MX_NEW(varType)

//==SDOC========================================================================
//<GROUP KERNEL_MACROS>
//
// Summary:
//      Deletes an object.
//
//  Parameters:
//      pInstance:
//       Pointer to the object to delete. If NULL, no operation is performed.
//
// Description:
//      This macro deletes an object created using MX_NEW.
//      This macro replaces the delete operator and should be used even if the
//      memory allocator is not enabled.
//
//      Two implementations of the macro are provided. When
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT is defined, the macro uses the
//      memory allocator. Otherwise, it uses the global delete operator. The
//      utilisation of the memory allocator centralizes the memory management
//      process. This gives us the ability to gather statistics, to perform
//      bound checking and memory leak detection.
//
//      Note that the memory allocator bypasses the protected and private access
//      controls of the classes' destructor. Therefore, it is possible to call a
//      non-public destructor using the MX_DELETE macro when
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT is defined.
//
//      IMPORTANT: This lack of control MUST not be used in any way. It is
//      STRICTLY PROHIBITED to implement code that will call MX_DELETE on a
//      pointer that has a protected or private destructor. Furthermore, the
//      code will not compile when MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT is not
//      defined.
//
// Location:
//      Defined in "CMemoryAllocator.h".
//
// See Also:
//      MX_DELETE_ARRAY, MX_NEW, MX_NEW_ARRAY,
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT.
//
//==EDOC========================================================================
#define MX_DELETE(pInstance)

//==SDOC========================================================================
//<GROUP KERNEL_MACROS>
//
// Summary:
//      Creates an array of objects.
//
//  Parameters:
//      varType:
//       Type of the objects to create.
//
//      uSize:
//       Size of the array to create
//
//  Returns:
//      A pointer to the first object of the created array of objects.
//
// Description:
//      This macro creates an array of uSize objects of type varType.  That
//      array must later be deleted with MX_DELETE_ARRAY.
//      This macro replaces the new[] operator and should be used even if the
//      memory allocator is not enabled.
//
//      Two implementations of the macro are provided. When
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT is defined, the macro uses the
//      memory allocator. Otherwise, it uses the global new operator. The
//      utilisation of the memory allocator centralizes the memory management
//      process. This gives us the ability to gather statistics, to perform
//      bound checking and memory leak detection.
//
// Location:
//      Defined in "CMemoryAllocator.h".
//
// See Also:
//      MX_DELETE_ARRAY, MX_DELETE, MX_NEW,
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT.
//
//==EDOC========================================================================
#define MX_NEW_ARRAY(varType, uSize)

//==SDOC========================================================================
//<GROUP KERNEL_MACROS>
//
// Summary:
//      Deletes an array of objects.
//
//  Parameters:
//      pArray:
//       A pointer to the first object of the array.
//
// Description:
//      This macro deletes an array of objects that was created using the
//      MX_NEW_ARRAY macro.
//      This macro replaces the delete[] operator and should be used even if the
//      memory allocator is not enabled.
//
//      Two implementations of the macro are provided. When
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT is defined, the macro uses the
//      memory allocator. Otherwise, it uses the global new operator. The
//      utilisation of the memory allocator centralizes the memory management
//      process. This gives us the ability to gather statistics, to perform
//      bound checking and memory leak detection.
//
// Location:
//      Defined in "CMemoryAllocator.h".
//
// See Also:
//      MX_NEW, MX_DELETE, MX_NEW_ARRAY,
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT.
//
//==EDOC========================================================================
#define MX_DELETE_ARRAY(pArray)

#endif // #if 0

#endif // MXG_CMEMORYALLOCATOR_H


