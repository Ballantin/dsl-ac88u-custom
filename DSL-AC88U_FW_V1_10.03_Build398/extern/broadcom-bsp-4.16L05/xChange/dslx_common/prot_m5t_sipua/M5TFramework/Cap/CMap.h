//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
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
#ifndef MXG_CMAP_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMAP_H
//M5T_INTERNAL_USE_END

//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================

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

// Interface Realized & Parent

// Other
#ifndef MXG_CAATREE_H
#include "Cap/CAATree.h"
#endif

#ifndef MXG_CPAIR_H
#include "Cap/CPair.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class IAllocator;

template<class _KeyType,
         class _ElementType,
         class _ContainerType>
class CMap;

template<class _Type>
class CAATree;

template<class _Type>
class CVector;

template<class _Type>
class CList;

template<class _Type>
class CVList;



//===========================================================================
//====  CONSTANTS + DEFINES  ================================================
//===========================================================================

//===========================================================================
//====  NEW TYPE DEFINITIONS  ===============================================
//===========================================================================
//==SDOC========================================================================
//== Class: CMapPair
//========================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//      Class used to handle pairs of elements in map files.
//
// Description:
//      This class is used to handle pairs of elements inside map files. These
//      pairs usually consist of a key and some other data. It contains methods
//      to create and compare pairs.
//
// Location:
//   Cap/CMap.h
//
// See Also:
//   CMap, CPair
//
//==EDOC========================================================================
template<class _Type, class _Type2>
class CMapPair : public CPair<_Type, _Type2>
{
//M5T_INTERNAL_USE_BEGIN
friend class CMap<_Type, _Type2, CAATree<CMapPair<_Type, _Type2> > >;
friend class CMap<_Type, _Type2, CVector<CMapPair<_Type, _Type2> > >;
friend class CMap<_Type, _Type2, CList<CMapPair<_Type, _Type2> > >;
friend class CMap<_Type, _Type2, CVList<CMapPair<_Type, _Type2> > >;
//M5T_INTERNAL_USE_END

public:
    //==SDOC====================================================================
    //==
    //==  CMapPair
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor
    //
    //  Description:
    //      Default constructor.
    //
    //==EDOC====================================================================
    CMapPair() :
        CPair<_Type, _Type2>()
    {
    }

    //==SDOC====================================================================
    //==
    //==  CMapPair
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor
    //
    //  Parameters:
    //      rFirst:
    //          First element to add to the pair.
    //
    //      rSecond:
    //          Second element to add to the pair.
    //
    //  Description:
    //      Constructor. Builds a CMapPair from the two elements.
    //
    //==EDOC====================================================================
    CMapPair(const _Type& rFirst, const _Type2& rSecond) :
        CPair<_Type, _Type2>(rFirst, rSecond)
    {
    }

    //==SDOC====================================================================
    //==
    //==  CMapPair
    //==
    //==========================================================================
    //
    //  Summary:
    //      Copy constructor
    //
    //  Parameters:
    //      rPair:
    //          The pair to copy.
    //
    //  Description:
    //      Copies the contents of the passed pair into this CMapPair.
    //
    //==EDOC====================================================================
    CMapPair(const CPair<_Type, _Type2>& rPair) :
        CPair<_Type, _Type2>(rPair)
    {
    }

    //==SDOC====================================================================
    //==
    //==  ~CMapPair
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==EDOC====================================================================
    virtual ~CMapPair()
    {
    }

    //==SDOC====================================================================
    //==
    //==  GetFirst
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the first element.
    //
    //  Returns:
    //      A reference to the first element.
    //
    //  Description:
    //      Gets the first element stored in the CMapPair.
    //
    //==EDOC====================================================================
    const _Type& GetFirst() const
    {
        return CPair<_Type, _Type2>::GetFirst();
    }

    //<COMBINE CMapPair::GetFirst@const>
    _Type& GetFirst()
    {
        return CPair<_Type, _Type2>::GetFirst();
    }

    //==SDOC====================================================================
    //==
    //==  operator<
    //==
    //==========================================================================
    //
    //  Summary:
    //      Less than operator.
    //
    //  Parameters:
    //      rPair:
    //          Right hand pair.
    //
    //  Returns:
    //      True if the left hand CMapPair is less than the right hand one.
    //
    //  Description:
    //      Verifies if the left hand CMapPair is less than the right hand one.
    //
    //==EDOC====================================================================
    bool operator< (const CMapPair<_Type, _Type2>& rPair) const
    {
        return (GetFirst() < rPair.GetFirst());
    };

    //==SDOC====================================================================
    //==
    //==  operator!=
    //==
    //==========================================================================
    //
    //  Summary:
    //      Different than operator.
    //
    //  Parameters:
    //      rPair:
    //          Right hand pair.
    //
    //  Returns:
    //      True if the first element of each CMapPair is different.
    //
    //  Description:
    //      Verifies that the left hand CMapPair is different than the left hand
    //      one.
    //
    //==EDOC====================================================================
    bool operator!= (const CMapPair<_Type, _Type2>& rPair) const
    {
        return (GetFirst() != rPair.GetFirst());
    };

//M5T_INTERNAL_USE_BEGIN
private:
    CMapPair(const _Type& rFirst) :
        CPair<_Type, _Type2>(rFirst)
    {
    }
//M5T_INTERNAL_USE_END
};


//@@CMap
//==SDOC========================================================================
//== Class: CMap
//========================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//  Implements an associative container that allows to retrieve values by using
//  keys.
//
// Description:
//   The CMap class implements an associative container that allows to retrieve
//   values by using keys. It is equivalent to the STL class "map".
//
//   The properties of the CMap are those of its underlying container. By
//   default, the CMap class uses a CAATree to store its elements (i.e. its
//   pairs of key-value).  Hence, the default properties of the CMap are those
//   of the CAATree class:
//      - Insert: O(log n)
//      - Erase:  O(log n)
//      - Find:   O(log n)
//
//   It may happen that a tree is not the most appropriate structure to use
//   inside the map.  For example, it could be more efficient to use a CVector
//   when memory requirements are very tight or when frequent iterations over
//   the map are necessary.  The CMap class allows to supply the container to
//   use by a template parameter.  Because the syntax to do this is a little
//   cumbersome, the following are two examples of definitions of a CMap, one
//   using a CAATree and another using a CVector as their underlying containers:
//
//  <CODE>
//
//      // Defines a CMap that associates an int key with a double value.  The
//      // container used is a CAATree.
//      CMap<int, double> CMap1;
//
//      // Defines a CMap that associates a char key with a pointer on a CVersion.
//      // The container used is a CVector.
//      CMap<char, CVersion*, CVector<CMapPair<char, CVersion*> > > CMap2;
//
//  </CODE>
//
//   The CMap class currently supports the CAATree, CVector, CList, and CVList
//   containers.  The CAATree (the default container) is the fastest one for
//   searches and most operations.  The performance is however improved for the
//   containers that use an array (CVector, CVList) by always keeping them
//   sorted.  The CList container is also kept sorted when used by the CMap,
//   although that does not affect performance very much.
//
//   In summary, the CMap:
//      - Allows searches of elements when their key is known.
//      - Does not allow association of multiple elements to the same key.
//      - Stores objects in an ordered way (defined by a comparison function).
//      - Manages its own memory buffer via its internal container.
//      - Auto increases its capacity when needed.
//      - Reduces its capacity when requested.
//      - Keeps track of how many objects are currently stored.
//      - Calls the keys' and values' constructors automatically at construction.
//      - Calls the keys' and values' destructors automatically at destruction.
//
// #When to choose CMap:#
//
//   It is important to choose your container with care.  Maps are useful when
//   you need to retrieve a value by using a key.  This kind of container is
//   often used to create dictionaries and can be thought of as an array for
//   which the index does not need to be an integer.
//
//   Choose a CMap mainly when you need to efficiently retrieve data according
//   to a particular key.  The other advantages/disadvantages are the ones of
//   the underlying container.
//
//   You MUST avoid the CMap when the keys used cannot be compared between
//   themselves or when they can be inserted twice in the container.
//
// #Reminders on how to use CMap:#
//
//      1. When the keys or the values used in the CMap are pointers, do not
//         forget to delete them before the container is destroyed.  You
//         otherwise produce memory leaks because the CMap only destroys the
//         pointers and not to what they point. Before deleting a CMap using
//         pointers as keys or values, you should always write something like
//         the following:
//
//            <CODE>
//
//            if(!pMap->IsEmpty())
//            {
//                unsigned int uIndex;
//                unsigned int uFirstIndex = pMap->GetFirstIndex();
//                unsigned int uLastIndex = pMap->GetLastIndex();
//                for (uIndex = uFirstIndex; uIndex <= uLastIndex; uIndex++)
//                {
//                    // _KeyType and _ValueType are of type CVersion*
//                    CPair<CVersion* const, CVersion*>& rData = (*pMap)[uIndex];
//                    MX_DELETE(rData.GetFirst());  // Delete key
//                    MX_DELETE(rData.GetSecond()); // Delete value
//                }
//                MX_DELETE(pMap);
//                pMap = NULL;
//             }
//
//             </CODE>
//
//         Note that even if the operator[] returns a
//         CPair\<const _KeyType, _ValueType\>&, the const of the first template
//         parameter of the CPair must be placed after the CVersion* expression.
//         The reason for this is that the template mechanism in C++ does not
//         do a simple text substitution in this case. It rather applies the
//         const to '_KeyType', yielding a const pointer to a CVersion in this
//         example.
//
//      2. Use the method ReserveCapacity to avoid unnecessary reallocations.
//         If you neglect to do so, each new insertion in a CMap that is too
//         small makes it grow by 1 element, added to the unavoidable overhead.
//
//      3. The default comparison function of the CMap uses the standard
//         operators between keys.  You must override these operators for the
//         type of key or supply a custom comparison function if you want them
//         to be compared differently.
//
//      4. The CMap class does not support duplicate values. A possible solution
//         for this issue may be to associate a container with each key.
//
//      5. When iterating by index and using the Erase method, you have
//         to make sure that the index is valid for the whole iteration scope.
//         Since the Erase method affects the size of the container, the
//         operator[] or the GetAt method should be used with caution. A
//         good practice is to use the GetSize method within the iteration
//         scope such as the following example:
//
//          <CODE>
//              CMap<SomeStruct> mapSomeStruct;
//              unsigned int i = 0;
//              for ( i = 0 ; i < mapSomeStruct.GetSize(); i++ )
//              {
//                  if ( mapSomeStruct[i].bToErase )
//                  {
//                      mapSomeStruct.Erase(i);
//                      i--;
//                  }
//              }
//          </CODE>
//
//          Another good practice is to iterate from the end such as the
//          following example:
//
//          <CODE>
//              CMap<SomeStruct> mapSomeStruct;
//              unsigned int i = 0;
//              for ( i = mapSomeStruct.GetSize(); i > 0; i-- )
//              {
//                  if ( mapSomeStruct[i-1].bToErase )
//                  {
//                      mapSomeStruct.Erase(i-1);
//                  }
//              }
//          </CODE>
//
//      6. CMap fully supports incomparable types such as structures or complex
//         objects. However since the != and < operators are required by the
//         CMap class, the user can use the CUncmp template helper rather than
//         implement operators. Please refer to the CUncmp class documentation
//         for more information and code examples.
//
// Warning:
//   This container is not thread safe.
//
// Location:
//   Cap/CMap.h
//
// See Also:
//   CAATree, CVector, CList, CVList
//
//==EDOC========================================================================
// M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
// GCC 2.7.2 does not support default parameters in templates,
// in which case all the parameters must be specified.
template<class _KeyType,
         class _ValueType,
         class _ContainerType>
#else
// M5T_INTERNAL_USE_END
template<class _KeyType,
         class _ValueType,
         class _ContainerType = CAATree<CMapPair<_KeyType, _ValueType> > >
// M5T_INTERNAL_USE_BEGIN
#endif
// M5T_INTERNAL_USE_END
class CMap
{
//-- Published Interface
//------------------------
public:

    // Constructors / Destructors / Operators
    //-------------------------------------------------

    //==SDOC====================================================================
    //==
    //==  CMap
    //==
    //==========================================================================
    //
    //  Summary:
    //     Constructor.
    //
    //  Parameters:
    //    pAllocator:
    //      Allows to specify an allocator used to allocate and free the
    //      elements.
    //
    //  Description:
    //      Default constructor.
    //
    //==EDOC====================================================================
    CMap(IN IAllocator* pAllocator = NULL);

    //==SDOC====================================================================
    //==
    //==  CMap
    //==
    //==========================================================================
    //
    //  Summary:
    //     Copy constructor.
    //
    //  Parameters:
    //    rMap
    //      A reference to the source map.
    //
    //  Description:
    //      Copy constructor.
    //
    //==EDOC====================================================================
    CMap(IN const CMap<_KeyType, _ValueType, _ContainerType>& rMap);

    //==SDOC====================================================================
    //==
    //==  ~CMap
    //==
    //==========================================================================
    //
    //  Summary:
    //     Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==EDOC====================================================================
    virtual ~CMap();

    //==SDOC====================================================================
    //==
    //==  operator=
    //==
    //==========================================================================
    //
    //  Summary:
    //      Assignment operator.
    //
    //  Parameters:
    //      rMap:
    //          Right hand CMap to assign to this.
    //
    //  Returns:
    //      A reference to this.
    //
    //  Description:
    //      Assigns the right hand CMap to the left hand CMap.
    //
    //==EDOC====================================================================
    CMap<_KeyType, _ValueType, _ContainerType>&
    operator=(IN const CMap<_KeyType, _ValueType, _ContainerType>& rMap);

    //==SDOC====================================================================
    //==
    //==  SetComparisonFunction
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the comparison function for the keys of the map.
    //
    //  Parameters:
    //      pfnCompare:
    //          Pointer to a comparison function.
    //      opq:
    //          Opaque comparison parameter.
    //  Returns:
    //      resS_OK
    //      resFE_INVALID_STATE
    //
    //  Description:
    //      Sets the comparison function to be used by the algorithms for the
    //      keys of this instance of the map.  The comparison function needs to
    //      return one of the following values:
    //          < 0 if rOneKey is "smaller" than rOtherKey.
    //          > 0 if rOneKey is "bigger" than rOtherKey.
    //          0 if rOneKey and rOtherKey are equivalent.
    //
    //      This method must be called only when the map is empty, otherwise
    //      it returns resFE_INVALID_STATE.
    //
    //==EDOC====================================================================
    mxt_result SetComparisonFunction(IN int (pfnCompare)(IN const _KeyType& rOneKey,
                                                         IN const _KeyType& rOtherKey,
                                                         IN mxt_opaque opq),
                                     IN  mxt_opaque opq);


    // Capacity (Max element count).
    //-------------------------------

    //==SDOC====================================================================
    //==
    //==  GetCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the capacity of the container.
    //
    //  Returns:
    //      The capacity.
    //
    //  Description:
    //      Returns the capacity of the container. The capacity is the number of
    //      elements already allocated that may or may not be already in use.
    //
    //==EDOC====================================================================
    unsigned int GetCapacity() const;

    //==SDOC====================================================================
    //==
    //==  GetLockCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the lock capacity count.
    //
    //  Returns:
    //      The lock capacity count.
    //
    //  Description:
    //      Returns the lock capacity count. The lock capacity count is a
    //      counter that is increased each time LockCapacity is called and
    //      decreased each time UnlockCapacity is called. If the lock capacity
    //      count is greater than zero, this means that any call to
    //      ReduceCapacity or ReserveCapacity fails.
    //
    //==EDOC====================================================================
    unsigned int GetLockCapacity() const;

    //==SDOC====================================================================
    //==
    //==  LockCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Locks the capacity.
    //
    //  Description:
    //      Locks the capacity. The lock capacity count is a counter that is
    //      increased each time LockCapacity is called and decreased each time
    //      UnlockCapacity is called. If the lock capacity count is greater than
    //      zero, this means that any call to ReduceCapacity or ReserveCapacity
    //      fails.
    //
    //==EDOC====================================================================
    void LockCapacity();

    //==SDOC====================================================================
    //==
    //==  ReduceCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reduces the capacity.
    //
    //  Parameters:
    //    uDownToCapacity:
    //      The wanted capacity.
    //
    //  Returns:
    //      resS_OK
    //      resFE_INVALID_STATE
    //
    //  Description:
    //      Reduces the capacity. The capacity is reduced to uDownToCapacity if
    //      uDownToCapacity is less than the current capacity. The method fails
    //      if the lock capacity count is not 0.
    //
    //==EDOC====================================================================
    mxt_result ReduceCapacity(IN unsigned int uDownToCapacity);

    //==SDOC====================================================================
    //==
    //==  ReserveCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Increases the capacity.
    //
    //  Parameters:
    //    uUpToCapacity:
    //      The wanted capacity.
    //
    //  Returns:
    //      resS_OK
    //      resFE_INVALID_STATE
    //      resFE_OUT_OF_MEMORY
    //
    //  Description:
    //      Increases the capacity. The capacity is increased to uUpToCapacity
    //      if uUpToCapacity is greater than the current capacity. The method
    //      fails if the lock capacity count is not 0 and the wanted capacity is
    //      greater than the current capacity.
    //
    //==EDOC====================================================================
    mxt_result ReserveCapacity(IN unsigned int uUpToCapacity);

    //==SDOC====================================================================
    //==
    //==  UnlockCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Unlocks the capacity.
    //
    //  Description:
    //      Unlocks the capacity. The lock capacity count is a counter that is
    //      increased each time LockCapacity is called and decreased each time
    //      UnlockCapacity is called. If the lock capacity count is greater than
    //      zero, this means that any call to ReduceCapacity or ReserveCapacity
    //      fails.
    //
    //==EDOC====================================================================
    void UnlockCapacity();


    // Size (Element count).
    //-----------------------

    //==SDOC====================================================================
    //==
    //==  GetSize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the size of the map.
    //
    //  Returns:
    //      The size.
    //
    //  Description:
    //      Returns the size. This is the number of elements that are allocated
    //      and already in use.
    //
    //==EDOC====================================================================
    unsigned int GetSize() const;

    //==SDOC====================================================================
    //==
    //==  IsEmpty
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates if the container is empty.
    //
    //  Returns:
    //      True if the size is 0.
    //
    //  Description:
    //      Returns true if the size is 0; in other words, if no elements are
    //      currently in use.
    //
    //==EDOC====================================================================
    bool IsEmpty() const;

    //==SDOC====================================================================
    //==
    //==  IsFull
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates if the container is full.
    //
    //  Returns:
    //      True when the container is full according to its capacity.
    //
    //  Description:
    //      This method returns true when the size of the container equals its
    //      capacity, i.e., there's no more room to add new elements without
    //      allocating more memory.
    //
    //==EDOC====================================================================
    bool IsFull() const;


    // Index operations.
    //-------------------

    //==SDOC====================================================================
    //==
    //==  GetFirstIndex
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the index of the first element of the map.
    //
    //  Returns:
    //      The index of the first element of the map.
    //
    //  Description:
    //      Returns the index of the first element of the map.
    //
    //==EDOC====================================================================
    unsigned int GetFirstIndex() const;

    //==SDOC====================================================================
    //==
    //==  GetLastIndex
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the index of the last element of the map.
    //
    //  Returns:
    //      The index of the last element of the map.
    //
    //  Description:
    //      Returns the index of the last element of the map.
    //
    //==EDOC====================================================================
    unsigned int GetLastIndex() const;

    //==SDOC====================================================================
    //==
    //==  GetEndIndex
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the index of the first unused element.
    //
    //  Returns:
    //      The index of the first unused element.
    //
    //  Description:
    //      Returns the index of the first unused element (i.e., the first
    //      invalid index).
    //
    //==EDOC====================================================================
    unsigned int GetEndIndex() const;

    //==SDOC====================================================================
    //==
    //==  GetMinElementIndex
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the index of the smallest key.
    //
    //  Returns:
    //      The index of the "smallest" key found in the container.  An empty
    //      container always returns 0.
    //
    //  Description:
    //      Returns the index of the smallest key of the map according to the
    //      comparison function.
    //
    //==EDOC====================================================================
    unsigned int GetMinElementIndex() const;

    //==SDOC====================================================================
    //==
    //==  GetMaxElementIndex
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the index of the biggest key.
    //
    //  Returns:
    //      The index of the "biggest" key found in the container.  An empty
    //      container always returns 0.
    //
    //  Description:
    //      Returns the index of the biggest key of the map according to the
    //      comparison function.
    //
    //==EDOC====================================================================
    unsigned int GetMaxElementIndex() const;


    // Direct element access.
    //------------------------

    //==SDOC====================================================================
    //==
    //==  GetAt
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a value and its key from an index.
    //
    //  Parameters:
    //    uIndex:
    //      The desired index.
    //
    //  Returns:
    //      A reference on a pair containing the key and the value at uIndex.
    //
    //  Description:
    //      Returns the key and the value at uIndex.  A crash is likely to occur
    //      if uIndex is greater than the last index.
    //
    //==EDOC====================================================================
    CPair<const _KeyType, _ValueType>& GetAt(IN unsigned int uIndex);

    // <COMBINE GetAt>
    const CPair<const _KeyType, _ValueType>& GetAt(IN unsigned int uIndex) const;

    //==SDOC====================================================================
    //==
    //==  operator[]
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a value and its key from an index.
    //
    //  Parameters:
    //    uIndex:
    //      The index of the wanted element.
    //
    //  Returns:
    //      A reference on a pair containing the key and the value at uIndex.
    //
    //  Description:
    //      Returns the key and the value at uIndex.  A crash is likely to occur
    //      if uIndex is greater than the last index.
    //
    //==EDOC====================================================================
    CPair<const _KeyType, _ValueType>& operator[](IN unsigned int uIndex);

    // <COMBINE []>
    const CPair<const _KeyType, _ValueType>& operator[](IN unsigned int uIndex) const;


    // Insert/Erase
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  Insert
    //==
    //==========================================================================
    //
    //  Summary:
    //      Inserts a key and a default value in the map.
    //
    //  Parameters:
    //    rKey:
    //      The key to insert.
    //
    //  Returns:
    //      resS_OK
    //      resFE_DUPLICATE
    //      resFE_INVALID_STATE
    //      resFE_OUT_OF_MEMORY
    //      resFE_FAIL
    //
    //  Description:
    //      Inserts a key in the map and leaves its associated value to its
    //      default state (i.e., the default constructor is called for it).  If
    //      the key to insert is already present in the map, then this method
    //      returns an error code to indicate the problem.
    //
    //      The capacity is increased if there are not enough allocated elements
    //      to accommodate the insertion of the new element. The lock capacity
    //      count is still enforced and if it is not zero, the insertion fails.
    //
    //==EDOC====================================================================
    mxt_result Insert(IN const _KeyType& rKey);

    //==SDOC====================================================================
    //==
    //==  Insert
    //==
    //==========================================================================
    //
    //  Summary:
    //      Inserts a key and its value in the map.
    //
    //  Parameters:
    //    rKey:
    //      The key to insert.
    //
    //    rValue:
    //      The value associated with the key to insert.
    //
    //  Returns:
    //      resS_OK
    //      resFE_DUPLICATE
    //      resFE_INVALID_STATE
    //      resFE_OUT_OF_MEMORY
    //      resFE_FAIL
    //
    //  Description:
    //      Inserts the given key and value in the map.  If the key to insert
    //      is a duplicate, then this method returns an error code to indicate
    //      the problem.
    //
    //      The capacity is increased if there are not enough allocated elements
    //      to accommodate the insertion of the new element. The lock capacity
    //      count is still enforced and if it is not zero, the insertion fails.
    //
    //==EDOC====================================================================
    mxt_result Insert(IN const _KeyType& rKey, IN const _ValueType& rValue);

    //==SDOC====================================================================
    //==
    //==  EraseElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes a key and its associated value from the map.
    //
    //  Parameters:
    //    rKey:
    //      The key to remove from the map.
    //
    //  Description:
    //      Erases one element from the map (the key and its value).  If the key
    //      to remove is not found, then this function does nothing at all.
    //
    //==EDOC====================================================================
    void EraseElement(IN const _KeyType& rKey);

    //==SDOC====================================================================
    //==
    //==  Erase
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes a key and its associated value from the map.
    //
    //  Parameters:
    //    uIndex
    //      The index of the key to remove from the map.
    //
    //  Description:
    //      Erases one entry from the map (the key and its value).  This method
    //      is likely to crash if uIndex is bigger the last index of the map.
    //
    //==EDOC====================================================================
    void Erase(IN unsigned int uIndex);

    //==SDOC====================================================================
    //==
    //==  EraseAll
    //==
    //==========================================================================
    //
    //  Summary:
    //      Erases all keys and their values from the map.
    //
    //  Description:
    //      Erases all keys from the map.  It is more efficient to use EraseAll
    //      than Erase for each key or index.
    //
    //==EDOC====================================================================
    void EraseAll();


    // Advanced operations.
    //----------------------

    //==SDOC====================================================================
    //==
    //==  Allocate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allocates one value but does not construct it.
    //
    //  Parameters:
    //    rKey:
    //      The key where the value is allocated.
    //
    //  Returns:
    //      A pointer on an allocated but uninitialized value. NULL is returned
    //      when the allocation fails or when the key is a duplicate.
    //
    //  Description:
    //      Acts like an Insert but does not initialize the value (no
    //      constructor is called).  The user is then free to do whatever it
    //      wants with the returned pointer.
    //
    //      The capacity is increased if there are not enough allocated elements
    //      to accommodate the insertion of the new element. The lock capacity
    //      count is still enforced and if it is not zero, the Allocate fails
    //      and returns NULL.
    //
    //==EDOC====================================================================
    void* Allocate(IN const _KeyType& rKey);

    //==SDOC====================================================================
    //==
    //==  FindPtr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Finds a value in the map from its key.
    //
    //  Parameters:
    //    rKey:
    //      The key of the value to find.
    //
    //    bInsertIfNotFound:
    //      Tells to insert an empty object if key is not found (Optional on
    //      non-const CMap)
    //
    //  Returns:
    //      A pointer on the value found, NULL otherwise.
    //
    //  Description:
    //      This function searches the map for the given key and returns
    //      a pointer to its associated value if it is found.
    //
    //      For a non-const CMap, when the key is not found, it is possible to
    //      automatically perform an insertion according to bInsertIfNotFound
    //      parameter, i.e., when bInsertIsNotFound is false, NULL is returned,
    //      otherwise, an association (key, empty object) is inserted in the
    //      map and the address of the empty object is returned.
    //
    //==EDOC====================================================================
    const _ValueType* FindPtr(IN const _KeyType& rKey) const;

    // <COMBINE FindPtr>
    _ValueType* FindPtr(IN const _KeyType& rKey, IN bool bInsertIfNotFound = false);

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    static int Compare(IN const CMapPair<_KeyType, _ValueType>& rOneKey,
                       IN const CMapPair<_KeyType, _ValueType>& rOtherKey,
                       IN mxt_opaque opq);

private:
    const CMapPair<_KeyType, _ValueType>* GetKeyPair(IN const _KeyType& rKey) const;
    mxt_result InsertKey(IN const _KeyType& rKey, OUT _ValueType** ppValue);

// Hidden Members
//----------------
protected:

    struct SCompareParams
    {
        int (*m_pfnCompare)(IN const _KeyType& rOneKey,
                            IN const _KeyType& rOtherKey,
                            IN  mxt_opaque opq);
        mxt_opaque m_opq;
    };

    SCompareParams m_stCompareParams;

private:
    _ContainerType m_Container;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  PUBLIC FUNCTIONS (INLINED OR NOT)  =====================================
//==============================================================================

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
CMap<_KeyType, _ValueType, _ContainerType>::CMap(IN IAllocator* pAllocator)
: m_Container(pAllocator)
{
    m_Container.SetComparisonFunction(CMap<_KeyType, _ValueType, _ContainerType>::Compare,
                                      MX_VOIDPTR_TO_OPQ(&m_stCompareParams));
    m_stCompareParams.m_pfnCompare = NULL;
    m_stCompareParams.m_opq = MX_INT32_TO_OPQ(0);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
CMap<_KeyType, _ValueType, _ContainerType>::CMap(IN const CMap<_KeyType, _ValueType, _ContainerType>& rMap)
{
    *this = rMap;
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
CMap<_KeyType, _ValueType, _ContainerType>::~CMap()
{
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
CMap<_KeyType, _ValueType, _ContainerType>&
CMap<_KeyType, _ValueType, _ContainerType>::operator=(IN const CMap<_KeyType, _ValueType, _ContainerType>& rMap)
{
    m_stCompareParams.m_pfnCompare = rMap.m_stCompareParams.m_pfnCompare;
    m_stCompareParams.m_opq = rMap.m_stCompareParams.m_opq;

    m_Container = rMap.m_Container;

    m_Container.SetComparisonFunction(CMap<_KeyType, _ValueType, _ContainerType>::Compare,
                                      reinterpret_cast<mxt_opaque>(&m_stCompareParams));

    return *this;
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
mxt_result
CMap<_KeyType, _ValueType, _ContainerType>::SetComparisonFunction(IN int (pfnCompare)(IN const _KeyType& rOneKey,
                                                                                      IN const _KeyType& rOtherKey,
                                                                                      IN mxt_opaque opq),
                                                                  IN  mxt_opaque opq)
{
    mxt_result res = resS_OK;

    if (!IsEmpty())
    {
        res = resFE_INVALID_STATE;
    }
    else
    {
        m_stCompareParams.m_pfnCompare = pfnCompare;
        m_stCompareParams.m_opq = opq;
    }

    return(res);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
unsigned int CMap<_KeyType, _ValueType, _ContainerType>::GetCapacity() const
{
    return m_Container.GetCapacity();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
unsigned int CMap<_KeyType, _ValueType, _ContainerType>::GetLockCapacity() const
{
    return m_Container.GetLockCapacity();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
void CMap<_KeyType, _ValueType, _ContainerType>::LockCapacity()
{
    m_Container.LockCapacity();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
mxt_result CMap<_KeyType, _ValueType, _ContainerType>::ReduceCapacity(IN unsigned int uDownToCapacity)
{
    return m_Container.ReduceCapacity(uDownToCapacity);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
mxt_result CMap<_KeyType, _ValueType, _ContainerType>::ReserveCapacity(IN unsigned int uUpToCapacity)
{
    return m_Container.ReserveCapacity(uUpToCapacity);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
void CMap<_KeyType, _ValueType, _ContainerType>::UnlockCapacity()
{
    m_Container.UnlockCapacity();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
unsigned int CMap<_KeyType, _ValueType, _ContainerType>::GetSize() const
{
    return m_Container.GetSize();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
bool CMap<_KeyType, _ValueType, _ContainerType>::IsEmpty() const
{
    return m_Container.IsEmpty();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
bool CMap<_KeyType, _ValueType, _ContainerType>::IsFull() const
{
    return m_Container.IsFull();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
unsigned int CMap<_KeyType, _ValueType, _ContainerType>::GetFirstIndex() const
{
    return m_Container.GetFirstIndex();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
unsigned int CMap<_KeyType, _ValueType, _ContainerType>::GetLastIndex() const
{
    return m_Container.GetLastIndex();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
unsigned int CMap<_KeyType, _ValueType, _ContainerType>::GetEndIndex() const
{
    return m_Container.GetEndIndex();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
unsigned int CMap<_KeyType, _ValueType, _ContainerType>::GetMinElementIndex() const
{
    return m_Container.GetMinElementIndex();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
unsigned int CMap<_KeyType, _ValueType, _ContainerType>::GetMaxElementIndex() const
{
    return m_Container.GetMaxElementIndex();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
CPair<const _KeyType, _ValueType>&
CMap<_KeyType, _ValueType, _ContainerType>::GetAt(IN unsigned int uIndex)
{
    return (CPair<const _KeyType, _ValueType>&)(m_Container.GetAt(uIndex));
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
const CPair<const _KeyType, _ValueType>&
CMap<_KeyType, _ValueType, _ContainerType>::GetAt(IN unsigned int uIndex) const
{
    return (CPair<const _KeyType, _ValueType>&)(m_Container.GetAt(uIndex));
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
CPair<const _KeyType, _ValueType>&
CMap<_KeyType, _ValueType, _ContainerType>::operator[](IN unsigned int uIndex)
{
    return (CPair<const _KeyType, _ValueType>&)(m_Container[uIndex]);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
const CPair<const _KeyType, _ValueType>&
CMap<_KeyType, _ValueType, _ContainerType>::operator[](IN unsigned int uIndex) const
{
    return (CPair<const _KeyType, _ValueType>&)(m_Container[uIndex]);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
mxt_result CMap<_KeyType, _ValueType, _ContainerType>::Insert(IN const _KeyType& rKey)
{
    _ValueType* pValue;
    mxt_result res = InsertKey(rKey, &pValue);
    if (MX_RIS_S(res))
    {
        new (pValue) _ValueType();
    }

    return(res);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
mxt_result CMap<_KeyType, _ValueType, _ContainerType>::Insert(IN const _KeyType& rKey,
                                                              IN const _ValueType& rValue)
{
    _ValueType* pValue;
    mxt_result res = InsertKey(rKey, &pValue);
    if (MX_RIS_S(res))
    {
        new (pValue) _ValueType(rValue);
    }

    return(res);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
void CMap<_KeyType, _ValueType, _ContainerType>::EraseElement(IN const _KeyType& rKey)
{
    m_Container.EraseSorted(*GetKeyPair(rKey));
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
void CMap<_KeyType, _ValueType, _ContainerType>::Erase(IN unsigned int uIndex)
{
    m_Container.Erase(uIndex);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
void CMap<_KeyType, _ValueType, _ContainerType>::EraseAll()
{
    m_Container.EraseAll();
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
void* CMap<_KeyType, _ValueType, _ContainerType>::Allocate(IN const _KeyType& rKey)
{
    void* pvoid = NULL;
    _ValueType* pValue;
    if (MX_RIS_S(InsertKey(rKey, &pValue)))
    {
        pvoid = pValue;
    }

    return(pvoid);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
const _ValueType* CMap<_KeyType, _ValueType, _ContainerType>::FindPtr(IN const _KeyType& rKey) const
{
    const _ValueType* pValueFound = NULL;

    const CMapPair<_KeyType, _ValueType>* pPairFound = m_Container.FindPtrSorted(*GetKeyPair(rKey));
    if (pPairFound != NULL)
    {
        pValueFound = &(pPairFound->GetSecond());
    }

    return pValueFound;
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
inline
_ValueType* CMap<_KeyType, _ValueType, _ContainerType>::FindPtr(IN const _KeyType& rKey,
                                                                IN bool bInsertIfNotFound)
{
    // We need to use a constant pointer on this in order to call the right
    // FindPtr function.
    const CMap<_KeyType, _ValueType, _ContainerType>* constThis = this;
    _ValueType* pValue = const_cast<_ValueType*>(constThis->FindPtr(rKey));

    if (bInsertIfNotFound == true && pValue == NULL )
    {
        mxt_result res = InsertKey(rKey, &pValue);
        if (MX_RIS_S(res))
        {
            new (pValue) _ValueType();
        }
    }

    return pValue;
}

//M5T_INTERNAL_USE_BEGIN
template<class _KeyType,
         class _ValueType,
         class _ContainerType>
int CMap<_KeyType, _ValueType, _ContainerType>::Compare(IN const CMapPair<_KeyType, _ValueType>& rOneKey,
                                                        IN const CMapPair<_KeyType, _ValueType>& rOtherKey,
                                                        IN mxt_opaque opq)
{
    int nResult = 0;
    const _KeyType& rKey1 = rOneKey.GetFirst();
    const _KeyType& rKey2 = rOtherKey.GetFirst();

    if ((reinterpret_cast<SCompareParams*>(opq))->m_pfnCompare != NULL)
    {
        nResult = reinterpret_cast<SCompareParams*>(opq)->m_pfnCompare(rKey1,
                                                                       rKey2,
                                                                       reinterpret_cast<SCompareParams*>(opq)->m_opq);
    }
    else if(rKey1 < rKey2)
    {
        nResult = -1;
    }
    else if(rKey1 != rKey2)
    {
        nResult = 1;
    }

    return nResult;
}

//==============================================================================
//====  HELPER AND PRIVATE METHODS  ============================================
//==============================================================================

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
const CMapPair<_KeyType, _ValueType>*
CMap<_KeyType, _ValueType, _ContainerType>::GetKeyPair(IN const _KeyType& rKey) const
{
    // This function returns a pointer on a pair with only the key initialized.
    // The trick used here to get that pair without constructing any object is
    // build it around rKey.  This is done by casting into a CMapPair the result
    // of the substraction of the address of rKey with the offset of the first
    // member of a CMapPair<_KeyType, _ValueType>.
    // The pair created is of course not complete : its constructor has not
    // been called and its second element has not been constructed.  It is
    // however enough for the containers methods that only read the first
    // member of this pair (like AllocateSorted()).
    CAlignedVariableStorage<CMapPair<_KeyType, _ValueType> > dummyPair;
    CMapPair<_KeyType, _ValueType>* pDummyPair = dummyPair.Instance();
    ptrdiff_t pKeyOffset = reinterpret_cast<uint8_t*>(&pDummyPair->GetFirst()) -
                           reinterpret_cast<uint8_t*>(pDummyPair);
    return reinterpret_cast<const CMapPair<_KeyType, _ValueType>*>(reinterpret_cast<const uint8_t*>(&rKey) - pKeyOffset);
}

template<class _KeyType,
         class _ValueType,
         class _ContainerType>
mxt_result CMap<_KeyType, _ValueType, _ContainerType>::InsertKey(IN const _KeyType& rKey,
                                                                 OUT _ValueType** ppValue)
{
    void *pvoid;
    mxt_result res = m_Container.AllocateSorted(*GetKeyPair(rKey), &pvoid);
    if (MX_RIS_S(res))
    {
        if (pvoid != NULL)
        {
            CMapPair<_KeyType, _ValueType>* pNewPair = new (pvoid) CMapPair<_KeyType, _ValueType>(rKey);
            *ppValue = &(pNewPair->GetSecond());
        }
        else
        {
            res = resFE_FAIL;
        }
    }

    return res;
}
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // MXG_CMAP_H
