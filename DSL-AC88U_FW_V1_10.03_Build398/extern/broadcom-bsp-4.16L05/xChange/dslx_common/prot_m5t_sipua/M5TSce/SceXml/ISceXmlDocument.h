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
#ifndef MXG_ISCEXMLDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEXMLDOCUMENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
//-------------------------
class CBlob;
class CMimeObject;
class CSipMessageBody;
class IXmlDocument;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
MX_DECLARE_ECOM_IID(ISceXmlDocument);

//M5T_INTERNAL_USE_END

//==============================================================================
//== Class: ISceXmlDocument
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Description:
//   This is a base interface for an XML document. It defines basic operations
//   that should be permitted on every XML document.
//
// Location:
//   SceXml/ISceXmlDocument.h
//
//==============================================================================
class ISceXmlDocument : public IEComUnknown
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
    MX_DECLARE_ECOM_GETIID(ISceXmlDocument);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  VerifyDocumentCleanup
    //==
    //==========================================================================
    //
    //  Summary:
    //      Checks if the document and associated allocator need to be
    //      cleaned-up and cleans them up if necessary.
    //
    //  Returns:
    //      - resS_OK upon successfull cleanup.
    //      - resSW_NOTHING_DONE when cleanup is not required.
    //      - Other failure codes if a the cleanup failed.
    //
    //  Description:
    //      Checks if the document and associated allocator need to be
    //      cleaned-up. If it is the case a cleanup is performed and resS_OK is
    //      returned. Otherwise resFE_INVALID_STATE is returned and no cleanup
    //      is performed.
    //
    //==========================================================================
    virtual mxt_result VerifyDocumentCleanup() = 0;

    //==========================================================================
    //==
    //==  Parse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Finds and parses the XML document found in the MIME object.
    //
    //  Parameters:
    //    rMimeObject:
    //      Reference to the MIME object from which to extract the information.
    //
    //  Returns:
    //      - resS_OK upon success;
    //      - resFE_INVALID_ARGUMENT if Parse was unable to find an
    //        expected payload type as part of the MIME object;
    //      - Other failure codes if a parsing error occurs.
    //
    //  Description:
    //      This method is a all-in-one parsing function that performs the
    //      following:
    //        - First, it searches rMimeObject for the first expected part type
    //          it can find. If rMimeObject directly holds the information, then
    //          it is directly used. If rMimeObject corresponds to a multipart
    //          mime object, then all sub-parts are traversed to find the first
    //          expected part type. If no expected document type is found, an
    //          error resFE_INVALID_ARGUMENT is returned.
    //        - Once the part is found, it is parsed as an XML document.
    //
    //  See Also:
    //      Serialize
    //
    //==========================================================================
    virtual mxt_result Parse(IN const CMimeObject& rMimeObject) = 0;

    //==========================================================================
    //==
    //==  Serialize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Serializes the XML document into a MIME object.
    //
    //  Parameters:
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
    //==========================================================================
    virtual void Serialize(OUT GO CMimeObject*& rpMimeObject) = 0;

    //==========================================================================
    //==
    //==  Parse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Finds and parses the authorization document found in the
    //      CSipMessageBody object.
    //
    //  Parameters:
    //    rMessageBody:
    //      Reference to the CSipMessageBody object from which to extract the
    //      information.
    //
    //  Returns:
    //      - resS_OK upon success;
    //      - resFE_INVALID_ARGUMENT if Parse was unable to find an expected
    //        payload type as part of the CSipMessageBody object.
    //      - Other failure codes if a parsing error occurs.
    //
    //  Description:
    //      This method is a all-in-one parsing function that performs the
    //      following:
    //        - First, it searches rMessageBody for the first expected part type
    //          it can find. If rMessageBody directly holds the information,
    //          then it is directly used. If rMessageBody corresponds to a
    //          multipart mime object, then all sub-parts are traversed to find
    //          the first expected part type. If no expected document type is
    //          found, an error resFE_INVALID_ARGUMENT is returned.
    //        - Once the part is found, it is parsed as an XML document.
    //
    //  See Also:
    //      Serialize
    //
    //==========================================================================
    virtual mxt_result Parse(IN const CSipMessageBody& rMessageBody) = 0;

    //==========================================================================
    //==
    //==  Serialize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Serializes the XML document into a CSipMessageBody object.
    //
    //  Parameters:
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
    //  See Also:
    //      Parse
    //
    //==========================================================================
    virtual void Serialize(OUT GO CSipMessageBody*& rpMessageBody) = 0;

    //==========================================================================
    //==
    //==  Parse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Parses the XML document from a pointer to the actual XML data.
    //
    //  Parameters:
    //    rpcPos:
    //      Reference to a Pointer to a C-style string that must point to the
    //      start of the XML document. When this method exits, rpcPos will
    //      point to the byte following the end of the XML document.
    //
    //    uLength:
    //      The length of rpcPos. When 0 is passed it means that rpcPos is NULL
    //      terminated. Default value is 0.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Parses information from raw XML information.
    //
    //  See Also:
    //      Serialize
    //
    //==========================================================================
    virtual mxt_result Parse(INOUT const char*& rpcPos, IN unsigned int uLength = 0) = 0;

    //==========================================================================
    //==
    //==  Serialize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Serializes the XML in a blob.
    //
    //  Parameters:
    //    rBlob:
    //      Reference to a CBlob into which the information is serialized (or
    //      written) in raw XML form. The XML information is written at the
    //      current insertion point of the blob.
    //
    //  Description:
    //      Serializes the information in raw XML format into a CBlob.
    //
    //  See Also:
    //      GetXmlDocument
    //
    //==========================================================================
    virtual void Serialize(INOUT CBlob& rBlob) = 0;

    //==========================================================================
    //==
    //==  IsCleanupRequired
    //==
    //==========================================================================
    //
    //  Summary:
    //      Verifies if the XML document can optimize the memory it uses.
    //
    //  Parameters:
    //    ruNumBytesSaved:
    //      Returns the number of bytes that could be saved if Cleanup is
    //      called.
    //
    //  Returns:
    //      True if memory can be saved by calling Cleanup, false otherwise.
    //
    //  Description:
    //      Verifies if the memory used by the XML representation of the XML
    //      document can be optimized to save memory.
    //
    //  See Also:
    //      Cleanup
    //
    //==========================================================================
    virtual bool IsCleanupRequired(OUT unsigned int& ruNumBytesSaved) = 0;

    //==========================================================================
    //==
    //==  Cleanup
    //==
    //==========================================================================
    //
    //  Summary:
    //      Optimizes the memory used by the XML document.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Optimizes the memory used by the XML document.
    //
    //      Callers must make sure they do not keep pointers to internal
    //      CXmlElement or IXmlDocument before calling Cleanup as this will
    //      invalidate any such pointers.
    //
    //      Note that to cleanup, the XML document is serialized and then
    //      parsed over again. Cleanup should only be called when
    //      IsCleanupRequired returns true, otherwise CPU resources are wasted
    //      for nothing.
    //
    //  See Also:
    //      IsCleanupRequired
    //
    //==========================================================================
    virtual mxt_result Cleanup() = 0;


//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceXmlDocument() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceXmlDocument() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceXmlDocument(IN const ISceXmlDocument& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceXmlDocument& operator=(IN const ISceXmlDocument& rSrc);


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

#endif //-- #ifndef MXG_ISCEXMLDOCUMENT_H
