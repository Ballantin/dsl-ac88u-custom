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
#ifndef MXG_IXMLGENERICWRITER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXMLGENERICWRITER_H
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
#include "Config/FrameworkCfg.h" // MXD_XML_ENABLE_SUPPORT
#endif

#if defined(MXD_XML_ENABLE_SUPPORT) && \
    defined(MXD_ECOM_ENABLE_SUPPORT)

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IXmlGenericWriter);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
// <GROUP XML_TYPES>
// Summary:
//  Callback type definition.
//
// Description:
//  This type defines a callback to output data to the XML container for the XML 
//  writer interface.
typedef mxt_result (*PFNDataWriter)(IN const char *pszBuffer, IN unsigned int uBufferSize);

//==SDOC========================================================================
//== Class: IXmlGenericWriter
//========================================
//<GROUP XML_CLASSES>
//
// Description:
//  This interface defines helper functionality for writing XML documents.
//
//  There are two ways to use this interface, depending on wich StartDocument is
//  called:
//      - Write the document in a blob.
//      - Use a callback to allow the user of this interface to write the
//        document into something other than a blob. This interface calls the
//        callback as it is provided with the data to write.
//
//  This interface only writes standalone XML documents. It does not allow
//  to directly write comments, CDATA or processing instructions.
//
//  This interface supports namespaces. XML namespaces must all be declared
//  before StartDocument is called. When namespaces are defined, they are all
//  declared within the first XML element.
//
// Location:
//   Xml/IXmlGenericWriter.h
//
// See Also:
//   IXmlParser
//
//==EDOC========================================================================
class IXmlGenericWriter : public IEComUnknown
{
//-- Friend Declarations.

//-- Published Interface.
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IXmlGenericWriter);
//M5T_INTERNAL_USE_END

    // Summary:
    //  Enumeration of the end of line modes.
    enum EEolMode
    {
        // Description:
        //  Each XML tag is on its own line, terminated by a LF. XML elements
        //  are properly indented.
        eLF,

        // Description:
        //  Each XML tag is on its own line, terminated by a CR. XML elements
        //  are properly indented.
        eCR,


        // Description:
        //  Each XML tag is on its own line, terminated by a CRLF. XML elements
        //  are properly indented.
        eCRLF,

        // Description:
        //  This is a special mode where the whole document it outputted on a
        //  single line. No indentation is performed.
        eNONE
    };

    // Summary:
    //  Enumeration of the type of XML document to create.
    enum EDocumentProlog
    {
        // Description:
        //  Creates a document without any prolog information.
        eTYPE_NOPROLOG,

        // Description:
        //  Creates a document with a prolog information identifying the
        //  document as standalone and XML 1.0.
        eTYPE_XMLv1_STANDALONE
    };

    //==========================================================================
    //==
    //==  StartDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //      Starts a XML Document, it can be a version 1 standalone XML or a
    //      a fragment of a XML document that does not include the prolog.
    //
    //  Parameters:
    //    pBuffer:
    //      Pointer to the blob into which the document is to be written. The
    //      writer will begin writing at the end of the blob and will preserve
    //      existing blob data, if any. It cannot be NULL.
    //
    //    pszEncoding:
    //      The encoding used. This string is simply written as the encoding
    //      string in the first line of the XML document. This class does not
    //      implement any logic regarding the encoding parameter. It is left to
    //      the caller to format the provided character strings according to the
    //      encoding parameter.
    //
    //    eMode:
    //      How lines should be terminated.
    //
    //    eDocumentType:
    //      Specifies the type of document to create: it can be
    //      eTYPE_XMLv1_STANDALONE for a standalone document that includes
    //      the prolog or eTYPE_NOPROLOG for a fragment XML that does
    //      not include the prolog.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Starts an XML document and has the output redirected into a CBlob
    //      object.
    //
    //      Although this method takes ownership of the blob provided in
    //      parameter, the same blob is provided back to the caller when
    //      EndDocument is called.
    //
    //  See Also:
    //      EndDocument@OUT GO CBlob*&
    //
    //==========================================================================
    virtual mxt_result StartDocument(IN TOA CBlob* pBuffer,
                                     IN const char* pszEncoding = "UTF-8",
                                     IN EEolMode = eCRLF,
                                     IN EDocumentProlog eDocumentType =
                                                        eTYPE_XMLv1_STANDALONE) = 0;

    //==========================================================================
    //==
    //==  EndDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //    Ends the XML Document.
    //
    //  Parameters:
    //    rpDocument:
    //      Pointer to the blob into which the XML document was written.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise. This method will succeed
    //      only if all previously started elements were properly terminated.
    //
    //  Description:
    //      Terminated the XML document and provides the blob back to the caller.
    //
    //==EDOC====================================================================
    virtual mxt_result EndDocument(OUT GO CBlob*& rpDocument) = 0;


    //==========================================================================
    //==
    //==  StartDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //      Starts an XML Document, it can be a version 1 standalone XML or a
    //      a fragment of a XML document that does not include the prolog.
    //
    //  Parameters:
    //    pWriter:
    //      Function pointer called to output data to the XML container.
    //
    //    opqWriter:
    //      Opaque data to provide to the callback.
    //
    //    pszEncoding:
    //      The encoding used. This string is simply written as the encoding
    //      string in the first line of the XML document. This class does not
    //      implement any logic regarding the encoding parameter. It is left to
    //      the caller to format the provided character strings according to the
    //      encoding parameter.
    //
    //    eMode:
    //      How lines should be terminated.
    //
    //    eDocumentType:
    //      Specifies the type of document to create: it can be
    //      eTYPE_XMLv1_STANDALONE for a standalone document that includes
    //      the prolog or eTYPE_NOPROLOG for a fragment XML that does
    //      not include the prolog.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Starts an XML document and has the output redirected to a caller
    //      provided function callback.
    //
    //      The caller must use the proper EndDocument method to terminate the
    //      XML document.
    //
    //  See Also:
    //      EndDocument
    //
    //==========================================================================
    virtual mxt_result StartDocument(IN PFNDataWriter pfnWriter,
                                     IN mxt_opaque opqWriter,
                                     IN const char* pszEncoding = "UTF-8",
                                     IN EEolMode = eCRLF,
                                     IN EDocumentProlog eDocumentType =
                                                        eTYPE_XMLv1_STANDALONE) = 0;

    //==========================================================================
    //==
    //==  EndDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //    Ends the XML document.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise. This method will succeed
    //      only if all previously started elements were properly terminated.
    //
    //  Description:
    //      Terminated the XML document that had its output redirected to a
    //      function callback.
    //
    //==========================================================================
    virtual mxt_result EndDocument() = 0;


    //==========================================================================
    //==
    //==  WriteElementAttributesNs
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes a namespace into the current element.
    //
    //  Parameters:
    //
    //    pszNamespacePrefix:
    //      The prefix to associate with the URI as a NULL terminated string.
    //      It can be NULL, it is the case of the default namespace declaration.
    //      It MUST be valid according to the XML syntax.
    //
    //    pszNamespaceUri:
    //      The namespace URI as a NULL terminated string. It can be NULL, in
    //      this case it will undeclare the name space.
    //
    //  Returns:
    //      - resS_OK upon success.
    //      - resFE_FAIL if this method is called outside an element.
    //      - Any other error returned by the Write or EscapeAndWrite methods.
    //
    //  Description:
    //      Writes a namespace for a previously started element. This can only
    //      be called after that WriteElementStart, a namespace can be declared
    //      anywhere an attribute is declared. Namespaces and attributes can be
    //      mixed. This function can be called several times for the same
    //      element.
    //      This function only writes the namespace and it does not perform any
    //      verification on it.
    //      Both arguments can be NULL:
    //        pszNamespacePrefix != NULL   pszNamespaceUri != NULL
    //          Declares a namespace
    //           Sample: xmlns:foo="http://www.foo.org/"
    //
    //        pszNamespacePrefix == NULL   pszNamespaceUri != NULL
    //          Declares a default namespace
    //           Sample: xmlns="http://www.foo.org/"
    //
    //        pszNamespacePrefix != NULL   pszNamespaceUri == NULL
    //          Undeclares the namespace.
    //           Sample: xmlns:foo=""
    //
    //        pszNamespacePrefix == NULL   pszNamespaceUri == NULL
    //          Undeclares the default spacename.
    //           Sample: xmlns=""
    //
    //==========================================================================
    virtual mxt_result WriteElementAttributesNs(IN const char* pszNamespacePrefix,
                                                IN const char* pszNamespaceUri) = 0;


    //==========================================================================
    //==
    //==  WriteElementStart
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes the start tag of an element.
    //
    //  Parameters:
    //    pszNamespacePrefix:
    //      The namespace prefix for this element, as a NULL terminated string.
    //      It must be NULL if the element must not be associated with any
    //      namespace.
    //
    //    pszElementName:
    //      The element name, as a NULL terminated string.
    //      It MUST be valid according to the XML syntax.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Writes the start tag of an XML element along with its namespace.
    //
    //  See Also:
    //      WriteElementAttribute, WriteElementValue, WriteElementEnd
    //
    //==========================================================================
    virtual mxt_result WriteElementStart(IN const char* pszNamespacePrefix,
                                         IN const char* pszElementName) = 0;

    //==========================================================================
    //==
    //==  WriteElementAttribute
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes attributes to the current element.
    //
    //  Parameters:
    //    pszNamespacePrefix:
    //      The namespace prefix for this element, as a NULL terminated string.
    //      It Must be NULL if the element must not be associated with any
    //      namespace. It MUST have been already defined.
    //
    //    pszAttributeName:
    //      The name of the attribute, it must not be NULL.
    //      It MUST be valid according to the XML syntax.
    //
    //    pszAttributeValue:
    //      The value of the attribute, it must not be NULL.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Writes an attribute for a previously started element. This can only
    //      be called immediately after WriteElementStart. It must be called
    //      once for each attribute to add.
    //
    //  See Also:
    //      WriteElementStart, WriteElementValue, WriteElementEnd
    //
    //==========================================================================
    virtual mxt_result WriteElementAttribute(IN const char* pszNamespacePrefix,
                                             IN const char* pszAttributeName,
                                             IN const char* pszAttributeValue) = 0;

    //==========================================================================
    //==
    //==  WriteElementValue
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes a value to the current element.
    //
    //  Parameters:
    //    pszValue:
    //      The data to write, as a NULL terminated string.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Writes a value associated with the current element. This can be called
    //      anytime after a call to WriteElementStart, and it can be called any
    //      number of times. Note that this can also be called before and after
    //      another element is inserted. See the example for further details.
    //
    //  See Also:
    //      WriteElementStart, WriteElementEnd, WriteElementAttribute
    //
    //  Example:
    //    To achieve the following XML construct:
    //  <CODE>
    //        \<test\>
    //          This is a
    //          \<bold\>
    //              TEST
    //          \</bold\>
    //          for WriteElementValue
    //        \</test\>
    //  </CODE>
    //
    //  The following calls would have to be done (shown in a simplified API)
    //  <CODE>
    //     WriteElementStart("test");      // Current element = test
    //     WriteElementValue("This is a");
    //     WriteElementStart("bold");      // Current element = bold
    //     WriteElementValue("TEST");
    //     WriteElementEnd();              // Current element = test
    //     WriteElementValue("for WriteElementValue");
    //     WriteElementEnd();
    //  </CODE>
    //
    //==========================================================================
    virtual mxt_result WriteElementValue(IN const char* pszValue) = 0;

    //==========================================================================
    //==
    //==  WriteElementAttribute
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes attributes to the current element.
    //
    //  Parameters:
    //    pszNamespacePrefix:
    //      The namespace prefix for this element, as a NULL terminated string.
    //      It must be NULL if the element must not be associated with any
    //      namespace. It MUST have been already defined.
    //
    //    pszAttributeName:
    //      The name of the attribute, it must not be NULL.
    //      It MUST be valid according to the XML syntax.
    //
    //    iAttributeValue:
    //      The value of the attribute.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Writes an attribute for a previously started element. This can only
    //      be called immediately after WriteElementStart. It must be called
    //      once for each attribute to add.
    //
    //  See Also:
    //      WriteElementStart, WriteElementValue, WriteElementEnd
    //
    //==========================================================================
    virtual mxt_result WriteElementAttribute(IN const char* pszNamespacePrefix,
                                             IN const char* pszAttributeName,
                                             IN int iAttributeValue) = 0;

    //==========================================================================
    //==
    //==  WriteElementValue
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes a value to the current element.
    //
    //  Parameters:
    //    iValue:
    //      The data value to write.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Writes a value associated with the current element. This can be called
    //      anytime after a call to WriteElementStart, and it can be called any
    //      number of times. Note that this can also be called before and after
    //      another element is inserted. See the example for further details.
    //
    //  See Also:
    //      WriteElementStart, WriteElementEnd, WriteElementAttribute
    //
    //==========================================================================
    virtual mxt_result WriteElementValue(IN int iValue) = 0;

    //==========================================================================
    //==
    //==  WriteElementAttribute
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes attributes to the current element.
    //
    //  Parameters:
    //    pszNamespacePrefix:
    //      The namespace prefix for this element, as a NULL terminated string.
    //      It Must be NULL if the element must not be associated with any
    //      namespace. It MUST have been already defined.
    //
    //    pszAttributeName:
    //      The name of the attribute, it must not be NULL.
    //      It MUST be valid according to the XML syntax.
    //
    //    uAttributeValue:
    //      The value of the attribute.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Writes an attribute for a previously started element. This can only
    //      be called immediately after WriteElementStart. It must be called
    //      once for each attribute to add.
    //
    //  See Also:
    //      WriteElementStart, WriteElementValue, WriteElementEnd
    //
    //==========================================================================
    virtual mxt_result WriteElementAttribute(IN const char* pszNamespacePrefix,
                                             IN const char* pszAttributeName,
                                             IN unsigned int uAttributeValue) = 0;

    //==========================================================================
    //==
    //==  WriteElementValue
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes a value to the current element.
    //
    //  Parameters:
    //    uValue:
    //      The data value to write.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Writes a value associated with the current element. This can be called
    //      anytime after a call to WriteElementStart, and it can be called any
    //      number of times. Note that this can also be called before and after
    //      another element is inserted. See the example for further details.
    //
    //  See Also:
    //      WriteElementStart, WriteElementEnd, WriteElementAttribute
    //
    //==========================================================================
    virtual mxt_result WriteElementValue(IN unsigned int uValue) = 0;

    //==========================================================================
    //==
    //==  WriteElementAttribute
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes attributes to the current element.
    //
    //  Parameters:
    //    pszNamespacePrefix:
    //      The namespace prefix for this element, as a NULL terminated string.
    //      It Must be NULL if the element must not be associated with any
    //      namespace. It MUST have been already defined.
    //
    //    pszAttributeName:
    //      The name of the attribute, it must not be NULL.
    //      It MUST be valid according to the XML syntax.
    //
    //    fAttributeValue:
    //      The value of the attribute.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Writes an attribute for a previously started element. This can only
    //      be called immediately after WriteElementStart. It must be called
    //      once for each attribute to add.
    //
    //  See Also:
    //      WriteElementStart, WriteElementValue, WriteElementEnd
    //
    //==========================================================================
    virtual mxt_result WriteElementAttribute(IN const char* pszNamespacePrefix,
                                             IN const char* pszAttributeName,
                                             IN float fAttributeValue) = 0;

    //==========================================================================
    //==
    //==  WriteElementValue
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes a value to the current element.
    //
    //  Parameters:
    //    fValue:
    //      The data value to write.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Writes a value associated with the current element. This can be called
    //      anytime after a call to WriteElementStart, and it can be called any
    //      number of times. Note that this can also be called before and after
    //      another element is inserted. See the example for further details.
    //
    //  See Also:
    //      WriteElementStart, WriteElementEnd, WriteElementAttribute
    //
    //==========================================================================
    virtual mxt_result WriteElementValue(IN float fValue) = 0;

    //==========================================================================
    //==
    //==  WriteElementEnd
    //==
    //==========================================================================
    //
    //  Summary:
    //    Writes the closing tag for the current element.
    //
    //  Parameters:
    //    bForceExplicitClosing:
    //      Forces empty elements to be closed in a separate XML tag.
    //      The default value is false.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Writes the end tag of the current XML element.
    //
    //      The parameter bForceExplicitClosing is only used when writing the
    //      end of an element for which there was no value writen with
    //      WriteElementValue. For such elements, if bForceExplicitClosing is
    //      true, it yields the following XML:
    //
    //  <CODE>
    //        \<test\>
    //        \</test\>
    //  </CODE>
    //
    //      If bForceExplicitClosing is false, which is the default, it yields:
    //  <CODE>
    //        \<test/\>
    //  </CODE>
    //
    //  See Also:
    //      WriteElementStart, WriteElementValue, WriteElementAttribute
    //
    //==========================================================================
    virtual mxt_result WriteElementEnd(IN bool bForceExplicitClosing = false) = 0;


//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IXmlGenericWriter() {}

    // Summary:
    //  Destructor.
    virtual ~IXmlGenericWriter() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IXmlGenericWriter(IN const IXmlGenericWriter& rSrc);

    // Summary:
    //  Assignment Operator.
    IXmlGenericWriter& operator=(IN const IXmlGenericWriter& rSrc);

//-- Hidden Data Members.
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_XML_ENABLE_SUPPORT) &&
       //--     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_IXMLGENERICWRITER_H
