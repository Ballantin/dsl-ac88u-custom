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
#ifndef MXG_CSCEXMLPRIVATEHELPER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEXMLPRIVATEHELPER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCEPIDFDATA_H
#include "SceXml/ScePidfData.h"
#endif

#ifndef MXG_CXMLELEMENT_H
#include "Xml/CXmlElement.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CBlob;
class CMimeObject;
class CSipMessageBody;
class CSubAllocator;
class CTime;
class IXmlDocument;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceXmlPrivateHelper
//========================================
//
// Summary:
//  This class is a helper class for XML.
//
// Description:
//  This class implements static XML helper function to be used in the
//  SipClientEngine.
//
// Location:
//   SceXml/CSceXmlPrivateHelper.h
//
//==============================================================================
class CSceXmlPrivateHelper
{
//-- Friend Declarations.


//-- Published Interface.
public:

    // Summary:
    //  Checks if the document and associated allocator need to be cleaned-up
    //  and cleans them up if necessary.
    static mxt_result VerifyDocumentCleanupHelper(IN CSubAllocator* pAllocator,
                                                  IN const unsigned int uBlockSize,
                                                  INOUT IXmlDocument* pXmlDocument);


    // Summary:
    //  Verifies if the XML document can optimize the memory it uses.
    static bool IsCleanupRequiredHelper(IN CSubAllocator* pAllocator,
                                        IN const unsigned int uBlockSize,
                                        OUT unsigned int& ruNumBytesSaved);

    // Summary:
    //  Optimizes the memory used by the XML document.
    static mxt_result CleanupHelper(IN CSubAllocator* pAllocator,
                                    IN const unsigned int uBlockSize,
                                    INOUT IXmlDocument* pXmlDocument);

    // Summary:
    //  Finds and parses the XML document found in the MIME object.
    static mxt_result ParseHelper(IN const CMimeObject& rMimeObject,
                                  IN const char* pszMType,
                                  IN const char* pszMSubType,
                                  INOUT IXmlDocument* pDocument);

    // Summary:
    //  Serializes the XML document into a MIME object.
    static void SerializeHelper(IN IXmlDocument* pDocument,
                                IN const char* pszMType,
                                IN const char* pszMSubType,
                                OUT GO CMimeObject*& rpMimeObject);


    // Summary:
    //  Finds and parses the authorization document found in the
    //  CSipMessageBody object.
    static mxt_result ParseHelper(IN const CSipMessageBody& rMessageBody,
                                  IN const char* pszMType,
                                  IN const char* pszMSubType,
                                  INOUT IXmlDocument* pDocument);

    // Summary:
    //  Serializes the XML document into a CSipMessageBody object.
    static void SerializeHelper(IN IXmlDocument* pDocument,
                                IN const char* pszMType,
                                IN const char* pszMSubType,
                                OUT GO CSipMessageBody*& rpMessageBody);

    // Summary:
    //  Parses the XML document from a pointer to the actual XML data.
    static mxt_result ParseHelper(INOUT IXmlDocument* pDocument,
                                  INOUT const char*& rpcPos,
                                  IN unsigned int uLength = 0);

    // Summary:
    //  Serializes the XML in a blob.
    static void SerializeHelper(IN IXmlDocument* pDocument, INOUT CBlob& rBlob);

    // Summary:
    //  Sets the user input information for pParent.
    static CXmlElement* SetUserInputHelper(IN CXmlElement* pParent,
                                           IN EPidfUserInputValue eInputValue,
                                           IN const unsigned int* puIdleThreshold,
                                           IN const CTime* pLastInputTime,
                                           IN const CVector<CXmlElement::SIdentificationInfo>* pvestSequence);

    // Summary:
    //  Returns the user-input information assigned to pParent.
    static CXmlElement* GetUserInputHelper(IN CXmlElement* pParent,
                                           OUT EPidfUserInputValue* peInputState);

    // Summary:
    //  Gets the idle-threshold information associated with a user-input
    //  element.
    static mxt_result GetUserInputIdleThresholdHelper(IN const CXmlElement* pUserInputElement,
                                                      OUT unsigned int& ruIdleThreshold);

    // Summary:
    //  Gets the last input time information associated with a user-input
    //  element.
    static mxt_result GetUserInputLastInputTimeHelper(IN const CXmlElement* pUserInputElement,
                                                      OUT CTime& rLastInputTime);

    // Summary:
    //  Deletes all child elements of pParent that are not notes.
    static void DeleteAllNonNoteChild(IN CXmlElement* pParent);



//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>
    // Summary:
    //  Default Constructor.
    CSceXmlPrivateHelper();

    // Summary:
    //  Destructor.
    virtual ~CSceXmlPrivateHelper();

private:

    // Summary:
    //  This method tries to find a message of the specified type in pObject.
    static const CMimeObject* FindMessage(IN const char* pszMType,
                                          IN const char* pszMSubType,
                                          IN const CMimeObject* pObject);

    // Summary:
    //  This method tries to find a message of the specified type in pObject.
    static const CSipMessageBody* FindMessage(IN const char* pszMType,
                                              IN const char* pszMSubType,
                                              IN const CSipMessageBody* pObject);

//-- Hidden Data Members.
protected:
private:
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCEXMLPRIVATEHELPER_H

//M5T_INTERNAL_USE_END

