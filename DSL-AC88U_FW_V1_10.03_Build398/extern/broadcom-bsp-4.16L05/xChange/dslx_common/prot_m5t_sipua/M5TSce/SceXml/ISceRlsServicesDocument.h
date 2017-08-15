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
#ifndef MXG_ISCERLSSERVICESDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCERLSSERVICESDOCUMENT_H
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
MX_DECLARE_ECOM_IID(ISceRlsServicesDocument);
//M5T_INTERNAL_USE_END

//==============================================================================
//== Class: ISceRlsServicesDocument
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Summary:
//  This interface is used to access a Resource List document.
//
// Description:
//  The IRlsServicesDocument stands for "Resource List Server Service Document".
//
//  The RLS Service document is used to define services (identified by an URI)
//  provided by a Resource List Server (RLS).
//
//  A Resource List Server accepts subscriptions to resource lists and sends
//  notifications to the subscriber with the state of each resources found on
//  the resource list.
//
//  As an example, a UA client that wants to subscribe to all of his buddies on
//  his buddy list could send an individual subscribe to each buddies, which
//  would create many subscriptions, or it could send a single subscription to a
//  resource list server, with the list of buddies to subscribe to, and the
//  server would subscribe to each buddy on the list on behalf of the UA client.
//  The server would then send notification to the UA client, possibly with
//  aggregated state information about each buddy.
//  
//  The IRlsServicesDocument defines a format to allow a UA client to define or
//  access the URI it must use to subscribe to the server, and it also allow to
//  either pass the list of users to subscribe to or a reference XCAP URI to a
//  list the server must use.
//  
//  The RLS Services Document keeps the RLS information encoded in XML. It
//  implements the \<rls-services\> element of RFC 4826 and supports the
//  application/rls-services+xml MIME format. The CSceResourceList helper class
//  can be used to manipulate a resource list that is included directly within a
//  RLS Services document.
//
//  The ISceRlsServicesDocument is used as a wrapper over an XML document to
//  make it easy for applications to set and access elements within this XML
//  tree.Users can always access the XML tree directly by using QueryIf, while
//  the methods of this class and CSceResourceList give direct access to the XML
//  elements within this tree.
//
// #XML API Concepts#
//   - The "Document" class is the class that holds the XML representation of
//     the data in the form of an IXmlDocument. 
//   - "Helper" classe(s) may be availble which further help manage elements
//     from the XML document. These helper classes usually take a CXmlElement as
//     a reference in their constructor, and further API calls on this helper
//     class will use this XML element or children elements to access and set
//     the XML data. These "Helper" classes are lightweight and can be created
//     on the stack (as a local variable) when the need to access and set this
//     particular data arises.
//   - Any modification done to the data using either the "Document" API,
//     "Helper" API or directly using a CXmlElement associated with this XML
//     document is immediately reflected in the XML representation of this data.
//   - The ownership of *all* pointers returned by the "Document" class, its
//     helper classe(s) and the CXmlElement, is *never* given to the caller.
//     This means that when a pointer is returned to the caller, it should
//     immediately use it or copy its content if the data is to be used
//     asynchronously. Callers that perserve a pointer for any length of time
//     run the chance to see the pointer invalidated at any time another API
//     call is done on the "Document", on an associated "Helper" instance or on
//     an associated CXmlElement.
//   - Some of the APIs offered by the helper classes offer Set operations
//     that take a pointer to a value. Passing a NULL pointer is allowed as a
//     convenient way to remove the specified element from the document.
//   - Most of the APIs offered by the helper classes offer Get operations
//     that take a pointer to a variable as a parameter and return a pointer to
//     a CXmlElement. An application can pass a NULL pointer as a parameter in
//     order to just have access to the corresponding CXmlElement. Passing a
//     non-NULL pointer means that the interpretation of the data found in the
//     element is <B>copied</B> (<I>see const char* exception below</I>) into
//     the storage space the pointer points to, and the corresponding
//     CXmlElement is also returned. A NULL CXmlElement is returned when no
//     such element exist yet in the document.
//   - Special care must be taken for helper APIs that either return <C>const
//     char*</C> or that returns through parameters an <C>OUT const char*&</C>
//     or <C>OUT const char**</C>. In all three cases, the returned value is a
//     pointer to internal data held by a CXmlElement. There is no guarantee on
//     the length of time that this pointer will remain valid, as the value it
//     points to may be changed any time the corresponding PIDF document is
//     changed. Applications wanting to keep the returned string buffer for any
//     length of time MUST copy the string into an application managed buffer.
//     As an example, an application could copy the string into a CString 
//     object if it needed to pass this information to some other thread for
//     further processing.
//
// Location:
//   SceXml/ISceRlsServicesDocument.h
//
//
//==============================================================================
class ISceRlsServicesDocument : public IEComUnknown
{
//-- Friend Declarations.
//------------------------

//-- New types within class namespace.
//-------------------------------------
public:
    // Description:
    //  This enumeration defines the two different ways a service can be held
    //  by this document.
    enum EServiceInfo
    {
        // Description:
        //  The service does not hold a valid \<list\> or \<resource-list\> element.
        eSVCINFO_INVALID,
    
        // Description:
        //  The service holds a complete resource list. The application must
        //  use the proper version of GetService to access this service.
        eSVCINFO_RESOURCELIST,
    
        // Description:
        //  The service holds a XCAP reference to a resource list. The
        //  application must use the proper version of GetService to access
        //  this service.
        eSVCINFO_RESOURCELISTREF
    };

protected:
private:

//-- Published Interface.
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceRlsServicesDocument);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  AddService
    //==
    //==========================================================================
    //
    // Summary:
    //  Adds a service that identifies its resources through a XCAP reference
    //  to a resource list.
    //
    //  Parameters:
    //    pszServiceUri:
    //      The service URI, it can not be NULL.
    //
    //    pszListReferenceUri:
    //      XCAP Uri pointing to a \<list\> element in a resource-list document.
    //      Can not be NULL.
    //
    //    uSipEventBitset:
    //      Bit set describing the packages to be added to this service. See the
    //      ESipEventType enumeration for a description of different event types.
    //
    //  Returns:
    //      A pointer to the created service Xml element or NULL on error.
    //
    //  Description:
    //      Add a service to the Rls-Services document setting the mandatory uri
    //      attribute. The pointer pszListReferenceUri points to a \<list\> in a 
    //      resource-list document. Both, pszServiceUri and pszListReferenceUri
    //      are required to create the \<service\> element and they can not be NULL.
    //
    //  See Also:
    //      GetServices, GetServiceInfo
    //
    //==========================================================================
    virtual CXmlElement* AddService( IN const char* pszServiceUri,
                                     IN const char* pszListReferenceUri,
                                     IN uint32_t uSipEventBitset ) = 0;

    //==========================================================================
    //==
    //==  AddService
    //==
    //==========================================================================
    //
    // Summary:
    //  Adds a service that identifies its resources through the inclusion of a
    //  resource list.
    //    
    //  Parameters:
    //    pszServiceUri:
    //      The service URI, it can not be NULL.
    //
    //    uSipEventBitset:
    //      Bit set describing the packages to be added to this service. See the
    //      ESipEventType enumeration for a description of different event types.
    //
    //  Returns:
    //      A pointer to the created service Xml element or NULL on error.
    //
    //  Description:
    //      Add a service to the Rls-Services document setting the mandatory uri
    //      attribute. A \<list\> element it created as the first child
    //      of the service element. 
    //
    //      Use GetServiceResourceList to access the created list element, and
    //      then use a CSceResourceList to further customize the list element.
    //
    //  Example:
    //      This provides an overview of how to get the resource list.
    //     <CODE>
    //          CXmlElement* pServiceElement = pRlsDocument->AddService("sip:test@m5t.com", uBitset);
    //          CXmlElement* pListElement = pRlsDocument->GetServiceResourceList(pServiceElement);
    //          CSceResourceList list(pListElement);
    //     </CODE>
    //
    //  See Also:
    //      GetServices, GetServiceInfo, GetServiceResourceList, CSceResourceList
    //
    //
    //==========================================================================
    virtual CXmlElement* AddService( IN const char* pszServiceUri,
                                     IN uint32_t uSipEventBitset ) = 0;


    //==========================================================================
    //==
    //==  GetServices
    //==
    //==========================================================================
    //
    // Summary:
    //  Gets the service elements found in this document.
    //
    //  Parameters:
    //    rvpService:
    //      Vector used to return the found service elements.
    //
    //  Returns:
    //      resS_OK upon succesful or resFE_FAIL if an error has happend.
    //
    //  Description:
    //      This method looks for the \<service\> elements found in the document and
    //      adds them to the vector rvpService.
    //
    //  See Also:
    //      GetServiceInfo
    //
    //==========================================================================
    virtual mxt_result GetServices(OUT CVector<CXmlElement*>& rvpService) = 0;

    //==========================================================================
    //==
    //==  GetServiceInfo
    //==
    //==========================================================================
    //
    // Summary:
    //  Gets Service information about the resource list attached to the service.
    //
    //  Parameters:
    //    pServiceElement:
    //      Pointer to the \<service\> element.
    //
    //  Returns:
    //      eSVCINFO_INVALID: on error or if an invalid \<list\> or \<resource-list\>
    //         reference has found.
    //      eSVCINFO_RESOURCELIST: if an embedded \<list\> element has been found.
    //      eSVCINFO_RESOURCELISTREF: if a \<resource-list\> with a reference to
    //         a \<list\> has been found.
    //
    //  Description:
    //      Gets Service information about the resource list attached to the
    //      service pointed by pServiceElement.
    //
    //  See Also:
    //      GetServices
    //
    //==========================================================================
    virtual EServiceInfo GetServiceInfo(IN CXmlElement* pServiceElement) = 0;

    //==========================================================================
    //==
    //==  GetServiceEvents
    //==
    //==========================================================================
    //
    // Summary:
    //  Gets the events supported by this service element.
    //
    //  Parameters:
    //    pServiceElement:
    //      Pointer to the service element.
    //
    //    rSipEventBitset:
    //      Used to return the bitset describing the packages supported by the
    //      service. See the ESipEventType enumeration for a description of
    //      the different event types.
    //
    //  Returns:
    //      resS_OK on success or resFE_FAIL otherwise.
    //
    //  Description:
    //      This method scans the service element looking for the events supported.
    //      The events are grouped in a bitset, according to their definition in
    //      SipEventTypes.h.
    //
    //  See Also:
    //      AddService.
    //
    //==========================================================================
    virtual mxt_result GetServiceEvents(IN CXmlElement* pServiceElement,
                                        OUT uint32_t& rSipEventBitset) = 0;


    //==========================================================================
    //==
    //==  GetServiceUri
    //==
    //==========================================================================
    //
    // Summary:
    //      Gets the URI associated with this service. Can not be NULL.
    //    
    //  Parameters:
    //    pServiceElement:
    //      Pointer to the service.
    //
    //    rpszServiceUri:
    //      Used to return the service Uri.
    //
    //  Returns:
    //      resS_OK on success or resFE_FAIL if there was an error.
    //
    //  Description:
    //      Returns the Uri associated to the service element pointed by
    //      pServiceElement. The uri attribut must be always present in the
    //      \<service\> element.
    //
    //  See Also:
    //      AddService
    //
    //==========================================================================
    virtual mxt_result GetServiceUri(IN CXmlElement* pServiceElement,
                                     OUT const char*& rpszServiceUri) = 0;


    //==========================================================================
    //==
    //==  GetServiceResourceListReference
    //==
    //==========================================================================
    //
    // Summary:
    //      Gets the reference URI that points to the resource list assigned to
    //      this service.
    //    
    //  Parameters:
    //    pServiceElement:
    //      Pointer to the service.
    //
    //    rpszListReferenceUri:
    //      Used to return the service Uri of the \<list\> reference.
    //
    //  Returns:
    //      resS_OK on success or resFE_FAIL if the service has no 
    //      \<resource-list\> element or if there was an error.
    //
    //  Description:
    //      This method returns the list reference Uri in the rpszListReferenceUri
    //      parameter. If the service has no \<resource-list\> element, this method will return
    //      an error and rpszListReferenceUri is set to NULL. The method
    //      GetServiceInfo() can be used in order to find out if the \<service\>
    //      has an embedded \<list\> or a \<resource-list\> element with a reference
    //      to a \<list\> in a resource-list document.
    //
    //  See Also:
    //      AddService, GetServiceResourceList, GetServiceInfo
    //
    //
    //==========================================================================
    virtual mxt_result GetServiceResourceListReference( IN CXmlElement* pServiceElement,
                                                        OUT const char*& rpszListReferenceUri) = 0;


    //==========================================================================
    //==
    //==  GetServiceResourceList
    //==
    //==========================================================================
    //
    // Summary:
    //  Gets the resource list held by the service element.
    //
    //  Parameters:
    //    pServiceElement:
    //      Param Description.
    //
    //  Returns:
    //      A pointer to the \<list\> element or NULL if not found.
    //
    //  Description:
    //      This method looks for a child \<list\> element and returns a pointer
    //      to it or NULL if not found. The method GetServiceInfo() can be used
    //      in order to find out if the \<service\> has an embedded \<list\> or
    //      a \<resource-list\> element with a reference to a \<list\> in a
    //      resource-list document.
    //
    //      A CSceResourceList can be use to further manipulate the returned
    //      element.
    //
    //  See Also:
    //      AddService, GetServiceResourceListReference, GetServiceInfo, CSceResourceList
    //
    //
    //==========================================================================
    virtual CXmlElement* GetServiceResourceList(IN CXmlElement* pServiceElement) = 0;


//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceRlsServicesDocument() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceRlsServicesDocument() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceRlsServicesDocument(IN const ISceRlsServicesDocument& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceRlsServicesDocument& operator=(IN const ISceRlsServicesDocument& rSrc);

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

#endif //-- #ifndef MXG_ISCERLSSERVICESDOCUMENT_H
