//==============================================================================
//==============================================================================
//
//          Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_IFILTERDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IFILTERDOCUMENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
//-------------------------
class CBlob;
class CSipMessageBody;
class CTime;
class CXmlElement;
class IUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
MX_DECLARE_ECOM_IID(ISceFilterDocument);

//M5T_INTERNAL_USE_END

//==============================================================================
//== Class: ISceFilterDocument
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Summary:
//  This interafce represents a Filter document.
//
// Description:
//  The ISceFilterDocument interface defines the API needed to create and
//  manipulate a filter XML document, as defined in RFC 4661. 
//
//  This class keeps the filter information encoded in XML. It implements
//  the \<filter-set\> element of RFC 4661 and supports the
//  "application/simple-filter+xml" MIME format. The CSceFilterElement
//  helper interface can be used to manipulate filters and the \<filter\>
//  element included by this document.
//
//  The filter data format is encoded in XML and is meant to be quite
//  extensible. The ISceFilterDocument is used as a wrapper over a filter XML
//  document to make it easy for applications to set and access elements within
//  this XML tree. Users of this class can always access the XML tree directly
//  by using QueryIf, while most of the methods of the filter document
//  gives direct access to the XML elements within this tree.
//
//#Filter Overview#
//
//  The filter document is composed of zero or one ns-bindings, which contains
//  one or more ns-binding. The filter document is also composed of one or more 
//  filter. An application can create a ns-binding by calling AddNsBinding or
//  create a filter by calling AddFilter. It then can configure the filter
//  using a ISceFilterElement instance.
//
//  The filter document is generally used by two different entities:
//    - It is used by a subscriber to define how and which information should be
//      filtered by the notifier. It can also be used to specify when new
//      information should be sent by the notifier.
//    - When a notifier receives filtering information it should use this
//      information to filter how and what is sent to the subscriber. Honouring
//      those filters is however at the discretion of the notifier.
//
//  As specified in RFC 4660, when information changes on the server, it will
//  look at the filter document and based on the contents of a filter, the
//  following processing occurs:
//
// <CODE>
//   -  A filter with only a <what> element will result in sending the
//      requested resource state information in that <what> element
//      whenever there is a change in the resource state.
//
//   -  A filter with only a <trigger> element will result in sending all
//      resource state information whenever there is a change in the
//      resource state that matches the triggers.
//
//   -  A filter with <what> and <trigger> elements will result in sending
//      the requested resource state information in that <what> element
//      whenever there is a change in the resource state that matches the
//      triggers.
// </CODE>
//
//  You can add multiple <trigger> to the same <filter>, these triggers are
//  "ORed", which means that either of the trigger must match in order to send
//  resource change information. On the other hand, conditions within a trigger
//  are "ANDed" which means that all condition must be satisfied for a trigger
//  to match.
//
// <CODE>
//   When a filter is disabled (by setting the 'enabled' attribute to "false"),
//   it means the same thing as the absence of that filter and state changes are
//   reported by issuing a notification to the subscriber
//   (assuming there are no other filters).
// </CODE>
//      
// #Extensibility Mechanism# 
//  Using QueryIf, applications can have access to the IXmlDocument, in
//  DOM format. Since the filter document gets and stores all of its information
//  directly into the XML document, applications are free to further customize
//  the filtering information and add their own extensions by using the
//  IXmlDocument.
//
// Location:
//   SceXml/ISceFilterDocument.h
//
// See Also:
//   CSceFilterElement
//
//==============================================================================
class ISceFilterDocument : public IEComUnknown
{
//-- Friend Declarations.
//------------------------

//-- New types within class namespace.
//-------------------------------------
public:
protected:
private:

//-- Published Interface.
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceFilterDocument);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetPackage
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the package to which the filter criteria is applied.
    //
    //  Parameters:
    //    pszPackage:
    //      The package to which the filter criteria is applied. Cannot be NULL.
    //
    //  Returns:
    //      resS_OK upon success an error otherwise.
    //
    //  Description:
    //      Sets the package to which the filter criteria is applied.
    //
    //==========================================================================
    virtual mxt_result SetPackage(IN const char* pszPackage) = 0;

    //==========================================================================
    //==
    //==  GetPackage
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the package to which the filter criteria is applied.
    //
    //  Returns:
    //      The package to which the filter criteria is applied. NULL can be
    //      returned when no package was set.
    //
    //  Description:
    //      Gets the package to which the filter criteria is applied.
    //
    //==========================================================================
    virtual const char* GetPackage() = 0;

    //==========================================================================
    //==
    //==  GetNsBindings
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a pointer to all <ns-binding> elements held by this filter
    //      document.
    //
    //  Parameters:
    //    rvecpNsBindings:
    //      Vector in which to put every <ns-binding> element held in this
    //      document.
    //
    //  Returns:
    //      resS_OK upon success an error otherwise.
    //
    //  Description:
    //      Gets a pointer to all <ns-binding> elements held by this filter
    //      document. As defined in RFC 4661:
    //
    // <CODE>
    //     The <ns-bindings> element is used to bind namespaces to local
    //     prefixes used in expressions that select elements or attributes using
    //     the syntax in Section 5. Those prefixes apply to the <include>,
    //     <exclude>, <changed>, <added>, and <removed> elements.
    // </CODE>
    //
    //      More information about each <ns-binding> element can be retrieved
    //      using GetNsBinding.
    //
    //==========================================================================
    virtual mxt_result GetNsBindings(OUT CVector<CXmlElement*>& rvecpNsBindings) = 0;

    //==========================================================================
    //==
    //==  GetNsBinding
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the namespace and its associated prefix of a <ns-binding>
    //      element.
    //
    //  Parameters:
    //    pNsBinding:
    //      Pointer to the <ns-binding> element from which to get its namespace
    //      information. Cannot be NULL. This is usually an element retreived
    //      after calling GetNsBindings.
    //
    //    rpszPrefix:
    //      Reference to a const char pointer, which is set to point to the
    //      namespace prefix.
    //
    //    rpszUrn:
    //      Reference to a const char pointer, which is set to point to the
    //      namespace.
    //
    //      The pointer returned through this parameter must be considered as
    //      temporary and can become invalid any time this element or the filter
    //      document is modified. The user should copy the string it points to
    //      if it wants to keep this information for any length of time.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Gets the namespace and its associated prefix of a <ns-binding>
    //      element.
    //
    //  See Also:
    //      GetNsBindings, AddNsBinding
    //
    //==========================================================================
    virtual mxt_result GetNsBinding(IN CXmlElement* pNsBinding,
                                    OUT const char*& rpszPrefix,
                                    OUT const char*& rpszUrn) const = 0;
    
    //==========================================================================
    //==
    //==  AddNsBinding
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a <ns-binding> element to the filter document.
    //
    //  Parameters:
    //    pszPrefix:
    //      The prefix to bind. Cannot be NULL.
    //
    //    pszUrn:
    //      The namespace to bind. Cannot be NULL.
    //
    //  Returns:
    //      A pointer to the newly added <ns-binding> element. NULL in case an
    //      error occured.
    //
    //  Description:
    //      Adds a <ns-binding> element to the filter document.
    //
    //==========================================================================
    virtual CXmlElement* AddNsBinding(IN const char* pszPrefix,
                                      IN const char* pszUrn) = 0;

    //==========================================================================
    //==
    //==  GetFilters
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a pointer to all <filter> elements held by this filter
    //       document.
    //
    //  Parameters:
    //    rvpNsFilter:
    //      Vector in which to put every <filter> element held in this document.
    //
    //  Returns:
    //      resS_OK upon success an error otherwise.
    //
    //  Description:
    //      Gets a pointer to all <filter> elements held by this filter
    //      document. As defined in RFC 4661:
    //
    // <CODE>
    //     The <filter> element is used to specify the content of an individual
    //     filter.
    // </CODE>
    //
    //      More information about each <filter> element can be retrieved using
    //      GetFilter.
    //
    //==========================================================================
    virtual mxt_result GetFilters(OUT CVector<CXmlElement*>& rvecpFilters) = 0;

    //==========================================================================
    //==
    //==  AddFilter
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a <filter> element to the filter document.
    //
    //  Parameters:
    //    pszUri:
    //      The resource to which the filter applies. Can be NULL when no URI
    //      needs to be set. Must be NULL when a the filter applies to a domain.
    //      Default value is NULL.
    //
    //    pszDomain:
    //      The domain of the resources to which the filter applies. Can be NULL
    //      when no domains needs to be set. Must be NULL when a the filter
    //      applies to a URI. Default value is NULL.
    //
    //    bRemove:
    //      Whether or not this filter should be removed. Default value is
    //      false.
    //
    //    bEnabled:
    //      Whether or not this filter is enabled. Default value is true.
    //
    //  Returns:
    //      A pointer to the newly added <filter> element.
    //
    //  Description:
    //      Adds a <filter> element to the filter document.
    //
    //==========================================================================
    virtual CXmlElement* AddFilter(IN const char* pszUri = NULL,
                                   IN const char* pszDomain = NULL,
                                   IN bool bRemove = false,
                                   IN bool bEnable = true) = 0;

    //==========================================================================
    //==
    //==  AddFilter
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a <filter> element to the filter document.
    //
    //  Parameters:
    //    pUri:
    //      The resource to which the filter applies. Can be NULL when no URI
    //      needs to be set. Must be NULL when the filter applies to a domain.
    //      Default value is NULL.
    //
    //    pszDomain:
    //      The domain of the resources to which the filter applies. Can be NULL
    //      when no domains needs to be set. Must be NULL when the filter
    //      applies to a URI. Default value is NULL.
    //
    //    bRemove:
    //      Whether or not this filter should be removed. Default value is
    //      false.
    //
    //    bEnabled:
    //      Whether or not this filter is enabled. Default value is true.
    //
    //  Returns:
    //      A pointer to the newly added <filter> element.
    //
    //  Description:
    //      Adds a <filter> element to the filter document.
    //
    //==========================================================================
    virtual CXmlElement* AddFilter(IN const IUri* pUri,
                                   IN const char* pszDomain = NULL,
                                   IN bool bRemove = false,
                                   IN bool bEnable = true) = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceFilterDocument() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceFilterDocument() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceFilterDocument(IN const ISceFilterDocument& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceFilterDocument& operator=(IN const ISceFilterDocument& rSrc);


//-- Hidden Data Members.
//------------------------
protected:
private:

//M5T_INTERNAL_USE_END

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_IFILTERDOCUMENT_H
