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
#ifndef MXG_CSCEXCAPPATCHOPERATIONS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEXCAPPATCHOPERATIONS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCEUAIMPPCFG_H
#include "Config/SceUaImppCfg.h"
#endif

#if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)

#ifndef MXG_IXMLPATCHMGR_H
#include "Xml/IXmlPatchMgr.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CMimeObject;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceXcapPatchOperations
//==============================================================================
//
// Summary:
//   This class manages XCAP patch operations.
//
// Description:
//   This class transforms IXmlDocument changes into a list of patch operations
//   to be done on a remote copy of the same XML document.
//
//   The patch operations are listed as a list of XCAP operations defined in
//   RFC4825.
//
// Location:
//   SceUaImpp/CSceXcapPatchOperations.h
//
//  See Also:
//   IXmlDocument
//
//==============================================================================
class CSceXcapPatchOperations : public IXmlPatchMgr
{
//-- Friend Declarations

//-- New types within class namespace
public:

    // Description:
    //  These are the HTTP requests that can be used to send XCAP payload.
    enum EXcapPatchOperationHttpMethod
    {
        // Description:
        //  The given XCAP content must be sent in a PUT HTTP request.
        eHTTP_PUT,

        // Description:
        //  The given XCAP content must be sent in a DELETE HTTP request.
        eHTTP_DELETE
    };


protected:
private:

    // These are the possible operations.
    enum EXcapPatchOperation
    {
        eOP_CREATE_ELEMENT,
        eOP_UPDATE_ELEMENT,
        eOP_DELETE_ELEMENT,
        eOP_OTHER_PUT,
        eOP_OTHER_DELETE
    };

    // This represents an XCAP operation.
    struct SXcapOperation
    {
        CString* m_pstrNodeSelector;

        EXcapPatchOperation m_eOperation;

        CMimeObject* m_pRequestPayload;

        // Points to the selected element, if one is selected. This pointer
        // MUST NEVER be casted back to a CXmlElement since it might be deleted.
        const void* m_pSelectedElement;

        SXcapOperation();

        ~SXcapOperation();
    };

    // This is needed to be able to compile with MSVC6 in which it is not
    // possible for a struct declared in a class to access the enumeration
    // values that are declared as private or protected in the same class.
    friend struct SXcapOperation;

    // The prefix to use in a selector when none is applicable.
    static char s_szDefaultPrefixValue[];


//-- Published Interface
public:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceXcapPatchOperations();

    // Summary:
    //  Destructor.
    virtual ~CSceXcapPatchOperations();


    //-- << Patch operation management >>

    // Summary:
    //  Returns the first patch operation and removes it from the list.
    mxt_result GetFirstOperation(OUT GO CString*& rpstrNodeSelector,
                                 OUT EXcapPatchOperationHttpMethod& reHttpMethod,
                                 OUT GO CMimeObject*& rpRequestPayload);

    // Summary:
    //  Returns the number of XCAP operations that are accumulated.
    unsigned int GetNumberOfOperations() const;

    // Summary:
    //  Removes the accumulated patches from this element.
    mxt_result ResetPatches();


//-- Hidden Methods
protected:

    //-- << IXmlPatchMgr >>
    virtual void EvNewElement(IN IXmlDocument* pDocument,
                              IN const CXmlElement* pNewElementAfterInsertion);

    virtual void EvDeletedElement(IN IXmlDocument* pDocument,
                                  IN const CXmlElement* pDeletedElement,
                                  IN const CXmlElement* pParentElement,
                                  IN unsigned int uIndex);

    virtual void EvChangedElement(IN IXmlDocument* pDocument,
                                  IN const CXmlElement* pUpdatedElement);

    virtual void EvChangedElementName(IN IXmlDocument* pDocument,
                                      IN const CXmlElement* pUpdatedElement);

    virtual void EvChangedElementValue(IN IXmlDocument* pDocument,
                                       IN const CXmlElement* pUpdatedElement);

    virtual void EvNewAttribute(IN IXmlDocument* pDocument,
                                IN const CXmlElement* pUpdatedElement,
                                IN const CXmlElement::SNamespace* pstNamespace,
                                IN const char* pszAttributeName,
                                IN const char* pszAttributeValue);

    virtual void EvDeletedAttribute(IN IXmlDocument* pDocument,
                                    IN const CXmlElement* pUpdatedElement,
                                    IN const CXmlElement::SNamespace* pstNamespace,
                                    IN const char* pszAttributeName);

    virtual void EvChangedAttribute(IN IXmlDocument* pDocument,
                                    IN const CXmlElement* pUpdatedElement,
                                    IN const CXmlElement::SNamespace* pstNamespace,
                                    IN const char* pszAttributeName,
                                    IN const char* pszAttributeValue);

    virtual void EvNewNamespace(IN IXmlDocument* pDocument,
                                IN CXmlElement* pUpdatedElement,
                                IN CXmlElement::SNamespace* pstNewNamespace);

    virtual void EvChangedNamespace(IN IXmlDocument* pDocument,
                                    IN CXmlElement* pUpdatedElement,
                                    IN CXmlElement::SNamespace* pstChangedNamespace);

    virtual void EvDeletedNamespace(IN IXmlDocument* pDocument,
                                    IN CXmlElement* pUpdatedElement,
                                    IN CXmlElement::SNamespace* pstDeletedNamespace);


    //-- << Helpers >>

    // Summary:
    //  Tries to reuse the last operation to contain the new operation.
    mxt_result ReuseLastOperation(IN EXcapPatchOperation eCurrentOperation,
                                  IN const CXmlElement& rCurrentElement);

    // Summary:
    //  Verifies if a pointer represents the parent of the specified element.
    const CXmlElement* IsParentOfElement(IN const void* pParentToSearch,
                                         IN const CXmlElement& rElement);

    // Summary:
    //  Verifies if a pointer represents the child of the specified parent.
    static bool IsChildOfElement(IN const void* pChildToSearch,
                                 IN const CXmlElement& rParent);

    // Summary:
    //  Searches for a unique attribute.
    mxt_result FindUniqueAttribute(IN const CXmlElement& rSelectedElement,
                                   IN const CXmlElement* pParentElement,
                                   IN const CXmlElement& rElementToDiscriminate,
                                   OUT const char*& rpszUniqueAttrNsUri,
                                   OUT const char*& rpszUniqueAttrPreferredNsPrefix,
                                   OUT const char*& rpszUniqueAttrName,
                                   OUT const char*& rpszUniqueAttrValue);

    // Summary:
    //  Appends an attribute predicate made from a unique attribute.
    mxt_result AppendPredicateAttr(IN const char* pszUniqueAttrNsUri,
                                   IN const char* pszUniqueAttrPreferredNsPrefix,
                                   IN const char* pszUniqueAttrName,
                                   IN const char* pszUniqueAttrValue,
                                   INOUT CString& rstrSelector,
                                   INOUT CVector<CXmlElement::SNamespace>* pvecstDeclaredNs = NULL);

    // Summary:
    //  Generates an XCAP operation where an attribute is added, changed or
    //  deleted.
    mxt_result GenerateAttributeOperation(IN const CXmlElement& rUpdatedElement,
                                          IN const CXmlElement::SNamespace* pstNamespace,
                                          IN const char* pszAttributeName,
                                          IN const char* pszNewAttributeValue);

    // Summary:
    //  Generates a selector with a position predicate in the last step.
    mxt_result GenerateSelectorByIndex(IN const CXmlElement* pParentElement,
                                       IN unsigned int uElementIndex,
                                       OUT CString& rstrSelector,
                                       IN const char* pszNameTestNsUri = NULL,
                                       IN const char* pszNameTestPreferredNsPrefix = NULL,
                                       IN const char* pszNameTest = NULL,
                                       INOUT CVector<CXmlElement::SNamespace>* pvecstAlreadyDefinedNs = NULL);

    // Summary:
    //  Generates a node selector only from position predicates.
    static mxt_result GenerateSelectorPathByPos(IN const CXmlElement* pTopElement,
                                                IN bool bAddSlash,
                                                OUT CString& rstrNodeSelector);

    // Summary:
    //  Inserts a QName to a CString and declares the needed namespace.
    static mxt_result InsertQname(IN const char* pszNsUri,
                                  IN const char* pszPreferredNsPrefix,
                                  IN const char* pszName,
                                  INOUT CString& rstrTarget,
                                  IN unsigned int uInsertionIndex = ~0,
                                  INOUT CVector<CXmlElement::SNamespace>* pvecstAlreadyDefinedNs = NULL);

    // Summary:
    //  Generates an operation from the specified parameters.
    mxt_result GenerateOperation(IN TOA CString* pstrNodeSelector,
                                 IN EXcapPatchOperation eXcapPatchOperation,
                                 IN const CXmlElement* pSelectedElement,
                                 IN TOA CBlob* pRequestContent = NULL,
                                 IN const char* pszContentTypeMSubType = NULL,
                                 OUT unsigned int* puXcapOperationIndex = NULL);

    // Summary:
    //  Generates a CMimeObject.
    static mxt_result CreateMimeObject(IN TOA CBlob* pMimeContent,
                                       IN const char* pszContentTypeSubType,
                                       OUT GO CMimeObject*& rpCreatedMimeObject);

    // Summary:
    //  Serializes an element into a blob.
    static GO CBlob* SerializeElementForPayload(IN const CXmlElement& rElementToSerialize);


    // Summary:
    //  Sets in a mode where it is known that an error was faced.
    void SetInErrorMode();

    // Summary:
    //  Tells if two QName constructs are equivalent.
    static bool AreQnameEquivalent(IN const char* pszNsUri1,
                                   IN const char* pszName1,
                                   IN const char* pszNsUri2,
                                   IN const char* pszName2);

    // Summary:
    //  Compares two strings considering NULL and empty as equivalent.
    static bool IsEquivalentStringWithNullAsEmpty(IN const char* pszString1,
                                                  IN const char* pszString2);


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceXcapPatchOperations(IN const CSceXcapPatchOperations& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceXcapPatchOperations& operator=(IN const CSceXcapPatchOperations& rSrc);


//-- Hidden Data Members
protected:
private:

    // The list of operations that were not accessed yet.
    CList<SXcapOperation> m_lststXcapOperation;

    // Tells if an error was faced while generating patches.
    bool m_bPatchGenerationError;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  PUBLIC METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetNumberOfOperations
//==
//==============================================================================
//
//  Returns:
//      The number of operations that are accumulated.
//
//  Description:
//      Returns the number of XCAP operations that are accumulated in this
//      element.
//
//==============================================================================
inline unsigned int CSceXcapPatchOperations::GetNumberOfOperations() const
{
    return m_lststXcapOperation.GetSize();
}


//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==   AreQnameEquivalent
//==
//==============================================================================
//
//  Parameters:
//      pszNsUri1: the first namespace URI to compare.
//
//      pszName1: the first name to compare. MUST NOT be NULL.
//
//      pszNsUri2: the second namespace URI to compare.
//
//      pszName2: the second name to compare. MUST NOT be NULL.
//
//  Returns:
//      - true if both QNames are the same.
//      - false otherwise.
//
//  Description:
//      Tells if two QName are equivalent.
//
//==============================================================================
inline bool CSceXcapPatchOperations::AreQnameEquivalent(IN const char* pszNsUri1,
                                                        IN const char* pszName1,
                                                        IN const char* pszNsUri2,
                                                        IN const char* pszName2)
{
    return (pszNsUri1 == pszNsUri2 ||
            pszNsUri1 != NULL &&
            pszNsUri2 != NULL &&
            strcmp(pszNsUri1,
                   pszNsUri2) == 0) &&
            strcmp(pszName1,
                   pszName2) == 0;
}


//==============================================================================
//==
//==   IsEquivalentStringWithNullAsEmpty
//==
//==============================================================================
//
//  Parameters:
//      pszString1: the first string to compare.
//
//      pszString2: the second string to compare.
//
//  Returns:
//      - true if the strings are equivalent.
//      - false otherwise.
//
//  Description:
//      Compares two strings considering NULL and empty as equivalent.
//
//==============================================================================
inline bool CSceXcapPatchOperations::IsEquivalentStringWithNullAsEmpty(IN const char* pszString1,
                                                                       IN const char* pszString2)
{
    return strcmp(MX_MAKE_NULL_EMPTY_STRING(pszString1),
                  MX_MAKE_NULL_EMPTY_STRING(pszString2)) == 0;
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CSCEXCAPPATCHOPERATIONS_H

//M5T_INTERNAL_USE_END
