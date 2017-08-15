//==============================================================================
//==============================================================================
//
//              Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPTARGETREUSESVC_H
#define MXG_ISIPTARGETREUSESVC_H

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


#ifndef MXD_SIPSTACK_ENABLE_SIP_TARGET_REUSE_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_TARGET_REUSE_SVC_SUPPORT \
to be able to use this class.
#endif

#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h"
#endif

//-- Parameters used by Value
#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_NETWORKTOOLS_H
#include "Network/NetworkTools.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CHostPort;
class CSipUri;
class ISipTargetReuseMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID( ISipTargetReuseSvc );

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==
//==  mxt_PFNReuseTargetListModifier
//==
//==============================================================================
//
//  Summary:
//      Callback to list modifier.
//
//  Parameters:
//    rFqdn: The address being resolved.
//
//    rlstNaptrRecord: The list of NAPTR records.
//
//    opq: An opaque.
//
//  Description:
//      This data type defines a function prototype for a list modifier,
//      which can modify the list, removing or adding records, or simply
//      modifying the order of the records created by the resolution service.
//      The only restriction is that the modification be made without changing
//      execution context.
//
//
//==============================================================================
typedef void (*mxt_PFNTargetListModifier)(IN const CHostPort& rFqdn,
                                          INOUT CList<SNaptrRecord>& rlstNaptrRecord,
                                          IN mxt_opaque opq);



//==============================================================================
//== Class: ISipTargetReuseSvc
//==============================================================================
//
// Description:
//   This service implements the server location services functionalities
//   adding the target reuse capability. It performs the DNS resolution for
//   the next hop URI and allows to manipulate the resolved list of targets.
//   This service also remembers the selected target of the previous next hop
//   URI. When a next hop URI is resolved, the service looks for the saved target
//   in the new list and, if it is found, it is chosen as the selected
//   target. The application can always modify this behavior in many ways, it
//   can modify the list of targets, it can force a current target or it can
//   select the target with the highest priority.
//
//  #The ISipTargetReuseSvc is an ECOM object#
//
//  The ISipTargetReuseSvc is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipTargetReuseSvc</B>
//
//  <B>Interface Id: IID_ISipTargetReuseSvc</B>
//
//  A user can query the ISipContext to which this service is attached 
//  by calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipTargetReuseSvc.h
//
// See Also:
//
//==============================================================================
class ISipTargetReuseSvc : public IEComUnknown
{
//-- New type definition
public:
    // The structure representing target
    struct SResolvedTarget
    {
        CSocketAddr     m_sTargetAddress;           // The target IP address and port
        ESipTransport   m_eTransport;               // The transport to be used.
    };

//-- Published Interface
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipTargetReuseSvc);
    //M5T_INTERNAL_USE_END


    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  GetCurrentNextHop
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the selected next hop URI.
    //
    //  Parameters:
    //    rCurrentTarget:
    //      The selected next hop URI.
    //
    //  Returns:
    //    resS_OK: the next hop URI is available.
    //    resFE_FAIL: no next hop URI is available or the DNS resolution has
    //      not been completed.
    //
    //  Description:
    //      Returns the selected target for the next hop URI.
    //      This method returns an error if no next hop URI has been chosen and
    //      resolved yet.
    //
    //==========================================================================
    virtual mxt_result GetCurrentNextHop(OUT CSipUri & rCurrentTarget) = 0;


    //==========================================================================
    //==
    //==  GetResolvedTargets
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the list of resolved targets for the next hop URI.
    //
    //  Parameters:
    //    rlststTargets:
    //      Reference used to return the list of targets.
    //      The returned list will be empty on error conditions.
    //
    //  Returns:
    //    resS_OK:
    //      If the list is available.
    //    resFE_FAIL:
    //      If the list is not available (if no next hop URI has been chosen yet
    //      or if the resolution has not been completed yet).
    //
    //  Description:
    //      Gives access to the resolved targets for the next hop URI. The
    //      application can modify the target list using SetResolvedTargets.
    //      This method returns an error if no resolution has been done yet.
    //
    //==========================================================================
    virtual mxt_result GetResolvedTargets(OUT CList<SResolvedTarget>& rlststTargets) = 0;


    //==========================================================================
    //==
    //==  SetResolvedTargets
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the list of resolved targets list for the next hop URI.
    //
    //  Parameters:
    //    plststTargets:
    //      The list to use as resolved targets.
    //
    //  Returns:
    //    resS_OK:
    //      Successful execution.
    //    resFE_FAIL:
    //      If no next hop URI has been set.
    //
    //  Description:
    //      Sets the list of resolved targets for the next hop URI.
    //      This method returns an error if the next hop URI has not been set
    //      yet. This method allows the application to manipulate the list of
    //      targets.
    //
    //==========================================================================
    virtual mxt_result SetResolvedTargets(TOA CList<SResolvedTarget>* plststTargets) = 0;


    //==========================================================================
    //==
    //==  GetCurrentTarget
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the current target of the ISipContext.
    //
    //  Parameters:
    //    reTransport:
    //      The currently used transport for this target.
    //    rTargetAddr:
    //      The selected target address.
    //
    //  Returns:
    //    resS_OK:
    //      Successful execution.
    //    resFE_FAIL:
    //      If no next hop URI has been set.
    //
    //
    //  Description:
    //      Gives access to the selected target address and transport of the
    //      ISipContext. Returns an error if none is currently set.
    //
    //==========================================================================
    virtual mxt_result GetCurrentTarget(OUT ESipTransport& reTransport,
                                        OUT CSocketAddr& rTargetAddr) = 0;


    //==========================================================================
    //==
    //==  SetCurrentTarget
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the transport and address to use as selected target.
    //
    //  Parameters:
    //    uIndex:
    //      The index of target to be used.
    //
    //  Returns:
    //    resS_OK:
    //      If the index are valid.
    //    resFE_INVALID_ARGUMENT:
    //      If the specified index value is out of range.
    //    resFE_FAIL:
    //      If no next hop URI has been set and resolved yet.
    //
    //
    //  Description:
    //      This method allows the application to select the target to use among
    //      the ones contained in the list of targets. The application can
    //      access this list by using the method GetResolvedTargets.
    //      This method returns an error if no next hop URI has been set and
    //      resolved.
    //
    //==========================================================================
    virtual mxt_result SetCurrentTargetIndex(IN unsigned int uIndex) = 0;


    //==========================================================================
    //==
    //==  UseHighestPriorityTarget
    //==
    //==========================================================================
    //
    //  Summary:
    //      Selects the highest priority target.
    //
    //  Returns:
    //    resS_OK:
    //      If the highest priority target has been selected.
    //    resFE_FAIL:
    //      If there are no resolved targets.
    //
    //  Description:
    //      This method makes this service select the highest priority target
    //      that is available. It returns an error if there is no target
    //      to use.
    //
    //==========================================================================
    virtual mxt_result UseHighestPriorityTarget() = 0;


    //==========================================================================
    //==
    //==  IsHigherPriorityTarget
    //==
    //==========================================================================
    //
    //  Summary:
    //      Checks if the specified target has a higher priority than the
    //      selected target.
    //
    //  Parameters:
    //    eTransport:
    //      The transport to verify.
    //    rTargetAddr:
    //      The target address to verify.
    //
    //  Returns:
    //    true:
    //      If the specified target address/transport has a higher priority
    //      than the selected target.
    //    false:
    //      If the specified transport-address are not in the list of target
    //      address or if the specified target address/transport has a priority
    //      that is lower than or equal to the current target priority.
    //
    //
    //  Description:
    //      This method returns true if the specified target address and
    //      transport have a higher priority than the currently selected
    //      target.
    //
    //==========================================================================
    virtual bool IsHigherPriorityTarget(IN ESipTransport eTransport,
                                        IN const CSocketAddr& rTargetAddr) = 0;


    //==========================================================================
    //==
    //==  IsHigherPriorityTarget
    //==
    //==========================================================================
    //
    //  Summary:
    //      Checks if the specified target has a higher priority than the
    //      current target.
    //
    //  Parameters:
    //    ruIndex:
    //      The index of the target to check.
    //
    //  Returns:
    //    true:
    //      If the specified target address/transport has a higher priority
    //      than the selected target.
    //    false:
    //      Otherwise.
    //
    //  Description:
    //      This method returns true if the specified target has a higher
    //      priority than the currently selected target.
    //
    //==========================================================================
    virtual bool IsHigherPriorityTarget(IN unsigned int uIndex) = 0;


    //==========================================================================
    //==
    //==  GetCurrentTargetIndex
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the index of the current target of the ISipContext.
    //
    //  Parameters:
    //    ruIndex:
    //      The index of the current target address.
    //
    //  Returns:
    //    resS_OK:
    //      Successful execution.
    //    resFE_FAIL:
    //      If no next hop URI has been set.
    //
    //
    //  Description:
    //      This method returns the index of the current target of the
    //      ISipContext.
    //      This method returns an error if no next hop URI has been set and
    //      resolved.
    //
    //==========================================================================
    virtual mxt_result GetCurrentTargetIndex(OUT unsigned int& ruIndex) = 0;


    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the ISipTargetReuse service manager.
    //
    //  Parameters:
    //    pMgr:
    //      The manager of the service. It can be NULL.
    //
    //  Description:
    //      Sets the manager of this service. The pointer to the manager
    //      can be NULL, in which case no event will be reported.
    //
    //==========================================================================
    virtual void SetManager(IN ISipTargetReuseMgr* pMgr) = 0;


    //==========================================================================
    //==
    //==  SetTargetListModifier
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets a callback function for target list modifications.
    //
    //  Parameters:
    //    pfnNew:
    //      The new callback function for the target list modifications. Must
    //      not be NULL.
    //
    //    opqToPass:
    //      The opaque value that will be passed to the callback function.
    //
    //    rpfnPrevious:
    //      The callback function that was already set in the service. This
    //      callback function should be called by the new callback method.
    //      The previous callback will not be changed if this method fails.
    //      NULL if there was no callback function already set.
    //
    //    ropqToPassToPrevious:
    //      The opaque value that must be passed to rpfnPrevious when it is
    //      called. Will not be changed if this method fails.
    //      This parameter must not be used if rpfnPrevious is NULL.
    //
    //  Returns:
    //    resS_OK:
    //      Successful execution.
    //    resFE_INVALID_ARGUMENT:
    //      pfnNew is NULL.
    //
    //  Description:
    //      Sets a new callback method for target list modifications. The
    //      callback function method will be called, using opqCallbackParameter
    //      as parameter, after an asynchronous resolution has succeeded to
    //      allow another object to modify the record list.
    //
    //      The old callback method returned by this method should be called
    //      with its opaque value in the new callback function.
    //
    //      The change in callback function will only affect the new
    //      transactions. It will not be used for current requests.
    //
    //==========================================================================
    virtual mxt_result SetTargetListModifier(IN mxt_PFNTargetListModifier pfnNew,
                                             IN mxt_opaque opqCallbackParameter,
                                             OUT mxt_PFNTargetListModifier& rpfnPrevious,
                                             OUT mxt_opaque& ropqPreviousParameter) = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipTargetReuseSvc() {}

    // Summary:
    //  Destructor.
    virtual ~ISipTargetReuseSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipTargetReuseSvc(IN const ISipTargetReuseSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipTargetReuseSvc& operator=(IN const ISipTargetReuseSvc& rSrc);


//-- Hidden Data Members
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTARGETREUSESVC_H

