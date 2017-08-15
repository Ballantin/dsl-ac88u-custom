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
#ifndef MXG_IXMLDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXMLDOCUMENT_H
//M5T_INTERNAL_USE_END

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

#if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CBlob;
class CXmlElement;
class IAllocator;
class IXmlDocumentMgr;
class IXmlPatchMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IXmlDocument);
//M5T_INTERNAL_USE_END

//==============================================================================
//== Class: IXmlDocument
//========================================
//<GROUP XML_CLASSES>
//
// Description:
//  The IXmlDocument interface represents an XML document in the Framework's XML
//  Document Object Model (DOM). An XML document is represented as a tree of
//  CXmlElements.
//
//  This interface can only handle standalone UTF-8 encoded XML documents.
//
//  The document can either be created through parsing of an XML document or by
//  setting values directly into the document.
//
//  The user of this interface only has to call the Parse method when the
//  IXmlDocument is used for parsing and accessing an existing document. The
//  root element is accessible through GetRootElement. The child elements of the
//  root element are accessible through the various CXmlElement API offered by
//  the root element.
//
//  The user of this interface must take the following steps when creating a new
//  XML document:
//      - Create the XML document using CreateEComInstance.
//      - Create the root element using CreateRootElement
//      - Use the root element API to add child elements.
//      - Use the root or children element API to add more elements to the
//        document.
//      - Use the Serialize method to serialize the document into a buffer.
//
//  An IXmlDocument can only handle a single XML document at a time. This
//  document is either created manually or parsed. It cannot be used to parse
//  many different documents without being reset.
//
// Location:
//   Xml/IXmlDocument.h
//
// See Also:
//   CXmlElement
//
//==============================================================================
class IXmlDocument : public IEComUnknown
{
//-- Friend Declarations.

//-- Published Interface.
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IXmlDocument);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetAllocator
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the memory allocator to use when creating this XML document.
    //
    //  Parameters:
    //    pAllocator:
    //      The allocator to use within the document. It can be NULL, in which
    //      case the normal MX_NEW and MX_DELETE are used.
    //
    //  Returns:
    //      resS_OK upon success, an error if the allocator has already been
    //      set or if it cannot be set at this time.
    //
    //  Description:
    //      Sets the memory allocator to use within the scope of this XML
    //      document. The memory allocator should increase performances
    //      for parsed document or otherwise built from known values that shall
    //      not change over time. For building a document with no previously
    //      known values or otherwise changing values, it is strongly encouraged
    //      NOT to set any allocator and use the normal MX_NEW and MX_DELETE
    //      operators.
    //
    //      This method must be called first on any new document. The allocator
    //      cannot be changed afterwards.
    //
    //      If set, the allocator MUST be valid throughout the life of the
    //      document. Thus, the document MUST be released before deleting the
    //      allocator it uses.
    //
    //==========================================================================
    virtual mxt_result SetAllocator(IN IAllocator* pAllocator) = 0;


    //==========================================================================
    //==
    //==  SetPatchManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets a manager to which detailled changes to the document are
    //      reported.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the manager to configure. It can be NULL, in which case 
    //      no event will be reported.
    //      
    //  Description:
    //      Sets a manager to which detailled changes to the document are
    //      reported.
    //
    //      This API can be called at any time during the life of the
    //      IXmlDocument to change the patch manager.
    //
    //  See Also:
    //      SetDocumentManager
    //
    //==========================================================================
    virtual void SetPatchManager(IN IXmlPatchMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  GetPatchManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured patch manager.
    //
    //  Returns:
    //      Pointer to the currently configured patch manager.
    //      
    //  Description:
    //      Gets the configured patch manager.
    //
    //  See Also:
    //      SetPatchManager
    //
    //==========================================================================
    virtual IXmlPatchMgr* GetPatchManager() = 0;

    //==========================================================================
    //==
    //==  SetDocumentManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets a document manager that can be notified whenever the document
    //      changes.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the manager to configure. It can be NULL, in which case 
    //      no event will be reported.
    //
    //  Description:
    //      Sets a document manager that can be notified whenever the document
    //      changes.
    //
    //      The document manager is only notified once the document has been
    //      changed, however the actual changes are not reported. To be notified
    //      of the detailed changes, the user should configure a patch manager
    //      instead by using SetPatchManager.
    //
    //      This API can be called at any time during the life of the
    //      IXmlDocument to change the document manager.
    //
    //  See Also:
    //      SetPatchManager
    //
    //==========================================================================
    virtual void SetDocumentManager(IN IXmlDocumentMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  GetDocumentManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured document manager.
    //
    //  Returns:
    //      Pointer to the currently configured document manager.
    //      
    //  Description:
    //      Gets the configured document manager.
    //
    //  See Also:
    //      SetDocumentManager
    //
    //==========================================================================
    virtual IXmlDocumentMgr* GetDocumentManager() = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the opaque for this IXmlDocument.
    //
    //  Parameters:
    //    opq:
    //      The opaque to set.
    //
    //  Description:
    //      Sets the opaque associated with this IXmlDocument.
    //
    //==========================================================================
    virtual void SetOpaque(IN mxt_opaque opqData) = 0;


    //==========================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the opaque identifier.
    //
    //  Returns:
    //      This IXmlDocument's opaque.
    //
    //  Description:
    //      Returns the opaque associated with this IXmlDocument.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;
    
    //==========================================================================
    //==
    //==  CreateRootElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Creates the root element.
    //
    //  Parameters:
    //    pszNamespace:
    //      Pointer to the root element's namespace, as a NULL terminated
    //      string. This namespace is automatically declared in the root element
    //      It can be NULL.
    //
    //    pszNamespacePrefix:
    //      Pointer to the prefix for pszNamespace as a NULL terminated string.
    //      It can be NULL if pszNamespace is the default namespace. This
    //      parameter is ignored when pszNamespace is NULL.
    //
    //    pszElementName:
    //      Pointer to the root element's name, as a NULL terminated
    //      string. It cannot be NULL.
    //
    //  Returns:
    //      Pointer to the created root element. NULL pointer if a root element
    //      already exists or if the element name is NULL.
    //
    //  Description:
    //      Creates and configures the root element. This method is only useful
    //      when the caller wants to build an XML document. It is not useful
    //      when parsing an XML document and must not be called before Parse is
    //      called.
    //
    //  See Also:
    //      CXmlElement, DeleteRootElement
    //
    //==========================================================================
    virtual CXmlElement* CreateRootElement(IN const char* pszNamespace,
                                           IN const char* pszNamespacePrefix,
                                           IN const char* pszElementName) = 0;

    //==========================================================================
    //==
    //==  GetRootElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides access to the root element of the XML document.
    //
    //  Returns:
    //      Pointer to the root element. NULL if no root element has been
    //      created yet.
    //
    //  Description:
    //      Provides access to the root element of the XML document.
    //
    //  See Also:
    //      CXmlElement
    //
    //==========================================================================
    virtual CXmlElement* GetRootElement() = 0;

    //==========================================================================
    //==
    //==  GetRootElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides access to the root element of the XML document.
    //
    //  Returns:
    //      Pointer to the root element. NULL if no root element has been
    //      created yet.
    //
    //  Description:
    //      Provides access to the root element of the XML document.
    //
    //  See Also:
    //      CXmlElement
    //
    //==========================================================================
    virtual const CXmlElement* GetRootElement() const = 0;

    //==========================================================================
    //==
    //==  DeleteRootElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Deletes the root element.
    //
    //  Description:
    //      Deletes the root element and all its children, if any. After this
    //      call, the document is effectively empty.
    //
    //  See Also:
    //      CreateRootElement
    //
    //==========================================================================
    virtual void DeleteRootElement() = 0;

    //==========================================================================
    //==
    //==  Parse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Parses a buffer containing an XML document.
    //
    //  Parameters:
    //    puXmlBuffer:
    //      Pointer to the buffer holding the XML document. It cannot be NULL.
    //
    //    uBufferSize:
    //      The size of the buffer to parse.
    //
    //  Returns:
    //      resS_OK upon success, a failure otherwise.
    //
    //  Description:
    //      Parses a buffer containing an XML document. The XML namespaces and
    //      the root element are automatically configured from the information
    //      found in the document. There is no need to create a root element
    //      before calling Parse.
    //
    //      Because of the nature of the design, it is not possible to have both
    //      child element(s) and value, because there is no way to know in
    //      which order they happened. While this does not affect parsing at all,
    //      it will affect the parsed document serialization.
    //
    //      So, it would be impossible to know which of this:
    //      <CODE>
    //          \<test\>
    //              This is a
    //              \<bold\>
    //                  TEST
    //              \</bold\>
    //          \</test\>
    //      </CODE>
    //
    //      or this would be the correct order.
    //      <CODE>
    //          \<test\>
    //              \<bold\>
    //                  TEST
    //              \</bold\>
    //              This is a
    //          \</test\>
    //      </CODE>
    //
    //      Again, this limitation has no impact on the parsing of the document,
    //      because both value and children are kept internally. Only the
    //      element's serialization will be impacted by this.
    //
    //  #Note:#
    //      No patch events are reported during parsing and only one
    //      IXmlDocumentMgr is reported at the end.
    //
    //==========================================================================
    virtual mxt_result Parse(IN const uint8_t* puXmlBuffer,
                             IN const unsigned int uBufferSize) = 0;

    //==========================================================================
    //==
    //==  Serialize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Serializes the current representation of the document into a blob.
    //
    //  Parameters:
    //    rBlob:
    //      The blob into which the XML document is serialized.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Serializes the current representation of the document into a blob.
    //
    //      All defined namespaces are automatically declared as part of the
    //      first XML element.
    //
    //      The document is serialized at the current blob insertion point.
    //      Previously inserted data is thus kept.
    //
    //      If an element within the document has both a value and child
    //      element(s), the value will not be serialized as it is impossible
    //      to know which of the children or the value came first.
    //
    //      So, it would be impossible to know which of this:
    //      <CODE>
    //          \<test\>
    //              This is a
    //              \<bold\>
    //                  TEST
    //              \</bold\>
    //          \</test\>
    //      </CODE>
    //
    //      or this would be the correct order.
    //      <CODE>
    //          \<test\>
    //              \<bold\>
    //                  TEST
    //              \</bold\>
    //              This is a
    //          \</test\>
    //      </CODE>
    //
    //      This is why only the child element(s) will be serialized if both
    //      child element(s) and value are set.
    //
    //==========================================================================
    virtual mxt_result Serialize(INOUT CBlob& rBlob) = 0;


    //==========================================================================
    //==
    //==  SetDictionary
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the dictionary for this IXmlDocument.
    //
    //  Parameters:
    //    pvecpszDictionary:
    //      The dictionary to use to find element names, attribute names and
    //      namespace URI. It MUST NOT be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: pvecpszDictionary is NULL.
    //      - resFE_INVALID_STATE: the dictionary was already set.
    //      - resS_OK: the dictionary is set.
    //
    //  Description:
    //      Sets the dictionary to use to search element names, attribute names
    //      and namespace URI. When one of these is set and its value is found
    //      in the dictionary, the C-style string pointer from the dictionary
    //      is taken instead of creating a new C-style string on the heap.
    //
    //==========================================================================
    virtual mxt_result SetDictionary(const CVector<const char*>* pvecpszDictionary) = 0;


    //==========================================================================
    //==
    //==  GetDictionary
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the dictionary for this IXmlDocument.
    //
    //  Returns:
    //      - The dictionary that was set on this IXmlDocument.
    //      - NULL if no IXmlDocument was set on this IXmlDocument.
    //
    //  Description:
    //      Returns the dictionary for this IXmlDocument.
    //
    //==========================================================================
    virtual const CVector<const char*>* GetDictionary() = 0;

    //==========================================================================
    //==
    //==  SetCaseSensitive
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets whether or not the XML document is case sensitive.
    //
    //  Parameters:
    //    bCaseSensitive:
    //      Whether or not the XML document is case sensitive
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: pvecpszDictionary is NULL.
    //      - resS_OK: upon success.
    //
    //  Description:
    //      Sets whether or not the XML document is case sensitive. The default
    //      value is case sensitive.
    //
    //==========================================================================
    virtual void SetCaseSensitive(IN bool bCaseSensitive) = 0;
    
    
    //==========================================================================
    //==
    //==  IsCaseSensitive
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns whether or not the XML document is case sensitive.
    //
    //  Returns:
    //      - Whether or not the XML document is case sensitive
    //
    //  Description:
    //      Returns whether or not the XML document is case sensitive.
    //
    //==========================================================================
    virtual bool IsCaseSensitive() = 0;


//M5T_INTERNAL_USE_BEGIN

    //==========================================================================
    //==
    //==  AllocateMemory
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allocates memory using the proper allocation method.
    //
    //  Parameters:
    //      uAllocationSize:
    //          The size of the memory to allocate.
    //
    //  Returns:
    //      The pointer to the allocated memory. Ownership of the memory is
    //      given to the caller.
    //
    //  Description:
    //      This method allocates memory using the allocator if it is available.
    //      If the allocator is not available, MX_NEW_ARRAY is used.
    //
    //  Warning:
    //      This method is for internal use only and MUST NOT be used
    //       externally.
    //
    //  See Also:
    //      ReleaseMemory
    //
    //==========================================================================
    virtual uint8_t* GO AllocateMemory(IN unsigned int uAllocationSize) = 0;

    //==========================================================================
    //==
    //==  ReleaseMemory
    //==
    //==========================================================================
    //
    //  Summary:
    //      Releases memory using the proper release method.
    //
    //  Parameters:
    //      pMemoryBlock:
    //          The memory block to release. Ownership is always taken.
    //
    //  Description:
    //      This method releases memory using the allocator is it is available.
    //      If the allocator is not available, MX_DELETE_ARRAY is used.
    //
    //  Warning:
    //      This method is for internal use only and MUST NOT be used
    //      externally.
    //
    //  See Also:
    //      AllocateMemory
    //
    //==========================================================================
    virtual void ReleaseMemory(IN TOA uint8_t* pMemoryBlock) = 0;

    //==========================================================================
    //==
    //==  AllocateString
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allocates memory for a C-style string.
    //
    //  Parameters:
    //      rpszDestination:
    //          The string to allocate memory for and to copy into.
    //
    //      rpszDestination:
    //          The string to copy from.
    //
    //  Description:
    //      This method allocates memory for a string using the correct
    //      memory allocation method. It then copies the source string into the
    //      destination one and adds the termination character.
    //
    //  Warning:
    //      This method is for internal use only and MUST NOT be used
    //      externally.
    //
    //  See Also:
    //      ReleaseString
    //
    //==========================================================================
    virtual void AllocateString(OUT GO char*& rpszDestination,
                                IN const char* pszSource) = 0;

    //==========================================================================
    //==
    //==  ReleaseString
    //==
    //==========================================================================
    //
    //  Summary:
    //      Release memory for a C-style string.
    //
    //  Parameters:
    //      pszString:
    //          The string to release. Ownership is always taken.
    //
    //  Description:
    //      This method releases memory for a string using the correct method.
    //
    //  Warning:
    //      This method is for internal use only and MUST NOT be used
    //      externally.
    //
    //  See Also:
    //      AllocateString
    //
    //==========================================================================
    virtual void ReleaseString(IN TOA char* pszString) = 0;
//M5T_INTERNAL_USE_END


//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IXmlDocument() {}

    // Summary:
    //  Destructor.
    virtual ~IXmlDocument() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IXmlDocument(IN const IXmlDocument& rSrc);

    // Summary:
    //  Assignment Operator.
    IXmlDocument& operator=(IN const IXmlDocument& rSrc);


//-- Hidden Data Members.
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_IXMLDOCUMENT_H
