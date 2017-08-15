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
#ifndef MXG_CSCEFILTERELEMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEFILTERELEMENT_H
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

#ifndef MXG_CXMLELEMENT_H
#include "Xml/CXmlElement.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceFilterElement
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Description:
//  As per RFC 4661, the CSceFilterElement represents a filter, which is used to
//  specify the content of an individual filter.
//
//  The CSceFilterElement is a simple wrapper that can be created from a
//  CXmlElement returned by the CSceFilterDocument. It makes it easy to access
//  all  standardized elements of the filter element.
//
//  The CSceFilterElement is a light weigth class, it can be created on the
//  stack (as a local variable) without any problem. It however shares a pointer
//  to a CXmlElement that is owned by a CSceFilterDocument. Deleting the
//  associated CSceFilterDocument, calling Cleanup on it or deleting parent XML
//  elements of this filter element will render this object invalid.
//
// Location:
//   SceXml/CSceFilterElement.h
//
// See Also:
//   CSceFilterDocument
//
//==============================================================================
class CSceFilterElement
{
//-- Friend Declarations
//------------------------
    // MXI_DESIGN_NOTES (2008-12-16): CSceFilterElement should eventually be
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
    CSceFilterElement(IN CXmlElement& rFilterElement);

    // Summary:
    //  Destructor.
    virtual ~CSceFilterElement();

    //-- << id Attribute - RFC 4661>>
    // Nothing special is done for this attribute. It is automatically created
    // by the CSceFilterDocument when the filter element is created. It is a
    // unique string used to identify this element.

    //-- << uri Attribute - RFC 4661>>
    // Since a subscription to an event package may be addressed to an event
    // list, the 'uri' attribute allows the subscriber to define a filter
    // specific to an individual resource within that list. That resource may be
    // another list. If the <filter> does not contain the 'uri' attribute, the
    // filter applies to the resource identified in the subscription request.

    // Summary:
    //  Gets the Uri to which this filter applies.
    void GetUri(OUT const char*& rpszUri);

    // Summary:
    //  Gets the Uri to which this filter applies.
    void GetUri(OUT GO IUri*& rpUri);

    // Summary:
    //  Sets the Uri to which this filter applies.
    mxt_result SetUri(IN const char* pszUri);

    // Summary:
    //  Sets the Uri to which this filter applies.
    mxt_result SetUri(IN GO IUri* pUri);


    //-- << domain Attribute - RFC 4661>>
    // The 'domain' attribute carries a domain. In this case, the filter applies
    // to resources whose URI has a domain part matching that domain.

    // Summary:
    //  Gets the domain to which this filter applies.
    void GetDomain(OUT const char*& rpszDomain);

    // Summary:
    //  Sets the domain to which this filter applies.
    mxt_result SetDomain(IN const char* pszDomain);


    //-- << remove Attribute - RFC 4661>>
    // The <filter> MAY have a 'remove' attribute that together with the
    // 'id' attribute indicates the existing filter to be removed.

    // Summary:
    //  Gets whether or not this filter should be removed.
    void GetRemove(OUT bool& rbRemove);

    // Summary:
    //  Sets whether or not this filter should be removed.
    mxt_result SetRemove(IN bool bRemove);


    //-- << enable Attribute - RFC 4661>>
    // The <filter> MAY have an 'enabled' attribute that indicates whether a
    // filter is enabled or disabled.

    // Summary:
    //  Gets whether or not this filter is enabled.
    void GetEnable(OUT bool& rbEnable);

    // Summary:
    //  Sets whether or not this filter is enabled.
    mxt_result SetEnable(IN bool bEnable);



    //-- << what element - RFC 4661>>

    // Description:
    //   This enumeration defines the possible value of the type attribute
    //   present in the <include> or <exclude> elements.
    enum EWhatType
    {
        // Description:
        //  The "xpath" value is used when the <include> or <exclude> element
        //  contains a value that selects an element or an attribute.
        eTYPE_XPATH,

        // Description:
        //  The "namespace" value is used when the <include> or <exclude>
        //  element contains a value of a namespace.
        eTYPE_NAMESPACE
    };


    // Summary:
    //  Gets all include filter rules information found under the <what>
    //  element.
    mxt_result GetWhatIncludes(OUT CVector<CXmlElement*>& rvpWhat);

    // Summary:
    //  Gets the content to be delivered of an <include> element.
    mxt_result GetWhatInclude(IN CXmlElement* pWhatIncludeElement,
                              OUT const char*& rpszWhatValue,
                              OUT EWhatType& reType) const;

    // Summary:
    //  Adds content to be delivered information to the filter.
    CXmlElement* AddWhatInclude(IN const char* pszWhatValue,
                                IN EWhatType eType = eTYPE_XPATH);

    // Summary:
    //  Updates the content to be delivered of a filter element.
    mxt_result UpdateWhatInclude(IN CXmlElement* pNetworkElement,
                                 IN const char* pszWhatValue,
                                 IN EWhatType eType = eTYPE_XPATH);

    // Summary:
    //  Gets all exclude filter rules information found under the <what>
    //  element.
    mxt_result GetWhatExcludes(OUT CVector<CXmlElement*>& rvecpWhat);

    // Summary:
    //  Gets the content to be delivered of an <exclude> element.
    mxt_result GetWhatExclude(IN CXmlElement* pWhatExcludeElement,
                              OUT const char*& rpszWhatValue,
                              OUT EWhatType& reType) const;

    // Summary:
    //  Adds content to be delivered information to the filter.
    CXmlElement* AddWhatExclude(IN const char* pszWhatValue,
                                IN EWhatType eType = eTYPE_XPATH);

    // Summary:
    //  Updates the content to be delivered of a filter element.
    mxt_result UpdateWhatExclude(IN CXmlElement* pWhatExcludeElement,
                                 IN const char* pszWhatValue,
                                 IN EWhatType eType = eTYPE_XPATH);

    //-- << trigger element - RFC 4661>>

    // Description:
    //   This enumeration defines the possible trigger condition type.
    enum ETriggerType
    {
        // Description:
        //   This is the <changed> element. It means that the value of the
        //   element/attribute must change in order to activate the trigger.
        eTYPE_CHANGED,

        // Description:
        //   This is the <added> element. It means that the element/attribute
        //   must be added in order to activate the trigger.
        eTYPE_ADDED,

        // Description:
        //   This is the <removed> element. It means that the element/attribute
        //   must be removed in order to activate the trigger.
        eTYPE_REMOVED
    };

    // Summary:
    //  Gets all trigger information found under the <filter> element.
    mxt_result GetTriggers(OUT CVector<CXmlElement*>& rvecpTriggers);

    // Summary:
    //  Adds a trigger to the filter and automatically assigns a trigger
    //  condition to it.
    CXmlElement* AddTrigger(IN ETriggerType eType,
                            IN const char* pszValue,
                            IN const char* pszFrom = NULL,
                            IN const char* pszTo = NULL,
                            IN int* pnBy = NULL);

    // Summary:
    //  Gets all condition information found under the <trigger> element.
    mxt_result GetTriggerConditions(IN CXmlElement* pTriggerElement,
                                    OUT CVector<CXmlElement*>& rvecpTriggerConditions);

    // Summary:
    //  Gets the condition informations of the specified trigger condition
    //  element.
    mxt_result GetTriggerCondition(IN CXmlElement* pTriggerConditionElement,
                                   OUT ETriggerType& reType,
                                   OUT const char*& rpszValue,
                                   OUT const char** ppszFrom = NULL,
                                   OUT const char** ppszTo = NULL,
                                   OUT int* pnBy = NULL) const;

    // Summary:
    //  Adds a trigger condition to the trigger.
    CXmlElement* AddTriggerCondition(IN CXmlElement* pTriggerElement,
                                     IN ETriggerType eType,
                                     IN const char* pszValue,
                                     IN const char* pszFrom = NULL,
                                     IN const char* pszTo = NULL,
                                     IN int* pnBy = NULL);

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:
    //-- << Initialization mechanism >>
    // Summary:
    //  Initializes the CSceFilterElement.
    static mxt_result InitializeCSceFilterElement();

    // Summary:
    //  Finalizes the CSceFilterElement.
    static void FinalizeCSceFilterElement();

private:

    //-- << Helper methods>>

    // Summary:
    //  Gets all child element of the specified name found under the <what>
    //  element.
    mxt_result GetWhatChilds(IN const char* pszName,
                             OUT CVector<CXmlElement*>& rvecpWhat);

    // Summary:
    //  Gets the content to be delivered of a <what> child element.
    mxt_result GetWhatChild(IN const char* pszName,
                            IN CXmlElement* pWhatChildElement,
                            OUT const char*& rpszValue,
                            OUT EWhatType& reType) const;

    // Summary:
    //  Adds a child to the <what> element.
    CXmlElement* AddWhatChild(IN const char* pszName,
                              IN const char* pszValue,
                              IN EWhatType eType);

    // Summary:
    //  Updates the content to be delivered of a <what> child element.
    mxt_result UpdateWhatChild(IN const char* pszName,
                               IN CXmlElement* pWhatChildElement,
                               IN const char* pszValue,
                               IN EWhatType eType);

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSceFilterElement(IN const CSceFilterElement& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSceFilterElement& operator=(IN const CSceFilterElement& rSrc);

//-- Hidden Data Members
//-----------------------
protected:
private:

    // The internal filter XML element.
    CXmlElement* m_pFilterXmlElement;

    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecFilterSequence;
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecWhatSequence;
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecTriggerSequence;

    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetUri
//==
//==============================================================================
//
//  Parameters:
//    rpszUri:
//      Reference to a const char pointer, which is set to point to the URI of
//      the resource to which the filter applies. Can be NULL when no URI is
//      specified or when a domain is specified within the filter.
//
//  Description:
//      Gets the Uri to which this filter applies.
//
//  See Also:
//      SetUri
//
//==============================================================================
inline void CSceFilterElement::GetUri(OUT const char*& rpszUri)
{
    m_pFilterXmlElement->GetAttribute(NULL, g_pszXMLFILTER_URI, rpszUri);
}

//==============================================================================
//==
//==  GetDomain
//==
//==============================================================================
//
//  Parameters:
//    rpszDomain:
//      Reference to a const char pointer, which is set to point to the
//      domain of the resources to which the filter applies. Can be NULL when no
//      domain is specified or when a URI is specified within the filter.
//
//  Description:
//      Gets the domain to which this filter applies.
//
//  See Also:
//      SetDomain
//
//==============================================================================
inline void CSceFilterElement::GetDomain(OUT const char*& rpszDomain)
{
    m_pFilterXmlElement->GetAttribute(NULL, g_pszXMLFILTER_DOMAIN, rpszDomain);
}

//==============================================================================
//==
//==  GetWhatIncludes
//==
//==============================================================================
//
//  Parameters:
//    rvpWhat:
//      Vector provided by the application, which is filled with all <include>
//      elements of the filter's <what> element.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all include filter rules information found under the <what>
//      element. As defined in RFC 4661:
//
//<CODE>
//   The <what> element is used to specify the content to be delivered to the
//   user. It does not specify the exact content but the rules that are used to
//   construct that information.
//</CODE>
//
//<CODE>
//   The <include> element is used to select the content to be delivered. Its
//   value can identify an XML element, an attribute, or a namespace of an XML
//   document to be filtered.  This is indicated using the 'type' attribute.
//</CODE>
//
//      More information about each <inlude> element can be retreived using
//      GetWhatInclude
//
//  See Also:
//      GetWhatInclude, AddWhatInclude, UpdateWhatInclude
//
//==============================================================================
inline mxt_result CSceFilterElement::GetWhatIncludes(OUT CVector<CXmlElement*>& rvpWhat)
{
    return GetWhatChilds(g_pszXMLFILTER_INCLUDE, rvpWhat);
}

//==============================================================================
//==
//==  GetWhatInclude
//==
//==============================================================================
//
//  Parameters:
//    pWhatIncludeElement:
//      Pointer to the <include> element from which to get its content to be
//      delivered. Cannot be NULL. This is usually an element retreived after
//      calling GetWhatIncludes.
//
//    rpszValue:
//      Reference to a const char pointer, which is set to point to the content
//      to be delivered.
//
//    reType:
//      Reference to a EWhatType, which is set to the type of the content to be
//      delivered.
//
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the filter
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets the content to be delivered of an <include> element.
//
//  See Also:
//      GetWhatIncludes, AddWhatInclude
//
//==============================================================================
inline mxt_result CSceFilterElement::GetWhatInclude(IN CXmlElement* pWhatIncludeElement,
                                                    OUT const char*& rpszValue,
                                                    OUT EWhatType& reType) const
{
    return GetWhatChild(g_pszXMLFILTER_INCLUDE,
                        pWhatIncludeElement,
                        rpszValue,
                        reType);
}

//==============================================================================
//==
//==  AddWhatInclude
//==
//==============================================================================
//
//  Parameters:
//    pszValue:
//      The content to be delivered. Cannot be NULL.
//
//    eType:
//      The type of the content to be delivered. The Default value is
//      eTYPE_XPATH.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information. Will
//      return NULL if pszWhatValue or pszType is NULL.
//
//  Description:
//      Adds content to be delivered information to the filter.
//
//  See Also:
//      GetWhatInclude, UpdateWhatInclude
//
//==============================================================================
inline CXmlElement* CSceFilterElement::AddWhatInclude(IN const char* pszValue,
                                                      IN EWhatType eType)
{
    return AddWhatChild(g_pszXMLFILTER_INCLUDE, pszValue, eType);
}

//==============================================================================
//==
//==  UpdateWhatInclude
//==
//==============================================================================
//
//  Parameters:
//    pWhatIncludeElement:
//      Pointer to the <include> element from which to get its content to be
//      delivered. Cannot be NULL. This is usually an element retreived after
//      calling GetWhatIncludes.
//
//    pszValue:
//      The new content to be delivered. Cannot be NULL.
//
//    eType:
//      The type of the new content to be delivered.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Updates the content to be delivered of a <what> filter element.
//
//  See Also:
//      AddWhatInclude
//
//==============================================================================
inline mxt_result CSceFilterElement::UpdateWhatInclude(IN CXmlElement* pWhatIncludeElement,
                                                       IN const char* pszValue,
                                                       IN EWhatType eType)
{
    return UpdateWhatChild(g_pszXMLFILTER_INCLUDE,
                           pWhatIncludeElement,
                           pszValue,
                           eType);
}

//==============================================================================
//==
//==  GetWhatExcludes
//==
//==============================================================================
//
//  Parameters:
//    rvecpWhat:
//      Vector provided by the application, which is filled with all <exclude>
//      elements of the filter's <what> element.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all exclude filter rules information found under the <what>
//      element. As defined in RFC 4661:
//
//<CODE>
//   The <what> element is used to specify the content to be delivered to the
//   user. It does not specify the exact content but the rules that are used to
//   construct that information.
//</CODE>
//
//<CODE>
//   The <exclude> element is used to define exceptions to the set of XML
//   elements and/or attributes selected by the <include> elements. Thus, XML
//   elements (including their lower-level "child" elements) and/or attributes
//   defined by the <exclude> element are not delivered. This is most useful
//   when an <include> element identifies a namespace.
//</CODE>
//
//      More information about each <exclude> element can be retreived using
//      GetWhatInclude
//
//  See Also:
//      GetWhatExclude, AddWhatExclude, UpdateWhatExclude
//
//==============================================================================
inline mxt_result CSceFilterElement::GetWhatExcludes(OUT CVector<CXmlElement*>& rvecpWhat)
{
    return GetWhatChilds(g_pszXMLFILTER_EXCLUDE, rvecpWhat);
}

//==============================================================================
//==
//==  GetWhatExclude
//==
//==============================================================================
//
//  Parameters:
//    pWhatExcludeElement:
//      Pointer to the <exclude> element from which to get the exception to the
//      content to be delivered. Cannot be NULL. This is usually an element
//      retreived after calling GetWhatExcludes.
//
//    rpszValue:
//      Reference to a const char pointer, which is set to point to the
//      exception to the content to be delivered.
//
//    reType:
//      Reference to a EWhatType pointer, which is set to the type of the
//      exception to the content to be delivered.
//
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the filter
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets the exception to the content to be delivered of an <exclude>
//      element.
//
//  See Also:
//      GetWhatExcludes, AddWhatExclude
//
//==============================================================================
inline mxt_result CSceFilterElement::GetWhatExclude(IN CXmlElement* pWhatExcludeElement,
                                                    OUT const char*& rpszValue,
                                                    OUT EWhatType& reType) const
{
    return GetWhatChild(g_pszXMLFILTER_EXCLUDE,
                        pWhatExcludeElement,
                        rpszValue,
                        reType);
}

//==============================================================================
//==
//==  AddWhatExclude
//==
//==============================================================================
//
//  Parameters:
//    pszValue:
//      The exception to the content to be delivered. Cannot be NULL.
//
//    eType:
//      The type of the exception to the content to be delivered. The Default
//      value is eTYPE_XPATH.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information. Will
//      return NULL if pszWhatValue is NULL.
//
//  Description:
//      Adds exception to the content to be delivered information of the filter.
//
//  See Also:
//      GetWhatExclude, UpdateWhatExclude
//
//==============================================================================
inline CXmlElement* CSceFilterElement::AddWhatExclude(IN const char* pszValue,
                                                      IN EWhatType eType)
{
    return AddWhatChild(g_pszXMLFILTER_EXCLUDE, pszValue, eType);
}

//==============================================================================
//==
//==  UpdateWhatExclude
//==
//==============================================================================
//
//  Parameters:
//    pWhatExcludeElement:
//      Pointer to the <exclude> element from which to get its exception to the
//      content to be delivered. Cannot be NULL. This is usually an element
//      retreived after calling GetExcludes.
//
//    rpszValue:
//      The new exception to the content to be delivered. Cannot be NULL.
//
//    eType:
//      The type of the exception to the new content to be delivered.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Updates the exception to the content to be delivered of a <what> filter
//      element.
//
//  See Also:
//      AddWhatInclude
//
//==============================================================================
inline mxt_result CSceFilterElement::UpdateWhatExclude(IN CXmlElement* pWhatExcludeElement,
                                                       IN const char* pszValue,
                                                       IN EWhatType eType)
{
    return UpdateWhatChild(g_pszXMLFILTER_EXCLUDE,
                           pWhatExcludeElement,
                           pszValue,
                           eType);
}

//==============================================================================
//==
//==  GetTriggers
//==
//==============================================================================
//
//  Parameters:
//    rvecpTriggers:
//      Vector provided by the application, which is filled with all <trigger>
//      elements of the filter element.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all trigger information found under the <filter> element. As
//      defined in RFC 4661:
//
//<CODE>
//   The <trigger> element is used to identify the package-specific
//   changes that a resource has to encounter before the content is
//   delivered to the subscriber.
//</CODE>
//
//      More information about each <trigger> element can be retreived using
//      GetTrigger.
//
//  See Also:
//      GetTriggerConditions, AddTrigger
//
//==============================================================================
inline mxt_result CSceFilterElement::GetTriggers(OUT CVector<CXmlElement*>& rvecpTriggers)
{
    return CSceFilterDocument::GetElementFrom(m_pFilterXmlElement,
                                              g_pszXMLFILTER_NAMESPACE_FILTER,
                                              g_pszXMLFILTER_TRIGGER,
                                              OUT rvecpTriggers);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCEFILTERELEMENT_H

