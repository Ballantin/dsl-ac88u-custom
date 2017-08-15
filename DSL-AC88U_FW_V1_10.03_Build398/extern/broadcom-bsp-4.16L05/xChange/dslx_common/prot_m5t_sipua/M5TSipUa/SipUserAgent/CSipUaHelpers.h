//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPUAHELPERS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPUAHELPERS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT to use this class.
#endif

#if !defined(MXG_CVECTOR_H)
#include "Cap/CVector.h"
#endif

#if !defined(MXG_METHOD_H)
#include "SipParser/Method.h"
#endif

//-- Data Members

//-- Interface Realized and/or Parent

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CHeaderList;
class CSipContextTerminator;
class CSipMessageBody;
class ISipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipUaHelpers
//==============================================================================
//
// Description:
//   This class provides a simple access to many helper classes to ease the use
//   of the SIP stack. As such, it delegates most of its operations to its
//   internal helper components.
//
// Location:
//   SipUserAgent/CSipUaHelpers.h
//
// See Also:
//   CSipContextTerminator
//
//==============================================================================
class CSipUaHelpers
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

//M5T_INTERNAL_USE_BEGIN
    // Summary:
    //  Constructor.
    CSipUaHelpers();

    // Summary:
    //  Destructor.
    ~CSipUaHelpers();
//M5T_INTERNAL_USE_END

    // Summary:
    //  Terminates a context gracefully but immediately.
    mxt_result TerminateContext(IN ISipContext* pContext);

    // Summary:
    //  Adds a SIP method to a list for which the transaction will be terminated
    //  gracefully.
    mxt_result AddGracefullyTerminatedClientRequest(IN ESipMethod eRequestMethod);

    // Summary:
    //  Removes a SIP method from a list for which the transaction will be
    //  terminated gracefully.
    mxt_result RemoveGracefullyTerminatedClientRequest(IN ESipMethod eRequestMethod);

    // Summary:
    //  Returns the current list of SIP methods for which existing client
    //  transactions will be terminated gracefully.
    const CVector<ESipMethod>& GetGracefullyTerminatedClientRequests() const;

    // Summary:
    //  Customizes the headers and body of potential request sent to terminate the context.
    mxt_result CustomizePacketForContextTermination(IN ISipContext* pContext,
                                                    IN ESipMethod eMethod,
                                                    IN TOA CHeaderList* pExtraHeaders,
                                                    IN TOA CSipMessageBody* pBody);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipUaHelpers(IN const CSipUaHelpers& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipUaHelpers& operator=(IN const CSipUaHelpers& rSrc);

//-- Hidden Data Members
protected:
private:

    CSipContextTerminator* m_pContextTerminator;
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPUAHELPERS_H

