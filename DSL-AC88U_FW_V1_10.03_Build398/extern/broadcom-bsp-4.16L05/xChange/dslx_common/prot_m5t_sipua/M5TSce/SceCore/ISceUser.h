//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCEUSER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEUSER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSipPacket;
class IEComUnknown;
class ISceUserConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceUser
//========================================
//<GROUP TID_PACKAGE_SCECORE_CLASSES>
//
// Description:
//  The ISceUser defines the interface of a user of the system. This
//  interface is used to get access to an ISceUserConfig associated
//  with the user if the request matches this user.
//
// Location:
//   SceCore/ISceUser.h
//
//==============================================================================
class ISceUser
{
//-- Friend Declarations.


//-- Published Interface.
public:

    //==========================================================================
    //==
    //==  CanHandleRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Verifies if this user is able to handle the provided request.
    //
    //  Parameters:
    //    rRequest:
    //      The incoming request.
    //
    //    rResponseCode:
    //      The response code to send.
    //
    //    rpUserConfig:
    //      Reference to a pointer to a user configuration object. This pointer
    //      will be valid upon return of this call if it return resS_OK.
    //
    //  Returns:
    //      - resS_OK: The request is for this user. The rpUserConfig
    //        parameter contains the configuration for this user for further
    //        processing the request.
    //
    //      - resFE_FAIL: The request was not for this user. The caller must
    //        not make any assumption about the validity or invalidity of
    //        rpUserConfig.
    //
    //
    //  Description:
    //      Provides the user with a request to see if this user can handle it.
    //      This method is not to immediately have the user handle the request
    //      but instead to get a user configuration object associated with this
    //      user for further processing by another component. If the user cannot
    //      handle the request because of external issues, like caller
    //      preferences, it will return the correct response code to send and
    //      all further processing of the request stops.
    //
    //==========================================================================
    virtual mxt_result CanHandleRequest(IN const CSipPacket& rRequest,
                                        OUT uint16_t& ruResponseCode,
                                        OUT IEComUnknown*& rpUserConfig) = 0;


//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceUser() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceUser() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceUser(IN const ISceUser& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceUser& operator=(IN const ISceUser& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_ISCEUSER_H

