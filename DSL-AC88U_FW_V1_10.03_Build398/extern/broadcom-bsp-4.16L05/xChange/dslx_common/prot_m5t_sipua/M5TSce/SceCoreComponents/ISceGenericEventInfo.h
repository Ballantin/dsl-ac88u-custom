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
#ifndef MXG_ISCEGENERICEVENTINFO_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEGENERICEVENTINFO_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CNameAddr;
class CSceCallerPreferences;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceGenericEventInfo);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceGenericEventInfo
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface presents generic information that can be reported to the
//   application by all M5T SIP Client Engine components when handling incoming
//   requests or responses.
//
// Description:
//  This interface presents generic information that can be reported to the
//  application by all M5T SIP Client Engine components when handling incoming
//  requests or responses.
//
//  When a SIP packet is received by the M5T SIP Client Engine, it is
//  first processed by a core that is common to all components offered by the
//  engine. If this common core wants to report additional information about the
//  incoming packet, it does so by storing the information in this
//  ISceGenericEventInfo interface. Once processed by the common core, the
//  packet is processed by the component itself that is in charge of interpreting
//  it. This is the component that the application built over the M5T SIP Client
//  Engine is using, like the call, notifier or subscriber components. 
//
//  This component may also want to provide more information about the packet to
//  the application. This is done through a component-specific interface that is
//  aggregated by the ISceGenericEventInfo. Whenever the
//  ISceGenericEventInfo aggregates additional component-specific
//  information that must be reported to the application, the component-specific
//  interface will be queriable from this interface. If the component-specific
//  interface is not queriable, then it means that the component had no
//  additional information to report to the application.
//
// Location:
//   SceCoreComponents/ISceGenericEventInfo.h
//
//==============================================================================
class ISceGenericEventInfo : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceGenericEventInfo);
//M5T_INTERNAL_USE_END

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  AddComponent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Aggregates a server event component to the generic server events.
    //
    //  Parameters:
    //    classId:
    //      The class ID to aggregate.
    //
    //  Description:
    //      This method allows a component using the CSceBaseComponent to add
    //      its own event in the form of an aggregated instance.
    //
    //      This method is private to the M5T SIP Client Engine and should not
    //      be used by users of the engine.
    //
    //==========================================================================
    virtual void AddComponent(IN mxt_clsid classId) = 0;
//M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  SetNetworkAssertedIdentity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the trusted network asserted identity associated with the
    //      incoming request.
    //
    //  Parameters:
    //    pNaiSip:
    //      The asserted identity of the user sending the request in SIP URI
    //      format. Can be NULL if pNaiTel is not NULL.
    //
    //    pNaiTel:
    //      The asserted identity of the user sending the request in TEL URI
    //      format. Can be NULL if pNaiSip is not NULL.
    //
    //  Description:
    //      Sets the trusted network asserted identity of the user sending the
    //      request.
    //
    //      Both pNaiSip and pNaiTel can be non NULL if both identities are
    //      asserted by the network.
    //
    //  See Also:
    //      GetNetworkAssertedIdentity,
    //      ISceUserIdentityConfig::SetNetworkAssertedIdentityMode.
    //
    //==========================================================================
    virtual void SetNetworkAssertedIdentity(IN TO CNameAddr* pNaiSip,
                                            IN TO CNameAddr* pNaiTel) = 0;

    //==========================================================================
    //==
    //==  GetNetworkAssertedIdentity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the network asserted identity associated with the request
    //      sender.
    //
    //  Parameters:
    //    rpNaiSip:
    //      Reference to a pointer that will be set to point to the asserted
    //      identity received as a SIP URI. Can be set to NULL if no asserted
    //      SIP identity was received.
    //
    //    rpNaiTel:
    //      Reference to a pointer that will be set to point to the asserted
    //      identity received as a TEL URI. Can be set to NULL if no asserted
    //      TEL identity was received.
    //
    //  Description:
    //      Provides access to the network asserted identity of the user that
    //      sent the request.
    //
    //  See Also:
    //      SetNetworkAssertedIdentity,
    //      ISceUserIdentityConfig::SetNetworkAssertedIdentityMode.
    //
    //==========================================================================
    virtual void GetNetworkAssertedIdentity(OUT const CNameAddr*& rpNaiSip,
                                            OUT const CNameAddr*& rpNaiTel) = 0;

    //==========================================================================
    //==
    //==  SetCallerPreferences
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets caller preferences associated with the incoming request.
    //
    //  Parameters:
    //    pCallerPrefs:
    //      The caller preferences received.
    //
    //  Description:
    //      Sets the caller preferences of the user sending the request.
    //
    //  See Also:
    //      GetCallerPreferences
    //
    //==========================================================================
    virtual void SetCallerPreferences(IN TO CSceCallerPreferences* pCallerPrefs) = 0;

    //==========================================================================
    //==
    //==  GetCallerPreferences
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets caller preferences associated with the incoming request.
    //
    //  Parameters:
    //    rpCallerPrefs:
    //      The caller preferences received. Can be NULL if no preferences
    //      were received.
    //
    //  Description:
    //      Provides access to the caller preferences of the user that
    //      sent the request.
    //
    //  See Also:
    //      SetCallerPreferences
    //
    //==========================================================================
    virtual void GetCallerPreferences(OUT const CSceCallerPreferences*& rpCallerPrefs) = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceGenericEventInfo() {}

    // Summary:
    //  Destructor.
    virtual ~ISceGenericEventInfo() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceGenericEventInfo(IN const ISceGenericEventInfo& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceGenericEventInfo& operator=(IN const ISceGenericEventInfo& rSrc);


//-- Hidden Data Members.
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEGENERICEVENTINFO_H

