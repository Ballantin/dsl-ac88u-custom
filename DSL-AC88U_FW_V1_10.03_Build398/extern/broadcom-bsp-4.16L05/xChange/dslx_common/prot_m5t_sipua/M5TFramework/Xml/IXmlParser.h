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
#ifndef MXG_IXMLPARSER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXMLPARSER_H
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
#include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IXmlParserMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IXmlParser);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IXmlParser
//========================================
//<GROUP XML_CLASSES>
//
// Description:
//  This defines the minimal interface to abstract a simple SAX-based XML
//  parser.
//
//  As the XML is being parsed, the parser reports events through an
//  IXmlParserMgr.
//
//#Requirements#
//
//  - Parsing of UTF-8 XML encoded files only. Other encodings should be treated
//    as errors.
//  - Parsed documents must all be standalone.
//  - Namespaces must be supported and returned in the reported events.
//  - Namespace declarations must not be reported as attributes to elements.
//  - XML declaration must not be reported.
//  - CDATA must be ignored and is not reported.
//  - Comments must be ignored and are not reported.
//
// Location:
//   Xml/IXmlParser.h
//
// See Also:
//   IXmlParserMgr, IXmlWriter
//
//==============================================================================
class IXmlParser : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IXmlParser);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  Parse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Parses a buffer containing an XML file.
    //
    //  Parameters:
    //    pXmlParserMgr:
    //      Pointer to the manager to which events are reported.
    //
    //    puXmlBuffer:
    //      Pointer to the buffer containing the XML document to parse.
    //
    //    uBufferSize:
    //      Number of bytes in puXmlBuffer
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Parses a complete XML document. This performs SAX-like parsing, with
    //      events being reported through pXmlParserMgr.
    //
    //  See Also:
    //      IXmlParserMgr
    //
    //==========================================================================
    virtual mxt_result Parse(IN IXmlParserMgr* pXmlParserMgr,
                             IN const uint8_t* puXmlBuffer,
                             IN const uint32_t uBufferSize) = 0;

    //==========================================================================
    //==
    //==  Reset
    //==
    //==========================================================================
    //
    //  Summary:
    //      Resets the XML parser internal state.
    //
    //  Description:
    //      Resets the XML parser internal state. Calling this method resets 
    //      the parser internal state to the initial state it had right after 
    //      creation. This allows this parser instances to be reused to parse
    //      multiple files.
    //
    //==========================================================================
    virtual void Reset() = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IXmlParser() {};

    // Summary:
    //  Destructor.
    virtual ~IXmlParser() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IXmlParser(IN const IXmlParser& rSrc);

    // Summary:
    //  Assignment Operator.
    IXmlParser& operator=(IN const IXmlParser& rSrc);

//-- Hidden Data Members
protected:
private:
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_IXMLPARSER_H

