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
#ifndef MXG_CSUBALLOCATOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSUBALLOCATOR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)

#if defined(MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT)
    #ifndef MXG_CVECTOR_H
    #include "Cap/CVector.h"
    #endif
#endif

#ifndef MXG_IALLOCATOR_H
#include "Cap/IAllocator.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
#if defined(MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT)
class CString;
#endif

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//<GROUP CAP_VARIABLES>
// Summary:
//  This is the default memory block size for the memory blocks from which the
//  sub-allocation is performed.
//
// Description:
//  This is the default memory block size for the memory blocks from which the
//  sub-allocation is performed. Its value is controlled with the
//  MXD_CAP_SUBALLOCATOR_DEFAULT_MEMORY_BLOCK_SIZE_IN_BYTES macro.
const unsigned int g_uDEFAULT_MEMORY_BLOCK_IN_BYTES = MXD_CAP_SUBALLOCATOR_DEFAULT_MEMORY_BLOCK_SIZE_IN_BYTES;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSubAllocator
//==============================================================================
//<GROUP CAP_CLASSES>
//
// Summary:
//   This class performs sub-allocation in a huge memory block allocated upon
//   creation of the class.
//
// Description:
//   This class performs sub-allocation in a large memory block allocated upon
//   creation of the class. This is true when
//   MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT is defined, when it is not defined,
//   this class is only a wrapper over MX_NEW_ARRAY/MX_DELETE_ARRAY.
//
// #Sub-allocation when MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT is defined:#
//   Upon creation, this class allocates a big buffer (default size is 2048
//   bytes but it is fully customizable through the definition of
//   MXD_CAP_SUBALLOCATOR_DEFAULT_MEMORY_BLOCK_SIZE_IN_BYTES).
//
//   When the Allocate method is called, it returns a pointer in that memory
//   block. When the memory block has insufficient size for the allocation, a
//   new memory block of the same size or bigger is allocated and chained to the
//   others. The returned pointer MUST be released through the Release method
//   of the same allocator on which the Allocate call has been performed.
//
//   Furthermore, the pointers returned by Allocate are always aligned according
//   to MXD_MINIMAL_ALIGNMENT_IN_BYTES.
//
// #Sub-allocation when MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT is NOT defined:#
//   When the sub-allocator is not enabled, this class becomes a simple wrapper
//   over MX_NEW_ARRAY and MX_DELETE_ARRAY. Therefore, the
//   MXD_MINIMAL_ALIGNMENT_IN_BYTES constraint stated in the previous section
//   is not applicable. In fact, when MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT is
//   NOT defined, the default alignment of the platform prevails.
//
// Location:
//   Cap/CSubAllocator.h
//
//==============================================================================
class CSubAllocator : public IAllocator
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:
//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT)
    // This structure represents a sub-allocation memory block and is used to
    // manage each allocated large memory block.
    struct SSubAllocationBlock
    {
#if defined(MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT)
        // The allocated size for this memory block, includes the
        // SSubAllocationBlock structure size. Only needed by the statistics.
        unsigned int m_uAllocatedSizeInBytes;
#endif
        // Pointer to the next block.
        SSubAllocationBlock* m_pstNextBlock;

        // Initial available size of the block. This excludes the
        // SSubAllocationBlock structure size.
        unsigned int m_uInitialSizeInBytes;

        // Available size in bytes for allocation in the memory block.
        // Each call to Allocate will decrement this member as the available
        // size is decreased by each allocation.
        unsigned int m_uAvailableSizeInBytes;

        // Starting point of the memory block. (This pointer will always be
        // aligned according to MXD_MINIMAL_ALIGNMENT_IN_BYTES.)
        // The memory layout is as follows:
        //
        // sub-allocator memory block (Size: X Bytes)
        // +---------------------+---------------------------------------------+
        // | SSubAllocationBlock | memory buffer where sub-allocation is       |
        // |         m_puBuffer->| performed.                                  |
        // +---------------------+---------------------------------------------+
        // Byte 0                                                         Byte X
        //
        // In the schematic above, "m_puBuffer->" indicates that this member of
        // the management structure is set to point at the first byte that
        // follows the actual structure in memory. If that address is not
        // aligned according to MXD_MINIMAL_ALIGNMENT_IN_BYTES, the pointer is
        // advanced to the next address that is aligned.
        //
        // Each call to Allocate will further advance this pointer and its value
        // is kept aligned according to MXD_MINIMAL_ALIGNMENT_IN_BYTES.
        uint8_t* m_puBuffer;
    };
#endif
//M5T_INTERNAL_USE_END

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSubAllocator(IN unsigned int uMemoryBlockSizeInBytes = g_uDEFAULT_MEMORY_BLOCK_IN_BYTES);

    // Summary:
    //  Destructor.
    virtual ~CSubAllocator();

    //-- << IAllocator >>
    virtual SBlock* GO Allocate(IN unsigned int uBlockCount,
                                IN unsigned int uBlockSize,
                                IN SBlock** ppstLastBlock = NULL);

    virtual void Release(IN TOA SBlock* pstBlock,
                         IN unsigned int uBlockSize);

    // Summary:
    //  Returns the total number of allocated bytes.
    uint32_t GetTotalAllocatedBytes();

    // Summary:
    //  Returns the total number of available bytes.
    uint32_t GetTotalAvailableBytes();

    // Summary:
    //  Returns the total number of allocations.
    uint32_t GetTotalNumAllocations();

    // Summary:
    //  Returns the total number of releases.
    uint32_t GetTotaNumlReleases();

    // Summary:
    //  Releases all allocations and restores SubAllocator to the initial state.
    void Renew(IN unsigned int uMemoryBlockSizeInBytes = 0);


//M5T_INTERNAL_USE_BEGIN
    //-- << Statistics >>
#if defined(MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT)
    // Summary:
    //  Dumps the current statistics to the provided string.
    void DumpStatistics(INOUT CString& rstrOutput);
#endif

//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSubAllocator(IN const CSubAllocator& rSrc);

    // Summary:
    //  Assignment Operator.
    CSubAllocator& operator=(IN const CSubAllocator& rSrc);

    // << Helpers >>

#if defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT)
    // Summary:
    //  Creates and chains a new sub-allocation block.
    SSubAllocationBlock* AddNewSubAllocationBlock(IN unsigned int uSubAllocationBlockSizeInBytes);

    // Summary:
    //  Updates the offset in the memory sub-allocation block.
    void UpdateBufferPointerAndSize(IN unsigned int uAllocatedSize,
                                    INOUT SSubAllocationBlock*& rpstSubAllocationBlock);
#endif

//-- Hidden Data Members
protected:
private:

#if defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT)
#if defined(MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT)
    // Defines the operation performed by the sub-allocator.
    enum EOperation
    {
        eOP_ALLOCATE,
        eOP_RELEASE
    };

    // Keeps track of an operation (i.e. EOperation) performed by the
    // sub-allocator.
    struct SSubAllocationBlockStatisticsOperation
    {
        // The operation performed
        EOperation m_eOperation;

        // For eOP_ALLOCATE, the size in bytes that has been allocated.
        // For eOP_RELEASE, always 0.
        unsigned int m_uSizeInBytes;

        // The pointer returned by the Allocate method or provided to the
        // Release method. Meant to help keeping track of allocation and release
        // of memory buffers.
        SBlock* m_pstBlock;
    };

    // Keeps track of the sub-allocator statistics. These statistics are meant
    // for M5T's internal use only. They will allow M5T to optimize its code
    // that is meant for use with the sub-allocator.
    struct SSubAllocationBlockStatistics
    {
        // Total number of sub-allocation memory blocks allocated
        // by the sub-allocator.
        unsigned int m_uSubAllocationBlocksCount;

        // Number of sub-allocation memory blocks that were allocated because
        // the size given to Allocate was greater than or equal to
        // g_uDEFAULT_MEMORY_BLOCK_IN_BYTES.
        unsigned int m_uBigSubAllocationBlocksCount;

        // Number of calls to Allocate.
        unsigned int m_uAllocations;

        // Number of calls to Release.
        unsigned int m_uReleases;

        // The list of operations in the order in which they occured (FIFO).
        CVector<SSubAllocationBlockStatisticsOperation> m_vecstOperations;

    } m_stSubAllocationStatistics;
#endif //-- #if defined(MXD_CAP_SUBALLOCATOR_STATISTICS_ENABLE_SUPPORT)

    // The first allocated sub-allocation block (ie. tip of the chain).
    SSubAllocationBlock* m_pstSubAllocationBlock;

    // The default allocation block size passed in the constructor or in
    // in the call to Renew.
    uint32_t m_uDefaultAllocationBlockSize;

    // The total number of allocations.
    uint32_t m_uTotalAllocations;

    // The total number of releases;
    uint32_t m_uTotalReleases;

    // Alignment that MUST be respected.
    static const unsigned int ms_uMINIMAL_ALIGNMENT_IN_BYTES;
#endif //-- #if defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT)

//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==============================================================================
//==
//==  GetTotalNumAllocations
//==
//==============================================================================
//
//  Returns:
//      The total allocation number.
//
//  Description:
//      Returns the total number of allocations.
//      If MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT is not defined, this function
//      always returns 0.
//
//==============================================================================
inline uint32_t CSubAllocator::GetTotalNumAllocations()
{
#if defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT)
    return m_uTotalAllocations;
#else
    return 0;
#endif
}

//==============================================================================
//==
//==  GetTotalNumReleases
//==
//==============================================================================
//
//  Returns:
//      The total release number.
//
//  Description:
//      Returns the total number of releases.
//      If MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT is not defined, this function
//      always returns 0.
//
//==============================================================================
inline uint32_t CSubAllocator::GetTotaNumlReleases()
{
#if defined(MXD_CAP_SUBALLOCATOR_ENABLE_SUPPORT)
    return m_uTotalReleases;
#else
    return 0;
#endif
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CSUBALLOCATOR_H
