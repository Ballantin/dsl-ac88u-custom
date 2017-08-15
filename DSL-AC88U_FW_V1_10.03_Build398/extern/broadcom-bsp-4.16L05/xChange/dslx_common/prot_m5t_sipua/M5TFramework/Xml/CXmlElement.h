//==============================================================================
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
//==============================================================================
#ifndef MXG_CXMLELEMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CXMLELEMENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

// Even though this file does not directly need ECom support, it needs the
// IXmlDocument to be compiled and it can only be if both XML and ECom are
// activated.
#if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

#ifndef MXG_IXMLDOCUMENT_H
#include "Xml/IXmlDocument.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CBlob;
class IXmlGenericWriter;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//M5T_INTERNAL_USE_END

//==============================================================================
//== Class: CXmlElement
//========================================
//<GROUP XML_CLASSES>
//
// Description:
//  The CXmlElement represents an XML element. An XML element is best
//  illustrated by an example:
//
//  <CODE>
//      \<elem1\>
//         \<elem2\>
//             TEST
//         \</elem2\>
//      \</elem1\>
//  </CODE>
//
//  In the above example, both elem1 and elem2 are XML elements. XML elements
//  always have a name. They can be associated with a namespace or not. They
//  also can have attributes and values.
//
//  The XML element are structured as a tree to represent an XML document.
//
//  XML elements are always created either using an IXmlDocument or using
//  another XML element.
//
// Location:
//   Xml/CXmlElement.h
//
// See Also:
//   IXmlDocument
//
//==============================================================================
class CXmlElement
{
//-- Friend Declarations.

//-- New types.
public:

    // Summary:
    //  Structure used to hold a chained list of namespaces.
    struct SNamespace
    {
        // Description:
        //  This enumeration is used to specify the namespace characteristics.
        enum ENamespaceCharacteristics
        {
            // Description:
            //  When this bitmask is present, m_pszUri was allocated with the
            //  IXmlDocument and must be released with it.
            eURI_MUST_BE_DELETED = 0x01
        };

        // Description:
        //  A bitset keeping characteristics of this instance. This is a bitset
        //  of ENamespaceCharacteristics values.
        char m_cCharacteristics;

        // Description:
        //  The URI string.
        const char* m_pszUri;

        // Description:
        //  The prefix string.
        char* m_pszPrefix;

        // Description:
        //  Next namespace in the chained list.
        SNamespace* m_pstNextNamespace;

        // Description:
        //  Default constructor.
        SNamespace()
        :   m_cCharacteristics(0),
            m_pszUri(NULL),
            m_pszPrefix(NULL),
            m_pstNextNamespace(NULL)
        {}

        // Description:
        //  Constructor.
        SNamespace(IN char cCharacteristics,
                   IN const char* pszUri,
                   IN char* pszPrefix,
                   IN SNamespace* pstNextNamespace)
        :   m_cCharacteristics(cCharacteristics),
            m_pszUri(pszUri),
            m_pszPrefix(pszPrefix),
            m_pstNextNamespace(pstNextNamespace)
        {}
    };


    // Description:
    //  This enumeration is used to specify where a child element is inserted
    //  with CreateElement.
    enum ECreatePosition
    {
        // Description:
        //  Inserts the element as the first sibling element.
        ePOS_FIRST_SIBLING,

        // Description:
        //  Inserts the element as the previous element (i.e. previous sibling)
        //  of the current element.
        ePOS_PREVIOUS_SIBLING,

        // Description:
        //  Inserts the element as the next element (i.e. next sibling) of the
        //  current element.
        ePOS_NEXT_SIBLING,

        // Description:
        //  Inserts the element as the last sibling element.
        ePOS_LAST_SIBLING,

        // Description:
        //  Inserts the element as the first child element of this element.
        ePOS_FIRST_CHILD,

        // Description:
        //  Inserts the element as the last child element of this element.
        ePOS_LAST_CHILD
    };


    // Description:
    //  This enumeration defines how and when namespaces are declared.
    // 
    //  In the following section, the *used element* is the element upon which
    //  is performed an operation that has the effect of potentially creating a
    //  namespace. This is the element upon which a method like
    //  <C>DeclareNamespace()</C>, <C>CreateElement</C> or any other method that
    //  takes an ENsDeclarationBehavior is called. This is not necessarily the
    //  same element where the namespace will be declared, as this depends on
    //  the ENsDeclarationPosition parameter. This later element is called the
    //  *specified element*.
    enum ENsDeclarationBehavior
    {
        // Description:
        //  Declares no namespace and fails if the namespace was not previously
        //  declared, or if the namespace is not accessible from the *used
        //  element*.
        eNSDECLARE_NONE,
        
        // Description:
        //  Forces the declaration of the namespace with the specified prefix.
        //  It can only be used when the namespace is created with
        //  eNSPOS_THIS_ELEMENT or eNSPOS_NEW_ELEMENT. This method fails if
        //  declaring the namespace on eNSPOS_THIS_ELEMENT and the prefix is
        //  already declared on this element.
        //
        //  Users must be careful when using this value to force the declaration
        //  of a namespace, as it is possible that the declared prefix "hides" a
        //  previously declared namespace on which a child element depends.
        eNSDECLARE_FORCE,

        // Description:
        //  Declares the namespace only if it is not accessible from the
        //  *used element*.
        //    - If the namespace is declared but not accessible from the *used
        //      element*, the namespace is re-declared in the *specified
        //      element*. If needed, the prefix is updated to ensure the
        //      namespace is in scope for the *used element* and to ensure
        //      that the prefix does not override any existing prefix.
        //    - If the namespace is not declared, it is declared in the
        //      *specified element* and, if needed, the prefix is
        //      updated to ensure that it does not override an existing prefix.
        eNSDECLARE_OPTIMIZE,

        // Description:
        //  Declares the namespace only if it is not accessible from the *used
        //  element*. Furthermore, if the namespace has to be declared, it will
        //  use the specified prefix.
        //
        //  If the specified prefix is not accessible from the *used element*
        //  because there is a namespace declaration between the *specified
        //  element* and the *used element* that overrides the prefix, the
        //  namespace declaration fails.
        eNSDECLARE_OPTIMIZE_FAIL_ON_CONFLICT
    };


    // Description:
    //  This enumeration describes where a namespace must be created.
    //
    //  In the following section, the *used element* is the element upon which
    //  is performed an operation that has the effect of potentially creating a
    //  namespace. This is the element upon which a method like
    //  <C>DeclareNamespace()</C>, <C>CreateElement</C> or any other method that
    //  takes an ENsDeclarationBehavior is called. This is not necessarily the
    //  same element where the namespace will be declared, as this depends on
    //  the ENsDeclarationPosition parameter. This later element is called the
    //  *specified element*.
    enum ENsDeclarationPosition
    {
        // Description:
        //  The *specified element* where the namespace is created is the new
        //  element being created.
        //
        //  This value can only be used when creating an element and cannot be
        //  used when calling DeclareNamespace or Copy.
        eNSPOS_NEW_ELEMENT,

        // Description:
        //  The *specified element* is the same element as the *used element*.
        eNSPOS_THIS_ELEMENT,

        // Description:
        //  The *specified element* is the parent element of the *used element*.
        eNSPOS_PARENT_ELEMENT,

        // Description:
        //  The *specified element* is the root element.
        eNSPOS_ROOT_ELEMENT,
        
        // Description:
        //  This value can only be used when copying an element into another, or
        //  when creating a new element from another source element.
        //
        //  The *specified element* is updated dynamically when performing the
        //  copy to ensure that namespaces are copied from the source elements
        //  to the target elements as they are declared in the source.
        //
        //  Note that if an element or attribute from the source uses a
        //  namespace that is declared outside of the scope of the copy, then
        //  its namespace is declared in the first copied element.
        eNSPOS_AS_COPY
    };


    // Description:
    //  This structure permits identification of an XML component.
    struct SIdentificationInfo
    {
        // Description:
        //  The namespace where the name is defined.
        const char* m_pszNamespace;

        // Description.
        //  The name for which to search.
        const char* m_pszName;

        // Description:
        //  Default constructor.
        SIdentificationInfo()
        :   m_pszNamespace(NULL),
            m_pszName(NULL)
        {}
    };


private:
//M5T_INTERNAL_USE_BEGIN
    // Summary:
    //  Structure used to hold a chained list of attributes.
    struct SAttribute
    {
        // Description:
        //  The associated namespace.
        const SNamespace* m_pstNamespace;

        enum EAttributeCharacteristics
        {
            // Description:
            //  When this bitmask is present, m_pszName was allocated with the
            //  IXmlDocument and must be released with it.
            eNAME_MUST_BE_DELETED = 0x01
        };

        // Description:
        //  A bitset keeping characteristics of this instance. This is a bitset
        //  of EAttributeCharacteristics values.
        char m_cCharacteristics;

        // Description:
        //  The name of the attribute.
        const char* m_pszName;

        // Description:
        //  The value of the attribute.
        char* m_pszValue;

        // Description:
        //  The next attribute in the chain.
        SAttribute* m_pstNext;

        // Description:
        //  Default constructor.
        SAttribute()
        :   m_pstNamespace(NULL),
            m_cCharacteristics(0),
            m_pszName(NULL),
            m_pszValue(NULL),
            m_pstNext(NULL)
        {}
    };
//M5T_INTERNAL_USE_END

//-- Published Interface.
public:

    // Summary:
    //  Gets the namespace for this element from the namespace URI.
    const SNamespace* GetNamespaceByUri(IN const char* const pszNamespaceUri) const ;

    // Summary:
    //  Gets the namespace for this element from the namespace prefix.
    const SNamespace* GetNamespaceByPrefix(IN const char* const pszNamespacePrefix) const;

    // Summary:
    //  Declares a namespace in this element.
    mxt_result DeclareNamespace(IN const char* pszNamespaceUri,
                                IN const char* pszNamespacePrefix,
                                IN ENsDeclarationBehavior eBehavior = eNSDECLARE_OPTIMIZE,
                                IN ENsDeclarationPosition ePosition = eNSPOS_ROOT_ELEMENT);

    // Summary:
    //  Sets the namespace for this element.
    mxt_result SetNamespace(IN const char* pszNamespace);

    // Summary:
    //  Gets the namespace for this element.
    const char* GetNamespace() const;

    // Summary:
    //  Gets the prefix of the namespace for this element.
    const char* GetNamespacePrefix() const;

    // Summary:
    //  Gets the namespaces declared at this level.
    SNamespace* GetDeclaredNamespaces();

    // Summary:
    //  Updates the URI of a declared namespace.
    mxt_result UpdateDeclaredNamespace(IN const char* pszPrefix,
                                       IN const char* pszNewUri);

    // Summary:
    //  Deletes a declared namespace.
    mxt_result DeleteDeclaredNamespace(IN const char* pszPrefix);

    // Summary:
    //  Gets the name of this element.
    const char* GetName() const;

    // Summary:
    //  Sets the name of this element.
    void SetName(IN const char* pszName);

    // Summary:
    //  Gets the string value associated with this element.
    const char* GetValue() const;

    // Summary:
    //  Sets the string value associated with this element.
    void SetValue(IN const char* pszValue);

    // Summary:
    //  Deletes this element.
    mxt_result Delete();

    // Summary:
    //  Gets the number of attributes held by this element.
    unsigned int GetNumAttributes() const;

    // Summary:
    //  Gets an attribute that has the specified name.
    mxt_result GetAttribute(IN const char* pszAttrNamespace,
                            IN const char* pszAttrName,
                            OUT const char*& rpszAttrValue) const;

    // Summary:
    //  Gets an attribute at a specific index.
    mxt_result GetAttribute(IN unsigned int uIndex,
                            OUT const char*& rpszAttrNamespace,
                            OUT const char*& rpszAttrName,
                            OUT const char*& rpszAttrValue) const;

    // Summary:
    //  Adds an attribute at the end of the current attribute list.
    mxt_result AppendAttribute(IN const char* pszAttrNamespace,
                               IN const char* pszAttrName,
                               IN const char* pszAttrValue);

    // Summary:
    //  Changes the namespace, name and value associated with an existing
    //  attribute.
    mxt_result UpdateAttribute(IN unsigned int uIndex,
                               IN const char* pszAttrValue);

    // Summary:
    //  Changes the namespace, name and value associated with an existing
    //  attribute.
    mxt_result UpdateAttribute(IN const char* pszAttrNamespace,
                               IN const char* pszAttrName,
                               IN const char* pszAttrNewValue);

    // Summary:
    //  Sets the value of the attribute or creates it if it does not exist.
    mxt_result SetAttribute(IN const char* pszAttrNamespace,
                            IN const char* pszAttrName,
                            IN const char* pszNewAttrValue);

    // Summary:
    //  Deletes an attribute at the specified index.
    mxt_result DeleteAttribute(IN unsigned int uIndex);

    // Summary:
    //  Deletes the attribute that has the specified name.
    mxt_result DeleteAttribute(IN const char* pszAttrNamespace,
                               IN const char* pszAttrName);

    // Summary:
    //  Copies an element into this element.
    mxt_result Copy(IN const CXmlElement& rFrom,
                    IN ENsDeclarationBehavior eBehavior = eNSDECLARE_OPTIMIZE,
                    IN ENsDeclarationPosition ePosition = eNSPOS_ROOT_ELEMENT);

    // Summary:
    //  Creates a child or sibling element and allows to declare the namespace
    //  assigned to this new element.
    CXmlElement* CreateElement(IN const char* pszNamespaceUri,
                               IN const char* pszNamespacePrefix,
                               IN const char* pszElementName,
                               IN const char* pszValue,
                               IN ECreatePosition ePos,
                               IN ENsDeclarationBehavior eBehavior = eNSDECLARE_OPTIMIZE,
                               IN ENsDeclarationPosition ePosition = eNSPOS_ROOT_ELEMENT);

    // Summary:
    //  Creates a child or sibling element that has its namespace already
    //  declared or that is assigned to no namespace.
    CXmlElement* CreateElement(IN const char* pszNamespace,
                               IN const char* pszElementName,
                               IN const char* pszValue,
                               IN ECreatePosition ePos);

    // Summary:
    //  Creates a child or sibling element that is a copy of another element..
    CXmlElement* CreateElement(IN const CXmlElement& rFrom,
                               IN ECreatePosition ePos,
                               IN ENsDeclarationBehavior eBehavior = eNSDECLARE_OPTIMIZE,
                               IN ENsDeclarationPosition ePosition = eNSPOS_ROOT_ELEMENT);

    // Summary:
    //  Creates a child element that is inserted following a specified order.
    CXmlElement* CreateChildElement(IN const char* pszNamespace,
                                    IN const char* pszNamespacePrefix,
                                    IN const char* pszElementName,
                                    IN const char* pszNewValue,
                                    IN const CVector<SIdentificationInfo>* pvecstElementOrder,
                                    IN ENsDeclarationBehavior eBehavior = eNSDECLARE_OPTIMIZE,
                                    IN ENsDeclarationPosition ePosition = eNSPOS_ROOT_ELEMENT);

    // Summary:
    //  Creates a child element that is a copy of rFrom.
    CXmlElement* CreateChildElement(IN const CXmlElement& rFrom,
                                    IN const CVector<SIdentificationInfo>* pvecstElementOrder,
                                    IN ENsDeclarationBehavior eBehavior = eNSDECLARE_OPTIMIZE,
                                    IN ENsDeclarationPosition ePosition = eNSPOS_ROOT_ELEMENT);

    // Summary:
    //  Creates or replaces a child element that is inserted following a
    //  specified order.
    CXmlElement* SetChildElement(IN const char* pszNamespace,
                                 IN const char* pszNamespacePrefix,
                                 IN const char* pszElementName,
                                 IN const char* pszNewValue,
                                 IN const CVector<SIdentificationInfo>* pvecstElementOrder,
                                 IN ENsDeclarationBehavior eBehavior = eNSDECLARE_OPTIMIZE,
                                 IN ENsDeclarationPosition ePosition = eNSPOS_ROOT_ELEMENT);

    // Summary:
    //  Gets the number of child elements held by this element.
    unsigned int GetNumChildElement() const;

    // Summary:
    //  Gets a child element at the specified index.
    CXmlElement* GetChildElement(IN unsigned int uIndex);

    // Summary:
    //  Gets a child element at the specified index.
    const CXmlElement* GetChildElement(IN unsigned int uIndex) const;

    // Summary:
    //  Finds a child element according to its name and index.
    CXmlElement* FindChildElement(IN const char* pszNamespace,
                                  IN const char* pszElementName,
                                  IN unsigned int uIndex);

    // Summary:
    //  Finds a child element according to its name and index.
    const CXmlElement* FindChildElement(IN const char* pszNamespace,
                                        IN const char* pszElementName,
                                        IN unsigned int uIndex) const;

    // Summary:
    //  Gets the next sibling element to this element.
    CXmlElement* GetNextSibling();

    // Summary:
    //  Gets the next sibling element to this element.
    const CXmlElement* GetNextSibling() const;

    // Summary:
    //  Gets the previous sibling element to this element.
    CXmlElement* GetPreviousSibling();

    // Summary:
    //  Gets the previous sibling element to this element.
    const CXmlElement* GetPreviousSibling() const;

    // Summary:
    //  Gets the first sibling element from this element.
    CXmlElement* GetFirstSibling();

    // Summary:
    //  Gets the first sibling element from this element.
    const CXmlElement* GetFirstSibling() const;

    // Summary:
    //  Gets the last sibling element from this element.
    CXmlElement* GetLastSibling();

    // Summary:
    //  Gets the last sibling element from this element.
    const CXmlElement* GetLastSibling() const;

    // Summary:
    //  Gets the parent element to this element.
    CXmlElement* GetParentElement();

    // Summary:
    //  Gets the parent element to this element.
    const CXmlElement* GetParentElement() const;

    // Summary:
    //  Serializes the current element and children into a blob.
    mxt_result Serialize(IN IXmlGenericWriter& rWriter) const;

    //  Summary:
    //   Sets the opaque for this CXmlElement.
    void SetOpaque(mxt_opaque opq);

    //  Summary:
    //   Retrieves the opaque identifier.
    const mxt_opaque GetOpaque();

    // Summary:
    //  Returns the IXmlDocument value.
    void GetXmlDocument(OUT IXmlDocument** ppDocument) const;

//M5T_INTERNAL_USE_BEGIN

    // Summary:
    //  Creates an element. Should only be called internally by the
    //  CXmlElement.
    static GO CXmlElement* AllocateElement(IN IXmlDocument* pDocument,
                                           IN const char* pszElementName);

    // Summary:
    //  Creates an element. Should only be called internally by the
    //  CXmlElement.
    static GO CXmlElement* AllocateElement(IN IXmlDocument* pDocument,
                                           IN const SNamespace* pstNamespace,
                                           IN const char* pszElementName,
                                           IN const char* pszElementValue);

    // Summary:
    //  Releases an element. Should only be called by the IXmlDocument when
    //  releasing the root element and internally by the CXmlElement.
    static void ReleaseElement(IN IXmlDocument* pDocument,
                               IN TOA CXmlElement* pElement);

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CXmlElement(IN IXmlDocument* pDocument);

    // Summary:
    //  Destructor.
    ~CXmlElement();


    //-- << Static Initializer >>

    static mxt_result InitializeCXmlElement();
    static void FinalizeCXmlElement();

    //-- Friendship declaration.
    friend mxt_result InitializeCXmlElement();
    friend void FinalizeCXmlElement();

private:

    //-- << Helper methods >>

    // Summary:
    //  Gets the namespace from the namespace URI.
    const SNamespace* GetNamespaceByUriHelper(IN const char* const pszNamespaceUri,
                                              INOUT CVector<const char*>& rveczOverridenPrefix) const;

    // Summary:
    //  Gets a namespace defined in this object by its prefix.
    SNamespace* GetInternalNamespaceByPrefix(IN const char* const pszNamespacePrefix);

    // Summary:
    //  Gets a namespace defined in this object by its prefix.
    const SNamespace* GetInternalNamespaceByPrefix(IN const char* const pszNamespacePrefix) const;

    // Summary:
    //  Declares a namespace.
    mxt_result PrivateDeclareNamespace(IN const char* pszNamespaceUri,
                                       IN const char* pszNamespacePrefix,
                                       IN ENsDeclarationBehavior eBehavior,
                                       IN ENsDeclarationPosition ePosition,
                                       IN bool bReportPatchEvent = false,
                                       IN bool bReportDocumentEvent = false);

    // Summary:
    //  Declares a namespace.
    mxt_result PrivateDeclareNamespace(IN const char* pszNamespaceUri,
                                       IN const char* pszNamespacePrefix,
                                       IN ENsDeclarationBehavior eBehavior,
                                       IN CXmlElement* pDeclPosition,
                                       IN bool bReportPatchEvent = false,
                                       IN bool bReportDocumentEvent = false);

    // Summary:
    //  Gets the element refered to by ePosition.
    CXmlElement* GetNamespaceDeclarationElement(IN ENsDeclarationPosition ePosition,
                                                OUT bool& bReportNewNamespaceEvent);

    // Summary:
    //  Sets the namespace for this element.
    mxt_result PrivateSetNamespace(IN const char* pszNamespace,
                                   IN bool bReportEvents = false);

    // Summary:
    //  Sets the name of this element.
    void PrivateSetName(IN const char* pszName, IN bool bReportEvents = false);

    // Summary:
    //  Sets the string value associated with this element.
    void PrivateSetValue(IN const char* pszValue, IN bool bReportEvents = false);

    // Summary:
    //  Internal method that deletes this element.
    mxt_result PrivateDelete(IN IXmlDocument* pXmlDocument, IN bool bReportEvent = false);

    // Summary:
    //  Adds an attribute at the end of the current attribute list.
    mxt_result PrivateAppendAttribute(IN const char* pszAttrNamespace,
                                      IN const char* pszAttrName,
                                      IN const char* pszAttrValue,
                                      IN bool bReportEvents = false);

    // Summary:
    //  Sets the value of the attribute or creates it if it does not exist.
    mxt_result PrivateSetAttribute(IN const char* pszAttrNamespace,
                                   IN const char* pszAttrName,
                                   IN const char* pszNewAttrValue,
                                   IN bool bReportEvent = false);

    // Summary:
    //  Creates a child or sibling element and allows to declare the namespace
    //  assigned to this new element.
    CXmlElement* PrivateCreateElement(IN const char* pszNamespaceUri,
                                      IN const char* pszNamespacePrefix,
                                      IN const char* pszElementName,
                                      IN const char* pszValue,
                                      IN ECreatePosition ePos,
                                      IN ENsDeclarationBehavior eBehavior = eNSDECLARE_OPTIMIZE,
                                      IN ENsDeclarationPosition ePosition = eNSPOS_ROOT_ELEMENT,
                                      IN bool bReportEvent = false);

    // Summary:
    //  Creates a child or sibling element and allows to declare the namespace
    //  assigned to this new element.
    CXmlElement* PrivateCreateElement(IN const char* pszNamespaceUri,
                                      IN const char* pszNamespacePrefix,
                                      IN const char* pszElementName,
                                      IN const char* pszValue,
                                      IN ECreatePosition ePos,
                                      IN ENsDeclarationBehavior eBehavior,
                                      IN CXmlElement* pElementForDeclaration,
                                      IN bool bReportEvent = false,
                                      IN bool bReportNewNamespaceEvent = false);

    // Summary:
    //  Searches the string in the dictionary.
    static const char* GetFromDictionary(IN const char* pszString,
                                         IN IXmlDocument* pXmlDocument);

    // Summary:
    //  Allocates a SNamespace and initializes it with the given content.
    GO SNamespace* AllocateNamespace(IN const char* pszNamespaceUri,
                                     IN const char* pszNamespacePrefix);

    // Summary:
    //  Releases allocated memory for a SNamespace.
    static void ReleaseNamespace(IN TOA SNamespace* pstNamespace,
                                 IN IXmlDocument* pXmlDocument);

    // Summary:
    //  Sets the SNamespace.
    void SetNamespace(IN const SNamespace* pstNamespace);

    // Summary:
    //  Allocates an attribute and the proper strings using the IAllocator.
    //  C-style strings are copied.
    GO SAttribute* AllocateAttribute(IN const SNamespace* pstAttrNamespace,
                                     IN const char* pszAttrName,
                                     IN const char* pszAttrValue);

    // Summary:
    //  Releases an attribute and its strings.
    static void ReleaseAttribute(IN TOA SAttribute* pAttribute,
                                 IN IXmlDocument* pXmlDocument);

    // Summary:
    //  Releases the internal ressources of the element.
    void ReleaseInternals(IN IXmlDocument* pXmlDocument);

    // Summary:
    //  Gets a pointer to the attribute that is at the designated index.
    SAttribute* InternalGetAttribute(IN unsigned int uIndex,
                                     OUT SAttribute** ppstPrevAttribute = NULL) const;

    // Summary:
    //  Gets a pointer to the attribute that matches the parameters.
    SAttribute* InternalGetAttribute(IN const char* pszAttrNamespace,
                                     IN const char* pszAttrName,
                                     OUT SAttribute** ppstPrevAttribute = NULL) const;

    // Summary:
    //  Updates the given attribute with the new value.
    mxt_result InternalUpdateAttribute(INOUT SAttribute& rstAttribute,
                                       IN const char* pszAttrNewValue,
                                       IN IXmlDocument* pXmlDocument,
                                       IN bool bReportEvents = false);

    // Summary:
    //  Finds the namespace/name index in a vector.
    unsigned int FindIndexInInfoVector(IN const char* pszNamespace,
                                       IN const char* pszName,
                                       IN const CVector<SIdentificationInfo>& rvecstOrder,
                                       OUT unsigned int& ruAnyIndex);

    // Summary:
    //  Creates a child or sibling element that has its namespace already
    //  declared or that is assigned to no namespace.
    CXmlElement* PrivateCreateElement(IN const char* pszNamespace,
                                      IN const char* pszElementName,
                                      IN const char* pszValue,
                                      IN ECreatePosition ePos,
                                      IN bool bReportEvents = false);

    // Summary:
    //  Adds or updates the selected element.
    CXmlElement* PrivateSetChildElement(IN const char* pszNamespace,
                                        IN const char* pszNamespacePrefix,
                                        IN const char* pszElementName,
                                        IN const char* pszNewValue,
                                        IN const CVector<SIdentificationInfo>* pvecstElementOrder,
                                        IN ENsDeclarationBehavior eBehavior,
                                        IN bool bAlwaysInsertElement,
                                        IN ENsDeclarationPosition ePosition,
                                        IN bool bReportEvents = false);

    // Summary:
    //  Deletes the given attribute.
    mxt_result InternalDeleteAttribute(IN TOA SAttribute* pstAttribute,
                                       IN SAttribute* pstPrevAttribute = NULL,
                                       IN bool bReportEvents = false);

    // Summary:
    //  Releases the content of the element (value or childs).
    void ReleaseContent(IN IXmlDocument* pXmlDocument);

    // Summary:
    //  Releases the current link with the IXmlDocument.
    void ReleaseLinkToDocument();

    // Summary:
    //  Sets the parent element of this element.
    void SetParentElement(IN CXmlElement* pParentElement);

    // Summary:
    //  Sets and links a next sibling to this element.
    void SetAndLinkNextSibling(IN TOA CXmlElement* pNextSibling);

    // Summary:
    //  Sets and links a first child to this element.
    void SetAndLinkFirstChild(IN TOA CXmlElement* pFirstChild,
                              IN IXmlDocument* pXmlDocument);

    // Summary:
    //  Sets a new value in a string with dictionary support.
    static void SetNewValueWithDictionarySupport(IN const char* pszNewValue,
                                                 IN IXmlDocument* pXmlDocument,
                                                 IN char cOwershipBitmask,
                                                 INOUT char& rcOwnershipBitset,
                                                 INOUT char*& rpszTargetString);

    // Summary:
    //  Reuses a C-style string to set a new value to it when possible.
    static mxt_result ReuseString(IN const char* pszNewStringValue,
                                  INOUT char* pszTargetString);

    // Summary:
    //  Compares two C-style strings by their pointer and then value.
    bool CompareDictionaryString(IN const char* pszFirstString,
                                 IN const char* pszSecondString) const;

    // Summary:
    //  Sets the specified bitmask in m_cElementCharacteristics.
    void SetInCharacteristics(IN char cBitmask);

    // Summary:
    //  Resets the specified bitmask in m_cElementCharacteristics.
    void ResetInCharacteristics(IN char cBitmask);

    // Summary:
    //  Verifies if a bitmask value is present in the m_cElementCharacteristics
    //  bitset.
    bool IsInCharacteristics(IN char cBitmask) const;

    // Summary:
    //  Copies every namespace from rFrom to this element.
    mxt_result CopyNamespace(IN const CXmlElement& rFrom,
                             IN ENsDeclarationBehavior eBehavior,
                             IN CXmlElement* pElementForDeclaration,
                             IN bool bReportEvent);

    // Summary:
    //  Copies every attributes from rFrom to this element.
    mxt_result CopyAttributes(IN const CXmlElement& rFrom);

    // Summary:
    //  Recursivly copies every child element from rFrom to this element.
    mxt_result CopyChildrenElements(IN const CXmlElement& rFrom,
                                    IN ENsDeclarationBehavior eBehavior,
                                    IN CXmlElement* pFirstElementCopy,
                                    IN CXmlElement* pElementForDeclaration,
                                    IN bool bReportNamespaceEvent);

    // Summary:
    //  Deletes all attributes from this element.
    void DeleteAllAttributes(IN IXmlDocument* pXmlDocument);

    // Summary:
    //  Releases all declared namespace on this element.
    void ReleaseAllNamespace(IN IXmlDocument* pXmlDocument);

    // Summary:
    //  Deletes all child elements of this element.
    void DeleteAllChildElements(IN IXmlDocument* pXmlDocument);

    // Summary:
    //  Link element with sibling elements.
    CXmlElement* LinkElement(IN TOA CXmlElement* pAllocatedElement,
                             IN CXmlElement* pParent,
                             IN ECreatePosition ePos);

    // Summary:
    //  Validates if a namespace prefix is in use by a child of this element.
    bool ChildUsesNamespace(IN const SNamespace* pstNamespace);

    // Summary:
    //  Finds an unused prefix for namespace declaration.
    mxt_result FindUnusedPrefixHelper(IN const char* pszOriginalPrefix,
                                      IN const char* pszNamespaceUri,
                                      IN const unsigned int uSizeOfNewPrefix,
                                      OUT char* pszNewPrefix);

    // Summary:
    //  Adds a namespace to the declared namespace of this element.
    mxt_result DeclareNamespaceHelper(IN const char* pszNamespaceUri,
                                      IN const char* pszNamespacePrefix,
                                      IN bool bReportPatchEvent,
                                      IN bool bReportDocumentEvent);

    // Summary:
    //  Copies an element into this element.
    mxt_result CopyHelper(IN const CXmlElement& rFrom,
                          IN ENsDeclarationBehavior eBehavior,
                          IN CXmlElement* pFirstElementCopy,
                          IN CXmlElement* pElementForDeclaration,
                          IN bool bReportNewNamespaceEvent);
    
    // Summary:
    //  Compares two strings using case sensitivity when set.
    int CompareHelper(IN const char* pszFirstString, IN const char* pszSecondString) const;


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CXmlElement(IN const CXmlElement& rSrc);

    // Summary:
    //  Assignment Operator.
    CXmlElement& operator=(IN const CXmlElement& rSrc);


//-- Hidden Data Members.
protected:
private:

    // The element name.
    const char* m_pszElementName;

    // Points to namespace structure declared elsewhere.
    const SNamespace* m_pstElementNamespace;

    // Points to namespaces declared at this element level. Linked list.
    SNamespace* m_pstDeclaredNamespaces;

    union UElementContent
    {
        // The element value.
        char* m_pszElementValue;

        // The list of child(ren) element(s).
        CXmlElement* m_pRootChildElement;
    };

    // The CXmlElement currently only support either an element with a value or
    // an element with a child element.
    UElementContent m_stuContent;

    union ULinkToDocument
    {
        // The XML document that owns this XML element.
        IXmlDocument* m_pDocument;

        // The parent element.
        CXmlElement* m_pParentElement;
    };

    // This member contains the link to the IXmlDocument.
    ULinkToDocument m_stuLinkToDocument;

    enum ECharacteristics
    {
        // Description:
        //  When this bitmask is present, the content of this object is a value
        //  (ie. m_pszElementValue). When the bitmask is not present, the
        //  content of this object is nested elements (ie. m_pRootChildElement).
        eCONTENT_IS_VALUE = 0x01,

        // Description:
        //  When this bitmask is present, the link to reach the IXmlDocument is
        //  our parent (ie. m_pParentElement). When eLINK_TO_DOC_IS_PARENT
        //  is not set, this element is the root element (the IXmlDocument can
        //  be directly accessed via m_pDocument).
        eLINK_TO_DOC_IS_PARENT = 0x02,

        // Description:
        //  When this bitmask is present, it means that m_pszElementName was
        //  allocated with the IXmlDocument. when the bitset is present, it
        //  must be released. When the bitset is not present m_pszElementName is
        //  pointing to a dictionary entry (it must not be deleted).
        eNAME_MUST_BE_DELETED = 0x04
    };

    // A bitset of ECharacteristics values.
    char m_cElementCharacteristics;

    // The element list of attributes.
    SAttribute* m_pstRootAttribute;

    // The next element sharing the same level.
    CXmlElement* m_pNextSiblingElement;

    // The CVector to use in PrivateSetChildElement to insert an unexisting
    // element as the last child.
    static CVector<SIdentificationInfo>* ms_pvecstAnyOrder;

    // The opaque identifier of this CXmlElement.
    mxt_opaque m_opq;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetNamespace
//==
//==============================================================================
//
//  Returns:
//      This element's namespace as a C-style string.
//
//      NULL if the namespace was not set yet.
//
//  Description:
//      This method returns the element's namespace.
//
//==============================================================================
inline const char* CXmlElement::GetNamespace() const
{
    return m_pstElementNamespace != NULL? m_pstElementNamespace->m_pszUri :
                                          NULL;
}

//==============================================================================
//==
//==  GetNamespacePrefix
//==
//==============================================================================
//
//  Returns:
//      The prefix of this element's namespace as a C-style string.
//
//      NULL if the namespace was not set yet or if it is in the default
//      namespace.
//
//  Description:
//      This method returns the prefix of this element's namespace.
//
//==============================================================================
inline const char* CXmlElement::GetNamespacePrefix() const
{
    return m_pstElementNamespace != NULL? m_pstElementNamespace->m_pszPrefix :
                                          NULL;
}

//==============================================================================
//==
//==  DeclareNamespace
//==
//==============================================================================
//
//  Parameters:
//      pszNamespaceUri:
//       The namespace URI to declare in this element. It MUST NOT be NULL.
//
//      pszNamespacePrefix:
//       The prefix of namespace to declare in this element. It can be NULL if
//       this is the default namespace.
//
//      ePosition:
//       enum specifying where to declare namespaces.
//
//      eBehavior:
//       The behavior when declaring namespaces.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT: pszNamespaceUri is NULL or the namespace
//        prefix was already defined in this element.
//      - resFE_FAIL: failed to allocate a new SNamespace structure.
//      - resS_OK: the namespace was successfully declared in this element.
//
//  Description:
//      This method declares a namespace in the element.
//
//      This namespace declaration applies to this element and its children.
//
//  See also:
//      ENsDeclarationBehavior, ENsDeclarationPosition
//
//==============================================================================
inline mxt_result CXmlElement::DeclareNamespace(IN const char* pszNamespaceUri,
                                                IN const char* pszNamespacePrefix,
                                                IN ENsDeclarationBehavior eBehavior,
                                                IN ENsDeclarationPosition ePosition)
{
    return PrivateDeclareNamespace(pszNamespaceUri,
                                   pszNamespacePrefix,
                                   eBehavior,
                                   ePosition,
                                   true,
                                   true);
}

//==============================================================================
//==
//==  SetNamespace
//==
//==============================================================================
//
//  Parameters:
//      pszNamespace:
//          The namespace to set.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT: the namespace is not defined in this element
//        or in one of its parents.
//      - resS_OK: the namespace was set.
//
//  Description:
//      This method sets the namespace of the element. The namespace MUST be
//      first defined in the document before being set on the element, otherwise
//      it won't be set and the namespace's element will be empty.
//
//==============================================================================
inline mxt_result CXmlElement::SetNamespace(IN const char* pszNamespace)
{
    return PrivateSetNamespace(pszNamespace, true);
}

//==============================================================================
//==
//==  GetDeclaredNamespaces
//==
//==============================================================================
//
//  Returns:
//      The list of namespace declared at this level.
//
//  Description:
//      Gets the namespaces declared at this level. To iterate through the list,
//      use SNamespace::m_pstNextNamespace.
//
//==============================================================================
inline CXmlElement::SNamespace* CXmlElement::GetDeclaredNamespaces()
{
    return m_pstDeclaredNamespaces;
}

//==============================================================================
//==
//==  GetName
//==
//==============================================================================
//
//  Returns:
//      The pointer to the element's name.
//
//  Description:
//      This method returns the element's name.
//
//==============================================================================
inline const char* CXmlElement::GetName() const
{
    return m_pszElementName;
}

//==============================================================================
//==
//==  SetName
//==
//==============================================================================
//
//  Parameters:
//      pszName:
//          The name to set. It MUST not be NULL.
//
//  Description:
//      This method sets the name of the element.
//
//==============================================================================
inline void CXmlElement::SetName(IN const char* pszName)
{
    PrivateSetName(pszName, true);
}

//==============================================================================
//==
//==  SetValue
//==
//==============================================================================
//
//  Parameters:
//      pszValue:
//          The value to set. It can be NULL.
//
//  Description:
//      This method sets the value of the element.
//
//==============================================================================
inline void CXmlElement::SetValue(IN const char* pszValue)
{
    PrivateSetValue(pszValue,  true);
}

//==============================================================================
//==
//==  AppendAttribute
//==
//==============================================================================
//
//  Parameters:
//      pszAttrNamespace:
//          The namespace of the attribute.
//
//      pszAttrName:
//          The name of the attribute. It must not be NULL.
//
//      pszAttrValue:
//          The value of the attribute. It must not be NULL.
//
//  Description:
//      This method appends an attribute to the end of the attribute list of the
//      current element.
//
//==============================================================================
inline mxt_result CXmlElement::AppendAttribute(IN const char* pszAttrNamespace,
                                               IN const char* pszAttrName,
                                               IN const char* pszAttrValue)
{
    return PrivateAppendAttribute(pszAttrNamespace, pszAttrName, pszAttrValue, true);
}

//==============================================================================
//==
//==  CreateElement
//==
//==============================================================================
//
//  Parameters:
//      pszNamespace:
//          The namespace of the new element. It can be NULL.
//
//          When non-NULL, the namespace MUST have been declared in the new
//          element's parents. Otherwise this method will fail.
//
//      pszElementName:
//          The name of the new element. It must not be NULL.
//
//      pszValue:
//          The value of the new element.
//
//      ePos:
//          The position where the element must be inserted.
//
//          - ePOS_FIRST_SIBLING: The new element is created as the first child
//            of the parent element.
//          - ePOS_PREVIOUS_SIBLING: The new element is created and inserted as
//            the previous sibling of this element.
//          - ePOS_NEXT_SIBLING: The new element is created and inserted as the
//            next sibling of this element.
//          - ePOS_LAST_SIBLING: The new element is created and inserted as the
//            last child of the parent element.
//          - ePOS_FIRST_CHILD: The new element is created and inserted as the
//            first child of this element.
//          - ePOS_LAST_CHILD: The new element is created and inserted as the
//            last child of this element.
//
//  Returns:
//      A pointer to the new element.
//
//      NULL if the insertion failed (for example, if pszElementName was
//      invalid).
//
//  Description:
//      Creates an element that is attached at the requested place.
//
//      This method cannot be called on a root element to create a sibling. Only
//      child elements can be created from a root element.
//
//  See Also:
//      IXmlDocument::CreateRootElement, IXmlDocument::GetRootElement,
//      IXmlDocument::DeleteRootElement.
//
//==============================================================================
inline CXmlElement* CXmlElement::CreateElement(IN const char* pszNamespace,
                                               IN const char* pszElementName,
                                               IN const char* pszValue,
                                               IN ECreatePosition ePos)
{
    return PrivateCreateElement(pszNamespace, pszElementName, pszValue, ePos, true);
}

//==============================================================================
//==
//==  CreateElement
//==
//==============================================================================
//
//  Parameters:
//      pszNamespaceUri:
//          The namespace URI of the element to create.
//
//          The namespace may be declared at this element level depending on the
//          eBehavior parameter.
//
//          It MUST NOT be NULL.
//
//      pszNamespacePrefix:
//          The prefix for pszNamespaceUri. It can be NULL if pszNamespaceUri is
//          the default namespace.
//
//          This parameter is ignored if the eBehavior parameter is set to
//          eOPTIMIZE_NS and pszNamespaceUri is already declared in one of the
//          parents.
//
//      pszElementName:
//          The name of the new element. It must not be NULL.
//
//      pszValue:
//          The value of the new element.
//
//      ePos:
//          The position where the element must be inserted.
//
//          - ePOS_FIRST_SIBLING: The new element is created as the first child
//            of the parent element.
//          - ePOS_PREVIOUS_SIBLING: The new element is created and inserted as
//            the previous sibling of this element.
//          - ePOS_NEXT_SIBLING: The new element is created and inserted as the
//            next sibling of this element.
//          - ePOS_LAST_SIBLING: The new element is created and inserted as the
//            last child of the parent element.
//          - ePOS_FIRST_CHILD: The new element is created and inserted as the
//            first child of this element.
//          - ePOS_LAST_CHILD: The new element is created and inserted as the
//            last child of this element.
//
//      eBehavior:
//          The behavior to have to declare the namespace.
//
//      ePosition:
//          The position specifying where to declare missing namespaces.
//
//  Returns:
//      A pointer to the new element.
//
//      NULL if the insertion failed (for example, if pszElementName was
//      invalid).
//
//  Description:
//      Creates an element and attaches it at the requested place in relation
//      with this element.
//
//      When calling this method, the specified namespace is declared in
//      the created element if is not already declared in a parent element and
//      usable in the created element.
//
//      This method cannot be called on a root element to create a sibling. Only
//      child elements can be created from a root element.
//
//  See Also:
//      IXmlDocument::CreateRootElement, IXmlDocument::GetRootElement,
//      IXmlDocument::DeleteRootElement, ENsDeclarationBehavior,
//      ENsDeclarationPosition
//
//==============================================================================
inline CXmlElement* CXmlElement::CreateElement(IN const char* pszNamespaceUri,
                                               IN const char* pszNamespacePrefix,
                                               IN const char* pszElementName,
                                               IN const char* pszValue,
                                               IN ECreatePosition ePos,
                                               IN ENsDeclarationBehavior eBehavior,
                                               IN ENsDeclarationPosition ePosition)
{
    return PrivateCreateElement(pszNamespaceUri,
                                pszNamespacePrefix,
                                pszElementName,
                                pszValue,
                                ePos,
                                eBehavior,
                                ePosition,
                                true);
}


//==============================================================================
//==
//==  GetChildElement
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          The index of the child element.
//
//  Returns:
//      The pointer to the child element at the given index. It can be NULL if 
//      no element is at the given index.
//
//  Description:
//      This method gets the child element located at the given index.
//
//==============================================================================
inline const CXmlElement* CXmlElement::GetChildElement(IN unsigned int uIndex) const
{
    return const_cast<CXmlElement*>(this)->GetChildElement(uIndex);
}

//==============================================================================
//==
//==  FindChildElement
//==
//==============================================================================
//
//  Parameters:
//      pszNamespace:
//          The namespace to match.
//
//      pszElementName:
//          The name to match. It must not be NULL.
//
//      uIndex:
//          The number of times the namespace and name must match before
//          returning the element. If 0, the first element that matches is
//          returned.
//
//  Returns:
//      The pointer to the child element found with the given parameters. It can
//      be NULL if no element was found.
//
//  Description:
//      This method tries to match a number of times (uIndex + 1) the given
//      namespace and name pair. It then returns an element pointer if there is
//      any. If uIndex is 0, the first element matched is returned, if 1
//      the second, and so on.
//
//==============================================================================
inline const CXmlElement* CXmlElement::FindChildElement(IN const char* pszNamespace,
                                                        IN const char* pszElementName,
                                                        IN unsigned int uIndex) const
{
    return const_cast<CXmlElement*>(this)->FindChildElement(pszNamespace,
                                                            pszElementName,
                                                            uIndex);
}

//==============================================================================
//==
//==  GetNextSibling
//==
//==============================================================================
//
//  Returns:
//      The pointer to the next sibling, or NULL if there is none.
//
//  Description:
//      This method returns the pointer to the next sibling element if there is
//      any. A sibling element is one that shares the same level (same parent
//      element) as the current element.
//
//==============================================================================
inline CXmlElement* CXmlElement::GetNextSibling()
{
    return m_pNextSiblingElement;
}

//==============================================================================
//==
//==  GetNextSibling
//==
//==============================================================================
//
//  Returns:
//      The pointer to the next sibling, or NULL if there is none.
//
//  Description:
//      This method returns the pointer to the next sibling element if there is
//      any. A sibling element is one that shares the same level (same parent
//      element) as the current element.
//
//==============================================================================
inline const CXmlElement* CXmlElement::GetNextSibling() const
{
    return const_cast<CXmlElement*>(this)->GetNextSibling();
}

//==============================================================================
//==
//==  GetPreviousSibling
//==
//==============================================================================
//
//  Returns:
//      The pointer to the previous sibling, or NULL if there is none.
//
//  Description:
//      This method returns the pointer to the previous sibling element if there
//      is any. A sibling element is one that shares the same level as the
//      current element.
//
//==============================================================================
inline const CXmlElement* CXmlElement::GetPreviousSibling() const
{
    return const_cast<CXmlElement*>(this)->GetPreviousSibling();
}

//==============================================================================
//==
//==  GetFirstSibling
//==
//==============================================================================
//
//  Returns:
//      The pointer to the first sibling, this element can be returned if it is
//      the first sibling.
//
//  Description:
//      This method returns the pointer to the first sibling element from this
//      element.
//
//      A sibling element is one that shares the same level as the current
//      element.
//
//==============================================================================
inline const CXmlElement* CXmlElement::GetFirstSibling() const
{
    return const_cast<CXmlElement*>(this)->GetFirstSibling();
}

//==============================================================================
//==
//==  GetLastSibling
//==
//==============================================================================
//
//  Returns:
//      The pointer to the last sibling, this element can be returned if it is
//      the last sibling.
//
//  Description:
//      This method returns the pointer to the last sibling element from this
//      element.
//
//      A sibling element is one that shares the same level as the current
//      element.
//
//==============================================================================
inline const CXmlElement* CXmlElement::GetLastSibling() const
{
    return const_cast<CXmlElement*>(this)->GetLastSibling();
}

//==============================================================================
//==
//==  GetParentElement
//==
//==============================================================================
//
//  Returns:
//      The pointer to the parent, or NULL if it is the root element.
//
//  Description:
//      This method returns the pointer to the parent element if there is any. A
//      parent element is the one that contains the current element.
//
//==============================================================================
inline const CXmlElement* CXmlElement::GetParentElement() const
{
    return const_cast<CXmlElement*>(this)->GetParentElement();
}

//==============================================================================
//==
//==  SetOpaque
//==
//==============================================================================
//
//  Parameters:
//    opq:
//      The opaque to set.
//
//  Description:
//      Sets the opaque associated with this CXmlElement.
//
//==============================================================================
inline void CXmlElement::SetOpaque(mxt_opaque opq)
{
    m_opq = opq;
}

//==============================================================================
//==
//==  GetOpaque
//==
//==============================================================================
//
//  Returns:
//      This CXmlElement's opaque.
//
//  Description:
//      Returns the opaque associated with this CXmlElement.
//
//==============================================================================
inline const mxt_opaque CXmlElement::GetOpaque()
{
    return m_opq;
}

//M5T_INTERNAL_USE_BEGIN

//==============================================================================
//==
//==  GetInternalNamespaceByPrefix
//==
//==============================================================================
//
//  Parameters:
//      pszNamespacePrefix:
//          The prefix of the namespace to get.
//
//          MAY be NULL in which case a default namespace will match.
//
//  Returns:
//      - The namespace having pszNamespacePrefix as prefix.
//      - NULL if pszNamespacePrefix is not declared in this element.
//
//  Description:
//      Gets a namespace defined in this object by its prefix.
//
//==============================================================================
inline const CXmlElement::SNamespace* CXmlElement::GetInternalNamespaceByPrefix(IN const char* const pszNamespacePrefix) const
{
    return const_cast<CXmlElement*>(this)->GetInternalNamespaceByPrefix(pszNamespacePrefix);
}

//==============================================================================
//==
//==  SetNamespace
//==
//==============================================================================
//
//  Parameters:
//      pstNamespace:
//       The namespace to set.
//
//  Description:
//      Sets the SNamespace.
//
//==============================================================================
inline void CXmlElement::SetNamespace(IN const SNamespace* pstNamespace)
{
    m_pstElementNamespace = pstNamespace;
}

//==============================================================================
//==
//==  CompareDictionaryString
//==
//==============================================================================
//
//  Parameters:
//      pszFirstString:
//       The first C-style string to compare. Can be NULL.
//
//      pszSecondString:
//       The second C-style string to compare. Can be NULL.
//
//  Description:
//      Compares two C-style strings by their pointer and then value.
//
//==============================================================================
inline bool CXmlElement::CompareDictionaryString(IN const char* pszFirstString,
                                                 IN const char* pszSecondString) const
{
    return (pszFirstString == pszSecondString) ||
           (pszFirstString != NULL &&
            pszSecondString != NULL &&
            CompareHelper(pszFirstString,
                   pszSecondString) == 0);
}

//==============================================================================
//==
//==  SetInBitset
//==
//==============================================================================
//
//  Parameters:
//      cBitmask:
//       The value to set in cBitset.
//
//      rcBitset:
//       The bitset where data is kept.
//
//  Description:
//      Sets the specified bitmask in the specified bitset.
//
//==============================================================================
inline void SetInBitset(IN char cBitmask,
                        INOUT char& rcBitset)
{
    rcBitset = rcBitset | cBitmask;
}


//==============================================================================
//==
//==  ResetInBitset
//==
//==============================================================================
//
//  Parameters:
//      cBitmask:
//       The value to reset in rcBitset.
//
//      rcBitset:
//       The bitset where data is kept.
//
//  Description:
//      Resets the specified bitmask in the specified bitset.
//
//==============================================================================
inline void ResetInBitset(IN char cBitmask,
                          INOUT char& rcBitset)
{
    rcBitset = rcBitset & (~cBitmask);
}


//==============================================================================
//==
//==  IsInBitset
//==
//==============================================================================
//
//  Parameters:
//      cBitmask:
//       The value to search in cBitset.
//
//      cBitset:
//       The bitset to search in.
//
//  Returns:
//      true:
//       cBitmask is present in cBitset.
//
//      false:
//       cBitmask is not present in cBitset.
//
//  Description:
//      Verifies if a bitmask value is present in the specified bitset.
//
//==============================================================================
inline bool IsInBitset(IN char cBitmask,
                       IN char cBitset)
{
    return (cBitset & cBitmask) == cBitmask;
}

//==============================================================================
//==
//==  SetInCharacteristics
//==
//==============================================================================
//
//  Parameters:
//      cBitmask:
//       The value to set in m_cElementCharacteristics.
//
//  Description:
//      Sets the specified bitmask in m_cElementCharacteristics.
//
//==============================================================================
inline void CXmlElement::SetInCharacteristics(IN char cBitmask)
{
    SetInBitset(cBitmask,
                INOUT m_cElementCharacteristics);
}


//==============================================================================
//==
//==  ResetInCharacteristics
//==
//==============================================================================
//
//  Parameters:
//      cBitmask:
//       The value to reset in m_cElementCharacteristics.
//
//  Description:
//      Resets the specified bitmask in m_cElementCharacteristics.
//
//==============================================================================
inline void CXmlElement::ResetInCharacteristics(IN char cBitmask)
{
    ResetInBitset(cBitmask,
                  INOUT m_cElementCharacteristics);
}


//==============================================================================
//==
//==  IsInCharacteristics
//==
//==============================================================================
//
//  Parameters:
//      cBitmask:
//       The value to search in m_cElementCharacteristics.
//
//  Returns:
//      true:
//       cBitmask is present in m_cElementCharacteristics.
//
//      false:
//       cBitmask is not present in m_cElementCharacteristics.
//
//  Description:
//      Verifies if a bitmask value is present in the m_cElementCharacteristics
//      bitset.
//
//==============================================================================
inline bool CXmlElement::IsInCharacteristics(IN char cBitmask) const
{
    return IsInBitset(cBitmask,
                      m_cElementCharacteristics);
}
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CXMLELEMENT_H
