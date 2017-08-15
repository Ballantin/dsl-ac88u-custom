//===========================================================================
//==============================================================================
//
//      Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_CXMLWRITER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CXMLWRITER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_XML_DEPRECATED_ENABLE_SUPPORT
#endif

// Activate this file only if Xml is required.
//---------------------------------------------
#if defined(MXD_XML_DEPRECATED_ENABLE_SUPPORT)

// Interface Realized & Parent
//-----------------------------

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace
//-----------------------------------------------
class IXmlWriterOutputHandler;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==
//==  Class: CXmlWriter
//==
//==============================================================================
//<GROUP XML_CLASSES>
//
//  Summary:
//      Implements methods for writing an XML document.
//
//  Description:
//      Class implementing methods to write an XML document. A document can be
//      written tag by tag or have a large buffer containing the complete
//      document output at once.
//
//==============================================================================
class CXmlWriter
{
public:
    //<GROUP XML_ENUMS>
    //
    // Summary:
    //  Enumeration of the indentation modes.
    //----------------------------------------
    enum EIndentingMode
    {
        // Description:
        // Normal indentation mode. Uses a tab character for indentation.
        eINDENT,
        // Description:
        // Compact indentation mode.
        eCOMPACT
    };

    // Summary:
    //  Enumeration of the end of line modes.
    //----------------------------------------
    enum EEolMode
    {
        // Description:
        // eLF.
        eLF,
        // Description:
        // eCR.
        eCR,
        // Description:
        // eCRLF.
        eCRLF,
        // Description:
        // eNONE.
        eNONE
    };

// Published Interface
//-------------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //==========================================================================
    //==
    //==  CXmlWriter
    //==
    //==========================================================================
    //
    // Summary:
    //  Constructor.
    //
    // Description:
    //  Constructor
    //
    //==========================================================================
    CXmlWriter();

    //==========================================================================
    //==
    //==  CXmlWriter
    //==
    //==========================================================================
    //
    // Summary:
    //  Constructor.
    //
    // Description:
    //  Creates a XML writer that uses the specified output handler.
    //
    //==========================================================================
    CXmlWriter(IN IXmlWriterOutputHandler* pXmlWriterOutputHandler,
               IN EEolMode eEolMode = eLF);

    //==========================================================================
    //==
    //==  ~CXmlWriter
    //==
    //==========================================================================
    //
    // Summary:
    //  Destructor.
    //
    // Description:
    //  Destructor
    //
    //==========================================================================
    virtual ~CXmlWriter() {};

    // Summary:
    // Initialize method
    //--------------------
    void Initialize(IN IXmlWriterOutputHandler* pXmlWriterOutputHandler,
                    IN EEolMode eEolMode = eLF);

    // Summary:
    // Starts an XML Document
    //--------------------------------
    bool StartDocument(IN const char* pszVersion = "1.0",
                       IN const char* pszEncoding = "US-ASCII",
                       IN bool bStandAlone = true);

    // Summary:
    // Ends an XML Document.
    //-----------------------
    bool EndDocument();

    // Summary:
    // Writes an XML element.
    //------------------------
    bool WriteElement(IN const char* pszName,
                      IN const char* pszValue);

    //<COMBINE CXmlWriter::WriteElement@IN const char*@IN const char*>
    bool WriteElement(IN const char* pszName,
                      IN int nValue);

    //<COMBINE CXmlWriter::WriteElement@IN const char*@IN const char*>
    bool WriteElement(IN const char* pszName,
                      IN unsigned int uValue);

    // Summary:
    // Writes an XML attribute.
    //--------------------------
    bool WriteAttributes(IN const char* pszName,
                         IN const char* pszValue);

    //<COMBINE CXmlWriter::WriteAttributes@IN const char*@IN const char*>
    bool WriteAttributes(IN const char* pszName,
                         IN int nValue);

    //<COMBINE CXmlWriter::WriteAttributes@IN const char*@IN const char*>
    bool WriteAttributes(IN const char* pszName,
                         IN unsigned int uValue);


    // Starts and Ends XML Tags
    //------------------------

    // Summary:
    // Writes a start element tag.
    //-----------------------------
    bool StartElement(IN const char* pszName,
                      IN EIndentingMode eIndentingMode = eINDENT);

    // Summary:
    // Writes an end element tag.
    //----------------------------
    bool EndElement(IN const char* pszName,
                    IN EIndentingMode eIndentingMode = eINDENT);

    // Summary:
    // Writes an empty element tag.
    //------------------------------
    bool EmptyElement(IN const char* pszName,
                      IN EIndentingMode eIndentingMode = eINDENT);

    // Summary:
    // Writes the start of an element attribute.
    //-------------------------------------------
    bool StartElementAttribute(IN const char* pszName,
                               IN EIndentingMode eIndentingMode = eINDENT);

    // Summary:
    // Writes the end of an element attribute and the content value of
    // the element.
    //-----------------------------------------------------------------
    bool EndElementAttribute(IN const char* pszName,
                             IN EIndentingMode eIndentingMode = eINDENT,
                             IN const char* pszValue = NULL);

    // Summary:
    // Writes the end of an empty element attribute.
    //-----------------------------------------------
    bool EndEmptyElementAttribute(IN const char* pszName,
                                  IN EIndentingMode eIndentingMode = eINDENT);

//M5T_INTERNAL_USE_BEGIN
// Internal Methods Definition
//-----------------------------
private:

    // Indentation method
    //--------------------
    bool Indent();

    // End of line method.
    //---------------------
    bool WriteEol();

    // Writes out methods
    //-------------------
    bool Write(IN const char* pszValue);
    bool Write(IN int nValue);
    bool Write(IN unsigned int uValue);

    // Basic XML writer method
    //-------------------------
    bool Write(IN const char *pcBuffer,
               IN unsigned int uBufferSize);

// Private Data Members
//---------------------
private:
    unsigned int m_uIndentLevel;
    IXmlWriterOutputHandler* m_pXmlWriterOutputHandler;
    EEolMode m_eEolMode;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_XML_DEPRECATED_ENABLE_SUPPORT)

#endif // #ifndef MXG_CXMLWRITER_H

