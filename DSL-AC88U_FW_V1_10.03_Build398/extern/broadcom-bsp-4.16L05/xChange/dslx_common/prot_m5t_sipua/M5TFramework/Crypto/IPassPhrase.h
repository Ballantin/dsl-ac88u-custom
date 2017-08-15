//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_IPASSPHRASE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IPASSPHRASE_H
//M5T_INTERNAL_USE_END

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

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IPassPhrase
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Interface through which requests for pass phrases are done to decrypt
//   an object stored in PEM format.
//
// Description:
//   This is the interface through which requests for pass phrases are done to
//   to decrypt an object stored in PEM format.
//
// Location:
//   Crypto/IPassPhrase.h
//
// See Also:
//
//==EDOC========================================================================
class IPassPhrase
{
public:

    //==SDOC====================================================================
    //==
    //==  GetPassPhrase
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the pass phrase associated with a PEM object.
    //
    //  Parameters:
    //    opqPassPhraseParameter:
    //      The parameter associated with the pass phrase context.
    //
    //    pPassPhrase:
    //      A container for the retrieved pass phrase.
    //
    //  Returns:
    //      An error if unable to provide the pass phrase.
    //
    //  Description:
    //      Called to retrieve the pass phrase associated with a PEM object.
    //      This pass phrase is used to decrypt the PEM object.
    //
    //==EDOC====================================================================
    virtual mxt_result GetPassPhrase(IN mxt_opaque opqPassPhraseParameter,
                                     OUT CBlob* pPassPhrase) = 0;
//M5T_INTERNAL_USE_BEGIN
protected:
    //==SDOC====================================================================
    //==
    //==  IPassPhase
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Default Constructor.
    //
    //==EDOC====================================================================
    IPassPhrase() {}

    //==SDOC====================================================================
    //==
    //==  ~IPassPhase
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==EDOC====================================================================
    virtual ~IPassPhrase() {}
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_IPASSPHRASE_H

