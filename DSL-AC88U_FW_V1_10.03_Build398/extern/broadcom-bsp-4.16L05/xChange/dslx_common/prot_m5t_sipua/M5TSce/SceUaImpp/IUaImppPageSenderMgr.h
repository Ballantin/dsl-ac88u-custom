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
#ifndef MXG_IUAIMPPPAGESENDERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPPAGESENDERMGR_H

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
class IUaImppPageSender;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppPageSenderMgr
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//    This interface defines the events that the IUaImppPageSender component
//    reports.
//
// Description:
//  This interface defines the events that the IUaImppPageSender component
//  reports. It must be implemented by the user of the IUaImppPageSender
//  component.
//
// Location:
//   SceUaImpp/IUaImppPageSenderMgr.h
//
// See Also:
//   IUaImppPageSender
//
//==============================================================================
class IUaImppPageSenderMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==========================================================================
    //==
    //==  EvResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a response was received to a MESSAGE request.
    //
    //  Parameters:
    //    pComponent:
    //      The pager component reporting the event, which is the same that was
    //      used to send the request.
    //
    //    opqTransaction:
    //      The opaque identifier that the application passed to SendCpim or
    //      SendIm.
    //
    //    uResponse:
    //      The received response as a SIP response value. Values between 200
    //      and 299 represent success responses, while higher values represent
    //      failures. Responses between 0 and 199 are not reported by the pager
    //      component.
    //
    //  Description:
    //      Reports that a final response was received to an instant message.
    //
    //==========================================================================
    virtual void EvResponse(IN IUaImppPageSender* pComponent,
                            IN mxt_opaque opqTransaction,
                            IN uint16_t uResponse) = 0;
 
//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IUaImppPageSenderMgr() {};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IUaImppPageSenderMgr() {};


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IUaImppPageSenderMgr(IN const IUaImppPageSenderMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IUaImppPageSenderMgr& operator=(IN const IUaImppPageSenderMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_IUAIMPPPAGESENDERMGR_H

