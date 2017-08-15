//==SDOC========================================================================
//==============================================================================
//
//   Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation (“Media5”)
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CAATREEBASE_H
#define MXG_CAATREEBASE_H

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
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT, MXD_CAATREE_ENABLE_DEBUG
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)

//-- Data Members
//-----------------
#ifndef MXG_CALIGNEDVARIABLESTORAGE_H
#include "Basic/CAlignedVariableStorage.h"
#endif

#ifndef MXG_CBLOCKALLOCATOR_H
#include "Cap/CBlockAllocator.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class IAllocator;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CAATreeBase
//==============================================================================
//
// Description:
//   The CAATreeBase class provides an implementation for an untyped AA tree,
//   which is a type of Binary Search Tree.  It is used by the templated class
//   CAATree as a base class to avoid the duplication of code caused by the
//   templates.
//
//   The CAATreeBase class has the following properties:
//       Insert O(log n)
//       Erase O(log n)
//       Find O(log n)
//
// Location:
//   Cap/CAATreeBase.h
//
// See also:
//   CVectorBase, CVListBase, CListBase, CAATree
//
//==EDOC========================================================================
class CAATreeBase
{
// Hidden Structures
//---------------------
protected:
    // Structure holding the comparison function.
    struct SCompareVoidParams
    {
        int (*m_pfnCompare)(IN const void* pOneElement,
                            IN const void* pOtherElement,
                            IN mxt_opaque opq);
        mxt_opaque m_opq;

        // Constructor
        SCompareVoidParams(IN int (*pfnCompare)(IN const void* pOneElement,
                                                IN const void* pOtherElement,
                                                IN mxt_opaque opq),
                           IN mxt_opaque opq)
          : m_pfnCompare(pfnCompare), m_opq(opq)
        {
        }
    };

private:
    // Nodes of the tree
    class CAATreeNode
    {
    public:
        CAATreeNode* m_pLeft;
        CAATreeNode* m_pRight;
        CAATreeNode* m_pParent;
        unsigned int m_uLevel;

        // Constructor

        CAATreeNode()
          : m_pLeft(NULL),
            m_pRight(NULL),
            m_pParent(NULL),
            m_uLevel(0)
        {}

        CAATreeNode(IN CAATreeNode* pLeftNode,
                    IN CAATreeNode* pRightNode,
                    IN CAATreeNode* pParent,
                    IN unsigned int uNodeLevel = 1)
          : m_pLeft(pLeftNode),
            m_pRight(pRightNode),
            m_pParent(pParent),
            m_uLevel(uNodeLevel)
        {}
    };

// Published Interface
//---------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------
    CAATreeBase(IN unsigned int uSizeOfType,
                IN int (pfnCompare)(IN const void* pOneElement,
                                    IN const void* pOtherElement,
                                    IN mxt_opaque opq),
                IN mxt_opaque opq,
                IN IAllocator* pAllocator);
    CAATreeBase(IN const CAATreeBase& rAATreeBase);

protected:
    CAATreeBase(IN unsigned int uSizeOfType,
                IN int (pfnCompare)(IN const void* pOneElement,
                                    IN const void* pOtherElement,
                                    IN mxt_opaque opq),
                IN mxt_opaque opq,
                IN const CBlockAllocator& rBlockAllocator);

public:
    virtual ~CAATreeBase();

    CAATreeBase& operator=(IN const CAATreeBase& rAATreeBase);

    mxt_result SetComparisonFunction(IN int (pfnCompare)(IN const void* pOneElement,
                                                         IN const void* pOtherElement,
                                                         IN  mxt_opaque opq),
                                     IN  mxt_opaque opq);

    // Capacity (Max element count).
    unsigned int GetCapacity() const;
    unsigned int GetLockCapacity() const;
    void LockCapacity();
    mxt_result ReduceCapacity(IN unsigned int uDownToCapacity);
    mxt_result ReserveCapacity(IN unsigned int uUpToCapacity);
    void UnlockCapacity();

    // Size (Element count).
    unsigned int GetSize() const;
    bool IsEmpty() const;
    bool IsFull() const;

    // Index operations.
    unsigned int GetFirstIndex() const;
    unsigned int GetLastIndex() const;
    unsigned int GetEndIndex() const;
    unsigned int GetMinElementIndex() const;
    unsigned int GetMaxElementIndex() const;

    // Direct element access.
    const void* GetAt(IN unsigned int uIndex) const;
    void* GetAt(IN unsigned int uIndex);
    const void* operator[](IN unsigned int uIndex) const;
    void* operator[](IN unsigned int uIndex);

    // Insert/erase operations.
    mxt_result Insert(IN const void* pvoidContent);
    void EraseElement(IN const void* pvoidContent);
    void Erase(IN unsigned int uIndex);
    void EraseAll();

    // Advanced operations.
    GO void* Allocate();
    mxt_result InsertAllocated(IN TOA void* pElement);
    void EraseAllocated(IN TOA void* pElement);
    const void* FindPtr(IN const void* pvoidContent) const;
    void* FindPtr(IN const void* pvoidContent);

    // CMap methods.
    mxt_result AllocateSorted(IN const void* pvoidContent,
                              OUT void** ppAllocatedZone);

// Hidden Methods
//----------------
protected:
    virtual void Construct(OUT void* pvoid);

    virtual void ConstructFrom(OUT void* pvoidDestination,
                               IN const void* pvoidSource);

    virtual void Destruct(OUT void* pvoid);

private:
    enum EInsertAction
    {
        eINSERT_CONTENT,
        eALLOCATE_NODE_ONLY,
        eINSERT_ALREADY_ALLOCATED_NODE
    };

    void CopyTree(INOUT GO CAATreeNode** ppDestNode,
                  IN const CAATreeNode*  pSrcNode,
                  IN       CAATreeNode*  pParentNodeOfDestTree);
    CAATreeNode* FindNode(IN const void* pvoidContent) const;
    CAATreeNode* FindNodeByIndex(IN unsigned int uIndex) const;
    CAATreeNode* FindNodeMin(IN CAATreeNode* pNode) const;
    CAATreeNode* FindNodeMax(IN CAATreeNode* pNode) const;
    void FindAndBalanceTreeForErase(IN const void* pvoidContent,
                                    INOUT CAATreeNode** ppDeletedNode,
                                    INOUT CAATreeNode** ppLastNode,
                                    INOUT CAATreeNode** ppNode,
                                    OUT CAATreeNode** ppNodeToDelete,
                                    OUT CAATreeNode** ppSuccessorNode);
    void FreeTree(INOUT TOA CAATreeNode** ppNode);
    mxt_result Insert(IN const void* pvoidContent,
                      IN CAATreeNode* pParentNodeOfSubTree,
                      IN EInsertAction eActionToDo,
                      INOUT CAATreeNode** ppNode,
                      OUT CAATreeNode** ppInsertedNode = NULL);
    void Skew(INOUT CAATreeNode** ppNode);
    void Split(INOUT CAATreeNode** ppNode);

    // Current node manipulation.
    bool CurrentNodeIsInvalid() const;
    void DecrementCurrentNode() const;
    CAATreeNode* GetCurrentNode() const;
    void IncrementCurrentNode() const;
    void InvalidateCurrentNode() const;
    void SetCurrentNode(IN CAATreeNode* pNode) const;

    static mxt_result InitializeCAATreeBase();
    static void FinalizeCAATreeBase();

// Friends declarations
//----------------------
    friend mxt_result InitializeCAATreeBase();
    friend void FinalizeCAATreeBase();

// Hidden Data Members
//---------------------
protected:
    CBlockAllocator m_blockAllocator;
    unsigned int m_uSizeOfType;
    SCompareVoidParams m_stCompareVoidParams;

private:
    unsigned int m_uLockCapacity;
    unsigned int m_uSizeOfAATree;
    unsigned int m_uNbNotInsertedElements;
    CAATreeNode* m_pRoot;
    mutable CAATreeNode* m_pCurrentNode;
    mutable unsigned int m_uCurrentIndex;

    // The same null node can be used by all AA trees.
    static CAlignedVariableStorage<CAATreeNode> ms_nullNode;

// Debug methods
//---------------------
#ifdef MXD_CAATREE_ENABLE_DEBUG
public:
    void PrintTree();

private:
    void PrintTree(IN CAATreeNode* pNode, IN int nIndent = 0);
#endif //-- MXD_CAATREE_ENABLE_DEBUG
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
void CAATreeBase::Construct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);
    // BRCM_CUSTOM mod [begin] - Suppress warning
    int value = 0;
    MX_ASSERT(value);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(value);
    // BRCM_CUSTOM end [begin] - Suppress warning
}

inline
void CAATreeBase::ConstructFrom(OUT void* pvoidDestination,
                                IN const void* pvoidSource)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoidDestination);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoidSource);
    // BRCM_CUSTOM mod [begin] - Suppress warning
    int value = 0;
    MX_ASSERT(value);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(value);
    // BRCM_CUSTOM end [begin] - Suppress warning
}

inline
void CAATreeBase::Destruct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);
    // BRCM_CUSTOM mod [begin] - Suppress warning
    int value = 0;
    MX_ASSERT(value);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(value);
    // BRCM_CUSTOM end [begin] - Suppress warning
}

inline
unsigned int CAATreeBase::GetCapacity() const
{
    return m_uSizeOfAATree + m_uNbNotInsertedElements + m_blockAllocator.GetBlockCount();
}

inline
unsigned int CAATreeBase::GetLockCapacity() const
{
    return m_uLockCapacity;
}

inline
void CAATreeBase::LockCapacity()
{
    m_uLockCapacity++;
}

inline
void CAATreeBase::UnlockCapacity()
{
    MX_ASSERT(m_uLockCapacity);

    if (m_uLockCapacity)
    {
        m_uLockCapacity--;
    }
}

inline
unsigned int CAATreeBase::GetSize() const
{
    return m_uSizeOfAATree;
}

inline
bool CAATreeBase::IsEmpty() const
{
    return m_uSizeOfAATree == 0;
}

inline
bool CAATreeBase::IsFull() const
{
    return GetSize() == GetCapacity();
}

inline
unsigned int CAATreeBase::GetFirstIndex() const
{
    return 0;
}

inline
unsigned int CAATreeBase::GetLastIndex() const
{
    return m_uSizeOfAATree - 1;
}

inline
unsigned int CAATreeBase::GetEndIndex() const
{
    return GetLastIndex() + 1;
}

inline
unsigned int CAATreeBase::GetMinElementIndex() const
{
    return GetFirstIndex();
}

inline
unsigned int CAATreeBase::GetMaxElementIndex() const
{
    return IsEmpty()? 0 : GetLastIndex();
}

inline
const void* CAATreeBase::GetAt(IN unsigned int uIndex) const
{
    CAATreeNode* pNode = FindNodeByIndex(uIndex);
    return(pNode != ms_nullNode.Instance()? pNode + 1 : NULL);
}

inline
void* CAATreeBase::GetAt(IN unsigned int uIndex)
{
    CAATreeNode* pNode = FindNodeByIndex(uIndex);
    return(pNode != ms_nullNode.Instance()? pNode + 1 : NULL);
}

inline
const void* CAATreeBase::operator[](IN unsigned int uIndex) const
{
    CAATreeNode* pNode = FindNodeByIndex(uIndex);
    return(pNode != ms_nullNode.Instance()? pNode + 1 : NULL);
}

inline
void* CAATreeBase::operator[](IN unsigned int uIndex)
{
    CAATreeNode* pNode = FindNodeByIndex(uIndex);
    return(pNode != ms_nullNode.Instance()? pNode + 1 : NULL);
}

inline
const void* CAATreeBase::FindPtr(IN const void* pvoidContent) const
{
    CAATreeNode* pNode = FindNode(pvoidContent);
    return(pNode != ms_nullNode.Instance()? pNode + 1 : NULL);
}

inline
void* CAATreeBase::FindPtr(IN const void* pvoidContent)
{
    CAATreeNode* pNode = FindNode(pvoidContent);
    return(pNode != ms_nullNode.Instance()? pNode + 1 : NULL);
}

inline
void CAATreeBase::EraseAll()
{
    FreeTree(&m_pRoot);
    InvalidateCurrentNode();
}

// Manipulation of the current node
inline
bool CAATreeBase::CurrentNodeIsInvalid() const
{
    return m_pCurrentNode == ms_nullNode.Instance();
}

inline
CAATreeBase::CAATreeNode* CAATreeBase::GetCurrentNode() const
{
    return m_pCurrentNode;
}

inline
void CAATreeBase::InvalidateCurrentNode() const
{
    m_pCurrentNode = ms_nullNode.Instance();
}

inline
void CAATreeBase::SetCurrentNode(IN CAATreeNode* pNode) const
{
    m_pCurrentNode = pNode;
}

#ifdef MXD_CAATREE_ENABLE_DEBUG
inline
void CAATreeBase::PrintTree()
{
    PrintTree(m_pRoot);
}
#endif //-- MXD_CAATREE_ENABLE_DEBUG

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // MXG_CAATREEBASE_H

//M5T_INTERNAL_USE_END
