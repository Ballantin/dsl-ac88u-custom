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
#ifndef MXG_IXMLPATCHMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXMLPATCHMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CXMLELEMENT_H
#include "Xml/CXmlElement.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class IXmlDocument;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IXmlPatchMgr
//========================================
//<GROUP XML_CLASSES>
//
// Description:
//  This interface defines the detailed modification events that can be
//  reported by an IXmlDocument. These events are reported each time something
//  in the IXmlDocument is changed.
//
//  The reported events can be used to track the actual changes done to the
//  IXmlDocument. This can be useful when partial changes done to an XML
//  document need to be provided or sent to an entity external to the XML
//  document.
//
//  This manager can be configured by using the SetPatchManager API of
//  IXmlDocument.
//  
// Location:
//   Xml/IXmlPatchMgr.h
//
// See Also:
//   IXmlDocument::SetPatchManager
//
//==============================================================================
class IXmlPatchMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    
    //==========================================================================
    //==
    //==  EvNewElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a new element was created in the XML document.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will 
    //      never be NULL.
    //
    //    pNewElementAfterInsertion:
    //      Pointer to the newly inserted XML element. It will never be NULL.
    //
    //  Description:
    //      Reports that a new element was created in the XML document.
    //      
    //      This event is reported after the XML element is inserted into the
    //      document.
    //
    //      Only this event is reported if a new namespace is being declared and
    //      assigned to the new element at the same time it is being created.
    //
    //==========================================================================
    virtual void EvNewElement(IN IXmlDocument* pDocument,
                              IN const CXmlElement* pNewElementAfterInsertion) = 0;
    
    //==========================================================================
    //==
    //==  EvDeletedElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that an element was deleted from the XML document.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will
    //      never be NULL.
    //
    //    pDeletedElement:
    //      Pointer to the element that was deleted from the document. It will
    //      never be NULL. This parameter still points to a valid XML element,
    //      however this element is no longer attached to the document and will
    //      be deleted when this event returns. Only the attributes, name,
    //      namespace, and value of this element should be accessed while the
    //      element is in this state.
    //
    //    pParentElement:
    //      Pointer to the parent of the deleted element. It can be NULL if the
    //      deleted element was the root element. Note that pDeletedElement is
    //      no longer a child element of pParentElement and cannot be accessed
    //      through this parameter.
    //
    //    uIndex:
    //      Zero-based index of the deleted element. This corresponds to the
    //      index the deleted element had as a child of pParentElement.
    //
    //  Description:
    //      Reports that an element was deleted from the XML document.
    //
    //      This event is reported before the element is deleted, but after it
    //      is removed from the document. This allows finding out the relative
    //      position of the deleted element with regards to its parents
    //      and siblings.
    //
    //==========================================================================
    virtual void EvDeletedElement(IN IXmlDocument* pDocument,
                                  IN const CXmlElement* pDeletedElement,
                                  IN const CXmlElement* pParentElement,
                                  IN unsigned int uIndex) = 0;
    
    //==========================================================================
    //==
    //==  EvChangedElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a whole element was changed.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will
    //      never be NULL.
    //
    //    pUpdatedElement:
    //      Pointer to the updated element. It will never be NULL.
    //
    //  Description:
    //      Reports that an element, including its name, namespace and
    //      attributes have changed.
    //
    //      This event is reported after the element pointed by pUpdatedElement
    //      is changed. As such, the previous information held by this element
    //      is lost and cannot be used to identify this element. 
    //
    //==========================================================================
    virtual void EvChangedElement(IN IXmlDocument* pDocument,
                                  IN const CXmlElement* pUpdatedElement) = 0;


    //==========================================================================
    //==
    //==  EvChangedElementName
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the name of the element has changed.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will
    //      never be NULL.
    //
    //    pUpdatedElement:
    //      Pointer to the updated element. It will never be NULL.
    //
    //  Description:
    //      Reports that the name of the element has changed.
    //
    //      This event is reported whenever the name or the namespace assigned
    //      to an element is changed. 
    //
    //      This event is reported after the element pointed by pUpdatedElement
    //      is updated.
    //
    //==========================================================================
    virtual void EvChangedElementName(IN IXmlDocument* pDocument,
                                      IN const CXmlElement* pUpdatedElement) = 0;

    //==========================================================================
    //==
    //==  EvChangedElementValue
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the value of the element has changed.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will
    //      never be NULL.
    //
    //    pUpdatedElement:
    //      Pointer to the updated element. It will never be NULL.
    //
    //  Description:
    //      Reports that the value of the element is changing.
    //
    //      This event is reported after the element pointed by pUpdatedElement
    //      is updated.
    //
    //==========================================================================
    virtual void EvChangedElementValue(IN IXmlDocument* pDocument,
                                       IN const CXmlElement* pUpdatedElement) = 0;


    //==========================================================================
    //==
    //==  EvNewAttribute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a new attribute was assigned to an element.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will
    //      never be NULL.
    //
    //    pUpdatedElement:
    //      Pointer to the updated element. It will never be NULL.
    //
    //    pstNamespace:
    //      Pointer to the namespace information of the attribute. It can be NULL
    //      if no namespace is associated with the attribute.
    //
    //    pszAttributeName:
    //      Pointer to the name of the new attribute.
    //
    //    pszAttributeValue:
    //      Pointer to the value of the new attribute.
    //
    //  Description:
    //      Reports that a new attribute was assigned to an element.
    //
    //      This event is reported after the element pointed by pUpdatedElement
    //      is updated.
    //
    //==========================================================================
    virtual void EvNewAttribute(IN IXmlDocument* pDocument,
                                IN const CXmlElement* pUpdatedElement,
                                IN const CXmlElement::SNamespace* pstNamespace,
                                IN const char* pszAttributeName,
                                IN const char* pszAttributeValue) = 0;

    //==========================================================================
    //==
    //==  EvDeletedAttribute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that an attribute was deleted.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will
    //      never be NULL.
    //
    //    pUpdatedElement:
    //      Pointer to the updated element. It will never be NULL.
    //
    //    pstNamespace:
    //      Pointer to the namespace information of the attribute. It can be NULL
    //      if no namespace is associated with the attribute.
    //
    //    pszAttributeName:
    //      Pointer to the name of the attribute that was deleted.
    //
    //  Description:
    //      Reports that an attribute was deleted.
    //      
    //      This event is reported after the attribute is removed from
    //      pUpdatedElement.
    //
    //==========================================================================
    virtual void EvDeletedAttribute(IN IXmlDocument* pDocument,
                                    IN const CXmlElement* pUpdatedElement,
                                    IN const CXmlElement::SNamespace* pstNamespace,
                                    IN const char* pszAttributeName) = 0;

    //==========================================================================
    //==
    //==  EvChangedAttribute
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the value assigned to an attribute was changed.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will
    //      never be NULL.
    //
    //    pUpdatedElement:
    //      Pointer to the updated element. It will never be NULL.
    //
    //    pstNamespace:
    //      Pointer to the namespace information of the attribute. It can be NULL
    //      if no namespace is associated with the attribute.
    //
    //    pszAttributeName:
    //      Pointer to the name of the attribute that was changed. It will never
    //      be NULL.
    //
    //    pszAttributeValue:
    //      Pointer to the new value taken by the attribute. It can be NULL if
    //      the attribute is set with no value.
    //
    //  Description:
    //      Reports that the value assigned to an attribute was changed.
    //
    //      This event is reported after the element pointed by pUpdatedElement
    //      is updated.
    //
    //==========================================================================
    virtual void EvChangedAttribute(IN IXmlDocument* pDocument,
                                    IN const CXmlElement* pUpdatedElement,
                                    IN const CXmlElement::SNamespace* pstNamespace,
                                    IN const char* pszAttributeName,
                                    IN const char* pszAttributeValue) = 0;

    //==========================================================================
    //==
    //==  EvNewNamespace
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a new namespace was declared.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will
    //      never be NULL.
    //
    //    pUpdatedElement:
    //      Pointer to the updated element. It will never be NULL. This is the
    //      element where the namespace was declared.
    //      
    //    pstNewNamespace:
    //      Pointer to a namespace structure which identifies the information
    //      about the namespace that was declared.
    //
    //  Description:
    //      Reports that a new namespace was declared.
    //
    //      This event is reported after the element pointed by pUpdatedElement
    //      is updated.
    //
    //==========================================================================
    virtual void EvNewNamespace(IN IXmlDocument* pDocument,
                                IN CXmlElement* pUpdatedElement,
                                IN CXmlElement::SNamespace* pstNewNamespace) = 0;

    //==========================================================================
    //==
    //==  EvChangedNamespace
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a namespace was changed.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will
    //      never be NULL.
    //
    //    pUpdatedElement:
    //      Pointer to the updated element. It will never be NULL. This is the
    //      element where the namespace was declared.
    //      
    //    pstNewNamespace:
    //      Pointer to a namespace structure which identifies the information
    //      about the namespace that was changed.
    //
    //  Description:
    //      Reports that a namespace was changed.
    //
    //      This event is reported after the element pointed by pUpdatedElement
    //      is updated.
    //
    //==========================================================================
    virtual void EvChangedNamespace(IN IXmlDocument* pDocument,
                                    IN CXmlElement* pUpdatedElement,
                                    IN CXmlElement::SNamespace* pstChangedNamespace) = 0;

    //==========================================================================
    //==
    //==  EvDeletedNamespace
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a namespace was deleted.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the XML document where the change took place. It will
    //      never be NULL.
    //
    //    pUpdatedElement:
    //      Pointer to the updated element. It will never be NULL. This is the
    //      element where the namespace was declared.
    //      
    //    pstNewNamespace:
    //      Pointer to a namespace structure which identifies the information
    //      about the namespace that was changed.
    //
    //  Description:
    //      Reports that a namespace was deleted.
    //
    //      This event is reported right before the namespace is deleted but
    //      after it is removed from the declared namespaces.
    //
    //==========================================================================
    virtual void EvDeletedNamespace(IN IXmlDocument* pDocument,
                                    IN CXmlElement* pUpdatedElement,
                                    IN CXmlElement::SNamespace* pstDeletedNamespace) = 0;



//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IXmlPatchMgr() {};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IXmlPatchMgr() {};


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IXmlPatchMgr(IN const IXmlPatchMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IXmlPatchMgr& operator=(IN const IXmlPatchMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IXMLPATCHMGR_H

