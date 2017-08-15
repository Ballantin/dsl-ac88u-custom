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
#ifndef MXG_IXMLDOCUMENTMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXMLDOCUMENTMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class IXmlDocument;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IXmlDocumentMgr
//========================================
//<GROUP XML_CLASSES>
//
// Description:
//  This interface represents the document related events that can be reported
//  by an IXmlDocument.
//
//  This manager interface can be set on the IXmlDocument by using the
//  SetDocumentManager API.
//
// Location:
//   Xml/IXmlDocumentMgr.h
//
// See Also:
//   IXmlDocument::SetDocumentManager
//
//==============================================================================
class IXmlDocumentMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==========================================================================
    //==
    //==  EvChangedDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the IXmlDocument was changed.
    //
    //  Parameters:
    //    pDocument:
    //      Pointer to the document that was changed.
    //
    //  Description:
    //      Reports that the IXmlDocument was changed. This event will be
    //      reported every time that the document is changed.
    //
    //  See Also:
    //      IXmlDocument
    //
    //==========================================================================
    virtual void EvChangedDocument(IN IXmlDocument* pDocument) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IXmlDocumentMgr() {};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IXmlDocumentMgr() {};


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IXmlDocumentMgr(IN const IXmlDocumentMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IXmlDocumentMgr& operator=(IN const IXmlDocumentMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IXMLDOCUMENTMGR_H

