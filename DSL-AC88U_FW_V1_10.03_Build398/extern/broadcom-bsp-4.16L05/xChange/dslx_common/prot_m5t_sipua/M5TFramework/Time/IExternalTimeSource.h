//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_IEXTERNALTIMESOURCE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IEXTERNALTIMESOURCE_H
//M5T_INTERNAL_USE_END

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

class CTime;

//===========================================================================
//====  CONSTANTS + DEFINES  ================================================
//===========================================================================

//==SDOC=====================================================================
//
//  class IExternalTimeSource
//
//===========================================================================
//<GROUP TIME_CLASSES>
//
//  Summary:
//      Base class to be inherited by every class that wants to be a time source.
//
//  Description:
//      IExternalTimeSource is a base class that must be inherited by every
//      class that wants to become a time source, e.g., CSntpClient.
//
//==EDOC=====================================================================
class IExternalTimeSource
{
// Published Interface
//--------------------
public:

    //==SDOC=====================================================================
    //==
    //==  GetTimeS
    //==
    //===========================================================================
    //
    //  Summary:
    //      Gets the time.
    //
    //  Parameters:
    //      rTime:
    //          Structure used to return the time.
    //
    //  Returns:
    //      A mxt_result set to resS_OK or resFE_FAIL.
    //
    //  Description:
    //      Gets the time from the source.
    //
    //  Note:
    //      This method is synchronized and may take some time to return.
    //
    //==EDOC=====================================================================
    virtual mxt_result GetTimeS(OUT CTime& rTime) = 0;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:
    // Summary:
    //  Constructor.
    //---------------
    IExternalTimeSource() {};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IExternalTimeSource() {};
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // __IEXTERNALTIMESOURCE_H


