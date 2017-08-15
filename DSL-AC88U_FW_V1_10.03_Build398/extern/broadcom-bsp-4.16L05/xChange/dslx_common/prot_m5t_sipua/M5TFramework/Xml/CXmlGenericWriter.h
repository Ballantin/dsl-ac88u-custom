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
#ifndef MXG_CXMLGENERICWRITER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CXMLGENERICWRITER_H

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

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_CSTACK_H
#include "Cap/CStack.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

//-- Interface Realized and/or Parent.

#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#ifndef MXG_IXMLGENERICWRITER_H
#include "Xml/IXmlGenericWriter.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CXmlGenericWriter
//========================================
//<GROUP XML_CLASSES>
//
// Description:
//  Implements the IXmlWriter interface.
//
// Location:
//   Xml/CXmlGenericWriter.h
//
// See Also:
//   IXmlGenericWriter
//
//==============================================================================
class CXmlGenericWriter : protected CEComUnknown,
                          private IXmlGenericWriter
{
//-- Friend Declarations.


//-- Published Interface.
public:

//-- Hidden Methods.
protected:
    //-- << Static Initializer >>

    static mxt_result InitializeCXmlGenericWriter();
    static void FinalizeCXmlGenericWriter();

    //-- Friendship declaration.
    friend mxt_result InitializeCXmlGenericWriter();
    friend void FinalizeCXmlGenericWriter();

private:
    //-- << ECom Pattern >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComUnknown Pattern >>
    MX_DECLARE_IECOMUNKNOWN
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);
    virtual mxt_result InitializeInstance();
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    // Summary:
    //  Default Constructor.
    CXmlGenericWriter();

    // Summary:
    //  Destructor.
    virtual ~CXmlGenericWriter();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CXmlGenericWriter(IN const CXmlGenericWriter& rSrc);

    // Summary:
    //  Assignment Operator.
    CXmlGenericWriter& operator=(IN const CXmlGenericWriter& rSrc);

    //-- << IXmlGenericWriter interface >>

    virtual mxt_result StartDocument(IN TO CBlob* pBuffer,
                                     IN const char* pszEncoding = "UTF-8",
                                     IN EEolMode = eCRLF,
                                     IN EDocumentProlog eDocumentType =
                                                        eTYPE_XMLv1_STANDALONE);

    virtual mxt_result EndDocument(OUT GO CBlob*& rpDocument);


    virtual mxt_result StartDocument(IN PFNDataWriter pfnWriter,
                                     IN mxt_opaque opqWriter,
                                     IN const char* pszEncoding = "UTF-8",
                                     IN EEolMode = eCRLF,
                                     IN EDocumentProlog eDocumentType =
                                                        eTYPE_XMLv1_STANDALONE);

    virtual mxt_result EndDocument();

    virtual mxt_result WriteElementStart(IN const char* pszNamespacePrefix,
                                         IN const char* pszElementName);

    virtual mxt_result WriteElementAttributesNs(IN const char* pszNamespacePrefix,
                                                IN const char* pszNamespaceUri);

    virtual mxt_result WriteElementAttribute(IN const char* pszNamespacePrefix,
                                             IN const char* pszAttributeName,
                                             IN const char* pszAttributeValue);

    virtual mxt_result WriteElementValue(IN const char* pszValue);

    virtual mxt_result WriteElementAttribute(IN const char* pszNamespacePrefix,
                                             IN const char* pszAttributeName,
                                             IN int iAttributeValue);

    virtual mxt_result WriteElementValue(IN int iValue);

    virtual mxt_result WriteElementAttribute(IN const char* pszNamespacePrefix,
                                             IN const char* pszAttributeName,
                                             IN unsigned int uAttributeValue);

    virtual mxt_result WriteElementValue(IN unsigned int uValue);

    virtual mxt_result WriteElementAttribute(IN const char* pszNamespacePrefix,
                                             IN const char* pszAttributeName,
                                             IN float fAttributeValue);

    virtual mxt_result WriteElementValue(IN float fValue);

    virtual mxt_result WriteElementEnd(IN bool bForceExplicitClosing = false);

    //-- << Helpers >>

    // Summary:
    //  Writes the XML header.
    mxt_result WriteXmlHeader(IN const char* pszEncoding);

    // Summary:
    //  Escapes and writes the buffer.
    mxt_result EscapeAndWrite(IN const char* pszBuffer);

    // Summary:
    //  Indents the line correctly.
    mxt_result Indent(IN bool bForClosingElement = false);

    // Summary:
    //  Writes the correct end of line.
    mxt_result WriteEol();

    // Summary:
    //  Writes the buffer into the document.
    mxt_result Write(IN const char *pszBuffer, IN unsigned int uBufferSize);

    // Summary:
    //  Writes the buffer into the document.
    mxt_result Write(IN const char *pszBuffer);


//-- Hidden Data Members.
protected:
private:
    // Summary:
    //  Enumeration of the writer status.
    enum EWriterStatus
    {
        // Description:
        //  Waiting for first element to write the namespace into it, no prolog has been written.
        eWAITING_FOR_FIRST_ELEMENT_NO_PROLOG,

        // Description:
        //  Waiting for first element to write the namespace into it, prolog has been written.
        eWAITING_FOR_FIRST_ELEMENT,

        // Description:
        //  Writing of an element has been started.
        eELEMENT_STARTED,

        // Description:
        //  A value has been added to the element.
        eELEMENT_VALUE,

        // Description:
        //  The element is now closed.
        eELEMENT_DONE,
    };

    CBlob* m_pDocument;
    PFNDataWriter m_pfnWriter;
    mxt_opaque m_opqWriter;
    EEolMode m_eEolMode;
    bool m_bDefaultNamespaceAdded;
    EWriterStatus m_eWriterStatus;

    CStack<CString, CList<CString> > m_stkElement;
    CString m_strFormat;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  Write
//==
//==============================================================================
//
//  Parameters:
//    pszBuffer:
//      The string buffer to write.
//
//  Returns:
//      - resS_OK: The buffer was written.
//      - resFE_FAIL: No output handler could be found to write the buffer.
//
//  Description:
//      Appends the string into the document buffer or calls the function
//      pointer.
//
//==============================================================================
inline mxt_result CXmlGenericWriter::Write(IN const char *pszBuffer)
{
    return Write(pszBuffer, strlen(pszBuffer));
}

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CXMLGENERICWRITER_H
