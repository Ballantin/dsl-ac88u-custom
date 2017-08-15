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
#ifndef MXG_CSCEXMLPATCHOPERATIONS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEXMLPATCHOPERATIONS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IXMLPATCHMGR_H
#include "Xml/IXmlPatchMgr.h"
#endif

#ifndef MXG_CXMLELEMENT_H
#include "Xml/CXmlElement.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceXmlPatchOperations
//==============================================================================
//
// Summary:
//   This class manages XML patch operations.
//
// Description:
//   This class transforms IXmlDocument changes into a document containing a
//   list of patch operations to be done on a remote copy of the same XML
//   document.
//
//   The patch operations are listed in a <diff> element as defined in RFC5261.
//
//   This class also has a helper function that applies changes described in an
//   XML patch operations (RFC5261) based format to an IXmlDocument.
//
// Location:
//   SceUaImpp/CSceXmlPatchOperations.h
//
//  See Also:
//   IXmlDocument
//
//==============================================================================
class CSceXmlPatchOperations : public IXmlPatchMgr
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:

    enum EPatchOperation
    {
        // Description:
        //  An <add> element.
        ePO_ADD,

        // Description:
        //  A <replace> element.
        ePO_REPLACE,

        // Description:
        //  A <remove> element.
        ePO_REMOVE
    };


    // Supported predicate types.
    enum EPredicateType
    {
        // From RFC 5261 pos : "\[\d+\]"
        // From http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/ :
        //      "\d" : "\p{Nd}"
        //      "\p{Nd}" : decimal digit
        //
        // ABNF equivalent :
        //      "[" 1*DIGIT "]"
        ePREDICATE_POSITION,

        // From RFC 5261 :
        //      attr : "\[&aname;='(.)*'\]|\[&aname;=&quot;(.)*&quot;\]"
        //      aname : "@&qname;"
        //      qname : "(&ncname;:)?&ncname;"
        //      ncname : "\i\c*"
        // From http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/ :
        //      '.' : "[^\n\r]"
        //      "\i" : the set of initial name characters, those matched by
        //             Letter | '_' | ':'
        //      "\c" : the set of name characters, those matched by NameChar
        // From http://www.w3.org/TR/2006/REC-xml-20060816 :
        //      "&quot;" : '"'
        //
        // ABNF equivalent :
        //      "[@" qname "=" (("'" non-crlf-string "'")/
        //                      ('"' non-crlf-string '"')) "]"
        ePREDICATE_ATTRIBUTE,

        // From RFC 5261 :
        //      value : "\[(&qname;|\.)='(.)*'\]|&valueq;"
        //      valueq : "\[(&qname;|\.)=&quot;(.)*&quot;\]"
        //      qname : "(&ncname;:)?&ncname;"
        //      ncname : "\i\c*"
        // From http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/ :
        //      '.' : "[^\n\r]"
        //      "\i" : the set of initial name characters, those matched by
        //             Letter | '_' | ':'
        //      "\c" : the set of name characters, those matched by NameChar
        // From http://www.w3.org/TR/2006/REC-xml-20060816 :
        //      quot : '"'
        //
        // ABNF equivalent :
        //      "[" ( qname / "." ) "=" (("'" non-crlf-string "'")/
        //                               ('"' non-crlf-string '"')) "]"
        ePREDICATE_ELEMENT_VALUE
    };


    struct SPredicate
    {
        // The type of the predicate.
        EPredicateType m_ePredicateType;

        // The prefix for the namespace declaring the m_pszName value:
        //  - ePREDICATE_POSITION : NULL.
        //  - ePREDICATE_ATTRIBUTE : - The namespace URI of the namespace where
        //                             the attribute is defined.
        //                           - NULL for none.
        //  - ePREDICATE_ELEMENT_VALUE : - the namespace URI of the namespace
        //                                 where the element isdefined.
        //                               - NULL for none.
        const char* m_pszNamespace;

        // The name of the predicate:
        //  - ePREDICATE_POSITION : NULL.
        //  - ePREDICATE_ATTRIBUTE : the name of the attribute (the string
        //                           after '@' and before the '=').
        //  - ePREDICATE_ELEMENT_VALUE : the string before the '='.
        char* m_pszName;

        // The length of the name:
        //  - ePREDICATE_POSITION : the position value (the number between
        //                          '[' and ']') (note that the first element
        //                          has an XML position of 1.
        //  - ePREDICATE_ATTRIBUTE and
        //    ePREDICATE_ELEMENT_VALUE : length of m_pszName (note that
        //                               m_pszName[length] points to ']' that
        //                               was changed to a '\0').
        unsigned int m_uNameLength;

        // The value of the predicate:
        //  - ePREDICATE_POSITION : NULL.
        //  - ePREDICATE_ATTRIBUTE : the value of the attribute (the string in
        //                           the simple or double quotes).
        //  - ePREDICATE_ELEMENT_VALUE : the value of the element (the string in
        //                               the simple or double quotes).
        char* m_pszValue;

        // The length of the value:
        //  - ePREDICATE_POSITION : 0.
        //  - ePREDICATE_ATTRIBUTE : the length of the string in simple or
        //                           double quotes (note that m_pszValue[length]
        //                           points to '"' or "'" that was changed to a
        //                           '\0').
        //  - ePREDICATE_ELEMENT_VALUE : the length of the string in simple or
        //                               double quotes (note that
        //                               m_pszValue[length] points to '"' or "'"
        //                               that was changed to a '\0').
        unsigned int m_uValueLength;

        SPredicate()
        :   m_ePredicateType(ePREDICATE_POSITION),
            m_pszNamespace(NULL),
            m_pszName(NULL),
            m_uNameLength(0),
            m_pszValue(NULL),
            m_uValueLength(0)
        {}
    };

    // This is needed to be able to compile with MSVC6 in which it is not
    // possible for a struct declared in a class to access the enumeration
    // values that are declared as private or protected in the same class.
    friend struct SPredicate;


    // Supported node test type.
    enum ENodeTestType
    {
        // From RFC 5261 :
        //      id : "id\(('&ncname;')?\)|id\((&quot;&ncname;&quot;)?\)"
        //      ncname : "\i\c*"
        // From http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/ :
        //      "\i" : the set of initial name characters, those matched by
        //             Letter | '_' | ':'
        //      "\c" : the set of name characters, those matched by NameChar
        // From http://www.w3.org/TR/2006/REC-xml-20060816 :
        //      quot : '"'
        eNODE_SET_FUNCTION_ID,

        // From RFC 5261 :
        //      step   "(&qname;|\*)(&cond;)*"
        //
        // This type is the first part of the step construction
        // ("(&qname;|\*)").
        //
        // From RFC 5261 :
        //      qname : "(&ncname;:)?&ncname;"
        //      ncname : "\i\c*"
        // From http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/ :
        //      "\i" : the set of initial name characters, those matched by
        //             Letter | '_' | ':'
        //      "\c" : the set of name characters, those matched by NameChar
        eNODE_TEST_NAME_TEST,

        // From RFC 5261 :
        //      aname : "@&qname;"
        //      qname : "(&ncname;:)?&ncname;"
        //      ncname : "\i\c*"
        // From http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/ :
        //      "\i" : the set of initial name characters, those matched by
        //             Letter | '_' | ':'
        //      "\c" : the set of name characters, those matched by NameChar
        eNODE_TEST_ATTRIBUTE_NAME,

        // From RFC 5261 :
        //      nspa : "namespace::&ncname;"
        //      ncname : "\i\c*"
        // From http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/ :
        //      "\i" : the set of initial name characters, those matched by
        //             Letter | '_' | ':'
        //      "\c" : the set of name characters, those matched by NameChar
        eAXIS_NAMESPACE_PREFIX,

        // From RFC 5261 :
        //      com : "comment\(\)"
        eNODE_TEST_COMMENT,

        // From RFC 5261 :
        //      pi : "processing-instruction\(('&ncname;')?\)|&piq;"
        //      piq : "processing-instruction\((&quot;&ncname;&quot;)\)"
        //      ncname : "\i\c*"
        // From http://www.w3.org/TR/2004/REC-xmlschema-2-20041028/ :
        //      "\i" : the set of initial name characters, those matched by
        //             Letter | '_' | ':'
        //      "\c" : the set of name characters, those matched by NameChar
        // From http://www.w3.org/TR/2006/REC-xml-20060816 :
        //      quot : '"'
        eNODE_TEST_PROCESSING_INSTRUCTION,

        // From RFC 5261 :
        //      text : "text\(\)"
        eNODE_TEST_TEXT
    };


    struct SLocationStep
    {
        // The type of the node test.
        ENodeTestType m_eNodeTestType;

        // The value of the node test:
        //  - eNODE_SET_FUNCTION_ID: - NULL if there was nothing in the
        //                             parenthesis.
        //                           - The value of the id parameter to select
        //                             (empty strings will return NULL).
        //  - eNODE_TEST_NAME_TEST: the value of the node test. Is never NULL or
        //                          empty.
        //  - eNODE_TEST_ATTRIBUTE_NAME: the name of the selected attribute. Is
        //                               never NULL or empty.
        //  - eAXIS_NAMESPACE_PREFIX: the prefix of the namespace to select. Is
        //                            never NULL or empty.
        //  - eNODE_TEST_COMMENT: NULL.
        //  - eNODE_TEST_PROCESSING_INSTRUCTION: - NULL if there was nothing in
        //                                         the parenthesis.
        //                                       - The name of the selected
        //                                         processing instruction (empty
        //                                         strings will return NULL).
        //  - eNODE_TEST_TEXT: NULL.
        const char* m_pszNodeTestValue;

        // The length of m_pszNodeTestValue.
        // m_pszNodeTestValue[m_uNodeTestLength] points to the character ending
        // the string. It was changed for a '\0'.
        //
        // Ignore if m_pszNodeTestValue is NULL.
        unsigned int m_uNodeTestValueLength;

        // The namespace for the node-test:
        //  - eNODE_SET_FUNCTION_ID: NULL
        //  - eNODE_TEST_NAME_TEST: - the namespace URI of the namespace
        //                            declaring m_pszNodeTestValue.
        //                          - NULL if none was specified and there is no
        //                            default namespace.
        //  - eNODE_TEST_ATTRIBUTE_NAME: - the namespace URI of the namespace
        //                                 declaringm_pszNodeTestValue.
        //                               - NULL if none was specified.
        //  - eAXIS_NAMESPACE_PREFIX: NULL
        //  - eNODE_TEST_COMMENT: NULL.
        //  - eNODE_TEST_PROCESSING_INSTRUCTION: NULL.
        //  - eNODE_TEST_TEXT: NULL.
        const char* m_pszNamespace;

        // The predicates for this step, in order.
        CVector<SPredicate> m_vecstPredicate;

        // A pointer to the next step. We have ownership on this.
        SLocationStep* m_pNextLocationStep;

        SLocationStep()
        :   m_eNodeTestType(eNODE_TEST_NAME_TEST),
            m_pszNodeTestValue(NULL),
            m_uNodeTestValueLength(0),
            m_pszNamespace(NULL),
            m_pNextLocationStep(NULL)
        {}

        ~SLocationStep();
    };

    // This is needed to be able to compile with MSVC6 in which it is not
    // possible for a struct declared in a class to access the enumeration
    // values that are declared as private or protected in the same class.
    friend struct SLocationStep;

    // Used with GetPosValue.
    enum EPosValue
    {
        ePOS_AFTER,
        ePOS_BEFORE,
        ePOS_PREPEND,
        ePOS_NONE
    };

    // Tells an history step.
    enum EHistoryOperation
    {
        eOP_CREATE_ELEMENT,
        eOP_UPDATE_ELEMENT,
        eOP_DELETE_ELEMENT,
        eOP_OTHER
    };

    struct SHistoryStep
    {
        EHistoryOperation m_eOperation;

        // Points to the selected element, if one is selected. This pointer
        // MUST NEVER be casted back to a CXmlElement since it might be deleted.
        const void* m_pSelectedElement;

        SHistoryStep()
        :   m_eOperation(eOP_OTHER),
            m_pSelectedElement(NULL)
        {}
    };

    // This is needed to be able to compile with MSVC6 in which it is not
    // possible for a struct declared in a class to access the enumeration
    // values that are declared as private or protected in the same class.
    friend struct SHistoryStep;


private:

//-- Published Interface
public:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceXmlPatchOperations();

    // Summary:
    //  Destructor.
    virtual ~CSceXmlPatchOperations();


    //-- << Configuration >>

    // Summary:
    //  Sets the namespace of the extension of the base XML patch operations.
    mxt_result SetNamespace(IN const char* pszExtensionNamespace,
                            IN const char* pszPrefix);


    //-- << Patch operations management >>

    // Summary:
    //  Returns the accumulated patches and removes these patches from this
    //  element.
    mxt_result GetPatches(OUT IXmlDocument** ppPatchOperations);

    // Summary:
    //  Removes all the accumulated patches.
    mxt_result ResetPatches();

    // Summary:
    //  Applies XML patch operations on the given IXmlDocument.
    mxt_result ApplyXmlPatchOperations(IN const CXmlElement* pXmlPatchOperations,
                                       INOUT IXmlDocument* pXmlDocumentToUpdate);

    // Summary:
    //  Applies XML patch operations on the given IXmlDocument.
    static mxt_result ApplyXmlPatchOperations(IN const CXmlElement* pXmlPatchOperations,
                                              IN const char* pszDiffExtensionNamespace,
                                              INOUT IXmlDocument* pXmlDocumentToUpdate);


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


    //-- << Selector generation helpers >>

    // Summary:
    //  Creates a patch operation element identifying an attribute in an element
    //  selected by its index.
    CXmlElement* CreatePatchOpByIndexAttribute(IN const char* pszElementName,
                                               IN const CXmlElement& rSelectedElement,
                                               IN const CXmlElement::SNamespace* pstNamespace,
                                               IN const char* pszAttributeName);

    // Summary:
    //  Creates a patch operation element identifying a namespace in an element
    //  selected by its index.
    CXmlElement* CreatePatchOpByIndexNamespace(IN const char* pszElementName,
                                               IN const CXmlElement& rSelectedElement,
                                               IN const char* pszNsPrefix);

    // Summary:
    //  Creates a patch operation element identifying an element with its index.
    CXmlElement* CreatePatchOpByIndex(IN const char* pszElementName,
                                      IN const CXmlElement& rSelectedElement,
                                      IN EHistoryOperation eOperation);

    // Summary:
    //  Generates a path only from the id function and or position predicates.
    static mxt_result GeneratePositionPath(IN const CXmlElement& rTopElement,
                                           IN bool bTopElementIsLastStep,
                                           OUT CString& rstrPathDestination);

    // Summary:
    //  Generates a "@attr" or "@pf:attr" type string.
    static mxt_result GenerateAttributeNodeTest(IN const CXmlElement::SNamespace* pstNamespace,
                                                IN const char* pszAttributeName,
                                                INOUT CString& rstrDestination,
                                                INOUT CXmlElement& rPatchOperation);

    // Summary:
    //  Generates a "namespace::pref" type string.
    static void GenerateNamespaceNodeTest(IN const char* pszNsPrefix,
                                          INOUT CString& rstrDestination);


    //-- << Change application helpers >>

    // Summary:
    //  Apply the changes on given CXmlElement object.
    static mxt_result ApplyXmlPatchOperation(IN const CXmlElement& rOperationElement,
                                             IN EPatchOperation ePatchOperation,
                                             INOUT CXmlElement& rRootElement);

    // Summary:
    //  Selects a CXmlElement from the given steps, starting from the first
    //  step.
    static CXmlElement* GetElementFromSteps(IN CXmlElement& rRoot,
                                            IN const SLocationStep& rstFirstStep,
                                            OUT const SLocationStep*& rpstUnprocessedStep);

    // Summary:
    //  Selects a CXmlElement from the given steps.
    static CXmlElement* GetSelectedElement(IN CXmlElement& rCurrentRoot,
                                           IN const SLocationStep& rCurrentStep,
                                           OUT const SLocationStep*& rpstUnprocessedStep);

    // Summary:
    //  Returns the element with the given id.
    static CXmlElement* GetIdElement(IN CXmlElement& rElementToTest,
                                     IN const SLocationStep& rstIdStep);

    // Summary:
    //  Tells if the element matches the name test step.
    static bool IsMatchingNameTestStep(IN const CXmlElement& rElementToTest,
                                       IN const SLocationStep& rstNameTestStep,
                                       INOUT unsigned int& ruNumElementsToMatchBeforePosMatch);

    // Summary:
    //  Tells if an element matches the predicates.
    static bool IsMatchingPredicates(IN const CXmlElement& rElementToTest,
                                     IN const CVector<SPredicate>& rvecstPredicate,
                                     INOUT unsigned int& ruNumElementsToMatchBeforePosMatch);

    // Summary:
    //  Applies the given patch to the selected element.
    static mxt_result ApplyPatch(IN const CXmlElement& rOperationElement,
                                 IN EPatchOperation ePatchOperation,
                                 INOUT CXmlElement& rSelectedElement,
                                 IN const SLocationStep* pstUnprocessedStep);

    // Summary:
    //  Adds the content of rPatchOpsElement into rSelectedElement, at the
    // selected place.
    static mxt_result ApplyAddNode(IN const CXmlElement& rPatchOpsElement,
                                   INOUT CXmlElement& rSelectedElement);

    // Summary:
    //  Adds the content of rPatchOpsElement into an attribute in
    //  rSelectedElement.
    static mxt_result ApplyAddAttribute(IN const CXmlElement& rAddElement,
                                        IN const char* pszAttributeName,
                                        INOUT CXmlElement& rSelectedElement);

    // Summary:
    //  Applies an operation on an attribute.
    static mxt_result ApplyAttributeOperation(IN const CXmlElement& rOperationElement,
                                              IN EPatchOperation ePatchOperation,
                                              IN const SLocationStep& rstAttributeNodeTest,
                                              INOUT CXmlElement& rSelectedElement);

    // Summary:
    //  Applies an operation on a namespace.
    static mxt_result ApplyNamespaceOperation(IN const CXmlElement& rOperationElement,
                                              IN EPatchOperation ePatchOperation,
                                              IN const SLocationStep& rstNsAxis,
                                              INOUT CXmlElement& rSelectedElement);

    // Summary:
    //  Applies an operation on a text node.
    static mxt_result ApplyTextOperation(IN const CXmlElement& rOperationElement,
                                         IN EPatchOperation ePatchOperation,
                                         IN const SLocationStep& rstTextNodeTest,
                                         INOUT CXmlElement& rSelectedElement);

    // Summary:
    //  Merges two text node in the selected element.
    static void MergeTextNodes(INOUT CXmlElement& rSelectedElement,
                               IN const char* pszValue2,
                               IN bool bInvertValues);


    //-- << Parsing helpers >>

    // Summary:
    //  Parses a path in its location step parts.
    static mxt_result ParseLocationSteps(IN const CXmlElement& rPatchOpsElement,
                                         INOUT char*& rpszPath,
                                         OUT GO SLocationStep*& rpstLocationStep);

    // Summary:
    //   Parses the name test of a node test.
    static mxt_result ParseNameTestPart(IN const CXmlElement& rPatchOpsElement,
                                        INOUT char*& rpszPath,
                                        INOUT SLocationStep& rstLocationStep);

    // Summary:
    //  Parses the predicates of a node test.
    static mxt_result ParsePredicates(IN const CXmlElement& rPatchOpsElement,
                                      INOUT char*& rpszPath,
                                      INOUT SLocationStep& rstLocationStep);

    // Summary:
    //  Parses the namespace axis value.
    static mxt_result GetNamespaceAxisValue(IN const char* pszNamespaceAxis,
                                            INOUT char*& rpszNamespaceAxisValue,
                                            OUT SLocationStep& rstLocationStep);

    // Summary:
    //  Parses a supported node set function.
    static mxt_result GetNodeSetFunction(IN const char* pszNodeSetFunction,
                                         INOUT char*& rpszFunctionParameter,
                                         OUT SLocationStep& rstLocationStep);

    // Summary:
    //  Extracts the name and the value from a predicate.
    static mxt_result ExtractPredicateNameValue(IN const CXmlElement& rPatchOpsElement,
                                                IN bool bDefaultNamespaceIsApplicable,
                                                INOUT char*& rpszNameStart,
                                                OUT const char*& rpszNamespace,
                                                OUT char*& rpszName,
                                                OUT unsigned int& ruNameLength,
                                                OUT char*& rpszValue,
                                                OUT unsigned int& ruValueLength);

    // Summary:
    //  Tells if a character is a node test character.
    static bool IsNodeTestCharacter(IN char cChar);

    // Summary:
    //  Goes to the end character of a predicate.
    static void GoToPredicateEnd(INOUT char*& rpszCurrentChar);

    // Summary:
    //  Goes to the end character of a predicate.
    static void GoToPredicateEnd(INOUT char*& rpszCurrentChar,
                                 IN char cAdditionalEndChar);

    // Summary:
    //  Tells if a character is a predicate character.
    static bool IsPredicateCharacter(IN char cChar);

    // Summary:
    //  Compares two strings considering NULL and empty as equivalent.
    static bool CompareStringWithNullAsEmpty(IN const char* pszString1,
                                             IN const char* pszString2);

    // Summary:
    //  Tells if a node test type MUST be the last node test of a path.
    static bool IsFinalNodeTest(IN ENodeTestType eNodeTestType);

    // Summary:
    //  Gets the "pos" attribute value.
    static mxt_result GetPosValue(IN const CXmlElement& rPatchOpsElement,
                                  OUT EPosValue& rePosValue);


    //-- << Member helpers >>

    // Summary:
    //  Tries to reuse the last operation to contain the new operation.
    mxt_result ReuseLastOperation(IN EHistoryOperation eCurrentOperation,
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
    //  Creates the specified patch operation element with the specified 'sel'
    //  attribute value.
    CXmlElement* CreatePatchOperation(IN const char* pszElementName,
                                      IN const char* pszSelAttributeValue,
                                      IN EHistoryOperation eOperation,
                                      IN const CXmlElement* pSelectedElement);

    // Summary:
    //  Creates the specified patch operation element.
    CXmlElement* CreatePatchOperation(IN const char* pszElementName,
                                      IN EHistoryOperation eOperation,
                                      IN const CXmlElement* pSelectedElement);

    // Summary:
    //  Sets in a mode where it is known that an error was faced.
    void SetInErrorMode();

    // Summary:
    //  Deletes the last patch operation element.
    void DeleteLastPatchOperation(IN CXmlElement* pLastElement = NULL);

    // Summary:
    //  Gives a reference on the current XML patch operations root element.
    void GetXmlPatchOperationsRoot(OUT CXmlElement*& rpRoot);


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceXmlPatchOperations(IN const CSceXmlPatchOperations& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceXmlPatchOperations& operator=(IN const CSceXmlPatchOperations& rSrc);


//-- Hidden Data Members
protected:
private:

    // The IXmlDocument containing the accumulated patches.
    IXmlDocument* m_pXmlPatchOperations;

    // true if we failed to generate a patch operation. All patches are resetted
    // in this case.
    bool m_bPatchGenerationError;

    // The namespace in which the RFC 5261 patch operations are used.
    const char* m_pszExtensionNamespace;

    // The prefix to use when generating an XML document.
    const char* m_pszExtensionNsPrefix;

    // This is the history of operations. It keeps a pointer to the selected
    // element and the operation done. Is is additional data to
    // m_pXmlPatchOperations and is only used to reuse the previous patches when
    // a new modification happens.
    CList<SHistoryStep> m_lststOpHistory;

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  ApplyXmlPatchOperations
//==
//==============================================================================
//
//  Parameters:
//      pXmlPatchOperations:
//       The CXmlElement from which all children are patch operations.
//
//       MUST NOT be NULL.
//
//      pXmlDocumentToUpdate:
//       The IXmlDocument to update with the patch operations present in
//       pXmlPatchOperations.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - resS_OK: Success.
//      - resSW_WARNING: Some elements were unknown and were ignored or an
//                       operation was ignored or simplified due to a parser
//                       limitation.
//      - Failure: Failed to apply some patches. After a patch failed, other
//        patches were NOT applied.
//
//  Description:
//      Applies XML patch operations on the given IXmlDocument.
//
//      The expected extension namespace is the one that was set in
//      SetNamespace.
//
//  See Also:
//      SetNamespace.
//
//==============================================================================
inline mxt_result CSceXmlPatchOperations::ApplyXmlPatchOperations(IN const CXmlElement* pXmlPatchOperations,
                                                                  INOUT IXmlDocument* pXmlDocumentToUpdate)
{
    return ApplyXmlPatchOperations(pXmlPatchOperations,
                                   m_pszExtensionNamespace,
                                   INOUT pXmlDocumentToUpdate);
}


//==============================================================================
//==
//==  IsNodeTestCharacter
//==
//==============================================================================
//
//  Parameters:
//      cChar:
//       Character to test.
//
//  Returns:
//      - true: the character is a valid node test character (ie. not a node
//              test control character).
//      - false: the character is a node test control character, which means
//               that it cannot be present in a node test.
//
//  Description:
//      Tells if a character is a node test character.
//
//==============================================================================
inline bool CSceXmlPatchOperations::IsNodeTestCharacter(IN char cChar)
{
    return cChar != '\0' &&
           cChar != '[' &&
           cChar != '/';
}


//==============================================================================
//==
//==  GoToPredicateEnd
//==
//==============================================================================
//
//  Parameters:
//      rpszCurrentChar:
//       The pointer to the beginning of the predicate (just after the "["
//       character).
//
//       When this method ends, this pointer will point to the character ending
//       the predicate (no necessary valid).
//
//       MUST NOT be NULL. It is the responsibility of the caller to verify
//       this.
//
//  Description:
//      Goes to the end character of a predicate.
//
//      This method will increment rpszCurrentChar until '\0', '[', ']', '/' is
//      encountered.
//
//==============================================================================
inline void CSceXmlPatchOperations::GoToPredicateEnd(INOUT char*& rpszCurrentChar)
{
    while (IsPredicateCharacter(*rpszCurrentChar))
    {
        rpszCurrentChar++;
    }
}


//==============================================================================
//==
//==  IsPredicateCharacter
//==
//==============================================================================
//
//  Parameters:
//      cChar:
//       Character to test.
//
//  Returns:
//      - true: the character is a valid predicate character (ie. not a
//              predicate control character).
//      - false: the character cannot be present in a predicate.
//
//  Description:
//      Tells if a character is a predicate character.
//
//==============================================================================
inline bool CSceXmlPatchOperations::IsPredicateCharacter(IN char cChar)
{
    return cChar != '\0' &&
           cChar != '[' &&
           cChar != ']' &&
           cChar != '/';
}


//==============================================================================
//==
//==   CompareStringWithNullAsEmpty
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
inline bool CSceXmlPatchOperations::CompareStringWithNullAsEmpty(IN const char* pszString1,
                                                                 IN const char* pszString2)
{
    return strcmp(MX_MAKE_NULL_EMPTY_STRING(pszString1),
                  MX_MAKE_NULL_EMPTY_STRING(pszString2)) == 0;
}


//==============================================================================
//==
//==   IsFinalNodeTest
//==
//==============================================================================
//
//  Parameters:
//      eNodeTestType: The node test type to verify.
//
//  Returns:
//      true if the node test type MUST be at the end of a path.
//
//  Description:
//      Tells if a node test type MUST be the last node test of a path.
//
//==============================================================================
inline bool CSceXmlPatchOperations::IsFinalNodeTest(IN ENodeTestType eNodeTestType)
{
    return eNodeTestType == eNODE_TEST_ATTRIBUTE_NAME ||
            eNodeTestType == eAXIS_NAMESPACE_PREFIX ||
            eNodeTestType == eNODE_TEST_TEXT ||
            eNodeTestType == eNODE_TEST_COMMENT ||
            eNodeTestType == eNODE_TEST_PROCESSING_INSTRUCTION;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCEXMLPATCHOPERATIONS_H

//M5T_INTERNAL_USE_END
