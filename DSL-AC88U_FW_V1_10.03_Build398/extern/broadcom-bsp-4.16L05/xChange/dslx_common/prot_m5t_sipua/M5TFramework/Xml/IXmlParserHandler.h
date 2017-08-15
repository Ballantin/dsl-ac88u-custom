//==SDOC=====================================================================
//===========================================================================
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
#ifndef MXG_IXMLPARSERHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXMLPARSERHANDLER_H
//M5T_INTERNAL_USE_END

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations.
//-----------------------
class CMarshaler;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//
//  class IXmlParserHandler
//
//==============================================================================
//<GROUP XML_CLASSES>
//
//  Summary:
//  Provides the interface for any class that wants to be an XML parser handler.
//
//  Description:
//
//  This file provides the interface for any class that wants to be an XML
//  Parser handler. In other words, this handler is called with the right method
//  according to the XML being parsed.
//
//  All events are synchronous.
//
//==EDOC========================================================================
class IXmlParserHandler
{
// Published Interface
//--------------------
public:
    //==SDOC====================================================================
    //==
    //==  EvXmlParserHandlerStartElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the parser handler that Start or End elements have been
    //      detected.
    //
    //  Parameters:
    //    pszElementName:
    //      The element's name.
    //
    //    pAttributes:
    //      Attributes pairs.
    //
    //    uOffset:
    //      Offset of the element.
    //
    //  Description:
    //      Handler for start and end elements tags. Attributes are marshaled in
    //      pairs. The attribute name followed by the attribute value. The
    //      offset is the number of bytes between the start element tag and the
    //      beginning of the buffer.
    //
    //==EDOC====================================================================
    virtual void EvXmlParserHandlerStartElement(IN const char* pszElementName,
                                                IN CMarshaler* pAttributes,
                                                IN const uint32_t uOffset) = 0;

    // <COMBINE IXmlParserHandler::EvXmlParserHandlerStartElement@IN const char*@IN CMarshaler*@IN const uint32_t>
    virtual void EvXmlParserHandlerEndElement(IN const char* pszElementName,
                                              IN const uint32_t uOffset) = 0;

    //==SDOC====================================================================
    //==
    //==  EvXmlParserHandlerCharacterData
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the parser handler that character data has been detected.
    //
    //  Parameters:
    //    pcText:
    //      The text string.
    //
    //    uTextSize:
    //      The size of the text string.
    //
    //  Description:
    //      Text handler. The string received is NOT null-terminated. A single
    //      block of contiguous text free of mark-ups may still result in a
    //      sequence of calls to this handler. If you are searching for a
    //      pattern in the text, it may be split across calls to this handler.
    //
    //==EDOC====================================================================
    virtual void EvXmlParserHandlerCharacterData(IN const char* pcText,
                                                 IN const unsigned int uTextSize) = 0;

    //==SDOC====================================================================
    //==
    //==  EvXmlParserHandlerComment
    //==
    //==========================================================================
    //
    //  Summary:
    //      Comment handler. The data is all text inside the comment delimiters.
    //
    //  Parameters:
    //    pszComments:
    //      Pointer to a string that holds the comment.
    //
    //  Description:
    //      Comment handler. The data is all text inside the comment delimiters.
    //
    //==EDOC====================================================================
    virtual void EvXmlParserHandlerComment(IN const char* pszComments) = 0;

    //==SDOC====================================================================
    //==
    //==  EvXmlParserHandlerDefault
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the parser handler that a default element has been
    //      detected.
    //
    //  Parameters:
    //    pcText:
    //      The text string.
    //
    //    uTextSize:
    //      The size of the text string.
    //
    //  Description:
    //      Default handler. Sets a handler for any character that would not be
    //      handled otherwise. This includes both data for which no handlers can
    //      be set and data that could be reported but currently has no handler
    //      set.
    //
    //==EDOC====================================================================
    virtual void EvXmlParserHandlerDefault(IN const char* pcText,
                                           IN const unsigned int uTextSize) = 0;

    //==SDOC====================================================================
    //==
    //==  EvXmlParserHandlerSkippedContent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the parser handler that a skipped content is available.
    //
    //  Parameters:
    //    pcSkippedContent:
    //      A pointer to a non NULL terminated skipped content.
    //
    //    uSize:
    //      The size of the skipped content.
    //
    //  Description:
    //      Skipped content handler. This handler is called only if
    //      CXmlParser::SkipElementContent was formerly called from
    //      EvXmlParserHandlerStartElement. It is called once for an entire
    //      skipped construct content.
    //
    //      A call to this handler is immediately followed by a call to
    //      EvXmlParserHandlerEndElement associated with the start element for
    //      which the content was to be skipped.
    //
    //==EDOC====================================================================
    virtual void EvXmlParserHandlerSkippedContent(IN const char* pcSkippedContent,
                                                  IN unsigned int uSize) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  IXmlParserHandler
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Constructor.
    //
    //==EDOC====================================================================
    IXmlParserHandler() {};

    //==SDOC====================================================================
    //==
    //==  ~IXmlParserHandler
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor
    //
    //  Description:
    //      Destructor
    //
    //==EDOC====================================================================
    virtual ~IXmlParserHandler() {};

private:
    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    IXmlParserHandler(const IXmlParserHandler& from);
    IXmlParserHandler& operator=(const IXmlParserHandler& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_IXMLPARSERHANDLER_H

