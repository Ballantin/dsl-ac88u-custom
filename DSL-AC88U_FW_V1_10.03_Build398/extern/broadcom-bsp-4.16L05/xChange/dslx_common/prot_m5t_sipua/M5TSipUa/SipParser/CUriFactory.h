//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CURIFACTORY_H
#define MXG_CURIFACTORY_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members
//-----------------


//-- Interface Realized and/or Parent
//-------------------------------------


//-- Parameters Used by Value
//-----------------------------
#ifndef MXG_IURI_H
#include "SipParser/IUri.h" // ESpecialCharactersAllowed
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CString;
class CBlob;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CUriFactory
//======================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   This class has the responsability of instanciating the correct type of URI
//   when parsing. It looks at the scheme and then creates the correct parser.
//
//   It is also responsible to parse the URI once its scheme has been found, and
//   also handles the bracket enclosing setting.
//
//   IUri users MUST use this class to parse and serialize URIs. This is
//   required since it enables easy extension and support of new URI types.
//
// Location:
//   SipParser/CUriFactory.h
//
// See Also:
//   CSipUri, CAbsoluteUri, IUri
//
//==EDOC========================================================================
class CUriFactory
{
//-- Published Interface
//------------------------
public:

    //-- << Public methods >>
    //--------------------------------------------

    // Summary:
    //  Compares the scheme using the rule stated in RFC 2396
    //  (case-insensitive).
    //--------------------------------------------------------
    static bool CompareScheme( IN const char* szFirstScheme,
                               IN const char* szSecScheme );

    // Summary:
    //  Parses a URI, including the brackets, the scheme, and the body.
    //------------------------------------------------------------------
    static mxt_result ParseUri( IN IUri::ESpecialCharactersAllowed eAllowSpecials,
                                INOUT const char*& rpcPos,
                                OUT GO IUri*& rpUri );


//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // waring or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif
//M5T_INTERNAL_USE_END

    // Summary:
    //  Deactivated Constructor.
    //---------------------------
    CUriFactory();

    // Summary:
    //  Deactivated Destructor.
    //--------------------------
    virtual ~CUriFactory();

    // Summary:
    //  Copy constructor.
    //--------------------
    CUriFactory(const CUriFactory& rFrom);

    // Summary:
    //  Assignment operator.
    //-----------------------
    CUriFactory& operator=(const CUriFactory& rFrom);


//-- Hidden Data Members
//------------------------
protected:
    // Summary:
    //  Parses the scheme part.
    //--------------------------
    static mxt_result ParseScheme( INOUT const char*& rpcPos,
                                   OUT CString& rstrScheme );

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CURIFACTORY_H
