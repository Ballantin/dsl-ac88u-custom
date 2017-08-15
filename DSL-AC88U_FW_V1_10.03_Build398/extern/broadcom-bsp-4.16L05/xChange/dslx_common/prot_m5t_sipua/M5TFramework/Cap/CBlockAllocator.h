//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CBLOCKALLOCATOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CBLOCKALLOCATOR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)

//-- Data Members
//-----------------
#ifndef MXG_IALLOCATOR_H
#include "Cap/IAllocator.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CBlockAllocator
//========================================
//<GROUP CAP_CLASSES>
//
// Summary:
//  Manages a list of free blocks.
//
// Description:
//   CBlockAllocator manages a list of free blocks. It supports four different
//   modification methods:
//      Pop, Push, Reduce, and Reserve.
//
//   The Pop method retrieves a free block from the list. A new block is
//   allocated if the list of free blocks is empty.
//
//   The Push method returns a block to the list of free blocks. The memory
//   associated with the block is not released.
//
//   The Reserve method exists for performance reasons. When Reserve is not
//   called, each call to Pop forces a memory allocation if the list of free
//   blocks is empty. Reserve should be called to make sure a minimum number of
//   free blocks are contained within the list. If the list does not contain
//   enough blocks, they are allocated and added to the list.
//
//   The Reduce method exists for conditions where the list of free blocks is
//   too large. Reduce can be called to reduce the number of free blocks in the
//   list to a maximum level.
//
//   For example:
//
//      Calling Pop when the list contains 0 free block generates an allocation
//      of 1 block.
//
//      Calling Pop when the list contains more than 0 free blocks does not
//      generate an allocation.
//
//      Calling Push adds the block to the list of free blocks.
//
//      Calling Reserve(n) when the list already contains n or more free blocks
//      does nothing.
//
//      Calling Reserve(n) when the list contains less than n free blocks
//      forces the allocation of the missing blocks.
//
//      Calling Reduce(n) when the list already contains n or less free blocks
//      does nothing.
//
//      Calling Reduce(n) when the list contains more than n free blocks
//      forces the releasing of the extra blocks.
//
// Location:
//   Cap/CBlockAllocator.h
//
//==EDOC========================================================================
class CBlockAllocator
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    CBlockAllocator(IN unsigned int uBlockSize,
                    IN IAllocator* pAllocator = NULL);
    CBlockAllocator(IN const CBlockAllocator& rBlockAllocator);
    virtual ~CBlockAllocator();

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(const CBlockAllocator& rAllocator) const;
    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(const CBlockAllocator& rAllocator) const;

    // Summary:
    //  Retrieves a block from the list of free blocks. Allocates it if the list
    //  is empty.
    //--------------------------------------------------------------------------
    void* GO Pop();

    // Summary:
    //  Returns a block to the list of free blocks.
    //----------------------------------------------
    virtual void Push(IN TOA void* pBlock);

    // Summary:
    //  Retrieves the number of free blocks available.
    //-------------------------------------------------
    unsigned int GetBlockCount() const;

    // Summary:
    //  Reduces the list of free blocks to a maximum of uDownToBlockCount blocks.
    //--------------------------------------------------------------------------
    void Reduce(IN unsigned int uDownToBlockCount);

    // Summary:
    //  Increments the list of free blocks to a minimum of uUpToBlockCount blocks.
    //--------------------------------------------------------------------------
    bool Reserve(IN unsigned int uUpToBlockCount);

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:
private:

    CBlockAllocator() {}

    //  Allocates a block.
    IAllocator::SBlock* GO Allocate(INOUT unsigned int uBlockCount,
                                    IN unsigned int uBlockSize,
                                    IN IAllocator::SBlock** ppstLastBlock = NULL);

    //  Releases an allocated block.
    void Release(IN TOA IAllocator::SBlock* pBlock, IN unsigned int uBlockSize);


// Hidden Data Members
//---------------------
protected:

    IAllocator* m_pAllocator;
    IAllocator::SBlock* m_pstFirstBlock;
    IAllocator::SBlock* m_pstLastBlock;
    unsigned int m_uBlockCount;
    unsigned int m_uBlockSize;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//      rAllocator:
//       Reference to the block allocator to compare.
//
//  Returns:
//      True if both CBlockAllocators are equal, false otherwise.
//
//  Description:
//      Comparison operator. Checks if both block allocators are equal.
//
//==EDOC========================================================================
inline
bool CBlockAllocator::operator==(const CBlockAllocator& rAllocator) const
{
    return ! operator!=(rAllocator);
}

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rAllocator:
//       Reference to the block allocator to compare.
//
//  Returns:
//      True if both CBlockAllocators are different, false otherwise.
//
//  Description:
//      Different than operator. Checks if both block allocators are different.
//
//==EDOC========================================================================
inline
bool CBlockAllocator::operator!=(const CBlockAllocator& rAllocator) const
{
    // Two block allocators are different if they use different IAllocator...
    return m_pAllocator != rAllocator.m_pAllocator;
}


//==SDOC========================================================================
//==
//==  Pop
//==
//==============================================================================
//
//  Returns:
//      A pointer to the block. NULL if it fails.
//
//  Description:
//      Retrieves a block from the list of free blocks. Allocates it if the list
//      is empty. Returns NULL if out of memory. Calling Reserve before Pop is a
//      good way to ensure that blocks are available.
//
//==EDOC========================================================================
inline
void* GO CBlockAllocator::Pop()
{
    if (m_pstFirstBlock == NULL)
    {
        Reserve(1);
    }

    if (m_pstFirstBlock)
    {
        IAllocator::SBlock* pstBlock = m_pstFirstBlock;
        m_pstFirstBlock = m_pstFirstBlock->pstNextBlock;

        if (m_pstFirstBlock == NULL)
        {
            m_pstLastBlock = NULL;
        }

        m_uBlockCount--;

        return pstBlock;
    }

    return NULL;
}

//==SDOC========================================================================
//==
//==  Push
//==
//==============================================================================
//
//  Parameters:
//    pBlock:
//      A pointer to the block that must be returned to the list of free blocks.
//
//  Description:
//      Returns a block to the list of free blocks.
//
//==EDOC========================================================================
inline
void CBlockAllocator::Push(IN TOA void* pBlock)
{
    if (pBlock)
    {
        IAllocator::SBlock* pstBlock =
            reinterpret_cast<IAllocator::SBlock*>(pBlock);

        if (m_pstFirstBlock == NULL)
        {
            m_pstLastBlock = pstBlock;
        }

        pstBlock->pstNextBlock = m_pstFirstBlock;
        m_pstFirstBlock = pstBlock;
        m_uBlockCount++;
    }
}

//==SDOC========================================================================
//==
//==  GetBlockCount
//==
//==============================================================================
//
//  Returns:
//      The number of free blocks available.
//
//  Description:
//      Retrieves the number of free blocks available.
//
//==EDOC========================================================================
inline
unsigned int CBlockAllocator::GetBlockCount() const
{
    return m_uBlockCount;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // MXG_CBLOCKALLOCATOR_H

