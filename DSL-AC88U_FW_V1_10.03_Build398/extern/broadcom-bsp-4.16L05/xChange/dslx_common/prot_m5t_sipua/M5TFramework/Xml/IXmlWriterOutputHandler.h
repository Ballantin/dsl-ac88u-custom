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
#ifndef MXG_IXMLWRITEROUTPUTHANDLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXMLWRITEROUTPUTHANDLER_H
//M5T_INTERNAL_USE_END

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

// Interface Realized & Parent
//--------------------------------------

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace
//-----------------------------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//
//  class IXmlWriterOutputHandler
//
//==============================================================================
//<GROUP XML_CLASSES>
//
//  Summary:
//      This file provides the interface for any class that wants to be an
//      XmlWriter Manager.
//
//  Description:
//      This file provides the interface for any class that wants to be an
//      XmlWriter Manager.
//
//==EDOC========================================================================
class IXmlWriterOutputHandler
{
// Published Interface
//--------------------
public:
    //==SDOC====================================================================
    //==
    //==  EvXmlWriterOutputHandlerWrite
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the XML output handler that it can write XML data to the
    //      desired destination.
    //
    //  Returns:
    //      True if the write operation is a success, false otherwise.
    //
    //  Parameters:
    //    pcBuffer:
    //      A pointer to the buffer containing the XML data to be written.
    //
    //    uBufferSize:
    //      The size of the XML buffer.
    //
    //  Description:
    //      Writes up to uBufferLen characters to an output source implemented
    //      by the XmlWriterOutputHandler. The uBufferLen contains the number of
    //      characters in the string, excluding the terminal NULL.
    //
    //==EDOC====================================================================
    virtual bool EvXmlWriterOutputHandlerWrite(IN const char* pcBuffer,
                                               IN unsigned int uBufferSize) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  IXmlWriterOutputHandler
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor
    //
    //  Description:
    //      Constructor
    //
    //==EDOC====================================================================
    IXmlWriterOutputHandler() {};

    //==SDOC====================================================================
    //==
    //==  ~IXmlWriterOutputHandler
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
    virtual ~IXmlWriterOutputHandler() {};

private:
    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    IXmlWriterOutputHandler(const IXmlWriterOutputHandler& from);
    IXmlWriterOutputHandler& operator=(const IXmlWriterOutputHandler& from);
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_IXMLWRITEROUTPUTHANDLER_H

