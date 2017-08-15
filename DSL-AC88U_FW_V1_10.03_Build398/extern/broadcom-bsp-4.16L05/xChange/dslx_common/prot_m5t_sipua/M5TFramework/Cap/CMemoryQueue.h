//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CMEMORYQUEUE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMEMORYQUEUE_H
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
//== Class: CMemoryQueue
//========================================
//<GROUP CAP_CLASSES>
//
// Summary:
//   Implements a fixed size queue containing memory blocks of variable length.
//
// Description:
//   This class implements a fixed size queue containing memory blocks of
//   variable length. It is possible to set the alignment for the start of the
//   allocated block in memory and is usually required for structures.
//
// WARNING:
//   The queue is ALWAYS the owner of the allocated memory block. Never keep or
//   access a pointer to the memory block after PushFinalize or PopFinalize is
//   called. After these methods are called, the pointer to the memory block
//   becomes invalid and all references to it must be set to NULL. If the data
//   is to be used and accessed later, you must make a copy of it in another
//   memory area.
//
//  Published Interface:
//
//      Initialize:
//          Allocates space for the queue and initializes its internal
//          variables.
//
//      Uninitialize:
//          Deallocates space for the queue.
//
//      Clear:
//          Resets the read and write pointers to the beginning of the queue.
//
//      GetBlockSize:
//          Returns the size of a block.
//
//      IsBlockInQueue:
//          Returns if a block is located within the queue.
//
//      IsEmpty:
//          Returns true if the queue is empty.
//
//      Push:
//          Makes sure space is available at the end of the queue and returns a
//          pointer to the allocated memory block.
//
//      PushFinalize:
//          Advances the write pointer to the next free memory zone. WARNING:
//          The user MUST always call PushFinalize after memory block access is
//          completed in order to complete the allocation.
//
//      PushAbort:
//          Ignores the last Push call.  Always call this method if the push
//          should be undone.
//
//      Pop:
//          Returns the oldest block within the queue and its size.
//
//      PopFinalize:
//          Frees the memory zone and advances the read pointer to the next
//          memory block. WARNING: The user MUST always call PopFinalize after
//          memory block access is completed in order to release and free the
//          block.
//
//      PopAbort:
//          Ignores the last Pop call.  Always call this method if the pop
//          should be undone.
//
// Location:
//   Architecture/CMemoryQueue.h
//
//
//==EDOC========================================================================
class CMemoryQueue
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //-------------------------------
    CMemoryQueue();

    // Summary:
    //  Destructor.
    //-------------------------------
    virtual ~CMemoryQueue();

    // Summary:
    //  Allocates space for the queue and initializes its internal variables.
    //------------------------------------------------------------------------
    bool Initialize(IN unsigned int uQueueSize,
                    IN unsigned int uMemoryAlignment = sizeof(unsigned int));
    // Summary:
    //  Deallocates space for the queue.
    //-----------------------------------
    void Uninitialize();

    // Summary:
    //  Resets the read and write pointers to the beginning of the queue.
    //--------------------------------------------------------------------
    void Clear();

    // Summary:
    //  Returns the size of a block.
    //-------------------------------
    unsigned int GetBlockSize(IN uint8_t* puBlock);

    // Summary:
    //  Returns if a block is located within the queue.
    //--------------------------------------------------
    bool IsBlockInQueue(IN uint8_t* puBlock);

    // Summary:
    //  Returns true if the queue is empty.
    //--------------------------------------
    bool IsEmpty();

    // Summary:
    //  Makes sure space is available at the end of the queue and returns a
    //  pointer to the allocated memory block.
    //-----------------------------------------------------------------------
    uint8_t* Push(IN unsigned int uSize);

    // Summary:
    //  Aborts the previous call to Push.
    //------------------------------------
    void PushAbort();

    // Summary:
    //  Finalizes the previous call to Push.
    //---------------------------------------
    void PushFinalize(IN unsigned int uSize = 0);

    // Summary:
    //  Returns a pointer to the first block of the queue.
    //-----------------------------------------------------
    uint8_t* Pop(OUT unsigned int* puSize);

    // Summary:
    //  Aborts the previous call to Pop.
    //-----------------------------------
    void PopAbort();

    // Summary:
    //  Finalizes the previous call to Pop.
    //--------------------------------------
    void PopFinalize();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//-------------------
private:

    // Deactivated Constructors / Destructors / Operators
    //---------------------------------------------------
    CMemoryQueue(IN CMemoryQueue& from);
    CMemoryQueue& operator=(IN CMemoryQueue& from);

// Hidden Data Members
//------------------------
private:

    // A pointer to the allocated uint8_t buffer used for the queue.
    // Must always be aligned on sizeof(unsigned int).
    uint8_t* m_puQueue;
    // A pre-calculated pointer to the end of the allocated queue.
    // It is always located one uint8_t more than the last uint8_t of the queue.
    uint8_t* m_puQueueEnd;
    // This is the pointer to the decreased end of the queue.
    // This is required when a write is tried and does not fit in the rest of
    // the queue.  In such a case, the queue size is artificially reduced and
    // the write position wraps to the beginning of the queue (if there is
    // enough space left).
    // When different than m_puQueueEnd, it must always be aligned on
    // sizeof(unsigned int).
    uint8_t* m_puQueueEndTrunked;
    // This is the current read position within the queue.
    // Must always be aligned on sizeof(unsigned int).
    uint8_t* m_puQueueRead;
    // This is the current write position within the queue.
    // Must always be aligned on sizeof(unsigned int).
    uint8_t* m_puQueueWrite;

    // This is the alignment factor by which each allocated buffer is aligned.
    unsigned int m_uMemoryAlignment;

    // These variables are used in assertions to verify that the user does not
    // do illegal actions within the queue.
    MX_ASSERT_ONLY(bool m_bPopFinalized;)
    MX_ASSERT_ONLY(bool m_bPushFinalized;)
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//== Clear
//==
//==============================================================================
//
//  Description:
//      Resets the read and write pointers to the beginning of the queue.
//
//==============================================================================
inline
void CMemoryQueue::Clear()
{
    MX_ASSERT_RT(m_bPushFinalized && m_bPopFinalized);

    m_puQueueEndTrunked = m_puQueueEnd;
    m_puQueueRead = m_puQueue;
    m_puQueueWrite = m_puQueue;
}

//==============================================================================
//==
//== GetBlockSize
//==
//==============================================================================
//
//  Parameters:
//      puBlock:
//       Pointer to a block of memory.
//
//  Returns:
//      The size of the memory block.
//
//  Description:
//      Returns the size of a block.
//
//==============================================================================
inline
unsigned int CMemoryQueue::GetBlockSize(IN uint8_t* puBlock)
{
    MX_ASSERT_RT(IsBlockInQueue(puBlock));

    unsigned int uSizeOfUnsignedInt = sizeof(unsigned int);

    return
        *reinterpret_cast<unsigned int*>
            (puBlock - MX_ALIGNUP_VAL(uSizeOfUnsignedInt, m_uMemoryAlignment));
}

//==============================================================================
//==
//== IsBlockInQueue
//==
//==============================================================================
//
//  Parameters:
//      puBlock:
//       Pointer to a block of memory.
//
//  Returns:
//      True if the block is located within the queue, false otherwise.
//
//  Description:
//      Returns if a block is located within the queue.
//
//==============================================================================
inline
bool CMemoryQueue::IsBlockInQueue(IN uint8_t* puBlock)
{
    return puBlock > m_puQueue && puBlock < m_puQueueEnd;
}

//==============================================================================
//==
//== IsEmpty
//==
//==============================================================================
//
//  Returns:
//      True when empty, false otherwise.
//
//  Description:
//      Returns true if the queue is empty.
//
//==============================================================================
inline
bool CMemoryQueue::IsEmpty()
{
    return m_puQueueRead == m_puQueueWrite;
}

//==============================================================================
//==
//== PushAbort
//==
//==============================================================================
//
//  Description:
//      The method must be called if the user wants to abort the push procedure.
//
//==============================================================================
inline
void CMemoryQueue::PushAbort()
{
    MX_ASSERT_RT(m_bPushFinalized == false);
    MX_ASSERT_ONLY(m_bPushFinalized = true;)
}

//==============================================================================
//==
//== PushFinalize
//==
//==============================================================================
//
//  Parameters:
//      uSize:
//        Offers the possibility to reduce the size of the pushed block. If 0,
//        it keeps the size received in Push.
//
//  Description:
//      Advances the write pointer to the next free memory zone.
//
//  Note:
//      The user MUST always call PushFinalize after memory block access is
//      completed in order to complete the allocation.
//
//  Warning:
//      uSize cannot be greater than the uSize passed in the previous call to
//      Push.
//
//  Warning:
//      Never keep or access a pointer to the memory block after PushFinalize or
//      PopFinalize is called. After these methods are called, the queue becomes
//      the owner of the memory block and any external pointer must be set to
//      NULL. If the data is to be used and accessed later, you must make a copy
//      of it in another memory area.
//
//==============================================================================
inline
void CMemoryQueue::PushFinalize(IN unsigned int uSize)
{
    MX_ASSERT_RT(m_bPushFinalized == false);
    MX_ASSERT_RT(uSize <= *reinterpret_cast<unsigned int*>(m_puQueueWrite));

    if (uSize == 0)
    {
        // Assume that m_puQueueWrite is aligned on sizeof(unsigned int)
        uSize = *reinterpret_cast<unsigned int*>(m_puQueueWrite);
    }
    else
    {
        // Assume that m_puQueueWrite is aligned on sizeof(unsigned int)
        *reinterpret_cast<unsigned int*>(m_puQueueWrite) = uSize;
    }

    unsigned int uSizeOfUnsignedInt = sizeof(unsigned int);

    m_puQueueWrite +=
        MX_ALIGNUP_VAL(MX_ALIGNUP_VAL(uSizeOfUnsignedInt, m_uMemoryAlignment) + uSize, sizeof(unsigned int));

    // This is required for the unique case where the last push fills completly
    // the queue.
    if (m_puQueueWrite == m_puQueueEnd)
    {
        m_puQueueWrite = m_puQueue;
    }

    // NOTE: m_puQueueWrite will never be >= m_puQueue because alignment is
    // performed inside Push method.

    MX_ASSERT_RT(IsEmpty() == false);
    MX_ASSERT_ONLY(m_bPushFinalized = true;)
}

//==============================================================================
//==
//== Pop
//==
//==============================================================================
//
//  Parameters:
//      puSize:
//        Contains the size of the retrieved block. If the size is unnecessary,
//        NULL can be passed.
//
//  Returns:
//      Returns the allocated block, NULL if the queue is empty.
//
//  Description:
//      This method returns the oldest block within the queue and its size.
//      The user MUST always call PopFinalize when it is finished with the
//      block.  This releases the memory associated with the block.
//
//==============================================================================
inline
uint8_t* CMemoryQueue::Pop(OUT unsigned int* puSize)
{
    MX_ASSERT_RT(m_bPopFinalized);
    MX_ASSERT_RT(m_puQueue != NULL);

    uint8_t* puBlock = NULL;

    if (IsEmpty() == false)
    {
        if (puSize != NULL)
    {
        // Assume that m_puQueueRead is aligned on sizeof(unsigned int)
        *puSize = *reinterpret_cast<unsigned int*>(m_puQueueRead);
        }

        puBlock = MX_ALIGNUP_PTR(m_puQueueRead + sizeof(unsigned int),
                                 m_uMemoryAlignment);
    }

    MX_ASSERT_ONLY(m_bPopFinalized = puBlock == NULL;)

    return puBlock;
}

//==============================================================================
//==
//== PopAbort
//==
//==============================================================================
//
//  Description:
//      This method must be called if the user wants to abort the pop procedure.
//
//==============================================================================
inline
void CMemoryQueue::PopAbort()
{
    MX_ASSERT_RT(m_bPopFinalized == false);
    MX_ASSERT_ONLY(m_bPopFinalized = true;)
}

//==============================================================================
//==
//== PopFinalize
//==
//==============================================================================
//
//  Description:
//      Advances the write pointer to the next free memory zone.
//
//  Note:
//      The user MUST always call PushFinalize after the memory block access is
//      completed in order to complete the allocation.
//
//  Warning:
//      Never keep or access a pointer to the memory block after PushFinalize or
//      PopFinalize is called. After these methods are called, the queue becomes
//      the owner of the memory block and any external pointers must be set to
//      NULL. If the data is to be used and accessed later, you must make a copy
//      of it in another memory area.
//
//==============================================================================
inline
void CMemoryQueue::PopFinalize()
{
    MX_ASSERT_RT(m_bPopFinalized == false);

    // We already assume that m_puQueueRead is aligned on sizeof(unsigned int)
    unsigned int uSize = *reinterpret_cast<unsigned int*>(m_puQueueRead);

    unsigned int uSizeOfUnsignedInt = sizeof(unsigned int);

    m_puQueueRead +=
        MX_ALIGNUP_VAL(MX_ALIGNUP_VAL(uSizeOfUnsignedInt, m_uMemoryAlignment) + uSize, sizeof(unsigned int));

    if (m_puQueueRead >= m_puQueueEndTrunked)
    {
        m_puQueueRead = m_puQueue;
        m_puQueueEndTrunked = m_puQueueEnd;
    }

    MX_ASSERT_ONLY(m_bPopFinalized = true;)
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // MXG_CMEMORYQUEUE_H

