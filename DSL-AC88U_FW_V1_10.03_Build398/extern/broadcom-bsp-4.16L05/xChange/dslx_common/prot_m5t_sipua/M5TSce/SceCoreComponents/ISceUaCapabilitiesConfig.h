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
#ifndef MXG_ISCEUACAPABILITIESCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEUACAPABILITIESCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSceSipFeatureSet;

//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceUaCapabilitiesConfig);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceUaCapabilitiesConfig
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines the required operations to set and get User-Agent
//   Capabilities.
//
// Description:
//  This interface defines the required operations to set and get User-Agent
//  Capabilities on components supporting this interface.
//  
// Location:
//   SceCoreComponents/ISceUaCapabilitiesConfig.h
//
//==============================================================================
class ISceUaCapabilitiesConfig : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceUaCapabilitiesConfig);
//M5T_INTERNAL_USE_END    

    //==========================================================================
    //==
    //==  SetUaCapabilities
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures or updates the User-Agent Capabilities associated with
    //      the component or configuration object.
    //
    //  Parameters:
    //    pCapabilities:
    //      Pointer to a SIP feature-set object that contains the capabilities.
    //      May be NULL, in which case the current capabilities are removed.
    //      Ownership of the pointer is taken.
    //
    //  Returns:
    //      Always a success.
    //
    //  Description:
    //      This method is used to configure User-Agent capabilities, as defined
    //      in RFC 3840. The capabilities are configured through a
    //      CSipFeatureSet. The configured capabilities are used in the Contact
    //      header of outgoing requests. In the REGISTER requests, the
    //      capabilities found in the Contact can be used by the server to make
    //      its routing decisions. In other requests, the capabilities can be
    //      used by other endpoints to know exactly what the other peer supports
    //      and act accordingly.
    //
    //      The +sip.instance capability is ignored here. To use it, the
    //      ISceUserConfig::SetInstanceId method must be called instead.
    //
    //  See Also:
    //      GetUaCapabilities, ISceUserConfig::SetInstanceId()
    //
    //==========================================================================
    virtual mxt_result SetUaCapabilities(IN TO CSceSipFeatureSet* pCapabilities) = 0;


    //==========================================================================
    //==
    //==  GetUaCapabilities
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured UA Capabilities.
    //
    //  Returns:
    //      A pointer to the configured feature-set. May be NULL if no
    //      capabilities were configured.
    //
    //  Description:
    //      Gets the configured UA capabilities.
    //
    //  See Also:
    //      SetUaCapabilities
    //
    //==========================================================================
    virtual const CSceSipFeatureSet* GetUaCapabilities() const = 0;

//-- Hidden Methods
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceUaCapabilitiesConfig() {};

    // Summary:
    //  Destructor.
    virtual ~ISceUaCapabilitiesConfig() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    ISceUaCapabilitiesConfig(IN const ISceUaCapabilitiesConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceUaCapabilitiesConfig& operator=(IN const ISceUaCapabilitiesConfig& rSrc);
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEUACAPABILITIESCONFIG_H

