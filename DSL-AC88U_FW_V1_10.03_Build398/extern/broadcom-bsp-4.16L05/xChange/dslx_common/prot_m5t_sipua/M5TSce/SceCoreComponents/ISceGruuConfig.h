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
#ifndef MXG_ISCEGRUUCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEGRUUCONFIG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
//-------------------------
class CSipUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceGruuConfig);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceGruuConfig
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines various Globally Routable User Agent URI (GRUU)
//   related APIs used by components and also available to the application.
//
// Description:
//  This interface defines various Globally Routable User Agent URI (GRUU)
//  related APIs used by components and also available to the application.
//
//  The public and temporary GRUUs are usually configured by a registration
//  component. This works when this configuration object is associated with a
//  registration component. Under such conditions, no user/application
//  intervention is required in order to have GRUU work properly.
//
//  If no registration component is available, it is still possible to have the
//  application configure a public GRUU by using SetPublicGruu.
//
//  When using a registration component, the application may also want to
//  configure the maximum number of temporary GRUUs the system can cache using
//  ConfigureTempGruus.
//
// Location:
//   SceCoreComponents/ISceGruuConfig.h
//
//==============================================================================
class ISceGruuConfig : public IEComUnknown
{
//-- Friend Declarations.
//------------------------

//-- Published Interface.
//------------------------
public:

//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceGruuConfig);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetPublicGruu
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures a public Globally Routable User Agent URI (GRUU).
    //
    //  Parameters:
    //    pGruu:
    //      The GRUU as a pointer to an URI. Ownership of this pointer is TAKEN.
    //      It can be NULL.
    //
    //  Description:
    //      Configures a public GRUU. The GRUU is later used as part of the
    //      Contact header sent for this user. The GRUU mechanism ensures that
    //      the Contact provided to other endpoints is globally routable, or
    //      that other endpoints can send packets to it.
    //
    //      Some components, like the registration component, may be responsible
    //      to set the GRUUs for the user. However it is possible for the
    //      application to manually configure a GRUU if it knows how to build
    //      one and can ensure that it will be globally routable. This may be
    //      the case for some devices like gateways that do not register to a
    //      registrar and that still want to benefit from the GRUU
    //      functionality.
    //
    //      Public GRUUs are exposing the underlying Address of Record
    //      configured in SetUserAddress and thus provide no privacy.
    //
    //      If no public GRUU is configured, it defaults to nothing and will not
    //      be used.
    //
    //  See Also:
    //      SetPrivateGruu, GetPublicGruu, ISceUserConfig::SetUserAddress
    //
    //==========================================================================
    virtual void SetPublicGruu(IN TO CSipUri* pGruu) = 0;

    //==========================================================================
    //==
    //==  GetPublicGruu
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the public Globally Routable User Agent URI (GRUU).
    //
    //  Returns:
    //      The GRUU as a SIP (or SIPS) URI. Can be NULL if no public GRUU was
    //      configured.
    //
    //  Description:
    //      Gets the public GRUU.
    //
    //  See Also:
    //      SetPublicGruu
    //
    //==========================================================================
    virtual const CSipUri* GetPublicGruu() const = 0;


    //==========================================================================
    //==
    //==  AddTempGruu
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a Globally Routable User Agent URI (GRUU) to the list of 
    //      temporary GRUUs.
    //
    //  Parameters:
    //    pGruu:
    //      Pointer to the temporary GRUU. Ownership of this pointer is TAKEN.
    //
    //  Returns:
    //      - resS_OK: The temporary GRUU was added successfully.
    //      - resFE_INVALID_ARGUMENT: Invalid pGruu (== NULL).
    //
    //  Description:
    //      This method adds a temporary GRUU to the list of temporary GRUUs.
    //
    //      The GRUU can later be used as part of the Contact header sent for
    //      this user. The GRUU mechanism ensures that the Contact provided to
    //      other endpoints is globally routable, or that other endpoints can
    //      send packets to it.
    //
    //      Temporary GRUUs are used instead of a public GRUU when privacy is
    //      required. Temporary GRUUs do not expose the underlying AOR to which
    //      it matches.
    //
    //      Temporary GRUUs are usually handled out by a network entity. The
    //      Registration component is responsible to get temporary GRUUs from
    //      the SIP registrar.
    //
    //      More than one temporary GRUU may be configured for a user. The
    //      implementation will try to provide a different temporary GRUU each
    //      time GetTempGruu is called, thus providing a higher level of
    //      privacy. However, depending on the number of temporary GRUUs
    //      available, it is possible that GetTempGruu returns twice the same
    //      GRUU. Implementations wanting to ensure that a different temporary
    //      GRUU is provided when calling GetTempGruu should refresh their
    //      registrations between calls to GetTempGruu. At the application
    //      level, this means refreshing registrations between the use of
    //      components that are configured to require privacy treatment at the
    //      signaling level.
    //
    //  See Also:
    //      SetPublicGruu, GetPublicGruu, ISceUserConfig::SetUserAddress
    //
    //==========================================================================
    virtual mxt_result AddTempGruu(IN TO CSipUri* pGruu) = 0;

    //==========================================================================
    //==
    //==  GetTempGruu
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a temporary Globally Routable User Agent URI (GRUU).
    //
    //  Returns:
    //      The temporary GRUU. Can be NULL if no temporary GRUU is available.
    //
    //  Description:
    //      This method tries to get a new GRUU from the list of unused
    //      temporary GRUUs.
    //    
    //      This method cannot guarantee that the provided GRUU was unused as it
    //      is possible that no more temporary GRUUs are cached. Under such
    //      conditions, the system returns the same temporary GRUU as the last
    //      call to GetTempGruu.
    //
    //      Moreover, it is possible that the number of available temporary
    //      GRUUs is zero if no successful registration was performed yet. A
    //      single temporary GRUU is usually fetched each time a registration
    //      request is sent to the registrar.
    //
    //      This method is usually used by components the application has
    //      configured to provide privacy treatment.
    //
    //  See Also:
    //      ConfigureTempGruus, AddTempGruu, GetPublicGruu, SetPublicGruu
    //
    //==========================================================================
    virtual const CSipUri* GetTempGruu() = 0;
    
    //==========================================================================
    //==
    //==  ConfigureTempGruus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the maximum number of temporary GRUUs the system can
    //      cache.
    //
    //  Parameters:
    //    uMaxTempGruus:
    //      The maximum number of GRUUs the system can cache.
    //
    //  Description:
    //      This configures the maximum number of temporary GRUUs the system
    //      can cache.
    //      
    //      The system will potentially allocate a SIP URI for each cached
    //      temporary GRUU.
    //
    //      This defaults to 10 if not configured by the application. This
    //      configuration can be updated at any time.
    //
    //  See Also:
    //      AddTempGruu, GetTempGruu, SetPublicGruu, GetPublicGruu
    //
    //==========================================================================
    virtual void ConfigureTempGruus(IN uint32_t uMaxTempGruus) = 0;


    //==========================================================================
    //==
    //==  InvalidateAllGruus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Invalidates all public and temporary Globally Routable User Agent
    //      URIs (GRUUs).
    //
    //  Description:
    //      Invalidates all GRUUs for this user.
    //
    //      This is usually used by the registration component associated with
    //      this user when it unregisters or if a registration has failed to be
    //      refreshed in time.
    //
    //  See Also:
    //      SetPublicGruu, AddTempGruu
    //
    //==========================================================================
    virtual void InvalidateAllGruus() = 0;

//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceGruuConfig() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceGruuConfig() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceGruuConfig(IN const ISceGruuConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceGruuConfig& operator=(IN const ISceGruuConfig& rSrc);

//-- Hidden Data Members.
//------------------------
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEGRUUCONFIG_H
