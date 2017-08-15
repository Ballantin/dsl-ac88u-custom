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
#ifndef MXG_ISCEPIDFDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEPIDFDOCUMENT_H

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
class CMimeObject;
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
MX_DECLARE_ECOM_IID(IScePidfDocument);

//M5T_INTERNAL_USE_END

//==============================================================================
//== Class: IScePidfDocument
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Summary:
//  This interface offers the basic APIs required to manipulate a PIDF document.
//
// Description:
//  The PIDF document represents presence information of a single user as
//  defined in the following IETF documents:
//          - RFC 3863 - Presence Information Data Format (PIDF)
//          - RFC 4479 - A Data Model for Presence
//          - RFC 4480 - Rich Presence Extension to PIDF
//          - RFC 4481 - Timed Presence Extension to PIDF
//          - RFC 4482 - Contact Information for PIDF
//          - RFC 5196 - UA Capabilities extension to PIDF
//
//  The user that is represented by the PIDF document is set using SetEntity,
//  and retrieved using one of GetEntity or GetEntityUri. This class allows the
//  representation of presence characteristics and statuses of the entity, and
//  of devices and services used by this entity.
//
//  The PIDF data format is encoded in XML and is meant to be quite extensible.
//  The IScePidfDocument interface is used as a wrapper over an XML document to
//  make it easy for applications to set and access elements within this XML
//  tree. Users of this interface can always access the XML tree directly by
//  querying the IXmlDocument interface, while most of the methods of the PIDF
//  document gives direct access to the XML elements within this tree.
//
//  The IScePidfDocument interface offers APIs to access the highest level
//  elements of a presence document, like the device, service (tuple) and person
//  elements. The PIDF document only provides access to these elements as
//  CXmlElement objects (which is the root XML node of the element) but the
//  following helper classes can be used by the application to easily access and
//  manipulate the data found in these elements:
//      - CScePidfService to help manage the information found in the service
//        elements (which is encoded as the tuple XML element).
//      - CScePidfDevice to help manage the information found in the device
//        elements.
//      - CScePidfPerson to help manage the information found in the person
//        element.
//
//  Refer to each helper class to find out more about the service, device and
//  person elements and what they represents.
//
//  This interface defines the API (with ISceXmlDocument) to parse and serialize
//  "application/pidf+xml" and also "application/pidf-diff+xml" when the root
//  element is in the \<pidf-full\> format.
//
// #PIDF API Concepts#
//    - The PIDF Document owns PIDF information which is kept in the form of
//      an IXmlDocument which represents the XML tree in Document Object Model
//      (DOM) format. Any access and modification to the PIDF information done
//      using the IScePidfDocument interface is immediately reflected into the
//      IXmlDocument. 
//    - CScePidfService, CScePidfDevice and CScePidfPerson are simple helper
//      classes that are created using a reference to a CXmlElement returned by
//      the IScePidfDocument and facilitates access to the data contained in
//      this element. They only have one data member, which is the CXmlElement
//      they are initialized with. This CXmlElement is not copied and remains
//      shared between the IXmlDocument of the PIDF Document and the
//      corresponding helper class. That is a modification done using these
//      helper classes is immediately represented in the presence document.
//    - Most of the APIs offered by the IScePidfDocument and the helper class
//      return a pointer to a CXmlElement. This is a pointer that belongs to the
//      IXmlDocument of the PIDF Document. Any modifications done to the XML
//      element is represented in the PIDF document, as this pointer is not a
//      copy. An application can remove an element and all of its childrens from
//      the document by using CXmlElement::Delete. It must not be deleted using
//      any of the standard mechanisms (delete or MX_DELETE).
//    - Most of the APIs offered by the helper classes offer Get operations
//      that take a pointer to a variable as a parameter and return a pointer to
//      a CXmlElement. An application can pass a NULL pointer as a parameter in
//      order to just have access to the corresponding CXmlElement. Passing a
//      non-NULL pointer means that the interpretation of the data found in the
//      element is <B>copied</B> (<I>see const char* exception below</I>) into
//      the storage space the pointer points to, and the corresponding
//      CXmlElement is also returned. A NULL CXmlElement is returned when no
//      such element exist yet in the document.
//    - Special care must be taken for helper APIs that either return <C>const
//      char*</C> or that returns through parameters an <C>OUT const char*&</C>
//      or <C>OUT const char**</C>. In all three cases, the returned value is a
//      pointer to internal data held by a CXmlElement. There is no guarantee on
//      the length of time that this pointer will remain valid, as the value it
//      points to may be changed any time the corresponding PIDF document is
//      changed. Applications wanting to keep the returned string buffer for any
//      length of time MUST copy the string into an application managed buffer.
//      As an example, an application could copy the string into a CString 
//      object if it needed to pass this information to some other thread for
//      further processing.
//      
// #Extensibility Mechanism# 
//  Querying for the IXmlDocument interface, applications can have access to the
//  XML document, in DOM format, that corresponds to the PIDF of the user. Since
//  the PIDF document gets and stores all of its information directly into the
//  XML document, applications are free to further customize the PIDF
//  information and add their own extensions by using the IXmlDocument.
//
// Location:
//   SceXml/IScePidfDocument.h
//
// See Also:
//   CScePidfService, CScePidfDevice, CScePidfPerson
//
//==============================================================================
class IScePidfDocument : public IEComUnknown
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
    MX_DECLARE_ECOM_GETIID(IScePidfDocument);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetEntity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the entity associated with this document.
    //
    //  Parameters:
    //    pEntityUri:
    //      The new entity to set.
    //
    //  Returns:
    //      resS_OK upon success an error otherwise.
    //
    //  Description:
    //      Sets the entity associated with this document. This is the same as
    //      the entity attribute of the <presence> element.
    //
    //==========================================================================
    virtual mxt_result SetEntity(IN IUri* pEntityUri) = 0;

    //==========================================================================
    //==
    //==  SetEntity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the entity associated with this document.
    //
    //  Parameters:
    //    pszEntityUri:
    //      The new entity to set.
    //
    //  Returns:
    //      resS_OK upon success an error otherwise.
    //
    //  Description:
    //      Sets the entity associated with this document. This is the same as
    //      the entity attribute of the <presence> element.
    //
    //==========================================================================
    virtual mxt_result SetEntity(IN const char* pszEntityUri) = 0;

    //==========================================================================
    //==
    //==  GetEntityUri
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the entity associated with this document.
    //
    //  Returns:
    //      The entity of this document. NULL can be returned when no entity was
    //      set. When non NULL ownership is given.
    //
    //  Description:
    //      Gets the entity associated with this document.
    //
    //==========================================================================
    virtual GO IUri* GetEntityUri() = 0;

    //==========================================================================
    //==
    //==  GetEntity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the entity associated with this document.
    //
    //  Returns:
    //      The entity of this document. NULL can be returned when no entity was
    //      set. When non NULL ownership is given.
    //
    //  Description:
    //      Gets the entity associated with this document.
    //
    //==========================================================================
    virtual const char* GetEntity() = 0;

    //==========================================================================
    //==
    //==  GetServices
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a pointer to all Service elements held by this PIDF document.
    //
    //  Parameters:
    //    rvpServices:
    //      Vector in which to put every <service> element held in this
    //      document.
    //
    //  Returns:
    //      resS_OK upon success an error otherwise.
    //
    //  Description:
    //      Gets a pointer to all Service elements held by this PIDF document.
    //
    //==========================================================================
    virtual mxt_result GetServices(OUT CVector<CXmlElement*>& rvpServices) = 0;

    //==========================================================================
    //==
    //==  AddService
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a service (or a tuple) element to the PIDF document.
    //
    //  Returns:
    //      A pointer to the newly added service element.
    //
    //  Description:
    //      Adds a service (or a tuple) element to the PIDF document.
    //
    //==========================================================================
    virtual CXmlElement* AddService() = 0;

    //==========================================================================
    //==
    //==  GetPersons
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a pointer to all Person elements held by this PIDF document.
    //
    //  Parameters:
    //    rvpPersons:
    //      Vector in which to put every <person> element held in this document.
    //
    //  Returns:
    //      resS_OK upon success an error otherwise.
    //
    //  Description:
    //      Gets a pointer to all Person elements held by this PIDF document.
    //
    //==========================================================================
    virtual mxt_result GetPersons(OUT CVector<CXmlElement*>& rvpPersons) = 0;

    //==========================================================================
    //==
    //==  AddPerson
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a Person element to the PIDF document.
    //
    //  Returns:
    //      A pointer to the newly added person element.
    //
    //  Description:
    //      Adds a Person element to the PIDF document.
    //
    //==========================================================================
    virtual CXmlElement* AddPerson() = 0;

    //==========================================================================
    //==
    //==  GetDevices
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a pointer to all Device elements held by this PIDF document.
    //
    //  Parameters:
    //    rvpDevices:
    //      Vector in which to put every <device> element held in this document.
    //
    //  Returns:
    //      resS_OK upon success an error otherwise.
    //
    //  Description:
    //      Gets a pointer to all Device elements held by this PIDF document.
    //
    //==========================================================================
    virtual mxt_result GetDevices(OUT CVector<CXmlElement*>& rvpDevices) = 0;

    //==========================================================================
    //==
    //==  AddDevice
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a Device element to the PIDF document.
    //
    //  Parameters:
    //    pszDeviceId:
    //      Pointer to the device ID to set. Must not be NULL.
    //
    //  Returns:
    //      A pointer to the newly added device element.
    //
    //  Description:
    //      Adds a Device element to the PIDF document.
    //
    //==========================================================================
    virtual CXmlElement* AddDevice(IN const char* pszDeviceId) = 0;

    //==========================================================================
    //==
    //==  GetNotes
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a pointer to all Note elements held by this PIDF document.
    //
    //  Parameters:
    //    rvpNotes:
    //      Vector in which to put every <notes> element held in this document.
    //
    //  Returns:
    //      resS_OK upon success an error otherwise.
    //
    //  Description:
    //      Gets a pointer to all Notes elements held by this PIDF document.
    //
    //==========================================================================
    virtual mxt_result GetNotes(OUT CVector<CXmlElement*>& rvpNotes) = 0;

    //==========================================================================
    //==
    //==  AddNote
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a note to the document.
    //
    //  Parameters:
    //    pszUtf8EncodedNote:
    //      Value of the <note> element.
    //
    //    pszLanguageTag:
    //      Value of the xml:lang attribute which is used to specify the
    //      language used in the note. Can be NULL to set no such attribute.
    //      The format for defining language tags is defined in RFC 4646.
    //
    //  Returns:
    //      A pointer to the newly added note element.
    //
    //  Description:
    //      Adds a Note element to the PIDF document.
    //
    //==========================================================================
    virtual CXmlElement* AddNote(IN const char* pszUtf8EncodedNote,
                                 IN const char* pszLanguageTag) = 0;
    
    //==========================================================================
    //==
    //==  SerializePidfFull
    //==
    //==========================================================================
    //
    //  Summary:
    //      Serializes the XML document into a MIME object.
    //
    //  Parameters:
    //    uVersion:
    //      Version number of this document. This represents the version
    //      attribute to add to the <pidf-full\> element. If uVersion is 0,
    //      no version attribute is added to the document.
    //
    //    rpMimeObject:
    //      Reference to a pointer that will be set to point to a newly created
    //      CMimeObject that has been configured with all the necessary
    //      information to identify the payload as the expected content type. 
    //      MUST not point on allocated memory, otherwise it will leak as this
    //      method allocates new memory using the same pointer.
    //
    //  Description:
    //      Serializes the information into a CMimeObject and sets all the
    //      necessary MIME headers to identify the content type.
    //
    //      This method serializes the PIDF information in the
    //      "application/pidf-diff+xml" format, using a \<pidf-full\> root
    //      element.
    //
    //==========================================================================
    virtual void SerializePidfFull(IN unsigned int uVersion, OUT GO CMimeObject*& rpMimeObject) = 0;


    //==========================================================================
    //==
    //==  SerializePidfFull
    //==
    //==========================================================================
    //
    //  Summary:
    //      Serializes the XML document into a CSipMessageBody object.
    //
    //  Parameters:
    //    uVersion:
    //      Version number of this document. This represents the version
    //      attribute to add to the <pidf-full\> element. If uVersion is 0,
    //      no version attribute is added to the document.
    //
    //    rpMessageBody:
    //      Reference to a pointer that will be set to point to a newly created
    //      CSipMessageBody that has been configured with all the necessary
    //      information to identify the payload. MUST not point on allocated
    //      memory, otherwise it will leak as this method allocates new memory
    //      using the same pointer.
    //
    //  Description:
    //      Serializes the information into a CSipMessageBody and sets all the
    //      necessary MIME headers to identify the content.
    //
    //      This method serializes the PIDF information in the
    //      "application/pidf-diff+xml" format, using a \<pidf-full\> root
    //      element.
    //
    //==============================================================================
    virtual void SerializePidfFull(IN unsigned int uVersion, OUT GO CSipMessageBody*& rpMessageBody) = 0;


    //==========================================================================
    //==
    //==  SerializePidfFull
    //==
    //==========================================================================
    //
    //  Summary:
    //      Serializes the XML in a blob.
    //
    //  Parameters:
    //    uVersion:
    //      Version number of this document. This represents the version
    //      attribute to add to the <pidf-full\> element.
    //
    //    rBlob:
    //      Reference to a CBlob into which the information is serialized (or
    //      written) in raw XML form. The XML information is written at the
    //      current insertion point of the blob.
    //
    //  Description:
    //      Serializes the information in raw XML format into a CBlob.
    //
    //      This method serializes the PIDF information in the
    //      "application/pidf-diff+xml" format, using a \<pidf-full\> root
    //      element.
    //
    //==========================================================================
    virtual void SerializePidfFull(IN unsigned int uVersion, INOUT CBlob& rBlob) = 0;


//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IScePidfDocument() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IScePidfDocument() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IScePidfDocument(IN const IScePidfDocument& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IScePidfDocument& operator=(IN const IScePidfDocument& rSrc);


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


#endif //-- #ifndef MXG_ISCEPIDFDOCUMENT_H
