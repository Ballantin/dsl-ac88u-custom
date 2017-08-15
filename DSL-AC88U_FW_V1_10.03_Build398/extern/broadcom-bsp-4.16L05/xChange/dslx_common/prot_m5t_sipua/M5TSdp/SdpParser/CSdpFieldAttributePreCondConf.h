//==============================================================================
//==============================================================================
//
//   Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSDPFIELDATTRIBUTEPRECONDCONF_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEPRECONDCONF_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXD_CSDPFIELDATTRIBUTEPRECOND_D
#include "SdpParser/CSdpFieldAttributePreCond.h"
#endif

// BRCM_CUSTOM - [add] Precondition support
#ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSdpFieldAttributePreCondConf
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements a CONF precondition field attribute.
//
// Description:
//   This class represents a CONF precondition field attribute.
//
// Location:
//   SdpParser/CSdpFieldAttributePreCondConf.h
//
//==============================================================================
class CSdpFieldAttributePreCondConf : public CSdpFieldAttributePreCond
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSdpFieldAttributePreCondConf();

    // Summary:
    //  Destructor.
    virtual ~CSdpFieldAttributePreCondConf();

    // Summary:
    //  Copy Constructor.
    CSdpFieldAttributePreCondConf(IN const CSdpFieldAttributePreCondConf& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFieldAttributePreCondConf& operator=(IN const CSdpFieldAttributePreCondConf& rSrc);


    //-- << Stereotype >>

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>


//-- Hidden Data Members
protected:
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

// BRCM_CUSTOM - [add] Precondition support
#endif //-- #ifdef MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support

#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTEPRECONDCONF_H

