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
#ifndef MXG_CXMLPARSEREXPAT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CXMLPARSEREXPAT_H

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

#if defined(MXD_XML_PARSER_EXPAT_ENABLE_SUPPORT)

#if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

//-- Data Members
#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

//-- Interface Realized and/or Parent.
#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#ifndef MXG_IXMLPARSER_H
#include "Xml/IXmlParser.h"
#endif

//======= EXPAT SPECIFIC =======
#include "expat.h"

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IXmlParserMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CXmlParserExpat
//==============================================================================
//<GROUP XML_CLASSES>
//
// Description:
//   This class implements the XML Parser abstraction layer for the expat
//   library.
//
// #Limitations:#
//  - The input MUST be encoded in UTF-8.
//
// Location:
//   Xml/CXmlParserExpat.h
//
// See Also:
//   IXmlParser
//
//==============================================================================
class CXmlParserExpat : protected CEComUnknown,
                        private IXmlParser
{
public:

//-- Hidden Methods.
protected:
    //-- << Static Initializer >>

    static mxt_result InitializeCXmlParserExpat();
    static void FinalizeCXmlParserExpat();

    //-- Friendship declaration.
    friend mxt_result InitializeCXmlParserExpat();
    friend void FinalizeCXmlParserExpat();

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

    //-- << IXmlParser interface >>

    virtual mxt_result Parse(IN IXmlParserMgr* pXmlParserMgr,
                             IN const uint8_t* puXmlBuffer,
                             IN const uint32_t uBufferSize);
    virtual void Reset();

    //-- << expat Handlers >>

    // Summary:
    //  Tells that the XML document is not encoded in a supported encoding.
    static int EvUnknownFileEncodingHandler(IN void* pEncodingHandlerData,
                                            IN const char* pszName,
                                            INOUT XML_Encoding* pstInfo);

    // Summary:
    //  Tells that the XML document is not "stand alone".
    static int EvDocumentNotStandAloneHandler(IN void* pUserData);

    //-- The methods listed below are only entry points for the expat library, they
    //-- all do only one thing, call an internal helper. This is why they are not
    //-- documented.
    //--
    //-- Methods:
    //--     - EvNamespaceStartHandler
    //--     - EvNamespaceEndHandler
    //--     - EvElementStartHandler
    //--     - EvElementEndHandler
    //--     - EvCharacterDataHandler
    //--     - EvCdataSectionStartHandler
    //--     - EvCdataSectionEndHandler
    //--     - EvCommentHandler
    //--     - EvProcessingInstructionsHandler
    //--     - EvUnparsedEntityHandler
    //--     - EvNotationDeclarationHandler
    //--     - EvDefaultHandler
    //--
    //-- The pUserData parameter is a pointer to this class's instance on which the
    //-- helpers are called. See the helpers's documentation for more details on the
    //-- events and other parameters.

    static void EvNamespaceStartHandler(IN void* pUserData,
                                        IN const char* pszPrefix,
                                        IN const char* pszUri);
    static void EvNamespaceEndHandler(IN void* pUserData, IN const char* pszPrefix);
    static void EvElementStartHandler(IN void* pUserData,
                                      IN const char* pszName,
                                      IN const char** ppszAttributes);
    static void EvElementEndHandler(IN void* pUserData, IN const char* pszName);
    static void EvCharacterDataHandler(IN void* pUserData, IN const char* psz, IN int nLength);
    static void EvCdataSectionStartHandler(IN void* pUserData);
    static void EvCdataSectionEndHandler(IN void* pUserData);
    static void EvCommentHandler(IN void* pUserData, IN const char* pszComment);
    static void EvProcessingInstructionsHandler(IN void* pUserData,
                                                IN const char* pszTarget,
                                                IN const char* pszData);
    static void EvUnparsedEntityHandler(IN void* pUserData,
                                        IN const char* pszEntityName,
                                        IN const char* pszBase,
                                        IN const char* pszSystemId,
                                        IN const char* pszPublicId,
                                        IN const char* pszNotationName);
    static void EvNotationDeclarationHandler(IN void* pUserData,
                                             IN const char* pszNotationName,
                                             IN const char* pszBase,
                                             IN const char* pszSystemId,
                                             IN const char* pszPublicId);
    static void EvDefaultHandler(IN void* pUserData, IN const char* psz, IN int nLength);

    //-- << Helpers >>

    // Summary:
    //  Reports the start of a XML namespace.
    void EvNamespaceStartHandlerHelper(IN const char* pszPrefix, IN const char* pszUri);

    // Summary:
    //  Reports the end of a XML namespace.
    void EvNamespaceEndHandlerHelper(IN const char* pszPrefix);

    // Summary:
    //  Reports the start of a XML element.
    void EvElementStartHandlerHelper(IN const char* pszName, IN const char** ppszAttributes);

    // Summary:
    //  Reports the end of a XML element.
    void EvElementEndHandlerHelper(IN const char* pszName);

    // Summary:
    //  Reports character data enclosed in an element.
    void EvCharacterDataHandlerHelper(IN const char* psz, IN int nLength);

    // Summary:
    //  Reports the start of a CDATA section.
    void EvCdataSectionStartHandlerHelper();

    // Summary:
    //  Reports the end of a CDATA section.
    void EvCdataSectionEndHandlerHelper();

    // Summary:
    //  Reports a XML comment.
    void EvCommentHandlerHelper(IN const char* pszComment);

    // Summary:
    //  Reports XML processing instructions definition.
    void EvProcessingInstructionsHandlerHelper(IN const char* pszTarget, IN const char* pszData);

    // Summary:
    //  Reports an unparsed XML entity.
    void EvUnparsedEntityHandlerHelper(IN const char* pszEntityName,
                                       IN const char* pszBase,
                                       IN const char* pszSystemId,
                                       IN const char* pszPublicId,
                                       IN const char* pszNotationName);

    // Summary:
    //  Reports a XML notation declaration.
    void EvNotationDeclarationHandlerHelper(IN const char* pszNotationName,
                                            IN const char* pszBase,
                                            IN const char* pszSystemId,
                                            IN const char* pszPublicId);

    // Summary:
    //  Reports events not reported elsewhere.
    void EvDefaultHandlerHelper(IN const char* psz, IN int nLength);

    // Summary:
    //  Splits a string consisting of "Namespace URI<sep>Local Name" into its components.
    void SplitExpandedName(IN const char* pszExpandedName,
                           OUT const char*& rpszNamespaceUri,
                           OUT const char*& rpszLocalName);

    // Summary:
    //  Restores the separators that have been replaced by '\0' in SplitExpandedName().
    void RestoreExpandedNamesSeparator();

    // Summary:
    //  Clears all known Namespaces from the internal state.
    void ClearNamespaceUris();

    // Summary:
    //  Assigns all expat's handlers to the instance of this class in order to
    //  manage expat events.
    void SetupExpatHandlers();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CXmlParserExpat();

    // Summary:
    //  Destructor.
    virtual ~CXmlParserExpat();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CXmlParserExpat(IN const CXmlParserExpat& rSrc);

    // Summary:
    //  Assignment Operator.
    CXmlParserExpat& operator=(IN const CXmlParserExpat& rSrc);

//-- Hidden Data Members
protected:
private:

    struct SNamespace
    {
        const char* pszNamespacePrefix;         // Namespace prefix.
        const char* pszNamespaceUri;            // Namespace Uri.
    };

    // The manager that shall receive the parsing events.
    IXmlParserMgr* m_pParserMgr;

    // The expat Parser instance.
    XML_Parser m_pParser;

    // This list will keep the new namespace prefix and URIs associated to
    // an element.
    CList<SNamespace>  m_lstSNamespaces;

    // This list will keep the pointers to the separators that have been
    // replaced by '\0' and that must been restored.
    CList<char*> m_lstSeparatorReplacements;

    // The supported encoding.
    static const char* const ms_pszSUPPORTED_ENCODING;

    // The separator character for expanded namespaces.
    static const char ms_cNAMESPACE_SEPARATOR;
};

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif //-- #if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

#endif //-- #if defined(MXD_XML_PARSER_EXPAT_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CXMLPARSEREXPAT_H
