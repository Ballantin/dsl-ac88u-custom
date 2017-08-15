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
#ifndef MXG_IASYNCSOCKETQUALITYOFSERVICEOPTIONS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCSOCKETQUALITYOFSERVICEOPTIONS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_ECOM_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_ECOM_ENABLE_SUPPORT)

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncSocketQualityOfServiceOptions);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncSocketQualityOfServiceOptions
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Interface defining the socket options that are related to quality of
//    service.
//
//  Description:
//    This interface defines the socket options that are related to quality of
//    service.
//
//  Location:
//    Network/IAsyncSocketBufferSizeOptions.h
//
//  See Also:
//    IAsyncSocketUdpOptions
//    IAsyncSocketTcpOptions
//    IAsyncSocketBufferSizeOptions
//    IAsyncSocketWindowsGqosOptions
//
//==============================================================================
class IAsyncSocketQualityOfServiceOptions : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncSocketQualityOfServiceOptions);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  Set8021QUserPriority
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets a socket level option which enables/disables utilization
    //    of the 802.1Q user priority.
    //
    //  Parameters:
    //    bEnable:
    //      Determines whether the option is enabled or not.
    //
    //    uUserPriority:
    //      The value to use in the 802.1Q user priority field.
    //
    //  Returns:
    //    resS_OK: The option has been successfully set.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method sets a socket level option which enables/disables
    //    utilization of the 802.1Q user priority. If the option is enabled,
    //    then the 802.1Q user priority field is set to uUserPriority.
    //    This method can be called even if the socket is not bound yet.
    //
    //  Notes:
    //    This option is target dependent. If it is compiled for a target that
    //    does not support the option (like Nucleus), the execution of this
    //    method will output an error trace before returning with resS_OK.
    //
    //==========================================================================
    virtual mxt_result Set8021QUserPriority(IN bool bEnable,
                                            IN uint8_t uUserPriority) = 0;

    //==========================================================================
    //==
    //==  SetTos
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets a socket level option which sets the TOS byte field in
    //    the IP protocol header.
    //
    //  Parameters:
    //    uTos:
    //      The value of the TOS byte field for this socket.
    //
    //  Returns:
    //    resS_OK: The option has been successfully set.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method sets a socket level option which sets the TOS
    //    byte field in the IP protocol header.  This method can be called
    //    even if the socket is not bound yet.
    //
    //    #Windows platforms specific information:#
    //    For this option to work under Windows 2000, XP, and 2003 Server,
    //    the value <B>DisableUserTOSSetting</B> must be set to 0 in the registry.
    //
    //    It is located in the following key:
    //      HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/Tcpip/Parameters
    //
    //    The value is a REG_DWORD that must be set to 0 in order for the TOS
    //    flags to be set in the IP header. On Windows 2000 and some older
    //    builds of Windows XP, the value must be added. Windows must be
    //    rebooted for the changes to take effect. Additional information can
    //    be found here:
    //      http://support.microsoft.com/default.aspx?scid=kb;en-us;248611
    //
    //  Notes:
    //    This option is target dependent. If it is compiled for a target that
    //    does not support the option (like Windows CE), the execution of this
    //    method will output an error trace before returning with resS_OK.
    //
    //    In Windows CE 4.0, the function call succeeds but the TOS byte
    //    field in the IP protocol header will remain unchanged.
    //
    //    Under Nucleus, this method sets the DSCP bytefield instead of the
    //    TOS. It is impossible to set the TOS byte field.
    //
    //==========================================================================
    virtual mxt_result SetTos(IN uint8_t uTos) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncSocketQualityOfServiceOptions
    //==
    //==========================================================================
    //
    //  Summary:
    //    Constructor.
    //
    //  Description:
    //    Default Constructor.
    //
    //==========================================================================
    IAsyncSocketQualityOfServiceOptions() {}

    //==========================================================================
    //==
    //==  ~IAsyncSocketQualityOfServiceOptions
    //==
    //==========================================================================
    //
    //  Summary:
    //    Destructor.
    //
    //  Description:
    //    Destructor.
    //
    //==========================================================================
    virtual ~IAsyncSocketQualityOfServiceOptions() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncSocketQualityOfServiceOptions(const IAsyncSocketQualityOfServiceOptions& from);
    IAsyncSocketQualityOfServiceOptions& operator=(const IAsyncSocketQualityOfServiceOptions& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)


#endif // MXG_IASYNCSOCKETQUALITYOFSERVICEOPTIONS_H
