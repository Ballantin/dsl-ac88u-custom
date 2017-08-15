//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_IALLOCATOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IALLOCATOR_H
//M5T_INTERNAL_USE_END

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

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IAllocator
//========================================
//<GROUP CAP_CLASSES>
//
//  Summary:
//   Interface allowing the allocation and deallocation of free blocks.
//
// Description:
//   This interface allows the allocation and the deallocation of free blocks.
//   It is possible to allocate and release one or more blocks with one call.
//   To do this, free blocks must be chained and the list must be terminated
//   will a NULL pointer.
//
//   Note to the implementers:
//
//   1- Blocks may physically be of a bigger size than what was initially
//      required.
//   2- Blocks must never be packed. Each block must be accessible using
//      aligned memory access.
//   3- If Allocate fails because of an out of memory condition, the partially
//      allocated blocks must be freed and NULL must be returned.
//
// Location:
//   Cap/IAllocator.h
//
//==EDOC========================================================================
class IAllocator
{
// Published Structure
//---------------------
public:
    //<GROUP CAP_STRUCTS>
    //
    // Summary:
    //  Structure used to hold a chained list of memory blocks.
    struct SBlock
    {
        // Description:
        // Next memory block in the chained list.
        SBlock* pstNextBlock;
    };

// Published Interface
//---------------------
public:
    //==SDOC====================================================================
    //==
    //==  Allocate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allocates a list of SBlocks.
    //
    //  Parameters:
    //    uBlockCount:
    //      The number of blocks to allocate.
    //
    //    uBlockSize:
    //      The minimum size of each block. The allocated block size is at least
    //      sizeof(SBlock) in size.
    //
    //    ppstLastBlock:
    //      A pointer to a pointer to the last allocated block. It may be NULL.
    //      In such a case, the pointer to the last allocated block is not
    //      updated.
    //
    //  Returns:
    //      A pointer to the list of allocated blocks. NULL if the allocation
    //      fails.
    //
    //  Description:
    //      This method allocates a list of SBlocks. Each block is chained to
    //      the next by using the SBlock::pstNextBlock member. The last block of
    //      the list must be terminated by NULL. The block size is provided to
    //      ease the implementation of some allocators.
    //
    //      Some allocators do not support the block allocation of different
    //      sizes and are thus limited to one specific size.
    //
    //      It is important to understand that it is possible to get more
    //      allocated blocks than what was specified in the parameter. This is
    //      also to ease the implementation of some allocators.
    //
    //==EDOC====================================================================
    virtual SBlock* GO Allocate(IN unsigned int uBlockCount,
                                IN unsigned int uBlockSize,
                                IN SBlock** ppstLastBlock = NULL) = 0;

    //==SDOC====================================================================
    //==
    //==  Release
    //==
    //==========================================================================
    //
    //  Summary:
    //      Releases a list of SBlocks.
    //
    //  Parameters:
    //    pstBlock:
    //      A pointer to the list of allocated blocks that must be released.
    //
    //    uBlockSize:
    //      The minimum size of each block. It must be the same as the one that
    //      was previously passed to Allocate when the block was allocated.
    //
    //  Description:
    //      This method releases a list of SBlocks. Each block must be chained
    //      to the next by using the SBlock::pstNextBlock member. The last block
    //      of the list must be terminated by NULL. It is also possible to call
    //      Release multiple times to free a whole list of blocks if chaining
    //      them is not an option. Remember that you still need to set pstNext
    //      to NULL. The block size is provided to ease the implementation of
    //      some allocators.
    //
    //==EDOC====================================================================
    virtual void Release(IN TOA SBlock* pstBlock,
                         IN unsigned int uBlockSize) = 0;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  IAllocator
    //==
    //==========================================================================
    //
    // Summary:
    //     Constructor.
    //
    //  Description:
    //      Constructor.
    //
    //==EDOC====================================================================
    IAllocator() {}

    //==SDOC====================================================================
    //==
    //==  ~IAllocator
    //==
    //==========================================================================
    //
    // Summary:
    //     Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==EDOC====================================================================
    virtual ~IAllocator() {}

private:

// Hidden Data Members
//---------------------
protected:

//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_IALLOCATOR_H

