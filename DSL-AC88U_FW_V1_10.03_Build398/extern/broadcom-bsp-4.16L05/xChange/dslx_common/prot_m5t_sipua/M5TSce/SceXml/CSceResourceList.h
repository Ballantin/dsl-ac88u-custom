//==SDOC========================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CRESOURCELIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CRESOURCELIST_H
//M5T_INTERNAL_USE_END


//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CFILTERDOCUMENT_H
#include "SceXml/CSceFilterDocument.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CXmlElement;
class IXmlDocument;
class CSubAllocator;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSceResourceList
//==============================================================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Description:
//  The CSceResourceList class is used to represent a single resource list, as
//  defined by the \<list\> element of RFC 4826. A resource list is able to hold
//  a set of resources. A resource is identified by an URI and can also have an
//  optional display name. In many cases, a resource will identify a "sip" or a
//  "tel" URI, but it could also identify other URI types.
//
//  To further allow a user to customize and arrange how the resources of a list
//  are sorted, a resource list can also hold references to resource,
//  references to other resource list and it can also hold other resource lists.
//
//  The CSceResourceList is a simple wrapper over the XML representation of a
//  \<list\> that can be created from a CXmlElement returned by an XML document
//  class or returned by an other list element. It makes it easy to access the
//  standard elements found in a resource list.
//
//  The CSceResourceList is a light weigth class, it can be created on the stack (as
//  a local variable) without any problem. It however shares a pointer to a
//  CXmlElement that is owned by an other object. Deleting the associated
//  object, calling Cleanup on it or deleting parent XML elements of this
//  resource list element will render this object invalid.
//
//  The following sections details the different item types that a resource
//  list can hold.
//
//#Resource Item#
//  The resource is used to identify a single resource using the URI of the
//  resource. The resource can also optionnaly associate a display name with the
//  URI.
//
//  In RFC 4826, the resource item corresponds to the \<entry\> element.
//
//#Resource Reference Item#
//  The resource reference can be used to reference a resource item found
//  in a document on the XCAP server that is holding the resource list. As such,
//  this reference makes sense only when the document is managed by an XCAP
//  server.
//
//  The resource reference specifies an URI which is the relative XCAP URI of
//  the resource on the XCAP server. The resource reference can also hold an
//  optional display name used to identify the referenced resource.
//
//  In RFC 4826, the resource reference item corresponds to the \<entry-ref\>
//  element.
//
//#Resource List Item#
//  The resource list can hold another resource list, which allows users to
//  structure its resources in a hierarchical manner.
//
//  In RFC 4826, the resource list item corresponds to the \<list\>
//  element.
//
//#Resource List Reference Item#
//  The resource list can also hold a reference to another resource list which
//  is part of an external document. This allows sharing a resource list among
//  multiple users for example.
//
//  A resource list reference has a URI that must be a full XCAP URI that points
//  to a \<list\> item within a document. As opposed to the resource reference
//  item, the resource list reference can reference a document that is not
//  located on the same XCAP server as the one that currently manages the
//  resource list.
//
//  In RFC 4826, the resource list reference item corresponds to the \<external\>
//  element.
//
// Location:
//   SceXml/CSceResourceList.h
//
// See Also:
//   CSceResourceListsDocument
//
//==EDOC========================================================================
class CSceResourceList
{
//-- Friend Declarations
    // MXI_DESIGN_NOTES (2009-02-23): CSceFilterElement should eventually be
    //                                initialized in CoreComponents once it
    //                                no longer requires UA-IMPP specific files.
//M5T_INTERNAL_USE_BEGIN
    friend class CSceUaImppInitializer;
//M5T_INTERNAL_USE_END

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSceResourceList(IN CXmlElement& rListElement);


    // Warning: Destructor is not virtual to keep this class as lightweight as
    // possible. Keep this in mind if inheriting from this class.
    //-------------------------------------------------------------------------

    // Summary:
    //  Destructor.
    //--------------
    ~CSceResourceList();


    // Summary:
    //  Sets the optional name of this list.
    mxt_result SetName(IN const char* pszListName);

    // Summary:
    //  Gets the optional name of the list.
    const char* GetName();

    // Summary:
    //  Sets the optional display name of the list.
    CXmlElement* SetDisplayName(IN const char* pszUtf8DisplayName,
                                IN const char* pszLang = NULL);


    // Summary:
    //  Gets the optional display name of the list.
    CXmlElement* GetDisplayName(OUT const char** ppszUtf8DisplayName,
                                OUT const char** ppszLang = NULL);

    // Summary:
    //  Gets a vector pointing the items found in the list.
    mxt_result GetListItems(OUT CVector<CXmlElement*>& rvpItems);

    // Description:
    //  This enumeration defines the type of items that a resource list can hold.
    enum EListItemType
    {
        // Description:
        //  The item is a resource, and the resource information can be
        //  accessed using GetItemResource.
        eTYPE_RESOURCE,

        // Description:
        //  The item is a reference to a resource that must be part of the same
        //  document. The information about the resource reference can be
        //  accessed using GetItemResourceRef.
        eTYPE_RESOURCE_REF,

        // Description:
        //  The item is a resource list. It can be accessed using
        //  GetItemResourceList.
        eTYPE_RESOURCELIST,

        // Description:
        //  The item is a reference to an external resource list. The
        //  information about the list reference can be accessed using
        //  GetItemResourceListRef.
        eTYPE_RESOURCELIST_REF,

        // Description:
        //  The item is not a recognized type of item.
        eTYPE_INVALID

    };

    // Summary:
    //  Gets the type of item pointed by pElement.
    EListItemType GetItemType(IN CXmlElement* pElement);


    // Summary:
    //  Set the optional display-name child for pItem element.
    CXmlElement* SetItemDisplayName(IN CXmlElement* pItem,
                                    IN const char*  pszUtf8DisplayName,
                                    IN const char*  pszLang = NULL);

    // Summary:
    //  Get the value of the display name element, that is, if it exists, the
    //  first child of pItem.
    CXmlElement* GetItemDisplayName(IN CXmlElement* pItem,
                                    OUT const char** ppszUtf8DisplayName,
                                    OUT const char** ppszLang = NULL);

    // Summary:
    //  Inserts a Resource element.
    CXmlElement* InsertItemResource(IN CXmlElement* pNextSibling,
                                    IN const char* pszUri);

    // Summary:
    //   Returns the URI of the specified resource.
    mxt_result GetItemResource(IN CXmlElement* pResourceItem,
                               OUT const char*& rpszUri);

    // Summary:
    //  Inserts a reference to a resource.
    CXmlElement* InsertItemResourceRef(IN CXmlElement* pNextSibling,
                                       IN const char* pszUri);

    // Summary:
    //   Returns the URI of the specified resource.
    mxt_result GetItemResourceRef(IN CXmlElement* pResourceItem,
                                  OUT const char*& rpszUri);

    // Summary:
    //  Inserts a resource list within this resource list.
    CXmlElement* InsertItemResourceList(IN CXmlElement* pNextSibling,
                                        IN const char* pszUri);

     // Summary:
    //  Inserts a reference to a resource list.
    CXmlElement* InsertItemResourceListRef(IN CXmlElement* pNextSibling,
                                           IN const char* pszUri);

    // Summary:
    //   Returns the URI of the specified resource.
    mxt_result GetItemResourceListRef(IN CXmlElement* pResourceItem,
                                      OUT const char*& rpszUri);

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:
    //-- << Initialization mechanism >>
    // Summary:
    //  Initializes the CSceFilterElement.
    static mxt_result InitializeCSceResourceList();

    // Summary:
    //  Finalizes the CSceFilterElement.
    static void FinalizeCSceResourceList();

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSceResourceList(IN const CSceResourceList& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSceResourceList& operator=(IN const CSceResourceList& rSrc);

    // Summary:
    //  Inserts a new resourse in the list at the specified position.
    CXmlElement* InsertResourceHelper(IN CXmlElement* pNextSibling,
                                      IN const char* pszResourceTypeName,
                                      IN const char* pszAttributeName,
                                      IN const char* pszAttributeValue);

    // Summary:
    //  Gets the URI of the specified resource from the resource list.
    mxt_result GetResourceUriHelper(IN CXmlElement* pResourceItem,
                                    IN const char* pszResourceName,
                                    IN const char* pszAttributeName,
                                    OUT const char*& rpszAttributeValue);

//-- Hidden Data Members
//------------------------
protected:
private:
    // Reference to a single resource list that is part of a resource
    // list document. A resource list document may include several
    // <list> elements.
    CXmlElement* m_pListElement;

    // ms_vecElements specifies, in the case of the display name element does not exist,
    // the element insertion order. The display-name element must be always the first
    // child. As a direct consequence, this vector is invariable and it is declared as
    // a static member.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecElements;

//M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CRESOURCELIST_H

