//==SDOC========================================================================
//==============================================================================
//
//        Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CXMLPARSER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CXMLPARSER_H
//M5T_INTERNAL_USE_END

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_XML_DEPRECATED_ENABLE_SUPPORT
#endif

// Activate this file only if Xml is required.
//---------------------------------------------
#if defined(MXD_XML_DEPRECATED_ENABLE_SUPPORT)

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//======= EXPAT SPECIFIC =======
#include "expat.h"

// Interface Realized & Parent
//--------------------------------------

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace
//-----------------------------------------------
class IXmlParserHandler;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC=====================================================================
//==
//==  Class: CXmlParser
//==
//===========================================================================
//<GROUP XML_CLASSES>
//
//  Summary:
//      Implements a XML parser.
//
//  Description:
//      Class implementing an XML parser. Parses an XML buffer by using the
//      Parse method.
//
//==EDOC=====================================================================
class CXmlParser
{

// Published Interface
//-------------------------
public:
    // << Constructors / Destructors >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  CXmlParser
    //==
    //==========================================================================
    //
    // Summary:
    //  Constructor.
    //
    // Description:
    //  Constructor
    //
    //==EDOC====================================================================
    CXmlParser();

    //==SDOC====================================================================
    //==
    //==  ~CXmlParser
    //==
    //==========================================================================
    //
    // Summary:
    //  Destructor.
    //
    // Description:
    //  Destructor
    //
    //==EDOC====================================================================
    virtual ~CXmlParser() {};

    // Summary:
    // Parsing method
    //-----------------
    mxt_result Parse(IN IXmlParserHandler* pXmlParserHandler,
                     IN const uint8_t* puXmlBuffer,
                     IN const uint32_t uBufferSize);

    // Summary:
    // Parser skip element control.
    //------------------------------
    void SkipElementContent();

//M5T_INTERNAL_USE_BEGIN
// Internal Methods Definition
//-----------------------------
private:

    // Handler methods for the XML Parser
    //------------------------------------

    // Handler for start tags
    void StartElementHandler(IN const char *pszElementName,   // Element tag name
                             IN const char **ppcAttribute);  // Vector of attribute name and value
    static void StartElementHandler(IN void *opq,
                                    IN const char *pszElementName,
                                    IN const char **ppcAttribute)
    {
        MX_ASSERT(opq != NULL);
        reinterpret_cast<CXmlParser*>(opq)->StartElementHandler(pszElementName,ppcAttribute);
    }

    // Handler for end tags
    void EndElementHandler(IN const char *pszElementName);
    static void EndElementHandler(IN void *opq,IN const char *pszElementName)
    {
        MX_ASSERT(opq != NULL);
        reinterpret_cast<CXmlParser*>(opq)->EndElementHandler(pszElementName);
    }

    // Handler for text
    void CharacterDataHandler(IN const char* pcCharacterData, // Text string
                              IN int nSize);                  // Length of the text
    static void CharacterDataHandler(IN void *opq,IN const char* pcCharacterData,IN int nSize)
    {
        MX_ASSERT(opq != NULL);
        reinterpret_cast<CXmlParser*>(opq)->CharacterDataHandler(pcCharacterData,nSize);
    }

    // Handler for comments
    void CommentHandler(IN const char* pszComment);
    static void CommentHandler(IN void *opq, IN const char* pszComment)
    {
        MX_ASSERT(opq != NULL);
        reinterpret_cast<CXmlParser*>(opq)->CommentHandler(pszComment);
    }

    // Handler for any character not handled
    void DefaultHandler(IN const char* pcDefault,    // Text unhandled
                        IN int nSize);                // Length of the text
    static void DefaultHandler(IN void *opq, IN const char* pcDefault,IN int nSize)
    {
        MX_ASSERT(opq != NULL);
        reinterpret_cast<CXmlParser*>(opq)->DefaultHandler(pcDefault,nSize);
    }

    // Parser creation and deletion
    //------------------------------
    void CreateParser();
    void FreeParser();

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    CXmlParser(const CXmlParser& from);
    CXmlParser& operator=(const CXmlParser& from);

// Private Data Members
//---------------------
private:

    // XML Parser
    //------------
    XML_Parser      m_pXMLParser;

    // XML Parser manager
    IXmlParserHandler*  m_pXmlParserHandler;
    const uint8_t* m_puXmlBuffer;

    // Data members to allow skipping elements transparently.
    bool m_bSkipElement;
    uint32_t m_uSkipDepth;
    unsigned int m_uElementNameOffset;
    unsigned int m_uSkippedContentOffset;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_XML_DEPRECATED_ENABLE_SUPPORT)

#endif // #ifndef MXG_CXMLPARSER_H

